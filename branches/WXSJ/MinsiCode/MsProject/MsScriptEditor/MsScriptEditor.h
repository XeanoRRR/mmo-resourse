
// MsScriptEditor.h : MsScriptEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMsScriptEditorApp:
// �йش����ʵ�֣������ MsScriptEditor.cpp
//

class CMsScriptEditorApp : public CWinAppEx
{
public:
	CMsScriptEditorApp();


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

extern CMsScriptEditorApp theApp;
