// WorldServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WorldServer.h"
#include "game.h"
#include "..\nets\networld\message.h"
//#include "..\..\..\DBAccess\WorldDB\RsPlayer.h"
#include "..\nets\Servers.h"
#include "..\../setup/GlobeSetup.h"
//#include "../DBAccess/WorldDB/Largess.h"
#include "../appworld/cardsdealingsystem/cardshandler.h"
#include "../AppWorld/GlobalRgnManager.h"

#define MAX_LOADSTRING 100

extern unsigned volatile RELOAD_PROFILE_FLAGS;	// �ض������ļ���ʶ
extern unsigned volatile RELOAD_CONFIG_FLAGS;	// �ض������ļ���ʶ

// define const
extern const unsigned RELOAD_LOGING;		//����־����
extern const unsigned RELOAD_GMLIST;		//��GM����
extern const unsigned RELOAD_BROADCAST;		//���㲥
extern const unsigned RELOAD_VILLAGEWAR;	//����ս

extern const unsigned RELOAD_BUILD;			//����ս
extern const unsigned RELOAD_INCREMENT;		//����ֵ�̵�
extern const unsigned RELOAD_GAMESETUP;		//������͵�����
extern const unsigned RELOAD_WORDFILTER;	//�����ֹ���


extern const unsigned RELOAD_PLAYERLIST;	//������Ժ;���
extern const unsigned RELOAD_GOODSLIST;		//��Ʒ����
extern const unsigned RELOAD_MONSTERLIST;	//�������Ժ͵���
extern const unsigned RELOAD_TRADELIST;		//�̵�����

extern const unsigned RELOAD_SKILLLIST;		//��������
extern const unsigned RELOAD_GOLBELSETUP;	//ȫ������
extern const unsigned RELOAD_ALLSCRIPT;		//���Խű�
extern const unsigned RELOAD_ONESCRIPT;		//�����ű�

extern const unsigned RELOAD_GENVAR;		//�ű�����
extern const unsigned RELOAD_REGIONLIST;	//�����б�
extern const unsigned RELOAD_REGIONSETUP;	//��������
extern const unsigned RELOAD_ONEREGION;		//��������*.rs

extern const unsigned RELOAD_ALLREGION;		//���г���*.rs
extern const unsigned RELOAD_ALLREGIONALL;	//���г���*.rs

extern const unsigned RELOAD_FACTION;		//�������
extern const unsigned RELOAD_FACTIONWAR;	//���ս
extern const unsigned RELOAD_QUEST;			//����

extern const unsigned RELOAD_CONTRIBUTE;	//���ҹ��ױ�����Ʒ
extern const unsigned RELOAD_DOCCULIST;		//��ְҵϵͳ�б�
extern const unsigned RELOAD_STRINGRESOURCE;//�ַ�����Դ
extern const unsigned RELOAD_SYSMAIL;		//ϵͳ�ʼ�
extern const unsigned RELOAD_CREDIT;		//��������

extern const unsigned RELOAD_FUNCSETUP;		//WS�Ĳ��ֹ�������(FuncSetup.ini)
extern const unsigned RELOAD_DUPRGNSERVER;	//WS�ĸ�����������(data/duprgnserver.ini)
extern const unsigned RELOAD_MAPFILESETUP;	//�ڴ�Ӱ���ļ�����

extern const unsigned RELOAD_PLAYER_MAN;	// Fox@20081120 ��ҹ���
extern const unsigned RELOAD_PET_LIST;		// Fox@20081119 ����
extern const unsigned RELOAD_ANTI_CHEAT;    // BaiYun@2009525 �����

extern const unsigned RELOAD_COUNT_TIMER;    //  ��Ҷ�ʱ��
extern const unsigned RELOAD_COSMETOLOGY;	 // ����ϵͳ
extern const unsigned RELOAD_QUEST_INDEX;    //  ��������
extern const unsigned RELOAD_LOTTERY;        //  ��Ʊϵͳ

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
HWND g_hGameServerListBox;
HWND g_hPlayerListBox;

long g_lInfoWindowHeight = 140;			// ��Ϣ��ʾ���ڵĴ�С
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
	LoadString(hInstance, IDC_WORLDSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GetCurrentDirectory(MAX_PATH, g_strCurrentPath);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WORLDSERVER);

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
	PutLogInfo("\r\n//       WorldServer Exit       //");
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WORLDSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_WORLDSERVER;
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

			// �ض���Ҷ�ʱ��
		case ID_MENU_RELOADCOUNTTIMERLIST:
			GetGame()->ReLoad("CountTimerList", true, false);
			break;
		// ����LoginServer
		case ID_CONNECT_LOGINSERVER:
			//GetGame()->ReConnectLoginServer();
			GetGame()->CreateConnectLoginThread();
			break;
			// ����LoginServer
		case ID_CONNECT_DBSERVER:
			//GetGame()->ReConnectLoginServer();
			GetGame()->CreateConnectDBThread();
			break;

		case ID_RELOAD_MAPFILESETUP:
			RELOAD_PROFILE_FLAGS |= RELOAD_MAPFILESETUP;
			break;

		case ID_RELOAD_FUNCSETUP:
			RELOAD_PROFILE_FLAGS |= RELOAD_FUNCSETUP;
			break;
		case ID_RELOAD_DUPRGNSERVER:
			RELOAD_PROFILE_FLAGS |= RELOAD_DUPRGNSERVER;
			break;
		case ID_MENU_RELOAD_PLAYERLIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_PLAYERLIST;
			break;

		case ID_MENU_RELOAD_GOODSLIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_GOODSLIST;
			break;

		case ID_MENU_RELOAD_MONSTERLIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_MONSTERLIST;
			break;
		case ID_MENU_ReloadBuild:
			RELOAD_PROFILE_FLAGS |=RELOAD_BUILD;
			break;
		case ID_MENU_RELOAD_TRADELIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_TRADELIST;
			break;

		case ID_MENU_RELOAD_SKILLLIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_SKILLLIST;
			break;

		case ID_MENU_RELOAD_GLOBESETUP:
			RELOAD_PROFILE_FLAGS |= RELOAD_GOLBELSETUP;
			break;

		case ID_MENE_ReLoadGameSetup:
			RELOAD_PROFILE_FLAGS |= RELOAD_GAMESETUP;
			break;

		case ID_MENU_RELOAD_LOGSYSTEM:
			RELOAD_PROFILE_FLAGS |= RELOAD_LOGING;
			break;
		case ID_MENU_RELOAD_GMLIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_GMLIST;
			break;

		case ID_MENU_RELOADSCRIPTS:
			RELOAD_PROFILE_FLAGS |= RELOAD_ALLSCRIPT;
			break;

		case ID_MENU_RELOAD_REGIONLIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_REGIONLIST;
			break;

		case ID_MENU_RELOAD_REGIONSETUP:
			RELOAD_PROFILE_FLAGS |= RELOAD_REGIONSETUP;
			break;
		case ID_MENU_RELOADSTRINGRESOURCE:
			RELOAD_PROFILE_FLAGS |= RELOAD_STRINGRESOURCE;
			break;
		case ID_MENU_RELOADSYSMAIL:
			RELOAD_PROFILE_FLAGS |= RELOAD_SYSMAIL;
			break;
		case ID_MENU_RELOADCOSMETOLOGY:
			{
				GetGame()->LoadServerResource();
				GetGame()->ReLoad("Cosmetology",true);
				GetGame()->UnloadServerResource();
			}		
			break;
		case ID_MENU_RELOADCREDIT:
			RELOAD_PROFILE_FLAGS |=RELOAD_CREDIT;
			break;
		case ID_MENU_RELOAD_BROADCAST:
			RELOAD_PROFILE_FLAGS |= RELOAD_BROADCAST;
			break;

		case ID_MENU_RELOAD_GENVARLIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_GENVAR;
			break;

		case ID_MENU_RELOAD_FACTIONPARA:
			RELOAD_PROFILE_FLAGS |= RELOAD_FACTION;
			break;

		case ID_MENU_StatPlayerRanks:
			{
				extern bool g_bStatPlayerRanks;
				g_bStatPlayerRanks = true;
			}
			break;

		case ID_MENU_ReloadCityWarPara:
			RELOAD_PROFILE_FLAGS |= RELOAD_BUILD;
			break;

		case ID_MENU_ReloadFactionWarPara:
			RELOAD_PROFILE_FLAGS |= RELOAD_FACTIONWAR;
			break;

		case ID_MENU_ReloadQuestDat:
			RELOAD_PROFILE_FLAGS |= RELOAD_QUEST;
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

		// ���̴浱ǰ����
		case ID_Menu_SaveNow:
			extern bool g_bSaveNowData;
			extern bool g_bSendSaveMsgNow;
			g_bSendSaveMsgNow = true;
			g_bSaveNowData = true;
			break;
		case ID_Menu_SaveAllOrgNow:
			{
				extern bool g_bSaveAllOrg;
				extern bool g_bSendSaveMsgNow;
				g_bSendSaveMsgNow = true;
				g_bSaveAllOrg = true;
			}
			break;
		
			// ������������
		case ID_MENU_KICKALL:
			{
				extern bool g_bKickAllPlayer;
				g_bKickAllPlayer = true;
			}
			break;

		// �����ű��ļ�
		case ID_MENU_RELOADONESCRIPT:
			{
				char strFile[MAX_PATH];
				if (GetFileDialog(true, NULL, strFile))
				{
					SetCurrentDirectory(g_strCurrentPath);
					strcpy(strFile, &strFile[lstrlen(g_strCurrentPath)+1]);
					ReplaceLine(strFile);
					strlwr(strFile);	//  ת����Сд
					GetGame()->ReLoadOneScript(strFile);
				}
				SetCurrentDirectory(g_strCurrentPath);
			}
			break;
		case ID_MENU_RELOAD_ONEREGIONSETUP:
			{//װ��һ������������
				char strFile[MAX_PATH];
				if (GetFileDialog(true, NULL, strFile))
				{
					SetCurrentDirectory(g_strCurrentPath);
					strcpy(strFile, &strFile[lstrlen(g_strCurrentPath)+1]);
					ReplaceLine(strFile);
					strlwr(strFile);	//  ת����Сд

					//�õ������ı��
					char strRegion[20];
					strncpy(strRegion,&strFile[8],5);
					strRegion[5]='\0';
					//lPos
					long lRegionID =atoi(strRegion);
					GetGame()->ReLoadOneRegionSetup(lRegionID);
				}
				SetCurrentDirectory(g_strCurrentPath);
			}
			break;
		case ID_MENU_RELOAD_AllREGION:
			RELOAD_PROFILE_FLAGS|= RELOAD_ALLREGIONALL;
			break;
		case ID_MENU_RELOAD_AllREGIONSETUP:
			//װ�����г���������
			RELOAD_PROFILE_FLAGS |= RELOAD_ALLREGION;
			break;

		case ID_CANCEL_ALL_CARDS:
			{
				CCardsHandler::getInstance()->CancelAllCards();
			}
			break;

		case ID_MENE_ReLoadIncrement:
			RELOAD_PROFILE_FLAGS |= RELOAD_INCREMENT;
			break;

		case ID_MENU_RELOAD_CONTRIBUTE_ITEM:
			RELOAD_PROFILE_FLAGS |= RELOAD_CONTRIBUTE;
			break;

		case ID_MENU_RELOADDOCCULIST:
			RELOAD_PROFILE_FLAGS |= RELOAD_DOCCULIST;
			break;

		case ID_Menu_GiftChargeItems:
			GetGame()->ReLoad("GiftItems", true, true);
			break;

		case ID_RELOAD_WORDFILTER:
			RELOAD_PROFILE_FLAGS |= RELOAD_WORDFILTER;
			break;

		///* �ض�����
		case ID_RELOADSETUP:
			{
				GetGame()->LoadSetup();
			}
			break;

		// ��ʾ������Ϣ����
		case ID_SHOWSAVEINFO:
			g_bShowSaveInfo ? g_bShowSaveInfo=false : g_bShowSaveInfo=true;
			break;

		case ID_SWITCH_LOGSYS:
			break;

		// Fox@20081120 �µ��ض�����
		case ID_RELOAD_PLAYER_MAN:			// Fox@20081120 ��ҹ���
			RELOAD_CONFIG_FLAGS |= RELOAD_PLAYER_MAN;
			break;

		case ID_RELOAD_PET_LIST:			// Fox@20081120 ����
			RELOAD_CONFIG_FLAGS |= RELOAD_PET_LIST;
			break;

		// BaiYun@2009525 �ض�������������
		case ID_RELOAD_ANTI_CHEAT:
			RELOAD_CONFIG_FLAGS |= RELOAD_ANTI_CHEAT;
			break;
        //�ض���Ʊ����
		case ID_MENU_RELOADLOTTERY:
			RELOAD_CONFIG_FLAGS |= RELOAD_LOTTERY;
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
		if (wParam!=0 || MessageBox(g_hWnd, "�Ƿ��˳�?", "msg", MB_YESNO)==IDYES)
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
void RefeashInfoText()
{
	static char str[10240];

	if (!GetGame() || !GetGame()->GetNetServer())
	{
		return;
	}
	static long lMaxPlayers = 0;
	static DWORD dwMaxAvaiVirMem = 0;

	static long lMaxDupRgnNum = 0;

	//������������
	static long	lNetTrafficW2L  = 0;
	static long	lNetTrafficL2W  = 0;
	static long	lNetTrafficW2DB  = 0;
	static long	lNetTrafficDB2W  = 0;
	static long	lNetTrafficW2S  = 0;
	static long	lNetTrafficS2W  = 0;
	long lPlayers			= GetGame()->GetMapPlayerCount();
	long lCurDupRgnNum     = GetGame()->GetGlobalRgnManager()->GetRgnMap().size();
	MEMORYSTATUS msC;GlobalMemoryStatus(&msC);
	DWORD dwCurAvaiVirMem = msC.dwAvailVirtual/1048576;

	long	lTemptNetTrafficW2L  = GetGame()->GetNetServer()->GetSendSizePerSecond(NF_LServer_Client);
	long	lTemptNetTrafficL2W  = GetGame()->GetNetServer()->GetRecvSizePerSecond(NF_LServer_Client);
	long	lTemptNetTrafficW2DB  = GetGame()->GetNetServer()->GetSendSizePerSecond(NF_DBServer_Client);
	long	lTemptNetTrafficDB2W  = GetGame()->GetNetServer()->GetRecvSizePerSecond(NF_DBServer_Client);
	long	lTemptNetTrafficW2S  = GetGame()->GetNetServer()->GetSendSizePerSecond(NF_Server_Client);
	long	lTemptNetTrafficS2W  = GetGame()->GetNetServer()->GetRecvSizePerSecond(NF_Server_Client);

	if(lTemptNetTrafficW2L > lNetTrafficW2L)
		lNetTrafficW2L = lTemptNetTrafficW2L;
	if(lTemptNetTrafficL2W > lNetTrafficL2W)
		lNetTrafficL2W = lTemptNetTrafficL2W;
	if(lTemptNetTrafficW2DB > lNetTrafficW2DB)
		lNetTrafficW2DB = lTemptNetTrafficW2DB;
	if(lTemptNetTrafficDB2W > lNetTrafficDB2W)
		lNetTrafficDB2W = lTemptNetTrafficDB2W;
	if(lTemptNetTrafficW2S > lNetTrafficW2S)
		lNetTrafficW2S = lTemptNetTrafficW2S;
	if(lTemptNetTrafficS2W > lNetTrafficS2W)
		lNetTrafficS2W = lTemptNetTrafficS2W;
		
	if (lPlayers>lMaxPlayers)
		lMaxPlayers = lPlayers;

	if(lCurDupRgnNum>lMaxDupRgnNum)
		lMaxDupRgnNum=lCurDupRgnNum;

	if(dwCurAvaiVirMem > dwMaxAvaiVirMem)
		dwMaxAvaiVirMem = dwCurAvaiVirMem;

	_snprintf(str,10240,"OnlinePlayer=%d/%d	AvaiVirMem=%d/%d(MB)\r\n\
��������������=%d/%d\r\n\
NT-W2L=%d/%d(B/S)	NT-L2W=%d/%d(B/S)\r\n\
NT-W2DB=%d/%d(B/S)	NT-DB2W=%d/%d(B/S)\r\n\
NT-W2S=%d/%d(B/S)	NT-S2W=%d/%d(B/S)\r\n",
						lPlayers,lMaxPlayers,dwCurAvaiVirMem,dwMaxAvaiVirMem,
						lCurDupRgnNum,lMaxDupRgnNum,
						lTemptNetTrafficW2L,lNetTrafficW2L,lTemptNetTrafficL2W,lNetTrafficL2W,
						lTemptNetTrafficW2DB,lNetTrafficW2DB,lTemptNetTrafficDB2W,lNetTrafficDB2W,
						lTemptNetTrafficW2S,lNetTrafficW2S,lTemptNetTrafficS2W,lNetTrafficS2W);
	


/*
	sprintf(str, "Last Save Time : %04d-%02d-%02d %02d:%02d:%02d %s\r\n\
This Save Time : %d ��  Saving = %d/%d\r\n\
Map = %d/%d  Online = %d/%d  Offline = %d/%d  Login = %d/%d  Create = %d/%d  Delete = %d/%d  Resume = %d/%d\r\n\
Teams = %d/%d  Largess = %d/%d  WriteLog = %d/%d  LoadPlayer = %d/%d  ReMsg = %d/%d\r\n",
		g_stLastSaveTime.wYear, g_stLastSaveTime.wMonth, g_stLastSaveTime.wDay, g_stLastSaveTime.wHour, g_stLastSaveTime.wMinute, g_stLastSaveTime.wSecond,
		strInfo[nSave],
		g_dwThisSaveStartTime?(timeGetTime() - g_dwThisSaveStartTime)/1000:0,
		lSavePlayers, lMaxSavePlayers, 
		lMapPlayers, lMaxMapPlayers,
		lPlayers, lMaxPlayers,
		lOfflinePlayers, lMaxOfflinePlayers,
		lLoginPlayers, lMaxLoginPlayers,
		lCreatePlayers, lMaxCreatePlayers,
		lDeletePlayers, lMaxDeletePlayers,
		lResumePlayers, lMaxResumePlayers,
		lTeams, lMaxTeams,
		lLargess, lMaxLargess,
		lWriteLog, lMaxWriteLog,
		lLoadPlayer, lMaxLoadPlayer,
		lRebackMessage, lMaxRebackMessage);
*/

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
