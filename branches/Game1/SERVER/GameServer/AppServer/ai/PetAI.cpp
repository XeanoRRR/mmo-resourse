#include "StdAfx.h"
#include "PetAI.h"
#include "../Pet.h"
#include "../Player.h"
#include "../ServerRegion.h"
#include "../../GameServer/GameManager.h"



#ifdef _DEBUG
extern long g_lPetAICount;
#endif

CPetAI::CPetAI()
: m_bFightUseSkill(FALSE)
, m_lCount(0)
, m_lReturnPeaceTime(0)
{
}

CPetAI::~CPetAI()
{
	LogoutEvent(m_aeRetPeace);
}

// ��һ��ָ�������ƶ�
BOOL CPetAI::MoveTo(float fDestX, float fDestY)
{
	// �������
	if( FALSE == m_pOwner->CanMove() ||
		TRUE == m_pOwner->IsInAct() )
	{
		PutoutLog("PetAI", LT_ERROR,
			"����<%s : %s>��ͼ�ƶ�.",
			m_pHost->GetName(),
			m_pOwner->GetName());
		m_pOwner -> OnCannotMove();
		return FALSE;
	}

	CPet* pPet = (CPet*)m_pOwner;
	if( PET_AS_ACTIVE == pPet->GetFightType() && NULL == GetTarget() )
	{
		AddAIEvent(AET_MOVE, SKILL_FAIL_SPACE_TIME);
	}

	m_pOwner->OnMove(fDestX, fDestY);

	return TRUE;
}

// ������״̬�ȴ���
DWORD CPetAI::BornSpring(void)
{
	return 0;
}

// �״̬AI����
BOOL CPetAI::OnActive(void)
{
	if( NULL == m_pOwner ||
		NULL == m_pHost ||
		!((CPet*)m_pOwner)->IsActive() )
	{
		PutoutLog("PetAI", LT_ERROR,
			"����AI���� || ��������ΪNULL.");
		m_pOwner -> OnCannotMove();
		return FALSE;
	}

	WORD wState = m_pOwner->GetState();

	switch( wState )
	{
	case CShape::STATE_PEACE:		// ��ƽ״̬
	case CShape::STATE_FIGHT:		// ս��״̬
		{
			if( PET_AS_ACTIVE == dynamic_cast<CPet*>(m_pOwner)->GetFightType() )
			{
				SearchEnemy();
			}
		}
		break;

	case CShape::STATE_DIED:		// ����״̬
		{
			ChangeStateTo(CShape::STATE_DIED);
			StartDiedEvent(CGlobeSetup::GetSetup()->dwMonsterKeepDeadTime);
		}
		break;

	default:
		break;
	}

	return TRUE;
}

BOOL CPetAI::OnDeadState()
{
	((CPet*)m_pOwner)->DelFromRegion(PET_OT_DISAPPEAR);
	return TRUE;
}

// ���ܽ�������
BOOL CPetAI::OnSkillEnd(long lAddSub)
{
	m_lReturnPeaceTime = timeGetTime() + CGlobeSetup::GetSetup()->dwFightStateTime;
	m_bFightUseSkill = FALSE;
	LogoutRetPeaceAct();
	AddRetPeaceAct();

	if ( eAdd_Type == lAddSub &&
		CShape::STATE_PEACE == m_pOwner->GetState() )
	{	
		m_pOwner->ChangeStateTo(CShape::STATE_PEACE);
	}

	CMoveShape* pObj = GetTarget();
	if( NULL == pObj ||
		pObj->IsDied() )
	{
		AddAIEvent(AET_MOVE);
	}

	return TRUE;
}

// ע�����غ�ƽ�¼�
void CPetAI::LogoutRetPeaceAct()
{
	GameManager::GetInstance()->Cancel(m_aeRetPeace.lTimerID,0);
	m_aeRetPeace.Clean();
}

// �ظ���ƽ״̬
void CPetAI::AddRetPeaceAct(void)
{
	if( -1 == m_aeRetPeace.lTimerID )
	{
		m_aeRetPeace.aetEventType = AET_RETURNPEACE;
		tagTimerVar* pTimvar = CBaseObject::CreateTimerVar(18);
		pTimvar->TimerType = ChildObjType_AI;
		pTimvar->lvar = AET_RETURNPEACE;
		m_aeRetPeace.dwSpringTime = m_lReturnPeaceTime;
		m_aeRetPeace.lTimerID = GameManager::GetInstance()->Schedule(m_pOwner,
			(void*)pTimvar,
			m_lReturnPeaceTime);
	}
}

BOOL CPetAI::OnReturnPeace(void)
{
	m_aeReturnPeace.Clean();
	if (!m_pOwner->IsDied() && !((CPet*)m_pOwner)->IsHangUp())
	{
		ChangeStateTo(CShape::STATE_PEACE);
	}

	return TRUE;
}

//Ŀ�꣺ ���ã�ѯ�ʣ���ȡ���ͣ���ȡid����ȡָ��
VOID CPetAI::SetTarget( LONG lType, const CGUID& TargetGuid )
{
	if( m_pOwner != NULL &&
		m_pHost != NULL )
	{
		m_lTargetType	= lType;
		m_TargetGuid	= TargetGuid;

		CPet* pPet = (CPet*)m_pOwner;
		if( pPet->GetPetType() != PET_TYPE_TRAP )
		{
			GetInst(CPetCtrl).InformClientAI((CPet*)m_pOwner, PET_OT_AI_SET_TARGET);
		}
	}
}

VOID CPetAI::Attack(long lSkillID,
					long lX,
					long lY,
					CMoveShape* pTarget)
{
	char szLog[512];
	if( m_pHost == NULL )
	{
		_snprintf(szLog, 512, 
			"����%s���� == NULL.",
			m_pOwner->GetName());
		PutoutLog("Pet", LT_DEFAULT, szLog);
		return;
	}

	if ( 0/*AET_MOVE == m_aeActive.aetEventType*/ )
	{
#ifdef _DEBUG
		_snprintf(szLog, 512, 
			"���%s����ʹ�ü���%ldʧ��.",
			m_pHost->GetName(),
			lSkillID);
		PutoutLog("Pet", LT_DEFAULT, szLog);
#endif

		CMessage msg(MSG_S2C_SKILL_USE_PET);
		msg.Add((CHAR)SKILL_USE_RESULT_FAILED);
		msg.Add(m_pOwner->GetExID());
		msg.Add(lSkillID);
		msg.Add((BYTE)1);
		msg.Add((DWORD)0);
		msg.Add((long)SKILL_USE_FAILED_INVALID_UNKNOWN);
		msg.SendToPlayer(m_pHost->GetExID());
		return;
	}

	stModuParam *pCurrentSkill = m_pOwner->GetInstanceSkill();
	if( pCurrentSkill != NULL )
	{
		eAddSub eSkillType = pCurrentSkill->GetAddSub();
		if( BeginSkill(pCurrentSkill,lX,lY,pTarget) &&
			eSkillType == eSub_Type )
		{
			if( pTarget != NULL && pTarget != GetTarget() )
			{
				SetTarget(pTarget->GetType(), pTarget->GetExID());
			}
		}
	}
}

bool CPetAI::SearchEnemy(void)
{
	CMoveShape* pHost = ((CPet*)m_pOwner)->GetHost();
	if( NULL == pHost )
		return false;
	CMoveShape* pTarget = NULL;

	// ��ǰ��׷��Ŀ��
	pTarget = GetTarget();
	if( pTarget != NULL )
	{
		long lDis = m_pOwner->Distance(pTarget);
		long lTracDis = m_pOwner->GetTrackRange();
		if( pTarget != NULL &&
			!pTarget->IsDied() &&
			lTracDis>lDis &&
			!pTarget->IsGod() )
		{
			return true;
		}
	}

	// Ѱ�����ڳ���
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(pHost->GetFather());
	if( NULL == pRegion )
		return false;

	vector<CShape*> vOut;
	pRegion->FindAroundObject(m_pOwner, TYPE_PLAYER, vOut);
	pRegion->FindAroundObject(m_pOwner, TYPE_MONSTER, vOut);
	pRegion->FindAroundObject(m_pOwner, TYPE_PET, vOut);

	bool bRet = false;
	map<long, CMoveShape*> AroundObjs;
	long lDistance = 0;
	for( size_t i=0; i<vOut.size(); ++i )
	{
		pTarget = (CMoveShape*)vOut[i];
		if( pTarget != NULL &&
			!pTarget->IsDied() &&
			!pTarget->IsGod() &&
			pTarget->IsAttackAble(m_pOwner) )
		{
			lDistance = m_pOwner->Distance(pTarget->GetTileX(), pTarget->GetTileY());
			AroundObjs[lDistance] = pTarget;
			bRet = true;
		}
	}

	if( bRet )		// �����������ö���
	{
		map<long, CMoveShape*>::iterator itr = AroundObjs.begin();
		pTarget = itr->second;
		SetTarget(pTarget->GetType(), pTarget->GetExID());
		CPet* pPet = (CPet*)m_pOwner;
		if( PET_TYPE_TRAP == pPet->GetPetType() )
		{
			m_pOwner->SetActTime(0, 0);		// ����Ѱ�к��޷�����
			pPet->OnTrapSpring();
		}
	}

	return bRet;
}

bool CPetAI::MissTarget(void)
{
	m_TargetGuid			= NULL_GUID;
	m_lTargetType			= 0;
	m_lTargetX				= 0;
	m_lTargetY				= 0;

	return true;
}

void CPetAI::ChangeStateTo(INT lState)
{
	if( lState == m_pOwner->GetState() ||
		m_pOwner->IsDied() )
	{
		return;
	}

	if( CShape::STATE_DIED == lState ||
		CShape::STATE_HANGUP == lState )
	{
		m_pOwner->ChangeStateTo((CShape::eSTATE)lState);
		m_aesAIState = AES_FIGHT_NORMAL;
		CleanCycleSpring();
	}

	else
	{
		CBaseAI::ChangeStateTo(lState);
	}
}

//����������ʱ��
BOOL CPetAI::WhenBeenHurted(LONG lType,const CGUID& guid ,DWORD a)
{
	if (m_pOwner->IsGod()) return TRUE;
	m_pOwner->ChangeStateTo(CShape::STATE_FIGHT);
	return TRUE;
}

// ���ﱻɱ��
BOOL CPetAI::WhenBeenKilled(LONG lType,const CGUID& guid)
{
	OnActive();
	m_pOwner->OnDied();
	return TRUE;
}