#pragma once            //��//��
#ifndef __MSTHREAD_H__  //��//��
#define __MSTHREAD_H__  //��//��
//��������������������������//��
//�ļ����ƣ�MsThread.h      //��
//�����������̻߳�������    //��
//ƽ̨��أ�WINDOWS         //��
//������������������������������

// �߳��¼�֪ͨ
enum E_THREAD_EVENT_NOTIFY
{
    E_ON_THREAD_RUN,
    E_ON_THREAD_SUSPEND,
};

class CMsThread
{
// ����
public:
    CMsThread();
    virtual ~CMsThread();

    // �����߳�
    virtual BOOL Run(VOID);

    // ����
    virtual BOOL Suspend(VOID);

    // ֹ֮ͣǰ
    virtual BOOL ForeStop(VOID) = 0;

    // ֹͣ�߳�
    virtual BOOL Stop(BOOL IsBackup = FALSE, BOOL bForce = FALSE, DWORD dwWaitMillisecond = 100);

    // �̻߳ص�
    virtual BOOL ThreadCallBack(VOID);

    // ��ȡ�߳�Id
    DWORD GetMsThreadId(VOID);

// ˽��
protected:

    static DWORD WINAPI CMsThreadCallBack(LPVOID lpThreadParameter);

    // �Ƿ���������
    BOOL    m_IsRunning;

    // �Ƿ�ֹͣ����
    BOOL    m_bStop;

    // �߳�ID
    DWORD   m_dwThreadId;

    // �߳̾��
    HANDLE  m_hThread;
};









//����������������������
#endif//#pragma once//��
//����������������������
