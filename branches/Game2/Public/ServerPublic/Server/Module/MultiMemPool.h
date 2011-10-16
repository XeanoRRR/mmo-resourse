
/*
*	file:		MultiMemPool.h
*	Brief:		�̰߳�ȫ�ڴ��MultiMemPool
*	detail:		MultiMemPool����MemPool�Ļ������޸Ļ��
*
*				Ϊ����߲����ԣ�
*				ֻ��Alloc��Free�����ӿڣ�����ͬ������
*				�����ӿڻ�����Ҫ���Ե���
*				
*				���ļ���ʵ����һ��������Win32������LockOfWin32������ֱ�ӷ���MultiMemPoolģ�塣
*				��������ʵ�֣��밴��LockOfWin32��д
*				
*				
*	Authtor:	�ſ���
*	Datae:		2007
*/

#include "BaseDef/pooldef.h"

#pragma once


//! �̰߳�ȫ�ڴ����
//! ֻ��Multi_ǰ׺�Ľӿ����̰߳�ȫ��
/////////////////////////////////////////////////////////////////////////////
template<typename _MyLock>
class MultiMemPool
{
#pragma region �����ʼ��

public:
	//! ���캯��
	//!	��ʼ����Ԥ�����ڴ�
	MultiMemPool(void);
	//! ��������
	~MultiMemPool(void);

public:
	//!				Ĭ�ϳ�ʼ������
	//!				��Ĭ�ϵķ�ʽԤ�����ڴ�
	void			DefaultInit(void);

	//!				ָ��һ���ߴ���ڴ�Ԥ�ȷ�����ٿ�
	bool			SetInitialTileNum(long lSize, long lNum);

private:
	//!				�ͷ������ڴ�
	void			Release(void);



#pragma  endregion


#pragma region ����Ӧ��

public:
	//! 			�õ�һ��ָ����С���ڴ�
	//! 			���ڴ汻0���
	void*			Multi_Alloc(long lSize, const char *pInfo);

	//!				�ͷ�һ��ָ����С���ڴ�
	bool			Multi_Free(void *pBuf, long lSize);


	void			OutInfo(void);


#pragma  endregion


#pragma region ˽�г�Ա

private:
	//!				Ϊĳһ�����ڴ�����һ���ڴ�
	bool			AddBlock(long lIndex, long lTileNum = 0);

	//!				�ͷ�һ���ߴ������������ڴ��
	void			FreeAllBlockByIndex(long lIndex);

	//!				�õ�һ���ߴ������
	long			GetSizeIndex(long lSize);

private:
	struct tagBlockNode
	{
		long lBlockSize;
	};

	struct tagHeap
	{
		long					_lTileNum;		//! ��Ƭ��
		long					_lLastAddNum;	//! ��һ����ӵ�Ƭ��
		MyList<void>			_TileFreeList;	//! δʹ�õ��ڴ�Ƭ�б�
		_MyLock					_FreeListLock;
		MyList<tagBlockNode>	_BlockList;		//! ��������ڴ���б�

		tagHeap(void):_lTileNum(0),_lLastAddNum(0){}
	};

	tagHeap						m_arrFreeList[MAX_TILE_BUF_SZIE];


#ifdef MEM_POOL_LOG
	//!							�������ȥ���ڴ����Ϣ
	map<LPVOID, string>			m_mapAllocMemInfo;
	_MyLock						m_setLogLock;
#endif

#pragma endregion


};


//! MultiMemPool����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4311;disable:4291)



#pragma region �����ʼ��


//!----------------------------------------------------------------------------------------------------------------
//! ���캯��
//!	��ʼ����Ԥ�����ڴ�
template<typename _MyLock>
MultiMemPool<_MyLock>::MultiMemPool(void)
{

}
//! ��������
template<typename _MyLock>
MultiMemPool<_MyLock>::~MultiMemPool(void)
{
	Release();
}

//!----------------------------------------------------------------------------------------------------------------
//! Ĭ�ϳ�ʼ������
//! ��Ĭ�ϵķ�ʽԤ�����ڴ�
template<typename _MyLock>
void MultiMemPool<_MyLock>::DefaultInit(void)
{
	for(long i = 0; i < MAX_TILE_BUF_SZIE; ++i)
	{
		AddBlock(i);
	}
}

//!----------------------------------------------------------------------------------------------------------------
//! ָ��һ���ߴ���ڴ�Ԥ�ȷ�����ٿ�
template<typename _MyLock>
bool MultiMemPool<_MyLock>::SetInitialTileNum(long lSize, long lNum)
{
	return AddBlock(GetSizeIndex(lSize), lNum);
}

//!----------------------------------------------------------------------------------------------------------------
//! �ͷ������ڴ�
template<typename _MyLock>
void MultiMemPool<_MyLock>::Release(void)
{
#ifdef MEM_POOL_LOG
	m_setLogLock.Lock();
	if(0 != m_mapAllocMemInfo.size())
	{
		string strOutInfo("�ڴ����������\r\n");

		for (map<LPVOID, string>::iterator ite = m_mapAllocMemInfo.begin(); m_mapAllocMemInfo.end() != ite; ++ite)
		{
			char szInfo[10240] = {0};
			sprintf(szInfo, "	δ�ͷ��ڴ棺��ַ[%x]�������Ϣ[%s]\r\n", (ulong)ite->first, ite->second.c_str());
			strOutInfo.append(szInfo);
		}

		char szFileName[128] = {0};
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		sprintf(szFileName, "log/memory_leak_%04d-%02d-%02d.txt", stTime.wYear, stTime.wMonth, stTime.wDay);
		OverloadFile(szFileName, strOutInfo.c_str());
		m_mapAllocMemInfo.clear();
	}
	m_setLogLock.UnLock();
#endif

#ifdef MEM_POOL_DEBUG
	//m_setBigMemLock.Lock();
	//MyAssert(0 == m_setBigMem.size());
	//m_setBigMemLock.UnLock();
#endif
	for(long i = 0; i < MAX_TILE_BUF_SZIE; ++i)
	{
		FreeAllBlockByIndex(i);
	}
}


#pragma  endregion


#pragma region ����Ӧ��

//!----------------------------------------------------------------------------------------------------------------
//! �õ�һ��ָ����С���ڴ�
//! ���ڴ汻0���
template<typename _MyLock>
void* MultiMemPool<_MyLock>::Multi_Alloc(long lSize, const char *pInfo)
{
	if(MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE < lSize)
	{
		LPVOID pBigMem = malloc(lSize);


#ifdef MEM_POOL_LOG
		m_setLogLock.Lock();
		MyAssert(m_mapAllocMemInfo.end() == m_mapAllocMemInfo.find(pBigMem));
		if(NULL == pInfo)
			m_mapAllocMemInfo[pBigMem] = string("�޴�����Ϣ");
		else
			m_mapAllocMemInfo[pBigMem] = string(pInfo);
		m_setLogLock.UnLock();
#endif
		return pBigMem;
	}
	else
	{
		long lIndex = GetSizeIndex(lSize);
		MyAssert(-1 != lIndex);

		//! ��������ȡ���̼�������������ӻ����
		m_arrFreeList[lIndex]._FreeListLock.Lock();
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
		m_arrFreeList[lIndex]._FreeListLock.UnLock();

#ifdef MEM_POOL_LOG
		m_setLogLock.Lock();
		MyAssert(m_mapAllocMemInfo.end() == m_mapAllocMemInfo.find(pReBuf));
		if(NULL == pInfo)
			m_mapAllocMemInfo[pReBuf] = string("�޴�����Ϣ");
		else
			m_mapAllocMemInfo[pReBuf] = string(pInfo);
		m_setLogLock.UnLock();
#endif

#ifdef FILL_MEM
		memset(pReBuf, FILL_VALUE, (lIndex * MIN_BUF_SIZE_DIFFERENCE));
#endif

		return pReBuf;
	}
}

template<typename _MyLock>
void MultiMemPool<_MyLock>::OutInfo(void)
{
#ifdef MEM_POOL_LOG
	char szInfo[512 * 64] = {0};
	strcat(szInfo, "\r\n");
	for (long i = 0; i < MAX_TILE_BUF_SZIE; ++i)
	{
		tagHeap &memHeap = m_arrFreeList[i];
		if(0 == memHeap._lTileNum)
			continue;

		memHeap._FreeListLock.Lock();

		char szTmp[512] = {0};
		sprintf(szTmp, "�ڴ��[Size:%d]���ܿ���[%d]����Ƭ��[%d]��δʹ��Ƭ��[%d]\r\n", MIN_BUF_SIZE_DIFFERENCE * (i + 1), memHeap._BlockList.Size(), memHeap._lTileNum, memHeap._TileFreeList.Size());
		strcat(szInfo, szTmp);

		memHeap._FreeListLock.UnLock();
	}

    Log4c::Trace(ROOT_MODULE,szInfo);
#endif
}

//!----------------------------------------------------------------------------------------------------------------
//! �ͷ�һ��ָ����С���ڴ�
template<typename _MyLock>
bool MultiMemPool<_MyLock>::Multi_Free(void *pBuf, long lSize)
{
#ifdef MEM_POOL_LOG
	m_setLogLock.Lock();
	MyAssert(m_mapAllocMemInfo.end() != m_mapAllocMemInfo.find(pBuf));
	m_mapAllocMemInfo.erase(pBuf);
	m_setLogLock.UnLock();
#endif

	if(MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE < lSize)
	{
		try
		{
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

	//! ���ڴ�Ƭ�ڵ�黹��������
	LPBYTE pByteBuf = (LPBYTE)pBuf;
	pByteBuf = pByteBuf - ListNode<void>::GetAddedSize();
	MemHeap._FreeListLock.Lock();
	MemHeap._TileFreeList.PushNode((ListNode<void>*)pByteBuf);
	MemHeap._FreeListLock.UnLock();

	return true;
}


#pragma  endregion


#pragma region ˽�г�Ա

//!----------------------------------------------------------------------------------------------------------------
//! Ϊĳһ�ߴ��ڴ�����һ���ڴ�
template<typename _MyLock>
bool MultiMemPool<_MyLock>::AddBlock(long lIndex, long lTileNum)
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

//!----------------------------------------------------------------------------------------------------------------
//! �ͷ�һ���ߴ������������ڴ��
template<typename _MyLock>
void MultiMemPool<_MyLock>::FreeAllBlockByIndex(long lIndex)
{
	MyAssert(MAX_TILE_BUF_SZIE > lIndex);
	tagHeap &MemHeap = m_arrFreeList[lIndex];

	//MyAssert(MemHeap._lTileNum == MemHeap._TileFreeList.Size());

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

//!----------------------------------------------------------------------------------------------------------------
//! �õ�һ���ߴ������
template<typename _MyLock>
inline long MultiMemPool<_MyLock>::GetSizeIndex(long lSize)
{
	if(MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE < lSize)
		return -1;
	MyAssert(0 < lSize);
	return lSize / MIN_BUF_SIZE_DIFFERENCE - ((0 == (lSize % MIN_BUF_SIZE_DIFFERENCE)) ? 1 : 0);
}


