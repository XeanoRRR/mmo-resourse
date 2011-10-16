// UniEditor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UniEditor.h"
#include "UniEditorDlg.h"
#include "DlgFileList.h"

#define _CRTDBG_MAP_ALLOC 
#include <time.h>
#include<stdlib.h> 
#include<crtdbg.h> 
#include "tools.h"
//#include <vld.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#include "debug_new.h"

// CUniEditorApp

BEGIN_MESSAGE_MAP(CUniEditorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUniEditorApp ����

CUniEditorApp::CUniEditorApp()
{
	// TODO: �ڴ˴���ӹ������,
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	
}


// Ψһ��һ�� CUniEditorApp ����

CUniEditorApp theApp;


// CUniEditorApp ��ʼ��

BOOL CUniEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ,
	//����Ҫ InitCommonControlsEx()������,���޷��������ڡ�
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
	// ���տ�ִ���ļ��Ĵ�С,��Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ���,
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CUniEditorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹر�,���Խ����� FALSE �Ա��˳�Ӧ�ó���,
	//  ����������Ӧ�ó������Ϣ�á�
	
	return FALSE;
}

// �ļ���ȡ��ĳָ���ַ�
BOOL CUniEditorApp::ReadTo(const string &strReadTo, ifstream &ifStream)
{
	// ����ָ���ַ�
	string strBuf;

	int i = 0;
	do 
	{
		ifStream >> strBuf;
	} while(strBuf != strReadTo && strBuf != strEnd && !ifStream.eof());

	if(strReadTo == strBuf)
	{
		return TRUE;
	}
	return FALSE;
}
