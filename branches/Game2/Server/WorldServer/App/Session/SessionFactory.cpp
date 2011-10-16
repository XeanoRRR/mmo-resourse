//CSessionFactory.cpp/////////////////////////////////////////////////////////////////////
//������:�Ự���������
//Author:������
//Create Time:2008.11.03
#include "StdAfx.h"
#include "SessionFactory.h"

#include "Session.h"
#include "WorldServerSession.h"

//##ModelId=41DDF142029F
LONG CSessionFactory::s_lSessionID = 1;


//##ModelId=41DDF1B40196
hash_map<CGUID, CSession*, hash_guid_compare> CSessionFactory::s_mSessions;

//##ModelId=41DDF2410119
CSession* CSessionFactory::CreateSession(DWORD dwLifeTime, SESSION_TYPE st )
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CSession* pSession	= NULL;

	switch( st )
	{
	case ST_WS_SESSION:
		{
			pSession = new CWorldServerSession( dwLifeTime );
		}
		break;
		//case ST_TEAM:
		//	{
		//		pSession = new CTeam( dwMinPlugs, dwMaxPlugs, dwLifeTime );
		//	}
		//break;
	}
	if( pSession )
	{
		CGUID guid;
		CGUID::CreateGUID(guid);
		pSession->SetSessionType( st );
		pSession->SetExID( guid );
		s_mSessions[pSession->GetExID()] = pSession;
	}
	return pSession;
}

//##ModelId=41DDF445000F
const CSession* CSessionFactory::QuerySession(const CGUID& SessionGuid)
{
	// TODO: Add your specialized code here.
	CSession* pResult = NULL;
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.find( SessionGuid );
	if( it != s_mSessions.end() )
	{
		pResult = it->second;
	}
	return pResult;
}

//##ModelId=41DDF3B1036B
VOID CSessionFactory::AI()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it->second )
		{
			//##�����������Abort����End֮�󣬽������ֱ��ɾ����
			if( it->second->IsSessionShouldBeenRemoved() )
			{
				SAFE_DELETE( it->second );
				s_mSessions.erase( it ++ );
			}
			//##���SessionʧЧ��
			else if( it->second->IsSessionAvailable() == FALSE )
			{
				it->second->Abort();
				SAFE_DELETE( it->second );
				s_mSessions.erase( it ++ );
			}
			//##���Session��ʱ�䵽��Զ�������
			else if( it->second->IsSessionEnded() )
			{
				it->second->End();
				SAFE_DELETE( it->second );
				s_mSessions.erase( it ++ );
			}
			else 
			{
				it->second->AI();
				it ++;
			}
		}
		else 
		{
			s_mSessions.erase( it ++ );
		}
	}
}

//##ModelId=41DDF26E0232
VOID CSessionFactory::GarbageCollect_Session(const CGUID& guid)
{
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.find( guid );
	if( it != s_mSessions.end() )
	{
		SAFE_DELETE( it->second );
		s_mSessions.erase( it );
	}
}
VOID CSessionFactory::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	hash_map<CGUID, CSession*>::iterator it = s_mSessions.begin();
	while( it != s_mSessions.end() )
	{
		if( it->second )
		{
			//##�����쳣ֹͣ��Session
			it->second->Abort();
			SAFE_DELETE( it->second );
		}
		it ++;
	}
	s_mSessions.clear();
}
//! �������
void CSessionFactory::OutSessionInfo(VOID)
{
// 	char szInfo[1024] = {0};	
// 	sprintf(szInfo, "SessionNum = %d.", s_mSessions.size());
// 	PutStringToFile("MemAllocInfo", szInfo);

    Log4c::Trace(ROOT_MODULE, "SessionNum = %d.", s_mSessions.size());
}