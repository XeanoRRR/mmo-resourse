// QuestEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "struct.h"
#include "CXmlEdit.h"

// CQuestEditorApp:
// �йش����ʵ�֣������ QuestEditor.cpp
//

class CQuestEditorApp : public CWinApp
{

public:
	CQuestEditorApp();
	~CQuestEditorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};
extern CQuestEditorApp theApp;

