// LoginServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LoginServer.h"
#include "game.h"
#include "../../nets/Servers.h"

#define MAX_LOADSTRING 100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

HWND g_hWnd;
HWND g_hInfoText;
HWND g_hLogText;
HWND g_hWorldListBox;
HWND g_hCdkeyListBox;

const DWORD MAIN_WIDTH				= 1024;
const DWORD MAIN_HEIGHT				= 768;

const DWORD INFOTEXT_WIDTH			= 400;
const DWORD INFOTEXT_HEIGHT			= 300;


// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	EnableDebugNew();
	//_CrtSetBreakAlloc(170);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOGINSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_LOGINSERVER);

	unsigned threadID;
	HANDLE hGameThread  = (HANDLE)_beginthreadex(NULL,0,GameThreadFunc,NULL,0,&threadID);

	g_hGameThreadExitEvent = CreateEvent(NULL,0,0,0);

	// ����Ϣѭ��:
	while(1)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) )
			{
				g_bGameThreadExit = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //if(g_bActive)
		{
			Sleep(1);
		}
	}

	// ������־��¼
	SaveLogText(true);

	PutLogInfo("\r\n//////////////////////////////////");
	PutLogInfo("\r\n//       LoginServer Exit       //");
	PutLogInfo("\r\n//////////////////////////////////\r\n\r\n");

	// �ȴ����߳̽���
	WaitForSingleObject(g_hGameThreadExitEvent, INFINITE);
	CloseHandle(hGameThread);

	return (int) msg.wParam;
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

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_LOGINSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_LOGINSERVER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HANDLE, int)
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
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, MAIN_WIDTH, MAIN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   RECT rect;
   GetClientRect(g_hWnd, &rect);
   int w=rect.right-rect.left;
   int h=rect.bottom-rect.top;

   g_hInfoText=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,INFOTEXT_WIDTH + 2,00,rect.right- INFOTEXT_WIDTH - 2, INFOTEXT_HEIGHT-4 ,g_hWnd,0,hInst,NULL);
   g_hLogText=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,INFOTEXT_WIDTH + 2,INFOTEXT_HEIGHT,rect.right- INFOTEXT_WIDTH - 2,rect.bottom-INFOTEXT_HEIGHT,g_hWnd,0,hInst,NULL);
   g_hWorldListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,0,0,INFOTEXT_WIDTH,INFOTEXT_HEIGHT,g_hWnd,0,hInst,NULL);
   g_hCdkeyListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,0,INFOTEXT_HEIGHT,INFOTEXT_WIDTH,h-INFOTEXT_HEIGHT,g_hWnd,0,hInst,NULL);
   SendMessage(g_hLogText, EM_SETLIMITTEXT, -1, 0);

   return TRUE;
}

//
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

	switch (message) 
	{
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
			if (MessageBox(g_hWnd, "�Ƿ��˳�?", "msg", MB_YESNO)==IDYES)
				DestroyWindow(hWnd);
			break;

		// �߳��ʺ�
		case ID_MENU_KICKCDKEY:
			{

			}
			break;

		case ID_RELOADSETUP:
			GetGame()->ReLoadSetup();
			break;
		case ID_RELOADSETUPEX:
			GetGame()->ReLoadSetupEx();
			break;
		case ID_32785:
			GetGame()->ReLoadLoginSetup();
			break;
		case ID_32787:
			GetGame()->ReLoadIpLimitSetup();
			break;
		case ID_RENOQUEUECDKEYLIST:
			GetGame()->ReLoadNoQueue();
			break;
		case ID_RELOAD_WORLDOPENSTATE:
			GetGame()->ReLoadWorldSetup();
			break;
		// �ָ���½
		case ID_Menu_ResumeLogin:
			break;

		// ��ͣ��½
		case ID_Menu_PauseLogin:
			break;

		case ID_32786:
#ifdef SHOW_LOGIN_MSG_20090817
			GetGame()->OpenCloseInfo();
#endif
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

	case WM_CLOSE:
		if (1 == wParam || MessageBox(g_hWnd, "�Ƿ��˳�?", "msg", MB_YESNO)==IDYES)
			PostQuitMessage(0);
		break;

	case WM_SIZE:
		{
			long lW = LOWORD(lParam);
			long lH = HIWORD(lParam);

			MoveWindow(g_hInfoText,INFOTEXT_WIDTH + 2,0,lW-INFOTEXT_WIDTH-2,INFOTEXT_HEIGHT-4, true);
			MoveWindow(g_hLogText,INFOTEXT_WIDTH + 2,INFOTEXT_HEIGHT,lW-INFOTEXT_WIDTH-2,lH-INFOTEXT_HEIGHT, true);
			MoveWindow(g_hWorldListBox, 0,0, INFOTEXT_WIDTH, INFOTEXT_HEIGHT, true);
			MoveWindow(g_hCdkeyListBox, 0, INFOTEXT_HEIGHT, INFOTEXT_WIDTH, lH-INFOTEXT_HEIGHT, true);			
		}
	default:
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

// ������־
void SaveLogText(bool bForce)
{
	const int lMaxLength = 64000;	// log��Ϣ��󳤶�
	static char str[lMaxLength];

	// д��־�ļ�
	static DWORD dwLastSaveTime = timeGetTime();

	if (bForce ||											// ǿ��д
		timeGetTime() - dwLastSaveTime > GetGame()->GetSetup()->dwSaveInfoTime ||		// ����ʱ����
		GetWindowTextLength(g_hLogText) >= lMaxLength		// ���ݳ���64K
		)
	{
		dwLastSaveTime = timeGetTime();

		PutLogInfo("\r\n=============================== Start Save Log ===============================\r\n");

		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d\r\n\r\n", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
		PutLogInfo(str);

		GetWindowText(g_hInfoText, str, lMaxLength);
		PutLogInfo(str);
		PutLogInfo("\r\n");
		SetWindowText(g_hLogText, "");

		PutLogInfo("================================ End Save Log ================================\r\n");
	}
}

// ����һ����¼
void AddLogText(char* msg, ...)
{
	const int lMaxLength = 64000;	// log��Ϣ��󳤶�
	static char str[lMaxLength];

	// д��־�ļ�
	SaveLogText(false);

	// д����������
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(str, "[%02d-%02d %02d:%02d:%02d] ", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	va_list va;
	va_start(va,msg);
	vsprintf(&str[strlen(str)],msg,va);
	va_end(va);

	strcat(str, "\r\n");

	PutLogInfo(str);

	SendMessage(g_hLogText, EM_SETSEL, 0, -1);
	SendMessage(g_hLogText, EM_SETSEL, -1, 0);
	SendMessage(g_hLogText, EM_REPLACESEL, FALSE, (LPARAM)str);
}

// ˢ�·�������Ϣ
void RefeashInfoText()
{
	static char str[102400];

	if (!GetGame() || !GetGame()->GetNetServer_Client())
	{
		return;
	}

	static long lMaxConnects = 0, lMaxCdkeys = 0, lMaxPWErrorCdkeys = 0;
	long lConnects = GetGame()->GetNetServer_Client()->GetClientCount();

	long lCdkeys = GetGame()->GetLoginWorldCdkeyNumbers();

	long lPWErrorCdkeys = GetGame()->GetLoginManager().GetFrozenCdkeyNum();

	if (lConnects>lMaxConnects)
		lMaxConnects = lConnects;
	if (lCdkeys>lMaxCdkeys)
		lMaxCdkeys = lCdkeys;
	if (lPWErrorCdkeys>lMaxPWErrorCdkeys)
		lMaxPWErrorCdkeys=lPWErrorCdkeys;

	sprintf(str, "Connects = %d/%d  Cdkeys = %d/%d  PWErrorCdkeys = %d/%d",
				lConnects, lMaxConnects,
				lCdkeys, lMaxCdkeys,
				lPWErrorCdkeys, lMaxPWErrorCdkeys);

	// ��ʾÿ��World������
	GetGame()->WsPlayerNumIntoStringFoot(str);
			
	SetWindowText(g_hInfoText, str);
}
