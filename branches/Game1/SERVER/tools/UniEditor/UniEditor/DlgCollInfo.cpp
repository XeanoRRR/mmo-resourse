// DlgCollInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditor.h"
#include "DlgCollInfo.h"
#include "UniEditorDlg.h"

// CDlgCollInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgCollInfo, CDialog)

CDlgCollInfo::CDlgCollInfo(CWnd* pParent /*=NULL*/)
	: CDlgEdit(CDlgCollInfo::IDD, pParent)
	
{

}

CDlgCollInfo::~CDlgCollInfo()
{
}

void CDlgCollInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCollInfo, CDialog)
	ON_CBN_SELCHANGE(IDC_COLL_LIST, &CDlgCollInfo::OnCbnSelchangeCollList)
	ON_EN_CHANGE(IDC_COLL_PROBABILITY, &CDlgCollInfo::OnEnChangeCollProbability)
	ON_CBN_EDITCHANGE(IDC_COLL_LIST, &CDlgCollInfo::OnCbnEditchangeCollList)
END_MESSAGE_MAP()
string Name(string str)
{
	int i=0;
	int j;
	char strVal[100];
	for(;;i++)
	{
		if(34==str[i])
		{
			for(j=0;34!=str[i+1];j++,i++)	
				strVal[j]=str[i+1];
			strVal[j]='\0';
			return strVal;	
		}
	}
	return strVal;
}

// CDlgCollInfo ��Ϣ�������
BOOL CDlgCollInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ifstream file;
	string strTemp;
	string str;
	file.open(strFileCollection, ios_base::in);
	int i=0;
	m_pCollectionList = (CComboBox *)(GetDlgItem(IDC_COLL_LIST));
	if (file.is_open())
	{
		while(CUniEditorApp::ReadTo("<Collection",file))
		{
			file>>str>>strTemp;
			string str1=Name(str);
			string str2=Name(strTemp);
			string str3=str1+"<"+str2+">";
			m_pCollectionList->AddString(str3.c_str());
		}
	} 
	else
	{
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_ERROR, _T("�޷���ʼ�������б�"));
	}

	file.close();
	m_pWndParent = GetParent();
	m_ptreeRegion = ((CUniEditorDlg *)m_pWndParent)->GetTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CDlgCollInfo::SetDlg(CRegion *pRegion, CCollectionInfo *pCollectionInfo, BOOL bALL)
{
	m_pCollInfo = pCollectionInfo;
	m_hItem = m_ptreeRegion->GetItem();
	m_uOffset= m_ptreeRegion->GetOffset(m_hItem);
	m_pRegion=pRegion;
	//pmonsterInfo->MonsterInfo2String(m_szModifyNode);
 	SetDlgItemText(IDC_COLL_LIST,m_pCollInfo->GetName().c_str());
	SetDlgItemInt(IDC_COLL_PROBABILITY,m_pCollInfo->GetProbability());
	CDlgEdit::SetDlg(pRegion, m_pCollInfo);
}
inline string GetNameString(string str)
{
	char strVal[100];
	int i=0,j=0;
	for(;60!=str[i];i++)
	{	
	}
	i+=1;
	for(;62!=str[i];i++,j++)	
	{
		strVal[j]=str[i];
		if(62==str[i+1])
		{
			strVal[j+1]='\0';
			break;
		}
	}
	return strVal;
}
void CDlgCollInfo::OnCbnSelchangeCollList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str(_T(""));
	m_pCollectionList->GetLBText(m_pCollectionList->GetCurSel(), str);
	string strName=GetNameString(str.GetString()).c_str();
	m_pRegion->GetCollection();
	m_pCollInfo->SetName(str.GetString());
	
	// �������οؼ�
	m_ptreeRegion->SetItemText(m_ptreeRegion->GetItem(), str);
	
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateCollInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(1);
	}
}

void CDlgCollInfo::OnEnChangeCollProbability()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDlgEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pRegion->GetCollection();
	m_pCollInfo->SetProbability(GetDlgItemInt(IDC_COLL_PROBABILITY));
	m_pRegion->GetCollection();
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateCollInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(1);
	}
}

void CDlgCollInfo::OnCbnEditchangeCollList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_Auto)
		return;
	CString str;
	m_pCollectionList->GetWindowText(str);
	int nLength = str.GetLength();
	DWORD dwCurSel = m_pCollectionList->GetEditSel();
	DWORD dStart = LOWORD(dwCurSel);
	DWORD dEnd = HIWORD(dwCurSel);
	if(m_pCollectionList->SelectString(-1,str) == CB_ERR)
	{	
		
		m_pCollectionList->SetWindowText(str);
		if(dwCurSel != CB_ERR)
		{
			
			m_pCollectionList->SetEditSel(dStart,dEnd);
		}
	}
	else
	{
		m_pCollectionList->GetWindowText(str);
		m_pCollectionList->CComboBox::ShowDropDown(TRUE);
		if(dEnd < nLength && dwCurSel != CB_ERR)
			m_pCollectionList->SetEditSel(dStart,dEnd);
		else
			m_pCollectionList->SetEditSel(nLength,-1);
	}
	
	int nIndex; 
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	nIndex= m_pCollectionList->FindString(-1, str);
	if(nIndex>0)
	m_pCollectionList->GetLBText(m_pCollectionList->GetCurSel(), str);
	else
	m_pCollectionList->GetLBText(1, str);
	string strName=GetNameString(str.GetString()).c_str();
	m_pCollInfo->SetName(str.GetString());
	m_ptreeRegion->SetItemText(m_ptreeRegion->GetItem(), str);
}
BOOL CDlgCollInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			
			m_Auto=TRUE;
			switch(pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;

			case VK_RETURN:
				{
					pMsg->wParam = VK_TAB;
					break;
				}

			case VK_DELETE:
				m_Auto=FALSE;
			case VK_BACK:
				m_Auto=FALSE;
			default:
				break;
			}//switch
			break;
		}
	default:
		break;
	}//switch

	return CDialog::PreTranslateMessage(pMsg);
}