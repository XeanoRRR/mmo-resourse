// YaYaDiskDlg.h : ͷ�ļ�
//

#pragma once


// CYaYaDiskDlg �Ի���
class CYaYaDiskDlg : public CDialog
{
// ����
public:
	CYaYaDiskDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_YAYADISK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

    CFile m_File;

	// ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    //afx_msg void OnBnClickedOk();
    afx_msg void OnStnClickedQq();
    afx_msg void OnStnClickedBeach();
    afx_msg void OnStnClickedWind();
    afx_msg void OnStnClickedQqgame();
    afx_msg void OnStnClickedQqDance();
    afx_msg void OnStnClickedExit();
    afx_msg void OnStnClickedSmall();
};
