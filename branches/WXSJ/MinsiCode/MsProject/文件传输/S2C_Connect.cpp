#include "Precompiled.h"
#include "S2C_Connect.h"

BOOL S2C_Connect::ExecutePacket(CMsSocket* lpCMsSocket)
{
    USE_PARAM(lpCMsSocket);
    LPMS_PACKET_DATA lpData = (LPMS_PACKET_DATA)lpCMsSocket->GetPacketData();
    if (lpData->m_bOK)
    {
        _tcout << _T("���\t") << _T("���ӷ������ɹ�\n");
        // ���ӷ������ɹ�
        return TRUE;
    }
    return FALSE;
}

