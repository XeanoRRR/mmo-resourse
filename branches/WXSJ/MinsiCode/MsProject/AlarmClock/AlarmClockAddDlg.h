#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

// AlarmClockAddDlg �Ի���

class AlarmClockAddDlg : public CDialog
{
	DECLARE_DYNAMIC(AlarmClockAddDlg)

public:
	AlarmClockAddDlg(S_CLOCK_REGISTER* pClockReg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AlarmClockAddDlg();

// �Ի�������
	enum { IDD = IDD_ALARMCLOCK_DIALOG_ADD };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    CButton m_ButtonOnce;
    CButton m_ButtonEveryDay;
    CButton m_ButtonWeek0;
    CButton m_ButtonWeek1;
    CButton m_ButtonWeek2;
    CButton m_ButtonWeek3;
    CButton m_ButtonWeek4;
    CButton m_ButtonWeek5;
    CButton m_ButtonWeek6;
    CButton m_ButtonClueNormal;
    CButton m_ButtonClueAlong;
    CButton m_ButtonClueInterval;

    S_CLOCK_REGISTER*   m_pClockReg;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonEnterAdd();
    CEdit m_EditText;
    CEdit m_EditInterval;
    CEdit m_EditTime;
    CDateTimeCtrl m_Date;
    CDateTimeCtrl m_Time;
    afx_msg void OnBnClickedRadio13();
    afx_msg void OnBnClickedRadio14();
    afx_msg void OnBnClickedButton3();
};
