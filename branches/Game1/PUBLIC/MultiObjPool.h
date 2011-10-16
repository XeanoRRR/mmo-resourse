
/*
*	file:		MultiObjPool.h
*	Brief:		�̰߳�ȫ�ڴ��MultiObjPool
*	detail:		
*				Ϊ����߲����ԣ�
*				ֻ��Alloc��Free�����ӿڣ�����ͬ������
*				�����ӿڻ�����Ҫ���Ե���
*				
*				
*				
*				Ϊ������Ӷ�����ṩ��2�Ժ�ӿڣ��ֺŽ�β������Ϊ��Ϊ�����CreateObj��Release�����ӿ�
*				
*				�̰߳�ȫ�ģ�WIN32����
*					����������ʹ�ã�ADD_OBJECT_MULTIPOOL_H��������
*					��CPP�ļ��У�	ADD_OBJECT_MULTIPOOL_CPP�������� ��ʼ�������������
*				
*				���̰߳�ȫ�ģ�
*					����������ʹ�ã�ADD_OBJECT_POOL_H��������
*					��CPP�ļ��У�	ADD_OBJECT_POOL_CPP�������� ��ʼ�������������
*				
*				
*				
*	Authtor:	�ſ���
*	Datae:		2007
*/

#include "pooldef.h"

#pragma once

//! ��Ӷ���غ꣨WIN32��
/////////////////////////////////////////////////////////////////////////////
//! Ϊ�����һ���̰߳�ȫ�������Ҫ���������꣺
//!		ADD_OBJECT_MULTIPOOL_H		���ඨ������ӣ���Ҫ�ֺŽ�β
//!		ADD_OBJECT_MULTIPOOL_CPP	���ඨ���ļ�����ӣ���Ҫ�ֺŽ�β������lInitObjNum����ʼ����Ķ������������С��0��ֵ�򲻷���
//!
//!	�����κ�Ϊһ�������������̬�ӿڣ�
//!		CreateObj(BOOL bRunConstructor)��	����һ�����󣬲���bRunConstructor��ʾ�Ƿ����Ĭ�Ϲ��캯��
//!		Release(ClassName **ppObj):			����һ����������������������


//! ������
#define ADD_OBJECT_MULTIPOOL_H(ClassName)													\
public:																						\
	static ClassName*	CreateObj(BOOL bRunConstructor = TRUE){								\
	ClassName* pObj;																		\
	pObj = c_ObjPool.Multi_Alloc(LINK_OBJ_INFO(ClassName));															\
	if(NULL != pObj && bRunConstructor) new(pObj)ClassName();								\
	return pObj;																			\
		}																					\
		static VOID			Release(ClassName **ppObj, BOOL bRunDestructor = TRUE){			\
		if(NULL == ppObj || NULL == *ppObj)return;											\
		if(bRunDestructor)(*ppObj)->~ClassName();											\
		c_ObjPool.Multi_Free(*ppObj);														\
		*ppObj=NULL;																		\
		}																					\
private:																					\
	static MultiObjPool<ClassName, LockOfWin32> c_ObjPool				


//! �����
#define ADD_OBJECT_MULTIPOOL_CPP(ClassName, lInitObjNum)									\
	MultiObjPool<ClassName, LockOfWin32> ClassName::c_ObjPool(lInitObjNum)


//! ��Ӷ���غ꣨���̰߳�ȫ��
/////////////////////////////////////////////////////////////////////////////
//! Ϊ�����һ�����̰߳�ȫ�������Ҫ���������꣺
//!		ADD_OBJECT_MULTIPOOL_H		���ඨ������ӣ���Ҫ�ֺŽ�β
//!		ADD_OBJECT_MULTIPOOL_CPP	���ඨ���ļ�����ӣ���Ҫ�ֺŽ�β������lInitObjNum����ʼ����Ķ������������С��0��ֵ�򲻷���
//!
//!	�����κ�Ϊһ�������������̬�ӿڣ�
//!		CreateObj(BOOL bRunConstructor)��	����һ�����󣬲���bRunConstructor��ʾ�Ƿ����Ĭ�Ϲ��캯��
//!		Release(ClassName **ppObj):			����һ����������������������


//! ������
#define ADD_OBJECT_POOL_H(ClassName)													\
public:																						\
	static ClassName*	CreateObj(BOOL bRunConstructor = TRUE){								\
	ClassName* pObj;																		\
	pObj = c_ObjPool.Multi_Alloc(LINK_OBJ_INFO(ClassName));															\
	if(NULL != pObj && bRunConstructor) new(pObj)ClassName();								\
	return pObj;																			\
		}																					\
		static VOID			Release(ClassName **ppObj, BOOL bRunDestructor = TRUE){			\
		if(NULL == ppObj || NULL == *ppObj)return;											\
		if(bRunDestructor)(*ppObj)->~ClassName();											\
		c_ObjPool.Multi_Free(*ppObj);														\
		*ppObj=NULL;																		\
		}																					\
private:																					\
	static MultiObjPool<ClassName, InvalidLock> c_ObjPool				


//! �����
#define ADD_OBJECT_POOL_CPP(ClassName, lInitObjNum)									\
	MultiObjPool<ClassName, InvalidLock> ClassName::c_ObjPool(lInitObjNum)



//! �������
/////////////////////////////////////////////////////////////////////////////
template<typename _MyObject, typename _MyLock>
class MultiObjPool
{
public:
	//! ���캯��
	MultiObjPool(LONG lInitObjNum = -1);
	//! ��������
	~MultiObjPool(VOID);

public:
	//! 			�õ�һ��ָ����С���ڴ�
	//! 			���ڴ汻0���

	_MyObject*		Multi_Alloc(const char *pInfo);

	//!				�ͷ�һ��ָ����С���ڴ�
	BOOL			Multi_Free(_MyObject *pObj);


	

private:
	//!				��ӿ��ж���
	BOOL			AddTileNum(LONG lObjNum = 0);

	//!				�ͷ������ڴ�
	VOID			Release(VOID);






private:
	struct tagBlockNode
	{
		LONG lBlockSize;
	};

	struct tagHeap
	{
		LONG					_lTileNum;		//! ��Ƭ��
		LONG					_lLastAddNum;	//! ��һ����ӵ�Ƭ��
		MyList<VOID>			_TileFreeList;	//! δʹ�õ��ڴ�Ƭ�б�
		_MyLock					_FreeListLock;
		MyList<tagBlockNode>	_BlockList;		//! ��������ڴ���б�

		tagHeap(VOID):_lTileNum(0),_lLastAddNum(0){}
	};

	tagHeap						m_ObjHeap;

#ifdef MEM_POOL_LOG
	//!							�������ȥ���ڴ����Ϣ
	map<LPVOID, string>			m_mapAllocMemInfo;
	_MyLock						m_setLogLock;
#endif

};



//!----------------------------------------------------------------------------------------------------------------
//! ���캯��
//!	��ʼ����Ԥ�����ڴ�
template<typename _MyObject, typename _MyLock>
MultiObjPool<_MyObject, _MyLock>::MultiObjPool(LONG lInitObjNum)
{
	if(0 > lInitObjNum)
		return;
	AddTileNum(lInitObjNum);
}
//! ��������
template<typename _MyObject, typename _MyLock>
MultiObjPool<_MyObject, _MyLock>::~MultiObjPool(VOID)
{
	Release();
}


//!----------------------------------------------------------------------------------------------------------------
//! ��ӿ��ж���
template<typename _MyObject, typename _MyLock>
BOOL MultiObjPool<_MyObject, _MyLock>::AddTileNum(LONG lObjNum)
{
	LONG lTileSize = sizeof(_MyObject);
	//! �����С�������С��һ��
	if(2 > (USABLE_BLOCK_SIZE / lTileSize))
		return TRUE;

	//! ����ľ������ȡlMinTileNum��������
	LONG lMinTileNum = USABLE_BLOCK_SIZE / lTileSize;
	lObjNum = (lMinTileNum > lObjNum) ? lMinTileNum : lObjNum;
	lObjNum = ((((lObjNum % lMinTileNum) == 0) ? 0 : 1) + lObjNum / lMinTileNum) * lMinTileNum;


	//! ʵ�ʷ���Ĵ�С = ������ * �����ÿ��С + ������ڵ��С�� + ����ڴ������ڵ��С
	LONG lAllocSize = lObjNum * (lTileSize + ListNode<VOID>::GetAddedSize()) + ListNode<tagBlockNode>::GetAddedSize();

	void *pNewBuf = malloc(lAllocSize);
	if(NULL == pNewBuf)
		return FALSE;

	//! �ڴ����ӵ�����
	ListNode<tagBlockNode> *pBlockNode = new(pNewBuf)ListNode<tagBlockNode>();
	tagBlockNode *p = pBlockNode->GetNodeData();
	p->lBlockSize = lAllocSize;

	m_ObjHeap._BlockList.PushNode(pBlockNode);
	m_ObjHeap._lTileNum += lObjNum;
	m_ObjHeap._lLastAddNum = lObjNum;

	//! �ڴ�Ƭ��ӵ�����
	LPBYTE pTileNode = (LPBYTE)(pBlockNode->GetTile());

	for (LONG i = 0; i < lObjNum; ++i)
	{
		ListNode<VOID> *pTmpNode = (ListNode<VOID>*)(pTileNode + i * (lTileSize + ListNode<VOID>::GetAddedSize()));
		new(pTmpNode)ListNode<VOID>();
		m_ObjHeap._TileFreeList.PushNode((ListNode<VOID>*)pTmpNode);
	}

	return TRUE;
}

//!----------------------------------------------------------------------------------------------------------------
//! �ͷ������ڴ�
template<typename _MyObject, typename _MyLock>
VOID MultiObjPool<_MyObject, _MyLock>::Release(VOID)
{
#ifdef MEM_POOL_LOG
	m_setLogLock.Lock();
	if(0 != m_mapAllocMemInfo.size())
	{
		string strOutInfo("�ڴ����������\r\n");

		for (map<LPVOID, string>::iterator ite = m_mapAllocMemInfo.begin(); m_mapAllocMemInfo.end() != ite; ++ite)
		{
			char szInfo[1024] = {0};
			sprintf(szInfo, "	δ�ͷ��ڴ棺��ַ[%x]�������Ϣ[%s]\r\n", (DWORD)ite->first, ite->second.c_str());
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

	//MyAssert(m_ObjHeap._lTileNum == m_ObjHeap._TileFreeList.Size());

	m_ObjHeap._TileFreeList.Release();
	ListNode<tagBlockNode> *pCurrNode = m_ObjHeap._BlockList.PopNode();
	while(NULL != pCurrNode)
	{
		pCurrNode->Rlease();
		free(pCurrNode);
		pCurrNode = m_ObjHeap._BlockList.PopNode();
	}
	m_ObjHeap._lTileNum = 0;
}

//!----------------------------------------------------------------------------------------------------------------
//! �õ�һ��ָ����С���ڴ�
//! ���ڴ汻0���
template<typename _MyObject, typename _MyLock>
_MyObject* MultiObjPool<_MyObject, _MyLock>::Multi_Alloc(const char *pInfo)
{
	LONG lSize = sizeof(_MyObject);
	//! �����С�������С��һ��
	if(2 > (USABLE_BLOCK_SIZE / lSize))
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
		return (_MyObject*)pBigMem;
	}
	else
	{
		//! ��������ȡ���̼�������������ӻ����
		m_ObjHeap._FreeListLock.Lock();
		if( 0 == m_ObjHeap._TileFreeList.Size())
		{
			//! ��������ڴ����꣬���ٷ����µĴ��
			if(!AddTileNum())
			{
				//! �ڴ�ľ�
				m_ObjHeap._FreeListLock.UnLock();
				return NULL;
			}
		}

		//! �ӿ���������ɾ��һ���ڵ㣬��0���ڵ�������ڴ�
		VOID *pReBuf = m_ObjHeap._TileFreeList.PopNode()->GetTile();
		m_ObjHeap._FreeListLock.UnLock();

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
		memset(pReBuf, FILL_VALUE, lSize);
#endif

		return (_MyObject*)pReBuf;
	}
}


//!----------------------------------------------------------------------------------------------------------------
//! �ͷ�һ��ָ����С���ڴ�
template<typename _MyObject, typename _MyLock>
BOOL MultiObjPool<_MyObject, _MyLock>::Multi_Free(_MyObject *pObj)
{

#ifdef MEM_POOL_LOG
	m_setLogLock.Lock();
	MyAssert(m_mapAllocMemInfo.end() != m_mapAllocMemInfo.find(pObj));
	m_mapAllocMemInfo.erase(pObj);
	m_setLogLock.UnLock();
#endif

	LONG lSize = sizeof(_MyObject);
	//! �����С�������С��һ��
	if(2 > (USABLE_BLOCK_SIZE / lSize))
	{
		try
		{
			free(pObj);
		}
		catch (...)
		{
			MyAssert(false);
			return FALSE;
		}
		return TRUE;
	}

	//! ���ڴ�Ƭ�ڵ�黹��������
	LPBYTE pByteBuf = (LPBYTE)pObj;
	pByteBuf = pByteBuf - ListNode<VOID>::GetAddedSize();
	m_ObjHeap._FreeListLock.Lock();
	m_ObjHeap._TileFreeList.PushNode((ListNode<VOID>*)pByteBuf);
	m_ObjHeap._FreeListLock.UnLock();

	return TRUE;
}


