// SetupServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "urlfile.h"
#include "SetupServer.h"
#include "SetupDialog.h"


// SetupServer �Ի���

IMPLEMENT_DYNAMIC(SetupServer, CDialog)

SetupServer::SetupServer(CWnd* pParent /*=NULL*/)
	: CDialog(SetupServer::IDD, pParent)
{
}

SetupServer::~SetupServer()
{
}

void SetupServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CCombo);
}


BEGIN_MESSAGE_MAP(SetupServer, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDOK, &SetupServer::OnBnClickedOk)
	//ON_CBN_SELCHANGE(IDC_COMBO1, &SetupServer::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDCANCEL, &SetupServer::OnBnClickedCancel)
END_MESSAGE_MAP()


// SetupServer ��Ϣ�������
void SetupServer::OnBnClickedOk()
{
	int sel = m_CCombo.GetCurSel();
	CSetupDialog::m_Sever=m_CCombo.GetCurSel();
	OnOK();
}

BOOL SetupServer::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetPage();
	return 1;
}


void SetupServer::SetPage()
{
	m_CCombo.InsertString(-1,"����");
	m_CCombo.InsertString(-1,"��ͨ");
	m_CCombo.SetCurSel(CSetupDialog::m_Sever);
}

int SetupServer::GetCurSelect()
{
	return m_CCombo.GetCurSel();
}

void SetupServer::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
