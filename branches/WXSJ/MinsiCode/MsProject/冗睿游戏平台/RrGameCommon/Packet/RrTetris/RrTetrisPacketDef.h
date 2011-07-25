#pragma once                    //��//��
#ifndef __MSRRGAMEPACKETDEF_H__ //��//��
#define __MSRRGAMEPACKETDEF_H__ //��//��
//����������������������������������//��
//�ļ����ƣ�MsRrGamePacketDef.h     //��
//���������������Ϸƽ̨������      //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

enum E_PACKET_TYPE
{
    DG2S_PACKET_BEGIN = E_PACKET_BOUND_RRTETRIS_BEGIN, // �����ʼ
    DG2S_PACKET_ENTER_GAME,     // DG2S ������Ϸ

    DG2S_PACKET_MAX_COUNT,      // ����˹���������ֵ
};

#include "DG2S_EnterGame.h"
static lpCMsBasePacket g_lpCMsBasePacket[] = 
{
    REGISTER_PACKET(DG2S_EnterGame),
};

// �궨��������
#define PACKET_MAX_COUNT sizeof(g_lpCMsBasePacket) / sizeof(lpCMsBasePacket)

//����������������������
#endif//#pragma once//��
//����������������������
