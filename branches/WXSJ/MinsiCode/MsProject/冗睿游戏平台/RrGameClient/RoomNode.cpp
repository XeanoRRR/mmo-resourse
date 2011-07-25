#include "Precompiled.h"

CRoomNode::CRoomNode(VOID)
: CBaseNode       (RGB(0,255,255))
{
    m_eType = NODE_ROOM;
    if (g_pCGameStation)
    {
        m_dwWidth = g_pCGameStation->GetConfig()->dwRoomWidth; 
        m_dwHight = g_pCGameStation->GetConfig()->dwRoomHight;
        m_dwIndent = g_pCGameStation->GetConfig()->dwRoomIndent;
    }
    else
    {
        AssertEx(g_pCGameStation, _T("զ���£�"));
    }
}

CRoomNode::~CRoomNode(VOID)
{
    ;
}

// ��ʼ��
BOOL CRoomNode::Init(DWORD dwRoomNodeCount)
{
    m_dwChildCount = dwRoomNodeCount;
    if (m_dwChildCount)
    {
        m_lppChildList = NEW lpCBaseNode[m_dwChildCount];
        if (m_lppChildList)
        {
            FOR_x_TO(i, m_dwChildCount)
            {
                m_lppChildList[i] = NEW CDeskNode;
                AssertEx(m_lppChildList[i], _T("�ڴ治��"));
            }

            if (g_pCGameStation)
            {
                DWORD dwPlayerCountInEveryDesk = g_pCGameStation->GetConfig()->dwPlayerCountInEveryDesk;
                FOR_x_TO(i, m_dwChildCount)
                {
                    m_lppChildList[i]->Init(dwPlayerCountInEveryDesk);
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
    return FALSE;
}

// ��Ⱦ
VOID CRoomNode::Render(INT& nBottom)
{
    return CBaseNode::Render(nBottom);
}