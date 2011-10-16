/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�GameTest.h
*  ���ܣ�GametestMFC��������
*  ��ǰ�汾��1.2.0
*  ���ߣ� LGR
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�2009
*
*  1 .�޸ģ�����������ʾ����	 BY:LGR		 Date:2010.3.5   
============================================================
*/

#pragma once
//#include "../App/GameClient.h"
//#include "afxcmn.h"
//#include "DriveClientDlg.h"
//#include "..\Script\MapScriptSystem.h"
//#include "SkillTest.h"
#include "../Utility/TimerDef.h"
#include "TestInfo.h"
#include "TestScript.h"
#include "AllTestDlg.h"
#include "TestResult.h"
#include "..\App\AutoDriveClient\AutoDriveClient.h"
#include "TaskbarNotifier.h"

#define  WM_TRAYICON  WM_USER+101 //����    

class CClientRegion;

class CDataBlockAllocator;
class CGameClient;
class CScriptSys;
class CServer;
class CCDKey;
class CDriveClient;
//class CMapScriptSystem;
class CSkillTest;
class CAllTestDlg;

//CRITICAL_SECTION g_csMapFile; 

class CGameTestDlg : public CDialog
{
public:

	CGameTestDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_GAMETEST_DIALOG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);

public:

	virtual BOOL	OnInitDialog();
	afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void	OnPaint();
	afx_msg void	OnTimer(UINT nIDEvent);
	afx_msg	void	OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();

	//����
	afx_msg LRESULT OnTrayIcon(WPARAM wparam,LPARAM lparam);    
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);   
	afx_msg void	OnLButtonDBClick(UINT nFlags, CPoint point);
	afx_msg void	OnBnClickedOK();  
	afx_msg void	OnShow() {};
	afx_msg void	OnShowInfo(){ m_bTaskbarNotifier = true; }
	afx_msg void	OnShowConnectInfo();
	afx_msg void	SetTaskbarNotifierInfo(const char* pchar);
	afx_msg void	OnHideDlg();
	afx_msg void	OnShowDLg();
	afx_msg void	OnShowTestReslut();
	afx_msg void	OnExit();
	afx_msg LRESULT OnTaskbarNotifierClicked(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL	TrayNotifyMsg(DWORD Msg, UINT Icon, char* csTip);

	DECLARE_MESSAGE_MAP()

private:

	HICON m_hIcon;

	//������Ϣ��ʾ
	CTaskbarNotifier m_TaskbarNotifier;
	CTaskbarNotifier m_TaskbarNotifierEnd;

private:

	//���ݿ������
	CDataBlockAllocator* m_pDBAllocator;

	//CDataBlockAllocator *m_pFileMapDBAllocator;

	//����ϵͳ
	CServer* m_pNetServer;

	//����ͻ��˹���
	typedef map<long,CGameClient*>	MultiGameClients;
	typedef map<long,CGameClient*>::iterator	itGC;
	MultiGameClients	m_MultiGameClients;

	////[8/10/2009 Square]
	typedef map<CGUID,CClientRegion*>	CMapClientRegion;
	typedef CMapClientRegion::iterator	itMapClientRegion;
	CMapClientRegion	m_mapClientRegion;
	long	m_ltbWaitLoginCount;

public:

	CClientRegion*	FindClientRegion( long lResourceID,const CGUID& guidRgn = CGUID::GUID_INVALID );
	long GettbWaitLoginCount(){ return  m_ltbWaitLoginCount;}
	void SettbWaitLoginCount(long lIndex){ m_ltbWaitLoginCount = lIndex;}

private:

	//AutoDrive�������		by MartySa 2009.2.1 (�¼�)
	CDriveClient	*m_DriveClientManager;		
	
	//��ʱ��
	Timer*	m_pTimer;
	UINT	m_nMemMapTimerID;

	//���̶̹�ˢ�¿���
	bool    m_bTaskbarNotifier;
	bool    m_bOnTbClicked;
	
	//�ű����
	const string m_strGlTbDbFileName;
	const string m_strRequireFileName;

	BOOL m_bInitResult;

public:
	//��Ӷ���������Ŀ���Ա���(����Ϊ�ǳ�������)  BY:LGR
	static int  m_iProjName;
 	enum m_eProjName
 	{
 		PROJ_XINGCHEN,
 		PROJ_PANLONG
 		//...
 	};

public:

	//��ȡ�ڴ�ӳ��������
	CDriveClient		   *GetDriveClientManager()	{ return m_DriveClientManager; }
	map<long,CGameClient*> GetClient()				{ return m_MultiGameClients; }

	//��ʼ��
	bool	IniDBAllocator();
	bool	IniTimer();
	bool	IniGameClients();
	bool	IniScriptSys();
	bool	IniTestReporter();
	bool	IniMessage();
	bool	IniNetServer();
	BOOL	IniQuestSys(const char*);
	bool	IniMappingFileScriptSys();			//	MapFileScriptSys by MartySa 2009.2.1 (�¼�)
	bool	LoadDifProjRes();					//�ṩ�ֶ���ȡ������Դ�ӿ�   By: LGR 2010.11.2

	//�ͷ�
	bool	ReleaseDBAllocator();
	bool	ReleaseTimer();
	bool	ReleaseGameClients();
	bool	ReleaseScriptSys();
	bool	ReleaseTestReporter();
	bool	ReleaseNetServer();
	bool	ReleaseMessage();
	bool	ReleaseMappingFileScriptSys();		//	MapFileScriptSys by MartySa 2009.2.1 (�¼�)

	Timer*  GetTimer()		{ return m_pTimer; }
	CServer* GetNetServer() { return m_pNetServer; }

	void	CreateClient(long lSocketID);
	void	DeleteGameClient(long lSocketID);
	void	OnGameTimerOut();

	CGameClient*	GetGameClient(long lSocketID);
	CGameClient*	GetOneGameClient();

public:

	//MFC������
	CTabCtrl	m_tabPage;
	CTestInfo m_Info;
	CTestScript m_Script;
	CTestResult  m_TsRlt;
	//CDriveClientDlg m_DriveClientDlg;
	CAllTestDlg m_dlgAllTest;

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

public:

	//CDriveClientDlg* GetDriveDlg()	{ return &m_DriveClientDlg; }
	int CGameTestDlg::SetFlashWindows();
	int OperMessageBox( const char* pchar );
	CTestInfo* GetTestInfo()	{ return &m_Info; }
	CTestScript* GetTestScript()	{ return &m_Script; }
	CTestResult* GetTestResult()	{ return &m_TsRlt;}
	

	void ResetDisplayInfoGameClient( long lSelClient );
	void OutputScriptInfo( const char* strInfo );
	void UpdataTreeGameClientAmount();

	void S2C_Login( CMessage* msg );
	void L2C_MsgAnswerCDKey( CMessage* msg );
	void L2C_MsgAnswerPlayerBastList( CMessage* msg );
	void L2C_MsgAnswerPlayerData( CMessage* msg );
	void L2C_MsgAnswerCreate( CMessage* msg );
	void S2C_MsgAnswerPlayerDate( CMessage* msg );

private:

	msgQueue MsgQueueMapFile;
//	CRITICAL_SECTION g_csMapFile;
//	CMapFile * m_pMapFile;

public:

	void FreeMsg(CMessage* pMsg)	{ if( pMsg != NULL ) CBaseMessage::FreeMsg(pMsg); }
//	CRITICAL_SECTION* GetTestCS()	{ return &g_csMapFile; }
	msgQueue& GetMsgQuesuMapFile()	{ return MsgQueueMapFile; }
	CMemServer *GetMapFileManager()	{ return CDriveClient::GetMemServerInstance(); } 
	void OnMapTimer();

private:

	//�߳����
	enum 
	{
		WORKER_MSG,
		WORKER_LUA,
	};

	HANDLE		m_WorkerExit[2];
	HANDLE		m_WorkerEvent[2];
	CWinThread*	m_ArrWorker[2];

	static UINT MessageWorkerFunc( LPVOID lParam );
	void DoMessageWorker();
	
	static UINT TimerWorkerFunc( LPVOID lParam );
	void DoTimerWorker();
public:
	void Exit();
	static UINT __stdcall ExitThread( LPVOID lParam );
	afx_msg void OnTcnSelchangingTabPage( NMHDR *pNMHDR, LRESULT *pResult );

public:

	//����ʢ��ѹ������ƽ̨   square  09.8.3
	struct	tagTokenCDKey
	{
		//�����ʺ�
		string	strNumCDKey;
		//token
		string	strToken;
		BOOL	bUsed;
	};

private:

	typedef list<tagTokenCDKey*>	CListTokenCDKey;
	typedef CListTokenCDKey::iterator	itListTokenCDKey;
	CListTokenCDKey		m_listFreeTokenKey;

	typedef map<string,tagTokenCDKey*>	CMapTokenCDKey;
	typedef	CMapTokenCDKey::iterator itMapTokenKey;
	CMapTokenCDKey		m_mapUsedTokenKey;

	VOID	ReleaseToken();
	BOOL	InitTokenFile( const char* filename );
	BOOL	InitPtSessionSetup( const char* filename );

public:

	tagTokenCDKey*	AllocTokenCDKey();
	void	FreeTokenCDKey( string const strNumKey );

};
