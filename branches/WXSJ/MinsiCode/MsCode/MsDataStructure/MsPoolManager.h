#pragma once                //��//��
#ifndef __MSPOOLMANAGER_H__ //��//��
#define __MSPOOLMANAGER_H__ //��//��
//������������������������������//��
//�ļ����ƣ�MsPoolManager.h     //��
//���������������ع�����ط�װ  //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// �ع�����
class CMsPoolManager
{
public:
    CMsPoolManager(                 // ���캯��
        DWORD dwPoolMaxCount,
        BOOL bNoData = FALSE,
        BOOL bAutoInit = TRUE,
        BOOL bAutoLock = TRUE
        );

    ~CMsPoolManager(VOID);          // ��������

    VOID operator = (CMsPoolManager){AssertEx(AL_NORMAL, FALSE, _T("���ø�ֵ�����"));}
public:
    BOOL    Init(VOID);                             // ���ӹ�������ʼ������
    BOOL    Release(VOID);                          // �ͷų��ӹ�����
    BOOL    Reset(VOID);                            // ������������
    DWORD   GetCurrentCount(VOID);                  // ��ȡ��ǰԪ�ظ���
    DWORD   GetMaxCount(VOID);                      // ��ȡ���Ԫ�ظ���
    BOOL    IsFull(VOID);                           // �����Ƿ��Ѿ�����
    BOOL    IsEmpty(VOID);                          // �����Ƿ��Ѿ�����
    BOOL    Append(CPoolMember* lpData);            // ���Ԫ��
    BOOL    Remove(CPoolMember* lpData);            // �Ƴ�Ԫ��
    DWORD   GetNextIndex(BOOL bPossess = TRUE);     // ��ȡ����λ��
    BOOL    FreeIndexByPool(DWORD dwIndexInPool);   // ���ݳ����ͷ�����λ��
    BOOL    FreeIndexByLand(DWORD dwIndexInLand);   // ���ݵ�½�ͷ�����λ��
    BOOL    RemoveByPoolIndex(DWORD dwIndexInPool); // ���ݳ�������ɾ��
    BOOL    RemoveByLandIndex(DWORD dwIndexInLand); // ���ݵ�½����ɾ��
    VOID    PoolLock(VOID);                         // ���Ӽ���
    VOID    PoolUnLock(VOID);                       // ���ӽ���

    // ��Ѱ����ת���ɳ�������
    DWORD   LocateIndexToPoolIndex(DWORD dwIndexInLocate);

    // ��ȡ�������
    E_MS_ERROR      GetLastErrorCMsPoolManager(VOID);

    // ���ݳ���������ȡԪ��
    CPoolMember*    GetDataByPool(DWORD dwIndexByPool);

    // ���ݵ�½������ȡԪ��
    CPoolMember*    GetDataByLand(DWORD dwIndexByLand);

    // ���ݳ���������ȡԪ�ص�½ʱ��
    LPFILETIME      GetDataLandTime(DWORD dwIndexByPool);

private:
    MsLock          m_MsPoolLock;
    CONST DWORD     m_dwPoolMaxCount;       // �������ĳ�Ա����
    CPoolMember**   m_lppCPoolMember;       // ���ӳ�Ա����ָ��
    LPDWORD         m_lpdwLandArray;        // ���ӵ�½����
    DWORD           m_dwCurrentMemberCount; // ��ǰ���ӳ�Ա����
    SYSTEMTIME      m_stLandTime;           // ��½ʱ��
    E_MS_ERROR      m_ePoolErrorCode;       // ���Ӵ������
    BOOL            m_bInit;                // �Ƿ��Ѿ���ʼ��
    CONST BOOL      m_bLock;                // �Ƿ���ж��̼߳���
    BOOL            m_bNoData;              // �Ƿ�洢����
};

typedef CMsPoolManager* lpCMsPoolManager;
//����������������������
#endif//#pragma once//��
//����������������������
