// DlgMoninfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditor.h"
#include "UniEditorDlg.h"
#include "DlgMoninfo.h"


// CDlgMoninfo �Ի���

IMPLEMENT_DYNAMIC(CDlgMoninfo, CDialog)

CDlgMoninfo::CDlgMoninfo(CWnd* pParent /*=NULL*/)
	: CDlgEdit(CDlgMoninfo::IDD, pParent)
{
	m_pMonsterList = NULL;
	m_pMonsterInfo = NULL;
	m_ptreeRegion = NULL;
	m_pWndParent = NULL;
}

CDlgMoninfo::~CDlgMoninfo()
{
}

void CDlgMoninfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgMoninfo, CDialog)
	ON_EN_CHANGE(IDC_MONSTER_DISTANCE, &CDlgMoninfo::OnEnChangeMonsterDistance)
	ON_EN_CHANGE(IDC_MONSTER_IDENTIFIER, &CDlgMoninfo::OnEnChangeMonsterIdentifier)
	ON_EN_CHANGE(IDC_MONSTER_LEADER, &CDlgMoninfo::OnEnChangeMonsterLeader)
	ON_EN_CHANGE(IDC_MONSTER_PROBABILITY, &CDlgMoninfo::OnEnChangeMonsterProbability)
	ON_EN_CHANGE(IDC_MONSTER_SCRIPT, &CDlgMoninfo::OnEnChangeMonsterScript)
	ON_CBN_SELCHANGE(IDC_MONSTER_LIST, &CDlgMoninfo::OnCbnSelchangeMonsterList)
	ON_CBN_EDITCHANGE(IDC_MONSTER_LIST, &CDlgMoninfo::OnCbnEditchangeMonsterList)
	ON_BN_CLICKED(IDC_BROWSE, &CDlgMoninfo::OnBnClickedBrowse)
	ON_EN_CHANGE(IDC_MONSTER_PATHID, &CDlgMoninfo::OnEnChangeMonsterPathid)
	ON_EN_CHANGE(IDC_MONSTER_TALKSCRIPT, &CDlgMoninfo::OnEnChangeMonsterTalkscript)
	ON_BN_CLICKED(IDC_TALKBROWSE, &CDlgMoninfo::OnBnClickedTalkbrowse)
END_MESSAGE_MAP()

void CDlgMoninfo::SetDlg(CRegion *pRegion, CMonsterInfo *pmonsterInfo, BOOL bALL)
{
	m_pMonsterInfo = pmonsterInfo;
	m_hItem = m_ptreeRegion->GetItem();
	m_uOffset= m_ptreeRegion->GetOffset(m_hItem);
	pmonsterInfo->MonsterInfo2String(m_szModifyNode);

	int iInd = m_pMonsterList->FindString(-1, pmonsterInfo->GetName().c_str());
	m_pMonsterList->SetCurSel(iInd);
	SetDlgItemInt(IDC_MONSTER_PROBABILITY, pmonsterInfo->GetProbability());
	SetDlgItemInt(IDC_MONSTER_IDENTIFIER, pmonsterInfo->GetIdentifier());
	SetDlgItemInt(IDC_MONSTER_LEADER, pmonsterInfo->GetLeader());
	SetDlgItemInt(IDC_MONSTER_DISTANCE, pmonsterInfo->GetDistance());
	SetDlgItemInt(IDC_MONSTER_PATHID,pmonsterInfo->GetPathIdentifier());
	SetDlgItemText(IDC_MONSTER_SCRIPT, pmonsterInfo->GetScript().c_str());
	SetDlgItemText(IDC_MONSTER_TALKSCRIPT,pmonsterInfo->GetTalkScript().c_str());
	CDlgEdit::SetDlg(pRegion, pmonsterInfo);
}

// CDlgMoninfo ��Ϣ�������
//������ƶ�����*��֮��
int ReadTo1(ifstream &stream,string name)
{
		string str;
	stream >> str;
	while (str != name)
	{
		//����ѵ��ļ�ĩβ������0
		if (stream.eof())
		{

			return 0;
		}
		//��ȡ��һ����
		stream >> str;
	}
	return 1;
}
BOOL CDlgMoninfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ifstream ifStream;
	string strTemp;
	string str;
	ifStream.open(strFileMonster, ios_base::in);
	int i=0;
	m_pMonsterList = (CComboBox *)(GetDlgItem(IDC_MONSTER_LIST));
	
	if (ifStream.is_open())
	{
		while(ReadTo1(ifStream,"10009001")!=0)
		{
			ifStream >> strTemp >> str;
			str+="<"+strTemp+">";
			m_pMonsterList->AddString(str.c_str());
			while(ReadTo1(ifStream,"*")!=0)
			{
				ifStream >> strTemp >> str>>strTemp;

		
				strTemp+="<"+str+">";
				m_pMonsterList->AddString(strTemp.c_str());
				
			}
		}
	} 
	else
	{
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_ERROR, _T("�޷���ʼ�������б�"));
	}

	ifStream.close();
	m_pWndParent = GetParent();
	m_ptreeRegion = ((CUniEditorDlg *)m_pWndParent)->GetTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgMoninfo::OnEnChangeMonsterDistance()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	unsigned	uOriValue(m_pMonsterInfo->GetDistance()), uCurValue(GetDlgItemInt(IDC_MONSTER_DISTANCE));
	m_pMonsterInfo->SetDistance(uCurValue);

	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMoninfo::OnEnChangeMonsterIdentifier()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonsterInfo->SetIdentifier(GetDlgItemInt(IDC_MONSTER_IDENTIFIER));

	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMoninfo::OnEnChangeMonsterLeader()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonsterInfo->SetLeader(GetDlgItemInt(IDC_MONSTER_LEADER));

	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMoninfo::OnEnChangeMonsterProbability()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonsterInfo->SetProbability(GetDlgItemInt(IDC_MONSTER_PROBABILITY));

	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMoninfo::OnEnChangeMonsterScript()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�,����������
	// ���͸�֪ͨ,������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask(),
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str(_T(""));
	GetDlgItemText(IDC_MONSTER_SCRIPT, str);
	if(""!=str)
	m_pMonsterInfo->SetScript(str.GetString());
	else
	m_pMonsterInfo->SetScript("0");
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}
//inline string GetNameString1(string str)
//{
//	char strVal[100];
//	int i=0;
//	for(;60!=str[i];i++)
//	{
//		strVal[i]=str[i];
//		if(60==str[i+1])
//		{
//			strVal[i+1]='\0';
//			break;
//		}
//	}
//	return strVal;
//}
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
void CDlgMoninfo::OnCbnSelchangeMonsterList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str(_T(""));
	m_pMonsterList->GetLBText(m_pMonsterList->GetCurSel(), str);
	string strName=GetNameString(str.GetString()).c_str();
	m_pMonsterInfo->SetName(str.GetString());

	// �������οؼ�
	m_ptreeRegion->SetItemText(m_ptreeRegion->GetItem(), str.GetString());

	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

BOOL CDlgMoninfo::PreTranslateMessage(MSG* pMsg)
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

void CDlgMoninfo::OnCbnEditchangeMonsterList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	
	if(!m_Auto)
		return;
	CString str,str1;
	m_pMonsterList->GetWindowText(str);
	int nLength = str.GetLength();
	DWORD dwCurSel = m_pMonsterList->GetEditSel();
	DWORD dStart = LOWORD(dwCurSel);
	DWORD dEnd = HIWORD(dwCurSel);
	if(m_pMonsterList->SelectString(-1,str) == CB_ERR)
	{	
		
		m_pMonsterList->SetWindowText(str);
		if(dwCurSel != CB_ERR)
		{
			
			m_pMonsterList->SetEditSel(dStart,dEnd);
		}
	}
	else
	{
		m_pMonsterList->GetWindowText(str);
	//	::SendMessage(m_pMonsterList->GetSafeHwnd(), CB_SHOWDROPDOWN, 1,0);
		m_pMonsterList->CComboBox::ShowDropDown(TRUE);
		if(dEnd < nLength && dwCurSel != CB_ERR)
			m_pMonsterList->SetEditSel(dStart,dEnd);
		else
			m_pMonsterList->SetEditSel(nLength,-1);
	}
	int nIndex; 
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	nIndex= m_pMonsterList->FindString(-1, str);
	if(nIndex>0)
	m_pMonsterList->GetLBText(m_pMonsterList->GetCurSel(), str);
	else
	m_pMonsterList->GetLBText(1, str);
	string strName=GetNameString(str.GetString()).c_str();
	m_pMonsterInfo->SetName(str.GetString());
	m_ptreeRegion->SetItemText(m_ptreeRegion->GetItem(), str);
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMoninfo::OnBnClickedBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		SetDlgItemText(IDC_MONSTER_SCRIPT,FilePathName);
		if(!CUniEditorDlg::m_bSelChange)
		{
			CUniEditorDlg::m_bUpdateMonInfo = TRUE;
			((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
		}
	}
}

void CDlgMoninfo::OnEnChangeMonsterPathid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDlgEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pMonsterInfo->SetPathIdentifier(GetDlgItemInt(IDC_MONSTER_PATHID));

	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMoninfo::OnEnChangeMonsterTalkscript()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDlgEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str(_T(""));
	GetDlgItemText(IDC_MONSTER_TALKSCRIPT, str);
	if(""!=str)
		m_pMonsterInfo->SetTalkScript(str.GetString());
	else
		m_pMonsterInfo->SetTalkScript("0");
	if(!CUniEditorDlg::m_bSelChange)
	{
		CUniEditorDlg::m_bUpdateMonInfo = TRUE;
		((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
	}
}

void CDlgMoninfo::OnBnClickedTalkbrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		SetDlgItemText(IDC_MONSTER_TALKSCRIPT,FilePathName);
		if(!CUniEditorDlg::m_bSelChange)
		{
			CUniEditorDlg::m_bUpdateMonInfo = TRUE;
			((CUniEditorDlg *)m_pWndParent)->UpdateFileList(0);
		}
	}
}
