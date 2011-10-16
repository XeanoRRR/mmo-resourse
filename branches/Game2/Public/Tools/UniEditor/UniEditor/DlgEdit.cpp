// DlgEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditor.h"
#include "DlgEdit.h"


// CDlgEdit �Ի���

IMPLEMENT_DYNAMIC(CDlgEdit, CDialog)

CDlgEdit::CDlgEdit(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEdit::IDD, pParent)
{
	m_lRegionWidth = 0;
	m_lRegionHeight = 0;
}

CDlgEdit::~CDlgEdit()
{
}

void CDlgEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEdit, CDialog)
END_MESSAGE_MAP()

// ��ʾ�Ի�����Ϣ
void CDlgEdit::SetDlg(CRegion *pRegion, void *pmonster, BOOL bALL)
{
	m_szRegionName = pRegion->GetRegionName();
 	m_lRegionWidth = pRegion->Width();
	m_lRegionHeight = pRegion->Height();
}

// CDlgEdit ��Ϣ�������
