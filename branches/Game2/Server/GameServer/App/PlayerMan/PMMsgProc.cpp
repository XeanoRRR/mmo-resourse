#include "StdAfx.h"
#include "PlayerMan.h"
#include "../../Net/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��Ӧ��ҹ�����Ϣ
void OnPMMessage(CMessage* pMsg)
{
	assert(NULL != pMsg);

	switch(pMsg->GetType())
	{
	case MSG_C2S_PM_REQ:		// CLIENT�����ѯ�����Ϣ
		{
			GetInst(CPlayerMan).Query(pMsg);
		}
		break;

	case MSG_W2S_PM_ACK:		// WSӦ���ѯ�����Ϣ
		{
			pMsg->SetType(MSG_S2C_PM_ACK);
			CGUID ExID;
			pMsg->GetGUID(ExID);
			pMsg->SendToPlayer(ExID);
		}
		break;
	}
}