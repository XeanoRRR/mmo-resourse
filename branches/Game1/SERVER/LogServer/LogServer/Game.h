
#include "../public/UseMemManageLogS.h"
#include "..\public\AsynchronousExecute.h"


#pragma once


class LogManage;

class CServer;
class CDBCountry;
class CDataBlockAllocator;

class CMessage;
class CCostNetClient;
extern HWND g_hLogText;;
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
	BOOL ProcessMessage();

	


	//////////////////////////////////////////////////////////////////////////
	//	GameServer�������
	//////////////////////////////////////////////////////////////////////////
public:
	// GameServer��Ϣ
	struct tagGameServer
	{
		bool			bConnected;			// ��Client�Ƿ��Ѿ�������
		DWORD			dwIndex;			// Client��
		string			strIP;				// IP
		DWORD			dwPort;				// �˿�
	};

public:
	tagGameServer* GetGameServer(DWORD dwIndex)	{return (s_mapGameServer.find(dwIndex)!=s_mapGameServer.end())?&s_mapGameServer[dwIndex]:NULL;}
	tagGameServer* GetGameServer(const char* strIP, DWORD dwPort);			// ��ȡGameServer������

	long GetGameServerNumber_ByRegionID(long lRegionID);
	long GetGameServerNumber_ByPlayerID(const CGUID& PlayerGuid);

	bool LoadServerSetup(const char *filename);								// ��ȡ��Ϸ����������
	long GetConnectedGameServerCount();										// �����Ѿ������ϵ�GameServer����

	void OnGameServerConnect(DWORD dwIP, LONG lSocket);
	void OnGameServerClose(long lSocket);									// ��ĳ��GameServer�Ͽ����ӵ�ʱ��
	void OnGameServerLost(DWORD dwIndex);									// ��ĳ��GameServer�Ͽ�ʱ�Ĵ���

	map<DWORD, tagGameServer>* GetGameServerList(){return &s_mapGameServer;}

private:
	map<DWORD, tagGameServer>		s_mapGameServer;	// �������б�<dwIndex, tagGameServer>
	set<DWORD>						m_setClientIP;

	


	//////////////////////////////////////////////////////////////////////////
	//	�����ӿ�
	//////////////////////////////////////////////////////////////////////////
public:                                          
	void SendMsg2GameServer(DWORD dwIndex,CMessage* pMsg);					// ����һ����Ϣ��ָ��Index��GameServer
	void SendAllGameServer(CMessage* pMsg);									// ����һ����Ϣ������GameServer


	AsynchronousExecute&	GetAsynchronousExecute(VOID){return m_AsynchronousExecute;};


private:
	AsynchronousExecute		m_AsynchronousExecute;
	//////////////////////////////////////////////////////////////////////////
	//	���粿��
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL InitNetServer(void);					//��ʼ����Ϸ����������ˣ���������������ˣ�װ������
	bool StartAccept();							//��ʼ��Ϊ����������

	
	CServer* GetNetServer()	{return s_pNetServer;}

private:
	CDataBlockAllocator	*m_pDBAllocator;
	CServer				*s_pNetServer;			// ��ΪGameServer�ķ�������


	/////////////////////////////////////////////////////////////////////////
	// ��Ϸ����
	/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		DWORD	dwNumber;					// ���������(0-7)
		string	strName;					// ����������
		DWORD	dwListenPort;				// ����Port

		//Client����������ò���
		DWORD	dwDataBlockNum;				//���ݿ�����
		DWORD	dwDataBlockSize;			//���ݿ��С
		DWORD	dwFreeMsgNum;				//Ԥ�������Ϣ����

		DWORD	dwFreeSockOperNum;			//�����������Ԥ��������
		DWORD	dwFreeIOOperNum;			//��ɶ˿���IO����Ԥ��������
		long	lIOOperDataBufNum;			//Ĭ��IO������DataBuf����

		bool	bCheckNet;					// �Ƿ��������м��
		DWORD	dwBanIPTime;				// ��ֹIP��ʱ��
		DWORD	dwMaxMsgLen;				// ������������Ϣ����		
		long	lMaxConnectNum;				// �ͻ��˵������������
		long	lMaxClientsNum;				// ���Ŀͻ��˷��ͻ�������С
		long	lPendingWrBufNum;			// ���ķ���IO����Buf�ܴ�С
		long	lPendingRdBufNum;			// ���Ľ���IO����Buf�ܴ�С
		long	lMaxSendSizePerSecond;		// ��ͻ���ÿ�뷢�͵�����ֽ���
		long	lMaxRecvSizePerSecond;		// �ӿͻ��˽��ܵ�ÿ������ֽ���
		long	lMaxBlockedSendMsgNum;		// ��������ķ�����Ϣ����
		long	lConPendingWrBufNum;		// �ͻ������ķ���IO����Buf�ܴ�С
		long	lConPendingRdBufNum;		// �ͻ������Ľ���IO����Buf�ܴ�С
		long	lConMaxSendSizePerSecond;	//����������͵�ÿ������ֽ���
		long	lConMaxRecvSizePerSecond;	//�ӷ��������ܵ�ÿ������ֽ���
		long	lConMaxBlockedSendMsgNum;	// ��������ķ�����Ϣ����

		DWORD	dwRefeashInfoTime;			// ˢ����ʾʱ��
		DWORD	dwSaveInfoTime;				// ����һ����Ϣ��ʱ����

		DWORD	dwReleaseLoginPlayerTime;			// �ͷ�LoginList���Player�ĳ�ʱʱ��


		DWORD	dwMinDbOptThreadNum;		//! ��СDB�����̱߳�����
		DWORD	dwMaxDbOptThreadNum;		//! ���DB�����߳���
		DWORD	dwThreadNumAdjustSpace;		//! �Զ������߳����ļ��ʱ��
		DWORD	dwGappID;
		tagSetup()
		{
			dwRefeashInfoTime = 1000;
			dwSaveInfoTime = 60000;
		}
	};

private:
	tagSetup	m_Setup;
	DWORD		m_dwtick;
public:
	DWORD		GetAITick()	{return m_dwtick;}
	bool		LoadSetup();
	bool		LoadServerResource();
	tagSetup*	GetSetup()	{return &m_Setup;}
	

	///////////////////////////////
	// ��־����
	///////////////////////////////
private:
	//! ��Ϸ��־�������
	LogManage *m_pGameLogManage;
	//!	��Ӧ�˵��еĴ�������ã�������־��Ϊ�������߳��н����������ݿ����
	BOOL	m_bCreateTable;

public:
	void OnClientLogin(CMessage *pMsg);
	void OnLog(CMessage *pMsg);
	void OnStrSqlLog(CMessage *pMsg);
	void CreateTableInDB(void);
	void CreateTableInSqlFile(void);
	void OutTableInfoToFile(void);
	void ReLoadConfig(void);


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
