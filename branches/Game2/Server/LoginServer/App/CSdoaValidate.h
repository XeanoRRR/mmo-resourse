#pragma once

#include "../../../Public/ServerPublic/SDAPI/sdoa4serverdll/include/sdoa4server.h"



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

private:
	//!									��ȡһ����ʱ��֤ID
	LONG								AllotTmpID(VOID);

private:
	//!									��Ӧ�첽����֤�������
	static		int		SNDACALLBACK	ValidateReturn(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg);


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


};