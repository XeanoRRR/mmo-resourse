#include "StdAfx.h"
#include "FixedPositionArcher.h"
#include "..\Player.h"
#include "..\Monster.h"
#include "..\ServerRegion.h"
#include "..\BuffSkill\BuffSkillDef.h"
#include "..\BuffSkill\BaseModule.h"



CFixedPositionArcher::CFixedPositionArcher()
{
}

CFixedPositionArcher::~CFixedPositionArcher()
{
}

INT	CFixedPositionArcher::Tracing(CMoveShape*pTarget)
{
	SearchSkill(pTarget);

	CBaseModule* basemodule=0;
	stStaticModuParam* moduparam=0;
	BOOL  HasSkill=TRUE;
	INT MinAtkDistance=0;
	INT MaxAtkDistance=0;

	if (HasChoiceSkill())
	{
		basemodule=dynamic_cast<CMonster*>(m_pOwner)->GetActiveSkill(0);//GetSearchedSkillKey()
	}
	else
	{
		Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());
		WhenLoseTarget();
		return TRACE_RESULT_TRACE;
	}




	if (basemodule!=NULL)
	{
		moduparam=basemodule->GetStaticParam();
		MinAtkDistance=moduparam->lMinAtkDistance;
		MaxAtkDistance=moduparam->lMaxAtkDistance;
	}
	else
	{
		Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());
		WhenLoseTarget();
		return TRACE_RESULT_TRACE;
	}

	//##�Ϻ�����Ҫ��
	LONG lDistance = m_pOwner -> RealDistance( pTarget );

	//����ڼ��ܵľ����ڣ� ��ʹ�ü���
	if(lDistance >= MinAtkDistance && lDistance <= MaxAtkDistance )
	{
		if (HasSkill)
		{
			return TRACE_RESULT_SUCCESS;
		}
		else
		{
			Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());
			WhenLoseTarget();
			return TRACE_RESULT_TRACE;
		}

	}
	//##�����Ȼ��׷����Χ�ڣ��Ϳ�ʼ������ҡ�
	else
	{
		Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());
		return TRACE_RESULT_TRACE;

	}
}

BOOL CFixedPositionArcher::RandomRun()
{
	Stand(1000);
	return FALSE;
}

BOOL CFixedPositionArcher::SearchEnemy()
{
	if(!CanSearchEnemy())
		return false;

	if(m_pOwner==NULL)
		return false;

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );//Ѱ�����ڳ���
	if( pRegion == NULL ) return TRUE;

	vector<CPlayer*> vOut;
	pRegion -> FindAroundPlayer( m_pOwner, vOut );

	//vector<CMonster*> vPets;
	//pRegion -> FindAroundPets( m_pOwner, vPets );

	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	CMoveShape* pTarget = NULL;//GetTarget();

	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CPlayer* pPlayer = vOut[i];
		if( pPlayer == NULL ) continue;
		if( pPlayer -> IsDied() ) continue;

		if ( !pPlayer->IsVisible(pOwner) )			// ����
		{
			continue;
		}

		int iAimDistance=m_pOwner -> Distance(pPlayer);
		if( pTarget )
		{
			if(iAimDistance <=pOwner->GetAckRangeMin() &&
				iAimDistance>= pOwner->GetAckRangeMax()&&
				pPlayer->GetExID()!= m_pOwner->GetBefoolID())
			{
				pTarget = pPlayer;
			}
		}
		else
		{
			if( iAimDistance >= pOwner -> GetAckRangeMin() &&
				iAimDistance <= pOwner -> GetAckRangeMax() &&
				pPlayer->GetExID()!= m_pOwner->GetBefoolID())
			{
				pTarget = pPlayer;
			}
		}
	}

	if( pTarget )
	{
		//##�����������ö���
		SetTarget( pTarget -> GetType(), pTarget -> GetExID() );
	}
	else
	{
		WhenLoseTarget();
	}

	return FALSE;
}
/*
CBaseAI::AI_EXEC_STATE CFixedPositionArcher::Run()
{
	return CMonsterAI::Run();
}

BOOL CFixedPositionArcher::OnChangeSkill()
{
	CBaseAI::OnChangeSkill();
	if( m_pOwner )
	{
		SelectAttackSkill();
		CSkill* pSkill = m_pOwner -> GetCurrentSkill();
		//##�����ǰ���ܲ�����,���ߵ�ǰ����û����ȴ���.
		if( pSkill == NULL || pSkill -> IsRestored() == FALSE )
		{
			if( pSkill == NULL )
			{
				m_pOwner -> SetCurrentSkill( m_pOwner -> GetDefaultAttackSkillID() );
			}
			else 
			{
				AddAIEvent( ASA_STAND, pSkill -> GetRestoreTime() );
			}
		}
	}
	return TRUE;
}

VOID CFixedPositionArcher::OnSchedule()
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


VOID CFixedPositionArcher::OnIdle()
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

VOID CFixedPositionArcher::WhenBeenHurted( LONG lType, LONG lID, DWORD dwStiffenTime )
{
	CBaseAI::WhenBeenHurted( lType, lID, dwStiffenTime );
	if( HasTarget() == FALSE )
	{
		SetTarget( lType, lID );
	}
}

//##��ÿ����Ҫ�ƶ���ʱ����Ѱ�����Player
BOOL CFixedPositionArcher::OnMoving()
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

BOOL CFixedPositionArcher::OnFighting()
{
	if( CMonsterAI::OnFighting() )
	{
		AddAIEvent( ASA_SEARCH_ENEMY );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CFixedPositionArcher::OnSearchEnemy()
{
	if( CMonsterAI::OnSearchEnemy() )
	{
		//##��������߲�����.ֱ�ӷ���.
		if( m_pOwner == NULL ) return TRUE;

		CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
		if( pRegion == NULL ) return TRUE;
		
		//##�����ǰ�������ܲ�����.
		CSkill* pSkill = m_pOwner -> GetCurrentSkill();
		if( pSkill == NULL ) return TRUE;

		vector<CPlayer*> vOut;
		pRegion -> FindAroundPlayer( m_pOwner, vOut );

		vector<CMonster*> vPets;
		pRegion -> FindAroundPets( m_pOwner, vPets );

		//vector<CMonster*> vCars;
		//pRegion->FindAroundCarriages( m_pOwner, vCars );

		CMoveShape* pTarget = NULL;
		for( size_t i = 0; i < vOut.size(); i ++ )
		{
			CPlayer* pPlayer = vOut[i];
			if( pPlayer == NULL ) continue;
			if( pPlayer -> IsDied() ) continue;

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

		for( size_t i = 0; i < vPets.size(); i ++ )
		{
			CMonster* pPet = vPets[i];
			if( pPet == NULL ) continue;
			if( pPet -> IsDied() ) continue;

			//##����ó���Ҳ�ھ��䷶Χ��
			if( m_pOwner -> RealDistance(pPet) <= 
				m_pOwner -> GetAlertRange() )
			{
				//##�����ǰĿ�겻���ڣ����Ȱ����Ŀ��������
				if( pTarget == NULL )
				{
					pTarget = pPet;
					continue;
				}

				//##�������ұ�ѡ���Ŀ��ľ���Ҫ�̡�
				if( m_pOwner -> RealDistance(pPet) < 
					m_pOwner -> RealDistance(pTarget) )
				{
					//##�������ҵľ���ȼ��ܵ���С��������Ҫ��
					if( m_pOwner -> RealDistance(pPet) >=
						pSkill -> GetAffectRangeMin() )
					{
						pTarget = pPet;
					}
				}
				//##�������ұ�ѡ��Ŀ��ľ���ҪԶ.
				else
				{
					//##���ѡ�е�Ŀ���ڼ��ܵ���С���÷�Χ��.
					if( m_pOwner -> RealDistance(pTarget) <
						pSkill -> GetAffectRangeMin() )
					{
						pTarget = pPet;
					}
				}
			}
		}
		//for( size_t i = 0; i < vCars.size(); i ++ )
		//{
		//	CMonster* pCar = vCars[i];
		//	if( pCar == NULL ) continue;
		//	if( pCar -> IsDied() ) continue;

		//	//##����ó���Ҳ�ھ��䷶Χ��
		//	if( m_pOwner -> RealDistance(pCar) <= 
		//		m_pOwner -> GetAlertRange() )
		//	{
		//		//##�����ǰĿ�겻���ڣ����Ȱ����Ŀ��������
		//		if( pTarget == NULL )
		//		{
		//			pTarget = pCar;
		//			continue;
		//		}

		//		//##�������ұ�ѡ���Ŀ��ľ���Ҫ�̡�
		//		if( m_pOwner -> RealDistance(pCar) < 
		//			m_pOwner -> RealDistance(pTarget) )
		//		{
		//			//##�������ҵľ���ȼ��ܵ���С��������Ҫ��
		//			if( m_pOwner -> RealDistance(pCar) >=
		//				pSkill -> GetAffectRangeMin() )
		//			{
		//				pTarget = pCar;
		//			}
		//		}
		//		//##�������ұ�ѡ��Ŀ��ľ���ҪԶ.
		//		else
		//		{
		//			//##���ѡ�е�Ŀ���ڼ��ܵ���С���÷�Χ��.
		//			if( m_pOwner -> RealDistance(pTarget) <
		//				pSkill -> GetAffectRangeMin() )
		//			{
		//				pTarget = pCar;
		//			}
		//		}
		//	}
		//}
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