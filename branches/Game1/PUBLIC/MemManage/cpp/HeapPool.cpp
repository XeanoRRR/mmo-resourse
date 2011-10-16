


#ifndef _HEAP_POOL_CPP_
#define _HEAP_POOL_CPP_

#include "../include/HeapPool.h"

namespace XM_Tools
{
	template<typename _TLock>
	CHeapPool<_TLock>::CHeapPool(void)
#ifdef LOG_ALLOC_FREE_INFO
		:m_uAllocSize(0)
#endif
	{

	}
	template<typename _TLock>
	CHeapPool<_TLock>::~CHeapPool(void)
	{
		Release();
	}

	//!
	template<typename _TLock>
	void CHeapPool<_TLock>::Release(void)
	{
		MM_ASSERY(0 == m_UsingList.GetNodeNum());

#ifdef DESTRUTOR_FREE_SYSMEM
		CListBNode<tagHeapMemInfo> *pBNode = m_FreeList.PopNode();
		while (NULL != pBNode)
		{
			free(pBNode);
			pBNode = m_FreeList.PopNode();
		}
#endif
	}

	//! �������ʱ��Ϣ
	template<typename _TLock>
	void CHeapPool<_TLock>::OutInfo(char *pInfoBuf)
	{
#ifdef LOG_ALLOC_FREE_INFO
		m_InfoLock.Lock();
		if(0 == m_mapAllocInfo.size())
		{
			sprintf(pInfoBuf, "CHeapPool:\r\n	��ǰ�ڴ������û�н����ڴ���䣡\r\n");
		}
		else
		{
			sprintf(pInfoBuf, "CHeapPool:\r\n	��ǰ��ϵͳ������ڴ������\r\n	�ܹ������ˡ�%u��KB���ڴ棬���ơ�%u���飬���С�%u��������ʹ�á���%u������ʱ���桢���顾%u��KB����С�顾%u��KB\r\n",
				m_uAllocSize / 1024, m_UsingList.GetNodeNum() + m_FreeList.GetNodeNum(), m_UsingList.GetNodeNum(), m_FreeList.GetNodeNum(), (--m_mapAllocInfo.end())->first / 1024, m_mapAllocInfo.begin()->first / 1024);
		}
		m_InfoLock.UnLock();
#endif 
	}

	//!		
	template<typename _TLock>
	void* CHeapPool<_TLock>::Alloc(unsigned long uSize)
	{
		CListBNode<tagHeapMemInfo> *pBNode = m_FreeList.GetLikestNode(tagHeapMemInfo(uSize));
		if(NULL == pBNode)
		{
			pBNode = (CListBNode<tagHeapMemInfo>*)malloc(uSize + sizeof(CListBNode<tagHeapMemInfo>));
			pBNode->GetInfo().uSize = uSize;
#ifdef LOG_ALLOC_FREE_INFO
			m_InfoLock.Lock();
			m_uAllocSize += uSize;
			if(m_mapAllocInfo.end() == m_mapAllocInfo.find(uSize))
				m_mapAllocInfo[uSize] = 1;
			else
				++m_mapAllocInfo[uSize];
			m_InfoLock.UnLock();
#endif
		}

		m_UsingList.PushNode(pBNode);
		void *pRe = NULL;
		if (NULL != pBNode)
			pRe = pBNode + 1;
		return pRe;
	}

	//!		
	template<typename _TLock>
	void CHeapPool<_TLock>::Free(void *pMem)
	{
		CListBNode<tagHeapMemInfo> *pBNode = ((CListBNode<tagHeapMemInfo>*)pMem) - 1;
		MM_ASSERY(m_UsingList.Find(pBNode));
		m_UsingList.PopNode(pBNode);
		m_FreeList.PushNode(pBNode);
	}
};
#endif //! _HEAP_POOL_CPP_