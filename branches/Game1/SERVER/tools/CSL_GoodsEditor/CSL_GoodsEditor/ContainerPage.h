#pragma once
#include "AllGoodsPage.h"
#include "GoodAttrPage.h"

// CContainerPage ������ͼ

class CContainerPage : public CFormView
{
	DECLARE_DYNCREATE(CContainerPage)

protected:
	CContainerPage();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CContainerPage();

public:
	CAllGoodsPage m_wndAllGoodsPage;
	CGoodAttrPage m_wndGoodAttrPage;
	enum { IDD = IDD_CONTAINER };
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


