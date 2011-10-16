#pragma once

#include "../../../../SCMPublic/def/UserDef.h"
#include "../Setup/UserSetup.h"
#include "OnlineUser.h"

class UserManage
{
public:
	UserManage(VOID);
	~UserManage(VOID);

public:
	//!
	static		UserManage&		GetInstance(VOID);

public:
	//!							��ʼ������
	BOOL						Init(VOID);
	//!							�ͷŶ���
	VOID						Release(VOID);

public:
	VOID						SendMsgToUser(const char *pUserName, CMessage &Msg);
	//!							��Ȩ�޷��͸������û�
	VOID						SendToAll(DWORD dwPurviewType, CMessage &Msg);

public:
	//!							��Ӧ�ͻ�������
	VOID						OnUserConn(LONG lSocket);
	//!							��Ӧ�û���½
	VOID						OnUserLogin(tagUserLogin &UserLoginInfo, LONG lSocket);
	//!							��Ӧ�û��Ͽ�
	VOID						OnUserClose(LONG lSocket);

	//!							��Ӧ�û��޸�����
	VOID						OnUserChangePwd(tagChangePWD &ChangePWD, LONG lSocket);
	//!							�ҵ������û�
	OnlineUser*					FindUser(LONG lUserID);
	//!							�ҵ��û�����
	const		string&			FindUserName(LONG lUserID);

private:
	//!							��ȡ����
	BOOL						LoadSetup(VOID);
	//!							��ʼ���û�
	BOOL						InitUser(VOID);
	
	const		OnlineUser*		FindUser(const char *pUserName);
	
	//!							�������û�
	VOID						ClearUser(const char *pUserName, const char *pKickInfo = NULL);
	VOID						ClearUser(LONG lUserID);

private:
	//!							�����û�
	map<LONG, OnlineUser*>		m_mapOnlineUser;
	//!							�����û��Ķ�Ӧ��
	map<string, LONG>			m_mapCdkeyToSocket;

private:
	//!
	static	UserManage		c_UserManage;
};