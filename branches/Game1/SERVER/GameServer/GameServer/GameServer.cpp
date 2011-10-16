// GameServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GameServer.h"
#include "Game.h"
#include "../AppServer/ServerRegion.h"
#include "../AppServer/script/Script.h"
#include "../nets/Servers.h"
#include "../AppServer/RgnManager.h"
#include "GameManager.h"
#include "..\LogClient\gs\LogicLogInterface.h"
#include "../AppServer/AntiCheat/AntiCheatCommon.h"

#define MAX_LOADSTRING 100
	


// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

HWND g_hWnd;
HWND g_hInfoText;
HWND g_hLogText;
HWND g_hRegionListBox;
//HWND g_hPlayerListBox;

long g_lInfoWindowHeight = 140;			// ��Ϣ��ʾ���ڵĴ�С

extern unsigned volatile RELOAD_SETUP_FLAGS;	// �ض������ļ���ʾ
extern const unsigned RELOAD_SETUP;
extern const unsigned RELOAD_SETUPEX;
extern const unsigned RELOAD_SETUPMAPFILE;
extern const unsigned OUTPUT_SCRIPT_INFO;
extern const unsigned OUTPUT_MEMPOOL_INFO;


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
	//_CrtSetBreakAlloc(333);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMESERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GAMESERVER);

	unsigned threadID;
	HANDLE  hGameThread = (HANDLE)_beginthreadex(NULL,0,GameThreadFunc,NULL,0,&threadID);

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
	PutLogInfo("\r\n//       GameServer Exit        //");
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GAMESERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_GAMESERVER;
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
	g_hRegionListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,rect.right-160,0,160,rect.bottom,g_hWnd,0,hInst,NULL);
//	g_hPlayerListBox=CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,rect.right-160,g_lInfoWindowHeight,160,h-g_lInfoWindowHeight,g_hWnd,0,hInst,NULL);
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

		case ID_CONNECT_WORLDSERVER:
			//GetGame()->ReConnectWorldServer();
			GetGame()->CreateConnectWorldThread();
			break;
		case ID_CONNECT_DBSERVER:
			//GetGame()->ReConnectWorldServer();
			GetGame()->CreateConnectDBThread();
			break;

		case ID_CONNECT_BOUNSSERVER:
			if(GetGame()->GetSetup()->lUseBounsServer)
			{
				GetGame()->CreateConnectBounsServerThread();
			}			
			break;

		case ID_RELOADSETUP:
			RELOAD_SETUP_FLAGS |= RELOAD_SETUP;
			break;
		case ID_RELOADSETUPEX:
			RELOAD_SETUP_FLAGS |= RELOAD_SETUPEX;
			break;
		case ID_RELOAD_MAPFILESETUP:
			RELOAD_SETUP_FLAGS |= RELOAD_SETUPMAPFILE;
			break;
		case ID_OUTPUTSCRIPTINFO:
			RELOAD_SETUP_FLAGS |= OUTPUT_SCRIPT_INFO;
			break;
		case ID_MEMPOOLINFO:
			RELOAD_SETUP_FLAGS |= OUTPUT_MEMPOOL_INFO;
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
			MoveWindow(g_hInfoText, 0, 0, lW-160, g_lInfoWindowHeight-4, true);
			MoveWindow(g_hLogText, 0, g_lInfoWindowHeight, lW-160, lH-g_lInfoWindowHeight, true);
			MoveWindow(g_hRegionListBox, lW-160,0,160,lH, true);
		//	MoveWindow(g_hPlayerListBox, lW-160,g_lInfoWindowHeight,160,lH-g_lInfoWindowHeight, true);
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
		timeGetTime() - dwLastSaveTime > GetGame()->GetSetup()->dwSaveInfoTime ||		// ����ʱ��
		GetWindowTextLength(g_hLogText) >= lMaxLength		// ���ݳ���64K
		)
	{
		dwLastSaveTime = timeGetTime();

		PutLogInfo("\r\n=============================== Start Save Log ===============================\r\n");

		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		_snprintf(str, lMaxLength, "%04d-%02d-%02d %02d:%02d:%02d\r\n\r\n", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
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
	_snprintf(str, lMaxLength, "[%02d-%02d %02d:%02d:%02d] ", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	va_list va;
	va_start(va,msg);
	vsprintf(&str[strlen(str)],msg,va);
	va_end(va);

	strcat_s(str, lMaxLength, "\r\n");

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
	_snprintf(str, lMaxLength, "[%02d-%02d %02d:%02d:%02d] <error> ", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	va_list va;
	va_start(va,msg);
	vsprintf(&str[strlen(str)],msg,va);
	va_end(va);

	strcat_s(str, 64000, "\r\n");

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

	static long lMaxConnects = 0,lMaxPlayers = 0, lMaxTeams = 0, lMaxScripts = 0;
	static long lSendSizePerSecondToClient = 0, lRecvSizePerSecondFromClient = 0;
	static long lSendSizePerSecondToWorld = 0, lRecvSizePerSecondFromWorld = 0;

	static long lMaxTeamRgnNum = 0;
	static long lMaxPRgnNum = 0;
	static DWORD dwMaxAvaiVirMem = 0;
	static DWORD dwHandleCount = 0;

	static long lMaxDupRgnDataNum = 0;

	long lConnects = GetGame()->GetNetServer()->GetClientCount();
	long lPlayers = GetGame()->GetPlayerCount();
	long lTeams = GetGame()->GetNumTeams();
	long lScripts = g_Scripts.size();
	MEMORYSTATUS msC;GlobalMemoryStatus(&msC);
	DWORD dwCurAvaiVirMem = msC.dwAvailVirtual/1048576;
	DWORD dwCurNandleCount = 0;
	GetProcessHandleCount(GetCurrentProcess(),&dwCurNandleCount);

	DWORD dwAcceptThreadTick = GetGame()->GetNetServer()->GetAcceptThreadTick();
	DWORD dwNetThreadTick = GetGame()->GetNetServer()->GetNetThreadTick();
	DWORD dwWorkThreadTick = GetGame()->GetNetServer()->GetWorkThreadTick();
	DWORD dwMainThreadTick = GetGame()->GetAITick();

	long lCurTeamRgnNum = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().size();
	long lCurPRgnNum = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().size();

	long lSendSizePerSecondToClientTempt = GetGame()->GetNetServer()->GetSendSizePerSecond(NF_Server_Client);
	long lRecvSizePerSecondFromClientTempt = GetGame()->GetNetServer()->GetRecvSizePerSecond(NF_Server_Client);	
	long lSendSizePerSecondToWorldTempt = GetGame()->GetNetServer()->GetSendSizePerSecond(NF_WServer_Client);
	long lRecvSizePerSecondFromWorldTempt = GetGame()->GetNetServer()->GetRecvSizePerSecond(NF_WServer_Client);

	long lCurDupRgnDataNum = GameManager::GetInstance()->GetRgnManager()->GetDupRgnDataNum();

	if (lConnects>lMaxConnects)
		lMaxConnects = lConnects;
	if (lPlayers>lMaxPlayers)
		lMaxPlayers = lPlayers;
	if (lTeams>lMaxTeams)
		lMaxTeams = lTeams;
	if (lScripts>lMaxScripts)
		lMaxScripts = lScripts;

	if(lCurTeamRgnNum>lMaxTeamRgnNum)
		lMaxTeamRgnNum = lCurTeamRgnNum;

	if(lCurPRgnNum > lMaxPRgnNum)
		lMaxPRgnNum = lCurPRgnNum;

	if(dwCurAvaiVirMem > dwMaxAvaiVirMem)
		dwMaxAvaiVirMem = dwCurAvaiVirMem;
	if(dwCurNandleCount > dwHandleCount)
		dwHandleCount = dwCurNandleCount;

	if(lCurDupRgnDataNum > lMaxDupRgnDataNum)
		lMaxDupRgnDataNum = lCurDupRgnDataNum;

	
	if(lSendSizePerSecondToClientTempt > lSendSizePerSecondToClient)
		lSendSizePerSecondToClient = lSendSizePerSecondToClientTempt;
	if(lRecvSizePerSecondFromClientTempt > lRecvSizePerSecondFromClient)
		lRecvSizePerSecondFromClient = lRecvSizePerSecondFromClientTempt;
	if(lSendSizePerSecondToWorldTempt > lSendSizePerSecondToWorld)
		lSendSizePerSecondToWorld = lSendSizePerSecondToWorldTempt;
	if(lRecvSizePerSecondFromWorldTempt > lRecvSizePerSecondFromWorld)
		lRecvSizePerSecondFromWorld = lRecvSizePerSecondFromWorldTempt;

	// �������Ϣ��� :Ques\AccWrong\AccTimeOut\AccRight
	const AntiCheat::LogInfo *ac_log = AntiCheat::GetLogInfo();

	_snprintf(str, 102400, "Connects = %d/%d  Players = %d/%d\r\n\
HandleCount=%d/%d	AvaiVirMem =%d/%d\r\n\
AccTTick=%u NetTTick=%u WorkTTick=%u MainTTick=%u\r\n\
Teams=%d/%d  Scripts=%d/%d  ServerID = %d\r\nNT-S2C=%d/%d(B/S)\t\tNT-C2S=%d/%d(B/S)\r\nNT-S2W=%d/%d(B/S)\t\tNT-W2S=%d/%d(B/S)\r\n���鸱������=%d/%d ���˸�������=%d/%d �������ݿ����=%d/%d\r\n\
AntiCheat=%u/%u/%u/%u/%u\r\n\r\n",
					lConnects, lMaxConnects,lPlayers, lMaxPlayers,
					dwCurNandleCount,dwHandleCount,dwCurAvaiVirMem,dwMaxAvaiVirMem,
					dwAcceptThreadTick,dwNetThreadTick,dwWorkThreadTick,dwMainThreadTick,
					lTeams, lMaxTeams,lScripts, lMaxScripts, GetGame()->GetIDIndex(),
					lSendSizePerSecondToClientTempt,lSendSizePerSecondToClient,lRecvSizePerSecondFromClientTempt,lRecvSizePerSecondFromClient,
					lSendSizePerSecondToWorldTempt,lSendSizePerSecondToWorld,lRecvSizePerSecondFromWorldTempt,lRecvSizePerSecondFromWorld, 
					lCurTeamRgnNum, lMaxTeamRgnNum, lCurPRgnNum, lMaxPRgnNum, lCurDupRgnDataNum, lMaxDupRgnDataNum,
					ac_log->AccQues, ac_log->AccWrong, ac_log->AccTimeOut, ac_log->AccRight, ac_log->AccSysErr );


	// ��ʾÿ������������
	static char strTemp[256];
	

	
	// Normal Region
	//for(map<long, CServerRegion*>::iterator it=GetRegionMap()->begin(); it!=GetRegionMap()->end(); it++)
	for(CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().begin(); it != GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end(); it++)
	{
		CServerRegion* pRegion = it->second;
		if (pRegion)
		{
			_snprintf(strTemp, 256, "%s=%d ", pRegion->GetName(), pRegion->GetPlayerAmout() );
			strcat(str, strTemp);
		}
	}

	SetWindowText(g_hInfoText, str);
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
	SendMessage(g_hRegionListBox,LB_INSERTSTRING,-1,(LPARAM)(str));
}

void DelRegionList(const char *str)
{
	LRESULT ret = SendMessage(g_hRegionListBox,LB_SELECTSTRING, 0,(LPARAM)(str));
	if (ret!=LB_ERR)
	{
		SendMessage(g_hRegionListBox, LB_DELETESTRING, ret, 0);
	}
}
