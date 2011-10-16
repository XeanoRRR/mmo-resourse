
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

void OnDBLoadMessage(CMessage* pMsg)
{
	BYTE Data[102400];
	pMsg->GetEx(Data,102400);

	switch(pMsg->GetType())
	{
	case MSG_DB2S_LOAD_PLAYER://�����������
		{
			BYTE buf[102400];
			pMsg->GetEx(buf, 102400);
			long pos = 0;
			long msgTimerID = _GetLongFromByteArray(buf, pos);
			CGUID id;
			_GetBufferFromByteArray(buf, pos, id);	// playerID
			CPlayer* player = GetGame()->FindPlayer(id);
			if(player)
			{
				player->DecordFromByteArray(buf, pos);
			}
			// ɾ����ʱ��
			GetGame()->DelMsgTimer(msgTimerID);
			AddLogText("��DBS��ȡPlayer������Ϣ...OK��");
		}
		break;
	case MSG_DB2S_LOAD_CONFEDERATION:
		{
			long msgid = pMsg->GetLong();

			//�����˵�ȫ��Ψһ��ʶ4
			CGUID lID;
			pMsg->GetGUID(lID);
			//��������16
			char strName[16] = {0};
			pMsg->GetStr(strName, 15);
			//�������ID4
			CGUID lMastterID;
			pMsg->GetGUID(lMastterID);	

			GameUnion* pConf = new GameUnion(lID, lMastterID, string(strName));

			pConf->InitByWSMsg(pMsg);

			GetOrganiCtrl()->AddConfederationOrganizing(lID, pConf);

			// ɾ����ʱ��
			GetGame()->DelMsgTimer(msgid);

			char str[128];
			sprintf(str,"�ɹ���ȡͬ�� ID[%d]", lID);
			AddLogText(str);
		}
		break;
	case MSG_DB2S_LOAD_FACTION:
		{
			long msgid = pMsg->GetLong();

			//�ð��ȫ��Ψһ��ʶ4
			CGUID lID;
			pMsg->GetGUID(lID);
			//�������16
			char strName[16] = {0};
			pMsg->GetStr(strName, 15);
			//����ID4
			CGUID lMastterID;
			pMsg->GetGUID(lMastterID);	

			//�ð�����ʱ��32
			tagTime EstablishedTime;
			pMsg->GetEx(&EstablishedTime, sizeof(tagTime));

			GameFaction* pFaction = new GameFaction(lID, lMastterID,EstablishedTime,string(strName));

			pFaction->InitByWSMsg(pMsg);

			GetOrganiCtrl()->AddConfederationOrganizing(lID, pFaction);

			// ɾ����ʱ��
			GetGame()->DelMsgTimer(msgid);

			char str[128];
			sprintf(str,"�ɹ���ȡ��� ID[%d]", lID);
			AddLogText(str);
		}
		break;
	case MSG_S2DB_LOAD_PLAYER_GOOD://���������Ʒ
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
