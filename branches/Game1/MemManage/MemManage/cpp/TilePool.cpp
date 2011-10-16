


#ifndef _TILE_POOL_CPP_
#define _TILE_POOL_CPP_


#include "../include/TilePool.h"

namespace XM_Tools
{
	template<typename _TLock, typename _TAllocator>
	CTilePool<_TLock, _TAllocator>::CTilePool(_TAllocator &Allocator, unsigned long uDefaultAddSize)
		:m_Allocator(Allocator)
		,m_uDefaultAddSize(uDefaultAddSize)
	{
#ifndef USE_TILE_MANAGE
		return;
#endif
		for (unsigned long i = 0; i < TILE_POOL_NUM; ++i)
		{
			//! �������������
			m_arrMemList[i].uIndex			= i;
			//!	�ܴ�С������������Ϣ��
			m_arrMemList[i].uSize			= 0;
			//! �ֿ�����
			m_arrMemList[i].uTileSum		= 0;
			//! ��ʹ�ÿ���
			m_arrMemList[i].uUseCount		= 0;
			//! �ϴ�����Ŀ���
			m_arrMemList[i].uLastAllocCount	= 0;
		}
	}

	template<typename _TLock, typename _TAllocator>
	CTilePool<_TLock, _TAllocator>::~CTilePool(void)
	{
		Release();	
	}

	/*
	��ʼ������

	*/
	template<typename _TLock, typename _TAllocator>
	bool CTilePool<_TLock, _TAllocator>::InitForSize(unsigned long uSize, unsigned long uProvideCount)
	{
#ifndef USE_TILE_MANAGE
		return true;
#endif
		unsigned long uIdx = GetListIndex(uSize);
		if(0xFFFFFFFF == uIdx || 0 == uProvideCount)
			RETURN_THROW(false);

		tagTileManageInfo &TileManageInfo = m_arrMemList[uIdx];
		//! һ�η���Ŀ������ܳ���0xFFFF
		uProvideCount = (0xFFFF < uProvideCount) ? 0xFFFF : uProvideCount;

		//! ��������
		uProvideCount = SNAP_TO_GRID(uProvideCount, GetDefaultAddCount(uIdx));
		AddNewTile(TileManageInfo, uProvideCount);

		return true;
	}

	//!
	template<typename _TLock, typename _TAllocator>
	void CTilePool<_TLock, _TAllocator>::Release(void)
	{
#ifndef USE_TILE_MANAGE
		return;
#endif
		for (unsigned long i = 0; i < TILE_POOL_NUM; ++i)
		{
			//! ����ڴ�й¶
			MM_ASSERY(0 == m_arrMemList[i].uUseCount);
			//! ��������Ϣ
#ifdef LOG_ALLOC_FREE_INFO

#endif 

			//! �ͷ��ڴ�
#ifdef DESTRUTOR_FREE_SYSMEM
			CListNode<void> *pCListNode = m_arrMemList[i].SysMemList.PopNode();
			while(NULL != pCListNode)
			{
				m_Allocator.Free(pCListNode);
				pCListNode = m_arrMemList[i].SysMemList.PopNode();
			}
#endif
		}
	}

	//! �������ʱ��Ϣ
	template<typename _TLock, typename _TAllocator>
	void CTilePool<_TLock, _TAllocator>::OutInfo(char *pInfoBuf)
	{
#ifdef LOG_ALLOC_FREE_INFO
	#ifndef USE_TILE_MANAGE
		_snprintf(pInfoBuf, INFO_BUF_SIZE, "CTilePool:\r\n	��ǰƬ��û�н����ڴ���䣡\r\n");
	#else
		_snprintf(pInfoBuf, INFO_BUF_SIZE, "CTilePool:\r\n");
		for (unsigned long i = 0; i < TILE_POOL_NUM; ++i)
		{
			_snprintf(&pInfoBuf[strlen(pInfoBuf)], INFO_BUF_SIZE - strlen(pInfoBuf), "	��%u��byte���䡾%u���飬���ġ�%u��KB����ʹ�á�%u���顢\r\n", 
				MIN_GRANULARITY * (i + 1), m_arrMemList[i].uTileSum, m_arrMemList[i].uSize / 1024, m_arrMemList[i].uUseCount);
		}
	#endif
#endif
	}

	/*
	����һ��ָ����С���ڴ�
	*/
	template<typename _TLock, typename _TAllocator>
	void* CTilePool<_TLock, _TAllocator>::Alloc(unsigned long uSize)
	{
#ifndef USE_TILE_MANAGE
		return malloc(uSize);
#endif
		unsigned long uIdx = GetListIndex(uSize);
		if(0xFFFFFFFF == uIdx)
			RETURN_THROW(NULL);

		tagTileManageInfo &TileManageInfo = m_arrMemList[uIdx];
		if(0 == TileManageInfo.FreeList.GetNodeNum())
		{
			if(!AddNewTile(TileManageInfo, GetDefaultAddCount(uIdx)))
				RETURN_THROW(NULL);
		}
		
		CListNode<unsigned char> *pFreeNode = TileManageInfo.FreeList.PopNode();
		MM_ASSERY(NULL != pFreeNode);
		//! ��ʹ�ÿ���
		TileManageInfo.InfoLock.Lock();
		++TileManageInfo.uUseCount;
		TileManageInfo.InfoLock.UnLock();

		unsigned char cInfo = (unsigned char)TileManageInfo.uIndex;
		pFreeNode->SetInfo(cInfo);
		return pFreeNode->GetReBuf();
	}

	/*
	*/
	template<typename _TLock, typename _TAllocator>
	void CTilePool<_TLock, _TAllocator>::Free(void *pMem)
	{
#ifndef USE_TILE_MANAGE
		free(pMem);
		return;
#endif
		//! �ڴ�ƫ�Ƶ��ڵ���Ϣ
		CListNode<unsigned char> *pFreeNode = (CListNode<unsigned char>*)(((char*)pMem) - SNAP_TO_GRID(sizeof(unsigned char), LIST_NODE_GRID_SIZE));
		unsigned long uIdx = pFreeNode->GetInfo();
		if(0xFFFFFFFF == uIdx)
			RETURN_THROW_VOID;
		
		tagTileManageInfo &TileManageInfo = m_arrMemList[uIdx];
		//! ��ʹ�ÿ���
		TileManageInfo.InfoLock.Lock();
		--TileManageInfo.uUseCount;
		TileManageInfo.InfoLock.UnLock();

		m_arrMemList[uIdx].FreeList.PushNode(pFreeNode);
	}

	/*
	�õ�һ���ߴ��Ӧ������
	ʧ�ܷ���0xFFFFFFFF
	*/
	template<typename _TLock, typename _TAllocator>
	unsigned long CTilePool<_TLock, _TAllocator>::GetListIndex(unsigned long uSize)
	{
		if(TILE_MAX_SIZE < uSize || 0 == uSize)
			return 0xFFFFFFFF;
		return uSize / MIN_GRANULARITY - ((0 == uSize % MIN_GRANULARITY) ? 1 : 0);
	}

	/*
	�õ�Ĭ��һ�����ӵĿ���
	*/
	template<typename _TLock, typename _TAllocator>
	unsigned long CTilePool<_TLock, _TAllocator>::GetDefaultAddCount(unsigned long uIndex)
	{
		MM_ASSERY(TILE_POOL_NUM > uIndex);
		return m_uDefaultAddSize / (MIN_GRANULARITY * (uIndex + 1));
	}


	//! �����µĿ��п�
	template<typename _TLock, typename _TAllocator>
	bool CTilePool<_TLock, _TAllocator>::AddNewTile(tagTileManageInfo &TileManageInfo, unsigned long uAddCount)
	{
		MM_ASSERY(GetDefaultAddCount(TileManageInfo.uIndex) >= uAddCount && 0 != uAddCount && 0 == (uAddCount % GetDefaultAddCount(TileManageInfo.uIndex)));
		//! �ڵ㳤�� = ����Ľڵ���Ϣ���� + ���õ��ڴ棨Nextָ�롢�����ڴ泤�ȣ�
		unsigned long uNodeSize = SNAP_TO_GRID(sizeof(unsigned char), LIST_NODE_GRID_SIZE) + max(sizeof(CListNode<unsigned char>), (TileManageInfo.uIndex + 1) * MIN_GRANULARITY);

		//! ���������ڴ�
		char *pMem = (char*)m_Allocator.Alloc(sizeof(CListNode<void>) + uNodeSize * uAddCount);
		if(NULL == pMem)
			return false;

		//! ��ӵ�ϵͳ�ڴ����
		TileManageInfo.SysMemList.PushNode((CListNode<void>*)pMem);
		pMem += sizeof(CListNode<void>);

		//! ��ӵ���������
		for (unsigned long i = 0; i < uAddCount; ++i)
		{
			CListNode<unsigned char> *pFreeNode = (CListNode<unsigned char>*)(pMem + i * uNodeSize);
#ifdef _TEST_ALLOC_TILE_
			static stdext::hash_set<void*> HasMem;
			stdext::hash_set<void*>::iterator itrMem = HasMem.find(pFreeNode);
			if (itrMem != HasMem.end())
			{
				throw;
			}
			HasMem.insert(pFreeNode);
#endif	
			pFreeNode->GetInfo() = (unsigned char)TileManageInfo.uIndex;
			TileManageInfo.FreeList.PushNode(pFreeNode);
		}

		TileManageInfo.InfoLock.Lock();
		{
			//!	�ܴ�С������������Ϣ��
			TileManageInfo.uSize			+= uNodeSize * uAddCount;
			//! �ֿ�����
			TileManageInfo.uTileSum			+= uAddCount;
			//! �ϴ�����Ŀ���
			TileManageInfo.uLastAllocCount	= uAddCount;
		}
		TileManageInfo.InfoLock.UnLock();
		
		return true;
	}
	
};


#endif //! _TILE_POOL_CPP_