#include "Precompiled.h"
#include "DG2S_EnterGame.h"
BOOL DG2S_EnterGame::ExecutePacket(CMsSocket* lpCMsSocket)
{
    USE_PARAM(lpCMsSocket);
#ifdef MS_RRGAME_SERVER
    LPMS_PACKET_DATA lpData = (LPMS_PACKET_DATA)lpCMsSocket->GetPacketData();
    if (lpData->m_dwData == CONNECT_KEY)
    {
        g_pMsLog->Logf(_T("����˹����[%08X]��½��Ϸ\n"), lpCMsSocket->GetSocket());
        return TRUE;
    }
    g_pMsLog->Logf(_T("����˹����[%08X]��½��Ϸʧ��:Key����\n"), lpCMsSocket->GetSocket());
    return FALSE;
#else
    return FALSE;
#endif
}

