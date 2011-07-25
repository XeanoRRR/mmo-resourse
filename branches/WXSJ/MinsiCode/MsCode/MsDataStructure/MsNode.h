#pragma once        //��//��
#ifndef __MSNODE_H__//��//��
#define __MSNODE_H__//��//��
//����������������������//��
//�ļ����ƣ�MsNode.h    //��
//�����������ڵ���      //��
//ƽ̨��أ�Windows     //��
//��������������������������

class CMsNode
{
public:
    CMsNode(VOID);

    BOOL Init(
        CMsNode* lpRootNode,
        CMsNode* lpParentNode,
        CMsNode** lppSubNode,
        LPVOID lpData
        );

    virtual LPVOID ReSet(
        CMsNode* lpParentNode,
        CMsNode** lppSubNode,
        LPVOID lpData
        );

    virtual VOID Release(VOID) = 0;

    LPVOID  GetData(VOID);                      // ��ȡ�Լ�������
    LPVOID  GetSubNodeData(DWORD dwIndex);      // ��ȡ�¼�������
    LPVOID  GetParentData(VOID);                // ��ȡ�ϼ�������

    CMsNode* GetRootNode(VOID);                 // ��ȡ�����ĵ�ַ
    CMsNode* SetRootNode(CMsNode* lpTreeNode);  // ���ø����ĵ�ַ

    CMsNode* GetParentNode(VOID);               // ��ȡ�ϼ��ĵ�ַ
    CMsNode* SetParentNode(CMsNode* lpTreeNode);// �����ϼ��ĵ�ַ

    CMsNode* GetSubNode(DWORD dwIndex);         // ��ȡ�¼��ĵ�ַ
    CMsNode* SetSubNode(                        // �����¼��ĵ�ַ
        DWORD dwIndex,
        CMsNode* lpTreeNode
        );

    DWORD AddBearLevel(VOID);       // ����Ȩ�ؼ���
    DWORD SubBearLevel(VOID);       // ����Ȩ�ؼ���
    DWORD GetBearLevel(VOID);       // ��ȡȨ�ؼ���

protected:
    CMsNode*    m_lpRootNode;       // ���ڵ�ָ��
    CMsNode*    m_lpParentNode;     // ���ڵ�ָ��
    CMsNode**   m_lppSubNode;       // ��һ���ڵ�����
    DWORD       m_dwSubNodeCount;   // ��һ���ڵ����
    LPVOID      m_lpData;           // ����
    DWORD       m_dwBearLevel;      // Ȩ�ؼ���
    BOOL        m_bInit;            // �Ƿ��Ѿ���ʼ��
};
typedef class CMsNode* lpCMsNode;

//����������������������
#endif//#pragma once//��
//����������������������
