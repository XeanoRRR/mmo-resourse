
#include <stdafx.h>

#include "../../Engine\utility.h"
#include "../../Engine\console.h"
#include "../../Engine\math.h"

#include "../../EngineExtend\Gamemap.h"
#include "../../EngineExtend\GameSelectray.h"
#include "MoveShape.h"
#include "../GameClient/Game.h"
#include "Horse.h"
#include "ClientRegion.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CHorse::CHorse()
{
	m_iHorsetype = 1;
	ChangeStatus(HS_IDLE);
	SetType(TYPE_MONSTER);
	
}

CHorse::~CHorse()
{
}
void CHorse::ChangeStatus(DWORD eStatus,bool bforceupdate)
{
	DWORD dwCurTime = timeGetTime();
	if ((m_eStatus == eStatus||m_iHorsetype == 0)&&!bforceupdate)
	{
		return;
	}
	char strtype[] = 
	{
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'1'
	};
	bool bchangestate = FALSE;
	switch(eStatus)
	{
	case HS_IDLE:
		m_pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		m_pAnimInfo->GetActionInfo()->dwOldAction = m_pAnimInfo->GetActionInfo()->dwCurAction;
		m_pAnimInfo->GetActionInfo()->dwOldActionTimeElapse = m_pAnimInfo->GetActionInfo()->dwCurActionElapseTime;
		m_pAnimInfo->GetActionInfo()->fOldActionSpeedRate = m_pAnimInfo->GetActionInfo()->fCurActionSpeedRate;
		m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
		m_pAnimInfo->GetActionInfo()->dwInterpolationStartTime  = dwCurTime;
		m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 300;
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
	//	m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('R','I','0',strtype[m_iHorsetype - 1]);
		
		bchangestate = TRUE;
		break;

	case HS_STAND:
		m_pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		m_pAnimInfo->GetActionInfo()->dwOldAction = m_pAnimInfo->GetActionInfo()->dwCurAction;
		m_pAnimInfo->GetActionInfo()->dwOldActionTimeElapse = m_pAnimInfo->GetActionInfo()->dwCurActionElapseTime;
		m_pAnimInfo->GetActionInfo()->fOldActionSpeedRate = m_pAnimInfo->GetActionInfo()->fCurActionSpeedRate;
		m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
		m_pAnimInfo->GetActionInfo()->dwInterpolationStartTime  = dwCurTime;
		m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 300;
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
		//m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('R','S','0',strtype[m_iHorsetype - 1]);

		
		bchangestate = TRUE;
		break;
	case HS_WALK:
		/*if ( m_eStatus == eStatus )
			break;
		m_pAnimInfo->GetActionInfo()->dwOldAction = m_pAnimInfo->GetActionInfo()->dwCurAction;
		m_pAnimInfo->GetActionInfo()->dwOldActionTimeElapse = m_pAnimInfo->GetActionInfo()->dwCurActionElapseTime;
		m_pAnimInfo->GetActionInfo()->fOldActionSpeedRate = m_pAnimInfo->GetActionInfo()->fCurActionSpeedRate;
		m_pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
		m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 240;
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
		m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		m_pAnimInfo->GetActionInfo()->dwCurActionStartTime = dwCurTime;*/
		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('R','W','0',strtype[m_iHorsetype - 1]);
		bchangestate = TRUE;
		break;
	case HS_RUN:
		/*if ( m_eStatus == eStatus )
			break;
		m_pAnimInfo->GetActionInfo()->dwOldAction = m_pAnimInfo->GetActionInfo()->dwCurAction;
		m_pAnimInfo->GetActionInfo()->dwOldActionTimeElapse = m_pAnimInfo->GetActionInfo()->dwCurActionElapseTime;
		m_pAnimInfo->GetActionInfo()->fOldActionSpeedRate = m_pAnimInfo->GetActionInfo()->fCurActionSpeedRate;
		m_pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
		m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 240;
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
		m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		m_pAnimInfo->GetActionInfo()->dwCurActionStartTime = dwCurTime;*/
		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('R','R','0',strtype[m_iHorsetype - 1]);
		bchangestate = TRUE;
		break;
	case HS_HURT:
		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('R','H','0',strtype[m_iHorsetype - 1]);
		/*m_pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		m_pAnimInfo->GetActionInfo()->dwOldAction = m_pAnimInfo->GetActionInfo()->dwCurAction;
		m_pAnimInfo->GetActionInfo()->dwOldActionTimeElapse = m_pAnimInfo->GetActionInfo()->dwCurActionElapseTime;
		m_pAnimInfo->GetActionInfo()->fOldActionSpeedRate = m_pAnimInfo->GetActionInfo()->fCurActionSpeedRate;
		m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
		m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 240;
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = FALSE;
		m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		m_pAnimInfo->GetActionInfo()->dwCurActionStartTime = dwCurTime;*/
		bchangestate = TRUE;
		break;
	case HS_MISS:
		/*m_pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		m_pAnimInfo->GetActionInfo()->dwOldAction = m_pAnimInfo->GetActionInfo()->dwCurAction;
		m_pAnimInfo->GetActionInfo()->dwOldActionTimeElapse = m_pAnimInfo->GetActionInfo()->dwCurActionElapseTime;
		m_pAnimInfo->GetActionInfo()->fOldActionSpeedRate = m_pAnimInfo->GetActionInfo()->fCurActionSpeedRate;
		m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
		m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 240;
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = FALSE;
		m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		m_pAnimInfo->GetActionInfo()->dwCurActionStartTime = dwCurTime;*/
		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('R','K','0',strtype[m_iHorsetype - 1]);
		bchangestate = TRUE;
		break;
	case HS_MEGRIM:
		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('R','Y','0',strtype[m_iHorsetype - 1]);
		/*m_pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		m_pAnimInfo->GetActionInfo()->dwInterpolationElapseTime = 0;
		m_pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 240;
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = FALSE;
		m_pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		m_pAnimInfo->GetActionInfo()->dwCurActionStartTime = dwCurTime;*/
		bchangestate = TRUE;
		break;
	}
	if (bchangestate)
	{
		m_eStatus = eStatus;
	}
}



/*
 * ����: ��ʾ����
 * ժҪ: pParentObj	   - �������
 *		 bIsUpdateAnim - �Ƿ���¶�����Ϣ
 * ����: -
 * ����ֵ: -
 * ����: ��ǫ
 * ��������: -
 * �޸���־:
 *	2008.10.23 - lpf
 *		�����������Ϊһ�����������,��������һ��bIsUpdateAnim����;
 *		�����˽��䶯������;
 *	2009.02.22 - lpf
 *		�����˵����׶������쳣״̬��ɫʱ,����ҲҪͬ��������ɫ;
 *	2009.04.24 - lpf
 *		�������жϸ�����ģ��δ���������ֱ�ӷ���;
 */
void CHorse::Display(CMoveShape * pParentObj, bool bIsUpdateAnim)
{
	float fX = pParentObj->GetPosX() + pParentObj->GetXIncrement();
	float fY = pParentObj->GetPosY() + pParentObj->GetYIncrement();
	float fH = pParentObj->GetHeight();
	float fDir = pParentObj->GetNowDir();

	if (!m_bVisiable || NULL == m_pDisplayModel)
		return;
	if (pParentObj->GetDisplayModel()->GetLoadState() != Load_Did || m_pDisplayModel->GetLoadState() != Load_Did)
		return;

	// �õ����
	CClientRegion * pRegion = GetGame()->GetRegion();
	if(pRegion == NULL)
		return;
	m_pDisplayModel->SetRendSimpleMesh(m_bsimplemesh);
	render::Camera * pCamera = pRegion->GetCamera();
	D3DXVECTOR3 GridNormal = pRegion->GetGameMap()->GetGridNormal((int)fY,(int)fX);
	D3DXVECTOR3 RotVector;
	D3DXMATRIX matrot,matyrot;
	float angle = FM_AngleBetweenVectors(D3DXVECTOR3(0,1,0),GridNormal);
	D3DXVec3Cross(&RotVector,&D3DXVECTOR3(0,1,0),&GridNormal);
	D3DXMatrixRotationAxis(&matrot,&RotVector,angle);
	D3DXMatrixRotationY(&matyrot,fDir);
	matrot =matyrot * matrot;
	
	if (!bIsUpdateAnim)
	{
		DWORD dwCurrent = m_pAnimInfo->GetCurrentTime();
		m_pAnimInfo->SetupAnimInfoRotMatrix(fX,fY,fH,matrot,pCamera);
		m_pAnimInfo->SetCurrentTime(dwCurrent);
	}else
		m_pAnimInfo->SetupAnimInfoRotMatrix(fX,fY,fH,matrot,pCamera);

	m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
	m_pDisplayModel->ProcessVisibility(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
	m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
	m_pDisplayModel->ProcessLocator(m_pAnimInfo);
	m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
	m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
	//m_pDisplayModel->Display(m_pAnimInfo);

	// ��ʾģ��
	if (pParentObj->IsStateColorPlay())
	{
		m_pDisplayModel->SetModelColorCompel(pParentObj->GetCurrentModelColor());
		m_pDisplayModel->RenderModelTransparent();
	}else
	{
		// ����ģ�ͽ���Ч��
		if (pParentObj->GetIsNeedAlphaFade() && pParentObj->GetAlphaFadeMode() != CAlphaFadeObjectList::ALPHA_FADE_NONE)
		{
			if (pParentObj->GetAlphaFadeMode() == CAlphaFadeObjectList::ALPHA_FADE_IN)
			{
				CShape * pShape = dynamic_cast<CShape *>(pParentObj);
				long lElapseTime = timeGetTime() - pShape->GetAlphaFadeInStartTime();

				m_pDisplayModel->MultiplyModelAlpha((float)lElapseTime / (float)pShape->GetAlphaFadeInTimeLength());
				m_pDisplayModel->RenderModelTransparent();
			}else if (pParentObj->GetAlphaFadeMode() == CAlphaFadeObjectList::ALPHA_FADE_OUT)
			{
				CShape * pShape = dynamic_cast<CShape *>(pParentObj);
				m_pDisplayModel->MultiplyModelAlpha(pShape->GetAlphaValue());
				m_pDisplayModel->RenderModelTransparent();
			}
		}else
			m_pDisplayModel->RenderModel();
	}
}