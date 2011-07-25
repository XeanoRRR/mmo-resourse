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

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "RrMMOLauncherServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static CHAR THIS_FILE[]=__FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
    ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
    ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
    ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    ON_COMMAND(ID_PROPERTIES1, OnProperties1)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
    ON_COMMAND(ID_PROPERTIES2, OnProperties2)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar ��Ϣ�������

VOID CPropertiesWnd::AdjustLayout()
{
    if (GetSafeHwnd() == NULL)
    {
        return;
    }

    CRect rectClient,rectCombo;
    GetClientRect(rectClient);

    m_wndObjectCombo.GetWindowRect(&rectCombo);

    INT cyCmb = rectCombo.Size().cy;
    INT cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

INT CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // �������:
    CONST DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
    {
        TRACE0("δ�ܴ���������� \n");
        return -1;      // δ�ܴ���
    }

    m_wndObjectCombo.AddString(_T("Ӧ�ó���"));
    m_wndObjectCombo.AddString(_T("���Դ���"));
    m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
    m_wndObjectCombo.SetCurSel(0);

    if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
    {
        TRACE0("δ�ܴ�����������\n");
        return -1;      // δ�ܴ���
    }

    InitPropList();

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
    m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ������*/);
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ����*/);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
}

VOID CPropertiesWnd::OnSize(UINT nType, INT cx, INT cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

VOID CPropertiesWnd::OnExpandAllProperties()
{
    m_wndPropList.ExpandAll();
}

VOID CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
    USE_PARAM(pCmdUI);
}

VOID CPropertiesWnd::OnSortProperties()
{
    m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

VOID CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

VOID CPropertiesWnd::OnProperties1()
{
    // TODO: �ڴ˴���������������
}

VOID CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
    // TODO: �ڴ˴����������� UI ����������
}

VOID CPropertiesWnd::OnProperties2()
{
    // TODO: �ڴ˴���������������
}

VOID CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
    // TODO: �ڴ˴����������� UI ����������
}

VOID CPropertiesWnd::InitPropList()
{
    CMFCPropertyGridProperty* pGroup1   = NULL;
    CMFCPropertyGridProperty* pGroup2   = NULL;
    CMFCPropertyGridProperty* pGroup3   = NULL;
    CMFCPropertyGridProperty* pGroup4   = NULL;
    CMFCPropertyGridProperty* pGroup41  = NULL;
    CMFCPropertyGridProperty* pGroup411 = NULL;
    CMFCPropertyGridProperty* pProp     = NULL;
    CMFCPropertyGridProperty* pSize     = NULL;
    _ENTER_FUN_C;

    SetPropListFont();

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea();
    m_wndPropList.SetVSDotNetLook();
    m_wndPropList.MarkModifiedProperties();

    pGroup1 = new CMFCPropertyGridProperty(_T("���"));

    pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��ά���"), (_variant_t) false, _T("ָ�����ڵ����岻ʹ�ô��壬���ҿؼ���ʹ����ά�߿�")));

    pProp = new CMFCPropertyGridProperty(_T("�߿�"), _T("Dialog Frame"), _T("����֮һ: ��(None)��ϸ(Thin)���ɵ�����С(Resizable)���Ի������(Dialog Frame)"));
    pProp->AddOption(_T("None"));
    pProp->AddOption(_T("Thin"));
    pProp->AddOption(_T("Resizable"));
    pProp->AddOption(_T("Dialog Frame"));
    pProp->AllowEdit(FALSE);

    pGroup1->AddSubItem(pProp);
    pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));

    m_wndPropList.AddProperty(pGroup1);

    pSize = new CMFCPropertyGridProperty(_T("���ڴ�С"), 0, TRUE);

    pProp = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t) 250l, _T("ָ�����ڵĸ߶�"));
    pProp->EnableSpinControl(TRUE, 50, 300);
    pSize->AddSubItem(pProp);

    pProp = new CMFCPropertyGridProperty( _T("���"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
    pProp->EnableSpinControl(TRUE, 50, 200);
    pSize->AddSubItem(pProp);

    m_wndPropList.AddProperty(pSize);

    pGroup2 = new CMFCPropertyGridProperty(_T("����"));

    LOGFONT lf;
    CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    font->GetLogFont(&lf);

    lstrcpy(lf.lfFaceName, _T("����, Arial"));

    pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ�����ڵ�Ĭ������")));
    pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ʹ��ϵͳ����"), (_variant_t) true, _T("ָ������ʹ�á�MS Shell Dlg������")));

    m_wndPropList.AddProperty(pGroup2);

    pGroup3 = new CMFCPropertyGridProperty(_T("����"));
    pProp = new CMFCPropertyGridProperty(_T("(����)"), _T("Ӧ�ó���"));
    pProp->Enable(FALSE);
    pGroup3->AddSubItem(pProp);

    CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), RGB(210, 192, 254), NULL, _T("ָ��Ĭ�ϵĴ�����ɫ"));
    pColorProp->EnableOtherButton(_T("����..."));
    pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
    pGroup3->AddSubItem(pColorProp);

    static TCHAR BASED_CODE szFilter[] = _T("ͼ���ļ�(*.ico)|*.ico|�����ļ�(*.*)|*.*||");
    pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("ͼ��"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("ָ������ͼ��")));

    pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("�ļ���"), _T("c:\\")));

    m_wndPropList.AddProperty(pGroup3);

    pGroup4 = new CMFCPropertyGridProperty(_T("��νṹ"));

    pGroup41 = new CMFCPropertyGridProperty(_T("��һ���Ӽ�"));
    pGroup4->AddSubItem(pGroup41);

    pGroup411 = new CMFCPropertyGridProperty(_T("�ڶ����Ӽ�"));
    pGroup41->AddSubItem(pGroup411);

    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 1"), (_variant_t) _T("ֵ 1"), _T("��Ϊ˵��")));
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 2"), (_variant_t) _T("ֵ 2"), _T("��Ϊ˵��")));
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 3"), (_variant_t) _T("ֵ 3"), _T("��Ϊ˵��")));

    pGroup4->Expand(FALSE);
    m_wndPropList.AddProperty(pGroup4);
    _DISPOSE_END_C
    {
        SAFE_DELETE(pGroup1);
        SAFE_DELETE(pGroup2);
        SAFE_DELETE(pGroup3);
        SAFE_DELETE(pGroup4);
        SAFE_DELETE(pGroup41);
        SAFE_DELETE(pGroup411);
        SAFE_DELETE(pProp);
        SAFE_DELETE(pSize);
    }_LEAVE_FUN;
}

VOID CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);
    m_wndPropList.SetFocus();
}

VOID CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetPropListFont();
}

VOID CPropertiesWnd::SetPropListFont()
{
    ::DeleteObject(m_fntPropList.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;

    m_fntPropList.CreateFontIndirect(&lf);

    m_wndPropList.SetFont(&m_fntPropList);
}
