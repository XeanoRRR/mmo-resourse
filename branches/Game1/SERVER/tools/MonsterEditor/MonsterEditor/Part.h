#pragma once


// CPart �Ի���

class CPart : public CDialog
{
	DECLARE_DYNAMIC(CPart)

public:
	//���öԻ����м�¼��
	void SetData(vector<tagMonster>*vecMonster,int n,int m);
	CPart(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPart();

// �Ի�������
	enum { IDD = IDD_PART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int nfirstA;
	int nfirstB;
	int nallrecords;
	afx_msg void OnPart();
	int nallskillrecords;
};
