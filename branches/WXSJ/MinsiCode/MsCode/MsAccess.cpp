#include "MFCPrecompiled.h"

// ���졢����
CMsAccess::CMsAccess(VOID)
: m_pCDaoDatabase   (NULL)
, m_pCDaoRecordset  (NULL)
, m_pCDaoException  (NULL)
, m_COleVariant     (m_ResultBuff)
{
    m_pCDaoDatabase = NEW CDaoDatabase;
    AssertEx(m_pCDaoDatabase, _T("�ڴ治��"));

    m_pCDaoRecordset = NEW CDaoRecordset(m_pCDaoDatabase);
    AssertEx(m_pCDaoRecordset, _T("�ڴ治��"));

    ZeroMemoryArray(m_ResultBuff);
}

CMsAccess::~CMsAccess(VOID)
{
    SAFE_DELETE(m_pCDaoRecordset);
    SAFE_DELETE(m_pCDaoDatabase);
}

// �����ݿ��ļ�
BOOL CMsAccess::Open(TCHAR* szDBName)
{
    _ENTER_FUN_C;

    BOOL bRet = m_pCDaoDatabase->IsOpen();
    if (bRet)
    {
        m_pCDaoDatabase->Close();
    }
    m_pCDaoDatabase->Open(szDBName);

    return m_pCDaoDatabase->IsOpen();;

    _DISPOSE_CUSTOM_END_C(CDaoException* pException)
    {
        m_pCDaoException = pException;
        fAssertEx(
            FALSE,
            _T("ִ��SQL������.\n������[%d]\nԴ[%s]\n����[%s]\n����[%s]"),
            m_pCDaoException->m_pErrorInfo->m_lErrorCode,
            m_pCDaoException->m_pErrorInfo->m_strSource,
            m_pCDaoException->m_pErrorInfo->m_strDescription,
            m_pCDaoException->m_pErrorInfo->m_strHelpFile
            );
    }
    _LEAVE_FUN;
    return FALSE;
}

// �ر����ݿ��ļ�
BOOL CMsAccess::Close(VOID)
{
    BOOL bRet = m_pCDaoDatabase->IsOpen();
    if (bRet)
    {
        m_pCDaoDatabase->Close();
    }
    bRet = m_pCDaoRecordset->IsOpen();
    if (bRet)
    {
        m_pCDaoRecordset->Close();
    }

    return !(m_pCDaoDatabase->IsOpen() || m_pCDaoRecordset->IsOpen());

}

// ִ��SQL���
VOID CMsAccess::ExecuteSQL(TCHAR* szSQLText)
{
    _ENTER_FUN_C;

    if (m_pCDaoRecordset->IsOpen())
    {
        m_pCDaoRecordset->Close();
    }
    m_pCDaoRecordset->Open(dbOpenDynaset, szSQLText);

    _DISPOSE_CUSTOM_END_C(CDaoException* pException)
    {
        m_pCDaoException = pException;
        fAssertEx(
            FALSE,
            _T("ִ��SQL������.\n������[%d]\nԴ[%s]\n����[%s]\n����[%s]"),
            m_pCDaoException->m_pErrorInfo->m_lErrorCode,
            m_pCDaoException->m_pErrorInfo->m_strSource,
            m_pCDaoException->m_pErrorInfo->m_strDescription,
            m_pCDaoException->m_pErrorInfo->m_strHelpFile
            );
    }
    _LEAVE_FUN;
}

// ��ȡ�����¼����
LONG CMsAccess::GetResultCount(VOID)
{
    LONG lCount = m_pCDaoRecordset->GetRecordCount();
    if (lCount)
    {
        m_pCDaoRecordset->MoveLast();
        m_pCDaoRecordset->MoveFirst();
        return m_pCDaoRecordset->GetRecordCount();
    }
    else
    {
        return 0;
    }
}

// ��ȡ����ֶθ���
SHORT CMsAccess::GetFieldCount(VOID)
{
    return m_pCDaoRecordset->GetFieldCount();
}

// ��һ��
VOID CMsAccess::MoveFirst(VOID)
{
    m_pCDaoRecordset->MoveFirst();
}

// ���һ��
VOID CMsAccess::MoveLast(VOID)
{
    m_pCDaoRecordset->MoveLast();
}

// ��һ��
VOID CMsAccess::MoveNext(VOID)
{
    m_pCDaoRecordset->MoveNext();
}

// ��һ��
VOID CMsAccess::MovePrev(VOID)
{
    m_pCDaoRecordset->MovePrev();
}

// ��ȡ�����¼,�����ֶ���
COleVariant* CMsAccess::GetResultValue(TCHAR* szFieldName)
{
    m_pCDaoRecordset->GetFieldValue(szFieldName, m_COleVariant);
    return &m_COleVariant;
}

// ��ȡ�����¼,�����ֶ�����
COleVariant* CMsAccess::GetResultValue(INT nIndex)
{
    m_pCDaoRecordset->GetFieldValue(nIndex, m_COleVariant);
    return &m_COleVariant;
}
