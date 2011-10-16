///
/// @file AIFactory.h
/// @brief To create ai object by its type.
/// 
/// �����AI�ķ�ʽ��
/// 1. CAIFactory������µ�AI����ֵ��
/// 2. ����REGISTER_AI�����µ�AI����ֵ���ֵ��Ӧ��AI������
///

#pragma once
#include "MonsterAI.h"

#include <list>
#include <map>

const LONG AI_ROW_LENTH = 256;


namespace Private
{
#define MAX_CACHE_SIZE  AI_ROW_LENTH

template <typename Type, typename BaseType>
class Obj
{
public:
	typedef Type this_type;
	typedef BaseType base_type;
public:
	static base_type *Create() 
	{
		if( _ObjCache.size() == 0 )
		{
			return MP_NEW this_type;
		}
		else
		{
			base_type *obj = _ObjCache.back();
			_ObjCache.pop_back();
			return obj;
		}
	}

	static void Delete( base_type *obj )
	{
		if( _ObjCache.size() >= MAX_CACHE_SIZE )
		{
			MP_DELETE(obj);
		}
		else
		{
			_ObjCache.push_back( static_cast<this_type*>( obj ) );
		}
	}
	
	static void Clear()
	{
		for( std::list<this_type*>::iterator it = _ObjCache.begin(); it != _ObjCache.end(); ++ it )
		{
			MP_DELETE(*it);
		}
	}

private:
	/// why use this_type, because i want the cache to store the correct type inherited from base_type.
	static std::list<this_type*> _ObjCache;
};

template <typename Type, typename BaseType>
std::list<Type*> Obj<Type, BaseType>::_ObjCache;

///
/// Because script ai value is in a range ( > 255 ), so i should implement my less predicator.
///
struct MyLess
{
	bool operator() ( long lLeft, long lRight ) const
	{
		if( lLeft > 255 && lRight > 255 )
		{
			// ai script
			return false;
		}
		else
		{
			return lLeft < lRight;
		}
	}
};

}

class CAIFactory
{
public:
	enum AI_TYPE
	{
		NPC_NORMAL						= 0,	//##npc��ͨ
		GLADIATOR						= 1,	//##�ⲫ��ͨ
		PASSIVE_GLADIATOR				= 2,	//##�ⲫ����
		STUPID_GLADIATOR				= 3,	//##�ⲫ�ޱ�
		FIXED_POSITION_ARCHER			= 4,	//##Զ�̶̹�
		SNIPE_ARCHER					= 5,	//##Զ�̾ѻ�
		MODEL_WOLF						= 6,	//����-��
		GUARD							= 7,	//����
		DHF_GLADIATOR					= 8,	//(Distance and Hurted First)���ȼ�¼-����
		DHF_PASSIVE_GLADIATOR			= 9,	//(Distance and Hurted First)���ȼ�¼-����
		CITY_GUARD_WITH_SWORD			= 10,	//##���д�����
		CITY_GUARD_WITH_BOW				= 11,	//##���й�������
		GUARD_COUNTRY					= 13,	// ��������
		GUARD_COUNTRY2					= 14,	// ��������2,ֻ���������
		VILL_COUNTRY_GUARD_WITH_SWORD	= 15,	//##�ݵ�Ĵ�������ֻ�ǹ����Ǿݵ���ҵ���
		VILL_COUNTRY_GUARD_WITH_BOW		= 16,	//##�ݵ�Ĺ���������ֻ�ǹ����Ǿݵ���ҵ���
		AI_TYPE_PET						= 17,	// ����

		SCRIPT_AI						= 256,  //�ű�AI
		// ��Ҫ��256�Ժ�����κ�ֵ
	};

public:
	typedef CBaseAI *(*AICreateFunc)();
	typedef void (*AIDeleteFunc)( CBaseAI* );
	typedef void (*AIClearFunc)();
	struct FuncList
	{
		AICreateFunc Create;
		AIDeleteFunc Delete;
		AIClearFunc Clear;
		FuncList( AICreateFunc cr = 0, AIDeleteFunc de = 0, AIClearFunc cl = 0 ) : 
			Create( cr ), Delete( de ), Clear( cl )
		{
		}
	};
	typedef std::map<long, FuncList, Private::MyLess> AIFuncTable;
	
	///
	/// RegisterHelper can make the registering automatically. You donot need to call CAIFactory::RegisterAI manually.
	///
	template <typename _Tp>
	struct RegisterHelper
	{
		RegisterHelper( long ai_type )
		{
			CAIFactory::RegisterAI<_Tp>( ai_type ); 
		}
	};

public:

	static CBaseAI *CreateAI( long ai_type );
	static CBaseAI *CreateAI( CMonster *pMonster );
	static bool DeleteAI( CBaseAI *pBaseAI );
	static void Clear();

	template <typename _Tp>
	static void RegisterAI( long ai_type )
	{
		typedef Private::Obj<_Tp, CBaseAI> ObjCreatorType;
		// create a temp obj to generate c++ codes from template
		ObjCreatorType t;
		// avoid the compiler warning
		t;
		// save the ai create function.
		mAIFuncTable[ai_type] = FuncList( ObjCreatorType::Create, ObjCreatorType::Delete, ObjCreatorType::Clear );
	}

	static bool IsScriptAI( long ai_type )
	{
		return ai_type >= SCRIPT_AI;
	}

private:
	static AIFuncTable mAIFuncTable;
};
