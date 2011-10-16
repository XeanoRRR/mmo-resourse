#pragma once

#include "../../../Public/Common/NetSessionManager.h"
#include "../../../Public/ServerPublic/Server/Common/AsynchronousExecute.h"

class LogManage;
class CServer;
class CDBCountry;
class CDataBlockAllocator;

class CMessage;
class CCostNetClient;
extern HWND g_hLogText;
extern HWND g_hPlayerListBox;

void RefeashInfoText(const char *pInfoText);					// ˢ�·�������Ϣ
void AddInfoText(char* msg, ...);								// ����һ����¼
void AddLogText(char* msg, ...);								// ����һ����¼
void AddErrorLogText(char* msg,...);							// ����һ�������¼
void AddClientList(const char *str);
void DelClientList(const char *str);

enum eNetFlag
{
	NF_Server_Client=0x001,
	NF_LServer_Client=0x010,
	NF_CServer_Client=0x100,
};

//class CDBGoods;
// ��Ϸȫ�ֿ���
class CGame
{
public:
	CGame(void);
	virtual ~CGame(void);

	BOOL Init();
	BOOL Release();
	BOOL MainLoop();

	
	//////////////////////////////////////////////////////////////////////////
	//	ȫ�ֽű����� (�����ݿ�)
	//////////////////////////////////////////////////////////////////////////
public:


private:


	//////////////////////////////////////////////////////////////////////////
	//	���� ����������������Ҫ�����ݿ�����ݣ�
	//////////////////////////////////////////////////////////////////////////
public:


public:

private:




	//////////////////////////////////////////////////////////////////////////
	//	GameServer�������
	//////////////////////////////////////////////////////////////////////////
public:
	// GameServer��Ϣ
	struct tagGameServer
	{
		bool			bConnected;			// �÷������Ƿ��Ѿ�������
		DWORD			dwIndex;			// ���������
		string			strIP;				// IP
		DWORD			dwPort;				// �˿�

		long			lReceivedPlayerData;	// ���յ��������������(��ʱ��)
	};

public:
	tagGameServer* GetGameServer(DWORD dwIndex)	{return (s_mapGameServer.find(dwIndex)!=s_mapGameServer.end())?&s_mapGameServer[dwIndex]:NULL;}
	tagGameServer* GetGameServer(const char* strIP, DWORD dwPort);			// ��ȡGameServer������
	tagGameServer* GetRegionGameServer(long lRegionID);						// ��ȡregion���ڵ�GameServer
	tagGameServer* GetPlayerGameServer(const CGUID& PlayerGuid);						// ��ȡplayer���ڵ�GameServer

	long GetGameServerNumber_ByRegionID(long lRegionID);
	long GetGameServerNumber_ByPlayerID(const CGUID& PlayerGuid);


	bool LoadServerSetup(const char *filename);								// ��ȡ��Ϸ����������
	long GetConnectedGameServerCount();										// �����Ѿ������ϵ�GameServer����

	void OnGameServerConnect(DWORD dwIP, LONG lSocket);
	void OnGameServerClose(long lSocket);									//��ĳ��GameServer�Ͽ����ӵ�ʱ��
	void OnGameServerLost(DWORD dwIndex);									// ��ĳ��GameServer�Ͽ�ʱ�Ĵ���

	map<DWORD, tagGameServer>* GetGameServerList(){return &s_mapGameServer;}

private:
	map<DWORD, tagGameServer>		s_mapGameServer;	// �������б�<dwIndex, tagGameServer>
	set<DWORD>						m_setClientIP;

	BOOL ProcessMessage();
	bool ProcMenus();

	//////////////////////////////////////////////////////////////////////////
	//	�����ӿ�
	//////////////////////////////////////////////////////////////////////////
public:                                          
	void SendMsg2GameServer(DWORD dwIndex,CMessage* pMsg);					// ����һ����Ϣ��ָ��Index��GameServer
	void SendAllGameServer(CMessage* pMsg);					// ����һ����Ϣ������GameServer


	AsynchronousExecute&	GetAsynchronousExecute(VOID){return m_AsynchronousExecute;};


private:

	DWORD	m_dwLastReLoginTime;											// �ϴ�����LoginServer��ʱ��

	AsynchronousExecute		m_AsynchronousExecute;
	//////////////////////////////////////////////////////////////////////////
	//	���粿��
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL InitNetServer(void);					//��ʼ����Ϸ����������ˣ���������������ˣ�װ������
	//BOOL StartConnLS(void);						//����LoginServer
	bool StartAccept();							//��ʼ��Ϊ����������

	
	CServer* GetNetServer()	{return s_pNetServer;}

private:
	CDataBlockAllocator	*m_pDBAllocator;
	CServer				*s_pNetServer;			// ��ΪGameServer�ķ�������
	long				m_lLogingSocket;		//����loginserver�Ŀͻ���
	long				m_lCostSocket;			//����costserver�Ŀͻ���

	

	/////////////////////////////////////////////////////////////////////////
	// ��Ϸ����
	/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		DWORD	dwNumber;			// ���������(0-7)
		string	strName;			// ����������
		DWORD	dwListenPort;		// ����Port
		//string	strSqlConType;		//sql server connection type
		//string	strSqlServerIP;		//sql server ip address
		//string  strSqlUserName;		//sql server user name
		//string  strSqlPassWord;		//password
		//string  strDBName;			//database name

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

		DWORD	dwReleaseLoginPlayerTime;			// �ͷ�LoginList���Player�ĳ�ʱʱ��


		DWORD	dwMinDbOptThreadNum;	//! ��СDB�����̱߳�����
		DWORD	dwMaxDbOptThreadNum;	//! ���DB�����߳���
		DWORD	dwThreadNumAdjustSpace;	//! �Զ������߳����ļ��ʱ��

		// ��־ϵͳ���ݿ����������
		//bool	bUseLogSys;			//�Ƿ�������־ϵͳ�����ݿ������
		//string	strLogSysProvider;	
		//string	strLogSysServer;
		//string	strLogSysDatabase;
		//string	strLogSysUID;
		//string	strLogSysPWD;

		tagSetup()
		{
			dwRefeashInfoTime = 1000;
			dwSaveInfoTime = 60000;
			//bUseLogSys = false;
		}
	};

private:
	tagSetup	m_Setup;
	
public:
	bool		LoadSetup();
	bool		LoadServerResource();
	tagSetup*	GetSetup()	{return &m_Setup;}
	//CDupliRegionSetup* GetDupliRegionSetup()	{return m_pDupliRegionSetup;}

	

	//////////////////////////////////////////////////////////////////////////
	//	��ʱping
	//////////////////////////////////////////////////////////////////////////
public:

	// GameServer��Ϣ
	struct tagPingGameServerInfo
	{
		string	strIP;
		DWORD	dwPort;
		DWORD	dwPlayerNum;
	};

	void AddPingGameServerInfo(tagPingGameServerInfo& st)	{m_vectorPingGameServerInfo.push_back(st);}
	void ClearPingGameServerInfo()							{m_vectorPingGameServerInfo.clear();}
	void SetInPing(bool b)									{m_bInPing=b;}
	void SetLastPingGameServerTime(DWORD dw)				{m_dwLastPingGameServerTime=dw;}

private:

	vector<tagPingGameServerInfo>	m_vectorPingGameServerInfo;
	bool	m_bInPing;							// �Ƿ���Ping�Ĺ�����
	DWORD	m_dwLastPingGameServerTime;			// �ϴη���ping��ʱ��

	//////////////////////////////////////////////////////////////////////////
	// �첽�����ݿ��ȡ�����Ϣ
	//////////////////////////////////////////////////////////////////////////

private:

public:

	//void ProcessPlayerDataQueue();

	///////////////////////////////
	// ��־����
	///////////////////////////////
private:
	//! ��Ϸ��־�������
	LogManage *m_pGameLogManage;
	//!	��Ӧ�˵��еĴ�������ã�������־��Ϊ�������߳��н����������ݿ����
	BOOL	m_bCreateTable;
	//CWriteLogQueue m_qWriteLogData;
public:
	void OnClientLogin(CMessage *pMsg);
	void OnLog(CMessage *pMsg);
	void OnStrSqlLog(CMessage *pMsg);
	void CreateTableInDB(void);
	void CreateTableInSqlFile(void);
	void OutTableInfoToFile(void);
	void ReLoadConfig(void);
	//CWriteLogQueue* GetWriteLogDataQueue()			{ return &m_qWriteLogData; }


};// end of class


BOOL CreateGame();			// ������Ϸ
CGame* GetGame();			// ��ȡ��Ϸ
BOOL DeleteGame();			// �ͷ���Ϸ

unsigned __stdcall GameThreadFunc(void* pArguments);	// ��Ϸ���߳�


extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�

// ��ʾ������Ϣ����
extern bool g_bShowSaveInfo;

void ShowSaveInfo(LPCSTR szInfo, ...);
