/// ����������Ʒ
/// Add by Ф�� 2010.5.21
#pragma once
#include "afxwin.h"


// Search �Ի���

class Search : public CDialog
{
	DECLARE_DYNAMIC(Search)

public:
	Search(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Search();

// �Ի�������
	enum { IDD = IDD_SEARCH };

// ������ʽ
	//enum{
	//	ST_ID=0,			// ����Ʒ�ɣ�ֵ��������
	//	ST_ORINAME=1,		// ����Ʒԭʼ����������
	//	ST_SHOWNAME=2,��	// ����Ʒ��ʾ���ƽ�������
	//};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeSearchoption();
	afx_msg void OnEnKillfocusSearchcontent();
	afx_msg void OnBnClickedCancel();
	// ������ʽѡ������
	CComboBox m_choseType;
	afx_msg void OnBnClickedNext();
	// Ҫ����������
	CEdit m_SerContent;

	// ������һ������������Ʒ����ֵ
	int m_preSerResult;

	// ����ַ����Ƿ�������
	bool CheckStrIsNum(const char* str,DWORD len);
};
