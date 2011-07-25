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

// RrMMOLauncherServerView.h : CRrMMOLauncherServerView ��Ľӿ�
//


#pragma once


class CRrMMOLauncherServerView : public CView
{
protected: // �������л�����
    CRrMMOLauncherServerView();
    DECLARE_DYNCREATE(CRrMMOLauncherServerView)

// ����
public:
    CRrMMOLauncherServerDoc* GetDocument() CONST;

// ����
public:

// ��д
public:
    virtual VOID OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg VOID OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg VOID OnLButtonDblClk(UINT nFlags, CPoint point);

protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual VOID OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual VOID OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
    virtual ~CRrMMOLauncherServerView();
#ifdef _DEBUG
    virtual VOID AssertValid() CONST;
    virtual VOID Dump(CDumpContext& dc) CONST;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
    afx_msg VOID OnFilePrintPreview();
    afx_msg VOID OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg VOID OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // RrMMOLauncherServerView.cpp �еĵ��԰汾
inline CRrMMOLauncherServerDoc* CRrMMOLauncherServerView::GetDocument() CONST
   { return reinterpret_cast<CRrMMOLauncherServerDoc*>(m_pDocument); }
#endif

