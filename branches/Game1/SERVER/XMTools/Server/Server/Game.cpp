#include "StdAfx.h"
#include <sstream>
#include "Game.h"


#include "..\..\..\nets\net\message.h"
#include "..\..\..\nets\Servers.h"
#include "..\..\..\nets\MemClient.h"



CGame* g_pGame = NULL;						// ��Ϸ����
bool g_bGameThreadExit = false;				// �Ƿ��˳����߳�
HANDLE g_hGameThreadExitEvent = NULL;		// ���߳��˳��¼�

bool bConnectServerThreadExit = false;		// �Ƿ��˳����ӷ������߳�
HANDLE hConnectServerThread = NULL;			// ���ӷ������߳̾��

// ������Ϸ����
BOOL CreateGame()
{
	g_pGame = new CGame();
	return TRUE;
}

// �ͷ���Ϸ����
BOOL DeleteGame()
{
	if(g_pGame)
	{
		delete g_pGame;
		g_pGame = NULL;
	}
	return TRUE;
}

CGame* GetGame()
{
	return g_pGame;
}

LogicLogInterface* GetGameLogInterface()
{
	return GetGame()->GetLogicLogInterface();
}

CGame::CGame(void)
{
	s_pNetServer = NULL;

	m_lServerInitSendFlag = 0;
	InitializeCriticalSection(&m_csServerInitSendFlag);

	m_pLogicLogInterface= new LogicLogInterface(NULL);
}

CGame::~CGame(void)
{
	DeleteCriticalSection(&m_csServerInitSendFlag);
}

BOOL CGame::Init()
{
	// ������־����Ŀ¼
	CreateDirectory("log", NULL);

#ifdef _RUNSTACKINFO_
	InitStackFileName();
#endif

	srand((unsigned)time(NULL));
	random(100);
	PutDebugString("Server start!!!");

	// װ����Դ
	LoadServerResource();

	if(!LoadSetup())
	{
		return FALSE;
	}

	// ���ط�������Ϸ����
	if(!LoadServerSetup("ServerSetup.ini"))
	{
		return FALSE;
	}

	// �������ݿ�
	m_pDBAllocator = new CDataBlockAllocator(m_tgSetup.dwDataBlockNum, m_tgSetup.dwDataBlockSize);

	CMessage::Initial(m_pDBAllocator, m_tgSetup.dwFreeMsgNum);
	// ��ʼ������������
	CMySocket::MySocketInit();

	// װ���ڴ�Ӱ���ļ�����
	LoadMapFileSetup();

	// ������������
	if( !InitNetServer() )
	{
		MessageBox(g_hWnd, "Initial NetServer failed!", "Message", MB_OK);
		return FALSE;
	}

	// ��ʼ����
	if( !StartAccept() )
	{
		return FALSE;
	}


	return TRUE;
}

BOOL CGame::InitStackFileName()
{
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	_snprintf(m_pszRunStackFile,MAX_PATH,"RunStackInfo[%02d_%02d_%02d_%02d_%02d].txt",stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
		
	return TRUE;
}

bool CGame::LoadServerResource()
{
	CHAR* pszPath = new CHAR[MAX_PATH];
	string strWorkPath("");
	if(::GetCurrentDirectory(MAX_PATH, pszPath))
	{
		strWorkPath = string(pszPath);
	}
	delete[] pszPath;

	// ��ʼ�����ݰ�
	string strFileInfo("PackRes.cfg");
	CClientResource::InitialInstance();
	CClientResource* pClientResource = CClientResource::GetInstance();
	pClientResource->Initial(GAME_RES, strWorkPath, strFileInfo);
	if(!pClientResource->LoadEx())
	{
	//	AddLogText("Load server resource failed!");
	//	return false;
	}

	AddLogText("Load server resource OK!");

	return true;
}

bool CGame::LoadSetup()
{
	// ����ȫ������
	string strTemp;

	ifstream streamLog;

	streamLog.open("LogSetup.ini");
	if( streamLog.is_open() )
	{
		streamLog 
			>> strTemp >> GetSetup()->lUseLogServer
			>> strTemp >> GetSetup()->strLogIP
			>> strTemp >> GetSetup()->dwLogPort
			>> strTemp >> GetSetup()->strErrFile
			>> strTemp >> GetSetup()->strLogFile
			>> strTemp >> GetSetup()->strTxtLogFile
			>> strTemp >> GetSetup()->dClientNo;
		streamLog.close();
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file LogSetup.ini", "message", MB_OK);
		return false;
	}

	ifstream isSetup;
	isSetup.open("Setup.ini");
	if( isSetup.is_open() )
	{
		isSetup
			>> strTemp >> m_tgSetup.dwNumber
			>> strTemp >> m_tgSetup.strName
			>> strTemp >> m_tgSetup.strLocalIP
			>> strTemp >> m_tgSetup.dwListenPort;
		isSetup.close();

		strTemp = "Server";
		strTemp += "[";
		strTemp += m_tgSetup.strName;
		strTemp += "]-Game";

		// �鿴�˳����Ƿ���������
		HWND hWnd = FindWindow(NULL, strTemp.c_str());
		if( hWnd != NULL )
		{
			char strMsg[256];
			sprintf(strMsg, "%s ��������������!", strTemp.c_str());
			MessageBox(g_hWnd, _T(strMsg), "error", MB_OK);
			return false;
		}

		SendMessage(g_hWnd, WM_SETTEXT, 0, (LPARAM)strTemp.c_str());
	}
	else	
	{
		MessageBox(g_hWnd, "Can't find file setup.ini", "message", MB_OK);
		return false;
	}

	// load setupex.ini
	ifstream isEx;
	isEx.open("SetupEx.ini");
	if( isEx.is_open() )
	{
		isEx
			>> strTemp >> m_tgSetup.dwDataBlockNum
			>> strTemp >> m_tgSetup.dwDataBlockSize
			>> strTemp >> m_tgSetup.dwFreeMsgNum
			
			>> strTemp >> m_tgSetup.dwFreeSockOperNum
			>> strTemp >> m_tgSetup.dwFreeIOOperNum
			>> strTemp >> m_tgSetup.lIOOperDataBufNum
			>> strTemp >> m_tgSetup.bCheckNet
			>> strTemp >> m_tgSetup.dwBanIPTime
			>> strTemp >> m_tgSetup.dwMaxMsgLen
			>> strTemp >> m_tgSetup.lMaxConnectNum
			>> strTemp >> m_tgSetup.lMaxClientsNum
			>> strTemp >> m_tgSetup.lPendingWrBufNum
			>> strTemp >> m_tgSetup.lPendingRdBufNum
			>> strTemp >> m_tgSetup.lMaxSendSizePerSecond
			>> strTemp >> m_tgSetup.lMaxRecvSizePerSecond
			>> strTemp >> m_tgSetup.lMaxBlockedSendMsgNum
			>> strTemp >> m_tgSetup.lConPendingWrBufNum
			>> strTemp >> m_tgSetup.lConPendingRdBufNum
			>> strTemp >> m_tgSetup.lConMaxSendSizePerSecond
			>> strTemp >> m_tgSetup.lConMaxRecvSizePerSecond
			>> strTemp >> m_tgSetup.lConMaxBlockedSendMsgNum

			>> strTemp >> m_tgSetup.dwRefeashInfoTime
			>> strTemp >> m_tgSetup.dwSaveInfoTime;

		isEx.close();
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file setupex.ini", "message", MB_OK);
		return false;
	}
	return true;
}

bool CGame::LoadServerSetup(const char* pszFileName)
{
	CRFile* prfile = rfOpen(pszFileName);
	if( !prfile )
	{
		char str[256];
		sprintf(str, "Can't find file %s", pszFileName);
		MessageBox(g_hWnd, str, "message", MB_OK);
		return false;
	}

	string strTemp;
	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	// ����Ϸ��������Ϣ
	tagClientInfo stCInfo;
	long lClientNum = 0;
	stream >> strTemp >> lClientNum;
	for(int i=0; i<lClientNum; i++)
	{
		ReadTo(stream, "#");
		stream >> stCInfo.dwIndex >> stCInfo.strIP >> stCInfo.dwPort;
		stCInfo.bConnected = false;
		s_mapClientInfo[stCInfo.dwIndex] = stCInfo;
	}
//	AddLogText(CStringReading::LoadString(IDS_WS_GAME, STR_WS_GAME_LOADWSSETUPOK));
	return true;
}

bool CGame::LoadMapFileSetup()
{
	string strTemp("");
	string strMemClientID("");
	string strMapObjName("");
	ifstream stream;
	stream.open("MapFileSetup.ini");
	if( stream.is_open() )
	{
		stream
			>> strTemp >> strMemClientID
			>> strTemp >> strMapObjName;
		m_tgSetup.dwGappID = atoi(strMemClientID.c_str());
		GetInst(CMemClient).Initial(strMapObjName.c_str(), m_pDBAllocator);
		return true;
	}
	return false;
}

bool CGame::InitNetServer()
{
	s_pNetServer = new CServer;
	//��Ϊ�������Ϳͻ���˫���������
	long lRet = s_pNetServer->Start(1|2,m_pDBAllocator,
		m_tgSetup.dwFreeSockOperNum,m_tgSetup.dwFreeIOOperNum,m_tgSetup.lIOOperDataBufNum,
		m_tgSetup.bCheckNet,m_tgSetup.dwBanIPTime,
		m_tgSetup.dwMaxMsgLen,m_tgSetup.lMaxConnectNum,m_tgSetup.lMaxClientsNum,
		m_tgSetup.lPendingWrBufNum,m_tgSetup.lPendingRdBufNum,
		m_tgSetup.lMaxSendSizePerSecond,m_tgSetup.lMaxRecvSizePerSecond,
		m_tgSetup.lMaxBlockedSendMsgNum,
		m_tgSetup.lConPendingWrBufNum,m_tgSetup.lConPendingRdBufNum,
		m_tgSetup.lConMaxSendSizePerSecond,m_tgSetup.lConMaxRecvSizePerSecond,
		m_tgSetup.lConMaxBlockedSendMsgNum);
	if( !lRet )
	{
		MessageBox(g_hWnd, "Start NetServer Error", "Message", MB_OK);
		return FALSE;
	}
	CMessage::SetServer(s_pNetServer);
//	AddLogText(CStringReading::LoadString(IDS_WS_GAME, STR_WS_GAME_NETSERVERINITOK));

	return true;
}

bool CGame::StartAccept()
{
	if(NULL == s_pNetServer)	return false;
	BOOL bRet = s_pNetServer->Host(m_tgSetup.dwListenPort,NULL,NF_SERVER_CLIENT,SOCK_STREAM);
	if(!bRet)
	{
		MessageBox(g_hWnd, "Start Host Error", "Message", MB_OK);
		return false;
	}
//	AddLogText(CStringReading::LoadString(IDS_WS_GAME, STR_WS_GAME_WSFORGSINITOK));
	return true;
}

bool CGame::StartConnectServer()
{
	return ReConnectServer();
}

bool CGame::ReConnectServer()
{
	// ��ʱ�������ʾ NF_LSERVER_CLIENT
	m_lServerSocket = s_pNetServer->Connect(m_tgSetup.strServerIP.c_str(), m_tgSetup.dwServerPort, NF_LSERVER_CLIENT);
	if( m_lServerSocket>0 )
	{
		CMessage::SetServerSocket(m_lServerSocket);
		SetServerInitSendFlag(LOGIN_SERVER_INITSENDFLAG);
		return true;
	}
	return false;
}

void CGame::SetServerInitSendFlag(eServerInitSendMsgFlag flag)
{
	EnterCriticalSection(&m_csServerInitSendFlag);
	m_lServerInitSendFlag = m_lServerInitSendFlag | flag;
	LeaveCriticalSection(&m_csServerInitSendFlag);
}

void CGame::CreateConnectServerThread()
{
	bConnectServerThreadExit = true;
	WaitForSingleObject(hConnectServerThread, INFINITE);
	CloseHandle(hConnectServerThread);

	bConnectServerThreadExit = false;
	unsigned int threadID;
	hConnectServerThread = (HANDLE)_beginthreadex(NULL,0,ConnectServerFunc,NULL,0,&threadID);

}

// ��������Ͽ�����
void CGame::OnLostServer()
{
	AddLogText("============the Server's server closed==========!!!");
	m_lServerSocket = 0;
	CMessage::SetServerSocket(0);
}

bool CGame::StartConnectLogServer()
{
	m_LogClient.SetSoket(s_pNetServer->Connect(m_tgSetup.strLogIP.c_str(), m_tgSetup.dwLogPort,NF_LOGSERVER_CLIENT));
	if(m_LogClient.GetSoket() > 0)
	{
		CMessage::SetLogSocket(m_LogClient.GetSoket());
		AddLogText(CStringReading::LoadString(IDS_WS_GAME, STR_WS_GAME_CONNECTLOGSOK));

		CMessage msg(MSG_02LOG_SERVER_LOGIN);
		msg.Add(GetSetup()->dClientNo);
		msg.SendToSocket(m_LogClient.GetSoket());

		return true;
	}
	return false;
}

bool CGame::ReConnectLogServer()
{
	return GetLogClient().bConnectThreadExit = StartConnectLogServer();
}

// ����������־�������߳�
void CGame::CreateConnectLogThread()
{
	m_LogClient.bConnectThreadExit=true;
	WaitForSingleObject(m_LogClient.hConnectThread,INFINITE);
	CloseHandle(m_LogClient.hConnectThread);

	m_LogClient.bConnectThreadExit=false;

	unsigned int threadID;
	m_LogClient.hConnectThread = (HANDLE)_beginthreadex(NULL,0,ConnectLogServerFunc,NULL,0,&threadID);
}

void CGame::OnLostLogServer()
{
	AddLogText("============the LogServer closed==========!!!");
	m_LogClient.SetSoket(0);
	CMessage::SetLogSocket(0);
	CreateConnectLogThread();
}

void CGame::InitLogClient(CMessage *pMsg)
{
	LogClientInfo tmpLogClientInfo;
	tmpLogClientInfo.ErrLogFileName = GetSetup()->strErrFile;
	tmpLogClientInfo.LogFileName	= GetSetup()->strLogFile;
	tmpLogClientInfo.TxtLogFileName = GetSetup()->strTxtLogFile;
	tmpLogClientInfo.lClientNo		= GetSetup()->dClientNo;

	bool re = m_LogClient.Init(pMsg, tmpLogClientInfo);
	if(re)
	{
		if (NULL != m_pLogicLogInterface)
		{
			SAFE_DELETE(m_pLogicLogInterface);
		}
		m_pLogicLogInterface = new LogicLogInterface(&m_LogClient);
	}
}

void CGame::LogServerAffirm(CMessage *pMsg)
{
	if(0 == pMsg->GetLong())
		m_LogClient.PassAffirm(false);
	else
		m_LogClient.PassAffirm(true);
}

void CGame::LogServerCondition(CMessage *pMsg)
{
	m_LogClient.UpdateCondition(pMsg);
}

LogicLogInterface*	CGame::GetLogicLogInterface()
{
	assert(NULL != m_pLogicLogInterface); 
	return m_pLogicLogInterface;
}
// ��Ϸ�ͷ�
BOOL CGame::Release()
{
	PutDebugString("Server Exiting...");

#ifdef _RUNSTACKINFO_
	CMessage::AsyCloseFile(GetGame()->GetStackFileName());
#endif

	GetInst(CMemClient).Release();

	// �ȴ��������������߳̽���
	bConnectServerThreadExit = true;
	WaitForSingleObject(hConnectServerThread, INFINITE);
	CloseHandle(hConnectServerThread);

	// �ر���������
	if( GetNetServer() )
	{
		//�Ͽ���������˵�����
		s_pNetServer->DisConn(m_lServerSocket);
		s_pNetServer->Exit();
		SAFE_DELETE(s_pNetServer);
	}

	CMySocket::MySocketClearUp();
	CMessage::Release();

	// ��־ϵͳ
	SAFE_DELETE(m_pLogicLogInterface);
	m_LogClient.Release();

	return TRUE;
}

// ��Ϸ��ѭ��
BOOL CGame::MainLoop()
{
#ifdef _RUNSTACKINFO_
	CMessage::AsyClearFile(GetGame()->GetStackFileName());
	CMessage::AsyWriteFile(GetGame()->GetStackFileName(), "MainLoop() Start.");
#endif

	ProcessMessage();

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStackFileName(), "MainLoop() End.");
#endif
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
	long lMessageCount = GetNetServer()->GetRecvMessages()->GetSize();
	for(long i=0; i<lMessageCount; i++)
	{
		CMessage* pMsg = (CMessage*)GetNetServer()->GetRecvMessages()->PopMessage();
		if( pMsg )
		{
			pMsg->Run();
			CBaseMessage::FreeMsg(pMsg);
		}
	}
	return TRUE;
}

void OnLogServerMessage(CMessage *pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_LOG20_SERVER_TABLE://���ṹ��Ϣ
		{
			GetGame()->InitLogClient(pMsg);
		}
		break;
	case MSG_LOG20_SERVER_AFFIRM://ȷ�Ϸ�������LogServer�������õ����ݿ�
		{
			GetGame()->LogServerAffirm(pMsg);
		}
		break;
	case MSG_LOG20_SERVER_CONDITION://��Ϣ�ķ��Ϳ�����Ϣ
		{
			GetGame()->LogServerCondition(pMsg);
		}
		break;
	}
}


// ��Ϸ���߳�
unsigned __stdcall GameThreadFunc(void* pArguments)
{
#ifndef _DEBUG
	_try
	{
#endif
		CreateGame();
		BOOL bRet = GetGame()->Init();
		
		if( bRet )
		{
			while( true )
			{
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

		if( GetGame() )
		{
			GetGame()->Release();
		}
		DeleteGame();

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(), GetExceptionCode()))
	{
		PutDebugString("������ִ�����鿴���µĴ��󱨸��ļ�");
		AddErrorLogText("������ִ�����鿴���µĴ��󱨸��ļ�");
		
		if( GetGame() )
		{
			GetGame()->Release();
		}
		DeleteGame();

		Sleep(5000);

		WinExec("Server.exe", SW_SHOW);
		PostMessage(g_hWnd, WM_DESTROY, 0, 0);
		SetEvent(g_hGameThreadExitEvent);
		_endthreadex(0);
		return 0;
	}	
#endif

	SetEvent(g_hGameThreadExitEvent);
	PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	_endthreadex(0);
	return 0;

}

//
unsigned __stdcall ConnectServerFunc(void* pArguments)
{
	while(true)
	{
		if(bConnectServerThreadExit)
			break;
		Sleep(8000);

		if(GetGame()->ReConnectServer()==true)
		{
			break;
		}

	}
	SetEvent(hConnectServerThread);
	return 0;
}

unsigned __stdcall ConnectLogServerFunc(void* pArguments)
{
	while(true)
	{
		if(GetGame()->GetLogClient().bConnectThreadExit)
			break;
		Sleep(8000);

		if(GetGame()->ReConnectLogServer()==TRUE)
		{
			break;
		}

	}
	_endthreadex( 0 );
	return 0;
}