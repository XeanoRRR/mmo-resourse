#pragma once                    //��//��
#ifndef __CMSSERVERSOCKET_H__   //��//��
#define __CMSSERVERSOCKET_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�CMsServerSocket.h       //��
//���������������ն��׽��ַ�װ      //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// �����ն���
class CMsServerSocket
    : public CMsSocket
{
// ����
public:

    // ���졢����
    CMsServerSocket(
        DWORD dwBuffSize,
        DWORD dwLogicThreadId,
        DWORD dwNetThreadId,
        CMsPacketFactory* lpFactory
        );
    virtual ~CMsServerSocket(VOID);

protected:// ����
};

//����������������������
#endif//#pragma once  ��
//����������������������
