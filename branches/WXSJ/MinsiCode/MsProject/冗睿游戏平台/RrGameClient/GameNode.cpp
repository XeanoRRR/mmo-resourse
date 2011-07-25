#include "Precompiled.h"

CGameNode::CGameNode(VOID)
: CBaseNode (RGB(0,255,0))
{
    m_eType = NODE_GAME;
    if (g_pCGameStation)
    {
        m_dwWidth = g_pCGameStation->GetConfig()->dwGameWidth; 
        m_dwHight = g_pCGameStation->GetConfig()->dwGameHight;
        m_dwIndent = g_pCGameStation->GetConfig()->dwGameIndent;
    }
    else
    {
        AssertEx(g_pCGameStation, _T("զ���£�"));
    }
}

CGameNode::~CGameNode(VOID)
{
    ;
}

// ��ʼ��
BOOL CGameNode::Init(DWORD dwDistricNodeCount)
{
    _ENTER_FUN_C;

    m_dwChildCount = dwDistricNodeCount;
    if (m_dwChildCount)
    {
        m_lppChildList = NEW lpCBaseNode[m_dwChildCount];
        if (m_lppChildList)
        {
            FOR_x_TO(i, m_dwChildCount)
            {
                m_lppChildList[i] = NEW CDistricNode;
                AssertEx(m_lppChildList[i], _T("�ڴ治��"));
            }

            if (g_pCGameStation)
            {
                DWORD dwRoomCountInEveryDistrict = g_pCGameStation->GetConfig()->dwRoomCountInEveryDistrict;
                FOR_x_TO(i, m_dwChildCount)
                {
                    m_lppChildList[i]->Init(dwRoomCountInEveryDistrict);
                }
                return TRUE;
            }
            else
            {
                AssertEx(g_pCGameStation, _T("��զ����"));
            }
        }
        else
        {
            AssertEx(m_lppChildList, _T("�ڴ治��"));
        }
    }

    _DISPOSE_END_C
    {
        INT a = 100;
        a = 200;
    }
    _LEAVE_FUN;
    return FALSE;
}

// ��Ⱦ
VOID CGameNode::Render(IN OUT INT& nBottom)
{
    return CBaseNode::Render(nBottom);
}