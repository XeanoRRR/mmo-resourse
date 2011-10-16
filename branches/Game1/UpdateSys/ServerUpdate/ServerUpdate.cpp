// ServerUpdate.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ServerUpdate.h"
#include "ServerUpdateDlg.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//���������Ϣ
void PutDebugString(const char *msg)
{
	AfxMessageBox(msg);
}
// CServerUpdateApp

BEGIN_MESSAGE_MAP(CServerUpdateApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CServerUpdateApp ����

CServerUpdateApp::CServerUpdateApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CServerUpdateApp ����

CServerUpdateApp theApp;

// CServerUpdateApp ��ʼ��

BOOL CServerUpdateApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

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

	bool bExceRun = false;
	string strExeWinTitle("");
	string strCurDirectory("");
	string strServerAddress("");
	string strServerFolder("");
	string strExceFileName("");

	CHAR* strPath = new CHAR[MAX_PATH];
	if(::GetCurrentDirectory(MAX_PATH,strPath)==false)
		return false;
	strCurDirectory=string(strPath);
	delete []strPath;

	string strTemp;
	ifstream stream;
	stream.open("UpdateSetup.ini");
	if( stream.is_open() )
	{
		stream	>> strTemp >> strServerAddress
			>> strTemp >> strServerFolder
			>> strTemp >> strExceFileName
			>> strTemp >> strExeWinTitle;
		stream.close();
	}
	CServerUpdateDlg dlg(strCurDirectory,strServerAddress,strServerFolder,strExceFileName,strExeWinTitle);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		//��ִ�г����Ƿ���������
		HWND hWnd = FindWindow(NULL,strExeWinTitle.c_str());
		if (hWnd != NULL)
		{
			bExceRun = true;
		}

		if(bExceRun == false)
		{
			WinExec(strExceFileName.c_str(),SW_SHOWNORMAL);
		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

