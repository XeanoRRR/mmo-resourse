#pragma once                //��//��
#ifndef __MSRRGAMEDEF_H__   //��//��
#define __MSRRGAMEDEF_H__   //��//��
//������������������������������//��
//�ļ����ƣ�MsRrGameDef.h       //��
//���������������Ϸƽ̨����    //��
//ƽ̨��أ�Windows             //��
//����������������������������������

//����������������������������������
#define I_KNOW_THE_BASE_TYPE    //��
#define USE_MS_ASSERTX_FULL     //��
#define USE_MS_INI              //��
#define USE_MS_LOG              //��
#define USE_MS_NET              //��
#ifdef MS_MMO_CLIENT            //��
#   define USE_MS_WINDOW        //��
#   define USE_MS_CLIENT_NET    //��
#endif                          //��
#ifdef MS_MMO_SERVER            //��
#   define USE_MS_CONSOLE       //��
#   define USE_MS_SERVER_NET    //��
#endif                          //��
#include "MsBaseDef.h"          //��
//����������������������������������

enum E_RR_GAME_MSM
{
    MSM_RRGAME_SHUT_DWON_SERVER = MSM_MAX_COUNT + 0x0001,   // WPARAM 0         LPARAM 0        ֪ͨ�������˳�  W(0)    L(0)
};

// ����ֵ
#define HGEK_LBUTTON    0x01
#define HGEK_RBUTTON    0x02
#define HGEK_MBUTTON    0x04

//����������������������
#endif//#pragma once//��
//����������������������
