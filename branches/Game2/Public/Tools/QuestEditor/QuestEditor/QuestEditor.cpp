// QuestEditor.cpp : ����Ӧ�ó��������Ϊ��
//
#include "stdafx.h"
#include "QuestEditor.h"
#include "QuestEditorDlg.h"
#include "CXmlEdit.h"
//#include "vld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CQuestEditorApp
BEGIN_MESSAGE_MAP(CQuestEditorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
// CQuestEditorApp ����
CQuestEditorApp::CQuestEditorApp()
{
}
CQuestEditorApp::~CQuestEditorApp()
{
/*
	vector<tagRole*>::iterator iter;
	iter = m_AllRole.begin();
	for (;iter!=m_AllRole.end();iter++)
	{
		if (*iter==0)
		{
			break;
		}
		else
		{
			delete (*iter);
			*iter=0;
		}
	}
	m_AllRole.clear();*/

}
// Ψһ��һ�� CQuestEditorApp ����
 CQuestEditorApp theApp;
// CQuestEditorApp ��ʼ��
BOOL CQuestEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CQuestEditorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	return FALSE;
}

