#pragma once                        //��//��
#ifndef __S2C_RET_GAMETYPELIST_H__  //��//��
#define __S2C_RET_GAMETYPELIST_H__  //��//��
#pragma pack(push, 1)               //��//��
//��������������������������������������//��
//�ļ����ƣ�S2C_Ret_GameTypeList.h      //��
//����������C2S������Ϸ�����б�         //��
//ƽ̨��أ�Windows                     //��
//������������������������������������������
// S2C�ظ���Ϸ�����б�
class S2C_Ret_GameTypeList; // Ϊ��VA����������
BUILD_PACKET_BEGIN(S2C_Ret_GameTypeList, S2C_PACKET_RET_GAME_TYPE)   // �˴���ֹ�ֺ�

// �����Ч����
struct{
    S_GAME_TYPE_INFO    m_GameTypeInfo[MAX_GAME_TYPE_COUNT];
    DWORD               m_dwCount;
    DWORD               m_dwGameTypeIndex;

}BUILD_PACKET_END(S2C_Ret_GameTypeList)
//����������������������
#pragma pack(pop)   //��
#endif//#pragma once//��
//����������������������
