#pragma once
#include "myserverclient.h"

class CMyNetServerClient_World :
	public CMyServerClient
{
public:
	CMyNetServerClient_World(CServer* pServer);
	virtual ~CMyNetServerClient_World(void);

private:

public:
	virtual void OnClose(int errorCode=0);		// ��Է��Ͽ���ʱ�򼤻�
	//virtual	void OnReceive(int nErrorCode=0);// ��ɶ˿����ܵ���Ϣʱ����
	virtual CBaseMessage* CreateMsg(void* pBuf, unsigned long nSize);	//������Ϣ

	virtual void SetSendRevBuf();
};
