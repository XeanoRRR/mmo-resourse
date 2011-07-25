// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

#include "Precompiled.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

INT COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_Font.CreateStockObject(DEFAULT_GUI_FONT);

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // ����ѡ�����:
    if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
    {
        TRACE0("δ�ܴ������ѡ�����\n");
        return -1;      // δ�ܴ���
    }

    // �����������:
    CONST DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

    if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
        !m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
        !m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
    {
        TRACE0("δ�ܴ����������\n");
        return -1;      // δ�ܴ���
    }

    m_wndOutputBuild.SetFont(&m_Font);
    m_wndOutputDebug.SetFont(&m_Font);
    m_wndOutputFind.SetFont(&m_Font);

    CString strTabName;
    BOOL bNameValid;

    // ���б��ڸ��ӵ�ѡ�:
    bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
    ASSERT(bNameValid);
    m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
    bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
    ASSERT(bNameValid);
    m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
    bNameValid = strTabName.LoadString(IDS_FIND_TAB);
    ASSERT(bNameValid);
    m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

    // ʹ��һЩ�����ı���д���ѡ�(���踴������)
    FillBuildWindow();
    FillDebugWindow();
    FillFindWindow();

    return 0;
}

VOID COutputWnd::OnSize(UINT nType, INT cx, INT cy)
{
    CDockablePane::OnSize(nType, cx, cy);

    // ѡ��ؼ�Ӧ��������������:
    m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

VOID COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
    CClientDC dc(this);
    CFont* pOldFont = dc.SelectObject(&m_Font);

    INT cxExtentMax = 0;

    for (INT i = 0; i < wndListBox.GetCount(); i ++)
    {
        CString strItem;
        wndListBox.GetText(i, strItem);

        cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
    }

    wndListBox.SetHorizontalExtent(cxExtentMax);
    dc.SelectObject(pOldFont);
}

VOID COutputWnd::FillBuildWindow()
{
    m_wndOutputBuild.AddString(_T("�����������ʾ�ڴ˴���"));
    m_wndOutputBuild.AddString(_T("�������ʾ���б���ͼ������"));
    m_wndOutputBuild.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

VOID COutputWnd::FillDebugWindow()
{
    m_wndOutputDebug.AddString(_T("�����������ʾ�ڴ˴���"));
    m_wndOutputDebug.AddString(_T("�������ʾ���б���ͼ������"));
    m_wndOutputDebug.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

VOID COutputWnd::FillFindWindow()
{
    m_wndOutputFind.AddString(_T("�����������ʾ�ڴ˴���"));
    m_wndOutputFind.AddString(_T("�������ʾ���б���ͼ������"));
    m_wndOutputFind.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
    ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ��Ϣ�������

VOID COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
    CMenu menu;
    menu.LoadMenu(IDR_OUTPUT_POPUP);

    CMenu* pSumMenu = menu.GetSubMenu(0);

    if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
    {
        CMFCPopupMenu* pPopupMenu = NEW CMFCPopupMenu;

        if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
            return;

        ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
        UpdateDialogControls(this, FALSE);
    }

    SetFocus();
}

VOID COutputList::OnEditCopy()
{
    MessageBox(_T("�������"));
}

VOID COutputList::OnEditClear()
{
    MessageBox(_T("������"));
}

VOID COutputList::OnViewOutput()
{
    CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
    CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

    if (pMainFrame != NULL && pParentBar != NULL)
    {
        pMainFrame->SetFocus();
        pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
        pMainFrame->RecalcLayout();

    }
}


