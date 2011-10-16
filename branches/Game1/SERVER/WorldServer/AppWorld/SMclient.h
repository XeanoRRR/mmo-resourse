#pragma once
class CPlayer;
typedef struct tagReportedPlayer:public tagBaseResultInfo
{
	CGUID _OptGuid;//����GUID,���ظ�SCC
}REPORTPLAYER;
class CSMClient
{
public:
	CSMClient(void);
	~CSMClient(void);
	void SendAITick(bool b = false);
	void ExitOk();
	void InitOk();
	static void	ReleaseSM();
	void	SendRefreshInfo();
	void	ReloadFileNotify(char* szfile,BYTE bSuccess);
	void	SendInfo(char* szInfo,...);
	void	ProcessMessage();
	bool	LoadMapFileSetup(CDataBlockAllocator *pDBAllocator);
	void	SendReportInfo(CPlayer* pPlayer,CPlayer* pObjPlayer,WORD wType);
	bool	BanPlayer(tagBaseResultInfo& tgInfo,CGUID& guid);
	bool	OnProcessBanPlayer(const char* name,bool bSucc=true);
	//����:����ɹ���־,���ظ�SCC�Ĳ���GUID
	void	SendBanResult(BYTE bSucc,CGUID& guid);
	void	GetBroadCastInfo(REPORTPLAYER& tgInfo,char* szInfo);
	void	SetWatchedPlayer(const CGUID& guid, const char* name, long GsSocket)
	{m_WatchedGuid=guid; strcpy(m_WatchedName,name);m_WatchedGsSocket=GsSocket;}
	CGUID&	GetWatchedGuid(){return m_WatchedGuid;}
	const	char* GetWatchedName(){if(0 != m_WatchedName[0])return m_WatchedName;return NULL;}
	long	GetWatchedGsSocket(){return m_WatchedGsSocket;}
	void	ClearWatch(void){m_WatchedGuid == NULL_GUID; memset(m_WatchedName, 0, 32); m_WatchedGsSocket=0;}
private:
	static CSMClient* g_SMInstance;
	bool m_bMapFile;
	map<string,REPORTPLAYER>mapBanPlayer;
	CGUID m_WatchedGuid;//��������ҵ�GUID
	char  m_WatchedName[32];//��������ҵ�����
	long m_WatchedGsSocket;//������������ڵ�GsSocket
public:
	static CSMClient* GetSM();
	
};
