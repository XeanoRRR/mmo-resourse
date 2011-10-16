// E:\Metempsychosis\App\BaseObject.cpp : implementation file
//

#include "stdafx.h"
#include "BaseObject.h"
#include "player.h"
#include "Build.h"
#include "CityGate.h"
#include "region.h"
#include "../public/public.h"
#include "ServerRegion.h"
#include "PlayerMan/PlayerMan.h"



// CBaseObject 

CBaseObject::CBaseObject()
{
	m_lType = 0;
	//m_lID = 0;
	m_lGraphicsID = 0;
	m_strName = "";
	m_pFather = NULL;

	m_bIncludeChild = true;

	m_byCanHide = 0; // �ܷ�����
	m_byHideFlag = 0; // ��ǰ����״̬
}

CBaseObject::~CBaseObject()
{
	m_lEventIDCount = 0;
	//DeleteAllChildObject();
	ClearAllEventNode();
	//test.������Ϣ��������ͷŶ����ʱ��û�аѶ�ʱ��ע���꣬��ʾ���ش���
	int nTimeNum = GameManager::GetInstance()->Cancel(this);
	/*if(nTimeNum > 0 && !g_bGameThreadExit)
	{
	char pszErrorInfo[512]="";
	_snprintf(pszErrorInfo,512,"���ش����ͷŶ���(Type:%d)ʱ����%d����ʱ��δע��!",GetType(),nTimeNum);
	PutDebugString(pszErrorInfo);
	}*/
}

/////////////////////////////////////////////////////////////////////
// ��ȡ�ӿ�
/////////////////////////////////////////////////////////////////////

// ��ȡ
BOOL CBaseObject::Load()
{
	return true;
}

// �洢
BOOL CBaseObject::Save()
{
	return true;
}

// ��ӵ�CByteArray
bool CBaseObject::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
{
	_AddToByteArray(pByteArray, GetType());
	_AddToByteArray(pByteArray, GetExID());
	_AddToByteArray(pByteArray, GetGraphicsID());
	_AddToByteArray(pByteArray, (char*)GetName());
	return true;
}

// ����
bool CBaseObject::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	m_lType = _GetLongFromByteArray(pByte, pos);
	_GetBufferFromByteArray(pByte, pos, m_guid);
	m_lGraphicsID = _GetLongFromByteArray(pByte, pos);

	char str[256];
	m_strName =_GetStringFromByteArray(pByte, pos, str);

	return true;
}

// ��ӵ�CByteArray
bool CBaseObject::CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData)
{
	// �������书��
	setWriteDB.AddToByteArray((BYTE)m_byCanHide);
	setWriteDB.AddToByteArray((BYTE)m_byHideFlag);

	setWriteDB.AddToByteArray(GetType());
	setWriteDB.AddToByteArray(GetExID());
	setWriteDB.AddToByteArray(GetGraphicsID());
	setWriteDB.AddToByteArray((char*)GetName());
	return true;
}

// ����
bool CBaseObject::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	m_byCanHide = setReadDB.GetByteFromByteArray();
	m_byHideFlag = setReadDB.GetByteFromByteArray();

	m_lType = setReadDB.GetLongFromByteArray();
	setReadDB.GetBufferFromByteArray(m_guid);
	m_lGraphicsID =setReadDB.GetLongFromByteArray();
	char str[256];
	m_strName =setReadDB.GetStringFromByteArray(str,256);
	return true;
}

/////////////////////////////////////////////////////////////////////
// �ӽڵ��б�������������
/////////////////////////////////////////////////////////////////////

//������object!
bool CBaseObject::FindChildObject(CBaseObject* p)
{
	return false;
}

//������object!
CBaseObject* CBaseObject::FindChildObject(long typeID,const CGUID& guid)
{
	return NULL;
}

// ɾ����OBJECT
void CBaseObject::DeleteChildObject(CBaseObject* pObj)
{
}

// ɾ����OBJECT
void CBaseObject::DeleteChildObject(long typeID,long lID,CGUID& guExID)
{
}

// ɾ��������OBJECT(pExpect = ����)
void CBaseObject::DeleteAllChildObject(CBaseObject* pExpect)
{
}

// �Ƴ�OBJECT
void CBaseObject::RemoveObject(CBaseObject* pObject)
{
	//m_listObject.remove(pObject);
}

VOID CBaseObject::RemoveObject( LONG lType, LONG lID )
{
}

// ���OBJECT
void CBaseObject::AddObject(CBaseObject* pObject)
{
	//pObject->SetFather(this);
	//m_listObject.push_back(pObject);
}

#ifdef _DEBUG_TIMER_VAR_
stdext::hash_map<tagTimerVar*, long> CBaseObject::m_TimerFlag;
#endif

tagTimerVar* CBaseObject::CreateTimerVar(BYTE Flag)
{
	tagTimerVar *pRe = (tagTimerVar*)M_ALLOC(sizeof(tagTimerVar));
#ifdef _DEBUG_TIMER_VAR_
	new(pRe)tagTimerVar(Flag);

	if(1024 * 1024 * 64 < m_TimerFlag.size())
		m_TimerFlag.clear();
	else
	{
		stdext::hash_map<tagTimerVar*, long>::iterator itrFlag = m_TimerFlag.find(pRe);
		if (itrFlag != m_TimerFlag.end())
		{
			//! ����һ�εķ����������ƣ�������Ա���4�α��
			itrFlag->second = ((itrFlag->second << 8) | Flag);
		}
		else
			m_TimerFlag[pRe] = Flag;
	}
#else
	new(pRe)tagTimerVar();
#endif
	return pRe;
}

void CBaseObject::ReleaseTimerVar(tagTimerVar **ppTimer)
{
	if (NULL != ppTimer && NULL != *ppTimer)
	{
		tagTimerVar* pTimvar = *ppTimer;
#ifdef _DEBUG_TIMER_VAR_
		BYTE TimerFlag = 0xFF;
		stdext::hash_map<tagTimerVar*, long>::iterator itrFlag = m_TimerFlag.find(pTimvar);
		if (itrFlag != m_TimerFlag.end())
		{
			TimerFlag = (BYTE)(itrFlag->second & 0x000000FF);
			if (TimerFlag != (BYTE)pTimvar->DebugFlag)
			{
				char szTmp[128] = {0};
				_snprintf(szTmp, 128, "CBaseObject::ReleaseTimerVar TimerFlag[%x] != DebugFlag[%x]", itrFlag->second, pTimvar->DebugFlag);
				OutMemInfo("log\\", szTmp);
			}
		}
		char szInfo[128] = {0};
		_snprintf(szInfo, 128, "Pointer=%x m_TimerFlag=%d DebugFlag=%d TimerType=%d lvar=%d lvar1=%d", (DWORD)pTimvar, TimerFlag, pTimvar->DebugFlag, (long)pTimvar->TimerType, pTimvar->lvar, pTimvar->lvar1);
		M_FREE_INFO(pTimvar, sizeof(tagTimerVar), szInfo);
#else
		M_FREE(pTimvar, sizeof(tagTimerVar));
#endif
		*ppTimer = NULL;
	}
}


void CBaseObject::OnTimerCancel(DWORD timerid,const void* var)
{
	if(var)
	{
		tagTimerVar* pTimvar = (tagTimerVar*)var;
		if (pTimvar->pvar!= NULL)
		{
			M_FREE(pTimvar->pvar, pTimvar->pvar_size);
			pTimvar->pvar_size = 0;
		}

		ReleaseTimerVar(&pTimvar);
	}
}

//ע���¼�
void CBaseObject::RegisterEvent(eEventType nType,tagEvenRegVar &RegVar)
{
	EventVar& EventRegVar = m_Events[nType];
	EventRegVar[RegVar.lEventID] = RegVar;
}
//ע���¼�
void CBaseObject::UnRegisterEvent(eEventType nType,long lEventID)
{
	tagDelEventNode DelNode ={nType,lEventID};
	m_DelEventNodes.push_back(DelNode);
}

//�����¼�
void CBaseObject::ProduceEvent(eEventType nType,long lvar1,long lVar2,long lVar3,long lvar4, void* pVar3)
{
	if(m_Events.find(nType) == m_Events.end()) return;

	EventVar RegVar = m_Events[nType];
	if(RegVar.size() > 0)
	{
		static tagProcVar ProcVar;
		itEventVar it = RegVar.begin();
		for(;it != RegVar.end();it++)
		{
			tagEvenRegVar& RegVar = (*it).second;			
			//��������
			ProcVar.lID = RegVar.lID2;
			ProcVar.lType = RegVar.lType2;
			ProcVar.lVar1 = lvar1;
			ProcVar.lVar2 = lVar2;
			ProcVar.lVar3 = lVar3;
			ProcVar.lVar4 = lvar4;
			ProcVar.pVar = pVar3;


			//�����¼�
			if(RegVar.ID1 == this->GetExID() && RegVar.lType1 == GetType())
			{
				Proc(nType,&ProcVar);
			}
			else
			{

				CServerRegion* pRegion = dynamic_cast<CServerRegion*>(m_pFather);
				if (pRegion)
				{
					CBaseObject* pObj = pRegion->FindChildObject((OBJECT_TYPE)RegVar.lType1,RegVar.ID1);							
					if(pObj)
						pObj->Proc(nType,&ProcVar);
				}

			}
		}

		//����ɾ���Ľڵ�
		ClearEventNode();

	}
}

//ɾ��ע���¼��ڵ�
void CBaseObject::ClearEventNode()
{
	if(m_DelEventNodes.size() > 0)
	{
		itDelEvent itDel = m_DelEventNodes.begin();
		for(;itDel != m_DelEventNodes.end();itDel++)
		{
			tagDelEventNode &DelNode = (*itDel);
			itEvent itEvent = m_Events.find(DelNode.type);
			if(itEvent != m_Events.end())
			{
				itEventVar itEventVar = (*itEvent).second.find(DelNode.lEventID);
				if(itEventVar != (*itEvent).second.end())
				{
					(*itEvent).second.erase(itEventVar);
				}
				else
				{
					PutDebugString("Delete the Event Error.");
				}
			}
		}
		m_DelEventNodes.clear();
	}
}

void CBaseObject::ClearAllEventNode()
{
	m_Events.clear();
}
bool CBaseObject::CodeToDataBlock_ForClient(DBWriteSet& setWriteDB,bool bExData)		// (������)��ӵ�CByteArray
{
	// �������书��
	setWriteDB.AddToByteArray((BYTE)m_byHideFlag);

	setWriteDB.AddToByteArray(GetType());
	setWriteDB.AddToByteArray(GetExID());
	setWriteDB.AddToByteArray(GetGraphicsID());
	CRegion* pLocalRgn = static_cast<CRegion*>(GetFather());
	if(!bExData && m_byHideFlag == 1 && pLocalRgn && pLocalRgn->GetRgnHideFlag() != 2) // ����
		setWriteDB.AddToByteArray((char*)CGlobeSetup::GetSetup()->szHideName);
	else
		setWriteDB.AddToByteArray((char*)GetName());
	return true;
}
void CBaseObject::SetHideFlag(BYTE flag)
{
	CRegion* pLocalRgn = static_cast<CRegion*>(GetFather());

	if(pLocalRgn && pLocalRgn->GetRgnHideFlag() == 2)
		m_byHideFlag = 0;
	else
		m_byHideFlag = flag;

	CMessage msg(MSG_S2W_OTHER_UPDATE_HIDE_FLAG);
	msg.Add(GetExID()); // player guid
	msg.Add((BYTE)m_byHideFlag);
	msg.Send();

	CMessage notifyMsg(MSG_S2C_OTHER_UPDATE_HIDE_FLAG);
	notifyMsg.Add(GetExID()); // player guid
	notifyMsg.Add((BYTE)m_byHideFlag);
	if(m_byHideFlag == 1 && pLocalRgn && pLocalRgn->GetRgnHideFlag() != 2)
		notifyMsg.Add(CGlobeSetup::GetSetup()->szHideName);
	else
		notifyMsg.Add(GetName());
	notifyMsg.SendToAround(static_cast<CShape*>(this), static_cast<CPlayer*>(this));

	// ˢ�½�ɫ����״̬
	GetInst(CPlayerMan).RefreshElem(PET_HIDENAME, static_cast<CPlayer*>(this));
}