#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE g_hInst;								        // ��ǰʵ��
TCHAR g_szTitle[MAX_LOADSTRING] = "AppFrame";         	// �������ı�
TCHAR g_szWindowClass[MAX_LOADSTRING] = "AppFrame";		// ����������
unsigned (__stdcall *mainThreadFunc)(void*);


#include "AppFrameDef.h"
#include "../Package/ConfigParse.h"

namespace AppFrame
{
    ATOM				MyRegisterClass(HINSTANCE hInstance);
    BOOL				InitInstance(HINSTANCE, int);
    LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

    BOOL InitApp(HINSTANCE hInstance, int nCmdShow, unsigned (__stdcall* func)(void*))
    {
        EnableDebugNew();
        mainThreadFunc = func;

        MSG msg;
        memset(&msg, 0, sizeof(MSG));
        MyRegisterClass(hInstance);

        // ִ��Ӧ�ó����ʼ��:
        if (!InitInstance (hInstance, nCmdShow))
        {
            return FALSE;
        }

        // ����Ϣѭ��:
        while(1)
        {
            if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
            {
                if( !GetMessage( &msg, NULL, 0, 0 ) )
                {
                    FiniConfig();
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
        wcex.hIcon			= LoadIcon(hInstance, NULL);
        wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName	= NULL;
        wcex.lpszClassName	= g_szWindowClass;
        wcex.hIconSm		= LoadIcon(wcex.hInstance, NULL);

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
        g_hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
        HWND hWnd = CreateWindow(g_szWindowClass, g_szTitle,
            /*WS_DLGFRAME|WS_SYSMENU|WS_MINIMIZEBOX*/WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
            NULL, NULL, hInstance, NULL);

        if (!hWnd)
        {
            return FALSE;
        }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);
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

            // �Զ���ؼ���Ϣ����
            if (MsgProc((HWND)lParam, wmId, wmEvent))
                return 0;
            // �����˵�ѡ��:
            switch (wmId)
            {
                // TODO:
                // �������ͨ�ò˵�����, �� ABOUT
            case 0:
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;

        case WM_CREATE:
            if (!InitConfig(hWnd))
                PostQuitMessage(0);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ���������ͼ����...
            EndPaint(hWnd, &ps);
            break;

        case WM_WINDOWPOSCHANGED:
            {
                int cx = ((LPWINDOWPOS)lParam)->cx;
                int cy = ((LPWINDOWPOS)lParam)->cy;
                RECT rect;
                GetWindowRect(hWnd,&rect);
                int frameHeight = GetFrameHeight();
                int frameWidth = GetFrameWidth();

                HWND subWnd = GetWnd(INFO);
                if (!subWnd) break;
                Layouts::Window* wnd = GetWnd(subWnd);
                if (wnd)
                {
                    int x = wnd->x;
                    int y = wnd->y;
                    int width = wnd->width;
                    int height = wnd->height;
                    ::SetWindowPos(subWnd,
                        0,
                        x,
                        y,
                        (rect.right-rect.left)-frameWidth+width,
                        height,
                        SWP_SHOWWINDOW | SWP_NOZORDER);
                }

                subWnd = GetWnd(LOG);
                if (!subWnd) break;
                wnd = GetWnd(subWnd);
                if (wnd)
                {
                    int x = wnd->x;
                    int y = wnd->y;
                    int width = wnd->width;
                    int height = wnd->height;
                    ::SetWindowPos(subWnd,
                        0,
                        x,
                        y,
                        (rect.right-rect.left)-frameWidth+width,
                        (rect.bottom-rect.top)-frameHeight+height,
                        SWP_SHOWWINDOW | SWP_NOZORDER);
                }

                subWnd = GetWnd(LIST);
                if (!subWnd) break;
                wnd = GetWnd(subWnd);
                if (wnd)
                {
                    int x = wnd->x;
                    int y = wnd->y;
                    int width = wnd->width;
                    int height = wnd->height;
                    ::SetWindowPos(subWnd,
                        0,
                        (rect.right-rect.left)-frameWidth + x,
                        y,
                        width,
                        (rect.bottom-rect.top)-frameHeight+height,
                        SWP_SHOWWINDOW | SWP_NOZORDER);
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_CLOSE:
            if (NotifyMessageBox("�Ƿ��˳�?", "msg", MB_YESNO)==IDYES)
                PostQuitMessage(0);
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
}
