

#include "StdAfx.h"
#include "GSAutoSupply.h"
#include "Player.h"


CTimerQueue<GSAutoSupply*>		GSAutoSupply::s_TimerQueue(4, Auto_Supply_Space);

//! GSAutoSupply::�õ���ʱ������
CTimerQueue<GSAutoSupply*>& GSAutoSupply::GetTimerQueue(void)
{
	return s_TimerQueue;
}

GSAutoSupply::GSAutoSupply(void)
:m_pOwner(NULL)
,m_bTimerRunning(FALSE)
{
	
}

GSAutoSupply::~GSAutoSupply(void)
{
	CloseTimer();
}

//! ����������
void GSAutoSupply::SetOwner(CPlayer *pMainPlayer)
{
	m_pOwner = pMainPlayer;
	if (NULL != m_pOwner)
	{
		BOOL bOpenTimer = FALSE;
		//! ��¼������Ʒ��λ�ñ���
		for (LONG i = eAST_HP1; i < AotoSupplyTypeNum; ++i)
		{
			if(GoodsReady(i))
				bOpenTimer = TRUE;
		}

		if (bOpenTimer)
		{
			OpenTimer();
		}
	}
}

//! �ı䲹������
BOOL GSAutoSupply::ChangeSetup(LONG eSupplyType, tagAotoSupplyNode &AotoSupplyNode, BOOL bSysOpt)
{
	if (NULL != m_pOwner)
	{
		if(eASET_NotErr == SetOneSetup(eSupplyType, AotoSupplyNode) || bSysOpt)
		{
			if(GoodsReady(eSupplyType))
			{
				OpenTimer();
			}
			else
			{
				GoOnTimer();
			}

			return TRUE;
		}
	}
	return FALSE;
}

//! �Զ�����ִ��
void GSAutoSupply::Supply(LONG eSupplyType)
{
	assert(NULL != m_pOwner);
	const tagAotoSupplyNode* pAotoSupplyNode = GetOneSetup(eSupplyType);
	assert(NULL != pAotoSupplyNode);

	if (!pAotoSupplyNode->bActive || 0 == pAotoSupplyNode->lGoodsIndex)
		return;
	
	//! ----------------------------ʹ�������ж�----------------------------
	LONG lCurrScale = A_Hundred_Percent;
	switch(eSupplyType)
	{
	case eAST_HP1:
	case eAST_HP2:
		{
			if(0 != m_pOwner->GetMaxHP())
				lCurrScale = m_pOwner->GetHP() * 100 / m_pOwner->GetMaxHP();
		}
		break;
	case eAST_MP:
		{
			if(0 != m_pOwner->GetMaxMP())
				lCurrScale = m_pOwner->GetMP() * 100 / m_pOwner->GetMaxMP();
		}
		break;
	case eAST_ArmPower:
		{
			lCurrScale = m_pOwner->GetArmCurrPower();
		}
		break;
	case eAST_SpriteMP:
		{
			// 10.14.2009 ����λ��Ϊ���PK CP��λ�����ڴ���λ����Ʒ�Զ��������CP
			// ��תְҵ��80�����ϲŻᲹ��
			if( m_pOwner->GetLevel() < 80 ||
				!( m_pOwner->GetOccupation() >= OCC_Paladin &&
				m_pOwner->GetOccupation() <= OCC_Prophet ) )
			{
				break;
			}
			long lMaxCP = m_pOwner->GetMaxPKCP();
			if( lMaxCP != 0 )
			{
				lCurrScale = 100 * m_pOwner->GetPKCP() / lMaxCP;
			}
		}
		break;
	}

	if((eAST_ArmPower != eSupplyType && pAotoSupplyNode->lSpringScale <= lCurrScale)
		|| (eAST_ArmPower == eSupplyType && (0 < lCurrScale || -1 == lCurrScale))
		)
		return;

	//! ----------------------------ʹ����Ʒ----------------------------

	//! ��Ʒʹ�ü��
	DWORD lNow = timeGetTime();
	if((DWORD)(m_arrGoodsParam[eSupplyType].lLastTime + m_arrGoodsParam[eSupplyType].lCoolingTime) > lNow)
		return;

	//! ʹ��
	if(!m_pOwner->AutoUseItem(
		m_arrGoodsParam[eSupplyType].wPlace, 
		m_arrGoodsParam[eSupplyType].lPos,
		m_arrGoodsParam[eSupplyType].lTargetType,
		m_arrGoodsParam[eSupplyType].TargetGuid
		)
		)
	{
		//! ʹ�û������Ʒʧ�ܣ�˵����Ʒ�������Ʒλ���ѱ��
		//! ���»�����Ʒ
		if(m_pOwner->FindFirstGoodsByIndex(pAotoSupplyNode->lGoodsIndex, m_arrGoodsParam[eSupplyType]))
		{
			//! �ٴ�ʹ����Ʒ
			if(!m_pOwner->AutoUseItem(
				m_arrGoodsParam[eSupplyType].wPlace, 
				m_arrGoodsParam[eSupplyType].lPos,
				m_arrGoodsParam[eSupplyType].lTargetType,
				m_arrGoodsParam[eSupplyType].TargetGuid
				)
				)
			{
				return;//! ʧ���˲���¼ʹ��ʱ�䣬ֱ�ӷ�����
			}
		}
		else
		{
			GoOnTimer();
			return;
		}
	}

	CMessage msg(MSG_S2C_SPRITE_AutoSupply_UseItem_Notify);
	msg.Add(eSupplyType);
	msg.SendToPlayer(m_pOwner->GetExID(), false);

	m_arrGoodsParam[eSupplyType].lLastTime = lNow;
}

//!								��ʱ����ʱ��Ӧ
long GSAutoSupply::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
	if (NULL != m_pOwner)
	{
		BOOL bOpenTimer = FALSE;
		//! ��¼������Ʒ��λ�ñ���
		for (LONG i = eAST_HP1; i < AotoSupplyTypeNum; ++i)
		{
			Supply(i);
		}
	}

	return 0;
}

//! ����ʱ����ȡ���ĵ���
void GSAutoSupply::OnTimerCancel(DWORD timerid,const void* var)
{

}

//! ���Զ�������ʱ��
void GSAutoSupply::OpenTimer(void)
{
	//! ����������������ʱ��
	if(!m_bTimerRunning)
	{
		GetTimerQueue().Schedule(this, NULL, Auto_Supply_Space, Auto_Supply_Space);
		m_bTimerRunning = TRUE;
	}
}

//! �ر��Զ�������ʱ��
void GSAutoSupply::CloseTimer(void)
{
	//! ȡ������ʱ
	m_bTimerRunning = FALSE;
	GetTimerQueue().Cancel(this);
}

void GSAutoSupply::GoOnTimer(void)
{
	BOOL bGoOnTimer = FALSE;
	for (LONG i = eAST_HP1; i < AotoSupplyTypeNum; ++i)
	{
		bGoOnTimer |= GoodsReady(i);
	}

	if(bGoOnTimer)
		return;

	CloseTimer();
}

//! ׼����Ʒ
BOOL GSAutoSupply::GoodsReady(LONG eSupplyType)
{
	assert(NULL != m_pOwner);
	const tagAotoSupplyNode* pAotoSupplyNode = GetOneSetup(eSupplyType);
	assert(NULL != pAotoSupplyNode);
	if (0 != pAotoSupplyNode->lGoodsIndex)
	{
		//! ��ʹû���ҵ�������Ʒ��Ҳ��Ӱ�췵�ؽ��
		m_pOwner->FindFirstGoodsByIndex(pAotoSupplyNode->lGoodsIndex, m_arrGoodsParam[eSupplyType]);
		//! ����ֵӰ�첹����ʱ���Ƿ���������˷��ص���[����״̬]��[��Ʒ�Ƿ�����]����ֵ
		return pAotoSupplyNode->bActive;
	}
	return FALSE;
}