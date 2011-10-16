#include "Monster.h"
#include "DlgEdit.h"
#pragma once

// CDlgMonster �Ի���
class CDlgMonster : public CDlgEdit
{
	DECLARE_DYNAMIC(CDlgMonster)

public:
	CDlgMonster(CWnd* pParent = NULL);						// ��׼���캯��
	virtual ~CDlgMonster();
	void SetDlg(CRegion *pRegion, CMonster *pmonster, BOOL bALL = FALSE);		// ��ʾMONSTER������Ϣ
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// �Ի�������
	enum { IDD = IDD_MONSTER };

public:
	afx_msg void OnEnChangeMonsterNo();
	afx_msg void OnEnChangeMonsterCoorLeft();
	afx_msg void OnEnChangeMonsterCoorTop();
	afx_msg void OnEnChangeMonsterCoorRight();
	afx_msg void OnEnChangeMonsterCoorBottom();
	afx_msg void OnEnChangeMonsterDirection();
	afx_msg void OnEnChangeMonsterQuantity();
	afx_msg void OnEnChangeMonsterTimeBorn();
	afx_msg void OnEnChangeMonsterTimeStart();
	afx_msg void OnBnClickedCheckBorn();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CMonster		*m_pMonster;						// ��ǰ��ӦMonsterָ��
	CRegion			*m_pRegion;							// ��ǰ��ӦRegionָ��
	BOOL			m_bDlgEdit;
	BOOL			m_bError;
};
