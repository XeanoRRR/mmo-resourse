#pragma once
#include "Collection.h"
#include "DlgEdit.h"
// CDlgCollection �Ի���

class CDlgCollection : public  CDlgEdit
{
	DECLARE_DYNAMIC(CDlgCollection)

public:
	CDlgCollection(CWnd* pParent = NULL);   // ��׼���캯��
	void SetDlg(CRegion *pRegion, CCollection *pCollection, BOOL bALL = FALSE);
	virtual ~CDlgCollection();
	virtual BOOL OnInitDialog();
	
// �Ի�������
	enum { IDD = IDD_COLLECTION };

protected:
	CCollection		*m_pCollection;						// ��ǰ��ӦCollectionָ��
	CRegion			*m_pRegion;							// ��ǰ��ӦRegionָ��
	BOOL			m_bDlgEdit;
	BOOL			m_bError;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	int m_ncollleft;
	afx_msg void OnEnChangeCollNo();
	afx_msg void OnEnChangeCollCoorLeft();
	afx_msg void OnEnChangeCollCoorTop();
	afx_msg void OnEnChangeCollCoorRight();
	afx_msg void OnEnChangeCollCoorBottom();
	afx_msg void OnEnChangeCollQuantity();
	afx_msg void OnEnChangeCollTimeBorn();
	afx_msg void OnEnChangeCollTimeStart();
	afx_msg void OnEnChangeCollDirection();
	afx_msg void OnBnClickedCheckBorn();
	afx_msg void OnBnClickedCheckCollBorn();
};
