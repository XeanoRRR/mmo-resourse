
#include "stdafx.h"
#include "ClientManager.h"
#include "ValidateOperator.h"
#include "OtherOperator.h"




ClientManager::ClientManager(VOID)
{

}
ClientManager::~ClientManager(VOID)
{

}



//! ��ʼ������
BOOL ClientManager::Init(const char *pFilename)
{
	ValidateOperator::Init(GetGame()->GetSetup()->lServerID);
	return LoadClientSetup(pFilename);
}

//! �ض��ͻ�������
VOID ClientManager::ReLoadClientSetup(const char *pFilename)
{
	if(LoadClientSetup(pFilename))
	{
		CheckOnlineClient();
	}
}

//! �ͷŶ���
VOID ClientManager::Release(VOID)
{
	ValidateOperator::Release();
	OtherOperator::Release();
}



//! ��֤һ���ͻ����Ƿ�Ϸ�
//!		�ɹ�ʱ��dwRetrunInfo���ؿͻ�������
//!		ʧ��ʱ��dwRetrunInfo���ص�½��������
BOOL ClientManager::OnClientLogin(DWORD dwIP, DWORD dwNo, DWORD dwSocket, DWORD &dwRetrunInfo)
{
	iteClientInfo iteInfo = m_mapClientInfo.find(dwNo);
	if(m_mapClientInfo.end() == iteInfo)
	{
		dwRetrunInfo = eALET_NotRegister;
		AddLogText("�Ƿ��ͻ��˵�½��ID[%d]û���������ļ����ҵ���", dwNo);
		return FALSE;
	}
	if(iteInfo->second._dwIP != dwIP)
	{
		dwRetrunInfo = eALET_IpDiffer;
		AddLogText("�ͻ���[%d]��IP�������г��룬�ܾ���½��", dwNo);
		return FALSE;
	}
	if(iteInfo->second._dwLoginState == eLS_OnLine)
	{
		//! �����ظ���½��һ��������
		GetGame()->DisConnClient(iteInfo->second._dwCurrSocket);
		GetGame()->DisConnClient(dwSocket);
		AddLogText("�ͻ���[%d]�Ѿ���½�ˣ��ܾ���ε�½��", dwNo);
		return FALSE;
	}

	//! ͨ����֤
	iteInfo->second._dwLoginState = eLS_OnLine;
	iteInfo->second._dwCurrSocket = dwSocket;
	dwRetrunInfo = iteInfo->second._dwType;

	m_LockForClientInfo.Lock();
	m_mapMultiClientInfo[dwNo]._dwCurrSocket = dwSocket;
	m_LockForClientInfo.UnLock();
	m_mapOnlineClient[dwSocket] = dwNo;

	char szTmp[128] = {0};
	UCHAR arrIp[4] = {0};
	memmove(arrIp, &dwIP, sizeof(LONG));
	sprintf(szTmp, "%s[%d][%u.%u.%u.%u]", iteInfo->second._szName, dwNo, (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);
	
	AddLoginClientList(szTmp);

	return TRUE;
}

//! ��Ӧ�ͻ����˳�
BOOL ClientManager::OnClientLogout(DWORD dwSocket)
{
	iteOnlineClient iteOnline = m_mapOnlineClient.find(dwSocket);
	if(m_mapOnlineClient.end() == iteOnline) return FALSE;

	DWORD dwNo = iteOnline->second;
	iteClientInfo iteInfo = m_mapClientInfo.find(iteOnline->second);
	if(m_mapClientInfo.end() == iteInfo)
	{
		AddErrorLogText("δ���õĿͻ��˳����ڵ�½�б���[IP]��������");
		GetGame()->DisConnClient(dwSocket);
		assert(FALSE);
		return FALSE;
	}

	m_mapOnlineClient.erase(iteOnline);
	iteInfo->second._dwLoginState = eLS_OffLine;
	iteInfo->second._dwCurrSocket = 0;
#ifdef ENABLE_FCM
	iteInfo->second._bAbleWork = FALSE;
#else
	iteInfo->second._bAbleWork = TRUE;
#endif

	m_LockForClientInfo.Lock();
	m_mapMultiClientInfo[dwNo]._dwCurrSocket = 0;
	m_LockForClientInfo.UnLock();

	char szTmp[128] = {0};
	UCHAR arrIp[4] = {0};
	memmove(arrIp, &(iteInfo->second._dwIP), sizeof(LONG));
	sprintf(szTmp, "%s[%d][%u.%u.%u.%u]", iteInfo->second._szName, iteInfo->second._dwNo, (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);

	DelLoginClientList(szTmp);

	return TRUE;
}

//! �ÿͻ����ܷ���в���
DWORD ClientManager::ClientAbleWork(LONG lSocket)
{
	iteOnlineClient iteOnline = m_mapOnlineClient.find(lSocket);
	if(m_mapOnlineClient.end() == iteOnline)
		return 0;

	iteClientInfo iteInfo = m_mapClientInfo.find(iteOnline->second);
	if(m_mapClientInfo.end() == iteInfo)
	{
		AddErrorLogText("δ���õĿͻ��˳����ڵ�½�б���[IP]��������");
		GetGame()->DisConnClient(lSocket);
		assert(FALSE);
		return 0;
	}

#ifdef ENABLE_FCM
	return (iteInfo->second._bAbleWork) ? iteInfo->second._dwNo : 0;
#else
	return iteInfo->second._dwNo;
#endif
}


//! ��ȡ�ͻ�������
BOOL ClientManager::LoadClientSetup(const char *pFilename)
{
	if(NULL == pFilename)
		return FALSE;

	// ��ȡȫ������
	ifstream stream;
	stream.open(pFilename);
	if( !stream.is_open() )
	{
		AddErrorLogText("%s�ļ���ʧ�ܣ�", pFilename);
		return FALSE;
	}

	string strTemp;
	stream >> strTemp; //�ļ�ע��

	tagClientInfo tmpClientInfo;
	tmpClientInfo._dwLoginState = eLS_OffLine;
	tmpClientInfo._dwCurrSocket = NULL;

	BOOL bRe = TRUE;
	do 
	{
		string strIp;
		string strType;
		stream 
			>> tmpClientInfo._dwNo
			>> strIp
			>> strType
			>> tmpClientInfo._szName;

		//! ת��IP
		tmpClientInfo._dwIP = inet_addr(strIp.c_str());   
		//tmpClientInfo._dwIP = htonl(tmpClientInfo._dwIP);

		//! ת������
		if(0 == stricmp("ReadOnly", strType.c_str()))
		{
			tmpClientInfo._dwType = eACT_ReadOnly;
		}
		else if(0 == stricmp("Admin", strType.c_str()))
		{
			tmpClientInfo._dwType = eACT_Admin;
		}
		else
		{
			AddErrorLogText("����Ŀͻ����������á�%s����", strType.c_str());
			bRe = FALSE;
			continue;
		}

		iteClientInfo iteInfo = m_mapClientInfo.find(tmpClientInfo._dwNo);
		if(m_mapClientInfo.end() != iteInfo)
		{
			AddErrorLogText("�ͻ��˱���ظ���%d��������֮ǰ��Ϣ��", tmpClientInfo._dwNo);
			iteInfo->second._dwIP = tmpClientInfo._dwIP;
			iteInfo->second._dwType = tmpClientInfo._dwType;
			memcpy(iteInfo->second._szName, tmpClientInfo._szName, CLIENT_NAME_SIZE);
		}
		else
		{
			m_mapClientInfo[tmpClientInfo._dwNo] = tmpClientInfo;
			m_LockForClientInfo.Lock();
			m_mapMultiClientInfo[tmpClientInfo._dwNo] = tmpClientInfo;
			m_LockForClientInfo.UnLock();
		}
	} 
	while(!stream.eof());

	return bRe;
}

//! ���¼�⵱ǰ���߿ͻ����Ƿ�Ϸ�
VOID ClientManager::CheckOnlineClient(VOID)
{
}

//! �õ��ͻ��˵�Socket
LONG ClientManager::GetClientSocket(DWORD dwNo)
{
	iteClientInfo iteInfo = m_mapClientInfo.find(dwNo);
	if(m_mapClientInfo.end() == iteInfo)
		return 0;
	return iteInfo->second._dwCurrSocket;
}

//! �õ��ͻ��˵�Socket
LONG ClientManager::MultiGetClientSocket(DWORD dwNo)
{
	LONG lRe = 0;
	m_LockForClientInfo.Lock();

	iteClientInfo iteInfo = m_mapMultiClientInfo.find(dwNo);
	if(m_mapMultiClientInfo.end() != iteInfo)
		lRe = iteInfo->second._dwCurrSocket;

	m_LockForClientInfo.UnLock();
	return lRe;
}

//! �õ��ͻ��˵�����
DWORD ClientManager::GetClientType(DWORD dwNo)
{
	iteClientInfo iteInfo = m_mapClientInfo.find(dwNo);
	if(m_mapClientInfo.end() == iteInfo)
		return 0;
	return iteInfo->second._dwType;
}

#ifdef ENABLE_FCM
//! ������Ϣ����
VOID ClientManager::OnlineInfoRevise(DBReadSet &dbReadSet, LONG lSocket)
{
	//! ����õ�LS�����������˺�
	////////////////////////////////////////////////////////////
	LONG lNum = dbReadSet.GetLongFromByteArray();
	
	set<string> setOnLSAccount;
	char szTmpAccount[CDKEY_SIZE] = {0};
	for (LONG i = 0; i < lNum; ++i)
	{
		dbReadSet.GetStringFromByteArray(szTmpAccount, CDKEY_SIZE);
		setOnLSAccount.insert(string(szTmpAccount));
	}

	//! �õ�DB�е������˺�
	////////////////////////////////////////////////////////////
	iteOnlineClient iteOnline = m_mapOnlineClient.find(lSocket);
	if(m_mapOnlineClient.end() == iteOnline)
		return;

	set<string> setOnDBAccount;
	OtherOperator::GetLsOnlineAccount(iteOnline->second, setOnDBAccount);

	//! ֮ǰ���ߣ�������Ȼ���ߵ��˺�
	set<string> setOnlineGoonAccount;

	//! ������ͬ�˺�
	////////////////////////////////////////////////////////////
	set<string>::iterator iteLs = setOnLSAccount.begin();
	while (setOnLSAccount.end() != iteLs)
	{
		set<string>::iterator iteDB = setOnDBAccount.find(*iteLs);
		if (setOnDBAccount.end() != iteDB)
		{
			setOnlineGoonAccount.insert(*iteLs);
			setOnDBAccount.erase(iteDB);
			iteLs = setOnLSAccount.erase(iteLs);
		}
		else
			++iteLs;
	}

	//! setOnLSAccount��ʣ�µ��������ߵ��û�
	for (set<string>::iterator ite = setOnLSAccount.begin(); setOnLSAccount.end() != ite; ++ite)
	{
		OtherOperator *pOtherOperator =	OtherOperator::CreateLoginAndLogout(ite->c_str(), iteOnline->second, eLALT_Login);
		if(NULL != pOtherOperator)
			pOtherOperator->LoginAndLogout();
	}

	//! setOnDBAccount��ʣ�µ��������ߵ��û�
	for (set<string>::iterator ite = setOnDBAccount.begin(); setOnDBAccount.end() != ite; ++ite)
	{
		OtherOperator *pOtherOperator =	OtherOperator::CreateLoginAndLogout(ite->c_str(), iteOnline->second, eLALT_Logout);
		if(NULL != pOtherOperator)
			pOtherOperator->LoginAndLogout();
	}

	//! ����״̬Ϊ����˺ţ�Ҳ��Ҫ����ͳ��ʱ�䷢�͸�LS
	for (set<string>::iterator ite = setOnlineGoonAccount.begin(); setOnlineGoonAccount.end() != ite; ++ite)
	{
		OtherOperator *pOtherOperator =	OtherOperator::CreateLoginAndLogout(ite->c_str(), iteOnline->second, eLALT_GetTime);
		if(NULL != pOtherOperator)
			pOtherOperator->LoginAndLogout();
	}

	iteClientInfo iteInfo = m_mapClientInfo.find(iteOnline->second);
	if(m_mapClientInfo.end() == iteInfo)
	{
		AddErrorLogText("δ���õĿͻ��˳����ڵ�½�б���[IP]��������");
		GetGame()->DisConnClient(lSocket);
		assert(FALSE);
		return;
	}

	iteInfo->second._bAbleWork = TRUE;
}

#endif