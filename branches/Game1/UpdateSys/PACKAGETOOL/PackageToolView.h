// PackageToolView.h : CPackageToolView ��Ľӿ�
//


#pragma once

#include "TreeListCtrl.h"
#include "PackageToolDoc.h"
#include "TreeToolTipCtrl.h"
#include "../../PUBLIC/Package.h"
#include "../../PUBLIC/FilesInfo.h"

typedef enum eDoItemType
{
	DO_PacWithCom,
	DO_Pac,
	DO_UnPac,
}DoItemType;


class CPackageToolView : public CView
{
protected: // �������л�����
	CPackageToolView();
	DECLARE_DYNCREATE(CPackageToolView)

// ����
public:
	CPackageToolDoc* GetDocument() const;

	CImageList m_cImageList;
	CImageList m_StatusImageList;

	//attributes
public:
	BOOL m_RTL;
	CFont m_headerFont;
	CTreeListCtrl m_tree;
	CScrollBar m_horScrollBar;
private:	
	void DisplayToolTip(CPoint point,TVHITTESTINFO hInfo, CString colText);		
	CTreeToolTipCtrl m_tooltip;	
	BOOL m_TLInitialized;	

	// Operations
public:
	static LONG FAR PASCAL DummyWndProc(HWND, UINT, WPARAM, LPARAM);
	void Initialize();

public:
	//static void RegisterClass();	
	void SortTree(int nCol, BOOL bAscending, HTREEITEM hParent);	
	void ResetScrollBar();

	BOOL VerticalScrollVisible();
	BOOL HorizontalScrollVisible();

	const string& GetPackFormatSring(DWORD dwPackID);
	const string& GetCompFormatSring(DWORD dwCompID);

	int StretchWidth(int m_nWidth, int m_nMeasure);

	void DoWithItem(HTREEITEM Item,DoItemType Type,
					DWORD dwPackType,DWORD dwCompressType,CPackageToolDoc* pDoc);

	void InsertAllFile(CFilesInfo* pFilesInfo,tagFileInfo* ptagFileInfo);
	void OnInsertItem(tagFileIndex* FileIndex);

	void GetItemImage(tagFileInfo* ptagFileInfo,int& nImage,int& nSelectImage);
	void OnAddFileInfo(CFilesInfo* pFileInfo,tagFileInfo* ptagFileInfo);
	void OnUpdateFileInfo(CFilesInfo* pFileInfo,tagFileInfo* ptagFileInfo);
	void OnDeleteFileInfo(CFilesInfo* pFileInfo,tagFileInfo* ptagFileInfo);
	void OnDeleteAllFileInfo();
	void OnInitialFileInfo();

	void OnOperatonFile(char* strFileName);

	void SetItemInfo(HTREEITEM hItem,tagFileInfo* ptagFileInfo);

	void UpdateItemWithChild(CFilesInfo* pFileInfo,tagFileInfo* ptagFileInfo);
	void InsertItemWithChild(HTREEITEM hItem,CFilesInfo* pFileInfo,tagFileInfo* ptagFileInfo);

	void OnLoadResourceStart(CClientResource* pClientReousrce);							//װ����Դ��ʼ
	void OnLoadResourceEnd(CClientResource* pClientReousrce);							//װ����Դ����
	void OnReLoadResourceStart(CClientResource* pClientReousrce);						
	void OnReLoadResourceEnd(CClientResource* pClientReousrce);							

	void OnMakePackageStart(CClientResource* pClientReousrce);							//�����ʼ
	void OnMakePackageing(CFilesInfo* pFileInfo,tagFileInfo* ptagFileInfo);				//�����
	void OnMakePackageEnd(CClientResource* pClientReousrce);							//�������

	void OnReMakePackageStart(CClientResource* pClientReousrce);						//�������ʼ
	void OnReMakePackageEnd(CClientResource* pClientReousrce);							//���������

// ����
public:

// ��д
	public:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CPackageToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	//{{AFX_MSG(CNewTreeListCtrlView)
	afx_msg void OnSize(UINT nType, int cx, int cy);		
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);	    
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopmenuPackagewithcompress();
	afx_msg void OnPopmenuPackage();
	afx_msg void OnPopmenuUnpackage();
	afx_msg void OnResoucePackage();
	afx_msg void OnPopmenuAddfolder();
	afx_msg void OnPopmenuAddfile();
	afx_msg void OnPopmenuDelete();
	afx_msg void OnPopmenuExtract();
	afx_msg void OnResouceRemakepackage();
	afx_msg void OnResouceReloadresource();
	afx_msg void OnPopmenuFilesSet();
	afx_msg void OnPopmenuReloadresource();
	afx_msg void OnPopmenuReloadresourceex();
};

#ifndef _DEBUG  // PackageToolView.cpp �ĵ��԰汾
inline CPackageToolDoc* CPackageToolView::GetDocument() const
   { return reinterpret_cast<CPackageToolDoc*>(m_pDocument); }
#endif

