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
    RR_GAME_PACKET_INVALID = -1,

    C2S_PACKET_BEGIN = 0x00000000,      // C2S �����ʼ
    C2S_PACKET_CONNECT,                 // C2S ��������
    C2S_PACKET_ASK_GAME_TYPE,           // C2S ������Ϸ�����б�
    C2S_PACKET_ASK_GAME,                // C2S ������Ϸ�б�
    C2S_PACKET_ASK_DISTRIC,             // C2S ��������б�
    C2S_PACKET_ASK_ROOM,                // C2S ���󷿼��б�
    C2S_PACKET_ASK_DESK,                // C2S ���������б�

    C2S_PACKET_SERVER_EXIT,             // ����ʹ��,����������˳�

    S2C_PACKET_BEGIN = 0x00001000,      // S2C �����ʼ
    S2C_PACKET_RET_CONNECT,             // S2C �ظ�����
    S2C_PACKET_RET_GAME_TYPE,           // S2C �ظ���Ϸ�����б�
    C2S_PACKET_RET_GAME,                // S2C �ظ���Ϸ�б�
    C2S_PACKET_RET_DISTRIC,             // S2C �ظ������б�
    C2S_PACKET_RET_ROOM,                // S2C �ظ������б�
    C2S_PACKET_RET_DESK,                // S2C �ظ������б�

    RR_GAME_PACKET_MAX_COUNT,           // ƽ̨������ֵ
};

#include "C2S_Connect.h"
#include "S2C_Connect.h"
#include "C2S_Ask_GameTypeList.h"
#include "S2C_Ret_GameTypeList.h"
#include "C2S_ExitServer.h"
static lpCMsBasePacket g_lpCMsBasePacket[] = 
{
    REGISTER_PACKET(C2S_Connect),
    REGISTER_PACKET(S2C_Connect),
    REGISTER_PACKET(C2S_Ask_GameTypeList),
    REGISTER_PACKET(S2C_Ret_GameTypeList),
    REGISTER_PACKET(C2S_ExitServer),
};

// �궨��������
#define PACKET_MAX_COUNT sizeof(g_lpCMsBasePacket) / sizeof(lpCMsBasePacket)

//����������������������
#endif//#pragma once//��
//����������������������
