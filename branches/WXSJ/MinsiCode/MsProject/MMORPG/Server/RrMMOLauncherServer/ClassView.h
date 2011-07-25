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

#include "ViewTree.h"

class CClassToolBar : public CMFCToolBar
{
    virtual VOID OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() CONST { return FALSE; }
};

class CClassView : public CDockablePane
{
public:
    CClassView();
    virtual ~CClassView();

    VOID AdjustLayout();
    VOID OnChangeVisualStyle();

protected:
    CClassToolBar m_wndToolBar;
    CViewTree m_wndClassView;
    CImageList m_ClassViewImages;
    UINT m_nCurrSort;

    VOID FillClassView();

// ��д
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg VOID OnSize(UINT nType, INT cx, INT cy);
    afx_msg VOID OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg VOID OnClassAddMemberFunction();
    afx_msg VOID OnClassAddMemberVariable();
    afx_msg VOID OnClassDefinition();
    afx_msg VOID OnClassProperties();
    afx_msg VOID OnNewFolder();
    afx_msg VOID OnPaint();
    afx_msg VOID OnSetFocus(CWnd* pOldWnd);
    afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
    afx_msg VOID OnSort(UINT id);
    afx_msg VOID OnUpdateSort(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()
};

