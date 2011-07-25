
// MsScriptEditorView.cpp : CMsScriptEditorView ���ʵ��
//

#include "stdafx.h"
#include "MsScriptEditor.h"

#include "MsScriptEditorDoc.h"
#include "MsScriptEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMsScriptEditorView

IMPLEMENT_DYNCREATE(CMsScriptEditorView, CView)

BEGIN_MESSAGE_MAP(CMsScriptEditorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMsScriptEditorView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMsScriptEditorView ����/����

CMsScriptEditorView::CMsScriptEditorView()
{
	EnableActiveAccessibility();
	// TODO: �ڴ˴���ӹ������

}

CMsScriptEditorView::~CMsScriptEditorView()
{
}

BOOL CMsScriptEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMsScriptEditorView ����

void CMsScriptEditorView::OnDraw(CDC* /*pDC*/)
{
	CMsScriptEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMsScriptEditorView ��ӡ


void CMsScriptEditorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMsScriptEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMsScriptEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMsScriptEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMsScriptEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMsScriptEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMsScriptEditorView ���

#ifdef _DEBUG
void CMsScriptEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMsScriptEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMsScriptEditorDoc* CMsScriptEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMsScriptEditorDoc)));
	return (CMsScriptEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMsScriptEditorView ��Ϣ�������
