// AlarmClock.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAlarmClockApp:
// �йش����ʵ�֣������ AlarmClock.cpp
//

class CAlarmClockApp : public CWinApp
{
public:
	CAlarmClockApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAlarmClockApp theApp;