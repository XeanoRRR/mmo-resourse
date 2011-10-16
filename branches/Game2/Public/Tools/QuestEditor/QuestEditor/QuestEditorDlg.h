// QuestEditorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "CAttribute.h"
#include "CStep.h"
#include "CEncouragement.h"
#include "afxwin.h"

#include <map>


// CQuestEditorDlg �Ի���
class CQuestEditorDlg : public CDialog
{
// ����
public:
	CQuestEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CQuestEditorDlg();

// �Ի�������
	enum { IDD = IDD_QUESTEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HACCEL Menu;;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnOK();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//Tab�ؼ�����
	CTabCtrl Tab;		
	//3��ҳ��ĵı���
	CAttribute Attribute;		
	CStep Step;		
	CEncouragement Encouragement;	
   // ����һ�����ͽṹ�б�ͼ���б�
	CTreeCtrl RoleList;
	//
	CImageList imagelist;
	//��ǩ����
	int m_CurSelTab;
	//
	CListBox m_Loglist;
	
	//��ʼ��TAB����ĺ���
	afx_msg void ShowTab();
	//��ʾ�ļ��б���
	afx_msg void ShowRoleList();
	//�л���ǩ�ĺ���
	afx_msg void OnNMClickTab(int TabNum);
	//�����Ҽ���Ӧ����
	afx_msg void OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//�����б�ѡ�еĽڵ㷢���仯ʱ����Ӧ
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
    //Tab�ؼ��¼��ı�
    afx_msg void OnTvnSelchangedTab(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void ShowLog(vector<string>strlog);
    afx_msg void OnAddNewQuest();
	afx_msg void On32792();
	afx_msg void OnExit();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnDelQuest();
    afx_msg void OnCopyQuest();
    afx_msg void OnStickQuest();
    afx_msg void OnCreateTextRes();
    afx_msg void OnQuestQuery();

    afx_msg void OnOnlySaveQuest();
    afx_msg void OnSaveAllRes();
    afx_msg void OnReLoad();

    LRESULT OnHotKey(WPARAM wPararm,LPARAM lPararm);
    
    void    GoOnFindTreeItem();

private:
    long        CopyId;
    CString     m_Findtext;
    std::map<HTREEITEM,long>   m_VcCurrItem;

};
