#include "StdAfx.h"
#include "SkillXml.h"
#include "CityGate.h"
#include "Other/WeaponEffectXml.h"
#include "Player.h"
#include "PetSystem/Pet.h"
#include "Monster.h"
#include "ClientRegion.h"
#include "Other/AudioList.h"
#include "Effect.h"
#include "../GameClient/GameControl.h"
#include "../../Public/Common/SkillRelated.h"
#include "../GameClient/Game.h"
#include "../../Input/KeyBoard.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSkillXml::CSkillXml()
{
	SetType(TYPE_SKILL);
	m_pLauncher = NULL;
	m_pDestShape = NULL;
	m_pMoveShape = NULL;

	m_lStep = -1;

	m_lSourType = 0;
	m_lDestX = 0;
	m_lDestY = 0;
	m_lSkillID = 0;
	m_lSkillLevel = 0;

	m_pSkillAI = NULL;

	m_fDir = 0.0f;

	//m_lStarEquipId = 0;
	//m_lStarEquipPlace = 0;

	m_lItemId =0;
	m_lItemNum = 0;
	m_lItemPlace = 0;

	m_lIntonateTime = 0;
	m_wIntonateMainAct = 0;

	m_lIntonateEffect = 0;
	m_lIntonateEffectIndex = 0;
	m_bIntonateEffectByEquip = false;
	m_bIntonateEffectCycle = 0;
	m_eIntonateEffectPosition = LOCATOR_FOOTCENTER;

	m_lIntonateSound = 0;
	m_bIntonateSoundByEquip = false;
	m_bIntonateSoundCycle = false;
	m_lReleaseSoundDest = 0;
	m_IntonateSoundChannel = 0;

	m_lReleaseEffect = 0;
	m_lReleaseEffectIndex = 0;
	m_bReleaseEffectByEquip = false;
	m_lReleaseEffectDest = 0;
	m_eReleaseEffectPosition = LOCATOR_FOOTCENTER;
	m_bReleaseEffectCycle = false;

	m_lReleaseSound = 0;
	m_bReleaseSoundByEquip = false;
	m_bReleaseSoundCycle = false;
	m_lReleaseSoundDest = 0;

	m_fShockRatio = 0.0f;
	m_lShockInterval = 0;
	m_lShockTimeLength = 0;

	m_lFlyEffectNum = 0;
	m_lFlyEffect = 0;
	m_lFlyEffectIndex = 0;
	m_bFlyEffectByEquip = false;
	m_eFlyEffectPosition = LOCATOR_FOOTCENTER;
	m_bFlyEffectCycle = false;
	m_lFlyEffectTurnX = 0;
	m_lFlyEffectTurnY = 0;

	m_lFlySound = 0;
	m_bFlySoundByEquip = false;
	m_bFlySoundCycle = false;
	m_eFlySoundPosition = LOCATOR_FOOTCENTER;

	m_lSummonLifeTime = 0;				// �ٻ���������
	m_lRandomSummonX = 0;				// ����ٻ�����X�᷶Χ
	m_lRandomSummonY = 0;				// ����ٻ�����Y�᷶Χ
	m_lRandomSummonNum = 0;				// ����ٻ������ٻ�����
	m_lRandomSummonDelayTime = 0;		// �漴�ٻ������ٻ�����ӳٲ���ʱ��

	m_lDelLocEffect = 0;
	m_lDelLocEffectIndex = 0;

	m_lCycleEffectDelayTime = 0;

	m_bIsLastActExCycle = FALSE;

	m_com = NULL;
}

CSkillXml::CSkillXml(SkillComponent* com):
m_com(com)
{
	SetType(TYPE_SKILL);
	m_pLauncher = NULL;
	m_pDestShape = NULL;
	m_pMoveShape = NULL;

	m_lStep = -1;

	m_lSourType = 0;
	m_lDestX = 0;
	m_lDestY = 0;
	m_lSkillID = 0;
	m_lSkillLevel = 0;

	m_pSkillAI = NULL;

	m_fDir = 0.0f;

	//m_lStarEquipId = 0;
	//m_lStarEquipPlace = 0;

	m_lItemId =0;
	m_lItemNum = 0;
	m_lItemPlace = 0;

	m_lIntonateTime = 0;
	m_wIntonateMainAct = 0;

	m_lIntonateEffect = 0;
	m_lIntonateEffectIndex = 0;
	m_bIntonateEffectByEquip = false;
	m_bIntonateEffectCycle = 0;
	m_eIntonateEffectPosition = LOCATOR_FOOTCENTER;

	m_lIntonateSound = 0;
	m_bIntonateSoundByEquip = false;
	m_bIntonateSoundCycle = false;
	m_lReleaseSoundDest = 0;
	m_IntonateSoundChannel = 0;

	m_lReleaseEffect = 0;
	m_lReleaseEffectIndex = 0;
	m_bReleaseEffectByEquip = false;
	m_lReleaseEffectDest = 0;
	m_eReleaseEffectPosition = LOCATOR_FOOTCENTER;
	m_bReleaseEffectCycle = false;

	m_lReleaseSound = 0;
	m_bReleaseSoundByEquip = false;
	m_bReleaseSoundCycle = false;
	m_lReleaseSoundDest = 0;

	m_fShockRatio = 0.0f;
	m_lShockInterval = 0;
	m_lShockTimeLength = 0;

	m_lFlyEffectNum = 0;
	m_lFlyEffect = 0;
	m_lFlyEffectIndex = 0;
	m_bFlyEffectByEquip = false;
	m_eFlyEffectPosition = LOCATOR_FOOTCENTER;
	m_bFlyEffectCycle = false;
	m_lFlyEffectTurnX = 0;
	m_lFlyEffectTurnY = 0;

	m_lFlySound = 0;
	m_bFlySoundByEquip = false;
	m_bFlySoundCycle = false;
	m_eFlySoundPosition = LOCATOR_FOOTCENTER;

	m_lSummonLifeTime = 0;				// �ٻ���������
	m_lRandomSummonX = 0;				// ����ٻ�����X�᷶Χ
	m_lRandomSummonY = 0;				// ����ٻ�����Y�᷶Χ
	m_lRandomSummonNum = 0;				// ����ٻ������ٻ�����
	m_lRandomSummonDelayTime = 0;		// �漴�ٻ������ٻ�����ӳٲ���ʱ��

	m_lDelLocEffect = 0;
	m_lDelLocEffectIndex = 0;

	m_lCycleEffectDelayTime = 0;

	m_bIsLastActExCycle = FALSE;
}
CSkillXml::~CSkillXml()
{
	// ɾ�����ж����������Ч
	DelAllEffectListByStep(STEP_START);
	DelAllEffectListByStep(STEP_RUN);
	DelAllEffectListByStep(STEP_END);
	m_vecFlyEffect.clear();
	m_vecFlyEffectIndex.clear();
	m_vecFlyEffectPos.clear();
	m_vecFlyTime.clear();
	m_vecFlyDelay.clear();
	m_vecSummonEffect.clear();
	m_vecSummonSound.clear();
	m_vecDestMoveShape.clear();
	m_vecFlyEffectTurnX.clear();
	m_vecFlyEffectTurnY.clear();
	m_vecStarEquipId.clear();
	m_vecStarEquipPlace.clear();
	//////////////////////////////////////////////////////////////////////////
	// �����ٻ�����
	/*if(m_pSkillAI)
		m_pSkillAI->SetIsDeleted(true);*/
	//////////////////////////////////////////////////////////////////////////
	SAFE_DELETE(m_pSkillAI);
	SAFE_DELETE(m_com);
}
void CSkillXml::AI()
{
	CShape::AI();
	if(m_pSkillAI )
		m_pSkillAI->AI();
}
bool CSkillXml::Display()
{
	CShape::Display();
	if(m_pSkillAI)
		m_pSkillAI->Display();
	return true;
}
// ִ�����м��ܲ���
BOOL CSkillXml::ExecuteStep(int iWordKey, long lValue, const string& strValue)
{
	BOOL bChecked = TRUE;
	switch(iWordKey)
	{
	case SKILL_OBJECT				: (SkillObject(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case SKILL_FORCE_OBJECT			: (SkillForceObject(lValue))?bChecked = TRUE : bChecked = FALSE; break;
	case FIRS_TATT					: (FirstAtt(lValue))?bChecked = TRUE :bChecked = FALSE;; break;
	case SKILL_STAR_BUFF			: (SkillStarBuff(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case SKILL_STAR_STATE			: (SkillStarState(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case SKILL_STAR_EQUIP_ID		: SkillStarEquipId(lValue); break;
	case SKILL_STAR_EQUIP_PLACE		: SkillStarEquipPlace(lValue); break;
	case SKILL_STAR_EQUIP_USE		: (SkillStarEquipUse())?bChecked = TRUE :bChecked = FALSE; break;
	case SKILL_ITEM_ID				: SkillItemId(lValue); break;
	case SKILL_ITEM_NUM				: SkillItemNum(lValue); break;
	case SKILL_ITEM_PLACE			: SkillItemPlace(lValue); break;
	case SKILL_USE_ITEM				: (SkillUseItem())?bChecked = TRUE :bChecked = FALSE; break;
	case SKILL_HP					: (SkillHp(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case SKILL_MP					: (SkillMp(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case SKILL_ENERGY				: (SkillEnergy(lValue))? bChecked = TRUE :bChecked = FALSE; break;
	case INTONATE_LEAST_SPACE		: (IntonateLeastSpace(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case INTONATE_MOST_SPACE		: (IntonateMostSpace(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case FIRE_LEAST_SPACE			: (FireLeastSpace(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case FIRE_MOST_SPACE			: (FireMostSpace(lValue))?bChecked = TRUE :bChecked = FALSE; break;
	case INTONATE_ACT				: IntonateAct(lValue); break;
	case INTONATE_EFFECT			: IntonateEffect(lValue); break;
	case INTONATE_EFFECT_INDEX		: IntonateEffectIndex(lValue); break;
	case INTONATE_EFFECT_BY_EQUIP	: IntonateEffectByEquip(lValue); break;
	case INTONATE_EFFECT_POSITION	: IntonateEffectPosition(lValue); break;
	case INTONATE_EFFECT_CYCLE		: IntonateEffectCycle(lValue); break;
	case INTONATE_PLAY				: IntonatePlay(); break;
	case INTONATE_SOUND				: IntonateSound(lValue); break;
	case INTONATE_SOUND_BY_EQUIP	: IntonateSoundByEquip(lValue); break;
	case INTONATE_SOUND_CYCLE		: IntonateSoundCycle(lValue); break;
	case INTONATE_SOUND_PLAY		: IntonateSoundPlay(); break;
	case INTONATE_BREAK				: IntonateBreak(lValue); break;
	case RELEASE_ACT				: ReleaseAct(lValue); break;
	case RELEASE_EFFECT				: ReleaseEffect(lValue); break;
	case RELEASE_EFFECT_INDEX		: ReleaseEffectIndex(lValue); break;
	case RELEASE_EFECT_BY_EQUIP		: ReleaseEffectByEquip(lValue); break;
	case RELEASE_EFFECT_DEST		: ReleaseEffectDest(lValue); break;
	case RELEASE_EFFECT_POSITION	: ReleaseEffectPosition(lValue); break;
	case RELEASE_EFFECT_CYCLE		: ReleaseEffectCycle(lValue); break;
	case RELEASE_PLAY				: ReleasePlay(); break;
	case RELEASE_SOUND				: ReleaseSound(lValue); break;
	case RELEASE_SOUND_BY_EQUIP		: ReleaseSoundByEquip(lValue); break;
	case RELEASE_SOUND_CYCLE		: ReleaseSoundCycle(lValue); break;
	case RELEASE_SOUND_DEST			: ReleaseSoundDest(lValue); break;
	case RELEASE_SOUND_PLAY			: ReleaseSoundPlay();break;
	case SHOCK_RATIO				: ShockRatio(lValue); break;
	case SHOCK_INTERVAL				: ShockInterval(lValue); break;
	case SHOCK_TIME_LENGTH			: ShockTimeLength(lValue); break;
	case SHOCK_PLAY					: ShockPlay(); break;
	case FLY_EFFECT					: FlyEffect(lValue); break;
	case FLY_EFFECT_INDEX			: FlyEffectIndex(lValue); break;
	case FLY_EFFECT_BY_EQUIP		: FlyEffectByEquip(lValue); break;
	case FLY_EFFECT_POSITION		: FlyEffectPosition(lValue); break;
	case FLY_EFFECT_CYCLE			: FlyEffectCycle(lValue); break;
	case FLY_EFFECT_TURNX			: FlyEffectTurnX(lValue); break;
	case FLY_EFFECT_TURNY			: FlyEffectTurnY(lValue); break;
	case FLY_PLAY					: FlyPlay(); break;
	case FLY_SOUND					: FlySound(lValue); break;
	case FLY_SOUND_BY_EQUIP			: FlySoundByEquip(lValue); break;
	case FLY_SOUND_POSITION			: FlySoundPosition(lValue); break;
	case FLY_SOUND_CYCLE			: FlySoundCycle(lValue); break;
	case FLY_SOUND_PLAY				: FlySoundPlay(); break;
	case FLY_TIME					: FlyTime(lValue); break;
	case FLY_DELAY					: FlyDelay(lValue); break;
	case DEL_LOC_EFFECT				: DelLocEffect(lValue); break;
	case DEL_LOC_EFFECT_INDEX		: DelLocEffectIndex(lValue); break;
	case START_DEL_EFFECT			: StartDelEffect(lValue); break;
	case SUMMON_EFFECT				: SummonEffect(lValue); break;
	case SUMMON_SOUND				: SummonSound(lValue); break;
	case SUMMON_LIFETIME			: SummonLifeTime(lValue); break;
	case RANDOM_SUMMONX				: RandomSummonX(lValue); break;
	case RANDOM_SUMMONY				: RandomSummonY(lValue); break;
	case RANDOM_SUMMON_NUM			: RandomSummonNum(lValue); break;
	case INTONATE_TIME				: IntonateTime(lValue);	break;					// ����ʱ��
	case FLY_EFFECT_NUM				: FlyEffectNum(lValue);	break;					// ������Ч����
	case INTONATE_MAINACT			: IntonateMainAct((WORD)lValue); break;				// �����׶���Ҫѭ�����ŵ���Ҫ����(74)
	case SUMMON_DELAYTIMA			: RandomSummonDelayTime(lValue); break;			// ����ٻ����ӳٲ���ʱ�䣨75��
	case WEAPONEFFECT				: WeaponEffect(lValue); break;					// �ͷż���ʱ���Ƿ񲥷�������Ч�ı�ʶ��76��
	case CHANGEPROPERTY				: ChangeProperty(lValue,strValue); break;		// �����ͷ��ߵ�ĳ������(77)
	case ACTTIME					: SetActTime((DWORD)lValue); break;				// ���õ�ǰ�����Ĺ涨ʱ��(78)
	case RELEASE_ACTEX				: ReleaseActEx(lValue); break;					// �����ͷŽ׶εĺ���������79��
	case DELAY_CYCELEFFECT_TIME		: SetCycleEffectDelayTime(lValue); break;		// ����ѭ����Ч�ӳ�ɾ��ʱ�䣨80��
	case LAST_ACTEX_ISCYCEL			: SetActExIsCycle(lValue>0?TRUE:FALSE);	break;	// �������һ���ͷŽ׶εĺ�̶����Ƿ�ѭ������(81)
	default:break;
	}
	return bChecked;
}
// �ж���Щְҵ����ʹ�õ�ǰ����
BOOL CSkillXml::SkillOccType(CSkillListXml::tagSkill *pSkill, CMoveShape *sShape)
{
	if (pSkill->vecUseByOcc.size()== 0)	// ���û��ְҵ�ж���ֱ�ӷ���
		return TRUE;
	int iCounts = 0;
	vector<CSkillListXml::tagSkillOcc>::iterator it;
	for (it = pSkill->vecUseByOcc.begin(); it!=pSkill->vecUseByOcc.end(); it++)
	{
		if(sShape->GetType() == TYPE_PLAYER)
		{
			if(((CPlayer*)sShape)->GetOccupation() == it->dwOccId)
			iCounts++;
		}
		else if(sShape->GetType() == TYPE_PET)
		{
			if(((CPet*)sShape)->GetPetType() == it->dwOccId)
				iCounts++;
		}
	}
	if (iCounts>0)
		return TRUE;
	else
		return FALSE;

}
//////////////////////////////////////////////////////////////////////////
// ִ�з��ͼ�����Ϣ����
//////////////////////////////////////////////////////////////////////////
bool CSkillXml::StepSend(DWORD dwID,DWORD dwLevel,
						 CMoveShape *sShape, CMoveShape *dShape, 
						 long dX, long dY)
{
	int iCount = 0;	// ����������
	bool bFlag = false;
	m_pLauncher = sShape;
	m_pDestShape = dShape;
	m_lDestX = dX;
	m_lDestY = dY;
	m_lSkillID = dwID;
	m_lSkillLevel = dwLevel;
	CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(dwID);
	if(NULL == pSkill)
		return FALSE;

	if(SkillOccType(pSkill, sShape) == FALSE)	// �ж�ְҵ����
		return FALSE;
	// ����Ŀ�꼼�ܣ�Ŀ���������Ҳ��ܶ�����Ŀ�귢�ͼ���
	if(dShape && dShape->IsDied() && pSkill->bLockTarget && !pSkill->bIsUseToDiedObj)
	{
		/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
															CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_OBJDIED),
															D3DCOLOR_ARGB(255,255,0,0));*/
		return FALSE;
	}
	DWORD dwlev = pSkill->vectorLevels.size() >= dwLevel? dwLevel - 1:(pSkill->vectorLevels.size() - 1);
	vector<CSkillListXml::tagLevelValue> vectSend = pSkill->vectorLevels[dwlev].vecStepSend;
	vector<CSkillListXml::tagLevelValue>::iterator it;
	for(it = vectSend.begin(); it!= vectSend.end(); it++)
	{
		if(strcmp(it->strKeyWord.c_str(),"SkillObject")==0 || strcmp(it->strKeyWord.c_str(),"SkillForceObject")==0)
		{
			bFlag = true;
			// ����Ŀ��������û��ɣ�����������һ������
			if(iCount == 0)
			{
				long lID				= it->dwID;
				long lValue		= it->dwValue;
				string strValue	= it->strValue;
				if(ExecuteStep(lID, lValue, strValue))	// �������Ŀ����������������1
					iCount ++;
			}
			// ������������������
			else
				continue;
		}
		else 
		{
			if(ExecuteStep(it->dwID, it->dwValue, it->strValue))	// ʣ�µ�����ֻҪһ���������ͷ���
				continue;
			else
				return FALSE;
		}
		
	}
	if(iCount>0 && bFlag)							// �������������0��ʾĿ��������һ����������
		return TRUE;
	// ��ʹ�ö����趨
	else if(!bFlag)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_OBJERROR),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}
// ���鼼��ʩ�Ŷ���
BOOL CSkillXml::SkillObject(long l)
{
	switch(l)
	{
	case 1:		// ����
		if(m_pDestShape && m_pDestShape == m_pLauncher)
			return TRUE;
		break;
	case 2:		// ���� ���� �г�
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape != m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;

			if(pLauncher && (pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == true ||
				pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == true))
			{
				return true;
			}
		}
		break;
	case 3:		// ����
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER)
		{
			CMonster* pDestMonster = dynamic_cast<CMonster *>(m_pDestShape);
			if(!pDestMonster->IsDied() && pDestMonster->GeteNpcType() != NT_Normal)
				return TRUE;
		}
		break;

    // �������ע�� Bugs 2008-9-12
    //case 4:		// ����
	//	if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER)
	//	{
	//		CMonster* pDestPet = dynamic_cast<CMonster*>(m_pDestShape);
	//		if(pDestPet->GetPetInfo()/* && pDestPet->IsDied() != true*/)
	//			return TRUE;
	//	}
	//	break;
	case 5:		// NPC
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER)
		{
			CMonster* pDestNpc = dynamic_cast<CMonster*>(m_pDestShape);
			if(pDestNpc->GeteNpcType() == NT_Normal/* && pDestNpc->IsDied()!= true*/)
				return TRUE;
		}
		break;
	case 6:		// �������S_MONSTER
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER)
		{
			CMonster *pDestMonster = dynamic_cast<CMonster*>(m_pDestShape);
			if(pDestMonster->GeteNpcType() == NT_Monster/* && pDestMonster->IsDied()!=true*/)
				return TRUE;
		}
		break;
	case 7:		// ��������
		//if(m_pDestShape == NULL && m_lDestX>=0 && m_lDestY>=0)
		return TRUE;
	case 8:		// ����������
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_PLAYER && m_pDestShape != m_pLauncher)
		{
			CPlayer* pDestPlayer = dynamic_cast<CPlayer *>(m_pDestShape);
			if(pDestPlayer->GetPVPValue() > 0)
				return TRUE;
		}
		break;
	case 9:		// ����
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_CITYGATE)
		{
			CCityGate* pDestCityGate = dynamic_cast<CCityGate *>(m_pDestShape);
			if(pDestCityGate->IsDied()!=true&&pDestCityGate->GetSwitch()!=1)  //����û�лٻ���û���޵�
			    return TRUE;
		}
		break;
	case 31:	// ����������һ��߳���
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape!= m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;

			if(pLauncher && (pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == false||
					pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == false)/* && pDestPlayer->IsDied() != true*/
					&& pDestPlayer->GetPkValue() == 0)
					return TRUE;
		}
		break;
	case 32:	// ����������� ���߳���
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape!= m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;

			if((pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == false||
				pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == false)/* && pDestPlayer->IsDied() != true*/
				&& pDestPlayer->GetPkValue() > 0)
				return TRUE;
		}
		break;
	case 33:	// ����������� ���߳���
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape!= m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;

			if((pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == false||
				pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == false)/* && pDestPlayer->IsDied() != true*/
				&& pDestPlayer->GetPkValue() > 1000)
				return TRUE;
		}
		break;
	}
	return FALSE;
}
// ���鼼��ǿ��ʩ�Ŷ���
BOOL CSkillXml::SkillForceObject(long l)
{
	// ����Ctrlǿ�ƹ���
	if(!GetKeyboard()->IsKeyDown(DIK_LCONTROL) && !GetKeyboard()->IsKeyDown(DIK_RCONTROL))
		return FALSE;
	switch(l)
	{
	case 1:		// ����
		if(m_pDestShape && m_pDestShape == m_pLauncher)
			return TRUE;
		break;
	case 2:		// ���� ���� ����
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape != m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;
			if(pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == true||
				pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == true/* && pDestPlayer->IsDied() != true*/)
				return TRUE;
		}
		break;
	case 3:		// ����
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER)
		{
			CMonster* pDestMonster = dynamic_cast<CMonster *>(m_pDestShape);
			if(!pDestMonster->IsDied() && pDestMonster->GeteNpcType() != NT_Normal)
				return TRUE;
		}
		break;
	//case 4:		// ����
	//	if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER)
	//	{
	//		CMonster* pDestPet = dynamic_cast<CMonster*>(m_pDestShape);
	//		if(pDestPet->GetPetInfo() && pDestPet->IsDied() != true)
	//			return TRUE;
	//	}
	//	break;
	case 5:		// NPC
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER )
		{
			CMonster* pDestNpc = dynamic_cast<CMonster*>(m_pDestShape);
			if(pDestNpc->GeteNpcType() == NT_Normal/* && pDestNpc->IsDied()!= true*/)
				return TRUE;
		}
		break;
	case 6:		// �������S_MONSTER
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_MONSTER)
		{
			CMonster *pDestMonster = dynamic_cast<CMonster*>(m_pDestShape);
			if(pDestMonster->GeteNpcType() == NT_Monster/* && pDestMonster->IsDied()!=true*/)
				return TRUE;
		}
		break;
	case 7:		// ��������
		//if(m_pDestShape == NULL && m_lDestX>=0 && m_lDestY>=0)
		return TRUE;
	case 8:		// ����������
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_PLAYER && m_pDestShape != m_pLauncher)
		{
			CPlayer* pDestPlayer = dynamic_cast<CPlayer *>(m_pDestShape);
			if(pDestPlayer->GetPVPValue() > 0)
				return TRUE;
		}
		break;
	case 9:		// ����
		if(m_pDestShape && m_pDestShape->GetType() == TYPE_CITYGATE)
		{
			CCityGate* pDestCityGate = dynamic_cast<CCityGate *>(m_pDestShape);
			if(pDestCityGate->IsDied()!=true&&pDestCityGate->GetSwitch()!=1)  //����û�лٻ���û���޵�
			    return TRUE;
		}
		break;
	case 31:	// ����������� ���� ����
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape!= m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;

			if((pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == false||
				pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == false)/* && pDestPlayer->IsDied() != true*/
				&& pDestPlayer->GetPkValue() == 0)
				return TRUE;
		}
		break;
	case 32:	// �����������
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape!= m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;

			if((pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == false||
				pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == false)/* && pDestPlayer->IsDied() != true*/
				&& pDestPlayer->GetPkValue() > 0)
				return TRUE;
		}
		break;
	case 33:	// �����������
		if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape!= m_pLauncher)
		{
			CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
			if(pDestPlayer == NULL)		return false;
			CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
			if(pLauncher == NULL)	return false;

			if((pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == false||
				pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == false)/* && pDestPlayer->IsDied() != true*/
				&& pDestPlayer->GetPkValue() > 1000)
				return TRUE;
		}
		break;
	}
	return FALSE;
}
// �Ƿ���Զ�������һ��߳���ʹ�ü���
BOOL CSkillXml::FirstAtt(long l)
{
	bool bAtt = (l==0)? false:true;
	bool bIsNormal = false;
	if(m_pDestShape && (m_pDestShape->GetType() == TYPE_PLAYER || m_pDestShape->GetType() == TYPE_PET) && m_pDestShape!= m_pLauncher)
	{
		CPlayer* pDestPlayer = ConvertFromPetOrPlayer(m_pDestShape);
		if(pDestPlayer == NULL)		return false;
		CPlayer* pLauncher =  ConvertFromPetOrPlayer(m_pLauncher);
		if(pLauncher == NULL)	return false;

		if((pLauncher->IsCityWarEnemyFaction(pDestPlayer->GetFactionID()) == false||
			pLauncher->IsEnemyFaction(pDestPlayer->GetFactionID()) == false) &&
			/*pDestPlayer->IsDied() != true && */pDestPlayer->GetPkValue() == 0)
			bIsNormal = true;
		if(bIsNormal && bAtt)
			return TRUE;
		else
			return FALSE;
	}
	return TRUE;
}
// �ж�Ҫʩ�ż��������buff
BOOL CSkillXml::SkillStarBuff(long l)
{
	if(m_pLauncher->IsHaveStateEx(l))
		return TRUE;
	else
		return FALSE;
}
// �ж�ʩ�ż���ʱ��ұ��봦�ں�����Ϸ״̬
BOOL CSkillXml::SkillStarState(long l)
{
	if(m_pLauncher->GetState() == l)
		return TRUE;
	else
		return FALSE;
}
// ���Ҫʩ�ż�������װ����id
void CSkillXml::SkillStarEquipId(long l)
{
	m_vecStarEquipId.push_back(l);
}
// ���Ҫʩ�ż�������װ������ںδ�
void CSkillXml::SkillStarEquipPlace(long l)
{
	 m_vecStarEquipPlace.push_back(l);
}
// Ҫʩ�ż��ܱ����װ������
BOOL CSkillXml::SkillStarEquipUse()
{
	BOOL bFlag = FALSE;
	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	if(m_vecStarEquipId.size() == m_vecStarEquipPlace.size() && pPlayer)
	{
		for (size_t i = 0; i<m_vecStarEquipId.size(); ++i)
		{
			//// ����
			//if(m_vecStarEquipId[i] == -1)
			//{
			//	CGoods *pGoods = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON2);
			//	if(pGoods)
			//	{
			//		CGoodsList::tagBaseProperty* pBPro = pGoods->GetProperty();
			//		if(pBPro && pBPro->dwType == GT_WEAPON2)
			//		{
			//			bFlag = TRUE;
			//			break;
			//		}
			//	}
			//}
			//else 
            if(m_pLauncher->GetType() == TYPE_PLAYER && 
                ((CPlayer*)m_pLauncher)->FindAppointGoods(1,m_vecStarEquipId[i],0,m_vecStarEquipPlace[i]))
			{
				bFlag = TRUE;
				break;
			}
			else if(m_pLauncher->GetType() == TYPE_PET)		//���ﲻ�ж�
			{
				bFlag = TRUE;
				break;
			}
		}
	}
	//if(!bFlag)
	//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
	//														CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_ARM),
	//														D3DCOLOR_ARGB(255,255,0,0));
	return bFlag;
}
// ���ʩ�ż�������ĵ���
void CSkillXml::SkillItemId(long l)
{
	m_lItemId = l;
}
// ���ʩ�ż���������ߵ�����
void CSkillXml::SkillItemNum(long l)
{
	m_lItemNum = l;
}
// ���ʩ�ż���������߷��úδ�
void CSkillXml::SkillItemPlace(long l)
{
	m_lItemPlace = l;
}
// �ж�ʩ�ż��������������
BOOL CSkillXml::SkillUseItem()
{
	BOOL bFlag =false;
	if(m_pLauncher->GetType() == TYPE_PLAYER)
	{
		bFlag = ((CPlayer*)m_pLauncher)->FindAppointGoods(2,m_lItemId,m_lItemNum,m_lItemPlace);
		//if(!bFlag)
		//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//	CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_GOODS),
		//	D3DCOLOR_ARGB(255,255,0,0));
	}
	else if(m_pLauncher->GetType() == TYPE_PET)		//�����ݲ��ж�
		bFlag = true;
	return bFlag;
}
// �ж�ʩ�ż�������mp
BOOL CSkillXml::SkillHp(long l)
{
	if( (long)m_pLauncher->GetHp() >= l)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_HP),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}
// �ж�ʩ�ż�����������
BOOL CSkillXml::SkillMp(long l)
{
	long lMp = 0;
	if(m_pLauncher->GetType() == TYPE_PLAYER)
		lMp = ((CPlayer*)m_pLauncher)->GetMp();
	else if(m_pLauncher->GetType() == TYPE_PLAYER)
		lMp = ((CPet*)m_pLauncher)->GetMp();
	
	if(lMp >= l)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_MP),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}
// �ж�ʩ�ż�����������
BOOL CSkillXml::SkillEnergy(long l)
{
	long lEnergy = 0;
	if(m_pLauncher->GetType() == TYPE_PLAYER)
		lEnergy = ((CPlayer*)m_pLauncher)->GetEnergy();
	else if(m_pLauncher->GetType() == TYPE_PLAYER)
		lEnergy = 0;
	if(lEnergy>= l)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_NP),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}
// �ж�������Сʩ�ž���
BOOL CSkillXml::IntonateLeastSpace(long l)
{
	if(m_pDestShape && m_pLauncher->Distance(m_pDestShape) >=l)
		return TRUE;
	else if(m_pDestShape == NULL && m_pLauncher->RealDistance(m_lDestX,m_lDestY) >=l)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_INT_MINDEST),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}
// �ж��������ʩ�ž���
BOOL CSkillXml::IntonateMostSpace(long l)
{
	if(m_pDestShape && m_pLauncher->Distance(m_pDestShape) <= l)
		return TRUE;
	else if(m_pDestShape == NULL && m_pLauncher->RealDistance(m_lDestX,m_lDestY) <= l)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_INT_MAXDEST),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}
// �ж�ʩ����Сʩ�ž���
BOOL CSkillXml::FireLeastSpace(long l)
{
	long lDistance = 0;
	if(m_pDestShape )
		lDistance = m_pLauncher->Distance(m_pDestShape);
	else if(m_pDestShape == NULL )
		lDistance = m_pLauncher->RealDistance(m_lDestX,m_lDestY); 
	if(lDistance >= l)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_MINDEST),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}
// �ж�ʩ�����ʩ�ž���
BOOL CSkillXml::FireMostSpace(long l)
{
	long lDistance = 0;
	if(m_pDestShape )
		lDistance = m_pLauncher->Distance(m_pDestShape);

	else if(m_pDestShape == NULL )
		lDistance = m_pLauncher->RealDistance(m_lDestX,m_lDestY); 
	if(lDistance <= l)
		return TRUE;
	else
	{
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
		//													CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_MAXDEST),
		//													D3DCOLOR_ARGB(255,255,0,0));
		return FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
// ִ�м��ܿ�ʼ����
//////////////////////////////////////////////////////////////////////////
bool CSkillXml::StepBegin(long lID, long lLevel, 
						  long sType, const CGUID& sID,
						  long lDestX, long lDestY, float fDir,
						  vector<long> vType,
						  vector<CGUID> vID)
{
	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	m_lStep = 0;
	m_pMoveShape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(sType,sID));
	vector<long>::iterator itShape;
	int index = 0;
	if(m_vecDestMoveShape.size()>0)
		m_vecDestMoveShape.clear();
	for(itShape = vType.begin(); itShape!=vType.end(); itShape++)
	{
		CMoveShape* moveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject((*itShape),vID[index]));
		if(moveshape)
		{		
			m_vecDestMoveShape.push_back(moveshape);
			// ������������������񱣻���
			CMonster *pMonster = dynamic_cast<CMonster *>(moveshape);
			if(m_pMoveShape == pPlayer && pMonster && 
				pMonster->GetCreatorType() != 0 && pMonster->GetCreatorID() != CGUID::GUID_INVALID)
			{
				// ��������monster���ж��Ƿ�������
				if(pMonster->GetCreatorType() == TYPE_MONSTER)
				{
					CMonster* pCreater = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(pMonster->GetCreatorType(),pMonster->GetCreatorID()));
					if(pCreater && pCreater->GeteNpcType() == NT_Guard)
					{
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_ATTACK_QUESTOBJ),D3DCOLOR_ARGB(255,255,0,0),
							D3DCOLOR_ARGB(255,255,0,0));*/
					}
				}
				// �������뱾���û�����������ʾ��Ϣ
				else if((pMonster->GetCreatorType() == TYPE_PLAYER && pMonster->GetCreatorID() != pPlayer->GetExID())
					|| (pMonster->GetCreatorType() == TYPE_TEAM && pMonster->GetCreatorID() != pPlayer->GetTeamID()))
				{
					/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
																		CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_ATTACK_QUESTOBJ),D3DCOLOR_ARGB(255,255,0,0),
																		D3DCOLOR_ARGB(255,255,0,0));*/
				}
			}
		}
		index++;
	}
	/*if(m_vecDestMoveShape.size() == 0)
		return FALSE;*/
	m_lSourType = sType;
	m_SourID = sID;
	m_lDestType = (vType.size()>0)?vType[0]:0;
	m_DestID = (vID.size()>0)?vID[0]:CGUID::GUID_INVALID;
	m_lDestX = lDestX;
	m_lDestY = lDestY;
	m_fDir = fDir;
	m_lSkillID = lID;
	m_lSkillLevel = lLevel;

	//SetPosXY(m_pMoveShape->GetPosX(), m_pMoveShape->GetPosY());
	CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(lID);
	if(pSkill==NULL)
		return FALSE;
	DWORD dwlev = (long)pSkill->vectorLevels.size() >= lLevel? (DWORD)lLevel-1:((DWORD)pSkill->vectorLevels.size() - 1);
	long lAiID = pSkill->vectorLevels[dwlev].dwSkillLevelAI;
	vector<CSkillListXml::tagLevelValue> vectBegin = pSkill->vectorLevels[dwlev].vecStepBegin;
	m_pSkillAI = CSkillAI::CreateSkillAI(this, lAiID);
	vector<CSkillListXml::tagLevelValue>::iterator it;
	for(it = vectBegin.begin(); it!=vectBegin.end();it++)
	{
		ExecuteStep(it->dwID, it->dwValue, it->strValue);
	}
	// ���ܿ�ʼʱ����ͼ������ȴ����
	if(m_pMoveShape == GetGame()->GetMainPlayer())
	{
		// ��ȴͼ��
		CPlayer::tagSkill *pS = GetGame()->GetMainPlayer()->GetSkill(lID);
		if(pS)
		{
			if(pS->dwRestoreTime>0)
			{
				pS->dwCoolDownStartTime = timeGetTime();
				pS->dwCoolDownGrayStartTime = 0;
			}
		}
		// ��������Ч��ʱ�䣬��������������ʾ����
		if(m_lIntonateTime > 0)
		{
			DWORD dwCurTime = timeGetTime();
			GetGame()->GetMainPlayer()->SetIntonateSkill(lID);
			GetGame()->GetMainPlayer()->SetCurIntonateTime(m_lIntonateTime,dwCurTime);
			// ����������࣬��������ʾ������
			if(pSkill->bIsLead)
				GetGame()->GetMainPlayer()->SetIsLToR(false);
			else
				GetGame()->GetMainPlayer()->SetIsLToR(true);
		}
	}
	if(m_pMoveShape->GetType() == TYPE_PET)
	{
		CPet *pPet = (CPet*)m_pMoveShape;
		if (pPet->IsMainPlayerPet())
		{
			//���µ�ǰ���ܵĿ�ʼ��ȴʱ��
			tagPetSkillInfo *pPetSkill = pPet->GetPetSkillInfoByID(lID);
			if(pPetSkill && pPetSkill->lRestorTimeLength>0)
			{
				pPetSkill->lStartCoolTime = GetCurTickCount();
			}
			//����������
			// 
			if(m_lIntonateTime > 0)
			{
				DWORD dwCurTime = timeGetTime();
				pPet->SetIntonateSkill(lID);
				pPet->SetCurIntonateTime(m_lIntonateTime,dwCurTime);
				// ����������࣬��������ʾ������
				if(pSkill->bIsLead)
					pPet->SetIsLToR(false);
				else
					pPet->SetIsLToR(true);
			}
		}
	}
	if(m_pSkillAI && m_pSkillAI->StepBeginAI(vType, vID))
	{
		return TRUE;
	}
	else
		return FALSE;
}

// ��������ʱ��
void CSkillXml::IntonateTime(long lTime)
{
	m_lIntonateTime = lTime;
}

// ���������׶���Ҫѭ������
void CSkillXml::IntonateMainAct(WORD l)
{
	m_wIntonateMainAct = l;
}

// ��������ǰ�ڶ���ID
void CSkillXml::IntonateAct(long l)
{
	if(l)
	{
		DWORD dwCurrentTime = timeGetTime();
		AnimInfo::tagActionInfo *pActionInfo = m_pMoveShape->GetAnimInfo()->GetActionInfo();
		pActionInfo->bInterpolation = TRUE;
		pActionInfo->dwOldAction = pActionInfo->dwCurAction;
		pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
		pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
		pActionInfo->dwInterpolationElapseTime = 0;
		pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
		pActionInfo->dwInterpolationTimeLength =240;
		m_pMoveShape->SetAction((WORD)l);
		//pActionInfo->bInterpolation = FALSE;
		
	}
}
// ��������������ЧID
void CSkillXml::IntonateEffect(long l)
{
	m_lIntonateEffect = l;
}
// ����������Ч����
void CSkillXml::IntonateEffectIndex(long l)
{
	m_lIntonateEffectIndex = l;
}
// ������Ч�Ƿ����������
void CSkillXml::IntonateEffectByEquip(long l)
{
	m_bIntonateEffectByEquip = ( l == 0)?false : true;
}
// ����������Ч��λ��
void CSkillXml::IntonateEffectPosition(long l)
{
	switch(l)
	{
	case 0: m_eIntonateEffectPosition = LOCATOR_WIND; break;
	case 1: m_eIntonateEffectPosition = LOCATOR_HEAD; break;
	case 2: m_eIntonateEffectPosition = LOCATOR_FOOTCENTER; break;
	case 3: m_eIntonateEffectPosition = LOCATOR_LEFTHAND; break;
	case 4: m_eIntonateEffectPosition = LOCATOR_RIGHTHAND; break;
	case 5: m_eIntonateEffectPosition = LOCATOR_LEFTFOOT; break;
	case 6: m_eIntonateEffectPosition = LOCATOR_RIGHTFOOT; break;
	default: m_eIntonateEffectPosition = LOCATOR_FOOTCENTER; break;
	}
}
// ����������Ч�Ƿ�ѭ��
void CSkillXml::IntonateEffectCycle(long l)
{
	m_bIntonateEffectCycle = (l == 0)? false: true;
}
// ��ʼ����
void CSkillXml::IntonatePlay()
{
	if(m_lIntonateEffect)
	{
		// ������Ч�Ƿ���װ�����
		if (m_bIntonateEffectByEquip)
		{
			if(m_pMoveShape->GetType() == TYPE_PLAYER)		// ���
			{
				DWORD dwIndex = 0;
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(m_pMoveShape);
				if (GetGame()->GetMainPlayer() == m_pMoveShape)				// ��ǰ���
				{
					CGoods *pGoods = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON);
					if(pGoods)
						dwIndex = pGoods->GetProperty()->dwGroundId;
				}
				else	// �������
				{
					DWORD dwWeapon = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
					CGoodsList::tagGoods2 *pGP = CGoodsList::GetProperty(dwWeapon);
					if(pGP)
					{
						dwIndex = pGP->BaseProperty.dwGroundId;
					}
				}
				m_lIntonateEffect = CWeaponEffectXml::GetWeaponEffect(dwIndex);
			}
		}
		// ���������������趨����
		bool bAsyn = true;
		// �Լ��Ǽ����ͷ���
		if(GetGame()->GetMainPlayer() == m_pMoveShape)
			bAsyn = false;
		long lType = m_pMoveShape->GetType();
		CGUID guidEffect = CGUID::GUID_INVALID;
		if(m_pMoveShape)
			guidEffect = m_pMoveShape->AddEffectLocator(m_lIntonateEffect,m_pMoveShape,m_eIntonateEffectPosition,m_bIntonateEffectCycle,0.f,0,bAsyn);
		// ѭ����Ч���Ҵ���ɾ���ӳ�ʱ��
		if(m_bIntonateEffectCycle && m_lCycleEffectDelayTime > 0)
		{
			CEffect * pEffect = m_pMoveShape->FindEffect(m_lIntonateEffect);
			if(pEffect)
			{
				pEffect->SetIsNeedAlphaFade(true);
				pEffect->SetAlphaFadeInTimeLength(m_lCycleEffectDelayTime);
			}
		}
		
		//=====================07.12.19����===================
		if(lType != 0 && guidEffect != CGUID::GUID_INVALID)
		{
			// ����һ���µ���Ч�����ṹ
			tagEffectIndex stEffectIndex;
			stEffectIndex.guidEffect = guidEffect;
			stEffectIndex.lEffectIndex = m_lIntonateEffectIndex;
			// �ڱ����ܲ����£����ҹ��ڱ�Ŀ�����Ч�б�����ҵ���ֱ����ӽ�ȥ�����򣬹���һ���µĶ�����Ч�ṹ��ӵ����׶��б���ȥ
			list<CSkillXml::tagEffectIndex> *lsEffectIndex = GetObjectEffectList(m_lStep,lType,m_pMoveShape->GetExID());
			if(lsEffectIndex)
			{
				lsEffectIndex->push_back(stEffectIndex);
			}
			else
			{
				vector<CSkillXml::tagObjectEffect> *vecObjEffect = GetEffectIndex(m_lStep);
				if(vecObjEffect)
				{
					tagObjectEffect stObjEffect;
					stObjEffect.guid = m_pMoveShape->GetExID();
					stObjEffect.lType = lType;
					stObjEffect.lsEffectIndex.push_back(stEffectIndex);
					vecObjEffect->push_back(stObjEffect);
				}
			}
			//tagEffectIndex stEffectIndex;
			//stEffectIndex.guidEffect = guidEffect;
			//stEffectIndex.lEffectIndex = m_lIntonateEffectIndex;
			//// ���Ȳ鿴���׶ε���Ч�б����Ƿ��й��ڸö������Ч�б�����У����ڶ������Ч�б���ֱ����ӣ�������Ҫ����һ���µĶ�����Ч
			//for(int i=0; i<m_vecEffectIndex[m_lStep].size(); ++i)
			//{
			//	if((m_vecEffectIndex[m_lStep][i]).guid == m_pMoveShape->GetExID() && (m_vecEffectIndex[m_lStep][i]).lType)
			//	{
			//		(m_vecEffectIndex[m_lStep][i]).lsEffectIndex.push_back(stEffectIndex);
			//		return;
			//	}
			//}
			//tagObjectEffect stObjEffect;
			//stObjEffect.guid = m_pMoveShape->GetExID();
			//stObjEffect.lType = lType;
			//stObjEffect.lsEffectIndex.push_back(stEffectIndex);
			//m_vecEffectIndex[m_lStep].push_back(stObjEffect);
		}
		//====================================================
	}
		
}
// ����������Чid
void CSkillXml::IntonateSound(long l)
{
	m_lIntonateSound = l;
}
// ������Ч�Ƿ����������
void CSkillXml::IntonateSoundByEquip(long l)
{
	m_bIntonateSoundByEquip = (l == 0)?false:true;
}
// ����������Ч�Ƿ�ѭ��
void CSkillXml::IntonateSoundCycle(long l)
{
	m_bIntonateSoundCycle = (l == 0)? false:true;
}
// ��ʼ����������Ч
void CSkillXml::IntonateSoundPlay()
{
	if(m_lIntonateSound)
	{
		//�ж���Ů��Ч,��С��5λ���������1λ��1Ϊ������2ΪŮ��
		if(((int)m_lIntonateSound-10000)<0&&m_pMoveShape->GetType()==TYPE_PLAYER)
		{
			CPlayer *pPlayerOther = dynamic_cast<CPlayer*>(m_pMoveShape);
			if(pPlayerOther->GetSex())
			{
				m_lIntonateSound = m_lIntonateSound*10+2;
			}else{
				m_lIntonateSound = m_lIntonateSound*10+1;
			}
		}


		// ������Ч�Ƿ���װ���й�
		if(m_bIntonateSoundByEquip)
		{
			if (m_pMoveShape->GetType() == TYPE_PLAYER)				// ���
			{
				DWORD dwIndex = 0;
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(m_pMoveShape);
				if (GetGame()->GetMainPlayer() == m_pMoveShape)
				{
					CGoods *pWeapon = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON);
					if(pWeapon)
						dwIndex = pWeapon->GetIndex();
				}
				else
				{
					dwIndex = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
				}
				DWORD dwSoundID1 = 0, dwSoundID2 = 0;
				CGoodsList::tagGoods2 *pGoods = CGoodsList::GetProperty(dwIndex);
				if (pGoods)
				{
					// ����װ����Ʒ�趨��������Ч
					dwSoundID1 = pGoods->BaseProperty.dwSoundID1;
					dwSoundID2 = pGoods->BaseProperty.dwSoundID2;
					GetGame()->GetAudioList()->Play(NULL, TYPE_SKILL, (WORD)dwSoundID1, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(),
						pPlayer->GetPosX()),m_bIntonateSoundCycle);
					if (dwSoundID2)
					{
						GetGame()->GetAudioList()->Play(NULL, TYPE_SKILL, (WORD)dwSoundID2, Vector3(pPlayer->GetPosY(), 
							pPlayer->GetHeight(), pPlayer->GetPosX()),m_bIntonateSoundCycle);
					}
				}
			}
			else		// �����ľͲ����������趨����Ч 

				m_IntonateSoundChannel = GetGame()->GetAudioList()->Play(m_pMoveShape,TYPE_SKILL,(WORD)m_lIntonateSound, Vector3(m_pMoveShape->GetPosY(),
				m_pMoveShape->GetHeight(),m_pMoveShape->GetPosX()),m_bIntonateSoundCycle);
		}
		else	// �������װ����ؾͲ����������趨����Ч
			m_IntonateSoundChannel = GetGame()->GetAudioList()->Play(m_pMoveShape,TYPE_SKILL,(WORD)m_lIntonateSound, Vector3(m_pMoveShape->GetPosY(),
			m_pMoveShape->GetHeight(),m_pMoveShape->GetPosX()),m_bIntonateSoundCycle);	
	}
}
// �����Ƿ���Ա����
void CSkillXml::IntonateBreak(long l)
{
	GetGame()->GetGameControl()->SetIntonateBreak((l == 0)?false:true);
}


//////////////////////////////////////////////////////////////////////////
// ִ�м���ʵʩ����
//////////////////////////////////////////////////////////////////////////
bool CSkillXml::StepRun(long lID, long lLevel, long sType, const CGUID& sID,long dX, long dY, float fDir, vector<long> vType, vector<CGUID> vID)
{
	m_lStep = 1;
	m_pMoveShape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(sType, sID));
	m_fDir = fDir;
	vector<long>::iterator itShape;
	int index = 0;
	if(m_vecDestMoveShape.size()>0)
		m_vecDestMoveShape.clear();
	for(itShape = vType.begin(); itShape!=vType.end(); itShape++)
	{
		CMoveShape* moveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject((*itShape),vID[index]));
		if(moveshape)
			m_vecDestMoveShape.push_back(moveshape);
		index++;
	}
	m_lSourType = sType;
	m_SourID = sID;
	m_lDestType = (vType.size()>0)?vType.back():0;
	m_DestID = (vID.size()>0)?vID.back():CGUID::GUID_INVALID;
	m_lDestX = dX;
	m_lDestY = dY;
	m_lSkillID = lID;
	m_lSkillLevel = lLevel;
	// ִ�п�ʼ����
	CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(lID);
	if(pSkill == NULL)
		return FALSE;
	DWORD dwlev = (long)pSkill->vectorLevels.size() >= lLevel? (DWORD)lLevel-1:((DWORD)pSkill->vectorLevels.size() - 1);
	vector<CSkillListXml::tagLevelValue> vectRun = pSkill->vectorLevels[dwlev].vecStepRun;
	SAFE_DELETE(m_pSkillAI);
	m_pSkillAI = CSkillAI::CreateSkillAI(this, pSkill->vectorLevels[dwlev].dwSkillLevelAI);
	vector<CSkillListXml::tagLevelValue>::iterator it;
	for (it = vectRun.begin(); it != vectRun.end(); it++) 
	{
		ExecuteStep(it->dwID, it->dwValue, it->strValue);
	}
	//////////////////////////////////////////////////////////////////////////
	// �����ٻ����ܴ���
	vector<CSkillListXml::tagLevelValue> vecSummon = pSkill->vectorLevels[dwlev].vecStepSummon;
	if(vecSummon.size()>0)
	{
		//m_pSkillAI = CSkillAI::CreateSkillAI(this, pSkill->vectorLevels[dwlev].dwSkillLevelAI);
		for (it = vecSummon.begin(); it != vecSummon.end(); it++)
		{
			ExecuteStep(it->dwID, it->dwValue, it->strValue);
		}
	}
	// ���������࣬��������ʾ������
	if(m_pMoveShape == GetGame()->GetMainPlayer() && m_lIntonateTime > 0 && pSkill->bIsLead)
	{
		// ��������Ч��ʱ�䣬��������������ʾ����
		DWORD dwCurTime = timeGetTime();
		GetGame()->GetMainPlayer()->SetIntonateSkill(lID);
		GetGame()->GetMainPlayer()->SetCurIntonateTime(m_lIntonateTime,dwCurTime);
		GetGame()->GetMainPlayer()->SetIsLToR(false);
	}
	// ɾ��ǰһ���׶ε�������Ч
	DelAllEffectListByStep(STEP_START);
	//////////////////////////////////////////////////////////////////////////
	if (m_pSkillAI && m_pSkillAI->StepRunAI(vType,vID) == false)
		return FALSE;
	else
		return TRUE;
}
// ����ʩ�Ŷ���ID
void CSkillXml::ReleaseAct(long l)
{
	m_pMoveShape->SetDirEx(m_fDir);
	if(l)
	{
		extern bool g_bForceUpdate;
		g_bForceUpdate = true;
		// �������Ϊ��ͨ����
		if(l == ACT_ATTACK)
		{
			m_pMoveShape->SetAttackAction(random(3));
			m_pMoveShape->SetAction((WORD)l);
		}
		else   
		{
   			m_pMoveShape->SetAction((WORD)l);
		}
	}
}

// ����ʩ�ź�������ID
void CSkillXml::ReleaseActEx(long l)
{
	if (l)
		m_queueRunStepActEx.push((WORD)l);
}

// ����ʩ�Ŷ�����Чid
void CSkillXml::ReleaseEffect(long l)
{
	m_lReleaseEffect = l;
}
// ���ö�����Ч����
void CSkillXml::ReleaseEffectIndex(long l)
{
	m_lReleaseEffectIndex = l;
}
// ʩ����Ч�Ƿ����������
void CSkillXml::ReleaseEffectByEquip(long l)
{
	m_bReleaseEffectByEquip = (l == 0)?false:true;
}
// ����ʩ����ЧĿ��
void CSkillXml::ReleaseEffectDest(long l)
{
	m_lReleaseEffectDest = l;
}
// ����ʩ����Ч��λ��
void CSkillXml::ReleaseEffectPosition(long l)
{
	switch(l)
	{
	case 0: m_eReleaseEffectPosition = LOCATOR_WIND; break;
	case 1: m_eReleaseEffectPosition = LOCATOR_HEAD; break;
	case 2: m_eReleaseEffectPosition = LOCATOR_FOOTCENTER; break;
	case 3: m_eReleaseEffectPosition = LOCATOR_LEFTHAND; break;
	case 4: m_eReleaseEffectPosition = LOCATOR_RIGHTHAND; break;
	case 5: m_eReleaseEffectPosition = LOCATOR_LEFTFOOT; break;
	case 6: m_eReleaseEffectPosition = LOCATOR_RIGHTFOOT; break;
	default: m_eReleaseEffectPosition = LOCATOR_FOOTCENTER; break;
	}
}
// ����ʩ����Ч�Ƿ�ѭ��
void CSkillXml::ReleaseEffectCycle(long l)
{
	m_bReleaseEffectCycle = (l == 0)?false : true;
}
// ��ʼʩ�Ŷ���
void CSkillXml::ReleasePlay()
{
	bool bAsyn = true;
	// �Լ��Ǽ����ͷ���
	CMoveShape* pMoveshape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(GetSourType(), GetSourID()));
	if(GetGame()->GetMainPlayer() == pMoveshape)
		bAsyn = false;
	if(m_lReleaseEffect)
	{
		// ʩ����Ч�Ƿ���װ�����
		if (m_bReleaseEffectByEquip)
		{
			if (m_pMoveShape->GetType() == TYPE_PLAYER)				// ���
			{
				DWORD dwIndex = 0;
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(m_pMoveShape);
				if (GetGame()->GetMainPlayer() == m_pMoveShape)		// ��ǰ���
				{
					CGoods *pGoods = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON);
					if(pGoods)
						dwIndex = pGoods->GetProperty()->dwGroundId;
				}
				else	// �������
				{
					DWORD dwWeapon = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
					CGoodsList::tagGoods2 *pGP = CGoodsList::GetProperty(dwWeapon);
					if (pGP)
					{
						dwIndex = pGP->BaseProperty.dwGroundId;
					}
				}
				m_lIntonateEffect = CWeaponEffectXml::GetWeaponEffect(dwIndex);
			}
		}
		// ��ʱ���������ڱ�����Ч����ߵ����ͺ�GUID����Ч�����GUID
		long lType = 0;
		CGUID guidObj = CGUID::GUID_INVALID;
		CGUID guidEffect = CGUID::GUID_INVALID;
		CEffect * pEffect = NULL;
		switch(m_lReleaseEffectDest)
		{
			// ������Ч (���Լ�)
		case 0:
			if(m_pMoveShape)
			{
				lType = m_pMoveShape->GetType();
				guidObj = m_pMoveShape->GetExID();
				guidEffect = m_pMoveShape->AddEffectLocator(m_lReleaseEffect, m_pMoveShape, m_eReleaseEffectPosition, m_bReleaseEffectCycle, m_pMoveShape->GetNowDir(), m_lReleaseEffectIndex,bAsyn);
				pEffect = m_pMoveShape->FindEffect(m_lReleaseEffect,m_lReleaseEffectIndex);
			}
			break;
			// ������Ч (��Ŀ��)
		case 1:
			if(m_vecDestMoveShape.size()>0)
			{
				vector<CMoveShape*>::iterator it;
				for (it = m_vecDestMoveShape.begin(); it!=m_vecDestMoveShape.end(); it++)
				{
					if((*it))
					{
						lType = (*it)->GetType();
						guidObj = (*it)->GetExID();
						guidEffect = (*it)->AddEffectLocator(m_lReleaseEffect, (*it), m_eReleaseEffectPosition, m_bReleaseEffectCycle, m_pMoveShape->GetNowDir(), m_lReleaseEffectIndex,bAsyn);
						pEffect = (*it)->FindEffect(m_lReleaseEffect,m_lReleaseEffectIndex);
					}
				}
			}
			else
			{
				lType = TYPE_REGION;
				guidObj = GetGame()->GetRegion()->GetExID();
				pEffect = GetGame()->GetRegion()->PlayEffect(m_lReleaseEffect, 
					m_lDestX+0.5f, m_lDestY+0.5f, 
					GetGame()->GetRegion()->GetCellHeight((float)m_lDestX,(float) m_lDestY), 
					m_pMoveShape->GetNowDir(),bAsyn);
				pEffect->SetLoop(m_bReleaseEffectCycle);
				if(pEffect)
					guidEffect = pEffect->GetExID();
			}
			break;
			// ������Ч (�Ե���)
		case 2: 
			lType = TYPE_REGION;
			guidObj = GetGame()->GetRegion()->GetExID();
			pEffect = GetGame()->GetRegion()->PlayEffect(m_lReleaseEffect, 
				m_lDestX+0.5f, m_lDestY+0.5f,
				GetGame()->GetRegion()->GetCellHeight((float)m_lDestX,(float) m_lDestY),
				-1.f,bAsyn);
			pEffect->SetLoop(m_bReleaseEffectCycle);
			if(pEffect)
				guidEffect = pEffect->GetExID();
			break;
			// ������Ч-����
		case 3: 
			if(m_pMoveShape)
			{
				lType = TYPE_REGION;
				guidObj = GetGame()->GetRegion()->GetExID();
				pEffect = GetGame()->GetRegion()->PlayEffect(m_lReleaseEffect, m_pMoveShape->GetPosX(), m_pMoveShape->GetPosY(),  GetGame()->GetRegion()->GetCellHeight(m_pMoveShape->GetPosX(), m_pMoveShape->GetPosY()), m_pMoveShape->GetNowDir(),bAsyn);
				if(pEffect)
					guidEffect = pEffect->GetExID();
			}
			break;
		default: 
			break;
		}
		// ѭ����Ч���Ҵ���ɾ���ӳ�ʱ��
		if(pEffect && m_bReleaseEffectCycle && m_lCycleEffectDelayTime > 0)
		{
			pEffect->SetIsNeedAlphaFade(true);
			pEffect->SetAlphaFadeInTimeLength(m_lCycleEffectDelayTime);
		}
		//=====================07.12.19����===================
		if(lType != 0 && guidEffect != CGUID::GUID_INVALID && guidObj != CGUID::GUID_INVALID)
		{
			// ����һ���µ���Ч�����ṹ
			tagEffectIndex stEffectIndex;
			stEffectIndex.guidEffect = guidEffect;
			stEffectIndex.lEffectIndex = m_lReleaseEffectIndex;
			// �ڱ����ܲ����£����ҹ��ڱ���������Ч�б�����ҵ���ֱ����ӽ�ȥ�����򣬹���һ���µĶ�����Ч�ṹ��ӵ����׶��б���ȥ
			list<CSkillXml::tagEffectIndex> *lsEffectIndex = GetObjectEffectList(m_lStep,lType,guidObj);
			if(lsEffectIndex)
			{
				lsEffectIndex->push_back(stEffectIndex);
			}
			else
			{
				vector<CSkillXml::tagObjectEffect> *vecObjEffect = GetEffectIndex(m_lStep);
				if(vecObjEffect)
				{
					tagObjectEffect stObjEffect;
					stObjEffect.guid = guidObj;
					stObjEffect.lType = lType;
					stObjEffect.lsEffectIndex.push_back(stEffectIndex);
					vecObjEffect->push_back(stObjEffect);
				}
			}
			//tagEffectIndex stEffectIndex;
			//stEffectIndex.guidEffect = guidEffect;
			//stEffectIndex.lEffectIndex = m_lReleaseEffectIndex;
			//// ���Ȳ鿴���׶ε���Ч�б����Ƿ��й��ڸö������Ч�б�����У����ڶ������Ч�б���ֱ����ӣ�������Ҫ����һ���µĶ�����Ч
			//for(int i=0; i<m_vecEffectIndex[m_lStep].size(); ++i)
			//{
			//	if((m_vecEffectIndex[m_lStep][i]).guid == guidObj && (m_vecEffectIndex[m_lStep][i]).lType)
			//	{
			//		(m_vecEffectIndex[m_lStep][i]).lsEffectIndex.push_back(stEffectIndex);
			//		return;
			//	}
			//}
			//tagObjectEffect stObjEffect;
			//stObjEffect.guid = guidObj;
			//stObjEffect.lType = lType;
			//stObjEffect.lsEffectIndex.push_back(stEffectIndex);
			//m_vecEffectIndex[m_lStep].push_back(stObjEffect);
		}
		
		//====================================================
	}
	
}
// ����ʩ����Чid
void CSkillXml::ReleaseSound(long l)
{
	m_lReleaseSound = l;
}
// ʩ����Ч�Ƿ����������
void CSkillXml::ReleaseSoundByEquip(long l)
{
	m_bReleaseSoundByEquip = (l == 0)?false:true;
}
// ����ʩ����Ч�Ƿ�ѭ��
void CSkillXml::ReleaseSoundCycle(long l)
{
	m_bReleaseSoundCycle = (l == 0)?false:true;
}
// ����ʩ����ЧĿ��
void CSkillXml::ReleaseSoundDest(long l)
{
	m_lReleaseSoundDest = l;
}
// ����ʩ����Ч
void CSkillXml::ReleaseSoundPlay()
{
	if(m_lReleaseSound)
	{
		//�ر��ʳ�����Ч��
		char strbuf[MAX_PATH]="";
		sprintf(strbuf,	"sounds/skills/%03d.wav", m_lIntonateSound);
		if(GetGame()->GetAudioList()->IsPlaying(strbuf,m_IntonateSoundChannel))
		{
			GetGame()->GetAudioList()->Stop3DSound(strbuf,m_IntonateSoundChannel);
		}
		//�ж���Ů��Ч����С��5λ���������1λ��1Ϊ������2ΪŮ��
		if(((int)m_lReleaseSound-10000)<0&&m_pMoveShape->GetType()==TYPE_PLAYER)
		{
			CPlayer *pPlayerOther = dynamic_cast<CPlayer*>(m_pMoveShape); 
			if(pPlayerOther->GetSex())
			{
				m_lReleaseSound = m_lReleaseSound*10+2;
			}else{
				m_lReleaseSound = m_lReleaseSound*10+1;
			}
		}


		// ʩ����Ч�Ƿ���װ���й�
		if (m_bReleaseSoundByEquip)
		{
			if(m_pMoveShape->GetType() == TYPE_PLAYER)			// ���
			{
				DWORD dwIndex = 0;
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(m_pMoveShape);
				if(GetGame()->GetMainPlayer() == m_pMoveShape)		// ��ǰ���
				{
					CGoods *pWeapon = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON);
					if(pWeapon)
						dwIndex = pWeapon->GetIndex();
				}
				else	// �������
				{
					dwIndex = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
				}
				DWORD dwSoundID1 = 0, dwSoundID2 = 0;
				CGoodsList::tagGoods2 *pGoods = CGoodsList::GetProperty(dwIndex);		// ȡ��װ���йص���Ч
				if (pGoods)
				{
					// ����װ����Ʒ�趨��������Ч
					dwSoundID1 = pGoods->BaseProperty.dwSoundID1;
					dwSoundID2 = pGoods->BaseProperty.dwSoundID2;
					GetGame()->GetAudioList()->Play(NULL, TYPE_SKILL, (WORD)dwSoundID1,Vector3(pPlayer->GetPosY(),pPlayer->GetHeight(),
						pPlayer->GetPosX()),m_bReleaseSoundCycle);
					if (dwSoundID2)
					{
						GetGame()->GetAudioList()->Play(NULL, TYPE_SKILL, (WORD)dwSoundID2,Vector3(pPlayer->GetPosY(),pPlayer->GetHeight(),
							pPlayer->GetPosX()),m_bReleaseSoundCycle);
					}
				}
			}
			else	// ��������
			{
				switch(m_lReleaseSoundDest)
				{
					char strFile[MAX_PATH];
				case 0:// ���Լ�������Ч
					GetGame()->GetAudioList()->Play(m_pMoveShape,TYPE_SKILL,(WORD)m_lReleaseSound, Vector3(m_pMoveShape->GetPosY(),
						m_pMoveShape->GetHeight(),m_pMoveShape->GetPosX()),m_bReleaseSoundCycle);
					break;
				case 1:// ��Ŀ�겥����Ч
					{
						sprintf(strFile, "sounds/skills/%d.wav",m_lReleaseSound);
						if (m_vecDestMoveShape.size()>0)
						{
							vector<CMoveShape*>::iterator it;
							for (it = m_vecDestMoveShape.begin(); it!=m_vecDestMoveShape.end(); it++)
							{
								GetGame()->GetAudioList()->Play(strFile, (*it)->GetPosX(),(*it)->GetPosY(),
									(*it)->GetHeight(),m_bReleaseSoundCycle);
							}
						}
						else
						{
							CClientRegion* pRegion = (CClientRegion*)m_pMoveShape->GetFather();
							float x = (float)m_lDestX + 0.5f;
							float y = (float)m_lDestY + 0.5f;
							GetGame()->GetAudioList()->Play(strFile, x, y, pRegion->GetCellHeight(x, y),m_bReleaseSoundCycle);
						}
					}
					break;
				case 2:// �Ե��沥����Ч
					{
						sprintf(strFile, "sounds/skills/%d.wav", m_lReleaseSound);
						CClientRegion* pRegion = (CClientRegion*)m_pMoveShape->GetFather();
						float x = (float)m_lDestX + 0.5f;
						float y = (float)m_lDestY + 0.5f;
						GetGame()->GetAudioList()->Play(strFile, x, y, pRegion->GetCellHeight(x, y),m_bReleaseSoundCycle);
					}
					break;
				default: break;
				}
			}
		}
		else	// ����װ���й�
		{
			switch(m_lReleaseSoundDest)
			{
				char strFile[MAX_PATH];
			case 0:// ���Լ�������Ч
				GetGame()->GetAudioList()->Play(m_pMoveShape,TYPE_SKILL,(WORD)m_lReleaseSound, Vector3(m_pMoveShape->GetPosY(),
					   m_pMoveShape->GetHeight(),m_pMoveShape->GetPosX()),m_bReleaseSoundCycle);
				break;
			case 1:// ��Ŀ�겥����Ч
				{
					sprintf(strFile, "sounds/skills/%d.wav",m_lReleaseSound);
					if (m_vecDestMoveShape.size()>0)
					{
						vector<CMoveShape*>::iterator it;
						for (it = m_vecDestMoveShape.begin(); it!=m_vecDestMoveShape.end(); it++)
						{
							GetGame()->GetAudioList()->Play(strFile, (*it)->GetPosX(),(*it)->GetPosY(),
								(*it)->GetHeight(),m_bReleaseSoundCycle);
						}
					}
					else
					{
						CClientRegion* pRegion = (CClientRegion*)m_pMoveShape->GetFather();
						float x = (float)m_lDestX + 0.5f;
						float y = (float)m_lDestY + 0.5f;
						GetGame()->GetAudioList()->Play(strFile, x, y, pRegion->GetCellHeight(x, y),m_bReleaseSoundCycle);
					}
				}
				break;
			case 2:// �Ե��沥����Ч
				{
					sprintf(strFile, "sounds/skills/%d.wav", m_lReleaseSound);

					CClientRegion* pRegion = (CClientRegion*)m_pMoveShape->GetFather();
					float x = (float)m_lDestX + 0.5f;
					float y = (float)m_lDestY + 0.5f;
					GetGame()->GetAudioList()->Play(strFile, x, y, pRegion->GetCellHeight(x, y),m_bReleaseSoundCycle);
				}
				break;
			default: break;
			}
		}
	}
}
// ��Ƶ��(�ٷֱ�)
void CSkillXml::ShockRatio(long l)
{
	m_fShockRatio = (float)(l/100.0f);
}
// �𶯼��ʱ
void CSkillXml::ShockInterval(long l)
{
	m_lShockInterval = l;
}
// ��ʱ�䳤��
void CSkillXml::ShockTimeLength(long l)
{
	m_lShockTimeLength = l;
}



/*
 * ����: ��ʼ��
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.01.17 - lpf
 *		�����˶�ϵͳ�������Ƿ����𶯿��ص��ж�
 */
void CSkillXml::ShockPlay()
{
	if (GetGame()->GetSetup()->bShock &&
		m_fShockRatio && m_lShockInterval && m_lShockTimeLength)
	{
		GetGame()->GetRegion()->GetRegionCamera()->ShockCamera(m_fShockRatio, m_lShockInterval, m_lShockTimeLength);
	}
}



// ���÷�����ЧID
void CSkillXml::FlyEffect(long l)
{
	m_lFlyEffect = l;
}
// ���÷�����Ч����
void CSkillXml::FlyEffectIndex(long l)
{
	m_lFlyEffectIndex = l;
	m_vecFlyEffectIndex.push_back(m_lFlyEffectIndex);
}
// ������Ч�Ƿ����������
void CSkillXml::FlyEffectByEquip(long l)
{
	m_bFlyEffectByEquip = (l == 0)?false:true;
}
// ���÷�����Ч�Ӻδ�����
void CSkillXml::FlyEffectPosition(long l)
{
	switch(l)
	{
	case 0: m_eFlyEffectPosition = LOCATOR_WIND; break;
	case 1: m_eFlyEffectPosition = LOCATOR_HEAD; break;
	case 2: m_eFlyEffectPosition = LOCATOR_FOOTCENTER; break;
	case 3: m_eFlyEffectPosition = LOCATOR_LEFTHAND; break;
	case 4: m_eFlyEffectPosition = LOCATOR_RIGHTHAND; break;
	case 5: m_eFlyEffectPosition = LOCATOR_LEFTFOOT; break;
	case 6: m_eFlyEffectPosition = LOCATOR_RIGHTFOOT; break;
	default: m_eFlyEffectPosition = LOCATOR_FOOTCENTER; break;
	}
	m_vecFlyEffectPos.push_back(m_eFlyEffectPosition);
}
// ���÷�����Ч�Ƿ�ѭ������
void CSkillXml::FlyEffectCycle(long l)
{
	m_bFlyEffectCycle = (l == 0)?false : true;
}
// ���÷�����ЧX�᷽��ƫ�ƽǶ�
void CSkillXml::FlyEffectTurnX(long l)
{
	m_lFlyEffectTurnX = l;
	m_vecFlyEffectTurnX.push_back(l);
}
// ���÷�����ЧY�᷽��ƫ�ƽǶ�
void CSkillXml::FlyEffectTurnY(long l)
{
	m_lFlyEffectTurnY = l;
	m_vecFlyEffectTurnY.push_back(l);
}
// ��ʼ���ж���
void CSkillXml::FlyPlay()
{
	if(m_pMoveShape && m_lFlyEffect )
	{
		if (m_bFlyEffectByEquip)			// ������Ч�Ƿ���װ�����
		{
			if(m_pMoveShape->GetType() == TYPE_PLAYER)	// ���
			{
				DWORD dwIndex = 0;
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(m_pMoveShape);
				if (GetGame()->GetMainPlayer() == m_pMoveShape)		// ��ǰ���
				{
					CGoods *pGoods = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON);
					if (pGoods)
						dwIndex = pGoods->GetProperty()->dwGroundId;
				}
				else	// �������
				{
					DWORD dwWeapon = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
					CGoodsList::tagGoods2 *pGP = CGoodsList::GetProperty(dwWeapon);
					if (pGP)
					{
						dwIndex = pGP->BaseProperty.dwGroundId;
					}
				}
				m_lFlyEffect = CWeaponEffectXml::GetWeaponEffect(dwIndex);	// ��������ȡ����Ч���
			}
		}
		m_vecFlyEffect.push_back(m_lFlyEffect);
	}
	
}

// ���÷�����Чid
void CSkillXml::FlySound(long l)
{
	m_lFlySound = l;
}
// ������Ч�Ƿ����������
void CSkillXml::FlySoundByEquip(long l)
{
	m_bFlySoundByEquip =(l == 0)?false:true;
}
// ���÷�����Ч�Ӻδ��ͷ�
void CSkillXml::FlySoundPosition(long l)
{
	switch(l)
	{
	case 0: m_eFlySoundPosition = LOCATOR_WIND; break;
	case 1: m_eFlySoundPosition = LOCATOR_HEAD; break;
	case 2: m_eFlySoundPosition = LOCATOR_FOOTCENTER; break;
	case 3: m_eFlySoundPosition = LOCATOR_LEFTHAND; break;
	case 4: m_eFlySoundPosition = LOCATOR_RIGHTHAND; break;
	case 5: m_eFlySoundPosition = LOCATOR_LEFTFOOT; break;
	case 6: m_eFlySoundPosition = LOCATOR_RIGHTFOOT; break;
	default: m_eFlySoundPosition = LOCATOR_FOOTCENTER; break;
	}
}
// ���÷�����Ч�Ƿ�ѭ��
void CSkillXml::FlySoundCycle(long l)
{
	m_bFlySoundCycle = (l == 0)?false:true;
}
// ��ʼ���ŷ�����Ч
void CSkillXml::FlySoundPlay()
{
	if(m_pMoveShape && m_lFlySound )
	{
		int nChannel = 0;
		if (m_bFlySoundByEquip)		// ������Ч�Ƿ���װ�����
		{
			if (m_pMoveShape->GetType() == TYPE_PLAYER)		// ���
			{
				DWORD dwIndex = 0;
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(m_pMoveShape);
				if (GetGame()->GetMainPlayer() == m_pMoveShape)		// ��ǰ���
				{
					CGoods *pWeapon = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON);
					if (pWeapon)
						dwIndex = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
				}
				else		// �������
				{
					dwIndex = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
				}
				DWORD dwSoundID1 = 0, dwSoundID2 = 0;
				CGoodsList::tagGoods2 *pGoods = CGoodsList::GetProperty(dwIndex);
				if (pGoods)
				{
					// ����װ����Ʒ�趨��������Ч
					dwSoundID1 = pGoods->BaseProperty.dwSoundID1;
					dwSoundID2 = pGoods->BaseProperty.dwSoundID2;
					GetGame()->GetAudioList()->Play(NULL, TYPE_SKILL, (WORD)dwSoundID1, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(),
						pPlayer->GetPosX()),m_bFlySoundCycle);
					if (dwSoundID2)
					{
						GetGame()->GetAudioList()->Play(NULL, TYPE_SKILL, (WORD)dwSoundID2, Vector3(pPlayer->GetPosY(), 
							pPlayer->GetHeight(), pPlayer->GetPosX()),m_bFlySoundCycle);
					}
				}
			}
			else		// �������ﲥ�������е���Ч
			{
				char strFile[MAX_PATH];
				sprintf(strFile, "sounds/skills/%d.wav", m_lFlySound); 
				float x, y, fHeight;
				m_pMoveShape->GetLocatorPos(m_eFlySoundPosition, x, y, fHeight);
				nChannel = GetGame()->GetAudioList()->Play(strFile, x, y, fHeight, m_bFlySoundCycle);
			}
		}
		else		// ���������е���Ч
		{
			char strFile[MAX_PATH];
			sprintf(strFile, "sounds/skills/%d.wav", m_lFlySound);
			float x, y, fHeight;
			m_pMoveShape->GetLocatorPos(m_eFlySoundPosition, x, y, fHeight);
			nChannel = GetGame()->GetAudioList()->Play(strFile, x, y, fHeight, m_bFlySoundCycle);
		}
		SetFlySoundChannel(nChannel);
	}
}
// ���÷����ٶ�
void CSkillXml::FlyTime(long l)
{
	m_vecFlyTime.push_back(l);
}
// ���÷�����ʱ
void CSkillXml::FlyDelay(long l)
{
	 m_vecFlyDelay.push_back(l);
}

// ������Ч�ĸ���
void CSkillXml::FlyEffectNum(long l)
{
	m_lFlyEffectNum = l;
}

// �����ٻ���Ч����
void CSkillXml::SummonEffect(long l)
{
	m_vecSummonEffect.push_back(l);
}
// �����ٻ���Ч
void CSkillXml::SummonSound(long l)
{
	m_vecSummonSound.push_back(l);
}
// �����ٻ���������
void CSkillXml::SummonLifeTime(long l)
{
	m_lSummonLifeTime = l ;
}
// ��������ٻ�����X�᷶Χ
void CSkillXml::RandomSummonX(long l)
{
	m_lRandomSummonX = l;
}
// ��������ٻ�����Y�᷶Χ
void CSkillXml::RandomSummonY(long l)
{
	m_lRandomSummonY = l;
}
// ��������ٻ������ٻ�������
void CSkillXml::RandomSummonNum(long l)
{
	m_lRandomSummonNum = l;
}

// ��������ٻ������ٻ�����ӳٲ���ʱ��
void CSkillXml::RandomSummonDelayTime(long l)
{
	m_lRandomSummonDelayTime = l;
}

// �����Ƿ���ʾ�Ӷ������Ĺ�ӰЧ��
void CSkillXml::WeaponEffect(long l)
{
	// ��0������������ӰЧ��
	if(l > 0 && m_pMoveShape && m_pMoveShape->GetType() == TYPE_PLAYER)
	{
		// ��õ�ǰ��ҵ���������
		CGoods* pWeapon = ((CPlayer*)m_pMoveShape)->GetEquip( CPlayer::EQUIP_WEAPON );
		CDisplayModel* pDisplayModel = m_pMoveShape->GetDisplayModel();
		if( pWeapon && pDisplayModel)
		{
			//##0������;1�����ֽ�;2�����ִ�;3��ذ��;4������(˫��);5��˫�ֽ�(˫��);6��˫�ָ�(˫��);7����;8��˫��(˫��);9��ȭ��(˫��)
			long lWeaponCategory = pWeapon->GetEffectVal( GAP_WEAPON_CATEGORY, 1);
			// ���������������ù�ӰЧ��
			switch(lWeaponCategory)
			{
			case WT_SINGLE_SWORD:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','0'),TRUE);
				break;
			case WT_SINGLE_HAMMER:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','4'),TRUE);
				break;
			case WT_SINGLE_KNIFE:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','5'),TRUE);
				break;
			case WT_WAND:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','3'),TRUE);
				break;
			case WT_HEAVY_SWORD:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','2'),TRUE);
				break;
			case WT_HEAVY_AX:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','3'),TRUE);
				break;
			case WT_DOUBLE_SWORD:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','0'),TRUE);
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','1'),TRUE);
				break;
			case WT_FIST:
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','6'),TRUE);
				pDisplayModel->EnableTrailEmitter(MAKEFOURCC('0','0','0','7'),TRUE);
				break;
			default:
				pDisplayModel->SetAllTrailEmitter(FALSE);
				break;
			}
		}
	}
}


// �ı�ָ��������
void CSkillXml::ChangeProperty(long lvalue, const string& strValue)
{
	if(!strValue.empty())
	{
		CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_lSourType, m_SourID));
		if(pMoveShape)
		{
			pMoveShape->SetProperty(strValue.c_str(),lvalue);
		}
	}
}

// ���õ�ǰ�����Ĺ涨ʱ��
void CSkillXml::SetActTime(DWORD dwTime)
{
	// ֻ�Ա���Һ��Լ��ĳ�������
	if (dwTime > 0 && (m_pMoveShape == GetGame()->GetMainPlayer() || (m_pMoveShape ->GetType() == TYPE_PET && ((CPet*)m_pMoveShape)->IsMainPlayerPet())))
	{
		m_pMoveShape->SetCurSkillActTime(dwTime);
		m_pMoveShape->SetCurSkillStartTime(timeGetTime());
		if (m_pMoveShape ->GetType() == TYPE_PET)
		{
			CPet *pPet = (CPet*)m_pMoveShape;
			if (pPet->IsMainPlayerPet())
			{
				tagPetSkillInfo *pPetSkillInfo = pPet->GetPetSkillInfoByID(m_lSkillID);
				if(pPetSkillInfo == NULL)	return;
				pPetSkillInfo->lSkillStartTime = GetCurTickCount();
				pPetSkillInfo->lSkillActTime = dwTime;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ִ�м��ܽ�������
//////////////////////////////////////////////////////////////////////////
bool CSkillXml::StepEnd(long lID, long lLevel, long sType, const CGUID& sID, float fDir)
{
	m_lStep = 2;
	m_pMoveShape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(sType, sID));
	m_lSourType = sType;
	m_SourID = sID;
	m_fDir = fDir;
	m_lSkillID = lID;
	m_lSkillLevel = lLevel;
	if (m_pMoveShape)
		m_pMoveShape->SetDirEx(m_fDir);
	
	CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(lID);
	if(pSkill == NULL)
		return false;
	DWORD dwlev = (long)pSkill->vectorLevels.size() >= lLevel? (DWORD)lLevel-1:((DWORD)pSkill->vectorLevels.size() - 1);
	vector<CSkillListXml::tagLevelValue> vectEnd = pSkill->vectorLevels[dwlev].vecStepEnd;
	vector<CSkillListXml::tagLevelValue>::iterator it;
	for (it = vectEnd.begin(); it != vectEnd.end(); it++)
	{
		ExecuteStep(it->dwID, it->dwValue, it->strValue);
	}
	if(m_pSkillAI)
		m_pSkillAI->StepEndAI();
	//�ر��ʳ�����Ч��
	char strbuf[MAX_PATH]="";
	sprintf(strbuf,	"sounds/skills/%03d.wav", m_lIntonateSound);
	if(GetGame()->GetAudioList()->IsPlaying(strbuf,m_IntonateSoundChannel))
	{
		GetGame()->GetAudioList()->Stop3DSound(strbuf,m_IntonateSoundChannel);
	}
	// ɾ�����ж����������Ч
	DelAllEffectListByStep(STEP_START);
	DelAllEffectListByStep(STEP_RUN);
	DelAllEffectListByStep(STEP_END);
	return true;
}

bool CSkillXml::StepSummon()
{
	return true;
}

// �趨ɾ����Ч
void CSkillXml::DelLocEffect(long l)
{
	m_lDelLocEffect = l;
}
// �趨ɾ������
void CSkillXml::DelLocEffectIndex(long l)
{
	m_lDelLocEffectIndex = l;
}
// ��ʼɾ����Ч
void CSkillXml::StartDelEffect(long l)
{
	CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(m_lSourType, m_SourID));
	if(pMoveShape)
		pMoveShape->DelEffect(m_lDelLocEffect, m_lDelLocEffectIndex);
}


// ȡ�õ�ǰ������ָ���������Ч�б�
list<CSkillXml::tagEffectIndex> *CSkillXml::GetObjectEffectList(long lStep, long lType, CGUID guid)
{
	if(lStep<STEP_START || lStep >= STEP_NUM)
		return NULL;
	for(size_t i=0; i<m_vecEffectIndex[lStep].size(); ++i)
	{
		if((m_vecEffectIndex[lStep][i]).lType == lType && (m_vecEffectIndex[lStep][i]).guid == guid)
			return &((m_vecEffectIndex[lStep][i]).lsEffectIndex);
	}
	return NULL;
}


// ȡ�õ�ǰ�����У�ָ���׶ε�������Ч�����б�
vector<CSkillXml::tagObjectEffect> *CSkillXml::GetEffectIndex(long lStep)
{
	if(lStep<STEP_START || lStep >= STEP_NUM)
		return NULL;
	return &m_vecEffectIndex[lStep];
}


// ɾ����ǰ�����У��ض��׶ε�ָ���������Ч�б�
bool CSkillXml::DelObjectEffect(long lStep, long lTypeObj, CGUID guidObj)
{
	if(lStep < STEP_START || lStep >= STEP_NUM)
		return false;
	vector<tagObjectEffect>::iterator it = m_vecEffectIndex[lStep].begin();
	for(; it!=m_vecEffectIndex[lStep].end(); ++it)
	{
		if(it->lType == lTypeObj && it->guid == guidObj)
		{
			m_vecEffectIndex[lStep].erase(it);
			return true;
		}
	}
	return false;
}

// ɾ����ǰ���ܣ���ָ���׶ε��ƶ������������Ч
bool CSkillXml::DelAllEffectListByObject(long lStep, long lTypeObj, CGUID guidObj)
{
	if(lStep < STEP_START || lStep >= STEP_NUM)
		return false;
	list<CSkillXml::tagEffectIndex> * lsEffectIndex = GetObjectEffectList(lStep, lTypeObj, guidObj);
	if(lsEffectIndex)
	{
		// �ҵ�ָ���Ķ���
		CBaseObject * pObj = NULL;
		if(lTypeObj == TYPE_REGION)
			pObj = GetGame()->GetRegion();
		else
			pObj = GetGame()->GetRegion()->FindChildObject(lTypeObj,guidObj);
		if(pObj)
		{
			//����ö�����ڣ�ɾ����ǰ�������֮��ص�������Ч
			list<CSkillXml::tagEffectIndex>::iterator it = lsEffectIndex->begin();
			for(; it != lsEffectIndex->end(); ++it)
			{
				CEffect *pEffect = NULL;
				// ����
				if(pObj->GetType() == TYPE_REGION)
				{
					// �������Ч��ѭ�����ŵģ�����ɾ�������������ڳ����е�AI��ɾ��
					pEffect = (CEffect *)pObj->FindChildObject(TYPE_EFFECT,(*it).guidEffect);
					if(pEffect && pEffect->IsLoop())
						pEffect->SetIsDeleted(true);
						//pObj->DeleteChildObject(TYPE_EFFECT,(*it).guidEffect);
				}
				// ��һ���Monster���߳���
				else if(pObj->GetType() == TYPE_PLAYER || pObj->GetType() == TYPE_MONSTER || pObj->GetType() == TYPE_PET)
				{
					// �������Ч��ѭ�����ŵģ�����ɾ���������������Լ���AI��ɾ��
					pEffect = ((CShape *)pObj)->FindEffectByIndex((*it).guidEffect);
					if(pEffect && pEffect->IsLoop())
					{
						//pEffect->SetIsNeedAlphaFade(true);
						//pEffect->SetAlphaFadeInTimeLength(500);
						pEffect->SetIsDeleted(true);
						//((CShape *)pObj)->DelEffectByIndex((*it).guidEffect);
					}
				}
			}
		}
		lsEffectIndex->clear();
		DelObjectEffect(lStep, lTypeObj, guidObj);
		return true;
	}
	return false;
}

// ɾ����ǰ���ܣ���ָ���׶ε����ж����������Ч
bool CSkillXml::DelAllEffectListByStep(long lStep)
{
	if(lStep < STEP_START || lStep >= STEP_NUM)
		return false;
	long lTypeObj = 0;
	CGUID guidObj = CGUID::GUID_INVALID;
	while(m_vecEffectIndex[lStep].size()>0)
	{
		lTypeObj = (m_vecEffectIndex[lStep][0]).lType;
		guidObj = (m_vecEffectIndex[lStep][0]).guid;
		if(lTypeObj != 0 && guidObj != CGUID::GUID_INVALID)
			DelAllEffectListByObject(lStep,lTypeObj,guidObj);
	}
	/*for(int i=0; i<m_vecEffectIndex[lStep].size(); ++i)
	{
		lTypeObj = (m_vecEffectIndex[lStep][i]).lType;
		guidObj = (m_vecEffectIndex[lStep][i]).guid;
		if(lTypeObj != 0 && guidObj != CGUID::GUID_INVALID)
			DelAllEffectListByObject(lStep,lTypeObj,guidObj);
	}*/
	return true;
}

// ����һ��������Ч
void CSkillXml::SetFlySoundChannel(int nChannel)
{
	m_VecFlySoundChannel.push_back(nChannel);
}

// �رշ�����Ч
void CSkillXml::StopFlySound()
{
	for(size_t i=0;i<m_VecFlySoundChannel.size();i++)
	{
		int nChannel = m_VecFlySoundChannel[i];
		if(nChannel!=-1)
          GetGame()->GetAudioList()->Stop3DSound(nChannel);
	}
    m_VecFlySoundChannel.clear();
}

CPlayer *CSkillXml::ConvertFromPetOrPlayer(CMoveShape *pTransform)		//��������ת�� ָ��
{
	if(pTransform)
	{
		CPlayer* pDestPlayer = 0;
		if(pTransform->GetType() == TYPE_PLAYER)
			pDestPlayer = (CPlayer*)pTransform;
		else if(pTransform->GetType() == TYPE_PET)
			pDestPlayer = ((CPet*)pTransform)->GetMaster();

		return		pDestPlayer;
	}
	return NULL;
}

CPlayer *CSkillXml::ConvertFromPetOrPlayer(CShape *pTransform)		//��������ת�� ָ��
{
	if(pTransform)
	{
		CPlayer* pDestPlayer = 0;
		if(pTransform->GetType() == TYPE_PLAYER)
			pDestPlayer = (CPlayer*)pTransform;
		else if(pTransform->GetType() == TYPE_PET)
			pDestPlayer = ((CPet*)pTransform)->GetMaster();

		return		pDestPlayer;
	}
	return NULL;
}


/***********************************************************************/
/*zhaohang add ����װ��ʵ���� */
ConcreteDecSkillXml::ConcreteDecSkillXml(SkillComponent* com):
CSkillXml(com)
{
}
ConcreteDecSkillXml::~ConcreteDecSkillXml()
{

}
/***********************************************************************/
