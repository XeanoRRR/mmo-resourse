/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CDriveClientDlg.h
*  ���ܣ���Ҫ���ڹ������DriveClient Dlg
*  ��ǰ�汾��1.1.0
*  ���ߣ� MartySa
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�  2009.2.2
*
============================================================
*/

#pragma once

#include "afxcmn.h"
#include "../Utility/TimerDef.h"

class CDriveClient;

class CDriveClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CDriveClientDlg)
private:
	
	//�����ͻ��˹�����
	CDriveClient	*m_DriveClientManager;

	//��ʱ��
	Timer			*m_pTimer;

public:
	
	//����DirverClient������
	void		    SetDriveClientManager(CDriveClient *driveclient)  { m_DriveClientManager = driveclient; }

	//���ö�ʱ��
	void			SetDriverClientDlgTimer(Timer *pPimer)			  {	m_pTimer = pPimer; }

public:
	
	CDriveClientDlg(CWnd* pParent = NULL);
	virtual ~CDriveClientDlg();

	enum { IDD = IDD_DriveClient };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnEnChangeEdit1();
	long m_lStartCDkey;
	long m_lEndCDkey;
	void _CreatDriveGameClient(long startcdkey, long endcdkey, unsigned uID);

//public:
//	struct tagOutputInfo
//	{
//		HANDLE hThrd;
//		HANDLE hEvent;
//		BOOL bExit;
//		tagOutputInfo(void)
//		{
//			memset(this,0,sizeof(tagOutputInfo));
//		}
//	};
//
//private:
//	tagOutputInfo m_stOutputInfo;
//
//public:
//	void SetThreadExit(BOOL b)	{ m_stOutputInfo.bExit = b; }
//	BOOL GetThreadExit()	{ return m_stOutputInfo.bExit; }
//	void SetEvent(BOOL b)	{ ::SetEvent(m_stOutputInfo.hEvent); }
//	tagOutputInfo *GetstOutPutInfo()  { return &m_stOutputInfo; }

};
