#pragma once                    //��//��
#ifndef __GAMETREECAPTION_H__   //��//��
#define __GAMETREECAPTION_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�GameTreeCaption.h       //��
//������������Ϸ������              //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ��Ϸ������
class CGameTreeCaption
{
public:
    CGameTreeCaption(CMsGdiBuffImage* pBuffImage, HFONT hFont = NULL);
    ~CGameTreeCaption(VOID);

    BOOL SetFont(INT nSize, DWORD dwWeight, BOOL bItalic, BOOL bUnderline, TCHAR* lpszFontName);
    BOOL PrintText(RECT* lpRc, TCHAR* szText, INT nTextLen, COLORREF cColor, DWORD dwFormat);

private:

    HFONT               m_hFont;
    CMsGdiBuffImage*    m_pBuffImage;
};

//����������������������
#endif//#pragma once  ��
//����������������������
