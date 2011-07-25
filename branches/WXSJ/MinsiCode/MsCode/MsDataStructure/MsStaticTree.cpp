#include "Precompiled.h"
#include "MsNode.h"
#include "MsPoolManager.h"
#include "MsStaticTree.h"

// ����
CMsStaticTree::CMsStaticTree(DWORD dwMaxSaveCount, DWORD dwTreeDepth)
: m_dwMaxNodeCount      (dwMaxSaveCount*dwTreeDepth)
, m_CMsPoolManager      (dwMaxSaveCount*dwTreeDepth)
, m_dwMaxSaveCount      (dwMaxSaveCount*dwTreeDepth)
, m_dwTreeDepth         (dwTreeDepth)
, m_lpCMsNode           (NULL)
{
    AssertEx(AL_NORMAL, m_dwMaxSaveCount, _T("���нڵ��������Ϊ��"));
    AssertEx(AL_NORMAL, m_dwTreeDepth, _T("���㲻��Ϊ��"));

    BOOL bRet = this->Init();
    AssertEx(AL_NORMAL, bRet, _T("��ʼ����̬��ʧ��"));
}

// ����
CMsStaticTree::~CMsStaticTree(VOID)
{
    ;
}

// ��ʼ��
BOOL CMsStaticTree::Init(VOID)
{
    m_lpCMsNode = NEW CMsTreeNode[m_dwMaxNodeCount];
    AssertEx(AL_CATAST, m_lpCMsNode, _T("�ڴ治��"));
    return TRUE;
}

// �ͷ�
BOOL CMsStaticTree::Release(VOID)
{
    SAFE_DELETE_ARRAY(m_lpCMsNode);
    return TRUE;
}

// ����һ���ڵ����
lpCMsTreeNode CMsStaticTree::Develop(DWORD dwTreeDepth, lpCMsTreeNode lpParentNode, DWORD dwSubNodeIndex)
{
    lpCMsTreeNode lpNewNode = NULL;
    // ����Ǹ��ڵ�������
    if (0 == dwTreeDepth)
    {
        // ����˽ڵ��Ѿ�����
        if (m_lpCMsNode->m_lpSubNode[dwSubNodeIndex])
        {
            return m_lpCMsNode->m_lpSubNode[dwSubNodeIndex];
        }
        else
        {
            // ��ȡ�¸���������
            DWORD dwIndex = m_CMsPoolManager.GetNextIndex();

            // ��¼�½ڵ�
            lpNewNode = &m_lpCMsNode[dwIndex];

            // ���ϼ��ڵ�Ķ�Ӧ��֧���½ڵ�����
            m_lpCMsNode->m_lpSubNode[dwSubNodeIndex] = lpNewNode;

            // ���������������½ڵ�
            lpNewNode->m_dwPoolIndex = dwIndex;

            // �����ڵ�Ȩ������
            lpNewNode->AddBearLevel();
        }
    }

    // ������Ǹ��ڵ�
    else
    {
        // ����˽ڵ��Ѿ�����
        if (lpParentNode->m_lpSubNode[dwSubNodeIndex])
        {
            return lpParentNode->m_lpSubNode[dwSubNodeIndex];
        }
        else
        {
            // ��ȡ�¸���������
            DWORD dwIndex = m_CMsPoolManager.GetNextIndex();

            // ��¼�½ڵ�
            lpNewNode = &m_lpCMsNode[dwIndex];

            // ���ϼ��ڵ�Ķ�Ӧ��֧���½ڵ�����
            m_lpCMsNode->m_lpSubNode[dwSubNodeIndex] = lpNewNode;

            // �����ϼ��ڵ�
            lpNewNode->SetParentNode(lpParentNode);

            // ���������������½ڵ�
            lpNewNode->m_dwPoolIndex = dwIndex;

            // ѭ�����ϼ��ڵ�Ȩ������
            lpCMsNode lpNode = lpNewNode;
            do 
            {
                lpNode->AddBearLevel();
                lpNode = lpNode->GetParentNode();
            } while (lpNode);
        }
    }
    return lpNewNode;
}

// ����һ���ڵ�
BOOL CMsStaticTree::Pruning(lpCMsTreeNode lpParentNode, DWORD dwSubNodeIndex)
{
    lpCMsNode lpSubNodeForDel = lpParentNode->GetSubNode(dwSubNodeIndex);

    if (!lpSubNodeForDel)
    {
        return FALSE;
    }

    if (lpSubNodeForDel->GetBearLevel() > 0)
    {
        return FALSE;
    }

    m_CMsPoolManager.FreeIndexByPool(lpParentNode->m_dwPoolIndex);
    lpSubNodeForDel->ReSet(NULL, NULL, NULL);
    lpParentNode->SetSubNode(dwSubNodeIndex, NULL);
    return TRUE;
}

// ��ȡʹ�ø���
DWORD CMsStaticTree::GetUseCount(VOID)
{
    return m_CMsPoolManager.GetCurrentCount();
}

// ͨ������ֱ�ӻ�ȡ�ڵ�
lpCMsTreeNode CMsStaticTree::GetNodeByIndex(DWORD dwIndex)
{
    DWORD dwPoolIndex = m_CMsPoolManager.LocateIndexToPoolIndex(dwIndex);
    return &m_lpCMsNode[dwPoolIndex];
}