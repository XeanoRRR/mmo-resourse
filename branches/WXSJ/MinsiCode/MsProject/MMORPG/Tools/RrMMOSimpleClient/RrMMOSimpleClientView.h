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

// RrMMOSimpleClientView.h : CRrMMOSimpleClientView ��Ľӿ�
//


#pragma once


class CRrMMOSimpleClientView : public CView
{
protected: // �������л�����
    CRrMMOSimpleClientView();
    DECLARE_DYNCREATE(CRrMMOSimpleClientView)

// ����
public:
    CRrMMOSimpleClientDoc* GetDocument() CONST;

// ����
public:

// ��д
public:
    virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
    virtual ~CRrMMOSimpleClientView();
#ifdef _DEBUG
    virtual void AssertValid() CONST;
    virtual void Dump(CDumpContext& dc) CONST;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // RrMMOSimpleClientView.cpp �еĵ��԰汾
inline CRrMMOSimpleClientDoc* CRrMMOSimpleClientView::GetDocument() CONST
   { return reinterpret_cast<CRrMMOSimpleClientDoc*>(m_pDocument); }
#endif

