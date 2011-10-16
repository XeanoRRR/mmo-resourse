#pragma once
#include "..\serverclient.h"

class CPlayer;

class CMyServerClient :
	public CServerClient
{
public:
	CMyServerClient(CServer*	pServers);
	virtual ~CMyServerClient(void);

public:
	
	virtual void OnClose(int errorCode=0);		// ��Է��Ͽ���ʱ�򼤻�
	//virtual	void OnReceive(int nErrorCode=0);	// ��ɶ˿����ܵ���Ϣʱ����
	virtual bool CheckMsg(CBaseMessage *pMsg);

	void OnOneMessageSizeOver(long lSize,long lPermitSize);
	void OnTotalMessageSizeOver(long lSize,long lPermitSize);
	
};
