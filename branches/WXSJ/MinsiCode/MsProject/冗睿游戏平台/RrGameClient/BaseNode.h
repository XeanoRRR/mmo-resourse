#pragma once                //��//��
#ifndef __BASENOD_H__       //��//��
#define __BASENOD_H__       //��//��
//������������������������������//��
//�ļ����ƣ�DeskNode.h          //��
//�������������ӽڵ�            //��
//ƽ̨��أ�Windows             //��
//����������������������������������

//
////���ڵ�λ��
//#define TREE_X 10
//#define TREE_Y 10
//#define TREE_W 40
//#define TREE_H 20
//
////��������Χ
//#define SCROLLBARUP     40
//#define SCROLLBARDOWN   640
//
////�������߶�
//#define SCROLLBARHIGHT  60
//
////�ڵ���ʾ��Χ
//#define TREESHOWUP TREE_Y + TREE_H
//
//#define TREESHOWDOWN 700

// ͷ�ڵ�궨��
#define HEAD_NODE   (CBaseNode*)INVALID_POINTER

// β�ڵ�궨��
#define TAIL_NODE   (CBaseNode*)INVALID_POINTER

// �ڵ����
class CBaseNode
{
public:
    CBaseNode(CBaseNode* lpPreviousNode = HEAD_NODE, CBaseNode* lpNextNode = TAIL_NODE);
    virtual ~CBaseNode(VOID);
    virtual VOID Open(BOOL bPrint);     // �򿪽ڵ�
    virtual VOID Close(BOOL bPrint);    // �رսڵ�
    virtual VOID Destroy(VOID);         // ���ٽڵ�
    virtual VOID CutMe(VOID);           // �������жϿ��Լ�

public:
    LPRECT      GetRect(VOID);          // �ڵ�λ��
    BOOL        IsOpen(VOID);           // �Ƿ��
    DWORD       GetNodeId(VOID);        // ��ýڵ�Id
    CBaseNode*  GetNextNode(VOID);      // ��ȡ��һ���ڵ�
    CBaseNode*  GetPreviousNode(VOID);  // ��ȡǰһ���ڵ�

protected:
    CBaseNode*  m_lpNextNode;       // ��һ���ڵ�
    CBaseNode*  m_lpPreviousNode;   // ǰһ���ڵ�

    RECT        m_Rect;         // �ڵ�λ��
    BOOL        m_bOpen;        // �Ƿ�չ���ڵ�
    DWORD       m_dwWidth;      // �ڵ���
    DWORD       m_dwHight;      // �ڵ�߶�
    DWORD       m_dwIndent;     // �ڵ�����
    DWORD       m_dwBetween;    // �ڵ���
    DWORD       m_eNodeType;    // �ڵ�����
    DWORD       m_dwNodeId;     // �ڵ�Id
};
typedef CBaseNode* lpCBaseNode;
//����������������������
#endif//#pragma once  ��
//����������������������
