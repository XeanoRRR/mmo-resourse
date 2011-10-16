#pragma once

#include "../App/BounsDBManager.h"
#include "../Net/Message.h"
#include "../../../Public/Common/BounsDef.h"

class CRsBouns;

extern HWND g_hClientListBox;
void AddLogText(char* msg, ...);		// ����һ����¼
void AddErrorLogText(char* msg, ...);	// ����һ������ļ�¼
void RefeashInfoText();

enum eNetFlag
{
	NF_SERVER_CLIENT	=	0x001,		// GameServer��Ϊ�ͻ���
	NF_LOGSERVER_CLIENT	=	0x002,		// ��־������
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
		long	lUseLogServer;		// �Ƿ�ʹ��LogServer��0=��ʹ�ã�1=ʹ��
		string	strLogIP;			// LogServerIP
		DWORD	dwLogPort;			// LogServerPort
		string  strErrFile;			// �����ļ�·��+�ļ���
		string  strLogFile;			// ������־�ļ�·��+�ļ�����SQL��ʽ��
		string  strTxtLogFile;		// ������־�ļ�·��+�ļ��������ĸ�ʽ��
		DWORD	dwClientNo;			// �ͻ��˱��

		DWORD	dwWorkerThreadNum;	// �����߳�����
		DWORD	dwMaxBounsNum;		// ������Ʒ������ѯ��ȡ����

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
	map<DWORD, tagBounsDes>		m_mapBounsDes;

public:
	tagSetup* GetSetup() {return &m_tgSetup;}

	bool LoadSetup();
	bool LoadGSSetup(const char* pszFileName);
	bool ReLoadGSSetup();
	bool LoadMapFileSetup();							// װ���ڴ�Ӱ���ļ�����

	const char* GetStackFileName() { return m_pszRunStackFile; }

	map<DWORD, tagBounsDes>* GetBounsDes()	{return &m_mapBounsDes;}
	void AddBounsDes(tagBounsDes &tgBD);

//////////////////////////////////////////////////////////////////////////
//	���粿��
//////////////////////////////////////////////////////////////////////////
public:
	// Client��Ϣ��GameServer����Ϣ
	struct tagClientInfo
	{
		bool			bConnected;			// �÷������Ƿ��Ѿ�������
		DWORD			dwIndex;			// ���������
		string			strIP;				// IP
		DWORD			dwPort;				// �˿�
	//	long			lState;				// 0-�Ͽ���1-����
		tagClientInfo()
		{
			bConnected	= false;
			dwIndex		= 0;
			strIP		= "";
			dwPort		= 0;
		//	lState		= 0;
		}
	};

public:
	bool InitNetServer();			// ��ʼ����Ϸ����������ˣ���������������ˣ�װ������
	bool StartAccept();				// ��ʼ��Ϊ����������

	bool StartConnectLogServer();	// ������־������
	bool ReConnectLogServer();
	//void CreateConnectLogThread();	// ����������־�������߳�
	void OnLostLogServer();			


	CServer*	GetNetServer()		{ return s_pNetServer; }

	void		SetServerInitSendFlag(eServerInitSendMsgFlag flag);

	bool AddGameServer(long lGSNum, const char* strGSIP);
	bool DelGameServer(long lGSNum);
	void UpdateGSStateDisplay();
	tagClientInfo* GetGameServer(const char* strIP, DWORD dwPort);
	const char* GetGSIPByID(DWORD dwIndex);

	void OnGSLost(DWORD dwIndex);

	CRsBouns*	GetRsBouns()	{ return m_pRsBouns; }
	CBounsDBManager* GetBounsDBManager()	{ return m_pBounsDBManager; }

	size_t GetGSMapSize()	{ return s_mapGSInfo.size(); }
	

private:
	CDataBlockAllocator	*m_pDBAllocator;
	CServer				*s_pNetServer;					// ��ΪClient�ķ�������
	long				m_lServerInitSendFlag;			// ��Ӧ�ķ��������ͱ�ʾ
	CRITICAL_SECTION	m_csServerInitSendFlag;


	map<DWORD, tagClientInfo>		s_mapGSInfo;	// GameServer�б�<dwIndex, tagClientInfo>

	CRsBouns	*m_pRsBouns;
	CBounsDBManager	*m_pBounsDBManager;

//	//////////////////////////////////////////////////////////////////////////
//	//	LogClient
//	//////////////////////////////////////////////////////////////////////////
//public:
//	//!		��ӦDB��Ϣ
//	void	InitLogClient(CMessage *pMsg);
//	//!		��Ӧȷ����Ϣ
//	void	LogServerAffirm(CMessage *pMsg);
//	//!		��Ӧɸѡ������Ϣ
//	void	LogServerCondition(CMessage *pMsg);
//	
//	//!		
//	LogicLogInterface*	GetLogicLogInterface();
//
//public:
//	LogClient			&GetLogClient(void){return m_LogClient;}
//private:
//
//	LogClient			m_LogClient;
//	LogicLogInterface	*m_pLogicLogInterface;

};

BOOL CreateGame();				// ������Ϸ
BOOL DeleteGame();				// �ͷ���Ϸ
CGame* GetGame();				// ��ȡ��Ϸ
//LogicLogInterface*	GetGameLogInterface();			// ��ȡ��Ϸ��־�߼��ӿڶ���

// ��Ϸ���߳�
unsigned __stdcall GameThreadFunc(void *pArguments);
unsigned __stdcall ConnectServerFunc( void* pArguments );	// ����Server���̡߳���Server�Ͽ�ʱ���ã���ͬʱ��ر�
//unsigned __stdcall ConnectLogServerFunc(void* pArguments);	// ����LogServer���߳�

extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�
