// QuestEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QuestEditor.h"
#include "QuestEditorDlg.h"
#include "tinyxml.h"
#include "CXmlEdit.h"
#include "CAttribute.h"
#include "CStep.h"
#include "CEncouragement.h"
#include "CFileSave.h"
#include "Input.h"
#include "CreateNewRes.h"
#include "QueryFrame.h"
#include "StickQuest.h"


#define TabOfAttribute		0     //�������Ա�ǩ
#define TabOfStep			1	  //�������ǩ
#define TabOfEncouragement	2	  //��������ǩ
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog 
{
public:
	CAboutDlg();
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void Onload();
	afx_msg void OnSave();
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CQuestEditorDlg �Ի���

CQuestEditorDlg::CQuestEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Menu=::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU2));
    CopyId =0;
}

void CQuestEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, Tab);
	DDX_Control(pDX, IDC_TREE1, RoleList);
	DDX_Control(pDX, IDC_LIST1, m_Loglist);

}
CQuestEditorDlg::~CQuestEditorDlg()
{
}

BEGIN_MESSAGE_MAP(CQuestEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CQuestEditorDlg::OnNMRclickTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CQuestEditorDlg::OnTvnSelchangedTree1)
    ON_NOTIFY(TCN_SELCHANGE , IDC_TAB,   &CQuestEditorDlg::OnTvnSelchangedTab)
	ON_COMMAND(ID_32792, &CQuestEditorDlg::On32792)
	ON_COMMAND(ID_32791, &CQuestEditorDlg::OnExit)
    ON_BN_CLICKED(IDCANCEL, &CQuestEditorDlg::OnBnClickedCancel)
    ON_COMMAND(ID_32795, &CQuestEditorDlg::OnAddNewQuest)
    ON_COMMAND(ID_32794, &CQuestEditorDlg::OnDelQuest)
    ON_COMMAND(ID_32793, &CQuestEditorDlg::OnCopyQuest)
    ON_COMMAND(ID_32796, &CQuestEditorDlg::OnStickQuest)
    ON_COMMAND(ID_32797, &CQuestEditorDlg::OnCreateTextRes)
    ON_COMMAND(ID_32798, &CQuestEditorDlg::OnQuestQuery)
    ON_MESSAGE(WM_HOTKEY,&CQuestEditorDlg::OnHotKey)

    ON_COMMAND(ID_32802, &CQuestEditorDlg::OnOnlySaveQuest)
    ON_COMMAND(ID_32803, &CQuestEditorDlg::OnSaveAllRes)
    ON_COMMAND(ID_32804, &CQuestEditorDlg::OnReLoad)

END_MESSAGE_MAP()


// CQuestEditorDlg ��Ϣ�������

BOOL CQuestEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// ��ʼ����
    ShowTab();
	ShowRoleList();

    RegisterHotKey( GetSafeHwnd(),1001,NULL,VK_F3);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CQuestEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQuestEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CQuestEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CQuestEditorDlg::OnNMClickTab(int TalNum)
{
	if (TalNum==TabOfAttribute)
	{
		Attribute.ShowWindow(SW_SHOW);
		Step.ShowWindow(SW_HIDE);
		Encouragement.ShowWindow(SW_HIDE);
	}
	if (TalNum==TabOfStep)
	{
		Attribute.ShowWindow(SW_HIDE);
		Step.ShowWindow(SW_SHOW);
		Encouragement.ShowWindow(SW_HIDE);
	}
	if (TalNum==TabOfEncouragement)
	{
		Attribute.ShowWindow(SW_HIDE);
		Step.ShowWindow(SW_HIDE);
		Encouragement.ShowWindow(SW_SHOW);
	}
}

//��ʼ��Tab�ؼ�����
void CQuestEditorDlg::ShowTab()
{
	//ΪTab���3����ǩ
	Tab.InsertItem(0,_T("�������� + ������"));
	Tab.InsertItem(1,_T("������ + �����ı�"));
    Tab.InsertItem(2,_T("ȫ�������ı�"));

	//����3���Ի���
	Attribute.Create(IDD_DIALOG1,&Tab);
	Step.Create(IDD_DIALOG2,&Tab);
	Encouragement.Create(IDD_DIALOG3,&Tab);
	//�趨���ڵ���ʾ��Χ����ʾ��λ��
	CRect rc;
	Tab.GetClientRect(rc);

	rc.top += 20;
	rc.bottom += -5;
	rc.left += 5;
	rc.right += -5;

	Attribute.MoveWindow(&rc);
	Step.MoveWindow(&rc);
	Encouragement.MoveWindow(&rc);
	//���ó�ʼ������
	Attribute.ShowWindow(SW_SHOW);
	Step.ShowWindow(SW_HIDE);
	Encouragement.ShowWindow(SW_HIDE);
	m_CurSelTab = 0;
}
//��ʾ�����б��������ͼ��
void CQuestEditorDlg::ShowRoleList()
{
	char strVal[256];
	HTREEITEM	QuetRoote;
	HTREEITEM	TextRoote;
	HTREEITEM   RoleStep;
	QuetRoote = RoleList.InsertItem("�����б�",0,0);
	TextRoote = RoleList.InsertItem("�����ı���Դ",0,0);
	vector<tagRole*>           vcAllRole;
	vector<tagRole*>::iterator AllRileIter;
    CXmlEdit::MapFileTable     AllFiletable;
    CXmlEdit::MapTextFileTable AllTextRes;
    //vcAllRole   = GetInst(CXmlEdit).m_vcAllRole;
    AllFiletable= GetInst(CXmlEdit).m_AllQuestList;//;theApp.Filetable;
    AllTextRes  = GetInst(CXmlEdit).m_AllQuestTextFile;

    CXmlEdit::MapFileTable::iterator    fileIter = AllFiletable.begin();
    CXmlEdit::MapTextFileTable::iterator textIter = AllTextRes.begin();

    for ( ; fileIter!= AllFiletable.end(); fileIter ++  )
    {
        HTREEITEM RoleRoote = RoleList.InsertItem( fileIter->first,0,0,QuetRoote );
        vcAllRole   = (fileIter->second);
        AllRileIter = vcAllRole.begin();
        for (;AllRileIter!=vcAllRole.end();AllRileIter++)
        {
            tagRole* Role           = *AllRileIter;
            tagRoleAttribute* Attribute = Role->vcRole_Attribute;
            CString Rolename           = Attribute->m_cName.c_str();
            CString RoleId            = itoa(Attribute->m_nID,strVal,10);

            
            map<CString,CString>::iterator  sitr = GetInst(CXmlEdit).m_TextRes.find( "Q"+RoleId );        
            if( sitr != GetInst(CXmlEdit).m_TextRes.end() )
            {
                Rolename = sitr->second;
            }

            HTREEITEM RoleRootee = RoleList.InsertItem(RoleId+"("+Rolename+")",0,0,RoleRoote);
            RoleList.InsertItem("��������",0,0,RoleRootee);
            RoleStep = RoleList.InsertItem("������",0,0,RoleRootee);
        }
    }


    for ( ; textIter!= AllTextRes.end() ; textIter ++ )
    {
        HTREEITEM RoleRoote = RoleList.InsertItem( textIter->first,0,0,TextRoote );
    }
   
	RoleList.Expand(QuetRoote,TVE_EXPAND); 
	RoleList.Expand(TextRoote,TVE_EXPAND); 
}

//�������Ҽ��˵�
void CQuestEditorDlg::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu menu;
	CPoint pt;
	menu.LoadMenu(IDR_MENU1);
	CMenu* pMenu=menu.GetSubMenu(0);
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x,pt.y, this);
	*pResult = 0;
}
//���ػس�����Ӧ
void CQuestEditorDlg::OnOK()
{
	//�����Ӧ;
}

void CQuestEditorDlg::OnTvnSelchangedTab(NMHDR *pNMHDR, LRESULT *pResult)
{
     int CurSle = Tab.GetCurSel();

     switch( CurSle )
     {
     case 0:
         OnNMClickTab(TabOfAttribute);
         break;
     case 1:
         OnNMClickTab(TabOfStep);
         break;
     case 2:
         OnNMClickTab(TabOfEncouragement);
         break;
     }
}
//������������Ӧ
void CQuestEditorDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM selectedItem =RoleList.GetSelectedItem();
	HTREEITEM parentItem = RoleList.GetParentItem(selectedItem);
	CString ItemText = RoleList.GetItemText(selectedItem);
	CString RoleID = RoleList.GetItemText(parentItem);
	CString *pRoleId = &RoleID;
	if (ItemText=="��������")
	{
		Attribute.ReadRoleAttribute(pRoleId);
		OnNMClickTab(TabOfAttribute);
        Tab.SetCurSel( 0 );
	}
	if (ItemText=="������")
	{
		Step.ReadRoleStep(pRoleId);
		OnNMClickTab(TabOfStep);
        Tab.SetCurSel( 1 );
        Step.Showdetail(1);
	}
 	if (RoleID=="�����ı���Դ")
 	{
 		Encouragement.ReadRoleEncouragement(&ItemText);
		OnNMClickTab(TabOfEncouragement);
        Tab.SetCurSel( 2 );
 	}
	*pResult = 0;
}
//��־��ʾ
void CQuestEditorDlg::ShowLog(vector<string>strlog)
{
	int a  = m_Loglist.GetCount();
	if (!strlog.empty())
	{
		vector<string>::iterator iter;
		iter = strlog.begin();
		for (;iter!=strlog.end();iter++)
		{
			m_Loglist.InsertString(a,(*iter).c_str());
		}
		strlog.clear();
	}
}

//����
void CQuestEditorDlg::On32792()
{
	CFileSave objSave;
	objSave.OnReWrite();
    objSave.OnSaveRes();
}
//�˳�
void CQuestEditorDlg::OnExit()
{
	// TODO: �ڴ���������������
	SendMessage(WM_CLOSE);
}

void CQuestEditorDlg::OnBnClickedCancel()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    int nRet=MessageBox("�뿪ǰȷ�������˻��߲���Ҫ������?","",MB_YESNO);
    if( nRet == IDYES )
    {
        OnCancel();
    }   
}

void CQuestEditorDlg::OnAddNewQuest()
{
    // TODO: �ڴ���������������
    char strVal[256];
    CInput in;
    INT_PTR  Res =in.DoModal() ;
    if( Res == IDCANCEL )
        return ;
    bool bIsSame = false;
    std::map<long,CString>::iterator iter;

    iter = GetInst(CXmlEdit).m_AllQuestStrId.find( in.m_nInPutRoleID );

    //��ID����Ч�Խ����ж�
    if ( iter != GetInst(CXmlEdit).m_AllQuestStrId.end() )
    {
        bIsSame = true;
    }
    if (!bIsSame && in.m_nInPutRoleID > 100000)
    {
        sprintf_s( strVal , "quest/Quest%d.xml" , in.m_nInPutRoleID/1000 );
        CXmlEdit::MapFileTable::iterator itr = GetInst(CXmlEdit).m_AllQuestList.find( strVal ); 
        if ( itr != GetInst(CXmlEdit).m_AllQuestList.end() )
        {
            tagRole* quest = new  tagRole;
            quest->m_nID = in.m_nInPutRoleID;
            tagRoleAttribute* Attribute = new tagRoleAttribute;
            tagRoleStep*     Step = new tagRoleStep ;
            Attribute->m_nID = in.m_nInPutRoleID;
            Step->m_nID      = in.m_nInPutRoleID;
            quest->vcRole_Attribute = Attribute;
            quest->vcRole_Step      = Step;
            itr->second.push_back( quest );
            GetInst(CXmlEdit).m_AllQuestStrId[ in.m_nInPutRoleID ]  = strVal; 
        }
        else
        {
            tagRole* quest = new  tagRole;
            quest->m_nID = in.m_nInPutRoleID;
            tagRoleAttribute* Attribute = new tagRoleAttribute;
            tagRoleStep*     Step = new tagRoleStep ;
            Attribute->m_nID = in.m_nInPutRoleID;
            Step->m_nID      = in.m_nInPutRoleID;
            quest->vcRole_Attribute = Attribute;
            quest->vcRole_Step      = Step;
         
            GetInst(CXmlEdit).m_AllQuestList[ strVal ].push_back( quest );
            GetInst(CXmlEdit).m_AllQuestStrId[ in.m_nInPutRoleID ]  = strVal;

        }
        RoleList.DeleteAllItems();
        ShowRoleList();
    }
    else if (bIsSame)
    {
        MessageBox("�½�����ID��ͻ");
    }
    else if(in.m_nInPutRoleID < 100000 )
    {
        MessageBox("�½�����ID�����Ϲ淶");
    }
}


void CQuestEditorDlg::OnDelQuest()
{
    //��ȡ��Ӧ�ڵ����Ϣ
    HTREEITEM selectedItem = RoleList.GetSelectedItem();
    CString ItemText = RoleList.GetItemText(selectedItem);
    int SlecteRoleId  = atoi(ItemText);

    char  strVal[256];
    if ( SlecteRoleId > 100000 )
    {
        sprintf_s( strVal ," ȷ��ɾ������id:%d(%s)" , SlecteRoleId,ItemText.GetBuffer() );
        int nRet=MessageBox(strVal,"",MB_YESNO);
        if( nRet == IDYES )
        {
            //ɾ������
            std::map<long,CString>::iterator FileIter = GetInst(CXmlEdit).m_AllQuestStrId.find ( SlecteRoleId ); 
            if ( FileIter != GetInst(CXmlEdit).m_AllQuestStrId.end() )
            {
                CXmlEdit::MapFileTable::iterator  RoleIter = GetInst(CXmlEdit).m_AllQuestList.find( FileIter->second ) ;
                if ( RoleIter != GetInst(CXmlEdit).m_AllQuestList.end() )
                {
                    vector<tagRole*>::iterator iter;
                    iter = RoleIter->second.begin();
                    for ( ; iter != RoleIter->second.end() ; iter ++ )
                    {
                        if ( (*iter)->m_nID == SlecteRoleId )
                        {
                            RoleIter->second.erase( iter );
                            break;
                        }
                    }
                }
            }
            //ɾ������
            std::map<long,CString>::iterator   AwardFile = GetInst(CXmlEdit).m_AwardIdFileList.find( SlecteRoleId );
            if ( AwardFile == GetInst(CXmlEdit).m_AwardIdFileList.end() )
            {
                RoleList.DeleteAllItems();
                ShowRoleList();
                return;
            } 
            //= *Awardtable 
            CXmlEdit::MapAwardFileTable::iterator fileIter= GetInst(CXmlEdit).m_AllAwardList.find( AwardFile->second );
            if ( fileIter == GetInst(CXmlEdit).m_AllAwardList.end())
            {
                RoleList.DeleteAllItems();
                ShowRoleList();
                return ;
            }
            CXmlEdit::MapAwardTable::iterator   AwardIter = (*(fileIter->second)).find( SlecteRoleId );
            if ( AwardIter != (*(fileIter->second)).end() )
            {
                 (*(fileIter->second)).erase( AwardIter );
            }
            RoleList.DeleteAllItems();
            ShowRoleList();
        }

    }
    else
    {
        MessageBox("����ѡ�о�����Ҫɾ��������!");
    }

}

void CQuestEditorDlg::OnCopyQuest()
{
    //��ȡ��Ӧ�ڵ����Ϣ
    HTREEITEM selectedItem = RoleList.GetSelectedItem();
    CString ItemText = RoleList.GetItemText(selectedItem);
    int SlecteRoleId  = atoi(ItemText);
    char  strVal[256];
    if ( SlecteRoleId > 100000 )
    {
        sprintf_s( strVal ," ȷ����������id:%d(%s)" , SlecteRoleId,ItemText.GetBuffer() );
        int nRet=MessageBox(strVal,"",MB_YESNO);
        if( nRet == IDYES )
        {
            //����
            CopyId = SlecteRoleId;
        }
    }
    else
    {
        MessageBox("����ѡ�о�����Ҫ���Ƶ�����!");
    }
}

void CQuestEditorDlg::OnStickQuest()
{
    //��ȡ��Ӧ�ڵ����Ϣ
    HTREEITEM selectedItem = RoleList.GetSelectedItem();
    CString ItemText = RoleList.GetItemText(selectedItem);

    char  strVal[256] = "quest/Quest";
    if ( CopyId > 0 && strstr( ItemText.GetBuffer(),strVal) )
    {
        //sprintf_s( strVal ," ȷ��ճ������id:%d��" , CopyId);
        int nRet=MessageBox(strVal,"",MB_YESNO);
        if( nRet == IDYES )
        {
            //ɾ��
            CStickQuest  stick;
            if( stick.DoModal() == IDOK  )
            {
                long   NId = stick.m_QuestID;

                std::map<long,CString>::iterator FileIter = GetInst(CXmlEdit).m_AllQuestStrId.find ( NId ); 
                if ( FileIter != GetInst(CXmlEdit).m_AllQuestStrId.end() )
                {
                    MessageBox("���ظ�������ɣ�");
                    return ;
                }

                FileIter = GetInst(CXmlEdit).m_AllQuestStrId.find ( CopyId ); 
                tagRole* role = NULL;
                if ( FileIter != GetInst(CXmlEdit).m_AllQuestStrId.end() )
                {
                    CXmlEdit::MapFileTable::iterator  RoleIter = GetInst(CXmlEdit).m_AllQuestList.find( FileIter->second ) ;
                    if ( RoleIter != GetInst(CXmlEdit).m_AllQuestList.end() )
                    {
                        vector<tagRole*>::iterator iter;
                        iter = RoleIter->second.begin();
                        for ( ; iter != RoleIter->second.end() ; iter ++ )
                        {
                            if ( (*iter)->m_nID == CopyId )
                            {
                                role = (*iter);
                                break;
                            }
                        }
                    }
                }
                if( !role )
                    return ;

                sprintf_s( strVal , "quest/Quest%d.xml" , NId/1000 );
                CXmlEdit::MapFileTable::iterator itr = GetInst(CXmlEdit).m_AllQuestList.find( strVal ); 
                if ( itr != GetInst(CXmlEdit).m_AllQuestList.end() )
                {
                       tagRole * newRole = new tagRole( *role);
                       newRole->m_nID    = NId;
                       newRole->vcRole_Attribute->m_nID = NId;
                       newRole->vcRole_Step->m_nRoleID      = NId;
                       newRole->vcRole_Step->m_nID      = 1;
                       itr->second.push_back( newRole );
                       GetInst(CXmlEdit).m_AllQuestStrId[ NId ]  = strVal; 
                }
                else
                {
                     tagRole * newRole = new tagRole( *role);
                     newRole->m_nID    = NId;
                     newRole->vcRole_Attribute->m_nID = NId;
                     newRole->vcRole_Step->m_nRoleID      = NId;
                     newRole->vcRole_Step->m_nID      = 1;
                     GetInst(CXmlEdit).m_AllQuestList[ strVal ].push_back( newRole );
                     GetInst(CXmlEdit).m_AllQuestStrId[ NId]  = strVal;

                }
                RoleList.DeleteAllItems();
                ShowRoleList();
                
            }
        }
    }
    else if ( CopyId <= 0 )
    {
        int nRet=MessageBox("ճ��ǰ��Ҫ���ƾ��������!");
    }
    else
    {
        MessageBox("����ѡ�о�����Ҫճ���������ļ���!");
    }
}

void  CQuestEditorDlg::OnCreateTextRes()
{
     CCreateNewRes  res;
     if( res.DoModal() == IDOK )
     {
         HTREEITEM selectedItem = RoleList.GetSelectedItem();
         CString ItemText = RoleList.GetItemText(selectedItem);


         CString  filename = res.m_fileresname;
         if ( filename.GetLength() > 6 )
         {
             filename = "textres/" + filename;
             CXmlEdit::MapTextFileTable::iterator  MapIter = GetInst(CXmlEdit).m_AllQuestTextFile.find( filename);
             if ( MapIter != GetInst(CXmlEdit).m_AllQuestTextFile.end() )
             {
                 MessageBox("�Լ����ڴ��ļ������뻻һ��!");
                 return ;
             }
             map<CString,CString> mp;
             GetInst(CXmlEdit).m_AllQuestTextFile[filename] = mp ;
             RoleList.DeleteAllItems();
             ShowRoleList();
         }
         else
         {
             MessageBox("��������ļ�������������Ҫxml�ļ���׺��!");
         }
     }
}

void  CQuestEditorDlg::OnQuestQuery()
{
    CQueryFrame  query;
    if( query.DoModal() == IDOK )
    {
        long Sel = query.m_Sel;
        if ( Sel < 0 )
        {
            return ;
        }
        CString filename = query.m_questname;

        if ( filename.GetLength() > 0 )
        {
            m_VcCurrItem.clear();
            m_Findtext = filename;
            HTREEITEM hRoot = RoleList.GetRootItem();
            if ( RoleList.ItemHasChildren(hRoot) )
            {
                HTREEITEM  hNextItem;
                HTREEITEM  hChildItem = RoleList.GetChildItem( hRoot ) ;

                CString  str =  RoleList.GetItemText(hChildItem).GetBuffer();
                if ( strstr( str.GetBuffer(),filename.GetBuffer() ))
                {
                       RoleList.SelectItem(hChildItem);
                       RoleList.Expand(hChildItem,TVE_EXPAND);
                       m_VcCurrItem[hChildItem] = 1;// .push_back( hChildItem );
                       return ;
                }

                while( hChildItem != NULL )
                {
                    if ( RoleList.ItemHasChildren(hChildItem) )
                    {
                        HTREEITEM  hGrandChildItem = RoleList.GetChildItem( hChildItem ) ;
                        HTREEITEM  hGrandNextItem;

                        str =  RoleList.GetItemText(hGrandChildItem).GetBuffer();
                        if ( strstr( str.GetBuffer(),filename.GetBuffer() ))
                        {
                            RoleList.SelectItem(hGrandChildItem);
                            RoleList.Expand(hGrandChildItem,TVE_EXPAND);
                            m_VcCurrItem[hGrandChildItem] = 1;
                            return ;
                        }
                        while ( hGrandChildItem!= NULL )
                        {
                            hGrandNextItem = RoleList.GetNextItem( hGrandChildItem , TVGN_NEXT );

                            str =  RoleList.GetItemText(hGrandNextItem).GetBuffer();
                            if ( strstr( str.GetBuffer(),filename.GetBuffer() ))
                            {
                                RoleList.SelectItem(hGrandNextItem);
                                RoleList.Expand(hGrandNextItem,TVE_EXPAND);
                                m_VcCurrItem[hGrandNextItem] = 1;
                                return ;
                            }
                            hGrandChildItem = hGrandNextItem;               
                        }

                        hNextItem = RoleList.GetNextItem( hChildItem , TVGN_NEXT );
                        hChildItem = hNextItem;
                    }
                }
            }
        }
    }
}

void CQuestEditorDlg::GoOnFindTreeItem()
{
   if ( m_VcCurrItem.size() < 1 )
        return ;
   
   HTREEITEM hRoot = RoleList.GetRootItem();
   if ( RoleList.ItemHasChildren(hRoot) )
   {
       HTREEITEM  hNextItem;
       HTREEITEM  hChildItem = RoleList.GetChildItem( hRoot ) ;

       CString  str =  RoleList.GetItemText(hChildItem).GetBuffer();
       if ( m_VcCurrItem[hChildItem] < 1 && strstr( str.GetBuffer(),m_Findtext.GetBuffer() ))
       {
           RoleList.SelectItem(hChildItem);
           RoleList.Expand(hChildItem,TVE_EXPAND);
           m_VcCurrItem[hChildItem] = 1;
           return ;
       }

       while( hChildItem != NULL )
       {
           if ( RoleList.ItemHasChildren(hChildItem) )
           {
               HTREEITEM  hGrandChildItem = RoleList.GetChildItem( hChildItem ) ;
               HTREEITEM  hGrandNextItem;

               str =  RoleList.GetItemText(hGrandChildItem).GetBuffer();
               if ( m_VcCurrItem[hGrandChildItem] < 1 && strstr( str.GetBuffer(),m_Findtext.GetBuffer() ))
               {
                   RoleList.SelectItem(hGrandChildItem);
                   RoleList.Expand(hGrandChildItem,TVE_EXPAND);
                   m_VcCurrItem[hGrandChildItem] = 1;
                   //m_VcCurrItem.push_back( hGrandChildItem );
                   return ;
               }
               while ( hGrandChildItem!= NULL )
               {
                   hGrandNextItem = RoleList.GetNextItem( hGrandChildItem , TVGN_NEXT );

                   str =  RoleList.GetItemText(hGrandNextItem).GetBuffer();
                   if ( m_VcCurrItem[hGrandNextItem] < 1 &&  strstr( str.GetBuffer(),m_Findtext.GetBuffer() ))
                   {
                       RoleList.SelectItem(hGrandNextItem);
                       RoleList.Expand(hGrandNextItem,TVE_EXPAND);
                       m_VcCurrItem[hGrandNextItem] = 1;
                       //m_VcCurrItem.push_back( hGrandNextItem );
                       return ;
                   }
                   hGrandChildItem = hGrandNextItem;               
               }

               hNextItem = RoleList.GetNextItem( hChildItem , TVGN_NEXT );
               hChildItem = hNextItem;
           }
       }
   }
   m_VcCurrItem.clear();
   return ;
}

LRESULT  CQuestEditorDlg::OnHotKey(WPARAM wPararm,LPARAM lPararm)
{
    if ( wPararm == 1001 )
    {
          if ( m_Findtext.GetLength() < 1)
            return 0;
          
          if ( m_VcCurrItem.size() < 1 )
            return 0;
          
          GoOnFindTreeItem();
    }
    return 0;
}

void CQuestEditorDlg::OnOnlySaveQuest()
{
    CFileSave objSave;
    objSave.OnReWrite();
}

void CQuestEditorDlg::OnSaveAllRes()
{
    CFileSave objSave;
    objSave.OnSaveRes();
}

void CQuestEditorDlg::OnReLoad()
{
    RoleList.DeleteAllItems();
    ShowRoleList();
}