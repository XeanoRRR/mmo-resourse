// YaYaDisk.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CYaYaDiskApp:
// �йش����ʵ�֣������ YaYaDisk.cpp
//

class CYaYaDiskApp : public CWinApp
{
public:
	CYaYaDiskApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CYaYaDiskApp theApp;