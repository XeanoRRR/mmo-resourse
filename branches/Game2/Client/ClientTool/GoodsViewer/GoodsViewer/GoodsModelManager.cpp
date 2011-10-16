#include "stdafx.h"
#include "GoodsModelManager.h"

#include "GameSetting.h"
#include "../../../EngineExtend/DisplayModel.h"
#include "../../../EngineExtend/GameModelManager.h"

IMPLEMENT_SINGLETON(GoodsModelManager);

GoodsModelManager::GoodsModelManager()
{
	m_DisplayModel = NULL;
	//ģ��ϵͳ��ʼ��
	CGameModelManager::InitInstance();	
	CGameModelManager::GetInstance()->SetTextureShareFolder(Setting::GetInstance()->m_szTextureSharFolder);
	m_GameModelManager = CGameModelManager::GetInstance();
	//���Ϊ����Ҫ���´���
	if(m_GameModelManager == NULL)
	{
		MessageBox(NULL,"CGameModelManager δ��ʼ��!","Error",MB_OK|MB_ICONERROR);
		VERIFY(m_GameModelManager);
	}
	m_Camera	= NULL;
	m_Frustum	= new render::Frustum;
	VERIFY(m_Frustum);
	SetPathName(Setting::GetInstance()->m_szGoodsModelFolder);
	SetOpenModelFileName("");
	//����ģ����ʾλ�úͽǶ�
	ResetPosAndRot();
}

GoodsModelManager::~GoodsModelManager()
{
	if(m_Camera !=NULL)
	{
		SAFEDELETE(m_Camera);
	}

	if(m_Frustum !=NULL)
	{
		SAFEDELETE(m_Frustum);
	}
	m_DisplayModel = NULL;
	m_GameModelManager = NULL;
}

void GoodsModelManager::Destory()
{
	ReleaseModel();
	CGameModelManager::GetInstance()->Release();
	CGameModelManager::ReleaseInstance();
}

bool GoodsModelManager::InitiliseCamera()
{
	m_Camera = new render::Camera();
	VERIFY(m_Camera);
	if(!m_Camera)
		return false;
	render::Interface *pInterface = render::Interface::GetInstance();
	int iWidth =  pInterface->GetWndWidth();
	int iHeight = pInterface->GetWndHeight();
	float fAspect = (float)iWidth / (float)iHeight;
	m_Camera->SetAspect(fAspect);
	m_Camera->SetFOVY(D3DXToRadian(60));
	m_Camera->SetNearClipPlane(0.01f);
	m_Camera->SetFarClipPlane(1000.0f);

	ResetCamera();
	return true;
}

void GoodsModelManager::ResetCamera(float XPos /* = 0.0f  */,float YPos /* = 0.5f  */,float ZPos /* = -2.5f */)
{
	m_Camera->SetPosition(&D3DXVECTOR3(XPos,YPos,ZPos));
	m_Camera->SetView(&D3DXVECTOR3(0.0f,YPos,0.0f));
}

void GoodsModelManager::MoveCameraRel(float relX,float relY,float relZ)
{
	D3DXVECTOR3 cameraPos = *m_Camera->GetPosition();
	cameraPos.x += relX;
	cameraPos.y += relY;
	cameraPos.z += relZ;
	m_Camera->SetPosition(&cameraPos);
}

bool GoodsModelManager::OpenModel(const char *szModelFileName /*= "model"*/, bool IsFullPathName /*= false*/)
{
	ReleaseModel();//����ǰ������
	ResetCamera();
	string strFullPathFileName;
	//�Ƿ�������·������
	if(IsFullPathName == false) //����
	{
		strFullPathFileName = m_PathName;
		strFullPathFileName += '\\';
		strFullPathFileName += szModelFileName;//strFullPathFileName = ·��+�ļ���
		SetOpenModelFileName(szModelFileName);
	}
	else // ��
	{
		strFullPathFileName = szModelFileName;//strFullPathFilename = ����·������
		//���·�����ļ���
		string strPathName = szModelFileName;
		string strFilename = szModelFileName;
		strFilename = strFilename.erase(0,strFilename.find("goods")+5);
		SetOpenModelFileName(strFilename.c_str());
		//strPathName = strPathName.erase(strPathName.find_last_of("/\\")+1);
		//SetPathName(strPathName.c_str());
	}
	// �ж��������ȷ��
	if (string(strFullPathFileName).find("model") == string::npos)
	{
		MessageBox(NULL, "�����·����û���ҵ� model �ļ�\n��ȷ��·���Ƿ���ȷ", "Error:", MB_OK);
		return false;
	}
	//����Ҫ��ʾ��ģ��
	m_DisplayModel = m_GameModelManager->CreateDisplayModel(strFullPathFileName,false);
	VERIFY(m_DisplayModel);
	if(m_DisplayModel==NULL)
		return false;
	m_DisplayModel->ShowAllGroup(false);
	m_DisplayModel->EnableParticleEmitter(TRUE);
	m_DisplayModel->EnableTrailEmitter(TRUE);

	//�����������ʹ��,ʹģ�ʹ�С��λ��
	ResetScaleCumulate();
	ScaleCumulate(0.0f,0.0f,0.0f);

	m_cAnimInfo.GetActionInfo()->dwCurAction	= 0;
	m_cAnimInfo.GetActionInfo()->bInterpolation = FALSE;
	m_cAnimInfo.SetTextureProjectiveTime(timeGetTime());

	return true;
}

void GoodsModelManager::SetupTransform()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer2D   *pLayer2D   = pInterface->GetLayer2D();
	//����ԭʼ�ľ�����Ϣ
	pInterface->GetDevice()->GetTransform(D3DTS_WORLD,&m_matWorld);
	pInterface->GetDevice()->GetTransform(D3DTS_VIEW,&m_matView);
	pInterface->GetDevice()->GetTransform(D3DTS_PROJECTION,&m_matProject);
	pInterface->GetDevice()->GetViewport(&m_OldViewPort);
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pInterface->GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);
	pInterface->GetDevice()->SetTransform(D3DTS_VIEW,m_Camera->GetViewMatrix());
	pInterface->GetDevice()->SetTransform(D3DTS_PROJECTION,m_Camera->GetProjectionMatrix());

	////���ָ��������
	int iWidth = pInterface->GetWndWidth();
	int iHeight = pInterface->GetWndHeight();
	int ixStart = iWidth * 3 /4;
	int iyStart = 0;
	int iViewWidth = iWidth-ixStart;
	int iViewHeight = iHeight / 4;
	SetViewPort(ixStart,iyStart,iViewWidth,iViewHeight);//��������1/4�����С	
	D3DRECT d3dRect;
	d3dRect.x1 = ixStart;
	d3dRect.y1 = iyStart;
	d3dRect.x2 = iWidth;
	d3dRect.y2 = iHeight/4;
	pInterface->GetDevice()->Clear(1,&d3dRect,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|( (pInterface->GetInfo()->d3dpp.AutoDepthStencilFormat == D3DFMT_D16) ? 0 : D3DCLEAR_STENCIL ) ,\
		D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0);
}

void GoodsModelManager::ResetTransform()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	pInterface->GetDevice()->SetTransform(D3DTS_WORLD,&m_matWorld);
	pInterface->GetDevice()->SetTransform(D3DTS_VIEW,&m_matView);
	pInterface->GetDevice()->SetTransform(D3DTS_PROJECTION,&m_matProject);
	pInterface->GetDevice()->SetViewport(&m_OldViewPort);
}
void GoodsModelManager::SetViewPort(int ixStart,int iyStart,int iWidth,int iHeight)
{
	D3DVIEWPORT9 vp;
	vp.X = ixStart;
	vp.Y = iyStart;
	vp.Width = iWidth;
	vp.Height = iHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer2D *pLayer2D = pInterface->GetLayer2D();
	pInterface->GetDevice()->SetViewport(&vp);
}

void GoodsModelManager::SetViewPort(const RECT& rect)
{
	D3DVIEWPORT9 vp;
	vp.X = rect.left;
	vp.Y = rect.top;
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer2D *pLayer2D = pInterface->GetLayer2D();
	pInterface->GetDevice()->SetViewport(&vp);
}

void GoodsModelManager::Update()
{
	CGameModelManager::GetInstance()->Run();
	//������׶
	if(m_Frustum)
		m_Frustum->CalculateFrustum(m_Camera->GetViewMatrix(),m_Camera->GetProjectionMatrix());
}

bool GoodsModelManager::RenderModel()
{
	render::Interface		   * pInterface = render::Interface::GetInstance();
	render::Interface::Layer2D * pLayer2D   = pInterface->GetLayer2D();
	pInterface->Clear(Setting::GetInstance()->GetBackGroundColor());
	render::Interface::Layer3D * pLayer3D = pInterface->GetLayer3D();
	pLayer3D->SetFillMode(D3DFILL_SOLID);

	//����任
	SetupTransform();

	//����ģ��
	if (m_DisplayModel/*&&m_pDisplayModel->GetLoadState() == Load_Did*/)
	{
		m_cAnimInfo.SetupAnimInfo(m_PosX,m_PosY, m_PosH, m_Rot, m_Camera);
		m_DisplayModel->SetDirLightEnable(false);

		m_cAnimInfo.SetViewMatrix(m_Camera->GetViewMatrix());
		m_cAnimInfo.SetProjectedMatrix(m_Camera->GetProjectionMatrix());
		m_DisplayModel->ProcessAnimJoint(&m_cAnimInfo);
		m_DisplayModel->ProcessVisibility(&m_cAnimInfo);
		m_DisplayModel->ProcessAnimUV(&m_cAnimInfo);
		m_DisplayModel->ProcessAnimTexture(&m_cAnimInfo);
		m_DisplayModel->ProcessAnimColor(&m_cAnimInfo);
		m_DisplayModel->ProcessLocator(&m_cAnimInfo);
		m_DisplayModel->ProcessParticleEmitter(&m_cAnimInfo);
		m_DisplayModel->ProcessTrailEmitter(&m_cAnimInfo);
		m_DisplayModel->ProcessEnvironmentMaps(m_Camera->GetViewMatrix());
		m_DisplayModel->ProcessTextureProjective(&m_cAnimInfo);
		
		m_DisplayModel->RenderModel();
	}
	pLayer3D->SetMipMap(TRUE);
	pLayer3D->SetFogEnable(FALSE);
	pLayer3D->RenderTest();
	pLayer3D->RenderBlendLow();
	pLayer3D->EndEffectsState();
	pLayer3D->RenderBlend();
	pLayer3D->Clear();
	//���þ���任
	ResetTransform();
	return true;
}

void GoodsModelManager::ReleaseModel()
{
	CGameModelManager::GetInstance()->ReleaseDisplayModel(m_DisplayModel);
}



