// AlarmClock.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "resource.h"
#include "AlarmClock.h"
#include "AlarmClockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAlarmClockApp

BEGIN_MESSAGE_MAP(CAlarmClockApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAlarmClockApp ����

CAlarmClockApp::CAlarmClockApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAlarmClockApp ����

CAlarmClockApp theApp;


// CAlarmClockApp ��ʼ��

BOOL CAlarmClockApp::InitInstance()
{
	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CAlarmClockDlg* pdlg = new CAlarmClockDlg;
	m_pMainWnd = pdlg;
	INT_PTR nResponse = pdlg->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

    delete pdlg;
    pdlg = NULL;
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
