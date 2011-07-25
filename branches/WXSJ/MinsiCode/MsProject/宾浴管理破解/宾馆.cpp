// Check.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Check.h"
#include <Tlhelp32.h>

// �궨����ЧID
#define INVALID_ID -1

// �ϵ��ַ
#define INTERMIT_ADDR   (LPVOID)0x006A9E81

// ����ַ
#define CHECK_ADDR      (LPVOID)0x0093E977
//0x003F6EC5
// ��ת��ַ
#define JMP_ADDR1       (LPVOID)0x006A4AB1
#define JMP_ADDR2       (LPVOID)0x006A4B4E
#define JMP_ADDR3       (LPVOID)0x006A4C58
//#define JMP_ADDR4       (LPVOID)0x0094BB74

// ���Ҵ���������
#define FIND_WND_MAX_TIME 1000;

PROCESS_INFORMATION g_ProcessInfo;  // ������Ϣ
DWORD g_dwflOldIntermitProtect;     // �ϵ�Ȩ�ޱ���
DWORD g_dwflOldCheckProtect;        // ����Ȩ�ޱ���
DWORD g_dwflOldJmpProtect;          // ��ת��Ȩ�ޱ���

BOOL g_IsBreakPoint = FALSE;        // �ϵ��Ƿ��Ѿ�����
BOOL g_IsContinueBreakPoint = FALSE;// �Ƿ�����ϵ�ִ��
BOOL g_AllOK = FALSE;               // ����ȫ�����,�˳�

BOOL ChangeOfficeName(VOID);
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

    // �޸ĵ�λ����
    bRet = ChangeOfficeName();
    if (!bRet)
    {
        return INVALID_ID;
    }
    
    // �򿪽���
    bRet = InitProcess();
    if (!bRet)
    {
        return INVALID_ID;
    }

    // ��ȡ�޸�Ȩ��
    bRet = AmendAddrProtect();
    if (!bRet)
    {
        return INVALID_ID;
    }

    // �¶ϵ�
    BYTE bCC = 0xCC;
    SIZE_T NumberOfBytesWritten;
    bRet = ::WriteProcessMemory(g_ProcessInfo.hProcess, INTERMIT_ADDR, &bCC, 0x01, &NumberOfBytesWritten);
    if (!bRet || (0x01 != NumberOfBytesWritten))
    {
        ::MessageBox(::GetActiveWindow(), _T("ע����̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return INVALID_ID;
    }

    DEBUG_EVENT DebugEvent;
    ::CreateThread(NULL, 0, WatchBreakPoint, NULL, 0, NULL);
    do
    {
        g_IsBreakPoint = TRUE;
        ::WaitForDebugEvent(&DebugEvent, INFINITE);

        if ((EXCEPTION_DEBUG_EVENT == DebugEvent.dwDebugEventCode) && (EXCEPTION_BREAKPOINT == DebugEvent.u.Exception.ExceptionRecord.ExceptionCode))
        {
            // ѭ���������Ƿ��Ѿ��ѿ�
            BYTE bProcessCode;
            SIZE_T NumberOfBytesRead;
            bRet = ::ReadProcessMemory(g_ProcessInfo.hProcess, CHECK_ADDR, &bProcessCode, 0x01, &NumberOfBytesRead);
            if (!bRet || (0x01 != NumberOfBytesRead))
            {
                ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }
            if (!bProcessCode)
            {
                ::ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE);
                continue;
            }

            SIZE_T NumberOfBytesWritten;
            // �޸���ת��
            BYTE bCode1[] = {0xE9, 0x8A, 0x00, 0x00, 0x00, 0x90};
            bRet = ::WriteProcessMemory(g_ProcessInfo.hProcess, JMP_ADDR1, bCode1, sizeof(bCode1), &NumberOfBytesWritten);
            if (!bRet || (sizeof(bCode1) != NumberOfBytesWritten))
            {
                ::MessageBox(::GetActiveWindow(), _T("ע����̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }

            // �޸���ת��2
            BYTE bCode2[] = {0xE9, 0xB7, 0x01, 0x00, 0x00, 0x90};
            bRet = ::WriteProcessMemory(g_ProcessInfo.hProcess, JMP_ADDR2, bCode2, sizeof(bCode2), &NumberOfBytesWritten);
            if (!bRet || (sizeof(bCode2) != NumberOfBytesWritten))
            {
                ::MessageBox(::GetActiveWindow(), _T("ע����̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }

            // �޸���ת��3
            BYTE bCode3[] = {0xE9, 0x85, 0x00, 0x00, 0x00, 0x90};
            bRet = ::WriteProcessMemory(g_ProcessInfo.hProcess, JMP_ADDR3, bCode3, sizeof(bCode3), &NumberOfBytesWritten);
            if (!bRet || (sizeof(bCode3) != NumberOfBytesWritten))
            {
                ::MessageBox(::GetActiveWindow(), _T("ע����̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }

            // ȥ���ϵ�
            BYTE b50 = 0x50;
            bRet = ::WriteProcessMemory(g_ProcessInfo.hProcess, INTERMIT_ADDR, &b50, 0x01, &NumberOfBytesWritten);
            if (!bRet || (0x01 != NumberOfBytesWritten))
            {
                ::MessageBox(::GetActiveWindow(), _T("ע����̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
                return INVALID_ID;
            }

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

            // �ָ���ַȨ��
            bRet = ResumeAddrProtect();
            if (!bRet)
            {
                return INVALID_ID;
            }
            break;
        }
        else
        {
            if (EXCEPTION_DEBUG_EVENT == DebugEvent.dwDebugEventCode)
            {
                ::ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
            }
            else
            {
                ::ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE);
            }
        }
    } while (TRUE);

    bRet = ::DebugActiveProcessStop(g_ProcessInfo.dwProcessId);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�����ӳ���ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return INVALID_ID;
    }
	return 0;
}

BOOL ChangeOfficeName(VOID)
{
    HANDLE hFileExe = ::CreateFile(_T("���ݹ���ϵͳ.exe"), FILE_ALL_ACCESS, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFileExe == INVALID_HANDLE_VALUE)
    {
        return TRUE;
    }
    else
    {
        DWORD dwWriteLen = sizeof(TCHAR) * 6;
        ::SetFilePointer(hFileExe, 0x000a8064, 0, FILE_BEGIN);// 0x006A43FF  0x004a8064
        if (!::WriteFile(hFileExe, _T("������Ӫҵ"), dwWriteLen, &dwWriteLen, NULL))
        {
            return FALSE;
        }
        ::CloseHandle(hFileExe);
    }
    return TRUE;
}

BOOL InitProcess(VOID)
{
    BOOL bRet = TRUE;
    STARTUPINFOW StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    StartInfo.cb = sizeof(StartInfo);
    TCHAR* strProcessName = _T("���ݹ���ϵͳ.exe");

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

BOOL AmendAddrProtect(VOID)
{
    BOOL bRet = TRUE;
    // �޸Ķϵ��ַȨ��
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, INTERMIT_ADDR, 0x01, PAGE_EXECUTE_READWRITE, &g_dwflOldIntermitProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �޸ļ����ַȨ��
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, CHECK_ADDR, 0x01, PAGE_EXECUTE_READWRITE, &g_dwflOldCheckProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �޸���ת���ַȨ��
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR1, 0x06, PAGE_EXECUTE_READWRITE, &g_dwflOldJmpProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �޸���ת���ַȨ��2
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR2, 0x06, PAGE_EXECUTE_READWRITE, &g_dwflOldJmpProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �޸���ת���ַȨ��3
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR3, 0x06, PAGE_EXECUTE_READWRITE, &g_dwflOldJmpProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �޸���ת���ַȨ��4
    //bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR4, 0x06, PAGE_EXECUTE_READWRITE, &g_dwflOldJmpProtect);
    //if (!bRet)
    //{
    //    ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
    //    return FALSE;
    //}
    return TRUE;
}

BOOL ResumeAddrProtect(VOID)
{
    BOOL bRet = TRUE;
    // �ָ��ϵ��ַȨ��
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, INTERMIT_ADDR, 0x01, g_dwflOldIntermitProtect, &g_dwflOldIntermitProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�ָ��ϵ��ַʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �ָ�����ַȨ��
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, CHECK_ADDR, 0x01, g_dwflOldCheckProtect, &g_dwflOldCheckProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�ָ�����ַʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �ָ���ת��ַȨ��
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR1, 0x06, g_dwflOldJmpProtect, &g_dwflOldJmpProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �ָ���ת��ַȨ��2
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR2, 0x06, g_dwflOldJmpProtect, &g_dwflOldJmpProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �ָ���ת��ַȨ��3
    bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR3, 0x06, g_dwflOldJmpProtect, &g_dwflOldJmpProtect);
    if (!bRet)
    {
        ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �ָ���ת��ַȨ��4
    //bRet = ::VirtualProtectEx(g_ProcessInfo.hProcess, JMP_ADDR4, 0x06, g_dwflOldJmpProtect, &g_dwflOldJmpProtect);
    //if (!bRet)
    //{
    //    ::MessageBox(::GetActiveWindow(), _T("�򿪽��̿ռ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
    //    return FALSE;
    //}
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
        bRet = ::TerminateProcess(g_ProcessInfo.hProcess, INVALID_ID);
        if (!bRet)
        {
            ::MessageBox(::GetActiveWindow(), _T("��ֹ����ʧ��\nϵͳ�в����˽�����Ƭ,�޷�������"), _T("��ʾ"), MB_OK);
        }
    }
    return 0;
}
