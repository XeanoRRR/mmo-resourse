#pragma once
#include "../../PUBLIC/FilesInfo.h"

// CVersionDialog �Ի���

class CVersionDialog : public CDialog
{
	DECLARE_DYNAMIC(CVersionDialog)

public:
	CVersionDialog(CWnd* pParent = NULL);   // ��׼���캯��
	CVersionDialog(PackTypeInfos& PackInfo,bool bSelPack,CWnd* pParent = NULL);   // ���캯��
	virtual ~CVersionDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldVersion;
	CString m_strNewVersion;

	int		m_nPackType;					//��������

private:
	PackTypeInfos	m_PackageSets;
	bool			m_bSelPack;
	int		GetPackIDByPos(int nPos);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboPatchPack();
};
