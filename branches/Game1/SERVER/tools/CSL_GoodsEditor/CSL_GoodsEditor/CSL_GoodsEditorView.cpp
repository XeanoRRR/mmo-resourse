// CSL_GoodsEditorView.cpp : CCSL_GoodsEditorView ���ʵ��
//

#include "stdafx.h"
#include "CSL_GoodsEditor.h"

#include "CSL_GoodsEditorDoc.h"
#include "CSL_GoodsEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCSL_GoodsEditorView

IMPLEMENT_DYNCREATE(CCSL_GoodsEditorView, CView)

BEGIN_MESSAGE_MAP(CCSL_GoodsEditorView, CView)
END_MESSAGE_MAP()

// CCSL_GoodsEditorView ����/����

CCSL_GoodsEditorView::CCSL_GoodsEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CCSL_GoodsEditorView::~CCSL_GoodsEditorView()
{
}

BOOL CCSL_GoodsEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CCSL_GoodsEditorView ����

void CCSL_GoodsEditorView::OnDraw(CDC* /*pDC*/)
{
	CCSL_GoodsEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CCSL_GoodsEditorView ���

#ifdef _DEBUG
void CCSL_GoodsEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CCSL_GoodsEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCSL_GoodsEditorDoc* CCSL_GoodsEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCSL_GoodsEditorDoc)));
	return (CCSL_GoodsEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CCSL_GoodsEditorView ��Ϣ�������
