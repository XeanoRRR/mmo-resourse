// PackageTool.h : PackageTool Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CPackageToolApp:
// �йش����ʵ�֣������ PackageTool.cpp
//

class CPackageToolApp : public CWinApp
{
public:
	CPackageToolApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CPackageToolApp theApp;
