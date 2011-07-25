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

#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList ����

class COutputList : public CListBox
{
// ����
public:
    COutputList();

// ʵ��
public:
    virtual ~COutputList();

protected:
    afx_msg VOID OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg VOID OnEditCopy();
    afx_msg VOID OnEditClear();
    afx_msg VOID OnViewOutput();

    DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// ����
public:
    COutputWnd();

// ����
protected:
    CFont m_Font;

    CMFCTabCtrl    m_wndTabs;

    COutputList m_wndOutputBuild;
    COutputList m_wndOutputDebug;
    COutputList m_wndOutputFind;

protected:
    VOID FillBuildWindow();
    VOID FillDebugWindow();
    VOID FillFindWindow();

    VOID AdjustHorzScroll(CListBox& wndListBox);

// ʵ��
public:
    virtual ~COutputWnd();

protected:
    afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg VOID OnSize(UINT nType, INT cx, INT cy);

    DECLARE_MESSAGE_MAP()
};

