#include "Precompiled.h"

// ����
CMsClientSocket::CMsClientSocket(
    DWORD dwBuffSize,
    DWORD dwLogicThreadId,
    DWORD dwNetThreadId,
    CMsPacketFactory* lpFactory
    )
: CMsSocket(dwBuffSize, dwLogicThreadId, dwNetThreadId, lpFactory)
{
    ;
}

// ����
CMsClientSocket::~CMsClientSocket(VOID)
{
    ;
}

// �����׽���
VOID CMsClientSocket::SetSocket(SOCKET Socket)
{
    m_Socket = Socket;
}
