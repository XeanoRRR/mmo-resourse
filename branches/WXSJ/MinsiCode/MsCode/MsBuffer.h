#pragma once                //��//��
#ifndef __MSRECVBUFFER_H__  //��//��
#define __MSRECVBUFFER_H__  //��//��
//������������������������������//��
//�ļ����ƣ�MsRecvBuffer.h      //��
//�������������ջ�����          //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ���ͻ�����
struct MsRecvBuffer
{
    // ����,����
    MsRecvBuffer(DWORD dwPacketCount, DWORD dwMaxBuffSize);
    ~MsRecvBuffer(VOID);
    VOID operator = (MsRecvBuffer){AssertEx(AL_NORMAL, FALSE, _T("���ø�ֵ�����"));}

    // ��ȡ����λ�á��ߴ�
    LPBYTE  GetRecvAddr(DWORD& dwSize);

    // ��ȡ���гߴ�
    DWORD   GetFreeSize(VOID);

    // ���û�����
    VOID    Reset(VOID);

    // ѹ������
    BOOL    Push(DWORD dwSize);

    // ��������
    VOID    Pop(DWORD dwSize);

    // ���ִ��
    VOID    CompleteExe(VOID);

    // ��ȡִ�з��
    VOID    GetExePacket(OUT lpPacketHead& lpPacketHead, OUT LPVOID& lpData);

    // ��ȡ��ɰ�����
    DWORD   GetCompletePacketCount(VOID);

    // �����׽���ָ��
    VOID    SetSocket(CMsSocket* lpSocket);

    struct RecvPacketBuff
    {
        PacketHead  m_PacketHead;   // ��ͷ
        LPBYTE      m_lpPacketData; // ������
        BOOL        m_IsValid;      // ��Ч��
    }*              m_lpPacketHead; // ������
    CONST DWORD     m_dwPacketCount;// ��ͷ�������

    DWORD   m_dwCompletePacketCount;    // ��ɰ�����
    DWORD   m_dwCurrentRecvPacketIndex; // ��ǰ���հ�����
    DWORD   m_dwCurrentExePacketIndex;  // ��ǰִ�а�����
    DWORD   m_dwExeingPacketIndex;      // ��ǰ����ִ�а�������

    enum E_RECV_BUFF_STATE
    {
        RECV_HEAD,
        RECV_DATA,
    }               m_eRecvState;           // ����״̬
    DWORD           m_dwRecvedSize;         // �ѽ��ճߴ�

    LPBYTE          m_lpBuffHead;           // ������ͷ
    LPBYTE          m_lpBuffTail;           // ������β
    CONST DWORD     m_dwMaxBuffSize;        // ���������ߴ�

    CMsSocket*      m_lpSocket;             // �׽���ָ��
    MsLock          m_Lock;                 // �߳���

    LPBYTE          m_pDataHead;            // ����ͷָ��
    LPBYTE          m_pDataTail;            // ����βָ��
    DWORD           m_dwCurrentDataSize;    // ��ǰ���ݳߴ�
};


// ���ջ�����
struct MsSendBuffer
{
    // ����,����
    MsSendBuffer(DWORD dwMaxBuffSize);
    ~MsSendBuffer(VOID);
    VOID operator = (MsSendBuffer){AssertEx(AL_NORMAL, FALSE, _T("���ø�ֵ�����"));}

    // ��ȡ����λ�á��ߴ�
    LPBYTE  GetSendAddr(DWORD& dwSize);

    // ��ȡ���ݳߴ�
    DWORD   GetDataSize(VOID);

    // ��ȡ���гߴ�
    DWORD   GetFreeSize(VOID);

    // ���û�����
    VOID    Reset(VOID);

    // ѹ������
    BOOL    Push(LPBYTE lpData, DWORD dwSize);

    // ��������
    VOID    Pop(DWORD dwSize);

    // �Ƿ����ڷ���
    BOOL    IsSending(VOID);

    // �����Ƿ����ڷ���
    VOID    SetSending(BOOL bSending);

    LPBYTE          m_lpBuffHead;           // ������ͷ
    LPBYTE          m_lpBuffTail;           // ������β
    CONST DWORD     m_dwMaxBuffSize;        // ���������ߴ�

    BOOL            m_bSending;             // �Ƿ����ڷ���
    MsLock          m_Lock;                 // �߳���

    LPBYTE          m_pDataHead;            // ����ͷָ��
    LPBYTE          m_pDataTail;            // ����βָ��
    DWORD           m_dwCurrentDataSize;    // ��ǰ���ݳߴ�
};

//����������������������
#endif//#pragma once//��
//����������������������
