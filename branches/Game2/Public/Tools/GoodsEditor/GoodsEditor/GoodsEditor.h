// CSL_GoodsEditor.h : CSL_GoodsEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CCSL_GoodsEditorApp:
// �йش����ʵ�֣������ CSL_GoodsEditor.cpp
//

class CCSL_GoodsEditorApp : public CWinApp
{
public:
	CCSL_GoodsEditorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCSL_GoodsEditorApp theApp;
