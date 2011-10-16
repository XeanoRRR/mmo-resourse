#pragma once
#include "SignPos.h"
#include "DlgEdit.h"
// CDlgSignPos �Ի���

class CDlgSignPos : public  CDlgEdit
{
	DECLARE_DYNAMIC(CDlgSignPos)

public:
	CDlgSignPos(CWnd* pParent = NULL);   // ��׼���캯��
	void SetDlg(CRegion *pRegion, CSignPos *pSignPos, BOOL bALL = FALSE);
	virtual ~CDlgSignPos();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_SIGNPOS };

protected:
	CSignPos		*m_pSignPos;
	CRegion			*m_pRegion;							// ��ǰ��ӦRegionָ��
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeSignposNo();
	afx_msg void OnEnChangeSignposX();
	afx_msg void OnEnChangeSignposY();
};
