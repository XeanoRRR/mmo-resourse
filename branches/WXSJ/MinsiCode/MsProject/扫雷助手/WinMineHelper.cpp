#include "stdafx.h"
#include <Tlhelp32.h>

// �������
#define ERROR_CODE      -1

// ɨ�׳��򴰿�����
#define MINE_PROCESS_WINDOW_NAME    "ɨ��"

// �������߶�
#define MAX_MINE_HEIGHT         26

// ���������
#define MAX_MINE_WIDTH          32

// �����׸���
#define MAX_MINE_COUNT          667

// ɨ�׼�ʱ
#define TIMER_COUNT     (LPVOID)0x0100579C
#define TIMER_COUNT_LEN 0x04

// ɨ���������
#define MINE_BEGIN_FLAG_ADDRESS     (LPVOID)0x01005160
#define MINE_BEGIN_FLAG_ADDRESS_LEN 0x04

// ���׸���
#define MINE_COUNT_ADDRESS      (LPVOID)0x01005330
#define MINE_COUNT_ADDRESS_LEN  0x04
#define MINE_COUNT_ADDRESS2     (LPVOID)0x01005194
#define MINE_COUNT_ADDRESS2_LEN 0x04

// �������
#define MINE_WIDTH_ADDRESS      (LPVOID)(((ULONG_PTR)MINE_COUNT_ADDRESS) + MINE_COUNT_ADDRESS_LEN)
#define MINE_WIDTH_ADDRESS_LEN  0x04

// �����߶�
#define MINE_HEIGHT_ADDRESS     (LPVOID)(((ULONG_PTR)MINE_WIDTH_ADDRESS) + MINE_WIDTH_ADDRESS_LEN)
#define MINE_HEIGHT_ADDRESS_LEN 0x04

// ɨ�׼���ַ
#define MINE_CHECK_ADDRESS      (LPVOID)(((ULONG_PTR)MINE_HEIGHT_ADDRESS) + MINE_HEIGHT_ADDRESS_LEN)
#define MINE_CHECK_ADDRESS_LEN  0x04

// ������ַ
#define MINE_ADDRESS            (LPVOID)(((ULONG_PTR)MINE_CHECK_ADDRESS) + MINE_CHECK_ADDRESS_LEN)

// ���������ֵ
#define MINE_ADDRESS_MAX_LEN    (MAX_MINE_HEIGHT) * (MAX_MINE_WIDTH)

// ɨ��ʣ��յص�ַ
#define MINE_MAX_SPACE_ADDRESS      (LPVOID)0x010057A0
#define MINE_MAX_SPACE_ADDRESS_LEN  0x04

// ɨ��ʣ��յص�ַ
#define MINE_CURR_SPACE_ADDRESS      (LPVOID)(((ULONG_PTR)MINE_MAX_SPACE_ADDRESS) + MINE_MAX_SPACE_ADDRESS_LEN)
#define MINE_CURR_SPACE_ADDRESS_LEN  0x04

enum E_MINE_FLAG
{
    E_NUMBER_ERROR                  = -2,   // ����
    E_NUMBER_UNKNOW                 = 0x0F, // δ֪
    E_NUMBER_EMPTY                  = 0x40, // ��
    E_NUMBER_1                      = 0x41, // ����1
    E_NUMBER_2                      = 0x42, // ����2
    E_NUMBER_3                      = 0x43, // ����3
    E_NUMBER_4                      = 0x44, // ����4
    E_NUMBER_5                      = 0x45, // ����5
    E_NUMBER_6                      = 0x46, // ����6
    E_NUMBER_7                      = 0x47, // ����7
    E_NUMBER_8                      = 0x48, // ����8
    E_NUMBER_NO_MINE_INCERTITUDE    = 0x0D, // ���ױ��Ϊ��
    E_NUMBER_IS_MINE_INCERTITUDE    = 0x8D, // ���ױ��Ϊ��
    E_NUMBER_NO_MINE_FLAG           = 0x0E, // ���ױ��Ϊ��
    E_NUMBER_MINE                   = 0x8F, // ����
    E_NUMBER_IS_MINE_FLAG           = 0x8E, // ���ױ��Ϊ��
    E_NUMBER_WALL                   = 0x10, // ǽ
};


// ��ȡ�����ڴ��е���Ϣ
BOOL MsReadProcessMemory(HANDLE hProcess, LPVOID lpAddr, LPVOID lpBuff, ULONG_PTR ulReadLen);

// д����Ϣ�������ڴ���
BOOL MsWriteProcessMemory(HANDLE hProcess, LPVOID lpAddr, LPVOID lpBuff, ULONG_PTR ulReadLen);

// ��ȡɨ���ڴ��е���Ϣ
BOOL MsReadWinMineData(VOID);

// д����Ϣ��ɨ���ڴ���
BOOL MsWriteWinMineData(VOID);

// ����������Ϣ
BOOL MsUpdateMineAreaData(BOOL bToMineProcess/*TRUE->���µ�ɨ����, FALSE->���µ�����*/);

// ��ȡ�׵ı��
E_MINE_FLAG GetMineFlag(INT X, INT Y);

// ������Χ�׵ĸ���
DWORD FindAroundMineCount(INT X, INT Y);

// �޸��׵ı��
VOID ChangeMineFlag(INT X, INT Y, E_MINE_FLAG eMineFlag);

// ɨ��
BOOL DightMine(VOID);

// ����
BOOL ClearMine(VOID);

// ����
BOOL SeeMine(VOID);

INT_PTR CALLBACK MainDialogProc(
    HWND hwndDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    );

INT WINAPI _tWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    INT nShowCmd)
{
    return ::DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), NULL, MainDialogProc);
}

INT_PTR CALLBACK MainDialogProc(
    HWND hwndDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            while (TRUE)
            {
                G::g_hWinMine = ::FindWindow(_T("ɨ��"), _T("ɨ��"));
                if (!G::g_hWinMine)
                {
                    INT nRet = ::MessageBox(hwndDlg, _T("ɨ�׳���û������\n�Ƿ����²���"), _T("��ʾ"), MB_YESNO);
                    if (IDYES == nRet)
                    {
                        continue;
                    }
                    else
                    {
                        ::EndDialog(hwndDlg, ERROR_CODE);
                        return TRUE;
                    }
                }
                else
                {
                    ::GetWindowThreadProcessId(G::g_hWinMine, &G::g_dwWinMineProcessId);
                    G::g_hWinMineProcessHandle = ::OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE, TRUE, G::g_dwWinMineProcessId);
                    if (!G::g_hWinMineProcessHandle)
                    {
                        ::MessageBox(hwndDlg, _T("ɨ�׳����ʧ��"), _T("��ʾ"), MB_OK);
                    }
                    break;
                }
            }

            G::g_lpMineArea = NEW BYTE[MINE_ADDRESS_MAX_LEN];
            if (!G::g_lpMineArea)
            {
                ::MessageBox(hwndDlg, _T("�ڴ治��"), _T("��ʾ"), MB_OK);
                ::EndDialog(hwndDlg, ERROR_CODE);
                return TRUE;
            }
            return TRUE;
        }break;

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                // ɨ��
            case IDC_DIGHT_MINE:
                {
                    // ��ȡ������Ϣ
                    BOOL bRet = MsUpdateMineAreaData(FALSE);
                    if (!bRet)
                    {
                        ::MessageBox(hwndDlg, _T("Ƕ��ɨ�׳���ʧ��"), _T("��ʾ"), MB_OK);
                        ::EndDialog(hwndDlg, ERROR_CODE);
                        return TRUE;
                    }

                    bRet = DightMine();
                    if (!bRet)
                    {
                        ::MessageBox(hwndDlg, _T("ɨ��ʧ��"), _T("��ʾ"), MB_OK);
                    }
                    MsUpdateMineAreaData(TRUE);
                }break;

                // ����
            case IDC_CLEAR_MINE:
                {
                    // ��ȡ������Ϣ
                    BOOL bRet = MsUpdateMineAreaData(FALSE);
                    if (!bRet)
                    {
                        ::MessageBox(hwndDlg, _T("Ƕ��ɨ�׳���ʧ��"), _T("��ʾ"), MB_OK);
                        ::EndDialog(hwndDlg, ERROR_CODE);
                        return TRUE;
                    }

                    bRet = ClearMine();
                    if (!bRet)
                    {
                        ::MessageBox(hwndDlg, _T("����ʧ��"), _T("��ʾ"), MB_OK);
                    }
                    MsUpdateMineAreaData(TRUE);
                }break;

                // ����
            case IDC_SEE_MINE:
                {
                    // ��ȡ������Ϣ
                    BOOL bRet = MsUpdateMineAreaData(FALSE);
                    if (!bRet)
                    {
                        ::MessageBox(hwndDlg, _T("Ƕ��ɨ�׳���ʧ��"), _T("��ʾ"), MB_OK);
                        ::EndDialog(hwndDlg, ERROR_CODE);
                        return TRUE;
                    }

                    bRet = SeeMine();
                    if (!bRet)
                    {
                        ::MessageBox(hwndDlg, _T("����ʧ��"), _T("��ʾ"), MB_OK);
                    }
                    MsUpdateMineAreaData(TRUE);
                }break;
            }
        }break;

    case WM_CLOSE:
        {
            if (G::g_lpMineArea)
            {
                delete[] G::g_lpMineArea;
                G::g_lpMineArea = NULL;
            }
            ::EndDialog(hwndDlg, 0);
        }break;

    default:
        {
            ;
        }break;
    }
    return (INT_PTR)FALSE;
}

// ��ȡ�����ڴ��е���Ϣ
BOOL MsReadProcessMemory(HANDLE hProcess, LPVOID lpAddr, LPVOID lpBuff, ULONG_PTR ulReadLen)
{
    // �ڴ��ַ��ǰ��Ȩ��
    DWORD dwOldMemProtect;

    BOOL bRet = ::VirtualProtectEx(
        hProcess,
        lpAddr,
        ulReadLen,
        PAGE_READONLY,
        &dwOldMemProtect
        );
    if (!bRet)
    {
        ::MessageBox(NULL, _T("�޸��ڴ�Ȩ��Aʧ��"), _T("��ʾ"), MB_OK);
        if (hProcess)
        {
            ::MessageBox(NULL, _T("�о��"), _T("��ʾ"), MB_OK);
        }
        if (lpAddr)
        {
            ::MessageBox(NULL, _T("�е�ַ"), _T("��ʾ"), MB_OK);
        }
        if (ulReadLen)
        {
            ::MessageBox(NULL, _T("�г���"), _T("��ʾ"), MB_OK);
        }
        return FALSE;
    }

    // ��ȡ�ڴ��е�����
    DWORD dwNumberOfBytesRead = 0;
    bRet = ::ReadProcessMemory(
        hProcess,
        lpAddr,
        lpBuff,
        ulReadLen,
        &dwNumberOfBytesRead
        );
    if (!bRet || (ulReadLen != dwNumberOfBytesRead))
    {
        ::MessageBox(NULL, _T("��ȡ�ڴ�����ʧ��"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �ָ��ڴ���ǰ�ķ���Ȩ��
    bRet = ::VirtualProtectEx(
        hProcess,
        lpAddr,
        ulReadLen,
        dwOldMemProtect,
        &dwOldMemProtect
        );
    if (!bRet)
    {
        ::MessageBox(NULL, _T("�޸��ڴ�Ȩ��Bʧ��"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

// д����Ϣ�������ڴ���
BOOL MsWriteProcessMemory(HANDLE hProcess, LPVOID lpAddr, LPVOID lpBuff, ULONG_PTR ulReadLen)
{
    // �ڴ��ַ��ǰ��Ȩ��
    DWORD dwOldMemProtect;

    BOOL bRet = ::VirtualProtectEx(
        hProcess,
        lpAddr,
        ulReadLen,
        PAGE_WRITECOPY,
        &dwOldMemProtect
        );
    if (!bRet)
    {
        ::MessageBox(NULL, _T("�޸��ڴ�Ȩ��AAʧ��"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // д���ڴ��е�����
    DWORD dwNumberOfBytesWritten = 0;
    bRet = ::WriteProcessMemory(
        hProcess,
        lpAddr,
        lpBuff,
        ulReadLen,
        &dwNumberOfBytesWritten
        );
    if (!bRet || (ulReadLen != dwNumberOfBytesWritten))
    {
        ::MessageBox(NULL, _T("д���ڴ�����ʧ��"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    // �ָ��ڴ���ǰ�ķ���Ȩ��
    bRet = ::VirtualProtectEx(
        hProcess,
        lpAddr,
        ulReadLen,
        dwOldMemProtect,
        &dwOldMemProtect
        );
    if (!bRet)
    {
        ::MessageBox(NULL, _T("�޸��ڴ�Ȩ��BBʧ��"), _T("��ʾ"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

// ��ȡ�����ڴ��е���Ϣ
BOOL MsReadWinMineData(VOID)
{
    // ��ȡɨ���������
    BOOL bRet = MsReadProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_BEGIN_FLAG_ADDRESS,
        &G::g_bMineIsBegin, 
        MINE_BEGIN_FLAG_ADDRESS_LEN
        );
    if (!bRet)
    {
        return FALSE; 
    }

    // ��ȡ���׸���
    bRet = MsReadProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_COUNT_ADDRESS,
        &G::g_dwMineCount, 
        MINE_COUNT_ADDRESS_LEN
        );
    if (!bRet || (0 == G::g_dwMineCount))
    {
        return FALSE; 
    }

    // ��ȡ�������
    bRet = MsReadProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_WIDTH_ADDRESS,
        &G::g_nMineWidth,
        MINE_WIDTH_ADDRESS_LEN
        );
    if (!bRet || (0 == G::g_nMineWidth))
    {
        return FALSE;
    }
    G::g_nMineWidth += 2;

    // ��ȡ�����߶�
    bRet = MsReadProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_HEIGHT_ADDRESS,
        &G::g_nMineHeight,
        MINE_HEIGHT_ADDRESS_LEN
        );
    if (!bRet || (0 == G::g_nMineHeight))
    {
        return FALSE;
    }
    G::g_nMineHeight += 2;

    // ��ȡ���������
    bRet = MsReadProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_CHECK_ADDRESS,
        &G::g_dwWinMineCheck,
        MINE_CHECK_ADDRESS_LEN
        );
    if (!bRet || (0 != G::g_dwWinMineCheck))
    {
        return FALSE;
    }

    // ��ȡɨ�����յ�
    bRet = MsReadProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_MAX_SPACE_ADDRESS,
        &G::g_dwMineMaxSpace,
        MINE_MAX_SPACE_ADDRESS_LEN
        );
    if (!bRet || (0 == G::g_dwMineMaxSpace))
    {
        return FALSE;
    }

    // ��ȡɨ�׵�ǰ�յ�
    bRet = MsReadProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_CURR_SPACE_ADDRESS,
        &G::g_dwMineCurrSpace,
        MINE_CURR_SPACE_ADDRESS_LEN
        );
    if (!bRet)
    {
        return FALSE;
    }

    if (((G::g_nMineWidth)*(G::g_nMineHeight)) > MINE_ADDRESS_MAX_LEN)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

// д����Ϣ��ɨ���ڴ���
BOOL MsWriteWinMineData(VOID)
{
    // д��ɨ���������
    BOOL bRet = MsWriteProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_BEGIN_FLAG_ADDRESS,
        &G::g_bMineIsBegin, 
        MINE_BEGIN_FLAG_ADDRESS_LEN
        );
    if (!bRet)
    {
        return FALSE; 
    }

    // д����׸���
    bRet = MsWriteProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_COUNT_ADDRESS2,
        &G::g_dwMineCount, 
        MINE_COUNT_ADDRESS2_LEN
        );
    if (!bRet)
    {
        return FALSE; 
    }

    // д���������
    bRet = MsWriteProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_WIDTH_ADDRESS,
        &G::g_nMineWidth,
        MINE_WIDTH_ADDRESS_LEN
        );
    if (!bRet)
    {
        return FALSE;
    }

    // д�������߶�
    bRet = MsWriteProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_HEIGHT_ADDRESS,
        &G::g_nMineHeight,
        MINE_HEIGHT_ADDRESS_LEN
        );
    if (!bRet)
    {
        return FALSE;
    }

    // д�����������
    bRet = MsWriteProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_CHECK_ADDRESS,
        &G::g_dwWinMineCheck,
        MINE_CHECK_ADDRESS_LEN
        );
    if (!bRet)
    {
        return FALSE;
    }

    // д��ɨ�׼�ʱ
    bRet = MsWriteProcessMemory(
        G::g_hWinMineProcessHandle,
        TIMER_COUNT,
        &G::g_dwMineTimerCount,
        TIMER_COUNT_LEN
        );
    if (!bRet)
    {
        return FALSE;
    }

    // ��ȡ���������
    bRet = MsWriteProcessMemory(
        G::g_hWinMineProcessHandle,
        MINE_CURR_SPACE_ADDRESS,
        &G::g_dwMineCurrSpace,
        MINE_CURR_SPACE_ADDRESS_LEN
        );
    if (!bRet)
    {
        return FALSE;
    }

    return TRUE;
}

// ����������Ϣ
BOOL MsUpdateMineAreaData(BOOL bToMineProcess)
{
    BOOL bRet = TRUE;

    // ���µ�ɨ��
    if (bToMineProcess)
    {
        if (((G::g_nMineWidth)*(G::g_nMineHeight)) > MINE_ADDRESS_MAX_LEN)
        {
            return FALSE;
        }

        if (G::g_lpMineArea)
        {
            // ��ȡɨ����Ϸ������
            bRet = MsWriteProcessMemory(
                G::g_hWinMineProcessHandle,
                MINE_ADDRESS,
                G::g_lpMineArea,
                MINE_ADDRESS_MAX_LEN
                );
            if (!bRet)
            {
                return FALSE;
            }
            if (MsWriteWinMineData())
            {
                ::InvalidateRect(G::g_hWinMine, NULL, TRUE);
                return TRUE;
            }
            return FALSE;
        }
        else
        {
            return FALSE;
        }
    }

    // ���µ�����
    else
    {
        if (((G::g_nMineWidth)*(G::g_nMineHeight)) > MINE_ADDRESS_MAX_LEN)
        {
            return FALSE;
        }

        if (G::g_lpMineArea)
        {
            bRet = MsReadWinMineData();
            if (!bRet)
            {
                return FALSE;
            }

            // ��ȡɨ����Ϸ������
            bRet = MsReadProcessMemory(
                G::g_hWinMineProcessHandle,
                MINE_ADDRESS,
                G::g_lpMineArea,
                MINE_ADDRESS_MAX_LEN
                );
            if (!bRet)
            {
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

// ��ȡ�׵ı��
E_MINE_FLAG GetMineFlag(INT X, INT Y)
{
    if (!G::g_lpMineArea || (X < 0) || (Y < 0))
    {
        return E_NUMBER_ERROR;
    }
    return (E_MINE_FLAG)G::g_lpMineArea[Y * MAX_MINE_WIDTH + X];
}

// ������Χ�׵ĸ���
DWORD FindAroundMineCount(INT X, INT Y)
{
    if (!G::g_lpMineArea || (X < 0) || (Y < 0))
    {
        return E_NUMBER_ERROR;
    }
    DWORD dwMineCount = 0;
    E_MINE_FLAG eMineFlag = E_NUMBER_ERROR;

    eMineFlag = GetMineFlag(X-1, Y-1);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    eMineFlag = GetMineFlag(X, Y-1);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    eMineFlag = GetMineFlag(X+1, Y-1);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    eMineFlag = GetMineFlag(X-1, Y);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    eMineFlag = GetMineFlag(X+1, Y);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    eMineFlag = GetMineFlag(X-1, Y+1);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    eMineFlag = GetMineFlag(X, Y+1);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    eMineFlag = GetMineFlag(X+1, Y+1);
    switch (eMineFlag)
    {
    case E_NUMBER_IS_MINE_INCERTITUDE:
    case E_NUMBER_IS_MINE_FLAG:
    case E_NUMBER_MINE:
        {
            dwMineCount++;
        }
    default:
        {
            ;
        }break;
    }

    return dwMineCount;
}

// �޸��׵ı��
VOID ChangeMineFlag(INT X, INT Y, E_MINE_FLAG eMineFlag)
{
    if (!G::g_lpMineArea || (X < 0) || (Y < 0))
    {
        return;
    }
    G::g_lpMineArea[Y * MAX_MINE_WIDTH + X] = (BYTE)eMineFlag;
}

// ɨ��
BOOL DightMine(VOID)
{
    if (!G::g_lpMineArea)
    {
        return FALSE;
    }

    for (INT Y = 0; Y < G::g_nMineHeight; Y++)
    {
        for (INT X = 0; X < G::g_nMineWidth; X++)
        {
            E_MINE_FLAG eMineFlag = GetMineFlag(X, Y);
            switch (eMineFlag)
            {
            case E_NUMBER_UNKNOW:
            case E_NUMBER_NO_MINE_INCERTITUDE:
            case E_NUMBER_NO_MINE_FLAG:
                {
                    DWORD dwMineCount = FindAroundMineCount(X, Y);
                    ChangeMineFlag(X, Y, (E_MINE_FLAG)(dwMineCount + (DWORD)E_NUMBER_EMPTY));
                }break;

            case E_NUMBER_MINE:
            case E_NUMBER_IS_MINE_INCERTITUDE:
            case E_NUMBER_IS_MINE_FLAG:
                {
                    ChangeMineFlag(X, Y, E_NUMBER_IS_MINE_FLAG);
                }break;

            default:
            case E_NUMBER_WALL:
            case E_NUMBER_ERROR:
            case E_NUMBER_EMPTY:
            case E_NUMBER_1:
            case E_NUMBER_2:
            case E_NUMBER_3:
            case E_NUMBER_4:
            case E_NUMBER_5:
            case E_NUMBER_6:
            case E_NUMBER_7:
            case E_NUMBER_8:
                {
                    ;
                }break;
            }
        }
    }

    G::g_dwMineCount        = 0;
    G::g_bMineIsBegin       = TRUE;
    G::g_dwMineTimerCount   = 0;
    G::g_dwMineCurrSpace    = G::g_dwMineMaxSpace;
    return TRUE;
}

// ����
BOOL ClearMine(VOID)
{
    if (!G::g_lpMineArea)
    {
        return FALSE;
    }

    for (INT Y = 0; Y < G::g_nMineHeight; Y++)
    {
        for (INT X = 0; X < G::g_nMineWidth; X++)
        {
            E_MINE_FLAG eMineFlag = GetMineFlag(X, Y);
            switch (eMineFlag)
            {
            case E_NUMBER_MINE:
            case E_NUMBER_IS_MINE_INCERTITUDE:
            case E_NUMBER_IS_MINE_FLAG:
                {
                    ChangeMineFlag(X, Y, E_NUMBER_UNKNOW);
                }break;

            default:
            case E_NUMBER_UNKNOW:
            case E_NUMBER_NO_MINE_INCERTITUDE:
            case E_NUMBER_NO_MINE_FLAG:
            case E_NUMBER_WALL:
            case E_NUMBER_ERROR:
            case E_NUMBER_EMPTY:
            case E_NUMBER_1:
            case E_NUMBER_2:
            case E_NUMBER_3:
            case E_NUMBER_4:
            case E_NUMBER_5:
            case E_NUMBER_6:
            case E_NUMBER_7:
            case E_NUMBER_8:
                {
                    ;
                }break;
            }
        }
    }

    G::g_dwMineCount        = 0;
    G::g_dwMineCurrSpace    = G::g_dwMineMaxSpace;
    return TRUE;
}

// ����
BOOL SeeMine(VOID)
{
    if (!G::g_lpMineArea)
    {
        return FALSE;
    }

    for (INT Y = 0; Y < G::g_nMineHeight; Y++)
    {
        for (INT X = 0; X < G::g_nMineWidth; X++)
        {
            E_MINE_FLAG eMineFlag = GetMineFlag(X, Y);
            switch (eMineFlag)
            {
            case E_NUMBER_MINE:
            case E_NUMBER_IS_MINE_INCERTITUDE:
            case E_NUMBER_IS_MINE_FLAG:
                {
                    ChangeMineFlag(X, Y, E_NUMBER_IS_MINE_FLAG);
                }break;

            default:
            case E_NUMBER_UNKNOW:
            case E_NUMBER_NO_MINE_INCERTITUDE:
            case E_NUMBER_NO_MINE_FLAG:
            case E_NUMBER_WALL:
            case E_NUMBER_ERROR:
            case E_NUMBER_EMPTY:
            case E_NUMBER_1:
            case E_NUMBER_2:
            case E_NUMBER_3:
            case E_NUMBER_4:
            case E_NUMBER_5:
            case E_NUMBER_6:
            case E_NUMBER_7:
            case E_NUMBER_8:
                {
                    ;
                }break;
            }
        }
    }

    //G::g_dwMineCount        = 0;
    //G::g_bMineIsBegin       = TRUE;
    //G::g_dwMineTimerCount   = 0;
    return TRUE;
}