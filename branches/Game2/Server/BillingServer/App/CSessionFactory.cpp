#include "stdafx.h"
#include "CSessionFactory.h"

#include "CSession.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LONG CSessionFactory::s_lSessionID = 1;

hash_map<CGUID, CSession*, hash_guid_compare> CSessionFactory::s_mSessions;


CSession* CSessionFactory::CreateSession(const char* strCdkey)
{	
	CSession* pSession	= NULL;
	pSession = new CSession( strCdkey );

	if( pSession )
	{
		CGUID guid;
		CGUID::CreateGUID(guid);		
		pSession -> SetExID( guid );
		s_mSessions[pSession -> GetExID()] = pSession;
	}
	return pSession;
}

const CSession* CSessionFactory::QuerySession(const CGUID& SessionGuid)
{	
	CSession* pResult = NULL;
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.find( SessionGuid );
	if( it != s_mSessions.end() )
	{
		pResult = it -> second;
	}
	return pResult;
}

VOID CSessionFactory::AI()
{
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it -> second )
		{
			//##�����������Abort����End֮�󣬽������ֱ��ɾ����
			if( it -> second -> IsSessionShouldBeenRemoved() )
			{
				SAFE_DELETE( it -> second );
				s_mSessions.erase( it ++ );
			}
			//##���SessionʧЧ��
			else if( it -> second -> IsSessionAvailable() == FALSE )
			{
				it -> second -> Abort();
				SAFE_DELETE( it -> second );
				s_mSessions.erase( it ++ );
			}
			//##���Session��ʱ�䵽��Զ�������
			else if( it -> second -> IsSessionEnded() )
			{
				it -> second -> End();
				SAFE_DELETE( it -> second );
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
		SAFE_DELETE( it -> second );
		s_mSessions.erase( it );
	}
}

VOID CSessionFactory::Release()
{	
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it -> second )
		{
			//##�����쳣ֹͣ��Session
			it -> second -> Abort();
			SAFE_DELETE( it -> second );
		}
		it ++;
	}
	s_mSessions.clear();
}