#pragma once                //��//��
#ifndef __DISTRICNODE_H__   //��//��
#define __DISTRICNODE_H__   //��//��
//������������������������������//��
//�ļ����ƣ�DistricNode.h       //��
//���������������ڵ�            //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// �����ڵ�
class CDistricNode 
    : public CBaseNode
{
public:
    CDistricNode(VOID);
    ~CDistricNode(VOID);

    // ��ʼ��
    virtual BOOL Init(DWORD dwRoomNodeCount);

    // ��Ⱦ
    virtual VOID Render(IN OUT INT& nBottom);
private:

};
typedef CDistricNode*  lpCDistricNode;


//����������������������
#endif//#pragma once  ��
//����������������������
