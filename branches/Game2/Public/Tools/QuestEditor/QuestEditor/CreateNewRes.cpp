// CreateNewRes.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QuestEditor.h"
#include "CreateNewRes.h"


// CCreateNewRes �Ի���

IMPLEMENT_DYNAMIC(CCreateNewRes, CDialog)

CCreateNewRes::CCreateNewRes(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateNewRes::IDD, pParent)
    , m_fileresname(_T(""))
{

}

CCreateNewRes::~CCreateNewRes()
{
}

void CCreateNewRes::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_fileresname);
}


BEGIN_MESSAGE_MAP(CCreateNewRes, CDialog)
END_MESSAGE_MAP()


// CCreateNewRes ��Ϣ�������
