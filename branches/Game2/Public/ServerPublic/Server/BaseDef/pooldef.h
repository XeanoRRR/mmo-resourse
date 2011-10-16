/*
*	file:		pooldef.h
*	Brief:		�ڴ����ص�һЩ����
*	detail:		
*				���ļ���ʵ����һ��������Win32������LockOfWin32������ֱ�ӷ���MultiMemPoolģ�塣
*				��������ʵ�֣��밴��LockOfWin32��д				
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
inline void MyAssert(bool bExpression)	
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
const long MIN_BUF_SIZE_DIFFERENCE = 8;


//! Ĭ�ϴ�黺��ߴ�
//! ����ߴ�ֻ����ڿ��������Ŀռ䣬����ָʵ��һ����Ĵ�С
const long USABLE_BLOCK_SIZE = MAX_TILE_BUF_SZIE * MIN_BUF_SIZE_DIFFERENCE * 128;

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
	ListNode(void):m_pNext(NULL){}
	LPVOID operator new(size_t, LPVOID pBuf){return pBuf;}

public:
	LPMyType		GetNext(void)			{return m_pNext;};
	void			SetNext(LPMyType pNext)	{m_pNext = pNext;};
	LPVOID			GetTile(void)			{return (&m_pNext) + 1;}

	NodeData*		GetNodeData(void)		{return &m_NodeData;}
	void			Rlease(void)			{memset(&m_NodeData, 0, sizeof(NodeData)); m_pNext = NULL;}

	static	long	GetAddedSize(void)		{return sizeof(NodeData) + sizeof(LPMyType);}
private:
	NodeData	m_NodeData;
	LPMyType	m_pNext;
};

template<>
class ListNode<void>
{
	typedef		ListNode<void>			MyType;
	typedef		ListNode<void>*			LPMyType;

public:

	ListNode(void):m_pNext(NULL){}
	LPVOID operator new(size_t, LPVOID pBuf){return pBuf;}

public:

	LPMyType		GetNext(void)			{return m_pNext;};
	void			SetNext(LPMyType pNext)	{m_pNext = pNext;};
	LPVOID			GetTile(void)			{return (&m_pNext) + 1;}

	void			Rlease(void)			{m_pNext = NULL;}

	static	long	GetAddedSize(void)		{return sizeof(LPMyType);}

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
	MyList(void)
		: m_pHead(NULL)
		, m_NodeNum(0)
	{}


	//! 
	void PushNode(NodeType *pNode)
	{
		pNode->SetNext(m_pHead);
		m_pHead = pNode;
		++m_NodeNum;
	}

	//! 
	NodeType* PopNode(void)
	{
		if(NULL == m_pHead) return NULL;
		NodeType *pReNode = m_pHead;
		m_pHead = m_pHead->GetNext();
		--m_NodeNum;
		MyAssert(0 <= m_NodeNum);
		return pReNode;
	}

	void Release(void)
	{
		m_pHead		= NULL;
		m_NodeNum	= 0;
	}

	//! 
	long Size(void)
	{
		return m_NodeNum;
	}

#ifdef MEM_POOL_DEBUG
	//!
	NodeType* GetHead(void)
	{
		return m_pHead;
	}
#endif


private:
	NodeType					*m_pHead;
	long						m_NodeNum;
};



/////////////////////////////////////////////////////////////////////////////
//! һ��WIN32����
class LockOfWin32
{
public:
	LockOfWin32(void)		{InitializeCriticalSection(&m_CriticalSection);}
	~LockOfWin32(void)		{DeleteCriticalSection(&m_CriticalSection);}
public:
	void	Lock(void)		{EnterCriticalSection(&m_CriticalSection);}
	void	UnLock(void)	{LeaveCriticalSection(&m_CriticalSection);}
private:
	CRITICAL_SECTION		m_CriticalSection;
};


/////////////////////////////////////////////////////////////////////////////
//! һ����Ч���������ڲ���Ҫͬ���Ķ���
class InvalidLock
{
public:
	InvalidLock(void)		{}
	~InvalidLock(void)		{}
public:
	void	Lock(void)		{}
	void	UnLock(void)	{}
};