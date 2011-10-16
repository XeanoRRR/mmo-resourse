// VersionDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PackageTool.h"
#include "VersionDialog.h"
#include ".\versiondialog.h"


// CVersionDialog �Ի���

IMPLEMENT_DYNAMIC(CVersionDialog, CDialog)
CVersionDialog::CVersionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CVersionDialog::IDD, pParent)
	, m_strOldVersion(_T(""))
	, m_strNewVersion(_T(""))
	, m_bSelPack(false)
{
	m_nPackType = 1;
}

// ���캯��
CVersionDialog::CVersionDialog(PackTypeInfos& PackInfo,bool bSelPack,CWnd* pParent)
	: CDialog(CVersionDialog::IDD, pParent)
	, m_strOldVersion(_T(""))
	, m_strNewVersion(_T(""))
	,m_PackageSets(PackInfo)
	,m_bSelPack(bSelPack)
{
	tagPackFileInfo	Pack = {1,"Ĭ��"};
	m_PackageSets.push_front(Pack);

	m_nPackType = 1;
}

CVersionDialog::~CVersionDialog()
{
}

void CVersionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_OLDVERSION, m_strOldVersion);
	DDV_MaxChars(pDX, m_strOldVersion, 10);
	DDX_Text(pDX, IDC_EDIT_NEWVERSION, m_strNewVersion);
	DDV_MaxChars(pDX, m_strNewVersion, 10);
}


BEGIN_MESSAGE_MAP(CVersionDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PATCH_PACK, OnCbnSelchangeComboPatchPack)
END_MESSAGE_MAP()


// CVersionDialog ��Ϣ�������

BOOL CVersionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CComboBox* pComboPack = (CComboBox*)GetDlgItem(IDC_COMBO_PATCH_PACK);	
	if( pComboPack == NULL)	return false;
	pComboPack->Clear();

	if(m_bSelPack)
	{
		DWORD dwSize = 0;
		pComboPack->EnableWindow(true);
		itTypeInfo it = m_PackageSets.begin();
		for(;it != m_PackageSets.end();it++,dwSize++)
		{
			pComboPack->AddString( (*it).strFileName.c_str());
		}
		pComboPack->SetCurSel(dwSize-1);
		m_nPackType = GetPackIDByPos(dwSize-1);

	}
	else
	{
		pComboPack->EnableWindow(false);
		pComboPack->SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CVersionDialog::OnCbnSelchangeComboPatchPack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox* pComboPack= (CComboBox*)GetDlgItem(IDC_COMBO_PATCH_PACK);
	if( pComboPack == NULL)	return;

	int nPos = pComboPack->GetCurSel();
	int nPackID = GetPackIDByPos(nPos);

	m_nPackType = nPackID;
}

int		CVersionDialog::GetPackIDByPos(int nPos)
{
	int n = 0;
	itTypeInfo it = m_PackageSets.begin();
	for(;it != m_PackageSets.end();it++,n++)
	{
		if( n == nPos )	return (*it).dwID;
	}
	return 1;
}
