#pragma once                //��//��
#ifndef __ROOMNODE_H__      //��//��
#define __ROOMNODE_H__      //��//��
//������������������������������//��
//�ļ����ƣ�RoomNode.h          //��
//���������������ڵ�            //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// ����ڵ�
class CRoomNode
    : public CBaseNode
{
public:
    CRoomNode(VOID);
    ~CRoomNode(VOID);

    // ��ʼ��
    virtual BOOL Init(DWORD dwDeskNodeCount);

    // ��Ⱦ
    virtual VOID Render(IN OUT INT& nBottom);
private:

};
typedef CRoomNode*  lpCRoomNode;


//����������������������
#endif//#pragma once  ��
//����������������������
