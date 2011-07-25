#pragma once                    //��    //��
#ifndef __MSIMAGEMANAGER_H__    //��    //��
#define __MSIMAGEMANAGER_H__    //��    //��
//����������������������������������    //��
//�ļ����ƣ�MsImageManager.h            //��
//����������ͼƬ������                  //��
//ƽ̨��أ�WINDOWS                     //��
//������������������������������������������

// ��ͼƬ��С�ߴ�
#define SUB_MAP_LEAST_SIZE  100

class CMsImageManager
{
public:

    // ���졢����
    CMsImageManager(VOID);
    virtual ~CMsImageManager(VOID);

    BOOL    OpenImageFile(TCHAR* szImageFileName);      // ��ͼƬ
    BOOL    SaveImageFile(TCHAR* szImageFileName);      // ����ͼƬ
    VOID    SetImageFileType(E_Image_Type eImageType);  // ����ͼƬ����
    DWORD   SplitImageFile(                             // ���ͼƬ
        DWORD dwSubImageSize = SUB_MAP_LEAST_SIZE
        );
    VOID    CloseAllImage(VOID);                        // �ر�����ͼƬ
    DWORD   GetSplitImageSize(VOID)                     // ��ȡ��ͼƬ�ߴ�
    {
        return m_dwSubImageSize;
    }
    
    BOOL DrawMainImage(         // ����ͼƬ
        HDC hDc,
        LONG X,
        LONG Y,
        FLOAT fZoom = 1.0f
        );

    BOOL DrawSubImage(          // ������������ͼƬ
        HDC hDc,
        DWORD dwIndex,
        LONG X,
        LONG Y,
        FLOAT fZoom = 1.0f
        );

    // ������ͼƬָ������Ŀ��ͼƬָ��λ��
    BOOL CopyImageInMainImageRect(
        CxImage* pDesImage,
        LPRECT lpSrcRc,
        LONG DesX,
        LONG DesY,
        BOOL bCopyAlpha
        );

    // ��ȡ��ͼƬ�ĳߴ�
    VOID GetMainImageSize(
        DWORD& dwWidth,
        DWORD& dwHeight);

private:

    CxImage         m_MainImage;            // ��ͼƬ����
    CxImage*        m_pSubImage;            // ��ͼƬ����
    LPPOINT         m_lpPointArray;         // ��ͼƬƫ����������
    DWORD           m_dwSubImageSize;       // ��ͼƬ�ߴ�
    DWORD           m_dwSubImageCountByX;   // ÿ��������ͼƬ����
    DWORD           m_dwSubImageCountByY;   // ÿ��������ͼƬ����
    DWORD           m_dwSubImageCount;      // ��ͼƬ����
    E_Image_Type    m_eImageType;           // ͼƬ����

    CMsLog          m_MsLog;
};



//����������������������
#endif//#pragma once//��
//����������������������
