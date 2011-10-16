/*
=========================================================================
��ע���ù�����ȡ��������ȡ����2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�ACAnswerDlg.h
*  ���ܣ���������ԶԻ������
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

class CACAnswer : public CDialog
{
	DECLARE_DYNAMIC(CACAnswer)

public:
	CACAnswer(CWnd* pParent = NULL);
	virtual ~CACAnswer();

	enum { IDD = IDD_PROPPAGE_AC_ANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	// ����ID
	long m_lRegionID;
	long m_lPosX;
	long m_lPosY;
	long m_lStartCdKey;
	long m_lEndCdKey;
	long m_lSendMsgTimer;
	long m_lSeverSub;
	// �Ƿ񼤻����
	BOOL m_bIsCanAnswer;
	// �Ƿ񼤻ȫ����
	BOOL m_bIsCanSafeTest;
	// �Ƿ���������Ϣ
	BOOL m_bIsCopyMsg;
	// �Ƿ�����ϢSend�Ǹ���
	BOOL m_bIsCopySnedMsg;
	// �Ƿ�������
	BOOL m_bIsExitClient;
	// �Ƿ�ʢ��ʽ��½
	BOOL m_bIsCanPtLogin;	
	afx_msg void OnBnClickedButtonAccStart();
	afx_msg void OnBnClickedAccCheckAnswer();
	afx_msg void OnBnClickedAccCheckSafe();
	afx_msg void OnBnClickedAccCheckCopymsg();
	afx_msg void OnBnClickedAccCheckCopysend();
	afx_msg void OnBnClickedAccCheckExitclient();
	afx_msg void OnBnClickedCheckAcaPtLogin();
};
