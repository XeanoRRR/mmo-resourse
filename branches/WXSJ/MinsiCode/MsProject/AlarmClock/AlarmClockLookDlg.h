#pragma once


// AlarmClockLookDlg �Ի���

class AlarmClockLookDlg : public CDialog
{
	DECLARE_DYNAMIC(AlarmClockLookDlg)

public:
	AlarmClockLookDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AlarmClockLookDlg();

// �Ի�������
	enum { IDD = IDD_ALARMCLOCK_DIALOG_LOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
