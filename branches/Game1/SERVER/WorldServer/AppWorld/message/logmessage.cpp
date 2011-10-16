#include "stdafx.h"
#include "..\nets\networld\message.h"
#include "..\nets\Servers.h"
#include "..\..\worldserver\game.h"
#include "..\player.h"
#include "../goods/CGoods.h"
#include "..\..\appworld\session\csessionfactory.h"
#include "..\..\appworld\session\csession.h"
#include "..\..\appworld\Organizingsystem\OrganizingCtrl.h"
#include "../../public/wordsfilter.h"
#include "../../Server/setup/GlobeSetup.h"
#include "../../server/setup/LogSystem.h"
#include "..\nets\Servers.h"
#include "..\..\appworld\Country\CountryHandler.h"
#include "..\..\appworld\PlayerMan\PlayerMan.h"
#include "../DBEntity/Account.h"
#include "..\dbentity/entityManager.h"
#include "../DBEntity/EntityGroup.h"

#include "../GlobalRgnManager.h"
#include "../../AppWorld/Linkman/LinkmanSystem.h"
#include "../public/strtype.h"
#include "../public/StrUnifyInput.h"
#include "../business/CBusinessManager.h"

#include "..\..\appworld\SMclient.h"



// ��ӦLOG��Ϣ
void OnLogMessage(CMessage* pMsg)
{
	DWORD curTime = timeGetTime();
	char pszLogingInfo[512]="";
	switch(pMsg->GetType())
	{
	case MSG_L2W_LOG_QUEST_PLAYERBASELIST://loginserver����һ���˺��µ���һ�����Ϣ�б�
		{
			char szCdkey[ACCOUNT_SIZE];
			pMsg->GetStr(szCdkey,ACCOUNT_SIZE);
			//ȥ���ַ����ұߵĿո�
			TrimRight(szCdkey);

#ifdef _RUNSTACKINFO1_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszLogingInfo);
#endif

			// �ж�DBS�Ƿ�׼������
			if(	!GetGame()->GetInitVarLoadFlag() 
				|| !GetGame()->GetInitRegionLoadFlag()
				|| !GetGame()->GetInitFactionLoadFlag()
				|| !GetGame()->GetInitUnionLoadFlag()
				|| !GetGame()->GetDbIsReady()
				|| !GetGame()->GetInitPersonalHouseLoadFlag()
				|| ( GetInst( Business::CBusinessManager ).IsEnable() &&
				!GetInst( Business::CBusinessManager ).IsDBLoaded() ) )
			{
				AddLogText("���[%s]��¼ʱDBSδȫ��׼�����!",szCdkey);
				CMessage pMsga(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);
				// �����֤����
				pMsga.Add((long)0xFFFFFFFF);
				pMsga.Add((BYTE)2);// flag
				pMsga.Add((szCdkey));// cdkey
				pMsga.Add((short)0);// flag
				pMsga.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}

			// �ж�GS�Ƿ�׼������
			map<DWORD, CGame::tagGameServer>::iterator gsItr = GetGame()->GetGameServerList()->begin();
			for(; gsItr != GetGame()->GetGameServerList()->end(); gsItr++)
			{
				if(!gsItr->second.bConnected)
				{
					CMessage pMsga(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);
					// �����֤����
					pMsga.Add((long)0xFFFFFFFF);
					pMsga.Add((BYTE)2);// flag
					pMsga.Add((szCdkey));// cdkey
					pMsga.Add((short)0);// flag
					pMsga.SendToSocket(GetGame()->GetLSSocketID());
					return;
				}
			}
			
			Account* acc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
			// ����LoginPlayer���󻺳�������
			if(acc)
			{
				acc->ClearAllLoginPlayer();
				acc->SetName(szCdkey);
				GetGame()->GetEntityManager()->CreateLoadAccountSession(acc);
			}
			else // δ�ҵ��������еĶ���
			{
				// ����session
				if(szCdkey[0] != '\0')
				{
					acc = MP_NEW Account;
					if(acc)
					{
						CGUID accGuid;
						CGUID::CreateGUID(accGuid);
						acc->SetGUID(accGuid);
						acc->SetName(szCdkey);
					}
					GetGame()->GetEntityManager()->CreateLoadAccountSession(acc);
					MP_DELETE(acc);
				}
			}
		}
		break;

	case MSG_L2W_LOG_QUEST_PLAYERDATA://loginserver������ҵ���ϸ��Ϣ
		{
			// ȡ����֤��
			long lTestSignCode = pMsg->GetLong();

			char szCdkey[ACCOUNT_SIZE];
			CGUID guid;
			pMsg->GetGUID(guid);//��ȡPLAYERID
			pMsg->GetStr(szCdkey,ACCOUNT_SIZE);//��ȡCDKEY

			// �����֤��
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(szCdkey);
			if(!tLoginInfo)
			{
				char szGUID[128];
				guid.tostring(szGUID);
				_snprintf(pszLogingInfo,512,"MSG_L2W_LOG_QUEST_PLAYERDATA! Account[%s] Is Null!", szGUID, szCdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
				msg.Add((long)0xFFFFFFFF);
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}		
			
			if(tLoginInfo->GetSignCode() != lTestSignCode)
			{
				char szGUID[128];
				guid.tostring(szGUID);
				_snprintf(pszLogingInfo,512,"MSG_W2L_LOG_ANSWER_PLAYERDATA ���ID[%s],Cdkey[%s] ��֤�����!", szGUID, szCdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				
				// ����CLoginPlayer����,���û�ж������״̬ΪOFFLINE��ʾ�Ƿ���¼
				tLoginInfo->SetPlayerState(PLAYER_STATE_VOID);
				CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
				long tmpSignCode = 0;
				GetGame()->GenerateSignCodeByTime(tmpSignCode);
				tLoginInfo->SetSignCode(tmpSignCode);
				msg.Add((long)tLoginInfo->GetSignCode());
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_SIGNCODE_ERROR);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}	
			// ����CLoginPlayer����,���û�ж������״̬ΪOFFLINE��ʾ�Ƿ���¼
			CEntityGroup* lPlayer = (CEntityGroup*)tLoginInfo->FindLoginPlayer(guid);
			// �ҵ�CDBLoginPlayer����
			if( lPlayer && (tLoginInfo->GetPlayerState() == PLAYER_STATE_LOADED_BASE_DATA) )
			{	
				if(GetGame()->GetEntityManager()->ComputeLoginPlayerDelTime(lPlayer) != -1)
				{
					char szGUID[128];
					guid.tostring(szGUID);
					_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_QEUSTPROPERTYCHARDELED), szCdkey);
					PutStringToFile("Login_WS_Info",pszLogingInfo);

					CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);	
					msg.Add((long)-1);
					msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
					msg.Add(szCdkey);
					msg.SendToSocket(GetGame()->GetLSSocketID());

					tLoginInfo->SetPlayerState(PLAYER_STATE_VOID);
					return;
				}
				// ��Ҫ�����ݿ��ȡ
				GetGame()->GetEntityManager()->CreateLoadPlayerDataSession(szCdkey, guid);
				GetGame()->GetEntityManager()->CreateInitLoadMailSession(guid);
			}
			else
			{
				CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
				msg.Add((long)-1);
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());

				tLoginInfo->SetPlayerState(PLAYER_STATE_VOID);
			}
		}
		break;

	case MSG_S2W_LOG_QUEST_PLAYERDATA://gameserver����ĳ����ҵ���ϸ����
		{
			long lTestSignCode = pMsg->GetLong();

			CGUID guid;
			pMsg->GetGUID(guid);
			long lSocketIDC2S = pMsg->GetLong();

			char szCdkey[128];
			pMsg->GetStr(szCdkey, sizeof(szCdkey));
			LONG lIP = pMsg->GetLong();
			
			// �����֤��
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(szCdkey);
			if(!tLoginInfo)
			{
				CPlayer *pPlayer = GetGame()->GetMapPlayer(guid);
				if(pPlayer)
				{
					GetGame()->OnPlayerQuit(pPlayer, CMessage::GetGSIDBySocket(pPlayer->GetGsSocket()), 0L);
				}
				_snprintf(pszLogingInfo,512, "MSG_S2W_LOG_QUEST_PLAYERDATA! Account[%s] Is Null!", szCdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				CMessage msg(MSG_W2S_LOG_ANSWER_PLAYERDATA);
				msg.Add((long)0xFFFFFFFF);
				msg.Add((long)-1);
				msg.Add(guid);
				msg.Add(lSocketIDC2S);
				msg.SendToSocket(pMsg->GetSocketID());   //send to gameserver
				return;
			}	
			
			CEntityGroup* lPlayer = (CEntityGroup*)tLoginInfo->FindLoginPlayer(guid);
			if(!lPlayer)
			{
				CPlayer *pPlayer = GetGame()->GetMapPlayer(guid);
				if(pPlayer)
				{
					GetGame()->OnPlayerQuit(pPlayer, CMessage::GetGSIDBySocket(pPlayer->GetGsSocket()), 0L);
				}
				tLoginInfo->SetPlayerState(PLAYER_STATE_VOID);
				CMessage msg(MSG_W2S_LOG_ANSWER_PLAYERDATA);
				msg.Add((long)0xFFFFFFFF);
				msg.Add((long)-1);
				msg.Add(guid);
				msg.Add(lSocketIDC2S);
				msg.SendToSocket(pMsg->GetSocketID());   //send to gameserver
				return;
			}

			if(tLoginInfo->GetSignCode() != lTestSignCode)
			{
				char szGUID[128];
				guid.tostring(szGUID);
				_snprintf(pszLogingInfo,512, "MSG_S2W_LOG_QUEST_PLAYERDATA ���ID[%s],Cdkey[%s] ��֤�����!", szGUID, lPlayer->GetStringAttr(string("Account")));
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				CMessage msg(MSG_W2S_LOG_ANSWER_PLAYERDATA);
				msg.Add(lTestSignCode);
				msg.Add((long)-3);
				msg.Add(guid);
				msg.Add(lSocketIDC2S);
				msg.SendToSocket(pMsg->GetSocketID());   //send to gameserver
				return;
			}
			
			if( tLoginInfo->GetPlayerState() == PLAYER_STATE_LOADED_CHAR_DATA// ����ѵ�¼����������ȡ��ϸ����
				|| tLoginInfo->GetPlayerState() == PLAYER_STATE_CHANGING_SERVER
				|| tLoginInfo->GetPlayerState() == PLAYER_STATE_ENTERING_GAME )//�����л�������
			{
				CPlayer *pPlayer = GetGame()->GetMapPlayer(guid);
				if(pPlayer)
				{
					long lPlayerState = tLoginInfo->GetPlayerState();
					if(GetGame()->OnPlayerEnter(pPlayer, tLoginInfo, lTestSignCode, lIP, pMsg->GetGSID(), lSocketIDC2S, pMsg->GetSocketID()))
					{
						CMessage msg(MSG_W2S_LOG_ANSWER_PLAYERDATA);
						msg.Add(lTestSignCode);
						msg.Add(1L);//��ӱ��
						msg.Add(guid);
						msg.Add(lSocketIDC2S);
						DBWriteSet setWriteDB;
						msg.GetDBWriteSet(setWriteDB);
						pPlayer->CodeToDataBlock(setWriteDB);
						msg.SendToSocket(pMsg->GetSocketID());   //send to gameserver

						GetGame()->OnPlayerEnterLateCode(pPlayer, tLoginInfo);
						tLoginInfo->SetPlayerState(PLAYER_STATE_ENTERED_GAME);	
						tLoginInfo->SetLoginTimeoutValue(-1);
					}

					if(lPlayerState == PLAYER_STATE_CHANGING_SERVER)
					{	
						//! SM���ӵ���ҵĴ���
						if(pPlayer->GetExID() == CSMClient::GetSM()->GetWatchedGuid())
						{
							//���͵���������ڵ�GS
							CMessage msgLogout(MSG_W2S_OTHER_WATCH);
							msgLogout.Add((DWORD)FALSE);
							msgLogout.Add(pPlayer->GetName());
							msgLogout.SendToSocket(CSMClient::GetSM()->GetWatchedGsSocket());

							CSMClient::GetSM()->SetWatchedPlayer(pPlayer->GetExID(), pPlayer->GetName(), pMsg->GetSocketID());

							CMessage msgLogin(MSG_W2S_OTHER_WATCH);
							msgLogin.Add((DWORD)TRUE);
							msgLogin.Add(pPlayer->GetName());
							msgLogin.SendToSocket(pMsg->GetSocketID());
						}

						pPlayer->SetGsSocket(pMsg->GetSocketID());
						LinkmanSystem::GetInstance().OnPlayerChangeGs(pPlayer);
					}
				}
			}
			else
			{
				CPlayer *pPlayer = GetGame()->GetMapPlayer(guid);
				if(pPlayer)
				{
					CMessage msg(MSG_W2S_OTHER_KICKPLAYER);
					msg.Add((BYTE)AT_ONCE_KICK);
					msg.Add(pPlayer->GetExID());
					msg.SendToSocket(pPlayer->GetGsSocket());
				}
				tLoginInfo->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
				CMessage msg(MSG_W2S_LOG_ANSWER_PLAYERDATA);
				msg.Add((long)0xFFFFFFFF);
				msg.Add((long)-1);
				msg.Add(guid);
				msg.Add(lSocketIDC2S);
				msg.SendToSocket(pMsg->GetSocketID());   //send to gameserver
			}
		}
		break;

	//gameserver֪ͨworldserverһ��������˳���Ϸ, ת��DBS������Ϣ
	case MSG_S2W_LOG_PLAYERQUIT:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			long lFlag = pMsg->GetLong();
			DWORD dwServerKey[SERVER_KEY_DWORD_SIZE];
			for (LONG i = 0; i < SERVER_KEY_DWORD_SIZE; ++i)
			{
				dwServerKey[i] = pMsg->GetDWord();
			}

			char szGUID[128]="";
			PlayerGuid.tostring(szGUID);

			switch(lFlag)
			{
			case 1: // �����˳�
				{
					CPlayer* pPlayer = GetGame()->GetMapPlayer(PlayerGuid);
					if(NULL == pPlayer)
					{
						pPlayer = GetGame()->NewRawPlayer(ePlayerQuitMsg);
						pPlayer->SetExID(PlayerGuid);
						time_t curTime;
						time(&curTime);
						pPlayer->SetLastExitGameTime((DWORD)curTime);
						// ����WS�ϸ��������
						DBReadSet setReadDB;
						pMsg->GetDBReadSet(setReadDB);
						pPlayer->DecodeFromDataBlock(setReadDB);
						if(pPlayer)
						{
							if(GetGame()->GetDbIsReady())
							{
								CWorldServerSession pSession(5000);
								// ���õ�ǰ�˺�״̬
								Account* pAcc = NULL;
								CEntityGroup* tDBLoginPlayer = NULL;
								if(pPlayer->GetAccount())
									pAcc = GetGame()->GetEntityManager()->FindAccount(pPlayer->GetAccount());
								if(pAcc)
								{
									if(pAcc->GetPlayerState() != PLAYER_STATE_VOID)
										pAcc->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
									pAcc->SetCurGamedPlayerGuid(NULL_GUID);
									tDBLoginPlayer = pAcc->FindLoginPlayer(pPlayer->GetExID());
									pSession.NakeSendSaveLoginPlayerDataMsg(pPlayer, tDBLoginPlayer);
								}
								else
								{
									tDBLoginPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[loginplayer]"), pPlayer->GetExID());
									pSession.NakeSendSaveLoginPlayerDataMsg(pPlayer, tDBLoginPlayer);
									GetGame()->GetEntityManager()->DelBaseEntity((CBaseEntity*)tDBLoginPlayer);
								}

								CEntityGroup* tDBPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[player]"), pPlayer->GetExID());
								pSession.NakeSendSavePlayerDataMsg(SAVE_DETAIL_ALL, pPlayer, tDBPlayer, true);
								GetGame()->GetEntityManager()->DelBaseEntity((CBaseEntity*)tDBPlayer);
								GetGame()->DelRawPlayer(pPlayer);
							}
							else
								GetGame()->PushSavePlayer(pPlayer);
						}
						return;
					}
					else
					{
						// ����WS�ϸ��������
						DBReadSet setReadDB;
						pMsg->GetDBReadSet(setReadDB);
						pPlayer->DecodeFromDataBlock(setReadDB);
						pPlayer->SetPlayerLogin(false);
						pPlayer->SetFirstLogin(false);
						time_t curTime;
						time(&curTime);
						pPlayer->SetLastExitGameTime((DWORD)curTime);
						if(GetGame()->GetDbIsReady())
						{
							const char* szCdkey = pPlayer->GetAccount();
							Account* pAcc = NULL;
							if(szCdkey)
							{
								pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
								if(pAcc)
								{
									if(pAcc->GetPlayerState() != PLAYER_STATE_VOID)
										pAcc->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
									pAcc->SetCurGamedPlayerGuid(NULL_GUID);
									pAcc->SetLoginTimeoutValue(timeGetTime());
								}
							}
							GetGame()->GetEntityManager()->CreateSavePlayerDataSession(PlayerGuid, SAVE_DETAIL_ALL);
							GetGame()->GetEntityManager()->CreateSaveLoginPlayerDataSession(PlayerGuid);	
						}
					}

					//! SM���ӵ���ҵĴ���
					if(pPlayer->GetExID() == CSMClient::GetSM()->GetWatchedGuid())
					{
						CMessage Replymsg(MSG_GAPP2SM_WATCH_INFO_Reply);
						Replymsg.Add(GetGame()->GetSetup()->dwGappID);
						Replymsg.Add((BYTE)0);
						Replymsg.SendToSM();

						CMessage msg(MSG_W2S_OTHER_WATCH);
						msg.Add((DWORD)0);
						msg.Add(pPlayer->GetName());
						msg.SendToSocket(CSMClient::GetSM()->GetWatchedGsSocket());

						CSMClient::GetSM()->ClearWatch();
					}

					GetGame()->OnPlayerQuit(pPlayer, pMsg->GetGSID(), dwServerKey);
				}
				break;
			default:
				{
					char szGuid[128];
					PlayerGuid.tostring(szGuid);
					_snprintf(pszLogingInfo,512,"���[%s]�˳�ʱ�쳣,�˳���־[%d],��������߼�!",szGuid, lFlag);
					PutStringToFile("Login_WS_Info",pszLogingInfo);

					CPlayer* pPlayer = GetGame()->GetMapPlayer(PlayerGuid);
					if(pPlayer)
					{
						time_t curTime;
						time(&curTime);
						pPlayer->SetLastExitGameTime((DWORD)curTime);
						Account* pAcc = GetGame()->GetEntityManager()->FindAccount(pPlayer->GetAccount());
						if(pAcc)
						{
							if(pAcc->GetPlayerState() != PLAYER_STATE_VOID)
								pAcc->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
							pAcc->SetCurGamedPlayerGuid(NULL_GUID);
							pAcc->SetLoginTimeoutValue(timeGetTime());
						}
						GetGame()->OnPlayerQuit(pPlayer, pMsg->GetGSID(), dwServerKey);
					}
				}
				break;
			}
		}
		break;

	case MSG_L2W_LOG_FCM_TIME:
		{
			char szCdkey[128];
			pMsg->GetStr(szCdkey, 1228);
			long lTime = pMsg->GetLong();
			if(szCdkey)
			{
				Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
				if(pAcc)
				{
					CPlayer* pPlayer = GetGame()->GetMapPlayer(pAcc->GetCurGamedPlayerGuid());
					if(pPlayer)
					{
						CMessage msg(MSG_W2S_LOG_FCM_TIME);
						msg.Add(pPlayer->GetExID());
						msg.Add(lTime);
						msg.SendToSocket(pPlayer->GetGsSocket());
					}
				}
			}
		}
		break;
	case MSG_L2W_LOG_FCM_BEGIN:
		{
			char szCdkey[128];
			pMsg->GetStr(szCdkey, 1228);
			if(szCdkey)
			{
				Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
				if(pAcc)
				{
					CPlayer* pPlayer = GetGame()->GetMapPlayer(pAcc->GetCurGamedPlayerGuid());
					if(pPlayer)
					{
						CMessage msg(MSG_W2S_OTHER_KICKPLAYER);
						msg.Add((BYTE)FCM_KICK_PLAYER);
						msg.Add(pPlayer->GetExID());
						msg.SendToSocket(pPlayer->GetGsSocket());
					}
				}
			}
		}
		break;

	case MSG_L2W_LOG_KICKPLAYER://����CDKEY�߳�һ�����
		{
			char strCDKey[256];
			pMsg->GetStr(strCDKey,256);
			Account* pAcc = GetGame()->GetEntityManager()->FindAccount(strCDKey);
			if(pAcc 
				&& pAcc->GetPlayerState() != PLAYER_STATE_ENTERED_GAME)
			{
				pAcc->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
				pAcc->SetLoginTimeoutValue(timeGetTime());
			}
		}
		break;

	case MSG_L2W_LOG_REPEAT_LOGIN:
		{
			char strCDKey[256];
			pMsg -> GetStr( strCDKey, 256 );

			// �����ʺ��Ƿ��н�ɫ����Ϸ��
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(strCDKey);
			if(tLoginInfo)
			{
				if(tLoginInfo->GetEntityGroupMap().size())
				{
					CGUID CurGamedPlayerGuid = tLoginInfo->GetCurGamedPlayerGuid();
					if( CurGamedPlayerGuid != NULL_GUID)
					{
						CPlayer* pPlayer = GetGame()->GetMapPlayer(CurGamedPlayerGuid);
						if(pPlayer)
						{
							long gsid = GetGame()->GetGlobalRgnManager()->FindGSIDByRgnID(pPlayer->GetRegionExID());
							
							if(gsid != -1)
							{
							CMessage msg( MSG_W2S_LOG_REPEAT_LOGIN );
							msg.Add( CurGamedPlayerGuid );
							msg.SendGSID(gsid);
							}
							else
							{
								tLoginInfo->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
								CMessage msg(MSG_W2L_LOG_PLAYERQUIT);
								msg.Add(strCDKey);
								msg.Add(0L);
								msg.SendToSocket(GetGame()->GetLSSocketID());//send to loginserver
							}
							
							_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_REPEATLOGINTOGS), strCDKey);
							PutStringToFile("Login_WS_Info",pszLogingInfo);
							return;
						}
					}
				}
			}

			CMessage msg(MSG_W2L_LOG_PLAYERQUIT);
			msg.Add(strCDKey);
			msg.Add(0L);
			msg.SendToSocket(GetGame()->GetLSSocketID());//send to loginserver
		}
		break;

	case MSG_L2W_LOG_DELETEROLE://loginserver����ɾ��һ����ɫ
		{
			CGUID guid;
			DWORD dwIP;
			char szIP[64];
			char cdkey[ACCOUNT_SIZE];
			pMsg->GetStr(cdkey,ACCOUNT_SIZE);	//cdkey
			pMsg->GetGUID(guid);	//playerid
			dwIP = pMsg->GetDWord();
		
			char szGUID[128];
			guid.tostring(szGUID);
			CEntityGroup* lPlayer = NULL;
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(cdkey);
			if(!tLoginInfo)
			{
				_snprintf(pszLogingInfo,512,"DeleteRole::δ�ȴ����˺Ŷ���[%s]!", cdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Send();//send to loginserver
				return;
			}
			lPlayer = tLoginInfo->FindLoginPlayer(guid);
			if(NULL == lPlayer)
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Send();//send to loginserver
				_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_DELCHARNOCHAROBJ), cdkey, szGUID);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				return;
			}
			if( tLoginInfo->GetPlayerState() != PLAYER_STATE_LOADED_BASE_DATA )
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Send();//send to loginserver
		
				_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_DELCHARINVALID), 
					tLoginInfo->GetPlayerState(), cdkey, szGUID);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				return;
			}

			// �ж������ɫ�ȼ��Ƿ�����ɾ������
			if ((DWORD)lPlayer->GetLongAttr(string("Levels"))<CGlobeSetup::GetSetup()->dwDelLevelCondition)
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Add((long)4);
				msg.Send();//send to loginserver
				return;
			}
			
			sprintf(szIP, "%d.%d.%d.%d", LOBYTE(LOWORD(dwIP)), HIBYTE(LOWORD(dwIP)), LOBYTE(HIWORD(dwIP)), HIBYTE(HIWORD(dwIP)));

			time_t tDelDate;		// ɾ������
			time(&tDelDate);		// ��ǰ���ڵ�DWORD

			//! ��ϵ��ϵͳ
			LinkmanSystem::GetInstance().OnPlayerDelStateChange(guid, TRUE);

			//ɾ��������ڵİ����֯
			int nResult = GetOrganiCtrl()->OnDeleteRole(guid);
			//ӵ������ 
			if( nResult == 1 )
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Add((long)1);
				msg.Send();//send to loginserver

				return;
			}
			//�˳�ͬ��
			else if( nResult == 2 )
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Add((long)2);
				msg.Send();//send to loginserver
				return;
			}
			//��ɢͬ��
			else if(nResult == 3)
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Add((long)3);
				msg.Send();//send to loginserver
				return;
			}

			// �������ݼ���MSG���͸�DBS
			GetGame()->GetEntityManager()->CreateUpdateLoginPlayerDelTimeSession(cdkey, guid);

			char szGuid[40] = {0};
			guid.tostring(szGuid);			
			_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_DELCHAROK), szGuid);
			PutStringToFile("Login_WS_Info",pszLogingInfo);
			return;
		}
		break;

	case MSG_L2W_LOG_RESTOREROLE:
		{
			CGUID guid;
			char cdkey[ACCOUNT_SIZE];
			pMsg->GetStr(cdkey,ACCOUNT_SIZE);	//cdkey
			pMsg->GetGUID(guid);	//playerid

			char szGUID[128];
			guid.tostring(szGUID);
			CEntityGroup* lPlayer = NULL;//GetGame()->GetEntityManager()->FindLoginPlayer(guid);
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(cdkey);
			if(!tLoginInfo)
			{
				_snprintf(pszLogingInfo,512,"RestoreRole::δ�ȴ����˺Ŷ���[%s]!", cdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				CMessage msg(MSG_W2L_LOG_RESTOREROLE);
				msg.Add((char)LOGIN_RESTOREROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Send();//send to loginserver
				return;
			}
			lPlayer = tLoginInfo->FindLoginPlayer(guid);
			if(NULL == lPlayer)
			{
				CMessage msg(MSG_W2L_LOG_RESTOREROLE);
				msg.Add((char)LOGIN_RESTOREROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Send();//send to loginserver
				_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_RESTORCHARNOCHAROBJ), cdkey, szGUID);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				return;
			}
			if(tLoginInfo->GetPlayerState() != PLAYER_STATE_LOADED_BASE_DATA)
			{
				CMessage msg(MSG_W2L_LOG_RESTOREROLE);
				msg.Add((char)LOGIN_RESTOREROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.Send();//send to loginserver
				_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_RESTORECHARVALID), 
					tLoginInfo->GetPlayerState(), cdkey, szGUID);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				return;
			}

			//! ��ϵ��ϵͳ
			LinkmanSystem::GetInstance().OnPlayerDelStateChange(guid, TRUE);

			// �������ݼ���MSG���͸�DBS
			GetGame()->GetEntityManager()->CreateRestoreLoginPlayerDelTimeSession(cdkey, guid);
		}
		break;

	case MSG_L2W_LOG_CREATEROLE://loginserver���󴴽�һ����ɫ
		{
			char szName[50],szCdkey[ACCOUNT_SIZE];
			pMsg->GetStr(szName,50);
			char nOccupation = pMsg->GetChar();
			char nSex = pMsg->GetChar();
			BYTE nHead = pMsg->GetByte();
			BYTE nFace = pMsg->GetByte();
			BYTE btCountry = pMsg->GetByte();
			BYTE byConstellation = pMsg->GetByte();
			//�Ƿ�ѡ�����Ƽ��Ĺ���
			BYTE bySelectRecommCountry = pMsg->GetByte();
			bool bSelectRecommCountry = (bySelectRecommCountry==0?false:true);
			BYTE btMaxCharactersNum = CGlobeSetup::GetSetup()->btMaxCharactersNum;
			pMsg->GetStr(szCdkey,ACCOUNT_SIZE);

			if(szName[0] == '\0') // ����Ϊ��
			{
				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_NAMEINVALID);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}

			// ��ϴ����б�����ݿ⣬����ɫ�Ƿ񱻽�����
			// ���ְҵ���Ա��Ƿ��������
			CGlobeSetup::GetSetup()->btMaxCharactersNum;
			bool bValid = false;
			//�ж�ְҵ�Ƿ�Ϸ�
			if(nOccupation < 0 || nOccupation >= OCC_Max)
				return;
			//�ж��Ա��Ƿ�Ϸ�
			if( nSex < 0 || nSex >= CPlayerList::SEX_NUMS)
				return;
			//�ж�����
			if(byConstellation < CONST_Aries || byConstellation > CONST_Pisces)
				return;

			// ������
			bValid = false;
			if( !GetCountryHandler()->GetCountry(btCountry) )
			{
				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_DBERROR);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());

				_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_CREATECHARNOCOUNTRY),btCountry);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				return;
			}

			// ��������Ƿ����
			string strName(szName);
			if (!CWordsFilter::GetInstance()->Check(strName))
			{
				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_NAMEINVALID);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}

			// ��ɫ��Ҳ��ʹ��(��list��MapPlayer ���DBS)
			if(	GetGame()->IsNameExistInMapPlayer(szName) )
			{
				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_NAMEEXIST);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}

			// �ټ��һ���Ƿ����и���ҵĴ�������(ͨ��LoginPlayer����)
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(szCdkey);
			if(!tLoginInfo)
			{
				_snprintf(pszLogingInfo,512,"MSG_W2L_LOG_CREATEROLE! LoginInfo[%s] Is Null!", szCdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);

				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_FAILED);
				msg.Add(szCdkey);
				msg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}

			if(tLoginInfo->GetPlayerState() == PLAYER_STATE_CREATING) return;
			
			// �ٵ�DB�����в���һ��
			Account* pAcc = NULL;
			CEntityManager::AccountMapItr accItr = GetGame()->GetEntityManager()->GetAccountMap().begin();
			for(; accItr != GetGame()->GetEntityManager()->GetAccountMap().end(); accItr++)
			{
				pAcc = accItr->second;
				if(pAcc->GetEntityGroupMap().size() == 1)
				{
					CEntityGroup* pLoginPlayerGroup = (CEntityGroup*)pAcc->GetEntityGroupMap().begin()->second;
					map<CGUID, CBaseEntity*>::iterator setItr;
					for(setItr=pLoginPlayerGroup->GetEntityGroupMap().begin(); setItr!=pLoginPlayerGroup->GetEntityGroupMap().end(); setItr++)
					{
						const char* szEPName = setItr->second->GetStringAttr(string("Name"));
						if(szEPName)
						{
							if(strcmp(szName, szEPName) == 0)
							{
								// �ҵ�����ҵ�LoginPlayer���� �����ô�����Ϣ
								CMessage msg(MSG_W2L_LOG_CREATEROLE);
								msg.Add((BYTE)LOGIN_CREATEROLE_NAMEEXIST);
								msg.Add(szCdkey);
								msg.SendToSocket(GetGame()->GetLSSocketID());
								return;
							}
						}
					}
				}
			}

			
			// ����һ��������һỰ
			GetGame()->GetEntityManager()->CreatePlayerCreateSession(szCdkey, szName, nOccupation, nSex, nHead, nFace,
				btCountry, btMaxCharactersNum,byConstellation,bSelectRecommCountry);		
			_snprintf(pszLogingInfo,512,CStringReading::LoadString(IDS_WS_LOGMESSAGE, STR_WS_LOGMESSAGE_SENDCREATETODBS), szName);
			PutStringToFile("Login_WS_Info",pszLogingInfo);
		}
		break;
	}
}
/*
<!--�ͻ�����������־-->
<Table name="t550_client_in_game_log" log_no="550" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="alone"    Item_name="cdkey"         data_type="str128"></item><!--IP-->
	<item type="alone"    Item_name="ip"            data_type="int32"></item><!--CDKEY-->
	<item type="player"   head_name=""></item>
	<item type="pos"      head_name=""></item>
	<item type="alone"    Item_name="region_guid"   data_type="int32"></item><!--level-->
	<item type="alone"    Item_name="level"         data_type="int32"></item><!--level-->
	<item type="alone"    Item_name="gold"          data_type="int32"></item><!--gold-->
	<item type="alone"    Item_name="silver"        data_type="int32"></item><!--silver-->
	<item type="alone"    Item_name="flag"          data_type="int32"></item><!--�������ͣ�1=���ߡ�2=����-->
	<item type="alone"    Item_name="online_time"   data_type="int32"></item><!--�˴β������߷��������������ͣ��˴�Ϊ0���������ͣ��˴���ֵ-->
</Table>
*/
bool LogicLogInterface::logT550_client_in_game_log(CPlayer *pPlayer, LONG lState)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;
	if(NULL == m_pLogClient)return false;
	if(NULL == pPlayer) return false;

	string strTime;
	GetCurrTimeString(strTime);

	const char *pRegionName = "";
	map<LONG, CGlobalRgnManager::tagRegion> &mapRegionList = *(GetGame()->GetGlobalRgnManager()->GetTemplateRegionListMap());
	map<LONG, CGlobalRgnManager::tagRegion>::iterator ite = mapRegionList.find(pPlayer->GetRegionID());
	if (ite != mapRegionList.end())
	{
		CWorldRegion *pWorldRegion = ite->second.pRegion;
		if (NULL != pWorldRegion)
		{
			pRegionName = pWorldRegion->GetName();
		}
	}

	float fPosX = pPlayer->GetPosX();
	float fPosY = pPlayer->GetPosY();

	//! ת��IP
	;
	char szTmp[128] = {0};
	UCHAR arrIp[4] = {0};
	LONG lIP = pPlayer->GetIP();
	memmove(arrIp, &lIP, sizeof(LONG));
	sprintf(szTmp, "%u.%u.%u.%u", (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);

	LONG lBankMoney = 0;
	CGoodsContainer* pBank = pPlayer->m_PlayerDepot.FindContainer(eDCT_Gold);
	if(NULL != pBank)
		lBankMoney = pBank->GetGoodsAmount();

	DWORD OnlineTime = 0;
	if(2 == lState)
		OnlineTime = (timeGetTime() - pPlayer->GetLoginTime()) / (1000 * 60);

	return m_pLogClient->SendLogToServer( 
		550, strTime.c_str(),
		pPlayer->GetAccount(),
		szTmp,
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		pPlayer->GetRegionID(),
		pRegionName,
		&fPosX,
		&fPosY,
		&(pPlayer->GetRegionExID()),
		pPlayer->GetLevel(),
		pPlayer->GetMoney() + lBankMoney,
		pPlayer->GetSilver(),
		lState,
		OnlineTime
		);
}
