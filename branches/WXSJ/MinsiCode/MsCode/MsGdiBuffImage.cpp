#include "Precompiled.h"

// ���졢����
CMsGdiBuffImage::CMsGdiBuffImage(VOID)
: m_bInit       (FALSE)
, m_hMainWnd    (NULL)
, m_hMainDc     (NULL)
, m_hBuffDc     (NULL)
, m_nDcIndex    (INVALID_NID)
, m_hBrush      (NULL)
, m_cBackColor  (0)
{
    ;
}

CMsGdiBuffImage::~CMsGdiBuffImage(VOID)
{
    this->Close();
}

// ��ʼ��
BOOL CMsGdiBuffImage::Init(HWND hWnd, DWORD dwWidth, DWORD dwHeight, COLORREF cBackColor, BOOL bTransparent)
{
    if (hWnd && dwWidth && dwHeight)
    {
        m_hMainWnd = hWnd;
        m_hMainDc = ::GetDC(hWnd);
        AssertEx(AL_NORMAL, m_hMainDc, _T("��ȡ����DCʧ��"));

        m_dwWidth = dwWidth;
        m_dwHeight = dwHeight;
        m_Rc.left = 0;
        m_Rc.top  = 0;
        m_Rc.right = m_dwWidth;
        m_Rc.bottom = m_dwHeight;

        this->SetBackColor(cBackColor);

        m_hBuffDc = ::CreateCompatibleDC(m_hMainDc);
        AssertEx(AL_NORMAL, m_hBuffDc, _T("�����豸��ʧ��"));

        if (bTransparent)
        {
            ::SetBkMode(m_hBuffDc, TRANSPARENT);
        }
        else
        {
            ::SetBkMode(m_hBuffDc, OPAQUE);
        }

        m_hBuffBitmap = ::CreateCompatibleBitmap(m_hMainDc, dwWidth, dwHeight);
        AssertEx(AL_NORMAL, m_hBuffBitmap, _T("����ͼ����ʧ��"));

        ::SelectObject(m_hBuffDc, m_hBuffBitmap);

        m_bInit = TRUE;

        return TRUE;
    }
    else
    {
        AssertEx(AL_NORMAL, hWnd, _T("���ڲ���Ϊ��"));
        return FALSE;
    }
}

// ����ͼƬ
BOOL CMsGdiBuffImage::Save(TCHAR* szImageFileName)
{
    USE_PARAM(szImageFileName);
    return TRUE;
}

// �ر�ͼƬ
VOID CMsGdiBuffImage::Close(VOID)
{
    if (m_hBuffBitmap)
    {
        ::DeleteObject(m_hBuffBitmap);
        m_hBuffBitmap = NULL;
    }
    if (m_hBuffDc)
    {
        ::DeleteDC(m_hBuffDc);
        m_hBuffDc = NULL;
    }
    if (m_hBrush)
    {
        ::DeleteObject(m_hBrush);
        m_hBrush = NULL;
    }
}

// ��ȡ���豸
HDC CMsGdiBuffImage::GetMainDc(VOID)
{
    return m_hMainDc;
}

// ��ȡ�����豸
HDC CMsGdiBuffImage::GetBuffDc(VOID)
{
    m_nDcIndex = ::SaveDC(m_hBuffDc);
    return m_hBuffDc;
}

// �黹DC
VOID CMsGdiBuffImage::RestoreBuffDc(VOID)
{
    if (m_nDcIndex != INVALID_NID)
    {
        ::RestoreDC(m_hBuffDc, m_nDcIndex);
    }
}

// ���ñ���ɫ
VOID CMsGdiBuffImage::SetBackColor(COLORREF cBackColor)
{
    m_cBackColor = cBackColor;
    m_hBrush = ::CreateSolidBrush(m_cBackColor);
}

// ׼������
BOOL CMsGdiBuffImage::BeginDraw(BOOL bErasure)
{
    if (bErasure)
    {
        ::FillRect(m_hBuffDc, &m_Rc, m_hBrush);
    }
    return TRUE;
}

// ��������
BOOL CMsGdiBuffImage::PrintText(HFONT hFont, RECT* lpRc, TCHAR* szText, COLORREF cColor, INT nTextLen, DWORD dwFormat)
{
    if (hFont)
    {
        ::SetTextColor(m_hBuffDc, cColor);
        HANDLE hObj = ::SelectObject(m_hBuffDc, hFont);
        ::DrawText(m_hBuffDc, szText, nTextLen, lpRc, dwFormat);
        ::SelectObject(m_hBuffDc, hObj);
        return TRUE;
    }
    return FALSE;
}

// ����ͼƬ
BOOL CMsGdiBuffImage::EndDraw(INT nDrawX, INT nDrawY, DWORD dwRop)
{
    if (m_bInit && m_hMainDc && m_hBuffBitmap && m_hBuffDc)
    {
        return ::BitBlt(m_hMainDc, 0, 0, m_dwWidth, m_dwHeight, m_hBuffDc, nDrawX, nDrawY, dwRop);
    }
    else
    {
        return FALSE;
    }
}

BOOL CMsGdiBuffImage::EndDraw(HDC hDc, INT nDrawX, INT nDrawY, DWORD dwRop)
{
    if (m_bInit && hDc && m_hBuffBitmap && m_hBuffDc)
    {
        return ::BitBlt(hDc, 0, 0, m_dwWidth, m_dwHeight, m_hBuffDc, nDrawX, nDrawY, dwRop);
    }
    else
    {
        return FALSE;
    }
}
