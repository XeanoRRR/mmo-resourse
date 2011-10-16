///
/// @file BaseFunc.cpp
/// @brief ȫ�ֻ���֧�ֺ���
/// @see BaseFunc.h
///
#include "stdafx.h"
#include "BaseFunc.h"
#include "lua.hpp"
#include "../ScriptSys.h"
#include "GlobalScriptTimer.h"

class Script;
namespace ScriptFuncs
{
	///
	/// ��ȡScript���ܶຯ��λ��Script���ڣ���ʹ����Щ����֮ǰ��Ҫ��ȡ��Script����
	///
	/// script = GetScript()
	/// @return Script��
	///
	static int GetScript( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		tolua_pushusertype( L, script, "Script" );
		return 1;
	}

	/// 
	/// ����ָ���ű����µ�����ʵ���뵱ǰ����ʵ���Ľű�������ͬ
	///
	/// ret = Call( scriptFile )
	/// @param scriptFile �ű��ļ���
	/// @return ���ط�0��ʾ�ɹ�
	///
	static int Call( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		int ret = 0;
		if( script != NULL )
		{
			const char *file = luaL_checkstring( L, -1 );
			if( file != NULL )
			{
				// the same run env.
				ret = GetInst( ScriptSys ).RunScript( script->GetEnv(), file ) ? 1:0;
			}
		}
		lua_pushnumber( L, (lua_Number)ret );
		return 1;
	}

	///
	/// �õ�ǰ�ű����𲢵ȴ�һ��ʱ��
	///
	/// Wait( millSecs )
	/// @param millSecs �ȴ���ʱ�䣨���룩
	///
	static int Wait( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		if( script == NULL )
		{
			return 0;
		}
		long millSecs = (long) tolua_tonumber( L, 1, 0 );
		CGlobalScriptTimer::GetInstance()->RegisterWaitScript( script->ID(), millSecs );		
		return lua_yield( L, 0 );
	}

	void OpenBaseLibs( lua_State *L )
	{
		lua_register( L, "GetScript", GetScript );
		lua_register( L, "Call", Call );
		lua_register( L, "Wait", Wait );

		lua_pushnumber( L, TYPE_PLAYER );
		lua_setglobal( L, "TYPE_PLAYER" );
		lua_pushnumber( L, TYPE_MONSTER );
		lua_setglobal( L, "TYPE_MONSTER" );

		lua_pushnumber( L, Script::EXEC_ERROR );
		lua_setglobal( L, "EXEC_ERROR" );
		lua_pushnumber( L, Script::EXEC_FAIL );
		lua_setglobal( L, "EXEC_FAIL" );
		lua_pushnumber( L, Script::EXEC_SUCCESS );
		lua_setglobal( L, "EXEC_SUCCESS" );
	}

	ulong ARGB( byte a, byte r, byte g, byte b )
	{
		return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
	}

	long RegisterTimer( const char *file, long userData, long trigerTime, long interTime )
	{
		return CGlobalScriptTimer::GetInstance()->RegisterTimer( trigerTime, interTime, file, userData );
	}

	void UnRegisterTimer( long timerID, long userData )
	{
		CGlobalScriptTimer::GetInstance()->UnRegisterTimer( userData, timerID );
	}
}


