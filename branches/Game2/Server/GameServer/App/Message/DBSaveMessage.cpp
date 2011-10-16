
#include "stdafx.h"
#include "..\nets\netserver\message.h"
//#include "..\nets\netserver\MyServerClient.h"
//#include "..\nets\netserver\MyNetServer.h"
//#include "..\nets\netserver\MyServerClient.h"
#include "..\goods\cgoods.h"
#include "..\..\gameserver\game.h"
#include "..\player.h"
#include "..\OrganizingSystem\GameFaction.h"
#include "..\OrganizingSystem\GameUnion.h"
#include "..\OrganizingSystem\GameOrganizingCtrl.h"
#include "..\..\..\..\public\Date.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnDBSaveMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_DB2S_SAVE_PLAYER://�����������
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������Player������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2S_SAVE_FACTION://�����������
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������faction������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2S_SAVE_CONFEDERATION://�����������
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������union������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_S2DB_SAVE_PLAYER_GOOD://���������Ʒ
		{
			pMsg->SetType(MSG_W2S_OTHER_DELGOODS);
			pMsg->SendAll();
		}
		break;
	case MSG_S2W_PLAYER_DELSKILL://gm�������ɾ��һ����ҵļ���,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_DELSKILL);
			pMsg->SendAll();
		}
		break;
	case MSG_S2W_PLAYER_SETLEVEL://gm�����������һ����ҵĵȼ�,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_SETLEVEL);
			pMsg->SendAll();
		}
		break;	

	}
}
