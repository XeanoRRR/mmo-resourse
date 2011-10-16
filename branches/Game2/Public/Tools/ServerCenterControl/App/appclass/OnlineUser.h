

#include "../../../../SCMPublic/def/PurviewDef.h"

#pragma once


class OnlineUser
{
public:
	static		OnlineUser*			CreateUser(LONG lSocket, DWORD dwUserType, DWORD dwPurview, const char *pName);
	static		VOID				ReleaseUser(OnlineUser **ppUser);

public:
	//!								
	LONG							GetID(VOID) const {return m_lSocket;}
	//!
	const char*						GetName(VOID) const {return m_szName;}
	//!								�ߵ����û�
	VOID							Kick(VOID) const;

	//!								���û������ı���Ϣ
	VOID							SendInfo(const char *pText) const;
	//!								���û������ⲿ��Ϣ
	VOID							SendMsg(CMessage &msg) const;

	//!								����Ȩ��
	BOOL							HasPurview(DWORD dwPurviewType) const;

	//!								���һ�����Ƶ�GAPP
	VOID							AddCtrlGapp(DWORD dwGappID, DWORD dwSMID);
	//!								ɾ��һ�����Ƶ�GAPP
	VOID							DelCtrlGapp(DWORD dwGappID);
	

private:
	//!								
	LONG							m_lSocket;
	//!								GroupID
	DWORD							m_dwUserType;
	//!								Ȩ��
	DWORD							m_dwPurview;
	//!
	char							m_szName[MAX_USER_NAME_SZIE];

	//!								���ڿ��Ƶ�GAPP<GAPPID, SMID>
	map<DWORD, DWORD>				m_mapControlingGappID;

private:
	OnlineUser(VOID);
	OnlineUser(LONG lSocket, DWORD dwUserType, DWORD dwPurview, const char *pName);
	~OnlineUser(VOID);
	OnlineUser(OnlineUser&);
};