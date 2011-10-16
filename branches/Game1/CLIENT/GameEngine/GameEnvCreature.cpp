#include "StdAfx.h"

#include "..\frostengine\console.h"
#include "..\frostengine\utility.h"
#include "..\frostengine\render.h"

#include "gameenvcreature.h"
#include "gamegraphics2d.h"
#include "gamemap.h"
#include "GameModelManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
* ����: ������������
* ժҪ: -
* ����: szFileName	- �������������ļ�·����
* ����ֵ: �����õĻ����������ָ��
* ����: -
* ��������: -
* �޸���־:
*	2008.07.01 - lpf
*		���������ǵĴ���
*	2010.02.03 - lpf
*		������Ϣ�޸�ΪӢ��
*/
GameBaseEnvCreature * GameBaseEnvCreature::CreateEnvCreature(const char * szFileName)
{
	//��ȡ����
	CRFile * pfile = rfOpen(szFileName);
	if (!pfile)
	{
		MessageBox(NULL,szFileName,"Read file fail!",MB_OK);
		return NULL;
	}
	utility::Text  text;
	text.Create(pfile);
	rfClose(pfile);

	char szPath[1024];
	utility::File::GetFilePath(szFileName, szPath);

	GameBaseEnvCreature * p(NULL); 

	utility::Text::Node *pRoot = text.GetRootNode();

	//��ȡ����
	static const char * szEnvCreatureTypeString[] = { "butterfly", "crow", "beetle", "rabbit", "meteor" };
	switch(pRoot->GetVar("type")->GetEnumValue(szEnvCreatureTypeString, 5))
	{
	case 0:
		//����
		p = new GameEnvCreature_Butterfly;
		if (!p->Create(pRoot->GetChildNode("parameter"), szPath))
		{
			MessageBox(NULL, szPath, "Entironment creature [Bufferfly] create fail", MB_OK);
			SAFEDELETE(p);
		}
		break;
	case 1:
		p = new GameEnvCreature_Crow;
		if (!p->Create(pRoot->GetChildNode("parameter"), szPath))
		{
			MessageBox(NULL, szPath, "Entironment creature [Crow] create fail", MB_OK);
			SAFEDELETE(p);
		}
		break;
	case 2:
		p = new GameEnvCreature_Beetle;
		if (!p->Create(pRoot->GetChildNode("parameter"), szPath))
		{
			MessageBox(NULL, szPath, "Entironment creature [Beetle] create fail", MB_OK);
			SAFEDELETE(p);
		}
		break;
	case 3:
		p = new GameEnvCreature_Rabbit;
		if (!p->Create(pRoot->GetChildNode("parameter"), szPath))
		{
			MessageBox(NULL, szPath, "Entironment creature [Rabbit] create fail", MB_OK);
			SAFEDELETE(p);
		}
		break;
	case 4:		//����
		p = new GameEnvCreature_Meteor;
		if (!p->Create(pRoot->GetChildNode("parameter"), szPath))
		{
			MessageBox(NULL, szPath, "Entironment creature [Meteor] create fail", MB_OK);
			SAFEDELETE(p);
		}
		break;
	}

	text.Destroy();

	return p;
}



/*
* ����: ȡ�ø����������
* ժҪ: -
* ����: fStart	- ��ʼλ��
*		 fEnd	- ����λ��
* ����ֵ: -
* ����: lpf
* ��������: 2008.7.1
* �޸���־:
*/
FLOAT GameBaseEnvCreature::FieldRandom(FLOAT fStart, FLOAT fEnd)
{
	FLOAT fTemp = (FLOAT)rand() / (FLOAT)RAND_MAX;
	return (fStart + (fEnd - fStart) * fTemp);
}



/*
* ����: ȡ�����������
* ժҪ: -
* ����: fStart	- ��ʼλ��
*		 fEnd	- ����λ��
* ����ֵ: -
* ����: lpf
* ��������: 2008.7.1
* �޸���־:
*/
DWORD GameBaseEnvCreature::FieldRandom(DWORD dwStart, DWORD dwEnd)
{
	float fTemp = (float)rand() / (float)RAND_MAX;
	return (dwStart + (dwEnd - dwStart) * fTemp);
}



/*
* ����: ���๹�캯��
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.11.12
* �޸���־:
*/
GameBaseEnvCreature::GameBaseEnvCreature()
{
	m_pModel = NULL;
	m_fScale = 0.0f;
	m_fRadius = 0.0f;
	m_dwAlphaFadeTimeLength = 0;

	m_bIsDisplay = TRUE;
	m_bFollowPlayer = TRUE;
	m_vBirthPos.x = 0.0f;
	m_vBirthPos.y = 0.0f;
	m_vBirthPos.z = 0.0f;
}



GameEnvCreature_Butterfly::GameEnvCreature_Butterfly()
{
	m_dwLastTime = 0;
}

GameEnvCreature_Butterfly::~GameEnvCreature_Butterfly()
{
}



/*
* ����: ����������������
* ժҪ: -
* ����: pNode			- �����ļ��ڵ�
*		 szResourcePath	- ����������Դ·��
* ����ֵ: �ɹ�����true
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.11.13 - lpf
*		�����˻��������Զ���������������ö�ȡ
*	2008.12.05 - lpf
*		�����˻�������͸������ʱ�����ö�ȡ
*/
BOOL GameEnvCreature_Butterfly::Create(utility::Text::Node * pNode, char * szResourcePath)
{
	char szTemp[1024];

	_snprintf(szTemp, 1024, "%s\\%s", szResourcePath, pNode->GetVar("model")->GetStringValue());

	m_pModel = CGameModelManager::GetInstance()->CreateDisplayModel(szTemp);
	VERIFY(m_pModel);
	m_pModel->ShowAllGroup();
	m_pModel->EnableParticleEmitter(TRUE);
	m_pModel->EnableTrailEmitter(TRUE);

	m_fScale  = pNode->GetVar("scale")->GetFloatValue();
	m_fRadius = pNode->GetVar("radius")->GetFloatValue();
	m_dwAlphaFadeTimeLength = (pNode->GetVar("AlphaFadeTimeLength") == NULL) ? 0 : (DWORD)pNode->GetVar("AlphaFadeTimeLength")->GetIntValue();

	if (pNode->GetVar("FollowPlayer") != NULL)
	{
		m_bFollowPlayer = pNode->GetVar("FollowPlayer")->GetBOOLValue();
		if (m_bFollowPlayer == FALSE)
		{
			utility::Text::Node * pNodeBirthPos = pNode->GetChildNode("BirthPos");
			if (pNodeBirthPos != NULL)
			{
				m_vBirthPos.x = pNodeBirthPos->GetVar("x")->GetFloatValue();
				m_vBirthPos.y = pNodeBirthPos->GetVar("y")->GetFloatValue();
				m_vBirthPos.z = pNodeBirthPos->GetVar("z")->GetFloatValue();
			}
		}
		m_RegionBox.Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_fRadius * 2.0f);
	}

	m_dwMinLifeTime = (DWORD)pNode->GetVar("MinLifeTime")->GetIntValue();
	m_dwMaxLifeTime = (DWORD)pNode->GetVar("MaxlifeTime")->GetIntValue();

	m_fMinSpeed = pNode->GetVar("MinSpeed")->GetFloatValue();
	m_fMaxSpeed = pNode->GetVar("MaxSpeed")->GetFloatValue();
	m_fMinAngle = pNode->GetVar("MinAngle")->GetFloatValue();
	m_fMaxAngle = pNode->GetVar("MaxAngle")->GetFloatValue();

	// ��ȡ��������
	m_dwButterflyCount = (DWORD)pNode->GetVar("number")->GetIntValue();
	m_pButterflyArray  = new tagButterfly[m_dwButterflyCount];
	ZeroMemory(m_pButterflyArray, sizeof(tagButterfly) * m_dwButterflyCount);
	DWORD dwCurrentTime = timeGetTime();
	for (DWORD n = 0; n < m_dwButterflyCount; ++n)
	{
		if (m_dwAlphaFadeTimeLength > 0)
		{
			m_pButterflyArray[n].iAlphaMode			  = GameBaseEnvCreature::EC_Fade_In;
			m_pButterflyArray[n].dwAlphaFadeStartTime = dwCurrentTime;
		}

		m_pButterflyArray[n].pAnimInfo = new AnimInfo();
	}

	return TRUE;
}



/*
* ����: ���ٺ�����Դ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.02.19 - lpf
*		�޸�����ʾģ���ͷ�˳��
*/
void GameEnvCreature_Butterfly::Destroy()
{
	// ����
	CGameModelManager::GetInstance()->ReleaseDisplayModel(m_pModel);
	m_pModel = NULL;

	for(DWORD n = 0; n < m_dwButterflyCount;n++)
		SAFEDELETE(m_pButterflyArray[n].pAnimInfo);

	SAFEDELETEARRAY(m_pButterflyArray);
}



/*
* ����: �˶�û�к�ʱ�����---�޸�
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.11.13 - lpf
*		�����˻���������޳�,���Զ�������㴦��
*	2008.12.05 - lpf
*		�����˻�������͸������Ĵ���
*/
void GameEnvCreature_Butterfly::Update(const D3DXVECTOR3 *pPosition, render::Frustum * pFrustum)
{
	if (m_bFollowPlayer == FALSE)
	{
		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, m_vBirthPos.x, m_vBirthPos.y, m_vBirthPos.z);
		m_bIsDisplay = pFrustum->BoxIntersectFrustum(&mWorld, &m_RegionBox);
		if (m_bIsDisplay == FALSE)
			return;
	}

	DWORD dwCurrentTime = timeGetTime();
	DWORD dwTimeElapse = dwCurrentTime - m_dwLastTime;
	m_dwLastTime = dwCurrentTime;

	//ˢ��
	for(DWORD n = 0; n < m_dwButterflyCount;n++)
	{
		tagButterfly &tButterfly = m_pButterflyArray[n];

		if (tButterfly.dwLifeTime == 0)
		{
			if (tButterfly.iAlphaMode == GameBaseEnvCreature::EC_Fade_Out)
			{
				tButterfly.fCurrentAlphaValue = (float)((int)m_dwAlphaFadeTimeLength - ((int)dwCurrentTime - (int)tButterfly.dwAlphaFadeStartTime)) / (float)m_dwAlphaFadeTimeLength;

				if (tButterfly.fCurrentAlphaValue < 0.0f)
				{
					tButterfly.fCurrentAlphaValue = 0.0f;
					tButterfly.iAlphaMode = GameBaseEnvCreature::EC_Fade_In;
				}

				AI(tButterfly, dwTimeElapse);
			}else
			{
				if (m_bFollowPlayer == TRUE)
				{
					tButterfly.vPosition.x = pPosition->x + m_fRadius * 2 * (float)rand() / (float)RAND_MAX - m_fRadius;
					tButterfly.vPosition.y = pPosition->y + m_fRadius * 2 * (float)rand() / (float)RAND_MAX - m_fRadius;
					tButterfly.vPosition.z = pPosition->z + m_fRadius * 2 * (float)rand() / (float)RAND_MAX - m_fRadius;
				}else
				{
					tButterfly.vPosition.x = m_vBirthPos.x + m_fRadius * 2 * (float)rand() / (float)RAND_MAX - m_fRadius;
					tButterfly.vPosition.y = m_vBirthPos.y + m_fRadius * 2 * (float)rand() / (float)RAND_MAX - m_fRadius;
					tButterfly.vPosition.z = m_vBirthPos.z + m_fRadius * 2 * (float)rand() / (float)RAND_MAX - m_fRadius;
				}

				tButterfly.dwLifeTime = m_dwMinLifeTime + rand() % (m_dwMaxLifeTime - m_dwMinLifeTime);
				tButterfly.dwAlphaFadeStartTime = dwCurrentTime;

				float fRadomSpeed = m_fMaxSpeed - m_fMinSpeed;
				tButterfly.fSpeed = m_fMinSpeed + fRadomSpeed * (float)rand() / (float)RAND_MAX - fRadomSpeed / 2;
			}
		}else
		{
			if (tButterfly.iAlphaMode == GameBaseEnvCreature::EC_Fade_In)
			{
				tButterfly.fCurrentAlphaValue = (float)(dwCurrentTime - tButterfly.dwAlphaFadeStartTime) / (float)m_dwAlphaFadeTimeLength;

				if (tButterfly.fCurrentAlphaValue > 1.0f)
				{
					tButterfly.fCurrentAlphaValue = 1.0f;
					tButterfly.iAlphaMode = GameBaseEnvCreature::EC_Fade_None;
				}
			}else
			{
				if (tButterfly.dwLifeTime < dwTimeElapse)
				{
					tButterfly.dwLifeTime = 0;
					tButterfly.iAlphaMode = GameBaseEnvCreature::EC_Fade_Out;
					tButterfly.dwAlphaFadeStartTime = dwCurrentTime;
				}else
					tButterfly.dwLifeTime = tButterfly.dwLifeTime - dwTimeElapse;
			}

			AI(tButterfly, dwTimeElapse);
		}		
	}	
}



/*
* ����: ����AI
* ժҪ: ��Ҫ�����ڻ���������״̬�µ������ꡢ�����ٶȵȵȵĸ���
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.12.05
* �޸���־:
*/
void GameEnvCreature_Butterfly::AI(tagButterfly & tButterfly, DWORD dwTimeElapse)
{
	float fAngle = m_fMaxAngle - m_fMinAngle;
	tButterfly.fRotationY += D3DXToRadian(m_fMinAngle + fAngle * (float)rand() / (float)RAND_MAX - fAngle/2);
	float fRatio = (float)dwTimeElapse * 0.001f;
	D3DXVECTOR3 vStep(0.0f,0.0f,tButterfly.fSpeed * fRatio);
	D3DXMATRIX mRot;
	D3DXMatrixRotationY(&mRot,tButterfly.fRotationY);
	D3DXVec3TransformNormal(&vStep,&vStep,&mRot);
	tButterfly.vPosition += vStep;
}



/*
* ����: ��Ⱦ����
* ժҪ: -
* ����: pMap	 - ��ǰʹ�õ�ͼ����ָ��
*		 pCamera - ���������ָ��
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.08.13 - lpf
*		�����˶�ģ���Ƿ���ȫ������ж�
*	2008.11.13 - lpf
*		�����˻���������޳���ʾ
*	2008.12.05 - lpf
*		�����˻�������͸��������Ⱦ����
*/
void GameEnvCreature_Butterfly::Render(const GameMap *pMap,render::Camera *pCamera)
{
	if (m_pModel->GetLoadState() != Load_Did ||
		m_bIsDisplay == FALSE)
		return;

	static D3DXMATRIX  matWorld,matTrans,matScale,matRotY;

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
	DWORD dwCurTime = timeGetTime();
	//��Ⱦ
	for(DWORD n = 0; n < m_dwButterflyCount;n++)
	{
		tagButterfly * pButterfly = &m_pButterflyArray[n];

		D3DXMatrixScaling(&matScale,m_fScale,m_fScale,m_fScale);
		D3DXMatrixRotationY(&matRotY,pButterfly->fRotationY);
		D3DXMatrixTranslation(&matTrans,pButterfly->vPosition.x,pButterfly->vPosition.y,pButterfly->vPosition.z);
		D3DXMatrixMultiply(&matWorld,&matScale,&matRotY);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matTrans);

		pButterfly->pAnimInfo->SetBillboard(pCamera->GetBillboard());
		pButterfly->pAnimInfo->SetBillboardY(pCamera->GetBillboardY());
		pButterfly->pAnimInfo->SetWorldMatrix(&matWorld);
		pButterfly->pAnimInfo->SetViewMatrix(pCamera->GetViewMatrix());
		pButterfly->pAnimInfo->SetProjectedMatrix(pCamera->GetProjectionMatrix());
		pButterfly->pAnimInfo->SetCurrentTime(dwCurTime);
		pButterfly->pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('0','0','0','0');
		pButterfly->pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;

		m_pModel->SetDirLightEnable(FALSE);
		m_pModel->ProcessAnimJoint(pButterfly->pAnimInfo);
		m_pModel->ProcessAnimTexture(pButterfly->pAnimInfo);
		m_pModel->ProcessAnimUV(pButterfly->pAnimInfo);
		m_pModel->ProcessParticleEmitter(pButterfly->pAnimInfo);
		m_pModel->ProcessTrailEmitter(pButterfly->pAnimInfo);
		m_pModel->ProcessLocator(pButterfly->pAnimInfo);
		m_pModel->SetModelColor(0xFFFFFFFF);
		m_pModel->ProcessAnimColor(pButterfly->pAnimInfo);
		m_pModel->ProcessVisibility(pButterfly->pAnimInfo);

		if (pButterfly->iAlphaMode != GameBaseEnvCreature::EC_Fade_None)
		{
			m_pModel->MultiplyModelAlpha(pButterfly->fCurrentAlphaValue);
			m_pModel->RenderModelTransparent();
		}else
			m_pModel->RenderModel();
	}
}



GameEnvCreature_Crow::GameEnvCreature_Crow()
{
	m_dwLastTime = 0;
}

GameEnvCreature_Crow::~GameEnvCreature_Crow()
{
}



/*
* ����: ������ѻ��������
* ժҪ: -
* ����: pNode			- �����ļ��ڵ�
*		 szResourcePath	- ����������Դ·��
* ����ֵ: �ɹ�����true
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.11.13 - lpf
*		�����˻��������Զ���������������ö�ȡ
*	2008.12.05 - lpf
*		�����˻�������͸������ʱ�����ö�ȡ
*/
BOOL GameEnvCreature_Crow::Create(utility::Text::Node * pNode, char * szResourcePath)
{
	char szTemp[1024];

	_snprintf(szTemp, 1024, "%s\\%s",szResourcePath, pNode->GetVar("model")->GetStringValue());

	m_pModel = CGameModelManager::GetInstance()->CreateDisplayModel(szTemp);
	VERIFY(m_pModel);
	m_pModel->ShowAllGroup();
	m_pModel->EnableParticleEmitter(TRUE);
	m_pModel->EnableTrailEmitter(TRUE);

	m_fScale   = pNode->GetVar("scale")->GetFloatValue();
	m_bLighted = pNode->GetVar("Lighted")->GetBOOLValue();
	m_fRadius  = pNode->GetVar("radius")->GetFloatValue();
	m_dwAlphaFadeTimeLength = (pNode->GetVar("AlphaFadeTimeLength") == NULL) ? 0 : (DWORD)pNode->GetVar("AlphaFadeTimeLength")->GetIntValue();

	if (pNode->GetVar("FollowPlayer") != NULL)
	{
		m_bFollowPlayer = pNode->GetVar("FollowPlayer")->GetBOOLValue();
		if (m_bFollowPlayer == FALSE)
		{
			utility::Text::Node * pNodeBirthPos = pNode->GetChildNode("BirthPos");
			if (pNodeBirthPos != NULL)
			{
				m_vBirthPos.x = pNodeBirthPos->GetVar("x")->GetFloatValue();
				m_vBirthPos.y = pNodeBirthPos->GetVar("y")->GetFloatValue();
				m_vBirthPos.z = pNodeBirthPos->GetVar("z")->GetFloatValue();
			}
		}
		m_RegionBox.Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_fRadius * 2.0f);
	}

	m_dwMinLifeTime = (DWORD)pNode->GetVar("MinLifeTime")->GetIntValue();
	m_dwMaxLifeTime = (DWORD)pNode->GetVar("MaxLifeTime")->GetIntValue();

	m_dwMinTurnForceTime = (DWORD)pNode->GetVar("MinTurnForceTime")->GetIntValue();		//���ת�����ȱ仯����
	m_dwMaxTurnForceTime = (DWORD)pNode->GetVar("MaxTurnForceTime")->GetIntValue();		//���ת�����ȱ仯����
	m_fMinTurnForce		 = (float)pNode->GetVar("MinTurnForce")->GetIntValue();			//ת������
	m_fMaxTurnForce		 = (float)pNode->GetVar("MaxTurnForce")->GetIntValue();

	m_fMinRadomSpeed = pNode->GetVar("MinRadomSpeed")->GetFloatValue();
	m_fMaxRadomSpeed = pNode->GetVar("MaxRadomSpeed")->GetFloatValue();

	m_dwMinTurnDirectionTime = (DWORD)pNode->GetVar("MinTurnDirectionTime")->GetIntValue();
	m_dwMaxTurnDirectionTime = (DWORD)pNode->GetVar("MaxTurnDirectionTime")->GetIntValue();

	// ��ȡ��������
	m_dwCrowCount = (DWORD)pNode->GetVar("number")->GetIntValue();
	m_pCrowArray  = new tagCrow[m_dwCrowCount];
	ZeroMemory(m_pCrowArray, sizeof(tagCrow) * m_dwCrowCount);	
	DWORD dwCurrentTime = timeGetTime();
	for (DWORD n = 0; n < m_dwCrowCount; ++n)
	{
		if (m_dwAlphaFadeTimeLength > 0)
		{
			m_pCrowArray[n].iAlphaMode			 = GameBaseEnvCreature::EC_Fade_In;
			m_pCrowArray[n].dwAlphaFadeStartTime = dwCurrentTime;
		}

		m_pCrowArray[n].pAnimInfo = new AnimInfo();
	}

	return TRUE;
}



/*
* ����: ������ѻ��Դ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.02.19 - lpf
*		�޸�����ʾģ���ͷ�˳��
*/
void GameEnvCreature_Crow::Destroy()
{
	// ����
	CGameModelManager::GetInstance()->ReleaseDisplayModel(m_pModel);
	m_pModel = NULL;

	for(DWORD n = 0; n < m_dwCrowCount; ++n)
		SAFEDELETE(m_pCrowArray[n].pAnimInfo);

	SAFEDELETEARRAY(m_pCrowArray);
}



/*
* ����: ��ѻ����
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.11.13 - lpf
*		�����˻���������޳�,���Զ�������㴦��
*	2008.12.05 - lpf
*		�����˻�������͸������Ĵ���
*/
void GameEnvCreature_Crow::Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum)
{
	if (m_bFollowPlayer == FALSE)
	{
		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, m_vBirthPos.x, m_vBirthPos.y, m_vBirthPos.z);
		m_bIsDisplay = pFrustum->BoxIntersectFrustum(&mWorld, &m_RegionBox);
		if (m_bIsDisplay == FALSE)
			return;
	}

	DWORD dwCurrentTime = timeGetTime();
	DWORD dwTimeElapse = dwCurrentTime - m_dwLastTime;
	m_dwLastTime = dwCurrentTime;

	// ˢ��
	for(DWORD n = 0; n < m_dwCrowCount;n++)
	{
		tagCrow * pCrow = &m_pCrowArray[n];

		if (pCrow->dwLifeTime == 0)
		{
			if (pCrow->iAlphaMode == GameBaseEnvCreature::EC_Fade_Out)
			{
				pCrow->fCurrentAlphaValue = (float)((int)m_dwAlphaFadeTimeLength - ((int)dwCurrentTime - (int)pCrow->dwAlphaFadeStartTime)) / (float)m_dwAlphaFadeTimeLength;

				if (pCrow->fCurrentAlphaValue < 0.0f)
				{
					pCrow->fCurrentAlphaValue = 0.0f;
					pCrow->iAlphaMode = GameBaseEnvCreature::EC_Fade_In;
				}

				AI(pPosition, pCrow, dwCurrentTime, dwTimeElapse);
			}else
			{
				if (m_bFollowPlayer == TRUE)
				{
					pCrow->vPosition.x = pPosition->x + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
					pCrow->vPosition.y = pPosition->y + m_fRadius * (float)rand() / (float)RAND_MAX;
					pCrow->vPosition.z = pPosition->z + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
				}else
				{
					pCrow->vPosition.x = m_vBirthPos.x + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
					pCrow->vPosition.y = m_vBirthPos.y + m_fRadius * (float)rand() / (float)RAND_MAX;
					pCrow->vPosition.z = m_vBirthPos.z + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
				}

				pCrow->dwLifeTime = m_dwMinLifeTime + rand() % (m_dwMaxLifeTime - m_dwMinLifeTime);
				pCrow->dwAlphaFadeStartTime = dwCurrentTime;

				float fRadomSpeed = m_fMaxRadomSpeed - m_fMinRadomSpeed;
				pCrow->fSpeed = m_fMinRadomSpeed + fRadomSpeed * (float)rand() / (float)RAND_MAX - fRadomSpeed / 2;

				pCrow->fTurnForce = m_fMinTurnForce + (m_fMaxTurnForce - m_fMinTurnForce) * (float)rand() / (float)RAND_MAX;
			}
		}else
		{
			if (pCrow->iAlphaMode == GameBaseEnvCreature::EC_Fade_In)
			{
				pCrow->fCurrentAlphaValue = (float)(dwCurrentTime - pCrow->dwAlphaFadeStartTime) / (float)m_dwAlphaFadeTimeLength;

				if (pCrow->fCurrentAlphaValue > 1.0f)
				{
					pCrow->fCurrentAlphaValue = 1.0f;
					pCrow->iAlphaMode = GameBaseEnvCreature::EC_Fade_None;
				}
			}else
			{
				if (pCrow->dwLifeTime < dwTimeElapse)
				{
					pCrow->dwLifeTime = 0;
					pCrow->iAlphaMode = GameBaseEnvCreature::EC_Fade_Out;
					pCrow->dwAlphaFadeStartTime = dwCurrentTime;
				}else
					pCrow->dwLifeTime = pCrow->dwLifeTime - dwTimeElapse;
			}

			AI(pPosition, pCrow, dwCurrentTime, dwTimeElapse);
		}		
	}	
}



/*
* ����: ��ѻAI
* ժҪ: ��Ҫ�����ڻ���������״̬�µ������ꡢ�����ٶȵȵȵĸ���
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.12.05
* �޸���־:
*/
void GameEnvCreature_Crow::AI(const D3DXVECTOR3 * pPosition, tagCrow * pCrow, DWORD dwCurrentTime, DWORD dwTimeElapse)
{
	if (dwCurrentTime - pCrow->dwLastTurnDirectionTime > m_dwMinTurnDirectionTime + rand() % (m_dwMaxTurnDirectionTime - m_dwMinTurnDirectionTime))
	{
		pCrow->dwLastTurnDirectionTime = dwCurrentTime;

		//��ǰ��������ļн�
		float fDirection = CalculateDirection(pPosition,&pCrow->vPosition);

		//��ǰ��������ļн�
		int iAngleDest   = (int)D3DXToDegree(fDirection);
		iAngleDest = iAngleDest % 360;
		if (iAngleDest < 0) 
			iAngleDest += 360;

		int iAngleSrouce = (int)D3DXToDegree(pCrow->fRotationY);
		iAngleSrouce = iAngleSrouce % 360;
		if (iAngleSrouce < 0) 
			iAngleSrouce += 360;

		//�����ֵ
		int iDisA = iAngleDest - iAngleSrouce;
		int iDisB = (iAngleDest + 360) - iAngleSrouce;
		if ( iDisA != 0 )
		{
			iDisA = iDisA / abs(iDisA);
			float fSign = (float)(iDisA / abs(iDisA));
			pCrow->fTurnDirection = (abs(iDisA) < abs(iDisB))? fSign : -fSign;
		}
	}

	//ÿ��һ��������ڸı�ת������ֵ
	DWORD dwTurnTimeRange = m_dwMaxTurnForceTime - m_dwMinTurnForceTime;
	if (dwCurrentTime - pCrow->dwTurnForceLastTime < m_dwMinTurnForceTime + rand() % dwTurnTimeRange)
	{
		pCrow->dwTurnForceLastTime = dwCurrentTime;
		pCrow->fTurnForce = m_fMinTurnForce + (m_fMaxTurnForce - m_fMinTurnForce) * (float)rand() / (float)RAND_MAX;
	}

	pCrow->fRotationY += D3DXToRadian(pCrow->fTurnDirection * pCrow->fTurnForce);
	float fRatio = (float)dwTimeElapse * 0.001f;
	D3DXVECTOR3 vStep(0.0f,0.0f,pCrow->fSpeed * fRatio);
	D3DXMATRIX mRot;
	D3DXMatrixRotationY(&mRot,pCrow->fRotationY);
	D3DXVec3TransformNormal(&vStep,&vStep,&mRot);
	pCrow->vPosition += vStep;
}



/*
* ����: ��Ⱦ��ѻ
* ժҪ: -
* ����: pMap	 - ��ǰʹ�õ�ͼ����ָ��
*		 pCamera - ���������ָ��
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.08.13 - lpf
*		�����˶�ģ���Ƿ���ȫ������ж�
*	2008.10.16 - lpf
*		ȡ���˵��θ�����ɫ��ģ�͵�Ӱ��
*	2008.11.13 - lpf
*		�����˻���������޳���ʾ
*	2008.12.05 - lpf
*		�����˻�������͸��������Ⱦ����
*/
void GameEnvCreature_Crow::Render(const GameMap *pMap,render::Camera *pCamera)
{
	if (m_pModel->GetLoadState() != Load_Did ||
		m_bIsDisplay == FALSE)
		return;

	static D3DXMATRIX  matWorld,matTrans,matScale,matRotY;

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
	DWORD dwCurTime = timeGetTime();
	DWORD dwSunLightAmbient = pMap->GetSunLightAmbientColorforBuild();
	DWORD dwSunLightDiffuse = pMap->GetSunLightDiffuseColor();
	const D3DXVECTOR3 *pLightDirectiion = pMap->GetSunLightDirection();

	//��Ⱦ
	for(DWORD n = 0; n < m_dwCrowCount;n++)
	{
		tagCrow * pCrow = &m_pCrowArray[n];

		D3DXMatrixScaling(&matScale,m_fScale,m_fScale,m_fScale);
		D3DXMatrixRotationY(&matRotY,pCrow->fRotationY);
		D3DXMatrixTranslation(&matTrans,pCrow->vPosition.x,pCrow->vPosition.y,pCrow->vPosition.z);
		D3DXMatrixMultiply(&matWorld,&matScale,&matRotY);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matTrans);

		pCrow->pAnimInfo->SetBillboard(pCamera->GetBillboard());
		pCrow->pAnimInfo->SetBillboardY(pCamera->GetBillboardY());
		pCrow->pAnimInfo->SetWorldMatrix(&matWorld);
		pCrow->pAnimInfo->SetViewMatrix(pCamera->GetViewMatrix());
		pCrow->pAnimInfo->SetProjectedMatrix(pCamera->GetProjectionMatrix());
		pCrow->pAnimInfo->SetCurrentTime(dwCurTime);
		pCrow->pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('0','0','0','0');
		pCrow->pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;

		m_pModel->SetDirLightEnable(false);
		m_pModel->ProcessAnimJoint(pCrow->pAnimInfo);
		m_pModel->ProcessAnimTexture(pCrow->pAnimInfo);
		m_pModel->ProcessAnimUV(pCrow->pAnimInfo);
		m_pModel->ProcessParticleEmitter(pCrow->pAnimInfo);
		m_pModel->ProcessTrailEmitter(pCrow->pAnimInfo);
		m_pModel->ProcessLocator(pCrow->pAnimInfo);
		if (!m_bLighted)
			m_pModel->SetModelColor(0xFFFFFFFF);

		m_pModel->ProcessAnimColor(pCrow->pAnimInfo);
		m_pModel->ProcessVisibility(pCrow->pAnimInfo);

		if (pCrow->iAlphaMode != GameBaseEnvCreature::EC_Fade_None)
		{
			m_pModel->MultiplyModelAlpha(pCrow->fCurrentAlphaValue);
			m_pModel->RenderModelTransparent();
		}else
			m_pModel->RenderModel();
	}
}



float GameEnvCreature_Crow::CalculateDirection(const D3DXVECTOR3 *pDst,const D3DXVECTOR3 *pSrc)
{
	D3DXVECTOR3 dir = - (*pDst - *pSrc);
	D3DXVec3Normalize(&dir,&dir);
	float fDirection(0);
	if( dir.x != 0 )
	{
		float f =  atanf( dir.z/dir.x );
		if( dir.x < 0 )
			f += D3DX_PI;
		fDirection = - f -D3DX_PI/2;
	}
	else
		fDirection = - dir.z/fabsf(dir.z)*D3DX_PI/2-D3DX_PI/2 ;
	return fDirection;
}

GameEnvCreature_Beetle::GameEnvCreature_Beetle()
{
	m_dwLastTime = 0;
	m_dwLastCenterChangeTime = 0;
}

GameEnvCreature_Beetle::~GameEnvCreature_Beetle()
{
}



/*
* ����: �����׳滷������
* ժҪ: -
* ����: pNode			- �����ļ��ڵ�
*		 szResourcePath	- ����������Դ·��
* ����ֵ: �ɹ�����true
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.11.13 - lpf
*		�����˻��������Զ���������������ö�ȡ
*	2008.12.05 - lpf
*		�����˻�������͸������ʱ�����ö�ȡ
*/
BOOL GameEnvCreature_Beetle::Create(utility::Text::Node * pNode, char * szResourcePath)
{
	char szTemp[1024];

	_snprintf(szTemp, 1024, "%s\\%s", szResourcePath, pNode->GetVar("model")->GetStringValue());

	m_pModel = CGameModelManager::GetInstance()->CreateDisplayModel(szTemp);
	VERIFY(m_pModel);
	m_pModel->ShowAllGroup();
	m_pModel->EnableParticleEmitter(TRUE);
	m_pModel->EnableTrailEmitter(TRUE);

	m_fScale   = pNode->GetVar("scale")->GetFloatValue();
	m_bLighted = pNode->GetVar("Lighted")->GetBOOLValue();
	m_fRadius  = pNode->GetVar("radius")->GetFloatValue();
	m_dwAlphaFadeTimeLength = (pNode->GetVar("AlphaFadeTimeLength") == NULL) ? 0 : (DWORD)pNode->GetVar("AlphaFadeTimeLength")->GetIntValue();

	if (pNode->GetVar("FollowPlayer") != NULL)
	{
		m_bFollowPlayer = pNode->GetVar("FollowPlayer")->GetBOOLValue();
		if (m_bFollowPlayer == FALSE)
		{
			utility::Text::Node * pNodeBirthPos = pNode->GetChildNode("BirthPos");
			if (pNodeBirthPos != NULL)
			{
				m_vBirthPos.x = pNodeBirthPos->GetVar("x")->GetFloatValue();
				m_vBirthPos.y = pNodeBirthPos->GetVar("y")->GetFloatValue();
				m_vBirthPos.z = pNodeBirthPos->GetVar("z")->GetFloatValue();
			}
		}
		m_RegionBox.Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_fRadius * 3.0f);
	}

	m_dwMinLifeTime = (DWORD)pNode->GetVar("MinLifeTime")->GetIntValue();
	m_dwMaxLifeTime = (DWORD)pNode->GetVar("MaxLifeTime")->GetIntValue();

	m_dwMinTurnForceTime = (DWORD)pNode->GetVar("MinTurnForceTime")->GetIntValue();		//���ת�����ȱ仯����
	m_dwMaxTurnForceTime = (DWORD)pNode->GetVar("MaxTurnForceTime")->GetIntValue();		//���ת�����ȱ仯����
	m_fMinTurnForce		 = (float)pNode->GetVar("MinTurnForce")->GetIntValue();			//ת������
	m_fMaxTurnForce		 = (float)pNode->GetVar("MaxTurnForce")->GetIntValue();

	m_fMinRadomSpeed = pNode->GetVar("MinRadomSpeed")->GetFloatValue();
	m_fMaxRadomSpeed = pNode->GetVar("MaxRadomSpeed")->GetFloatValue();

	m_dwMinTurnDirectionTime = (DWORD)pNode->GetVar("MinTurnDirectionTime")->GetIntValue();
	m_dwMaxTurnDirectionTime = (DWORD)pNode->GetVar("MaxTurnDirectionTime")->GetIntValue();

	m_dwMinCenterChangeTime = (DWORD)pNode->GetVar("MinCenterChangeTime")->GetIntValue();
	m_dwMaxCenterChangeTime = (DWORD)pNode->GetVar("MaxCenterChangeTime")->GetIntValue();
	m_fRandomCenterRadius   = pNode->GetVar("RandomCenterRadius")->GetFloatValue();

	//��ȡ��������
	m_dwBeetleCount = (DWORD)pNode->GetVar("number")->GetIntValue();
	m_pBeetleArray  = new tagBeetle[m_dwBeetleCount];
	ZeroMemory(m_pBeetleArray, sizeof(tagBeetle) * m_dwBeetleCount);	
	DWORD dwCurrentTime = timeGetTime();
	for(DWORD n = 0; n < m_dwBeetleCount; ++n)
	{
		if (m_dwAlphaFadeTimeLength > 0)
		{
			m_pBeetleArray[n].iAlphaMode		   = GameBaseEnvCreature::EC_Fade_In;
			m_pBeetleArray[n].dwAlphaFadeStartTime = dwCurrentTime;
		}

		m_pBeetleArray[n].pAnimInfo = new AnimInfo();
	}

	return TRUE;
}



/*
* ����: ���ټ׳���Դ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.02.19 - lpf
*		�޸�����ʾģ���ͷ�˳��
*/
void GameEnvCreature_Beetle::Destroy()
{
	// ����
	CGameModelManager::GetInstance()->ReleaseDisplayModel(m_pModel);
	m_pModel = NULL;

	for(DWORD n = 0; n < m_dwBeetleCount; ++n)
		SAFEDELETE(m_pBeetleArray[n].pAnimInfo);

	SAFEDELETEARRAY(m_pBeetleArray);
}



/*
* ����: �׳����
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.11.13 - lpf
*		�����˻���������޳�,���Զ�������㴦��
*	2008.12.05 - lpf
*		�����˻�������͸������Ĵ���
*/
void GameEnvCreature_Beetle::Update(const D3DXVECTOR3 *pPosition, render::Frustum * pFrustum)
{
	if (m_bFollowPlayer == FALSE)
	{
		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, m_vBirthPos.x, m_vBirthPos.y, m_vBirthPos.z);
		m_bIsDisplay = pFrustum->BoxIntersectFrustum(&mWorld, &m_RegionBox);
		if (m_bIsDisplay == FALSE)
			return;
	}

	DWORD dwCurrentTime = timeGetTime();
	DWORD dwTimeElapse = dwCurrentTime - m_dwLastTime;
	m_dwLastTime = dwCurrentTime;

	if (dwCurrentTime - m_dwLastCenterChangeTime > m_dwMinCenterChangeTime + rand() % (m_dwMaxCenterChangeTime - m_dwMinCenterChangeTime))
	{
		m_dwLastCenterChangeTime = dwCurrentTime;
		m_vCenter.x = pPosition->x + (m_fRandomCenterRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRandomCenterRadius;
		m_vCenter.y = 0;
		m_vCenter.z = pPosition->z + (m_fRandomCenterRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRandomCenterRadius;
	}

	//ˢ��
	for(DWORD n = 0; n < m_dwBeetleCount;n++)
	{
		tagBeetle * pBeetle = &m_pBeetleArray[n];

		if (pBeetle->dwLifeTime == 0)
		{
			if (pBeetle->iAlphaMode == GameBaseEnvCreature::EC_Fade_Out)
			{
				pBeetle->fCurrentAlphaValue = (float)((int)m_dwAlphaFadeTimeLength - ((int)dwCurrentTime - (int)pBeetle->dwAlphaFadeStartTime)) / (float)m_dwAlphaFadeTimeLength;

				if (pBeetle->fCurrentAlphaValue < 0.0f)
				{
					pBeetle->fCurrentAlphaValue = 0.0f;
					pBeetle->iAlphaMode = GameBaseEnvCreature::EC_Fade_In;
				}

				AI(pBeetle, dwCurrentTime, dwTimeElapse);
			}else
			{
				if (m_bFollowPlayer == TRUE)
				{
					pBeetle->vPosition.x = pPosition->x + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
					pBeetle->vPosition.z = pPosition->z + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
				}else
				{
					pBeetle->vPosition.x = m_vBirthPos.x + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
					pBeetle->vPosition.z = m_vBirthPos.z + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
				}

				pBeetle->dwLifeTime = m_dwMinLifeTime + rand() % ( m_dwMaxLifeTime - m_dwMinLifeTime );
				pBeetle->dwAlphaFadeStartTime = dwCurrentTime;

				float fRadomSpeed = m_fMaxRadomSpeed - m_fMinRadomSpeed;
				pBeetle->fSpeed = m_fMinRadomSpeed + fRadomSpeed * (float)rand() / (float)RAND_MAX - fRadomSpeed / 2;
				//
				float fDirection = CalculationDirection(&m_vCenter,&pBeetle->vPosition);
				int iAngleDest   = (int)D3DXToDegree(fDirection);
				iAngleDest = iAngleDest % 360;
				if (iAngleDest < 0) 
					iAngleDest += 360;

				int iAngleSrouce = (int)D3DXToDegree(pBeetle->fRotationY);
				iAngleSrouce = iAngleSrouce % 360;
				if (iAngleSrouce < 0) 
					iAngleSrouce += 360;

				//�����ֵ
				int iDisA = iAngleDest - iAngleSrouce;
				int iDisB = (iAngleDest + 360) - iAngleSrouce;
				if (iDisA != 0)
				{
					float fSign = (float)(iDisA / abs(iDisA));
					pBeetle->fTurnDirection = (float)( ( abs(iDisA) < abs(iDisB) )? fSign : -fSign);
				}
				//
				pBeetle->fTurnForce = m_fMinTurnForce + (m_fMaxTurnForce - m_fMinTurnForce) * (float)rand() / (float)RAND_MAX;

				pBeetle->pAnimInfo->ClearTrailContainer();
			}
		}else
		{
			if (pBeetle->iAlphaMode == GameBaseEnvCreature::EC_Fade_In)
			{
				pBeetle->fCurrentAlphaValue = (float)(dwCurrentTime - pBeetle->dwAlphaFadeStartTime) / (float)m_dwAlphaFadeTimeLength;

				if (pBeetle->fCurrentAlphaValue > 1.0f)
				{
					pBeetle->fCurrentAlphaValue = 1.0f;
					pBeetle->iAlphaMode = GameBaseEnvCreature::EC_Fade_None;
				}
			}else
			{
				if (pBeetle->dwLifeTime < dwTimeElapse)
				{
					pBeetle->dwLifeTime = 0;
					pBeetle->iAlphaMode = GameBaseEnvCreature::EC_Fade_Out;
					pBeetle->dwAlphaFadeStartTime = dwCurrentTime;
				}else
					pBeetle->dwLifeTime = pBeetle->dwLifeTime - dwTimeElapse;
			}

			AI(pBeetle, dwCurrentTime, dwTimeElapse);
		}
	}	
}



/*
* ����: �׳�AI
* ժҪ: ��Ҫ�����ڻ���������״̬�µ������ꡢ�����ٶȵȵȵĸ���
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.12.05
* �޸���־:
*/
void GameEnvCreature_Beetle::AI(tagBeetle * pBeetle, DWORD dwCurrentTime, DWORD dwTimeElapse)
{
	DWORD dwTurnDirectionRange = m_dwMaxTurnDirectionTime - m_dwMinTurnDirectionTime;
	if (dwCurrentTime - pBeetle->dwTurnDirectionLastTime > m_dwMinTurnDirectionTime + rand() % dwTurnDirectionRange)
	{
		pBeetle->dwTurnDirectionLastTime = dwCurrentTime;
		//��ǰ��������ļн�
		float fDirection = CalculationDirection(&m_vCenter,&pBeetle->vPosition);
		int iAngleDest   = (int)D3DXToDegree(fDirection);
		iAngleDest = iAngleDest % 360;
		if (iAngleDest < 0) 
			iAngleDest += 360;

		int iAngleSrouce = (int)D3DXToDegree(pBeetle->fRotationY);
		iAngleSrouce = iAngleSrouce % 360;
		if (iAngleSrouce < 0) 
			iAngleSrouce += 360;

		//�����ֵ
		int iDisA = iAngleDest - iAngleSrouce;
		int iDisB = (iAngleDest + 360) - iAngleSrouce;
		if (iDisA != 0)
		{
			float fSign = (float)(iDisA / abs(iDisA));
			pBeetle->fTurnDirection = (float)( ( abs(iDisA) < abs(iDisB) )? fSign : -fSign);
		}
	}

	//ÿ��һ��������ڸı�ת������ֵ
	DWORD dwTurnTimeRange = m_dwMaxTurnForceTime - m_dwMinTurnForceTime;
	if (dwCurrentTime - pBeetle->dwTurnForceLastTime > m_dwMinTurnForceTime + rand() % dwTurnTimeRange)
	{
		pBeetle->dwTurnForceLastTime = dwCurrentTime;
		pBeetle->fTurnForce = m_fMinTurnForce + (m_fMaxTurnForce - m_fMinTurnForce) * (float)rand() / (float)RAND_MAX;
	}

	pBeetle->fRotationY += D3DXToRadian(pBeetle->fTurnDirection * pBeetle->fTurnForce);

	float fRatio = (float)dwTimeElapse * 0.001f;
	D3DXVECTOR3 vStep(0.0f,0.0f,pBeetle->fSpeed * fRatio);
	D3DXMATRIX mRot;
	D3DXMatrixRotationY(&mRot,pBeetle->fRotationY);
	D3DXVec3TransformNormal(&vStep,&vStep,&mRot);
	pBeetle->vPosition += vStep;
}



/*
* ����: ��Ⱦ�׳�
* ժҪ: -
* ����: pMap	 - ��ǰʹ�õ�ͼ����ָ��
*		 pCamera - ���������ָ��
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.08.13 - lpf
*		�����˶�ģ���Ƿ���ȫ������ж�
*	2008.11.13 - lpf
*		�����˻���������޳���ʾ
*	2008.12.05 - lpf
*		�����˻�������͸��������Ⱦ����
*/
void GameEnvCreature_Beetle::Render(const GameMap *pMap,render::Camera *pCamera)
{
	if (m_pModel->GetLoadState() != Load_Did ||
		m_bIsDisplay == FALSE)
		return;

	static D3DXMATRIX matWorld,matTrans,matScale,matRotY;

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
	DWORD dwCurTime = timeGetTime();
	DWORD dwSunLightAmbient = pMap->GetSunLightAmbientColorforBuild();
	DWORD dwSunLightDiffuse = pMap->GetSunLightDiffuseColor();
	const D3DXVECTOR3 *pLightDirectiion = pMap->GetSunLightDirection();

	//��Ⱦ
	for(DWORD n = 0; n < m_dwBeetleCount;n++)
	{
		tagBeetle * pBeetle = &m_pBeetleArray[n];

		D3DXMatrixScaling(&matScale,m_fScale,m_fScale,m_fScale);
		D3DXMatrixRotationY(&matRotY,pBeetle->fRotationY);
		D3DXMatrixTranslation(&matTrans,pBeetle->vPosition.x,pMap->CalculateHeight(pBeetle->vPosition.x,pBeetle->vPosition.z),pBeetle->vPosition.z);
		D3DXMatrixMultiply(&matWorld,&matScale,&matRotY);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matTrans);

		pBeetle->pAnimInfo->SetBillboard(pCamera->GetBillboard());
		pBeetle->pAnimInfo->SetBillboardY(pCamera->GetBillboardY());
		pBeetle->pAnimInfo->SetWorldMatrix(&matWorld);
		pBeetle->pAnimInfo->SetViewMatrix(pCamera->GetViewMatrix());
		pBeetle->pAnimInfo->SetProjectedMatrix(pCamera->GetProjectionMatrix());
		pBeetle->pAnimInfo->SetCurrentTime(dwCurTime);
		pBeetle->pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('0','0','0','0');
		pBeetle->pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;

		m_pModel->ProcessAnimJoint(pBeetle->pAnimInfo);
		m_pModel->ProcessAnimTexture(pBeetle->pAnimInfo);
		m_pModel->ProcessAnimUV(pBeetle->pAnimInfo);
		m_pModel->ProcessParticleEmitter(pBeetle->pAnimInfo);
		m_pModel->ProcessTrailEmitter(pBeetle->pAnimInfo);
		m_pModel->ProcessLocator(pBeetle->pAnimInfo);

		m_pModel->SetDirLightEnable(FALSE);
		if (m_bLighted)
		{
			if (!((int)pBeetle->vPosition.x <0 || (int)pBeetle->vPosition.z < 0 || (int)pBeetle->vPosition.x >= (int)pMap->GetWidth() ||
				(int)pBeetle->vPosition.z >= (int)pMap->GetDepth()))
			{
				DWORD dwColor = pMap->GetGridColor((int)pBeetle->vPosition.x,(int)pBeetle->vPosition.z);
				//��ɫ����1.25��
				DWORD r = ( (dwColor & 0x00FF0000) >> 16) * 5 / 4;
				if (r > 255 )
					r = 255;
				DWORD g = ( (dwColor & 0x0000FF00) >>  8) * 5 / 4;
				if (g > 255) 
					g = 255;
				DWORD b = (  dwColor & 0x000000FF) * 5 / 4;
				if (b > 255)
					b = 255;
				dwColor = r << 16 | g << 8 | b;
				m_pModel->MultiplyModelColor(dwColor);
			}
		}
		else
		{
			m_pModel->SetModelColor(0xFFFFFFFF);
		}
		m_pModel->ProcessAnimColor(pBeetle->pAnimInfo);
		m_pModel->ProcessVisibility(pBeetle->pAnimInfo);

		if (pBeetle->iAlphaMode != GameBaseEnvCreature::EC_Fade_None)
		{
			m_pModel->MultiplyModelAlpha(pBeetle->fCurrentAlphaValue);
			m_pModel->RenderModelTransparent();
		}else
			m_pModel->RenderModel();
	}
}


float GameEnvCreature_Beetle::CalculationDirection(const D3DXVECTOR3 *pDest,const D3DXVECTOR3 *pSrc)
{
	//��ǰ��������ļн�
	D3DXVECTOR3 dir = - (*pDest - *pSrc);
	D3DXVec3Normalize(&dir,&dir);
	float fDirection(0);

	if( dir.x != 0 )
	{
		float f =  atanf( dir.z/dir.x );
		if( dir.x < 0 )
			f += D3DX_PI;
		fDirection = - f -D3DX_PI/2;
	}
	else
		fDirection = - dir.z/fabsf(dir.z)*D3DX_PI/2-D3DX_PI/2 ;
	return fDirection;
}


GameEnvCreature_Rabbit::GameEnvCreature_Rabbit()
{
	m_dwLastTime = 0;
}

GameEnvCreature_Rabbit::~GameEnvCreature_Rabbit()
{
}



/*
* ����: �������ӻ�������
* ժҪ: -
* ����: pNode			- �����ļ��ڵ�
*		 szResourcePath	- ����������Դ·��
* ����ֵ: �ɹ�����true
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.11.13 - lpf
*		�����˻��������Զ���������������ö�ȡ
*	2008.12.05 - lpf
*		�����˻�������͸������ʱ�����ö�ȡ
*/
BOOL GameEnvCreature_Rabbit::Create(utility::Text::Node *pNode,char *szResourcePath)
{
	char szTemp[1024];

	_snprintf(szTemp, 1024, "%s\\%s", szResourcePath, pNode->GetVar("model")->GetStringValue());

	m_pModel = CGameModelManager::GetInstance()->CreateDisplayModel(szTemp);
	VERIFY(m_pModel);
	m_pModel->ShowAllGroup();
	m_pModel->EnableParticleEmitter(TRUE);
	m_pModel->EnableTrailEmitter(TRUE);

	m_fScale   = pNode->GetVar("scale")->GetFloatValue();
	m_bLighted = pNode->GetVar("Lighted")->GetBOOLValue();
	m_fRadius  = pNode->GetVar("radius")->GetFloatValue();
	m_dwAlphaFadeTimeLength = (pNode->GetVar("AlphaFadeTimeLength") == NULL) ? 0 : (DWORD)pNode->GetVar("AlphaFadeTimeLength")->GetIntValue();

	if (pNode->GetVar("FollowPlayer") != NULL)
	{
		m_bFollowPlayer = pNode->GetVar("FollowPlayer")->GetBOOLValue();
		if (m_bFollowPlayer == FALSE)
		{
			utility::Text::Node * pNodeBirthPos = pNode->GetChildNode("BirthPos");
			if (pNodeBirthPos != NULL)
			{
				m_vBirthPos.x = pNodeBirthPos->GetVar("x")->GetFloatValue();
				m_vBirthPos.y = pNodeBirthPos->GetVar("y")->GetFloatValue();
				m_vBirthPos.z = pNodeBirthPos->GetVar("z")->GetFloatValue();
			}
		}
		m_RegionBox.Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_fRadius * 2.0f);
	}

	m_dwMinLifeTime = (DWORD)pNode->GetVar("MinLifeTime")->GetIntValue();
	m_dwMaxLifeTime = (DWORD)pNode->GetVar("MaxLifeTime")->GetIntValue();

	m_dwMinTurnForceTime = (DWORD)pNode->GetVar("MinTurnForceTime")->GetIntValue();		//���ת�����ȱ仯����
	m_dwMaxTurnForceTime = (DWORD)pNode->GetVar("MaxTurnForceTime")->GetIntValue();		//���ת�����ȱ仯����
	m_fMinTurnForce		 = (float)pNode->GetVar("MinTurnForce")->GetIntValue();			//ת������
	m_fMaxTurnForce		 = (float)pNode->GetVar("MaxTurnForce")->GetIntValue();

	m_fMinRadomSpeed = pNode->GetVar("MinRadomSpeed")->GetFloatValue();
	m_fMaxRadomSpeed = pNode->GetVar("MaxRadomSpeed")->GetFloatValue();

	m_dwMinTurnDirectionTime = (DWORD)pNode->GetVar("MinTurnDirectionTime")->GetIntValue();
	m_dwMaxTurnDirectionTime = (DWORD)pNode->GetVar("MaxTurnDirectionTime")->GetIntValue();

	m_dwMinIdleInterval = (DWORD)pNode->GetVar("MinIdleInterval")->GetIntValue();
	m_dwMaxIdleInterval = (DWORD)pNode->GetVar("MaxIdleInterval")->GetIntValue();

	m_dwMinWalkInterval = (DWORD)pNode->GetVar("MinWalkInterval")->GetIntValue();
	m_dwMaxWalkInterval = (DWORD)pNode->GetVar("MaxWalkInterval")->GetIntValue();

	m_dwMinStandInterval = (DWORD)pNode->GetVar("MinStandInterval")->GetIntValue();
	m_dwMaxStandInterval = (DWORD)pNode->GetVar("MaxStandInterval")->GetIntValue();

	// ��ȡ��������
	m_dwRabbitCount = (DWORD)pNode->GetVar("number")->GetIntValue();
	m_pRabbitArray  = new tagRabbit[m_dwRabbitCount];
	ZeroMemory(m_pRabbitArray, sizeof(tagRabbit) * m_dwRabbitCount);	
	DWORD dwCurrentTime = timeGetTime();
	for(DWORD n = 0; n < m_dwRabbitCount; ++n)
	{
		tagRabbit * pRabbit = &m_pRabbitArray[n];

		if (m_dwAlphaFadeTimeLength > 0)
		{
			pRabbit->iAlphaMode			  = GameBaseEnvCreature::EC_Fade_In;
			pRabbit->dwAlphaFadeStartTime = dwCurrentTime;
		}

		pRabbit->pAnimInfo  = new AnimInfo();
		pRabbit->dwLifeTime = 0;

		float fRadomSpeed = m_fMaxRadomSpeed - m_fMinRadomSpeed;
		pRabbit->fSpeed   = m_fMinRadomSpeed + fRadomSpeed * (float)rand() / (float)RAND_MAX - fRadomSpeed / 2;

		int iAngleDest = rand() % 360;
		iAngleDest	   = iAngleDest % 360;
		if (iAngleDest < 0) 
			iAngleDest += 360;

		int iAngleSrouce = (int)D3DXToDegree(pRabbit->fRotationY);
		iAngleSrouce	 = iAngleSrouce % 360;
		if (iAngleSrouce < 0) 
			iAngleSrouce += 360;

		// �����ֵ
		int iDisA = iAngleDest - iAngleSrouce;
		int iDisB = (iAngleDest + 360) - iAngleSrouce;
		if (iDisA != 0)
		{
			float fSign = (float)(iDisA / abs(iDisA));
			pRabbit->fTurnDirection = (float)( ( abs(iDisA) < abs(iDisB) ) ? fSign : -fSign);
		}

		pRabbit->fTurnForce = m_fMinTurnForce + (m_fMaxTurnForce - m_fMinTurnForce) * (float)rand() / (float)RAND_MAX;

		ChangeStatus(pRabbit, (RABBIT_STATUS)(rand() % (RS_WALK + 1)), dwCurrentTime);
	}

	//���ÿ�ʼʱ��
	m_dwLastTime = dwCurrentTime;

	return TRUE;
}



/*
* ����: ����������Դ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.02.16 - lpf
*		�������첽������ʾģ��
*	2008.02.19 - lpf
*		�޸�����ʾģ���ͷ�˳��
*/
void GameEnvCreature_Rabbit::Destroy()
{
	// ����
	CGameModelManager::GetInstance()->ReleaseDisplayModel(m_pModel);
	m_pModel = NULL;

	for (DWORD n = 0; n < m_dwRabbitCount; ++n)
		SAFEDELETE(m_pRabbitArray[n].pAnimInfo);

	SAFEDELETEARRAY(m_pRabbitArray);
}



/*
* ����: ���Ӹ���
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.11.13 - lpf
*		�����˻���������޳�,���Զ�������㴦��
*	2008.12.05 - lpf
*		�����˻�������͸������Ĵ���
*/
void GameEnvCreature_Rabbit::Update(const D3DXVECTOR3 *pPosition, render::Frustum * pFrustum)
{
	if (m_bFollowPlayer == FALSE)
	{
		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, m_vBirthPos.x, m_vBirthPos.y, m_vBirthPos.z);
		m_bIsDisplay = pFrustum->BoxIntersectFrustum(&mWorld, &m_RegionBox);
		if (m_bIsDisplay == FALSE)
			return;
	}

	DWORD dwCurrentTime = timeGetTime();
	DWORD dwTimeElapse = dwCurrentTime - m_dwLastTime;
	m_dwLastTime = dwCurrentTime;

	//ˢ��
	for(DWORD n = 0; n < m_dwRabbitCount;n++)
	{
		tagRabbit * pRabbit = &m_pRabbitArray[n];

		if (pRabbit->dwLifeTime == 0)
		{
			if (pRabbit->iAlphaMode == GameBaseEnvCreature::EC_Fade_Out)
			{
				pRabbit->fCurrentAlphaValue = (float)((int)m_dwAlphaFadeTimeLength - ((int)dwCurrentTime - (int)pRabbit->dwAlphaFadeStartTime)) / (float)m_dwAlphaFadeTimeLength;

				if (pRabbit->fCurrentAlphaValue < 0.0f)
				{
					pRabbit->fCurrentAlphaValue = 0.0f;
					pRabbit->iAlphaMode = GameBaseEnvCreature::EC_Fade_In;
				}

				AI(pRabbit, dwCurrentTime, dwTimeElapse);
			}else
			{
				//�жϾ��룬�ɼ�����������
				float fx = (pPosition->x - pRabbit->vPosition.x);
				float fy = (pPosition->y - pRabbit->vPosition.y);
				float fz = (pPosition->z - pRabbit->vPosition.z);
				if ( (fx * fx + fy * fy + fz * fz) > (m_fRadius * m_fRadius))
				{
					if (m_bFollowPlayer == TRUE)
					{
						pRabbit->vPosition.x = pPosition->x + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
						pRabbit->vPosition.z = pPosition->z + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
					}else
					{
						pRabbit->vPosition.x = m_vBirthPos.x + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
						pRabbit->vPosition.z = m_vBirthPos.z + (m_fRadius * 2) * (float)rand() / (float)RAND_MAX - m_fRadius;
					}

					pRabbit->pAnimInfo->ClearTrailContainer();
					ChangeStatus(pRabbit,RS_STAND,dwCurrentTime);
				}

				pRabbit->dwLifeTime = m_dwMinLifeTime + rand() % ( m_dwMaxLifeTime - m_dwMinLifeTime );
				pRabbit->dwAlphaFadeStartTime = dwCurrentTime;

				float fRadomSpeed = m_fMaxRadomSpeed - m_fMinRadomSpeed;
				pRabbit->fSpeed = m_fMinRadomSpeed + fRadomSpeed * (float)rand() / (float)RAND_MAX - fRadomSpeed / 2;
				//
				int iAngleDest   = rand() % 360;
				iAngleDest = iAngleDest % 360;
				if (iAngleDest < 0) 
					iAngleDest += 360;

				int iAngleSrouce = (int)D3DXToDegree(pRabbit->fRotationY);
				iAngleSrouce = iAngleSrouce % 360;
				if (iAngleSrouce < 0) 
					iAngleSrouce += 360;

				//�����ֵ
				int iDisA = iAngleDest - iAngleSrouce;
				int iDisB = (iAngleDest + 360) - iAngleSrouce;
				if (iDisA != 0)
				{
					float fSign = (float)(iDisA / abs(iDisA));
					pRabbit->fTurnDirection = (float)( ( abs(iDisA) < abs(iDisB) )? fSign : -fSign);
				}

				pRabbit->fTurnForce = m_fMinTurnForce + (m_fMaxTurnForce - m_fMinTurnForce) * (float)rand() / (float)RAND_MAX;
			}
		}
		else
		{
			if (pRabbit->iAlphaMode == GameBaseEnvCreature::EC_Fade_In)
			{
				pRabbit->fCurrentAlphaValue = (float)(dwCurrentTime - pRabbit->dwAlphaFadeStartTime) / (float)m_dwAlphaFadeTimeLength;

				if (pRabbit->fCurrentAlphaValue > 1.0f)
				{
					pRabbit->fCurrentAlphaValue = 1.0f;
					pRabbit->iAlphaMode = GameBaseEnvCreature::EC_Fade_None;
				}
			}else
			{
				if (pRabbit->dwLifeTime < dwTimeElapse)
				{
					pRabbit->dwLifeTime = 0;
					pRabbit->iAlphaMode = GameBaseEnvCreature::EC_Fade_Out;
					pRabbit->dwAlphaFadeStartTime = dwCurrentTime;
				}else
					pRabbit->dwLifeTime = pRabbit->dwLifeTime - dwTimeElapse;
			}

			AI(pRabbit, dwCurrentTime, dwTimeElapse);
		}		
	}	
}



/*
* ����: ����AI
* ժҪ: ��Ҫ�����ڻ���������״̬�µ������ꡢ�����ٶȵȵȵĸ���
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.12.05
* �޸���־:
*/
void GameEnvCreature_Rabbit::AI(tagRabbit * pRabbit, DWORD dwCurrentTime, DWORD dwTimeElapse)
{
	DWORD dwTurnDirectionRange = m_dwMaxTurnDirectionTime - m_dwMinTurnDirectionTime;
	if (dwCurrentTime - pRabbit->dwTurnDirectionLastTime > m_dwMinTurnDirectionTime + rand() % dwTurnDirectionRange)
	{
		pRabbit->dwTurnDirectionLastTime = dwCurrentTime;
		//��ǰ��������ļн�
		int iAngleDest   = rand() % 360;
		int iAngleSrouce = (int)D3DXToDegree(pRabbit->fRotationY);
		iAngleSrouce = iAngleSrouce % 360;
		if (iAngleSrouce < 0) 
			iAngleSrouce += 360;

		//�����ֵ
		int iDisA = iAngleDest - iAngleSrouce;
		int iDisB = (iAngleDest + 360) - iAngleSrouce;
		if (iDisA != 0)
		{
			float fSign = (float)(iDisA / abs(iDisA));
			pRabbit->fTurnDirection = (float)( ( abs(iDisA) < abs(iDisB) )? fSign : -fSign);
		}
	}

	switch(pRabbit->eStatus)
	{
	case RS_STAND:
		if (dwCurrentTime - pRabbit->dwLastAI_Time > m_dwMinStandInterval + rand() % (m_dwMaxStandInterval - m_dwMinStandInterval))
		{
			ChangeStatus(pRabbit,RS_IDLE,dwCurrentTime);
		}
		break;
	case RS_IDLE:
		{
			if (dwCurrentTime - pRabbit->dwLastAI_Time > m_dwMinIdleInterval + rand() % (m_dwMaxIdleInterval - m_dwMinIdleInterval))
			{
				pRabbit->fRotationY = D3DXToRadian(rand() % 360);
				ChangeStatus(pRabbit,RS_WALK,dwCurrentTime);
			}
		}
		break;
	case RS_WALK:
		{
			if (dwCurrentTime -  pRabbit->dwLastAI_Time> m_dwMinWalkInterval + rand() % (m_dwMaxWalkInterval  - m_dwMinWalkInterval ))
			{
				ChangeStatus(pRabbit,RS_STAND,dwCurrentTime);
			}
			if (pRabbit->pAnimInfo->GetActionInfo()->dwCurActionLoopCount > 0)
			{
				//ÿ��һ��������ڸı�ת������ֵ
				DWORD dwTurnTimeRange = m_dwMaxTurnForceTime - m_dwMinTurnForceTime;
				if (dwCurrentTime - pRabbit->dwTurnForceLastTime > m_dwMinTurnForceTime + rand() % dwTurnTimeRange)
				{
					pRabbit->dwTurnForceLastTime = dwCurrentTime;
					pRabbit->fTurnForce = m_fMinTurnForce + (m_fMaxTurnForce - m_fMinTurnForce) * (float)rand() / (float)RAND_MAX;
				}

				pRabbit->fRotationY += D3DXToRadian(pRabbit->fTurnDirection * pRabbit->fTurnForce);
			}

			float fRatio = (float)dwTimeElapse * 0.001f;
			D3DXVECTOR3 vStep(0.0f,0.0f,pRabbit->fSpeed * fRatio);
			D3DXMATRIX mRot;
			D3DXMatrixRotationY(&mRot,pRabbit->fRotationY);
			D3DXVec3TransformNormal(&vStep,&vStep,&mRot);
			pRabbit->vPosition += vStep;
		}
	}
}



/*
* ����: ��Ⱦ����
* ժҪ: -
* ����: pMap	 - ��ǰʹ�õ�ͼ����ָ��
*		 pCamera - ���������ָ��
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.08.13 - lpf
*		�����˶�ģ���Ƿ���ȫ������ж�
*	2008.11.13 - lpf
*		�����˻���������޳�
*	2008.12.05 - lpf
*		�����˻�������͸��������Ⱦ����
*/
void GameEnvCreature_Rabbit::Render(const GameMap *pMap,render::Camera *pCamera)
{
	if (m_pModel->GetLoadState() != Load_Did ||
		m_bIsDisplay == FALSE)
		return;

	static D3DXMATRIX  matWorld,matTrans,matScale,matRotY;

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
	DWORD dwCurTime = timeGetTime();
	DWORD dwSunLightAmbient = pMap->GetSunLightAmbientColorforBuild();
	DWORD dwSunLightDiffuse = pMap->GetSunLightDiffuseColor();
	const D3DXVECTOR3 *pLightDirectiion = pMap->GetSunLightDirection();
	//	GameMap::tagGrid *pGrid(NULL);
	float fMapWidth = (float)pMap->GetWidth();
	float fMapDepth = (float)pMap->GetDepth();

	//��Ⱦ
	for(DWORD n = 0; n < m_dwRabbitCount;n++)
	{
		tagRabbit * pRabbit = &m_pRabbitArray[n];
		if (!((int)pRabbit->vPosition.x <0 || (int)pRabbit->vPosition.z < 0 || (int)pRabbit->vPosition.x >= (int)pMap->GetWidth() ||
			(int)pRabbit->vPosition.z >= (int)pMap->GetDepth()))
		{
			D3DXMatrixScaling(&matScale,m_fScale,m_fScale,m_fScale);
			D3DXMatrixRotationY(&matRotY,pRabbit->fRotationY);
			D3DXMatrixTranslation(&matTrans,pRabbit->vPosition.x,pMap->CalculateHeight(pRabbit->vPosition.x,pRabbit->vPosition.z),pRabbit->vPosition.z);
			D3DXMatrixMultiply(&matWorld,&matScale,&matRotY);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTrans);

			pRabbit->pAnimInfo->SetBillboard(pCamera->GetBillboard());
			pRabbit->pAnimInfo->SetBillboardY(pCamera->GetBillboardY());
			pRabbit->pAnimInfo->SetWorldMatrix(&matWorld);
			pRabbit->pAnimInfo->SetViewMatrix(pCamera->GetViewMatrix());
			pRabbit->pAnimInfo->SetProjectedMatrix(pCamera->GetProjectionMatrix());
			pRabbit->pAnimInfo->SetCurrentTime(dwCurTime);

			m_pModel->SetDirLightEnable(FALSE);
			m_pModel->ProcessAnimJoint(pRabbit->pAnimInfo);
			m_pModel->ProcessAnimTexture(pRabbit->pAnimInfo);
			m_pModel->ProcessAnimUV(pRabbit->pAnimInfo);
			m_pModel->ProcessParticleEmitter(pRabbit->pAnimInfo);
			m_pModel->ProcessTrailEmitter(pRabbit->pAnimInfo);
			m_pModel->ProcessLocator(pRabbit->pAnimInfo);
			if (m_bLighted)
			{
				DWORD dwColor = pMap->GetGridColor((int)pRabbit->vPosition.x,(int)pRabbit->vPosition.z);

				//��ɫ����1.25��
				DWORD r = ( (dwColor & 0x00FF0000) >> 16) * 5 / 4;
				if (r > 255 )
					r = 255;
				DWORD g = ( (dwColor & 0x0000FF00) >>  8) * 5 / 4;
				if (g > 255) 
					g = 255;
				DWORD b = (  dwColor & 0x000000FF) * 5 / 4;
				if (b > 255)
					b = 255;
				dwColor = r << 16 | g << 8 | b;
				m_pModel->MultiplyModelColor(dwColor);
			}
			else
			{
				m_pModel->SetModelColor(0xFFFFFFFF);
			}
			m_pModel->ProcessAnimColor(pRabbit->pAnimInfo);
			m_pModel->ProcessVisibility(pRabbit->pAnimInfo);

			if (pRabbit->iAlphaMode != GameBaseEnvCreature::EC_Fade_None)
			{
				m_pModel->MultiplyModelAlpha(pRabbit->fCurrentAlphaValue);
				m_pModel->RenderModelTransparent();
			}else
				m_pModel->RenderModel();
		}
	}
}

void GameEnvCreature_Rabbit::ChangeStatus(GameEnvCreature_Rabbit::tagRabbit *pRabbit,GameEnvCreature_Rabbit::RABBIT_STATUS eStatus,DWORD dwCurTime)
{
	pRabbit->eStatus = eStatus;	
	pRabbit->dwLastAI_Time = dwCurTime; 
	//Setup Action
	switch(eStatus)
	{
	case RS_STAND:
		pRabbit->pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('0','0','0','0');
		pRabbit->pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		pRabbit->pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 500;
		pRabbit->pAnimInfo->GetActionInfo()->dwInterpolationStartTime = dwCurTime;
		pRabbit->pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
		pRabbit->pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		break;
	case RS_IDLE:
		pRabbit->pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('0','0','0','1');
		pRabbit->pAnimInfo->GetActionInfo()->bInterpolation = TRUE;
		pRabbit->pAnimInfo->GetActionInfo()->dwInterpolationTimeLength = 500;
		pRabbit->pAnimInfo->GetActionInfo()->dwInterpolationStartTime = dwCurTime;
		pRabbit->pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
		pRabbit->pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		break;
	case RS_WALK:
		pRabbit->pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('0','0','0','2');
		pRabbit->pAnimInfo->GetActionInfo()->bInterpolation = FALSE;
		pRabbit->pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;
		pRabbit->pAnimInfo->GetActionInfo()->dwCurActionLoopCount = 0;
		break;
	}
}



GameEnvCreature_Meteor::GameEnvCreature_Meteor()
{
}



GameEnvCreature_Meteor::~GameEnvCreature_Meteor()
{
}



/*
* ����: �������ǻ�������
* ժҪ: -
* ����: pNode			- �����ļ��ڵ�
*		 szResourcePath	- ����������Դ·��
* ����ֵ: �ɹ�����true
* ����: lpf
* ��������: 2008.7.1
* �޸���־:
*	2008.11.13 - lpf
*		�����˻��������Զ���������������ö�ȡ
*	2008.12.05 - lpf
*		�����˻�������͸������ʱ�����ö�ȡ
*/
BOOL GameEnvCreature_Meteor::Create(utility::Text::Node * pNode, char * szResourcePath)
{
	char szTemp[1024];
	_snprintf(szTemp, 1024, "%s\\%s", szResourcePath, pNode->GetVar("model")->GetStringValue());
	m_pModel = CGameModelManager::GetInstance()->CreateDisplayModel(szTemp);
	VERIFY(m_pModel);
	m_pModel->ShowAllGroup();
	m_pModel->EnableParticleEmitter(TRUE);
	m_pModel->EnableTrailEmitter(TRUE);

	m_fScale   = pNode->GetVar("Scale")->GetFloatValue();
	m_fRadius  = pNode->GetVar("Radius")->GetFloatValue();
	m_fHeight = pNode->GetVar("Height")->GetFloatValue();
	m_bLighted = pNode->GetVar("Lighted")->GetBOOLValue();
	m_dwLiveTimeMax = (DWORD)pNode->GetVar("MaxLiveTime")->GetIntValue();
	m_dwAlphaFadeTimeLength = (pNode->GetVar("AlphaFadeTimeLength") == NULL) ? 0 : (DWORD)pNode->GetVar("AlphaFadeTimeLength")->GetIntValue();

	if (pNode->GetVar("FollowPlayer") != NULL)
	{
		m_bFollowPlayer = pNode->GetVar("FollowPlayer")->GetBOOLValue();
		if (m_bFollowPlayer == FALSE)
		{
			utility::Text::Node * pNodeBirthPos = pNode->GetChildNode("BirthPos");
			if (pNodeBirthPos != NULL)
			{
				m_vBirthPos.x = pNodeBirthPos->GetVar("x")->GetFloatValue();
				m_vBirthPos.y = pNodeBirthPos->GetVar("y")->GetFloatValue();
				m_vBirthPos.z = pNodeBirthPos->GetVar("z")->GetFloatValue();
			}
		}
		m_RegionBox.Set(D3DXVECTOR3(m_fRadius * -2.0f, m_fRadius * -2.0f, m_fRadius * -2.0f), D3DXVECTOR3(m_fRadius * 2.0f, m_fHeight, m_fRadius * 2.0f));
	}

	// �ٶ�ʸ��
	m_vSpeedMin.x = pNode->GetChildNode("Speed")->GetChildNode("Min")->GetVar("x")->GetFloatValue();
	m_vSpeedMin.y = pNode->GetChildNode("Speed")->GetChildNode("Min")->GetVar("y")->GetFloatValue();
	m_vSpeedMin.z = pNode->GetChildNode("Speed")->GetChildNode("Min")->GetVar("z")->GetFloatValue();
	m_vSpeedMax.x = pNode->GetChildNode("Speed")->GetChildNode("Max")->GetVar("x")->GetFloatValue();
	m_vSpeedMax.y = pNode->GetChildNode("Speed")->GetChildNode("Max")->GetVar("y")->GetFloatValue();
	m_vSpeedMax.z = pNode->GetChildNode("Speed")->GetChildNode("Max")->GetVar("z")->GetFloatValue();

	// ��ȡ��������
	m_dwMeteorCount = (DWORD)pNode->GetVar("Number")->GetIntValue();
	m_pMeteorArray  = new tagMeteor[m_dwMeteorCount];
	ZeroMemory(m_pMeteorArray, sizeof(tagMeteor) * m_dwMeteorCount);
	DWORD dwCurrentTime = timeGetTime();
	tagMeteor * pMeteor;
	for (DWORD dw = 0; dw < m_dwMeteorCount; ++dw)
	{
		pMeteor = &m_pMeteorArray[dw];

		if (m_dwAlphaFadeTimeLength > 0)
		{
			pMeteor->iAlphaMode			  = GameBaseEnvCreature::EC_Fade_In;
			pMeteor->dwAlphaFadeStartTime = dwCurrentTime;
		}

		pMeteor->dwStartTime = dwCurrentTime;
		pMeteor->dwLiveTimeMax = FieldRandom(m_dwLiveTimeMax * 0.5f, m_dwLiveTimeMax * 1.5f);
		pMeteor->pAnimInfo = new AnimInfo();
	}

	return TRUE;
}



/*
* ����: ��������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.7.1
* �޸���־:
*/
void GameEnvCreature_Meteor::Destroy()
{
	// ����
	CGameModelManager::GetInstance()->ReleaseDisplayModel(m_pModel);
	m_pModel = NULL;

	for (DWORD dw = 0; dw < m_dwMeteorCount; ++dw)
		SAFEDELETE(m_pMeteorArray[dw].pAnimInfo);

	SAFEDELETEARRAY(m_pMeteorArray);
}



/*
* ����: ��������
* ժҪ: -
* ����: pPosition - ����ʱ�Ĳο�����
* ����ֵ: -
* ����: lpf
* ��������: 2008.7.1
* �޸���־:
*	2008.11.13 - lpf
*		�����˻���������޳�,���Զ�������㴦��
*	2008.12.05 - lpf
*		�����˻�������͸������Ĵ���
*/
void GameEnvCreature_Meteor::Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum)
{
	if (m_bFollowPlayer == FALSE)
	{
		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, m_vBirthPos.x, m_vBirthPos.y, m_vBirthPos.z);
		m_bIsDisplay = pFrustum->BoxIntersectFrustum(&mWorld, &m_RegionBox);
		if (m_bIsDisplay == FALSE)
			return;
	}

	DWORD dwCurrentTime = timeGetTime();
	tagMeteor * pMeteor = NULL;

	//ˢ��
	for (DWORD dw = 0; dw < m_dwMeteorCount; ++dw)
	{
		pMeteor = &m_pMeteorArray[dw];

		if (pMeteor->iAlphaMode == GameBaseEnvCreature::EC_Fade_In)
		{
			pMeteor->fCurrentAlphaValue = (float)(dwCurrentTime - pMeteor->dwAlphaFadeStartTime) / (float)m_dwAlphaFadeTimeLength;

			if (pMeteor->fCurrentAlphaValue > 1.0f)
			{
				pMeteor->fCurrentAlphaValue = 1.0f;
				pMeteor->dwStartTime		= dwCurrentTime;
				pMeteor->iAlphaMode			= GameBaseEnvCreature::EC_Fade_None;
			}
		}else if (pMeteor->iAlphaMode == GameBaseEnvCreature::EC_Fade_Out)
		{
			pMeteor->fCurrentAlphaValue = (float)((int)m_dwAlphaFadeTimeLength - ((int)dwCurrentTime - (int)pMeteor->dwAlphaFadeStartTime)) / (float)m_dwAlphaFadeTimeLength;

			if (pMeteor->fCurrentAlphaValue < 0.0f)
			{
				pMeteor->fCurrentAlphaValue = 0.0f;
				pMeteor->iAlphaMode = GameBaseEnvCreature::EC_Fade_In;
				pMeteor->dwAlphaFadeStartTime = dwCurrentTime;

				if (m_bFollowPlayer == TRUE)
				{
					pMeteor->vPosition.x = pPosition->x + FieldRandom(-m_fRadius, m_fRadius);
					pMeteor->vPosition.y = m_fHeight;
					pMeteor->vPosition.z = pPosition->z + FieldRandom(-m_fRadius, m_fRadius);
				}else
				{
					pMeteor->vPosition.x = m_vBirthPos.x + FieldRandom(-m_fRadius, m_fRadius);
					pMeteor->vPosition.y = m_fHeight;
					pMeteor->vPosition.z = m_vBirthPos.z + FieldRandom(-m_fRadius, m_fRadius);
				}

				pMeteor->vSpeed.x = FieldRandom(m_vSpeedMin.x, m_vSpeedMax.x);
				pMeteor->vSpeed.y = FieldRandom(m_vSpeedMin.y, m_vSpeedMax.y);
				pMeteor->vSpeed.z = FieldRandom(m_vSpeedMin.z, m_vSpeedMax.z);

				pMeteor->dwLiveTimeMax = FieldRandom(m_dwLiveTimeMax * 0.5f, m_dwLiveTimeMax * 1.5f);
				pMeteor->pAnimInfo->ClearTrailContainer();
			}
		}else
		{
			if (dwCurrentTime - pMeteor->dwStartTime >= pMeteor->dwLiveTimeMax)
			{
				pMeteor->iAlphaMode			  = GameBaseEnvCreature::EC_Fade_Out;
				pMeteor->dwAlphaFadeStartTime = dwCurrentTime;
			}
		}

		AI(pMeteor);
	}
}



/*
* ����: ����AI
* ժҪ: ��Ҫ�����ڻ���������״̬�µ������ꡢ�����ٶȵȵȵĸ���
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.12.05
* �޸���־:
*/
void GameEnvCreature_Meteor::AI(tagMeteor * pMeteor)
{
	pMeteor->vPosition += pMeteor->vSpeed;
}



/*
* ����: ��Ⱦ����
* ժҪ: -
* ����: pMap	 - ��ǰʹ�õ�ͼ����ָ��
*		 pCamera - ���������ָ��
* ����ֵ: -
* ����: lpf
* ��������: 2008.7.1
* �޸���־:
*	2008.08.13 - lpf
*		�����˶�ģ���Ƿ���ȫ������ж�
*	2008.10.16 - lpf
*		ȡ���˵��θ�����ɫ��ģ�͵�Ӱ��
*	2008.11.13 - lpf
*		�����˻���������޳���ʾ
*	2008.12.05 - lpf
*		�����˻�������͸��������Ⱦ����
*/
void GameEnvCreature_Meteor::Render(const GameMap * pMap, render::Camera * pCamera)
{
	if (m_pModel->GetLoadState() != Load_Did ||
		m_bIsDisplay == FALSE)
		return;

	static D3DXMATRIX matWorld, matTrans, matScale, matRotY;

	render::Interface		   * pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D * pLayer3D	= pInterface->GetLayer3D();
	DWORD dwCurTime			= timeGetTime();
	DWORD dwSunLightAmbient = pMap->GetSunLightAmbientColorforBuild();
	DWORD dwSunLightDiffuse = pMap->GetSunLightDiffuseColor();
	const D3DXVECTOR3 * pLightDirectiion = pMap->GetSunLightDirection();
	tagMeteor * pMeteor = NULL;

	//��Ⱦ
	for(DWORD dw = 0; dw < m_dwMeteorCount; ++dw)
	{
		pMeteor = &m_pMeteorArray[dw];

		D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
		D3DXMatrixTranslation(&matTrans, pMeteor->vPosition.x, pMeteor->vPosition.y, pMeteor->vPosition.z);
		D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);

		pMeteor->pAnimInfo->SetBillboard(pCamera->GetBillboard());
		pMeteor->pAnimInfo->SetBillboardY(pCamera->GetBillboardY());
		pMeteor->pAnimInfo->SetWorldMatrix(&matWorld);
		pMeteor->pAnimInfo->SetViewMatrix(pCamera->GetViewMatrix());
		pMeteor->pAnimInfo->SetProjectedMatrix(pCamera->GetProjectionMatrix());
		pMeteor->pAnimInfo->SetCurrentTime(dwCurTime);
		pMeteor->pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('0','0','0','0');
		pMeteor->pAnimInfo->GetActionInfo()->bCurActionLooped = TRUE;

		m_pModel->SetDirLightEnable(FALSE);
		m_pModel->ProcessAnimJoint(pMeteor->pAnimInfo);
		m_pModel->ProcessAnimTexture(pMeteor->pAnimInfo);
		m_pModel->ProcessAnimUV(pMeteor->pAnimInfo);
		m_pModel->ProcessParticleEmitter(pMeteor->pAnimInfo);
		m_pModel->ProcessTrailEmitter(pMeteor->pAnimInfo);
		m_pModel->ProcessLocator(pMeteor->pAnimInfo);

		if (!m_bLighted)
			m_pModel->SetModelColor(0xFFFFFFFF);

		m_pModel->ProcessAnimColor(pMeteor->pAnimInfo);
		m_pModel->ProcessVisibility(pMeteor->pAnimInfo);

		if (pMeteor->iAlphaMode != GameBaseEnvCreature::EC_Fade_None)
		{
			m_pModel->MultiplyModelAlpha(pMeteor->fCurrentAlphaValue);
			m_pModel->RenderModelTransparent();
		}else
			m_pModel->RenderModel();
	}
}