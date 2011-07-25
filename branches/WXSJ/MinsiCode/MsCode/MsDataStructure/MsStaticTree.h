#pragma once                //��//��
#ifndef __MSSTATICTREE_H__  //��//��
#define __MSSTATICTREE_H__  //��//��
//������������������������������//��
//�ļ����ƣ�MsStaticTree.h      //��
//������������̬��              //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ��ÿ���ڵ��֧������
#define MAX_EMBRANCHMENT_COUNT 256

class CMsTreeNode
    : public CMsNode
{
public:
    CMsTreeNode(VOID)
        : m_dwPoolIndex (INVALID_UID)
    {
        CMsNode::m_lppSubNode = (CMsNode**)m_lpSubNode;
        ZeroMemoryArray(m_lpSubNode);
    }

    CMsTreeNode(
        lpCMsNode lpRootNode,
        lpCMsNode lpParentNode,
        lpCMsNode* lppSubNode,
        LPVOID lpData
        )
        : m_dwPoolIndex (INVALID_UID)
    {
        _ENTER_FUN_C;

        CMsNode::m_bInit = FALSE;
        CMsNode::m_lppSubNode = (CMsNode**)m_lpSubNode;
        CMsNode::m_dwSubNodeCount = MAX_EMBRANCHMENT_COUNT;
        BOOL bRet = this->Init(
            lpRootNode,
            lpParentNode,
            lppSubNode,
            lpData
            );
        AssertEx(AL_SHOW_ONLY, bRet, _T("��ʼ���ڵ�ʧ��"));
        ZeroMemoryArray(m_lpSubNode);

        _LEAVE_FUN_DISPOSE_END_C;
    }

    virtual VOID Release(VOID)
    {
        CMsNode::m_bInit = FALSE;
        m_dwPoolIndex = INVALID_UID;
        ZeroMemoryArray(m_lpSubNode);
    }

    virtual LPVOID ReSet(
        CMsNode*,
        CMsNode**,
        LPVOID)
    {
        m_dwPoolIndex = INVALID_UID;
        return CMsNode::ReSet(NULL, NULL, NULL);
    }

    CMsTreeNode*    m_lpSubNode[MAX_EMBRANCHMENT_COUNT];    // ��һ���ڵ�����
    DWORD           m_dwPoolIndex;  // �����е�����
    //DWORD           m_dwTierIndex;  // ������������
};
typedef class CMsTreeNode* lpCMsTreeNode;
typedef class CMsTreeNode** lppCMsTreeNode;


class CMsStaticTree
{
public:
    CMsStaticTree(DWORD dwMaxSaveCount, DWORD dwTreeDepth); // ����
    ~CMsStaticTree(VOID);   // ����

    VOID operator = (CMsStaticTree){AssertEx(AL_NORMAL, FALSE, _T("���ø�ֵ�����"));}
    BOOL Init(VOID);        // ��ʼ��
    BOOL Release(VOID);     // �ͷ�

    // ����һ���ڵ����
    lpCMsTreeNode Develop(
        DWORD dwTreeDepth,
        lpCMsTreeNode lpParentNode,
        DWORD dwSubNodeIndex
        );

    // ����һ���ڵ�
    BOOL Pruning(lpCMsTreeNode lpParentNode, DWORD dwSubNodeIndex);

    // ��ȡʹ�ø���
    DWORD GetUseCount(VOID);

    // ͨ������ֱ�ӻ�ȡ�ڵ�
    /* ����ô˺��������ڵ�, �뱣֤�ڱ����ڼ䲻������κ���������ĳ�Ա���� */
    lpCMsTreeNode GetNodeByIndex(DWORD dwIndex);
private:

    lpCMsTreeNode   m_lpCMsNode;        // �ڵ�����
    CONST DWORD     m_dwMaxSaveCount;   // ��󴢴�����
    CONST DWORD     m_dwMaxNodeCount;   // ���ڵ����
    CONST DWORD     m_dwTreeDepth;      // �����������
    CMsPoolManager  m_CMsPoolManager;   // ��̬���ڵ�������
};

//����������������������
#endif//#pragma once//��
//����������������������
