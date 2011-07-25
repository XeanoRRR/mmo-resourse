#pragma once            //��//��
#ifndef __MSCONSOLE_H__ //��//��
#define __MSCONSOLE_H__ //��//��
//��������������������������//��
//�ļ����ƣ�MsConsole.h     //��
//������������ϣͼ���      //��
//ƽ̨��أ�Windows         //��
//������������������������������

enum E_FOREGOUND_TEXT_COLOR
{
    FC_BLACK        = 0,                        // ��ɫǰ��

    FC_WHITE        =                           // ��ɫ��׼ǰ��
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED,

    FC_BLUE         = FOREGROUND_BLUE,          // ��ɫǰ��
    FC_GREEN        = FOREGROUND_GREEN,         // ��ɫǰ��
    FC_RED          = FOREGROUND_RED,           // ��ɫǰ��

    FC_BLUE_GREEN   =                           // ����ǰ��
    FOREGROUND_BLUE|FOREGROUND_GREEN,

    FC_BLUE_RED     =                           // ����ǰ��
    FOREGROUND_BLUE|FOREGROUND_RED,

    FC_GREEN_RED    =                           // �̺�ǰ��
    FOREGROUND_GREEN|FOREGROUND_RED,

    CL_RV           = COMMON_LVB_REVERSE_VIDEO, // ��ɫǰ��
    E_FOREGOUND_TEXT_COLOR_TO_DWORD = 0X7FFF
};

enum E_BACKGOUND_TEXT_COLOR
{
    BC_BLACK        = 0,                // ��ɫ��׼����

    BC_WHITE        =                   // ��ɫ����
    BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED,  

    BC_BLUE         = BACKGROUND_BLUE,  // ��ɫ����
    BC_GREEN        = BACKGROUND_GREEN, // ��ɫ����
    BC_RED          = BACKGROUND_RED,   // ��ɫ����

    BC_BLUE_GREEN   =                   // ���̱���
    BACKGROUND_BLUE|BACKGROUND_GREEN,

    BC_BLUE_RED     =                   // ���챳��
    BACKGROUND_BLUE|BACKGROUND_RED,

    BC_GREEN_RED    =                   // �̺챳��
    BACKGROUND_GREEN|BACKGROUND_RED,

    E_BACKGOUND_TEXT_COLOR_TO_DWORD = 0X7FFF
};
class CMsConsole
{
public:
    CMsConsole(RUNCALLBACK pRunCallBack);
    virtual ~CMsConsole(VOID);
public:

    // ��ʼ��
    BOOL Init(INT nArgc, LPTCHAR* strArgv);

    // �ͷ�
    BOOL Release(VOID);

    // ����
    BOOL Run(VOID);

    // ���ÿ���̨�ߴ�
    BOOL SetConsoleSize(DWORD dwSize);

    // ��ȡ����̨�ı���ɫ
    DWORD GetSithColor(VOID);

    // ���ÿ���̨����
    BOOL SetConsoleTitle(LPTCHAR szTitle);

    // ���������ı���ɫ
    BOOL SetSithTextColor(
        E_FOREGOUND_TEXT_COLOR fColor,
        E_BACKGOUND_TEXT_COLOR bColor,
        BOOL bfIntensity = FALSE,
        BOOL bbIntensity = FALSE
        );

    // ���ÿ���̨������ɫ
    BOOL SetSceneColor(
        E_BACKGOUND_TEXT_COLOR bColor,
        BOOL bbIntensity = FALSE
        );

    //COORD GetCursorPosition(VOID);                      // ��ȡ����̨���λ��
    //BOOL  SetCursorPosition(COORD dwCursorPosition);    // ���ÿ���̨���λ��

    //VOID OutText(LPTCHAR strText, DWORD dwLength, COORD dwCoord);
    //VOID OutTextW(LPWCHAR strText, DWORD dwLength, COORD dwCoord);
    //VOID OutTextA(LPCHAR strText, DWORD dwLength, COORD dwCoord);

private:
    RUNCALLBACK m_pRunCallBack;
    INT         m_nArgc;
    LPTCHAR*    m_strArgv;
    HANDLE      m_hConsoleOut;
    HANDLE      m_hConsoleErr;
    HANDLE      m_hConsoleIn;
    CONSOLE_SCREEN_BUFFER_INFO m_csbi;
    CONSOLE_FONT_INFOEX m_cfi;
    LPWORD      m_lpwAttributesBuff;
    size_t      m_sScreenBuffSize;
};

extern CMsConsole* g_pCMsConsole;

//����������������������
#endif//#pragma once//��
//����������������������
