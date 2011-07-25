#pragma once                    //��//��
#ifndef __MSSERVERNETMANAGER_H__//��//��
#define __MSSERVERNETMANAGER_H__//��//��
//����������������������������������//��
//�ļ����ƣ�MsServerNetManager.h    //��
//�������������������������        //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ÿ֡�������ӵ�������
#define MAX_COUNT_LISTEN_IN_ONE_FRAME   5

// ÿ֡������ɶ˿ڸ���������
#define MAX_COUNT_UPDATE_IOPC_MSG       100

class CMsServerNetManager
    : public CMsMsgThread
{
public:
    // ����
    CMsServerNetManager(
        TCHAR* szIP,
        WORD wProt,
        DWORD dwLogicThreadId,
        DWORD dwMaxClientCount,
        CMsPacketFactory* pCMsPacketFactory
        );

    // ����
    virtual ~CMsServerNetManager(VOID);

    // ��¼��־
    VOID LogServerNet(TCHAR* szLog);

    // �ж��Ƿ���Ч
    BOOL IsValid(VOID);

    // ���ܿͻ���
    virtual BOOL NetAccept(VOID);

    // ֹ֮ͣǰ
    virtual BOOL ForeStop(VOID);

    // ��Ϣ����
    virtual VOID OnThreadMsgExecute(LPMSG lpMsg);

    // ���д���
    virtual VOID OnThreadFreeWork(VOID);

    // �߳��¼�֪ͨ
    virtual VOID OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent);

    // ��ȡָ���ͻ����׽���
    lpCMsClientSocket GetClientSocket(DWORD dwIndexByPool);

    // ��ȡ��ǰ�ͻ������Ӹ���
    DWORD GetCurrentClientCount(VOID);

    // �Ͽ�������
    BOOL StopNet(VOID);

private:

    // ����������
    BOOL RunNet(VOID);


    // ��������
    BOOL Accept(VOID);

    // ɾ��һ���ͻ���
    BOOL RemoveClient(CMsClientSocket* pCMsClientSocket);

    DWORD               m_dwLogicThreadId;  // �߼������߳�Id
    BOOL                m_bRunning;         // �Ƿ��Ѿ�������
    CMsPacketFactory*   m_pCMsPacketFactory;// �������������
    CMsIocpManager*     m_lpCMsIocpManager; // ��ɶ˿ڹ�����
    CMsServerSocket*    m_pMsServerSocket;  // �������׽��ֶ���
    lpCMsClientSocket*  m_ppMsClientSocket; // �ͻ����׽�������
    CMsPoolManager*     m_pCMsPoolManager;  // �������ӹ�����
    ACHAR               m_szIp[IP_SIZE];    // ������IP
    WORD                m_wProt;            // �������˿�
    sockaddr_in         m_ServerAddr;
    CMsLog              m_MsServerNetLog;   // �����������־
};

typedef CMsServerNetManager* lpCMsServerNetManager;

//����������������������
#endif//#pragma once//��
//����������������������