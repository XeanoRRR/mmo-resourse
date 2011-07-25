#pragma once            //��//��
#ifndef __MS2DENGINE_H__//��//��
#define __MS2DENGINE_H__//��//��
//��������������������������//��
//�ļ����ƣ�Ms2DEngine.h    //��
//����������2D��Ϸ������  //��
//ƽ̨��أ�Windows         //��
//������������������������������

class CMs2DEngine
{

// ���졢����
public:
    CMs2DEngine(VOID);
    ~CMs2DEngine(VOID);
// �ӿ�
public:

    static VOID                     s_Release(VOID);          // �ͷ�
    static LPDIRECT3D9              s_GetDirect3D9(VOID);     // ��ȡD3D9����
    static LPDIRECT3DDEVICE9        s_GetDirectDevice9(VOID); // ��ȡD3D9�豸
    static D3DPRESENT_PARAMETERS*   s_GetDirect3DPP(VOID);    // ��ȡD3D9����

    // ����D3D9
    static BOOL s_CreateD3D9(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight);

    // ɾ��D3D9
    static VOID s_DeleteD3D9(VOID);

    // ����D3D9����
    static ID3DXFont* s_CreateD3D9Font(
        DWORD dwSize,                                           // �ߴ�
        DWORD dwWeight = FW_BOLD,                               // ����
        DWORD dwMipLevels = 1,                                  // Mip�ȼ�
        BOOL bItalic = FALSE,                                   // �Ƿ�б��
        LPCTSTR szFaceName = _T("����"),                        // ��������
        DWORD dwCharSet = DEFAULT_CHARSET,                      // �ַ���
        DWORD dwOutputPrecision = OUT_DEFAULT_PRECIS,           // �������
        DWORD dwQuality = DEFAULT_QUALITY,                      // Ʒ��
        DWORD dwPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE    // �Ƕ�
        );

    // ����Ms2DEngine����
    static VOID s_Ms2DEngine_SetState(
        E_MS2DENGINE_STATE_BOOL eState,
        BOOL bValue
        );

    static VOID s_Ms2DEngine_SetState(
        E_MS2DENGINE_STATE_FUNC eState,
        Ms2DCBFun funValue
        );

    static VOID s_Ms2DEngine_SetState(
        E_MS2DENGINE_STATE_HWND eState,
        HWND hValue
        );

    static VOID s_Ms2DEngine_SetState(
        E_MS2DENGINE_STATE_INT eState,
        INT nValue
        );

    static VOID s_Ms2DEngine_SetState(
        E_MS2DENGINE_STATE_STRING eState,
        CONST LPTCHAR szValue
        );

    // ��ȡMs2DEngine����
    static BOOL             s_Ms2DEngine_GetState(E_MS2DENGINE_STATE_BOOL);
    static Ms2DCBFun        s_Ms2DEngine_GetState(E_MS2DENGINE_STATE_FUNC);
    static HWND             s_Ms2DEngine_GetState(E_MS2DENGINE_STATE_HWND);
    static INT              s_Ms2DEngine_GetState(E_MS2DENGINE_STATE_INT);
    static CONST LPTCHAR    s_Ms2DEngine_GetState(E_MS2DENGINE_STATE_STRING);

    static CMsIni*  s_GetIni(VOID){return m_pMsIni;}// ��ȡ���ù�����
    static BOOL     s_Log(TCHAR* szText);           // ��¼��־
    static BOOL     s_Logf(TCHAR* szText, ...);     // ��¼��־
    static VOID     s_EnableLog(BOOL bEnable);      // ������־
    static DWORD    s_GetKeyFlag(VOID);             // ��ȡ�������

// ˽��
private:
    static LPDIRECT3D9              m_lpDirect9;        // D3D9����ָ��
    static LPDIRECT3DDEVICE9        m_lpDirectDevice9;  // D3D9�豸ָ��
    static D3DPRESENT_PARAMETERS    m_d3dPP;            // ���ò���
    static BOOL         m_bInit;        // �Ƿ��ʼ��
    static HWND         m_hWnd;         // ���ھ��
    static CMsIni*      m_pMsIni;       // �����ļ�
    static CMsLog*      m_pMsLog;       // ��־����
    static LONG         m_lScreenWidth; // ���ڿ��
    static LONG         m_lScreenHeight;// ���ڸ߶�
    static BOOL         m_bActive;      // �Ƿ񱻼���
    static BOOL         m_bHideCursor;  // �Ƿ����ع��
    static MsLock       m_Lock;         // ������
};

//����������������������
#endif//#pragma once  ��
//����������������������
