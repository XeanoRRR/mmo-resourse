
#pragma once

#include "..\clients.h"

class CMyNetClient :
	public CClient
{

protected:
	CSocketCommands		m_SocketOperaCommands;		//������������������
public:
	CMyNetClient(void);
	virtual ~CMyNetClient(void);

	// ��������ʹ�õĽӿ�
	void HandleReceive();
	void HandleConnect();
	void HandleClose();

	virtual bool IsSameSocketID(long lSocketID1,long lSocketID2)
	{
		return lSocketID1 == lSocketID2;
	}

	CSocketCommands& GetSocketCommand()
	{
		return m_SocketOperaCommands;
	}

	virtual void CMyNetClient::SetSendRevBuf();
};
