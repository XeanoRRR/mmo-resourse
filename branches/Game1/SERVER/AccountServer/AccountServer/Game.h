#pragma once

//! ���÷�����
//#define ENABLE_FCM

#include "../public/UseMemManageAS.h"
#include "..\public\AsynchronousExecute.h"
#include "..\AppAccount\ClientManager.h"


class CServer;
class CMessage;
class CMyAdoBase;
class CMySocket;
class CDataBlockAllocator;
extern HWND g_hLogText;
extern HWND g_hWorldListBox;
extern HWND g_hCdkeyListBox;

void RefeashInfoText();					// ˢ�·�������Ϣ
void AddErrorLogText(char* msg, ...);	// ����һ�������¼
void AddLogText(char* msg, ...);		// ����һ����¼
void SaveLogText(bool bForce);			// �����¼
void AddLoginClientList(const char *str);
void DelLoginClientList(const char *str);

enum eNetFlag
{
	NF_AsscountServer_Client=0x100000,
};
// ��Ϸȫ�ֿ���
class CGame
{
public:
	CGame(void);
	virtual ~CGame(void);

public:
	BOOL Init();
	BOOL Release();
	BOOL MainLoop();

	BOOL InitNetServer_Client(void);			//��ʼ�����Client������������ˣ���������������ˣ�װ������
	BOOL AI();
	BOOL ProcessMessage();
	VOID DisConnClient(LONG lSocket);

/////////////////////////////////////////////////////////////////////////
// ��Ϸȫ�ֽӿ�
/////////////////////////////////////////////////////////////////////////
public:
	struct tagCdkey
	{
		string	strCdkey;			// cdkey
		long	lWorldNumber;		// ����World���
	};

private:
	DWORD	s_lAITick;								// ��ϷAI�����������ڹ����еĴ���

	//���ݿ������
	CDataBlockAllocator	*m_pDBAllocator;
	//������ܿͻ�����Ϣ���������
	CServer	*s_pNetServer_Client;

public:

	DWORD GetAITick()							{return s_lAITick;}
	void AddAITick()							{s_lAITick++;}

	CServer* GetNetServer_Client()	{return s_pNetServer_Client;}

	
/////////////////////////////////////////////////////////////////////////
// �̹߳���
/////////////////////////////////////////////////////////////////////////
public:
	HANDLE m_hEndEvent[2];				// �����¼�
	bool m_bExit;						// �˳���Ϸ

	AsynchronousExecute&					GetAsynchronousExecute(VOID)	{return m_AsynchronousExecute;}
	string&									GetConnectionString(VOID)		{return m_strConnectionString;}
	string&									GetFcmConnectionString(VOID)	{return m_strFcmConnectionString;}
	string&									GetSecurityConnectionString(VOID)	{return m_strSecurityConnectionString;}

private:
	//!									�첽ִ�ж���
	AsynchronousExecute					m_AsynchronousExecute;
	//!
	string								m_strConnectionString;
	string								m_strFcmConnectionString;
	string								m_strSecurityConnectionString;

/////////////////////////////////////////////////////////////////////////
// �������߼�
/////////////////////////////////////////////////////////////////////////
public:
	ClientManager&						GetClientManager(VOID) {return m_ClientManager;}
	VOID								ReLoadLsSetup(VOID){m_bReLoadLsSetup = TRUE;}

private:
	//!									�ͻ�����Ϣ�������
	ClientManager						m_ClientManager;
	BOOL								m_bReLoadLsSetup;
/////////////////////////////////////////////////////////////////////////
// ��Ϸ����
/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		LONG	lServerID;				//! AS�Լ���ID
		DWORD	dwListenPort_Client;	// ����Client��Port

		string	strSqlConType;			//sql server connection type
		string	strSqlServerIP;			//sql server ip address
		string  strSqlUserName;
		string  strSqlPassWord;
		string  strDBName;

		string	strSecuritySqlConType;			
		string	strSecuritySqlServerIP;
		string  strSecuritySqlUserName;
		string  strSecuritySqlPassWord;
		string  strSecurityDBName;

		string	strFcmSqlConType;			
		string	strFcmSqlServerIP;
		string  strFcmSqlUserName;
		string  strFcmSqlPassWord;
		string  strFcmDBName;

		//Client����������ò���
		DWORD	dwDataBlockNum;			//���ݿ�����
		DWORD	dwDataBlockSize;		//���ݿ��С
		DWORD	dwFreeMsgNum;			//Ԥ�������Ϣ����

		DWORD	dwFreeSockOperNum;		//�����������Ԥ��������
		DWORD	dwFreeIOOperNum;		//��ɶ˿���IO����Ԥ��������
		long	lIOOperDataBufNum;		//Ĭ��IO������DataBuf����
		bool	bCheckNet;				// �Ƿ��������м��
		DWORD	dwBanIPTime;			// ��ֹIP��ʱ��
		DWORD	dwMaxMsgLen;			// ������������Ϣ����		
		long	lMaxConnectNum;			// �ͻ��˵������������
		long	lMaxClientsNum;			// ���Ŀͻ��˷��ͻ�������С
		long	lPendingWrBufNum;		// ���ķ���IO����Buf�ܴ�С
		long	lPendingRdBufNum;		// ���Ľ���IO����Buf�ܴ�С
		long	lMaxSendSizePerSecond;	// ��ͻ���ÿ�뷢�͵�����ֽ���
		long	lMaxRecvSizePerSecond;	// �ӿͻ��˽��ܵ�ÿ������ֽ���
		long	lMaxBlockedSendMsgNum;	// ��������ķ�����Ϣ����
		long	lConPendingWrBufNum;	// �ͻ������ķ���IO����Buf�ܴ�С
		long	lConPendingRdBufNum;	// �ͻ������Ľ���IO����Buf�ܴ�С
		long	lConMaxSendSizePerSecond;//����������͵�ÿ������ֽ���
		long	lConMaxRecvSizePerSecond;//�ӷ��������ܵ�ÿ������ֽ���
		long	lConMaxBlockedSendMsgNum;// ��������ķ�����Ϣ����

		DWORD	dwRefeashInfoTime;		// ˢ����ʾʱ��
		DWORD	dwSaveInfoTime;			// ����һ����Ϣ��ʱ����
		DWORD	dwDoQueueInter;			// ����ȴ����еļ��ʱ��
		DWORD	dwSendMsgToQueInter;	// ���ȴ����з�����Ϣ�ļ��ʱ��

		DWORD	dwWorldMaxPlayers;		// ��½WorldServer���������
		float	fWorldBusyScale;		// ��æ����������
		float	fWorldFullScale;		// ��������������

		DWORD	dwPingWorldServerTime;	// ��ʱPingWorldServer�ļ��
		DWORD	dwPingWorldServerErrorTime;	// PingWorldServer���յ����صĳ���ʱ����

		bool	bCheckForbidIP;			// �Ƿ������IP����
		bool	bCheckAllowIP;			// �Ƿ���IP������
		bool	bCheckBetweenIP;		// ���ip�Ƿ�������IP��

		DWORD	dwMinDbOptThreadNum;	//! ��СDB�����̱߳�����
		DWORD	dwMaxDbOptThreadNum;	//! ���DB�����߳���
		DWORD	dwThreadNumAdjustSpace;	//! �Զ������߳����ļ��ʱ��

		tagSetup()
		{
			bCheckNet = 1;
			dwMaxMsgLen = 102400;
			dwBanIPTime = 10;
			dwRefeashInfoTime = 1000;
			dwSaveInfoTime = 60000;
		}
	};

	struct tagSetupEx
	{
		long	lClientMaxBlockConNum;
		long	lClientValidDelayRecDataTime;
	};

private:
	tagSetup	m_Setup;
	tagSetupEx	m_SetupEx;

public:
	bool		LoadSetup();
	bool		ReLoadSetup();
	bool		LoadSetupEx();
	bool		ReLoadSetupEx();


	tagSetup*	GetSetup()	{return &m_Setup;}
	tagSetupEx* GetSetupEx(){return &m_SetupEx;}

};

BOOL CreateGame();			// ������Ϸ
CGame* GetGame();			// ��ȡ��Ϸ
BOOL DeleteGame();			// �ͷ���Ϸ

unsigned __stdcall GameThreadFunc(void* pArguments);	// ��Ϸ���߳�

extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�