// DlgSignPos.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditorDlg.h"
#include "DlgSignPos.h"


// CDlgSignPos �Ի���

IMPLEMENT_DYNAMIC(CDlgSignPos, CDialog)

CDlgSignPos::CDlgSignPos(CWnd* pParent /*=NULL*/)
	: CDlgEdit(CDlgSignPos::IDD, pParent)
{

}

CDlgSignPos::~CDlgSignPos()
{
}

void CDlgSignPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSignPos, CDialog)
	ON_EN_CHANGE(IDC_SIGNPOS_NO, &CDlgSignPos::OnEnChangeSignposNo)
	ON_EN_CHANGE(IDC_SIGNPOS_X, &CDlgSignPos::OnEnChangeSignposX)
	ON_EN_CHANGE(IDC_SIGNPOS_Y, &CDlgSignPos::OnEnChangeSignposY)
END_MESSAGE_MAP()

BOOL CDlgSignPos::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pWndParent = GetParent();
	m_ptreeRegion = ((CUniEditorDlg *)m_pWndParent)->GetTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
// CDlgSignPos ��Ϣ�������
void CDlgSignPos::SetDlg(CRegion *pRegion,CSignPos *pSignPos, BOOL bALL)
{
	m_pSignPos=pSignPos;
	SetDlgItemInt(IDC_SIGNPOS_NO,pSignPos->GetNo());
	SetDlgItemInt(IDC_SIGNPOS_X,pSignPos->GetxPos());
	SetDlgItemInt(IDC_SIGNPOS_Y,pSignPos->GetyPos());
	// ȫ������
	if(bALL)
	{
		m_hItem = m_ptreeRegion->GetItem();
		m_uOffset= m_ptreeRegion->GetOffset(m_hItem);
		//pmonster->Monster2String(m_szModifyNode);
		m_pRegion = pRegion;
		SetDlgItemInt(IDC_SIGNPOS_NO,pSignPos->GetNo());
		SetDlgItemInt(IDC_SIGNPOS_X,pSignPos->GetxPos());
		SetDlgItemInt(IDC_SIGNPOS_Y,pSignPos->GetyPos());
		CDlgEdit::SetDlg(pRegion, pSignPos);    
	}

	// ������
	else
	{
		CUniEditorDlg::m_bUpdateSignPos = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(2);
	}
}
void CDlgSignPos::OnEnChangeSignposNo()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDlgEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pSignPos->SetNo(GetDlgItemInt(IDC_SIGNPOS_NO));
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateSignPos = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(2);
	}
	
}

void CDlgSignPos::OnEnChangeSignposX()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDlgEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pSignPos->SetxPos(GetDlgItemInt(IDC_SIGNPOS_X));
	::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateSignPos = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(2);
	}
}

void CDlgSignPos::OnEnChangeSignposY()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDlgEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pSignPos->SetyPos(GetDlgItemInt(IDC_SIGNPOS_Y));
	::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateSignPos = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(2);
	}
}
