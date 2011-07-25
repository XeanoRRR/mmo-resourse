#include "Precompiled.h"
#include "TestRender.h"

CTestRender::CTestRender(
    HWND hWnd,
    DWORD dwWidth,
    DWORD dwHeight
    )
    : CMs2DRender_DX9(hWnd, dwWidth, dwHeight)
    , m_lpTexture   (NULL)
{
    // ����ͼԪͼƬ
    TCHAR szFullPath[MAX_PATH] = {0};
    CreateFullPath(szFullPath, _T("%s/RrTetris/"));
    _tcscat(szFullPath, _T("BackBitmap.PNG"));
    m_lpTexture = NEW CMs2DTexture(szFullPath);
    if (m_lpTexture)
    {
        RECT rc = {0, 0, dwWidth, dwHeight};
        m_lpTexture->SetRect(rc, rc);
    }
    else
    {
        AssertEx(AL_CATAST, m_lpTexture, _T("�ڴ治��"));
    }
}

CTestRender::~CTestRender(VOID)
{
    SAFE_DELETE(m_lpTexture);
}

// ����
VOID CTestRender::Draw(VOID)
{
    m_lpTexture->RenderTexture(m_lpDirectDevice9);
}
