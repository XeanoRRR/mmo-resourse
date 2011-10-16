/***************************************************
      filename:GameWorld.cpp
      created: 2009/12/16
      author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
      purpose: �������ʵ��
****************************************************/
#include "stdafx.h"
#include "GameWorld.h"

#include "GameSetting.h"
#include "GoodsModelManager.h"
#include "../CEGUIEngine/CEGUIManager.h"
#include "../CEGUIEngine/BaseUIClass.h"

//!< ���Թ��ܿ���
bool   g_bDebug = true;

//!< ��̬������ʼ��
GameWorld *GameWorld::m_Instance = NULL;

GameWorld *GameWorld::GetInstance()
{
	return m_Instance;
}

void GameWorld::InitInstance()
{
	m_Instance = new GameWorld;
}

void GameWorld::ExitInstance()
{
	delete m_Instance;
}


void GameWorld::Create(HWND hWnd)
{
	// ������Ϸ������Ҫ�������
	m_hWnd = hWnd;//!< ���洰�ھ��

	// ��������̨
	console::Manager::InitInstance();
	console::Manager::GetInstance()->Create(console::CONSOLE_ERROR | console::CONSOLE_WARRING | console::CONSOLE_OUTPUT);

	//	��ʼ�������ӿ�
	render::Interface::InitInstance();//!< ��Ⱦ�ӿ�
	particle::Manager::InitInstance();//!< ����ϵͳ�ӿ�
	GoodsModelManager::InitInstance();//!< ��Ʒ�������ӿ�
	CEGUIManager::InitInstance();//!< ��ʼ��CEGUI�ӿ�,���Լ���һ��Dinput

	//װ�ؿͻ�����Դ
	CHAR *strPath = new CHAR[MAX_PATH];
	string strWorPath("");
	if(GetCurrentDirectory(MAX_PATH,strPath))
		strWorPath = strPath;
	delete []strPath;
	string strFilesInfo("FilesInfo.ril");//!< �ͻ��˴������Դ��Ϣ�ļ�
	CClientResource::InitialInstance();//!< �ͻ�����Դװ������ʼ��
	CClientResource  *pClientResource = CClientResource::GetInstance();
	pClientResource->Initial(GAME_RES,strWorPath,strFilesInfo);
	if(!pClientResource->LoadEx())
	{
		//װ�س���
		//MessageBox(NULL,"װ�ؿͻ�����Դ������!","ERROR",MB_OK);
	}
	
	//��ʼ����ʾ����
	utility::File file;
	if(!file.Open("render.config"))
	{
		OutputConsole("render.config doesn't exits.\n");
		MessageBox(NULL,"render.config","Error",MB_OK);
		PostQuitMessage(-1);
	}

	int iFileLenght = file.GetLength();
	char *pszTemp = new char[iFileLenght+1];
	memcpy(pszTemp,file.GetData(),file.GetLength());
	pszTemp[iFileLenght]=0;
	render::Interface::GetInstance()->Create(hWnd,pszTemp);
	SAFEDELETEARRAY(pszTemp);
	file.Close();

	//���ӹ�������صĴ�������
	particle::Manager::GetInstance()->Create();
	//��Ʒ����������ع���
	GoodsModelManager::GetInstance()->InitiliseCamera();//!< ��ʼ�������
	/////////////////////////////////////////////////
	//��������
	//CEGUI ��ʼ��
	CEGUIManager  *pCEGUIManager = CEGUIManager::GetInstance();
	if(pCEGUIManager)
	{
		pCEGUIManager->InitCEGUI(render::Interface::GetInstance()->GetDevice());
		pCEGUIManager->InitiliseResourceDirectories();
		pCEGUIManager->InitialiseDefaultResourceGroups();
		pCEGUIManager->InitialiseDirectInput(hWnd,pCEGUIManager->GetDirectInputState());
		//�Ƿ���ʾ֡��
		//pCEGUIManager->EnableDisplayFPS(false);
		//ע��UIClass��
		pCEGUIManager->RegisterUIClassInList();

		//��ʼ��UI��
		if(pCEGUIManager->GetUIClass("ControlGoodsView") != NULL)
			pCEGUIManager->GetUIClass("ControlGoodsView")->Init();
		if(pCEGUIManager->GetUIClass("GoodsTree") != NULL)
			pCEGUIManager->GetUIClass("GoodsTree")->Init();
		//pCEGUIManager->ExecuteScriptFile("demo8.lua");
	}
}

void GameWorld::Destroy()
{
	/////////////////////////////////////////////////
	//��ջ˳������
	/////////////////////////////////////////////////
	//�ͻ�����Դ��
	CClientResource::GetInstance()->Release();
	CClientResource::ReleaseInstance();
	//UI��
	CEGUIManager::GetInstance()->CleanupDirectInput(CEGUIManager::GetInstance()->GetDirectInputState());
	CEGUIManager::GetInstance()->ReleaseCEGUI();

	//ģ����Ⱦ��
	GoodsModelManager::GetInstance()->Destory();	
	//����ϵͳ��
	particle::Manager::GetInstance()->Destroy();
	//��Ⱦ��
	render::Interface::GetInstance()->Destroy();
	//����̨��
	console::Manager::GetInstance()->Destroy();//�ر���־

	/////////////////////////////////////////////////
	//�ͷ�
	/////////////////////////////////////////////////
	CEGUIManager::ExitInstance();
	GoodsModelManager::ExitInstance();
	particle::Manager::ExitInstance();
	render::Interface::ExitInstance();
	console::Manager::ExitInstance();
}

bool GameWorld::Update()
{
	//����Σ��ɵ͵��ߣ�ˢ��

	//�����Դװ�����
	CClientResource::GetInstance()->Run();

	//����ϵͳˢ��
	particle::Manager *pPM = particle::Manager::GetInstance();
	pPM->Update();

	//ģ��ϵͳˢ��
	GoodsModelManager::GetInstance()->Update();

	//CEGUIˢ��,���Լ���һ��dinput������Ϣ����
	CEGUIManager::GetInstance()->UpDate();

	return true;
}

void GameWorld::Render()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	pInterface->Clear(D3DCOLOR_ARGB(255,0,0,0));
	pInterface->BeginScene();

	//��Ⱦģ��
	GoodsModelManager::GetInstance()->RenderModel();
	CEGUIManager::GetInstance()->RenderCEGUI();
	pInterface->EndScene();

	//��Ⱦ����
	if (pInterface->GetDevice()->Present( NULL, NULL, NULL, NULL ) == D3DERR_DEVICELOST)
	{
		HRESULT hr = pInterface->GetDevice()->TestCooperativeLevel();
		while(hr == D3DERR_DEVICELOST)
		{
			hr = pInterface->GetDevice()->TestCooperativeLevel();
		}

		if (hr == D3DERR_DEVICENOTRESET)
		{
			hr = pInterface->GetDevice()->Reset(&pInterface->GetInfo()->d3dpp);
			if (hr != S_OK)
			{
				pInterface->Reset();
			}
		}
	}
}