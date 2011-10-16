
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

#include "../BaseDef/pooldef.h"

#pragma once

//! ��Ӷ���غ꣨WIN32��
/////////////////////////////////////////////////////////////////////////////
//! Ϊ�����һ���̰߳�ȫ�������Ҫ���������꣺
//!		ADD_OBJECT_MULTIPOOL_H		���ඨ������ӣ���Ҫ�ֺŽ�β
//!		ADD_OBJECT_MULTIPOOL_CPP	���ඨ���ļ�����ӣ���Ҫ�ֺŽ�β������lInitObjNum����ʼ����Ķ������������С��0��ֵ�򲻷���
//!
//!	�����κ�Ϊһ�������������̬�ӿڣ�
//!		CreateObj(bool bRunConstructor)��	����һ�����󣬲���bRunConstructor��ʾ�Ƿ����Ĭ�Ϲ��캯��
//!		Release(ClassName **ppObj):			����һ����������������������


//! ������
#define ADD_OBJECT_MULTIPOOL_H(ClassName)													\
public:																						\
	static ClassName*	CreateObj(bool bRunConstructor = true){								\
	ClassName* pObj;																		\
	pObj = c_ObjPool.Multi_Alloc(LINK_OBJ_INFO(ClassName));															\
	if(NULL != pObj && bRunConstructor) new(pObj)ClassName();								\
	return pObj;																			\
		}																					\
		static void			Release(ClassName **ppObj, bool bRunDestructor = true){			\
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
//!		CreateObj(bool bRunConstructor)��	����һ�����󣬲���bRunConstructor��ʾ�Ƿ����Ĭ�Ϲ��캯��
//!		Release(ClassName **ppObj):			����һ����������������������


//! ������
#define ADD_OBJECT_POOL_H(ClassName)													\
public:																						\
	static ClassName*	CreateObj(bool bRunConstructor = true){								\
	ClassName* pObj;																		\
	pObj = c_ObjPool.Multi_Alloc(LINK_OBJ_INFO(ClassName));															\
	if(NULL != pObj && bRunConstructor) new(pObj)ClassName();								\
	return pObj;																			\
		}																					\
		static void			Release(ClassName **ppObj, bool bRunDestructor = true){			\
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
	MultiObjPool(long lInitObjNum = -1);
	//! ��������
	~MultiObjPool(void);

public:
	//! 			�õ�һ��ָ����С���ڴ�
	//! 			���ڴ汻0���

	_MyObject*		Multi_Alloc(const char *pInfo);

	//!				�ͷ�һ��ָ����С���ڴ�
	bool			Multi_Free(_MyObject *pObj);


	

private:
	//!				��ӿ��ж���
	bool			AddTileNum(long lObjNum = 0);

	//!				�ͷ������ڴ�
	void			Release(void);






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
MultiObjPool<_MyObject, _MyLock>::MultiObjPool(long lInitObjNum)
{
	if(0 > lInitObjNum)
		return;
	AddTileNum(lInitObjNum);
}
//! ��������
template<typename _MyObject, typename _MyLock>
MultiObjPool<_MyObject, _MyLock>::~MultiObjPool(void)
{
	Release();
}


//!----------------------------------------------------------------------------------------------------------------
//! ��ӿ��ж���
template<typename _MyObject, typename _MyLock>
bool MultiObjPool<_MyObject, _MyLock>::AddTileNum(long lObjNum)
{
	long lTileSize = sizeof(_MyObject);
	//! �����С�������С��һ��
	if(2 > (USABLE_BLOCK_SIZE / lTileSize))
		return true;

	//! ����ľ������ȡlMinTileNum��������
	long lMinTileNum = USABLE_BLOCK_SIZE / lTileSize;
	lObjNum = (lMinTileNum > lObjNum) ? lMinTileNum : lObjNum;
	lObjNum = ((((lObjNum % lMinTileNum) == 0) ? 0 : 1) + lObjNum / lMinTileNum) * lMinTileNum;


	//! ʵ�ʷ���Ĵ�С = ������ * �����ÿ��С + ������ڵ��С�� + ����ڴ������ڵ��С
	long lAllocSize = lObjNum * (lTileSize + ListNode<void>::GetAddedSize()) + ListNode<tagBlockNode>::GetAddedSize();

	void *pNewBuf = malloc(lAllocSize);
	if(NULL == pNewBuf)
		return false;

	//! �ڴ����ӵ�����
	ListNode<tagBlockNode> *pBlockNode = new(pNewBuf)ListNode<tagBlockNode>();
	tagBlockNode *p = pBlockNode->GetNodeData();
	p->lBlockSize = lAllocSize;

	m_ObjHeap._BlockList.PushNode(pBlockNode);
	m_ObjHeap._lTileNum += lObjNum;
	m_ObjHeap._lLastAddNum = lObjNum;

	//! �ڴ�Ƭ��ӵ�����
	LPBYTE pTileNode = (LPBYTE)(pBlockNode->GetTile());

	for (long i = 0; i < lObjNum; ++i)
	{
		ListNode<void> *pTmpNode = (ListNode<void>*)(pTileNode + i * (lTileSize + ListNode<void>::GetAddedSize()));
		new(pTmpNode)ListNode<void>();
		m_ObjHeap._TileFreeList.PushNode((ListNode<void>*)pTmpNode);
	}

	return true;
}

//!----------------------------------------------------------------------------------------------------------------
//! �ͷ������ڴ�
template<typename _MyObject, typename _MyLock>
void MultiObjPool<_MyObject, _MyLock>::Release(void)
{
#ifdef MEM_POOL_LOG
	m_setLogLock.Lock();
	if(0 != m_mapAllocMemInfo.size())
	{
		string strOutInfo("�ڴ����������\r\n");

		for (map<LPVOID, string>::iterator ite = m_mapAllocMemInfo.begin(); m_mapAllocMemInfo.end() != ite; ++ite)
		{
			char szInfo[1024] = {0};
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
	long lSize = sizeof(_MyObject);
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
		void *pReBuf = m_ObjHeap._TileFreeList.PopNode()->GetTile();
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
bool MultiObjPool<_MyObject, _MyLock>::Multi_Free(_MyObject *pObj)
{

#ifdef MEM_POOL_LOG
	m_setLogLock.Lock();
	MyAssert(m_mapAllocMemInfo.end() != m_mapAllocMemInfo.find(pObj));
	m_mapAllocMemInfo.erase(pObj);
	m_setLogLock.UnLock();
#endif

	long lSize = sizeof(_MyObject);
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
			return false;
		}
		return true;
	}

	//! ���ڴ�Ƭ�ڵ�黹��������
	LPBYTE pByteBuf = (LPBYTE)pObj;
	pByteBuf = pByteBuf - ListNode<void>::GetAddedSize();
	m_ObjHeap._FreeListLock.Lock();
	m_ObjHeap._TileFreeList.PushNode((ListNode<void>*)pByteBuf);
	m_ObjHeap._FreeListLock.UnLock();

	return true;
}


