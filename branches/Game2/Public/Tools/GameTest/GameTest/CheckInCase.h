#pragma once
#include "afxwin.h"


// CCheckInCase �Ի���

class CCheckInCase : public CDialog
{
	DECLARE_DYNAMIC(CCheckInCase)

public:
	CCheckInCase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckInCase();

// �Ի�������
	enum { IDD = IDD_DIALOG_CheckInCase };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCasePath;
	afx_msg void OnBnClickedCheck();
	CEdit m_edCasePath;
	CString m_strClassName;
};
