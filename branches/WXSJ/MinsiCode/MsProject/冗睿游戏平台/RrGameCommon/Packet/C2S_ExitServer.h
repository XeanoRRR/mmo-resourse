#pragma once                //��//��
#ifndef __C2S_EXITSERVER_H__//��//��
#define __C2S_EXITSERVER_H__//��//��
#pragma pack(push, 1)       //��//��
//������������������������������//��
//�ļ����ƣ�C2S_ExitServer.h    //��
//����������C2S����������˳�   //��
//ƽ̨��أ�Windows             //��
//����������������������������������
// C2S����������˳�

// �رշ�����������󳤶�
#define SHUT_DOWN_PASSWORD_MAX_LEN  26

class C2S_ExitServer;
BUILD_PACKET_BEGIN(C2S_ExitServer, C2S_PACKET_SERVER_EXIT)

// �����Ч����
struct{
    TCHAR   m_szShutDownPassword[SHUT_DOWN_PASSWORD_MAX_LEN];

}BUILD_PACKET_END(C2S_ExitServer)

//����������������������
#pragma pack(pop)   //��
#endif//#pragma once//��
//����������������������
