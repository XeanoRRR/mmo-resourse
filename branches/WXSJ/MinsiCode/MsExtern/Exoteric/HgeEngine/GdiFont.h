#ifndef GDIFONT_H
#define GDIFONT_H
#include "Sprites.h"
#include <list>

class GdiFont :
    public FontSprite
{
public:
    GdiFont(CONST TCHAR* lpsFontName, INT nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bAntialias = TRUE);
    ~GdiFont(VOID);

    VOID StaticCacheCharacter(CONST TCHAR* text);

public:
    // �ͷ�һ��GdiFont�������
    virtual VOID    Release();

    virtual VOID    ResetFont( CONST TCHAR* lpsFontName, INT nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bAntialias = TRUE );

    // ��Ⱦ�ı�
    virtual VOID    Printf( FLOAT x, FLOAT y, CONST TCHAR *format, ... );
    virtual VOID    Render( FLOAT x, FLOAT y, CONST TCHAR* text );
    virtual VOID    RenderEx( FLOAT x, FLOAT y, CONST TCHAR* text, FLOAT scale = 1.0f );

    // �������ȡ��ɫ
    virtual VOID    SetColor( DWORD dwColor, INT i = -1 );
    virtual DWORD   GetColor( INT i = 0 );

    // ��ȡ�ı����
    virtual SIZE    GetTextSize( CONST TCHAR* text );

    // ���������ȡ�ַ�
    virtual TCHAR   GetCharacterFromPos( CONST TCHAR* text, FLOAT pixel_x, FLOAT pixel_y );

    // �����ּ��
    virtual VOID    SetKerningWidth( FLOAT kerning );
    virtual VOID    SetKerningHeight( FLOAT kerning );

    // ��ȡ�ּ��
    virtual FLOAT   GetKerningWidth();
    virtual FLOAT   GetKerningHeight();

    // �����С
    virtual FLOAT   GetFontSize();

private:
    // �����ַ���ȡ����
    UINT            GetGlyphByCharacter( TCHAR c );
    inline FLOAT    GetWidthFromCharacter( TCHAR c );
    inline VOID     CacheCharacter(UINT idx, TCHAR c);

    typedef struct tagEngineFontGlyph
    {
        FLOAT       x;
        FLOAT       y;
        FLOAT       w;
        FLOAT       h;
        HTEXTURE    t;
        TCHAR       c;
    }TENGINEFONTGLYPH;

    static CONST UINT   font_count = 0xFFFF;// = sizeof(TCHAR);
    static CONST INT    tex_size = 256;

    std::list<HTEXTURE> m_FontTextures;
    TENGINEFONTGLYPH    m_FontGlyphs[font_count];
    FLOAT               m_nFontSize;
    FLOAT               m_nKerningWidth;
    FLOAT               m_nKerningHeight;
    BOOL                m_bItalic;

    HGE*                m_pHGE;
    hgeSprite*          m_pSprite;

    // GDI�豸
    HDC                 m_hMemDC;
    HBRUSH              m_hBrush;
    HFONT               m_hFont;
    HBITMAP             m_hBitmap;
    VOID*               m_pBuffer;

    // ��ʱ������ģ������Ϣ
    HTEXTURE            m_hTexLetter;
    POINT               m_ptLetter;
};
#endif//GDIFONT_H
