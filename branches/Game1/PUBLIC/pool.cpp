#include "stdafx.h"
#include "pool.h"

#pragma warning(disable:4311;disable:4291)



#pragma region �����ʼ��


//! ���캯��
//!	��ʼ����Ԥ�����ڴ�
MemPool::MemPool(VOID)
{

}
//! ��������
MemPool::~MemPool(VOID)
{
	Release();
}


//! Ĭ�ϳ�ʼ������
//! ��Ĭ�ϵķ�ʽԤ�����ڴ�
VOID MemPool::DefaultInit(VOID)
{
	for(LONG i = 0; i < MAX_TILE_BUF_SZIE; ++i)
	{
		AddBlock(i);
	}
}

//! ָ��һ���ߴ���ڴ�Ԥ�ȷ�����ٿ�
BOOL MemPool::SetInitialTileNum(LONG lSize, LONG lNum)
{
	return AddBlock(GetSizeIndex(lSize), lNum);
}

//! �ͷ������ڴ�
VOID MemPool::Release(VOID)
{
#ifdef MEM_POOL_DEBUG
	MyAssert(0 == m_setBigMem.size());
#endif
	for(LONG i = 0; i < MAX_TILE_BUF_SZIE; ++i)
	{
		FreeAllBlockByIndex(i);
	}
}


#pragma  endregion


#pragma region ����Ӧ��

//! �õ�һ��ָ����С���ڴ�
//! ���ڴ汻0���
VOID* MemPool::Alloc(LONG lSize)
{
	if(MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE < lSize)
	{
		LPVOID pBigMem = malloc(lSize);
#ifdef MEM_POOL_DEBUG
		MyAssert(m_setBigMem.end() == m_setBigMem.find(pBigMem));
		m_setBigMem.insert(pBigMem);
#endif
		return pBigMem;
	}
	else
	{
		LONG lIndex = GetSizeIndex(lSize);
		MyAssert(-1 != lIndex);
		if( 0 == m_arrFreeList[lIndex]._TileFreeList.Size())
		{
			//! ��������ڴ����꣬���ٷ����µĴ��
			if(!AddBlock(lIndex))
			{
				//! �ڴ�ľ�
				return NULL;
			}
		}

		//! �ӿ���������ɾ��һ���ڵ㣬��0���ڵ�������ڴ�
		VOID *pReBuf = m_arrFreeList[lIndex]._TileFreeList.PopNode()->GetTile(); 
#ifdef FILL_MEM
		memset(pReBuf, FILL_VALUE, (lIndex * MIN_BUF_SIZE_DIFFERENCE));
#endif

		return pReBuf;
	}
}


//! �ͷ�һ��ָ����С���ڴ�
BOOL MemPool::Free(VOID *pBuf, LONG lSize)
{
	if(MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE < lSize)
	{
		try
		{
#ifdef MEM_POOL_DEBUG
			MyAssert(m_setBigMem.end() != m_setBigMem.find(pBuf));
			m_setBigMem.erase(pBuf);
#endif
			free(pBuf);
		}
		catch (...)
		{
			MyAssert(false);
			return FALSE;
		}
		return TRUE;
	}

	LONG lIndex = GetSizeIndex(lSize);
	MyAssert(-1 != lIndex);
	tagHeap &MemHeap = m_arrFreeList[lIndex];

	//! ����ͷŵ��ڴ��Ƿ��Ƕ�������ȥ��
#ifdef MEM_POOL_DEBUG	
	{
		ListNode<tagBlockNode> *pCurrNode = MemHeap._BlockList.GetHead();
		BOOL bFind = FALSE;
		while(NULL != pCurrNode)
		{
			LPBYTE pBegin = (LPBYTE)pCurrNode->GetTile();
			LPBYTE pEnd = pBegin + pCurrNode->GetNodeData()->lBlockSize - ListNode<tagBlockNode>::GetAddedSize();
			if((LONG)pBegin < (LONG)pBuf && (LONG)pEnd > (LONG)pBuf)
			{
				bFind = TRUE;
				break;
			}
			pCurrNode = pCurrNode->GetNext();
		}

		if(!bFind)MyAssert(false);
	}
#endif

	//! ���ڴ�Ƭ�ڵ�黹��������
	LPBYTE pByteBuf = (LPBYTE)pBuf;
	pByteBuf = pByteBuf - ListNode<VOID>::GetAddedSize();
	MemHeap._TileFreeList.PushNode((ListNode<VOID>*)pByteBuf);

	return TRUE;
}


#pragma  endregion


#pragma region ˽�г�Ա


//! Ϊĳһ�ߴ��ڴ�����һ���ڴ�
BOOL MemPool::AddBlock(LONG lIndex, LONG lTileNum)
{
	if(MAX_TILE_BUF_SZIE <= lIndex || 0 > lIndex) return FALSE;
	tagHeap &MemHeap = m_arrFreeList[lIndex];

	LONG lTileSize = MIN_BUF_SIZE_DIFFERENCE * (lIndex + 1);

	//! ����ľ������ȡlMinTileNum��������
	LONG lMinTileNum = USABLE_BLOCK_SIZE / lTileSize;
	lTileNum = (lMinTileNum > lTileNum) ? lMinTileNum : lTileNum;
	lTileNum = ((((lTileNum % lMinTileNum) == 0) ? 0 : 1) + lTileNum / lMinTileNum) * lMinTileNum;


	//! ʵ�ʷ���Ĵ�С = ������ * �����ÿ��С + ������ڵ��С�� + ����ڴ������ڵ��С
	LONG lAllocSize = lTileNum * (lTileSize + ListNode<VOID>::GetAddedSize()) + ListNode<tagBlockNode>::GetAddedSize();

	void *pNewBuf = malloc(lAllocSize);
	if(NULL == pNewBuf)
		return FALSE;

	//! �ڴ����ӵ�����
	ListNode<tagBlockNode> *pBlockNode = new(pNewBuf)ListNode<tagBlockNode>();
	tagBlockNode *p = pBlockNode->GetNodeData();
	p->lBlockSize = lAllocSize;

	MemHeap._BlockList.PushNode(pBlockNode);
	MemHeap._lTileNum += lTileNum;
	MemHeap._lLastAddNum = lTileNum;

	//! �ڴ�Ƭ��ӵ�����
	LPBYTE pTileNode = (LPBYTE)(pBlockNode->GetTile());

	for (LONG i = 0; i < lTileNum; ++i)
	{
		ListNode<VOID> *pTmpNode = (ListNode<VOID>*)(pTileNode + i * (lTileSize + ListNode<VOID>::GetAddedSize()));
		new(pTmpNode)ListNode<VOID>();
		MemHeap._TileFreeList.PushNode((ListNode<VOID>*)pTmpNode);
	}

	return TRUE;
}

//! �ͷ�һ���ߴ������������ڴ��
VOID MemPool::FreeAllBlockByIndex(LONG lIndex)
{
	MyAssert(MAX_TILE_BUF_SZIE > lIndex);
	tagHeap &MemHeap = m_arrFreeList[lIndex];

	MyAssert(MemHeap._lTileNum == MemHeap._TileFreeList.Size());

	MemHeap._TileFreeList.Release();
	ListNode<tagBlockNode> *pCurrNode = MemHeap._BlockList.PopNode();
	while(NULL != pCurrNode)
	{
		pCurrNode->Rlease();
		free(pCurrNode);
		pCurrNode = MemHeap._BlockList.PopNode();
	}
	MemHeap._lTileNum = 0;
}

//! �õ�һ���ߴ������
inline LONG MemPool::GetSizeIndex(LONG lSize)
{
	if(MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE < lSize)
		return -1;
	MyAssert(0 < lSize);
	return lSize / MIN_BUF_SIZE_DIFFERENCE - ((0 == (lSize % MIN_BUF_SIZE_DIFFERENCE)) ? 1 : 0);
}


#pragma endregion
