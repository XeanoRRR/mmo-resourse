#pragma once                //��//��
#ifndef __MSPNGIMAGE_H__    //��//��
#define __MSPNGIMAGE_H__    //��//��
//������������������������������//��
//�ļ����ƣ�MsPngImage.h        //��
//����������PNGͼƬ             //��
//ƽ̨��أ�WINDOWS             //��
//����������������������������������
#include "../MsExtern/Exoteric/Png/png.h"
class CMsPngImage
    : public CMsBaseImage
{
public:
    // ���졢����
    CMsPngImage(VOID);
    virtual ~CMsPngImage(VOID);

    // ����ͼƬ
    //virtual BOOL Create(VOID);

    // ��ͼƬ
    virtual BOOL Open(TCHAR* szImageFileName);

    // ����ͼƬ
    virtual BOOL Decode(VOID);

    // ����ͼƬ
    virtual BOOL Encode(VOID);

    // ����ͼƬ
    virtual BOOL Save(TCHAR* szImageFileName);

    // �ر�ͼƬ
    virtual VOID Close(VOID);

    // ����ͼƬ
    virtual BOOL Draw(HDC hDc, INT x, INT y, RECT DesRc);


    HBITMAP GetBitmap(VOID){return m_hBitmap;};
private:

    // PNG ��ȡ�ص�
    static VOID PNGAPI ErrorPng(png_structp PngPtr, png_const_charp ErrorMsg);
    static VOID PNGAPI ReadPng(png_structp PngPtr, png_bytep lpData, png_size_t Length);
    static VOID PNGAPI WritePng(png_structp PngPtr, png_bytep lpData, png_size_t Length);
    static VOID PNGAPI FlushPng(png_structp PngPtr);


    // ͼƬ����·����
    TCHAR   m_szFullPath[MAX_PATH];
    HANDLE  m_hFile;        // ͼƬ�ļ����
    LPBYTE* m_lppPngBuff;   // ͼƬ����
    LPBYTE  m_lpBitData;    // ͼƬ����
    DWORD   m_dwPngWidth;   // ͼƬ���
    DWORD   m_dwPngHeight;  // ͼƬ�߶�

    HBITMAP m_hBitmap;      // λͼ���

    // PNG ��ȡ�ṹ��ָ��
    png_struct* m_PngReadPtr;

    // PNG ��ȡ��Ϣ�ṹ��ָ��
    png_info*   m_PngReadInfoPtr;

    // PNG д��ṹ��ָ��
    png_struct* m_PngWritePtr;

    // PNG ��ȡ��Ϣ�ṹ��ָ��
    png_info*   m_PngWriteInfoPtr;

    // λͼ��Ϣ
    BITMAPINFO  m_BitmapInfo;

    // λͼ��Ϣͷ��
    BITMAPINFOHEADER    m_BitmapInfoHeader;
};

//����������������������
#endif//#pragma once//��
//����������������������
