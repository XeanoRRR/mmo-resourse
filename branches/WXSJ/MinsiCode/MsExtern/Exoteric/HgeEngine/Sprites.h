//////////////////////////////////////////////////////////////////////////////////////
// �������
//////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "hgesprite.h"

//////////////////////////////////////////////////////////////////
// ��������
#define DELETE_SELF(p)                { if(p) { delete p; } };
#define DELETE_OBJECT(p)            { if(p) { delete p; p=NULL; } };
#define DELETE_OBJECTARRAY(p)        { if(p) { delete [] p; p=NULL; } };
#define RELEASE_OBJECT(p)            { if(p) { p->Release(); p=NULL; } };

__inline FLOAT _floor(FLOAT f)
{
    static INT _n;
    //_asm fld f
    //_asm fistp _n
    return (FLOAT)_n;
}

//////////////////////////////////////////////////////////////////////////////////////
// ���徫��
//////////////////////////////////////////////////////////////////////////////////////

class FontSprite
{
public:
    // ��ȡ��������ָ��
    // CONST CHAR* lpsFontName    - ��������
    // INT nFaceSize            - �����С
    // BOOL bBold                - �Ƿ����
    // BOOL bItalic                - �Ƿ�б��
    // BOOL bUnderline            - �Ƿ����»���
    static FontSprite* Create(CONST TCHAR* lpsFontName, INT nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bAntialias = TRUE);

    // ��ȡ��������ָ��
    // CONST CHAR* lpsTTFile    - ʸ������·���ļ���
    // FLOAT nFontSize            - ʸ�������С
    static FontSprite* CreateEx(CONST TCHAR* lpsTTFile, FLOAT nFontSize);

    // ��������
    virtual VOID    Release() = 0;

    // ��Ⱦ�ı�
    virtual VOID    Printf( FLOAT x, FLOAT y, CONST TCHAR *format, ... ) = 0;
    virtual VOID    Render( FLOAT x, FLOAT y, CONST TCHAR* text ) = 0;
    virtual VOID    RenderEx( FLOAT x, FLOAT y, CONST TCHAR* text, FLOAT scale = 1.0f ) = 0;

    // �������ȡ��ɫ
    virtual VOID    SetColor( DWORD dwColor, INT i = -1 ) = 0;
    virtual DWORD    GetColor( INT i = 0 ) = 0;

    // ��ȡ�ı������С
    virtual SIZE    GetTextSize( CONST TCHAR* text ) = 0;

    // ������������ȡ�ַ�
    virtual TCHAR    GetCharacterFromPos( CONST TCHAR* text, FLOAT pixel_x, FLOAT pixel_y ) = 0;

    // �����ּ��
    virtual VOID    SetKerningWidth( FLOAT kerning ) = 0;
    virtual VOID    SetKerningHeight( FLOAT kerning ) = 0;

    // ��ȡ�ּ��
    virtual FLOAT    GetKerningWidth() = 0;
    virtual FLOAT    GetKerningHeight() = 0;

    // ��ȡ�����С
    virtual FLOAT    GetFontSize() = 0;

protected:
    // ��ʽ�ó����౻new
    virtual ~FontSprite(VOID) { };

};//class FontSprite
