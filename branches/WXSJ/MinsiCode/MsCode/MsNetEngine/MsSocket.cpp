#include "Precompiled.h"

// ����
CMsSocket::CMsSocket(
    DWORD dwBuffSize,
    DWORD dwLogicThreadId,
    DWORD dwNetThreadId,
    CMsPacketFactory* lpFactory
    )
: m_MsSendBuff          (dwBuffSize)
, m_MsRecvBuff          (TEMP_PACKET_RECV_COUNT, dwBuffSize)
, m_dwLogicThreadId     (dwLogicThreadId)
, m_dwNetThreadId       (dwNetThreadId)
, m_Socket              (INVALID_SOCKET)
, m_lpPacketData        (NULL)
, m_lpPacket            (NULL)
, m_u64RecvSize         (0)
, m_u64SendSize         (0)
, m_lpFactory           (lpFactory)
{
    ZeroMemoryStruct(m_RecvOverlapped);
    ZeroMemoryStruct(m_SendOverlapped);
    ZeroMemoryStruct(m_Sockaddr);
    m_MsRecvBuff.SetSocket(this);
    this->UpdataTime();
}

// ����
CMsSocket::~CMsSocket(VOID)
{
    if (INVALID_SOCKET != m_Socket)
    {
        ::closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
}

// ��ȡ�׽���
SOCKET CMsSocket::GetSocket(VOID)
{
    return m_Socket;
}

// ��ȡ�׽��ֶ˿�
DWORD CMsSocket::GetProt(VOID)
{
    INT nSize = sizeof(m_Sockaddr);
    ::getsockname(m_Socket, (SOCKADDR*)&m_Sockaddr, &nSize);
    return ::ntohs(m_Sockaddr.sin_port);
}

// ��ȡ�׽��ֵ�ַ
ACHAR* CMsSocket::GetAddr(VOID)
{
    int nSize = sizeof(m_Sockaddr);
    ::getsockname(m_Socket, (SOCKADDR*)&m_Sockaddr, &nSize);
    return ::inet_ntoa(m_Sockaddr.sin_addr);
}

// �ͷ�
BOOL CMsSocket::Release(VOID)
{
    ::closesocket(m_Socket);
    m_Socket = INVALID_SOCKET;
    return TRUE;
}

// ����
BOOL CMsSocket::Reset(VOID)
{
    m_MsRecvBuff.Reset();
    m_MsSendBuff.Reset();
    this->Release();
    //// �����������׽���
    //m_Socket = ::WSASocket(
    //    AF_INET,            // TCP��UDP
    //    SOCK_STREAM,        // TCP
    //    IPPROTO_TCP,        // TCP
    //    NULL,               // ��������ṹ��
    //    0,                  // ����
    //    WSA_FLAG_OVERLAPPED // �ص����
    //    );
    //if (INVALID_SOCKET == m_Socket)
    //{
    //    AssertEx((INVALID_SOCKET != m_Socket), _T("�׽��ִ���ʧ��"));
    //}
    return TRUE;
}

//// ����������
//BOOL CMsSocket::PacketToSend(CMsBasePacket* lpPacket)
//{
//    BOOL bRet = TRUE;
//    DWORD dwSize;
//    LPVOID lpData;
//
//    lpData = lpPacket->GetPacketHead(&dwSize);
//    bRet = m_MsSendBuff.PushData(lpData, dwSize);
//
//    lpData = lpPacket->GetValidPacketData(&dwSize);
//    bRet = m_MsSendBuff.PushData(lpData, dwSize);
//
//    if (bRet)
//    {
//        return ::PostThreadMessage(m_dwNetThreadId, MSM_MSG_SOCKET_SEND, (WPARAM)this, 0);
//    }
//    return bRet;
//}

//// �������
//BOOL CMsSocket::ParsePacket(CMsPacketFactory* lpFactory)
//{
//
//}

// ִ�а�
BOOL CMsSocket::ExecutePacket(VOID)
{
    BOOL bRet = TRUE;
    bRet = this->ReadPacket();
    if (bRet && m_lpPacket)
    {
        bRet = m_lpPacket->ExecutePacket(this);
        m_MsRecvBuff.CompleteExe();
    }
    else
    {
        AssertEx(AL_SHOW_ONLY, m_lpPacket, _T("ִ�а�����Ϊ��"));
        AssertEx(AL_SHOW_ONLY, bRet, _T("����������"));
    }
    return bRet;
}

// ����֪ͨ
BOOL CMsSocket::OnEnter(VOID)
{
    return this->RecvData();
}

// �˳�֪ͨ
BOOL CMsSocket::OnExit(VOID)
{
    this->Reset();
    return TRUE;
}

// ��ȡ���,׼��ִ��
BOOL CMsSocket::ReadPacket(VOID)
{
    lpPacketHead lpHead = NULL;
    m_MsRecvBuff.GetExePacket(lpHead, m_lpPacketData);
    AssertEx(AL_SHOW_ONLY, lpHead, _T("�����߼�"));
    AssertEx(AL_SHOW_ONLY, m_lpPacketData, _T("�����߼�"));

    if (m_lpFactory)
    {
        m_lpPacket = m_lpFactory->ParsePacketHead(lpHead);
        if (!m_lpPacket)
        {
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        AssertEx(AL_CATAST, m_lpFactory, _T("�����߼�"));
    }
    return FALSE;
}

// д����,׼������
BOOL CMsSocket::WritePacket(
    PACKET_TYPE PacketId,
    LPVOID lpData,
    DWORD dwSize
    )
{
    BOOL bRet = TRUE;
    // ��ȡ���
    CMsBasePacket* pPacket = m_lpFactory->GetPacket(PacketId);
    lpPacketHead lpHead = pPacket->GetPacketHead();
    DWORD dwFreeSize = m_MsSendBuff.GetFreeSize();
    if (pPacket)
    {
        //���ָ���������
        if (INVALID_UID != dwSize)
        {
            // ������ߴ��Ƿ��㹻
            if (dwFreeSize < sizeof(PacketHead) + dwSize)
            {
                return FALSE;
            }

            // ��¼ԭʼ�������
            DWORD dwPrimalPacketSize = m_lpFactory->GetPacketDefaultSize(PacketId);
            AssertEx(AL_CATAST, dwSize <= dwPrimalPacketSize, _T("�Զ���ߴ粻�ܳ���ԭʼ����ߴ�"));

            // ���·������
            lpHead->m_dwPackSize = sizeof(PacketHead) + dwSize;

            // �����Զ���ߴ�����
            bRet = m_MsSendBuff.Push((LPBYTE)lpHead, sizeof(PacketHead));
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }
            bRet = m_MsSendBuff.Push((LPBYTE)lpData, dwSize);
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }

            // ��ԭ����ߴ�
            lpHead->m_dwPackSize = dwPrimalPacketSize;
        }
        else
        {
            // ������ߴ��Ƿ��㹻
            if (dwFreeSize < lpHead->m_dwPackSize)
            {
                return FALSE;
            }

            bRet = m_MsSendBuff.Push((LPBYTE)lpHead, sizeof(PacketHead));
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }

            bRet = m_MsSendBuff.Push((LPBYTE)lpData, lpHead->m_dwPackSize - sizeof(PacketHead));
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }
        }

        if (!m_MsSendBuff.IsSending())
        {
            return this->SendData();
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// д����,׼������
BOOL CMsSocket::WritePacket(
    CMsPacketFactory* lpFactory,
    PACKET_TYPE PacketId,
    LPVOID lpData,
    DWORD dwSize
    )
{
    BOOL bRet = TRUE;
    if (!lpFactory)
    {
        return FALSE;
    }

    // ��ȡ���
    CMsBasePacket* pPacket = lpFactory->GetPacket(PacketId);
    lpPacketHead lpHead = pPacket->GetPacketHead();
    DWORD dwFreeSize = m_MsSendBuff.GetFreeSize();
    if (pPacket)
    {
        //���ָ���������
        if (INVALID_WID != dwSize)
        {
            // ������ߴ��Ƿ��㹻
            if (dwFreeSize < sizeof(PacketHead) + dwSize)
            {
                return FALSE;
            }

            // ��¼ԭʼ�������
            DWORD dwPrimalPacketSize = lpFactory->GetPacketDefaultSize(PacketId);
            AssertEx(AL_CATAST, dwSize <= dwPrimalPacketSize, _T("�Զ���ߴ粻�ܳ���ԭʼ����ߴ�"));

            // ���·������
            lpHead->m_dwPackSize = sizeof(PacketHead) + dwSize;

            // �����Զ���ߴ�����
            bRet = m_MsSendBuff.Push((LPBYTE)lpHead, sizeof(PacketHead));
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }
            bRet = m_MsSendBuff.Push((LPBYTE)lpData, dwSize);
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }

            // ��ԭ����ߴ�
            lpHead->m_dwPackSize = dwPrimalPacketSize;
        }
        else
        {
            // ������ߴ��Ƿ��㹻
            if (dwFreeSize < lpHead->m_dwPackSize)
            {
                return FALSE;
            }

            bRet = m_MsSendBuff.Push((LPBYTE)lpHead, sizeof(PacketHead));
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }

            bRet = m_MsSendBuff.Push((LPBYTE)lpData, lpHead->m_dwPackSize - sizeof(PacketHead));
            if (!bRet)
            {
                AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
            }
        }

        if (!m_MsSendBuff.IsSending())
        {
            return this->SendData();
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



// ��ȡ�������
LPVOID CMsSocket::GetPacketData(VOID)
{
    return m_lpPacketData;
}

// ���½���
BOOL CMsSocket::OnUpdate(S_IOCP_UPDATE* lpIocpUpdate)
{
    if (lpIocpUpdate == &m_RecvOverlapped)
    {
        return this->OnUpdateRecv(lpIocpUpdate->m_dwNumOfBytesComplete);
    }
    else if (lpIocpUpdate == &m_SendOverlapped)
    {
        return this->OnUpdateSend(lpIocpUpdate->m_dwNumOfBytesComplete);
    }
    else
    {
        AssertEx(AL_CATAST, FALSE, _T("�����߼�"));
        return FALSE;
    }
}

// ��ȡ�߼������߳�Id
DWORD CMsSocket::GetLogicThreadId(VOID)
{
    return m_dwLogicThreadId;
}

// ��ȡ���紦���߳�Id
DWORD CMsSocket::GetNetThreadId(VOID)
{
    return m_dwNetThreadId;
}

// ���½���(�����Լ��������)
BOOL CMsSocket::OnUpdateRecv(DWORD dwNumOfBytesUpdate)
{
    // ѹ������
    BOOL bRet = m_MsRecvBuff.Push(dwNumOfBytesUpdate);
    if (!bRet)
    {
        return bRet;
    }

    //// ��ȡ��ɰ�����
    //DWORD dwPacketCount = m_MsRecvBuff.GetCompletePacketCount();
    //// ����а�,֪ͨ���д���
    //if (dwPacketCount > 0)
    //{
    //    ::PostThreadMessage(m_dwLogicThreadId, MSM_MSG_SOCKET_EXECUTE, (WPARAM)this, 0);
    //}

    // ��������
    return this->RecvData();
}

// ���·���(�����Լ��������)
BOOL CMsSocket::OnUpdateSend(DWORD dwNumOfBytesUpdate)
{
    m_MsSendBuff.Pop(dwNumOfBytesUpdate);

    DWORD dwDataSize = m_MsSendBuff.GetDataSize();
    if (dwDataSize == 0)
    {
        m_MsSendBuff.SetSending(FALSE);
        return TRUE;
    }
    else
    {
        return this->SendData();
    }
}

// ��ȡ�ն�ʱ��
FILETIME& CMsSocket::GetTime(VOID)
{
    return m_Time;
}

// �����ն�ʱ��
VOID CMsSocket::UpdataTime(VOID)
{
    ::GetSystemTimeAsFileTime(&m_Time);
}

// �ж��Ƿ���Ч
BOOL CMsSocket::IsValid(VOID)
{
    return (m_Socket != INVALID_SOCKET);
}


// ���ó�������
VOID CMsSocket::SetPoolIndex(ULONG_PTR ulIndex)
{
    m_ulIndexInPool = ulIndex;
}

// ��ȡ��������
ULONG_PTR CMsSocket::GetPoolIndex(VOID)
{
    return m_ulIndexInPool;
}

// ��ȡ����IO�ṹ��
S_IOCP_UPDATE* CMsSocket::GetRecvIO(VOID)
{
    return &m_RecvOverlapped;
}

// ��������
BOOL CMsSocket::RecvData(VOID)
{
    DWORD   dwFlags = 0;
    DWORD   dwTemp = 0;
    WSABUF  wb;

    NO_MFC_CODE(_tcout << _T("��������") << std::endl);
    // ׼������
    wb.buf = (LPCHAR)m_MsRecvBuff.GetRecvAddr(wb.len);

    // ���ջ�����û�пؼ�
    if (wb.len == 0)
    {
        return TRUE;
    }

    // ����
    INT nRet = ::WSARecv(
        m_Socket,
        &wb,
        1,
        &dwTemp,
        &dwFlags,
        &m_RecvOverlapped,
        NULL
        );

    // ���û��˳���������
    if (SOCKET_ERROR == nRet)
    {
        // ��ȡ������
        nRet = ::GetLastError();

        // ����������ڽ�����
        if (ERROR_IO_PENDING != nRet)
        {
            return FALSE;
        }
    }
    return TRUE;
}

// ��������
BOOL CMsSocket::SendData(VOID)
{
    DWORD   dwFlags = 0;
    DWORD   dwTemp = 0;
    WSABUF  wb;

    NO_MFC_CODE(_tcout << _T("��������") << std::endl);
    // ׼������
    wb.buf = (LPCHAR)m_MsSendBuff.GetSendAddr(wb.len);

    // ���ͻ�����û������
    if (wb.len == 0)
    {
        return TRUE;
    }

    m_MsSendBuff.SetSending(TRUE);

    // ����
    INT nRet = ::WSASend(
        m_Socket,
        &wb,
        1,
        &dwTemp,
        dwFlags,
        &m_SendOverlapped,
        NULL
        );

    // ���û��˳���������
    if (SOCKET_ERROR == nRet)
    {
        // ��ȡ������
        nRet = ::GetLastError();

        // ����������ڷ�����
        if (ERROR_IO_PENDING != nRet)
        {
            return FALSE;
        }
    }
    return TRUE;
}
