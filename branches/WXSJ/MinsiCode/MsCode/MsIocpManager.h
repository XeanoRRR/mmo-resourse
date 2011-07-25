#pragma once                    //��//��
#ifndef __CMSIOCPMANAGER_H__    //��//��
#define __CMSIOCPMANAGER_H__    //��//��
//����������������������������������//��
//�ļ����ƣ�CMsIocpManager.h        //��
//������������ɶ˿ڹ�����          //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ��ɶ˿ڷ��ص�״̬ö��
enum PORT_STATUS
{
    COMPLETIONPORT_STATUS_UNKNOW = 0,   // δ֪״̬
    COMPLETIONPORT_STATUS_NOTHING,      // ����״̬
    COMPLETIONPORT_STATUS_UPDATAED,     // �����ݸ���
    COMPLETIONPORT_STATUS_EXIT,         // ���ӱ������˳�
    COMPLETIONPORT_STATUS_ERROR,        // ���Ӵ���

    COMPLETIONPORT_STATUS_COERCE_DWORD = 0x7FFFFFFF,
};

// ��ɶ˿ڸ���֪ͨ�ṹ��
struct S_IOCP_UPDATE
    : public OVERLAPPED
{
    ULONG_PTR   m_ulpCompletionKey;
    DWORD       m_dwNumOfBytesComplete;
    PORT_STATUS m_PortStatus;
};

class CMsIocpManager
    : public CMsThread
{
public: // ���졢����
    CMsIocpManager(TCHAR* szIocpLogFileName, DWORD dwThreadId);
    virtual ~CMsIocpManager(VOID);

public: // �ӿ�
    BOOL AssociateDevice(   // ��ָ���������ɶ˿ڶ���
        HANDLE hDevice,
        ULONG_PTR CompletionKey
        );
    BOOL AssociateDevice(   // ��ָ���׽��ֵ���ɶ˿ڶ���
        SOCKET hSocket,
        ULONG_PTR CompletionKey
        );

    // ��ȡ��ɰ�״̬
    PORT_STATUS GetCompletionStatus(
        ULONG_PTR* pCompletionKey,
        DWORD& dwNumOfBytesReceive,
        S_IOCP_UPDATE** lppOverlapped,
        DWORD dwWaitTime
        );

    // ֹ֮ͣǰ
    virtual BOOL ForeStop(VOID);

    // �߳��¼�֪ͨ
    virtual VOID OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent, BOOL bReady);

    S_IOCP_UPDATE* GetRecvOverlapped(VOID); // ��ȡ�����ص�IO�ṹ��
    S_IOCP_UPDATE* GetSendOverlapped(VOID); // ��ȡ�����ص�IO�ṹ��

    BOOL PostCompletionStatus(              // Ͷ����ɰ�״̬
        ULONG_PTR CompletionKey,
        DWORD dwNumBytes,
        S_IOCP_UPDATE *pOverlapped
        );

    // �̻߳ص�
    virtual BOOL ThreadCallBack(VOID);

private:// ˽��

    BOOL Init(VOID);            // ��ʼ����ɶ˿�
    BOOL Release(VOID);         // �ͷ���ɶ˿�
    VOID LogIocp(TCHAR* szLog); // ��¼��־

    BOOL            m_IsInit;               // �Ƿ��Ѿ���ʼ��
    HANDLE          m_hCompetionPort;       // ��ɶ˿ھ��
    CMsLog          m_MsIocpLog;            // ��ɶ˿���־
    DWORD           m_dwParentThreadId;     // �����ߵ��߳�Id
};

//����������������������
#endif//#pragma once//��
//����������������������
