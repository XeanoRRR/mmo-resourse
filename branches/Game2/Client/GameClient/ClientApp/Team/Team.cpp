#include "stdafx.h"
#include "Team.h"
#include "../Player.h"
#include "../PhaseSys/FollowLeader.h"
#include "../PhaseSys/PhaseSys.h"
#include "../Other/StateListXml.h"
#include "../../Game.h"
#include "../../../Net/Message.h"
#include "../../../ui/UIDef.h"
#include "../../../ui/Windows/TeamDef.h"
class CGUID;
TeamData::TeamData()
{
	CurSelMemberIndex = -1;
	Clear();
}

TeamData::~TeamData()
{
	Clear();
}
void TeamData::AddGUIDSrc(CGUID guid)
{
	m_vecIDSrc.push_back(guid);
}

void TeamData::AddMemberState(const CGUID& PlayerID, DWORD dwState, DWORD dwLev)
{
	for(uint i=0; i<m_vectorTeam.size(); ++i)
	{
		// ��ID�Ķ��Ѵ���
		if(m_vectorTeam[i].PlayerID == PlayerID)
		{
			// ����״̬ID��״̬�б��в�����Ӧ��״̬
			CStateListXml::tagStateList * pStateList = CStateListXml::GetState(dwState);
			if (pStateList)
			{
				// ��״̬�Ѵ��ڣ���������
				for(uint j=0; j<m_vectorTeam[i].m_vectorState.size(); ++j)
				{
					if(m_vectorTeam[i].m_vectorState[j].lID == dwState)
						return;
				}
				tagState st;
				st.lID = dwState;
				st.lLevel = dwLev;
				st.lIconID = pStateList->lAfettleICON;
				if(pStateList->AfettleIsBuff==0)
					st.IsDeBuff = true;
				st.dwLeftTime = 0;
				st.dwAlpha = 0xffffffff;
				m_vectorTeam[i].m_vectorState.push_back(st);
			}
		}
	}
	FireUIUpdate();
}

bool TeamData::AddTeamMember(tagWSMemberInfo& sWSMemberInfo)
{
	// ��Ա��Ϣ�ṹ
	tagTeam sTeam;
	sTeam.strPlayerName = sWSMemberInfo.szName;
	sTeam.PlayerID = sWSMemberInfo.guid;
	sTeam.lSex = sWSMemberInfo.lSex;
	CPlayer* player = GetGame()->GetMainPlayer();
	//�ӳ��ŵ�һλ
	if( sWSMemberInfo.guid == player->GetCaptainID())
	{
		m_vectorTeam.insert(m_vectorTeam.begin(),sTeam);
		FireUIUpdate();
		return true;
	}
	else
	{
		m_vectorTeam.push_back(sTeam);
		FireUIUpdate();
		return true;
	}
}

void TeamData::Clear()
{
	m_vecIDSrc.clear();
	m_vecSrcName.clear();
	m_vectorTeam.clear();
}

bool TeamData::ChangeCaptain(const CGUID& CapID)
{
	CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
	if(pMainPlayer->GetCaptainID() == CapID)
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_TEAM,eIDS_TEAM_CAPED23));
		return false;
	}
	// �Լ������óɶӳ�
	if(pMainPlayer->GetExID() == CapID)
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_TEAM,eIDS_TEAM_CAPSELF));
		FireUIUpdate();
		return true;
	}
	vector<tagTeam>::iterator it = m_vectorTeam.begin();
	for(; it!=m_vectorTeam.end(); ++it)
	{
		// ���Ѵ���
		if((*it).PlayerID == CapID)
		{
			tagTeam sTeam = (*it);
			m_vectorTeam.erase(it);
			m_vectorTeam.insert(m_vectorTeam.begin(),sTeam);
			/////////////////////////////////////////////////
			// zhaohang  2010/6/17 
			////////////////////////////////////////////////
			FireUIUpdate();
			return true;
		}
	}
	return false;
}

void TeamData::RemoveMemberState(const CGUID& PlayerID, DWORD dwState)
{
	for ( uint i = 0 ; i < m_vectorTeam.size() ; ++i)
	{
		//ָ�����Ѵ���
		if(m_vectorTeam[i].PlayerID == PlayerID )
		{
			//ָ��״̬����
			vector<tagState>::iterator iter = m_vectorTeam[i].m_vectorState.begin();
			for (; iter != m_vectorTeam[i].m_vectorState.begin() ; ++iter)
			{
				if( (*iter).lID == dwState)
				{
					m_vectorTeam[i].m_vectorState.erase(iter);
					return ;
				}
			}
		}
	}
}


bool TeamData::RemoveTeamMember(const CGUID& MemberID)
{
	vector<tagTeam>::iterator it = m_vectorTeam.begin();
	for(; it!=m_vectorTeam.end(); ++it)
	{
		// ���Ѵ���
		if((*it).PlayerID == MemberID)
		{
			(*it).m_vectorState.clear();
			m_vectorTeam.erase(it);
			FireUIUpdate();
			return true;
		}
	}
	return false;
}


void TeamData::MemberChangeServer(const CGUID& MemID)
{
	CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
	// �Լ����,�������ж�����Ϣ
	if(pMainPlayer->GetExID() == MemID)
	{
		for(uint i=0; i<m_vectorTeam.size(); ++i)
		{
			m_vectorTeam[i].dwHp = 0;
			m_vectorTeam[i].dwMp = 0;
			m_vectorTeam[i].dwRp = 0;
			m_vectorTeam[i].m_vectorState.clear();
		}
		FireUIUpdate();
	}
	else
	{
		for(uint i=0; i<m_vectorTeam.size(); ++i)
		{
			if(MemID == m_vectorTeam[i].PlayerID)
			{
				m_vectorTeam[i].dwHp = 0;
				m_vectorTeam[i].dwMp = 0;
				m_vectorTeam[i].dwRp = 0;
				m_vectorTeam[i].m_vectorState.clear();
				FireUIUpdate();
				return;
			}
		}
	}
}

void TeamData::UpdateMemberPos(const CGUID& MemberID, float fPosX,float fPosY)
{
	for(uint i=0; i<m_vectorTeam.size(); ++i)
	{
		// ��ID�Ķ��Ѵ���,��������
		if(m_vectorTeam[i].PlayerID == MemberID)
		{
			m_vectorTeam[i].fPosX = fPosX;
			m_vectorTeam[i].fPosY = fPosY;
		}
	}
}
bool TeamData::UpdateTeamList()
{
	//��ն����б�
	m_vectorTeam.clear();
	//��������ϻ�ȡ�����б�
	CPlayer* player = GetGame()->GetMainPlayer();
	vector<tagWSMemberInfo>::iterator it  = player->GetTeamMateInfo().begin();
	for(; it != player->GetTeamMateInfo().end() ; ++it)
	{
		// �Լ�����
		if((*it).guid == player->GetExID())
			continue;
		else
		{
			// ��ӳ�Ա,�ɹ��������Ա��ʾλ��
			if(AddTeamMember((*it)))
			{
				//UpdateTeamOrder();
			}
		}
	}
	FireUIUpdate();
	return true;
}


void TeamData::UpdateMemberInfo(const CGUID& MemberID, long lType, long lValue)
{
	for(uint i=0; i<m_vectorTeam.size(); ++i)
	{
		// ��ID�Ķ��Ѵ���
		if(m_vectorTeam[i].PlayerID == MemberID)
		{
			bool bIsHp = false;
			switch(lType)
			{
			case eMIS_CurrHp:
				bIsHp = true;
				m_vectorTeam[i].dwHp = lValue;
				break;
			case eMIS_CurrMp:
				m_vectorTeam[i].dwMp = lValue;
				break;
			case eMIS_CurrEnergy:
				if(m_vectorTeam[i].lIconID >= OCC_Paladin && m_vectorTeam[i].lIconID <= OCC_Prophet)
				{
					m_vectorTeam[i].dwRp = lValue;
				}
				else
				{
					return;
				}
				break;
			case eMIS_MaxHp:
				bIsHp = true;
				m_vectorTeam[i].dwMaxHp = lValue;
				break;
			case eMIS_MaxMp:
				m_vectorTeam[i].dwMaxMp = lValue;
				break;
			case eMIS_MaxEnergy:
				if(m_vectorTeam[i].lLev >= 80)
				{
					m_vectorTeam[i].dwMaxRp = lValue;
					if(g_bDebug)
					{	
						Log4c::Trace(ROOT_MODULE,"MaxCP=%d,CurCP=%d",lValue,m_vectorTeam[i].dwRp);
					}

					if(m_vectorTeam[i].dwMaxRp<m_vectorTeam[i].dwRp)
					{
						if(g_bDebug)
						{
							Log4c::Trace(ROOT_MODULE,"CPValueUpdata OK!");
						}
					}
				}
				else
				{
					return;
				}
				break;
			case eMIS_Level:
				m_vectorTeam[i].lLev = lValue;
				break;
			case eMIS_Occupation:
				// ����ְҵ�ı�
				if(m_vectorTeam[i].lIconID != lValue && lValue >= OCC_Novice && lValue < OCC_Max)
				{
					m_vectorTeam[i].lIconID = lValue;
				}
				break;
			case eMIS_Sex:
				break;
			case eMIS_State:
				break;
			}
			FireUIUpdate();
			return;
		}
	}
}

void TeamData::SetMemberInfo(const CGUID& MemberID, tagMemberIdioinfo sMemberinfo)
{
	for(uint i=0; i<m_vectorTeam.size(); ++i)
	{
		// ��ID�Ķ��Ѵ���
		if(m_vectorTeam[i].PlayerID == MemberID)
		{
			m_vectorTeam[i].dwMaxHp = sMemberinfo.lMaxHp;
			m_vectorTeam[i].dwMaxMp = sMemberinfo.lMaxMp;
			m_vectorTeam[i].dwMaxRp = sMemberinfo.lMaxEnergy;

			m_vectorTeam[i].dwHp = sMemberinfo.lCurrHp;
			m_vectorTeam[i].dwMp = sMemberinfo.lCurrMp;
			m_vectorTeam[i].dwRp = sMemberinfo.lCurrEnergy;

			m_vectorTeam[i].lIconID = sMemberinfo.lOccupation;
			m_vectorTeam[i].lRegionID = sMemberinfo.lRegionID;
			m_vectorTeam[i].RegionGuid = sMemberinfo.RegionGuid;
			m_vectorTeam[i].lLev = sMemberinfo.lLevel;
			m_vectorTeam[i].fPosX = sMemberinfo.fPosX;
			m_vectorTeam[i].fPosY = sMemberinfo.fPosY;
			FireUIUpdate();
			return;
		}
	}
}

void TeamData::SetMemberIsOnline(const CGUID& PlayerID, long lState)
{
	for(uint i=0; i<m_vectorTeam.size(); ++i)
	{
		// �ö��Ѵ���
		if(m_vectorTeam[i].PlayerID == PlayerID)
		{
			// ����
			if(lState == eMS_LEAVE)
			{
				m_vectorTeam[i].dwHp = 0;
				m_vectorTeam[i].dwMp = 0;
				m_vectorTeam[i].dwRp = 0;
				m_vectorTeam[i].lRegionID = 0;
				m_vectorTeam[i].RegionGuid = CGUID::GUID_INVALID;
				m_vectorTeam[i].bIsOnline = false;
				m_vectorTeam[i].m_vectorState.clear();
			}
			// ����
			else
				m_vectorTeam[i].bIsOnline = true;
			FireUIUpdate();
			return;
		}
	}
}
const CGUID& TeamData::GetSelMemberGUID()
{
	return m_vectorTeam[GetSelMember()].PlayerID;
}
void TeamData::FireUIUpdate()
{
	//֪ͨ����
	FireUIEvent(TEAM_PAGE_NAME,"UpdateTeam");
}
/////////////////////////////////////////////////
//TeamMsgMgr
void TeamMsgMgr::SendInviteJoinTeam(CPlayer* first,CPlayer* second)
{
	if ( !first || !second ) 
		return ;

	CMessage  msg(MSG_C2S_TEAM_QUERY_JOIN);
	msg.Add( second->GetExID() );
	msg.Add( first->GetExID());
	msg.Send();
}
void TeamMsgMgr::SendLeaveTeam(const CGUID& guid)
{
	if(guid == CGUID::GUID_INVALID)
		return;
	CMessage  msg(MSG_C2S_TEAM_KICK_PLAYER);
	msg.Add(guid);
	msg.Send();
}
void TeamMsgMgr::SendLeaveTeam(CPlayer* first)
{
	SendLeaveTeam(first->GetExID());
}

void TeamMsgMgr::SendSelfLeaveTeam()
{
	CPlayer* player = GetGame()->GetMainPlayer();
	if(player)
		SendLeaveTeam(player);
}

void TeamMsgMgr::SendInviteJoinPhase(CPlayer* first,CPlayer* second)
{
	//GetInst(FollowLeaderManager).FollowOper(first,TF_Team_Member_Follow_Leader);
}

void TeamMsgMgr::SendAddLinkMan(CPlayer* first,CPlayer* second)
{
	CMessage  msg(MSG_C2S_LINKMAN_ADD);

}

void TeamMsgMgr::SendSetCaptain(const CGUID& guid)
{
	if(guid == CGUID::GUID_INVALID)
		return ;
	//�ӳ���Ϣ
	CMessage msg(MSG_C2S_TEAM_CHANGE_LEADER);
	msg.Add(guid);// ������Ŀ��
	msg.Send();
}
void TeamMsgMgr::SendFollowLeader(CPlayer* first)
{
	//if( first->GetTeamID() != NULL_GUID )  
	// GetInst(FollowLeaderManager).FollowOper(first,TF_Team_Stop_TeamFollow);

	if( first->GetTeamID() != NULL_GUID )
	{
		GetInst(FollowLeaderManager).FollowOper(first,TF_Team_Member_Follow_Leader);
	}
}