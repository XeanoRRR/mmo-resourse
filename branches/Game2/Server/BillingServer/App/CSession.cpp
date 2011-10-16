#include "stdafx.h"
#include "CSession.h"

#include "CSessionFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSession::CSession(const char* strCdkey)
{
	m_strCdkey=strCdkey;
	m_eBusinessState=eBSTATE_PRELOCK;
}

CSession::~CSession()
{	
	
}

VOID CSession::AI()
{
	//##AI���ֻ������ж����е�Plug�Ƿ���Ч���������Ч��Plug��
	//##��ִ��һЩ��������֪ͨ������Plug�󣬰Ѹ�Plug�ϵ���
	if( m_bSessionStarted == TRUE )
	{
		if( m_bSessionEnded || m_bSessionAborted )
		{
			return;
		}
		if( m_dwLifeTime && timeGetTime() >= m_dwStartingTimeStamp + m_dwLifeTime )
		{
			End();
			return;
		}
		/*
		list<long>::iterator it = m_lPlugs.begin();
		while( it != m_lPlugs.end() )
		{
			CPlug* pPlug = const_cast<CPlug*>( CSessionFactory::QueryPlug(*it) );
			if( pPlug )
			{
				if( pPlug -> IsPlugAvailable() == FALSE )
				{
					OnPlugAborted( *it );

					//##���ո�Plug��ɾ��������
					CSessionFactory::GarbageCollect_Plug( *it );
					it = m_lPlugs.erase( it );
				}
				else if( pPlug -> IsPlugEnded() )
				{
					OnPlugEnded( *it );

					//##���ո�Plug��ɾ��������
					CSessionFactory::GarbageCollect_Plug( *it );
					it = m_lPlugs.erase( it );
				}
				else
				{
					it ++;
				}
			}
			else 
			{
				it = m_lPlugs.erase( it );
			}
		}*/
	}
}

BOOL CSession::Start( BOOL bFlag )
{
	m_dwStartingTimeStamp	= timeGetTime();
	m_bSessionStarted		= TRUE;
	return OnSessionStarted( bFlag );
}

BOOL CSession::End( BOOL bFlag )
{
	m_dwStartingTimeStamp	= 0;
	m_bSessionEnded			= TRUE;
	m_bRemoveFlag			= TRUE;
	return OnSessionEnded( bFlag );
}

BOOL CSession::Abort( BOOL bFlag )
{
	m_dwStartingTimeStamp	= 0;
	m_bSessionAborted		= TRUE;
	m_bRemoveFlag			= TRUE;
	return OnSessionAborted( bFlag );
}


BOOL CSession::OnSessionStarted( BOOL bFlag )
{
	return TRUE;
}


BOOL CSession::OnSessionEnded( BOOL bFlag )
{
	return TRUE;
}

BOOL CSession::OnSessionAborted( BOOL bFlag )
{
	return TRUE;
}

BOOL CSession::IsSessionShouldBeenRemoved()
{
	return m_bRemoveFlag;
}

BOOL CSession::IsSessionAvailable()
{
	//##�ú������������ж�Ŀǰ�ĻỰ�Ƿ�����Ч�ĻỰ��
	//##��Ҫ�Ǹ���Plug��״̬���жϡ�Plug������û�����������
	//##�û��쳣�˳�������»᷵��FALSE��

	BOOL bResult = FALSE;
	DWORD dwAvailablePlugAmount = 0;
	
	return bResult;
}

BOOL CSession::IsSessionEnded()
{
	BOOL bResult = FALSE;
	if( m_bSessionStarted && m_bSessionEnded )
	{
		bResult = TRUE;
	}
	return bResult;
}