#pragma once                //��//��
#ifndef __S2C_FLYFILE_H__   //��//��
#define __S2C_FLYFILE_H__   //��//��
#pragma pack(push, 1)       //��//��
//������������������������������//��
//�ļ����ƣ�S2C_FlyFile.h       //��
//�������������ļ�              //��
//ƽ̨��أ�Windows             //��
//����������������������������������
// S2C���ļ�
class S2C_FlyFile; // Ϊ��VA����������
BUILD_PACKET_BEGIN(S2C_FlyFile, S2C_PACKET_FLY_FILE) // �˴���ֹ�ֺ�

// �����Ч����
struct{
    BYTE m_Data[KB_SIZE * 5];

}BUILD_PACKET_END(S2C_FlyFile)
//����������������������
#pragma pack(pop)   //��
#endif//#pragma once//��
//����������������������
