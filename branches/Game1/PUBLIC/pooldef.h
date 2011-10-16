/*
*	file:		pooldef.h
*	Brief:		�ڴ����ص�һЩ����
*	detail:		
*				���ļ���ʵ����һ��������Win32������LockOfWin32������ֱ�ӷ���MultiMemPoolģ�塣
*				��������ʵ�֣��밴��LockOfWin32��д
*				
*				
*	Authtor:	�ſ���
*	Datae:		2007
*/

#pragma warning(disable:4311;disable:4291)

#pragma once

//! �����ú�
#define MEM_POOL_DEBUG

//! ��¼��־��
#define MEM_POOL_LOG



///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MEM_POOL_LOG

	#define FILE_LINE_TEMP2(x, y)	"�����ļ�["x"]����������["#y"]"
	#define FILE_LINE_TEMP(x, y)	FILE_LINE_TEMP2(x, y)
	#define _FILE_AND_LINE_			FILE_LINE_TEMP(__FILE__, __LINE__)

		//! ������־��¼��һ���꣨�������������������ڵ��ļ������к����ӳ�һ���ַ�����
	#define LINK_OBJ_INFO(ClassName) (#ClassName ## _FILE_AND_LINE_ )


#else

	#define _FILE_AND_LINE_			NULL

		//! ������־��¼��һ���꣨�������������������ڵ��ļ������к����ӳ�һ���ַ�����
	#define LINK_OBJ_INFO(ClassName) NULL

#endif


//! 
inline VOID MyAssert(BOOL bExpression)	
{								
#ifdef MEM_POOL_DEBUG		
	if(!bExpression) throw;			
#endif						
}

//! ����ڴ��ֵ
#ifdef MEM_POOL_DEBUG
#define FILL_VALUE (0xcd)
#else
#define FILL_VALUE (0)
#endif

//! �����ڴ�ʱ��������ڴ�
#define FILL_MEM


//! ��������С�ڴ��ߴ磬��������ߴ���ڴ�����ֱ�ӵ���ϵͳAPI����
//! Ĭ�ϳߴ�: 64 * 8 = 512byte
//! �ߴ� = MAX_BUF_SZIE * 8
#ifndef MAX_TILE_BUF_SZIE
#define MAX_TILE_BUF_SZIE 64
#endif

//! �����ڴ�߶ȵ���С����
const LONG MIN_BUF_SIZE_DIFFERENCE = 8;


//! Ĭ�ϴ�黺��ߴ�
//! ����ߴ�ֻ����ڿ��������Ŀռ䣬����ָʵ��һ����Ĵ�С
const LONG USABLE_BLOCK_SIZE = MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE * 128;

/*
�ڴ��ͷŴ���ö��
*/
enum eFreeError
{
	eFE_NotErr,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
����ڵ�
*/
template<typename NodeData>
class ListNode
{
	typedef		ListNode<NodeData>			MyType;
	typedef		ListNode<NodeData>*			LPMyType;

public:
	ListNode(VOID):m_pNext(NULL){}
	LPVOID operator new(size_t, LPVOID pBuf){return pBuf;}

public:
	LPMyType		GetNext(VOID)			{return m_pNext;};
	VOID			SetNext(LPMyType pNext)	{m_pNext = pNext;};
	LPVOID			GetTile(VOID)			{return (&m_pNext) + 1;}

	NodeData*		GetNodeData(VOID)		{return &m_NodeData;}
	VOID			Rlease(VOID)			{memset(&m_NodeData, 0, sizeof(NodeData)); m_pNext = NULL;}

	static	LONG	GetAddedSize(VOID)		{return sizeof(NodeData) + sizeof(LPMyType);}
private:
	NodeData	m_NodeData;
	LPMyType	m_pNext;
};

template<>
class ListNode<VOID>
{
	typedef		ListNode<VOID>			MyType;
	typedef		ListNode<VOID>*			LPMyType;

public:
	ListNode(VOID):m_pNext(NULL){}
	LPVOID operator new(size_t, LPVOID pBuf){return pBuf;}

public:
	LPMyType		GetNext(VOID)			{return m_pNext;};
	VOID			SetNext(LPMyType pNext)	{m_pNext = pNext;};
	LPVOID			GetTile(VOID)			{return (&m_pNext) + 1;}

	VOID			Rlease(VOID)			{m_pNext = NULL;}

	static	LONG	GetAddedSize(VOID)		{return sizeof(LPMyType);}
private:
	LPMyType	m_pNext;
};


/*
����
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename NodeData>
class MyList
{
public:
	typedef ListNode<NodeData>	NodeType;
public:
	MyList(VOID)
		: m_pHead(NULL)
		, m_NodeNum(0)
	{}


	//! 
	VOID PushNode(NodeType *pNode)
	{
		pNode->SetNext(m_pHead);
		m_pHead = pNode;
		++m_NodeNum;
	}

	//! 
	NodeType* PopNode(VOID)
	{
		if(NULL == m_pHead) return NULL;
		NodeType *pReNode = m_pHead;
		m_pHead = m_pHead->GetNext();
		--m_NodeNum;
		MyAssert(0 <= m_NodeNum);
		return pReNode;
	}

	VOID Release(VOID)
	{
		m_pHead		= NULL;
		m_NodeNum	= 0;
	}

	//! 
	LONG Size(VOID)
	{
		return m_NodeNum;
	}

#ifdef MEM_POOL_DEBUG
	//!
	NodeType* GetHead(VOID)
	{
		return m_pHead;
	}
#endif


private:
	NodeType					*m_pHead;
	LONG						m_NodeNum;
};



/////////////////////////////////////////////////////////////////////////////
//! һ��WIN32����
class LockOfWin32
{
public:
	LockOfWin32(VOID)		{InitializeCriticalSection(&m_CriticalSection);}
	~LockOfWin32(VOID)		{DeleteCriticalSection(&m_CriticalSection);}
public:
	VOID	Lock(VOID)		{EnterCriticalSection(&m_CriticalSection);}
	VOID	UnLock(VOID)	{LeaveCriticalSection(&m_CriticalSection);}
private:
	CRITICAL_SECTION		m_CriticalSection;
};


/////////////////////////////////////////////////////////////////////////////
//! һ����Ч���������ڲ���Ҫͬ���Ķ���
class InvalidLock
{
public:
	InvalidLock(VOID)		{}
	~InvalidLock(VOID)		{}
public:
	VOID	Lock(VOID)		{}
	VOID	UnLock(VOID)	{}
};