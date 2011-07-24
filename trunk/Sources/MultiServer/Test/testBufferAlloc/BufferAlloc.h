//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   BufferAlloc.h
//  Version     :   1.0
//  Creater     :   Freeway Chen 
//  Date        :   2003-6-26 15:03:30
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _BUFFERALLOC_H
#define _BUFFERALLOC_H  1

#include "windows.h"


// ===================================================================================
// �ڲ�ʹ������
extern long g_lBufferAllocInitCount;

int _BufferAlloc_Init_Internal();
int _BufferAlloc_UnInit_Internal();


// ===================================================================================
// ����ĳ���ӿ�

inline int BufferAlloc_Init()
{
    if (InterlockedIncrement(&g_lBufferAllocInitCount) == 1)
    {
        return _BufferAlloc_Init_Internal();
    }

    return true;
}

inline int BufferAlloc_UnInit()
{
    if (InterlockedDecrement(&g_lBufferAllocInitCount) == 0)
    {
        return _BufferAlloc_UnInit_Internal();
    }

    return true;
}

// �������Ӵ�ŵ������л���������Ŀ
int BufferAlloc_IncMaxFreeNodeCount(long lCount);

// �������ٴ�ŵ������л���������Ŀ
int BufferAlloc_DecMaxFreeNodeCount(long lCount);

// ���� uSize ��С�Ļ�����
void *BufferAlloc_Alloc(size_t uSize);

// �ͷ� uSize ��С�Ļ�����
int BufferAlloc_Free(void *pvBuffer, size_t uSize);

// ===================================================================================


#endif  // _BUFFERALLOC_H
