// MonsterEditorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMonsterEditorDlg �Ի���
class CMonsterEditorDlg : public CDialog
{
// ����
public:
	void ReadFile();//��ȡ�ļ�
	void MonList();//��ʾ��listcontrol
	void MonTree();//��ʾ��Tree
	void ListSetData(vector<tagMonster>::iterator iter,int i);//����listcontrol��ֵ
	void LogList(vector<tagMonster>::iterator iter,int nflag);//������־�ļ�
	void SaveFile(const char *filename,const char *filenameskill);//�����ļ�
	void RecoverFile(string strfilename,string strfilenameskill);//�ָ��ļ�
	CMonsterEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MONSTEREDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HACCEL m_hAccelTable;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_MonTree;
	CListCtrl m_MonList;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNew();
	afx_msg void OnBnClickedButton1();
	
	CListBox m_LogListBox;
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDelete();//����ʽ�˵���ɾ����¼
	afx_msg void OnSaveToFile();//�˵��������ļ�
	afx_msg void OnRecover();//�˵����ָ��ļ�
	afx_msg void OnPartFile();

	afx_msg void OnMergerFile();
	afx_msg void OnClose();


};
