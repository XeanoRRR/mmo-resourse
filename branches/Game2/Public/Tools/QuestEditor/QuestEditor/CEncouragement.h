#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// Encouragement �Ի���
class CEncouragement : public CDialog
{
	DECLARE_DYNAMIC(CEncouragement)

public:
	CEncouragement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEncouragement();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	  //���ػس�������Ӧ����
	  afx_msg void OnOK();
	  //��ȡ�����������������Ϣ
	  afx_msg void ReadRoleEncouragement(CString *pItemText);
	  //��ȡ���º����������Ϣ
	  afx_msg bool GetUpdatedEn(vector<tagRoleEncouragement*>& vcUpdatedeEn,int& Cell);
	  //����ؼ�����Ӧ
	  afx_msg void OnBnClickedButton3();
	  afx_msg void OnBnClickedButton1();
	  afx_msg void OnBnClickedButton2();
	  //��ʾ������,List�ؼ�����Ӧ
	  afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	  //���浱ǰ��ѡ��������������
	  vector<tagRoleEncouragement*>m_pSelectedRoleEn;
	  //tagRoleEncouragement���͵ĵ�����
	  vector<tagRoleEncouragement*>::iterator iter;
	  //������º��˵�����Ľ���
	  vector<tagRoleEncouragement*>vcUpdatedeEn;
	  //����ID
	  int RoleId;
	  //�����ļ�����
	  string strFileName;
	  //��������Ʒ����Ҫ�İ���������
	  int m_EncouragementCell;
	  // //������Ʒ������
	  CString m_EncouragementName;
	  // ������Ʒ������
	  int m_EncouragementNum;
	  //��ʾ������������
	  int m_EncouragementTypeIndex;
	  //����һ��CComBox�ؼ�
	  CComboBox m_EncouragementType;
	  // //��ʾ���������б�
	  CListCtrl m_EncouragementList;
      CString m_textDes;

      afx_msg void OnEnChangeEdit3();
      afx_msg void OnEnChangeEdit1();


private:
    bool  isLoading ;
public:
    CString  m_Textfile;
    CEdit m_TextIndex;
    CEdit m_TextContext;

public:
    CString  filename;
};
