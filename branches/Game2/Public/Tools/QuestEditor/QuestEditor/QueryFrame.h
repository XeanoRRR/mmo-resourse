#pragma once
#include "afxwin.h"


// CQueryFrame �Ի���

class CQueryFrame : public CDialog
{
	DECLARE_DYNAMIC(CQueryFrame)

public:
	CQueryFrame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryFrame();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    long    m_Sel;
    CString m_questname;
    afx_msg void OnBnClickedButton1();
    CComboBox m_ComBox;
};
