#include "Precompiled.h"

// ���졢����
CMsPngImage::CMsPngImage(VOID)
: m_lppPngBuff      (NULL)
, m_lpBitData       (NULL)
, m_dwPngWidth      (0)
, m_dwPngHeight     (0)
, m_hFile           (NULL)
, m_PngReadPtr      (NULL)
, m_PngWritePtr     (NULL)
, m_PngReadInfoPtr  (NULL)
, m_PngWriteInfoPtr (NULL)
{
    ZeroMemoryArray(m_szFullPath);

    // ������ȡ�ṹ��
    m_PngReadPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    AssertEx(m_PngReadPtr, _T("����PNG��ȡ�ṹ��ʧ��"));

    // ������ȡ��Ϣ�ṹ��
    m_PngReadInfoPtr = png_create_info_struct(m_PngReadPtr);
    AssertEx(m_PngReadInfoPtr, _T("����PNG��ȡ��Ϣ�ṹ��ʧ��"));

    // ����д��ṹ��
    m_PngWritePtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    AssertEx(m_PngWritePtr, _T("����PNGд��ṹ��ʧ��"));

    // ����д����Ϣ�ṹ��
    m_PngWriteInfoPtr = png_create_info_struct(m_PngWritePtr);
    AssertEx(m_PngWriteInfoPtr, _T("����PNGд����Ϣ�ṹ��ʧ��"));

    // ����PNG ��ȡ�ص�����
    png_set_read_fn(m_PngReadPtr, this, ReadPng);
    png_set_write_fn(m_PngWritePtr, this, WritePng, FlushPng);
}

CMsPngImage::~CMsPngImage(VOID)
{
    // �ر�
    this->Close();

    ::DeleteObject(m_hBitmap);

    // ���ٶ�ȡ��Ϣ�ṹ��
    png_destroy_info_struct(m_PngReadPtr, &m_PngReadInfoPtr);

    // ����д����Ϣ�ṹ��
    png_destroy_info_struct(m_PngWritePtr, &m_PngWriteInfoPtr);

    // ���ٶ�ȡ�ṹ��
    png_destroy_read_struct(&m_PngReadPtr, NULL, NULL);

    // ����д��ṹ��
    png_destroy_write_struct(&m_PngWritePtr, NULL);
}

// ��ͼƬ
BOOL CMsPngImage::Open(TCHAR* szImageFileName)
{
    BOOL bRet = CreateFullPath(m_szFullPath, szImageFileName);
    AssertEx(bRet, _T("·����������"));

    // ��ͼƬ�ļ�
    m_hFile = ::CreateFile(
        m_szFullPath,
        FILE_READ_DATA,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
        );
    AssertEx(m_hFile, _T("��ͼƬʧ��"));
    return TRUE;
}

// PNG ����ص�
VOID PNGAPI CMsPngImage::ErrorPng(png_structp PngPtr, png_const_charp ErrorMsg)
{
    USE_PARAM(PngPtr);
    AssertExA(FALSE, ErrorMsg);
}

// PNG ��ȡ�ص�
VOID PNGAPI CMsPngImage::ReadPng(png_structp png_ptr, png_bytep data, png_size_t length)
{
    CMsPngImage* pPngImage = (CMsPngImage*)png_get_io_ptr(png_ptr);

    if (pPngImage->m_hFile)
    {
        DWORD dwReadSize;
        BOOL bRet = ::ReadFile(pPngImage->m_hFile, data, length, &dwReadSize, NULL);
        AssertEx(bRet, _T("��ȡͼƬʧ��"));
    }
    else
    {
        AssertEx(pPngImage->m_hFile, _T("��ȡͼƬʧ��"));
    }
}

VOID PNGAPI CMsPngImage::WritePng(png_structp png_ptr, png_bytep data, png_size_t length)
{
    CMsPngImage* pPngImage = (CMsPngImage*)png_get_io_ptr(png_ptr);

    if (pPngImage->m_hFile)
    {
        DWORD dwWriteSize;
        BOOL bRet = ::WriteFile(pPngImage->m_hFile, data, length, &dwWriteSize, NULL);
        AssertEx(bRet, _T("д��ͼƬʧ��"));
    }
    else
    {
        AssertEx(pPngImage->m_hFile, _T("д��ͼƬʧ��"));
    }
}
VOID PNGAPI CMsPngImage::FlushPng(png_structp png_ptr)
{
    CMsPngImage* pPngImage = (CMsPngImage*)png_get_io_ptr(png_ptr);

    if (pPngImage->m_hFile)
    {
        BOOL bRet = ::FlushFileBuffers(pPngImage->m_hFile);
        AssertEx(bRet, _T("д��ͼƬʧ��"));
    }
    else
    {
        AssertEx(pPngImage->m_hFile, _T("д��ͼƬʧ��"));
    }
}


// ����ͼƬ
BOOL CMsPngImage::Decode(VOID)
{
    png_read_info(m_PngReadPtr, m_PngReadInfoPtr);
    m_dwPngWidth = m_PngReadInfoPtr->width;
    m_dwPngHeight = m_PngReadInfoPtr->height;
    m_lppPngBuff = NEW LPBYTE[m_dwPngHeight];
    m_lpBitData = NEW BYTE[m_dwPngHeight * m_dwPngWidth * m_PngReadInfoPtr->channels];
    if (m_lppPngBuff)
    {
        FOR_x_TO(i, m_dwPngHeight)
        {
            m_lppPngBuff[i] = &m_lpBitData[i * m_dwPngWidth];
        }
    }
    else
    {
        AssertEx(m_lppPngBuff, _T("�ڴ治��"));
    }
    png_read_rows(m_PngReadPtr, m_lppPngBuff, NULL, m_dwPngHeight);
    INT a = 24;
    INT b = 1;
    m_hBitmap = ::CreateBitmap(m_dwPngWidth, m_dwPngHeight, a, b, m_lpBitData);
    //m_hBitmap = (HBITMAP)::LoadImage(g_pCMsWinMain->GetHInstance(), _T("D:/MinsiWork/Debug/MsRes/Project/Exe_�����Ϸƽ̨/RrRes/Image/00.bmp"), IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
    AssertEx(m_hBitmap, _T("ͼƬת��ʧ��"));
    ::CloseHandle(m_hFile);
    m_hFile = NULL;

    return TRUE;
}

// ����ͼƬ
BOOL CMsPngImage::Encode(VOID)
{

    return TRUE;
}

// ����ͼƬ
BOOL CMsPngImage::Save(TCHAR* szImageFileName)
{
    USE_PARAM(szImageFileName);

    return TRUE;
}

// �ر�ͼƬ
VOID CMsPngImage::Close(VOID)
{
    SAFE_DELETE_ARRAY(m_lppPngBuff);
    SAFE_DELETE_ARRAY(m_lpBitData);
    m_dwPngWidth = 0;
    m_dwPngHeight = 0;
    ZeroMemoryArray(m_szFullPath);
}

// ����ͼƬ
BOOL CMsPngImage::Draw(HDC hDc, INT x, INT y, RECT DesRc)
{
    HDC TmpDC = ::CreateCompatibleDC(hDc);
    ::SelectObject(TmpDC, m_hBitmap);
    ::BitBlt(hDc, DesRc.left, DesRc.top, (DesRc.right - DesRc.left), (DesRc.bottom - DesRc.top), TmpDC, x, y, SRCCOPY);
    return ::DeleteDC(TmpDC);
}