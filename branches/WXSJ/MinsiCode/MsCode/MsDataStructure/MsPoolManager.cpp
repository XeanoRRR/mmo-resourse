#include "Precompiled.h"
#include "MsPoolManager.h"

// �ع��������캯��
CMsPoolManager::CMsPoolManager(
    DWORD dwPoolMaxCount,
    BOOL bNoData,
    BOOL bAutoInit,
    BOOL bAutoLock
    )
: m_lppCPoolMember          (NULL)
, m_dwPoolMaxCount          (dwPoolMaxCount)
, m_dwCurrentMemberCount    (0)
, m_lpdwLandArray           (NULL)
, m_ePoolErrorCode          (MS_ERROR_SUCCEED)
, m_bInit                   (FALSE)
, m_bLock                   (bAutoLock)
, m_bNoData                 (bNoData)
{
    _ENTER_FUN_C;

    AssertEx(AL_NORMAL, m_dwPoolMaxCount, _T("�ص����ߴ粻��Ϊ0,�������"));

    ZeroMemoryStruct(m_stLandTime);

    m_lppCPoolMember = NEW CPoolMember*[m_dwPoolMaxCount];
    if (!m_bNoData)
    {
        ZeroMemory(m_lppCPoolMember, sizeof(CPoolMember*) * m_dwPoolMaxCount);
    }
    else
    {
        if (m_lppCPoolMember)
        {
            FOR_x_TO(i, m_dwPoolMaxCount)
            {
                m_lppCPoolMember[i] = NEW CNoDataPoolMember;
                AssertEx(
					AL_CATAST,
                    m_lppCPoolMember[i],
                    _T("m_lppCPoolMember[i]Ϊ��,�ڴ治��")
                    );
            }
        }
        else
        {
            AssertEx(AL_CATAST, m_lppCPoolMember, _T("m_lpCPoolMemberΪ��,�ڴ治��"));
        }
    }

    m_lpdwLandArray = NEW DWORD[m_dwPoolMaxCount];
    AssertEx(AL_CATAST, m_lpdwLandArray, _T("m_lpdwLandArrayΪ��,�ڴ治��"));
    ZeroMemory(m_lpdwLandArray, sizeof(DWORD) * m_dwPoolMaxCount);

    if (bAutoInit)
    {
        this->Init();
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}

// ���ӹ�������������
CMsPoolManager::~CMsPoolManager(VOID)
{
    _ENTER_FUN_C;

    this->Release();

    SAFE_DELETE_ARRAY(m_lpdwLandArray);
    if (!m_bNoData)
    {
        SAFE_DELETE_ARRAY(m_lppCPoolMember);
    }
    else
    {
        FOR_x_TO(i, m_dwPoolMaxCount)
        {
            SAFE_DELETE(m_lppCPoolMember[i]);
        }
        SAFE_DELETE_ARRAY(m_lppCPoolMember);
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
}

// ���ӹ�������ʼ������
BOOL CMsPoolManager::Init(VOID)
{
    _ENTER_FUN_C;

    if (m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_SUCCEED;
        return TRUE;
    }

    // ѭ����ʼ��״̬������ÿ��λ�ö�����
    for (DWORD i = 0; i < m_dwPoolMaxCount; i++)
    {
        // ʹ״̬������ÿ��λ�ö�ָ����������Ķ�Ӧλ��
        m_lpdwLandArray[i] = i;
    }

    // ���سɹ�
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    return m_bInit = TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

// �ͷų��ӹ�����
BOOL CMsPoolManager::Release(VOID)
{
    _ENTER_FUN_C;

    // ȥ����ʼ�����
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    m_dwCurrentMemberCount = 0;
    ZeroMemoryStruct(m_stLandTime);
    ZeroMemory(m_lpdwLandArray, sizeof(DWORD) * m_dwPoolMaxCount);
    m_bInit = FALSE;
    return !m_bInit;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

// ������������
BOOL CMsPoolManager::Reset(VOID)
{
    this->Release();
    return this->Init();
}


// ��ȡ��ǰԪ�ظ���
DWORD CMsPoolManager::GetCurrentCount(VOID)
{
    if (!m_bInit)
    {
        // ������δ��ʼ��
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return INVALID_UID;
    }
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    return m_dwCurrentMemberCount;
}

// ��ȡ���Ԫ�ظ���
DWORD CMsPoolManager::GetMaxCount(VOID)
{
    return m_dwPoolMaxCount;
}

// �����Ƿ��Ѿ�����
BOOL CMsPoolManager::IsFull(VOID)
{
    if (!m_bInit)
    {
        // ������δ��ʼ��
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return INVALID_NID;
    }
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    return (m_dwCurrentMemberCount < m_dwPoolMaxCount) ?  FALSE : TRUE;
}

// �����Ƿ��Ѿ�����
BOOL CMsPoolManager::IsEmpty(VOID)
{
    if (!m_bInit)
    {
        // ������δ��ʼ��
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return INVALID_NID;
    }
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    return m_dwCurrentMemberCount ? FALSE : TRUE;
}

// ���Ԫ�ص�����
BOOL CMsPoolManager::Append(CPoolMember* lpData)
{
    if (!m_bInit)
    {
        // ������δ��ʼ��
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return INVALID_NID;
    }
    _ENTER_FUN_C;

    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);
    AssertEx(AL_NORMAL, lpData, _T("lpData����Ϊ��"));

    // �������Ƿ�����
    if (this->IsFull())
    {
        m_ePoolErrorCode = MS_ERROR_BUFFER_NOT_ENOUGH_FREE;
        // �������ʧ��
        return FALSE;
    }

    // ��ǰ���õĳ�������
    DWORD dwCurrentLandIndexByUseableness = 
        m_lpdwLandArray[m_dwCurrentMemberCount];

    // ��¼���ݵĵ�ַ
    m_lppCPoolMember[dwCurrentLandIndexByUseableness] = lpData;

    // �������еĳ�����Ϣд��
    if (lpData)
    {
        lpData->m_dwIndexInPool     = dwCurrentLandIndexByUseableness;
        lpData->m_dwIndexInLocate   = m_dwCurrentMemberCount++;
        ::GetLocalTime(&m_stLandTime);
        ::SystemTimeToFileTime(
            &m_stLandTime,
            (LPFILETIME)&lpData->m_n64EnterIndex
            );

        m_ePoolErrorCode = MS_ERROR_SUCCEED;
        return TRUE;
    }
    else
    {
        AssertEx(AL_NORMAL, lpData,_T("lpDataΪ��"));
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

// ɾ��Ԫ�ص�ַ������
BOOL CMsPoolManager::Remove(CPoolMember* lpData)
{
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return INVALID_NID;
    }
    _ENTER_FUN_C;

    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);
    if (lpData == m_lppCPoolMember[lpData->m_dwIndexInPool])
    {
        return this->RemoveByPoolIndex(lpData->m_dwIndexInPool);
    }
    else
    {
        return FALSE;
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

// ���ݳ�������ɾ��
BOOL CMsPoolManager::RemoveByPoolIndex(DWORD dwIndexInPool)
{
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return FALSE;
    }
    _ENTER_FUN_C;

    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);

    // �����Ƿ��Ѿ�����
    if (this->IsEmpty())
    {
        m_ePoolErrorCode = MS_ERROR_BUFFER_NOT_ENOUGH_DATA;
        // ����ɾ��ʧ��
        return FALSE;
    }

    // ��ȡҪɾ����Ԫ��
    CPoolMember* lpCPoolMember  = m_lppCPoolMember[dwIndexInPool];

    // ��ȡҪɾ��Ԫ�����ڵĵ�½λ��
    DWORD dwIndexInLand         = lpCPoolMember->m_dwIndexInLocate;

    // ��֤��������ֵ�봫��ֵ�Ƿ�ƥ��
    AssertEx(
		AL_NORMAL,
        dwIndexInPool == m_lpdwLandArray[dwIndexInLand],
        _T("�����쳣")
        );

    // ʵ�ֽ���ɾ����Ԫ�������һ������Ԫ�ص�������Ϣ������
    // �������һ��������Ϣ�ͷš�
    {
        // ��Ԫ���еĳ�����Ϣ���
        lpCPoolMember->m_dwIndexInLocate    = INVALID_UID;
        lpCPoolMember->m_dwIndexInPool      = INVALID_UID;
        lpCPoolMember->m_n64EnterIndex      = INVALID_NID;

        // ��ǰ���ټ�����Ϊ��ֱ�Ӷ�λ�����һ������Ԫ��,
        // �����ظ��������Ч�ʡ�
        m_dwCurrentMemberCount--;   // ����һ����Ա����

        // ���ƶ���Ԫ�صĳ�������(��½���������ߵ����һ��Ԫ��)
        DWORD dwPoolIndexByMotion = m_lpdwLandArray[m_dwCurrentMemberCount];

        // ����ɾ��Ԫ�صĳ��������޸ĳ����һ����½��Ԫ�صĳ�������
        m_lpdwLandArray[dwIndexInLand]  = dwPoolIndexByMotion;

        // �����ƶ���Ԫ���еĳ�����Ϣ����
        m_lppCPoolMember[dwPoolIndexByMotion]->m_dwIndexInLocate =
            dwIndexInLand;

        // ������¼��Ԫ�صĳ��������޸ĳɱ�ɾ��Ԫ�صĳ�������
        m_lpdwLandArray[m_dwCurrentMemberCount] = dwIndexInPool;

        if (!m_bNoData)
        {
            // ��Ԫ�������е�ָ�����
            m_lppCPoolMember[dwIndexInPool] = NULL;
        }
    }

    // ���سɹ�
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

BOOL CMsPoolManager::RemoveByLandIndex(DWORD dwIndexInLand)
{
    _ENTER_FUN_C;

    // ���Ҫɾ��������λ���Ƿ����
    AssertEx(
		AL_NORMAL,
		(dwIndexInLand < m_dwCurrentMemberCount),
        _T("dwLandIndexֵ�쳣,���ô���")
        );

    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);

    // ��ӵ��ó�����������ɾ��
    return this->RemoveByPoolIndex(m_lpdwLandArray[dwIndexInLand]);

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

DWORD CMsPoolManager::GetNextIndex(BOOL bPossess)
{
    _ENTER_FUN_C;
    AssertEx(AL_NORMAL, m_bNoData, _T("���ô���,�˺�����������˵���->��ʹ��������ģʽ"));
    if (!m_bInit)
    {
        // ������δ��ʼ��
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return INVALID_UID;
    }

    // ռ��ģʽ
    if (bPossess)
    {
        IS_AUTO_LOCK(m_MsPoolLock, m_bLock);

        // �������Ƿ�����
        if (this->IsFull())
        {
            m_ePoolErrorCode = MS_ERROR_BUFFER_NOT_ENOUGH_FREE;
            // �������ʧ��
            return INVALID_UID;
        }

        // ��ǰ���õĳ�������
        DWORD dwCurrentLandIndexByUseableness =
            m_lpdwLandArray[m_dwCurrentMemberCount];

        // ��¼���ݵĵ�ַ
        CPoolMember* lpData =
            m_lppCPoolMember[dwCurrentLandIndexByUseableness];

        // �������еĳ�����Ϣд��
        lpData->m_dwIndexInPool     = dwCurrentLandIndexByUseableness;
        lpData->m_dwIndexInLocate   = m_dwCurrentMemberCount++;
        ::GetLocalTime(&m_stLandTime);
        ::SystemTimeToFileTime(
            &m_stLandTime,
            (LPFILETIME)&lpData->m_n64EnterIndex
            );

        m_ePoolErrorCode = MS_ERROR_SUCCEED;
        return dwCurrentLandIndexByUseableness;
    }

    // ��ռ��ģʽ
    else
    {
        IS_AUTO_LOCK(m_MsPoolLock, m_bLock);

        // �������Ƿ�����
        if (this->IsFull())
        {
            m_ePoolErrorCode = MS_ERROR_BUFFER_NOT_ENOUGH_FREE;
            // �������ʧ��
            return INVALID_UID;
        }
        // ��ǰ���õĳ�������
        m_ePoolErrorCode = MS_ERROR_SUCCEED;
        return m_lpdwLandArray[m_dwCurrentMemberCount];
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return INVALID_UID;
}

// ���ݳ����ͷ�����λ��
BOOL CMsPoolManager::FreeIndexByPool(DWORD dwIndexInPool)
{
    _ENTER_FUN_C;
    AssertEx(AL_NORMAL, m_bNoData, _T("���ô���,�˺�����������˵���->��ʹ��������ģʽ"));
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return FALSE;
    }

    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);

    // �����Ƿ��Ѿ�����
    if (this->IsEmpty())
    {
        m_ePoolErrorCode = MS_ERROR_BUFFER_NOT_ENOUGH_DATA;
        // ����ɾ��ʧ��
        return FALSE;
    }

    // ��ȡҪɾ����Ԫ��
    CPoolMember* lpCPoolMember  = m_lppCPoolMember[dwIndexInPool];

    // ��ȡҪɾ��Ԫ�����ڵĵ�½λ��
    DWORD dwIndexInLand         = lpCPoolMember->m_dwIndexInLocate;

    // ��֤��������ֵ�봫��ֵ�Ƿ�ƥ��
    AssertEx(
		AL_NORMAL,
        dwIndexInPool == m_lpdwLandArray[dwIndexInLand],
        _T("�����쳣")
        );

    // ʵ�ֽ���ɾ����Ԫ�������һ������Ԫ�ص�������Ϣ������
    // �������һ��������Ϣ�ͷš�
    {
        // ��Ԫ���еĳ�����Ϣ���
        lpCPoolMember->m_dwIndexInLocate    = INVALID_UID;
        lpCPoolMember->m_dwIndexInPool      = INVALID_UID;
        lpCPoolMember->m_n64EnterIndex      = INVALID_NID;

        // ��ǰ���ټ�����Ϊ��ֱ�Ӷ�λ�����һ������Ԫ��,
        // �����ظ��������Ч�ʡ�
        m_dwCurrentMemberCount--;   // ����һ����Ա����

        // ���ƶ���Ԫ�صĳ�������(��½���������ߵ����һ��Ԫ��)
        DWORD dwPoolIndexByMotion = m_lpdwLandArray[m_dwCurrentMemberCount];

        // ����ɾ��Ԫ�صĳ��������޸ĳ����һ����½��Ԫ�صĳ�������
        m_lpdwLandArray[dwIndexInLand]  = dwPoolIndexByMotion;

        // �����ƶ���Ԫ���еĳ�����Ϣ����
        m_lppCPoolMember[dwPoolIndexByMotion]->m_dwIndexInLocate =
            dwIndexInLand;

        // ������¼��Ԫ�صĳ��������޸ĳɱ�ɾ��Ԫ�صĳ�������
        m_lpdwLandArray[m_dwCurrentMemberCount] = dwIndexInPool;
    }

    // ���سɹ�
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

// ���ݵ�½�ͷ�����λ��
BOOL CMsPoolManager::FreeIndexByLand(DWORD dwIndexInLand)
{
    _ENTER_FUN_C;
    AssertEx(AL_NORMAL, m_bNoData, _T("���ô���,�˺�����������˵���->��ʹ��������ģʽ"));

    // ���Ҫɾ��������λ���Ƿ����
    AssertEx(
		AL_NORMAL,
        (dwIndexInLand < m_dwCurrentMemberCount),
        _T("dwLandIndexֵ�쳣,���ô���")
        );

    // ��ӵ��ó�����������ɾ��
    return this->FreeIndexByPool(m_lpdwLandArray[dwIndexInLand]);

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    m_ePoolErrorCode = MS_ERROR_EXCEPTION;
    return FALSE;
}

// ���ݳ���������ȡԪ��
CPoolMember* CMsPoolManager::GetDataByPool(DWORD dwIndexByPool)
{
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return NULL;
    }
    _ENTER_FUN_C;

    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);

    // �����Ƿ��Ѿ�����
    if (this->IsEmpty())
    {
        m_ePoolErrorCode = MS_ERROR_BUFFER_NOT_ENOUGH_DATA;
        // ����ɾ��ʧ��
        return NULL;
    }

    CPoolMember* pMember = m_lppCPoolMember[dwIndexByPool];

    // ���ָ��λ�õ�������Ч
    if (INVALID_NID == pMember->m_dwIndexInPool ||
        INVALID_NID == pMember->m_dwIndexInLocate)
    {
        m_ePoolErrorCode = MS_ERROR_INVALID_PARAMETER;
        return NULL;
    }
    else
    {
        m_ePoolErrorCode = MS_ERROR_SUCCEED;
        return pMember;
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return NULL;
}

// ���ݵ�½������ȡԪ��
CPoolMember* CMsPoolManager::GetDataByLand(DWORD dwIndexByLand)
{
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return NULL;
    }
    _ENTER_FUN_C;

    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);

    // �����Ƿ��Ѿ�����
    if (this->IsEmpty())
    {
        m_ePoolErrorCode = MS_ERROR_BUFFER_NOT_ENOUGH_DATA;
        // ����ɾ��ʧ��
        return NULL;
    }

    CPoolMember* pMember = m_lppCPoolMember[m_lpdwLandArray[dwIndexByLand]];

    // ���ָ��λ�õ�������Ч
    if (INVALID_NID == pMember->m_dwIndexInPool ||
        INVALID_NID == pMember->m_dwIndexInLocate)
    {
        m_ePoolErrorCode = MS_ERROR_INVALID_PARAMETER;
        return NULL;
    }
    else
    {
        m_ePoolErrorCode = MS_ERROR_SUCCEED;
        return pMember;
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return NULL;
}

// ��ȡ�������
E_MS_ERROR CMsPoolManager::GetLastErrorCMsPoolManager(VOID)
{
    IS_AUTO_LOCK(m_MsPoolLock, m_bLock);
    return m_ePoolErrorCode;
}

VOID CMsPoolManager::PoolLock(VOID)
{
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
    }
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    m_MsPoolLock.Lock();
}

VOID CMsPoolManager::PoolUnLock(VOID)
{
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
    }
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    m_MsPoolLock.UnLock();
}

// ��Ѱ����ת���ɳ�������
DWORD CMsPoolManager::LocateIndexToPoolIndex(DWORD dwIndexInLand)
{
    if (!m_bInit)
    {
        m_ePoolErrorCode = MS_ERROR_NO_INIT;
        //AssertEx(AL_NORMAL, FALSE, _T("���ӹ�����δ��ʼ��"));
        return INVALID_UID;
    }
    m_ePoolErrorCode = MS_ERROR_SUCCEED;
    return m_lpdwLandArray[dwIndexInLand];
}

// ���ݳ���������ȡԪ�ص�½ʱ��
LPFILETIME CMsPoolManager::GetDataLandTime(DWORD dwIndexByPool)
{
    CPoolMember* pMember = m_lppCPoolMember[dwIndexByPool];

    // ���ָ��λ�õ�������Ч
    if (INVALID_NID == pMember->m_dwIndexInPool ||
        INVALID_NID == pMember->m_dwIndexInLocate)
    {
        m_ePoolErrorCode = MS_ERROR_INVALID_PARAMETER;
        return NULL;
    }
    else
    {
        m_ePoolErrorCode = MS_ERROR_SUCCEED;
        return pMember->GetLandTime();
    }
}
