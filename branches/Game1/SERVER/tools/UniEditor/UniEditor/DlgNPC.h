#include "NPC.h"
#include "DlgEdit.h"

#pragma once


// CDlgNPC �Ի���

class CDlgNPC : public CDlgEdit
{
	DECLARE_DYNAMIC(CDlgNPC)

public:
	CDlgNPC(CWnd* pParent = NULL);						// ��׼���캯��
	virtual ~CDlgNPC();
	void SetDlg(CRegion *pRegion, CNPC *pnpc, BOOL bALL = FALSE);			// ��ʾNPC������Ϣ
	void Save(CNPC *pnpc);								// ����MONSTER������Ϣ

// �Ի�������
	enum { IDD = IDD_COLLECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CNPC			*m_pNPC;
	BOOL			m_bDlgEdit;
	BOOL			m_bError;
	Cell			*m_pCell;
	MAPTREE			m_mapTree;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangeNpcCoorLeft();
	afx_msg void OnEnChangeNpcCoorTop();
	afx_msg void OnEnChangeNpcCoorRight();
	afx_msg void OnEnChangeNpcCoorBottom();
	afx_msg void OnEnChangeNpcDirection();
	afx_msg void OnEnChangeNpcName();
	afx_msg void OnEnChangeNpcPicid();
	afx_msg void OnEnChangeNpcQuantity();
	afx_msg void OnEnChangeNpcScript();
	afx_msg void OnBnClickedCheckBorn();
	afx_msg void OnBnClickedCheckShow();
};
