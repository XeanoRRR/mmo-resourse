#pragma once                //��//��
#ifndef __CMSSOCKET_H__     //��//��
#define __CMSSOCKET_H__     //��//��
//������������������������������//��
//�ļ����ƣ�CMsSocket.h         //��
//������������������׽��ַ�װ  //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ��ʱ�궨����ջ���������
#define TEMP_PACKET_RECV_COUNT  100

// �����׽���
class CMsSocket
{
// ����
public:

    // ���졢����
    CMsSocket(
        DWORD dwBuffSize,
        DWORD dwLogicThreadId,
        DWORD dwNetThreadId,
        CMsPacketFactory* lpFactory
        );
    virtual ~CMsSocket(VOID);

// ����
public:
    SOCKET  GetSocket(VOID);// ��ȡ�׽���
    DWORD   GetProt(VOID);  // ��ȡ�׽��ֶ˿�
    ACHAR*  GetAddr(VOID);  // ��ȡ�׽��ֵ�ַ
    BOOL    Release(VOID);  // �ͷ�
    BOOL    Reset(VOID);    // ����

    virtual BOOL ExecutePacket(VOID);  // ִ�а�
    virtual BOOL OnEnter(VOID);        // ����֪ͨ
    virtual BOOL OnExit(VOID);         // �˳�֪ͨ
    
    // ��ȡ���,׼��ִ��
    BOOL ReadPacket(VOID);

    // д����,׼������
    BOOL WritePacket(
        PACKET_TYPE PacketId,
        LPVOID lpData,
        DWORD dwSize = INVALID_UID
        );

    // д����,׼������
    BOOL WritePacket(
        CMsPacketFactory* lpFactory,
        PACKET_TYPE PacketId,
        LPVOID lpData,
        DWORD dwSize = INVALID_UID
        );

    LPVOID  GetPacketData(VOID);                    // ��ȡ�������
    BOOL    OnUpdate(S_IOCP_UPDATE* lpIocpUpdate);  // ���½���
    DWORD   GetLogicThreadId(VOID);                 // ��ȡ�߼������߳�Id
    DWORD   GetNetThreadId(VOID);                   // ��ȡ���紦���߳�Id
    FILETIME& GetTime(VOID);                        // ��ȡ�ն�ʱ��
    VOID    UpdataTime(VOID);                       // �����ն�ʱ��
    BOOL    IsValid(VOID);                          // �ж��Ƿ���Ч
    VOID    SetPoolIndex(ULONG_PTR ulIndex);        // ���ó�������
    ULONG_PTR GetPoolIndex(VOID);                   // ��ȡ��������
    S_IOCP_UPDATE* GetRecvIO(VOID);                 // ��ȡ����IO�ṹ��

// ����
protected:

    BOOL RecvData(VOID);    // ��������(�����Լ��������)
    BOOL SendData(VOID);    // ��������(�����Լ��������)
    BOOL OnUpdateRecv(DWORD dwNumOfBytesUpdate);    // ���½���(�����Լ��������)
    BOOL OnUpdateSend(DWORD dwNumOfBytesUpdate);    // ���·���(�����Լ��������)

    DWORD           m_dwLogicThreadId;  // �߼������߳�Id
    DWORD           m_dwNetThreadId;    // ���紦���߳�Id

    ULONG_PTR       m_ulIndexInPool;    // ����

    SOCKET          m_Socket;           // �׽���
    SOCKADDR_IN     m_Sockaddr;         // �׽��ֵ�ַ

    MsRecvBuffer    m_MsRecvBuff;       // ���ջ�����
    MsSendBuffer    m_MsSendBuff;       // ���ͻ�����

    S_IOCP_UPDATE   m_RecvOverlapped;   // �����ص�IO
    UINT64          m_u64RecvSize;      // ����IO����

    S_IOCP_UPDATE   m_SendOverlapped;   // �����ص�IO
    UINT64          m_u64SendSize;      // ����IO����

    LPVOID          m_lpPacketData;     // ִ�а�����
    CMsBasePacket*  m_lpPacket;         // ִ�а�ָ��

    FILETIME        m_Time;             // �ն�64λʱ��
    MsLock          m_Lock;             // �ն˷�����

    CMsPacketFactory* m_lpFactory;      // �������ָ��
};

//����������������������
#endif//#pragma once  ��
//����������������������
