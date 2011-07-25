// Check.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Check.h"

// �궨����ЧID
#define INVALID_ID -1

// �ϵ��ַ
#define INTERMIT_ADDR   (LPVOID)0x007C7C8E

// ������ַ
//#define CHECK_ADDR1     (LPVOID)0x0040D2BD
//#define CHECK_ADDR2     (LPVOID)0x0040D2C2

PROCESS_INFORMATION g_ProcessInfo;  // ������Ϣ
DWORD g_dwflOldIntermitProtect;     // �ϵ�Ȩ�ޱ���
DWORD g_dwflOldCheckProtect;        // ����Ȩ�ޱ���

BOOL g_IsBreakPoint = FALSE;        // �ϵ��Ƿ��Ѿ�����
//BOOL g_IsContinueBreakPoint = TRUE; // �Ƿ�����ϵ�ִ��
//BOOL g_IsFirst = TRUE;              // ����ȫ�����,�˳�

BOOL InitProcess(VOID);
BOOL AmendAddrProtect(VOID);
BOOL ResumeAddrProtect(VOID);

DWORD WINAPI WatchBreakPoint(LPVOID lpThreadParameter);

INT WINAPI _tWinMain(
                     HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     INT nShowCmd)
{
    BOOL bRet = TRUE;

    // �򿪽���
    bRet = InitProcess();
    if (!bRet)
    {
        return INVALID_ID;
    }

    //// ��ȡ�޸�Ȩ��
    //bRet = AmendAddrProtect();
    //if (!bRet)
    //{
    //    return INVALID_ID;
    //}

    // �¶ϵ�
    BYTE bCC = 0xCC;
    //BYTE b66 = 0x66;
    BYTE b8B = 0x8B;
    SIZE_T NumberOfBytesWritten;
    CONTEXT con;
    con.ContextFlags = CONTEXT_ALL;

    ::VirtualProtectEx(g_ProcessInfo.hProcess, INTERMIT_ADDR, 0x01, PAGE_EXECUTE_READWRITE, &g_dwflOldIntermitProtect);
    ::WriteProcessMemory(g_ProcessInfo.hProcess, INTERMIT_ADDR, &bCC, 0x01, &NumberOfBytesWritten);

    DEBUG_EVENT DebugEvent;
    ::CreateThread(NULL, 0, WatchBreakPoint, NULL, 0, NULL);
    static INT i = 0;
    g_IsBreakPoint = TRUE;
    do
    {
        ::WaitForDebugEvent(&DebugEvent, INFINITE);
        ::GetThreadContext(g_ProcessInfo.hThread, &con);

        if ((EXCEPTION_DEBUG_EVENT == DebugEvent.dwDebugEventCode) && (EXCEPTION_BREAKPOINT == DebugEvent.u.Exception.ExceptionRecord.ExceptionCode) && (con.Eip < 0x70000000))
        {
            CHAR szOfficeName[] = "����ϴԡ";
            NumberOfBytesWritten = sizeof(szOfficeName);

            ::VirtualProtectEx(g_ProcessInfo.hProcess, (LPVOID)con.Eax, NumberOfBytesWritten, PAGE_EXECUTE_READWRITE, &g_dwflOldCheckProtect);
            ::WriteProcessMemory(g_ProcessInfo.hProcess, (LPVOID)con.Eax, szOfficeName, NumberOfBytesWritten, &NumberOfBytesWritten);
            ::VirtualProtectEx(g_ProcessInfo.hProcess, (LPVOID)con.Eax, NumberOfBytesWritten, g_dwflOldCheckProtect, &g_dwflOldCheckProtect);

            struct  
            {
                BYTE    bCode;
                DWORD   dwCode;
                WORD    wCode;
            } s_Code;
            s_Code.bCode   = 0xB8;
            s_Code.dwCode  = 0x00000003;
            s_Code.wCode   = 0x9090;
            NumberOfBytesWritten = sizeof(s_Code);

            ::VirtualProtectEx(g_ProcessInfo.hProcess, (LPVOID)0x009E77DC, NumberOfBytesWritten, PAGE_EXECUTE_READWRITE, &g_dwflOldCheckProtect);
            ::WriteProcessMemory(g_ProcessInfo.hProcess, (LPVOID)0x009E77DC, &s_Code, NumberOfBytesWritten, &NumberOfBytesWritten);
            ::VirtualProtectEx(g_ProcessInfo.hProcess, (LPVOID)0x009E77DC, NumberOfBytesWritten, g_dwflOldCheckProtect, &g_dwflOldCheckProtect);

            // ȥ��ǰ�ϵ�
            bRet = ::WriteProcessMemory(g_ProcessInfo.hProcess, INTERMIT_ADDR, &b8B, 0x01, &NumberOfBytesWritten);
            if (!bRet || (0x01 != NumberOfBytesWritten))
            {
                ::MessageBox(::GetActiveWindow(), _T("ע����̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }

            // ��ȡ�Ĵ���
            CONTEXT context;
            context.ContextFlags = CONTEXT_ALL;
            if (!::GetThreadContext(g_ProcessInfo.hThread, &context))
            {
                ::MessageBox(::GetActiveWindow(), _T("����ϵ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }
            context.Eip = (DWORD)INTERMIT_ADDR;
            if (!::SetThreadContext(g_ProcessInfo.hThread, &context))
            {
                ::MessageBox(::GetActiveWindow(), _T("����ϵ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }
            ::ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE);
            break;
        }
        else
        {
            ::ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE);
        }
    } while (TRUE);

    ::VirtualProtectEx(g_ProcessInfo.hProcess, INTERMIT_ADDR, 0x01, g_dwflOldIntermitProtect, &g_dwflOldIntermitProtect);
    ::DebugActiveProcessStop(g_ProcessInfo.dwProcessId);
    return 0;
}

BOOL InitProcess(VOID)
{
    BOOL bRet = TRUE;
    STARTUPINFOW StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    StartInfo.cb = sizeof(StartInfo);
    TCHAR* strProcessName = _T("HTClient.exe");

    // ��������
    bRet = ::CreateProcess(
        strProcessName,
        strProcessName,
        NULL,
        NULL,
        FALSE,
        DEBUG_ONLY_THIS_PROCESS | CREATE_SUSPENDED,
        NULL,
        NULL,
        &StartInfo,
        &g_ProcessInfo);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("������Ŀʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

DWORD WINAPI WatchBreakPoint(LPVOID lpThreadParameter)
{
    BOOL bRet = TRUE;
    do 
    {
        ::SleepEx(100, TRUE);
    } while (!g_IsBreakPoint);
    // ���н���
    if (INVALID_ID == ::ResumeThread(g_ProcessInfo.hThread))
    {
        ::MessageBox(::GetActiveWindow(), _T("������Ŀʧ�ܣ�"), _T("��ʾ"), MB_OK);
        ::TerminateProcess(g_ProcessInfo.hProcess, INVALID_ID);
    }
    return 0;
}
