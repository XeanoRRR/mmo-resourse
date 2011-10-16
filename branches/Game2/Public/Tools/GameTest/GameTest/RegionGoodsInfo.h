/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CRegionGoodsInfo.h
*  ���ܣ�����������ڳ�����Ʒ��ʾ�б�
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ� 
*
============================================================
*/

#pragma once
#include "afxcmn.h"
#include "..\\App\\Region.h"

class CRegionGoodsInfo : public CDialog
{
	DECLARE_DYNAMIC(CRegionGoodsInfo)

public:

	CRegionGoodsInfo(CWnd* pParent = NULL); 
	virtual ~CRegionGoodsInfo();

	enum { IDD = IDD_PROPPAGE_Region_Goods_Info };

protected:

	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()

public:

	struct staRgnGoods{
		CGUID guid;
		long lType;
	};

private:
	list<CRegionGoodsInfo::staRgnGoods*>	m_pRegionGoods;
	
public:
	list<CRegionGoodsInfo::staRgnGoods*>	GetRegionGoods()	{ return m_pRegionGoods;}
private:
	CListCtrl m_ctlRegionGoodsList;
public:
	virtual BOOL OnInitDialog();
	void SetDisplayGoods(CRegion* pRegion);
};
