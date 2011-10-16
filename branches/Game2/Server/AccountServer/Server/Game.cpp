#include "StdAfx.h"

#include "Game.h"
#include "../App/ValidateOperator.h"
#include "../App/ValidateOperator.h"
#include "../Net/Message.h"

#include "../../../Public/AppFrame/TextRes.h"
using namespace AppFrame;

long g_lVersion =	200;	// ����汾�ţ�ͬGameClientͳһ��
CGame* g_pGame= NULL;		// ��Ϸ�ͻ�������

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum MyMenuID
{
    // (����)LOAD
    LOAD_LS_LIST = MY_MENU * 2,
};

void AddLoginClientList(const char *str)
{
    AppFrame::AddToList("list",str);
}

void DelLoginClientList(const char *str)
{
    AppFrame::DelFromList("list", str);
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


CGame::CGame(void)
:m_pDBAllocator(NULL)
,m_AsynchronousExecute(TRUE)
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
        Log4c::Trace(ROOT_MODULE,GetText("AS_GAME_0"));
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

         Log4c::Trace(ROOT_MODULE,GetText("AS_GAME_0"));//���������ļ�...ok!
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
// ��Ϸ��ʼ��
BOOL CGame::Init()
{
	srand((unsigned)time(NULL));
	random(100);
	CreateDirectory( "log",	NULL);


	//////////////////////////////////////////////////////////////////////////add by shixi
	LoadServerResource();
	//��ȡȫ���ַ���
	AppFrame::LoadTextRes();
	//////////////////////////////////////////////////////////////////////////

    Log4c::Trace(ROOT_MODULE,GetText("AS_GAME_2"));//AccountServer start!

	if (LoadSetup()==false)
	{
        AppFrame::NotifyMessageBox("Can't find file setup.ini");
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

	char title[256];
	sprintf(title, "[%d]AccountServer[%d]", GetSetup()->dwListenPort_Client, g_lVersion);
    if (!AppFrame::SetTitle(title))
        return false;

	if(LoadSetupEx() == false)
	{
        Log4c::Trace(ROOT_MODULE,"Can't find file SetupEx.ini");
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
	CMessage::Initial(m_pDBAllocator,m_Setup.dwFreeMsgNum);
	CMySocket::MySocketInit();

	if( !InitNetServer_Client() )
	{
		AppFrame::NotifyMessageBox("Can't init NetServerClient!");
		return false;
	}

	AddLoginClientList("��½�ͻ����б�");
	UnloadServerResource();
	return true;
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
		//ASSERT(false);
		//return false;
    }
	
    Log4c::Trace(ROOT_MODULE,"Load package file OK!");
	return true;
}

bool CGame::UnloadServerResource()
{
	CClientResource* pClientResource = CClientResource::GetInstance();
	if(pClientResource)
	{
		pClientResource->Release();
		CClientResource::ReleaseInstance();
	}
	return true;
}


// ��Ϸ�ͷ�
BOOL CGame::Release()
{
    Log4c::Trace(ROOT_MODULE,"AccountServer Exiting...");
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
	}
    Log4c::Trace(ROOT_MODULE,"AccountServer Exited!");

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
		AppFrame::NotifyMessageBox("Start Client Net server Error");
		return FALSE;
	}

	BOOL bRet = s_pNetServer_Client->Host(m_Setup.dwListenPort_Client,NULL,NF_AsscountServer_Client,SOCK_STREAM);
	if(!bRet)
	{
		AppFrame::NotifyMessageBox("Start Host for client Error");
		return FALSE;
	}

	CMessage::SetClientServer(s_pNetServer_Client);
	
    Log4c::Trace(ROOT_MODULE,GetText("AS_GAME_1"));//AccountServer ��ΪLoginServer�ķ�������ʼ���ɹ� !

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

bool CGame::ProcMenus()
{
    bool res = false;
    if (AppFrame::MenuClicked(LOAD_LS_LIST))
        GetClientManager().ReLoadClientSetup("ClientSetup.ini");

    return res;
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

    // �˵�����
    ProcMenus();

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
        AppFrame::ToggleMenus();

		while(ret)
		{
			// �����߳�
			if (AppFrame::IsExit() || !GetGame()->MainLoop() )
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
		if(GetGame())
			GetGame()->Release();
		DeleteGame();

        AppFrame::ExitThread();
		return 0;
	}
#endif

    AppFrame::ExitThread();
	return 0;
}


unsigned __stdcall UpdateOnlineUser2DB(void *param) {

	::CoInitialize(NULL);
	if(param) {
		struct tagTreadArgs {
			long wid;
			list<string> *list;

			~tagTreadArgs() { list->clear(); SAFE_DELETE(list); }
		};
	
		char sql[1024];
		_ConnectionPtr cn;
		_RecordsetPtr rs;
		tagTreadArgs *tempParam = (tagTreadArgs *)param;

		if( CMyAdoBase::CreateCn(cn) && CMyAdoBase::OpenCn(cn) ) {

			sprintf(sql, "DELETE FROM online_user WHERE worldNumber=%d", tempParam->wid);
			CMyAdoBase::ExecuteCn(sql,cn);

			for( list<string>::iterator i = tempParam->list->begin(); i != tempParam->list->end(); ++i ) {
				sprintf(sql, "INSERT INTO online_user(userAccount, worldNumber) VALUES('%s',%d)",
					(*i).data(), tempParam->wid);
				CMyAdoBase::ExecuteCn(sql, cn);
			}

			CMyAdoBase::CloseCn(cn);
		}
		CMyAdoBase::ReleaseCn(cn);

		SAFE_DELETE(param);
	}

	::CoUninitialize();
	_endthreadex(0);
	return 0;
}
