// QueryFrame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QuestEditor.h"
#include "QueryFrame.h"


// CQueryFrame �Ի���

IMPLEMENT_DYNAMIC(CQueryFrame, CDialog)

CQueryFrame::CQueryFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryFrame::IDD, pParent)
    , m_questname(_T(""))
{

}

CQueryFrame::~CQueryFrame()
{
}

void CQueryFrame::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT2, m_questname);
    DDX_Control(pDX, IDC_COMBO1, m_ComBox);
}


BEGIN_MESSAGE_MAP(CQueryFrame, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &CQueryFrame::OnBnClickedButton1)
END_MESSAGE_MAP()


// CQueryFrame ��Ϣ�������

void CQueryFrame::OnBnClickedButton1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_Sel = m_ComBox.GetCurSel();
    OnOK();
}
