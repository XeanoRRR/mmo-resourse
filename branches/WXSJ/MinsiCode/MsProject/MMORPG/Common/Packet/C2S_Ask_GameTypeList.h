#pragma once                        //��//��
#ifndef __C2S_ASK_GAMETYPELIST_H__  //��//��
#define __C2S_ASK_GAMETYPELIST_H__  //��//��
#pragma pack(push, 1)               //��//��
//��������������������������������������//��
//�ļ����ƣ�C2S_Ask_GameTypeList.h      //��
//����������C2S������Ϸ�����б�         //��
//ƽ̨��أ�Windows                     //��
//������������������������������������������
// C2S������Ϸ�����б�
class C2S_Ask_GameTypeList;
BUILD_PACKET_BEGIN(C2S_Ask_GameTypeList, C2S_PACKET_ASK_GAME_TYPE)

// �����Ч����
struct{
    DWORD   m_dwGameTypeIndex;  // �ͻ�����Ϸ��������
    DWORD   m_dwData[2000];

}BUILD_PACKET_END(C2S_Ask_GameTypeList)
//����������������������
#pragma pack(pop)   //��
#endif//#pragma once//��
//����������������������
