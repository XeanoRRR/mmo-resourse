// GameClient.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GameClient.h"
#include "game.h"
#include "../GameClient/ClientApp/ClientWordsFilter.h"
#include "../GameClient/ClientApp/ClientRegion.h"
#include "../GameClient/ClientApp/MiniMap.h"
#include "../Engine/Utility.h"
#include "../Engine/Console.h"

#include "../EngineExtend/JpegEncode.h"
#include "../EngineExtend/TextureProjective.h"

#include "../GameClient/ClientApp/Other/Video.h"
#include "../GameClient/ClientApp/Other/VideoManager.h"
#include "../ui/UIManager.h"

#pragma comment (lib,"imm32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HWND	g_hWnd;									// ���ھ��
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
BYTE  g_cPressedKey;
DEVMODE    g_lpDevMode;		// ԭʼ��������
bool	   g_bIsInitialized = false;
bool	   g_bIsFocus		= false;
bool	   g_bIsMinimize	= false;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


ulong new_timer = GetCurTickCount();
ulong old_timer = new_timer;

char	dupfilename[256] = "";


int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{

	EnableDebugNew(); 
	//	_CrtSetBreakAlloc(218019);

	// TODO: �ڴ˷��ô��롣
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMECLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	InitialDebugFileName();

	CreateGame();


	Log4c::Trace(ROOT_MODULE,"CreateGame ok");

	GetGame()->LoadSetup();
	Log4c::Trace(ROOT_MODULE,"LoadSetup ok");

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GAMECLIENT);

	// ��Ϸ��ʼ��
	GetGame()->Init();

	g_bIsInitialized = true;
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	render::Interface *pInterface = render::Interface::GetInstance();


	// ��ѭ��:
	while(1)
	{
#ifndef _DEBUG
		__try{
#endif	

#ifdef _RUNSTACKINFO_
			CMessage::AsyClearFile(GetGame()->GetStatckFileName());
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame Loop Start.");
#endif
			static DWORD dwLoopTime = 0,dwDisplayTime = 0;
			DWORD dwCurTime1 = timeGetTime();
			int ret = GameLoop();
            static int ret2 = ret;
            if (ret2 != ret)
            {
                ret2 = ret2;
            }
			DWORD dwCurTime2 = timeGetTime();
			switch(ret)
			{
			case 0:	// �˳���Ϸ
				{
					goto _exit;
				}
				break;

			case 1:	// ִ����ϵͳ��Ϣ
                {
                    int a = 0;
                }
				break;

			case 2:	// ִ������Ϸ
				{

					static bool bLimitFPS = true;

					// ����������
					if( g_bDebug && GetKeyboard() && GetKeyboard()->IsKeyDown(DIK_SPACE) )
						bLimitFPS = false;
					else	// ����
						bLimitFPS = true;

                    if (GameDisplay(bLimitFPS) == 1)
                    {
                        GameUpdateScreen();
                    }
                    if (bLimitFPS)
                    {
                        GameLimitFps();
                    }
				}
				break;
			}
			DWORD dwCurTime3 = timeGetTime();
			dwLoopTime += dwCurTime2-dwCurTime1;
			dwDisplayTime += dwCurTime3-dwCurTime2;
#ifdef _RUNSTACKINFO_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame Loop End.");
#endif

#ifndef _DEBUG
		}
		__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode(),dupfilename))
		{
			char execommand[1024]= "";
			sprintf_s<1024>(execommand,"Error.exe -d ");
			strcat_s<1024>(execommand,dupfilename);
			strcat_s<1024>(execommand," -b ");
			strcat_s<1024>(execommand,GetDebugFileName());
			strcat_s<1024>(execommand," -h 127.0.0.1");
			strcat_s<1024>(execommand," -p 2348");
			::WinExec(execommand,true);
			break;
		}
#endif
	}

_exit:

	GetGame()->Release();
	DeleteGame();

	ChangeDisplaySettingsEx(NULL,&g_lpDevMode,NULL,0,NULL);

	return 0;
}

// -------------------------------------------------------
// Name: GameLoop()
// Describe: ѭ�h
// -------------------------------------------------------
long GameLoop(bool bControl)
{
	MSG msg;
	if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if( !GetMessage( &msg, NULL, 0, 0 ) )
		{
			return 0;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return 1;
	}
	else //if(g_bActive)
	{
		//#ifdef _DEBUG
		//		static char str[256];
		//		static long dwStart;
		//		dwStart = GetCurTickCount();
		//#endif

		static DWORD dwProceMsgTime = 0,dwRunTime = 0;
		DWORD dwCurTime1 = timeGetTime();
		GetGame()->ProcessMessage();
		DWORD dwCurTime2 = timeGetTime();
		GetGame()->Run(bControl);
		DWORD dwCurTime3 = timeGetTime();

		dwProceMsgTime += dwCurTime2-dwCurTime1;
		dwRunTime += dwCurTime3-dwCurTime2;
		g_cPressedKey=0;
		//#ifdef _DEBUG
		//		static long dwEnd;
		//		dwEnd = GetCurTickCount();
		//		if (dwEnd - dwStart > 10)
		//		{
		//			sprintf(str, "RunTime: %d\n", dwEnd - dwStart);
		//			OutputDebugString(str);
		//		}
		//#endif
	}

	return 2;
}



/*
* ����: ��Ϸ��ʾ
* ժҪ: -
* ����: bLimitFPS - �Ƿ�����֡��
* ����ֵ: long
* ����: -
* ��������: -
* �޸���־:
*	2008.05.28 - lpf
*		�����˵��ͻ�����С��ʱ,ִ��һ������Ҷ�������
*	2008.06.04 - lpf
*		�޸��˸�������MoveShape�����ʱ��
*	2008.10.20 - lpf
*		�����������С������ķ�ʽ,��ȡ����ԭ����"ִ��һ������Ҷ�������"
*/
long GameDisplay(bool bLimitFPS)
{
	new_timer = GetCurTickCount();
	if(!bLimitFPS || 
		new_timer - old_timer < GetGame()->GetSetup()->ulFrameControl)
	{
		GetGame()->UpdateMoveShapePositionEx();
	}

	// ��С�����ڲ���ʾ
	if ((g_bIsMinimize)&&!GetGame()->GetSetup()->bupdateonlostfoucs)
	{
		// ������Ⱦ������Ϊ���ͷ���������
		//��Ⱦ����
		particle::Manager::GetInstance()->Render(1.0,false);
		GetGame()->UpdateAnimation();
		return 0;
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"particle::Manager::GetInstance() Start.");
#endif

	if(particle::Manager::GetInstance())
		particle::Manager::GetInstance()->Update();
	render::Interface *pInterface = render::Interface::GetInstance();
	pInterface->Clear(GetGame()->GetBackColor());
	return GetGame()->Display();

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"particle::Manager::GetInstance() End.");
#endif
}



void GameUpdateScreen()
{
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GameUpdateScreen Start.");
#endif
    render::Interface *pInterface = render::Interface::GetInstance();
    //ˢ����Ļ
    pInterface->EndScene();
	if ((!g_bIsMinimize) || GetGame()->GetSetup()->bupdateonlostfoucs/*!(GetWindowLong(g_hWnd,GWL_STYLE)&WS_MINIMIZE)*/)
	{	
		if (FAILED(pInterface->GetDevice()->Present( NULL, NULL, NULL, CVideoManager::GetInstance()->GetClipList() )))
		{

			HRESULT hr = pInterface->GetDevice()->TestCooperativeLevel();
			if (hr == D3DERR_DEVICELOST)
			{
				if (GetGame())
				{
					if (GetGame()->GetRegion())
					{
						GetGame()->GetRegion()->GetMiniMap()->DestroyDirect3DResource();
					}
				}
				
				CTextureProjective::CheckReSetRelease();
				pInterface->ResetRelease();
				pInterface->SetDeviceLost(TRUE);
				/* ֪ͨIGW d3d�豸��ʧ */
				GetGame()->IGWOnLostDevice();
				return;
			}
			if (hr == D3DERR_DEVICENOTRESET)
			{
				if (FAILED(pInterface->GetDevice()->Reset(&pInterface->GetInfo()->d3dpp)))
				{
					if (pInterface->Reset())
					{
						pInterface->ResetRelease();
						pInterface->ResetRebuild();
						pInterface->SetDeviceLost(FALSE);
						CTextureProjective::CheckReSetRelease();
						CTextureProjective::CheckReSetReBuild();
						GetGame()->IGWOnResetDevice();
						
						if (GetGame())
						{
							if (GetGame()->GetRegion())
							{
								GetGame()->GetRegion()->GetMiniMap()->DestroyDirect3DResource();
								GetGame()->GetRegion()->GetMiniMap()->ResetDirect3DResource();
							}
						}
					}
				}
				else
				{
					pInterface->SetDeviceLost(FALSE);
					pInterface->ResetRebuild();
					GetGame()->IGWOnResetDevice();
					CTextureProjective::CheckReSetReBuild();
					
					if (GetGame())
					{
						if (GetGame()->GetRegion())
						{
							GetGame()->GetRegion()->GetMiniMap()->ResetDirect3DResource(); 
						}
					}
				}
			}
		}
		else
		{
			pInterface->SetDeviceLost(FALSE);

		}
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GameUpdateScreen End.");
#endif

}


void GameLimitFps()
{
	new_timer = GetCurTickCount();
	if (new_timer - old_timer < GetGame()->GetSetup()->ulFrameControl )
	{
		WaitForSingleObject(GetCurrentProcess(), GetGame()->GetSetup()->ulFrameControl - (new_timer - old_timer));
	}
	old_timer = GetCurTickCount(); 
}

//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��: 
//
//    ����ϣ��������ӵ� Windows 95 ��
//    ��RegisterClassEx������֮ǰ�˴����� Win32 ϵͳ����ʱ��
//    ����Ҫ�˺��������÷������ô˺���
//    ʮ����Ҫ������Ӧ�ó���Ϳ��Ի�ù�����
//   ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GAMECLIENT);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	//(LPCTSTR)IDC_GAMECLIENT;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}



/*
* ����: ����ʵ�����������������
* ժҪ: �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
*       ��������ʾ�����򴰿ڡ�
* ����: -
* ����ֵ: TRUE	 - ��ʼ���ɹ�
*		   FALSE - ��ʼ��ʧ��
* ����: -
* ��������: -
* �޸���־:
*	2008.05.29 - lpf
*		�����˿ͻ��˳�����󻯰�ť�ʹ���ߴ�ı�
*	2008.05.30 - lpf
*		"�����˿ͻ��˳�����󻯰�ť�ʹ���ߴ�ı�"���ָ��˻�����
*	2009.03.09 - lpf
*		�ٴ������˿ͻ��˳�����ߴ�ı�(��󻯰�ť���ܱ���)
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//   HWND hWnd;

	SCREEN_WIDTH = GetGame()->GetSetup()->lWidth;
	SCREEN_HEIGHT = GetGame()->GetSetup()->lHeight;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	// ȫ��
	if (GetGame()->GetSetup()->bFullscreen)
	{
		// �ı�����ֱ���
		g_lpDevMode.dmSize=sizeof(DEVMODE);
		EnumDisplaySettings(NULL,ENUM_REGISTRY_SETTINGS,&g_lpDevMode);

		DEVMODE    lpDevMode;
		memset(&lpDevMode, 0, sizeof(lpDevMode));
		lpDevMode.dmBitsPerPel=GetGame()->GetSetup()->lBitsPerPel;	//Change    from    24    to    16
		lpDevMode.dmPelsWidth=SCREEN_WIDTH;
		lpDevMode.dmPelsHeight=SCREEN_HEIGHT;
		lpDevMode.dmDisplayFrequency=GetGame()->GetSetup()->lFrequency;
		lpDevMode.dmSize=sizeof(DEVMODE);
		lpDevMode.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL|DM_DISPLAYFREQUENCY;
		LONG    result;
		result = ChangeDisplaySettingsEx(NULL,&lpDevMode,NULL,0,NULL);
		if (result==DISP_CHANGE_SUCCESSFUL)
		{
		}
		else
		{
			result=ChangeDisplaySettingsEx(NULL,&lpDevMode,NULL,0,NULL);
		}

		g_hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP|WS_CLIPCHILDREN,
			0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
		if (!g_hWnd)
		{
			return FALSE;
		}
	}
	else	// ����
	{
		// �������ڴ�С
		RECT rect;
		SetRect(&rect, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

		g_hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, NULL);
        
		if (!g_hWnd)
		{
			return FALSE;
		}

		RECT  rcWork;
		RECT  rc;
		DWORD dwStyle;
		//// If we are still a WS_POPUP window we should convert to a normal app
		//// window so we look like a windows app.
		dwStyle  = GetWindowLong( g_hWnd, GWL_STYLE );
		dwStyle &= ~WS_POPUP;
		dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		SetWindowLong( g_hWnd, GWL_STYLE, dwStyle );

		//// set window size
		SetRect( &rc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		AdjustWindowRectEx( &rc, GetWindowLong(g_hWnd, GWL_STYLE), GetMenu(g_hWnd) != NULL, GetWindowLong(g_hWnd, GWL_EXSTYLE) );
		//		SetRect( &rc, 0, 0, m_nScreenWidth+(rc.right-rc.left), m_nScreenHeight+(rc.bottom-rc.top) );
		SetWindowPos( g_hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
		SetWindowPos( g_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

		// Make sure our window does not hang outside of the work area
		SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
		//// Calculate window position in desktop
		////08.09.10 kaixuan
		////ʹ��Ϸ����ʼ�ձ�����������֮�ϡ�
		int CaptionHeight = GetSystemMetrics(SM_CYCAPTION);			//��ǰ�������߶�
		int nPosX = (GetSystemMetrics(SM_CXSCREEN)-SCREEN_WIDTH) / 2;
		int nPosY = (GetSystemMetrics(SM_CYSCREEN)-SCREEN_HEIGHT) / 2;
		GetWindowRect( g_hWnd, &rc );
		if( rc.left < rcWork.left ) rc.left = rcWork.left;
		if( rc.top  < rcWork.top )  rc.top  = rcWork.top;
		long		lSystemHeight = GetSystemMetrics(SM_CYSCREEN);		//��ǰ����ϵͳ��Ļ�߶�
		long    lTaskBarHeight = lSystemHeight - rcWork.bottom;			//�������߶�
		if (nPosY + SCREEN_HEIGHT >  lSystemHeight - lTaskBarHeight)
			nPosY -= lTaskBarHeight;
		SetWindowPos( g_hWnd, NULL, nPosX, nPosY, 0, 0,	SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
	}

	return TRUE;
}

//  ����: WndProc(HWND, unsigned, WORD, LONG)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
    extern bool g_bInIME;
   
	if(!GetGame()->OnWindowProc( hWnd, message, wParam, lParam))
	{
		return true;
	}
    //�Ƿ����˽�����Ϣ nikai
    bool isProcessedWinMsgInUIsystem = false;
	//extern BYTE g_cPressedKey;
	static BOOL bIsFullSreenMode = false;
	//if (GetGame() && GetGame()->GetSetup())
	//	bIsFullSreenMode = GetGame()->GetSetup()->bFullscreen;
	//static char strImeName[128];	//���뷨����
	switch (message) 
	{
    case WM_CHAR:
	case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_LBUTTONDBLCLK:
        // ��IO�¼�����Game������
        if (!UIManager::IOProc(message, wParam, lParam) && GetGame() && g_bIsFocus)
            GetGame()->PushWinMessage(message, wParam, lParam);
        break;

	case WM_CREATE:			//��������ʱ�������Ϣ������Ĭ�����ָ��
		{
			HCURSOR hCursor=LoadCursor(NULL,IDC_ARROW);
			SetCursor(hCursor);
		}
		break;

	case WM_SETCURSOR:		//���ô������ָ�����Ϣ������Ϊ��ǰ��ָ��
		{
			static RECT rect;
			static POINT pt;
			GetClientRect(hWnd,&rect);
			GetCursorPos(&pt);
			ScreenToClient(hWnd,&pt);
			if( PtInRect(&rect,pt) )//����Ϊ��ǰ����ָ��
			{
				SetCursor(GetGame()->GetMouseCursor());
				return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
        ValidateRect( g_hWnd, NULL );
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
			GetGame()->QuitGame();
		break;

	case WM_CLOSE:
		{
			// ���ڴ�����С����ֱ���˳�
			if(IsIconic(hWnd))
				PostQuitMessage(0);
			else
				GetGame()->QuitGame();
		}
		break;

		// ��������
	case WM_SIZE:
		{
			WORD w = LOWORD(lParam);
			WORD h = HIWORD(lParam);
			WINDOW_WIDTH = w;
			WINDOW_HEIGHT = h;
		}
		break;
	case WM_MOVE:
		if (g_bIsInitialized)
		{
			GameLoop();

			if (GameDisplay(true) == 1)
				GameUpdateScreen();

			GameLimitFps();
		}
		return 0;
	case WM_ERASEBKGND:
		if (g_bIsInitialized)
		{
			/*GameLoop();

			if (GameDisplay(true) == 1)
			GameUpdateScreen();

			GameLimitFps();*/
		}
		return 0;
	case WM_KILLFOCUS:
		g_bIsFocus = false;
		break;
	case WM_SETFOCUS:
		g_bIsFocus = true;
		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU)
		{
			return 0;
		}
		else if (wParam == SC_MINIMIZE)
		{
			g_bIsMinimize = true;
		}
		else if (wParam == SC_MAXIMIZE || wParam == SC_RESTORE)
			g_bIsMinimize = false;

		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_INPUTLANGCHANGE:
		//OutputDebugStr("WM_INPUTLANGCHANGE\n");
		if (GetGame() && g_bIsFocus)
		{
			//if (bIsFullSreenMode == false)
				//GetGame()->PushWinMessage(message, wParam, lParam);
			//else
			//{
				//GetGame()->GetCGuiEx()->GetImeBarPage()->OnImeChanged();
				//sprintf_s<128>(strImeName,"������ʹ�õ��ǣ�%s",GetGame()->GetCGuiEx()->GetImeBarPage()->GetCurImeName());
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER, strImeName, 0xffff0000);
			//}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_IME_STARTCOMPOSITION:
		//OutputDebugStr("WM_IME_STARTCOMPOSITION\n");
        g_bInIME = true;
		if (GetGame() && g_bIsFocus)
		{
			if (bIsFullSreenMode)
            {
                //GetGame()->GetCGuiEx()->GetImeBarPage()->Open();
            }
            else
			    return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_IME_COMPOSITION:
		//OutputDebugStr("WM_IME_COMPOSITION\n");
        g_bInIME = false;
		if (GetGame() && g_bIsFocus)
		{
			if (bIsFullSreenMode)
			{
				//GetGame()->GetCGuiEx()->GetImeBarPage()->UpdateTypeCharArray(lParam);
				//GetGame()->GetCGuiEx()->GetImeBarPage()->ProcessEndComposition(lParam);
			}
			else
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		//OutputDebugStr("WM_IME_ENDCOMPOSITION\n");
		if (GetGame() && g_bIsFocus)
		{
			if (bIsFullSreenMode)
            {
                //GetGame()->GetCGuiEx()->GetImeBarPage()->Close();
            }
            else
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_IME_NOTIFY:	//This message is sent to an application to notify it of changes to the IME window.
		//An application processes this message if it is responsible for managing the IME window.
		//OutputDebugStr("WM_IME_NOTIFY\n");
		if (GetGame() && g_bIsFocus)
		{
			switch(wParam)
			{
			case IMN_CHANGECANDIDATE:
				//OutputDebugStr("IMN_CHANGECANDIDATE\n");
                if (bIsFullSreenMode)
                {
                    //GetGame()->GetCGuiEx()->GetImeBarPage()->UpdateCandidateList(lParam);
                }
                else
                {
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
				break;
			case IMN_OPENCANDIDATE:
				if (bIsFullSreenMode)
                {
                    //GetGame()->GetCGuiEx()->GetImeBarPage()->UpdateCandidateList(lParam);
                }
                else
				{
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
				break;
			case IMN_SETSENTENCEMODE:
			case IMN_OPENSTATUSWINDOW:
			case IMN_SETSTATUSWINDOWPOS:
			case IMN_CLOSECANDIDATE:
			case IMN_CLOSESTATUSWINDOW:
			case IMN_GUIDELINE:
			case IMN_SETCANDIDATEPOS:
			case IMN_SETCOMPOSITIONFONT:
			case IMN_SETCOMPOSITIONWINDOW:
			case IMN_SETCONVERSIONMODE:
			case IMN_SETOPENSTATUS:
			default:
				if(bIsFullSreenMode == false)
				{	
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
		}
		break;
	case WM_ACTIVATEAPP:	// ʹ��ALT+TAB�л�����
		// �л���
		g_bIsFocus = wParam>0?true:false;
		if(!wParam)
		{
			message = WM_CHAR;
			wParam = '\t';
			lParam = -99999;
		}

	default:
		if (GetGame() && g_bIsFocus)
		{
			//GetGame()->PushWinMessage(message, wParam, lParam);
		}
        
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// �����ڡ������Ϣ�������
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
