#include "StdAfx.h"
#include "GuardCountry.h"
#include "..\Player.h"
#include "..\Monster.h"



CGuardCountry::CGuardCountry()
{
}

CGuardCountry::~CGuardCountry()
{
}
/*
CBaseAI::AI_EXEC_STATE CGuardCountry::Run()
{
	return CMonsterAI::Run();
}

VOID CGuardCountry::OnSchedule()
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


VOID CGuardCountry::OnIdle()
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

VOID CGuardCountry::WhenBeenHurted( LONG lType, LONG lID, DWORD dwStiffenTime )
{
	CBaseAI::WhenBeenHurted( lType, lID, dwStiffenTime );
	if( HasTarget() == FALSE )
	{
		CMoveShape* pTarget = SearchOtherCountry();

		if( pTarget == NULL )
		{
			pTarget = SearchCriminal();
		}

		if( pTarget == NULL )
		{
			pTarget = SearchMonster();
		}

		if( pTarget )
		{
			SetTarget( pTarget -> GetType(), pTarget -> GetID() );
		}
	}
}

//##��ÿ����Ҫ�ƶ���ʱ����Ѱ�����Player
BOOL CGuardCountry::OnMoving()
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

BOOL CGuardCountry::OnFighting()
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


CMoveShape* CGuardCountry::SearchOtherCountry()
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return NULL;
	

	CMonster *pOwner =  dynamic_cast<CMonster*>(m_pOwner);
	if( !pOwner ) return NULL;

	//##�����ǰ�������ܲ�����.
	CSkill* pSkill = pOwner -> GetCurrentSkill();
	if( pSkill == NULL ) return NULL;

	vector<CShape*> vOut;
	pRegion -> FindAroundObject( pOwner, TYPE_PLAYER, vOut );

	CPlayer* pTarget = NULL;
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>( vOut[i] );
		if( pPlayer == NULL )
		{
			continue;
		}

		// ����������û���ã�ֱ�ӷ���
//		if( !pOwner->GetBaseProperty()->dwRace ) continue;
				if( !pOwner->GetCountry()) continue;

		if( pPlayer -> GetCountry() == pOwner->GetCountry())
		{
			continue;
		}

		//##��������Ҳ�ھ��䷶Χ��
		if( pOwner -> RealDistance(pPlayer) <= pOwner -> GetAlertRange() )
		{
			//##�����ǰĿ�겻���ڣ����Ȱ����Ŀ��������
			if( pTarget == NULL )
			{
				pTarget = pPlayer;
				continue;
			}

			//##�������ұ�ѡ���Ŀ��ľ���Ҫ�̡�
			// ����
			//##�������ҵľ���ȼ��ܵ���С��������Ҫ��
			if( pOwner -> RealDistance(pPlayer) < pOwner -> RealDistance(pTarget) &&
				 pOwner -> RealDistance(pPlayer) >= pSkill -> GetAffectRangeMin() )
			{
				pTarget = pPlayer;
			}
		}
	}
	return pTarget;
}


CMoveShape* CGuardCountry::SearchCriminal()
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
		if( pPlayer -> IsBadman() == FALSE ||
			pPlayer -> IsAttackAble(m_pOwner) == false )
		{
			continue;
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

CMoveShape* CGuardCountry::SearchMonster()
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return NULL;
	
	//##�����ǰ�������ܲ�����.
	CSkill* pSkill = m_pOwner -> GetCurrentSkill();
	if( pSkill == NULL ) return NULL;

	vector<CShape*> vOut;
	pRegion -> FindAroundObject( m_pOwner, TYPE_MONSTER, vOut );

	CMonster* pTarget = NULL;
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CMonster* pMonster = dynamic_cast<CMonster*>( vOut[i] );
		if( pMonster == NULL ) 
		{
			continue;
		}
		if( pMonster -> GetMonsterKind() == 
			CMonsterList::MK_GUARD ||
			pMonster -> IsAttackAble(m_pOwner) == false ) 
		{
			continue;
		}

		//##��������Ҳ�ھ��䷶Χ��
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

BOOL CGuardCountry::OnSearchEnemy()
{
	if( CMonsterAI::OnSearchEnemy() )
	{
		//##��������߲�����.ֱ�ӷ���.
		if( m_pOwner == NULL ) return TRUE;

		//##���������ǰĿ��
		OnLoseTarget();

		//##ʱ���ж�.��ʱ����ʱ���ж�.

		CMoveShape* pTarget = SearchOtherCountry();

		if( pTarget == NULL )
		{
			pTarget = SearchCriminal();
		}

		if( pTarget == NULL )
		{
			pTarget = SearchMonster();
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