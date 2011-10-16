
#include "stdafx.h"
#include "..\nets\networld\message.h"
#include "..\nets\Servers.h"
#include "..\goods\cgoods.h"
#include "..\..\worldserver\game.h"
#include "..\player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnDBSaveMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_DB2W_SAVE_CREATE_PLAYER:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������Create Player������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_COUNTRY:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������country������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_CARDS_LARGESS:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������cards largess������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_ALL_CITY_WAR_PARAM:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������cities war param������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_ALL_VILLAGE_WAR_PARAM:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������billages war param������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_ALL_ENEMY_FACTIONS:
		{
			//ȡ����ʱ��

			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������enemy factions������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_REGION_PARAM:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������region param������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_CONFEDERATION:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������union������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_FACTION:
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������faction������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_DB2W_SAVE_PLAYER: // �����������
		{
			//ȡ����ʱ��
			long MsgTimerID = pMsg->GetLong();
			GetGame()->DelMsgTimer(MsgTimerID);
			AddLogText("��DBS��������Player������Ϣ [%d]OK��", MsgTimerID);
		}
		break;
	case MSG_S2DB_SAVE_PLAYER://�����������
		{
			DWORD dwGSIndex=pMsg->GetGSID();
			pMsg->SetType(MSG_W2S_OTHER_ADDSKILL);
			pMsg->Add(dwGSIndex);
			pMsg->SendAll();			
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
