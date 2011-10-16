///
/// @file VarListFuncs.cpp
/// @brief �����ű������ĺ���
///
#include "stdafx.h"
#include "VarListFuncs.h"
#include "lua.hpp"
#include "../ScriptSys.h"
#include "../ScriptUtils.h"
#include "../../../../../Public/ServerPublic/Server/Common/VariableList.h"
#include "../../../../../Public/ServerPublic/Server/Common/VarListUtils.h"

namespace ScriptFuncs
{
	///
	/// ��鵱ǰ�ű�����ʵ���Ƿ���ĳ���ű��б�
	///
	/// ret = HasVarList( operType )
	/// @param operType: VT_SCRIPT�ű�����ֻ�ڼ��ܽű�����Ч��
	/// VT_SRCSHAPEԴ����VT_DESTSHAPEĿ�����
	/// VT_GLOBALȫ�ֱ���
	/// @return 1��ʾ����
	///
	static int HasVarList( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		int ret = 1;
		long operType = (long) tolua_tonumber( L, 1, 0 );
		if( script == NULL || script->GetVarList( operType ) == NULL )
		{
			ret = 0;	
		}
		lua_pushboolean( L, ret );
		return 1;
	}

	///
	/// ���һ���ű�������ָ�������б���
	///
	/// ret = AddVar( operType, "$varName", initVal, count =1 )
	/// @param operType: VT_SCRIPT, VT_SRCSHAPE, VT_DESTSHAPE
	/// @param varName ������
	/// @param initVal ��ʼֵ
	/// @param count [��ѡ]���鳤�ȣ�������Ϊ1
	///
	static int AddVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long operType = (long) tolua_tonumber( L, 1, 0 );
		VariableList *varList = ( script != NULL ? script->GetVarList( operType ) : NULL );
		bool ret = false;
		if( operType == Script::VT_GLOBAL )
		{
			// not support global variable list
			varList = NULL;
		}
		if( varList != NULL )
		{
			ret = true;
			long count = (long) tolua_tonumber( L, 4, 1 );
			const char *varName = TOLUA_TOSTRING( varName, L, 2, "" );
			long varType = VarList::VarType( varName );
			if( varType == VariableList::VAR_NUM )
			{
				VariableList::Number val = (VariableList::Number) tolua_tonumber( L, 3, 0 );
				varList->AddVar( varName, val, count );
			}
			else if( varType == VariableList::VAR_STRING )
			{
				const char *val = TOLUA_TOSTRING( val, L, 3, "" ); 
				varList->AddVar( varName, tString( val ), count );
			}
			else if( varType == VariableList::VAR_GUID )
			{
				const CGUID *val = TOLUA_TOGUID( val, L, 3, &NULL_GUID );
                varList->AddVar( varName, *val, count );
			}
			else
			{
				// argument error
				ret = false;
			}
		}
		else
		{
			// pop the arguments.
			int top = lua_gettop( L );
			lua_settop( L, top - 3 );
		}

		lua_pushboolean( L, ret ? 1 : 0 );
		return 1;
	}

	///
	/// �Ƴ�ָ���б����ĳ���ű�����
	///
	/// RemoveVar( operType, "$varName" )
	/// @param operType:VT_SCRIPT, VT_SRCSHAPE, VT_DESTSHAPE
	/// @param varName ������
	///
	static int RemoveVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long operType = (long) tolua_tonumber( L, 1, 0 );
		VariableList *varList = ( script != NULL ? script->GetVarList( operType ) : NULL );
		if( operType == Script::VT_GLOBAL )
		{
			// not support global variable list
			varList = NULL;
		}
		const char *varName = TOLUA_TOSTRING( varName, L, 2, "" );
		if( varList != NULL )
		{
			varList->RemoveVar( varName );
		}
		return 0;
	}

	///
	/// ȡ��ָ���б���ĳ��������ֵ
	///
	/// luaVar = GetVar( operType, "$varName", [pos] )
	/// @param operType:VT_SCRIPT, VT_SRCSHAPE, VT_DESTSHAPE, VT_GLOBAL
	/// @param varName ������
	/// @param pos [��ѡ] ������������0��ʼ
	/// @return ����ֵ��������������ڣ�����Ҫ������Ͷ�Ӧ�ķǷ�ֵ���Ƚϣ�
	/// �ַ�������""�Ƚϣ�������INVALID_NUM�Ƚϣ�GUID�� CGUID::GUID_INVALID ���Ƚ�
	///
	static int GetVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long operType = (long) tolua_tonumber( L, 1, 0 );
		VariableList *varList = ( script != NULL ? script->GetVarList( operType ) : NULL );
		long index = (long) tolua_tonumber( L, 3, 0 );
		const char *varName = TOLUA_TOSTRING( varName, L, 2, "" );
		long varType = VarList::VarType( varName );
		if( varList == NULL )
		{
			lua_pushnumber( L, -1 );
			return 1;
		}
		if( varType == VariableList::VAR_NUM )
		{
			VariableList::Number val = varList->GetVarValue<VariableList::Number>( varName, index );
			lua_pushnumber( L, (lua_Number) val );
		}
		else if( varType == VariableList::VAR_STRING )
		{
			const tString &val = varList->GetVarValue<tString>( varName, index );
			lua_pushstring( L, val.Valid() ? val.Str() : "" );
		}
		else if( varType == VariableList::VAR_GUID )
		{
			const CGUID &val = varList->GetVarValue<CGUID>( varName, index );
            Script::PushGCGUID( L, val );
		}
		else
		{
			// argument error.
			lua_pushnumber( L, -1 );
		}
		return 1;
	}

	///
	/// ����ָ���б���ĳ��������ֵ
	///
	/// ret = SetVar( operType, "$varName", val, pos )
	/// @param operType:VT_SCRIPT, VT_SRCSHAPE, VT_DESTSHAPE, VT_GLOBAL
	/// @param varName ������
	/// @param val ����ֵ
	/// @param pos [��ѡ]��������
	///
	static int SetVar( lua_State *L )
	{
		Script *script = GetInst( ScriptSys ).GetScript( L );
		long operType = (long) tolua_tonumber( L, 1, 0 );
		VariableList *varList = ( script != NULL ? script->GetVarList( operType ) : NULL );
		bool ret = false;
		if( varList != NULL )
		{
			ret = true;
			long index = (long) tolua_tonumber( L, 4, 0 );
			const char *varName = TOLUA_TOSTRING( varName, L, 2, "" );
			long varType = VarList::VarType( varName );
			if( varType == VariableList::VAR_NUM )
			{
				VariableList::Number val = (VariableList::Number) tolua_tonumber( L, 3, 0 );
				varList->SetVarValue( varName, val, index );
				if( operType == Script::VT_GLOBAL )
				{
					ScriptUtils::UpdateValToWS( varName, val, index );
				}
			}
			else if( varType == VariableList::VAR_STRING )
			{
				const char *val = TOLUA_TOSTRING( val, L, 3, "" ); 
				varList->SetVarValue( varName, tString( val ), index );
				if( operType == Script::VT_GLOBAL )
				{
					ScriptUtils::UpdateValToWS( varName, tString( val ), index );
				}
			}
			else if( varType == VariableList::VAR_GUID )
			{
				const CGUID *val = TOLUA_TOGUID( val, L, 3, &NULL_GUID ); 
				varList->SetVarValue( varName, *val, index );
				if( operType == Script::VT_GLOBAL )
				{
					ScriptUtils::UpdateValToWS( varName, *val, index );
				}
			}
			else
			{
				// argument error
				ret = false;
			}
		}
		else
		{
			// pop the arguments.
			int top = lua_gettop( L );
			lua_settop( L, top - 3 );
		}

		lua_pushboolean( L, ret ? 1 : 0 );
		return 1;
	}

	void OpenVarListLibs( lua_State *L )
	{
		lua_register( L, "HasVarList", HasVarList );
		lua_register( L, "AddVar", AddVar );
		lua_register( L, "RemoveVar", RemoveVar );
		lua_register( L, "SetVar", SetVar );
		lua_register( L, "GetVar", GetVar );

		// some constants.
		lua_pushinteger( L, Script::VT_SCRIPT );
		lua_setglobal( L, "VT_SCRIPT" );
		lua_pushinteger( L, Script::VT_SRCSHAPE );
		lua_setglobal( L, "VT_SRCSHAPE" );
		lua_pushinteger( L, Script::VT_DESTSHAPE );
		lua_setglobal( L, "VT_DESTSHAPE" );
		lua_pushinteger( L, Script::VT_REGION );
		lua_setglobal( L, "VT_REGION" );
		lua_pushinteger( L, Script::VT_GLOBAL );
		lua_setglobal( L, "VT_GLOBAL" );

		lua_pushnumber( L, (lua_Number) Private::TypeTrait<VariableList::Number>::InvalidVal() );
		lua_setglobal( L, "INVALID_NUM" );
	}
}
