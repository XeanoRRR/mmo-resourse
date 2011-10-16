#pragma once

#include "..\..\..\LogClient\bs\LogBSClient.h"

class CServer;
class CMessage;
class CRsAccount;
class CRsLogInfo;
class CMyAdoBase;
class CMySocket;


class CDataBlockAllocator;
class CBillDBManager;
extern HWND g_hLogText;
extern HWND g_hWorldListBox;
extern HWND g_hCdkeyListBox;

void RefeashInfoText();					// ˢ�·�������Ϣ
void AddLogText(char* msg, ...);		// ����һ����¼
void SaveLogText(bool bForce);			// �����¼


enum eNetFlag
{
	NF_Server_Client=0x01,
	NF_Server_World=0x10,
	NF_Log_Client = 0x100,
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

	BOOL InitNetServer_GS(void);				//��ʼ�����WorldServer������������ˣ���������������ˣ�װ������
	BOOL AI();
	BOOL ProcessMessage();

	void	CreateConnectLogThread(void);

	

/////////////////////////////////////////////////////////////////////////
// ��Ϸȫ�ֽӿ�
/////////////////////////////////////////////////////////////////////////
public:
	struct tagCdkey
	{
		string	strCdkey;			// cdkey
		long	lWorldNumber;		// ����World���
	};

	// ��������

	CRsLogInfo *m_pRsLogInfo;						// ����д���¼��־ logInfo ��
private:
	DWORD	s_lAITick;								// ��ϷAI�����������ڹ����еĴ���

    CRsAccount *m_pRsAccount;							
	
	std::map<string,int>			m_mapPWError;			// �������������ʺ�
	
	long					m_StateLvl[4];
	

	//���ݿ������
	CDataBlockAllocator	*m_pDBAllocator;

	CBillDBManager* m_pBillDBManager;
	
	//�������GameServer��Ϣ���������
	CServer	*s_pNetServer_GS;
	// ��ͨ��QUEST_PLAYER_DATA�����
	map<string, long> m_PassDetailDataCdkey;


public:
	map<string, long/*GSID*/>& GetPassDetailDataCdkeyMap(void) { return m_PassDetailDataCdkey; }	

	DWORD GetAITick()							{return s_lAITick;}
	void AddAITick()							{s_lAITick++;}
	
	CServer* GetNetServer_GS()	{return s_pNetServer_GS;}
	CRsAccount* GetRsAccount() {return m_pRsAccount;}
	

	bool KickOut(const char *szCdkey);			// �߳�һ��CDKEY
	std::map<string,int>* GetPWErrorMap()		{return &m_mapPWError;}

	CBillDBManager* GetBillDBManager() {return m_pBillDBManager;}

	
	
/////////////////////////////////////////////////////////////////////////
// �̹߳���
/////////////////////////////////////////////////////////////////////////
public:
	HANDLE m_hEndEvent[2];				// �����¼�
	bool m_bExit;						// �˳���Ϸ

/////////////////////////////////////////////////////////////////////////
// WorldServer��Ϣ�б�
/////////////////////////////////////////////////////////////////////////
public:
	struct tagGameServerInfo
	{
		bool bConnected;	//�Ƿ�����
		DWORD dwIndex;		//���
		string	strName;	//GameServer����
		string  strIp;		//GS IP
		DWORD dwPort;		//GS PORT
		long	lStateLvl;	// ͳ�Ʒ�������״̬0:ά����1:0-75%,2:75-90%,3:90%-100%

		tagGameServerInfo ()
		{
			bConnected=false;
			dwIndex=0;
			strName="";
			strIp="";
			dwPort=0;
			lStateLvl=0;
		}
	};

private:
	map<long,tagGameServerInfo>		m_listGSInfo;

	map<long,tagGameServerInfo>		m_GSInfoSetup;//�����ļ��е�WorldServer��Ϣ

	LogBSClient			m_LogBSClient;
public:
	//���ļ���ȡGameServer����
	bool LoadGSSetup();

	void SetListWorldInfoBySetup();

	void OnLOGSLost();

	bool IsExitWorld(const char* strWorldName);
	long GetWorldNum()					{return (long)m_listGSInfo.size();}

	void AddWorldInfoToMsg(CMessage& msg,string &strCdkey);

	tagGameServerInfo* GetGameServer(const char* strIP, DWORD dwPort);	

	long AddGS(long lGSNumber,const char* strGSName);			//���GAMESERVER
	long DelGS(long lNum);	// ɾ��GAMESERVER

	bool WorldServerIsOpenState(long lNum);

	long GetWorldSateLvl(long lPlayerNum);
	void OnChangeStateLvl(long lWorldNum,long lPlayerNum);
	void ChangeAllWorldSate();
	const char *GetGSNameByID(long lNum);
	long  GetGSIDByName(const char* strWorldName);
	bool SendMsg2World(CMessage* pMsg,long lWorldServerID);	//������Ϣ��ָ����WORLDSERVER	

	void UpdateDisplayWorldInfo();

	void OnNetGSClose(long lGSID);

	//!		��ӦDB��Ϣ
	void	InitLogClient(CMessage *pMsg);
	//!		��Ӧȷ����Ϣ
	void	LogServerAffirm(CMessage *pMsg);
	//!		��Ӧɸѡ������Ϣ
	void	LogServerCondition(CMessage *pMsg);

	LogBSClient&			GetLogBSClient(void){return m_LogBSClient;}

	BOOL ConnectLogServer();

	bool ReConnectLogServer();

/////////////////////////////////////////////////////////////////////////
// ��Ϸ����
/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		DWORD	dwListenPort_GameServer;	//����GameServer�Ķ˿ں�		
		string	strSqlConType;				//���ݿ���������
		string	strSqlServerIP;				//���ݿ� IP��ַ
		string  strSqlUserName;				//���ݿ������û���
		string  strSqlPassWord;				//���ݿ���������
		string  strDBName;					//���ݿ�DB��
		bool	bUseLogServer;
		string	strLogIP;
		DWORD   dwLogPort;
		DWORD   dwLogClientNo;
		DWORD	dwWorkerThreadNum;			//�������߳�����
		bool	bUseSystemRollback;			//�Ƿ�ʹ��ϵͳ�ع�
		DWORD   dwSystemRollbackTime;		//ϵͳ�ع�ʱ��
		DWORD	dwRollbackLockTime;			//�ع�����ʱ��

		//Client����������ò���
		DWORD	dwDataBlockNum;			//���ݿ�����
		DWORD	dwDataBlockSize;		//���ݿ��С
		DWORD	dwFreeMsgNum;			//Ԥ�������Ϣ����

		//GS����������ò���
		DWORD	dwGSFreeSockOperNum;	// �����������Ԥ��������
		DWORD	dwGSFreeIOOperNum;	// ��ɶ˿���IO����Ԥ��������
		long	lGSIOOperDataBufNum;	// Ĭ��IO������DataBuf����
		bool	bGSCheckNet;			// �Ƿ��������м��
		DWORD	dwGSBanIPTime;		// ��ֹIP��ʱ��
		DWORD	dwGSMaxMsgLen;		// ������������Ϣ����		
		long	lGSMaxConnectNum;		// �ͻ��˵������������
		long	lGSMaxClientsNum;		// ���Ŀͻ��˷��ͻ�������С
		long	lGSPendingWrBufNum;		// ���ķ���IO����Buf�ܴ�С
		long	lGSPendingRdBufNum;		// ���Ľ���IO����Buf�ܴ�С
		long	lGSMaxSendSizePerSecond;	// ��ͻ���ÿ�뷢�͵�����ֽ���
		long	lGSMaxRecvSizePerSecond;	// �ӿͻ��˽��ܵ�ÿ������ֽ���
		long	lGSMaxBlockedSendMsgNum;	// ��������ķ�����Ϣ����
		long	lGSConPendingWrBufNum;	// �ͻ������ķ���IO����Buf�ܴ�С
		long	lGSConPendingRdBufNum;	// �ͻ������Ľ���IO����Buf�ܴ�С
		long	lGSConMaxSendSizePerSecond;//����������͵�ÿ������ֽ���
		long	lGSConMaxRecvSizePerSecond;//�ӷ��������ܵ�ÿ������ֽ���
		long	lGSConMaxBlockedSendMsgNum;	// ��������ķ�����Ϣ����

		DWORD	dwRefeashInfoTime;		// ˢ����ʾʱ��
		DWORD	dwSaveInfoTime;			// ����һ����Ϣ��ʱ����
		DWORD	dwDoQueueInter;			// ����ȴ����еļ��ʱ��
		DWORD	dwSendMsgToQueInter;	// ���ȴ����з�����Ϣ�ļ��ʱ��		

		DWORD	dwGSMaxPlayers;		// ��½WorldServer���������
		float	fGSBusyScale;		// ��æ����������
		float	fGSFullScale;		// ��������������

		DWORD	dwPingGSTime;	// ��ʱPingWorldServer�ļ��
		DWORD	dwPingGSErrorTime;	// PingWorldServer���յ����صĳ���ʱ����

		bool	bCheckForbidIP;			// �Ƿ������IP����
		bool	bCheckAllowIP;			// �Ƿ���IP������
		bool	bCheckBetweenIP;		// ���ip�Ƿ�������IP��

		tagSetup()
		{			
		}
	};

	
private:
	tagSetup	m_Setup;

public:
	bool		LoadSetup();
	bool		ReLoadSetup();
	bool		ReLoadWorldSetup();
	tagSetup*	GetSetup()	{return &m_Setup;}	

//////////////////////////////////////////////////////////////////////////
//	��ʱping
//////////////////////////////////////////////////////////////////////////
public:

	// WorldServer��Ϣ
	struct tagPingWorldServerInfo
	{
		// GameServer��Ϣ
		struct tagPingGameServerInfo
		{
			string	strIP;
			DWORD	dwPort;
			DWORD	dwPlayerNum;
		};

		string	strIP;
		DWORD	dwPort;
		DWORD	dwPlayerNum;
		vector<tagPingGameServerInfo>	vectorGameServerInfo;
	};

	void AddPingWorldServerInfo(tagPingWorldServerInfo& st)	{m_vectorPingWorldServerInfo.push_back(st);}

private:
	vector<tagPingWorldServerInfo>	m_vectorPingWorldServerInfo;

public:	
	int		EnterGame( const char *szCdkey, DWORD dwIP, long lSockID, const char *szWSName, bool bLimit=false, bool bMobileUser=false );
};

BOOL CreateGame();			// ������Ϸ
CGame* GetGame();			// ��ȡ��Ϸ
BOOL DeleteGame();			// �ͷ���Ϸ

unsigned __stdcall GameThreadFunc(void* pArguments);	// ��Ϸ���߳�

extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�


//////////////////////////////////////////////////////////////////////////
// ������״̬��־����
//////////////////////////////////////////////////////////////////////////
void AppendServerInfoLog(vector<CGame::tagPingWorldServerInfo> &vWorldServerInfo);
extern CRITICAL_SECTION g_csServerInfoLog;
extern HANDLE g_hServerInfoLog;

extern vector<CGame::tagPingWorldServerInfo> g_vWorldServerInfo;
//extern _ConnectionPtr g_SerInfoLogConn;
//////////////////////////////////////////////////////////////////////////


unsigned __stdcall UpdateOnlineUser2DB(void *param);

unsigned __stdcall		ConnectLogServerFunc( void* pArguments );