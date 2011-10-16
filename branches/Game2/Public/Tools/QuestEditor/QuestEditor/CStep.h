#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Step �Ի���

class CStep : public CDialog
{
	DECLARE_DYNAMIC(CStep)

public:
	CStep(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStep();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnOK();
		afx_msg void ReadRoleStep(CString *pItemText);
		afx_msg void Showdetail(int StepID);
		afx_msg bool GetUpdatedStep(tagRoleStep* pUpdatedeStep);
		// �������б�
		CListCtrl m_StepList;
		// �����ı��б�
		CListCtrl m_StepTextList;
		// �Ͳ���ȡ����Ʒ�б�
		CListCtrl m_StepEncourageList;
		// ����Ŀ���б�
		CListCtrl m_StepAimList;
		// ������ID
		int m_StepId;
		// �͚h��NPCԭʼ��
		CString m_StepNpcId;
		// //.............
		int m_StepEffect_1;
		// .......................
		int m_StepEffect_2;
		// ��ʾģʽ
		int m_ShowNum;
		// ׷����Ʒ������
		int m_Sum;
		// ׷����ƷID
		CString m_TraceID;
		// ȷ������������
		int m_Param;
		// ��ʾ�ı����͵�ID
		int m_TextID;
		// �����ı�������
		int m_TextName;
		// �͚h��������Ʒ����
		CString m_StepEncouragementName;
		// �͚h������Ʒ������
		int m_StepEncouragementNum;
		// �͚h������Ʒ������
		int m_StepEncouragementType;
		//����ID�ı�Ǳ���,��¼����ID
		int m_nRoleID;
		//�������ڵ��ļ���
		string	strFileName;
		// ����׷���б�
		CListCtrl m_StepTraceList;
		//������º���������ָ��
		tagRoleStep* pUpdatedStep;
		//����ѡ�е������������
		tagRoleStep*  m_pSlectRoleStep;
		//tagRoleStep���͵ĵ�����
		vector<tagRoleStep*>::iterator StepIter;
		//��������ؼ�����Ӧ����
		afx_msg void OnBnClickedButton8();
		afx_msg void OnBnClickedButton14();
		afx_msg void OnBnClickedButton10();
		afx_msg void OnBnClickedButton11();
		afx_msg void OnBnClickedButton5();
		afx_msg void OnBnClickedButton1();
		afx_msg void OnBnClickedButton9();
		afx_msg void OnBnClickedButton4();
		afx_msg void OnBnClickedButton6();
		afx_msg void OnBnClickedButton12();
		afx_msg void OnBnClickedButton13();
		afx_msg void OnBnClickedButton2();
		afx_msg void OnBnClickedButton3();
		afx_msg void OnBnClickedButton7();
		//�����и���List�ؼ����������Ӧ
		afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnNMClickList5(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnNMClickList6(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult);

        afx_msg void OnEnChangeEdit3();
        afx_msg void OnEnChangeEdit1();

        long m_QuestCell;
        CEdit m_TextIndex;
        CEdit m_TextContext;
        CString m_textDes;


private:
    bool  isLoading;
public:
    long m_QuestId;
    CEdit m_TraceDes;
    CEdit m_TraceShowNum;
    CEdit m_TraceSum;
    CEdit m_TraceParam;
    CEdit m_TraceId;
    CEdit m_AimId;
    CEdit m_AimModel;
    CEdit m_AimType;
    CEdit m_AimName;
    CEdit m_AimNum;
    CEdit m_DropFrom;
    CEdit m_DropOdds;
    CEdit m_AimParam;
    afx_msg void OnEnChangeEdit7();
    afx_msg void OnEnChangeEdit8();
    afx_msg void OnEnChangeEdit9();
    afx_msg void OnEnChangeEdit10();
    afx_msg void OnEnChangeEdit11();
    afx_msg void OnEnChangeEdit31();
    afx_msg void OnEnChangeEdit27();
    afx_msg void OnEnChangeEdit23();
    afx_msg void OnEnChangeEdit22();
    afx_msg void OnEnChangeEdit21();
    afx_msg void OnEnChangeEdit19();
    afx_msg void OnEnChangeEdit18();
    afx_msg void OnEnChangeEdit20();
};
