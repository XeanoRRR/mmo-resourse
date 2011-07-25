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

class CPropertiesToolBar : public CMFCToolBar
{
public:
    virtual VOID OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() CONST { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// ����
public:
    CPropertiesWnd();

    VOID AdjustLayout();

// ����
public:
    VOID SetVSDotNetLook(BOOL bSet)
    {
        m_wndPropList.SetVSDotNetLook(bSet);
        m_wndPropList.SetGroupNameFullWidth(bSet);
    }

protected:
    CFont m_fntPropList;
    CComboBox m_wndObjectCombo;
    CPropertiesToolBar m_wndToolBar;
    CMFCPropertyGridCtrl m_wndPropList;

// ʵ��
public:
    virtual ~CPropertiesWnd();

protected:
    afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg VOID OnSize(UINT nType, INT cx, INT cy);
    afx_msg VOID OnExpandAllProperties();
    afx_msg VOID OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
    afx_msg VOID OnSortProperties();
    afx_msg VOID OnUpdateSortProperties(CCmdUI* pCmdUI);
    afx_msg VOID OnProperties1();
    afx_msg VOID OnUpdateProperties1(CCmdUI* pCmdUI);
    afx_msg VOID OnProperties2();
    afx_msg VOID OnUpdateProperties2(CCmdUI* pCmdUI);
    afx_msg VOID OnSetFocus(CWnd* pOldWnd);
    afx_msg VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    DECLARE_MESSAGE_MAP()

    VOID InitPropList();
    VOID SetPropListFont();
};

