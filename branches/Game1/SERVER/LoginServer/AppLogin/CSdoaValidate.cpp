



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
	int iRe=-1;
	if(GetGame()->GetLoginSetup()->bAuthenEnable)
	{
	   iRe = m_pISDOAUserInfoAuthen->Initialize("sdoa4server.ini", CSdoaValidate::ValidateReturnEx);
	}else{
	   iRe = m_pISDOAUserInfoAuthen->Initialize("sdoa4server.ini", CSdoaValidate::ValidateReturn);
	}
	if(0 != iRe)
	{
		//! ��ʼ��ʧ��
		AddLogText("��ʼ��ISDOAUserInfoAuthenʧ��,error:%d", iRe);
		return false;
	}

	OnInitAuthBusiness();

	return true;
}

//!
VOID CSdoaValidate::Release(VOID)
{
	m_mapTmpClientInfo.clear();
	m_mapSocket2ID.clear();
	m_mapTokenSocketID.clear();
	m_mapSocketTokenID.clear();

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
				m_mapTokenSocketID.clear();
	            m_mapSocketTokenID.clear();
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

	map<string,long>::iterator  it = m_mapTokenSocketID.find(AccountInfo_FromAS._szCdkey);
	if(it != m_mapTokenSocketID.end() )
		m_mapTokenSocketID.erase(it);
	map<long,string>::iterator  itor = m_mapSocketTokenID.find(AccountInfo_FromAS._lSocket);
	if(itor != m_mapSocketTokenID.end() )
		m_mapSocketTokenID.erase(itor);

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
	map<long,string>::iterator itor = m_mapSocketTokenID.find(lClientSocket);
	if(m_mapSocketTokenID.end() != itor)
	{
		m_mapTokenSocketID.erase(itor->second);
		m_mapSocketTokenID.erase(itor);
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

//! ��Ӧ�첽����֤�������(��ȫ����)
int	SNDACALLBACK CSdoaValidate::ValidateReturnEx(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg)
{
	AinMessage *pAinMessage = AinMessage::Create(AIN_MSG_SDOA_Business_RETURN);
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

//����ʼ����ȫ���Ͻӿ�
VOID  CSdoaValidate::OnInitAuthBusiness()
{
	m_pISDOAuthBusiness=NULL;
	m_pISDOAuthBusiness=CreateSDOAuthBusinessInstance();
	int re = m_pISDOAuthBusiness->Init("client.conf");
	if(re!=0)
	{
		char Error[50];
		sprintf(Error,"��ʼ��client.confʧ�ܣ�%d",re);
			PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, Error);
	}
	AddLogText("��ʼ��AuthBusiness,������:%d", re);
	SSDOAuthCallBack  *pbjCb = new SSDOAuthCallBack;
	pbjCb->GetUserInfoCallBack = GetUserInfoCB;
	m_pISDOAuthBusiness->SetCallBack(pbjCb);

}

//!�����ҷ��������ϣ�����2Ϊ�Ƿ���е�һ����֤�� 
VOID  CSdoaValidate::OnAsyncGetUserInfo(string strUserId,bool IsFirst,long lSocketID)
{
	unsigned int dwRpcId=0;
	//vector<int> vecKey;
	int arrKey[2];
	if(IsFirst)
	{
		arrKey[0] = EM_SDO_UserInfo_FirstNeedFullInfo;
		arrKey[1] = EM_SDO_UserInfo_FullInfoUrl;
	}else{
		arrKey[0] = EM_SDO_UserInfo_SecondNeedFullInfo;
		arrKey[1] = EM_SDO_UserInfo_FullInfoUrl;
	}
	SKeyValue oKeyValue;
	oKeyValue.nKey = 1;
	oKeyValue.pValue = (char *)malloc(128);
	strcpy(oKeyValue.pValue, "");
	int iRe = m_pISDOAuthBusiness->AsyncGetUserInfo(0,strUserId.c_str(),arrKey, 2, &oKeyValue, 1, &dwRpcId);
	if(iRe==0)
	{
		mapRpcUserId[dwRpcId]=strUserId;
		//todo other
	}else{   //ʧ��ʱ��������Ϣ
		//long lSocketID =  GetGame()->GetCSdoaValidate().GetSocketIDByCDKey(strUserId);
		//AddLogText("��֤��������ʧ��:%d����",iRe);
		char chText[250];
		sprintf(chText,"��֤��������ʧ�ܣ�%d��%s",iRe,strUserId.c_str());
		PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, chText);
		CMessage msg(MSG_L2C_LOG_ANSWERCDKEY);
		msg.Add((long)LOGIN_AuthBusiness_ERROR);
		msg.Add((long)iRe);
		msg.SendToClient(lSocketID);
	}
	//printf("request error, continue login\n");

}

//�������ҷ��������ϣ��ýӿ��ڵ�һ����֤ʱ����
VOID  CSdoaValidate::OnAsyncGetUserInfo(tagSdoaValidateResult SdoaValidateResult)
{
	if(SdoaValidateResult.bNotFcm==true)
	{
		OnValidateResult(SdoaValidateResult);
	}else{
		DWORD dwSdoID = atoi(SdoaValidateResult.szSndaID);
		map<DWORD, tagTmpClientInfo>::iterator iteInfo = m_mapTmpClientInfo.find(SdoaValidateResult.nUniqueId);
		if(iteInfo!=m_mapTmpClientInfo.end())
		{
			MapTokenSocketID(SdoaValidateResult.szSndaID,iteInfo->second.lSocket); 		
			m_mapSdoaValidateResult[dwSdoID] = SdoaValidateResult;
		    OnAsyncGetUserInfo(SdoaValidateResult.szSndaID,true,iteInfo->second.lSocket);
		}

	}
}

//!ͨ��RpcId�����ҵ������ʺ�
string CSdoaValidate::GetUserCDKey(int dwRpcId)
{
	map<unsigned int,string>::iterator itor = mapRpcUserId.find(dwRpcId);
	if(itor!=mapRpcUserId.end())
	{
		return itor->second;
	}else{
		return "";
	}
}

//ͨ��RpcId���к�����֤
VOID CSdoaValidate::OnValidateByRpcId(int dwRpcId)
{
	string strCDKey = GetUserCDKey(dwRpcId);
	DWORD dwSdoID = atoi(strCDKey.c_str());

	map<DWORD,tagSdoaValidateResult>::iterator itor = m_mapSdoaValidateResult.find(dwSdoID);
	if(itor!=m_mapSdoaValidateResult.end())
	{	
		OnValidateResult(itor->second);
		m_mapSdoaValidateResult.erase(itor);
	}
}
//������ϲ�ѯ�ķ���
void SNDACALLBACK CSdoaValidate::GetUserInfoCB(unsigned int dwRpcId,int nResult, SKeyValue *pKeyValue, int nKeyValueNum)
{
	//please send the return information to other thread which call GetUserInfo. 
	//The message should include (dwRpcId,nResult,pKeyValue);
	//��ø�cdkey
	//string strCDkey = GetUserCDKey(dwRpcId);
	if(nResult==0)
	{
		AinMessage *pAinMessage = AinMessage::Create(AIN_MSG_ACCOUNT_Business_Re);
		if(pAinMessage!=NULL)
		{
			pAinMessage->AddDword((int)dwRpcId);
			for(int i = 0; i< nKeyValueNum; i++)
			{
				SKeyValue *pTemp = &(pKeyValue[i]);
				if(strcmp(pTemp->pValue,"")==0)
				{
					pTemp->pValue = NULL;
				}
				//printf("%d %s\n", pTemp->nKey, pTemp->pValue);
				if(pTemp->nKey==EM_SDO_UserInfo_FirstNeedFullInfo)
				{
				   if(pTemp->pValue!=NULL)
				   {
					   pAinMessage->AddByte(atoi(pTemp->pValue));
				   }else{
					   pAinMessage->AddByte(0);
				   }
				}else if(pTemp->nKey==EM_SDO_UserInfo_SecondNeedFullInfo){
				   //if(pTemp->pValue!=NULL)
				   //{
					  // pAinMessage->AddByte(atoi(pTemp->pValue));
					  // sprintf(chText,"��֤�����AIN_MSG_ACCOUNT_Business_Re��%d %d��",pTemp->nKey,atoi(pTemp->pValue));
					  // PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, chText);
				   //}else{
					   pAinMessage->AddByte(0);
				   //}
				}else if(pTemp->nKey==EM_SDO_UserInfo_FullInfoUrl)
				{
				   if(pTemp->pValue!=NULL)
				   {
					   pAinMessage->AddStr(pTemp->pValue);
				   }else{
					   pAinMessage->AddStr("Url=NULL");
				   }
				}
			}		
			GetGame()->SendToSelf(pAinMessage);
		}

	}else{
		char chText[250]="";
		sprintf(chText,"�첽���RPCID��%d����Ϣ����ʧ�ܣ�%d",dwRpcId,nResult);
		PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, chText);
		//AinMessage *pAinMessage = AinMessage::Create(AIN_MSG_SDOA_Business_Validate);
		//pAinMessage->AddDword((int)dwRpcId);
		//GetGame()->SendToSelf(pAinMessage);
		AinMessage *pAinMessage = AinMessage::Create(AIN_MSG_SDOA_Business_Re_Error);
		pAinMessage->AddDword((int)dwRpcId);
		pAinMessage->AddDword((int)nResult);
		GetGame()->SendToSelf(pAinMessage);
	}
}

VOID CSdoaValidate::MapTokenSocketID(string szToken,long SocketID)
{
	if(strcmp(szToken.c_str(),"")==0|| SocketID <= 0)	return;
	//���֮ǰ��szToken��¼
	map<string,long>::iterator  it = m_mapTokenSocketID.find(szToken);
	if(it != m_mapTokenSocketID.end() )
	{
		m_mapTokenSocketID.erase(it);
		map<long,string>::iterator  itor = m_mapSocketTokenID.find(SocketID);
		if(itor != m_mapSocketTokenID.end() )
			m_mapSocketTokenID.erase(itor);
	}

	m_mapTokenSocketID[szToken] = SocketID;
	m_mapSocketTokenID[SocketID] = szToken;
}

long CSdoaValidate::GetSocketIDByCDKey(string szCDKey)
{
	if(strcmp(szCDKey.c_str(),"")==0)
		return 0;

	map<string,long>::iterator  it = m_mapTokenSocketID.find(szCDKey);
	if(it != m_mapTokenSocketID.end() )
	{
		return (*it).second;
	}
	char chText[250]="";
	sprintf(chText,"���SocketID:%d����ѯcdKey:%s==ʧ�� size��%d",0,szCDKey.c_str(),m_mapTokenSocketID.size());	
	PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, chText);
	map<string,long>::iterator  itor = m_mapTokenSocketID.begin();
	int i=0;
	for(itor;itor!=m_mapTokenSocketID.end();itor++)
	{
	   char chText[250]="";
	   sprintf(chText,"mapTokenSocketID--������%d   %s   %d",i,itor->first.c_str(),itor->second);
	   PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, chText);
	   i++;
	}

	return 0;
}
string CSdoaValidate::GetCDKeyBySocketID(long SocketID)
{
	if(SocketID<=0)
		return "";
	map<long,string>::iterator it = m_mapSocketTokenID.find(SocketID);
	if(it!=m_mapSocketTokenID.end())
	{
		return (*it).second;
	}
	return "";
}
