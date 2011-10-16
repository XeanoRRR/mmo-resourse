#include "StdAfx.h"
#include "CityGuardWithBow.h"
#include "..\Player.h"
#include "..\Monster.h"



CCityGuardWithBow::CCityGuardWithBow()
{
}

CCityGuardWithBow::~CCityGuardWithBow()
{
}
/*
CBaseAI::AI_EXEC_STATE CCityGuardWithBow::Run()
{
	return CMonsterAI::Run();
}

VOID CCityGuardWithBow::OnSchedule()
{
	CBaseAI::OnSchedule();

	//##�ж��Ƿ�ʼִ�е�����
	if( m_pOwner == NULL || m_pOwner -> IsDied() ) return;

	if( HasTarget() == TRUE &&
		m_qActiveAction.size() == 0 &&
		m_qPassiveAction.size() == 0 )
	{
		if( m_pOwner -> IsFightable() == FALSE )
		{
			OnLoseTarget();
			return;
		}

		CMoveShape* pShape = GetTarget();
		if( pShape == NULL || pShape -> IsDied() )
		{
			OnLoseTarget();
			return;
		}

		//##��һ�ν���ʱ������ѡ���ܣ�֮��ÿ��Attack�¼����ʱ��
		//##�������һ��ChangeSkill�¼��������ѡȡ��ǰ�ļ��ܡ�
		CSkill* pSkill = m_pOwner -> GetCurrentSkill();
		if( pSkill == NULL ) 
		{
			OnChangeSkill();
			pSkill = m_pOwner -> GetCurrentSkill();
			if( pSkill == NULL )
			{
				OnLoseTarget();
				return;
			}
		}

		//##�жϵ����Ƿ��Ѿ����빥����Χ.
		//##�Ϻ�����Ҫ��
		LONG lDistance = m_pOwner -> RealDistance( pShape );
		if( lDistance >= pSkill -> GetAffectRangeMin() &&
			lDistance <= pSkill -> GetAffectRangeMax() )
		{
			if( pSkill -> Begin(m_pOwner, pShape) )
			{
				//##��������ɹ��ˣ������һ�������¼���
				AddAIEvent( ASA_ATTACK );
			}
			else
			{
				//##��ΪĳЩ��������¹���ʧ�ܡ����п����Ǿ��벻���ȡ�
				//##���һ�������¼������Ҫע����ǣ����ܹ���������ھ��䷶Χ��
				//##����ʹ�ü���ʧ�ܡ�
				OnLoseTarget();
				AddAIEvent( ASA_SEARCH_ENEMY );
			}
		}
		else
		{
			OnLoseTarget();
			AddAIEvent( ASA_SEARCH_ENEMY );
		}
	}
}


VOID CCityGuardWithBow::OnIdle()
{
	//##�����ǰ��Area��û����ҡ�
	if( m_pOwner -> GetArea() -> GetAllPlayers().size() == 0 )
	{
		CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
		LONG lX = m_pOwner -> GetArea() -> GetX();
		LONG lY = m_pOwner -> GetArea() -> GetY();

		BOOL bNoPlayerAround = TRUE;
		for( LONG i = 0; i < 9; i ++ )
		{
			CArea* pArea = pRegion -> GetArea( lX + _area[i][0], lY + _area[i][1] );
			if( pArea )
			{
				if( pArea -> GetAllPlayers().size() > 0 )
				{
					bNoPlayerAround = FALSE;
					break;
				}
			}
		}
		//##���û����ң���������״̬��
		if( bNoPlayerAround )
		{
			Hibernate();
			return;
		}
	}	

	CMonster* pOwner = dynamic_cast<CMonster*>( GetOwner() );
	if( pOwner && pOwner -> IsDied() == FALSE )
	{
		//##�������һ��ѡ���ܵ��¼�
		AddAIEvent( ASA_CHANGE_SKILL );
		//##���һ��վ��״̬
//		AddAIEvent( ASA_STAND, pOwner -> GetBaseProperty() -> dwAIInterval );
		//##֮��,���һ�����е�״̬
		AddAIEvent( ASA_SEARCH_ENEMY );
	}
}

VOID CCityGuardWithBow::WhenBeenHurted( LONG lType, LONG lID, DWORD dwStiffenTime )
{
	CBaseAI::WhenBeenHurted( lType, lID, dwStiffenTime );
	if( HasTarget() == FALSE )
	{
		CMoveShape* pTarget = SearchEnemyGuildMember();
		CMoveShape* pPet	= SearchEnemyGuildPet();
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
			SetTarget( pTarget -> GetType(), pTarget -> GetID() );
		}
	}
}

//##��ÿ����Ҫ�ƶ���ʱ����Ѱ�����Player
BOOL CCityGuardWithBow::OnMoving()
{
	if( m_pOwner == NULL ) return TRUE;

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

BOOL CCityGuardWithBow::OnFighting()
{
	if( m_pOwner == NULL ) return TRUE;

	if( CMonsterAI::OnFighting() )
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

CMoveShape* CCityGuardWithBow::SearchEnemyGuildMember()
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

		//##�����Ӵ��룬���Player���������/���˵ģ���������
		if( pPlayer->GetFactionID() && pPlayer->GetFactionID() == pRegion->GetOwnedCityFaction() )
		{
			continue;
		}
		else
		{
			if( pPlayer->GetUnionID() && pPlayer->GetUnionID() == pRegion->GetOwnedCityUnion() )
			{
				continue;
			}
		}

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

CMoveShape* CCityGuardWithBow::SearchEnemyGuildPet()
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

		//##�����Ӵ��룬����������������/���˵ģ���������
		if( pMonster -> GetMasterInfo().lMasterType == TYPE_PLAYER )
		{
			CPlayer* pMaster = GetGame() -> FindPlayer( pMonster -> GetMasterInfo().lMasterID );
			if( pMaster )
			{
				//##�����Ӵ��룬����������������/���˵ģ���������
				if( pMaster->GetFactionID() && pMaster->GetFactionID() == pRegion->GetOwnedCityFaction() )
				{
					continue;
				}
				else
				{
					if( pMaster->GetUnionID() && pMaster->GetUnionID() == pRegion->GetOwnedCityUnion() )
					{
						continue;
					}
				}
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
CMoveShape* CCityGuardWithBow::SearchEnemyGuildCarriage()
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

		//##�����Ӵ��룬����������������/���˵ģ���������
		if( pMonster -> GetMasterInfo().lMasterType == TYPE_PLAYER )
		{
			CPlayer* pMaster = GetGame() -> FindPlayer( pMonster -> GetMasterInfo().lMasterID );
			if( pMaster )
			{
				//##�����Ӵ��룬����������������/���˵ģ���������
				if( pMaster->GetFactionID() && pMaster->GetFactionID() == pRegion->GetOwnedCityFaction() )
				{
					continue;
				}
				else
				{
					if( pMaster->GetUnionID() && pMaster->GetUnionID() == pRegion->GetOwnedCityUnion() )
					{
						continue;
					}
				}
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
BOOL CCityGuardWithBow::OnSearchEnemy()
{
	if( CMonsterAI::OnSearchEnemy() )
	{
		//##��������߲�����.ֱ�ӷ���.
		if( m_pOwner == NULL ) return TRUE;

		//##���������ǰĿ��
		OnLoseTarget();

		//##ʱ���ж�.��ʱ����ʱ���ж�.
		CMoveShape* pTarget = SearchEnemyGuildMember();
		CMoveShape* pPet	= SearchEnemyGuildPet();
		///CMoveShape* pCar	= SearchEnemyGuildCarriage();
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
			//else if( pCar )
			//{
			//	if( m_pOwner -> RealDistance(pCar) < m_pOwner -> RealDistance(pTarget) )
			//	{
			//		pTarget = pCar;
			//	}
			//}
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
*/