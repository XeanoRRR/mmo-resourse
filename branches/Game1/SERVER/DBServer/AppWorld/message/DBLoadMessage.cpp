
#include "stdafx.h"
#include "..\nets\netdb\message.h"
#include "..\nets\Servers.h"
#include "..\dbgood.h"
#include "..\..\dbserver\game.h"
#include "..\dbplayer.h"
#include "../../../../dbaccess/WorldDB/RsPlayer.h"
#include "../../../../dbaccess/WorldDB/RSGoods.h"
#include "../../../../dbaccess/WorldDB/RSCountry.h"
#include "../../../../dbaccess/WorldDB/RsCityWar.h"
#include "../../../../dbaccess/WorldDB/RsEnemyFactions.h"
#include "../../../../dbaccess/WorldDB/RsFaction.h"
#include "../../../../dbaccess/WorldDB/RsRegion.h"
#include "../../../../dbaccess/WorldDB/RsUnion.h"
#include "../../../../dbaccess/WorldDB/RsVillageWar.h"
#include "..\dbFaction.h"
#include "..\dbUnion.h"
#include "..\dbCityWar.h"
#include "..\dbVillageWar.h"
#include "..\DBentityManager.h"
//#include "StreamOperator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

long LoadPlayer(CMessage* msg, CDBPlayer* player, _ConnectionPtr cnLoad)
{
	long nTransactionCounter = 0;
	try
	{
		nTransactionCounter = cnLoad->BeginTrans();	// ����ʼ

		char szPlayerID[128];
		player->GetExID().tostring(szPlayerID);
		ShowSaveInfo("��ʼ��ȡPlayer, [ID]:%s", szPlayerID);
		if( GetGame()->GetRsPlayer()->LoadPlayer(player, cnLoad))
		{
			player->SetEnergy(player->GetEnergy ());
			//SetGraphicsID(GetBaseProperty()->lOccupation*2+GetBaseProperty()->lSex+1);
			player->SetGraphicsID( player->GetOccupation() * 2 + player->GetSex()+1);

			//���ø���ҵ���֯��ϵ
			//GetOrganiCtrl()->SetPlayerOrganizing(this);

			//��������ٶ�
			//player->SetSpeed(CGlobeSetup::GetSetup()->fPlayerSpeed);

			cnLoad->CommitTrans();		// �����ύ
			ShowSaveInfo("���ȡPlayer�ɹ�");
//			player->Load(msg);
			//_AddToByteArray(&ByteData, (long)player->GetID());
			//player->AddToByteArray(&ByteData);
		}
		else
		{
			cnLoad->RollbackTrans(); //ʧ�ܻع�
			AddLogText("���ȡPlayerʧ��");
		}
	}
	catch(...) 
	{
		if(nTransactionCounter)
		{
			cnLoad->RollbackTrans(); //ʧ�ܻع�
		}
		AddLogText("����ȡPlayerBase�쳣");
	}

	return nTransactionCounter;
}
void OnDBLoadMessage(CMessage* pMsg)
{
	size_t nSavePlayerCnt = 0, 
		nCreationCnt = 0,
		nRestoreCnt = 0,
		nDeletionCnt = 0;
	long nTransactionCounter = 0;
	_ConnectionPtr cnLoad;

	switch(pMsg->GetType())
	{
	case MSG_W2DB_LOAD_REGION_PARAM:
		{
/*		//	CWorldRegion* wr = new CWorldRegion;
		//	if(wr)
			{
				// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
				long msgTimerID = pMsg->GetLong();

				wr->SetID(pMsg->GetLong());
				GetGame()->GetRsRegion()->LoadRegionParam(wr);
				tagRegionParam& pa = wr->GetRegionParam();

				CMessage msg(MSG_DB2W_LOAD_REGION_PARAM);
				msg.Add(msgTimerID);           // MsgID
				msg.Add(wr->GetID());         // RegionID
				msg.Add(pa.lOwnedFactionID); //OwnedFactionID
				msg.Add(pa.lOwnedUnionID); //OwnedUnionID
				msg.Add(pa.lCurrentTaxRate); //CurTaxRate
				msg.Add(pa.dwTodayTotalTax); //TodayTotalTax
				msg.Add(pa.dwTotalTax); //TotalTax
				msg.SendToSocket(pMsg->GetSocketID());

				ShowSaveInfo("����REGION_PARAM������Ϣ...OK!");
			}
			delete wr;*/
		}
		break;
	case MSG_W2DB_LOAD_PLAYER_WORLD_ID:
		{
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
/*			long msgTimerID = pMsg->GetLong();

			long playerID = GetGame()->GetDBPlayerID();
			long worldID  = GetGame()->GetDBLeaveWordID();
			CMessage msg(MSG_DB2W_LOAD_PLAYER_WORLD_ID);
			msg.Add(msgTimerID); // MsgID
			msg.Add(playerID);
			msg.Add(worldID);
			msg.SendToSocket(pMsg->GetSocketID());*/

			ShowSaveInfo("����PLAYER_WORLD_ID������Ϣ...OK!");
		}
		break;
	case MSG_W2DB_LOAD_INIT_ENEMY_FACTION:
		{
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
/*			long msgTimerID = pMsg->GetLong();

			GetGame()->GetRsEnemyFactions()->LoadAllEnemyFactions();
			CMessage msg(MSG_DB2W_LOAD_INIT_ENEMY_FACTION);
			msg.Add(msgTimerID);
			list<tagEnemyFaction>& pList = GetFactionWarSys()->GetEnemyFactionList();
			if(pList.size() != 0)
			{
				msg.Add((long)pList.size());

				list<tagEnemyFaction>::iterator itr = pList.begin();
				for(; itr != pList.end(); itr++)
				{
					msg.Add((long)itr->lFactionID1);
					msg.Add((long)itr->lFactionID2);
					msg.Add((long)itr->dwDisandTime);
				}

				ShowSaveInfo("����ENEMY_FACTION������Ϣ...OK!");
			}
			else
			{
				msg.Add((long)0);
			}
			msg.SendToSocket(pMsg->GetSocketID());	*/	
		}
		break;
	case MSG_W2DB_LOAD_INIT_COUNTRY:
		{
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
/*			long msgTimerID = pMsg->GetLong();

			GetGame()->GetDBCountry()->Load();
			vector<BYTE> pBA;
			_AddToByteArray(&pBA, msgTimerID); // MsgID
			GetCountryHandler()->AddToByteArray(&pBA);
			CMessage msg(MSG_DB2W_LOAD_INIT_COUNTRY);
			msg.AddEx(&pBA[0], pBA.size());
			msg.SendToSocket(pMsg->GetSocketID());

			ShowSaveInfo("����COUNTRY������Ϣ...OK!");*/
		}
		break;
	case MSG_W2DB_LOAD_PLAYER:
	case MSG_S2DB_LOAD_PLAYER://��ȡ�������
		{
/*			CMyAdoBase::CreateCn(cnLoad);

			// ����һ����������
			if(!CMyAdoBase::OpenCn(cnLoad)) 
				AddLogText("�������ݿ�ʧ��");
			else
			{
				//---------------- ��ȡPlayer
				long msgid = pMsg->GetLong();
				long id = pMsg->GetLong();
				CDBPlayer* player = new CDBPlayer;
				player->SetID(id);
				//g_StreamOperator.StreamToPlayer(player, &Data[4]); // ����Type�ֶ�

				AddLogText("����Player������Ϣ...OK��");
				vector<BYTE> ByteData;
			    _AddToByteArray(&ByteData, msgid); // MSGID
				nTransactionCounter = LoadPlayer(ByteData, player, cnLoad);

				// send player data
				if(MSG_S2DB_LOAD_PLAYER == pMsg->GetType())
				{
					CMessage msg(MSG_DB2S_LOAD_PLAYER);
					msg.AddEx(&ByteData[0], ByteData.size());
					msg.SendToSocket(pMsg->GetSocketID()); //GameServer ID
				}
				else if(MSG_W2DB_LOAD_PLAYER == pMsg->GetType())
				{
					CMessage msg(MSG_DB2W_LOAD_PLAYER);
					msg.AddEx(&ByteData[0], ByteData.size());
					msg.SendToSocket(pMsg->GetSocketID()); //GameServer ID
				}
				
				delete player;
			}*/
		}
		break;
	case MSG_W2DB_LOAD_COUNTRY:
		{
/*			long msgid = pMsg->GetLong();
			GetGame()->GetDBCountry()->Load();
			map<BYTE,CCountry*>& country = GetCountryHandler()->GetCountryMap();
			map<BYTE,CCountry*>::iterator itr = country.begin();
			CMessage msg(MSG_DB2W_LOAD_COUNTRY);
			msg.Add(msgid); // MSGID
			msg.Add((long)country.size()); // country Nums
			for(; itr != country.end(); itr++)
			{
				msg.Add((*itr).second->GetID());
				msg.Add((*itr).second->GetPower());
				msg.Add((*itr).second->GetKing());
				msg.Add((*itr).second->GetTechLevel());
			}
			msg.SendToSocket(pMsg->GetSocketID()); //GameServer ID

			AddLogText("����Country������Ϣ...OK��");*/
		}
		break;
	case MSG_W2DB_LOAD_CARDS_LARGESS:
		{
/*			long msgid = pMsg->GetLong();
			CMessage msg(MSG_DB2W_LOAD_CARDS_LARGESS);
			msg.Add(msgid); // MSGID
			CCardsHandler::getInstance()->Initialize();
			map<string,CCard*>& cardMap = CCardsHandler::getInstance()->GetCardsMap();
			map<string,CCard*>::iterator itr = cardMap.begin();
			msg.Add((long)cardMap.size());
			for(; itr != cardMap.end(); itr++)
			{
				msg.Add((*itr).second->GetCardNumber());
				msg.Add((*itr).second->GetCardType());
				msg.Add((*itr).second->GetSellerAccount());
				msg.Add((*itr).second->GetSellerID());
				msg.Add((*itr).second->GetBuyerAccount());
				msg.Add((*itr).second->GetBuyerID());
				msg.Add((*itr).second->GetPrice());
				msg.Add((*itr).second->GetChargePrice());
				msg.Add((long)(*itr).second->GetCardState());
				msg.Add((long)(*itr).second->GetCardTime());
				msg.Add((*itr).second->GetStartTime());
				msg.Add((*itr).second->GetSerialNumber());
			}
			msg.SendToSocket(pMsg->GetSocketID()); //GameServer ID
			AddLogText("����cards largess������Ϣ...OK��");*/
		}
		break;
	case MSG_W2DB_LOAD_ALL_CITY_WAR_PARAM:
		{
	/*		long msgid = pMsg->GetLong();
			GetGame()->GetRsCityWar()->LoadAllCityWarParam();

			CMessage msg(MSG_DB2W_LOAD_ALL_CITY_WAR_PARAM);
			vector<BYTE> pBA;
			_AddToByteArray(&pBA, msgid);

			msg.Add((long)GetAttackCitySys()->GetAttackMap().size());

			map<long,tagACT>::iterator it = GetAttackCitySys()->GetAttackMap().begin();
			
			for(; it != GetAttackCitySys()->GetAttackMap().end(); it++)
			{
				_AddToByteArray(&pBA, (*it).second.lCityRegionID);
				_AddToByteArray(&pBA, &(*it).second.AttackCityStartTime, sizeof(tagTime));
	
				list<long>& warFactions = (*it).second.DecWarFactions;
				_AddToByteArray(&pBA, (long)warFactions.size());
				list<long>::iterator itr = warFactions.begin();
				for(; itr != warFactions.end(); itr++)
				{
					 _AddToByteArray(&pBA, *itr);
				}
			}

			msg.SendToSocket(pMsg->GetSocketID()); //ID
			AddLogText("����city war param������Ϣ...OK��");*/
		}
		break;
	case MSG_W2DB_LOAD_ALL_VILLAGE_WAR_PARAM:
		{
	/*		long msgid = pMsg->GetLong();
			GetGame()->GetRsVillageWar()->LoadAllVillageWarParam();
			CMessage msg(MSG_DB2W_LOAD_ALL_VILLAGE_WAR_PARAM);
			vector<BYTE> pBA;
			_AddToByteArray(&pBA, msgid);

			msg.Add((long)GetVilWarSys()->GetVilWarsMap().size());
	
			CVillageWarSys::itVelWar it = GetVilWarSys()->GetVilWarsMap().begin();

			for(; it != GetVilWarSys()->GetVilWarsMap().end(); it++)
			{
				_AddToByteArray(&pBA, (*it).second.lWarRegionID);
				_AddToByteArray(&pBA, &(*it).second.WarStartTime, sizeof(tagTime));
		
				list<long>& warFactions = (*it).second.DecWarFactions;
				_AddToByteArray(&pBA, (long)warFactions.size());
				list<long>::iterator itr = warFactions.begin();
				for(; itr != warFactions.end(); itr++)
				{
					_AddToByteArray(&pBA, *itr);
				}
			}

			msg.SendToSocket(pMsg->GetSocketID()); //ID
			AddLogText("����village war param������Ϣ...OK��");*/
		}
		break;
	case MSG_W2DB_LOAD_ALL_ENEMY_FACTIONS:
		{
	/*		long msgid = pMsg->GetLong();
			GetGame()->GetRsEnemyFactions()->LoadAllEnemyFactions();
			CMessage msg(MSG_DB2W_LOAD_ALL_ENEMY_FACTIONS);
			msg.Add(msgid);
			
			list<tagEnemyFaction>& tlist = GetFactionWarSys()->GetEnemyFactionList();
			msg.Add((long)tlist.size());

			list<tagEnemyFaction>::iterator itr = tlist.begin();
			for(; itr != tlist.end(); itr++)
			{
				msg.Add((*itr).lFactionID1);
				msg.Add((*itr).lFactionID2);
				msg.Add((*itr).dwDisandTime);
			}
			msg.SendToSocket(pMsg->GetSocketID()); //ID
			AddLogText("����enemy factions������Ϣ...OK��");*/
		}
		break;
	case MSG_W2DB_LOAD_ALL_FACTIONS:
		{
	/*		long msgid = pMsg->GetLong();
			
			GetGame()->GetRsFaction()->LoadAllFaction();
			map<long,COrganizing*>& facMap = GetOrganiCtrl()->GetFactionMap();
			map<long,COrganizing*>::iterator itr = facMap.begin();
			for(; itr != facMap.end(); itr++)
			{
				CMessage msg(MSG_DB2W_LOAD_ALL_FACTIONS);
				msg.Add(msgid);
				msg.Add((long)facMap.size()); // �������
				((CDBFaction*)(*itr).second)->PushBackToMsg(msg);
				msg.SendToSocket(pMsg->GetSocketID());
			}
			AddLogText("����all factions������Ϣ...OK��");*/
		}
		break;
	case MSG_W2DB_LOAD_ALL_CONFEDERATIONS:
		{			
		/*	long msgid = pMsg->GetLong();

			GetGame()->GetRsConfederation()->LoadAllConfederation();
			map<long,COrganizing*>& facMap = GetOrganiCtrl()->GetUnionMap();
			map<long,COrganizing*>::iterator itr = facMap.begin();
			for(; itr != facMap.end(); itr++)
			{
				CMessage msg(MSG_DB2W_LOAD_ALL_CONFEDERATIONS);
				msg.Add(msgid);
				msg.Add((long)facMap.size()); // �������
				((CDBUnion*)(*itr).second)->PushBackToMsg(msg);
				msg.SendToSocket(pMsg->GetSocketID());
			}
			AddLogText("����all unions������Ϣ...OK��");*/
		}
		break;
	case MSG_S2DB_LOAD_FACTION:
	case MSG_W2DB_LOAD_FACTION:
		{
	/*		long msgid = pMsg->GetLong();
			long facID = pMsg->GetLong();
			GetGame()->GetRsFaction()->LoadAllFaction();
			if(MSG_S2DB_LOAD_FACTION == pMsg->GetType())
			{
				CMessage msg(MSG_DB2S_LOAD_FACTION);
				msg.Add((msgid));
				((CDBFaction*)GetOrganiCtrl()->GetFactionOrganizing(facID))->PushBackToMsg(msg);
				msg.SendToSocket(pMsg->GetSocketID());
				AddLogText("����GS faction������Ϣ...OK��");
			}
			else if(MSG_W2DB_LOAD_FACTION == pMsg->GetType())
			{
				CMessage msg(MSG_DB2W_LOAD_FACTION);
				msg.Add((msgid));
				((CDBFaction*)GetOrganiCtrl()->GetFactionOrganizing(facID))->PushBackToMsg(msg);
				msg.SendToSocket(pMsg->GetSocketID());
				AddLogText("����WS faction������Ϣ...OK��");
			}*/
		}
		break;
	case MSG_S2DB_LOAD_CONFEDERATION:
	case MSG_W2DB_LOAD_CONFEDERATION:
		{
	/*		long msgid = pMsg->GetLong();
			long facID = pMsg->GetLong();
			GetGame()->GetRsConfederation()->LoadAllConfederation();
			if(MSG_S2DB_LOAD_CONFEDERATION == pMsg->GetType())
			{
				CMessage msg(MSG_DB2S_LOAD_CONFEDERATION);
				msg.Add((msgid));
				((CDBFaction*)GetOrganiCtrl()->GetFactionOrganizing(facID))->PushBackToMsg(msg);
				msg.SendToSocket(pMsg->GetSocketID());
				AddLogText("����GS union������Ϣ...OK��");
			}
			else if(MSG_W2DB_LOAD_CONFEDERATION == pMsg->GetType())
			{
				CMessage msg(MSG_DB2W_LOAD_CONFEDERATION);
				msg.Add((msgid));
				((CDBFaction*)GetOrganiCtrl()->GetFactionOrganizing(facID))->PushBackToMsg(msg);
				msg.SendToSocket(pMsg->GetSocketID());
				AddLogText("����WS union������Ϣ...OK��");
			}
			*/
		}
		break;
	case MSG_W2DB_LOAD_INIT_PLAYER:
		{
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
	/*		long msgTimerID = pMsg->GetLong();

			long id = 0;
			char CDKey[256];
			memset(CDKey, 0, sizeof(CDKey));
			
			// �������ʧ�ܣ�pPlayer=NULL,�ڴ���������ݵط����ڽ��з���ʧ����Ϣ����
			CMyAdoBase::CreateCn(cnLoad);

			// ����һ����������
			if(!CMyAdoBase::OpenCn(cnLoad)) 
				AddLogText("�������ݿ�ʧ��");
			else
			{
				//---------------- ��ȡPlayerBase
				long lPtr = 0;
				long id = pMsg->GetLong();
				CDBPlayer* player = new CDBPlayer;
				player->SetID(id);
				pMsg->GetStr(CDKey, 256);

				AddLogText("��ȡPlayer��ʼ...");

				vector<BYTE> ByteData;
				nTransactionCounter = LoadPlayer(ByteData, player, cnLoad);

				CMessage msg(MSG_DB2W_LOAD_INIT_PLAYER);
				vector<BYTE> pBA;
				_AddToByteArray(&pBA, msgTimerID);
				_AddToByteArray(&pBA, player->GetID()); // player ID
				_AddToByteArray(&pBA, CDKey);	// CDKey
				player->AddToByteArray(&pBA);   // Player's data
				msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(pMsg->GetSocketID());
			}*/
		}
		break;
	}
}
