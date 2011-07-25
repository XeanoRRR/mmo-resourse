#include "Precompiled.h"

PacketHead::PacketHead(
    DWORD wIdentifier,
    PACKET_TYPE ePackType,
    E_KEY_TYPE eKeyType,
    DWORD dwSize
)
: m_dwIdentifier    (wIdentifier)
, m_ePackType       (ePackType)
, m_eKeyType        (eKeyType)
, m_dwPackSize      (dwSize)
, m_i64Time         (0)
{
    ;
}

BOOL PacketHead::CheckRegular(VOID)
{
    return (m_dwIdentifier == PACKET_IDENTIFIER);
}


CMsBasePacket::CMsBasePacket(
    PACKET_TYPE ePackType,
    DWORD dwSize,
    DWORD wIdentifier,
    E_KEY_TYPE eKeyType
    )
: m_PackHead(wIdentifier, ePackType, eKeyType, dwSize)
{
    ;
}

// �������
VOID CMsBasePacket::Lock(VOID)
{
    m_IsLock = TRUE;
}

// ��ȡ����ߴ�
DWORD CMsBasePacket::GetPacketSize(VOID)
{
    return m_PackHead.m_dwPackSize;
}

// ��ȡ�������
PACKET_TYPE CMsBasePacket::GetPacketType(VOID)
{
    return m_PackHead.m_ePackType;
}

// ��ȡ���ʱ��
INT64 CMsBasePacket::GetPacketTime(VOID)
{
    return m_PackHead.m_i64Time;
}

// ��ȡ�����Կ
E_KEY_TYPE CMsBasePacket::GetPacketKeyType(VOID)
{
    return m_PackHead.m_eKeyType;
}

// �жϷ���Ƿ�������
BOOL CMsBasePacket::IsLock(VOID)
{
    return m_IsLock;
}

// �����
BOOL CMsBasePacket::CheckRegular(VOID)
{
    return m_PackHead.CheckRegular();
}

// ��ȡ��ͷ��ַ
lpPacketHead CMsBasePacket::GetPacketHead(DWORD* lpSize)
{
    if (lpSize)
    {
        *lpSize = sizeof(m_PackHead);
    }
    return &m_PackHead;
}

// ���÷��
VOID CMsBasePacket::Reset(VOID)
{
    //ZeroMemoryStruct(MS_PACKET_DATA);
    m_IsLock = FALSE;
}

BOOL CMsBasePacket::ExecutePacket(CMsSocket* lpCMsSocket)
{
    USE_PARAM(lpCMsSocket);
    AssertEx(AL_SHOW_ONLY, FALSE, _T("�Ƿ�����"));
    return FALSE;
}
