/*
=========================================================================
��ע����δʹ�á�2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CGoodsTestDlg.h
*  ���ܣ� ��Ʒ���
*  ��ǰ�汾��1.2.0
*  ���ߣ� LGR
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�2009
*
============================================================
*/

#pragma once

class CGoodsTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CGoodsTestDlg)

public:
	CGoodsTestDlg(CWnd* pParent = NULL);
	virtual ~CGoodsTestDlg();

	enum { IDD = IDD_DIALOG_GOODS_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
