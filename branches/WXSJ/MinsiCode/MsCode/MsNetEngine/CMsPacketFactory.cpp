#include "Precompiled.h"

// ����
CMsPacketFactory::CMsPacketFactory(PACKET_TYPE PacketMaxCount)
: m_PacketMaxCount  (PacketMaxCount)
, m_ppPacketPool    (NULL)
{
    if (m_PacketMaxCount)
    {
        m_ppPacketPool = NEW lpCMsBasePacket[m_PacketMaxCount];
        AssertEx(AL_CATAST, m_ppPacketPool, _T("�ڴ治��"));
        ZeroMemory(m_ppPacketPool, sizeof(lpCMsBasePacket) * m_PacketMaxCount);
    }
    else
    {
        AssertEx(AL_CATAST, m_PacketMaxCount, _T("ע��ķ����������Ϊ��"));
    }
}

// ����
CMsPacketFactory::~CMsPacketFactory(VOID)
{
    SAFE_DELETE_ARRAY(m_ppPacketPool);
    m_PacketMaxCount = 0;
}

// ע����
BOOL CMsPacketFactory::Register(lpCMsBasePacket* lppPacket, DWORD dwPacketCount)
{
    DWORD dwPacketType = INVALID_UID;
    if (lppPacket)
    {
        FOR_x_TO(i, dwPacketCount)
        {
            dwPacketType = lppPacket[i]->GetPacketType();
            if (m_ppPacketPool[dwPacketType] != NULL)
            {
                fAssertEx(
					AL_CATAST,
                    FALSE,
                    _T("���Id�ظ�ע�᡾%d,%d��"),
                    dwPacketType,
                    i
                    );
                return FALSE;
            }
            else
            {
                m_ppPacketPool[dwPacketType] = lppPacket[i];
            }
        }
    }
    return TRUE;
}

// ��ȡ���Ĭ�ϳߴ�
DWORD CMsPacketFactory::GetPacketDefaultSize(PACKET_TYPE PacketId)
{
    CMsBasePacket* pPacket = this->GetPacket(PacketId);
    if (pPacket)
    {
        return pPacket->GetPacketSize();
    }
    return 0;
}

// ��ȡ�������Ĭ�ϳߴ�
DWORD CMsPacketFactory::GetPacketDataDefaultSize(PACKET_TYPE PacketId)
{
    CMsBasePacket* pPacket = this->GetPacket(PacketId);
    if (pPacket)
    {
        return pPacket->GetPacketSize() - sizeof(PacketHead);
    }
    return 0;
}

// ��ȡ���
CMsBasePacket* CMsPacketFactory::GetPacket(PACKET_TYPE PacketId)
{
    if (PacketId >= m_PacketMaxCount)
    {
        return NULL;
    }
    else
    {
        return m_ppPacketPool[PacketId];
    }
}

// ������ͷ�õ����
CMsBasePacket* CMsPacketFactory::ParsePacketHead(PacketHead* lpPacketHead)
{
    if (lpPacketHead->CheckRegular())
    {
        return this->GetPacket(lpPacketHead->m_ePackType);
    }
    return NULL;
}