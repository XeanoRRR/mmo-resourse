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

// RrMMOLauncherServerView.cpp : CRrMMOLauncherServerView ���ʵ��
//

#include "Precompiled.h"
#include "RrMMOLauncherServer.h"

#include "RrMMOLauncherServerDoc.h"
#include "RrMMOLauncherServerView.h"

// CRrMMOLauncherServerView

IMPLEMENT_DYNCREATE(CRrMMOLauncherServerView, CView)

BEGIN_MESSAGE_MAP(CRrMMOLauncherServerView, CView)
    // ��׼��ӡ����
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRrMMOLauncherServerView::OnFilePrintPreview)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CRrMMOLauncherServerView ����/����

CRrMMOLauncherServerView::CRrMMOLauncherServerView()
{
    // TODO: �ڴ˴���ӹ������

}

CRrMMOLauncherServerView::~CRrMMOLauncherServerView()
{
}

BOOL CRrMMOLauncherServerView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: �ڴ˴�ͨ���޸�
    //  CREATESTRUCT cs ���޸Ĵ��������ʽ

    return CView::PreCreateWindow(cs);
}

// CRrMMOLauncherServerView ����

VOID CRrMMOLauncherServerView::OnDraw(CDC* /*pDC*/)
{
    CRrMMOLauncherServerDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CRrMMOLauncherServerView ��ӡ


VOID CRrMMOLauncherServerView::OnFilePrintPreview()
{
    AFXPrintPreview(this);
}

BOOL CRrMMOLauncherServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Ĭ��׼��
    return DoPreparePrinting(pInfo);
}

VOID CRrMMOLauncherServerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

VOID CRrMMOLauncherServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӵ�ӡ����е��������
}

VOID CRrMMOLauncherServerView::OnRButtonUp(UINT nFlags, CPoint point)
{
    USE_PARAM(nFlags);
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

VOID CRrMMOLauncherServerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    USE_PARAM(pWnd);
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CRrMMOLauncherServerView ���

#ifdef _DEBUG
VOID CRrMMOLauncherServerView::AssertValid() CONST
{
    CView::AssertValid();
}

VOID CRrMMOLauncherServerView::Dump(CDumpContext& dc) CONST
{
    CView::Dump(dc);
}

CRrMMOLauncherServerDoc* CRrMMOLauncherServerView::GetDocument() CONST // �ǵ��԰汾��������
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRrMMOLauncherServerDoc)));
    return (CRrMMOLauncherServerDoc*)m_pDocument;
}
#endif //_DEBUG


// CRrMMOLauncherServerView ��Ϣ�������


VOID CRrMMOLauncherServerView::OnLButtonDown(UINT nFlags, CPoint point)
{
    USE_PARAM(nFlags, point);
    //this->MessageBox(_T("CRrMMOLauncherServerDoc::OnLButtonDown"));
}

VOID CRrMMOLauncherServerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    USE_PARAM(nFlags, point);
    this->MessageBox(_T("CRrMMOLauncherServerDoc::OnLButtonDblClk"));
}
