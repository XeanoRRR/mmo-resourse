#pragma once


// CCaseNameDlg �Ի���

class CCaseNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CCaseNameDlg)

public:
	CCaseNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaseNameDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AddCaseName };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCaseName;
	CString m_strTitle;
	CString m_Name;
};
