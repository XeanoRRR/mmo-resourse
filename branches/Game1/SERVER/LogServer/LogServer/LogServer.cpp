// LogServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LogServer.h"
#include "game.h"
#include "..\nets\netlog\message.h"
//#include "..\nets\netlog/MyNetServer.h"
#include "..\../setup/GlobeSetup.h"


#define MAX_LOADSTRING 100



// end define

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool volatile g_bPlayerLoadThreadsExit;
extern vector<HANDLE> g_vPlayerLoadThreads;

// ������־
void SaveLogText(bool bForce);

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

HWND g_hWnd;
HWND g_hInfoText;
HWND g_hLogText;
//HWND g_hGameServerListBox;
HWND g_hPlayerListBox;

long g_lInfoWindowHeight = 200;			// ��Ϣ��ʾ���ڵĴ�С
SYSTEMTIME g_stLastSaveTime;			// �ϴγɹ����̵�ʱ��
DWORD g_dwLastSaveTick=timeGetTime();	// �ϴδ��̳ɹ��ļ���
DWORD g_dwThisSaveStartTime = 0;		// ���δ��̿�ʼʱ��

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

char g_strCurrentPath[MAX_PATH];

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	EnableDebugNew();

	//_CrtSetBreakAlloc(1214);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOGSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GetCurrentDirectory(MAX_PATH, g_strCurrentPath);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_LOGSERVER);

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

	// ������־��Ϣ
	SaveLogText(true);

	PutLogInfo("\r\n//////////////////////////////////");
	PutLogInfo("\r\n//       LogServer Exit       //");
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_LOGSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_LOGSERVER;
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
		CW_USEDEFAULT, 0, 900, 400, NULL, NULL, hInstance, NULL);

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

	g_hInfoText=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,0,0,rect.right-320, g_lInfoWindowHeight-4,g_hWnd,0,hInst,NULL);
	g_hLogText=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,0,g_lInfoWindowHeight,rect.right-320, rect.bottom-g_lInfoWindowHeight,g_hWnd,0,hInst,NULL);
	//g_hGameServerListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,rect.right-320,0,320,g_lInfoWindowHeight,g_hWnd,0,hInst,NULL);
	g_hPlayerListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,rect.right-320,0,320,h,g_hWnd,0,hInst,NULL);
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
			if (MessageBox(g_hWnd, "End Server?", "msg", MB_YESNO)==IDYES)
				DestroyWindow(hWnd);
			break;
		
			// ������������
		case ID_MENU_KICKALL:
			{
				map<DWORD, CGame::tagGameServer>* pServerList = GetGame() -> GetGameServerList();

				if( pServerList ) {
					CMessage msg( MSG_W2S_OTHER_KICKALL );
					//msg.Add( lPlayerID_GM );
					for( map<DWORD, CGame::tagGameServer>::iterator itBegin = pServerList -> begin();
						itBegin != pServerList -> end(); itBegin ++ ) {
						if( itBegin -> second.bConnected ) {				
							msg.SendGSID( itBegin->first );
						}
					}
				}
			}
			break;




		///* �ض�����
		case ID_RELOADSETUP:
			{
				//bool bOldUseLogSys = GetGame()->GetSetup()->bUseLogSys;
				GetGame()->LoadSetup();

			}
			break;
		case ID_32870:
			{
				//! �ض���־�Ķ�д�������ã������͵��������ӵĿͻ���
				GetGame()->ReLoadConfig();
			}
			break;

		// ��ʾ������Ϣ����
		case ID_SHOWSAVEINFO:
			g_bShowSaveInfo ? g_bShowSaveInfo=false : g_bShowSaveInfo=true;
			break;


		case ID_CREATE_DB_TABLE://��������DB�д������ݱ�
			{
				GetGame()->CreateTableInDB();
			}
			break;
		case ID_CREATE_SQL://�������ݱ����ű�
			{
				GetGame()->CreateTableInSqlFile();
				AddLogText("create_table.sql�ļ��ѱ����ڷ���Ŀ¼�£�");
			}
			break;
		case ID_32854:
			{
				GetGame()->OutTableInfoToFile();
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
	case WM_CLOSE:
		if (wParam!=0 || MessageBox(g_hWnd, "End Server?", "msg", MB_YESNO)==IDYES)
			PostQuitMessage(0);
		break;
	case WM_SIZE:
		{
			long lW = LOWORD(lParam);
			long lH = HIWORD(lParam);
			MoveWindow(g_hInfoText, 0,0, lW-320, g_lInfoWindowHeight-4, true);
			MoveWindow(g_hLogText, 0,g_lInfoWindowHeight, lW-320, lH-g_lInfoWindowHeight, true);
			//MoveWindow(g_hGameServerListBox, lW-320,0,320,g_lInfoWindowHeight, true);
			MoveWindow(g_hPlayerListBox, lW-320,0,320,lH, true);
		}
		break;
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
		timeGetTime() - dwLastSaveTime > GetGame()->GetSetup()->dwSaveInfoTime ||		// ����
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

	//	GetWindowText(g_hLogText, str, lMaxLength);
	//	PutLogInfo(str);
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

	SendMessage(g_hLogText, EM_SETSEL, 0, -1);
	SendMessage(g_hLogText, EM_SETSEL, -1, 0);
	SendMessage(g_hLogText, EM_REPLACESEL, FALSE, (LPARAM)str);
}

// ˢ�·�������Ϣ
void RefeashInfoText(const char *pInfoText)
{
	if(NULL == pInfoText) 
		return;
	SetWindowText(g_hInfoText, pInfoText);
}

void AddClientList(const char *str)
{
	SendMessage(g_hPlayerListBox,LB_INSERTSTRING,-1,(LPARAM)(str));
}

void DelClientList(const char *str)
{
	LRESULT ret = SendMessage(g_hPlayerListBox,LB_SELECTSTRING, 0,(LPARAM)(str));
	if (ret!=LB_ERR)
	{
		SendMessage(g_hPlayerListBox, LB_DELETESTRING, ret, 0);
	}
}
