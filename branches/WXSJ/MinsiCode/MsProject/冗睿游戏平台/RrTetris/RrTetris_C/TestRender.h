#pragma once

class CTestRender
    : public CMs2DRender_DX9
{
public:
    CTestRender(
        HWND hWnd,
        DWORD dwWidth,
        DWORD dwHeight
        );
    ~CTestRender(VOID);
    virtual VOID Draw(VOID);                // ����

private:
    LPCMs2DTexture          m_lpTexture;    // ͼƬ

    // ��Ӱ��Ⱦ��ɫ����
    Gdiplus::ColorMatrix    m_ShadowColorBate;

      // ��Ӱ��Ⱦ�ҶȾ���
    Gdiplus::ColorMatrix    m_ShadowGrayness;

};