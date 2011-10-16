//WorldServerSession.h/////////////////////////////////////////////////////////////////////
//������:�Ự����ʵ����
//Author:������
//Create Time:2008.11.03

#include "stdafx.h"
#include "../../../../nets/networld/Message.h"
#include "WorldServerSession.h"
#include "CSession.h"
#include "CSessionFactory.h"
#include "../dbentity/entitygroup.h"
#include "../DBEntity/EntityManager.h"
#include "../Country/Country.h"
#include "../Country/CountryHandler.h"
#include "../Player.h"
#include "..\Organizingsystem\organizingctrl.h"
#include "../Mail/Mail.h"
#include "../Linkman/LinkmanSystem.h"
#include "../script/VariableList.h"
#include "../business/CBusinessManager.h"
#include "..\SMclient.h"

//##ModelId=42266DFF0242
CWorldServerSession::CWorldServerSession(DWORD dwLifeTime)
: CSession(NULL_GUID, dwLifeTime),m_lOperCount(0)
{
	// TODO: Add your specialized code here.
}


//##ModelId=42266E1A0109
CWorldServerSession::~CWorldServerSession()
{
	// TODO: Add your specialized code here.
	Release();
}
VOID CWorldServerSession::Release()
{
	vector<CTempEntityProperty*>::iterator itr = m_pTempDataMgr.begin();
	for(; itr != m_pTempDataMgr.end(); itr++)
		SAFE_DELETE(*itr);
	m_pTempDataMgr.clear();
}

void CWorldServerSession::TempDataRelease()
{
	vector<CTempEntityProperty*>::iterator epItr = m_pTempDataMgr.begin();
	for(; epItr != m_pTempDataMgr.end(); epItr++)
	{
		delete *epItr;
	}
	m_pTempDataMgr.clear();
}

//##ModelId=41DCD273036B
VOID CWorldServerSession::AI()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	//##AI���ֻ������ж����е�Plug�Ƿ���Ч���������Ч��Plug��
	//##��ִ��һЩ��������֪ͨ������Plug�󣬰Ѹ�Plug�ϵ���
	if( m_bSessionStarted == TRUE )
	{
		if( m_bSessionEnded || m_bSessionAborted )
		{
			return;
		}
		if( m_dwLifeTime && timeGetTime() >= m_dwStartingTimeStamp + m_dwLifeTime )
		{
			End();
			return;
		}
	}
}

//##ModelId=41DCD60D0119
BOOL CWorldServerSession::Start( BOOL bFlag )
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_dwStartingTimeStamp	= timeGetTime();
	m_bSessionStarted		= TRUE;
	return OnSessionStarted( bFlag );
}

//##ModelId=41DCD61A00DA
BOOL CWorldServerSession::End( BOOL bFlag )
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_dwStartingTimeStamp	= 0;
	m_bSessionEnded			= TRUE;
	m_bRemoveFlag			= TRUE;

	return OnSessionEnded( bFlag );
}

//##ModelId=41DCD62A0186
BOOL CWorldServerSession::Abort( BOOL bFlag )
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_dwStartingTimeStamp	= 0;
	m_bSessionAborted		= TRUE;
	m_bRemoveFlag			= TRUE;

	return OnSessionAborted( bFlag );
}


BOOL CWorldServerSession::OnSessionStarted( BOOL bFlag )
{
	return TRUE;
}


BOOL CWorldServerSession::OnSessionEnded( BOOL bFlag )
{
	return TRUE;
}

BOOL CWorldServerSession::OnSessionAborted( BOOL bFlag )
{
	return TRUE;
}
BOOL CWorldServerSession::IsSessionAvailable()
{
	if(CSessionFactory::QuerySession(GetExID()))
		return TRUE;
	return FALSE;
}
BOOL CWorldServerSession::IsSessionShouldBeenRemoved()
{
	return m_bRemoveFlag;
}


//##ModelId=41DE28EC0242
BOOL CWorldServerSession::IsSessionEnded()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	if( m_bSessionStarted && m_bSessionEnded )
	{
		bResult = TRUE;
	}
	return bResult;
}

//---------------------------------------------------
// �Ự������Ϣ�ӿ�
//---------------------------------------------------
void CWorldServerSession::SendLoadAccountMsg(Account* acc)
{
	if(acc == NULL) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	msg.Add(GetExID());

	msg.Add("[account]");
	msg.Add(acc->GetGUID());
	msg.Add(acc->GetName());
	msg.Add((BYTE)1);

	vector<BYTE> pBA;
	// ������Ҫ��ѯ������
	// �˴�����LoginPlayerGroup����

	if(acc)
	{
		acc->SetCurWsSessionStep(CWorldServerSession::WORLD_SESSION_LOAD_ACCOUNT);

		CEntityGroup* pLoginPlayerGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(acc, string("[loginplayergroup]"));
		if(pLoginPlayerGroup)
		{
			pLoginPlayerGroup->QueryDataRelease();
			CTempEntityProperty* pEP = new CTempEntityProperty( string("Account"), DATA_OBJECT_STRING);
			if(pEP)
			{
				pEP->SetStringAttr(string(acc->GetName()));
				pEP->SetDbRelateType(DB_RELATE_NULL);
				pLoginPlayerGroup->GetQueryDataVec().push_back(pEP);
				pLoginPlayerGroup->SetCurDbOperType(DB_OPER_LOAD);
			}
		}
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(acc, true);
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(acc, DB_OPER_LOAD);
		acc->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendSaveAccountMsg(Account* acc)
{
	if(acc == NULL) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	msg.Add(GetExID());

	msg.Add("[account]");
	msg.Add(acc->GetGUID());
	msg.Add(acc->GetName());
	msg.Add((BYTE)1);
	vector<BYTE> pBA;
	if(acc)
	{
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(acc, true);
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(acc, DB_OPER_SAVE);
		acc->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendLoadFactionGroupMsg()
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pFactionGroup = NULL;
	string strGroup = "[factiongroup]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pFactionGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if(!pFactionGroup) return;

	msg.Add(GetExID());

	msg.Add("[factiongroup]");
	msg.Add(pFactionGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pFactionGroup)
	{
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pFactionGroup, DB_OPER_LOAD);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFactionGroup, true);
		pFactionGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendLoadUnionGroupMsg()
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pUnionGroup = NULL;
	string strGroup = "[union_group]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pUnionGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if(!pUnionGroup) return;

	msg.Add(GetExID());

	msg.Add("[union_group]");
	msg.Add(pUnionGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pUnionGroup)
	{
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pUnionGroup, DB_OPER_LOAD);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pUnionGroup, true);
		pUnionGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendLoadCountryMsg()
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pCountryGroup = NULL;
	string strGroup = "[countrygroup]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pCountryGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if(!pCountryGroup) return;

	msg.Add(GetExID());
	msg.Add("[countrygroup]");
	msg.Add(pCountryGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pCountryGroup)
	{
		pCountryGroup->SetCurDbOperType(DB_OPER_LOAD);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pCountryGroup, true);
		pCountryGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendLoadRegionGroupMsg(void)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pCountryGroup = NULL;
	string strGroup = "[regiongroup]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pCountryGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if(!pCountryGroup) return;

	msg.Add(GetExID());

	msg.Add("[regiongroup]");
	msg.Add(pCountryGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pCountryGroup)
	{
		pCountryGroup->SetCurDbOperType(DB_OPER_LOAD);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pCountryGroup, true);
		pCountryGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendSaveRegionMsg(CEntityGroup* pRegion)
{
	if(pRegion == NULL) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������

	msg.Add(GetExID());

	msg.Add("[region]");
	msg.Add(pRegion->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	// �����ѯ����
	pRegion->QueryDataRelease();
	pRegion->RetQueryDataRelease();

	vector<BYTE> pBA;
	pRegion->SetCurDbOperType(DB_OPER_SAVE);
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pRegion, true);
	pRegion->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendCreateRegionMsg(CEntityGroup* pRegion)
{
	if(pRegion == NULL) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������

	msg.Add(GetExID());

	msg.Add("[region]");
	msg.Add(pRegion->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	pRegion->SetCurDbOperType(DB_OPER_INSERT);
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pRegion, true);
	pRegion->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendSaveFactionMsg(vector<CEntityGroup*> &vCEntityGroup)
{
	if(vCEntityGroup.size() == 0) return;

#ifdef _RUNSTACKINFO_
	char szOut[1024] = {0};
	sprintf(szOut, "SendSaveFactionMsg start vCEntityGroup size %d", vCEntityGroup.size());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), szOut);
#endif

	CEntityGroup* pFacGroup = NULL;
	CEntityManager::DBEntityComponentMapItr facMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[factiongroup]"));
	if(facMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
	{
		if(facMapItr->second.size() == 0)
		{
			AddLogText("Err SendSaveFactionMsg δ�ҵ�FactionGroup!");
			return;
		}
		pFacGroup = (CEntityGroup*)facMapItr->second.begin()->second;
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFacGroup, false);
	}

	for (LONG i = 0; i < vCEntityGroup.size(); ++i)
	{
		CEntityGroup *pFaction = vCEntityGroup[i];
		if(pFaction == NULL) continue;

		pFaction->QueryDataRelease();
		pFaction->RetQueryDataRelease();

		// ���ò�ѯ����
		CTempEntityProperty* pFactionGuid = new CTempEntityProperty(string("FactionGuid"), DATA_OBJECT_GUID);
		pFactionGuid->SetGuidAttr( pFaction->GetGUID());
		pFactionGuid->SetDbRelateType(DB_RELATE_NULL);
		pFaction->GetQueryDataVec().push_back(pFactionGuid);

		// ���ó�Ա��ѯ����
		CEntityGroup* pFactionGroup = (CEntityGroup*)pFaction;
		CEntityGroup* pMemberGroup = NULL;
		map<string, CGUID>::iterator guidItr = pFactionGroup->GetGuidByComFlagMap().find(string("[factionmembergroup]"));
		if(guidItr != pFactionGroup->GetGuidByComFlagMap().end())
		{
			map<CGUID, CBaseEntity*>::iterator enItr = pFactionGroup->GetEntityGroupMap().find(guidItr->second);
			if(enItr != pFactionGroup->GetEntityGroupMap().end())
				pMemberGroup = (CEntityGroup*)enItr->second;
		}
		if(pMemberGroup)
		{
			CTempEntityProperty* pFactionGuid_members = new CTempEntityProperty(string("FactionGuid_members"), DATA_OBJECT_GUID);
			pFactionGuid_members->SetGuidAttr( pFaction->GetGUID());
			pFactionGuid_members->SetDbRelateType(DB_RELATE_NULL);
			pMemberGroup->GetQueryDataVec().push_back(pFactionGuid_members);
			char szFacGuid[128];
			pFaction->GetGUID().tostring(szFacGuid);
			//AddLogText("SendSaveFactionMsg factionID[%s]", szFacGuid);
			map<CGUID, CBaseEntity*>::iterator enItr = pMemberGroup->GetEntityGroupMap().begin();
			for(; enItr != pMemberGroup->GetEntityGroupMap().end(); enItr++)
			{
				CGUID tmpGuid;
				enItr->second->GetGuidAttr(string("FactionGuid_members"),  tmpGuid);
				tmpGuid.tostring(szFacGuid);
				//AddLogText("SendSaveFactionMsg pMemberGroup[%s]", szFacGuid);
			}
		}

		CEntityGroup* pApplyGroup = NULL;
		guidItr = pFactionGroup->GetGuidByComFlagMap().find(string("[factionapplygroup]"));
		if(guidItr != pFactionGroup->GetGuidByComFlagMap().end())
		{
			map<CGUID, CBaseEntity*>::iterator enItr = pFactionGroup->GetEntityGroupMap().find(guidItr->second);
			if(enItr != pFactionGroup->GetEntityGroupMap().end())
				pApplyGroup = (CEntityGroup*)enItr->second;
		}
		if(pApplyGroup)
		{
			CTempEntityProperty* pFactionGuid_apply = new CTempEntityProperty(string("FactionGuid_apply"), DATA_OBJECT_GUID);
			pFactionGuid_apply->SetGuidAttr( pFaction->GetGUID());
			pFactionGuid_apply->SetDbRelateType(DB_RELATE_NULL);
			pApplyGroup->GetQueryDataVec().push_back(pFactionGuid_apply);
		}
		
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pFaction, DB_OPER_DELETE_INSERT);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFaction, true);
	}

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());
	msg.Add("[factiongroup]");
	msg.Add(pFacGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);
	vector<BYTE> pBA;
	pFacGroup->SetCurDbOperType(DB_OPER_DELETE_INSERT);
	pFacGroup->SetCurDbOperFlag(1);
	pFacGroup->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "SendSaveFactionMsg end");
#endif
}

void CWorldServerSession::SendSaveUnionMsg(CEntityGroup* pUnion)
{
	if(pUnion == NULL) return;

	CEntityGroup* pFacGroup = NULL;
	CEntityManager::DBEntityComponentMapItr facMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[uniongroup]"));
	if(facMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
	{
		if(facMapItr->second.size() == 0)
		{
			AddLogText("Err SendSaveUnionMsg δ�ҵ�UnionGroup!");
			return;
		}
		pFacGroup = (CEntityGroup*)facMapItr->second.begin()->second;
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFacGroup, false);
		pFacGroup->SetCurDbOperType(DB_OPER_SAVE);
		pFacGroup->SetCurDbOperFlag(1);
	}

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������

	msg.Add(GetExID());

	msg.Add("[uniongroup]");
	msg.Add(pFacGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	pUnion->QueryDataRelease();
	pUnion->RetQueryDataRelease();

	// ���ò�ѯ����
	CTempEntityProperty* pUnionGuid = new CTempEntityProperty(string("UnionGuid"), DATA_OBJECT_GUID);
	pUnionGuid->SetGuidAttr( pUnion->GetGUID());
	pUnionGuid->SetDbRelateType(DB_RELATE_NULL);
	pUnion->GetQueryDataVec().push_back(pUnionGuid);

	// ���ó�Ա��ѯ����
	CEntityGroup* pUnionGroup = (CEntityGroup*)pUnion;
	CEntityGroup* pMemberGroup = NULL;
	map<string, CGUID>::iterator guidItr = pUnionGroup->GetGuidByComFlagMap().find(string("[union_member_group]"));
	if(guidItr != pUnionGroup->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pUnionGroup->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pUnionGroup->GetEntityGroupMap().end())
			pMemberGroup = (CEntityGroup*)enItr->second;
	}
	if(pMemberGroup)
	{
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pMemberGroup, DB_OPER_DELETE_INSERT);
		pUnionGuid = new CTempEntityProperty(string("UnionGuid"), DATA_OBJECT_GUID);
		pUnionGuid->SetGuidAttr( pUnion->GetGUID());
		pUnionGuid->SetDbRelateType(DB_RELATE_NULL);
		pMemberGroup->GetQueryDataVec().push_back(pUnionGuid);
	}

	pUnion->SetCurDbOperType(DB_OPER_SAVE);
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pUnion, true);
	vector<BYTE> pBA;
	pUnion->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendInsertFactionMsg(CEntityGroup* pFaction)
{
	if(pFaction == NULL) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������

	CEntityGroup* pFacGroup = NULL;
	CEntityManager::DBEntityComponentMapItr facMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[factiongroup]"));
	if(facMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
	{
		if(facMapItr->second.size() == 0)
		{
			AddLogText("Err SendInsertFactionMsg δ�ҵ�FactionGroup!");
			return;
		}
		pFacGroup = (CEntityGroup*)facMapItr->second.begin()->second;
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFacGroup, false);
		pFacGroup->SetCurDbOperType(DB_OPER_SAVE);
		pFacGroup->SetCurDbOperFlag(1);
	}

	msg.Add(GetExID());

	msg.Add("[factiongroup]");
	msg.Add(pFacGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);
	
	CEntityGroup* pMemberGroup = NULL;
	map<string, CGUID>::iterator guidItr = pFaction->GetGuidByComFlagMap().find(string("[factionmembergroup]"));
	if(guidItr != pFaction->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pFaction->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pFaction->GetEntityGroupMap().end())
		{
			pMemberGroup = (CEntityGroup*)enItr->second;
			if(pMemberGroup)
			{
				CTempEntityProperty* pEpGuid = new CTempEntityProperty( string("FactionGuid_Members"), DATA_OBJECT_GUID);
				if(pEpGuid)
				{
					pEpGuid->SetGuidAttr( pFaction->GetGUID());
					pEpGuid->SetDbRelateType(DB_RELATE_NULL);
					pMemberGroup->GetQueryDataVec().push_back(pEpGuid);
				}
			}
		}
	}

	CEntityGroup* pApplyGroup = NULL;
	guidItr = pFaction->GetGuidByComFlagMap().find(string("[factionapplygroup]"));
	if(guidItr != pFaction->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pFaction->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pFaction->GetEntityGroupMap().end())
		{
			pApplyGroup = (CEntityGroup*)enItr->second;
			if(pApplyGroup)
			{
				pApplyGroup->SetCurDbOperType(DB_OPER_INSERT);
				CTempEntityProperty* pEpGuid = new CTempEntityProperty( string("FactionGuid_Apply"), DATA_OBJECT_GUID);
				if(pEpGuid)
				{
					pEpGuid->SetGuidAttr( pFaction->GetGUID());
					pEpGuid->SetDbRelateType(DB_RELATE_NULL);
					pApplyGroup->GetQueryDataVec().push_back(pEpGuid);
				}
			}
		}
	}

	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFaction, true);
	GetGame()->GetEntityManager()->SetEntityAllDbOperType(pFaction, DB_OPER_INSERT);
	
	vector<BYTE> pBA;
	pFaction->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendInsertUnionMsg(CEntityGroup* pUnion)
{
	if(pUnion == NULL) return;

	CEntityGroup* pFacGroup = NULL;
	CEntityManager::DBEntityComponentMapItr facMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[uniongroup]"));
	if(facMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
	{
		if(facMapItr->second.size() == 0)
		{
			AddLogText("Err SendInsertUnionMsg δ�ҵ�uniongroup!");
			return;
		}
		pFacGroup = (CEntityGroup*)facMapItr->second.begin()->second;
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFacGroup, false);
		pFacGroup->SetCurDbOperType(DB_OPER_SAVE);
		pFacGroup->SetCurDbOperFlag(1);
	}

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������

	msg.Add(GetExID());

	msg.Add("[uniongroup]");
	msg.Add(pFacGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	CEntityGroup* pMemberGroup = NULL;
	map<string, CGUID>::iterator guidItr = pUnion->GetGuidByComFlagMap().find(string("[union_member_group]"));
	if(guidItr != pUnion->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pUnion->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pUnion->GetEntityGroupMap().end())
		{
			pMemberGroup = (CEntityGroup*)enItr->second;
			if(pMemberGroup)
			{
				CTempEntityProperty* pEpGuid = new CTempEntityProperty( string("UnionGuid"), DATA_OBJECT_GUID);
				if(pEpGuid)
				{
					pEpGuid->SetGuidAttr( pUnion->GetGUID());
					pEpGuid->SetDbRelateType(DB_RELATE_NULL);
					pMemberGroup->GetQueryDataVec().push_back(pEpGuid);
				}
			}
		}
	}

	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pUnion, true);
	GetGame()->GetEntityManager()->SetEntityAllDbOperType(pUnion, DB_OPER_INSERT);
	
	vector<BYTE> pBA;
	pUnion->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendLoadPhRgnGroupMsg(void)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pCountryGroup = NULL;
	string strGroup = "[phrgngroup]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pCountryGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if(!pCountryGroup) return;

	msg.Add(GetExID());

	msg.Add("[phrgngroup]");
	msg.Add(pCountryGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pCountryGroup)
	{
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pCountryGroup, DB_OPER_LOAD);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pCountryGroup, true);
		pCountryGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendSavePhRgnMsg(CEntityGroup* pPhRgn)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pPhRgnGroup = NULL;
	CEntityManager::DBEntityComponentMapItr rgnMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find("[phrgngroup]");
	if(rgnMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
	{
		pPhRgnGroup = (CEntityGroup*)(rgnMapItr->second.begin()->second);
	}
	if(!pPhRgnGroup) return;

	pPhRgnGroup->QueryDataRelease();
	pPhRgnGroup->RetQueryDataRelease();

	pPhRgnGroup->ReleaseLeaf(pPhRgn->GetGUID());
	pPhRgnGroup->AddChild(pPhRgn);

	msg.Add(GetExID());
	msg.Add("[phrgngroup]");
	msg.Add(pPhRgnGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pPhRgnGroup)
	{
		// ���ò�ѯ����
		CTempEntityProperty* pRgnId = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
		if(pRgnId)
		{
			pRgnId->SetGuidAttr( pPhRgn->GetGUID());
			pRgnId->SetDbRelateType(DB_RELATE_NULL);
			pPhRgn->GetQueryDataVec().push_back(pRgnId);
		}
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pPhRgn, true);
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pPhRgn, DB_OPER_DELETE_INSERT);
		pPhRgnGroup->SetCurDbOperType(DB_OPER_SAVE);
		pPhRgnGroup->SetCurDbOperFlag(1);
		pPhRgnGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendLoadGlobalVarGroupMsg(void)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pVarGroup = NULL;
	string strGroup = "[scriptvargroup]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pVarGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if(!pVarGroup) return;

	msg.Add(GetExID());
	msg.Add("[scriptvargroup]");
	msg.Add(pVarGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pVarGroup)
	{
		pVarGroup->SetCurDbOperType(DB_OPER_LOAD);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pVarGroup, true);
		pVarGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendSaveGlobalVarGroupMsg(void)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pVarGroup = NULL;
	CEntityManager::DBEntityComponentMapItr rgnMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[scriptvargroup]"));
	if(rgnMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
	{
		pVarGroup = (CEntityGroup*)(rgnMapItr->second.begin()->second);
	}
	if(!pVarGroup) return;

	GetGame()->GetEntityManager()->CGlobalVarGroupToCGDBlobalVar(pVarGroup);

	msg.Add(GetExID());

	msg.Add("[scriptvargroup]");
	msg.Add(pVarGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pVarGroup)
	{
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pVarGroup, true);
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pVarGroup, DB_OPER_DELETE_INSERT);
		pVarGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendLoadBusinessMsg()
{
	CMessage msg( MSG_W2DB_OPER_ENTITY );
	CEntityGroup *pVarGroup = NULL;
	string strGroup = "[trade_spot_group]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pVarGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if( pVarGroup == NULL )
	{
		return;
	}

	msg.Add( GetExID() );
	msg.Add( "[trade_spot_group]" );
	msg.Add( pVarGroup->GetGUID() );
	msg.Add( "" );
	msg.Add((BYTE)0);

	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pVarGroup, true);
	GetGame()->GetEntityManager()->SetEntityAllDbOperType(pVarGroup, DB_OPER_LOAD);
	vector<BYTE> pBA;
	pVarGroup->AddToByteArray( pBA );
	msg.Add((long)pBA.size());
	if(pBA.size())
	{
		msg.AddEx(&pBA[0], (long)pBA.size());
	}
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendLoadLimitGoodsRecordUpdateMsg()
{
	CMessage msg( MSG_W2DB_OPER_ENTITY );
	CEntityGroup *pVarGroup = NULL;
	string strGroup = "[limit_goods_record_group]";
	tagEntityBuildInfo* pInfo = GetGame()->GetEntityManager()->GetEntityBuildInfo(strGroup);
	if(pInfo)
		pVarGroup = GetGame()->GetEntityManager()->CreateRootEntity(strGroup, pInfo->guid);
	if( pVarGroup == NULL )
	{
		return;
	}

	msg.Add( GetExID() );
	msg.Add( strGroup.c_str() );
	msg.Add( pVarGroup->GetGUID() );
	msg.Add( "" );
	msg.Add((BYTE)0);

	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pVarGroup, true);
	GetGame()->GetEntityManager()->SetEntityAllDbOperType(pVarGroup, DB_OPER_LOAD);
	vector<BYTE> pBA;
	pVarGroup->AddToByteArray( pBA );
	msg.Add((long)pBA.size());
	if(pBA.size())
	{
		msg.AddEx(&pBA[0], (long)pBA.size());
	}
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendLimitGoodsRecordUpdateMsg()
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);

	CEntityGroup* pVarGroup = NULL;
	CEntityManager::DBEntityComponentMapItr rgnMapItr = 
		GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[limit_goods_record_group]"));
	if(rgnMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
	{
		pVarGroup = (CEntityGroup*)(rgnMapItr->second.begin()->second);
	}
	if(!pVarGroup) 
		return;	

	GetGame()->GetEntityManager()->CLimitGoodsRecordToCDBRecord(pVarGroup);

	msg.Add(GetExID());

	msg.Add("[limit_goods_record_group]");
	msg.Add(pVarGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pVarGroup)
	{
		map<CGUID, CBaseEntity*>::iterator childItr = pVarGroup->GetEntityGroupMap().begin();
		map<CGUID, CBaseEntity*>::iterator childEndItr = pVarGroup->GetEntityGroupMap().end();
		childEndItr--;
		for(; childItr != pVarGroup->GetEntityGroupMap().end(); childItr++)
		{
			// ��Ӳ�ѯ����
			CTempEntityProperty* pGoodsId = new CTempEntityProperty( string("goods_id"), DATA_OBJECT_LONG);
			if(pGoodsId)
			{
				pGoodsId->SetLongAttr(childItr->second->GetLongAttr(string("goods_id")));
				if(childItr != childEndItr)
					pGoodsId->SetDbRelateType(DB_RELATE_OR);
				else
					pGoodsId->SetDbRelateType(DB_RELATE_NULL);
				pVarGroup->GetQueryDataVec().push_back(pGoodsId);
			}
		}

		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pVarGroup, true);
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pVarGroup, DB_OPER_DELETE_INSERT);
		pVarGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendSaveBusinessMsg()
{
	CMessage msg( MSG_W2DB_OPER_ENTITY );
	
	CEntityGroup* pVarGroup = NULL;
	CEntityManager::DBEntityComponentMapItr rgnMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find( string( "[trade_spot_group]" ) );
	if( rgnMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end() )
	{
		pVarGroup = (CEntityGroup*)(rgnMapItr->second.begin()->second);
	}
	if( pVarGroup == NULL )
	{
		return;
	}

	// ������ݵ�entity group
	GetInst( Business::CBusinessManager ).EncodeToEntityGroup( pVarGroup );

	msg.Add( GetExID() );
	msg.Add( "[trade_spot_group]" );
	msg.Add( pVarGroup->GetGUID() );
	msg.Add( "" );
	msg.Add( (BYTE)0 );

	vector<BYTE> pBA;
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag( pVarGroup, true );
	GetGame()->GetEntityManager()->SetEntityAllDbOperType( pVarGroup, DB_OPER_DELETE_INSERT );
	pVarGroup->AddToByteArray( pBA );
	msg.Add((long)pBA.size());
	if(pBA.size())
	{
		msg.AddEx(&pBA[0], (long)pBA.size());
	}
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

void CWorldServerSession::SendCreatePlayerCheckProcMsg(const char* szCdkey, const char* szName, BYTE nOccu, 
													   BYTE sex,BYTE nHead, BYTE nFace, BYTE btCountry, BYTE btMaxCharactersNum,
													   BYTE byConstellation,bool bSelectRecommCountry)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pProcedure = NULL;
	CGUID ProcGuid;
	CGUID::CreateGUID(ProcGuid);
	pProcedure = (CEntityGroup*)(GetGame()->GetEntityManager()->CreateRootEntity(string("[procedure]"), ProcGuid));
	if(!pProcedure) return;

	msg.Add(GetExID());
	msg.Add("[procedure]");
	msg.Add(ProcGuid);
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pProcedure)
	{
		pProcedure->QueryDataRelease();
		pProcedure->RetQueryDataRelease();
		pProcedure->ReleaseChilds();

		// ��Ӳ�ѯ����
		CTempEntityProperty* pCdkey = new CTempEntityProperty( string("Account"), DATA_OBJECT_STRING);
		if(pCdkey)
		{
			pCdkey->SetStringAttr(string(szCdkey));
			pCdkey->SetDbRelateType(DB_RELATE_NULL);
			pProcedure->GetQueryDataVec().push_back(pCdkey);
		}

		CTempEntityProperty* pTmpCdkey = new CTempEntityProperty( string("Account"), DATA_OBJECT_STRING);
		if(pTmpCdkey)
		{
			pTmpCdkey->SetStringAttr(string(szCdkey));
			GetTempDataVec().push_back(pTmpCdkey);
		}
		CTempEntityProperty* pName = new CTempEntityProperty( string("name"), DATA_OBJECT_STRING);
		if(pName)
		{
			pName->SetStringAttr(string(szName));
			pName->SetDbRelateType(DB_RELATE_NULL);
			pProcedure->GetQueryDataVec().push_back(pName);
		}

		CTempEntityProperty* pMaxNum = new CTempEntityProperty( string("MaxNum"), DATA_OBJECT_LONG);
		if(pMaxNum)
		{
			pMaxNum->SetLongAttr(btMaxCharactersNum);
			pMaxNum->SetDbRelateType(DB_RELATE_NULL);
			pProcedure->GetQueryDataVec().push_back(pMaxNum);
		}
		// ��ӷ���ֵ
		CTempEntityProperty* pRetName = new CTempEntityProperty( string("NameIsExist"), DATA_OBJECT_LONG);
		if(pRetName)
		{
			pProcedure->GetRetQueryDataVec().push_back(pRetName);
		}
		CTempEntityProperty* pRetCdkey = new CTempEntityProperty( string("PlayerNum"), DATA_OBJECT_LONG);
		if(pRetCdkey)
		{
			pProcedure->GetRetQueryDataVec().push_back(pRetCdkey);
		}
		// ���ִ�д洢������
		pProcedure->SetStringAttr(string("ProcName"),  "CreatePlayerCheckProc");

		// ��ʼ��LoginPlayer����
		Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(szCdkey);
		if(!tLoginInfo)
		{
			char outStr[1024];
			sprintf(outStr, "MSG_DB2W_PASS_LOG_CREATEROLE! LoginInfo[%s] Is Null!", szCdkey);
			AddLogText(outStr);
			CMessage msgx(MSG_W2L_LOG_CREATEROLE);
			msgx.Add((char)LOGIN_CREATEROLE_FAILED);
			msgx.Add(szCdkey);
			msgx.SendToSocket(GetGame()->GetLSSocketID());
			tLoginInfo->SetPlayerState(PLAYER_STATE_VOID);
			return;
		}

		tLoginInfo->SetCurWsSessionStep(CWorldServerSession::WORLD_SESSION_CREATE_ACCOUNT);
		tLoginInfo->SetPlayerState(PLAYER_STATE_CREATING);
		tLoginInfo->SetLoginTimeoutValue(timeGetTime());

		////CEntityGroup* pLoginPlayerGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(tLoginInfo, string("[loginplayergroup]"));
		////EntityGroupMap EntityGroup = pLoginPlayerGroup->GetEntityGroupMap();
		////EntityGroupMapItr EntityGroupIt_begin = EntityGroup.begin();
		////EntityGroupMapItr EntityGroupIt_end = EntityGroup.end();
		////DWORD dwCreateNo = 0;
		////for (;EntityGroupIt_begin!=EntityGroupIt_end;EntityGroupIt_begin++)
		////{
		////	// find guid
		////	CEntityGroup* pFindPlayer = (CEntityGroup*)(EntityGroupIt_begin->second);//GetGame()->GetMapPlayer(EntityGroupIt_begin->first);
		////	if (!pFindPlayer) continue;
		////	//dwCreateNo = pFindPlayer->GetCreateNo()>dwCreateNo?pFindPlayer->GetCreateNo():dwCreateNo;
		////	dwCreateNo = pFindPlayer->GetLongAttr(string("CreateNo"))>dwCreateNo?pFindPlayer->GetLongAttr(string("CreateNo")):dwCreateNo;
		////}

		// ����LoginPlayer
		CGUID tGuid;
		CGUID::CreateGUID(tGuid);
		// �����ɹ�
		char szGUID[128];
		tGuid.tostring(szGUID);
		//  �ӳ���ɫ
		CPlayer *pPlayer = GetGame()->NewRawPlayer(ePlayerCreateProc);	
		if(!pPlayer) return;

		//pPlayer->SetCreateNo(dwCreateNo+1);
		time_t curTime;
		time(&curTime);
		pPlayer->SetRoleCreateTime((DWORD)curTime);
		pPlayer->SetExID(tGuid);

		pPlayer->SetName(szName);
		pPlayer->LoadDefaultProperty(nOccu, sex, btCountry,byConstellation);

		pPlayer->SetHeadPic (nHead);
		pPlayer->SetFacePic (nFace);
		pPlayer->SetAccount(szCdkey);
		pPlayer->SetName(szName);
		pPlayer->SetOccupation(nOccu);
		pPlayer->SetGraphicsID(sex+1);
		pPlayer->SetPKOnOff(65535);

		//������żID����ʾ����Ƿ���ѡ���Ƽ��ķ�����
		if(bSelectRecommCountry)
			pPlayer->SetSpouseId(1);

		// �ӳ�ʼװ��������
		GetGame()->AddOrginGoodsToPlayer(pPlayer);
		GetGame()->InitSkillToPlayer(pPlayer);

		// ��ʼ����ʾ����
		pPlayer->SetShowFashion((BYTE)3);
		CEntityGroup* dblPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[loginplayer]"), tGuid);
		dblPlayer->SetStringAttr(string("Account"),  szCdkey);
		// ��ɫ����ʱ��
		long delTime[6] = {0};
		DWORD dwCreateTime = pPlayer->GetRoleCreateTime();
		tm *pCurTime = localtime((time_t*)&dwCreateTime);

		delTime[0] = pCurTime->tm_year + 1900; 
		delTime[1] = pCurTime->tm_mon + 1; 
		delTime[2] = pCurTime->tm_mday;
		delTime[3] = pCurTime->tm_hour;
		delTime[4] = pCurTime->tm_min;
		delTime[5] = pCurTime->tm_sec;

		dblPlayer->SetTimeAttr(string("CreateTime"),  delTime, sizeof(long)*6);

		GetGame()->GetEntityManager()->CPlayerToCDBLoginPlayer(pPlayer, dblPlayer);
		dblPlayer->SetGUID(NULL_GUID);
		pProcedure->AddChild(dblPlayer);
		dblPlayer->SetCurDbOperType(DB_OPER_INSERT);
		dblPlayer->SetCurDbOperFlag(1);

		CEntityGroup* dbPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[player]"), tGuid);
		if(dbPlayer)
		{
			GetGame()->GetEntityManager()->CPlayerToCDBPlayer(pPlayer, dbPlayer);
			// ����AutoSupplyData����
			AutoSupplyData tsupplyData;
			vector<BYTE> psDataBA;
			tsupplyData.AddToByteArray(psDataBA);
			dbPlayer->SetBufAttr(string("AutoSupplyData"),  &psDataBA[0], psDataBA.size());

			// ����PlayerSetupOnServer����
			PlayerSetupOnServer tSetupOnServer;
			vector<BYTE> psetupServerBA;
			tSetupOnServer.AddToByteArray(psetupServerBA);
			dbPlayer->SetBufAttr(string("PlayerSetup"),  &psetupServerBA[0], psetupServerBA.size());
			pProcedure->AddChild(dbPlayer);


			// С��������
			CEntityGroup* pSpriteGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dbPlayer, string("[playerspritegroup]"));
			if(!pSpriteGroup)
			{
				AddLogText("SendCreatePlayerCheckProcMsg() δ�ҵ����[%s]��playerspritegroup����", szCdkey);
				GetGame()->DelRawPlayer(pPlayer);
				GetGame()->GetEntityManager()->DelOneRootEntity(string("[procedure]"), ProcGuid);
				return;
			}

			CTempEntityProperty *pEP = new CTempEntityProperty( string("PlayerGuid"), DATA_OBJECT_GUID);
			if(pEP)
			{
				pEP->SetGuidAttr( tGuid);
				pEP->SetDbRelateType(DB_RELATE_NULL);
				pSpriteGroup->GetQueryDataVec().push_back(pEP);
			}

			// ����
			CEntityGroup* pPetList = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dbPlayer, string("[playerpetlist]"));
			if( NULL == pPetList )
			{
				AddLogText("SendCreatePlayerCheckProcMsg() δ�ҵ����[%s]��playerspritegroup����", szCdkey);
				GetGame()->DelRawPlayer(pPlayer);
				GetGame()->GetEntityManager()->DelOneRootEntity(string("[procedure]"), ProcGuid);
				return;
			}

			CTempEntityProperty *pEPPet = new CTempEntityProperty( string("HostID"), DATA_OBJECT_GUID);
			if(pEPPet)
			{
				pEPPet->SetGuidAttr( tGuid);
				pEPPet->SetDbRelateType(DB_RELATE_NULL);
				pPetList->GetQueryDataVec().push_back(pEPPet);
			}

			// ΪDBPlayer��GoodsGroup�������ò�ѯ����
			CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(dbPlayer, string("[goodsgroup]"));
			if(pGoodsGroup)
			{
				CTempEntityProperty* pOwnerID = new CTempEntityProperty(string("PlayerID"), DATA_OBJECT_GUID);
				if(pOwnerID)
				{
					pOwnerID->SetGuidAttr( dbPlayer->GetGUID());
					pOwnerID->SetDbRelateType(DB_RELATE_NULL);
					pGoodsGroup->GetQueryDataVec().push_back(pOwnerID);
				}
			}

			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(dbPlayer, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(dbPlayer, DB_OPER_INSERT);
		}

		pProcedure->SetCurDbOperType(DB_OPER_CREATEROLE_PROCEDURE);
		pProcedure->SetCurDbOperFlag(1);
		pProcedure->AddToByteArray(pBA);
		GetGame()->DelRawPlayer(pPlayer);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendUpdateLoginPlayerTimeMsg(const char* szCdkey, const CGUID& guid)
{
	Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
	if(pAcc == NULL) return;

	CEntityGroup* dbLPlayer = (CEntityGroup*)pAcc->FindLoginPlayer(guid);
	if(dbLPlayer == NULL) return;

	// �����ѯ����
	dbLPlayer->QueryDataRelease();
	dbLPlayer->RetQueryDataRelease();

	// �����ʱ����
	CTempEntityProperty* pCdkey = new CTempEntityProperty( string("Account"), DATA_OBJECT_STRING);
	if(pCdkey)
	{
		pCdkey->SetStringAttr(string(szCdkey));
		GetTempDataVec().push_back(pCdkey);
	}
	CTempEntityProperty* pGuid = new CTempEntityProperty( string("Guid"), DATA_OBJECT_GUID);
	if(pGuid)
	{
		pGuid->SetGuidAttr( guid);
		GetTempDataVec().push_back(pGuid);
	}

	// ��Ӳ�ѯ����
	CTempEntityProperty* pQGuid = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
	if(pQGuid)
	{
		pQGuid->SetGuidAttr( guid);
		pQGuid->SetDbRelateType(DB_RELATE_NULL);
		dbLPlayer->GetQueryDataVec().push_back(pQGuid);
	}

	// ���ɾ��ʱ��
	long delTime[6] = {0};
	time_t tCurTime = time(0);


	tCurTime += 3600 * 24 * 7;
	tm *pCurTime = localtime(&tCurTime);

	delTime[0] = pCurTime->tm_year + 1900; 
	delTime[1] = pCurTime->tm_mon + 1; 
	delTime[2] = pCurTime->tm_mday;

	dbLPlayer->SetTimeAttr(string("DelDate"),  delTime, sizeof(long)*6);

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());

	msg.Add("[account]");
	msg.Add(pAcc->GetGUID());
	msg.Add(pAcc->GetName());
	msg.Add((BYTE)1);

	vector<BYTE> pBA;
	dbLPlayer->SetCurDbOperType(DB_OPER_SAVE);
	dbLPlayer->SetCurDbOperFlag(1);
	dbLPlayer->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

	// ����ɾ������ʼ��Ĵ洢����
	SendDelRoleMailProcMsg(dbLPlayer->GetStringAttr(string("Name")));

	// ��Ӧ�ͻ�����Ϣ	
	// ��Ϊ�Ǹ�ɾ���ģ�ֱ�ӷ����������ɾ���������ɡ�
	char nRemainDays = char(CGlobeSetup::GetSetup()->dwDelDays);
	CMessage retmsg(MSG_W2L_LOG_DELETEROLE);
	retmsg.Add((char)LOGIN_DELETEROLE_SUCCEED);
	retmsg.Add(guid);
	retmsg.Add(szCdkey);
	retmsg.Add(nRemainDays);
	retmsg.SendToSocket(GetGame()->GetLSSocketID());//send to loginserver
}
void CWorldServerSession::SendRestoreLoginPlayerTimeMsg(const char* szCdkey, const CGUID& guid)
{
	Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
	if(pAcc == NULL) return;

	CEntityGroup* dbLPlayer = (CEntityGroup*)pAcc->FindLoginPlayer(guid);
	if(dbLPlayer == NULL) return;

	// �����ѯ����
	dbLPlayer->QueryDataRelease();
	dbLPlayer->RetQueryDataRelease();

	// �����ʱ����
	CTempEntityProperty* pCdkey = new CTempEntityProperty( string("Account"), DATA_OBJECT_STRING);
	if(pCdkey)
	{
		pCdkey->SetStringAttr(string(szCdkey));
		GetTempDataVec().push_back(pCdkey);
	}
	CTempEntityProperty* pGuid = new CTempEntityProperty( string("Guid"), DATA_OBJECT_GUID);
	if(pGuid)
	{
		pGuid->SetGuidAttr( guid);
		GetTempDataVec().push_back(pGuid);
	}

	// ��Ӳ�ѯ����
	CTempEntityProperty* pQGuid = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
	if(pQGuid)
	{
		pQGuid->SetGuidAttr( guid);
		pQGuid->SetDbRelateType(DB_RELATE_NULL);
		dbLPlayer->GetQueryDataVec().push_back(pQGuid);
	}

	// ���ɾ��ʱ��
	long delTime[6] = {0};

	dbLPlayer->SetTimeAttr(string("DelDate"),  delTime, sizeof(long)*6);

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());

	msg.Add("[account]");
	msg.Add(pAcc->GetGUID());
	msg.Add(pAcc->GetName());
	msg.Add((BYTE)1);

	vector<BYTE> pBA;
	dbLPlayer->SetCurDbOperType(DB_OPER_SAVE);
	dbLPlayer->SetCurDbOperFlag(1);
	dbLPlayer->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

	// ��Ӧ�ͻ�����Ϣ	
	CMessage retmsg(MSG_W2L_LOG_RESTOREROLE);
	retmsg.Add((char)LOGIN_RESTOREROLE_SUCCEED);
	retmsg.Add(guid);
	retmsg.Add(szCdkey);
	retmsg.SendToSocket(GetGame()->GetLSSocketID());//send to loginserver
}
BOOL CWorldServerSession::SendDelFactionProcMsg(const CGUID &OrganGuid)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityManager::DBEntityComponentMapItr mapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[factiongroup]"));
	if(mapItr == GetGame()->GetEntityManager()->GetBaseEntityMap().end())
		return FALSE;
	if(mapItr->second.size() == 0) return FALSE;
	CEntityGroup* pGroup = mapItr->second.begin()->second;
	if(pGroup)
	{
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, false);
		pGroup->SetCurDbOperType(DB_OPER_DELETE);
		pGroup->SetCurDbOperFlag(1);
	}
	map<CGUID, CBaseEntity*>::iterator groupItr = pGroup->GetEntityGroupMap().find(OrganGuid);

	CEntityGroup* pFac = NULL;
	if(groupItr == pGroup->GetEntityGroupMap().end())
	{
		pFac = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[faction]"), OrganGuid);
		pGroup->AddChild(pFac);
	}
	else
		pFac = (CEntityGroup*)groupItr->second;

	msg.Add(GetExID());
	msg.Add("[factiongroup]");
	msg.Add(pGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pFac)
	{
		pFac->QueryDataRelease();
		pFac->RetQueryDataRelease();

		// ��Ӳ�ѯ����
		CTempEntityProperty* pEpGuid = new CTempEntityProperty( string("FactionGuid"), DATA_OBJECT_GUID);
		if(pEpGuid)
		{
			pEpGuid->SetGuidAttr( OrganGuid);
			pEpGuid->SetDbRelateType(DB_RELATE_NULL);
			pFac->GetQueryDataVec().push_back(pEpGuid);
		}
		pEpGuid = new CTempEntityProperty( string("FactionGuid"), DATA_OBJECT_GUID);
		if(pEpGuid)
		{
			pEpGuid->SetGuidAttr( OrganGuid);
			GetTempDataVec().push_back(pEpGuid);
		}

		CEntityGroup* pMemberGroup = NULL;
		map<string, CGUID>::iterator guidItr = pFac->GetGuidByComFlagMap().find(string("[factionmembergroup]"));
		if(guidItr != pFac->GetGuidByComFlagMap().end())
		{
			map<CGUID, CBaseEntity*>::iterator enItr = pFac->GetEntityGroupMap().find(guidItr->second);
			if(enItr != pFac->GetEntityGroupMap().end())
			{
				pMemberGroup = (CEntityGroup*)enItr->second;
				if(pMemberGroup)
				{
					pMemberGroup->SetCurDbOperType(DB_OPER_DELETE);
					pEpGuid = new CTempEntityProperty( string("FactionGuid_Members"), DATA_OBJECT_GUID);
					if(pEpGuid)
					{
						pEpGuid->SetGuidAttr( OrganGuid);
						pEpGuid->SetDbRelateType(DB_RELATE_NULL);
						pMemberGroup->GetQueryDataVec().push_back(pEpGuid);
					}
				}
			}
		}

		CEntityGroup* pApplyGroup = NULL;
		guidItr = pFac->GetGuidByComFlagMap().find(string("[factionapplygroup]"));
		if(guidItr != pFac->GetGuidByComFlagMap().end())
		{
			map<CGUID, CBaseEntity*>::iterator enItr = pFac->GetEntityGroupMap().find(guidItr->second);
			if(enItr != pFac->GetEntityGroupMap().end())
			{
				pApplyGroup = (CEntityGroup*)enItr->second;
				if(pApplyGroup)
				{
					pApplyGroup->SetCurDbOperType(DB_OPER_DELETE);
					pEpGuid = new CTempEntityProperty( string("FactionGuid_Apply"), DATA_OBJECT_GUID);
					if(pEpGuid)
					{
						pEpGuid->SetGuidAttr( OrganGuid);
						pEpGuid->SetDbRelateType(DB_RELATE_NULL);
						pApplyGroup->GetQueryDataVec().push_back(pEpGuid);
					}
				}
			}
		}
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFac, true);
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pFac, DB_OPER_DELETE);
		pFac->AddToByteArray(pBA);

		// ���������ݶ���
		pGroup->ReleaseLeaf(OrganGuid);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

	return TRUE;
}
void CWorldServerSession::SendDelUnionProcMsg(const CGUID &OrganGuid)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityManager::DBEntityComponentMapItr mapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[union_group]"));
	if(mapItr == GetGame()->GetEntityManager()->GetBaseEntityMap().end())
		return;
	if(mapItr->second.size() == 0) return;
	CEntityGroup* pGroup = mapItr->second.begin()->second;
	if(pGroup)
	{
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, false);
		pGroup->SetCurDbOperType(DB_OPER_DELETE);
		pGroup->SetCurDbOperFlag(1);
	}
	map<CGUID, CBaseEntity*>::iterator groupItr = pGroup->GetEntityGroupMap().find(OrganGuid);
	if(groupItr == pGroup->GetEntityGroupMap().end()) return;

	CEntityGroup* pFac = (CEntityGroup*)groupItr->second;

	msg.Add(GetExID());
	msg.Add("[union]");
	msg.Add(OrganGuid);
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pFac)
	{
		pFac->QueryDataRelease();
		pFac->RetQueryDataRelease();

		// ��Ӳ�ѯ����
		CTempEntityProperty* pEpGuid = new CTempEntityProperty( string("UnionGuid"), DATA_OBJECT_GUID);
		if(pEpGuid)
		{
			pEpGuid->SetGuidAttr( OrganGuid);
			pEpGuid->SetDbRelateType(DB_RELATE_NULL);
			pFac->GetQueryDataVec().push_back(pEpGuid);
		}
		pEpGuid = new CTempEntityProperty( string("UnionGuid"), DATA_OBJECT_GUID);
		if(pEpGuid)
		{
			pEpGuid->SetGuidAttr( OrganGuid);
			GetTempDataVec().push_back(pEpGuid);
		}

		CEntityGroup* pMemberGroup = NULL;
		map<string, CGUID>::iterator guidItr = pFac->GetGuidByComFlagMap().find(string("[union_member_group]"));
		if(guidItr != pFac->GetGuidByComFlagMap().end())
		{
			map<CGUID, CBaseEntity*>::iterator enItr = pFac->GetEntityGroupMap().find(guidItr->second);
			if(enItr != pFac->GetEntityGroupMap().end())
			{
				pMemberGroup = (CEntityGroup*)enItr->second;
				if(pMemberGroup)
				{
					pMemberGroup->SetCurDbOperType(DB_OPER_DELETE);
					pEpGuid = new CTempEntityProperty( string("UnionGuid"), DATA_OBJECT_GUID);
					if(pEpGuid)
					{
						pEpGuid->SetGuidAttr( OrganGuid);
						pEpGuid->SetDbRelateType(DB_RELATE_NULL);
						pMemberGroup->GetQueryDataVec().push_back(pEpGuid);
					}
				}
			}
		}

		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pFac, true);
		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pFac, DB_OPER_DELETE);
		pFac->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::SendCheckOfflinePlayerNameProcMsg(const char* szOwnerName, const CGUID& WriterGuid)
{
	if(szOwnerName == NULL) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);

	CGUID ProcGuid;
	CGUID::CreateGUID(ProcGuid);
	CEntityGroup* pProcedure = NULL;
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	pProcedure = (CEntityGroup*)(GetGame()->GetEntityManager()->CreateRootEntity(string("[procedure]"), ProcGuid));
	if(!pProcedure) return;

	msg.Add(GetExID());
	msg.Add("[procedure]");
	msg.Add(ProcGuid);
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pProcedure)
	{
		pProcedure->QueryDataRelease();
		pProcedure->RetQueryDataRelease();

		// ��Ӳ�ѯ����
		CTempEntityProperty* pName = new CTempEntityProperty( string("PlayerName"), DATA_OBJECT_STRING);
		if(pName)
		{
			pName->SetStringAttr(string(szOwnerName));
			pName->SetDbRelateType(DB_RELATE_NULL);
			pProcedure->GetQueryDataVec().push_back(pName);
		}
		CTempEntityProperty* pSenderGuid = new CTempEntityProperty( string("SenderGuid"), DATA_OBJECT_GUID);
		if(pSenderGuid)
		{
			pSenderGuid->SetGuidAttr( WriterGuid);
			pSenderGuid->SetDbRelateType(DB_RELATE_NULL);
			pProcedure->GetQueryDataVec().push_back(pSenderGuid);
		}

		// ��ӷ���ֵ
		CTempEntityProperty* pRetGuid = new CTempEntityProperty( string("PlayerGuid"), DATA_OBJECT_GUID);
		if(pRetGuid)
		{
			pProcedure->GetRetQueryDataVec().push_back(pRetGuid);
		}

		// ���ִ�д洢������
		pProcedure->SetStringAttr(string("ProcName"),  "PlayerNameCheckProc");
		pProcedure->SetCurDbOperType(DB_OPER_PROCEDURE);
		pProcedure->SetCurDbOperFlag(1);
		pProcedure->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::NakeSendSaveLoginPlayerDataMsg(CPlayer* pPlayer, CEntityGroup* dbLPlayer)
{
	if(!pPlayer || !dbLPlayer) return;

	// ת������
	GetGame()->GetEntityManager()->CPlayerToCDBLoginPlayer(pPlayer, dbLPlayer);

	// �����ѯ����
	dbLPlayer->QueryDataRelease();
	dbLPlayer->RetQueryDataRelease();

	// ��Ӳ�ѯ����
	CTempEntityProperty* pGuid = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
	if(pGuid)
	{
		pGuid->SetGuidAttr( pPlayer->GetExID());
		pGuid->SetDbRelateType(DB_RELATE_NULL);
		dbLPlayer->GetQueryDataVec().push_back(pGuid);
	}

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());

	msg.Add("[account]");
	msg.Add(pPlayer->GetExID());
	msg.Add(pPlayer->GetAccount());
	msg.Add((BYTE)1);

	vector<BYTE> pBA;
	dbLPlayer->SetCurDbOperType(DB_OPER_SAVE);
	dbLPlayer->SetCurDbOperFlag(1);
	dbLPlayer->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::NakeSendSavePlayerDataMsg(long bitValue, CPlayer* player, CEntityGroup* pPlayer, bool isSaveDbPlayerFlag)
{
	if(!player || !pPlayer) return;
	if(isSaveDbPlayerFlag == true) bitValue = SAVE_DETAIL_ALL;

	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pPlayer, false);
	{
		GetGame()->GetEntityManager()->DetailAreaCreditCPlayerToCDBPlayer(player, pPlayer);
		GetGame()->GetEntityManager()->DetailLimitGoodsRecordCPlayerToCDBPlayer( player, pPlayer );
		GetGame()->GetEntityManager()->DetailIncTradeRecordCPlayerToCDBPlayer( player, pPlayer );
		GetGame()->GetEntityManager()->DetailPropertyCPlayerToCDBPlayer(player, pPlayer);
		GetGame()->GetEntityManager()->DetailQuestCPlayerToCDBPlayer(player, pPlayer);
		GetGame()->GetEntityManager()->DetailMerQuestCPlayerToCDBPlayer(player, pPlayer);//
		GetGame()->GetEntityManager()->DetailSkillCPlayerToCDBPlayer(player, pPlayer);
		GetGame()->GetEntityManager()->DetailStateCPlayerToCDBPlayer(player, pPlayer);
		GetGame()->GetEntityManager()->DetailVarDataCPlayerToCDBPlayer(player, pPlayer);
		GetGame()->GetEntityManager()->DetailMedalCPlayerToCDBPlayer(player, pPlayer);
		//ϵͳ�ʼ��б�
		GetGame()->GetEntityManager()->DetailSysMailCplayerToCDBPlayer(player, pPlayer);
		// ��������10����Ʒ�б�
		GetGame()->GetEntityManager()->DetailIncGoodsCPlayerToCDBPlayer(player, pPlayer);
		// new Occupation
		GetGame()->GetEntityManager()->DetailDeOccuCPlayerToCDBPlayer(player, pPlayer);

		GetGame()->GetEntityManager()->DetailGoodsCooldownCPlayerToCDBPlayer(player,pPlayer);

		// ��ݼ�����
		GetGame()->GetEntityManager()->DetailFuncHotKeyCPlayerToCDBPlayer(player,pPlayer);

		// ������������
		GetGame()->GetEntityManager()->DetailQuestIndexCPlayerToCDBPlayer(player, pPlayer);
		// ��Ʊϵͳ
		GetGame()->GetEntityManager()->DetailLotteryCPlayerToCDBPlayer(player, pPlayer);
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "NakeSendSavePlayerDataMsg() translate GoodsGroup Start.");
#endif

	CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pPlayer, string("[goodsgroup]"));
	if(!pGoodsGroup)
	{
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),"goodsgroup is null.");
#endif
		return;
	}


	// ����ոö�����ӽڵ����ݿ������־
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGoodsGroup, false);
	
	bool isGoodsChanged = false;
	bool isSubGoodsChanged = false;
	bool isSubGoodsDepotChanged = false;
	// ��Ӳ�ѯ����
	if( bitValue & SAVE_DETAIL_ATTREQUIPMENTGOODS ) // װ��
	{
		GetGame()->GetEntityManager()->DetailEquipmentCPlayerToCDBPlayer(player, pPlayer);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_equip]"));

		if(pGroup)
		{
#ifdef _RUNSTACKINFO1_
			char szOutPutStr[256] = {0};
			sprintf(szOutPutStr, " %s size=%d.",pGroup->GetCompositeFlag().c_str(), pGroup->GetEntityGroupMap().size());
			CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),szOutPutStr);
#endif
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;

	}
	if( bitValue & SAVE_DETAIL_ATTRPACKGOODS) // ס����
	{
		GetGame()->GetEntityManager()->DetailOrignalPackCPlayerToCDBPlayer(player, pPlayer);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_packet]"));

		if(pGroup)
		{
#ifdef _RUNSTACKINFO1_
			char szOutPutStr[256] = {0};
			sprintf(szOutPutStr, " %s size=%d.",pGroup->GetCompositeFlag().c_str(), pGroup->GetEntityGroupMap().size());
			CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),szOutPutStr);
#endif
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
	}
	if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS1) // �ӱ���
	{
		GetGame()->GetEntityManager()->DetailSubpackCPlayerToCDBPlayer(player, pPlayer, 0);
		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subpackone]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
		isSubGoodsChanged = true;
	}
	if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS2)
	{
		GetGame()->GetEntityManager()->DetailSubpackCPlayerToCDBPlayer(player, pPlayer, 1);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subpacktwo]"));
		if(pGroup)
		{
#ifdef _RUNSTACKINFO1_
			char szOutPutStr[256] = {0};
			sprintf(szOutPutStr, " %s size=%d.",pGroup->GetCompositeFlag().c_str(), pGroup->GetEntityGroupMap().size());
			CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),szOutPutStr);
#endif
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
		isSubGoodsChanged = true;
	}
	if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS3)
	{
		GetGame()->GetEntityManager()->DetailSubpackCPlayerToCDBPlayer(player, pPlayer, 2);

		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subpackthree]"));
		if(pGroup)
		{
#ifdef _RUNSTACKINFO1_
			char szOutPutStr[256] = {0};
			sprintf(szOutPutStr, " %s size=%d.",pGroup->GetCompositeFlag().c_str(), pGroup->GetEntityGroupMap().size());
			CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),szOutPutStr);
#endif
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
		isSubGoodsChanged = true;
	}
	if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS4)
	{
		GetGame()->GetEntityManager()->DetailSubpackCPlayerToCDBPlayer(player, pPlayer, 3);

		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subpackfour]"));
		if(pGroup)
		{
#ifdef _RUNSTACKINFO1_
			char szOutPutStr[256] = {0};
			sprintf(szOutPutStr, " %s size=%d.",pGroup->GetCompositeFlag().c_str(), pGroup->GetEntityGroupMap().size());
			CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),szOutPutStr);
#endif
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}

		isGoodsChanged = true;
		isSubGoodsChanged = true;
	}
	if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS5)
	{
		GetGame()->GetEntityManager()->DetailSubpackCPlayerToCDBPlayer(player, pPlayer, 4);

		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subpackfive]"));
		if(pGroup)
		{
#ifdef _RUNSTACKINFO1_
			char szOutPutStr[256] = {0};
			sprintf(szOutPutStr, " %s size=%d.",pGroup->GetCompositeFlag().c_str(), pGroup->GetEntityGroupMap().size());
			CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),szOutPutStr);
#endif
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}

		isGoodsChanged = true;
		isSubGoodsChanged = true;
	}

	if(isSubGoodsChanged) // �ӱ����仯����ͳ��
	{
		// �ӱ�����
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_pack]"));
		if(pGroup)
		{
#ifdef _RUNSTACKINFO1_
			char szOutPutStr[256] = {0};
			sprintf(szOutPutStr, " %s size=%d.",pGroup->GetCompositeFlag().c_str(), pGroup->GetEntityGroupMap().size());
			CMessage::AsyWriteFile(GetGame()->GetSaveGoodsStatckFileName(),szOutPutStr);
#endif
			// ����ӱ�������
			pGroup->ReleaseChilds();
			// �������
			// �ӱ�����������
			CSubpackContainer* pSubpackContainer = player->GetSubpackContainer();
			for(int TmpSubPos=0; TmpSubPos < 5; TmpSubPos++)
			{
				CGoods *pGoods = pSubpackContainer->GetGoods(10001+TmpSubPos);
				if(pGoods)
				{
					GetGame()->GetEntityManager()->AddGoodsToPlayer(pPlayer, pGoods, (long)(PEI_PACK1+TmpSubPos), PEI_PACK);
				}
			}
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
	}

	if( ( bitValue & SAVE_DETAIL_ATTRWALLETGOODS)	)// ���
	{
		GetGame()->GetEntityManager()->DetailWalletCPlayerToCDBPlayer(player, pPlayer);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_gold]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
	}
	if( ( bitValue & SAVE_DETAIL_ATTRSILVERGOODS) )// ����
	{
		GetGame()->GetEntityManager()->DetailSilverCPlayerToCDBPlayer(player, pPlayer);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, "[gp_silver]");
		if(pGroup)
		{

			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
	}
	if( ( bitValue & SAVE_DETAIL_ATTRDEPOTGOLD)	)// �ֿ���
	{
		GetGame()->GetEntityManager()->DetailDepotWalletCPlayerToCDBPlayer(player, pPlayer);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_depotgold]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
	}
	if( ( bitValue & SAVE_DETAIL_ATTRDEPOTSILVER) )// �ֿ�����
	{
		GetGame()->GetEntityManager()->DetailDepotSilverCPlayerToCDBPlayer(player, pPlayer);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_depotsilver]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
	}
	if( ( bitValue & SAVE_DETAIL_ATTRPRIMARY) )// ���ֿ�
	{
		GetGame()->GetEntityManager()->DetailDepotCPlayerToCDBPlayer(player, pPlayer);
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_depot]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
	}

	
	if( ( bitValue & SAVE_DETAIL_ATTRSUBDEPOT1) )// �ֿ��ӱ���1
	{
		GetGame()->GetEntityManager()->DetailSubDepotCPlayerToCDBPlayer(player, pPlayer, 0);
		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subdepotone]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}

		isGoodsChanged = true;
		isSubGoodsDepotChanged = true;
	}

	if( ( bitValue & SAVE_DETAIL_ATTRSUBDEPOT2) )// �ֿ��ӱ���2
	{
		GetGame()->GetEntityManager()->DetailSubDepotCPlayerToCDBPlayer(player, pPlayer, 1);

		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subdepottwo]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}

		isGoodsChanged = true;
		isSubGoodsDepotChanged = true;
	}
	if( ( bitValue & SAVE_DETAIL_ATTRSUBDEPOT3) )// �ֿ��ӱ���3
	{
		GetGame()->GetEntityManager()->DetailSubDepotCPlayerToCDBPlayer(player, pPlayer, 2);

		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subdepotthree]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}

		isGoodsChanged = true;
		isSubGoodsDepotChanged = true;
	}
	if( ( bitValue & SAVE_DETAIL_ATTRSUBDEPOT4) )// �ֿ��ӱ���4
	{
		GetGame()->GetEntityManager()->DetailSubDepotCPlayerToCDBPlayer(player, pPlayer, 3);

		// �ӱ��������仯
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_subdepotfour]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}

		isGoodsChanged = true;
		isSubGoodsDepotChanged = true;
	}
	
	if(isSubGoodsDepotChanged) // �ֿ��ӱ����仯����ͳ��
	{
		// �ӱ�����
		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_depotsecondary]"));
		if(pGroup)
		{
			// ����ӱ�������
			pGroup->ReleaseChilds();
			// ������� �ӱ�����������
			CSubpackContainer *pContainer = (CSubpackContainer*)player->m_PlayerDepot.FindContainer(eDCT_Secondary);
			assert(NULL != pContainer);
			if(NULL == pContainer) return;
			for(int TmpSubPos=0; TmpSubPos < 4; TmpSubPos++)
			{
				CGoods *pGoods = pContainer->GetGoods(10001+TmpSubPos);
				if(pGoods)
				{
					GetGame()->GetEntityManager()->AddGoodsToPlayer(pPlayer, pGoods, (long)(10001+TmpSubPos), PEI_DCT_Secondary);
				}
			}
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
	}

	// ��ҵ�����浵
	if( bitValue & SAVE_DETAIL_ATTRBUSINESSPACK )
	{
		// ��CPlayerת�����ݵ�DBPlayer
		GetGame()->GetEntityManager()->DetailBusinessPackCPlayerToCDBPlayer( player, pPlayer );

		CEntityGroup* pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_businesspack_field]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}

		pGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pGoodsGroup, string("[gp_businesspack]"));
		if(pGroup)
		{
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pGroup, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pGroup, DB_OPER_DELETE_INSERT);
		}
		isGoodsChanged = true;
	}

	if(isGoodsChanged)
	{
		// �򿪲�����־
		pGoodsGroup->SetCurDbOperFlag(1);
		pGoodsGroup->SetCurDbOperType(DB_OPER_DELETE_INSERT);
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "NakeSendSavePlayerDataMsg() translate GoodsGroup End.");
#endif

	// С��������
	CEntityGroup* pSpriteGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pPlayer, string("[playerspritegroup]"));
	if(!pSpriteGroup) return;
	GetGame()->GetEntityManager()->DetailSpriteCPlayerToCDBPlayer(player, pPlayer);

	// Fox@20090108 ��ӳ����ѯ����, �����������
	if( bitValue & SAVE_DETAIL_PETS )
	{
		GetGame()->GetEntityManager()->SavePlayerPet(player, pPlayer);
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "NakeSendSavePlayerDataMsg() send message Start.");
#endif

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());

	msg.Add("[player]");
	msg.Add(player->GetExID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	pPlayer->SetCurDbOperType(DB_OPER_SAVE);
	pPlayer->SetCurDbOperFlag(1);
	pPlayer->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "NakeSendSavePlayerDataMsg() send message End.");
#endif
}

void CWorldServerSession::SendSaveLoginPlayerDataMsg(const CGUID& guid)
{
	CPlayer* pPlayer = GetGame()->GetMapPlayer(guid);
	if(!pPlayer) return;
	const char* szCdkey = pPlayer->GetAccount();
	if(szCdkey == NULL)
	{
		char szGuid[128];
		pPlayer->GetExID().tostring(szGuid);
		AddLogText("SendSaveLoginPlayerDataMsg: player[%s] cdkey is null!", szGuid);
		return;
	}
	Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
	if(pAcc == NULL)
	{
		// ����һ��LoginPlayer���ݱ���
		CEntityGroup* dbTmpLPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[loginplayer]"), pPlayer->GetExID());
		NakeSendSaveLoginPlayerDataMsg(pPlayer, dbTmpLPlayer);
		GetGame()->GetEntityManager()->DelBaseEntity((CBaseEntity*)dbTmpLPlayer);
		return;
	}

	CEntityGroup* pLoginPlayerGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pAcc, string("[loginplayergroup]"));
	if(pLoginPlayerGroup)
		pLoginPlayerGroup->SetCurDbOperType(DB_OPER_SAVE);

	CEntityGroup* dbLPlayer = pAcc->FindLoginPlayer(guid);
	if(dbLPlayer == NULL) return;

	pAcc->SetCurDbOperType(DB_OPER_SAVE);
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pAcc, true);

	NakeSendSaveLoginPlayerDataMsg(pPlayer, dbLPlayer);
}
void CWorldServerSession::SendSavePlayerDataMsg(long bitValue, const CGUID& guid, bool isSaveDbPlayerFlag)
{
	CPlayer* player = GetGame()->GetMapPlayer(guid);
	if(player == NULL) return;
	if(isSaveDbPlayerFlag == true) bitValue = SAVE_DETAIL_ALL;

	CEntityGroup* pPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[player]"), guid);
	//if(!pPlayer) return;

	NakeSendSavePlayerDataMsg(bitValue, player, pPlayer, isSaveDbPlayerFlag);
	GetGame()->GetEntityManager()->DelBaseEntity((CBaseEntity*)pPlayer);
}
void CWorldServerSession::SendLoadPlayerDataMsg(const char* szCdkey, const CGUID& guid)
{
	if(szCdkey == NULL) return;
	Account* acc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
	if(acc == NULL) return;

	// ������Ҫ��ѯ������
	TempDataRelease();
	CTempEntityProperty* pCdkey = new CTempEntityProperty(string("cdkey"), DATA_OBJECT_STRING);
	if(pCdkey)
	{
		pCdkey->SetStringAttr(szCdkey);
		pCdkey->SetDbRelateType(DB_RELATE_NULL);
		GetTempDataVec().push_back(pCdkey);
	}

	CEntityGroup* pPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[player]"), guid);
	if(!pPlayer) return;

	pPlayer->SetName(acc->GetName());
	pPlayer->SetStringAttr(string("Account"),  szCdkey);
	pPlayer->SetGuidAttr(string("guid"),  guid);

	const char* szName = pPlayer->GetStringAttr(string("Name"));
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	msg.Add(GetExID());

	msg.Add("[player]");
	msg.Add(guid);
	if(szName)
		msg.Add(szName);
	else
		msg.Add("");
	msg.Add((BYTE)1);

	vector<BYTE> pBA;
	// ΪGoodsGroup���ò�ѯ����
	CEntityGroup* pGoodslGroup = NULL;
	map<string, CGUID>::iterator guidItr = pPlayer->GetGuidByComFlagMap().find(string("[goodsgroup]"));
	if(guidItr != pPlayer->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pPlayer->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pPlayer->GetEntityGroupMap().end())
			pGoodslGroup = (CEntityGroup*)enItr->second;
	}
	if(pGoodslGroup)
	{
		pGoodslGroup->SetCurDbOperFlag(1);
		CTempEntityProperty* pOwnerID = new CTempEntityProperty(string("PlayerID"), DATA_OBJECT_GUID);
		if(pOwnerID)
		{
			pOwnerID->SetGuidAttr( guid);
			pOwnerID->SetDbRelateType(DB_RELATE_NULL);
			pGoodslGroup->GetQueryDataVec().push_back(pOwnerID);
		}
	}

	// ΪSpriteGroup���ò�ѯ����
	CEntityGroup* pSpriteGroup = NULL;
	guidItr = pPlayer->GetGuidByComFlagMap().find(string("[playerspritegroup]"));
	if(guidItr != pPlayer->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pPlayer->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pPlayer->GetEntityGroupMap().end())
			pSpriteGroup = (CEntityGroup*)enItr->second;
	}
	if(pSpriteGroup)
	{
		CTempEntityProperty* pOwnerGuid = new CTempEntityProperty(string("PlayerGuid"), DATA_OBJECT_GUID);
		pOwnerGuid->SetGuidAttr( guid);
		pOwnerGuid->SetDbRelateType(DB_RELATE_NULL);
		pSpriteGroup->GetQueryDataVec().push_back(pOwnerGuid);
	}

	// Ϊ�����б����ò�ѯ����
	CEntityGroup* pPets = NULL;
	guidItr = pPlayer->GetGuidByComFlagMap().find("[playerpetlist]");
	if(guidItr != pPlayer->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pPlayer->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pPlayer->GetEntityGroupMap().end())
			pPets = (CEntityGroup*)enItr->second;
	}
	if( pPets != NULL )
	{
		CTempEntityProperty* pOwnerGuid = new CTempEntityProperty("HostID", DATA_OBJECT_GUID);
		pOwnerGuid->SetGuidAttr( guid);
		pOwnerGuid->SetDbRelateType(DB_RELATE_NULL);
		pPets->GetQueryDataVec().push_back(pOwnerGuid);
	}

	GetGame()->GetEntityManager()->SetEntityAllDbOperType(pPlayer, DB_OPER_LOAD);
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pPlayer, true);
	pPlayer->AddToByteArray(pBA);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

	GetGame()->GetEntityManager()->DelBaseEntity((CBaseEntity*)pPlayer);
}

void CWorldServerSession::SendLoadLinkmanMsg(const CGUID &PlayerGuid)
{
	Release();
	// ��¼���β�����ҵ�GUID
	CTempEntityProperty* pCurGuid = new CTempEntityProperty(string("PlayerGuid"), DATA_OBJECT_GUID);
	pCurGuid->SetGuidAttr(PlayerGuid);
	pCurGuid->SetDbRelateType(DB_RELATE_NULL);
	GetTempDataVec().push_back(pCurGuid);

	CGUID LoadGuid;
	CGUID::CreateGUID(LoadGuid);
	CEntityGroup* pLinkmanRoot = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[friendgroup]"), LoadGuid);
	if (NULL == pLinkmanRoot)
		return;

	CEntityGroup* pLinkmanGroup = NULL;
	map<string, CGUID>::iterator guidItr = pLinkmanRoot->GetGuidByComFlagMap().find(string("[playerfriendgroup]"));
	if(guidItr != pLinkmanRoot->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pLinkmanRoot->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pLinkmanRoot->GetEntityGroupMap().end())
			pLinkmanGroup = (CEntityGroup*)enItr->second;
	}
	if (NULL == pLinkmanGroup)
		return;

	// ΪFriendGroup���ò�ѯ����
	CEntityGroup* pFriendGroupOwner = NULL;
	guidItr = pLinkmanGroup->GetGuidByComFlagMap().find(string("[playerfriendgroupOwner]"));
	if(guidItr != pLinkmanGroup->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pLinkmanGroup->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pLinkmanGroup->GetEntityGroupMap().end())
			pFriendGroupOwner = (CEntityGroup*)enItr->second;
	}
	if(NULL == pFriendGroupOwner)
		return;
	CTempEntityProperty* pOwnerGuid = new CTempEntityProperty(string("OwnerGUID"), DATA_OBJECT_GUID);
	pOwnerGuid->SetGuidAttr(PlayerGuid);
	pOwnerGuid->SetDbRelateType(DB_RELATE_NULL);
	pFriendGroupOwner->GetQueryDataVec().push_back(pOwnerGuid);

	// ΪFriendGroup���ò�ѯ����
	CEntityGroup* pFriendGroupAim = NULL;
	guidItr = pLinkmanGroup->GetGuidByComFlagMap().find(string("[playerfriendgroupAim]"));
	if(guidItr != pLinkmanGroup->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pLinkmanGroup->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pLinkmanGroup->GetEntityGroupMap().end())
			pFriendGroupAim = (CEntityGroup*)enItr->second;
	}
	if(NULL == pFriendGroupAim)
		return;
	CTempEntityProperty* pAimGuid = new CTempEntityProperty(string("AimGUID"), DATA_OBJECT_GUID);
	pAimGuid->SetGuidAttr(PlayerGuid);
	pAimGuid->SetDbRelateType(DB_RELATE_NULL);
	pFriendGroupAim->GetQueryDataVec().push_back(pAimGuid);

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());
	msg.Add("[friendgroup]");
	msg.Add(pLinkmanRoot->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	GetGame()->GetEntityManager()->SetEntityAllDbOperType(pLinkmanRoot, DB_OPER_LOAD_NOCACHE);
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pLinkmanRoot, true);
	pLinkmanRoot->AddToByteArray(pBA);

	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

	GetGame()->GetEntityManager()->DelBaseEntity(pLinkmanRoot);
}

void CWorldServerSession::SendSaveLinkmanMsg(const CGUID &PlayerGuid)
{
	CGUID SaveGuid;
	CGUID::CreateGUID(SaveGuid);
	CEntityGroup* pLinkmanRoot = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[friendgroup]"), SaveGuid);
	if (NULL == pLinkmanRoot)
		return;
	CTempEntityProperty* pOwnerGuid = new CTempEntityProperty(string("OwnerGUID"), DATA_OBJECT_GUID);
	pOwnerGuid->SetGuidAttr(PlayerGuid);
	pOwnerGuid->SetDbRelateType(DB_RELATE_NULL);
	pLinkmanRoot->GetQueryDataVec().push_back(pOwnerGuid);

	CEntityGroup* pLinkmanGroup = NULL;
	map<string, CGUID>::iterator guidItr = pLinkmanRoot->GetGuidByComFlagMap().find(string("[playerfriendgroup]"));
	if(guidItr != pLinkmanRoot->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pLinkmanRoot->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pLinkmanRoot->GetEntityGroupMap().end())
			pLinkmanGroup = (CEntityGroup*)enItr->second;
	}
	if (NULL == pLinkmanGroup)
		return;

	pOwnerGuid = new CTempEntityProperty(string("OwnerGUID"), DATA_OBJECT_GUID);
	pOwnerGuid->SetGuidAttr(PlayerGuid);
	pOwnerGuid->SetDbRelateType(DB_RELATE_NULL);
	pLinkmanGroup->GetQueryDataVec().push_back(pOwnerGuid);

	// ΪFriendGroup���ò�ѯ����
	CEntityGroup* pFriendGroupOwner = NULL;
	guidItr = pLinkmanGroup->GetGuidByComFlagMap().find(string("[playerfriendgroupOwner]"));
	if(guidItr != pLinkmanGroup->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pLinkmanGroup->GetEntityGroupMap().find(guidItr->second);
		if(enItr != pLinkmanGroup->GetEntityGroupMap().end())
			pFriendGroupOwner = (CEntityGroup*)enItr->second;
	}
	if(NULL == pFriendGroupOwner)
		return;
	pOwnerGuid = new CTempEntityProperty(string("OwnerGUID"), DATA_OBJECT_GUID);
	pOwnerGuid->SetGuidAttr(PlayerGuid);
	pOwnerGuid->SetDbRelateType(DB_RELATE_NULL);
	pFriendGroupOwner->GetQueryDataVec().push_back(pOwnerGuid);

	GetGame()->GetEntityManager()->CPlayerFriendToCDBFriendGroup(PlayerGuid, pFriendGroupOwner);

	CMessage msg( MSG_W2DB_OPER_ENTITY );
	msg.Add( GetExID() );
	msg.Add( "[friendgroup]" );
	msg.Add( pLinkmanRoot->GetGUID() );
	msg.Add( "" );
	msg.Add( (BYTE)0 );

	vector<BYTE> pBA;
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag( pLinkmanRoot, true );
	GetGame()->GetEntityManager()->SetEntityAllDbOperType( pLinkmanRoot, DB_OPER_DELETE_INSERT );

	// ��������aimGroup������־Ϊ0
	map<string, CGUID>::iterator aimItr = pLinkmanGroup->GetGuidByComFlagMap().find(string("[playerfriendgroupAim]"));
	if(aimItr != pLinkmanGroup->GetGuidByComFlagMap().end())
	{
		map<CGUID, CBaseEntity*>::iterator enItr = pLinkmanGroup->GetEntityGroupMap().find(aimItr->second);
		if(enItr != pLinkmanGroup->GetEntityGroupMap().end())
			enItr->second->SetCurDbOperFlag(0);
	}

	pLinkmanRoot->AddToByteArray( pBA );
	msg.Add((long)pBA.size());
	if(pBA.size())
	{
		msg.AddEx(&pBA[0], (long)pBA.size());
	}
	msg.SendToSocket(GetGame()->GetDBSocketID());

	GetGame()->GetEntityManager()->DelBaseEntity(pLinkmanRoot);
}

void CWorldServerSession::SendInitLoadMail(const CGUID& guid)
{
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CGUID groupGuid;
	CGUID::CreateGUID(groupGuid);
	CEntityGroup* pMailGroup = GetGame()->GetEntityManager()->CreateRootEntity("[mailgroup]", groupGuid);
	if(pMailGroup == NULL) return;


	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());
	msg.Add("[mailgroup]");
	msg.Add(pMailGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pMailGroup)
	{
		// �ͷ��������ӽڵ�
		pMailGroup->ReleaseChilds();

		pMailGroup->QueryDataRelease();

		// ��Ӳ�ѯ����
		CTempEntityProperty* pOwnerID = new CTempEntityProperty( string("PlayerID"), DATA_OBJECT_GUID);
		if(pOwnerID)
		{
			pOwnerID->SetGuidAttr( guid);
			pOwnerID->SetDbRelateType(DB_RELATE_NULL);
			pMailGroup->GetQueryDataVec().push_back(pOwnerID);
		}

		TempDataRelease();
		pOwnerID = new CTempEntityProperty( string("PlayerID"), DATA_OBJECT_GUID);
		if(pOwnerID)
		{
			pOwnerID->SetGuidAttr( guid);
			pOwnerID->SetDbRelateType(DB_RELATE_NULL);
			GetTempDataVec().push_back(pOwnerID);
		}
		pMailGroup->SetCurDbOperFlag(1);
		pMailGroup->SetCurDbOperType(DB_OPER_LOAD);
		pMailGroup->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}

// �������ż�
void CWorldServerSession::SendInsertMailMsg(CMail* pMail)
{
	if(pMail == NULL) return;

	// ����һ��Procdure
	CEntityGroup* pProcedure = NULL;
	CGUID ProcGuid;
	CGUID::CreateGUID(ProcGuid);
	pProcedure = (CEntityGroup*)(GetGame()->GetEntityManager()->CreateRootEntity(string("[procedure]"), ProcGuid));
	if(!pProcedure) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());
	msg.Add("[procedure]");
	msg.Add(ProcGuid);
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pProcedure)
	{
		pProcedure->QueryDataRelease();
		pProcedure->RetQueryDataRelease();
		pProcedure->ReleaseChilds();

		CEntityGroup* pDbMail = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[mail]"), pMail->GetExID());
		GetGame()->GetEntityManager()->CMailToCDBMail(pMail, pDbMail);
		 
		CTempEntityProperty* pWriterName = new CTempEntityProperty(string("WriterName"), DATA_OBJECT_STRING);
		if (pWriterName)
		{
			pWriterName->SetStringAttr(pMail->GetWriter());
			pProcedure->GetQueryDataVec().push_back(pWriterName);
		}

		// 
		CTempEntityProperty* pMailID = new CTempEntityProperty(string("mailGuid"), DATA_OBJECT_GUID);
		if (pMailID)
		{
			pMailID->SetGuidAttr( pMail->GetExID());
			pProcedure->GetQueryDataVec().push_back(pMailID);
		}

		//  
		CTempEntityProperty* pRecverName = new CTempEntityProperty(string("RecverName"), DATA_OBJECT_STRING);
		if (pRecverName)
		{
			pRecverName->SetStringAttr(pMail->GetReceiver());
			pProcedure->GetQueryDataVec().push_back(pRecverName);
		}

		CTempEntityProperty* pIncGoodsBuf = new CTempEntityProperty(string("incGoodsBuf"), DATA_OBJECT_BUFFER);
		if (pIncGoodsBuf)
		{
			long incBuf = 0;
			pIncGoodsBuf->SetBufAttr((BYTE*)&incBuf, sizeof(long));
			pProcedure->GetQueryDataVec().push_back(pIncGoodsBuf);
		}

		// ��Ӳ�ѯ����
		if(pDbMail)
		{
			CTempEntityProperty* pTMailID = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
			if (pTMailID)
			{
				pTMailID->SetGuidAttr( pMail->GetExID());
				pTMailID->SetDbRelateType(DB_RELATE_NULL);
				pDbMail->GetQueryDataVec().push_back(pTMailID);
			}
			// �򿪲�����־λ
			GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pDbMail, true);
			GetGame()->GetEntityManager()->SetEntityAllDbOperType(pDbMail, DB_OPER_INSERT);
			pProcedure->AddChild(pDbMail);
		}
		
		// ��ӷ���ֵ
		CTempEntityProperty* pRetValue = new CTempEntityProperty(string("retvalue"), DATA_OBJECT_LONG);
		if (pRetValue)
		{
			pRetValue->SetLongAttr(0);
			pProcedure->GetRetQueryDataVec().push_back(pRetValue);
		}
		// ���ִ�д洢������
		pProcedure->SetStringAttr(string("ProcName"),  "InsertMailProc");
		// �򿪲�����־λ
		pProcedure->SetCurDbOperFlag(1);
		pProcedure->SetCurDbOperType(DB_OPER_INSERTMAIL_PROCEDURE);
		pProcedure->AddToByteArray(pBA);

		// �����ʱ��¼
		CTempEntityProperty* pTempMailID = new CTempEntityProperty(string("MailID"), DATA_OBJECT_GUID);
		if(pTempMailID)
		{
			pTempMailID->SetGuidAttr(pMail->GetExID());
			GetTempDataVec().push_back(pTempMailID);
		}
	}

	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
// �������ɾ���ż�
void CWorldServerSession::SendDeleteOnlineMailMsg(const CGUID& ownerID, vector<CMail*>& mails)
{
	if(mails.size() == 0) return;

	CGUID groupGuid;
	CGUID::CreateGUID(groupGuid);
	CEntityGroup* pMailGroup = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity("[mailgroup]", groupGuid);
	if(!pMailGroup) return;
	pMailGroup->ReleaseChilds();

	for(int i=0; i<mails.size(); i++)
	{
		CEntityGroup* pDbMail = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[mail]"), mails[i]->GetExID());
		GetGame()->GetEntityManager()->CMailToCDBMail(mails[i], pDbMail);
		pMailGroup->AddChild(pDbMail);
		// add query guid for mail
		CTempEntityProperty* pOwnerID = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
		if(pOwnerID)
		{
			pOwnerID->SetGuidAttr( pDbMail->GetGUID());
			pOwnerID->SetDbRelateType(DB_RELATE_NULL);
			pDbMail->GetQueryDataVec().push_back(pOwnerID);
		}
		// add query guid for mailgroup
		CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pDbMail, "[mailgoodsgroup]");
		if(pGoodsGroup)
		{
			CTempEntityProperty* pMailGuid = new CTempEntityProperty(string("MailID"), DATA_OBJECT_GUID);
			if(pMailGuid)
			{
				pMailGuid->SetGuidAttr( pDbMail->GetGUID());
				pMailGuid->SetDbRelateType(DB_RELATE_NULL);
				pGoodsGroup->GetQueryDataVec().push_back(pMailGuid);
			}
		}
	}

	// record player guid
	CTempEntityProperty* pOwnerID = new CTempEntityProperty(string("OwnerID"), DATA_OBJECT_GUID);
	if (pOwnerID)
	{
		pOwnerID->SetGuidAttr( ownerID);
		GetTempDataVec().push_back(pOwnerID);
	}
	//  record delete mails guid
	for(int i=0; i<mails.size(); i++)
	{
		CTempEntityProperty* pGuid = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
		if(pGuid)
		{
			pGuid->SetGuidAttr( mails[i]->GetExID());
			GetTempDataVec().push_back(pGuid);
		}
	}

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());
	msg.Add("[mailgroup]");
	msg.Add(pMailGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	// �򿪲�����־λ
	GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pMailGroup, true);
	GetGame()->GetEntityManager()->SetEntityAllDbOperType(pMailGroup, DB_OPER_DELETE);
	pMailGroup->SetCurDbOperType(DB_OPER_DELETE_INSERT);
	vector<BYTE> pBA;
	pMailGroup->AddToByteArray(pBA);

	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

	// ���mailgroup����
	GetGame()->GetEntityManager()->DelBaseEntity((CBaseEntity*)pMailGroup);
}
// ������ұ����ż�
void CWorldServerSession::SendSaveOnlineMailMsg(const CGUID& ownerID, vector<CMail*>& mails)
{
	if(mails.size() == 0) return;

	CGUID groupGuid;
	CGUID::CreateGUID(groupGuid);
	CEntityGroup* pMailGroup = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity("[mailgroup]", groupGuid);
	if(!pMailGroup) return;
	pMailGroup->ReleaseChilds();

	for(int i=0; i<mails.size(); i++)
	{
		CEntityGroup* pDbMail = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[mail]"), mails[i]->GetExID());
		GetGame()->GetEntityManager()->CMailToCDBMail(mails[i], pDbMail);
		pMailGroup->AddChild(pDbMail);

		// add query guid for mail
		CTempEntityProperty* pOwnerID = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
		if(pOwnerID)
		{
			pOwnerID->SetGuidAttr( pDbMail->GetGUID());
			pOwnerID->SetDbRelateType(DB_RELATE_NULL);
			pDbMail->GetQueryDataVec().push_back(pOwnerID);
		}
		// add query guid for mailgroup
		CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pDbMail, "[mailgoodsgroup]");
		if(pGoodsGroup)
		{
			CTempEntityProperty* pMailGuid = new CTempEntityProperty(string("MailID"), DATA_OBJECT_GUID);
			if(pMailGuid)
			{
				pMailGuid->SetGuidAttr( pDbMail->GetGUID());
				pMailGuid->SetDbRelateType(DB_RELATE_NULL);
				pGoodsGroup->GetQueryDataVec().push_back(pMailGuid);
			}
		}

		GetGame()->GetEntityManager()->SetEntityAllDbOperType(pDbMail, DB_OPER_DELETE_INSERT);
		GetGame()->GetEntityManager()->ResetEntityAllDbOperFlag(pDbMail, true);
		pDbMail->SetCurDbOperType(DB_OPER_SAVE);
		pDbMail->SetCurDbOperFlag(1);
	}

	// record player guid
	CTempEntityProperty* pOwnerID = new CTempEntityProperty(string("OwnerID"), DATA_OBJECT_GUID);
	if (pOwnerID)
	{
		pOwnerID->SetGuidAttr( ownerID);
		GetTempDataVec().push_back(pOwnerID);
	}
	//  record delete mails guid
	for(int i=0; i<mails.size(); i++)
	{
		CTempEntityProperty* pGuid = new CTempEntityProperty(string("guid"), DATA_OBJECT_GUID);
		if(pGuid)
		{
			pGuid->SetGuidAttr( mails[i]->GetExID());
			GetTempDataVec().push_back(pGuid);
		}
	}

	CMessage msg(MSG_W2DB_OPER_ENTITY);
	msg.Add(GetExID());
	msg.Add("[mailgroup]");
	msg.Add(pMailGroup->GetGUID());
	msg.Add("");
	msg.Add((BYTE)0);

	// �򿪲�����־λ
	pMailGroup->SetCurDbOperType(DB_OPER_SAVE);
	pMailGroup->SetCurDbOperFlag(1);
	vector<BYTE> pBA;
	pMailGroup->AddToByteArray(pBA);

	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());

	// ���mailgroup����
	GetGame()->GetEntityManager()->DelBaseEntity((CBaseEntity*)pMailGroup);
}

// Ban Player proc
void CWorldServerSession::SendBanPlayerProcMsg(const char* szPlayerName, long time)
{
	if(szPlayerName == NULL) return;

	CMessage msg(MSG_W2DB_OPER_ENTITY);

	CGUID ProcGuid;
	CGUID::CreateGUID(ProcGuid);
	CEntityGroup* pProcedure = NULL;
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	pProcedure = (CEntityGroup*)(GetGame()->GetEntityManager()->CreateRootEntity(string("[procedure]"), ProcGuid));
	if(!pProcedure) return;

	msg.Add(GetExID());
	msg.Add("[procedure]");
	msg.Add(ProcGuid);
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pProcedure)
	{
		pProcedure->QueryDataRelease();
		pProcedure->RetQueryDataRelease();

		// ��Ӳ�ѯ����
		CTempEntityProperty* pName = new CTempEntityProperty( string("name"), DATA_OBJECT_STRING);
		if(pName)
		{
			pName->SetStringAttr(szPlayerName);
			pName->SetDbRelateType(DB_RELATE_NULL);
			pProcedure->GetQueryDataVec().push_back(pName);
		}

		// ��Ӳ�ѯ��������
		CTempEntityProperty* pRetCdkey = new CTempEntityProperty( string("name"), DATA_OBJECT_STRING);
		if(pRetCdkey)
		{
			pRetCdkey->SetStringAttr(szPlayerName);
			pRetCdkey->SetDbRelateType(DB_RELATE_NULL);
			pProcedure->GetRetQueryDataVec().push_back(pRetCdkey);
		}

		// �����ʱ��¼
		CTempEntityProperty* pTmpName = new CTempEntityProperty( string("name"), DATA_OBJECT_STRING);
		if(pTmpName)
		{
			pTmpName->SetStringAttr(szPlayerName);
			GetTempDataVec().push_back(pTmpName);
		}

		// ���ִ�д洢������
		pProcedure->SetStringAttr(string("ProcName"),  "CdkeyCheckProc");
		pProcedure->SetCurDbOperType(DB_OPER_PROCEDURE);
		pProcedure->SetCurDbOperFlag(1);
		pProcedure->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
//---------------------------------------------------
// Process Session �ӿ�
//---------------------------------------------------
void CWorldServerSession::ProcessLoadAccount(long retFlag, CEntityGroup* pEntity)
{
	if(!pEntity) return;
	Account* pAcc = (Account*)pEntity;

	BYTE nRoleCnt;
	CGUID playerid;

	CEntityGroup* pLoginPlayerGroup = (CEntityGroup*)GetGame()->GetEntityManager()->FindLeafByComFlag(pAcc, string("[loginplayergroup]"));
	if(!pLoginPlayerGroup)
	{
		AddLogText("ProcessLoadAccount() ���[%s] Account���󴴽�ʧ��,DBS�������ݶ�ʧ!", pAcc->GetName());

		CMessage pMsga(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);
		// �����֤����
		pMsga.Add((long)0xFFFFFFFF);
		pMsga.Add((BYTE)0);// flag
		pMsga.Add((pAcc->GetName()));// cdkey
		pMsga.Add((short)0);// flag
		pMsga.SendToSocket(GetGame()->GetLSSocketID());
		pAcc->SetPlayerState(PLAYER_STATE_VOID);
		pAcc->SetLoginTimeoutValue(timeGetTime());
		return;
	}
	nRoleCnt=BYTE(pLoginPlayerGroup->GetEntityGroupMap().size());

	char szGUID[128]={0};

	CMessage logMsg(MSG_W2L_LOG_ANSWER_PLAYERBASELIST);

	// ������֤��
	long lTestSignCode = 0;
	GetGame()->GenerateSignCodeByTime(lTestSignCode);
	// �����֤����
	logMsg.Add((long)lTestSignCode);

	if(retFlag == S_OK)
	{
		logMsg.Add((BYTE)1);
		pAcc->SetPlayerState(PLAYER_STATE_LOADED_CHAR_DATA);
	}
	else
	{
		logMsg.Add((BYTE)0);
	}
	logMsg.Add(pAcc->GetName());
	logMsg.Add(GetGame()->GetCountryLessPlayer());
	logMsg.Add((short)nRoleCnt);
	pAcc->SetSignCode(lTestSignCode);
	if(0 < nRoleCnt)
	{
		map<CGUID, CBaseEntity*>::iterator itr = pLoginPlayerGroup->GetEntityGroupMap().begin();
		for(; itr != pLoginPlayerGroup->GetEntityGroupMap().end(); itr++)
		{
			CGUID lplayerID = itr->first;
			lplayerID.tostring(szGUID);
			CEntityGroup* lPlayer = (CEntityGroup*)itr->second;
			if(lPlayer)
			{
				lPlayer->SetName(pAcc->GetName());
				pAcc->SetPlayerState(PLAYER_STATE_LOADED_BASE_DATA);
				if(!lPlayer->GetStringAttr(string("Name")))
					AddLogText("���%s����Ϊ��",pAcc->GetName());
				// ���
				logMsg.Add((short)nRoleCnt);//��ɫ����
				logMsg.Add(lplayerID);
				logMsg.Add(lPlayer->GetStringAttr(string("Name")));//�õ�����
				logMsg.Add((WORD)lPlayer->GetLongAttr(string("Levels")));//�õ��ȼ�
				//string strName = lPlayer->GetStringAttr(string("Name"));
				logMsg.Add((short)lPlayer->GetLongAttr(string("Occupation")));//�õ�ְҵ���				
				logMsg.Add((short)lPlayer->GetLongAttr(string("Sex")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("Country")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("HEAD")));			
				logMsg.Add((long)lPlayer->GetLongAttr(string("HAIR")));			
				logMsg.Add((long)lPlayer->GetLongAttr(string( "FACE")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("NECKLACE")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("WING")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("BODY")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("BACK")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("GLOVE")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("BOOT")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("HEADGEAR")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("FROCK")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("LRING")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("RRING")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("MEDAL1")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("MEDAL2")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("MEDAL3")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("FAIRY")));	
				logMsg.Add((short)lPlayer->GetLongAttr(string("WEAPON")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("WEAPON2")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("CLOAK")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("HeadLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("NecklaceLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("WingLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("BodyLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("BackLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("GloveLevel")));	
				logMsg.Add((short)lPlayer->GetLongAttr(string("BootLevel")));	
				logMsg.Add((short)lPlayer->GetLongAttr(string("HeadgearLevel")));	
				logMsg.Add((short)lPlayer->GetLongAttr(string("FrockLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("LRingLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("RRingLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("Medal1Level")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("Medal2Level")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("Medal3Level")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("FairyLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("WeaponLevel")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("Weapon2Level")));
				logMsg.Add((short)lPlayer->GetLongAttr(string("CloakLevel")));
				//DWORD dwNo = (DWORD)lPlayer->GetLongAttr(string("CreateNo"));
				//logMsg.Add((DWORD)lPlayer->GetLongAttr(string("CreateNo")));
				//DWORD dwLogoutTime = (DWORD)lPlayer->GetLongAttr(string("LogoutTime"));
				logMsg.Add((DWORD)lPlayer->GetLongAttr(string("LogoutTime")));
				//--Start ����ɾ��ʱ��
				long delTime[6];
				long m_DelTime = 0;
				lPlayer->GetTimeAttr(string("DelDate"),  delTime, sizeof(long)*6);

				if(delTime[0] == 0 && delTime[1] == 0 && delTime[2] == 0
					&& delTime[3] == 0 && delTime[4] == 0 && delTime[5] == 0)
				{
					m_DelTime = -1;
					logMsg.Add((long)m_DelTime);
					logMsg.Add((long)lPlayer->GetLongAttr(string("Region")));
					continue;
				}

				time_t tCurTime = time(0);
				tm *pCurTime = localtime(&tCurTime);

				tm tt;
				memset(&tt,0, sizeof(tm));
				tt.tm_year= delTime[0]-1900; tt.tm_mon = delTime[1]-1; tt.tm_mday = delTime[2];
				DWORD res = mktime(&tt);
				if(res == -1)
				{
					m_DelTime = -1;
					logMsg.Add((long)m_DelTime);
					logMsg.Add((long)lPlayer->GetLongAttr(string("Region")));
					continue;
				}

				double dDiffSecond = difftime(tCurTime, res);
				dDiffSecond -= 86399.0;
				short days = -short(dDiffSecond/86400.0);
				m_DelTime = days;

				if(m_DelTime < 0) m_DelTime = 0; 
				//--End ����ɾ��ʱ��
				logMsg.Add((long)m_DelTime);
				logMsg.Add((long)lPlayer->GetLongAttr(string("Region")));
			}
			AddLogText("MSG_DB2W_PASS_LOG_PLAYERBASELIST!MSGID[1175297],���ID[%s],Cdkey[%s],ret[%d]!", szGUID, pAcc->GetName(), retFlag);
		}
		logMsg.SendToSocket(GetGame()->GetLSSocketID());

		pAcc->SetLoginTimeoutValue(timeGetTime());
		return;
	}
	else
	{
		logMsg.Add((short)0);
		// ���
		logMsg.Add((short)0);//��ɫ����
		logMsg.SendToSocket(GetGame()->GetLSSocketID());
		pAcc->SetPlayerState(PLAYER_STATE_VOID);
		pAcc->SetLoginTimeoutValue(timeGetTime());
	}
	if(retFlag != S_OK)
	{
		// ������Ϣ��DBS���DB����
		GetGame()->SendUnRegDBEntity(pAcc->GetName());
		GetGame()->GetEntityManager()->DelAccount(pAcc);
	}
}
void CWorldServerSession::ProcessLoadCountry(long retFlag,CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{
		CEntityGroup* pCountryGroup = (CEntityGroup*)pBaseEntity;
		// ���CCountry
		GetCountryHandler()->Release();
		map<CGUID, CBaseEntity*>::iterator itr = pCountryGroup->GetEntityGroupMap().begin();
		for(; itr != pCountryGroup->GetEntityGroupMap().end(); itr++)
		{
			CCountry* country = MP_NEW CCountry;
			GetGame()->GetEntityManager()->CDBCountryToCCountry(itr->second, country);
			GetCountryHandler()->Append(country);
		}

		map<DWORD, CGame::tagGameServer>::iterator gameServerItr = GetGame()->GetGameServerList()->begin();
		for(; gameServerItr!=GetGame()->GetGameServerList()->end(); gameServerItr++)
		{
			if(gameServerItr->second.bConnected)
			{
				// ���Ҳ�����
				vector<BYTE> vectorByte;
				vectorByte.clear();
				GetCountryHandler()->AddToByteArray(&vectorByte);
				CMessage msgCountry(MSG_W2S_SERVER_SENDSETUP);
				msgCountry.Add((long)SI_COUNTRY);
				msgCountry.Add((long)vectorByte.size());
				msgCountry.AddEx(&vectorByte[0], (long)vectorByte.size());
				msgCountry.SendGSID(gameServerItr->first);
			}
		}
		GetGame()->SetInitCountryLoadFlag(true);
	}
}
void CWorldServerSession::ProcessLoadFaction(long retFlag,CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{
		CEntityGroup* pCountryGroup = (CEntityGroup*)pBaseEntity;

		GetOrganiCtrl()->ClearFaction();
		map<CGUID, CBaseEntity*>::iterator itr = pCountryGroup->GetEntityGroupMap().begin();
		for(; itr != pCountryGroup->GetEntityGroupMap().end(); itr++)
		{
			CFaction* pFaction = MP_NEW CFaction();
			GetGame()->GetEntityManager()->CDBFactionToCFaction((CEntityGroup*)itr->second, pFaction);
			if(!GetOrganiCtrl()->AddFactionOrganizing(pFaction))
				MP_DELETE(pFaction);

		}

		GetOrganiCtrl()->OnInitFactionEndMsg();
	}
}

void CWorldServerSession::ProcessLoadUnion(long retFlag,CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{
		CEntityGroup* pCountryGroup = (CEntityGroup*)pBaseEntity;

		map<CGUID, CBaseEntity*>::iterator itr = pCountryGroup->GetEntityGroupMap().begin();
		GetOrganiCtrl()->ClearUnion();
		for(; itr != pCountryGroup->GetEntityGroupMap().end(); itr++)
		{
			CUnion* pUnion = MP_NEW CUnion();
			GetGame()->GetEntityManager()->CDBUnionToCUnion((CEntityGroup*)itr->second, pUnion);
			GetOrganiCtrl()->AddUnionOrganizing(pUnion);
		}

		GetOrganiCtrl()->OnInitUnionEndMsg();
	}
}

void CWorldServerSession::ProcessLoadRegion(long retFlag,CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{
		CEntityGroup* pCountryGroup = NULL;//(CEntityGroup*)pBaseEntity;
		CEntityManager::DBEntityComponentMapItr rgnMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[regiongroup]"));
		if(rgnMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
		{
			pCountryGroup = (CEntityGroup*)(rgnMapItr->second.begin()->second);
			map<CGUID, CBaseEntity*>::iterator itr = pCountryGroup->GetEntityGroupMap().begin();
			for(; itr != pCountryGroup->GetEntityGroupMap().end(); itr++)
			{

				CWorldRegion* rgn = MP_NEW CWorldRegion;
				GetGame()->GetEntityManager()->CDBRegionToCRegion((CEntityGroup*)itr->second, rgn);
				char szGuid[128];
				rgn->GetExID().tostring(szGuid);
				AddLogText("������[%s]!", szGuid);
				//GetCountryHandler()->Append(country);
			}
		}
	}
	GetGame()->SetInitRegionLoadFlag(true);
}

void CWorldServerSession::ProcessLoadGlobalVar(long retFlag,CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{
		if(GetGame()->GetGeneralVariableList())
		{
			CEntityGroup* pVarGroup = NULL;//(CEntityGroup*)pBaseEntity;
			CEntityManager::DBEntityComponentMapItr rgnMapItr = GetGame()->GetEntityManager()->GetBaseEntityMap().find(string("[scriptvargroup]"));
			if(rgnMapItr != GetGame()->GetEntityManager()->GetBaseEntityMap().end())
			{
				pVarGroup = (CEntityGroup*)(rgnMapItr->second.begin()->second);
				GetGame()->GetEntityManager()->CDBGlobalVarGroupToCGlobalVar(pVarGroup);
				AddLogText("����ȫ�ֱ���[%d]����ʼ����ȡ�ɹ�!",pVarGroup->GetEntityGroupMap().size());
			}
		}
	}
	else
	{
		AddLogText("δ�ܴ�DBSȡ��ȫ�ֱ������ݣ�����DBS״̬��������WS��");
		return;
	}

	if(GetGame()->GetGeneralVariableList())
	{
		// ��GS����ȫ�ֱ�������
		CMessage msgGereralVariable(MSG_W2S_SERVER_SENDSETUP);
		msgGereralVariable.Add((long)SI_GENERALVARIABLELIST);
		vector<BYTE> GererData;
		GetGame()->GetGeneralVariableList()->AddToByteArray(&GererData);
		msgGereralVariable.Add((long)GererData.size());
		if(GererData.size() > 0)
			msgGereralVariable.AddEx(&GererData[0],GererData.size());
		msgGereralVariable.SendAll();
	}
	GetGame()->SetInitVarLoadFlag(true);
}

bool CWorldServerSession::ProcessCreatePlayerCheck(long retFlag,CEntityGroup* pBaseEntity)
{
	if(GetTempDataVec().size() != 1 
		|| GetTempDataVec()[0] == NULL) return false;

	if(pBaseEntity && retFlag == S_OK)
	{
		CEntityGroup* pProcedure = (CEntityGroup*)pBaseEntity;
		// ȡ�ò�ѯ����ֵ
		long NameIsExist = 0;
		long charNum = 0;
		if( pProcedure->GetRetQueryDataVec().size() == 2
			&& pProcedure->GetRetQueryDataVec()[0]
			&& pProcedure->GetRetQueryDataVec()[1] )
		{
			NameIsExist = pProcedure->GetRetQueryDataVec()[0]->GetLongAttr();
			charNum = pProcedure->GetRetQueryDataVec()[1]->GetLongAttr();
		}

		// ��ʱ����
		CTempEntityProperty* pEP = GetTempDataVec()[0];
		if(!pEP) return false;

		Account* pAcc = GetGame()->GetEntityManager()->FindAccount(pEP->GetStringAttr());

		if( (NameIsExist != 0) )// ����ͬ�����
		{
			// �����˺�״̬
			if(pAcc)
			{
				pAcc->SetPlayerState(PLAYER_STATE_LOADED_BASE_DATA);
				pAcc->SetLoginTimeoutValue(timeGetTime());
			}
			
			CMessage msgx(MSG_W2L_LOG_CREATEROLE);
			msgx.Add((char)LOGIN_CREATEROLE_NAMEEXIST);
			msgx.Add(pEP->GetStringAttr());
			msgx.SendToSocket(GetGame()->GetLSSocketID());
			
			return false;
		}
		if( (charNum != 1) ) // �����ﵽ�����������������
		{
			// �����˺�״̬
			if(pAcc)
			{
				pAcc->SetPlayerState(PLAYER_STATE_LOADED_BASE_DATA);
				pAcc->SetLoginTimeoutValue(timeGetTime());
			}

			CMessage msgx(MSG_W2L_LOG_CREATEROLE);
			msgx.Add((char)LOGIN_CREATEROLE_FULL);
			msgx.Add(pEP->GetStringAttr());
			msgx.SendToSocket(GetGame()->GetLSSocketID());
			return false;
		}

		if(((CEntityGroup*)pProcedure)->GetEntityGroupMap().size() != 2)
		{
			CMessage msgx(MSG_W2L_LOG_CREATEROLE);
			msgx.Add((char)LOGIN_CREATEROLE_DBERROR);
			msgx.Add(pEP->GetStringAttr());
			msgx.SendToSocket(GetGame()->GetLSSocketID());
			return false;
		}

		// loginplayer
		CEntityGroup* pLPlayer = NULL;
		CGUID tmpGuid;
		map<CGUID, CBaseEntity*>::iterator itr = ((CEntityGroup*)pProcedure)->GetEntityGroupMap().begin();
		itr->second->GetGuidAttr(string("guid"),  tmpGuid);
		const char* szCdkey = itr->second->GetStringAttr(string("Account"));
		if(szCdkey)
		{
			if(pAcc)
			{
				pLPlayer = pAcc->FindLoginPlayer(tmpGuid);
				if(!pLPlayer)
				{
					pLPlayer = (CEntityGroup*)GetGame()->GetEntityManager()->NewBaseEntity(string("[loginplayer]"), tmpGuid);
					pAcc->AddLoginPlayer(pLPlayer);
				}
				if(pLPlayer)
				{
					*pLPlayer = *(CEntityGroup*)itr->second;
					pLPlayer->SetGUID(tmpGuid);
				}

				pAcc->SetPlayerState(PLAYER_STATE_LOADED_BASE_DATA);
			}
		}
		
		// DBPlayer
		itr++;
		CEntityGroup* pDBPlayer = (CEntityGroup*)itr->second;

		CPlayer *pPlayer = NULL;
		pPlayer = GetGame()->GetMapPlayer(tmpGuid);
		bool isCreate = false;
		if(pPlayer == NULL)
		{
			isCreate = true;
			pPlayer = GetGame()->NewRawPlayer(ePlayerProcessCreateProc);	
			pPlayer->SetExID(tmpGuid);
		}
		
		if(!pLPlayer || !pDBPlayer || !pPlayer)
		{
			GetGame()->DelRawPlayer(pPlayer);
			CMessage msgx(MSG_W2L_LOG_CREATEROLE);
			msgx.Add((char)LOGIN_CREATEROLE_DBERROR);
			msgx.Add((GetTempDataVec()[0])->GetStringAttr());
			msgx.SendToSocket(GetGame()->GetLSSocketID());
			return false;
		}

		GetGame()->GetEntityManager()->CDBLoginPlayerToCPlayer(pLPlayer, pPlayer);
		GetGame()->GetEntityManager()->CDBPlayerToCPlayer(pDBPlayer, pPlayer);
		if(isCreate)
			GetGame()->AppendMapPlayer(pPlayer);
		
		// ��ͻ��˷��ʹ����ɹ���Ϣ
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
		msg.Add(GetTempDataVec()[0]->GetStringAttr());
		msg.Add(pPlayer->GetExID());
		msg.Add(pPlayer->GetName());
		msg.Add((short)pPlayer->GetLevel());
		//msg.Add((DWORD)pPlayer->GetCreateNo());
		msg.Add((char)pPlayer->GetOccupation());
		msg.Add((char)pPlayer->GetSex());
		msg.Add((BYTE)pPlayer->GetCountry());
		msg.Add((BYTE)pPlayer->GetHeadPic());
		msg.Add((BYTE)pPlayer->GetHeadPic());
		msg.Add((BYTE)pPlayer->GetFacePic());

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

		// ���ʹ����ɹ���Ϣ��LS
		msg.SendToSocket(GetGame()->GetLSSocketID());
		return true;
	}

	// ��ͻ��˷��ʹ���ʧ����Ϣ
	CMessage msgx(MSG_W2L_LOG_CREATEROLE);
	msgx.Add((char)LOGIN_CREATEROLE_FAILED);
	msgx.Add(GetTempDataVec()[0]->GetStringAttr());
	msgx.SendToSocket(GetGame()->GetLSSocketID());
	return false;
}
bool CWorldServerSession::ProcessCheckOfflinePlayerNameProc(long retFlag, CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{
		CGUID mailID;
		if(GetTempDataVec().size() != 1)
			return false;
		// ȡ�ò�ѯ������GUID
		if(pBaseEntity->GetRetQueryDataVec().size() != 1)
			return false;
		CGUID ownerID;
		if(pBaseEntity->GetRetQueryDataVec()[0])
			pBaseEntity->GetRetQueryDataVec()[0]->GetGuidAttr( ownerID);
		// ȡ���ʼ�GUID
		GetTempDataVec()[0]->GetGuidAttr( mailID);
		CMail* pMail = GetGame()->GetMailManager()->GetMail(mailID);
		if(pMail && ownerID != NULL_GUID)
		{			
			pMail->SetReceiverExID(ownerID);
			SendInsertMailMsg(pMail);
			GetGame()->GetMailManager()->SendMailMsgToGS(mailID,1);
		}
		else if(pMail && ownerID == NULL_GUID)
		{
			//���ݿⱣ����������ʼ�ʧ��
			GetGame()->GetMailManager()->SendMailMsgToGS(mailID,0);

		}
	}
	return true;
}

bool CWorldServerSession::ProcessLoadPlayerData(long retFlag,CEntityGroup* pBaseEntity)
{
	char* szCdkey = NULL;
	if(GetTempDataVec().size() != 1)
		return false;
	if(GetTempDataVec()[0])
		szCdkey = (char*)GetTempDataVec()[0]->GetStringAttr();

	Account* pAcc = GetGame()->GetEntityManager()->FindAccount(szCdkey);
	if(!pAcc)
	{
		// ��ͻ��˷���ʧ����Ϣ
		CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
		// �����֤����
		msg.Add((long)0xFFFFFFFF);
		msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
		msg.Add(szCdkey);
		msg.Send();
		return false;
	}

	// ȡ����ҵ�¼����
	pAcc->SetCurWsSessionStep(WORLD_SESSION_STEP_NULL);

	if(pBaseEntity && retFlag == S_OK && szCdkey)
	{
		char szGUID[128];
		pBaseEntity->GetGUID().tostring(szGUID);

		CEntityGroup* lPlayer = (CEntityGroup*)pAcc->FindLoginPlayer(pBaseEntity->GetGUID());
		if(NULL == lPlayer)
		{
			AddLogText("--[W2DB�����������ʧ��,��¼��������Ƿ���¼!ID[%s],CdKey[%s]!", szGUID, szCdkey);
			// ��ͻ��˷���ʧ����Ϣ
			CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
			// �����֤����
			msg.Add((long)0xFFFFFFFF);
			msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
			msg.Add(szCdkey);
			msg.Send();
			return false;
		}

		// �ȼ���Ƿ��ɫ�Ѿ�������ɾ�����
		time_t tDel = GetGame()->GetEntityManager()->ComputeLoginPlayerDelTime(pBaseEntity);
		if(!tDel) 
		{
			tDel = (time_t)time;
		}

		// ɾ�����ڲ�Ϊ0������ڣ����Բ��ܵ�½
		if(tDel>0)
		{
			AddLogText("--[W2DB�����������ʧ��,������ϸ��Ϣʧ��(ɾ�����ڲ�Ϊ0)!ID[%s],CdKey[%s]!", szGUID, szCdkey);

			CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
			// ������֤��
			long lSignCode = 0;
			GetGame()->GenerateSignCodeByTime(lSignCode);
			pAcc->SetSignCode(lSignCode);
			// �����֤����
			msg.Add((long)lSignCode);
			msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
			msg.Add(szCdkey);
			msg.SendToSocket(GetGame()->GetLSSocketID());	
			pAcc->SetPlayerState(PLAYER_STATE_LOADED_BASE_DATA);
			return false;
		}	

		CPlayer* pPlayer = GetGame()->GetMapPlayer(pBaseEntity->GetGUID());
		// ���CPlayer
		bool isPlayerFindFlag = false;
		if(NULL == pPlayer)
		{
			pPlayer = GetGame()->NewRawPlayer(ePlayerLoadData);
			pPlayer->SetExID(pBaseEntity->GetGUID());
			// ��ʼ��Account����
			pPlayer->SetAccount(szCdkey);
			isPlayerFindFlag = true;
		}
		if(pPlayer)
		{
			GetGame()->GetEntityManager()->CDBPlayerToCPlayer(pBaseEntity, pPlayer);
			if(isPlayerFindFlag)
				GetGame()->AppendMapPlayer(pPlayer);

			//// �������createno 
			//pPlayer->SetCreateNo(lPlayer->GetLongAttr(std::string("CreateNo")));
		}
		pAcc->SetPlayerState(PLAYER_STATE_LOADED_CHAR_DATA);		
		// ����LS��Ϣ
		GetGame()->InitPlayerInfo(pPlayer, szCdkey);
		return true;
	}
	else
	{
		// ��ͻ��˷���ʧ����Ϣ
		CMessage msg(MSG_W2L_LOG_ANSWER_PLAYERDATA);
		// �����֤����
		msg.Add((long)0xFFFFFFFF);
		msg.Add((char)LOGIN_QUEST_PLAYERDATA_FAILED);
		msg.Add(szCdkey);
		msg.Send();
	}
	return false;
}


bool CWorldServerSession::ProcessSaveLoginPlayerData(long retFlag,CEntityGroup* pBaseEntity)
{
	char szGuid[128];
	pBaseEntity->GetGUID().tostring(szGuid);
	if(retFlag == S_OK)
		AddLogText("�������[%s]�������ݳɹ�!", szGuid);
	else
		AddLogText("�������[%s]��������ʧ��!", szGuid);
	return true;	
}
bool CWorldServerSession::ProcessSavePlayerData(long retFlag,CEntityGroup* pBaseEntity)
{
	if(!pBaseEntity) return false;
	return true;
}
bool CWorldServerSession::ProcessSaveFaction(long retFlag,CEntityGroup* pBaseEntity)
{
	if(NULL == pBaseEntity)
	{
		assert(false);
		return false;
	}

	char szGuid[128];
	pBaseEntity->GetGUID().tostring(szGuid);
	CFaction *pFac = GetOrganiCtrl()->GetFactionOrganizing(pBaseEntity->GetGUID());

	if(retFlag == S_OK)
	{
		//AddLogText("���湤��[%s]���ݳɹ�!", szGuid);
		if(NULL != pFac)
			pFac->m_dwDataChanged = eFDCF_Not;
	}
	else
	{
		AddLogText("���湤��[%s]����ʧ�ܣ����±��濪ʼ!", szGuid);
	}
	return true;
}
bool CWorldServerSession::ProcessSaveUnion(long retFlag,CEntityGroup* pBaseEntity)
{
	if(NULL == pBaseEntity)
	{
		assert(false);
		return false;
	}

	char szGuid[128];
	pBaseEntity->GetGUID().tostring(szGuid);
	CUnion *pUnion = GetOrganiCtrl()->GetUnionOrganizing(pBaseEntity->GetGUID());

	if(retFlag == S_OK)
	{
		if(NULL != pUnion)
			pUnion->m_dwDataChanged = eFDCF_Not;
	}
	else
	{
		AddLogText("��������[%s]����ʧ�ܣ����±��濪ʼ!", szGuid);
	}
	return true;
}

bool CWorldServerSession::ProcessInsertFaction(long retFlag,CEntityGroup* pBaseEntity)
{
	if(NULL == pBaseEntity)
	{
		assert(false);
		return false;
	}
	char szGuid[128] = {0};
	pBaseEntity->GetGUID().tostring(szGuid);
	CFaction *pFac = GetOrganiCtrl()->GetFactionOrganizing(pBaseEntity->GetGUID());

	if(retFlag == S_OK)
	{
		if(NULL != pFac)
			pFac->m_dwDataChanged = eFDCF_Not;
	}
	else
	{
		AddLogText("���湤��[%s]����ʧ�ܣ����±��濪ʼ!", szGuid);
	}
	return true;
}
bool CWorldServerSession::ProcessInsertUnion(long retFlag,CEntityGroup* pBaseEntity)
{
	if(NULL == pBaseEntity)
	{
		assert(false);
		return false;
	}
	char szGuid[128] = {0};
	pBaseEntity->GetGUID().tostring(szGuid);
	CUnion *pUnion = GetOrganiCtrl()->GetUnionOrganizing(pBaseEntity->GetGUID());

	if(retFlag == S_OK)
	{
		if(NULL != pUnion)
			pUnion->m_dwDataChanged = eFDCF_Not;
	}
	else
	{
		AddLogText("��������[%s]����ʧ�ܣ����±��濪ʼ!", szGuid);
	}
	return true;
}

bool CWorldServerSession::ProcessDelFaction(long retFlag,CEntityGroup* pBaseEntity)
{
	if(NULL == pBaseEntity)
	{
		assert(false);
		return false;
	}
		
	CGUID AimGuid;
	GetTempDataVec()[0]->GetGuidAttr( AimGuid);

	char szGuid[128] = {0};
	AimGuid.tostring(szGuid);

	CEntityGroup* pProcedure = (CEntityGroup*)pBaseEntity;
	
	if(S_OK == retFlag)
	{
		
	}
	else
	{
		AddLogText("ɾ������[%s]����ʧ�ܣ�����ִ��ɾ������������", szGuid);
	}

	return true;
}
bool CWorldServerSession::ProcessDelUnion(long retFlag,CEntityGroup* pBaseEntity)
{
	if(NULL == pBaseEntity)
	{
		assert(false);
		return false;
	}

	CGUID AimGuid;
	GetTempDataVec()[0]->GetGuidAttr( AimGuid);

	char szGuid[128] = {0};
	AimGuid.tostring(szGuid);

	CEntityGroup* pProcedure = (CEntityGroup*)pBaseEntity;

	if(S_OK == retFlag)
	{
		GetOrganiCtrl()->OnDelUnionFinishMsg(AimGuid);
	}
	else
	{
		AddLogText("ɾ������[%s]����ʧ�ܣ�����ִ��ɾ������������", szGuid);
	}

	return true;
}

bool CWorldServerSession::ProcessInsertOnlineMail(long retFlag, CEntityGroup* pBaseEntity)
{
	int nRet = 0;
	if(GetTempDataVec().size() != 1)
		return false;

	CGUID mailID;
	if(GetTempDataVec()[0])
		GetTempDataVec()[0]->GetGuidAttr( mailID);

	long lRet = -3;
	if(pBaseEntity->GetRetQueryDataVec().size() == 1)
		lRet = pBaseEntity->GetRetQueryDataVec()[0]->GetLongAttr();

	if (retFlag!=S_OK && retFlag!=0x80040E14/*�޷�������ݿ�����Դ*/)
	{
		//�����ʼ�ʧ��
		GetGame()->GetMailManager()->FailedSaveMail(mailID);
		return true;
	}

	GetGame()->GetMailManager()->SendSaveMailRet(mailID,lRet);

	// ����ϴδ�����ʼ�����
	if(pBaseEntity)
		pBaseEntity->ReleaseChilds();
	return true;
}
bool CWorldServerSession::ProcessDeleteOnlineMail(long retFlag, CEntityGroup* pBaseEntity)
{
	if(pBaseEntity)
	{
		// ����ϴδ�����ʼ�����
		pBaseEntity->ReleaseChilds();
	}
	return true;
}
bool CWorldServerSession::ProcessSaveOnlineMail(long retFlag, CEntityGroup* pBaseEntity)
{
	if(pBaseEntity)
	{
		// ����ϴδ�����ʼ�����
		pBaseEntity->ReleaseChilds();
	}
	return true;
}
bool CWorldServerSession::ProcessLoadPlayerMails(long retFlag, CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{	
		list<CMail*> lMail;
		string strReceiver;

		CGUID tmpGuid;
		GetTempDataVec()[0]->GetGuidAttr( tmpGuid);
		// do something...
		CEntityGroup* mailGroup = (CEntityGroup*)pBaseEntity;

			map<CGUID, CBaseEntity*>::iterator mailItr = mailGroup->GetEntityGroupMap().begin();
		
			for(; mailItr != mailGroup->GetEntityGroupMap().end(); mailItr++)
			{
				CMail *pMail = MP_NEW CMail();
				GetGame()->GetEntityManager()->CDBMailToCMail((CEntityGroup*)mailItr->second, pMail);
				if (pMail)
				{
					if (tmpGuid == pMail->GetReceiverExID())
					{
						lMail.push_back(pMail);
						strReceiver = pMail->GetReceiver();	
					}
					else
					{
						MP_DELETE(pMail);
#ifdef 	_OPERATE_MAIL

						char szOutPutStr[1024];						
						char strGuid[256];
						char strRGuid[128];
						tmpGuid.tostring(strGuid);
						pMail->GetReceiverExID().tostring(strRGuid);
						sprintf(szOutPutStr, "�����˵�GUID:%s ���ʼ�ӵ���ߵ�Guid:%s��ƥ��",strGuid,strRGuid);
						CMessage::AsyWriteFile(GetGame()->GetOpeateMailFileName(),szOutPutStr);
#endif
						continue;
					}
					
				}	
			}	

#ifdef 	_OPERATE_MAIL1
			char szOutPutStr[256];						
			if (strReceiver.size()>0)
			{
				sprintf(szOutPutStr, "���:%s ���մ�DB�������ʼ���Ϣ",strReceiver.c_str());
			}
			else
			{
				CPlayer *pPlayer = GetGame()->GetMapPlayer(tmpGuid);
				if (pPlayer)
				{
					sprintf(szOutPutStr, "���:%s��DB�������ʼ�����Ϊ0",pPlayer->GetName());
				}
				
			}
			
			CMessage::AsyWriteFile(GetGame()->GetOpeateMailFileName(),szOutPutStr);			
#endif	
		GetGame()->GetMailManager()->AddPlayerMail(tmpGuid,lMail);
	
		// ����ϴδ�����ʼ�����
		pBaseEntity->ReleaseChilds();
	}
	else
	{
#ifdef 	_OPERATE_MAIL1
		char szOutPutStr[1024];						
		sprintf(szOutPutStr, "��DB��ȡ�ʼ�ʧ��");
		CMessage::AsyWriteFile(GetGame()->GetOpeateMailFileName(),szOutPutStr);
#endif
	}

	return true;
}
bool CWorldServerSession::ProcessDeleteOnlineMails(long retFlag, CEntityGroup* pBaseEntity)
{
	if(pBaseEntity && retFlag == S_OK)
	{
		//  record delete mails guid
		list<CGUID> lGuid;
		CGUID playerGuid;		

		for(int i=0; i<GetTempDataVec().size(); i++)
		{		
			CGUID tmpGuid;
			if (i==0)
			{
				GetTempDataVec()[0]->GetGuidAttr(playerGuid);
				continue;
			}			
			GetTempDataVec()[i]->GetGuidAttr( tmpGuid);
			lGuid.push_back(tmpGuid);
		}

		// ����ϴδ�����ʼ�����
		pBaseEntity->ReleaseChilds();
	}
	return true;

}

// ����BanPlayer
bool CWorldServerSession::ProcessBanPlayer(long retFlag, CEntityGroup* pBaseEntity)
{
	if(GetTempDataVec().size() != 2) return false;
	const char* name = NULL;
	bool bBanSucc=false;
	if(GetTempDataVec()[1])
	{
		name = GetTempDataVec()[1]->GetStringAttr();
	}
	if(pBaseEntity && retFlag == S_OK)
	{
		if(pBaseEntity->GetRetQueryDataVec().size() != 1) return false;

		long time = 0;
		const char* szCdkey = NULL;
		if(GetTempDataVec()[0])
			time = GetTempDataVec()[0]->GetLongAttr();
		bBanSucc=true;
		if(pBaseEntity->GetRetQueryDataVec()[0])
			szCdkey = pBaseEntity->GetRetQueryDataVec()[0]->GetStringAttr();

		CMessage msg(MSG_W2L_GM_BANPLAYER);
		msg.Add(szCdkey);
		msg.Add(time);
		msg.SendToSocket(GetGame()->GetLSSocketID());
		AddLogText("DB2W:ban player %s OK!", szCdkey);
	}
	if (name!=NULL)
	{
		CSMClient::GetSM()->OnProcessBanPlayer(name,bBanSucc);
	}
	return true;
}
// ����ɾ������ʼ��Ĵ洢����
void CWorldServerSession::SendDelRoleMailProcMsg(const char* szName)
{
	if(szName)
	{
		CMessage msg(MSG_W2DB_OPER_ENTITY);

		CGUID ProcGuid;
		CGUID::CreateGUID(ProcGuid);
		CEntityGroup* pProcedure = NULL;
		// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
		pProcedure = (CEntityGroup*)(GetGame()->GetEntityManager()->CreateRootEntity(string("[procedure]"), ProcGuid));
		if(!pProcedure) return;

		msg.Add(GetExID());
		msg.Add("[procedure]");
		msg.Add(ProcGuid);
		msg.Add("");
		msg.Add((BYTE)0);

		vector<BYTE> pBA;
		if(pProcedure)
		{
			pProcedure->QueryDataRelease();
			pProcedure->RetQueryDataRelease();

			// ��Ӳ�ѯ����
			CTempEntityProperty* pName = new CTempEntityProperty( string("RecieverName"), DATA_OBJECT_STRING);
			if(pName)
			{
				pName->SetStringAttr(string(szName));
				pName->SetDbRelateType(DB_RELATE_NULL);
				pProcedure->GetQueryDataVec().push_back(pName);
			}
			CTempEntityProperty* pBuf = new CTempEntityProperty( string("IncGoodsBuf"), DATA_OBJECT_BUFFER);
			if(pBuf)
			{
				long lBuf = 0;
				pBuf->SetBufAttr(&lBuf, sizeof(long));
				pBuf->SetDbRelateType(DB_RELATE_NULL);
				pProcedure->GetQueryDataVec().push_back(pBuf);
			}

			// ���ִ�д洢������
			pProcedure->SetStringAttr(string("ProcName"),  "DelRoleMailProc");
			pProcedure->SetCurDbOperType(DB_OPER_PROCEDURE);
			pProcedure->SetCurDbOperFlag(1);
			pProcedure->AddToByteArray(pBA);
		}
		msg.Add((long)pBA.size());
		if(pBA.size())
			msg.AddEx(&pBA[0], (long)pBA.size());
		msg.SendToSocket(GetGame()->GetDBSocketID());
	}
}

bool CWorldServerSession::ProcessSaveGlobalVarMsg(long retFlag, CEntityGroup* pBaseEntity)
{
	if(retFlag == S_OK && pBaseEntity)
	{
		return true;
	}
	return false;
}
bool CWorldServerSession::ProcessLoadPhRgnGroupMsg(long retFlag, CEntityGroup* pBaseEntity)
{
	if(retFlag == S_OK && pBaseEntity)
	{
		GetGame()->SetInitPersonalHouseLoadFlag(true);
		return true;
	}
	return false;
}
void CWorldServerSession::SendChangeNameProc(const CGUID& guid, const char* szName, long gsscoketid)
{
	if(guid == NULL_GUID || !szName) return;
	
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	// ת����DBServer,DBServer�յ�����Ϣʱ����WS����LOGINPLAYER������
	CEntityGroup* pProcedure = NULL;
	CGUID ProcGuid;
	CGUID::CreateGUID(ProcGuid);
	pProcedure = (CEntityGroup*)(GetGame()->GetEntityManager()->CreateRootEntity(string("[procedure]"), ProcGuid));
	if(!pProcedure) return;

	msg.Add(GetExID());
	msg.Add("[procedure]");
	msg.Add(ProcGuid);
	msg.Add("");
	msg.Add((BYTE)0);

	vector<BYTE> pBA;
	if(pProcedure)
	{
		pProcedure->QueryDataRelease();
		pProcedure->RetQueryDataRelease();
		pProcedure->ReleaseChilds();

		// ��¼��ʱ����
		CTempEntityProperty* pgssocketid = new CTempEntityProperty( string("gssocketid"), DATA_OBJECT_LONG);
		if(pgssocketid)
		{
			pgssocketid->SetLongAttr(gsscoketid);
			GetTempDataVec().push_back(pgssocketid);
		}

		CTempEntityProperty* ptmpid = new CTempEntityProperty( string("tmpid"), DATA_OBJECT_GUID);
		if(ptmpid)
		{
			ptmpid->SetGuidAttr(guid);
			GetTempDataVec().push_back(ptmpid);
		}

		CTempEntityProperty* ptmpname = new CTempEntityProperty( string("tmpname"), DATA_OBJECT_STRING);
		if(ptmpname)
		{
			ptmpname->SetStringAttr(string(szName));
			GetTempDataVec().push_back(ptmpname);
		}

		// ��Ӳ�ѯ����
		CTempEntityProperty* pGuid = new CTempEntityProperty( string("playerid"), DATA_OBJECT_GUID);
		if(pGuid)
		{
			pGuid->SetGuidAttr(guid);
			pProcedure->GetQueryDataVec().push_back(pGuid);
		}

		CTempEntityProperty* pName = new CTempEntityProperty( string("playername"), DATA_OBJECT_STRING);
		if(pName)
		{
			pName->SetStringAttr(string(szName));
			pProcedure->GetQueryDataVec().push_back(pName);
		}
		
		// ��Ӳ�ѯ���ؽ��
		CTempEntityProperty* pRet0 = new CTempEntityProperty( string("lret"), DATA_OBJECT_LONG);
		if(pRet0)
		{
			pRet0->SetLongAttr(0);
			pProcedure->GetRetQueryDataVec().push_back(pRet0);
		}
	
		// ���ִ�д洢������
		pProcedure->SetStringAttr(string("ProcName"),  "ChangePlayerNameProc");

		pProcedure->SetCurDbOperType(DB_OPER_CHANGENAME_PROCEDURE);
		pProcedure->SetCurDbOperFlag(1);
		pProcedure->AddToByteArray(pBA);
	}

	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToSocket(GetGame()->GetDBSocketID());
}
void CWorldServerSession::ProcessChangeNameProcMsg(long retFlag, CEntityGroup* pEntity)
{
	if(!pEntity) return;
	if(GetTempDataVec().size() != 3) return;

	CMessage msg(MSG_W2S_OTHER_CHANGENAME);

	long gsscoketid = 0;
	char* szChangeName = NULL;
	CGUID guid;

	if(GetTempDataVec()[0])
		gsscoketid = GetTempDataVec()[0]->GetLongAttr();
	
	if(GetTempDataVec()[1])
		GetTempDataVec()[1]->GetGuidAttr(guid);

	if(GetTempDataVec()[2])
		szChangeName = (char*)GetTempDataVec()[2]->GetStringAttr();

	msg.Add(guid);

	if(szChangeName[0] != '\0')
		msg.Add(szChangeName);
	else
		msg.Add("");

	if(retFlag==S_OK)
	{
		if(pEntity->GetRetQueryDataVec().size() == 1 && pEntity->GetRetQueryDataVec()[0])
		{
			if(pEntity->GetRetQueryDataVec()[0]->GetLongAttr() == 1) // �ɹ�
			{
				CPlayer* pPlayer = GetGame()->GetMapPlayer(guid);
	
				if(pPlayer)
					pPlayer->SetName(szChangeName);
				else
				{
					char szGuid[128];
					pEntity->GetGUID().tostring(szGuid);
					AddLogText("ProcessChangeNameProcMsg() δ�ҵ����%s!", szGuid);
				}

				msg.Add((BYTE)1); // �޸������Ƿ�ɹ���1���ɹ�
			}
			else
				msg.Add((BYTE)pEntity->GetRetQueryDataVec()[0]->GetLongAttr());
		}
		else
			msg.Add((BYTE)0);
	}
	else 
		msg.Add((BYTE)retFlag); // ʧ��

	msg.SendToSocket(gsscoketid);
}