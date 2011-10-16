///
/// @file OrganFunc.cpp
/// @brief ȫ�ֹ��ắ��
///
#include "stdafx.h"
#include "lua.hpp"
#include "../ScriptSys.h"
#include "../../Player.h"
#include "../../OrganizingSystem/GSTeam.h"
#include "../../OrganizingSystem/GameOrganizingCtrl.h"

namespace ScriptFuncs
{
	///
	/// \brief �ж�����Ķ�Ա�����ȼ����Ƿ���ĳ����Χ֮��
	/// \detail bool InLvlDifference(space)
	/// \param space �ȼ���ķ�Χ
	///
	static int InLvlDifference(lua_State* L)
	{
		long space = (long)tolua_tonumber(L, 1, 0);

		Script* script = GetInst( ScriptSys ).GetScript( L );
		CPlayer* player = dynamic_cast<CPlayer*>(script->GetEnv().srcShape);
		if(NULL == player)
		{
			tolua_pushnumber( L, -1 );
			return 1;
		}

		GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(team==NULL)
		{
			tolua_pushnumber( L, -1 );
			return 1;
		}

		CMessage msg(MSG_S2W_TEAM_INLVLDIFFERENCE);
		msg.Add(script->ID());
		msg.Add(team->GetExID());
		msg.Add(space);
		msg.Add("");
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief �ж�����Ķ�Ա�����ȼ����Ƿ���ĳ����Χ֮��
	/// \detail bool InLvlDifference(space)
	/// \param space �ȼ���ķ�Χ
	///
	static int InLvlSpace(lua_State* L)
	{
		long minLv = static_cast<long>(tolua_tonumber(L, 1, 0));
		long maxLv = static_cast<long>(tolua_tonumber(L, 2, 0));

		Script* script = GetInst(ScriptSys).GetScript(L);
		CPlayer* player = dynamic_cast<CPlayer*>(script->GetEnv().srcShape);
		if(player==NULL)
			return 0;

		GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(team==NULL)
			return 0;

		CMessage msg(MSG_S2W_TEAM_INLVLSPACE);
		msg.Add(script->ID());
		msg.Add(team->GetExID());
		msg.Add(minLv);
		msg.Add(maxLv);
		msg.Add("");
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief ���Ӱ����Դ
	/// \detail AddFacRes(plyaerGuid, facResValue)
	///
	static int AddFacRes(lua_State* L)
	{
		const CGUID* playerGuid = TOLUA_TOGUID( playerGuid,L, 1, 0);
		Script* script = GetInst(ScriptSys).GetScript(L);
		CPlayer* player = NULL;
		if(playerGuid==NULL)
			player = dynamic_cast<CPlayer*>(script->GetEnv().srcShape);
		else
			player = GetGame()->FindPlayer(*playerGuid);
		if(player==NULL)
			return 0;

		GameFaction* faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
		if(faction==NULL)
			return 0;

		CMessage msg(MSG_S2W_Faction_ChangeRes_Reques);
		msg.Add(faction->GetExID());
		msg.Add(static_cast<long>(tolua_tonumber(L, 2, 0)));
		msg.Add(script->ID());
		msg.Add("");
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief ���Ӱ�ᾭ��ֵ
	/// \detail AddFacExp(plyaerGuid, facExpValue)
	///
	static int AddFacExp(lua_State* L)
	{
		const CGUID* playerGuid = TOLUA_TOGUID( playerGuid,L, 1, 0);
		Script* script = GetInst(ScriptSys).GetScript(L);
		CPlayer* player = NULL;
		if(playerGuid==NULL)
			player = dynamic_cast<CPlayer*>(script->GetEnv().srcShape);
		else
			player = GetGame()->FindPlayer(*playerGuid);
		if(player==NULL)
			return 0;

		GameFaction* faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
		if(faction==NULL)
			return 0;

		CMessage msg(MSG_S2W_Faction_ChangeExp_Reques);
		msg.Add(faction->GetExID());
		msg.Add(static_cast<long>(tolua_tonumber(L, 2, 0)));
		msg.Add(script->ID());
		msg.Add("");
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief ����ĳ������Ա�Ĺ���ֵ
	/// \detail AddFacMemberContribute(plyaerGuid, contributeValue)
	///
	static int AddFacMemberContribute(lua_State* L)
	{
		const CGUID* playerGuid = TOLUA_TOGUID( playerGuid,L, 1, 0);
		Script* script = GetInst(ScriptSys).GetScript(L);
		CPlayer* player = NULL;
		if(playerGuid==NULL)
			player = dynamic_cast<CPlayer*>(script->GetEnv().srcShape);
		else
			player = GetGame()->FindPlayer(*playerGuid);
		if(player==NULL)
			return 0;

		GameFaction* faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
		if(faction==NULL)
			return 0;

		CMessage msg(MSG_S2W_Faction_ChangeExp_Reques);
		msg.Add(faction->GetExID());
		msg.Add(player->GetExID());
		msg.Add(static_cast<long>(tolua_tonumber(L, 2, 0)));
		msg.Add(script->ID());
		msg.Add("");
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief ����ĳ������ĳ���ݵ����Ʊ���������ٴ�������
	/// \detail AddFacTickets(factionGuid, footholdName, ticketAmount)
	///
	static int AddFacTickets(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);

		const CGUID* factionGuid = TOLUA_TOGUID( factionGuid,L, 1, 0);
		if(factionGuid==NULL)
			return 0;
		GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(*factionGuid);
		if(faction==NULL)
			return 0;

		const char* footholdName = TOLUA_TOSTRING( footholdName,L, 2, 0);
		if(footholdName==NULL)
			return 0;

		long ticketAmount = static_cast<long>(tolua_tonumber(L, 3, 0));
		if(ticketAmount==0)
			return 0;

		CMessage msg(MSG_S2W_Faction_ChangeTickets);
		msg.Add(faction->GetExID());
		msg.Add(script->ID());
		msg.Add("");
		msg.Add(footholdName);
		msg.Add(ticketAmount);
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief ����ĳ������ĳ���ݵ��ս��ֵ�����ٴ�������
	/// \detail AddFacBaseBattle(factionGuid, footholdName, battleCredit)
	///
	static int AddFacBaseBattle(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);

		const CGUID* factionGuid = TOLUA_TOGUID( factionGuid,L, 1, 0);
		if(factionGuid==NULL)
			return 0;
		GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(*factionGuid);
		if(faction==NULL)
			return 0;

		const char* footholdName = TOLUA_TOSTRING( footholdName,L, 2, 0);
		if(footholdName==NULL)
			return 0;

		long battleCredit = static_cast<long>(tolua_tonumber(L, 3, 0));
		if(battleCredit==0)
			return 0;

		CMessage msg(MSG_S2W_Faction_ChangeBaseBattle);
		msg.Add(faction->GetExID());
		msg.Add(script->ID());
		msg.Add("");
		msg.Add(footholdName);
		msg.Add(battleCredit);
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief �޸�һ������һ���ݵ����Ϣ
	/// \detail bool ChangeBaseInfo(factionGuid, footholdName)
	///
	static int ChangeBaseInfo(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);

		const CGUID* factionGuid = TOLUA_TOGUID( factionGuid,L, 1, 0);
		if(factionGuid==NULL)
			return 0;
		GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(*factionGuid);
		if(faction==NULL)
			return 0;

		const char* footholdName = TOLUA_TOSTRING( footholdName,L, 2, 0);
		if(footholdName==NULL)
			return 0;

		CMessage msg(MSG_S2W_Faction_ChangeBase);
		msg.Add(faction->GetExID());
		msg.Add(script->ID());
		msg.Add("");
		msg.Add(footholdName);
		msg.Send(false);

		return lua_yield(L, 0);
	}

	///
	/// \brief ����һ������һ������Ա��ս��ֵ
	/// \detail AddBattleVal(guid, battleValue, flag)
	/// \param guid ���GUID����GUID���ɲ���flag����
	/// \param flag 1�����GUID��0�����GUID
	///
	static int AddBattleVal(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);

		const CGUID* guid = TOLUA_TOGUID( guid,L, 1, 0);
		if(guid==NULL)
			return 0;

		long value = static_cast<long>(tolua_tonumber(L, 2, 0));
		if(value==0)
			return 0;

		long flag = static_cast<long>(tolua_tonumber(L, 3, 0));
		GameFaction* faction=NULL;
		CPlayer* player=NULL;
		CMessage msg(MSG_S2W_Faction_ChangeBattle_Reques);
		switch(flag)
		{
		case 0://player's guid
			if(*guid==NULL_GUID)
				player = dynamic_cast<CPlayer*>(script->GetEnv().srcShape);
			else
				player = GetGame()->FindPlayer(*guid);
			if(player==NULL)
				return 0;

			faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
			if(faction==NULL)
				return 0;

			msg.Add(faction->GetExID());
			msg.Add(script->ID());
			msg.Add("");
			msg.Add(value);
			msg.Add(player->GetExID());
			msg.Send();
			break;

		case 1://faction's guid
			faction = GetOrganiCtrl()->GetFactionOrganizing(*guid);
			if(faction==NULL)
				return 0;

			msg.Add(faction->GetExID());
			msg.Add(script->ID());
			msg.Add("");
			msg.Add(value);
			msg.Add(NULL_GUID);
			msg.Send();
			break;

		default://other
			return 0;
		}

		return lua_yield(L, 0);
	}

	///
	/// \brief ����һ������һ������Ա�ľ���Ͷ��ֵ
	/// \detail AddArmyInvest(guid, investValue, flag)
	/// \param guid ���GUID����GUID���ɲ���flag����
	/// \param flag 1�����GUID��0�����GUID
	///
	static int AddArmyInvest(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);

		const CGUID* guid = TOLUA_TOGUID( guid,L, 1, 0);
		if(guid==NULL)
			return 0;

		long value = static_cast<long>(tolua_tonumber(L, 2, 0));
		if(value==0)
			return 0;

		long flag = static_cast<long>(tolua_tonumber(L, 3, 0));
		GameFaction* faction=NULL;
		CPlayer* player=NULL;
		CMessage msg(MSG_S2W_Faction_ChangeArmyInvest_Reques);
		switch(flag)
		{
		case 0://player's guid
			if(*guid==NULL_GUID)
				player = dynamic_cast<CPlayer*>(script->GetEnv().srcShape);
			else
				player = GetGame()->FindPlayer(*guid);
			if(player==NULL)
				return 0;

			faction = GetOrganiCtrl()->GetFactionOrganizing(player->GetFactionID());
			if(faction==NULL)
				return 0;

			msg.Add(faction->GetExID());
			msg.Add(script->ID());
			msg.Add("");
			msg.Add(value);
			msg.Add(player->GetExID());
			msg.Send();
			break;

		case 1://faction's guid
			faction = GetOrganiCtrl()->GetFactionOrganizing(*guid);
			if(faction==NULL)
				return 0;

			msg.Add(faction->GetExID());
			msg.Add(script->ID());
			msg.Add("");
			msg.Add(value);
			msg.Add(NULL_GUID);
			msg.Send();
			break;

		default://other
			return 0;
		}

		return lua_yield(L, 0);
	}

	///
	/// \brief ����һ�����ĿƼ�
	/// \detail AddTech(factionGuid, techNum, techLv)
	///
	static int AddTech(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);

		const CGUID* factionGuid = TOLUA_TOGUID( factionGuid,L, 1, 0);
		if(factionGuid==NULL)
			return 0;
		GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(*factionGuid);
		if(faction==NULL)
			return 0;

		long techNum = static_cast<long>(tolua_tonumber(L, 2, 0));
		long lv = static_cast<long>(tolua_tonumber(L, 3, 0));

		CMessage msg(MSG_S2W_Faction_ChangeTechLevel);
		msg.Add(faction->GetExID());
		msg.Add(script->ID());
		msg.Add("");
		msg.Add(techNum);
		msg.Add(lv);
		msg.Send();

		return lua_yield(L, 0);
	}

	///
	/// \brief �ı�һ�����ĵж԰����Ϣ
	/// \detail ChangeEnemy(factionGuid, clanName, battleStartTime, flag, battleType)
	/// \param flag ��ӻ�ɾ����־
	///
	static int ChangeEnemy(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);

		const CGUID* factionGuid = TOLUA_TOGUID( factionGuid,L, 1, 0);
		if(factionGuid==NULL)
			return 0;
		GameFaction* faction=GetOrganiCtrl()->GetFactionOrganizing(*factionGuid);
		if(faction==NULL)
			return 0;

		const char* clanName = TOLUA_TOSTRING( clanName,L, 2, 0);
		if(clanName==NULL)
			return 0;

		long time = static_cast<long>(tolua_tonumber(L, 3, 0));
		long flag = static_cast<long>(tolua_tonumber(L, 4, 0));
		long battleType = static_cast<long>(tolua_tonumber(L, 5, 0));
		if (battleType>=eMaxWar)
			return 0;

		CMessage msg(MSG_S2W_Faction_ChangeEnemy);
		msg.Add(faction->GetExID());
		msg.Add(script->ID());
		msg.Add("");
		msg.Add(clanName);
		msg.Add(time);
		msg.Add(battleType);
		msg.Add(flag);
		msg.Send();

		return lua_yield(L, 0);
	}

	void OpenOrganLibs( lua_State *L )
	{
		lua_register( L, "InLvlDifference", InLvlDifference );
		lua_register( L, "InLvlSpace", InLvlSpace );
		lua_register( L, "AddFacRes", AddFacRes );
		lua_register( L, "AddFacExp", AddFacExp );
		lua_register( L, "AddFacMemberContribute", AddFacMemberContribute );
		lua_register( L, "AddFacTickets", AddFacTickets );
		lua_register( L, "AddFacBaseBattle", AddFacBaseBattle );
		lua_register( L, "ChangeBaseInfo", ChangeBaseInfo );
		lua_register( L, "AddBattleVal", AddBattleVal );
		lua_register( L, "AddArmyInvest", AddArmyInvest );
		lua_register( L, "AddTech", AddTech );
		lua_register( L, "ChangeEnemy", ChangeEnemy );
	}
}