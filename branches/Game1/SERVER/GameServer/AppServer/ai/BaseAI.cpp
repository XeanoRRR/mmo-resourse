 #include "StdAfx.h"
#include "BaseAI.h"
#include "..\Monster.h"
#include "..\MoveShape.h"
#include "../../GameServer/GameManager.h"
#include "AIFactory.h"
#include <MMSystem.h>
#include "..\ServerRegion.h"
#include "../BuffSkill/BaseModule.h"
#include "../BuffSkill/NewSkillFactory.h"
#include "..\Player.h"
#include "ScriptAI.h"




//--------------------------------------------------------------------------------------
// ����:	CBaseAI���캯��
// ����:	��
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------

CBaseAI::AI_EVENT ::AI_EVENT()
{
	aetEventType=AET_NULL;      //��ʱ�¼�����
	lTimerID=-1;				//ʱ��id
	dwSpringTime=0;				//����ʱ��
}

VOID CBaseAI::AI_EVENT ::Clean()
{
	aetEventType=AET_NULL;      //��ʱ�¼�����
	lTimerID=-1;				//ʱ��id
	dwSpringTime=0;				//����ʱ��
}
VOID CBaseAI::AI_EVENT ::operator=(CBaseAI::AI_EVENT& ae)
{
	aetEventType=ae.aetEventType;	    //��ʱ�¼�����
	lTimerID=ae.lTimerID;				//ʱ��id
	dwSpringTime=ae.dwSpringTime;		//����ʱ��
}
CBaseAI::SCycleSpringEvent::SCycleSpringEvent()
{
	csSpringType=CS_NULL;
	unActType.tgMoveToCell.lX=0;
	unActType.tgMoveToCell.lY=0;
}
VOID CBaseAI::SCycleSpringEvent::Clean()
{
	csSpringType=CS_NULL;
	memset(&unActType,0,sizeof(unActType));
	gAimGUID= NULL_GUID;
}
CBaseAI::CBaseAI()
{
	m_lAIType=0;					//##AI���ͱ��
	m_dwLastExecTimeStamp=0;		//##AI���һ��ִ�е�ʱ��

	//m_TargetGuid=0;				//##��������ID
	m_lTargetType=0;				//##������������

	m_lTargetX=0;					//##Ŀ��X
	m_lTargetY=0;					//##Ŀ��Y

	m_pOwner=NULL;					//##AI���� ���� OR ���

	m_dwDormancyTime=0;			// ���߿�ʼʱ��
	m_dwDormancyInterval=0;		// ����ʱ����


	m_aesAIState=AES_FIGHT_NORMAL;               //AI״̬

}

//CBaseAI::CBaseAI(CBaseAI& AI)
//{
//	//m_lAIType=0;					//##AI���ͱ��
//	m_dwLastExecTimeStamp=0;		//##AI���һ��ִ�е�ʱ��
//
//	m_TargetGuid=AI.m_TargetGuid;				//##��������ID
//	m_lTargetType=AI.m_lTargetType;				//##������������
//
//	m_lTargetX=AI.m_lTargetX;					//##Ŀ��X
//	m_lTargetY=AI.m_lTargetY;					//##Ŀ��Y
//	m_pOwner=AI.m_pOwner;					//##AI���� ���� OR ���
//
//	m_dwDormancyTime=0;			// ���߿�ʼʱ��
//	m_dwDormancyInterval=0;		// ����ʱ����
//
//	m_aesAIState=AES_FIGHT_NORMAL;               //AI״̬
//}
//VOID CBaseAI::operator=(CBaseAI& AI)
//{
//
//	m_dwLastExecTimeStamp=AI.m_dwLastExecTimeStamp;		//##AI���һ��ִ�е�ʱ��
//
//	m_TargetGuid=AI.m_TargetGuid;				//##��������ID
//	m_lTargetType=AI.m_lTargetType;				//##������������
//
//	m_lTargetX=AI.m_lTargetX;					//##Ŀ��X
//	m_lTargetY=AI.m_lTargetY;					//##Ŀ��Y
//
//	m_pOwner=AI.m_pOwner;					//##AI���� ���� OR ���
//
//
//	//BOOL						m_bIsDormancy;				//##�Ƿ�������״̬��
//	// ����ʱ�䣬�´�������ʱ��ͨ����ǰʱ���ȥ����ʱ�䣬�Ϳ�����������˶೤ʱ�䡣
//	// ͨ�����ʱ�䣬���ϻ�Ѫ�������Ϳ�����������ʱ��ظ�HP�ˡ�
//	m_dwDormancyTime=AI.m_dwDormancyTime;			// ���߿�ʼʱ��
//	m_dwDormancyInterval=AI.m_dwDormancyInterval;		// ����ʱ����
//
////	m_fGBHStamp=AI.m_fGBHStamp;				//���˱�ʶ
////	m_dwGuardTimeStamp=AI.m_dwGuardTimeStamp;			//����ʱ���ʶ
////	m_dwHurtedTimeStamp=AI.m_dwHurtedTimeStamp;
////	m_dwSkillSpaceTimeStamp=AI.m_dwSkillSpaceTimeStamp;	//���ܼ��ʱ���־
//	m_aeActive=AI.m_aeActive;					//������Ϊ��¼
//	 m_aeCycle=AI.m_aeCycle;					//������Ϊ��¼
//	m_aesAIState=AI.m_aesAIState;               //AI״̬
////	m_csCycleSpring=AI.m_csCycleSpring;			//���ڴ����¼� 
//	m_strIdleActScript=AI.m_strIdleActScript;			//������Ϊ�ű�
//	//m_pCurrentSkill=AI.m_pCurrentSkill;			//��ǰ����
//	//·�����
//}

CBaseAI::~CBaseAI()
{
	Reset();
}

//�¼������ĵ��ýӿ�
BOOL CBaseAI::Run(DWORD dwTimerID,int iOutTimeType)
{
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[256]="";
	_snprintf(pszStatckInfo,256,"CBaseAI::Run(TimerID:%d,OutTimeType:%d) Start",dwTimerID,iOutTimeType);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	bool bRet = true;
	if(iOutTimeType==AET_CYCLE)
	{
		//DeleteAIEvent((AI_EVENT_TYPE)iOutTimeType);
		OnCycle();
	}
	else if(iOutTimeType==AET_ACT || iOutTimeType==AET_MOVE )
	{
		DeleteAIEvent((AI_EVENT_TYPE)iOutTimeType);
		OnActive();
	}
	else if (iOutTimeType==AET_DIED)
	{
		DeleteAIEvent((AI_EVENT_TYPE)iOutTimeType);
		OnDeadState();
	}
	//else if (iOutTimeType==AET_RELIVE)
	//{
	//	DeleteAIEvent((AI_EVENT_TYPE)iOutTimeType);
	//	OnReLive();
	//}
	else if (iOutTimeType==AET_RETURNPEACE)
	{
		OnReturnPeace();
	}
	else if(iOutTimeType==AET_CHANGEAI)
	{
		OnChangeAI();
	}
	else if( iOutTimeType == AET_SKILL_CONTINUE && CAIFactory::IsScriptAI( GetAIType() ) )
	{
		CScriptAI *pScriptAI = static_cast<CScriptAI*>( this );
		pScriptAI->SkillContinue();
	}
		bRet = false;

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CBaseAI::Run() End");
#endif
	return bRet;

}
//���һ��ai�¼�,
BOOL CBaseAI::AddAIEvent(AI_EVENT_TYPE iOutTimeType,DWORD dwOutTime )
{
	if( iOutTimeType== AET_CYCLE)
	{
		if(m_aeCycle.aetEventType==AET_NULL)
		{
			m_aeCycle.aetEventType=AET_CYCLE;
			//���������ע����Ϣ��ע��һ���¼�
			tagTimerVar* pTimvar = CBaseObject::CreateTimerVar(7);
			pTimvar->TimerType = ChildObjType_AI;
			pTimvar->lvar = iOutTimeType;
			m_aeCycle.lTimerID=GameManager::GetInstance()->Schedule(m_pOwner,(void*)pTimvar,timeGetTime()+dwOutTime,dwOutTime);
			m_aeCycle.dwSpringTime=dwOutTime;
		}
		else 
			return false;
	}
	else if(iOutTimeType== AET_MOVE  )
	{
		if(m_aeActive.aetEventType==AET_NULL)
		{
			m_pOwner->SetActTime(timeGetTime(),dwOutTime);
			m_aeActive.aetEventType=(AI_EVENT_TYPE)iOutTimeType;
			//���������ע����Ϣ��ע��һ���¼�
			tagTimerVar* pTimvar = CBaseObject::CreateTimerVar(8);
			pTimvar->TimerType = ChildObjType_AI;
			pTimvar->lvar = iOutTimeType;
			m_aeActive.lTimerID=GameManager::GetInstance()->Schedule(m_pOwner,(void*)pTimvar,timeGetTime()+dwOutTime,0);
			m_aeActive.dwSpringTime=dwOutTime;
		}
		else 
			return false;
	}
	else if( iOutTimeType==AET_DIED )//|| iOutTimeType== AET_RELIVE 
	{
		if(m_aeActive.aetEventType==AET_NULL)
		{
			m_aeActive.aetEventType=(AI_EVENT_TYPE)iOutTimeType;
			//���������ע����Ϣ��ע��һ���¼�
			tagTimerVar* pTimvar = CBaseObject::CreateTimerVar(9);
			pTimvar->TimerType = ChildObjType_AI;
			pTimvar->lvar = iOutTimeType;
			m_aeActive.lTimerID=GameManager::GetInstance()->Schedule(m_pOwner,(void*)pTimvar,timeGetTime()+dwOutTime,0);
			m_aeActive.dwSpringTime=dwOutTime;
		}
		else 
			return false;
	}
	else if (iOutTimeType== AET_ACT)
	{
		if (m_aeActive.aetEventType==AET_NULL)
		{
			m_aeActive.aetEventType=AET_ACT;
		}
		else
			return false;
	}
	else if( iOutTimeType== AET_CHANGEAI )
	{
		//���������ע����Ϣ��ע��һ���¼�
		tagTimerVar* pTimvar = CBaseObject::CreateTimerVar(10);
		pTimvar->TimerType = ChildObjType_AI;
		pTimvar->lvar = iOutTimeType;
		GameManager::GetInstance()->Schedule(m_pOwner,(void*)pTimvar,timeGetTime()+dwOutTime,0);
	}
	else if( iOutTimeType == AET_SKILL_CONTINUE )
	{
		tagTimerVar *pTimvar = CBaseObject::CreateTimerVar(11);
		pTimvar->TimerType = ChildObjType_AI;
		pTimvar->lvar = iOutTimeType;
		m_aeScriptAISkill.aetEventType = (AI_EVENT_TYPE)iOutTimeType;
		m_aeScriptAISkill.lTimerID = GameManager::GetInstance()->Schedule( m_pOwner, pTimvar, timeGetTime() + dwOutTime, 0 );
	}
	else 
		return false;
	return true;

}

//ɾ��һ��AI�¼�
BOOL CBaseAI::DeleteAIEvent(AI_EVENT_TYPE iOutTimeType)
{
	if(iOutTimeType== AET_CYCLE)
	{
		m_aeCycle.Clean();
	}
	else if( iOutTimeType== AET_ACT || iOutTimeType ==AET_MOVE || iOutTimeType== AET_DIED)//|| iOutTimeType== AET_RELIVE 
	{
		
	m_aeActive.Clean();
	}
	else
		return false;
	return true;;
}

//�ڶ�ʱ����ע��һ���¼�
BOOL CBaseAI::LogoutEvent(AI_EVENT& aeAiEvent)
{
	if(aeAiEvent.aetEventType==AET_CYCLE)
	{
		//ע���¼�
		GameManager::GetInstance()->Cancel(aeAiEvent.lTimerID,0);
		m_aeCycle.Clean();
		return true;
	}

	else if(aeAiEvent.aetEventType==AET_MOVE  || aeAiEvent.aetEventType ==AET_RETURNPEACE || aeAiEvent.aetEventType==AET_ACT)//|| aeAiEvent.aetEventType == AET_RELIVE
	{
		//ע���¼�
		GameManager::GetInstance()->Cancel(aeAiEvent.lTimerID,0);
		m_aeActive.Clean();
		return true;
	}
	else if( aeAiEvent.aetEventType == AET_SKILL_CONTINUE )
	{
		GameManager::GetInstance()->Cancel( aeAiEvent.lTimerID, 0 );
		m_aeScriptAISkill.Clean();
	}
	//else if (aeAiEvent.aetEventType==AET_ACT)
	//{
	//	//�������
	//	//EndCurrentSkill();
	//	m_aeActive.Clean();
	//}
	else
		return false;
	return TRUE;
}

VOID CBaseAI::OnBoring()
{
	StartActiveAct();
	BornSpring();
}


//���ڵ��ú���
BOOL CBaseAI::OnCycle()
{

	return TRUE;
}

BOOL CBaseAI::OnChangeAI ()
{
	return TRUE;
}


//������Ϊ��Ӧ
BOOL CBaseAI::OnActive()
{
	return FALSE;
}

BOOL CBaseAI::OnReturnPeace()
{
	return ReturnPeace();
}

//������Ϊ����Ĳ�ͬ״̬����Ϊ  ��ƽ��ս�������ܣ�����������
BOOL CBaseAI::OnPeaceState()
{
	return true;
}
BOOL CBaseAI::OnFightState()
{
	return true;
}
BOOL CBaseAI::OnEscapeState()
{
	return true;
}
BOOL CBaseAI::OnNoHpState()
{
	return true;
}
BOOL CBaseAI::OnDeadState()
{
	return true;
}

BOOL CBaseAI::OnTransitionState()
{
	return true;
}

BOOL CBaseAI::OnPeaceIdle()
{
	return true;
}

//�����£�Ѱ�ҵ���
//BOOL CBaseAI::SearchEnemy()
//{
//	 if(m_pOwner && m_pOwner->GetForceAttack())
//	 {
//		 if (dynamic_cast<CMonster*>(m_pOwner)->GetSearchEnemyScript()!="0"
//			 && dynamic_cast<CMonster*>(m_pOwner)->GetSearchEnemyScript()!="null")
//		 {
//			 SearchEnemyByScript();
//			 return FALSE;
//		 }
//		 return TRUE;
//	 }
//	 return FALSE;
//}

//ͨ���ű�Ѱ�ҵ���
//BOOL CBaseAI::SearchEnemyByScript()
//{
//	return FALSE;
//}

//Ѱ�Ҽ���
BOOL CBaseAI::SearchSkill(CMoveShape* pTarget)
{
	return m_pOwner->IsFightable();

}


//������ս�����������˴�������������
DWORD CBaseAI::CycleSpring()
{
	return 0;
}
DWORD CBaseAI::BornSpring()
{
	return 0;
}
DWORD CBaseAI::FightSpring()
{
	return 0;
}
DWORD CBaseAI::GBHSpring(int hurt)
{
	return 0;
}
DWORD CBaseAI::DeadSpring()
{
	return 0;
}
DWORD CBaseAI::FindTargetSpring()
{
	return 0;
}
DWORD CBaseAI::EscapeSpring()
{
	return 0;
}

//�õ���һ���ƶ���Ŀ���
//lMaxDistance:һ���ƶ�����Զ����
BOOL CBaseAI::GetNextMoveDestCell(long lDestX,long lDestY,long lMaxDistance,long &lX,long &lY)
{
	long lTileX = m_pOwner->GetTileX();
	long lTileY = m_pOwner->GetTileY();
	long lDir	= GetLineDir(lTileX,lTileY,lDestX,lDestY);	
	long lDistance = m_pOwner->Distance(lDestX,lDestY);
	lMaxDistance = min(lMaxDistance,lDistance);
	return GetNextMoveDestCell(lDir,lMaxDistance,lX,lY);
}

//�õ���һ���ƶ���Ŀ���
//lMaxDistance:һ���ƶ�����Զ����
BOOL CBaseAI::GetNextMoveDestCell(long lDir,long lMaxDistance,long &lX,long &lY)
	{
	if( m_pOwner == NULL ) return FALSE;
	CRegion* pRegion = dynamic_cast<CRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) return FALSE;

	long lCurX = m_pOwner->GetTileX();
	long lCurY = m_pOwner->GetTileY();
	long lDestX = lCurX;
	long lDestY = lCurY;

	long i = 0;
	for(; i < 8; i++ )
	{
		bool bMove		= true;
		long lSlipDir	= _slip_order[lDir][i];

		//##���ݲ�ͬ���ε�shape��������һ���Ƿ�������ߣ�3-10-2009��Ѱ·��������
		for( list<POINT>::iterator it = s_listMoveCheckCell[0][lSlipDir].begin();
			it != s_listMoveCheckCell[0][lSlipDir].end(); it++ )
		{
			long lCellX = lCurX + it -> x;
			long lCellY = lCurY + it -> y;

			if( !IsCanMoveTo(pRegion,lCellX,lCellY) )
			{
				bMove = false;
				break;
			}
		}
		if( bMove )
		{
			lDestX = lDestX + _dir[lSlipDir][0];
			lDestY = lDestY + _dir[lSlipDir][1];
			break;
		}
	}
	if(i == 9)	return FALSE;

	//�ڴ˷�����������������
	lDir = GetLineDir(m_pOwner->GetTileX(), m_pOwner->GetTileY(), lDestX,lDestY);
	i = lMaxDistance-1;
	while(i)
	{
		long lNextX = lDestX + _dir[lDir][0];
		long lNextY = lDestY + _dir[lDir][1];
		if(  !IsCanMoveTo(pRegion,lNextX,lNextY) )
		{
			break;
		}

		lDestX = lNextX;
		lDestY = lNextY;
		i--;
	}
	lX = lDestX;
	lY = lDestY;
	return TRUE;
}

	//�жϸø����Ƿ����ƶ�
BOOL CBaseAI::IsCanMoveTo(CRegion* pRegion,LONG lX, LONG lY)
{
	if(NULL == pRegion)	return FALSE;

	if (pRegion->GetBlock(lX,lY) != CRegion::BLOCK_NO)
	{
		return FALSE;
	}
	return TRUE;
}

//�ƶ����ض����ӵļ��
BOOL CBaseAI::MoveToCell(LONG lX, LONG lY)
{
	CRegion* pRegion = dynamic_cast<CRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) 
	{
		return FALSE;
	}

	int figure=m_pOwner->GetFigure();
	// 3-10-2009,���ڹ���ƶ���������
	if( m_pOwner->GetType() == TYPE_MONSTER )
	{
		figure = 0;
	}

	for (int x=-figure; x<=figure; x++)
	{
		for (int y=-figure; y<=figure; y++)
		{
			if ( !IsCanMoveTo(pRegion,lX+x,lY+y) )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CBaseAI::MoveToCell(LONG lDir,LONG lCurrentX,LONG lCurrentY)
{
	if( m_pOwner == NULL ) 
	{
		return FALSE;
	}

	if( m_pOwner ->CanMove() == FALSE )
	{
		return FALSE;
	}

	CRegion* pRegion = dynamic_cast<CRegion*>( m_pOwner -> GetFather() );
	if( pRegion == NULL ) 
	{
		return FALSE;
	}

	//##���ܹ��ƶ��ķ���
	LONG lCellX,lCellY;
	BOOL bMove=TRUE;

	// 3-10-2009 ���ڹ����������
	for( list<POINT>::iterator it = s_listMoveCheckCell[0][lDir].begin();
		it != s_listMoveCheckCell[0][lDir].end(); it++ )
	{
		lCellX = lCurrentX + it -> x;
		lCellY = lCurrentY + it -> y;

		if(  !IsCanMoveTo(pRegion,lCellX,lCellY) )
		{
			bMove = FALSE;
			break;
		}
	}
	return bMove;
}

//ʹ��һ������
BOOL CBaseAI::BeginSkill(stModuParam *pModuParam, LONG lx, LONG ly,CMoveShape* pTarget)
{
	if (pModuParam==NULL)
	{
#ifdef _SKILL_DEBUG
		char str[256];
		_snprintf(str, 256, "Skill ʧ��:����ID:%d,����%d\n",pSkill->GetID(),4);
		PutStringToFile("SKill.log",str);
#endif
		return FALSE;
	}
	long ltype=0;
	CGUID guid;
	if (pTarget)
	{
		ltype = pTarget->GetType();
		guid  = pTarget->GetExID();
	}

	CBaseModule *pSkill = CNewSkillFactory::CreateModule(pModuParam->GetID(),pModuParam->GetLv(),pModuParam,pModuParam->GetGlobleID(),pModuParam->GetGoodsIndex());
	if (NULL == pSkill)
	{
		//ʹ�ü���ʧ��ʱ��Ĵ���	
		CNewSkillFactory::RealeaseModuParam(pModuParam);
		return FALSE;
	}

	if(pModuParam->GetModType()==ModuType_Skill_Passive	|| pModuParam->GetModType()==ModuType_Buff
														|| pModuParam->GetModType()==ModuType_DeBuff 
														|| pModuParam->GetModType()==ModuType_Skill_OnOff
														|| pSkill->Begin(lx,ly,ltype,guid)==MR_Modu_End)
	{
		
		//ʹ�ü���ʧ��ʱ��Ĵ���	
		CNewSkillFactory::RealeaseModuParam(pModuParam);
		return FALSE;
	}
	else
	{	
		if (m_pOwner->GetType()==TYPE_PLAYER && pModuParam->GetID()<50000)
		{
			((CPlayer*)m_pOwner)->OnWeaponDamaged();
		}
		if (pModuParam->GetAttack()==eYesAttack)
		{
			//��������
			m_pOwner->SetAttackTarget(guid,ltype);
			m_pOwner->ProduceEvent(ET_Attack);
		}
		m_pOwner->ProduceEvent(ET_UseSkill);
		m_pOwner->AddActiveSkill(pModuParam);
		return TRUE;
	}
}

BOOL CBaseAI::OnMove(LONG lX, LONG lY,LONG actTime)
{
	if (m_pOwner && m_pOwner->CanMove())
	{
		m_pOwner->OnMove((float)lX+0.5f,(float)lY+0.5f);
		AddAIEvent(AET_MOVE,actTime);
	}
	else
	{
		AddAIEvent(AET_MOVE,MOVE_FAIL_APACE_TIME);
	}

	return true;
}

//ɾ�����ڴ���
bool CBaseAI::CleanCycleSpring()
{
	if (m_csCycleSpring.csSpringType!=CS_LEVELFIGHT)
	{
		m_csCycleSpring.Clean();
	}
	return true;
}

//�ص���ƽ״̬
BOOL CBaseAI::ReturnPeace()
{
	return false;
}

BOOL CBaseAI::CheckCycleSpring(SCycleSpringEvent& cycleevent)
{
	if(cycleevent.csSpringType==CS_NULL)
		return FALSE;

	else if(cycleevent.csSpringType==CS_MOVEBY_STEP)
	{
		if(--cycleevent.unActType.tgMoveByStep.lStepNum<0)
		{
			cycleevent.Clean();
			//chenaly_zhanshi
			//m_bIsLeaveRoad=FALSE;
			return FALSE;
		}

		//CheckLevelRoad();

		CRegion* pRegion = dynamic_cast<CRegion*>( m_pOwner -> GetFather() );

		if (!pRegion)
		{
			cycleevent.Clean();
			return FALSE;
		}

		MoveTo((LONG)cycleevent.unActType.tgMoveByStep.lDirect);

		return TRUE;
	}
	else if (cycleevent.csSpringType==CS_MOVEBY_TIME)
	{
		//CheckLevelRoad();

		if (timeGetTime()> cycleevent.unActType.tgMoveByTime.lStopTime)
		{
			cycleevent.Clean();
			//chenaly_zhanshi
			//m_bIsLeaveRoad=FALSE;
			return FALSE;
		}

		CRegion* pRegion = dynamic_cast<CRegion*>( m_pOwner -> GetFather() );
		if (!pRegion)
		{
			cycleevent.Clean();
			return FALSE;
		}
		MoveTo((LONG)cycleevent.unActType.tgMoveByTime.lDirect);
		return TRUE;
	}
	else if (cycleevent.csSpringType==CS_MOVETO_CELL)
	{
		//CheckLevelRoad();

		LONG lX		= m_pOwner -> GetTileX();
		LONG lY		= m_pOwner -> GetTileY();
		LONG lDir	= GetLineDir(  lX, lY ,cycleevent.unActType.tgMoveToCell.lX, cycleevent.unActType.tgMoveToCell.lY);
		m_pOwner -> GetDirPos( lDir, lX, lY );
		CRegion* pRegion = dynamic_cast<CRegion*>( m_pOwner -> GetFather() );
		if (!pRegion)
		{
			cycleevent.Clean();
			return FALSE;
		}
		if (lX== cycleevent.unActType.tgMoveToCell.lX && lY==cycleevent.unActType.tgMoveToCell.lY)
		{
			cycleevent.Clean();
			//chenaly_zhanshi
			//m_bIsLeaveRoad=FALSE;
			return FALSE;
		}
		MoveTo(lX,lY);
		return TRUE;
	}
	else if(cycleevent.csSpringType==CS_SKILL)
	{
		CMoveShape* pShape=GetTarget();
		if(pShape==NULL)
		{
			cycleevent.Clean();
			return FALSE;
		}
		CMonster *pMonster = dynamic_cast<CMonster*>(m_pOwner);
		if (pMonster!=NULL)
		{
#ifdef _RUNSTACKINFO1_
			char pszStatckInfo[256]="";
			_snprintf(pszStatckInfo,256,"CBaseAI::CheckCycleSpring(SCycleSpringEvent& cycleevent) skillid:%d,lv:%d Start",cycleevent.unActType.tgUseSkill.lSkillID,cycleevent.unActType.tgUseSkill.lSkillLevel);
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

			pMonster->InitSkillInstance(cycleevent.unActType.tgUseSkill.lSkillID,cycleevent.unActType.tgUseSkill.lSkillLevel,0,0,0,NULL_GUID,9);
			BeginSkill(pMonster->GetInstanceSkill(),
				pShape->GetTileX(),pShape->GetTileY(),pShape);
#ifdef _RUNSTACKINFO1_
			//char pszStatckInfo[256]="";
			_snprintf(pszStatckInfo,256,"CBaseAI::CheckCycleSpring(SCycleSpringEvent& cycleevent) end");
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
		}
		else
		{
			char pszInfo[1024]="";
			_snprintf(pszInfo,1024,"CBaseAI::CheckCycleSpring(SCycleSpringEvent& cycleevent) exception");
			PutStringToFile("CheckCycleSpring",pszInfo);
		}	

		cycleevent.Clean();
		return TRUE;
	}
	else if( cycleevent.csSpringType == CS_SKILLEX )
	{
		// find the target specified by cycleevent.gAimGUID
		CServerRegion *pRegion = static_cast<CServerRegion*>( m_pOwner->GetFather() );
		CMoveShape *pTarget = static_cast<CMoveShape*>( pRegion->FindChildObject( TYPE_MONSTER, cycleevent.gAimGUID ));
		if( pTarget == NULL )
		{
			pTarget = static_cast<CMoveShape*>( pRegion->FindChildObject( TYPE_PLAYER, cycleevent.gAimGUID ));
		}

		if (pTarget!=NULL)
		{
			static_cast<CMonster*>( m_pOwner )->InitSkillInstance( cycleevent.unActType.tgUseSkill.lSkillID, cycleevent.unActType.tgUseSkill.lSkillLevel,0,0,0,NULL_GUID,8 );
			BeginSkill( m_pOwner->GetInstanceSkill(), pTarget->GetTileX(), pTarget->GetTileY(), pTarget );
		}
		

		cycleevent.Clean();
		return TRUE;
	}
	else if( cycleevent.csSpringType == CS_SKILLCELL )
	{
		LONG lPos = cycleevent.unActType.tgUseSkill.lPos;
		LONG lPosX = lPos >> 16;
		LONG lPosY = lPos & 0x0000ffff;

		static_cast<CMonster*>( m_pOwner )->InitSkillInstance( cycleevent.unActType.tgUseSkill.lSkillID, cycleevent.unActType.tgUseSkill.lSkillLevel,0,0,0,NULL_GUID,7 );
		BeginSkill( m_pOwner->GetInstanceSkill(), lPosX, lPosY, NULL );

		cycleevent.Clean();

		return TRUE;
	}
	else if (cycleevent.csSpringType==CS_STAND)
	{
		if (--cycleevent.unActType.tgStand.lStandTimes<0)
		{
			cycleevent.Clean();
			return FALSE;
		}
		Stand(cycleevent.unActType.tgStand.lStandTime);
		return TRUE;
	}
	else if (cycleevent.csSpringType==CS_RANDOMMOVE)
	{
		if (timeGetTime()<cycleevent.unActType.lRandomMoveTime)
		{
			RandomRun();
			return TRUE;
		}
		else
		{
			cycleevent.Clean();
			//chenaly_zhanshi
			//m_bIsLeaveRoad=FALSE;
			return FALSE;
		}
	}
	else if (cycleevent.csSpringType==CS_ESCAPE)
	{
		if (timeGetTime()<cycleevent.unActType.lRandomMoveTime)
		{
			OnEscapeState();
			return TRUE;
		}
		else
		{
			// �Ƴ�����״̬���ָ�����(�����״̬�Ѿ���OnEscapeState�б��Ƴ����������Ҳ�ǰ�ȫ��)
			m_pOwner->DelNewState( STATE_BASE_WEAKNESS, 1 );
			cycleevent.Clean();
			return FALSE;
		}

	}
	else if (cycleevent.csSpringType== CS_SENDEXPACT)
	{
		//CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);
		Stand(EXCEPTION_STATE_SPACE);
		AddCycleSpring(CS_SENDEXPACTNEXT,cycleevent.unActType.tgSendExpAct.lActionID,
			cycleevent.unActType.tgSendExpAct.lActTime,
			cycleevent.unActType.tgSendExpAct.lAimType,
			cycleevent.gAimGUID);
		return TRUE;
	}
	else if (cycleevent.csSpringType== CS_SENDEXPACTNEXT)
	{
		CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);
		if (pMonster!= NULL)
		{
			pMonster->ExpAction(cycleevent.unActType.tgSendExpAct.lActionID,cycleevent.unActType.tgSendExpAct.lAimType,cycleevent.gAimGUID);
		}

		AddCycleSpring(CS_STAND,cycleevent.unActType.tgSendExpAct.lActTime/EXCEPTION_STATE_SPACE ,EXCEPTION_STATE_SPACE);
		Stand(EXCEPTION_STATE_SPACE);
		return TRUE;
	}
	else if(cycleevent.csSpringType==CS_LEVELFIGHT)
	{

		CMonster* pMonster= dynamic_cast<CMonster*>(m_pOwner);
		long aimX,aimY;
		aimX=cycleevent.unActType.tgLevelFight.AimX;
		aimY=cycleevent.unActType.tgLevelFight.AimY;
		long x= pMonster->GetPosX();
		long y= pMonster->GetPosY();

		if (cycleevent.unActType.tgLevelFight.lTime>= timeGetTime())
		{
			int lDir=GetLineDir(x,y,aimX,aimY);
			m_pOwner -> GetDirPos( lDir, x, y );
			float fDis=1;
			if (lDir%2!=0) fDis*= 1.414f;
			float fTime= fDis/m_pOwner->GetSpeed();

			OnMove(x,y,(DWORD)fTime);
			//m_pOwner->OnMove()
			//����λ��ֵ
			m_pOwner->SetPosXY(x,y);

			return TRUE;
		}
		else
		{
			m_pOwner->ProduceEvent(ET_Move);
			m_pOwner->ProduceEvent(ET_EventBreak);

			m_pOwner->SetPosition(aimX,aimY);

			// ���ù�λ����ʧ�ܣ�Ŀ��㲻��BLOCK_NO
			if( m_pOwner->GetTileX() != aimX || m_pOwner->GetTileY() != aimY )
			{
				CRegion *region = static_cast<CRegion*>( m_pOwner->GetFather()  );
				if( region != NULL )
				{
					// Ŀ�������赲������Χ�������赲��
					long rx, ry;
					region->GetRandomPosInRange( rx, ry, aimX - 3, aimY - 3, 6, 6 );

					char msg[1024];
					_snprintf( msg, sizeof( msg ), "��λ��������ʧ�ܣ�Ŀ���赲���ͣ�%u�����²��ҵ㣺(%d,%d):%u.", 
						region->GetBlock( aimX, aimY ), rx, ry, region->GetBlock( rx, ry ) );
					PutStringToFile( "debug_monster", msg );

					m_pOwner->SetPosition( rx, ry );
				}
			}
			CMonsterList::tagMonster* p = CMonsterList::GetPropertyByOrginName(
				static_cast<CMonster*>( m_pOwner )->GetOriginName());
			m_pOwner->God( p->byIsunbeatable == 1 ? true : false );
			m_pOwner->SetChangeSpeed(0);
			m_pOwner->SendChangedSpeedToClients();
			Stand(1000);
			cycleevent.Clean();
			return TRUE;
		}
			
	}
	else
		return false;
}

BOOL CBaseAI::GetNearPosition(LONG &x, LONG &y, LONG dir,LONG searchDir)
{
	if (dir= -1)
		dir= random(8);
	else
		dir=dir%8;

	if (searchDir= -1)
		searchDir= random(8);
	else
		searchDir=searchDir%8;

	// ��һȦ
	static int pos_1[8][2]=
	{
		-1,-1, 0,-1, 1,-1,
		-1, 0,	   1, 0,
		-1, 1, 0, 1, 1, 1,
	};

	// �ڶ�Ȧ
	static int pos_2[16][2]=
	{
		-2,-2, -1,-2, 0,-2, 1,-2, 2,-2,
		-2,-1,					 2,-1,
		-2, 0,					 2, 0,
		-2, 1,					 2, 1,
		-2, 2, -1, 2, 0, 2, 1, 2, 2, 2,
	};

	// ����Ȧ
	static int pos_3[24][2]=
	{
		-3,-3, -2,-3, -1,-3, 0,-3, 1,-3, 2,-3, 3,-3,
		-3,-2,								   3,-2,
		-3,-1,								   3,-1,
		-3, 0,								   3, 0,
		-3, 1,								   3, 1,
		-3, 2,								   3, 2,
		-3, 3, -2, 3, -1, 3, 0, 3, 1, 3, 2, 3, 3, 3
	};

	CRegion* pRegion= dynamic_cast<CRegion*>(m_pOwner->GetFather());
	if (pRegion== NULL)	return FALSE;

	POINT  point;	
	int iSearchPos= searchDir;
	int iIndex=0;
	for (int a=0; a<8/2; a++)
	{
		iIndex=(iSearchPos+a)%8;
		point.x= x+ pos_1[iIndex][0];
		point.y= y+ pos_1[iIndex][1];
		if (pRegion->GetBlock(point.x,point.y)== CRegion::BLOCK_NO)
		{x= point.x; y= point.y;return TRUE;}

		iIndex=(iSearchPos-a-1)%8;
		point.x= x+ pos_1[iIndex][0];
		point.y= y+ pos_1[iIndex][1];
		if (pRegion->GetBlock(point.x,point.y)== CRegion::BLOCK_NO)
		{x= point.x; y= point.y; return TRUE;}
	}

	iSearchPos= searchDir*2;
	for (int a=0; a<16/2; a++)
	{
		iIndex= (iSearchPos+a)%8;
		point.x= x+ pos_2[iIndex][0];
		point.y= y+ pos_2[iIndex][1];
		if (pRegion->GetBlock(point.x,point.y)== CRegion::BLOCK_NO)
		{x= point.x; y= point.y; return TRUE;}

		iIndex=(iSearchPos-a-1)%8;
		point.x= x+ pos_2[iIndex][0];
		point.y= y+ pos_2[iIndex][1];
		if (pRegion->GetBlock(point.x,point.y)== CRegion::BLOCK_NO)
		{x= point.x; y= point.y; return TRUE;}
	}

	iSearchPos= searchDir*3;
	for (int a=0; a<24/2; a++)
	{
		iIndex= (iSearchPos+a)%8;
		point.x= x+ pos_3[iIndex][0];
		point.y= y+ pos_3[iIndex][1];
		if (pRegion->GetBlock(point.x,point.y)== CRegion::BLOCK_NO)
		{x= point.x; y= point.y; return TRUE;}

		iIndex= (iSearchPos-a-1)%8;
		point.x= x+ pos_3[iIndex][0];
		point.y= y+ pos_3[iIndex][1];
		if (pRegion->GetBlock(point.x,point.y)== CRegion::BLOCK_NO)
		{x= point.x; y= point.y; return TRUE;}
	}

	return FALSE;
}
//�������ڴ���
BOOL CBaseAI::AddCycleSpring(CYCLE_SPRINGACT type, LONG m_1, LONG m_2,LONG m_3,CGUID guid)
{
	if (m_csCycleSpring.csSpringType==CS_LEVELFIGHT) return FALSE;
	
	m_csCycleSpring.csSpringType=type;
	m_csCycleSpring.unActType.tgSendExpAct.lActionID=m_1;
	m_csCycleSpring.unActType.tgSendExpAct.lActTime=m_2;
	m_csCycleSpring.unActType.tgSendExpAct.lAimType=m_3;

	m_csCycleSpring.gAimGUID= guid;
	return TRUE;
}

BOOL CBaseAI::RunByRoad()
{return FALSE;}


BOOL CBaseAI::RandomRun()
{
	return TRUE;
}

VOID CBaseAI::StartCycle(DWORD dwStartTime,DWORD dwCycleTime )
{
	if (m_aeCycle.aetEventType==AET_NULL)
	{
		m_aeCycle.aetEventType=AET_CYCLE;
		//���������ע����Ϣ��ע��һ���¼�
		tagTimerVar* pTimvar = CBaseObject::CreateTimerVar(12);
		pTimvar->TimerType = ChildObjType_AI;
		pTimvar->lvar = AET_CYCLE;
		m_aeCycle.lTimerID=GameManager::GetInstance()->Schedule(m_pOwner,(void*)pTimvar,timeGetTime()+dwStartTime,dwCycleTime);
		m_aeCycle.dwSpringTime=dwCycleTime;
	}
}

VOID CBaseAI::EndCycle()
{
	LogoutEvent(m_aeCycle);
}

void CBaseAI::Reset()
{
	LogoutEvent( m_aeCycle );
	LogoutEvent( m_aeActive );
	LogoutEvent( m_aeScriptAISkill );
	m_aesAIState = AES_INVALID;
}

VOID CBaseAI::StartActiveAct()
{
	AddAIEvent(AET_MOVE,0);
}

VOID CBaseAI::ChangeStateTo(INT state)
{
	if (m_pOwner->GetState()==state)return;

	if (m_pOwner->IsDied())return;

	if (state==CShape::STATE_PEACE)
	{
		m_pOwner->ChangeStateTo((CShape::eSTATE)state);
		m_aesAIState=AES_FIGHT_NORMAL;
		CleanCycleSpring();
	}
	else if (state==CShape::STATE_FIGHT)
	{
		m_pOwner->ChangeStateTo((CShape::eSTATE)state);
		m_aesAIState=AES_FIGHT_NORMAL;
		CleanCycleSpring();
		FightSpring();
	}
	else if (state==CShape::STATE_DIED)
	{
		m_pOwner->ChangeStateTo((CShape::eSTATE)state);
		m_aesAIState=AES_FIGHT_NORMAL;
		CleanCycleSpring();
	}

}

VOID CBaseAI::Stand(LONG time)
{
	AddAIEvent(AET_MOVE ,time);
}
CMoveShape*	CBaseAI::GetOwner()
{
	return m_pOwner;
}
VOID CBaseAI::SetOwner(CMoveShape* pOwner)
{
	if( pOwner )
	{
		Clear();
		m_pOwner = pOwner;
	}
}

//AI����
LONG CBaseAI::GetAIType()
{
	return m_lAIType;
}
VOID CBaseAI::SetAIType(LONG lAIType)
{
	m_lAIType=lAIType;
}
//�����ܽ�����ʱ�򣬣����ܽ�����ʱ�򱻵��ã�
BOOL CBaseAI::OnSkillEnd(long lAddSub)
{
	// ���ܻص�ʱAI�Ѿ���Ч
	if( m_aesAIState == AES_INVALID )
	{
		return FALSE;
	}

	DWORD	dwActStartTime= m_pOwner->GetActStartTime();				//������ʼʱ��
	DWORD   dwActTime= m_pOwner->GetActTime();					//����ʱ��
	DWORD   dwLeftTime= dwActStartTime+dwActTime-timeGetTime();
	//DeleteAIEvent(AET_ACT);
	AddAIEvent(AET_MOVE,dwLeftTime>0?dwLeftTime:0);
	return TRUE;
}
//����������ʱ��
BOOL CBaseAI::WhenBeenHurted(LONG lType,const CGUID& guid ,DWORD a)
{
	return m_pOwner->GetForceAttack();
}

//��ɱ����ʱ��
BOOL CBaseAI::WhenBeenKilled(LONG lType,const CGUID& guid)
{
	return false;
}

//���ߣ����ߣ����ѣ���ȡ����ʱ�䣬��ѯ�Ƿ���
VOID CBaseAI::Hibernate()
{
	//m_aesAIState=AES_FIGHT_NULL;
	ChangeStateTo(CShape::STATE_HANGUP);
	//m_bIsHibernate=TRUE;
	m_dwDormancyTime = timeGetTime();
	m_dwDormancyInterval = 0;
}
VOID CBaseAI::WakeUp()
{
 	m_dwDormancyInterval = timeGetTime() - m_dwDormancyTime;
	m_dwDormancyTime = 0;
	ChangeStateTo(CShape::STATE_PEACE);
	//m_bIsHibernate=FALSE;
	StartActiveAct();

	//if(m_aeActive.aetEventType==AET_ACT)
	//{
	//	ChangeStateTo(CShape::STATE_FIGHT);
	//	m_aesAIState=AES_FIGHT_ATTACK;
	//}
	//else if( m_aeActive.aetEventType==AET_MOVE)
	//{
	//	//
	//	ChangeStateTo(CShape::STATE_PEACE);
	//	//ChangeStateTo(AES_PEACE);
	//	//m_aesAIState=AES_PEACE;

	//}
	//else 
	//{
	//	//m_aesAIState=AES_PEACE;
	//	ChangeStateTo(CShape::STATE_PEACE);
	//}

	//if(m_aeCycle.aetEventType==AET_NULL)
	//{
	//	AddAIEvent(AET_CYCLE,1500);
	//}
}

DWORD CBaseAI::GetDormancyInterval()
{
	return m_dwDormancyInterval;
}
BOOL CBaseAI::IsHibernated()
{
	return m_pOwner->GetState()==CShape::STATE_HANGUP? true:false;
}

//�ƶ�
BOOL CBaseAI::MoveTo(float fDestX, float fDestY)
{
	if( !m_pOwner ) return	FALSE;

	LONG lPosX		= m_pOwner -> GetTileX();
	LONG lPosY		= m_pOwner -> GetTileY();
	LONG lDir		= GetLineDir( lPosX, lPosY, (long)fDestX, (long)fDestY );
	return MoveTo(lDir);
}



//VOID CBaseAI::ForceMove( LONG lDestX, LONG lDestY,DWORD dwTime )
//{
//	LogoutEvent(m_aeActive);
//	AddAIEvent(AET_MOVE,dwTime);//δ���
//}
//����

VOID CBaseAI::Attack(long tID, LONG lX/* =0 */, LONG lY/* =0 */, CMoveShape* pTarget/* =NULL  */)
{

}

//Ŀ�꣺ ���ã�ѯ�ʣ���ȡ���ͣ���ȡid����ȡָ��
VOID CBaseAI::SetTarget( LONG lType, const CGUID& TargetGuid )
{
	//if (m_pOwner->IsGod()) return;
	// ������ڹ�λ����(�ض��ڹ���)���Ͳ�������Ŀ��
	if( m_csCycleSpring.csSpringType == CS_LEVELFIGHT )
	{
		return;
	}
	m_lTargetType	= lType;
	m_TargetGuid	= TargetGuid;
	FindTargetSpring();
	ChangeStateTo(CShape::STATE_FIGHT);
	if (m_aesAIState==AES_FIGHT_TRANSITION)
	{
		m_aesAIState= AES_FIGHT_NORMAL;
	}
}

VOID CBaseAI::SetForceTarget( LONG lType, const CGUID& TargetGuid )
{
}
VOID CBaseAI::CleanForceTarget ()
{}

BOOL CBaseAI::HasTarget()
{
	if( m_pOwner != NULL && 
		( (NULL_GUID != m_TargetGuid && m_lTargetType > 0) ||
		(m_lTargetX != 0 && m_lTargetY != 0) ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LONG CBaseAI::GetTargetType()
{
	return m_lTargetType;
}
const CGUID& CBaseAI::GetTargetID()
{
	return m_TargetGuid;
}
CMoveShape* CBaseAI::GetTarget()
{
	if( m_pOwner == NULL ) 
	{
		return NULL;
	}
	if( m_lTargetType == 0 || m_TargetGuid == NULL_GUID )
	{
		return NULL;
	}
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( m_pOwner ->GetFather() );
	if( pRegion == NULL ) 
	{
		return NULL;
	}

	return dynamic_cast<CMoveShape*>( pRegion -> 
		FindChildObject( m_lTargetType, m_TargetGuid ) );
}
//��ʧĿ��
BOOL CBaseAI::WhenLoseTarget()
{
	return LoseTarget();
}

BOOL CBaseAI::LoseTarget()
{
	if( m_pOwner == NULL ) return FALSE;

	//##�����ǰ��AI�¼���Attack���Ƿ���Ҫ������¼���

	CMonster*pOwner=dynamic_cast<CMonster*>(m_pOwner);
	//##���һЩ��Ա����
	m_TargetGuid			= NULL_GUID;
	m_lTargetType			= 0;
	m_lTargetX				= 0;
	m_lTargetY				= 0;

	return TRUE;
}
//ǿ��Ŀ��
//BOOL CBaseAI::ForceTarget(CMoveShape * pTarget , DWORD dwForceTime)
//{
//
//	return true;
//}

//�ı�Ϊ�쳣״̬
//BOOL CBaseAI::ChangeToExceptional(AI_EXEC_STATE ExpectionalType,LONG ExpectionalTime)
//{
//	return FALSE;
//}
//��ʱ����ǰͳһ�Ľӿ�
//##��ȡ��ǰ�����¼����е�ͷ��
BOOL CBaseAI::IsInAct()
{
	return m_aeActive.aetEventType==AET_ACT;
}

//##���AI�����������¼��Ľӿ�
VOID CBaseAI::Clear()                                    //�иĶ�
{
	m_dwLastExecTimeStamp	= 0;
	//m_TargetGuid				= 0;
	m_lTargetType			= 0;
	m_pOwner				= NULL;
	m_aesAIState            = AES_FIGHT_NORMAL;
	LogoutEvent(m_aeActive);
	m_aeActive.Clean();
	LogoutEvent(m_aeCycle);
	m_aeCycle.Clean();
	CleanCycleSpring();

}
//##��Ӻ�̨����ʱ���õĺ���
//VOID CBaseAI::WhenAddBackStageSkill(long tID)
//{
//	//##��������߲����ڡ�ֱ���˳���
//	if( m_pOwner == NULL ) return;
//
//	if( tID != SKILL_UNKNOW )
//	{
//		m_vBackStageSkills.push_back( tID );
//	}
//}


//��Ӻ�ƽʱ��Ľű���
VOID CBaseAI::AddIdleActScript(string & strScriptName)
{
	m_strIdleActScript=strScriptName;
}


//�ű��ӿ�
BOOL CBaseAI::UseSkillByID(LONG skillid,LONG skilllevel, const CGUID &targetID)
{
	if( targetID == NULL_GUID )
	{
		AddCycleSpring(CS_SKILL,skillid,skilllevel);
	}
	else
	{
		AddCycleSpring( CS_SKILLEX, skillid, skilllevel, 0, targetID );
	}
	return TRUE;
}

BOOL CBaseAI::UseSkillByIDOnCell( LONG skillID, LONG skillLevel, LONG posX, LONG posY )
{
	if( posX == 0 && posY == 0 )
	{
		CMoveShape *pTarget = GetTarget();
		if( pTarget == NULL )
		{
			return FALSE;
		}

		posX = pTarget->GetTileX();
		posY = pTarget->GetTileY();
	}
	// maybe it will occur some bugs somedays
	LONG pos = ( posX << 16 ) | ( posY );
	AddCycleSpring( CS_SKILLCELL, skillID, skillLevel, pos );
	return true;
}

BOOL CBaseAI::MoveToCellOnPeace(LONG x,LONG y)
{
	if (m_pOwner->GetState()== CShape::STATE_PEACE)
	{
		AddCycleSpring(CS_MOVETO_CELL,x,y);
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseAI::MoveToCellOnFight		(LONG x,LONG y)
{
	if (m_pOwner->GetState()==CShape::STATE_FIGHT && m_aesAIState!= AES_FIGHT_ESCAPE	)
	{
		AddCycleSpring(CS_MOVETO_CELL,x,y);
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseAI::MoveToCellAround(LONG x,LONG y)
{
	if (m_pOwner->Distance(x,y)>POST_AREA)
	{
		AddCycleSpring(CS_MOVETO_CELL,x,y);
	}
	return TRUE;
}

BOOL CBaseAI::MoveByStepOnPeace(LONG dir, LONG step)
{
	if (m_pOwner->GetState()== CShape::STATE_PEACE)
	{
		AddCycleSpring(CS_MOVEBY_STEP,dir,step);
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseAI::MoveByStepOnFight(LONG dir, LONG step)
{
	if (m_pOwner->GetState()==CShape::STATE_FIGHT && m_aesAIState!= AES_FIGHT_ESCAPE)
	{
		AddCycleSpring(CS_MOVEBY_STEP,dir,step);
		return TRUE;
	}
	return FALSE;
}

//�ں�ƽ״̬�²���һ������
BOOL CBaseAI::SendExpActOnPeace(LONG lActID,LONG lAimTime,LONG lAimType,CGUID guid )
{
	if (m_pOwner!=NULL && m_pOwner->GetState()==CShape::STATE_PEACE &&m_csCycleSpring.csSpringType!= CS_LEVELFIGHT)	
	{
		AddCycleSpring(CS_SENDEXPACT,lActID,lAimTime,lAimType,guid);
		return TRUE;
	}
	return FALSE;
}
//��ս��״̬�²���һ������
BOOL CBaseAI::SendExpActOnFight(LONG lActID,LONG lAimTime,LONG lAimType,CGUID guid )
{
	if (m_pOwner!=NULL && m_pOwner->GetState()==CShape::STATE_FIGHT && m_csCycleSpring.csSpringType!= CS_LEVELFIGHT)	
	{
		AddCycleSpring(CS_SENDEXPACT,lActID,lAimTime,lAimType,guid);
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseAI::MoveByTimeOnPeace(LONG dir, LONG time)
{
	if (m_pOwner->GetState()== CShape::STATE_PEACE)
	{
		AddCycleSpring(CS_MOVEBY_TIME,dir,timeGetTime()+time);
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseAI::MovebyTimeOnFight(LONG dir, LONG time)
{
	if (m_pOwner->GetState()==CShape::STATE_FIGHT && m_aesAIState!= AES_FIGHT_ESCAPE)
	{
		AddCycleSpring(CS_MOVEBY_TIME,dir,timeGetTime()+time);
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseAI::RandomMoveByTime(LONG time)//��ս��״̬��ʹ֮�����߶�
{
		AddCycleSpring(CS_RANDOMMOVE,time+timeGetTime());
		return TRUE;

	return FALSE;
}

BOOL CBaseAI::Stand(LONG times,LONG time)
{
	AddCycleSpring(CS_STAND,times,time);
	return TRUE;
}


//����һ��ʱ��(��ƽ״̬�£�  ֻ��Ҫģ�����ܣ���Ϊ����������)
BOOL CBaseAI::EscapeByTimeOnPeace(LONG dir,LONG time)
{
	return MoveByTimeOnPeace(dir,time);
}

//����һ��ʱ��
BOOL CBaseAI::EscapeByTimeOnFight(LONG time)
{
	if (m_pOwner->GetState()==CShape::STATE_FIGHT && m_aesAIState!= AES_FIGHT_ESCAPE)
	{
		AddCycleSpring(CS_ESCAPE,timeGetTime()+time);
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseAI::LevelFight(LONG time,LONG aimX,LONG aimY)
{
	//ChangeStateTo(m_)

	AddCycleSpring(CS_LEVELFIGHT,timeGetTime()+time,aimX ,aimY);
	int speed=1/(CGlobeSetup::GetSetup()->fReturnPeaceSpeed - m_pOwner->GetSpeed());
	m_pOwner->SetChangeSpeed(speed);
	m_pOwner->SendChangedSpeedToClients();
	m_pOwner->God(true);
	return TRUE;

}

//ȡ��ע�����Ϊ
BOOL CBaseAI::DeleteSpring()
{
	CleanCycleSpring();;
	return TRUE;
}
//�ر�����״̬
BOOL CBaseAI::EndEscape()
{
	if (m_aesAIState==AES_FIGHT_ESCAPE)
	{
		m_aesAIState=AES_FIGHT_NORMAL;
	}
	if (m_csCycleSpring.csSpringType==CS_ESCAPE)
	{
		CleanCycleSpring();
	}
	return TRUE;
}

bool LogicLogInterface::logT470_AIExc_log(CMonster* pMonster,DWORD dwExcType)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;

	if (pMonster== NULL)
	{
		return false;
	}
	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		470, strTime.c_str(),
		&(pMonster->GetExID()),pMonster->GetName(),
		dwExcType);
}