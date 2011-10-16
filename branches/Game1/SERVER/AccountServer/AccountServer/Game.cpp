#include "StdAfx.h"
#include <sstream>

#include "game.h"

#include "..\nets\netAccount\message.h"
#include "..\nets\servers.h"
#include "..\..\..\dbaccess\myadobase.h"
#include "..\public\readwrite.h"
#include "..\AppAccount\ValidateOperator.h"

long g_lVersion =	200;		// ����汾�ţ�ͬGameClientͳһ��

extern HWND g_hInfoText;

CGame* g_pGame= NULL;		// ��Ϸ�ͻ�������

HANDLE g_hGameThreadExitEvent=NULL;		// ���߳��˳��¼�
bool	g_bGameThreadExit=false;		// �Ƿ��˳����߳�


string g_strServerInfoConnStr;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
:m_pDBAllocator(NULL)
,m_AsynchronousExecute(TRUE)
,m_bReLoadLsSetup(FALSE)
{
	s_lAITick = 0;

	s_pNetServer_Client = NULL;
	m_bExit = false;

	m_SetupEx.lClientMaxBlockConNum = 10;
	m_SetupEx.lClientValidDelayRecDataTime = 4000;

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
		stream
			>> strTemp >> GetSetup()->lServerID
			>> strTemp >> GetSetup()->dwListenPort_Client

			>> strTemp >> GetSetup()->strSqlConType
			>> strTemp >> GetSetup()->strSqlServerIP
			>> strTemp >> GetSetup()->strSqlUserName
			>> strTemp >> GetSetup()->strSqlPassWord
			>> strTemp >> GetSetup()->strDBName

			>> strTemp >> GetSetup()->strSecuritySqlConType		
			>> strTemp >> GetSetup()->strSecuritySqlServerIP
			>> strTemp >> GetSetup()->strSecuritySqlUserName
			>> strTemp >> GetSetup()->strSecuritySqlPassWord
			>> strTemp >> GetSetup()->strSecurityDBName

			>> strTemp >> GetSetup()->strFcmSqlConType		
			>> strTemp >> GetSetup()->strFcmSqlServerIP
			>> strTemp >> GetSetup()->strFcmSqlUserName
			>> strTemp >> GetSetup()->strFcmSqlPassWord
			>> strTemp >> GetSetup()->strFcmDBName

			>> strTemp >> GetSetup()->dwDataBlockNum
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
			>> strTemp >> GetSetup()->dwDoQueueInter
			>> strTemp >> GetSetup()->dwSendMsgToQueInter

			>> strTemp >> GetSetup()->dwWorldMaxPlayers
			>> strTemp >> GetSetup()->fWorldBusyScale		// ��æ����������
			>> strTemp >> GetSetup()->fWorldFullScale		// ��������������

			>> strTemp >> GetSetup()->dwPingWorldServerTime
			>> strTemp >> GetSetup()->dwPingWorldServerErrorTime

			>> strTemp >> GetSetup()->bCheckForbidIP
			>> strTemp >> GetSetup()->bCheckAllowIP
			>> strTemp >> GetSetup()->bCheckBetweenIP

			>> strTemp >> GetSetup()->dwMinDbOptThreadNum		//! ��СDB�����̱߳�����
			>> strTemp >> GetSetup()->dwMaxDbOptThreadNum		//! ���DB�����߳���
			>> strTemp >> GetSetup()->dwThreadNumAdjustSpace	//! �Զ������߳����ļ��ʱ��
			;

		stream.close();

		AddLogText("���������ļ�...ok!");
		return true;
	}

	// �Ӽ����ļ���ȡ
	else
	{
		int length = GetFileLength("setup.dat");

		FILE *fp;
		fp = fopen("setup.dat", "rb");
		if (fp == NULL)
		{
			return false;
		}

		char *strIn = new char[length+1];
		char *strOut = new char[(length+1)*2];
		if( strIn == NULL ) return false;
		memset(strIn, 0, length+1);
		memset(strOut, 0, (length+1)*2);

		fread(strIn, length, 1, fp);
		fclose(fp);

		IniDecoder(strIn, strOut, length);

		stringstream streamSetup;

		streamSetup << strOut;
		streamSetup.seekg(0, std::ios_base::beg);

		streamSetup
			>> strTemp >> GetSetup()->lServerID
			>> strTemp >> GetSetup()->dwListenPort_Client

			>> strTemp >> GetSetup()->strSqlConType
			>> strTemp >> GetSetup()->strSqlServerIP
			>> strTemp >> GetSetup()->strSqlUserName
			>> strTemp >> GetSetup()->strSqlPassWord
			>> strTemp >> GetSetup()->strDBName

			>> strTemp >> GetSetup()->strFcmSqlConType		
			>> strTemp >> GetSetup()->strFcmSqlServerIP
			>> strTemp >> GetSetup()->strFcmSqlUserName
			>> strTemp >> GetSetup()->strFcmSqlPassWord
			>> strTemp >> GetSetup()->strFcmDBName

			>> strTemp >> GetSetup()->dwDataBlockNum
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
			>> strTemp >> GetSetup()->dwDoQueueInter
			>> strTemp >> GetSetup()->dwSendMsgToQueInter

			>> strTemp >> GetSetup()->dwWorldMaxPlayers
			>> strTemp >> GetSetup()->fWorldBusyScale		// ��æ����������
			>> strTemp >> GetSetup()->fWorldFullScale		// ��������������

			>> strTemp >> GetSetup()->dwPingWorldServerTime
			>> strTemp >> GetSetup()->dwPingWorldServerErrorTime

			>> strTemp >> GetSetup()->bCheckForbidIP
			>> strTemp >> GetSetup()->bCheckAllowIP
			>> strTemp >> GetSetup()->bCheckBetweenIP

			>> strTemp >> GetSetup()->dwMinDbOptThreadNum		//! ��СDB�����̱߳�����
			>> strTemp >> GetSetup()->dwMaxDbOptThreadNum		//! ���DB�����߳���
			>> strTemp >> GetSetup()->dwThreadNumAdjustSpace	//! �Զ������߳����ļ��ʱ��
			;

		AddLogText("���������ļ�...ok!");
		return true;
	}

	return false;
}

bool CGame::ReLoadSetup()
{
	LoadSetup();
	return true;
}

bool CGame::LoadSetupEx()
{
	string strTemp;
	ifstream stream;
	stream.open("setupex.ini");
	if( stream.is_open() )
	{
		stream  >> strTemp >> GetSetupEx()->lClientMaxBlockConNum
			>> strTemp >> GetSetupEx()->lClientValidDelayRecDataTime;
		return true;
	}
	return false;
}

bool CGame::ReLoadSetupEx()
{
	LoadSetupEx();

	s_pNetServer_Client->SetParamEx(GetSetupEx()->lClientMaxBlockConNum,GetSetupEx()->lClientValidDelayRecDataTime);

	return true;
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
	
	PutDebugString("AccountServer start!");

	SET_OUT_INFO_SETUP(OutMemInfo, "log\\");

	if (LoadSetup()==false)
	{
		MessageBox(g_hWnd, "Can't find file setup.ini", "message", MB_OK);
		return false;
	}

	CMyAdoBase::Initialize(
		GetSetup()->strSqlConType,
		GetSetup()->strSqlServerIP,
		GetSetup()->strDBName,
		GetSetup()->strSqlUserName,
		GetSetup()->strSqlPassWord
		);

	GetConnectionString() = 
		"Provider=" + GetSetup()->strSqlConType +
		"; Server=" + GetSetup()->strSqlServerIP + 
		"; Database=" + GetSetup()->strDBName + 
		"; UID=" + GetSetup()->strSqlUserName + 
		"; PWD=" + GetSetup()->strSqlPassWord + 
		"; OLE DB Services=-1; Driver={SQL Server}";

	GetSecurityConnectionString() = 
		"Provider=" + GetSetup()->strSecuritySqlConType +
		"; Server=" + GetSetup()->strSecuritySqlServerIP + 
		"; Database=" + GetSetup()->strSecurityDBName + 
		"; UID=" + GetSetup()->strSecuritySqlUserName + 
		"; PWD=" + GetSetup()->strSecuritySqlPassWord + 
		"; OLE DB Services=-1; Driver={SQL Server}";

	GetFcmConnectionString() =
		"Provider=" + GetSetup()->strFcmSqlConType +
		"; Server=" + GetSetup()->strFcmSqlServerIP + 
		"; Database=" + GetSetup()->strFcmDBName + 
		"; UID=" + GetSetup()->strFcmSqlUserName + 
		"; PWD=" + GetSetup()->strFcmSqlPassWord + 
		"; OLE DB Services=-1; Driver={SQL Server}";

	char strVersion[256];
	extern long g_lVersion;
	sprintf(strVersion,"AccountServer[%d][%d]-StarDust", g_lVersion, GetSetup()->dwListenPort_Client);

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

	if(LoadSetupEx() == false)
	{
		AddLogText("Can't find file SetupEx.ini");
	}

	//! ��ʼ���ͻ�����Ϣ
	GetClientManager().Init("ClientSetup.ini");
	//! ��ʼ���첽ִ�ж���
	GetAsynchronousExecute().SetWorkParam(
		GetSetup()->dwMinDbOptThreadNum, 
		GetSetup()->dwMaxDbOptThreadNum, 
		GetSetup()->dwThreadNumAdjustSpace
		);
	GetAsynchronousExecute().BeginWork(GetSetup()->dwMinDbOptThreadNum);


	//�������ݿ�
	m_pDBAllocator = new CDataBlockAllocator(m_Setup.dwDataBlockNum,m_Setup.dwDataBlockSize);

	//��ʼ
	CMessage::Initial(m_pDBAllocator,m_Setup.dwFreeMsgNum);
	// ��ʼ������������
	CMySocket::MySocketInit();

	// 
	if( !InitNetServer_Client() )
	{
		MessageBox(g_hWnd, "Can't init NetServerClient!", "Message", MB_OK);
		return false;
	}

	AddLoginClientList("��½�ͻ����б�");

	return true;
}

// ��Ϸ�ͷ�
BOOL CGame::Release()
{
	PutDebugString("AccountServer Exiting...");
	if( !m_bExit )
	{
		m_bExit = true;
		
		//! �����첽ִ�ж���
		GetAsynchronousExecute().Exit();
		//! �ͷſͻ�����Ϣ
		GetClientManager().Release();
		
		if( GetNetServer_Client() )
		{
			s_pNetServer_Client->Exit();
		}

		SAFE_DELETE(s_pNetServer_Client);

		CMySocket::MySocketClearUp();		

		ValidateOperator::Release();
		CMyAdoBase::Uninitalize();

		CMessage::Release();

		SAFE_DELETE(m_pDBAllocator);

		OUT_RUNNING_INFO(true);
	}

	PutDebugString("AccountServer Exited!");
	return true;
}

//
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

	BOOL bRet = s_pNetServer_Client->Host(m_Setup.dwListenPort_Client,NULL,NF_AsscountServer_Client,SOCK_STREAM);
	if(!bRet)
	{
		MessageBox(g_hWnd, "Start Host for client Error", "Message", MB_OK);
		return FALSE;
	}

	CMessage::SetClientServer(s_pNetServer_Client);
	
	AddLogText("AccountServer ��ΪLoginServer�ķ�������ʼ���ɹ� !");
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

	return TRUE;
}

VOID CGame::DisConnClient(LONG lSocket)
{
	s_pNetServer_Client->DisConn(lSocket);
}

// ��Ϸ��ѭ��
BOOL CGame::MainLoop()
{
/////////////////////////////////////////////////////////
//! ������������
#ifdef APEX_TEST

	static INT64 lSendNum = 0;
	static INT64 lLastSendNum = 2;
	static INT64 lValidateNum = 0;
	static LONG lTime = timeGetTime();
	static LONG lOldTmp = 0;

	LONG lCurrTime = timeGetTime();
	LONG lTmp = (lCurrTime - lTime) / 1000;
	if(lTmp != lOldTmp && 0 == (lTmp % 5) && 1000 < (lCurrTime - lTime))
	{
		lOldTmp = lTmp;
		float fValidateSpeed = lValidateNum / (((float)(lCurrTime - lTime)) / 1000.0f);
		char szInfo[256] = {0};
		sprintf(szInfo, "������ƽ����֤�ٶȣ�[%f/��]", fValidateSpeed);
		SetWindowText(g_hInfoText, szInfo);
	}
	lValidateNum = ValidateOperator::GetValidateNum();
	if(lSendNum == lValidateNum)
	{
		++lLastSendNum;
	}
	if(64 < lSendNum - lValidateNum)
	{
		if(0 < lLastSendNum)
			--lLastSendNum;
	}

	for (LONG i = 0; i < lLastSendNum; ++i)
	{
		tagAccountInfo_ToAS AccountInfo_ToAS;
		sprintf(AccountInfo_ToAS._szCdkey, "gm00%d", (rand() % 100));
		sprintf(AccountInfo_ToAS._szPwd, "698D51A19D8A121CE581499D7B701668");
		AccountInfo_ToAS._lSocket = -1;

		ValidateOperator *pValidateOperator = ValidateOperator::CreateValidateOperator(AccountInfo_ToAS, AccountInfo_ToAS._lSocket);

		assert(NULL != pValidateOperator);
		if(NULL == pValidateOperator)
		{
			break;
		}
		if(!pValidateOperator->BeginValidate())
		{
			pValidateOperator->SendResult();
			ValidateOperator::ReleasValidateOperator(&pValidateOperator);
		}
	}
	
	lSendNum += lLastSendNum;
#endif
/////////////////////////////////////////////////////////

	// �����߳�
	if( m_bExit )
	{
		return false;
	}

	// ��Ϣ����
	ProcessMessage();

	if(m_bReLoadLsSetup)
	{
		GetClientManager().ReLoadClientSetup("ClientSetup.ini");
		m_bReLoadLsSetup = FALSE;
	}

	//! 5���Ӽ�¼һ����Ϣ
	static DWORD LastTime = timeGetTime();
	DWORD CurrTime = timeGetTime();
	if(5 * 60 * 1000 < CurrTime - LastTime)
	{
		CurrTime = LastTime;
		OUT_RUNNING_INFO(false);
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
	PostMessage(g_hWnd, WM_CLOSE, 0, 0);

	_endthreadex( 0 );
	return 0;
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