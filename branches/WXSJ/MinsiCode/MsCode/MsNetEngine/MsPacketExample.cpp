#include "Precompiled.h"
#include "MsPacketExample.h"

CMsPacketExample g_CMsPacketExample;
CMsPacketExample::CMsPacketExample(VOID)
: CMsBasePacket(0, sizeof(MS_PACKET_DATA) + sizeof(PacketHead))
{
    ;
}

// ��ȡ��Ч���ݵ�ַ
LPVOID CMsPacketExample::GetValidPacketData(DWORD* lpSize)
{
    *lpSize = sizeof(ValidData);
    return &ValidData;
}

// ��ȡ��Ч���ݳߴ�
DWORD CMsPacketExample::GetValidPacketDataSize(VOID)
{
    return sizeof(ValidData);
}

BOOL CMsPacketExample::ExecutePacket(CMsSocket* lpCMsSocket)
{
    USE_PARAM(lpCMsSocket);
    LPMS_PACKET_DATA lpData = (LPMS_PACKET_DATA)lpCMsSocket->GetPacketBuff();
    USE_PARAM(lpData);
#ifdef MS_TEST_SERVER
    //g_MsLog.Logf(_T("���յ��ͻ��˵����ݡ�%d��"), lpData->m_dwData);
    return TRUE;
#else
    //g_MsLog.Logf(_T("���յ������������ݡ�%d��"), lpData->m_dwData);
    return TRUE;
#endif
}

