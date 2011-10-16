//=============================================================================
/**
 *  file: GameManager.cpp
 *
 *  Brief:��Ϸʵ��Ĺ����࣬�����¼�����Ϣ
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-5-17
 */
//=============================================================================
#include "StdAfx.h"
#include "GameManager.h"

#include "Game.h"
#include "../App/BaseObject.h"
#include "../App/Business/CBusinessManager.h"
#include "../App/Collection.h"
#include "../App/Goods/CGoods.h"
#include "../App/Mail/MailManager.h"
#include "../App/Monster.h"
#include "../App/PetCtrl.h"
#include "../App/Player.h"
#include "../App/RgnManager.h"
#include "../App/ServerRegion.h"

#include "../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
//CGUIDEvent

GameManager::CGUIDEvent::CGUIDEvent(GameManager *pManager)
:m_ObjectType((OBJECT_TYPE)0)
,m_Guid(CGUID::GUID_INVALID)
,m_nCount(0)
,m_pManager(pManager)
{
	assert(m_pManager);
	m_GUIDTimers.clear();

}

GameManager::CGUIDEvent::CGUIDEvent(void* pParam)
:m_ObjectType((OBJECT_TYPE)0)
,m_Guid(CGUID::GUID_INVALID)
,m_nCount(0)
,m_pManager((GameManager*)pParam)
{
	assert(pParam);
	m_GUIDTimers.clear();
}

//��ʱ��Ӧ
long GameManager::CGUIDEvent::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
	CBaseObject *pBaseObject = m_pManager->FindObject(m_ObjectType,m_Guid);
	if(pBaseObject)
	{
		pBaseObject->OnTimeOut(timerid,curtime,var);
	}
	m_GUIDTimers.erase(timerid);
	if(--m_nCount == 0)
	{
		m_pManager->FreeGUIDEvent(this);
	}
	return 0;
}

//����ʱ����ȡ���ĵ���
void GameManager::CGUIDEvent::OnTimerCancel(DWORD timerid,const void* var)
{
	CBaseObject *pBaseObject = m_pManager->FindObject(m_ObjectType,m_Guid);
	if(pBaseObject)
	{
		pBaseObject->OnTimerCancel(timerid,var);
	}
	m_GUIDTimers.erase(timerid);
	if(--m_nCount == 0)
	{
		m_pManager->FreeGUIDEvent(this);
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//GameManager

GameManager* GameManager::instance = NULL;
GameManager::GameManager(void)
:m_pTimer(0),
g_RgnManager(NULL),
g_MailManager(NULL)
{
	g_RgnManager		= OBJ_CREATE(CRgnManager);//new CRgnManager;
	g_MailManager		= OBJ_CREATE(CMailManager);
	m_LoginSession.clear();
}

GameManager::~GameManager(void)
{
}
CRgnManager*		GameManager::GetRgnManager(void)
{
	return g_RgnManager;
}
CMailManager*		GameManager::GetMailManager(void)
{
	return g_MailManager;
}


GameManager* GameManager::GetInstance()
{
	if(instance == NULL)
	{
		instance = OBJ_CREATE(GameManager);//new GameManager();
	}
	return instance;
}

bool   GameManager::Initialize(unsigned long nMaxFreeGuidEventNum)
{
	m_pTimer = new Timer(1024,32);
	int i = 0;
	for(;i < (int)nMaxFreeGuidEventNum;i++)
	{
		CGUIDEvent* pGuidEvent = OBJ_CREATE_PVOID(CGUIDEvent, (void*)this);
		m_FreeGUIDEvents.push_back(pGuidEvent);
	}
	return true;
}

//Ϊ��Ϸ���������ڴ������ԣ������С��һ�η�������Ķ�����������һ�η���Ĵ�����
void   GameManager::SetPolicy(void)
{
	GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
	//m_MultiMemPool.SetInitialTileNum(sizeof(CMonster),			pSetup->Monster_Policy.ObjNum * pSetup->Monster_Policy.blockNum);
	//m_MultiMemPool.SetInitialTileNum(sizeof(CPlayer),			pSetup->Player_Policy.ObjNum * pSetup->Player_Policy.blockNum);
}

void   GameManager::Release()
{
	itGUIDEvent itEvent = m_GuidEvents.begin();
	for(;itEvent != m_GuidEvents.end();itEvent++)
	{
		CGUIDEvent *pGuidEvent = (*itEvent).second;
		m_pTimer->Cancel(pGuidEvent);
	}
	
	list<CGUIDEvent*>::iterator  it = m_FreeGUIDEvents.begin();
	for(;it !=  m_FreeGUIDEvents.end();it++)
	{
		OBJ_RELEASE(CGUIDEvent, (*it));
	}


	OBJ_RELEASE(CRgnManager, g_RgnManager);
	g_RgnManager = NULL;
	OBJ_RELEASE(CMailManager, g_MailManager);
	g_MailManager = NULL;

	//CAIFactory::Clear();
	GetInst( Business::CBusinessManager ).Shutdown();

	CNewSkillFactory::RealeaseModuParam();

	SAFE_DELETE(m_pTimer);

	return;
}

void GameManager::ReleaseInstance()
{
	OBJ_RELEASE(GameManager, instance);
}

long GameManager::Schedule(	CGameEvent* const pEvent,
			  const void *arg,
			  unsigned long futuretime,
			  unsigned long interval)
{
	return m_pTimer->Schedule(pEvent,arg,futuretime,interval);
}

//��type+guid�ķ�ʽ���ƶ�ʱ��
long GameManager::Schedule(	OBJECT_TYPE objecttype,
				unsigned long nID,
			  const CGUID& guid,
			  const void *arg,
			  unsigned long futuretime,
			  unsigned long interval)
{
	CGUIDEvent *pGuidEvent = NULL;
	//Ѱ���Ƿ��Ѿ�����guid����CGUIDEvent
	itGUIDEvent it = m_GuidEvents.find(guid);
	if(it != m_GuidEvents.end())
	{
		pGuidEvent = (*it).second;
		if(pGuidEvent->m_ObjectType != objecttype)
		{
			OUTPUTLOG("GameManager::Schedule(...),objecttype��ƥ��.");
		}
	}
	else
	{
		//���û�������һ��
		pGuidEvent = AllocGUIDEvent();
		if(pGuidEvent)
		{
			pGuidEvent->m_nCount = 0;
			pGuidEvent->m_ObjectType = objecttype;
			pGuidEvent->m_nID = nID;
			pGuidEvent->m_Guid = guid;
		}
	}

	if(pGuidEvent)
	{
		m_GuidEvents[guid] = pGuidEvent;
		long timerid = m_pTimer->Schedule(pGuidEvent,arg,futuretime,interval);
		if(timerid != -1)
		{
			pGuidEvent->m_nCount++;
			pGuidEvent->m_GUIDTimers.insert(timerid);
		}
		return timerid;
	}
	return 0;
}

int GameManager::Cancel(CGameEvent* const pEvent)
{
	return m_pTimer->Cancel(pEvent);
}

//ȡ��guid�Ķ�ʱ��
int GameManager::Cancel(const CGUID& guid)
{
	itGUIDEvent it = m_GuidEvents.find(guid);
	if(it != m_GuidEvents.end())
	{
		CGUIDEvent *pGuidEvent = (*it).second;
		if(pGuidEvent)
		{
			//ȡ�����������timerid
			long timernum = 0;
			CGUIDEvent::itGUIDTimer it = pGuidEvent->m_GUIDTimers.begin();
			for(;it != pGuidEvent->m_GUIDTimers.end();it++)
			{
				timernum++;
				m_pTimer->Cancel((*it));
			}
			return timernum;
		}
	}
	return 0;
}

int GameManager::Cancel(long timerid, const void** arg)
{
	return m_pTimer->Cancel(timerid,arg);
}

void GameManager::Run()
{
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GameManager::Run() Start");
#endif

	DWORD curtime = timeGetTime();
	if(m_pTimer)
		long cancelnum = m_pTimer->Expire(curtime);

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GameManager::Run() End");
#endif
}

//! ����һ���������͵Ķ���
void* GameManager::CreateBlock(long lSize)
{
	//if (0 < lSize)
	//	return m_MultiMemPool.Multi_Alloc(lSize, _FILE_AND_LINE_);
	return NULL;
}


//! �ͷ�һ������
void GameManager::FreeBlock(void* pBlock, long lSize)
{
	//m_MultiMemPool.Multi_Free(pBlock, lSize);
}

CMonster* AllocMonster()
{

	return 0;
}
//�ͷ�һ��CMonster*
void FreeMoster(CMonster*)
{

}

//����һ��CGoods*
CGoods*	AllocGoods()
{
	return 0;
}
//�ͷ�һ��CGoods*
void FreeGoods(CGoods*)
{

}

//����ƥ��type+guid��CBaseObject;
inline
CBaseObject	*GameManager::FindObject(OBJECT_TYPE objecttype,const CGUID& guid)
{
	CBaseObject* pObj = NULL;
	switch(objecttype)
	{
	case TYPE_GOODS:		// ��Ʒ
		{
			OBJMAP::iterator ite = m_GoodsList.find(guid);
			if (m_GoodsList.end() != ite)
				pObj = ite->second;
			else
				pObj = NULL;
		}
		break;

	case TYPE_MONSTER:		// ����
		{
			OBJMAP::iterator ite = m_MonsterList.find(guid);
			if (m_MonsterList.end() != ite)
				pObj = ite->second;
			else
				pObj = NULL;
		}
		break;

	case TYPE_PLAYER:		// ���
		{
			OBJMAP::iterator ite = m_PlayerList.find(guid);
			if (m_PlayerList.end() != ite)
				pObj = ite->second;
			else
				pObj = NULL;
		}
		break;

	case TYPE_PET:			// ����
		{
			OBJMAP::iterator ite = m_PetList.find(guid);
			if (m_PlayerList.end() != ite)
				pObj = ite->second;
			else
				pObj = NULL;
		}
		break;

	case TYPE_REGION:		// ��ͼ
		{
			OBJMAP::iterator ite = m_RegionList.find(guid);
			if (m_RegionList.end() != ite)
				pObj = ite->second;
			else
				pObj = NULL;
		}
		break;

	case TYPE_BUILD:
		{
			OBJMAP::iterator ite = m_BuildList.find(guid);
			if (m_BuildList.end() != ite)
				pObj = ite->second;
			else
				pObj = NULL;
		}
		break;
	case TYPE_CITYGATE:
		{
			OBJMAP::iterator ite = m_GateList.find(guid);
			if (m_GateList.end() != ite)
				pObj = ite->second;
			else
				pObj = NULL;
		}
		break;
	}
	if (NULL != pObj)
	{
		if (objecttype == pObj->GetType())
		{
			return pObj;
		}
	}
	
	return NULL;
}

//����һ��CGUIDEvent*
GameManager::CGUIDEvent*	GameManager::AllocGUIDEvent()
{
	if(m_FreeGUIDEvents.size() == 0)
	{
		return OBJ_CREATE_PVOID(CGUIDEvent, (void*)this);
	}
	CGUIDEvent* pGuidEvent = m_FreeGUIDEvents.front();
	m_FreeGUIDEvents.pop_front();
	return pGuidEvent;
}
//�ͷ�һ��CGUIDEvent*
void GameManager::FreeGUIDEvent(GameManager::CGUIDEvent* guidevent)
{
	if(m_FreeGUIDEvents.size() >= m_nMaxFreeGuidEventNum)
	{
		OBJ_RELEASE(CGUIDEvent, guidevent);
	}
	m_FreeGUIDEvents.push_back(guidevent);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//�±���ʵ�����Ϣ������
void GameManager::SendMessage(OBJECT_TYPE objecttype,unsigned long nID,const CGUID& guid,
				 unsigned long type,const void* var1,const void* var2)
{
	CBaseObject *pBaseObject = FindObject(objecttype,guid);
	if(pBaseObject)
	{
		//pBaseObject->Proc(type,var1,var2);
	}
}
void GameManager::SendMessage(CGameEvent* pGameEvent,
				 unsigned long type,const void* var1,const void* var2)
{
	if(pGameEvent)
	{
		//pGameEvent->Proc(type,var1,var2);
	}
}


//���һ����½�Ự,���ؼ���
long GameManager::AddLoginSession(long lSocketID)
{
	static long lCount = 0;
	lCount++;
	m_LoginSession[lSocketID]=lCount;
	return lCount;
}
//�Ƴ�һ������ֵ��session,���ر����sokcetid
long GameManager::RemoveLoginSessionByCountNum(long lNum)
{
	long lSocketID = 0;
	itMapLoginLL it = m_LoginSession.begin();
	for(;it != m_LoginSession.end();it++)
	{
		if((*it).second == lNum)
		{
			lSocketID = (*it).first;
			m_LoginSession.erase(it);
			break; 
		}
	}
	return lSocketID;
}
//����socketID�Ƴ��Ự
void GameManager::RemoveLoginSessionBySocketID(long lSocketID)
{
	itMapLoginLL it = m_LoginSession.find(lSocketID);
	if(it != m_LoginSession.end())
		m_LoginSession.erase(it);
}

void GameManager::PutObjNumInfo(const char* pszFileName)
{
	//char str[256];
	//PutStringToFile(pszFileName,"ObjNum Start:");
	//_snprintf(str,256, "GoodsNum:%d",m_GoodsList.size());
	//PutStringToFile(pszFileName,str);

	//_snprintf(str, 256, "CollectionNum:%d",m_CollectionList.size());
	//PutStringToFile(pszFileName,str);

	//_snprintf(str, 256, "MonsterNum:%d",m_MonsterList.size());
	//PutStringToFile(pszFileName,str);

	//_snprintf(str, 256, "PlayerNum:%d",m_PlayerList.size());
	//PutStringToFile(pszFileName,str);

	//_snprintf(str, 256, "RegionNum:%d",m_RegionList.size());
	//PutStringToFile(pszFileName,str);

	//_snprintf(str, 256, "BuildNum:%d",m_BuildList.size());
	//PutStringToFile(pszFileName,str);

	//_snprintf(str, 256, "GateNum:%d",m_GateList.size());
	//PutStringToFile(pszFileName,str);

	//_snprintf(str, 256, "PetNum:%d",m_PetList.size());
	//PutStringToFile(pszFileName,str);

    Log4c::Trace(ROOT_MODULE,"ObjNum Start:");
    Log4c::Trace(ROOT_MODULE, "GoodsNum:%d",m_GoodsList.size());
    Log4c::Trace(ROOT_MODULE, "CollectionNum:%d",m_CollectionList.size());
    Log4c::Trace(ROOT_MODULE, "MonsterNum:%d",m_MonsterList.size());
    Log4c::Trace(ROOT_MODULE, "PlayerNum:%d",m_PlayerList.size());
    Log4c::Trace(ROOT_MODULE, "RegionNum:%d",m_RegionList.size());
    Log4c::Trace(ROOT_MODULE, "BuildNum:%d",m_BuildList.size());
    Log4c::Trace(ROOT_MODULE, "GateNum:%d",m_GateList.size());
    Log4c::Trace(ROOT_MODULE, "PetNum:%d",m_PetList.size());

}