
#include "stdafx.h"
#include "..\..\..\..\nets\networld\message.h"
#include "..\nets\Servers.h"
#include "..\goods\cgoods.h"
#include "..\..\worldserver\game.h"
#include "..\organizingsystem\OrganizingCtrl.h"
#include "..\player.h"
#include "..\..\appworld\IncrementLog\IncrementLog.h"
#include "MsgOutTimeDef.h"
#include "..\DBentityManager.h"
#include "..\dbplayer.h"
#include "../DBLoginPlayer.h"
#include "../GlobalRgnManager.h"
#include "../Player.h"
#include "../DBDupRgn.h"
#include "../public/AutoSupplyData.h"
#include "../public/PlayerSetupOnServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



void OnDBPassMessage(CMessage* pMsg)
{
	size_t nSavePlayerCnt = 0, 
		nCreationCnt = 0,
		nRestoreCnt = 0,
		nDeletionCnt = 0;
	long nTransactionCounter = 0;
	char pszLogingInfo[512]="";
	switch(pMsg->GetType())
	{
	case MSG_DB2W_PASS_GM_BANPLAYER:
		{			
			char szCdkey[128];
			pMsg->GetStr(szCdkey, 128);
			long time = pMsg->GetLong();
			CMessage msg(MSG_W2L_GM_BANPLAYER);
			msg.Add(szCdkey);
			msg.Add(time);
			msg.SendToSocket(GetGame()->GetLSSocketID());
			AddLogText("DB2W:ban player %s OK!", szCdkey);
		}
		break;
	case MSG_DB2W_PASS_LOG_PLAYERBASELIST_DELTIMER:				// WS����LS������
		{
			// ɾ����ʱ��
			long msgTimerID = pMsg->GetLong();
			//GetGame()->DelMsgTimer(msgTimerID);
		}
		break;
	case MSG_DB2W_PASS_LOG_PLAYERBASELIST:				// WS����LS������
		{
			char strCDKey[20];
			BYTE nRoleCnt;

			CGUID playerid;
			
			BYTE bRet=0;
			bRet=pMsg->GetByte();

			pMsg->GetStr(strCDKey,20);

			nRoleCnt=BYTE(pMsg->GetShort());
			
			char szGUID[128]={0};

			CMessage logMsg(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);
			
			// ������֤��
			long lTestSignCode = 0;
			GetGame()->GenerateSignCodeByTime(lTestSignCode);
			// �����֤����
			logMsg.Add((long)lTestSignCode);

			logMsg.Add((BYTE)bRet);
			logMsg.Add(strCDKey);
			logMsg.Add((short)nRoleCnt);

			CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(strCDKey);
			if(!tLoginInfo)
			{
				_snprintf(pszLogingInfo,512,"Err MSG_DB2W_PASS_LOG_PLAYERBASELIST: LoginInfo[%s] Is Null!", strCDKey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);

				CMessage log2Msg(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);
				// �����֤����
				log2Msg.Add((long)0xFFFFFFFF);
				log2Msg.Add((BYTE)0);
				log2Msg.Add(strCDKey);
				log2Msg.Add((short)0);
				logMsg.Add((short)0);
				// ���
				log2Msg.Add((short)0);//��ɫ����
				log2Msg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}

			tLoginInfo->lSignCode = lTestSignCode;

			if(0 < nRoleCnt)
			{
				for(int i=0;i<nRoleCnt;i++)
				{
					CGUID lplayerID;
					pMsg->GetGUID(lplayerID);
					lplayerID.tostring(szGUID);
					CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(lplayerID);
					if(lPlayer)
					{
						lPlayer->SetAccount(strCDKey, 20);
						tLoginInfo->PlayerState = PLAYER_STATE_LOADED_BASE_DATA;
						tLoginInfo->lLastLoginTime = timeGetTime();

						// ΪLoginPlayer�����������mapӳ��
						GetGame()->GetDBEntityManager()->AddLoginPlayerByCdkey(lPlayer);
				
						// ���
						logMsg.Add((short)nRoleCnt);//��ɫ����
						logMsg.Add(lplayerID);
						char* szName = (char*)lPlayer->GetStringAttr(string("Name"), 0);
						if(szName)
							logMsg.Add(szName);//�õ�����
						else
							logMsg.Add("");
						logMsg.Add((WORD)lPlayer->GetLongAttr(string("Levels"), 0));//�õ��ȼ�

						logMsg.Add((short)lPlayer->GetLongAttr(string("Occupation"), 0));//�õ�ְҵ���				
						logMsg.Add((short)lPlayer->GetLongAttr(string("Sex"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("Country"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("HEAD"), 0));			
						logMsg.Add((short)lPlayer->GetLongAttr(string("HAIR"), 0));			
						logMsg.Add((long)lPlayer->GetLongAttr(string( "FACE"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("NECKLACE"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("WING"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("BODY"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("BACK"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("GLOVE"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("BOOT"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("HEADGEAR"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("FROCK"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("LRING"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("RRING"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("MEDAL1"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("MEDAL2"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("MEDAL3"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("FAIRY"), 0));	
						logMsg.Add((short)lPlayer->GetLongAttr(string("WEAPON"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("WEAPON2"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("HeadLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("NecklaceLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("WingLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("BodyLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("BackLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("GloveLevel"), 0));	
						logMsg.Add((short)lPlayer->GetLongAttr(string("BootLevel"), 0));	
						logMsg.Add((short)lPlayer->GetLongAttr(string("HeadgearLevel"), 0));	
						logMsg.Add((short)lPlayer->GetLongAttr(string("FrockLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("LRingLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("RRingLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("Medal1Level"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("Medal2Level"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("Medal3Level"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("FairyLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("WeaponLevel"), 0));
						logMsg.Add((short)lPlayer->GetLongAttr(string("Weapon2Level"), 0));
						logMsg.Add((long)lPlayer->GetDelTime());
						logMsg.Add((long)lPlayer->GetLongAttr(string("Region"), 0));
					}
					_snprintf(pszLogingInfo,512,"MSG_DB2W_PASS_LOG_PLAYERBASELIST!MSGID[1175297],���ID[%s],Cdkey[%s],ret[%d]!", szGUID, strCDKey, bRet);
					PutStringToFile("Login_WS_Info",pszLogingInfo);
				}
				logMsg.SendToSocket(GetGame()->GetLSSocketID());
				return;
			}
			else
			{
				logMsg.Add((short)0);
				// ���
				logMsg.Add((short)0);//��ɫ����
				logMsg.SendToSocket(GetGame()->GetLSSocketID());
			}
		}
		break;
	case MSG_DB2W_PASS_LOG_QUEST_PLAYERDATA:						// WS����LS������
		{
			long msgTimerID = pMsg->GetLong();

			char flag = pMsg->GetChar();

			// ɾ����ʱ��
			//GetGame()->DelMsgTimer(msgTimerID);

			if(flag == LOGIN_QUEST_PLAYERDATA_FAILED)
			{
				char szCdkey[32];
				CGUID guid;

				pMsg->GetGUID(guid);//��ȡPLAYERID
				pMsg->GetStr(szCdkey,32);//��ȡCDKEY

				CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
				CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(szCdkey);
				if(!tLoginInfo)
				{
					_snprintf(pszLogingInfo,512, "Err MSG_DB2W_PASS_LOG_QUEST_PLAYERDATA: LoginInfo[%s] Is Null!", szCdkey);
					PutStringToFile("Login_WS_Info",pszLogingInfo);

					CMessage msg2(MSG_W2L_LOG_ANSWER_PLAYERDATA);
					// �����֤����
					msg2.Add((long)0xFFFFFFFF);
					msg2.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
					msg2.Add(szCdkey);
					msg2.Send();
					return;
				}
				if(tLoginInfo)
				{
					// ������֤��
					GetGame()->GenerateSignCodeByTime(tLoginInfo->lSignCode);
					// �����֤����
					msg.Add((long)tLoginInfo->lSignCode);
				}
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
				msg.Add(szCdkey);
				msg.Send();
				char szGuid[128]="";
				guid.tostring(szGuid);
				_snprintf(pszLogingInfo,512,"W2DB�����������ʧ��,ID[%s],Cdkey[%s]!", szGuid, szCdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);

				CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(guid);
				if(lPlayer)
				{
					tLoginInfo->PlayerState = PLAYER_STATE_OFFLINE;
					tLoginInfo->lLastLoginTime = timeGetTime();
				}
				return;
			}
			else if( flag == LOGIN_QUEST_PLAYERDATA_SUCCEED )
			{
				char szCdkey[32];
				CGUID guid;
				LONG64 time;
				pMsg->GetGUID(guid);//��ȡPLAYERID
				pMsg->GetStr(szCdkey,32);//��ȡCDKEY	
				char szGUID[128];
				guid.tostring(szGUID);

				//�Ʒ���
				DWORD dwClientIP=pMsg->GetDWord();
				time = pMsg->GetLONG64();
			
				CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(guid);
				if(NULL == lPlayer)
				{
					_snprintf(pszLogingInfo,512,"--[W2DB�����������ʧ��,��¼��������Ƿ���¼!ID[%s],CdKey[%s]!", szGUID, szCdkey);
					PutStringToFile("Login_WS_Info",pszLogingInfo);
					return;
				}

				//if(lPlayer->GetPlayerState() != PLAYER_STATE_RESTORED)
				{
					// �ȼ���Ƿ��ɫ�Ѿ�������ɾ�����
					time_t tDel = GetGame()->GetDeletionPlayerTime(guid);
					if(!tDel) 
					{
						tDel = (time_t)time;
					}

					// ɾ�����ڲ�Ϊ0������ڣ����Բ��ܵ�½
					if(tDel>0)
					{
						_snprintf(pszLogingInfo,512,"--[W2DB�����������ʧ��,������ϸ��Ϣʧ��(ɾ�����ڲ�Ϊ0)!ID[%s],CdKey[%s]!", szGUID, szCdkey);
						PutStringToFile("Login_WS_Info",pszLogingInfo);

						CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
						CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(szCdkey);
						if(!tLoginInfo)
						{
							_snprintf(pszLogingInfo,512,"Err MSG_DB2W_PASS_LOG_QUEST_PLAYERDATA_2: LoginInfo[%s] Is Null!", szCdkey);
							PutStringToFile("Login_WS_Info",pszLogingInfo);

							CMessage msg2(MSG_W2L_LOG_ANSWER_PLAYERDATA);
							// �����֤����
							msg2.Add((long)0xFFFFFFFF);
							msg2.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
							msg2.Add(szCdkey);
							msg2.SendToSocket(GetGame()->GetLSSocketID());
							return;
						}
						if(tLoginInfo)
						{
							// ������֤��
							GetGame()->GenerateSignCodeByTime(tLoginInfo->lSignCode);
							// �����֤����
							msg.Add((long)tLoginInfo->lSignCode);
						}
						msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
						msg.Add(szCdkey);
						msg.SendToSocket(GetGame()->GetLSSocketID());	
						tLoginInfo->PlayerState = PLAYER_STATE_LOADED_BASE_DATA;
						tLoginInfo->lLastLoginTime = timeGetTime();
						return;
					}
				}
				
				// ���CPlayer
				CPlayer* pPlayer = GetGame()->GetMapPlayer(guid);
				if(NULL == pPlayer)
				{
					pPlayer = new CPlayer;
					pPlayer->SetExID(guid);
					// ��ʼ��Account����
					pPlayer->SetAccount(szCdkey);
					GetGame()->DeleteMapPlayer(guid);
					GetGame()->AppendMapPlayer(pPlayer);
				}
				
				// ���CDBPlayer
				CDBPlayer* dbPlayer = GetGame()->GetDBEntityManager()->FindPlayer(guid);
				if(NULL == dbPlayer)
				{
					_snprintf(pszLogingInfo,512,"DBS�ڻ�ӦMSG_DB2W_PASS_LOG_QUEST_PLAYERDATA��Ϣǰδ����DBPlayer����!");
					PutStringToFile("Login_WS_Info",pszLogingInfo);

					CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(szCdkey);
					if(!tLoginInfo)
					{
						_snprintf(pszLogingInfo,512,"DBS�ڻ�ӦMSG_DB2W_PASS_LOG_QUEST_PLAYERDATA��Ϣǰδ����DBPlayer����! LoginInfo[%s] Is Null!", szCdkey);
						PutStringToFile("Login_WS_Info",pszLogingInfo);

						CMessage msg2(MSG_W2L_LOG_ANSWER_PLAYERDATA);
						// �����֤����
						msg2.Add((long)0xFFFFFFFF);
						msg2.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
						msg2.Add(szCdkey);
						msg2.SendToSocket(GetGame()->GetLSSocketID());
						return;
					}
					if(tLoginInfo)				
					{
						tLoginInfo->PlayerState = PLAYER_STATE_OFFLINE;
						tLoginInfo->lLastLoginTime = timeGetTime();
					}
					return;
				}
				GetGame()->GetDBEntityManager()->CDBPlayerToCPlayer(dbPlayer, pPlayer);
				GetGame()->GetDBEntityManager()->CDBLoginPlayerToCPlayer(lPlayer, pPlayer);
				CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(szCdkey);
				if(!tLoginInfo)
				{
					_snprintf(pszLogingInfo,512,"PLAYER_STATE_LOADED_CHAR_DATA! LoginInfo[%s] Is Null!", szCdkey);
					PutStringToFile("Login_WS_Info",pszLogingInfo);

					CMessage msg2(MSG_W2L_LOG_ANSWER_PLAYERDATA);
					// �����֤����
					msg2.Add((long)0xFFFFFFFF);
					msg2.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
					msg2.Add(szCdkey);
					msg2.SendToSocket(GetGame()->GetLSSocketID());
					return;
				}
				if(tLoginInfo)	
				{
					tLoginInfo->PlayerState = PLAYER_STATE_LOADED_CHAR_DATA;
					// ���ϱ��ε�¼ʱ��
					tLoginInfo->lLastLoginTime = timeGetTime();
				}
				// ����LS��Ϣ
				GetGame()->InitPlayerInfo(pPlayer, szCdkey);

				_snprintf(pszLogingInfo,512,"MSG_DB2W_PASS_LOG_QUEST_PLAYERDATA!MSGID[1175299],���ID[%s],Cdkey[%s]!", szGUID, szCdkey);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
			}
		}
		break;
	case MSG_DB2W_PASS_LOG_CREATEROLE:						// WS���󴴽�һ����ɫ
		{
			long msgTimerID = pMsg->GetLong();

			char flag = pMsg->GetChar(); // ���������־

			if(flag == LOGIN_CREATEROLE_SUCCEED)
			{
				CGUID tGuid;
				char szName[50],szCdkey[20];
				pMsg->GetStr(szName,50);
				char nOccupation = pMsg->GetChar();
				char nSex = pMsg->GetChar();
				BYTE nHead = pMsg->GetByte();
				BYTE nFace = pMsg->GetByte();
				BYTE btCountry = pMsg->GetByte();
				pMsg->GetStr(szCdkey,20);
				pMsg->GetGUID(tGuid);

				char szGUID[128];
				tGuid.tostring(szGUID);
				//  �ӳ���ɫ
				CPlayer *pPlayer = GetGame()->GetMapPlayer(tGuid);
				if(NULL == pPlayer)
				{
					char szGUID[128];
					tGuid.tostring(szGUID);
					_snprintf(pszLogingInfo,512,"������ɫ������Ϣ:δ�ҵ�Ԥ�ȴ�����CPlayer����!:ID[%s]!", szGUID);
					PutStringToFile("Login_WS_Info",pszLogingInfo);
				}
				
				if(pPlayer)
				{
					pPlayer->SetName(szName);
					pPlayer->LoadDefaultProperty(nOccupation, nSex, btCountry);
					pPlayer->SetHeadPic (nHead);
					pPlayer->SetFacePic (nFace);
					pPlayer->SetAccount(szCdkey);
					pPlayer->SetName(szName);
					pPlayer->SetOccupation(nOccupation);
					pPlayer->SetGraphicsID(nSex+1);
					pPlayer->SetPKOnOff(127);
					pPlayer->SetOccuLevel(1);
					
					// �ӳ�ʼװ��������
					GetGame()->AddOrginGoodsToPlayer(pPlayer);
					GetGame()->InitSkillToPlayer(pPlayer);

					//���ʹ����ɹ�����Ϣ
					short nLevel=pPlayer->GetLevel ();//m_Property.lLevel;
					CGUID lPlayerid=pPlayer->GetExID();

					DWORD dwBody=0,dwGlov=0,dwBoot=0,dwHelm=0,dwWeapon=0,dwBack=0,
						dwHeadgear=0,dwFrock=0,dwWing=0,dwManteau=0,dwFairy=0;
					BYTE nBodyLevel=0,nGlovLevel=0,nBootLevel=0,nHelmLevel=0,nWeaponLevel=0,nBackLevel=0,
						nHeadgearLevel=0,nFrockLevel=0,nWingLevel=0,nManteauLevel=0,nFairyLevel=0;

					GetGame()->GetPlayerEquipID(pPlayer, dwHelm, dwBody, dwGlov, dwBoot, dwWeapon, dwBack,
						dwHeadgear,dwFrock,dwWing,dwManteau,dwFairy,
						nHelmLevel, nBodyLevel, nGlovLevel, nBootLevel, nWeaponLevel, nBackLevel,
						nHeadgearLevel,nFrockLevel,nWingLevel,nManteauLevel,nFairyLevel);

					CMessage msg(MSG_W2L_LOG_CREATEROLE);
					msg.Add((char)LOGIN_CREATEROLE_SUCCEED);
					msg.Add(szCdkey);
					msg.Add(lPlayerid);
					msg.Add(szName);
					msg.Add(nLevel);
					msg.Add(nOccupation);
					msg.Add(nSex);
					msg.Add(btCountry);
					msg.Add(nHead);
					msg.Add(nHead);
					msg.Add(nFace);

					msg.Add(dwHelm);
					msg.Add(dwBody);
					msg.Add(dwGlov);
					msg.Add(dwBoot);
					msg.Add(dwWeapon);
					msg.Add(dwBack);

					// -- ��װ�� 
					msg.Add(dwHeadgear);
					msg.Add(dwFrock);
					msg.Add(dwWing);
					msg.Add(dwManteau);
					msg.Add(dwFairy);
					// --

					msg.Add(nHelmLevel);
					msg.Add(nBodyLevel);
					msg.Add(nGlovLevel);
					msg.Add(nBootLevel);
					msg.Add(nWeaponLevel);
					msg.Add(nBackLevel);

					// -- ��װ�� 
					msg.Add(nHeadgearLevel);
					msg.Add(nFrockLevel);
					msg.Add(nWingLevel);
					msg.Add(nManteauLevel);
					msg.Add(nFairyLevel);
					
					 // ���ʹ�����ϢMSG_W2DB_SAVE_PLAYER
					CDBPlayer* dbPlayer = NULL;
					CDBEntityManager::DBPlayerMapItr itr = GetGame()->GetDBEntityManager()->GetDBPlayerMap().find(pPlayer->GetExID());
					if(itr != GetGame()->GetDBEntityManager()->GetDBPlayerMap().end())
					{
						dbPlayer = itr->second;
					}
					else
					{
						char szGUID[128];
						tGuid.tostring(szGUID);
						_snprintf(pszLogingInfo,512,"������ɫ������Ϣ:δ�ҵ�Ԥ�ȴ�����DBPlayer����!:ID[%s]!", szGUID);
						PutStringToFile("Login_WS_Info",pszLogingInfo);
					}
			
					if(dbPlayer)
					{
						string tableName = "baseproperty";
						dbPlayer->SetStringAttr(string("Name"), tableName, 0, szName);
						dbPlayer->SetLongAttr(string("HeadPic"), tableName, 0, nHead);
						dbPlayer->SetLongAttr(string("FacePic"), tableName, 0, nFace);
						dbPlayer->SetAccount(szCdkey, sizeof(szCdkey));
						dbPlayer->SetLongAttr(string("Occupation"), tableName, 0, nOccupation);
						GetGame()->GetDBEntityManager()->CPlayerToCDBPlayer(pPlayer, dbPlayer);
						// ����AutoSupplyData����
						AutoSupplyData tsupplyData;
						vector<BYTE> psDataBA;
						tsupplyData.AddToByteArray(psDataBA);
						dbPlayer->SetBufAttr(string("AutoSupplyData"), string("CSL_Player_Sprite_System"), 0, &psDataBA[0], psDataBA.size());

						// ����PlayerSetupOnServer����
						PlayerSetupOnServer tSetupOnServer;
						vector<BYTE> psetupServerBA;
						tSetupOnServer.AddToByteArray(psetupServerBA);
						dbPlayer->SetBufAttr(string("PlayerSetup"), string("CSL_Player_Sprite_System"), 0, &psetupServerBA[0], psetupServerBA.size());
						GetGame()->SendSavePlayerMsg(dbPlayer);	
						GetGame()->SendSaveSetup();
					}

					// ����LoginPlayer
					CDBLoginPlayer* dblPlayer = NULL;
					CDBEntityManager::DBLoginPlayerMapItr litr = GetGame()->GetDBEntityManager()->GetDBLoginPlayerMap().find(pPlayer->GetExID());
					if(litr != GetGame()->GetDBEntityManager()->GetDBLoginPlayerMap().end())
					{
						dblPlayer = litr->second;
					}
					else
					{
						char szGUID[128];
						tGuid.tostring(szGUID);
						_snprintf(pszLogingInfo,512,"������ɫ������Ϣ:δ�ҵ�Ԥ�ȴ�����DBLoginPlayer����!:ID[%s]!", szGUID);
						PutStringToFile("Login_WS_Info",pszLogingInfo);
					}

					if(dblPlayer)
					{
						CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(szCdkey);
						if(!tLoginInfo)
						{
							_snprintf(pszLogingInfo,512,"MSG_DB2W_PASS_LOG_CREATEROLE! LoginInfo[%s] Is Null!", szCdkey);
							PutStringToFile("Login_WS_Info",pszLogingInfo);

							CMessage msgx(MSG_W2L_LOG_CREATEROLE);
							msgx.Add((char)LOGIN_CREATEROLE_FAILED);
							msgx.Add(szCdkey);
							msgx.SendToSocket(GetGame()->GetLSSocketID());
							return;
						}
						if(tLoginInfo)	
						{
							tLoginInfo->PlayerState = PLAYER_STATE_LOADED_BASE_DATA;
							// ���ϱ��ε�¼ʱ��
							tLoginInfo->lLastLoginTime = timeGetTime();
						}

						dblPlayer->SetDelTime(-1);
						string logTableName = "CSL_PLAYER_BASE";
						dblPlayer->SetStringAttr(string("Name"), logTableName, 0, szName);//�õ�����
						dblPlayer->SetLongAttr(string("Levels"), logTableName, 0,pPlayer->GetLevel());//�õ��ȼ�
						dblPlayer->SetLongAttr(string("Occupation"), logTableName, 0, nOccupation);//�õ�ְҵ���				
						dblPlayer->SetLongAttr(string("Sex"), logTableName, 0, pPlayer->GetSex());
						dblPlayer->SetLongAttr(string("Country"), logTableName, 0, pPlayer->GetCountry());
						dblPlayer->SetLongAttr(string("Region"), logTableName, 0, pPlayer->GetRegionID());
						dblPlayer->SetLongAttr(string("HEAD"), logTableName, 0, nHead);	
						dblPlayer->SetLongAttr(string("HAIR"), logTableName, 0, nHead);	
						dblPlayer->SetLongAttr(string( "FACE"), logTableName, 0, nFace);	
						dblPlayer->SetLongAttr(string("BODY"), logTableName, 0, dwBody);	
						dblPlayer->SetLongAttr(string("BACK"), logTableName, 0, dwBack);	
						dblPlayer->SetLongAttr(string("GLOVE"), logTableName, 0, dwGlov);	
						dblPlayer->SetLongAttr(string("BOOT"), logTableName, 0, dwBoot);	
						dblPlayer->SetLongAttr(string("HEADGEAR"), logTableName, 0, dwHeadgear);	
						dblPlayer->SetLongAttr(string("FROCK"), logTableName, 0, dwFrock);	
						dblPlayer->SetLongAttr(string("FAIRY"), logTableName, 0, dwFairy);	
						dblPlayer->SetLongAttr(string("HeadLevel"), logTableName, 0, nHelmLevel);	
						dblPlayer->SetLongAttr(string("BodyLevel"), logTableName, 0, nBodyLevel);	
						dblPlayer->SetLongAttr(string("BackLevel"), logTableName, 0, nBackLevel);	
						dblPlayer->SetLongAttr(string("GloveLevel"), logTableName, 0, nGlovLevel);	
						dblPlayer->SetLongAttr(string("BootLevel"), logTableName, 0, nBootLevel);	
						dblPlayer->SetLongAttr(string("HeadgearLevel"), logTableName, 0, nHeadgearLevel);	
						dblPlayer->SetLongAttr(string("FrockLevel"), logTableName, 0, nFrockLevel);	
						dblPlayer->SetLongAttr(string("FairyLevel"), logTableName, 0, nFairyLevel);	
						GetGame()->SendSaveLoginPlayer(tGuid);
						
					}

					// ���ʹ����ɹ���Ϣ��LS
					msg.SendToSocket(GetGame()->GetLSSocketID());
				}
				_snprintf(pszLogingInfo,512,"������ɫ Name:[%s], ID:[%s] �ɹ�!", szCdkey, szGUID);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
			}
			else 
			{
				char szCDKEY[128];
				pMsg->GetStr(szCDKEY, 128);
				CGUID tGuid;
				pMsg->GetGUID(tGuid);
				
				CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(szCDKEY);
				if(!tLoginInfo)
				{
					_snprintf(pszLogingInfo,512,"MSG_DB2W_PASS_LOG_CREATEROLE_2! LoginInfo[%s] Is Null!", szCDKEY);
					PutStringToFile("Login_WS_Info",pszLogingInfo);

					CMessage msgx(MSG_W2L_LOG_CREATEROLE);
					msgx.Add((char)LOGIN_CREATEROLE_FAILED);
					msgx.Add(szCDKEY);
					msgx.SendToSocket(GetGame()->GetLSSocketID());
					return;
				}
				if(tLoginInfo)	
				{
					tLoginInfo->PlayerState = PLAYER_STATE_OFFLINE;
					tLoginInfo->lLastLoginTime = timeGetTime();
				}

				// ɾ��CDBLoginPlayer����
				GetGame()->GetDBEntityManager()->DelOneDBLoginPlayer(tGuid);
				// ɾ��CDBPlayer����
				GetGame()->GetDBEntityManager()->DelOneDBPlayer(tGuid);
				// ɾ��CPlayer����
				GetGame()->DeleteMapPlayer(tGuid);

				tLoginInfo->setLoginGuid.erase(tGuid);

				CMessage msg(MSG_W2L_LOG_CREATEROLE);
				switch(flag)
				{
				case (char)LOGIN_CREATEROLE_NAMEEXIST:
					msg.Add((char)LOGIN_CREATEROLE_NAMEEXIST);
					break;
				case (char)LOGIN_CREATEROLE_DBERROR:
					msg.Add((char)LOGIN_CREATEROLE_DBERROR);
					break;
				case (char)LOGIN_CREATEROLE_FULL:
					msg.Add((char)LOGIN_CREATEROLE_FULL);
					break;
				case (char)LOGIN_CREATEROLE_NAMEINVALID:
					msg.Add((char)LOGIN_CREATEROLE_NAMEINVALID);
					break;
				}
				msg.Add(szCDKEY);
				msg.SendToSocket(GetGame()->GetLSSocketID());

				char szGUID[128];
				tGuid.tostring(szGUID);
				_snprintf(pszLogingInfo,512,"������ɫ Name:[%s], ID:[%s] ʧ��, ʧ�ܱ�־:[%d]!", szCDKEY, szGUID, flag);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
			}
	
			// ɾ����ʱ��
			//GetGame()->DelMsgTimer(msgTimerID);
		}
		break;
	case MSG_DB2W_PASS_LOG_DELETEROLE:							    // WS���󴴽�һ����ɫ
		{
			long msgTimerID = pMsg->GetLong();

			char Flag = pMsg->GetChar(); // �Ƿ�ɹ���־λ
			CGUID nID;
			pMsg->GetGUID(nID); // Player ID
			char CDKey[64];
			pMsg->GetStr(CDKey, 64);
			long delTime = pMsg->GetLong();
			char name[128]; // player name
			pMsg->GetStr(name, 128);
			
			if(Flag == LOGIN_DELETEROLE_SUCCEED)
			{
				// �ڻָ��б���ɾ��
				//GetGame()->DeleteRestorePlayer(nID);
				// �ѽ�ɫid��ɾ������׷�ӵ�ɾ���б�
				//GetGame()->AppendDeletionPlayer(nID, delTime);
				CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(nID);
				if(lPlayer)
				{
					lPlayer->SetDelTime(delTime);
					CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(CDKey);
					if(!tLoginInfo)
					{
						_snprintf(pszLogingInfo,512,"DELETEROLE! LoginInfo[%s] Is Null!", CDKey);
						PutStringToFile("Login_WS_Info",pszLogingInfo);

						CMessage msg(MSG_W2L_LOG_DELETEROLE);
						msg.Add((char)LOGIN_DELETEROLE_FAILED);
						msg.Add(nID);
						msg.Add(CDKey);
						msg.Send();//send to loginserver
						return;
					}
					if(tLoginInfo)	
					{
						tLoginInfo->PlayerState = PLAYER_STATE_LOADED_BASE_DATA;
						tLoginInfo->lLastLoginTime = timeGetTime();
					}

					tm *now;
					time_t tDate;
					time(&tDate);
					now = localtime(&tDate);
					long delTime[6] = {0};
					delTime[0] = now->tm_year+1900;
					delTime[1] = now->tm_mon+1;
					delTime[2] = now->tm_mday;
					lPlayer->SetTimeAttr(string("DelDate"), string("CSL_PLAYER_BASE"), 0, delTime, sizeof(long)*6);
				}
		
				// ��Ϊ�Ǹ�ɾ���ģ�ֱ�ӷ����������ɾ���������ɡ�
				char nRemainDays = char(CGlobeSetup::GetSetup()->dwDelDays);
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)LOGIN_DELETEROLE_SUCCEED);
				msg.Add(nID);
				msg.Add(CDKey);
				msg.Add(nRemainDays);
				msg.Send();//send to loginserver
			}
			else
			{
				CMessage msg(MSG_W2L_LOG_DELETEROLE);
				msg.Add((char)Flag);
				msg.Add(nID);
				msg.Add(CDKey);
				msg.Send();//send to loginserver
			}
		
			// ɾ����ʱ��
			//GetGame()->DelMsgTimer(msgTimerID);
		}
		break;
	case MSG_DB2W_PASS_LOG_RESTOREROLE:
		{
			char flag;
			CGUID guid;
			char cdkey[128];
			flag = pMsg->GetChar();
			pMsg->GetGUID(guid);
			pMsg->GetStr(cdkey, 128);
			CMessage msg(MSG_W2L_LOG_RESTOREROLE);
			if(flag == 0)
			{
				msg.Add((char)LOGIN_RESTOREROLE_FAILED);
			}
			else 
			{
				CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(guid);
				if(lPlayer)
				{
					CDBEntityManager::tagLoginInfo* tLoginInfo = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(cdkey);
					if(!tLoginInfo)
					{
						_snprintf(pszLogingInfo,512,"MSG_DB2W_PASS_LOG_RESTOREROLE! LoginInfo[%s] Is Null!", cdkey);
						PutStringToFile("Login_WS_Info",pszLogingInfo);

						msg.Add((char)LOGIN_RESTOREROLE_FAILED);
						msg.Add(guid);
						msg.Add(cdkey);
						msg.Send();
						return;
					}
					if(tLoginInfo)	
					{
						tLoginInfo->PlayerState = PLAYER_STATE_LOADED_BASE_DATA;
						tLoginInfo->lLastLoginTime = timeGetTime();
					}
					lPlayer->SetDelTime((time_t)-1);
				}
				
				msg.Add((char)LOGIN_RESTOREROLE_SUCCEED);

				long delTime[6] = {0};
				lPlayer->SetTimeAttr(string("DelDate"), string("CSL_PLAYER_BASE"), 0, delTime, sizeof(long)*6);
			}
			msg.Add(guid);
			msg.Add(cdkey);
			msg.Send();
		}
		break;
	case MSG_W2DB_PASS_CARD_VALIDATE_RESPONSE:					// ���ֿ�
		break;
	case MSG_DB2W_PSASS_OTHER_GETLOG_REQUEST:
		{
			long gsid = pMsg->GetLong(); // gsid
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid); // ��������GUID
			long IncLogNum = pMsg->GetLong();// ����

			//[070716 AHC]
			BYTE byType;
			DWORD dwMoney;
			char szDesc[256];
			char szAcc[64];
		
			if(IncLogNum)
			{
				// ��ո������־
				CIncrementLog::GetInstance()->DelAllPlayerIncLog(PlayerGuid);

				for(int i=0; i<IncLogNum; i++)
				{
					byType  = pMsg->GetByte();
					dwMoney = pMsg->GetDWord();
					pMsg->GetStr(szDesc, 256);
					pMsg->GetStr(szAcc, 64);
				
					SYSTEMTIME time;
					pMsg->GetEx(&time, sizeof(SYSTEMTIME));
					// ��ӵ���¼����
					CIncrementLog::GetInstance()->Add(PlayerGuid, &time, byType, dwMoney, szDesc);			
				}

				CMessage msg(MSG_W2S_OTHER_GETLOG_RESPONSE);
				msg.Add(PlayerGuid);
				vector<BYTE> vecLogData;
				if (CIncrementLog::GetInstance()->AddIncrementLogsToByteArray(vecLogData, PlayerGuid))
				{
					msg.Add((long)vecLogData.size());
					if(vecLogData.size() > 0)
						msg.AddEx(&vecLogData[0],vecLogData.size());
					msg.SendGSID(gsid);
				}
			}	
		}
		break;
	case MSG_DB2W_PASS_LOAD_ALL_FACTION_FINISH:
		{
			GetOrganiCtrl()->OnInitFactionEndMsg(pMsg);
		}
		break;
	case MSG_DB2W_PASS_DelFaction_Finish:
		{
			GetOrganiCtrl()->OnDelFactionFinishMsg(pMsg);
		}
		break;
	}
}