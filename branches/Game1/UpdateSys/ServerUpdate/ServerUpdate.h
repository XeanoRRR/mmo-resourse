// ServerUpdate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������

// CServerUpdateApp:
// �йش����ʵ�֣������ ServerUpdate.cpp
//

class CServerUpdateApp : public CWinApp
{
public:
	CServerUpdateApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

void PutDebugString(const char*);

extern CServerUpdateApp theApp;
