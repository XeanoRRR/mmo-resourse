#pragma once


// CMerger �Ի���

class CMerger : public CDialog
{
	DECLARE_DYNAMIC(CMerger)

public:
	bool MergerFile(CString strfilename);//�ϲ������ļ�
	bool MergerFileSkill(CString strfilename);//�ϲ������ļ�
	bool IsSame(CString strfilename,tagMonster *monster);//�ж��Ƿ����ظ������Լ�¼
	bool IsSameSkill(CString strfilename,tagMonster *monster);
	CMerger(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMerger();

// �Ի�������
	enum { IDD = IDD_MERGER };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//���ļ��ؼ�
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnMerger();
};
