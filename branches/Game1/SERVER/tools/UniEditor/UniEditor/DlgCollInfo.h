#pragma once
#include "DlgEdit.h"
#include "Collection.h"
// CDlgCollInfo �Ի���
const CString strFileCollection = _T("data\\CollectionList.xml");	// �����б��ļ�
class CDlgCollInfo : public CDlgEdit
{
	DECLARE_DYNAMIC(CDlgCollInfo)

public:
	CDlgCollInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCollInfo();
	void SetDlg(CRegion *pRegion, CCollectionInfo *pCollectionInfo, BOOL bALL = FALSE);		// ��ʾ�ɼ�����ϸ��Ϣ
// �Ի�������
	enum { IDD = IDD_COLLECTIONINFO };
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL PreTranslateMessage(MSG* pMsg);
	CCollectionInfo		*m_pCollInfo;					// ��ǰ��ӦCollectionָ��
	CRegion			*m_pRegion;							// ��ǰ��ӦRegionָ��
	BOOL			m_bDlgEdit;
	BOOL			m_bError;
	CComboBox		*m_pCollectionList;
	BOOL m_Auto;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCollList();
	afx_msg void OnEnChangeCollProbability();
	afx_msg void OnCbnEditchangeCollList();
};
