#pragma once                //��//��
#ifndef __GAMETREE_H__      //��//��
#define __GAMETREE_H__      //��//��
//������������������������������//��
//�ļ����ƣ�GameTree.h          //��
//������������Ϸ�б���          //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ��Ϸ�б���
class CGameTree
{
public:
    CGameTree(CMsGdiBuffImage* lpCMsGdiBuffImage);
    ~CGameTree(VOID);

    // ���ƽڵ�
    VOID Render(VOID);

    // ���ٽڵ�
    VOID Destroy(VOID);

    // ������Ϸ����
    CGameTypeNode* AddGameTypeNode(VOID);

    // ����Ƿ񴩹�������
    BOOL IsOnScrollbar(POINT& Pos);

    // ��¼���λ��
    VOID SaveMousePos(POINT& Pos);

    // ��¼���λ��
    VOID OperationPosOffset(POINT& Pos);

    // �ƶ�������
    VOID MoveScroll(POINT& Pos);

    // ���ù���������
    VOID GetScrollFocus(VOID);

    // �ͷŹ���������
    VOID ReleaseScrollFocus(VOID);

    // �Ƿ����������
    BOOL IsScrollFocus(VOID);

    // ���ýڵ�
    VOID Reset(VOID);
private:

    // �б���������
    CGameTreeScrollbar* m_pCGameTreeScrollbar;

    // ����������
    BOOL                m_bScrollFocus;

    // �б�������
    CGameTreeCaption*   m_pCGameTreeCaption;

    // ���ⷶΧ
    RECT                m_CaptionRc;

    // ��������ͼ
    CMsGdiBuffImage*    m_lpCMsGdiBuffImage;

    // ԭ���λ��
    POINT   m_OldMousePos;

    // ��ǰ���λ��
    POINT   m_CurMousePos;

    // �������λ��
    INT     m_nLastBottom;

    // ��Ϸ���͸��ڵ�
    lpCGameTypeNode m_lpCGameTypeRoot;

    // ���һ���ڵ�
    lpCGameTypeNode m_lpLastNode;

    // ��Ϸ���͸���
    DWORD   m_dwGameTypeNodeCount;

    // ��������
    MsLock  m_Lock;
};

//����������������������
#endif//#pragma once  ��
//����������������������
