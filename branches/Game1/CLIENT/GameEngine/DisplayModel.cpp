//=============================================================================
/**
*  file: DisplayModel.h
*
*  Brief:����һ��������Ҫ��ʾģ�͵���Դ
*
*  Authtor:wangqiao
*	
*	Datae:2007-10-25
*/
//=============================================================================
#include "stdafx.h"
#include "displaymodel.h"
#include "GameModelManager.h"
#include "textureprojective.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


AnimInfo::AnimInfo(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matview);
	D3DXMatrixIdentity(&m_matscal);
	D3DXMatrixIdentity(&m_matBillboard);
	D3DXMatrixIdentity(&m_matBillboardY);
	m_dwAnimColorStartTime = 0;
	ZeroMemory(&m_tActionInfo,sizeof(tagActionInfo));
	ZeroMemory(&m_dwParticleEmitterLastTimeArray,GAMEMODEL_MAXPARTICLEEMITTER);

	m_tActionInfo.fCurActionSpeedRate = 1.0f;
	m_tActionInfo.fOldActionSpeedRate = 1.0f;

	//����
	m_pTrailContainerArray = new GameModel::TrailContainer[GAMEMODEL_MAXTRAILEMITTER]; 
	m_bIsProcessSelfAnim = false;
	m_fAlpha = -1.0f;
	m_fDirY = 0.0f;
}

//����AnimInfo
AnimInfo::~AnimInfo(void)
{
	SAFEDELETEARRAY(m_pTrailContainerArray);
}

void AnimInfo::ClearTrailContainer(void)
{
	for(DWORD n = 0; n < GAMEMODEL_MAXTRAILEMITTER;n++)
	{
		m_pTrailContainerArray[n].Clear();
	}
}

void AnimInfo::DeleteTrailContainer(void)
{
	for(DWORD n = 0; n < GAMEMODEL_MAXTRAILEMITTER;n++)
	{
		m_pTrailContainerArray[n].Clear();
	}

	SAFE_DELETE_ARRAY(m_pTrailContainerArray);
}

void AnimInfo::CaculateJointPos(D3DXVECTOR3 *vecSrcPos, D3DXVECTOR3 *vecTargetPos, DWORD dwjointCount)
{
	if (dwjointCount != 3)
		return;
	m_bIsProcessSelfAnim = true;
	
	float average_x=0.0f, average_y = 0.0f,average_z =0.0f;
	
	average_x = (vecTargetPos->x - vecSrcPos->x) / (dwjointCount - 2);
	average_y = (vecTargetPos->y - vecSrcPos->y) / (dwjointCount - 2);
	average_z = (vecTargetPos->z - vecSrcPos->z) / (dwjointCount - 2);

	m_vec3JointArrayPos[0].x = 0;
	m_vec3JointArrayPos[0].y = 0;
	m_vec3JointArrayPos[0].z = 0;
	m_vec3JointArrayPos[1].x = m_vec3JointArrayPos[2].x = average_x;
	m_vec3JointArrayPos[1].y = m_vec3JointArrayPos[2].y = average_y;
	m_vec3JointArrayPos[1].z = m_vec3JointArrayPos[2].z = average_z;
}
void AnimInfo::SetupAnimInfo(float fX,float fY,float fH,
							 float fDir,render::Camera * pCamera)
{
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld,fY,fH,fX);
	D3DXMATRIX matRot,matscal;
	D3DXMatrixRotationY(&matRot,fDir);
	//D3DXMatrixMultiply(&matRot,&m_matscal,&matRot);
	D3DXMatrixMultiply(&matWorld,&matRot,&matWorld);

	DWORD newtime = timeGetTime();
	SetCurrentTime(newtime);
	SetDirY(fDir);
	SetWorldMatrix(&matWorld);
	SetViewMatrix(pCamera->GetViewMatrix());
	//SetScalMatrix(&m_matscal);
	SetProjectedMatrix(pCamera->GetProjectionMatrix());
	SetBillboard(pCamera->GetBillboard());
	SetBillboardY(pCamera->GetBillboardY());
}
void AnimInfo::SetupAnimInfoRotMatrix(float fX,float fY,float fH,
									  D3DXMATRIX mat3DDir,render::Camera * pCamera)
{
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld,fY,fH,fX);
	D3DXMatrixMultiply(&matWorld,&mat3DDir,&matWorld);

	DWORD newtime = timeGetTime();
	SetCurrentTime(newtime);

	SetWorldMatrix(&matWorld);
	SetViewMatrix(pCamera->GetViewMatrix());
	SetProjectedMatrix(pCamera->GetProjectionMatrix());
	//SetScalMatrix(&m_matscal);
	SetBillboard(pCamera->GetBillboard());
	SetBillboardY(pCamera->GetBillboardY());
}

CDisplayModel::CDisplayModel(CGameModelManager* pGameModelManager)
:m_pGameModelManager(pGameModelManager)
,m_bAllGroup(FALSE)
,m_nAllTrailEnable(-1)
,m_nAllParticleEnable(-1)
,m_pPickBox(NULL)
,m_pPickMesh(NULL)
,m_pSizeBox(NULL)
,m_pWater(NULL)
,m_eGroupLoadState(Load_Not)
,m_eParticleLoadState(Load_Not)
,m_dwShaderGroupHandle(0)
{
	m_dwAnimColorCircle=5000;
	m_dwAnimUVCircle=5000;
	m_dwAnimVisibilityCircle=5000;
	m_dwAnimTextureCircle=5000;
	m_brendsimplemesh = false;
}

CDisplayModel::~CDisplayModel(void)
{
	ReleaseAll();
}

CDisplayModel::CDisplayGroup::CDisplayGroup(void)
:m_dwPartName(-1)
,m_dwPartLevel(-1)
,m_pLevel(NULL)
{
}


CDisplayModel::CDisplayGroup::~CDisplayGroup(void)
{
	ReleaseRenderMesh();
}

void CDisplayModel::CDisplayGroup::ReleaseRenderMesh()
{
}

void CDisplayModel::CDisplayGroup::ReleaseLevelInfo()
{
	m_pLevel = NULL;
}


void CDisplayModel::CDisplayGroup::SetLevelInfo(GameModel::Group::Part::Level *pLevel)
{
	m_pLevel = pLevel;
}

//�õ������θ���
DWORD CDisplayModel::CDisplayGroup::GetTriangleCount(void)
{
	if(NULL == m_pLevel)	return 0;

	DWORD dwTriangleCount = 0;
	if(NULL == m_pLevel)	return 0;
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		model::Mesh *pMesh = pRenderMesh->pMesh;
		dwTriangleCount += pMesh->GetTriangleCount();
	}
	return dwTriangleCount;
}

void CDisplayModel::CDisplayGroup::SetColor(DWORD dwColor)
{
	if(NULL == m_pLevel)	return;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState()!= Load_Did) continue;

		if (pRenderMesh->bEffectByLight)
		{
			model::Buffer *pBuffer = pRenderMesh->pBuffer;
			if (pBuffer->GetGpuCaculate())
			{
				pBuffer->SetBaseColor(dwColor);
			}
			else
			{
				D3DCOLOR	  *pColBuffer = pBuffer->GetColBufferDiffuse();
				DWORD dwColCount = pBuffer->GetPNCCount();
				if (pBuffer->GetColBufferDiffuse())
				{
					_asm
					{
						mov edi,[pColBuffer]
						mov eax,dwColor
							cld 
							mov ecx,dwColCount
							rep stosd
					}
				}

			}


		}
	}
}
void CDisplayModel::CDisplayGroup::ForceToEnableLight()
{
	//��������Ż�֮����ʱ����ʹ��!!!
	//���Դ���Ժ���SHADERʵ��
	return;
	if(NULL == m_pLevel)	return;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState()!= Load_Did) continue;
		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		pBuffer->SetUseNormal(true);
		pBuffer->SetLightEnable(true);
	}
}
void CDisplayModel::CDisplayGroup::SetDirLightEnable(bool enable)
{
	if(NULL == m_pLevel)	return;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState()!= Load_Did) continue;
		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		if (pRenderMesh->bEffectByLight)
		{
			if (!enable||(m_pLevel->GetAnimColor()&&pRenderMesh->bAnimColor)||(m_pLevel->GetAnimVisibility()&&pRenderMesh->bAnimVisibility))
			{
				pBuffer->SetLightEnable(false);

				if (!pRenderMesh->bEnvironmentMaps)
				{
					pBuffer->SetUseNormal(false);
				}
				else
				{
					pBuffer->SetUseNormal(true);
				}
			}
			else
			{

				pBuffer->SetLightEnable(true);

				pBuffer->SetUseNormal(true);


			}
		}
		else
		{

			if (!pRenderMesh->bEnvironmentMaps)
			{
				pBuffer->SetUseNormal(false);
			}
			else
			{
				pBuffer->SetUseNormal(true);
			}
			pBuffer->SetLightEnable(false);
		}
		pBuffer->SetNoAnim(true);
		pBuffer->SetUseEnvTexture(false);
		pBuffer->SetUseUVAnim(false);
		pBuffer->SetNormalTransScal(false);
		pBuffer->SetTextureProjected(false);
	}

}
void CDisplayModel::CDisplayGroup::SetUseNormal(bool busenormal)
{
	if(NULL == m_pLevel)	return;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState()!= Load_Did) continue;
		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		pBuffer->SetUseNormal(busenormal);

	}
}
//void CDisplayModel::CDisplayGroup::SetRegionAmbient(DWORD dwShaderGroupHandle,DWORD  m)
//{
//	if(NULL == m_pLevel)	return;
//
//	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
//	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
//	{
//		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
//		if(pRenderMesh->GetLoadState()!= Load_Did) continue;
//		model::Buffer *pBuffer = pRenderMesh->pBuffer;
//		if (pRenderMesh->bEffectByLight)
//		{
//			pBuffer->SetRegionAmbient(m);
//		}
//		else
//		{
//			pBuffer->SetRegionAmbient(0xffffffff);
//			pBuffer->SetAmbient(0xffffffff);
//			pBuffer->SetTempStreamType(dwShaderGroupHandle,LMST_POSCOLTEX0);
//			pBuffer->SetUseNormal(false);
//
//		}
//		
//			
//		
//	}
//}
//void CDisplayModel::CDisplayGroup::SetAmbient(DWORD dwShaderGroupHandle,DWORD dwcolor)
//{
//	if(NULL == m_pLevel)	return;
//
//	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
//	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
//	{
//		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
//		if(pRenderMesh->GetLoadState()!= Load_Did) continue;
//			model::Buffer *pBuffer = pRenderMesh->pBuffer;
//			if (pRenderMesh->bEffectByLight)
//			{
//				pBuffer->SetAmbient(dwcolor);
//			}
//	}
//}


/*
* ����: �ܿ�ģ����ʾ���Ƿ��ܵƹ�Ч��Ӱ�������,ǿ��Ϊģ����ʾ���趨��ɫ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.12
*/
void CDisplayModel::CDisplayGroup::SetColorCompel(DWORD dwColor)
{
	if (m_pLevel == NULL)
		return;

	GameModel::Group::Part::LevelMeshes & vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i < m_pLevel->GetRMCount(); ++i)
	{
		GameModel::Group::Part::tagRenderMesh * pRenderMesh = vRenderMeshes[i];
		if (pRenderMesh->GetLoadState() != Load_Did)
			continue;

		model::Buffer * pBuffer	   = pRenderMesh->pBuffer;
		if (pBuffer->GetGpuCaculate())
		{
			pBuffer->SetBaseColor(dwColor);
		}
		else
		{
			D3DCOLOR	  * pColBuffer = pBuffer->GetColBufferDiffuse();
			DWORD			dwColCount = pBuffer->GetPNCCount();
			_asm
			{
				mov edi, [pColBuffer]
				mov eax, dwColor
					cld 
					mov ecx, dwColCount
					rep stosd
			}
		}

	}
}



void CDisplayModel::CDisplayGroup::MultiplyColor(DWORD dwColor)
{
	if(NULL == m_pLevel)	return;



	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState()!= Load_Did) continue;

		if (pRenderMesh->bEffectByLight)
		{
			model::Buffer *pBuffer = pRenderMesh->pBuffer;

			D3DXVECTOR3   *pPosBuffer  = pBuffer->GetPosBuffer();
			D3DXVECTOR3   *pNmlBuffer  = pBuffer->GetNmlBuffer();
			D3DCOLOR	  *pColBuffer  = pBuffer->GetColBufferDiffuse();
			if (pBuffer->GetGpuCaculate())
			{
				pBuffer->SetMultiColor(dwColor);
				/*DWORD &dwDestColor = pBuffer->GetBaseColor();

				DWORD dwRD =  (dwDestColor & 0x00FF0000) >> 16;
				DWORD dwGD =  (dwDestColor & 0x0000FF00) >>  8;
				DWORD dwBD =  (dwDestColor & 0x000000FF);

				DWORD r = (dwR * dwRD ) >> 8;  	
				DWORD g = (dwG * dwGD ) >> 8;  	
				DWORD b = (dwB * dwBD ) >> 8;  	
				dwDestColor = (dwDestColor & 0xFF000000) | (r << 16) | (g << 8) | b;*/
			}
			else
			{
				DWORD dwColCount = pBuffer->GetPNCCount();
				DWORD  dwR = (dwColor & 0x00FF0000) >> 16;
				DWORD  dwG = (dwColor & 0x0000FF00) >>  8;
				DWORD  dwB = dwColor & 0x000000FF;

				for(DWORD n = 0; n < dwColCount;n++)
				{
					DWORD &dwDestColor = pColBuffer[n];

					DWORD dwRD =  (dwDestColor & 0x00FF0000) >> 16;
					DWORD dwGD =  (dwDestColor & 0x0000FF00) >>  8;
					DWORD dwBD =  (dwDestColor & 0x000000FF);

					DWORD r = (dwR * dwRD ) >> 8;  	
					DWORD g = (dwG * dwGD ) >> 8;  	
					DWORD b = (dwB * dwBD ) >> 8;  	
					dwDestColor = (dwDestColor & 0xFF000000) | (r << 16) | (g << 8) | b;
				}

			}

		}
	}
}

void CDisplayModel::CDisplayGroup::MultiplyAlpha(float fRatio)
{
	if(NULL == m_pLevel)	return;

	DWORD  dwRatio = (DWORD)(fRatio * 255.0f) & 0xFF;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState()!= Load_Did) continue;

		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		D3DCOLOR	  *pColBuffer  = pBuffer->GetColBufferDiffuse();
		if (pBuffer->GetGpuCaculate())
		{
			DWORD &dwDestColor = pBuffer->GetBaseColor();

			dwDestColor &= 0X00FFFFFF;
			dwDestColor |= (dwRatio << 24);

		}
		else
		{
			DWORD dwColCount = pBuffer->GetPNCCount();
			for(DWORD n = 0; n < dwColCount;n++)
			{
				DWORD &dwDestColor = pColBuffer[n];

				dwDestColor &= 0X00FFFFFF;
				dwDestColor |= (dwRatio << 24);
			}

		}

	}
}



/*
* ����: ��������Ϣ
* ժҪ: -
* ����: pAnimInfo			   - ������Ϣ
*		 pOldAnimJoint		   - �ϵĶ����ؽ���Ϣ
*		 pCurAnimJoint		   - ��ǰ�����ؽ���Ϣ
*		 dwOldActionTimeElapse - �ϵĶ�������ʱ��
*		 dwCurActionTimeElapse - ��ǰ��������ʱ��
*		 pBindPose			   - �󶨵���Ϣ
*		 fRatio				   - ��ֵ����ϵ��
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.03.27 - lpf
*		��ProcessAnimJointInterpolation()����ʱ,�������ʲ���
*/
void  CDisplayModel::CDisplayGroup::ProcessAnimJoint(
	const AnimInfo *pAnimInfo,model::AnimJoint * pOldAnimJoint,
	model::AnimJoint *pCurAnimJoint,
	DWORD dwOldActionTimeElapse,DWORD dwCurActionTimeElapse,
	model::BindPose *pBindPose,float fRatio)
{
	if(NULL == m_pLevel)	return;

	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;
		D3DXMATRIX matrixscal;
		D3DXMatrixIdentity(&matrixscal);

		model::Mesh   *pMesh   = pRenderMesh->pMesh;
		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		if (pRenderMesh->bProjected)
		{
			pBuffer->SetMatFotTP(pAnimInfo->GetMatrixForTP());
		}
		if (!D3DXMatrixIsIdentity(pAnimInfo->GetScalMatrix()) )
		{
			pBuffer->SetNormalTransScal(true);
		}
		if (pRenderMesh->bBillboard)
		{
			pBuffer->SetIsBillBoard(true);
			if (pRenderMesh->bAroundYBillboard)
			{
				pBuffer->SetIsBillBoardY(true);
				pBuffer->SetMatForBB(pAnimInfo->GetBillboardY());
			}
			else
			{
				pBuffer->SetIsBillBoardY(false);
				pBuffer->SetMatForBB(pAnimInfo->GetBillboard());
			}
			if (pActionInfo->bInterpolation && pOldAnimJoint && pCurAnimJoint&&pMesh->GetGroupCount())
				pMesh->ProcessAnimJointInterpolation(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),pBindPose,pOldAnimJoint,dwOldActionTimeElapse,pActionInfo->fOldActionSpeedRate,pCurAnimJoint,0,1.0f,fRatio,matrixscal);
			else
				if (pCurAnimJoint&&pMesh->GetGroupCount())
					pMesh->ProcessAnimJoint(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),pBindPose,pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate,matrixscal);
				else
					pMesh->CopyPosition(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix());

			pMesh->CopyColor(pBuffer);
			pMesh->CopyUV(pBuffer); 
			if (!pBuffer->GetGpuCaculate())
			{
				matrixscal = matrixscal * *pAnimInfo->GetScalMatrix();
				if (pRenderMesh->bAroundYBillboard)
				{
					CalculateBillboard(pMesh,pBuffer,pAnimInfo->GetBillboardY(),&matrixscal);
				}
				else
					CalculateBillboard(pMesh,pBuffer,pAnimInfo->GetBillboard(),&matrixscal);

			}
		}
		else
		{
			const D3DXMATRIX *pMatWord = pAnimInfo->GetWorldMatrix();
			const D3DXMATRIX *pMatView = pAnimInfo->GetViewMatrix();
			const D3DXMATRIX *pMatProj=  pAnimInfo->GetProjectedMatrix();
			
			const D3DXVECTOR3 *pVec3Joint = pAnimInfo->GetSelfJointArrayPos();
			if (pAnimInfo->IsSelfAnimJoint() && pCurAnimJoint && pMesh->GetGroupCount() && pVec3Joint)
			{
				pMesh->ProcessAnimJointSelf(pBuffer,pMatWord,pMatView,pMatProj,pBindPose,
					pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate,pVec3Joint);
				
			} 
			else
			{
				if (pActionInfo->bInterpolation && pOldAnimJoint && pCurAnimJoint &&pMesh->GetGroupCount())
				{
					pMesh->ProcessAnimJointInterpolation(pBuffer,pMatWord,pMatView,pMatProj,pBindPose,pOldAnimJoint,
						dwOldActionTimeElapse,pActionInfo->fOldActionSpeedRate,pCurAnimJoint,0,1.0f,fRatio); 
				}
				else 
				{
					if (pCurAnimJoint && pMesh->GetGroupCount())
						pMesh->ProcessAnimJoint(pBuffer,pMatWord,pMatView,pMatProj,pBindPose,pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate);
					else
						pMesh->CopyPosition(pBuffer,pMatWord,pMatView,pMatProj);
				}
			}


			
			pMesh->CopyColor(pBuffer);
			pMesh->CopyUV(pBuffer); 
		}
	}
}

void CDisplayModel::CDisplayGroup::ProcessLevelAnimVisibility(DWORD dwShaderHandle,DWORD dwTimeElapse,DWORD dwTimeCircle)
{
	if(NULL == m_pLevel)	return;
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();

	if (m_pLevel->GetAnimVisibility() && 
		m_pLevel->GetAnimVisibility()->GetLoadState() == Load_Did)
	{
		GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
		for(int i = 0; i <m_pLevel->GetRMCount(); i++)
		{
			GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
			if(pRenderMesh->GetLoadState() != Load_Did) continue;

			if (pRenderMesh->bAnimVisibility)
			{
				model::Mesh   *pMesh   = pRenderMesh->pMesh;
				model::Buffer *pBuffer = pRenderMesh->pBuffer;
				DWORD dwShaderIndex = pBuffer->GetAttachMesh()->GetShaderIndex();
				render::Interface::Layer3D::ShaderGroup *pShaderGroup = pLayer3D->GetShaderGroup(dwShaderHandle);
				if (!pShaderGroup)
				{
					return;
				}
				render::Interface::Layer3D::ShaderGroup::_tagShader *pShader = pShaderGroup->GetShader(dwShaderIndex);
				if (!pShader)
				{
					return;
				}
				if (pRenderMesh->dwAlphaMode == 1 && pShader->dwAlphaBlendMode == 1)
				{
					pMesh->ProcessAnimVisibility(pBuffer,m_pLevel->GetAnimVisibility(),dwTimeElapse,dwTimeCircle,true);
				}
				else if(pRenderMesh->dwAlphaMode == 1 && pShader->dwAlphaBlendMode == 0)
				{
					pMesh->ProcessAnimVisibility(pBuffer,m_pLevel->GetAnimVisibility(),dwTimeElapse,dwTimeCircle,false);
				}
			}
		}
	}
}

void CDisplayModel::CDisplayGroup::ProcessLevelAnimUV(DWORD dwTimeElapse,DWORD dwTimeCircle)
{
	if(NULL == m_pLevel)	return;

	if (m_pLevel->GetAnimUV() && 
		m_pLevel->GetAnimUV()->GetLoadState() == Load_Did)
	{
		GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
		for(int i = 0; i <m_pLevel->GetRMCount(); i++)
		{
			GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
			if(pRenderMesh->GetLoadState() != Load_Did) continue;
			if (pRenderMesh->bAnimUV)	
			{
				model::Mesh   *pMesh   = pRenderMesh->pMesh;
				model::Buffer *pBuffer = pRenderMesh->pBuffer;
				pBuffer->SetUseUVAnim(false);
				pMesh->ProcessAnimUV(pBuffer,m_pLevel->GetAnimUV(),dwTimeElapse,dwTimeCircle);	
			}
		}
	}
}

void CDisplayModel::CDisplayGroup::ProcessLevelAnimTexture(DWORD dwTimeElapse,DWORD dwTimeCircle)
{	
	if(NULL == m_pLevel)	return;

	if (m_pLevel->GetAnimTexture() && 
		m_pLevel->GetAnimTexture()->GetLoadState() == Load_Did)
	{
		GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
		for(int i = 0; i <m_pLevel->GetRMCount(); i++)
		{
			GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
			if(pRenderMesh->GetLoadState() != Load_Did) continue;
			if (pRenderMesh->bAnimTexture)
			{
				model::Mesh   *pMesh   = pRenderMesh->pMesh;
				model::Buffer *pBuffer = pRenderMesh->pBuffer;
				pMesh->ProcessAnimTexture(pBuffer,m_pLevel->GetAnimTexture(),dwTimeElapse,dwTimeCircle);	
			}
		}
	}
}

void CDisplayModel::CDisplayGroup::ProcessLevelAnimColor(DWORD dwTimeElapse,DWORD dwTimeCircle)
{
	if(NULL == m_pLevel)	return;

	if (m_pLevel->GetAnimColor() &&
		m_pLevel->GetAnimColor()->GetLoadState() == Load_Did)
	{
		GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
		for(int i = 0; i <m_pLevel->GetRMCount(); i++)
		{
			GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
			if(pRenderMesh->GetLoadState() != Load_Did) continue;
			if (pRenderMesh->bAnimColor)	
			{
				model::Mesh   *pMesh   = pRenderMesh->pMesh;
				model::Buffer *pBuffer = pRenderMesh->pBuffer;
				pMesh->ProcessAnimColor(pBuffer,m_pLevel->GetAnimColor(),dwTimeElapse,dwTimeCircle);	
			}
		}
	}
}

void CDisplayModel::CDisplayGroup::ProcessEnvironmentMaps(const D3DXMATRIX *pViewMatrix,bool bnormalize)
{	
	if(NULL == m_pLevel)	return;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;
		if (pRenderMesh->bEnvironmentMaps)
		{
			model::Mesh   *pMesh   = pRenderMesh->pMesh;
			model::Buffer *pBuffer = pRenderMesh->pBuffer;
			if (pBuffer&&pMesh)
			{
				if (pBuffer->GetGpuCaculate())
				{
					pBuffer->SetUseEnvTexture(true);
					pBuffer->SetMatForUse(pViewMatrix);
				}
				else
				{
					pMesh->ProcessEnvironmentMaps(pBuffer,pViewMatrix,bnormalize);
				}

			}

		}
	}
}

//������Դ
void  CDisplayModel::CDisplayGroup::ProcessPointLight(const D3DXVECTOR3 * pLightPosition,
													  float fIntensity, DWORD dwColor)
{
	return;
	if(NULL == m_pLevel)	return;

	float fR = (float)( (dwColor & 0x00FF0000) >> 16) / 255.0f;
	float fG = (float)( (dwColor & 0x0000FF00) >>  8) / 255.0f;
	float fB = (float)(  dwColor & 0x000000FF)        / 255.0f;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		D3DXVECTOR3   *pPosBuffer  = pBuffer->GetPosBuffer();
		D3DXVECTOR3   *pNmlBuffer  = pBuffer->GetNmlBuffer();
		D3DCOLOR	  *pColBufferDiffuse  = pBuffer->GetColBufferDiffuse();
		if (!pPosBuffer||!pNmlBuffer||!pColBufferDiffuse)
		{
			return;
		}

		DWORD dwColCount = pBuffer->GetPNCCount();

		for(DWORD n = 0; n < dwColCount;n++)
		{
			DWORD &dwDestColor = pColBufferDiffuse[n];

			D3DXVECTOR3 L = pPosBuffer[n] - *pLightPosition;
			float fLightDistance = D3DXVec3Length(&L);		//��Դ��vertex�ľ���
			if (fLightDistance == 0.0f)
				fLightDistance = 0.001f;

			D3DXVec3Normalize(&L,&L);
			float DOT = - D3DXVec3Dot(&L,&pNmlBuffer[n]);// L . N

			if (DOT > 0)//�������������
			{	
				float fValue = fIntensity / fLightDistance * DOT;

				DWORD r = (dwDestColor & 0x00FF0000) >> 16;
				DWORD g = (dwDestColor & 0x0000FF00) >> 8;
				DWORD b = (dwDestColor & 0x000000FF);

				r = r + (DWORD)(( fR * fValue) * 255.0f);  	
				if (r > 255) r = 0x000000FF;
				g = g + (DWORD)(( fG * fValue) * 255.0f);  	
				if (g > 255) g = 0x000000FF;
				b = b + (DWORD)(( fB * fValue) * 255.0f);  	
				if (b > 255) b = 0x000000FF;
				dwDestColor = (dwDestColor & 0xFF000000) | (r << 16) | (g << 8) | b;
			}                                    
		}
	}
}

//�������
void  CDisplayModel::CDisplayGroup::ProcessDirectionalLight(const D3DXVECTOR3 *pCameraPosition,
															const D3DXVECTOR3 *pLightDirection,DWORD dwColor)
{
	return;
	if(NULL == m_pLevel)	return;

	DWORD dwR = ( (dwColor & 0x00FF0000) >> 16) ;
	DWORD dwG = ( (dwColor & 0x0000FF00) >>  8) ;
	DWORD dwB = (  dwColor & 0x000000FF)        ;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		if (!pRenderMesh->bEffectByLight)
		{
			continue;
		}
		//
		model::Buffer *pBuffer     = pRenderMesh->pBuffer;
		D3DXVECTOR3   *pPosBuffer  = pBuffer->GetPosBuffer();
		D3DXVECTOR3   *pNmlBuffer  = pBuffer->GetNmlBuffer();
		D3DCOLOR	  *pColBufferDiffuse  = pBuffer->GetColBufferDiffuse();

		DWORD dwColCount = pBuffer->GetPNCCount();
		//����Diffuse��
		DWORD n = 0;
		for(; n < dwColCount;n++)
		{
			DWORD &dwDestColor = pColBufferDiffuse[n];

			DWORD r = (dwDestColor & 0x00FF0000) >> 16;
			DWORD g = (dwDestColor & 0x0000FF00) >> 8;
			DWORD b = (dwDestColor & 0x000000FF);

			float DOT = - D3DXVec3Dot(pLightDirection,&pNmlBuffer[n]);// L . N

			if (DOT > 0)//�������������
			{	
				DWORD dwValue = (DWORD)(2.0f * DOT * 256.0f);

				r = r + dwR * dwValue / 256;  	
				if (r > 255) r = 0x000000FF;
				g = g + dwG * dwValue / 256;  	
				if (g > 255) g = 0x000000FF;
				b = b + dwB * dwValue / 256;  	
				if (b > 255) b = 0x000000FF;
			}                                    

			dwDestColor = 0xFF000000 | (r << 16) | (g << 8) | b;
		}

		//if (!pRenderMesh->bSpecular)
		//{
		//	continue;
		//}
		//D3DCOLOR  *pColBufferSpecular = pBuffer->GetColBufferSpecular();
		////����specular��=======================================		
		//D3DXVECTOR3 H,T;
		//for(n = 0; n < dwColCount;n++)
		//{
		//	T = *pCameraPosition - pPosBuffer[n];
		//	D3DXVec3Normalize(&T,&T);
		//	H = T - *pLightDirection;
		//	D3DXVec3Normalize(&H,&H);
		//	float fDOT = D3DXVec3Dot(&pNmlBuffer[n],&H);
		//	DWORD c = (DWORD)(pow(fDOT,32.0f) * 256.0f);
		//	if (c > 255)
		//	{
		//		c  = 255;
		//	}

		//	DWORD r = (dwR * c) >> 8;  	
		//	DWORD g = (dwG * c) >> 8;  	
		//	DWORD b = (dwB * c) >> 8;  	
		//	pColBufferSpecular[n] = 0xFF000000 | (r << 16) | (g << 8) | b;
		//}
	}
}

void CDisplayModel::CDisplayGroup::CopyColorToBuffer(std::vector<D3DCOLOR*>& vecbuffer)
{
	if(NULL == m_pLevel)	return;
	if (vecbuffer.empty())
	{
		return;
	}
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	int n = 0;
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		if (!pRenderMesh->bEffectByLight)
		{
			continue;
		}
		//
		model::Buffer *pBuffer     = pRenderMesh->pBuffer;
		D3DXVECTOR3   *pPosBuffer  = pBuffer->GetPosBuffer();
		D3DXVECTOR3   *pNmlBuffer  = pBuffer->GetNmlBuffer();
		D3DCOLOR	  *pColBufferDiffuse = pBuffer->GetColBufferDiffuse();
		DWORD dwColCount = pBuffer->GetPNCCount();
		D3DCOLOR* pcolor= vecbuffer[n];
		memcpy(pColBufferDiffuse,pcolor,sizeof(D3DCOLOR) * dwColCount);
		n++;
	}
}
void CDisplayModel::CDisplayGroup::CopyBufferToColor(std::vector<D3DCOLOR*>& vecbuffer)
{
	if(NULL == m_pLevel)	return;
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		if (!pRenderMesh->bEffectByLight)
		{
			continue;
		}
		//
		model::Buffer *pBuffer     = pRenderMesh->pBuffer;
		D3DXVECTOR3   *pPosBuffer  = pBuffer->GetPosBuffer();
		D3DXVECTOR3   *pNmlBuffer  = pBuffer->GetNmlBuffer();
		D3DCOLOR	  *pColBufferDiffuse = pBuffer->GetColBufferDiffuse();
		DWORD dwColCount = pBuffer->GetPNCCount();
		D3DCOLOR* pcolor = new D3DCOLOR[dwColCount];
		vecbuffer.push_back(pcolor);
		memcpy(pcolor,pColBufferDiffuse,sizeof(D3DCOLOR) * dwColCount);
	}
}
void CDisplayModel::CDisplayGroup::CalculateBillboard(const model::Mesh *pMesh,model::Buffer * pBuffer,
													  const D3DXMATRIX *pBillboarMatrix,const D3DXMATRIX *pScalMatrix)
{
	D3DXMATRIX matBillBoard;
	memcpy(&matBillBoard,pBillboarMatrix,sizeof(D3DXMATRIX));
	float x(0),y(0),z(0);
	float cx(0),cy(0),cz(0);

	D3DXMATRIX matTransform;
	D3DXVECTOR3 *pPosBuffer = pBuffer->GetPosBuffer();
	D3DXVECTOR3 *pMeshPosBuffer = pMesh->GetPosBuffer();

	DWORD n = 0;
	for(; n < pBuffer->GetPNCCount(); n++)
	{
		x += pPosBuffer[n].x;
		y += pPosBuffer[n].y;
		z += pPosBuffer[n].z;

		cx += pMeshPosBuffer[n].x;
		cy += pMeshPosBuffer[n].y;
		cz += pMeshPosBuffer[n].z;
	}

	float reciprocal = 1.0f / (float)n;

	x *= reciprocal;
	y *= reciprocal;
	z *= reciprocal;

	cx *= - reciprocal;
	cy *= - reciprocal;
	cz *= - reciprocal;

	matBillBoard._41 = x;
	matBillBoard._42 = y;
	matBillBoard._43 = z;

	D3DXMatrixTranslation(&matTransform,cx,cy,cz);
	if (pScalMatrix)
	{
		D3DXMatrixMultiply(&matTransform,&matTransform,pScalMatrix);
	}

	D3DXMatrixMultiply(&matTransform,&matTransform,&matBillBoard);

	for(n = 0; n < pBuffer->GetPNCCount(); n++)
	{
		D3DXVECTOR3 &vb = pPosBuffer[n];	
		D3DXVECTOR3 &vm = pMeshPosBuffer[n];
		D3DXVec3TransformCoord(&vb,&vm,&matTransform);
	}
}
void  CDisplayModel::CDisplayGroup::GetEffectTypeAndMacoDecla(model::Buffer *pBuffer,render::Interface::Layer3D::RendMesh* prendmesh,render::Interface::Layer3D::RendMesh::TAGMESH* ptagmesh,DWORD& dwEffectType,DWORD& dwMacoDecla)
{
	const model::Mesh* pMesh = pBuffer->GetAttachMesh();
	if (prendmesh->GetToReCreate())
	{
		prendmesh->ReCreate(pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),false);
	}
	if (prendmesh->GetTexHandle())
	{
		dwEffectType |= EFFECTTYPE_USEUV;
		dwMacoDecla |= EFFECTTYPE_USEUV;
	}
	if(!prendmesh->GetStatic()&&!pBuffer->GetNoAnim())
	{
		dwEffectType |= EFFECTTYPE_USEMATINDEX;
		dwMacoDecla |= EFFECTTYPE_USEMATINDEX;
	}
	if (pBuffer->GetUseEnvTexture())
	{
		dwEffectType |= EFFECTTYPE_USENORMAL|EFFECTTYPE_ENVIROMENT;
		dwMacoDecla |= EFFECTTYPE_USENORMAL;
		ptagmesh->SetMatrixForUse(pBuffer->GetMatForUse());
	}
	else if (pBuffer->GetUseUVAnim())
	{
		dwEffectType |= EFFECTTYPE_UVANIMI;
		ptagmesh->SetMatrixForUse(pBuffer->GetMatForUse());
	}

	if (pBuffer->GetLightEnable())
	{
		dwEffectType |= EFFECTTYPE_USENORMAL|EFFECTTYPE_USELIGHT;
		dwMacoDecla  |= EFFECTTYPE_USENORMAL;
		ptagmesh->SetMultiColor(pBuffer->GetMultiColor());
	}
	if ((dwEffectType&EFFECTTYPE_USENORMAL) != 0)
	{

		if(pBuffer->GetNormalTransScal())
		{
			dwEffectType |= EFFECTTYPE_ANIMUSESCAL;
			ptagmesh->SetMatrixForNormal(pBuffer->GetJointMatrixBufferForNormals(),prendmesh->GetJointCount());
		}
	}
	if (render::Interface::GetInstance()->GetSetting()->bgpuprocess)
	{
		if (render::Interface::GetInstance()->GetSetting()->bHVProcess&&!pBuffer->GetIsSWProcess())
		{
			if ((dwEffectType&EFFECTTYPE_ANIMUSESCAL) != 0)
			{
				if (prendmesh->GetJointCount() * 2 <= render::Interface::GetInstance()->GetSetting()->ihvpmatrixcount)
				{
					dwEffectType |= EFFECTTYPE_HWMAXMAT;
				}
				else
				{
					dwEffectType |= EFFECTTYPE_SWMAXMAT;
					if (!prendmesh->GetIsSoftWareProcessing())
					{
						if (!prendmesh->GetIsRsSvpCreated())
						{
							prendmesh->ReCreate(pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),true);
						}
						prendmesh->SetSoftWareVertexprocess(true);

					}
				}
			}
			else
			{
				dwEffectType |= EFFECTTYPE_HWMAXMAT;
				prendmesh->SetSoftWareVertexprocess(false);
			}

		}
		else
		{
			dwEffectType |= EFFECTTYPE_SWMAXMAT;
		}
	}
}
//��Ⱦģ��
void  CDisplayModel::CDisplayGroup::Render(DWORD dwShaderGroupHandle)
{
	if(NULL == m_pLevel)	return;
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	render::Interface::Layer3D::RendMeshManager* prendmeshmanager = render::Interface::GetInstance()->GetLayer3D()->GetRendMeshManager();
	render::Interface::Layer3D::EffectManager* peffectmanager = render::Interface::GetInstance()->GetLayer3D()->GetEffectManager();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;
		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		model::Mesh *pMesh   = pRenderMesh->pMesh;
		if (pBuffer&&pMesh)
		{
			bool busenormal = false;
			if (pRenderMesh->dwRendMeshIndex == 0xffffffff)
			{
				if (pRenderMesh->bEnvironmentMaps||pRenderMesh->bEffectByLight)
				{
					busenormal = true;
				}
				pRenderMesh->dwRendMeshIndex = prendmeshmanager->CreateRendMesh(dwShaderGroupHandle,pMesh->GetShaderIndex(),pRenderMesh->dwAlphaMode,pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pMesh->GetPNCCount(),pMesh->GetIndexCount(),pMesh->GetGroupCount(),pBuffer->GetRendStatic(),busenormal,false);
			}
			if (pRenderMesh->dwRendMeshIndex != 0xffffffff)
			{
				render::Interface::Layer3D::RendMesh* prendmesh = prendmeshmanager->GetRendMesh(pRenderMesh->dwRendMeshIndex);
				render::Interface::Layer3D::RendMesh::TAGMESH* ptagmesh = prendmesh->GetTagMesh();
				DWORD dwEffecttype = EFFECTTYPE_USEPOS;
				DWORD dwMacoDecl = EFFECTTYPE_USEPOS;
				GetEffectTypeAndMacoDecla(pBuffer,prendmesh,ptagmesh,dwEffecttype,dwMacoDecl);

				ptagmesh->SetMatrixForVP(pBuffer->GetMatViewProjective());

				ptagmesh->SetMatrix(pBuffer->GetJointMatrixBufferForVertex(),prendmesh->GetJointCount());
				ptagmesh->SetFlash(false);
				ptagmesh->SetBaseColor(pBuffer->GetBaseColor());
				ptagmesh->SetEffectID(peffectmanager->CreateEffectFile(dwEffecttype));
				ptagmesh->GetVecPassID().push_back(0);
				ptagmesh->SetDeclID(dwMacoDecl);
				prendmesh->SendToRendList(pRenderMesh->dwAlphaMode);
			}
		}

	}
}
/*
* ����: ��Ⱦ͸����
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.03.10 - lpf
*		�����˻���͸��ģ�͵ĺ�������,ԭ����model::Buffer::Render();
*/
void CDisplayModel::CDisplayGroup::RenderTransparent(DWORD dwShaderGroupHandle)
{
	if(NULL == m_pLevel)	return;
	render::Interface *pInterface = render::Interface::GetInstance();
	if (pInterface->GetDeviceLost())
	{
		return;
	}
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	render::Interface::Layer3D::RendMeshManager* prendmeshmanager = render::Interface::GetInstance()->GetLayer3D()->GetRendMeshManager();
	render::Interface::Layer3D::EffectManager* peffectmanager = render::Interface::GetInstance()->GetLayer3D()->GetEffectManager();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		DWORD dwShaderIndex = pBuffer->GetAttachMesh()->GetShaderIndex();
		model::Mesh *pMesh   = pRenderMesh->pMesh;
		if (pBuffer&&pMesh)
		{
			bool busenormal = false;
			if (pRenderMesh->dwRendMeshIndex == 0xffffffff)
			{
				if (pRenderMesh->bEnvironmentMaps||pRenderMesh->bEffectByLight)
				{
					busenormal = true;
				}
				pRenderMesh->dwRendMeshIndex = prendmeshmanager->CreateRendMesh(dwShaderGroupHandle,pMesh->GetShaderIndex(),pRenderMesh->dwAlphaMode,pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pMesh->GetPNCCount(),pMesh->GetIndexCount(),pMesh->GetGroupCount(),pBuffer->GetRendStatic(),busenormal,false);
			}
			if (pRenderMesh->dwRendMeshIndex != 0xffffffff)
			{
				render::Interface::Layer3D::RendMesh* prendmesh = prendmeshmanager->GetRendMesh(pRenderMesh->dwRendMeshIndex);
				
				render::Interface::Layer3D::RendMesh::TAGMESH* ptagmesh = prendmesh->GetTagMesh(!(pRenderMesh->dwAlphaMode == 1 && prendmesh->GetAlphaOrBlendMode() == 0));
				
				DWORD dwEffecttype = EFFECTTYPE_USEPOS;
				DWORD dwMacoDecl = EFFECTTYPE_USEPOS;
				GetEffectTypeAndMacoDecla(pBuffer,prendmesh,ptagmesh,dwEffecttype,dwMacoDecl);

				ptagmesh->SetMatrix(pBuffer->GetJointMatrixBufferForVertex(),prendmesh->GetJointCount());
				ptagmesh->SetMatrixForVP(pBuffer->GetMatViewProjective());
				ptagmesh->SetFlash(false);
				ptagmesh->SetEffectID(peffectmanager->CreateEffectFile(dwEffecttype));
				ptagmesh->GetVecPassID().push_back(0);
				ptagmesh->SetDeclID(dwMacoDecl);
				if (pRenderMesh->dwAlphaMode == 1 && prendmesh->GetAlphaOrBlendMode() == 0)
				{
					pBuffer->MultiAlphaToColor();
					ptagmesh->SetBaseColor(pBuffer->GetBaseColor());
					prendmesh->SendToRendList(pRenderMesh->dwAlphaMode);
				}
				else
				{
					ptagmesh->SetBaseColor(pBuffer->GetBaseColor());
					prendmesh->SendToRendList(2);
				}
			}
		}
	}
}
/*
* ����: ģ������,�õ�DynamicBuffer
* ժҪ: -
* ����: dwShaderGroupHandle - ��Ⱦ����
*
* ����ֵ: -
* ����: ��ǫ
* ��������: 2008.09.2
*/
void CDisplayModel::CDisplayGroup::RenderModelFlash(DWORD dwShaderGroupHandle)
{
	if (m_pLevel == NULL)
		return;
	render::Interface *pInterface = render::Interface::GetInstance();
	if (pInterface->GetDeviceLost())
	{
		return;
	}
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
	render::Interface::Layer3D::RendMeshManager* prendmeshmanager = render::Interface::GetInstance()->GetLayer3D()->GetRendMeshManager();
	render::Interface::Layer3D::EffectManager* peffectmanager = render::Interface::GetInstance()->GetLayer3D()->GetEffectManager();
	GameModel::Group::Part::LevelMeshes & vRenderMeshes = m_pLevel->GetRMArray();
	for (int i = 0; i < m_pLevel->GetRMCount(); ++i)
	{
		GameModel::Group::Part::tagRenderMesh * pRenderMesh = vRenderMeshes[i];
		if (pRenderMesh->GetLoadState() != Load_Did)
			continue;

		model::Buffer * pBuffer		  = pRenderMesh->pBuffer;
		DWORD dwShaderIndex = pBuffer->GetAttachMesh()->GetShaderIndex();
		model::Mesh *pMesh   = pRenderMesh->pMesh;
		if (pBuffer&&pMesh)
		{
			bool busenormal = false;
			if (pRenderMesh->dwRendMeshIndex == 0xffffffff)
			{
				if (pRenderMesh->bEnvironmentMaps||pRenderMesh->bEffectByLight)
				{
					busenormal = true;
				}
				pRenderMesh->dwRendMeshIndex = prendmeshmanager->CreateRendMesh(dwShaderGroupHandle,pMesh->GetShaderIndex(),pRenderMesh->dwAlphaMode,pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pMesh->GetPNCCount(),pMesh->GetIndexCount(),pMesh->GetGroupCount(),pBuffer->GetRendStatic(),busenormal,false);
			}
			if (pRenderMesh->dwRendMeshIndex != 0xffffffff)
			{
				render::Interface::Layer3D::RendMesh* prendmesh = prendmeshmanager->GetRendMesh(pRenderMesh->dwRendMeshIndex);
				render::Interface::Layer3D::RendMesh::TAGMESH* ptagmesh = prendmesh->GetTagMesh();
				DWORD dwEffecttype = EFFECTTYPE_USEPOS;
				DWORD dwMacoDecl = EFFECTTYPE_USEPOS;
				GetEffectTypeAndMacoDecla(pBuffer,prendmesh,ptagmesh,dwEffecttype,dwMacoDecl);
				ptagmesh->SetMatrix(pBuffer->GetJointMatrixBufferForVertex(),prendmesh->GetJointCount());
				ptagmesh->SetMatrixForVP(pBuffer->GetMatViewProjective());
				ptagmesh->SetBaseColor(pBuffer->GetBaseColor());
				ptagmesh->SetEffectID(peffectmanager->CreateEffectFile(dwEffecttype));
				ptagmesh->GetVecPassID().push_back(0);
				ptagmesh->SetDeclID(dwMacoDecl);
				//Blend + Addtive��ģ�Ͳ������Ͷ���ɫ��ӵĲ���
				if (pRenderMesh->dwAlphaMode == 1 && prendmesh->GetAlphaOrBlendMode() == 0)
				{
					ptagmesh->SetFlash(false);
				}
				else
				{
					ptagmesh->SetFlash(true);
				}
				prendmesh->SendToRendList(pRenderMesh->dwAlphaMode);
			}
		}
	}
}




/*
* ����: ʹ�ö�̬��ɫ����������Ⱦ
* ժҪ: -
* ����: dwShaderGroupHandle - ��Ⱦ����
*		 pShaderState		 - ��̬��ɫ����
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.23
*/
void CDisplayModel::CDisplayGroup::RenderByShaderState(DWORD dwShaderGroupHandle, const render::Interface::Layer3D::_tagShaderState * pShaderState)
{
	if (m_pLevel == NULL)
		return;

	GameModel::Group::Part::LevelMeshes & vRenderMeshes = m_pLevel->GetRMArray();
	for (int i = 0; i < m_pLevel->GetRMCount(); ++i)
	{
		GameModel::Group::Part::tagRenderMesh * pRenderMesh = vRenderMeshes[i];
		if (pRenderMesh->GetLoadState() != Load_Did)
			continue;

		model::Buffer * pBuffer		  = pRenderMesh->pBuffer;

		pBuffer->RenderByShaderState(dwShaderGroupHandle, pRenderMesh->dwAlphaMode, pShaderState);
	}
}





void  CDisplayModel::CDisplayGroup::DrawAlphaTestOnly(DWORD dwShaderGroupHandle)
{
	if(NULL == m_pLevel)	return;	
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	render::Interface::Layer3D::RendMeshManager* prendmeshmanager = render::Interface::GetInstance()->GetLayer3D()->GetRendMeshManager();
	render::Interface::Layer3D::EffectManager* peffectmanager = render::Interface::GetInstance()->GetLayer3D()->GetEffectManager();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		if (pRenderMesh->dwAlphaMode == 0)
		{
			model::Buffer* pBuffer = pRenderMesh->pBuffer;
			model::Mesh*   pMesh   = pRenderMesh->pMesh;
			bool busenormal = false;
			if (pRenderMesh->dwRendMeshIndex == 0xffffffff)
			{
				if (pRenderMesh->bEnvironmentMaps||pRenderMesh->bEffectByLight)
				{
					busenormal = true;
				}
				pRenderMesh->dwRendMeshIndex = prendmeshmanager->CreateRendMesh(dwShaderGroupHandle,pMesh->GetShaderIndex(),pRenderMesh->dwAlphaMode,pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pMesh->GetPNCCount(),pMesh->GetIndexCount(),pMesh->GetGroupCount(),pBuffer->GetRendStatic(),busenormal);
			}
			if (pRenderMesh->dwRendMeshIndex != 0xffffffff)
			{
				render::Interface::Layer3D::RendMesh* prendmesh = prendmeshmanager->GetRendMesh(pRenderMesh->dwRendMeshIndex);
				D3DXCOLOR bcolor(pBuffer->GetBaseColor());
				D3DXVECTOR4 v4basecolor(bcolor.r,bcolor.g,bcolor.b,bcolor.a);
				D3DXCOLOR mcolor(pBuffer->GetMultiColor());
				D3DXVECTOR4 v4multicolor(mcolor.r,mcolor.g,mcolor.b,mcolor.a);
				prendmesh->Draw(pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pBuffer->GetJointMatrixBufferForVertex(),pBuffer->GetJointMatrixBufferForNormals(),pBuffer->GetMatForUse(),pBuffer->GetMatView(),pBuffer->GetMatProjective(),pBuffer->GetMatFotTP(),v4basecolor,v4multicolor,pBuffer->GetNoAnim(),pBuffer->GetUseEnvTexture(),pBuffer->GetUseUVAnim(),pBuffer->GetLightEnable(),pBuffer->GetNormalTransScal(),true,false,0);
			}
		}
	}
}



/*
* ����: alpha����ʹ����ɫ����ֱ�ӻ���ģ��
* ժҪ: -
* ����: dwShaderGroupHandle - ��Ⱦ����
*		 dwColorOp			 - ��ɫ����
*		 dwColorArg1		 - ��ɫ����1
*		 dwColorArg2		 - ��ɫ����2
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.16
*/
void CDisplayModel::CDisplayGroup::DrawAlphaTestByColorOp(DWORD dwShaderGroupHandle, DWORD dwColorOp, DWORD dwColorArg1, DWORD dwColorArg2)
{
	if(m_pLevel == NULL)
		return;
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}

	GameModel::Group::Part::LevelMeshes & vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i < m_pLevel->GetRMCount(); ++i)
	{
		GameModel::Group::Part::tagRenderMesh * pRenderMesh = vRenderMeshes[i];
		if (pRenderMesh->GetLoadState() != Load_Did)
			continue;

		if (pRenderMesh->dwAlphaMode == 0)
		{
			model::Buffer * pBuffer		  = pRenderMesh->pBuffer;

			pBuffer->DrawByColorOp(dwShaderGroupHandle, 0, dwColorOp, dwColorArg1, dwColorArg2);
		}
	}
}



void  CDisplayModel::CDisplayGroup::DrawAlphaBlendOnly(DWORD dwShaderGroupHandle)
{
	if(NULL == m_pLevel)	return;	
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	render::Interface::Layer3D::RendMeshManager* prendmeshmanager = render::Interface::GetInstance()->GetLayer3D()->GetRendMeshManager();
	render::Interface::Layer3D::EffectManager* peffectmanager = render::Interface::GetInstance()->GetLayer3D()->GetEffectManager();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		if (pRenderMesh->dwAlphaMode == 1)
		{
			model::Buffer* pBuffer = pRenderMesh->pBuffer;
			model::Mesh*   pMesh   = pRenderMesh->pMesh;
			bool busenormal = false;
			if (pRenderMesh->dwRendMeshIndex == 0xffffffff)
			{
				if (pRenderMesh->bEnvironmentMaps||pRenderMesh->bEffectByLight)
				{
					busenormal = true;
				}
				pRenderMesh->dwRendMeshIndex = prendmeshmanager->CreateRendMesh(dwShaderGroupHandle,pMesh->GetShaderIndex(),pRenderMesh->dwAlphaMode,pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pMesh->GetPNCCount(),pMesh->GetIndexCount(),pMesh->GetGroupCount(),pBuffer->GetRendStatic(),busenormal);
			}
			if (pRenderMesh->dwRendMeshIndex != 0xffffffff)
			{
				render::Interface::Layer3D::RendMesh* prendmesh = prendmeshmanager->GetRendMesh(pRenderMesh->dwRendMeshIndex);
				D3DXCOLOR bcolor(pBuffer->GetBaseColor());
				D3DXVECTOR4 v4basecolor(bcolor.r,bcolor.g,bcolor.b,bcolor.a);
				D3DXCOLOR mcolor(pBuffer->GetMultiColor());
				D3DXVECTOR4 v4multicolor(mcolor.r,mcolor.g,mcolor.b,mcolor.a);
				prendmesh->Draw(pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pBuffer->GetJointMatrixBufferForVertex(),pBuffer->GetJointMatrixBufferForNormals(),pBuffer->GetMatForUse(),pBuffer->GetMatView(),pBuffer->GetMatProjective(),pBuffer->GetMatFotTP(),v4basecolor,v4multicolor,pBuffer->GetNoAnim(),pBuffer->GetUseEnvTexture(),pBuffer->GetUseUVAnim(),pBuffer->GetLightEnable(),pBuffer->GetNormalTransScal(),true,false,1);
			}
		}
	}
}



/*
* ����: alpha���ʹ����ɫ����ֱ�ӻ���ģ��
* ժҪ: -
* ����: dwShaderGroupHandle - ��Ⱦ����
*		 dwColorOp			 - ��ɫ����
*		 dwColorArg1		 - ��ɫ����1
*		 dwColorArg2		 - ��ɫ����2
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.16
*/
void CDisplayModel::CDisplayGroup::DrawAlphaBlendByColorOp(DWORD dwShaderGroupHandle, DWORD dwColorOp, DWORD dwColorArg1, DWORD dwColorArg2)
{
	if (m_pLevel == NULL)
		return;
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i < m_pLevel->GetRMCount(); ++i)
	{
		GameModel::Group::Part::tagRenderMesh * pRenderMesh = vRenderMeshes[i];
		if (pRenderMesh->GetLoadState() != Load_Did)
			continue;

		if (pRenderMesh->dwAlphaMode == 1)
		{
			model::Buffer * pBuffer		  = pRenderMesh->pBuffer;

			pBuffer->DrawByColorOp(dwShaderGroupHandle, 1, dwColorOp, dwColorArg1, dwColorArg2);
		}
	}
}



void  CDisplayModel::CDisplayGroup::DrawTransparent(DWORD dwShaderGroupHandle)
{
	if(NULL == m_pLevel)	return;	
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;
		if (pRenderMesh->dwAlphaMode)
		{
			continue;
		}
		model::Buffer *pBuffer = pRenderMesh->pBuffer;

		pBuffer->DrawTransparent(dwShaderGroupHandle);
	}
}
void  CDisplayModel::CDisplayGroup::DrawWithoutState(DWORD dwShaderGroupHandle)
{
	if(NULL == m_pLevel)	return;	
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	render::Interface::Layer3D::RendMeshManager* prendmeshmanager = render::Interface::GetInstance()->GetLayer3D()->GetRendMeshManager();
	render::Interface::Layer3D::EffectManager* peffectmanager = render::Interface::GetInstance()->GetLayer3D()->GetEffectManager();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;
		model::Buffer *pBuffer = pRenderMesh->pBuffer;
		model::Mesh*   pMesh   = pRenderMesh->pMesh;
		bool busenormal = false;
		if (pRenderMesh->dwRendMeshIndex == 0xffffffff)
		{
			if (pRenderMesh->bEnvironmentMaps||pRenderMesh->bEffectByLight)
			{
				busenormal = true;
			}
			pRenderMesh->dwRendMeshIndex = prendmeshmanager->CreateRendMesh(dwShaderGroupHandle,pMesh->GetShaderIndex(),pRenderMesh->dwAlphaMode,pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pMesh->GetPNCCount(),pMesh->GetIndexCount(),pMesh->GetGroupCount(),pBuffer->GetRendStatic(),busenormal);
		}
		if (pRenderMesh->dwRendMeshIndex != 0xffffffff)
		{
			render::Interface::Layer3D::RendMesh* prendmesh = prendmeshmanager->GetRendMesh(pRenderMesh->dwRendMeshIndex);
			D3DXCOLOR bcolor(pBuffer->GetBaseColor());
			D3DXVECTOR4 v4basecolor(bcolor.r,bcolor.g,bcolor.b,bcolor.a);
			D3DXCOLOR mcolor(pBuffer->GetMultiColor());
			D3DXVECTOR4 v4multicolor(mcolor.r,mcolor.g,mcolor.b,mcolor.a);
			prendmesh->Draw(pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pBuffer->GetJointMatrixBufferForVertex(),pBuffer->GetJointMatrixBufferForNormals(),pBuffer->GetMatForUse(),pBuffer->GetMatView(),pBuffer->GetMatProjective(),pBuffer->GetMatFotTP(),v4basecolor,v4multicolor,pBuffer->GetNoAnim(),false,false,false,false,true,false,10);
		}


	}
}
void  CDisplayModel::CDisplayGroup::DrawWithoutStateAndTexture(DWORD dwShaderGroupHandle)
{
	if(NULL == m_pLevel)	return;
	if (render::Interface::GetInstance()->GetDeviceLost())
	{
		return;
	}
	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	render::Interface::Layer3D::RendMeshManager* prendmeshmanager = render::Interface::GetInstance()->GetLayer3D()->GetRendMeshManager();
	render::Interface::Layer3D::EffectManager* peffectmanager = render::Interface::GetInstance()->GetLayer3D()->GetEffectManager();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;

		if (pRenderMesh->bProjected)
		{
			model::Buffer *pBuffer = pRenderMesh->pBuffer;
			model::Mesh*   pMesh   = pRenderMesh->pMesh;

			bool busenormal = false;
			if (pRenderMesh->dwRendMeshIndex == 0xffffffff)
			{
				if (pRenderMesh->bEnvironmentMaps||pRenderMesh->bEffectByLight)
				{
					busenormal = true;
				}
				pRenderMesh->dwRendMeshIndex = prendmeshmanager->CreateRendMesh(dwShaderGroupHandle,pMesh->GetShaderIndex(),pRenderMesh->dwAlphaMode,pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),pMesh->GetUV0Buffer(),pMesh->GetIndexBuffer(),pMesh->GetPNCCount(),pMesh->GetIndexCount(),pMesh->GetGroupCount(),pBuffer->GetRendStatic(),busenormal);
			}
			if (pRenderMesh->dwRendMeshIndex != 0xffffffff)
			{
				render::Interface::Layer3D::RendMesh* prendmesh = prendmeshmanager->GetRendMesh(pRenderMesh->dwRendMeshIndex);
				D3DXCOLOR bcolor(pBuffer->GetBaseColor());
				D3DXVECTOR4 v4basecolor(bcolor.r,bcolor.g,bcolor.b,bcolor.a);
				D3DXCOLOR mcolor(pBuffer->GetMultiColor());
				D3DXVECTOR4 v4multicolor(mcolor.r,mcolor.g,mcolor.b,mcolor.a);

				prendmesh->Draw(pMesh->GetPosBuffer(),pMesh->GetJointIndexBuffer(),pMesh->GetNmlBuffer(),NULL,pMesh->GetIndexBuffer(),pBuffer->GetJointMatrixBufferForVertex(),pBuffer->GetJointMatrixBufferForNormals(),pBuffer->GetMatForUse(),pBuffer->GetMatView(),pBuffer->GetMatProjective(),pBuffer->GetMatFotTP(),v4basecolor,v4multicolor,pBuffer->GetNoAnim(),false,false,false,false,false,true,10);

			}

		}

	}
}

void CDisplayModel::CDisplayGroup::_DEBUG_RenderNormal()
{
	if(NULL == m_pLevel)	return;

	GameModel::Group::Part::LevelMeshes& vRenderMeshes = m_pLevel->GetRMArray();
	for(int i = 0; i <m_pLevel->GetRMCount(); i++)
	{
		GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
		if(pRenderMesh->GetLoadState() != Load_Did) continue;
		pRenderMesh->pBuffer->_DEBUG_RenderNormal();
	}
}

//�ͷ���Դ
void CDisplayModel::ReleaseAll()
{
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		delete (*it).second;
	}
	m_DisplayGroups.clear();
	particle::Manager *pPM = particle::Manager::GetInstance();
	itParticleEmitter itp = m_ParticleEmitters.begin();
	for (;itp!=m_ParticleEmitters.end();itp++)
	{
		itp->second->GetParticleEmitterRef()->SetReadyToDie();
		//SAFEDELETE(itp->second->GetParticleEmitterRef());
		SAFEDELETE(itp->second);
	}
}
//�ж�Group->Part�Ƿ���ʾ
BOOL CDisplayModel::IsGroupPartShow(DWORD dwGroup,DWORD dwPart)
{
	CDisplayGroup *pDisplayGroup = NULL;
	itDisplayGroup it = m_DisplayGroups.find(dwGroup);
	if(it != m_DisplayGroups.end())
	{
		pDisplayGroup = (*it).second;
		if(pDisplayGroup->GetPartName() == dwPart ||
			pDisplayGroup->GetPartName()==0)
			return TRUE;
	}
	return FALSE;
}

DWORD CDisplayModel::GetLvlOfGroupPart(DWORD dwGroup,DWORD dwPart)
{
	CDisplayGroup *pDisplayGroup = NULL;
	itDisplayGroup it = m_DisplayGroups.find(dwGroup);
	if(it != m_DisplayGroups.end())
	{
		pDisplayGroup = (*it).second;
		if(pDisplayGroup->GetPartName() == dwPart)
			return pDisplayGroup->GetPartLevel();
	}
	return 0;
}


BOOL CDisplayModel::IsGroupPartLevelShow(DWORD dwGroup,DWORD dwPart,DWORD dwLevel)
{
	CDisplayGroup *pDisplayGroup = NULL;
	itDisplayGroup it = m_DisplayGroups.find(dwGroup);
	if(it != m_DisplayGroups.end())
	{
		pDisplayGroup = (*it).second;
		if(pDisplayGroup->GetPartName() == dwPart &&
			pDisplayGroup->GetPartLevel() == dwLevel )
			return TRUE;
	}
	return FALSE;
}



/*
* ����: ����group��Ϣ
* ժҪ: -
* ����: dwGroup   - ��ʾ����
*		 dwPart	   - ��ʾ������
*		 dwLevel   - ��Ҫ��ʾ�Ĳ����ȼ�����(��0��ʼ,�������0xffffffff��Ϊ��������ĵȼ���Ϣ)
*		 bAsyn	   - �Ƿ��첽����
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*/
void CDisplayModel::ShowGroup(DWORD dwGroup,DWORD dwPart,DWORD dwLevel,bool bAsyn)
{
	GameModel::Group::Part::Level *pLvlInfo = NULL;
	CDisplayGroup *pDisplayGroup = NULL;
	itDisplayGroup it = m_DisplayGroups.find(dwGroup);
	if(it != m_DisplayGroups.end())
	{
		//�����򷵻�
		pDisplayGroup = (*it).second;
		if( (dwPart == 0 || pDisplayGroup->GetPartName() == dwPart) &&
			pDisplayGroup->GetPartLevel() == dwLevel )
			return;
	}
	GameModel::Group::Part::Level *pLevel = NULL;
	if(pDisplayGroup)
	{
		pLevel= pDisplayGroup->GetLevelInfo();
		m_pGameModelManager->ReleaseLevelInfo(m_dwGameModelID,dwGroup,pDisplayGroup->GetPartName(),
			pDisplayGroup->GetPartLevel(),pLevel);
	}
	else
	{
		pDisplayGroup = new CDisplayGroup();
		m_DisplayGroups[dwGroup]=pDisplayGroup;
	}	
	pLevel = m_pGameModelManager->AcquireLevelInfo(m_dwGameModelID,dwGroup,dwPart,dwLevel,bAsyn);
	pDisplayGroup->SetPartNameLevel(dwPart,dwLevel);
	pDisplayGroup->SetLevelInfo(pLevel);
	return;
}

void CDisplayModel::ShowAllGroupbyLevel(DWORD dwlevel,bool bAsyn)
{

	m_bAllGroup = TRUE;
	m_pGameModelManager->ShowDisplayModelAllGroupbyLevel(this,dwlevel,bAsyn);
}

//��ʾȫ��Group,��ѡ��ȱʡ��Part�͵ȼ�
void CDisplayModel::ShowAllGroup(bool bAsyn)
{
	m_bAllGroup = TRUE;
	m_pGameModelManager->ShowDisplayModelAllGroup(this,bAsyn);
}

//����ĳ��Group
//���ظ�Group����Part��Lvl��Ϣ
void CDisplayModel::HideGroup(DWORD dwGroup)
{
	CDisplayGroup *pDisplayGroup = NULL;
	itDisplayGroup it = m_DisplayGroups.find(dwGroup);
	if(it != m_DisplayGroups.end())
	{
		pDisplayGroup = (*it).second;
		m_DisplayGroups.erase(it);

		GameModel::Group::Part::Level *pLevel= pDisplayGroup->GetLevelInfo();
		m_pGameModelManager->ReleaseLevelInfo(m_dwGameModelID,dwGroup,pDisplayGroup->GetPartName(),
			pDisplayGroup->GetPartLevel(),pLevel);
		delete 	pDisplayGroup;
	}
	return ;
}



/*
 * ����: ��������Group
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2010.01.25
 * �޸���־:
 */
void CDisplayModel::HideAllGroup()
{
	DWORD			 dwGroup;
	CDisplayGroup  * pDisplayGroup = NULL;
	itDisplayGroup	 it			   = m_DisplayGroups.begin();
	GameModel::Group::Part::Level * pLevel;

	while (it != m_DisplayGroups.end())
	{
		dwGroup		  = (*it).first;
		pDisplayGroup = (*it).second;
		it			  = m_DisplayGroups.erase(it);
		pLevel		  = pDisplayGroup->GetLevelInfo();

		m_pGameModelManager->ReleaseLevelInfo(m_dwGameModelID, dwGroup, pDisplayGroup->GetPartName(), pDisplayGroup->GetPartLevel(), pLevel);
		delete pDisplayGroup;
	}
}



BOOL CDisplayModel::SetLeveInfo(DWORD dwGroup,DWORD dwPart,DWORD dwLevel,
								GameModel::Group::Part::Level *pLevel)
{
	CDisplayGroup *pDisplayGroup = NULL;
	itDisplayGroup it = m_DisplayGroups.find(dwGroup);
	if(it != m_DisplayGroups.end())
	{
		pDisplayGroup = (*it).second;
		//Group,part,level��ͬ���˳�
		if( (pDisplayGroup->GetPartName() == dwPart ||
			pDisplayGroup->GetPartName() == 0) &&
			pDisplayGroup->GetPartLevel() == dwLevel )
		{
			pDisplayGroup->SetLevelInfo(pLevel);
			pDisplayGroup->SetPartNameLevel(dwPart,dwLevel);
		}
	}
	return TRUE;
}

void CDisplayModel::SetLocatorArray(mapLocators& Locators)
{
	m_Locators=Locators;
	itLocator it = m_Locators.begin();
	for(;it != m_Locators.end();it++)
	{
		itChildModel itChild = m_ChildDisplayModels.find((*it).first);
		if(itChild != m_ChildDisplayModels.end())
			(*itChild).second.pLocator = (*it).second;
	}
}

//�õ�group��partѡ�еĵȼ�
DWORD CDisplayModel::GetSelectedLvl(DWORD dwGroup,DWORD dwPart)
{
	DWORD dwLevel = 0;
	itDisplayGroup it = m_DisplayGroups.find(dwGroup);
	if(it != m_DisplayGroups.end())
	{
		dwLevel=(*it).second->GetPartLevel();
	}
	return dwLevel;
}

//�õ������θ���
DWORD CDisplayModel::GetTriangleCount(void)
{
	DWORD dwTriangleCount = 0;
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		dwTriangleCount += (*it).second->GetTriangleCount();
	}
	return dwTriangleCount;
}

//����ID�õ�locator
GameModel::Locator* CDisplayModel::GetLocator(DWORD dwLocator)
{
	itLocator it = m_Locators.find(dwLocator);
	if( it != m_Locators.end())
	{
		if((*it).second->GetLoadState() == Load_Did )
			return (*it).second;
	}
	return NULL;
}

void CDisplayModel::SetParticleEmitters(ParticleEmitters& partiEmitters)
{
	particle::Manager *pPM = particle::Manager::GetInstance();
	m_ParticleEmitters=partiEmitters;
	itParticleEmitter it = m_ParticleEmitters.begin();
	for (;it != m_ParticleEmitters.end();it++)
	{
		it->second = new GameModel::ParticleEmitter;
		it->second->CloneEmitter(partiEmitters[it->first]);
		particle::Emitter*& pemitter = it->second->GetParticleEmitterRef();
		pPM->Add(pemitter);
	}
}

void CDisplayModel::SetTextureProjective(TextureProjectives& ptextureprojective)
{
	m_TextureProjectives=ptextureprojective;
}

void CDisplayModel::SetTrailEmitters(TrailEmitters& trailEmitters)
{
	m_TrailEmitters=trailEmitters;
}

void CDisplayModel::EnableParticleEmitter(DWORD dwName,BOOL bValue)
{
	m_ParticleEnables[dwName]=bValue;
	//SetParticleEmitter(dwName,bValue);
}

void CDisplayModel::SetParticleEmitter(DWORD dwName,BOOL bValue)
{
	itParticleEmitter it = m_ParticleEmitters.find(dwName);
	if(it != m_ParticleEmitters.end())
	{
		GameModel::ParticleEmitter *pEmitter = (*it).second;
		pEmitter->Enable(bValue);
	}
}

void CDisplayModel::EnableTrailEmitter(DWORD dwName,BOOL bValue)
{
	m_TrailEnables[dwName] = bValue;
	//SetTrailEmitter(dwName,bValue);
}

void CDisplayModel::SetTrailEmitter(DWORD dwName,BOOL bValue)
{
	itTrailEmitter it = m_TrailEmitters.find(dwName);
	if(it != m_TrailEmitters.end())
	{

		GameModel::TrailEmitter *pEmitter = (*it).second;
		pEmitter->Enable(bValue);

	}
}

void CDisplayModel::EnableParticleEmitter(BOOL bValue)
{
	SetAllParticleEmitter(bValue);
}

BOOL CDisplayModel::IsParticleEnable(DWORD dwName)
{

	return m_ParticleEnables[dwName];


}


void CDisplayModel::EnableTrailEmitter(BOOL bValue)
{

	SetAllTrailEmitter(bValue);
}

void CDisplayModel::SetAllTrailEmitter(BOOL bValue)
{
	itTrailEnable it = m_TrailEnables.begin();
	for(;it != m_TrailEnables.end();it++)
	{
		(*it).second = bValue;
	}
}


BOOL CDisplayModel::IsTrailEnable(DWORD dwName)
{

	return m_TrailEnables[dwName];



	return FALSE;
}

void CDisplayModel::EnableTextureProjective(BOOL bValue)
{
	SetAllTextureProjective(bValue);
}

void CDisplayModel::SetAllParticleEmitter(BOOL bValue)
{
	itParticleEnable it = m_ParticleEnables.begin();
	for(;it != m_ParticleEnables.end();it++)
	{
		(*it).second = bValue;

	}
}

void CDisplayModel::SetAllTextureProjective(BOOL bValue)
{
	itTextureProjectives it = m_TextureProjectives.begin();
	for(;it != m_TextureProjectives.end();it++)
	{
		GameModel::TextureProjective *ptexproj = (*it).second;
		ptexproj->Enable(bValue);
	}
}

//ģ����ɫ����
void CDisplayModel::SetModelColor(DWORD dwColor)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetColor(dwColor);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetColor(dwColor);
			}
		}
	}
}


//void CDisplayModel::SetRegionAmbient(DWORD m)
//{
//	//����������Ҫ��ʾ�����
//	itDisplayGroup it = m_DisplayGroups.begin();
//	for(;it!=m_DisplayGroups.end();it++)
//	{
//		(*it).second->SetRegionAmbient(m_dwShaderGroupHandle,m);
//	}
//}

//void CDisplayModel::SetAmbient(DWORD dwcolor)
//{
//	//����������Ҫ��ʾ�����
//	itDisplayGroup it = m_DisplayGroups.begin();
//	for(;it!=m_DisplayGroups.end();it++)
//	{
//		(*it).second->SetAmbient(m_dwShaderGroupHandle,dwcolor);
//	}
//}
void CDisplayModel::ForceToEnableLight()
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->ForceToEnableLight();
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->ForceToEnableLight();
			}
		}
	}
}
void CDisplayModel::SetDirLightEnable(bool lightenable /* = true */)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{

		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetDirLightEnable(lightenable);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetDirLightEnable(lightenable);
			}
		}
	}
}
void CDisplayModel::SetUseNormal(bool busenormal)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetUseNormal(busenormal);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetUseNormal(busenormal);
			}
		}

	}
}



/*
* ����: �ܿ�ģ���Ƿ��ܵƹ�Ч��Ӱ�������,ǿ��Ϊģ���趨��ɫ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.12
*/
void CDisplayModel::SetModelColorCompel(DWORD dwColor)
{
	// ��������������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(; it != m_DisplayGroups.end(); ++it)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetColorCompel(dwColor);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->SetColorCompel(dwColor);
			}
		}

	}
}



void CDisplayModel::MultiplyModelColor(DWORD dwColor)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->MultiplyColor(dwColor);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->MultiplyColor(dwColor);
			}
		}

	}
}
void CDisplayModel::MultiplyModelAlpha(float fRatio)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->MultiplyAlpha(fRatio);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->MultiplyAlpha(fRatio);
			}
		}


	}
}



/*
* ����: ��������ģ�Ͷ���ʱ��ѭ��
* ժҪ: ���˸���������,Ŀ�����ڼ�ʹ��ִ�й�������������,Ҳ�ܽ�����������Ķ���ѭ�������ۼ�
* ����: pAnimInfo - ��ɫ������Ϣ
* ����ֵ: -
* ����: lpf
* ��������: 2008.05.28
* �޸���־:
*/
void CDisplayModel::ProcessAnimLoop(AnimInfo *pAnimInfo)
{
	DWORD					  dwCurrentTime = pAnimInfo->GetCurrentTime();
	AnimInfo::tagActionInfo * pActionInfo   = pAnimInfo->GetActionInfo();			//��ǰ����Ķ���

	// �����ֵ
	if (pActionInfo->bInterpolation)
	{
		// ��ֵ
		pActionInfo->dwInterpolationElapseTime = dwCurrentTime - pActionInfo->dwInterpolationStartTime;
		if (pActionInfo->dwInterpolationElapseTime > pActionInfo->dwInterpolationTimeLength)
		{
			// ��ֵ���
			pActionInfo->bInterpolation		   = FALSE;
			pActionInfo->dwCurActionStartTime  = dwCurrentTime;
			pActionInfo->dwCurActionLoopCount  = 0 ;
			pActionInfo->dwCurActionElapseTime = 0 ;
		}
	}else
	{
		GameModel::Action * pCurAction = m_pGameModelManager->GetActionAnim(m_dwGameModelID, pActionInfo->dwCurAction);
		if (pCurAction)				//�ж�������ʾ
		{
			// ���õ�ǰ�������ݵ�ʱ��
			model::AnimJoint * pAnimJoint = pCurAction->GetAnimJoint();	
			if (pAnimJoint)
			{
				// ���ݿ�ʼ������ʱ���趨֡
				DWORD dwCurActionTimeElapse = dwCurrentTime - pActionInfo->dwCurActionStartTime;
				DWORD dwTimeLength			= pAnimJoint->GetTimeLength(pActionInfo->fCurActionSpeedRate);

				// ����ѭ��
				if (dwCurActionTimeElapse >= dwTimeLength)
				{
					// ѭ��һ��
					dwCurActionTimeElapse = dwTimeLength;
					if (pActionInfo->bCurActionLooped)
					{
						pActionInfo->dwCurActionLoopCount += 1;
						pActionInfo->dwCurActionStartTime =  dwCurrentTime;
					}else
						pActionInfo->dwCurActionLoopCount = 1;
				}

				pActionInfo->dwCurActionElapseTime = dwCurActionTimeElapse;
			}
		}
	}
}



/*
* ����: ����ģ�͵Ĺ���������Ϣ
* ժҪ: ����ģ����ʾ,����ʱ�� -> ���㴦�� -��Ч������ -> ��Ⱦ
* ����: pAnimInfo - ��ɫ������Ϣ
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.02.14 - lpf
*		���Ӷ��û��Զ����ʱ��Ĵ���
*/
void CDisplayModel::ProcessAnimJoint(AnimInfo * pAnimInfo)
{
	DWORD					  dwCurrentTime = pAnimInfo->GetCurrentTime();
	AnimInfo::tagActionInfo * pActionInfo   = pAnimInfo->GetActionInfo();			//��ǰ����Ķ���

	// �����ֵ
	if (pActionInfo->bInterpolation)
	{
		// ��ֵ
		pActionInfo->dwInterpolationElapseTime = dwCurrentTime - pActionInfo->dwInterpolationStartTime;
		if (pActionInfo->dwInterpolationElapseTime > pActionInfo->dwInterpolationTimeLength)
		{
			// ��ֵ���
			pActionInfo->bInterpolation		   = FALSE;
			pActionInfo->dwCurActionStartTime  = dwCurrentTime;
			pActionInfo->dwCurActionLoopCount  = 0 ;
			pActionInfo->dwCurActionElapseTime = 0 ;
		}
	}else
	{
		GameModel::Action * pCurAction = m_pGameModelManager->GetActionAnim(m_dwGameModelID, pActionInfo->dwCurAction);
		if (pCurAction)				//�ж�������ʾ
		{
			// ���õ�ǰ�������ݵ�ʱ��
			model::AnimJoint * pAnimJoint = pCurAction->GetAnimJoint();	
			if (pAnimJoint)
			{
				// ���ݿ�ʼ������ʱ���趨֡
				DWORD dwCurActionTimeElapse = dwCurrentTime - pActionInfo->dwCurActionStartTime;
				DWORD dwTimeLength			= pAnimJoint->GetTimeLength(pActionInfo->fCurActionSpeedRate);

				// ����ѭ��
				if (dwCurActionTimeElapse >= dwTimeLength)
				{
					// ѭ��һ��
					dwCurActionTimeElapse = dwTimeLength;
					if (pActionInfo->bCurActionLooped)
					{
						pActionInfo->dwCurActionLoopCount += 1;
						pActionInfo->dwCurActionStartTime =  dwCurrentTime;
					}
					else
						pActionInfo->dwCurActionLoopCount = 1;
				}

				pActionInfo->dwCurActionElapseTime = dwCurActionTimeElapse;
			}
		}
	}

	const D3DXMATRIX * pWorldMatrix = pAnimInfo->GetWorldMatrix();
	const D3DXMATRIX * pBillboard   = pAnimInfo->GetBillboard();
	const D3DXMATRIX * pBillboardY  = pAnimInfo->GetBillboardY();

	GameModel::Action * pOldAction = m_pGameModelManager->GetActionAnim(m_dwGameModelID, pActionInfo->dwOldAction);
	GameModel::Action * pCurAction = m_pGameModelManager->GetActionAnim(m_dwGameModelID, pActionInfo->dwCurAction);

	model::AnimJoint * pOldAnimJoint = (pOldAction) ? pOldAction->GetAnimJoint() : NULL;	
	model::AnimJoint * pCurAnimJoint = (pCurAction) ? pCurAction->GetAnimJoint() : NULL;	

	DWORD dwCurActionTimeElapse = pActionInfo->dwCurActionElapseTime;

	if (pActionInfo->bStayToLastFrame && pCurAnimJoint)
		dwCurActionTimeElapse = pCurAnimJoint->GetTimeLength();

	DWORD dwOldActionTimeElapse    = pActionInfo->dwOldActionTimeElapse;
	DWORD dwInterplationTimeElapse = pActionInfo->dwInterpolationElapseTime;

	float fRatio = (float)dwInterplationTimeElapse / (float)pActionInfo->dwInterpolationTimeLength;

	// ��ʾ������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(; it != m_DisplayGroups.end(); ++it)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->ProcessAnimJoint(pAnimInfo, pOldAnimJoint, pCurAnimJoint, dwOldActionTimeElapse, dwCurActionTimeElapse, m_pBindPose, fRatio);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->ProcessAnimJoint(pAnimInfo, pOldAnimJoint, pCurAnimJoint, dwOldActionTimeElapse, dwCurActionTimeElapse, m_pBindPose, fRatio);
			}
		}

	}
}

void CDisplayModel::ProcessVisibility(AnimInfo *pAnimInfo)
{
	DWORD dwCurrentTime = pAnimInfo->GetCurrentTime();
	//ˢ��ģ�����ϵ���ɫ����Ч��
	DWORD dwTimeElapse = dwCurrentTime - pAnimInfo->m_dwAnimVisibilityStartTime;
	if (dwTimeElapse >= m_dwAnimVisibilityCircle)
	{
		//ѭ��һ��
		dwTimeElapse = m_dwAnimVisibilityCircle;
		pAnimInfo->m_dwAnimVisibilityStartTime = dwCurrentTime;
	}
	GameModel::Action* paction = m_pGameModelManager->GetActionAnim(m_dwGameModelID,pAnimInfo->GetActionInfo()->dwCurAction);
	if (paction)
	{
		if (!paction->GetVisibility())
		{
			return;
		}

	}
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->ProcessLevelAnimVisibility(m_dwShaderGroupHandle,dwTimeElapse,m_dwAnimVisibilityCircle);
	}
}
void CDisplayModel::ProcessAnimUV(AnimInfo *pAnimInfo)
{
	DWORD dwCurrentTime = pAnimInfo->GetCurrentTime();
	//ˢ��ģ�����ϵ���ɫ����Ч��
	DWORD dwTimeElapse = dwCurrentTime - pAnimInfo->m_dwAnimUVStartTime;
	if (dwTimeElapse >= m_dwAnimUVCircle)
	{
		//ѭ��һ��
		dwTimeElapse = m_dwAnimUVCircle;
		pAnimInfo->m_dwAnimUVStartTime = dwCurrentTime;
	}

	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->ProcessLevelAnimUV(dwTimeElapse,m_dwAnimUVCircle);
	}
}
void CDisplayModel::ProcessAnimTexture(AnimInfo *pAnimInfo)
{
	//��������Ч��
	DWORD dwCurrentTime = pAnimInfo->GetCurrentTime();

	//���ݿ�ʼ������ʱ���趨֡
	DWORD dwTimeElapse = dwCurrentTime - pAnimInfo->m_dwAnimTextureStartTime;
	if (dwTimeElapse >= m_dwAnimTextureCircle)
	{
		//ѭ��һ��
		dwTimeElapse = m_dwAnimTextureCircle;
		pAnimInfo->m_dwAnimTextureStartTime = dwCurrentTime;
	}

	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->ProcessLevelAnimTexture(dwTimeElapse,m_dwAnimTextureCircle);
	}
}

void CDisplayModel::ProcessAnimColor(AnimInfo *pAnimInfo)
{

	DWORD dwCurrentTime = pAnimInfo->GetCurrentTime();
	//ˢ��ģ�����ϵ���ɫ����Ч��
	DWORD dwAnimColorTimeElapse = dwCurrentTime - pAnimInfo->m_dwAnimColorStartTime;
	if (dwAnimColorTimeElapse >= m_dwAnimColorCircle)
	{
		//ѭ��һ��
		dwAnimColorTimeElapse = m_dwAnimColorCircle;
		pAnimInfo->m_dwAnimColorStartTime = dwCurrentTime;
	}

	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->ProcessLevelAnimColor(dwAnimColorTimeElapse,m_dwAnimColorCircle);
	}
}

void CDisplayModel::ProcessEnvironmentMaps(const D3DXMATRIX *pViewMatrix,bool bnormalize)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->ProcessEnvironmentMaps(pViewMatrix,bnormalize);
	}
}

void CDisplayModel::ProcessLocator(AnimInfo *pAnimInfo)
{
	//��ǰ����Ķ���  
	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();
	DWORD  dwCurrentTime = pAnimInfo->GetCurrentTime();

	const D3DXMATRIX *pWorldMatrix = pAnimInfo->GetWorldMatrix();
	const D3DXMATRIX *pBillboard = pAnimInfo->GetBillboard();
	const D3DXMATRIX *pBillboardY = pAnimInfo->GetBillboardY();

	GameModel::Action *pOldAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwOldAction);
	GameModel::Action *pCurAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwCurAction);

	model::AnimJoint *pOldAnimJoint = (pOldAction)?  pOldAction->GetAnimJoint() : NULL;	
	model::AnimJoint *pCurAnimJoint = (pCurAction)?  pCurAction->GetAnimJoint() : NULL;	

	DWORD dwCurActionTimeElapse = pActionInfo->dwCurActionElapseTime;
	DWORD dwOldActionTimeElapse = pActionInfo->dwOldActionTimeElapse;

	DWORD dwInterplationTimeElapse = pActionInfo->dwInterpolationElapseTime;

	float fRatio = (float)dwInterplationTimeElapse / (float)pActionInfo->dwInterpolationTimeLength;

	itLocator it = m_Locators.begin();
	//������ʾ�����ȡ��������㿽��ȱʡ����
	for(; it != m_Locators.end();it++)
	{
		GameModel::Locator *pLocator = (*it).second;
		if(pLocator->GetLoadState() != Load_Did) continue;

		D3DXMATRIX matnowworld,matnowmodel;
		model::Mesh   *pMesh   = pLocator->GetMesh();
		model::Buffer *pBuffer = pLocator->GetBuffer();
		if(pMesh == NULL || pBuffer == NULL)	continue;
		if (pActionInfo->bInterpolation && pOldAnimJoint && pCurAnimJoint&&pMesh->GetGroupCount())
		{
			pMesh->ProcessAnimJointInterpolation(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),m_pBindPose,pOldAnimJoint,dwOldActionTimeElapse,pActionInfo->fOldActionSpeedRate,pCurAnimJoint,0,1.0f,fRatio,matnowworld,matnowmodel); 
		}
		else
		{
			if (pCurAnimJoint&&pMesh->GetGroupCount())
			{
				pMesh->ProcessAnimJoint(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),m_pBindPose,pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate,matnowworld,matnowmodel);
			}	
			else
			{
				D3DXMatrixIdentity(&matnowworld);
				D3DXMatrixIdentity(&matnowmodel);
				pMesh->CopyPosition(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix());
			}
		}
		pLocator->SetNowWorldMatrix(matnowworld);
		pLocator->SetNowModelMatrix(matnowmodel);
	}
}
void CDisplayModel::ProcessPickMesh(AnimInfo *pAnimInfo)
{
	if(m_pPickMesh == NULL || m_pPickMesh->GetLoadState() != Load_Did)
		return;
	//��ǰ����Ķ���  
	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();
	DWORD  dwCurrentTime = pAnimInfo->GetCurrentTime();

	const D3DXMATRIX *pWorldMatrix = pAnimInfo->GetWorldMatrix();
	const D3DXMATRIX *pBillboard = pAnimInfo->GetBillboard();
	const D3DXMATRIX *pBillboardY = pAnimInfo->GetBillboardY();

	GameModel::Action *pOldAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwOldAction);
	GameModel::Action *pCurAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwCurAction);

	model::AnimJoint *pOldAnimJoint = (pOldAction)?  pOldAction->GetAnimJoint() : NULL;	
	model::AnimJoint *pCurAnimJoint = (pCurAction)?  pCurAction->GetAnimJoint() : NULL;	

	DWORD dwCurActionTimeElapse = pActionInfo->dwCurActionElapseTime;
	DWORD dwOldActionTimeElapse = pActionInfo->dwOldActionTimeElapse;

	DWORD dwInterplationTimeElapse = pActionInfo->dwInterpolationElapseTime;

	float fRatio = (float)dwInterplationTimeElapse / (float)pActionInfo->dwInterpolationTimeLength;


	model::Mesh   *pMesh   = m_pPickMesh->GetMesh();
	model::Buffer *pBuffer = m_pPickMesh->GetBuffer();
	if(pMesh == NULL || pBuffer == NULL)	return;

	if (pActionInfo->bInterpolation && pOldAnimJoint && pCurAnimJoint&&pMesh->GetGroupCount())
	{
		pMesh->ProcessAnimJointInterpolation(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),m_pBindPose,pOldAnimJoint,dwOldActionTimeElapse,pActionInfo->fOldActionSpeedRate,pCurAnimJoint,0,1.0f,fRatio); 
	}
	else
	{
		if (pCurAnimJoint&&pMesh->GetGroupCount())
		{
			pMesh->ProcessAnimJoint(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),m_pBindPose,pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate);
		}	
		else
		{

			pMesh->CopyPosition(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix());
		}
	}

}
void CDisplayModel::ProcessTextureProjective(AnimInfo *pAnimInfo)
{
	if(!render::Interface::GetInstance()->GetSetting()->bborderaddress)
	{
		return;
	}
	const D3DXMATRIX* matworld = pAnimInfo->GetWorldMatrix();
	GameModel::TextureProjective* ptexprj;
	std::map<DWORD,GameModel::TextureProjective*>::iterator it = m_TextureProjectives.begin();
	DWORD dwTimeElapse = pAnimInfo->GetCurrentTime() - pAnimInfo->m_dwtextureprojectivetime;

	for (;it != m_TextureProjectives.end();it++)
	{
		ptexprj = it->second;
		if (ptexprj->IsEnable())
		{
			ptexprj->GetTextureProjective()->SetPosDirAlphaNowtime(matworld->_41,0,matworld->_43,pAnimInfo->GetDirY(),pAnimInfo->GetTextureProjectiveAlpha(),dwTimeElapse);
			pAnimInfo->SetTextureProjectiveAlpha(-1.0f);


		}
		//ptexprj->GetTextureProjective()->SetNowTime(pAnimInfo->GetCurrentTime());
		//ptexprj->GetTextureProjective()->UpdateMapArray();
		//ptexprj->GetTextureProjective()->Rend();
		// GetGame()->GetRegion->GetGameMap()->GetTagMapBuffer(matworld->_41,matworld->_43,ptexprj->GetTextureProjective()->GetSize(),ptexprj->GetTextureProjective()->GetBuffer());

	}
}



/*
* ����: ����ˮ��Ч��
* ժҪ: Ŀǰ����Խ���
* ����: pAnimInfo - ������Ϣ
* ����ֵ: -
* ����: lpf
* ��������: 2009.10.20
* �޸���־:
*/
void CDisplayModel::ProcessWater(AnimInfo * pAnimInfo)
{
	if (!render::Interface::GetInstance()->GetSetting()->bborderaddress ||
		m_pWater == NULL)
		return;

	Water::_tagRenderElement re;
	re.vPos.x		= pAnimInfo->GetWorldMatrix()->_41;
	re.vPos.y		= pAnimInfo->GetWorldMatrix()->_42 + 0.01f;
	re.vPos.z		= pAnimInfo->GetWorldMatrix()->_43;
	re.dwElapseTime = pAnimInfo->GetCurrentTime() - pAnimInfo->m_dwAnimWaterStartTime;
	re.pWater		= m_pWater;

	Water::CManager::GetInstance()->AddRenderWater(re);
}



void CDisplayModel::ProcessLocator(AnimInfo *pAnimInfo,GameModel::Locator* pLocator)
{
	if(NULL == pLocator || pLocator->GetLoadState() != Load_Did) return;
	//��ǰ����Ķ���  
	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();
	DWORD  dwCurrentTime = pAnimInfo->GetCurrentTime();

	const D3DXMATRIX *pWorldMatrix = pAnimInfo->GetWorldMatrix();
	const D3DXMATRIX *pBillboard = pAnimInfo->GetBillboard();
	const D3DXMATRIX *pBillboardY = pAnimInfo->GetBillboardY();

	GameModel::Action *pOldAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwOldAction);
	GameModel::Action *pCurAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwCurAction);

	model::AnimJoint *pOldAnimJoint = (pOldAction)?  pOldAction->GetAnimJoint() : NULL;	
	model::AnimJoint *pCurAnimJoint = (pCurAction)?  pCurAction->GetAnimJoint() : NULL;	

	DWORD dwCurActionTimeElapse = pActionInfo->dwCurActionElapseTime;
	DWORD dwOldActionTimeElapse = pActionInfo->dwOldActionTimeElapse;

	DWORD dwInterplationTimeElapse = pActionInfo->dwInterpolationElapseTime;

	float fRatio = (float)dwInterplationTimeElapse / (float)pActionInfo->dwInterpolationTimeLength;

	D3DXMATRIX matnowworld,matnowmodel;
	model::Mesh   *pMesh   = pLocator->GetMesh();
	model::Buffer *pBuffer = pLocator->GetBuffer();
	if(pMesh == NULL || pBuffer == NULL)return;
	if (pActionInfo->bInterpolation && pOldAnimJoint && pCurAnimJoint&&pMesh->GetGroupCount())
	{
		pMesh->ProcessAnimJointInterpolation(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),m_pBindPose,pOldAnimJoint,dwOldActionTimeElapse,pActionInfo->fOldActionSpeedRate,pCurAnimJoint,0,1.0f,fRatio,matnowworld,matnowmodel); 
	}
	else
	{
		if (pCurAnimJoint&&pMesh->GetGroupCount())
		{
			pMesh->ProcessAnimJoint(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),m_pBindPose,pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate,matnowworld,matnowmodel);
		}	
		else
		{
			D3DXMatrixIdentity(&matnowworld);
			D3DXMatrixIdentity(&matnowmodel);
			pMesh->CopyPosition(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix());
		}
	}
	pLocator->SetNowWorldMatrix(matnowworld);
	pLocator->SetNowModelMatrix(matnowmodel);
}
void CDisplayModel::ProcessLocatorByFrame(AnimInfo *pAnimInfo,GameModel::Locator* pLocator,DWORD dwframe)
{
	if(NULL == pLocator || pLocator->GetLoadState() != Load_Did) return;
	//��ǰ����Ķ���  
	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();
	DWORD  dwCurrentTime = pAnimInfo->GetCurrentTime();

	const D3DXMATRIX *pWorldMatrix = pAnimInfo->GetWorldMatrix();
	const D3DXMATRIX *pViewMatrix = pAnimInfo->GetViewMatrix();
	const D3DXMATRIX *pProjMatrix = pAnimInfo->GetProjectedMatrix();

	GameModel::Action *pCurAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwCurAction);

	model::AnimJoint *pCurAnimJoint = (pCurAction)?  pCurAction->GetAnimJoint() : NULL;	


	model::Mesh   *pMesh   = pLocator->GetMesh();
	model::Buffer *pBuffer = pLocator->GetBuffer();
	if(pMesh == NULL || pBuffer == NULL)	return;
	if (pCurAnimJoint)
	{
		pMesh->ProcessAnimJointByFrame(pBuffer,pWorldMatrix,pViewMatrix,pProjMatrix,m_pBindPose,pCurAnimJoint,dwframe);
	}	
	else
	{
		pMesh->CopyPosition(pBuffer,pWorldMatrix,pViewMatrix,pProjMatrix);
	}


}

void CDisplayModel::ProcessParticleEmitter(AnimInfo *pAnimInfo,bool bdrawtoscreen,bool brendtotarget)
{
	if(m_eParticleLoadState != Load_Did)
	{
		CheckParticleLoadState();
		return;
	}
	EnableParticleEmitter(FALSE);
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		CDisplayGroup* dgp= (*it).second;
		if (dgp->GetLevelInfo())
		{
			std::vector<DWORD>vParticleEnable = dgp->GetLevelInfo()->GetParticleEnable();
			for (int i = 0;i < vParticleEnable.size();i++)
			{
				EnableParticleEmitter(vParticleEnable[i],TRUE);
			}
		}
	}
	//ˢ������
	const D3DXMATRIX *pWorldMatrix = pAnimInfo->GetWorldMatrix();
	const D3DXMATRIX *pViewMatrix = pAnimInfo->GetViewMatrix();
	const D3DXMATRIX *pProjMatrix = pAnimInfo->GetProjectedMatrix();
	const D3DXMATRIX *pBillboard = pAnimInfo->GetBillboard();
	const D3DXMATRIX *pBillboardY = pAnimInfo->GetBillboardY();

	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();

	GameModel::Action *pOldAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwOldAction);
	GameModel::Action *pCurAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwCurAction);

	model::AnimJoint *pCurAnimJoint = (pCurAction)? pCurAction->GetAnimJoint() : NULL;
	model::AnimJoint *pOldAnimJoint = (pOldAction)?  pOldAction->GetAnimJoint() : NULL;	

	DWORD dwCurActionTimeElapse = pActionInfo->dwCurActionElapseTime;
	DWORD dwOldActionTimeElapse = pActionInfo->dwOldActionTimeElapse;

	DWORD dwInterplationTimeElapse = pActionInfo->dwInterpolationElapseTime;

	float fRatio = (float)dwInterplationTimeElapse / (float)pActionInfo->dwInterpolationTimeLength;

	DWORD dwCurrentTime = pAnimInfo->GetCurrentTime();
	itParticleEmitter itEmitter = m_ParticleEmitters.begin();
	//����������
	DWORD dwParticleEmitterTimeIndex = 0;
	for(; itEmitter != m_ParticleEmitters.end();itEmitter++)
	{
		GameModel::ParticleEmitter *pEmitter = itEmitter->second;
		if(pEmitter->GetLoadState() != Load_Did)
			continue;

		//����ֻˢ������ʱ��		
		if (pEmitter->GetParticleEmitter()->UpdateTime(pAnimInfo->m_dwParticleEmitterLastTimeArray[dwParticleEmitterTimeIndex],dwCurrentTime))
		{	
			pAnimInfo->m_dwParticleEmitterLastTimeArray[dwParticleEmitterTimeIndex] = dwCurrentTime;
		}
		dwParticleEmitterTimeIndex++;
		//
		GameModel::Action::tagParticleParameter *pPP = pCurAction ? pCurAction->GetParticleParameter(pEmitter->GetName()) : NULL;
		DWORD dwStartTime(0),dwEndTime(0xFFFFFFFF);

		if (pPP)
		{
			if (!pPP->bVisibile)
			{
				//���ɼ�
				continue;
			}
			dwStartTime = pPP->dwStartTime;
			dwEndTime   = pPP->dwEndTime;
		}
		if (IsParticleEnable( pEmitter->GetName() ))
		{
			model::Mesh   * pMesh      = pEmitter->GetMesh();
			particle::Emitter * pParticleEmitter = pEmitter->GetParticleEmitter();
			D3DXMATRIX mattemp = *pWorldMatrix;
			D3DXVECTOR3 srcposition(0.0f,0.0f,0.0f);
			//����Ƿ��ǿ�ʼʱ��ͽ���
			if (dwCurActionTimeElapse < dwStartTime || dwCurActionTimeElapse > dwEndTime)
			{
				if (pParticleEmitter->GetParameter()->btransinownzone)
				{
					pParticleEmitter->SetOwnZoneMatrix(&mattemp);
				}

				continue;
			}

			if(NULL == pMesh || NULL == pParticleEmitter)
				continue;
			model::Buffer * pBuffer   = pEmitter->GetBuffer();
			D3DXVECTOR3   * pPosBuffer = pBuffer->GetPosBuffer();	
			D3DXMATRIX matworld,matmodel;
			const D3DXVECTOR3 *pVec3Joint = pAnimInfo->GetSelfJointArrayPos();
			if (pAnimInfo->IsSelfAnimJoint() && pVec3Joint&&pCurAnimJoint)
			{
				pMesh->ProcessAnimJointSelf(pBuffer,pWorldMatrix,pViewMatrix,pProjMatrix,m_pBindPose,
				                        pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate,
										matworld,matmodel,pVec3Joint);
			}
			else
			{
				if (pActionInfo->bInterpolation && pOldAnimJoint && pCurAnimJoint&&pMesh->GetGroupCount())       
					pMesh->ProcessAnimJointInterpolation(pBuffer,pWorldMatrix,pViewMatrix,pProjMatrix,m_pBindPose,pOldAnimJoint,dwOldActionTimeElapse,pActionInfo->fOldActionSpeedRate,pCurAnimJoint,0,1.0f,fRatio,matworld,matmodel); 
				else
				{
					if (pCurAnimJoint&&pMesh->GetGroupCount())
					{
						pMesh->ProcessAnimJoint(pBuffer,pWorldMatrix,pViewMatrix,pProjMatrix,m_pBindPose,pCurAnimJoint,dwCurActionTimeElapse,pActionInfo->fCurActionSpeedRate,matworld,matmodel);
					}
					else
					{
						D3DXMatrixIdentity(&matworld);
						D3DXMatrixIdentity(&matmodel);
						pMesh->CopyPosition(pBuffer,pWorldMatrix,pViewMatrix,pProjMatrix);
						matworld = *pWorldMatrix;

					}
				}
			}

			pParticleEmitter->SetBillboard(pBillboard); 
			pParticleEmitter->SetBillboardY(pBillboardY);

			if (pParticleEmitter->GetParameter()->btransinownzone)
			{
				pParticleEmitter->SetOwnZoneMatrix(&mattemp);
				D3DXVECTOR3* meshbuffer = pMesh->GetPosBuffer();
				D3DXVec3TransformCoord(&srcposition,meshbuffer,&matmodel);
			}
			if (pParticleEmitter->GetParameter()->bfollowedEmitter)
			{
				matworld._41 = matworld._42 = matworld._43 = 0.0f;
				pParticleEmitter->SetNowMatrix(&matworld);
			}
			else if (pParticleEmitter->GetParameter()->bfollowedowner)
			{
				mattemp._41 = mattemp._42 = mattemp._43 = 0.0f;
				pParticleEmitter->SetNowMatrix(&mattemp);
			}
			else
			{
				D3DXMatrixIdentity(&matworld);
				pParticleEmitter->SetNowMatrix(&matworld);
			}

			for(DWORD i = 0; i < pEmitter->GetEmitteNumber(); i++)
			{

				switch(pEmitter->GetType())
				{
				case 0://POINT
					{
						int iIndex = rand() % pBuffer->GetPNCCount();  		
						//����

						pParticleEmitter->SetPosition(&pPosBuffer[iIndex]);
						pParticleEmitter->SetSrcPosition(&srcposition);

					}
					break;
				case 1://LINE
					{
						//���λ�ü���һ��λ��
						int iPosCount = pBuffer->GetPNCCount();
						int iIndexStart = rand() % iPosCount;
						int iIndexEnd   = (iIndexStart + 1) % iPosCount;

						D3DXVECTOR3 &pos0 = pPosBuffer[iIndexStart];
						D3DXVECTOR3 &pos1 = pPosBuffer[iIndexEnd];

						//��ֵ
						D3DXVECTOR3 vNormal = pos1 - pos0;
						float fLength = D3DXVec3Length(&vNormal);
						D3DXVec3Normalize(&vNormal,&vNormal);
						float fCur = (float)rand() / RAND_MAX * fLength;
						D3DXVECTOR3 vCur = pos0 + vNormal * fCur;
						pParticleEmitter->SetPosition(&vCur);
						pParticleEmitter->SetSrcPosition(&srcposition);

					}
					break;
				}
				pParticleEmitter->SetDrawToScreen(bdrawtoscreen);
				pParticleEmitter->SetRenderToTarget(brendtotarget);
				pParticleEmitter->Emission();
			}
		}
	}
}

void CDisplayModel::ProcessTrailEmitter(AnimInfo *pAnimInfo)
{
	//����β��Ч��
	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();
	//���㶯��
	GameModel::Action *pCurAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwCurAction);
	if (!pCurAction)
	{
		//�޶����޷���β
		return;
	}
	model::AnimJoint *pAnimJoint = 	pCurAction->GetAnimJoint();
	//����û���ڲ壬Ӧ���޸�֮
	DWORD dwCurrentTime = pAnimInfo->GetCurrentTime();
	DWORD dwAnimTimeElapse = pAnimInfo->GetActionInfo()->dwCurActionElapseTime;

	if (pAnimJoint)
	{
		DWORD dwTCIndex = 0;
		itTrailEmitter itEmitter = m_TrailEmitters.begin();
		for(; itEmitter != m_TrailEmitters.end();itEmitter++)
		{
			GameModel::TrailEmitter *pEmitter = itEmitter->second;
			if (pEmitter->GetLoadState() != Load_Did || !pCurAction->GetTrailVisible(pEmitter->GetName()))
			{
				continue;
			}
			if (!IsTrailEnable(pEmitter->GetName()))
			{
				continue;
			}
			//
			model::Mesh   *pMesh      = pEmitter->GetMesh();
			model::Buffer *pBuffer    = pEmitter->GetBuffer();
			D3DXVECTOR3   *pPosBuffer = pBuffer->GetPosBuffer();
			if(NULL == pMesh)	continue;

			DWORD dwTimeElapse = dwCurrentTime - pActionInfo->dwCurActionStartTime;
			if (dwTimeElapse > pAnimJoint->GetTimeLength())
			{
				dwTimeElapse = pAnimJoint->GetTimeLength();
			}
			if (pMesh->GetGroupCount())
			{
				pMesh->ProcessAnimJoint(pBuffer,pAnimInfo->GetWorldMatrix(),pAnimInfo->GetViewMatrix(),pAnimInfo->GetProjectedMatrix(),m_pBindPose,pAnimJoint,dwAnimTimeElapse,pActionInfo->fCurActionSpeedRate);
			}
			GameModel::TrailContainer::tagTrail *pTrail = pAnimInfo->m_pTrailContainerArray[dwTCIndex].GetFreeTrail();//array of 6 	

			// [0] = a[i]  [1] = b[i] [2] = b[i+1]
			pTrail->vPos[0]     = pPosBuffer[0];
			pTrail->vPos[1]     = pPosBuffer[1];
			pTrail->dwStartTime = pAnimInfo->GetCurrentTime();
			pTrail->dwTimeLength= pEmitter->GetLifeTimeLength(); 
			unsigned __int64 qwColor = pEmitter->GetColor();
			pTrail->dwTopColor	= (DWORD)(qwColor >> 32);
			pTrail->dwBottomColor = (DWORD)(qwColor & 0xFFFFFFFF);
			//OutputConsole("%64x %x %x\n",pTrail->dwTopColor,pTrail->dwBottomColor);

			pAnimInfo->m_pTrailContainerArray[dwTCIndex].Update(dwCurrentTime);
			pAnimInfo->m_pTrailContainerArray[dwTCIndex].Render(m_dwShaderGroupHandle,
				pMesh->GetShaderIndex());
			dwTCIndex++;
		}



	}
}

//���ռ���
void CDisplayModel::ProcessDirectionalLight(const D3DXVECTOR3 *pCameraPosition,
											const D3DXVECTOR3 *pLightDirectiion,DWORD dwColor)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->ProcessDirectionalLight(pCameraPosition,pLightDirectiion,dwColor);
	}
}
void CDisplayModel::CopyBufferToColor(std::vector<D3DCOLOR*>& vecbuffer)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->CopyBufferToColor(vecbuffer);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->CopyBufferToColor(vecbuffer);
			}
		}

	}
}
void CDisplayModel::CopyColorToBuffer(std::vector<D3DCOLOR*>& vecbuffer)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{

		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->CopyColorToBuffer(vecbuffer);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->CopyColorToBuffer(vecbuffer);
			}
		}
	}
}
void CDisplayModel::ProcessPointLight(const D3DXVECTOR3 *pLightPosition,
									  float fIntensity,DWORD dwColor)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->ProcessPointLight(pLightPosition,fIntensity,dwColor);
	}
}

//��Ⱦģ��
void CDisplayModel::RenderModel()
{

	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->Render(m_dwShaderGroupHandle);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->Render(m_dwShaderGroupHandle);
			}
		}

	}
}


// ��Ⱦ͸��ģ��
void CDisplayModel::RenderModelTransparent()
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->RenderTransparent(m_dwShaderGroupHandle);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->RenderTransparent(m_dwShaderGroupHandle);
			}
		}

	}
}



/*
* ����: ��Ⱦ����ģ��
* ժҪ: ע��Ҫ�ﵽ���׹�Ч��,�������ɫֵ��RGB������������һ��
* ����: dwFlashColor - ������ɫ
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.23
* �޸���־:
*	2008.04.21 - lpf
*		�����˶Լ�ģ����Ⱦ�Ĵ���
*/
void CDisplayModel::RenderModelFlash(DWORD dwFlashColor)
{
	SetModelColorCompel(dwFlashColor);

	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(; it != m_DisplayGroups.end(); ++it)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
				(*it).second->RenderModelFlash(m_dwShaderGroupHandle);
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
				(*it).second->RenderModelFlash(m_dwShaderGroupHandle);
		}
	}
}

//ֱ�ӻ�ģ��
void CDisplayModel::DrawModel(void)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawAlphaTestOnly(m_dwShaderGroupHandle);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawAlphaTestOnly(m_dwShaderGroupHandle);
			}
		}

	}
	it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawAlphaBlendOnly(m_dwShaderGroupHandle);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawAlphaBlendOnly(m_dwShaderGroupHandle);
			}
		}

	}
}
void CDisplayModel::DrawModel(DWORD Group)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawAlphaTestOnly(m_dwShaderGroupHandle);
				return;
			}
		}
		else if ((*it).first == Group)
		{
			(*it).second->DrawAlphaTestOnly(m_dwShaderGroupHandle);
		}

	}
	it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawAlphaBlendOnly(m_dwShaderGroupHandle);
				return;
			}
		}
		else if ((*it).first == Group)
		{
			(*it).second->DrawAlphaBlendOnly(m_dwShaderGroupHandle);
		}
	}
}
void CDisplayModel::DrawModelWithoutState()
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawWithoutState(m_dwShaderGroupHandle);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawWithoutState(m_dwShaderGroupHandle);
			}
		}
	}
}
void CDisplayModel::DrawModelWithoutStateAndTexture()
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawWithoutStateAndTexture(m_dwShaderGroupHandle);
				return;
			}
		}
		else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
			{
				(*it).second->DrawWithoutStateAndTexture(m_dwShaderGroupHandle);
			}
		}

	}
}



/*
* ����: ʹ����ɫ����ֱ�ӻ���ģ��
* ժҪ: -
* ����: dwColorOp			 - ��ɫ����
*		 dwColorArg1		 - ��ɫ����1
*		 dwColorArg2		 - ��ɫ����2
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.16
* �޸���־:
*	2008.04.21 - lpf
*		�����˶Լ�ģ����Ⱦ�Ĵ���
*/
void CDisplayModel::DrawModelByColorOp(DWORD dwColorOp, DWORD dwColorArg1, DWORD dwColorArg2)
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(; it != m_DisplayGroups.end(); ++it)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
				(*it).second->DrawAlphaTestByColorOp(m_dwShaderGroupHandle, dwColorOp, dwColorArg1, dwColorArg2);
		}else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
				(*it).second->DrawAlphaTestByColorOp(m_dwShaderGroupHandle, dwColorOp, dwColorArg1, dwColorArg2);
		}
	}		

	it = m_DisplayGroups.begin();
	for(; it != m_DisplayGroups.end(); ++it)
	{
		if (m_brendsimplemesh)
		{
			if ((*it).first == MAKEFOURCC('S','P','M','S'))
				(*it).second->DrawAlphaBlendByColorOp(m_dwShaderGroupHandle, dwColorOp, dwColorArg1, dwColorArg2);
		}else
		{
			if ((*it).first != MAKEFOURCC('S','P','M','S'))
				(*it).second->DrawAlphaBlendByColorOp(m_dwShaderGroupHandle, dwColorOp, dwColorArg1, dwColorArg2);
		}
	}
}



//��Ⱦ��Icon
void CDisplayModel::DisplayIcon(long lX,long lY,float fScale,float fRotation,AnimInfo *pAnimInfo)
{

	render::Interface *pInterface = render::Interface::GetInstance();
	float fWidth = (float)pInterface->GetWndWidth();
	float fHeight= (float)pInterface->GetWndHeight();	

	float fDrawX = lX - fWidth / 2;
	float fDrawY = fHeight / 2 - lY;
	static D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );

	static D3DXMATRIX matProj;
	D3DXMatrixOrthoLH( &matProj,fWidth, fHeight, -10000.0f, 10000.0f );

	pInterface->GetDevice()->SetTransform( D3DTS_WORLD, &matWorld);
	D3DXMatrixTranslation(&matWorld,0,0,10000.0f); 
	pInterface->GetDevice()->SetTransform( D3DTS_VIEW, &matWorld );
	pInterface->GetDevice()->SetTransform( D3DTS_PROJECTION, &matProj );

	static D3DXMATRIX matBillboard;
	D3DXMatrixInverse( &matBillboard, NULL, &matWorld );
	matBillboard._41 = matBillboard._42 = matBillboard._43 = 0.0f;

	static D3DXMATRIX matModelWorld,matSCL,matROT;

	D3DXMatrixRotationY( &matROT,fRotation);
	D3DXMatrixScaling(&matSCL,fScale,fScale,fScale);

	D3DXMatrixTranslation(&matModelWorld,(float)fDrawX,(float)fDrawY,-50.0f);
	D3DXMatrixMultiply(&matModelWorld,&matROT,&matModelWorld);
	D3DXMatrixMultiply(&matModelWorld,&matSCL,&matModelWorld);

	pAnimInfo->SetWorldMatrix(&matModelWorld);
	pAnimInfo->SetCurrentTime(timeGetTime());
	pAnimInfo->SetBillboard(&matBillboard);
	pAnimInfo->SetBillboardY(&matBillboard);

	ProcessAnimJoint(pAnimInfo);
	ProcessAnimTexture(pAnimInfo);
	ProcessEnvironmentMaps(&matWorld);
	ProcessAnimUV(pAnimInfo);	
	ProcessAnimColor(pAnimInfo);	
	DrawModel();
}
//��Ⱦ��Icon
void CDisplayModel::DrawModelByAniminfo(AnimInfo *pAnimInfo)
{

	DWORD nowtime = timeGetTime();
	pAnimInfo->SetCurrentTime(nowtime);
	//pAnimInfo->SetWorldMatrix(&matWorld);
	//SetUseNormal(true);
	ProcessAnimJoint(pAnimInfo);
	ProcessEnvironmentMaps(pAnimInfo->GetViewMatrix(),true);
	ProcessAnimColor(pAnimInfo);
	ProcessVisibility(pAnimInfo);
	ProcessAnimTexture(pAnimInfo);
	ProcessAnimUV(pAnimInfo);
	ProcessLocator(pAnimInfo);
	ProcessParticleEmitter(pAnimInfo,true);
	//pmodel->DrawModelWithoutState();
	DrawModel();
	//	D3DXSaveSurfaceToFile("1.tga",D3DXIFF_TGA,CTextureProjective::m_pTextureSurf,NULL,NULL);
}

void CDisplayModel::Dump(void)
{
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		DWORD dwName = (*it).first;
		OutputConsole("Dump: %c%c%c%c\n",dwName >> 24 , dwName >> 16 & 0xff,dwName >> 8 & 0xff,dwName & 0xff);
	}
}

//����
void CDisplayModel::_DEBUG_RenderNormal()
{
	//����������Ҫ��ʾ�����
	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->_DEBUG_RenderNormal();
	}
}

void CDisplayModel::_DEBUG_RenderLocator()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D   *pLayer3D   = pInterface->GetLayer3D();

	itLocator it =  m_Locators.begin();

	for(; it != m_Locators.end();it++)
	{
		GameModel::Locator *pLocator = (*it).second;
		model::Buffer *pBuffer = pLocator->GetBuffer();
		pBuffer->Render(m_dwShaderGroupHandle,0);
	}	
}

void CDisplayModel::_DEBUG_RenderPickBox(const D3DXMATRIX *pWroldMatrix,D3DCOLOR dwColor)
{
	if (m_pPickBox)
		m_pPickBox->_DEBUG_Render(pWroldMatrix,dwColor);
}

void CDisplayModel::_DEBUG_RenderSizeBox(const D3DXMATRIX *pWroldMatrix,D3DCOLOR dwColor)
{
	if (m_pSizeBox)
		m_pSizeBox->_DEBUG_Render(pWroldMatrix,dwColor);
}


void CDisplayModel::_DEBUG_RenderAction(AnimInfo *pAnimInfo)
{
	const AnimInfo::tagActionInfo *pActionInfo = pAnimInfo->GetActionInfo();
	GameModel::Action *pCurAction =
		m_pGameModelManager->GetActionAnim(m_dwGameModelID,pActionInfo->dwCurAction);
	DWORD dwActionTimeElapse = pAnimInfo->GetActionInfo()->dwCurActionElapseTime;
	if (pCurAction)//�ж�������Ⱦ
	{
		//������Ⱦskeleton
		model::AnimJoint *pAnimJoint = pCurAction->GetAnimJoint();	
		pAnimJoint->_DEBUG_Render(pAnimInfo->GetWorldMatrix(),dwActionTimeElapse);  
	}
}


//������ģ�ͽڵ�
void CDisplayModel::AddChildModel(DWORD dwLocatorID,CDisplayModel* pDisplayModel)
{
	if(NULL == pDisplayModel)	return;
	//�ж��Ƿ��д�Locator
	itChildModel itModel = m_ChildDisplayModels.find(dwLocatorID);
	if(itModel != m_ChildDisplayModels.end())
	{
		(*itModel).second.setChildModels.insert(pDisplayModel);
	}

	itLocator it = m_Locators.find(dwLocatorID);
	tagChildModelInfo ChildModel;
	if(it != m_Locators.end() )
		ChildModel.pLocator = (*it).second;
	else
		ChildModel.pLocator = NULL;

	ChildModel.setChildModels.insert(pDisplayModel);
	m_ChildDisplayModels[dwLocatorID]= ChildModel;
}

void CDisplayModel::RemoveChildeModel(DWORD dwLocatorID,CDisplayModel* pDisplayModel)
{
	itChildModel itModel = m_ChildDisplayModels.find(dwLocatorID);
	if(itModel != m_ChildDisplayModels.end())
	{
		(*itModel).second.setChildModels.erase(pDisplayModel);
	}
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//Ϊ�߼����ṩ��Ӧ�ýӿ�

//�õ���λ��λ��
BOOL CDisplayModel::GetLocatorPos(AnimInfo *pAnimInfo,DWORD dwLocator,
								  float &fX, float &fY, float &fH,DWORD dwFrame)
{
	itLocator it = m_Locators.find(dwLocator);
	if(it == m_Locators.end())	return FALSE;

	GameModel::Locator *pLocator = (*it).second;
	if(pLocator && pLocator->GetLoadState() == Load_Did)
	{
		if (dwFrame == -1)
		{
			ProcessLocator(pAnimInfo,pLocator);
			model::Buffer *pBuffer = pLocator->GetBuffer();
			D3DXVECTOR3 *pPosBuffer = pBuffer->GetPosBuffer();
			fX = pPosBuffer[0].z;
			fY = pPosBuffer[0].x;
			fH = pPosBuffer[0].y;
		}
		else
		{
			ProcessLocatorByFrame(pAnimInfo,pLocator,dwFrame);
			model::Buffer *pBuffer = pLocator->GetBuffer();
			D3DXVECTOR3 *pPosBuffer = pBuffer->GetPosBuffer();
			fX = pPosBuffer[0].z;
			fY = pPosBuffer[0].x;
			fH = pPosBuffer[0].y;
		}


	}
	return TRUE;
}

//��ʾģ������
void CDisplayModel::Display(AnimInfo *pAnimInfo)
{
	ProcessAnimJoint(pAnimInfo);
	ProcessVisibility(pAnimInfo);
	ProcessAnimUV(pAnimInfo);
	ProcessEnvironmentMaps(pAnimInfo->GetViewMatrix());
	ProcessAnimTexture(pAnimInfo);
	ProcessAnimColor(pAnimInfo);
	ProcessLocator(pAnimInfo);
	ProcessParticleEmitter(pAnimInfo);
	ProcessTrailEmitter(pAnimInfo);
	ProcessTextureProjective(pAnimInfo);
	RenderModel();
}
//��ʾģ�Ͱ󶨵�����ģ�͵İ󶨵�
void CDisplayModel::GetAnimInfobyLocator(AnimInfo *pOwnAnimInfo,CDisplayModel *pParentModel,AnimInfo *pParentAnimInfo,DWORD parentlocatorname)
{
	if(NULL == pParentModel)return;
	GameModel::Locator * pLocator = pParentModel->GetLocator(parentlocatorname);
	if(NULL == pLocator)	return;
	const D3DXMATRIX*  pmatBillboard = pParentAnimInfo->GetBillboard();
	const D3DXMATRIX*  pmatBillboardY = pParentAnimInfo->GetBillboardY();
	DWORD dwCurTime = pParentAnimInfo->GetCurrentTime();

	D3DXMATRIX matWorld,matJoint;
	model::Mesh *pMesh = pLocator->GetMesh();
	D3DXVECTOR3 *pPosBuf = pMesh->GetPosBuffer();
	matJoint = pLocator->GetNowWorldMatrix();
	D3DXMatrixTranslation(&matWorld,pPosBuf[0].x,pPosBuf[0].y,pPosBuf[0].z);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matJoint);

	pOwnAnimInfo->SetWorldMatrix(&matWorld);
	pOwnAnimInfo->SetViewMatrix(pParentAnimInfo->GetViewMatrix());
	pOwnAnimInfo->SetProjectedMatrix(pParentAnimInfo->GetProjectedMatrix());
	//pOwnAnimInfo->SetScalMatrix(pParentAnimInfo->GetScalMatrix());
	pOwnAnimInfo->SetBillboard(pmatBillboard);
	pOwnAnimInfo->SetBillboardY(pmatBillboardY);
	pOwnAnimInfo->SetCurrentTime(dwCurTime);
}


//���ģ���ļ���Shader�Ƿ��ȡ���
eLoadState CDisplayModel::CheckBaseModelLoadState()
{
	if(0 == m_dwGameModelID || m_dwShaderGroupHandle == 0)
	{
		return Load_Not;
	}
	if( m_pGameModelManager->GetGameModel(m_dwGameModelID)->GetLoadState() != Load_Did )
		return Load_Not;
	return Load_Did;
}

//�õ�װ�����(��Ҫ��Group)
eLoadState CDisplayModel::GetLoadState()
{
	if( m_eGroupLoadState != Load_Did)
	{
		CheckGroupLoadState();
	}
	return m_eGroupLoadState;
}

//�������Group��װ�����
void CDisplayModel::CheckGroupLoadState()
{
	m_eGroupLoadState = CheckBaseModelLoadState();
	if(m_eGroupLoadState != Load_Did)	return;

	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		GameModel::Group::Part::Level *pLevel = (*it).second->GetLevelInfo();
		if(NULL == pLevel)
		{
			m_eGroupLoadState = Load_Not;
			break;
		}
		GameModel::Group::Part::LevelMeshes& vRenderMeshes = pLevel->GetRMArray();
		for(int i = 0; i <pLevel->GetRMCount(); i++)
		{
			GameModel::Group::Part::tagRenderMesh *pRenderMesh = vRenderMeshes[i];
			m_eGroupLoadState = pRenderMesh->GetLoadState();
			if(m_eGroupLoadState != Load_Did)
			{
				break;
			}
			model::Mesh *pMesh = pRenderMesh->pMesh;
			/*if(NULL == pMesh)
			{
			m_eGroupLoadState = Load_Not;
			break;
			}*/
			render::Interface *pInterface = render::Interface::GetInstance();
			render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
			render::Interface::Layer3D::ShaderGroup *pShaderGroup =
				pLayer3D->GetShaderGroup(m_dwShaderGroupHandle);
			if(NULL == pShaderGroup) continue;
			render::Interface::Layer3D::ShaderGroup::_tagShader *pShader =
				pShaderGroup->GetShader(pMesh->GetShaderIndex());
			if(NULL == pShader)	continue;
			DWORD dwTexture0 = pShaderGroup->GetTextureHandle(0,pShader->tLayer[0].dwTexture);
			//DWORD dwTexture1 = pShaderGroup->GetTextureHandle(1,pShader->tLayer[1].dwTexture);

			render::Interface::tagTextureInfo* pTextureInfo0 = pInterface->GetTextureInfo(dwTexture0);
			if(NULL == pTextureInfo0) continue;
			//render::Interface::tagTextureInfo* pTextureInfo1 = pInterface->GetTextureInfo(dwTexture1);
			//����0������
			m_eGroupLoadState = pTextureInfo0->GetLoadState();
			if(m_eGroupLoadState != Load_Did)
			{
				break;
			}
		}
		if(m_eGroupLoadState != Load_Did)
			break;
	}
}
//������ӵ�װ�����
void CDisplayModel::CheckParticleLoadState()
{
	m_eParticleLoadState = CheckBaseModelLoadState();
	if(m_eParticleLoadState != Load_Did)	return;

	itParticleEmitter itEmitter = m_ParticleEmitters.begin();
	for(; itEmitter != m_ParticleEmitters.end();itEmitter++)
	{
		GameModel::ParticleEmitter *pParticleEmitter = itEmitter->second;
		if ( !IsParticleEnable(pParticleEmitter->GetName()) ) continue;

		m_eParticleLoadState = pParticleEmitter->GetLoadState();
		if(m_eParticleLoadState != Load_Did)
			break;
		particle::Emitter *pEmitter = pParticleEmitter->GetParticleEmitter();
		m_eParticleLoadState = pEmitter->GetLoadState();
		if(m_eParticleLoadState != Load_Did)
			break;

		render::Interface *pInterface = render::Interface::GetInstance();
		render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
		render::Interface::Layer3D::ShaderGroup *pShaderGroup =
			pLayer3D->GetShaderGroup(pEmitter->GetShaderGroupHandle());
		if(NULL == pShaderGroup) continue;
		render::Interface::Layer3D::ShaderGroup::_tagShader *pShader =
			pShaderGroup->GetShader(0);
		if(NULL == pShader)	continue;
		DWORD dwTexture0 = pShaderGroup->GetTextureHandle(0,pShader->tLayer[0].dwTexture);
		//DWORD dwTexture1 = pShaderGroup->GetTextureHandle(1,pShader->tLayer[1].dwTexture);

		render::Interface::tagTextureInfo* pTextureInfo0 = pInterface->GetTextureInfo(dwTexture0);
		if(NULL == pTextureInfo0) continue;
		//render::Interface::tagTextureInfo* pTextureInfo1 = pInterface->GetTextureInfo(dwTexture1);
		//����0������
		m_eParticleLoadState = pTextureInfo0->GetLoadState();
		if(m_eParticleLoadState != Load_Did)
		{
			break;
		}
	}
	return ;
}
bool CDisplayModel::CheckStatic()
{
	return false;
}

void CDisplayModel::Destroy(void)
{
	m_dwShaderGroupHandle = 0;

	itDisplayGroup it = m_DisplayGroups.begin();
	for(;it!=m_DisplayGroups.end();it++)
	{
		(*it).second->SetLevelInfo(NULL);
	}
	particle::Manager *pPM = particle::Manager::GetInstance();
	itParticleEmitter itp = m_ParticleEmitters.begin();
	for (;itp!=m_ParticleEmitters.end();itp++)
	{
		itp->second->GetParticleEmitterRef()->SetReadyToDie();
		//SAFEDELETE(itp->second->GetParticleEmitterRef());
		SAFEDELETE(itp->second);
	}


	m_Locators.clear();
	m_TrailEmitters.clear();
	m_ParticleEmitters.clear();
	m_TextureProjectives.clear();

	m_pBindPose = NULL;
	m_pPickBox = NULL;
	m_pSizeBox = NULL;
}