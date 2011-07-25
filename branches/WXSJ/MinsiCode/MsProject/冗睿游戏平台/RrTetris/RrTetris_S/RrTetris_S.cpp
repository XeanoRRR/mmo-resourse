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

    RrTetrisWritePacket(0,NULL);

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


    ::PostThreadMessage(gs_dwGameStationLogicThreadId, MSM_DLL_EXIT, 0, 0);
    SAFE_DELETE(RrTetris::gs_lpCMsPacketFactory);

    // �ȴ������ͷ�
    while(g_IsMsBaseInit)
    {
        NO_CONTINUOUS;
    }
    return 0;
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
    return RrTetris::GetDllGameLogicThread();
}