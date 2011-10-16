#pragma once
#include "..\..\..\nets\net\message.h"
#include "..\..\..\LogClient\server\LogClient.h"
#include "..\..\..\LogClient\server\LogicLogInterface.h"


void AddLogText(char* msg, ...);		// ����һ����¼
void AddErrorLogText(char* msg, ...);	// ����һ������ļ�¼

enum eNetFlag
{
	NF_SERVER_CLIENT	=	0x001,
	NF_GATE_CLIENT		=	0x002,
	NF_LSERVER_CLIENT	=	0x010,
	NF_CSERVER_CLIENT	=	0x100,
	NF_DBSERVER_CLIENT	=	0x1000,
	NF_LOGSERVER_CLIENT =	0x10000,
};

enum eServerInitSendMsgFlag
{
	NULL_SERVER_INITSENDFLAG	= 0,
	DB_SERVER_INITSENDFLAG		= 1,
	LOGIN_SERVER_INITSENDFLAG	= 2,
	LOG_SERVER_INITSENDFLAG		= 4,
};

// ��Ϸȫ�ֿ���
class CGame
{
public:
	CGame(void);
	virtual ~CGame(void);

	BOOL Init();
	// ��ʼ����ջ�ļ���
	BOOL InitStackFileName();
	BOOL Release();
	BOOL MainLoop();
	BOOL AI();
	BOOL ProcessMessage();

public:
	bool LoadServerResource();


/////////////////////////////////////////////////////////////////////////
	// ��Ϸ����
	/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		string	strServerIP;			// ServerIP
		DWORD	dwServerPort;			// ServerPort

		long	lUseLogServer;		// �Ƿ�ʹ��LogServer��0=��ʹ�ã�1=ʹ��
		string	strLogIP;			// LogServerIP
		DWORD	dwLogPort;			// LogServerPort
		string  strErrFile;			// �����ļ�·��+�ļ���
		string  strLogFile;			// ������־�ļ�·��+�ļ�����SQL��ʽ��
		string  strTxtLogFile;		// ������־�ļ�·��+�ļ��������ĸ�ʽ��
		DWORD	dClientNo;			// �ͻ��˱��

		DWORD	dwNumber;			// ���������(0-7)
		string	strName;			// ����������
		string	strLocalIP;			// ����IP 
		DWORD	dwListenPort;		// ����port
		
		string	strSqlConType;		//sql server connection type
		string	strSqlServerIP;		//sql server ip address
		string  strSqlUserName;		//sql server user name
		string  strSqlPassWord;		//password
		string  strDBName;			//database name

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

		DWORD	dwGappID;				//�ڴ�ӳ��ͻ���ID

		tagSetup()
		{
			dwRefeashInfoTime	= 1000;
			dwSaveInfoTime		= 60000;
		}
	};

private:
	tagSetup m_tgSetup;

	// ÿ�����²����ļ�¼��ջ���ļ�����
	char m_pszRunStackFile[MAX_PATH];

public:
	tagSetup* GetSetup() {return &m_tgSetup;}

	bool LoadSetup();
	bool LoadServerSetup(const char* pszFileName);
	bool LoadMapFileSetup();							// װ���ڴ�Ӱ���ļ�����

	const char* GetStackFileName() { return m_pszRunStackFile; }

//////////////////////////////////////////////////////////////////////////
//	���粿��
//////////////////////////////////////////////////////////////////////////
public:
	// Client��Ϣ
	struct tagClientInfo
	{
		bool			bConnected;			// �÷������Ƿ��Ѿ�������
		DWORD			dwIndex;			// ���������
		string			strIP;				// IP
		DWORD			dwPort;				// �˿�
	};

public:
	bool InitNetServer();			// ��ʼ����Ϸ����������ˣ���������������ˣ�װ������
	bool StartAccept();				// ��ʼ��Ϊ����������

	bool StartConnectServer();		// ��Ϊ�ͻ���������Ӧ�ķ�����
	bool ReConnectServer();			// �������ӷ�����
	void CreateConnectServerThread();
	void OnLostServer();			// ��������Ͽ�����
	bool StartConnectLogServer();	// ������־������
	bool ReConnectLogServer();
	void CreateConnectLogThread();	// ����������־�������߳�
	void OnLostLogServer();			


	CServer*	GetNetServer()		{ return s_pNetServer; }
	long		GetServerSocket()	{ return m_lServerSocket; }	 

	void		SetServerInitSendFlag(eServerInitSendMsgFlag flag);
	

private:
	CDataBlockAllocator	*m_pDBAllocator;
	CServer				*s_pNetServer;					// ��ΪClient�ķ�������
	long				m_lServerSocket;				// ����Server�Ŀͻ���		
	long				m_lServerInitSendFlag;			// ��Ӧ�ķ��������ͱ�ʾ
	CRITICAL_SECTION	m_csServerInitSendFlag;


	map<DWORD, tagClientInfo>		s_mapClientInfo;	// �������б�<dwIndex, tagClientInfo>

	//////////////////////////////////////////////////////////////////////////
	//	LogClient
	//////////////////////////////////////////////////////////////////////////
public:
	//!		��ӦDB��Ϣ
	void	InitLogClient(CMessage *pMsg);
	//!		��Ӧȷ����Ϣ
	void	LogServerAffirm(CMessage *pMsg);
	//!		��Ӧɸѡ������Ϣ
	void	LogServerCondition(CMessage *pMsg);
	
	//!		
	LogicLogInterface*	GetLogicLogInterface();

public:
	LogClient			&GetLogClient(void){return m_LogClient;}
private:

	LogClient			m_LogClient;
	LogicLogInterface	*m_pLogicLogInterface;

};

BOOL CreateGame();				// ������Ϸ
BOOL DeleteGame();				// �ͷ���Ϸ
CGame* GetGame();				// ��ȡ��Ϸ
LogicLogInterface*	GetGameLogInterface();			// ��ȡ��Ϸ��־�߼��ӿڶ���

// ��Ϸ���߳�
unsigned __stdcall GameThreadFunc(void *pArguments);
unsigned __stdcall ConnectServerFunc( void* pArguments );	// ����Server���̡߳���Server�Ͽ�ʱ���ã���ͬʱ��ر�
unsigned __stdcall ConnectLogServerFunc(void* pArguments);	// ����LogServer���߳�

extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�
