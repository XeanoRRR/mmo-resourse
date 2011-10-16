/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�AddNameDlg.h
*  ���ܣ�ΪTestScript�����case�����ṩ��������Ի���
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�
*
============================================================
*/

#include "stdafx.h"
#include "GameTest.h"
#include "AddNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CAddNameDlg, CDialog)

CAddNameDlg::CAddNameDlg(CWnd* pParent)
	: CDialog(CAddNameDlg::IDD, pParent)
{

}

CAddNameDlg::~CAddNameDlg()
{
}

void CAddNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AddTitle, m_ctlAddTitle);
	DDX_Control(pDX, IDC_AddName, m_ctlAddName);
}

BEGIN_MESSAGE_MAP(CAddNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CAddNameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_ctlAddName.GetWindowText(m_strAddName);
	OnOK();
}
