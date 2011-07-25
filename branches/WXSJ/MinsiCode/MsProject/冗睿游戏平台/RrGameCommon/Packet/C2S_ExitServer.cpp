#include "Precompiled.h"
BOOL C2S_ExitServer::ExecutePacket(CMsSocket* lpCMsSocket)
{
    USE_PARAM(lpCMsSocket);
#ifdef MS_RRGAME_SERVER
    LPMS_PACKET_DATA lpData = (LPMS_PACKET_DATA)lpCMsSocket->GetPacketData();
    if (0 == _tcscmp(lpData->m_szShutDownPassword, _T("abcdefghijklmn")))
    {
        g_pMsLog->Logf(_T("���յ��Ϸ��Ĺرշ���������,�ͻ���[%08X]\n"), lpCMsSocket->GetSocket());
        ::PostThreadMessage(lpCMsSocket->GetLogicThreadId(), MSM_RRGAME_SHUT_DWON_SERVER, 0, 0);
    }
    else
    {
        g_pMsLog->Logf(_T("�ͻ���[%08X]���Թرշ�����,���������[%s]\n"), lpCMsSocket->GetSocket(), lpData->m_szShutDownPassword);
    }
    return TRUE;
#else
    return FALSE;
#endif
}

