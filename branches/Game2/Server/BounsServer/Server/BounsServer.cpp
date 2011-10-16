// BounsServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "BounsServer.h"

#include "Game.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

const long	CW_WIDTH		=	700;			// �����ڿ�
const long	CW_HEIGHT		=	500;			// �����ڸ�
const long	INFOTEXT_WIDTH	=	200;			// ��Ϣ��ʾ���ڿ�
const long	INFOTEXT_HEIGHT	=	140;			// ��Ϣ��ʾ���ڸ�

HWND g_hWnd;
HWND g_hInfoText;
HWND g_hLogText;
HWND g_hClientListBox;

bool g_bDealMsgState	= true;	// ��ǰ������Ϣ������
long g_lLoadMsgNum		= 0;		// �����ѯ����Ϣ����
long g_lDealLoadMsgNum	= 0;		// ������Ĳ�ѯ��Ϣ����
long g_lFreezeMsgNum	= 0;		// ���󶳽����Ϣ����
long g_lFreezeBakNum	= 0;		// ������Ķ�����Ϣ����
long g_lObtainMsgNum	= 0;		// ��ȡ��������Ϣ����
long g_lObtainBakNum	= 0;		// ������Ļ�ȡ��Ϣ����
long g_lConnectedGSNum	= 0;		// �����ϵ�GS����


// ������־
void SaveLogText(bool bForce);

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BOUNSDB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOUNSDB));

	unsigned int threadID;
	HANDLE hGameThread = (HANDLE)_beginthreadex(NULL, 0, GameThreadFunc, NULL, 0, &threadID);

	g_hGameThreadExitEvent = CreateEvent(NULL, 0, 0, 0);

	// ����Ϣѭ��:
	while(1)
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		{
			if( !GetMessage(&msg, NULL, 0, 0) )
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

	// ������־��Ϣ
	SaveLogText(true);
	PutLogInfo("\r\n//////////////////////////////////");
	PutLogInfo("\r\n//       BounsServer Server Exit	//");
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
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOUNSDB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BOUNSDB);
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

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_WIDTH, CW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   RECT rect;
   GetClientRect(g_hWnd, &rect);
   long lWeiht  = rect.right - rect.left;
   long lHeight = rect.bottom - rect.top;

   g_hInfoText	= CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,0,0,rect.right-INFOTEXT_WIDTH, INFOTEXT_HEIGHT-4,g_hWnd,0,hInst,NULL);
   g_hLogText	= CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_READONLY |LBS_STANDARD ,0,INFOTEXT_HEIGHT,rect.right-INFOTEXT_WIDTH, rect.bottom-INFOTEXT_HEIGHT,g_hWnd,0,hInst,NULL);
   g_hClientListBox	= CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_STANDARD,rect.right-INFOTEXT_WIDTH,0,INFOTEXT_WIDTH,rect.bottom,g_hWnd,0,hInst,NULL);
   SendMessage(g_hLogText, EM_SETLIMITTEXT, -1, 0);

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
			{
				if(MessageBox(g_hWnd, "�Ƿ��˳�?", "��ʾ", MB_YESNO) == IDYES)
				{
					DestroyWindow(hWnd);
				}
			}
			break;

			// �ض�GSSetup.ini
		case ID_OPR_RELAOD_GSSETUP:
			{
				GetGame()->ReLoadGSSetup();
			}
			break;
			// ������Ϣ������
		case ID_OPR_MSG:
			{
				HMENU hMenu = GetMenu(g_hWnd);
				UINT uState = GetMenuState(hMenu, ID_OPR_MSG,MF_BYCOMMAND);
				if(uState == MF_CHECKED)
				{
				//	ModifyMenu(hMenu, ID_OPR_MSG, 0, ID_OPR_MSG,"DealMsg(Close)" );
					CheckMenuItem(hMenu,ID_OPR_MSG, MF_BYCOMMAND | MF_UNCHECKED); 
					g_bDealMsgState = true;
				}
				else
				{
				//	ModifyMenu(hMenu, ID_OPR_MSG, 0, ID_OPR_MSG,"DealMsg(Open)" );
					CheckMenuItem(hMenu,ID_OPR_MSG, MF_BYCOMMAND | MF_CHECKED); 
					g_bDealMsgState = false;
				}

				RefeashInfoText();
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
		{
			if(MessageBox(g_hWnd, "�Ƿ��˳�?", "��ʾ", MB_YESNO) == IDYES)
			{
				PostQuitMessage(0);
			}
		}
		break;
	case WM_SIZE:
		{
			long lW = LOWORD(lParam);
			long lH = HIWORD(lParam);
			MoveWindow(g_hInfoText, 0,0, lW-INFOTEXT_WIDTH, INFOTEXT_HEIGHT-4, true);
			MoveWindow(g_hLogText, 0, INFOTEXT_HEIGHT, lW-INFOTEXT_WIDTH, lH-INFOTEXT_HEIGHT, true);
			MoveWindow(g_hClientListBox, lW-INFOTEXT_WIDTH,0,INFOTEXT_WIDTH,lH, true);
		}
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


// ������־
void SaveLogText(bool bForce)
{
	// log��Ϣ��󳤶�
	const int lMaxLength = 64000;
	static char str[lMaxLength];

	// д��־�ļ�
	static DWORD dwLastSaveTime = timeGetTime();
	if( bForce ||
		timeGetTime() - dwLastSaveTime > GetGame()->GetSetup()->dwSaveInfoTime ||
		GetWindowTextLength(g_hLogText) >= lMaxLength )
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
	// log��Ϣ��󳤶�
	const int lMaxLength = 64000;
	static char str[lMaxLength];

	// д��־�ļ�
	SaveLogText(false);

	// д����������
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(str, "[%02d-%02d %02d:%02d:%02d] ", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	va_list va;
	va_start(va, msg);
	vsprintf(&str[strlen(str)],msg,va);
	va_end(va);

	strcat(str, "\r\n");

	PutLogInfo(str);

	SendMessage(g_hLogText, EM_SETSEL, 0, -1);
	SendMessage(g_hLogText, EM_SETSEL, -1, 0);
	SendMessage(g_hLogText, EM_REPLACESEL, FALSE, (LPARAM)str);
}

// ����һ������ļ�¼
void AddErrorLogText(char* msg, ...)
{
	// log��Ϣ��󳤶�
	const int lMaxLength = 64000;
	static char str[lMaxLength];

	// д��־�ļ�
	SaveLogText(false);

	// д����������
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(str, "[%02d-%02d %02d:%02d:%02d] <error>", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	va_list va;
	va_start(va, msg);
	vsprintf(&str[strlen(str)],msg,va);
	va_end(va);

	strcat(str, "\r\n");

	PutLogInfo(str);

	SendMessage(g_hLogText, EM_SETSEL, 0, -1);
	SendMessage(g_hLogText, EM_SETSEL, -1, 0);
	SendMessage(g_hLogText, EM_REPLACESEL, FALSE, (LPARAM)str);
}

void RefeashInfoText()
{
	static char str[1024];
	sprintf(str, "GSConnects: %d/%d\r\n\
��ǰ��Ϣ����״̬:%s\r\n\
����(��ѯ/����):%d/%d\r\n\
����(����/����): %d/%d\r\n\
����(��ȡ/����): %d/%d\r\n",
		GetGame()->GetGSMapSize(), g_lConnectedGSNum,
		(g_bDealMsgState==true)?("��"):("�ر�"), 
		g_lLoadMsgNum, g_lDealLoadMsgNum, 
		g_lFreezeMsgNum,g_lFreezeBakNum, 
		g_lObtainMsgNum, g_lObtainBakNum);
	SetWindowText(g_hInfoText, str);
}
