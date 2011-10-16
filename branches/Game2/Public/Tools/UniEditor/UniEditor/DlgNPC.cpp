// DlgNPC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditor.h"
#include "UniEditorDlg.h"
#include "DlgNPC.h"


// CDlgNPC �Ի���

IMPLEMENT_DYNAMIC(CDlgNPC, CDialog)

CDlgNPC::CDlgNPC(CWnd* pParent /*=NULL*/)
	: CDlgEdit(CDlgNPC::IDD, pParent)
{
	m_pNPC =NULL;
	m_ptreeRegion = NULL;
	m_pWndParent = NULL;
	m_bDlgEdit = FALSE;
	m_bError = FALSE;
	m_pCell = NULL;
}

CDlgNPC::~CDlgNPC()
{
}

void CDlgNPC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNPC, CDialog)
	ON_EN_CHANGE(IDC_NPC_COOR_LEFT, &CDlgNPC::OnEnChangeNpcCoorLeft)
	ON_EN_CHANGE(IDC_NPC_COOR_TOP, &CDlgNPC::OnEnChangeNpcCoorTop)
	ON_EN_CHANGE(IDC_NPC_COOR_RIGHT, &CDlgNPC::OnEnChangeNpcCoorRight)
	ON_EN_CHANGE(IDC_NPC_COOR_BOTTOM, &CDlgNPC::OnEnChangeNpcCoorBottom)
	ON_EN_CHANGE(IDC_NPC_DIRECTION, &CDlgNPC::OnEnChangeNpcDirection)
	ON_EN_CHANGE(IDC_NPC_NAME, &CDlgNPC::OnEnChangeNpcName)
	ON_EN_CHANGE(IDC_NPC_PICID, &CDlgNPC::OnEnChangeNpcPicid)
	ON_EN_CHANGE(IDC_NPC_QUANTITY, &CDlgNPC::OnEnChangeNpcQuantity)
	ON_EN_CHANGE(IDC_NPC_SCRIPT, &CDlgNPC::OnEnChangeNpcScript)
	ON_BN_CLICKED(IDC_CHECK_BORN, &CDlgNPC::OnBnClickedCheckBorn)
	ON_BN_CLICKED(IDC_CHECK_SHOW, &CDlgNPC::OnBnClickedCheckShow)
END_MESSAGE_MAP()

void CDlgNPC::SetDlg(CRegion *pRegion, CNPC *pnpc, BOOL bALL) 
{
	m_pNPC = pnpc;
	SetDlgItemInt(IDC_NPC_COOR_LEFT, pnpc->GetCoordinate().left);
	SetDlgItemInt(IDC_NPC_COOR_TOP, pnpc->GetCoordinate().top);
	SetDlgItemInt(IDC_NPC_COOR_RIGHT, pnpc->GetCoordinate().right);
	SetDlgItemInt(IDC_NPC_COOR_BOTTOM, pnpc->GetCoordinate().bottom);
	m_bDlgEdit = TRUE;

	if(bALL)
	{
		m_hItem = m_ptreeRegion->GetItem();
		m_uOffset= m_ptreeRegion->GetOffset(m_hItem);

		pnpc->NPC2String(m_szModifyNode);
		CString str;
		str.Format(_T("%-u\t"), m_uOffset);
		m_szModifyNode = str + m_szModifyNode;
		SetDlgItemText(IDC_NPC_NAME, pnpc->GetName().c_str());
		SetDlgItemInt(IDC_NPC_PICID, pnpc->GetPicID());
		SetDlgItemInt(IDC_NPC_QUANTITY, pnpc->GetQuantity());
		SetDlgItemInt(IDC_NPC_DIRECTION, pnpc->GetDirection());
//		SetDlgItemText(IDC_NPC_SCRIPT, pnpc->GetScript().c_str());
		((CButton *)GetDlgItem(IDC_CHECK_BORN))->SetCheck(pnpc->GetSwitch());
		((CButton *)GetDlgItem(IDC_CHECK_SHOW))->SetCheck(pnpc->GetNo());
		CDlgEdit::SetDlg(pRegion, pnpc);
	}
}

// CDlgNPC ��Ϣ�������
void CDlgNPC::OnEnChangeNpcCoorLeft()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//if(m_bDlgEdit)
	//{
	//	int i = GetDlgItemInt(IDC_NPC_COOR_LEFT);

	//	// NPC����һ����
	//	if(i>0 && i<=m_lRegionWidth)
	//	{
	//		if(i <= m_pNPC->GetCoordinate().right)
	//		{
	//			m_pNPC->SetCoorLeft(i);
	//			::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

	//			// ��������л����ʱ�ı�,��Ҫ����
	//			if(!CUniEditorDlg::m_bSelChange)
	//			{
	//				CUniEditorDlg::m_bUpdateNPC = TRUE;
	//				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	//			}
	//		}
	//	}

	//	else
	//	{
	//		SetDlgItemInt(IDC_NPC_COOR_LEFT, m_pNPC->GetCoordinate().left);
	//	}
	//}
}

void CDlgNPC::OnEnChangeNpcCoorTop()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//if(m_bDlgEdit)
	//{
	//	int i = GetDlgItemInt(IDC_NPC_COOR_TOP);

	//	// ��������л����ʱ�ı�,��Ҫ����
	//	if(i>0 && i<=m_lRegionHeight)
	//	{
	//		if(i <= m_pNPC->GetCoordinate().bottom)
	//		{
	//			m_pNPC->SetCoorTop(i);
	//			::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

	//			// ��������л����ʱ�ı�,��Ҫ����
	//			if(!CUniEditorDlg::m_bSelChange)
	//			{
	//				CUniEditorDlg::m_bUpdateNPC = TRUE;
	//				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	//			}
	//		}
	//	}

	//	else
	//	{
	//		SetDlgItemInt(IDC_NPC_COOR_TOP, m_pNPC->GetCoordinate().top);
	//	}
	//}
}

void CDlgNPC::OnEnChangeNpcCoorRight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//if(m_bDlgEdit)
	//{
	//	int i = GetDlgItemInt(IDC_NPC_COOR_RIGHT);
	//	if(i>0 && i<=m_lRegionWidth)
	//	{
	//		if(i >= m_pNPC->GetCoordinate().left)
	//		{
	//			m_pNPC->SetCoorRight(i);
	//			::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

	//			// ��������л����ʱ�ı�,��Ҫ����
	//			if(!CUniEditorDlg::m_bSelChange)
	//			{
	//				CUniEditorDlg::m_bUpdateNPC = TRUE;
	//				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	//			}
	//		}
	//	}

	//	else
	//	{
	//		SetDlgItemInt(IDC_NPC_COOR_RIGHT, m_pNPC->GetCoordinate().right);
	//	}
	//}
}

void CDlgNPC::OnEnChangeNpcCoorBottom()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	// ͨ���Ի���༭, ������ֱ�������༭
	//if(m_bDlgEdit)
	//{
	//	int i = GetDlgItemInt(IDC_NPC_COOR_BOTTOM);
	//	if(i>0 && i<=m_lRegionHeight)
	//	{
	//		if(i >= m_pNPC->GetCoordinate().top)
	//		{
	//			m_pNPC->SetCoorBottom(i);
	//			::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

	//			// ��������л����ʱ�ı�,��Ҫ����
	//			if(!CUniEditorDlg::m_bSelChange)
	//			{
	//				CUniEditorDlg::m_bUpdateNPC = TRUE;
	//				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	//			}
	//		}
	//	}

	//	else
	//	{
	//		SetDlgItemInt(IDC_NPC_COOR_BOTTOM, m_pNPC->GetCoordinate().bottom);
	//	}
	//}
}

void CDlgNPC::OnEnChangeNpcDirection()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//m_pNPC->SetDirection(GetDlgItemInt(IDC_NPC_DIRECTION));
	//if(!CUniEditorDlg::m_bSelChange)
	//{
	//	CUniEditorDlg::m_bUpdateNPC = TRUE;
	//	((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	//}
}

void CDlgNPC::OnEnChangeNpcName()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	//// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CString str(_T(""));
	//GetDlgItemText(IDC_NPC_NAME, str);
	//m_pNPC->SetName(str.GetString());

	//// �������οؼ�
	//m_ptreeRegion->SetItemText(m_ptreeRegion->GetItem(), str);

	//if(!CUniEditorDlg::m_bSelChange)
	//{
	//	CUniEditorDlg::m_bUpdateNPC = TRUE;
	//	((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	//}
}

void CDlgNPC::OnEnChangeNpcPicid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	m_pNPC->SetPicID(GetDlgItemInt(IDC_NPC_PICID));
	/*if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateNPC = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	}*/
}

void CDlgNPC::OnEnChangeNpcQuantity()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*m_pNPC->SetQuantity(GetDlgItemInt(IDC_NPC_QUANTITY));
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateNPC = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	}*/
}

void CDlgNPC::OnEnChangeNpcScript()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	CString str(_T(""));
//	GetDlgItemText(IDC_NPC_SCRIPT, str);
////	m_pNPC->SetScript(str.GetString());
//	if(!CUniEditorDlg::m_bSelChange)
//	{
//		CUniEditorDlg::m_bUpdateNPC = TRUE;
//		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
//	}
}

void CDlgNPC::OnBnClickedCheckBorn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CButton *pbtn = (CButton *)GetDlgItem(IDC_CHECK_BORN);
	//if (m_pNPC->GetSwitch())
	//{
	//	pbtn->SetCheck(BST_UNCHECKED);
	//	m_pNPC->SetSwitch(false);
	//}
	//else
	//{
	//	pbtn->SetCheck(BST_CHECKED);
	//	m_pNPC->SetSwitch();
	//}

	//if(!CUniEditorDlg::m_bSelChange)
	//{
	//	CUniEditorDlg::m_bUpdateNPC = TRUE;
	//	((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
	//}
}

void CDlgNPC::OnBnClickedCheckShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CButton *pbtn = (CButton *)GetDlgItem(IDC_CHECK_SHOW);
//	if (m_pNPC->GetNo())
//	{
//		pbtn->SetCheck(BST_UNCHECKED);
//		m_pNPC->SetNo(0);
//	}
//	else
//	{
//		pbtn->SetCheck(BST_CHECKED);
//		m_pNPC->SetNo(1);
//	}
//
//	if(!CUniEditorDlg::m_bSelChange)
//	{
//		CUniEditorDlg::m_bUpdateNPC = TRUE;
//		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(FALSE);
//	}
}

BOOL CDlgNPC::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//m_pWndParent = GetParent();
	//m_ptreeRegion = ((CUniEditorDlg *)m_pWndParent)->GetTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlgNPC::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;

			case VK_RETURN:
				{
					pMsg->wParam = VK_TAB;
					break;
				}

			default:
				break;
			}
			break;
		}

	default:
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
