// BillingServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#ifdef _MYDEBUG
#include <vld.h>
#endif

#include "BillingServer.h"
#include "Game.h"
//#include "../../nets/netserver/MyNetServer.h"
//#include "../../nets/netserver/MyNetClient.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
bool				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND g_hWnd;							// ������
HWND g_hWndInfo;						// ��Ϣ���ڣ����ϣ�
HWND g_hWndLog;							// ��־�鿴�����£�
HWND g_hWndGSList;						// GS�б��ң�

long g_lInfoWindowHeight = 140;			// ��Ϣ��ʾ���ڵĴ�С

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	// TODO: �ڴ˷��ô��롣
	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BILLINGSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BILLINGSERVER);

	// ΪApp�������ӵ���
	EnableDebugNew();

	InitialDebugFileName();

	MSG msg;

	unsigned threadID;
	HANDLE  hGameThread = (HANDLE)_beginthreadex(NULL,0,GameThreadFunc,NULL,0,&threadID);

	g_hGameThreadExitEvent = CreateEvent(NULL,0,0,0);

#ifndef _DEBUG
	__try
	{
#endif

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
		else
		{
			Sleep(1);
		}
	}

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		MessageBox(g_hWnd, "The main thread occured some fatal error!Please check out the error report file!", "Error", MB_OK);

		PostMessage(g_hWnd, WM_DESTROY, 0, 0);
		SetEvent(g_hGameThreadExitEvent);
		_endthreadex( 0 );
		return 0;
	}
#endif

	//������־��¼
	SaveLogText(true);

	PutLogInfo("\r\n/////////////////////////////////////");
	PutLogInfo("\r\n//       BillingServer Exit        //");
	PutLogInfo("\r\n/////////////////////////////////////\r\n\r\n");

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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_BILLINGSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_BILLINGSERVER;
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
bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 800, 600, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow( g_hWnd, nCmdShow );
	UpdateWindow( g_hWnd );

	RECT rectWnd;
	GetClientRect( g_hWnd, &rectWnd );
	long lWidth		= rectWnd.right - rectWnd.left;
	long lHeight	= rectWnd.bottom - rectWnd.top;

	g_hWndInfo		= CreateWindow( "EDIT", "", WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY|LBS_STANDARD,
		0, 0, rectWnd.right-200, g_lInfoWindowHeight-4, g_hWnd, 0, hInst, NULL );
	g_hWndLog		= CreateWindow( "EDIT", "", WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY|LBS_STANDARD,
		0, g_lInfoWindowHeight, rectWnd.right-200, rectWnd.bottom-g_lInfoWindowHeight, g_hWnd, 0, hInst, NULL );
	g_hWndGSList	= CreateWindow( "LISTBOX", "", WS_CHILD|WS_VISIBLE|LBS_STANDARD,
		rectWnd.right-200, 0, 200, rectWnd.bottom, g_hWnd, 0, hInst, NULL );
	SendMessage( g_hWndLog, EM_SETLIMITTEXT, -1, 0 );

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
			if (MessageBox(g_hWnd, "Sure To EXIT?", "msg", MB_YESNO)==IDYES)
				DestroyWindow(hWnd);
			break;

		case ID_LOAD_GSLIST:
			{
				GetGame()->LoadGSSetup();
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		//case ID_RELOADSETUP:
		//	GetGame()->ReLoadSetup();
		//	break;
		//case ID_RELOADSETUPEX:
		//	GetGame()->ReloadSetupEx();
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		if (MessageBox(g_hWnd, "Sure To EXIT?", "msg", MB_YESNO)==IDYES)
			PostQuitMessage(0);
		break;

	case WM_SIZE:
		{
			long lW = LOWORD(lParam);
			long lH = HIWORD(lParam);
			MoveWindow( g_hWndInfo, 0, 0, lW-160, g_lInfoWindowHeight-4, true );
			MoveWindow( g_hWndLog, 0, g_lInfoWindowHeight, lW-160, lH-g_lInfoWindowHeight, true );
			MoveWindow( g_hWndGSList, lW-160,0,160,lH, true );
		}
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
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
		timeGetTime() - dwLastSaveTime > GetGame()->GetSetup()->dwSaveInfoTime ||		// ����ʱ��
		GetWindowTextLength(g_hWndLog) >= lMaxLength		// ���ݳ���64K
		)
	{
		dwLastSaveTime = timeGetTime();

		PutLogInfo("\r\n=============================== Start Save Log ===============================\r\n");

		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d\r\n\r\n", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
		PutLogInfo(str);

		GetWindowText(g_hWndInfo, str, lMaxLength);
		PutLogInfo(str);
		PutLogInfo("\r\n");
		SetWindowText(g_hWndLog, "");
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

	SendMessage(g_hWndLog, EM_SETSEL, 0, -1);
	SendMessage(g_hWndLog, EM_SETSEL, -1, 0);
	SendMessage(g_hWndLog, EM_REPLACESEL, FALSE, (LPARAM)str);
}

// ����һ�������¼
void AddErrorLogText(char* msg, ...)
{
	const int lMaxLength = 64000;	// log��Ϣ��󳤶�
	static char str[lMaxLength];

	// д��־�ļ�
	SaveLogText(false);

	// д����������
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(str, "[%02d-%02d %02d:%02d:%02d] <error> ", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	va_list va;
	va_start(va,msg);
	vsprintf(&str[strlen(str)],msg,va);
	va_end(va);

	strcat(str, "\r\n");

	PutLogInfo(str);

	SendMessage(g_hWndLog, EM_SETSEL, 0, -1);
	SendMessage(g_hWndLog, EM_SETSEL, -1, 0);
	SendMessage(g_hWndLog, EM_REPLACESEL, FALSE, (LPARAM)str);
}

// ˢ�·�������Ϣ
void RefreashInfo()
{
	static char str[1024] = "Waitting For Filling";

	if (!GetGame() || !GetGame()->GetServerForGS())
	{
		return;
	}

	SetWindowText(g_hWndInfo, str);
}

void AddPlayerList(const char *str)
{
	//SendMessage(g_hPlayerListBox,LB_INSERTSTRING,-1,(LPARAM)(str));
}

void DelPlayerList(const char *str)
{
	//LRESULT ret = SendMessage(g_hPlayerListBox,LB_SELECTSTRING, 0,(LPARAM)(str));
	//if (ret!=LB_ERR)
	//{
	//	SendMessage(g_hPlayerListBox, LB_DELETESTRING, ret, 0);
	//}
}

void AddRegionList(const char *str)
{
	SendMessage(g_hWndGSList,LB_INSERTSTRING,-1,(LPARAM)(str));
}

void DelRegionList(const char *str)
{
	LRESULT ret = SendMessage(g_hWndGSList,LB_SELECTSTRING, 0,(LPARAM)(str));
	if (ret!=LB_ERR)
	{
		SendMessage(g_hWndGSList, LB_DELETESTRING, ret, 0);
	}
}
