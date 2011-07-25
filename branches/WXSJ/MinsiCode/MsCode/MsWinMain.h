#pragma once            //��//��
#ifndef __MSWINMAIN_H__ //��//��
#define __MSWINMAIN_H__ //��//��
//��������������������������//��
//�ļ����ƣ�MsWinMain.h     //��
//����������WinMain���     //��
//ƽ̨��أ�Windows         //��
//������������������������������

// HookApi��¼�ṹ��
struct HOOKAPI32
{
    TCHAR*      m_strFunc;
    DWORD       m_pOldProc;
    DWORD       m_pNewProc;
};

struct HOOKAPI64
{
    TCHAR*      m_strFunc;
    ULONG_PTR   m_pOldProc;
    ULONG_PTR   m_pNewProc;
};

#ifdef _WIN64
#define HOOKAPI HOOKAPI64
#else
#define HOOKAPI HOOKAPI32
#endif

struct S_2DEngineWndInfo
{
    BOOL    m_bTopMost;
    INT     m_nX;
    INT     m_nY;
    INT     m_nWidth;
    INT     m_nHeight;
    DWORD   m_dwStyle;
};

// WinMainִ������
enum E_WIN_MAIN_FLOW
{
    E_WIN_MAIN_FLOW_NORMAL = 0,     // ��׼����
    E_WIN_MAIN_FLOW_2DENGINE,       // 2D��������


};
class CMsWinMain
    : public CMsThread
{
public:
    enum E_ERROR_CODE
    {
        ERROR_UNKNOW = -1,          // δ֪����
        ERROR_NO_ERROR = 0,         // �޴���
        ERROR_UNSPECIFIED_DIALOGID, // δָ���Ի���ID
        ERROR_OPERATE_EXCEPTION,    // �����쳣
    };

public:

    // ���캯��
    CMsWinMain(
        LPTSTR szClassName,
        LPTSTR szWndName,
        HOOKWNDPROC HookWndProc,
        WNDPROC WndProc,
        PTHREAD_START_ROUTINE ThreadProc,
        LPVOID lpCreateParameter
        );

    static LRESULT CALLBACK CMsWinMainWndProc(HWND,UINT,WPARAM,LPARAM);

    virtual BOOL ForeStop(VOID);        // ֹ֮ͣǰ
    virtual BOOL ThreadCallBack(VOID);  // �̻߳ص�

    // ��ʼ��
    VOID Init(
        __in HINSTANCE hInstance,           // Ӧ�ó���ʵ�����
        __in_opt HINSTANCE hPrevInstance,   // ǰһ���γ����ʵ�����
        __in LPTSTR lpCmdLine,              // �����в���
        __in INT nShowCmd,                  // ������ʾģʽ
        S_2DEngineWndInfo* s_pewi
        );

    // ���ô��ڿ�ݼ���ID
    HACCEL SetAcceleratorID(DWORD dwAcceleratoID);

    DWORD GetWndThreadId(VOID);                 // ��ȡ�����߳�Id
    VOID WaitQuit(VOID);                        // �ȴ�����
    LPTCHAR GetWinCommandLine(VOID);            // ��ȡ�����в���
    HINSTANCE GetHInstance(VOID);               // ��ȡʵ�����
    HINSTANCE GetPrevHInstance(VOID);           // ��ȡ��һ��ʵ�����
    HWND GethWnd(VOID);                         // ��ȡ���ھ��

    // ֱ��Ͷ����Ϣ
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

    BOOL SetWndName(LPTSTR szWndName);          // ���ô�������
    BOOL SetWinShow(INT nShowCmd);              // ������ʾ��ʽ
    BOOL SetWinMenu(HMENU hMenu);               // ���ò˵�
    BOOL CloseWindow(VOID);                     // �رմ���
    VOID SetMaximize(BOOL bEnabled);            // ������󻯰�ť
    VOID SetMinimize(BOOL bEnabled);            // ������С����ť
    VOID SetSysMenu(BOOL bEnabled);             // ����ϵͳ�˵�
    VOID SetIcon(INT nIconId, BOOL bBigIcon);   // ����ͼ��
    VOID SetIcon(HICON hIcon, BOOL bBigIcon);   // ����ͼ��
    VOID SetEnabled(BOOL bEnabled);             // �Ƿ񼤻�
    VOID SetMinSize(POINT& Pos);                // ������С�ߴ�
    VOID SetMaxSize(POINT& Pos);                // �������ߴ�
    VOID SetLockSize(POINT& Pos);               // ���������ߴ�
    VOID SetLockSize(DWORD dwWidth, DWORD dwHeight);
    VOID SetClientSize(POINT& Pos);             // ���ô��ڿͻ����ߴ�

    // ����ϵͳ����ͼ��
    VOID SetSysIcon(UINT IconID);

    // ���ϵͳ����
    BOOL AddSysIcon(UINT IconID, TCHAR* szToolTip);

    // ɾ��ϵͳ����
    BOOL DelSysIcon(UINT IconID);

    // ����ϵͳ������Ϣ
    BOOL PopSysIconMsg(UINT IconID, TCHAR* szTitle, TCHAR* szInfo);

    VOID SetIconPreCreate(DWORD dwIconID);      // ����ͼ��(����������ǰ����)
    VOID SetIconPreCreate(HICON m_hIcon);       // ����ͼ��(����������ǰ����)

    // ��ȡIAT��Ϣ
    static VOID LocationIAT(
        HMODULE hModule,
        LPTCHAR szImportMod,
        PIMAGE_IMPORT_DESCRIPTOR& lpImportDesc
        );

    // �滻IAT��¼
    static BOOL HookAPIByName(
        HMODULE hModule,
        LPTCHAR szImportMod,
        HOOKAPI* pHookApi
        );

private:
    BOOL MsCreateMainWindow(VOID);      // ����������
    BOOL MsLeachMsg(VOID);              // �ڲ�������Ϣ
    HINSTANCE       m_hInstance;
    HINSTANCE       m_hPrevInstance;
    INT             m_nShowCmd;
    HWND            m_hMainWnd;
    WNDPROC         m_AWndProc;
    HOOKWNDPROC     m_HookWndProc;
    PTHREAD_START_ROUTINE m_ThreadProc;
    HICON           m_hAIcon;
    HICON           m_hSysIcon;
    TCHAR           m_szAstrWndClassName[MAX_PATH];
    TCHAR           m_szAstrWndName[MAX_PATH];
    TCHAR           m_szCommandLine[MAX_PATH];
    HACCEL          m_MsRun_hAccelTable;
    E_ERROR_CODE    m_eWinMainErrorCode;
    DWORD           m_dwMsgThreadId;
    LPVOID          m_lpCreateParameter;    // �������������ݵĲ���
    BOOL            m_bTopMost;
    INT             m_nX;
    INT             m_nY;
    INT             m_nWidth;
    INT             m_nHeight;
    DWORD           m_dwStyle;
    BOOL            m_bMinLockSize;
    BOOL            m_bMaxLockSize;
    POINT           m_MinSize;
    POINT           m_MaxSize;
    NOTIFYICONDATA  m_nId;
    MSG             m_Msg;
};

extern CMsWinMain* g_pCMsWinMain;


//����������������������
#endif//#pragma once//��
//����������������������
