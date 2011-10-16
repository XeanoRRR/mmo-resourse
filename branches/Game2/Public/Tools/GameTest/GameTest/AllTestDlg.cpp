/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�AllTestDlg.cpp
*  ���ܣ��ڳ������Ի������ṩCTab�ؼ��ĶԻ���
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
#include "AllTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CAllTestDlg, CDialog)

CAllTestDlg::CAllTestDlg(CWnd* pParent)
	: CDialog(CAllTestDlg::IDD, pParent)
{
}

CAllTestDlg::~CAllTestDlg()
{
}

void CAllTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_TypePage, m_tabTypePage);
}

BEGIN_MESSAGE_MAP(CAllTestDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TypePage, &CAllTestDlg::OnTcnSelchangeTabTypepage)
END_MESSAGE_MAP()

BOOL CAllTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rs;
	m_tabTypePage.GetClientRect(rs);
	rs.top =+ 20;

	tagPageUIInfo uiInfo;

	//���ܲ���
	m_dlgSkill.Create(IDD_PROPPAGE_SKILL,GetDlgItem(IDC_TAB_TypePage));
	uiInfo.pDlg = &m_dlgSkill;
	strcpy(uiInfo.szText,"���ܹ���");
	m_listPageUIInfo.push_back(uiInfo);

	//�����ʼ�
	m_MailDlg.Create(IDD_PROPPAGE_MAIL, GetDlgItem(IDC_TAB_TypePage));
	uiInfo.pDlg = &m_MailDlg;
	strcpy(uiInfo.szText,"�ʼ�����");
	m_listPageUIInfo.push_back(uiInfo);

	//���Ժ���
	m_LinkManDlg.Create(IDD_PROPPAGE_LINKMAN, GetDlgItem(IDC_TAB_TypePage));
	uiInfo.pDlg = &m_LinkManDlg;
	strcpy(uiInfo.szText,"���ѹ���");
	m_listPageUIInfo.push_back(uiInfo);


	//2009-8-3 by:G++
	//m_dlgGoodsTest.Create(IDD_DIALOG_GOODS_TEST,GetDlgItem(IDC_TAB_TypePage));
	//uiInfo.pDlg = &m_dlgGoodsTest;
	//strcpy(uiInfo.szText,"GoodsTest");
	//m_listPageUIInfo.push_back(uiInfo);

	//���Է����
	m_ACADlg.Create(IDD_PROPPAGE_AC_ANSWER, GetDlgItem(IDC_TAB_TypePage));
	uiInfo.pDlg = &m_ACADlg;
	strcpy(uiInfo.szText,"����ҹ���");
	m_listPageUIInfo.push_back(uiInfo);

	list<tagPageUIInfo>::iterator it = m_listPageUIInfo.begin();

	for(long lFlag = 0; it != m_listPageUIInfo.end(); it++,lFlag++)
	{
		(*it).lFlag = lFlag;
		m_tabTypePage.InsertItem((*it).lFlag,(*it).szText);
		(*it).pDlg->MoveWindow(rs);
	}
	m_dlgSkill.ShowWindow(TRUE);




	return TRUE;
}

void CAllTestDlg::OnTcnSelchangeTabTypepage(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCurSel = m_tabTypePage.GetCurSel();
	list<tagPageUIInfo>::iterator it = m_listPageUIInfo.begin();
	for(; it != m_listPageUIInfo.end(); it++)
	{
		if( (*it).lFlag == nCurSel )
			(*it).pDlg->ShowWindow(TRUE);
		else
			(*it).pDlg->ShowWindow(FALSE);
	}
	*pResult = 0;
}
