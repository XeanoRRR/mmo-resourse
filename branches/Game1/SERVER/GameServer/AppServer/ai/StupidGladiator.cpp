#include "StdAfx.h"
#include ".\StupidGladiator.h"
#include "..\Player.h"
#include "..\Monster.h"

CStupidGladiator::CStupidGladiator()
{
}

CStupidGladiator::~CStupidGladiator()
{
}

BOOL CStupidGladiator::SearchEnemy()
{
	if (!HasTarget())
	{
		CMonsterAI::SearchEnemy();
	}
	return TRUE;
}

BOOL CStupidGladiator::WhenBeenHurted(LONG lType,const CGUID& guid, DWORD hurt)
{
	CMonsterAI::WhenBeenHurted(lType,guid,hurt);
	return TRUE;
}
/*
CBaseAI::AI_EXEC_STATE CStupidGladiator::Run()
{
	return CMonsterAI::Run();
}

VOID CStupidGladiator::OnSchedule()
{
	CMonsterAI::OnSchedule();
}

//##��ÿ����Ҫ�ƶ���ʱ����Ѱ�����Player
BOOL CStupidGladiator::OnMoving()
{
	if( CMonsterAI::OnMoving() )
	{
		if( m_pOwner -> IsDied() == FALSE )
		{
			AddAIEvent( ASA_SEARCH_ENEMY );
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CStupidGladiator::OnSearchEnemy()
{
	if( CMonsterAI::OnSearchEnemy() )
	{
		//##��������߲�����.ֱ�ӷ���.
		if( m_pOwner == NULL ) return TRUE;
		//##�����Ŀ����,��ֱ�ӷ���.
		if( HasTarget() ) return TRUE;

		CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
		if( pRegion == NULL ) return TRUE;
		
		vector<CPlayer*> vOut;
		pRegion -> FindAroundPlayer( m_pOwner, vOut );

		vector<CMonster*> vPets;
		pRegion -> FindAroundPets( m_pOwner, vPets );

		//vector<CMonster*> vCars;
		//pRegion->FindAroundCarriages( m_pOwner, vCars );


		CMoveShape* pTarget = NULL;
		LONG lNearestDistance = 0xFFFFFFFF;
		for( size_t i = 0; i < vOut.size(); i ++ )
		{
			CPlayer* pPlayer = vOut[i];
			if( pPlayer == NULL ) continue;
			if( pPlayer -> IsDied() ) continue;
			LONG lDistance = m_pOwner -> RealDistance( pPlayer );
			if( lDistance <= m_pOwner -> GetAlertRange() )
			{
				if( pTarget == NULL )
				{
					pTarget = pPlayer;
					lNearestDistance = lDistance;
				}
				else 
				{
					if( lDistance <= lNearestDistance )
					{
						pTarget = pPlayer;
						lNearestDistance = lDistance;
					}
				}
			}
		}

		for( size_t i = 0; i < vPets.size(); i ++ )
		{
			CMonster* pPet = vPets[i];
			if( pPet == NULL ) continue;
			if( pPet -> IsDied() ) continue;
			LONG lDistance = m_pOwner -> RealDistance( pPet );
			if( lDistance <= m_pOwner -> GetAlertRange() )
			{
				if( pTarget == NULL )
				{
					pTarget = pPet;
					lNearestDistance = lDistance;
				}
				else 
				{
					if( lDistance <= lNearestDistance )
					{
						pTarget = pPet;
						lNearestDistance = lDistance;
					}
				}
			}
		}

		//for( size_t i = 0; i < vCars.size(); i ++ )
		//{
		//	CMonster* pCar = vCars[i];
		//	if( pCar == NULL ) continue;
		//	if( pCar -> IsDied() ) continue;

		//	LONG lDistance = m_pOwner -> RealDistance( pCar );
		//	if( lDistance <= m_pOwner -> GetAlertRange() )
		//	{
		//		if( pTarget == NULL )
		//		{
		//			pTarget = pCar;
		//			lNearestDistance = lDistance;
		//		}
		//		else 
		//		{
		//			if( lDistance <= lNearestDistance )
		//			{
		//				pTarget = pCar;
		//				lNearestDistance = lDistance;
		//			}
		//		}
		//	}
		//}
		if( pTarget )
		{
			//##��������������ö���			
			SetTarget( pTarget -> GetType(), pTarget -> GetID() );
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

*/