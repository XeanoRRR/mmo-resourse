#pragma once            //��    //��
#ifndef __SERVER_H__    //��    //��
#define __SERVER_H__    //��    //��
//��������������������������    //��
//�ļ����ƣ�Player.h            //��
//�������������������װ        //��
//ƽ̨��أ�Windows             //��
//����������������������������������

#include "MsSocket.h"
class CClientNetManager;

class CServer
    :public CMsSocket
{
public: // ���졢����

    CServer(VOID);  // ���������캯��
    ~CServer(VOID); // ��������������

public: // �ӿ�

    // ��Ϣ����
    BOOL    OnUpDataed(VOID);  // ���ݸ���״̬
    BOOL    OnExit(VOID);      // �˳�״̬
    BOOL    OnError(VOID);     // ����״̬
    BOOL    WritePacket(CBasePacket* lpPacket);         // �����ͷ�������������ͻ�����
    USIZE   ReadPacket(CBasePacket* lpPacket);          // ���յ������ݿ��������ջ�����
    BOOL    ExecutePacket(LPVOID pCClientNetManager);   // ִ���յ��ķ��
private:// ˽��

};


//����������������������
#endif//#pragma once//��
//����������������������
