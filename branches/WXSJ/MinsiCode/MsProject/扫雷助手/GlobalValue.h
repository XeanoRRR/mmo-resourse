#pragma once

// �����̸���
#define MAX_PROCESS_COUNT 1024
using namespace std;

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

class CGlobalValue
{
public:
    static HWND     g_hWinMine;                     // ɨ�״��ھ��
    static DWORD    g_dwWinMineProcessId;           // ɨ�׳���Id
    static HANDLE   g_hWinMineProcessHandle;        // ɨ�׳�����


    static BOOL     g_bMineIsBegin;     // �����������
    static DWORD    g_dwMineCount;      // ���׸���
    static INT      g_nMineWidth;       // �������
    static INT      g_nMineHeight;      // �����߶�
    static DWORD    g_dwMineTimerCount; // ɨ�׼�ʱ
    static DWORD    g_dwWinMineCheck;   // ɨ�׼����
    static LPBYTE   g_lpMineArea;       // ɨ����Ϸ��
    static DWORD    g_dwMineCurrSpace;  // ɨ��ʣ��յ�
    static DWORD    g_dwMineMaxSpace;   // ɨ�����յ�



};

typedef CGlobalValue Global, G;