#pragma once
#include "afxwin.h"


// CDlgFileList �Ի���

class CDlgFileList : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileList)

public:
	CDlgFileList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFileList();

// �Ի�������
	enum { IDD = IDD_FILELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	// δ�����ļ����б�
	CListBox	m_listFile;
	int			m_iCurSel;

public:
	afx_msg void OnLbnSetfocusFilelist();
	afx_msg void OnLbnKillfocusFilelist();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSaveAll();
	afx_msg void OnLbnSelcancelFilelist();
	afx_msg void OnLbnSelchangeFilelist();
};
