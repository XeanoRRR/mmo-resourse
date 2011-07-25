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

class CFileViewToolBar : public CMFCToolBar
{
    virtual VOID OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() CONST { return FALSE; }
};

class CFileView : public CDockablePane
{
// ����
public:
    CFileView();

    VOID AdjustLayout();
    VOID OnChangeVisualStyle();

// ����
protected:

    CViewTree m_wndFileView;
    CImageList m_FileViewImages;
    CFileViewToolBar m_wndToolBar;

protected:
    VOID FillFileView();

// ʵ��
public:
    virtual ~CFileView();

protected:
    afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg VOID OnSize(UINT nType, INT cx, INT cy);
    afx_msg VOID OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg VOID OnProperties();
    afx_msg VOID OnFileOpen();
    afx_msg VOID OnFileOpenWith();
    afx_msg VOID OnDummyCompile();
    afx_msg VOID OnEditCut();
    afx_msg VOID OnEditCopy();
    afx_msg VOID OnEditClear();
    afx_msg VOID OnPaint();
    afx_msg VOID OnSetFocus(CWnd* pOldWnd);

    DECLARE_MESSAGE_MAP()
};

