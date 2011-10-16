#include "StdAfx.h"
#include <sstream>

#include ".\game.h"


#include "..\nets\servers.h"
#include "..\..\..\dbaccess\myadobase.h"
#include "..\public\readwrite.h"
#include "..\public\BaseDef.h"
#include "..\AppLogin\SMClient.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

long g_lVersion =	10000;		// ����汾�ţ�ͬGameClientͳһ����һλ�ǲ��԰汾�ţ�����λ�ǰ汾

CGame* g_pGame= NULL;		// ��Ϸ�ͻ�������

HANDLE g_hGameThreadExitEvent=NULL;		// ���߳��˳��¼�
bool	g_bGameThreadExit=false;		// �Ƿ��˳����߳�

HANDLE	hASConnectThread=NULL;			// AS�����߳̾�� 
bool	bASConnectThreadExit=false;		// �Ƿ��˳�AS�����߳�


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


CGame::CGame(void)
:m_AsynchronousExecute(TRUE)
,m_pMapFile(NULL)
,m_bLoadNoQueueCDKeyList(FALSE)
,m_pPat(NULL)
,m_bReLoadLoginSetup(false)
,m_bReLoadIpLimitSetup(false)
#ifdef SHOW_LOGIN_MSG_20090817
,bOutInfo(false)
#endif
{
	s_lAITick = 0;

	s_pNetServer_Client = NULL;
	s_pNetServer_World = NULL;
	m_lASSocket = 0;
	m_bExit = false;
	
	m_StateLvl[0] = -1;
	m_StateLvl[1] = 10000;
	m_StateLvl[2] = 20000;
	m_StateLvl[3] = 30000;

	m_Setup.lClientMaxBlockConNum = 10;
	m_Setup.lClientValidDelayRecDataTime = 4000;

	m_Setup.lWorldMaxBlockConNum = 5;
	m_Setup.lWorldValidDelayRecDataTime = 4000;

}

CGame::~CGame(void)
{
}

bool CGame::LoadSetup()
{
	// ��������setup.ini
	string strTemp;
	string strSqlConType;//��������
	string strSqlIP;//���ݿ������IP
	string strSqlUserName;//���ݿ��û���
	string strSqlPassWord;//����
	string strDBName;//DATABASE NAME
	ifstream stream;
	stream.open("setup.ini");
	if( stream.is_open() )
	{
		stream  >> strTemp >> GetSetup()->dwListenPort_Client
			>> strTemp >> GetSetup()->dwListenPort_World

			>> strTemp >> GetSetup()->strAccountServerIp	//! ��֤������IP
			>> strTemp >> GetSetup()->dwAccountServerPort	//! ��֤�������˿�
			>> strTemp >> GetSetup()->dwAccountClientNo		//! ��֤�ͻ��˱��


			>> strTemp >> GetSetup()->strLocalSqlConType		//! �����˺���֤���ã�DB���
			>> strTemp >> GetSetup()->strLocalSqlServerIP
			>> strTemp >> GetSetup()->strLocalSqlUserName
			>> strTemp >> GetSetup()->strLocalSqlPassWord
			>> strTemp >> GetSetup()->strLocalDBName;

		stream.close();

		AddLogText("Load Setup.ini file...ok!");
	}

	// �Ӽ����ļ���ȡ
	else
	{
		MessageBox(g_hWnd, "Can't find file setup.ini", "message", MB_OK);
		return false;
	}

	ifstream streamSetupEx;
	streamSetupEx.open("SetupEx.ini");
	if( streamSetupEx.is_open() )
	{
		streamSetupEx>> strTemp >> GetSetup()->dwDataBlockNum
			>> strTemp >> GetSetup()->dwDataBlockSize
			>> strTemp >> GetSetup()->dwFreeMsgNum
			>> strTemp >> GetSetup()->dwFreeSockOperNum
			>> strTemp >> GetSetup()->dwFreeIOOperNum
			>> strTemp >> GetSetup()->lIOOperDataBufNum
			>> strTemp >> GetSetup()->lEncryptType
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
			>> strTemp >> GetSetup()->lClientMaxBlockConNum
			>> strTemp >> GetSetup()->lClientValidDelayRecDataTime

			>> strTemp >> GetSetup()->dwWorldFreeSockOperNum
			>> strTemp >> GetSetup()->dwWorldFreeIOOperNum
			>> strTemp >> GetSetup()->lWorldIOOperDataBufNum
			>> strTemp >> GetSetup()->bWorldCheckNet
			>> strTemp >> GetSetup()->dwWorldBanIPTime
			>> strTemp >> GetSetup()->dwWorldMaxMsgLen			
			>> strTemp >> GetSetup()->lWorldMaxConnectNum
			>> strTemp >> GetSetup()->lWorldMaxClientsNum
			>> strTemp >> GetSetup()->lWorldPendingWrBufNum
			>> strTemp >> GetSetup()->lWorldPendingRdBufNum
			>> strTemp >> GetSetup()->lWorldMaxSendSizePerSecond
			>> strTemp >> GetSetup()->lWorldMaxRecvSizePerSecond
			>> strTemp >> GetSetup()->lWorldMaxBlockedSendMsgNum
			>> strTemp >> GetSetup()->lWorldConPendingWrBufNum
			>> strTemp >> GetSetup()->lWorldConPendingRdBufNum
			>> strTemp >> GetSetup()->lWorldConMaxSendSizePerSecond
			>> strTemp >> GetSetup()->lWorldConMaxRecvSizePerSecond
			>> strTemp >> GetSetup()->lWorldConMaxBlockedSendMsgNum
			>> strTemp >> GetSetup()->lWorldMaxBlockConNum
			>> strTemp >> GetSetup()->lWorldValidDelayRecDataTime

			>> strTemp >> GetSetup()->dwRefeashInfoTime			// ˢ����ʾʱ��
			>> strTemp >> GetSetup()->dwSaveInfoTime				// ����һ����Ϣ��ʱ����
			>> strTemp >> GetSetup()->dwPingWorldServerTime
			>> strTemp >> GetSetup()->dwPingWorldServerErrorTime

			>> strTemp >> GetSetup()->dwMinDbOptThreadNum		//! �����˺���֤����, ��СDB�����̱߳�����
			>> strTemp >> GetSetup()->dwMaxDbOptThreadNum		//! �����˺���֤���ã����DB�����߳���
			>> strTemp >> GetSetup()->dwThreadNumAdjustSpace;	//! �����˺���֤���ã��Զ������߳����ļ��ʱ��

		streamSetupEx.close();

		m_StateLvl[0] = -1;
		m_StateLvl[1] = (long)(GetLoginSetup()->m_dwMaxWsMum * GetLoginSetup()->fWorldBusyScale);
		m_StateLvl[2] = (long)(GetLoginSetup()->m_dwMaxWsMum * GetLoginSetup()->fWorldFullScale);
		m_StateLvl[3] = GetLoginSetup()->m_dwMaxWsMum;

		ChangeAllWorldSate();
		AddLogText("Load SetupEx.ini file...ok!");
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file SetupEx.ini", "message", MB_OK);
		return false;
	}

	return LoadLoginSetup() && LoadIpLimitSetup();
}

bool CGame::LoadLoginSetup(VOID)
{
	string strTemp;
	ifstream streamSetupEx;
	streamSetupEx.open("loginSetup.ini");
	if( streamSetupEx.is_open() )
	{
		streamSetupEx
			>> strTemp >> GetLoginSetup()->bSndaValidate				//! �Ƿ�ʹ��ʢ��ʽ��֤
			>> strTemp >> GetLoginSetup()->bXmCardEnable				//! �Ƿ�ʹ�������ܱ���
			>> strTemp >> GetLoginSetup()->bSndaFcmEnable				//! �Ƿ�����ʢ�������
			>> strTemp >> GetLoginSetup()->bAuthenEnable                //�� ʢ�����ϲ��뿪��

			>> strTemp >> GetLoginSetup()->dwDoQueueInter				//! ����ȴ����еļ��ʱ��
			>> strTemp >> GetLoginSetup()->dwSendMsgToQueInter			//! ���ȴ����з�����Ϣ�ļ��ʱ��

			>> strTemp >> GetLoginSetup()->m_dwMaxWsMum					//!	WS�������
			>> strTemp >> GetLoginSetup()->m_dwProcessRankInterval		//!	�����ŶӼ�������룩
			>> strTemp >> GetLoginSetup()->m_dwSendQueueMsgInterval		//!	�����Ŷ���Ϣ��������룩
			>> strTemp >> GetLoginSetup()->m_dwOneProcessNum			//!	ÿ�δ���������ÿ��WS��

			>> strTemp >> GetLoginSetup()->fWorldBusyScale				//! ��æ����������
			>> strTemp >> GetLoginSetup()->fWorldFullScale				//! ��������������

			>> strTemp >> GetLoginSetup()->m_ValidateWaitTimeSpace		//!	��֤�ȴ�ʱ�䣨���룩
			>> strTemp >> GetLoginSetup()->m_SelectWaitTimeSpace		//!	ѡ�˽�����ʱ�䣨���룩
			>> strTemp >> GetLoginSetup()->m_JustOutTimeSpace			//!	С�˳�ʱʱ�䣨���룩

			>> strTemp >> GetLoginSetup()->m_MaxPwdErrCount				//!	���������������������
			>> strTemp >> GetLoginSetup()->m_PwdErrSaveTime				//!	��������������ʱ��(��)
			>> strTemp >> GetLoginSetup()->m_FrostCdkeyTime				//! �����½ʱ��(��)

			>> strTemp >> GetLoginSetup()->m_szCdkeyRegex				//! ��֤�˺��õ�������ʽ

			>> strTemp >> GetLoginSetup()->m_bValideClientResourceVersion//! �����½ʱ��(��)
			>> strTemp >> GetLoginSetup()->m_szClientResourceVersion;	//! �����½ʱ��(��)


		streamSetupEx.close();

		m_StateLvl[0] = -1;
		m_StateLvl[1] = (long)(GetLoginSetup()->m_dwMaxWsMum * GetLoginSetup()->fWorldBusyScale);
		m_StateLvl[2] = (long)(GetLoginSetup()->m_dwMaxWsMum * GetLoginSetup()->fWorldFullScale);
		m_StateLvl[3] = GetLoginSetup()->m_dwMaxWsMum;

		ChangeAllWorldSate();
		AddLogText("Load loginSetup.ini file...ok!");
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file loginSetup.ini", "message", MB_OK);
		return false;
	}

	return true;
}

bool CGame::LoadIpLimitSetup(VOID)
{
	string strTemp;
	ifstream streamSetupEx;
	streamSetupEx.open("IpLimitSetup.ini");
	if( streamSetupEx.is_open() )
	{
		LoginManager::tagLimitIpSetup LimitIpSetup;
		streamSetupEx
			//! �Ƿ�����IP��½����
			>> strTemp >> LimitIpSetup.m_bIpLogin
			//! һ��IPͬһ���˺�����½����
			>> strTemp >> LimitIpSetup.m_MaxIpLoginOncCdKeyCount
			//! һ��IP��½��ͬ�˺������
			>> strTemp >> LimitIpSetup.m_MaxIpLoginCdKeyCount
			//! �����½IP��¼ʱ�䣨���룩
			>> strTemp >> LimitIpSetup.m_ClearIpLoginTime
			//! �Ƿ�����IP������ɫ����
			>> strTemp >> LimitIpSetup.m_bIpCreate
			//! ͬIP��󴴽���ɫ����
			>> strTemp >> LimitIpSetup.m_MaxIpCreateCount
			//! ���������ɫIP��¼ʱ�䣨���룩
			>> strTemp >> LimitIpSetup.m_ClearIpCreateTime
			//! ����IPʱ�䣨���룩
			>> strTemp >> LimitIpSetup.m_FrozenIpTime;		

		streamSetupEx.close();

		//! �������ж����Ƿ�������ת��ʱ�䵥λ
		LimitIpSetup.m_ClearIpLoginTime		*= 1000 * 60;
		LimitIpSetup.m_ClearIpCreateTime	*= 1000 * 60;
		LimitIpSetup.m_FrozenIpTime			*= 1000 * 60;

		GetLoginManager().ChangeIpSetup(LimitIpSetup);
		AddLogText("Load IpLimitSetup.ini file...ok!");
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file IpLimitSetup.ini", "message", MB_OK);
		return false;
	}

	return true;
}

VOID CGame::ReLoadLoginSetup(VOID)
{
	m_bReLoadLoginSetup = true;
}

VOID CGame::ReLoadIpLimitSetup(VOID)
{
	m_bReLoadIpLimitSetup = true;
}

rpattern& CGame::GetRpattern(VOID)
{
	if(NULL == m_pPat)
		m_pPat = new rpattern(GetGame()->GetLoginSetup()->m_szCdkeyRegex);
	return *m_pPat;
}

bool CGame::ReLoadSetup()
{
	LoadSetup();
	GetLoginManager().ChangeSetup(
		GetLoginSetup()->m_dwMaxWsMum,
		GetLoginSetup()->m_dwProcessRankInterval,
		GetLoginSetup()->m_dwSendQueueMsgInterval,
		GetLoginSetup()->m_dwOneProcessNum	
		);
	return true;
}


bool CGame::ReLoadSetupEx()
{
	LoadSetup();

	s_pNetServer_Client->SetParamEx(GetSetup()->lClientMaxBlockConNum,GetSetup()->lClientValidDelayRecDataTime);

	s_pNetServer_World->SetParamEx(GetSetup()->lWorldMaxBlockConNum,GetSetup()->lWorldValidDelayRecDataTime);
	return true;
}


bool	CGame::ReLoadWorldSetup()
{
	LoadWorldSetup();

	//���¿ͻ��˵���ʾ��Ϣ
	UpdateWorldInfoToAllClient();
	UpdateDisplayWorldInfo();

	return true;
}

BOOL CGame::InitStackFileName()
{
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	_snprintf(m_pszRunStackFile,MAX_PATH,"RunStackInfo[%02d_%02d_%02d_%02d_%02d].txt",stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	return TRUE;
}

bool	CGame::LoadWorldSetup()
{
	m_WorldInfoSetup.clear();
	ifstream stream;
	stream.open("WorldInfoSetup.ini");
	if( !stream.is_open() )
		return false;
	long	lNum;
	while(ReadTo(stream, "#"))
	{
		tagWorldInfo	WorldInfo;
		stream >> lNum >> WorldInfo.strName >> WorldInfo.lStateLvl;
		m_WorldInfoSetup[lNum] = WorldInfo;
	}
	stream.close();
	return true;
}

void CGame::SetListWorldInfoBySetup()
{
	m_listWorldInfo.clear();
	m_listWorldInfo = m_WorldInfoSetup;
	map<long,tagWorldInfo>::iterator it = m_listWorldInfo.begin();
	for(;it != m_listWorldInfo.end();it++)
	{
		(*it).second.lStateLvl = 0;//Ĭ��Ϊά����
	}
}

void OutMemInfo(const char *pFileName, const char *pTxt)
{
	if(NULL == pTxt)
		return;

	if (NULL != pFileName)
	{
		char strFile[MAX_PATH];
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		_snprintf(strFile, MAX_PATH, "%s\\MemPoolInfo_%04d-%02d-%02d.txt", pFileName, stTime.wYear, stTime.wMonth, stTime.wDay);

		FILE *fp;
		if( (fp=fopen(strFile, "a+")) == NULL )
		{
			return;
		}

		fseek(fp, 0, SEEK_END);
		char strTime[MAX_PATH];
		_snprintf(strTime, MAX_PATH, "%d:%d:%d\r\n", stTime.wHour, stTime.wMinute, stTime.wSecond);
		fwrite(strTime, strlen(strTime), 1, fp);
		fwrite(pTxt, strlen(pTxt), 1, fp);
		fclose(fp);
	}
}

// ��Ϸ��ʼ��
BOOL CGame::Init()
{
	srand((unsigned)time(NULL));
	random(100);
	CreateDirectory( "log",	NULL);
	
	PutDebugString("LoginServer start!");

	SET_OUT_INFO_SETUP(OutMemInfo, "log\\");

	if (LoadSetup()==false)
	{
		MessageBox(g_hWnd, "Can't find file setup.ini", "message", MB_OK);
		return false;
	}

#ifdef _RUNSTACKINFO_
	InitStackFileName();
#endif

	

	//! ��ʼ���첽ִ�ж���
	GetAsynchronousExecute().SetWorkParam(
		GetSetup()->dwMinDbOptThreadNum, 
		GetSetup()->dwMaxDbOptThreadNum, 
		GetSetup()->dwThreadNumAdjustSpace
		);
	GetAsynchronousExecute().BeginWork(GetSetup()->dwMinDbOptThreadNum);

	GetLoginManager().ChangeSetup(
		GetLoginSetup()->m_dwMaxWsMum,
		GetLoginSetup()->m_dwProcessRankInterval,
		GetLoginSetup()->m_dwSendQueueMsgInterval,
		GetLoginSetup()->m_dwOneProcessNum	
		);

	GetLoginManager().ReLoadNoQueue();
					
	string	strConnectionString = 
		"Provider=" + GetSetup()->strLocalSqlConType +
		"; Server=" + GetSetup()->strLocalSqlServerIP + 
		"; Database=" + GetSetup()->strLocalDBName + 
		"; UID=" + GetSetup()->strLocalSqlUserName + 
		"; PWD=" + GetSetup()->strLocalSqlPassWord + 
		"; OLE DB Services=-1; Driver={SQL Server}";
#ifdef ENABLE_LOCAL_VALIDATE
	LoaclAccountValidate::Init(strConnectionString);
#endif

	char strVersion[256];
	extern long g_lVersion;
	sprintf(strVersion,"LoginServer[%d][%d]-StarDust", g_lVersion, GetSetup()->dwListenPort_Client);

	//�鿴�˳����Ƿ���������
	HWND hWnd = FindWindow(NULL,strVersion);
	if (hWnd != NULL)
	{
		char strMsgBox[256]="";
		sprintf(strMsgBox,"%s ��������������!",strVersion);
		MessageBox(g_hWnd,_T(strMsgBox), "error", MB_OK);
		return false;
	}

	SetWindowText(g_hWnd, strVersion);

	if(LoadWorldSetup() == false)
	{
		AddLogText("Can't find file WorldSetup.ini");
		return false;
	}
	SetListWorldInfoBySetup();
	UpdateDisplayWorldInfo();

	//��ʼ��CMyAdoBase
	CMyAdoBase::Initialize(GetSetup()->strLocalSqlConType,GetSetup()->strLocalSqlServerIP,GetSetup()->strLocalDBName,
		GetSetup()->strLocalSqlUserName,GetSetup()->strLocalSqlPassWord); 


	//�������ݿ�
	m_pDBAllocator = new CDataBlockAllocator(m_Setup.dwDataBlockNum,m_Setup.dwDataBlockSize);

	//��ʼ
	CMessage::Initial(m_pDBAllocator,m_Setup.dwFreeMsgNum);
	// ��ʼ������������
	CMySocket::MySocketInit();
	CMySocket::SetEncryptFlag(m_Setup.lEncryptType);

	//װ���ڴ�Ӱ���ļ�����
	CSMClient::GetSM()->LoadMapFileSetup(m_pDBAllocator);

	// ������������
	if( !InitNetServer_World() )
	{
 		MessageBox(g_hWnd, "Can't init NetServerWorld!", "Message", MB_OK);
		return false;
	}

	// ���ӵ�LoginServer
	if( !InitNetServer_Client() )
	{
		MessageBox(g_hWnd, "Can't init NetServerClient!", "Message", MB_OK);
		return false;
	}

	// ������AccountServer������
	if( !StartConnAccountServer() )
	{
		AddLogText("Can't connect to AccountServer, please run AccountServer first!");
		CreateConnectAccountServer();
	}


	if(GetLoginSetup()->bSndaValidate)
	{
		if(GetCSdoaValidate().Init())
			AddLogText("ʢ��ͨ��֤ģ���ʼ��   ...ok!");
		else
		{
			MessageBox(g_hWnd, "ʢ��ͨ��֤ģ���ʼ��ʧ�ܣ���������!", "Message", MB_OK);
			return false;
		}

		if(GetLoginSetup()->bSndaFcmEnable)
		{
			if(GetCSdoaFcm().Init())
				AddLogText("ʢ�������ģ���ʼ��   ...ok!");
			else
			{
				MessageBox(g_hWnd, "ʢ�������ģ���ʼ��ʧ�ܣ���������!", "Message", MB_OK);
				return false;
			}
		}
	}
	
	//! ����Ϣ��Ӧ����
	m_COnCmdConnect.Init();
	CSMClient::GetSM()->InitOk();
	return true;
}

// ��Ϸ�ͷ�
BOOL CGame::Release()
{
	PutDebugString("LoginServer Exiting...");
	if( !m_bExit )
	{
		m_bExit = true;
		bASConnectThreadExit = true;

		m_COnCmdConnect.Release();

		GetCSdoaFcm().Release();
		GetCSdoaValidate().Release();

		//! �����첽ִ�ж���
		GetAsynchronousExecute().Exit();

		m_AinMsgQueue.Release();
		CSMClient::GetSM()->ExitOk();
		if( GetNetServer_World() )	
		{
			s_pNetServer_World->Exit();			
		}

		if( GetNetServer_Client() )
		{
			s_pNetServer_Client->Exit();
		}

		SAFE_DELETE(s_pNetServer_Client);
		SAFE_DELETE(s_pNetServer_World);

		CMySocket::MySocketClearUp();		

#ifdef ENABLE_LOCAL_VALIDATE
		LoaclAccountValidate::Release();
#endif

		// �ͷ�CMyAdoBase;
		CMyAdoBase::Uninitalize();
		CSMClient::GetSM()->ReleaseSM();
		CMessage::Release();
		SAFE_DELETE(m_pDBAllocator);

		SAFE_DELETE(m_pPat);

		OUT_RUNNING_INFO(true);
	}

	PutDebugString("LoginServer Exited!");
	return true;
}

//������LoginServer�����ӣ���Ϊ�ͻ��˳�ʼ��
BOOL CGame::InitNetServer_Client(void)
{
	s_pNetServer_Client = new CServer;
	long lRet = s_pNetServer_Client->Start(2,m_pDBAllocator,
		m_Setup.dwFreeSockOperNum,m_Setup.dwFreeIOOperNum,m_Setup.lIOOperDataBufNum,
		m_Setup.bCheckNet,m_Setup.dwBanIPTime,
		m_Setup.dwMaxMsgLen,m_Setup.lMaxConnectNum,m_Setup.lMaxClientsNum,
		m_Setup.lPendingWrBufNum,m_Setup.lPendingRdBufNum,
		m_Setup.lMaxSendSizePerSecond,m_Setup.lMaxRecvSizePerSecond,
		m_Setup.lMaxBlockedSendMsgNum);
	if( !lRet )
	{
		MessageBox(g_hWnd, "Start Client Net server Error", "Message", MB_OK);
		return FALSE;
	}

	BOOL bRet = s_pNetServer_Client->Host(m_Setup.dwListenPort_Client,
		NULL, NF_Server_Client,SOCK_STREAM);
	if(!bRet)
	{
		MessageBox(g_hWnd, "Start Host for client Error", "Message", MB_OK);
		return FALSE;
	}

	CMessage::SetClientServer(s_pNetServer_Client);
	
	AddLogText("LoginServer ��ΪClient�ķ�������ʼ���ɹ� !");
	return TRUE;
}

//��ʼ����Ϸ����������ˣ���������������ˣ�װ������
BOOL CGame::InitNetServer_World(void)
{
	s_pNetServer_World = new CServer;
	long lRet = s_pNetServer_World->Start(3,m_pDBAllocator,
		m_Setup.dwWorldFreeSockOperNum,m_Setup.dwWorldFreeIOOperNum,m_Setup.lWorldIOOperDataBufNum,
		m_Setup.bWorldCheckNet,m_Setup.dwWorldBanIPTime,
		m_Setup.dwWorldMaxMsgLen,m_Setup.lWorldMaxConnectNum,m_Setup.lWorldMaxClientsNum,
		m_Setup.lWorldPendingWrBufNum,m_Setup.lWorldPendingRdBufNum,
		m_Setup.lWorldMaxSendSizePerSecond,m_Setup.lWorldMaxRecvSizePerSecond,
		m_Setup.lWorldMaxBlockedSendMsgNum);
	if( !lRet )
	{
		MessageBox(g_hWnd, "Start World Net server Error", "Message", MB_OK);
		return FALSE;
	}

	BOOL bRet = s_pNetServer_World->Host(m_Setup.dwListenPort_World,NULL,NF_Server_World,SOCK_STREAM);
	if(!bRet)
	{
		MessageBox(g_hWnd, "Start Host for world server Error", "Message", MB_OK);
	}
	
	CMessage::SetWorldServer(s_pNetServer_World);
	AddLogText("LoginServer ��ΪWorldServer�ķ�������ʼ���ɹ� !");
	return TRUE;
}



// ��ϷAI
BOOL CGame::AI()
{
	return TRUE;
}

// ��Ϣ����
BOOL CGame::ProcessMessage()
{

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Process World Message Start.");
#endif

	if(GetNetServer_World())
	{
		long lMessageCount = GetNetServer_World()->GetRecvMessages()->GetSize();

		for(int i=0; i<lMessageCount; i++)
		{
			CMessage* pMsg =(CMessage*) GetNetServer_World()->GetRecvMessages()->PopMessage();
			if( pMsg )
			{
				//ShowMessage("4:%x:%x",pMsg, pMsg->GetClientSocket());

				pMsg->Run();
				CBaseMessage::FreeMsg(pMsg);
			}
		}
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Process Client Message Start.");
#endif
	if(GetNetServer_Client())
	{
		long lMessageCount = GetNetServer_Client()->GetRecvMessages()->GetSize();

		for(int i=0; i<lMessageCount; i++)
		{
			CMessage* pMsg = (CMessage*)GetNetServer_Client()->GetRecvMessages()->PopMessage();
			if( pMsg )
			{
				pMsg->Run();
				CBaseMessage::FreeMsg(pMsg);
			}
		}
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Process Ain Message Start.");
#endif
	//! �����Լ������Լ�����Ϣ
	list<AinMessage*> listMsg;
	m_AinMsgQueue.PopMessage(listMsg);
	for(list<AinMessage*>::iterator ite = listMsg.begin(); listMsg.end() != ite; ite++)
	{
		if( NULL != *ite )
		{
			(*ite)->Run();
			AinMessage::Release(&(*ite));
		}
	}
	CSMClient::GetSM()->ProcessMessage();
	return TRUE;
}

//! ��ӦAccountServer���ӶϿ�
VOID CGame::OnNetASClose(VOID)
{
	m_AccountClient.SetASSocket(0);
	CreateConnectAccountServer();
}
//! ������AccountServer�����ӣ���Ϊ�ͻ��˳�ʼ��
BOOL CGame::StartConnAccountServer(void)
{
	AddLogText("��������AccountServer...!");
	m_lASSocket = s_pNetServer_World->Connect(GetSetup()->strAccountServerIp.c_str(), GetSetup()->dwAccountServerPort,NF_AsscountServer_Client);
	if(m_lASSocket>0)
	{
		CMessage::SetASSocket(m_lASSocket);
		m_AccountClient.SetASSocket(m_lASSocket);
		
		CMessage msg(MSG_O2A_LOGIN_Affirm_Request);
		msg.Add(GetSetup()->dwAccountClientNo);
		msg.SendToAS();
		return TRUE;
	}
	AddLogText("����AccountServerʧ��!");
	return FALSE;
}
//! ����AccountServer
BOOL CGame::ReConnectAccountServer(VOID)
{
	if(bASConnectThreadExit)
		return FALSE;
	AddLogText("��������AccountServer...!");
	m_lASSocket = s_pNetServer_World->Connect(GetSetup()->strAccountServerIp.c_str(), GetSetup()->dwAccountServerPort,NF_AsscountServer_Client);
	if(m_lASSocket>0)
	{
		CMessage::SetASSocket(m_lASSocket);
		m_AccountClient.SetASSocket(m_lASSocket);

		CMessage msg(MSG_O2A_LOGIN_Affirm_Request);
		msg.Add(GetSetup()->dwAccountClientNo);
		msg.SendToAS();
		return TRUE;
	}
	AddLogText("����AccountServerʧ��!");
	return FALSE;
}

//! ���������߳�
VOID CGame::CreateConnectAccountServer(VOID)
{
	bASConnectThreadExit=true;
	WaitForSingleObject(hASConnectThread,INFINITE);
	CloseHandle(hASConnectThread);

	bASConnectThreadExit=false;

	unsigned threadID;
	hASConnectThread = (HANDLE)_beginthreadex(NULL,0,CGame::ConnectAccountFunc,NULL,0,&threadID);
}

unsigned __stdcall CGame::ConnectAccountFunc(LPVOID pArguments)
{
	try
	{
		while(true)
		{
			if(bASConnectThreadExit)
				break;
			Sleep(4000);

			if(GetGame()->ReConnectAccountServer()==TRUE)
			{
				break;
			}
		}
	}
	catch(...)
	{
		PutDebugString("����AccountServer���̳߳���!");
	}
	hASConnectThread = 0;
	_endthreadex( 0 );
	return 0;
}

// ��Ϸ��ѭ��
BOOL CGame::MainLoop()
{
#ifdef _RUNSTACKINFO_
	CMessage::AsyClearFile(GetGame()->GetStatckFileName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MainLoop() Start.");
#endif
	static DWORD  now = timeGetTime(), lastRefeashInfoTime = now, lastShowLMInfo = now;
	now = timeGetTime();
	s_lAITick++;
	static DWORD ServerInfoLogLastTime = now;
	// ˢ����ʾ
	if (now - lastRefeashInfoTime > GetSetup()->dwRefeashInfoTime)
	{
		lastRefeashInfoTime = now;
		RefeashInfoText();
		CSMClient::GetSM()->SendRefreshInfo();
	}

	//! 5������ʾһ�ε�����Ϣ
	if(now - lastShowLMInfo > 1000 * 60 * 5)
	{
		lastShowLMInfo = now;
		GetLoginManager().OutLoginInfo();
		OUT_RUNNING_INFO(false);
	}

	static bool bStart = true;
	if (bStart)
	{
		CSMClient::GetSM()->SendAITick(true);
		bStart = false;
	}

	// �����߳�
	if( m_bExit )
	{
		return false;
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ProcessMessage() Start.");
#endif

	// ��Ϣ����
	ProcessMessage();

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ProcessMessage() End.");
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"LoginManager::ProcessQueue() Start.");
#endif

	//! �����½
	GetLoginManager().ProcessQueue();

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"LoginManager::ProcessQueue() End.");
#endif

	// ��ʱping���µ�WorldServer��Ϣ
	static DWORD lastPingWorldTime = now;
	static bool bInPing = false;

	if (bInPing==false)	// ����Ƿ���Ҫping�Ĺ���
	{
		if (now - lastPingWorldTime > GetSetup()->dwPingWorldServerTime)
		{
			lastPingWorldTime = now;
			bInPing = true;

			GetGame()->m_vectorPingWorldServerInfo.clear();

			CMessage msg(MSG_L2W_SERVER_QUEST_PING);
			msg.SendAllWorld();
		}
	}
	else	// �ȴ�����WorldServer���صĹ���
	{
		// ȫ���յ�WorldServer�ķ���
		if ((long)m_vectorPingWorldServerInfo.size() >= GetWorldNum())
		{
			bInPing = false;
		}
		// ������Ϊ�е�WorldServer�Ѿ�ʧȥ��Ӧ
		else if (now - lastPingWorldTime > GetSetup()->dwPingWorldServerErrorTime)
		{
			bInPing = false;

			AddLogText("------------ ��Ϸ���������쳣���� -------------");
			AddLogText("��ǰ����״̬�������б�");

			for (int i=0; i<(int)m_vectorPingWorldServerInfo.size(); i++)
			{
				AddLogText("World[%s:%s] has %d players!",
					m_vectorPingWorldServerInfo[i].strName.c_str(),
					m_vectorPingWorldServerInfo[i].strIP.c_str(),
					m_vectorPingWorldServerInfo[i].dwPlayerNum);
			}
			AddLogText("-------------------------------");

			CMessage msg(MSG_O2A_SERVER_RUN_ERROR);
			msg.SendToAS();
		}
	}

	if(m_bLoadNoQueueCDKeyList)
	{
		GetLoginManager().ReLoadNoQueue();
		m_bLoadNoQueueCDKeyList = false;
	}

	if (m_bReLoadLoginSetup)
	{
		if(!LoadLoginSetup())
		{
			AddLogText("Load LoginSetup.ini file...failed!!!");
		}
		else
		{
			GetLoginManager().ChangeSetup(
				GetLoginSetup()->m_dwMaxWsMum,
				GetLoginSetup()->m_dwProcessRankInterval,
				GetLoginSetup()->m_dwSendQueueMsgInterval,
				GetLoginSetup()->m_dwOneProcessNum	
				);

			if(NULL != m_pPat)
				SAFE_DELETE(m_pPat);
			m_pPat = new rpattern(GetGame()->GetLoginSetup()->m_szCdkeyRegex);

			m_bReLoadLoginSetup = false;
		}
	}

	if (m_bReLoadIpLimitSetup)
	{
		LoadIpLimitSetup();
		m_bReLoadIpLimitSetup = false;
	}

	Sleep(1);
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MainLoop() End.");
#endif
	return true;
}


/////////////////////////////////////////////////////////////////////////
// WorldServer��Ϣ�б�
/////////////////////////////////////////////////////////////////////////

// �������
long CGame::AddWorld(long lWorldNumber,const char* strWorldName)
{
	if(strWorldName==NULL)
		return -1;

	map<long,tagWorldInfo>::iterator it = m_listWorldInfo.find(lWorldNumber);
	if(it == m_listWorldInfo.end())
	{
		AddLogText("�Ƿ���World Server����1! WsName=%s", strWorldName);
		return -1;
	}
	if( (*it).second.strName != strWorldName)
	{
		AddLogText("�Ƿ���World Server����2! WsName=%s", strWorldName);
		return -1;
	}
	(*it).second.lStateLvl = 1;

	//���¿ͻ���World����ʾ��Ϣ
	UpdateWorldInfoToAllClient();
	UpdateDisplayWorldInfo();

	long lSize = (long)m_listWorldInfo.size();

	//���һ��WorldServer�� CDkeyList
	WorldCdkeyList CDkeyList;
	CDkeyList.clear();
	s_listCdkey[lWorldNumber] = CDkeyList;
	GetLoginManager().AddWsWaitQueue(lWorldNumber);

	return lSize;
}

// ɾ������
long CGame::DelWorld(long lNum)
{
	map<long,tagWorldInfo>::iterator it = m_listWorldInfo.find(lNum);
	if(it != m_listWorldInfo.end())
	{
		(*it).second.lStateLvl = 0;
	}

	//���¿ͻ��˵���ʾ��Ϣ
	UpdateWorldInfoToAllClient();
	UpdateDisplayWorldInfo();
	//m_listWorldInfo.erase(lNum);
	long lSize = (long)m_listWorldInfo.size();

	//���
	GetLoginManager().DelWsWaitQueue(lNum);
	s_listCdkey.erase(lNum);

	return lSize;
}

const char *CGame::GetWorldNameByID(long lNum)
{
	std::map<long,tagWorldInfo>::iterator it = m_listWorldInfo.find(lNum);
	if(it != m_listWorldInfo.end())
	{
		return (*it).second.strName.c_str();
	}
	return NULL;
}

long  CGame::GetWorldIDByName(const char* strWorldName)
{
	if(strWorldName==NULL)
		return -1;
	std::map<long,tagWorldInfo>::iterator it = m_listWorldInfo.begin();
	for(;it != m_listWorldInfo.end();it++)
	{
		if(strcmp(strWorldName,(*it).second.strName.c_str())==0)
		{
			if( (*it).second.lStateLvl == 0 )	return -1;
			else return (*it).first;
		}
	}
	return -1;
}

bool CGame::WorldServerIsOpenState(long lNum)
{
	map<long,tagWorldInfo>::iterator it = m_WorldInfoSetup.find(lNum);
	if(it != m_WorldInfoSetup.end())
	{
		return (*it).second.lStateLvl != 0;
	}
	return false;
}

void CGame::AddWorldInfoToMsg(CMessage& msg,string &strCdkey)
{
	msg.Add( (short)GetGame()->GetWorldNum() );	//��ǰ��Ч��worldserver��
	std::map<long,tagWorldInfo>::iterator it = m_listWorldInfo.begin();
	for(;it != m_listWorldInfo.end();it++)
	{
		//�ж�����������Ƿ��ǿ���״̬���Ƿ��ǲ��Ŷӵ�CDKEY
		if( WorldServerIsOpenState(it->first))
		{
			msg.Add( (*it).second.lStateLvl );
		}
		else
		{
			msg.Add( (long)0 );
		}
		msg.Add( (char*)((*it).second.strName.c_str()) );
	}
}

bool CGame::IsExitWorld(const char* strWorldName)
{
	if(strWorldName == NULL)
		return false;

	if( GetWorldIDByName(strWorldName) != -1 )
		return true;
	return false;
}

bool CGame::SendMsg2World(CMessage* pMsg,long lWorldServerID)
{
	if(!pMsg)
		return false;
	
	pMsg->SendToWorldNumID(lWorldServerID);
	return true;
}

void CGame::WsPlayerNumIntoStringFoot(char *pString)
{
	itWorldCdkeyList itWCList = s_listCdkey.begin();
	for (; s_listCdkey.end() != itWCList; ++itWCList)
	{
		map<long,tagWorldInfo>::iterator iteWsInfo = m_WorldInfoSetup.find(itWCList->first);
		if (m_WorldInfoSetup.end() != iteWsInfo)
		{
			char szTmp[256] = {0};
			sprintf(szTmp, "\r\n���������[Name��%s][ID��%d]����ǰ����[%d]", iteWsInfo->second.strName.c_str(), itWCList->first, (LONG)itWCList->second.size());
			strcat(pString, szTmp);
		}
	}
}

long CGame::GetLoginWorldPlayerNumByWorldID(long lID)
{
	itWorldCdkeyList itWCList = s_listCdkey.find(lID);
	if(itWCList != s_listCdkey.end())
	{
		return (long)(*itWCList).second.size();
	}
	return -1;
}
long  CGame::GetLoginWorldPlayerNumByWorldName(const char* strWorldName)
{
	long lWorlNumber = GetWorldIDByName(strWorldName);
	if(lWorlNumber != -1)
		return GetLoginWorldPlayerNumByWorldID(lWorlNumber);
	return -1;
}

bool CGame::AddCdkey(const char *szCdkey, long lWorldNumber)
{
	assert(NULL != szCdkey);
	if(0 == strlen(szCdkey))
		return false;
	itWorldCdkeyList itWCList = s_listCdkey.find(lWorldNumber);
	if(itWCList == s_listCdkey.end())
		return false;
	WorldCdkeyList& TemptWorldCdkeyList = (*itWCList).second;

	itCdkeyList iter;
	for(iter=TemptWorldCdkeyList.begin();iter!=TemptWorldCdkeyList.end();iter++)
	{
		if(strcmp(szCdkey,(*iter).c_str()) == 0)
		{
			return false;
		}
	}

	TemptWorldCdkeyList.push_back(szCdkey);

	//ͳ�Ʒ�����״̬
	long lCurNum = (long)TemptWorldCdkeyList.size();
	OnChangeStateLvl(lWorldNumber,lCurNum);

	return true;
}

VOID CGame::SendOnlineInfoToAs(VOID)
{
	if(0 == m_AccountClient.GetASSocket())
		return;

	WorldCdkeyList listCdkey;

	for (itWorldCdkeyList ite = s_listCdkey.begin(); s_listCdkey.end() != ite; ++ite)
	{
		for (itCdkeyList iteCdkey = ite->second.begin(); ite->second.end() != iteCdkey; ++iteCdkey)
		{
			listCdkey.push_back(*iteCdkey);
		}
	}

	CMessage msg(MSG_O2A_LOGIN_SendOnlineInfo);
	DBWriteSet dbWriteSet;
	msg.GetDBWriteSet(dbWriteSet);

	dbWriteSet.AddToByteArray((LONG)listCdkey.size());
	for (itCdkeyList iteCdkey = listCdkey.begin(); listCdkey.end() != iteCdkey; ++iteCdkey)
	{
		dbWriteSet.AddToByteArray((*iteCdkey).c_str());
	}

	msg.SendToAS();

	GetLoginManager().ClearFcm();
}

VOID CGame::DelWsCdKey(long lWorldNumber)
{
	itWorldCdkeyList itWCList = s_listCdkey.find(lWorldNumber);
	if(s_listCdkey.end() == itWCList)
		return;
	WorldCdkeyList& TemptWolrdCdkeyList = (*itWCList).second;

	TemptWolrdCdkeyList.clear();
	OnChangeStateLvl((*itWCList).first,0);
}

void CGame::ClearCDKey(const char *szCdkey)
{
	itWorldCdkeyList itWCList = s_listCdkey.begin();
	for(;itWCList != s_listCdkey.end();itWCList++)
	{
		WorldCdkeyList& TemptWolrdCdkeyList = (*itWCList).second;
		itCdkeyList iter;
		for(iter=TemptWolrdCdkeyList.begin();iter!=TemptWolrdCdkeyList.end();iter++)
		{
			if(strcmp(szCdkey,(*iter).c_str()) ==0)
			{
				TemptWolrdCdkeyList.erase(iter);
				long lCurNum = (long)TemptWolrdCdkeyList.size();
				OnChangeStateLvl((*itWCList).first,lCurNum);
				ClearLoginCdkey(szCdkey);
				return;
			}
		}
	}
}

long CGame::GetWorldSateLvl(long lPlayerNum)
{
	int i = 1;
	for(i = 1;i <= 3;i++)
	{
		if( lPlayerNum < m_StateLvl[i] )
		{
			break;
		}
	}
	if(i > 3)
		i = 3;
	return i;
}


void CGame::OnChangeStateLvl(long lWorldNum,long lPlayerNum)
{
	long lStateLvl = GetWorldSateLvl(lPlayerNum);

	map<long,tagWorldInfo>::iterator it = m_listWorldInfo.find(lWorldNum);
	if(it != m_listWorldInfo.end())
		(*it).second.lStateLvl = lStateLvl;
}

void CGame::ChangeAllWorldSate()
{
	std::map<long,tagWorldInfo>::iterator it = m_listWorldInfo.begin();
	for(;it != m_listWorldInfo.end();it++)
	{
		itWorldCdkeyList itWCList = s_listCdkey.find((*it).first);
		if(itWCList != s_listCdkey.end())
		{
			WorldCdkeyList& TemptWorldCdkeyList = (*itWCList).second;
			long lStateLvl = GetWorldSateLvl((long)TemptWorldCdkeyList.size());
			(*it).second.lStateLvl = lStateLvl;
		}
	}
}
void CGame::ClearCDKeyByWorldServerID(long lID)
{
	itWorldCdkeyList itWCList = s_listCdkey.find(lID);
	if(itWCList != s_listCdkey.end())
	{
		WorldCdkeyList& TemptWolrdCdkeyList = (*itWCList).second;
		TemptWolrdCdkeyList.clear();
	}
}

int CGame::FindCdkey(const char *szCdkey)
{
	itWorldCdkeyList itWCList = s_listCdkey.begin();
	for(;itWCList != s_listCdkey.end();itWCList++)
	{
		WorldCdkeyList& TemptWolrdCdkeyList = (*itWCList).second;
		itCdkeyList iter;
		for(iter=TemptWolrdCdkeyList.begin();iter!=TemptWolrdCdkeyList.end();iter++)
		{
			if(strcmp(szCdkey,(*iter).c_str()) ==0)
			{
				return (*itWCList).first;
			}
		}
	}
	return -1;
}

long CGame::GetLoginWorldCdkeyNumbers()
{
	long lNumbers = 0;
	itWorldCdkeyList itWCList = s_listCdkey.begin();
	for(;itWCList != s_listCdkey.end();itWCList++)
	{
		lNumbers += (long)(*itWCList).second.size();
	}
	return lNumbers;
}

// �߳�һ���û�,����û����ڵ�½״̬,��ôֱ�Ӷϵ�������Ϣ����Ӧ��WorldServerȥ���;
// ����Ѿ�������Ϸ,��ô����Ϣ����Ӧ��WorldServerȥ���.
bool CGame::KickOut(const char *szCdkey)
{
	//�Ƿ��Ѿ������ʹ�ø�CDKEY
	//���������������LOGINSERVER�����
	if( GetGame()->GetLoginCdkeyWorldServer(szCdkey)!= NULL )
	{
		//�ϵ���Ӧ�Ŀͻ���
		long lSocketID = CMessage::GetSocketByCdkey(szCdkey);
		if(lSocketID)
		{
			GetGame()->GetNetServer_Client()->DisConn(lSocketID);
			GetLoginManager().OnClientNetClose(szCdkey, FALSE);
		}
	}
	
	int nWordNum = FindCdkey(szCdkey);
	if(nWordNum != -1)
	{
		//##������Ϣ��World Server������World Server��ͬһ�ʺų��Ե�½��
		CMessage msgToWorldServer( MSG_L2W_LOG_REPEAT_LOGIN );
		msgToWorldServer.Add( (char*)(szCdkey) );
		GetGame() -> SendMsg2World( &msgToWorldServer, nWordNum);
		return true;
	}
	return false;
}
void CGame::KickOut(LONG lSocket)
{
	GetGame()->GetNetServer_Client()->DisConn(lSocket);
}

//����Cdkey��½��worldServer;
void CGame::SetLoginCdkeyWorldServer(const char* szCdkey,const char* strWorldServer)
{
	if(szCdkey == NULL)	return ;
	
	m_LoginCdkeyWorld[szCdkey] = pair<string, long>(strWorldServer, GetWorldIDByName(strWorldServer));
}

//�õ�Cdkey��½��WorldServer;
const char* CGame::GetLoginCdkeyWorldServer(const char* szCdkey)
{
	if(NULL != szCdkey)
	{
		map<string, pair<string, LONG>>::iterator it = m_LoginCdkeyWorld.find(szCdkey);
		if( it != m_LoginCdkeyWorld.end() )
		{
			return (*it).second.first.c_str();
		}
	}
	return NULL;
}

LONG CGame::GetLoginCdkeyWorldServerID(const char* szCdkey)
{
	if(NULL != szCdkey)
	{
		map<string, pair<string, LONG>>::iterator it = m_LoginCdkeyWorld.find(szCdkey);
		if( it != m_LoginCdkeyWorld.end() )
		{
			return (*it).second.second;
		}
	}
	return -1;
}

//�����½�����е�Cdkey
void CGame::ClearLoginCdkey(const char* szCdkey)
{
	m_LoginCdkeyWorld.erase(szCdkey);
}

//loginserver��worldserver������һ�����Ϣ 
bool CGame::L2W_PlayerBase_Send(const char *wsName,const char *szCdkey)
{
	if(wsName == NULL || szCdkey == NULL)
		return false;

	//cdkey����
	if(strcmp(szCdkey,"") == 0)
		return false;

	//�ж�ĳ��WorldServer�Ƿ�����������
	long lWorldeNumber = GetWorldIDByName(wsName);
	if(lWorldeNumber != -1)
	{
		//�ǿ�״̬�����ǲ��Ŷӵ�Cdkey
		if( WorldServerIsOpenState(lWorldeNumber))
		{
			CMessage msg(MSG_L2W_LOG_QUEST_PLAYERBASELIST);
			msg.Add((char*)szCdkey);
			msg.SendToWorldNumID(lWorldeNumber);

#ifdef SHOW_LOGIN_MSG_20090817
			if(bOutInfo)
			{
				AddLogText("��WS���ͻ�����Ϣ����WS��NAME��%s��ID��%d, SocketID��%s������ҡ�Account��%s��", wsName, lWorldeNumber, CMessage::GetWorldIDBySocket(lWorldeNumber), szCdkey);
			}
#endif
			return true;
		}
	}
	return false;
}

//loginserver��worldserver����ɾ����ɫ������
void CGame::L2W_DeleteRole_Send(const char *szWSName,const char *szCdkey,CGUID& PlayerID,DWORD dwClientIP)
{
	if (szWSName==NULL)
		return;

	//cdkey����
	if(strcmp(szCdkey,"") == 0)
		return;

	long lWorldeNumber = GetWorldIDByName(szWSName);
	if(lWorldeNumber != -1)
	{
		CMessage msg(MSG_L2W_LOG_DELETEROLE);
		msg.Add(const_cast<char*>(szCdkey));
		msg.Add(PlayerID);
		msg.Add((DWORD) dwClientIP );
		msg.SendToWorldNumID(lWorldeNumber);
	}
}

void CGame::L2W_RestoreRole_Send(LPCSTR szWSName, LPCSTR szCdkey, const CGUID& playerID)
{
	_ASSERT(szWSName);_ASSERT(szCdkey);

	if(szWSName == NULL || NULL == szCdkey)
		return;

	long lWorldNumber = this->GetWorldIDByName(szWSName);
	if(lWorldNumber != -1)
	{
		CMessage msg(MSG_L2W_LOG_RESTOREROLE);
		msg.Add(const_cast<char*>(szCdkey));
		msg.Add(playerID);

		msg.SendToWorldNumID(lWorldNumber);
	}
}

//loginserver��worldserver���ʹ�����ɫ������
void CGame::L2W_CreateRole_Send(const char *szWSName,const char *szCdkey,CMessage *pMsg)
{
	if (szWSName==NULL)
		return;

	long lWorldNumber = this->GetWorldIDByName(szWSName);
	if(lWorldNumber != -1)
	{
		pMsg->SetType(MSG_L2W_LOG_CREATEROLE);
		pMsg->Add(const_cast<char*>(szCdkey));	
		pMsg->SendToWorldNumID(lWorldNumber);
	}
}

//loginserver��worldserver������ҵ���ϸ��Ϣ
void CGame::L2W_QuestDetail_Send(const char *szWSName,const char *szCdkey,CGUID& PlayerID,DWORD dwClientIP, DWORD dwSignCode)
{
	if (szWSName==NULL)
		return;

	//cdkey����
	if(strcmp(szCdkey,"") == 0)
		return;

	long lWorldNumber = this->GetWorldIDByName(szWSName);
	if(lWorldNumber != -1)
	{
		CMessage msg(MSG_L2W_LOG_QUEST_PLAYERDATA);
		msg.Add(dwSignCode);
		msg.Add(PlayerID);
		msg.Add(const_cast<char*>(szCdkey));

		//�Ʒ���
		msg.Add(dwClientIP);
		
		msg.SendToWorldNumID(lWorldNumber);
	}
}

void CGame::UpdateDisplayWorldInfo()
{
	SendMessage(g_hWorldListBox,LB_RESETCONTENT,NULL,NULL);
	//���¿ؼ���ķ�����״̬
	std::map<long,tagWorldInfo>::iterator it = m_listWorldInfo.begin();
	for(;it != m_listWorldInfo.end();it++)
	{ 
		string strName = (*it).second.strName;
		strName += "[";
		long lFlag = (*it).second.lStateLvl;
		map<long,tagWorldInfo>::iterator itSetup =
								m_WorldInfoSetup.find( (*it).first );			
		if(itSetup != m_WorldInfoSetup.end())
		{
			if(lFlag != 0 && (*itSetup).second.lStateLvl == 0)
			{
				lFlag = -1;
			}
		}
		if(lFlag == 0)
		{
			strName += "�Ͽ�";
		}
		else if(lFlag == -1)
		{
			strName += "�ر�";
		}
		else
		{
			strName += "����";
		}
		strName += "]";
		SendMessage(g_hWorldListBox,LB_INSERTSTRING,-1,(LPARAM)(strName.c_str()));
	}

}

//����WorldServer��״̬��Ϣ���ͻ���
void CGame::UpdateWorldInfoToAllClient()
{
	map<string,pair<string, LONG>>::iterator it = m_LoginCdkeyWorld.begin();
	for(;it != m_LoginCdkeyWorld.end();it++)
	{
		if( (*it).second.first == "" )
		{
			string strCdkey = (*it).first;
			CMessage msg(MSG_L2C_LOG_UPDATEWORLDSERVERINFO);
			AddWorldInfoToMsg( msg,strCdkey);
			msg.SendToClient(strCdkey.c_str());
		}
	}
}

unsigned CGame::GetCdkeyCount()
{
	unsigned cnt = 0;
	for(std::map<long,WorldCdkeyList>::iterator it = GetGame()->s_listCdkey.begin();
		it != GetGame()->s_listCdkey.end(); it++)
	{
		cnt += (long)it->second.size();
	}
	return cnt;
}


void CGame::OnNetWorldClose(long lWorldID)
{
	const char* strWorldName = GetGame()->GetWorldNameByID(lWorldID);
	if( strWorldName )
	{
		AddLogText("WorldServer [%s] lost!",strWorldName);
		ClearCDKeyByWorldServerID(lWorldID);	//����͸�worldserver��ص���ҵ�CDKEY
		SetWsReadyState(lWorldID, FALSE);
	}
	GetGame()->DelWorld(lWorldID);
}
void CGame::OnNetClientClose(const char* szCdkey)
{
	if(szCdkey==NULL)	return;

	
	// ������Ϸ
	BOOL bInWs = FALSE;
	if( FindCdkey(szCdkey) != -1 )
	{
		bInWs = TRUE;
	}
	else	// ֱ���˳�
	{
		// ����Ϣ��WorldServer�ߵ���Ӧ�����LoginList��Ϣ
		long lWorldID = GetWorldIDByName(GetLoginCdkeyWorldServer(szCdkey));
		if (lWorldID != -1)
		{
			// ����Ϣ��WorldServer�ߵ���Ӧ�����LoginList��Ϣ
			CMessage msgToWorldServer(MSG_L2W_LOG_KICKPLAYER);
			msgToWorldServer.Add( (char*)(szCdkey) );
			SendMsg2World( &msgToWorldServer, lWorldID);
		}

		// �˳���־�����£�
		ClearCDKey(szCdkey);	
	}

	GetLoginManager().OnClientNetClose(szCdkey, bInWs);
	ClearLoginCdkey(szCdkey);
}

int CGame::ReadyToEnter(const char *szCdkey, DWORD dwIP, long lSockID, const char *szWSName)
{
	if( !szCdkey || !dwIP || !lSockID || !szWSName ) return 1;

	LONG lWsID = FindCdkey(szCdkey);
	if(-1 == lWsID)
	{
		if( GetGame()->GetLoginCdkeyWorldServer(szCdkey)!= NULL )
		{
			//�ϵ���Ӧ�Ŀͻ���
			long lSocketID = CMessage::GetSocketByCdkey(szCdkey);
			if(lSocketID)
				GetGame()->GetNetServer_Client()->DisConn(lSocketID);
			GetGame()->ClearLoginCdkey(szCdkey);

			CMessage msg( MSG_L2C_LOG_ANSWERCDKEY );
			msg.Add((LONG)LOGIN_CHECKOUT);
			msg.SendToClient(lSockID);

			return 1;
		}
		else if( strlen(szWSName) ) 	
		{
			//! �Ƿ�ѡ�񣬶ϵ��ͻ���
			GetGame()->GetNetServer_Client()->DisConn(lSockID);
			return 1;
		}
	}
	else
	{
		// ����Ϣ��WorldServer�ߵ���Ӧ�����LoginList��Ϣ
		CMessage msgToWorldServer( MSG_L2W_LOG_REPEAT_LOGIN );
		msgToWorldServer.Add( (char*)(szCdkey) );
		GetGame() -> SendMsg2World( &msgToWorldServer, lWsID);
		
		CMessage msg( MSG_L2C_LOG_ANSWERCDKEY );
		msg.Add((LONG)LOGIN_CHECKOUT);
		msg.SendToClient(lSockID);
		GetLoginManager().OnClientNetClose(szCdkey, FALSE);

		GetGame()->ClearLoginCdkey(szCdkey);

		return 1;
	}

	//���ͻ����׽��ֹ���Cdkey
	SetLoginCdkeyWorldServer(szCdkey,"");

	// ����client
	CMessage msg(MSG_L2C_LOG_ANSWERCDKEY);
	msg.Add((LONG)LOGIN_LOGINSERVEROK);
	GetGame()->AddWorldInfoToMsg(msg,string(szCdkey));
	msg.SendToClient(lSockID);

	return 0;
}

BOOL CGame::EnterToSelect(const char *pCdkey, const char *pWSName, DWORD dwIP, long lSockID)
{
	LONG lWsID = FindCdkey(pCdkey);
	if(-1 != lWsID)
	{
		// ����Ϣ��WorldServer�ߵ���Ӧ�����LoginList��Ϣ
		CMessage msgToWorldServer( MSG_L2W_LOG_REPEAT_LOGIN );
		msgToWorldServer.Add( (char*)(pCdkey) );
		GetGame() -> SendMsg2World( &msgToWorldServer, lWsID);

		CMessage msg( MSG_L2C_LOG_ANSWERCDKEY );
		msg.Add((LONG)LOGIN_CHECKOUT);
		msg.SendToClient(lSockID);
		GetLoginManager().OnClientNetClose(pCdkey, FALSE);
		GetGame()->ClearLoginCdkey(pCdkey);

		return FALSE;
	}

	if( L2W_PlayerBase_Send(pWSName,pCdkey) )//��worldserver����ÿͻ��Ļ�����Ϣ
	{
		SetLoginCdkeyWorldServer(pCdkey,pWSName);//���ø�������ѡ���worldserver��
		GetCSdoaFcm().UserOnline(pCdkey, dwIP);
		return TRUE;
	}
	else
	{
		// ����client
		CMessage msg(MSG_L2C_LOG_ANSWERCDKEY);
		msg.Add((LONG)LOGIN_LOGINSERVEROK);
		GetGame()->AddWorldInfoToMsg(msg,string(pCdkey));
		msg.SendToClient(lSockID);
	}
	return FALSE;
}

int CGame::EnterGame( const char *szCdkey, DWORD dwIP, long lSockID, const char *szWSName, bool bLimit, bool bMobileUser )
{
	if( !szCdkey || !dwIP || !lSockID || !szWSName ) return 1;

	// ����Ѿ�����Ϸ��,��ô�ߵ����ʺ�
	if( !strlen(szWSName) && GetGame()->KickOut(szCdkey) ) 	{
		//##����ʺ�����Ϸ��,֪ͨ���,��ʱ�޷���½.
		CMessage msg( MSG_L2C_LOG_ANSWERCDKEY );
		msg.Add( (LONG)(LOGIN_CHECKOUT) );
		msg.SendToClient(lSockID);

		GetGame()->ClearLoginCdkey(szCdkey);
		return 1;
	}

	//���ͻ����׽��ֹ���Cdkey
	SetLoginCdkeyWorldServer(szCdkey,"");

	// ֱ�ӽ���ѡ�˽��棨�˵�ѡ�˽�����룩
	if (szWSName[0]!=0)
	{
		if( L2W_PlayerBase_Send(szWSName,szCdkey) )//��worldserver����ÿͻ��Ļ�����Ϣ
		{
			SetLoginCdkeyWorldServer(szCdkey,szWSName);//���ø�������ѡ���worldserver��
			GetCSdoaFcm().UserOnline(szCdkey, dwIP);
		}
	}
	else
	{
		// ����client
		CMessage msg(MSG_L2C_LOG_ANSWERCDKEY);
		msg.Add((LONG)LOGIN_LOGINSERVEROK);
		GetGame()->AddWorldInfoToMsg(msg,string(szCdkey));
		msg.SendToClient(lSockID);
	}

	return 0;
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
		}

		if(GetGame())
			GetGame()->Release();
		DeleteGame();

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		MessageBox(g_hWnd, "������ִ�����鿴���µĴ��󱨸��ļ�", "Error", MB_OK);

		PostMessage(g_hWnd, WM_DESTROY, 0, 0);
		SetEvent(g_hGameThreadExitEvent);
		_endthreadex( 0 );
		return 0;
	}
#endif

	SetEvent(g_hGameThreadExitEvent);
	PostMessage(g_hWnd, WM_CLOSE, 1, 0);
	_endthreadex( 0 );
	return 0;
}

bool ClearOnlineUserDatabase() {

	_ConnectionPtr cn;
	if(!CMyAdoBase::CreateCn(cn))
	{
		CMyAdoBase::ReleaseCn(cn);
		return false;
	}
	if(!CMyAdoBase::OpenCn(cn))
	{
		CMyAdoBase::ReleaseCn(cn);
		return false;
	}
	if(!CMyAdoBase::ExecuteCn("DELETE FROM online_user" ,cn))
	{
		CMyAdoBase::ReleaseCn(cn);
		return false;
	}
	CMyAdoBase::ReleaseCn(cn);
	return true;
}


AsynchronousExecute::MyOverlapped* AsynchronousExecute::CreateMyOverlapped(VOID)
{
	return (MyOverlapped*)M_ALLOC(sizeof(MyOverlapped));
}
VOID AsynchronousExecute::RestoreMyOverlapped(MyOverlapped** ppMyOverlapped)
{
	if(NULL != ppMyOverlapped)
		M_FREE(*ppMyOverlapped, sizeof(MyOverlapped));
}
