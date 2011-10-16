


/*
���ļ��������ڴ����Ľڵ�ʹ�õ�������������

*/

#include "mmdef.h"

#ifndef _MEM_LIST_H_
#define _MEM_LIST_H_

namespace XM_Tools
{
	/*
	============================================================================
	============================================================================
	����ڵ�
	============================================================================
	============================================================================
	*/
//!			����ڵ����ֵ
#define		LIST_NODE_GRID_SIZE		4

#pragma pack(push)
#pragma pack(LIST_NODE_GRID_SIZE)
	
	//! ------------------------------------------------------------------------
	//! ͨ��ģ��
	template<typename _TNodeInfo>
	class CListNode
	{
	public:
		inline CListNode*		GetNext(void)				{return m_pNext;}
		inline void				SetNext(CListNode *pNext)	{m_pNext = pNext;}
		inline _TNodeInfo& 		GetInfo(void)				{return m_pInfo;}
		inline void 			SetInfo(_TNodeInfo &Info)	{m_pInfo = Info;}

		//!						�ڴ����÷�ʽȡ�ÿ����ڴ棨����ǰ���뱣֤�ڵ��Ѿ��ڹ���LIST���Ƴ���
		inline void*			GetReBuf(void)				{return &m_pNext;}
		//!						��ͨ��ʽȡ�ÿ����ڴ�
		inline void*			GetBuf(void)				{return ++(&m_pNext);}

	private:
		//!						�ڵ���Ϣ�������Ա����Ϊ����ĵ�һ����Ա��m_pNext�ڴ渴�ã�
		_TNodeInfo				m_pInfo;
		//!	
		CListNode<_TNodeInfo>	*m_pNext;
		//! ��������ö���
	private:
		CListNode(void);
		~CListNode(void);
	};

	//! ------------------------------------------------------------------------
	//! �ػ�ģ��
	template<>
	class CListNode<void>
	{
	public:
		inline CListNode*		GetNext(void)					{return m_pNext;}
		inline void				SetNext(CListNode<void> *pNext)	{m_pNext = pNext;}

		//!						�ڴ����÷�ʽȡ�ÿ����ڴ�
		inline void*			GetReBuf(void)					{return &m_pNext;}
		//!						��ͨ��ʽȡ�ÿ����ڴ�
		inline void*			GetBuf(void)					{return (&m_pNext) + 1;}

	private:
		CListNode<void>			*m_pNext;

		//! ��������ö���
	private:
		CListNode(void);
		~CListNode(void);
	};

#pragma pack(pop)
	/*
	============================================================================
	============================================================================
	����
	============================================================================
	============================================================================
	*/
	template<typename _TNodeInfo, typename _TLock>
	class CMemList
	{
	public:
		inline CMemList(void);
		inline ~CMemList(void);

	public:
		//!								ѹ��һ���ڵ�
		inline void						PushNode(CListNode<_TNodeInfo> *pNode);
		//!								ȡ��һ���ڵ�
		inline CListNode<_TNodeInfo>*	PopNode(void);
		//!								�õ��ڵ���
		inline unsigned long			GetNodeNum(void) const;

	private:
		CListNode<_TNodeInfo>			*m_pHead;
		_TLock							m_Lock;
		unsigned long					m_uNum;
	};

	/*
	============================================================================
	============================================================================
	˫������
	============================================================================
	============================================================================
	*/
	template<typename _TNodeInfo>
	class CListBNode
	{
	public:
		inline CListBNode*		GetPrev(void)				{return m_pPrev;}
		inline void				SetPrev(CListBNode *pPrev)	{m_pPrev = pPrev;}
		inline CListBNode*		GetNext(void)				{return m_pNext;}
		inline void				SetNext(CListBNode *pNext)	{m_pNext = pNext;}
		inline _TNodeInfo& 		GetInfo(void)				{return m_pInfo;}
		inline void 			SetInfo(_TNodeInfo &Info)	{m_pInfo = Info;}

		//!						�ڴ����÷�ʽȡ�ÿ����ڴ棨����ǰ���뱣֤�ڵ��Ѿ��ڹ���LIST���Ƴ���
		inline void*			GetReBuf(void)				{return &m_pPrev;}
		//!						��ͨ��ʽȡ�ÿ����ڴ�
		inline void*			GetBuf(void)				{return ++(&m_pNext);}

	private:
		//!						�ڵ���Ϣ�������Ա����Ϊ����ĵ�һ����Ա��m_pPrev��m_pNext�ڴ渴�ã�
		_TNodeInfo				m_pInfo;
		//!
		CListBNode<_TNodeInfo>	*m_pPrev;
		//!	
		CListBNode<_TNodeInfo>	*m_pNext;
		//! ��������ö���
	private:
		CListBNode(void);
		~CListBNode(void);
	};


	/*
	============================================================================
	============================================================================
	��������
	============================================================================
	============================================================================
	*/
	template<typename _TNodeInfo, typename _TLock, typename _TLikeCompare>
	class CMemSerialBList
	{
	public:
		inline CMemSerialBList(void);
		inline ~CMemSerialBList(void);

	public:
		//!									ѹ��һ���ڵ�
		inline void							PushNode(CListBNode<_TNodeInfo> *pNode);
		//!									ȡ��һ���ڵ�
		inline CListBNode<_TNodeInfo>*		PopNode(void);
		//!									ȡ��һ���ڵ�
		inline CListBNode<_TNodeInfo>*		PopBackNode(void);
		//!									ȡ��һ���ڵ�
		inline CListBNode<_TNodeInfo>*		PopNode(CListBNode<_TNodeInfo> *pNode);
		//!									�ҵ�һ���ڵ�
		inline bool							Find(CListBNode<_TNodeInfo> *pNode);
		//!									ȡ�����������һ���ڵ�
		inline CListBNode<_TNodeInfo>*		GetLikestNode(_TNodeInfo &NodeInfo);
		//!									�õ��ڵ���
		inline unsigned long				GetNodeNum(void) const;

	private:
		//!
		CListBNode<_TNodeInfo>				*m_pHead;
		//!
		CListBNode<_TNodeInfo>				*m_pFoot;
		//!
		_TLock								m_Lock;
		//!
		unsigned long						m_uNum;
	};

};

#include "../cpp/MemList.tcc"

#endif //! _MEM_LIST_H_