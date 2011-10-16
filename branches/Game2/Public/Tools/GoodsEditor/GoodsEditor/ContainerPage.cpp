// ContainerPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GoodsEditor.h"
#include "ContainerPage.h"
#include ".\containerpage.h"


// CContainerPage

IMPLEMENT_DYNCREATE(CContainerPage, CFormView)

CContainerPage::CContainerPage()
	: CFormView(CContainerPage::IDD)
{
	
}

CContainerPage::~CContainerPage()
{
}

void CContainerPage::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CContainerPage, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CContainerPage ���

#ifdef _DEBUG
void CContainerPage::AssertValid() const
{
	CFormView::AssertValid();
}

void CContainerPage::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CContainerPage ��Ϣ�������

int CContainerPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  �ڴ������ר�õĴ�������
	//CRect rect;
	//GetWindowRect(&rect);
    m_wndAllGoodsPage.Create(NULL,"AllGoodsPage",WS_CHILD|WS_VISIBLE,CRect(0,0,100,100),this,1,NULL);
	m_wndGoodAttrPage.Create(NULL,"GoodAttrPage",WS_CHILD|WS_VISIBLE,CRect(0,0,1000,1000),this,2,NULL);
	//m_wndAllGoodsPage.ShowWindow(SW_HIDE);
	m_wndGoodAttrPage.ShowWindow(SW_HIDE);
	return 0;
}

void CContainerPage::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_wndAllGoodsPage.DestroyWindow();
}

void CContainerPage::OnSize(UINT nType, int cx, int cy)
{
	if(cx<200) cx=200;
	if(cy<200) cy=200;
	m_wndAllGoodsPage.MoveWindow(0,0,cx,cy);
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

//	m_wndAllGoodsPage.OnSize(nType, cx, cy);
//	if(m_wndAllGoodsPage.GetInitFlag())	
//		m_wndAllGoodsPage.m_cAllGoodsList.MoveWindow(0,0,cx,cy);
}
