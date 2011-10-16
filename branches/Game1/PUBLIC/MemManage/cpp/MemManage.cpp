
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
		for (unsigned long i = 0; i < TILE_POOL_NUM; ++i)
		{
			m_TilePool.InitForSize(MIN_GRANULARITY * (i + 1), 1024 * 1024 / (MIN_GRANULARITY * (i + 1)) / SHRINK_SCALE);
		}

		m_BlockPool.Init();

		return 0;
	}

	//! �ͷ�
	template<typename _TLock>
	void CMemManage<_TLock>::Release(void)
	{
#ifdef _LOG_ALLOC_FILE_INFO_
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
				strTempInfo += std::string("��") + ite->second.c_str() + "����������ڴ�û���ͷţ�����";
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
	void CMemManage<_TLock>::OutRunTimeObjInfo(void)
	{
#ifdef _LOG_ALLOC_FILE_INFO_
		if(NULL == m_OutInfoFun || NULL == m_pOutFile)
			return;
		std::string strTempInfo("*********************************�ڴ������ʱ��Ϣ*********************************\r\n");
		//! OutInfo���жϴ���Ļ����С�����Ҫ����һ���ϴ�����ݿ飬�������
		char szBsInfo[10240] = {0};
		m_TilePool.OutInfo(szBsInfo);
		strTempInfo += szBsInfo;
		m_BlockPool.OutInfo(szBsInfo);
		strTempInfo += szBsInfo;
		m_HeapPool.OutInfo(szBsInfo);
		strTempInfo += szBsInfo;

		strTempInfo += "******************************************************************\r\n";
		m_OutInfoFun(m_pOutFile, strTempInfo.c_str());
#endif
	}

#ifdef _LOG_ALLOC_FILE_INFO_
	//! �õ�һ���ڴ�
	template<typename _TLock>
	void* CMemManage<_TLock>::AsyncAlloc(unsigned long lSize, const char *pInfo)
	{
		if(MAX_GRANULARITY < lSize)
			RETURN_THROW(NULL);

		void *pRe = NULL;

		if(TILE_MAX_SIZE >= lSize)
			pRe = m_TilePool.Alloc(lSize);
		else if (BLOCK_MAX_ALLOC_SIZE >= lSize)
			pRe = m_BlockPool.Alloc(lSize);
		else
			pRe = m_HeapPool.Alloc(lSize);

		if(NULL != pRe)
		{
			m_InfoLock.Lock();
			m_mapAllocInfo[pRe] = pInfo;
			m_InfoLock.UnLock();
		}
	#ifdef FILL_MEM
		if(NULL != pRe)
			memset(pRe, FILL_VALUE, lSize);
	#endif
		return pRe;
	}

#else

	//! �õ�һ���ڴ�
	template<typename _TLock>
	void* CMemManage<_TLock>::AsyncAlloc(unsigned long lSize)
	{
		if(MAX_GRANULARITY < lSize)
			RETURN_THROW(NULL);

		void *pRe = NULL;

		if(TILE_MAX_SIZE >= lSize)
			pRe = m_TilePool.Alloc(lSize);
		else if (BLOCK_MAX_ALLOC_SIZE >= lSize)
			pRe = m_BlockPool.Alloc(lSize);
		else
			pRe = m_HeapPool.Alloc(lSize);

	#ifdef FILL_MEM
		if(NULL != pRe)
			memset(pRe, FILL_VALUE, lSize);
	#endif
		return pRe;
	}

	
#endif //! _LOG_ALLOC_FILE_INFO_


	//! �ͷ�һ���ڴ�
	template<typename _TLock>
	void CMemManage<_TLock>::AsyncFree(void* pBuf, unsigned long lSize)
	{
		if(TILE_MAX_SIZE >= lSize)
			m_TilePool.Free(pBuf);
		else if (BLOCK_MAX_ALLOC_SIZE >= lSize)
			m_BlockPool.Free(pBuf);
		else
			m_HeapPool.Free(pBuf);
#ifdef _LOG_ALLOC_FILE_INFO_
		m_InfoLock.Lock();
		m_mapAllocInfo.erase(pBuf);
		m_InfoLock.UnLock();
#endif
	}


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