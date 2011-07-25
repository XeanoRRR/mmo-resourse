#pragma once                //��//��
#ifndef __MS2DRENDER_DX9_H__//��//��
#define __MS2DRENDER_DX9_H__//��//��
//������������������������������//��
//�ļ����ƣ�Ms2DRender_DX9.h    //��
//����������2D��Ϸ����DX9��Ⱦ   //��
//ƽ̨��أ�Windows             //��
//����������������������������������

class CMs2DRender_DX9
    :public CMsThread
{
public: // ���졢����
    CMs2DRender_DX9(
        HWND hWnd,
        DWORD dwScreenWidth,
        DWORD dwScreenHeight
        );
    ~CMs2DRender_DX9(VOID);

// �ӿ�
public:
    BOOL Render(VOID);                      // ��Ⱦ
    virtual VOID Draw(VOID) = 0;            // ����
    DWORD GetFrameCount(FLOAT);             // ��ȡ֡��
    DOUBLE GetFrameCount(DWORD);            // ��ȡ֡��

	// ����
	INT DrawFont(ID3DXFont* lpD3DFrameFont, TCHAR* szText, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color);

    // ����Ⱦ֡��
    VOID DisableFrame(VOID);

    // ����Ⱦ֡��
    VOID EnableFrame(
        ID3DXFont* lpD3DFrameFont,
        BOOL bEven,
        RECT& rc,
        TCHAR* szFormatEven = _T("֡:%dÿ��\n"),
        TCHAR* szFormat = _T("֡:%4.3lfÿ��\n֡:%dÿ��\n��:%4.3lf����\n"),
        D3DCOLOR D3DColor = ARGB(255, 255, 0, 0)
        );

// ˽��
protected:
    virtual BOOL ForeStop(VOID);        // ֹ֮ͣǰ
    virtual BOOL ThreadCallBack(VOID);  // ��Ⱦ�߳�
    VOID UpdataFrame(VOID);             // ����֡��

    LPDIRECT3DDEVICE9   m_lpDirectDevice9;  // D3D9�豸ָ��
    BOOL                m_bInit;            // �Ƿ��ѳ�ʼ��
    CMsIni*             m_pMsIni;           // �����ļ�
    CMsLog*             m_pMsLog;           // ��־����
    BOOL                m_bQuit;            // �Ƿ��˳�
    HWND                m_hWnd;
    DWORD               m_dwScreenWidth;
    DWORD               m_dwScreenHeight;

    ID3DXFont*          m_lpD3DFrameFont;   // ֡����ʾ����
    DOUBLE              m_lfConsumeTime;    // ��Ⱦ��ʱ
    RECT                m_FrameRect;        // ֡����ʾλ��
    BOOL                m_bRenderFrame;     // �Ƿ���Ⱦ֡��
    BOOL                m_bEven;            // �Ƿ�ƽ����ʾ
    TCHAR               m_szFrameBuff[MAX_PATH];
    TCHAR               m_szFormatFrame[MAX_PATH];
    D3DCOLOR            m_D3DColor;         // ֡����ʾ��ɫ

    INT64               m_n64TimeEvenA;     // ƽ����Ⱦ��ʱA
    INT64               m_n64TimeEvenB;     // ƽ����Ⱦ��ʱB
    DWORD               m_dwFrameEvenCount; // ƽ��֡��

    INT64               m_n64TimeA;         // ��Ⱦ��ʱA
    INT64               m_n64TimeB;         // ��Ⱦ��ʱB
    DWORD               m_dwFrameCount;     // ͬһʱ����Ⱦ֡��
    DWORD               m_dwRenderFrame;    // ��Ⱦ֡��
    DOUBLE              m_lfRenderFrame;    // ��Ⱦ֡��
};

//����������������������
#endif//#pragma once  ��
//����������������������
