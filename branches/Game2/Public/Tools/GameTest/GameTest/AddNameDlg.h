/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�AddNameDlg.h
*  ���ܣ�ΪTestScript�����case�����ṩ��������Ի���
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
#include "afxwin.h"

class CAddNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddNameDlg)

public:
	CAddNameDlg(CWnd* pParent = NULL); 
	virtual ~CAddNameDlg();

	enum { IDD = IDD_DIALOG_AddName };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_ctlAddTitle;
	CEdit m_ctlAddName;

	CString m_strAddName;
public:
	afx_msg void OnBnClickedOk();
	CString GetAddName()	{ return m_strAddName;}
};
