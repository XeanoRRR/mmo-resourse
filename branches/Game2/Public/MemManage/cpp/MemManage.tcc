
#ifndef _MEM_MANAGE_CPP_
#define _MEM_MANAGE_CPP_


#include "../include/MemManage.h"

namespace XM_Tools
{
    template<typename _TLock>
    CMemManage<_TLock>::CMemManage(void)
        :m_HeapPool()
        ,m_BlockPool(m_HeapPool)
        ,m_TilePool(m_HeapPool)
        ,m_OutInfoFun(NULL)
        ,m_pOutFile(NULL)
    {
        Init();
    }

    template<typename _TLock>
    CMemManage<_TLock>::~CMemManage(void)
    {
        Release();
    }

    //! ��ʼ��
    template<typename _TLock>
    unsigned long CMemManage<_TLock>::Init(void)
    {

#ifndef USE_C_LIB_ALLOC
#ifdef USE_TILE_MANAGE
        unsigned long TileInit[TILE_POOL_NUM] = 
        {
            //! 0~7
            1, 4, 2, 1, 1, 1, 1, 1,
            //! 8~15
            1, 1, 1, 1, 1, 1, 1, 1,
            //! 16~23
            1, 1, 1, 1, 1, 1, 1, 1,
            //! 24~31
            1, 1, 1, 1, 1, 1, 1, 1,
        };
        for (unsigned long i = 1; i < TILE_POOL_NUM; ++i)
        {
            m_TilePool.InitForSize(MIN_GRANULARITY * (i + 1), TileInit[i] * TILE_DEFAULT_ADD_SIZE / (MIN_GRANULARITY * (i + 1)));
        }
#endif

#ifdef USE_BLOCK_MANAGE
        m_BlockPool.Init();
#endif
#endif
        return 0;
    }

    //! �ͷ�
    template<typename _TLock>
    void CMemManage<_TLock>::Release(void)
    {
#ifdef LOG_ALLOC_FREE_INFO
        if(NULL == m_OutInfoFun || NULL == m_pOutFile)
            return;

        std::string strTempInfo("*********************************�ڴ��������Ϣ*********************************\r\n");
        char szBsInfo[128] = {0};
        sprintf(szBsInfo, "��ǰ�С�%u�����ڴ�й¶����ϸ��Ϣ��\r\n", (unsigned long)m_mapAllocInfo.size());
        strTempInfo += szBsInfo;
        if(0 < m_mapAllocInfo.size())
        {
            //! ��ֹй¶����ʱд�ļ�������һ��ֻдһ����������Ϣ
            unsigned long uWriteNum = 0;
            for (std::map<void*, std::string>::iterator ite = m_mapAllocInfo.begin(); m_mapAllocInfo.end() != ite; ++ite)
            {
                char szBufInfo[512] = {0};
                strTempInfo += std::string("��") + ite->second.c_str() + "����������ڴ�û���ͷţ�����\r\n";
                ++uWriteNum;

                //! ����200����Ϣ��дһ��
                if(200 <= uWriteNum)
                {
                    m_OutInfoFun(m_pOutFile, strTempInfo.c_str());
                    strTempInfo.clear();
                    uWriteNum = 0;
                }
            }
        }

        strTempInfo += "������Ϣ\r\n";
        m_OutInfoFun(m_pOutFile, strTempInfo.c_str());

        strTempInfo.clear();
        m_InfoLock.Lock();
        m_MemInfo.OutInfo(strTempInfo, true);
        m_InfoLock.UnLock();
        strTempInfo += "******************************************************************\r\n";
        m_OutInfoFun(m_pOutFile, strTempInfo.c_str());
#endif

        m_TilePool.Release();
        m_BlockPool.Release();
        m_HeapPool.Release();
    }

    //! ������Ϣ������������·��
    template<typename _TLock>
    void CMemManage<_TLock>::SetOutInfoSetup(OutInfoFun pOutInfoFun, const char *pFileName)
    {
        m_OutInfoFun = pOutInfoFun;
        m_pOutFile = pFileName;
    }

    //! ����ʱ��Ϣ���
    template<typename _TLock>
    void CMemManage<_TLock>::OutRunTimeObjInfo(bool Detailed)
    {
#ifdef LOG_ALLOC_FREE_INFO
        if(NULL == m_OutInfoFun || NULL == m_pOutFile)
            return;

        std::string strTempInfo("*********************************�ڴ������ʱ��Ϣ*********************************\r\n");
        //! OutInfo���жϴ���Ļ����С�����Ҫ����һ���ϴ�����ݿ飬�������
        char szBsInfo[INFO_BUF_SIZE] = {0};
        m_TilePool.OutInfo(szBsInfo);
        strTempInfo += szBsInfo;
        m_BlockPool.OutInfo(szBsInfo);
        strTempInfo += szBsInfo;
        m_HeapPool.OutInfo(szBsInfo);
        strTempInfo += szBsInfo;

        strTempInfo += "������Ϣ\r\n";
        m_OutInfoFun(m_pOutFile, strTempInfo.c_str());

        strTempInfo.clear();
        m_InfoLock.Lock();
        m_MemInfo.OutInfo(strTempInfo, Detailed);
        m_InfoLock.UnLock();

        strTempInfo += "******************************************************************\r\n";
        m_OutInfoFun(m_pOutFile, strTempInfo.c_str());
#endif
    }

#ifdef LOG_ALLOC_FREE_INFO
    //! �õ�һ���ڴ�
    template<typename _TLock>
    void* CMemManage<_TLock>::AsyncAlloc(unsigned long lSize, const char *pInfo)
    {
        lSize += sizeof(long);
        if(MAX_GRANULARITY < lSize)
            RETURN_THROW(NULL);

        void *pRe = NULL;

        LARGE_INTEGER LargeIntegerBegin;
        CMemInfo::GetTimeFrequencyCount(&LargeIntegerBegin);

#ifdef USE_C_LIB_ALLOC
        pRe = malloc(lSize);
#else
        if(TILE_MAX_SIZE >= lSize)
            pRe = m_TilePool.Alloc(lSize);
        else if (BLOCK_MAX_ALLOC_SIZE >= lSize)
            pRe = m_BlockPool.Alloc(lSize);
        else
            pRe = m_HeapPool.Alloc(lSize);
#endif //! USE_C_LIB_ALLOC

        LARGE_INTEGER LargeIntegerEnd;
        CMemInfo::GetTimeFrequencyCount(&LargeIntegerEnd);
        LargeIntegerEnd.QuadPart = LargeIntegerEnd.QuadPart - LargeIntegerBegin.QuadPart;
        m_InfoLock.Lock();
        m_MemInfo.LogMemOptInfo(true, lSize, LargeIntegerEnd);
        m_InfoLock.UnLock();

        if(NULL != pRe)
        {
#ifdef FILL_MEM
            memset(pRe, FILL_VALUE, lSize);
#endif
            //! ������Ϣ
            long *pTmp = (long*)pRe;
            pTmp[0] = lSize - sizeof(long);
            pRe = ++pTmp;

            m_InfoLock.Lock();
            m_mapAllocInfo[pRe] = pInfo;
            m_InfoLock.UnLock();	
            //! ������Ϣ-END
        }

        return pRe;
    }

    //! �ͷ�һ���ڴ�
    template<typename _TLock>
    void CMemManage<_TLock>::AsyncFree(void* pBuf, unsigned long lSize, const char *pInfo)
    {
        //! ������Ϣ
        m_InfoLock.Lock();
        std::map<void*, std::string>::iterator ite = m_mapAllocInfo.find(pBuf);
        if(m_mapAllocInfo.end() == ite)
        {
            if(NULL != m_OutInfoFun && NULL != m_pOutFile)
            {
                char szTmp[1024] = {0};
                sprintf(szTmp, "ERROR����������һ�����ڴ��ָ�롾%u�����ͷţ�λ�á�%s��\r\n", (unsigned long)pBuf, pInfo);
                m_OutInfoFun(m_pOutFile, szTmp);
            }
            m_InfoLock.UnLock();
            return;
        }

        long *pTmp = (long*)pBuf;
        --pTmp;
        if(pTmp[0] != lSize)
        {
            if(NULL != m_OutInfoFun && NULL != m_pOutFile)
            {
                char szTmp[1024] = {0};
                sprintf(szTmp, "ERROR����������һ���ͷŴ�С������С������ָ�롾%u�����ͷţ�������Ϣ��%s�����ͷ���Ϣ��%s��\r\n", (unsigned long)pBuf, ite->second.c_str(), pInfo);
                m_OutInfoFun(m_pOutFile, szTmp);
            }
            lSize = pTmp[0];
        }
        pBuf = pTmp;

        m_mapAllocInfo.erase(ite);
        m_InfoLock.UnLock();
        //! ������Ϣ-END

        lSize += sizeof(long);

        LARGE_INTEGER LargeIntegerBegin;
        CMemInfo::GetTimeFrequencyCount(&LargeIntegerBegin);

#ifdef USE_C_LIB_ALLOC
        free(pBuf);
#else
        if(TILE_MAX_SIZE >= lSize)
            m_TilePool.Free(pBuf);
        else if (BLOCK_MAX_ALLOC_SIZE >= lSize)
            m_BlockPool.Free(pBuf);
        else
            m_HeapPool.Free(pBuf);
#endif //! USE_C_LIB_ALLOC

        LARGE_INTEGER LargeIntegerEnd;
        CMemInfo::GetTimeFrequencyCount(&LargeIntegerEnd);
        LargeIntegerEnd.QuadPart = LargeIntegerEnd.QuadPart - LargeIntegerBegin.QuadPart;
        m_InfoLock.Lock();
        m_MemInfo.LogMemOptInfo(false, lSize, LargeIntegerEnd);
        m_InfoLock.UnLock();
    }

#else

    //! �õ�һ���ڴ�
    template<typename _TLock>
    void* CMemManage<_TLock>::AsyncAlloc(unsigned long lSize)
    {
        if(MAX_GRANULARITY < lSize)
            RETURN_THROW(NULL);

        void *pRe = NULL;

#ifdef USE_C_LIB_ALLOC
        pRe = malloc(lSize);
#else
        if(TILE_MAX_SIZE >= lSize)
            pRe = m_TilePool.Alloc(lSize);
        else if (BLOCK_MAX_ALLOC_SIZE >= lSize)
            pRe = m_BlockPool.Alloc(lSize);
        else
            pRe = m_HeapPool.Alloc(lSize);
#endif //! USE_C_LIB_ALLOC

#ifdef FILL_MEM
        if(NULL != pRe)
            memset(pRe, FILL_VALUE, lSize);
#endif
        return pRe;
    }

    //! �ͷ�һ���ڴ�
    template<typename _TLock>
    void CMemManage<_TLock>::AsyncFree(void* pBuf, unsigned long lSize)
    {
#ifdef USE_C_LIB_ALLOC
        free(pBuf);
#else
        if(TILE_MAX_SIZE >= lSize)
            m_TilePool.Free(pBuf);
        else if (BLOCK_MAX_ALLOC_SIZE >= lSize)
            m_BlockPool.Free(pBuf);
        else
            m_HeapPool.Free(pBuf);
#endif //! USE_C_LIB_ALLOC
    }

#endif //! LOG_ALLOC_FREE_INFO

    /*
    ============================================================================
    ============================================================================
    �ڴ������
    ============================================================================
    ============================================================================
    */
    //!							��ȡ�ڴ�������
    template<typename _TLock>
    CMemManage<_TLock>& GetMemManage(void)
    {
        static CMemManage<_TLock> c_MemManage;
        return c_MemManage;
    }
};
#endif //! _MEM_MANAGE_CPP_