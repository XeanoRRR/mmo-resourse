#include "Precompiled.h"

CMsFileFromRes::CMsFileFromRes(HINSTANCE hInstance)
:m_hInstance(hInstance)     // Ӧ�ó���ʵ�����
,m_hResFile(NULL)           // ��ʱ��Դ�ļ����
{
    _ENTER_FUN_C;

    // ��ʼ����Դ�����ַ���
    ZeroMemory(m_strResType, MAX_PATH);

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}

CMsFileFromRes::~CMsFileFromRes(VOID)
{
    _ENTER_FUN_C;


    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}

BOOL CMsFileFromRes::CreateFileFromRes(INT ID, TCHAR* strResType, TCHAR* szFilePath)
{
    _ENTER_FUN_C;

    // �������ͷ���ֵ
    BOOL bRet = TRUE;

    // ��Դ���
    HRSRC hRes = NULL;

    // ���ü��
    if (strResType)
    {
        TCHAR szResID[DWORD_NUMBER_SIZE] = {0};
        _itot_s(ID, szResID, DWORD_NUMBER_SIZE, RADIX_10);

        // ����ID�����Ͳ�����Դ
        if (strResType)
        {
            hRes = ::FindResource(m_hInstance, MAKEINTRESOURCE(ID), strResType);
        }
        else
        {
            hRes = ::FindResource(m_hInstance, MAKEINTRESOURCE(ID), m_strResType);
        }

        if (hRes)
        {
            // װ����Դ
            HGLOBAL hLoadRes = ::LoadResource(m_hInstance, hRes);

            if (hLoadRes)
            {
                // ������Դ
                LPVOID  lpLockRes = ::LockResource(hLoadRes);
                AssertEx(AL_NORMAL, lpLockRes, _T("LoadResourceʧ��,����ʧ��"));

                // ������Դ�ļ�
                m_hResFile = ::CreateFile(
                    szFilePath,
                    GENERIC_WRITE,
                    0,
                    NULL,
                    CREATE_ALWAYS,
                    0,
                    NULL
                    );
                AssertEx(AL_NORMAL, m_hResFile, _T("CreateFileʧ��,����ʧ��"));

                // ��ȡ��Դ�ߴ�
                DWORD nResSize = ::SizeofResource(NULL, hRes);

                // ��Դд���ļ�
                bRet = ::WriteFile(m_hResFile, lpLockRes, nResSize, &nResSize, NULL);
                AssertEx(AL_NORMAL, bRet, _T("WriteFileʧ��,����ʧ��"));

                // �ر��ļ����
                bRet = ::CloseHandle(m_hResFile);
                AssertEx(AL_NORMAL, bRet, _T("CloseHandleʧ��,����ʧ��"));
                m_hResFile = NULL;

                // �ͷ���Դ�ļ�
                ::FreeResource(hLoadRes);

                // ���سɹ�
                return TRUE;
            }
            else
            {
                AssertEx(AL_NORMAL, hLoadRes, _T("LoadResourceʧ��,����ʧ��"));
            }
        }
        else
        {
            AssertEx(AL_NORMAL, hRes, _T("FindResourceʧ��,����ʧ��"));
        }
    }
    else
    {
        AssertEx(AL_NORMAL, strResType, _T("strResType����Ϊ��"));
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

// ������Դ
LPVOID CMsFileFromRes::LoadFileFromRes(INT ID, TCHAR* strResType, DWORD* lpdwFileSize)
{
    _ENTER_FUN_C;

    // ���ü��
    if (strResType)
    {
        // ��Դ���
        HRSRC hRes = ::FindResource(m_hInstance, MAKEINTRESOURCE(ID), strResType);

        if (hRes)
        {
            // װ����Դ
            HGLOBAL hLoadRes = ::LoadResource(m_hInstance, hRes);

            if (hLoadRes)
            {
                *lpdwFileSize = ::SizeofResource(NULL, hRes);

                // ������Դ
                LPVOID lpRes = ::LockResource(hLoadRes);

                // �ͷ���Դ�ļ�
                ::FreeResource(hLoadRes);

                return lpRes;
            }
            else
            {
                AssertEx(AL_NORMAL, hLoadRes, _T("LoadResourceʧ��,����ʧ��"));
            }
        }
        else
        {
            AssertEx(AL_NORMAL, hRes, _T("FindResourceʧ��,����ʧ��"));
        }
    }
    else
    {
        AssertEx(AL_NORMAL, strResType, _T("strResType����Ϊ��"));
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return NULL;
}
