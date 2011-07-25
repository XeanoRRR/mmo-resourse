#pragma once                    //��//��
#ifndef __MSCLIENTNETMANAGER_H__//��//��
#define __MSCLIENTNETMANAGER_H__//��//��
//����������������������������������//��
//�ļ����ƣ�MsClientNetManager.h    //��
//�����������ͻ������������        //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

class CMsClientNetManager
    : public CMsMsgThread
{
public:
    // ����
    CMsClientNetManager(
        TCHAR* szIP,
        WORD wProt,
        DWORD dwLogicThreadId,
        CMsPacketFactory* pCMsPacketFactory
        );

    // ����
    virtual ~CMsClientNetManager(VOID);

    // ��־
    VOID LogClientNet(TCHAR* szLog);

    // �ж��Ƿ���Ч
    BOOL IsValid(VOID);

    // ���ӷ�����
    BOOL Connect(VOID);

    // �Ͽ�������
    BOOL Disconnect(VOID);

    // ֹ֮ͣǰ
    virtual BOOL ForeStop(VOID);

    // ��Ϣ����
    virtual VOID OnThreadMsgExecute(LPMSG lpMsg);

    // ���д���
    virtual VOID OnThreadFreeWork(VOID);

    // �߳��¼�֪ͨ
    virtual VOID OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent);

    // ��ȡ�������׽���
    lpCMsServerSocket GetServerSocket(VOID);
private:

    // �߼������̺߳���
    LPTHREAD_START_ROUTINE  m_lpLogicThread;

    // �߼������߳�Id
    DWORD               m_dwLogicThreadId;

    BOOL                m_bConnected;       // �Ƿ��Ѿ�������
    CMsPacketFactory*   m_pCMsPacketFactory;// �������������
    CMsIocpManager*     m_lpCMsIocpManager; // ��ɶ˿ڹ�����
    CMsServerSocket*    m_pMsServerSocket;  // �������׽��ֶ���
    ACHAR               m_szIp[IP_SIZE];    // ������IP
    WORD                m_wProt;            // �������˿�
    sockaddr_in         m_ServerAddr;
    CMsLog              m_MsClientNetLog;   // �����������־
};

typedef CMsClientNetManager* lpCMsClientNetManager;

//����������������������
#endif//#pragma once//��
//����������������������
