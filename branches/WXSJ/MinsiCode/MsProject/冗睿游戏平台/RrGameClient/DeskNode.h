#pragma once                //��//��
#ifndef __DESKNODE_H__      //��//��
#define __DESKNODE_H__      //��//��
//������������������������������//��
//�ļ����ƣ�DeskNode.h          //��
//�������������ӽڵ�            //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ���ӽڵ�
class CDeskNode
    : public CBaseNode
{
public:
    CDeskNode(VOID);
    ~CDeskNode(VOID);

    // ��ʼ��
    BOOL Init(DWORD dwPlayerCount);

    // ��Ⱦ
    VOID Render(HDC hDC);

private:
    DWORD   m_dwPlayerCount;

};
typedef CDeskNode*  lpCDeskNode;


//����������������������
#endif//#pragma once  ��
//����������������������
