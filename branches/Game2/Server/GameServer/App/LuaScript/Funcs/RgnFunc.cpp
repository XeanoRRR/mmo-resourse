///
/// @file RgnFunc.cpp
/// @brief ȫ�ֳ�������
///
#include "stdafx.h"
#include "lua.hpp"
#include "RgnFunc.h"
#include "../ScriptSys.h"
#include "../../Player.h"
#include "../../RgnManager.h"
#include "../../../../../Public/ServerPublic/Server/Common/VariableList.h"
#include "../../../../../Public/ServerPublic/Server/Common/VarListUtils.h"
#include "../../OrganizingSystem/GSTeam.h"
#include "../../OrganizingSystem/GameOrganizingCtrl.h"

namespace ScriptFuncs
{
	///
	/// \brief ��������
	///
	/// \detail ret, rgnGuid = CreateRgn(rgnType, ownerGuid, rgnTemplateID)
	/// \return �����ɹ����ʧ�ܷ���nil
	/// \return �³���GUID��ʧ�ܷ���nil
	/// \param rgnType ��������
	/// \param ownerGuid ������GUID
	/// \param rgnTemplateID ������ԴID
	///
	static int CreateRgn( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		if( script == NULL )
			return 0;

		long rgnType = (long) tolua_tonumber( L, 1, RGN_PERSONAL );
		const CGUID *ownerGuid = TOLUA_TOGUID( ownerGuid, L, 2, NULL );
		long rgnResID = (long) tolua_tonumber( L, 3, 0 );
		if( ownerGuid == NULL )
			return 0;

		CPlayer *player = GetGame()->FindPlayer( *ownerGuid );
		player->SetCreateDupRgnType( (eRgnType) rgnType );
		player->SetlCreateDupRgnTID( rgnResID );
		player->SetlCreateDupRgnScriptID( script->ID() );
		long ret = GameManager::GetInstance()->GetRgnManager()->CreateRgnMsg(
			script, rgnResID, (eRgnType) rgnType, player->GetExID(), "", "" ); 
		if( ret < 0 )
			return 0;

		return lua_yield( L, 0 );//���ܵ�������OnCreateRgnMsg�У�flag����Ϊ0����ʾʧ�ܣ���0��lua�б�ʾtrue
	}

	///
	/// \brief ɾ��ָ������
	///
	/// \detail ret = DeleteRgn( rgnType, rgnGuid )
	/// \return ɾ���ɹ����ʧ�ܷ���nil
	/// \param rgnType ��������
	/// \param rgnGuid ����GUID
	///
	static int DeleteRgn( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		if(script==NULL)
			return 0;

		long rgnType = (long) tolua_tonumber( L, 1, RGN_PERSONAL );
		const CGUID *rgnGuid = TOLUA_TOGUID( rgnGuid, L, 2, NULL );
		if( rgnGuid == NULL )
			return 0;

		long ret = GameManager::GetInstance()->GetRgnManager()->DeleteRgnMsg( script->ID(), (eRgnType) rgnType, NULL_GUID, 
			*rgnGuid, "", "" );
		if( ret == 0 )
			return 0;

		return lua_yield( L, 0 );
	}

	///
	/// \brief ��ó�����GUID
	///
	/// \detail rgnGuid = GetRgnID( rgnType, playerID, [rgnResID] )
	/// \detail rgnGuid = GetRgnID( RGN_NORMAL, rgnResID )
	/// \return ����GUID
	/// \param rgnType ��������
	/// \param playerID ӵ����GUID
	/// \param rgnResID ������ԴID
	///
	static int GetRgnID( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		if( script == NULL )
			return 0;

		long rgnType = (long) tolua_tonumber( L, 1, RGN_PERSONAL );
		if( rgnType == RGN_NORMAL )
		{
			long rgnResID = (long) tolua_tonumber( L, 2, 0 );
			CServerRegion *rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(
				RGN_NORMAL, rgnResID );
			if(rgn == NULL)
				return 0;

			script->PushRetValue(rgn->GetExID());
			return 1;
		}

		const CGUID *playerID = TOLUA_TOGUID( playerID, L, 2, NULL );
		if( playerID == NULL )
			return 0;

		CPlayer *player = GetGame()->FindPlayer( *playerID );
		if( player == NULL )
		{
			CMessage msg( MSG_S2W_RGN_VIEWPLAYERPID );
			msg.Add( *playerID );
			msg.Add( "" );
			msg.Add( script->ID() );
			msg.Send();
			return lua_yield( L, 0 );
		}
		CGUID retID;
		if( rgnType == RGN_PERSONAL )
		{
			retID = player->GetPersonalRgnGUID();
		}
		else if( rgnType == RGN_PERSONAL_HOUSE )
		{
			retID = player->GetPersonalHouseRgnGUID();
		}
		else if( rgnType == RGN_TEAM )
		{
			long rgnResID = (long) tolua_tonumber( L, 3, 0 );
			retID = GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGUID( *playerID, rgnResID );
		}
		script->PushRetValue( retID );
		return 1;
	}

	///
	/// \brief ��ӳ���������������
	///
	/// \detail AddRgnVar( rgnType, rgnGuid, varName, varValue )
	/// \param rgnType ��������
	/// \param rgnGuid ����ID
	/// \param varName ������
	/// \param varValue ����ֵ
	/// \see AddVar
	///
	static int AddRgnVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long rgnType = (long) tolua_tonumber( L, 1, RGN_NORMAL );
		const CGUID *rgnGuid = TOLUA_TOGUID( rgnGuid, L, 2, (void*)&NULL_GUID );
		const char *varName = TOLUA_TOSTRING( varName, L, 3, "" );
		CServerRegion *rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID( *rgnGuid );
		VariableList *varList = NULL;
		if( rgn != NULL )
		{
			varList = rgn->GetVariableList();
		}

		CMessage msg( MSG_S2W_RGN_VAR_ADD );
		msg.Add( (uchar) GetGame()->GetIDIndex() );
		msg.Add( script->ID() );
		msg.Add( rgnType ); 
		msg.Add( *rgnGuid );
		msg.Add( varName );

		long varType = VarList::VarType( varName );
		if( varType == VariableList::VAR_NUM )
		{
			double val = tolua_tonumber( L, 4, 0 );
			msg.AddEx( &val, sizeof( val ) );
			if( varList != NULL )
			{
				varList->AddVar( varName, val );
			}
		}
		else if( varType == VariableList::VAR_STRING )
		{
			const char *val = TOLUA_TOSTRING( val, L, 4, "" );
			msg.Add( val );
			if( varList != NULL )
			{
				varList->AddVar( varName, tString( val ) );
			}
		}
		else if( varType == VariableList::VAR_GUID )
		{
			const CGUID *val = TOLUA_TOGUID( val, L, 4, (void*)&NULL_GUID );
			msg.Add( *val );
			if( varList != NULL )
			{
				varList->AddVar( varName, *val );
			}
		}
		msg.Add( (uchar) GetGame()->GetIDIndex() );
		msg.Send();
		if( rgn == NULL )
		{
			return lua_yield( L, 0 );
		}
		return 0;
	}

	///
	/// \brief ����ָ������������ֵ
	///
	/// \detail SetRgnVar( rgnType, rgnGuid, varName, varValue )
	/// \param rgnType ��������
	/// \param rgnGuid ����ID
	/// \param varName ������
	/// \param varValue ����ֵ
	/// \see SerVar
	///
	static int SetRgnVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long rgnType = (long) tolua_tonumber( L, 1, RGN_NORMAL );
		const CGUID *rgnGuid = TOLUA_TOGUID( rgnGuid, L, 2, (void*)&NULL_GUID );
		const char *varName = TOLUA_TOSTRING( varName, L, 3, "" );
		CServerRegion *rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID( *rgnGuid );
		VariableList *varList = NULL;
		if( rgn != NULL )
		{
			varList = rgn->GetVariableList();
		}

		CMessage msg( MSG_S2W_RGN_VAR_SET );
		msg.Add( (uchar) GetGame()->GetIDIndex() );
		msg.Add( script->ID() );
		msg.Add( rgnType ); 
		msg.Add( *rgnGuid );
		msg.Add( varName );

		long varType = VarList::VarType( varName );
		if( varType == VariableList::VAR_NUM )
		{
			double val = tolua_tonumber( L, 4, 0 );
			msg.AddEx( &val, sizeof( val ) );
			if( varList != NULL )
			{
				varList->SetVarValue( varName, val );
			}
		}
		else if( varType == VariableList::VAR_STRING )
		{
			const char *val = TOLUA_TOSTRING( val, L, 4, "" );
			msg.Add( val );
			if( varList != NULL )
			{
				varList->SetVarValue( varName, tString( val ) );
			}
		}
		else if( varType == VariableList::VAR_GUID )
		{
			const CGUID *val = TOLUA_TOGUID( val, L, 4, (void*)&NULL_GUID );
			msg.Add( *val );
			if( varList != NULL )
			{
				varList->SetVarValue( varName, *val );
			}
		}
		msg.Add( (uchar) GetGame()->GetIDIndex() );
		msg.Send();
		if( rgn == NULL )
		{
			return lua_yield( L, 0 );
		}
		return 0;
	}

	///
	/// \brief �Ƴ�ĳ����������
	///
	/// \detail RemoveRgnVar( rgnType, rgnGuid, varName )
	/// \param rgnType ��������
	/// \param rgnGuid ����ID
	/// \param varName ������
	/// \see RemoveVar
	///
	static int RemoveRgnVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long rgnType = (long) tolua_tonumber( L, 1, RGN_NORMAL );
		const CGUID *rgnGuid = TOLUA_TOGUID( rgnGuid, L, 2, (void*)&NULL_GUID );
		const char *varName = TOLUA_TOSTRING( varName, L, 3, "" );
		CServerRegion *rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID( *rgnGuid );
		VariableList *varList = NULL;
		if( rgn != NULL )
		{
			varList = rgn->GetVariableList();
		}

		CMessage msg( MSG_S2W_RGN_VAR_DEL );
		msg.Add( (uchar) GetGame()->GetIDIndex() );
		msg.Add( script->ID() );
		msg.Add( rgnType ); 
		msg.Add( *rgnGuid );
		msg.Add( varName );

		if( varList != NULL )
		{
			varList->RemoveVar( varName );
		}

		msg.Add( (uchar) GetGame()->GetIDIndex() );
		msg.Send();
		if( rgn == NULL )
		{
			return lua_yield( L, 0 );
		}
		return 0;
	}

	///
	/// \brief ��ȡָ������������ֵ
	///
	/// \detail val = GetRgnVar( rgnType, rgnGuid, varName )
	/// \return ����ֵ
	/// \param rgnType ��������
	/// \param rgnGuid ����ID
	/// \param varName ������
	/// \see GetVar
	///
	static int GetRgnVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long rgnType = (long) tolua_tonumber( L, 1, RGN_NORMAL );
		const CGUID *rgnGuid = TOLUA_TOGUID( rgnGuid, L, 2, (void*)&NULL_GUID );
		const char *varName = TOLUA_TOSTRING( varName, L, 3, "" );
		CServerRegion *rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID( *rgnGuid );
		VariableList *varList = NULL;
		if( rgn != NULL )
		{
			varList = rgn->GetVariableList();
		}

		CMessage msg( MSG_S2W_RGN_VAR_GET );
		msg.Add( (uchar) GetGame()->GetIDIndex() );
		msg.Add( script->ID() );
		msg.Add( rgnType ); 
		msg.Add( *rgnGuid );
		msg.Add( varName );

		long varType = VarList::VarType( varName );
		if( varType == VariableList::VAR_NUM )
		{
			if( varList != NULL )
			{
				double val = varList->GetVarValue<double>( varName );
				tolua_pushnumber( L, val );
			}
		}
		else if( varType == VariableList::VAR_STRING )
		{
			if( varList != NULL )
			{
				const tString &val = varList->GetVarValue<tString>( varName );
				tolua_pushstring( L, val.Str() );
			}
		}
		else if( varType == VariableList::VAR_GUID )
		{
			if( varList != NULL )
			{
				const CGUID &val = varList->GetVarValue<CGUID>( varName );
                Script::PushGCGUID( L, val );
			}
		}
		msg.Add( (uchar) GetGame()->GetIDIndex() );
		if( rgn == NULL )
		{
			msg.Send();
			return lua_yield( L, 0 );
		}
		return 1;
	}

	///
	/// \brief ��ȡָ���������������
	///
	/// \detail playerAmount = GetRegionPlayerNum(rgnGuid)
	/// \return �������������������󷵻�nil
	/// \param rgnGuid ����GUID
	///
	static int GetRegionPlayerNum(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const CGUID* rgnGuid = TOLUA_TOGUID( rgnGuid,L, 1, NULL);
		if(rgnGuid==NULL)
			return 0;

		CServerRegion* region = NULL;
		region = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(*rgnGuid);
		if(region == NULL)//�����ѯ
		{
			// ������Ϣ��WS
			CMessage msg(MSG_S2W_RGN_VIEWGSPLAYERNUM);
			msg.Add(*rgnGuid);
			msg.Add(script->GetEnv().srcShape->GetExID()); // �鿴��GUID
			msg.Add(script->ID()); // script id
			msg.Add("");
			msg.Send();
			return lua_yield(L, 0);
		}
		else
		{
			lua_pushnumber(L, (lua_Number)region->GetPlayerAmout());//Ӧ����amount����amout��
			return 1;
		}
	}

	///
	/// \brief ָ�������ɣĵĸ�������ͼ�ɣ�
	///
	/// \detail templateGuid = GetTemplateIDByGUID(rgnGuid)
	/// \return ��ͼGUID��ʧ�ܻ���󷵻�nil
	/// \param rgnType ��������
	/// \param rgnGuid ����GUID
	///
	static int GetTemplateIDByGUID(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		int rgnType = (int)tolua_tonumber(L, 1, 0);
		const CGUID* rgnGuid = TOLUA_TOGUID( rgnGuid,L, 2, NULL);
		if(rgnGuid == NULL)
			return 0;
		
		CServerRegion* rgn = NULL;
		switch((eRgnType)rgnType)
		{
		case RGN_PERSONAL:
			{
				CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().find(*rgnGuid);
				if(it != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end())
				{
					rgn = it->second;
				}
			}
			break;
		case RGN_PERSONAL_HOUSE:
			{
				CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().find(*rgnGuid);
				if(it != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end())
				{
					rgn = it->second;
				}
			}
			break;
		case RGN_TEAM:
			{
				CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().find(*rgnGuid);
				if(it != GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end())
				{
					rgn = it->second;
				}
			}
			break;
		case RGN_NORMAL:
			{
				CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().find(*rgnGuid);
				if(it != GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end())
				{
					rgn = it->second;
				}
			}
			break;
		}
	
		//wq:2008-7-9,��Ҫ�޸�
		if(rgn != NULL) // ����Rgn
		{
			script->PushRetValue(rgn->GetID());
			return 1;
		}

		// ������Ϣ��WS
		CMessage msg(MSG_S2W_RGN_GETTEMPLATEID);
		if(script->GetEnv().srcShape != NULL)
			msg.Add(script->GetEnv().srcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add((long)rgnType);
		msg.Add(*rgnGuid);
		msg.Add("");
		msg.Add(script->ID());
		msg.Send();
		return lua_yield(L, 0);
	}

	///
	/// \brief ��ȡָ�������ɣĵĸ���������
	///
	/// \detail dupType = GetDupTypeByGUID(rgnGuid)
	/// \return ���������ͣ�����������󷵻�nil
	/// \param rgnGuid ����GUID
	///
	static int GetDupTypeByGUID(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const CGUID* rgnGuid = TOLUA_TOGUID( rgnGuid,L, 1, NULL);
		if(rgnGuid == NULL)
			return 0;

		CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().find(*rgnGuid);
		if(it != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end())
		{
			lua_pushnumber(L, (lua_Number)RGN_PERSONAL);
			return 1;
		}

		it = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().find(*rgnGuid);
		if(it != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end())
		{
			lua_pushnumber(L, (lua_Number)RGN_PERSONAL_HOUSE);
			return 1;
		}

		it = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().find(*rgnGuid);
		if(it != GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end())
		{
			lua_pushnumber(L, (lua_Number)RGN_TEAM);
			return 1;
		}

		// ������Ϣ��WS
		CMessage msg(MSG_S2W_RGN_GETDUPTYPE);
		if(script->GetEnv().srcShape != NULL)
			msg.Add(script->GetEnv().srcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(*rgnGuid);
		msg.Add("");
		msg.Add(script->ID());
		msg.Send();

		return lua_yield(L, 0);
	}

	///
	/// \brief ָ�������ɣĵĸ����������ߣɣ�
	///
	/// \detail ownerGuid = GetDupRgnOnwerGUID(rgnGuid)
	/// \return ���������ߵ�GUID������������󷵻�nil
	/// \param rgnGuid ����GUID
	///
	static int GetDupRgnOnwerGUID(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const CGUID* rgnGuid = TOLUA_TOGUID( rgnGuid,L, 1, NULL);
		if(rgnGuid == NULL)
			return 0;


		CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(*rgnGuid);
		if(rgn != NULL)
		{
			script->PushRetValue(rgn->GetOwnerGUID());
			return 1;
		}

		// ������Ϣ��WS
		CMessage msg(MSG_S2W_RGN_GETDUPRGNONWER);
		if(script->GetEnv().srcShape != NULL)
			msg.Add(script->GetEnv().srcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(*rgnGuid);
		msg.Add("");
		msg.Add(script->ID());
		msg.Send();
		return lua_yield(L, 0);
	}

	///
	/// \brief ͨ��������ֲ���GUID
	///
	/// \detail guid = GetGUIDByPlayerName(playerName)
	/// \return GUID������������󷵻�nil
	/// \param playerName �������
	/// \param guidType GUID�����ͣ�1��GUID��2��PGUID��3��PHGUID��4��TGUID��
	/// \param templateId ��ͼGUID������guidType=4ʱ��Ч
	///
	static int GetGUIDByPlayerName(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const char* playerName = TOLUA_TOSTRING( playerName,L, 1, NULL);
		int guidType = (int)tolua_tonumber(L, 2, 0);
		if(playerName==NULL)
			return 0;
		CPlayer* player = GetGame()->FindPlayer(playerName);
		if(player != NULL)//�ڵ�ǰ��������
		{
			switch(guidType)
			{
			case 1:
				script->PushRetValue(player->GetExID());
				break;

			case 2:
				script->PushRetValue(player->GetPersonalRgnGUID());
				break;

			case 3:
				script->PushRetValue(player->GetPersonalHouseRgnGUID());
				break;

			case 4:
				{
					int templateId = (int)tolua_tonumber(L, 3, 0);
					script->PushRetValue(GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGUID(player->GetExID(), templateId));
				}
				break;

			default:
				return 0;
			}
			return 1;
		}

		//���ڵ�ǰ��������,��WorldServer�ύ��ѯ
		CMessage msg;
		switch(guidType)
		{
		case 1:
			msg.SetType(MSG_S2W_GM_QUERY_PLAYER);
			break;
		case 2:
			msg.SetType(MSG_S2W_GM_QUERY_PLAYER_P);
			break;
		case 3:
			msg.SetType(MSG_S2W_GM_QUERY_PLAYER_PH);
			break;
		case 4:
			msg.SetType(MSG_S2W_GM_QUERY_PLAYER_T);
			break;
		default:
			return 0;
		}
		if(script->GetEnv().srcShape != NULL)
			msg.Add(script->GetEnv().srcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(playerName);// player����
		msg.Add(script->ID()); // script id
		msg.Add("");
		msg.Send();
		return lua_yield(L, 0);		
	}

	///
	/// \brief �鿴Rgn�Ƿ����
	///
	/// \detail isExist = ViewRgnValidByGUID(rgnGuid)
	/// \return Rgn�Ƿ���ڣ�����������󷵻�nil
	/// \param rgnGuid ����GUID
	///
	static int ViewRgnValidByGUID(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const CGUID* rgnGuid = TOLUA_TOGUID( rgnGuid,L, 1, NULL);
		if(rgnGuid==NULL)
			return 0;
	
		CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(*rgnGuid);
		if(rgn)
		{
			lua_pushnumber(L, 1.0);
			return 1;
		}

		CMessage msg(MSG_S2W_RGN_VIEWRGNAVLID);
		msg.Add(*rgnGuid); // rgn guid
		if(script->GetEnv().srcShape != NULL)
			msg.Add(script->GetEnv().srcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(""); // ret name
		msg.Add(script->ID()); //Script id
		msg.Send();
		return lua_yield(L, 0);//���ܵ�������������0����ʾʧ�ܣ���0��lua�б�ʾtrue
	}

	///
	/// \brief ˢ����ҵĸ�����¼�б�
	///
	/// \detail ReFreshPlayerDupRgnList(playerGuid)
	/// \param playerGuid ���GUID
	///
	static int ReFreshPlayerDupRgnList(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		CGUID playerGuid = NULL_GUID;
		const CGUID* guid = TOLUA_TOGUID( guid,L, 1, NULL);
		if(guid != NULL)
			playerGuid = *guid;
		else if(script->GetEnv().srcShape != NULL)
			playerGuid = script->GetEnv().srcShape->GetExID();
		if(playerGuid==NULL_GUID)
			return 0;


		CMessage msg(MSG_S2W_RGN_QUESTPLAYERRGNLIST);
		msg.Add(script->ID());
		msg.Add(playerGuid);

		CPlayer* player = GetGame()->FindPlayer(playerGuid);
		if(player != NULL)
		{
			msg.Add((BYTE)1); // ��GS��ѯ
			map<LONG, CGUID>* rgnIdMap = GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGuidMap(playerGuid);
			if(rgnIdMap)
			{
				msg.Add((long)rgnIdMap->size());
				for(map<LONG, CGUID>::iterator it = rgnIdMap->begin(); it != rgnIdMap->end(); ++it)
				{
					msg.Add((long)it->first);
					msg.Add(it->second);
				}
			}
			else
			{
				msg.Add((long)0);
			}
		}
		else
		{
			msg.Add((BYTE)0); // �Ǳ�GS��ѯ
		}
		msg.Send();
		return lua_yield(L, 0);
	}


	///
	/// \brief �õ�������ڶ���Ķ��鸱����
	///
	/// \detail dupAmount = GetTeamRgnIDNum(playerGuid)
	/// \return ������ڶ���Ķ��鸱����
	/// \param playerGuid ���GUID
	///
	static int GetTeamRgnIDNum(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const CGUID* playerGuid = TOLUA_TOGUID( playerGuid,L, 1, NULL);
		if(playerGuid == NULL)
			return 0;

		CPlayer* player = GetGame()->FindPlayer(*playerGuid);
		if(player==NULL)
			return 0;
	
		GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(NULL == team)
			return 0;

		CPlayer *master = GetGame()->FindPlayer(team->GetLeader());
		if(NULL != master)
		{
			long rgnNum = 0;
			map<LONG, CGUID>* rgnIdMap = GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGuidMap(master->GetExID());
			if(rgnIdMap!=NULL)
				rgnNum = (long)rgnIdMap->size();
			lua_pushnumber(L, rgnNum);
			return 1;
		}
		else
		{
			CMessage msg(MSG_S2W_TEAM_GetRgnNum);
			msg.Add(team->GetLeader());
			msg.Add(script->ID());
			msg.Add("");
			msg.Send();
			return lua_yield(L, 0);
		}
	}

	///
	/// \brief ��ȡ������ڶ���ĵ�N�����鸱��ID
	///
	/// \detail dupGuid = GetTeamRgnIDByIndex(playerGuid,idx)
	/// \return ������ڶ���ĵ�N�����鸱��ID
	/// \param playerGuid ���GUID
	/// \param idx ���������
	///
	static int GetTeamRgnIDByIndex(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const CGUID* playerGuid = TOLUA_TOGUID( playerGuid,L, 1, NULL);
		if(playerGuid == NULL)
			return 0;

		CPlayer* player = GetGame()->FindPlayer(*playerGuid);
		if(player==NULL)
			return 0;

		int idx = (int)tolua_tonumber(L, 2, 0);

		GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(team==NULL)
			return 0;

		CPlayer* master = GetGame()->FindPlayer(team->GetLeader());
		if(NULL != master)
		{
			script->PushRetValue(GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGuidByIndex(master->GetExID(), idx));
			return 1;
		}

		CMessage msg(MSG_S2W_TEAM_GetRgnGuid);
		msg.Add(team->GetLeader());
		msg.Add(script->ID());
		msg.Add((long)idx);
		msg.Add("");
		msg.Send();
		return lua_yield(L, 0);
	}

	///
	/// \brief �õ�������ڶ���ĵ�N�����鸱������ͼID
	///
	/// \detail templateId = GetTeamRgnTemplatIDByIndex(playerGuid,idx)
	/// \return ������ڶ���ĵ�N�����鸱������ͼID
	/// \param playerGuid ���GUID
	/// \param idx ���������
	///
	static int GetTeamRgnTemplatIDByIndex(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		const CGUID* playerGuid = TOLUA_TOGUID( playerGuid,L, 1, NULL);
		if(playerGuid == NULL)
			return 0;

		CPlayer* player = GetGame()->FindPlayer(*playerGuid);
		if(player==NULL)
			return 0;

		int idx = (int)tolua_tonumber(L, 2, 0);

		GSTeam* team = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(team==NULL)
			return 0;

		CPlayer* master = GetGame()->FindPlayer(team->GetLeader());
		if(NULL != master)
		{
			lua_pushnumber(L, (GameManager::GetInstance()->GetRgnManager()->GetTeamRgnTemplateIdByIndex(master->GetExID(), idx)));
			return 1;
		}

		CMessage msg(MSG_S2W_TEAM_GetRgnTemplateID);
		msg.Add(team->GetLeader());
		msg.Add(script->ID());
		msg.Add((long)idx);
		msg.Add("");
		msg.Send();
		return lua_yield(L, 0);
	}

	///
	/// \brief ��ȡָ����ͼָ����Ӫ��ȷ����
	///
	/// \detail playerAmount = GetRegionCampPlayerNum(rgnGuid,campType)
	/// \return ָ����ͼָ����Ӫ��ȷ����
	/// \param rgnGuid ����GUID
	/// \param campType ��Ӫ����
	///
	static int GetRegionCampPlayerNum(lua_State* L)
	{
		Script* script = GetInst(ScriptSys).GetScript(L);
		if(script==NULL)
			return 0;

		CServerRegion* region = NULL;
		const CGUID* rgnGuid = TOLUA_TOGUID( rgnGuid,L, 1, NULL);
		int campType = (int)tolua_tonumber(L, 2, 0);
		if(rgnGuid == NULL)
			region = dynamic_cast<CServerRegion*>(script->GetEnv().region);
		else
			region = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(*rgnGuid);
		if(region==NULL)
			return 0;


		if(region != NULL) // �Ǳ��س���
		{
			lua_pushnumber(L, region->GetCampPlayerNum((eCampType)campType));
			return 1;
		}

		// ����������������
		// ���Ͳ�ѯ��Ϣ
		CMessage msg(MSG_S2W_RGN_CAMP_PLAYER_NUM_QUEST);
		msg.Add((long)GetGame()->GetIDIndex()); // gsid
		msg.Add(*rgnGuid);// ����GUID
		msg.Add((long)campType);// ָ����Ӫ����
		msg.Add(script->ID()); // ����ű�guid
		msg.Add(""); //����ű����ؽű�������
		msg.Send();
		return lua_yield(L, 0);
	}

	void OpenRgnLibs( lua_State *L )
	{
		lua_register( L, "CreateRgn", CreateRgn );
		lua_register( L, "DeleteRgn", DeleteRgn );
		lua_register( L, "GetRgnID", GetRgnID );
		lua_register( L, "AddRgnVar", AddRgnVar );
		lua_register( L, "SetRgnVar", SetRgnVar );
		lua_register( L, "RemoveRgnVar", RemoveRgnVar );
		lua_register( L, "GetRgnVar", GetRgnVar );
		lua_register( L, "GetRegionPlayerNum", GetRegionPlayerNum );
		lua_register( L, "GetTemplateIDByGUID", GetTemplateIDByGUID );
		lua_register( L, "GetDupTypeByGUID", GetDupTypeByGUID );
		lua_register( L, "GetDupRgnOnwerGUID", GetDupRgnOnwerGUID );
		lua_register( L, "GetGUIDByPlayerName", GetGUIDByPlayerName );
		lua_register( L, "ViewRgnValidByGUID", ViewRgnValidByGUID );
		lua_register( L, "ReFreshPlayerDupRgnList", ReFreshPlayerDupRgnList );
		lua_register( L, "GetTeamRgnIDNum", GetTeamRgnIDNum );
		lua_register( L, "GetTeamRgnIDByIndex", GetTeamRgnIDByIndex );
		lua_register( L, "GetTeamRgnTemplatIDByIndex", GetTeamRgnTemplatIDByIndex );
		lua_register( L, "GetRegionCampPlayerNum", GetRegionCampPlayerNum );

		lua_pushnumber( L, RGN_NORMAL );
		lua_setglobal( L, "RGN_NORMAL" );
		lua_pushnumber( L, RGN_PERSONAL );
		lua_setglobal( L, "RGN_PERSONAL" );
		lua_pushnumber( L, RGN_PERSONAL_HOUSE );
		lua_setglobal( L, "RGN_PERSONAL_HOUSE" );
		lua_pushnumber( L, RGN_TEAM );
		lua_setglobal( L, "RGN_TEAM" );
	}
}
