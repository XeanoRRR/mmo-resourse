#include "stdafx.h"
#include "fengine.h"
#include "futility.h"
#include "fconsole.h"
#include "finput.h"
#include "fparticle.h"
#include "fui.h"
#include "frender.h"


IMPLEMENT_SINGLETON(engine::Manager);

engine::Manager::Manager(void)
{
}

engine::Manager::~Manager(void)
{
}

BOOL engine::Manager::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	//��������̨
	console::Manager::InitInstance();
	console::Manager::GetInstance()->Create(console::CONSOLE_ERROR |
		console::CONSOLE_OUTPUT | console::CONSOLE_WARRING);
	
	//��ʼ�����ӿ�
	input::Interface::InitInstance();
	ui::Manager::InitInstance();
	render::Interface::InitInstance();
	particle::Manager::InitInstance();

	//����
	input::Interface::GetInstance()->Create(hWnd);
	ui::Manager::GetInstance()->Create();
	//��ʼ����ʾ����
	utility::File file;
	if (!file.Open("frost.engine.config"))
	{
        OutputConsole("Engine config file \"frost.engine.config\" doesn't exist.\n");
		PostQuitMessage(-1);		
		return FALSE;
	}
	int iFileLength = file.GetLength();
	char *pszTemp = new char[iFileLength + 1];			
	memcpy(pszTemp,file.GetData(),file.GetLength());
	pszTemp[iFileLength] = 0;
	render::Interface::GetInstance()->Create(hWnd,pszTemp);
	SAFEDELETE(pszTemp);
	file.Close();
	//��ʼ�����ӹ�����
	particle::Manager::GetInstance()->Create();

	//д�����汾����
#ifdef _DEBUG
	OutputConsole("<FONT style=\"BACKGROUND-COLOR: lavender\">Engine Compiling:<FONT COLOR=\"#4f2000\">_DEBUG</FONT></FONT>\n");
#else
	OutputConsole("<FONT style=\"BACKGROUND-COLOR: lavender\">Engine Compiling:<FONT COLOR=\"#4f2000\">_RELEASE</FONT></FONT>\n");
#endif

	
	
	return TRUE;	
}

void engine::Manager::Destroy()
{
	//SAFEDELETE(m_pFrustum);

	//����
	particle::Manager::GetInstance()->Destroy();
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

//������Windows��Ϣ
BOOL engine::Manager::WindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage) 
	{
	case WM_ACTIVATE:
			m_bWndActive = LOWORD(wParam);  
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:		
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case 0x020a:
	case WM_CHAR:		
		{
			input::Interface *pInput = input::Interface::GetInstance();
			if (pInput)
			{
				pInput->ParseMsg(uMessage,wParam,lParam);
			}
		}
		break;
	default:
		return FALSE;//δ�����κ���Ϣ
	}

	return TRUE;
}

void engine::Manager::Update()
{
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
}

void engine::Manager::Render()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	pInterface->BeginRender();

	//������Ⱦ
	particle::Manager::GetInstance()->Render();
	
	//������Ⱦ
	ui::Manager::GetInstance()->Render();	
	
	//��Ⱦ����
	pInterface->EndRender();
}
void engine::Manager::SetWnd(void * pWnd)
{
	ui::Manager::GetInstance()->SetMainWnd((ui::Wnd*)pWnd);
}

//void engine::Manager::SetActiveCamera(render::Camera *pCamera)
//{
//	//���㵱ǰʹ�õ��������Frustum
//	m_pFrustum->CalculateFrustum(pCamera->GetViewMatrix(),pCamera->GetProjectionMatrix());
//}
