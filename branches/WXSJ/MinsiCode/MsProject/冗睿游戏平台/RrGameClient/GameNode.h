#pragma once                //��//��
#ifndef __GAMENODE_H__      //��//��
#define __GAMENODE_H__      //��//��
//������������������������������//��
//�ļ����ƣ�GameNode.h          //��
//������������Ϸ�ڵ�            //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ��Ϸ�ڵ�
class CGameNode
    : public CBaseNode
{
public:
    CGameNode(VOID);
    ~CGameNode(VOID);

    // ��ʼ��
    virtual BOOL Init(DWORD dwDistricNodeCount);

    // ��Ⱦ
    virtual VOID Render(IN OUT INT& nBottom);
private:

};
typedef CGameNode*  lpCGameNode;


//����������������������
#endif//#pragma once  ��
//����������������������
