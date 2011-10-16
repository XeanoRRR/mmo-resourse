// DlgMonster.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditorDlg.h"
#include "DlgMonster.h"


// CDlgMonster �Ի���

IMPLEMENT_DYNAMIC(CDlgMonster, CDialog)

CDlgMonster::CDlgMonster(CWnd* pParent /*=NULL*/)
	: CDlgEdit(CDlgMonster::IDD, pParent)
{
	m_pMonster = NULL;
	m_ptreeRegion = NULL;
	m_pWndParent = NULL;
	m_bDlgEdit = FALSE;
	m_bError = FALSE;
}

CDlgMonster::~CDlgMonster()
{
}

void CDlgMonster::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// ��ʾMONSTER������Ϣ
void CDlgMonster::SetDlg(CRegion *pRegion, CMonster *pmonster, BOOL bALL)
{
	m_pMonster = pmonster;
	SetDlgItemInt(IDC_MONSTER_COOR_LEFT, pmonster->GetCoordinate().left);
	SetDlgItemInt(IDC_MONSTER_COOR_TOP, pmonster->GetCoordinate().top);
	SetDlgItemInt(IDC_MONSTER_COOR_RIGHT, pmonster->GetCoordinate().right);
	SetDlgItemInt(IDC_MONSTER_COOR_BOTTOM, pmonster->GetCoordinate().bottom);
	m_bDlgEdit = TRUE;

	// ȫ������
	if(bALL)
	{
		m_hItem = m_ptreeRegion->GetItem();
		m_uOffset= m_ptreeRegion->GetOffset(m_hItem);
		//pmonster->Monster2String(m_szModifyNode);
		m_pRegion = pRegion;
		SetDlgItemInt(IDC_MONSTER_NO, pmonster->GetNo());
		SetDlgItemInt(IDC_MONSTER_QUANTITY, pmonster->GetQuantity());
		SetDlgItemInt(IDC_MONSTER_TIME_BORN, pmonster->GetTimeBorn());
		SetDlgItemInt(IDC_MONSTER_TIME_START, pmonster->GetTimeStart());
		SetDlgItemInt(IDC_MONSTER_DIRECTION, pmonster->GetDirection());
		((CButton *)GetDlgItem(IDC_CHECK_BORN))->SetCheck(pmonster->GetSwitch());
		CDlgEdit::SetDlg(pRegion, pmonster);    
	}

	// ������
	else
	{
		CUniEditorDlg::m_bUpdateMonster = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

BEGIN_MESSAGE_MAP(CDlgMonster, CDialog)
	ON_EN_CHANGE(IDC_MONSTER_NO, &CDlgMonster::OnEnChangeMonsterNo)
	ON_EN_CHANGE(IDC_MONSTER_COOR_LEFT, &CDlgMonster::OnEnChangeMonsterCoorLeft)
	ON_EN_CHANGE(IDC_MONSTER_COOR_TOP, &CDlgMonster::OnEnChangeMonsterCoorTop)
	ON_EN_CHANGE(IDC_MONSTER_COOR_RIGHT, &CDlgMonster::OnEnChangeMonsterCoorRight)
	ON_EN_CHANGE(IDC_MONSTER_COOR_BOTTOM, &CDlgMonster::OnEnChangeMonsterCoorBottom)
	ON_EN_CHANGE(IDC_MONSTER_DIRECTION, &CDlgMonster::OnEnChangeMonsterDirection)
	ON_EN_CHANGE(IDC_MONSTER_QUANTITY, &CDlgMonster::OnEnChangeMonsterQuantity)
	ON_EN_CHANGE(IDC_MONSTER_TIME_BORN, &CDlgMonster::OnEnChangeMonsterTimeBorn)
	ON_EN_CHANGE(IDC_MONSTER_TIME_START, &CDlgMonster::OnEnChangeMonsterTimeStart)
	ON_BN_CLICKED(IDC_CHECK_BORN, &CDlgMonster::OnBnClickedCheckBorn)
END_MESSAGE_MAP()


// CDlgMonster ��Ϣ�������

void CDlgMonster::OnEnChangeMonsterCoorLeft()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(m_bDlgEdit)
	{
		int i = GetDlgItemInt(IDC_MONSTER_COOR_LEFT);
		if(i>0 && i<=m_lRegionWidth)
		{
			
			//if(i <= m_pMonster->GetCoordinate().right)
			{
				m_pMonster->SetCoorLeft(i);
				::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

				// ��������л����ʱ�ı�,��Ҫ����
				if(!CUniEditorDlg::m_bSelChange)
				{
					CUniEditorDlg::m_bUpdateMonster = TRUE;
					((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
				}
			}
		}

		else
		{
			SetDlgItemInt(IDC_MONSTER_COOR_LEFT, m_pMonster->GetCoordinate().left);
			if(!CUniEditorDlg::m_bSelChange)
			{
				CUniEditorDlg::m_bUpdateMonster = TRUE;
				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
			}
		}
	}
}

void CDlgMonster::OnEnChangeMonsterCoorTop()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(m_bDlgEdit)
	{
		int i = GetDlgItemInt(IDC_MONSTER_COOR_TOP);
		if(i>0 && i<=m_lRegionHeight)
		{
			
			//if(i <= m_pMonster->GetCoordinate().bottom)
			{
				m_pMonster->SetCoorTop(i);
				::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

				// ��������л����ʱ�ı�,��Ҫ����
				if(!CUniEditorDlg::m_bSelChange)
				{
					CUniEditorDlg::m_bUpdateMonster = TRUE;
					((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
				}
			}
		}

		else
		{
			SetDlgItemInt(IDC_MONSTER_COOR_TOP, m_pMonster->GetCoordinate().top);
			if(!CUniEditorDlg::m_bSelChange)
			{
				CUniEditorDlg::m_bUpdateMonster = TRUE;
				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
			}
		}
	}
}

void CDlgMonster::OnEnChangeMonsterCoorRight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(m_bDlgEdit)
	{
		int i = GetDlgItemInt(IDC_MONSTER_COOR_RIGHT);
		if(i>0 && i<=m_lRegionWidth)
		{
			
			//if(i >= m_pMonster->GetCoordinate().left)
			{
				m_pMonster->SetCoorRight(i);
				::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

				// ��������л����ʱ�ı�,��Ҫ����
				if(!CUniEditorDlg::m_bSelChange)
				{
					CUniEditorDlg::m_bUpdateMonster = TRUE;
					((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
				}
			}
		}

		else
		{
			SetDlgItemInt(IDC_MONSTER_COOR_RIGHT, m_pMonster->GetCoordinate().right);
			if(!CUniEditorDlg::m_bSelChange)
			{
				CUniEditorDlg::m_bUpdateMonster = TRUE;
				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
			}
		}
	}
}

void CDlgMonster::OnEnChangeMonsterCoorBottom()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	// ͨ���Ի���༭, ������ֱ�������༭
	if(m_bDlgEdit)
	{
		int i = GetDlgItemInt(IDC_MONSTER_COOR_BOTTOM);
		if(i>0 && i<=m_lRegionHeight)
		{
			
			//if(i >= m_pMonster->GetCoordinate().top)
			{
				m_pMonster->SetCoorBottom(i);
				::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);

				// ��������л����ʱ�ı�,��Ҫ����
				if(!CUniEditorDlg::m_bSelChange)
				{
					CUniEditorDlg::m_bUpdateMonster = TRUE;
					((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
				}
			}
		}

		else
		{
			SetDlgItemInt(IDC_MONSTER_COOR_BOTTOM, m_pMonster->GetCoordinate().bottom);
			if(!CUniEditorDlg::m_bSelChange)
			{
				CUniEditorDlg::m_bUpdateMonster = TRUE;
				((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
			}
		}
	}
}

void CDlgMonster::OnEnChangeMonsterNo()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	unsigned uNo = GetDlgItemInt(IDC_MONSTER_NO);
	unsigned uInd = 0;

	if (0 == uNo || !m_pRegion->CheckMonsterNo(uNo, uInd))
	{
		m_pMonster->SetNo(uNo);
		// �������οؼ�
		CString str;
		str.Format(_T("%u"), uNo);
		m_ptreeRegion->SetItemText(m_ptreeRegion->GetItem(), str);

		if(!CUniEditorDlg::m_bSelChange)
		{
			CUniEditorDlg::m_bUpdateMonster = TRUE;
			((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
		}
	}
}

void CDlgMonster::OnEnChangeMonsterDirection()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonster->SetDirection(GetDlgItemInt(IDC_MONSTER_DIRECTION));
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonster = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMonster::OnEnChangeMonsterQuantity()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonster->SetQuantity(GetDlgItemInt(IDC_MONSTER_QUANTITY));
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonster = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMonster::OnEnChangeMonsterTimeBorn()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonster->SetTimeBorn(GetDlgItemInt(IDC_MONSTER_TIME_BORN));
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonster = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMonster::OnEnChangeMonsterTimeStart()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonster->SetTimeStart(GetDlgItemInt(IDC_MONSTER_TIME_START));
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonster = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMonster::OnBnClickedCheckBorn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton *pbtn = (CButton *)GetDlgItem(IDC_CHECK_BORN);
	if (m_pMonster->GetSwitch())
	{
		pbtn->SetCheck(BST_UNCHECKED);
		m_pMonster->SetSwitch(false);
	}
	else
	{
		pbtn->SetCheck(BST_CHECKED);
		m_pMonster->SetSwitch();
	}

	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonster = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

BOOL CDlgMonster::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pWndParent = GetParent();
	m_pMonster=NULL;
	m_ptreeRegion = ((CUniEditorDlg *)m_pWndParent)->GetTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlgMonster::PreTranslateMessage(MSG* pMsg)
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