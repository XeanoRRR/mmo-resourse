//=============================================================================
/**
*  file: GameManager.h
*
*  Brief:��Ϸʵ��Ĺ����࣬�����¼�����Ϣ
*
*  Authtor:wangqiao
*	
*	Datae:2007-5-17
*/
//=============================================================================
#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__
#pragma once
#include <set>
#include "..\public\GameEvent.h"
#include "..\public\TimerQueue.h"
#include "..\public\BaseDef.h"
#include "..\public\GUID.h"
#include "..\AppServer\goods\CGoods.h"
#include "..\AppServer\Build.h"
#include "..\Public\UseMemManage.h"

typedef CTimerQueue<CGameEvent*> Timer;
class CBaseObject;
class CPlayer;
class CMonster;

// ȫ�ֶ���

class CGlobalRgnManager;	
class CRgnManager;
class CMailManager;
typedef hash_map<CGUID, CBaseObject*, hash_guid_compare> OBJMAP;

class GameManager : public BaseMemObj
{
public:
	GameManager(void);
	~GameManager(void);
private:
	//ȱʡ���¼��࣬��������type+guid�ĵ��ȷ�ʽ
	class CGUIDEvent: public BaseMemObj, public CGameEvent
	{
	public:
		OBJECT_TYPE		m_ObjectType;
		unsigned long	m_nID;
		CGUID			m_Guid;
		//����ֵ�������ۼƸ�Eventʹ�õĴ���
		long	m_nCount;
		//���������GUID���ƵĶ�ʱ��,������ȡ����ʱ��ʹ��
		typedef set<unsigned long>::iterator itGUIDTimer;
		set<unsigned long>	m_GUIDTimers;
		GameManager	*m_pManager;
	public:
		//ȱʡ�Ĺ��캯��
		CGUIDEvent(GameManager *pManager);
		~CGUIDEvent(){}

		//��ʱ��Ӧ
		long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
		//����ʱ����ȡ���ĵ���
		void OnTimerCancel(DWORD timerid,const void* var);
		//������Ϣ��Ӧ
		void OnMessage(CMessage*){}
		//��ʱ��Ӧ
		//����ʵ�����Ϣ����
		long Proc(eEventType type,const tagProcVar *pVar){return 0;}
	};

private:
	static GameManager* instance;

	Timer	*m_pTimer;

	typedef hash_map<CGUID,CGUIDEvent*,hash_guid_compare> GUIDEvents;
	typedef hash_map<CGUID,CGUIDEvent*,hash_guid_compare>::iterator itGUIDEvent;
	GUIDEvents	m_GuidEvents;

	//����Ԥ����GUIDEvent�ڵ�
	unsigned long m_nMaxFreeGuidEventNum;
	list<CGUIDEvent*>	m_FreeGUIDEvents;

	//�����ɫ�ĵ�½�Ự
	typedef map<long,long> MapLoginLL;
	typedef map<long,long>::iterator itMapLoginLL;

	//��½��<socketid,count>
	MapLoginLL	m_LoginSession;

public:
	static GameManager* GetInstance();

	bool   Initialize(unsigned long nMaxFreeGuidEventNum = 10);
	void   Release();
	static void ReleaseInstance();

	//����һ��CGUIDEvent*
	CGUIDEvent*	AllocGUIDEvent();
	//�ͷ�һ��CGUIDEvent*
	void	FreeGUIDEvent(CGUIDEvent*);

	//��<type>����Ԥ��һ����ʱ��,�����ʱ����Ҫ�ھ���ʱ��futuretime����
	//interval:�������0,��ʾ����һ�������Զ�ʱ��,��һ�����ʱ��
	//��ʾ��ʱ����һ�δ����ļ��ʱ��,�ú�������-1��ʾ����ʧ��
	//arg��һ���û��Զ������	
	long Schedule(	CGameEvent* const pEvent,
		const void *arg,
		unsigned long futuretime,
		unsigned long interval = 0);

	//����ֵ��ʾȡ���Ķ�ʱ������
	int Cancel(CGameEvent* const pEvent);
	//ȡ������<m_dwTimerID>����<timerid>�Ķ�ʱ��
	//���arg�����,��Ҫ���õ���ʱ�����õ�<arg>����
	//����ֵ��ʾȡ���Ķ�ʱ������
	int Cancel(long timerid, const void** arg = 0);

	//��type+guid�ķ�ʽ���ƶ�ʱ��
	long Schedule(	OBJECT_TYPE objecttype,
		unsigned long nID,
		const CGUID& guid,
		const void *arg,
		unsigned long futuretime,
		unsigned long interval = 0);
	//ȡ��guid�Ķ�ʱ��
	int Cancel(const CGUID& guid);

	void Run();

//////////////////////////////////////////////////////////////////////////////////////////////
//�±ߵı����ͺ������ڹ�����Ϸ�ľ���ʵ����
public:
	//! ���÷������
	void   GameManager::SetPolicy(void);

	//! ����һ��object
	CBaseObject* CreateObject(long typeID, const CGUID& guid, const char *pInfo);
	//! �ͷ�һ��object
	void FreeObject(CBaseObject** ppObj);

	//! һЩ�����Ķ���
	enum eOtherType
	{
		eOT_TimerVar,	//! ��ʱ����������
	};
	//! ����һ���������͵Ķ���
	void*		CreateBlock(long lSize);
	//! �ͷ�һ������
	void		FreeBlock(void* pBlock, long lSize);

	//���һ����½�Ự,���ؼ���
	long AddLoginSession(long lSocketID);
	//�Ƴ�һ������ֵ��session,���ر����sokcetid
	long RemoveLoginSessionByCountNum(long lNum);
	//����socketID�Ƴ��Ự
	void RemoveLoginSessionBySocketID(long lSocketID);

	void PutObjNumInfo(const char* pszFileName);
	
public:
	//! ����ƥ��type+guid��CBaseObject;
	CBaseObject	*FindObject(OBJECT_TYPE objecttype,const CGUID& guid);

#ifdef DEBUG_NEW
	//! buf�ռ�Ҫ����512BYTE
	void	GetPoolStateString(char *buf, long bufSize);
#endif

	
public:
	CRgnManager*		GetRgnManager(void);
private:
	// ȫ�ֶ���
	CRgnManager*		g_RgnManager;
public:
	CMailManager*		GetMailManager(void);
private:
	CMailManager*		g_MailManager;

private:
	OBJMAP m_GoodsList;					// TYPE_GOODS
	OBJMAP m_MonsterList;				// TYPE_MONSTER
	OBJMAP m_CollectionList;			// TYPE_COLLECTION
	OBJMAP m_PlayerList;				// TYPE_PLAYER
	OBJMAP m_RegionList;				// TYPE_REGION
	OBJMAP m_BuildList;					// TYPE_BUILD
	OBJMAP m_GateList;					// TYPE_CITYGATE
	OBJMAP m_OtherList;					// ��������

//////////////////////////////////////////////////////////////////////////////////////////////
//�±���ʵ�����Ϣ������
	void SendMessage(OBJECT_TYPE objecttype,unsigned long nID,const CGUID& guid,
					unsigned long type,const void* var1,const void* var2);
	void SendMessage(CGameEvent* pGameEvent,
					unsigned long type,const void* var1,const void* var2);
};

#define TIMER_SCHEDULE GameManager::GetInstance()->Schedule
#define TIMER_CANCEL GameManager::GetInstance()->Cancel

//������Ϣ���ͺ�
#define SEND_MESSAGE GameManager::GetInstance()->SendMessage





#endif

