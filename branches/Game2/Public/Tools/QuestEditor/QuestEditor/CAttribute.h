#pragma once
#include "afxwin.h"
#include <string.h>
#include "resource.h"
#include "struct.h"
#include "afxcmn.h"

// Attribute �Ի���

class CAttribute : public CDialog
{
	DECLARE_DYNAMIC(CAttribute)

public:
	// ����ID
	int m_RoleID;
	//�ļ�����
	string	strFileName;
	// ������������
	int m_RoleType;
	// ����������������
	int m_RoleEmbracer;
	//
	int m_ShowAble;
	//
	int m_BeRecord;
	//
	int m_Repeated;
	//
	int m_Deadlost;
	//
	int	m_LostRepeated;
	// �����Ѷ�
	int m_HardFactor;
	// ɾ������ű�
	CString m_DeleteScript;
	// ��������
	CString m_RoleName;
	//
	int m_EmbracerEffrct;
	// �������ʱ����Ч
	int m_FinishEmbracerEffect;
	// ��������ʱ�ĵ�ͼ��Ч
	int m_EmbracerMapEffect;
	// �����ϡ�ж�
	int m_Odds;
	// �����
	int m_MassPoint;
	// ���������ı���ID
	CString m_DiscribTextID;
	// ��С�ȼ�����
	int m_MiniGrade;
	// ���ȼ�����
	int m_MaxGrade;
	// Ӷ����������
	int m_EngageArmsRepute;
	// ������ʾ����
	int m_NPCShowType;
	// NPCԭʼ��
	CString m_NPCID;
	// ���������Ч
	int m_FinishEffect;
	// ������������
	int m_NPCClewType;
	// ��ѯ������������
	CString m_BeSearchDesc;
	// ��ѯ�����������
	CString m_BeSearchSpDesc;
	// ����________��������
	int m_ChapterNum;
	// ͼƬ________������������
	int m_picture;
	// ͼ��_______������������
	int m_Icon;
	// �������������ı�
	CString m_StoryDesc;
	// //ǰ����������
	CString m_RoleIndex;
	// �Ƿ񱻼�¼
	CComboBox m_BoxBeRecord;
	// �Ƿ���ʾ
	CComboBox m_BoxShowAble;
	// �Ƿ��ظ�
	CComboBox m_BoxRepeated;
	// �����Ƿ�ʧ��
	CComboBox m_BoxDeadlost;
	// �������Ƿ�����ظ�
	CComboBox m_BoxLostRepeated;
	CAttribute(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAttribute();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
		//���浱ǰѡ�е��������������
		tagRoleAttribute*           m_pSelectedRoleAttribute;
        tagRole         *           m_pRole;

	    //���ػس�������Ӧ����
	    afx_msg void OnOK();
		//���ļ��ж�ȡ�ļ���Ϣ�ĺ���
		afx_msg void ReadRoleAttribute(CString *pItemText);
		//�ӽ����ȡ�����е�����������Ϣ
		afx_msg bool GetUpdatedAttribute(vector<tagRoleAttribute*>& vcUpdatedeAttribute);
		//ȷ���ؼ�����Ӧ����
		afx_msg void OnBnClickedButton1();

        //
        afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnCbnSelchangeCombo1();

        CString m_Sex;
        long m_ReqSkill;
        CString m_DDoc;
        CString m_Occupation;
        CListCtrl m_EncouragementList;
        CString m_QuestType;
        CString m_QuestShare;
        CString m_DelScript;
        long m_TotePet;
//         long m_EncouragementCell;
//         CString m_AwardName;
//         long m_AwardNum;
//         double m_AwardOdds;


private:
       bool   isLoading ;
public:
    CString m_AwardTypeDes;
    //CString m_AwardType;
    afx_msg void OnEnChangeEdit37();
    afx_msg void OnEnChangeEdit3();
    afx_msg void OnEnChangeEdit8();
    afx_msg void OnEnChangeEdit36();
    CEdit m_EncouragementCell;
    CComboBox m_AwardType;
    CEdit m_AwardName;
    CEdit m_AwardOdds;

    long  m_QuestId;
    CEdit m_AwardNum;
    afx_msg void OnBnClickedButton7();
    afx_msg void OnBnClickedButton8();
};
