// CheckInCase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameTest.h"
#include "CheckInCase.h"
#include ".\checkincase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CCheckInCase �Ի���

IMPLEMENT_DYNAMIC(CCheckInCase, CDialog)
CCheckInCase::CCheckInCase(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckInCase::IDD, pParent)
	, m_strCasePath(_T(""))
	, m_strClassName(_T(""))
{
}

CCheckInCase::~CCheckInCase()
{
}

void CCheckInCase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CasePath, m_strCasePath);
	DDX_Control(pDX, IDC_EDIT_CasePath, m_edCasePath);
	DDX_Text(pDX, IDC_STATIC_ClassName, m_strClassName);
}


BEGIN_MESSAGE_MAP(CCheckInCase, CDialog)
	ON_BN_CLICKED(IDC_Check, OnBnClickedCheck)
END_MESSAGE_MAP()


// CCheckInCase ��Ϣ�������

void CCheckInCase::OnBnClickedCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString str;
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = name;
	bi.lpszTitle = "ѡ����";
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if(idl == NULL)
		return;
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	m_strCasePath = str;			//Ϊ�Ի�������һ�༭���Ӧ��CString�ͱ��������沢��ʾѡ�е�·����
	//if(str.GetAt(str.GetLength()-1)!='\\')
	//	m_strCasePath+="\\";
	m_edCasePath.SetWindowText(m_strCasePath);
	UpdateData(FALSE); 
}
