#include "StdAfx.h"
#include ".\monsterai.h"
#include "..\monster.h"
#include "..\Player.h"
#include "..\Colony.h"
#include "..\ServerRegion.h"
#include <mmsystem.h>
#include "..\..\..\setup\globesetup.h"
#include "..\script\Script.h"
#include "..\BuffSkill\BuffSkillDef.h"
#include "..\BuffSkill\BaseModule.h"
#include "..\BuffSkill\NewSkillFactory.h"
#include "../../GameServer/GameManager.h"
#include "AIUtilities.h"
#include "AIFactory.h"





CMonsterAI::CMonsterAI()
{

	mm=0;
	//ѡ����id
	m_dwChoiceSkillID= 0;;
	//ѡ���ܵȼ�
	m_dwChoiceSkillLevel= 0;
	//ѡ��������
	m_wSkillType= 0;
	//ѡ��������
	m_wSkillIndex= 0;
	//�Ƿ���뱻ʹ��
	m_bMustUsed=FALSE;
	//�Ƿ�ʹ��
	m_bBeenUsed=TRUE;

	//���˱�ʶ
	//m_fGBHStamp=100.0;				
	//����ʱ���ʶ
	m_dwGuardTimeStamp=0;			
	//��󱻹���ʱ���ʶ
	m_dwHurtedTimeStamp=0;		
	//���ܼ��ʱ���־
	m_dwSkillSpaceTimeStamp=0;	
	//���ɽ׶�ʱ���־
	m_dwTransitionTimeStamp=0;
	m_dwRebornTime = 0;

	//·�����


	//���ߵ��¼
	m_iRoadRecord=0;				
	//���߷���
	m_iRoadDirect=1;             
	////�Ƿ��뿪��·��
	//m_bIsLeaveRoad;				
	m_bAttackedByGuard = false;

}
CMonsterAI::~CMonsterAI(){}


BOOL CMonsterAI::OnCycle()
{
	CycleSpring();
	return TRUE;
}
//������Ϊ��Ӧ
BOOL CMonsterAI::OnActive()
{
	if (m_pOwner==NULL)
	{
		return FALSE;
	}
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[256]="";
	_snprintf(pszStatckInfo,256,"CMonsterAI::OnActive(MonsterState:%d, FightSubState:%d) Start", 
		m_pOwner->GetState(), m_aesAIState );
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	BOOL bRet = TRUE;
	do
	{

		if (CheckRandomRunState()== TRUE)
		{
			break;
		}
		// �����Ѻ�ʱ�������������
		if( !IsInKindlyTime() && CheckSearchEnemy()==TRUE)
		{
			break;
		}

		//��ͬ״̬���ò�ͬ�Ĵ���
		if (m_pOwner->GetState()==CShape::STATE_PEACE)
		{
			OnPeaceState();
		}
		else if (m_pOwner->GetState()== CShape::STATE_FIGHT)
		{
			if(CheckCycleSpring(m_csCycleSpring))//����ս�����ڴ�����������ڴ���Ӱ����������Ϊ��ֱ�ӷ���
			{
				break;
			}
			if (CheckReturnPeace())
			{
				break;
			}

			if (m_aesAIState== AES_FIGHT_NORMAL)
			{
				OnFightState();
			}
			else if (m_aesAIState== AES_FIGHT_TRANSITION)
			{
				OnTransitionState();
			}
			else if (m_aesAIState== AES_FIGHT_ESCAPE)
			{
				OnEscapeState();
			}
			else
			{
				bRet = FALSE;
				break;
			}
		}
		else if(m_pOwner->GetState()== CShape::STATE_DIED)
		{
			OnNoHpState();
		}
		else
		{
			bRet = FALSE;
		}
	} while( 0 );

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMonsterAI::OnActive() End");
#endif

	return bRet;
}

BOOL CMonsterAI::OnChangeAI ()
{
	if (m_pOwner==NULL)
	{
		return FALSE;
	}
	return dynamic_cast<CMonster*>(m_pOwner)->ChangeAI();
}

BOOL CMonsterAI::OnReBorn()
{
	// ��¼����ʱ��
	m_dwRebornTime = timeGetTime();
	CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
	if (pMonster!= NULL && pMonster->GetColonyID()!= 0 && pRegion!= NULL)
	{
		CColony* pColony= pRegion->GetOneColony(pMonster->GetColonyID());
		if (pColony!= NULL)
		{
			pColony->WhenBorn(m_pOwner);
		}
	}
	return TRUE;
}
	//��ƽ״̬
BOOL CMonsterAI::OnPeaceState()
{
	extern long AREA_WIDTH;		// ����Ŀ��
	extern long AREA_HEIGHT;	// ����ĸ߶�
	DWORD x,y,  area_x,area_y;

	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);


	if ( pOwner->GetAIHangupType()==0 && pOwner->GetArea()->IsHibernate())
	{	
		x=m_pOwner->GetTileX();
		y=m_pOwner->GetTileY();
		area_x=x/AREA_WIDTH;
		area_y=y/AREA_HEIGHT;
		INT born_x=m_BornPoint.x/AREA_WIDTH;
		INT born_y=m_BornPoint.y/AREA_HEIGHT;

		if (area_x == m_pOwner->GetArea()->GetX() && area_y == m_pOwner->GetArea()->GetY()
			&& area_x== born_x && area_y== born_y)
		{
			Hibernate();
			return TRUE;
		}
	
	}
	else if (dynamic_cast<CServerRegion*>(pOwner->GetFather())->IsHibernate())
	{
		if (pOwner->GetAIHangupType()<2)
		{
			Hibernate();
			return TRUE;
		}
	}

	


	BOOL bReturnType=FALSE;

	if(CheckCycleSpring(m_csCycleSpring))
	{
		//�����ƽ���ڵĴ���
		
		//return true;
		bReturnType=TRUE;
	}
	else if (OnPeaceStateOnColony()==TRUE)//��Ⱥ�ֵĺ�ƽ״̬����
	{
		bReturnType=TRUE;
	}
	else 
	{
		if(RunByRoad())
		{
			bReturnType=TRUE;
		}
		else 
		{
			OnPeaceIdle();
		}

	}

	return bReturnType;
}

BOOL CMonsterAI::OnPeaceStateOnColony()
{
	//��Ⱥai����
	CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);
	if (pMonster!= NULL && pMonster->HasColony()==TRUE && pMonster->GetColonyState()==TRUE)
	{
		CServerRegion* pServer= dynamic_cast<CServerRegion*>(pMonster->GetFather());
		CColony* pColony=0;
		if (pServer!= NULL)
		{
			pColony= pServer->GetOneColony(pMonster->GetColonyID());
		}
		if (pColony!= NULL)
		{
			pColony->OnPeaceState(m_pOwner);
			
			 //point;//= pColony->GetAimPoint(pMonster->GetExID());
			if (pColony->HasRoad())
			{
				POINT point=pColony->GetAimPoint(pMonster->GetExID());
				if (point.x== 0 || point.y== 0)	return FALSE;
				LONG Distance= pMonster->Distance(point.x,point.y);
				if (Distance> pMonster->GetColonyDistance())
				{
					pMonster->SetPosition(point.x,point.y);
					Stand(pMonster->GetStopFrame());
				}
				else 
				{
					if (m_pOwner->GetTileX()== point.x && m_pOwner->GetTileY()== point.y)
					{
						Stand(STAND_ONCE_TIME);
					}
					else
					{
						CBaseAI::MoveTo(point.x, point.y);
					}
				}
			}
			else
			{
				POINT point=pColony->GetLeaderPoint();
				if (point.x== 0 || point.y== 0)	return FALSE;
				LONG Distance= pMonster->Distance(point.x,point.y);

				if (Distance> pMonster->GetColonyDistance())
				{
					POINT aimPoint;
					pServer->GetRandomPosInRange(aimPoint.x,aimPoint.y ,point.x,point.y,pMonster->GetColonyDistance(),pMonster->GetColonyDistance());
					pMonster->SetPosition(aimPoint.x,aimPoint.y);
					Stand(pMonster->GetStopFrame());
				}
				else
				{
					RandomRunOnColony();
				}
			}
			


			//else
			//{
			//	if (pColony->HasRoad() )//�ṹ�ı�
			//	{
			//		if (m_pOwner->GetTileX()== point.x && m_pOwner->GetTileY()== point.y)
			//		{
 			//			Stand(STAND_ONCE_TIME);
			//		}
			//		else
			//		{
			//			CBaseAI::MoveTo(point.x, point.y);
			//		}
			//	}
			//	else
			//	{
			//		RandomRunOnColony(); 
			//	}
			//}
			return TRUE;

		}
	}
	return FALSE;
}

//ս��״̬
BOOL CMonsterAI::OnFightState()
{
#ifdef _DUMP
	char filename[64];
	_snprintf(filename, 64, "AI_%d",GetAIType());
	PutStringToFile(filename,"fight_state");
#endif

	if (OnFightStateOnColony()==TRUE)
	{
		return TRUE;
	}

	CMoveShape* pShape = GetTarget();

	if (pShape!=NULL)
	{
		int TraceResult=Tracing(pShape);

		if(TraceResult==TRACE_RESULT_SUCCESS)     //׷Ѱ
		{
			m_pOwner->InitSkillInstance(m_dwChoiceSkillID,m_dwChoiceSkillLevel,0,0,0,NULL_GUID,5);
			BeginSkill(m_pOwner->GetInstanceSkill(),
				pShape->GetTileX(),pShape->GetTileY(),pShape);
			RecordUsedCurrentSkill();
			CleanChoiceSkillRecord();
		}
		else
		{
			if (GetChoiceSkillType()!= SKILLTYPE_CYCLE)
			{
				CleanChoiceSkillRecord();
			}
		}
		return true;
	}
	else
	{
		Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());//return CBaseAI::RandomRun();
		LoseTarget();
		return true;
	}

	#ifdef _DUMP
			char text[64];
			_snprintf(text, 64, "OnFightState:hasTarget_%d;fightstate_%d",HasTarget(),m_aesAIState);
			PutStringToFile("ai_Exceptional",text);
	#endif


}

BOOL CMonsterAI::OnFightStateOnColony()
{
	if (HasTarget()==TRUE)	return FALSE;

	CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);

	if (pMonster!= NULL && pMonster->HasColony()==TRUE && pMonster->GetColonyState()==TRUE)
	{
		CServerRegion* pServer= dynamic_cast<CServerRegion*>(pMonster->GetFather());
		CColony* pColony=0;
		if (pServer== NULL)	return FALSE;

		pColony= pServer->GetOneColony(pMonster->GetColonyID());

		if (pColony== NULL)	return FALSE;

		//pColony->OnFightState(m_pOwner);

		if (m_aesAIState!= AES_FIGHT_TRANSITION)
		{
			CMoveShape* pFriend= SearchFriendOnColony();
			if (pFriend!= NULL)
			{
				if (FlyToFriendOnColony(pFriend)==TRUE)
				{
					//Stand(STAND_ONCE_TIME);
				}
				else
				{
					CBaseAI::MoveTo(pFriend->GetTileX(),pFriend->GetTileY());
				}
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

BOOL CMonsterAI::OnEscapeState()
{
	// ����һ������DEBUFF
	if( m_pOwner->GetStateParamByID( STATE_BASE_WEAKNESS, "Num" ) == 0 )
	{
		m_pOwner->AddNewState( m_pOwner, STATE_BASE_WEAKNESS, 1,10 );
	}

	return Escape();
}

BOOL CMonsterAI::OnNoHpState()
{
	m_pOwner->OnDied(); /* need m_bAttackedByGuard */
	m_bAttackedByGuard = false;
	ChangeStateTo(CShape::STATE_DIED);
	m_aesAIState=AES_FIGHT_NORMAL;

	if (m_pOwner->GetCollectionID()== 0)
	{
		StartDiedEvent(CGlobeSetup::GetSetup()->dwMonsterKeepDeadTime);//AddAIEvent(AET_DIED,);//���崦��ɷ�ɼ���
	}
	else
	{
		StartDiedEvent(CGlobeSetup::GetSetup()->dwCanCollectMonsterKeepDeadTime);//AddAIEvent(AET_DIED,);
	}
	return TRUE;
}

BOOL CMonsterAI::OnDeadState()
{
	//m_pOwner->OnDelEvent();
	m_pOwner->AddDelEvent(0);
	return TRUE;
}

BOOL CMonsterAI::OnTransitionState()
{

	CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);
	if (pMonster== NULL)	return FALSE;
	CServerRegion *pRegion= dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
	if (pRegion== NULL)	return FALSE;
	CColony* pColony= dynamic_cast<CColony*>(pRegion->GetOneColony(pMonster->GetColonyID()));

	if (pColony!= NULL)
	{
		pColony->OnFightState(m_pOwner);
	}	

	if (timeGetTime() > m_dwTransitionTimeStamp+ CGlobeSetup::GetSetup()->dwMonsterTransitionTime)
	{	

		if (pColony!= NULL && pColony->HasTarget())
		{
			CMoveShape* pFriend= SearchFriendOnColony();
			if (pFriend!= NULL)
			{
				//FlyToFriendOnColony(pFriend);
				if (FlyToFriendOnColony(pFriend)==TRUE)
				{
					Stand(STAND_ONCE_TIME);
				}
				else
				{
					CBaseAI::MoveTo(pFriend->GetTileX(),pFriend->GetTileY());
				}

			}
			m_aesAIState= AES_FIGHT_NORMAL;
			m_dwGuardTimeStamp= timeGetTime()+pMonster->GetGuardTime();
		}
		else
		{
			ReturnPeace();
			Stand(0);
		}
	}
	else
	{
		Stand(STAND_ONCE_TIME);
	}
	return TRUE;
}

	//�����£�Ѱ�ҵ���
BOOL CMonsterAI::SearchEnemy()
{
	if(!CanSearchEnemy())
		return false;


	if(m_pOwner==NULL)
		return false;

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );//Ѱ�����ڳ���
	CMonster*	pOwner=dynamic_cast<CMonster*>(m_pOwner);

	if( pRegion == NULL ) return TRUE;

	vector<CPlayer*> vOut;
	pRegion -> FindAroundPlayer( m_pOwner, vOut );

	CMoveShape* pTarget = NULL;
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CPlayer* pPlayer = vOut[i];
		if( pPlayer == NULL ) continue;
		if( pPlayer -> IsDied() ) continue;

		if ( !pPlayer->IsVisible(m_pOwner) )			// ����
		{
			continue;
		}

		if( AIUtilities::IsInGodState( pPlayer ) )	// �����޵�BUF״̬
		{
			continue;
		}

		if( pTarget )
		{
			if( m_pOwner -> Distance(pPlayer) <= pOwner -> GetAttribute(string("C_GuardRange")) &&//GetGuardRange() &&
				 m_pOwner->Distance(pPlayer) < m_pOwner->Distance(pTarget) &&
				 pPlayer->GetExID()!= m_pOwner->GetBefoolID())
			{
				pTarget = pPlayer;
			}
		}
		else
		{
			if( m_pOwner -> Distance(pPlayer) <= pOwner -> GetAttribute(string("C_GuardRange")) &&  pPlayer->GetExID()!= m_pOwner->GetBefoolID())
				
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

	return TRUE;
}

//�����������
BOOL CMonsterAI::SearchEnemyRandom(int iDisMax,int iDisMin)
{
	//if(!CanSearchEnemy())
	//	return false;


	if(m_pOwner==NULL)
		return false;

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );//Ѱ�����ڳ���
	CMonster*	pOwner=dynamic_cast<CMonster*>(m_pOwner);

	if( pRegion == NULL ) return TRUE;

	vector<CPlayer*> vOut;
	pRegion -> FindAroundPlayer( m_pOwner, vOut );

	//vector<CMonster*> vPets;
	//pRegion -> FindAroundPets( m_pOwner, vPets );

	//CMoveShape* pTarget = NULL;
	vector<CMoveShape*> vAim;

	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CPlayer* pPlayer = vOut[i];
		if( pPlayer == NULL ) continue;
		if( pPlayer -> IsDied() ) continue;

		if (!pPlayer->IsVisible(m_pOwner) )			// ����
		{
			continue;
		}

		long lDis= m_pOwner -> Distance(pPlayer);
		if (lDis<=iDisMax && lDis>=iDisMin && pPlayer->GetExID()!= m_pOwner->GetBefoolID()) 
		{
			vAim.push_back(pPlayer);
		}
	}

	CMoveShape* pTarget= vAim[random(vAim.size())];
	SetTarget(pTarget->GetType() , pTarget -> GetExID() );

	return TRUE;
}

BOOL CMonsterAI::CanSearchEnemy()
{
	if(m_pOwner && m_pOwner->GetForceAttack())
	{
		if (dynamic_cast<CMonster*>(m_pOwner)->GetSearchEnemyScript()!="0"
			&& dynamic_cast<CMonster*>(m_pOwner)->GetSearchEnemyScript()!="null")
		{
			SearchEnemyByScript();
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
BOOL CMonsterAI::CheckSearchEnemy()
{
	// ������ڹ�λ���ڣ��Ͳ�������
	if( m_csCycleSpring.csSpringType == CS_LEVELFIGHT )
	{
		return FALSE;
	}

	//�Ƿ�Ӧ�ö�ʧĿ��
	if (m_aesAIState== AES_FIGHT_TRANSITION)
	{
		return FALSE;
	}
	if (m_pOwner->GetState()!= CShape::STATE_PEACE &&
		m_pOwner->GetState()!= CShape::STATE_FIGHT)
	{
		return FALSE;
	}
	if (HasTarget() )
	{
		CMoveShape* pShape = GetTarget();
		if( pShape == NULL || pShape -> IsDied() ||
			pShape -> IsAttackAble(m_pOwner) == false )
		{
			WhenLoseTarget();
		}
		else
		{
			LONG lDistance=m_pOwner->Distance(pShape);
			if ( lDistance> m_pOwner->GetTrackRange() )
			{
				WhenLoseTarget();
			}

			if ( !pShape->IsVisible(m_pOwner) )			// ����
			{
				WhenLoseTarget();
			}
		}
	}
	
	//��ȺѰ��
	if (SearchEnemyOnColony()== TRUE)
	{
		return TRUE;
	}

	
	//����ai��
	if (m_pOwner->GetState()==CShape::STATE_PEACE ||
		(m_pOwner->GetState()== CShape::STATE_FIGHT && m_aesAIState!= AES_FIGHT_NOHP))
	{
		if (!m_pOwner->GetRandomMove())
		{
			RandomRun();
			return TRUE;
		}
		
		if (m_pOwner->GetState()== CShape::STATE_FIGHT && m_aesAIState!= AES_FIGHT_NOHP && m_csCycleSpring.csSpringType!= CS_NULL)
		{
			//��
		}
		else
		{
			if (m_pOwner->GetState()== CShape::STATE_FIGHT && m_aesAIState== AES_FIGHT_ESCAPE)
			{
				CMonsterAI::SearchEnemy();
			}
			else
			{
				SearchEnemy();
			}
		}	
	}

	if( HasTarget() && m_aesAIState != AES_FIGHT_ESCAPE )
	{
		EscapeSpring();
	}

	return FALSE;
}
//�ڼ�Ⱥ����������
BOOL CMonsterAI::SearchEnemyOnColony()
{
	CMonster* pOwner= dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner== NULL ) return FALSE;
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
	if (pRegion== NULL)	return FALSE;

	DWORD dwColonyID= pOwner->GetColonyID();
	CColony* pColony= pRegion->GetOneColony(dwColonyID);

	CMoveShape* pTarget=0;

	if (pColony!=NULL)
	{
		pColony->OnFightState(m_pOwner);
		if (pOwner->GetMemberType()==2)   //2 ������Ϊ�˹ֵ�Ŀ���ʵ���쵼��Ŀ��һ��
		{
			CMonster* pLeader= pColony->GetLeader();
			pTarget= pColony->GetLeaderTarget();
			if (pLeader!= NULL && pTarget!= NULL)
			{
				if (GetTarget()!= pTarget)
				{
					SetTarget(pTarget->GetType(),pTarget->GetExID());
				}

				LONG lDistance= pOwner->Distance(pTarget);
				LONG lFriendDis=pOwner->Distance(pLeader);

				if( lDistance < pOwner->GetColonyDistance() || lFriendDis < pOwner->GetColonyDistance() )
				{
					return FALSE;
				}
				if (FlyToFriendOnColony(pLeader)==FALSE)
				{
					Stand(STAND_ONCE_TIME);
				}
				
				return TRUE;
			}
		}
		vector<CMoveShape*> vecMoveShape;
		pColony->GetTargetList(vecMoveShape);
		if (vecMoveShape.size()==0)	return FALSE;
		for (vector<CMoveShape*>::iterator it= vecMoveShape.begin();
			it!= vecMoveShape.end(); it++)
		{
			CMoveShape* pMoveShape= *it;
			if( pMoveShape == NULL ) continue;
			if( pMoveShape -> IsDied() ) continue;

			if (!pMoveShape->IsVisible(m_pOwner) )		// ����
			{
				continue;
			}

			if( pTarget )
			{
				if( m_pOwner -> Distance(pMoveShape) < pOwner -> GetTrackRange() &&
					m_pOwner->Distance(pMoveShape) < m_pOwner->Distance(pTarget) )
				{
					pTarget = pMoveShape;
				}
			}
			else
			{
					pTarget = pMoveShape;
			}
		}

		if( pTarget != NULL )
		{
			//##�����������ö���			
			SetTarget( pTarget -> GetType(), pTarget -> GetExID() );
			LONG lDistance=m_pOwner->Distance(pTarget);
			if ( lDistance> m_pOwner->GetTrackRange() )
			{
				WhenLoseTarget();
			}
		}
		return FALSE;
	}
	return FALSE;
}
//�ڼ�Ⱥ��Ѱ�����������Ŀ��ĳ�Ա
CMonster* CMonsterAI::SearchFriendOnColony()
{
	CMonster* pOwner= dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner== NULL ) return FALSE;
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
	if (pRegion== NULL)	return FALSE;

	DWORD dwColonyID= pOwner->GetColonyID();
	CColony* pColony= pRegion->GetOneColony(dwColonyID);

	CMonster* pTarget=0;


	if (pColony!=NULL)
	{
		vector<CMonster*> vecMonster;
		pColony->GetFightMemberList(vecMonster);
		for (vector<CMonster*>::iterator it= vecMonster.begin();
			it!= vecMonster.end(); it++)
		{
			CMonster* pMonster= *it;
			if (pMonster==NULL)	continue;
			if (pMonster->IsDied())	continue;
			if (pTarget== NULL)
			{
				pTarget= pMonster;
			}
			else
			{
				if (m_pOwner->Distance(pTarget)<m_pOwner->Distance(pMonster))
				{
					pTarget=pMonster;
				}
			}

			//if( pTarget== NULL )
			//{
			//	if (pMonster->GetAI()->HasTarget())
			//	{
			//		pTarget= pMonster;
			//	}
			//}
			//else
			//{
			//	if (pMonster->GetAI()->HasTarget() &&
			//		pOwner->Distance(pMonster)< pOwner->Distance(pTarget))
			//	{
			//		pTarget= pMonster;
			//	}
			//}
			
		}
	}
	return pTarget;

}
//�ýű�Ѱ�ҵ���
BOOL CMonsterAI::SearchEnemyByScript()
{return FALSE;}
	//Ѱ�Ҽ���
BOOL CMonsterAI::SearchOneTypeSkill(DWORD dwSkillID,DWORD dwSkillLevel,DWORD dwType,LONG lDistance)
{

	CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwSkillID);
	if (pSkillProperty== NULL)
	{		
		return FALSE;
	}		
	if (dwType!= SKILLTYPE_CYCLE)
	{
		int maxDis=pSkillProperty->GetStaticParam(dwSkillLevel)->lMaxAtkDistance;
		int minDis=pSkillProperty->GetStaticParam(dwSkillLevel)->lMinAtkDistance;
		if ( lDistance>maxDis || lDistance<minDis)
		{
			return FALSE;
		}
	}

	if (!m_pOwner->GetUseRabbSkill() && pSkillProperty->GetUseType()==SKILL_TYPE_MAGIC)
	{
		return FALSE;
	}
	if (!m_pOwner->GetUseFightSkill() && pSkillProperty->GetUseType()==SKILL_TYPE_PHYSICAL)
	{
		return FALSE;
	}
	

	BOOL BufferCheck=TRUE;
	list<long> lBuffID = pSkillProperty->GetStaticParam(dwSkillLevel)->lBuffID;//�����е�״̬
	for (list<long>::iterator it=lBuffID.begin();it!=lBuffID.end();++it)
	{

		map<LONG,stModuParam*>::iterator bt = m_pOwner->GetBuff().find(*it);
		if (bt == m_pOwner->GetBuff().end())
		{
			continue;
		}
		else
		{
			BufferCheck=FALSE;
			break;
		}
	}

	if (BufferCheck==FALSE)
	{
		return FALSE;
	}

	list<long> lBuffID_=pSkillProperty->GetStaticParam(dwSkillLevel)->lNBuffID;//����û�е�״̬
	for (list<long>::iterator it=lBuffID_.begin();it!=lBuffID_.end();it++)
	{
		map<LONG,stModuParam*>::iterator bt = m_pOwner->GetBuff().find(*it);
		if (bt != m_pOwner->GetBuff().end())
		{
			continue;
		}
		else
		{
			BufferCheck=FALSE;
			break;
		}
	}

	if (BufferCheck==FALSE)
	{
		return FALSE;
	}

	return TRUE;
}
BOOL CMonsterAI::SearchSkill(CMoveShape* pTarget)//
{
	//m_pOwner->SetInstanceSkill(NULL);

	if(!CBaseAI::SearchSkill(pTarget))		//���Ƿ�Ӧ�ü���Ѱ�Ҽ���
		return FALSE;
	if (HasChoiceSkill())
		return FALSE;

	BOOL bMustUsed=FALSE;

	if (!m_pOwner ) //���ownerΪ�գ� �����Ѿ��м��ܣ��򷵻�
	{
		return FALSE;
	}
	if (!m_pOwner->GetUseFightSkill())
	{
		return FALSE;
	}
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	LONG lDistance = m_pOwner ->RealDistance( pTarget );

	if( lDistance < 0 )
	{
		lDistance = 0;
	}
	int skill_min=pOwner->GetAckRangeMin();
	int skill_max=pOwner->GetAckRangeMax();
	int iProportion=pOwner->GetSkillProportion();

	if (lDistance<skill_min || lDistance> skill_max)
	{
		return TRUE;
	}
	LONG thisTime=timeGetTime();
	//
	struct SkillChoicRecord
	{
		DWORD SkillID;
		DWORD SkillLevel;
		WORD SkillType;
		WORD SkillIndex;
	};
	vector<SkillChoicRecord> SearchedSkill;


	//ѡ�����ڴ�������
	vector<CMonster::tagSkillRecord>& cycleskillmap=pOwner->GetCycleSkillList();
	if (cycleskillmap.size()!=0)
	{
		for (int a=0; a<cycleskillmap.size(); a++)
		{
			if (cycleskillmap[a].dwCoolDownTime> thisTime) continue;

			if (!SearchOneTypeSkill(cycleskillmap[a].wID,cycleskillmap[a].wLevel,SKILLTYPE_CYCLE,lDistance))
			{
				continue;
			}

			SkillChoicRecord record;
			record.SkillType = SKILLTYPE_CYCLE;
			record.SkillIndex = a;
			record.SkillID= cycleskillmap[a].wID;
			record.SkillLevel= cycleskillmap[a].wLevel;
			SearchedSkill.push_back(record);
		}
	}


	//ѡ����ͨ����
	if( SearchedSkill.size() == 0 )
	{
		if( thisTime > m_dwSkillSpaceTimeStamp )
		{
			m_dwSkillSpaceTimeStamp = thisTime + pOwner->GetSkillSpace();
			if( random( 100 ) < iProportion )
			{
				vector<CMonster::tagSkillRecord>& skillmap=pOwner->GetSkillList();
				if (skillmap.size()!=0)
				{
					for (int a=0; a<skillmap.size(); a++)
					{	
						if (skillmap[a].dwCoolDownTime> thisTime) continue;

						if (!SearchOneTypeSkill(skillmap[a].wID,skillmap[a].wLevel,SKILLTYPE_NORMAL,lDistance))
						{
							continue;
						}

						SkillChoicRecord record;
						record.SkillType = SKILLTYPE_NORMAL;
						record.SkillIndex =a;
						record.SkillID= skillmap[a].wID;
						record.SkillLevel= skillmap[a].wLevel;
						SearchedSkill.push_back(record);
					}
				}
			}
		}
	}

	//ѡ���������
	if (SearchedSkill.size()==0 && iProportion!=100)
	{
		vector<CMonster::tagSkillRecord>& skillmap=pOwner->GetBaseSkillList();
		if (skillmap.size()!= 0)
		{
			for (int a=0; a<skillmap.size(); a++)
			{
				if (skillmap[a].dwCoolDownTime> thisTime) continue;
				if (!SearchOneTypeSkill(skillmap[a].wID,skillmap[a].wLevel,SKILLTYPE_BASE,lDistance))
				{
					continue;
				}

				SkillChoicRecord record;
				record.SkillType = SKILLTYPE_BASE;
				record.SkillIndex =a;
				record.SkillID= skillmap[a].wID;
				record.SkillLevel= skillmap[a].wLevel;
				SearchedSkill.push_back(record);
			}
		}
	}


	if (SearchedSkill.size()!=0)
	{
		int random_num=random(SearchedSkill.size());
		SkillChoicRecord& record=SearchedSkill[random_num];
		RecordChoiceSkill(record.SkillID,record.SkillLevel,record.SkillType,record.SkillIndex);
	}
	else
	{
		m_pOwner->SetInstanceSkill(NULL);
	}

	return TRUE;

}

	//����
BOOL CMonsterAI::Escape()
{
	CMoveShape* pTarget = GetTarget();

	if (pTarget!=NULL)
	{
		LONG lX		= pTarget ->GetTileX();
		LONG lY		= pTarget ->GetTileY();
		Escape(dynamic_cast<CRegion*>(m_pOwner->GetFather()),lX,lY,FALSE);
		return true;
	}
	else
	{
		// �Ƴ�����״̬���ָ�����
		m_pOwner->DelNewState( STATE_BASE_WEAKNESS, 1 );

		Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());
		WhenLoseTarget();
		return TRUE;
	}
}

VOID CMonsterAI::Escape(CRegion* pRegion, LONG lX_, LONG lY_, BOOL bRun )
{
	LONG lX = m_pOwner -> GetTileX();
	LONG lY = m_pOwner -> GetTileY();

	LONG lDir=GetLineDir(lX_,lY_,lX,lY);
	//m_pOwner -> GetDirPos( lDir, lX, lY );

	MoveTo(lDir);
}

VOID CMonsterAI::Escape(CRegion* pRegion, LONG lDir, BOOL bRun)
{
	LONG lDirect=(lDir+4)%8;
	MoveTo(lDirect);
}

	//׷��,����û�� Ŀ�� �� ������ ���жϣ�������Ҫ�ڵ��ô˺���ǰ�������жϡ�
INT CMonsterAI::Tracing(CMoveShape* pTarget)
{
	SearchSkill(pTarget);
	BOOL HasSkillConditon= FALSE;
	BOOL HasSkillToAttac= TRUE;

	INT MinAtkDistance=0;
	INT MaxAtkDistance=0;

	CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(m_dwChoiceSkillID);
	if (pSkillProperty== NULL)
	{	
		vector<CMonster::tagSkillRecord>& skillmap=dynamic_cast<CMonster*>(m_pOwner)->GetBaseSkillList();
		if (skillmap.size()>0)
		{
			pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(skillmap[0].wID);
		}
		HasSkillToAttac= FALSE;
		CleanChoiceSkillRecord();
	}	
	if (pSkillProperty!=NULL)
	{
		
		if (HasSkillToAttac)
		{
			MaxAtkDistance=pSkillProperty->GetStaticParam(m_dwChoiceSkillLevel)->lMaxAtkDistance;
			MinAtkDistance=pSkillProperty->GetStaticParam(m_dwChoiceSkillLevel)->lMinAtkDistance;
		}
		else
		{
			vector<CMonster::tagSkillRecord>& skillmap=dynamic_cast<CMonster*>(m_pOwner)->GetBaseSkillList();
			if (skillmap.size()>0)
			{
				MaxAtkDistance=pSkillProperty->GetStaticParam(skillmap[0].wLevel)->lMaxAtkDistance;
				MinAtkDistance=pSkillProperty->GetStaticParam(skillmap[0].wLevel)->lMinAtkDistance;
			}
		}

		HasSkillConditon =TRUE;
	}
	if (HasSkillConditon==FALSE)
	{
		RandomRun();
		return TRACE_RESULT_TRACE;
	}

	//##�Ϻ�����Ҫ��
	LONG lDistance = m_pOwner -> RealDistance( pTarget );
	//LONG lDistance= Distance(m_pOwner->GetTileX(),m_pOwner->GetTileY(),pTarget->GetTileX(),pTarget->GetTileY());
	if (lDistance<0) lDistance=0;

	//����ڼ��ܵľ����ڣ� ��ʹ�ü���
	if(lDistance >= MinAtkDistance && lDistance <= MaxAtkDistance )
	{
		if (HasSkillToAttac==TRUE )
		{
			return TRACE_RESULT_SUCCESS;
		}
		else
		{
			Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());
			return TRACE_RESULT_TRACE;
		}
		
	}
	//##�����Ȼ��׷����Χ�ڣ��Ϳ�ʼ������ҡ�
	else if(m_pOwner -> GetTrackRange() >=  lDistance )
	{
		//##���������󹥻����뷶Χ��,����ҿ���
		if( lDistance > MaxAtkDistance)
		{
			CBaseAI::MoveTo(pTarget->GetTileX(),pTarget -> GetTileY());
		}
		//##��������С��������֮�ڣ��������ƿ�
		else
		{
			LONG lX		= m_pOwner -> GetTileX();
			LONG lY		= m_pOwner -> GetTileY();

			LONG lDir	= GetLineDir( 
				pTarget -> GetTileX(), pTarget -> GetTileY(), lX, lY );

			//m_pOwner -> GetDirPos( lDir, lX, lY );

			MoveTo( lDir);
		}

		return TRACE_RESULT_TRACE;
	}
	//##��ҳ�����Χ������׷һ�������ض�ʧĿ��
	else
	{
		CBaseAI::MoveTo(pTarget -> GetTileX(), pTarget -> GetTileY());
		WhenLoseTarget();
		return TRACE_RESULT_OUT;
	}
}

//�жϸø����Ƿ����ƶ�
BOOL CMonsterAI::IsCanMoveTo(CRegion* pRegion,LONG lX, LONG lY)
{
	if(NULL == pRegion)	return FALSE;
	CMonster *pMonster = dynamic_cast<CMonster*>(m_pOwner);
	//�жϸ�monster�Ƿ����赲
	if(!pMonster)	return FALSE;
	//���赲
	BYTE byBlock = pRegion->GetBlock(lX,lY);
	if (byBlock !=CRegion::BLOCK_NO && byBlock!= CRegion::BLOCK_AIM)	return FALSE;

	BYTE byBlockType= pMonster->GetBlockType();	
	if(byBlockType==CRegion::BLOCK_AIM)//������ﲻ�ܲȹ����赲
	{
		if ( byBlock!= CRegion::BLOCK_NO)
			return FALSE;
	}

	vector<CRegion::tagSwitch> &vecSwitch = pRegion->GetSwitchVec();
	WORD SwitchIndex = pRegion->GetCell( lX, lY )->lSwitch;
	if( SwitchIndex != 0 && SwitchIndex < vecSwitch.size() && vecSwitch[SwitchIndex].lState != CRegion::CPT_SCRIPT ) //���л��㵫���ǽű�������
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMonsterAI::MoveTo(long lDir)
{
	//##�������
	if( !m_pOwner ) return FALSE;
	//*
	if( m_pOwner -> CanMove() == FALSE )
	{
		AddAIEvent(AET_MOVE,MOVE_FAIL_APACE_TIME);
		return FALSE;
	}

	//*/
	//##�ƶ�����ǰ��׼������ȡ��ǰ�����꣬��Ҫ�ƶ��ķ���
	LONG lPosX		= m_pOwner -> GetTileX();
	LONG lPosY		= m_pOwner -> GetTileY();
	LONG lNewX		= lPosX;
	LONG lNewY		= lPosY;

	float fDis = 1.0f;
	BOOL bMoveable	= GetNextMoveDestCell(lDir,fDis,lNewX,lNewY);
	//##��������
	if( bMoveable )
	{
		//##���濪ʼ��λ����Ϣ
		//m_lLastPositionX = lPosX;
		//m_lLastPositionY = lPosY;
		if( GetLineDir(lPosX, lPosY, lNewX, lNewY) % 2 != 0 )
		{
			fDis *=1.414f;
		}
		float	fTime	= fDis/m_pOwner->GetSpeed();
		if(m_pOwner->GetIsRun())
			fTime /= 2.00f;

		//##�ƶ�Shape��ָ��λ�á�
		OnMove((float)lNewX+0.5f,(float)lNewY+0.5f,(DWORD)fTime);
		//����λ��ֵ
		m_pOwner->SetPosXY(lNewX+0.5f,lNewY+0.5f);		
	}
	else
	{
		//m_pOwner -> OnCannotMove();
		AddAIEvent(AET_MOVE,MOVE_FAIL_APACE_TIME);
	}
	return TRUE;

}

//�ű����������ڴ�����ս�����������˴�������������
DWORD CMonsterAI::CycleSpring()
{	
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner==NULL)return 0;
	return ScriptSpring(pOwner,pOwner->GetCycleScript());
}
DWORD CMonsterAI::BornSpring()
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner==NULL)return 0;
	return ScriptSpring(pOwner,pOwner->GetBornScriptName());
}
DWORD CMonsterAI::FightSpring()
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner==NULL)return 0;
	return ScriptSpring(pOwner,pOwner->GetFightScriptName());
}
DWORD CMonsterAI::GBHSpring(int hurt)//���˴���
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner==NULL)return 0;

	int iHurtProportionHp= pOwner->GetMaxHP()* pOwner->GetHurtProportion()/100;

	int Hp= pOwner->GetHP();
	int LastHp= Hp+ hurt;

	if (iHurtProportionHp<LastHp && iHurtProportionHp>= Hp)
	{
		return ScriptSpring(pOwner,pOwner->GetGBHScriptName());
	}
	return 0;
	
}

DWORD CMonsterAI::DeadSpring()//��������
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner==NULL)return 0;
	return ScriptSpring(pOwner,pOwner->GetDeadScriptName());
}

DWORD CMonsterAI::FindTargetSpring()
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner==NULL)return 0;
	return ScriptSpring(pOwner,pOwner->GetFindTargetScript());
}

DWORD CMonsterAI::EscapeSpring()
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner->GetHP()*100.0f/pOwner->GetMaxHP() <= pOwner->GetEscapePoint())
	{
		m_aesAIState=AES_FIGHT_ESCAPE;
	}
	return 0;
}
DWORD CMonsterAI::ScriptSpring(CMonster* pMonster, const string& str)
{
	if (pMonster== NULL) return 0;
	if (str=="0" || str=="null")return 0;

	stRunScript st;
	st.pszFileName   = (char*)(str.c_str());
	st.srcShape  = pMonster;
	st.desShape	 = pMonster;
	st.pRegion   = dynamic_cast<CRegion*>(m_pOwner->GetFather());
	RunScript(&st,(char*)GetGame()->GetScriptFileData(st.pszFileName));	
	return 0;
}
VOID CMonsterAI::ChangeStateTo(INT state)
{
	if (m_pOwner->GetState()==state)return;

	if (m_pOwner->IsDied())return;

	if (state==CShape::STATE_PEACE)
	{
		if (m_pOwner->GetType()==TYPE_MONSTER)
		{
			m_pOwner->SetSpeed(dynamic_cast<CMonster*>(m_pOwner)->GetMoveSpeed());
			m_pOwner->SendChangedSpeedToClients();
		}

		m_pOwner->ChangeStateTo((CShape::eSTATE)state);
		m_aesAIState=AES_FIGHT_NORMAL;
		CleanCycleSpring();

	}
	else if (state==CShape::STATE_FIGHT)
	{
		if (m_pOwner->GetType()==TYPE_MONSTER &&m_pOwner->GetWeaknessState()==true)
		{
			m_pOwner->SetSpeed(dynamic_cast<CMonster*>(m_pOwner)->GetFightMoveSpeed());
			m_pOwner->SendChangedSpeedToClients();
		}
		if( GetAIType() == CAIFactory::GUARD )
		{
			// �������������������ս����Ϊ������
			m_EnterFightPoing = m_BornPoint;
		}
		else
		{
			m_EnterFightPoing.x=m_pOwner->GetTileX();
			m_EnterFightPoing.y=m_pOwner->GetTileY();
		}
		m_pOwner->ChangeStateTo((CShape::eSTATE)state);
		m_aesAIState=AES_FIGHT_NORMAL;
		CleanCycleSpring();
		FightSpring();
	}
	else if (state==CShape::STATE_DIED || state==CShape::STATE_HANGUP)
	{
		m_pOwner->ChangeStateTo((CShape::eSTATE)state);
		m_aesAIState=AES_FIGHT_NORMAL;
		CleanCycleSpring();
	}
}
//�ص���ƽ״̬
BOOL CMonsterAI::ReturnPeace()
{
	//  [11/11/2009 chenxianj]
	//������λ�¼�
	m_pOwner->ProduceEvent(ET_Homing);

	m_bAttackedByGuard = false;
	m_pOwner->CleanBeneficiary();
	static_cast<CMonster*>( m_pOwner )->CleanHpBar();
	LoseTarget();
	ChangeStateTo(CShape::STATE_PEACE);
	ResumeAll();
	OnBoring(); // �ָ���ƽ״̬ʱ��������������11.21.2008
	FlyToRecordePoint();
	
	// ���²�������
	m_pOwner->AddAttributeToDirty( string( "Hp" ) );
	m_pOwner->UpdateAttribute( true );

	return TRUE;
}

BOOL CMonsterAI::RunByRoad()
{
	if(m_pOwner==NULL)
		return FALSE;
	if(m_vectRoad.size()==0)
		return FALSE;

	if (m_LeaveRoadPoint.x!=m_pOwner->GetTileX() || m_LeaveRoadPoint.y!= m_pOwner->GetTileY())
	{
		CBaseAI::MoveTo(m_LeaveRoadPoint.x,m_LeaveRoadPoint.y);
		return TRUE;
	}

	/*if (m_vectRoad[0].x!=m_pOwner->GetTileX() || m_vectRoad[0].y!=m_pOwner->GetTileY())
	{
		CBaseAI::MoveTo(m_vectRoad[0].x,m_vectRoad[0].y);
		m_LeaveRoadPoint.x		= m_pOwner -> GetTileX();
		m_LeaveRoadPoint.y		= m_pOwner -> GetTileY();
		return TRUE;
	}*/

	if (m_vectRoad.size()==1)
	{
		Stand(STAND_ONCE_TIME);
		return TRUE;
	}

	LONG lX		= m_pOwner -> GetTileX();
	LONG lY		= m_pOwner -> GetTileY();
	INT CurrentPointIndex=m_iRoadRecord;

	INT  iGoStep=0;
	for(int a=0;a<3;a++)
	{
		iGoStep++;
		LONG x=lX;
		LONG y=lY;
		CountNextStepByRoad(lX,lY,CurrentPointIndex);
		BOOL bCanrun=FALSE;

		if (iGoStep==1)
		{
			LONG lDir=GetLineDir(x,y,lX,lY);
			bCanrun=MoveToCell(lDir,x,y);
		}
		else
		{
			bCanrun=MoveToCell(lX,lY);
		}

		if(bCanrun)
		{
			m_iRoadRecord=CurrentPointIndex;

			if(iGoStep==1)
			{
				CBaseAI::MoveTo(lX,lY);
				m_LeaveRoadPoint.x		= m_pOwner -> GetTileX();
				m_LeaveRoadPoint.y		= m_pOwner -> GetTileY();
				return true;
			}
			else
			{
				m_pOwner->SetPosition(lX,lY);
				//����ƶ��Ķ�ʱ��
				AddAIEvent(AET_MOVE, 2000);
				m_LeaveRoadPoint.x		= m_pOwner -> GetTileX();
				m_LeaveRoadPoint.y		= m_pOwner -> GetTileY();
				return true;
			}
		}
	}
	return false;
}


BOOL CMonsterAI::RandomRun()
{
	//�����߶����ַŵ���ƽ���ڴ���

	CMonster* pOwner = dynamic_cast<CMonster*>( GetOwner() );
	if (pOwner== NULL)	return FALSE;
	if( static_cast<DWORD>( random(10000) ) < pOwner->GetMoveRandomValue() )
	{
		LONG lDir	=0;
		int distance=pOwner->Distance(pOwner->GetTileX(),pOwner->GetTileY(),m_BornPoint.x,m_BornPoint.y);
		if (distance> pOwner->GetPeaceMoveRange())
		{
			LONG dir = GetLineDir( pOwner->GetTileX(),pOwner->GetTileY(), m_BornPoint.x, m_BornPoint.y );
			int a=(lDir-dir+8)%8;
			if (a>=2 || a<=6)
			{
				lDir=(dir+random(3)-1+8)%8;
			}
		}
		else
		{
			lDir	= random( 8 );
		}

		INT MaxRunTimes=pOwner->GetMaxRunTimes();
		INT MinRunTimes=pOwner->GetMinRunTimes();

		MoveTo(lDir);
			
		MoveByStepOnPeace(lDir,random(MaxRunTimes-MinRunTimes)+MinRunTimes);
		return true;
	}
	else
	{
		Stand(pOwner->GetStopFrame());
		Stand(random(pOwner->GetMaxStopTimes() - pOwner->GetMinStopTimes())+pOwner->GetMinStopTimes() ,pOwner->GetStopFrame());
	}

	return true;
}

BOOL CMonsterAI::RandomRunOnColony()
{
	return RandomRun();
}
//���״̬����
BOOL CMonsterAI::CheckRandomRunState()
{
	if (m_pOwner->GetRandomMove())	return FALSE;
	LONG dir= random(8);
	MoveTo(dir);
	return TRUE;
}

BOOL CMonsterAI::OnPeaceIdle()
{
	return RandomRun();
}

BOOL CMonsterAI::CheckReturnPeace()
{

	if (m_aesAIState== AES_FIGHT_TRANSITION)
	{
		return FALSE;
	}

	int dis = m_pOwner->Distance(m_EnterFightPoing.x , m_EnterFightPoing.y , m_pOwner->GetTileX(),m_pOwner->GetTileY() );


	if ( dis > dynamic_cast<CMonster*>(m_pOwner)->GetReturnRange())
	{
		if ((HasTarget()==FALSE && timeGetTime()>m_dwGuardTimeStamp)||timeGetTime()> m_dwHurtedTimeStamp)
		{
			if (HasTarget())
			{
				WhenLoseTarget();
			}
			m_aesAIState= AES_FIGHT_TRANSITION;
			m_dwTransitionTimeStamp= timeGetTime();
			return FALSE;
		}
	}
	else
	{
		if (HasTarget()==FALSE && timeGetTime()>m_dwGuardTimeStamp)
		{
			m_aesAIState= AES_FIGHT_TRANSITION;
			m_dwTransitionTimeStamp= timeGetTime();
			return FALSE;
		}
	}

	return FALSE;
}

//��¼������
VOID CMonsterAI::RecordBornPoint(int x, int y)
{
	m_BornPoint.x=x;
	m_BornPoint.y=y;
	m_LeaveRoadPoint.x=x;
	m_LeaveRoadPoint.y=y;
	m_EnterFightPoing.x= x;
	m_EnterFightPoing.y= y;
}
//�ɻص���ƽ��¼�㣬������·����
VOID CMonsterAI::FlyToRecordePoint()
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner==NULL)	return;

	//�ж��Ƿ�Ӧ�÷ɻ�ȥ
	if (pOwner->GetIsFlyToPeace()==0) return;

	if (pOwner->GetIsFlyToPeace()==1)
	{
		POINT point;
		point.x=0;
		point.y =0;
		if (pOwner->HasColony() )//&& pOwner->GetColonyState()!= CMonster::COLONY_STATE_NOLEADER)
		{
			CServerRegion* pRegion= dynamic_cast<CServerRegion*>(pOwner->GetFather());
			CColony* pColony= 0;
			if (pRegion!= NULL)
			{
				pColony= pRegion->GetOneColony(pOwner->GetColonyID());
				if (pColony!= NULL && pColony->HasRoad()== TRUE)
				{
					point= pColony->GetAimPoint(pOwner->GetExID());
					//if (point.x!=0 || point.y!=0)
					//{
					//	m_pOwner->SetPosition(point.x,point.y);
					//	return;
					//}
				}
			}
		}
		if (point.x==0 && point.y==0)
		{		
			if (m_vectRoad.size()==0)
			{
				point= m_EnterFightPoing;//m_pOwner->SetPosition(m_EnterFightPoing.x,m_EnterFightPoing.y);
			}
			else
			{
				point= m_LeaveRoadPoint;//m_pOwner->SetPosition(m_LeaveRoadPoint.x,m_LeaveRoadPoint.y);
			}
		}

		LONG dis= pOwner->Distance(point.x,point.y);
		DWORD time= dis/CGlobeSetup::GetSetup()->fReturnPeaceSpeed;
		if (time> CGlobeSetup::GetSetup()->dwReturnPeaceEffectTime)
		{
			time= CGlobeSetup::GetSetup()->dwReturnPeaceEffectTime;
		}
		pOwner->SendLeaveFightMessage(point.x,point.y,time);
		LevelFight(time,point.x,point.y);
		//�����޵�
	}
	else
	{
		ChangeStateTo(CShape::STATE_DIED);
	}

	//m_bIsLeaveRoad=FALSE;
}
//�ɵ������������Χ
BOOL CMonsterAI::FlyToFriendOnColony(CMoveShape* pFriend)
{
	//= SearchFriendOnColony();
	CMoveShape* pAim=0;
	if (pFriend!= NULL && pFriend->GetAI()!= NULL)
	{
		pAim= dynamic_cast<CMoveShape*>(pFriend->GetAI()->GetTarget());
	}

	if (pAim== NULL)	return FALSE;
	LONG distance= m_pOwner->Distance(pAim);
	LONG frienddis=m_pOwner->Distance(pFriend);
	//if (distance< 6 || frienddis<6)
	//{
	//	CBaseAI::MoveTo(pFriend->GetTileX(),pFriend->GetTileY());
	//	m_dwHurtedTimeStamp=timeGetTime()+dynamic_cast<CMonster*>(m_pOwner)->GetChaseTime();
	//	return FALSE;
	//}
	CMonster *pOwner = static_cast<CMonster*>( m_pOwner );
	DWORD ColDis = pOwner->GetColonyDistance();
	if (distance<ColDis+6 && distance >= ColDis)
	{
		CBaseAI::MoveTo(pAim->GetTileX(),pAim->GetTileY());
		return TRUE;
	}
	else if (frienddis<ColDis+6 && frienddis >= ColDis)
	{
		CBaseAI::MoveTo(pFriend->GetTileX(),pFriend->GetTileY());
		return TRUE;
	}
	else 
	{
		LONG x,y,dir;
		x= pFriend->GetTileX();
		y= pFriend->GetTileY();
		dir= GetLineDir(x,y,pAim->GetTileX(),pAim->GetTileY());

		if (GetNearPosition(x,y,dir,dir))
		{
			m_pOwner->SetPosition(x,y);
			m_pOwner->SetDir(dir);
			m_dwHurtedTimeStamp=timeGetTime()+dynamic_cast<CMonster*>(m_pOwner)->GetChaseTime();
		}
		return FALSE;
	}

	return FALSE;


}
//��ȫ�ָ�������aiҲ��������ǰ��״̬
VOID CMonsterAI::ResumeAll( bool bForce )
{
	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	if ( !bForce && pOwner->GetIsResumeAll() ==0 )
	{
		return ;
	}
	pOwner->ResumeAll();
}
//����ս��
void CMonsterAI::EnterFight(){ChangeStateTo(CShape::STATE_FIGHT);}

//ʹ�ü���
BOOL CMonsterAI::BeginSkill(stModuParam *pSkill, LONG lx, LONG ly,CMoveShape* pTarget)
{

	if (CBaseAI::BeginSkill(pSkill, lx, ly, pTarget))
	{
		return TRUE;
	}
	else
	{
		Stand(dynamic_cast<CMonster*>(m_pOwner)->GetStopFrame());
		return FALSE;
	}
}
	//����������ʱ��(��������ͣ�
BOOL CMonsterAI::WhenBeenHurted(LONG lType,const CGUID& guid, DWORD hurt)
{
	if (m_pOwner->IsGod())	return TRUE;
	m_dwHurtedTimeStamp=timeGetTime()+dynamic_cast<CMonster*>(m_pOwner)->GetChaseTime();
	if(CBaseAI::WhenBeenHurted (lType,guid,hurt))//�����ǿ��Ŀ��ʱ��
	{
		CServerRegion *pRegion = static_cast<CServerRegion*>(m_pOwner->GetFather());
		if (pRegion)
		{
			CMoveShape* pTarget = static_cast<CMoveShape*>(pRegion->FindChildObject(lType, guid));
			// ��־�Ƿ�����������	
			if( !m_bAttackedByGuard && pTarget != NULL && pTarget->GetType() == TYPE_MONSTER )
			{
				CMonster *pMonster = static_cast<CMonster*>( pTarget );
				m_bAttackedByGuard = ( pMonster->GetNpcType() == NT_Guard );
			}
			if( pTarget != NULL && pTarget->IsGod() )
			{
				CMoveShape* pHost = pTarget->GetHost();
				if( pHost != NULL )
				{
					CBaseAI::SetTarget(pHost->GetType(), pHost->GetExID());
				}
			}
			else
			{
				CBaseAI::SetTarget(lType, guid);
			}
		}		
	}
	// ��Ѫ���ű���������
	static_cast<CMonster*>( m_pOwner )->OnHpChanged( m_pOwner->GetHP(), hurt );
	GBHSpring(hurt);
	
	return TRUE;
}

	//��ɱ����ʱ��(ע�����ں������¼�����������
BOOL CMonsterAI::WhenBeenKilled(LONG lType,const CGUID& guid)
{
	CleanCycleSpring();
	m_aesAIState=AES_FIGHT_NOHP;
	DeadSpring();
	LogoutEvent(m_aeActive);
	LogoutEvent(m_aeCycle);
	OnActive();
	LoseTarget();
	CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
	if (pMonster !=NULL && pMonster->GetColonyID()!= 0 && pRegion!= NULL)
	{
		CColony* pColony= pRegion->GetOneColony(pMonster->GetColonyID());
		if (pColony!= NULL)
		{
			pColony->WhenBeenKilled(m_pOwner);
		}
	}

	return FALSE;
}

BOOL CMonsterAI::WhenLoseTarget()
{
	if (CBaseAI::WhenLoseTarget())
	{
		CMonster* pOwner= dynamic_cast<CMonster*>(m_pOwner);
 		m_dwGuardTimeStamp	= timeGetTime() +pOwner->GetGuardTime();
		pOwner->SetForceAttack(TRUE);
		return TRUE;
	}
	return FALSE;
}
BOOL CMonsterAI::WhenBeenHurtedOnColony(LONG lType,const CGUID& guid, DWORD hunrt)
{
	CMonster * pOwner= dynamic_cast<CMonster*>(m_pOwner);
	if (pOwner== NULL || pOwner->HasColony()== FALSE)
	{
		return FALSE;
	}
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
	if (pRegion== NULL) return FALSE;
	CColony* pColony= pRegion->GetOneColony(pOwner->GetColonyID());
	if (pColony!= NULL)
	{
		//pColony->AddTarget(lType,guid);
	}
	return TRUE;
}
//����·��
VOID CMonsterAI::AddRoad(vector<POINT>& vectRoad)
{
  	if (vectRoad.size() > 0 && m_vectRoad.size()==0)
	{
		m_vectRoad=vectRoad;
		m_iRoadRecord=0;				//���ߵ��¼
		m_iRoadDirect=1;              //���߷���
	}

}

	//�������һ��Ҫ�ߵ�λ�ã��������ǽ����¸���¼�λ�����ͣ�����ǡ���
INT  CMonsterAI::CountNextStepByRoad(LONG &lX, LONG &lY,INT & CurrentIndex)
{
	int countresult=0;
	int NextPointIndex=CurrentIndex + m_iRoadDirect;

	if ( NextPointIndex < 0 || NextPointIndex >= m_vectRoad.size())
	{
		//assert(1);
		PutDebugString("·������ԭ�� ����Խ��");
		return NEXTSTEP_RESULT_NULL;
	}
	
	//�����ǰ�����Ѿ�������һ����¼
	if(lX==m_vectRoad[NextPointIndex].x && lY==m_vectRoad[NextPointIndex].y)
	{
		//��������¼������β��
		if(NextPointIndex==m_vectRoad.size()-1 || NextPointIndex==0)		
		{
			//�����·������β������
			if(m_vectRoad[NextPointIndex].x==m_vectRoad[(NextPointIndex+m_iRoadDirect+m_vectRoad.size())% m_vectRoad.size()].x
				&& m_vectRoad[NextPointIndex].y==m_vectRoad[(NextPointIndex+m_iRoadDirect+m_vectRoad.size())% m_vectRoad.size()].y)
			{
				countresult=NEXTSTEP_RESULT_NEXTCYCLE;
				CurrentIndex=(NextPointIndex+m_iRoadDirect+m_vectRoad.size())% m_vectRoad.size();
			}
			//������β����
			else
			{
				countresult=NEXTSTEP_RESULT_RETURN;
				CurrentIndex=NextPointIndex;
				m_iRoadRecord=CurrentIndex;
				m_iRoadDirect=-m_iRoadDirect;
			}

		}
		//������β��
		else																
		{
			countresult=NEXTSTEP_RESULT_NEXT;
			CurrentIndex=NextPointIndex;
		}
	}
	else
	{
		countresult=NEXTSTEP_RESULT_NULL;
	}

	LONG lPosX		= m_vectRoad[CurrentIndex+m_iRoadDirect].x;
	LONG lPosY		= m_vectRoad[CurrentIndex+m_iRoadDirect].y;
	LONG lDir		= GetLineDir(  lX, lY ,lPosX, lPosY);
	m_pOwner -> GetDirPos( lDir, lX, lY );

	return countresult;
}
VOID CMonsterAI::SetTarget( LONG lType, const CGUID& TargetGuid )
{
	if(HasTarget()==FALSE || m_aesAIState== AES_FIGHT_TRANSITION)
	{
		CServerRegion *pRegion = static_cast<CServerRegion*>(m_pOwner->GetFather());
		CMoveShape* pTarget = static_cast<CMoveShape*>(pRegion->FindChildObject(lType, TargetGuid));
		if( pTarget != NULL && pTarget->IsGod() )
		{
			CMoveShape* pHost = pTarget->GetHost();
			if( pHost != NULL )
			{
				CBaseAI::SetTarget(pHost->GetType(), pHost->GetExID());
			}
		}
		else
		{
			CBaseAI::SetTarget(lType, TargetGuid);
		}
	}
}

VOID CMonsterAI::SetForceTarget( LONG lType, const CGUID& TargetGuid )
{
	m_pOwner->SetForceAttack(FALSE);
	SetTarget(lType,TargetGuid);
}
VOID CMonsterAI::CleanForceTarget()
{
	m_pOwner->SetForceAttack(TRUE);
}

BOOL CMonsterAI::CallFriendAround()  //������Χ��ͬ���͹���
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return 0;

	vector<CShape*> vOut;
	pRegion -> FindAroundObject( m_pOwner, TYPE_MONSTER, vOut );

	CMoveShape* pTarget = GetTarget();
	if (pTarget==NULL)
	{
		return FALSE;
	}

	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	INT iFriendNum=0;

	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CMonster* pMonster = dynamic_cast<CMonster*>( vOut[i] );
		if( pMonster == NULL ) 
		{
			continue;
		}
		if (pMonster->GetNpcType()== NT_Guard ||
			pMonster->GetIndex()!=pOwner->GetIndex())
		{
			continue;
		}
		//##���������Ҳ�ھ��䷶Χ��
		if( m_pOwner -> RealDistance(pMonster) <= 
			pOwner -> GetAttribute(string("C_GuardRange")) )
		{
			//����
			
			pMonster->GetAI()->MoveToCellOnPeace(pTarget->GetTileX(),pTarget->GetTileY());
		}
	}
	return TRUE;
}

BOOL CMonsterAI::CallMonsterAround()//������Χ�Ĺ���
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return 0;

	vector<CShape*> vOut;
	pRegion -> FindAroundObject( m_pOwner, TYPE_MONSTER, vOut );

	CMoveShape* pTarget = GetTarget();
	if (pTarget==NULL)
	{
		return FALSE;
	}

	CMonster* pOwner=dynamic_cast<CMonster*>(m_pOwner);
	INT iFriendNum=0;

	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		CMonster* pMonster = dynamic_cast<CMonster*>( vOut[i] );
		if( pMonster == NULL ) 
		{
			continue;
		}
		if (pMonster->GetNpcType()== NT_Guard )
		{
			continue;
		}
		//##���������Ҳ�ھ��䷶Χ��
		if( m_pOwner -> RealDistance(pMonster) <= 
			m_pOwner -> GetAlertRange() )
		{
			//����
			pMonster->GetAI()->MoveToCellOnPeace(pTarget->GetTileX(),pTarget->GetTileY());
		}
	}
	return TRUE;
}



BOOL CMonsterAI::AddChangeAIEvent(LONG AIType)
{
	if (m_pOwner==NULL)
	{
		return FALSE;
	}
	CMonster* pOwner=(CMonster*)m_pOwner;
	if(pOwner->GetAIType()==AIType)
	{
		return TRUE;
	}
	pOwner->SetAIType(AIType);
	AddAIEvent(AET_CHANGEAI,100);
	return TRUE;
}

VOID CMonsterAI::RecordChoiceSkill(DWORD SkillID,DWORD SkillLevel,WORD SkillType,WORD SkillIndex)
{
	m_dwChoiceSkillID= SkillID;
	m_dwChoiceSkillLevel= SkillLevel;
	m_wSkillType= SkillType;
	m_wSkillIndex= SkillIndex;
	m_bMustUsed= SkillType==SKILLTYPE_CYCLE;
	m_bBeenUsed= FALSE;
}

VOID CMonsterAI::RecordUsedCurrentSkill()
{
	m_bBeenUsed= TRUE;

	vector<CMonster::tagSkillRecord>* skillrecordlist=0;
	if (m_wSkillType== SKILLTYPE_BASE) skillrecordlist= & (dynamic_cast<CMonster*>(m_pOwner)->GetBaseSkillList());
	else if (m_wSkillType== SKILLTYPE_NORMAL) skillrecordlist= &(dynamic_cast<CMonster*>(m_pOwner)->GetSkillList());
	else if (m_wSkillType== SKILLTYPE_CYCLE) skillrecordlist= &(dynamic_cast<CMonster*>(m_pOwner)->GetCycleSkillList());
	else return;

	if (skillrecordlist->size() > m_wSkillIndex)
	{
		(*skillrecordlist)[m_wSkillIndex].dwCoolDownTime= timeGetTime()+(*skillrecordlist)[m_wSkillIndex].dwCool;
	}

}

VOID CMonsterAI::CleanChoiceSkillRecord()
{
	m_dwChoiceSkillID= 0;
	m_dwChoiceSkillLevel= 0;
	m_wSkillType= 0;
	m_wSkillIndex= 0;
	m_bMustUsed= FALSE;
	m_bBeenUsed= TRUE;
}

BOOL CMonsterAI::IsInKindlyTime()
{
	// �ճ���ʱ����������Ϊ���Ѻ�״̬
	if( m_dwRebornTime == 0 )
	{
		return FALSE;
	}

	return m_dwRebornTime + CGlobeSetup::GetSetup()->dwMonsterKindlyTime >= timeGetTime();	
}

bool CMonsterAI::UseSkillSyncSafe( long skillType, long skillID, long skillLevel, const CGUID &targetID )
{
	CMonster *pMonster = static_cast<CMonster*>( m_pOwner );
	CMoveShape *pTarget = NULL;
	if( targetID != NULL_GUID )
	{
		CServerRegion *pRegion = static_cast<CServerRegion*>( pMonster->GetFather() );
		pTarget = static_cast<CMoveShape*>( pRegion->FindChildObject( TYPE_MONSTER, targetID ));
		if( pTarget == NULL )
		{
			pTarget = static_cast<CMoveShape*>( pRegion->FindChildObject( TYPE_PLAYER,targetID ) );
		}
	}
	else
	{
		pTarget = GetTarget();
	}
	
	if( pTarget == NULL )
	{
		Stand( static_cast<CMonster*>( m_pOwner )->GetStopFrame() );	
		return false;
	}
	else if( !UseSkillSync( skillType, skillID, skillLevel, pTarget->GetTileX(), pTarget->GetTileY(), pTarget ) )
	{
		Stand( static_cast<CMonster*>( m_pOwner )->GetStopFrame() );
		return false;
	}

	return true;
}

bool CMonsterAI::UseSkillOnCellSyncSafe( long skillType, long skillID, long skillLevel, long targetX, long targetY )
{
	if( targetX == 0 && targetY == 0 )
	{
		CMoveShape *pTarget = GetTarget();
		if( pTarget == NULL )
		{
			Stand( static_cast<CMonster*>( m_pOwner )->GetStopFrame() );
			return false;
		}

		targetX = pTarget->GetTileX();
		targetY = pTarget->GetTileY();
	}

	if( !UseSkillSync( skillType, skillID, skillLevel, targetX, targetY ) )
	{
		Stand( static_cast<CMonster*>( m_pOwner )->GetStopFrame() );
		return false;
	}

	return true;
}

bool CMonsterAI::UseSkillSync( long skillType, long skillID, long skillLevel, long targetX, long targetY, CMoveShape *pTarget )
{
	if( m_pOwner == NULL )
	{
		return false;
	}

	CMonster *pMonster = static_cast<CMonster*>( m_pOwner );
	vector<CMonster::tagSkillRecord> *skillRecordList = NULL;
	if( skillType == SKILLTYPE_BASE )
	{
		skillRecordList = &pMonster->GetBaseSkillList();
	}
	else if( skillType == SKILLTYPE_NORMAL )
	{
		skillRecordList = &pMonster->GetSkillList();
	}
	else if( skillType == SKILLTYPE_CYCLE )
	{
		skillRecordList = &pMonster->GetCycleSkillList();
	}

	if( skillRecordList == NULL )
	{
		return false;
	}

	// find the record by its id.
	CMonster::tagSkillRecord *skillRecord = NULL;
	for( vector<CMonster::tagSkillRecord>::iterator it = skillRecordList->begin(); it != skillRecordList->end(); ++ it )
	{
		if( it->wID == skillID )
		{
			skillRecord = &(*it);

			break;
		}
	}

	if( skillRecord == NULL )
	{
		// not found
		return false;
	}

	DWORD this_time = timeGetTime();
	// someone told me do NOT consider the cool time for monsters - -|
	if( 0 && skillRecord->dwCoolDownTime > this_time )
	{
		// cd is not ready
		return false;
	}

	// ready to fire....
	skillRecord->dwCoolDownTime = this_time + skillRecord->dwCool ;

	pMonster->InitSkillInstance( skillID, skillLevel ,0,0,0,NULL_GUID,6);
	return BeginSkill( pMonster->GetInstanceSkill(), targetX, targetY, pTarget ) == TRUE;
}
