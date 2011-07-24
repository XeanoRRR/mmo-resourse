//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   BufferAlloc.cpp
//  Version     :   1.0
//  Creater     :   Freeway Chen
//  Date        :   2003-6-26 15:04:15
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <crtdbg.h>
#include "BufferAlloc.h"

#include <vector>
using namespace std;

#define defMAX_BUFFER_ALLOC_ITEM_NUM    64

long g_lBufferAllocInitCount = 0;
CRITICAL_SECTION g_BufferAllocCrit;
int g_BufferAllocCritInitFlag = false;

HANDLE g_hBufferAllocHeap = NULL;

// �����������ͷŽڵ����Ŀ
long g_lMaxFreeNodeCount = 1024 * 64;
long g_lFreeNodeCount    = 0;

// ���� �� n ������ָ���һ����̬���� 
typedef vector<unsigned char *> BufferAllocFreeItemType;

// �������һ���������ݵ����飬�����ÿһ���ʾ��ָ�� ��n K��һ����̬����
// Ϊʲô���þ�̬�������飬����ʹ�ö�̬���䣬��Ҫ�ǵ�����Щȫ�ֵ����Ѿ�ʹ�ø����飬�������黹û�г�ʼ��
BufferAllocFreeItemType *g_pBufferAllocFreeArray = NULL;

int _BufferAlloc_Init_Internal()
{
    int nResult = false;
    
    InitializeCriticalSection(&g_BufferAllocCrit);
    g_BufferAllocCritInitFlag = true;

    EnterCriticalSection(&g_BufferAllocCrit);

    g_hBufferAllocHeap = HeapCreate(HEAP_NO_SERIALIZE, 1024 * 1024 * 4, 0);
    if (!g_hBufferAllocHeap)
        goto Exit0;

    try
    {
        g_pBufferAllocFreeArray = new BufferAllocFreeItemType[defMAX_BUFFER_ALLOC_ITEM_NUM];
    }
    catch (...) 
    {
        goto Exit0;
    }

    nResult = true;
Exit0:

    if (!nResult)
    {
        if (g_hBufferAllocHeap)
        {
            HeapDestroy(g_hBufferAllocHeap);
            g_hBufferAllocHeap = NULL;
        }
    }

    LeaveCriticalSection(&g_BufferAllocCrit);

    if (!nResult)
    {
        if (g_BufferAllocCritInitFlag)
        {
            DeleteCriticalSection(&g_BufferAllocCrit);
            g_BufferAllocCritInitFlag = false;
        }
    }

    return nResult;
}

int _BufferAlloc_UnInit_Internal()
{
    if (!g_BufferAllocCritInitFlag)
        return true;

    EnterCriticalSection(&g_BufferAllocCrit);

    if (g_pBufferAllocFreeArray)
    {
        for (int i = 0; i < defMAX_BUFFER_ALLOC_ITEM_NUM; i++)
        {
            BufferAllocFreeItemType::iterator pPos;
            for (pPos = g_pBufferAllocFreeArray[i].begin(); pPos != g_pBufferAllocFreeArray[i].end(); pPos++)
            {
                HeapFree(g_hBufferAllocHeap, HEAP_NO_SERIALIZE, *pPos);
                *pPos = NULL;
            }
            
            g_pBufferAllocFreeArray[i].clear();
        }

        delete []g_pBufferAllocFreeArray;
        g_pBufferAllocFreeArray = NULL;
    }

    LeaveCriticalSection(&g_BufferAllocCrit);

    if (g_hBufferAllocHeap)
    {
        HeapDestroy(g_hBufferAllocHeap);
        g_hBufferAllocHeap = NULL;
    }

    DeleteCriticalSection(&g_BufferAllocCrit);
    g_BufferAllocCritInitFlag = false;

    return false;
}

int BufferAlloc_IncMaxFreeNodeCount(long lCount)
{
    EnterCriticalSection(&g_BufferAllocCrit);
    
    g_lMaxFreeNodeCount += lCount;
    
    LeaveCriticalSection(&g_BufferAllocCrit);

    return true;
}

int BufferAlloc_DecMaxFreeNodeCount(long lCount)
{
    EnterCriticalSection(&g_BufferAllocCrit);
    
    g_lMaxFreeNodeCount -= lCount;
    
    LeaveCriticalSection(&g_BufferAllocCrit);

    return true;
}

void *BufferAlloc_Alloc(size_t uSize)
{
    _ASSERT(uSize > 0);
    if (uSize == 0)
        return NULL;

    // ��������ֽ�ת��Ϊ�� K Ϊ��λ
    size_t uAllocKSize = (uSize + (1024 - 1)) / 1024;

    void *pvResult = NULL;
    
    // ���ڿ���������±�� 0 ��ʼ�������Ҫ - 1
    size_t uAllocFreeIndex = uAllocKSize - 1;

    EnterCriticalSection(&g_BufferAllocCrit);

    if (uAllocKSize > defMAX_BUFFER_ALLOC_ITEM_NUM)
    {
        // �����������ݴ�С����  defMAX_BUFFER_ALLOC_ITEM_NUM(64) K��ֱ����ϵͳ����
        pvResult = HeapAlloc(g_hBufferAllocHeap, HEAP_NO_SERIALIZE, uAllocKSize * 1024);
        goto Exit1;
    }

    if (g_lFreeNodeCount == 0)
    {
        // ��������б���û�ж���Ľڵ㣬��ֱ����ϵͳ�����ڴ�
        pvResult = HeapAlloc(g_hBufferAllocHeap, HEAP_NO_SERIALIZE, uAllocKSize * 1024);
        goto Exit1;
    }

    if (g_pBufferAllocFreeArray[uAllocFreeIndex].empty())
    {
        // ����Ҳ������ʵĿռ䣬��ֱ���������
        pvResult = HeapAlloc(g_hBufferAllocHeap, HEAP_NO_SERIALIZE, uAllocKSize * 1024);
        goto Exit1;
    }

    // ����п��еĻ�����

    g_lFreeNodeCount--; // ���ٿ��л������ļ���
    //InterlockedDecrement(&g_lFreeNodeCount);

    
    pvResult = g_pBufferAllocFreeArray[uAllocFreeIndex].back();
    g_pBufferAllocFreeArray[uAllocFreeIndex].pop_back();

Exit1:
    LeaveCriticalSection(&g_BufferAllocCrit);


    return pvResult;

}

// �ͷ� uSize ��С�Ļ�����
int BufferAlloc_Free(void *pvBuffer, size_t uSize)
{
    _ASSERT(pvBuffer);
    if (!pvBuffer)
        return true;

    _ASSERT(uSize > 0);
    if (uSize == 0)
        return true;

    // ��������ֽ�ת��Ϊ�� K Ϊ��λ
    size_t uAllocKSize = (uSize + (1024 - 1)) / 1024;

    int nResult = false;

    // ���ڿ���������±�� 0 ��ʼ�������Ҫ - 1
    size_t uAllocFreeIndex = uAllocKSize - 1;

    EnterCriticalSection(&g_BufferAllocCrit);

    if (uAllocKSize > defMAX_BUFFER_ALLOC_ITEM_NUM)
    {
        // �����������ݴ�С����  defMAX_BUFFER_ALLOC_ITEM_NUM(64) K��ֱ���ͷ��ڴ�
        HeapFree(g_hBufferAllocHeap, HEAP_NO_SERIALIZE, pvBuffer);
        goto Exit1;
    }

    if (g_lFreeNodeCount >= g_lMaxFreeNodeCount)
    {
        // ��������б����Ѿ��ﵽ���ֵ��ֱ���ͷ��ڴ�
        HeapFree(g_hBufferAllocHeap, HEAP_NO_SERIALIZE, pvBuffer);
        goto Exit1;
    }

    try
    {
        g_pBufferAllocFreeArray[uAllocFreeIndex].push_back((unsigned char *)pvBuffer);
    }
    catch (...)
    {
        goto Exit0;
    }
                    
    g_lFreeNodeCount++; // ���ӿ��л������ļ���
    //InterlockedIncrement(&g_lFreeNodeCount);

Exit1:
    nResult = true;
Exit0:
    
    if (!nResult)
    {
        // ���л��������ò��ɹ�����ֱ���ͷ��ڴ�
        HeapFree(g_hBufferAllocHeap, HEAP_NO_SERIALIZE, pvBuffer);
        nResult = true;
    }
    LeaveCriticalSection(&g_BufferAllocCrit);

    return nResult;
}


class KHeap 
{
private:
    HANDLE m_hHeap;

    #ifdef _DEBUG
    long   m_lAllocCount;
    long   m_lFreeCount;
    #endif

public:
    KHeap(size_t uInitialSize = 0); 
    ~KHeap();
    
    void *Alloc(size_t uSize);
    void Free(void *pvBuffer, size_t uSize);
};

inline KHeap::KHeap(size_t uInitialSize)
{
    #ifdef _DEBUG
    m_lAllocCount = 0;
    m_lFreeCount  = 0;
    #endif

    m_hHeap = HeapCreate(0, uInitialSize, 0);

    _ASSERT(m_hHeap);
}

inline KHeap::~KHeap()
{
    #ifdef _DEBUG
    _ASSERT(m_lAllocCount == m_lFreeCount);
    #endif

    if (m_hHeap)
    {
        HeapDestroy(m_hHeap);
        m_hHeap = NULL;
    }
}

inline void *KHeap::Alloc(size_t uSize)
{
    if (!m_hHeap)
        return NULL;
        
    #ifdef _DEBUG
    InterlockedIncrement(&m_lAllocCount);
    #endif

    return HeapAlloc(m_hHeap, 0, uSize);
}

inline void KHeap::Free(void *pvBuffer, size_t uSize)
{
    if (!m_hHeap)
    {
        _ASSERT(false);
        return;
    }

    uSize = uSize;      // make compiler no warning
    
    HeapFree(m_hHeap, 0, pvBuffer);

    #ifdef _DEBUG
    InterlockedIncrement(&m_lFreeCount);
    #endif
}
