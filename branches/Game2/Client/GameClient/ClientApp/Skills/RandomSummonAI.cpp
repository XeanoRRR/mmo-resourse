#include "stdafx.h"
#include "RandomSummonAI.h"
#include "../../EngineExtend\DisplayModel.h"
#include "PetSystem\Pet.h"
#include "SkillXml.h"
#include "ClientRegion.h"
#include "../GameClient/Game.h"
#include "Other/AudioList.h"
#include "../ClientApp/Player.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRandomSummonAI::CRandomSummonAI()
{
	m_pSkill = NULL;
	m_dwLifeTime = 0;
	m_dwTimeStamp = 0;
	m_dwTotalTime = 0;
	m_lCurShotPoint = 0;
	m_lPreSkillTime = 0;
	m_lDeleTime = 0;
}
CRandomSummonAI::CRandomSummonAI(CSkillXml *pSkill)
{
	m_pSkill = pSkill;
	m_vecSummonEffect = m_pSkill->GetSummonEffect();
	m_vecSummonSound = m_pSkill->GetSummonSound();
	m_dwLifeTime = 0;
	m_dwTimeStamp = 0;
	m_dwTotalTime = 0;
	m_lCurShotPoint = 0;
	m_lPreSkillTime = 0;
	m_lDeleTime = 0;
}
CRandomSummonAI::~CRandomSummonAI()
{
	m_vecShotPoint.clear();
	m_vecSummonEffect.clear();
	m_vecSummonSound.clear();
}

void CRandomSummonAI::AI()
{
	if(GetRemainedTime() == 0)
	{
		StepEndAI();
		return;
	}
	if(m_vecShotPoint.size() <= 0 || m_vecSummonEffect.empty())
		return;
	static size_t index = 0;
	long lPassTime = timeGetTime() - m_dwTimeStamp;
	if(lPassTime < m_lDeleTime)
		return;
	m_lDeleTime = 0;
	if(m_lCurShotPoint < (int)m_vecShotPoint.size() && lPassTime >= m_lPreSkillTime*m_lCurShotPoint)
	{
		//m_dwTimeStamp = GetCurTickCount();
		float x = m_vecShotPoint[m_lCurShotPoint].lX + 0.5f;
		float y = m_vecShotPoint[m_lCurShotPoint].lY + 0.5f;
		CClientRegion* m_pRegion = GetGame()->GetRegion();
		if(index >= m_vecSummonEffect.size())
			index = 0;
		char strFile[MAX_PATH];
		bool bAsyn = true;
		// �Լ��Ǽ����ͷ���
		CMoveShape* pMoveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_pSkill->GetSourType(), m_pSkill->GetSourID()));
		if(GetGame()->GetMainPlayer() == pMoveshape)
			bAsyn = false;
		GetGame()->GetRegion()->PlayEffect(m_vecSummonEffect[index], x, y,m_pRegion->GetCellHeight(x, y),-1,bAsyn);
		sprintf(strFile, "sounds/skills/%d.wav", m_vecSummonSound[index]);
		GetGame()->GetAudioList()->Play(strFile, x, y, m_pRegion->GetCellHeight(x, y));
		index++;
		m_lCurShotPoint++;
	}
}

bool CRandomSummonAI::Display()
{
	// ����ͷ���
	CMoveShape* pMoveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_pSkill->GetSourType(), m_pSkill->GetSourID()));
	if(pMoveshape)
	{
		if(m_pSkill && m_pSkill->GetCurStep() == CSkillXml::STEP_START && m_pSkill->GetIntonateMainAct())
		{
			// ������������׶Σ���ô��ǰ����������������֮��ѭ��������Ҫ����������
				AnimInfo::tagActionInfo *pActionInfo = pMoveshape->GetAnimInfo()->GetActionInfo();
				if(pMoveshape && pActionInfo &&
					(pMoveshape->GetAction() == CShape::ACT_IDLE ||
					 pMoveshape->GetAction() == CShape::ACT_STAND))
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
bool CRandomSummonAI::StepBeginAI(vector<long> vecType, vector<CGUID> vecID)
{
	// �ڸü��ܿ�ʼʱ������������ʣ��ʱ��
	m_dwTotalTime	= m_pSkill->GetIntonateTime();
	m_dwTimeStamp	= GetCurTickCount();
	m_dwLifeTime	= m_dwTotalTime;
	return true;
}
bool CRandomSummonAI::StepRunAI(vector<long> vecType, vector<CGUID> vecID)
{
	m_vecSummonEffect = m_pSkill->GetSummonEffect();
	m_vecSummonSound = m_pSkill->GetSummonSound();
	m_dwTotalTime	= m_pSkill->GetSummonLifeTime();
	m_lDeleTime = m_pSkill->GetRandomSummonDelayTime();
	m_dwTimeStamp	= GetCurTickCount();	// �ٻ���ʼ��ʱ��
	m_dwLifeTime = m_dwTotalTime+m_lDeleTime;

	DWORD dwSize = m_pSkill->GetRandomSummonNum();
	int LengthX = (int)m_pSkill->GetRandomSummonX();
	int LengthY = (int)m_pSkill->GetRandomSummonY();
	tagCell pCells;
	for (size_t i = 0; i<dwSize; i++)
	{
		int tempX = random(LengthX);
		int tempY = random(LengthY);
		pCells.lX = m_pSkill->GetDestX() +  (tempX %2 == 1? tempX : -tempX);
		pCells.lY = m_pSkill->GetDestY() +  (tempY %2 == 1? tempY : -tempY);
		m_vecShotPoint.push_back(pCells);
	}
	// ƽ��ÿ����Ч�ͷŵ�ʱ����
	if(m_vecShotPoint.size() > 0)
		m_lPreSkillTime = m_dwTotalTime/m_vecShotPoint.size();
	else
		m_lPreSkillTime = m_dwTotalTime;
	
	return true;
}
bool CRandomSummonAI::StepEndAI()
{
	if(!m_pSkill)
		return false;
	CMoveShape* pMoveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_pSkill->GetSourType(), m_pSkill->GetSourID()));
	if(pMoveshape == GetGame()->GetMainPlayer())
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
		}
	}
	// ɾ���ü���ÿһ���׶Σ��ڳ����ϵ�������Ч
	m_pSkill->DelAllEffectListByStep(CSkillXml::STEP_START);
	m_pSkill->DelAllEffectListByStep(CSkillXml::STEP_RUN);
	m_pSkill->DelAllEffectListByStep(CSkillXml::STEP_END);
	m_pSkill->SetIsDeleted(true);
	return true;
}
DWORD CRandomSummonAI::GetRemainedTime()
{
	if (GetCurTickCount() >= m_dwTimeStamp + m_dwLifeTime)
		return 0;
	else
		return m_dwTimeStamp + m_dwLifeTime - GetCurTickCount();
}

