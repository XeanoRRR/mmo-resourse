// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT



// ����������ϢID
#define MS_WM_NOTIFYICON WM_USER + 0x0101

// �궨����ЧID
#define INVALID_ID -1

// ���嵥�����Ӽ�¼����
#define MAX_CLOCK_COUNT 100

// �������Ͷ���
enum E_CLOCK_TYPE
{
    E_CLOCK_TYPE_UNKNOW = -1,   // δ֪����
    E_CLOCK_TYPE_ONCE,          // ��һ��
    E_CLOCK_TYPE_EVERYDAY,      // ��ÿ��
    E_CLOCK_TYPE_ONLY_WEEK_0,   // ������
    E_CLOCK_TYPE_ONLY_WEEK_1,   // ����һ
    E_CLOCK_TYPE_ONLY_WEEK_2,   // ���ܶ�
    E_CLOCK_TYPE_ONLY_WEEK_3,   // ������
    E_CLOCK_TYPE_ONLY_WEEK_4,   // ������
    E_CLOCK_TYPE_ONLY_WEEK_5,   // ������
    E_CLOCK_TYPE_ONLY_WEEK_6,   // ������
    E_CLOCK_TYPE_MAX_OF_DWORD = 0x7FFFFFFF
};

// ��ʾ���Ͷ���
enum E_CLUE_TYPE
{
    E_CLUE_TYPE_NORMAL = 0, // ��׼���� ->  ����ʾһ��,�յ�������!
    E_CLUE_TYPE_ALONG,      // һֱ���� ->  һֱ��ʾ,ֱ�����ͣ��!
    E_CLUE_TYPE_INTERVAL,   // ������� ->  �����ʾ,ÿ��һ��ʱ����ʾ,ֱ�����ͣ�ֻ�ﵽһ������!
    E_CLUE_TYPE_MAX_OF_DWORD = 0x7FFFFFFF
};

// ���Ӽ�¼�ṹ��
struct S_CLOCK_REGISTER
{
    S_CLOCK_REGISTER(VOID)
        : m_ClockType   (E_CLOCK_TYPE_ONCE)
        , m_ClueType    (E_CLUE_TYPE_NORMAL)
        , m_ClueLong    (10000)
        , m_IntervalLong(INVALID_ID)
        , m_MaxClueTime (INVALID_ID)
        , m_IsRuing     (FALSE)
    {
        ZeroMemory(&m_szClueOnText, sizeof(m_szClueOnText));
        ZeroMemory(&m_ClueOnTime, sizeof(m_ClueOnTime));
    }
    E_CLOCK_TYPE    m_ClockType;        // ��������
    TCHAR           m_szClueOnText[128];// ������ʾ����
    SYSTEMTIME      m_ClueOnTime;       // ��ʾʱ��
    E_CLUE_TYPE     m_ClueType;         // ��ʾ����
    UINT            m_ClueLong;         // ��ʾ��ó���(����)
    UINT            m_IntervalLong;     // �����ó���(����)
    UINT            m_MaxClueTime;      // �����ʾ����
    BOOL            m_IsRuing;          // ��������
};






