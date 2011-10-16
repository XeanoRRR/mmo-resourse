#include "StdAfx.h"
#include "PassiveGladiator.h"
#include "..\Player.h"
#include "..\Monster.h"
#include "..\ServerRegion.h"
#include "AIUtilities.h"



CPassiveGladiator::CPassiveGladiator(void)
{
}

CPassiveGladiator::~CPassiveGladiator(void)
{
}
BOOL CPassiveGladiator::WhenBeenHurted(LONG lType,const CGUID& guid, DWORD hurt)
{
	if( lType == TYPE_PLAYER && m_pOwner != NULL )
	{
		//##���Ȳ����������Ƿ��������ڲ�
		BOOL bAlreadyInList = FALSE;
		for( size_t i = 0; i < m_vEnemy.size(); i ++ )
		{
			if( m_vEnemy[i] == guid )
			{
				bAlreadyInList = TRUE;
				break;
			}
		}
		//##��������ڡ���ֱ�Ӽӽ�ȥ��
		if( bAlreadyInList == FALSE )
		{
			m_vEnemy.push_back( guid );
		}

		//##�������������������Ͱ�����б��������˺��������ƿ���
		if( m_vEnemy.size() > 10 )
		{
			m_vEnemy.erase( m_vEnemy.begin() );
		}
	}

	CMonsterAI::WhenBeenHurted (lType,guid,hurt);//�����ǿ��Ŀ��ʱ��
	
	return TRUE;
}

BOOL CPassiveGladiator::SearchEnemy()
{
	if(!CanSearchEnemy())
		return false;

	if (m_vEnemy.size()==0)
	{
		return TRUE;
	}
	if (HasTarget())
	{
		return TRUE;
	}
	//##��������߲�����.ֱ�ӷ���.
	if( m_pOwner == NULL )
	{
		m_vEnemy.clear();
		return TRUE;
	}

	//##�������Ҳ������.ֱ�ӷ���.
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) 
	{
		m_vEnemy.clear();
		return TRUE;
	}

	//##�������Լ�����Ĺ�����.
	CPlayer* pNearestEnemy = NULL;
	vector<CGUID>::iterator it = m_vEnemy.begin(); 

	while(it!= m_vEnemy.end())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>( pRegion ->FindAroundObject(m_pOwner, TYPE_PLAYER, *it) );

		if (pPlayer != NULL && pPlayer->IsDied() == FALSE && m_pOwner -> RealDistance(pPlayer) <= m_pOwner -> GetTrackRange())
		{
			if ( !pPlayer->IsVisible(m_pOwner) )			// ����
			{
				it= m_vEnemy.erase(it);
				continue;
			}

			if(  pPlayer->GetExID() == m_pOwner->GetBefoolID() )
			{
				it= m_vEnemy .erase(it);
				continue;
			}

			if( AIUtilities::IsInGodState( pPlayer ) )
			{
				it = m_vEnemy.erase( it );
				continue;
			}

			if (pNearestEnemy== NULL || m_pOwner -> RealDistance(pPlayer) < m_pOwner -> RealDistance(pNearestEnemy))
			{
				pNearestEnemy = pPlayer;
			}
			it++;
		}
		else
		{
			it= m_vEnemy.erase(it);
		}
			
	}

	//##����ҵ��˹�����.
	if( pNearestEnemy )
	{
		//##��������������ö���
		SetTarget( pNearestEnemy -> GetType(), pNearestEnemy -> GetExID() );
	}
	return TRUE;


}
VOID CPassiveGladiator::Clear()
{
	CBaseAI::Clear();
	m_vEnemy.clear();
}

VOID CPassiveGladiator::ResumeAll( bool bForce )
{
	CMonsterAI::ResumeAll( bForce );
	m_vEnemy.clear();
}
VOID CPassiveGladiator::SetTarget( LONG lType, const CGUID& TargetGuid )
{
	CBaseAI::SetTarget(lType,TargetGuid);
}
