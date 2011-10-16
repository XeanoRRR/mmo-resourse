// Patch.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Patch.h"
#include "PatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPatchApp

BEGIN_MESSAGE_MAP(CPatchApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPatchApp ����

CPatchApp::CPatchApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	CoUninitialize();
}


// Ψһ��һ�� CPatchApp ����

CPatchApp theApp;


// CPatchApp ��ʼ��

BOOL CPatchApp::InitInstance()
{
	// ������³����Ѿ��򿪣���ر��Լ�
	HANDLE hM = CreateMutex(NULL,TRUE,"CSUpdate");
	if( GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}
	HWND hWnd = FindWindow("���ǳ���˵��",NULL);
	if (hWnd != NULL)
	{
		AfxMessageBox(_T("������������Ϸ�������ȹر���Ϸ���������и��³���"));
		return FALSE;
	}

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	CoInitialize(NULL);

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CPatchDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�

	if (hM) 
	{
		ReleaseMutex(hM);
		CloseHandle(hM);
	}

	return FALSE;
}
