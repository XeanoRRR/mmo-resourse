// GoodsViewer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GoodsViewer.h"
#include <shellapi.h>
#include <CommDlg.h>
#include "GameSetting.h"
#include "GameWorld.h"
#include "GoodsModelManager.h"
#include "..\CEGUIEngine\CEGUIManager.h"

#include <vld.h>

#define MAX_LOADSTRING 100
// ȫ�ֱ���:
WNDCLASSEX wcex;
HINSTANCE hInst = NULL;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

BOOL	  bActive   = FALSE;
HWND	  hWndMain	= NULL;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL                ExitInstance(HINSTANCE hInstance);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	//HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GOODSVIEWER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	ZeroMemory(&msg,sizeof(msg));
	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if(!IsIconic(hWndMain))//!< ����δ��С��
			{
				//���������Ⱦ�ĺ���
				GameWorld::GetInstance()->Update();
				GameWorld::GetInstance()->Render();
			}
		}
	}

	//�˳���Ĵ���
	ExitInstance(hInstance);
	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GOODSVIEWER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GOODSVIEWER);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWndMain=hWnd = CreateWindow(szWindowClass, szTitle,WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	   0,0,1,1, 
	   NULL, NULL, hInstance, NULL);
  
   DragAcceptFiles(hWndMain,TRUE);//֧���ļ���ק

   if (!hWnd)
   {
      return FALSE;
   }

   Setting::InitInstance();
   GameWorld::InitInstance();

   if (!Setting::GetInstance()->LoadSetting())
   {
	   Setting::ExitInstance();
	   return FALSE;
   }

   GameWorld::GetInstance()->Create(hWndMain);

   ShowWindow(hWndMain, nCmdShow);
   UpdateWindow(hWndMain);
   OutputConsole("GameWorld Created.\n");

   return TRUE;
}

//--------------------------------------------------------------------
// Name: ExitInstance()
// Desc: 
//--------------------------------------------------------------------
BOOL ExitInstance(HINSTANCE hInstance)
{
	OutputConsole("GameWorld Class Destroyed.\n");

	Setting::GetInstance()->SaveSetting();
	Setting::ExitInstance();

	GameWorld::GetInstance()->Destroy();
	GameWorld::ExitInstance(); 

	UnregisterClass(wcex.lpszClassName,hInstance);

	return TRUE;
}


//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
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

	//����CEGUI �� WndProc���� �����Լ���һ��Dinput��Ӧ
	CEGUIManager* pUIManager = CEGUIManager::GetInstance();
	if (pUIManager)
	{
		pUIManager->ParseMsg(message,wParam,lParam);
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPEN://��ģ��
			{
				OPENFILENAME ofn;								// common dialog box structure
				static char szFile[_MAX_PATH] = "model";		// buffer for file name

				// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWndMain;
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "Model File (*)\0*\0All Files\0*.*\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = "model";
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 
				static char szOldDirectory[_MAX_PATH];
				GetCurrentDirectory(_MAX_PATH ,szOldDirectory);
				if (GetOpenFileName(&ofn)==TRUE)
				{
					SetCurrentDirectory(szOldDirectory);
					GoodsModelManager::GetInstance()->OpenModel(szFile,true);
				}
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DROPFILES:
		{
			char szQueryFile[1024] = { 0 };
			if (DragQueryFile((HDROP)wParam, 0, szQueryFile, 1023) != 0xFFFFFFFF)
			{
				//�����Ŀ¼?
				if (GetFileAttributes(szQueryFile) & FILE_ATTRIBUTE_DIRECTORY)
					strcat(szQueryFile,"\\model");
				GoodsModelManager::GetInstance()->OpenModel(szQueryFile,true);
			}
		}
		break;
	case 0x020A:  //WM_MOUSEWHEEL: �������ڲ���ģ�����������֮��ŵ�CEGUI����Ϣ��Ӧ��
		//GoodsModelManager::GetInstance()->MoveCamera(0.0f,static_cast<float>( (short)HIWORD(wParam) ) / static_cast<float>(1200),static_cast<float>( (short)HIWORD(wParam) ) / static_cast<float>(1200));
		/*CGoodsModelManager::GetInstance()->ScaleCumulate(static_cast<float>( (short)HIWORD(wParam) ) / static_cast<float>(1200),\
												 static_cast<float>( (short)HIWORD(wParam) ) / static_cast<float>(1200),\
												 static_cast<float>( (short)HIWORD(wParam) ) / static_cast<float>(1200));*/
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

