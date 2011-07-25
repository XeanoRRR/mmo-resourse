#include "Precompiled.h"
#pragma comment(lib, "Kernel32.Lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.Lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "GdiPlus")

//������������������������MsBaseDef ��ء���������������
// ȫ�ַ����������������                           //��
DOUBLE      g_dRandRadix[RFREQUENCY] = {0.0f};      //��
                                                    //��
// ȫ�ַ����������������                           //��
UINT        g_uRandRadixIndex               = 0;    //��
                                                    //��
// ȫ�ֻ������ͳ�ʼ���߳�ID                         //��
DWORD       g_BaseInitthreadID              = NULL; //��
                                                    //��
ULONG_PTR   g_lToken                        = 0;    //��
                                                    //��
BOOL        g_IsMsBaseInit                  = FALSE;//��
                                                    //��
// ��ǰʱ���ȫ�ֱ���                               //��
SYSTEMTIME  g_LocalTime                     = {0};  //��
                                                    //��
// ���������������ʹ�õĶ������                   //��
DWORD       g_dwArrayHelperClassParameter   = 0;    //��
MsLock      g_ArrayHelperlock;                      //��
//������������������������������������������������������

//������������������������MsAssertx ��ء�����������������������
#ifdef  NO_LOG          // ����˺�ȥ��������־             //��
INT     g_Command_Assert_Log = COMMAND_IGNORE_LOG;          //��
#else                                                       //��
INT     g_Command_Assert_Log = COMMAND_NORMAL_LOG;          //��
#endif                                                      //��
                                                            //��
#ifdef  NO_MSGBOX       // ����˺�ȥ�����ԶԻ���           //��
INT     g_Command_MessageBox = COMMAND_IGNORE_MSGBOX;       //��
#else                                                       //��
INT     g_Command_MessageBox = COMMAND_NORMAL_MSGBOX;       //��
#endif                                                      //��
                                                            //��
HANDLE  g_hAssertLogFile = NULL;// ȫ�ֶ�����־�ļ����     //��
BOOL    g_bAssertInit = FALSE;  // ȫ�ֶ����Ƿ��ʼ�����   //��
//��������������������������������������������������������������

//������������������������Rand��ء�����������������������������
BOOL InitRand(VOID* /*Parameter*/)                          //��
{                                                           //��
    g_dRandRadix[0] = R_DOUBLE_A(0.0f,1.0f);                //��
    for (DWORD i = 1; i < RFREQUENCY; i++) {                //��
        g_dRandRadix[i] = R_DOUBLE_A(0.0f,1.0f);            //��
        if (g_dRandRadix[i] == g_dRandRadix[i-1]) {         //��
            g_dRandRadix[i] += R_DOUBLE_A(0.0f,1.0f);       //��
        }                                                   //��
    }                                                       //��
    return TRUE;                                            //��
}                                                           //��
//��������������������Rand��ء���������������������������������

//����������������������ʼ�����Ժ�������������������������������
BOOL InitAssertx(TCHAR* szAssertFileName)                   //��
{                                                           //��
    if (!szAssertFileName)                                  //��
    {                                                       //��
        return FALSE;                                       //��
    }                                                       //��
                                                            //��
    TCHAR szAssertPathBuff[MAX_PATH] = {0};                 //��
    TCHAR szCurrentDirectory[MAX_PATH] = {0};               //��
    ::GetCurrentDirectory(MAX_PATH, szCurrentDirectory);    //��
    _stprintf_s(                                            //��
        szAssertPathBuff,                                   //��
        MAX_PATH,                                           //��
        szAssertFileName,                                   //��
        szCurrentDirectory                                  //��
        );                                                  //��
                                                            //��
    // ��־������                                           //��
    TCHAR szLog[ASSERTX_STR_MAX_LEN] = {0};                 //��
                                                            //��
    ::CreateDirectory(_T("./Log/"), NULL);                  //��
    if (!g_hAssertLogFile)                                  //��
    {                                                       //��
        g_hAssertLogFile = ::CreateFile(                    //��
            szAssertPathBuff,                               //��
            FILE_APPEND_DATA,                               //��
            FILE_SHARE_READ,                                //��
            NULL,                                           //��
            OPEN_EXISTING,                                  //��
            0,                                              //��
            NULL                                            //��
            );                                              //��
    }                                                       //��
    if (INVALID_HANDLE_VALUE == g_hAssertLogFile)           //��
    {                                                       //��
        if (ERROR_FILE_NOT_FOUND == ::GetLastError())       //��
        {                                                   //��
            g_hAssertLogFile = ::CreateFile(                //��
                szAssertPathBuff,                           //��
                FILE_APPEND_DATA,                           //��
                FILE_SHARE_READ,                            //��
                NULL,                                       //��
                CREATE_NEW,                                 //��
                NULL,                                       //��
                NULL                                        //��
                );                                          //��
#ifdef UNICODE                                              //��
            // �ɹ�д����־���ֽ���                         //��
            DWORD uLogSize = 0;                             //��
            WORD UnicodeHead = 0xFEFF;                      //��
            uLogSize = sizeof(UnicodeHead);                 //��
            if (!::WriteFile(                               //��
                g_hAssertLogFile,                           //��
                &UnicodeHead,                               //��
                uLogSize,                                   //��
                &uLogSize,                                  //��
                NULL                                        //��
                ))                                          //��
            {                                               //��
                _stprintf_s(                                //��
                    szLog,                                  //��
                    ASSERTX_STR_MAX_LEN,                    //��
                    _T("������־�ļ���ʧ�ܣ�����\n%d"),   //��
                    ::GetLastError()                        //��
                    );                                      //��
                MESSAGEBOX(AL_SHOW_ONLY, szLog);            //��
            }                                               //��
#endif                                                      //��
        }                                                   //��
        else                                                //��
        {                                                   //��
            _stprintf_s(                                    //��
                szLog,                                      //��
                ASSERTX_STR_MAX_LEN,                        //��
                _T("������־�ļ���ʧ�ܣ�����\n%d"),       //��
                ::GetLastError()                            //��
                );                                          //��
            MESSAGEBOX(AL_SHOW_ONLY, szLog);				//��
        }                                                   //��
    }                                                       //��
                                                            //��
    g_bAssertInit = TRUE;                                   //��
    return TRUE;                                            //��
}                                                           //��
//��������������������������������������������������������������

//�����������������������ͷŶ��Ժ�������������������������������
BOOL ReleaseAssertx(VOID* /*Parameter*/)                    //��
{                                                           //��
    // �������ͷ���ֵ                                       //��
    BOOL bRet = TRUE;                                       //��
    // ���������־�ļ��Ѿ���                             //��
    if (g_hAssertLogFile && g_bAssertInit)                  //��
    {                                                       //��
        bRet = ::CloseHandle(g_hAssertLogFile);             //��
        if (bRet)                                           //��
        {                                                   //��
            g_hAssertLogFile = NULL;                        //��
        }                                                   //��
        g_bAssertInit = FALSE;                              //��
        return bRet;                                        //��
    }                                                       //��
    else                                                    //��
    {                                                       //��
        g_bAssertInit = FALSE;                              //��
        return TRUE;                                        //��
    }                                                       //��
}                                                           //��
//��������������������������������������������������������������

//��������������������������������������������������������������
//������                                                    //��
MsLock::MsLock()                                            //��
{                                                           //��
    ::InitializeCriticalSection(&m_Lock);                   //��
}                                                           //��
                                                            //��
//������ɾ��������                                        //��
MsLock::~MsLock()                                           //��
{                                                           //��
    ::DeleteCriticalSection(&m_Lock);                       //��
}                                                           //��
                                                            //��
//�����ٽ���                                                //��
inline VOID MsLock::ReSet()                                 //��
{                                                           //��
    ::InitializeCriticalSection(&m_Lock);                   //��
}                                                           //��
                                                            //��
//�����ٽ���                                                //��
inline VOID MsLock::Lock()                                  //��
{                                                           //��
    ::EnterCriticalSection(&m_Lock);                        //��
}                                                           //��
                                                            //��
//�뿪�ٽ���                                                //��
inline VOID MsLock::UnLock()                                //��
{                                                           //��
    ::LeaveCriticalSection(&m_Lock);                        //��
}                                                           //��
                                                            //��
inline AutoLock::AutoLock(MsLock& rLock, BOOL bUse)         //��
{                                                           //��
    m_bUse = bUse;                                          //��
    if (m_bUse)                                             //��
    {                                                       //��
        m_pLock = &rLock;                                   //��
        m_pLock->Lock();                                    //��
    }                                                       //��
}                                                           //��
                                                            //��
//�����н���                                                //��
inline AutoLock::~AutoLock()                                //��
{                                                           //��
    if (m_bUse)                                             //��
    {                                                       //��
        m_pLock->UnLock();                                  //��
    }                                                       //��
}                                                           //��
//��������������������������������������������������������������

VOID InitMsBase(
    TCHAR* szAssertFileName,
    BOOL bConsole,
    RUNCALLBACK pRunCallBack,
    LPTSTR szClassName,
    LPTSTR szWndName,
    HOOKWNDPROC HookWndProc,
    WNDPROC WndProc,
    PTHREAD_START_ROUTINE ThcreadProc
    )
{
    USE_PARAM(ThcreadProc, WndProc, HookWndProc, szWndName, szClassName, pRunCallBack, bConsole);
    g_IsMsBaseInit = FALSE;
    if (g_IsMsBaseInit)
    {
        return;
    }

    // ���õ��Ա��
    _DEBUG_CODE(_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF));
    setlocale(LC_ALL, "chs");

    InitRand(NULL);
    if (szAssertFileName)
    {
        InitAssertx(szAssertFileName);
    }

    WSADATA WsaData; // Winsock��ϸ��Ϣ
    // ��ʼ�� Winsock
    INT nRet = ::WSAStartup(MAKEWORD(2,2), &WsaData);
    AssertEx(AL_CATAST, !nRet, _T("WSAStartupʧ��,����ʧ��"));

#ifndef USE_MFC
    if (bConsole)
    {
        if (pRunCallBack)
        {
            g_pCMsConsole = NEW CMsConsole(pRunCallBack);
        }
    }
    else if (WndProc || ThcreadProc)
    {
        g_pCMsWinMain = NEW CMsWinMain(
            szClassName,
            szWndName,
            HookWndProc,
            WndProc,
            ThcreadProc,
            NULL
            );

        Gdiplus::GdiplusStartupInput Input;
        Gdiplus::GdiplusStartup(&g_lToken, &Input, NULL);
    }
    else
    {
        AssertEx(AL_CATAST, FALSE, _T("��CMsBaseRun������ִ�к�������ȫ��Ϊ��"));
    }
#endif
    g_IsMsBaseInit = TRUE;
}


VOID ReleaseMsBase(LPVOID)
{
    if (!g_IsMsBaseInit)
    {
        return;
    }
    if (g_lToken)
    {
        Gdiplus::GdiplusShutdown(g_lToken);
    }

#ifndef USE_MFC
    HANDLE hProcess32Snap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcess32Snap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32;
        ZeroMemoryStruct(pe32);
        pe32.dwSize = sizeof(PROCESSENTRY32);
        do 
        {
            ::SleepEx(10, TRUE);
            if (::Process32First(hProcess32Snap, &pe32))
            {
                if (pe32.cntThreads > 1)
                {
                    break;
                }
            }
            else
            {
                AssertEx(AL_CATAST, FALSE, _T("�޷��鿴�߳���Ϣ��"));
                break;
            }
        } while (pe32.cntThreads > 1);
    }
    ::CloseHandle(hProcess32Snap);
    SAFE_DELETE(g_pCMsWinMain);
    SAFE_DELETE(g_pCMsConsole);
    ::WSACleanup();
#endif
    ReleaseAssertx(NULL);
    g_IsMsBaseInit = FALSE;
}