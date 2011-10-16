#pragma once
#include "../clients.h"

class CMyNetClient :
	public CClient
{
public:
	CMyNetClient(void);
	~CMyNetClient(void);

	// ��������ʹ�õĽӿ�
	void HandleReceive();
	void HandleConnect();
	void HandleClose();

	//virtual	void OnReceive(int nErrorCode=0);	// �յ�����Ϣʱ����
	virtual CBaseMessage* CreateMsg(void* pBuf, unsigned long nSize);
	virtual bool IsCheckByCRC()	{return true;}

	virtual void CMyNetClient::SetSendRevBuf();
};
