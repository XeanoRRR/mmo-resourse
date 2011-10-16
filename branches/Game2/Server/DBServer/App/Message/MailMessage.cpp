
#include "stdafx.h"
#include "..\nets\netdb\message.h"
#include "..\nets\Servers.h"
#include "..\DBMail.h"
#include "..\dbmailgoods.h"
#include "..\DBmailGroup.h"
#include "..\DBGood.h"
#include "..\DBGoodsGroup.h"

#include "..\DBentityManager.h"
#include "..\..\dbserver\game.h"
#include "..\dbplayer.h"
#include "..\DBLoginPlayer.h"
#include "../../../../dbaccess\worlddb\rsplayer.h"
#include "../../../../dbaccess/WorldDB/RsMail.h"
#include "../DBAccessThread.h"
#include "../DBLoadThread.h"
#include "../DBSaveThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnMailMessage(CMessage* pMsg)
{
	size_t nSavePlayerCnt = 0, 
		nCreationCnt = 0,
		nRestoreCnt = 0,
		nDeletionCnt = 0;
	long nTransactionCounter = 0;
	
	switch(pMsg->GetType())
	{

	case MSG_W2DB_MAIL_INITLOAD:
		{
			CGUID ownerID;
			pMsg->GetGUID(ownerID);
	
			// Ͷ�ݶ�ȡ�߳�ȡ��
			CGUID tGUID;
			CGUID::CreateGUID(tGUID);
			LoadThreadData* loadData = new LoadThreadData(tGUID, DB_MAILGROUP);
			loadData->loadObj->opType = DB_OPER_LOAD;
			loadData->loadObj->objGUID = ownerID;
			loadData->loadObj->objType = DB_MAILGROUP;
			loadData->loadObj->objNum  = 1;
			((CDBMailGroup*)loadData->dbObj)->SetOwnerGUID(ownerID);
			GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
		}
		break;
	case MSG_W2DB_MAIL_INSERT_OFFLINE: // ���һ������������ʼ�Group
		{
			char szOwnerName[128];
			pMsg->GetStr(szOwnerName, sizeof(szOwnerName));
			CGUID ownerID = GetGame()->GetRsPlayer()->GetPlayerExID(szOwnerName);
			if(ownerID != NULL_GUID)
			{
				long bufSize = pMsg->GetLong();
				// ��Ϣ���ݿ�
				BYTE* msgBuf = new BYTE[bufSize];
				// pos
				long msgBufPos = 0;
				// ��ȡMsgBuf
				pMsg->GetEx(msgBuf, bufSize);
				
				CGUID tGUID;
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGUID, bufSize); // GUID
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGUID, bufSize); // GUID

				SaveThreadData* saveData = new SaveThreadData(NULL_GUID, DB_MAILGROUP);
				saveData->saveObj->opType = DB_OPER_INSERT;
				saveData->saveObj->objGUID = NULL_GUID;
				saveData->saveObj->objType = DB_MAILGROUP;
				((CDBMailGroup*)(saveData->dbObj))->Insert(0, msgBuf, msgBufPos, bufSize, ownerID);
				saveData->saveObj->objNum  = ((CDBMailGroup*)(saveData->dbObj))->GetMailGroupSet().size();
				GetGame()->GetDBEntityManager()->PushSaveThreadData(saveData);
				SAFE_DELETE_ARRAY(msgBuf);
			}
			else
			{
				// ���ز����ڵ���Ϣ
				CMessage msg(MSG_DB2W_MAIL_OPERATION_RESLUT);
				msg.SendToSocket(GetGame()->GetWSSocket());
			}
		}
		break;
	case MSG_W2DB_MAIL_DELETE_OFFLINE: // ɾ��һ������������ʼ�Group
		{
			char szOwnerName[128];
			pMsg->GetStr(szOwnerName, sizeof(szOwnerName));
			CGUID ownerID = GetGame()->GetRsPlayer()->GetPlayerExID(szOwnerName);
			if(ownerID != NULL_GUID)
			{
				long bufSize = pMsg->GetLong();
				// ��Ϣ���ݿ�
				BYTE* msgBuf = new BYTE[bufSize];
				// pos
				long msgBufPos = 0;
				// ��ȡMsgBuf
				pMsg->GetEx(msgBuf, bufSize);
				
				CGUID tGUID;
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGUID, bufSize); // GUID
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGUID, bufSize); // GUID

				SaveThreadData* saveData = new SaveThreadData(NULL_GUID, DB_MAILGROUP);
				saveData->saveObj->opType = DB_OPER_INSERT;
				saveData->saveObj->objGUID = NULL_GUID;
				saveData->saveObj->objType = DB_MAILGROUP;
				((CDBMailGroup*)(saveData->dbObj))->Delete(0, msgBuf, msgBufPos, bufSize, ownerID);
				saveData->saveObj->objNum  = -(long)(((CDBMailGroup*)(saveData->dbObj))->GetMailGroupSet().size());
				GetGame()->GetDBEntityManager()->PushSaveThreadData(saveData);
				SAFE_DELETE_ARRAY(msgBuf);
			}
			else
			{
				// ���ز����ڵ���Ϣ
				// ���ز����ڵ���Ϣ
				CMessage msg(MSG_DB2W_MAIL_OPERATION_RESLUT);
				msg.SendToSocket(GetGame()->GetWSSocket());
			}
		}
		break;
	case MSG_W2DB_MIAL_QUERY:
		{
			long curTime = pMsg->GetLong();
			long lQueryTime = pMsg->GetLong();
			LoadThreadData* loadData = new LoadThreadData(NULL_GUID, DB_MAILGROUP);
			loadData->loadObj->objGUID = NULL_GUID;
			loadData->loadObj->objType = DB_MAILGROUP;
			loadData->loadObj->objNum  = 0;
			loadData->loadObj->opType  = DB_OPER_QUERY;
			loadData->loadObj->lQueryTime = lQueryTime;
			GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
		}
		break;
	case MSG_W2DB_MIAL_ADDOFFLINEPLAYERCOSTGOLD:
		{
/*			char szOwnerName[128];
			pMsg->GetStr(szOwnerName, sizeof(szOwnerName));

			CDBGoods pDBGoods(NULL_GUID);
			long bufSize = pMsg->GetLong();
			BYTE* buf = new BYTE[bufSize];
			pMsg->GetEx(buf, bufSize);
			long pos = 0;
			pDBGoods.Save(0, buf, pos, bufSize);
			SAFE_DELETE_ARRAY(buf);

			CGUID ownerID = GetGame()->GetRsPlayer()->GetPlayerExID(szOwnerName);
			
			CDBPlayer* pPlayer = GetGame()->GetDBEntityManager()->FindPlayer(ownerID);
			if(pPlayer)
			{
				// ���ҽ��Ǯ��
				map<long, set<CGUID>>::iterator itr = pPlayer->GetGoodsGroupPtr()->GetGoodsGroupMap().find(CRSGoods::PLACE_WALLET);
				if(itr != pPlayer->GetGoodsGroupPtr()->GetDirtyGoodsMap().end())
				{
					CDBGoods* pGoods = pPlayer->GetGoodsGroupPtr()->FindGoods( *((itr->second).begin()) );
					if(pGoods)
						*pGoods = pDBGoods;
				}
			}
			bool ret = GetGame()->GetRSGoods()->UpdatePlayerGoldCoins(&pDBGoods, ownerID, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());
			CMessage msg(MSG_DB2W_MIAL_ADDOFFLINEPLAYERCOSTGOLD);
			msg.Add((BYTE)ret);
			msg.Add(szOwnerName);
			msg.Add(ownerID);
			// ��Ҹ���
			long lGoldNum = pDBGoods.GetLongAttr(string("amount"), 0);
			msg.Add(lGoldNum);
			msg.SendToSocket(GetGame()->GetWSSocket());*/
		}
		break;

	case MSG_W2DB_MIAL_SENDSAVEGROUPMAILS:
		{
			MAIL_LEVEL_OPER_FLAG levelOpFlag = (MAIL_LEVEL_OPER_FLAG)pMsg->GetLong();
			long level = pMsg->GetLong();
			long sex = pMsg->GetLong();
			long countryNum = pMsg->GetLong();
			long* countryIDArray = NULL;
			if(countryNum)
			{
				countryIDArray = new long[countryNum];
				pMsg->GetEx(countryIDArray, countryNum);
			}

			CDBMail* tmpDbMail = NULL;

			long bufSize = pMsg->GetLong();
			if(bufSize)
			{
				BYTE* buf = new BYTE[bufSize];
				pMsg->GetEx(buf, bufSize);

				CDBMail* pDBMail = new CDBMail(NULL_GUID);

				long pos = 0;
				pDBMail->Save(0, buf, pos, bufSize);
				SAFE_DELETE_ARRAY(buf);
				
				// �ڴ������ϵͳ�ż�
				bool bSuitableFlag = false;
				CDBEntityManager::DBPlayerMapItr playerItr = GetGame()->GetDBEntityManager()->GetDBPlayerMap().begin();
				for(; playerItr != GetGame()->GetDBEntityManager()->GetDBPlayerMap().end(); playerItr++)
				{
					if(level != -1)
					{
						switch(levelOpFlag)
						{
						case MAIL_LEVEL_GREATANDEQUAL:
							{
								if(level <= playerItr->second->GetLongAttr(string("Levels"),0))
									bSuitableFlag = true;
								else
									bSuitableFlag = false;
							}
							break;
						case MAIL_LEVEL_LESSANDEQUAL:
							{
								if(level >= playerItr->second->GetLongAttr(string("Levels"),0))
									bSuitableFlag = true;
								else
									bSuitableFlag = false;
							}
							break;
						}
						
					}
					if(sex != -1)
					{
						if(level <= playerItr->second->GetLongAttr(string("Sex"),0))
							bSuitableFlag = true;
						else
							bSuitableFlag = false;
					}
					if(countryNum != 0 && countryIDArray != NULL)
					{
						long tmpCountryID = playerItr->second->GetLongAttr(string("Country"),0);
						for(int i=0; i<countryNum; i++)
						{
							if(countryIDArray[i] == tmpCountryID)
							{
								bSuitableFlag = true;
								break;
							}
							else
								bSuitableFlag = false;
						}
					}

					if(bSuitableFlag == true) // ��������
					{
						tmpDbMail = new CDBMail(NULL_GUID);
						*tmpDbMail = *pDBMail;
						playerItr->second->GetMailGroupPtr()->GetMailGroupSet().insert(tmpDbMail);
					}
				}

				// ���ݿ������ϵͳ�ż�
				GetGame()->GetRsMail()->InsertSysMailToGroup(tmpDbMail, levelOpFlag, level, sex, countryIDArray, countryNum, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr());
			}
		}
	}
}

