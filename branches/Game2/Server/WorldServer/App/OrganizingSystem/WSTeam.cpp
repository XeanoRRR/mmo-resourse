#include "StdAfx.h"
#include "WSTeam.h"

#include "OrganizingCtrl.h"
#include "../Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WSTeam::tagDefaultSetup WSTeam::s_tagDefaultSetup(eGA_FREE, 6, 10);

WSTeam::tagDefaultSetup::tagDefaultSetup(long lGA, long lMMN, long lSTS)
:
lGoodsAllot(lGA),
lMaxMemberNum(lMMN),
lSessionTimeoutS(lSTS)
{

}

WSTeam::WSTeam()
:
m_MemberList(0),
m_ShareQuestList(0),
m_lCurrShareQuestID(0),
m_dwAcceptTime(0),
m_eGoodsAllot(s_tagDefaultSetup.lGoodsAllot)
{
}

WSTeam::~WSTeam(void){}


//��ʼ������
bool WSTeam::Initial(const CGUID& TeamGuid, const CGUID& MastterGuid, const CGUID& MemberGuid)
{
	CPlayer *pMastter = GetGame().GetMapPlayer(MastterGuid);
	CPlayer *pMember = GetGame().GetMapPlayer(MemberGuid);
	//�Ҳ������߽�ɫ
	if (NULL == pMastter || NULL == pMember)
	{
		return false;
	}
	
	SetExID(TeamGuid);
	m_MestterGuid	= MastterGuid;
	m_eGoodsAllot	= eGA_QUEUE;

	GetOrganiCtrl()->PlayerJoinTeam(MastterGuid, TeamGuid);
	GetOrganiCtrl()->PlayerJoinTeam(MemberGuid, TeamGuid);
	pMastter->SetTeamID(TeamGuid);
	pMember->SetTeamID(TeamGuid);

	//�ӳ�
	tagWSMemberInfo MemberInfo;
	MemberInfo.guid			= MastterGuid;
	MemberInfo.lState		= eMS_ONLINE;
	MemberInfo.lOccupation	= pMastter->GetOccupation();
	MemberInfo.lSex			= pMastter->GetSex();
	MemberInfo.dwJoinTime	= (ulong)time(NULL);
	MemberInfo.lCompleteQuest = 0;
	memmove(MemberInfo.szName, pMastter->GetName(), 20); 
	m_MemberList.push_back(MemberInfo);
	GetOrganiCtrl()->PlayerJoinTeam(MastterGuid, TeamGuid);

	//��һ����Ա
	MemberInfo.guid			= MemberGuid;
	MemberInfo.lState		= eMS_ONLINE;
	MemberInfo.lOccupation	= pMember->GetOccupation();
	MemberInfo.lSex			= pMember->GetSex();
	MemberInfo.dwJoinTime	= (ulong)time(NULL);
	MemberInfo.lCompleteQuest = 0;
	memmove(MemberInfo.szName, pMember->GetName(), 20);
	m_MemberList.push_back(MemberInfo);
	GetOrganiCtrl()->PlayerJoinTeam(MemberGuid, TeamGuid);

	CMessage msg(MSG_W2S_TEAM_CREATE);
	PushBackToMsg(msg);
	msg.SendToAllGS();

	GetLogicLogInterface()->LogT340_team_create(this, pMastter, pMember);
	return true;
}

//! �������Ķ���������ӵ���Ϣĩβ
void WSTeam::PushBackToMsg(CMessage &msg)
{
	msg.Add(GetExID());
	msg.Add(m_MestterGuid);
	msg.Add(m_eGoodsAllot);
	msg.Add((long)m_MemberList.size());
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		msg.AddEx(&(*ite), sizeof(tagWSMemberInfo));
	}
}

const CGUID& WSTeam::SetNewMastter()
{
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		if(ite->guid != m_MestterGuid && ite->lState == eMS_ONLINE)
		{	
			m_MestterGuid = ite->guid;
			return ite->guid;
		}
	}
	return NULL_GUID;
}

void WSTeam::SetGoodsAllot(long eSet)
{
	if (eSet >= eGA_FREE && eSet <= eGA_RANDOM)
	{
		m_eGoodsAllot = eSet;
		CMessage msg(MSG_W2S_TEAM_GoodsSetChange);
		msg.Add(GetExID());
		msg.Add(m_eGoodsAllot);
		msg.SendToAllGS();
	}
}

bool WSTeam::AddToByteArray(vector<BYTE>& pByteArray)
{
	_AddToByteArray(&pByteArray, GetExID());
	_AddToByteArray(&pByteArray, m_MestterGuid);
	_AddToByteArray(&pByteArray, m_eGoodsAllot);
	_AddToByteArray(&pByteArray, (LONG)m_MemberList.size());
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		_AddToByteArray(&pByteArray, &(*ite), sizeof(tagWSMemberInfo));
	}
	return true;
}

//����Ҫ������֯����Ա
bool WSTeam::DoJoin(const CGUID& ExecuteGuid,const CGUID& BeExecutedGuid,long lAgree,tagTime& Time)
{
	if(NULL_GUID == IsMaster(ExecuteGuid)) return false;
	CPlayer *pMember = GetGame().GetMapPlayer(BeExecutedGuid);
	//�Ҳ������߽�ɫ
	if (NULL == pMember || FindMember(pMember->GetExID()) != m_MemberList.end())
	{
		return false;
	}
	
	GetOrganiCtrl()->PlayerJoinTeam(BeExecutedGuid, GetExID());
	pMember->SetTeamID(GetExID());

	tagWSMemberInfo MemberInfo;
	MemberInfo.guid			= BeExecutedGuid;
	MemberInfo.lState		= eMS_ONLINE;
	MemberInfo.lOccupation	= pMember->GetOccupation();
	MemberInfo.lSex			= pMember->GetSex();
	MemberInfo.dwJoinTime	= (ulong)time(NULL);
	MemberInfo.lCompleteQuest = 0;
	memmove(MemberInfo.szName, pMember->GetName(), 20);
	m_MemberList.push_back(MemberInfo);
	GetOrganiCtrl()->PlayerJoinTeam(BeExecutedGuid, GetExID());

	CMessage msg(MSG_W2S_TEAM_PlayerJoin);
	msg.Add(GetExID());
	msg.AddEx(&MemberInfo, sizeof(tagWSMemberInfo));
	msg.SendToAllGS();

	return true;
}

//������Ա����
bool WSTeam::Invite(const CGUID& InvitedGuid, const CGUID& BeInvitedGuid)
{
	if(NULL_GUID != IsMaster(InvitedGuid)) return false;
	CPlayer *pMember = GetGame().GetMapPlayer(BeInvitedGuid);
	//�Ҳ������߽�ɫ
	if (NULL == pMember)
	{
		return false;
	}

	assert(false);return true;
}

//�˳�
bool WSTeam::Exit(const CGUID& PlayerGuid)
{
	list<tagWSMemberInfo>::iterator ite = FindMember(PlayerGuid);
	if(ite == m_MemberList.end()) return false;
	GetOrganiCtrl()->PlayerLeaveTeam(PlayerGuid);
	OnExit( (*ite).szName,PlayerGuid);
	CMessage msg(MSG_W2S_TEAM_MemberExit);
	msg.Add(GetExID());
	msg.Add(PlayerGuid);
	msg.SendToAllGS();
	m_MemberList.erase(ite);
	return true;
}

//
void WSTeam::OnExit(const char* szname,const CGUID& PlayerGuid)
{
// 	list<tagShareQuestList>::iterator it=m_ShareQuestList.begin();
// 	for (;it!=m_ShareQuestList.end();it++)
// 	{
// 		if ( strcmp((*it).szName,szname)==0 )
// 		{
// 			m_ShareQuestList.erase(it);
// 			break;
// 		}
// 	}
// 	long lGameServerID = GetGame().GetGameServerNumber_ByPlayerID(PlayerGuid);
// 	CMessage msg(MSG_W2S_SHARE_QUESTLIST_UPDATE);
// 	msg.Add(m_guid);
// 	msg.Add(m_dwAcceptTime);
// 	msg.Add( (long)m_ShareQuestList.size() );
// 	it=m_ShareQuestList.begin();
// 	for (;it!=m_ShareQuestList.end();it++)
// 	{
// 		msg.Add( (*it).lQuestID );
// 		msg.Add( (*it).szName );
// 	}
// 	msg.SendToGS(lGameServerID);
}

list<tagWSMemberInfo>::iterator WSTeam::FindMember(const CGUID& PlayerGuid)
{
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		if(ite->guid == PlayerGuid) return ite;
	}
	return m_MemberList.end();
}

bool WSTeam::PlayerInTeam(const CGUID& PlayerGuid)
{
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		if(ite->guid == PlayerGuid) return true;
	}
	return false;
}


list<CGUID>	WSTeam::GetMemberIDList()
{
	list<CGUID>  memberIDList;
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		memberIDList.push_back(ite->guid);
	}

	return memberIDList;
}

//void WSTeam::UpdateTeamInfoToGS(void)
//{
//
//}

//����
bool WSTeam::FireOut(const CGUID& FireID,const CGUID& BeFiredID)
{

	assert(false);return true;
}

//��λ
bool WSTeam::Demise(const CGUID& MasterID,const CGUID& guid)
{
	this->m_MestterGuid = guid;

	CMessage msg(MSG_W2S_TEAM_MastterChange);
	msg.Add(GetExID());
	msg.Add(guid);
	msg.SendToAllGS();
	return true;
}

//��ɢ
bool WSTeam::Disband(const CGUID& guid)
{
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		GetOrganiCtrl()->PlayerLeaveTeam(ite->guid);
	}
	CMessage msg(MSG_W2S_TEAM_Disband);
	msg.Add(GetExID());
	msg.SendToAllGS();

	GetLogicLogInterface()->LogT370_team_disband(this);
	return true;
}

//�õ���֯��ID
//long WSTeam::GetID()
//{
//	return m_lID;
//}

const	CGUID& WSTeam::GetExID()
{
	return m_guid;
}

void	WSTeam::SetExID(const CGUID& guid)
{
	m_guid = guid;
}

const	CGUID& WSTeam::GetMasterID()
{
	return m_MestterGuid;
}

//�õ���Ա�б�
void WSTeam::GetMemberList(list<CGUID>& TemptList)
{
	assert(false);
}

//������ǰ�����
const CGUID& WSTeam::IsMaster(const CGUID& guid)
{
	if(guid == m_MestterGuid) 
		return guid;
	else
		return NULL_GUID;
}

//������Ƿ��Ǹð��Ա��
const CGUID& WSTeam::IsMember(const CGUID& guid)
{
	for (list<tagWSMemberInfo>::iterator ite = m_MemberList.begin(); ite != m_MemberList.end(); ++ite)
	{
		if (ite->guid == guid)
		{
			return guid;
		}
	}
	return NULL_GUID;
}

//�õ���Ա��
long WSTeam::GetMemberNum()
{
	return (long)m_MemberList.size();
}
//! �õ����߳�Ա������
long WSTeam::GetOnlineMemberNum(void)
{
	long lReNum = 0;
	list<tagWSMemberInfo>::iterator ite = m_MemberList.begin();
	for (;m_MemberList.end() != ite; ++ite)
	{
		if (eMS_ONLINE == ite->lState)
		{
			++lReNum;
		}
	}
	return lReNum;
}

//�õ�ͷ��
const CGUID& WSTeam::GetPlayerHeader()
{
	return m_MestterGuid;
}

//������ҵ���Ϣ����֯�ĳ�Ա�б�
bool WSTeam::UpdatePlayerDataToOrgMemberList(const CGUID& PlayerGuid)
{
	assert(false);return false;
}

//! �ж϶�Ա�����ȼ����Ƿ�������ķ�Χ��
BOOL WSTeam::InLvlDifference(LONG lMaxLvlSpace)
{
	long lMinLvl = 9999999, lMaxLvl = 0;
	list<tagWSMemberInfo>::iterator ite = m_MemberList.begin();
	for (;m_MemberList.end() != ite; ++ite)
	{
		CPlayer *pPlayer = GetGame().GetMapPlayer(ite->guid);
		if(NULL == pPlayer)
			return FALSE;
		if(pPlayer->GetLevel() < lMinLvl)
			lMinLvl = pPlayer->GetLevel();
		if(pPlayer->GetLevel() > lMaxLvl)
			lMaxLvl = pPlayer->GetLevel();
	}

	return (lMaxLvlSpace >= (lMaxLvl - lMinLvl));
}

//! �ж����ж�Ա�ĵȼ��Ƿ��������Χ��
BOOL WSTeam::InLvlSpace(LONG lMin, LONG lMax)
{
	list<tagWSMemberInfo>::iterator ite = m_MemberList.begin();
	for (;m_MemberList.end() != ite; ++ite)
	{
		CPlayer *pPlayer = GetGame().GetMapPlayer(ite->guid);
		if(NULL == pPlayer)
			return FALSE;
		if(pPlayer->GetLevel() < lMin)
			return FALSE;
		if(pPlayer->GetLevel() > lMax)
			return FALSE;
	}
	return TRUE;
}

//����һ����Ա������Ϸ
void WSTeam::OnMemberEnterGame(const CGUID& MemGuid)
{
	list<tagWSMemberInfo>::iterator ite = FindMember(MemGuid);
	if (ite == m_MemberList.end())
	{
		return;
	}
	ite->lState = eMS_ONLINE;
	CMessage msg(MSG_W2S_TEAM_MemberStateChange);
	msg.Add(GetExID());
	msg.Add(ite->guid);
	msg.Add((long)eMS_ONLINE);
	msg.SendToAllGS();
}

//����һ����Ա�뿪��Ϸ
void WSTeam::OnMemberExitGame(const CGUID& MemGuid)
{
	list<tagWSMemberInfo>::iterator ite = FindMember(MemGuid);
	if (ite == m_MemberList.end())
	{
		return;
	}

	//�ӳ����ߣ���λ
	if (MemGuid == GetMasterID())
	{
		const CGUID& MastterGuid = SetNewMastter();
		if (NULL_GUID != MastterGuid)
		{
			CMessage msgMC(MSG_W2S_TEAM_MastterChange);
			msgMC.Add(GetExID());
			msgMC.Add(MastterGuid);
			msgMC.SendToAllGS();
		}
	}

	ite->lState = eMS_LEAVE;
	CMessage msg(MSG_W2S_TEAM_MemberStateChange);
	msg.Add(GetExID());
	msg.Add(ite->guid);
	msg.Add((long)eMS_LEAVE);
	msg.SendToAllGS();
}

//��Ա�ȼ������˱仯
void WSTeam::OnMemberLvlChange(const CGUID& MemGuid,long lLvl)
{
	assert(false);
}

//��Ա�ı���λ��(����)
void WSTeam::OnMemberPosChange(const CGUID& MemGuid,long lNewPosID)
{
	assert(false);
}

//��ʼ��
bool WSTeam::Initial()
{
	assert(false);return false;
}

void WSTeam::UpdateShareQuestList(long lGSID,tagShareQuestList* quest,bool flag,CPlayer* player)
{
	if (flag) 
	{
		m_ShareQuestList.clear();
	}
    if( !player ) return;
	CMessage msg(MSG_W2S_SHARE_QUESTLIST_UPDATE);
	msg.Add(m_guid);
	msg.Add(m_dwAcceptTime);
    msg.Add( quest->lQuestID );  
    msg.Add( quest->szName );   
    msg.Add( player->GetExID() );

    msg.SendToAllGS();
}
	
void WSTeam::SetCurrShareQuest(long lGSID,long questID)
{
	m_dwAcceptTime = (DWORD) time(NULL);
	m_lCurrShareQuestID=questID;
	ClearMemberQuest();
	CMessage msg(MSG_W2S_SHARE_CURRQUEST);
	msg.Add(this->GetExID());
	msg.Add(m_dwAcceptTime);
	msg.Add(m_lCurrShareQuestID);
	msg.SendToGS(lGSID);
}
bool WSTeam::FindShareQuest(long lquest)
{
	if (0 == lquest)
	{
		return true;
	}
	list<tagShareQuestList>::iterator it=m_ShareQuestList.begin();
	for (;it!=m_ShareQuestList.end();it++)
	{
		if ( (*it).lQuestID == lquest )
			return true;
	}
	return false;
}
bool WSTeam::DelCurrShareQuest(long lGSID,char* name)
{
	if (name == NULL)
	{
		return false;
	}
	list<tagShareQuestList>::iterator it=m_ShareQuestList.begin();
	for (;it!=m_ShareQuestList.end();it++)
	{
		if ( 0 == strcmp((*it).szName,name))
		{
			m_ShareQuestList.erase(it);
			UpdateShareQuestList(lGSID,NULL);
			break;
		}
	}
	return false;
}

void WSTeam::SetMemberQuest(long lGSID,CGUID &guid,long lQuestID)
{
	if (lQuestID!=m_lCurrShareQuestID)
		return;
	list<tagWSMemberInfo>::iterator it=m_MemberList.begin();
	for (;it!=m_MemberList.end();it++)
	{
		if ( it->guid == guid )
		{
			it->lCompleteQuest=lQuestID;
			CMessage msg(MSG_W2S_SHARE_SETMEMBERQUEST);
			msg.Add(guid);//���GUID
			msg.Add(m_guid);
			msg.Add(lQuestID);
			msg.SendToGS(lGSID);
			break;
		}
	}
}
//�����ж����ѳɵĹ���������0
void WSTeam::ClearMemberQuest()
{
	list<tagWSMemberInfo>::iterator it=m_MemberList.begin();
	for (;it!=m_MemberList.end();it++)
	{
		it->lCompleteQuest = 0;
	}
}

