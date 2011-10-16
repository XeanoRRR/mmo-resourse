

#include "stdafx.h"
#include "SmManage.h"




SmManage SmManage::c_SmManage;

SmManage::SmManage(VOID)
{

}
SmManage::~SmManage(VOID)
{

}
SmManage& SmManage::GetInstance(VOID)
{
	return c_SmManage;
}

//! ��ʼ������
BOOL SmManage::Init(VOID)
{
	if(!LoadSetup())
		return FALSE;

	return TRUE;
}

//! �ͷŶ���
VOID SmManage::Release(VOID)
{
	map<DWORD, SM*>::iterator ite = m_mapRunningSM.begin();
	for (; m_mapRunningSM.end() != ite; ++ite)
	{
		SM::ReleaseSM(&(ite->second));
	}
	m_mapRunningSM.clear();
}

//! ��Ӧ������SM
DWORD SmManage::OnConnect(LONG lSocket, LONG lIP)
{
	const tagSMSetup* pSMSetup = SMSetup::GetInstance().GetSMSetupByIP(lIP);
	if(NULL != pSMSetup)
	{
		//CMessage msg(MSG_SCC2SM_LOGIN_Request_Login);
		//msg.AddEx((void*)pSMSetup->szMD5LoginPwd, MD5_16_PASSWORD_SIZE);
		//msg.SendToSocket(lSocket);
		return pSMSetup->dwID;
	}
	return 0;
}

//! ��Ӧ��½SM
VOID SmManage::OnLogin(LONG lSocket, LPCSTR pPwd, DWORD VersionNo)
{
	if(CURR_VERSION != VersionNo)
	{
		LogTools::OutErrLog("SM version error��SCC version��%u��SM version��%u����disconn!", CURR_VERSION, VersionNo);
		CMessage::DisSmConn(lSocket);
		return;
	}

	DWORD dwSMID = CMessage::GetSMID(lSocket);
	
	SM *pSM = FindSM(dwSMID);
	if(NULL == pSM)
	{
		if(0 == dwSMID)
		{
			LogTools::OutErrLog("can't find SMID!");
			CMessage::DisSmConn(lSocket);
			return;
		}
		const tagSMSetup *pSMSetup = SMSetup::GetInstance().GetSMSetup(dwSMID);
		if(NULL == pSMSetup)
		{
			LogTools::OutErrLog("can't find SM's setup!");
			CMessage::DisSmConn(lSocket);
			return;
		}

		if(0 != memcmp(pPwd, pSMSetup->szMD5LoginPwd, MD5_16_PASSWORD_SIZE))
		{
			LogTools::OutErrLog("SM's password error!");
			CMessage::DisSmConn(lSocket);
			return;
		}

		pSM = AddNewSM(dwSMID, pSMSetup);
		
		if(NULL == pSM)
		{
			LogTools::OutErrLog("create SM object lost!");
			CMessage::DisSmConn(lSocket);
			return;
		}

		LogTools::OutLog("SM[%d] first login!", dwSMID);
	}
	else
	{
		LogTools::OutLog("SM[%d] login over again.", dwSMID);
	}

	ActionLogManage::GetInstance().ActionLog(dwSMID, -1, "", "SM[%d] login!", dwSMID);
	pSM->SetSocket(lSocket);
	CMessage msg(MSG_SCC2SM_LOGIN_Re_Login);
	msg.AddEx((void*)&(SMSetup::GetInstance().GetScc2SmSetup()), sizeof(tagScc2SmSetup));
	msg.SendToSm(lSocket);

	pSM->SendInfoToAll();
}

//! ��Ӧ�Ͽ�����
VOID SmManage::OnClose(DWORD dwSMID)
{
	SM *pSM = FindSM(dwSMID);
	if(NULL != pSM)
	{
		ActionLogManage::GetInstance().ActionLog(dwSMID, -1, "", "SM[%d] logout!", dwSMID);
		pSM->Release();
	}
}


//! ���ͻ�����Ϣ���ͻ���
VOID SmManage::SendInfoToSocket(LONG lSocket)
{
	map<DWORD, SM*>::iterator ite = m_mapRunningSM.begin();
	for (; m_mapRunningSM.end() != ite; ++ite)
	{
		ite->second->SendInfoToSocket(lSocket);
	}
}

//! ���������Ϣ
VOID SmManage::ShowAllInfo(VOID)
{
	LogTools::OutLogAndTxt("SM, setup amount:%d��connect amount:%d", SMSetup::GetInstance().GetSmNum(), (long)m_mapRunningSM.size());
	map<DWORD, SM*>::iterator ite = m_mapRunningSM.begin();
	for (; m_mapRunningSM.end() != ite; ++ite)
	{
		ite->second->ShowAllInfo();
	}
}

//! ��������GAPP�����°汾
VOID SmManage::GetNeedToHighestGapp(vector<tagUpdateInfo> &vUpdateInfo)
{
	vUpdateInfo.clear();
	map<DWORD, SM*>::iterator ite = m_mapRunningSM.begin();
	for (; m_mapRunningSM.end() != ite; ++ite)
	{
		tagUpdateInfo UpdateInfo;
		UpdateInfo.lSocket = ite->second->GetSocket();
		ite->second->GetUpdateInfo(UpdateInfo);
		if(0 < UpdateInfo.setType.size())
			vUpdateInfo.push_back(UpdateInfo);
	}
}

//! ��ȡ����
BOOL SmManage::LoadSetup(VOID)
{
	if(!SMSetup::GetInstance().LoadSetup("setup/SMList.xml"))
	{
		LogTools::OutLog("load setup/SMList.xml lost!!!");
		return FALSE;
	}
	else
	{
		LogTools::OutLog("load setup/SMList.xml succeed.");
	}

	return TRUE;
}

//! �¼�һ��SM
SM* SmManage::AddNewSM(DWORD dwSMID, const tagSMSetup *pSMSetup)
{
	DelSM(dwSMID);
	SM* pRunningSM = SM::CreateSM(dwSMID, pSMSetup->szName, pSMSetup->szExplain, pSMSetup->dwRegionID, pSMSetup->szRegionName, pSMSetup->dwGroupID, pSMSetup->szGroupName);
	if(NULL != pRunningSM)
	{
		m_mapRunningSM[dwSMID] = pRunningSM;
		//pRunningSM->SendInfoToAll();
		return pRunningSM;
	}
	return NULL;
}

//! ɾ��һ��SM
VOID SmManage::DelSM(DWORD dwSMID)
{
	map<DWORD, SM*>::iterator ite = m_mapRunningSM.find(dwSMID);
	if (m_mapRunningSM.end() != ite)
	{
		SM::ReleaseSM(&(ite->second));
		m_mapRunningSM.erase(ite);
	}
}

//! �ҵ�һ��SM
SM* SmManage::FindSM(DWORD dwSMID)
{
	map<DWORD, SM*>::iterator ite = m_mapRunningSM.find(dwSMID);
	if (m_mapRunningSM.end() != ite)
	{
		return ite->second;
	}
	return NULL;
}

