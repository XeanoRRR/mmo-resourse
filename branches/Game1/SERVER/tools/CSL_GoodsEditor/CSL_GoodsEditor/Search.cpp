/// ����������Ʒ
/// Add by Ф�� 2010.5.21

// Search.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CSL_GoodsEditor.h"
#include "Search.h"
#include "MainFrm.h"
#include "GoodAttrPage.h"
#include "ContainerPage.h"

extern vector<tagGoodAttr> vecGoodAttr; 

// Search �Ի���
IMPLEMENT_DYNAMIC(Search, CDialog)

Search::Search(CWnd* pParent):CDialog(Search::IDD, pParent)
{
	m_preSerResult = -1;
}

Search::~Search()
{
	m_preSerResult = -1;
}

void Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCHOPTION, m_choseType);
	DDX_Control(pDX, IDC_SEARCHCONTENT, m_SerContent);
}


BEGIN_MESSAGE_MAP(Search, CDialog)
	ON_CBN_SELCHANGE(IDC_SEARCHOPTION, &Search::OnCbnSelchangeSearchoption)
	ON_EN_KILLFOCUS(IDC_SEARCHCONTENT, &Search::OnEnKillfocusSearchcontent)
	ON_BN_CLICKED(IDCANCEL, &Search::OnBnClickedCancel)
	ON_BN_CLICKED(ID_NEXT, &Search::OnBnClickedNext)
END_MESSAGE_MAP()


// Search ��Ϣ�������

void Search::OnCbnSelchangeSearchoption()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_choseType.GetCurSel();
	//switch(index)
	//{
	//case ST_ID:
	//	break;
	//case ST_ORINAME:
	//	break;
	//case ST_SHOWNAME:
	//    break;
	//default:
	//    break;
	//}
}

//void Search::OnEnChangeSearchcontent()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
//	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}

void Search::OnEnKillfocusSearchcontent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//void Search::OnBnClickedOk()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	//OnOK();
//}

void Search::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_preSerResult = -1;
	OnCancel();
}

// ����ַ����Ƿ�������
bool Search::CheckStrIsNum(const char* str,DWORD len)
{
	if(str==NULL) return false;
	char* _tstr = new char[len+1];
	strcpy_s(_tstr,len+1,str);
	_tstr[len] = '\0';
	DWORD count = 0;
	while (count<len)
	{
		if (_tstr[count]<=57&&_tstr[count]>=48) 
			count++;
		else
		{	
			delete _tstr;
			return false;
		}
	}
	delete _tstr;
	return true;
}

void Search::OnBnClickedNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szContent[128] = "";
	LONG val = 0;
	int nSelected=0;
	if(m_SerContent.GetWindowText(szContent,128)==0) return;
	vector<tagGoodAttr>::iterator iter = vecGoodAttr.begin();
	switch(m_choseType.GetCurSel())
	{
		//����ƷIDֵ����
	case 0:
		if (!CheckStrIsNum(szContent,(DWORD)strlen(szContent)))	//���༭���е������Ƿ�������
		{
			MessageBox("����ƷID����ʱ���������֣�","��ʾ",MB_OK);
			return;
		}
		val = atol(szContent);
		for(;iter!=vecGoodAttr.end();iter++,nSelected++)
		{			
			if(iter->dwIndex==val)
				break;
		}
		break;
		//����Ʒԭʼ������
	case 1:
		for(;iter!=vecGoodAttr.end();iter++,nSelected++)
		{			
			if(strcmp(iter->strOrigName.c_str(),szContent)==0
				&&nSelected>m_preSerResult)
			break;
		}
		break;
		//����Ʒ��ʾ������
	case 2:
		for(;iter!=vecGoodAttr.end();iter++,nSelected++)
		{			
			if(strcmp(iter->strName.c_str(),szContent)==0
				&&nSelected>m_preSerResult)
				break;
		}
	    break;
	default:
		MessageBox("û�д���������ʽ��","����",MB_OK);
		OnCancel();
		return;
	}

	if (iter==vecGoodAttr.end()&&m_preSerResult==-1)
	{
		MessageBox("û���ҵ�����������Ʒ��","��ʾ",MB_OK);
		return;
	}

	if (m_preSerResult!=nSelected&&iter!=vecGoodAttr.end())
		m_preSerResult = nSelected;
	else
	{
		m_preSerResult = -1;
		MessageBox("�Ѳ����¶�������","��ʾ",MB_OK);
		return;
	}

	/// ����ѡ���������ʽ��������
	CSplitterWnd& pSplitter = ((CMainFrame*)GetParent())->GetSplitterWnd();
	CContainerPage *pContainer=(CContainerPage*)pSplitter.GetPane(0,1);
	CAllGoodsTree *pGoodsTree=(CAllGoodsTree*)pSplitter.GetPane(0,0);

	pContainer->m_wndGoodAttrPage.SetEditFlag(false);
	pContainer->m_wndGoodAttrPage.ClearCtrls();
	pContainer->m_wndGoodAttrPage.FillCtrls(m_preSerResult);		
	pContainer->m_wndAllGoodsPage.ShowWindow(SW_HIDE);
	pContainer->m_wndGoodAttrPage.ShowWindow(SW_NORMAL);		
	pContainer->m_wndGoodAttrPage.SetEditFlag(true);
}
