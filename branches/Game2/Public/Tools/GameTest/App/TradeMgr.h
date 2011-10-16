


#ifndef _TreadMgr_H_
#define _TreadMgr_H_
#pragma once


class CTradeMgr
{
public:
	CTradeMgr(void);
	~CTradeMgr(void);

	void	SetRequestGuid(const CGUID& guid)	{ m_RequestGuid = guid; }
	void	SetSessionGuid(const CGUID& guid)	{ m_SessionGuid = guid; }

	void	StartTrade(const CGUID& sessionGuid,const CGUID& myGuid,long myPlugID,const CGUID& youGuid,long youPlugID);

	void	SetYouLock(bool bLock)	{ m_bYouLocked = bLock; }
	void	SetMeLock(bool bLock)		{ m_bMyLocked = bLock; }

	void	SetCanTrade( bool bCanTrade)	{ m_bCanTrade = bCanTrade; }
	bool	IsCanTrade(void)				{ return m_bCanTrade ; }

private:
	//	������GUID
	CGUID	m_RequestGuid;
	//	����Session
	CGUID	m_SessionGuid;
	//	�ҵ����ID
	CGUID		m_MyGuid;				
	//	�Է����ID
	CGUID		m_YourGuid;			
	//	�Լ��ڴ˴λỰ�е�ID
	long		m_lMyPlugID;			
	//	�Է��ڴ˴λỰ�е�ID
	long		m_lYouPlugID;			
	//	�ҷ�����״̬��־
	bool		m_bMyLocked;		
	//	�Է�����״̬��־
	bool		m_bYouLocked;		
	//	�����Ƿ�ɹ�
	bool		m_bResult;
	//	�����Ƿ��ܹ�����
	bool		m_bCanTrade;
};
#endif//_TreadMgr_H_