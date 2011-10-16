#pragma once
#include <list>
#include <vector>

using namespace std;

#include "../serverClient.h"


//const int MAX_MESSAGE_LENGTH = 10240;


class CMyServerClient:
	public CServerClient
{
public:
	CMyServerClient(CServer* pServer);
	~CMyServerClient(void);

	virtual void OnClose(int errorCode=0);		// ��Է��Ͽ���ʱ�򼤻�
	//virtual	void OnReceive(int nErrorCode=0);// ��ɶ˿����ܵ���Ϣʱ����
	virtual CBaseMessage* CreateMsg(void* pBuf, unsigned long nSize);	//������Ϣ

	virtual void SetSendRevBuf();
};
