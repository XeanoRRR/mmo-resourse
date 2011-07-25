#include "Precompiled.h"
#include "MsImageManager.h"

CMsImageManager::CMsImageManager(VOID)
: m_pSubImage           (NULL)
, m_dwSubImageSize      (INVALID_UID)
, m_dwSubImageCount     (INVALID_UID)
, m_lpPointArray        (NULL)
, m_eImageType          (CXIMAGE_FORMAT_UNKNOWN)
, m_MsLog               (_T("CMsImageManager.log"), FALSE)
{
    ;
}

CMsImageManager::~CMsImageManager(VOID)
{
    this->CloseAllImage();
}

// ��ͼƬ
BOOL CMsImageManager::OpenImageFile(TCHAR* szImageFileName)
{
    BOOL bRet = m_MainImage.Load(szImageFileName, CXIMAGE_SUPPORT_BMP);
    if (!bRet)
    {
        m_MsLog << _T("��ͼƬʧ��") << L_NEND;
    }
    return bRet;
}

// ����ͼƬ
BOOL CMsImageManager::SaveImageFile(TCHAR* szImageFileName)
{
    AssertEx(szImageFileName, _T("����·������Ϊ��"));
    if (CXIMAGE_FORMAT_UNKNOWN == m_eImageType)
    {
        return FALSE;   // �������Ͳ�ȷ��
    }
    BOOL bRet = m_MainImage.Save(szImageFileName, m_eImageType);
    if (bRet)
    {
        m_MsLog
            << _T("����ͼƬ[") 
            << szImageFileName 
            << _T("]ʧ��") 
            << L_NEND;
    }
    return bRet;
}

// ����ͼƬ����
VOID CMsImageManager::SetImageFileType(E_Image_Type eImageType)
{
    m_eImageType = eImageType;
}

// ���ͼƬ
DWORD CMsImageManager::SplitImageFile(DWORD dwSubImageSize)
{
    _ENTER_FUN_C;

    m_dwSubImageSize = dwSubImageSize;

    if (m_dwSubImageSize < SUB_MAP_LEAST_SIZE)
    {
        return INVALID_UID;  // ��ͼƬ�ߴ粻������� SUB_MAP_LEAST_SIZE ��
    }

    if (0 == m_dwSubImageSize)
    {
        m_dwSubImageSize = 1;
    }

    // ��ȡ�ָ�������ͼƬ����
    m_dwSubImageCountByX = m_MainImage.GetWidth() / m_dwSubImageSize + 1;

    // ��ȡ�ָ��������ͼƬ����
    m_dwSubImageCountByY = m_MainImage.GetHeight() / m_dwSubImageSize + 1;

    // ������ͼƬ�ܸ���
    m_dwSubImageCount = m_dwSubImageCountByX * m_dwSubImageCountByY;

    // ����������һ����ͼƬ��Ҫ��ʾ�Ŀ��
    DWORD dwLastSubImageWidthByX = SUB_MAP_LEAST_SIZE - 
    (m_dwSubImageCountByX * SUB_MAP_LEAST_SIZE - m_MainImage.GetWidth());

    // �����������һ����ͼƬ��Ҫ��ʾ�ĸ߶�
    DWORD dwLastSubImageHeightByY = SUB_MAP_LEAST_SIZE - 
    (m_dwSubImageCountByY * SUB_MAP_LEAST_SIZE - m_MainImage.GetHeight());

    // ������ͼƬ
    m_pSubImage = NEW CxImage[m_dwSubImageCount];
    AssertEx(m_pSubImage, _T("�ڴ治��"));

    // ������ͼƬƫ������
    m_lpPointArray = NEW POINT[m_dwSubImageCount];
    AssertEx(m_lpPointArray, _T("�ڴ治��"));

    // ��ȡͼƬ��ɫ���
    DWORD dwImageBpp    = m_MainImage.GetBpp();
    DWORD dwImageType   = m_MainImage.GetType();

    // ��ȡ
    FOR_x_TO(dwIndex, m_dwSubImageCount)
    {
        DWORD dwCurrentSubImageWidth = m_dwSubImageSize;
        DWORD dwCurrentSubImageHeight = m_dwSubImageSize;

        // ��������һ����ͼƬ
        if ((dwIndex+1) == 
            m_dwSubImageCount)
        {
            // ������ͼƬ
            if (m_pSubImage)
            {
                m_pSubImage[dwIndex].Create(dwLastSubImageWidthByX,
                    dwLastSubImageHeightByY,
                    dwImageBpp,
                    dwImageType
                    );

                dwCurrentSubImageWidth = dwLastSubImageWidthByX;
                dwCurrentSubImageHeight = dwLastSubImageHeightByY;
            }
            else
            {
                AssertEx(m_pSubImage, _T("���ô���"));
            }
        }

        // ����Ǻ������һ����ͼƬ
        else if (0 != dwIndex &&
            0 == (dwIndex+1)%(m_dwSubImageCountByX))
        {
            // ������ͼƬ
            m_pSubImage[dwIndex].Create(dwLastSubImageWidthByX,
                m_dwSubImageSize,
                dwImageBpp,
                dwImageType
                );

            dwCurrentSubImageWidth = dwLastSubImageWidthByX;
        }

        // ������������һ����ͼƬ
        else if ((dwIndex+1) > 
            ((m_dwSubImageCountByY - 1) * m_dwSubImageCountByX))
        {
            // ������ͼƬ
            m_pSubImage[dwIndex].Create(m_dwSubImageSize,
                dwLastSubImageHeightByY,
                dwImageBpp,
                dwImageType
                );

            dwCurrentSubImageHeight = dwLastSubImageHeightByY;
        }

        // ������ͼƬ
        else
        {
            // ������ͼƬ
            if (m_pSubImage)
            {
                m_pSubImage[dwIndex].Create(m_dwSubImageSize,
                    m_dwSubImageSize,
                    dwImageBpp,
                    dwImageType
                    );

                RECT  rc = {0,0,0,0};
                rc.left     = (dwIndex%m_dwSubImageCountByX)*m_dwSubImageSize;
                rc.top      = (dwIndex/m_dwSubImageCountByX)*m_dwSubImageSize;
                rc.right    = rc.left + dwCurrentSubImageWidth;
                rc.bottom   = rc.top  + dwCurrentSubImageHeight;
                this->CopyImageInMainImageRect(&m_pSubImage[dwIndex],
                    &rc,
                    0,
                    0,
                    FALSE
                    );
            }
            else
            {
                AssertEx(m_pSubImage, _T("���ô���"));
            }
        }
    }
    // ������ͼƬ����
    return m_dwSubImageCount;

    _LEAVE_FUN_DISPOSE_END_C;
    return INVALID_UID;
}

// �ر�����ͼƬ
VOID CMsImageManager::CloseAllImage(VOID)
{
    // �ͷ���ͼƬ����
    SAFE_DELETE_ARRAY(m_pSubImage);

    // �ͷ�ƫ������
    SAFE_DELETE_ARRAY(m_lpPointArray);
}

// ����ͼƬ
BOOL CMsImageManager::DrawMainImage(
    HDC hDc,
    LONG X,
    LONG Y,
    FLOAT fZoom
    )
{
    // ����ͼƬ
    FLOAT fDrawCX = fZoom * m_MainImage.GetWidth();
    FLOAT fDrawCY = fZoom * m_MainImage.GetHeight();
    INT nRet = m_MainImage.Draw(hDc, X, Y, (LONG)fDrawCX, (LONG)fDrawCY);
    if (nRet)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// ������������ͼƬ
BOOL CMsImageManager::DrawSubImage(
    HDC hDc,
    DWORD dwIndex,
    LONG X,
    LONG Y,
    FLOAT fZoom
    )
{
    _ENTER_FUN_C;

    AssertEx(dwIndex < m_dwSubImageCount, _T("ָ������ͼƬ������"));

    if (m_pSubImage)
    {
        FLOAT fDrawCX = fZoom * m_pSubImage[dwIndex].GetWidth();
        FLOAT fDrawCY = fZoom * m_pSubImage[dwIndex].GetHeight();

        FLOAT fRealDrawX = 
            (dwIndex % m_dwSubImageCountByX) * m_dwSubImageSize * fZoom;

        FLOAT fRealDrawY = 
            (dwIndex / m_dwSubImageCountByX) * m_dwSubImageSize * fZoom;

        INT nRet = m_pSubImage[dwIndex].Draw(
            hDc,
            X+(LONG)fRealDrawX,
            Y+(LONG)fRealDrawY,
            (LONG)fDrawCX,
            (LONG)fDrawCY
            );

        if (nRet)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        AssertEx(m_pSubImage, _T("���ô���"));
    }

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ������ͼƬָ������Ŀ��ͼƬָ��λ��
BOOL CMsImageManager::CopyImageInMainImageRect(
    CxImage* pDesImage,
    LPRECT lpSrcRc,
    LONG DesX,
    LONG DesY,
    BOOL bCopyAlpha
    )
{
    if (!pDesImage)
    {
        return FALSE;
    }
    //DWORD dwMainImageWidth =
    //    m_MainImage.GetWidth();        // ��ͼƬ���

    DWORD dwMainImageHeight =
        m_MainImage.GetHeight();      // ��ͼƬ�߶�

    DWORD dwSrcBeginX =
        lpSrcRc->left;                      // ת����ͼƬ��ʼλ��X

    DWORD dwSrcBeginY =
        dwMainImageHeight - lpSrcRc->bottom;// ת����ͼƬ��ʼλ��Y

    DWORD dwCopyWidth =
        lpSrcRc->right - lpSrcRc->left;     // ���ƿ��

    DWORD dwCopyHeight =
        lpSrcRc->bottom - lpSrcRc->top;    // ���Ƹ߶�

    RGBQUAD rgba;
    FOR_x_TO(x, dwCopyWidth)
    {
        for (DWORD y = 0; y < dwCopyHeight; y++)
        {
            rgba = m_MainImage.GetPixelColor(
                dwSrcBeginX+x,
                dwSrcBeginY+y,
                bCopyAlpha?true:false
                );

            pDesImage->SetPixelColor(
                DesX+x,
                DesY+y,
                rgba,
                bCopyAlpha?true:false
                );
        }
    }
    return TRUE;
}

// ��ȡ��ͼƬ�ĳߴ�
VOID CMsImageManager::GetMainImageSize(
    DWORD& dwWidth,
    DWORD& dwHeight
    )
{
    dwWidth = m_MainImage.GetWidth();
    dwHeight = m_MainImage.GetHeight();
}
