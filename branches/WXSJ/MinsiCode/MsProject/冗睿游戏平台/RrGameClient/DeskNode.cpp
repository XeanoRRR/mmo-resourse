#include "Precompiled.h"

CDeskNode::CDeskNode(VOID)
: CBaseNode         (RGB(0,0,100))
, m_dwPlayerCount   (0)
{
    m_eType = NODE_DESK;
    if (g_pCGameStation)
    {
        m_dwWidth = g_pCGameStation->GetConfig()->dwDeskWidth; 
        m_dwHight = g_pCGameStation->GetConfig()->dwDeskHight;
        m_dwIndent = g_pCGameStation->GetConfig()->dwDeskIndent;
    }
    else
    {
        AssertEx(g_pCGameStation, _T("զ���£�"));
    }
}

CDeskNode::~CDeskNode(VOID)
{
    ;
}

// ��ʼ��
BOOL CDeskNode::Init(DWORD dwPlayerCount)
{
    m_dwChildCount = 0;
    m_dwPlayerCount = dwPlayerCount;
    return TRUE;
}

// ��Ⱦ
VOID CDeskNode::Render(HDC hDC)
{
    USE_PARAM(hDC);
    return;
}