#include "Precompiled.h"
#include "MsNode.h"

CMsNode::CMsNode(VOID)
: m_lpRootNode      (NULL)
, m_lpParentNode    (NULL)
, m_lppSubNode      (NULL)
, m_lpData          (NULL)
, m_bInit           (FALSE)
, m_dwBearLevel     (INVALID_UID)
{
    _ENTER_FUN_C;

    _LEAVE_FUN_DISPOSE_END_C;
}

BOOL CMsNode::Init(
    CMsNode* lpRootNode,
    CMsNode* lpParentNode,
    CMsNode** lppSubNode,
    LPVOID lpData
    )
{
    m_lpRootNode        = lpRootNode;
    m_lpParentNode      = lpParentNode;
    m_lpData            = lpData;
    m_dwBearLevel       = 0;

    memcpy(
        m_lppSubNode,
        lppSubNode,
        sizeof(lpCMsNode) * m_dwSubNodeCount
        );

    m_bInit = TRUE;

    return TRUE;
}

LPVOID CMsNode::ReSet(
    CMsNode* lpParentNode,
    CMsNode** lppSubNode,
    LPVOID lpData
    )
{
    if (!m_bInit)
    {
        return NULL;
    }

    m_dwBearLevel = 0;

    LPVOID lpOldData = m_lpData;

    m_lpParentNode  = lpParentNode;
    m_lpData        = lpData;
    if (lppSubNode)
    {
        memcpy(
            m_lppSubNode,
            lppSubNode,
            sizeof(lpCMsNode) * m_dwSubNodeCount
            );
    }
    else
    {
        ZeroMemory(m_lppSubNode, sizeof(CMsNode**) * m_dwSubNodeCount);
    }

    return lpOldData;
}

// ��ȡ�Լ�������
LPVOID CMsNode::GetData(VOID)
{
    return m_lpData;
}

// ��ȡ�Ӽ�������
LPVOID CMsNode::GetSubNodeData(DWORD dwIndex)
{
    // �����ǰ�ڵ����Ӽ�
    if (!m_lppSubNode)
    {
        return NULL;
    }

    // ������ʵ��ӽڵ㲻����
    if (dwIndex >= m_dwSubNodeCount)
    {
        return NULL;
    }

    return m_lppSubNode[dwIndex]->GetData();
}

// ��ȡ�ϼ�������
LPVOID CMsNode::GetParentData(VOID)
{
    // �����ǰ�ڵ����ϼ�
    if (!m_lpParentNode)
    {
        return NULL;
    }

    return m_lpParentNode->GetData();
}

// ��ȡ�Ӽ��ĵ�ַ
lpCMsNode CMsNode::GetSubNode(DWORD dwIndex)
{
    // �����ǰ�ڵ����Ӽ�
    if (!m_lppSubNode)
    {
        return NULL;
    }

    // ������ʵ��ӽڵ㲻����
    if (dwIndex >= m_dwSubNodeCount)
    {
        return NULL;
    }

    return m_lppSubNode[dwIndex];
}

// ��ȡ�ϼ��ĵ�ַ
lpCMsNode CMsNode::GetParentNode(VOID)
{
    return m_lpParentNode;
}

// �����Ӽ��ĵ�ַ
lpCMsNode CMsNode::SetSubNode(DWORD dwIndex, lpCMsNode lpTreeNode)
{
    // �����ǰ�ڵ����Ӽ�
    if (!m_lppSubNode)
    {
        return NULL;
    }

    // ������ʵ��ӽڵ㲻����
    if (dwIndex >= m_dwSubNodeCount)
    {
        return NULL;
    }

    lpCMsNode pCMsTreeNode_Temp = m_lppSubNode[dwIndex];
    m_lppSubNode[dwIndex] = lpTreeNode;
    return pCMsTreeNode_Temp;
}

// �����ϼ��ĵ�ַ
lpCMsNode CMsNode::SetParentNode(lpCMsNode lpTreeNode)
{
    lpCMsNode pCMsTreeNode_Temp = m_lpParentNode;
    m_lpParentNode = lpTreeNode;
    return pCMsTreeNode_Temp;
}

// ��ȡ�����ĵ�ַ
lpCMsNode CMsNode::GetRootNode(VOID)
{
    return m_lpRootNode;
}

// ���ø����ĵ�ַ
lpCMsNode CMsNode::SetRootNode(lpCMsNode lpTreeNode)
{
    lpCMsNode pCMsTreeNode_Temp = m_lpRootNode;
    m_lpRootNode = lpTreeNode;
    return pCMsTreeNode_Temp;
}

// ����Ȩ�ؼ���
DWORD CMsNode::AddBearLevel(VOID)
{
    return m_dwBearLevel++;
}


// ����Ȩ�ؼ���
DWORD CMsNode::SubBearLevel(VOID)
{
    return m_dwBearLevel--;
}

// ��ȡȨ�ؼ���
DWORD CMsNode::GetBearLevel(VOID)
{
    return m_dwBearLevel;
}
