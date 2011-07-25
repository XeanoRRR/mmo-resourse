#include "Precompiled.h"

CMsClientNetManager::CMsClientNetManager(
    TCHAR* szIP,
    WORD wProt,
    DWORD dwLogicThreadId,
    CMsPacketFactory* pCMsPacketFactory
    )
: m_wProt               (wProt)
, m_MsClientNetLog      (_T("%s/Log/MsClientNetManager.Log"), TRUE)
, m_pMsServerSocket     (NULL)
, m_bConnected          (FALSE)
, m_dwLogicThreadId     (dwLogicThreadId)
, m_pCMsPacketFactory   (pCMsPacketFactory)
{
    _ENTER_FUN_C;

    m_MsClientNetLog.SetSaveLog(FALSE);

    this->LogClientNet(_T("��ʼ����!"));

    AssertEx(m_pCMsPacketFactory, _T("�����������Ϊ��"));
    if (szIP)
    {
#ifdef UNICODE
        ::WideCharToMultiByte(
            CP_ACP, 
            0, 
            szIP,
            (INT)wcslen(szIP) + 1,
            m_szIp,
            IP_SIZE,
            NULL,
            NULL
            );
#else
        _tcscpy_s(m_szIp, IP_SIZE, szIP);
#endif
    }
    else
    {
        AssertEx(szIP, _T("IP��ַ����Ϊ��"));
    }

    this->LogClientNet(_T("������ɶ˿�!"));
    m_lpCMsIocpManager = NEW CMsIocpManager(_T("%s/Log/CMsClientIocpManager.log"), m_dwThreadId);
    this->LogClientNet(_T("�����������׽���!"));
    m_pMsServerSocket = NEW CMsServerSocket(
        MAX_SOCK_BUFF_SIZE,
        dwLogicThreadId,
        m_dwThreadId,
        m_pCMsPacketFactory
        );
    if (m_pMsServerSocket)
    {
        ;
    }
    else
    {
        AssertEx(m_pMsServerSocket, _T("�ڴ治��"));
    }

    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_addr.s_addr = ::inet_addr(m_szIp);
    m_ServerAddr.sin_port = ::htons(m_wProt);

    this->LogClientNet(_T("�����ɹ�!"));
    _LEAVE_FUN_DISPOSE_END_C;
}

CMsClientNetManager::~CMsClientNetManager(VOID)
{
    _ENTER_FUN_C;
    this->LogClientNet(_T("��ʼ�ͷ�!"));
    this->Disconnect();
    SAFE_DELETE(m_pMsServerSocket);
    SAFE_DELETE(m_lpCMsIocpManager);

    this->LogClientNet(_T("�ͷųɹ�!"));
    _LEAVE_FUN_DISPOSE_END_C;
    this->LogClientNet(_T("�ͷ�ʧ��!"));
}

// ��¼��־
VOID CMsClientNetManager::LogClientNet(TCHAR* szLog)
{
    if (szLog)
    {
        m_MsClientNetLog << L_LOCK << L_TIME;
        m_MsClientNetLog.Logf(_T("[�ͻ������������]\t%s"), szLog);
        m_MsClientNetLog << L_NEWLINE << L_UNLOCK;
    }
}

// �ж��Ƿ���Ч
BOOL CMsClientNetManager::IsValid(VOID)
{
    return (m_bConnected && m_pMsServerSocket->IsValid());
}

BOOL CMsClientNetManager::Connect(VOID)
{
    this->LogClientNet(_T("���ӷ�����!"));
    SOCKET sock = m_pMsServerSocket->GetSocket();

    // �������ӷ�����
    INT nRet = ::WSAConnect(
        sock,
        (SOCKADDR*)&m_ServerAddr,
        sizeof(m_ServerAddr),
        NULL,
        NULL,
        NULL,
        NULL
        );

    if (SOCKET_ERROR != nRet)
    {
        this->LogClientNet(_T("���׽��ֵ���ɶ˿�!"));
        BOOL bRet = m_lpCMsIocpManager->AssociateDevice(
            sock, 
            SERVER_SOCKET_INDEX
            );
        AssertEx(bRet, _T("�׽��ְ���ɶ˿�ʧ��"));

        // �����׽��ֳ�������
        m_pMsServerSocket->SetPoolIndex(SERVER_SOCKET_INDEX);

        m_bConnected = TRUE;
        this->LogClientNet(_T("֪ͨ���ӳɹ�!"));
        return ::PostThreadMessage(m_dwLogicThreadId, MSM_CLIENT_SOCKET_ON_ENTER, (WPARAM)m_pMsServerSocket, 0);
    }
    else
    {
        m_MsClientNetLog.ShowLastError();
        m_MsClientNetLog.LogfA(
            "���ӷ�����ʧ��\nIP:%s\n�˿�:%d\n",
            m_szIp,
            m_wProt            
            );
    }
    this->LogClientNet(_T("���ӷ�����ʧ��!"));
    return FALSE;
}

// �Ͽ�������
BOOL CMsClientNetManager::Disconnect(VOID)
{
    if (!m_bStop)
    {
        this->LogClientNet(_T("֪ͨ���ӶϿ�!"));
        ::PostThreadMessage(m_dwLogicThreadId, MSM_CLIENT_SOCKET_ON_EXIT, (WPARAM)m_pMsServerSocket, 0);

        this->LogClientNet(_T("��ʼֹͣ����!"));
        m_lpCMsIocpManager->Stop(TRUE);

        this->Stop(TRUE);

        return m_pMsServerSocket->Release();
    }
    return TRUE;
}

// ��ȡ�������׽���
lpCMsServerSocket CMsClientNetManager::GetServerSocket(VOID)
{
    return m_pMsServerSocket;
}

// ��Ϣ����
BOOL CMsClientNetManager::OnThreadMsgExecute(LPMSG lpMsg)
{
    // ����֪ͨ
    if (!lpMsg)
    {
        ::SleepEx(10, TRUE);
        return TRUE;
    }

    // ��Ϣ֪ͨ
    switch (lpMsg->message)
    {
        // ���������
    case MSM_IOCP_UPDATE:
        {
            // ��ɶ˿�֪ͨ�ṹ��
            S_IOCP_UPDATE* lpIocpUpdate = (S_IOCP_UPDATE*)lpMsg->wParam;
            m_pMsServerSocket->OnUpdate(lpIocpUpdate);
        }break;

        // �˳������
    case MSM_IOCP_EXIT_OR_ERROR:
        {
            this->LogClientNet(_T("���Ӵ���!"));
            this->Disconnect();
        }break;

        // �ⲿͶ�������Ϣ
    case MSM_IOCP_POST_QUEUED_COMPLETION_STATUS:
        {
            CMsSocket* lpSocket = (CMsSocket*)lpMsg->wParam;
            ULONG_PTR ulKey = lpSocket->GetPoolIndex();
            S_IOCP_UPDATE* pRecvIO = lpSocket->GetRecvIO();
            m_lpCMsIocpManager->PostCompletionStatus(ulKey, 0, pRecvIO);
        }break;

    default:
        {
            this->LogClientNet(_T("δ֪��Ϣ!"));
            AssertEx(FALSE, _T("δ֪IOCP��Ϣ"));
        }break;
    }
    return TRUE;
}

// �߳��¼�֪ͨ
VOID CMsClientNetManager::OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent)
{
    BOOL bRet = TRUE;
    switch (eThreadEvent)
    {
    case E_ON_THREAD_RUN:
        {
            this->LogClientNet(_T("�����߳̿�ʼ����!"));
            m_lpCMsIocpManager->Run();

            // ֱ�����ӳɹ���ֹͣ����
            while (!m_bStop && !m_bConnected)
            {
                bRet = this->Connect();
                if (bRet)
                {
                    break;
                }
                else
                {
                    ::SleepEx(100, TRUE);
                }
            }
        }break;

    case E_ON_THREAD_SUSPEND:
        {
            ;
        }break;

    default:
        {
            this->LogClientNet(_T("δ֪֪ͨ״̬!"));
            AssertEx(FALSE, _T("���ش����߼�"));
        }
    }
}