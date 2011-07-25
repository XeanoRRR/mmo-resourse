#pragma once                //��//��
#ifndef __MSMSGTHREAD_H__   //��//��
#define __MSMSGTHREAD_H__   //��//��
//������������������������������//��
//�ļ����ƣ�MsMsgThread.h       //��
//������������Ϣ�̶߳���        //��
//ƽ̨��أ�WINDOWS             //��
//����������������������������������

// �ȴ��߳��������ʱʱ��
#define MAX_SLEEP_THREAD_TIME_MILLISECOND   1000

class CMsMsgThread
    : public CMsThread
{
// ����
public:
    CMsMsgThread(VOID);
    ~CMsMsgThread(VOID);

    // �����߳�
    virtual BOOL Run(VOID);

    // ����
    virtual BOOL Suspend(VOID);

    // ֹͣ�߳�
    virtual BOOL Stop(BOOL IsBackup = FALSE, BOOL bForce = FALSE, DWORD dwWaitMillisecond = 100);

    // �߳��¼�֪ͨ
    virtual VOID OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent) = 0;

    // ���д���
    virtual VOID OnThreadFreeWork(VOID) = 0;

    // ��Ϣ����
    virtual VOID OnThreadMsgExecute(LPMSG lpMsg) = 0;

    // Ͷ����Ϣ���߳�
    BOOL PostMsg(E_MSM e_Msm, WPARAM wParam, LPARAM lParam);

// ˽��
protected:
    MSG m_Msg;  // ��Ϣ

private:
    // �̻߳ص�
    virtual BOOL ThreadCallBack(VOID);

};

//����������������������
#endif//#pragma once//��
//����������������������
