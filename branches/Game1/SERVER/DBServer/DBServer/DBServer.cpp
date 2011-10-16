// dbserver.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DBServer.h"
#include "game.h"
#include "..\..\..\DBAccess\WorldDB\RsEntity.h"
#include "..\nets\Servers.h"

#include "../AppWorld/entity/entityManager.h"

#define MAX_LOADSTRING 100

extern unsigned volatile RELOAD_PROFILE_FLAGS;	// �ض������ļ���ʾ
// define const
extern const unsigned RELOAD_LOGING;		//����־����
// end define

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool volatile g_bWriteLogThreadExit;
extern HANDLE g_hWriteLogThread;

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
HWND g_hGameServerListBox;
HWND g_hPlayerListBox;

long g_lInfoWindowHeight = 140;			// ��Ϣ��ʾ���ڵĴ�С
SYSTEMTIME g_stLastSaveTime;			// �ϴγɹ����̵�ʱ��
DWORD g_dwLastSaveTick=timeGetTime();	// �ϴδ��̳ɹ��ļ���
DWORD g_dwThisSaveStartTime = 0;		// ���δ��̿�ʼʱ��

// ��ʱˢ����Ϣ
long lConnects		= 0;
long lMapAccounts= 0;
long lMapScripts= 0;
long lMapProcs= 0;

long lMapPlayers	= 0;
long lPlayers		= 0;
long lFriendGroup	= 0;
long lOfflinePlayers= 0;
long lLoginPlayers	= 0;
long lCreatePlayers = 0;
long lDeletePlayers = 0;
long lResumePlayers = 0;
long lOperingLoadEntitys	= 0;
long lOperingSaveEntitys	= 0;
long lTeams			= 0;
long lLargess		= 0;
long lWriteLog		= 0;
long lLoadPlayer	= 0; 

long lMails = 0;
long lFactions = 0;

long lCurLoadThreadQueueSize = 0;
long lCurSaveThreadQueueSize = 0;

long lBeginLoadNum = 0;
long lBeginSaveNum = 0;
long lEndNum = 0;

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
	LoadString(hInstance, IDC_dbserver, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GetCurrentDirectory(MAX_PATH, g_strCurrentPath);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_dbserver);

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
	PutLogInfo("\r\n//       dbserver Exit       //");
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_dbserver);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_dbserver;
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
		CW_USEDEFAULT, 0, 600, 400, NULL, NULL, hInstance, NULL);

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

	g_hInfoText=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,0,0,rect.right-160, g_lInfoWindowHeight-4,g_hWnd,0,hInst,NULL);
	g_hLogText=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,0,g_lInfoWindowHeight,rect.right-160, rect.bottom-g_lInfoWindowHeight,g_hWnd,0,hInst,NULL);
	g_hGameServerListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,rect.right-160,0,160,g_lInfoWindowHeight,g_hWnd,0,hInst,NULL);
	g_hPlayerListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,rect.right-160,g_lInfoWindowHeight,160,h-g_lInfoWindowHeight,g_hWnd,0,hInst,NULL);
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

		// ����LoginServer
		case ID_CONNECT_LOGINSERVER:

			break;

		case ID_CONNECT_COSTSERVER:
			break;

		case ID_MENU_RELOAD_LOGSYSTEM:
			RELOAD_PROFILE_FLAGS |= RELOAD_LOGING;
			break;
	
		// ���̴���
		case ID_SAVEDATA:
			extern bool g_bSendSaveMsgNow;
			g_bSendSaveMsgNow = true;
			break;

		// ����GameServer�������
		case ID_MENU_COLLECTPLAYERDATA:
			extern bool g_bSendCollectPlayerDataMsgNow;
			g_bSendCollectPlayerDataMsgNow = true;
			break;

		case ID_DELPLAYER:
			extern bool g_bDelPlayers;
			g_bDelPlayers = true;
			break;
		// ���̴浱ǰ����
		case ID_SAVEALLDATA:
			extern bool g_bSaveNowData;
			g_bSaveNowData = true;
			break;
		case ID_MAKEBINDATA:
			GetGame()->Test3GoodsImageDataBuild();
			break;
		case ID_MAILGOODS:
			GetGame()->Test3MailGoodsDataBuild();
			break;

		case ID_Menu_GiftChargeItems:
			GetGame()->ReLoad("GiftItems", true, true);
			break;

	
		// ��ʾ������Ϣ����
		case ID_SHOWSAVEINFO:
			g_bShowSaveInfo ? g_bShowSaveInfo=false : g_bShowSaveInfo=true;
			break;

		case ID_SWITCH_LOGSYS:
			
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
		if (wParam !=0 || MessageBox(g_hWnd, "�Ƿ��˳�?", "msg", MB_YESNO)==IDYES)
			PostQuitMessage(0);
		break;
	case WM_SIZE:
		{
			long lW = LOWORD(lParam);
			long lH = HIWORD(lParam);
			MoveWindow(g_hInfoText, 0,0, lW-160, g_lInfoWindowHeight-4, true);
			MoveWindow(g_hLogText, 0,g_lInfoWindowHeight, lW-160, lH-g_lInfoWindowHeight, true);
			MoveWindow(g_hGameServerListBox, lW-160,0,160,g_lInfoWindowHeight, true);
			MoveWindow(g_hPlayerListBox, lW-160,g_lInfoWindowHeight,160,lH-g_lInfoWindowHeight, true);
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
void RefeashInfoText()
{
	static char str[102400];

	if (!GetGame() || !GetGame()->GetNetServer())
	{
		return;
	}

	static long lMaxConnects = 0;

	static long lMaxMapAccounts= 0;
	static long lMaxMapScripts= 0;
	static long lMaxMapProcs= 0;

	static long lMaxMapPlayers = 0;
	static long lMaxDepots = 0;
	static long lMaxPlayers = 0;
	static long lMaxFriendGroup = 0;
	static long lMaxOfflinePlayers = 0;
	static long lMaxLoginPlayers = 0;
	static long lMaxCreatePlayers = 0;
	static long lMaxDeletePlayers = 0;
	static long lMaxResumePlayers = 0;
	static long lMaxLoadingEntitys = 0;
	static long lMaxSavingEntitys = 0;
	static long lMaxTeams = 0;
	static long lMaxLargess = 0;
	static long lMaxWriteLog = 0;
	static long lMaxLoadPlayer = 0;
	static long lMaxRebackMessage = 0;

	static long lMaxMails = 0;
	static long lMaxFactions = 0;
	static long lBeginLoadNumMax = 0;
	static long lBeginSaveNumMax = 0;
	static long lEndNumMax = 0;

	static DWORD dwMaxAvaiVirMem = 0;
	MEMORYSTATUS msC;GlobalMemoryStatus(&msC);
	DWORD dwCurAvaiVirMem = msC.dwAvailVirtual/1048576;

	extern long g_lRebackMessageCount;
	long lRebackMessage = g_lRebackMessageCount;

	if (lMapAccounts>lMaxMapAccounts)
		lMaxMapAccounts = lMapAccounts;
	if (lMapScripts>lMaxMapScripts)
		lMaxMapScripts = lMapScripts;
	if (lMapProcs>lMaxMapProcs)
		lMaxMapProcs = lMapProcs;

	if (lConnects>lMaxConnects)
		lMaxConnects = lConnects;
	if (lMapPlayers>lMaxMapPlayers)
		lMaxMapPlayers = lMapPlayers;
	if (lPlayers>lMaxPlayers)
		lMaxPlayers = lPlayers;
	if (lFriendGroup>lMaxFriendGroup)
		lMaxFriendGroup = lFriendGroup;
	if (lOfflinePlayers>lMaxOfflinePlayers)
		lMaxOfflinePlayers = lOfflinePlayers;
	if (lLoginPlayers>lMaxLoginPlayers)
		lMaxLoginPlayers = lLoginPlayers;
	if (lCreatePlayers>lMaxCreatePlayers)
		lMaxCreatePlayers = lCreatePlayers;
	if (lDeletePlayers>lMaxDeletePlayers)
		lMaxDeletePlayers = lDeletePlayers;
	if (lResumePlayers>lMaxResumePlayers)
		lMaxResumePlayers = lResumePlayers;
	if (lOperingLoadEntitys>lMaxLoadingEntitys)
		lMaxLoadingEntitys = lOperingLoadEntitys;
	if (lOperingSaveEntitys>lMaxSavingEntitys)
		lMaxSavingEntitys = lOperingSaveEntitys;
	if (lTeams>lMaxTeams)
		lMaxTeams=lTeams;
	if (lLargess>lMaxLargess)
		lMaxLargess=lLargess;
	if( lWriteLog > lMaxWriteLog )
		lMaxWriteLog = lWriteLog;
	if( lLoadPlayer > lMaxLoadPlayer )
		lMaxLoadPlayer = lLoadPlayer;
	if (lRebackMessage > lMaxRebackMessage)
		lMaxRebackMessage = lRebackMessage;

	if(lMails > lMaxMails)
		lMaxMails = lMails;
	if(lFactions > lMaxFactions)
		lMaxFactions = lFactions;

	if(lBeginLoadNum > lBeginLoadNumMax)
		lBeginLoadNumMax = lBeginLoadNum;

	if(lBeginSaveNum > lBeginSaveNumMax)
		lBeginSaveNumMax = lBeginSaveNum;

	if(lEndNum > lEndNumMax)
		lEndNumMax = lEndNum;

	if(dwCurAvaiVirMem > dwMaxAvaiVirMem)
		dwMaxAvaiVirMem = dwCurAvaiVirMem;

	int nSave = 0;

	static char *strInfo[] = {"(����)", "(�쳣!!!!)"};

	sprintf(str, 
"Last Save Time : %d ����	AvaiVirMem=%d/%d(MB)\r\n\
Connects=%d/%d    Mails=%d/%d	Factions=%d/%d\r\n\
Account=%d/%d   Player=%d/%d	ScriptVar=%d/%d	Proc=%d/%d\r\n\
LoadingEntitys=%d/%d SavingEntitys=%d/%d\r\n\
FriendGroup=%d/%d\r\n\
BeginLoadNum=%d/%d   BeginSaveNum=%d/%d\r\n\
WriteLog    = %d/%d    ReMsg = %d/%d\r\n\
Oper Load Thread Num[%d] Oper Save Thread Num[%d]\r\n",
		g_dwThisSaveStartTime,dwCurAvaiVirMem,dwMaxAvaiVirMem,
		lConnects, lMaxConnects, lMails, lMaxMails,lFactions, lMaxFactions,
		lMapAccounts, lMaxMapAccounts,lMapPlayers, lMaxMapPlayers, lMapScripts,lMaxMapScripts,lMapProcs,lMaxMapProcs,
		lOperingLoadEntitys, lMaxLoadingEntitys, lOperingSaveEntitys, lMaxSavingEntitys,
		lFriendGroup, lMaxFriendGroup,
		lBeginLoadNum,lBeginLoadNumMax,lBeginSaveNum,lBeginSaveNumMax,
		lWriteLog, lMaxWriteLog, lRebackMessage, lMaxRebackMessage,
		GetGame()->GetEntityManager()->GetLoadThreadMap().size(),
		GetGame()->GetEntityManager()->GetSaveThreadMap().size());

	// ��ԭ���̼�ʱ
	if(g_dwThisSaveStartTime != 0)
		g_dwThisSaveStartTime = 0;

	SetWindowText(g_hInfoText, str);
}

void AddPlayerList(const char *str)
{

}

void DelPlayerList(const char *str)
{

}
