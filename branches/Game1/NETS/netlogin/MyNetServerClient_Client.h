#pragma once
#include "myserverclient.h"


class CMyNetServerClient_Client :
	public CMyServerClient
{
public:
	CMyNetServerClient_Client(CServer* pServer);
	virtual ~CMyNetServerClient_Client(void);

public:
	virtual void OnClose(int errorCode=0);		// ��Է��Ͽ���ʱ�򼤻�
	//virtual	void OnReceive(int nErrorCode=0);	// ��ɶ˿����ܵ���Ϣʱ����
	virtual bool CheckMsg(CBaseMessage *pMsg);

	void OnOneMessageSizeOver(long lSize,long lPermitSize);
	void OnTotalMessageSizeOver(long lSize,long lPermitSize);
};
