#pragma once


// CCreateNewRes �Ի���

class CCreateNewRes : public CDialog
{
	DECLARE_DYNAMIC(CCreateNewRes)

public:
	CCreateNewRes(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateNewRes();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_fileresname;
};
