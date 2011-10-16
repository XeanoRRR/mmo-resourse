#include "StdAfx.h"
#include "pool.h"

#pragma warning(disable:4311;disable:4291)



#pragma region �����ʼ��


//! ���캯��
//!	��ʼ����Ԥ�����ڴ�
MemPool::MemPool(void)
{

}
//! ��������
MemPool::~MemPool(void)
{
	Release();
}


//! Ĭ�ϳ�ʼ������
//! ��Ĭ�ϵķ�ʽԤ�����ڴ�
void MemPool::DefaultInit(void)
{
	for(long i = 0; i < MAX_TILE_BUF_SZIE; ++i)
	{
		AddBlock(i);
	}
}

//! ָ��һ���ߴ���ڴ�Ԥ�ȷ�����ٿ�
bool MemPool::SetInitialTileNum(long lSize, long lNum)
{
	return AddBlock(GetSizeIndex(lSize), lNum);
}

//! �ͷ������ڴ�
void MemPool::Release(void)
{
#ifdef MEM_POOL_DEBUG
	MyAssert(0 == m_setBigMem.size());
#endif
	for(long i = 0; i < MAX_TILE_BUF_SZIE; ++i)
	{
		FreeAllBlockByIndex(i);
	}
}


#pragma  endregion


#pragma region ����Ӧ��

//! �õ�һ��ָ����С���ڴ�
//! ���ڴ汻0���
void* MemPool::Alloc(long lSize)
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
		long lIndex = GetSizeIndex(lSize);
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
		void *pReBuf = m_arrFreeList[lIndex]._TileFreeList.PopNode()->GetTile(); 
#ifdef FILL_MEM
		memset(pReBuf, FILL_VALUE, (lIndex * MIN_BUF_SIZE_DIFFERENCE));
#endif

		return pReBuf;
	}
}


//! �ͷ�һ��ָ����С���ڴ�
bool MemPool::Free(void *pBuf, long lSize)
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
			return false;
		}
		return true;
	}

	long lIndex = GetSizeIndex(lSize);
	MyAssert(-1 != lIndex);
	tagHeap &MemHeap = m_arrFreeList[lIndex];

	//! ����ͷŵ��ڴ��Ƿ��Ƕ�������ȥ��
#ifdef MEM_POOL_DEBUG	
	{
		ListNode<tagBlockNode> *pCurrNode = MemHeap._BlockList.GetHead();
		bool bFind = false;
		while(NULL != pCurrNode)
		{
			LPBYTE pBegin = (LPBYTE)pCurrNode->GetTile();
			LPBYTE pEnd = pBegin + pCurrNode->GetNodeData()->lBlockSize - ListNode<tagBlockNode>::GetAddedSize();
			if((long)pBegin < (long)pBuf && (long)pEnd > (long)pBuf)
			{
				bFind = true;
				break;
			}
			pCurrNode = pCurrNode->GetNext();
		}

		if(!bFind)MyAssert(false);
	}
#endif

	//! ���ڴ�Ƭ�ڵ�黹��������
	LPBYTE pByteBuf = (LPBYTE)pBuf;
	pByteBuf = pByteBuf - ListNode<void>::GetAddedSize();
	MemHeap._TileFreeList.PushNode((ListNode<void>*)pByteBuf);

	return true;
}


#pragma  endregion


#pragma region ˽�г�Ա


//! Ϊĳһ�ߴ��ڴ�����һ���ڴ�
bool MemPool::AddBlock(long lIndex, long lTileNum)
{
	if(MAX_TILE_BUF_SZIE <= lIndex || 0 > lIndex) return false;
	tagHeap &MemHeap = m_arrFreeList[lIndex];

	long lTileSize = MIN_BUF_SIZE_DIFFERENCE * (lIndex + 1);

	//! ����ľ������ȡlMinTileNum��������
	long lMinTileNum = USABLE_BLOCK_SIZE / lTileSize;
	lTileNum = (lMinTileNum > lTileNum) ? lMinTileNum : lTileNum;
	lTileNum = ((((lTileNum % lMinTileNum) == 0) ? 0 : 1) + lTileNum / lMinTileNum) * lMinTileNum;


	//! ʵ�ʷ���Ĵ�С = ������ * �����ÿ��С + ������ڵ��С�� + ����ڴ������ڵ��С
	long lAllocSize = lTileNum * (lTileSize + ListNode<void>::GetAddedSize()) + ListNode<tagBlockNode>::GetAddedSize();

	void *pNewBuf = malloc(lAllocSize);
	if(NULL == pNewBuf)
		return false;

	//! �ڴ����ӵ�����
	ListNode<tagBlockNode> *pBlockNode = new(pNewBuf)ListNode<tagBlockNode>();
	tagBlockNode *p = pBlockNode->GetNodeData();
	p->lBlockSize = lAllocSize;

	MemHeap._BlockList.PushNode(pBlockNode);
	MemHeap._lTileNum += lTileNum;
	MemHeap._lLastAddNum = lTileNum;

	//! �ڴ�Ƭ��ӵ�����
	LPBYTE pTileNode = (LPBYTE)(pBlockNode->GetTile());

	for (long i = 0; i < lTileNum; ++i)
	{
		ListNode<void> *pTmpNode = (ListNode<void>*)(pTileNode + i * (lTileSize + ListNode<void>::GetAddedSize()));
		new(pTmpNode)ListNode<void>();
		MemHeap._TileFreeList.PushNode((ListNode<void>*)pTmpNode);
	}

	return true;
}

//! �ͷ�һ���ߴ������������ڴ��
void MemPool::FreeAllBlockByIndex(long lIndex)
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
inline long MemPool::GetSizeIndex(long lSize)
{
	if(MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE < lSize)
		return -1;
	MyAssert(0 < lSize);
	return lSize / MIN_BUF_SIZE_DIFFERENCE - ((0 == (lSize % MIN_BUF_SIZE_DIFFERENCE)) ? 1 : 0);
}


#pragma endregion
