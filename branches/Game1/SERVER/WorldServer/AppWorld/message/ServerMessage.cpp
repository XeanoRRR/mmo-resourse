#include "stdafx.h"
#include "..\nets\networld\message.h"
#include "..\nets\Servers.h"
#include "..\worldregion.h"
#include "..\player.h"
#include "../script/VariableList.h"

#include "setup\playerlist.h"
#include "..\Goods\cgoodsfactory.h"
#include "..\goods\csuitrulefactory.h"
#include "..\goods\IncrementShopManager.h"
#include "..\goods\enchase\CKitCardRuleFactory.h"
#include "../business/CBusinessManager.h"
#include "setup\monsterlist.h"
#include "setup\GateList.h"
#include "setup\CountTimerList.h"
#include "setup\tradelist.h"
#include "setup\incrementShopList.h"
#include "setup\ContributeSetup.h"
#include "setup\hitlevelsetup.h"
#include "..\NewSkill\NewSkillFactory.h"
#include "setup\globesetup.h"
#include "setup/logsystem.h"
#include "setup/gmlist.h"
#include "setup/emotion.h"
#include "setup/QuestSystem.h"
#include "setup\MailList.h"
#include "setup\ScriptAILoader.h"
#include "../public/DeputyOccu.h"
#include "setup/regionsetup.h"
#include "..\public\dupliregionsetup.h"
#include "..\public\OccupSkillList.h"
#include "..\public\StudySkillCondition.h"
#include "../public/strtype.h"
#include "..\public\StrUnifyInput.h"
#include "..\public\WordsFilter.h"
#include "..\public\ReliveGoodsList.h"
#include "..\public\Cosmetology.h"
#include "..\..\appworld\session\csessionfactory.h"
#include "..\..\appworld\session\csession.h"
#include "..\..\appworld\Organizingsystem\OrganizingCtrl.h"
#include "..\..\appworld\Country\CountryParam.h"
#include "..\..\appworld\Country\CountryHandler.h"
#include <mmsystem.h>
#include "../GlobalRgnManager.h"
#include "../DBentity/EntityGroup.h"
#include "../DBentity/entityManager.h"
#include "../public/entityproperty.h"
#include "../PlayerDepot.h"
#include <vector>
#include "../../AppWorld/Linkman/LinkmanSystem.h"
#include "../public/StrUnifyInput.h"
#include "..\..\appworld\PlayerMan\PlayerMan.h"
#include "..\..\appworld\PetCtrl.h"
#include "..\..\AppWorld\DBEntity\Entity.h"
#include "..\..\appworld\anticheat\QuestionManager.h"
#include "..\public\CreditSystem.h"
#include "../FBusiness/Configer.h"
#include "..\public\QuestIndexXml.h"
#include "..\public\LotteryXml.h"



extern HANDLE g_hSavingThread;

extern const unsigned RELOAD_PLAYER_MAN;	// Fox@20081120 ��ҹ���
extern const unsigned RELOAD_PET_LIST;		// Fox@20081119 ����
extern const unsigned RELOAD_ANTI_CHEAT;

// ��Ӧ������Ϣ
void OnServerMessage(CMessage* pMsg)
{
	long lSocketID = pMsg->GetSocketID();
	switch(pMsg->GetType())
	{
	case MSG_DB2W_SERVER_SENDSETUP:
		{
			// �����ԭ������
			std::map<string, tagEntityBuildInfo*>::iterator deItr = GetGame()->GetEntityManager()->GetObjAttrDef().begin();
			for(; deItr != GetGame()->GetEntityManager()->GetObjAttrDef().end(); deItr++)
			{
				OBJ_RELEASE(tagEntityBuildInfo,deItr->second);
			}
			GetGame()->GetEntityManager()->GetObjAttrDef().clear();

			long objAttrDefNum = pMsg->GetLong();
			long bufSize = pMsg->GetLong();
			BYTE* buf = (BYTE*)M_ALLOC(bufSize);
			pMsg->GetEx(buf, bufSize);
			long pos = 0;
			for(int i=0; i<objAttrDefNum; i++)
			{
				tagEntityBuildInfo* tBuildInfo = OBJ_CREATE(tagEntityBuildInfo);
				tBuildInfo->DecodeFromByteArray(buf, pos, bufSize);
				GetGame()->GetEntityManager()->GetObjAttrDef()[tBuildInfo->strComFlag] = tBuildInfo;
			}
			M_FREE(buf,bufSize);

			GetGame()->SetDbIsReady(true);
	
			// ��ȡ��ʼ������
			//if(!GetGame()->GetInitCountryLoadFlag())
			//	GetGame()->GetEntityManager()->CreateLoadCountrySession();
			if(!GetGame()->GetInitRegionLoadFlag())
				GetGame()->GetEntityManager()->CreateLoadRegionSession();
			if(!GetGame()->GetInitPersonalHouseLoadFlag())
				GetGame()->GetEntityManager()->CreateLoadPhRgnSession();
			if(!GetGame()->GetInitVarLoadFlag())
				GetGame()->GetEntityManager()->CreateLoadGlobalVarSession();
			if(!GetGame()->GetInitFactionLoadFlag())
				GetGame()->GetEntityManager()->CreateLoadFactionGroupSession();
			if(!GetGame()->GetInitUnionLoadFlag())
				GetGame()->GetEntityManager()->CreateLoadUnionGroupSession();

			// ����Ƿ�����GS��������ɲ���DBS׼�����, �ɹ�����saveplayer������������
			bool isSaveFlag = false;
			map<DWORD, CGame::tagGameServer>::iterator gsItr = GetGame()->GetGameServerList()->begin();
			for(; gsItr != GetGame()->GetGameServerList()->end(); gsItr++)
			{
				if(gsItr->second.bConnected)
					isSaveFlag = true;
				else
				{
					isSaveFlag = false;
					break;
				}
			}
			
			// ͬ��DBS��Ϸ����
			GetGame()->DoSaveData(true);
			
			if(isSaveFlag && GetGame()->GetDbIsReady())
			{	
				//DoSaveData();
				GetGame()->ProcessDelaySavePlayer();
				GetGame()->SendCdkeyToLoginServer(TRUE);
			}
			else
				GetGame()->SendCdkeyToLoginServer(FALSE);

			// �����ȡ��ҵ���ݵ�����(֮ǰδ��DBװ��)
			if( GetInst( Business::CBusinessManager ).IsEnable() && 
				!GetInst( Business::CBusinessManager ).IsDBLoaded() )
			{
				GetGame()->GetEntityManager()->CreateLoadBusinessSession();
			}

			GetGame()->GetEntityManager()->CreateLoadLimitGoodsRecordSession();
		}
		break;
	case MSG_DB2W_SERVER_QUEST_RECONNECTION:
		{
			long msgid = pMsg->GetLong();
			// ɾ����ʱ��
			GetGame()->DelMsgTimer(msgid);
			AddLogText(CStringReading::LoadString(IDS_WS_SERVERMESSAGE, STR_WS_SERVERMESSAGE_DBSRECONNETIONINFO));
		}
		break;
	case MSG_DB2W_SERVER_INIT_PLAYERDATABUFFER:
		{
			// ��ʼ��CLoginPlayer��CPlayer�Ļ�����

		}
		break;
	case MSG_S2W_SERVER_QUEST_RECONNECTION:
		
		break;
		// ����server��Ϣ��WorldServer
	case MSG_S2W_SERVER_SENDSERVERINFO:
		{
			static long lGsSleepTime = 0; // Minutes

			// ����gameserver��ʶ
			bool bReconnect = pMsg->GetChar()?true:false;
			long lMsgLen = 0;
			DWORD dwPort = pMsg->GetLong();
			char strGameServerIP[256];
			pMsg->GetStr(strGameServerIP,256);

			CGame::tagGameServer* pGameServer = GetGame()->GetGameServer(strGameServerIP, dwPort);
			if (!pGameServer)
			{
				AddLogText(CStringReading::LoadString(IDS_WS_SERVERMESSAGE, STR_WS_SERVERMESSAGE_GSRECONNSETUPDATA), strGameServerIP, dwPort);
				break;
			}

			char str[256];
			sprintf(str, "%s [%d]", strGameServerIP, dwPort);
			SendMessage(g_hGameServerListBox,LB_INSERTSTRING,-1,(LPARAM)str);

			if (bReconnect)
			{
				// ȡ�����ݿ�
				DBReadSet setReadDB;
				pMsg->GetDBReadSet(setReadDB);

				// ����GSȫ�ֱ���ֵ
				BYTE byVarFlag = setReadDB.GetByteFromByteArray();
				if(1 == byVarFlag)
					GetGame()->GetGeneralVariableList()->DecodeFromDataBlock(setReadDB);

				// ���������������
				long playerNum = setReadDB.GetLongFromByteArray();
				for(int i=0; i<playerNum; i++)
				{
					bool bFlag = setReadDB.GetByteFromByteArray()==0?false:true;
					if(bFlag)
					{
						if (pGameServer)
						{
							pGameServer->lReceivedPlayerData++;
							pGameServer->lPlayerNum++;
						}

						CGUID lPlayerID;
						setReadDB.GetBufferFromByteArray(lPlayerID);

						GetGame()->DeleteMapPlayer(lPlayerID);
						CPlayer* pPlayer = GetGame()->NewRawPlayer(ePlayerGsReconnectOne);
						if(pPlayer)
						{
							pPlayer->SetExID(lPlayerID);
							pPlayer->DecodeFromDataBlock(setReadDB);
							GetGame()->AppendMapPlayer(pPlayer);
							pPlayer->SetGsSocket(lSocketID);
							if(GetGame()->GetDbIsReady())
							{
								GetGame()->GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);
									
								Account* pAcc = NULL;
								if(pPlayer->GetAccount() != 0)
								{
									pAcc = MP_NEW Account;
									if (pAcc)
									{
										CGUID accGuid;
										CGUID::CreateGUID(accGuid);
										pAcc->SetGUID(accGuid);
										pAcc->SetName(pPlayer->GetAccount());
										if(!GetGame()->GetEntityManager()->AddAccount(pAcc))
										{
											MP_DELETE(pAcc);
										}
										else
										{
											pAcc->SetCurGamedPlayerGuid(pPlayer->GetExID());
											pAcc->SetPlayerState(PLAYER_STATE_ENTERED_GAME);
										}
									}
								}

								CEntityGroup* lPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[loginplayer]"), pPlayer->GetExID());
								if(pPlayer && pAcc)
								{
									pAcc->AddLoginPlayer(lPlayer);
									GetGame()->GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());
								}
							}

							GetOrganiCtrl()->OnPlayerEnterGame(pPlayer->GetExID());
							LinkmanSystem::GetInstance().OnPlayerOnline(pPlayer->GetExID());
						}
					}
				}
				sprintf(str, CStringReading::LoadString(IDS_WS_SERVERMESSAGE, STR_WS_SERVERMESSAGE_GSRECONNLOGINPLAYERNUM), pGameServer->dwIndex, playerNum);
				AddLogText(str);

				// ���������������
				playerNum = setReadDB.GetLongFromByteArray();
				for(int i=0; i<playerNum; i++)
				{
					bool bFlag = setReadDB.GetByteFromByteArray()==0?false:true;
					if(bFlag)
					{
						CGUID lPlayerID;
						setReadDB.GetBufferFromByteArray(lPlayerID);
						GetGame()->DeleteMapPlayer(lPlayerID);

						CPlayer* pPlayer=GetGame()->NewRawPlayer(ePlayerGsReconnectTwo);
						if(pPlayer)
						{
							pPlayer->SetExID(lPlayerID);
							pPlayer->DecodeFromDataBlock(setReadDB);
							GetGame()->PushSavePlayer(pPlayer);
							pPlayer->SetGsSocket(lSocketID);
						}
					}
				}

				sprintf(str, CStringReading::LoadString(IDS_WS_SERVERMESSAGE, STR_WS_SERVERMESSAGE_GSRECONNSENTLOGINPLAYERNUM), pGameServer->dwIndex, playerNum);
				AddLogText(str);
				// ���븱������
				long rgnNum = setReadDB.GetLongFromByteArray();
				for(int i=0; i<rgnNum; i++)
				{
					BYTE* RgnDataBuf = NULL;
					// rgn data size
					long dataSize = setReadDB.GetLongFromByteArray();
					RgnDataBuf = (BYTE*)M_ALLOC(dataSize);
					setReadDB.GetBufferFromByteArray(RgnDataBuf, dataSize);
					BYTE* RgnVarBuf = NULL;
					// varlist size
					long varSize = setReadDB.GetLongFromByteArray();
					RgnVarBuf = (BYTE*)M_ALLOC(varSize);
					setReadDB.GetBufferFromByteArray(RgnVarBuf, varSize);
					// �滻rgn
					CWorldRegion* rgn = MP_NEW CWorldRegion;
					if(rgn)
					{
						long pos = 0;
						rgn->DecordFromByteArray(RgnDataBuf, pos);
						//rgn->Load();
						CGlobalRgnManager::tagRegion* pTemplateRgn = NULL;
						switch(rgn->GetRgnType())
						{
						case RGN_PERSONAL_HOUSE:
							{
								pTemplateRgn = GetGame()->GetGlobalRgnManager()->GetTemplatePersonalHouseRegion(rgn->GetID());
							}
							break;
						case RGN_PERSONAL:
							{
								pTemplateRgn = GetGame()->GetGlobalRgnManager()->GetTemplatePersonalRegion(rgn->GetID());
							}
							break;
						case RGN_TEAM:
							{
								pTemplateRgn = GetGame()->GetGlobalRgnManager()->GetTemplateTeamRegion(rgn->GetID());
							}
							break;
						}
						if(pTemplateRgn && pTemplateRgn->pRegion)
							rgn->CloneAllData(pTemplateRgn->pRegion);

						pos = 0;
						rgn->GetVariableList()->DecordFromByteArray(RgnVarBuf, pos);
						GetGame()->GetGlobalRgnManager()->GetRgnMap()[rgn->GetExID()] = rgn;
						// gsid
						rgn->SetGsid(pGameServer->dwIndex);
						if(rgn->GetRgnType() == RGN_PERSONAL)
							GetGame()->GetGlobalRgnManager()->AddPersonalRgnGUID(rgn->GetOwnerGUID(), rgn->GetExID());
					}
					M_FREE(RgnDataBuf,dataSize);
					M_FREE(RgnVarBuf,varSize);
				}
				sprintf(str, CStringReading::LoadString(IDS_WS_SERVERMESSAGE, STR_WS_SERVERMESSAGE_GSRECONNSENDRGNDATA), pGameServer->dwIndex);
				AddLogText(str);

				// ���븱����Ʒ����
				long PHRgnNum = setReadDB.GetLongFromByteArray();
				long bufSize = setReadDB.GetLongFromByteArray();
				BYTE* goodsBuf = (BYTE*)M_ALLOC(bufSize);
				setReadDB.GetBufferFromByteArray(goodsBuf, bufSize);
				long pos = 0;
				for(int j=0; j<PHRgnNum; j++)
				{
					CGUID phGUID;
					_GetBufferFromByteArray(goodsBuf, pos, phGUID);
					long goodsNum = _GetLongFromByteArray(goodsBuf, pos);
					if(goodsNum)
					{
						CEntityGroup* dupRgn = NULL;
						CEntityManager::DBEntityComponentMapItr dupItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
						if(dupItr == GetGame()->GetEntityManager()->GetBaseEntityMap().end())
						{
							AddLogText("δ��WS���ȴ���PersonalHouseGroup����!");
							return;
						}

						if(dupItr->second.size() != 1) return;
						CEntityGroup* pPhRgnGroup = dupItr->second.begin()->second;
						map<CGUID, CBaseEntity*>::iterator rgnItr = pPhRgnGroup->GetEntityGroupMap().find(phGUID);
						if(rgnItr == pPhRgnGroup->GetEntityGroupMap().end())
						{
							dupRgn = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgn]"), phGUID);
							pPhRgnGroup->GetEntityGroupMap()[phGUID] = dupRgn;
						}
						else
							dupRgn = (CEntityGroup*)rgnItr->second;

						if(dupRgn)
						{
							// ������˷��ݸ����ڵ���Ʒ
							CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dupRgn, string("[phrgngoodsgroup]"));
							if(pGoodsGroup)
								pGoodsGroup->ReleaseChilds();
						}

						for(int i=0; i<goodsNum; i++)
						{
							struct PHGoods
							{
								// ��ƷGUID
								CGUID guid;
								// ��Ʒ����
								char name[64];
								// ����
								int dir;
								// X����
								int width;
								// Y����
								int height;
								// ����
								float posX;
								float posY;
							};
							PHGoods tGoods;
							_GetBufferFromByteArray(goodsBuf, pos, &tGoods, sizeof(PHGoods));

							CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dupRgn, string("[phrgngoodsgroup]"));
							if(pGoodsGroup)
							{
								CEntity* goods = (CEntity*)GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgngoods]"), tGoods.guid);

								goods->SetGuidAttr(string("guid"),  tGoods.guid);
								goods->SetStringAttr(string("Name"), tGoods.name);
								goods->SetLongAttr(string("Dir"), tGoods.dir);
								goods->SetLongAttr(string("Width"), tGoods.width );
								goods->SetLongAttr(string("Height"), tGoods.height);
								goods->SetLongAttr(string("PosX"), tGoods.posX);
								goods->SetLongAttr(string("PosY"), tGoods.posY);
								goods->SetGuidAttr(string("DupRgnGUID"), dupRgn->GetGUID());

								pGoodsGroup->AddChild(goods);
							}
						}	
					}
				}
				M_FREE(goodsBuf,bufSize);

				pGameServer->bConnected = true;
				CMessage::MapGSIDSocket(pGameServer->dwIndex,lSocketID);

				// ����Ƿ�����GS��������ɲ���DBS׼�����, �ɹ�����saveplayer������������
				bool isSaveFlag = false;
				map<DWORD, CGame::tagGameServer>::iterator gsItr = GetGame()->GetGameServerList()->begin();
				for(; gsItr != GetGame()->GetGameServerList()->end(); gsItr++)
				{
					if(gsItr->second.bConnected)
						isSaveFlag = true;
					else
					{
						isSaveFlag = false;
						break;
					}
				}
				if(isSaveFlag && GetGame()->GetDbIsReady())
				{
					//DoSaveData();
					GetGame()->ProcessDelaySavePlayer();
					GetGame()->SendCdkeyToLoginServer(TRUE);
				}
				else
				{
					GetGame()->SendCdkeyToLoginServer(FALSE);
				}
			}
			else
			{
				GetGame()->InitPersonalHouseDataOnGS(pGameServer->dwIndex);

				//////////////////////////////////////////////////////////////////////////
				// ����ȫ�����ø�GameServer
				vector<BYTE> vectorByte;
				DBWriteSet setWriteDB;

				//WS ��Ϣ				
				CMessage msgWSInfo(MSG_W2S_SERVER_SENDSETUP);
				msgWSInfo.Add((long)SI_WS_INFO);
				msgWSInfo.Add(GetGame()->GetSetup()->dwNumber);
				msgWSInfo.SendToSocket(lSocketID);

				//ȫ���ַ���
				vectorByte.clear();
				CStringReading::AddToByteArray(&vectorByte);
				CMessage msgGlobeString(MSG_W2S_SERVER_SENDSETUP);
				msgGlobeString.Add((long)SI_GLOBALSTRING);
				msgGlobeString.Add((long)vectorByte.size());
				msgGlobeString.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgGlobeString.SendToSocket(lSocketID);

				// ���ͷ��߳�������
				vectorByte.clear();
				GetGame()->GetGlobalRgnManager()->AddLinedRgnInfoToByteArray(vectorByte);
				CMessage msgLinedRgnInfo(MSG_W2S_SERVER_SENDSETUP);
				msgLinedRgnInfo.Add((long)SI_LINED_RGN_INFO);
				msgLinedRgnInfo.Add((long)vectorByte.size());
				msgLinedRgnInfo.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgLinedRgnInfo.SendToSocket(lSocketID);

				//! ���д�
				vectorByte.clear();
				CWordsFilter::GetInstance()->AddToByteArray(vectorByte);
				CMessage msgSensitiveWord(MSG_W2S_SERVER_SENDSETUP);
				msgSensitiveWord.Add((long)SI_SensitiveWord);
				msgSensitiveWord.Add((long)vectorByte.size());
				msgSensitiveWord.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgSensitiveWord.SendToSocket(lSocketID);

				// ȫ�ֲ���
				vectorByte.clear();
				CGlobeSetup::AddToByteArray(&vectorByte);
				CMessage msgGlobeSetup(MSG_W2S_SERVER_SENDSETUP);
				msgGlobeSetup.Add((long)SI_GLOBESETUP);
				msgGlobeSetup.Add((long)vectorByte.size());
				msgGlobeSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgGlobeSetup.SendToSocket(lSocketID);

				// ��Ʒ��Ϣ
				vectorByte.clear();
				CGoodsFactory::Serialize(&vectorByte);
				CMessage msgGoodsList(MSG_W2S_SERVER_SENDSETUP);
				msgGoodsList.Add((long)SI_GOODSLIST);
				msgGoodsList.Add((long)vectorByte.size());
				msgGoodsList.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgGoodsList.SendToSocket(lSocketID);

				//COUNTTIMERLIST SETUP
				vectorByte.clear();
				CCountTimerList::Serialize(vectorByte);
				CMessage msgCountTimer(MSG_W2S_SERVER_SENDSETUP);
				msgCountTimer.Add((long)SI_COUNTTIMERLIST);
				msgCountTimer.Add((long)vectorByte.size());
				msgCountTimer.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgCountTimer.SendToSocket(lSocketID);

				//SUIT SETUP
				vectorByte.clear();
				CSuitRuleFactory::Serialize(&vectorByte);
				CMessage msgSuitSetup(MSG_W2S_SERVER_SENDSETUP);
				msgSuitSetup.Add((long)SI_SUITSETUP);
				msgSuitSetup.Add((long)vectorByte.size());
				msgSuitSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgSuitSetup.SendToSocket(lSocketID);

				//ENCHASE SETUP
				vectorByte.clear();
				CKitCardRuleFactory::Serialize(&vectorByte);
				CMessage msgEnchaseSetup(MSG_W2S_SERVER_SENDSETUP);
				msgEnchaseSetup.Add((long)SI_ENCHASESETUP);
				msgEnchaseSetup.Add((long)vectorByte.size());
				msgEnchaseSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgEnchaseSetup.SendToSocket(lSocketID);

				// ������������  
				GetInst( Business::CBusinessManager ).SendSetupToGS( pGameServer->dwIndex, lSocketID );

				//! ��Ʒ��չ����
				vectorByte.clear();
				GetGame()->SerializeGoodsExSetup(vectorByte);
				CMessage msgGoodsExSetup(MSG_W2S_SERVER_SENDSETUP);
				msgGoodsExSetup.Add((long)SI_GOODSEX);
				msgGoodsExSetup.Add((long)vectorByte.size());
				msgGoodsExSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgGoodsExSetup.SendToSocket(lSocketID);

				//! �Ǹ�ְҵ�����䷽
				CMessage msgGoodsTailor(MSG_W2S_SERVER_SENDSETUP);
				msgGoodsTailor.Add((long)SI_GOODSTAILOR);
				DBWriteSet setGoodsTailorWriteDB;
				msgGoodsTailor.GetDBWriteSet(setGoodsTailorWriteDB);
				GoodsTailorSetup::GetInstance().Code(setGoodsTailorWriteDB);
				msgGoodsTailor.SendToSocket(lSocketID);
				
				//! �ֿ�����
				CMessage msgDepotMsg(MSG_W2S_SERVER_SENDSETUP);
				msgDepotMsg.Add((long)SI_DEPOT);
				msgDepotMsg.Add((long)sizeof(tagDepotSetup));
				msgDepotMsg.AddEx((void*)PlayerDepot::GetDepotSetup(), sizeof(tagDepotSetup));
				msgDepotMsg.SendToSocket(lSocketID);

				//! ����ϵͳ����
				vectorByte.clear();
				LinkmanSystem::GetInstance().SetupAddToByteArray(vectorByte);
				CMessage msgLGSManage(MSG_W2S_SERVER_SENDSETUP);
				msgLGSManage.Add((long)SI_LINKMAN);
				msgLGSManage.Add((long)vectorByte.size());
				msgLGSManage.AddEx(&(vectorByte[0]), (long)vectorByte.size());
				msgLGSManage.SendToSocket(lSocketID);

				// ��������
				vectorByte.clear();
				CMonsterList::AddToByteArray(&vectorByte);
				CMessage msgMonsterList(MSG_W2S_SERVER_SENDSETUP);
				msgMonsterList.Add((long)SI_MONSTERLIST);
				msgMonsterList.Add((long)vectorByte.size());
				msgMonsterList.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgMonsterList.SendToSocket(lSocketID);
				
				// Fox@20081120 ��ҹ���
				GetGame()->LoadEx(RELOAD_PLAYER_MAN, lSocketID, false);

				// Fox@20081119 ����
				GetGame()->LoadEx(RELOAD_PET_LIST, lSocketID, false);
			
				// BaiYun@2009525 �����
				GetGame()->LoadEx(RELOAD_ANTI_CHEAT, lSocketID, false);

				// ��Ŀ����
				GetInst( AntiCheat::CQuestionManager ).SendQuestionsToGS( lSocketID );
			
				//������
				vectorByte.clear();
				CGateList::AddToByteArray(&vectorByte);
				CMessage msgGateList(MSG_W2S_SERVER_SENDSETUP);
				msgGateList.Add((long)SI_GATELIST);
				msgGateList.Add((long)vectorByte.size());
				msgGateList.AddEx(&vectorByte[0],(long)vectorByte.size());
				msgGateList.SendToSocket(lSocketID);
				
				//CDeputyOccuSystem
				vectorByte.clear();
				CDeputyOccuSystem::AddToByteArray(&vectorByte);
				CMessage msgDeputyOccu(MSG_W2S_SERVER_SENDSETUP);
				msgDeputyOccu.Add((long)SI_DEPUTYOCCU);
				msgDeputyOccu.Add((long)vectorByte.size());
				msgDeputyOccu.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgDeputyOccu.SendToSocket(lSocketID);

				// ��������
				vectorByte.clear();
				CHitLevelSetup::AddToByteArray(&vectorByte);
				CMessage msgHitLevelSetup(MSG_W2S_SERVER_SENDSETUP);
				msgHitLevelSetup.Add((long)SI_HITLEVEL);
				msgHitLevelSetup.Add((long)vectorByte.size());
				msgHitLevelSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgHitLevelSetup.SendToSocket(lSocketID);

				// �����Ϣ
				vectorByte.clear();
				CPlayerList::AddToByteArray(&vectorByte);
				CMessage msgPlayerList(MSG_W2S_SERVER_SENDSETUP);
				msgPlayerList.Add((long)SI_PLAYERLIST);
				msgPlayerList.Add((long)vectorByte.size());
				msgPlayerList.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgPlayerList.SendToSocket(lSocketID);

				// ������Ϣ
				vectorByte.clear();
				CEmotion::Serialize( vectorByte );
				CMessage msgEmotion(MSG_W2S_SERVER_SENDSETUP);
				msgEmotion.Add((long)SI_EMOTION);
				msgEmotion.Add((long)vectorByte.size());
				msgEmotion.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgEmotion.SendToSocket(lSocketID);

				// ������Ϣ
				vectorByte.clear();
				CNewSkillFactory::CodeNewSkill(vectorByte);
				COccupSkillList::AddToByteArray(vectorByte);
				CStudySkillCondition::AddToByteArray(vectorByte);
				
				CMessage msgSkillList(MSG_W2S_SERVER_SENDSETUP);
				msgSkillList.Add((long)SI_SKILLLIST);
				msgSkillList.Add((long)vectorByte.size());
				msgSkillList.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgSkillList.SendToSocket(lSocketID);

				// ������Ϣ
				vectorByte.clear();
				CTradeList::AddToByteArray(&vectorByte);
				CMessage msgTradeList(MSG_W2S_SERVER_SENDSETUP);
				msgTradeList.Add((long)SI_TRADELIST);
				msgTradeList.Add((long)vectorByte.size());
				msgTradeList.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgTradeList.SendToSocket(lSocketID);

				// ��ֵ��Ʒ������Ϣ
				vectorByte.clear();
				CIncrementShopList::AddToByteArray(&vectorByte);
				CMessage msgIncrementShopList(MSG_W2S_SERVER_SENDSETUP);
				msgIncrementShopList.Add((long)SI_INCREMENTSHOPLIST);
				msgIncrementShopList.Add((long)vectorByte.size());
				msgIncrementShopList.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgIncrementShopList.SendToSocket(lSocketID);

				vectorByte.clear();
				CIncrementShopManager::GetSingleton().AddToByteArray(&vectorByte);
				CMessage msgIncLimitGoodsRec( MSG_W2S_SERVER_SENDSETUP );
				msgIncLimitGoodsRec.Add((long)SI_INCLIMITGOODSREC);
				msgIncLimitGoodsRec.Add((long)vectorByte.size());
				msgIncLimitGoodsRec.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgIncLimitGoodsRec.SendToSocket(lSocketID);
				

				// ��ֵ��Ʒ������Ϣ
				vectorByte.clear();
				CContributeSetup::AddToByteArray(&vectorByte);
				CMessage msgContributeSetup(MSG_W2S_SERVER_SENDSETUP);
				msgContributeSetup.Add((long)SI_CONTRIBUTEITEM);
				msgContributeSetup.Add((long)vectorByte.size());
				msgContributeSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgContributeSetup.SendToSocket(lSocketID);

				// ��־ϵͳ������
				vectorByte.clear();
				CLogSystem::AddToByteArray(&vectorByte);
				CMessage msgLogSystem(MSG_W2S_SERVER_SENDSETUP);
				msgLogSystem.Add((long)SI_LOGSYSTEM);
				msgLogSystem.Add((long)vectorByte.size());
				msgLogSystem.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgLogSystem.SendToSocket(lSocketID);

				// ���Ҳ�����
				vectorByte.clear();
				GetCountryParam()->AddToByteArray(&vectorByte);
				CMessage msgCountryParam(MSG_W2S_SERVER_SENDSETUP);
				msgCountryParam.Add((long)SI_COUNTRYPARAM);
				msgCountryParam.Add((long)vectorByte.size());
				msgCountryParam.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgCountryParam.SendToSocket(lSocketID);

				// ������Ϣ
				vectorByte.clear();
				GetCountryHandler()->AddToByteArray(&vectorByte);
				CMessage msgCountry(MSG_W2S_SERVER_SENDSETUP);
				msgCountry.Add((long)SI_COUNTRY);
				msgCountry.Add((long)vectorByte.size());
				msgCountry.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgCountry.SendToSocket(lSocketID);

				//ϵͳ�ʼ�
				vectorByte.clear();
				CMailList::AddToByteArray(&vectorByte);
				CMessage msgMailList(MSG_W2S_SERVER_SENDSETUP);
				msgMailList.Add((long)SI_SYSMAIL);
				msgMailList.Add((long)vectorByte.size());
				msgMailList.AddEx(&vectorByte[0],(long)vectorByte.size());
				msgMailList.SendToSocket(lSocketID);
			
				
				//�ű�AI����
				vectorByte.clear();
				CScriptAILoader::Instance().AddToByteArray( vectorByte );
				CMessage msgScriptAI( MSG_W2S_SERVER_SENDSETUP );
				msgScriptAI.Add( (long)SI_SCRIPTAI );
				msgScriptAI.Add( (long)vectorByte.size() );
				msgScriptAI.AddEx( &vectorByte[0], (long)vectorByte.size() );
				msgScriptAI.SendToSocket( lSocketID );

				//������Ʒ�б�
				vectorByte.clear();
				CReLiveGoodsList::AddToByteArray( vectorByte );
				CMessage msgReLiveGoods( MSG_W2S_SERVER_SENDSETUP );
				msgReLiveGoods.Add( (long)SI_RELIVEGOODS );
				msgReLiveGoods.Add( (long)vectorByte.size() );
				msgReLiveGoods.AddEx( &vectorByte[0], (long)vectorByte.size() );
				msgReLiveGoods.SendToSocket( lSocketID );

				//���������б�
				vectorByte.clear();
				CCosmetology::AddToByteArray(vectorByte);
				CMessage msgCosme( MSG_W2S_SERVER_SENDSETUP );
				msgCosme.Add( (long)SI_COSMESYSTEM );
				msgCosme.Add( (long)vectorByte.size() );
				msgCosme.AddEx( &vectorByte[0], (long)vectorByte.size() );
				msgCosme.SendToSocket( lSocketID );
			


				//�ı䵽�Ľű�����
				// FunctionList
				if (GetGame()->GetFuncitonList_FileData())
				{
					long len = GetGame()->GetFuncitonList_FileData_Size();

					CMessage msgFunction(MSG_W2S_SERVER_SENDSETUP);
					msgFunction.Add((long)SI_FUNCTIONLIST);
					msgFunction.Add(len);
					msgFunction.AddEx((void*)GetGame()->GetFuncitonList_FileData(), len);
					msgFunction.SendToSocket(lSocketID);
				}

				// VariableList
				if (GetGame()->GetVariableList_FileData())
				{
					long len = GetGame()->GetVariableList_FileData_Size();

					CMessage msgVariable(MSG_W2S_SERVER_SENDSETUP);
					msgVariable.Add((long)SI_VARIABLELIST);
					msgVariable.Add(len);
					msgVariable.AddEx((void*)GetGame()->GetVariableList_FileData(), len);
					msgVariable.SendToSocket(lSocketID);
				}

				// GeneralVariableList
				if (GetGame()->GetGeneralVariableList() && GetGame()->GetInitVarLoadFlag())
				{
					CMessage msgGereralVariable(MSG_W2S_SERVER_SENDSETUP);
					msgGereralVariable.Add((long)SI_GENERALVARIABLELIST);
					vector<BYTE> GererData;
					GetGame()->GetGeneralVariableList()->AddToByteArray(&GererData);
					msgGereralVariable.Add((long)GererData.size());
					if(GererData.size() > 0)
						msgGereralVariable.AddEx(&GererData[0],GererData.size());
					msgGereralVariable.SendToSocket(lSocketID);
				}

				// �ű��ļ�
				for(map<string, char*>::iterator it2=GetGame()->GetScriptFileDataMap()->begin(); it2!=GetGame()->GetScriptFileDataMap()->end(); it2++)
				{
					string filename = it2->first;
					CMessage msgScript(MSG_W2S_SERVER_SENDSETUP);
					msgScript.Add((long)SI_SCRIPTFILE);
					msgScript.Add(filename.c_str());
					long lLen = (long)lstrlen(it2->second);
					msgScript.Add(lLen);
					msgScript.AddEx(it2->second,lLen);
					msgScript.SendToSocket(lSocketID);
				}

				// Normal Rgn
				for(map<long, CGlobalRgnManager::tagRegion>::iterator it = GetGame()->GetGlobalRgnManager()->GetTemplateRegionListMap()->begin(); it!=GetGame()->GetGlobalRgnManager()->GetTemplateRegionListMap()->end(); it++)
				{
					CGlobalRgnManager::tagRegion* pRegion = &it->second;
					if( pRegion )
					{
						if (pRegion->dwGameServerIndex == pGameServer->dwIndex)
						{
							vectorByte.clear();
							pRegion->pRegion->AddToByteArray(&vectorByte);
							CMessage msgTradeList(MSG_W2S_SERVER_SENDSETUP);
							msgTradeList.Add((long)SI_REGION);
							msgTradeList.Add((long)pRegion->RegionType);
							msgTradeList.Add((long)pRegion->pRegion->GetID());
							msgTradeList.Add(pRegion->pRegion->GetExID());
							msgTradeList.Add((long)vectorByte.size());
							msgTradeList.AddEx(&vectorByte[0], (long)vectorByte.size());
							msgTradeList.SendToSocket(lSocketID);
							Sleep(10);
						}
					}
				}
			
				// �����ȼ��б�
				vectorByte.clear();
				CRegionSetup::AddToByteArray(&vectorByte);
				CMessage msgRegionSetup(MSG_W2S_SERVER_SENDSETUP);
				msgRegionSetup.Add((long)SI_REGIONLEVELSETUP);
				msgRegionSetup.Add((long)vectorByte.size());
				msgRegionSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgRegionSetup.SendToSocket(lSocketID);

				//���͸���������Ϣ
				vectorByte.clear();
				GetGame()->GetDupliRegionSetup()->AddToByteArray(&vectorByte);
				CMessage msgDupliRegionSetup(MSG_W2S_SERVER_SENDSETUP);
				msgDupliRegionSetup.Add((long)SI_DUPLIREGIONSETUP);
				msgDupliRegionSetup.Add((long)vectorByte.size());
				msgDupliRegionSetup.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgDupliRegionSetup.SendToSocket(lSocketID);

				//QuestList
				vectorByte.clear();
				CQuestSystem::AddToByteArray(&vectorByte);
				CMessage msgQuest(MSG_W2S_SERVER_SENDSETUP);
				msgQuest.Add((long)SI_QUEST);
				msgQuest.Add((long)vectorByte.size());
				msgQuest.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgQuest.SendToSocket(lSocketID);

				// GMLIST
				vectorByte.clear();
				CGMList::AddToByteArray(&vectorByte);
				CMessage msgGMList(MSG_W2S_SERVER_SENDSETUP);
				msgGMList.Add((long)SI_GMLIST);
				msgGMList.Add((long)vectorByte.size());
				msgGMList.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgGMList.SendToSocket(lSocketID);

				CMessage msgGoodsID(MSG_W2S_SERVER_SENDSETUP);
				msgGoodsID.Add((long)SI_GAMESERVERINDEX);
				msgGoodsID.Add((BYTE)pGameServer->dwIndex);
				msgGoodsID.SendToSocket(lSocketID);

				GetOrganiCtrl()->OnGsConnect(lSocketID);
				//! ��������
				GetOrganiCtrl()->SendTeamSetupToGS(lSocketID);
				GetOrganiCtrl()->SendTeamDataToGS(lSocketID);

				//! С����ϵͳ
				vectorByte.clear();
				SpriteSystem::SetupAddToByteArray(vectorByte);
				CMessage msgSprite(MSG_W2S_SERVER_SENDSETUP);
				msgSprite.Add((LONG)SI_SPRITE);
				msgSprite.Add((long)vectorByte.size());
				msgSprite.AddEx(&vectorByte[0], vectorByte.size());
				msgSprite.SendToSocket(lSocketID);
				
				pGameServer->bConnected = true;
				CMessage::MapGSIDSocket(pGameServer->dwIndex,lSocketID);
				
				map<long, CGlobalRgnManager::tagRegion>* pPMap = GetGame()->GetGlobalRgnManager()->GetTemplatePersonalRgnListMap();
				map<long, CGlobalRgnManager::tagRegion>* pTMap = GetGame()->GetGlobalRgnManager()->GetTemplateTeamRgnListMap();
				if(pPMap && pTMap)
				{
					CMessage msgDupData(MSG_W2S_SERVER_SENDSETUP);
					msgDupData.Add((LONG)SI_DUPRGNDATA);
					msgDupData.Add((long)pPMap->size() + pTMap->size()); // ����ģ�����

					map<long, CGlobalRgnManager::tagRegion>::iterator prgnitr = pPMap->begin();
					for(; prgnitr != pPMap->end(); prgnitr++)
					{
						msgDupData.Add((long)prgnitr->first); // ����ģ��ID
						vector<BYTE> pBA;
						if(prgnitr->second.pRegion)
						{
							CWorldRegion* pRgn = MP_NEW CWorldRegion;
							pRgn->CloneAllData(prgnitr->second.pRegion);
							pRgn->AddToByteArray(&pBA);
							delete pRgn;
						}
						msgDupData.Add((long)pBA.size()); // bufsize
						if(pBA.size() > 0)
							msgDupData.AddEx(&pBA[0], (long)pBA.size()); // buf	
					}

					map<long, CGlobalRgnManager::tagRegion>::iterator trgnitr = pTMap->begin();
					for(; trgnitr != pTMap->end(); trgnitr++)
					{
						msgDupData.Add((long)trgnitr->first); // ����ģ��ID
						vector<BYTE> pBA;
						if(trgnitr->second.pRegion)
						{
							CWorldRegion* pRgn = MP_NEW CWorldRegion;
							pRgn->CloneAllData(trgnitr->second.pRegion);
							pRgn->AddToByteArray(&pBA);
							delete pRgn;
						}
						msgDupData.Add((long)pBA.size()); // bufsize
						if(pBA.size() > 0)
							msgDupData.AddEx(&pBA[0], (long)pBA.size()); // buf	
					}

					msgDupData.SendToSocket(lSocketID);
				}
				
				//������������
				CMessage msgCredit(MSG_W2S_SERVER_SENDSETUP);
				msgCredit.Add((long)SI_CREDITSYSTEM);
				DBWriteSet dbwriteset;
				msgCredit.GetDBWriteSet(dbwriteset);
				GetInst(CCreditSystem).CodeToByteArray(dbwriteset);
				msgCredit.SendToSocket(lSocketID);

				// ������������
				GetInst( FBusiness::CConfiger ).SendToGS( lSocketID );

				//����������������
				CMessage msgQuestIndex(MSG_W2S_SERVER_SENDSETUP);
				msgQuestIndex.Add((long)SI_QUESTINDEX);
				DBWriteSet dbwriteset_Index;
				msgQuestIndex.GetDBWriteSet(dbwriteset_Index);
				GetInst(CQuestIndexXml).CodeToByteArray(dbwriteset_Index);
				msgQuestIndex.SendToSocket(lSocketID);

				//���Ͳ�Ʊϵͳ����
				CMessage msgLottery(MSG_W2S_SERVER_SENDSETUP);
				msgLottery.Add((long)SI_Lottery);
				DBWriteSet dbwriteset_Lottery;
				msgLottery.GetDBWriteSet(dbwriteset_Lottery);
				GetInst(CLotteryXml).CodeToByteArray(dbwriteset_Lottery);
				msgLottery.SendToSocket(lSocketID);

				// ������GameServer��ʽ��������Ϣ
				CMessage msg(MSG_W2S_SERVER_SENDSETUP);
				msg.Add((long)SI_END);
				msg.Add((long)lGsSleepTime);
				msg.SendToSocket(lSocketID);

				lGsSleepTime++;

				// ����Ƿ�����GS��������ɲ���DBS׼�����, �ɹ�����saveplayer������������
				bool isSaveFlag = false;
				map<DWORD, CGame::tagGameServer>::iterator gsItr = GetGame()->GetGameServerList()->begin();
				for(; gsItr != GetGame()->GetGameServerList()->end(); gsItr++)
				{
					if(gsItr->second.bConnected)
						isSaveFlag = true;
					else
					{
						isSaveFlag = false;
						break;
					}
				}

				// ��������
				if( GetGame()->GetGameServerList() && lGsSleepTime > GetGame()->GetGameServerList()->size()) lGsSleepTime = 0;

				if(isSaveFlag && GetGame()->GetDbIsReady())
				{
					//DoSaveData();
					GetGame()->ProcessDelaySavePlayer();
					GetGame()->SendCdkeyToLoginServer(TRUE);
				}
				else
					GetGame()->SendCdkeyToLoginServer(FALSE);
			}
		}
		break;
		
	case MSG_W2W_SERVER_LOGINSERVERCLOSE:
		{
			AddLogText("========= LoginServer closed =========");
			GetGame()->CreateConnectLoginThread();
		}
		break;

		// �Ʒѷ������Ͽ�
	case MSG_W2W_SERVER_COSTSERVERCLOSE:
		{
			AddLogText("========= CostServer closed =========");
			GetGame()->CreateConnectCostThread();
		}
		break;

	case MSG_W2W_RECONNECTLOGINSERVEROK:
		{
			
		}
		break;

	case MSG_W2W_RECONNECTCOSTSERVEROK:
		{
		
		}
		break;

		// �л�������
	case MSG_S2W_SERVER_CHANGEREGION:
		{
			// RgnType
			eRgnType rgnType = (eRgnType)pMsg->GetLong();
			// ��ͨ������ģ��ID���Բ��ҵ�GUID
			long templateID = pMsg->GetLong(); 

			CGUID guid; pMsg->GetGUID(guid);
			CGUID lRegionID;
			pMsg->GetGUID(lRegionID);

			CPlayer* pPlayer = GetGame()->GetMapPlayer(guid);
			if(!pPlayer)
			{
				char szGuid[128];
				guid.tostring(szGuid);
				AddLogText("ChangeRgn,δ�ҵ�CPlayer����, GUID[%s]!", szGuid);
				// �л�����ʧ�� 
				CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
				// �����֤����
				msg.Add((long)0xFFFFFFFF);
				msg.Add((char)0);	// ʧ��
				msg.Add(guid);
				msg.SendToSocket(lSocketID);
				return;
			}

			long lTileX = pMsg->GetLong();
			long lTileY = pMsg->GetLong();
			long lDir = pMsg->GetLong();
			long lUseGoods = pMsg->GetLong();
			long lRange = pMsg->GetLong();

			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
			pPlayer->DecodeFromDataBlock(setReadDB);
			
			const char* szCdkey = pPlayer->GetAccount();
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(szCdkey);
			if(!tLoginInfo)
			{
				char outStr[1024];
				sprintf(outStr, "MSG_S2W_SERVER_CHANGEREGION! LoginInfo[%s] Is Null!", szCdkey);
				AddLogText(outStr);
				// �л�����ʧ�� 
				CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
				// �����֤����
				msg.Add((long)0xFFFFFFFF);
				msg.Add((char)0);	// ʧ��
				msg.Add(guid);
				msg.SendToSocket(lSocketID);
				return;
			}
			if(tLoginInfo)
			{
				// ������֤��
				long lTmpSignCode = 0;
				GetGame()->GenerateSignCodeByTime(lTmpSignCode);
				tLoginInfo->SetSignCode(lTmpSignCode);
				tLoginInfo->SetPlayerState(PLAYER_STATE_CHANGING_SERVER);
				tLoginInfo->SetLoginTimeoutValue(timeGetTime());
			}
			else
			{
				AddLogText("ChangeRgn,δ�ҵ�tLoginInfo����, cdkey[%s]!", szCdkey);
				return;
			}

			// ��ͨ����
			if(rgnType == RGN_NORMAL)
			{
				CGlobalRgnManager::tagRegion* tRgn = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(templateID);
				if(tRgn)
					lRegionID = tRgn->pRegion->GetExID();
			}

			CWorldRegion* pRegion = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(lRegionID);
			long changeGSID = -1;
			if(pRegion)
				changeGSID = pRegion->GetGsid();
		
			if (!pRegion)
			{
				// �л�����ʧ�� 
				CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
				// �����֤����
				msg.Add((long)tLoginInfo->GetSignCode());
				msg.Add((char)0);	// ʧ��
				msg.Add(guid);
				msg.SendToSocket(lSocketID);

				// ����һ�α�����Ϣ
				GetGame()->GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);
				GetGame()->GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());

				char szGuid[40] = {0};
				guid.tostring(szGuid);
				char pszLogingInfo[512]="";
				_snprintf(pszLogingInfo,512, CStringReading::LoadString(IDS_WS_SERVERMESSAGE, STR_WS_SERVERMESSAGE_CHANGERGNNORGNONGS), templateID, szGuid);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				return;
			}

			CGame::tagGameServer* pServer = GetGame()->GetGameServer(changeGSID);
			if (!pServer || pServer->bConnected == false)
			{
				// ����״̬
				tLoginInfo->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);
				
				// ����һ�α�����Ϣ
				GetGame()->GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);
				GetGame()->GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());

				// �л�����ʧ��
				CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
				// �����֤����
				msg.Add((long)tLoginInfo->GetSignCode());
				msg.Add((char)0);	// ʧ��
				msg.Add(guid);
				msg.SendToSocket(lSocketID);
				char szGuid[40] = {0};
				guid.tostring(szGuid);
				char pszLogingInfo[512]="";
				_snprintf(pszLogingInfo,512, CStringReading::LoadString(IDS_WS_SERVERMESSAGE, STR_WS_SERVERMESSAGE_CHANGERGNGSNOTRUN), templateID, szGuid, changeGSID);
				PutStringToFile("Login_WS_Info",pszLogingInfo);
				return;
			}

			pPlayer->SetRegionExID(lRegionID);
			pPlayer->SetPosXY(lTileX+0.5f, lTileY+0.5f);
			pPlayer->SetDir(lDir);
			pPlayer->SetRegionID(pRegion->GetID());
			pPlayer->SetCurRgnType(rgnType);

			// Online -> Login
			// ��ӵ���½�б���
			pPlayer->SetPlayerLogin(true);
			pPlayer->SetFirstLogin(false);
			pPlayer->SetBeginLoginTime(timeGetTime());
			
			// ����һ�α�����Ϣ
			GetGame()->GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);
			GetGame()->GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());

			// �л������ɹ�
			CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
			// �����֤����
			msg.Add((long)tLoginInfo->GetSignCode());
			msg.Add((char)1);	// �ɹ�
			msg.Add(guid);
			msg.Add(pServer->strIP.c_str());
			msg.Add(pServer->dwPort);
			msg.SendToSocket(lSocketID);
		}
		break;
	case MSG_S2W_SERVER_ADDINFO:
		/**## ��������Ϣ������GameServer����� */
		{
			char	strName[24];
			char	strMsg[1024];
			DWORD	dwColor = 0;
			DWORD	dwBkColor = 0;
			CGUID	SenderGuid;

			/**## ������ʼ�� */
			ZeroMemory( strName, sizeof( strName ) );
			ZeroMemory( strMsg,  sizeof( strMsg )  );

			pMsg -> GetStr( strName,24 );
			pMsg -> GetStr( strMsg,1024 );
			dwColor = pMsg -> GetDWord();
			dwBkColor = pMsg->GetDWord();
			pMsg -> GetGUID(SenderGuid);

			CPlayer* pPlayer;
			pPlayer = GetGame() -> GetMapPlayer( strName );
			if( pPlayer )
				/**## �ҵ��û���������Ϣ���û����ڵķ����� */
				/**## �ɹ���LONG Reciver, char* Msg, DWORD Color, char* Sender    
				## ʧ�ܣ�LONG Reciver(=0), char* Reciver, LONG Sender */
			{
				/**## ��ȡ�û���ǰ���ڵ�GameServer */
				long lGameServer =
					GetGame() -> GetGameServerNumber_ByPlayerID( pPlayer->GetExID() );
				if( lGameServer )
				{
					CMessage msg( MSG_W2S_SERVER_ADDINFO );
					msg.Add( pPlayer->GetExID() );
					msg.Add( strMsg );
					msg.Add( dwColor );
					msg.Add( dwBkColor );
					/**## ��д�����ߵ����� */
					CPlayer* pSender = GetGame() -> GetMapPlayer( SenderGuid );
					if( pSender )
					{
						msg.Add( const_cast<char*>( pSender -> GetName() ) );
					}
					else
					{
						char strSender[256];
						char szSenderGuid[40] = {0};
						SenderGuid.tostring(szSenderGuid);
						sprintf( strSender, "uid[%s]", szSenderGuid );
						msg.Add( strSender );
					}
					msg.SendGSID( lGameServer );
				}
			}
			else
				/**## �Ҳ����û���������Ϣ�������� */
			{
				CPlayer* pSender = GetGame() -> GetMapPlayer( SenderGuid );
				if( pSender )
				{
					long lGameServer =
						GetGame() -> GetGameServerNumber_ByPlayerID( pSender -> GetExID() );
					if( lGameServer )
					{
						CMessage msg( MSG_W2S_SERVER_ADDINFO );
						msg.Add( static_cast<long>(0) );
						msg.Add( strName );
						msg.Add( SenderGuid );
						msg.SendGSID( lGameServer );
					}
				}
			}
		}
		break;

		// ����ȫ�ֽű�������ֵ
	case MSG_S2W_SERVER_CHANGE_SCRIPT_VARIABLE:
		{
			char strVarName[256] = {0};
			pMsg->GetStr(strVarName, 256);
			if( strVarName )
			{
				//ͳ�ƴ���
				//��ӵ�map��,������Ӻ�����������˼Ӳ���������ֻ����ֵ
				GetGame()->AddVarScriptExpendTime(strVarName,1);

				long lPos = 0;
				int lRet = CVariableList::VAR_NOT_FOUND;
				double nValue = 0;
				LONG64 llRet = 0;
				BYTE nFlag = 0;
				char szStr[1024] = {0};
				CGUID guid;

				nFlag = pMsg->GetByte(); // 0 ��ֵ
				if( eVarNumber == nFlag )
				{
					lPos = pMsg->GetLong();
					llRet = pMsg->GetLONG64();
					nValue = *(double*)&llRet;
					lRet = GetGame()->GetGeneralVariableList()->SetVarValue(strVarName, lPos, nValue);
				}
				else if( eVarString == nFlag ) // 1 �ַ���
				{
					pMsg->GetStr(szStr, 1024);
					lRet = GetGame()->GetGeneralVariableList()->SetVarValue(strVarName, szStr);
				}
				else if( eVarGuid == nFlag ) // 2 guid
				{
					pMsg->GetGUID(guid);
					lRet = GetGame()->GetGeneralVariableList()->SetGuidValue(strVarName, &guid);
				}
				CEntityManager::DBEntityComponentMapItr varMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[scriptvargroup]"));
				if( varMapItr == GetGame()->GetEntityManager()->GetBaseEntityMap().end() )
				{
					AddLogText("MSG_S2W_OTHER_UPDATEGLOBALSCTIPTVARS δ�ҵ�VarGroup����,���������ļ�!");
					return;
				}
				map<CGUID, CEntityGroup*>::iterator enItr = varMapItr->second.begin();
				if( enItr == varMapItr->second.end() )
				{
					AddLogText("MSG_S2W_OTHER_UPDATEGLOBALSCTIPTVARS δ�ҵ�VarGroup����,��������ʼ������!");
					return;
				}
				CEntityGroup* pVarGroup = (CEntityGroup*)enItr->second;
				if( pVarGroup )
				{
					pVarGroup->ReleaseChilds();
					GetGame()->GetEntityManager()->CGlobalVarGroupToCGDBlobalVar(pVarGroup);
				}

				if( lRet != CVariableList::VAR_NOT_FOUND )
				{
					CMessage msg(MSG_W2S_SERVER_CHANGE_SCRIPT_VARIABLE);
					msg.Add(strVarName);
					msg.Add((BYTE)nFlag);
					if( eVarNumber == nFlag )
					{
						msg.Add((long)lPos);
						msg.Add((LONG64)llRet);
					}
					else if( eVarString == nFlag )
						msg.Add(szStr);
					else if( eVarGuid == nFlag )
						msg.Add(guid);
					msg.SendAll();
				}
			}
		}
		break;

		//##��Ϸ��������������������;ٱ�ɱ���ߵ���Ϣ
	case MSG_S2W_SERVER_REPORT_MURDERER:
		{
			LONG lSuffererType			= pMsg -> GetLong();
			LONG lSuffererID			= pMsg -> GetLong();
			LONG lMurdererType			= pMsg -> GetLong();
			CGUID MurdererID;
			pMsg -> GetGUID(MurdererID);
			//##��ѯ������Ǹ�������
			long lGameServer	= GetGame() -> GetGameServerNumber_ByPlayerID( MurdererID );
			if( lGameServer )
			{
				pMsg -> SetType( MSG_W2S_SERVER_SYNC_MURDERER_COUNT );
				pMsg -> SendGSID( lGameServer );
			}
			//##���������,�����ڱ��ز���
			else 
			{
				CPlayer* pPlayer = GetGame() -> GetMapPlayer( MurdererID );
				if( pPlayer )
				{
					pPlayer -> SetPkCount( pPlayer -> GetPkCount() + 1 );
					pPlayer -> SetPkValue( pPlayer -> GetPkValue() + 1 );
				}
				//##����Ҳ���,�Ƿ�ֱ���޸����ݿ�?
				//##Jar@2004-7-16
			}
		}
		break;

	case MSG_S2W_SERVER_UpdateRegionInfo:
		{
			long lRegionID = pMsg->GetLong();
			//CGame::tagRegion* ptagRegion = GetGame()->GetRegion(lRegionID);
			CGlobalRgnManager::tagRegion* ptagRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
			if (ptagRegion && ptagRegion->pRegion)
			{
				long lBufSize = pMsg->GetLong();
				BYTE *pRegionData = (BYTE*)M_ALLOC(lBufSize);
				pMsg->GetEx(pRegionData,lBufSize);
				long lPos = 0;
				ptagRegion->pRegion->DecordFromByteArray(pRegionData,lPos);
				M_FREE(pRegionData,lBufSize);
			}
		}
		break;
	// DBServer�Ķ�ʱping
	case MSG_DB2W_SERVER_QUEST_PING:
		{
			GetGame()->EndPingDS();

			if(GetGame()->GetPingGsNum() >= GetGame()->GetGSNum() && GetGame()->FinishPingDS())
			{
				CMessage msg(MSG_W2L_SERVER_ANSWER_PING);
				msg.Add((long)1);
				msg.SendToSocket(GetGame()->GetLSSocketID());
			}
		}
		break;
		// LoginServer�Ķ�ʱping
	case MSG_L2W_SERVER_QUEST_PING:
		{
			GetGame()->SetInPing(true);
			GetGame()->ClearPingGameServerInfo();
			GetGame()->SetLastPingGameServerTime(timeGetTime());

			GetGame()->BeginPingDS();

			CMessage msg(MSG_W2S_SERVER_QUEST_PING);
			msg.Add((DWORD)pMsg->GetType());
			msg.SendAll();

			CMessage msgDB(MSG_W2DB_SERVER_QUEST_PING);
			msgDB.SendToSocket(GetGame()->GetDBSocketID());


			GetGame()->ReturnGsPlayerNum();
			
		}
		break;

		// �յ�GameServer�ķ���
	case MSG_S2W_SERVER_ANSWER_PING:
		{
			char strIP[256];
			CGame::tagPingGameServerInfo stPingGame;
			stPingGame.dwPlayerNum = pMsg->GetLong();

			DWORD flag = pMsg->GetDWord();

			DWORD dwIP = pMsg->GetIP();
			sprintf(strIP, "%d.%d.%d.%d", LOBYTE(LOWORD(dwIP)), HIBYTE(LOWORD(dwIP)), LOBYTE(HIWORD(dwIP)), HIBYTE(HIWORD(dwIP)));

			CGame::tagGameServer* gs = GetGame()->GetGameServer(pMsg->GetGSID());
			if(gs)
			{
				stPingGame.strIP = strIP;
				stPingGame.dwPort = gs->dwPort;
				gs->lPlayerNum = stPingGame.dwPlayerNum;
			}
			if(flag == MSG_L2W_SERVER_QUEST_PING)
			{
				GetGame()->AddPingGameServerInfo(stPingGame);

				if(GetGame()->GetPingGsNum() >= GetGame()->GetGSNum() && GetGame()->FinishPingDS())
				{
				CMessage msg(MSG_W2L_SERVER_ANSWER_PING);
				msg.Add((long)1);
				msg.SendToSocket(GetGame()->GetLSSocketID());
			}
		}
		}
		break;
	case MSG_S2W_SERVER_CREATEMONSTER:
		{
			CGUID lRegionID;
			pMsg -> GetGUID(lRegionID);
			
			pMsg -> SetType( MSG_W2S_SERVER_CREATEMONSTER );
			long lIndex = -1;
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(lRegionID);
			if(rgn)
				lIndex = rgn->GetGsid();
			GetGame() -> SendMsg2GameServer( lIndex, pMsg );
		}
		break;
	case MSG_S2W_SERVER_CREATEPHGOODS:
	case MSG_S2W_SERVER_UPDATEPHGOODS:
		{
			BYTE bType = pMsg->GetByte();
	
			CGUID playerID = NULL_GUID;

			CGUID guid;
			
			CGUID phRgnID;
			
			char name[64];
			
			long dir = 0;
			long width = 0;
			long height = 0;
			long posX = 0;
			long posY = 0;
			long templateID = 0;

			if(bType == 1) // ���ش���
			{
				pMsg->GetGUID(guid);
				pMsg->GetGUID(phRgnID);
				pMsg->GetStr(name, 64);
				dir = pMsg->GetLong();
				width = pMsg->GetLong();
				height = pMsg->GetLong();
				posX = pMsg->GetLong();
				posY = pMsg->GetLong();
				templateID = pMsg->GetLong();

				pMsg -> SetType( MSG_W2S_SERVER_CREATEPHGOODS );
				long lIndex = pMsg->GetGSID();
				CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(phRgnID);
				if(rgn)
					playerID = rgn->GetOwnerGUID();
			}
			else
			{
				pMsg->GetGUID(guid);
				pMsg->GetGUID(phRgnID);
				pMsg->GetStr(name, 64);
				dir = pMsg->GetLong();
				width = pMsg->GetLong();
				height = pMsg->GetLong();
				posX = pMsg->GetLong();
				posY = pMsg->GetLong();
				templateID = pMsg->GetLong();

				CGUID phRgnID = NULL_GUID;
				CPlayer* player = GetGame()->GetMapPlayer(phRgnID);
				if(player == NULL) return;

				phRgnID = player->GetPersonalHouseRgnGUID();

				pMsg -> SetType( MSG_W2S_SERVER_CREATEPHGOODS );
				long lIndex = -1;
				CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(phRgnID);
				if(rgn)
				{
					lIndex = rgn->GetGsid();
					playerID = rgn->GetOwnerGUID();
				}

				// �����ڷ�����Ϣ��GS�ϴ���goods
				GetGame() -> SendMsg2GameServer( lIndex, pMsg );
			}

			// ����PHGoods����
			CEntityGroup* dbDupRgn = NULL;
			CEntityManager::DBEntityComponentMapItr itr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
			if(itr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
			{
				if(itr->second.size() != 1)
				{
					AddLogText("��Ӹ��˷�����Ʒʱphrgngroup�������!");
					return;
				}
				
				map<CGUID, CBaseEntity*>::iterator rgnItr = itr->second.begin()->second->GetEntityGroupMap().find(phRgnID);
				if(rgnItr != itr->second.begin()->second->GetEntityGroupMap().end())
				{
					dbDupRgn = (CEntityGroup*)rgnItr->second;
				}
			}
			else
			{
				dbDupRgn = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgn]"), phRgnID);
				itr->second.begin()->second->AddChild(dbDupRgn);
			}

			if(dbDupRgn)
			{
				CEntity* rgnGoods = (CEntity*)GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgngoods]"), NULL_GUID);
				rgnGoods->SetGUID(guid);
				rgnGoods->SetGuidAttr(string("DupRgnGUID"),		dbDupRgn->GetGUID());
				rgnGoods->SetGuidAttr(string("guid"),			guid);
				rgnGoods->SetStringAttr(string("Name"),			name);
				rgnGoods->SetLongAttr(string("Dir"),			dir);
				rgnGoods->SetLongAttr(string("Width"),			width);
				rgnGoods->SetLongAttr(string("Height"),			height);
				rgnGoods->SetLongAttr(string("PosX"),			posX);
				rgnGoods->SetLongAttr(string("PosY"),			posY);
				if(dbDupRgn->GetEntityGroupMap().size() != 1)
				{
					AddLogText("dbrgn������ӽڵ����,���������ļ�!");
					return;
				}
				CEntityGroup* pRgnGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dbDupRgn, string("[phrgngoodsgroup]"));
				if(pRgnGoodsGroup)
					pRgnGoodsGroup->AddChild(rgnGoods);
			}
		}
		break;
	case MSG_S2W_SERVER_DELETEPHGOODS:
		{
			BYTE bType = pMsg->GetByte();
			CGUID lRegionID;
		
			CGUID guid;
	
			char name[64];
			pMsg->GetStr(name, 64);
			long dir = 0;
			long width = 0;
			long height = 0;
			long posX = 0;
			long posY = 0;
			
			pMsg -> GetGUID(lRegionID);
			pMsg->GetGUID(guid);
			pMsg->GetStr(name, 64);
			//dir = pMsg->GetLong();
			width = pMsg->GetLong();
			height = pMsg->GetLong();
			posX = pMsg->GetLong();
			posY = pMsg->GetLong();

			long lIndex = -1;
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(lRegionID);
			if(rgn)
				lIndex = rgn->GetGsid();

			// ��PHGoods������ɾ��
			bool isFindFlag = false;
			CEntityGroup* dbDupRgn = NULL;
			CEntityManager::DBEntityComponentMapItr itr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
			if(itr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
			{
				if(itr->second.size() == 0)
				{
					AddLogText("��Ӹ��˷�����Ʒʱδ�ҵ�phrgngroup����!");
					return;
				}
				map<CGUID, CEntityGroup*>::iterator rgnItr = itr->second.find(lRegionID);
				if(rgnItr != itr->second.end())
				{
					dbDupRgn = rgnItr->second;
				}
			}
			else
			{
				AddLogText("ɾ�����˷�����Ʒʱδ�ҵ�phrgn����!");
			}
			if(dbDupRgn)
			{
				if(dbDupRgn->GetEntityGroupMap().size() == 0)
				{
					AddLogText("ɾ�����˷�����Ʒʱdbrgn������ӽڵ�Ϊ��,���������ļ�!");
					return;
				}
				CEntityGroup* pRgnGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dbDupRgn, string("[phrgngoodsgroup]"));
				if(pRgnGoodsGroup)
					pRgnGoodsGroup->ReleaseLeaf(guid);
			}
			pMsg -> SetType( MSG_W2S_SERVER_DELETEPHGOODS );
			pMsg->SendGSID(lIndex);
		}
		break;
	case MSG_S2W_SERVER_SAVEPERSONALHOUSE:
		{
			long bufSize = pMsg->GetLong();
			BYTE *byteArray = (BYTE*)M_ALLOC(bufSize);
			pMsg->GetEx(byteArray, bufSize);

			long pos = 0;
			CGUID rgnGUID;
			_GetBufferFromByteArray(byteArray, pos, rgnGUID); // ������ID

			// �ҵ���Ӧ��GS��RgnManager
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(rgnGUID);
		
			long GSID = CMessage::GetGSIDBySocket(lSocketID);

			if(rgn)
			{	
				CGUID guid;
				_GetBufferFromByteArray(byteArray, pos, guid, bufSize);
				char name[128];
				memset(name, 0, sizeof(name));
				_GetStringFromByteArray(byteArray, pos, name, bufSize);
			
				CGUID ownerID;
				_GetBufferFromByteArray(byteArray, pos, ownerID, bufSize);
				long templateID = _GetLongFromByteArray(byteArray, pos, bufSize);

				long lResourceID = _GetLongFromByteArray(byteArray, pos, bufSize);

					// ���浽���ݿ�ı�������
					vector<BYTE> pVarBA;
					long varPos = pos;

					char strName[256];
					// ��ͨ��������
					long lVarNum = _GetLongFromByteArray(byteArray, pos, bufSize);		// ��������

					//GUID����
					long num = _GetLongFromByteArray( byteArray, pos, bufSize);		// ��������

					// buf size
					long tBufSize = _GetLongFromByteArray( byteArray, pos, bufSize); // ��������

					// ���浽���ݿ�ı�������
					for(int tVarPos=0; tVarPos<sizeof(long); tVarPos++)
					{
						pVarBA.push_back(((char*)(&lVarNum))[tVarPos]);
						varPos++;
					}
					for(int tVarPos=0; tVarPos<sizeof(long); tVarPos++)
					{
						pVarBA.push_back(((char*)(&num))[tVarPos]);
						varPos++;
					}
					for(int tVarPos=0; tVarPos<sizeof(long); tVarPos++)
					{
						pVarBA.push_back(((char*)(&tBufSize))[tVarPos]);
						varPos++;
					}
					for(int tVarPos=0; tVarPos<tBufSize; tVarPos++)
					{
						pVarBA.push_back(byteArray[varPos]);
						varPos++;
					}

					double value = 0;
					long lArray = 0;
					for(int i=0; i<lVarNum; i++)
					{
						_GetStringFromByteArray(byteArray, pos, strName, bufSize);
						lArray = _GetLongFromByteArray(byteArray, pos, bufSize);
						if( lArray == 0 )	// �������
						{
							value = _GetDoubleFromByteArray(byteArray, pos, bufSize);
							rgn->GetVariableList()->AddVar(strName, value);
						}
						else if(lArray > 0)	// ����
						{
							rgn->GetVariableList()->AddVar(strName, lArray, 0.0f);

							for(int j=0; j<lArray; j++)
							{
								value = _GetDoubleFromByteArray(byteArray, pos, bufSize);
								rgn->GetVariableList()->SetVarValue(strName, j, value);
							}
						}
						else
						{		
							//�ַ���
							char tmpString[1024];
							_GetStringFromByteArray(byteArray, pos, tmpString, bufSize);
							rgn->GetVariableList()->AddVar(strName, tmpString);
						}
					}

					// GUID
					char GuidName[256];
					CGUID tGuid;
					for(int i=0; i<num; i++)
					{
						_GetStringFromByteArray( byteArray, pos, GuidName, bufSize );	// ����	
						_GetBufferFromByteArray(byteArray, pos, tGuid, bufSize);	// GUID����
						rgn->GetVariableList()->AddGuid(GuidName, tGuid);
					}
				
					CEntityGroup* dupRgn = NULL;
					CEntityManager::DBEntityComponentMapItr phRgnMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
					if(phRgnMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
					{
						if(phRgnMapItr->second.size() == 0)
						{
							AddLogText("��Ӹ��˷�����Ʒʱδ�ҵ�phrgngroup����!");
							return;
						}
						map<CGUID, CEntityGroup*>::iterator rgnItr = phRgnMapItr->second.find(rgnGUID);
						if(rgnItr != phRgnMapItr->second.end())
						{
							dupRgn = rgnItr->second;
						}
						else
						{
							dupRgn = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgn]"), rgnGUID);
							GetGame()->GetEntityManager()->CPhRgnToCDBPhRgn(rgn, dupRgn);
							dupRgn->SetGuidAttr(string("OwnerGUID"),  ownerID);
							dupRgn->SetGuidAttr(string("guid"),  rgnGUID);
						}
					}
					else
					{
						AddLogText("ɾ�����˷�����Ʒʱδ�ҵ�phrgn����!");
					}

					if(dupRgn != NULL)
					{
						// ������Ʒ
						long goodsNum = _GetLongFromByteArray(byteArray, pos, bufSize);
						if (goodsNum != 0)
						{
							struct PHGoods
							{
								// ��ƷGUID
								CGUID guid;
								// ��Ʒ����
								char name[64];
								// ����
								int dir;
								// X����
								int width;
								// Y����
								int height;
								// ����
								float posX;
								float posY;
							};

							for(int i=0; i<goodsNum; i++)
							{
								CEntity* tdrGoods = (CEntity*)GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgngoods]"), NULL_GUID);
								PHGoods phGoods;
								_GetBufferFromByteArray(byteArray, pos, &phGoods, sizeof(PHGoods), bufSize);
								tdrGoods->SetGuidAttr(string("guid"),  phGoods.guid);
								tdrGoods->SetGuidAttr(string("DupRgnGUID"),  dupRgn->GetGUID());
								tdrGoods->SetStringAttr(string("Name"),  phGoods.name);
								tdrGoods->SetLongAttr(string("Dir"),  phGoods.dir);
								tdrGoods->SetLongAttr(string("Width"),  phGoods.width);
								tdrGoods->SetLongAttr(string("Height"),  phGoods.height);
								tdrGoods->SetLongAttr(string("PosX"),  phGoods.posX);
								tdrGoods->SetLongAttr(string("PosY"),  phGoods.posY);
								tdrGoods->SetGUID(phGoods.guid);
								
								CEntityGroup* pRgnGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dupRgn, string("[phrgngoodsgroup]"));
								if(pRgnGoodsGroup)
									pRgnGoodsGroup->AddChild(tdrGoods);
							}
						}
						dupRgn->SetLongAttr(string("GSID"),  GSID);
						
						char outStr[1024];
						sprintf(outStr, "---GS:SavePH() ���յ�%d��PH Goods��---", goodsNum);
						AddLogText(outStr);
					}
				
					GetGame()->GetEntityManager()->CreateSavePhRgnSession(dupRgn);
			}

			M_FREE(byteArray,bufSize);
		}
		break;
	case MSG_S2W_SERVER_LOADPERSONALHOUSE:
		{
			CGUID OwnerID;
			pMsg->GetGUID(OwnerID);
			long GSID = CMessage::GetGSIDBySocket(lSocketID);
//			GetGame()->SendLoadPersonalHouse(GSID, OwnerID);
		}
		break;
		// GS�Ľű����͵���Ϣ,��Ҫ��Ӧ����
	case MSG_S2W_SERVER_PLAYER_PERSONALHOUSEGUID:
		{
			long scriptID = pMsg->GetLong();// Script ID

			// player name
			char playerName[128];
			pMsg->GetStr(playerName, 128);

			// PH GUID
			CGUID guid;

			char szPHGuid[128]; // ���ص�PH GUID ������
			pMsg->GetStr(szPHGuid, 128);

			// �����ڴ��в���
			CPlayer* pPlayer = GetGame()->GetMapPlayer(playerName);
			if(pPlayer)// ��ͬ����
			{
				// ���ظ�GS
				CMessage msg(MSG_W2S_SERVER_PLAYER_PERSONALHOUSEGUID);
				msg.Add(scriptID);// Script ID
				msg.Add(playerName); // player name
				msg.Add(szPHGuid);// ���ص�PH GUID ������
				msg.Add(pPlayer->GetPersonalHouseRgnGUID());// // PH GUID
				msg.SendToSocket(lSocketID);
				break;
			}
		}
		break;
	}
}
