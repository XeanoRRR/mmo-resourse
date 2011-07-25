#pragma once                //��//��
#ifndef __MSGDIPLUSIMAGE_H__//��//��
#define __MSGDIPLUSIMAGE_H__//��//��
//������������������������������//��
//�ļ����ƣ�MsGdiPlusImage.h    //��
//����������GDI+ͼƬ            //��
//ƽ̨��أ�WINDOWS             //��
//����������������������������������

class CMsGdiPlusImage
{
public:
    // ���졢����
    CMsGdiPlusImage(VOID);
    virtual ~CMsGdiPlusImage(VOID);

    // ��ʼ��
    virtual BOOL Init(HDC hDc);

    virtual BOOL Init(CMsGdiBuffImage* lpCMsGdiBuffImage);

    // ����ͼƬ
    virtual BOOL Load(TCHAR* szImageFileName);

    // ����ͼƬ
    virtual BOOL Load(HBITMAP hBitmap);

    // ����ͼƬ
    virtual BOOL Create(DWORD dwWidth, DWORD dwHeight);

    // ����ͼƬ
    virtual BOOL Save(TCHAR* szImageFileName);

    // �ر�ͼƬ
    virtual VOID Close(VOID);

    // ��ȡ��DC
    virtual HDC  GetDc(VOID);

    // ������ɫ����
    virtual VOID SetColorMatrices(Gdiplus::ColorMatrix& CMatrix, Gdiplus::ColorMatrix& GMatrix);

    // ���û�������
    virtual VOID ResetImageAttr(VOID);
    
    // ����ͼƬ
    virtual BOOL Draw(RECT& DesRc, INT X = 0, INT Y = 0);

    virtual BOOL Draw(INT X, INT Y, INT srcWidth = INVALID_NID, INT srcHeight = INVALID_NID, INT srcX = 0, INT srcY = 0);

    // ���Ż���ͼƬ
    virtual BOOL ZoomDraw(RECT& DesRc, INT srcWidth, INT srcHeight, INT X, INT Y);

    virtual BOOL ZoomDraw(INT X, INT Y, INT desWidth, INT desHeight, INT srcWidth, INT srcHeight, INT srcX, INT srcY);
protected:

    // ͼƬ����·����
    TCHAR       m_szFullPath[MAX_PATH];
    HANDLE      m_hFile;        // ͼƬ�ļ����
    HDC         m_hDc;          // ��DC
    DWORD       m_dwWidth;      // ���
    DWORD       m_dwHeight;     // �߶�
    MsGraphics* m_pMsGraphics;  // ��������
    MsImage*    m_pMsImage;     // ͼƬ����
    BOOL        m_bInit;        // �Ƿ��Ѿ���ʼ��

    Gdiplus::ImageAttributes m_ImageAttr;   // ��������
    Gdiplus::Rect m_DestRect;   // Ŀ������
};  

//����������������������
#endif//#pragma once//��
//����������������������
