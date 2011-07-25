#include "Precompiled.h"

CBaseNode::CBaseNode(CBaseNode* lpPreviousNode, CBaseNode* lpNextNode)
: m_lpNextNode      (lpNextNode)
, m_lpPreviousNode  (lpPreviousNode)
, m_bOpen           (FALSE)
, m_dwWidth         (0)
, m_dwHight         (0)
, m_dwIndent        (0)
, m_dwBetween       (0)
, m_eNodeType       (INVALID_UID)
, m_dwNodeId        (INVALID_UID)
{
    if (m_lpNextNode && m_lpPreviousNode)
    {
        if (m_lpPreviousNode != HEAD_NODE)
        {
            lpPreviousNode->m_lpNextNode = this;
        }
        if (m_lpNextNode != TAIL_NODE)
        {
            lpNextNode->m_lpPreviousNode = this;
        }
    }
}

CBaseNode::~CBaseNode(VOID)
{
    this->CutMe();
}

// �򿪽ڵ�
VOID CBaseNode::Open(BOOL bPrint)
{
    m_bOpen = TRUE;
    if (bPrint)
    {
        ;
    }
}

// �رսڵ�
VOID CBaseNode::Close(BOOL bPrint)
{
    m_bOpen = FALSE;
    if (bPrint)
    {
        ;
    }
}

// ���ٽڵ�
VOID CBaseNode::Destroy(VOID)
{
    this->Close(TRUE);
}

// �������жϿ��Լ�
VOID CBaseNode::CutMe(VOID)
{
    if (m_lpNextNode && m_lpPreviousNode)
    {
        if (m_lpPreviousNode == HEAD_NODE)
        {
            if (m_lpNextNode != TAIL_NODE)
            {
                m_lpNextNode->m_lpPreviousNode = HEAD_NODE;
            }
            m_lpPreviousNode = NULL;
            m_lpNextNode = NULL;
        }
        else if (m_lpNextNode == TAIL_NODE)
        {
            if (m_lpPreviousNode != HEAD_NODE)
            {
                m_lpPreviousNode->m_lpNextNode = TAIL_NODE;
            }
            m_lpPreviousNode = NULL;
            m_lpNextNode = NULL;
        }
        else
        {
            m_lpNextNode->m_lpPreviousNode = m_lpPreviousNode;
            m_lpPreviousNode->m_lpNextNode = m_lpNextNode;
            m_lpPreviousNode = NULL;
            m_lpNextNode = NULL;
        }
    }
}

// �ڵ�λ��
LPRECT CBaseNode::GetRect(VOID)
{
    return &m_Rect;
}

BOOL CBaseNode::IsOpen(VOID)
{
    return m_bOpen;
}

// ��ýڵ�Id
DWORD CBaseNode::GetNodeId(VOID)
{
    return m_dwNodeId;
}

// ��ȡ��һ���ڵ�
CBaseNode* CBaseNode::GetNextNode(VOID)
{
    return m_lpNextNode;
}

// ��ȡǰһ���ڵ�
CBaseNode* CBaseNode::GetPreviousNode(VOID)
{
    return m_lpPreviousNode;
}