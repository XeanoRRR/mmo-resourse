
#include "stdafx.h"
#include "..\nets\netdb\message.h"
#include "..\nets\Servers.h"
#include "..\dbgood.h"
#include "..\..\dbserver\game.h"
#include "..\dbplayer.h"
//#include "StreamOperator.h"
#include "../../../../dbaccess/WorldDB/RsPlayer.h"
#include "../../../../dbaccess/WorldDB/RSGoods.h"
#include "../../../../dbaccess/WorldDB/RSCountry.h"
#include "../../../../dbaccess/WorldDB/RsCityWar.h"
#include "../../../../dbaccess/WorldDB/RsEnemyFactions.h"
#include "../../../../dbaccess/WorldDB/RsFaction.h"
#include "../../../../dbaccess/WorldDB/RsRegion.h"
#include "../../../../dbaccess/WorldDB/RsUnion.h"
#include "../../../../dbaccess/WorldDB/RsVillageWar.h"
#include"../../../../DBAccess/worlddb/RScards.h"
#include "..\DBentityManager.h"
#include "..\dbFaction.h"
#include "..\dbUnion.h"
#include "..\dbCityWar.h"
#include "..\dbVillageWar.h"
#include "..\dbCountry.h"
#include "..\dbRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnDBSaveMessage(CMessage* pMsg)
{
	size_t nSavePlayerCnt = 0, 
		nCreationCnt = 0,
		nRestoreCnt = 0,
		nDeletionCnt = 0;
	long nTransactionCounter = 0;
	_ConnectionPtr cnSave;

	switch(pMsg->GetType())
	{
	case MSG_W2DB_SAVE_CREATE_PLAYER:                    // ���洴���Ľ�ɫ
		{
	/*		BYTE Data[102400];
			pMsg->GetEx(Data,102400);

			long lPos = 0;
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
			long msgTimerID = _GetLongFromByteArray(Data, lPos);
			
			long createFlag = 0; // 0:ʧ��, 1:�ɹ�
			CMyAdoBase::CreateCn(cnSave);

			// ����һ����������
			if(!CMyAdoBase::OpenCn(cnSave)) 
				AddLogText("�������ݿ�ʧ��");

			CDBPlayer *pPlayer = new CDBPlayer;
			pPlayer->DecordFromByteArray(Data, lPos);
			if(pPlayer)
			{
				try
				{
					nTransactionCounter = 0; 
					nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

					if( GetGame()->GetRsPlayer()->CreatePlayer(pPlayer, cnSave) )
					{
						ShowSaveInfo("�𴴽���ɫ�ɹ���%s.%s.%d",pPlayer->GetAccount(), pPlayer->GetName(), pPlayer->GetID());
						createFlag = 1;
						// ���̳ɹ�����DBDATE��ɾ��
						//EnterCriticalSection(&g_CriticalSectionSavePlayerList);
						//CDBPlayer *pPlayer = (pPlayer);
						//SAFE_DELETE(pPlayer);
						//it = pCreationPlayer->erase(it);
						//LeaveCriticalSection(&g_CriticalSectionSavePlayerList);

						cnSave->CommitTrans();		// �����ύ
					}
					else 
					{
						AddLogText("�񴴽���ɫʧ�ܣ�%s.%s.%d",pPlayer->GetAccount(), pPlayer->GetName(), pPlayer->GetID() );
						//++it;
						cnSave->RollbackTrans(); //ʧ�ܻع�
						createFlag = 0;
					}
				}
				catch(...) 
				{
					if(nTransactionCounter)
					{
						cnSave->RollbackTrans(); //ʧ�ܻع�
					}
					AddLogText("��������ɫ�쳣");
					createFlag = 0;
					//++it;
				}
			}

			CMessage msg(MSG_DB2W_SAVE_CREATE_PLAYER);
			msg.Add(msgTimerID);
			msg.Add(createFlag);
			SAFE_DELETE(pPlayer);*/
		}
		break;
	case MSG_W2DB_SAVE_DELETE_PLAYER:                  // ����ɾ���Ľ�ɫ
		{
			
		}
		break;

	case MSG_S2DB_SAVE_PLAYER://�����������
	case MSG_W2DB_SAVE_PLAYER://�����������
		{
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
	/*		BYTE Data[10240];
			memset(Data, 0, sizeof(Data));
			pMsg->GetEx(Data, 10240);

			long pos = 0;
			long msgTimerID = _GetLongFromByteArray(Data, pos);
			CMyAdoBase::CreateCn(cnSave);

			// ����һ����������
			if(!CMyAdoBase::OpenCn(cnSave)) 
				AddLogText("�������ݿ�ʧ��");
			else
			{
				//---------------- ����Player
				CDBPlayer* player = new CDBPlayer;
	
				player->DecordFromByteArray(Data, pos);
			
				AddLogText("����Player��ʼ...");
				try
				{
					nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

					if( GetGame()->GetRsPlayer()->SavePlayer(player, cnSave))
					{
						cnSave->CommitTrans();		// �����ύ
						ShowSaveInfo("�𱣴�Player�ɹ�");
					}
					else
					{
						cnSave->RollbackTrans(); //ʧ�ܻع�
						AddLogText("�񱣴�Playerʧ��");
					}
				}
				catch(...) 
				{
					if(nTransactionCounter)
					{
						cnSave->RollbackTrans(); //ʧ�ܻع�
					}
					AddLogText("������Player�쳣");
				}

				SAFE_DELETE(player);
			}

			CMessage msg(MSG_DB2W_SAVE_PLAYER);
			msg.Add(msgTimerID);
			msg.SendToSocket(pMsg->GetSocketID());*/
		}
		break;
	case MSG_W2DB_SAVE_COUNTRY://�����������
		{
	/*		BYTE buf[102400];
			pMsg->GetEx(buf, 102400);

			long pos = 0;
			long msgid = _GetLongFromByteArray(buf, pos); // DBMSGID

			CCountry* pCountry = new CCountry;
			if(pCountry)
			{
				pCountry->DecordFromByteArray(buf, pos);

				CMyAdoBase::CreateCn(cnSave);

				// ����һ����������
				if(!CMyAdoBase::OpenCn(cnSave)) 
					AddLogText("�������ݿ�ʧ��");
				else
				{
					AddLogText("����Country��ʼ...");
					try
					{
						nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

						if( GetGame()->GetDBCountry()->Save(pCountry, cnSave))
						{
							cnSave->CommitTrans();		// �����ύ
							ShowSaveInfo("�𱣴�Country�ɹ�");
						}
						else
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
							AddLogText("�񱣴�Countryʧ��");
						}
					}
					catch(...) 
					{
						if(nTransactionCounter)
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
						}
						AddLogText("������Country�쳣");
					}
				}
			}

			
			SAFE_DELETE(pCountry);

			// ��Ӧ��Ϣ
			CMessage msg(MSG_DB2W_SAVE_COUNTRY);
			msg.Add(msgid);// DBMSGID
			msg.SendToSocket(pMsg->GetSocketID());*/
		}
		break;
	case MSG_W2DB_SAVE_CARDS_LARGESS://�������Ϳ�Ƭ����
		{
	/*		CCard* card = new CCard;
			if(card)
			{
				char CDKey[64];
				memset(CDKey, 0, sizeof(CDKey));

				pMsg->GetStr(CDKey, 64);
				card->SetBuyerAccount(CDKey); // Cdkey
				card->SetPrice(pMsg->GetLong()); // SendNum
		
				if( GetGame()->GetDBCards()->SaveLargess(card))
				{
					cnSave->CommitTrans();		// �����ύ
					ShowSaveInfo("�𱣴�CardLargess�ɹ�");
				}
				else
				{
					cnSave->RollbackTrans(); //ʧ�ܻع�
					AddLogText("�񱣴�CardLargessʧ��");
				}			
			}

			long msgid = pMsg->GetLong(); // DBMSGID
			SAFE_DELETE(card);

			// ��Ӧ��Ϣ
			CMessage msg(MSG_DB2W_SAVE_CARDS_LARGESS);
			msg.Add(msgid);// DBMSGID
			msg.SendToSocket(pMsg->GetSocketID());*/
		}
		break;
	case MSG_W2DB_SAVE_ALL_CITY_WAR_PARAM:
		{
	/*		list<tagAttackCityTime*> actime;
			//if(actime)
			{
				long msgID    = pMsg->GetLong(); // MSGID
				long paramNum = pMsg->GetLong();// Ҫ����ó�ս��������
				long decWarFactionNum = 0; // ��������б�

				for(int i = 0; i < paramNum; i++)
				{
					tagAttackCityTime* pSetup = new tagAttackCityTime;
					pSetup->DecWarFactions.clear();
					pSetup->lCityRegionID = pMsg->GetLong();
					pSetup->AttackCityStartTime.wYear = pMsg->GetWord();
					pSetup->AttackCityStartTime.wMonth = pMsg->GetWord();
					pSetup->AttackCityStartTime.wDay = pMsg->GetWord();
					pSetup->AttackCityStartTime.wHour = pMsg->GetWord();
					pSetup->AttackCityStartTime.wMinute = pMsg->GetWord();
					pSetup->AttackCityStartTime.wSecond = pMsg->GetWord();
					// ��������б�
					decWarFactionNum = pMsg->GetLong();
					
					for(int j = 0; j < decWarFactionNum; j++)
					{
						pSetup->DecWarFactions.push_back(pMsg->GetLong());
					}

					actime.push_back(pSetup);
				}

				CMyAdoBase::CreateCn(cnSave);

				// ����һ����������
				if(!CMyAdoBase::OpenCn(cnSave)) 
					AddLogText("�������ݿ�ʧ��");
				else
				{
					AddLogText("����CityWarParam��ʼ...");
					try
					{
						nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

						if( GetGame()->GetRsCityWar()->SaveAllCityWarParam(actime, cnSave) )
						{
							cnSave->CommitTrans();		// �����ύ
							ShowSaveInfo("�𱣴�CityWarParam�ɹ�");
						}
						else
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
							AddLogText("�񱣴�CityWarParamʧ��");
						}
					}
					catch(...) 
					{
						if(nTransactionCounter)
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
						}
						AddLogText("������CityWarParam�쳣");
					}
				}
			}

			long msgid = pMsg->GetLong(); // DBMSGID
			list<tagAttackCityTime*>::iterator itr = actime.begin();
			for(; itr != actime.end(); itr++)
			{
				if(*itr)
					SAFE_DELETE(*itr);
			}
			
			// ��Ӧ��Ϣ
			CMessage msg(MSG_DB2W_SAVE_ALL_CITY_WAR_PARAM);
			msg.Add(msgid);// DBMSGID
			msg.SendToSocket(pMsg->GetSocketID());*/
		}
		break;
	case MSG_W2DB_SAVE_ALL_VILLAGE_WAR_PARAM:
		{
	/*		list<tagVilWarSetup*> actime;
			//if(actime)
			{
				long msgID    = pMsg->GetLong(); // MSGID
				long paramNum = pMsg->GetLong();// Ҫ����ó�ս��������
				long decWarFactionNum = 0; // ��������б�

				for(int i = 0; i < paramNum; i++)
				{
					tagVilWarSetup* pSetup = new tagVilWarSetup;
					pSetup->DecWarFactions.clear();
					pSetup->lWarRegionID = pMsg->GetLong();
					pSetup->WarStartTime.wYear = pMsg->GetWord();
					pSetup->WarStartTime.wMonth = pMsg->GetWord();
					pSetup->WarStartTime.wDay = pMsg->GetWord();
					pSetup->WarStartTime.wHour = pMsg->GetWord();
					pSetup->WarStartTime.wMinute = pMsg->GetWord();
					pSetup->WarStartTime.wSecond = pMsg->GetWord();
					// ��������б�
					decWarFactionNum = pMsg->GetLong();

					for(int j = 0; j < decWarFactionNum; j++)
					{
						pSetup->DecWarFactions.push_back(pMsg->GetLong());
					}

					actime.push_back(pSetup);
				}

				CMyAdoBase::CreateCn(cnSave);

				// ����һ����������
				if(!CMyAdoBase::OpenCn(cnSave)) 
					AddLogText("�������ݿ�ʧ��");
				else
				{
					AddLogText("����VilWarParam��ʼ...");
					try
					{
						nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

						if( GetGame()->GetRsVillageWar()->SaveAllVillageWarParam(actime, cnSave) )
						{
							cnSave->CommitTrans();		// �����ύ
							ShowSaveInfo("�𱣴�VilWarParam�ɹ�");
						}
						else
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
							AddLogText("�񱣴�VilWarParamʧ��");
						}
					}
					catch(...) 
					{
						if(nTransactionCounter)
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
						}
						AddLogText("������VilWarParam�쳣");
					}
				}
			}

			long msgid = pMsg->GetLong(); // DBMSGID
			list<tagVilWarSetup*>::iterator itr = actime.begin();
			for(; itr != actime.end(); itr++)
			{
				if(*itr)
					SAFE_DELETE(*itr);
			}

			// ��Ӧ��Ϣ
			CMessage msg(MSG_DB2W_SAVE_ALL_VILLAGE_WAR_PARAM);
			msg.Add(msgid);// DBMSGID
			msg.SendToSocket(pMsg->GetSocketID());*/
		}
		break;
	case MSG_W2DB_SAVE_ALL_ENEMY_FACTIONS:
		{
	/*		list<tagEnemyFaction*> actime;
				long msgid    = pMsg->GetLong(); // MSGID
				long paramNum = pMsg->GetLong();// Ҫ����ó�ս��������
				long decWarFactionNum = 0; // ��������б�

				for(int i = 0; i < paramNum; i++)
				{
					tagEnemyFaction* pSetup = new tagEnemyFaction;
					pSetup->lFactionID1 = pMsg->GetLong();
					pSetup->lFactionID2 = pMsg->GetLong();
					pSetup->dwDisandTime = pMsg->GetDWord();
					actime.push_back(pSetup);
				}

				CMyAdoBase::CreateCn(cnSave);

				// ����һ����������
				if(!CMyAdoBase::OpenCn(cnSave)) 
					AddLogText("�������ݿ�ʧ��");
				else
				{
					AddLogText("����EnemyFactions��ʼ...");
					try
					{
						nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

						if( GetGame()->GetRsEnemyFactions()->SaveAllEnemyFactions(actime, cnSave) )
						{
							cnSave->CommitTrans();		// �����ύ
							ShowSaveInfo("�𱣴�EnemyFactions�ɹ�");
						}
						else
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
							AddLogText("�񱣴�EnemyFactionsʧ��");
						}
					}
					catch(...) 
					{
						if(nTransactionCounter)
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
						}
						AddLogText("������EnemyFactions�쳣");
					}
				}

			list<tagEnemyFaction*>::iterator itr = actime.begin();
			for(; itr != actime.end(); itr++)
			{
				if(*itr)
					SAFE_DELETE(*itr);
			}

			// ��Ӧ��Ϣ
			CMessage msg(MSG_DB2W_SAVE_ALL_ENEMY_FACTIONS);
			msg.Add(msgid);// DBMSGID
			msg.SendToSocket(pMsg->GetSocketID());*/
		}
		break;
	case MSG_W2DB_SAVE_REGION_PARAM:
		{
	/*		BYTE buf[102400];
			pMsg->GetEx(buf, 102400);

			long pos = 0;
			long msgid = _GetLongFromByteArray(buf, pos);

			CWorldRegion* pRegion = new CWorldRegion;

			if(pRegion)
			{
				pRegion->DecordFromByteArray(buf, pos);
				
				CMyAdoBase::CreateCn(cnSave);

				// ����һ����������
				if(!CMyAdoBase::OpenCn(cnSave)) 
					AddLogText("�������ݿ�ʧ��");
				else
				{
					AddLogText("����RegionParam��ʼ...");
					try
					{
						nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

						if( GetGame()->GetRsRegion()->Save(pRegion, cnSave) )
						{
							cnSave->CommitTrans();		// �����ύ
							ShowSaveInfo("�𱣴�RegionParam�ɹ�");
						}
						else
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
							AddLogText("�񱣴�RegionParamʧ��");
						}
					}
					catch(...) 
					{
						if(nTransactionCounter)
						{
							cnSave->RollbackTrans(); //ʧ�ܻع�
						}
						AddLogText("������RegionParam�쳣");
					}
				}
			}

			SAFE_DELETE(pRegion);
			
			// ��Ӧ��Ϣ
			CMessage msg(MSG_DB2W_SAVE_REGION_PARAM);
			msg.Add(msgid);// DBMSGID
			msg.SendToSocket(pMsg->GetSocketID());*/
		}
		break;
	case MSG_S2DB_SAVE_CONFEDERATION:
	case MSG_W2DB_SAVE_CONFEDERATION:
		{
	/*		long msgid = pMsg->GetLong();

			//�����˵�ȫ��Ψһ��ʶ4
			long lID = pMsg->GetLong();
			//��������16
			char strName[16] = {0};
			pMsg->GetStr(strName, 15);
			//�������ID4
			long lMastterID = pMsg->GetLong();	

			CDBUnion* pConf = new CDBUnion(lID, lMastterID, string(strName));

			pConf->InitByWSMsg(pMsg);

			CMyAdoBase::CreateCn(cnSave);

			// ����һ����������
			if(!CMyAdoBase::OpenCn(cnSave)) 
				AddLogText("�������ݿ�ʧ��");
			else
			{
				AddLogText("����Confederation��ʼ...");
				try
				{
					nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

					if( GetGame()->GetRsConfederation()->SaveConfederation(pConf, cnSave) )
					{
						cnSave->CommitTrans();		// �����ύ
						ShowSaveInfo("�𱣴�Confederation�ɹ�");
					}
					else
					{
						cnSave->RollbackTrans(); //ʧ�ܻع�
						AddLogText("�񱣴�Confederationʧ��");
					}
				}
				catch(...) 
				{
					if(nTransactionCounter)
					{
						cnSave->RollbackTrans(); //ʧ�ܻع�
					}
					AddLogText("������Confederation�쳣");
				}
			}

			// ��Ӧ��Ϣ
			if(pMsg->GetType() == MSG_W2DB_SAVE_CONFEDERATION)
			{
				CMessage msg(MSG_DB2W_SAVE_CONFEDERATION);
				msg.Add(msgid);// DBMSGID
				msg.SendToSocket(pMsg->GetSocketID());
			}
			else if(pMsg->GetType() == MSG_S2DB_SAVE_CONFEDERATION)
			{
				CMessage msg(MSG_DB2S_SAVE_CONFEDERATION);
				msg.Add(msgid);// DBMSGID
				msg.SendToSocket(pMsg->GetSocketID());
			}*/
		}
		break;

	case MSG_S2DB_SAVE_FACTION:
	case MSG_W2DB_SAVE_FACTION:
		{
	/*		long msgid = pMsg->GetLong(); // MSGID
			//�ð��ȫ��Ψһ��ʶ4
			long lID = pMsg->GetLong();
			//�������16
			char strName[16] = {0};
			pMsg->GetStr(strName, 15);
			//����ID4
			long lMastterID = pMsg->GetLong();	
			//�ð�����ʱ��32
			tagTime EstablishedTime;
			pMsg->GetEx(&EstablishedTime, sizeof(tagTime));

			CDBFaction* pFaction = new CDBFaction(lID,lMastterID,EstablishedTime,string(strName));
			
			pFaction->InitByWSMsg(pMsg);
			
			CMyAdoBase::CreateCn(cnSave);

			// ����һ����������
			if(!CMyAdoBase::OpenCn(cnSave)) 
				AddLogText("�������ݿ�ʧ��");
			else
			{
				AddLogText("����Faction��ʼ...");
				try
				{
					nTransactionCounter = cnSave->BeginTrans();	// ����ʼ

					if( GetGame()->GetRsFaction()->SaveFaction(pFaction, cnSave) )
					{
						cnSave->CommitTrans();		// �����ύ
						ShowSaveInfo("�𱣴�Faction�ɹ�");
					}
					else
					{
						cnSave->RollbackTrans(); //ʧ�ܻع�
						AddLogText("�񱣴�Factionʧ��");
					}
				}
				catch(...) 
				{
					if(nTransactionCounter)
					{
						cnSave->RollbackTrans(); //ʧ�ܻع�
					}
					AddLogText("������Faction�쳣");
				}
			}
	
			SAFE_DELETE(pFaction);

			// ��Ӧ��Ϣ
			if(pMsg->GetType() == MSG_W2DB_SAVE_FACTION)
			{
				CMessage msg(MSG_DB2W_SAVE_FACTION);
				msg.Add(msgid);// DBMSGID
				msg.SendToSocket(pMsg->GetSocketID());
			}
			else if(pMsg->GetType() == MSG_S2DB_SAVE_FACTION)
			{
				CMessage msg(MSG_DB2S_SAVE_FACTION);
				msg.Add(msgid);// DBMSGID
				msg.SendToSocket(pMsg->GetSocketID());
			}*/
		}
		break;
	}
}
