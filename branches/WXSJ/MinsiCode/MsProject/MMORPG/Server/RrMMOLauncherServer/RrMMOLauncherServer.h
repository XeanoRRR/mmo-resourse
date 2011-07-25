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

// RrMMOLauncherServer.h : RrMMOLauncherServer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������Precompiled.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CRrMMOLauncherServerApp:
// �йش����ʵ�֣������ RrMMOLauncherServer.cpp
//

class CRrMMOLauncherServerApp : public CWinAppEx
{
public:
    CRrMMOLauncherServerApp();


// ��д
public:
    virtual BOOL InitInstance();

// ʵ��
    COleTemplateServer m_server;
        // �����ĵ������ķ���������
    UINT  m_nAppLook;
    BOOL  m_bHiColorIcons;

    virtual VOID PreLoadState();
    virtual VOID LoadCustomState();
    virtual VOID SaveCustomState();

    afx_msg VOID OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CRrMMOLauncherServerApp theApp;
