#include "StdAfx.h"
#include "CSdoaValidate.h"

#define MAX_BE_VALIDATING_COUNT (256 * 1024)
#define MAX_WAIT_ID_COUNT		(128)

CSdoaValidate::CSdoaValidate(VOID)
:m_dwIDSeed(0)
,m_pISDOAUserInfoAuthen(NULL)
,m_TmpClientInfoFullCount(0)
{

}

CSdoaValidate::~CSdoaValidate(VOID)
{

}

//!					
bool CSdoaValidate::Init(VOID)
{
	//������½��֤����ʵ��
	if(NULL != m_pISDOAUserInfoAuthen)
		return false;

	m_pISDOAUserInfoAuthen = sdoaCreateUserInfoAuthen();

	//��֤�ӿڳ�ʼ��
	int iRe = m_pISDOAUserInfoAuthen->Initialize("sdoa4server.ini", CSdoaValidate::ValidateReturn);
	if(0 != iRe)
	{
		//! ��ʼ��ʧ��
		Log4c::Warn(ROOT_MODULE,"LOGIN_SDOAVAL_0", iRe);//��ʼ��ISDOAUserInfoAuthenʧ��,error:%d
		return false;
	}

	return true;
}

//!
VOID CSdoaValidate::Release(VOID)
{
	m_mapTmpClientInfo.clear();
	m_mapSocket2ID.clear();
}

//! ������֤
bool CSdoaValidate::RequestValidate(LPCSTR pToken, LONG lIP, LONG lClientSocket, LPCSTR pWSName)
{
	map<LONG, DWORD>::iterator iteID = m_mapSocket2ID.find(lClientSocket);
	if(m_mapSocket2ID.end() != iteID)
	{
		map<DWORD, tagTmpClientInfo>::iterator iteInfo = m_mapTmpClientInfo.find(iteID->second);
		if(m_mapTmpClientInfo.end() != iteInfo)
		{
			if(lIP == iteInfo->second.lIP)
				return true;

			m_mapTmpClientInfo.erase(iteInfo);
		}
		m_mapSocket2ID.erase(iteID);
	}

	//! ��ֹ����ѭ����ΪIDȫ�������������ѭ��
	if(MAX_BE_VALIDATING_COUNT == (DWORD)m_mapTmpClientInfo.size())
	{
		++m_TmpClientInfoFullCount;
		if(MAX_WAIT_ID_COUNT < m_TmpClientInfoFullCount)
			m_mapTmpClientInfo.clear();
		return false;
	}

	//! ��ȡһ��û��ʹ�õ�ID
	DWORD dwTmpID = AllotTmpID();
	while (m_mapTmpClientInfo.end() != m_mapTmpClientInfo.find(dwTmpID))
	{
		dwTmpID = AllotTmpID();
	}

	m_mapTmpClientInfo.insert(make_pair(dwTmpID, tagTmpClientInfo(lIP, lClientSocket, pWSName)));
	m_mapSocket2ID.insert(make_pair(lIP, dwTmpID));
	
	UCHAR arrIp[4] = {0};
	memmove(arrIp, &lIP, sizeof(LONG));
	char szClientIP[64] = {0};
	sprintf(szClientIP, "%u.%u.%u.%u", (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);
	int iRe = m_pISDOAUserInfoAuthen->AsyncGetUserInfo(pToken, szClientIP, dwTmpID);

#ifdef OUT_VALIDATE_FCM_INFO_20090720
	PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "�ͻ��ˡ�%u.%u.%u.%u������ͨ��PT��֤����֤�롾%s������֤ID��%u��", (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3], pToken, dwTmpID);
#endif

	return (0 == iRe);
}

//! ������֤���
VOID CSdoaValidate::OnValidateResult(tagSdoaValidateResult &SdoaValidateResult)
{
#ifdef OUT_VALIDATE_FCM_INFO_20090720
	PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "�ͻ���PT��֤������أ���֤ID��%u������֤�����%d��,PT�����˺š�%s��", SdoaValidateResult.nUniqueId, SdoaValidateResult.nResult, SdoaValidateResult.szSndaID);
#endif
	//! �ҵ��ͻ�����Ϣ
	map<DWORD, tagTmpClientInfo>::iterator iteInfo = m_mapTmpClientInfo.find(SdoaValidateResult.nUniqueId);
	if(m_mapTmpClientInfo.end() == iteInfo)
	{
#ifdef OUT_VALIDATE_FCM_INFO_20090720
		PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "CSdoaValidate::OnValidateResult()���󷵻�1");
#endif
		return;
	}

	//! ������
	if(0 != SdoaValidateResult.nResult)
	{
		switch(SdoaValidateResult.nResult)
		{
		case -10332006://! ϵͳ����
		case -10332007://! socket error
			{
				m_mapTmpClientInfo.clear();
				m_mapSocket2ID.clear();
			}
			break;
		default:
			{
				//! -10332001(���ݻ�ǩ��Ϊ��)
				//! -10332002(��ǩ���Ѿ���֤��)
				//! -10332003(ǩ����֤ʧ��)
				//! -10332004(���ݸ�ʽ����)
				//! -10332005(��ʱ)
			}
		    break;
		}

		m_mapSocket2ID.erase(iteInfo->second.lIP);
		m_mapTmpClientInfo.erase(iteInfo);

#ifdef OUT_VALIDATE_FCM_INFO_20090720
		PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "CSdoaValidate::OnValidateResult()���󷵻�2");
#endif
		return;
	}

	tagAccountInfo_FromAS AccountInfo_FromAS;
	strncpy(AccountInfo_FromAS._szCdkey, SdoaValidateResult.szSndaID, CDKEY_SIZE);
	strncpy(AccountInfo_FromAS._szWSName, iteInfo->second.szWSName, WS_NAME_SIZE);
	AccountInfo_FromAS._dwIp						= iteInfo->second.lIP;
	AccountInfo_FromAS._lSocket						= iteInfo->second.lSocket;
	AccountInfo_FromAS._LoginRerult._InfoCode		= LOGIN_OK;
	AccountInfo_FromAS._LoginRerult._dwLoginResult	= ePLR_Normal;
	
	m_mapTmpClientInfo.erase(iteInfo);

	//! ���Ҫ�����Լ��İ�ȫ��֤
	if(GetGame()->GetLoginSetup()->bXmCardEnable)
	{
		AccountInfo_FromAS._cValidateType = eVT_Not_Pwd | eVT_Not_Fcm;
	}
	else
	{
		AccountInfo_FromAS._cValidateType = eVT_Not_Pwd | eVT_Not_Safe | eVT_Not_Fcm;
	}

	GetGame()->GetLoginManager().AddValidating(AccountInfo_FromAS);
	//! AS��֤
	CMessage msg(MSG_O2A_ACCOUNT_Validate_Request);
	msg.AddEx(&AccountInfo_FromAS, sizeof(tagAccountInfo_ToAS));
	msg.SendToAS();
#ifdef OUT_VALIDATE_FCM_INFO_20090720
	PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "CSdoaValidate::OnValidateResult()�ɹ�����");
#endif
}


//! �ͻ��˶Ͽ�����
VOID CSdoaValidate::OnClientDisConn(LONG lClientSocket)
{
	map<LONG, DWORD>::iterator iteID = m_mapSocket2ID.find(lClientSocket);
	if(m_mapSocket2ID.end() != iteID)
	{
		m_mapTmpClientInfo.erase(iteID->second);
		m_mapSocket2ID.erase(iteID);
	}
}

//! ��ȡһ����ʱ��֤ID
LONG CSdoaValidate::AllotTmpID(VOID)
{
	if(MAX_BE_VALIDATING_COUNT <= m_dwIDSeed)
		m_dwIDSeed = 0;
	return m_dwIDSeed++;
}

//! ��������
inline bool GetYesOrNo(ISDOAMsg *pMsg, LPCSTR pItemName)
{
	LPCSTR pYesOrNo = pMsg->GetValue(pItemName);
	if(NULL != pYesOrNo && 0 == stricmp("Y", pYesOrNo))
		return true;
	return false;
}
//! ��Ӧ�첽����֤�������
int	SNDACALLBACK CSdoaValidate::ValidateReturn(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg)
{
	AinMessage *pAinMessage = AinMessage::Create(AIN_MSG_SDOA_VALIDARE_RETURN);
	if (NULL != pAinMessage)
	{
		tagSdoaValidateResult SdoaValidateResult;
		SdoaValidateResult.nResult = nResult;
		SdoaValidateResult.nUniqueId = nUniqueId;

		if(0 == nResult)
		{
			//! �õ������˺�
			LPCSTR pPTID = pMsg->GetValue("sndaID");
			if(NULL != pPTID)
				strncpy(SdoaValidateResult.szSndaID, pPTID, CDKEY_SIZE);
			//! �õ��Ƿ������
			if(GetYesOrNo(pMsg, "applingAdult"))
				SdoaValidateResult.bNotFcm = GetYesOrNo(pMsg, "adult");
			//! �Ƿ�Ϊ��ֹ�û�
			SdoaValidateResult.bForbidden = GetYesOrNo(pMsg, "forbiddenAccount");
		}
		
		pAinMessage->AddEx(&SdoaValidateResult, sizeof(tagSdoaValidateResult));
		GetGame()->SendToSelf(pAinMessage);
	}
	return 0;
}