#include "StdAfx.h"

#include "../AH/OrderMgr.h"
#include "../Business/BusinessManager.h"
#include "../Country/CountryHandler.h"
#include "../DBEntity/Account.h"
#include "../DBEntity/EntityGroup.h"
#include "../DBEntity/EntityManager.h"
#include "../GlobalRgnManager.h"
#include "../Goods/Goods.h"
#include "../Linkman/LinkmanSystem.h"
#include "../OrganizingSystem/OrganizingCtrl.h"
#include "../Player.h"
#include "../PlayerMan/PlayerMan.h"
#include "../Session/SessionFactory.h"
#include "../Session/Session.h"
#include "../../../../Public/ServerPublic/Server/Common/WordsFilter.h"
#include "../../../../Public/Setup/GlobalSetup.h"
#include "../../../../Public/Setup/PlayerList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

			//_snprintf(pszLogingInfo,512,"the cdkey(%s) MSG_L2W_LOG_QUEST_PLAYERBASELIST!", szCdkey);
			//PutStringToFile("Login_WS_Info",pszLogingInfo);
#ifdef _RUNSTACKINFO1_
			CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszLogingInfo);
#endif

			// �ж�DBS�Ƿ�׼������
			if (!GetGame().GetInitVarLoadFlag() 
				|| !GetGame().GetInitRegionLoadFlag()
				|| !GetGame().GetInitFactionLoadFlag()
				|| !GetGame().GetInitUnionLoadFlag()
				|| !GetNetHandler().IsServerReady(NF_DB_SERVER)
				|| !GetGame().GetInitPersonalHouseLoadFlag()
				|| ( GetInst( Business::CBusinessManager ).IsEnable() &&
				!GetInst( Business::CBusinessManager ).IsDBLoaded() ) 
                || !GetInst( AH::OrderMgr ).IsDBLoaded() )
			{
				Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGINMSG_40", szCdkey));
				CMessage pMsga(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);
				// �����֤����
				pMsga.Add((long)0xFFFFFFFF);
				pMsga.Add((BYTE)2);// flag
				pMsga.Add((szCdkey));// cdkey
				pMsga.Add((short)0);// flag
				pMsga.SendToServer(NF_LOGIN_SERVER);
				return;
			}

			// �ж�GS�Ƿ�׼������
            if (!CMessage::IsGSReady(-1))
            {
                CMessage pMsga(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);
                // �����֤����
                pMsga.Add((long)0xFFFFFFFF);
                pMsga.Add((BYTE)2);// flag
                pMsga.Add((szCdkey));// cdkey
                pMsga.Add((short)0);// flag
                pMsga.SendToServer(NF_LOGIN_SERVER);
                return;
            }
			
			Account* acc = GetGame().GetEntityManager()->FindAccount(szCdkey);
			// ����LoginPlayer���󻺳�������
			if(acc)
			{
				acc->ClearAllLoginPlayer();
				acc->SetName(szCdkey);
				GetGame().GetEntityManager()->CreateLoadAccountSession(acc);
			}
			else // δ�ҵ��������еĶ���
			{
				// ����session
				if(szCdkey[0] != '\0')
				{
					acc = new Account;
					if(acc)
					{
						CGUID accGuid;
						CGUID::CreateGUID(accGuid);
						acc->SetGUID(accGuid);
						acc->SetName(szCdkey);
					}
					GetGame().GetEntityManager()->CreateLoadAccountSession(acc);
					SAFE_DELETE(acc);
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
			Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(szCdkey);
			if(!tLoginInfo)
			{
				char szGUID[128];
				guid.tostring(szGUID);
// 				_snprintf(pszLogingInfo,512,"MSG_L2W_LOG_QUEST_PLAYERDATA! Account[%s] Is Null!", szGUID, szCdkey);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);
                Log4c::Warn(ROOT_MODULE,"MSG_L2W_LOG_QUEST_PLAYERDATA! Account[%s] Is Null!", szCdkey);
				CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
				msg.Add((long)0xFFFFFFFF);
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
				msg.Add(szCdkey);
				msg.SendToServer(NF_LOGIN_SERVER);
				return;
			}		
			
			if(tLoginInfo->GetSignCode() != lTestSignCode)
			{
				char szGUID[128];
				guid.tostring(szGUID);
// 				_snprintf(pszLogingInfo,512,"MSG_W2L_LOG_ANSWER_PLAYERDATA ���ID[%s],Cdkey[%s] ��֤�����!", szGUID, szCdkey);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,"MSG_W2L_LOG_ANSWER_PLAYERDATA ���ID[%s],Cdkey[%s] ��֤�����!", szGUID, szCdkey);
				
				// ����CLoginPlayer����,���û�ж������״̬ΪOFFLINE��ʾ�Ƿ���¼
				tLoginInfo->SetPlayerState(PLAYER_STATE_VOID);
				CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
				long tmpSignCode = 0;
				GetGame().GenerateSignCodeByTime(tmpSignCode);
				tLoginInfo->SetSignCode(tmpSignCode);
				msg.Add((long)tLoginInfo->GetSignCode());
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_SIGNCODE_ERROR);
				msg.Add(szCdkey);
				msg.SendToServer(NF_LOGIN_SERVER);
				return;
			}	
			// ����CLoginPlayer����,���û�ж������״̬ΪOFFLINE��ʾ�Ƿ���¼
			CEntityGroup* lPlayer = (CEntityGroup*)tLoginInfo->FindLoginPlayer(guid);
			// �ҵ�CDBLoginPlayer����
			if( lPlayer && (tLoginInfo->GetPlayerState() == PLAYER_STATE_LOADED_BASE_DATA) )
			{	
				if(GetGame().GetEntityManager()->ComputeLoginPlayerDelTime(lPlayer) != -1)
				{
					char szGUID[128];
					guid.tostring(szGUID);
// 					_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_0"), szCdkey);
// 					PutStringToFile("Login_WS_Info",pszLogingInfo);

                    Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_0", szCdkey));

					CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);	
					msg.Add((long)-1);
					msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
					msg.Add(szCdkey);
					msg.SendToServer(NF_LOGIN_SERVER);

					tLoginInfo->SetPlayerState(PLAYER_STATE_VOID);
					return;
				}
				// ��Ҫ�����ݿ��ȡ
				GetGame().GetEntityManager()->CreateLoadPlayerDataSession(szCdkey, guid);
				GetGame().GetEntityManager()->CreateInitLoadMailSession(guid);
			}
			else
			{
				CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
				msg.Add((long)-1);
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
				msg.Add(szCdkey);
				msg.SendToServer(NF_LOGIN_SERVER);

				//char szGUID[128];
				//guid.tostring(szGUID);
				/*_snprintf(pszLogingInfo,512, AppFrame::GetText("WS_LOGMESSAGE_QUESTPROPERTYNOCREATEORLOADBASE"), szGUID, szCdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);*/
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
			Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(szCdkey);
			if(!tLoginInfo)
			{
				CPlayer *pPlayer = GetGame().GetMapPlayer(guid);
				if(pPlayer)
				{
					GetGame().OnPlayerQuit(pPlayer, CMessage::GetGSIDBySocket(pPlayer->GetGsSocket()), 0L);
				}
// 				_snprintf(pszLogingInfo,512, "MSG_S2W_LOG_QUEST_PLAYERDATA! Account[%s] Is Null!", szCdkey);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Warn(ROOT_MODULE,"MSG_S2W_LOG_QUEST_PLAYERDATA! Account[%s] Is Null!", szCdkey);
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
				CPlayer *pPlayer = GetGame().GetMapPlayer(guid);
				if(pPlayer)
				{
					GetGame().OnPlayerQuit(pPlayer, CMessage::GetGSIDBySocket(pPlayer->GetGsSocket()), 0L);
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
// 				_snprintf(pszLogingInfo,512, "MSG_S2W_LOG_QUEST_PLAYERDATA ���ID[%s],Cdkey[%s] ��֤�����!", szGUID, lPlayer->GetStringAttr(string("Account")));
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Warn(ROOT_MODULE,"MSG_S2W_LOG_QUEST_PLAYERDATA ���ID[%s],Cdkey[%s] ��֤�����!", 
                    szGUID, lPlayer->GetStringAttr(string("Account")));
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
				CPlayer *pPlayer = GetGame().GetMapPlayer(guid);
				if(pPlayer)
				{
					long lPlayerState = tLoginInfo->GetPlayerState();
					if(GetGame().OnPlayerEnter(pPlayer, tLoginInfo, lTestSignCode, lIP, pMsg->GetGSID(), lSocketIDC2S, pMsg->GetSocketID()))
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

						GetGame().OnPlayerEnterLateCode(pPlayer, tLoginInfo);
						tLoginInfo->SetPlayerState(PLAYER_STATE_ENTERED_GAME);	
						tLoginInfo->SetLoginTimeoutValue(-1);
					}

					if(lPlayerState == PLAYER_STATE_CHANGING_SERVER)
					{	
						pPlayer->SetGsSocket(pMsg->GetSocketID());
						LinkmanSystem::GetInstance().OnPlayerChangeGs(pPlayer);
					}
				}
			}
			else
			{
				CPlayer *pPlayer = GetGame().GetMapPlayer(guid);
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
					CPlayer* pPlayer = GetGame().GetMapPlayer(PlayerGuid);
					if(NULL == pPlayer)
					{
						pPlayer = GetGame().NewRawPlayer(ePlayerQuitMsg);
						pPlayer->SetExID(PlayerGuid);
						
						// ����WS�ϸ��������
						DBReadSet setReadDB;
						pMsg->GetDBReadSet(setReadDB);
						pPlayer->DecodeFromDataBlock(setReadDB);
						if(pPlayer)
						{
							if(GetNetHandler().IsServerReady(NF_DB_SERVER))
							{
								CWorldServerSession pSession(5000);
								// ���õ�ǰ�˺�״̬
								Account* pAcc = NULL;
								CEntityGroup* tDBLoginPlayer = NULL;
								if(pPlayer->GetAccount())
									pAcc = GetGame().GetEntityManager()->FindAccount(pPlayer->GetAccount());
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
									tDBLoginPlayer = (CEntityGroup*)GetGame().GetEntityManager()->NewBaseEntity(string("[loginplayer]"), pPlayer->GetExID());
									pSession.NakeSendSaveLoginPlayerDataMsg(pPlayer, tDBLoginPlayer);
									GetGame().GetEntityManager()->DelBaseEntity((CBaseEntity*)tDBLoginPlayer);
								}

								CEntityGroup* tDBPlayer = (CEntityGroup*)GetGame().GetEntityManager()->NewBaseEntity(string("[player]"), pPlayer->GetExID());
								pSession.NakeSendSavePlayerDataMsg(SAVE_DETAIL_ALL, pPlayer, tDBPlayer, true);
								GetGame().GetEntityManager()->DelBaseEntity((CBaseEntity*)tDBPlayer);
								GetGame().DelRawPlayer(pPlayer);
							}
							else
								GetGame().PushSavePlayer(pPlayer);
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

						if(GetNetHandler().IsServerReady(NF_DB_SERVER))
						{
							const char* szCdkey = pPlayer->GetAccount();
							Account* pAcc = NULL;
							if(szCdkey)
							{
								pAcc = GetGame().GetEntityManager()->FindAccount(szCdkey);
								if(pAcc)
								{
									if(pAcc->GetPlayerState() != PLAYER_STATE_VOID)
										pAcc->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
									pAcc->SetCurGamedPlayerGuid(NULL_GUID);
									pAcc->SetLoginTimeoutValue(timeGetTime());
								}
							}
							GetGame().GetEntityManager()->CreateSavePlayerDataSession(PlayerGuid, SAVE_DETAIL_ALL);
							GetGame().GetEntityManager()->CreateSaveLoginPlayerDataSession(PlayerGuid);	
						}
					}
					GetGame().OnPlayerQuit(pPlayer, pMsg->GetGSID(), dwServerKey);
				}
				break;
			default:
				{
					char szGuid[128];
					PlayerGuid.tostring(szGuid);
// 					_snprintf(pszLogingInfo,512,"���[%s]�˳�ʱ�쳣,�˳���־[%d],��������߼�!",szGuid, lFlag);
// 					PutStringToFile("Login_WS_Info",pszLogingInfo);

                    Log4c::Warn(ROOT_MODULE,"���[%s]�˳�ʱ�쳣,�˳���־[%d],��������߼�!",szGuid, lFlag);

					CPlayer* pPlayer = GetGame().GetMapPlayer(PlayerGuid);
					if(pPlayer)
					{
						Account* pAcc = GetGame().GetEntityManager()->FindAccount(pPlayer->GetAccount());
						if(pAcc)
						{
							if(pAcc->GetPlayerState() != PLAYER_STATE_VOID)
								pAcc->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
							pAcc->SetCurGamedPlayerGuid(NULL_GUID);
							pAcc->SetLoginTimeoutValue(timeGetTime());
						}
						GetGame().OnPlayerQuit(pPlayer, pMsg->GetGSID(), dwServerKey);
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
				Account* pAcc = GetGame().GetEntityManager()->FindAccount(szCdkey);
				if(pAcc)
				{
					CPlayer* pPlayer = GetGame().GetMapPlayer(pAcc->GetCurGamedPlayerGuid());
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
				Account* pAcc = GetGame().GetEntityManager()->FindAccount(szCdkey);
				if(pAcc)
				{
					CPlayer* pPlayer = GetGame().GetMapPlayer(pAcc->GetCurGamedPlayerGuid());
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
			Account* pAcc = GetGame().GetEntityManager()->FindAccount(strCDKey);
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
			pMsg->GetStr( strCDKey, 256 );

			// �����ʺ��Ƿ��н�ɫ����Ϸ��
			Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(strCDKey);
			if(tLoginInfo)
			{
				if(tLoginInfo->GetEntityGroupMap().size())
				{
					CGUID CurGamedPlayerGuid = tLoginInfo->GetCurGamedPlayerGuid();
					if( CurGamedPlayerGuid != NULL_GUID)
					{
						CPlayer* pPlayer = GetGame().GetMapPlayer(CurGamedPlayerGuid);
						if(pPlayer)
						{
							long gsid = GetGame().GetGlobalRgnManager()->FindGSIDByRgnID(pPlayer->GetRegionExID());
							
							if(gsid != -1)
							{
							CMessage msg( MSG_W2S_LOG_REPEAT_LOGIN );
							msg.Add( CurGamedPlayerGuid );
							msg.SendToGS(gsid);
							}
							else
							{
								tLoginInfo->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
								CMessage msg(MSG_W2L_LOG_PLAYERQUIT);
								msg.Add(strCDKey);
								msg.Add(0L);
								msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
							}
							
// 							_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_12"), strCDKey);
// 							PutStringToFile("Login_WS_Info",pszLogingInfo);

                            Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_12", strCDKey));
							return;
						}
					}
				}
			}

			CMessage msg(MSG_W2L_LOG_PLAYERQUIT);
			msg.Add(strCDKey);
			msg.Add(0L);
			msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
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
			CEntityGroup* lPlayer = NULL;//GetGame().GetEntityManager()->FindLoginPlayer(guid);
			Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(cdkey);
			if(!tLoginInfo)
			{
// 				_snprintf(pszLogingInfo,512,"DeleteRole::δ�ȴ����˺Ŷ���[%s]!", cdkey);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);
                Log4c::Warn(ROOT_MODULE,"DeleteRole::δ�ȴ����˺Ŷ���[%s]!", cdkey);
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
				return;
			}
			lPlayer = tLoginInfo->FindLoginPlayer(guid);
			if(NULL == lPlayer)
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
// 				_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_14"), cdkey, szGUID);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_14", cdkey, szGUID));
				return;
			}
			if( tLoginInfo->GetPlayerState() != PLAYER_STATE_LOADED_BASE_DATA )
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
		
// 				_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_15"), 
// 					tLoginInfo->GetPlayerState(), cdkey, szGUID);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_15",tLoginInfo->GetPlayerState(), cdkey, szGUID));
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
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver

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
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
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
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
				return;
			}

			// �������ݼ���MSG���͸�DBS
			GetGame().GetEntityManager()->CreateUpdateLoginPlayerDelTimeSession(cdkey, guid);

			char szGuid[40] = {0};
			guid.tostring(szGuid);			
// 			_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_16"), szGuid);
// 			PutStringToFile("Login_WS_Info",pszLogingInfo);

            Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_16", szGuid));
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
			CEntityGroup* lPlayer = NULL;//GetGame().GetEntityManager()->FindLoginPlayer(guid);
			Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(cdkey);
			if(!tLoginInfo)
			{
// 				_snprintf(pszLogingInfo,512,"RestoreRole::δ�ȴ����˺Ŷ���[%s]!", cdkey);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);
                Log4c::Warn(ROOT_MODULE,"RestoreRole::δ�ȴ����˺Ŷ���[%s]!", cdkey);
				CMessage msg(MSG_W2L_LOG_RESTOREROLE);
				msg.Add((char)LOGIN_RESTOREROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
				return;
			}
			lPlayer = tLoginInfo->FindLoginPlayer(guid);
			if(NULL == lPlayer)
			{
				CMessage msg(MSG_W2L_LOG_RESTOREROLE);
				msg.Add((char)LOGIN_RESTOREROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
// 				_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_17"), cdkey, szGUID);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_17", cdkey, szGUID));
				return;
			}
			if(tLoginInfo->GetPlayerState() != PLAYER_STATE_LOADED_BASE_DATA)
			{
				CMessage msg(MSG_W2L_LOG_RESTOREROLE);
				msg.Add((char)LOGIN_RESTOREROLE_FAILED);
				msg.Add(guid);
				msg.Add(cdkey);
				msg.SendToServer(NF_LOGIN_SERVER);//send to loginserver
// 				_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_18"), 
// 					tLoginInfo->GetPlayerState(), cdkey, szGUID);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_18", 
                    tLoginInfo->GetPlayerState(), cdkey, szGUID));
				return;
			}

			//! ��ϵ��ϵͳ
			LinkmanSystem::GetInstance().OnPlayerDelStateChange(guid, TRUE);

			// �������ݼ���MSG���͸�DBS
			GetGame().GetEntityManager()->CreateRestoreLoginPlayerDelTimeSession(cdkey, guid);
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
			BYTE btMaxCharactersNum = (BYTE)GlobalSetup::GetSetup()->btMaxCharactersNum;
			pMsg->GetStr(szCdkey,ACCOUNT_SIZE);

			if(szName[0] == '\0') // ����Ϊ��
			{
				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_NAMEINVALID);
				msg.Add(szCdkey);
				msg.SendToServer(NF_LOGIN_SERVER);
				return;
			}

			// ��ϴ����б�����ݿ⣬����ɫ�Ƿ񱻽�����
			// ���ְҵ���Ա��Ƿ��������
			GlobalSetup::GetSetup()->btMaxCharactersNum;
			bool bValid = false;
			//�ж�ְҵ�Ƿ�Ϸ�
			if(nOccupation < 0 || nOccupation >= OCC_Max)
				return;
			//�ж��Ա��Ƿ�Ϸ�
			if( nSex < 0 || nSex >= PlayerList::SEX_NUMS)
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
				msg.SendToServer(NF_LOGIN_SERVER);

// 				_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_19"),btCountry);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_19",btCountry));
				return;
			}

			// ��������Ƿ����
			string strName(szName);
			if (!CWordsFilter::GetInstance()->Check(strName))
			{
				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_NAMEINVALID);
				msg.Add(szCdkey);
				msg.SendToServer(NF_LOGIN_SERVER);
				return;
			}

			// ��ɫ��Ҳ��ʹ��(��list��MapPlayer ���DBS)
			if(	GetGame().IsNameExistInMapPlayer(szName) )
			{
				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_NAMEEXIST);
				msg.Add(szCdkey);
				msg.SendToServer(NF_LOGIN_SERVER);
				return;
			}

			// �ټ��һ���Ƿ����и���ҵĴ�������(ͨ��LoginPlayer����)
			Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(szCdkey);
			if(!tLoginInfo)
			{
// 				_snprintf(pszLogingInfo,512,"MSG_W2L_LOG_CREATEROLE! LoginInfo[%s] Is Null!", szCdkey);
// 				PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,"MSG_W2L_LOG_CREATEROLE! LoginInfo[%s] Is Null!", szCdkey);

				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				msg.Add((BYTE)LOGIN_CREATEROLE_FAILED);
				msg.Add(szCdkey);
				msg.SendToServer(NF_LOGIN_SERVER);
				return;
			}

			if(tLoginInfo->GetPlayerState() == PLAYER_STATE_CREATING) return;
			
			// �ٵ�DB�����в���һ��
			Account* pAcc = NULL;
			CEntityManager::AccountMapItr accItr = GetGame().GetEntityManager()->GetAccountMap().begin();
			for(; accItr != GetGame().GetEntityManager()->GetAccountMap().end(); accItr++)
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
								msg.SendToServer(NF_LOGIN_SERVER);
								return;
							}
						}
					}
				}
			}

			
			// ����һ��������һỰ
			GetGame().GetEntityManager()->CreatePlayerCreateSession(szCdkey, szName, nOccupation, nSex, nHead, nFace,
				btCountry, btMaxCharactersNum,byConstellation,bSelectRecommCountry);		
// 			_snprintf(pszLogingInfo,512,AppFrame::GetText("WS_LOGMESSAGE_20"), szName);
// 			PutStringToFile("Login_WS_Info",pszLogingInfo);

            Log4c::Trace(ROOT_MODULE,FormatText("WS_LOGMESSAGE_20", szName));
		}
		break;
	}
}

