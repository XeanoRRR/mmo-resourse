
// EmptyMFCTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEmptyMFCTestApp:
// �йش����ʵ�֣������ EmptyMFCTest.cpp
//

class CEmptyMFCTestApp : public CWinAppEx
{
public:
	CEmptyMFCTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEmptyMFCTestApp theApp;