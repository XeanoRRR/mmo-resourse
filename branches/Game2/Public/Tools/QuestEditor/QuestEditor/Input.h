#pragma once
// CInput �Ի���
class CInput : public CDialog
{
	DECLARE_DYNAMIC(CInput)
public:
	CInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInput();
// �Ի�������
	enum { IDD = IDD_INPUT };
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	//ȷ���ؼ�����Ӧ
	afx_msg void OnBnClickedButton1();
	//ȡ���ռ����Ӧ
	afx_msg void OnBnClickedButton4();
	//�Ի�����������½�������ID
	int m_nInPutRoleID;
    afx_msg void OnBnClickedOk();
};
