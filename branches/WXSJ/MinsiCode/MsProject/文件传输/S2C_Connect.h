#pragma once                //��//��
#ifndef __S2C_CONNECT_H__   //��//��
#define __S2C_CONNECT_H__   //��//��
#pragma pack(push, 1)       //��//��
//������������������������������//��
//�ļ����ƣ�S2C_Connect.h       //��
//����������ƽ̨S2C���ӻظ���   //��
//ƽ̨��أ�Windows             //��
//����������������������������������
// S2C���Ӱ�
class S2C_Connect; // Ϊ��VA����������
BUILD_PACKET_BEGIN(S2C_Connect, S2C_PACKET_RET_CONNECT) // �˴���ֹ�ֺ�

// �����Ч����
struct{
    BOOL m_bOK;

}BUILD_PACKET_END(S2C_Connect)
//����������������������
#pragma pack(pop)   //��
#endif//#pragma once//��
//����������������������
