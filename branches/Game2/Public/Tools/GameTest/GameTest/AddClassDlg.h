#pragma once


// CAddClassDlg �Ի���

class CAddClassDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddClassDlg)

public:
	CAddClassDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddClassDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AddClass };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strClassName;
	CString m_strOneCaseName;
};
