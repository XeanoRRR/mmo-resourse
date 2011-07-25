#include "Precompiled.h"

// ����
CMsServerSocket::CMsServerSocket(
    DWORD dwBuffSize,
    DWORD dwLogicThreadId,
    DWORD dwNetThreadId,
    CMsPacketFactory* lpFactory
    )
: CMsSocket(dwBuffSize, dwLogicThreadId, dwNetThreadId, lpFactory)
{
    // �����������׽���
    m_Socket = ::WSASocket(
        AF_INET,            // TCP��UDP
        SOCK_STREAM,        // TCP
        IPPROTO_TCP,        // TCP
        NULL,               // ��������ṹ��
        0,                  // ����
        WSA_FLAG_OVERLAPPED // �ص����
        );
    if (INVALID_SOCKET == m_Socket)
    {
        AssertEx(AL_CATAST, (INVALID_SOCKET != m_Socket), _T("�׽��ִ���ʧ��"));
    }
}

// ����
CMsServerSocket::~CMsServerSocket(VOID)
{
    ;
}
