#pragma once
#include "../../PUBLIC/FilesInfo.h"

// CFolderSet �Ի���

class CFolderSet : public CDialog
{
	DECLARE_DYNAMIC(CFolderSet)

public:
	CFolderSet(CWnd* pParent = NULL);   // ��׼���캯��
	CFolderSet(string& strTitle,PackTypeInfos& PackInfo,CompTypeInfos& CompInfo,
						int nPackType,int nCompressType,CWnd* pParent = NULL);
	virtual ~CFolderSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_FILESETS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	int		m_nLastPackType;		//�ݴ����
	int		m_nLastCompressType;

public:
	string	m_strTitle;

	int		m_nPackType;			//�����
	int		m_nCompressType;		//ѹ������

	bool	m_bForceModifyPackType;
	bool	m_bForceModifyCompressType;

	PackTypeInfos	m_PackageSets;
	CompTypeInfos	m_CompresSets;

public:
	void	SetParam(string& strTitle,PackTypeInfos& PackInfo,CompTypeInfos& CompInfo,
						int nPackType,int nCompressType);

	bool	Initial();
	void	UpdatePackSet();
	void	UpdateCompressSet();
	int		GetComcoPackPosByID(int nPackID);
	int		GetPackIDByPos(int nPos);
	int		GetPackIDByName(string strName);

	int		GetComcoCompPosByID(int nCompID);
	int		GetCompIDByPos(int nPos);
	int		GetCompIDByName(string strName);


	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedRadioPack();
	afx_msg void OnBnClickedRadioUnpack();
	afx_msg void OnBnClickedRadioCompress();
	afx_msg void OnBnClickedRadioUncompress();
	afx_msg void OnCbnSelchangeComboPackageType();
	afx_msg void OnCbnSelchangeComboCompressType();
	afx_msg void OnBnClickedCheckForceModifyPacktype();
	afx_msg void OnBnClickedCheckForceModifyCompresstype();
};
