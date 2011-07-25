#pragma once                //��//��
#ifndef __MSRRGAMEDEF_H__   //��//��
#define __MSRRGAMEDEF_H__   //��//��
//������������������������������//��
//�ļ����ƣ�MsRrGameDef.h       //��
//���������������Ϸƽ̨����    //��
//ƽ̨��أ�Windows             //��
//����������������������������������

//����������������������������������
#   define I_KNOW_THE_BASE_TYPE //��
#   define USE_MS_ASSERTX_FULL  //��
#   define USE_MS_INI           //��
#   define USE_MS_LOG           //��
#   define USE_MS_NET           //��
#ifdef MS_RRGAME_CLIENT         //��
#   define USE_MS_GDI_PLUS_IMAGE//��
#   define USE_MS_WINDOW        //��
#   define USE_MS_CLIENT_NET    //��
#include "MsBaseDef.h"          //��
#else                           //��
#   define USE_MS_CONSOLE       //��
#   define USE_MS_SERVER_NET    //��
#include "MsBaseDef.h"          //��
#endif                          //��
//����������������������������������

enum E_RR_GAME_MSM
{
    MSM_RRGAME_SHUT_DWON_SERVER = MSM_MAX_COUNT + 0x0001,   // WPARAM 0         LPARAM 0        ֪ͨ�������˳�  W(0)    L(0)
};

// ������Ϸ���͸���
#define MAX_GAME_TYPE_COUNT 10

// ��Ϸ����������󳤶�
#define GAME_TYPE_NAME_LEN  20

// ��Ϸ���ͽṹ��
struct S_GAME_TYPE_INFO 
{
    TCHAR   m_szGameTypeName[GAME_TYPE_NAME_LEN];   // ��Ϸ��������
    WORD    m_wIconId;                              // ������Ҫ�Ĺ��Id
    BYTE    m_IsGreenGroup;                         // �Ƿ�����ɫ��Ϸ��
};

// ����ֵ
#define HGEK_LBUTTON    0x01
#define HGEK_RBUTTON    0x02
#define HGEK_MBUTTON    0x04

// �ڵ�����
enum E_NODE_TYPE
{
    NODE_ROOT,
    NODE_GAMETYPE,
    NODE_GAME,
    NODE_DISTRIC,
    NODE_ROOM,
    NODE_DESK,
};

// ����DllGame���ʹ�÷�Χ
enum E_PACKET_BOUND
{
    E_PACKET_BOUND_BEGIN = 0x00000000,                  // �ʼ
    E_PACKET_BOUND_GAME_STATION_BEGIN = 0x00001000,     // ƽ̨��ʼ
    E_PACKET_BOUND_RRTETRIS_BEGIN = 0x00002000,         // ����˹���鿪ʼ
};

// ��Ϸƽ̨������Ϣ
struct S_GameStationConfig
{
    DWORD dwGameStationWindowWidth;
    DWORD dwGameStationWindowHeight;
    DWORD m_dwBetween;

    DWORD dwGameTypeWidth;
    DWORD dwGameTypeHeight;
    DWORD dwGameTypeIndent;

    DWORD dwGameWidth;
    DWORD dwGameHeight;
    DWORD dwGameIndent;

    DWORD dwDistrictWidth;
    DWORD dwDistrictHeight;
    DWORD dwDistrictIndent;

    DWORD dwRoomWidth;
    DWORD dwRoomHeight;
    DWORD dwRoomIndent;

    DWORD dwDeskWidth;
    DWORD dwDeskHeight;
    DWORD dwDeskIndent;
};

// ������������Ϣ
struct ServerConfig
{
    TCHAR IP[IP_SIZE];
    WORD  wProt;  
};

// �궨����Դ�ļ�����󳤶�
#define MAX_RES_FILE_NAME_LEN   20

// ��Դ����
#define MAX_RES_COUNT           20
struct S_GameStationResConfig
{
    TCHAR   m_szImageDir[MAX_PATH];
    TCHAR   m_szImageName[MAX_RES_FILE_NAME_LEN][MAX_RES_COUNT];

    TCHAR   m_szMusicDir[MAX_PATH];
    TCHAR   m_szMusicName[MAX_RES_FILE_NAME_LEN][MAX_RES_COUNT];

    TCHAR   m_szSoundDir[MAX_PATH];
    TCHAR   m_szSoundName[MAX_RES_FILE_NAME_LEN][MAX_RES_COUNT];
};

// ��Ϸƽ̨����ߴ�
#define RR_GAME_CAPTION_TEXT_SIZE   24
//����������������������
#endif//#pragma once//��
//����������������������
