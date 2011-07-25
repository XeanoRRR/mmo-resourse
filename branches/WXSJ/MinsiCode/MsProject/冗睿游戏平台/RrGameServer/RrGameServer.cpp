#include "Precompiled.h"

INT MSCALL RrGameServerMainThread(INT, LPTCHAR*)
{
    BOOL bRet = TRUE;
    g_pMsLog = NEW CMsLog(_T("%s/Log/Exe_RrGameServer.log"), TRUE);
    g_dwMainThreadId = ::GetCurrentThreadId();
    DWORD dwCurrentThreadId = g_dwMainThreadId;
    CMsPacketFactory* pCMsPacketFactory = NEW CMsPacketFactory(RR_GAME_PACKET_MAX_COUNT);
    CMsServerNetManager* pCMsServerNetManager = NULL;
    DWORD dwNetThreadId = 0;
    if (pCMsPacketFactory)
    {
        pCMsPacketFactory->Register(g_lpCMsBasePacket, PACKET_MAX_COUNT);

        pCMsServerNetManager = NEW CMsServerNetManager(_T("127.0.0.1"), 8888, dwCurrentThreadId, 1000, pCMsPacketFactory);
        if (pCMsServerNetManager)
        {
            dwNetThreadId = pCMsServerNetManager->GetMsThreadId();
            pCMsServerNetManager->Run();
        }
        else
        {
            AssertEx(AL_CATAST, pCMsServerNetManager, _T("�ڴ治��"));
        }
    }
    else
    {
        AssertEx(AL_CATAST, pCMsPacketFactory, _T("�ڴ治��"));
    }

    MSG Msg;
    while (::GetMessage(&Msg, NULL, 0, 0))
    {
        switch (Msg.message)
        {
            // �пͻ��˽���
        case MSM_SERVER_SOCKET_ON_ENTER:
            {
                CMsClientSocket* pClientSocket = (CMsClientSocket*)Msg.wParam;
                g_pMsLog->Logf(_T("�¿ͻ��˽��������[%08X]\n"), pClientSocket->GetSocket());
            }break;

            // �пͻ��˶Ͽ�
        case MSM_SERVER_SOCKET_ON_EXIT:
            {
                CMsClientSocket* pClientSocket = (CMsClientSocket*)Msg.wParam;
                g_pMsLog->Logf(_T("�ͻ����뿪������[%08X]\n"), pClientSocket->GetSocket());
            }break;

            // �а���Ҫִ��
        case MSM_MSG_SOCKET_EXECUTE:
            {
                CMsClientSocket* pClientSocket = (CMsClientSocket*)Msg.wParam;
                // �����ִ�д���
                if (!bRet)
                {
                    ULONG_PTR ulIocpKey = pClientSocket->GetPoolIndex();
                    ::PostThreadMessage(dwNetThreadId, MSM_IOCP_EXIT_OR_ERROR, ulIocpKey, 0);
                }
            }break;

        case MSM_RRGAME_SHUT_DWON_SERVER:
            {
                g_pMsLog->Time();
                g_pMsLog->Log(_T("��������ʼ�ر�...\n"));
                ::PostQuitMessage(0);
            }break;
        }
    }

    pCMsServerNetManager->StopNet();
    SAFE_DELETE(pCMsServerNetManager);
    SAFE_DELETE(pCMsPacketFactory);

    g_pMsLog->Time();
    g_pMsLog->Log(_T("�������ɹ��ر�\n"));

    SAFE_DELETE(g_pMsLog);

    //_tsystem(_T("Pause"));
    return 0;
}
