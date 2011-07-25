#include "Precompiled.h"
#include "MsFindFileManager.h"

CMsFindFileManager::CMsFindFileManager(VOID)
:m_nRootPathLen(0)
,m_nFileTypeLen(0)
,m_nRealPathLen(0)
{
    _ENTER_FUN_C;

    // ��ʼ����Ҫ������·��
    ZeroMemory(m_szRootPath, MAX_PATH * sizeof(TCHAR));

    // ��ʼ����ǰ·��
    ZeroMemory(m_szCurrentDirectory, MAX_PATH * sizeof(TCHAR));

    // ��ʼ����Ҫ���ҵ��ļ�����
    ZeroMemory(m_szFileType, MAX_PATH);

    // ��ʼ����Ҫ���ҵľ���·�������͵��ַ���
    ZeroMemory(m_szRealFindPathName, MAX_PATH);

    // ��ʼ�����ҽ���ṹ��
    ZeroMemory(&m_FindFileData, sizeof(LPWIN32_FIND_DATA));

    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,����or��������Ҫ����ֵ*/;
}

CMsFindFileManager::~CMsFindFileManager(VOID)
{
    _ENTER_FUN_C;

   _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,����or��������Ҫ����ֵ*/;
}

// ����������Ŀ¼
BOOL CMsFindFileManager::SetRootPath(CONST TCHAR* strPath)
{
    _ENTER_FUN_C;

    // �����Ĳ���Ŀ¼
    CONST TCHAR* strRealPath = NULL;

    // ���ʹ��Ĭ�ϲ������ҵ�ǰ����Ŀ¼
    if (!strPath)
    {
        // ���õ�ǰ����Ŀ¼Ϊ����Ŀ¼
        ::GetCurrentDirectory(MAX_PATH, m_szCurrentDirectory);
        _tcsncpy_s(
            m_szRootPath,
            MAX_PATH,
            m_szCurrentDirectory,
            (INT)_tcslen(m_szCurrentDirectory)
            );

        strRealPath = m_szRootPath;
    }
    else
    {
        // ��鴫�����,���ô���Ŀ¼Ϊ����Ŀ¼
        AssertEx(AL_SHOW_ONLY, strPath, _T("�����������,�������"));
        strRealPath = strPath;
    }

    // ��ȡ·������
    m_nRootPathLen = (INT)_tcslen(strRealPath);
    AssertEx(AL_SHOW_ONLY, m_nRootPathLen < MAX_PATH - 2, _T("·�����ȴ���,���󴫲�"));

    // ���Ŀ¼�Ƿ����
    ::GetCurrentDirectory(MAX_PATH, m_szCurrentDirectory);
    AssertEx(AL_SHOW_ONLY, ::SetCurrentDirectory(strRealPath), _T("·��������,�������"));
    ::SetCurrentDirectory(m_szCurrentDirectory), _T("·��������,�������");

    // ��������Ŀ¼����Ա
    if (m_szRootPath != strRealPath)
    {
        _tcsncpy_s(m_szRootPath, MAX_PATH, strRealPath, m_nRootPathLen);
    }

    // ��Ŀ¼·����׷��'\'
    AppPath(m_szRootPath);
    m_nRootPathLen++;

    // ��������Ŀ¼�����ղ����ַ���
    _tcsncpy_s(m_szRealFindPathName, MAX_PATH, m_szRootPath, m_nRootPathLen);

    // �����ַ������׷��'*'
    AppPath(m_szRealFindPathName, _T('*'));

    // ��ȡ���ղ����ַ�������
    m_nRealPathLen = (INT)_tcslen(m_szRealFindPathName);

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,�쳣����ʧ��*/;
    return FALSE;
}

// ���������ļ�����
BOOL CMsFindFileManager::SetFindFileType(CONST TCHAR* strFileType)
{
    _ENTER_FUN_C;

    if (strFileType)
    {
        // ��ȡ���ļ����͵ĳ���
        m_nFileTypeLen =  (INT)_tcslen(strFileType);
        AssertEx(AL_SHOW_ONLY, m_nFileTypeLen < MAX_PATH - 2, _T("m_nFileTypeLen�쳣,���󴫲�"));

        // �������ļ����͵���Ա
        _tcsncpy_s(m_szFileType, MAX_PATH, strFileType, m_nFileTypeLen);
    }
    else
    {
        // �������Ƿ�����
        AssertEx(AL_SHOW_ONLY, strFileType, _T("strFileTypeΪ��,���󴫲�"));
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,�쳣����ʧ��*/;
    return FALSE;
}

// ����Ŀ¼����ָ�����͵��ļ�
BOOL CMsFindFileManager::BrowsePath(VOID)
{
    _ENTER_FUN_C;

    // ������ղ����ַ�������
    AssertEx(AL_SHOW_ONLY, m_nRealPathLen, _T("��Ҫ���ҵ�Ŀ¼����,�������"));

    // ���Ȳ��ҵ�һ���ļ�
    HANDLE hFindFile = ::FindFirstFile(m_szRealFindPathName, &m_FindFileData);
    do
    {
        // ����ļ������Ƿ�ɹ�
        AssertEx(
			AL_SHOW_ONLY,
            (INVALID_HANDLE_VALUE != hFindFile),
            _T("FindFirstFileʧ��,����ʧ��")
            );

        // �ж��ҵ�������Ǹ���Ŀ¼
        if (FILE_ATTRIBUTE_DIRECTORY & m_FindFileData.dwFileAttributes)
        {
            // ������ҵ�����Ŀ¼Ϊ"." �� ".." ��ô��������
            if(*m_FindFileData.cFileName == '.') 
            { 
                continue;
            } 
            else
            {
                // ���������ַ�������
                TCHAR   szRealFindPathName_Bk[MAX_PATH] = {0};

                // ���ݱ��������ַ���
                _tcsncpy_s(
                    szRealFindPathName_Bk,
                    MAX_PATH,
                    m_szRealFindPathName,
                    m_nRealPathLen
                    );

                // ���������ַ������ȱ���
                INT     nRealFindPathNameLen_Bk = 
                    (INT)_tcslen(szRealFindPathName_Bk);

                // ȥ�����һλ��'*'
                if (m_nRealPathLen < MAX_PATH)
                {
                    if (m_szRealFindPathName[m_nRealPathLen - 1] == '*')
                    {
                        m_szRealFindPathName[m_nRealPathLen - 1] = '\0';
                        m_nRealPathLen--;
                    }
                    else
                    {
                        AssertEx(AL_CATAST, FALSE, _T("�����쳣,�����߼�"));
                    }
                }
                else
                {
                    AssertEx(AL_CATAST, m_nRealPathLen < MAX_PATH, _T("����Խ��"));
                }

                // ���µ�ǰ���ҵ�Ŀ¼
                _tcsncat_s(
                    m_szRealFindPathName,
                    MAX_PATH,
                    m_FindFileData.cFileName,
                    (INT)_tcslen(m_FindFileData.cFileName)
                    );

                // �����ַ������׷��'\\'
                AppPath(m_szRealFindPathName);

                // ���ò��ҵ�Ŀ¼֪ͨ����
                this->ProcessPath(m_szRealFindPathName);

                // �����ַ������׷��'*'
                AppPath(m_szRealFindPathName, _T('*'));

                // ���µ�ǰ���ҵ�Ŀ¼����
                m_nRealPathLen = (INT)_tcslen(m_szRealFindPathName);

                // ������Ŀ¼�ݹ����
                BrowsePath();

                // �ָ����������ַ���
                _tcsncpy_s(
                    m_szRealFindPathName,
                    MAX_PATH,
                    szRealFindPathName_Bk,
                    nRealFindPathNameLen_Bk
                    );

                // ���µ�ǰ���ҵ�Ŀ¼����
                m_nRealPathLen = (INT)_tcslen(m_szRealFindPathName);
            }
        }
        else
        {
            // ���ҵ����ļ�ȫ·�����ַ���
            TCHAR szFindedFileName[MAX_PATH] = {0};

            // ������ҵ����ļ������������
            if (CheckFileType(m_FindFileData.cFileName))
            {
                // ��õ�ǰ���ҵ����ļ�������·����
                _tcsncpy_s(
                    szFindedFileName,
                    MAX_PATH,
                    m_szRealFindPathName,
                    m_nRealPathLen
                    );

                // ȥ�����һλ��'*'

                if (m_nRealPathLen < MAX_PATH)
                {
                    if (szFindedFileName[m_nRealPathLen - 1] == '*')
                    {
                        szFindedFileName[m_nRealPathLen - 1] = '\0';
                    }
                    else
                    {
                        AssertEx(AL_CATAST, FALSE, _T("�����쳣,"));
                    }
                }
                else
                {
                    AssertEx(AL_CATAST, m_nRealPathLen < MAX_PATH, _T("����Խ��"));
                }

                // ׷��'\\'
                AppPath(szFindedFileName);

                // ׷���ļ���
                _tcsncat_s(
                    szFindedFileName,
                    MAX_PATH,
                    m_FindFileData.cFileName,
                    (INT)_tcslen(m_FindFileData.cFileName)
                    );

                // ���ò����ļ��ɹ�֪ͨ����
                this->ProcessFile(szFindedFileName);
            }
        }
        // ������һ���ļ��Ƿ�ɹ�
    } while (::FindNextFile(hFindFile, &m_FindFileData));

    // ���Ŀ¼��û���κ�����
    if (ERROR_NO_MORE_FILES == GetLastError())
    {
        // ���سɹ�
        return TRUE;
    }
    else
    {
        // ����ʧ��
        return FALSE;
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,�쳣����ʧ��*/;
    return FALSE;
}


// ��������ļ������Ƿ�ƥ��
BOOL CMsFindFileManager::CheckFileType(CONST TCHAR* strPath)
{
    _ENTER_FUN_C;

    // �������Ƿ�����
    if (strPath)
    {
        // ��ȡ�ļ�������
        SIZE_T sPathLen = _tcslen(strPath);
        AssertEx(AL_SHOW_ONLY, sPathLen < MAX_PATH - 2, _T("�����ߴ����,���󴫲�"));

        // ��ò���·���е�������չ����λ��
        SIZE_T sFirstFileTypeName = sPathLen - m_nFileTypeLen;

        // ѭ���Ƚ���չ���Ƿ���ͬ
        for (INT i = 0; i < m_nFileTypeLen; i++)
        {
            // ���������չ������·���Ѿ�Խ��,ֱ�ӷ���ʧ��
            if (sFirstFileTypeName + i > sPathLen)
            {
                // ����ʧ��
                return FALSE;
            }

            // ���ĳһλ����չ����ͬ
            if (m_szFileType[i] != (strPath[sFirstFileTypeName + i]))
            {
                // ����ʧ��
                return FALSE;
            }
        }
        // ȫ���Ƚϳɹ�,���سɹ�
        return TRUE;
    }
    else
    {
        AssertEx(AL_SHOW_ONLY, strPath, _T("strPathΪ��,���󴫲�"));
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,�쳣����ʧ��*/;
    return FALSE;
}

// ·��Ĭ��׷��"\"
BOOL CMsFindFileManager::AppPath(TCHAR* strPath, TCHAR cChr)
{
    _ENTER_FUN_C;

    // ��ȡ�ַ���·������
    SIZE_T sPathLen = _tcslen(strPath);
    AssertEx(AL_SHOW_ONLY, (sPathLen + 1) < MAX_PATH, _T("�����ַ����ܳ���Խ��,���󴫲�"));

    // ������һλ׷�ӵ���'\\'
    if(_T('\\')== cChr || _T('/')== cChr)
    {
        // ������һλ����'\\'
        if (strPath[sPathLen - 1] != _T('\\') && 
            strPath[sPathLen - 1] != _T('/'))
        {
            // ���һλ����Ϊ'\\'
            strPath[sPathLen] = _T('\\');

            // ���һλ׷��һ��'\0'
            strPath[sPathLen + 1] = _T('\0');
        }
    }
    else
    {
        // ���һλ����ΪcChr
        strPath[sPathLen] = cChr;

        // ���һλ׷��һ��'\0'
        strPath[sPathLen + 1] = _T('\0');
    }
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,�쳣����ʧ��*/;
    return FALSE;
}

//// ��������ÿ�ҵ�һ���ļ�,�͵���ProcessFile,�����ļ�����Ϊ�������ݹ�ȥ
//// �û����Ը�д�ú���,�����Լ��Ĵ������
//BOOL CMsFindFileManager::ProcessFile(CONST TCHAR* /*strFileName*/)
//{
//    _ENTER_FUN;
//
//    // ���سɹ�
//    return TRUE;
//
//    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,�쳣����ʧ��*/;
//    return FALSE;
//}
//
//// ��������ÿ����һ��Ŀ¼,�͵���ProcessPath,
//// �������ڴ����Ŀ¼������һ��Ŀ¼����Ϊ�������ݹ�ȥ,
//// ������ڴ�����ǳ�ʼĿ¼,��strParentPath = NULL
//// �û����Ը�д�ú���,�����Լ��Ĵ������
//BOOL CMsFindFileManager::ProcessPath(CONST TCHAR* /*strFileName*/)
//{
//    _ENTER_FUN;
//
//    // ���سɹ�
//    return TRUE;
//
//    _LEAVE_FUN_DISPOSE_CONTINUE_C/*�쳣����,�쳣����ʧ��*/;
//    return FALSE;
//}
