#include "StdAfx.h"
#include "gamefaction.h"
#include "GameUnion.h"
#include "gameorganizingctrl.h"
#include "..\..\public\OrganizingParam.h"

#include "..\Player.h"
#include "..\Country\Country.h"
#include "..\Country\CountryHandler.h"
#include "..\..\public\Crc32Static.h"

GameFaction::GameFaction(void)
:m_OptingMember(60 * 30)
{
}

GameFaction::~GameFaction(void){}


BOOL GameFaction::AddToByteArray(vector<BYTE>& ByteArray)
{
	//! ����ӿ�Ӧ�ò����õ�
	assert(false);

	//! ������Ϣ
	_AddToByteArray(&ByteArray, m_FacPublicData.guid);
	_AddToByteArray(&ByteArray, m_FacPublicData.szName, MAXSIZE_FactionName);
	_AddToByteArray(&ByteArray, m_FacPublicData.lCurrSkillID);

	//! ������Ϣ����
	_AddToByteArray(&ByteArray, &m_FacBaseData, sizeof(tagFacBaseData));
	//! ��������
	_AddToByteArray(&ByteArray, &m_Pronounce, sizeof(tagOrgPronounceWord));
	//! ���
	_AddToByteArray(&ByteArray, (LONG)m_vIconData.size());
	if(0 != m_vIconData.size())
	{
		_AddToByteArray(&ByteArray, &m_vIconData[0], m_vIconData.size());
	}

	//! ��Ա��Ϣ�б�
	_AddToByteArray(&ByteArray, (LONG)m_mapMemInfo.size());
	for (map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin(); ite != m_mapMemInfo.end(); ++ite)
	{
		_AddToByteArray(&ByteArray, &(ite->second), sizeof(tagFacMemInfo));
	}

	//! �����б�
	_AddToByteArray(&ByteArray, (LONG)m_mapFacApplyPerson.size());
	for (map<CGUID, tagFacApplyPerson>::iterator ite = m_mapFacApplyPerson.begin(); ite != m_mapFacApplyPerson.end(); ++ite)
	{
		_AddToByteArray(&ByteArray, &(ite->second), sizeof(tagFacApplyPerson));
	}

	//! ��������
	_AddToByteArray(&ByteArray, &m_Pronounce, sizeof(tagOrgPronounceWord));
	_AddToByteArray(&ByteArray, &m_LeaveWord, sizeof(tagOrgPronounceWord));

	return TRUE;
}

BOOL GameFaction::DecodeFromMsgSet(DBReadSet &DbReadSet, map<CGUID, CGUID> *pMapApplicant)
{
	//! ������Ϣ
	DbReadSet.GetBufferFromByteArray(m_FacPublicData.guid);
	DbReadSet.GetBufferFromByteArray(m_FacPublicData.szName, MAXSIZE_FactionName);
	m_FacPublicData.lCurrSkillID = DbReadSet.GetLongFromByteArray();
	m_FacPublicData.lLevel = DbReadSet.GetLongFromByteArray();

	//! ������Ϣ����
	DbReadSet.GetBufferFromByteArray(&m_FacBaseData, sizeof(tagFacBaseData));
	//! ����
	DbReadSet.GetBufferFromByteArray(&m_Pronounce, sizeof(tagOrgPronounceWord));
	//! ���
	LONG lIocnSize = DbReadSet.GetLongFromByteArray();
	if(0 != lIocnSize)
	{
		m_vIconData.clear();
		m_vIconData.resize(lIocnSize);
		DbReadSet.GetBufferFromByteArray(&m_vIconData[0], lIocnSize);
		CCrc32Static::DataCrc32(&m_vIconData[0], m_vIconData.size(), m_dwCRC32IconCode);
	}
	else
	{
		m_dwCRC32IconCode = 0;
	}

	//! ��Ա��Ϣ�б�
	m_mapMemInfo.clear();
	LONG lMemInfoSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lMemInfoSize; ++i)
	{
		tagFacMemInfo FacMemInfo;
		DbReadSet.GetBufferFromByteArray(&FacMemInfo, sizeof(tagFacMemInfo));
		m_mapMemInfo[FacMemInfo.MemberGuid] = FacMemInfo;
		UpdateMemberFacInfo(m_mapMemInfo.find(FacMemInfo.MemberGuid));
	}

	//! �����б�
	m_mapFacApplyPerson.clear();
	LONG lApplyPersonSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lApplyPersonSize; ++i)
	{
		tagFacApplyPerson FacApplyPerson;
		DbReadSet.GetBufferFromByteArray(&FacApplyPerson, sizeof(tagFacApplyPerson));
		m_mapFacApplyPerson[FacApplyPerson.PlayerGuid] = FacApplyPerson;
		if(NULL != pMapApplicant)
			pMapApplicant->insert(pair<CGUID, CGUID>(FacApplyPerson.PlayerGuid, m_FacPublicData.guid));
	}

	//! ��������
	DbReadSet.GetBufferFromByteArray(&m_Pronounce, sizeof(tagOrgPronounceWord));
	DbReadSet.GetBufferFromByteArray(&m_LeaveWord, sizeof(tagOrgPronounceWord));
	//! GS���õ�һ��LONG�ͱ��
	DbReadSet.GetLongFromByteArray();
	
	m_FacBaseData.lFacBattle=DbReadSet.GetLongFromByteArray();
	m_FacBaseData.lFacArmyInvest=DbReadSet.GetLongFromByteArray();
	//�ݵ���Ϣ
	m_mapBaseInfo.clear();
	LONG lSize=DbReadSet.GetLongFromByteArray();
	tagBaseInfo tgInfo;
	for (LONG i=0;i<lSize;i++)
	{
		DbReadSet.GetBufferFromByteArray(&tgInfo,sizeof(tagBaseInfo));
		m_mapBaseInfo[tgInfo._cBaseName]=tgInfo;
	}
	//����Ƽ�
	m_TechLv.clear();
	lSize=DbReadSet.GetLongFromByteArray();
	DWORD dwLv=0,dwNo=0;
	for (LONG i=0;i<lSize;i++)
	{
		dwNo=DbReadSet.GetDwordFromByteArray();
		dwLv=DbReadSet.GetDwordFromByteArray();
		m_TechLv[dwNo]=dwLv;
	}
	//ս����Ϣ
	m_Enemy.clear();
	lSize=DbReadSet.GetLongFromByteArray();
	tagEnemy tgEnemy;
	list<tagEnemy> listEnemy;
	for (LONG i=0;i<lSize;i++)
	{
		eBattleType eType=(eBattleType)DbReadSet.GetByteFromByteArray();
		LONG lEnemyNum=DbReadSet.GetLongFromByteArray();
		listEnemy.clear();
		for (LONG j=0;j<lEnemyNum;j++)
		{
			DbReadSet.GetBufferFromByteArray(&tgEnemy,sizeof(tagEnemy));
			listEnemy.push_back(tgEnemy);
		}
		m_Enemy[eType]=listEnemy;
	}
	return TRUE;
}

//! ���������ݵ�bufĩβ
void GameFaction::AddIconData_ToArray(vector<BYTE> *pByteArray)
{
	_AddToByteArray(pByteArray, GetExID());
	_AddToByteArray(pByteArray, (LONG)m_vIconData.size());
	if(0 != m_vIconData.size())
		_AddToByteArray(pByteArray, &m_vIconData[0], m_vIconData.size());
}

//! ���빫����Ϣ����Ϣĩβ
void GameFaction::AddPublicData_ToMsgBack(CMessage *pMsg)
{
	pMsg->AddEx(&m_FacPublicData, sizeof(tagFacPublicData));
}

void GameFaction::AddPublicData_ToArray(vector<BYTE> *pByteArray)
{
	_AddToByteArray(pByteArray, &m_FacPublicData, sizeof(tagFacPublicData));
}

void GameFaction::AddPublicData_ToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(&m_FacPublicData, sizeof(tagFacPublicData));
}

//! ����������ϵ���Ϣĩβ
void GameFaction::AddBaseData_ToMsgBack(CMessage *pMsg)
{
	m_FacBaseData.lMemberNum = m_mapMemInfo.size();
	pMsg->AddEx(&m_FacBaseData, sizeof(tagFacBaseData));
}

//! �����Ա���ϵ���Ϣĩβ
void GameFaction::AddMember_ToMsgBack(CMessage *pMsg, const CGUID &AimGUID)
{
	pMsg->Add((LONG)m_mapMemInfo.size());
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		pMsg->AddEx(&(ite->second), sizeof(tagFacMemInfo));
	}
}

//! �����Ա����
void GameFaction::AddMember_ToMsgBack(CMessage* pMsg)
{
	pMsg->Add((LONG)m_mapMemInfo.size());
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		pMsg->AddEx(&(ite->second), sizeof(tagFacMemInfo));
	}
}

//! ����һ����Ա���ϵ���Ϣĩβ
void GameFaction::AddOneMember_ToMsgBack(const CGUID &MemeberGuid, CMessage *pMsg, const CGUID &AimGUID)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemeberGuid);
	if (m_mapMemInfo.end() != ite)
	{
		pMsg->AddEx(&(ite->second), sizeof(tagFacMemInfo));
	}	
}


//! �������������ϵ���Ϣĩβ
void GameFaction::AddApply_ToMsgBack(CMessage *pMsg)
{
	pMsg->Add((LONG)m_mapFacApplyPerson.size());
	map<CGUID, tagFacApplyPerson>::iterator ite = m_mapFacApplyPerson.begin();
	for ( ; ite != m_mapFacApplyPerson.end(); ++ite)
	{
		pMsg->AddEx(&(ite->second), sizeof(tagFacApplyPerson));
	}
}

//! ���빫�浽��Ϣĩβ
void GameFaction::AddPronounce_ToMsgBack(CMessage *pMsg)
{
	pMsg->AddEx(&m_Pronounce, sizeof(tagOrgPronounceWord));
}

//! �������Ե���Ϣĩβ
void GameFaction::AddLeaveWord_ToMsgBack(CMessage *pMsg)
{
	pMsg->AddEx(&m_LeaveWord, sizeof(tagOrgPronounceWord));
}

//! �õ�������������Ϣ
void GameFaction::GetApplyInfo(tagApplyInfo &ApplyInfo)
{
	//! ���ID
	ApplyInfo.guid				= m_FacPublicData.guid;
	//! �������
	memmove(ApplyInfo.szName, m_FacPublicData.szName, MAXSIZE_FactionName);
	//! ���ȼ�
	ApplyInfo.lLevel			= m_FacPublicData.lLevel;
	//! ��Ա����
	ApplyInfo.lMemberNum		= m_mapMemInfo.size();
	//! ����Ա����
	const tagFacLevelSetup *pFacLevelSetup = COrganizingParam::getInstance().GetLevelSetup(m_FacPublicData.lLevel);
	if(NULL == pFacLevelSetup)
		ApplyInfo.lMaxMemberNum = ApplyInfo.lMemberNum;
	else
		ApplyInfo.lMaxMemberNum = pFacLevelSetup->lMaxMemNum;
	//! �Ƿ�������ļ
	ApplyInfo.bIsRecruiting		= m_FacBaseData.bIsRecruiting;
	//! ��ǰӵ����Դ
	ApplyInfo.lCurrRes			= m_FacBaseData.lCurrRes;
}


//! ����һ����Ա��Player����Ĺ�����Ϣ
void GameFaction::UpdateMemberFacInfo(map<CGUID, tagFacMemInfo>::const_iterator &iteMemInfo)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(iteMemInfo->second.MemberGuid);
	if (NULL != pPlayer)
	{
		pPlayer->SetFactionInfo(this);
		pPlayer->SetFacMemInfo(&(iteMemInfo->second));

		CMessage msgFac(MSG_S2C_Faction_PublicData_Notify);
		AddPublicData_ToMsgBack(&msgFac);
		msgFac.SendToAround(pPlayer);

		CMessage msg(MSG_S2C_ORGSYS_UpdatePlayerFacInfo);
		msg.Add(iteMemInfo->second.MemberGuid);
		msg.Add(GetExID());
		msg.Add(iteMemInfo->second.szTitle);
		msg.SendToAround(pPlayer);
	}
}

void GameFaction::UpdateMemberFacInfo(CPlayer *pPlayer)
{
	if (NULL != pPlayer)
	{
		map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(pPlayer->GetExID());
		if(ite != m_mapMemInfo.end())
		{
			pPlayer->SetFactionID(GetExID());
			pPlayer->SetUnionID(m_FacBaseData.SuperiorGuid);
			pPlayer->SetFacMemInfo(&ite->second);
			return;
		}
	}
}

//!							��Ӧ��������
void GameFaction::OnJoinUnion(GameUnion *pUnion)
{
	assert(NULL != pUnion);
	if(NULL == pUnion)
		return;
	m_FacBaseData.SuperiorGuid = pUnion->GetExID();

	CMessage msgNotify(MSG_S2C_UNION_Notify_Creation);
	msgNotify.Add(GetExID());
	msgNotify.Add(pUnion->GetExID());
	msgNotify.Add(pUnion->GetName());

	CMessage msgBaseData(MSG_S2C_UNION_Notify_BaseData);
	pUnion->AddBaseToMsg(&msgBaseData);

	CMessage msgOneMember(MSG_S2C_UNION_Notify_AllMemberData);
	if(pUnion->AddOneMemberToMsg(GetExID(), &msgOneMember))
		return;

	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(ite->first);
		if(NULL != pPlayer)
		{
			pPlayer->SetUnionID(pUnion->GetExID());
			msgNotify.SendToAround(pPlayer);
			msgBaseData.SendToPlayer(pPlayer->GetExID());
			msgOneMember.SendToPlayer(pPlayer->GetExID());
		}
	}
}

//! ��Ӧ�˳�����
void GameFaction::OnQuitUnion(void)
{
	m_FacBaseData.SuperiorGuid = NULL_GUID;

	CMessage msgNotify(MSG_S2C_UNION_Notify_Quit);
	msgNotify.Add(GetExID());

	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(ite->first);
		if(NULL != pPlayer)
		{
			pPlayer->SetUnionID(NULL_GUID);
			msgNotify.SendToAround(pPlayer);
		}
	}
}

//! ��Ӧ���˽�ɢ
void GameFaction::OnDisbandUnion(void)
{
	m_FacBaseData.SuperiorGuid = NULL_GUID;

	CMessage msgNotify(MSG_S2C_UNION_Notify_Disband);
	msgNotify.Add(GetExID());

	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(ite->first);
		if(NULL != pPlayer)
		{
			pPlayer->SetUnionID(NULL_GUID);
			msgNotify.SendToAround(pPlayer);
		}
	}
}

//! Ϊ��Ա��ʼ��������Ϣ
void GameFaction::SetMemberUnionInfo(const CGUID &UnionGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(ite->first);
		if(NULL != pPlayer)
			pPlayer->SetUnionID(UnionGuid);
	}
}

//! ��Ӧִ�и�������
void GameFaction::OnBoonRun(void)
{
	//��ԴΪ��ʱ,��ִ�и�������
	if (m_FacBaseData.lCurrRes<0)
	{
		return;
	}
	const tagFacExpLevelSetup *pFacExpLevelSetup = COrganizingParam::getInstance().GetFacExpLevelSetup(m_FacBaseData.lSubExpLevel);
	if(NULL == pFacExpLevelSetup)
		return;

	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(ite->first);
		if(NULL != pPlayer)
		{
			DWORD dwOldExp = pPlayer->GetExp();
			DWORD dwAddExp = pFacExpLevelSetup->fBaseExp * CPlayerList::GetFacBoonExpScale(pPlayer->GetLevel()) + 0.5;
			pPlayer->IncExp(dwAddExp, eAET_FacBoon);
			pPlayer->CheckLevel();
			pPlayer->AddIncExpLog(dwOldExp, pPlayer->GetExpMultiple(), dwAddExp, 0, eAET_FacBoon, "���帣��");			
		}
	}
}

const CGUID&	GameFaction::GetExID()
{ 
	return m_FacPublicData.guid; 
}
const CGUID&	GameFaction::SetExID(const CGUID& guid) 
{ 
	return m_FacPublicData.guid = guid; 
}

const char* GameFaction::GetName()const
{
	return m_FacPublicData.szName;
}
void GameFaction::SetName(const char* pName)
{
	if (NULL != pName)
	{
		LONG lSize = strlen(pName);
		lSize = (15 >= lSize ) ? lSize : 15;
		memmove(m_FacPublicData.szName, pName, lSize);
	}
}

//! �õ�����
BYTE GameFaction::GetCountry(void)
{
	return m_FacBaseData.lCountryID;
}

//! ��Ա�ж�
const CGUID& GameFaction::IsMember(const CGUID& MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
		return ite->first;
	return NULL_GUID;
}

//! ���ݳ�Ա�����ж�
bool GameFaction::IsMember( const std::string &name ) const
{
	for( map<CGUID, tagFacMemInfo>::const_iterator it = m_mapMemInfo.begin();
		it != m_mapMemInfo.end(); ++ it )
	{
		const tagFacMemInfo &info = it->second;
		if( strcmp( info.szName, name.c_str() ) == 0 )
		{
			return true;
		}
	}

	return false;
}

//! �������ж�
BOOL GameFaction::HasApplicant(const CGUID& ApplicantGuid)
{
	map<CGUID, tagFacApplyPerson>::iterator ite = m_mapFacApplyPerson.find(ApplicantGuid);
	return m_mapFacApplyPerson.end() != ite;
}

//! ��û᳤
const CGUID& GameFaction::GetMaster(void)
{
	return m_FacBaseData.MasterGuid;
}

//! �õ���ǰ�л�ȼ�
LONG GameFaction::GetLevel(DWORD dwUpgradeType)
{
	switch(dwUpgradeType)
	{
	case eUT_FactionLevel:		//! ����ȼ�
		return m_FacPublicData.lLevel;
	case eUT_ExpLevel:			//! ���鸣���ȼ�
		return m_FacBaseData.lSubExpLevel;
	case eUT_SpecialityLevel:		//! �ֿ�ȼ�
		return m_FacBaseData.lSpecialityLevel;
	case eUT_BattleLevel:		//! ս���ȼ�
		return m_FacBaseData.lBattleLevel;
	case eUT_BussinessLevel:	//! ��ҵ�ȼ�
		return m_FacBaseData.lBussinessLevel;
	case eUT_NobleLevel:		//! ����ȼ�
		return m_FacBaseData.lNoblelevel;
	}
	return 0;
}

//! �õ���ǰ�л�ȼ�
BOOL GameFaction::SetLevel(LONG lLvl, DWORD dwUpgradeType)
{
	switch(dwUpgradeType)
	{
	case eUT_FactionLevel:		//! ����ȼ�
		{
			m_FacPublicData.lLevel = lLvl;
		}
		break;
	case eUT_ExpLevel:			//! ���鸣���ȼ�
		{
			m_FacBaseData.lSubExpLevel = lLvl;
		}
		break;
	case eUT_SpecialityLevel:		//! רҵ�ȼ�
		{
			m_FacBaseData.lSpecialityLevel = lLvl;
		}
		break;
	case eUT_BattleLevel:		//! ս���ȼ�
		{
			m_FacBaseData.lBattleLevel = lLvl;
		}
		break;
	case eUT_BussinessLevel:	//! ��ҵ�ȼ�
		{
			m_FacBaseData.lBussinessLevel = lLvl;
		}
		break;
	case eUT_NobleLevel:		//! ����ȼ�
		{
			m_FacBaseData.lNoblelevel = lLvl;
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

//! �õ���Ա�ٽ�
LONG GameFaction::GetMemberJobLvl(const CGUID& MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
		return ite->second.lJobLvl;

	return 0;
}

//! ������ļ״̬
BOOL GameFaction::SetRecruitState(BOOL bOpen)
{
	if(m_FacBaseData.bIsRecruiting != bOpen)
	{
		m_FacBaseData.bIsRecruiting = bOpen;
		return TRUE;
	}
	return FALSE;
}

//! �жϵ�ǰ�ܷ��ɢ
eFactionOptErrInfo GameFaction::CanDisban(void)
{
	LONG lNeedLevel = COrganizingParam::getInstance().GetFacPurviewNeedLevel(eFPI_Disband);

	if (lNeedLevel > GetLevel(eUT_FactionLevel))
	{
		return eFOEI_Err_Level_Noenough;
	}

	if(NULL_GUID != m_FacBaseData.SuperiorGuid)
	{
		return eFOEI_Err_HadUnion;
	}

	return eFOEI_NotErr;
}

//! ��ɢ
void GameFaction::Disband(void)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for ( ; ite != m_mapMemInfo.end(); ++ite)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(ite->first);
		if (NULL != pPlayer)
		{
			pPlayer->SetFactionID(NULL_GUID);
			pPlayer->SetUnionID(NULL_GUID);
			pPlayer->SetFacMemInfo(NULL);
		}
	}
}

//! ����ԱȨ��
BOOL GameFaction::TestPurview(const CGUID& MemberGuid, eMemberPurview ePurview)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		LONG lJobLvl = ite->second.lJobLvl;
		if (Exceptive_JobLvl == lJobLvl)
			return TRUE;
		else if (0 == lJobLvl)
			return FALSE;
		else if (0 < lJobLvl && 6 > lJobLvl)
			return (m_FacBaseData.arrJobPurview[lJobLvl - 1] & ePurview) != 0;
	}

	return FALSE;
}

//! �õ���ԱȨ��
LONG GameFaction::GetPurview(const CGUID& MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		LONG lJobLvl = ite->second.lJobLvl;
		if (Exceptive_JobLvl == lJobLvl)
			return eMP_All;
		else if (0 == lJobLvl)
			return eMP_Null;
		else if (0 < lJobLvl && 6 > lJobLvl)
			return m_FacBaseData.arrJobPurview[lJobLvl - 1];
	}

	return eMP_Null;
}

//!							�õ���Ա������
const tagFacMemInfo* GameFaction::GetMemberData(const CGUID& MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		return &(ite->second);
	}
	return NULL;
}

//! ��ӳ�Ա
void GameFaction::AddMember(tagFacMemInfo &FacMemInfo)
{
	assert(m_mapMemInfo.find(FacMemInfo.MemberGuid) == m_mapMemInfo.end());
	m_mapMemInfo[FacMemInfo.MemberGuid] = FacMemInfo;
	CPlayer *pPlayer = GetGame()->FindPlayer(FacMemInfo.MemberGuid);
	UpdateMemberFacInfo(m_mapMemInfo.find(FacMemInfo.MemberGuid));
}

//! ɾ����Ա
void GameFaction::DelMember(const CGUID& MemberGuid)
{
	m_mapMemInfo.erase(MemberGuid);
	DelAttention(MemberGuid);
}

//! ���һ��������
BOOL GameFaction::AddApplicant(tagFacApplyPerson &FacApplyPerson)
{
	map<CGUID, tagFacApplyPerson>::iterator ite = m_mapFacApplyPerson.find(FacApplyPerson.PlayerGuid);
	if (m_mapFacApplyPerson.end() == ite)
	{
		m_mapFacApplyPerson[FacApplyPerson.PlayerGuid] = FacApplyPerson;
		return TRUE;
	}
	return FALSE;
}

//! ɾ��һ��������
BOOL GameFaction::DelApplicant(const CGUID& ApplicantGuid)
{
	map<CGUID, tagFacApplyPerson>::iterator ite = m_mapFacApplyPerson.find(ApplicantGuid);
	if (m_mapFacApplyPerson.end() != ite)
	{
		m_mapFacApplyPerson.erase(ite);
		return TRUE;
	}
	return FALSE;
}

//! ����һ��ְ��������
BOOL GameFaction::SetJobName(LONG lJobLvl, const char *pNewName)
{
	if (0 < lJobLvl && MAXNUM_JobLvl >= lJobLvl && NULL != pNewName)
	{
		if(0 != strcmp(m_FacBaseData.arr_szJobName[lJobLvl - 1], pNewName))
		{
			assert(strlen(pNewName) < MAX_MEMBER_NAME_SIZE);
			strcpy_s(m_FacBaseData.arr_szJobName[lJobLvl - 1], MAX_MEMBER_NAME_SIZE, pNewName);
			return TRUE;
		}
	}
	return FALSE;
}

//! ����һ��ְ����Ȩ��
BOOL GameFaction::SetJobPurview(LONG lJobLvl, LONG lPurview)
{
	if (0 < lJobLvl && MAXNUM_JobLvl >= lJobLvl)
	{
		m_FacBaseData.arrJobPurview[lJobLvl - 1] = lPurview;
		return TRUE;
	}
	return FALSE;
}

//! ���ù���
BOOL GameFaction::SetPronounce(tagOrgPronounceWord &OrgPronounceWord)
{
	memmove(&m_Pronounce, &OrgPronounceWord, sizeof(tagOrgPronounceWord)); 
	return TRUE;
}

//! ���ñ���
BOOL GameFaction::SetLeaveword(tagOrgPronounceWord &OrgPronounceWord)
{
	memmove(&m_LeaveWord, &OrgPronounceWord, sizeof(tagOrgPronounceWord)); 
	return TRUE;
}

//! ���ó�Ա��ְ��
BOOL GameFaction::SetMemJobLvl(const CGUID& MemberGuid, LONG lJobLvl)
{
	if (0 <= lJobLvl && MAXNUM_JobLvl >= lJobLvl)
	{
		map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
		if(m_mapMemInfo.end() != ite && MemberGuid != m_FacBaseData.MasterGuid)
		{
			ite->second.lJobLvl = lJobLvl;
			return TRUE;
		}
	}

	return FALSE;
}

//! ���ó�Ա�ĳƺ�
BOOL GameFaction::SetMemberTitle(const CGUID& MemberGuid, const char *pNewTitle)
{
	if (NULL != pNewTitle)
	{
		map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
		if(m_mapMemInfo.end() != ite)
		{
			strcpy_s(ite->second.szTitle, MAX_MEMBER_NAME_SIZE, pNewTitle);
			UpdateMemberFacInfo(ite);
			return TRUE;
		}
	}
	
	return FALSE;
}

//! �õ���Ա�ĳƺ�
const char* GameFaction::GetMemberTitle(const CGUID& MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		return ite->second.szTitle;
	}
	return NULL;
}

//! ���ó�Ա��ͼ
BOOL GameFaction::SetMemberMap(const CGUID& MemberGuid, const char *pMapName)
{
	if (NULL != pMapName)
	{
		map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
		if(m_mapMemInfo.end() != ite)
		{
			strcpy_s(ite->second.szRegion, MAX_MEMBER_NAME_SIZE, pMapName);
			return TRUE;
		}
	}
	
	return FALSE;
}

//! ���ó�Ա�ȼ�
BOOL GameFaction::SetMemberLevel(const CGUID& MemberGuid, LONG lLevel)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		ite->second.lLvl = lLevel;
		return TRUE;
	}
	return FALSE;
}

//! ���ó�Ա����
BOOL GameFaction::SetMemberHide(const CGUID& MemberGuid, BYTE cHide)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		ite->second.cHide = cHide;
		return TRUE;
	}
	return FALSE;
}

//! �õ���Ա�ȼ�
LONG GameFaction::GetMemberLevel(const CGUID& MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		return ite->second.lLvl;
	}
	return 0;
}

//! ���ó�Աְҵ
BOOL GameFaction::SetMemberOccu(const CGUID& MemberGuid, LONG lOccu)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		ite->second.lOccu = lOccu;
		return TRUE;
	}
	return FALSE;
}

//!
BOOL GameFaction::SetMemberName(const CGUID& MemberGuid, const char *pName)
{
	if(NULL != pName)
	{
		map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
		if(m_mapMemInfo.end() != ite)
		{
			LONG lSize = min(MAX_MEMBER_NAME_SIZE - 1, strlen(pName));
			memcpy(ite->second.szName, pName, lSize);
			return TRUE;
		}
	}
	
	return FALSE;
}

//! �����»᳤
BOOL GameFaction::SetNewMaster(tagFacMemInfo &NewMaster, tagFacMemInfo &OldMaster)
{
	map<CGUID, tagFacMemInfo>::iterator iteOld = m_mapMemInfo.find(OldMaster.MemberGuid);
	map<CGUID, tagFacMemInfo>::iterator iteNew = m_mapMemInfo.find(NewMaster.MemberGuid);
	if(m_mapMemInfo.end() != iteOld && m_mapMemInfo.end() != iteNew)
	{
		iteNew->second = NewMaster;
		iteOld->second = OldMaster;

		m_FacBaseData.MasterGuid = NewMaster.MemberGuid;
		strcpy_s(m_FacBaseData.szMasterName, MAX_MEMBER_NAME_SIZE, NewMaster.szName);

		UpdateMemberFacInfo(iteNew);
		UpdateMemberFacInfo(iteOld);
		return TRUE;
	}
	return FALSE;
}

//! ����״̬�ı�
BOOL GameFaction::MemberOnlineChange(const CGUID& MemberGuid, BOOL bOnline, LONG lLastOnlineTime)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		ite->second.bIsOnline = bOnline;
		if(0 != lLastOnlineTime)
			ite->second.LastOnlineTime = lLastOnlineTime;
		return TRUE;
	}
	return FALSE;
}

//! �ȼ��ı�
BOOL GameFaction::MemberLvlChange(const CGUID& MemberGuid, LONG lLvl)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		ite->second.lLvl = lLvl;
		return TRUE;
	}
	return FALSE;
}


//! ��ù��ṫ��
const tagOrgPronounceWord& GameFaction::GetPronounceWord(void)
{
	return m_Pronounce;
}

//! ���û��
BOOL GameFaction::SetIcon(BYTE* IconData, long size)
{
	if(MAX_ICON_SIZE > size && NULL != IconData)
	{
		m_vIconData.clear();
		m_vIconData.resize(size);
		if(0 != m_vIconData.size())
		{
			memmove(&m_vIconData[0], IconData, size);
			CCrc32Static::DataCrc32(&m_vIconData[0], m_vIconData.size(), m_dwCRC32IconCode);
		}
		else
		{
			m_dwCRC32IconCode = 0;
		}
		return TRUE;
	}
	return FALSE;
}

//! �õ����
void GameFaction::GetIcon(vector<BYTE> &vIconData)
{
	vIconData.clear();
	vIconData.resize(m_vIconData.size());
	if(0 != m_vIconData.size())
		memmove(&vIconData[0], &m_vIconData[0], m_vIconData.size());
}

//! ������Ϣ�����г�Ա
void GameFaction::SendToAll(CMessage &msg)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.begin();
	for (; ite != m_mapMemInfo.end(); ++ite)
	{
		msg.SendToPlayer(ite->second.MemberGuid);
	}
}

//! ��ӹ�����Ϣ��ע�ߣ�ϵͳ��Թ�ע�ߵĹ�����Ϣ���ּ�ʱ����
void GameFaction::AddAttention(const CGUID &MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
		m_OptingMember.CreateSession(MemberGuid);
}

//! ɾ��������Ϣ��ע��
void GameFaction::DelAttention(const CGUID &MemberGuid)
{
	m_OptingMember.TestSession(MemberGuid);
}


//! ����һ�����ڻỰģ�����Ϣ���ͺ�����ֻ��GameFaction::SendToAllConcern��ʹ�ã�
inline void SendToSession(CGUID &PlayerGuid, void* pVoidMsg)
{
	((CMessage*)pVoidMsg)->SendToPlayer(PlayerGuid);
};

//! ������Ϣ�����д򿪹������ĳ�Ա
void GameFaction::SendToAllConcern(CMessage &msg)
{
	//! ������Ϣ
	m_OptingMember.RunFunction_ForEach(SendToSession, &msg);
}

//! �õ���ǰ��Դ��
LONG GameFaction::GetRes(VOID)
{
	return m_FacBaseData.lCurrRes;
}
//! �õ���ǰ������
LONG GameFaction::GetExp(VOID)
{
	return m_FacBaseData.lCurrExp;
}
//! �õ���ǰ��Ա������
LONG GameFaction::GetMemberContribute(const CGUID &MemberGuid)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
		return ite->second.lContribute;
	return -1;
}

//! ������Դ��ֵ
VOID GameFaction::SetRes(LONG lRes)
{
	m_FacBaseData.lCurrRes = lRes;
}
//! ���þ���
VOID GameFaction::SetExp(LONG lExp)
{
	m_FacBaseData.lCurrExp = lExp;
}
//! ���ó�Ա����
VOID GameFaction::SetMemberContribute(const CGUID &MemberGuid, LONG lContribute)
{
	map<CGUID, tagFacMemInfo>::iterator ite = m_mapMemInfo.find(MemberGuid);
	if(m_mapMemInfo.end() != ite)
	{
		ite->second.lContribute = lContribute;
	}
}

void GameFaction::SetTickets(char* cBaseName,long lNum)
{
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.find(cBaseName);
	if (it!=m_mapBaseInfo.end())
	{
		it->second._lTickets=lNum;
	}
	else
	{
		if (lNum<0)
			return;
		//��һ�������Ϣ
		tagBaseInfo tgInfo;
		tgInfo._lTickets=lNum;
		tgInfo._FacGuid=NULL_GUID;
		strcpy_s(tgInfo._cBaseName, 32, cBaseName);
		m_mapBaseInfo[cBaseName]=tgInfo;
	}
}

void GameFaction::SetBaseBattle(char* cBaseName,long lNum)
{
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.find(cBaseName);
	if (it!=m_mapBaseInfo.end())
	{
		it->second._lBattleVal=lNum;
	}
	else
	{
		if (lNum<0)
			return;
		//��һ�������Ϣ
		tagBaseInfo tgInfo;
		tgInfo._lBattleVal=lNum;
		tgInfo._FacGuid=NULL_GUID;
		strcpy_s(tgInfo._cBaseName, 32, cBaseName);
		m_mapBaseInfo[cBaseName]=tgInfo;
	}
}

LONG GameFaction::GetBaseBattle(char* cBaseName)
{
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.find(cBaseName);
	if (it!=m_mapBaseInfo.end())
	{
		return it->second._lBattleVal;
	}
	return 0;
}

void GameFaction::AddBattle(LONG lValue,CGUID PlayerGuid)
{

	if (PlayerGuid==NULL_GUID)
	{
		m_FacBaseData.lFacBattle=lValue;
		//���¹�������
		CMessage msg(MSG_S2C_Faction_BaseData_Notify);
		AddBaseData_ToMsgBack(&msg);
		SendToAllConcern(msg);	
	}
	else
	{
		MEMBERITER it=m_mapMemInfo.find(PlayerGuid);
		if (it!=m_mapMemInfo.end())
		{	
			it->second.lBattle=lValue;
			//���³�Ա����
			CMessage msg(MSG_S2C_Faction_ChangeBattle_Notify);
			msg.Add(it->second.MemberGuid);
			msg.Add(lValue);
			SendToAllConcern(msg);
		}	
	}	
}

void GameFaction::AddArmyInvest(LONG lValue,CGUID PlayerGuid)
{
	if (PlayerGuid==NULL_GUID)
	{
		m_FacBaseData.lFacArmyInvest=lValue;
		//���¹�������
		CMessage msg(MSG_S2C_Faction_BaseData_Notify);
		AddBaseData_ToMsgBack(&msg);
		SendToAllConcern(msg);
	}
	else
	{
		MEMBERITER it=m_mapMemInfo.find(PlayerGuid);
		if (it!=m_mapMemInfo.end())
		{	
			it->second.lArmyInvest=lValue;
			//���³�Ա����
			CMessage msg(MSG_S2C_Faction_ChangeArmyInvest_Notify);
			msg.Add(it->second.MemberGuid);
			msg.Add(lValue);
			SendToAllConcern(msg);
		}
		
	}
}

void GameFaction::SetJoin(CGUID& PlayerGuid,LONG lFlags)
{
	MEMBERITER it=m_mapMemInfo.find(PlayerGuid);
	if (it!=m_mapMemInfo.end())
	{
		it->second.bJoinBattle=(BOOL)lFlags;
		//���³�Ա����
		CMessage msg(MSG_S2C_Faction_ChangeJoinBattle_Notify);
		msg.Add(it->second.MemberGuid);
		msg.Add((BYTE)lFlags);
		AddMember_ToMsgBack(&msg);
		SendToAllConcern(msg);
	}	
}

BOOL GameFaction::GetJoin(const CGUID& guid)
{
	MEMBERITER it=m_mapMemInfo.find(guid);
	if (it!=m_mapMemInfo.end())
	{
		return it->second.bJoinBattle;
	}
	return false;
}

LONG GameFaction::GetTickets(char* cBaseName)
{
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.find(cBaseName);
	if (it!=m_mapBaseInfo.end())
	{
		return it->second._lTickets;
	}
	return 0;
}

LONG GameFaction::GetBattle(CGUID guid/* =NULL_GUID */)
{
	if (guid==NULL_GUID)
	{
		return m_FacBaseData.lFacBattle;
	}
	else
	{
		MEMBERITER it=m_mapMemInfo.find(guid);
		if (it!=m_mapMemInfo.end())
		{
			return it->second.lBattle;
		}
	}
	return 0;

}

LONG GameFaction::GetArmyInvest(CGUID guid/* =NULL_GUID */)
{
	if (guid==NULL_GUID)
	{
		return m_FacBaseData.lFacArmyInvest;
	}
	else
	{
		MEMBERITER it=m_mapMemInfo.find(guid);
		if (it!=m_mapMemInfo.end())
		{
			return it->second.lArmyInvest;
		}
	}
	return 0;
}

BOOL GameFaction::ChangeEnemy(eBattleType eType,char* cName,DWORD dwTime,DWORD dwEndTime,LONG lFlags/* =0 */)
{
	map<eBattleType,list<tagEnemy>>::iterator it=m_Enemy.find(eType);
	//ɾ��ָ������������Ϣ
	if (lFlags==0)
	{
		if (it==m_Enemy.end())
			return FALSE;
		list<tagEnemy>& Enemy=it->second;
		list<tagEnemy>::iterator itItr=Enemy.begin();
		for (;itItr!=Enemy.end();itItr++)
		{
			if (stricmp(cName,(*itItr)._cName)==0)
			{
				Enemy.erase(itItr);
				return TRUE;
			}
		}
		return FALSE;
	}
	//�����Ϣ
	tagEnemy tgEnemy;
	strcpy_s(tgEnemy._cName, MAXSIZE_FactionName, cName);
	tgEnemy._dwStartTime=dwTime;
	tgEnemy._dwEndTime=dwEndTime;	
	if (it!=m_Enemy.end())
	{
		list<tagEnemy>& Enemy=it->second;
		Enemy.push_back(tgEnemy);
	}
	else
	{
		list<tagEnemy>Enemy;
		Enemy.push_back(tgEnemy);
		m_Enemy[eType]=Enemy;
	}
	//����ս����Ϣ
	CMessage msg(MSG_S2C_Faction_ChangeEnemy_Notify);
	AddEnemyInfoToMsg(&msg);
	SendToAllConcern(msg);
	return TRUE;
}

BOOL GameFaction::AddBaseInfoToMsg(CMessage* pMsg)
{
	pMsg->Add((long)m_mapBaseInfo.size());
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.begin();
	for (;it!=m_mapBaseInfo.end();it++)
	{
		pMsg->AddEx(&(it->second),sizeof(tagBaseInfo));
	}
	return TRUE;
}

BOOL GameFaction::AddEnemyInfoToMsg(CMessage* pMsg)
{
	map<eBattleType,list<tagEnemy>>::iterator it=m_Enemy.begin();
	pMsg->Add((long)m_Enemy.size());
	for (;it!=m_Enemy.end();it++)
	{
		pMsg->Add((BYTE)it->first);
		list<tagEnemy>& Enemy=it->second;
		pMsg->Add((long)Enemy.size());
		list<tagEnemy>::iterator itItr=Enemy.begin();
		for (;itItr!=Enemy.end();itItr++)
		{
			pMsg->AddEx(&(*itItr),sizeof(tagEnemy));
		}
	}
	return TRUE;
}

BOOL GameFaction::ChangeBaseInfo(char* cName)
{
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.find(cName);
	if (it!=m_mapBaseInfo.end())
	{
		it->second._FacGuid=m_FacPublicData.guid;
		return TRUE;
	}
	else
		return FALSE;
}

void GameFaction::SendBaseInfo()
{
	//֪ͨ�򿪼������Ŀͻ���
	CMessage msg(MSG_S2C_Faction_Beachheads_Notify);
	AddBaseInfoToMsg(&msg);
	SendToAllConcern(msg);
}

LONG GameFaction::GetBaseNum()
{
	LONG lBaseNum=0;
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.begin();
	for(;it!=m_mapBaseInfo.end();it++)
	{
		if(it->second._FacGuid==m_FacPublicData.guid)
			lBaseNum+=1;
	}
	return lBaseNum;
}

void GameFaction::FacDisplay(const CGUID& guid,long lValue1,long lValue2)
{
	map<CGUID, tagFacMemInfo>::iterator it=m_mapMemInfo.find(guid);
	if (it==m_mapMemInfo.end())
		return;
	CMessage msg(MSG_S2C_Faction_OpenGUI);
	msg.Add(lValue1);
	msg.Add(lValue2);
	msg.SendToPlayer(guid);	
}

void GameFaction::ClearBase() 
{
	map<string,tagBaseInfo>::iterator it=m_mapBaseInfo.begin();
	for (;it!=m_mapBaseInfo.end();it++)
	{
		it->second._FacGuid=NULL_GUID;
		it->second._lBattleVal=0;
		it->second._lTickets=0;
	}
	SendBaseInfo();
}