#include "Precompiled.h"

CMsServerNetManager::CMsServerNetManager(
    TCHAR* szIP,
    WORD wProt,
    DWORD dwLogicThreadId,
    DWORD dwMaxClientCount,
    CMsPacketFactory* pCMsPacketFactory
    )
: m_wProt               (wProt)
, m_MsServerNetLog      (_T("%s/Log/MsServerNetManager.Log"), TRUE)
, m_ppMsClientSocket    (NULL)
, m_bRunning            (FALSE)
, m_dwLogicThreadId     (dwLogicThreadId)
, m_pCMsPacketFactory   (pCMsPacketFactory)
{
    _ENTER_FUN_C;

    m_MsServerNetLog.SetSaveLog(TRUE);

    this->LogServerNet(_T("��ʼ����!"));

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

    this->LogServerNet(_T("�����������׽���!"));
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

    this->LogServerNet(_T("������ɶ˿�!"));
    m_lpCMsIocpManager = NEW CMsIocpManager(_T("%s/Log/CMsServerIocpManager.log"), m_dwThreadId);
    this->LogServerNet(_T("������������!"));
    m_pCMsPoolManager = NEW CMsPoolManager(dwMaxClientCount, TRUE);
    this->LogServerNet(_T("�����ͻ����׽�������!"));
    m_ppMsClientSocket = NEW lpCMsClientSocket[dwMaxClientCount];
    if (m_pCMsPoolManager && m_ppMsClientSocket)
    {
        FOR_x_TO(i, dwMaxClientCount)
        {
            m_ppMsClientSocket[i] = NEW CMsClientSocket(
                MAX_SOCK_BUFF_SIZE,
                dwLogicThreadId,
                m_dwThreadId,
                m_pCMsPacketFactory
                );
            if (m_ppMsClientSocket[i])
            {
                ;
            }
            else
            {
                AssertEx(m_ppMsClientSocket[i], _T("�ڴ治��"));
            }
        }
    }
    else
    {
        AssertEx(m_pCMsPoolManager, _T("�ڴ治��"));
        AssertEx(m_ppMsClientSocket, _T("�ڴ治��"));
    }

    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_addr.s_addr = ::inet_addr(m_szIp);
    m_ServerAddr.sin_port = ::htons(m_wProt);

    this->LogServerNet(_T("�����ɹ�!"));
    _LEAVE_FUN_DISPOSE_END_C;
}

CMsServerNetManager::~CMsServerNetManager(VOID)
{
    this->LogServerNet(_T("��ʼ�ͷ�!"));
    this->StopNet();
    DWORD dwMaxClientCount = m_pCMsPoolManager->GetMaxCount();
    FOR_x_TO(i, dwMaxClientCount)
    {
        SAFE_DELETE(m_ppMsClientSocket[i]);
    }
    SAFE_DELETE_ARRAY(m_ppMsClientSocket);
    SAFE_DELETE(m_pCMsPoolManager);
    SAFE_DELETE(m_pMsServerSocket);
    SAFE_DELETE(m_lpCMsIocpManager);

    this->LogServerNet(_T("�ͷųɹ�!"));
}

// ��¼��־
VOID CMsServerNetManager::LogServerNet(TCHAR* szLog)
{
    if (szLog)
    {
        m_MsServerNetLog << L_LOCK << L_TIME;
        m_MsServerNetLog.Logf(_T("[���������������]\t%s"), szLog);
        m_MsServerNetLog << L_NEWLINE << L_UNLOCK;
    }
}

// �ж��Ƿ���Ч
BOOL CMsServerNetManager::IsValid(VOID)
{
    return m_pMsServerSocket->IsValid();
}

// ����������
BOOL CMsServerNetManager::RunNet(VOID)
{
    _ENTER_FUN_C;

    SOCKET sock = m_pMsServerSocket->GetSocket();

    // �����׽���Ϊ������ģʽ
    ULONG IsNoEncumber = TRUE;
    ::ioctlsocket(sock, FIONBIO, &IsNoEncumber);

    this->LogServerNet(_T("�󶨷�����!"));
    // ����ǰ��������ַ
    INT nRet = ::bind(sock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr));
    AssertExFormat(SOCKET_ERROR != nRet, _T("::bindʧ��\n%s"), m_MsServerNetLog.ShowLastError());

    this->LogServerNet(_T("����������!"));
    // ����������
    nRet = ::listen(sock, MAX_COUNT_LISTEN_IN_ONE_FRAME);
    AssertExFormat(SOCKET_ERROR != nRet, _T("::listenʧ��\n%s"), m_MsServerNetLog.ShowLastError());

    m_bRunning = TRUE;

    this->LogServerNet(_T("�����������ɹ�!"));
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    this->LogServerNet(_T("����������ʧ��!"));
    return FALSE;
}

// �Ͽ�������
BOOL CMsServerNetManager::StopNet(VOID)
{
    if (!m_bStop)
    {
        this->LogServerNet(_T("��ʼֹͣ������!"));
        DWORD dwMaxClientCount = m_pCMsPoolManager->GetMaxCount();
        FOR_x_TO(i, dwMaxClientCount)
        {
            m_ppMsClientSocket[i]->Reset();
        }
        m_pCMsPoolManager->Reset();

        m_lpCMsIocpManager->Stop(TRUE);

        this->Stop();
        this->LogServerNet(_T("ֹͣ�������ɹ�!"));
    }
    return TRUE;
}

// ��������
BOOL CMsServerNetManager::Accept(VOID)
{
    _ENTER_FUN_C;

    // ��ȡ�����¸�λ��
    DWORD dwNextIndex = m_pCMsPoolManager->GetNextIndex(FALSE);

    // ����������ʧ��
    if (INVALID_UID == dwNextIndex)
    {
        return FALSE;
    }

    // ��ȡ��������Ϣ
    SOCKET ServerSocket = m_pMsServerSocket->GetSocket();
    INT nSockAddrSize = sizeof(m_ServerAddr);

    // ����������
    SOCKET AcceptSocket = ::WSAAccept(
        ServerSocket,
        (SOCKADDR*)&m_ServerAddr,
        &nSockAddrSize,
        NULL,
        0
        );

    // �����������
    if (AcceptSocket != INVALID_SOCKET)
    {
        this->LogServerNet(_T("���������ܵ��µ�����!"));
        // ռ��һ������λ��
        m_pCMsPoolManager->GetNextIndex(TRUE);

        // ������λ�ü�¼���׽�����
        m_ppMsClientSocket[dwNextIndex]->SetPoolIndex(dwNextIndex);
        // �������׽��ּ�¼���׽�����
        m_ppMsClientSocket[dwNextIndex]->SetSocket(AcceptSocket);

        this->LogServerNet(_T("�������ӵ���ɶ˿�!"));
        // ����ɶ˿�
        BOOL bRet = m_lpCMsIocpManager->AssociateDevice(
            AcceptSocket, 
            dwNextIndex
            );
        AssertEx(bRet, _T("�׽��ְ���ɶ˿�ʧ��"));

        this->LogServerNet(_T("֪ͨ���µ����ӽ���!"));
        return ::PostThreadMessage(m_dwLogicThreadId, MSM_SERVER_SOCKET_ON_ENTER, (WPARAM)m_ppMsClientSocket[dwNextIndex], 0);
    }
    else
    {
        return FALSE;
    }

    _LEAVE_FUN_DISPOSE_END_C;
    this->LogServerNet(_T("����������ʧ��!"));
    return FALSE;
}

// ���ܿͻ���
BOOL CMsServerNetManager::NetAccept(VOID)
{
    // ֹ֮ͣ�󲻴���
    if (!m_bStop)
    {
        return this->Accept();
    }
    return TRUE;
}

// ��Ϣ����
BOOL CMsServerNetManager::OnThreadMsgExecute(LPMSG lpMsg)
{
    // ����֪ͨ
    if (!lpMsg)
    {
        FOR_x_TO(i, MAX_COUNT_LISTEN_IN_ONE_FRAME)
        {
            if (m_pCMsPoolManager && !m_pCMsPoolManager->IsFull())
            {
                // �����µĿͻ���
                BOOL bRet = this->NetAccept();
                if (!bRet)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

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
            S_IOCP_UPDATE*  lpIocpUpdate = (S_IOCP_UPDATE*)lpMsg->wParam;
            m_ppMsClientSocket[lpIocpUpdate->m_ulpCompletionKey]->OnUpdate(lpIocpUpdate);
            return TRUE;
        }break;

        // �˳������
    case MSM_IOCP_EXIT_OR_ERROR:
        {
            CMsClientSocket* pCMsClientSocket = m_ppMsClientSocket[(ULONG_PTR)lpMsg->wParam];
            this->RemoveClient(pCMsClientSocket);
            return TRUE;
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
            this->LogServerNet(_T("δ֪��Ϣ!"));
            AssertEx(FALSE, _T("δ֪IOCP��Ϣ"));
        }break;
    }
    return FALSE;
}

// ��ȡָ���ͻ����׽���
lpCMsClientSocket CMsServerNetManager::GetClientSocket(DWORD dwIndexByPool)
{
    DWORD dwMaxClientCount = m_pCMsPoolManager->GetMaxCount();
    if (dwMaxClientCount <= dwIndexByPool || !m_ppMsClientSocket)
    {
        return NULL;
    }
    return m_ppMsClientSocket[dwIndexByPool];
}

// ��ȡ��ǰ�ͻ������Ӹ���
DWORD CMsServerNetManager::GetCurrentClientCount(VOID)
{
    return m_pCMsPoolManager->GetCurrentCount();
}

// �ر�һ���ͻ���
BOOL CMsServerNetManager::RemoveClient(CMsClientSocket* pCMsClientSocket)
{
    DWORD dwIndexByPool = (DWORD)pCMsClientSocket->GetPoolIndex();
    (m_MsServerNetLog << L_TIME).Logf(_T("[���������������]\t[�Ͽ�����!][%d]\n"), dwIndexByPool);
    DWORD dwMaxClientCount = m_pCMsPoolManager->GetMaxCount();
    if (dwMaxClientCount <= dwIndexByPool || !m_ppMsClientSocket)
    {
        return FALSE;
    }
    else
    {
        this->LogServerNet(_T("֪ͨ�����ӶϿ�!"));
        ::PostThreadMessage(m_dwLogicThreadId, MSM_SERVER_SOCKET_ON_EXIT, (WPARAM)pCMsClientSocket, 0);
        return m_pCMsPoolManager->RemoveByPoolIndex(dwIndexByPool);
    }
}

// �߳��¼�֪ͨ
VOID CMsServerNetManager::OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent)
{
    BOOL bRet = TRUE;
    switch (eThreadEvent)
    {
    case E_ON_THREAD_RUN:
        {
            this->LogServerNet(_T("�����߳̿�ʼ����!"));
            m_lpCMsIocpManager->Run();
            // ֱ�������ɹ���ֹͣ����
            while (!m_bStop && !m_bRunning)
            {
                bRet = this->RunNet();
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
            this->LogServerNet(_T("δ֪֪ͨ״̬!"));
            AssertEx(FALSE, _T("���ش����߼�"));
        }
    }
}