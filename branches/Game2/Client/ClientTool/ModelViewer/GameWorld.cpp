// GameWorld.cpp: implementation of the GameWorld class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameWorld.h"
#include "GameSetting.h"

#include "..\..\frostengine\input.h"
#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\frostengine\ui.h"
#include "..\..\frostengine\render.h"

#include "..\..\..\public\ClientResource.h"
#include "..\..\GameEngine\GameModelManager.h"

#include "WndViewer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool g_bDebug = true;		// ���Թ��ܿ���
GameWorld * GameWorld::m_pInstance = NULL;

GameWorld* GameWorld::GetInstance()
{
	return m_pInstance;
}

void GameWorld::InitInstance()
{
	m_pInstance = new GameWorld;
}

void GameWorld::ExitInstance()
{
	delete m_pInstance;
}

BOOL GameWorld::Update()
{
	//�����Դװ�����
	CClientResource::GetInstance()->Run();

	static input::Interface::_tagEVENT stEvent;
	input::Interface * pInput = input::Interface::GetInstance();

	//������Ϣ
	pInput->UpdateInput();
	pInput->PopEvent(&stEvent);

	//����INPUT��Ϣ��UI
	ui::Manager *pUiMgr = ui::Manager::GetInstance();
	ui::Manager::_tagINPUTINFO *pInputInfo =  pUiMgr->GetInputInfoBuffer();
	pInputInfo->eType  = (UI_INPUT_MSG_TYPE)stEvent.eType;
	pInputInfo->dwData = stEvent.dwData;
	pInputInfo->wParam = stEvent.wParam;
	pInputInfo->lParam = stEvent.lParam;

	memcpy(pInputInfo->byKeyBuffer,pInput->GetKeyStateBuffer(),256);
	pInputInfo->dwMouseButtonState = pInput->GetMouseBtnStatus();
	pInput->GetMousePosition(&pInputInfo->ptMouse,&pInputInfo->ptMouseOld);

	pUiMgr->Update();

	//����ϵͳˢ��
	particle::Manager *pPM = particle::Manager::GetInstance();
	pPM->Update();

	CGameModelManager::GetInstance()->Run();

	return TRUE;
}

void GameWorld::Render()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	pInterface->BeginScene();
	
	//������Ⱦ
	ui::Manager::GetInstance()->Render();

	//��Ⱦ����
	pInterface->EndScene();
}

void GameWorld::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	//��������̨
	console::Manager::InitInstance();
	console::Manager::GetInstance()->Create(console::CONSOLE_ERROR | console::CONSOLE_OUTPUT | console::CONSOLE_WARRING);

	//��ʼ�����ӿ�
	input::Interface::InitInstance();
	ui::Manager::InitInstance();
	render::Interface::InitInstance();
	particle::Manager::InitInstance();

	CHAR * strPath = new CHAR[MAX_PATH];
	string strWorpath("");
	if(::GetCurrentDirectory(MAX_PATH, strPath))
		strWorpath=string(strPath);
	delete []strPath;
	string strFilesInfo("FilesInfo.ril");
	CClientResource::InitialInstance();
	CClientResource * pClientResource = CClientResource::GetInstance();
	pClientResource->Initial(GAME_RES,strWorpath, strFilesInfo);
	if(!pClientResource->LoadEx())
	{
		//װ�ؿͻ�����Դ��Ϣ����
		//ASSERT(false);
		//return false;
	}

	CGameModelManager::InitInstance();	
	CGameModelManager::GetInstance()->SetTextureShareFolder(Setting::GetInstance()->m_pszTextureSharFolder);

	//����
	input::Interface::GetInstance()->Create(hWnd);
	ui::Manager::GetInstance()->Create();

	//��ʼ����ʾ����
	utility::File file;
	if (!file.Open("render.config"))
	{
		OutputConsole("render.config doesn't exist.\n");
		MessageBox(NULL,"render.config","��ʧ��",MB_OK);
		PostQuitMessage(-1);
	}
	int iFileLength = file.GetLength();
	char * pszTemp = new char[iFileLength + 1];			
	memcpy(pszTemp, file.GetData(), file.GetLength());
	pszTemp[iFileLength] = 0;
	render::Interface::GetInstance()->Create(hWnd, pszTemp);
	SAFEDELETE(pszTemp);
	file.Close();

	//��ʼ�����ӹ�����
	particle::Manager::GetInstance()->Create();

	render::Interface * pInterface = render::Interface::GetInstance();
    
	int iWidth  = pInterface->GetWndWidth();
	int iHeight = pInterface->GetWndHeight();

	RECT rc= {0,0,iWidth,iHeight};
	
	m_pMainWnd = new WndViewer;
	m_pMainWnd->Create("test",&rc, FWS_VISIBLE | FWS_OUTLINE | FWS_SIZE | FWS_DRAG | FWS_FILLBKG ,NULL);

	//���ɽ���
	ui::Manager::GetInstance()->SetMainWnd(m_pMainWnd);


	//unsigned char a = 150;
	//DWORD dwA = a;
	//DWORD dw = 0x00ffffff;

	//dwA <<= 24;

	//dw |= dwA;
	//dw &= ~dwA;
	//dw ^= dwA;
}

void GameWorld::Destroy(void)
{
	SAFEDESTROY(m_pMainWnd);

	CGameModelManager::GetInstance()->Release();
	CGameModelManager::ReleaseInstance();

	CClientResource::GetInstance()->Release();
	CClientResource::ReleaseInstance();

	//����
	particle::Manager::GetInstance()->Destroy();
	CTextureProjective::Destroy();
	render::Interface::GetInstance()->Destroy();
	ui::Manager::GetInstance()->Destroy();
	input::Interface::GetInstance()->Destroy();

	//�ͷ�................................
	particle::Manager::ExitInstance();
	ui::Manager::ExitInstance();
	render::Interface::ExitInstance();
	input::Interface::ExitInstance();

	//���ٿ���̨
	console::Manager::GetInstance()->Destroy();//�ر���־
	console::Manager::ExitInstance();	
}