
#include "StdAfx.h"
/*
#include "cteam.h"
#include "cteamate.h"
#include "csessionfactory.h"
#include <mmsystem.h>
#include "..\Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

map<DWORD, CTeam::tagQueryResult> CTeam::s_mQuestedTeams;

CTeam::CTeam(LONG lID, DWORD dwMinPlugs, DWORD dwMaxPlugs, DWORD dwLifeTime):CSession(lID, dwMinPlugs, dwMaxPlugs, dwLifeTime)
{
	m_dwSessionType				= CSessionFactory::ST_TEAM;
	m_asAllocationScheme		= AS_SHARE;
	m_dwLastCheckedTimeStamp	= 0;
	m_lTeamLeaderID				= 0;
}

CTeam::CTeam(DWORD dwMinPlugs, DWORD dwMaxPlugs, DWORD dwLifeTime):CSession(dwMinPlugs, dwMaxPlugs, dwLifeTime)
{
	m_dwSessionType				= CSessionFactory::ST_TEAM;
	m_asAllocationScheme		= AS_SHARE;
	m_dwLastCheckedTimeStamp	= 0;
	m_lTeamLeaderID				= 0;
}

CTeam::~CTeam(void)
{
	Release();
}

CTeam::tagQueryResult::tagQueryResult() : dwQueryTimeStamp(0), bQueried(FALSE), bSucceed(FALSE)
{
}

VOID CTeam::QuestTeamData( DWORD dwID )
{
	tagQueryResult tResult;
	s_mQuestedTeams[dwID] = tResult;
}

VOID CTeam::CompleteTeamData( DWORD dwID )
{
	map<DWORD,tagQueryResult>::iterator it = s_mQuestedTeams.find( dwID );
	if( it != s_mQuestedTeams.end() )
	{
		it -> second.bSucceed = TRUE;
	}
}

//##���ڶӳ��Ĵ�λ,ֻ���ɶӳ����ڵķ����������.����ӳ��ķ�������Ϊ崻������޷���λ,����WorldServer������һ�Ρ�
VOID CTeam::SetTeamID( DWORD dwID )
{
	m_dwTeamID = dwID;
}

DWORD CTeam::GetTeamID()
{
	return m_dwTeamID;
}

VOID CTeam::SetLeader( LONG lPlayerID )
{
	CPlug* pTeamate = QueryPlugByOwner( TYPE_PLAYER, lPlayerID );
	if( IsSessionEnded() == FALSE && pTeamate )
	{
		LONG lPlugID = pTeamate -> GetID();
		if( m_lTeamLeaderID )
		{
			CPlug* pPlug = QueryPlugByOwner( TYPE_PLAYER, m_lTeamLeaderID );
			if( pPlug )
			{
				lPlugID = pPlug -> GetID();
			}
		}
		m_lTeamLeaderID = lPlayerID;
		OnPlugChangeState( lPlugID, CTeamate::TS_CHANGE_LEADER, reinterpret_cast<BYTE*>(&lPlayerID), TRUE );
	}
}

LONG CTeam::GetLeader()
{
	return m_lTeamLeaderID;
}

CHAR* CTeam::GetPassword()
{
	return const_cast<CHAR*>( m_strPassword.c_str() );
}

VOID CTeam::SetPassword( CHAR* pPassword )
{
	if( pPassword )
	{
		m_strPassword = pPassword;
	}
}

CHAR* CTeam::GetTeamName()
{
	return const_cast<CHAR*>( m_strTeamName.c_str() );
}

VOID CTeam::SetTeamName( CHAR* pTeamName )
{
	if( pTeamName )
	{
		m_strTeamName = pTeamName;
	}
}

VOID CTeam::AI()
{
	CSession::AI();

	//##��Ҫ�������ж���Session���Ƿ���Plug�Ƿ��ڱ������������һ����Ա��û�У���ɾ�����Session
	if( m_dwLastCheckedTimeStamp )
	{
		if( timeGetTime() >= m_dwLastCheckedTimeStamp + 60000 )
		{
			//##ȷ����Session�Ƿ���Ч��
			BOOL bSessionValid = FALSE;
			list<LONG>::iterator it = GetPlugList().begin();
			for( ; it != GetPlugList().end(); it ++ )
			{
				CTeamate* pTeamate = dynamic_cast<CTeamate*>( const_cast<CPlug*>( CSessionFactory::QueryPlug( (*it) ) ) );
				if( pTeamate )
				{
					if( GetGame() -> FindPlayer( pTeamate -> GetOwnerID() ) )
					{
						bSessionValid = TRUE;
						break;
					}
				}
			}

			//##����Ự�Ѿ���Ч����ɾ����Session��
			if( bSessionValid == FALSE )
			{
				Abort();
			}
		}
	}
	else
	{
		m_dwLastCheckedTimeStamp = timeGetTime();
	}
}

DWORD CTeam::GetTeamatesAmount()
{
	DWORD dwResult = 0;
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CPlug* pPlug = QueryPlugByID( *it );
		if( pPlug )
		{
			++ dwResult;
		}
	}
	return dwResult;
}

DWORD CTeam::GetCurrentServerTeamatesAmount()
{
	DWORD dwResult = 0;
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CPlug* pPlug = QueryPlugByID( *it );
		if( pPlug )
		{
			if( pPlug -> GetOwner() )
			{
				++ dwResult;
			}
		}
	}
	return dwResult;
}

//���������һ������
VOID CTeam::AddQuest(WORD wQuestID,CPlayer* pPlayer,long lDistance)
{
	if(pPlayer == NULL)	return;

	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CPlug* pPlug = QueryPlugByID( *it );
		if( pPlug && pPlug->GetOwnerType() == TYPE_PLAYER)
		{
			CPlayer* pTemptPlayer = GetGame()->FindPlayer( pPlug->GetOwnerID() );
			if( pTemptPlayer )
			{
				if(lDistance == 0)
				{
					pTemptPlayer->AddQuest(wQuestID);
				}
				else
				{
					if( pPlayer->Distance(pTemptPlayer) <= lDistance)
					{
						pTemptPlayer->AddQuest(wQuestID);
					}
				}
			}
			else
			{
				if(lDistance == 0)
				{
					CMessage msg(MSG_S2W_QUEST_PlayerAddQuest);
					msg.Add( pPlug->GetOwnerID() );
					msg.Add(wQuestID);
					msg.Send();
				}
			}
		}
	}
}

//���������һ���ű�
VOID CTeam::RunScript(char* strScirptName,CPlayer* pPlayer,long lDistance)
{
	if(pPlayer == NULL)	return;

	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CPlug* pPlug = QueryPlugByID( *it );
		if( pPlug && pPlug->GetOwnerType() == TYPE_PLAYER)
		{
			CPlayer* pTemptPlayer = GetGame()->FindPlayer( pPlug->GetOwnerID() );
			if( pTemptPlayer && pTemptPlayer->IsDied() == false)
			{
				if(lDistance == 0)
				{
					pTemptPlayer->PlayerRunScript(strScirptName);
				}
				else
				{
					if( pPlayer->Distance(pTemptPlayer) <= lDistance)
					{
						pTemptPlayer->PlayerRunScript(strScirptName);
					}
				}
			}
			else
			{
				if(lDistance == 0)
				{
					CMessage msg(MSG_S2W_QUEST_PlayerRunScript);
					msg.Add( pPlug->GetOwnerID() );
					msg.Add(strScirptName);
					msg.Send();
				}
			}
		}
	}
}

DWORD CTeam::GetCurrentRegionTeamatesAmount( LONG lRegionID )
{
	DWORD dwResult = 0;
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(*it) );
		if( pPlug )
		{
			if( pPlug -> GetOwnerRegionID() == lRegionID )
			{
				++ dwResult;
			}
		}
	}
	return dwResult;
}

// ��ȡ��ǰ�����л��ŵĶ���
DWORD CTeam::GetCurrentRegionTeamatesAmount_Alive( LONG lRegionID )
{
	DWORD dwResult = 0;
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(*it) );
		if( pPlug )
		{
			if( pPlug -> GetOwnerRegionID() == lRegionID )
			{
				CMoveShape* pShape = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
				if (pShape && pShape->IsDied() == false)
				{
					++ dwResult;
				}
			}
		}
	}
	return dwResult;
}

//�õ���������ڵ�ǰ�����ĳ�Ա
VOID CTeam::GetCurrentRegionTeamates(map<long,CPlayer*>& RegionTeamates,LONG lRegionID)
{
	if(lRegionID <= 0)	return;

	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(*it) );
		if( pPlug )
		{
			CPlayer* pTeamate = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
			if( pTeamate && pTeamate->GetRegionID() == lRegionID)
			{
				RegionTeamates[pTeamate->GetID()] = pTeamate;
			}
		}
	}
}

//�õ���������ڵ�ǰ�����Ļ��ŵĳ�Ա
VOID CTeam::GetCurrentRegionTeamates_Alive(map<long,CPlayer*>& RegionTeamates,LONG lRegionID)
{
	if(lRegionID <= 0)	return;

	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(*it) );
		if( pPlug )
		{
			CPlayer* pTeamate = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
			if( pTeamate && pTeamate->GetRegionID() == lRegionID && pTeamate->IsDied() == false)
			{
				RegionTeamates[pTeamate->GetID()] = pTeamate;
			}
		}
	}
}

FLOAT CTeam::GetCurrentRegionTeamatesAverageLevel( LONG lRegionID )
{
	FLOAT fResult = 0.0f;
	DWORD dwCount = 0;
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(*it) );
		if( pPlug )
		{
			if( pPlug -> GetOwnerRegionID() == lRegionID )
			{
				CMoveShape* pShape = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
				if( pShape )
				{
					fResult += pShape -> GetLevel();
					++ dwCount;
				}
			}
		}
	}
	if( dwCount > 0 )
	{
		fResult /= dwCount;
	}
	return fResult;
}

FLOAT CTeam::GetCurrentRegionTeamatesAverageLevel_Alive( LONG lRegionID )
{
	FLOAT fResult = 0.0f;
	DWORD dwCount = 0;
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(*it) );
		if( pPlug )
		{
			if( pPlug -> GetOwnerRegionID() == lRegionID )
			{
				CMoveShape* pShape = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
				if( pShape && pShape->IsDied() == false)
				{
					fResult += pShape -> GetLevel();
					++ dwCount;
				}
			}
		}
	}
	if( dwCount > 0 )
	{
		fResult /= dwCount;
	}
	return fResult;
}

CPlayer* CTeam::FindTeamatesInCurrentRegion( LONG lRegionID )
{
	CPlayer* pResult = NULL;
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(*it) );
		if( pPlug )
		{
			if( pPlug -> GetOwnerRegionID() == lRegionID )
			{
				pResult = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
				if( pResult )
				{
					break;
				}
			}
		}
	}
	return pResult;
}

BOOL CTeam::Serialize( vector<BYTE>* pStream )
{
	CSession::Serialize( pStream );

	//##Session����Ϣ
	_AddToByteArray( pStream, m_dwTeamID );
	_AddToByteArray( pStream, const_cast<CHAR*>( m_strTeamName.c_str() ) );
	_AddToByteArray( pStream, const_cast<CHAR*>( m_strPassword.c_str() ) );
	_AddToByteArray( pStream, m_lTeamLeaderID );

	//##Plug����Ϣ
	DWORD dwNumTeamates = GetTeamatesAmount();
	_AddToByteArray( pStream, dwNumTeamates );

	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		CPlug* pPlug = QueryPlugByID( *it );
		if( pPlug )
		{
			pPlug -> Serialize( pStream );
		}
	}

	return TRUE;
}

BOOL CTeam::Unserialize( BYTE* pStream, LONG& lOffset )
{
	BOOL bResult = TRUE;
	CSession::Unserialize( pStream, lOffset );
	if( pStream )
	{
		CHAR szMsg[256];
		m_dwTeamID		= _GetDwordFromByteArray( pStream, lOffset );
		m_strTeamName	= _GetStringFromByteArray( pStream, lOffset, szMsg );
		m_strPassword	= _GetStringFromByteArray( pStream, lOffset, szMsg );
		m_lTeamLeaderID	= _GetLongFromByteArray( pStream, lOffset );

		Start();

		//##��ȡ��Ա����Ϣ
		DWORD dwNumTeamates = _GetDwordFromByteArray( pStream, lOffset );
		for( DWORD i = 0; i < dwNumTeamates; i ++ )
		{
			LONG lPlugID = CSessionFactory::UnserializePlug( pStream, lOffset );
			if( lPlugID )
			{
				CSessionFactory::InsertPlug( GetID(), lPlugID );
			}
			else
			{
				bResult = FALSE;
				break;
			}
		}
	}
	return bResult;
}

BOOL CTeam::OnSessionStarted( BOOL bFlag )
{
	//##��Ӷ��������
	GetGame() -> AddTeam( m_dwTeamID, GetID() );

	if( bFlag == TRUE )
	{
		//##������Ϣ��World Server
		vector<BYTE> vOut;
		Serialize( &vOut );

		CMessage msg( MSG_S2W_TEAM_CREATE );
		msg.AddEx( &vOut[0], static_cast<LONG>( vOut.size() ) );
		msg.Send();
	}

	return TRUE;
}

//##����End�����Ƕӳ�ֱ�ӵ��øú�������ֹ����Ĵ��ڡ������Ҫ������Ϣ��WorldServer��
BOOL CTeam::OnSessionEnded( BOOL bFlag )
{
	//##������Ϣ��World Server
	CMessage msg( MSG_S2W_TEAM_DESTORY );
	msg.Add( GetTeamID() );
	msg.Send();

	//##������Ƴ����е�Plug
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		OnPlugEnded( *it );
	}

	GetGame() -> RemoveTeam( m_dwTeamID );
	return TRUE;
}

//##����End�������������㣬Session������Ч��ʱ����ֹ����Ĵ��ڡ��������Ҫ������Ϣ��WorldServer��
BOOL CTeam::OnSessionAborted( BOOL bFlag )
{
	//##������Ƴ����е�Plug
	list<LONG>::iterator it = GetPlugList().begin();
	for( ; it != GetPlugList().end(); it ++ )
	{
		OnPlugAborted( *it );
	}

	//##������Ϣ��World Server
	GetGame() -> RemoveTeam( m_dwTeamID );
	return TRUE;
}

BOOL CTeam::OnPlugInserted(LONG lPlugID)
{
	BOOL bResult = FALSE;

	//##����Ҫ���ú����Plug��TeamID
	CPlug* pPlug = const_cast<CPlug*>( CSessionFactory::QueryPlug(lPlugID) );
	if( pPlug )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
		if( pPlayer )
		{
			pPlayer -> SetTeamID( GetTeamID() );
		}

		//##Ȼ��������Plug֪����
		bResult = CSession::OnPlugInserted( lPlugID );
	}

	return bResult;
}

BOOL CTeam::OnPlugEnded(LONG lPlugID)
{
	BOOL bResult = FALSE;

	//##����Ҫ���ú����Plug��TeamID
	CPlug* pPlug = const_cast<CPlug*>( CSessionFactory::QueryPlug(lPlugID) );
	if( pPlug )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
		if( pPlayer )
		{
			pPlayer -> SetTeamID( 0 );
		}
		bResult = CSession::OnPlugEnded( lPlugID );
	}
	return bResult;
}

BOOL CTeam::OnPlugAborted(LONG lPlugID)
{
	BOOL bResult = FALSE;

	//##����Ҫ���ú����Plug��TeamID
	CPlug* pPlug = const_cast<CPlug*>( CSessionFactory::QueryPlug(lPlugID) );
	if( pPlug )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
		if( pPlayer )
		{
			pPlayer -> SetTeamID( 0 );
		}

		bResult = CSession::OnPlugEnded( lPlugID );
	}

	return bResult;
}


VOID CTeam::SetAllocationScheme( CTeam::ALLOCATION_SCHEME as )
{
	CPlug* pPlug = QueryPlugByOwner( TYPE_PLAYER, m_lTeamLeaderID );
	if( pPlug )
	{
		m_asAllocationScheme = as;

		OnPlugChangeState( pPlug -> GetID(), CTeamate::TS_CHANGE_ALLOCATION_SCHEME, 
			reinterpret_cast<UCHAR*>(&m_asAllocationScheme), TRUE );
	}
}

CTeam::ALLOCATION_SCHEME CTeam::GetAllocationScheme()
{
	return m_asAllocationScheme;
}

VOID CTeam::KickPlayer( LONG lPlayerID )
{
	CPlug* pPlug = QueryPlugByOwner( TYPE_PLAYER, lPlayerID );
	if( pPlug )
	{
		if( pPlug -> GetOwner() )
		{
			//##�ڵ�ǰ�������ϡ�
			pPlug -> Exit();
		}
		else
		{
			//##������Ϣ��WorldServer���߳�ĳ����ҡ�
			CMessage msg( MSG_S2W_TEAM_KICK_PLAYER );
			msg.Add( GetTeamID() );
			msg.Add( lPlayerID );
			msg.Send();
		}
	}
}

BOOL CTeam::OnPlugChangeState(LONG lPlugID, LONG lState, BYTE* pData, BOOL bSendToTrigger)
{
	BOOL bResult = CSession::OnPlugChangeState( lPlugID, lState, pData, bSendToTrigger );
	if( bResult )
	{
		//##�����Ҫ���͸�World Server�����͡�
		switch( lState )
		{
		case CTeamate::TS_CHANGE_ALLOCATION_SCHEME:
			{
				if( pData )
				{
					CPlug* pPlug = QueryPlugByID( lPlugID );
					if( pPlug )
					{
						//##������Owner�ڸ÷���������ô����WorldServer������Ϣ�����򲻷��͡�
						if( pPlug -> GetOwner() )
						{
							CTeam::ALLOCATION_SCHEME as = CTeam::AS_FORCE_DWORD;
							memcpy( &as, pData, sizeof(CTeam::ALLOCATION_SCHEME) );

							CMessage msg( MSG_S2W_TEAM_CHANGE_ALLOCATION_SCHEME );
							msg.Add( GetTeamID() );
							msg.Add( LONG(as) );
							msg.Send();
						}
					}
				}
			}
			break;
		case CTeamate::TS_CHANGE_LEADER:
			{
				if( pData )
				{
					CPlug* pPlug = QueryPlugByID( lPlugID );
					if( pPlug )
					{
						//##������Owner�ڸ÷���������ô����WorldServer������Ϣ�����򲻷��͡�
						if( pPlug -> GetOwner() )
						{
							LONG lNewLeaderID = 0;
							memcpy( &lNewLeaderID, pData, sizeof(LONG) );
							if( lNewLeaderID && QueryPlugByOwner(TYPE_PLAYER, lNewLeaderID) )
							{
								CMessage msg( MSG_S2W_TEAM_CHANGE_LEAER );
								msg.Add( GetTeamID() );
								msg.Add( lNewLeaderID );
								msg.Send();
							}
						}
					}
				}
			}
			break;
		case CTeamate::TS_CHANGE_REGION:
			{
				if( pData )
				{
					//CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
					//if( pPlug )
					//{
					//	//##������Owner�ڸ÷���������ô����WorldServer������Ϣ�����򲻷��͡�
					//	if( pPlug -> GetOwner() )
					//	{
					//		LONG lNewRegionID = 0;
					//		memcpy( &lNewRegionID, pData, sizeof(LONG) );
					//		if( lNewRegionID )
					//		{
					//			CMessage msg( MSG_S2W_TEAM_TEAMATE_CHANGE_REGION );
					//			msg.Add( GetTeamID() );
					//			msg.Add( pPlug -> GetOwnerType() );
					//			msg.Add( pPlug -> GetOwnerID() );
					//			msg.Add( lNewRegionID );
					//			msg.Send();
					//		}
					//	}
					//}
				}
			}
			break;
		case CTeamate::PS_INSERTED:
			{
				//##�ɸ�������ڵķ�������������Ϣ��WorldServer��
				CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
				if( pPlug )
				{
					CMoveShape* pShape = pPlug -> GetOwner();
					//##������Owner�ڸ÷���������ô����WorldServer������Ϣ�����򲻷��͡�
					if( pShape )
					{
						CMessage msg( MSG_S2W_TEAM_TEAMATE_JOIN );
						msg.Add( GetTeamID() );
						msg.Add( pPlug -> GetOwnerType() );
						msg.Add( pPlug -> GetOwnerID() );
						msg.Add( pPlug -> GetOwnerRegionID() );
						msg.Add( pPlug -> GetOwnerName() );
						msg.Send();
					}
				}
			}
			break;
		case CTeamate::TS_CHAT:
			{
				if( pData )
				{
					CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
					if( pPlug )
					{
						//##������Owner�ڸ÷���������ô����WorldServer������Ϣ�����򲻷��͡�
						if( pPlug -> GetOwner() )
						{
							CMessage msg( MSG_S2W_TEAM_CHAT );
							msg.Add( GetTeamID() );
							msg.Add( pPlug -> GetOwnerType() );
							msg.Add( pPlug -> GetOwnerID() );
							msg.Add( reinterpret_cast<CHAR*>(pData) );
							msg.Send();
						}
					}
				}
			}
			break;
		case CTeamate::TS_CHANGE_STATE:
			{
				if( pData )
				{
					CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
					if( pPlug )
					{
						//##������Owner�ڸ÷���������ô����WorldServer������Ϣ�����򲻷��͡�
						if( pPlug -> GetOwner() )
						{
							FLOAT fHp = 0;
							memcpy( &fHp, pData, sizeof(FLOAT) );

							CMessage msg( MSG_S2W_TEAM_CHANGE_STATE );
							msg.Add( GetTeamID() );
							msg.Add( pPlug -> GetOwnerType() );
							msg.Add( pPlug -> GetOwnerID() );
							msg.Add( fHp );
							msg.Send();
						}
					}
				}
			}
			break;
		case CTeamate::PS_REMOVED:
		case CTeamate::PS_ABORTED:
			{
				//##�ɸ�������ڵķ�������������Ϣ��WorldServer��
				CTeamate* pPlug = dynamic_cast<CTeamate*>( QueryPlugByID(lPlugID) );
				if( pPlug )
				{
					CMoveShape* pShape = pPlug -> GetOwner();
					//##������Owner�ڸ÷���������ô����WorldServer������Ϣ�����򲻷��͡�
					if( pShape )
					{
						CMessage msg( MSG_S2W_TEAM_TEAMATE_LEAVE );
						msg.Add( GetTeamID() );
						msg.Add( pPlug -> GetOwnerType() );
						msg.Add( pPlug -> GetOwnerID() );
						msg.Send();
					}
				}
			}
			break;
		}
	}
	return bResult;
}
*/