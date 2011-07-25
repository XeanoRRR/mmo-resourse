// AlarmClockDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"
#include "AlarmClockAddDlg.h"
#include "AlarmClockLookDlg.h"
#include "MsSound.h"

// CAlarmClockDlg �Ի���
class CAlarmClockDlg : public CDialog
{
// ����
public:
    CAlarmClockDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CAlarmClockDlg(void);

// �Ի�������
	enum { IDD = IDD_ALARMCLOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    friend DWORD WINAPI CallBackFun(LPVOID lpThreadParameter);

// ʵ��
protected:
	HICON               m_hIcon;                // ͼ����
    NOTIFYICONDATA      m_nid;                  // ������Ϣ
    CMenu               m_Menu;                 // ���̲˵�
    UINT                m_RegCount;             // ��ʾ��¼����

    AlarmClockAddDlg*   m_pAddDlg;
    AlarmClockLookDlg*  m_pLookDlg;

    S_CLOCK_REGISTER    m_OnceClockReg[MAX_CLOCK_COUNT];    // һ����ʾ��¼����
    UINT                m_OnceClockRegCount;                // һ����ʾ��¼����

    S_CLOCK_REGISTER    m_EverydayClockReg[MAX_CLOCK_COUNT];// ÿ����ʾ��¼����
    UINT                m_EverydayClockRegCount;            // ÿ����ʾ��¼����

    S_CLOCK_REGISTER    m_Week0ClockReg[MAX_CLOCK_COUNT];   // ������ʾ��¼����
    UINT                m_Week0ClockRegCount;               // ������ʾ��¼����

    S_CLOCK_REGISTER    m_Week1ClockReg[MAX_CLOCK_COUNT];   // ��һ��ʾ��¼����
    UINT                m_Week1ClockRegCount;               // ��һ��ʾ��¼����

    S_CLOCK_REGISTER    m_Week2ClockReg[MAX_CLOCK_COUNT];   // �ܶ���ʾ��¼����
    UINT                m_Week2ClockRegCount;               // �ܶ���ʾ��¼����

    S_CLOCK_REGISTER    m_Week3ClockReg[MAX_CLOCK_COUNT];   // ������ʾ��¼����
    UINT                m_Week3ClockRegCount;               // ������ʾ��¼����

    S_CLOCK_REGISTER    m_Week4ClockReg[MAX_CLOCK_COUNT];   // ������ʾ��¼����
    UINT                m_Week4ClockRegCount;               // ������ʾ��¼����

    S_CLOCK_REGISTER    m_Week5ClockReg[MAX_CLOCK_COUNT];   // ������ʾ��¼����
    UINT                m_Week5ClockRegCount;               // ������ʾ��¼����

    S_CLOCK_REGISTER    m_Week6ClockReg[MAX_CLOCK_COUNT];   // ������ʾ��¼����
    UINT                m_Week6ClockRegCount;               // ������ʾ��¼����

    BOOL                m_bRun;
    BOOL                m_IsExit;

    CMsSound            m_CMsSound;
	// ���ɵ���Ϣӳ�亯��
    BOOL            ShowBallute(TCHAR* pText);
    void            OnGetToTime(S_CLOCK_REGISTER* pRegister, UINT uIndex);
    virtual BOOL    OnInitDialog();
    afx_msg void    OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void    OnDestroy();
    afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
    afx_msg void    OnClose();
    afx_msg void    OnMenuShow();
    afx_msg void    OnMenuExit();
    afx_msg void    OnBnClickedButtonAdd();
    afx_msg void    OnBnClickedButtonLook();
    DECLARE_MESSAGE_MAP()
};
