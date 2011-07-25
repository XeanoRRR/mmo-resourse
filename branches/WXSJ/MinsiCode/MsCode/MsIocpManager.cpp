#include "Precompiled.h"
#include "MsIocpManager.h"

// ��ɶ˿ڹ��캯��
CMsIocpManager::CMsIocpManager(TCHAR* szIocpLogFileName, DWORD dwThreadId)
: m_IsInit          (FALSE) // ��ǰ���󴴽�ʱ��δ��ʼ��
, m_hCompetionPort  (NULL)  // ��ɶ˿ھ����ʼ��ΪNULL
, m_MsIocpLog       (szIocpLogFileName, TRUE)
, m_dwParentThreadId(dwThreadId)
{
    _ENTER_FUN_C;

    m_MsIocpLog.SetSaveLog(FALSE);
    this->Init();

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}

// ��ɶ˿���������
CMsIocpManager::~CMsIocpManager(VOID)
{
    _ENTER_FUN_C;

    this->Release();

    // �����ɶ˿ھ����ʹ��
    if (m_hCompetionPort)
    {
        // �ͷ���ɶ˿ڶ���
        ::CloseHandle(m_hCompetionPort);
        m_hCompetionPort = NULL;
    }
    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}

// ��ʼ����ɶ˿�
BOOL CMsIocpManager::Init(VOID)
{
    _ENTER_FUN_C;
    this->LogIocp(_T("[��ʼ��ʼ��!]"));

    // ������ɶ˿ڶ���
    m_hCompetionPort = ::CreateIoCompletionPort(
        INVALID_HANDLE_VALUE,   // ���������µ���ɶ˿ڶ���
        NULL,                   // �����µ���ɶ˿ڶ���,����ΪNULL
        0,                      // �����µ���ɶ˿ڶ���,����Ϊ0
        0);                     // 0����ʹ�õ�ǰϵͳCPU���������߳�
    AssertEx(AL_CATAST, m_hCompetionPort, _T("��ɶ˿ڴ���ʧ��,����ʧ��"));

    this->LogIocp(_T("[�����ɹ�!]"));
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    this->LogIocp(_T("[����ʧ��!]"));
    return FALSE;
}

// �ͷ���ɶ˿�
BOOL CMsIocpManager::Release(VOID)
{
    _ENTER_FUN_C;
    this->LogIocp(_T("[��ʼ�ͷ�!]"));

    BOOL bRet = TRUE;

    this->Stop(TRUE, TRUE);

    if (m_hCompetionPort)
    {
        bRet = ::CloseHandle(m_hCompetionPort);
        AssertEx(AL_CATAST, bRet, _T("�ͷ���ɶ˿�ʧ��,����ʧ��"));
        m_hCompetionPort = NULL;
    }
    m_IsInit = FALSE;

    this->LogIocp(_T("[�ͷųɹ�!]"));
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    this->LogIocp(_T("[�ͷ�ʧ��!]"));
    return FALSE;
}

// ��¼��־
VOID CMsIocpManager::LogIocp(TCHAR* szLog)
{
    if (szLog)
    {
        m_MsIocpLog << L_LOCK_A << L_TIME;
        m_MsIocpLog.Logf(_T("[��ɶ˿�]\t\t%s"), szLog);
        m_MsIocpLog << L_NEWLINE << L_UNLOCK_A;
    }
}

// ��ָ���������ɶ˿ڶ���
BOOL CMsIocpManager::AssociateDevice(HANDLE hDevice, ULONG_PTR CompletionKey)
{
    _ENTER_FUN_C;
    this->LogIocp(_T("[�󶨾��]"));

    // ���½��ܶ���󶨵���ɶ˿�
    m_hCompetionPort = ::CreateIoCompletionPort(
        hDevice,            // ������������ɶ˿�
        m_hCompetionPort,   // �󶨵������ɶ˿�
        CompletionKey,      // �󶨺��Ψһ�ؼ���(KEY)
        0);                 // ���󶨶˿�ʱ���Դ˲���
    fAssertEx(
		AL_CATAST,
        m_hCompetionPort,
        _T("��ɶ˿ڰ�ʧ��,����ʧ��\n%s"),
        m_MsIocpLog.ShowLastError()
        );

    this->LogIocp(_T("[�󶨳ɹ�]"));
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    this->LogIocp(_T("[��ʧ��]"));
    return FALSE;
}

// ��ָ���׽��ֵ���ɶ˿ڶ���
BOOL CMsIocpManager::AssociateDevice(
    SOCKET hSocket,
    ULONG_PTR CompletionKey
    )
{
    _ENTER_FUN_C;
    this->LogIocp(_T("[��ʼ�󶨾��]"));

    // ���½��ܶ���󶨵���ɶ˿�
    m_hCompetionPort = ::CreateIoCompletionPort(
        (HANDLE)hSocket,    // ������׽��ֵ���ɶ˿�
        m_hCompetionPort,   // �󶨵������ɶ˿�
        CompletionKey,      // �󶨺��Ψһ�ؼ���(KEY)
        0);                 // ���󶨶˿�ʱ���Դ˲���
    fAssertEx(
		AL_CATAST,
		m_hCompetionPort,
        _T("��ɶ˿ڰ�ʧ��,����ʧ��\n%s"),
        m_MsIocpLog.ShowLastError()
        );

    this->LogIocp(_T("[�󶨳ɹ�]"));
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    this->LogIocp(_T("[��ʧ��]"));
    return FALSE;
}

// ��ȡ��ɰ�״̬
PORT_STATUS CMsIocpManager::GetCompletionStatus(
    ULONG_PTR* pCompletionKey,
    DWORD& dwNumOfBytesReceive,
    S_IOCP_UPDATE** lppOverlapped,
    DWORD dwWaitTime
    )
{
    _ENTER_FUN_C;

    BOOL bRet = FALSE;  // ��������ֵ

    // ��ȡ��ɰ�״̬
    bRet = ::GetQueuedCompletionStatus(
        m_hCompetionPort,               // ��ȡ�ĸ���ɶ˿ڵ�״̬
        &dwNumOfBytesReceive,           // ��ȡ��ɶ˿��յ��˶��ٸ��ֽ�
        pCompletionKey,                 // ��ȡ��ɶ˿ڰ����ĸ��ؼ���(KEY)��״̬
        (LPOVERLAPPED*)lppOverlapped,   // ��ȡ��ɶ˿��ص�״̬�ṹ��ָ��
        dwWaitTime);                    // �ȴ�ÿ��������ɵ�ʱ��

    // �����ɰ���ȡʧ��
    if (!bRet)
    {
        INT nErrorCode = ::GetLastError();
        if(WAIT_TIMEOUT == nErrorCode)
        {
            // ����״̬
            return COMPLETIONPORT_STATUS_NOTHING;
        }
        else if (ERROR_ABANDONED_WAIT_0 == nErrorCode)
        {
            this->LogIocp(_T("[���ӶϿ�]"));
            // ���ӱ������˳�
            return COMPLETIONPORT_STATUS_EXIT;
        }
        else
        {
            this->LogIocp(_T("[���Ӵ���]"));
            // ���Ӵ���
            return COMPLETIONPORT_STATUS_ERROR;
        }
    }

    // �����ɰ����յ�������
    else if (dwNumOfBytesReceive == 0)
    {
        this->LogIocp(_T("[���ӶϿ�]"));
        // ���ӱ������˳�
        return COMPLETIONPORT_STATUS_EXIT;
    }

    else if (dwNumOfBytesReceive == INVALID_UID)
    {
        dwNumOfBytesReceive = 0;
        this->LogIocp(_T("[���ݸ���]"));
        // ���������ݸ���
        return COMPLETIONPORT_STATUS_UPDATAED;
    }
    else
    {
        this->LogIocp(_T("[���ݸ���]"));
        // ���������ݸ���
        return COMPLETIONPORT_STATUS_UPDATAED;
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    this->LogIocp(_T("[δ֪״̬]"));
    return COMPLETIONPORT_STATUS_UNKNOW;
}

// Ͷ����ɰ�״̬
BOOL CMsIocpManager::PostCompletionStatus(
    ULONG_PTR CompletionKey,
    DWORD dwNumBytes,
    S_IOCP_UPDATE *pOverlapped
    )
{
    _ENTER_FUN_C;

    this->LogIocp(_T("[Ͷ��״̬]"));

    return ::PostQueuedCompletionStatus(
        m_hCompetionPort,
        dwNumBytes,
        CompletionKey,
        pOverlapped
        );

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    this->LogIocp(_T("[Ͷ��״̬ʧ��]"));
    return FALSE;
}

// �̻߳ص�
BOOL CMsIocpManager::ThreadCallBack(VOID)
{
    S_IOCP_UPDATE*  lpIocpUpdate;
    PORT_STATUS     PortStatus;
    ULONG_PTR       ulpCompletionKey = (ULONG_PTR)INVALID_NID;
    DWORD           dwNumOfBytesComplete;
    this->LogIocp(_T("[��ʼ�����߳�]"));
    while (!m_bStop)
    {
        PortStatus = this->GetCompletionStatus(
            &ulpCompletionKey,
            dwNumOfBytesComplete,
            &lpIocpUpdate,
            10
            );

        if (ulpCompletionKey == (ULONG_PTR)INVALID_NID)
        {
            continue;
        }

        switch (PortStatus)
        {
            // ��������ɸ���
        case COMPLETIONPORT_STATUS_UPDATAED:
            {
                lpIocpUpdate->m_PortStatus = PortStatus;
                lpIocpUpdate->m_ulpCompletionKey = ulpCompletionKey;
                lpIocpUpdate->m_dwNumOfBytesComplete = dwNumOfBytesComplete;
                this->LogIocp(_T("[֪ͨ���ݸ���]"));
                ::PostThreadMessage(m_dwParentThreadId, MSM_IOCP_UPDATE, (WPARAM)lpIocpUpdate, NULL);
            }break;

            // �쳣�Ͽ�
        case COMPLETIONPORT_STATUS_ERROR:

            // �����Ͽ�
        case COMPLETIONPORT_STATUS_EXIT:
            {
                this->LogIocp(_T("[֪ͨ���ӶϿ�]"));
                ::PostThreadMessage(m_dwParentThreadId, MSM_IOCP_EXIT_OR_ERROR, (WPARAM)ulpCompletionKey, NULL);
            }break;
        }
    }
    this->LogIocp(_T("[�ɹ��˳�]"));
    return TRUE;
}

// ֹ֮ͣǰ
BOOL CMsIocpManager::ForeStop(VOID)
{
    return TRUE;
}

// �߳��¼�֪ͨ
VOID CMsIocpManager::OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent, BOOL bReady)
{
    USE_PARAM(eThreadEvent, bReady);
}