// Encouragement.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "QuestEditor.h"
#include "CEncouragement.h"
#include "tinyxml.h"
#include <string.h>
#include <vector>
#include "CXmlEdit.h"
#include "CFileSave.h"
#include "Tools.h"
// Encouragement �Ի���
IMPLEMENT_DYNAMIC(CEncouragement, CDialog)
//���캯��
CEncouragement::CEncouragement(CWnd* pParent /*=NULL*/)
	: CDialog(CEncouragement::IDD, pParent)
	, m_EncouragementCell(0)
	, m_EncouragementName(_T(""))
	, m_EncouragementNum(0)
    , m_textDes(_T(""))
{
    isLoading = false;
}
//��������
CEncouragement::~CEncouragement()
{
}
void CEncouragement::DoDataExchange(CDataExchange* pDX)
{

    CDialog::DoDataExchange(pDX);

    if( !isLoading )
    {
        DDX_Control(pDX, IDC_LIST1, m_EncouragementList);
        m_EncouragementList.SetExtendedStyle(
            LVS_EX_FLATSB
            |LVS_EX_FULLROWSELECT
            |LVS_EX_HEADERDRAGDROP
            |LVS_EX_ONECLICKACTIVATE
            |LVS_EX_GRIDLINES
            );
        m_EncouragementList.InsertColumn(0,"����",LVCFMT_LEFT,100,0);
        m_EncouragementList.InsertColumn(1,"����",LVCFMT_LEFT,700,0);
        DDX_Text(pDX, IDC_EDIT2, m_textDes);
        isLoading = true;
    }

    DDX_Control(pDX, IDC_EDIT1, m_TextIndex);
    DDX_Control(pDX, IDC_EDIT3, m_TextContext);
}
void CEncouragement::OnOK()
{
}

BEGIN_MESSAGE_MAP(CEncouragement, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CEncouragement::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CEncouragement::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEncouragement::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CEncouragement::OnNMClickList1)
    ON_EN_CHANGE(IDC_EDIT3, &CEncouragement::OnEnChangeEdit3)
    ON_EN_CHANGE(IDC_EDIT1, &CEncouragement::OnEnChangeEdit1)
END_MESSAGE_MAP()
/*����:ʵ������������ʾ��
*ժҪ:���������ID��ȡ������Ȼ����ʾ����������б��е������������ʱ����øú�����
*����:pItemText�������б��нڵ������,�����������ID
*����ֵ:void
*���ߣ��� ��
*��������:2010-7-6
*/
void CEncouragement::ReadRoleEncouragement(CString *pItemText)
{
	m_EncouragementList.DeleteAllItems();

    CXmlEdit::MapTextFileTable::iterator  MapIter = GetInst(CXmlEdit).m_AllQuestTextFile.find( *pItemText );

    if ( MapIter!= GetInst(CXmlEdit).m_AllQuestTextFile.end() )
    {
        m_Textfile = *pItemText;

        map<CString,CString>::iterator  TextIter = MapIter->second.begin();

        for ( int i = 0 ; TextIter!= MapIter->second.end() ; i++ , TextIter++)
        {
            m_EncouragementList.InsertItem(i,"");
            m_EncouragementList.SetItemText(i,0,TextIter->first);
            m_EncouragementList.SetItemText(i,1,TextIter->second);
        }
    }
}
//�޸�������
void CEncouragement::OnBnClickedButton3()
{
	POSITION   pos   =   m_EncouragementList.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		return;		
	}
	int nSelectItem = m_EncouragementList.GetNextSelectedItem(pos);
	CString strType;
	CString strName;
	CString strNum;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strType);
	GetDlgItem(IDC_EDIT2)->GetWindowText(strName);
	GetDlgItem(IDC_EDIT3)->GetWindowText(strNum);
	m_EncouragementList.SetItemText(nSelectItem,0,strType);
	m_EncouragementList.SetItemText(nSelectItem,1,strName);
	m_EncouragementList.SetItemText(nSelectItem,2,strNum);

}
//���������
void CEncouragement::OnBnClickedButton1()
{
	POSITION   pos   =   m_EncouragementList.GetFirstSelectedItemPosition();
	int ItemNum = m_EncouragementList.GetNextSelectedItem(pos);
	if (ItemNum<0)
	{
		ItemNum = 0;
	}
    CXmlEdit::MapTextFileTable::iterator  MapIter = GetInst(CXmlEdit).m_AllQuestTextFile.find( m_Textfile);
    if ( MapIter != GetInst(CXmlEdit).m_AllQuestTextFile.end() )
    {
        MapIter->second[ "Q" ] = ":";
    }
    else
    {
        return ;
    }

    m_TextIndex.SetWindowText("Q");
    m_TextContext.SetWindowText("");

    ReadRoleEncouragement( &m_Textfile );
    m_EncouragementList.EnsureVisible( 0 , FALSE );
    m_EncouragementList.SetItemState(  0 , LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
    m_EncouragementList.SetFocus();
    m_EncouragementList.SetSelectedColumn(0);
}

//ɾ��
void CEncouragement::OnBnClickedButton2()
{
	POSITION   pos   =   m_EncouragementList.GetFirstSelectedItemPosition();
    if ( pos < 0 )
    {
        return;
    }
	int ItemNum   = m_EncouragementList.GetNextSelectedItem(pos);
    CString Context =  m_EncouragementList.GetItemText(ItemNum,0);
    CXmlEdit::MapTextFileTable::iterator  MapIter = GetInst(CXmlEdit).m_AllQuestTextFile.find( m_Textfile);
    if ( MapIter!= GetInst(CXmlEdit).m_AllQuestTextFile.end() )
    {
        map<CString,CString>::iterator  TextIter = MapIter->second.find( Context );
        if ( TextIter!= MapIter->second.end() )
        {
            MapIter->second.erase( TextIter );
        }
    }
	m_EncouragementList.DeleteItem(ItemNum);
}


//�����б��е�ĳһ�����Ӧ
void CEncouragement::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION   pos   =   m_EncouragementList.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{	
		return;		
	}
	int nSelectItem= m_EncouragementList.GetNextSelectedItem(pos);
	CString selectType =  m_EncouragementList.GetItemText(nSelectItem,0);
	CString selectNum =   m_EncouragementList.GetItemText(nSelectItem,1);

    GetDlgItem(IDC_EDIT1)->SetWindowText( selectType );
    GetDlgItem(IDC_EDIT3)->SetWindowText( selectNum );
	*pResult = 0;
}


bool CEncouragement::GetUpdatedEn(vector<tagRoleEncouragement*>& vcUpdatedeEn ,int &Cell)
{
	CString strCell;
    GetDlgItem(IDC_EDIT1)->GetWindowText(strCell);
	int ItemCount  = m_EncouragementList.GetItemCount();
	if (ItemCount==0)
	{	tagRoleEncouragement *En = new tagRoleEncouragement;
		En->m_nID = RoleId;
		vcUpdatedeEn.push_back(En);
	}
	else
	{
		for (int i = 0;i<ItemCount;i++)
		{	tagRoleEncouragement * En = new tagRoleEncouragement;
			En->m_nID = RoleId;
			En->m_nEncouragementCell = atoi(strCell);
			En->m_nNum = atoi(m_EncouragementList.GetItemText(i,1));
			En->m_nType = atoi(m_EncouragementList.GetItemText(i,0));
			En->m_cName = m_EncouragementList.GetItemText(i,2);
			vcUpdatedeEn.push_back(En);
		}
	}
	Cell = atoi(strCell);
	return true;
}
void CEncouragement::OnEnChangeEdit3()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
    // ���͸�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    POSITION   pos   =   m_EncouragementList.GetFirstSelectedItemPosition();
    if(pos==NULL)
    {	
        return;		
    }

    int nSelectItem= m_EncouragementList.GetNextSelectedItem(pos);
    CString selectType =  m_EncouragementList.GetItemText(nSelectItem,0);
    CString selectNum =   m_EncouragementList.GetItemText(nSelectItem,1);
    CXmlEdit::MapTextFileTable::iterator  MapIter = GetInst(CXmlEdit).m_AllQuestTextFile.find( m_Textfile);

    if ( MapIter!= GetInst(CXmlEdit).m_AllQuestTextFile.end() )
    {
        UpdateData( TRUE );

        map<CString,CString>::iterator  TextIter1 = MapIter->second.find( selectType );

        char  QText[5120];
        m_TextContext.GetWindowText( QText , 5120 );
        map<CString,CString>::iterator  TextIter2 = MapIter->second.find( QText );

        if ( strcmp( selectType.GetBuffer(),QText) == 0 )
        {
            return ;
        }
        if ( TextIter1 != MapIter->second.end() )
        {
            TextIter1->second = QText;
        }

        m_EncouragementList.SetItemText(nSelectItem,1,QText);
    }

}

void CEncouragement::OnEnChangeEdit1()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
    // ���͸�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    POSITION   pos   =   m_EncouragementList.GetFirstSelectedItemPosition();
    if(pos==NULL)
    {	
        return;		
    }

    
    int nSelectItem= m_EncouragementList.GetNextSelectedItem(pos);
    CString selectType =  m_EncouragementList.GetItemText(nSelectItem,0);
    CString selectNum =   m_EncouragementList.GetItemText(nSelectItem,1);
    CXmlEdit::MapTextFileTable::iterator  MapIter = GetInst(CXmlEdit).m_AllQuestTextFile.find( m_Textfile);

    if ( MapIter!= GetInst(CXmlEdit).m_AllQuestTextFile.end() )
    {
         UpdateData( TRUE );

         map<CString,CString>::iterator  TextIter1 = MapIter->second.find( selectType );

         char  QText[256];
         m_TextIndex.GetWindowText( QText , 256 );

         map<CString,CString>::iterator  TextIter2 = MapIter->second.find( QText );
         if ( strcmp( selectType.GetBuffer(),QText) == 0 )
         {
             return ;
         }
         if ( TextIter2 != MapIter->second.end() )
         {
             //MessageBox("�����ظ��������ı���");
             return ;
         }
         if ( TextIter1 != MapIter->second.end() )
         {
              MapIter->second.erase( TextIter1 );
              MapIter->second.insert( std::make_pair(QText,selectNum) );
         }

         m_EncouragementList.SetItemText(nSelectItem,0,QText);
    }
    
}
