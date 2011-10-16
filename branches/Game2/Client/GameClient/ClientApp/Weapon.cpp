
#include <stdafx.h>

#include "../../Engine\utility.h"
#include "../../Engine\console.h"
#include "../../Engine\math.h"

#include "../../EngineExtend\gamemap.h"
#include "../../EngineExtend\gameselectray.h"
#include "../../EngineExtend\DisplayModel.h"
#include "../../EngineExtend\GameModelManager.h"
#include "Shape.h"
#include "Weapon.h"
#include "clientregion.h"
#include "ModelTools.h"
#include "../GameClient/Game.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWeapon::CWeapon()
{
	ChangeStatus(WEAPON_ACTION0);
	SetType(TYPE_GOODS);
	m_dwWeaponType = 0;
	m_pSimpleModel = NULL;
}

CWeapon::~CWeapon()
{
	Destroy();
}
void CWeapon::Copy(CWeapon* psequip)
{
	
	CSpecialEquip::Copy(psequip);
	m_dwWeaponType = psequip->m_dwWeaponType;
	
}
void CWeapon::SetDisplayModel()
{
	CSpecialEquip::SetDisplayModel();
	DWORD dwsimplemodelid = 0;
	switch(m_dwWeaponType)
	{
	case WEAPONSHOW_DOUBLEWEAPON:
	case WEAPONSHOW_SINGLESWORD:
		dwsimplemodelid = 2010;
		break;
	case WEAPONSHOW_SINGLEHAMMER:
		dwsimplemodelid = 2020;
		break;
	case WEAPONSHOW_SINGLEKNIFE:
		dwsimplemodelid = 2030;
		break;
	case WEAPONSHOW_BIGSWORD:
		dwsimplemodelid = 2050;
		break;
	case WEAPONSHOW_BIGWANG:
		dwsimplemodelid = 2040;
		break;
	case WEAPONSHOW_BIGAX:
		dwsimplemodelid = 2060;
		break;
	case WEAPONSHOW_BOXINGLEFT:
		dwsimplemodelid = 2080;
		break;
	case WEAPONSHOW_BOXINGRIGHT:
		dwsimplemodelid = 2090;
		break;
	case WEAPONSHOW_SHIELD:
		dwsimplemodelid = 1040;
		break;
	case WEAPONSHOW_BOW:
		dwsimplemodelid = 2070;
		break;
	}
	// ���ļ���ȡͼ������
	char pszFileName[_MAX_PATH];
	GetModelFormatString(m_dwType,dwsimplemodelid,pszFileName);
	if(!GetGame()->IsFileExist(pszFileName))
	{
		Log4c::Warn(ROOT_MODULE,"model�ļ�%s�����ڡ�",pszFileName);
		OutputError("Model ID : %d %d ������\n",m_dwType,m_dwModelID);
		return;
	}

	CGameModelManager* pModelManager = CGameModelManager::GetInstance();
	if(m_pSimpleModel)
	{
		//�����ͬ���˳�
		if(m_pSimpleModel->GetGameModelID() ==
			pModelManager->GetGameModelID(pszFileName))
			return;
	}

	//�ͷ�
	pModelManager->ReleaseDisplayModel(m_pSimpleModel);

	//����
	m_pSimpleModel = pModelManager->CreateDisplayModel(pszFileName,false);
	if (m_pSimpleModel != NULL)
		m_pSimpleModel->ShowAllGroup(false);

}
void CWeapon::Destroy()
{
	CSpecialEquip::Destroy();
	CGameModelManager* pModelManager = CGameModelManager::GetInstance();
	//�ͷ�
	pModelManager->ReleaseDisplayModel(m_pSimpleModel);
	m_pSimpleModel = NULL;
}



/*
 * ����: ��ʾ����
 * ժҪ: -
 * ����: pParentObj - ������ָ��
 * ����ֵ: -
 * ����: ��ǫ
 * ��������: -
 * �޸���־:
 *	2009.04.24 - lpf
 *		�����˼�ģ����Ⱦʱ,�жϸ�����ģ��δ���������ֱ�ӷ���;
 */
void CWeapon::Display(CShape * pParentObj)
{
	if (m_bsimplemesh)
	{
		if (!m_bVisiable || pParentObj == NULL || pParentObj->GetDisplayModel() == NULL || m_pSimpleModel == NULL)
			return;
		if (pParentObj->GetDisplayModel()->GetLoadState() != Load_Did || m_pSimpleModel->GetLoadState() != Load_Did )
			return;

		m_pSimpleModel->GetAnimInfobyLocator(m_pAnimInfo, pParentObj->GetDisplayModel(), pParentObj->GetAnimInfo(), m_dwLocator);
		m_pSimpleModel->SetDirLightEnable(false);
		
		m_pSimpleModel->ProcessAnimJoint(m_pAnimInfo);
		m_pSimpleModel->ProcessLocator(m_pAnimInfo);
		// ����ģ�ͽ���Ч��
		if (pParentObj->GetIsNeedAlphaFade() && pParentObj->GetAlphaFadeMode() != CAlphaFadeObjectList::ALPHA_FADE_NONE)
		{
			if (pParentObj->GetAlphaFadeMode() == CAlphaFadeObjectList::ALPHA_FADE_IN)
			{
				long lElapseTime = timeGetTime() - pParentObj->GetAlphaFadeInStartTime();
				m_pSimpleModel->MultiplyModelAlpha((float)lElapseTime / (float)pParentObj->GetAlphaFadeInTimeLength());
				m_pSimpleModel->RenderModelTransparent();
			}else if (pParentObj->GetAlphaFadeMode() == CAlphaFadeObjectList::ALPHA_FADE_OUT)
			{
				m_pSimpleModel->MultiplyModelAlpha(pParentObj->GetAlphaValue());
				m_pSimpleModel->RenderModelTransparent();
			}
		}else
			m_pSimpleModel->RenderModel();

	}
	else
	{
		CSpecialEquip::Display(pParentObj);
	}

}



/*
 * ����: �쳣״̬��ɫʱ��������ʾ
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: ������
 * ��������: -
 * �޸���־:
 *	2009.04.24 - lpf
 *		�����˼�ģ����Ⱦʱ,�жϸ�����ģ��δ���������ֱ�ӷ���;
 */
void CWeapon::DisplayForExStateColor(CDisplayModel * pParentModel, AnimInfo * pParentAnimInfo, DWORD dwExColor)
{
	if (m_bsimplemesh)
	{
		if (!m_bVisiable || pParentModel == NULL || m_pSimpleModel == NULL)
			return;
		if (pParentModel->GetLoadState() != Load_Did || m_pSimpleModel->GetLoadState() != Load_Did)
			return;

		m_pSimpleModel->GetAnimInfobyLocator(m_pAnimInfo,pParentModel,pParentAnimInfo,m_dwLocator);
		//m_pDisplayModel->Display(m_pAnimInfo);
		m_pSimpleModel->SetDirLightEnable(false);
		m_pSimpleModel->ProcessAnimJoint(m_pAnimInfo);
		m_pSimpleModel->ProcessLocator(m_pAnimInfo);
		if ((dwExColor & 0xff000000) != 0xff000000)
		{
			dwExColor &= 0xff000000;
			dwExColor |= 0x00ffffff;
			m_pSimpleModel->SetModelColorCompel(dwExColor);
			m_pSimpleModel->ProcessAnimColor(m_pAnimInfo);
			m_pSimpleModel->RenderModelTransparent();
		}
		else
		{
			m_pSimpleModel->SetModelColorCompel(dwExColor);
			m_pSimpleModel->ProcessAnimColor(m_pAnimInfo);
			m_pSimpleModel->RenderModel();
		}

	}
	else
	{
		CSpecialEquip::DisplayForExStateColor(pParentModel,pParentAnimInfo,dwExColor);
	}
}



/*
 * ����: ����״̬��ɫʱ��������ʾ
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: ������
 * ��������: -
 * �޸���־:
 *	2009.04.24 - lpf
 *		�����˼�ģ����Ⱦʱ,�жϸ�����ģ��δ���������ֱ�ӷ���;
 */
void CWeapon::DisplayForFlashColor(CDisplayModel * pParentModel, AnimInfo * pParentAnimInfo, DWORD dwFlashColor)
{
	if (m_bsimplemesh)
	{
		if (!m_bVisiable ||pParentModel == NULL ||m_pSimpleModel == NULL)
			return;
		if (pParentModel->GetLoadState() != Load_Did || m_pSimpleModel->GetLoadState() != Load_Did)
			return;

		m_pSimpleModel->GetAnimInfobyLocator(m_pAnimInfo,pParentModel,pParentAnimInfo,m_dwLocator);
		//m_pDisplayModel->Display(m_pAnimInfo);
		m_pSimpleModel->SetDirLightEnable(false);
		m_pSimpleModel->ProcessAnimJoint(m_pAnimInfo);
		m_pSimpleModel->ProcessLocator(m_pAnimInfo);
		m_pSimpleModel->RenderModelFlash(dwFlashColor);
	}
	else
	{
		CSpecialEquip::DisplayForFlashColor(pParentModel,pParentAnimInfo,dwFlashColor);
	}
}
void CWeapon::ChangeStatus(DWORD eStatus)
{
	DWORD dwCurTime = timeGetTime();
	if (m_eStatus == eStatus)
	{
		return;
	}
	m_eStatus = eStatus;

	m_pAnimInfo->GetActionInfo()->dwCurAction = eStatus;
	m_pAnimInfo->GetActionInfo()->dwOldAction = m_pAnimInfo->GetActionInfo()->dwCurAction;
	m_pAnimInfo->GetActionInfo()->dwOldActionTimeElapse = m_pAnimInfo->GetActionInfo()->dwCurActionElapseTime;



	m_pAnimInfo->GetActionInfo()->bInterpolation = FALSE;
	m_pAnimInfo->GetActionInfo()->dwInterpolationStartTime = dwCurTime;
	m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
	m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 240;

	m_pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
	m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
	m_pAnimInfo->GetActionInfo()->dwCurActionStartTime = dwCurTime;
	
}


