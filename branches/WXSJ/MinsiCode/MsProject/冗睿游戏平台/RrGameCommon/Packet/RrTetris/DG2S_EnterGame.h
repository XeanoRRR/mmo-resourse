#pragma once                //��//��
#ifndef __C2S_CONNECT_H__   //��//��
#define __C2S_CONNECT_H__   //��//��
#pragma pack(push, 1)       //��//��
//������������������������������//��
//�ļ����ƣ�DG2S_EnterGame.h    //��
//����������ƽ̨C2S���������   //��
//ƽ̨��أ�Windows             //��
//����������������������������������
// DG2S��Ϸ����
#define GAMEENTER_KEY 1234567890  // ����KEY
class DG2S_EnterGame; // Ϊ��VA����������
BUILD_PACKET_BEGIN(DG2S_EnterGame, DG2S_PACKET_ENTER_GAME) // �˴���ֹ�ֺ�

// �����Ч����
struct{
    DWORD   m_dwData;

}BUILD_PACKET_END(DG2S_EnterGame)
//����������������������
#pragma pack(pop)   //��
#endif//#pragma once//��
//����������������������
