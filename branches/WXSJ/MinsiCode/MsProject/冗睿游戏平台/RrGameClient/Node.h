#pragma once                //��//��
#ifndef __NODE_H__          //��//��
#define __NODE_H__          //��//��
//������������������������������//��
//�ļ����ƣ�Node.h              //��
//������������ϷTree�ڵ�        //��
//ƽ̨��أ�Windows             //��
//����������������������������������
#define CHILDMAX 10
#define NODEWIDTH 40
#define NODEHIGHT 20
#define NODEWIDTHINDENT  5

using namespace std;

class CNode
{
public:
    CNode(VOID);
    ~CNode(VOID);

    BOOL Init(RECT PaterRect,DWORD dwNodeId, BOOL bIsRoot = FALSE);//��ʼ����ǰ�ڵ�
    VOID Realese();//�ӵ�ǰ�ڵ㿪ʼ�ͷ�
    VOID AddNode(DWORD dwNewNodeId);//��ӽڵ�
    VOID OpenNode(BOOL bOpen);//չ���رյ�ǰ�ڵ�
    BOOL IsOpen(){return m_bIsOpen;}
    typedef vector<CNode*> ChildNodeList;

    RECT GetRect(){return m_rect;}
    VOID UpdataRect();
    VOID Render(HDC hDC);
    ChildNodeList GetChildList(){return m_pChildList;}
private:
    ChildNodeList    m_pChildList;//�ӽڵ��
    DWORD            m_dwChildCurNum;//�ӽڵ����
    BOOL             m_bIsOpen;//�Ƿ�չ��
    RECT             m_rect;//�ڵ�����
    DWORD            m_dwNodeId;//�ڵ�����
    DWORD            m_dwChildId;//�ӽڵ�����
};


//����������������������
#endif//#pragma once  ��
//����������������������
