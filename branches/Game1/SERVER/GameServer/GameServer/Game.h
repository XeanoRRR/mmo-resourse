
#pragma warning(disable:4996)
#pragma warning(disable:4018;disable:4244;disable:4267;disable:4311;disable:4312)
#pragma once

#include "../../../public/BaseDef.h"
#include"../../../public/PlayerDef.h"
#include "../../../public/GameEvent.h"
#include "..\..\..\public\TimerQueue.h"
#include "..\..\..\public\msgtype.h"
#include "../../../public/StrType.h"
#include "..\..\..\public\BounsDef.h"
#include "..\..\..\LogClient\gs\LogClient.h"
#include "../AppServer/RgnDef.h"
#include <set>
#include "..\..\..\public\GoodsTailorSetup.h"
#include "../../sdoa4serverdll/include/sdoa4server.h"
#include "../../sdoa4serverdll/include/sndabase.h"
#include "..\..\Public\UseMemManage.h"
#include "../../../nets/MsgQueue.h"
//#define  _SCRIPT_INFO_

class LogicLogInterface;
class CBaseObject;
class CMoveShape;
class CServer;
class CMyDataBase;
class CPlayer;
class CGoods;
class CMonster;//���monster�������
class CServerRegion;
class CProxyServerRegion;
class CMyServerClient;
class CMySocket;
extern HWND g_hLogText;
extern HWND g_hRegionListBox;
extern HWND g_hPlayerListBox;
class CSequenceString;
class CDupliRegionSetup;
class CGUID;
class CDataBlockAllocator;
class GameUnion;
class GameFaction;
class CDBPlayer;
class CDBUnion;
class CDBFaction;

class CBillClient;
class CMail;

void AddLogText(char* msg, ...);		// ����һ����¼
void AddErrorLogText(char* msg, ...);	// ����һ�������¼
void SaveLogText(bool bForce);			// �����¼
void AddPlayerList(const char *str);
void DelPlayerList(const char *str);
void AddRegionList(const char *str);
void DelRegionList(const char *str);

void OutMemInfo(const char *pFileName, const char *pTxt);

enum eNetFlag
{
	NF_Server_Client			=	0x001,
	NF_WServer_Client			=	0x010,
	NF_DBServer_Client			=	0x100,// [AHC 080809 DBSMSG]
	NF_BillingServer_Client		=	0x1000,
	NF_LogServer_Client = 0x10000,
	NF_BounsServer_Client		=	0x100000,
};
enum eServerInitSendMsgFlag
{
	Null_Server_InitSendFlag = 0,
	World_Server_InitSendFlag = 1,
	Bill_Server_InitSendFlag = 2,
	Log_Server_InitSendFlag = 4,
};
#define MAX_MSG_TIME_NUM 1024

//�Ʒ�����ѡ�� 
enum eBillConnectOption
{
	BILL_CONNECT_NONE,	//����
	BILL_CONNECT_XM,	//������
	BILL_CONNECT_SNDA	//��ʢ��
};

typedef struct tagPlayerChangeRegion
{
	long lType;
	long ltemplateID; // ��ͨ������ģ��ID���Բ��ҵ�GUID
	CGUID RegionID;
	long lX;
	long lY;
	long lDir;
	long lUseGoods;
	long lRange;
}tagPCR;

// ��¼�ű��ļ�������Ϣ
struct tagScriptDataInfoParam
{
	tagScriptDataInfoParam(void)
		:pInfoData(NULL),lInfoSize(0)
	{

	}
	~tagScriptDataInfoParam(void)
	{

	}
	char* pInfoData;
	long lInfoSize;
};
struct tagScriptDataInfo : public BaseMemObj
{
public:
	tagScriptDataInfo(tagScriptDataInfoParam* pParam);
	~tagScriptDataInfo(void);
	char* pInfoData;
	long lInfoSize;
};

struct tagSDOA_ITEM_SEND_REQUEST
{
	const char *szOrderId;
	const char* szPayStatus;
	const char* szSendTimes;
	const char* szCallTime;
	const char* szUserId;
	const char* szRoleName;
	const char* szGoodsNum;
	const char* szGoodsInfoEx;
	const char* szIndication;
	const char* payErrorMsg;
	int	nMsgType;
	int nMsgFrom;
	int nMsgSeq;
};	
struct tagValue
{
	string strName;
	string strValue;
};

class CGame
{
public:
	CGame(void);
	virtual ~CGame(void);

	BOOL Init();
	//��ʼ��������Ϣ�����ȼ��Ͷ�����Ϣ
	BOOL InitMsgInfo();
	BOOL InitStackFileName();
	BOOL InitGoodsLogFileName();
	BOOL Release();
	BOOL MainLoop();

	BOOL InitNetServer(void);			//��ʼ����Ϸ����������ˣ���������������ˣ�װ������

	BOOL StartConnWS(void);			//������WordServer�����ӣ���Ϊ�ͻ��˳�ʼ��
	//������DBServer�����ӣ���Ϊ�ͻ��˳�ʼ��
	BOOL StartConnDBS(void);
	//������LogServer�����ӣ���Ϊ�ͻ��˳�ʼ��
	bool StartConnLogS(void);

	BOOL StartConnBS();//������BillingServer������

	//������BounsServer�����ӣ���Ϊ�ͻ��˳�ʼ��
	bool StartConnBounsServer(bool bReCon = false);

	BOOL StartAccept();
	void OnPlayerLost(const CGUID& PlayerGuid,long lSocketID);
	void OnPlayerExit(const CGUID& PlayerGuid, BOOL isChangeServer=FALSE);
	void OnWSLost();
	void OnDBSLost();
	void OnLOGSLost(void);
	void OnBSLost();
	void OnBounsServerLost();

	//���紫��ӳ�ͨ��ӵ��ʱ����
	void OnTransferCongestion(const CGUID& PlayerGuid,long lSocketID);
	//���紫���ӵ������ͨʱ����
	void OnTransferSmoothly(const CGUID& PlayerGuid,long lSocketID);
	void RemoveTransferCongPlayerID(hash_set<CGUID,hash_guid_compare>& PlayersGUID);
	bool IsTransferCongPlayerID(const CGUID& PlayerGUID);

	BOOL ProcessMessage();


	// [AHC 080809 DBSMSG]
	void AddReconnectionMsg(DBWriteSet& setWriteDB);
	BOOL ReConnectWorldServer();
	BOOL ReConnectDBServer();
	bool ReConnectLogServer();
	BOOL ReConnectBSServer();
	bool ReConnectBounsServer();


	VOID	CreateConnectWorldThread();						//���������߳�
	void	CreateConnectDBThread();
	void	CreateConnectLogThread(void);
	VOID	CreateConnectBSThread();
	void	CreateConnectBounsServerThread();

	/////////////////////////////////////////////////////////////////////////
	// ��Ϸȫ�ֽӿ�
	/////////////////////////////////////////////////////////////////////////

	long GetWSSocket(void) { return m_lWSSocket; }
	long GetDBSSocket(void) { return m_lDBSocket; }
	long GetBSSocket(void) {return m_lBSSocket;}
	bool GetFirstConnectionWSFlag(void) { return b_IsFirstConnectionWS; }
	void SetFirstConnectionWSFlag(bool flag) { b_IsFirstConnectionWS = flag; }

	void SetServerInitSendFlag(eServerInitSendMsgFlag flag);
	void ProcessServerInitSendMsg();

	bool GetIsGenVarInit(void) { return m_bIsGenVarInit; }
	void SetIsGenVarInit(bool flag) { m_bIsGenVarInit = flag; }

	void SetSaveDeltaTime(long lTime) { g_lSaveDeltaTime = lTime; }

	void OutPlayerAndRegionInfo(const char* pszFileName);

	//! ��Ӧ�µ�һ�쿪ʼ��
	void OnNewDayStart(DWORD dwTime);

	//! ����������
	void ResetStarDay(DWORD dwTime);
	//! �õ�������
	eConstellation GetStarDay(void);
private:
	//! ��ǰ������
	eConstellation m_Constellation;

private:	
	DWORD	s_lAITick;							// ��ϷAI�����������ڹ����еĴ���
	long	g_lSaveDeltaTime;					// ����ʱ����

	CDataBlockAllocator	*m_pDBAllocator;
	CServer				*s_pNetServer;			// ��ΪGameServer�ķ�������
	long				m_lWSSocket;			// ����worldserver�Ŀͻ���
	long				m_lDBSocket;			// ����DBserver�Ŀͻ���
	long				m_lBounsSocket;			// ����BounsServer�Ŀͻ���
	long				m_lBSSocket;			// ����BillingServer�Ŀͻ���
	long				m_ServerInitSendFlag;
	CRITICAL_SECTION	m_CSServerInitSendFlag;

	ISDOAOrderHandler* m_pSDOAOrderHandler;
	bool m_bSDOAInitialized;


	typedef map<CGUID, CPlayer*,guid_compare>::iterator itPlayer;
	std::map<CGUID, CPlayer*,guid_compare> s_mapPlayer;			// ���汾����������б�
	//����ӵ����PlayerGUID
	hash_set<CGUID,hash_guid_compare>	m_TransCongPlayers;

	typedef map<string,CGUID>	mapStringToGUID;
	typedef map<string,CGUID>::iterator itSG;
	mapStringToGUID m_PlayerNameToGUID;

	char*	m_strFunctionList_FileData;				// �ű������б��ļ�����
	long	m_lFuncFileDataLen;
	char*	m_strVariableList_FileData;				// �ű������б��ļ�����
	long	m_lVariableFileDataLen;
	map<string, tagScriptDataInfo*>		m_mapScript_FileData;	// ���нű��ļ�������map<�ļ���, �ļ�����>
	
	// �Ƿ��������WS
	bool					b_IsFirstConnectionWS;

	BOOL					m_bBSConnected;
	bool					m_bBounsSConnected;			// BounsServer�Ƿ�����

	//û�����²����ļ�¼��ջ���ļ�����
	char m_pszRunStackFile[MAX_PATH];
	//��Ʒ������־
	char m_pszGoodsLogFile[MAX_PATH];

	// �����Ƿ��ʼ�����
	bool m_bIsGenVarInit;
public:
	ISDOAOrderHandler* GetSDOAOrderHandler(){return m_pSDOAOrderHandler;}
	bool IsSDOAInitialized(){return m_bSDOAInitialized;}
	bool IsSDOAValid(){return (m_pSDOAOrderHandler && m_bSDOAInitialized);}

	const char* GetCountryName(BYTE cCountryType);

	DWORD GetAITick()							{return s_lAITick;}
	void AddAITick()							{s_lAITick++;}
	CServer* GetNetServer()				{return s_pNetServer;}

	const char* GetStatckFileName()	{return m_pszRunStackFile;}
	const char* GetGoodsLogFile()	{return m_pszGoodsLogFile;}

	long GetPlayerCount()						{return (long)s_mapPlayer.size();}
    CMoveShape* FindShape(OBJECT_TYPE type, const CGUID& entityID);
	CBaseObject *FindBaseObject(unsigned long objecttype,const CGUID& guid);
	// ͨ��RgnGUID���ҳ���
	CServerRegion* FindRegionByRgnID(const CGUID& RgnGUID);
	// ͨ��RgnGUID ���� OwnerGUID���ҳ���
	CServerRegion* FindRegion(const CGUID& RegionID);

	CServerRegion* FindProxyRegion(const CGUID& lRegionID);
	CServerRegion* FindRegion(const char* strName);
	CPlayer* FindPlayer(const CGUID& PlayerGuid)			{return (s_mapPlayer.find(PlayerGuid) != s_mapPlayer.end()) ? s_mapPlayer[PlayerGuid] : NULL;}
	CPlayer* FindPlayer(const char* strName);
	void SendSysMailMsgToOnlinePlayer(CMail *pMail);//Ϊ������ҷ��� {return &s_mapPlayer;}

	void AddPlayer(CPlayer* pPlayer);
	void RemovePlayer(const CGUID& PlayerID);
	CMonster* FindMonster(const CGUID& MonsterID);//Ѱ��һ��monster
	bool KickPlayer(const CGUID& PlayerGuid);
	void KickAllPlayer(const CGUID& exceptID=NULL_GUID);
	void AddRegion(eRgnType rgnType, const CGUID& RegionID, CServerRegion* pRegion);
	
	map<string, tagScriptDataInfo*>* GetScriptFileDataMap()		{return &m_mapScript_FileData;}
	const char* GetFuncitonList_FileData()			{return m_strFunctionList_FileData;}
	const char* GetVariableList_FileData()			{return m_strVariableList_FileData;}

	const char* GetScriptFileData(const char* filename)	
	{
		if(NULL == filename)	return NULL;
		map<string, tagScriptDataInfo*>::iterator it = m_mapScript_FileData.find(filename);
		if(it != m_mapScript_FileData.end()) 
		{
			if(it->second && it->second->pInfoData)
				return it->second->pInfoData;
		}
		return NULL;
	}

	BOOL IsBSConnected(){return m_bBSConnected;}
    bool IsBounsSConnected()    { return m_bBounsSConnected; }
	BOOL SetBSConnectedFlag(BOOL bVal){m_bBSConnected=bVal;}

	void SetFunctionFileData(char* pData, long lLen);
	void SetVariableFileData(char* pData, long lLen);
	void SetGeneralVariableFileData(BYTE* pByte,long lPos);
	void SetScriptFileData(const char* filename, char* pData, long lLen);

	//���빲�����ݿ�
	BYTE* AllockShareDB(long lSize);
	//�ͷŹ������ݿ�
	void FreeShareDB(BYTE* pData);

	bool CheckInvalidString(string& str,bool bReplace=false);

	void SendTopInfoToClient(long lTopInfoID,const CGUID& PlayerGuid,long lFlag,long lParam,string strInfo);//���Ͷ�����ʾ��Ϣ���ͻ���

	VOID SendNotifyMessage( char* szText,const CGUID& playerGuid, DWORD dwColor = 0xffffffff, DWORD dwBkColor = 0,eNOTIFY_POS ePos=eNOTIFYPOS_CENTER);
	VOID SendNotifyMessage( char* szText,CServerRegion* pRegion, DWORD dwColor = 0xffffffff, DWORD dwBkColor = 0,eNOTIFY_POS ePos=eNOTIFYPOS_CENTER);
	//������Ϣ���������
	void SendMsgToAllPlayer(CMessage *pMsg);
	//������Ϣ��ͬ���ҵ����
	void SendMsgToSameCountryPlayer(CMessage *pMsg,BYTE btCountry);
	//������Ϣ��ͬ��Ӫ���ҵ����
	void SendMsgToSameCampPlayer(CMessage *pMsg,BYTE btCountry);
	//��ѯ�������
	void QuerySilencePlayers(CGUID& GmID);
	/////////////////////////////////////////////////////////////////////////
	// �̹߳���
	/////////////////////////////////////////////////////////////////////////
public:
	HANDLE m_hEndEvent[2];				// �����¼�

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
		DWORD	dClientNo;			// �ͻ��˱��
		long	lUseBillingServer;		//�Ƿ�����BillingServers
		long	lUseBounsServer;		// �Ƿ�����BounsServer
		DWORD	dwGameServerNum;		// GS����BounsServer�ı��

		string	strWorldIP;			// WorldServerIP
		DWORD	dwWorldPort;		// WorldServerPort
		DWORD	dwListenPort;		// ����Port
		string	strLocalIP;			// ����IP

		long	lMaxBlockConNum;
		long	lValidDelayRecDataTime;

		DWORD	dwDataBlockNum;			//���ݿ�����
		DWORD	dwDataBlockSize;		//���ݿ��С
		DWORD	dwFreeMsgNum;			//Ԥ�������Ϣ����

		DWORD	dwFreeSockOperNum;		//�����������Ԥ��������
		DWORD	dwFreeIOOperNum;		//��ɶ˿���IO����Ԥ��������
		long	lIOOperDataBufNum;		//Ĭ��IO������DataBuf����
		long	lEncryptType;			// ��������

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
		long	lConMaxBlockedSendMsgNum;//��������ķ�����Ϣ����

		DWORD	dwRefeashInfoTime;	// ˢ��һ����Ϣ��ʱ��
		DWORD	dwSaveInfoTime;		// ����һ����Ϣ�ļ��ʱ��
		DWORD	dwGoodsLogTime;		// ��ƷLog���ٷ������һ��
		bool	bWatchRunTimeInfo;	// �Ƿ������е�ʱ����Ϣ
		DWORD	dwWatchRunTime;		// ��ؼ��
		DWORD	dwMsgValidateTime;	// ��Ϣ��֤ʱ�䣨���룬0Ϊ�رգ�
		DWORD	dwGappID;			//ӳ���ļ��ͻ���ID
		tagSetup()
		{
			strWorldIP = "127.0.0.1";
			dwWorldPort = 8100;
			dwListenPort = 2347;
			strLocalIP = "127.0.0.1";
		
			lUseBillingServer=BILL_CONNECT_NONE;
		}
	};

	struct tagBSSetup
	{
		DWORD dwIndex;
		string strIp;
		DWORD dwPort;
	};

private:
	tagSetup		m_Setup;
	BYTE			m_lIDIndex;			// ��������ţ���worldserver�õ�
	DWORD			m_dwWSNumber;		//���ӵ�WS���
	CDupliRegionSetup	*m_pDupliRegionSetup;

	list<tagBSSetup> m_listBSSetup;
	list<tagBSSetup> m_listBDSSetup;	// ���ͷ������б�

public:
	bool	LoadSetup();
	bool	ReLoadSetup();			//�ض������ļ�
	bool	ReloadSetupEx();

	//����װ����������
	tagSetup*	GetSetup()			{return &m_Setup;}
	BYTE GetIDIndex()				{return m_lIDIndex;}
	void SetIDIndex(BYTE byteVal)	{m_lIDIndex=byteVal;}
	CDupliRegionSetup* GetDupliRegionSetup()	{return m_pDupliRegionSetup;}

	DWORD GetWSNumber(){return m_dwWSNumber;}
	VOID SetWSNumber(DWORD dwNumber){m_dwWSNumber=dwNumber;}

	//////////////////////////////////////////////////////////////////////////
	//	���
	//////////////////////////////////////////////////////////////////////////
public:
	long GetNumTeams();	
public:
	void RefreashAllMonsterBaseProperty();		// ˢ�����й����BasePropertyָ��

	//////////////////////////////////////////////////////////////////////////
	// �����������Client��ϢЧ�������ַ���
	//////////////////////////////////////////////////////////////////////////
private:
	map<CGUID,CSequenceString*> m_SequenceMap;
public:
	map<CGUID,CSequenceString*>* GetSequenceMap() { return &m_SequenceMap; }

	CSequenceString* GetSequence(const CGUID& guid);
	void CleanSequenceMap();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �����������Client��ϢЧ��ʱ��
	//////////////////////////////////////////////////////////////////////////
private:
	map<CGUID,bool> m_ValidateTime;
public:
	map<CGUID,bool>* GetValidateTimeMap() { return &m_ValidateTime; }

	bool *GetValidateTime(const CGUID& guid);
	void AppendValidateTime(const CGUID& guid, bool enable = false);
	void RemoveValidateTime(const CGUID& guid);
	void CleanValidateTime() { GetValidateTimeMap()->clear(); }

	// [AHC 080809 DBSMSG]

	// ����MSGID
	inline long AllocMsgID(void);

	// �ͷ�һ��MSGID
	inline void FreeMsgID(long id);

	// ���ͱ���PHDupRgn ����
	void SendSavePHDupRgn(const CGUID& ownerGUID);

	// ������ݷֲ㱣��ӿ�
	// ������ҽ�Ǯ/�ȼ��ı� ����������Ϣ
	void SendPlayerAttrData(const CGUID& playerID);
	// �������������Ʒ�ı䱣����Ϣ
	void SendPlayerContainerGoodsData(const CGUID& playerID, long place/*������������*/);
	// ������ұ����б�ı䱣����Ϣ
	void SendPlayerVarListData(const CGUID& playerID);
	// ��������������ݸı䱣����Ϣ
	void SendPlayerQuestData(const CGUID& playerID);
	// ������Ҽ������ݸı���Ϣ
	void SendPlayerSkillData(const CGUID& playerID);
	// �������״̬���ݸı���Ϣ
	void SendPlayerStateData(const CGUID& playerID);

	// ���ݷֲ�洢�ӿ�
	void AppendSaveMsgProperty		(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgEquipmentGoods(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgPackGoods		(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgSubPackGoods	(long packID, CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgVarList		(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgQuestList		(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgMerQuestList	(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgSkillList		(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgStateList		(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgDeputyOccuList(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgWalletGoods	(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgSilverGoods	(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgDepotGold		(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgDepotSilver	(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgDepotGoods	(CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgSubDepot		(long packID, CPlayer* player, DBWriteSet& setWriteDB);
	void AppendSaveMsgSpriteSystem	(CPlayer* player, DBWriteSet& setWriteDB);
	

	void AppendSaveMsgRegion(CServerRegion* pRegion,CMessage* pSendMsg);
	void UpdatePlayerPack(CPlayer* pPlayer,DWORD dwContainerId,DWORD dwPos);	
	// ��ӻ����޸�һ�����Ҫ���������ö��ֵ
	void UpdateSavePlayer(CPlayer* player, SAVE_DETAIL_ATTR_TYPE type);
	// ��ӻ����޸�һ�����Ҫ���������ö��ֵ
	void UpdateSavePlayer(const CGUID& playerID, SAVE_DETAIL_ATTR_TYPE type);
	// ������ұ��淢����Ϣ
	void SendQueuePlayerOperMsg(void);

	// �ӳٱ�����������Ҷ���(delete)
	void ClearSavePlayer(const CGUID& guid);
	void AddSaveDetailPartToByteArray(const CGUID& guid, DWORD flag, CPlayer* player);

	//���Ҫ���泡��
	void UpdateSaveRegion(CServerRegion* pRegion);
	void ClearSaveRegion(CServerRegion* pRegion);
	void ClearSaveRegion();
	void PushSavePlayer(CPlayer* pPlayer);
	CPlayer* FindSavePlayer(const CGUID& guid);
	std::map<CGUID, CPlayer*>&	GetSavePlayerMap(void) { return m_mapSavePlayers; }

	VOID LogRegion(VOID);

	CPlayer* FindChangeRgnPlayer(const CGUID& guid);
	void ClearChangeRgnPlayerVec(void);

	void PushChangeRgnPlayer(const CGUID& playerID,tagPCR& ChangeRegion);
		
	void PushDelDupRgn(CServerRegion* pRgn);
	void ClearDelDupRgn(void);
//ʱ������ͳ��
private:
	//��Ϣ����ͳ��
	map<long,long>	m_mapMsgExpendTime;
	//��ʱ������ͳ��
	map<long,long>	m_mapTimerExpendTime;
public:

#ifdef _SCRIPT_INFO_
	struct tagScriptRunInfo
	{
		DWORD	runTime;
		long	runCount;
		tagScriptRunInfo(void){memset(this, 0, sizeof(tagScriptRunInfo));}
	};

	typedef map<string, tagScriptRunInfo>			ScriptInfoMap;
	typedef map<string, tagScriptRunInfo>::iterator	ScriptInfoMapItr;

	ScriptInfoMap m_ScriptInfo;

	//!				����������Ϣ
	void			ClearScriptInfo(void)									{m_ScriptInfo.clear();}
	//!				���һ���ű���Ϣ
	void			AddScriptInfo(const char *pScriptName, DWORD runTime)	{
		string strScriptName(pScriptName);
		ScriptInfoMapItr itr = m_ScriptInfo.find(strScriptName);
		if(m_ScriptInfo.end() == itr)
		{
			m_ScriptInfo.insert(make_pair(strScriptName, tagScriptRunInfo()));
			itr = m_ScriptInfo.find(strScriptName);
		}
		itr->second.runTime += runTime;
		++itr->second.runCount;
	}
	void			GetScriptInfo(string &strOutInfo)
	{
		strOutInfo.append("\r\n");
		ScriptInfoMapItr itr = m_ScriptInfo.begin();
		for (; itr != m_ScriptInfo.end(); ++itr)
		{
			char szTmpInfo[10240] = {0};
			_snprintf(szTmpInfo, 10240, "%-80s, %6d��, %8ums\r\n", itr->first.c_str(), itr->second.runCount, itr->second.runTime);
			strOutInfo.append(szTmpInfo);
		}
	}
#endif

	inline void AddMsgExpendTime(long lType,long lTime)
	{
		if(GetSetup()->bWatchRunTimeInfo)
			m_mapMsgExpendTime[lType] += lTime;
	}
	inline void AddTimerExpendTime(long lType,long lTime)
	{
		if(GetSetup()->bWatchRunTimeInfo)
			m_mapTimerExpendTime[lType] += lTime;
	}

	//�����Ϣ
	void OutputMsgExpendTimeInfo(const char* pszFileName);
private:
	std::map<CGUID, CPlayer*> m_mapSavePlayers;
	std::set<CServerRegion*> m_setSaveRegion;

	typedef map<CGUID,tagPCR,guid_compare>::iterator itCRP;
	map<CGUID,tagPCR,guid_compare>	m_ChangeRgnPlayers;
	map<CGUID, CServerRegion*> m_mapDelDupRgn;

private:
	//����SDOA_ITEM_SEND_REQUEST��Ϣ����
	CMsgQueue m_RecvSDOMessages;
public:
	void OnReceiveSdoItemRequest(tagSDOA_ITEM_SEND_REQUEST &stSdo_Item_Send_Request);

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
	LogClient&			GetLogClient(void){return m_LogClient;}
	//CMailManager		*GetMailManager();
private:

	LogClient			m_LogClient;
	LogicLogInterface	*m_pLogicLogInterface;


	//-------------------------------------------------------------------------------
	// �Ʒѿͻ��˶���. by Fox.		2008-01-22
	// func : ����Ʒѿͻ��˵ĳ�ʼ�����ص��������߳�ͨ��
	//-------------------------------------------------------------------------------
	HINSTANCE		m_hBsipDll;
	CBillClient		*pBillClient;


	
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////

private:
	map<DWORD, tagBounsDes*>		m_mapBounsDes;

public:
	map<DWORD, tagBounsDes*>* GetBounsDes()	{return &m_mapBounsDes;}
	void AddBounsDes(tagBounsDes* pBD);
	tagBounsDes* FindBounsDes(DWORD dwBounsID);
	void ClearBounsDes();

};

BOOL				CreateGame();			// ������Ϸ
CGame*				GetGame();				// ��ȡ��Ϸ
BOOL				DeleteGame();			// �ͷ���Ϸ
LogicLogInterface*	GetGameLogInterface();	// ��ȡ��Ϸ��־�߼��ӿڶ���

unsigned __stdcall		GameThreadFunc(void* pArguments);	// ��Ϸ���߳�
extern HANDLE g_hGameThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bGameThreadExit;				// �Ƿ��˳����߳�

unsigned __stdcall		ConnectWorldFunc( void* pArguments );	// ����WorldServer���̡߳���WorldServer�Ͽ�ʱ���ã���ͬʱ��ر�

unsigned __stdcall		ConnectLogServerFunc( void* pArguments );

unsigned __stdcall		ConnectBSFunc( void* pArguments );

// ����BounsServer���߳�
unsigned __stdcall		ConnectBounsServerFunc(void* pArguments);


