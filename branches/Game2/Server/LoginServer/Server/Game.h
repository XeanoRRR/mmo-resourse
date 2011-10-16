#pragma once

#include "../App/AccountClient.h"
#include "../App/COnCmdConnect.h"
#include "../App/CSdoaFcm.h"
#include "../App/CSdoaValidate.h"
#include "../App/LoaclAccountValidate.h"
#include "../App/LoginManager.h"

#include "../Net/AinMessage.h"
#include "../Net/Message.h"

#include "../../../Public/ServerPublic/Greta/regexpr2.h"
#include "../../../Public/ServerPublic/Server/Common/AsynchronousExecute.h"
#include "../../../Public/AppFrame/TextRes.h"
using namespace AppFrame;

using namespace regex;

class CServer;
class CMessage;
class CRsCDKey;
class CRsLogInfo;
class CMyAdoBase;
class CMySocket;
class CDataBlockAllocator;
class CMapFile;

enum eNetFlag
{
	NF_Server_Client=0x01,
	NF_Server_World=0x10,
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
	BOOL InitNetServer_World(void);				//��ʼ�����WorldServer������������ˣ���������������ˣ�װ������
	BOOL AI();
	BOOL ProcessMessage();

	VOID OnNetASClose(VOID);					//! ��ӦAccountServer���ӶϿ�
	BOOL StartConnAccountServer(VOID);			//! ������AccountServer�����ӣ���Ϊ�ͻ��˳�ʼ��
	BOOL ReConnectAccountServer(VOID);			//! ����AccountServer
	VOID CreateConnectAccountServer(VOID);		//! ���������߳�
	static unsigned __stdcall ConnectAccountFunc(LPVOID pArguments);	// ����WorldServer���̡߳���WorldServer�Ͽ�ʱ���ã���ͬʱ��ر�

	VOID SendToSelf(AinMessage *pMsg)			{m_AinMsgQueue.PushMessage(pMsg);}
/////////////////////////////////////////////////////////////////////////
// ��֤�ͻ���
/////////////////////////////////////////////////////////////////////////
public:


public:
	AccountClient&				GetAccountClient(VOID){return m_AccountClient;}
	AsynchronousExecute&		GetAsynchronousExecute(VOID){return m_AsynchronousExecute;}
	LoginManager&				GetLoginManager(VOID){return m_LoginManager;}
	CSdoaValidate&				GetCSdoaValidate(VOID){return m_CSdoaValidate;}
	CSdoaFcm&					GetCSdoaFcm(VOID){return m_CSdoaFcm;}


private:
	//!
	AccountClient				m_AccountClient;
	//!							�첽ִ�ж���
	AsynchronousExecute			m_AsynchronousExecute;
	//!
	LoginManager				m_LoginManager;

	//!							L2L��Ϣ
	AinMsgQueue					m_AinMsgQueue;

	//!							ʢ��֤ͬ��֤
	CSdoaValidate				m_CSdoaValidate;
	//!							ʢ�������ģ��
	CSdoaFcm					m_CSdoaFcm;

    bool ProcMenus();

/////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////
public:
	COnCmdConnect&				GetOnCmdConnect(VOID) {return m_COnCmdConnect;}
private:
	COnCmdConnect				m_COnCmdConnect;
	
/////////////////////////////////////////////////////////////////////////
// ��Ϸȫ�ֽӿ�
/////////////////////////////////////////////////////////////////////////
public:


	// ��������
private:
	DWORD	s_lAITick;								// ��ϷAI�����������ڹ����еĴ���


    map<long, string>                     m_mapLoginSocket;
	map<string, pair<string, LONG>>				m_LoginCdkeyWorld;		//��ʾ�ڵ�½�����е�Cdkey�Լ���½��WorldServer�����ֺ�ID
	typedef	std::list<string>		WorldCdkeyList;
	std::map<long,WorldCdkeyList>	s_listCdkey;			// ���ߵ�CDKEY�б�

	typedef std::list<string>::iterator itCdkeyList;
	typedef std::map<long,WorldCdkeyList>::iterator itWorldCdkeyList;

    void UpdateStateLvls();
		
	long					m_StateLvl[4];

	//���ݿ������
	CDataBlockAllocator	*m_pDBAllocator;
	//������ܿͻ�����Ϣ���������
	CServer	*s_pNetServer_Client;
	//�������WorldServer��Ϣ���������
	CServer	*s_pNetServer_World;
	//!		��ΪAS�Ŀͻ���
	LONG	m_lASSocket;
	// ��ͨ��QUEST_PLAYER_DATA�����
	map<string, long> m_PassDetailDataCdkey;
public:
	map<string, long/*GSID*/>& GetPassDetailDataCdkeyMap(void) { return m_PassDetailDataCdkey; }

	unsigned GetCdkeyCount();

	DWORD GetAITick()							{return s_lAITick;}
	void AddAITick()							{s_lAITick++;}

	CServer* GetNetServer_Client()	{return s_pNetServer_Client;}
	CServer* GetNetServer_World()	{return s_pNetServer_World;}
 

	void	WsPlayerNumIntoStringFoot(char *pString);
	long	GetLoginWorldPlayerNumByWorldID(long lID);
	long	GetLoginWorldPlayerNumByWorldName(const char* strWorldName);


	bool AddCdkey(const char *szCdkey, long lWorldNumber);
	VOID DelWsCdKey(long lWorldNumber);
	void ClearCDKey(const char *szCdkey);
	void ClearCDKeyByWorldServerID(long lID);
	int FindCdkey(const char *szCdkey);
	long GetLoginWorldCdkeyNumbers();

    void AddSocketCdkey(long lSocketID, const char* szCdkey);
    void RemoveSocketCdkey(long lSocketID);
    void ClearSocketCdkey(const char* szCdkey);
    bool IsInLoginServer(const char* szCdkey);

	bool KickOut(const char *szCdkey);			// �߳�һ��CDKEY
	void KickOut(LONG lSocket);			// �߳�һ������

	VOID SendOnlineInfoToAs(VOID);

	BOOL		WsIsReady(LONG lWorldNumber)
	{
		map<LONG, BOOL>::iterator ite = m_mapWsIsReady.find(lWorldNumber);
		if(m_mapWsIsReady.end() != ite)
			return ite->second;
		return FALSE;
	}
	VOID		SetWsReadyState(LONG lWorldNumber, BOOL bIsReady)
	{
		m_mapWsIsReady[lWorldNumber] = bIsReady;
		if(bIsReady)
        {
			Log4c::Trace(ROOT_MODULE,FormatText("LOGIN_GAME_25", lWorldNumber));//WorldServer[%d] is ready!!!
        }
		else
		{
			Log4c::Trace(ROOT_MODULE,FormatText("LOGIN_GAME_26", lWorldNumber));//Wait WorldServer[%d] reset!!!
			GetOnCmdConnect().ClearGsInfo();
		}
	}

private:
	map<LONG, BOOL>		m_mapWsIsReady;
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

	void AddWorldInfoToMsg(CMessage& msg,string &strCdkey);

	long AddWorld(long lWorldNumber,const char* strWorldName);			// �������
	long DelWorld(long lNum);	// ɾ������

	bool WorldServerIsOpenState(long lNum);

	long GetWorldSateLvl(long lPlayerNum);
	void OnChangeStateLvl(long lWorldNum,long lPlayerNum);
	void ChangeAllWorldSate();
	const char *GetWorldNameByID(long lNum);
	long  GetWorldIDByName(const char* strWorldName);
	bool SendMsg2World(CMessage* pMsg,long lWorldServerID);	//������Ϣ��ָ����WORLDSERVER
		
	void SetLoginCdkeyWorldServer(const char* szCdkey,const char* strWorldServer);	//�����ڵ�½�����е�Cdkey
	const char* GetLoginCdkeyWorldServer(const char* szCdkey);						//�õ���½�����е�Cdkey��WS��Ϣ
	LONG CGame::GetLoginCdkeyWorldServerID(const char* szCdkey);					//�õ���½�����е�Cdkey��WS��ID��Ϣ
	void ClearLoginCdkey(const char* szCdkey);										//�����½�����е�Cdkey
	/*
	 *	��Ϣ����
	 */
	////////////////////loginserver to worldserver
	bool L2W_PlayerBase_Send(const char *wsName,const char *szCdkey);//loginserver��worldserver������һ�����Ϣ 
	void L2W_DeleteRole_Send(const char *szWSName,const char *szCdkey,CGUID& PlayerID,DWORD dwClientIP);//loginserver��worldserver����ɾ����ɫ������
	
	void L2W_RestoreRole_Send(LPCSTR szWSName, LPCSTR szCdkey, const CGUID& playerID); // ls �� ws ���ͻָ���ɫ����

	void L2W_CreateRole_Send(const char *szWSName,const char *szCdkey,CMessage *pMsg);//loginserver��worldserver���ʹ�����ɫ������
	void L2W_QuestDetail_Send(const char *szWSName,const char *szCdkey,CGUID& PlayerID,DWORD dwClientIP, DWORD dwSignCode);//loginserver��worldserver������ҵ���ϸ��Ϣ

	void UpdateDisplayWorldInfo();
	void UpdateWorldInfoToAllClient();	//����WorldServer��״̬��Ϣ���ͻ���

	void OnNetWorldClose(long lWorldID);
	void OnNetClientClose(const char* szCdkey, long lSocketID);

	bool LoadServerResource();
	bool UnLoadServerResource();

/////////////////////////////////////////////////////////////////////////
// ��Ϸ����
/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		DWORD	dwListenPort_World;			// ����World��Port	
		DWORD	dwListenPort_Client;		// ����Client��Port

		string	strAccountServerIp;		//! ��֤������IP
		DWORD	dwAccountServerPort;	//! ��֤�������˿�
		DWORD	dwAccountClientNo;		//! ��֤�ͻ��˱��

		string	strLocalSqlConType;		//! �����˺���֤���ã�DB���
		string	strLocalSqlServerIP;	
		string  strLocalSqlUserName;
		string  strLocalSqlPassWord;
		string  strLocalDBName;

		//Client����������ò���
		DWORD	dwDataBlockNum;			//���ݿ�����
		DWORD	dwDataBlockSize;		//���ݿ��С
		DWORD	dwFreeMsgNum;			//Ԥ�������Ϣ����

		DWORD	dwFreeSockOperNum;		//�����������Ԥ��������
		DWORD	dwFreeIOOperNum;		//��ɶ˿���IO����Ԥ��������
		long	lIOOperDataBufNum;		//Ĭ��IO������DataBuf����
		long	lEncryptType;			// ͨ�ż��ܿͻ������� (eNetFlag: NF_Server_Client)
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
		long	lClientMaxBlockConNum;
		long	lClientValidDelayRecDataTime;

	
		//World����������ò���
		DWORD	dwWorldFreeSockOperNum;	// �����������Ԥ��������
		DWORD	dwWorldFreeIOOperNum;	// ��ɶ˿���IO����Ԥ��������
		long	lWorldIOOperDataBufNum;	// Ĭ��IO������DataBuf����
		bool	bWorldCheckNet;			// �Ƿ��������м��
		DWORD	dwWorldBanIPTime;		// ��ֹIP��ʱ��
		DWORD	dwWorldMaxMsgLen;		// ������������Ϣ����		
		long	lWorldMaxConnectNum;		// �ͻ��˵������������
		long	lWorldMaxClientsNum;		// ���Ŀͻ��˷��ͻ�������С
		long	lWorldPendingWrBufNum;		// ���ķ���IO����Buf�ܴ�С
		long	lWorldPendingRdBufNum;		// ���Ľ���IO����Buf�ܴ�С
		long	lWorldMaxSendSizePerSecond;	// ��ͻ���ÿ�뷢�͵�����ֽ���
		long	lWorldMaxRecvSizePerSecond;	// �ӿͻ��˽��ܵ�ÿ������ֽ���
		long	lWorldMaxBlockedSendMsgNum;	// ��������ķ�����Ϣ����
		long	lWorldConPendingWrBufNum;	// �ͻ������ķ���IO����Buf�ܴ�С
		long	lWorldConPendingRdBufNum;	// �ͻ������Ľ���IO����Buf�ܴ�С
		long	lWorldConMaxSendSizePerSecond;//����������͵�ÿ������ֽ���
		long	lWorldConMaxRecvSizePerSecond;//�ӷ��������ܵ�ÿ������ֽ���
		long	lWorldConMaxBlockedSendMsgNum;	// ��������ķ�����Ϣ����
		long	lWorldMaxBlockConNum;
		long	lWorldValidDelayRecDataTime;

		DWORD	dwRefeashInfoTime;			// ˢ����ʾʱ��
		DWORD	dwSaveInfoTime;				// ����һ����Ϣ��ʱ����
		DWORD	dwPingWorldServerTime;	// ��ʱPingWorldServer�ļ��
		DWORD	dwPingWorldServerErrorTime;	// PingWorldServer���յ����صĳ���ʱ����

		DWORD	dwMinDbOptThreadNum;	//! �����˺���֤����, ��СDB�����̱߳�����
		DWORD	dwMaxDbOptThreadNum;	//! �����˺���֤���ã����DB�����߳���
		DWORD	dwThreadNumAdjustSpace;	//! �����˺���֤���ã��Զ������߳����ļ��ʱ��
		DWORD	dwGappID;				//GAPP�ı��
		tagSetup()
		{
			bCheckNet = 1;
			dwMaxMsgLen = 102400;
			dwBanIPTime = 10;
			dwRefeashInfoTime = 1000;
			dwSaveInfoTime = 60000;
		}
	};

	struct tagLoginSetup
	{
		bool	bSndaValidate;				//! �Ƿ�ʹ��ʢ��ʽ��֤
		bool	bXmCardEnable;				//! �Ƿ�ʹ�������ܱ���
		bool	bSndaFcmEnable;				//! �Ƿ�����ʢ�������
		
		DWORD	dwDoQueueInter;				//! ����ȴ����еļ��ʱ��
		DWORD	dwSendMsgToQueInter;		//! ���ȴ����з�����Ϣ�ļ��ʱ��

		DWORD	m_dwMaxWsMum;				//!	WS�������
		DWORD	m_dwProcessRankInterval;	//!	�����ŶӼ�������룩
		DWORD	m_dwSendQueueMsgInterval;	//!	�����Ŷ���Ϣ��������룩
		DWORD	m_dwOneProcessNum;			//!	ÿ�δ���������ÿ��WS��

		float	fWorldBusyScale;			//! ��æ����������
		float	fWorldFullScale;			//! ��������������

		
		DWORD	m_ValidateWaitTimeSpace;	//!	��֤�ȴ�ʱ�䣨���룩
		DWORD	m_SelectWaitTimeSpace;		//!	ѡ�˽�����ʱ�䣨���룩
		DWORD	m_JustOutTimeSpace;			//!	С�˳�ʱʱ�䣨���룩

		DWORD	m_MaxPwdErrCount;			//!	���������������������
		DWORD	m_PwdErrSaveTime;			//!	��������������ʱ��(��)
		DWORD	m_FrostCdkeyTime;			//! �����½ʱ��(��)

		char	m_szCdkeyRegex[128];		//! ��֤�˺��õ�������ʽ
		bool	m_bValideClientResourceVersion;//�Ƿ���֤�ͻ�����Դ�汾
		char	m_szClientResourceVersion[128];//�ͻ�����Դ�汾
	};


private:
	tagSetup		m_Setup;
	tagLoginSetup	m_LoginSetup;
	rpattern		*m_pPat;

	//�ڴ�Ӱ���ļ�
	CMapFile	*m_pMapFile;
	//û�����²����ļ�¼��ջ���ļ�����
	char m_pszRunStackFile[MAX_PATH];

	bool			m_bReLoadLoginSetup;
	bool			m_bReLoadIpLimitSetup;

public:
	bool			LoadSetup();
	bool			LoadMapFileSetup();
	bool			ReLoadSetup();
	bool			ReLoadSetupEx();
	bool			ReLoadWorldSetup();
	bool			LoadLoginSetup(VOID);
	bool			LoadIpLimitSetup(VOID);
	rpattern&		GetRpattern(VOID);
	tagSetup*		GetSetup()	{return &m_Setup;}
	tagLoginSetup*	GetLoginSetup(VOID){return &m_LoginSetup;}
	bool			UseAsFcm(VOID){return !(GetLoginSetup()->bSndaValidate && GetLoginSetup()->bSndaFcmEnable);}
	

	//��ʼ����ջ�ļ���
	BOOL InitStackFileName();

	const char* GetStatckFileName()	{return m_pszRunStackFile;}

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
		string	strName;
		DWORD	dwPlayerNum;
		vector<tagPingGameServerInfo>	vectorGameServerInfo;
	};

	void AddPingWorldServerInfo(tagPingWorldServerInfo& st)	{m_vectorPingWorldServerInfo.push_back(st);}

private:
	vector<tagPingWorldServerInfo>	m_vectorPingWorldServerInfo;


public:

	int		ReadyToEnter(const char *szCdkey, DWORD dwIP, long lSockID, const char *szWSName);
	BOOL	EnterToSelect(const char *pCdkey, const char *pWSName, DWORD dwIP, long lSockID);
	int		EnterGame( const char *szCdkey, DWORD dwIP, long lSockID, const char *szWSName, bool bLimit=false, bool bMobileUser=false );

#ifdef SHOW_LOGIN_MSG_20090817
public:
	void	OpenCloseInfo(void)
    {
        bOutInfo = !bOutInfo;   
        //��½��Ϣ�����%s��/����/�ر�
        Log4c::Trace(ROOT_MODULE,FormatText("LOGIN_GAME_4",((bOutInfo) ? GetText("LOGIN_GAME_5") : GetText("LOGIN_GAME_6")))
    }
	bool	OutInfo(void){return bOutInfo;}

private:
	bool	bOutInfo;
#endif
};

BOOL CreateGame();			// ������Ϸ
CGame* GetGame();			// ��ȡ��Ϸ
BOOL DeleteGame();			// �ͷ���Ϸ

unsigned __stdcall GameThreadFunc(void* pArguments);	// ��Ϸ���߳�
