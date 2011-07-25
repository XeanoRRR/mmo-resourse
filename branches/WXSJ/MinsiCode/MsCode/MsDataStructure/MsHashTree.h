#pragma once            //��//��
#ifndef __MSHASHTREE_H__//��//��
#define __MSHASHTREE_H__//��//��
//��������������������������//��
//�ļ����ƣ�MsHashTree.h    //��
//������������ϣ�����      //��
//ƽ̨��أ�Windows         //��
//������������������������������
//
//// ��辽ڵ�
//class CMsTreeNode
//    : public CPoolMember
//{
//public:
//    friend class CMsTreeIndex;
//    CMsTreeNode(CMsTreeIndex* lpCMsTreeIndex, DWORD dwTreeFurcation, CMsTreeNode* lpFatherTreeFurcation);
//    CMsTreeNode(VOID);
//    ~CMsTreeNode(VOID);
//    VOID         Create(CMsTreeIndex* lpCMsTreeIndex, DWORD dwTreeFurcation, CMsTreeNode* lpFatherTreeFurcation, DWORD dwBeginFlag);
//    VOID         SetFather(CMsTreeNode* lpFatherTreeFurcation);
//    CMsTreeNode* GetFather(VOID){return m_lpFatherTreeFurcation;}
//    VOID         AddUse(VOID){m_dwUseCount++;}
//    VOID         SubUse(VOID){m_dwUseCount--;}
//    BOOL         IsUse(VOID){return m_dwUseCount?TRUE:FALSE;}
//    BOOL         SetData(LPVOID lpData);
//protected:
//private:
//    virtual BOOL SetData(LPVOID lpData, DWORD dwDataSize);
//    BOOL            m_bMustInit;            // ������г�ʼ��
//    DWORD           m_dwBeginFlag;          // ��ʼ���ֵ
//    CMsTreeIndex*   m_lpCMsTreeIndex;       // ��ͷָ��
//    CMsTreeNode**   m_lppTreeFurcation;     // �����ָ������
//    CMsTreeNode*    m_lpFatherTreeFurcation;// �����ָ��
//    DWORD           m_dwTreeFurcationCount; // ӵ������辸���
//    DWORD           m_dwUseCount;           // ���ڵ�Ȩ��(��ʹ�ö��ٴ�)
//};typedef CMsTreeNode* lpCMsTreeNode;
//
//// ��ͷ����
//class CMsTreeIndex
//{
//public:
//    enum E_ERROR_CODE
//    {
//        ERROR_UNKNOW = -1,          // δ֪����
//        ERROR_NO_ERROR = 0,         // �޴���
//        ERROR_PARAMETER_OVERFLOW,   // �������
//        ERROR_TREE_FULL,            // ������
//        ERROR_DATA_IS_EXIST,        // �����Ѵ���
//        ERROR_DATA_NOT_EXIST,       // ���ݲ�����
//    };
//    CMsTreeIndex(DWORD dwTreeFurcation, DWORD dwMaxNodeCount, DWORD dwBeginFlag);
//    ~CMsTreeIndex(VOID);
//    BOOL    AddData(LPBYTE lpByte, DWORD dwSize, LPVOID lpData);// ���Ԫ��
//    LPVOID  GetData(LPBYTE lpByte, DWORD dwSize);               // ��ȡԪ��
//    BOOL    DelData(LPBYTE lpByte, DWORD dwSize);               // ɾ��Ԫ��
//    E_ERROR_CODE    GetLastErrorCMsTreeIndex(VOID);             // ��ȡ�������
//private:
//    DWORD           m_dwTreeFurcation;  // ÿ���ڵ�ķ�辸���
//    DWORD           m_dwMaxNodeCount;   // ���������ڵ����
//    CMsTreeNode*    m_lpTreeFurcation;  // ��辽ڵ�����
//    DWORD           m_dwBeginFlag;      // ��ʼ���ֵ
//    CMsPoolManager  m_CMsNodePool;      // �ڵ����
//    CMsPoolManager  m_CMsDataPool;      // ���ݳ���
//    CMsPoolManager  m_CMsFurcationPool; // ��辳���
//    E_ERROR_CODE    m_eTreeErrorCode;   // ���������
//};

//����������������������
#endif//#pragma once//��
//����������������������
