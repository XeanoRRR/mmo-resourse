#include "StdAfx.h"
//#include "..\..\nets\networld\message.h"
////#include "cteam.h"
////#include "cteamate.h"
//#include "csessionfactory.h"
//#include <mmsystem.h>
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
///*
//CTeam::CTeam(LONG lID, DWORD dwMinPlugs, DWORD dwMaxPlugs, DWORD dwLifeTime):CSession(lID, dwMinPlugs, dwMaxPlugs, dwLifeTime)
//{
//	m_dwLastCheckedTimeStamp	= 0;
//	m_lTeamLeaderID				= 0;
//	m_dwSessionType				= CSessionFactory::ST_TEAM;
//	m_asAllocationScheme		= CTeam::AS_EXCLUSIVE;
//
//	m_lDelayTime				= 125;		//125��AI
//}
//
//CTeam::CTeam(DWORD dwMinPlugs, DWORD dwMaxPlugs, DWORD dwLifeTime):CSession(dwMinPlugs, dwMaxPlugs, dwLifeTime)
//{
//	m_dwLastCheckedTimeStamp	= 0;
//	m_lTeamLeaderID				= 0;
//	m_dwSessionType				= CSessionFactory::ST_TEAM;
//	m_asAllocationScheme		= CTeam::AS_EXCLUSIVE;
//
//	m_lDelayTime				= 125;		//125��AI
//}
//
//CTeam::~CTeam(void)
//{
//	Release();
//}
//
////##���ڶӳ��Ĵ�λ,ֻ���ɶӳ����ڵķ����������.����ӳ��ķ�������Ϊ崻������޷���λ,����WorldServer������һ�Ρ�
//VOID CTeam::SetTeamID( DWORD dwID )
//{
//	m_dwTeamID = dwID;
//}
//
//DWORD CTeam::GetTeamID()
//{
//	return m_dwTeamID;
//}
//
//VOID CTeam::SetLeader( LONG lPlayerID )
//{
//	CPlug* pTeamate		= QueryPlugByOwner( TYPE_PLAYER, lPlayerID );
//	BOOL bSendToTrigger = TRUE;
//	if( IsSessionEnded() == FALSE && pTeamate )
//	{
//		LONG lPlugID = pTeamate -> GetID();
//		if( m_lTeamLeaderID )
//		{
//			CPlug* pPlug = QueryPlugByOwner( TYPE_PLAYER, m_lTeamLeaderID );
//			if( pPlug )
//			{
//				lPlugID = pPlug -> GetID();
//				bSendToTrigger = FALSE;
//			}
//		}
//		m_lTeamLeaderID = lPlayerID;
//		OnPlugChangeState( lPlugID, CTeamate::TS_CHANGE_LEADER, reinterpret_cast<BYTE*>(&lPlayerID), bSendToTrigger );
//	}
//}
//
//LONG CTeam::GetLeader()
//{
//	return m_lTeamLeaderID;
//}
//
//CHAR* CTeam::GetPassword()
//{
//	return const_cast<CHAR*>( m_strPassword.c_str() );
//}
//
//VOID CTeam::SetPassword( CHAR* pPassword )
//{
//	if( pPassword )
//	{
//		m_strPassword = pPassword;
//	}
//}
//
//CHAR* CTeam::GetTeamName()
//{
//	return const_cast<CHAR*>( m_strTeamName.c_str() );
//}
//
//VOID CTeam::SetTeamName( CHAR* pTeamName )
//{
//	if( pTeamName )
//	{
//		m_strTeamName = pTeamName;
//	}
//}
//
//BOOL CTeam::IsSessionAvailable()
//{
//	if(m_lDelayTime > 0)
//		return true;
//	return CSession::IsSessionAvailable();
//}
//
//VOID CTeam::AI()
//{
//	CSession::AI();
//
//	if(m_lDelayTime	> 0)
//	{
//		m_lDelayTime--;
//	}
//	//##��ѯ�ӳ�.����ӳ�������.��֪ͨ���е�Plug���Ķӳ���
//	if( m_dwLastCheckedTimeStamp )
//	{
//		if( timeGetTime() >= m_dwLastCheckedTimeStamp + 60000  )
//		{
//			//##����Ҳ����ӳ�.
//			if( QueryPlugByOwner(TYPE_PLAYER, m_lTeamLeaderID) == NULL )
//			{
//				list<LONG>::iterator it = GetPlugList().begin();
//				if( it != GetPlugList().end() )
//				{
//					CPlug* pTeamate = QueryPlugByID( *it );
//					if( pTeamate )
//					{
//						SetLeader( pTeamate -> GetOwnerID() );
//
//						//##������Ϣ��֪ͨGameserver
//					}
//				}
//			}
//
//			m_dwLastCheckedTimeStamp = timeGetTime();
//		}
//	}
//	else
//	{
//		m_dwLastCheckedTimeStamp = timeGetTime();
//	}
//}
//
//VOID CTeam::OnProcessQueryMessage( LONG lRegionID, LONG lPlayerID )
//{
//	vector<tagTeamateInfo>::iterator it = m_vQueriedPlugs.begin();
//	for( ; it != m_vQueriedPlugs.end(); )
//	{
//		if( it -> lPlayerID == lPlayerID )
//		{
//			if( it -> lRegionID != lRegionID )
//			{
//				CTeamate* pTeamate = dynamic_cast<CTeamate*>( QueryPlugByOwner(TYPE_PLAYER, lPlayerID) );
//				if( pTeamate )
//				{
//					pTeamate -> SetOwnerRegionID( lRegionID );
//
//					//##���͸������Plug��֪ͨ��Plug�ĳ����ı䡣
//				}
//			}
//
//			it = m_vQueriedPlugs.erase( it );
//			break;
//		}
//		else
//		{
//			it ++;
//		}
//	}
//}
//
//VOID CTeam::SetAllocationScheme( CTeam::ALLOCATION_SCHEME as )
//{
//	m_asAllocationScheme = as;
//
//	//##֪ͨ���е�Plug״̬�ı�
//	OnPlugChangeState( m_lTeamLeaderID, CTeamate::TS_CHANGE_ALLOCATION_SCHEME, reinterpret_cast<BYTE*>(&as), FALSE );
//}
//
//CTeam::ALLOCATION_SCHEME CTeam::GetAllocationScheme()
//{
//	return m_asAllocationScheme;
//}
//
//BOOL CTeam::Serialize( vector<BYTE>* pStream )
//{
//	BOOL bResult = TRUE;
//
//	CSession::Serialize( pStream );
//
//	//##Session����Ϣ
//	_AddToByteArray( pStream, m_dwTeamID );
//	_AddToByteArray( pStream, const_cast<CHAR*>( m_strTeamName.c_str() ) );
//	_AddToByteArray( pStream, const_cast<CHAR*>( m_strPassword.c_str() ) );
//	_AddToByteArray( pStream, m_lTeamLeaderID );
//
//	//##Plug����Ϣ
//	DWORD dwNumTeamates = GetPlugList().size();
//	_AddToByteArray( pStream, dwNumTeamates );
//
//	list<LONG>::iterator it = GetPlugList().begin();
//	for( ; it != GetPlugList().end(); it ++ )
//	{
//		CPlug* pPlug = QueryPlugByID( *it );
//		if( pPlug )
//		{
//			pPlug -> Serialize( pStream );
//		}
//		else 
//		{
//			bResult = FALSE;
//			break;
//		}
//	}
//
//	return bResult;
//}
//
//VOID CTeam::KickPlayer( LONG lPlayerID )
//{
//	CGame::tagGameServer* pServer = GetGame() -> GetPlayerGameServer( lPlayerID );
//	if( pServer )
//	{
//		CMessage msg( MSG_W2S_TEAM_KICK_PLAYER );
//		msg.Add( GetTeamID() );
//		msg.Add( lPlayerID );
//		msg.SendGSID( pServer -> dwIndex );
//	}
//}
//
//BOOL CTeam::Unserialize( BYTE* pStream, LONG& lOffset )
//{
//	BOOL bResult = TRUE;
//	CSession::Unserialize( pStream, lOffset );
//	if( pStream )
//	{
//		CHAR szMsg[256];
//		m_dwTeamID		= _GetDwordFromByteArray( pStream, lOffset );
//		m_strTeamName	= _GetStringFromByteArray( pStream, lOffset, szMsg );
//		m_strPassword	= _GetStringFromByteArray( pStream, lOffset, szMsg );
//		m_lTeamLeaderID	= _GetLongFromByteArray( pStream, lOffset );
//
//		Start();
//
//		//##��ȡ��Ա����Ϣ
//		DWORD dwNumTeamates = _GetDwordFromByteArray( pStream, lOffset );
//		for( DWORD i = 0; i < dwNumTeamates; i ++ )
//		{
//			LONG lPlugID = CSessionFactory::UnserializePlug( pStream, lOffset );
//			if( lPlugID )
//			{
//				CSessionFactory::InsertPlug( GetID(), lPlugID );
//			}
//			else
//			{
//				bResult = FALSE;
//				break;
//			}
//		}
//	}
//	return bResult;
//}
//
//BOOL CTeam::OnSessionStarted()
//{
//	//##��Ӷ��������
//	//GetGame() -> AddTeam( m_dwTeamID, GetID() );
//
//	//##������Ϣ��������GameServer
//	//##��������Ҫ����,��Ϊֻ����ͬһ��GameServer�Ż���������Ҫ��
//
//	return TRUE;
//}
//
//
//BOOL CTeam::OnSessionEnded()
//{
//	//##���͸����е�GameServer
//	//CMessage msg( MSG_W2S_TEAM_DESTORY );
//	//msg.Add( GetTeamID() );
//	//msg.SendAll();
//
//	//##������Ϣ��World Server
//	//GetGame() -> RemoveTeam( m_dwTeamID );
//
//	return TRUE;
//}
//
//BOOL CTeam::OnSessionAborted()
//{
//	//##���͸����е�GameServer
//	//CMessage msg( MSG_W2S_TEAM_DESTORY );
//	//msg.Add( GetTeamID() );
//	//msg.SendAll();
//
//	////##������Ϣ��World Server
//	//GetGame() -> RemoveTeam( m_dwTeamID );
//
//	return TRUE;
//}
//
//VOID CTeam::GetGameServers( vector<DWORD>& vOut, LONG lTriggerPlugID )
//{
//	//##����ȷ����������¼���Plug��ServerID
//	DWORD dwTriggerPlugGameServerIndex = 0xFFFFFFFF;
//	CPlug* pPlug = const_cast<CPlug*>( CSessionFactory::QueryPlug(lTriggerPlugID) );
//	if( pPlug )
//	{
//		CGame::tagGameServer* pServer = GetGame() -> GetPlayerGameServer( pPlug -> GetOwnerID() );
//		if( pServer )
//		{
//			dwTriggerPlugGameServerIndex = pServer -> dwIndex;
//		}
//	}
//
//	//##��Σ���ѯ���е�Plug
//	list<LONG>::iterator it = GetPlugList().begin();
//	for( ; it != GetPlugList().end(); it ++ )
//	{
//		if( *it != lTriggerPlugID )
//		{
//			CPlug* pPlug = QueryPlugByID( *it );
//			if( pPlug )
//			{
//				CGame::tagGameServer* pServer = GetGame() -> GetPlayerGameServer( pPlug -> GetOwnerID() );
//				if( pServer && pServer -> dwIndex != dwTriggerPlugGameServerIndex )
//				{
//					if( find( vOut.begin(), vOut.end(), pServer -> dwIndex ) == vOut.end() )
//					{
//						vOut.push_back( pServer -> dwIndex );
//					}
//				}
//			}
//		}
//	}
//}
//
//BOOL CTeam::OnPlugChangeState(LONG lPlugID, LONG lState, BYTE* pData, BOOL bSendToTrigger)
//{
//	BOOL bResult = CSession::OnPlugChangeState( lPlugID, lState, pData, bSendToTrigger );
//	if( bResult )
//	{
//		//##�����Ҫ���͸�����GameServer�����͡�
//		switch( lState )
//		{
//		case CTeamate::TS_CHANGE_ALLOCATION_SCHEME:
//			{
//				if( pData )
//				{
//					CTeam::ALLOCATION_SCHEME as = CTeam::AS_FORCE_DWORD;
//					memcpy( &as, pData, sizeof(CTeam::ALLOCATION_SCHEME) );
//
//					vector<DWORD> vOut;
//					GetGameServers( vOut, lPlugID );
//
//					CMessage msg( MSG_W2S_TEAM_CHANGE_ALLOCATION_SCHEME );
//					msg.Add( GetTeamID() );
//					msg.Add( LONG(as) );
//					
//					for( size_t i = 0; i < vOut.size(); i ++ )
//					{
//						msg.SendGSID( vOut[i] );
//					}
//				}
//			}
//			break;
//		case CTeamate::TS_CHANGE_LEADER:
//			{
//				if( pData )
//				{
//					LONG lNewLeaderID = 0;
//					memcpy( &lNewLeaderID, pData, sizeof(LONG) );
//					if( lNewLeaderID && QueryPlugByOwner(TYPE_PLAYER, lNewLeaderID) )
//					{
//						vector<DWORD> vOut;
//
//						if( bSendToTrigger )
//						{
//							GetGameServers( vOut );
//						}
//						else
//						{
//							GetGameServers( vOut, lPlugID );
//						}
//
//						CMessage msg( MSG_W2S_TEAM_CHANGE_LEAER );
//						msg.Add( GetTeamID() );
//						msg.Add( lNewLeaderID );
//						
//						for( size_t i = 0; i < vOut.size(); i ++ )
//						{
//							msg.SendGSID( vOut[i] );
//						}
//					}
//				}
//			}
//			break;
//		case CTeamate::TS_CHANGE_REGION:
//			{
//				if( pData )
//				{
//					CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
//					if( pPlug )
//					{
//						LONG lNewRegionID = 0;
//						memcpy( &lNewRegionID, pData, sizeof(LONG) );
//						if( lNewRegionID )
//						{
//							vector<DWORD> vOut;
//							GetGameServers( vOut, -1 );
//
//							CMessage msg( MSG_W2S_TEAM_TEAMATE_CHANGE_REGION );
//							msg.Add( GetTeamID() );
//							msg.Add( pPlug -> GetOwnerType() );
//							msg.Add( pPlug -> GetOwnerID() );
//							msg.Add( lNewRegionID );
//
//							for( size_t i = 0; i < vOut.size(); i ++ )
//							{
//								msg.SendGSID( vOut[i] );
//							}
//						}
//					}
//				}
//			}
//			break;
//		case CTeamate::TS_CHANGE_STATE:
//			{
//				if( pData )
//				{
//					CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
//					if( pPlug )
//					{
//						FLOAT fHp = 0;
//						memcpy( &fHp, pData, sizeof(FLOAT) );
//
//						vector<DWORD> vOut;
//						GetGameServers( vOut, lPlugID );
//
//						CMessage msg( MSG_W2S_TEAM_CHANGE_STATE );
//						msg.Add( GetTeamID() );
//						msg.Add( pPlug -> GetOwnerType() );
//						msg.Add( pPlug -> GetOwnerID() );
//						msg.Add( fHp );
//
//						for( size_t i = 0; i < vOut.size(); i ++ )
//						{
//							msg.SendGSID( vOut[i] );
//						}
//					}
//				}
//			}
//			break;
//		case CTeamate::TS_CHAT:
//			{
//				if( pData )
//				{
//					CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
//					if( pPlug )
//					{
//						vector<DWORD> vOut;
//						GetGameServers( vOut, lPlugID );
//
//						CMessage msg( MSG_W2S_TEAM_CHAT );
//						msg.Add( GetTeamID() );
//						msg.Add( pPlug -> GetOwnerType() );
//						msg.Add( pPlug -> GetOwnerID() );
//						msg.Add( reinterpret_cast<CHAR*>(pData) );
//
//						for( size_t i = 0; i < vOut.size(); i ++ )
//						{
//							msg.SendGSID( vOut[i] );
//						}
//					}
//				}
//			}
//			break;
//		case CTeamate::PS_INSERTED:
//			{
//				//##�ɸ�������ڵķ�������������Ϣ��WorldServer��
//				CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
//				if( pPlug )
//				{
//					vector<DWORD> vOut;
//					GetGameServers( vOut, lPlugID );
//
//					CMessage msg( MSG_W2S_TEAM_TEAMATE_JOIN );
//					msg.Add( GetTeamID() );
//					msg.Add( pPlug -> GetOwnerType() );
//					msg.Add( pPlug -> GetOwnerID() );
//					msg.Add( pPlug -> GetOwnerRegionID() );
//					msg.Add( pPlug -> GetOwnerName() );
//
//					for( size_t i = 0; i < vOut.size(); i ++ )
//					{
//						msg.SendGSID( vOut[i] );
//					}
//				}
//			}
//			break;
//		case CTeamate::PS_REMOVED:
//		case CTeamate::PS_ABORTED:
//			{
//				//##�ɸ�������ڵķ�������������Ϣ��WorldServer��
//				CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
//				if( pPlug )
//				{
//					vector<DWORD> vOut;
//					GetGameServers( vOut, lPlugID );
//
//					CMessage msg( MSG_W2S_TEAM_TEAMATE_LEAVE );
//					msg.Add( GetTeamID() );
//					msg.Add( pPlug -> GetOwnerType() );
//					msg.Add( pPlug -> GetOwnerID() );
//
//					for( size_t i = 0; i < vOut.size(); i ++ )
//					{
//						msg.SendGSID( vOut[i] );
//					}
//				}
//			}
//			break;
//		}
//	}
//	return bResult;
//}
//*/