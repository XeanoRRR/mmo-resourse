#include "stdafx.h"
#include "RgnManager.h"
#include "script/VariableList.h"
#include "../GameServer/GameManager.h"
#include "Player.h"
#include "Monster.h"
#include "script/VariableList.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"
#include "script/Script.h"
#include "Country/CountryHandler.h"
#include "Country/CountryParam.h"

using namespace std;

CRgnManager g_RgnManager;

CRgnManager::tagDupRgnData::tagDupRgnData(void)
: lBufSize(0), pData(NULL)
{
}
CRgnManager::tagDupRgnData::~tagDupRgnData(void)
{
	if(pData && lBufSize > 0)
		M_FREE(pData, sizeof(char)*lBufSize);
}

CRgnManager::CRgnManager()
:	m_CurQueueNum(0), m_QueueMaxNum(0),m_RegionTimer(1024, 1024),m_QueueTimer(1024, 1024),
m_CreatePRgnTimer(1024,1024),m_CreateTRgnTimer(1024,1024),m_RgnKickPlayerTimer(1024,1024),
m_CurNormalRgnNum(0), m_CurPersonalDupRgnNum(0), m_CurPersonalHouseDupRgnNum(0), m_CurTeamDupRgnNum(0)
{
}
CRgnManager::~CRgnManager()
{
	RgnByIDMapItr rgnItr = GetNormalRgnMap().begin();
	for(; rgnItr != GetNormalRgnMap().end(); rgnItr++)
	{
		MP_DELETE(rgnItr->second);
	}
	GetNormalRgnMap().clear();

	rgnItr = GetPersonalDupRgnMap().begin();
	for(; rgnItr != GetPersonalDupRgnMap().end(); rgnItr++)
	{
		MP_DELETE(rgnItr->second);
	}
	GetPersonalDupRgnMap().clear();

	rgnItr = GetPersonalHouseDupRgnMap().begin();
	for(; rgnItr != GetPersonalHouseDupRgnMap().end(); rgnItr++)
	{
		MP_DELETE(rgnItr->second);
	}
	GetPersonalHouseDupRgnMap().clear();

	rgnItr = GetTeamDupRgnMap().begin();
	for(; rgnItr != GetTeamDupRgnMap().end(); rgnItr++)
	{
		MP_DELETE(rgnItr->second);
	}
	GetTeamDupRgnMap().clear();

	DelAllDupRgnData();
}
//! ��װ���߳�������
bool CRgnManager::AddLinedRgnInfoToByteArray(long linedId, DBWriteSet& setWriteDB)
{
	map<long, vector<long>>::iterator itr = m_mapLinedRgnIdByLinedId.find(linedId);
	if( itr != m_mapLinedRgnIdByLinedId.end() )
	{
		setWriteDB.AddToByteArray((BYTE)1);
		// line id
		setWriteDB.AddToByteArray(itr->first);
		// line num
		setWriteDB.AddToByteArray((long)itr->second.size());
		vector<long>::iterator templateIdItr = itr->second.begin();
		for(; templateIdItr != itr->second.end(); templateIdItr++)
		{
			// template id
			setWriteDB.AddToByteArray( (*templateIdItr));
			// rgn guid
			CServerRegion* pRgn = FindRgnByTemplateID(RGN_NORMAL, *templateIdItr);
			if( pRgn  )
				setWriteDB.AddToByteArray(pRgn->GetExID());
			else
				setWriteDB.AddToByteArray(NULL_GUID);
		}	
	}
	else
		setWriteDB.AddToByteArray((BYTE)0);

	return true;
}
//! ������߳�������
bool CRgnManager::DecodeLinedRgnInfoFromByteArray(BYTE* buf, long& pos, long bufSize)
{
	m_mapLinedRgnIdByLinedId.clear();

	long linedNum = _GetLongFromByteArray(buf, pos, bufSize);
	for(int i=0; i<linedNum; i++)
	{
		vector<long> linedRgnInfo;
		long linedId = _GetLongFromByteArray(buf, pos, bufSize);
		long linedRgnNum = _GetLongFromByteArray(buf, pos, bufSize);
		for(int i=0; i<linedRgnNum; i++)
		{
			long templateId = _GetLongFromByteArray(buf, pos, bufSize);
			CGUID rgnID;
			_GetBufferFromByteArray(buf, pos, rgnID, bufSize);
			linedRgnInfo.push_back(templateId);
		}
		m_mapLinedRgnIdByLinedId[linedId] = linedRgnInfo;
	}
	return true;
}
CServerRegion* CRgnManager::AddNormalRgn(const CGUID& rgnID, long templateRgnID)
{
	CServerRegion* ret = NULL;
	RgnByIDMapItr itr = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().find(rgnID);
	if ( itr == GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end() ) // δ�ҵ�
	{
		ret = ret = MP_NEW CServerRegion(rgnID);
		ret->SetExID(rgnID);
		m_mapNormalRgn[rgnID] = ret;
	}

	return ret;
}
void CRgnManager::DelNormalRgn(const CGUID& rgnID, long templateRgnID)
{
	RgnByIDMapItr itr = m_mapNormalRgn.find(rgnID);
	if(itr != m_mapNormalRgn.end())
	{
		MP_DELETE(itr->second);
		m_mapNormalRgn.erase(itr);
	}
}
CServerRegion* CRgnManager::AddCreatedDupRgn(const CGUID& rgnID, eRgnType type)
{
	CServerRegion* ret = NULL;
	RgnByIDMapItr itr;
	switch(type)
	{
	case RGN_PERSONAL:
		{
			itr = m_mapPersonalDupRgn.find(rgnID);
			if ( itr == m_mapPersonalDupRgn.end() ) // δ�ҵ�
			{
				ret = MP_NEW CServerRegion(rgnID);
				ret->SetExID(rgnID);
				ret->SetRgnType((long)RGN_PERSONAL);
				m_mapPersonalDupRgn[rgnID] = ret;
			}
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			itr = m_mapPersonalHouseDupRgn.find(rgnID);
			if ( itr == m_mapPersonalHouseDupRgn.end() ) // δ�ҵ�
			{
				ret = ret = MP_NEW CServerRegion(rgnID);
				ret->SetExID(rgnID);
				ret->SetRgnType((long)RGN_PERSONAL_HOUSE);
				m_mapPersonalHouseDupRgn[rgnID] = ret;
			}
		}
		break;
	case RGN_TEAM:
		{
			itr = m_mapTeamDupRgn.find(rgnID);
			if ( itr == m_mapTeamDupRgn.end() ) // δ�ҵ�
			{
				ret = ret = MP_NEW CServerRegion(rgnID);
				ret->SetExID(rgnID);
				ret->SetRgnType((long)RGN_TEAM);
				m_mapTeamDupRgn[rgnID] = ret;
			}
		}
		break;
	}
	return ret;
}
void CRgnManager::DelCreatedDupRgn(const CGUID& rgnID, eRgnType type)
{
	RgnByIDMapItr itr;
	switch(type)
	{
	case RGN_PERSONAL:
		{
			itr = m_mapPersonalDupRgn.find(rgnID);
			if(itr != m_mapPersonalDupRgn.end())
			{
				MP_DELETE(itr->second);
				m_mapPersonalDupRgn.erase(itr);
			}
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			itr = m_mapPersonalHouseDupRgn.find(rgnID);
			if(itr != m_mapPersonalHouseDupRgn.end())
			{
				MP_DELETE(itr->second);
				m_mapPersonalHouseDupRgn.erase(itr);
			}
		}
		break;
	case RGN_TEAM:
		{
			itr = m_mapTeamDupRgn.find(rgnID);
			if(itr != m_mapTeamDupRgn.end())
			{
				MP_DELETE(itr->second);
				m_mapTeamDupRgn.erase(itr);
			}
		}
		break;
	}
}

//! ����ʱ,�ȹ��ܵĽӿ�
void CRgnManager::Run(void)
{

}
//! ����Rgn,��WS����RGN_CREATE��Ϣ
long CRgnManager::CreateRgnMsg(CScript* pScript/*����Ľű�*/, long templateRgnID, eRgnType rgnType, const CGUID& ownerID, const char* szRetVarName/*�첽����ֵ������*/, const char* szRetRgnIDName/*�첽����ֵ������*/)
{
	if(!pScript) return -99999999;

	switch(rgnType)
	{
	case RGN_PERSONAL:
		{
			CPlayer* player = GetGame()->FindPlayer(ownerID);
			if(player == NULL) return 0;
			if(player->GetCurPRgnNum() >= CGlobeSetup::GetSetup()->lCreatePDupRgnMaxNum)
			{
				//���÷���ֵ
				pScript->SetScriptVarValue((CMoveShape*)pScript->GetSrcShape(),szRetVarName, 0, -4.0f);
				
				char outstr[1024];
				char szGUID[128];
				player->GetExID().tostring(szGUID);
				_snprintf(outstr, 1024, CStringReading::LoadString(IDS_GS_RGNMANAGER, STR_GS_RGNMANAGER_MAXCREATEPRGNNUM), CGlobeSetup::GetSetup()->lCreatePDupRgnMaxNum, szGUID);
				AddLogText(outstr);
				return -4.0f;
			}
		}
		break;
	case RGN_TEAM:
		{
			CPlayer* player = GetGame()->FindPlayer(ownerID);
			if(player == NULL) return 0;
			if(player->GetCurTRgnNum() >= CGlobeSetup::GetSetup()->lCreateTDupRgnMaxNum)
			{
				//���÷���ֵ
				pScript->SetScriptVarValue((CMoveShape*)pScript->GetSrcShape(),szRetVarName, 0, -4.0f);

				char outstr[1024];
				char szGUID[128];
				player->GetExID().tostring(szGUID);
				_snprintf(outstr, 1024, CStringReading::LoadString(IDS_GS_RGNMANAGER, STR_GS_RGNMANAGER_MAXCREATETRGNNUM), CGlobeSetup::GetSetup()->lCreateTDupRgnMaxNum, szGUID);
				AddLogText(outstr);
				return -4.0f;
			}
		}
		break;
	}

	long isDupRgnDataFlag = FindDupRgnData(templateRgnID);

	CMessage msg(MSG_S2W_RGN_CREATE);

	msg.Add(templateRgnID); // ģ��Rgn��ID
	msg.Add((long)rgnType); // ģ��Rgn��Type
	msg.Add(ownerID);		// �����ߵ�GUID
	msg.Add(pScript->GetID());	// ����Ľű�ID
	if(szRetVarName)
		msg.Add((char*)szRetVarName);  // �첽��������ֵ������
	else
		msg.Add((char*)"");
	if(szRetRgnIDName)
		msg.Add((char*)szRetRgnIDName);  // �첽��������ֵ������
	else
		msg.Add((char*)"");

	msg.Add((BYTE)(!isDupRgnDataFlag?0:1)); // �ø����Ƿ��л�������
	msg.Send();

	return 1.0f;
}

//! ɾ��Rgn,��WS����RGN_DELETE��Ϣ
long CRgnManager::DeleteRgnMsg(const CGUID& runScriptID/*����Ľű�ID*/, eRgnType rgnType, const CGUID& ownerID, const CGUID& rgnID, const char* szRetVarName/*�첽����ֵ������*/, const char* szRetRgnIDName/*�첽����ֵ������*/)
{
	CServerRegion* rgn = FindRgnByRgnGUID(rgnType, rgnID);

	// ����ø�����������򷵻�
	if( rgn && (rgn->GetPlayerAmout() != 0) )
		return 0;

	CMessage msg(MSG_S2W_RGN_DELETE);
	msg.Add(ownerID);   // owner id
	msg.Add(rgnID);		// Region��GUID
	msg.Add((long)rgnType); // ģ��Rgn��Type
	msg.Add(runScriptID);/*����Ľű�ID*/
	if(szRetVarName)
		msg.Add((char*)szRetVarName);  // �첽��������ֵ������
	else
		msg.Add((char*)"");
	if(szRetRgnIDName)
		msg.Add((char*)szRetRgnIDName);  // �첽��������ֵ������
	else
		msg.Add((char*)"");
	msg.Send();

	return 1;
}
//! ����Rgn,��WS����RGN_CREATE��Ϣ
long CRgnManager::OnCreateRgnMsg(CMessage* pMsg)
{
	long templateRgnID = pMsg->GetLong(); // ģ��Rgn��ID
	eRgnType rgnType = (eRgnType)pMsg->GetLong(); // ģ��Rgn��Type
	CGUID ownerID;
	CGUID rgnID;
	CGUID scriptID;
	pMsg->GetGUID(ownerID);			// �����ߵ�GUID
	pMsg->GetGUID(rgnID);			// ��Region ID
	long flag = pMsg->GetLong();	// �Ѿ�������־
	long srcGSID = pMsg->GetLong();

	BYTE isDupRgnDataFlag = pMsg->GetByte(); // �Ƿ��л���������
	long lBufSize = pMsg->GetLong();

	if(flag < 0) // ����ʧ��
	{
		pMsg->GetGUID(scriptID);
		char szRetVarName[128];// �첽��������ֵ������
		pMsg->GetStr(szRetVarName, 128);
		char szRetRgnIDName[128];// �첽��������ֵ������
		pMsg->GetStr(szRetRgnIDName, 128);
		// ȡ������Ľű�
		CVariableList* stackVarList = CScript::GetStackVarListInst();
		if(stackVarList)
		{
			stackVarList->Release();
			stackVarList->AddVar(szRetVarName, flag);
			stackVarList->AddGuid(szRetRgnIDName, rgnID);
		}
		ContinueRunScript(scriptID, stackVarList);
		return -1;
	}

	BYTE* buf = NULL;
	if(!isDupRgnDataFlag && lBufSize > 0) // û�л���������
	{
		buf = (BYTE*)M_ALLOC(sizeof(BYTE)*lBufSize);
		pMsg->GetEx(buf, lBufSize);
		// �����ڴ�ת�Ƶ�map����
		if(!AddDupRgnData(templateRgnID, buf, lBufSize))
			M_FREE(buf, lBufSize);
	}
	else if(isDupRgnDataFlag)
	{
		buf = (BYTE*)GetDupRgnData(templateRgnID);
		if(NULL == buf)
		{
			pMsg->GetGUID(scriptID);
			char szRetVarName[128];// �첽��������ֵ������
			pMsg->GetStr(szRetVarName, 128);
			char szRetRgnIDName[128];// �첽��������ֵ������
			pMsg->GetStr(szRetRgnIDName, 128);

			// ȡ������Ľű�
			CVariableList* stackVarList = CScript::GetStackVarListInst();
			if(stackVarList)
			{
				stackVarList->Release();
				stackVarList->AddVar(szRetVarName, flag);
				stackVarList->AddGuid(szRetRgnIDName, rgnID);
			}
			ContinueRunScript(scriptID, stackVarList);

			CMessage msg(MSG_S2W_RGN_CREATE_QUEST);
			msg.Add(srcGSID); // srcGSID
			msg.Add(ownerID); // owner ID
			msg.Add((long)rgnType); // Rgn Type
			msg.Add((long)templateRgnID); // Rgn TemplateID
			msg.Add((long)templateRgnID); // Rgn ResourceID
			msg.Add(rgnID);// Rgn GUID
			msg.Add(scriptID);
			msg.Add(szRetVarName);
			msg.Add(szRetRgnIDName);
			msg.Add((long)C_RET_THESE_RGNS_FULL_ERROR);
			msg.SendToSocket(GetGame()->GetWSSocket());
			return -1;
		}
	}

	pMsg->GetGUID(scriptID);
	char szRetVarName[128];// �첽��������ֵ������
	pMsg->GetStr(szRetVarName, 128);
	char szRetRgnIDName[128];// �첽��������ֵ������
	pMsg->GetStr(szRetRgnIDName, 128);

	CPlayer* pPlayer = GetGame()->FindPlayer(ownerID);

	long lResourceId = 0;
	// ��ǰ��������
	CServerRegion* rgn = NULL;
	switch(rgnType)
	{
	case RGN_PERSONAL:
		{	
			if(pPlayer && pPlayer->GetCurPRgnNum() >= CGlobeSetup::GetSetup()->lCreatePDupRgnMaxNum)
			{	
				CMessage msg(MSG_S2W_RGN_CREATE_QUEST);
				msg.Add(srcGSID); // srcGSID
				msg.Add(ownerID); // owner ID
				msg.Add((long)rgnType); // Rgn Type
				msg.Add((long)templateRgnID); // Rgn TemplateID
				msg.Add((long)templateRgnID); // Rgn ResourceID
				msg.Add(rgnID);// Rgn GUID
				msg.Add(scriptID);
				msg.Add(szRetVarName);
				msg.Add(szRetRgnIDName);
				msg.Add((long)C_RET_THESE_RGNS_FULL_ERROR);
				msg.SendToSocket(GetGame()->GetWSSocket());
				return -1;
			}
			
			RgnByIDMapItr itr = m_mapPersonalDupRgn.find(rgnID);
			if(itr == m_mapPersonalDupRgn.end())
			{
				rgn = AddCreatedDupRgn(rgnID, RGN_PERSONAL);
				rgn->SetRgnType((long)RGN_PERSONAL);
			}
			else
				rgn = itr->second;
			
			if(pPlayer)
				pPlayer->SetPersonalRgnGUID(rgnID);
			// ����PDupRgn����
			SetCurPersonalDupRgnNum(GetCurPersonalDupRgnNum()+1);
			AddPersonalRgnGUID(ownerID, rgnID);
			if(pPlayer)
			{
				if(pPlayer->GetCurPRgnNum() == 0) // �ǳ��δ���
					pPlayer->RegisterCreatePRgnTimerID(CGlobeSetup::GetSetup()->lCreateDupRgnDeltaTime);
				pPlayer->SetCurPRgnNum(pPlayer->GetCurPRgnNum()+1);
			}
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{		
			if(pPlayer && pPlayer->GetPersonalHouseRgnGUID() != NULL_GUID)
			{
				CMessage msg(MSG_S2W_RGN_CREATE_QUEST);
				msg.Add(srcGSID); // srcGSID
				msg.Add(ownerID); // owner ID
				msg.Add((long)rgnType); // Rgn Type
				msg.Add((long)templateRgnID); // Rgn TemplateID
				msg.Add((long)templateRgnID); // Rgn ResourceID
				msg.Add(rgnID);// Rgn GUID
				msg.Add(scriptID);
				msg.Add(szRetVarName);
				msg.Add(szRetRgnIDName);
				msg.Add((long)C_RET_THESE_RGNS_FULL_ERROR);
				msg.SendToSocket(GetGame()->GetWSSocket());
				return -1;
			}
			
			RgnByIDMapItr itr = m_mapPersonalHouseDupRgn.find(rgnID);
			if(itr == m_mapPersonalHouseDupRgn.end())
			{
				rgn = AddCreatedDupRgn(rgnID, RGN_PERSONAL_HOUSE);
				rgn->SetRgnType((long)RGN_PERSONAL_HOUSE);
			}
			else
				rgn = itr->second;
			
			if(pPlayer)
				pPlayer->SetPersonalHouseRgnGUID(rgnID);
		}
		break;
	case RGN_TEAM:
		{	
			if(pPlayer && pPlayer->GetCurTRgnNum() >= CGlobeSetup::GetSetup()->lCreateTDupRgnMaxNum)
			{	
				CMessage msg(MSG_S2W_RGN_CREATE_QUEST);
				msg.Add(srcGSID); // srcGSID
				msg.Add(ownerID); // owner ID
				msg.Add((long)rgnType); // Rgn Type
				msg.Add((long)templateRgnID); // Rgn TemplateID
				msg.Add((long)templateRgnID); // Rgn ResourceID
				msg.Add(rgnID);// Rgn GUID
				msg.Add(scriptID);
				msg.Add(szRetVarName);
				msg.Add(szRetRgnIDName);
				msg.Add((long)C_RET_THESE_RGNS_FULL_ERROR);
				msg.SendToSocket(GetGame()->GetWSSocket());
				return -1;
			}
			
			RgnByIDMapItr itr = m_mapTeamDupRgn.find(rgnID);
			if(itr == m_mapTeamDupRgn.end())
			{
				rgn = AddCreatedDupRgn(rgnID, RGN_TEAM);
				rgn->SetRgnType((long)RGN_TEAM);
			}
			else
				rgn = itr->second;

			if(pPlayer)
			{
				GameManager::GetInstance()->GetRgnManager()->AddTeamRgnGUID(pPlayer->GetExID(), templateRgnID, rgnID, TRUE);
				if(pPlayer->GetCurTRgnNum() == 0) // �ǳ��δ���
					pPlayer->RegisterCreateTRgnTimerID(CGlobeSetup::GetSetup()->lCreateDupRgnDeltaTime);
				pPlayer->SetCurTRgnNum(pPlayer->GetCurTRgnNum()+1);
			}
			// ����PDupRgn����
			SetCurTeamDupRgnNum(GetCurTeamDupRgnNum()+1);
		}
		break;
	}
	if(rgn)
	{
		rgn->RegisterDeleteRgnTimerID(CGlobeSetup::GetSetup()->lPDupRgnDelTime);
		long pos = 0;
		rgn->DecordFromByteArray(buf, pos);
		rgn->OnChangeWeather();
		rgn->RegisterWeatherTimer();
		rgn->SetExID(rgnID);
		rgn->SetOwnerGUID(ownerID);
		lResourceId = rgn->GetResourceID();
	}	

	if(flag == C_RET_REMOTE_SUCCESS) // �������������ϴ����ĸ���
	{
		CMessage msg(MSG_S2W_RGN_CREATE_QUEST);
		msg.Add(srcGSID); // srcGSID
		msg.Add(ownerID); // owner ID
		msg.Add((long)rgnType); // Rgn Type
		msg.Add((long)templateRgnID); // Rgn TemplateID
		msg.Add((long)lResourceId); // Rgn ResourceID
		msg.Add(rgnID);// Rgn GUID
		// ȡ������Ľű�
		msg.Add(scriptID);
		msg.Add(szRetVarName);
		msg.Add(szRetRgnIDName);
		msg.Add(flag);
		msg.Add((BYTE)(!isDupRgnDataFlag?0:1));
		if(isDupRgnDataFlag == 0)
		{
			msg.Add((long)lBufSize);
			if(lBufSize > 0)
				msg.AddEx(buf, lBufSize);
		}
		msg.SendToSocket(GetGame()->GetWSSocket());
	}

	// ���ͱ���PHRgn��Ϣ
	if(rgnType == RGN_PERSONAL_HOUSE)
		GetGame()->SendSavePHDupRgn(ownerID);

	// ȡ������Ľű�
	CVariableList* stackVarList = CScript::GetStackVarListInst();
	if(stackVarList)
	{
		stackVarList->Release();
		stackVarList->AddVar(szRetVarName, flag);
		stackVarList->AddGuid(szRetRgnIDName, rgnID);
	}
	ContinueRunScript(scriptID, stackVarList);
	return 1.0f;
}
	
long CRgnManager::OnDeleteRgnMsg(CMessage* pMsg)
{
	eRgnType rgnType = (eRgnType)pMsg->GetLong(); // ģ��Rgn��Type
	CGUID ownerID;
	pMsg->GetGUID(ownerID);
	CGUID rgnID;
	pMsg->GetGUID(rgnID);			// ��Region ID
	BYTE flag = pMsg->GetByte(); // flag

	//wq:2008-7-9,��Ҫ�޸�
	CGUID scriptID;
	pMsg->GetGUID(scriptID); // ����Ľű�ID

	CServerRegion* sRgn = NULL;
	switch(rgnType)
	{
	case RGN_PERSONAL:
		{
			RgnByIDMapItr itr = m_mapPersonalDupRgn.find(rgnID);
			if(itr != m_mapPersonalDupRgn.end())
			{
				// ɾ����ʱ��
				itr->second->CancelDeleteRgnTimer();

				// ����TDupRgn����
				SetCurPersonalDupRgnNum(GetCurPersonalDupRgnNum()-1);

				CPlayer* player = GetGame()->FindPlayer(itr->second->GetOwnerGUID());
				if(player)
				{
					if(rgnID == player->GetPersonalRgnGUID())
					{
						player->SetPersonalRgnGUID(NULL_GUID);
						DelPersonalRgnGUID(player->GetExID());
					}
					player->SetCurPRgnNum(player->GetCurPRgnNum()-1);
				}
				sRgn = itr->second;
				m_mapPersonalDupRgn.erase(itr);			
			}
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			RgnByIDMapItr itr = m_mapPersonalHouseDupRgn.find(rgnID);
			if(itr != m_mapPersonalHouseDupRgn.end())
			{
				// ����PHDupRgn����
				SetCurPersonalHouseDupRgnNum(GetCurPersonalDupRgnNum()-1);

				CPlayer* player = GetGame()->FindPlayer(ownerID);
				if(player)
				{
					if(rgnID == player->GetPersonalHouseRgnGUID())
						player->SetPersonalHouseRgnGUID(NULL_GUID);
					sRgn = itr->second;
					m_mapPersonalHouseDupRgn.erase(itr);
				}
			}
		}
		break;
	case RGN_TEAM:
		{
			RgnByIDMapItr itr = m_mapTeamDupRgn.find(rgnID);
			if(itr != m_mapTeamDupRgn.end())
			{
				// ɾ����ʱ��
				itr->second->CancelDeleteRgnTimer();
				// ����TDupRgn����
				SetCurTeamDupRgnNum(GetCurTeamDupRgnNum()-1);

				CPlayer* player = GetGame()->FindPlayer(ownerID);
				if(player)
				{
					GameManager::GetInstance()->GetRgnManager()->DelTeamRgnGUID(player->GetExID(), rgnID);
					player->SetCurTRgnNum(player->GetCurTRgnNum()-1);
				}
				sRgn = itr->second;
				m_mapTeamDupRgn.erase(itr);
			}
		}
		break;
	case RGN_NORMAL:
		break;
	}

	char szRetVarName[128];// �첽��������ֵ������
	pMsg->GetStr(szRetVarName, 128);

	char szRetRgnIDName[128];// �첽��������ֵ������
	pMsg->GetStr(szRetRgnIDName, 128);

	// Ϊ�ýű�����һ�����첽�����ķ���ֵ
	CVariableList* stackVarList = CScript::GetStackVarListInst();
	if(stackVarList)
	{
		stackVarList->Release();
		stackVarList->AddVar(szRetVarName, (double)flag);
		stackVarList->AddGuid(szRetRgnIDName, NULL_GUID);
	}
	ContinueRunScript(scriptID, stackVarList);
	
	// ��rgn�ڵ��������ת�Ƶ�Ĭ�ϳ���
	if(sRgn)
	{
		CGUID retGuid = NULL_GUID;
		long lRegion=0, l=0, t=0, r=0, b=0, d=0;
		hashsetGUID tmpPlayerSet;
		sRgn->FindAllPlayer(tmpPlayerSet);
		hashsetGUID::iterator itr = tmpPlayerSet.begin();
		for(; itr != tmpPlayerSet.end(); itr++)
		{
			CPlayer* player = GetGame()->FindPlayer((*itr));
			if(!player) continue;

			sRgn->GetDupRgnReturnPoint(player, lRegion, l, t, r, b, d);
			if(lRegion  == 0) lRegion = -1; // ���û��������Ĭ�Ϸ��ع���
			if(lRegion == -1)
			{
				BYTE byCountry = player->GetCountry();
				lRegion = GetCountryParam()->GetCountryMainRegionID(byCountry);
				l = GetCountryParam()->GetCountryMainRectLeft(byCountry);
				t = GetCountryParam()->GetCountryMainRectTop(byCountry);
				r = GetCountryParam()->GetCountryMainRectRight(byCountry);
				b = GetCountryParam()->GetCountryMainRectBottom(byCountry);
			}
			CServerRegion* retRgn = FindRgnByTemplateID(RGN_NORMAL, lRegion);
			if(retRgn)
				retGuid = retRgn->GetExID();
			player->ChangeRegion(RGN_NORMAL, retGuid, l, t, 0);
		}
		// ��ӵ��ӳ�ɾ���б�
		GetGame()->PushDelDupRgn(sRgn);
	}

	return (long)flag;
}

//! ����Rgn
CServerRegion* CRgnManager::FindRgnByRgnGUID(eRgnType rgnType, const CGUID& rgnID)
{
	CServerRegion* ret = NULL;
	switch(rgnType)
	{
	case RGN_PERSONAL:
		{
			RgnByIDMapItr rgnItr = m_mapPersonalDupRgn.find(rgnID);
			if(rgnItr != m_mapPersonalDupRgn.end())
				ret = rgnItr->second;
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			RgnByIDMapItr rgnItr = m_mapPersonalHouseDupRgn.find(rgnID);
			if(rgnItr != m_mapPersonalHouseDupRgn.end())
				ret = rgnItr->second;
		}
		break;
	case RGN_TEAM:
		{
			RgnByIDMapItr rgnItr = m_mapTeamDupRgn.find(rgnID);
			if(rgnItr != m_mapTeamDupRgn.end())
				ret = rgnItr->second;	
		}
		break;
	case RGN_NORMAL:
		{
			RgnByIDMapItr rgnItr = m_mapNormalRgn.find(rgnID);
			if(rgnItr != m_mapNormalRgn.end())
				ret = rgnItr->second;
		}
		break;
	case RGN_ALL:
		{
			RgnByIDMapItr rgnItr = m_mapPersonalDupRgn.find(rgnID);
			if(rgnItr != m_mapPersonalDupRgn.end())
				ret = rgnItr->second;
			else if( (rgnItr = m_mapPersonalDupRgn.find(rgnID)) != m_mapPersonalDupRgn.end() )
				ret = rgnItr->second;
			else if( (rgnItr = m_mapTeamDupRgn.find(rgnID)) != m_mapTeamDupRgn.end() )
				ret = rgnItr->second;
			else if( (rgnItr = m_mapNormalRgn.find(rgnID)) != m_mapNormalRgn.end() )
				ret = rgnItr->second;
		}
		break;
	}

	return ret;
}
//! ��������region
CServerRegion* CRgnManager::FindRgnByRgnGUID(const CGUID& rgnID)
{
	CServerRegion* pCerverRng=0;
	pCerverRng=FindRgnByRgnGUID(RGN_NORMAL,rgnID);
	if (pCerverRng!=NULL)
	{
		return pCerverRng;
	}

	pCerverRng=FindRgnByRgnGUID(RGN_PERSONAL,rgnID);
	if (pCerverRng!=NULL)
	{
		return pCerverRng;
	}

	pCerverRng=FindRgnByRgnGUID(RGN_PERSONAL_HOUSE,rgnID);
	if (pCerverRng!=NULL)
	{
		return pCerverRng;
	}

	pCerverRng=FindRgnByRgnGUID(RGN_TEAM,rgnID);
	if (pCerverRng!=NULL)
	{
		return pCerverRng;
	}

	return 0;
}
//! ��������region
CServerRegion* CRgnManager::FindRgnByTemplateID(eRgnType rgnType, long templateID)
{
	switch(rgnType)
	{
	case RGN_NORMAL:
		{
			RgnByIDMapItr itr = m_mapNormalRgn.begin();
			for(; itr != m_mapNormalRgn.end(); itr++)
			{
				if(itr->second->GetID() == templateID)
					return itr->second;
			}
		}
		break;
	case RGN_PERSONAL:
		{
			RgnByIDMapItr itr = m_mapPersonalDupRgn.begin();
			for(; itr != m_mapPersonalDupRgn.end(); itr++)
			{
				if(itr->second->GetID() == templateID)
					return itr->second;
			}
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			RgnByIDMapItr itr = m_mapPersonalHouseDupRgn.begin();
			for(; itr != m_mapPersonalHouseDupRgn.end(); itr++)
			{
				if(itr->second->GetID() == templateID)
					return itr->second;
			}
		}
		break;
	case RGN_TEAM:
		{
			RgnByIDMapItr itr = m_mapTeamDupRgn.begin();
			for(; itr != m_mapTeamDupRgn.end(); itr++)
			{
				if(itr->second->GetID() == templateID)
					return itr->second;
			}
		}
		break;
	}
	return 0;
}


// �������˷��ݸ�����Ʒ(Monster)
void CRgnManager::CreatePHGoods(const CGUID& rgnID, const char* name, long nCounts, long posX, long posY,
				   long width, long height, long dir, const char* strScript, const char* strTalk)
{
	RgnByIDMapItr rgnItr = m_mapPersonalHouseDupRgn.find(rgnID);
	if(rgnItr != m_mapPersonalHouseDupRgn.end())
	{
		CServerRegion* pRegion	= rgnItr->second;

		if( pRegion )
		{
			CMonster* pLeader = pRegion -> AddMonster( name, posX, posY, -1 );
			if( pLeader == NULL ) 
			{
				return;
			}

			// ��ӽű�
			if (strScript && strcmp(strScript,"0")!=0)
			{
				pLeader->SetScriptFile(strScript);
			}
			if (strTalk && strcmp(strTalk,"0")!=0)
			{
				pLeader->SetTalkScriptFile(strTalk);
			}

			CGUID ret = pLeader->GetExID();

			for (int i=0; i<nCounts-1; i++)
			{
				pLeader = pRegion -> AddMonster( name, posX, posY, -1 );
				if (pLeader)
				{
					if (strScript)
					{
						pLeader->SetScriptFile(strScript);
					}
					if (strTalk)
					{
						pLeader->SetTalkScriptFile(strTalk);
					}
				}
			}
		
			// ����PHGoods����
			vecPHGoods& tVecGoods = m_PHGoodsMap[rgnItr->second->GetExID()];
			vecPHGoodsItr vecGoodsItr = tVecGoods.begin();
			// �����Ƿ����иö���
			for(; vecGoodsItr != tVecGoods.end(); vecGoodsItr++)
			{
				if(	 ret != NULL_GUID &&
					(*vecGoodsItr).guid == ret	&&
					strcmp((*vecGoodsItr).name, name) == 0 &&
					(*vecGoodsItr).dir == dir &&
					(*vecGoodsItr).width == width &&
					(*vecGoodsItr).height == height &&
					(*vecGoodsItr).posX == posX &&
					(*vecGoodsItr).posY == posY) // ��ͬһ��GOODS
					break;
			}

			//##������Ϣ��World Server�����ͺ󲻹�
			CMessage msg( MSG_S2W_SERVER_CREATEPHGOODS );
			msg.Add( (BYTE)1 );

			if(vecGoodsItr == tVecGoods.end()) // δ�ҵ�
			{
				PHGoods tGoods;
				CGUID::CreateGUID(tGoods.guid);
				memcpy(tGoods.name, name, sizeof(tGoods.name));
				tGoods.dir = dir;
				tGoods.width = width;
				tGoods.height = height;
				tGoods.posX = posX;
				tGoods.posY = posY;
				tVecGoods.push_back(tGoods);
				msg.Add( tGoods.guid );
			}
			else
				msg.Add( (*vecGoodsItr).guid );

			msg.Add( rgnID );
			msg.Add( (char*)name );
			msg.Add( (long)nCounts );
			msg.Add( (long)width );
			msg.Add( (long)height );			
			msg.Add( (long)posX );
			msg.Add( (long)posY );
			msg.Add ( pRegion->GetID() );
			if( strScript )
			{
				msg.Add( BYTE(1) );
				msg.Add( strScript );
			}
			else 
			{
				msg.Add( BYTE(0) );
			}					
			msg.Send();
		}
	}
}
// ɾ�����˷��ݸ�����Ʒ(Monster)
void CRgnManager::DeletePHGoods(const CGUID& ownerID, const CGUID& goodsID)
{
	RgnByIDMapItr rgnItr = m_mapPersonalHouseDupRgn.find(ownerID);
	if(rgnItr != m_mapPersonalHouseDupRgn.end())
	{
		CServerRegion* pRegion = rgnItr->second;
		CShape* pShape = NULL;
		if( pRegion ) 
			pShape = dynamic_cast<CShape*>( pRegion -> FindChildObject( TYPE_MONSTER, goodsID) );
		if( pShape )
		{
			// ��PHGoods������ɾ��
			bool isFindFlag = false;

			PHGoods tGoods;

			PHGoodsMapItr goodsItr =m_PHGoodsMap.find(pRegion->GetExID());
			if(goodsItr != m_PHGoodsMap.end())
			{
				if(goodsItr->second.size() != 0)
				{
					vecPHGoodsItr vecGoodsItr = goodsItr->second.begin();
					for(; vecGoodsItr != goodsItr->second.end(); vecGoodsItr++)
					{
						if(
							//	vecGoodsItr->guid == right->guid &&
							strcmp(vecGoodsItr->name, pShape->GetName()) == 0 &&
							vecGoodsItr->dir == pShape->GetDir() &&
							vecGoodsItr->posX == ((CMonster*)pShape)->GetTileX() &&
							vecGoodsItr->posY == ((CMonster*)pShape)->GetTileY() ) // ��ͬһ��GOODS
						{	
							tGoods = *vecGoodsItr;
							goodsItr->second.erase(vecGoodsItr);
							isFindFlag = true;
							break;
						}
					}
				}
			}


			/**## ֪ͨ�ͻ��˹���ɾ����Ϣ */
			if(isFindFlag == true)
			{
				if(pRegion)
				{
					pRegion->DelShapeToAround(pShape);
					pRegion->DeleteChildObject(pShape);
				}
				
				// ֪ͨWS
				CMessage msg1( MSG_S2W_SERVER_DELETEPHGOODS );
				msg1.Add( ownerID );
				msg1.Add( tGoods.guid );
				msg1.Add( tGoods.name );
				msg1.Add( (long)tGoods.width );
				msg1.Add( (long)tGoods.height );			
				msg1.Add( tGoods.posX );
				msg1.Add( tGoods.posY );
				msg1.Send();
			}
			
		}
	}
}
// ɾ�����˷��ݸ�����Ʒ(Monster)
void CRgnManager::DeletePHGoods(const CGUID& rgnID, const char* goodsName)
{
	RgnByIDMapItr rgnItr = m_mapPersonalHouseDupRgn.find(rgnID);
	if(rgnItr != m_mapPersonalHouseDupRgn.end())
	{
		CServerRegion* pRegion = rgnItr->second;
		CShape* pShape = NULL;
		if( pRegion ) 
			pShape = dynamic_cast<CShape*>( pRegion -> FindChildObjectByName( TYPE_MONSTER, goodsName) );
		if( pShape )
		{
			// ��PHGoods������ɾ��
			bool isFindFlag = false;

			PHGoods tGoods;

			PHGoodsMapItr goodsItr =m_PHGoodsMap.find(pRegion->GetExID());
			if(goodsItr != m_PHGoodsMap.end())
			{
				if(goodsItr->second.size() != 0)
				{
					vecPHGoodsItr vecGoodsItr = goodsItr->second.begin();
					for(; vecGoodsItr != goodsItr->second.end(); vecGoodsItr++)
					{
						if(
							//	vecGoodsItr->guid == right->guid &&
							strcmp(vecGoodsItr->name, pShape->GetName()) == 0 &&
							vecGoodsItr->dir == pShape->GetDir() &&
							vecGoodsItr->posX == ((CMonster*)pShape)->GetTileX() &&
							vecGoodsItr->posY == ((CMonster*)pShape)->GetTileY() ) // ��ͬһ��GOODS
						{	
							tGoods = *vecGoodsItr;
							goodsItr->second.erase(vecGoodsItr);
							isFindFlag = true;
							break;
						}
					}
				}
			}

			char szRgnID[128];
			rgnID.tostring(szRgnID);
			char szGoodsID[128];
			tGoods.guid.tostring(szGoodsID);
			AddLogText(CStringReading::LoadString(IDS_GS_RGNMANAGER, STR_GS_RGNMANAGER_DELPHRGNGOODSOK), szRgnID, szGoodsID, tGoods.name);

			/**## ֪ͨ�ͻ��˹���ɾ����Ϣ */
			if(isFindFlag == true)
			{
				if(pRegion)
				{
					pRegion->DelShapeToAround(pShape);
					pRegion->DeleteChildObject(pShape);
				}
				// ֪ͨWS
				CMessage msg1( MSG_S2W_SERVER_DELETEPHGOODS );
				msg1.Add( tGoods.guid );
				msg1.Add( tGoods.name );
				msg1.Add( (long)tGoods.width );
				msg1.Add( (long)tGoods.height );			
				msg1.Add( tGoods.posX );
				msg1.Add( tGoods.posY );
				msg1.Send();
			}

		}
	}
}

void CRgnManager::AddTeamRgnGUID(const CGUID& ownerID, LONG lTemplateRgnID, const CGUID& rgnGuid, BOOL bCover)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		map<LONG, CGUID>::iterator itr = ite->second.find(lTemplateRgnID);
		if(ite->second.end() != itr && !bCover)
			return;
		ite->second[lTemplateRgnID] = rgnGuid;
	}
	else
	{
		map<LONG, CGUID> teamGuidMap;
		teamGuidMap[lTemplateRgnID] = rgnGuid;
		m_mapTeamRgnGUID[ownerID] = teamGuidMap;
	}

	SendTeamRgnToWs(ownerID);
}
void CRgnManager::DelTeamRgnGUID(const CGUID& ownerID, LONG lTemplateRgnID)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
		ite->second.erase(lTemplateRgnID);
	SendTeamRgnToWs(ownerID);
}
void CRgnManager::DelTeamRgnGUID(const CGUID& ownerID, const CGUID& rgnGuid)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			if(itr->second == rgnGuid)
			{
				ite->second.erase(itr);
				return;
			}
		}
	}
	SendTeamRgnToWs(ownerID);
}
//! ���һ����ҵĶ��鸱��
void CRgnManager::ClearTeamRgnGUID(const CGUID& ownerGuid)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerGuid);
	if( ite != m_mapTeamRgnGUID.end() )
		ite->second.clear();
	SendTeamRgnToWs(ownerGuid);
}
const CGUID& CRgnManager::GetTeamRgnGUID(const CGUID& ownerID, LONG lTemplateRgnID)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		map<LONG, CGUID>::iterator itr = ite->second.find(lTemplateRgnID);
		if(ite->second.end() != itr)
			return itr->second;
	}
	return NULL_GUID;
}
//! ͨ��˳��õ�һ������ĸ���GUID
const CGUID& CRgnManager::GetTeamRgnGuidByIndex(const CGUID& ownerID, LONG lIndex)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		if(ite->second.size() < lIndex || 0 == lIndex)
			return NULL_GUID;
		LONG lCurrIndex = 1;
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			if(lIndex == lCurrIndex)
			{
				return itr->second;
			}
			++lCurrIndex;
		}
	}
	return NULL_GUID;
}
//! ͨ��˳��õ�һ������ĸ���ģ��ID
LONG CRgnManager::GetTeamRgnTemplateIdByIndex(const CGUID& ownerID, LONG lIndex)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		if(ite->second.size() < lIndex || 0 == lIndex)
			return -1;
		LONG lCurrIndex = 1;
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			if(lIndex == lCurrIndex)
			{
				return itr->first;
			}
			++lCurrIndex;
		}
	}
	return -1;
}
map<LONG, CGUID>*	CRgnManager::GetTeamRgnGuidMap(const CGUID& ownerGuid)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerGuid);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		return &ite->second;
	}
	return NULL;
}
VOID CRgnManager::RgnGuidCode(const CGUID& ownerID, vector<BYTE> &vData)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		_AddToByteArray(&vData, (LONG)ite->second.size());
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			_AddToByteArray(&vData, itr->first);
			_AddToByteArray(&vData, itr->second);
		}
	}
}
VOID CRgnManager::RgnGuidDecode(const CGUID& ownerID, BYTE *pData, LONG &lPos)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		ite->second.clear();
		LONG lNum = _GetLongFromByteArray(pData, lPos);
		for (LONG i = 0; i < lNum; ++i)
		{
			LONG lTemplateRgnID = _GetLongFromByteArray(pData, lPos);
			CGUID guid;
			_GetBufferFromByteArray(pData, lPos, guid);
			if(ite->second.end() == ite->second.find(lTemplateRgnID))
			{
				ite->second.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
				CServerRegion* pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(guid);
				if(pRgn)
					pRgn->SetOwnerGUID(ownerID);
			}
		}
	}
}
//! ���뽻�����鸱������������ͬģ��ĸ�����
VOID CRgnManager::RgnGuidSwapFromArray(const CGUID& ownerID, BYTE *pData, LONG &lPos)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		LONG lNum = _GetLongFromByteArray(pData, lPos);
		for (LONG i = 0; i < lNum; ++i)
		{
			LONG lTemplateRgnID = _GetLongFromByteArray(pData, lPos);
			CGUID guid;
			_GetBufferFromByteArray(pData, lPos, guid);
			if(ite->second.end() == ite->second.find(lTemplateRgnID))
			{
				ite->second.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
				CServerRegion* pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(guid);
				if(pRgn)
					pRgn->SetOwnerGUID(ownerID);
			}
		}
		SendTeamRgnToWs(ownerID);
	}
}


//!					���鸱��GUID����
VOID CRgnManager::RgnGuidCode(const CGUID& ownerID, DBWriteSet& setWriteDB)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		setWriteDB.AddToByteArray((LONG)ite->second.size());
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			setWriteDB.AddToByteArray(itr->first);
			setWriteDB.AddToByteArray(itr->second);
		}
	}
}
//!					���鸱��GUID����
VOID CRgnManager::RgnGuidDecode(const CGUID& ownerID, DBReadSet& setReadDB)
{
	map<LONG, CGUID> mapRegionID;
	LONG lNum = setReadDB.GetLongFromByteArray();
	for (LONG i = 0; i < lNum; ++i)
	{
		LONG lTemplateRgnID = setReadDB.GetLongFromByteArray();
		CGUID guid;
		setReadDB.GetBufferFromByteArray(guid);
		mapRegionID.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
	}

	m_mapTeamRgnGUID[ownerID] = mapRegionID;
}
//!					���뽻�����鸱������������ͬģ��ĸ�����
VOID CRgnManager::RgnGuidSwapFromArray(const CGUID& ownerID, DBReadSet& setReadDB)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite == m_mapTeamRgnGUID.end() )
	{
		m_mapTeamRgnGUID.insert(pair<CGUID, map<LONG, CGUID>>(ownerID, map<LONG, CGUID>()));
		ite = m_mapTeamRgnGUID.find(ownerID);
	}

	LONG lNum = setReadDB.GetLongFromByteArray();
	for (LONG i = 0; i < lNum; ++i)
	{
		LONG lTemplateRgnID = setReadDB.GetLongFromByteArray();
		CGUID guid;
		setReadDB.GetBufferFromByteArray(guid);
		if(ite->second.end() == ite->second.find(lTemplateRgnID))
			ite->second.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
	}
	SendTeamRgnToWs(ownerID);
}

//!					���͸������ݵ�WS
VOID CRgnManager::SendTeamRgnToWs(const CGUID& ownerGuid)
{
	CMessage msg(MSG_S2W_RGN_UPDATE_PLAYER_TEAM_RGNGUID);
	DBWriteSet setWriteDB;
	msg.GetDBWriteSet(setWriteDB);

	msg.Add(ownerGuid);
	RgnGuidCode(ownerGuid, setWriteDB);
	msg.Send(false);
}
//!					�õ���ҵĸ��˸���GUID�б�
map<CGUID, CGUID>&	CRgnManager::GetPersonalRgnGuidMap(const CGUID& ownerGuid)
{
	return m_mapPersonalRgnGUID;
}
void CRgnManager::AddPersonalRgnGUID(const CGUID& ownerGuid, const CGUID& rgnGuid)
{
	m_mapPersonalRgnGUID[ownerGuid] = rgnGuid;
}
//! ɾ��һ����Ҹ��˸���GUID
void CRgnManager::DelPersonalRgnGUID(const CGUID& ownerGuid)
{
	m_mapPersonalRgnGUID.erase(ownerGuid);
}
//!					�õ�һ�����˸�����GUID
const CGUID& CRgnManager::GetPersonalRgnGUID(const CGUID& ownerGuid)
{
	map<CGUID, CGUID>::iterator itr = m_mapPersonalRgnGUID.find(ownerGuid);
	if(itr != m_mapPersonalRgnGUID.end())
		return itr->second;
	return NULL_GUID;
}
//!					���һ���������ݿ�
bool CRgnManager::AddDupRgnData(long lTemplateID, const BYTE* pData, long lBufSize)
{
	if(pData && lBufSize > 0)
	{
		if(m_mapDupRgnData.size() < 128)//CGlobeSetup::GetSetup()->lGsDupRgnDataMaxNum)
		{
			map<long, tagDupRgnData*>::iterator itr = m_mapDupRgnData.find(lTemplateID);
			if(itr != m_mapDupRgnData.end() && itr->second)
			{
				MP_DELETE(itr->second);
				m_mapDupRgnData.erase(itr);
			}
			tagDupRgnData* tagData = MP_NEW tagDupRgnData;
			tagData->lBufSize = lBufSize;
			tagData->pData = pData;
			m_mapDupRgnData[lTemplateID] = tagData;
			return true;
		}
	}

	return false;
}
//!					�ж��Ƿ��иø������ݿ�
long CRgnManager::FindDupRgnData(long lTemplateID)
{
	map<long, tagDupRgnData*>::iterator itr = m_mapDupRgnData.find(lTemplateID);
	if(itr != m_mapDupRgnData.end() && itr->second)
		return itr->second->lBufSize;
	return 0;
}
//!					�õ��ø������ݿ�
const BYTE* CRgnManager::GetDupRgnData(long lTemplateID)
{
	map<long, tagDupRgnData*>::iterator itr = m_mapDupRgnData.find(lTemplateID);
	if(itr != m_mapDupRgnData.end() && itr->second)
	{
		if(itr->second)
			return itr->second->pData;
	}
	return NULL;
}
//!					������и������ݿ�
void CRgnManager::DelAllDupRgnData(void)
{
	map<long, tagDupRgnData*>::iterator itr = m_mapDupRgnData.begin();
	for(; itr != m_mapDupRgnData.end(); itr++)
	{
		MP_DELETE(itr->second);
	}
	m_mapDupRgnData.clear();
}