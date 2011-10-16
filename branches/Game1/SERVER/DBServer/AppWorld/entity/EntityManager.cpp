//EntityManager.cpp/////////////////////////////////////////////////////////////////////
//������:�ö���������еĶ���ʵ���Ͷ����������Ϣ����
//Author:������
//Create Time:2008.11.03
#include "StdAfx.h"
#include "Account.h"
#include "entityManager.h"
#include "Entity.h"
#include "EntityGroup.h"
#include "../Thread/DBAdoBase.h"
#include "../../../../dbaccess/WorldDB/RsEntity.h"
#include "../../dbserver/DBServer.h"
#include "../Thread/DBThread.h"
#include "../Thread/DBAccessThread.h"
#include "../Thread/DBMailThread.h"
#include "../public/entityproperty.h"
#include "../public/public.h"
#include "../public/tools.h"
#include "../public/RFile.h"
#include "../public/readwrite.h"
#include "../../../../dbaccess/MyAdoBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

std::string NULL_STRING = "";

HANDLE   CEntityManager::m_hLoadThreadEvent;
HANDLE   CEntityManager::m_hSaveThreadEvent;

HANDLE   CEntityManager::m_hMailThreadEvent;

extern long lMapAccounts;
extern long lMapPlayers;
extern long lMapFactions;
extern long lMapScripts;
extern long lMapProcs;

extern bool g_bSaveNowData;


void CEntityManager::Release(void)
{
	// ������Ϣ
	ThreadData* mailDataQuit = new ThreadData;
	mailDataQuit->retFlag = -99999999; // ����һ��ֵ��ʾ�߳��˳�
	list<ThreadData*> mailDataList;
	mailDataList.push_back(mailDataQuit);
	PushMailThreadData(mailDataList);
	// �ȴ�mail�߳��Ƴ�
	WaitForSingleObject(m_MailThreadQuitEvent, INFINITE);

	// ɾ���߳�
	m_pMailThread->Exit();
	SAFE_DELETE(m_pMailThread);

	// �����߳��˳�����
	list<ThreadData*> tmpLoadDataList;
	for (map<long, DBThread*>::iterator ite = m_mapLoadThread.begin(); ite != m_mapLoadThread.end(); ++ite)
	{
		ThreadData* tLoadData = new ThreadData;
		tLoadData->retFlag = -99999999; // ����һ��ֵ��ʾ�߳��˳�
		tmpLoadDataList.push_back(tLoadData);
	}
	PushLoadThreadData(tmpLoadDataList);

	list<ThreadData*> tmpSaveDataList;
	for (map<long, DBThread*>::iterator ite = m_mapSaveThread.begin(); ite != m_mapSaveThread.end(); ++ite)
	{
		ThreadData* tLoadData = new ThreadData;
		tLoadData->retFlag = -99999999; // ����һ��ֵ��ʾ�߳��˳�
		tmpSaveDataList.push_back(tLoadData);
	}
	PushSaveThreadData(tmpSaveDataList);

	// �ȴ��߳��˳�
	WaitForMultipleObjects(m_mapLoadThread.size()+m_mapSaveThread.size(), &m_vecThreadQuitEvent[0], TRUE, INFINITE);

	for (map<long, DBThread*>::iterator ite = m_mapLoadThread.begin(); ite != m_mapLoadThread.end(); ++ite)
	{
		// ɾ���߳�
		ite->second->Exit();
		SAFE_DELETE(ite->second);
	}
	m_mapLoadThread.clear();

	for (map<long, DBThread*>::iterator ite = m_mapSaveThread.begin(); ite != m_mapSaveThread.end(); ++ite)
	{
		// ɾ���߳�
		ite->second->Exit();
		SAFE_DELETE(ite->second);
	}
	m_mapSaveThread.clear();

	std::map<string, tagEntityBuildInfo*>::iterator deItr = m_mapObjAttrDef.begin();
	for(; deItr != m_mapObjAttrDef.end(); deItr++)
	{
		delete deItr->second;
	}
	m_mapObjAttrDef.clear();

	DBEntityComponentMapItr enItr = m_mapEntitys.begin();
	for (; enItr != m_mapEntitys.end(); enItr++)
	{
		map<CGUID, CEntityGroup*>::iterator entityItr = enItr->second.begin();
		for(; entityItr != enItr->second.end(); entityItr++)
			delete entityItr->second;
	}
	m_mapEntitys.clear();

	AccountMapItr accItr = m_mapAccounts.begin();
	for(; accItr != m_mapAccounts.end(); accItr++)
		delete accItr->second;
	m_mapAccounts.clear();

	DeleteCriticalSection(&m_MailCriticalSection);
	DeleteCriticalSection(&m_LoadCriticalSection);
	DeleteCriticalSection(&m_SaveCriticalSection);
	DeleteCriticalSection(&m_RetCriticalSection);
	DeleteCriticalSection(&m_RetMailCriticalSection);
	DeleteCriticalSection(&m_ThreadQuitCS);

	m_pAccessThread->GetAdoBasePtr()->CloseCn();
	CoUninitialize();
	SAFE_DELETE(m_pAccessThread);

	// �ʼ�������ȫ����
	list<ThreadData*>::iterator listItr = m_listMailThreadData.begin();
	for(; listItr != m_listMailThreadData.end(); listItr)
		delete (*listItr);
	m_listMailThreadData.clear();

	// �ʼ�������������
	listItr = m_listMailThreadRetData.begin();
	for(; listItr != m_listMailThreadRetData.end(); listItr)
		delete (*listItr);
	m_listMailThreadRetData.clear();

	// Ͷ�ݵ��̵߳Ĳ�������
	listItr = m_listLoadThreadOperData.begin();
	for(; listItr != m_listLoadThreadOperData.end(); listItr)
		delete (*listItr);
	m_listLoadThreadOperData.clear();

	listItr = m_listSaveThreadOperData.begin();
	for(; listItr != m_listSaveThreadOperData.end(); listItr)
		delete (*listItr);
	m_listSaveThreadOperData.clear();

	// ��ȡ��洢���̷�������
	listItr = m_listThreadRetData.begin();
	for(; listItr != m_listThreadRetData.end(); listItr)
		delete (*listItr);
	m_listThreadRetData.clear();
}

// �Ƿ�������
bool bDBSExit;

CEntityManager::CEntityManager()
{
	m_lAttrEnumValue = 0;

	m_hLoadThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hSaveThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hMailThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pAccessThread = NULL;
	m_pMailThread   = NULL;

	InitializeCriticalSection(&m_MailCriticalSection);
	InitializeCriticalSection(&m_LoadCriticalSection);
	InitializeCriticalSection(&m_SaveCriticalSection);
	InitializeCriticalSection(&m_RetCriticalSection);
	InitializeCriticalSection(&m_RetMailCriticalSection);
	InitializeCriticalSection(&m_ThreadQuitCS);
}
CEntityManager::~CEntityManager()
{
	Release();
}
// ��ʼ��DBThread
void CEntityManager::InitDBThread(string strProvider, 
								  string strDataSource, 
								  string strInitialCatalog,
								  string strUserID, 
								  string strPassword)
{
	// �������߳�
	for(int i=0; i<GetGame()->GetSetup()->dwPlayerLoadThreadNumber; i++)
	{
		DBThread* loadThread1 = new DBThread(strProvider, strDataSource, 
			strInitialCatalog, strUserID, strPassword, eThreadLoadType);
		m_mapLoadThread[i] = loadThread1;
		loadThread1->Begin();
	}

	// ����д�߳�
	    for(int i=0; i<GetGame()->GetSetup()->dwPlayerSaveThreadNumber; i++)
	{
		DBThread* saveThread1 = new DBThread(strProvider, strDataSource, 
			strInitialCatalog, strUserID, strPassword, eThreadSaveType);
		m_mapSaveThread[i] = saveThread1;
		saveThread1->Begin();
	}

	m_pAccessThread = new DBAccessThread(strProvider, strDataSource, 
		strInitialCatalog, strUserID, strPassword);

	m_pMailThread = new DBMailThread(strProvider, strDataSource, 
		strInitialCatalog, strUserID, strPassword);
	if(m_pMailThread)
		m_pMailThread->Begin();

	CMyAdoBase::Initialize(strProvider, strDataSource, strInitialCatalog, strUserID, strPassword);
	m_pAccessThread->GetAdoBasePtr()->OpenCn();
	InitPropertyIniData();
}


// ����Ϣȡ�ò�������
CEntityGroup* CEntityManager::FindRootEntity(const string& flag, const CGUID& guid)
{
	DBEntityComponentMapItr itr = m_mapEntitys.find(flag);
	if(itr != m_mapEntitys.end())
	{
		std::map<CGUID, CEntityGroup*>::iterator enItr = itr->second.find(guid);
		if(enItr != itr->second.end())
			return enItr->second;
	}
	return NULL;
}
void CEntityManager::ProcessMsg(CMessage* pMsg)
{
	//��Ҫ���ص���Ϣ����
	CGUID SessionID;
	pMsg->GetGUID(SessionID);

	// root comflag
	char rootComFlag[64];
	memset(rootComFlag, 0, sizeof(rootComFlag));
	pMsg->GetStr(rootComFlag, sizeof(rootComFlag));

	// root guid
	CGUID rootGuid;
	pMsg->GetGUID(rootGuid);

	// root name
	char szRootName[128];
	pMsg->GetStr(szRootName, sizeof(szRootName));

	BYTE rootFindFlag = pMsg->GetByte();

	long bufSize = pMsg->GetLong();
	long pos = 0;
	BYTE* buf = new BYTE[bufSize];
	pMsg->GetEx(buf, bufSize);

	_GetByteFromByteArray(buf, pos, bufSize);
	char szComFlag[128];
	_GetStringFromByteArray(buf, pos, szComFlag, bufSize);
	CGUID leafGuid;
	_GetBufferFromByteArray(buf, pos, leafGuid, bufSize);
	BYTE DbOperFlag = _GetByteFromByteArray(buf, pos, bufSize);
	BYTE DBOperType = _GetByteFromByteArray(buf, pos, bufSize);

 	if(DbOperFlag == 1)
	{
		// ���ж�cdkey�ַ����Ƿ���Ч
		if( (strcmp(rootComFlag, "[account]") == 0) )
		{
			if(szRootName[0] == '\0')
			{
				AddLogText("ProcessMsg:[%d][%d] Account�˺�������Ч��",DbOperFlag,DBOperType);
				SAFE_DELETE_ARRAY(buf);
				return;
			}
		}

		bool isEntityMailGroupFlag = false;
		CEntityGroup* entity = NULL;
		if( (strcmp(rootComFlag, szComFlag) != 0) ) // ���ζ�����ĳ������ӽڵ�
		{
			if( (strcmp(rootComFlag, "[account]") == 0) )
			{
				//������ͨ�����в��ң����û�иø��ڵ�������ʱ��������һ��
				//��MainLoop()����ҪͶ���߳���Ϣʱ�ͷŸ���ʱ����
				entity = FindAccount(string(szRootName));
				if(!entity)
				{
					entity = new Account(rootGuid);
					entity->SetName(szRootName);
					entity->SetIsTempEntityFlag(1);//��ʱ����
					if(!AddAccount((Account*)entity))
					{
						SAFE_DELETE(entity);
					}
				}
			}
			else
			{
				entity = FindRootEntity(rootComFlag, rootGuid);
				if(!entity)
				{
					entity = (CEntityGroup*)NewBaseEntity(rootComFlag, rootGuid);
					entity->SetIsTempEntityFlag(1);//��ʱ����
					AddOneRootEntity(entity);
				}
			}

			if(entity)
			{
				entity->SetName(szRootName);
				if(DBOperType == DB_OPER_LOAD)
					entity->SetCurDbOperType(DB_OPER_LOAD);
				else if(DBOperType == DB_OPER_LOAD_NOCACHE)
					entity->SetCurDbOperType(DB_OPER_LOAD_NOCACHE);
				else
					entity->SetCurDbOperType(DB_OPER_SAVE);
				entity->SetCurDbOperFlag(1);
			}

			CEntityGroup* pFather = FindLeafFatherByComFlag(entity, szComFlag);
			if(pFather)
			{
				if(DBOperType == DB_OPER_LOAD)
					pFather->SetCurDbOperType(DB_OPER_LOAD);
				else if(DBOperType == DB_OPER_LOAD_NOCACHE)
					pFather->SetCurDbOperType(DB_OPER_LOAD_NOCACHE);
				else
					pFather->SetCurDbOperType(DB_OPER_SAVE);
				pFather->SetCurDbOperFlag(1);
			}

			switch(DBOperType)
			{
			case DB_OPER_DELETE_INSERT:
			case DB_OPER_ONLY_MEMSAVE:
				{
					if(pFather)
					{
						pFather->ReleaseLeaf(leafGuid);
						CEntityGroup* pLeaf = (CEntityGroup*)NewBaseEntity(szComFlag, leafGuid);
						pLeaf->SetCurDbOperType(DBOperType);
						pLeaf->SetCurDbOperFlag(DbOperFlag);
						if(DbOperFlag == 1)
							pLeaf->GetFromByteArray(buf, pos, bufSize);
						pFather->AddChild(pLeaf);
						pLeaf->SetSessionID(SessionID);
					}
					else
						AddLogText("ProcMsg DB_OPER_DELETE_INSERT ����ӽ��%sʱδ�ҵ��丸�ڵ�", szComFlag);
				}
				break;
			case DB_OPER_DELETE:
				{
					if(pFather)
					{
						// �ȴ�MainLoop()����ɾ��
						CEntityGroup* pLeaf = (CEntityGroup*)pFather->GetChild(leafGuid);		
						if(pLeaf)
						{
							pLeaf->SetCurDbOperType(DBOperType);
							pLeaf->SetCurDbOperFlag(DbOperFlag);
							if(DbOperFlag == 1)
							{
								pLeaf->GetFromByteArray(buf, pos, bufSize);
								pLeaf->SetIsTempEntityFlag(1);// ���ó���ʱ����
								pLeaf->SetSessionID(SessionID);
								// ���ø��ڵ�ɾ������
								entity->SetDelLeafGuid(leafGuid);
								entity->SetDelLeafFlag(szComFlag);
							}
						}
					}
					else
						AddLogText("ProcMsg DB_OPER_DELETE ����ӽ��%sʱδ�ҵ��丸�ڵ�", szComFlag);
				}
				break;
			case DB_OPER_INSERT:
				{
					if(pFather)
					{
						CEntityGroup* pLeaf = (CEntityGroup*)NewBaseEntity(szComFlag, leafGuid);
						pLeaf->SetCurDbOperType(DBOperType);
						pLeaf->SetCurDbOperFlag(DbOperFlag);
						if(DbOperFlag == 1)
							pLeaf->GetFromByteArray(buf, pos, bufSize);
						pFather->AddChild(pLeaf);
						pLeaf->SetSessionID(SessionID);
					}
					else
						AddLogText("ProcMsg DB_OPER_INSERT ����ӽ��%sʱδ�ҵ��丸�ڵ�", szComFlag);
				}
				break;
			case DB_OPER_SAVE:
				{
					if(pFather)
					{
						CEntityGroup* pLeaf = (CEntityGroup*)pFather->GetChild(leafGuid);
						if(!pLeaf)
						{
							pLeaf = (CEntityGroup*)NewBaseEntity(szComFlag, leafGuid);
							CEntityGroup* pFather = FindLeafFatherByComFlag(entity, szComFlag);
							if(pFather)
								pFather->AddChild(pLeaf);
						}
						if(pLeaf)
						{
							pLeaf->SetCurDbOperType(DBOperType);
							pLeaf->SetCurDbOperFlag(DbOperFlag);
							if(DbOperFlag == 1)
								pLeaf->GetFromByteArray(buf, pos, bufSize);
							pLeaf->SetSessionID(SessionID);
						}
					}
					else
						AddLogText("ProcMsg DB_OPER_SAVE ����ӽ��%sʱδ�ҵ��丸�ڵ�", szComFlag);
				}
				break;
			case DB_OPER_LOAD:
			case DB_OPER_LOAD_NOCACHE:
				{
					if(pFather)
					{
						CEntityGroup* pLeaf = (CEntityGroup*)pFather->GetChild(leafGuid);
						if(pLeaf && !pLeaf->GetIsTempEntityFlag())
						{
							CMessage msg(MSG_DB2W_OPER_ENTITY);
							msg.Add(SessionID);
							msg.Add((long)S_OK);
							msg.Add(rootComFlag);
							msg.Add(rootGuid);
							msg.Add("");
							msg.Add((BYTE)0);
							vector<BYTE> pBA;
							pLeaf->AddToByteArray(pBA, true); // ������
							msg.Add((long)pBA.size());
							if(pBA.size())
								msg.AddEx(&pBA[0], (long)pBA.size());
							msg.SendToSocket(GetGame()->GetWSSocket());
							SAFE_DELETE_ARRAY(buf);
							return;
						}
						else
						{
							pLeaf = (CEntityGroup*)NewBaseEntity(szComFlag, leafGuid);
							CEntityGroup* pFather = FindLeafFatherByComFlag(entity, szComFlag);
							if(pFather)
								pFather->AddChild(pLeaf);
							pLeaf->SetIsTempEntityFlag(1);// ��ʱ����
							pLeaf->SetSessionID(SessionID);
						}

						if(entity)
						{
							// Ͷ��Load����
							ThreadData* loadData = new ThreadData;
							loadData->pEntity = entity;
							PushLoadThreadData(loadData);
							SAFE_DELETE_ARRAY(buf);
						}
					}
					else
						AddLogText("ProcMsg DB_OPER_LOAD ����ӽ��%sʱδ�ҵ��丸�ڵ�", szComFlag);
				}
				break;
			default:
				{
					AddLogText("ProcessMsg(): ���ڵ�%s �ӽ��%s �Ĳ�������%d ��Ч!", 
						rootComFlag, szComFlag, DBOperType);
					SAFE_DELETE_ARRAY(buf);
					return;
				}
				break;
			}
		}
		else // �Ǹ����
		{
			// �Ƿ��ʼ����󣬵�������
			if(strcmp(rootComFlag, "[mailgroup]") == 0)
				isEntityMailGroupFlag = true;
			switch(DBOperType)
			{
			case DB_OPER_DELETE:
				{
					if(DbOperFlag == 1)
					{
						if( (strcmp(rootComFlag, "[account]") == 0) )
						{
							entity = FindAccount(szRootName);
							if(!entity)
							{
								entity = new Account(rootGuid);
								entity->SetName(szRootName);
							}
						}
						else
						{
							entity = FindRootEntity(rootComFlag, rootGuid);
							if(!entity)
							{
								if(!isEntityMailGroupFlag)
									entity = CreateRootEntity(rootComFlag, rootGuid);
								else
									entity = (CEntityGroup*)NewBaseEntity(rootComFlag, rootGuid);
							}
						}
						if(entity)
						{
							entity->GetFromByteArray(buf, pos, bufSize);
							entity->SetCurDbOperType(DBOperType);
							entity->SetCurDbOperFlag(DbOperFlag);
							entity->SetIsTempEntityFlag(1);
						}
					}
				}
				break;
			case DB_OPER_DELETE_INSERT:
			case DB_OPER_ONLY_MEMSAVE:
				{
					if(DbOperFlag == 1)
					{
						if( (strcmp(rootComFlag, "[account]") == 0) )
						{
							entity = FindAccount(szRootName);
							if(!entity)
							{
								entity = new Account(rootGuid);
								entity->SetName(szRootName);
								if(!AddAccount((Account*)entity))
								{
									SAFE_DELETE(entity);
								}
							}
						}
						else
						{
							entity = FindRootEntity(rootComFlag, rootGuid);
							if(!entity)
							{
								if(!isEntityMailGroupFlag)
								{
									entity = CreateRootEntity(rootComFlag, rootGuid);
									if( strcmp(rootComFlag, "[friendgroup]")==0 && entity )
										entity->SetIsTempEntityFlag(1);
								}
								else
								{
									entity = (CEntityGroup*)NewBaseEntity(rootComFlag, rootGuid);
									entity->SetIsTempEntityFlag(1);
								}
							}
						}
						if(entity)
						{
							entity->ReleaseChilds();
							entity->GetFromByteArray(buf, pos, bufSize);
							entity->SetCurDbOperType(DBOperType);
							entity->SetCurDbOperFlag(DbOperFlag);
						}
					}
				}
				break;
			case DB_OPER_INSERT:
				{
					if(DbOperFlag == 1)
					{
						if( (strcmp(rootComFlag, "[account]") == 0) )
						{
							entity = FindAccount(szRootName);
							if(!entity)
							{
								entity = new Account(rootGuid);
								entity->SetName(szRootName);
								if(!AddAccount((Account*)entity))
								{
									SAFE_DELETE(entity);
								}
							}
						}
						else
						{
							entity = FindRootEntity(rootComFlag, rootGuid);

							if(!entity)
							{
								if(!isEntityMailGroupFlag)
									entity = CreateRootEntity(rootComFlag, rootGuid);
								else
								{
									entity = (CEntityGroup*)NewBaseEntity(rootComFlag, rootGuid);
									entity->SetIsTempEntityFlag(1);
								}
							}
						}
						if(entity)
						{
							entity->GetFromByteArray(buf, pos, bufSize);
							entity->SetCurDbOperType(DBOperType);
							entity->SetCurDbOperFlag(DbOperFlag);
						}
					}
				}
				break;
			case DB_OPER_LOAD:
			case DB_OPER_LOAD_NOCACHE:
				{
 					if(DbOperFlag == 1)
					{
						if( (strcmp(rootComFlag, "[account]") == 0) )
						{
							entity = FindAccount(szRootName);
							if(!entity)
							{
								entity = new Account(rootGuid);
								entity->SetName(szRootName);
								entity->SetIsTempEntityFlag(1);
							
								entity->GetFromByteArray(buf, pos, bufSize);
								entity->SetCurDbOperType(DBOperType);
								entity->SetCurDbOperFlag(DbOperFlag);
								entity->SetSessionID(SessionID);
								// Ͷ��Load����
								ThreadData* loadData = new ThreadData;
								loadData->pEntity = entity;
								PushLoadThreadData(loadData);
								SAFE_DELETE_ARRAY(buf);
								return;
							}
						}
						else
						{
							if(DB_OPER_LOAD_NOCACHE != DBOperType)
								entity = FindRootEntity(rootComFlag, rootGuid);

							if(!entity)
							{
								entity = (CEntityGroup*)NewBaseEntity(rootComFlag, rootGuid);
								entity->SetIsTempEntityFlag(1);
								entity->GetFromByteArray(buf, pos, bufSize);
								entity->SetCurDbOperType(DBOperType);
								entity->SetCurDbOperFlag(DbOperFlag);
								entity->SetSessionID(SessionID);
								// Ͷ��Load����
								ThreadData* loadData = new ThreadData;
								loadData->pEntity = entity;
								if(!isEntityMailGroupFlag) // ������ʼ�Load��ͨ���˴�ֱ��Ͷ���߳�
									PushLoadThreadData(loadData);
								else
									PushMailThreadData(loadData);
								SAFE_DELETE_ARRAY(buf);
								return;
							}
						}

						if(entity)
						{
							if(!isEntityMailGroupFlag && !entity->GetIsTempEntityFlag())
							{
								CMessage msg(MSG_DB2W_OPER_ENTITY);
								msg.Add(SessionID);
								msg.Add((long)S_OK);
								msg.Add(rootComFlag);
								msg.Add(rootGuid);

								if( (strcmp(rootComFlag, "[account]") == 0) )
								{
									msg.Add(szRootName);
									msg.Add((BYTE)1);

									// ������˺ű�Load����ȡ���䶨ʱɾ��
									((Account*)entity)->SetLastLoginTime(-1);
								}
								else
								{
									msg.Add("");
									msg.Add((BYTE)0);
								}

								vector<BYTE> pBA;
								entity->AddToByteArray(pBA, true); // ������
								msg.Add((long)pBA.size());
								if(pBA.size())
									msg.AddEx(&pBA[0], (long)pBA.size());

								msg.SendToSocket(GetGame()->GetWSSocket());
								SAFE_DELETE_ARRAY(buf);
								return;
							}
						}
					}
				}
				break;
			case DB_OPER_SAVE:
				{	
					if(DbOperFlag == 1)
					{
						if( (strcmp(rootComFlag, "[account]") == 0) )
						{
							entity = FindAccount(szRootName);
							if(!entity)
							{
								entity = new Account(rootGuid);
								entity->SetName(szRootName);
								if(!AddAccount((Account*)entity))
								{
									SAFE_DELETE(entity);
								}
								if(entity)
									entity->SetIsTempEntityFlag(1);
							}
						}
						else
						{
							entity = FindRootEntity(rootComFlag, rootGuid);
							
							if(!entity)
							{
								if(!isEntityMailGroupFlag)
									entity = CreateRootEntity(rootComFlag, rootGuid);
								else
								{
									entity = (CEntityGroup*)NewBaseEntity(rootComFlag, rootGuid);
									entity->SetIsTempEntityFlag(1);
								}
							}
						}
						if(entity)
						{
							entity->GetFromByteArray(buf, pos, bufSize);
							entity->SetCurDbOperType(DBOperType);
							entity->SetCurDbOperFlag(DbOperFlag);
						}
					}
				}
				break;
			default: // Procedure
				{	
					if(DBOperType==DB_OPER_INIT || DBOperType==DB_OPER_REGISTER || DBOperType==DB_OPER_UNREGISTER)
					{
						AddLogText("ProceMsg:%s type %d is err!", rootComFlag, DBOperType);
					}
					else
					{
						if(DbOperFlag == 1)
						{
							entity = (CEntityGroup*)NewBaseEntity(rootComFlag, rootGuid);
							if(entity)
							{
								entity->GetFromByteArray(buf, pos, bufSize);
								entity->SetCurDbOperType(DBOperType);
								entity->SetCurDbOperFlag(DbOperFlag);
								entity->SetIsTempEntityFlag(1);
								entity->SetSessionID(SessionID);
								if(DBOperType == DB_OPER_INSERTMAIL_PROCEDURE) // �߳����ȴ���
								{
									// Ͷ��Mail����
									ThreadData* loadData = new ThreadData;
									loadData->pEntity = entity;
									PushMailThreadData(loadData);
									SAFE_DELETE_ARRAY(buf);
									return;
								}
								else
								{
									// Ͷ�ݶ���
									ThreadData* loadData = new ThreadData;
									loadData->pEntity = entity;
									PushLoadThreadData(loadData);
									SAFE_DELETE_ARRAY(buf);
									return;
								}
							}
						}
					}
				}
				break;
			}
		}

		if(entity)
		{
			entity->SetSessionID(SessionID);

			if(strcmp(rootComFlag, "[account]") == 0)
			{
				AddDirtyEntityName(szRootName);
			}
			else
			{
				if(isEntityMailGroupFlag) // ���ʼ�����
				{
					// Ͷ��Load����
					ThreadData* loadData = new ThreadData;
					loadData->pEntity = entity;
					PushMailThreadData(loadData);
				}
				else
				{
					AddDirtyEntityGuid(rootGuid);
				}
			}
		}
	}
	SAFE_DELETE_ARRAY(buf);
}

// ��������
CEntityGroup* CEntityManager::CreateRootEntity(const string& flag, const CGUID& guid)
{
	CEntityGroup* retEntity = new CEntityGroup(flag, guid);
	// Ĭ�ϴ˹������Ϊroot
	if(retEntity)
	{
		AddOneRootEntity(retEntity);
	}
	return retEntity;
}
// interfaces

//!	ѹ��һ��ThreadData
bool CEntityManager::PushMailThreadData(ThreadData* loadData)
{
	EnterCriticalSection(&m_MailCriticalSection);

	if(m_listMailThreadData.size() == 0)
	{
		// �����¼�
		CEntityManager::SetMailThreadEvent();
	}

	m_listMailThreadData.push_back(loadData);
	lMails = m_listMailThreadData.size();
	LeaveCriticalSection(&m_MailCriticalSection);
	return true;
}
bool CEntityManager::PushMailThreadData(list<ThreadData*>& loadDataList)
{
	EnterCriticalSection(&m_MailCriticalSection);

	if(m_listMailThreadData.size() == 0)
	{
		// �����¼�
		CEntityManager::SetMailThreadEvent();
	}

	m_listMailThreadData.insert(m_listMailThreadData.end(), loadDataList.begin(), loadDataList.end());
	lMails = m_listMailThreadData.size();
	LeaveCriticalSection(&m_MailCriticalSection);
	return true;
}
//!	ȡ��һ��ThreadData
bool CEntityManager::GetMailThreadData(list<ThreadData*>& loadDataList, long dwThreadID)
{
	EnterCriticalSection(&m_MailCriticalSection);
	while( m_listMailThreadData.size() == 0)
	{	
		CEntityManager::ResetMailThreadEvent();
		//���û�в��������ȴ���
		LeaveCriticalSection(&m_MailCriticalSection);

		CEntityManager::WaitMailThreadEvent();	
		//�ȴ�֪ͨ�¼�
		EnterCriticalSection(&m_MailCriticalSection);
	}

	loadDataList = m_listMailThreadData;
	m_listMailThreadData.clear();
	lMails = 0;
	LeaveCriticalSection(&m_MailCriticalSection);
	return true;
}
bool CEntityManager::SetMailThreadData(list<ThreadData*>& loadDataList)
{
	EnterCriticalSection(&m_RetMailCriticalSection);
	m_listMailThreadRetData = loadDataList;
	LeaveCriticalSection(&m_RetMailCriticalSection);
	return true;
}
long CEntityManager::GetSaveThreadOperDataSize(list<CGUID>& saveBeginGuidList)
{
	long lRet = 0;
	EnterCriticalSection(&m_SaveCriticalSection);
	saveBeginGuidList = m_listSaveBeginEntityGuid;
	lRet = m_listSaveThreadOperData.size();
	LeaveCriticalSection(&m_SaveCriticalSection);
	return lRet;
}
//!	ѹ��һ��ThreadData
bool CEntityManager::PushLoadThreadData(ThreadData* loadData)
{
	EnterCriticalSection(&m_LoadCriticalSection);

	if(m_listLoadThreadOperData.size() == 0)
	{
		// �����¼�
		CEntityManager::SetLoadThreadEvent();
	}
	if(loadData && loadData->pEntity)
	{
		m_listLoadThreadOperData.push_front(loadData);
		lOperingLoadEntitys = m_listLoadThreadOperData.size();
		m_listLoadBeginEntityGuid.push_front(loadData->pEntity->GetGUID());
		lBeginLoadNum = m_listLoadBeginEntityGuid.size();
	}
	LeaveCriticalSection(&m_LoadCriticalSection);
	return true;
}
bool CEntityManager::PushSaveThreadData(ThreadData* loadData)
{
	EnterCriticalSection(&m_SaveCriticalSection);

	if(m_listSaveThreadOperData.size() == 0)
	{
		// �����¼�
		CEntityManager::SetSaveThreadEvent();
	}

	if(loadData && loadData->pEntity)
	{
		m_listSaveThreadOperData.push_back(loadData);
		lOperingSaveEntitys = m_listSaveThreadOperData.size();
		m_listSaveBeginEntityGuid.push_back(loadData->pEntity->GetGUID());	
		lBeginSaveNum = m_listSaveBeginEntityGuid.size();
	}
	LeaveCriticalSection(&m_SaveCriticalSection);
	return true;
}

bool CEntityManager::PushLoadThreadData(list<ThreadData*>& loadDataList)
{
	EnterCriticalSection(&m_LoadCriticalSection);

	if(m_listLoadThreadOperData.size() == 0)
	{
		// �����¼�
		CEntityManager::SetLoadThreadEvent();
	}

	list<ThreadData*>::iterator itr = loadDataList.begin();
	for(; itr != loadDataList.end(); itr++)
	{
		if((*itr) && (*itr)->pEntity)
			m_listLoadBeginEntityGuid.push_back((*itr)->pEntity->GetGUID());	
	}
	lBeginLoadNum = m_listLoadBeginEntityGuid.size();
	m_listLoadThreadOperData.insert(m_listLoadThreadOperData.end(), loadDataList.begin(), loadDataList.end());
	LeaveCriticalSection(&m_LoadCriticalSection);
	return true;
}

bool CEntityManager::PushSaveThreadData(list<ThreadData*>& saveDataList)
{
	EnterCriticalSection(&m_SaveCriticalSection);

	if(m_listSaveThreadOperData.size() == 0)
	{
		// �����¼�
		CEntityManager::SetSaveThreadEvent();
	}

	list<ThreadData*>::iterator itr = saveDataList.begin();
	for(; itr != saveDataList.end(); itr++)
	{
		if((*itr) && (*itr)->pEntity)
			m_listSaveBeginEntityGuid.push_back((*itr)->pEntity->GetGUID());	
	}
	lBeginSaveNum = m_listSaveBeginEntityGuid.size();
	m_listSaveThreadOperData.insert(m_listSaveThreadOperData.end(), saveDataList.begin(), saveDataList.end());
	LeaveCriticalSection(&m_SaveCriticalSection);
	return true;
}

//!	����һ��ThreadData
bool CEntityManager::GetThreadData(ThreadData** loadData, long dwThreadID, eThreadType type)
{
	if(type == eThreadLoadType)
	{
		EnterCriticalSection(&m_LoadCriticalSection);
		while( m_listLoadThreadOperData.size() == 0 )
		{	
			CEntityManager::ResetLoadThreadEvent();
			//���û�в��������ȴ���
			LeaveCriticalSection(&m_LoadCriticalSection);

			CEntityManager::WaitLoadThreadEvent();	
			//�ȴ�֪ͨ�¼�
			EnterCriticalSection(&m_LoadCriticalSection);
		}

		if(m_listLoadThreadOperData.size())
		{
			*loadData = (*m_listLoadThreadOperData.begin());
			m_listLoadThreadOperData.erase(m_listLoadThreadOperData.begin());
		}
		
		LeaveCriticalSection(&m_LoadCriticalSection);
	}
	else if(type == eThreadSaveType)
	{
		EnterCriticalSection(&m_SaveCriticalSection);
		while( m_listSaveThreadOperData.size() == 0 )
		{	
			CEntityManager::ResetSaveThreadEvent();
			//���û�в��������ȴ���
			LeaveCriticalSection(&m_SaveCriticalSection);

			CEntityManager::WaitSaveThreadEvent();	
			//�ȴ�֪ͨ�¼�
			EnterCriticalSection(&m_SaveCriticalSection);
		}

		if(m_listSaveThreadOperData.size())
		{
			*loadData = (*m_listSaveThreadOperData.begin());
			m_listSaveThreadOperData.erase(m_listSaveThreadOperData.begin());
		}
		
		LeaveCriticalSection(&m_SaveCriticalSection);
	}
	else
	{
		AddLogText("GetThreadData() thread[%d] type is Init!", dwThreadID);
	}
	return true;
}
//! ����һ�����ڱ��̴߳����guid
bool CEntityManager::PopBeginEntityGuid(const CGUID& guid, eThreadType type)
{
	if(type == eThreadLoadType)
	{
		EnterCriticalSection(&m_LoadCriticalSection);
		list<CGUID>::iterator itr = std::find(m_listLoadBeginEntityGuid.begin(), m_listLoadBeginEntityGuid.end(), guid);
		if(itr != m_listLoadBeginEntityGuid.end())
			m_listLoadBeginEntityGuid.erase(itr);
		lBeginLoadNum = m_listLoadBeginEntityGuid.size();
		LeaveCriticalSection(&m_LoadCriticalSection);
	}
	else if(type == eThreadSaveType)
	{
		EnterCriticalSection(&m_SaveCriticalSection);
		list<CGUID>::iterator itr = std::find(m_listSaveBeginEntityGuid.begin(), m_listSaveBeginEntityGuid.end(), guid);
		if(itr != m_listSaveBeginEntityGuid.end())
			m_listSaveBeginEntityGuid.erase(itr);
		lBeginSaveNum = m_listSaveBeginEntityGuid.size();
		LeaveCriticalSection(&m_SaveCriticalSection);
	}
	else
	{
		AddLogText("PopBeginEntityGuid() thread type is Init!");
	}
	
	return true;
}

bool CEntityManager::PushRetThreadData(ThreadData* loadData)
{
	EnterCriticalSection(&m_RetCriticalSection);
	if(loadData && loadData->pEntity)
	{
		if(loadData->pEntity->GetCurDbOperType() == DB_OPER_LOAD || loadData->pEntity->GetCurDbOperType() == DB_OPER_LOAD_NOCACHE)
			m_listThreadRetData.push_front(loadData);
		else
			m_listThreadRetData.push_back(loadData);
	}
	LeaveCriticalSection(&m_RetCriticalSection);
	return true;
}
bool CEntityManager::GetRetThreadData(list<ThreadData*>& loadDataList)
{
	EnterCriticalSection(&m_RetCriticalSection);
	loadDataList = m_listThreadRetData;
	m_listThreadRetData.clear();
	LeaveCriticalSection(&m_RetCriticalSection);
	return true;
}
void CEntityManager::SetLoadThreadEvent(void)
{
	SetEvent(m_hLoadThreadEvent);
}
void CEntityManager::WaitLoadThreadEvent(void)
{
	WaitForSingleObject(m_hLoadThreadEvent, INFINITE);
}

void CEntityManager::ResetLoadThreadEvent(void)
{
	ResetEvent(m_hLoadThreadEvent);
}

void CEntityManager::SetSaveThreadEvent(void)
{
	SetEvent(m_hSaveThreadEvent);
}
void CEntityManager::WaitSaveThreadEvent(void)
{
	WaitForSingleObject(m_hSaveThreadEvent, INFINITE);
}

void CEntityManager::ResetSaveThreadEvent(void)
{
	ResetEvent(m_hSaveThreadEvent);
}

//!								����SaveEvent
void CEntityManager::SetMailThreadEvent(void)
{
	SetEvent(m_hMailThreadEvent);
}
//!								�ȴ�SaveEvent
void CEntityManager::WaitMailThreadEvent(void)
{
	WaitForSingleObject(m_hMailThreadEvent, INFINITE);
}
void CEntityManager::ResetMailThreadEvent(void)
{
	ResetEvent(m_hMailThreadEvent);
}

//! ѹ��һ���߳��Ƴ��¼����
bool CEntityManager::PushThreadQuitEventHandle(HANDLE& handle)
{
	EnterCriticalSection(&m_ThreadQuitCS);
	m_vecThreadQuitEvent.push_back(handle);
	LeaveCriticalSection(&m_ThreadQuitCS);
	return true;
}
//
bool CEntityManager::AddMailThreadQuitEventHandle(HANDLE& handle)
{	
	m_MailThreadQuitEvent = handle;
	return true;
}
// ���ر��ֶ�����
void CEntityManager::LoadObjInfo(vector<const char*>& AttrInfoList,
								 const char* szStructInfo,
								 const char* szTable)
{
	vector<const char*>::iterator itr = AttrInfoList.begin();
	string strNode = *itr;
	long lPos = strNode.find('[');
	strNode.erase(0, lPos);
	lPos = strNode.find('.');
	strNode.erase(lPos, strNode.size());

	for( ; itr != AttrInfoList.end(); ++itr )
	{
		LoadObjAttrInfo(*itr);
	}
	LoadObjStructInfo(szStructInfo);

	// ���������
	_RecordsetPtr rsPtr(0);
	if(m_pAccessThread)
	{
		char szSql[1024];
		sprintf(szSql, "SELECT * FROM %s",
			szTable);

		try
		{
			CMyAdoBase::CreateRs(rsPtr);
			CMyAdoBase::OpenRs(szSql, rsPtr, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(rsPtr);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(szTable, strNode);
		}
		
	}
}

// ��ʼ����������
void CEntityManager::InitPropertyIniData()
{
	// ��ʼ��loginplayer����
	LoadObjAttrInfo("structinfo\\[loginplayer].ini");
	LoadObjAttrInfo("structinfo\\[loginplayergroup].ini");

	//// ��ʼ��account����
	LoadObjAttrInfo("structinfo\\[account].ini");
	LoadObjStructInfo("structinfo\\[account_build].ini");

	// ���������
	_RecordsetPtr rsacc(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_PLAYER_BASE");

		try
		{
			CMyAdoBase::CreateRs(rsacc);
			CMyAdoBase::OpenRs(sql, rsacc, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(rsacc);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_PLAYER_BASE"), string("[loginplayer]"));
		}
		
	}

	// ��ʼ��goods����
	LoadObjAttrInfo("structinfo\\[goods].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_depot].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_depotgold].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_depotsecondary].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_depotsilver].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_equip].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_gold].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_pack].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_packet].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_silver].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subdepotone].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subdepotfour].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subdepotthree].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subdepottwo].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subpackfive].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subpackfour].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subpackone].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subpackthree].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_subpacktwo].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_businesspack].ini");
	LoadObjAttrInfo("structinfo\\goodsgroup_leaves\\[gp_businesspack_field].ini");
	LoadObjAttrInfo("structinfo\\[goodsgroup].ini");
	LoadObjStructInfo("structinfo\\[goodsgroup_build].ini");
	// ���������
	_RecordsetPtr goodsrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_PLAYER_GOODS");
		try
		{
			CMyAdoBase::CreateRs(goodsrs);
			CMyAdoBase::OpenRs(sql, goodsrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(goodsrs);
			
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_PLAYER_GOODS"), string("[goods]"));
		}
		
	}

	// ��������
	LoadObjAttrInfo("structinfo\\[player_friend].ini");
	LoadObjAttrInfo("structinfo\\[playerfriendgroup].ini");
	LoadObjStructInfo("structinfo\\[playerfriendgroup_build].ini");
	LoadObjAttrInfo("structinfo\\[friendgroup].ini");
	LoadObjStructInfo("structinfo\\[friendgroup_build].ini");
	//CreateRootEntity("friendgroup", NULL_GUID);
	_RecordsetPtr playerFriendRs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_PLAYER_FRIEND");
		try
		{
			CMyAdoBase::CreateRs(playerFriendRs);
			CMyAdoBase::OpenRs(sql, playerFriendRs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(playerFriendRs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_PLAYER_FRIEND"), string("[player_friend]"));
		}
		
	}

	LoadObjAttrInfo("structinfo\\[playerfriendgroupOwner].ini");
	LoadObjAttrInfo("structinfo\\[playerfriendgroupAim].ini");

	// ��������
	LoadObjAttrInfo("structinfo\\[player_sprite].ini");
	LoadObjAttrInfo("structinfo\\[playerspritegroup].ini");
	_RecordsetPtr playerSpriteRs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_PLAYER_SPRITE");
		try
		{
			CMyAdoBase::CreateRs(playerSpriteRs);
			CMyAdoBase::OpenRs(sql, playerSpriteRs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(playerSpriteRs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_PLAYER_SPRITE"), string("[player_sprite]"));
		}
		
	}

	// Fox@20081218 �����ṹ
	vector<const char*> AttrInfoList;
	AttrInfoList.push_back("structinfo/[pet].ini");
	AttrInfoList.push_back("structinfo/[playerpetlist].ini");
	LoadObjInfo(AttrInfoList, NULL, "TBL_PET");
	AttrInfoList.clear();

	// ��ҵϵͳ��ṹ
	AttrInfoList.push_back( "structinfo/business/[trade_spot].ini" );
	LoadObjInfo( AttrInfoList, NULL, "TBL_TRADE_SPOT" );
	LoadObjAttrInfo("structinfo/business/[trade_spot_group].ini");
	AttrInfoList.clear();

	// ��ʼ��Faction_Apply����
	LoadObjAttrInfo("structinfo\\[faction_apply].ini");
	LoadObjAttrInfo("structinfo\\[factionapplygroup].ini");
	// ���������
	_RecordsetPtr facApplyRs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_FACTION_APPLY");
		try
		{
			CMyAdoBase::CreateRs(facApplyRs);
			CMyAdoBase::OpenRs(sql, facApplyRs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(facApplyRs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_FACTION_APPLY"), string("[faction_apply]"));
		}
		
	}

	// ��ʼ��Faction_Member����
	LoadObjAttrInfo("structinfo\\[faction_member].ini");
	LoadObjAttrInfo("structinfo\\[factionmembergroup].ini");
	// ���������
	_RecordsetPtr facMembersRs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_FACTION_MEMBER");
		try
		{
			CMyAdoBase::CreateRs(facMembersRs);
			CMyAdoBase::OpenRs(sql, facMembersRs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(facMembersRs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_FACTION_MEMBER"), string("[faction_member]"));
		}
		
	}

	// ���������
	LoadObjAttrInfo("structinfo\\[faction].ini");
	LoadObjAttrInfo("structinfo\\[factiongroup].ini");
	LoadObjStructInfo("structinfo\\[faction_build].ini");
	_RecordsetPtr facBaseDataRs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_FACTION_BASE");
		try
		{
			CMyAdoBase::CreateRs(facBaseDataRs);
			CMyAdoBase::OpenRs(sql, facBaseDataRs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(facBaseDataRs);
			
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_FACTION_BASE"), string("[faction]"));
		}
		
	}

	// ��ʼ��Faction_Member����
	LoadObjAttrInfo("structinfo\\[union_member].ini");
	LoadObjAttrInfo("structinfo\\[union_member_group].ini");
	// ���������
	_RecordsetPtr UnionMembersRs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_UNION_MEMBER");
		try
		{
			CMyAdoBase::CreateRs(UnionMembersRs);
			CMyAdoBase::OpenRs(sql, UnionMembersRs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(UnionMembersRs);
			
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_UNION_MEMBER"), string("[union_member]"));
		}
		
	}

	// ���������
	LoadObjAttrInfo("structinfo\\[union].ini");
	LoadObjAttrInfo("structinfo\\[union_group].ini");
	LoadObjStructInfo("structinfo\\[union_build].ini");
	_RecordsetPtr UnionBaseDataRs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_UNION_BASE");
		try
		{
			CMyAdoBase::CreateRs(UnionBaseDataRs);
			CMyAdoBase::OpenRs(sql, UnionBaseDataRs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(UnionBaseDataRs);
			
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_UNION_BASE"), string("[union]"));
		}
		
	}


	// ��ʼ��mailgoods����
	LoadObjAttrInfo("structinfo\\[mailgoods].ini");
	LoadObjAttrInfo("structinfo\\[mailgoodsgroup].ini");
	// ���������
	_RecordsetPtr mailgoodsrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_MAIL_GOODS");
		try
		{
			CMyAdoBase::CreateRs(mailgoodsrs);
			CMyAdoBase::OpenRs(sql, mailgoodsrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(mailgoodsrs);
			
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_MAIL_GOODS"), string("[mailgoods]"));
		}
		
	}

	// ��ʼ��mail����
	LoadObjAttrInfo("structinfo\\[mail].ini");
	LoadObjAttrInfo("structinfo\\[mailgroup].ini");
	LoadObjStructInfo("structinfo\\[mail_build].ini");

	// ���������
	_RecordsetPtr mailrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_MAIL");
		try
		{
			CMyAdoBase::CreateRs(mailrs);
			CMyAdoBase::OpenRs(sql, mailrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(mailrs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_MAIL"), string("[mail]"));
		}
		
	}


	// ���������
	_RecordsetPtr rs(0);
	LoadObjAttrInfo("structinfo\\[player].ini");
	LoadObjStructInfo("structinfo\\[player_build].ini");
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_PLAYER_DETAIL");
		try
		{
			CMyAdoBase::CreateRs(rs);
			CMyAdoBase::OpenRs(sql, rs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(rs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_PLAYER_DETAIL"), string("[player]"));
		}
		
	}

	LoadObjAttrInfo("structinfo\\[country].ini");
	LoadObjAttrInfo("structinfo\\[countrygroup].ini");
	// ���������
	_RecordsetPtr countrysrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_COUNTRY");
		try
		{
			CMyAdoBase::CreateRs(countrysrs);
			CMyAdoBase::OpenRs(sql, countrysrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(countrysrs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_COUNTRY"), string("[country]"));
		}
		
	}


	// ���������
	LoadObjAttrInfo("structinfo\\[region].ini");
	LoadObjAttrInfo("structinfo\\[regiongroup].ini");
	_RecordsetPtr regionsrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_REGION");
		try
		{
			CMyAdoBase::CreateRs(regionsrs);
			CMyAdoBase::OpenRs(sql, regionsrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(regionsrs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_REGION"), string("[region]"));
		}
		
	}

	// ��ʼ��Global Var����
	LoadObjAttrInfo("structinfo\\[scriptvar].ini");
	LoadObjAttrInfo("structinfo\\[scriptvargroup].ini");

	// ���������
	_RecordsetPtr varrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_SCRIPTVAR");
		try
		{
			CMyAdoBase::CreateRs(varrs);
			CMyAdoBase::OpenRs(sql, varrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(varrs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_SCRIPTVAR"), string("[scriptvar]"));
		}
		
	}

	// ��ʼ�����˷��ݶ���
	LoadObjAttrInfo("structinfo\\[phrgngoods].ini");
	LoadObjAttrInfo("structinfo\\[phrgngoodsgroup].ini");
	// ���������
	_RecordsetPtr phgoodsrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_DUPRGN_GOODS");
		try
		{
			CMyAdoBase::CreateRs(phgoodsrs);
			CMyAdoBase::OpenRs(sql, phgoodsrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(phgoodsrs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_DUPRGN_GOODS"), string("[phrgngoods]"));
		}
		
	}

	LoadObjAttrInfo("structinfo\\[phrgn].ini");
	LoadObjAttrInfo("structinfo\\[phrgngroup].ini");
	LoadObjStructInfo("structinfo\\[phrgn_build].ini");
	// ���������
	_RecordsetPtr phrs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_DUPRGN_BASE");
		try
		{
			CMyAdoBase::CreateRs(phrs);
			CMyAdoBase::OpenRs(sql, phrs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(phrs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_DUPRGN_BASE"), string("[phrgn]"));
		}
		
	}

	LoadObjAttrInfo("structinfo\\[limit_goods_record].ini");	
	LoadObjAttrInfo("structinfo\\[limit_goods_record_group].ini");
	// ���������
	_RecordsetPtr limit_goods_record_rs(0);
	if(m_pAccessThread)
	{
		char sql[1024];
		sprintf(sql,"SELECT count(*) FROM TBL_LIMIT_GOODS_RECORD");
		try
		{
			CMyAdoBase::CreateRs(limit_goods_record_rs);
			CMyAdoBase::OpenRs(sql, limit_goods_record_rs, m_pAccessThread->GetConnPtr()); // ������
			CMyAdoBase::ReleaseRs(limit_goods_record_rs);
		}
		catch(_com_error& e)
		{
			if(e.Error() == 0x80040E37) // DB_E_NOTABLE
				CreateOtherTableInDb(string("TBL_LIMIT_GOODS_RECORD"), string("[limit_goods_record]"));
		}
	}

	LoadObjAttrInfo("structinfo\\[procedure].ini");
	LoadProcedureInfo("Procedure\\CreatePlayerCheckProc.proc");
	LoadProcedureInfo("Procedure\\PlayerNameCheckProc.proc");
	LoadProcedureInfo("Procedure\\CdkeyCheckProc.proc");
	LoadProcedureInfo("Procedure\\SystemDeleteMailCheckProc.proc");
	LoadProcedureInfo("Procedure\\QueryPHRgnOwnerNameProc.proc");
	LoadProcedureInfo("Procedure\\InsertMailProc.proc");
	LoadProcedureInfo("Procedure\\DelRoleMailProc.proc");
	LoadProcedureInfo("Procedure\\ChangePlayerNameProc.proc");

	// �Զ�ִ��һ�������ʱ�ʼ��Ĵ洢����
	RunDeleteTimeoutMailProc();
	// ����ָ���洢����
	// TBL_PLAYER_BASE
	CBaseEntity* pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[loginplayer]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);

	// TBL_PLAYER_DETAIL
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[player]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[player_friend]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[player_sprite]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[region]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);

	//TBL_PLAYER_GOODS
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[goods]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);

	//
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[faction]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);

	//
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[faction_apply]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[faction_member]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[mail]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);

	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[mailgoods]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);

	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[pet]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);

	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgn]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[phrgngoods]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[scriptvar]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[union]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[union_member]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[trade_spot]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
	
	pRoot = GetGame()->GetEntityManager()->NewBaseEntity(string("[limit_goods_record]"), NULL_GUID);
	CreateTblProcedure(pRoot);
	SAFE_DELETE(pRoot);
}

tagEntityBuildInfo::tagEntityBuildInfo()
: lDbQueryType(-1),
lComType(0),strComFlag(""),lHeight(0),
guid(NULL_GUID),strTblName(""),lDbOperFlag(0),lLeafNum(0),
strPrimarykey(""),lHhasAttrFlag(0),lLeafComType(0),
strLeafComFlag(""),lHasDetailLeaves(0),strDbQueryName("")
{
	memset(pQueryVar, 0, sizeof(pQueryVar));
	pDataMgr.clear();
}

tagEntityBuildInfo::~tagEntityBuildInfo()
{
	pDataMgr.clear();
	map<string, tagBuildFileInfo*>::iterator itr = mapBuildFileInfo.begin();
	for(; itr != mapBuildFileInfo.end(); itr++)
		SAFE_DELETE(itr->second);
}

void CEntityManager::LoadObjStructInfo(const char* szPath)
{
	if(NULL == szPath) return;

	CRFile* prfile = rfOpen(szPath);
	if( prfile )
	{
		stringstream stream;
		prfile->ReadToStream(stream);
		rfClose(prfile);

		tagEntityBuildInfo* entityBuildInfo = NULL;
		string deManagerName = szPath;

		// ȥ���ļ���·��
		long pos = deManagerName.find('[');
		deManagerName.erase(0, pos);
		pos = deManagerName.find('_');
		deManagerName.erase(pos, deManagerName.size());
		deManagerName.append("]");
		std::map<string, tagEntityBuildInfo*>::iterator itr = m_mapObjAttrDef.find(deManagerName);
		if(itr == m_mapObjAttrDef.end())
		{
			AddLogText("δ�ȴ����ö���[%s]�����!",deManagerName.c_str());
			return;
		}

		entityBuildInfo = itr->second;

		string tmp;
		// ���߶�
		ReadTo(stream, "#");
		stream >> tmp >> entityBuildInfo->lHeight;

		// Ҷ�Ӹ���
		ReadTo(stream, "#");
		stream >> tmp >> entityBuildInfo->lLeafNum;

		for(int i = 0; i < entityBuildInfo->lLeafNum; i++)
		{
			ReadTo(stream, "#");

			// �ýڵ�GUID
			string szChildGuid;
			// ���ڵ�GUID
			string szFatherGuid;

			long value = 0;
			tagBuildFileInfo* tFileInfo = new tagBuildFileInfo;
			stream 
				>> szChildGuid 
				>> tFileInfo->lLevel 
				>> tFileInfo->lComType 
				>> tFileInfo->strLeafComFlag 
				>> tFileInfo->strRootComFlag 
				>> szFatherGuid
				;

			CGUID childGUID(szChildGuid.c_str());
			CGUID fatherGUID(szFatherGuid.c_str());
			tFileInfo->leafGuid = childGUID;
			tFileInfo->rootGuid = fatherGUID;

			entityBuildInfo->mapBuildFileInfo[tFileInfo->strLeafComFlag] = tFileInfo;
		}
		return;
	}
}

const char* CEntityManager::LoadObjAttrInfo(const char* szPath)
{
	CRFile* prfile = NULL;
	if( NULL == szPath ||
		!(prfile = rfOpen(szPath)) ) 
		return NULL;

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	string deManagerName = szPath;

	tagEntityBuildInfo* entityBuildInfo = new tagEntityBuildInfo;

	string tmp;
	//��������
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->lComType;

	//ӵ�ж����־
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->strComFlag;

	// ����GUID
	string szGuid;
	ReadTo(stream, "#");
	stream >> tmp >> szGuid;
	CGUID tmpGuid(szGuid.c_str());
	entityBuildInfo->guid = tmpGuid;		

	// ���ݿ����
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->strTblName;

	// ���ݿ������־
	ReadTo(stream, "#");
	long lDbOperFlag = 0;
	stream >> tmp >> lDbOperFlag;

	// MEM������
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->strPrimarykey;

	// DB������
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->strDBPrimarykey;

	// �Ƿ�������
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->lHhasAttrFlag;

	// �ӽڵ�����
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->lLeafComType;

	// �ӽڵ�����
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->strLeafComFlag;

	// �Ƿ��з�ɢ�ڵ�
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->lHasDetailLeaves;

	// ���ݿ���ұ�����
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->strDbQueryName;

	// ���ݿ���ұ�������
	ReadTo(stream, "#");
	stream >> tmp >> entityBuildInfo->lDbQueryType;

	// ���ݿ���ұ���(��ǰĬ��Ϊlong)
	ReadTo(stream, "#");
	long value = 0;
	stream >> tmp >> value;
	memset(entityBuildInfo->pQueryVar, 0, sizeof(entityBuildInfo->pQueryVar));
	
	memcpy(entityBuildInfo->pQueryVar, &value, sizeof(long));

	// ���ݱ�ۼ�������
	string strTemp="";
	while(1)
	{
		stream >> strTemp;
		if (strTemp=="<end>" || stream.eof())
		{
			break;
		}

		if (strTemp=="#")
		{
			int lTmpUnique = -1;
			stream >> strTemp >> strTemp >> lTmpUnique;
			tagDbIdxInfo idxInfo(strTemp, lTmpUnique);
			entityBuildInfo->vecIndex.push_back(idxInfo);
		}
	}
	
	// ���ݱ�Ǿۼ�������
	string strCluTemp="";
	while(1)
	{
		stream >> strCluTemp;
		if (strCluTemp=="<end>" || stream.eof())
		{
			break;
		}

		if (strCluTemp=="#")
		{
			int lTmpUnique = -1;
			stream >> strCluTemp >> strCluTemp >> lTmpUnique;
			tagDbIdxInfo idxInfo(strCluTemp, lTmpUnique);
			entityBuildInfo->vecNoCluIndex.push_back(idxInfo);

		}
	}

	// ���ݱ���������
	string strComTemp="";
	while(1)
	{
		stream >> strComTemp;
		if (strComTemp=="<end>" || stream.eof())
		{
			break;
		}

		if (strComTemp=="#")
		{
			int lTmpUnique = -1;
			stream >> strComTemp >> strComTemp >> lTmpUnique;
			tagDbIdxInfo idxInfo(strComTemp, lTmpUnique);
			entityBuildInfo->vecCoIndex.push_back(idxInfo);
		}
	}

	CGUID tmpGUID(szGuid.c_str());
	// ȥ���ļ���·��
	long pos = deManagerName.find('[');
	deManagerName.erase(0, pos);
	// ȥ����׺��
	pos = deManagerName.find('.');
	deManagerName.erase(pos, deManagerName.size());

	m_mapObjAttrDef[deManagerName] = entityBuildInfo;

	while(ReadTo(stream, "#"))
	{
		string attrName = "";
		long attrType = 0;
		long UseageFlag = 0;
		long attrMaxSize = 0;
		long lAttrTypeEnum = 0;
		tagEntityAttrInfo tagAttrInfo;
		stream >> attrName >> lAttrTypeEnum >> tagAttrInfo.lMaxSize >> tagAttrInfo.lUseage;
		tagAttrInfo.eNumDataType = (DATA_OBJECT_TYPE)lAttrTypeEnum;
	
		m_lAttrEnumValue++;
		tagAttrInfo.lAttrTypeEnum = m_lAttrEnumValue;
		AddAttrEnumAndStr(attrName, m_lAttrEnumValue);
		entityBuildInfo->vecAttrEnum.push_back(m_lAttrEnumValue);
		entityBuildInfo->pDataMgr[attrName] = tagAttrInfo;
	}

	return deManagerName.c_str();
}
//!	������������
void CEntityManager::CreateOtherTableComIndexInDb(const string& tableName, std::vector<tagDbIdxInfo>& vecIdx)
{
	if(vecIdx.size() <= 0) return;

	string strCreateTableSql;

	//! �������ͷ
	strCreateTableSql = "\r\nCREATE ";
	if(vecIdx[0].lUnique == 1)
		strCreateTableSql += "UNIQUE ";
	 strCreateTableSql += "INDEX [";

	strCreateTableSql += "IDX_Com" + tableName + "] ON [dbo].[" + tableName + "]\r\n(";

	for(int i=0; i<vecIdx.size(); i++)
	{
		if(vecIdx[i].strIndex == "NULL") return;

		strCreateTableSql += "\r\n[";
		strCreateTableSql += vecIdx[i].strIndex;
		strCreateTableSql += "] ASC";

		if(i < vecIdx.size()-1) // ���һ�����Բ���','
			strCreateTableSql += ",";		
	}
	strCreateTableSql += "\r\n)WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF) ON [PRIMARY]";
	ExecSqlString(strCreateTableSql);
}
void CEntityManager::CreateOtherTableIndexInDb(const string& tableName, std::vector<tagDbIdxInfo>& vecIdx, bool isClustered)
{
	string strCreateTableSql;
	
	for(int i=0; i<vecIdx.size(); i++)
	{
		if(vecIdx[i].strIndex == "NULL") return;

		strCreateTableSql = "\r\nCREATE ";
		if(vecIdx[i].lUnique == 1)
			strCreateTableSql += "UNIQUE ";

		//! �������ͷ
		if(!isClustered)
		{
			
			strCreateTableSql += "NONCLUSTERED INDEX [";
		}
		else
		{
			strCreateTableSql += "CLUSTERED INDEX [";
		}

		strCreateTableSql += "IDX_" + tableName + vecIdx[i].strIndex + "] ON [dbo].[" + tableName + "]\r\n(";
		strCreateTableSql += "\r\n[";
		strCreateTableSql += vecIdx[i].strIndex;
		strCreateTableSql += "] ASC";
	
		strCreateTableSql += "\r\n)WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF) ON [PRIMARY]";
		ExecSqlString(strCreateTableSql);
	}
}
void CEntityManager::CreateOtherTableInDb(const string& tableName, const string& otherIniName)
{
	//! SQL����ͷ			= 1 + ���� + 3
	string strHead_1		= "\r\n\r\nCREATE TABLE [dbo].[";
	string strHead_3		= "] (\r\n";

	//! 8λ���������ֶ�	= 1 + �ֶ��� + 3
	string strInt8_1		= "\t[";
	string strInt8_3		= "] [tinyint] NOT NULL ,\r\n";

	//! 16λ���������ֶ�	= 1 + �ֶ��� + 3
	string strInt16_1		= "\t[";
	string strInt16_3		= "] [smallint] NOT NULL ,\r\n";

	//! 32λ���������ֶ�	= 1 + �ֶ��� + 3
	string strInt32_1		= "\t[";
	string strInt32_3		= "] [int] NOT NULL ,\r\n";
	//! ���������ֶ�		= 1 + �ֶ��� + 3
	string strFloat_1		= "\t[";
	string strFloat_3		= "] [float] NOT NULL ,\r\n";
	//! ���������ֶ�		= 1 + �ֶ��� + 3
	string strDate_1		= "\t[";
	string strDate_3		= "] [datetime] NULL ,\r\n";
	//!	GUID�����ֶ�		= 1 + �ֶ��� + 3
	string strGuid_1		= "\t[";
	string strGuid_3		= "] [uniqueidentifier] NOT NULL ,\r\n";
	//!	�ַ��������ֶ� = 1 + �ֶ��� + 3 + size(%d) + 5
	string strStr50_1		= "\t[";
	string strStr50_3		= "] [nvarchar] (";
	string strStr50_5		= ") COLLATE Chinese_PRC_CI_AS NOT NULL ,\r\n";

	//! IMAGE�����ֶ�	= 1 + �ֶ��� + 3
	string image_1		= "\t[";
	string image_3		= "] [image] NULL , \r\n";

	//!	SQL����β
	string strFinality		= ") ON [PRIMARY]";

	//! ����˳��ο�eDataTypeCode˳��
	string arrItemName[] = 
	{
		strInt8_1,		strInt8_3,
		strInt16_1,		strInt16_3,
		strInt32_1,		strInt32_3,
		strFloat_1,		strFloat_3,
		strDate_1,		strDate_3,
		strGuid_1,		strGuid_3,
		strStr50_1,		strStr50_3, //strStr50_5,
		image_1,		image_3,
	};

	string strCreateTableSql;
	//! �������ͷ
	strCreateTableSql += strHead_1 + tableName + strHead_3;
	tagEntityBuildInfo* pObjAttrDef = NULL;
	std::map<string, tagEntityBuildInfo*>::iterator itr = m_mapObjAttrDef.find(otherIniName);
	if(itr != m_mapObjAttrDef.end())
		pObjAttrDef = itr->second;

	if(!pObjAttrDef) return;

	bool hasUseImageFlag = false;

	//! ���������
	vector<long>::iterator attrItr = pObjAttrDef->vecAttrEnum.begin();
	for (; attrItr != pObjAttrDef->vecAttrEnum.end(); attrItr++)
	{
		DATA_OBJECT_TYPE enumDataType = (DATA_OBJECT_TYPE)GetGame()->GetEntityManager()->GetDataObjectType(itr->first, *attrItr);
		string strName = GetGame()->GetEntityManager()->GetAttrStrByEnum(*attrItr);
		long lMaxSize = GetGame()->GetEntityManager()->GetBufMaxSize(itr->first, *attrItr);

		switch(enumDataType)
		{
		case DATA_OBJECT_LONG:
		case DATA_OBJECT_DWORD:
			{
				strCreateTableSql += 
					arrItemName[2 * 2] + 
					strName +
					arrItemName[2 * 2 + 1];
			}
			break;
		case DATA_OBJECT_SHORT:
		case DATA_OBJECT_WORD:
			{
				strCreateTableSql += 
					arrItemName[2 * 1] + 
					strName +
					arrItemName[2 * 1 + 1];
			}
			break;
		case DATA_OBJECT_CHAR:
		case DATA_OBJECT_BYTE:
			{
				strCreateTableSql += 
					arrItemName[2 * 1] + 
					strName +
					arrItemName[2 * 1 + 1];
			}
			break;
		case DATA_OBJECT_FLOAT:
			{
				strCreateTableSql += 
					arrItemName[2 * 3] + 
					strName +
					arrItemName[2 * 3 + 1];
			}
			break;
		case DATA_OBJECT_STRING:
			{
				char szNum[128];
				itoa(lMaxSize, szNum, 10);
				strCreateTableSql += 
					arrItemName[2 * 6] + 
					strName +
					arrItemName[2 * 6 + 1] + szNum
					+ strStr50_5;
			}
			break;
		case DATA_OBJECT_TIME:
			{
				strCreateTableSql += 
					arrItemName[2 * 4] + 
					strName +
					arrItemName[2 * 4 + 1];
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				hasUseImageFlag = true;
				strCreateTableSql += 
					arrItemName[2 * 7] + 
					strName +
					arrItemName[2 * 7 + 1];
			}
			break;
		case DATA_OBJECT_GUID:
			{
				strCreateTableSql += 
					arrItemName[2 * 5] + 
					strName +
					arrItemName[2 * 5 + 1];
			}
			break;
		}
		
	}


	//! ������
	if(pObjAttrDef->strDBPrimarykey != "NULL")
	{
		strCreateTableSql += " CONSTRAINT [PK_" + pObjAttrDef->strTblName + "] PRIMARY KEY NONCLUSTERED\r\n(\r\n["
			+ pObjAttrDef->strDBPrimarykey + "] ASC\r\n)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]\r\n";

	}
	//! �������β
	if(hasUseImageFlag)
		strCreateTableSql += strFinality + " TEXTIMAGE_ON [PRIMARY]";
	else
		strCreateTableSql += strFinality;

	ExecSqlString(strCreateTableSql);

	// �����Ǿۼ�����
	CreateOtherTableIndexInDb(pObjAttrDef->strTblName, pObjAttrDef->vecIndex, true);

	// �����ۼ�����
	CreateOtherTableIndexInDb(pObjAttrDef->strTblName, pObjAttrDef->vecNoCluIndex, false);

	// ������������
	CreateOtherTableComIndexInDb(pObjAttrDef->strTblName, pObjAttrDef->vecCoIndex);
}

void CEntityManager::ExecSqlString(const string& sqlStr)
{
	//! ִ�����
	//! �����õ����ݿ���ִ��
	try
	{	
		m_pAccessThread->GetConnPtr()->Execute(sqlStr.c_str(), NULL, adCmdText);
	}
	catch(_com_error &e)
	{
		CMyAdoBase::PrintErr("ExecSqlString() Err!", e);
		return;
	}
}

void CEntityManager::AddTblProcedure(CBaseEntity* pRoot, DB_OPERATION_TYPE opType, const string& name)
{
	if(pRoot)
	{
		map<string, map<DB_OPERATION_TYPE,string>>::iterator itr = m_mapTblProc.find(pRoot->GetCompositeFlag());
		if(itr != m_mapTblProc.end())
		{
			itr->second[opType] = name;
		}
		else
		{
			map<DB_OPERATION_TYPE,string> mapName;
			mapName[opType] = name;
			m_mapTblProc[pRoot->GetCompositeFlag()] = mapName;
		}
	}
}
void CEntityManager::CreateTblProcedure(CBaseEntity* pRoot)
{
	if(!pRoot) return;

	//! 8λ���������ֶ�
	string strInt8	= "tinyint";

	//! 16λ���������ֶ�
	string strInt16 = "smallint";

	//! 32λ���������ֶ�
	string strInt32 = "int";
	//! ���������ֶ�
	string strFloat = "float";
	//! ���������ֶ�
	string strDate = "datetime";
	//!	GUID�����ֶ�
	string strGuid = "uniqueidentifier";
	//!	�ַ��������ֶ� = strnVarChar + ( + size() + )
	string strnVarChar = "nvarchar(";
	//! IMAGE�����ֶ�
	string image = "image";

	string strProcData = "";

	// ����Insert into
	string strTbl = "";
	string strPK = "";
	string strDbPK = "";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(pRoot->GetCompositeFlag());
	if(!pInfo) return;
	
	strTbl = pInfo->strTblName;
	strPK = pInfo->strPrimarykey;
	strDbPK = pInfo->strDBPrimarykey;

	// ���ɴ洢������
	string strInsertProcName = "Insert" + strTbl + "Proc";
	strProcData = "CREATE PROCEDURE " + strInsertProcName + "\r\n";

	// ���ɲ���
	vector<long>::iterator epItr = pInfo->vecAttrEnum.begin();
	for(; epItr != pInfo->vecAttrEnum.end(); epItr++)
	{
		DATA_OBJECT_TYPE enumDataType = (DATA_OBJECT_TYPE)GetGame()->GetEntityManager()->GetDataObjectType(pRoot->GetCompositeFlag(), *epItr);
		string strName = GetGame()->GetEntityManager()->GetAttrStrByEnum(*epItr);
		long lUseageFlag = GetGame()->GetEntityManager()->GetDbUseageFlag(pRoot->GetCompositeFlag(), *epItr);

		switch(enumDataType)
		{
		case DATA_OBJECT_GUID:
			{
				strProcData += "@" + strName + "\t" + strGuid;
			}
			break;
		case DATA_OBJECT_STRING:
			{
				long lMaxSize = GetGame()->GetEntityManager()->GetBufMaxSize(pRoot->GetCompositeFlag(), *epItr);
				char strLen[128];
				sprintf(strLen, "%d)", lMaxSize);
				string tmpStr = strnVarChar + strLen;
				strProcData += "@" + strName + "\t" + tmpStr;
			}
			break;
		case DATA_OBJECT_TIME:
			{
				strProcData += "@" + strName + "\t" + strDate;
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				strProcData += "@" + strName + "\t" + image;
			}
			break;
		default:
			{
				strProcData += "@" + strName + "\t" + strInt32;
			}
			break;
		}
			strProcData += ",";
		strProcData += "\r\n";
	}
	strProcData[strProcData.size()-3] = ' ';

	// ���ɴ洢��������
	strProcData += "AS\r\ninsert into " + strTbl + " with(ROWLOCK) (";
	epItr = pInfo->vecAttrEnum.begin();
	for(; epItr != pInfo->vecAttrEnum.end(); epItr++)
	{
		string strEpName = GetGame()->GetEntityManager()->GetAttrStrByEnum(*epItr);
		strProcData += strEpName;
			strProcData += ",";
	}
	strProcData[strProcData.size()-1] = ' ';

	strProcData += ") values(";
	epItr = pInfo->vecAttrEnum.begin();
	for(; epItr != pInfo->vecAttrEnum.end(); epItr++)
	{
		string strEpName = GetGame()->GetEntityManager()->GetAttrStrByEnum(*epItr);
		strProcData += "@" + strEpName;
			strProcData += ",";
	}
	strProcData[strProcData.size()-1] = ' ';
	strProcData += ")";

	// �����洢����
	string strClearPro = 
		"SET ANSI_NULLS ON\r\n\
		SET QUOTED_IDENTIFIER ON\r\n\
		\r\n\
		IF EXISTS (\r\n\
		SELECT * \r\n\
		FROM INFORMATION_SCHEMA.ROUTINES\r\n\
		WHERE SPECIFIC_NAME = N'"+ strInsertProcName + "'\r\n\
		)\r\n\
		DROP PROCEDURE "+ strInsertProcName+";\r\n";

	try
	{
		HRESULT hRet = CMyAdoBase::ExecuteCn(strClearPro.c_str(), GetAccessThread()->GetConnPtr());
		if(hRet > 0)
		{
			hRet = CMyAdoBase::ExecuteCn(strProcData.c_str(), GetAccessThread()->GetConnPtr());
		}
	}
	catch (_com_error& e)
	{
		AddLogText("CreateTblProcedure() ִ�д洢����[%s]����!", strInsertProcName.c_str());
		CMyAdoBase::PrintErr(strInsertProcName.c_str(), e);
	}
	AddTblProcedure(pRoot, DB_OPER_INSERT, strInsertProcName);
	AddTblProcedure(pRoot, DB_OPER_DELETE_INSERT, strInsertProcName);

	// ����Update
	if(strPK == "NULL") return;

	strProcData.clear();
	// ���ɴ洢������
	string strUpdateProcName = "Update" + strTbl + "Proc";
	strProcData = "CREATE PROCEDURE " + strUpdateProcName + "\r\n";

	// ���ɲ���
	epItr = pInfo->vecAttrEnum.begin();
	for(; epItr != pInfo->vecAttrEnum.end(); epItr++)
	{
		DATA_OBJECT_TYPE enumDataType = (DATA_OBJECT_TYPE)GetGame()->GetEntityManager()->GetDataObjectType(pRoot->GetCompositeFlag(), *epItr);
		string strName = GetGame()->GetEntityManager()->GetAttrStrByEnum(*epItr);
		long lUseageFlag = GetGame()->GetEntityManager()->GetDbUseageFlag(pRoot->GetCompositeFlag(), *epItr);

		switch(enumDataType)
		{
		case DATA_OBJECT_GUID:
			{
				strProcData += "@" + strName + "\t" + strGuid;
			}
			break;
		case DATA_OBJECT_STRING:
			{
				long lMaxSize = GetGame()->GetEntityManager()->GetBufMaxSize(pRoot->GetCompositeFlag(), *epItr);
				char strLen[128];
				sprintf(strLen, "%d)", lMaxSize);
				string tmpStr = strnVarChar + strLen;
				strProcData += "@" + strName + "\t" + tmpStr;
			}
			break;
		case DATA_OBJECT_TIME:
			{
				strProcData += "@" + strName + "\t" + strDate;
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				strProcData += "@" + strName + "\t" + image;
			}
			break;
		default:
			{
				strProcData += "@" + strName + "\t" + strInt32;
			}
			break;
		}
			strProcData += ",";
		strProcData += "\r\n";
	}
	strProcData[strProcData.size()-3] = ' ';

	// ���ɴ洢��������
	strProcData += "AS\r\nupdate " + strTbl + " with(ROWLOCK) set";
	epItr = pInfo->vecAttrEnum.begin();
	for(; epItr != pInfo->vecAttrEnum.end(); epItr++)
	{
		string strEpName = GetGame()->GetEntityManager()->GetAttrStrByEnum(*epItr);
		BYTE lUseFlag = GetGame()->GetEntityManager()->GetDbUseageFlag(pRoot->GetCompositeFlag(), *epItr);
		if( strEpName == strDbPK || lUseFlag == 0xFF )//1:�����Զ����� -1;һ�����ɾͲ��ٽ���д����
			continue;

		strProcData += " " + strEpName + "=@" + strEpName;
			strProcData += ",";
	}
	strProcData[strProcData.size()-1] = ' ';
	strProcData += " where " + strPK + "=@" + strPK;

	// �����洢����
	strClearPro = 
		"SET ANSI_NULLS ON\r\n\
		SET QUOTED_IDENTIFIER ON\r\n\
		\r\n\
		IF EXISTS (\r\n\
		SELECT * \r\n\
		FROM INFORMATION_SCHEMA.ROUTINES\r\n\
		WHERE SPECIFIC_NAME = N'"+ strUpdateProcName + "'\r\n\
		)\r\n\
		DROP PROCEDURE "+ strUpdateProcName+";\r\n";

	try
	{
		HRESULT hRet = CMyAdoBase::ExecuteCn(strClearPro.c_str(), GetAccessThread()->GetConnPtr());
		if(hRet > 0)
		{
			hRet = CMyAdoBase::ExecuteCn(strProcData.c_str(), GetAccessThread()->GetConnPtr());
		}
	}
	catch (_com_error& e)
	{
		AddLogText("CreateTblProcedure() ִ�д洢����[%s]����!", strUpdateProcName.c_str());
		CMyAdoBase::PrintErr(strUpdateProcName.c_str(), e);
	}
	AddTblProcedure(pRoot, DB_OPER_SAVE, strUpdateProcName);
}

const string& CEntityManager::FindTblProcedure(CBaseEntity* pRoot)
{
	if(pRoot)
	{
		map<string, map<DB_OPERATION_TYPE,string>>::iterator itr = m_mapTblProc.find(pRoot->GetCompositeFlag());
		if(itr != m_mapTblProc.end())
		{
			map<DB_OPERATION_TYPE,string>::iterator nameItr = itr->second.find((DB_OPERATION_TYPE)pRoot->GetCurDbOperType());
			if(nameItr != itr->second.end())
				return nameItr->second;
		}
	}
	return NULL_STRING;
}


//!					����һ�������ʱ�ʼ��Ĵ洢����
void CEntityManager::RunDeleteTimeoutMailProc(void)
{
	_CommandPtr		m_pCommand;   
	_RecordsetPtr	rs;   
	_variant_t		result1;   
	_bstr_t			strProc="SystemDeleteMailCheckProc";

	try
	{
		m_pCommand.CreateInstance(__uuidof(Command));
		m_pCommand->ActiveConnection = GetAccessThread()->GetAdoBasePtr()->GetConnPtr();
		m_pCommand->CommandText = strProc;
		m_pCommand->CommandType = adCmdStoredProc;

		//! ���ò���1
		_ParameterPtr tParam1;
		time_t tCurTime = time(0);
		tParam1 = m_pCommand->CreateParameter(_bstr_t("CurrTime"), adInteger, adParamInput, sizeof(LONG), (long)tCurTime);
		m_pCommand->Parameters->Append(tParam1);

		//! ���ò���2
		_ParameterPtr tParam2;
		long buf = 0;
		long bufSize = sizeof(long);
		VARIANT varBLOB;
		SAFEARRAY *psa = 0;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound->cElements = bufSize;
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		for(long i=0; i<bufSize; i++)
			SafeArrayPutElement(psa, &i, &((char*)&buf)[i]);
		varBLOB.parray = psa;
		varBLOB.vt = VT_ARRAY | VT_UI1;
		tParam2 = m_pCommand->CreateParameter(_bstr_t("IncGoodsBuf"), adVarBinary, adParamInput, bufSize, varBLOB);
		m_pCommand->Parameters->Append(tParam2);
		SafeArrayDestroy(psa);
		
		//! ִ��
		rs = m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
	}
	catch(_com_error &e)
	{
		CMyAdoBase::PrintErr("Run DeleteTimeoutMailProc error��", e);
		AddLogText("���������ʱ�ʼ��洢����ʧ�ܣ�");
		return;
	}
	AddLogText("���������ʱ�ʼ��洢���̳ɹ���");
}

// ���һ��BaseEntity��Mapĩβ
void CEntityManager::AddOneRootEntity(CEntityGroup* entity)
{
	if(entity)
	{
		DBEntityComponentMapItr itr = m_mapEntitys.find(entity->GetCompositeFlag());
		if(itr == m_mapEntitys.end())
		{
			map<CGUID, CEntityGroup*> entityMap;
			entityMap[entity->GetGUID()] = entity;
			m_mapEntitys[entity->GetCompositeFlag()] = entityMap;
			return;
		}
		else
		{
			map<CGUID, CEntityGroup*>::iterator enItr = itr->second.find(entity->GetGUID());
			if(enItr != itr->second.end())
			{
				if(entity != enItr->second)
					delete enItr->second;
				else
				{
					char szGuid[128];
					entity->GetGUID().tostring(szGuid);
					AddLogText("AddOneRootEntity:old ptr[%s],type[%s] has found.", szGuid, entity->GetCompositeFlag().c_str());
				}
				itr->second.erase(enItr);
			}
			itr->second[entity->GetGUID()] = entity;
		}
	}
}
// ɾ��һ��BaseEntity
void CEntityManager::DelOneRootEntity(CEntityGroup* entity)
{
	DBEntityComponentMapItr itr = m_mapEntitys.find(entity->GetCompositeFlag());
	if(itr != m_mapEntitys.end())
	{
		map<CGUID, CEntityGroup*>::iterator enItr = itr->second.find(entity->GetGUID());
		if(enItr != itr->second.end())
		{
			delete enItr->second;
			itr->second.erase(enItr);
		}
	}
}
// ɾ��һ��BaseEntity
void CEntityManager::DelOneRootEntity(const string& comFlag, const CGUID& playerID)
{
	DBEntityComponentMapItr itr = m_mapEntitys.find(comFlag);
	if(itr != m_mapEntitys.end())
	{
		map<CGUID, CEntityGroup*>::iterator enItr = itr->second.find(playerID);
		if(enItr != itr->second.end())
		{
			delete enItr->second;
			itr->second.erase(enItr);
		}
	}

}
// ���һ��Account��Mapĩβ
bool CEntityManager::AddAccount(Account* acc)
{
	if(acc)
	{
		string szCdkey = acc->GetName();
		if(szCdkey != "")
		{
			AccountMap::iterator itr = m_mapAccounts.find(szCdkey);
			if(itr != m_mapAccounts.end())
			{
				if(acc != itr->second)
				{
					delete itr->second;
					m_mapAccounts[szCdkey] = acc;
				}
				else
				{
					AddLogText("AddAccount: old ptr[%s] has found.", szCdkey);
				}
			}
			else
			{
				m_mapAccounts[szCdkey] = acc;
			}
			return true;
		}
		else
		{
			AddLogText("AddAccount: szCdkey is NULL.");
		}
	}
	return false;
}
// ɾ��һ��Account
void CEntityManager::DelAccount(const char* szCdkey)
{
	if(szCdkey)
	{
		// ����vector�в����Ƿ��Ѿ�����
		AccountMap::iterator itr = m_mapAccounts.find(string(szCdkey));
		if(itr != m_mapAccounts.end())
		{
			delete itr->second;
			m_mapAccounts.erase(itr);
		}
	}
}

Account* CEntityManager::FindAccount(const std::string& strCdkey)
{
	// ����vector�в����Ƿ��Ѿ�����
	AccountMap::iterator itr = m_mapAccounts.find(strCdkey);
	if(itr != m_mapAccounts.end())
	{
		return itr->second;
	}
	return NULL;
}
void CEntityManager::LoadProcedureInfo(const char* szPath)
{
	if(szPath)
	{
		CRFile* file = rfOpen(szPath);
		if(file)
		{
			long fileLen = file->GetDatalen();
			char* fileData = new char[fileLen+1];
			memset(fileData, 0, fileLen+1);
			memcpy(fileData, file->GetData(), fileLen);

			string procName = szPath;
			// ȥ���ļ���·��
			long pos = procName.find('\\');
			procName.erase(0, pos+1);
			// ȥ����׺��
			pos = procName.find('.');
			procName.erase(pos, procName.size());
			m_vecProcedureName.push_back(procName);

			string strClearPro = 
				"SET ANSI_NULLS ON\r\n\
				SET QUOTED_IDENTIFIER ON\r\n\
				\r\n\
				IF EXISTS (\r\n\
				SELECT * \r\n\
				FROM INFORMATION_SCHEMA.ROUTINES\r\n\
				WHERE SPECIFIC_NAME = N'"+ procName + "'\r\n\
				)\r\n\
				DROP PROCEDURE "+ procName+";\r\n";

			string strCreatePro = fileData;
			try
			{
				HRESULT hRet = CMyAdoBase::ExecuteCn(strClearPro.c_str(), GetAccessThread()->GetConnPtr());
				if(hRet > 0)
				{
					hRet = CMyAdoBase::ExecuteCn(strCreatePro.c_str(), GetAccessThread()->GetConnPtr());
				}
				SAFE_DELETE_ARRAY(fileData);
			}
			catch (_com_error& e)
			{
				char szOutInfo[1024];
				sprintf(szOutInfo, "LoadProcedureInfo() ִ�д洢����[%s]����!", procName.c_str());
				CMyAdoBase::PrintErr(szOutInfo, e);
				SAFE_DELETE_ARRAY(fileData);
			}
			rfClose(file);
		}
	}
}
// ����Ҷ�����Ͳ����丸�ڵ�
CEntityGroup* CEntityManager::FindLeafFatherByComFlag(CEntityGroup* pRoot, const string& leafComFlag)
{
	if(pRoot)
	{
		if( ((CEntityGroup*)pRoot)->GetLeafComFlag() == leafComFlag ) 
			return pRoot;

		if(pRoot->GetCompositeType() == COM_COMPOSITE)
		{
			map<CGUID, CBaseEntity*>::iterator itr = pRoot->GetEntityGroupMap().begin();
			for(; itr != pRoot->GetEntityGroupMap().end(); itr++)
			{
				if(itr->second->GetCompositeType() == COM_COMPOSITE)
				{
					if(((CEntityGroup*)itr->second)->GetLeafComFlag() == leafComFlag)
					return (CEntityGroup*)itr->second;

					CEntityGroup* retEntity = FindLeafFatherByComFlag((CEntityGroup*)itr->second, leafComFlag);
					if(retEntity)
						return retEntity;
				}
			}
		}
	}
	return NULL;
}
// ����Ҷ���������������²�����ڵ�
CBaseEntity* CEntityManager::FindLeafByComFlag(CEntityGroup* pRoot, const string& leafComFlag, const CGUID& leafGuid)
{
	if(pRoot)
	{
		if(pRoot->GetCompositeType() == COM_COMPOSITE)
		{
			map<CGUID, CBaseEntity*>::iterator itr = pRoot->GetEntityGroupMap().begin();
			for(; itr != pRoot->GetEntityGroupMap().end(); itr++)
			{
				if((itr->second)->GetCompositeFlag() == leafComFlag 
					&& itr->second->GetGUID() == leafGuid)
					return (CEntityGroup*)itr->second;

				if(itr->second->GetCompositeType() == COM_COMPOSITE)
				{
					CEntityGroup* retEntity = (CEntityGroup*)FindLeafByComFlag((CEntityGroup*)itr->second, leafComFlag, leafGuid);
					if(retEntity)
						return retEntity;
				}
			}
		}
	}
	return NULL;
}

// ����Ҷ���������������²�����ڵ�
CEntityGroup* CEntityManager::FindLeafGroupByComFlag(CEntityGroup* pRoot, const string& leafComFlag)
{
	if(pRoot)
	{
		if(pRoot->GetCompositeType() == COM_COMPOSITE)
		{
			map<CGUID, CBaseEntity*>::iterator itr = pRoot->GetEntityGroupMap().begin();
			for(; itr != pRoot->GetEntityGroupMap().end(); itr++)
			{
				if((itr->second)->GetCompositeFlag() == leafComFlag)
				{
					return (CEntityGroup*)itr->second;
				}

				if(itr->second->GetCompositeType() == COM_COMPOSITE)
				{
					CEntityGroup* retEntity = FindLeafGroupByComFlag((CEntityGroup*)itr->second, leafComFlag);
					if(retEntity)
						return retEntity;
				}
			}
		}
	}
	return NULL;
}

// ������ҽڵ���ĳЩ�ӽڵ�����ݿ������־λ
void CEntityManager::SetEntityLeavesDbOperFlag(CEntityGroup* pBaseEntity, const string& leafComFlag, bool operFlag)
{
	if(!pBaseEntity) return;
	CEntityGroup* pGoodsGroup = NULL;
	map<string, CGUID>::iterator guidItr = pBaseEntity->GetGuidByComFlagMap().find(leafComFlag);
	if(guidItr != pBaseEntity->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator gpItr = pBaseEntity->GetEntityGroupMap().find(guidItr->second);
		if(gpItr != pBaseEntity->GetEntityGroupMap().end())
			gpItr->second->SetCurDbOperFlag(operFlag);
	}
}
// ����ҽڵ��������ӽڵ����ݿ������־λ����
void CEntityManager::ResetEntityAllDbOperFlag(CBaseEntity* pBaseEntity, bool flag)
{
	if(!pBaseEntity) return;

	pBaseEntity->SetCurDbOperFlag(flag);

	if(pBaseEntity->GetCompositeType() != COM_COMPOSITE) return;// ����Ҷ�ӽڵ�

	pBaseEntity->SetCurDbOperFlag(flag);
	map<CGUID, CBaseEntity*>::iterator gpItr = ((CEntityGroup*)pBaseEntity)->GetEntityGroupMap().begin();
	for(; gpItr != ((CEntityGroup*)pBaseEntity)->GetEntityGroupMap().end(); gpItr++)
	{
		ResetEntityAllDbOperFlag(gpItr->second, flag);
	}
}

// ���ôӸ���㿪ʼ�����нڵ����ݿ������־
void CEntityManager::SetEntityAllDbOperType(CBaseEntity* pBaseEntity, DB_OPERATION_TYPE opType)
{
	if(!pBaseEntity) return;

	pBaseEntity->SetCurDbOperType(opType);

	if(pBaseEntity->GetCompositeType() != COM_COMPOSITE) return;// ����Ҷ�ӽڵ�

	pBaseEntity->SetCurDbOperType(opType);
	map<CGUID, CBaseEntity*>::iterator gpItr = ((CEntityGroup*)pBaseEntity)->GetEntityGroupMap().begin();
	for(; gpItr != ((CEntityGroup*)pBaseEntity)->GetEntityGroupMap().end(); gpItr++)
	{
		SetEntityAllDbOperType(gpItr->second, opType);
	}
}

// ����һ��EntityGroup�Լ����ӽ���Size
long CEntityManager::ComputeEntityGroupMemSize(CEntityGroup* pBaseEntity)
{
	return 0;
}
// ӳ�������ַ�������ö��ֵ��
void CEntityManager::AddAttrEnumAndStr(const string& strName, long lEnum)
{
	m_mapAttrEnumToStr[lEnum] = strName;
}
long CEntityManager::GetAttrEnumByStr(const string& strComFlag, const string& strName)
{
	AttrDefMapItr itr = m_mapObjAttrDef.find(strComFlag);
	if(itr != m_mapObjAttrDef.end())
	{
		map<string, tagEntityAttrInfo>::iterator attrItr = itr->second->pDataMgr.find(strName);
		if(attrItr != itr->second->pDataMgr.end())
			return (long)attrItr->second.lAttrTypeEnum;
	}
	return -1;
}
const string& CEntityManager::GetAttrStrByEnum(long lEnum)
{
	map<long, string>::iterator itr = m_mapAttrEnumToStr.find(lEnum);
	if(itr != m_mapAttrEnumToStr.end())
		return itr->second;
	return NULL_STRING;
}
// ȡ�������ļ���������Ϣ
tagEntityBuildInfo* CEntityManager::GetEntityBuildInfo(const string& strComFlag)
{
	AttrDefMapItr itr = m_mapObjAttrDef.find(strComFlag);
	if(itr != m_mapObjAttrDef.end())
	{
		return itr->second;
	}
	return NULL;
}
// DATA_OBJECT_TYPE
long CEntityManager::GetDataObjectType(const string& strComFlag, const string& strName)
{
	AttrDefMapItr itr = m_mapObjAttrDef.find(strComFlag);
	if(itr != m_mapObjAttrDef.end())
	{
		map<string, tagEntityAttrInfo>::iterator attrItr = itr->second->pDataMgr.find(strName);
		if(attrItr != itr->second->pDataMgr.end())
			return (long)attrItr->second.eNumDataType;
	}
	return (long)DATA_OBJECT_UNKNOWN;
}
long CEntityManager::GetDataObjectType(const string& strComFlag, long lEnum)
{
	return GetDataObjectType(strComFlag, GetAttrStrByEnum(lEnum));
}
// ���ݿ����Ա�־λ: 1|0: 1(���ݿ��Զ�����) 0�������(Ĭ��)
BYTE CEntityManager::GetDbUseageFlag(const string& strComFlag, const string& strName)
{
	AttrDefMapItr itr = m_mapObjAttrDef.find(strComFlag);
	if(itr != m_mapObjAttrDef.end())
	{
		map<string, tagEntityAttrInfo>::iterator attrItr = itr->second->pDataMgr.find(strName);
		if(attrItr != itr->second->pDataMgr.end())
			return attrItr->second.lUseage;
	}
	return 0;
}
BYTE CEntityManager::GetDbUseageFlag(const string& strComFlag, long lEnum)
{
	return GetDbUseageFlag(strComFlag, GetAttrStrByEnum(lEnum));
}
// ���ݿ���󳤶�
long CEntityManager::GetBufMaxSize(const string& strComFlag, const string& strName)
{
	AttrDefMapItr itr = m_mapObjAttrDef.find(strComFlag);
	if(itr != m_mapObjAttrDef.end())
	{
		map<string, tagEntityAttrInfo>::iterator attrItr = itr->second->pDataMgr.find(strName);
		if(attrItr != itr->second->pDataMgr.end())
			return attrItr->second.lMaxSize;
	}
	return 0;
}
long CEntityManager::GetBufMaxSize(const string& strComFlag, long lEnum)
{
	return GetBufMaxSize(strComFlag, GetAttrStrByEnum(lEnum));
}

// ���ݶ���Ľڵ���Ϣ������ѯ����
bool CEntityManager::GenerateQuerySavePlayerInfo(CEntityGroup* pPlayer)
{
	if(pPlayer && pPlayer->GetCurDbOperType() == DB_OPER_SAVE)
	{
		// ���ڵ�
		if(pPlayer->GetCurDbOperFlag())
		{
			pPlayer->QueryDataRelease();
			// ��Ӳ�ѯ����
			CTempEntityProperty* pGuid = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
			if(pGuid)
			{
				pGuid->SetGuidAttr( pPlayer->GetGUID());
				pGuid->SetDbRelateType(DB_RELATE_NULL);
				pPlayer->GetQueryDataVec().push_back(pGuid);
			}
		}
		// goodsgroup�ڵ�
		CEntityGroup* pGoodsGroup = GetGame()->GetEntityManager()->FindLeafGroupByComFlag(pPlayer, string("[goodsgroup]"));
		if(pGoodsGroup 
			&& pGoodsGroup->GetCurDbOperFlag() == 1
			&& pGoodsGroup->GetCurDbOperType() == DB_OPER_DELETE_INSERT)
		{
			// ����ոö�����ӽڵ����ݿ������־
			pGoodsGroup->QueryDataRelease();
			// ��Ӳ�ѯ��ҵ�GUID��AND (
			CTempEntityProperty* pPlayerID = new CTempEntityProperty( string("PlayerID"), DATA_OBJECT_GUID);
			if(pPlayerID)
			{
				pPlayerID->SetGuidAttr( pPlayer->GetGUID());
				pPlayerID->SetDbRelateType(DB_RELATE_LBLANKAND);
				pGoodsGroup->GetQueryDataVec().push_back(pPlayerID);
			}
			// ����ӽ��
			list<CEntityGroup*> listOperGroup;
			map<CGUID, CBaseEntity*>::iterator leafItr = ((CEntityGroup*)pGoodsGroup)->GetEntityGroupMap().begin();
			for(; leafItr != pGoodsGroup->GetEntityGroupMap().end(); leafItr++)
			{
				if(leafItr->second->GetCurDbOperFlag() == 1
					&& pGoodsGroup->GetCurDbOperType() == DB_OPER_DELETE_INSERT)
					listOperGroup.push_back((CEntityGroup*)leafItr->second);
			}

			if(listOperGroup.size())
			{
				list<CEntityGroup*>::iterator listItr = listOperGroup.begin();
				list<CEntityGroup*>::iterator listendItr = listOperGroup.end();
				// trick:��ȡ�õ�����һ��Group��Iter
				--listendItr;
				for(; listItr != listOperGroup.end(); listItr++)
				{
					if( listItr != listendItr ) // ���ǵ�����1��
					{
						CTempEntityProperty* pRetName = new CTempEntityProperty(string("place"), DATA_OBJECT_LONG);
						if(pRetName)
						{
							tagEntityBuildInfo* pBuildInfo = GetEntityBuildInfo((*listItr)->GetCompositeFlag());
							if(pBuildInfo)
							{
								long lPlace = *((long*)&pBuildInfo->pQueryVar[0]);
								pRetName->SetLongAttr(lPlace);
								pRetName->SetDbRelateType(DB_RELATE_OR);
								pGoodsGroup->GetQueryDataVec().push_back(pRetName);
							}
						}
					}
					else
					{
						CTempEntityProperty* pRetName = new CTempEntityProperty(string("place"), DATA_OBJECT_LONG);
						if(pRetName)
						{
							tagEntityBuildInfo* pBuildInfo = GetEntityBuildInfo((*listItr)->GetCompositeFlag());
							if(pBuildInfo)
							{
								long lPlace = *((long*)&pBuildInfo->pQueryVar[0]);
								pRetName->SetLongAttr(lPlace);
								pRetName->SetDbRelateType(DB_RELATE_RBLANK);
								pGoodsGroup->GetQueryDataVec().push_back(pRetName);
							}
						}
					}
				}
			}
			else // û���ӽ��Ҫ������������������
			{
				pGoodsGroup->SetCurDbOperFlag(0);
				pGoodsGroup->QueryDataRelease();
			}
		}

		// �������ݽڵ�
		CEntityGroup* pFriendGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafGroupByComFlag(pPlayer, string("[playerfriendgroupOwner]"));
		if(pFriendGroup 
			&& pFriendGroup->GetCurDbOperFlag() == 1
			&& pFriendGroup->GetCurDbOperType() == DB_OPER_DELETE_INSERT)
		{
			// ����ոö�����ӽڵ����ݿ������־
			pFriendGroup->QueryDataRelease();
			CTempEntityProperty* pEP1 = new CTempEntityProperty( string("OwnerGUID"), DATA_OBJECT_GUID);
			if(pEP1)
			{
				pEP1->SetGuidAttr( pPlayer->GetGUID());
				pEP1->SetDbRelateType(DB_RELATE_NULL);
				pFriendGroup->GetQueryDataVec().push_back(pEP1);
			}
		}
		// С��������

		CEntityGroup* pSpriteGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafGroupByComFlag(pPlayer, string("[playerspritegroup]"));
		if(pSpriteGroup 
			&& pSpriteGroup->GetCurDbOperFlag() == 1
			&& pSpriteGroup->GetCurDbOperType() == DB_OPER_DELETE_INSERT)
		{
			pSpriteGroup->QueryDataRelease();
			CTempEntityProperty *pEP = new CTempEntityProperty( string("PlayerGuid"), DATA_OBJECT_GUID);
			if(pEP)
			{
				pEP->SetGuidAttr( pPlayer->GetGUID());
				pEP->SetDbRelateType(DB_RELATE_NULL);
				pSpriteGroup->GetQueryDataVec().push_back(pEP);
			}
		}
		// ��������
		CEntityGroup* pPetList = GetGame()->GetEntityManager()->FindLeafGroupByComFlag(pPlayer, "[playerpetlist]");
		if( pPetList 
			&& pPetList->GetCurDbOperFlag() == 1
			&& pPetList->GetCurDbOperType() == DB_OPER_DELETE_INSERT)
		{
			pPetList->QueryDataRelease();
			CTempEntityProperty* pOwnerGuid = new CTempEntityProperty("HostID", DATA_OBJECT_GUID);
			pOwnerGuid->SetGuidAttr(pPlayer->GetGUID());
			pOwnerGuid->SetDbRelateType(DB_RELATE_NULL);
			pPetList->GetQueryDataVec().push_back(pOwnerGuid);
		}
	}
	return false;
}
//! ȡ��LoadThreadDataSize
long CEntityManager::GetLoadThreadDataSize(void)
{
	long lRet = 0;
	EnterCriticalSection(&m_LoadCriticalSection);
	lRet = m_listLoadThreadOperData.size();
	LeaveCriticalSection(&m_LoadCriticalSection);
	return lRet;
}
//! �������ж��󣬲�����Ϣ
void CEntityManager::TravelForGenerateThreadData(void)
{
	static long lMaxThreadDataNum  = m_mapSaveThread.size();
	static long lHalfThreadDataNum = (lMaxThreadDataNum / 2) ? (lMaxThreadDataNum / 2):1;

	long lCurTravelNum = 0;

	if(m_listDirtyEntityName.size() == 0
		&& m_listDirtyEntityGuid.size() == 0 ) return;

	if(GetLoadThreadDataSize() != 0) return;

	list<CGUID> tmpBeginGuid;
	if(GetSaveThreadOperDataSize(tmpBeginGuid) < lMaxThreadDataNum)
	{
		long lCurThreadDataPos = 0;
		list<ThreadData*> threadDataList;

		if(m_listDirtyEntityName.size() != 0)
		{
			list<string> tmpCheckNameList;
			list<string>::iterator accItr = m_listDirtyEntityName.begin();
			while(accItr != m_listDirtyEntityName.end())
			{
				if(lCurTravelNum == GetGame()->GetSetup()->dwTravleMaxSize) break;
				// �ж��Ƿ����ظ�
				list<string>::iterator tmpCheckItr = std::find(tmpCheckNameList.begin(), tmpCheckNameList.end(), *accItr);
				if(tmpCheckItr != tmpCheckNameList.end())
				{
					++accItr;
					lCurTravelNum++;
					continue;
				}

				AccountMap::iterator itr = m_mapAccounts.find(*accItr);
				if(itr != m_mapAccounts.end())
				{
					// ����ThreadData
					ThreadData* pData = new ThreadData;

					if(!itr->second->GetIsTempEntityFlag())
					{	
						CEntityGroup* tmpEntity = new Account(itr->second->GetGUID());
						*((CEntityGroup*)tmpEntity) = *((CEntityGroup*)(itr->second));
						pData->pEntity = tmpEntity;
						ResetEntityAllDbOperFlag(itr->second);
					}
					else// ���ʱ��ʱ������ȡ��������
					{
						pData->pEntity = ReleaseAccountPtr(itr->first);
					}
					threadDataList.push_back(pData);
					lCurThreadDataPos++;
				}
				tmpCheckNameList.push_back(*accItr);
				accItr = m_listDirtyEntityName.erase(accItr);
				
				lCurTravelNum++;

				if(lCurThreadDataPos == lHalfThreadDataNum)
					break;
			}
		}

		if(m_listDirtyEntityGuid.size() != 0)
		{
			list<CGUID> tmpCheckGuidList;
			list<CGUID>::iterator guidItr = m_listDirtyEntityGuid.begin();
			while(guidItr != m_listDirtyEntityGuid.end())
			{
				if(lCurTravelNum == GetGame()->GetSetup()->dwTravleMaxSize) break;
				// �ж��Ƿ����̴߳�����
				list<CGUID>::iterator threadGuidItr = std::find(tmpBeginGuid.begin(), tmpBeginGuid.end(), *guidItr);
				if(threadGuidItr != tmpBeginGuid.end())
				{
					++guidItr;
					lCurTravelNum++;
					continue;
				}
				// �ж��Ƿ����ظ�
				threadGuidItr = std::find(tmpCheckGuidList.begin(), tmpCheckGuidList.end(), *guidItr);
				if(threadGuidItr != tmpCheckGuidList.end())
				{
					++guidItr;
					lCurTravelNum++;
					continue;
				}

				bool isFind = false;
				DBEntityComponentMapItr engroupitr = m_mapEntitys.begin();
				while(engroupitr != m_mapEntitys.end())
				{
					if(engroupitr->first == "[mailgroup]") 
					{
						++engroupitr;
						continue;
					}

					std::map<CGUID, CEntityGroup*>::iterator enItr = engroupitr->second.find(*guidItr);
					if(enItr != engroupitr->second.end())
					{
						isFind = true;
						// ����ThreadData
						ThreadData* pData = new ThreadData;
						switch(enItr->second->GetCurDbOperType())
						{
						case DB_OPER_DELETE:
							{
								pData->pEntity = ReleaseEntityPtr(enItr->second->GetCompositeFlag(), enItr->first);
							}
							break;
						default:
							{								
								if(!enItr->second->GetIsTempEntityFlag())
								{
									CEntityGroup* tmpEntity = new CEntityGroup(enItr->second->GetCompositeFlag(), enItr->first);
									*((CEntityGroup*)tmpEntity) = *((CEntityGroup*)(enItr->second));
									pData->pEntity = tmpEntity;
									ResetEntityAllDbOperFlag(enItr->second);
								}
								else// ���ʱ��ʱ������ȡ��������
								{
									pData->pEntity = ReleaseEntityPtr(enItr->second->GetCompositeFlag(), enItr->first);
								}
							}
							break;
						}

						// ���Դ���
#ifdef _TESTRUNSTACKINFO_
						{
							if(pData->pEntity->GetCompositeFlag() == "[player]")
							{
								char szOutPutStr[2048] = {0};
								char szOutGuid[128];
								pData->pEntity->GetGUID().tostring(szOutGuid);
								sprintf(szOutPutStr, "Travel() %s[%s] leaf num %d, optype %d, isTemp %d.", pData->pEntity->GetCompositeFlag().c_str(), szOutGuid, 
									((CEntityGroup*)pData->pEntity)->GetEntityGroupMap().size(), pData->pEntity->GetCurDbOperType(), pData->pEntity->GetIsTempEntityFlag());
								CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),szOutPutStr);

								map<CGUID, CBaseEntity*>::iterator tmpItr = ((CEntityGroup*)pData->pEntity)->GetEntityGroupMap().begin();
								for(; tmpItr != ((CEntityGroup*)pData->pEntity)->GetEntityGroupMap().end(); tmpItr++)
								{
									if(tmpItr->second->GetCompositeFlag() == "[goodsgroup]")
									{
										tmpItr->second->GetGUID().tostring(szOutGuid);
										sprintf(szOutPutStr, "--Travel() %s[%s] leaf num %d, optype %d, isTemp %d.--", 
											tmpItr->second->GetCompositeFlag().c_str(), szOutGuid, ((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().size(),
											tmpItr->second->GetCurDbOperType(), tmpItr->second->GetIsTempEntityFlag());
										CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),szOutPutStr);

										map<CGUID, CBaseEntity*>::iterator leafItr = ((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().begin();
										for(; leafItr != ((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().end(); leafItr++)
										{
											leafItr->second->GetGUID().tostring(szOutGuid);
											sprintf(szOutPutStr, "--Travel() %s[%s] leaf num %d, optype %d, isTemp %d.--", 
												leafItr->second->GetCompositeFlag().c_str(), szOutGuid, ((CEntityGroup*)leafItr->second)->GetEntityGroupMap().size(),
												leafItr->second->GetCurDbOperType(), leafItr->second->GetIsTempEntityFlag());
											CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),szOutPutStr);
										}
									}
								}
							}
						}
#endif

						threadDataList.push_back(pData);
						++lCurThreadDataPos;
						break;
					}
					++engroupitr;
				}

				if(isFind)
					tmpCheckGuidList.push_back(*guidItr);
				
				guidItr = m_listDirtyEntityGuid.erase(guidItr);

				lCurTravelNum++;
				
				if(lCurThreadDataPos > lMaxThreadDataNum)
					break;
			}
		}

		if (threadDataList.size())
			PushSaveThreadData(threadDataList);
	}
}
//! ȡ�ô洢���̵Ķ���ָ��
CEntityGroup* CEntityManager::ReleaseProcedurePtr(const CGUID& guid)
{
	DBEntityComponentMapItr itr = m_mapEntitys.find(string("[procedure]"));
	if(itr != m_mapEntitys.end())
	{
		std::map<CGUID, CEntityGroup*>::iterator enItr = itr->second.find(guid);
		if(enItr != itr->second.end())
		{
			CEntityGroup* pRet = enItr->second;
			itr->second.erase(enItr);
			return pRet;
		}
	}
	return NULL;
}
//! ȡ��ָ�����Ͷ���ָ��
CEntityGroup* CEntityManager::ReleaseEntityPtr(const string& strComFlag, const CGUID& guid)
{
	DBEntityComponentMapItr itr = m_mapEntitys.find(strComFlag);
	if(itr != m_mapEntitys.end())
	{
		std::map<CGUID, CEntityGroup*>::iterator enItr = itr->second.find(guid);
		if(enItr != itr->second.end())
		{
			CEntityGroup* pRet = enItr->second;
			itr->second.erase(enItr);
			return pRet;
		}
	}
	return NULL;
}
//! ȡ��account����ָ��
CEntityGroup* CEntityManager::ReleaseAccountPtr(const string& strCdkey)
{
	// ����vector�в����Ƿ��Ѿ�����
	AccountMap::iterator itr = m_mapAccounts.find(strCdkey);
	if(itr != m_mapAccounts.end())
	{
		CEntityGroup* pRet = itr->second;
		m_mapAccounts.erase(itr);
		return pRet;
	}
	return NULL;
}

//! ������̷߳��ص���Ϣ
void CEntityManager::ProcessRetThreadData(void)
{
#ifdef _DEBUG_SM_INFO_
	char szOutPutStr[1024] = {0};
#endif

	list<ThreadData*> retDataList;
	GetRetThreadData(retDataList);
	list<ThreadData*>::iterator itr = retDataList.begin();
	for(; itr != retDataList.end(); itr++)
	{
		CEntityGroup* pBaseEntity = (CEntityGroup*)((*itr)->pEntity);
		if(!pBaseEntity)
		{
			AddLogText("ProcRetData() entity ptr is null!");
			continue;
		}

		// �������Ҵ����쳣�򵥶�����
		if((*itr)->retFlag != S_OK)
		{
			// �������ڲ����Ķ���GUID
			PopBeginEntityGuid(pBaseEntity->GetGUID(), (eThreadType)((*itr)->lThreadOpType));

			if(pBaseEntity->GetCompositeFlag() == "[player]" && pBaseEntity->GetCurDbOperType() == DB_OPER_SAVE)
			{
				// ���´���һ��
				CEntityGroup* player = (CEntityGroup*)NewBaseEntity(std::string("[player]"), pBaseEntity->GetGUID());
				CEntityGroup* tplayer = FindRootEntity(std::string("[player]"), pBaseEntity->GetGUID());
				if(player && tplayer)
				{
					ResetEntityAllDbOperFlag(tplayer, true);
					*player = *tplayer;
					ResetEntityAllDbOperFlag(tplayer, false);
					player->SetCurDbOperType(DB_OPER_SAVE);
					player->SetCurDbOperFlag(1);

					CEntityGroup* pGoodsGroup = (CEntityGroup*)FindLeafGroupByComFlag(player, string("[goodsgroup]"));
					if(pGoodsGroup)
					{
						SetEntityAllDbOperType(pGoodsGroup, DB_OPER_DELETE_INSERT);
						ResetEntityAllDbOperFlag(pGoodsGroup, true);
					}

					// ��������
					CEntityGroup* pFriendGroup = (CEntityGroup*)FindLeafGroupByComFlag(player, string("[playerfriendgroup]"));
					if(pFriendGroup)
					{
						SetEntityAllDbOperType(pFriendGroup, DB_OPER_DELETE_INSERT);
						ResetEntityAllDbOperFlag(pFriendGroup, true);
					}

					// С��������
					CEntityGroup* pSpriteGroup = (CEntityGroup*)FindLeafGroupByComFlag(player, string("[playerspritegroup]"));
					if(pSpriteGroup)
					{
						SetEntityAllDbOperType(pSpriteGroup, DB_OPER_DELETE_INSERT);
						ResetEntityAllDbOperFlag(pSpriteGroup, true);
					}

					AddDirtyEntityGuid(player->GetGUID());
				}
				ThreadData* pTmpThreadData = new ThreadData;
				pTmpThreadData->pEntity = player;
				PushSaveThreadData(pTmpThreadData);
				
				// ���¼�ʱɾ��ʱ��
				const char* szCdkey = pBaseEntity->GetStringAttr(std::string("Account"));
				if(szCdkey)
				{
					Account* pAcc = FindAccount(szCdkey);
					if(pAcc)
					{
						if(pAcc->GetLastLoginTime() != -1)
							pAcc->SetLastLoginTime(timeGetTime());
					}
				}
				SAFE_DELETE(*itr);
				continue;
			}
		}

		if(pBaseEntity->GetCurDbOperType() == DB_OPER_CREATEROLE_PROCEDURE)
		{
#ifdef _DEBUG_SM_INFO_
			sprintf(szOutPutStr, "IN ProcessRetThreadData() DB_OPER_CREATEROLE_PROCEDURE Start.");
			CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif
			// ���жϴ����Ƿ�ɹ�
			if(pBaseEntity->GetRetQueryDataVec().size() == 2)
			{
				if((*itr)->retFlag == S_OK)
				{
					long NameIsExist = 0;
					long charNum = 0;	
					NameIsExist = pBaseEntity->GetRetQueryDataVec()[0]->GetLongAttr();
					charNum = pBaseEntity->GetRetQueryDataVec()[1]->GetLongAttr();
					if( NameIsExist == 0 && charNum == 1 )
					{
						CGUID tmpGuid;
						map<CGUID, CBaseEntity*>::iterator lgitr = ((CEntityGroup*)pBaseEntity)->GetEntityGroupMap().begin();
						lgitr->second->GetGuidAttr(string("guid"), tmpGuid);
						const char* szCdkey = lgitr->second->GetStringAttr(string("Account"));
						if(szCdkey)
						{
							Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
							if(pAcc)
							{
								lgitr->second->SetGUID(tmpGuid);
								CEntityGroup* pLoginPlayer = pAcc->FindLoginPlayer(tmpGuid);
								if(!pLoginPlayer)
								{
									pAcc->AddLoginPlayer((CEntityGroup*)lgitr->second);
									GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pAcc, false);
									pAcc->SetIsTempEntityFlag(0);
								}
								else
									delete lgitr->second;

								lgitr++;
								lgitr->second->SetGUID(tmpGuid);
								CEntityGroup* pPlayer = GetGame()->GetEntityManager()->FindRootEntity(std::string("[player]"), tmpGuid);
								if(!pPlayer)
								{
									GetGame()->GetEntityManager()->AddOneRootEntity((CEntityGroup*)lgitr->second);
									GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag((CEntityGroup*)lgitr->second, false);
									lgitr->second->SetIsTempEntityFlag(0);
								}
								else
									delete lgitr->second;

								pBaseEntity->ReleaseChildPtr(NULL_GUID);
								pBaseEntity->ReleaseChildPtr(tmpGuid);
							}
							else
								pBaseEntity->ReleaseChilds();
						}
						else
							pBaseEntity->ReleaseChilds();
					}
					else
						pBaseEntity->ReleaseChilds();
				}
				else
				{
					map<CGUID, CBaseEntity*>::iterator lgitr = ((CEntityGroup*)pBaseEntity)->GetEntityGroupMap().begin();
					const char* szCdkey = lgitr->second->GetStringAttr(string("Account"));
					AddLogText("CreatePlayer: [%s], [%d] failed!", szCdkey, (*itr)->retFlag);
					pBaseEntity->ReleaseChilds();
				}
				// ������Ϣ
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				msg.Add(pBaseEntity->GetSessionID());
				msg.Add((long)(*itr)->retFlag);
				msg.Add(pBaseEntity->GetCompositeFlag().c_str());
				msg.Add(pBaseEntity->GetGUID());
				msg.Add("");
				msg.Add((BYTE)0);
				vector<BYTE> pBA;
				pBaseEntity->AddToByteArray(pBA, false);
				msg.Add((long)pBA.size());
				if(pBA.size())
					msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(GetGame()->GetWSSocket());
			}
#ifdef _DEBUG_SM_INFO_
			sprintf(szOutPutStr, "IN ProcessData() DB_OPER_CREATEROLE_PROCEDURE End.");
			CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif
	}
		else if(pBaseEntity->GetCurDbOperType() == DB_OPER_CHANGENAME_PROCEDURE)
		{
			if((*itr)->retFlag == S_OK && pBaseEntity->GetRetQueryDataVec().size() == 1 && pBaseEntity->GetRetQueryDataVec()[0] && pBaseEntity->GetRetQueryDataVec()[0]->GetLongAttr() == 1) // �޸ĳɹ�
			{
				if(pBaseEntity->GetQueryDataVec().size() == 2 && pBaseEntity->GetQueryDataVec()[0] && pBaseEntity->GetQueryDataVec()[1])
				{
					CGUID guid;
					pBaseEntity->GetQueryDataVec()[0]->GetGuidAttr(guid);
					char* szName = (char*)pBaseEntity->GetQueryDataVec()[1]->GetStringAttr();
					CEntityGroup* pPlayer = FindRootEntity(string("[player]"), guid);
					if(szName && pPlayer)
						pPlayer->SetStringAttr(string("Name"), szName);
				}
			}

			CMessage msg(MSG_DB2W_OPER_ENTITY);
			msg.Add(pBaseEntity->GetSessionID());
			msg.Add((long)(*itr)->retFlag);
			msg.Add(pBaseEntity->GetCompositeFlag().c_str());
			msg.Add(pBaseEntity->GetGUID());
			msg.Add(pBaseEntity->GetName());
			msg.Add((BYTE)pBaseEntity->GetFindByNameFlag());
			// ������Ϣ
			vector<BYTE> pBA;
			pBaseEntity->AddToByteArray(pBA, false);
			msg.Add((long)pBA.size());
			if(pBA.size())
				msg.AddEx(&pBA[0], pBA.size());
			msg.SendToSocket(GetGame()->GetWSSocket());
		}
		else
		{
#ifdef _DEBUG_SM_INFO_
			sprintf(szOutPutStr, "IN ProcessData() opFlag:%d, Entity[%s] Start.",pBaseEntity->GetCurDbOperType(),pBaseEntity->GetCompositeFlag().c_str());
			CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif

			// �ڴ滺�����ڶ������
			if(pBaseEntity->GetCurDbOperType() == DB_OPER_LOAD)
			{
				if((*itr)->retFlag == S_OK)
				{
					GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pBaseEntity);
					pBaseEntity->SetIsTempEntityFlag(0);

					// �϶��Ǹ����
					if( pBaseEntity->GetCompositeFlag() == "[account]" )
					{
						Account* pTmpAcc = GetGame()->GetEntityManager()->FindAccount(pBaseEntity->GetName());
						if(!pTmpAcc)
						{
							GetGame()->GetEntityManager()->AddAccount((Account*)pBaseEntity);
							// ת��ָ��Ŀ���Ȩ
							(*itr)->pEntity = NULL;
						}
						else
							(*itr)->retFlag = E_FAIL;
					}
					else
					{
						CEntityGroup* pPlayer = GetGame()->GetEntityManager()->FindRootEntity(std::string("[player]"), pBaseEntity->GetGUID());
						if(!pPlayer)
						{
							GetGame()->GetEntityManager()->AddOneRootEntity(pBaseEntity);
							// ת��ָ��Ŀ���Ȩ
							(*itr)->pEntity = NULL;
						}
						else
							(*itr)->retFlag = E_FAIL;
					}
				}
			}

			// ֻ��Load�����ŷ�������
			if(pBaseEntity->GetCurDbOperType() == DB_OPER_LOAD
				|| pBaseEntity->GetCurDbOperType() == DB_OPER_LOAD_NOCACHE
				|| pBaseEntity->GetCurDbOperType() == DB_OPER_PROCEDURE
				|| pBaseEntity->GetCurDbOperType() == DB_OPER_INSERTMAIL_PROCEDURE)
			{
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				msg.Add(pBaseEntity->GetSessionID());
				msg.Add((long)(*itr)->retFlag);
				msg.Add(pBaseEntity->GetCompositeFlag().c_str());
				msg.Add(pBaseEntity->GetGUID());
				msg.Add(pBaseEntity->GetName());
				msg.Add((BYTE)pBaseEntity->GetFindByNameFlag());
				// ������Ϣ
				vector<BYTE> pBA;
				if(pBaseEntity->GetCurDbOperType() == DB_OPER_LOAD
					||pBaseEntity->GetCurDbOperType() == DB_OPER_LOAD_NOCACHE)
					pBaseEntity->AddToByteArray(pBA, true);
				else
					pBaseEntity->AddToByteArray(pBA, false);

				msg.Add((long)pBA.size());
				if(pBA.size())
					msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(GetGame()->GetWSSocket());
			}
			else if(pBaseEntity->GetCurDbOperType() == DB_OPER_SAVE && pBaseEntity->GetDelLeafGuid() != NULL_GUID)
			{
				CEntityGroup* pFather = GetGame()->GetEntityManager()->FindLeafFatherByComFlag(pBaseEntity, 
					pBaseEntity->GetDelLeafFlag());
				if(pFather)
					pFather->ReleaseLeaf(pBaseEntity->GetDelLeafGuid());
			}
			else if(pBaseEntity->GetCurDbOperType() == DB_OPER_DELETE)
			{
				GetGame()->GetEntityManager()->DelOneRootEntity(pBaseEntity->GetCompositeFlag(), pBaseEntity->GetGUID());
			}

#ifdef _DEBUG_SM_INFO_
			sprintf(szOutPutStr, "IN ProcessData() opFlag:%d, Entity[%s] End.",
				pBaseEntity->GetCurDbOperType(),pBaseEntity->GetCompositeFlag().c_str());
			CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif
		}
		SAFE_DELETE(*itr);
	}
	retDataList.clear();
}
//	���ظ��Ĳ���һ���仯�����GUID
void CEntityManager::AddDirtyEntityGuid(const CGUID& guid)
{
	list<CGUID>::iterator itr = std::find(m_listDirtyEntityGuid.begin(), m_listDirtyEntityGuid.end(), guid);
	if(itr == m_listDirtyEntityGuid.end())
	{
		m_listDirtyEntityGuid.push_back(guid);
	}
}
void CEntityManager::AddDirtyEntityName(const char* szName)
{
	if(szName)
	{
		string tmpStr = szName;
		list<string>::iterator itr = std::find(m_listDirtyEntityName.begin(), m_listDirtyEntityName.end(), tmpStr);
		if(itr == m_listDirtyEntityName.end())
		{
			m_listDirtyEntityName.push_back(tmpStr);
		}
	}
}
CBaseEntity* CEntityManager::NewBaseEntity(const string& strComFlag, const CGUID& guid)
{
	CBaseEntity* pEntity = NULL;
	tagEntityBuildInfo* pInfo = GetEntityBuildInfo(strComFlag);
	if(pInfo)
	{
		if(pInfo->lComType == COM_COMPOSITE)
			pEntity = new CEntityGroup(strComFlag, guid);
		else
			pEntity = new CEntity(strComFlag, guid);
	}
	return pEntity;
}
void CEntityManager::DelBaseEntity(CBaseEntity** pEntity)
{
	SAFE_DELETE(*pEntity);
}
// ����ȫ������
void CEntityManager::SaveAllEntity(void)
{
	// ������Ҷ���
	DBEntityComponentMapItr epItr = m_mapEntitys.find(string("[player]"));
	if(epItr != m_mapEntitys.end())
	{
		map<CGUID, CEntityGroup*>::iterator playerItr = epItr->second.begin();
		for(; playerItr != epItr->second.end(); playerItr++)
		{
			CEntityGroup* player = playerItr->second;
			if(player)
			{
				player->SetCurDbOperType(DB_OPER_SAVE);
				player->SetCurDbOperFlag(1);

				CEntityGroup* pGoodsGroup = (CEntityGroup*)FindLeafGroupByComFlag(player, string("[goodsgroup]"));
				if(pGoodsGroup)
				{
					SetEntityAllDbOperType(pGoodsGroup, DB_OPER_DELETE_INSERT);
					ResetEntityAllDbOperFlag(pGoodsGroup, true);
				}

				// ��������
				CEntityGroup* pFriendGroup = (CEntityGroup*)FindLeafGroupByComFlag(player, string("[playerfriendgroup]"));
				if(pFriendGroup)
				{
					SetEntityAllDbOperType(pFriendGroup, DB_OPER_DELETE_INSERT);
					ResetEntityAllDbOperFlag(pFriendGroup, true);
				}

				// С��������
				CEntityGroup* pSpriteGroup = (CEntityGroup*)FindLeafGroupByComFlag(player, string("[playerspritegroup]"));
				if(pSpriteGroup)
				{
					SetEntityAllDbOperType(pSpriteGroup, DB_OPER_DELETE_INSERT);
					ResetEntityAllDbOperFlag(pSpriteGroup, true);
				}

				AddDirtyEntityGuid(player->GetGUID());
			}
		}
	}
	AddLogText("���������ϸ���ݳɹ�(�ȴ��߳̽���)��");

	AccountMapItr accItr = m_mapAccounts.begin();
	for(; accItr != m_mapAccounts.end(); accItr++)
	{
		CEntityGroup* pAcc = accItr->second;
		if(pAcc)
		{
			// �رն�ʱɾ��״̬,����ʱ�������
			((Account*)pAcc)->SetLastLoginTime(-1);

			pAcc->SetCurDbOperType(DB_OPER_SAVE);
			pAcc->SetCurDbOperFlag(1);

			CEntityGroup* lpGroup = (CEntityGroup*)FindLeafGroupByComFlag(pAcc, string("[loginplayergroup]"));
			if(lpGroup)
			{
				SetEntityAllDbOperType(lpGroup, DB_OPER_SAVE);
				ResetEntityAllDbOperFlag(lpGroup, true);
			}

			// ΪLoginplayer��Ӳ�ѯ����
			map<CGUID, CBaseEntity*>::iterator lpItr = lpGroup->GetEntityGroupMap().begin();
			for(; lpItr != lpGroup->GetEntityGroupMap().end(); lpItr++)
			{
				((CEntityGroup*)lpItr->second)->QueryDataRelease();
				((CEntityGroup*)lpItr->second)->RetQueryDataRelease();
				CTempEntityProperty* pGuid = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
				if(pGuid)
				{
					pGuid->SetGuidAttr( lpItr->first );
					pGuid->SetDbRelateType(DB_RELATE_NULL);
					((CEntityGroup*)lpItr->second)->GetQueryDataVec().push_back(pGuid);
				}
			}
			AddDirtyEntityName(pAcc->GetName());
		}
	}
	AddLogText("������һ������ݳɹ�(�ȴ��߳̽���)��");

	epItr = m_mapEntitys.find(string("[scriptvargroup]"));
	if(epItr != m_mapEntitys.end())
	{
		map<CGUID, CEntityGroup*>::iterator accItr = epItr->second.begin();
		for(; accItr != epItr->second.end(); accItr++)
		{
			CEntityGroup* pVarGroup = accItr->second;
			if(pVarGroup)
			{
				ResetEntityAllDbOperFlag(pVarGroup, true);
				SetEntityAllDbOperType(pVarGroup, DB_OPER_DELETE_INSERT);
				AddDirtyEntityGuid(pVarGroup->GetGUID());
			}
		}
	}
	AddLogText("����ȫ�ֱ������ݳɹ�(�ȴ��߳̽���)��");

	epItr = m_mapEntitys.find(string("[factiongroup]"));
	if(epItr != m_mapEntitys.end())
	{
		map<CGUID, CEntityGroup*>::iterator facGroupItr = epItr->second.begin();
		for(; facGroupItr != epItr->second.end(); facGroupItr++)
		{
			CEntityGroup* pFacGroup = facGroupItr->second;
			if(pFacGroup)
			{
				map<CGUID, CBaseEntity*>::iterator facItr = pFacGroup->GetEntityGroupMap().begin();
				for(; facItr != pFacGroup->GetEntityGroupMap().end(); facItr++)
				{
					CEntityGroup *pFaction = (CEntityGroup*)(facItr->second);
					if(pFaction == NULL) continue;

					pFaction->QueryDataRelease();
					pFaction->RetQueryDataRelease();

					// ���ò�ѯ����
					CTempEntityProperty* pFactionGuid = new CTempEntityProperty(string("FactionGuid"), DATA_OBJECT_GUID);
					pFactionGuid->SetGuidAttr(pFaction->GetGUID());
					pFactionGuid->SetDbRelateType(DB_RELATE_NULL);
					pFaction->GetQueryDataVec().push_back(pFactionGuid);

					// ���ó�Ա��ѯ����
					CEntityGroup* pMemberGroup = NULL;
					map<string, CGUID>::iterator guidItr = pFaction->GetGuidByComFlagMap().find(string("[factionmembergroup]"));
					if(guidItr != pFaction->GetGuidByComFlagMap().end())
					{
						map<CGUID, CBaseEntity*>::iterator enItr = pFaction->GetEntityGroupMap().find(guidItr->second);
						if(enItr != pFaction->GetEntityGroupMap().end())
							pMemberGroup = (CEntityGroup*)enItr->second;
					}
					if(pMemberGroup)
					{
						((CEntityGroup*)pMemberGroup)->QueryDataRelease();
						((CEntityGroup*)pMemberGroup)->RetQueryDataRelease();

						CTempEntityProperty* pFactionGuid_members = new CTempEntityProperty(string("FactionGuid_members"), DATA_OBJECT_GUID);
						pFactionGuid_members->SetGuidAttr( pFaction->GetGUID());
						pFactionGuid_members->SetDbRelateType(DB_RELATE_NULL);
						pMemberGroup->GetQueryDataVec().push_back(pFactionGuid_members);
						char szFacGuid[128];
						pFaction->GetGUID().tostring(szFacGuid);
						map<CGUID, CBaseEntity*>::iterator enItr = pMemberGroup->GetEntityGroupMap().begin();
						for(; enItr != pMemberGroup->GetEntityGroupMap().end(); enItr++)
						{
							CGUID tmpGuid;
							enItr->second->GetGuidAttr(string("FactionGuid_members"),  tmpGuid);
							tmpGuid.tostring(szFacGuid);
						}
					}

					CEntityGroup* pApplyGroup = NULL;
					guidItr = pFaction->GetGuidByComFlagMap().find(string("[factionapplygroup]"));
					if(guidItr != pFaction->GetGuidByComFlagMap().end())
					{
						map<CGUID, CBaseEntity*>::iterator enItr = pFaction->GetEntityGroupMap().find(guidItr->second);
						if(enItr != pFaction->GetEntityGroupMap().end())
							pApplyGroup = (CEntityGroup*)enItr->second;
					}
					if(pApplyGroup)
					{
						((CEntityGroup*)pApplyGroup)->QueryDataRelease();
						((CEntityGroup*)pApplyGroup)->RetQueryDataRelease();

						CTempEntityProperty* pFactionGuid_apply = new CTempEntityProperty(string("FactionGuid_apply"), DATA_OBJECT_GUID);
						pFactionGuid_apply->SetGuidAttr( pFaction->GetGUID());
						pFactionGuid_apply->SetDbRelateType(DB_RELATE_NULL);
						pApplyGroup->GetQueryDataVec().push_back(pFactionGuid_apply);
					}
					
					SetEntityAllDbOperType(pFaction, DB_OPER_DELETE_INSERT);
					ResetEntityAllDbOperFlag(pFaction, true);
				}

				pFacGroup->SetCurDbOperType(DB_OPER_SAVE);
				pFacGroup->SetCurDbOperFlag(1);
				AddDirtyEntityGuid(pFacGroup->GetGUID());
			}
		}
	}
	AddLogText("���������ݳɹ�(�ȴ��߳̽���)��");

	GetGame()->SetSaveStartFlag(true);
}

//!	������
bool CEntityManager::ClearEntitys(void)
{
	DBEntityComponentMapItr enItr = m_mapEntitys.begin();
	for (; enItr != m_mapEntitys.end(); enItr++)
	{
		map<CGUID, CEntityGroup*>::iterator entityItr = enItr->second.begin();
		for(; entityItr != enItr->second.end(); entityItr++)
			delete entityItr->second;
	}
	m_mapEntitys.clear();

	AccountMapItr accItr = m_mapAccounts.begin();
	for(; accItr != m_mapAccounts.end(); accItr++)
		delete accItr->second;
	m_mapAccounts.clear();
	AddLogText("���ȫ���������ݳɹ���");
	return true;
}

//! �����Ƿ��ڲ��������и��˺�
bool CEntityManager::FindAccountIsInOperQueue(const string& strAcc)
{
	list<string>::iterator itr = std::find(m_listDirtyEntityName.begin(), m_listDirtyEntityName.end(), strAcc);
	if(itr != m_listDirtyEntityName.end())
		return true;
	return false;
}
bool CEntityManager::FindEntityIsInOperQueue(const CGUID& guid)
{
	list<CGUID>::iterator itr = std::find(m_listDirtyEntityGuid.begin(), m_listDirtyEntityGuid.end(), guid);
	if(itr != m_listDirtyEntityGuid.end())
		return true;

	itr = std::find(m_listSaveBeginEntityGuid.begin(), m_listSaveBeginEntityGuid.end(), guid);
	if(itr != m_listSaveBeginEntityGuid.end())
		return true;

	return false;
}