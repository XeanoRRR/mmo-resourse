#include "stdafx.h"
#include "Server.h"

// ���������캯��
CServer::CServer(VOID)
{
    _ENTER_FUN;

    _LEAVE_FUN_KEEP_NO_DISPOSE;
}

// ��������������
CServer::~CServer(VOID)
{
    _ENTER_FUN;

    _LEAVE_FUN_KEEP_NO_DISPOSE;
}

// ��Ϣ����
// ���ݸ���״̬
BOOL CServer::OnUpDataed(VOID)
{
    _ENTER_FUN;

    _LEAVE_FUN_KEEP_NO_DISPOSE;
    return FALSE;
}
// �˳�״̬
BOOL CServer::OnExit(VOID)
{
    _ENTER_FUN;

    _LEAVE_FUN_KEEP_NO_DISPOSE;
    return FALSE;
}

// ����״̬
BOOL CServer::OnError(VOID)
{
    _ENTER_FUN;

    ::AfxMessageBox(_T("�������Ͽ�"));

    _LEAVE_FUN_KEEP_NO_DISPOSE;
    return FALSE;
}

// �����ͷ�������������ͻ�����
BOOL CServer::WritePacket(CBasePacket* lpPacket)
{
    return CMsSocket::WritePacket(lpPacket);
}

// ���յ������ݿ��������ջ�����
USIZE CServer::ReadPacket(CBasePacket* lpPacket)
{
    return CMsSocket::ReadPacket(lpPacket);
}

// ִ���յ��ķ��
BOOL CServer::ExecutePacket(LPVOID pCClientNetManager)
{
    _ENTER_FUN;

    CClientNetManager* lpCClientNetManager = (CClientNetManager*)pCClientNetManager;
    return m_pBasePacket->ExecutePacket(lpCClientNetManager, this);

    _LEAVE_FUN_KEEP_NO_DISPOSE;
    return FALSE;
}