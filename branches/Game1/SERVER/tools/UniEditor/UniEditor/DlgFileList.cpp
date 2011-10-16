// DlgFileList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditor.h"
#include "DlgFileList.h"
#include "UniEditorDlg.h"


// CDlgFileList �Ի���

IMPLEMENT_DYNAMIC(CDlgFileList, CDialog)

CDlgFileList::CDlgFileList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileList::IDD, pParent)
{

}

CDlgFileList::~CDlgFileList()
{
}

void CDlgFileList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST, m_listFile);
}


BEGIN_MESSAGE_MAP(CDlgFileList, CDialog)
	ON_LBN_SETFOCUS(IDC_FILELIST, &CDlgFileList::OnLbnSetfocusFilelist)
	ON_LBN_KILLFOCUS(IDC_FILELIST, &CDlgFileList::OnLbnKillfocusFilelist)
	ON_BN_CLICKED(IDC_SAVE, &CDlgFileList::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SAVE_ALL, &CDlgFileList::OnBnClickedSaveAll)
	ON_LBN_SELCANCEL(IDC_FILELIST, &CDlgFileList::OnLbnSelcancelFilelist)
	ON_LBN_SELCHANGE(IDC_FILELIST, &CDlgFileList::OnLbnSelchangeFilelist)
END_MESSAGE_MAP()


// CDlgFileList ��Ϣ�������

BOOL CDlgFileList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strFileName(_T(""));
	SETUPDATE &setList = ((CUniEditorDlg *)GetParent())->GetUpdateList();
	for(SETUPDATEITR itr=setList.begin(); itr!=setList.end(); ++itr)
	{
		m_listFile.AddString(itr->c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CDlgFileList::OnLbnSetfocusFilelist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgFileList::OnLbnKillfocusFilelist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgFileList::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iCurSel = m_listFile.GetCurSel();
	CString szFileName = _T("");

	m_listFile.GetText(iCurSel, szFileName);
	SETUPDATE &setList = ((CUniEditorDlg *)GetParent())->GetUpdateList();
	if (((CUniEditorDlg *)GetParent())->SaveFile(szFileName))
	{
		m_listFile.DeleteString(iCurSel);
		setList.erase(szFileName.GetString());
	}

	if(0 != m_listFile.GetCount())
	{
		m_listFile.SetCurSel(0);
	}

	else
	{
		GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SAVE_ALL)->EnableWindow(FALSE);
		EndDialog(0);
	}
}

void CDlgFileList::OnBnClickedSaveAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iCount = m_listFile.GetCount();
	CString szFileName = _T("");
	SETUPDATE &setList = ((CUniEditorDlg *)GetParent())->GetUpdateList();
	
	for(int i=0; i<iCount; ++i)
	{
		m_listFile.GetText(i, szFileName);
		if (((CUniEditorDlg *)GetParent())->SaveFile(szFileName))
		{
			m_listFile.DeleteString(i);
			--i;
			--iCount;
		}

	}

	if(0 != m_listFile.GetCount())
	{
		m_listFile.SetCurSel(0);
	}
	else
	{
		setList.clear();
		m_listFile.ResetContent();
		((CUniEditorDlg *)GetParent())->GetDlgItem(IDC_BTN_SAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SAVE_ALL)->EnableWindow(FALSE);
		EndDialog(0);
	}
}

void CDlgFileList::OnLbnSelcancelFilelist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_SAVE_ALL)->EnableWindow(FALSE);
}

void CDlgFileList::OnLbnSelchangeFilelist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCurSel = m_listFile.GetCurSel();

	GetDlgItem(IDC_SAVE)->EnableWindow();
	GetDlgItem(IDC_SAVE_ALL)->EnableWindow();
}
