#pragma once                //��//��
#ifndef __GAMESTATION_H__   //��//��
#define __GAMESTATION_H__   //��//��
//������������������������������//��
//�ļ����ƣ�GameStation.h       //��
//���������������Ϸƽ̨        //��
//ƽ̨��أ�Windows             //��
//����������������������������������
#include "BaseNode.h"
#include "GameTypeNode.h"
#include "GameTree.h"

class CGameStation
{
public:
    CGameStation(HWND hWnd, DWORD dwCurrentThreadId);
    ~CGameStation(VOID);

    BOOL Init();
    VOID Realese();

    // �ж������Ƿ���Ч
    BOOL IsNetValid(VOID);

    //��ƽ̨��Ⱦ
    VOID Render(VOID);

    //�õ������ļ�����  
    S_GameStationConfig* GetConfig(VOID);
    S_GameStationResConfig* GetResConfig(VOID);

    //���������ļ�
    VOID LoadConfig(VOID);  

    // ��ȡ�б���
    CGameTree* GetTree(VOID);
    //������Ϸ����
    //VOID AskNode(CBaseNode* pNode);
    
    //���ڵ�Idά��
    //DWORD GetNodeId(){return m_dwNodeId++;} 
    
    //ƽ̨��Ϣ����
    BOOL  GameStationWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
    
    //���������
    //CBaseNode* MouseTest(POINT Pos, CBaseNode* m_Node);                           
    
    // ��ȡ�������׽���
    lpCMsServerSocket GetServerSocket(VOID);

    HWND GetHwnd(VOID);

    // ��ȡ����ͼ
    CMsGdiBuffImage* GetBuffImage(VOID);

    // DllGame���ýӿ�
    static BOOL DllGameWritePacket(
        CMsPacketFactory* lpFactory,
        PACKET_TYPE PacketId,
        LPVOID lpData,
        DWORD dwSize = INVALID_WID
        );

private:
    BOOL                    m_bInit;                // ��ʼ�����
    HWND                    m_hWnd;                 // ƽ̨���ھ��
    DWORD                   m_dwGameWindowWidth;    // ƽ̨���ڿ��
    DWORD                   m_dwGameWindowHight;    // ƽ̨���ڸ߶�

    CGameTree*              m_pGameTree;            // ƽ̨���ṹ
    CMsGdiBuffImage*        m_pBuffImage;           // ����ͼ����

    HWND                    m_hDllGameWnd;          // DllGame���ھ��
    HMODULE                 m_hDllGameModule;       // DllGameģ����
    VOID(*m_funSetGameStationLogicThreadId)(DWORD); // DllGame�����߼��߳�Id�ӿ�
    HANDLE(*m_funGetDllGameLogicThread)(VOID);      // �õ�DllGame�߼��߳̾���ӿ�
    HANDLE(*m_funStopDllGame)(VOID);                // ֹͣGameDll�ӿ�

    S_GameStationConfig     m_Config;               // �����ļ��ṹ
    S_GameStationResConfig  m_ResConfig;            // ��Դ������Ϣ
    ServerConfig            m_ServerConfig;         // �����������ļ�
    CMsPacketFactory*       m_pCMsPacketFactory;    // �������
    CMsClientNetManager*    m_pCMsClientNetManager; // ���������
    DWORD                   m_dwCurrentThreadId;    // ��ǰ�߳�Id
};
//ȫ��ƽָ̨��
extern CGameStation* g_pCGameStation;

//����������������������
#endif//#pragma once  ��
//����������������������
