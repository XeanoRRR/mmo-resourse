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

// RrMMOSimpleClientView.cpp : CRrMMOSimpleClientView ���ʵ��
//

#include "Precompiled.h"
#include "RrMMOSimpleClient.h"

#include "RrMMOSimpleClientDoc.h"
#include "RrMMOSimpleClientView.h"

// CRrMMOSimpleClientView

IMPLEMENT_DYNCREATE(CRrMMOSimpleClientView, CView)

BEGIN_MESSAGE_MAP(CRrMMOSimpleClientView, CView)
    // ��׼��ӡ����
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRrMMOSimpleClientView::OnFilePrintPreview)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CRrMMOSimpleClientView ����/����

CRrMMOSimpleClientView::CRrMMOSimpleClientView()
{
    // TODO: �ڴ˴���ӹ������

}

CRrMMOSimpleClientView::~CRrMMOSimpleClientView()
{
}

BOOL CRrMMOSimpleClientView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: �ڴ˴�ͨ���޸�
    //  CREATESTRUCT cs ���޸Ĵ��������ʽ

    return CView::PreCreateWindow(cs);
}

// CRrMMOSimpleClientView ����

void CRrMMOSimpleClientView::OnDraw(CDC* /*pDC*/)
{
    CRrMMOSimpleClientDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CRrMMOSimpleClientView ��ӡ


void CRrMMOSimpleClientView::OnFilePrintPreview()
{
    AFXPrintPreview(this);
}

BOOL CRrMMOSimpleClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Ĭ��׼��
    return DoPreparePrinting(pInfo);
}

void CRrMMOSimpleClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CRrMMOSimpleClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӵ�ӡ����е��������
}

void CRrMMOSimpleClientView::OnRButtonUp(UINT nFlags, CPoint point)
{
    USE_PARAM(nFlags);
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CRrMMOSimpleClientView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    USE_PARAM(pWnd);
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CRrMMOSimpleClientView ���

#ifdef _DEBUG
void CRrMMOSimpleClientView::AssertValid() CONST
{
    CView::AssertValid();
}

void CRrMMOSimpleClientView::Dump(CDumpContext& dc) CONST
{
    CView::Dump(dc);
}

CRrMMOSimpleClientDoc* CRrMMOSimpleClientView::GetDocument() CONST // �ǵ��԰汾��������
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRrMMOSimpleClientDoc)));
    return (CRrMMOSimpleClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CRrMMOSimpleClientView ��Ϣ�������


void CRrMMOSimpleClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
    USE_PARAM(nFlags, point);
    this->MessageBox(_T("CRrMMOSimpleClientDoc::OnLButtonDown"));
}

void CRrMMOSimpleClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    USE_PARAM(nFlags, point);
    this->MessageBox(_T("CRrMMOSimpleClientDoc::OnLButtonDblClk"));
}
