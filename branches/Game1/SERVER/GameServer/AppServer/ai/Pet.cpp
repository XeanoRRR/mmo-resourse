#include "StdAfx.h"
#include <MMSystem.h>
#include "Pet.h"
#include "..\Player.h"
#include "..\Monster.h"
#include "..\..\..\setup\globesetup.h"

//#include "../Skills/SkillBaseProperties.h"
//#include "../skills/SkillFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//*
CPet::CPet() : m_dwInvalidMasterTimeStamp( 0 ), m_dwSeekMasterTimeStamp( 0 ), m_bMasterLogout( FALSE ), 
m_dwPetLifeCycleTimeStamp( 0 ), m_dwPetLifeCycleCounter( 0 ), m_paPetAction( PA_FALLOWING ), 
m_psemPetAIMode( PSEM_DEFENSE )
{
}

CPet::~CPet()
{
}


VOID CPet::SetPetCurrentAIMode( PET_SEARCH_ENEMY_MODE pm )
{
	
}

CPet::PET_SEARCH_ENEMY_MODE CPet::GetPetCurrentAIMode()
{
	return m_psemPetAIMode;
}

VOID CPet::SetPetCurrentAction( CPet::PET_ACTION pa )
{
}

CPet::PET_ACTION CPet::GetPetCurrentAction()
{
	return m_paPetAction;
}


VOID CPet::OnSchedule()
{
}

//##����������AI����
VOID CPet::OnAttackingSchedule()
{
}

VOID CPet::OnFallowingSchedule()
{
}

VOID CPet::OnStayingSchedule()
{		
}


CMoveShape* CPet::GetPetMaster()
{
	CMoveShape* pResult = NULL;
	return pResult;
}

//##��ÿ����Ҫ�ƶ���ʱ����Ѱ�����Ŀ��
BOOL CPet::OnMoving()
{
	return FALSE;
}


BOOL CPet::OnSearchEnemy()
{
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ����:	OnIdle
// ����:	��
// ����ֵ:	��
// ������	�ú�����AI���߱����õ�ʱ����á�����AI����������������վ�����������˵��¼�
//			��AI���ߡ�Ҳ���������õ�ʱ�����һЩ����ƶ��Ĳ�������ƷAI����������������
//			վ���¼�����������ʧ���¼������ﵽ��ʱ�����Ŀ�ġ�
//--------------------------------------------------------------------------------------
VOID CPet::OnIdle()
{
	//##Todo: ����ĸú�����ҪĿ����Ϊ���ڹ��﷢���ڼ������һ��Stand��״̬��
	//##�����Stand״̬����ҪĿ����Ϊ������ʱ��Ƭ�����⣬ż��������ƶ���
	//##��һ��ʱ�����󣬴�ʱ�������վ��״̬����ʧ����������һ����Ѱ����
	//##��״̬�����״̬���档�������Ѱ�Ƿ�������ڸ���
	CMonster* pOwner = dynamic_cast<CMonster*>( GetOwner() );
	if( pOwner && pOwner -> IsDied() == FALSE )
	{
		
	}
}

BOOL CPet::OnLoseTarget()
{
	return FALSE;
}

VOID CPet::SetTarget( LONG lType, const CGUID& TargetGuid )
{
	CMonsterAI::SetTarget( lType, TargetGuid );
}
