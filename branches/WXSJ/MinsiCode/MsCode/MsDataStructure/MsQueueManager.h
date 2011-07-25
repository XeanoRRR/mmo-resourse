#pragma once                //��//��
#ifndef __MSQUEUEMANAGER_H__//��//��
#define __MSQUEUEMANAGER_H__//��//��
//������������������������������//��
//�ļ����ƣ�MsQueueManager.h    //��
//�������������й�����ط�װ    //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// �ع�����
class CMsQueueManager
{
public:
    enum E_ERROR_CODE
    {
        ERROR_UNKNOW = -1,          // δ֪����
        ERROR_NO_ERROR = 0,         // �޴���
        ERROR_NO_INIT,              // ����δ��ʼ��
        ERROR_ISFULL,               // ��������
        ERROR_ISEMPTY,              // ���п���
        ERROR_OPERATE_EXCEPTION,    // �����쳣
    };

    // ���캯��
    CMsQueueManager(DWORD dwMaxCount, BOOL bAutoLock = TRUE);

    // ��������
    ~CMsQueueManager(VOID);

    VOID operator = (CMsQueueManager){AssertEx(AL_NORMAL, FALSE, _T("���ø�ֵ�����"));}
public:
    BOOL    Init(VOID);                                 // ���ӹ�������ʼ������
    BOOL    Release(VOID);                              // �ͷų��ӹ�����
    DWORD   GetCurrentCount(VOID);                      // ��ȡ��ǰԪ�ظ���
    BOOL    IsFull(VOID);                               // ����Ƿ��Ѿ�����
    BOOL    IsEmpty(VOID);                              // ����Ƿ��Ѿ�����
    BOOL    ClearAll(VOID);

    // ѹ������(���ݿ��ڲ��洢)
    BOOL    PushData(LPVOID lpData, DWORD dwSize);

    // ѹ������(���ݿ��ⲿ�洢)
    BOOL    PushData(LPVOID lpData);
    
    // ��ȡ����(�����ⲿ�洢ʱ,�ߴ�Ϊ0)
    LPVOID          GetData(DWORD& dwSize);
    VOID            DeleteData(VOID);                   // ɾ������
    VOID            QueueLock(VOID);                    // ��������
    VOID            QueueUnLock(VOID);                  // ��������
    ULONG_PTR       GetNextIndex(BOOL bPossess = TRUE); // ��ȡ�¸���������λ��
    E_ERROR_CODE    GetLastErrorCMsQueueManager(VOID);  // ��ȡ�������

private:
    MsLock          m_MsQueueLock;              // ���з�����
    CONST DWORD     m_dwQueueMemberMaxCount;    // ���г�Ա������
    LPVOID*         m_lppQueueMemberData;       // ���г�Ա����
    LPDWORD         m_lpdwQueueMemberSize;      // ���г�Ա�ߴ�����
    DWORD           m_dwQueueCurrentCount;      // ���е�ǰ��Ա����
    DWORD           m_dwQueueCurrentFirstUse;   // ���е�ǰ��һ������λ��
    DWORD           m_dwQueueCurrentFirstFree;  // ���е�ǰ��һ������λ��
    LPFILETIME      m_lpLandTime;               // ���г�Ա��½ʱ������
    SYSTEMTIME      m_stLandTime;               // ��½ʱ��
    E_ERROR_CODE    m_eQueueErrorCode;          // ���д������
    BOOL            m_bInit;                    // �Ƿ��Ѿ���ʼ��
    CONST BOOL      m_bLock;                    // �Ƿ���ж��̼߳���
};

//����������������������
#endif//#pragma once//��
//����������������������
