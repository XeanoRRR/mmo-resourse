#pragma once

#include "../Net/Message.h"
#include "../../../Public/Common/NetSessionManager.h"

class CServer;
class CEntity;
class CEntityGroup;
class CRsEntity;

class CDupliRegionSetup;
class CDataBlockAllocator;

class CVariableList;
class CCostNetClient;
class CEntityManager;
class DBAdoBase;
class DBThread;
class CMapFile;

struct tagEnemyFaction;
struct tagVilWarSetup;
struct tagAttackCityTime;
struct ThreadData;

extern CRITICAL_SECTION g_CriticalSectionSavePlayerList;	// ��������ҵ��߳�ͬ��(GameThread <-> SaveThread)
extern CRITICAL_SECTION g_CriticalSectionSaveThread;		// �����߳�ͬ��(SaveThread <-> SystemThread)

enum eNetFlag
{
	NF_Server_Client=0x001,
	NF_LServer_Client=0x010,
	NF_CServer_Client=0x100,
	NF_DBServer_Client=0x1000,
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
	BOOL AI();
	BOOL ProcessMessage();
	BOOL InitStackFileName();
	BOOL InitMainStackFileName();

	//////////////////////////////////////////////////////////////////////////
	//	WorldServer�������
	//////////////////////////////////////////////////////////////////////////
public:
	// WorldServer��Ϣ
	struct tagWorldServer
	{
		bool			bConnected;			// �÷������Ƿ��Ѿ�������
		DWORD			dwIndex;			// ���������
		string			strIP;				// IP
		DWORD			dwPort;				// �˿�
	};

public:
	tagWorldServer* GetWorldServer(DWORD dwIndex)	{return (s_mapWorldServer.find(dwIndex)!=s_mapWorldServer.end())?&s_mapWorldServer[dwIndex]:NULL;}
	tagWorldServer* GetWorldServer(const char* strIP, DWORD dwPort);			// ��ȡGameServer������
	bool LoadWSSetup(const char *filename);										// ��ȡWS��Ϸ����������
	long GetConnectedWorldServerCount();										// �����Ѿ������ϵ�WS����
	bool OnWorldServerClose(DWORD dwIndex);									//��ĳ��WS�Ͽ����ӵ�ʱ��
	bool OnWorldServerLost(DWORD dwIndex);									// ��ĳ��WS�Ͽ�ʱ�Ĵ���

	map<DWORD, tagWorldServer>* GetWorldServerList(){return &s_mapWorldServer;}

private:
	map<DWORD, tagWorldServer>		s_mapWorldServer;	// �������б�<dwIndex, tagWorldServer>
    bool ProcMenus();

	//////////////////////////////////////////////////////////////////////////
	//	���ݿ����
	//////////////////////////////////////////////////////////////////////////
public:	
	CRsEntity* GetRsEntity()					{return m_pRsEntity;}  
	
private:
	CRsEntity			*m_pRsEntity;				// ����
	
	/////////////////////////////////////////////////////////////////////////
	// WorldServer��Ϣ�б�
	/////////////////////////////////////////////////////////////////////////
public:
	struct tagWorldInfo
	{
		string	strName;						// ��������
		long	lStateLvl;						// ͳ�Ʒ�������״̬0:ά����1:0-75%,2:75-90%,3:90%-100%
	};

private:
	map<long,tagWorldInfo>		m_listWorldInfo;

	map<long,tagWorldInfo>		m_WorldInfoSetup;//�����ļ��е�WorldServer��Ϣ
public:

	bool LoadWorldSetup();
	void SetListWorldInfoBySetup();

	bool IsExitWorld(const char* strWorldName);
	long GetWorldNum()					{return (long)m_listWorldInfo.size();}

	void AddWorldInfoToMsg(CMessage& msg,string &strCDkey);

	long AddWorld(long lWorldNumber,const char* strWorldName);			// �������
	long DelWorld(long lNum);	// ɾ������

	bool WorldServerIsOpenState(long lNum);

	long GetWorldSateLvl(long lPlayerNum);
	void OnChangeStateLvl(long lWorldNum,long lPlayerNum);
	void ChangeAllWorldSate();
	const char *GetWorldNameByID(long lNum);
	long  GetWorldIDByName(const char* strWorldName);
	bool SendMsg2World(CMessage* pMsg,long lWorldServerID);	//������Ϣ��ָ����WORLDSERVER

	//////////////////////////////////////////////////////////////////////////
	//	�����ӿ�
	//////////////////////////////////////////////////////////////////////////
public:
	void SendMsg2WS(DWORD dwIndex,CMessage* pMsg);					// ����һ����Ϣ��ָ��Index��S
	//! [2007-7-19 add]
	void SendAllWS(CMessage* pMsg);					// ����һ����Ϣ������WS
	//! [2007-7-19 add end]

	DWORD GetAITick()							{return s_lAITick;}
	void AddAITick()							{s_lAITick++;}

private:
	DWORD	s_lAITick;														// ��ϷAI�����������ڹ����еĴ���
	DWORD	m_dwLastReLoginTime;											// �ϴ�����LoginServer��ʱ��
	//////////////////////////////////////////////////////////////////////////
	//	���粿��
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL InitNetServer(void);					//��ʼ����Ϸ����������ˣ���������������ˣ�װ������
	bool StartAccept();							//��ʼ��Ϊ����������

	
	CServer* GetNetServer()	{return s_pNetServer;}

	bool	LoadMapFileSetup();

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
		string	strLoginIP;			// LoginServerIP
		DWORD	dwLoginPort;		// LoginServerPort
		DWORD	dwListenPort;		// ����Port
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


		DWORD dwTravleMaxSize; // ���߳�ÿ�α��������б��������
		DWORD dwPlayerLoadThreadNumber;	
		DWORD dwPlayerSaveThreadNumber;
		DWORD dwPingWorldServerTime;
		DWORD dwPingWorldServerErrorTime;
		DWORD dwClearPlayerDataTime;

		DWORD dwViewDbOperDeltaTime; // ��ʾ���ݿ������ʱ����ֵ
		DWORD dwGappID;				 //ӳ���ļ��ͻ���ID
        bool  bOpenRanks;               // ���а񿪹�
        DWORD dwRefreshRanksTime;       // ��������ˢ��ʱ����(����)
		tagSetup()
		{
			dwRefeashInfoTime = 1000;
			dwSaveInfoTime = 60000;
		}

	};

private:
	tagSetup	m_Setup;
	
public:
	bool		LoadSetup();
	bool		LoadServerResource();
	tagSetup*	GetSetup()	{return &m_Setup;}

public:
	//ϵͳ�㲥���ݽṹ
	struct tagSysBroadcast
	{
		long	lImportLevel;		//��Ҫ��Ϊ0����Ϣֻ������������ڣ�Ϊ1����Ϣ���������Ļ�Ϸ���GM����λ��
		long	lRegionID;			//�������Ϊ0����ȫ����㲥
		DWORD	dwMinTime;			//С���ʱ�䳤��
		DWORD	dwMaxTime;			//����ʱ�䳤��
		DWORD	dwOdds;				//ÿ����Ϣ����ָ�����ֵļ��ʣ�100��֮N
		DWORD	dwTextColor;		//������ɫ
		DWORD	dwBackColor;		//��������ɫ�����ָ��255��255��0��255��ô����ʾ��������
		string	strMessage;			//�㲥����

		DWORD	dwIntervalTime;		// ʵ�ʵļ��ʱ��
		DWORD	dwLastNotifyTime;	// ��һ�η��ͳ�ȥ��ʱ��
	};

	list<tagSysBroadcast>* GetBroadcastList(){return &m_listBroadcast;}

private:
	list<tagSysBroadcast> m_listBroadcast;			//ϵͳ�㲥�����б�

	//////////////////////////////////////////////////////////////////////////
	//	��ʱping
	//////////////////////////////////////////////////////////////////////////
public:
	// WorldServer��Ϣ
	struct tagPingWorldServerInfo
	{
		string	strIP;
		DWORD	dwPort;
		DWORD	dwPlayerNum;
		vector<tagPingWorldServerInfo>	vectorGameServerInfo;
	};

	void AddPingWorldServerInfo(tagPingWorldServerInfo& st)	{m_vectorPingWorldServerInfo.push_back(st);}

private:
	vector<tagPingWorldServerInfo>	m_vectorPingWorldServerInfo;
	
private:
	bool m_bInPing; 
	DWORD m_dwLastPingGameServerTime;

public:
	CEntityManager* GetEntityManager(void);

	long GetWSSocket(void) { return m_lWSSocket; }
	void SetWSSocket(long s) { m_lWSSocket = s; }

	// �������ݿ�������Ϣ
	void SendDBTableInfo(long socketID);

	void Test3GoodsImageDataBuild();
	void Test3MailGoodsDataBuild();

private:
	CEntityManager* p_DBEntityManager;
	long m_lWSSocket;

	//�ڴ�Ӱ���ļ�
	CMapFile	*m_pMapFile;
	//û�����²����ļ�¼��ջ���ļ�����
	char m_pszRunStackFile[MAX_PATH];
	char m_pszMainRunStackFile[MAX_PATH];

	char m_pszThreadRunStackFile[2048];

public:
	bool GetSaveStartFlag(void) { return m_bIsSaveStartFlag; }
	void SetSaveStartFlag(bool flag) { m_bIsSaveStartFlag = flag; }
	const char* GetStatckFileName();
	const char* GetMainStatckFileName()	{return m_pszMainRunStackFile;}

	bool m_bIsSaveStartFlag;

};// end of class


BOOL CreateGame();			// ������Ϸ
CGame* GetGame();			// ��ȡ��Ϸ
BOOL DeleteGame();			// �ͷ���Ϸ

unsigned __stdcall GameThreadFunc(void* pArguments);	// ��Ϸ���߳�
unsigned __stdcall ConnectLoginServerFunc( void* pArguments );	// ����dbserver���̡߳���dbserver�Ͽ�ʱ���ã���ͬʱ��ر�
unsigned __stdcall ConnectCostServerFunc( void* pArguments );	// 


unsigned __stdcall ProcessWriteLogDataFunc(void* pArguments);	// д����־�߳�

unsigned __stdcall LoadPlayerDataFromDB(void* pArguments);			// ������������߳�

extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�

void DoSaveData();
void DoSaveLog();

extern long g_ThreadCountID;
// ��ʱˢ����Ϣ
static long lConnects;
static long lMapAccounts;
static long lMapScripts;
static long lMapProcs;
static long lMapPlayers;
static long lPlayers;
static long lOfflinePlayers;
static long lLoginPlayers;
static long lCreatePlayers;
static long lDeletePlayers;
static long lResumePlayers;
static long lOperingLoadEntitys;
static long lOperingSaveEntitys;
static long lTeams;
static long lLargess;
static long lWriteLog;
static long lLoadPlayer; 
static long lMails;
static long lFactions;
static long lBeginLoadNum;
static long lBeginSaveNum;
static long lEndNum;