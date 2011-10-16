
#include <stdafx.h>

#include "../../Engine\utility.h"
#include "../../Engine\console.h"
#include "../../Engine\math.h"

#include "../../EngineExtend\DisplayModel.h"
#include "../../EngineExtend\TextureProjective.h"
#include "../../EngineExtend\gamemap.h"
#include "../../EngineExtend\GameModelManager.h"
#include "../../EngineExtend\gameselectray.h"

#include "specialequip.h"
#include "clientregion.h"
#include "../GameClient/Game.h"
#include "ModelTools.h"
#include "Shape.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSpecialEquip::CSpecialEquip()
:m_pDisplayModel(NULL)
{
	m_dwModelID = 0;
	m_dwLevel  = 0;
	m_dwType   = 0000;
	m_bVisiable = FALSE;
	m_eStatus = -1;
	m_pAnimInfo = new AnimInfo();
	m_bsimplemesh = false;
	ZeroMemory(m_pAnimInfo->GetActionInfo(),sizeof(AnimInfo::tagActionInfo));
}

CSpecialEquip::~CSpecialEquip()
{
	Destroy();
	SAFE_DELETE(m_pAnimInfo);
}



/*
* ����: ǿ���趨װ����ʾģ����ɫ
* ժҪ: -
* ����: ��Ҫ�趨����ɫֵ
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.15
*/
void CSpecialEquip::SetModelColorCompel(DWORD dwColor)
{
	// ��������
	static D3DXVECTOR3 vLightVector(-0.613793f, -0.789465f, 0.001831f);

	if (!m_bVisiable || m_pDisplayModel == NULL)
		return;

	if (m_pDisplayModel->GetLoadState() != Load_Did)
		return;

	// �õ����
	CClientRegion * pRegion = GetGame()->GetRegion();
	if(pRegion == NULL)
		return;

	render::Camera * pCamera = pRegion->GetCamera();

	m_pDisplayModel->SetModelColorCompel(dwColor);
	//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(), &vLightVector, dwColor);
}

void CSpecialEquip::Destroy()
{
	CGameModelManager* pModelManager = CGameModelManager::GetInstance();
	//�ͷ�
	pModelManager->ReleaseDisplayModel(m_pDisplayModel);
	m_pDisplayModel = NULL;
}
void CSpecialEquip::Copy(CSpecialEquip* psequip)
{
	m_dwModelID = psequip->m_dwModelID;
	m_dwLevel = psequip->m_dwLevel;
	m_dwType = psequip->m_dwType;
	m_eStatus = psequip->m_eStatus;
	m_bVisiable = psequip->m_bVisiable;
	m_dwLocator = psequip->m_dwLocator;
};




/*
* ����: ������ʾģ��
* ժҪ: -
* ����: ��Ҫ�趨����ɫֵ
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.12.30 - lpf
*		��"����"��,������ʾģ���Ƿ�Ϊ�յ��ж�,�������ڴ���ʧ�ܷ��صĿ�ģ��ָ����ɵĵ���
*/
void CSpecialEquip::SetDisplayModel()
{
	if(!m_bVisiable)
	{
		Destroy();
		return;
	}
	// ���ļ���ȡͼ������
	char pszFileName[_MAX_PATH];
	GetModelFormatString(m_dwType,m_dwModelID,pszFileName);
	if(!GetGame()->IsFileExist(pszFileName))
	{
		Log4c::Warn(ROOT_MODULE,"model�ļ�%s�����ڡ�",pszFileName);
		OutputError("Model ID : %d %d ������\n",m_dwType,m_dwModelID);
		return;
	}

	CGameModelManager* pModelManager = CGameModelManager::GetInstance();
	if(m_pDisplayModel)
	{
		//�����ͬ���˳�
		if(m_pDisplayModel->GetGameModelID() ==
			pModelManager->GetGameModelID(pszFileName))
			return;
	}

	//�ͷ�
	pModelManager->ReleaseDisplayModel(m_pDisplayModel);

	//����
	m_pDisplayModel = pModelManager->CreateDisplayModel(pszFileName,false);
	if (m_pDisplayModel != NULL)
		m_pDisplayModel->ShowAllGroup(false);
}



//���õȼ�
void CSpecialEquip::SetLevel(DWORD level)
{
	if (!m_pDisplayModel)
	{
		return;
	}
	m_dwLevel = level;
	m_pDisplayModel->ShowAllGroupbyLevel(level,false);
	/*DWORD groupname;
	DWORD partname;
	CDisplayModel::CDisplayGroup* pGroup;
	CDisplayModel::itDisplayGroup it = m_pDisplayModel->GetGroups().begin();
	for (;it != m_pDisplayModel->GetGroups().end();it++ )
	{
	groupname = (*it).first;
	pGroup = (*it).second;
	partname = pGroup->GetPartName();
	m_pDisplayModel->ShowGroup(groupname,partname,level,false);
	}*/

}
//����
void CSpecialEquip::Hide()
{
	Destroy();
}



/*
* ����: ��ʾ����
* ժҪ: -
* ����: pParentObj - ������ָ��
* ����ֵ: -
* ����: ��ǫ
* ��������: -
* �޸���־:
*	2008.10.15 - lpf
*		 ���ú�������������CDisplay *��AnimInfo *�޸�ΪCShape *,�Դ��������Ϣ;
*	2009.04.24 - lpf
*		�������жϸ�����ģ��δ���������ֱ�ӷ���;
*/
void CSpecialEquip::Display(CShape * pParentObj)
{
	if (!m_bVisiable || pParentObj == NULL || pParentObj->GetDisplayModel() == NULL || m_pDisplayModel == NULL)
		return;
	if (pParentObj->GetDisplayModel()->GetLoadState() != Load_Did || m_pDisplayModel->GetLoadState() != Load_Did )
		return;

	m_pDisplayModel->GetAnimInfobyLocator(m_pAnimInfo, pParentObj->GetDisplayModel(), pParentObj->GetAnimInfo(), m_dwLocator);
	//m_pDisplayModel->Display(m_pAnimInfo);
	m_pDisplayModel->SetDirLightEnable(false);
	m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
	m_pDisplayModel->ProcessVisibility(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
	m_pDisplayModel->ProcessEnvironmentMaps(pParentObj->GetAnimInfo()->GetViewMatrix());
	m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
	m_pDisplayModel->ProcessLocator(m_pAnimInfo);
	m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);

	// ����ģ�ͽ���Ч��
	if (pParentObj->GetIsNeedAlphaFade() && pParentObj->GetAlphaFadeMode() != CAlphaFadeObjectList::ALPHA_FADE_NONE)
	{
		if (pParentObj->GetAlphaFadeMode() == CAlphaFadeObjectList::ALPHA_FADE_IN)
		{
			long lElapseTime = timeGetTime() - pParentObj->GetAlphaFadeInStartTime();
			m_pDisplayModel->MultiplyModelAlpha((float)lElapseTime / (float)pParentObj->GetAlphaFadeInTimeLength());
			m_pDisplayModel->RenderModelTransparent();
		}else if (pParentObj->GetAlphaFadeMode() == CAlphaFadeObjectList::ALPHA_FADE_OUT)
		{
			m_pDisplayModel->MultiplyModelAlpha(pParentObj->GetAlphaValue());
			m_pDisplayModel->RenderModelTransparent();
		}
	}else
		m_pDisplayModel->RenderModel();
}



/*
* ����: ��ʾװ������ɫ������Ч��(ǿ�ƽ��ܵƹ⴦��)
* ժҪ: -
* ����: pParentModel    - ����ʾģ��ָ��
*		 pParentAnimInfo - ��ģ�Ͷ�����Ϣָ��
*		 dwFlashColor	 - �����������ɫ
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.12
* �޸���־:
*	2009.04.24 - lpf
*		�������жϸ�����ģ��δ���������ֱ�ӷ���;
*/
void CSpecialEquip::DisplayForFlashColor(CDisplayModel * pParentModel, AnimInfo * pParentAnimInfo, DWORD dwFlashColor)
{
	if (!m_bVisiable || pParentModel == NULL || m_pDisplayModel == NULL)
		return;
	if (pParentModel->GetLoadState() != Load_Did || m_pDisplayModel->GetLoadState() != Load_Did)
		return;

	m_pDisplayModel->GetAnimInfobyLocator(m_pAnimInfo,pParentModel,pParentAnimInfo,m_dwLocator);
	//m_pDisplayModel->Display(m_pAnimInfo);
	m_pDisplayModel->SetDirLightEnable(false);
	m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
	m_pDisplayModel->ProcessVisibility(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
	m_pDisplayModel->ProcessEnvironmentMaps(pParentAnimInfo->GetViewMatrix());
	m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
	m_pDisplayModel->ProcessLocator(m_pAnimInfo);
	m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);

	m_pDisplayModel->RenderModelFlash(dwFlashColor);
}



/*
* ����: ��ʾװ�����쳣״̬��ɫ������Ч��(ǿ�ƽ��ܵƹ⴦��)
* ժҪ: -
* ����: pParentModel    - ����ʾģ��ָ��
*		 pParentAnimInfo - ��ģ�Ͷ�����Ϣָ��
*		 dwExColor		 - �쳣����ɫ
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.16
* �޸���־:
*	2008.03.11 - lpf
*		��Ⱦ�쳣״̬�ĺ����޸�ΪRenderModelTransparent(),������Ⱦ͸������
*	2009.04.24 - lpf
*		�������жϸ�����ģ��δ���������ֱ�ӷ���;
*/
void CSpecialEquip::DisplayForExStateColor(CDisplayModel * pParentModel, AnimInfo * pParentAnimInfo, DWORD dwExColor)
{
	if (!m_bVisiable || pParentModel == NULL || m_pDisplayModel == NULL)
		return;
	if (pParentModel->GetLoadState() != Load_Did || m_pDisplayModel->GetLoadState() != Load_Did)
		return;

	m_pDisplayModel->GetAnimInfobyLocator(m_pAnimInfo,pParentModel,pParentAnimInfo,m_dwLocator);
	//m_pDisplayModel->Display(m_pAnimInfo);
	m_pDisplayModel->SetDirLightEnable(false);
	m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
	m_pDisplayModel->ProcessVisibility(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
	m_pDisplayModel->ProcessEnvironmentMaps(pParentAnimInfo->GetViewMatrix());
	m_pDisplayModel->ProcessLocator(m_pAnimInfo);
	m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
	if ((dwExColor & 0xff000000) != 0xff000000)
	{
		dwExColor &= 0xff000000;
		dwExColor |= 0x00ffffff;
		m_pDisplayModel->SetModelColorCompel(dwExColor);
		m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
		m_pDisplayModel->RenderModelTransparent();
	}
	else
	{
		m_pDisplayModel->SetModelColorCompel(dwExColor);
		m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
		m_pDisplayModel->RenderModel();
	}

	//	//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(), &vLightVector, dwColor);

	//m_pDisplayModel->RenderModel();


}