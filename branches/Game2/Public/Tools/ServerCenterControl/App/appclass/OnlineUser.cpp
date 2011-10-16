

#include "stdafx.h"
#include "OnlineUser.h"
#include "SmManage.h"



OnlineUser::OnlineUser(LONG lSocket, DWORD dwUserType, DWORD dwPurview, const char *pName)
:m_lSocket(lSocket)
,m_dwUserType(dwUserType)
,m_dwPurview(dwPurview)
{
	strcpy_s(m_szName, MAX_USER_NAME_SZIE, pName);
}
OnlineUser::~OnlineUser(VOID)
{
	map<DWORD, DWORD>::iterator iteGappID = m_mapControlingGappID.begin();
	for (; m_mapControlingGappID.end() != iteGappID; ++iteGappID)
	{
		SM *pSM = SmManage::GetInstance().FindSM(iteGappID->second);
		if(NULL != pSM)
		{
			Gapp *pGapp = pSM->FindGapp(iteGappID->first);
			if(NULL != pGapp)
			{
				pGapp->InCtrlRoom(m_lSocket, FALSE);
			}
		}
	}

	m_mapControlingGappID.clear();
}

OnlineUser* OnlineUser::CreateUser(LONG lSocket, DWORD dwUserType, DWORD dwPurview, const char *pName)
{
	OnlineUser *pReUser = (OnlineUser*)M_ALLOC(sizeof(OnlineUser));
	if(NULL != pReUser)
	{
		new(pReUser)OnlineUser(lSocket, dwUserType, dwPurview, pName);
	}
	else
		LogTools::OutErrLog("OnlineUser::CreateUser lost!!!");
	return pReUser;
}

VOID OnlineUser::ReleaseUser(OnlineUser **ppUser)
{
	if(NULL != ppUser && NULL != (*ppUser))
	{
		(*ppUser)->~OnlineUser();
		M_FREE((*ppUser), sizeof(OnlineUser));
		(*ppUser) = NULL;
	}
}

//! �ߵ����û�
VOID OnlineUser::Kick(VOID)const
{
	CMessage::DisSucConn(m_lSocket);
}

//! ���û������ı���Ϣ
VOID OnlineUser::SendInfo(const char *pText)const
{
	if (NULL != pText)
	{
		CMessage msg(MSG_SCC2SUC_OTHER_Notify_SendText);
	//	msg.Add(pText);
        Json::Value var;
        var["sendText"] = pText;
        msg.AddJson(var);
		SendMsg(msg);
	}
}
//! ���û������ⲿ��Ϣ
VOID OnlineUser::SendMsg(CMessage &msg)const
{
	msg.SendToSuc(m_lSocket);
}

//! ����Ȩ��
BOOL OnlineUser::HasPurview(DWORD dwPurviewType)const
{
	return  (dwPurviewType == (dwPurviewType & m_dwPurview) || ePT_Penniless == dwPurviewType);
}


//! ���һ�����Ƶ�GAPP
VOID OnlineUser::AddCtrlGapp(DWORD dwGappID, DWORD dwSMID)
{
	m_mapControlingGappID.insert(make_pair(dwGappID, dwSMID));
}
//! ɾ��һ�����Ƶ�GAPP
VOID OnlineUser::DelCtrlGapp(DWORD dwGappID)
{
	m_mapControlingGappID.erase(dwGappID);
}