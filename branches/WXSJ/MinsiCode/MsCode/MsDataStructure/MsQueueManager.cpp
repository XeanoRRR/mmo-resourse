#include "Precompiled.h"
#include "MsPoolManager.h"

// �ع��������캯��
CMsQueueManager::CMsQueueManager(DWORD dwMaxCount, BOOL bAutoLock)
: m_dwQueueMemberMaxCount   (dwMaxCount)
, m_lppQueueMemberData      (NULL)
, m_lpdwQueueMemberSize     (NULL)
, m_dwQueueCurrentCount     (0)
, m_dwQueueCurrentFirstUse  (INVALID_UID)
, m_dwQueueCurrentFirstFree (0)
, m_lpLandTime              (NULL)
, m_eQueueErrorCode         (ERROR_NO_ERROR)
, m_bInit                   (FALSE)
, m_bLock                   (bAutoLock)

{
    _ENTER_FUN_C;

    ZeroMemoryStruct(m_stLandTime);

    AssertEx(AL_NORMAL, m_dwQueueMemberMaxCount, _T("���е����ߴ粻��Ϊ0,�������"));

    AssertEx(AL_NORMAL, this->Init(), _T("���г�ʼ��ʧ��"));

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}

// ���й�������������
CMsQueueManager::~CMsQueueManager(VOID)
{
    ;
}

// ���й�������ʼ������
BOOL CMsQueueManager::Init(VOID)
{
    _ENTER_FUN_C;

    // ������г�Ա����
    m_lppQueueMemberData = NEW LPVOID[m_dwQueueMemberMaxCount];
    AssertEx(AL_CATAST, m_lppQueueMemberData, _T("m_lppQueueMemberDataΪ��,�ڴ治��"));
    ZeroMemory(m_lppQueueMemberData, sizeof(LPVOID) * m_dwQueueMemberMaxCount);

    // ������г�Ա�ߴ�����
    m_lpdwQueueMemberSize = NEW DWORD[m_dwQueueMemberMaxCount];
    AssertEx(AL_CATAST, m_lpdwQueueMemberSize, _T("m_lpdwQueueMemberSizeΪ��,�ڴ治��"));
    ZeroMemory(m_lpdwQueueMemberSize, sizeof(DWORD) * m_dwQueueMemberMaxCount);

    m_lpLandTime = NEW FILETIME[m_dwQueueMemberMaxCount];
    AssertEx(AL_CATAST, m_lpLandTime, _T("m_lpdwQueueMemberSizeΪ��,�ڴ治��"));
    ZeroMemory(m_lpLandTime, sizeof(FILETIME) * m_dwQueueMemberMaxCount);

    m_eQueueErrorCode = ERROR_NO_ERROR;
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    m_eQueueErrorCode = ERROR_OPERATE_EXCEPTION;
    return FALSE;
}

// �ͷŶ��й�����
BOOL CMsQueueManager::Release(VOID)
{
    _ENTER_FUN_C;

    while (m_dwQueueCurrentCount)
    {
        this->DeleteData();
    }
    SAFE_DELETE_ARRAY(m_lppQueueMemberData);
    SAFE_DELETE_ARRAY(m_lpdwQueueMemberSize);
    SAFE_DELETE_ARRAY(m_lpLandTime);

    m_eQueueErrorCode = ERROR_NO_ERROR;
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    m_eQueueErrorCode = ERROR_OPERATE_EXCEPTION;
    return FALSE;
}

// ��ȡ��ǰԪ�ظ���
DWORD CMsQueueManager::GetCurrentCount(VOID)
{
    m_eQueueErrorCode = ERROR_NO_ERROR;
    return m_dwQueueCurrentCount;
}

// ����Ƿ��Ѿ�����
BOOL CMsQueueManager::IsFull(VOID)
{
    m_eQueueErrorCode = ERROR_NO_ERROR;
    return m_dwQueueCurrentCount >= m_dwQueueMemberMaxCount;
}

// ����Ƿ��Ѿ�����
BOOL CMsQueueManager::IsEmpty(VOID)
{
    m_eQueueErrorCode = ERROR_NO_ERROR;
    return m_dwQueueCurrentCount ? FALSE : TRUE;
}

// ѹ������(���ݿ��ڲ��洢)
BOOL CMsQueueManager::PushData(LPVOID lpData, DWORD dwSize)
{
    _ENTER_FUN_C;

    AssertEx(AL_NORMAL, FALSE, _T("�ֲ����ڴ�����,�˺���������"));
    if (this->IsFull())
    {
        m_eQueueErrorCode = ERROR_ISFULL;
        return FALSE;
    }

    lpData = NEW BYTE[dwSize];
    AssertEx(AL_CATAST, lpData, _T("lpDataΪ��,�ڴ治��"));

    BOOL bRet = this->PushData(lpData);
    if (bRet)
    {
        m_lpdwQueueMemberSize[m_dwQueueCurrentFirstFree] = dwSize;
        m_eQueueErrorCode = ERROR_NO_ERROR;
    }
    return bRet;

    _LEAVE_FUN_DISPOSE_END_C;
    m_eQueueErrorCode = ERROR_OPERATE_EXCEPTION;
    return FALSE;
}

// ѹ������(���ݿ��ⲿ�洢)
BOOL CMsQueueManager::PushData(LPVOID lpData)
{
    if (this->IsFull())
    {
        m_eQueueErrorCode = ERROR_ISFULL;
        return FALSE;
    }

    // �����ݷ��������м�¼
    m_lppQueueMemberData[m_dwQueueCurrentFirstFree] = lpData;
    m_lpdwQueueMemberSize[m_dwQueueCurrentFirstFree] = 0;
    ::GetLocalTime(&m_stLandTime);
    ::SystemTimeToFileTime(
        &m_stLandTime,
        &m_lpLandTime[m_dwQueueCurrentFirstFree]
    );

    m_dwQueueCurrentFirstFree++;
    // ����Ѿ������һ���ռ�,ѭ����ͷ������ʹ��
    if (m_dwQueueCurrentFirstFree >= (m_dwQueueMemberMaxCount - 1))
    {
        m_dwQueueCurrentFirstFree = 0;
    }

    // ��������
    m_dwQueueCurrentCount++;

    // ���֮ǰû������,����һ������λ�ö�λ
    if (INVALID_NID == m_dwQueueCurrentFirstUse)
    {
        m_dwQueueCurrentFirstUse = 0;
    }

    m_eQueueErrorCode = ERROR_NO_ERROR;
    return TRUE;
}

// ��ȡ����(�����ⲿ�洢ʱ,�ߴ�Ϊ0)
LPVOID CMsQueueManager::GetData(DWORD& dwSize)
{
    if (this->IsEmpty())
    {
        m_eQueueErrorCode = ERROR_ISEMPTY;
        return NULL;
    }
    AssertEx(
		AL_NORMAL,
        (0 == m_lpdwQueueMemberSize[m_dwQueueCurrentFirstUse]),
        _T("�ֲ����ڴ�����,�˺���������")
        );

    dwSize = m_lpdwQueueMemberSize[m_dwQueueCurrentFirstUse];

    LPVOID lpRet = m_lppQueueMemberData[m_dwQueueCurrentFirstUse];

    //m_dwQueueCurrentFirstUse++;
    //// ����Ѿ������һ���ռ�,ѭ����ͷ������ʹ��
    //if (m_dwQueueCurrentFirstUse >= (m_dwQueueMemberMaxCount - 1))
    //{
    //    m_dwQueueCurrentFirstUse = 0;
    //}

    return lpRet;
}

// ɾ������
VOID CMsQueueManager::DeleteData(VOID)
{
    if (this->IsEmpty())
    {
        m_eQueueErrorCode = ERROR_ISEMPTY;
        return;
    }

    if (0 != m_lpdwQueueMemberSize[m_dwQueueCurrentFirstUse])
    {
        AssertEx(AL_NORMAL, FALSE, _T("�ֲ����ڴ�����,�˺���������"));
        SAFE_DELETE_ARRAY(m_lppQueueMemberData[m_dwQueueCurrentFirstUse]);
        m_lpdwQueueMemberSize[m_dwQueueCurrentFirstUse] = 0;
    }
    if (INVALID_NID != m_dwQueueCurrentFirstUse)
    {
        m_lppQueueMemberData[m_dwQueueCurrentFirstUse] = NULL;
        m_dwQueueCurrentCount--;
        //if (m_dwQueueCurrentCount > 0)
        {
            m_dwQueueCurrentFirstUse++;
            // ����Ѿ������һ���ռ�,ѭ����ͷ������ʹ��
            if (m_dwQueueCurrentFirstUse >= (m_dwQueueMemberMaxCount - 1))
            {
                m_dwQueueCurrentFirstUse = 0;
            }
        }
        //else
        //{
            //m_dwQueueCurrentFirstUse = INVALID_NID;
        //}
    }
}

// ��������
VOID CMsQueueManager::QueueLock(VOID)
{
    m_eQueueErrorCode = ERROR_NO_ERROR;
    m_MsQueueLock.Lock();
}

// ��������
VOID CMsQueueManager::QueueUnLock(VOID)
{
    m_eQueueErrorCode = ERROR_NO_ERROR;
    m_MsQueueLock.UnLock();
}

// ��ȡ�������
CMsQueueManager::E_ERROR_CODE
CMsQueueManager::GetLastErrorCMsQueueManager(VOID)
{
    IS_AUTO_LOCK(m_MsQueueLock, m_bLock);
    return m_eQueueErrorCode;
}

// ��ȡ�¸���������λ��
ULONG_PTR CMsQueueManager::GetNextIndex(BOOL bPossess)
{
    _ENTER_FUN_C;

    // ռ��ģʽ
    if (bPossess)
    {
        IS_AUTO_LOCK(m_MsQueueLock, m_bLock);

        // �������Ƿ�����
        if (this->IsFull())
        {
            m_eQueueErrorCode = ERROR_ISFULL;
            // �������ʧ��
            return INVALID_UID;
        }

        ULONG_PTR ulFreeIndex = m_dwQueueCurrentFirstFree;
        this->PushData((LPVOID)ulFreeIndex);
        return ulFreeIndex;
    }

    // ��ռ��ģʽ
    else
    {
        IS_AUTO_LOCK(m_MsQueueLock, m_bLock);

        // �������Ƿ�����
        if (this->IsFull())
        {
            m_eQueueErrorCode = ERROR_ISFULL;
            // �������ʧ��
            return INVALID_UID;
        }

        m_eQueueErrorCode = ERROR_NO_ERROR;
        return m_dwQueueCurrentFirstFree;
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_eQueueErrorCode = ERROR_OPERATE_EXCEPTION;
    return INVALID_UID;
}

// �����������
BOOL CMsQueueManager::ClearAll(VOID)
{
    while (m_dwQueueCurrentCount)
    {
        this->DeleteData();
    }
    return TRUE;
}
