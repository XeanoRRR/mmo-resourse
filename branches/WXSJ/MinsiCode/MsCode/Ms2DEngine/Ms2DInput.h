#pragma once            //��//��
#ifndef __MS2DINPUT_H__ //��//��
#define __MS2DINPUT_H__ //��//��
//��������������������������//��
//�ļ����ƣ�Ms2DInput.h     //��
//����������2D��������ģ��  //��
//ƽ̨��أ�Windows         //��
//������������������������������

class CMs2DInput
{
public:
    // Ӣ�İ�������
    static LPTCHAR s_pszKeyNames_En[MS2D_INPUT_MAX_KEY_COUNT];

    // ���İ�������
    static LPTCHAR s_pszKeyNames_Cn[MS2D_INPUT_MAX_KEY_COUNT];

    // ���졢����
    CMs2DInput(
        HINSTANCE hInstance, HWND* phWnd,
        LPLONG lplScreenWidth, LPLONG lplScreenHeight);

    ~CMs2DInput(VOID);

    // ��ȡ�������
    VOID MSCALL Ms2DInput_GetMousePos(LPLONG lpX, LPLONG lpY);

    // �����������
    VOID MSCALL Ms2DInput_SetMousePos(LONG lX, LONG lY);

    // ��ȡ������ֵ
    INT MSCALL Ms2DInput_GetMouseWheel(VOID);

    // �ж�����Ƿ񴩹�
    BOOL MSCALL Ms2DInput_IsMouseOver(VOID);

    // ʵʱ�жϰ����Ƿ���
    BOOL MSCALL Ms2DInput_IsKeyDownRealTime(INT nKey);

    // �ж�ָ�������Ƿ��а����¼�����
    BOOL MSCALL Ms2DInput_IsKeyDownEvent(INT nKey);

    // �ж�ָ�������Ƿ����ɿ��¼�����
    BOOL MSCALL Ms2DInput_IsKeyUpEvent(INT nKey);

    // ��ȡָ������������
    TCHAR* MSCALL Ms2DInput_GetKeyName(INT nKey, DWORD& dwNameLen, BOOL bChineseName = FALSE);

    // ��ȡ���¼������İ�������
    DWORD MSCALL Ms2DInput_GetKey(VOID);

    // ��ȡ���¼������İ����ַ���
    DWORD MSCALL Ms2DInput_GetChar(VOID);

    // ��ȡ�����¼�
    BOOL MSCALL Ms2DInput_GetEvent(CMs2DInputEvent* pEvent);

    // �����һ�������¼�
    BOOL MSCALL Ms2DInput_ClearNextEvent(VOID);

    // ������������¼�
    BOOL MSCALL Ms2DInput_ClearAllEvent(VOID);

    // �����¼�
    VOID MSCALL Ms2DInput_BuildEvent(
        E_MS2D_INPUT_TYPE dwType,
        DWORD dwKey, SHORT snScan, 
        DWORD dwFlags,
        LONG lX, LONG lY
        );

private:

    HINSTANCE           m_hInstance;        // ����ʵ�����
    HWND*               m_phWnd;            // ���ھ��
    LPLONG              m_lplScreenWidth;   // ���ڿ��
    LPLONG              m_lplScreenHeight;  // ���ڸ߶�

    // Input
    DWORD               m_dwvKey;               // ��������
    DWORD               m_dwChar;               // �ַ���
    DWORD               m_dwFlags;              // �¼����
    LONG                m_lZpos;                // ������ֵ
    LONG                m_lXpos;                // ���x����
    LONG                m_lYpos;                // ���y����
    BOOL                m_bCaptured;            // �Ƿ񱻲���
    BOOL                m_bMouseOver;           // ����Ƿ񴩹�
    Ms2DInputEventQueue m_Ms2DInputEventQueue;  // �����¼�����
    MsLock              m_InputLock;            // ������

    // ����״̬����
    BYTE                m_keyz[MS2D_INPUT_MAX_KEY_COUNT*sizeof(TCHAR)];
};


//����������������������
#endif//#pragma once  ��
//����������������������
