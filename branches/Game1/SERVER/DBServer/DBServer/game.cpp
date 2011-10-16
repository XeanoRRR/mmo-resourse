#include "StdAfx.h"

#include <sstream>

#include ".\game.h"
#include "..\nets\Servers.h"
#include "..\..\..\dbaccess\myadobase.h"
#include "..\..\..\dbaccess\worlddb\rsEntity.h"
#include "..\public\WordsFilter.h"
#include "..\public\dupliregionsetup.h"
#include "..\public\DataBlockAllocator.h"
#include "../AppWorld/Thread/Threaddatadef.h"

#include "../AppWorld/Entity/EntityGroup.h"

#include "../AppWorld/entity/EntityManager.h"
#include "../public/strtype.h"
#include "../public/StrUnifyInput.h"
#include "../AppWorld/Thread/DBAccessThread.h"
#include "..\AppWorld\SMClient.h"
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const DWORD g_ms = 40;

// ������
DWORD g_lCurTime = 0;
DWORD g_lLSMessageTime = 0;
DWORD g_lGSMessageTime = 0;
DWORD g_lCSMessageTime = 0;

CGame* g_pGame= NULL;		// ��Ϸ�ͻ�������

HANDLE g_hGameThreadExitEvent=NULL;		// ���߳��˳��¼�
bool g_bGameThreadExit=false;		// �Ƿ��˳����߳�

HANDLE	hConnectThread=NULL;			// �����߳̾�� 
bool	bConnectThreadExit=false;		// �Ƿ��˳������߳�

HANDLE hConnectThread1=NULL;
bool	bConnectThreadExit1=false;

HANDLE g_hWriteLogThread = NULL;			// ��־�߳�
bool volatile g_bWriteLogThreadExit = false;			// �Ƿ��˳���־�߳�

vector<HANDLE> g_vPlayerLoadThreads;		// ��Ҷ����ݿ��߳�
bool volatile g_bPlayerLoadThreadsExit = false;
long g_vPlayerLoadThreadFlag; // ��Ҷ����ݿ��߳��˳���־


HANDLE g_hSavingThread = NULL; // �����߳̾��

// load thread ����ID
long g_ThreadCountID = 0;

// Ĭ�ϲ���ʾ������Ϣ
extern bool g_bShowSaveInfo = false;
void ShowSaveInfo(LPCSTR szInfo, ...)
{
	if(g_bShowSaveInfo)
	{
		char str[256];
		va_list va;
		va_start(va,szInfo);
		vsprintf(str,szInfo,va);
		va_end(va);

		AddLogText(const_cast<char*>(str));
	}
}

CRITICAL_SECTION g_CriticalSectionSavePlayerList;	// ��������ҵ��߳�ͬ��(GameThread <-> SaveThread)
CRITICAL_SECTION g_CriticalSectionSaveThread;		// �����߳�ͬ��

extern unsigned __stdcall SaveThreadFunc(void* pArguments);

bool g_bSendSaveMsgNow = false;
bool g_bSaveNowData = false;
bool g_bSaveAllOrg = false;			//�ֶ����뱣�����еİ��
bool g_bSendCollectPlayerDataMsgNow = false;
bool g_bDelPlayers = false;

bool g_bStatPlayerRanks = false;

unsigned volatile RELOAD_PROFILE_FLAGS = 0x0;	// �ض������ļ���ʾ
// define const
extern const unsigned RELOAD_LOGING = 0x00000001;		//����־����
extern const unsigned RELOAD_GMLIST = 0x00000002;		//��GM����
extern const unsigned RELOAD_BROADCAST = 0x00000004;	//���㲥
extern const unsigned RELOAD_VILLAGEWAR = 0x00000008;	//����ս

extern const unsigned RELOAD_CITYWAR = 0x00000010;		//����ս
extern const unsigned RELOAD_INCREMENT = 0x00000020;	//����ֵ�̵�
extern const unsigned RELOAD_GAMESETUP = 0x00000040;	//������͵�����
extern const unsigned RELOAD_WORDFILTER = 0x00000080;	//�����ֹ���

extern const unsigned RELOAD_PLAYERLIST = 0x00000100;	//������Ժ;���
extern const unsigned RELOAD_GOODSLIST = 0x00000200;	//��Ʒ����
extern const unsigned RELOAD_MONSTERLIST = 0x00000400;	//�������Ժ͵���
extern const unsigned RELOAD_TRADELIST = 0x00000800;	//�̵�����

extern const unsigned RELOAD_SKILLLIST = 0x00001000;	//��������
extern const unsigned RELOAD_GOLBELSETUP = 0x00002000;	//ȫ������
extern const unsigned RELOAD_ALLSCRIPT = 0x00004000;	//���Խű�
extern const unsigned RELOAD_ONESCRIPT = 0x00008000;	//�����ű���û��ʵ�֣���Ȼʹ����ʵ���÷�����

extern const unsigned RELOAD_GENVAR = 0x00010000;		//�ű�����
extern const unsigned RELOAD_REGIONLIST = 0x00020000;	//�����б�
extern const unsigned RELOAD_REGIONSETUP = 0x00040000;	//��������
extern const unsigned RELOAD_ONEREGION = 0x00080000;	//��������*.rs��û��ʵ�֣���Ȼʹ����ʵ���÷�����

extern const unsigned RELOAD_ALLREGION = 0x00100000;	//���г���*.rs
extern const unsigned RELOAD_FACTION = 0x00200000;		//�������
extern const unsigned RELOAD_FACTIONWAR = 0x00400000;	//���ս
extern const unsigned RELOAD_QUEST = 0x00800000;		//����

extern const unsigned RELOAD_CONTRIBUTE = 0x01000000;	//���ҹ��ױ�����Ʒ

// end define

void reload_profiles()
{
	// �ض������
	if( !RELOAD_PROFILE_FLAGS ) return;

	// ��λ��⣬�Ƿ���Ҫ�ض������Ϊ1ʱ���������־λ��
#define TEST(x,y) \
	(x)&(y)?((x)&=~(y)),1:0
#undef TEST
}


// ������Ϸ�ͻ�������
BOOL CreateGame()
{
	g_pGame = new CGame;
	return true;
}

// �ͷ���Ϸ�ͻ�������
BOOL DeleteGame()
{
	if (g_pGame)
	{
		delete g_pGame;
		g_pGame = NULL;
	}
	return true;
}

// ��ȡ��Ϸ�ͻ�������
CGame* GetGame()
{
	return g_pGame;
}

const char* CGame::GetStatckFileName()	
{
	DWORD lThreadID = ::GetCurrentThreadId();
	sprintf(m_pszThreadRunStackFile, "[%d]_%s", lThreadID, m_pszRunStackFile);
	return m_pszThreadRunStackFile;
}

CGame::CGame(void)
:p_DBEntityManager(NULL),m_bIsSaveStartFlag(false)
{
	m_lWSSocket = 0;
	p_DBEntityManager = new CEntityManager;

	s_lAITick = 0;

	s_pNetServer = NULL;

	m_pRsEntity = NULL;

	m_pDupliRegionSetup = NULL;

	m_pMapFile = NULL;

	GetSetup()->strLoginIP = "127.0.0.1";
	GetSetup()->dwLoginPort = 2345;
	GetSetup()->dwListenPort = 8100;	

	m_dwLastReLoginTime = timeGetTime();

	m_bInPing = false;
	m_dwLastPingGameServerTime = timeGetTime();

	memset(m_pszThreadRunStackFile, 0, sizeof(m_pszThreadRunStackFile));
}

CGame::~CGame(void)
{
	SAFE_DELETE(p_DBEntityManager);
}

BOOL CGame::InitStackFileName()
{
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	_snprintf(m_pszRunStackFile,MAX_PATH,"RunStackInfo[%02d_%02d_%02d_%02d_%02d].txt",stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);	
	return TRUE;
}

BOOL CGame::InitMainStackFileName()
{
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	_snprintf(m_pszMainRunStackFile,MAX_PATH,"MainRunStackInfo[%02d_%02d_%02d_%02d_%02d].txt",stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);	
	return TRUE;
}
CEntityManager* CGame::GetEntityManager(void)
{
	return p_DBEntityManager;
}
bool CGame::LoadServerResource()
{
	CHAR* strPath = new CHAR[MAX_PATH];
	string strWorpath("");
	if(::GetCurrentDirectory(MAX_PATH,strPath))
	{
		strWorpath=string(strPath);
	}
	delete []strPath;

	// ��ʼ�����ݰ�
	string strFilesInfo("PackRes.cfg");	
	CClientResource::InitialInstance();
	CClientResource* pClientResource = CClientResource::GetInstance();
	pClientResource->Initial(GAME_RES,strWorpath,strFilesInfo);
	if(!pClientResource->LoadEx())
	{
		//װ�ؿͻ�����Դ��Ϣ����
	}
	AddLogText("Load package file OK!");
	return true;
}

bool CGame::LoadSetup()
{
	// ��ȡȫ������
	string strTemp;
	ifstream stream;
	stream.open("setup.ini");
	if( stream.is_open() )
	{
		stream  >> strTemp >> GetSetup()->strLoginIP
			>> strTemp >> GetSetup()->dwLoginPort
			>> strTemp >> GetSetup()->dwListenPort
			>> strTemp >> GetSetup()->strSqlConType
			>> strTemp >> GetSetup()->strSqlServerIP
			>> strTemp >> GetSetup()->strSqlUserName
			>> strTemp >> GetSetup()->strSqlPassWord
			>> strTemp >> GetSetup()->strDBName; //end ;			   

		stream.close();

		strTemp = "DBServer";
		strTemp += "[";
		strTemp += GetSetup()->strDBName;
		strTemp += "]-StarDust";
		
		//�鿴�˳����Ƿ���������
		HWND hWnd = FindWindow(NULL,strTemp.c_str());
		if (hWnd != NULL)
		{
			char strMsgBox[256]="";
			sprintf(strMsgBox,"%s ��������������!",strTemp.c_str());
			MessageBox(g_hWnd,_T(strMsgBox), "error", MB_OK);
			return false;
		}

		SendMessage(g_hWnd, WM_SETTEXT, 0, (LPARAM)strTemp.c_str() );
	}

	else	// �Ӽ����ļ���ȡ
	{
		MessageBox(g_hWnd, "Can't find file setup.ini", "message", MB_OK);
		return false;
	}

	ifstream streamSetupEx;
	streamSetupEx.open("SetupEx.ini");
	if( streamSetupEx.is_open() )
	{
		streamSetupEx >> strTemp >> GetSetup()->dwDataBlockNum
			>> strTemp >> GetSetup()->dwDataBlockSize
			>> strTemp >> GetSetup()->dwFreeMsgNum
			>> strTemp >> GetSetup()->dwFreeSockOperNum
			>> strTemp >> GetSetup()->dwFreeIOOperNum
			>> strTemp >> GetSetup()->lIOOperDataBufNum			
			>> strTemp >> GetSetup()->bCheckNet
			>> strTemp >> GetSetup()->dwBanIPTime
			>> strTemp >> GetSetup()->dwMaxMsgLen			
			>> strTemp >> GetSetup()->lMaxConnectNum
			>> strTemp >> GetSetup()->lMaxClientsNum
			>> strTemp >> GetSetup()->lPendingWrBufNum
			>> strTemp >> GetSetup()->lPendingRdBufNum
			>> strTemp >> GetSetup()->lMaxSendSizePerSecond
			>> strTemp >> GetSetup()->lMaxRecvSizePerSecond
			>> strTemp >> GetSetup()->lMaxBlockedSendMsgNum
			>> strTemp >> GetSetup()->lConPendingWrBufNum
			>> strTemp >> GetSetup()->lConPendingRdBufNum
			>> strTemp >> GetSetup()->lConMaxSendSizePerSecond
			>> strTemp >> GetSetup()->lConMaxRecvSizePerSecond
			>> strTemp >> GetSetup()->lConMaxBlockedSendMsgNum

			>> strTemp >> GetSetup()->dwRefeashInfoTime
			>> strTemp >> GetSetup()->dwSaveInfoTime
			
			>>strTemp>>GetSetup()->dwTravleMaxSize
			>>strTemp>>GetSetup()->dwPlayerLoadThreadNumber
			>>strTemp>>GetSetup()->dwPlayerSaveThreadNumber

			>>strTemp>>GetSetup()->dwPingWorldServerTime
			>>strTemp>>GetSetup()->dwPingWorldServerErrorTime
			>>strTemp>>GetSetup()->dwClearPlayerDataTime
			>>strTemp>>GetSetup()->dwViewDbOperDeltaTime;
		streamSetupEx.close();
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file SetupEx.ini", "message", MB_OK);
		return false;
	}

	return true;
}

// ��Ϸ��ʼ��
BOOL CGame::Init()
{
	srand((unsigned)time(NULL));
	random(100);
	CreateDirectory( "log",	NULL);

#ifdef _RUNSTACKINFO_
	InitStackFileName();
	InitMainStackFileName();
#endif	

	PutDebugString("DBServer start!");

	//װ����Դ
	LoadServerResource();

	//��ȡȫ���ַ���
	bool ret = CStringReading::InitUniString("textresource/globaltextres.xml","GlobalFile");
	if (LoadSetup()==false)
	{
		return false;
	}
	if( !LoadWSSetup("WorldInfoSetup.ini") )
	{
		return false;
	}
	

	//�������ݿ�
	m_pDBAllocator = new CDataBlockAllocator(m_Setup.dwDataBlockNum,m_Setup.dwDataBlockSize);

	// ����ʹ��ADO�ӿڳ�ʼConnection string
	bool isADOConnected = CMyAdoBase::Initialize(GetSetup()->strSqlConType, GetSetup()->strSqlServerIP, GetSetup()->strDBName,
		GetSetup()->strSqlUserName, GetSetup()->strSqlPassWord);

	// [DB Connection Thread]
	GetGame()->GetEntityManager()->InitDBThread(GetSetup()->strSqlConType, GetSetup()->strSqlServerIP, GetSetup()->strDBName,
		GetSetup()->strSqlUserName, GetSetup()->strSqlPassWord);


	m_pRsEntity=new CRsEntity;
	
	ReLoad("GlobeSetup");

	CMessage::Initial(m_pDBAllocator,m_Setup.dwFreeMsgNum);
	// ��ʼ������������
	CMySocket::MySocketInit();
	
	//װ���ڴ�Ӱ���ļ�����
	CSMClient::GetSM()->LoadMapFileSetup(m_pDBAllocator);
	// ������������
	if( !InitNetServer() )
	{
		MessageBox(g_hWnd, "Can't init NetServer!", "Message", MB_OK);
		return false;
	}

	if(!StartAccept())
	{
		return false;
	}

	//�ͷ���Դ������
	CClientResource* pClientResource = CClientResource::GetInstance();
	pClientResource->Release();
	CClientResource::ReleaseInstance();

#ifdef _THREAD_
	// ������־�߳�
	unsigned threadID;
	g_hWriteLogThread = (HANDLE)_beginthreadex(NULL,0,ProcessWriteLogDataFunc,NULL,0,&threadID);
#endif
	CSMClient::GetSM()->InitOk();
	return true;
}
// �ض�����
BOOL CGame::ReLoad(const char* name, bool bSendToGameServer,bool bReloadFileInfo)
{
	BOOL ret=false;

	//�ض��ļ�֮ǰ���ض���Դ��Ϣ�б�
	if(bReloadFileInfo)
		LoadServerResource();

	if (stricmp(name, "GlobeSetup")==0)
	{
		
	}
	else if (stricmp(name, "GameSetup")==0)
	{

	}

	return ret;
}

// ��Ϸ�ͷ�
BOOL CGame::Release()
{
	PutDebugString("DBServer Exiting...");

#ifdef _RUNSTACKINFO_
	EndStackInfo();
#endif

	SAFE_DELETE(p_DBEntityManager);

	CStringReading::Release();
	CSMClient::GetSM()->ExitOk();
	// �ر���������
	if( GetNetServer() )
	{
		s_pNetServer->DisConn(m_lLogingSocket);
		s_pNetServer->DisConn(m_lCostSocket);
		s_pNetServer->Exit();

		SAFE_DELETE(s_pNetServer);
	}
	
	WaitForSingleObject(g_hSavingThread, INFINITE);
	CloseHandle(g_hSavingThread);

	CWordsFilter::GetInstance()->Release();
	GetTimer()->Release();

	SAFE_DELETE(s_pNetServer);
	SAFE_DELETE(m_pRsEntity);

	CMySocket::MySocketClearUp();
	CSMClient::GetSM()->ReleaseSM();
	CMessage::Release();

	GetNetSessionManager()->Release();

	// �ȴ���־�߳̽���
	g_bWriteLogThreadExit = true;
	WaitForSingleObject(g_hWriteLogThread, INFINITE);
	CloseHandle(g_hWriteLogThread);

	// �ر�������ݿ������߳�
	g_vPlayerLoadThreadFlag = 0;
	for(DWORD i=0; i<g_vPlayerLoadThreads.size(); ++i)
	{
		CloseHandle(g_vPlayerLoadThreads[i]);
	}

	SAFE_DELETE(m_pDupliRegionSetup);

	SAFE_DELETE(m_pDBAllocator);

	CMyAdoBase::Uninitalize();
	PutDebugString("DBServer Exited!");
	return true;
}

BOOL CGame::InitNetServer(void)
{
	s_pNetServer = new CServer;
	//��Ϊ�������������
	long lRet = s_pNetServer->Start(2,m_pDBAllocator,
		m_Setup.dwFreeSockOperNum,m_Setup.dwFreeIOOperNum,m_Setup.lIOOperDataBufNum,
		m_Setup.bCheckNet,m_Setup.dwBanIPTime,
		m_Setup.dwMaxMsgLen,m_Setup.lMaxConnectNum,m_Setup.lMaxClientsNum,
		m_Setup.lPendingWrBufNum,m_Setup.lPendingRdBufNum,
		m_Setup.lMaxSendSizePerSecond,m_Setup.lMaxRecvSizePerSecond,
		m_Setup.lMaxBlockedSendMsgNum);
	if( !lRet )
	{
		MessageBox(g_hWnd, "Start NetServer Error", "Message", MB_OK);
		return FALSE;
	}
	CMessage::SetServer(s_pNetServer);
	AddLogText("��������ʼ���ɹ� !");
	return TRUE;
}

//��ʼ��Ϊ����������
bool CGame::StartAccept()
{
	if(NULL == s_pNetServer)	return false;
	BOOL bRet = s_pNetServer->Host(m_Setup.dwListenPort,NULL,NF_Server_Client,SOCK_STREAM);
	if(!bRet)
	{
		MessageBox(g_hWnd, "Start Host Error", "Message", MB_OK);
		return false;
	}
	AddLogText("DBServer��������ʼ���ɹ� !");
	return true;
}


// ��ϷAI
BOOL CGame::AI()
{
	return TRUE;
}

long g_lRebackMessageCount = 0;
// ��Ϣ����
BOOL CGame::ProcessMessage()
{
	g_lCurTime = timeGetTime();
	long lMessageCount = GetNetServer()->GetRecvMessages()->GetSize();
	for(int i=0; i<lMessageCount; i++)
	{
		CMessage* pMsg = (CMessage*)GetNetServer()->GetRecvMessages()->PopMessage();
		if( pMsg )
		{
			pMsg->Run();
			CBaseMessage::FreeMsg(pMsg);
		}
	}
	CSMClient::GetSM()->ProcessMessage();
	return TRUE;
}
void CGame::ProcessData()
{

}

// ��Ϸ��ѭ��
BOOL CGame::MainLoop()
{
	// ��ʱ���������ջ��¼�ļ�
#ifdef _RUNSTACKINFO_
		CMessage::AsyClearFile(GetGame()->GetMainStatckFileName());
#endif

	// ͳ��ÿ������ĺ�ʱ
	static DWORD dwRefeashTextTime = 0,
		dwAITime = 0,
		dwMessageTime = 0,
		dwNetSessionTime = 0,
		dwFactionWarSysTime = 0,
		dwTimerTime = 0,
		dwProcessPlayerDataQueueTime = 0,
		dwSessionFactoryTime = 0,
		dwSavePointTime = 0;

	static DWORD dwCurTime = 0;

	static DWORD ai_tick, now, old = timeGetTime(), dwAnsyFileClearTime = old, dwClearPlayerDataTime=old, lastSaveTime=old, lastRefeashInfoTime = now, lastReleaseLoginPlayerTime = 0;
	now = timeGetTime();
	
	static bool bStart = true;
	if (bStart)
	{
		CSMClient::GetSM()->SendAITick(true);
		bStart = false;
	}

	ai_tick++;
	s_lAITick++;
	// ��ʱ���������ջ��¼�ļ�
#ifdef _DEBUG_SM_INFO_
	if(now - dwAnsyFileClearTime >= 3600 * 1000 * 1) // 1 Сʱ���һ��
	{
		dwAnsyFileClearTime = now;
		CMessage::AsyClearFile(GetGame()->GetStatckFileName());
	}
#endif
	// �Ʒ�
	static DWORD LastCostTime=0;
	static DWORD LastLoadLargessTime=0;
	static DWORD LastTimeLoadCardLargess=0;
	static DWORD LastTimeCardLoop=0;

	if( now - old > 600000 )
	{
		AddLogText("600 ������� %d ��AI\r\n\
AI:%d\r\n\
RefeashText:%d\r\n\
Message:%d (LS:%d  GS:%d  CS:%d)\r\n\
NetSession:%d\r\n\
FactionWarSys:%d\r\n\
Timer:%d\r\n\
ProcessPlayerDataQueue:%d\r\n\
SessionFactory:%d\r\n\
SavePoint:%d\r\n",

		ai_tick,
		dwRefeashTextTime,
		dwAITime,
		dwMessageTime, g_lLSMessageTime, g_lGSMessageTime, g_lCSMessageTime,
		dwNetSessionTime,
		dwFactionWarSysTime,
		dwTimerTime,
		dwProcessPlayerDataQueueTime,
		dwSessionFactoryTime,
		dwSavePointTime);

		dwRefeashTextTime = 0;
		dwAITime = 0;
		dwMessageTime = 0; g_lLSMessageTime = 0; g_lGSMessageTime = 0; g_lCSMessageTime = 0;
		dwNetSessionTime = 0;
		dwFactionWarSysTime = 0;
		dwTimerTime = 0;
		dwProcessPlayerDataQueueTime = 0;
		dwSessionFactoryTime = 0;
		dwSavePointTime = 0;

		ai_tick = 0;
		old = now;
	}

	dwCurTime = timeGetTime();
	ProcessMessage();
	dwMessageTime += timeGetTime() - dwCurTime;

	GetEntityManager()->TravelForGenerateThreadData();
	
	//##�Ự
	dwCurTime = timeGetTime();
	dwSessionFactoryTime += timeGetTime() - dwCurTime;

	//ʱ���ʱ��
	dwCurTime = timeGetTime();
	GetTimer()->Run();
	dwTimerTime += timeGetTime() - dwCurTime;

	//����Ự
	dwCurTime = timeGetTime();
	GetNetSessionManager()->Run();
	dwNetSessionTime += timeGetTime() - dwCurTime;

	char szOutPutStr[1024];
#ifdef _RUNSTACKINFO_
	sprintf(szOutPutStr, "ProcessData() Start.");
	CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif
	// �����̲߳��������ݲ�������Ϣ
	GetEntityManager()->ProcessRetThreadData();
#ifdef _RUNSTACKINFO_
	sprintf(szOutPutStr, "ProcessData() End.");
	CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif

	// ��ʱ����������
	if (now - dwClearPlayerDataTime > GetSetup()->dwClearPlayerDataTime)
	{
		dwClearPlayerDataTime = now;

#ifdef _RUNSTACKINFO_
		char szOutPutStr[1024];
		sprintf(szOutPutStr, "��ʱ����������() Start.");
		CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif
		// ִ�����
		CGUID pGUID;
		long dbPlayerNum = 0;
		CEntityManager::AccountMapItr deleteItr = GetEntityManager()->GetAccountMap().begin();
		while(deleteItr != GetEntityManager()->GetAccountMap().end())
		{
			if( (deleteItr->second->GetLastLoginTime() != -1)
				&& ((now-deleteItr->second->GetLastLoginTime()) >= GetSetup()->dwClearPlayerDataTime) ) // �������ʱ���
			{
				bool isWaitForSaveFlag = false;
				if(deleteItr->second->GetEntityGroupMap().size() < 1)
				{
					AddLogText("MainLoop() Account[%s]��Group����Ϊ��!", deleteItr->first.c_str());
					deleteItr++;
					continue;
				}
				CEntityGroup* pLoginPlayerGroup = NULL;
				pLoginPlayerGroup = (CEntityGroup*)((deleteItr->second->GetEntityGroupMap().begin())->second);

				isWaitForSaveFlag = GetEntityManager()->FindAccountIsInOperQueue(deleteItr->first);
				// �ж��Ƿ��ڲ���������
				if(!isWaitForSaveFlag)
				{
					if(pLoginPlayerGroup)
					{
						map<CGUID, CBaseEntity*>::iterator playeritr = pLoginPlayerGroup->GetEntityGroupMap().begin();
						for(; playeritr != pLoginPlayerGroup->GetEntityGroupMap().end(); playeritr++)
						{
							isWaitForSaveFlag = GetEntityManager()->FindEntityIsInOperQueue(playeritr->first);
							if(isWaitForSaveFlag)
								break;
						}
					}
				}

				if(isWaitForSaveFlag)
				{
					++deleteItr;
					continue;
				}

				if(pLoginPlayerGroup)
				{
					map<CGUID, CBaseEntity*>::iterator guidItr = pLoginPlayerGroup->GetEntityGroupMap().begin();
					while(guidItr != pLoginPlayerGroup->GetEntityGroupMap().end())
					{
						GetEntityManager()->DelOneRootEntity(string("[player]"), guidItr->first);// ���DBPlayer
						guidItr++;
						dbPlayerNum++;
					}
				}

				// ��ȫ���,ɾ�����˺Ŷ���
				delete deleteItr->second;
				deleteItr = GetEntityManager()->GetAccountMap().erase(deleteItr);
				dbPlayerNum++;
				continue;
			}
			
			deleteItr++;
		}
		// ���DBEntity
#ifdef _RUNSTACKINFO_
		sprintf(szOutPutStr, "��ʱ����������[%d]��() End.", dbPlayerNum);
		CMessage::AsyWriteFile(GetGame()->GetMainStatckFileName(),szOutPutStr);
#endif
	}

	// ˢ����ʾ
	dwCurTime = timeGetTime();
	if (now - lastRefeashInfoTime > GetSetup()->dwRefeashInfoTime)
	{
		lastRefeashInfoTime = now;
		lMapAccounts = 0;
		lMapAccounts += GetEntityManager()->GetAccountMap().size();
		CEntityManager::DBEntityComponentMapItr  enGroupItr = GetEntityManager()->GetBaseEntityMap().find(std::string("[player]"));
		if( enGroupItr != GetEntityManager()->GetBaseEntityMap().end() )
		{
			lMapPlayers = 0;
			lMapPlayers += enGroupItr->second.size();
		}

		enGroupItr = GetEntityManager()->GetBaseEntityMap().find(std::string("[scriptvargroup]"));
		if( enGroupItr != GetEntityManager()->GetBaseEntityMap().end() )
		{
			map<CGUID, CEntityGroup*>::iterator leafit = enGroupItr->second.begin();
			for(; leafit != enGroupItr->second.end(); leafit++)
			{
				if(leafit->second)
				{
					lMapScripts = 0;
					lMapScripts += leafit->second->GetEntityGroupMap().size();
				}
			}
		}

		enGroupItr = GetEntityManager()->GetBaseEntityMap().find(std::string("[procedure]"));
		if( enGroupItr != GetEntityManager()->GetBaseEntityMap().end() )
		{
			lMapProcs = 0;
			lMapProcs += enGroupItr->second.size();
		}

		enGroupItr = GetEntityManager()->GetBaseEntityMap().find(std::string("[factiongroup]"));
		if( enGroupItr != GetEntityManager()->GetBaseEntityMap().end() )
		{
			map<CGUID, CEntityGroup*>::iterator leafit = enGroupItr->second.begin();
			for(; leafit != enGroupItr->second.end(); leafit++)
			{
				if(leafit->second)
				{
					lFactions = 0;
					lFactions += leafit->second->GetEntityGroupMap().size();
				}
			}
		}

		enGroupItr = GetEntityManager()->GetBaseEntityMap().find(std::string("[friendgroup]"));
		if( enGroupItr != GetEntityManager()->GetBaseEntityMap().end() )
		{
			lFriendGroup = 0;
			lFriendGroup += enGroupItr->second.size();
		}

		lOperingLoadEntitys = GetEntityManager()->GetLoadWaitOperEntityNum();
		lOperingSaveEntitys = GetEntityManager()->GetSaveWaitOperEntityNum();

		RefeashInfoText();
	}
	dwRefeashTextTime += timeGetTime() - dwCurTime;

	// ����ȫ������
	if(g_bSaveNowData == true)
	{
		GetEntityManager()->SaveAllEntity();
		g_bSaveNowData = false;
	}

	if(GetSaveStartFlag())
	{
		if(	   GetEntityManager()->GetLoadWaitOperEntityNum() == 0 
			&& GetEntityManager()->GetSaveWaitOperEntityNum() == 0 
			&& GetEntityManager()->GetBeginLoadEntityNum()    == 0
			&& GetEntityManager()->GetBeginSaveEntityNum()    == 0 )
		{
			AddLogText("��������������ɣ�");
			SetSaveStartFlag(false);
		}
	}
	// ���ȫ������
	if(g_bDelPlayers == true)
	{
		GetEntityManager()->ClearEntitys();
		g_bDelPlayers = false;
	}
	return true;
}
bool CGame::LoadWSSetup(const char *filename)
{
	CRFile* prfile = rfOpen(filename);
	if( !prfile )
	{
		char str[256];
		sprintf(str, "Can't find file %s", filename);
		MessageBox(g_hWnd, str, "message", MB_OK);
		return false;
	}

	string strTemp;
	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	// ��WS��������Ϣ
	tagWorldServer stWS;
	long lWorldServerNum = 0;
	stream >> strTemp >> lWorldServerNum;
	for(int i=0; i<lWorldServerNum; i++)
	{
		ReadTo(stream, "#");
		stream >> stWS.dwIndex >> stWS.strIP >> stWS.dwPort;
		stWS.bConnected = false;
		s_mapWorldServer[stWS.dwIndex] = stWS;
	}
	AddLogText("DBS���������ö�ȡ���");
	return true;
}

long CGame::GetConnectedWorldServerCount()
{
	long lCnt=0;
	map<DWORD,tagWorldServer>::iterator it=s_mapWorldServer.begin();
	for(;it!=s_mapWorldServer.end();it++)
	{
		if(it->second.bConnected)
		{
			lCnt++;
		}
	}
	return lCnt;
}


CGame::tagWorldServer* CGame::GetWorldServer(const char* strIP, DWORD dwPort)
{
	for(map<DWORD, tagWorldServer>::iterator it=s_mapWorldServer.begin(); it!=s_mapWorldServer.end(); it++)
	{
		tagWorldServer *p = &it->second;
		if (p->strIP==strIP && p->dwPort==dwPort)
		{
			return p;
		}
	}
	return NULL;
}


//��ĳ��WorldServer�Ͽ����ӵ�ʱ��
bool CGame::OnWorldServerClose(DWORD dwIndex)
{
	tagWorldServer* pWorldServer = GetWorldServer(dwIndex);
	if (pWorldServer)
	{
		pWorldServer->bConnected = false;
		// ɾ��Player�б�
		char str[256];
		sprintf(str, "%s [%d]", pWorldServer->strIP.c_str(), pWorldServer->dwPort);
		long lPos = (long)SendMessage(g_hGameServerListBox, LB_FINDSTRING, 0, (LPARAM)str);
		if( lPos != LB_ERR )
		{
			SendMessage(g_hGameServerListBox,LB_DELETESTRING, lPos, 0);
		}

		AddLogText("WorldServer %s [%d] lost!", pWorldServer->strIP.c_str(), pWorldServer->dwPort);

		// ����������
		CEntityManager::DBEntityComponentMapItr  enGroupItr = GetEntityManager()->GetBaseEntityMap().find(string("[factiongroup]"));
		if( enGroupItr != GetEntityManager()->GetBaseEntityMap().end() )
		{
			map<CGUID, CEntityGroup*>::iterator facItr = enGroupItr->second.begin();
			for(; facItr != enGroupItr->second.end(); facItr++)
			{
				delete facItr->second;
			}
			GetEntityManager()->GetBaseEntityMap().erase(enGroupItr);
			AddLogText("WS�����������������!");
		}
	}
	else
	{
		return false;
	}

	return true;
}

//! [2007-7-9 add end]
void CGame::SendMsg2WS(DWORD dwIndex,CMessage* pMsg)
{
	if(pMsg)
	{
		pMsg->SendWSID(dwIndex);
	}
}

//! [2007-7-9 add]
void CGame::SendAllWS(CMessage* pMsg)
{
	if(pMsg)
	{
		map<DWORD, tagWorldServer>::iterator iteGS = s_mapWorldServer.begin();
		for (; iteGS != s_mapWorldServer.end(); ++iteGS)
		{
			long dwIndex = iteGS->first;
			pMsg->SendWSID(dwIndex);
		}
	}
}
// �������ݿ�������Ϣ
void CGame::SendDBTableInfo(long socketID)
{
	CMessage msg(MSG_DB2W_SERVER_SENDSETUP);
	
	long objAttrDefNum = GetEntityManager()->GetObjAttrDef().size();
	msg.Add(objAttrDefNum);
	vector<BYTE> pBA;
	std::map<string, tagEntityBuildInfo*>::iterator attrItr = GetEntityManager()->GetObjAttrDef().begin();
	for(; attrItr != GetEntityManager()->GetObjAttrDef().end(); attrItr++)
	{
		attrItr->second->AddToByteArray(pBA);
	}
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], pBA.size());
	msg.SendToSocket(socketID);
}
//! [2007-7-9 add end]


void CGame::Test3GoodsImageDataBuild()
{
	_RecordsetPtr rs;

	CMyAdoBase::CreateRs(rs);

	long lTranNumber=0;
	
	try
	{						
		CMyAdoBase::OpenRs("SELECT * FROM TBL_PLAYER_GOODS", rs, GetEntityManager()->GetAccessThread()->GetConnPtr(),adOpenDynamic,adLockOptimistic);
		
		DWORD dwRecordNum=1;
		while( !rs->GetadoEOF() )
		{			
			long nHoleNum=(int)rs->GetCollect("holeNum");
			long nHoleIdx1=(int)rs->GetCollect("hole1Idx");
			long nHoleIdx2=(int)rs->GetCollect("hole2Idx");
			long nHoleIdx3=(int)rs->GetCollect("hole3Idx");
			long nHoleIdx4=(int)rs->GetCollect("hole4Idx");
			long nHoleIdx5=(int)rs->GetCollect("hole5Idx");
			

			long lSize=0;
			FieldsPtr fldPtr = rs->GetFields();
			FieldPtr itemPtr = fldPtr->GetItem("AddonProperty");
			lSize = itemPtr->ActualSize;

			if(lSize > 0)
			{
				_variant_t varBLOB;
				varBLOB = itemPtr->GetChunk(lSize);

				byte *pBuf=NULL;
				SafeArrayAccessData(varBLOB.parray,(void**)&pBuf);
				SafeArrayUnaccessData(varBLOB.parray);

				//�����µ�SIZE
				long nMakerNameLen=0;
				long nAddonSize=sizeof(long)*2 + sizeof(long)*5;
				long nNewDataSize=lSize+nAddonSize;		
				byte* pNewDataBuffer=new byte[nNewDataSize];
				ZeroMemory(pNewDataBuffer,nNewDataSize);
				memcpy(pNewDataBuffer,pBuf,lSize);
				long loffset=lSize;
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nMakerNameLen,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleNum,sizeof(long));
				loffset+=sizeof(long);

				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx1,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx2,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx3,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx4,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx5,sizeof(long));

				VARIANT varBLOBWrite;
				SAFEARRAY *psa = 0;
				SAFEARRAYBOUND rgsabound[1];
				rgsabound[0].lLbound = 0;				
				rgsabound->cElements = nNewDataSize;
				psa = SafeArrayCreate(VT_UI1, 1, rgsabound);					
				for(long i=0; i<nNewDataSize; i++)
					SafeArrayPutElement(psa, &i, &pNewDataBuffer[i]);
				varBLOBWrite.parray = psa;
				varBLOBWrite.vt = VT_ARRAY | VT_UI1;
				itemPtr->AppendChunk(varBLOBWrite);
				
				SAFE_DELETE_ARRAY(pNewDataBuffer);
				SafeArrayDestroy(psa);										
			}
			
			rs->MoveNext();
			dwRecordNum++;
		}
		
		string strMsg="�ɹ�������ƷIMAGE���ݿ飡";
		
		MessageBox(NULL,strMsg.c_str(),"OK",MB_OK);
		
	}
	catch (_com_error &e)
	{
		if(lTranNumber)
		{
			
		}
		char buf[1024];
		string strErr="";
		char* msg="-_-!";

		if (e.Error() == 0x80004005)
		{
			wsprintf( buf, "%s (һ�����������): ����\r\n:%s", msg, (LPCSTR)((_bstr_t)e.Description()));
		}
		else
		{
			wsprintf( buf, "%s\r\n \
						   Code = %08lx\r\n \
						   Cnode meaning = %d\r\n \
						   Source = %s\r\n \
						   Description = %s\r\n",
						   msg ,e.Error(), e.ErrorMessage(), (LPCSTR)((_bstr_t)e.Source()), (LPCSTR)((_bstr_t)e.Description()));
			strErr=buf;
			strErr=strErr.substr(30);
		}
		MessageBox(NULL,"������ƷIMAGE���ݿ����","����",MB_OK);
		
	}
}

void CGame::Test3MailGoodsDataBuild()
{
	_RecordsetPtr rs;

	CMyAdoBase::CreateRs(rs);

	long lTranNumber=0;

	try
	{						
		CMyAdoBase::OpenRs("SELECT * FROM TBL_MAIL_GOODS", rs, GetEntityManager()->GetAccessThread()->GetConnPtr(),adOpenDynamic,adLockOptimistic);

		
		DWORD dwRecordNum=1;
		while( !rs->GetadoEOF() )
		{			
			long nHoleNum=(int)rs->GetCollect("holeNum");
			long nHoleIdx1=(int)rs->GetCollect("hole1Idx");
			long nHoleIdx2=(int)rs->GetCollect("hole2Idx");
			long nHoleIdx3=(int)rs->GetCollect("hole3Idx");
			long nHoleIdx4=(int)rs->GetCollect("hole4Idx");
			long nHoleIdx5=(int)rs->GetCollect("hole5Idx");


			long lSize=0;
			FieldsPtr fldPtr = rs->GetFields();
			FieldPtr itemPtr = fldPtr->GetItem("AddonProperty");
			lSize = itemPtr->ActualSize;

			if(lSize > 0)
			{
				_variant_t varBLOB;
				varBLOB = itemPtr->GetChunk(lSize);

				byte *pBuf=NULL;
				SafeArrayAccessData(varBLOB.parray,(void**)&pBuf);
				SafeArrayUnaccessData(varBLOB.parray);

				//�����µ�SIZE
				long nMakerNameLen=0;
				long nAddonSize=sizeof(long)*2 + sizeof(long)*5;
				long nNewDataSize=lSize+nAddonSize;		
				byte* pNewDataBuffer=new byte[nNewDataSize];
				ZeroMemory(pNewDataBuffer,nNewDataSize);
				memcpy(pNewDataBuffer,pBuf,lSize);
				long loffset=lSize;
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nMakerNameLen,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleNum,sizeof(long));
				loffset+=sizeof(long);

				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx1,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx2,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx3,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx4,sizeof(long));
				loffset+=sizeof(long);
				memcpy(&(((byte*)pNewDataBuffer)[loffset]),&nHoleIdx5,sizeof(long));

				VARIANT varBLOBWrite;
				SAFEARRAY *psa = 0;
				SAFEARRAYBOUND rgsabound[1];
				rgsabound[0].lLbound = 0;				
				rgsabound->cElements = nNewDataSize;
				psa = SafeArrayCreate(VT_UI1, 1, rgsabound);					
				for(long i=0; i<nNewDataSize; i++)
					SafeArrayPutElement(psa, &i, &pNewDataBuffer[i]);
				varBLOBWrite.parray = psa;
				varBLOBWrite.vt = VT_ARRAY | VT_UI1;
				itemPtr->AppendChunk(varBLOBWrite);

				SAFE_DELETE_ARRAY(pNewDataBuffer);
				SafeArrayDestroy(psa);										
			}

			rs->MoveNext();
			dwRecordNum++;
		}
		
		string strMsg="�ɹ������ʼ���ƷIMAGE���ݿ飡";

		MessageBox(NULL,strMsg.c_str(),"OK",MB_OK);

	}
	catch (_com_error &e)
	{
		if(lTranNumber)
		{
			
		}
		char buf[1024];
		string strErr="";
		char* msg="-_-!";

		if (e.Error() == 0x80004005)
		{
			wsprintf( buf, "%s (һ�����������): ����\r\n:%s", msg, (LPCSTR)((_bstr_t)e.Description()));
		}
		else
		{
			wsprintf( buf, "%s\r\n \
						   Code = %08lx\r\n \
						   Cnode meaning = %d\r\n \
						   Source = %s\r\n \
						   Description = %s\r\n",
						   msg ,e.Error(), e.ErrorMessage(), (LPCSTR)((_bstr_t)e.Source()), (LPCSTR)((_bstr_t)e.Description()));
			strErr=buf;
			strErr=strErr.substr(30);
		}
		MessageBox(NULL,"�����ʼ���ƷIMAGE���ݿ����","����",MB_OK);

	}
}

// ��Ϸ���߳�
unsigned __stdcall GameThreadFunc(void* pArguments)
{
#ifndef _DEBUG
	__try
	{
#endif
		CreateGame();
		BOOL ret = GetGame()->Init();

		if (ret)
		{
			while(1)
			{
				// �����߳�
				if( g_bGameThreadExit )
				{
					break;
				}

				if( !GetGame()->MainLoop() )
				{
					break;
				}

				Sleep(1);
			}
		}

		if(GetGame())
			GetGame()->Release();
		DeleteGame();

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		PutDebugString("������ִ�����鿴���µĴ��󱨸��ļ�");
		AddErrorLogText("������ִ�����鿴���µĴ��󱨸��ļ�");

		if(GetGame())
			GetGame()->Release();
		DeleteGame();

		Sleep(5000);

		WinExec("DBServer.exe", SW_SHOW);

		PostMessage(g_hWnd, WM_DESTROY, 0, 0);
		SetEvent(g_hGameThreadExitEvent);
		_endthreadex( 0 );

		return 0;
	}
#endif

	SetEvent(g_hGameThreadExitEvent);
	PostMessage(g_hWnd, WM_CLOSE, 1,0);

	_endthreadex( 0 );
	return 0;
}

unsigned __stdcall ProcessWriteLogDataFunc(void *pArguments)
{
return 0;
}

extern DWORD g_dwThisSaveStartTime;		// ���δ��̿�ʼʱ��

void DoSaveData()
{
	DWORD dwBeginTime = timeGetTime();
	g_dwThisSaveStartTime = dwBeginTime;		// ���δ��̿�ʼʱ��

	size_t nSavePlayerCnt = 0, 
		nCreationCnt = 0,
		nRestoreCnt = 0,
		nDeletionCnt = 0;
	long nTransactionCounter = 0;
	_ConnectionPtr cnSave;
}

void DoSaveLog()
{
	string cnstr;
	_ConnectionPtr cn;
}

