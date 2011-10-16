#include "StdAfx.h"
#include "netgameserverclient.h"
#include "message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNetGameserverClient::CNetGameserverClient(void)
{
}

CNetGameserverClient::~CNetGameserverClient(void)
{
}

void CNetGameserverClient::HandleReceive()
{
	// -----------------------------------------------------------
	// ������Ϸ����
	CMessage* pmsg = (CMessage*)CMessage::GetRecvQueue()->PopMessage();
	if( pmsg )
	{
	}
	// -----------------------------------------------------------

	return;
}

void CNetGameserverClient::HandleClose()
{
	// ����������
	CMessage* msg;
	msg = new CMessage(CMessage::MSG_WORLD_SERVERLOST);
	CMessage::GetRecvQueue()->PushMessage( msg );
}

void CNetGameserverClient::HandleConnect()
{
}