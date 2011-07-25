#pragma once                //��//��
#ifndef __GAMETYPENODE_H__  //��//��
#define __GAMETYPENODE_H__  //��//��
//������������������������������//��
//�ļ����ƣ�GameTypeNode.h      //��
//������������Ϸ���ͽڵ�        //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ���ͽڵ�
class CGameTypeNode
    : public CBaseNode
{
public:
    CGameTypeNode(CMsGdiBuffImage* lpCMsGdiBuffImage, CBaseNode* lpPreviousNode, CBaseNode* lpNextNode);
    ~CGameTypeNode(VOID);

    virtual VOID Open(BOOL bPrint);             // �򿪽ڵ�
    virtual VOID Close(BOOL bPrint);            // �رսڵ�
    virtual VOID Destroy(VOID);                 // ���ٽڵ�
    virtual VOID CutMe(VOID);                   // �������жϿ��Լ�

public:

    // ��ʼ��
    BOOL Init(S_GAME_TYPE_INFO* pInfo);

    // ���ƽڵ� 
    VOID Render(IN OUT INT& nBottom);

private:
    S_GAME_TYPE_INFO    m_GameTypeInfo;     // ��Ϸ���ͽṹ��
    CMsGdiPlusImage*    m_pIocnImage;       // ���ͼƬ����
    CMsGdiPlusImage*    m_pLinkImage;       // ����ͼƬ����
};

typedef CGameTypeNode* lpCGameTypeNode;

//����������������������
#endif//#pragma once  ��
//����������������������
