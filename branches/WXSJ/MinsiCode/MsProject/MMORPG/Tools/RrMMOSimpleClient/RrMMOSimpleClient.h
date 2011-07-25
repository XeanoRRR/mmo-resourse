// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// RrMMOSimpleClient.h : RrMMOSimpleClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������Precompiled.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CRrMMOSimpleClientApp:
// �йش����ʵ�֣������ RrMMOSimpleClient.cpp
//

class CRrMMOSimpleClientApp : public CWinAppEx
{
public:
    CRrMMOSimpleClientApp();


// ��д
public:
    virtual BOOL InitInstance();

// ʵ��
    COleTemplateServer m_server;
        // �����ĵ������ķ���������
    UINT  m_nAppLook;
    BOOL  m_bHiColorIcons;

    virtual void PreLoadState();
    virtual void LoadCustomState();
    virtual void SaveCustomState();

    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CRrMMOSimpleClientApp theApp;
