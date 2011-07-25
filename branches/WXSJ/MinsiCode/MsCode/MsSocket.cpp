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

// ִ�а�(�����߼��̵߳���)
BOOL CMsSocket::ExecutePacket(DWORD dwLogicThreadId)
{
    if (m_dwLogicThreadId != dwLogicThreadId)
    {
        return FALSE;
    }
    BOOL bRet = TRUE;
    bRet = this->ReadPacket();
    if (bRet && m_lpPacket)
    {
        bRet = m_lpPacket->ExecutePacket(this);
        m_MsRecvBuff.CompleteExe();
    }
    else
    {
        AssertEx(m_lpPacket, _T("ִ�а�����Ϊ��"));
        AssertEx(bRet, _T("����������"));
    }
    return bRet;
}

// ����֪ͨ(�����߼��̵߳���)
BOOL CMsSocket::OnEnter(DWORD dwLogicThreadId)
{
    if (m_dwLogicThreadId != dwLogicThreadId)
    {
        return FALSE;
    }
    return this->RecvData();
}

// �˳�֪ͨ(�����߼��̵߳���)
BOOL CMsSocket::OnExit(DWORD dwLogicThreadId)
{
    if (m_dwLogicThreadId != dwLogicThreadId)
    {
        return FALSE;
    }
    this->Reset();
    return TRUE;
}

// ��ȡ���,׼��ִ��
BOOL CMsSocket::ReadPacket(VOID)
{
    lpPacketHead lpHead = NULL;
    m_MsRecvBuff.GetExePacket(lpHead, m_lpPacketData);
    AssertEx(lpHead, _T("�����߼�"));
    AssertEx(m_lpPacketData, _T("�����߼�"));

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
        AssertEx(m_lpFactory, _T("�����߼�"));
    }
    return FALSE;
}

// д����,׼������
BOOL CMsSocket::WritePacket(LPVOID lpData, DWORD dwSize)
{
    m_MsSendBuff.Push((LPBYTE)lpData, dwSize);
    if (!m_MsSendBuff.IsSending())
    {
        return this->SendData();
    }
    return TRUE;
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
        AssertEx(FALSE, _T("�����߼�"));
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
