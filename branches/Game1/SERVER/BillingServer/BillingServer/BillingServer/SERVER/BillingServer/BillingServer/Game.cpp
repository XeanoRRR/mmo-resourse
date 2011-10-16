#include "StdAfx.h"
#include "Game.h"
#include "..\Nets\NetBilling\ServerForGS.h"
#include "..\Nets\NetBilling\ClientForGS.h"
#include "..\Nets\NetBilling\Message.h"
#include "..\Nets\MySocket.h"
#include "..\AppBilling\billingplayermanager.h"

HANDLE	g_hGameThreadExitEvent = NULL;		// ���߳��˳��¼�
bool	g_bGameThreadExit = false;			// �Ƿ��˳����߳�
CGame	*g_pGame = NULL;					// ��Ϸ�ͻ�������
CGame::CGame(void)
: m_pGSServer(NULL)
, m_bExit(false)
{
}

CGame::~CGame(void)
{
}

// ��������Setup.ini
bool CGame::LoadSetup()
{
	char strTemp[256];

	ifstream stream;
	stream.open("Setup.ini");
	if( stream.is_open() )
	{
		stream  >> strTemp >> GetSetup()->dwGSListenPort
			//>> strTemp >> GetSetup()->strSqlConType
			>> strTemp >> GetSetup()->strSqlServerIP
			>> strTemp >> GetSetup()->strSqlUserName
			>> strTemp >> GetSetup()->strSqlPassWord
			>> strTemp >> GetSetup()->strDBName
			>> strTemp >> GetSetup()->bLogSvrSwitch
			>> strTemp >> GetSetup()->strLogSvrIP
			>> strTemp >> GetSetup()->strLogSvrUsrName
			>> strTemp >> GetSetup()->strLogSvrPwd
			>> strTemp >> GetSetup()->strLogDBName
			>> strTemp >> GetSetup()->dwRefeashInfoTime
			>> strTemp >> GetSetup()->dwSaveInfoTime
			>> strTemp >> GetSetup()->dwSaveLogSvrTime
			>> strTemp >> GetSetup()->dwDBIOThreadNum
			;

		stream.close();

		AddLogText("Load Setup.ini Success");

		sprintf(strTemp, "BillingServer[%d]-FY", GetSetup()->dwGSListenPort);

		//�鿴�˳����Ƿ���������
		HWND hWnd = FindWindow(NULL,strTemp);
		if (hWnd != NULL)
		{
			char strMsgBox[256]="";
			sprintf(strMsgBox,"%s Is RUNNING!",strTemp);
			MessageBox(g_hWnd,_T(strMsgBox), "error", MB_OK);
			return false;
		}

		SendMessage(g_hWnd, WM_SETTEXT, 0, (LPARAM)strTemp);

		return true;
	}
	return false;
}

bool CGame::ReLoadSetup()
{
	LoadSetup();

	return true;
}

// Reload GSInfoSetup.ini
bool CGame::LoadGSSetup()
{ 
	bool bRes = m_pGSServer->LoadAllowedClient("GSInfoSetup.ini");
	if( bRes )
		AddLogText( "BillingServer As GS Server Init Successfully!" );
	else
		AddLogText( "WARNNING : BillingServer As GS Server Init FAILED!!!" );

	return bRes;
}

// ��Ϸ��ʼ��
bool CGame::Init()
{
	srand((unsigned)time(NULL));

	if( !LoadSetup() )
		return false;

	// ��ʼ
	CBaseMessage::Initial();

	// ��ʼ������������
	CMySocket::MySocketInit();

	// ����GS��������
	if( !InitServer() )
	{
 		MessageBox(g_hWnd, "InitServer Error", "Message", MB_OK);
		return false;
	}

	// Create BillingPlayerManager thread
	if( !InitBPManager() )
	{
 		MessageBox(g_hWnd, "InitBPManager Error", "Message", MB_OK);
		return false;
	}


	return true;
}

bool CGame::InitServer()
{
	// ��ʼ����Ϸ
	SAFE_DELETE( m_pGSServer );
	m_pGSServer = new CServerForGS;

	// ��ΪGS�ķ�������ʹ����ɶ˿�
	int hr = m_pGSServer->Host( m_Setup.dwGSListenPort, NULL, SOCK_STREAM, true );
	if( !hr )
	{
		MessageBox( g_hWnd, "Host Error", "Message", MB_OK );
		return false;
	}

	LoadGSSetup();

	// ��ȡ����IP��ַ
	char strName[128] = "";
	SOCKADDR_IN sockAddr;
	gethostname( strName, 128 );
	LPHOSTENT lphost = gethostbyname( strName );
	if ( lphost )
	{
		sockAddr.sin_addr.s_addr = ( ( LPIN_ADDR ) lphost->h_addr )->s_addr;
		m_pGSServer->SetDwordIP( sockAddr.sin_addr.S_un.S_addr );
		sprintf( strName, "%s", inet_ntoa(sockAddr.sin_addr) );

		m_pGSServer->SetIP( strName );
	}
	else
	{
		assert( 0 );
	}

	return true;
}

// Create BillingPlayerManager threads
bool CGame::InitBPManager()
{
	CBillingPlayerManager::Start(GetSetup()->bLogSvrSwitch);
	for( DWORD i=0; i<GetSetup()->dwDBIOThreadNum; ++i )
	{
		CBillingPlayerManager *pBPManager = new CBillingPlayerManager;
		if( !pBPManager || !pBPManager->CreateThread() )
			return false;
		vecBPM.push_back(pBPManager);
	}

	return true;
}

// Realse BillingPlayerManager threads
bool CGame::ReleaseBPManager()
{
	vector<CBillingPlayerManager*>::iterator itBPM = vecBPM.begin();
	for( ; itBPM!=vecBPM.end(); ++itBPM )
	{
		(*itBPM)->Release();
		delete (*itBPM);
	}
	vecBPM.clear();

	CBillingPlayerManager::End(GetSetup()->bLogSvrSwitch);

	return true;
}

// ������Ϸ�ͻ�������
bool CreateGame()
{
	g_pGame = new CGame;
	return true;
}

// ��ȡ��Ϸ�ͻ�������
CGame* GetGame()
{
	return g_pGame;
}

// ��Ϸ�ͷ�
bool CGame::Release()
{
	PutDebugString( "BillingServer Exiting" );

	ReleaseBPManager();

	if( !m_bExit )
	{
		m_bExit = true;

		if( m_pGSServer )
		{
			m_pGSServer->ExitWorkerThread();
		}

		SAFE_DELETE( m_pGSServer );
		CMySocket::MySocketClearUp();
		CBaseMessage::Release();
	}

	PutDebugString("BillingServer Exited");
	return true;
}


// �ͷ���Ϸ�ͻ�������
bool DeleteGame()
{
	SAFE_DELETE( g_pGame );
	return true;
}

// ��Ϣ����
bool CGame::ProcessMessage()
{
	if( m_pGSServer )
	{
		long lMessageCount = m_pGSServer->GetRecvMessages()->GetSize();
		for( long i=0; i<lMessageCount; ++i )
		{
			CMessage* pMsg =(CMessage*)m_pGSServer->GetRecvMessages()->PopMessage();
			if( pMsg )
			{
				pMsg->Run();
				delete pMsg;
			}
		}
	}

	return true;
}

// ��Ϸ��ѭ��
bool CGame::MainLoop()
{
	static DWORD  dwCurTime = timeGetTime(), dwLastRefreashInfoTime = dwCurTime;
	static DWORD dwServerInfoLogLastTime = dwCurTime;

	dwCurTime = timeGetTime();

	// ˢ����ʾ
	if ( dwCurTime - dwLastRefreashInfoTime > GetSetup()->dwRefeashInfoTime )
	{
		dwLastRefreashInfoTime = dwCurTime;
		RefreashInfo();
	}

	// �����߳�
	if( m_bExit )
	{
		return false;
	}

	// ��Ϣ����
	ProcessMessage();

	//GetBPManager()->Run();

	// ��ʱPING���µ�GS��Ϣ
	static DWORD dwLastPingTime = dwCurTime;
	static bool bInPing = false;

	if ( !bInPing )
	{
		if (dwCurTime - dwLastPingTime > 60000 )
		{
			dwLastPingTime = dwCurTime;
			bInPing = true;
		}
	}

	Sleep(1);
	return true;
}

// ��Ϸ���߳�
unsigned __stdcall GameThreadFunc(void* pArguments)
{
#ifndef _DEBUG
	__try
	{
#endif
		CreateGame();
		bool ret = GetGame()->Init();

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

		if( GetGame() )
		{
			GetGame()->Release();
		}
		DeleteGame();

#ifndef _DEBUG
	}
	__except( _Sword3::CrashFilter( GetExceptionInformation(), GetExceptionCode() ) )
	{
		MessageBox( g_hWnd, "GameThread Dump, Please Check Last Error Log", "Error", MB_OK );

		PostMessage( g_hWnd, WM_DESTROY, 0, 0 );
		SetEvent( g_hGameThreadExitEvent );
		_endthreadex( 0 );
		return 0;
	}
#endif

	SetEvent( g_hGameThreadExitEvent );
	PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
	_endthreadex( 0 );
	return 0;
}