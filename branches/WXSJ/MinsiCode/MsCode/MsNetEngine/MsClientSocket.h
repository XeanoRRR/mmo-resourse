#pragma once                    //��//��
#ifndef __CMSCLIENTSOCKET_H__   //��//��
#define __CMSCLIENTSOCKET_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�CMsClientSocket.h       //��
//���������������ն��׽��ַ�װ      //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// �����ն���
class CMsClientSocket
    : public CMsSocket
{
// ����
public:

    // ���졢����
    CMsClientSocket(
        DWORD dwBuffSize,
        DWORD dwLogicThreadId,
        DWORD dwNetThreadId,
        CMsPacketFactory* lpFactory
        );
    virtual ~CMsClientSocket(VOID);

// ����
public:

    // �����׽���
    VOID SetSocket(SOCKET Socket);
protected:// ����
};

//����������������������
#endif//#pragma once  ��
//����������������������
