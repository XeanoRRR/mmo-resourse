#pragma once
#include "..\serverclient.h"

class CClientForGS :
	public CServerClient
{
public:
	CClientForGS(CServer* pServer);
	virtual ~CClientForGS(void);

public:
	virtual void OnClose(int errorCode=0);		// ��Է��Ͽ���ʱ�򼤻�
	virtual	void OnReceive(int nErrorCode=0);	// ��ɶ˿����ܵ���Ϣʱ����

	virtual void SetSendRevBuf();
};
