#include "Precompiled.h"
#include "MsString.h"

// ����
CMsString::CMsString(DWORD dwMaxBuffLen, BOOL bChangeCharacterSet)
: m_dwMaxBuffLen        (dwMaxBuffLen)
, m_bChangeCharacterSet (bChangeCharacterSet)
, m_strTextA            (NULL)
, m_strTextW            (NULL)
, m_stTextLen           (0)
{
    if (m_bChangeCharacterSet)
    {
        m_strTextA = NEW ACHAR[dwMaxBuffLen];
        AssertEx(AL_CATAST, m_strTextA, _T("m_strTextAΪ��,�ڴ治��"));

        m_strTextW = NEW WCHAR[dwMaxBuffLen];
        AssertEx(AL_CATAST, m_strTextW, _T("m_strTextWΪ��,�ڴ治��"));
    }
    else
    {
#ifdef UNICODE
        m_strTextW = NEW WCHAR[dwMaxBuffLen];
        AssertEx(AL_CATAST, m_strTextW, _T("m_strTextWΪ��,�ڴ治��"));
        //m_strText = m_strTextW;
#else
        m_strTextA = NEW ACHAR[dwMaxBuffLen];
        AssertEx(AL_CATAST, m_strTextA, _T("m_strTextAΪ��,�ڴ治��"));
        //m_strText = m_strTextA;
#endif
    }
}

// ���ַ�����
CMsString::CMsString(LPCHAR szText, DWORD dwMaxBuffLen, BOOL bChangeCharacterSet)
: m_dwMaxBuffLen        (dwMaxBuffLen)
, m_bChangeCharacterSet (bChangeCharacterSet)
, m_strTextA            (NULL)
, m_strTextW            (NULL)
, m_stTextLen           (0)
{
    if (szText)
    {
        m_stTextLen = strlen(szText);
        if (m_bChangeCharacterSet)
        {
            m_strTextA = NEW ACHAR[dwMaxBuffLen];
            if (m_strTextA)
            {
                strncpy_s(m_strTextA, m_stTextLen, szText, dwMaxBuffLen);
            }
            else
            {
                AssertEx(AL_CATAST, m_strTextA, _T("m_strTextAΪ��,�ڴ治��"));
            }

            m_strTextW = NEW WCHAR[dwMaxBuffLen];
            if (m_strTextW && m_strTextA)
            {
                ::MultiByteToWideChar(
                    CP_ACP, 
                    0, 
                    m_strTextA,
                    (INT)(m_stTextLen + 1),
                    m_strTextW,
                    dwMaxBuffLen
                    );
            }
            else
            {
                AssertEx(AL_CATAST, m_strTextA, _T("m_strTextAΪ��,�ڴ治��"));
                AssertEx(AL_CATAST, m_strTextW, _T("m_strTextWΪ��,�ڴ治��"));
            }
        }
        else
        {
            m_strTextA = NEW ACHAR[dwMaxBuffLen];
            if (m_strTextA)
            {
                strncpy_s(m_strTextA, m_stTextLen, szText, dwMaxBuffLen);
            }
            else
            {
                AssertEx(AL_CATAST, m_strTextA, _T("m_strTextAΪ��,�ڴ治��"));
            }
        }
    }
}

// ���ַ�����
CMsString::CMsString(LPWCHAR szText, DWORD dwMaxBuffLen, BOOL bChangeCharacterSet)
: m_dwMaxBuffLen        (dwMaxBuffLen)
, m_bChangeCharacterSet (bChangeCharacterSet)
, m_strTextA            (NULL)
, m_strTextW            (NULL)
, m_stTextLen           (0)
{
    if (szText)
    {
        m_stTextLen = wcslen(szText);
        if (m_bChangeCharacterSet)
        {
            m_strTextW = NEW WCHAR[dwMaxBuffLen];
            if (m_strTextW)
            {
                wcsncpy_s(m_strTextW, m_stTextLen, szText, dwMaxBuffLen);
            }
            else
            {
                AssertEx(AL_CATAST, m_strTextW, _T("m_strTextWΪ��,�ڴ治��"));
            }

            m_strTextA = NEW ACHAR[dwMaxBuffLen];
            if (m_strTextA && m_strTextW)
            {
                ::WideCharToMultiByte(
                    CP_ACP, 
                    0, 
                    m_strTextW,
                    (INT)(m_stTextLen + 1),
                    m_strTextA,
                    dwMaxBuffLen,
                    NULL,
                    NULL
                    );
            }
            else
            {
                AssertEx(AL_CATAST, m_strTextA, _T("m_strTextAΪ��,�ڴ治��"));
            }
        }
        else
        {
            m_strTextW = NEW WCHAR[dwMaxBuffLen];
            if (m_strTextW)
            {
                wcsncpy_s(m_strTextW, m_stTextLen, szText, dwMaxBuffLen);
            }
            else
            {
                AssertEx(AL_CATAST, m_strTextW, _T("m_strTextWΪ��,�ڴ治��"));
            }
        }
    }
}

// ����
CMsString::~CMsString(VOID)
{
    SAFE_DELETE_ARRAY(m_strTextA);
    SAFE_DELETE_ARRAY(m_strTextW);
    m_dwMaxBuffLen = 0;
    m_stTextLen = 0;
}

// ��ȡ���ַ���
CMsString::operator LPCHAR()
{
    return m_strTextA;
}

// ��ȡ���ַ���
CMsString::operator LPWCHAR()
{
    return m_strTextW;
}

// ��ȡ�ַ�������(�ַ�����)
size_t CMsString::GetTextLen(VOID)
{
    return m_stTextLen;
}