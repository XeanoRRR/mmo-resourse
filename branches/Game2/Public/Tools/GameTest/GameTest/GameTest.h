/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�GameTest.h
*  ���ܣ�GametestMFC������ں���
*  ��ǰ�汾��1.1.0
*  ���ߣ� 
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�2009.2.2
*
============================================================
*/

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"

class CGameTestApp : public CWinApp
{
public:
	CGameTestApp();
	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CGameTestApp theApp;
