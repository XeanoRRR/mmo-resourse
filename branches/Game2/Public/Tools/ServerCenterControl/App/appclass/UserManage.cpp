

#include "stdafx.h"
#include "UserManage.h"
#include "../Setup/PurviewSetup.h"

#include "DbOperator.h"
#include "SmManage.h"
#include "PackManage.h"
#include "GmSystem.h"


UserManage::UserManage(VOID)
{

}
UserManage::~UserManage(VOID)
{
	Release();
}


UserManage UserManage::c_UserManage;
//!
UserManage& UserManage::GetInstance(VOID)
{
	return c_UserManage;
}


//! ��ʼ������
BOOL UserManage::Init(VOID)
{
	if (!LoadSetup())
	{
		return FALSE;
	}

	if (!InitUser())
	{
		return FALSE;
	}

	CMessage::AllowSUC(TRUE);
	return TRUE;
}

//! �ͷŶ���
VOID UserManage::Release(VOID)
{
	CMessage::AllowSUC(FALSE);

	map<LONG, OnlineUser*>::iterator iteUser = m_mapOnlineUser.begin();
	for (; m_mapOnlineUser.end() != iteUser; ++iteUser)
	{
		OnlineUser::ReleaseUser(&(iteUser->second));
	}
	m_mapOnlineUser.clear();
	m_mapCdkeyToSocket.clear();
}

VOID UserManage::SendMsgToUser(const char *pUserName, CMessage &Msg)
{
	if(NULL != pUserName)
	{
		const OnlineUser *pUser = FindUser(pUserName);
		if(NULL != pUser)
			pUser->SendMsg(Msg);
	}
}

//! ��Ȩ�޷��͸������û�
VOID UserManage::SendToAll(DWORD dwPurviewType, CMessage &Msg)
{
	for (map<LONG, OnlineUser*>::iterator iteUser = m_mapOnlineUser.begin(); m_mapOnlineUser.end() != iteUser; ++iteUser)
	{
		if(iteUser->second->HasPurview(dwPurviewType))
			Msg.SendToSuc(iteUser->first);
	}
}

//! ��Ӧ�ͻ�������
VOID UserManage::OnUserConn(LONG lSocket)
{
	
}

//! ��Ӧ�û���½
VOID UserManage::OnUserLogin(tagUserLogin &UserLoginInfo, LONG lSocket)
{
    Json::Value var;
    bool bLogin = true;
    int nErrCode = 0;

	CMessage msg(MSG_SCC2SUC_LOGIN_Re_Login);

	//! �汾
	if(CURR_VERSION != UserLoginInfo.dwVersion)
	{
        bLogin = false;
        nErrCode = eULE_VersionErr;
        goto ErrCode;
		/*msg.Add((DWORD)FALSE);
		msg.Add((DWORD)eULE_VersionErr);
		msg.SendToSuc(lSocket);
		return;*/
	}

	//! �û�����
	const tagUserSetup *pUserSetup = UserSetup::GetInstance().FindUser(UserLoginInfo.szName);
	if (NULL == pUserSetup)
	{
        bLogin = false;
        nErrCode = eULE_Nobody;
        goto ErrCode;
		/*msg.Add((DWORD)FALSE);
		msg.Add((DWORD)eULE_Nobody);
		msg.SendToSuc(lSocket);
		return;*/
	}

	//! ����
	char szTmpPwd[BASE_PASSWORD_SIZE] = {0};
	UserSetup::GetInstance().TransitionMD5ToStr(szTmpPwd, UserLoginInfo.szMD5Pwd);
	if (0 != memcmp(pUserSetup->szPwd, szTmpPwd, BASE_PASSWORD_SIZE))
	{
        bLogin = false;
        nErrCode = eULE_PwdErr;
        goto ErrCode;
		/*msg.Add((DWORD)FALSE);
		msg.Add((DWORD)eULE_PwdErr);
		msg.SendToSuc(lSocket);
		return;*/
	}

	//! ���Ȩ��
	const tagGMTPurviewSetup *pGMTPurviewSetup = PurviewSetup::GetInstance().GetPurviewByUserType(pUserSetup->dwUserType);
	if (NULL == pGMTPurviewSetup)
	{
        bLogin = false;
        nErrCode = eULE_SystemErr;
        goto ErrCode;
		/*msg.Add((DWORD)FALSE);
		msg.Add((DWORD)eULE_SystemErr);
		msg.SendToSuc(lSocket);
		return;*/
	}

ErrCode:
    if(!bLogin)
    {
        var["result"] = false;
        var["errType"] = nErrCode;
        msg.AddJson(var);
        msg.SendToSuc(lSocket);
        return;
    }
    else
    {
        // ��½�ɹ�
        var["result"] = true;
        var["userID"] = (int)pGMTPurviewSetup->dwUserType;
        var["purView"] = (int)pGMTPurviewSetup->dwPurview;
        var["groupName"] = pGMTPurviewSetup->szName;
        msg.AddJson(var);
        msg.SendToSuc(lSocket);
    }

    

	////! ��½�ɹ�
	//msg.Add((DWORD)TRUE);
	//msg.AddEx((void*)pGMTPurviewSetup, sizeof(tagGMTPurviewSetup));	
	//msg.SendToSuc(lSocket);

	//! ��½�ɹ���Ĵ���
	//////////////////////////////////////////////
	//����������û�
	ClearUser(UserLoginInfo.szName, "user rejoin, you had been break off.");
	//! ���´���ӵ������
	OnlineUser* pUser = OnlineUser::CreateUser(lSocket, pGMTPurviewSetup->dwUserType, pGMTPurviewSetup->dwPurview, UserLoginInfo.szName);
	if (NULL != pUser)
	{
		m_mapOnlineUser[lSocket] = pUser;
		m_mapCdkeyToSocket[string(UserLoginInfo.szName)] = lSocket;

		ActionLogManage::GetInstance().ActionLog(-1, -1, UserLoginInfo.szName, "user[%s] login.", UserLoginInfo.szName);
		//! ���ͻ�����Ϣ���û�
		SmManage::GetInstance().SendInfoToSocket(lSocket);
		if(pUser->HasPurview(ePT_Update))
			PackManage::GetInstance().SendPackInfoListToClient(lSocket);
	}
	else
		LogTools::OutLog("create user[%s] object lost!!!", UserLoginInfo.szName);
	
}

//! ��Ӧ�û��Ͽ�
VOID UserManage::OnUserClose(LONG lSocket)
{
	ClearUser(lSocket);
	GmSystem::GetInstance().DelListen(lSocket);
}

//! ��Ӧ�û��޸�����
VOID UserManage::OnUserChangePwd(tagChangePWD &ChangePWD, LONG lSocket)
{
	const OnlineUser *pUser = FindUser(lSocket);
	if(NULL != pUser)
	{
		DWORD dwErrorNo = eULE_SystemErr;

		//! ��������������
		const tagUserSetup *pUserSetup = UserSetup::GetInstance().FindUser(FindUserName(lSocket).c_str());
		if(NULL != pUserSetup)
		{
			char szTmpPwd[BASE_PASSWORD_SIZE] = {0};
			UserSetup::GetInstance().TransitionMD5ToStr(szTmpPwd, ChangePWD.szMD5PwdOld);
			if(0 == memcmp(pUserSetup->szPwd, szTmpPwd, BASE_PASSWORD_SIZE))
			{
				UserSetup::GetInstance().TransitionMD5ToStr(szTmpPwd, ChangePWD.szMD5PwdNew);
				DbOperator *pDbOperator = DbOperator::CreatePwdChangeOperator(pUserSetup->szName, szTmpPwd);
				if(NULL != pDbOperator)
				{
					if(pDbOperator->Begin())
					{
						return;
					}
					DbOperator::ReleaseOperator(&pDbOperator);
				}

			}
			else
				dwErrorNo = eULE_PwdErr;
		}

		CMessage msg(MSG_SCC2SUC_LOGIN_Re_ChangePwd);
		/*msg.Add((DWORD)FALSE);
		msg.Add(dwErrorNo);*/
        Json::Value var;
        var["result"] = false;
        var["errCode"] = (int)dwErrorNo;
		msg.SendToSuc(lSocket);
	}
}

//! ��ȡ����
BOOL UserManage::LoadSetup(VOID)
{
	if(!PurviewSetup::GetInstance().LoadSetup("setup/Purview.xml"))
	{
		LogTools::OutLog("load setup/Purview.xml lost!!!");
		return FALSE;
	}
	else
	{
		LogTools::OutLog("load setup/Purview.xml succeed!");
	}

	if(!UserSetup::GetInstance().LoadSetup("setup/User.xml"))
	{
		LogTools::OutLog("load setup/User.xml lost!!!");
		return FALSE;
	}
	else
	{
		LogTools::OutLog("load setup/User.xml succeed!");
	}

	return TRUE;
}
//! ��ʼ���û�
BOOL UserManage::InitUser(VOID)
{
	//! �����ݿ��л���û����µ�����
	UserSetup::GetInstance().BeginUserPwdCmp();

	return TRUE;
}

//! �ҵ������û�
OnlineUser* UserManage::FindUser(LONG lUserID)
{
	map<LONG, OnlineUser*>::iterator ite = m_mapOnlineUser.find(lUserID);
	if(m_mapOnlineUser.end() != ite)
		return ite->second;
	return NULL;
}
const OnlineUser* UserManage::FindUser(const char *pUserName)
{
	if(NULL != pUserName)
	{
		map<string, LONG>::iterator iteSocket = m_mapCdkeyToSocket.find(pUserName);
		if(m_mapCdkeyToSocket.end() != iteSocket)
		{
			map<LONG, OnlineUser*>::iterator iteUser = m_mapOnlineUser.find(iteSocket->second);
			if(m_mapOnlineUser.end() != iteUser)
			{
				return iteUser->second;
			}
			else
			{
				LogTools::OutErrLog("error!!! m_mapCdkeyToSocket is different with m_mapOnlineUser in UserManage::FindUser!!");
				m_mapCdkeyToSocket.erase(iteSocket);
			}
		}
	}
	return NULL;
}

const string& UserManage::FindUserName(LONG lUserID)
{
	for (map<string, LONG>::iterator iteSocket = m_mapCdkeyToSocket.begin(); m_mapCdkeyToSocket.end() != iteSocket; ++iteSocket)
	{
		if(lUserID == iteSocket->second)
			return iteSocket->first;
	}
	return INVALID_STRING;
}

//! �������û�
VOID UserManage::ClearUser(const char *pUserName, const char *pKickInfo)
{
	if(NULL != pUserName)
	{
		map<string, LONG>::iterator iteSocket = m_mapCdkeyToSocket.find(pUserName);
		if(m_mapCdkeyToSocket.end() != iteSocket)
		{
			map<LONG, OnlineUser*>::iterator iteUser = m_mapOnlineUser.find(iteSocket->second);
			if(m_mapOnlineUser.end() != iteUser)
			{
				if(NULL != pKickInfo)
					iteUser->second->SendInfo(pKickInfo);
				iteUser->second->Kick();
				OnlineUser::ReleaseUser(&(iteUser->second));
				m_mapOnlineUser.erase(iteUser);
			}
			else
			{
				LogTools::OutErrLog("error!!! m_mapCdkeyToSocket is different with m_mapOnlineUser in UserManage::ClearUser1!!");
			}

			m_mapCdkeyToSocket.erase(iteSocket);
		}
	}
}
VOID UserManage::ClearUser(LONG lUserID)
{
	map<LONG, OnlineUser*>::iterator iteUser = m_mapOnlineUser.find(lUserID);
	if(m_mapOnlineUser.end() != iteUser)
	{
		OnlineUser::ReleaseUser(&(iteUser->second));
		m_mapOnlineUser.erase(iteUser);

		map<string, LONG>::iterator iteSocket = m_mapCdkeyToSocket.begin();
		for (; m_mapCdkeyToSocket.end() != iteSocket; ++iteSocket)
		{
			if(iteSocket->second == lUserID)
			{
				m_mapCdkeyToSocket.erase(iteSocket);
				return;
			}
		}

		LogTools::OutErrLog("error!!! m_mapCdkeyToSocket is different with m_mapOnlineUser in UserManage::ClearUser2!!");
	}
}



