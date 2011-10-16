/*
=========================================================================
��ע����δʹ�ã�����ȡ����2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�LinkManDlg.h
*  ���ܣ����Ѳ��ԶԻ���
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�MartySa
*  ������ڣ� 2009.4.24
*
============================================================
*/

#pragma once
#include "afxwin.h"

class CLinkManDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinkManDlg)

public:
	CLinkManDlg(CWnd* pParent = NULL); 
	virtual ~CLinkManDlg();
	
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_PROPPAGE_LINKMAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	//��ʼ������ID
	long m_lRegionID;
	long m_lPosX;
	long m_lPosY;
	long m_lStartCdKey;
	long m_lEndCdKey;
	long m_lSendMsgTimer;
	long m_lSeverSub;	
	long m_lMoveRrob;
	long m_lDelProb;
	long m_lExitProb;
	
	CComboBox m_comboServerIP;
	
	bool m_isCanSendAdd;
	bool m_isCanResponseAdd;
	bool m_isCanMoveLink;
	bool m_isCanDelLink;
	bool m_isCanExit;
	
	//�Ƿ�ʢ���½��ʽ
	bool m_isCanPtLogin;		

	afx_msg void OnBnClickedCheckLinkmanSendadd();
	afx_msg void OnBnClickedCheckLinkmanResponseadd();
	afx_msg void OnBnClickedCheckLinkmanSendmove();
	afx_msg void OnBnClickedCheckLinkmanDel();
	afx_msg void OnBnClickedCheckLinkmanExitclient();
	afx_msg void OnBnClickedButtonLinkmanStart();
	//
	afx_msg void OnBnClickedCheckLinkmanPtLogin();

};
