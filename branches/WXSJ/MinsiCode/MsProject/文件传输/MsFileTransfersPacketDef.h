#pragma once                            //��    //��
#ifndef __MSFILETRANSFERSPACKETDEF_H__  //��    //��
#define __MSFILETRANSFERSPACKETDEF_H__  //��    //��
//������������������������������������������    //��
//�ļ����ƣ�MsFileTransfersPacketDef.h          //��
//�����������ļ�����������                    //��
//ƽ̨��أ�Windows                             //��
//��������������������������������������������������

enum E_PACKET_TYPE
{
    RR_GAME_PACKET_INVALID = -1,

    C2S_PACKET_BEGIN = 0x00000000,      // C2S �����ʼ
    C2S_PACKET_CONNECT,                 // C2S ��������

    S2C_PACKET_BEGIN = 0x00001000,      // S2C �����ʼ
    S2C_PACKET_RET_CONNECT,             // S2C �ظ�����
    S2C_PACKET_FLY_FILE,                // S2C ���ļ�
    S2C_PACKET_FILE_INFO,               // S2C �ļ���Ϣ

    RR_PACKET_MAX_COUNT,                // ������ֵ
};

#include "C2S_Connect.h"
#include "S2C_Connect.h"
#include "S2C_FlyFile.h"
#include "S2C_FileInfo.h"
static lpCMsBasePacket g_lpCMsBasePacket[] = 
{
    REGISTER_PACKET(C2S_Connect),
    REGISTER_PACKET(S2C_Connect),
    REGISTER_PACKET(S2C_FlyFile),
    REGISTER_PACKET(S2C_FileInfo),
};

// �궨��������
#define PACKET_MAX_COUNT sizeof(g_lpCMsBasePacket) / sizeof(lpCMsBasePacket)

//����������������������
#endif//#pragma once//��
//����������������������
