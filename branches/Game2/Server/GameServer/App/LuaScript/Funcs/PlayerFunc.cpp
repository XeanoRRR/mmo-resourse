///
/// @file PlayerFunc.cpp
/// @brief ȫ����Һ���
///
#include "stdafx.h"
#include "lua.hpp"
#include "PlayerFunc.h"
#include "../Script.h"
#include "../ScriptSys.h"
#include "../../Player.h"
#include "../../../../../Public/Setup/GlobalSetup.h"

namespace ScriptFuncs
{
	///
	/// ��ȡȫ���������
	///
	/// count = GetPlayerCount()
	/// @return �������
	///
	static int GetPlayerCount( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		CPlayer *player = dynamic_cast<CPlayer*>( script->GetEnv().srcShape );
		if( player == NULL )
		{
			return 0;
		}
		CMessage msg( MSG_S2W_GM_QUERY_PLAYERCOUNT );
		msg.Add( player->GetExID() );
		msg.Add( script->ID() );
		msg.Send();
		return lua_yield( L, 0 );
	}

	/// 
	/// ����ָ�����
	///
	/// ret = SilencePlayer( name, time )
	/// @param �������
	/// @param time ����ʱ��
	/// @return �ɹ����ط�0
	///
	static int SilencePlayer( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		CPlayer *self = dynamic_cast<CPlayer*>( script->GetEnv().srcShape );
		if( self == NULL )
		{
			return 0;
		}
		const char *name = TOLUA_TOSTRING( name, L, 1, "" );
		long time = (long) tolua_tonumber( L, 2, 1 );
		time *= 60000;
		CPlayer *player = GetGame()->FindPlayer( name );
		if( player != NULL )
		{
			player->SetSilence( time );
			tolua_pushnumber( L, 1 );
			return 1;
		}
		else
		{
			CMessage msg( MSG_S2W_GM_SILENCE_PLAYER );
			msg.Add( self->GetExID() );
			msg.Add( script->ID() );
			msg.Add( name );
			msg.Add( time );
			msg.Send();
			return lua_yield( L, 0 );
		}
	}

	///
	/// ȡ��������ڳ�����Ϣ
	///
	/// ret, rgnID, x, y = GetPlayerRegion( name )
	/// @param name �������
	/// @return �����ɹ����(1��ʾ�ɹ�)
	/// @return ����GUID
	/// @return X����
	/// @return Y����
	///
	static int GetPlayerRegion( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		const char *name = TOLUA_TOSTRING( name, L, 1, "" );
		CPlayer *player = GetGame()->FindPlayer( name );
		CPlayer *self = dynamic_cast<CPlayer*>( script->GetEnv().srcShape );
		if( self == NULL )
		{
			return 0;
		}
		if( player != NULL )
		{
			const CGUID &rgnID = player->GetRegionGUID();
			long x = player->GetTileX();
			long y = player->GetTileY();
			script->PushRetValue( 1 );
			script->PushRetValue( rgnID );
			script->PushRetValue( x );
			script->PushRetValue( y );
			return 4;
		}
		else
		{
			CMessage msg( MSG_S2W_GM_GETPLAYER_REGION );
			msg.Add( self->GetExID() );
			msg.Add( script->ID() );
			msg.Add( name );
			msg.Send();
			return lua_yield( L, 0 );
		}
	}

	///
	/// ����ָ�����
	///
	/// ret = ChangePlayerRegion( name, rgnID, x, y )
	/// @param name �������
	/// @param rgnID ����GUID
	/// @param x X����
	/// @param y Y����
    /// @return ����1��ʾ�г����ɹ�������ʧ��
	///
	static int ChangePlayerRegion( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		CPlayer *self = dynamic_cast<CPlayer*>( script->GetEnv().srcShape );
		if( self == NULL )
		{
			return 0;
		}
		const char *name = TOLUA_TOSTRING( name, L, 1, "" );
		const CGUID *rgnID = TOLUA_TOGUID( rgnID, L, 2, (void*) &NULL_GUID );
		long x = (long) tolua_tonumber( L, 3, 0 );
		long y = (long) tolua_tonumber( L, 4, 0 );

		CPlayer *player= GetGame()->FindPlayer( name );
		if( player != NULL )
		{
			player->ChangeRegion( RGN_ALL, *rgnID, x, y, 0 );
			tolua_pushnumber( L, 1 );
			return 1;
		}
		else
		{
			CMessage msg( MSG_S2W_GM_CHANGETPLAYER_REGION );
			msg.Add( self->GetExID() );
			msg.Add( script->ID() );
			msg.Add( name );
			msg.Add( *rgnID );
			msg.Add( x );
			msg.Add( y );
			msg.Send();
			return lua_yield( L, 0 );
		}
	}

	/// 
	/// �ڿͻ��˵����Ի���������޸�����
	///
	/// ChangePlayerName()
	///
	static int ChangePlayerName( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		CPlayer *self = dynamic_cast<CPlayer*>( script->GetEnv().srcShape );
		if( self == NULL )
		{
			return 0;
		}
		const char *str = strchr( self->GetName(), GlobalSetup::GetSetup()->strSpeStr[0] );
		if( str == NULL )
		{
			CMessage msg( MSG_S2C_OTHER_CHANGENAME );
			msg.Add( self->GetExID() );
			msg.Add( self->GetName() );
			msg.Add( (uchar)(-2) );
			msg.SendToPlayer( self->GetExID() );
			return 0;
		}
		CMessage msg( MSG_S2C_OTHER_UPDATENAMEOPEN );
		msg.Add( script->ID() );
		msg.SendToPlayer( self->GetExID() );
		return lua_yield( L, 0 );
	}

	/// 
	/// ���ָ������Ƿ�����
	///
	/// ret = IsPlayerOnline( playerName )
	/// @param playerName �������
	/// @return 1��ʾ����
	///
	static int IsPlayerOnline( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		CPlayer *self = dynamic_cast<CPlayer*>( script->GetEnv().srcShape );
		if( self == NULL )
		{
			tolua_pushnumber( L, 0 );
			return 1;
		}
		const char *name = TOLUA_TOSTRING( name, L, 1, "" );
		CPlayer *player = GetGame()->FindPlayer( name );
		if( player != NULL )
		{
			tolua_pushnumber( L, 1 );
			return 1;
		}
		else
		{
			CMessage msg( MSG_S2W_GM_QUERY_PLAYERONLINE );
			msg.Add( self->GetExID() );
			msg.Add( name );
			msg.Add( script->ID() );
			msg.Send();
			return lua_yield( L, 0 );
		}
	}

	void OpenPlayerLibs( lua_State *L )
	{
		lua_register( L, "GetPlayerCount", GetPlayerCount );
		lua_register( L, "SilencePlayer", SilencePlayer );
		lua_register( L, "GetPlayerRegion", GetPlayerRegion );
		lua_register( L, "ChangePlayerRegion", ChangePlayerRegion );
		lua_register( L, "ChangePlayerName", ChangePlayerName );
		lua_register( L, "IsPlayerOnline", IsPlayerOnline );
	}
}
