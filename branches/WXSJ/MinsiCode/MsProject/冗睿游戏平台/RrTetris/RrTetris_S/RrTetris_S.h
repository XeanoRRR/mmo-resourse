#pragma once

namespace RrTetris
{
    // DLlGame�����ͷ�֪ͨ�ص�
    extern BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved);

    // ȫ����־ָ��
    static CMsLog*      gs_pMsLog = NULL;

    // DllGame�����̺߳���
    static DWORD WINAPI RrTetrisThread(LPVOID lpThreadParameter);

    // DllGame���ͷ���ӿ�
    static BOOL(*PrivateRrTetrisWritePacket)(CMsPacketFactory*,PACKET_TYPE,LPVOID,DWORD) = NULL;

    // DllGame���ͷ��
    static BOOL RrTetrisWritePacket(
        PACKET_TYPE PacketId,
        LPVOID lpData,
        DWORD dwSize = INVALID_UID
        );

    static CMsPacketFactory* gs_lpCMsPacketFactory; // DllGame�������
    static CMsIni*  gs_pRrTetrisConfig;             // DllGame���ù�����ָ��
    static DWORD    gs_dwGameStationLogicThreadId;  // ��Ϸƽ̨�߼��߳�Id
    static HANDLE   gs_hThread;                     // DllGame�߳̾��

    // ������Ϸƽ̨�����ӿ�
    VOID USE_DLL SetGameStationWritePacketFun(BOOL(*FunWritePacket)(CMsPacketFactory*,PACKET_TYPE,LPVOID,DWORD)); 
    VOID USE_DLL SetGameStationLogicThreadId(DWORD dwThreadId); // ������Ϸƽ̨�߼��߳�Id��DllGame
    HANDLE USE_DLL GetDllGameLogicThread(VOID);                 // ��ȡDllGame�߳̾��
    HANDLE USE_DLL StopDllGame(VOID);                           // ֹͣDllGame
};
