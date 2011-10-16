#pragma once
#include "DlgEdit.h"

// CDlgRoadsInfo �Ի���

class CDlgRoadsInfo : public CDlgEdit
{
	DECLARE_DYNAMIC(CDlgRoadsInfo)

public:
	CDlgRoadsInfo(CWnd* pParent = NULL);   // ��׼���캯��
	void SetDlg(CRegion *pRegion, int n,CRoads::tagRoadsPoint *roadspoint=NULL, BOOL bALL = FALSE);		// ��ʾ·����
	void SetDlgNo(MAPROADS& mapRoads,int n);		// ��ʾ·����
	virtual BOOL OnInitDialog();
	virtual ~CDlgRoadsInfo();

// �Ի�������
	enum { IDD = IDD_ROADPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CRoads::tagRoadsPoint *m_roadspoint;
	BOOL	m_bDlgEdit;
	BOOL m_Auto;
	int  m_RoadsID;
	MAPROADS m_MapRoads;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeRoadspointx();
	afx_msg void OnEnChangeRoadspointy();
	afx_msg void OnEnChangeRoadsno();
};
