
#pragma once
#include "../public/DBDefine.h"
#include "../../../dbaccess/MyAdoBase.h"
#include "..\appworld\player.h"
#include "..\appworld\worldregion.h"
#include "..\..\..\DBAccess\WorldDBQueue\WriteLogQueue.h"
#include "..\..\..\public\TimerQueue.h"
#include "..\..\..\public\msgtype.h"
#include "..\LogClient\ws\LogClient.h"
#include "..\AppWorld\Mail\MailManager.h"
#include "..\..\..\public\GoodsTailorSetup.h"
#include "..\..\..\public\WSUseMemManage.h"
// ȫ�ֶ���
class Account;
class GoodsExSetup;
class CGlobalRgnManager;	
class CRgnManager;
class CEntityManager;	
class LogicLogInterface;
class CWrapDataObject;
class CServer;
class CPlayer;
class CWorldRegion;
class CDBCard;
class CDBGoods;
class CDBPlayer;
class CDBLoginPlayer;
class CDBFaction;
class CDBUnion;
class CDBCountry;
class CDBEnemyFaction;
class CDBGenVar;
class CDBSetup;
class CDBRegion;
class CDBMail;
class CDBGenVarGroup;
class CFaction;
class CUnion;
class CCountry;
class CCard;
class CDupliRegionSetup;
class CDataBlockAllocator;
class CMapFile;
struct tagEnemyFaction;
struct tagVilWarSetup;
struct tagAttackCityTime;
class CVariableList;
class CMessage;
class CCostNetClient;
class CSessionFactory;
extern HWND g_hLogText;
extern HWND g_hGameServerListBox;
extern HWND g_hPlayerListBox;

void RefeashInfoText();					// ˢ�·�������Ϣ
void AddInfoText(char* msg, ...);		// ����һ����¼
void AddLogText(char* msg, ...);		// ����һ����¼
void AddErrorLogText(char* msg,...);	// ����һ�������¼
void AddPlayerList(const char *str);
void DelPlayerList(const char *str);

enum eNetFlag
{
	NF_Server_Client=0x001,
	NF_Gate_Client = 0x002,
	NF_LServer_Client=0x010,
	NF_CServer_Client=0x100,
	NF_DBServer_Client=0x1000,
	NF_LogServer_Client = 0x10000,
};
enum eServerInitSendMsgFlag
{
	Null_Server_InitSendFlag = 0,
	DB_Server_InitSendFlag = 1,
	Login_Server_InitSendFlag = 2,
	Log_Server_InitSendFlag = 4,
};

#define MAX_MSG_TIME_NUM 1024


enum eCreatePlayerPlace
{
	ePlayerQuitMsg = 1,
	ePlayerGsReconnectOne = 2,
	ePlayerGsReconnectTwo = 3,
	ePlayerCreateProc = 4,
	ePlayerProcessCreateProc = 5,
	ePlayerLoadData = 6,
	ePlayerPushSave = 7,
};

// DBS�¼���ʱ����
class CMsgTimer : public CGameEvent
{
public:
	CMsgTimer()
		:	m_ID(0),
			m_PlayerID(0),
			m_DesSocketID(0),
			m_SrcSocketID(0),
			m_dwClientIP(0),
			m_RegionID(0)
	{
		memset(m_szCDKey, 0, sizeof(m_szCDKey));
	};

	CMsgTimer(eMsgTYpe msg)
	{
		m_DBSMsgType = msg;
	};

	~CMsgTimer()
	{

	}

	//������Ϣ��Ӧ
	virtual void OnMessage(CMessage*) {};
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//����ʱ����ȡ���ĵ���
	virtual void OnTimerCancel(DWORD timerid,const void* var);
	//����ʵ�����Ϣ����
	virtual long Proc(eEventType et, const tagProcVar* tPV) { return 0; };

	//!	ע��һ���ŶӶ�ʱ���������ض�ʱ��ID
	long RegisterMsgTimerID(long lFutureTime);
	//!	ȡ��һ���Ŷӵ�Timer
	void CancelMsgTimer(void);
	//!	��Ӧ�ŶӶ�ʱ���������¼�
	void OnMsgTimeout(long lTimerID);


	// ����ID
	void SetID(long ID) { m_ID = ID; }
	// �õ�ID
	long GetID(void) { return m_ID; }

	// ����MsgType
	void SetMsgType(eMsgTYpe msg) { m_DBSMsgType = msg; }
	// �õ�MsgType
	eMsgTYpe GetMsgType(void) { return m_DBSMsgType; }


	// ����PlayerID
	void SetPlayerID(long ID) { m_PlayerID = ID; }
	// �õ�PlayerID
	long GetPlayerID(void) { return m_PlayerID; }

	// ����CDKey
	void SetCDKey(char* szCDKey, long size) 
	{ 
		memset(m_szCDKey, 0, sizeof(m_szCDKey));

		if( size < (sizeof(szCDKey)) )
			memcpy(m_szCDKey, szCDKey, size);
		else
			memcpy( m_szCDKey, szCDKey, (sizeof(m_szCDKey)) );
	}
	// �õ�CDKey
	char* GetCDKey(void) { return m_szCDKey; }

	// ����DesSocketID
	void SetDesSocketID(long ID) { m_DesSocketID = ID; }
	// �õ�DesSocketID
	long GetDesSocketID(void) { return m_DesSocketID; }
	
	// ����SrcSocketID
	void SetSrcSocketID(long ID) { m_SrcSocketID = ID; }
	// �õ�SrcSocketID
	long GetSrcSocketID(void) { return m_SrcSocketID; }

	// ����ClientIP
	void SetClientIP(DWORD value) { m_dwClientIP = value; }
	// �õ�ClientIP
	DWORD GetClientIP(void) { return m_dwClientIP; }


	// �õ�RegionID
	long GetRegionID(void) { return m_RegionID; };
	// ����RegionID
	void SetRegionID(long id) { m_RegionID = id; };

	// �õ�TimerID
	long GetTimerID(void) { return m_lTimerID; }
	// ����TimerID
	void SetTimerID(long id) { m_lTimerID = id; }
private:
	long     m_ID;
	eMsgTYpe m_DBSMsgType;
	long     m_PlayerID;
	char     m_szCDKey[64];
	long     m_DesSocketID;
	long     m_SrcSocketID;
	DWORD    m_dwClientIP;
	long     m_RegionID;
	long	 m_lTimerID;
};

typedef CTimerQueue<CMsgTimer*> MsgTimer;

//class CDBGoods;
// ��Ϸȫ�ֿ���
class CGame
{
public:
	CGame(void);
	virtual ~CGame(void);

	BOOL Init();
	//��ʼ����ջ�ļ���
	BOOL InitStackFileName();
	BOOL Release();
	BOOL MainLoop();
	BOOL AI();
	BOOL ProcessMessage();

	void OnLSLost(void);
	void OnDBSLost(void);

	void OnLOGSLost(void);


	//////////////////////////////////////////////////////////////////////////
	// ��ɫ
	//////////////////////////////////////////////////////////////////////////
public:
	size_t GetMapPlayerCount() { return m_mPlayer.size(); }

	void AppendMapPlayer(CPlayer *pPlayer);
	void DeleteMapPlayer(const CGUID& PlayerGuid);
	void ClearMapPlayer(); // use ti on uninitialize
	bool IsNameExistInMapPlayer(LPCSTR szName);

	CPlayer* GetMapPlayer(const CGUID& PlayerGuid);  //equal to IsExistPlayer() : NULL is not exist, otherwise is exist.
	CPlayer* CloneMapPlayer(const CGUID& PlayerGuid);
	CPlayer* GetMapPlayer(const char* playerName); 
	BYTE GetCreationPlayerCountInCdkey(LPCSTR szCdkey);
	bool ValidateCreatioPlayerGuidinCdkey(LPCSTR szCdkey, const CGUID& PlayerGuid);
	bool ValidatePlayerIDinCdkey(LPCSTR szCdkey, const CGUID& PlayerGuid);
	
	const CGUID& GetOnlinePlayerIDByName( LPCSTR szName);
	const CGUID& GetPlayerIDByName(LPCSTR szName);
	
	void ProcessTimeOutLoginPlayer();

	// [070725 AHC DB_INIT_LOAD]
	// �Ƿ�������жԹ�ϵ���ݱ�־
	bool b_HasQuestEnemyFactionMsgFlag;
	// �Ƿ�������������ݱ�־
	bool b_HasQuestCountryMsgFlag;
	// �Ƿ���յ�����жԹ�ϵ���ݱ�־
	bool b_HasGetEnemyFactionMsgFlag;
	// �Ƿ���յ�����������ݱ�־
	bool b_HasGetCountryMsgFlag;
	// �Ƿ����ڴ���������֤�ɹ�Player���ݱ�־
	bool b_IsValidatePlayerIDinCdkey;
	// �Ƿ��ڴ�����������֤�ɹ�Player���ݱ�־
	bool b_ValidateDBPlayerIDinCdkey;

	// ����MSGID
	inline long AllocMsgID(void);

	// �ͷ�һ��MSGID
	inline void FreeMsgID(long id);

	// ȡ�� CMsgTimer ���ݽṹ
	typedef std::map<long, CMsgTimer*> mapMsgTimer;
	typedef mapMsgTimer::iterator mapMsgTimerItr;

	// ȡ�ü�¼DBS�¼���ʱ��
	inline MsgTimer* GetTimerQueue(void) { return &m_MsgTimer; }
	
	inline mapMsgTimer& GetMsgTimerMap(void) { return m_mapMsgTimer; }

	// ȡ��MsgTimer
	inline CMsgTimer* GetMsgTimer(long id);
	
	// ȡ��һ��δʹ�õ�MsgTime����
	CMsgTimer* AllocFreeMsgTimer(void);
	// Init CMsgTimer ���ݽṹ
	void InitMsgTimerMap(void);
	// Release CMsgTimer ���ݽṹ
	void ReleaseMsgTimerMap(void);

	// ���ö�ʱ��
	void SetMsgTimer(eMsgTYpe msg, CMsgTimer* msgTime);

	void SetNoonTimer(CMsgTimer* msgTime);
	// ɾ����ʱ��
	void DelMsgTimer(CMsgTimer* msgTime);
	void DelMsgTimer(long msgID);
	
	inline std::list<long>& GetFreeMsgIDList(void) { return m_listFreeMsgID; }
	inline std::list<CMsgTimer*>& GetFreeMsgList(void) { return m_listFreeMsgTimer; }

	void InitCountryPlayerNum();
	//�õ�����������3������,ͨ��BYTE��0��5λ��ȷ��
	BYTE GetCountryLessPlayer();

	//! ����������
	void ResetStarDay(DWORD dwTime);
	//! �õ�������
	eConstellation GetStarDay(void);
private:
	//! ��ǰ������
	eConstellation m_Constellation;

private:
	// ��¼DBS�¼���ʱ��,��WS��DBS������Ϣʱ����Ϣʱ����Ϊ�жϳ�ʱ������:������ϢAddMsgTime(),���ջ�ӦDelMsgTime()
	
	MsgTimer m_MsgTimer;
	// ��ʹ�õ�CMsgTimer ���ݽṹ
	std::map<long, CMsgTimer*> m_mapMsgTimer;
	// ���е�CMsgTimer ���ݽṹ
	std::list<CMsgTimer*> m_listFreeMsgTimer;
	// ��ǰMSGID
	long m_CurMsgID;
	// ����MSGID
	std::list<long> m_listFreeMsgID;

	// ���в�����ɫ�������
	map<CGUID, CPlayer*> m_mPlayer;
	//������ҵĹ�������ͳ��
	map<BYTE,long>	m_ContryPlayerNum;

	typedef map<string,CGUID>	mapStringToGUID;
	typedef map<string,CGUID>::iterator itSG;
	mapStringToGUID m_PlayerNameToGUID;

	//�ڴ�Ӱ���ļ�
	CMapFile	*m_pMapFile;
	//û�����²����ļ�¼��ջ���ļ�����
	char m_pszRunStackFile[MAX_PATH];

	// ������Ʒ�Ķ�ջ�ļ���
	char m_pszSaveGoodsRunStackFile[MAX_PATH];
	//�ʼ�������¼���ļ���
	char m_pszOperateMailFile[MAX_PATH];

public:
	bool GenerateDBPlayerID(IN CGUID& guid);
	const CGUID& GetDBPlayerID() { return m_PlayerID; }
	void SetDBPlayerID(const CGUID& PlayerID) { m_PlayerID = PlayerID; }


	long GenerateDBLeaveWordID()	{return ++m_nLeaveWordID;}
	long GetDBLeaveWordID()	{return m_nLeaveWordID;	}
	void SetDBLeaveWordID(long nID)	{m_nLeaveWordID=nID;}

	const char* GetStatckFileName()	{return m_pszRunStackFile;}

	const char* GetSaveGoodsStatckFileName()	{return m_pszSaveGoodsRunStackFile;}

	const char* GetOpeateMailFileName()			{return m_pszOperateMailFile;}

private:
	CGUID m_PlayerID;
	long m_nLeaveWordID;
public:
	CVariableList* GetGeneralVariableList()		{return m_pGeneralVariableList;}

private:
	CVariableList	*m_pGeneralVariableList;		// ȫ�ֽű�����

	//////////////////////////////////////////////////////////////////////////
	//	���� ����������������Ҫ�����ݿ�����ݣ�
	//////////////////////////////////////////////////////////////////////////
public:
	struct tagDBData
	{
		//
		// ���ñ�������
		//
		CGUID			PlayerGuid;		// ��ɫid
		CVariableList	*pVariableList;		// ȫ�ֽű�����
		long			nLeaveWordID;		//���Ե�ID

		map<CGUID, CPlayer*> mDBPlayer;

		//��Ҫ����İ������
		list<CFaction*>		listSaveFactions;
		//��Ҫ�����ͬ������
		list<CUnion*>		listSaveUnions;

		//��Ҫɾ���İ���б�
		list<CGUID>			listDeleteFactions;
		//��Ҫɾ����ͬ���б�
		list<CGUID>			listDeleteUnions;
		//�ж԰���б�
		list<tagEnemyFaction*> listEnemyFactions;
		list<tagVilWarSetup*>	 listVillageWar;
		list<tagAttackCityTime*>  listCityWar;
		//Ҫ����ĳ��������б�
		list<CWorldRegion*> listRegionParam;

		list<CCountry*>		ltDBCountrys;
	};

public:
	int GetDBResponsedCnt() {return m_nDBResponsed;}
	void SetDBResponsedCnt(int nCnt) {m_nDBResponsed=nCnt;}

	tagDBData* GetDBData() {return &m_stDBData;}

	// ������Ҫ���̵����ݵ�SaveData
	void GenerateDBData();
	void GeterateRegionDBData();
	
	bool IsNameExistInDBData(LPCSTR szName);

	//�Ͽ��������
	void KickAllPlayer();

private:
	tagDBData m_stDBData;				// ��������(ÿ�����̵�����һ�Σ����̽������ͷ�)
	int	m_nDBResponsed;			// �յ���GameServer���̻ظ����ݼ�����


	map<long,long>	m_mapMsgVirMemUseSize;

	/////////////////////////////////////////////////////////////////////////
	// �����������
	/////////////////////////////////////////////////////////////////////////
public:
	// ����������Ϣ
	struct tagRegion
	{
		CWorldRegion	*pRegion;			// ����ָ��
		DWORD			dwGameServerIndex;	// ����Gameserver���
		eRWT			RegionType;			// ��������
	};	
public:
	// GameServer��Ϣ
	struct tagGameServer
	{
		bool			bConnected;			// �÷������Ƿ��Ѿ�������
		DWORD			dwIndex;			// ���������
		string			strIP;				// IP
		DWORD			dwPort;				// �˿�

		long			lReceivedPlayerData;	// ���յ��������������(��ʱ��)
		long			lPlayerNum;			// �������
	};

public:
	VOID	ReturnGsPlayerNum(VOID);
	tagGameServer* GetGameServer(DWORD dwIndex)	{return (s_mapGameServer.find(dwIndex)!=s_mapGameServer.end())?&s_mapGameServer[dwIndex]:NULL;}
	tagGameServer* GetGameServer(const char* strIP, DWORD dwPort);			// ��ȡGameServer������	
	tagGameServer* GetPlayerGameServer(const CGUID& PlayerGuid);			// ��ȡplayer���ڵ�GameServer
	//�õ��Ǹ����ĳ���ID��GameServer
	tagGameServer* GetRegionGameServer(long RegionID);						// ��ȡregion���ڵ�GameServer
	tagGameServer* GetRegionGameServer(const CGUID& RegionID);				// ��ȡregion���ڵ�GameServer

	long GetGameServerNumber_ByRegionID(long RegionGUID);
	long GetGameServerNumber_ByRegionGUID(const CGUID& RegionGuid);

	long GetGameServerNumber_ByPlayerID(const CGUID& PlayerGuid);


	bool LoadServerSetup(const char *filename);								// ��ȡ��Ϸ����������
	//! ��ȡ�Ǹ�ְҵ�������ã�����ӿ�Ӧ������Ʒ������ɺ��ٵ���
	bool LoadGoodsTailorSetup(const char *filename);
	long GetConnectedGameServerCount();										// �����Ѿ������ϵ�GameServer����
	
	bool OnGameServerClose(DWORD dwIndex);									//��ĳ��GameServer�Ͽ����ӵ�ʱ��
	bool OnGameServerLost(DWORD dwIndex);									// ��ĳ��GameServer�Ͽ�ʱ�Ĵ���

	long GetGSNum(void) { return s_mapGameServer.size(); }
	map<DWORD, tagGameServer>* GetGameServerList(){return &s_mapGameServer;}

private:
	map<DWORD, tagGameServer>		s_mapGameServer;	// �������б�<dwIndex, tagGameServer>
public:
	// ��ȡLoadLoginGMList.dat
	bool LoadLoginGMList(const char* szName);
	vector<string>& GetLoginGMListVec(void) { return m_vecLoginGMList; }
	bool CheckCdkeyIsExsitInLoginGMList(const char* szCdkey);
	// �ж�����Ƿ���GM
	bool PlayerIsGM(CPlayer* pPlayer); 
	private:
		vector<string> m_vecLoginGMList;
	//////////////////////////////////////////////////////////////////////////
public:
	VOID   CreateConnectLoginThread();
	void CreateConnectDBThread();
	void CreateConnectLogThread(void);
	void CreateConnectCostThread();
	bool ReConnectLoginServer();											// ��������LoginServer
	bool ReConnectDBServer();
	bool ReConnectLogServer();
	bool ReConnectCostServer();                                             
	void SendCdkeyToLoginServer(BOOL bIsReady);											// ���͵�ǰWorldServer�ϵ�����Cdkey��LoginServer
	void SendMsg2GameServer(DWORD dwIndex,CMessage* pMsg);					// ����һ����Ϣ��ָ��Index��GameServer
	//! [2007-7-9 add]
	void SendAllGameServer(CMessage* pMsg);					// ����һ����Ϣ������GameServer
	//! [2007-7-9 add end]

	bool CheckInvalidString(string& str,bool bReplace=false);				// ���Ƿ��ַ���

	// ��player���ϻ�ȡװ����Ϣ
	void GetPlayerEquipID(CPlayer* pPlayer,
		DWORD& dwHelm, DWORD& dwBody, DWORD& dwGlov, DWORD& dwBoot, DWORD& dwWeapon, DWORD& dwBack,
		DWORD& dwHeadgear, DWORD& dwFrock, DWORD& dwWing, DWORD& dwManteau, DWORD& dwFairy,
		BYTE& nHelmLevel, BYTE& nBodyLevel, BYTE& nGlovLevel, BYTE& nBootLevel, BYTE& nWeaponLevel, BYTE& nBackLevel,
		BYTE& nHeadgearLevel, BYTE& nFrockLevel, BYTE& nWingLevel, BYTE& nManteauLevel, BYTE& nFairyLevel);

	void AddOrginGoodsToPlayer(CPlayer* pPlayer);							// Ϊ�����ӳ�ʼ��Ʒװ��
	void InitSkillToPlayer(CPlayer* pPlayer);								//Ϊ��ҳ�ʼ������

	BOOL ReLoad(const char* name, bool bSendToGameServer=false,bool bReloadFileInfo = false);			// �ض�����

	// Fox@20090105 ��չ�����ö�ȡ
	bool LoadEx(unsigned uFlag,			// ��������
		long lSocketID = 0,				// -1:��������GS, 0:��������GS, other:����.
		bool bReload = true);			// �Ƿ��ض�����

	DWORD GetAITick()							{return s_lAITick;}
	void AddAITick()							{s_lAITick++;}

	// ��Ӧ��Ҳ���
	bool	OnPlayerEnter(CPlayer* pPlayer, Account* pAcc, long lSignCode, long lIP,  long gsid, long lSocketIDC2S, long lMsgSocketID);
	//!		����ұ��뷢�ͺ���Ӧ
	bool	OnPlayerEnterLateCode(CPlayer* pPlayer, Account* pAcc);
	bool	OnPlayerQuit(CPlayer* pPlayer, long gsid, DWORD dwServerKey[], bool bIsChangingGs=false);
private:
	DWORD	s_lAITick;														// ��ϷAI�����������ڹ����еĴ���
	DWORD	m_dwLastReLoginTime;											// �ϴ�����LoginServer��ʱ��
	//////////////////////////////////////////////////////////////////////////
	//	���粿��
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL InitNetServer(void);					//��ʼ����Ϸ����������ˣ���������������ˣ�װ������
	BOOL StartConnLS(void);						//����LoginServer
	bool StartConnCS();							//��ʼ���ӼƷѷ�����
	bool StartAccept();							//��ʼ��Ϊ����������

	VOID BeginPingDS(VOID){m_bPinging = FALSE;}
	VOID EndPingDS(VOID){m_bPinging = TRUE;}
	BOOL FinishPingDS(VOID){return m_bPinging;}

	//������DBServer�����ӣ���Ϊ�ͻ��˳�ʼ��
	BOOL StartConnDBS(void);
	//������LogServer�����ӣ���Ϊ�ͻ��˳�ʼ��
	bool StartConnLogS(void);
	
	void SetInitVarLoadFlag(bool flag)
	{
		m_bInitVarLoadFlag = flag;
	}
	bool GetInitVarLoadFlag(void)
	{
		return m_bInitVarLoadFlag;
	}
	void SetInitRegionLoadFlag(bool flag)
	{
		m_bInitRegionLoadFlag = flag;
	}
	bool GetInitRegionLoadFlag(void)
	{
		return m_bInitRegionLoadFlag;
	}
	void SetInitCountryLoadFlag(bool flag)
	{
		m_bInitCountryLoadFlag = flag;
	}
	bool GetInitCountryLoadFlag(void)
	{
		return m_bInitCountryLoadFlag;
	}
	void SetInitFactionLoadFlag(bool flag)
	{
		m_bInitFactionLoadFlag = flag;
	}
	bool GetInitFactionLoadFlag(void)
	{
		return m_bInitFactionLoadFlag;
	}
	void SetInitUnionLoadFlag(bool flag)
	{
		m_bInitUnionLoadFlag = flag;
	}
	bool GetInitUnionLoadFlag(void)
	{
		return m_bInitUnionLoadFlag;
	}
	void SetInitPersonalHouseLoadFlag(bool flag)
	{
		m_bInitPersonalHouseLoadFlag = flag;
	}
	bool GetInitPersonalHouseLoadFlag(void)
	{
		return m_bInitPersonalHouseLoadFlag;
	}
	void SetRestartSessionFlag(bool flag)
	{
		m_bRestartSessionFlag = flag;
	}
	bool GetRestartSessionFlag(void)
	{
		return m_bRestartSessionFlag;
	}

	void SetKickPlayerFlag(bool flag) { m_bIsKickPlayerFlag = flag; }
	bool GetKickPlayerFlag(void) { return m_bIsKickPlayerFlag; }

	CServer* GetNetServer()	{return s_pNetServer;}

	// get dbsocketID
	long GetDBSocketID(void) { return m_lDBSocket; }
	// get LSsocketID
	long GetLSSocketID(void) { return m_lLogingSocket; }

	bool GetDbIsReady(void) { return m_bDbReady; }
	void SetDbIsReady(bool flag) { m_bDbReady = flag; }

	void SetServerInitSendFlag(eServerInitSendMsgFlag flag);
	void ProcessServerInitSendMsg();
private:
	CDataBlockAllocator	*m_pDBAllocator;
	CServer				*s_pNetServer;			// ��ΪGameServer�ķ�������
	long				m_lLogingSocket;		//����loginserver�Ŀͻ���
	long				m_lDBSocket;			//����dbserver�Ŀͻ���
	long				m_lCostSocket;			//����costserver�Ŀͻ���
	bool				m_bInitVarLoadFlag;
	bool				m_bInitCountryLoadFlag;
	bool				m_bInitRegionLoadFlag;
	bool				m_bInitFactionLoadFlag;
	bool				m_bInitUnionLoadFlag;
	bool				m_bRestartSessionFlag;
	bool				m_bInitPersonalHouseLoadFlag;
	bool				m_bDbReady;
	long				m_ServerInitSendFlag;
	CRITICAL_SECTION	m_CSServerInitSendFlag;
	bool				m_bIsKickPlayerFlag;
	BOOL				m_bPinging;


	/////////////////////////////////////////////////////////////////////////
	// �ű�����
	/////////////////////////////////////////////////////////////////////////
public:
	bool LoadScriptFileData(const char* dir_name, const char* function_filename, const char* var_filename, const char* general_var_filename);	// ��ȡָ��Ŀ¼�е����нű��ļ�(������Ŀ¼)
	bool LoadOneScript(const char* filename);		// ��ȡһ���ű��ļ�
	map<string, char*>* GetScriptFileDataMap()			{return &m_mapScript_FileData;}

	const char* GetFuncitonList_FileData()				{return m_strFunctionList_FileData;}
	const char* GetVariableList_FileData()				{return m_strVariableList_FileData;}
	long GetFuncitonList_FileData_Size()			{return m_lFunctionList_FileData_Size;}
	long GetVariableList_FileData_Size()			{return m_lVariableList_FileData_Size;}

	const char* GetScriptFileData(const char* filename)	
	{
		if(m_mapScript_FileData.find(filename)!=m_mapScript_FileData.end()) 
			return m_mapScript_FileData[filename];
		return NULL;
	}

	bool ReLoadOneScript(const char* filename);
	bool ReLoadOneRegionSetup(long lRegionID);
	bool ReLoadAllRegionSetup();
	bool ReLoadAllRegion();

private:
	char*	m_strFunctionList_FileData;				// �ű������б��ļ�����
	char*	m_strVariableList_FileData;				// �ű������б��ļ�����
	long	m_lFunctionList_FileData_Size;			// �ű������б��ļ����ݳ���
	long	m_lVariableList_FileData_Size;			// �ű������б��ļ����ݳ���
	map<string, char*>		m_mapScript_FileData;	// ���нű��ļ�������map<�ļ���, �ļ�����>


	/////////////////////////////////////////////////////////////////////////
	// ��Ϸ����
	/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		string	strDBIP;			// DBServerIP
		DWORD	dwDBPort;			// DBServerPort

		long	lUseLogServer;		// �Ƿ�ʹ��LogServer��0=��ʹ�ã�1=ʹ��
		string	strLogIP;			// LogServerIP
		DWORD	dwLogPort;			// LogServerPort
		string  strErrFile;			// �����ļ�·��+�ļ���
		string  strLogFile;			// ������־�ļ�·��+�ļ�����SQL��ʽ��
		string  strTxtLogFile;		// ������־�ļ�·��+�ļ��������ĸ�ʽ��
		DWORD	dClientNo;			// �ͻ��˱��

		DWORD	dwNumber;			// ���������(0-7)
		string	strName;			// ����������
		string	strLoginIP;			// LoginServerIP
		long	dwLoginPort;		// LoginServerPort
		DWORD	dwListenPort;		// ����Port
		string	strLocalIP;			// ����IP
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

		DWORD	dwReleaseLoginPlayerTime;			// �ͷ�LoginList���Player�ĳ�ʱʱ��

		DWORD	dwClearPlayerDataTime;
		
		DWORD	dwGappID;				//�ڴ�ӳ��ͻ���ID
		tagSetup()
		{
			dwRefeashInfoTime = 1000;
			dwSaveInfoTime = 60000;
		}
	};

private:
	tagSetup	m_Setup;
	CDupliRegionSetup	*m_pDupliRegionSetup;

	//!					��չ��Ʒ��������
	GoodsExSetup		*m_pGoodsExSetup;
	
	
public:
	bool		LoadSetup();
	bool		LoadMapFileSetup();
	bool		LoadServerResource();
	bool		UnloadServerResource();

	//!			���л���Ʒ��չ����
	bool		SerializeGoodsExSetup(vector<BYTE> &vecData);

	tagSetup*	GetSetup()	{return &m_Setup;}
	CDupliRegionSetup* GetDupliRegionSetup()	{return m_pDupliRegionSetup;}

	//////////////////////////////////////////////////////////////////////////
	// ������
	//////////////////////////////////////////////////////////////////////////
public:
	long GetNumTeams();
	
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
	//	��Ʒ����
	//////////////////////////////////////////////////////////////////////////
public:
	//��Ʒ�������ݽṹ
	struct tagGoodsLink
	{
		bool bChange;	//��Ʒ�����Ƿ�仯
		CGoods* pGoods;
		DWORD dwIndex;	//��Ʒ���ͱ�ţ������Ե���ʹ�ø��ֶ�)
		DWORD dwType;
		BYTE lNum;		//��Ʒ����
	};

	list<tagGoodsLink>* GetGoodsLinkList()	{return &m_listGoodsLink;}
	void AddGoodsLink(tagGoodsLink* pstGoodsLink);	//���һ����Ʒ����
	tagGoodsLink* FindGoodsLink(DWORD dwID);        //����IDѰ��һ����Ʒ����

private:
	list<tagGoodsLink> m_listGoodsLink;				//����ύ����Ʒ������Ŵ�

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

	void AddPingGameServerInfoDB(tagPingGameServerInfo& st)	{m_vectorPingGameServerInfoDB.push_back(st);}
	void ClearPingGameServerInfoDB()						{m_vectorPingGameServerInfoDB.clear();}	

	void AddPingGameServerInfo(tagPingGameServerInfo& st)	{m_vectorPingGameServerInfo.push_back(st);}
	void ClearPingGameServerInfo()							{m_vectorPingGameServerInfo.clear();}
	LONG GetPingGsNum(VOID)									{return m_vectorPingGameServerInfo.size();}

	void SetInPing(bool b)									{m_bInPing=b;}
	void SetLastPingGameServerTime(DWORD dw)				{m_dwLastPingGameServerTime=dw;}

private:

	vector<tagPingGameServerInfo>	m_vectorPingGameServerInfoDB;
	vector<tagPingGameServerInfo>	m_vectorPingGameServerInfo;
	bool	m_bInPing;							// �Ƿ���Ping�Ĺ�����
	DWORD	m_dwLastPingGameServerTime;			// �ϴη���ping��ʱ��

	//////////////////////////////////////////////////////////////////////////
	// �첽�����ݿ��ȡ�����Ϣ
	//////////////////////////////////////////////////////////////////////////
public:

	// ���մ�DB��ȡ��Player������ݳ�ʼ��
	void InitPlayerInfo(CPlayer* pPlayer, char* szCdkey);


	///////////////////////////////
	// ��־����
	///////////////////////////////

public:
	void DoSaveData(bool isSavePlayerFlag);
	// ����SendSetup
	void SendSetup(void);
	// ����MSG_W2DB_SERVER_UNREGENTITY ���DBS����Ķ���
	void SendUnRegDBEntity(const char* szCdkey);

	// ���Ͳ�ѯplayer��CDKEY��Ϣ
	void SendGMBanPlayerToDBSMsg(char* strPlayerName, long time);

	// decode MSG_S2W_PLAYER_DETAIL_SAVE_PROPERTY ��Ϣ
	bool DecodeSavePropertyMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveEquipmentGoodsMsg(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSavePackGoodsMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveSubPackGoodsMsg	(long subPackPos, CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveVarListMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveQuestListMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeDepotGoldMsg			(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeDepotSilverMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeDepotGoodsMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveSubDepotGoodsMsg (long subPackPos, CPlayer* pPlayer, DBReadSet& setReadDB);
	//ٸ����������
	bool DecodeSaveMerQuestListMsg	(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveSkillListMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveStateListMsg		(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveDeputyOccuListMsg(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveWalletGoodsMsg	(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveSivlerGoodsMsg	(CPlayer* pPlayer, DBReadSet& setReadDB);
	bool DecodeSaveSpriteSystemMsg	(CPlayer* pPlayer, DBReadSet& setReadDB);	
	
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
	LogicLogInterface*	GetLogicLogInterface(void);

public:
	LogClient			&GetLogClient(void){return m_LogClient;}
private:

	LogClient			m_LogClient;
	LogicLogInterface	*m_pLogicLogInterface;

	/*
	
	*/
public:
	CGlobalRgnManager*	GetGlobalRgnManager(void);
	CRgnManager*		GetRgnManager(void);
	CEntityManager*		GetEntityManager(void);
	CMailSystem*		GetMailManager(void);

	// ��GS����WSʱ��ʼ�����˷��ݸ���
	void InitPersonalHouseDataOnGS(long gsid);

	// ����ʱ�䴴����֤��ֵ
	void GenerateSignCodeByTime(long& lRetCodeValue);

	void SendSaveGenVarGroupMsg(CDBGenVarGroup* pGenVarGroup);

private:
	// ȫ�ֶ���
	CGlobalRgnManager*	g_GlobalRgnManager;
	CRgnManager*		g_RgnManager;
	CEntityManager*		g_EntityManager;

	CMailSystem		*g_pMailManager;

	bool				b_IsFirstConn;
// Fox@20080922----------------------------------------------
public:
	bool ConnectGate(bool bRec = false);		// ����GateServer
	void OnGateLost(void);						// GateServer�Ͽ�;
	void CreateGateThread(void);				// �Ͽ��󴴽������߳�
	int GetGateSocketID(void) { return m_nGateSocket; }

private:
	int					m_nGateSocket;			// ����GateServer��SocketID

public:
	void PushSavePlayer(CPlayer* pPlayer);
	CPlayer* FindSavePlayer(const CGUID& guid);
	std::map<CGUID, CPlayer*>&	GetSavePlayerMap(void) { return m_mapSavePlayers; }
	void ProcessDelaySavePlayer(void);
	void SavePlayer(CPlayer *pPlayer);
	static CSessionFactory* GetSessionFactoryInst(void);

	void AddCreatePlayerLog(eCreatePlayerPlace place, CPlayer* player);
	void DelPlayerLog(CPlayer* player);
	void OutputCreatePlayerInfo(void);
	CPlayer* NewRawPlayer(eCreatePlayerPlace place);
	void DelRawPlayer(CPlayer* player);

	//�����Ϣ�������ͺ�ʱ��
	inline void AddMsgExpendTime(long lType,long lTime)
	{
		//if(GetSetup()->bWatchRunTimeInfo)
		m_mapMsgExpendTime[lType] += lTime;
	}
	inline void AddTimerExpendTime(long lType,long lTime)
	{
		m_mapTimerExpendTime[lType] += lTime;
	}
	//��Ӹ���ȫ�ֱ�������ͳ��
	inline void AddVarScriptExpendTime(const char* pszVarName,long lTick)
	{
		if (NULL != pszVarName)
			m_mapVarScriptExpendTime[string(pszVarName)] += lTick;
	}
	//�������ͳ����Ϣ
	void OutputMsgExpendTimeInfo(const char* pszFileName);
private:
	//��Ϣ����ͳ��
	map<long,long>	m_mapMsgExpendTime;
	//��ʱ������ͳ��
	map<long,long>	m_mapTimerExpendTime;
	//����ȫ�ֽű�����ʱ������ͳ��
	map<string,long> m_mapVarScriptExpendTime;
private:
	std::map<CGUID, CPlayer*> m_mapSavePlayers;
	static CSessionFactory* m_pSessionFactory;

	std::map<eCreatePlayerPlace, set<CPlayer*>> m_mapCreatePlayerLog;
};// end of class


BOOL CreateGame();						// ������Ϸ
CGame* GetGame();						// ��ȡ��Ϸ
LogicLogInterface*	GetGameLogInterface();			// ��ȡ��Ϸ��־�߼��ӿڶ���
BOOL DeleteGame();						// �ͷ���Ϸ

unsigned __stdcall GameThreadFunc(void* pArguments);	// ��Ϸ���߳�
unsigned __stdcall ConnectLoginServerFunc( void* pArguments );	// ����WorldServer���̡߳���WorldServer�Ͽ�ʱ���ã���ͬʱ��ر�
unsigned __stdcall ConnectCostServerFunc( void* pArguments );	// 
unsigned __stdcall ConnectDBServerFunc( void* pArguments );	// 
unsigned __stdcall ConnectLogServerFunc( void* pArguments );	// 
unsigned __stdcall ConnectGateFunc(void* pArg);				// ����GateServer���߳�, �Ͽ�ʱ����, ��ͨʱ�ر�

unsigned __stdcall LoadPlayerDataFromDB(void* pArguments);			// ������������߳�

extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�

// ��ʾ������Ϣ����
extern bool g_bShowSaveInfo;

void ShowSaveInfo(LPCSTR szInfo, ...);
void DoSaveLog();