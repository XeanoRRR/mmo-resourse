/**
=========================================================================
��ע����δʹ�ã�����ȡ����2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�MailDlg.h
*  ���ܣ��ʼ����ԶԻ���
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

class CMailDlg : public CDialog
{

	DECLARE_DYNAMIC(CMailDlg)

public:

	CMailDlg(CWnd* pParent = NULL);
	virtual ~CMailDlg();
	
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_PROPPAGE_MAIL };
	CButton	m_CheckMailSend;		
	CButton m_CheckMailRepratName;
	CButton m_CheckMailRec;
	CButton	m_CheckMailReject;
	CButton m_CheckMailDelete;
	CButton m_CheckMailGetDoods;
	CButton m_CheckMailAgreePay;
	CButton m_CheckMailGetMoney;
	CButton m_CheckMailExitClient;
	CButton m_CheckMailSendMsgRepeat;
	
	//��½��ʽ�ؼ�
	CButton m_CheckMailPtLogin;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:

	// ��ʼ����Id
	long m_lStartRegionID;
	// ����X
	long m_lPosX;
	// ����Y
	long m_lPosY;
	// ��ʼ�ʺ�
	long m_lStartCdKey;
	// �����ʺ�
	long m_lEndCdKey;
	// ��Ϣ���ͼ��
	long m_lMsgSendTimer;	
	// �����������б�ؼ�
	CComboBox m_comboxServer;
	// ��Ʒ��
	CString m_cstrGoodsName;
	// ��Ǯ
	long m_lSendMoney;
	// �ʼ����ͣ� -1Ϊ�������������
	long m_lMailType;
	// ��Ʒ����
	long m_lGoodsNum;
	// ���͵Ľ���������
	CString m_cstrSendName;
	
	// �ظ����ʹ���
	long m_lSendRepeatNum;	
	// ���ż���
	long m_lRejectMailProb;
	// �����߼���
	long m_lExitClientPro;	
	// ɾ�ż���
	long m_lDeleteMailProb;
	// �������汾
	long m_lSeverSub;	
	// �ظ�����һ����Ϣ
	long m_lSendMsgRepeatNum;
	
	//button for Lua val;
	bool m_isCanSendMail;			//�Ƿ��ܷ���
	bool m_isCanRepeatSendName;		//�Ƿ��ظ����͸�һ�����
	bool m_isCanRecMail;			//�Ƿ�������
	bool m_isCanRejectMail;			//�Ƿ�����
	bool m_isCanDeleteMail;			//�Ƿ�ɾ��
	bool m_isCanGetGoods;			//�Ƿ�ȡ��Ʒ
	bool m_isCanAgreePay;			//�Ƿ�ͬ�⸶��
	bool m_isCanGetMoney;			//�Ƿ�ȡ��Ǯ
	bool m_isCanExitClient;			//�Ƿ�������
	bool m_isCanSendMsgRepeat;		//�Ƿ��ظ�����һ����Ϣ
	bool m_isCanPtLogin;			//�Ƿ�ʢ��ĵ�½��ʽ

	afx_msg void OnBnClickedCheckMailSend();
	afx_msg void OnBnClickedCheckMailRepeatsendname();
	afx_msg void OnBnClickedCheckMailrec();
	afx_msg void OnBnClickedCheckMailReject();
	afx_msg void OnBnClickedCheckMailDelete();
	afx_msg void OnBnClickedCheckMailGetgoods();
	afx_msg void OnBnClickedCheckMailAgreedpay();
	afx_msg void OnBnClickedCheckMailGetmoney();
	afx_msg void OnBnClickedCheckMailExitclient();
	afx_msg void OnBnClickedButtonMailstart();
	afx_msg void OnBnClickedCheckMailRepeatsendmsg();

	//ʢ���½��ʽ
	afx_msg void OnBnClickedCheckMailLogin();

};
