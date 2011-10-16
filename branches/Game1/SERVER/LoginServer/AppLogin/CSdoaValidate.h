


#include "../../sdoa4serverdll/include/sdoa4server.h"
#include "../../sdoa4serverdll/include/ISDOAuthBusiness.h"
#include "../../sdoa4serverdll/include/ISDOAuthCallBack.h"

#pragma once

//!
class CSdoaValidate
{
public:
	CSdoaValidate(VOID);
	~CSdoaValidate(VOID);

public:
	//!					
	bool								Init(VOID);
	//!
	VOID								Release(VOID);

	//!									������֤
	bool								RequestValidate(LPCSTR pToken, LONG lIP, LONG lClientSocket, LPCSTR pWSName);
	//!									������֤���
	VOID								OnValidateResult(tagSdoaValidateResult &SdoaValidateResult);
	//!									�ͻ��˶Ͽ�����
	VOID								OnClientDisConn(LONG lClientSocket);
	//!                                 ��ʼ����ȫ���Ͻӿ�
	VOID                                OnInitAuthBusiness();
	//!                                 ��ȡ��Ϣ����
	VOID                                OnAsyncGetUserInfo(string strUserId,bool IsFirst=true,long lSocketID=0);
	//!                                 ͨ��RpcId������cdkey
	string                              GetUserCDKey(int dwRpcId);
	//!                                 ��ȡע����Ϣ
	VOID                                OnAsyncGetUserInfo(tagSdoaValidateResult SdoaValidateResult);
	//!                                 ͨ��RpcId������֤��������к�����֤
	VOID                                OnValidateByRpcId(int dwRpcId);

	VOID                                MapTokenSocketID(string szToken,long SocketID);

	long                                GetSocketIDByCDKey(string szCDKey);
	string                              GetCDKeyBySocketID(long SocketID);

private:
	//!									��ȡһ����ʱ��֤ID
	LONG								AllotTmpID(VOID);

private:
	//!									��Ӧ�첽����֤�������
	static		int		SNDACALLBACK	ValidateReturn(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg);
	//!									��Ӧ�첽����֤�������(��ȫ����)
	static		int		SNDACALLBACK	ValidateReturnEx(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg);
	//!                                 ��Ӧ��������첽��ȡ�ķ���
	static      void    SNDACALLBACK    GetUserInfoCB(unsigned int dwRpcId,int nResult, SKeyValue *pKeyValue, int nKeyValueNum);

private:
	//! ������ʱ��֤��Ϣ�Ľṹ
	struct tagTmpClientInfo
	{
		LONG	lIP;
		LONG	lSocket;
		char	szWSName[WS_NAME_SIZE];
		
		tagTmpClientInfo(VOID){memset(this, 0, sizeof(tagTmpClientInfo));}
		tagTmpClientInfo(LONG lInIP, LONG lInSocket, LPCSTR pWSName)
			:lIP(lInIP),lSocket(lInSocket)
		{strncpy(szWSName, pWSName, WS_NAME_SIZE);}
	};


private:
	//!									��ʱ��֤ID����
	DWORD								m_dwIDSeed;
	//!									������֤�Ŀͻ���<��ʱID����ʱ��Ϣ>
	map<DWORD, tagTmpClientInfo>		m_mapTmpClientInfo;
	//!									��¼m_mapTmpClientInfo���Ĵ���
	DWORD								m_TmpClientInfoFullCount;
	//!									Socket��������ʱID<Socket, ��ʱID>
	map<LONG, DWORD>					m_mapSocket2ID;

	//!									ʢ��SDK�е�ͨ��֤��֤����
	ISDOAUserInfoAuthen					*m_pISDOAUserInfoAuthen;

	//!                                 ʢ�����ϲ�ȫ���
	ISDOAuthBusiness                    *m_pISDOAuthBusiness;

	//!                                 ��¼�û�id
	map<unsigned int,string>             mapRpcUserId;

	//��                                 ʢ����֤����ļ���
	map<DWORD,tagSdoaValidateResult>        m_mapSdoaValidateResult;

	//!                                 ���ڴ洢�����ʺź�SocketID��Ӱ���ϵ
	map<string,long>                    m_mapTokenSocketID;
	map<long,string>                    m_mapSocketTokenID;


};

