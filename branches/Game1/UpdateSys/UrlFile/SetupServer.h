#pragma once
#include "afxwin.h"


// SetupServer �Ի���

class SetupServer : public CDialog
{
	DECLARE_DYNAMIC(SetupServer)

public:
	SetupServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetupServer();
	afx_msg void OnBnClickedOk();
	void SetPage();
	virtual BOOL OnInitDialog();
	int GetCurSelect();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETUPSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_CCombo;
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCancel();
};
