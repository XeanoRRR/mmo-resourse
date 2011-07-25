#include "Precompiled.h"
#include "FileClient.h"
#include "FileServer.h"

VOID ClientLogic(VOID)
{
    TCHAR szServerIP[IP_SIZE] = {0};
    TCHAR szPassword[MAX_PATH] = {0};
    WORD wProt = 0;
    _tcout << _T("�����������IP:\n");
    _tcin >> szServerIP;
    _tcout << _T("����������˿�:\n");
    _tcin >> wProt;
    _tcout << _T("��������������:\n");
    _tcin >> szPassword;
    g_pCFileClient = NEW CFileClient(szServerIP, wProt, szPassword);
    AssertEx(g_pCFileClient, _T("�ڴ治��"));
    g_pCFileClient->Run();
}


VOID ServerLogic(VOID)
{
    WORD wProt = 0;
    DWORD dwMaxClient = 0;
    TCHAR szPassword[MAX_PATH] = {0};
    _tcout << _T("����������˿�:\n");
    _tcin >> wProt;
    _tcout << _T("�������������������:\n");
    _tcin >> dwMaxClient;
    _tcout << _T("��������������:\n");
    _tcin >> szPassword;
    if (wProt && dwMaxClient)
    {
        g_pCFileServer = NEW CFileServer(wProt, dwMaxClient, szPassword);
        AssertEx(g_pCFileServer, _T("�ڴ治��"));
        g_pCFileServer->Run();
    }
    else
    {
        _tcout << _T("������Ч:\n");
    }
}

INT MSCALL Exe_FileTransfers_Main(INT,LPTCHAR*)
{
    BOOL bExit = FALSE;
    INT nIndex = 0;
    while (!bExit)
    {
        _tcout << _T("��ѡ��:\n")
            << _T("1������������->�����ļ�\n")
            << _T("2������������->����·��\n")
            << _T("3����½���������\n")
            << _T("4���˳�\n");
        nIndex = _getch();
        switch(nIndex)
        {
        case _T('1'):
            {
                ServerLogic();
            }break;
        case _T('2'):
            {
                _tcout << _T("�ݲ�֧��\n");
            }break;
        case _T('3'):
            {
                ClientLogic();
            }break;
        case _T('4'):
            {
                bExit = TRUE;
            }break;
        default:
            {
                _tcout << _T("ѡ����Ч\n");
            }break;
        }
        _tsystem(_T("Pause"));
        _tsystem(_T("Cls"));
    }

    SAFE_DELETE(g_pCFileServer);
    SAFE_DELETE(g_pCFileClient);
    return 0;
}
