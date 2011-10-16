
#include "stdafx.h"
#include <algorithm>
#include "futility.h"
#include "fconsole.h"
#include "frender.h"
#include "fparticle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SINGLETON(particle::Manager);

//CLASS__(particle::Emitter)_____
void particle::Emitter::CreateFromFile(const char *pszFileName)
{
	static char pszResourcePath[_MAX_PATH];

	utility::File::GetFilePath(pszFileName,pszResourcePath);

	utility::File file;
	if (!file.Open(pszFileName))
	{
		OutputConsole("error: CreateFromFile() \"%s\" failed!\n",pszFileName);
		ASSERT(0);
	}

	char *szParameter = new char[file.GetLength() + 1];
	memcpy(szParameter,file.GetData(),file.GetLength());
	szParameter[file.GetLength()] = 0;;
	file.Close();

	Create(szParameter,pszResourcePath);

	SAFEDELETEARRAY(szParameter);

}

void particle::Emitter::Create(const char *pszParameter,const char *pszResourcePath)
{
	ZeroMemory(&m_tParameter,sizeof(m_tParameter));
	//����Shader
	utility::Text cScript;
	cScript.Create(pszParameter);

	//��ȡSHADER
	utility::Text::Node *pShaderNode = cScript.GetRootNode()->GetChildNode("shader");

	char szShaderFileName[_MAX_PATH];
	_snprintf(szShaderFileName,_MAX_PATH,"%s\\%s",pszResourcePath,pShaderNode->GetVar("file")->GetStringValue());

	utility::File fileShader;	
	if (!fileShader.Open(szShaderFileName))
	{
		OutputConsole("error: open \"%s\" failed.\n",szShaderFileName);
		ASSERT(0);
		return;
	}
	char *pszShaderText = new char [fileShader.GetLength() + 1];
	memcpy(pszShaderText,fileShader.GetData(),fileShader.GetLength());
	pszShaderText[fileShader.GetLength()] = 0;	
	fileShader.Close();
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D   *pLayer3D = pInterface->GetLayer3D();
	m_dwShaderGroupHandle = pLayer3D->CreateShaderGroup(pszShaderText,pszResourcePath);
	SAFEDELETE(pszShaderText);

	//��ȡ����
	utility::Text::Node *pParameterNode = cScript.GetRootNode()->GetChildNode("parameter");
	strncpy(m_tParameter.szName,pParameterNode->GetVar("name")->GetStringValue(),MAX_EMITTER_NAME - 1);
	m_tParameter.szName[MAX_EMITTER_NAME - 1] = 0;	

	utility::Text::Variable *pTempVar(NULL);
	//����
	static const char * szPTypeString[] = {"quad","belt"};
	m_tParameter.eParticleType     = (PARTICLE_TYPE)pParameterNode->GetVar("type")->GetEnumValue(szPTypeString,2);
	//BillBoard ���ͱ�־
	m_tParameter.bBillboard		   = pParameterNode->GetVar("billboard")->GetBOOLValue();
	pTempVar					   = pParameterNode->GetVar("billboard_y");
	m_tParameter.bBillboardY	   = (pTempVar)? pTempVar->GetBOOLValue() : FALSE;
	pTempVar					   = pParameterNode->GetVar("followedowner");
	m_tParameter.bfollowedowner	   = (pTempVar)? pTempVar->GetBOOLValue() : FALSE;
	m_tParameter.fAngleX		   = pParameterNode->GetVar("angleX")? D3DXToRadian(pParameterNode->GetVar("angleX")->GetFloatValue()) : 0.0f;

	m_tParameter.fInitVelocity     = pParameterNode->GetVar("velocity")->GetFloatValue();
	m_tParameter.dwLifeTimeLength  = (DWORD)pParameterNode->GetVar("lifetime")->GetIntValue();
	m_tParameter.iInitT            = pParameterNode->GetVar("t")->GetIntValue();
	m_tParameter.fInitWidth        = pParameterNode->GetVar("width")->GetFloatValue();
	m_tParameter.fInitHeight       = pParameterNode->GetVar("height")->GetFloatValue();
	m_tParameter.fInitWidthRange   = (pParameterNode->GetVar("width_range")) ? pParameterNode->GetVar("width_range")->GetFloatValue() : 0.0f;
	m_tParameter.fInitHeightRange   = (pParameterNode->GetVar("height_range"))? pParameterNode->GetVar("height_range")->GetFloatValue(): 0.0f;

	//��ȡ�������
	utility::Text::Node *pEmitNode = pParameterNode->GetChildNode("emit");
	m_tParameter.dwEmitInterval = (DWORD)pEmitNode->GetVar("interval")->GetIntValue();
	m_tParameter.dwEmitNumber   = (DWORD)pEmitNode->GetVar("number")->GetIntValue();

	//��ȡ�ɼ��б�
	utility::Text::Node *pVisibilityNode = pParameterNode->GetChildNode("visibility");
	m_tParameter.dwVisibilityFrameCount = pVisibilityNode->GetVarCount();
	if (m_tParameter.dwVisibilityFrameCount)
	{
		m_tParameter.pfVisibilityFrameArray = new float [m_tParameter.dwVisibilityFrameCount];
		for(DWORD n = 0; n < pVisibilityNode->GetVarCount();n++)
		{
			m_tParameter.pfVisibilityFrameArray[n] = pVisibilityNode->GetVar(n)->GetFloatValue();
		}
	}
	//��ɫ�б�
	utility::Text::Node *pColorNode = pParameterNode->GetChildNode("color");
	m_tParameter.dwColorFrameCount = pColorNode->GetVarCount();
	if (m_tParameter.dwColorFrameCount)
	{
		m_tParameter.pdwColorFrameArray = new DWORD [m_tParameter.dwColorFrameCount];
		for(DWORD n = 0; n < pColorNode->GetVarCount();n++)
		{
			m_tParameter.pdwColorFrameArray[n] = pColorNode->GetVar(n)->GetHexValue();
		}
	}

	//���� 
	utility::Text::Node *pAccNode = pParameterNode->GetChildNode("acceleration");
	m_tParameter.vAcceleration.x = pAccNode->GetVar("x")->GetFloatValue();
	m_tParameter.vAcceleration.y = pAccNode->GetVar("y")->GetFloatValue();
	m_tParameter.vAcceleration.z = pAccNode->GetVar("z")->GetFloatValue();
	//����Ƕ�
	utility::Text::Node *pPitchNode = pParameterNode->GetChildNode("pitch");
	m_tParameter.fPitchAngleStart = pPitchNode->GetVar("start")->GetFloatValue();
	m_tParameter.fPitchAngleRange = pPitchNode->GetVar("range")->GetFloatValue();


	//��ȡ����̬����                                                 	
	utility::Text::Node *pUVAnimNode = pParameterNode->GetChildNode("uvanim");
	m_tParameter.dwAnimTextureRows    = (DWORD)pUVAnimNode->GetVar("rows")->GetIntValue();
	m_tParameter.dwAnimTextureColumns = (DWORD)pUVAnimNode->GetVar("columns")->GetIntValue(); 	
	m_tParameter.dwAnimTextureTimeLength = (DWORD)pUVAnimNode->GetVar("timelength")->GetIntValue(); 	

	cScript.Destroy();
	//��ӵ�
	particle::Manager *pPM = particle::Manager::GetInstance();
	pPM->Add(this);
	//��ʼ������
	//m_dwUpdateEmitterLastTime = timeGetTime();
	m_pParticleFree = NULL;
	m_pParticleUsed = NULL;

	m_pParticleRenderHead = NULL;
}

void particle::Emitter::Destroy()
{	
	//TODo
	tagParticle *pParticle,*p;

	pParticle = m_pParticleUsed;	
	while(pParticle)
	{
		p = pParticle;
		pParticle = pParticle->pNext;
		SAFEDELETE(p);
	}

	pParticle = m_pParticleFree;	
	while(pParticle)
	{
		p = pParticle;
		pParticle = pParticle->pNext;
		SAFEDELETE(p);
	}

	particle::Manager *pPM = particle::Manager::GetInstance();
	pPM->Remove(this);  

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D   *pLayer3D = pInterface->GetLayer3D();

	pLayer3D->DestroyShaderGroup(m_dwShaderGroupHandle);

	SAFEDELETE(m_tParameter.pdwColorFrameArray);
	SAFEDELETE(m_tParameter.pfVisibilityFrameArray);
}

particle::Emitter::tagParticle * particle::Emitter::NewParticle(void)
{
	tagParticle *pNewParticle = NULL;

	//����Ƿ��п����
	if (m_pParticleFree)
	{
		pNewParticle = m_pParticleFree;
		m_pParticleFree = m_pParticleFree->pNext;
	}
	else
	{
		//û��
		pNewParticle = new tagParticle;
	}

	//�����б�
	pNewParticle->pNext = m_pParticleUsed;
	m_pParticleUsed = pNewParticle;    
	return pNewParticle;
}

void particle::Emitter::Update()
{
	DWORD dwCurTime = timeGetTime();

	m_pParticleRenderHead = NULL;

	tagParticle *pParticle,**ppParticle = &m_pParticleUsed;


	//	int iC = 0,iD = 0,iR = 0;
	while(*ppParticle)
	{
		pParticle = *ppParticle;

		//�������ŵ�����ʱ��
		DWORD dwTimeElapse = dwCurTime - pParticle->dwLifeTimeStart;
		float fT =  (m_tParameter.iInitT + (int)dwTimeElapse) * 0.001f;//����ʱ�����,��λΪ����

		//����·�� s = s0 + v0 * t + a * t * t * 0.5f;
		D3DXVECTOR3 dpos  = pParticle->vVelocity0  * fT+ m_tParameter.vAcceleration * fT * fT * 0.5f;
		if (m_tParameter.bfollowedowner)
		{
			D3DXVec3TransformCoord(&dpos,&dpos,&m_matNowMatrix);
		}

		pParticle->vPosition = pParticle->vPosition0 + dpos;
		/*pParticle->vPosition = pParticle->vPosition0 + pParticle->vVelocity0  * fT+ 
		m_tParameter.vAcceleration * fT * fT * 0.5f;*/
		//�ɼ���Visibility ��ֵ
		float fVisibility = InterpolationVisibility(dwTimeElapse);
		//��ɫ��֦
		DWORD dwColor = InterpolationColor(dwTimeElapse);
		//��ɫŨ������
		float fA = (float)( (dwColor & 0xFF000000) >> 24);
		float fR = (float)( (dwColor & 0x00FF0000) >> 16);
		float fG = (float)( (dwColor & 0x0000FF00) >>  8);
		float fB = (float)(  dwColor & 0x000000FF);

		DWORD c = (DWORD)( fA * fVisibility);
		dwColor = c << 24;
		c = (DWORD)( fR * fVisibility); 
		dwColor |= c << 16;
		c = (DWORD)( fG * fVisibility);
		dwColor |= c << 8;
		c = (DWORD)( fB * fVisibility);
		dwColor |= c;
		pParticle->dwColor = dwColor;

		//UV����  
		//�������ŵ�uv����ʱ��
		DWORD dwAnimTextureTimeElapse = dwCurTime - pParticle->dwAnimTextureStartTime;

		if (dwAnimTextureTimeElapse >= m_tParameter.dwAnimTextureTimeLength)                                            		
		{
			pParticle->dwAnimTextureStartTime = dwCurTime;
		}
		int total = m_tParameter.dwAnimTextureRows * m_tParameter.dwAnimTextureColumns;
		pParticle->iAnimTextureRectIndex  =  total * dwAnimTextureTimeElapse / m_tParameter.dwAnimTextureTimeLength;

		//��������,�����˶�			        
		if (dwTimeElapse > m_tParameter.dwLifeTimeLength)
		{
			//�������ƶ������б���
			*ppParticle = pParticle->pNext;
			pParticle->pNext = m_pParticleFree;
			m_pParticleFree = pParticle;
			//			iD++;
		}
		else
		{
			//װ����Ⱦ��
			pParticle->pNextRender = m_pParticleRenderHead;
			m_pParticleRenderHead  = pParticle;
			//��һ��
			ppParticle = &pParticle->pNext;
			//			iR++;
		}
		//		iC++;
	}
	//OutputConsole("%d %d %d\n",iC,iD,iR);
}

void particle::Emitter::SetBillboard(const D3DXMATRIX *pBillboard)
{
	memcpy(&m_matBillboard,pBillboard,sizeof(D3DXMATRIX));
}
void particle::Emitter::SetBillboardY(const D3DXMATRIX *pBillboardY)
{
	memcpy(&m_matBillboardY,pBillboardY,sizeof(D3DXMATRIX));
}

void particle::Emitter::SetPosition(const D3DXVECTOR3 * pPosition)
{
	m_vPosition = *pPosition;
}

void particle::Emitter::Render(void)
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();

	static D3DXVECTOR3 pos0[6] = { D3DXVECTOR3(-0.5f,0.5f,0.0f),D3DXVECTOR3(0.5f,0.5f,0.0f),D3DXVECTOR3(0.5f,-0.5f,0.0f),
		D3DXVECTOR3(-0.5f,0.5f,0.0f),D3DXVECTOR3(0.5f,-0.5f,0.0f),D3DXVECTOR3(-0.5f,-0.5f,0.0f) };		
	static D3DXVECTOR2 uv0[6]  = { D3DXVECTOR2(0,0),D3DXVECTOR2(1,0),D3DXVECTOR2(1,1),D3DXVECTOR2(0,0),D3DXVECTOR2(1,1),D3DXVECTOR2(0,1)};

	static D3DXVECTOR3 position[6];
	static D3DXVECTOR2 uv[6];
	static DWORD color[6],colorSpecular[6] = {0};   	
	static D3DXMATRIX  matWorld,matTrans,matScale,matRot;

	tagParticle *pParticle = m_pParticleRenderHead;
	while(pParticle)
	{
		DWORD dwColor = pParticle->dwColor;

		float x = (float)(pParticle->iAnimTextureRectIndex % m_tParameter.dwAnimTextureRows);
		float y = (float)(pParticle->iAnimTextureRectIndex / m_tParameter.dwAnimTextureRows);
		float w = 1.0f / (float)m_tParameter.dwAnimTextureRows;
		float h = 1.0f / (float)m_tParameter.dwAnimTextureColumns;
		float u = x * w;
		float v = y * h;


		switch(m_tParameter.eParticleType)
		{
		case PARTICLE_QUAD:
			{
				D3DXMatrixScaling(&matScale,pParticle->fScaleWidth,pParticle->fScaleHeight,1.0f);

				D3DXMatrixRotationX(&matRot,m_tParameter.fAngleX);
				if (m_tParameter.bBillboardY)
				{
					m_matBillboardY._41 = pParticle->vPosition.x;
					m_matBillboardY._42 = pParticle->vPosition.y;
					m_matBillboardY._43 = pParticle->vPosition.z;



					D3DXMatrixMultiply(&matWorld,&matScale,&m_matBillboardY);




				}
				else
				{
					if (m_tParameter.bBillboard)
					{
						m_matBillboard._41 = pParticle->vPosition.x;
						m_matBillboard._42 = pParticle->vPosition.y;
						m_matBillboard._43 = pParticle->vPosition.z;

						D3DXMatrixMultiply(&matWorld,&matScale,&m_matBillboard);


					}
					else
					{

						D3DXMatrixTranslation(&matTrans,pParticle->vPosition.x,pParticle->vPosition.y,pParticle->vPosition.z);
						D3DXMatrixMultiply(&matWorld,&matScale,&matRot);
						D3DXMatrixMultiply(&matWorld,&matWorld,&matTrans);


					}
				}

				for(int i = 0; i < 6; i++)
				{
					D3DXVec3TransformCoord(&position[i],&pos0[i],&matWorld);  
					uv[i].x = u + w * uv0[i].x;
					uv[i].y = v + h * uv0[i].y;
					color[i] = dwColor;
				}
			}
			pLayer3D->SendTriangleToCache(m_dwShaderGroupHandle,0,2,position,uv,color,colorSpecular,1);
			break;
		case PARTICLE_BELT:
			{
				D3DXVECTOR3 vDis = pParticle->vPosition - pParticle->vPosition0;

				if(abs(vDis.x) > abs(vDis.y))
				{
					position[0].x = 0;
					position[0].y = pParticle->fScaleHeight;
					position[0].z = 0;

					position[1].x = vDis.x;
					position[1].y = vDis.y + pParticle->fScaleHeight;
					position[1].z = vDis.z;

					position[2].x = vDis.x;
					position[2].y = vDis.y - pParticle->fScaleHeight;
					position[2].z = vDis.z;

					position[3].x = position[0].x;
					position[3].y = position[0].y;
					position[3].z = position[0].z;

					position[4].x = position[2].x;
					position[4].y = position[2].y;
					position[4].z = position[2].z;

					position[5].x = 0;
					position[5].y = -pParticle->fScaleHeight;
					position[5].z = 0;
				}
				else
				{
					position[0].x = -pParticle->fScaleWidth;
					position[0].y = 0;
					position[0].z = 0;

					position[1].x = vDis.x - pParticle->fScaleWidth;
					position[1].y = vDis.y;
					position[1].z = vDis.z;

					position[2].x = vDis.x + pParticle->fScaleWidth;
					position[2].y = vDis.y;
					position[2].z = vDis.z;

					position[3].x = position[0].x;
					position[3].y = position[0].y;
					position[3].z = position[0].z;

					position[4].x = position[2].x;
					position[4].y = position[2].y;
					position[4].z = position[2].z;

					position[5].x = pParticle->fScaleWidth;
					position[5].y = 0;
					position[5].z = 0;
				}

				m_matBillboard._41 = pParticle->vPosition0.x;
				m_matBillboard._42 = pParticle->vPosition0.y;
				m_matBillboard._43 = pParticle->vPosition0.z;

				for(int i = 0; i < 6; i++)
				{
					D3DXVec3TransformCoord(&position[i],&position[i],&m_matBillboard);  
					uv[i].x = u + w * uv0[i].x;
					uv[i].y = v + h * uv0[i].y;
					color[i] = dwColor;
				}
				pLayer3D->SendTriangleToCache(m_dwShaderGroupHandle,0,2,position,uv,color,colorSpecular,1);
			}
			break;
		}
		pParticle = pParticle->pNextRender;
	}

	m_pParticleRenderHead = NULL;
}

//CLASS__(particle::Manager)_____
BOOL particle::Manager::Create()
{
	return TRUE;
}

void particle::Manager::Destroy()
{
	//TODO:
}

void particle::Manager::Update(void)
{
	//ˢ����������
	std::list<Emitter *>::iterator it;
	for(it = m_lEmitterList.begin(); it != m_lEmitterList.end(); it++)
	{
		Emitter *pEmitter = *it;

		pEmitter->Update();
	}	
}

void particle::Manager::Render(void)
{
	std::list<Emitter *>::iterator it;
	for(it = m_lEmitterList.begin(); it != m_lEmitterList.end(); it++)
	{
		Emitter *pEmitter = *it;
		pEmitter->Render();
	}
}

void particle::Manager::Add(particle::Emitter *pEmitter)
{
	m_lEmitterList.push_back(pEmitter);
}

void particle::Manager::Remove(particle::Emitter *pEmitter)
{
	std::list<Emitter *>::iterator it;
	for(it = m_lEmitterList.begin(); it != m_lEmitterList.end(); it++)
	{
		if (*it == pEmitter)
			break;
	}

	if (it != m_lEmitterList.end())
	{
		m_lEmitterList.erase(it);
	}
}

BOOL particle::Emitter::UpdateTime(DWORD dwLastTime,DWORD dwCurTime)
{
	m_bEmited = FALSE;
	if (dwCurTime - dwLastTime > m_tParameter.dwEmitInterval)
	{
		m_bEmited = TRUE;
	}
	return m_bEmited;
}

void particle::Emitter::Emission()
{
	DWORD dwCurTime = timeGetTime();
	if (m_bEmited)
	{
		for(DWORD n = 0; n < m_tParameter.dwEmitNumber; n++)
		{
			//ȡ�ÿ��е�����
			tagParticle *pNewParticle = NewParticle();


			//�������ɲ���
			pNewParticle->vPosition0	        = m_vPosition;
			pNewParticle->dwLifeTimeStart	    = dwCurTime;
			pNewParticle->dwAnimTextureStartTime= dwCurTime; 
			pNewParticle->iAnimTextureRectIndex = 0;
			//�������ɵĵ�����

			pNewParticle->pNextRender = NULL;						//��Ⱦ��

			//��ʼ���ٶ�
			FLOAT fRandYaw   = ((float)rand()/(float)RAND_MAX) * 3.1415926f * 2.00f;
			FLOAT fRandPitch = (m_tParameter.fPitchAngleStart + ((FLOAT)rand()/(FLOAT)RAND_MAX) * m_tParameter.fPitchAngleRange) * 3.1415926f / 180.0f;

			pNewParticle->vVelocity0.y  = cosf(fRandPitch);
			pNewParticle->vVelocity0.x  = sinf(fRandPitch) * cosf(fRandYaw);
			pNewParticle->vVelocity0.z  = sinf(fRandPitch) * sinf(fRandYaw);
			pNewParticle->vVelocity0   *= m_tParameter.fInitVelocity;
			float fRatio = ((FLOAT)rand()/(FLOAT)RAND_MAX);
			pNewParticle->fScaleWidth   = m_tParameter.fInitWidth +  fRatio * m_tParameter.fInitWidthRange; 
			pNewParticle->fScaleHeight  = m_tParameter.fInitHeight + fRatio * m_tParameter.fInitHeightRange; 
		}
	}
}

float particle::Emitter::InterpolationVisibility(DWORD dwTimeElapse)
{
	//��ֵ
	//A -+- B
	DWORD dwFrameA = m_tParameter.dwVisibilityFrameCount * dwTimeElapse / m_tParameter.dwLifeTimeLength;
	if (dwFrameA >= m_tParameter.dwVisibilityFrameCount - 1)
	{
		//����Ҫ��ֵ
		dwFrameA = m_tParameter.dwVisibilityFrameCount - 1; 			
		return m_tParameter.pfVisibilityFrameArray[dwFrameA];   	
	}

	//else
	DWORD dwFrameB = (dwFrameA + 1) % m_tParameter.dwVisibilityFrameCount; 
	//get A time
	DWORD dwTimeA =  dwFrameA * m_tParameter.dwLifeTimeLength /  m_tParameter.dwVisibilityFrameCount;
	//����A B ֮�� ratio
	DWORD dwTimeRelateA = dwTimeElapse - dwTimeA;
	float fFrameInterval = (float)m_tParameter.dwLifeTimeLength / (float)m_tParameter.dwVisibilityFrameCount ;
	float fRatio  =  (float)dwTimeRelateA / fFrameInterval ; 

	//interpolation	a --- b
	float fFrameAData = m_tParameter.pfVisibilityFrameArray[dwFrameA];   	
	float fFrameBData = m_tParameter.pfVisibilityFrameArray[dwFrameB];   	
	return fFrameAData + (fFrameBData - fFrameAData) * fRatio;
}

DWORD particle::Emitter::InterpolationColor(DWORD dwTimeElapse)
{
	//��ֵ
	//A -+- B
	DWORD dwFrameA = (m_tParameter.dwColorFrameCount -1) * dwTimeElapse / m_tParameter.dwLifeTimeLength;
	if (dwFrameA >= m_tParameter.dwColorFrameCount - 1)
	{
		//����Ҫ��ֵ
		dwFrameA = m_tParameter.dwColorFrameCount - 1; 			
		return m_tParameter.pdwColorFrameArray[dwFrameA];   	
	}

	//else
	DWORD dwFrameB = (dwFrameA + 1) % m_tParameter.dwColorFrameCount; 
	//get A time
	DWORD dwTimeA = 0;
	//����A B ֮�� ratio
	float fFrameInterval = 0;
	if (m_tParameter.dwColorFrameCount > 1)
	{
		dwTimeA =  dwFrameA * m_tParameter.dwLifeTimeLength /  (m_tParameter.dwColorFrameCount-1);
		fFrameInterval = (float)m_tParameter.dwLifeTimeLength / (float)m_tParameter.dwColorFrameCount ;
	}
	else
	{
		fFrameInterval = (float)m_tParameter.dwLifeTimeLength;
	}
	DWORD dwTimeRelateA = dwTimeElapse - dwTimeA;
	float fRatio  =  (float)dwTimeRelateA / fFrameInterval ; 

	//interpolation	color a --- b
	DWORD dwFrameAColor = m_tParameter.pdwColorFrameArray[dwFrameA];   	
	DWORD dwFrameBColor = m_tParameter.pdwColorFrameArray[dwFrameB];  

	float fAA = (float)((dwFrameAColor & 0xFF000000) >> 24);
	float fRA = (float)((dwFrameAColor & 0x00FF0000) >> 16);
	float fGA = (float)((dwFrameAColor & 0x0000FF00) >>  8);
	float fBA = (float)( dwFrameAColor & 0x000000FF);

	float fAB = (float)((dwFrameBColor & 0xFF000000) >> 24);
	float fRB = (float)((dwFrameBColor & 0x00FF0000) >> 16);
	float fGB = (float)((dwFrameBColor & 0x0000FF00) >>  8);
	float fBB = (float)( dwFrameBColor & 0x000000FF);

	DWORD dwA = (DWORD)(fAA + (fAB - fAA) * fRatio);
	if (dwA > 255) dwA = 255;
	DWORD dwR = (DWORD)(fRA + (fRB - fRA) * fRatio);
	if (dwR > 255) dwR = 255;
	DWORD dwG = (DWORD)(fGA + (fGB - fGA) * fRatio);
	if (dwG > 255) dwG = 255;
	DWORD dwB = (DWORD)(fBA + (fBB - fBA) * fRatio);
	if (dwB > 255) dwB = 255;

	return D3DCOLOR_ARGB(dwA,dwR,dwG,dwB);
}