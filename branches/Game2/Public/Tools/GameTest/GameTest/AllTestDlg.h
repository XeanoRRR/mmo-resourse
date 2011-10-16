/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�AllTestDlg.h
*  ���ܣ��ڳ������Ի������ṩCTab�ؼ��ĶԻ���
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

#include "GameTest.h"
#include "SkillTest.h"
#include "MailDlg.h"
#include "LinkManDlg.h"
#include "GoodsTestDlg.h"
#include "ACAnswerDlg.h"

class CAllTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CAllTestDlg)

public:
	CAllTestDlg(CWnd* pParent = NULL);
	virtual ~CAllTestDlg();

	enum { IDD = IDD_DIALOG_AllTest };

	struct tagPageUIInfo
	{
		char szText[64];
		CDialog* pDlg;
		long lFlag;
		tagPageUIInfo(void)
		{
			pDlg = NULL;
			lFlag = -1;
			ZeroMemory(szText,64);
		}
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_tabTypePage;
	list<tagPageUIInfo> m_listPageUIInfo;

	//���ܲ��ԶԻ���
	CSkillTest m_dlgSkill;

	//�ʼ�
	CMailDlg m_MailDlg;

	//����
	CLinkManDlg m_LinkManDlg; 

	CGoodsTestDlg m_dlgGoodsTest;

	//�����
	CACAnswer m_ACADlg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabTypepage(NMHDR *pNMHDR, LRESULT *pResult);
};
