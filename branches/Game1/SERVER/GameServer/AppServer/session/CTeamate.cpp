
#include "stdafx.h"
/*
#include "CTeamate.h"
#include "CSession.h"
#include "CSessionFactory.h"
#include "CTeam.h"
#include <MMSystem.h>
#include "AppServer\Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTeamate::CTeamate()
{
	m_lOwnerRegionID		= 0;
	m_dwPlayerLoseTimeStamp	= 0;
}

CTeamate::~CTeamate()
{
#ifdef _DEBUG
	//Debug
	CHAR szMsg[256];
	sprintf( szMsg, "plug %s deleted.\n", GetOwnerName() );
	OutputDebugString( szMsg );
	//Debug End
#endif
}

BOOL CTeamate::IsPlugAvailable()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = TRUE;
	CPlayer* pOwner = dynamic_cast<CPlayer*>( GetOwner() );
	if( pOwner == NULL )
	{
		if( m_dwPlayerLoseTimeStamp )
		{
			//##����5���Ӻ�,��ʧЧ.
			if( timeGetTime() >= m_dwPlayerLoseTimeStamp + 300000 )
			{
				bResult = FALSE;
			}
		}
		else
		{
			if( GetGame() -> FindRegion(m_lOwnerRegionID) )
			{
				//##���Region����������������ϣ���ô��¼ʱ���
				m_dwPlayerLoseTimeStamp = timeGetTime();
			}
			else
			{
				m_dwPlayerLoseTimeStamp = 0;
			}
		}
	}
	else
	{
		if( m_dwPlayerLoseTimeStamp )
		{
			if( pOwner -> GetTeamID() == 0 )
			{
				CTeam* pTeam = dynamic_cast<CTeam*>( GetSession() );
				if( pTeam )
				{
					pOwner -> SetTeamID( pTeam -> GetTeamID() );

					//##���л�����,�����͸��ͻ���
					vector<BYTE> vOut;
					pTeam -> Serialize( &vOut );

					CMessage msg( MSG_S2C_TEAM_SYNCHRONIZE );
					msg.AddEx( &vOut[0], static_cast<LONG>( vOut.size() ) );
					msg.SendToPlayer( GetOwnerID() );
				}
			}
			m_dwPlayerLoseTimeStamp = 0;
		}
	}
	return bResult;
}

//##ModelId=41E0DC5C005D
BOOL CTeamate::OnSessionEnded()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return OnPlugEnded();
	//return TRUE;
}

//##ModelId=41E0DC64032C
BOOL CTeamate::OnSessionAborted()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return OnPlugEnded();
	//return TRUE;
}

VOID CTeamate::SetOwnerRegionID( LONG lID )
{
	m_lOwnerRegionID = lID;
	ChangeState( CTeamate::TS_CHANGE_REGION, reinterpret_cast<UCHAR*>(&m_lOwnerRegionID) );
}

LONG CTeamate::GetOwnerRegionID()
{
	return m_lOwnerRegionID;
}

VOID CTeamate::SetOwnerName( CHAR* szName )
{
	if( szName )
	{
		m_strOwnerName = szName;
	}
}

CHAR* CTeamate::GetOwnerName()
{
	return const_cast<CHAR*>( m_strOwnerName.c_str() );
}

BOOL CTeamate::Serialize( vector<BYTE>* pStream )
{
	BOOL bResult = FALSE;
	if( pStream )
	{
		if( CPlug::Serialize(pStream) )
		{
			_AddToByteArray( pStream, m_lOwnerRegionID );
			_AddToByteArray( pStream, const_cast<CHAR*>( m_strOwnerName.c_str() ) );
			bResult = TRUE;
		}
	}
	return bResult;
}

BOOL CTeamate::Unserialize( BYTE* pStream, LONG& lOffset )
{
	BOOL bResult = FALSE;
	if( pStream )
	{
		if( CPlug::Unserialize(pStream, lOffset) )
		{
			CHAR szTemp[256];
			m_lOwnerRegionID = _GetLongFromByteArray( pStream, lOffset );
			m_strOwnerName	 = _GetStringFromByteArray( pStream, lOffset, szTemp );
			bResult = TRUE;
		}
	}
	return bResult;
}

//##�����Plug���뵽���Session�е�ʱ��
BOOL CTeamate::OnChangeState(LONG lPlugID, LONG lStateID, BYTE* pData)
{
	CTeam* pSession = dynamic_cast<CTeam*>( GetSession() );
	if( pSession )
	{
		CTeamate* pPlug = dynamic_cast<CTeamate*>( pSession -> QueryPlugByID(lPlugID) );
		if( pPlug )
		{
			switch( lStateID )
			{
			case CTeamate::TS_CHANGE_ALLOCATION_SCHEME:
				{
					if( pData )
					{
						CTeam::ALLOCATION_SCHEME as = CTeam::AS_FORCE_DWORD;
						memcpy( &as, pData, sizeof(CTeam::ALLOCATION_SCHEME) );

						CMessage msg( MSG_S2C_TEAM_CHANGE_ALLOCATION_SCHEME );
						msg.Add( pSession -> GetTeamID() );
						msg.Add( LONG(as) );
						msg.SendToPlayer( GetOwnerID() );
					}
				}
				break;
			case CTeamate::TS_CHANGE_LEADER:
				{
					if( pData )
					{
						LONG lNewLeaderID = 0;
						memcpy( &lNewLeaderID, pData, sizeof(LONG) );
						if( lNewLeaderID && pSession -> QueryPlugByOwner(TYPE_PLAYER, lNewLeaderID) )
						{
							CMessage msg( MSG_S2C_TEAM_CHANGE_LEAER );
							msg.Add( pSession -> GetTeamID() );
							msg.Add( lNewLeaderID );
							msg.SendToPlayer( GetOwnerID() );
						}
					}
				}
				break;
			case CTeamate::TS_CHANGE_REGION:
				{
					if( pData )
					{
						LONG lNewRegionID = 0;
						memcpy( &lNewRegionID, pData, sizeof(LONG) );
						if( lNewRegionID )
						{
							CMessage msg( MSG_S2C_TEAM_TEAMATE_CHANGE_REGION );
							msg.Add( pSession -> GetTeamID() );
							msg.Add( pPlug -> GetOwnerType() );
							msg.Add( pPlug -> GetOwnerID() );
							msg.Add( lNewRegionID );
							msg.SendToPlayer( GetOwnerID() );
						}
					}
				}
				break;
			case CTeamate::TS_CHANGE_STATE:
				{
					if( pData )
					{
						FLOAT fHp = 0.0f;
						memcpy( &fHp, pData, sizeof(FLOAT) );

						CMessage msg( MSG_S2C_TEAM_CHANGE_STATE );
						msg.Add( pSession -> GetTeamID() );
						msg.Add( pPlug -> GetOwnerType() );
						msg.Add( pPlug -> GetOwnerID() );
						msg.Add( fHp );
						msg.SendToPlayer( GetOwnerID() );
					}
				}
				break;
			case CTeamate::TS_CHAT:
				{
					if( pData )
					{
						CMessage msg( MSG_S2C_TEAM_CHAT );
						msg.Add( pSession -> GetTeamID() );
						msg.Add( pPlug -> GetOwnerType() );
						msg.Add( pPlug -> GetOwnerID() );
						msg.Add( reinterpret_cast<CHAR*>(pData) );
						msg.SendToPlayer( GetOwnerID() );
					}
				}
				break;
			case CTeamate::PS_INSERTED:
				{
#ifdef _DEBUG
					//Debug
					CHAR szMsg[256];
					sprintf( szMsg, "Send to %s, %s Join this team.\n", GetOwnerName(), pPlug -> GetOwnerName() );
					OutputDebugString( szMsg );
					//Debug End
#endif

					FLOAT fHpPercent = 1.0f;
					CMoveShape* pShape = pPlug -> GetOwner();
					if( pShape )
					{
						fHpPercent = static_cast<FLOAT>( pShape -> GetHP() ) / static_cast<FLOAT>( pShape -> GetMaxHP() );
					}
					
					CMessage msg( MSG_S2C_TEAM_TEAMATE_JOIN );
					msg.Add( pSession -> GetTeamID() );
					msg.Add( pPlug -> GetOwnerType() );
					msg.Add( pPlug -> GetOwnerID() );
					msg.Add( pPlug -> GetOwnerRegionID() );
					msg.Add( fHpPercent );
					msg.Add( pPlug -> GetOwnerName() );
					msg.SendToPlayer( GetOwnerID() );
				}
				break;
			case CTeamate::PS_REMOVED:
			case CTeamate::PS_ABORTED:
				{
					CMessage msg( MSG_S2C_TEAM_TEAMATE_LEAVE );
					msg.Add( pSession -> GetTeamID() );
					msg.Add( pPlug -> GetOwnerType() );
					msg.Add( pPlug -> GetOwnerID() );
					msg.SendToPlayer( GetOwnerID() );
				}
				break;
			}
		}
	}
	return TRUE;
}

//##�����Plug���뵽��Session�е�ʱ��
BOOL CTeamate::OnPlugInserted()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>( GetOwner() );
	if( pPlayer )
	{
		//##����������ɵ�һ�У�������������ͻ������ڸ÷�������ʱ�򡣷���û�����塣
		SetOwnerName( const_cast<CHAR*>( pPlayer -> GetName() ) );
		SetOwnerRegionID( pPlayer -> GetRegionID() );
#ifdef _DEBUG
		//Debug
		CHAR szMsg[256];
		sprintf( szMsg, "%s Join team.\n", GetOwnerName() );
		OutputDebugString( szMsg );
		//Debug End
#endif
		//##���Plug���뵽�������ˣ�֪ͨ�ͻ��ˡ���һ�μ����ʱ�򣬱�������е����ݶ����͹�ȥ��
		CTeam* pTeam = dynamic_cast<CTeam*>( GetSession() );
		if( pTeam )
		{
			vector<BYTE> vOut;
			pTeam -> Serialize( &vOut );

			CMessage msg( MSG_S2C_TEAM_SYNCHRONIZE );
			msg.AddEx( &vOut[0], static_cast<LONG>( vOut.size() ) );
			msg.SendToPlayer( GetOwnerID() );
		}
	}
	return TRUE;
}

//##�����Plug�뿪��Session�е�ʱ��
BOOL CTeamate::OnPlugAborted()
{
	return OnPlugEnded();
}

//##�����Plug�뿪��Session�е�ʱ��
BOOL CTeamate::OnPlugEnded()
{
#ifdef _DEBUG
	//Debug
	CHAR szMsg[256];
	sprintf( szMsg, "%s leave team.\n", GetOwnerName() );
	OutputDebugString( szMsg );
	//Debug End
#endif

	CPlayer* pOwner = dynamic_cast<CPlayer*>( GetOwner() );
	if( pOwner )
	{
		pOwner -> SetTeamID( 0 );

		CTeam* pTeam = dynamic_cast<CTeam*>( GetSession() );
		if( pTeam )
		{
			CMessage msg( MSG_S2C_TEAM_TEAMATE_LEAVE );
			msg.Add( pTeam -> GetTeamID() );
			msg.Add( GetOwnerType() );
			msg.Add( GetOwnerID() );
			msg.SendToPlayer( GetOwnerID() );
		}
	}
	return TRUE;
}
*/