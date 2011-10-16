#include "stdafx.h"
#include <MMSystem.h>
#include "CBillSession.h"
#include "CBillSessionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LONG CBillSessionManager::s_lSessionID = 1;

hash_map<CGUID, CBillSession*, hash_guid_compare> CBillSessionManager::s_mSessions;


CBillSession* CBillSessionManager::CreateSession(const char* strCdkey)
{	
	CBillSession* pSession	= NULL;
	pSession = OBJ_CREATE_PVOID( CBillSession, (void*)strCdkey );

	if( pSession )
	{
		CGUID guid;
		CGUID::CreateGUID(guid);		
		pSession -> SetExID( guid );
		s_mSessions[pSession -> GetExID()] = pSession;
	}
	return pSession;
}

const CBillSession* CBillSessionManager::QuerySession(const CGUID& SessionGuid)
{	
	CBillSession* pResult = NULL;
	hash_map<CGUID, CBillSession*>::iterator it = s_mSessions.find( SessionGuid );
	if( it != s_mSessions.end() )
	{
		pResult = it -> second;
	}
	return pResult;
}

VOID CBillSessionManager::AI()
{
	hash_map<CGUID, CBillSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it -> second )
		{
			//##�����������Abort����End֮�󣬽������ֱ��ɾ����
			if( it -> second -> IsSessionShouldBeenRemoved() )
			{
				OBJ_RELEASE( CBillSession, it -> second );
				s_mSessions.erase( it ++ );
			}
			//##���SessionʧЧ��
			else if( it -> second -> IsSessionAvailable() == FALSE )
			{
				it -> second -> Abort();
				OBJ_RELEASE( CBillSession, it -> second );
				s_mSessions.erase( it ++ );
			}
			//##���Session��ʱ�䵽��Զ�������
			else if( it -> second -> IsSessionEnded() )
			{
				it -> second -> End();
				OBJ_RELEASE( CBillSession, it -> second );
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

VOID CBillSessionManager::GarbageCollect_Session(const CGUID& guid)
{
	hash_map<CGUID, CBillSession*>::iterator it = s_mSessions.find( guid );
	if( it != s_mSessions.end() )
	{
		OBJ_RELEASE( CBillSession, it -> second );
		s_mSessions.erase( it );
	}
}

VOID CBillSessionManager::Release()
{	
	hash_map<CGUID, CBillSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it -> second )
		{
			//##�����쳣ֹͣ��Session
			it -> second -> Abort();
			OBJ_RELEASE( CBillSession, it -> second );
		}
		it ++;
	}
	s_mSessions.clear();
}