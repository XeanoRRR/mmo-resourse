// CSL_GoodsEditorView.h : CCSL_GoodsEditorView ��Ľӿ�
//


#pragma once


class CCSL_GoodsEditorView : public CView
{
protected: // �������л�����
	CCSL_GoodsEditorView();
	DECLARE_DYNCREATE(CCSL_GoodsEditorView)

// ����
public:
	CCSL_GoodsEditorDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CCSL_GoodsEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CSL_GoodsEditorView.cpp �ĵ��԰汾
inline CCSL_GoodsEditorDoc* CCSL_GoodsEditorView::GetDocument() const
   { return reinterpret_cast<CCSL_GoodsEditorDoc*>(m_pDocument); }
#endif

