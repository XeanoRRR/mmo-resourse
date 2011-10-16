#pragma once
#include "Resource.h"

// CAffirmDialog �Ի���

class CAffirmDialog : public CDialog
{
	DECLARE_DYNCREATE(CAffirmDialog)

public:
	CAffirmDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAffirmDialog();
// ��д
	//HRESULT OnButtonOK(IHTMLElement *pElement);
	//HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG_AFFIRM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	BOOL m_bAlwaysCheck;
	int  m_iResult;
//	afx_msg void OnBnClickedOk();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedNo();
};
