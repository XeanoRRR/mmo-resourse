// AffirmDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PackageTool.h"
#include "AffirmDialog.h"
#include ".\affirmdialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CAffirmDialog �Ի���

IMPLEMENT_DYNCREATE(CAffirmDialog, CDHtmlDialog)

CAffirmDialog::CAffirmDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAffirmDialog::IDD,pParent)
	, m_strText(_T(""))
	, m_bAlwaysCheck(FALSE)
	,m_iResult(-1)
{
}

CAffirmDialog::~CAffirmDialog()
{
}

void CAffirmDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
	DDX_Check(pDX, IDC_CHECK_ALWAY, m_bAlwaysCheck);
}

BOOL CAffirmDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CAffirmDialog, CDialog)
//	ON_BN_CLICKED(IDOK, OnBnClickedOk)
ON_BN_CLICKED(ID_OK, OnBnClickedOk)
ON_BN_CLICKED(ID_NO, OnBnClickedNo)
END_MESSAGE_MAP()


// CAffirmDialog ��Ϣ�������
//
//HRESULT CAffirmDialog::OnButtonOK(IHTMLElement* /*pElement*/)
//{
//	OnOK();
//	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
//}
//
//HRESULT CAffirmDialog::OnButtonCancel(IHTMLElement* /*pElement*/)
//{
//	OnCancel();
//	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
//}

//void CAffirmDialog::OnBnClickedOk()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}

void CAffirmDialog::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDialog::OnCancel();
}

void CAffirmDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_iResult = 1;
	OnOK();
}

void CAffirmDialog::OnBnClickedNo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_iResult = 0;
	OnOK();
}
