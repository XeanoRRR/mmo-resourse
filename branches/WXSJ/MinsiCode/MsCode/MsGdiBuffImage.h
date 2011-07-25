#pragma once                //��//��
#ifndef __MSGDIBUFFIMAGE_H__//��//��
#define __MSGDIBUFFIMAGE_H__//��//��
//������������������������������//��
//�ļ����ƣ�MsGdiBuffImage.h    //��
//����������GDI+����ͼ          //��
//ƽ̨��أ�WINDOWS             //��
//����������������������������������

class CMsGdiBuffImage
{
public:
    // ���졢����
    CMsGdiBuffImage(VOID);
    virtual ~CMsGdiBuffImage(VOID);

    // ��ʼ��
    virtual BOOL Init(HWND hWnd, DWORD dwWidth, DWORD dwHeight, COLORREF cBackColor = RGB(0,0,0), BOOL bTransparent = TRUE);

    // ����ͼƬ
    virtual BOOL Save(TCHAR* szImageFileName);

    // �ر�ͼƬ
    virtual VOID Close(VOID);

    // ��ȡ���豸
    virtual HDC GetMainDc(VOID);

    // ��ȡ�����豸
    virtual HDC GetBuffDc(VOID);

    // �黹DC
    virtual VOID RestoreBuffDc(VOID);

    // ���ñ���ɫ
    virtual VOID SetBackColor(COLORREF cBackColor = RGB(0,0,0));

    // ׼������
    virtual BOOL BeginDraw(BOOL bErasure = TRUE);

    // ��������
    virtual BOOL PrintText(HFONT hFont, RECT* lpRc, TCHAR* szText, COLORREF cColor, INT nTextLen, DWORD dwFormat);

    // ����ͼƬ
    virtual BOOL EndDraw(INT nDrawX = 0, INT nDrawY = 0, DWORD dwRop = SRCCOPY);
    virtual BOOL EndDraw(HDC hDc, INT nDrawX = 0, INT nDrawY = 0, DWORD dwRop = SRCCOPY);
private:

    // ��DC�豸
    HDC         m_hMainDc;

    // �����ھ��
    HWND        m_hMainWnd;

    // ����ͼ���
    HBITMAP     m_hBuffBitmap;

    // ����ͼ���
    DWORD       m_dwWidth;
    DWORD       m_dwHeight;

    // �����豸
    HDC         m_hBuffDc;

    // �Ƿ��Ѿ���ʼ��
    BOOL        m_bInit;

    // ������ɫ
    COLORREF    m_cBackColor;
    RECT        m_Rc;
    HBRUSH      m_hBrush;

    // ����DC������ֵ
    INT         m_nDcIndex;
};  

//����������������������
#endif//#pragma once//��
//����������������������
