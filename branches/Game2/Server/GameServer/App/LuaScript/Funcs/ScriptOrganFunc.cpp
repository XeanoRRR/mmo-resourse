///
/// @file ScriptOrganFunc.cpp
/// @brief �μ� ScriptOrganFunc.h
///
#include "stdafx.h"
#include "../Script.h"
#include "../../Player.h"
#include "../../OrganizingSystem/GameOrganizingCtrl.h"
#include "../../OrganizingSystem/GSTeam.h"
#include "../../Country/CountryHandler.h"

long Script::GetTeamNum()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return -1;

	const CGUID& teamGuid = player->GetTeamID();
	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(teamGuid);
	if(team==NULL)
		return 0;

	return team->GetMemberNum();
}

long Script::GetTeamNumInRgn()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return -1;

	const CGUID& teamGuid = player->GetTeamID();

	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(teamGuid);
	if (team==NULL)
		return 0;

	return team->GetMemberNumInRgn(player->GetRegionGUID());
}

const char* Script::GetTeamerName(int pos)
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return NULL;

	const CGUID& teamGuid = player->GetTeamID();
	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(teamGuid);
	if(team==NULL)
		return NULL;

	list<CGUID> members;
	team->GetMemberList(members);
	list<CGUID>::iterator it = members.begin();
	for(int i = 0; it != members.end(); ++it, ++i)
	{
		if(i != pos)
			continue;

		CPlayer* player = GetGame()->FindPlayer(*it);
		if (player==NULL)
			return NULL;
		return player->GetName();
	}
	return NULL;
}

CGUID Script::GetTeamGUID()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return NULL_GUID;
	return player->GetTeamID();
}

CGUID Script::GetPlayerTeamGUID(const CGUID& playerGuid)
{
	if(m_RunEnv.srcShape==NULL)
		return NULL_GUID;

	CServerRegion* region=dynamic_cast<CServerRegion*>(m_RunEnv.srcShape->GetFather());
	if(region==NULL)
		return NULL_GUID;

	CPlayer* player= dynamic_cast<CPlayer*>(region->FindChildObject(TYPE_PLAYER, playerGuid));
	if(player==NULL)
		return NULL_GUID;

	return player->GetTeamID();
}

bool Script::IsTeamCaptain()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return false;

	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
	if(team==NULL)
		return false;

	if(team->GetLeader() == player->GetExID())
		return true;
	else
		return false;
}

long Script::GetTeamCaptainPos()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return -1;

	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
	if(team==NULL)
		return -1;
	const CGUID& captainId = team->GetLeader();

	list<CGUID> members;
	team->GetMemberList(members);
	list<CGUID>::iterator it = members.begin();
	for(int pos = 0; it != members.end(); ++it, ++pos)
	{
		if(captainId != *it)
			continue;
		return pos;
	}

	return -1;
}

long Script::SetRegionForTeam(int type, int rgnType, const CGUID& regionGuid, long x, long y, long dir, long range)
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return 0;

	// ����
	// 0�ɹ�  ʧ�ܣ� 1 û�ж���

	//���鴫������ 
	// 0 7x7��   1 ͬ����  2 ͬ������  3 ͬ����

	if(dir == -1)
		dir = player->GetDir();

	if(range==-1)
		range = 2;


	//��ȡ��������
	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player-> GetTeamID());

	// û�ж���
	if(team==NULL)
		return 1; // û�ж��飬����ʧ�ܡ�

	switch(type)
	{
	case 0:
		{
			//* 7x7�ڴ��� *//
			// �ö����Ƿ���ͬһ������
			if(team->GetCurrentServerTeamatesAmount() != team->GetTeamatesAmount())
				return 2; //���Ѳ��Ƕ���һ���������

			// ���Ѷ���һ����������
			// �ö���ÿ�������Ƿ���ͬһ����
			//bool bIsSameRegion = true;
			if(team->GetCurrentRegionTeamatesAmount(player->GetRegionGUID()) != team->GetTeamatesAmount())
				return 3; // ���Ѳ�����һ��������

			// ��� ���ж��Ѷ���ͬһ����
			// ��ʼ7x7����
			CServerRegion* region = dynamic_cast<CServerRegion*>(player->GetFather());
			if(region==NULL)
				return 0;

			int startX = player->GetTileX() - 3;
			int startY = player->GetTileY() - 3;
			if(player->GetTileX()+3 >= region->GetWidth())
				startX = region->GetWidth() - 7;
			else if(player->GetTileX()-3 <= 0)
				startX = 0;
			if(player->GetTileY()+3 >= region->GetHeight())
				startY = region->GetHeight() - 7;
			else if(player->GetTileY()-3 <= 0)
				startY = 0;

			// ��������
			// �ж϶����Ƿ� 7��7������
			bool is7x7 = true;
			CPlayer *member = NULL;
			list<CGUID> members;
			team->GetMemberList(members);
			for(list<CGUID>::iterator it = members.begin(); it != members.end(); ++it) 
			{
				member = GetGame()->FindPlayer(*it);
				if(member==NULL)
				{
					is7x7 = false;
					break;
				}
				if(member->GetTileX() < startX && member->GetTileX() <= startX+7
					|| member->GetTileY() < startY && member->GetTileY() <= startY+7)
				{
					is7x7 = false;
					break;
				}
			} 
			if(!is7x7)
				return 4; // ���Ѳ�����7x7������

			// ������7x7������
			// ����ÿ�����ѵ�ָ������[����]
			for(list<CGUID>::iterator it = members.begin(); it != members.end(); ++it) 
			{
				member = GetGame()->FindPlayer(*it);
				if(member!=NULL) 
					member->ChangeRegion(static_cast<eRgnType>(rgnType), regionGuid, x, y, dir, 0, range);
			}
			return 0; // �ɹ���
		}
		break;

	case 1:
		{
			//* ͬ�����ڴ��� *//
			// �ö����Ƿ���ͬһ������
			if(team->GetCurrentServerTeamatesAmount() != team->GetTeamatesAmount())
				return 2; //���Ѳ��Ƕ���һ���������

			// ���Ѷ���һ����������
			// �ö���ÿ�������Ƿ���ͬһ����
			//bool bIsSameRegion = true;
			if(team->GetCurrentRegionTeamatesAmount(player->GetRegionGUID()) != team->GetTeamatesAmount())
				return 3; // ���Ѳ�����һ��������

			//��һ��������
			// ����ÿ�����ѵ�ָ������[����]
			CPlayer *member = NULL;
			list<CGUID> members;
			team->GetMemberList(members);
			for(list<CGUID>::iterator it = members.begin(); it != members.end(); ++it) 
			{
				member = GetGame()->FindPlayer(*it);
				if(member!=NULL) 
					member -> ChangeRegion(static_cast<eRgnType>(rgnType), regionGuid, x, y, dir, 0, range);
			}
			return 0;// �ɹ�
		}
		break;

	case 2:
		{
			//* ͬ�������ڴ��� *//
			// �ö����Ƿ���ͬһ������
			if(team->GetCurrentServerTeamatesAmount() != team->GetTeamatesAmount())
				return 2; //���Ѳ��Ƕ���һ���������

			//��һ����������
			// ����ÿ�����ѵ�ָ������[����]
			CPlayer *member = NULL;
			list<CGUID> members;
			team->GetMemberList(members);

			for(list<CGUID>::iterator it = members.begin(); it != members.end(); ++it)  
			{
				member = GetGame()->FindPlayer(*it);
				if(member!=NULL)
					member->ChangeRegion(static_cast<eRgnType>(rgnType), regionGuid, x, y, dir, 0, range);
			}
			return 0;// �ɹ�
		}
		break;

	case 3:
		return 0;

	default:
		return 0;
	}
	return 1;
}

void Script::SetTeamRegion(int rgnType, const CGUID& regionGuid, long x, long y, long radius, long dir, long range)
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return;

	if(dir==-1)
		dir = player->GetDir();

	//��ȡ��������
	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
	if(team==NULL)
		return;

	long oldRegionId = player->GetRegionID();
	long oldTileX = player->GetTileX();
	long oldTileY = player->GetTileY();

	// ����ÿ�����ѵ�ָ������[����]
	CPlayer *member = NULL;
	list<CGUID> members;
	team->GetMemberList(members);

	for(list<CGUID>::iterator it = members.begin(); it != members.end(); ++it) 
	{
		member = GetGame()->FindPlayer(*it);
		if(member==NULL)
			continue;

		// �Լ�ֱ�Ӵ���
		if(player->GetExID() == member->GetExID()) 
			member->ChangeRegion(static_cast<eRgnType>(rgnType), regionGuid, x, y, dir, 0, range);
		// ��ͬһ���� �� ��һ����Χ��, �����޲���
		else if(member->IsInRegion(oldRegionId)
			&& member->IsInArea(oldTileX, oldTileY, radius))  
			member->ChangeRegion(static_cast<eRgnType>(rgnType), regionGuid, x, y, dir, 0, range );
	}
}

bool Script::IsTeammatesAroundMe(long type, long radius)
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return false;

	//��ȡ��������
	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
	if(team==NULL)
		return false; // û�ж���

	CPlayer* member = NULL;
	switch(type)
	{
		// �Ƿ���ĳ����Χ��
	case 0:
		{
			// �ö����Ƿ���ͬһ������

			//�������� ���Ѳ��Ƕ���һ���������
			if(team->GetCurrentServerTeamatesAmount() != team->GetTeamatesAmount())
				return false; 

			list<CGUID> members;
			team->GetMemberList(members);
			for(list<CGUID>::iterator it = members.begin(); it != members.end(); ++it) 
			{
				member = GetGame()->FindPlayer(*it);
				if(member==NULL)
					return false;

				// �����Լ�
				if(player->GetExID() == member->GetExID())
					continue;

				if(!member->IsInRegion(player->GetRegionID())) 
					return false; // ����һ������

				if(!member->IsInArea(player->GetTileX(), player->GetTileY(), radius)) 
					return false; // �������� ����һ������ ����һ��������
			}
		}
		break; // break 0

		// �Ƿ���һ��������
	case 1:
		{
			// �ö����Ƿ���ͬһ������
			if(team->GetCurrentServerTeamatesAmount() != team->GetTeamatesAmount())
				return false; 

			list<CGUID> members;
			team->GetMemberList(members);
			for(list<CGUID>::iterator it = members.begin(); it != members.end(); ++it) 
			{
				member = GetGame()->FindPlayer(*it);
				if(member==NULL)
					return false;

				// �����Լ�
				if(player->GetExID() == member->GetExID())
					continue;

				// ����Ƿ���ͬһ����
				if(!member->IsInRegion(player->GetRegionID()))
					return false; // ����һ������
			}
		}
		break; // break 1

		// �Ƿ���һ����������
	case 2:
		// �ö����Ƿ���ͬһ������
		if(team->GetCurrentServerTeamatesAmount() != team->GetTeamatesAmount())
			return false; 
		break;

	default:
		return false;// ��������
	} // end case

	return true; // ���ж�Ա���Ͳ��������һ����Χ��
}

bool Script::RunScriptForTeam(const char* scriptName, long distance, const CGUID& playerGuid)
{
	CPlayer* player = NULL;
	if(playerGuid == NULL_GUID)
		player = (dynamic_cast<CPlayer*>(m_RunEnv.srcShape));
	else
		player = GetGame()->FindPlayer(playerGuid);
	if(player==NULL)
		return false;

	GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player -> GetTeamID());
	if(team==NULL)
		return false;

	team->RunScript(scriptName, player, distance);
	return true;
}

bool Script::CreateFaction()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player==NULL)
		return false;

	CMessage msg(MSG_S2C_ORGSYS_CreateFaction_Notify);
	eFactionOptErrInfo eRe;
	if(NULL == GetCountryHandler()->GetCountry(player->GetCountry()))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((long)eFOEI_Err_Country);
	}
	else if(NULL_GUID != player->GetFactionID())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((long)eFOEI_Err_HadFaction);
	}
	else if(eFOEI_NotErr != (eRe=GetOrganiCtrl()->TestUpgradeFacCondition(player, 1, eUT_FactionLevel)))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((long)eRe);
	}
	else
	{
		//! ������֤����ʵ������ʱ�����
		//! �������֤��������������빤������
		GetOrganiCtrl()->PlayerFacOptBegin(player->GetExID(), GameOrganizingCtrl::ePFOT_Create);
		msg.Add(PLAYER_OPT_SUCCEED);
	}
	msg.SendToPlayer(player->GetExID());
	return true;
}

void Script::OpenApplyBoard()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player == NULL)
		return;
	GetOrganiCtrl()->OpenApplyBoard(player->GetExID());
}

void Script::OpenFacIconBoard()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player == NULL)
		return;
	GetOrganiCtrl()->OpenUpLoadIconBoard(player->GetExID());
}

long Script::GetFactionLevel()
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	return GetOrganiCtrl()->GetFactionLvl(player);
}

bool Script::SetFactionRecruitState(long inPut)
{
	CPlayer* player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	if(player == NULL)
		return false;
	return GetOrganiCtrl()->SetFactionRecruit(player, (0 != inPut));
}

CGUID Script::GetFactionIDByPlayerName(const char* playerName)
{
	CPlayer* player=NULL;
	if(playerName==NULL || strlen(playerName)==0)
		player = (dynamic_cast<CPlayer*>(m_RunEnv.srcShape));
	else
		player = GetGame()->FindPlayer(playerName);
	if(player==NULL)
		return NULL_GUID;

	return player->GetFactionID();
}

CGUID Script::GetUinionIDByPlayerName(const char* playerName)
{
	CPlayer* player=NULL;
	if(playerName==NULL || strlen(playerName)==0)
		player = (dynamic_cast<CPlayer*>(m_RunEnv.srcShape));
	else
		player = GetGame()->FindPlayer(playerName);
	if(player==NULL)
		return NULL_GUID;

	return player->GetUnionID();
}

bool Script::IsFacMaster()
{
	CPlayer* player = (dynamic_cast<CPlayer*>(m_RunEnv.srcShape));
	if(NULL == player)
		return false;

	GameFaction* faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
	if(NULL == faction)
		return false;

	return faction->GetMaster() == player->GetExID();
}

long Script::GetFacTickets(const CGUID& factionGuid, const char* footholdName)
{
	if(factionGuid==NULL_GUID || footholdName==NULL)
		return -1;

	GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(factionGuid);
	if(faction==NULL)
		return -1;

	return faction->GetTickets(footholdName);
}

long Script::GetBattleVal(int flag, const CGUID& guid)
{
	switch(flag)
	{
	case 0:
		{
			CPlayer* player=NULL;
			if(guid==NULL_GUID)
				player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
			else
				player = GetGame()->FindPlayer(guid);
			if(NULL==player)
				return -1;

			GameFaction* faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
			if(faction==NULL)
				return -1;

			return faction->GetBattle(player->GetExID());
		}
		break;

	case 1:
		{
			GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(guid);
			if (faction==NULL)
				return -1;
			return faction->GetBattle();
		}	
		break;

	default:
		return -1;
	}
}

long Script::GetArmyInvest(int flag, const CGUID& guid)
{
	switch(flag)
	{
	case 0:
		{
			CPlayer* player=NULL;
			if(guid==NULL_GUID)
				player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
			else
				player = GetGame()->FindPlayer(guid);
			if(NULL==player)
				return -1;

			GameFaction* faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
			if(faction==NULL)
				return -1;

			return faction->GetArmyInvest(player->GetExID());
		}
		break;

	case 1:
		{
			GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(guid);
			if (faction==NULL)
				return -1;
			return faction->GetArmyInvest();
		}	
		break;

	default:
		return -1;
	}
}

bool Script::GetJoinBattle(const CGUID& playerGuid)
{
	CPlayer* player=NULL;
	if(playerGuid==NULL_GUID)
		player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	else
		player = GetGame()->FindPlayer(playerGuid);
	if(NULL==player)
		return false;

	GameFaction* faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
	if(faction==NULL)
		return false;

	return faction->GetJoin(player->GetExID());
}

long Script::GetTechLv(const CGUID& factionGuid, long techNo)
{
	GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(factionGuid);
	if (faction==NULL)
		return -1;

	return faction->GetTechLv(techNo);
}

const char* Script::GetFactionName(const CGUID& factionGuid)
{
	CPlayer* player = (dynamic_cast<CPlayer*>(m_RunEnv.srcShape));
	if(player==NULL)
		return NULL;

	GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(factionGuid);
	if(faction==NULL)
		return NULL;

	return faction->GetName();
}

long Script::GetFacBaseBattle(const CGUID& factionGuid, const char* footholdName)
{
	GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(factionGuid);
	if(faction==NULL)
		return -1;

	return faction->GetBaseBattle(footholdName);
}

void Script::SendBaseInfo(const CGUID& factionGuid)
{
	GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(factionGuid);
	if (faction != NULL)
		faction->SendBaseInfo();
}

long Script::GetFacLevel(ulong type, const CGUID& playerGuid)
{
	CPlayer* player=NULL;
	if(playerGuid==NULL_GUID)
		player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	else
		player = GetGame()->FindPlayer(playerGuid);
	if(NULL==player)
		return -1;

	GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
	if(faction==NULL)
		return -1;

	return faction->GetLevel(type);
}

long Script::GetMemContribute(const CGUID& playerGuid)
{
	CPlayer* player=NULL;
	if(playerGuid==NULL_GUID)
		player = dynamic_cast<CPlayer*>(m_RunEnv.srcShape);
	else
		player = GetGame()->FindPlayer(playerGuid);
	if(NULL==player)
		return -1;

	GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
	if(faction==NULL)
		return -1;

	return faction->GetMemberContribute(player->GetExID());
}

void  Script::OpenCreateFactionUi()
{
    CPlayer* player = (dynamic_cast<CPlayer*>(m_RunEnv.srcShape));
    if(player==NULL)
        return ;

    if ( player->GetFactionID() != NULL_GUID )
        return ;

    CMessage Msg(MSG_S2C_ORGSYS_CreateORGSysUI);
    Msg.SendToPlayer(player->GetExID());
}