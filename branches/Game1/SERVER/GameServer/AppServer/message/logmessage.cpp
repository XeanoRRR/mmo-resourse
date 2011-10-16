#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"
#include "..\player.h"
#include "..\serverRegion.h"
#include "..\..\gameserver\game.h"
#include "../setup/GlobeSetup.h"
#include "../setup/QuestSystem.h"
#include "..\Session\CSessionFactory.h"
#include <MMSystem.h>
#include "SequenceString.h"
#include "../session/CPlug.h"
#include "..\public\dupliregionsetup.h"
#include "../setup/ContributeSetup.h"
#include "../RgnManager.h"
#include "..\PlayerMan\PlayerMan.h"


// ��ӦLOG��Ϣ
void OnLogMessage(CMessage* pMsg)
{
	char pszLogingInfo[512]="";
	switch(pMsg->GetType())
	{
	case MSG_C2S_LOG_PLAYERENTER://��������ӵ�gameserver
		{
			long lSignCode = pMsg->GetLong();

			CGUID PlayerID;
			pMsg -> GetGUID(PlayerID);

			char szCdkey[128];
			pMsg->GetStr(szCdkey, sizeof(szCdkey));

			// ��½ʱ��ɫ������GameServer ����player��ΪNULL��˵���Ѿ���ȥ����Ϸ
			CPlayer* player = GetGame()->FindPlayer(PlayerID);
			//���Player���ڣ�������˵�������
			if (player)
			{
				char szPlayerID[50] = "";
				PlayerID.tostring(szPlayerID);
				_snprintf(pszLogingInfo,512,"error,when the player(Name:%s,RegionID:%d)connect the gs,that has the same guid player!", player->GetName(),player->GetRegionID());
				PutStringToFile("Login_GS_Info",pszLogingInfo);
			}

			long lLoginCount = GameManager::GetInstance()->AddLoginSession(pMsg->GetSocketID());
			// ����
			CMessage msg(MSG_S2W_LOG_QUEST_PLAYERDATA);
			msg.Add((long)lSignCode);
			msg.Add(PlayerID);
			msg.Add(lLoginCount);
			msg.Add(szCdkey);
			msg.Add(pMsg->GetIP());
			msg.Send();
		}
		break;	

	case MSG_W2S_LOG_KICKPLAYER://worldserver֪ͨ��һ���ض�ID�����
		{
		}
		break;

	case MSG_W2S_LOG_ANSWER_PLAYERDATA://worldserver������ҵ���ϸ����
		{
#ifdef _RUNSTACKINFO3_
			char pszStatckInfo[1024]="";
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MSG_W2S_LOG_ANSWER_PLAYERDATA Start");
#endif
			long lSignCode = pMsg->GetLong();
			long lFlag=pMsg->GetLong();
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			long lLoginCount = pMsg->GetLong();
			char szPlayerID[40];
			PlayerID.tostring(szPlayerID);

#ifdef _RUNSTACKINFO3_
			char pszGUID[50]="";
			PlayerID.tostring(pszGUID);
			_snprintf(pszStatckInfo,1024,"playerID:%s,flag:%d",pszGUID,lFlag);
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

			long lC2SSocketID = GameManager::GetInstance()->RemoveLoginSessionByCountNum(lLoginCount);
			// login server��û�и����
			if (lFlag==0)
			{
				CMessage msg1(MSG_S2C_LOG_ANSWER_PLAYERDATA);
				msg1.Add(lSignCode);
				msg1.Add((long)0);
				msg1.SendToSocket(lC2SSocketID);

				_snprintf(pszLogingInfo,512,"������ҵ���ϸ����ʱ (%s) û��������½,�ѱ�����Ͽ�!", szPlayerID);
				PutStringToFile("Login_GS_Info",pszLogingInfo);
				GetGame()->GetNetServer()->DisConn(lC2SSocketID);
				return;
			}

			// ���������
			if (lFlag==-1)
			{
				CMessage msg1(MSG_S2C_LOG_ANSWER_PLAYERDATA);
				msg1.Add(lSignCode);
				msg1.Add((long)0);
				msg1.SendToSocket(lC2SSocketID);

				_snprintf(pszLogingInfo,512,"������ҵ���ϸ����ʱ (%s) ���������!", szPlayerID);
				PutStringToFile("Login_GS_Info",pszLogingInfo);
				GetGame()->GetNetServer()->DisConn(lC2SSocketID);
				return;
			}

			// GameServer�Ƿ�
			if (lFlag==-2)
			{
				CMessage msg1(MSG_S2C_LOG_ANSWER_PLAYERDATA);
				msg1.Add(lSignCode);
				msg1.Add((long)0);
				msg1.SendToSocket(lC2SSocketID);

				_snprintf(pszLogingInfo,512,"������ҵ���ϸ����ʱ (%s) GameServer�Ƿ�!", szPlayerID);
				PutStringToFile("Login_GS_Info",pszLogingInfo);
				GetGame()->GetNetServer()->DisConn(lC2SSocketID);
				return;
			}

			// ��֤�����
			if (lFlag==-3)
			{
				CMessage msg1(MSG_S2C_LOG_ANSWER_PLAYERDATA);
				msg1.Add(lSignCode);
				msg1.Add((long)0);
				msg1.SendToSocket(lC2SSocketID);

				_snprintf(pszLogingInfo,512,"������ҵ���ϸ����ʱ (%s) GameServer�Ƿ�!", szPlayerID);
				PutStringToFile("Login_GS_Info",pszLogingInfo);
				GetGame()->GetNetServer()->DisConn(lC2SSocketID);
				return;
			}

			//�������Ѿ��Ͽ�
			if(lC2SSocketID == 0)
			{
				//##������Ϣ
				CMessage msg( MSG_S2W_LOG_PLAYERQUIT );
				msg.Add( PlayerID );
				msg.Add((long)-1); // �������˳�
				msg.Add(0L);
				msg.Add("");
				msg.Send();
				return;
			}
			// ����Ҳ�������PlayerList,Ҳ����˵û���յ�����ҵ� MSG_C2S_LOG_PLAYERENTER ��Ϣ,�������м䱻������½�ߵ���
			if( lFlag == 1 )
			{
				// �������������
				if( GetGame()->GetSetup()->dwMsgValidateTime )
				{
					// ��ӵ�MAP��
					//GetGame()->RemoveValidateTime(lPlayerID);
					GetGame()->AppendValidateTime(PlayerID, true);

					// ����GamerSErver ��ʱ�䵽Cleint
					CMessage msgServerTime(MSG_S2C_LOG_GAMESERVERTIME);
					unsigned long ulServerTime = /*(ULONG)time(NULL)*/ timeGetTime();
					msgServerTime.Add( PlayerID );
					msgServerTime.Add( ulServerTime );
					//! ���һ������ʱ�䷢�͵��ͻ���[2008-1-3]
					long ldatatime = 0;
					time(&ldatatime);
					msgServerTime.Add(ldatatime);
					//!
					msgServerTime.SendToSocket(lC2SSocketID);
				}

				CPlayer *pPlayer = GetGame()->FindPlayer(PlayerID);
				if(pPlayer)
				{
					//����˴�,�����߼������˴���,Ҫ����bug
					//������������һЩǿ���Դ���
					_snprintf(pszLogingInfo,512,"error,when the player(Name:%s,RegionID:%d) enter the game,the gs has the same guid player!",
						pPlayer->GetName(),pPlayer->GetRegionID());
					PutStringToFile("Login_GS_Info",pszLogingInfo);

					CServerRegion* pRegion = dynamic_cast<CServerRegion*>(pPlayer->GetFather());
					if(pRegion)
						pRegion->RemoveObject(pPlayer);
				}
				else
				{
					pPlayer = MP_NEW CPlayer;
					if(pPlayer)
						pPlayer->SetExID(PlayerID);
				}
				pPlayer->SetSignCode(lSignCode);
				// ��ʼ���������
				DBReadSet setReadDB;
				pMsg->GetDBReadSet(setReadDB);
				pPlayer->DecodeFromDataBlock(setReadDB);

				//GetGame()->FreeShareDB(ByteData);

				//����SocketID��PlayerID
				CMessage::MapPlayerIDSocket(PlayerID,lC2SSocketID);
				GetGame()->AddPlayer(pPlayer);
				// Fox@20081110 �������
				GetInst(CPlayerMan).AddPlayer(pPlayer);

				// ������Ϸ
				if(!pPlayer->OnEnter())
				{
					GetGame()->KickPlayer(pPlayer->GetExID());
				}

				if(GetGame()->IsBSConnected())
				{
					CMessage msg(MSG_S2B_GET_BALANCE);
					msg.Add(pPlayer->GetAccount());
					msg.Add(pPlayer->GetExID());
					msg.SendToBS();
				}
			}
#ifdef _RUNSTACKINFO3_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MSG_W2S_LOG_ANSWER_PLAYERDATA End");
#endif
		}
		break;
	case MSG_W2S_LOG_REPEAT_LOGIN:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			char strCDKey[256];
			pMsg -> GetStr( strCDKey, 256 );
			char szPlayerID[40] = {0};
			PlayerID.tostring(szPlayerID);

			CPlayer* pPlayer	= GetGame() -> FindPlayer( PlayerID );
			if( pPlayer )
			{			
				//##���ȷ�����Ϣ������ҡ�
				CMessage repeateLoginMsg(MSG_S2C_OTHER_REPEATLOGIN);
				repeateLoginMsg.Add("���˳��Դ������ط���½��Ϸ,�㱻ǿ��������.");
				repeateLoginMsg.SendToPlayer(PlayerID);

				GetGame()->OnPlayerExit(PlayerID, FALSE);

				GetGame()->RemovePlayer(PlayerID);
				// �ϵ�����
				GetGame()->KickPlayer(PlayerID);
			}
			else
			{
				//##������Ϣ
				CMessage msg( MSG_S2W_LOG_PLAYERQUIT );
				msg.Add( PlayerID );
				msg.Add((long)-1); // �������˳�
				msg.Add(0L);
				msg.Add( strCDKey );
				msg.Send();
			}
		}
		break;

	case MSG_W2S_LOG_FCM_TIME:
		{
			CGUID playerID;
			pMsg->GetGUID(playerID);
			long lTime = pMsg->GetLong();
			CPlayer* pPlayer = GetGame()->FindPlayer(playerID);
			if(pPlayer)
			{
				CMessage msg(MSG_L2C_LOG_FCM_TIME);
				msg.Add(lTime);
				msg.SendToPlayer(playerID);
			}
		}
		break;
	}
}
