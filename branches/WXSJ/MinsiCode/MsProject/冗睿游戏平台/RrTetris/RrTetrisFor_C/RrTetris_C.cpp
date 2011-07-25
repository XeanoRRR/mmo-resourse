#include "Precompiled.h"

// DLlGame�����ͷ�֪ͨ�ص�
BOOL WINAPI DllMain(
    HANDLE  hDllHandle,
    DWORD   dwReason,
    LPVOID  lpreserved
    )
{
    USE_PARAM(hDllHandle);
    USE_PARAM(lpreserved);
    switch (dwReason)
    {
        // ����ǽ��̼���
    case DLL_PROCESS_ATTACH:
        {
            // ����DllGame�����߳�
            RrTetris::gs_hThread = ::CreateThread(NULL, 0, RrTetris::RrTetrisThread, NULL, 0, NULL);
        }break;

        // ����ǽ���ж��
    case DLL_PROCESS_DETACH:
        {
            // �ͷ����ö���
            SAFE_DELETE(RrTetris::gs_pRrTetrisConfig);

            // �ͷ���־����
            SAFE_DELETE(RrTetris::gs_pMsLog);

            // �ͷ�DllGame��ʼ������
            SAFE_DELETE(RrTetris::gs_pCMsBaseRun);

            RrTetris::gs_dwGameStationLogicThreadId = INVALID_UID;
            RrTetris::gs_hThread = NULL;
        }break;

        // ������̲߳���,����
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        {
            ;
        }break;
    }

    // ����
    return TRUE;
}

// DllGame�����̺߳���
DWORD WINAPI RrTetris::RrTetrisThread(LPVOID)
{
    RrTetris::gs_lpCMsPacketFactory = NEW CMsPacketFactory(DG2S_PACKET_MAX_COUNT);
    AssertEx(AL_CATAST, gs_lpCMsPacketFactory, _T("�ڴ治��"));
    RrTetris::gs_lpCMsPacketFactory->Register(g_lpCMsBasePacket, PACKET_MAX_COUNT);

    RrTetris::gs_pCMsBaseRun = NEW CMsBaseRun(_T("%s/Log/Dll_RrTetris_Assertx.log"), _T("RrTetrisClass"), _T("RrTetris"), RrTetrisHookProc, RrTetrisWndProc, NULL);
    AssertEx(AL_CATAST, gs_pCMsBaseRun, _T("�ڴ治��"));
    S_2DEngineWndInfo s_ewi;
    s_ewi.m_bTopMost= FALSE;
    s_ewi.m_nX      = CW_USEDEFAULT;
    s_ewi.m_nY      = 0;
    s_ewi.m_nWidth  = CW_USEDEFAULT;
    s_ewi.m_nHeight = 0;
    s_ewi.m_dwStyle = WS_OVERLAPPEDWINDOW;

    // �ȴ�������ʼ��
    while(!g_IsMsBaseInit)
    {
        NO_CONTINUOUS;
    }

    // ������־����
    RrTetris::gs_pMsLog = NEW CMsLog(_T("%s/Log/Dll_RrTetris_C.log"), TRUE);
    AssertEx(AL_CATAST, gs_pMsLog, _T("�ڴ治��"));

    // �������ö���
    RrTetris::gs_pRrTetrisConfig = NEW CMsIni(_T("%s/RrTetris/RrTetris.ini"));
    AssertEx(AL_CATAST, gs_pRrTetrisConfig, _T("�ڴ治��"));

    RrTetris::gs_hModule = ::GetModuleHandle(_T("Dll_RrTetris_C.dll"));
    RrTetris::gs_hInstance = (HINSTANCE)RrTetris::gs_hModule;
    if (RrTetris::gs_hInstance)
    {
        g_pCMsWinMain->Init(RrTetris::gs_hInstance, NULL, _T(""), SW_SHOW, &s_ewi);
        g_pCMsWinMain->SetLockSize(BACKSCENE_MAX_WIDTH, BACKSCENE_MAX_HEIGHT);
        g_pCMsWinMain->Run();
        g_pCMsWinMain->WaitQuit();
    }
    ::PostThreadMessage(gs_dwGameStationLogicThreadId, MSM_DLL_EXIT, (WPARAM)RrTetris::gs_hModule, 0);
    SAFE_DELETE(RrTetris::gs_pCMsBaseRun);
    SAFE_DELETE(RrTetris::gs_lpCMsPacketFactory);

    // �ȴ������ͷ�
    while(g_IsMsBaseInit)
    {
        NO_CONTINUOUS;
    }
    return 0;
}

// DllGame��Ϣ���Ӻ���
BOOL MSCALL RrTetris::RrTetrisHookProc(MSG* pMsg)
{
    USE_PARAM(pMsg);
    return FALSE;
}

// DllGame������Ϣ�ص�
LRESULT CALLBACK RrTetris::RrTetrisWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    USE_PARAM(hWnd);
    USE_PARAM(wParam);
    USE_PARAM(lParam);
    RrTetrisWritePacket(NULL, 0);

    switch (uMsg)
    {
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                // ��ʼ
            case ID_START_GAME:
                {
                    g_pGame->StartGame();
                }break;

                // ֹͣ��Ϸ
            case ID_STOP_GAME:
                {
                    g_pGame->EndGame();
                }break;

                // ����
            case ID_EXIT_GAME:
                {
                    g_pGame->ReleaseGame();
                }break;

                // ���¿�ʼ
            case ID_RESTART_GAME:
                {
                    g_pGame->EndGame();
                    g_pGame->StartGame();
                }break;

                // ��ͣ����
            case ID_PAUSE_CONTINUE_GAME:
                {
                    if (g_pGame->GetGameState() == GAME_STATE_PLAYING)
                    {
                        g_pGame->PauseGame();
                    }
                    else if (g_pGame->GetGameState() == GAME_STATE_PAUSE)
                    {
                        g_pGame->ContinueGame();
                    }
                }break;

                // ������Ϸ
            case ID_RESET_GAME:
                {
                    g_pGame->ReleaseGame();
                    g_pGame->InitGame();
                }break;
            }
        }break;

    case WM_TIMER:
        {
            g_pGame->GameHeartbeat();
        }break;

    case WM_CLOSE:
        {
            g_pGame->ReleaseGame();
            SAFE_DELETE(g_pGame);
            ::PostQuitMessage(0);
        }break;

    case WM_CREATE:
        {
            ::SetWindowPos(hWnd, NULL, 0, 0, BACKSCENE_MAX_WIDTH, BACKSCENE_MAX_HEIGHT, SWP_NOMOVE|SWP_NOZORDER);
            while (INVALID_UID == gs_dwGameStationLogicThreadId)
            {
                NO_CONTINUOUS;
            }
            g_pCMsWinMain->SetAcceleratorID(IDR_ACCELERATOR);
            g_pGame = NEW CT_Game(hWnd);
            AssertEx(AL_CATAST, g_pGame, _T("�ڴ治��"));
            g_pGame->InitGame();
            return ::PostThreadMessage(gs_dwGameStationLogicThreadId, MSM_DLL_NOTIFY_HWND, (WPARAM)hWnd, 0);
        }break;

    case WM_LBUTTONUP:
        {
            ::SendMessage(hWnd, WM_PRINT, 0, 0);
        }break;

    case WM_KEYDOWN:
        {
            switch(wParam)
            {
            case VK_SUBTRACT:
                {
                    g_pGame->SubMusicVolume();
                }break;
            case VK_ADD:
                {
                    g_pGame->AddMusicVolume();
                }break;
            }
            g_pGame->MoveShape((INT)wParam);
        }break;

    default:
        {
            return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
        }break;
    }
    return TRUE;
}

// DllGame���ͷ��
BOOL RrTetris::RrTetrisWritePacket(
    PACKET_TYPE PacketId,
    LPVOID lpData,
    DWORD dwSize
    )
{
    if (PrivateRrTetrisWritePacket && gs_lpCMsPacketFactory)
    {
        return PrivateRrTetrisWritePacket(gs_lpCMsPacketFactory, PacketId, lpData, dwSize);
    }
    return FALSE;
}

// ������Ϸƽ̨�����ӿ�
VOID USE_DLL RrTetris::SetGameStationWritePacketFun(BOOL(*FunWritePacket)(CMsPacketFactory*,PACKET_TYPE,LPVOID,DWORD))
{
    PrivateRrTetrisWritePacket = FunWritePacket;
}

// ������Ϸƽ̨�߼��߳�Id��DllGame
VOID USE_DLL RrTetris::SetGameStationLogicThreadId(DWORD dwThreadId)
{
    RrTetris::gs_dwGameStationLogicThreadId = dwThreadId;
}

// ��ȡDllGame�߳̾��
HANDLE USE_DLL RrTetris::GetDllGameLogicThread(VOID)
{
    return RrTetris::gs_hThread;
}

// ֹͣDllGame
HANDLE USE_DLL RrTetris::StopDllGame(VOID)
{
    g_pCMsWinMain->SendMessage(WM_CLOSE);
    return RrTetris::GetDllGameLogicThread();
}