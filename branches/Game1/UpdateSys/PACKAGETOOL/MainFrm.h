// MainFrm.h : CMainFrame ��Ľӿ�
//

struct tagStaDisplay;

#pragma once
class CMainFrame : public CFrameWnd
{
private:
	CString		m_strLastOpenFolder;					//�����ϴδ򿪵��ļ���
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

	void	OnDisplayStatus(tagStaDisplay* pStaDisplay);
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpenfolder();
	afx_msg void OnUpdateStatuOperator(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatuFileName(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatuTotaNumb(CCmdUI* pCmdUI);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnFileOpenclientinfo();
	afx_msg void OnFileOpenpatchinfo();
};


