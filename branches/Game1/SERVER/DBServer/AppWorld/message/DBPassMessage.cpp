
#include "stdafx.h"
#include "..\nets\netdb\message.h"
#include "..\nets\Servers.h"
#include "..\dbgood.h"
#include "..\dbgenvar.h"
#include "..\DBGoodsGroup.h"
#include "..\DBGenVarGroup.h"
#include "..\DBLinkMan.h"
#include "..\DBLinkManGroup.h"
#include "..\DBFactionApplyGroup.h"
#include "..\DBFactionMemberGroup.h"

#include "..\DBentityManager.h"
#include "..\..\dbserver\game.h"
#include "..\dbplayer.h"
#include "..\DBLoginPlayer.h"
#include "../../../../dbaccess\worlddb\rsplayer.h"
#include "../../../../dbaccess\worlddb\rsgenvar.h"
#include "../DBDupRgn.h"
#include "../DBRgnGoodsGroup.h"
#include "../../../../dbaccess/WorldDB/RsDupRgn.h"
#include "../DBAccessThread.h"
#include "../DBLoadThread.h"
#include "../DBSaveThread.h"
#include "../../../../dbaccess/WorldDB/DbIncrementLog.h"
#include "../../../../dbaccess/WorldDB/RsFaction.h"

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
	case MSG_W2DB_PASS_GM_BANPLAYER:
		{
			char szName[128];
			pMsg->GetStr(szName, 128);
			if(szName)
			{
				long time = pMsg->GetLong();
				string strCdkey = GetGame()->GetRsPlayer()->GetCDKey(szName);
				CMessage msg(MSG_DB2W_PASS_GM_BANPLAYER);
				msg.Add(strCdkey.c_str());
				msg.Add(time);
				msg.SendToSocket(pMsg->GetSocketID());
			}
		}
		break;
	case MSG_DB2W_PASS_LOAD_ALL_SCRIPTVAR:
		{
			// ���ڴ��в��Ҳ�����
			if(GetGame()->GetDBEntityManager()->GetDBGenVarMap().size())
			{
				CDBGenVarGroup genVarGroup(NULL_GUID);
				CDBEntityManager::DBGenVarMapItr itr = GetGame()->GetDBEntityManager()->GetDBGenVarMap().begin();
				for(; itr != GetGame()->GetDBEntityManager()->GetDBGenVarMap().end(); itr++)
				{
					CDBGenVar* tGenVar = new CDBGenVar(itr->first);
					*tGenVar = *(itr->second);
					genVarGroup.GetGenVarGroupSet().insert(tGenVar);
				}
				
				vector<BYTE> pBA;
				genVarGroup.AddToByteArray(pBA);
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				// ��װ��Ϣ
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg.Add((long)DB_GENVARGROUP);
				msg.Add((BYTE)0);
				msg.Add((BYTE)DB_OPER_LOAD);
				msg.Add((long)-99999999);
				msg.Add((WORD)1);
				msg.Add((long)pBA.size());
				if(pBA.size())
					msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(GetGame()->GetWSSocket());
				return;
			}

			// �����ݿ��ȡ
			LoadThreadData* loadData = new LoadThreadData(NULL_GUID, DB_GENVARGROUP);
			loadData->loadObj->objType = DB_GENVARGROUP;
			loadData->loadObj->objNum  = 1;
			loadData->loadObj->opType = DB_OPER_LOAD;
			GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
		}
		break;
	case MSG_W2DB_PASS_LOAD_ALL_DUPRGN:
		{
			// ���͸�WS���˷�������
			GetGame()->GetRsDupRgn()->LoadAll(GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());

			CDBEntityManager::DBDupRgnMapItr itr = GetGame()->GetDBEntityManager()->GetDBDupRgnMap().begin();
			for(; itr!=GetGame()->GetDBEntityManager()->GetDBDupRgnMap().end(); itr++)
			{
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				// ��װ��Ϣ
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg.Add((long)DB_PHDUPRGN);
				msg.Add((BYTE)0);
				msg.Add((BYTE)DB_OPER_LOAD);
				msg.Add((long)-99999999);
				msg.Add((WORD)1);
				vector<BYTE> pBA;
				itr->second->AddEntityDataToMsg(DB_OPER_LOAD, pBA);
				msg.Add((long)pBA.size());
				msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(pMsg->GetSocketID());
								
				// ������Ʒ����
				CMessage msg2(MSG_DB2W_OPER_ENTITY);
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg2.Add((long)DB_PHRGNGOODSGROUP);
				msg2.Add((BYTE)0);
				msg2.Add((BYTE)DB_OPER_LOAD);
				msg2.Add((long)-1);
				msg2.Add((WORD)1);
				vector<BYTE> pBAGoodsGroup;

				assert(itr->second != NULL);

				itr->second->GetGoodsGroupPtr()->SetOwnerGUID(itr->second->GetExID());
				itr->second->GetGoodsGroupPtr()->AddToByteArray(pBAGoodsGroup);
				msg2.Add((long)pBAGoodsGroup.size());
				msg2.AddEx(&pBAGoodsGroup[0], pBAGoodsGroup.size());
				msg2.SendToSocket(GetGame()->GetWSSocket());
			}
		}
		break;
	case MSG_W2DB_PASS_LOG_PLAYERBASELIST:				// WS����LS������
		{
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
			long msgTimerID = 0;
			char szCdkey[20];
			pMsg->GetStr(szCdkey, 20);
			msgTimerID = pMsg->GetLong();

			bool isFoundInMem = false;
			// �����ڴ��в���
			set<CGUID>* setLPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayerByCdkey(szCdkey);
			if(setLPlayer && setLPlayer->size())
			{
				CMessage msg(MSG_DB2W_PASS_LOG_PLAYERBASELIST);
				msg.Add((BYTE)1);
				msg.Add((szCdkey));
				msg.Add((short)setLPlayer->size());
				
				set<CGUID>::iterator vecItr;
				for(vecItr = setLPlayer->begin(); vecItr!=setLPlayer->end(); vecItr++)
				{
					CDBLoginPlayer* tlPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer((*vecItr));
					if(tlPlayer)
					{
						CMessage msg1(MSG_DB2W_OPER_ENTITY);
						// opType		  4byte, ������������
						// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
						// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
						// MSG ID:        4byte, ����Ϣ��MSGID
						// Entity Num:	2byte, ��ʵ�����(X)
						msg1.Add((long)DB_LOGINPLAYER);
						msg1.Add((BYTE)0);
						msg1.Add((BYTE)DB_OPER_LOAD);
						msg1.Add((long)-1);
						msg1.Add((WORD)1);
						vector<BYTE> pBA;
						tlPlayer->AddToByteArray(pBA);
						msg1.Add((long)pBA.size());
						msg1.AddEx(&pBA[0], pBA.size());
						msg1.SendToSocket(GetGame()->GetWSSocket());
						_snprintf(pszLogingInfo,512,"mem load LoginPlayer,CDKEY[%s].", szCdkey);
						PutStringToFile("Login_DBS_Info",pszLogingInfo);
						// loginplayer guid
						msg.Add(*vecItr);
						// ��ʱ���ʱ������
						tlPlayer->SetLogoutTime(-1);
					}
				}
				
				msg.SendToSocket(GetGame()->GetWSSocket());
				isFoundInMem = true;
			}
			
			
			// �ڴ�δ�ҵ�,Ͷ�ݵ�LoadThread
			if(isFoundInMem == false)
			{
				CGUID tGUID;
				CGUID::CreateGUID(tGUID);
				LoadThreadData* loadData = new LoadThreadData(tGUID, DB_LOGINPLAYER);
				loadData->loadObj->objGUID = tGUID;
				loadData->loadObj->objType = DB_LOGINPLAYER;
				loadData->loadObj->objNum  = 1;
				strcpy(loadData->loadObj->szCDKEY, szCdkey);
				loadData->loadObj->opType = DB_OPER_LOAD;
				GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
			}
		}
		break;
	case MSG_W2DB_PASS_LOG_QUEST_PLAYERDATA:						// WS����LS������
		{
			char szCdkey[20];
			CGUID nID;
			
			long lSignCode = pMsg->GetLong();
			pMsg->GetGUID(nID);
			pMsg->GetStr(szCdkey, 20);

			//�Ʒ���
			DWORD dwClientIP=pMsg->GetDWord();
			
			bool isFoundInMem = false;
			// �����ڴ��в���
			CDBEntityManager::DBPlayerMapItr pItr = GetGame()->GetDBEntityManager()->GetDBPlayerMap().find(nID);
			CDBEntityManager::DBLoginPlayerMapItr lpItr = GetGame()->GetDBEntityManager()->GetDBLoginPlayerMap().find(nID);
			if(pItr != GetGame()->GetDBEntityManager()->GetDBPlayerMap().end()
				&& lpItr != GetGame()->GetDBEntityManager()->GetDBLoginPlayerMap().end()) // �ҵ����
			{
				CMessage msg1(MSG_DB2W_OPER_ENTITY);
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg1.Add((long)DB_PLAYER);
				msg1.Add((BYTE)0);
				msg1.Add((BYTE)DB_OPER_LOAD);
				msg1.Add((long)-1);
				msg1.Add((WORD)1);
				vector<BYTE> pBA;
				pItr->second->AddToByteArray(pBA);
				msg1.Add((long)pBA.size());
				msg1.AddEx(&pBA[0], pBA.size());
				msg1.SendToSocket(GetGame()->GetWSSocket());
				const char* strName = pItr->second->GetStringAttr(string("Name"), 0);
				_snprintf(pszLogingInfo,512,"mem load Player,name[%s].", strName);
				PutStringToFile("Login_DBS_Info",pszLogingInfo);

				if(pItr->second->GetExID() == NULL_GUID)
				{
					_snprintf(pszLogingInfo,512,"mem load Player,name[%s], GUID is NULL!", strName);
					PutStringToFile("Login_DBS_Info",pszLogingInfo);
				}

				// ���ͽ�ɫ��Ʒ����
				CMessage msg2(MSG_DB2W_OPER_ENTITY);
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg2.Add((long)DB_GOODSGROUP);
				msg2.Add((BYTE)0);
				msg2.Add((BYTE)DB_OPER_LOAD);
				msg2.Add((long)-1);
				msg2.Add((WORD)1);
				vector<BYTE> pBAGoodsGroup;
				// ͳ�������Ʒ
				if(pItr->second->GetGoodsGroupPtr()->GetOwnerGUID() == NULL_GUID)
				{
					_snprintf(pszLogingInfo,512,"MSG_W2DB_PASS_LOG_QUEST_PLAYERDATA, GoodsGroup Owner[%s] ID is NULL!", strName);
					PutStringToFile("Login_DBS_Info",pszLogingInfo);
				}
				pItr->second->GetGoodsGroupPtr()->AddToByteArray(pBAGoodsGroup);
				msg2.Add((long)pBAGoodsGroup.size());
				msg2.AddEx(&pBAGoodsGroup[0], pBAGoodsGroup.size());
				msg2.SendToSocket(GetGame()->GetWSSocket());

				// ͳ�����linkman
				CDBLinkmanGroup tLinkmanGroup(NULL_GUID);
				GetGame()->GetRsPlayer()->LoadLinkmanData(pItr->second, &tLinkmanGroup, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());

				// ���ͽ�ɫlinkman����
				CMessage msg3(MSG_DB2W_OPER_ENTITY);
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg3.Add((long)DB_LINKMANGROUP);
				msg3.Add((BYTE)0);
				msg3.Add((BYTE)DB_OPER_LOAD);
				msg3.Add((long)-1);
				msg3.Add((WORD)1);
				vector<BYTE> pBALinkmanGroup;	
				tLinkmanGroup.SetOwnerGUID(nID);
				CDBEntityManager::DBLinkmanMapItr linkmanItr = GetGame()->GetDBEntityManager()->GetDBLinkmanMap().begin();
				for(; linkmanItr != GetGame()->GetDBEntityManager()->GetDBLinkmanMap().end(); linkmanItr++)
				{
					CGUID ownerGUID;
					CGUID AimGUID;
					linkmanItr->second->GetGuidAttr("OwnerGUID", 0, ownerGUID);
					linkmanItr->second->GetGuidAttr("AimGUID", 0, AimGUID);
					if(ownerGUID == nID || AimGUID == nID)
					{
						CDBLinkMan* tLinkMan = new CDBLinkMan(linkmanItr->first);
						*tLinkMan = *(linkmanItr->second);
						tLinkmanGroup.GetLinkmanGroupSet().insert(tLinkMan);
					}
				}
				tLinkmanGroup.AddToByteArray(pBALinkmanGroup);
				msg3.Add((long)pBALinkmanGroup.size());
				msg3.AddEx(&pBALinkmanGroup[0], pBALinkmanGroup.size());
				msg3.SendToSocket(GetGame()->GetWSSocket());

				CMessage msg(MSG_DB2W_PASS_LOG_QUEST_PLAYERDATA);
				msg.Add((long)-1);
				msg.Add((char)LOGIN_QUEST_PLAYERDATA_SUCCEED);
				msg.Add(nID);//PLAYERID
				msg.Add(szCdkey);//CDKEY
				msg.Add(dwClientIP);//�Ʒ���
				
				long delTime[6];
				lpItr->second->GetTimeAttr(string("DelDate"), 0, delTime, sizeof(long)*6);

				SYSTEMTIME st;
				tm tt;
				GetLocalTime(&st);
				tt.tm_year= st.wYear-delTime[0]; 
				tt.tm_mon = st.wMonth-1; 
				tt.tm_mday = st.wDay-delTime[2];
				time_t res = mktime(&tt);
				if(res)
					msg.Add((LONG64)res);
				else
					msg.Add((LONG64)0);

				msg.SendToSocket(GetGame()->GetWSSocket());
				isFoundInMem = true;

				char szGUID[128];
				nID.tostring(szGUID);
			}

			// �ڴ�δ�ҵ�,Ͷ�ݵ�LoadThread
			if(isFoundInMem == false)
			{
				LoadThreadData* loadData = new LoadThreadData(nID, DB_PLAYER);
				strcpy(loadData->loadObj->szCDKEY, szCdkey);
				loadData->loadObj->objGUID = nID;
				loadData->loadObj->objType = DB_PLAYER;
				loadData->loadObj->objNum  = 1;
				loadData->loadObj->opType = DB_OPER_LOAD;	
				GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);

				char szGUID[128];
				nID.tostring(szGUID);
				_snprintf(pszLogingInfo,512,"Ͷ��Load�̶߳�ȡ��ɫ Cdkey[%s],ID[%s]", szCdkey, szGUID);
				PutStringToFile("Login_DBS_Info",pszLogingInfo);
			}
		}
		break;
	case MSG_W2DB_PASS_LOG_DELETEROLE:						// WS����ɾ��һ����ɫ
		{	
			// ȡ�ø���Ϣ�Ķ�ʱ��MSGID
			long msgTimerID = pMsg->GetLong();

			CGUID nID = NULL_GUID;
			pMsg->GetGUID(nID);
			char szCDKey[64];
			pMsg->GetStr(szCDKey, 64);
			DWORD tDelTime = pMsg->GetDWord();

			// �ж��Ƿ�ɾ��// ���ý�ɫ�Ƿ��Ѿ���ɾ��
			CMessage msg(MSG_DB2W_PASS_LOG_DELETEROLE);
			msg.Add(msgTimerID);

			GetGame()->GetRsPlayer()->DeletePlayer(nID, tDelTime, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());

			time_t delTime = GetGame()->GetRsPlayer()->GetPlayerDeletionDate(nID, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());
			if( delTime )
			{
				char name[128];
				GetGame()->GetRsPlayer()->GetPlayerNameByID(nID, name);

				msg.Add((char)LOGIN_DELETEROLE_SUCCEED);
				msg.Add(nID);
				msg.Add(szCDKey);
				msg.Add((long)7);
				msg.Add(name);
				msg.SendToSocket(GetGame()->GetWSSocket());

				_snprintf(pszLogingInfo,512,"ɾ����ɫ, [CDKEY]:%s...OK!", szCDKey);
				PutStringToFile("Login_DBS_Info",pszLogingInfo);
				// ��ʱˢ������
				lDeletePlayers++;
				lResumePlayers--;
				// �޸��ڴ�������
				CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(nID);
				if(lPlayer)
				{
					tm *now;
					now = localtime(&delTime);
					long delTime[6] = {0};
					delTime[0] = now->tm_year+1900;
					delTime[1] = now->tm_mon+1;
					delTime[2] = now->tm_mday;
					lPlayer->SetTimeAttr(string("DelDate"), string("CSL_PLAYER_BASE"), 0, delTime, sizeof(long)*6);
				}
			}
			else //δ��ɾ��
			{
				char name[128];
				GetGame()->GetRsPlayer()->GetPlayerNameByID(nID, name);

				msg.Add((char)LOGIN_DELETEROLE_FAILED);
				msg.Add(nID);
				msg.Add(szCDKey);
				msg.Add(tDelTime);
				msg.Add(name);
				msg.SendToSocket(GetGame()->GetWSSocket());//send to loginserver
				_snprintf(pszLogingInfo,512,"ɾ����ɫ, [CDKEY]:%s...Failed!", szCDKey);
				PutStringToFile("Login_DBS_Info",pszLogingInfo);
			}
		}
		break;
	case MSG_W2DB_PASS_LOG_RESTOREROLE:					// WS����ָ�һ����ɫ
		{
			CGUID guid;
			char cdkey[128];
			pMsg->GetGUID(guid);
			pMsg->GetStr(cdkey, 128);

			CMessage msg(MSG_DB2W_PASS_LOG_RESTOREROLE);
	
			bool flag = GetGame()->GetRsPlayer()->RestorePlayer(guid, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());

			msg.Add((char)flag);
			msg.Add(guid);
			msg.Add(cdkey);
			msg.SendToSocket(GetGame()->GetWSSocket());
			_snprintf(pszLogingInfo,512,"�ָ���ɫ, [CDKEY]:%s...OK!", cdkey);
			PutStringToFile("Login_DBS_Info",pszLogingInfo);
			// ��ʱˢ������
			lDeletePlayers--;
			lResumePlayers++;
			
			if(flag == true)
			{
				// �޸��ڴ�������
				CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(guid);
				if(lPlayer)
				{
					long delTime[6] = {0};
					lPlayer->SetTimeAttr(string("DelDate"), string("CSL_PLAYER_BASE"), 0, delTime, sizeof(long)*6);
				}
			}
		}
		break;
	case MSG_W2DB_PASS_LOG_CREATEROLE:							    // WS���󴴽�һ����ɫ
		{
			char szName[50],szCdkey[20];
			pMsg->GetStr(szName,50);
			char nOccupation = pMsg->GetChar();
			char nSex = pMsg->GetChar();
			BYTE nHead = pMsg->GetByte();
			BYTE nFace = pMsg->GetByte();
			BYTE btCountry = pMsg->GetByte();
			pMsg->GetStr(szCdkey,20);

			CGUID tGuid;
			pMsg->GetGUID(tGuid);
			long msgTimerID = pMsg->GetLong();
			BYTE btMaxCharactersNum = pMsg->GetByte();

			BYTE nCnt = 0;
			nCnt = GetGame()->GetRsPlayer()->GetPlayerCountInCdkey(szCdkey);
			char szGUID[128];
			tGuid.tostring(szGUID);
	
			// ���ݿ��쳣
			if( nCnt == 0xff )
			{
				CMessage msg(MSG_DB2W_PASS_LOG_CREATEROLE);
				msg.Add(msgTimerID);
				msg.Add((char)LOGIN_CREATEROLE_DBERROR);
				msg.Add(szCdkey);
				msg.Add(tGuid);
				msg.SendToSocket(pMsg->GetSocketID());
				_snprintf(pszLogingInfo,512,"������ɫCDKEY : %s, ID : %s, ���ݿ��쳣!", szCdkey, szGUID);
				PutStringToFile("Login_DBS_Info",pszLogingInfo);
				return;
			}

			if( nCnt >= btMaxCharactersNum )
			{
				//���ʹ���ʧ�ܵ���Ϣ  ��ɫ����
				CMessage msg(MSG_DB2W_PASS_LOG_CREATEROLE);
				msg.Add(msgTimerID);
				msg.Add((char)LOGIN_CREATEROLE_FULL);
				msg.Add(szCdkey);
				msg.Add(tGuid);
				msg.SendToSocket(pMsg->GetSocketID());
				_snprintf(pszLogingInfo,512,"������ɫCDKEY : %s, ID : %s, ��ɫ����!", szCdkey, szGUID);
				PutStringToFile("Login_DBS_Info",pszLogingInfo);
				return;
			}

			// ��ɫ��Ҳ��ʹ��(WS���ȼ��list��MapPlayer �����DBS�ϼ��)
			if(	GetGame()->GetRsPlayer()->IsNameExist(szName) )
			{
				CMessage msg(MSG_DB2W_PASS_LOG_CREATEROLE);
				msg.Add(msgTimerID);
				msg.Add((char)LOGIN_CREATEROLE_NAMEEXIST);
				msg.Add(szCdkey);
				msg.Add(tGuid);
				msg.SendToSocket(pMsg->GetSocketID());
				_snprintf(pszLogingInfo,512,"������ɫCDKEY : %s, ID : %s, ��ɫ��Ҳ��ʹ��!", szCdkey, szGUID);
				PutStringToFile("Login_DBS_Info",pszLogingInfo);
				return;
			}

			// �������ݿ����
			CDBLoginPlayer* lPlayer = GetGame()->GetDBEntityManager()->FindLoginPlayer(tGuid);
			if(NULL == lPlayer)
			{
				lPlayer = new CDBLoginPlayer(tGuid);
				lPlayer->SetAccount(szCdkey, strlen(szCdkey));
				GetGame()->GetDBEntityManager()->AddOneDBLoginPlayer(lPlayer);
				GetGame()->GetDBEntityManager()->AddLoginPlayerByCdkey(lPlayer);
				string tableName = "CSL_PLAYER_BASE";
				lPlayer->SetGuidAttr(string("guid"), tableName, 0, tGuid);
				lPlayer->SetStringAttr(string("Name"), tableName, 0, szName);
				lPlayer->SetStringAttr(string("Account"), tableName, 0, szCdkey);
				lPlayer->SetAccount(szCdkey, strlen(szCdkey));
				lPlayer->SetLongAttr(string("Occupation"), tableName, 0, (long)nOccupation);
				lPlayer->SetLongAttr(string("Sex"), tableName, 0, (long)nSex);
				lPlayer->SetLongAttr(string("HEAD"), tableName, 0, (long)nHead);
				lPlayer->SetLongAttr(string("FACE"), tableName, 0, (long)nFace);
				lPlayer->SetLongAttr(string("Country"), tableName, 0, (long)btCountry);
			}
			CDBPlayer* pPlayer = NULL;
			CDBEntityManager::DBPlayerMapItr pItr = GetGame()->GetDBEntityManager()->GetDBPlayerMap().find(tGuid);
			if(pItr != GetGame()->GetDBEntityManager()->GetDBPlayerMap().end())
				pPlayer = pItr->second;
			else
			{
				string tableName = "baseproperty";
				pPlayer = new CDBPlayer(tGuid);
				GetGame()->GetDBEntityManager()->AddOneDBPlayer(pPlayer);

				pPlayer->SetGuidAttr(string("guid"), tableName, 0, tGuid);
				pPlayer->SetStringAttr(string("Name"), tableName, 0, szName);
				pPlayer->SetAccount(szCdkey, strlen(szCdkey));
				pPlayer->SetLongAttr(string("Occupation"), tableName, 0, (long)nOccupation);
				pPlayer->SetLongAttr(string("Sex"), tableName, 0, (long)nSex);
				pPlayer->SetLongAttr(string("HeadPic"), tableName, 0, (long)nHead);
				pPlayer->SetLongAttr(string("FacePic"), tableName, 0, (long)nFace);
				pPlayer->SetLongAttr(string("Country"), tableName, 0, (long)btCountry);
			}
			// �������ݿ���ж���
			GetGame()->GetRsPlayer()->CreatePlayer(pPlayer, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());

			CMessage msg(MSG_DB2W_PASS_LOG_CREATEROLE);
			msg.Add(msgTimerID);
			msg.Add((char)LOGIN_CREATEROLE_SUCCEED);
			msg.Add(szName);
			msg.Add((char)nOccupation);
			msg.Add((char)nSex);
			msg.Add((BYTE)nHead);
			msg.Add((BYTE)nFace);
			msg.Add((BYTE)btCountry);
			msg.Add(szCdkey);
			msg.Add(tGuid);
			msg.SendToSocket(pMsg->GetSocketID());

			_snprintf(pszLogingInfo,512,"������ɫCDKEY : %s, ID : %s, �ɹ�!", szCdkey, szGUID);
			PutStringToFile("Login_DBS_Info",pszLogingInfo);
			// ��ʱˢ������
			lCreatePlayers++;
		}
		break;
	case MSG_W2DB_PSASS_OTHER_WRITELOG:
		{
			BYTE byType;
			char szDesc[256];
			char szAcc[32];
			CDBPlayer *pPlayer;
			DWORD dwMoney;
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			byType = pMsg->GetByte();
			dwMoney = pMsg->GetLong();
			pMsg->GetStr(szDesc, 256);
			pMsg->GetStr(szAcc, 32);
			SYSTEMTIME time;
			pMsg->GetEx(&time, sizeof(SYSTEMTIME));
			pPlayer = GetGame()->GetDBEntityManager()->FindPlayer(PlayerID);
			GetGame()->GetDbIncrementLog()->Save(byType, dwMoney,szDesc, PlayerID, szAcc, &time, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());
		}
		break;
	case MSG_W2DB_PSASS_OTHER_GETLOG_REQUEST:
		{
			CMessage msg(MSG_DB2W_PSASS_OTHER_GETLOG_REQUEST);
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			long gsid = pMsg->GetLong();
			msg.Add(gsid);
			GetGame()->GetDbIncrementLog()->Load(PlayerID, &msg, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());
			msg.SendToSocket(GetGame()->GetWSSocket());
		}
		break;
	case MSG_W2DB_PASS_LOAD_ALL_FACTION:
		{
			// ���͸�WS���й�������
			_ConnectionPtr pCn = GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr();
			
			GetGame()->GetDBEntityManager()->ClearFactionMap();
			if(!GetGame()->GetRsFaction()->LoadAllFaction(GetGame()->GetDBEntityManager()->GetDBFactionMap(), pCn))
				break;

			CDBEntityManager::DBFactionMapItr itr = GetGame()->GetDBEntityManager()->GetDBFactionMap().begin();
			for(; itr != GetGame()->GetDBEntityManager()->GetDBFactionMap().end(); itr++)
			{
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				// ��װ��Ϣ
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg.Add((long)DB_FACTION);
				msg.Add((BYTE)0);
				msg.Add((BYTE)DB_OPER_LOAD);
				msg.Add((long)-99999999);
				msg.Add((WORD)1);
				vector<BYTE> pBA;
				itr->second->AddEntityDataToMsg(DB_OPER_LOAD, pBA);
				msg.Add((long)pBA.size());
				msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(pMsg->GetSocketID());

				CMessage msgApply(MSG_DB2W_OPER_ENTITY);
				// ��װ��Ϣ
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msgApply.Add((long)DB_FACTIONAPPLYGROUP);
				msgApply.Add((BYTE)0);
				msgApply.Add((BYTE)DB_OPER_LOAD);
				msgApply.Add((long)-99999999);
				msgApply.Add((WORD)1);
				vector<BYTE> pApplyBA;
				itr->second->GetFactionApplyGroupPtr()->AddToByteArray(pApplyBA);
				msgApply.Add((long)pApplyBA.size());
				msgApply.AddEx(&pApplyBA[0], pApplyBA.size());
				msgApply.SendToSocket(pMsg->GetSocketID());

				CMessage msgMember(MSG_DB2W_OPER_ENTITY);
				// ��װ��Ϣ
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msgMember.Add((long)DB_FACTIONMEMBERGROUP);
				msgMember.Add((BYTE)0);
				msgMember.Add((BYTE)DB_OPER_LOAD);
				msgMember.Add((long)-99999999);
				msgMember.Add((WORD)1);
				vector<BYTE> pMemberBA;
				itr->second->GetFactionMemberGroupPtr()->AddToByteArray(pMemberBA);
				msgMember.Add((long)pMemberBA.size());
				msgMember.AddEx(&pMemberBA[0], pMemberBA.size());
				msgMember.SendToSocket(pMsg->GetSocketID());
			}

			CMessage msg(MSG_DB2W_PASS_LOAD_ALL_FACTION_FINISH);			
			msg.SendToSocket(GetGame()->GetWSSocket());
		}
		break;
	case MSG_W2DB_PASS_DelOneFaction:
		{
			CGUID AimGuid;
			pMsg->GetGUID(AimGuid);
			_ConnectionPtr pCn = GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr();
			BOOL bOptResult = GetGame()->GetRsFaction()->DelOneFaction(AimGuid, pCn);

			CMessage msg(MSG_DB2W_PASS_DelFaction_Finish);
			msg.Add(AimGuid);
			msg.Add((LONG)bOptResult);
			msg.SendToSocket(GetGame()->GetWSSocket());
		}
		break;
	}
}

