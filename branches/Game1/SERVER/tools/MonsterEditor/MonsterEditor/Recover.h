#pragma once


// CRecover �Ի���

class CRecover : public CDialog
{
	DECLARE_DYNAMIC(CRecover)

public:
	CRecover(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecover();

// �Ի�������
	enum { IDD = IDD_RECOVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenMonsterFile();//�������ļ�
	afx_msg void OnOpenMonsterSkillFile();//�򿪼����ļ�
	afx_msg void OnRecover();//�ָ���ť������CMonsterEditorDlg��RecoverFile����
};
