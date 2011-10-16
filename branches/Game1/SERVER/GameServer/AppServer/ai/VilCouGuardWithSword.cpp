#include "StdAfx.h"
#include ".\vilcouguardwithsword.h"
#include "..\Player.h"
#include "..\Monster.h"


CVilCouGuardWithSword::CVilCouGuardWithSword(void)
{
}

CVilCouGuardWithSword::~CVilCouGuardWithSword(void)
{
}
/*
CBaseAI::AI_EXEC_STATE CVilCouGuardWithSword::Run()
{
	return CMonsterAI::Run();
}

VOID CVilCouGuardWithSword::OnSchedule()
{
	CMonsterAI::OnSchedule();
}

VOID CVilCouGuardWithSword::OnIdle()
{
	//##��һ�ν���AI�����ȱ���λ�����꣬Ȼ�����һ���ı䵱ǰ���ܵ��¼��Գ�ʼ�����ܡ�
	if( m_pOwner && m_lX == -1 && m_lY == -1 )
	{
		m_lX = m_pOwner -> GetTileX();
		m_lY = m_pOwner -> GetTileY();

		AddAIEvent( ASA_CHANGE_SKILL );
	}
	CMonsterAI::OnIdle();
}

BOOL CVilCouGuardWithSword::OnMoving()
{
	if( CMonsterAI::OnMoving() )
	{
		AddAIEvent( ASA_SEARCH_ENEMY );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID CVilCouGuardWithSword::WhenBeenHurted( LONG lType, LONG lID, DWORD dwStiffenTime )
{
	CBaseAI::WhenBeenHurted( lType, lID, dwStiffenTime );
	if( HasTarget() == FALSE )
	{
		SetTarget( lType, lID );
	}
}

CMoveShape* CVilCouGuardWithSword::SearchEnemyMember()
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return NULL;
	
	//##�����ǰ�������ܲ�����.
	CSkill* pSkill = m_pOwner -> GetCurrentSkill();
	if( pSkill == NULL ) return NULL;

	vector<CShape*> vOut;
	pRegion -> FindAroundObject( m_pOwner, TYPE_PLAYER, vOut );

	CPlayer* pTarget = NULL;
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>( vOut[i] );
		if( pPlayer == NULL ) 
		{
			continue;
		}

		//##�����Ӵ���,���Player�;ݵ����а������ͬһ�����������
		if(pRegion->GetCountry() != 0 && pPlayer->GetCountry() == pRegion->GetCountry())
			continue;

		//##��������Ҳ�ھ��䷶Χ��
		if( m_pOwner -> RealDistance(pPlayer) <= 
			m_pOwner -> GetAlertRange() )
		{
			//##�����ǰĿ�겻���ڣ����Ȱ����Ŀ��������
			if( pTarget == NULL )
			{
				pTarget = pPlayer;
				continue;
			}

			//##�������ұ�ѡ���Ŀ��ľ���Ҫ�̡�
			if( m_pOwner -> RealDistance(pPlayer) < 
				m_pOwner -> RealDistance(pTarget) )
			{
				//##�������ҵľ���ȼ��ܵ���С��������Ҫ��
				if( m_pOwner -> RealDistance(pPlayer) >=
					pSkill -> GetAffectRangeMin() )
				{
					pTarget = pPlayer;
				}
			}
			//##�������ұ�ѡ��Ŀ��ľ���ҪԶ.
			else
			{
				//##���ѡ�е�Ŀ���ڼ��ܵ���С���÷�Χ��.
				if( m_pOwner -> RealDistance(pTarget) <
					pSkill -> GetAffectRangeMin() )
				{
					pTarget = pPlayer;
				}
			}
		}
	}
	return pTarget;
}

CMoveShape* CVilCouGuardWithSword::SearchEnemyPet()
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return NULL;
	
	//##�����ǰ�������ܲ�����.
	CSkill* pSkill = m_pOwner -> GetCurrentSkill();
	if( pSkill == NULL ) return NULL;

	vector<CShape*> vOut;
	pRegion -> FindAroundObject( m_pOwner, TYPE_PET, vOut );

	CMonster* pTarget = NULL;
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CMonster* pMonster = dynamic_cast<CMonster*>( vOut[i] );
		if( pMonster == NULL ) 
		{
			continue;
		}

		if( pMonster -> GetMasterInfo().lMasterType == TYPE_PLAYER )
		{
			CPlayer* pMaster = GetGame() -> FindPlayer( pMonster -> GetMasterInfo().lMasterID );
			if( pMaster )
			{
				//##�����Ӵ���,���Player�;ݵ����а������ͬһ�����������
				if(pRegion->GetCountry() != 0 && pMaster->GetCountry() == pRegion->GetCountry())
					continue;
			}
		}

		//##����ö���Ҳ�ھ��䷶Χ��
		if( m_pOwner -> RealDistance(pMonster) <= 
			m_pOwner -> GetAlertRange() )
		{
			//##�����ǰĿ�겻���ڣ����Ȱ����Ŀ��������
			if( pTarget == NULL )
			{
				pTarget = pMonster;
				continue;
			}

			//##�������ұ�ѡ���Ŀ��ľ���Ҫ�̡�
			if( m_pOwner -> RealDistance(pMonster) < 
				m_pOwner -> RealDistance(pTarget) )
			{
				//##�������ҵľ���ȼ��ܵ���С��������Ҫ��
				if( m_pOwner -> RealDistance(pMonster) >=
					pSkill -> GetAffectRangeMin() )
				{
					pTarget = pMonster;
				}
			}
			//##�������ұ�ѡ��Ŀ��ľ���ҪԶ.
			else
			{
				//##���ѡ�е�Ŀ���ڼ��ܵ���С���÷�Χ��.
				if( m_pOwner -> RealDistance(pTarget) <
					pSkill -> GetAffectRangeMin() )
				{
					pTarget = pMonster;
				}
			}
		}
	}
	return pTarget;
}


CMoveShape* CVilCouGuardWithSword::SearchEnemyCarriage()
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return NULL;
	
	//##�����ǰ�������ܲ�����.
	CSkill* pSkill = m_pOwner -> GetCurrentSkill();
	if( pSkill == NULL ) return NULL;

	vector<CShape*> vOut;
	pRegion -> FindAroundObject( m_pOwner, TYPE_CARRIAGE, vOut );

	CMonster* pTarget = NULL;
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CMonster* pMonster = dynamic_cast<CMonster*>( vOut[i] );
		if( pMonster == NULL ) 
		{
			continue;
		}

		if( pMonster -> GetMasterInfo().lMasterType == TYPE_PLAYER )
		{
			CPlayer* pMaster = GetGame() -> FindPlayer( pMonster -> GetMasterInfo().lMasterID );
			if( pMaster )
			{
				//##�����Ӵ���,���Player�;ݵ����а������ͬһ�����������
				if(pRegion->GetCountry() != 0 && pMaster->GetCountry() == pRegion->GetCountry())
					continue;
			}
		}

		//##����ö���Ҳ�ھ��䷶Χ��
		if( m_pOwner -> RealDistance(pMonster) <= 
			m_pOwner -> GetAlertRange() )
		{
			//##�����ǰĿ�겻���ڣ����Ȱ����Ŀ��������
			if( pTarget == NULL )
			{
				pTarget = pMonster;
				continue;
			}

			//##�������ұ�ѡ���Ŀ��ľ���Ҫ�̡�
			if( m_pOwner -> RealDistance(pMonster) < 
				m_pOwner -> RealDistance(pTarget) )
			{
				//##�������ҵľ���ȼ��ܵ���С��������Ҫ��
				if( m_pOwner -> RealDistance(pMonster) >=
					pSkill -> GetAffectRangeMin() )
				{
					pTarget = pMonster;
				}
			}
			//##�������ұ�ѡ��Ŀ��ľ���ҪԶ.
			else
			{
				//##���ѡ�е�Ŀ���ڼ��ܵ���С���÷�Χ��.
				if( m_pOwner -> RealDistance(pTarget) <
					pSkill -> GetAffectRangeMin() )
				{
					pTarget = pMonster;
				}
			}
		}
	}
	return pTarget;
}
BOOL CVilCouGuardWithSword::OnSearchEnemy()
{
	if( CMonsterAI::OnSearchEnemy() )
	{
		//##��������߲�����.ֱ�ӷ���.
		if( m_pOwner == NULL ) 
		{
			return TRUE;
		}
		if( HasTarget() ) 
		{
			//##������벻���Լ���ԭ��λ��...
			if( m_lX != -1 && m_lY != -1 && m_pOwner -> 
				RealDistance( m_lX, m_lY ) > m_pOwner -> GetTrackRange() )
			{
				OnLoseTarget();
			}
			return TRUE;
		}

		CMoveShape* pTarget = SearchEnemyMember();
		CMoveShape* pPet	= SearchEnemyPet();
		
		if( pTarget == NULL )
		{
			pTarget = pPet;
		}
		else
		{
			if( pPet )
			{
				if( m_pOwner -> RealDistance(pPet) < m_pOwner -> RealDistance(pTarget) )
				{
					pTarget = pPet;
				}
			}
		}

		if( pTarget )
		{
			//##�����������ö���			
			SetTarget( pTarget -> GetType(), pTarget -> GetID() );
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CVilCouGuardWithSword::OnLoseTarget()
{
	if( CMonsterAI::OnLoseTarget() )
	{
		if( m_pOwner && m_lX != -1 && m_lY != -1 )
		{
			CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
			if( pRegion )
			{
				if( pRegion -> GetBlock(m_lX, m_lY) == CRegion::BLOCK_NO )
				{
					m_pOwner -> OnSetPosition( m_lX, m_lY );
				}
				else
				{
					LONG lX, lY;
					pRegion -> GetRandomPosInRange( lX, lY, m_lX - 1, m_lY - 1, 3, 3 );
					m_pOwner -> OnSetPosition( lX, lY );
				}
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
*/