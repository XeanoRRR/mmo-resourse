#pragma once

// �����ڱ���ͼ�Ͽ�ʼ���Ƶ�λ��ƫ��
#define DRAW_BACK_SCENE_POS_X 176
#define DRAW_BACK_SCENE_POS_Y 139

class CRender
    : public CMs2DRender_DX9
{
public:
    CRender(
        HWND hWnd,
        DWORD dwScreenWidth,
        DWORD dwScreenHeight,
        CT_Scene* pScene,
        CT_Square* pSquare);
    ~CRender(VOID);

    virtual VOID Draw(VOID);                            // ����
    BOOL DrawBackScene(VOID);                           // ���Ʊ���
    VOID ResetBackScene(VOID);                          // ���ñ���
    BOOL DrawActivityShape(BOOL IsDrawShadow);          // ����ǰ��
    BOOL DrawShapeShadow(UPOS Shape_X, UPOS Shape_Y);   // ������Ӱ
    BOOL DrawNextShape(VOID);                           // ������һ������
    BOOL DrawText(VOID);                                // �����ı���Ϣ
    BOOL DrawCenterText(TCHAR* szText, RECT* rcIntegral,  COLORREF cColor);// ���л����ı���Ϣ

private:
	// ������Ҫʹ�õ�ͼԪ��Ϣ
	CMs2DTexture* AccountMetafileInfo(DWORD dwCurrentMetafileIndex, UPOS& uMetafilePosX, UPOS& uMetafilePosY);
    CMs2DTexture*       m_pTextureBack;         // ����ͼƬ
	CMs2DTexture*       m_pTextureMetafile_0;   // ͼԪͼƬ
	CMs2DTexture*       m_pTextureMetafile_1;   // ͼԪͼƬ
	CMs2DTexture*       m_pTextureMetafile_2;   // ͼԪͼƬ
	CMs2DTexture*       m_pTextureMetafile_3;   // ͼԪͼƬ
    ID3DXFont*          m_lpD3DIntegralFont;    // ��������
    ID3DXFont*          m_lpD3DLevelFont;       // �ȼ�����
    CT_Scene*           m_pScene;               // ������ָ��
    CT_Square*          m_pSquare;              // ��ǰ�����ָ��
    BOOL                m_IsShowShadow;         // �Ƿ���ʾ��Ӱ

    // ��Ӱ��Ⱦ��ɫ����
    Gdiplus::ColorMatrix m_ShadowColorBate;

      // ��Ӱ��Ⱦ�ҶȾ���
    Gdiplus::ColorMatrix m_ShadowGrayness;

};
