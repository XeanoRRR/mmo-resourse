// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include <MMSystem.h>
#include "CSession.h"
#include "CPlug.h"
#include "CSessionFactory.h"
#include "CTrader.h"
#include "CPersonalShopSeller.h"
#include "CPersonalShopBuyer.h"

#include "CEquipmentEnchase.h"

//##ModelId=41DDF142029F
LONG CSessionFactory::s_lSessionID = 1;

//##ModelId=41EE17AC0128
LONG CSessionFactory::s_lPlugID = 1;

//##ModelId=41DDF1B40196
hash_map<CGUID, CSession*, hash_guid_compare> CSessionFactory::s_mSessions;

//##ModelId=41EE15F801C5
hash_map<long, CPlug*> CSessionFactory::s_mPlugs;

//##ModelId=41DDF2410119
const CGUID& CSessionFactory::CreateSession(DWORD dwMinPlugs, DWORD dwMaxPlugs, DWORD dwLifeTime, SESSION_TYPE st )
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CSession* pSession	= NULL;

	switch( st )
	{
	case ST_NORMAL_SESSION:
		{
			pSession = MP_NEW CSession(dwMinPlugs, dwMaxPlugs, dwLifeTime);
		}
		break;
	}
	if( pSession )
	{
		CGUID guid;
		CGUID::CreateGUID(guid);
		pSession -> SetType( TYPE_SESSION );
		pSession -> SetExID( guid );
		s_mSessions[pSession -> GetExID()] = pSession;
	}
	return pSession -> GetExID();
}

const CSession* CSessionFactory::QuerySession(const CGUID& SessionGuid)
{
	// TODO: Add your specialized code here.
	CSession* pResult = NULL;
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.find( SessionGuid );
	if( it != s_mSessions.end() )
	{
		pResult = it -> second;
	}
	return pResult;
}

BOOL CSessionFactory::InsertPlug(const CGUID& SessionGuid, long  lPlugID)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	CSession* pSession = const_cast<CSession*>( QuerySession(SessionGuid) );
	if( pSession )
	{
		CPlug* pPlug = const_cast<CPlug*>( QueryPlug(lPlugID) );
		bResult = pSession -> InsertPlug( lPlugID );
	}
	return bResult;
}


VOID CSessionFactory::AI()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it -> second )
		{
			//##�����������Abort����End֮�󣬽������ֱ��ɾ����
			if( it -> second -> IsSessionShouldBeenRemoved() )
			{
				MP_DELETE(it -> second );
				s_mSessions.erase( it ++ );
			}
			//##���SessionʧЧ��
			else if( it -> second -> IsSessionAvailable() == FALSE )
			{
				it -> second -> Abort();
				MP_DELETE(it -> second );
				s_mSessions.erase( it ++ );
			}
			//##���Session��ʱ�䵽��Զ�������
			else if( it -> second -> IsSessionEnded() )
			{
				it -> second -> End();
				MP_DELETE(it -> second );
				s_mSessions.erase( it ++ );
			}
			else 
			{
				it -> second -> AI();
				it ++;
			}
		}
		else 
		{
			s_mSessions.erase( it ++ );
		}
	}	
}

VOID CSessionFactory::GarbageCollect_Session(const CGUID& guid)
{
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.find( guid );
	if( it != s_mSessions.end() )
	{
		MP_DELETE(it -> second );
		s_mSessions.erase( it );
	}
}
VOID CSessionFactory::GarbageCollect_Plug(long lPlugID)
{
	hash_map<long, CPlug*>::iterator it = s_mPlugs.find( lPlugID );
	if( it != s_mPlugs.end() )
	{
		MP_DELETE(it -> second );
		s_mPlugs.erase( it );
	}
}

VOID CSessionFactory::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it -> second )
		{
			//##�����쳣ֹͣ��Session
			it -> second -> Abort();
			MP_DELETE(it -> second );
		}
		it ++;
	}
	s_mSessions.clear();
}

long CSessionFactory::CreatePlug( PLUG_TYPE pt, LONG lType, const CGUID& guid )
{
	// TODO: Add your specialized code here.
	LONG lResult = 0;
	CPlug* pPlug = NULL;
	switch( pt )
	{
	case PT_TRADER:
		pPlug = MP_NEW CTrader;
		break;
	case PT_PERSONAL_SHOP_SELLER:
		pPlug = MP_NEW CPersonalShopSeller;
		break;
	case PT_PERSONAL_SHOP_BUYER:
		pPlug = MP_NEW CPersonalShopBuyer;
		break;
	case PT_EQUIPMENT_ENCHASE:
		pPlug=MP_NEW CEquipmentEnchase;
		break;
	}
	if( pPlug )
	{
		pPlug -> SetType( TYPE_PLUG );
		pPlug -> Set32ID( CPlug::Create32ID());
		pPlug -> SetOwner( lType, guid );
		s_mPlugs[pPlug -> Get32ID()] = pPlug;
	}
	return pPlug -> Get32ID();
}

//##ModelId=41EE15B80261
const CPlug* CSessionFactory::QueryPlug(long lPlugID)
{
	// TODO: Add your specialized code here.
	CPlug* pResult = NULL;
	hash_map<long, CPlug*>::iterator it = s_mPlugs.find( lPlugID );
	if( it != s_mPlugs.end() )
	{
		pResult = it -> second;
	}
	return pResult;
}

long CSessionFactory::UnserializePlug( BYTE* pStream, LONG& lOffset )
{
	long lResultID = 0;
	if( pStream )
	{
		PLUG_TYPE pt	= static_cast<PLUG_TYPE>( _GetDwordFromByteArray(pStream, lOffset) );
		LONG lOwnerType	= _GetLongFromByteArray( pStream, lOffset );
		CGUID OwnerID;
		_GetBufferFromByteArray(pStream, lOffset, OwnerID);

		//##��������
		long lPlugID = CreatePlug( pt, lOwnerType, OwnerID );

		CPlug* pPlug = const_cast<CPlug*>( QueryPlug(lPlugID) );
		if( pPlug )
		{
			if( pPlug -> Unserialize(pStream, lOffset) )
			{
				lResultID = lPlugID;
			}
			else
			{
				GarbageCollect_Plug( lPlugID );
			}
		}
	}
	return lResultID;
}

CGUID CSessionFactory::UnserializeSession( BYTE* pStream, LONG& lOffset )
{
	CGUID ResultGuid;
	if( pStream )
	{
		SESSION_TYPE st		= static_cast<SESSION_TYPE>( _GetDwordFromByteArray(pStream, lOffset) );
		DWORD dwMinimumPlug = _GetDwordFromByteArray( pStream, lOffset );
		DWORD dwMaximumPlug = _GetDwordFromByteArray( pStream, lOffset );
		DWORD dwLifeTime	= _GetDwordFromByteArray( pStream, lOffset );

		//##����Session
		const CGUID& SessionGuid		= CreateSession( dwMinimumPlug, dwMaximumPlug, dwLifeTime, st );
		CSession* pSession	= const_cast<CSession*>( QuerySession(SessionGuid) );
		if( pSession )
		{
			//##Session��Unserialize�ڲ���Plug֮ǰҪ����������Session
			if( pSession -> Unserialize(pStream, lOffset) )
			{
				ResultGuid = SessionGuid;
			}
			else
			{
				GarbageCollect_Session( SessionGuid );
			}
		}
	}
	return ResultGuid;
}