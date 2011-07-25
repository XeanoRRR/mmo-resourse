#include "Precompiled.h"

MsRecvBuffer::MsRecvBuffer(DWORD dwPacketCount, DWORD dwMaxBuffSize)
: m_dwPacketCount           (dwPacketCount)
, m_dwMaxBuffSize           (dwPacketCount * dwMaxBuffSize)
, m_dwCompletePacketCount   (0)
, m_dwCurrentRecvPacketIndex(0)
, m_dwCurrentExePacketIndex (0)
, m_dwRecvedSize            (0)
, m_dwCurrentDataSize       (0)
, m_dwExeingPacketIndex     (0)
, m_eRecvState              (RECV_HEAD)
{
    AssertEx(AL_CATAST, m_dwPacketCount, _T("��ͷ��������Ϊ0"));
    AssertEx(AL_CATAST, dwMaxBuffSize, _T("�������ߴ粻��Ϊ0"));
    
    m_lpPacketHead = NEW RecvPacketBuff[m_dwPacketCount];
    if (m_lpPacketHead)
    {
        ZeroMemory(m_lpPacketHead, sizeof(RecvPacketBuff) * m_dwPacketCount);
    }
    else
    {
        AssertEx(AL_CATAST, m_lpPacketHead, _T("�ڴ治��"));
    }

    m_lpBuffHead = NEW BYTE[m_dwMaxBuffSize];
    if (m_lpBuffHead)
    {
        ZeroMemory(m_lpBuffHead, sizeof(BYTE) * m_dwMaxBuffSize);

        m_lpBuffTail = &m_lpBuffHead[m_dwMaxBuffSize - 1];
        m_pDataHead = m_pDataTail = m_lpBuffHead;
    }
    else
    {
        AssertEx(AL_CATAST, m_lpBuffHead, _T("�ڴ治��"));
    }
}

MsRecvBuffer::~MsRecvBuffer(VOID)
{
    SAFE_DELETE_ARRAY(m_lpBuffHead);
    SAFE_DELETE_ARRAY(m_lpPacketHead);
}

// ��ȡ����λ�á��ߴ�
LPBYTE MsRecvBuffer::GetRecvAddr(DWORD& dwSize)
{
    AUTO_LOCK(m_Lock);

    LPBYTE lpRecvAddr = NULL;
    DWORD dwFreeSize = this->GetFreeSize();
    if (dwFreeSize == 0)
    {
        dwSize = 0;
        return NULL;
    }

    // �����Ҫ���հ�ͷ
    if (m_eRecvState == RECV_HEAD)
    {
        // ���յ���ͷ������
        lpRecvAddr = (LPBYTE)&m_lpPacketHead[m_dwCurrentRecvPacketIndex].m_PacketHead;
        lpRecvAddr += m_dwRecvedSize;
        dwSize = sizeof(PacketHead) - m_dwRecvedSize;
        return lpRecvAddr;
    }

    // �����Ҫ��������
    else if (m_eRecvState == RECV_DATA)
    {
        DWORD dwNeedSize = m_lpPacketHead[m_dwCurrentRecvPacketIndex].m_PacketHead.m_dwPackSize - m_dwRecvedSize - sizeof(PacketHead);
        // ���յ�����ͷ��
        lpRecvAddr = m_pDataHead;
        if (m_pDataHead >= m_pDataTail)
        {
            dwSize = (DWORD)(m_lpBuffTail - m_pDataHead);
        }
        else
        {
            dwSize = (DWORD)(m_pDataTail - m_pDataHead);
        }

        if (dwSize > dwNeedSize)
        {
            dwSize = dwNeedSize;
        }
        return lpRecvAddr;
    }
    dwSize = 0;
    return NULL;
}

// ���û�����
VOID MsRecvBuffer::Reset(VOID)
{
    m_dwCompletePacketCount     = 0;
    m_dwCurrentRecvPacketIndex  = 0;
    m_dwCurrentExePacketIndex   = 0;
    m_dwRecvedSize              = 0;
    m_dwCurrentDataSize         = 0;
    m_dwExeingPacketIndex       = 0;
    m_eRecvState                = RECV_HEAD;

    ZeroMemory(m_lpPacketHead, sizeof(RecvPacketBuff) * m_dwPacketCount);

    ZeroMemory(m_lpBuffHead, sizeof(BYTE) * m_dwMaxBuffSize);

    m_lpBuffTail = &m_lpBuffHead[m_dwMaxBuffSize - 1];
    m_pDataHead = m_pDataTail = m_lpBuffHead;
}


// ��ȡ���гߴ�
DWORD MsRecvBuffer::GetFreeSize(VOID)
{
    AUTO_LOCK(m_Lock);

    if (m_dwCurrentDataSize < m_dwMaxBuffSize)
    {
        return m_dwMaxBuffSize - m_dwCurrentDataSize;
    }
    return 0;
}

// ѹ������
BOOL MsRecvBuffer::Push(DWORD dwSize)
{
    AUTO_LOCK(m_Lock);

    if (dwSize > this->GetFreeSize())
    {
        //AssertEx(AL_NORMAL, FALSE, _T("û���㹻�Ŀ���ռ�,�����߼�"));
        return FALSE;
    }

    // ����ǽ��հ�ͷ
    if (m_eRecvState == RECV_HEAD)
    {
        m_dwRecvedSize += dwSize;

        // ����������
        if (m_dwRecvedSize >= sizeof(PacketHead))
        {
            m_eRecvState = RECV_DATA;
            m_dwRecvedSize = 0;
        }

        // ��������ճߴ�Խ��
        else if (m_dwRecvedSize >= sizeof(PacketHead))
        {
            AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
        }
    }

    // ����ǽ�������
    else if (m_eRecvState == RECV_DATA)
    {
        m_dwRecvedSize += dwSize;

        // ����������
        if (m_dwRecvedSize >= m_lpPacketHead[m_dwCurrentRecvPacketIndex].m_PacketHead.m_dwPackSize - sizeof(PacketHead))
        {
            m_lpPacketHead[m_dwCurrentRecvPacketIndex].m_IsValid = TRUE;
            m_lpPacketHead[m_dwCurrentRecvPacketIndex].m_lpPacketData = m_pDataHead;
            m_dwCurrentRecvPacketIndex++;
            if (m_dwCurrentRecvPacketIndex >= m_dwPacketCount)
            {
                m_dwCurrentRecvPacketIndex = 0;
            }
            if (m_lpPacketHead[m_dwCurrentRecvPacketIndex].m_IsValid)
            {
                // ���ջ�������
                DWORD dwNetThreadId = m_lpSocket->GetNetThreadId();
                return ::PostThreadMessage(dwNetThreadId, MSM_IOCP_POST_QUEUED_COMPLETION_STATUS, (WPARAM)m_lpSocket, INVALID_UID);
            }
            m_eRecvState = RECV_HEAD;
            m_dwCompletePacketCount++;
            m_dwRecvedSize = 0;

            DWORD dwLogicThreadId = m_lpSocket->GetLogicThreadId();
            m_lpSocket->ExecutePacket();
            return ::PostThreadMessage(dwLogicThreadId, MSM_MSG_SOCKET_EXECUTE, (WPARAM)m_lpSocket, 0);
        }
    }

    // ��������ͷλ��
    m_pDataHead += dwSize;

    // �������ݳߴ�
    m_dwCurrentDataSize += dwSize;
    AssertEx(AL_CATAST, (m_pDataHead <= m_lpBuffTail), _T("����ͷ�����ڻ���β����,�����߼�"));

    // �������ͷ�Ѿ������
    if (m_pDataHead == m_lpBuffTail)
    {
        // ������ͷָ�򻺳�ͷ
        m_pDataHead = m_lpBuffHead;
    }
    return TRUE;
}

// ��������
VOID MsRecvBuffer::Pop(DWORD dwSize)
{
    AUTO_LOCK(m_Lock);

    // �ж��Ƿ����㹻�����ݿռ�
    if (dwSize > m_dwCurrentDataSize)
    {
        return;
    }

    // �������β������ͷ���
    if (m_pDataTail < m_pDataHead)
    {
        _DEBUG_CODE(ZeroMemory(m_pDataTail, dwSize));
        m_dwCurrentDataSize -= dwSize;
        m_pDataTail += dwSize;
        if (m_pDataTail == m_lpBuffTail)
        {
            m_pDataTail = m_lpBuffHead;
        }
    }

    // �������β������ͷ�Ҳ���ص�
    else
    {
        // ��ȡ��������ռ�ߴ�
        DWORD dwSeriateSize = (DWORD)(m_lpBuffTail - m_pDataTail);

        // ����������ݿռ乻��
        if (dwSize <= dwSeriateSize)
        {
            _DEBUG_CODE(ZeroMemory(m_pDataTail, dwSize));
            m_dwCurrentDataSize -= dwSize;
            m_pDataTail += dwSize;
            if (m_pDataTail == m_lpBuffTail)
            {
                m_pDataTail = m_lpBuffHead;
            }
        }

        // ����������ݿռ䲻����
        else
        {
            _DEBUG_CODE(ZeroMemory(m_pDataTail, dwSeriateSize));
            m_dwCurrentDataSize -= dwSeriateSize;
            dwSize -= dwSeriateSize;
            _DEBUG_CODE(ZeroMemory(m_lpBuffHead, dwSize));
            m_dwCurrentDataSize -= dwSize;
            m_pDataTail = m_lpBuffHead + dwSize;
            if (m_pDataTail == m_lpBuffTail)
            {
                m_pDataTail = m_lpBuffHead;
            }
        }
    }
}

// ���ִ��
VOID MsRecvBuffer::CompleteExe(VOID)
{
    AUTO_LOCK(m_Lock);

    DWORD dwSize = m_lpPacketHead[m_dwCurrentExePacketIndex].m_PacketHead.m_dwPackSize;
    this->Pop(dwSize);
    m_lpPacketHead[m_dwExeingPacketIndex].m_IsValid = FALSE;
}

// ��ȡִ�з��
VOID MsRecvBuffer::GetExePacket(OUT lpPacketHead& lpPacketHead, OUT LPVOID& lpData)
{
    AUTO_LOCK(m_Lock);

    if (m_dwCompletePacketCount == 0)
    {
        AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
    }
    if (!m_lpPacketHead[m_dwCurrentExePacketIndex].m_IsValid)
    {
        AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
    }

    lpPacketHead = &m_lpPacketHead[m_dwCurrentExePacketIndex].m_PacketHead;
    lpData = m_lpPacketHead[m_dwCurrentExePacketIndex].m_lpPacketData;
    m_dwExeingPacketIndex = m_dwCurrentExePacketIndex;
    m_dwCurrentExePacketIndex++;
    m_dwCompletePacketCount--;
    if (m_dwCurrentExePacketIndex >= m_dwPacketCount)
    {
        m_dwCurrentExePacketIndex = 0;
    }
}

// ��ȡ��ɰ�����
DWORD MsRecvBuffer::GetCompletePacketCount(VOID)
{
    AUTO_LOCK(m_Lock);

    return m_dwCompletePacketCount;
}

// �����׽���ָ��
VOID MsRecvBuffer::SetSocket(CMsSocket* lpSocket)
{
    AssertEx(AL_CATAST, lpSocket, _T("�׽���ָ�벻��Ϊ��"));
    m_lpSocket = lpSocket;
}





MsSendBuffer::MsSendBuffer(DWORD dwMaxBuffSize)
: m_dwMaxBuffSize       (dwMaxBuffSize)
, m_dwCurrentDataSize   (0)
, m_bSending            (FALSE)
{
    AssertEx(AL_CATAST, dwMaxBuffSize, _T("�������ߴ粻��Ϊ0"));

    m_lpBuffHead = NEW BYTE[m_dwMaxBuffSize];
    if (m_lpBuffHead)
    {
        m_lpBuffTail = &m_lpBuffHead[m_dwMaxBuffSize - 1];
        m_pDataHead = m_pDataTail = m_lpBuffHead;
    }
    else
    {
        AssertEx(AL_CATAST, m_lpBuffHead, _T("�ڴ治��"));
    }
}

MsSendBuffer::~MsSendBuffer(VOID)
{
    SAFE_DELETE_ARRAY(m_lpBuffHead);
}

// ��ȡ����λ�á��ߴ�
LPBYTE MsSendBuffer::GetSendAddr(DWORD& dwSize)
{
    AUTO_LOCK(m_Lock);

    if (m_dwCurrentDataSize == 0)
    {
        dwSize = 0;
        return NULL;
    }

    if (m_pDataHead > m_pDataTail)
    {
        dwSize = (DWORD)(m_pDataHead - m_pDataTail);
    }
    else
    {
        dwSize = (DWORD)(m_lpBuffTail - m_pDataTail);
    }
    return m_pDataTail;
}

// ��ȡ���ݳߴ�
DWORD MsSendBuffer::GetDataSize(VOID)
{
    AUTO_LOCK(m_Lock);

    return m_dwCurrentDataSize;
}

// ���û�����
VOID MsSendBuffer::Reset(VOID)
{
    m_dwCurrentDataSize = 0;
    m_bSending = FALSE;
    ZeroMemory(m_lpBuffHead, sizeof(BYTE) * m_dwMaxBuffSize);
    m_pDataHead = m_pDataTail = m_lpBuffHead;
}

// ��ȡ���гߴ�
DWORD MsSendBuffer::GetFreeSize(VOID)
{
    AUTO_LOCK(m_Lock);

    if (m_dwCurrentDataSize < m_dwMaxBuffSize)
    {
        return m_dwMaxBuffSize - m_dwCurrentDataSize;
    }
    return 0;
}

// ѹ������
BOOL MsSendBuffer::Push(LPBYTE lpData, DWORD dwSize)
{
    AUTO_LOCK(m_Lock);

    // �ж��Ƿ����㹻�Ŀ���ռ�
    if (dwSize > this->GetFreeSize())
    {
        return FALSE;
    }

    // �������ͷ������β���
    if (m_pDataHead < m_pDataTail)
    {
        memcpy(m_pDataHead, lpData, dwSize);
        m_dwCurrentDataSize += dwSize;
        m_pDataHead += dwSize;
        if (m_pDataHead == m_lpBuffTail)
        {
            m_pDataHead = m_lpBuffHead;
        }
        return TRUE;
    }

    // �������ͷ������β�Ҳ���ص�
    else
    {
        // ��ȡ��������ռ�ߴ�
        DWORD dwSeriateSize = (DWORD)(m_lpBuffTail - m_pDataHead);

        // �����������ռ乻��
        if (dwSize <= dwSeriateSize)
        {
            memcpy(m_pDataHead, lpData, dwSize);
            m_dwCurrentDataSize += dwSize;
            m_pDataHead += dwSize;
            if (m_pDataHead == m_lpBuffTail)
            {
                m_pDataHead = m_lpBuffHead;
            }
            return TRUE;
        }

        // �����������ռ䲻����
        else
        {
            LPBYTE lpByte = lpData;
            memcpy(m_pDataHead, lpByte, dwSeriateSize);
            m_dwCurrentDataSize += dwSeriateSize;
            lpByte += dwSeriateSize;
            dwSize -= dwSeriateSize;
            memcpy(m_lpBuffHead, lpByte, dwSize);
            m_dwCurrentDataSize += dwSize;
            m_pDataHead = m_lpBuffHead + dwSize;
            if (m_pDataHead == m_lpBuffTail)
            {
                m_pDataHead = m_lpBuffHead;
            }
            return TRUE;
        }
    }
}

// ��������
VOID MsSendBuffer::Pop(DWORD dwSize)
{
    AUTO_LOCK(m_Lock);

    // �ж��Ƿ����㹻�����ݿռ�
    if (dwSize > m_dwCurrentDataSize)
    {
        return;
    }

    // �������β������ͷ���
    if (m_pDataTail < m_pDataHead)
    {
        _DEBUG_CODE(ZeroMemory(m_pDataTail, dwSize));
        m_dwCurrentDataSize -= dwSize;
        m_pDataTail += dwSize;
        if (m_pDataTail == m_lpBuffTail)
        {
            m_pDataTail = m_lpBuffHead;
        }
    }

    // �������β������ͷ�Ҳ���ص�
    else
    {
        // ��ȡ��������ռ�ߴ�
        DWORD dwSeriateSize = (DWORD)(m_lpBuffTail - m_pDataTail);

        // ����������ݿռ乻��
        if (dwSize <= dwSeriateSize)
        {
            _DEBUG_CODE(ZeroMemory(m_pDataTail, dwSize));
            m_dwCurrentDataSize -= dwSize;
            m_pDataTail += dwSize;
            if (m_pDataTail == m_lpBuffTail)
            {
                m_pDataTail = m_lpBuffHead;
            }
        }

        // ����������ݿռ䲻����
        else
        {
            _DEBUG_CODE(ZeroMemory(m_pDataTail, dwSeriateSize));
            m_dwCurrentDataSize -= dwSeriateSize;
            dwSize -= dwSeriateSize;
            _DEBUG_CODE(ZeroMemory(m_pDataTail, dwSize));
            m_dwCurrentDataSize -= dwSize;
            m_pDataTail = m_lpBuffHead + dwSize;
            if (m_pDataTail == m_lpBuffTail)
            {
                m_pDataTail = m_lpBuffHead;
            }
        }
    }
}

// �Ƿ����ڷ���
BOOL MsSendBuffer::IsSending(VOID)
{
    AUTO_LOCK(m_Lock);

    return m_bSending;
}

// �����Ƿ����ڷ���
VOID MsSendBuffer::SetSending(BOOL bSending)
{
    AUTO_LOCK(m_Lock);

    m_bSending = bSending;
}