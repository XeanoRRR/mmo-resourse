#include "Precompiled.h"
#include "Ms2DImageDivide.h"
#pragma message("������������������������δ��ɩ�������������������������")

#include "MsString.h"
CMs2DImageDivide::CMs2DImageDivide(VOID)
{
    m_pMainImage = NEW CxImage;

}

CMs2DImageDivide::~CMs2DImageDivide(VOID)
{
    SAFE_DELETE_ARRAY(m_pSubImage);
    SAFE_DELETE(m_pMainImage);
}

// ��ͼƬ�ļ�
BOOL CMs2DImageDivide::OpenImage(TCHAR* szImageFilePathName)
{
    return m_pMainImage->Load(szImageFilePathName);
}

// �ر�������Դ
BOOL CMs2DImageDivide::CloseAll(VOID)
{
    BOOL bRet = TRUE;
    if (m_pSubImage)
    {
        FOR_x_TO(i, m_dwSubImageCount)
        {
            bRet = m_pSubImage[i].Destroy();
            if (!bRet)
            {
                return bRet;
            }
        }
    }
    if (m_pMainImage)
    {
        bRet = m_pMainImage->Destroy();
    }
    return bRet;
}

// ������ͼƬ�ߴ�
BOOL CMs2DImageDivide::SetSubImageSize(NSIZE /*nWidth*/, NSIZE /*nHeight*/)
{
    return TRUE;

}

// ���зָ�
BOOL CMs2DImageDivide::Divide(BOOL)
{
    return TRUE;
}

// ����ͼƬ���������
BOOL CMs2DImageDivide::SaveSubImagePack(TCHAR* /*szPackFilePathName*/)
{
    return TRUE;
}