// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "PackageTool.h"
#include "PackageToolDoc.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPENFOLDER, OnFileOpenfolder)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OPERATOR,OnUpdateStatuOperator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILENAME,OnUpdateStatuFileName)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOTALNUM,OnUpdateStatuTotaNumb)
	ON_COMMAND(ID_FILE_OPENCLIENTINFO, OnFileOpenclientinfo)
	ON_COMMAND(ID_FILE_PATCH_INFO, OnFileOpenpatchinfo)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_strLastOpenFolder = "F:\\saga3d2\\RUN\\UpdateSys\\Client-Resouce";
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}


	static UINT indicatorss[]=
	{
		ID_INDICATOR_OPERATOR,
		ID_INDICATOR_FILENAME,
		//ID_INDICATOR_TOTALNUM,
	};

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicatorss,
		  sizeof(indicatorss)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_OPERATOR);
	m_wndStatusBar.SetPaneInfo(index,ID_INDICATOR_OPERATOR,SBPS_NORMAL,150);

	index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_FILENAME);
	//m_wndStatusBar.SetPaneInfo(index,ID_INDICATOR_FILENAME,SBPS_NORMAL,60);
	m_wndStatusBar.SetPaneStyle(index,SBPS_STRETCH);

	//index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_TOTALNUM);
	//m_wndStatusBar.SetPaneInfo(index,ID_INDICATOR_TOTALNUM,SBPS_NORMAL,100);

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CMenu* pMenu = GetMenu();
	if(pMenu)
	{
		pMenu->CheckMenuItem(ID_SETUPSERVERMODE, MF_UNCHECKED);
		pMenu->CheckMenuItem(ID_SETUPCLIENTMODE, MF_CHECKED);
	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������


void CMainFrame::OnFileOpenfolder()
{
	// TODO: �ڴ���������������
	//char strPath[MAX_PATH] = "F:\saga3d2\RUN\UpdateSys\Client-Resouce";
	CString strFolder;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner=GetSafeHwnd();
	//bi.pidlRoot =SHSimpleIDListFromPath( (LPCWSTR)strPath );
	bi.lpszTitle="ѡ��Ҫ��������Դ�ļ���:";
	LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	if(idl==NULL)
		return;
	SHGetPathFromIDList(idl,strFolder.GetBuffer(MAX_PATH));
	//m_strLastOpenFolder = strFolder;

	CPackageToolDoc* pDoc = static_cast<CPackageToolDoc*>(GetActiveDocument());
	if(pDoc)	pDoc->OnOpenfolder(strFolder);
}


void CMainFrame::OnUpdateStatuOperator(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CMainFrame::OnUpdateStatuFileName(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
void CMainFrame::OnUpdateStatuTotaNumb(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void	CMainFrame::OnDisplayStatus(tagStaDisplay* pStaDisplay)
{
	if(pStaDisplay==NULL)	return;

	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_OPERATOR);
	m_wndStatusBar.SetPaneText(index,pStaDisplay->strOp.c_str());

	index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_FILENAME);
	m_wndStatusBar.SetPaneText(index,pStaDisplay->strFileName.c_str());

	//CString str;
	//str.Format("%d",pStaDisplay->nNumber);

	//index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_TOTALNUM);
	//m_wndStatusBar.SetPaneText(index,(LPCTSTR)str);
}
BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	switch(message)
	{
	case STATUS_UPDATE:
		{
			tagStaDisplay* pSta = (tagStaDisplay*)wParam;
			OnDisplayStatus(pSta);
		}
		break;
	}

	return CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CMainFrame::OnFileOpenclientinfo()
{
	// TODO: �ڴ���������������
	CString strFolder;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner=GetSafeHwnd();
	//bi.pidlRoot =SHSimpleIDListFromPath( (LPCWSTR)strPath );
	bi.lpszTitle="ѡ����Ϸ��Դ�ļ���:";
	LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	if(idl==NULL)
		return;
	SHGetPathFromIDList(idl,strFolder.GetBuffer(MAX_PATH));
	//m_strLastOpenFolder = strFolder;

	CPackageToolDoc* pDoc = static_cast<CPackageToolDoc*>(GetActiveDocument());
	if(pDoc)	pDoc->OnOpenGameResouce(strFolder);
}

void CMainFrame::OnFileOpenpatchinfo()
{
	// TODO: �ڴ���������������
	CString strFolder;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner=GetSafeHwnd();
	//bi.pidlRoot =SHSimpleIDListFromPath( (LPCWSTR)strPath );
	bi.lpszTitle="ѡ��patch��Դ�ļ���:";
	LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	if(idl==NULL)
		return;
	SHGetPathFromIDList(idl,strFolder.GetBuffer(MAX_PATH));
	//m_strLastOpenFolder = strFolder;

	CPackageToolDoc* pDoc = static_cast<CPackageToolDoc*>(GetActiveDocument());
	if(pDoc)	pDoc->OnOpenPatchResouce(strFolder);
}