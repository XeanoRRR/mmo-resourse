#pragma once
#include "../clients.h"

class CNetGameserverClient :
	public CClient
{
public:
	CNetGameserverClient(void);
	~CNetGameserverClient(void);

	// ��������ʹ�õĽӿ�
	void HandleReceive();
	void HandleConnect();
	void HandleClose();
};
