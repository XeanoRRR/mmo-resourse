#include "Precompiled.h"

BOOL S2C_Connect::ExecutePacket(CMsSocket* lpCMsSocket)
{
    USE_PARAM(lpCMsSocket);
#ifdef MS_RRGAME_CLIENT
    LPMS_PACKET_DATA lpData = (LPMS_PACKET_DATA)lpCMsSocket->GetPacketData();
        if (lpData->m_dwData == CONNECT_KEY)
    {
        // ���ӷ������ɹ�
        return TRUE;
    }
    return FALSE;
#else
    return FALSE;
#endif
}

