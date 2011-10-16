#include "stdafx.h"
#include "..\nets\Servers.h"
#include "..\entity\Account.h"
#include "..\Entity\EntityGroup.h"
#include "..\Entity\EntityManager.h"
#include "../../../../dbaccess/WorldDB/RsEntity.h"
#include "../Thread/DBAccessThread.h"
#include "../Thread/DBThread.h"
#include "../Thread/ThreadDataDef.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"

#include <mmsystem.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern unsigned __stdcall SaveThreadFunc(void* pArguments);
extern HANDLE g_hSavingThread;

// ��Ӧ������Ϣ
void OnServerMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_W2DB_SERVER_UNREGENTITY:
		{
			char szCdkey[128];
			pMsg->GetStr(szCdkey, sizeof(szCdkey));
			if(szCdkey)
			{
				Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
				if(pAcc)
				{
					long lCurTime = timeGetTime();
					pAcc->SetLastLoginTime(lCurTime);
				}
			}
		}
		break;
	case MSG_W2DB_SERVER_QUEST_RECONNECTION:
		{
			// ��ʱˢ������
			lConnects++;

			long msgid = pMsg->GetLong(); // MSGID
			CMessage msg(MSG_DB2W_SERVER_QUEST_RECONNECTION);
			msg.Add(msgid);
			// �����factions��unions�����ɸ��Ե���Ϣ����
			msg.SendToSocket(pMsg->GetSocketID());
		}
		break;
		// ����server��Ϣ��dbserver
	case MSG_W2DB_SERVER_SENDSERVERINFO:
		{
			// ��ʱˢ������
			lConnects++;

			// ����GameServer��ʶ
			bool bReconnect = pMsg->GetChar()?true:false;
			DWORD dwPort = pMsg->GetLong();
			char strGameServerIP[256];
			pMsg->GetStr(strGameServerIP,256);

			CGame::tagWorldServer* pGameServer = GetGame()->GetWorldServer(strGameServerIP, dwPort);
			if (pGameServer)
			{
				pGameServer->bConnected = true;
				CMessage::MapWSIDSocket(pGameServer->dwIndex, pMsg->GetSocketID());

				GetGame()->SetWSSocket(pMsg->GetSocketID());
			}
			else
			{
				AddLogText(CStringReading::LoadString(IDS_DBS_SERVERMESSAGE, STR_DBS_SERVERMESSAGE_WSCONNECTFAIL), strGameServerIP, dwPort);
				break;
			}
			char str[256];
			sprintf(str, "%s [%d]", strGameServerIP, dwPort);
			SendMessage(g_hGameServerListBox, LB_INSERTSTRING,-1, (LPARAM)str);
			AddLogText(CStringReading::LoadString(IDS_DBS_SERVERMESSAGE, STR_DBS_SERVERMESSAGE_WSCONNECTOK), strGameServerIP, dwPort);

			// �������ݿ�������Ϣ
			GetGame()->SendDBTableInfo(pMsg->GetSocketID());
		}
		break;
		// �յ�WorldServer�ķ���
	case MSG_W2DB_SERVER_ANSWER_PING:
		
		break;
		// WS�Ľű����͵���Ϣ,��Ҫ��Ӧ����
	case MSG_W2DB_SERVER_PLAYER_PERSONALHOUSEGUID:
		{
			long scriptID = pMsg->GetLong();// Script ID

			// player name
			char playerName[128];
			pMsg->GetStr(playerName, 128);

			// PH GUID
			CGUID guid;

			char szPHGuid[128]; // ���ص�PH GUID ������
			pMsg->GetStr(szPHGuid, 128);

			
			CMessage msg(MSG_W2DB_SERVER_PLAYER_PERSONALHOUSEGUID);
			msg.Add(scriptID);// Script ID
			msg.Add(playerName); // player name
			msg.Add(szPHGuid);// ���ص�PH GUID ������
			msg.Add(guid);// // PH GUID
			msg.SendToSocket(pMsg->GetSocketID());
		}
		break;
		// ���մ�WS����SaveData��Ϣ������־
	case MSG_W2DB_SERVER_SAVEDATAEND:
		{
			BYTE flag = pMsg->GetByte();
			if(flag == 0)
				AddLogText("����WS��ʱ������Ϣ����(�������̴߳������)��");
			else if(flag == 1)
				AddLogText("����WS���߱������������Ϣ����(�������̴߳������)��");
		}
		break;
	case MSG_W2DB_SERVER_QUEST_PING:
		{
			CMessage msg(MSG_DB2W_SERVER_QUEST_PING);
			msg.SendToSocket(pMsg->GetSocketID());
		}
		break;
	}
}
