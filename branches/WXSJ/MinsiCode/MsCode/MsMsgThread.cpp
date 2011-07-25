#include "Precompiled.h"

CMsMsgThread::CMsMsgThread(VOID)
{
    ZeroMemoryStruct(m_Msg);
    ::PeekMessage(&m_Msg, NULL, 0, 0, 0);
}

CMsMsgThread::~CMsMsgThread(VOID)
{
    ;
}

// �����߳�
BOOL CMsMsgThread::Run(VOID)
{
    BOOL bRet = FALSE;
    if (m_bStop)
    {
        bRet = CMsThread::Run();
        if (bRet)
        {
            bRet = FALSE;
            while (!bRet)
            {
                bRet = ::PostThreadMessage(m_dwThreadId, MSM_MSG_THREAD_ON_RUN, 0, 0);
                ::SleepEx(10, TRUE);
            }
        }
    }
    return TRUE;
}


// ����
BOOL CMsMsgThread::Suspend(VOID)
{
    BOOL bRet = FALSE;
    bRet = CMsThread::Suspend();
    if (bRet)
    {
        return ::PostThreadMessage(m_dwThreadId, MSM_MSG_THREAD_ON_SUSPEND, 0, 0);
    }
    return FALSE;
}

// ֹͣ�߳�
BOOL CMsMsgThread::Stop(BOOL IsBackup, BOOL bForce, DWORD dwWaitMillisecond)
{
    if (!m_bStop)
    {
        return CMsThread::Stop(IsBackup, bForce, dwWaitMillisecond);
    }
    return TRUE;
}

// �̻߳ص�
BOOL CMsMsgThread::ThreadCallBack(VOID)
{
    BOOL bRet = TRUE;
    // ֹ֮ͣǰѭ������
    while (!m_bStop)
    {
        m_Msg.message = MSM_NOTHING;
        bRet = ::PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE);
        if (!bRet)
        {
            this->OnThreadFreeWork();
            ::SleepEx(10, TRUE);
            continue;
        }
        switch (m_Msg.message)
        {
            // ֹͣ��Ϣ�߳�
        case MSM_MSG_THREAD_STOP:
            {
                return this->Stop();
            }
            break;

            // ����֪ͨ
        case MSM_NOTHING:
            {
                ;
            }break;

            // ֪ͨ�߳�����
        case MSM_MSG_THREAD_ON_RUN:
            {
                this->OnEventNotify(E_ON_THREAD_RUN);
            }break;

            // ֪ͨ�̹߳���
        case MSM_MSG_THREAD_ON_SUSPEND:
            {
                this->OnEventNotify(E_ON_THREAD_SUSPEND);
            }break;

            // Ĭ�ϴ���
        default:
            {
                this->OnThreadMsgExecute(&m_Msg);
            }
        }
    }
    return TRUE;
}

// Ͷ����Ϣ���߳�
BOOL CMsMsgThread::PostMsg(E_MSM e_Msm, WPARAM wParam, LPARAM lParam)
{
    if (m_bStop)
    {
        return INVALID_UID;
    }
    else
    {
        return ::PostThreadMessage(m_dwThreadId, e_Msm, wParam, lParam);
    }
}