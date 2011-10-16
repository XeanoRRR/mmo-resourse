#include "stdafx.h"
#include "NormalCloseInAttackAI.h"
#include "../../EngineExtend/DisplayModel.h"
#include "PetSystem/Pet.h"
#include "../GameClient/Game.h"
#include "ClientRegion.h"
#include "SkillXml.h"
#include "SkillListIni.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NormalCloseInAttackAI::NormalCloseInAttackAI()
{
	m_pSkill = NULL;
}
NormalCloseInAttackAI::NormalCloseInAttackAI(CSkillXml *pSkill)
{
	m_pSkill = pSkill;
}
NormalCloseInAttackAI::~NormalCloseInAttackAI()
{
	
}
void NormalCloseInAttackAI::AI()
{
	//CPlayer *pPlayer = GetGame()->GetMainPlayer();
	if(!m_pSkill)
		return;
	CMoveShape* pMoveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_pSkill->GetSourType(), m_pSkill->GetSourID()));
	// ��ʼ�׶ο�������������
	if(pMoveshape && m_pSkill && m_pSkill->GetCurStep() != CSkillXml::STEP_START && m_pSkill->GetRunStepActExQueue())
	{
		/***********************************************************************/
		/* zhaohang fix */
		/***********************************************************************/
		// �ǹ⻷�༼��
		CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(m_pSkill->GetSkillID());
		if (pSkill && (pSkill->bLockSkill || pSkill->dwSkillState == 0))
		//tagSkillIni* pSkill = SkillAttrIniSetup::GetSkillIni(m_pSkill->GetSkillID());
		//if( pSkill && pSkill->GetSkillBaseAttr() && 
		//	 pSkill->GetSkillBaseAttr()->IsLockSkill() )
		{
			// �ͷŽ׶��޺�������
			if(m_pSkill->GetRunStepActExQueue()->empty())
			{
				if(pMoveshape->GetAction() == ACT_STAND || pMoveshape->GetAction() == ACT_IDLE)
					StepEndAI();
			}
		}
	}
}



/*
 * ����: ������ʾ
 * ժҪ: -
 * ����: -
 * ����ֵ: ʼ��true
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.11.10 - lpf
 *		������"��̬�������л�"
 */
bool NormalCloseInAttackAI::Display()
{
	// ����ͷ���
	CMoveShape* pMoveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_pSkill->GetSourType(), m_pSkill->GetSourID()));
	if(pMoveshape)
	{
		if(m_pSkill->GetCurStep() == CSkillXml::STEP_START && m_pSkill->GetIntonateMainAct())
		{
			// ������Ϣ
			AnimInfo::tagActionInfo *pActionInfo = pMoveshape->GetAnimInfo()->GetActionInfo();

			// ������������׶Σ���ô��ǰ����������������֮��ѭ��������Ҫ����������
			if(pActionInfo &&
			   (pMoveshape->GetAction() == CShape::ACT_STAND ||
				pMoveshape->GetAction() == CShape::ACT_IDLE ||
				(pMoveshape->GetAction() >= CShape::ACT_FACEACTION && 
				pMoveshape->GetAction() <= CShape::ACT_FACEACTIONMAX)))		//��̬�������л� - add by lpf(2008.11.10)
			{
				pMoveshape->SetAction(m_pSkill->GetIntonateMainAct());
				pActionInfo->bCurActionLooped = TRUE;
			}
		}
		// �ͷŽ׶�
		else if(m_pSkill->GetCurStep() == CSkillXml::STEP_RUN && m_pSkill->GetRunStepActExQueue())
		{
			// �ͷŽ׶��������б�Ϊ�գ��ҵ�ǰ������վ��������ǰ��������Ϊ���������б�ĵ�һ������
			if(!m_pSkill->GetRunStepActExQueue()->empty() && pMoveshape->GetAction() == CShape::ACT_STAND)
			{
				pMoveshape->SetAction(m_pSkill->GetRunStepActExQueue()->front());
				m_pSkill->GetRunStepActExQueue()->pop();
				// �Ѿ������һ������
				if(m_pSkill->GetRunStepActExQueue()->empty())
				{
					AnimInfo::tagActionInfo *pActionInfo = pMoveshape->GetAnimInfo()->GetActionInfo();
					if(pActionInfo)
						pActionInfo->bCurActionLooped = m_pSkill->IsActExCycle();
				}
			}
		}
	}
	return true;
}
bool NormalCloseInAttackAI::StepBeginAI(vector<long> vecType, vector<CGUID> vecID)
{
	return true;
}
bool NormalCloseInAttackAI::StepRunAI(vector<long> vecType, vector<CGUID> vecID)
{
	return true;
}
bool NormalCloseInAttackAI::StepEndAI()
{
	if(m_pSkill)
	{
		CMoveShape* pMoveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_pSkill->GetSourType(), m_pSkill->GetSourID()));
		if(pMoveshape)
		{
			AnimInfo::tagActionInfo *pActionInfo = pMoveshape->GetAnimInfo()->GetActionInfo();
			if(pActionInfo && pMoveshape->GetAction() >= CShape::ACT_PERFORM)
			{
				pActionInfo->bCurActionLooped = FALSE;
			}
		}
		if (pMoveshape->GetType() == TYPE_PET)
		{
			CPet *pPet = (CPet*)pMoveshape;
			if (pPet->IsMainPlayerPet())
			{
				tagPetSkillInfo *pPetSkill = pPet->GetPetSkillInfoByID(m_pSkill->GetSkillID());
				if(pPetSkill)
					pPetSkill->bIsSkillUsingEnd = true;
				//pPet->SkillSuccReceived();
			}
		}
		// ɾ����Ч
		m_pSkill->DelAllEffectListByStep(CSkillXml::STEP_START);
		m_pSkill->DelAllEffectListByStep(CSkillXml::STEP_RUN);
		m_pSkill->DelAllEffectListByStep(CSkillXml::STEP_END);
		m_pSkill->SetIsDeleted(true);
		return true;
	}
	return false;
}
