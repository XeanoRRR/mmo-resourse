#include "StdAfx.h"
#include <time.h>

#include "..\LogClient\gs\LogicLogInterface.h"
#include ".\game.h"
#include "appserver\serverregion.h"


#include "..\nets\Servers.h"
#include "..\nets\netserver\message.h"
#include "appserver\goods\cgoods.h"
#include "appserver\Player.h"
#include "appserver\Session\CSessionFactory.h"
#include "appserver/monster.h"
#include "appserver/pksys.h"
#include "appserver\script\script.h"
#include "appserver\script\GlobalScriptTimer.h"

#include "appserver/goods/CGoodsFactory.h"
#include "appserver/goods/CSuitRuleFactory.h"
#include "appserver/goods/enchase/CKitCardRuleFactory.h"

#include "..\public\WordsFilter.h"
#include "..\public\dupliregionsetup.h"
#include "..\public\DataBlockAllocator.h"
#include "..\public\OccupSkillList.h"
#include "..\public\StudySkillCondition.h"
#include "..\public\DeputyOccu.h"
#include "..\public\StrUnifyInput.h"
#include "..\public\ReLiveGoodsList.h"
#include "..\public\Cosmetology.h"

#include "..\setup\MailList.h"
#include "../AppServer/Message/SequenceString.h"
#include "setup\questsystem.h"
#include "appserver\country\countryparam.h"
#include "appserver\country\CountryHandler.h"
#include "GameManager.h"

// [2007-7-16-add]
#include "../AppServer/OrganizingSystem/GameOrganizingCtrl.h"
#include "..\public\OrganizingParam.h"
//! [add end]

#include "../AppServer/goods/CSuitRuleFactory.h"

#include "..\appserver\Organizingsystem\gameunion.h"
#include "appserver\Organizingsystem\gamefaction.h"
#include "..\AppServer\Message\MsgOutTimeDef.h"
#include "..\AppServer\BuffSkill\NewSkillFactory.h"

#include "../public/public.h"

#include "../AppServer/RgnManager.h"
#include "../AppServer/script/VariableList.h"
#include "../AppServer/business/CBusinessManager.h"
#include "../AppServer/Linkman/LinkmanSystem.h"
#include "../AppServer/AI/AIFactory.h"
#include "../public/Crc32Static.h"


#include "../AppServer/PlayerMan/PlayerMan.h"
#include "../AppServer/PetCtrl.h"
#include "..\AppServer\SMClient.h"
#include "../Appserver/AntiCheat/CPlayerManager.h"
#include "..\public\CreditSystem.h"
#include "../AppServer/FBusiness/ShopManager.h"

extern bool IncShopProcessing( CPlayer* pPlayer, DWORD dwGoodsIndex, DWORD dwNum, DWORD dwTradeMode, long lTotalAmount);
extern int SNDACALLBACK MsgHandle(int msgType, int nMsgFrom, int nMsgSeq, ISDOAMsg* req) ;




CGame* g_pGame= NULL;					// ��Ϸ�ͻ�������
HANDLE g_hGameThreadExitEvent=NULL;		// ���߳��˳��¼�
bool	g_bGameThreadExit=false;		// �Ƿ��˳����߳�

HANDLE	hConnectThread=NULL;			// WS�����߳̾�� 
bool	bConnectThreadExit=false;		// �Ƿ��˳�WS�����߳�
HANDLE	hConnectThreadDBS=NULL;			// DBS�����߳̾�� 
bool	bConnectThreadExitDBS=false;		// �Ƿ��˳�DBS�����߳�
HANDLE	hConnectThreadLogS=NULL;			// DBS�����߳̾�� 
bool	bConnectThreadExitLogS=false;		// �Ƿ��˳�DBS�����߳�
HANDLE	hConnectThreadBouns = NULL;			// ����BounsServer���߳̾��
bool	bConnectThreadExitBouns = false;	// �Ƿ��˳�BounsS�������߳�

bool	bConnectThreadExitBS=false;
HANDLE	hConnectThreadBS=NULL;

int g_ms = 5;		// ÿһ���ʱ�䣨���룩

HANDLE g_hSavingThread = NULL; // �����߳̾��

unsigned __stdcall ConnectDBFunc( void* pArguments );

extern unsigned __stdcall SaveThreadFunc(void* pArguments);



//�ض�������ñ�ʾ
unsigned volatile RELOAD_SETUP_FLAGS = 0x0;	// �ض������ļ���ʾ
extern const unsigned RELOAD_SETUP = 0x00000001;
extern const unsigned RELOAD_SETUPEX = 0x00000002;
extern const unsigned RELOAD_SETUPMAPFILE = 0x00000004;
extern const unsigned OUTPUT_SCRIPT_INFO = 0x00000008;
extern const unsigned OUTPUT_MEMPOOL_INFO = 0x00000010;



tagScriptDataInfo::tagScriptDataInfo(tagScriptDataInfoParam* pParam)
	: pInfoData(NULL),lInfoSize(0)
{
	if(!pParam) return;
	char* pData = pParam->pInfoData;
	long lSize  = pParam->lInfoSize;
	if(pData && lSize > 0)
	{
		lInfoSize = lSize;
		pInfoData = (char*)M_ALLOC(sizeof(char)*lInfoSize);
		memcpy(pInfoData, pData, lSize);
	}
}
tagScriptDataInfo::~tagScriptDataInfo(void)
{
	M_FREE(pInfoData, sizeof(char)*lInfoSize);
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
: b_IsFirstConnectionWS(true),m_bIsGenVarInit(false),g_lSaveDeltaTime(0)
{

	m_ServerInitSendFlag = 0;
	InitializeCriticalSection(&m_CSServerInitSendFlag);

	s_lAITick = 0;

	m_dwWSNumber=0;

	m_bBSConnected=FALSE;

	s_pNetServer = NULL;
	m_pDupliRegionSetup = NULL;
	m_pDBAllocator = NULL;

	m_pSDOAOrderHandler=NULL;
	m_bSDOAInitialized=false;

	m_Setup.dwListenPort = 0;
	m_Setup.dwWorldPort = 0;

	m_strFunctionList_FileData = NULL;
	m_strVariableList_FileData = NULL;

	m_pLogicLogInterface= new LogicLogInterface(NULL);

	m_Setup.lMaxBlockConNum = 10;
	m_Setup.lValidDelayRecDataTime = 4000;


	m_lFuncFileDataLen = 0;
	m_lVariableFileDataLen = 0;
}

CGame::~CGame(void)
{
	m_Setup.strLocalIP.clear();
	m_Setup.strWorldIP.clear();

	DeleteCriticalSection(&m_CSServerInitSendFlag);


	//��ʱ������������Ϣ
	char pszMemInfo[1024]="";
	_snprintf(pszMemInfo,1024,"UsingScriptNum:%d",g_UsingScript.size());
	PutStringToFile("MemAllocInfo",pszMemInfo);

	CGoodsFactory::OutputUseGoodsInfo("MemAllocInfo");
	CNewSkillFactory::OutputBasemodelInfo("MemAllocInfo");

	OutPlayerAndRegionInfo("MemAllocInfo");

	OUT_RUNNING_INFO(true);
}

bool CGame::LoadSetup()
{
	// ��ȡȫ������
	ifstream streamLog;

	streamLog.open("LogSetup.ini");
	string strTemp;
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

	ifstream streamBS;
	streamBS.open("BillSetup.ini");

	if(streamBS.is_open())
	{
		streamBS>>strTemp>>GetSetup()->lUseBillingServer;
		m_listBSSetup.clear();
		while(ReadTo(streamBS,"#"))
		{
			tagBSSetup stBSSetup;
			streamBS>>stBSSetup.dwIndex>>stBSSetup.strIp>>stBSSetup.dwPort;
			m_listBSSetup.push_back(stBSSetup);
		}
	}
	else
	{
		GetSetup()->lUseBillingServer=BILL_CONNECT_NONE;
		MessageBox(g_hWnd, "Can't find file BillSetup.ini", "message", MB_OK);
		return false;
	}

	ifstream streamBDS;
	streamBDS.open("BounsSetup.ini");
	if(streamBDS.is_open())
	{
		streamBDS >> strTemp >> GetSetup()->lUseBounsServer;
		streamBDS >> strTemp >> GetSetup()->dwGameServerNum;
		m_listBDSSetup.clear();
		while(ReadTo(streamBDS,"#"))
		{
			tagBSSetup tgBDSSetup;
			streamBDS >> tgBDSSetup.dwIndex >> tgBDSSetup.strIp >> tgBDSSetup.dwPort;
			m_listBDSSetup.push_back(tgBDSSetup);
		}
	}
	else
	{
		GetSetup()->lUseBounsServer = 0;
		MessageBox(g_hWnd, "Can't find file BounsSetup.ini", "message", MB_OK);
		return false;
	}

	ifstream stream;
	stream.open("setup.ini");
	if( stream.is_open() )
	{
		char str[256];
		stream >> str >> GetSetup()->strWorldIP
			>> str >> GetSetup()->dwWorldPort
			>> str >> GetSetup()->dwListenPort
			>> str >> GetSetup()->strLocalIP;

		stream.close();

		_snprintf(str, 256, "GameServer[%d]-StarDust", GetSetup()->dwListenPort);

		//�鿴�˳����Ƿ���������
		HWND hWnd = FindWindow(NULL,str);
		if (hWnd != NULL)
		{
			char strMsgBox[256]="";
			_snprintf(strMsgBox,256, "%s ��������������!",str);
			MessageBox(g_hWnd,_T(strMsgBox), "error", MB_OK);
			return false;
		}

		SendMessage(g_hWnd, WM_SETTEXT, 0, (LPARAM)str );
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file Setup.ini", "message", MB_OK);
		return false;
	}


	ifstream streamSetupEx;
	streamSetupEx.open("SetupEx.ini");
	if( streamSetupEx.is_open() )
	{
		streamSetupEx  >> strTemp >> GetSetup()->lMaxBlockConNum
			>> strTemp >> GetSetup()->lValidDelayRecDataTime
			>> strTemp >> GetSetup()->dwDataBlockNum
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
			>> strTemp >> GetSetup()->dwRefeashInfoTime
			>> strTemp >> GetSetup()->dwSaveInfoTime
			>> strTemp >> GetSetup()->dwGoodsLogTime
			>> strTemp >> GetSetup()->bWatchRunTimeInfo
			>> strTemp >> GetSetup()->dwWatchRunTime
			>> strTemp >> GetSetup()->dwMsgValidateTime;
		streamSetupEx.close();
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file SetupEx.ini", "message", MB_OK);
		return false;
	}
	return true;
}

//�ض������ļ�
bool	CGame::ReLoadSetup()
{

	if(!RELOAD_SETUP_FLAGS)	return true;

	if(RELOAD_SETUP_FLAGS&RELOAD_SETUP)
		LoadSetup();
	if(RELOAD_SETUP_FLAGS&RELOAD_SETUPEX)
		ReloadSetupEx();
	if(RELOAD_SETUP_FLAGS&RELOAD_SETUPMAPFILE)
		CSMClient::GetSM()->LoadMapFileSetup(m_pDBAllocator);
	if(RELOAD_SETUP_FLAGS&OUTPUT_SCRIPT_INFO)
		OutputScriptInfo();
	if(RELOAD_SETUP_FLAGS&OUTPUT_MEMPOOL_INFO)
	{
		MEMORYSTATUS msC;GlobalMemoryStatus(&msC);
		DWORD dwCurAvaiVirMem = msC.dwAvailVirtual/1048576;
		char szTmp[128] = {0};
		_snprintf(szTmp, 128, "CurAvaiVirMem = %d M\r\n", dwCurAvaiVirMem);
		OutMemInfo("log\\", szTmp);
		OUT_RUNNING_INFO(true);	
	}

	RELOAD_SETUP_FLAGS = 0x0;
	return true;
}

bool CGame::ReloadSetupEx()
{
	LoadSetup();

	s_pNetServer->SetParamEx(GetSetup()->lMaxBlockConNum,GetSetup()->lValidDelayRecDataTime);

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
		_snprintf(strTime, MAX_PATH, "\n%d:%d:%d\r\n", stTime.wHour, stTime.wMinute, stTime.wSecond);
		fwrite(strTime, strlen(strTime), 1, fp);
		fwrite(pTxt, strlen(pTxt), 1, fp);
		fclose(fp);
	}
}


// ��Ϸ��ʼ��
BOOL CGame::Init()
{
	SET_OUT_INFO_SETUP(OutMemInfo, "log\\");
	//����һ��log�ı���Ŀ¼
	CreateDirectory( "log",	NULL);

	srand((unsigned)time(NULL));
	random(100);

	PutDebugString("Server start!");
#ifdef _RUNSTACKINFO_
	InitStackFileName();
#endif	

#ifdef _GOODSLOG_
	InitGoodsLogFileName();
#endif

	if (LoadSetup()==false)
	{
		return false;
	}

	for(int i = 0; i < CGlobeSetup::GetSetup()->lPreAllocedScriptNum; i++)
	{
		CScript* script = MP_NEW CScript;
		g_FreeScriptList.push_back(script);
	}
	
	//�������ݿ�
	m_pDBAllocator = new CDataBlockAllocator(m_Setup.dwDataBlockNum,m_Setup.dwDataBlockSize,true);


	//��ʼ�������ĳ�������
	m_pDupliRegionSetup = new CDupliRegionSetup;

	CShape::InitMoveCheckCellList();
	CPlayer::InitUpdateDirtyAttrData();
	CMonster::InitUpdateDirtyAttrData();

	//��ʼ����Ϣ
	CMessage::Initial(m_pDBAllocator,m_Setup.dwFreeMsgNum);
	//��ʼ����Ϣ�����ȼ��Ͷ�����Ϣ
	InitMsgInfo();
	// ��ʼ������������
	CMySocket::MySocketInit();
	CMySocket::SetEncryptFlag(m_Setup.lEncryptType);
	
	CSMClient::GetSM()->LoadMapFileSetup(m_pDBAllocator);

	if(!InitNetServer() )
	{
		return false;
	}	

	// ������worldserver������
	if( !StartConnWS() )
	{		
		AddLogText("Can't connect to WServer, please run WServer first!");
		CreateConnectWorldThread();
	}

	GameManager::GetInstance()->Initialize();

	if(GetSetup()->lUseBillingServer==BILL_CONNECT_NONE)
	{
		AddLogText("����BillingServer����Ϊ������!");
	}	
	else if(GetSetup()->lUseBillingServer==BILL_CONNECT_XM)
	{
		if(!StartConnBS())
		{
			MessageBox(g_hWnd, "�������ӼƷѷ����������������Ʒѷ�����!", "Message", MB_OK);
			CreateConnectBSThread();
		}
	}
	else if(GetSetup()->lUseBillingServer==BILL_CONNECT_SNDA)
	{
		m_pSDOAOrderHandler=sdoaCreateOrderHandler();
		int ret=m_pSDOAOrderHandler->InitializeEx("sdoa4server.ini",MsgHandle);
		if(ret==0)
		{
			//ok
			m_bSDOAInitialized=true;
			AddLogText("SDOA��ʼ���ɹ���");
		}
		else
		{
			m_bSDOAInitialized=false;
			MessageBox(g_hWnd,"SDOAOrderHandler��ʼ��ʧ��","Message",MB_OK);
		}
	}

	// ���ӵ�LogServer
	if (0 == GetSetup()->lUseLogServer)
	{
		AddLogText("����LogServer����Ϊ�����ӣ���־���񽫲���ʹ��!");
	}
	else
	{
		if( !StartConnLogS() )
		{
			MessageBox(g_hWnd, "Can't connect to LogServer, please run LogServer first!", "Message", MB_OK);
			CreateConnectLogThread();		
		}
		else
			AddLogText("����LogServer�ɹ�!");
	}

	// ����BounsServer
	if( 0 == GetSetup()->lUseBounsServer )
	{
		AddLogText("����BounsServer����Ϊ������!");
	}
	else
	{
		if( !StartConnBounsServer())
		{
			MessageBox(g_hWnd, "Can't connect to BounsServer, please run BounsServer first!", "message", MB_OK);
			CreateConnectBounsServerThread();
		}
	}

	//! ��ʼ��������
	CSMClient::GetSM()->InitOk();
	ResetStarDay(time(NULL));

	return true;
}

//��ʼ��������Ϣ�����ȼ��Ͷ�����Ϣ
BOOL CGame::InitMsgInfo()
{
	//��������ص���Ϣע��Ϊ������ȼ�����Ϣ
	CBaseMessage::RegisterMsgPrio(MSG_S2C_OTHER_TALK,1);
	CBaseMessage::RegisterMsgPrio(MSG_S2C_OTHER_FACTIONTALK,1);
	CBaseMessage::RegisterMsgPrio(MSG_S2C_OTHER_BROADCAST,1);
	CBaseMessage::RegisterMsgPrio(MSG_S2C_OTHER_AddTopInfo,1);
	CBaseMessage::RegisterMsgPrio(MSG_S2C_OTHER_TALK_WORLD,1);
	CBaseMessage::RegisterMsgPrio(MSG_S2C_OTHER_TALK_COUNTRY,1);
	CBaseMessage::RegisterMsgPrio(MSG_S2C_OTHER_TALK_NOTIFY,1);

	//��ʼ����Ҫ��������Ϣ
	CBaseMessage::RegisterDiscardMsg(MSG_S2C_RGN_ADDSHAPE);
	CBaseMessage::RegisterDiscardMsg(MSG_S2C_SHAPE_MOVE);
	CBaseMessage::RegisterDiscardMsg(MSG_S2C_SHAPE_STOPMOVE);
	return true;
}


BOOL CGame::InitStackFileName()
{
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	_snprintf(m_pszRunStackFile,MAX_PATH,"RunStackInfo[%02d_%02d_%02d_%02d_%02d].txt",stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);	
	return TRUE;
}

BOOL CGame::InitGoodsLogFileName()
{
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);	
	_snprintf(m_pszGoodsLogFile,MAX_PATH,"GoodsLog[%02d_%02d_%02d_%02d_%02d].txt",stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	return TRUE;
}

// ��Ϸ�ͷ�
BOOL CGame::Release()
{
	PutDebugString("Server Exiting...");

	if(m_pSDOAOrderHandler)
	{
		m_pSDOAOrderHandler->UnInitialize();
		m_pSDOAOrderHandler->Release();
	}	

	// Fox@20081105 ��ҹ���, ��ֻ���������������
	GetInst(CPlayerMan).Release();

	//�˳�WS�����߳�
	bConnectThreadExit=true;
	WaitForSingleObject(hConnectThread,INFINITE);
	CloseHandle(hConnectThread);

	//�˳�DBS�����߳�
	bConnectThreadExitDBS=true;
	WaitForSingleObject(hConnectThreadDBS,INFINITE);
	CloseHandle(hConnectThreadDBS);

	bConnectThreadExitBS=true;
	WaitForSingleObject(hConnectThreadBS,INFINITE);
	CloseHandle(hConnectThreadBS);

	bConnectThreadExitBouns = true;
	WaitForSingleObject(hConnectThreadBouns, INFINITE);
	CloseHandle(hConnectThreadBouns);
	char str[256] = "";

	try
	{
		// �����֯
		GameOrganizingCtrl::Release();

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::Release() clear player begin");
#endif
		// ������
		for(itPlayer it=s_mapPlayer.begin(); it!=s_mapPlayer.end(); it++)
		{
			GameManager::GetInstance()->FreeObject((CBaseObject**)&(it->second));
		}
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::Release() clear player end");
#endif
	}
	catch(...)
	{
		PutDebugString("Clear Player data error!");
	}
	PutDebugString("Clear Player data ok!");

	s_mapPlayer.clear();
	
	MedalContainer::Release();
	CGoodsFactory::Release();
	CSuitRuleFactory::Release();
	CKitCardRuleFactory::Release();
	CMonsterList::Release();
	CDeputyOccuSystem::Relese();
	CSessionFactory::Release();
	

	if( s_pNetServer )
	{
		s_pNetServer->DisConn(m_lWSSocket);
	}

	if( s_pNetServer )
	{
		s_pNetServer->Exit();		
	}
	SAFE_DELETE(s_pNetServer);

	CMySocket::MySocketClearUp();

	GetNetSessionManager()->Release();
	GetPKSys()->Release();
	CQuestSystem::Relese();

	CleanSequenceMap();
	CleanValidateTime();

	// �����������
	ClearBounsDes();

	// ���������֤��Ϣ�Ϸ��Ե������ַ���
	CSequenceString::Uninitialize();

	CWordsFilter::Release();
	
	
	SAFE_DELETE(m_pDupliRegionSetup);

	CCountryHandler::GetInstance()->Release();
	CCountryParam::GetInstance()->Release();
	
	GetInst( AntiCheat::CPlayerManager ).Release();
	GetInst( FBusiness::CShopManager ).Release();
	CGlobalScriptTimer::Release();
	GameManager::GetInstance()->Release();
	GameManager::ReleaseInstance();

	CNewSkillFactory::ClearSkillCache();
	COccupSkillList::Release();
	CStudySkillCondition::Release();
	CStringReading::Release();
	CMailList::Release();
	CSMClient::GetSM()->ExitOk();
	CMySocket::MySocketClearUp();
	CReLiveGoodsList::Release();
	GetInst(CCreditSystem).Release();
	CCosmetology::Release();
	GetInst(CQuestIndexXml).Release();

	// �����ԭ������
	M_FREE(m_strFunctionList_FileData, m_lFuncFileDataLen);
	M_FREE(m_strVariableList_FileData, m_lVariableFileDataLen);
	for(map<string, tagScriptDataInfo*>::iterator it=m_mapScript_FileData.begin(); it!=m_mapScript_FileData.end(); it++)
	{
		MP_DELETE(it->second);
	}
	m_mapScript_FileData.clear();
	PutDebugString("Release m_mapScript_FileData ok!");

	CScript::ReleaseFunction();
	CScript::ReleaseGeneralVariable();	

	PutDebugString("Release CScript Data ok!");
	// ���ȫ�ֵ�FreeScriptList
	FreeScript::iterator scriptItr = g_FreeScriptList.begin();
	while(scriptItr != g_FreeScriptList.end())
	{
		MP_DELETE(*scriptItr);
		scriptItr++;
	}
	g_FreeScriptList.clear();
	PutDebugString("Clear FREE scripts data ok!");
	// ���ȫ�ֵ�����ScriptList
	itScript rgnScriptItr = g_Scripts.begin();
	while(rgnScriptItr != g_Scripts.end())
	{
		MP_DELETE(rgnScriptItr->second);
		rgnScriptItr++;
	}
	g_Scripts.clear();
	// ����ݹ���÷���ֵ�����б�
	CScript::ReleaseStackVarListInst();
	PutDebugString("Clear Running scripts data ok!");

	//! ����־ϵͳ
	SAFE_DELETE(m_pLogicLogInterface);
	
	m_LogClient.Release();

	// �رմ����߳̾��
	WaitForSingleObject(g_hSavingThread, INFINITE);
	CloseHandle(g_hSavingThread);
	CSMClient::GetSM()->ReleaseSM();

	CMessage::Release();
	SAFE_DELETE(m_pDBAllocator);
	PutDebugString("Server Exited!");
	return true;
}

//Ϊ������ҷ���������Ϣ
void CGame::SendSysMailMsgToOnlinePlayer(CMail *pMail)
{
	itPlayer it = s_mapPlayer.begin();
	for (;it!=s_mapPlayer.end();++it)
	{
		CPlayer *pPlayer = (*it).second;
		GameManager::GetInstance()->GetMailManager()->SendSysMailMsg(pMail,pPlayer);
	}
}

BOOL CGame::ReConnectDBServer()
{
	return FALSE;
}
void CGame::AddReconnectionMsg(DBWriteSet& setWriteDB)
{
	// ����ȫ�ֱ���ֵ��WS
	if(CScript::GetGeneralVariableList())
	{
		setWriteDB.AddToByteArray((BYTE)1);
		CScript::GetGeneralVariableList()->CodeToDataBlock(setWriteDB);
	}
	else
		setWriteDB.AddToByteArray((BYTE)0);

	// ����ÿ��������� MSG_S2DB_SERVER_QUEST_RECONNECTION : 1
	setWriteDB.AddToByteArray((long)s_mapPlayer.size());
	for(itPlayer it=s_mapPlayer.begin(); it!=s_mapPlayer.end(); it++)
	{
		CPlayer* pPlayer=it->second;
		if(pPlayer)
		{
			setWriteDB.AddToByteArray((BYTE)1);
			setWriteDB.AddToByteArray(pPlayer->GetExID());
			pPlayer->CodeToDataBlock(setWriteDB);
		}
		else
		{
			setWriteDB.AddToByteArray((BYTE)0);
		}
	}

	setWriteDB.AddToByteArray((long)GetSavePlayerMap().size());
	map<CGUID, CPlayer*>::iterator saveitr = GetSavePlayerMap().begin();
	while( saveitr != GetSavePlayerMap().end() )
	{
		if(saveitr->second)
		{
			setWriteDB.AddToByteArray((BYTE)1);
			setWriteDB.AddToByteArray( saveitr->second->GetExID() );
			saveitr->second->CodeToDataBlock(setWriteDB);
			// �������
			GameManager::GetInstance()->FreeObject((CBaseObject**)&(saveitr->second));
			saveitr++;
		}
		else
		{
			setWriteDB.AddToByteArray((BYTE)0);
		}
	}
	GetSavePlayerMap().clear();
	
	// ���͸������������� MSG_S2DB_SERVER_QUEST_RECONNECTION : 3
	// rgn num
	long rgnNum = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().size()
		+ GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().size()
		+ GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().size();

	setWriteDB.AddToByteArray((long)rgnNum);

	vector<BYTE> pBA;
	// p rgn num
	CRgnManager::RgnByIDMapItr rgnItr = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().begin();
	for(; rgnItr != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end(); rgnItr++)
	{
		pBA.clear();
		// rgn data
		rgnItr->second->AddToByteArray(&pBA);
		setWriteDB.AddToByteArray((long)pBA.size()); //rgn data buf size
		setWriteDB.AddToByteArray(&pBA[0], (long)pBA.size());

		pBA.clear();
		rgnItr->second->GetVariableList()->AddToByteArray(&pBA);
		setWriteDB.AddToByteArray((long)pBA.size());//rgn varlist data size
		// varlist data
		setWriteDB.AddToByteArray(&pBA[0], (long)pBA.size());
	}

	// ph rgn num
	rgnItr = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().begin();
	for(; rgnItr != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end(); rgnItr++)
	{
		pBA.clear();
		// rgn data
		rgnItr->second->AddToByteArray(&pBA);
		setWriteDB.AddToByteArray((long)pBA.size()); //rgn data buf size
		setWriteDB.AddToByteArray(&pBA[0], (long)pBA.size());

		pBA.clear();
		rgnItr->second->GetVariableList()->AddToByteArray(&pBA);
		setWriteDB.AddToByteArray((long)pBA.size());//rgn varlist data size
		// varlist data
		setWriteDB.AddToByteArray(&pBA[0], (long)pBA.size());
	}

	// t rgn num
	rgnItr = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().begin();
	for(; rgnItr != GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end(); rgnItr++)
	{
		pBA.clear();
		// rgn data
		rgnItr->second->AddToByteArray(&pBA);
		setWriteDB.AddToByteArray((long)pBA.size()); //rgn data buf size
		setWriteDB.AddToByteArray(&pBA[0], (long)pBA.size());

		pBA.clear();
		rgnItr->second->GetVariableList()->AddToByteArray(&pBA);
		setWriteDB.AddToByteArray((long)pBA.size());//rgn varlist data size
		// varlist data
		setWriteDB.AddToByteArray(&pBA[0], (long)pBA.size());
	}
	
	// ���͸��˷��ݸ���������Ʒ���� MSG_S2DB_SERVER_QUEST_RECONNECTION : 4
	CRgnManager::PHGoodsMap& goodsMap = GameManager::GetInstance()->GetRgnManager()->GetPHGoodsMap();
	CRgnManager::PHGoodsMapItr goodsItr = goodsMap.begin();
	vector<BYTE> PHGoodsBufArray;

	for(; goodsItr != goodsMap.end(); goodsItr++)
	{
		// DupRgnID
		setWriteDB.AddToByteArray(goodsItr->first);

		// DupRgn goods num
		setWriteDB.AddToByteArray((long)goodsItr->second.size());

		// DupRgn Goods data
		CRgnManager::vecPHGoods::iterator vecItr = goodsItr->second.begin();
		for(; vecItr != goodsItr->second.end(); vecItr++)
			setWriteDB.AddToByteArray(&(*vecItr), sizeof(CRgnManager::PHGoods));
	}
	setWriteDB.AddToByteArray((long)GameManager::GetInstance()->GetRgnManager()->GetPHGoodsMap().size()); // ���˷��ݸ���
	setWriteDB.AddToByteArray((long)PHGoodsBufArray.size());
	if(PHGoodsBufArray.size())
		setWriteDB.AddToByteArray(&PHGoodsBufArray[0], (long)PHGoodsBufArray.size());
}
BOOL CGame::ReConnectWorldServer()
{
	m_lWSSocket = s_pNetServer->Connect(GetSetup()->strWorldIP.c_str(), GetSetup()->dwWorldPort,NF_WServer_Client);
	if(m_lWSSocket>0)
	{
		CMessage::SetWSSocket(m_lWSSocket);
		AddLogText("��������WorldServer�ɹ� !");

		SetServerInitSendFlag(World_Server_InitSendFlag);
		return TRUE;
	}
	return FALSE;
}

BOOL CGame::ReConnectBSServer()
{
	if(m_listBSSetup.size()<1)
		return FALSE;
	if(!s_pNetServer)
		return FALSE;
	list<tagBSSetup>::iterator it=m_listBSSetup.begin();
	tagBSSetup stSetup=*it;
	m_lBSSocket=s_pNetServer->Connect(stSetup.strIp.c_str(),stSetup.dwPort,NF_BillingServer_Client);

	if(m_lBSSocket>0)
	{
		m_bBSConnected=TRUE;
		CMessage::SetBSSocket(m_lBSSocket);
		AddLogText("��������BillingServer�ɹ� !");
		CMessage msg(MSG_S2B_SENDINFO);
		msg.Add((char)1); //reconnect
		msg.Add(GetSetup()->dwListenPort);
		msg.Add((char*)GetSetup()->strLocalIP.c_str());
		msg.SendToSocket(m_lBSSocket);
		return TRUE;
	}
	return FALSE;	
}

bool CGame::ReConnectBounsServer()
{
	return StartConnBounsServer(true);
}


//���������߳�
VOID   CGame::CreateConnectWorldThread()
{
	bConnectThreadExit=true;
	WaitForSingleObject(hConnectThread,INFINITE);
	CloseHandle(hConnectThread);

	bConnectThreadExit=false;

	unsigned threadID;
	hConnectThread = (HANDLE)_beginthreadex(NULL,0,ConnectWorldFunc,NULL,0,&threadID);
}

VOID  CGame::CreateConnectBSThread()
{
	bConnectThreadExitBS=true;
	WaitForSingleObject(hConnectThreadBS,INFINITE);
	CloseHandle(hConnectThreadBS);
	bConnectThreadExitBS=false;
	unsigned threadID;
	hConnectThreadBS = (HANDLE)_beginthreadex(NULL,0,ConnectBSFunc,NULL,0,&threadID);	
}

//���������߳�
void   CGame::CreateConnectDBThread()
{
	bConnectThreadExitDBS=true;
	WaitForSingleObject(hConnectThreadDBS,INFINITE);
	CloseHandle(hConnectThreadDBS);

	bConnectThreadExitDBS=false;


	unsigned threadID;
	hConnectThreadDBS = (HANDLE)_beginthreadex(NULL,0,ConnectDBFunc,NULL,0,&threadID);
}

// ��������BounsServer���߳�
void CGame::CreateConnectBounsServerThread()
{
	bConnectThreadExitBouns = true;
	WaitForSingleObject(hConnectThreadBouns, INFINITE);
	CloseHandle(hConnectThreadBouns);

	bConnectThreadExitBouns = false;
	unsigned threadID;
	hConnectThreadBouns = (HANDLE)_beginthreadex(NULL, 0, ConnectBounsServerFunc, NULL, 0, &threadID);
}

//��ʼ����Ϸ����������ˣ���������������ˣ�װ������
BOOL CGame::InitNetServer(void)
{
	s_pNetServer = new CServer;
	//��Ϊ�������Ϳͻ���˫���������
	long lRet = s_pNetServer->Start(1|2,m_pDBAllocator,
		m_Setup.dwFreeSockOperNum,m_Setup.dwFreeIOOperNum,m_Setup.lIOOperDataBufNum,
		m_Setup.bCheckNet,m_Setup.dwBanIPTime,
		m_Setup.dwMaxMsgLen,m_Setup.lMaxConnectNum,m_Setup.lMaxClientsNum,
		m_Setup.lPendingWrBufNum,m_Setup.lPendingRdBufNum,
		m_Setup.lMaxSendSizePerSecond,m_Setup.lMaxRecvSizePerSecond,
		m_Setup.lMaxBlockedSendMsgNum,
		m_Setup.lConPendingWrBufNum,m_Setup.lConPendingRdBufNum,
		m_Setup.lConMaxSendSizePerSecond,m_Setup.lConMaxRecvSizePerSecond,
		m_Setup.lConMaxBlockedSendMsgNum);
	if( !lRet )
	{
		MessageBox(g_hWnd, "Start NetServer Error", "Message", MB_OK);
		return FALSE;
	}
	CMessage::SetServer(s_pNetServer);
	AddLogText("GameServer�������������ʼ���ɹ� !");
	return TRUE;
}

BOOL CGame::StartAccept()
{
	if(s_pNetServer == NULL)	return FALSE;

	BOOL bRet = s_pNetServer->Host(m_Setup.dwListenPort,NULL,NF_Server_Client,SOCK_STREAM);
	if(!bRet)
	{
		MessageBox(g_hWnd, "Start Host Error", "Message", MB_OK);
		return FALSE;
	}
	AddLogText("GameServer��Ϊ��������ʼ���ɹ� !");
	return TRUE;
}

//������WordServer�����ӣ���Ϊ�ͻ��˳�ʼ��
BOOL CGame::StartConnWS(void)
{
	m_lWSSocket = s_pNetServer->Connect(GetSetup()->strWorldIP.c_str(), GetSetup()->dwWorldPort,NF_WServer_Client);
	if(m_lWSSocket>0)
	{
		CMessage::SetWSSocket(m_lWSSocket);
		AddLogText("����WorldServer �ɹ� !");

		CMessage msg(MSG_S2W_SERVER_SENDSERVERINFO);
		msg.Add((char)0);	// first connect
		msg.Add(GetSetup()->dwListenPort);
		msg.Add((char*)GetSetup()->strLocalIP.c_str());
		msg.SendToSocket(m_lWSSocket);

		b_IsFirstConnectionWS = false;
		return TRUE;
	}
	return FALSE;
}


BOOL CGame::StartConnBS()
{
	if(m_listBSSetup.size()<1)
		return FALSE;
	list<tagBSSetup>::iterator it=m_listBSSetup.begin();
	tagBSSetup stSetup=*it;
	m_lBSSocket=s_pNetServer->Connect(stSetup.strIp.c_str(),stSetup.dwPort,NF_BillingServer_Client);
	if(m_lBSSocket>0)
	{
		m_bBSConnected=TRUE;
		CMessage::SetBSSocket(m_lBSSocket);
		AddLogText("����BillingServer�ɹ� !");
		CMessage msg(MSG_S2B_SENDINFO);
		msg.Add((char)0); //first connect
		msg.Add(GetSetup()->dwListenPort);
		msg.Add((char*)GetSetup()->strLocalIP.c_str());
		msg.SendToSocket(m_lBSSocket);
		return TRUE;
	}
	return FALSE;
}

bool CGame::StartConnBounsServer(bool bReconn)
{
	if(m_listBDSSetup.size() < 1)
	{
		return false;
	}

	list<tagBSSetup>::iterator itor = m_listBDSSetup.begin();
	for(; itor != m_listBDSSetup.end(); ++itor)
	{
		tagBSSetup tgSetup = *itor;
		m_lBounsSocket = s_pNetServer->Connect(tgSetup.strIp.c_str(), tgSetup.dwPort, NF_BounsServer_Client);
		if(m_lBounsSocket > 0)
		{
			m_bBounsSConnected = true;
			CMessage::SetBounsSocket(m_lBounsSocket);
			if(bReconn)
			{
				AddLogText("��������BounsServer�ɹ�!");
			}
			else
			{
				AddLogText("����BounsServer�ɹ�!");
			}
			CMessage msg(MSG_S2BDS_SENDSERVERINFO);
			msg.Add(GetSetup()->strLocalIP.c_str());
			msg.Add(GetSetup()->dwGameServerNum);
			msg.SendToSocket(m_lBounsSocket);
			return true;
		}
	}

	return false;
}

//������DBServer�����ӣ���Ϊ�ͻ��˳�ʼ��
BOOL CGame::StartConnDBS(void)
{
	return FALSE;
}
void CGame::OnPlayerLost(const CGUID& PlayerGuid,long lSocketID)
{
	GameManager::GetInstance()->RemoveLoginSessionBySocketID(lSocketID);
	OnPlayerExit(PlayerGuid);

	RemovePlayer(PlayerGuid);
}


void CGame::OnPlayerExit(const CGUID& PlayerGuid, BOOL isChangeServer)
{
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[1024]="";
	char pszGUID[50]="";
	PlayerGuid.tostring(pszGUID);
	_snprintf(pszStatckInfo,1024,"CGame::OnPlayerExit(PlayerID:%s) Start",pszGUID);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	if(NULL_GUID == PlayerGuid)	return;

	m_TransCongPlayers.erase(PlayerGuid);

	CPlayer* pPlayer = FindPlayer(PlayerGuid);

	if(!pPlayer)
	{
		return;
	}
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::OnPlayerExit() DelPlayer Start");
#endif
	
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::OnPlayerExit() OnExitRegion Start");
#endif
	//�������¼�
	pPlayer->OnExitRegion();		
	//�ӳ�����ɾ�������
	CServerRegion* pServerRegion = dynamic_cast<CServerRegion*>( pPlayer -> GetFather() );
	if(pServerRegion)
	{
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::OnPlayerExit() pServerRegion->PlayerExit Start");
#endif
		pServerRegion->PlayerExit(pPlayer);
	}

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::OnPlayerExit() pPlayer->OnExit Start");
#endif
	pPlayer->OnExit(isChangeServer);

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::OnPlayerExit() End");
#endif
}

void CGame::UpdateSaveRegion(CServerRegion* pRegion)
{
	if(pRegion)
	{
		set<CServerRegion*>::iterator itr = m_setSaveRegion.find(pRegion);
		if(itr == m_setSaveRegion.end())
		{
			m_setSaveRegion.insert(pRegion);
		}		
	}
}

//���紫��ӳ�ͨ��ӵ��ʱ����
void CGame::OnTransferCongestion(const CGUID& PlayerGuid,long lSocketID)
{
	CPlayer* pPlayer = FindPlayer(PlayerGuid);
	if(!pPlayer)	return;
	m_TransCongPlayers.insert(PlayerGuid);
	pPlayer->SetNetTransferStatus(true);
	return;
}
//���紫���ӵ������ͨʱ����
void CGame::OnTransferSmoothly(const CGUID& PlayerGuid,long lSocketID)
{
	CPlayer* pPlayer = FindPlayer(PlayerGuid);
	if(!pPlayer)	return;
	m_TransCongPlayers.erase(PlayerGuid);
	pPlayer->SetNetTransferStatus(false);
	return;
}

void CGame::RemoveTransferCongPlayerID(hashsetGUID& PlayersGUID)
{
	if(m_TransCongPlayers.size() == 0)	return;
	hsGUIDIt it = PlayersGUID.begin();
	for(;it != PlayersGUID.end();)
	{
		if(m_TransCongPlayers.find((*it)) != m_TransCongPlayers.end())
			it =PlayersGUID.erase(it);
		else
			it++;
	}

}
bool CGame::IsTransferCongPlayerID(const CGUID& PlayerGUID)
{
	return !(m_TransCongPlayers.find(PlayerGUID) == m_TransCongPlayers.end());
}

void CGame::ClearSaveRegion(CServerRegion* pRegion)
{
	std::set<CServerRegion*>::iterator it=m_setSaveRegion.find(pRegion);
	if(it!=m_setSaveRegion.end())
		m_setSaveRegion.erase(it);
}

void CGame::ClearSaveRegion()
{
	m_setSaveRegion.clear();
}

void CGame::OnWSLost()
{
	AddLogText("============WorldServer closed==========!!!");
	m_lWSSocket = 0;
	CMessage::SetWSSocket(0);
}
void CGame::OnDBSLost()
{
	m_lWSSocket = 0;
	CMessage::SetDBSSocket(0);
}
void CGame::SetServerInitSendFlag(eServerInitSendMsgFlag flag) 
{
	EnterCriticalSection(&m_CSServerInitSendFlag);	
	m_ServerInitSendFlag = m_ServerInitSendFlag | flag;
	LeaveCriticalSection(&m_CSServerInitSendFlag);
}
void CGame::ProcessServerInitSendMsg()
{
	if(m_ServerInitSendFlag == Null_Server_InitSendFlag) return;

	EnterCriticalSection(&m_CSServerInitSendFlag);
	if(m_ServerInitSendFlag & Log_Server_InitSendFlag)
	{
		
	}
	if(m_ServerInitSendFlag & Bill_Server_InitSendFlag)
	{

	}
	if(m_ServerInitSendFlag & World_Server_InitSendFlag)
	{
		//! ���������ϵ�����ݵ�WS
		LinkmanSystem::GetInstance().SendDataToWs();

		// ����������ݺ͸������ݵ�WS
		CMessage msg1(MSG_S2W_SERVER_SENDSERVERINFO);
		if(false == b_IsFirstConnectionWS) // ���ǳ�������
			msg1.Add((char)1);	// reconnect?
		else
			msg1.Add((char)0);

		msg1.Add(GetSetup()->dwListenPort);
		msg1.Add((char*)GetSetup()->strLocalIP.c_str());
		DBWriteSet writeSet;
		msg1.GetDBWriteSet(writeSet);
		AddReconnectionMsg(writeSet);
		msg1.SendToSocket(m_lWSSocket);
	}
	m_ServerInitSendFlag = Null_Server_InitSendFlag;
	LeaveCriticalSection(&m_CSServerInitSendFlag);
}
// ��Ϣ����
BOOL CGame::ProcessMessage()
{
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ProcessMessage() Start");
#endif
	static msgQueue TemptMsgQueue;
	//����ͻ���������Ϣ
	if(s_pNetServer)
	{
		s_pNetServer->GetRecvMessages()->GetAllMessage(TemptMsgQueue);
		msgQueue::iterator it = TemptMsgQueue.begin();
		for(;it != TemptMsgQueue.end();it++)
		{
			CMessage* pMsg = (CMessage*)(*it);
			if( pMsg )
			{
				long lType = pMsg->GetType();
				DWORD dwStartTime = timeGetTime();
				pMsg->Run();
				AddMsgExpendTime(lType,timeGetTime()-dwStartTime);
				CBaseMessage::FreeMsg(pMsg);
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
#ifdef _ASCYN_DEBUG
	tagSDOA_ITEM_SEND_REQUEST stSdo_Item_Send_Request;
	//orderId
	stSdo_Item_Send_Request.szOrderId="orderId";


	//payStatus
	stSdo_Item_Send_Request.szPayStatus="0";
	if(!stSdo_Item_Send_Request.szPayStatus)
		return 0;
	//int nErrorCode=atoi(stSdo_Item_Send_Request.szPayStatus);			

	//sendTimes
	stSdo_Item_Send_Request.szSendTimes="sendTimes";


	//callTime
	stSdo_Item_Send_Request.szCallTime="callTime";


	//userId
	stSdo_Item_Send_Request.szUserId="userId";


	//roleName
	stSdo_Item_Send_Request.szRoleName="roleName";


	//goodsNum
	stSdo_Item_Send_Request.szGoodsNum="goodsNum";


	//goodsInfoEx
	stSdo_Item_Send_Request.szGoodsInfoEx="goodsInfoEx";


	//indication
	stSdo_Item_Send_Request.szIndication="indication";		

	//payErrorMsg
	stSdo_Item_Send_Request.payErrorMsg="payErrorMsg";
	stSdo_Item_Send_Request.nMsgType = 300;
	stSdo_Item_Send_Request.nMsgFrom = 200;
	stSdo_Item_Send_Request.nMsgSeq = 100;
	GetGame()->OnReceiveSdoItemRequest(stSdo_Item_Send_Request);
#endif
	//////////////////////////////////////////////////////////////////////////

	//����SDOA_ITEM_SEND_REQUEST��Ϣ
	static msgQueue TemptSDOMsgQueue;
	m_RecvSDOMessages.GetAllMessage(TemptSDOMsgQueue);


	if (TemptSDOMsgQueue.size()>0)
	{
#ifdef _ASCYN_DEBUG
	char szOutPut[512];
	_snprintf(szOutPut,512,"CGAME====BOOL CGame::ProcessMessage() TemptSDOMsgQueue��С:%d",TemptSDOMsgQueue.size());
	PutStringToFile("SDO_ITEM_SEND_RESPONSE",szOutPut);
#endif
		msgQueue::iterator it = TemptSDOMsgQueue.begin();
		for(;it != TemptSDOMsgQueue.end();it++)
		{
			CMessage *pMsg = (CMessage*)(*it);
			if (pMsg)
			{
#ifdef _ASCYN_DEBUG
				char szOutPut[512];
				_snprintf(szOutPut,512,"CGAME====BOOL CGame::ProcessMessage()pMsg->Run();");
				PutStringToFile("SDO_ITEM_SEND_RESPONSE",szOutPut);
#endif
				pMsg->Run();
				
				CBaseMessage::FreeMsg(pMsg);
			}
		}
	}
	CSMClient::GetSM()->ProcessMessage();
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ProcessMessage() End");
#endif
	return TRUE;
}


// ��Ϸ��ѭ��
BOOL CGame::MainLoop()
{
	// �����߳�
	if (g_bGameThreadExit)
	{
		return false;
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyClearFile(GetGame()->GetStatckFileName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MainLoop() Start.");
#endif

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ProcessServerInitSendMsg Start.");
#endif
	ProcessServerInitSendMsg();
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ProcessServerInitSendMsg End.");
#endif
	static bool bStart = true;
	if (bStart)
	{
		CSMClient::GetSM()->SendAITick(true);
		bStart = false;
	}
	// ͳ��ÿ������ĺ�ʱ
	static DWORD dwScriptTime = 0, dwAITime = 0, dwMessageTime = 0, dwSessionTime = 0, 
		dwNetSessionTime = 0,dwRgnManagerTime = 0,dwGameManagerTime = 0, dwBillClientTime = 0;
	static DWORD dwCurTime = 0;

	static DWORD ai_tick, now = timeGetTime(), lClearChangeRgnPlayer = now, old = now, lastRefeashInfoTime = now, lastSaveTime = now, lastDelayOperPlayerTime = now,lastGoodsLogClearTime=now,lCheckBaseModuleTime=now
		;
	now = timeGetTime();
	ai_tick++;
	s_lAITick++;
#ifdef _GOODSLOG_
	if(now-lastGoodsLogClearTime>GetSetup()->dwGoodsLogTime*60*1000)
	{
		CMessage::AsyClearFile(GetGame()->GetGoodsLogFile());
		lastGoodsLogClearTime=now;
	}	
#endif

	//�ض�����
	ReLoadSetup();

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GetRegionTimer()->Expire() Start.");
#endif
	// ������ʱ��
	dwCurTime = timeGetTime();
	GameManager::GetInstance()->GetRgnManager()->GetRegionTimer()->Expire(now);
	dwRgnManagerTime += timeGetTime() - dwCurTime;

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GetRegionTimer()->Expire() End.");
#endif


#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GetQueueTimer()->Expire() Start.");
#endif
	// ������ض�ʱ��
	GameManager::GetInstance()->GetRgnManager()->GetQueueTimer()->Expire(now);
	GameManager::GetInstance()->GetRgnManager()->GetCreatePRgnTimer()->Expire(now);
	GameManager::GetInstance()->GetRgnManager()->GetCreateTRgnTimer()->Expire(now);
	GameManager::GetInstance()->GetRgnManager()->GetRgnKickPlayerTimer()->Expire(now);
	GSAutoSupply::GetTimerQueue().Expire(now);
    GSAutoFight::GetTimerQueue().Expire(now);
    GSAutoFight::GetCommonTimerQueue().Expire(now);
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"GetQueueTimer()->Expire End.");
#endif

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"RefeashInfoText() Start.");
#endif
	// ˢ����ʾ
	if (now - lastRefeashInfoTime > GetSetup()->dwRefeashInfoTime)
	{
		lastRefeashInfoTime = now;
		extern void RefeashInfoText();
		RefeashInfoText();
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"RefeashInfoText() End");
#endif


#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CheckBaseModuleTimeOut() Start.");
#endif	
	if ( (CGlobeSetup::GetSetup()->lBaseModuleTime != 0) 
		&& ((now - lCheckBaseModuleTime) > CGlobeSetup::GetSetup()->lBaseModuleTime) )
	{
		lCheckBaseModuleTime = now;
		CNewSkillFactory::CheckBaseModuleTimeOut(now);
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CheckBaseModuleTimeOut() End.");
#endif




#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Send Save Data Start.");
#endif
	//������̵�
	dwCurTime = timeGetTime();
	if ( (CGlobeSetup::GetSetup()->lSaveTime != 0) 
		&& ((now - lastSaveTime) > CGlobeSetup::GetSetup()->lSaveTime) )
	{
		lastSaveTime=now;
		
		// ���ͱ�����Ϣ
		SendQueuePlayerOperMsg();
		if( GetInst( Business::CBusinessManager ).IsEnable() )
		{
			GetInst( Business::CBusinessManager ).SendSaveMsg();
		}
		m_setSaveRegion.clear();
		

		//��ʱ������������Ϣ
		char pszMemInfo[1024]="";
		_snprintf(pszMemInfo,1024,"UsingScriptNum:%d",g_UsingScript.size());
		PutStringToFile("MemAllocInfo",pszMemInfo);

		
		GameManager::GetInstance()->PutObjNumInfo("MemAllocInfo");
		CGoodsFactory::OutputUseGoodsInfo("MemAllocInfo");
		CNewSkillFactory::OutputBasemodelInfo("MemAllocInfo");

		OutPlayerAndRegionInfo("MemAllocInfo");

		//! �������ģ����Ϣ
		//s_pNetServer->OutputMsgStatInfo(0);

		//! ��¼��ͼ��־
		LogRegion();

		//! ����ڴ����־
#ifdef LOG_ALLOC_FREE_INFO
		MEMORYSTATUS msC;GlobalMemoryStatus(&msC);
		DWORD dwCurAvaiVirMem = msC.dwAvailVirtual/1048576;
		_snprintf(pszMemInfo, 1024, "CurAvaiVirMem = %d M\r\n", dwCurAvaiVirMem);
		OutMemInfo("log\\", pszMemInfo);
		OUT_RUNNING_INFO(false);
#endif
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Send Save Data End.");
#endif

	lastSaveTime += timeGetTime() - dwCurTime;
	if( now - old > GetSetup()->dwWatchRunTime )
	{
		if (GetSetup()->bWatchRunTimeInfo)
		{
			char pszTimeInfo[1024]="";
			_snprintf(pszTimeInfo,1024,"%d������� %d ��AI\r\nScript:%d\r\nAI:%d\r\nMessage:%d\r\nSession:%d\r\nNetSession:%d\r\nRgnManger:%d\r\nGameManger:%d\r\nBillClient:%d", 
				GetSetup()->dwWatchRunTime/1000, ai_tick, dwScriptTime, dwAITime, dwMessageTime, dwSessionTime, 
				dwNetSessionTime,dwRgnManagerTime, dwGameManagerTime, dwBillClientTime);

			AddLogText(pszTimeInfo);
			PutStringToFile("TimeExpendInfo",pszTimeInfo);
			OutputMsgExpendTimeInfo("TimeExpendInfo");
#ifdef _SCRIPT_INFO_
			string strScriptInfo;
			GetScriptInfo(strScriptInfo);
			PutStringToFile("TimeExpendInfo", strScriptInfo.c_str());
			ClearScriptInfo();
#endif

			dwScriptTime = 0;
			dwAITime = 0;
			dwMessageTime = 0;
			dwSessionTime = 0;
			dwNetSessionTime = 0;
			dwRgnManagerTime = 0;
			dwGameManagerTime = 0;
			dwBillClientTime = 0;
		}
		else
			AddLogText("%d������� %d ��AI", GetSetup()->dwWatchRunTime/1000, ai_tick);

		ai_tick = 0;
		old = now;
	}

	AddAITick();

	// ���ܼ��
	if (GetSetup()->bWatchRunTimeInfo)
	{
		// ��Ϸ��AI
		dwCurTime = timeGetTime();
		dwAITime += timeGetTime() - dwCurTime;

		// ��Ϣ����
		dwCurTime = timeGetTime();
		ProcessMessage();
		dwMessageTime += timeGetTime() - dwCurTime;

		//##���лỰ�Ĵ���
		dwCurTime = timeGetTime();
		CSessionFactory::AI();
		dwSessionTime += timeGetTime() - dwCurTime;

		// ����Ự
		dwCurTime = timeGetTime();
		GetNetSessionManager()->Run();
		dwNetSessionTime += timeGetTime() - dwCurTime;

		dwCurTime = timeGetTime();
		GameManager::GetInstance()->Run();
		dwGameManagerTime += timeGetTime() - dwCurTime;
	}
	else
	{
		// ��Ϣ����
		ProcessMessage();

		//##���лỰ�Ĵ���
		CSessionFactory::AI();

		// ����Ự
		GetNetSessionManager()->Run();

		GameManager::GetInstance()->Run();
	}

	//! [2007-8-6]���¶������Ա������
	static long lOldTimeForTeam = 0;
	if(timeGetTime() - lOldTimeForTeam > 10000)
	{
		lOldTimeForTeam = timeGetTime();
		GetOrganiCtrl()->UnpdateTeamMemberPos();
	}

	GetInst(CPetCtrl).OnDeletePet();

	// ��ʱ����л��������������
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClearChangeRgnPlayerVec() Start.");
#endif
	ClearChangeRgnPlayerVec();
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClearChangeRgnPlayerVec() End.");
#endif
	
	// ��ʱ�����������
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClearDupRgn() Start.");
#endif
	ClearDelDupRgn();
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClearDupRgn() End.");
#endif

	CNewSkillFactory::RealeaseModuParam();

	CGoodsFactory::ReleaseGCGoodsSet();
	//ɾ����ɾ���Ľű�
	CScript::DelScript();
	// ��������
	static DWORD new_timer = timeGetTime();
	static DWORD old_timer = new_timer;
	new_timer = timeGetTime();
	if (new_timer - old_timer < (DWORD)g_ms)
	{
		WaitForSingleObject(GetCurrentProcess(), g_ms - (new_timer - old_timer));
	}

	old_timer += g_ms; 
	if ((long)new_timer - (long)old_timer > 1000)
	{
		OutputDebugString("warning!!! 1 second not call AI()\n");
		old_timer = timeGetTime();
	}


#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MainLoop() End.");
#endif
	return true;
}



void CGame::OutputMsgExpendTimeInfo(const char* pszFileName)
{
	PutStringToFile(pszFileName,"MsgExpendTime:");
	char pszInfo[256]="";
	map<long,long>::iterator it = m_mapMsgExpendTime.begin();
	for(;it != m_mapMsgExpendTime.end();it++)
	{
		if((*it).second > 0)
		{
			_snprintf(pszInfo,256,"MsgType:%d,ExpendTime;%d",(*it).first,(*it).second);
			PutStringToFile(pszFileName,pszInfo);
		}
	}
	m_mapMsgExpendTime.clear();

	PutStringToFile(pszFileName,"TimerExpendTime:");
	it = m_mapTimerExpendTime.begin();
	for(;it != m_mapTimerExpendTime.end();it++)
	{
		if((*it).second > 0)
		{
			long lType = (*it).first;
			_snprintf(pszInfo,256,"TimerType:%d,TimerlVar1:%d,ExpendTime;%d",(lType&0xFF),(lType>>16) ,(*it).second);
			PutStringToFile(pszFileName,pszInfo);
		}
	}
	m_mapTimerExpendTime.clear();
}

const char* CGame::GetCountryName(BYTE cCountryType)
{
	switch(cCountryType)
	{
	case 1:
		return "��֮�������";
	case 2:
		return "��֮����������";
	case 3:
		return "��֮����ŵ��";
	case 4:
		return "��֮���޵���˹";
	case 5:
		return "��֮��Ħ���¶�";
	case 6:
		return "��֮�����ط�";
	}
	return "����Ĺ���ID";
}

CPlayer* CGame::FindPlayer(const char* strName)
{
	if(!strName)	
		return NULL;

	itSG it = m_PlayerNameToGUID.find(strName);
	if( it != m_PlayerNameToGUID.end() )
	{
		itPlayer itP = s_mapPlayer.find((*it).second);
		if( itP != s_mapPlayer.end() )
			return (*itP).second;
	}
	return NULL;
}

void CGame::AddPlayer(CPlayer* pPlayer)
{
	if(!pPlayer)	return;

	map<CGUID,CPlayer*,guid_compare>::iterator it = s_mapPlayer.find(pPlayer->GetExID());
	if(it != s_mapPlayer.end())
		AddLogText("������%sʱ���󣬸���ҵ�ʵ���Ѿ�����!", pPlayer->GetName());
	s_mapPlayer[pPlayer->GetExID()]=pPlayer;

	m_PlayerNameToGUID[pPlayer->GetName()] = pPlayer->GetExID();

}
void CGame::RemovePlayer(const CGUID& PlayerID)
{
	GetInst(AntiCheat::CPlayerManager).PlayerExit(PlayerID );
	map<CGUID,CPlayer*,guid_compare>::iterator it = s_mapPlayer.find(PlayerID);
	if(it != s_mapPlayer.end())
	{
		CPlayer* pPlayer = (*it).second;
		if(pPlayer)
		{
			// Fox@20081110 ɾ�����
			GetInst(CPlayerMan).DelPlayer(pPlayer);
			m_PlayerNameToGUID.erase((*it).second->GetName());

			if(GetGame()->GetWSSocket() > 0)
			{
#ifdef _RUNSTACKINFO1_
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::OnPlayerExit() FreeObject Start");
#endif
				GameManager::GetInstance()->FreeObject((CBaseObject**)&pPlayer);
			}
			else
			{
				pPlayer->SetSavePartFlag(0x007FFFFF);	
				GetGame()->PushSavePlayer(pPlayer);
			}			
		}
		s_mapPlayer.erase( PlayerID);
	}
}

CMonster* CGame::FindMonster(const CGUID& MonsterID)
{
	CMonster * pMonster=NULL;
	// Ĭ�� Normal rgn
	CRgnManager::RgnByIDMap& rgnMap = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap();

	CRgnManager::RgnByIDMapItr rit=rgnMap.begin();
	for ( ; rit != rgnMap.end(); rit++)
	{
		CServerRegion* pRegion=rit->second;
		if(pRegion)
		{
			pMonster = pRegion->FindMonsterByID(MonsterID);
			if (pMonster!=NULL)
			{
				break;
			}
		}
	}

	return pMonster;
}

bool CGame::KickPlayer(const CGUID& PlayerGuid)
{
	long lSocketID = CMessage::GetSocketByPlayerID(PlayerGuid);
	s_pNetServer->DisConn(lSocketID);
	return true;
}

void CGame::KickAllPlayer(const CGUID& exceptID)
{
	for( itPlayer itBegin = s_mapPlayer.begin();itBegin != s_mapPlayer.end(); itBegin ++ ) 
	{
		if((*itBegin).first != exceptID)
		{
			GetGame()->KickPlayer( (*itBegin).first);
		}
	}
}

void CGame::AddRegion(eRgnType rgnType, const CGUID& RegionID, CServerRegion* pRegion)
{
	switch(rgnType)
	{
	case RGN_PERSONAL:
		{
			CRgnManager::RgnByIDMap& rgnMap = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap();
			CRgnManager::RgnByIDMapItr itr = rgnMap.find(RegionID);
			if(itr == rgnMap.end())
				rgnMap[RegionID] = pRegion;
		}
		break;
	case RGN_NORMAL:
		{
			CRgnManager::RgnByIDMap& rgnMap = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap();
			CRgnManager::RgnByIDMapItr itr = rgnMap.find(RegionID);
			if(itr == rgnMap.end())
				rgnMap[RegionID] = pRegion;
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			CRgnManager::RgnByIDMap& rgnMap = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap();
			CRgnManager::RgnByIDMapItr itr = rgnMap.find(RegionID);
			if(itr == rgnMap.end())
				rgnMap[RegionID] = pRegion;
		}
		break;
	case RGN_TEAM:
		{
			CRgnManager::RgnByIDMap& rgnMap = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap();
			CRgnManager::RgnByIDMapItr itr = rgnMap.find(RegionID);
			if(itr == rgnMap.end())
				rgnMap[RegionID] = pRegion;
		}
		break;
	case RGN_PROXY:
		{
			
		}
		break;
	}

}

CServerRegion* CGame::FindRegionByRgnID(const CGUID& RgnGUID)
{
	CServerRegion* rgn = NULL;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_NORMAL, RgnGUID);
	if(rgn != NULL)
		return rgn;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_PERSONAL, RgnGUID);
	if(rgn != NULL)
		return rgn;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_TEAM, RgnGUID);
	if(rgn != NULL)
		return rgn;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_PERSONAL_HOUSE, RgnGUID);

	return rgn;
}
CServerRegion* CGame::FindRegion(const CGUID& RgnGUID)
{
	
	CServerRegion* rgn = NULL;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_NORMAL, RgnGUID);
	if(rgn != NULL)
		return rgn;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_PERSONAL, RgnGUID);
	if(rgn != NULL)
		return rgn;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_TEAM, RgnGUID);
	if(rgn != NULL)
		return rgn;

	rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RGN_PERSONAL_HOUSE, RgnGUID);
	return rgn;
}
CServerRegion* CGame::FindProxyRegion(const CGUID& RegionID)
{
	return NULL;
}

CServerRegion* CGame::FindRegion(const char* strName)
{
	CServerRegion* rgn = NULL;

	for(CRgnManager::RgnByIDMapItr it=GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().begin(); it!=GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end(); it++)
	{
		rgn = dynamic_cast<CServerRegion*>((*it).second);
		if (rgn)
		{
			if (strcmp(rgn->GetName(), strName)==0)
			{
				return rgn;
			}
		}
		rgn = NULL;
	}

	if(rgn == NULL)
	{
		for(CRgnManager::RgnByIDMapItr it1=GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().begin(); it1!=GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end(); it1++)
		{
			rgn = dynamic_cast<CServerRegion*>((*it1).second);
			if (rgn)
			{
				if (strcmp(rgn->GetName(), strName)==0)
				{
					return rgn;
				}
			}
			rgn = NULL;
		}
	}
	if(rgn == NULL)
	{
		for(CRgnManager::RgnByIDMapItr it1=GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().begin(); it1!=GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end(); it1++)
		{
			rgn = dynamic_cast<CServerRegion*>((*it1).second);
			if (rgn)
			{
				if (strcmp(rgn->GetName(), strName)==0)
				{
					return rgn;
				}
			}
			rgn = NULL;
		}
	}
	if(rgn == NULL)
	{
		for(CRgnManager::RgnByIDMapItr it1=GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().begin(); it1!=GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end(); it1++)
		{
			rgn = dynamic_cast<CServerRegion*>((*it1).second);
			if (rgn)
			{
				if (strcmp(rgn->GetName(), strName)==0)
				{
					return rgn;
				}
			}
			rgn = NULL;
		}
	}

	return rgn;
}

CBaseObject *CGame::FindBaseObject(unsigned long  objecttype, const CGUID& guid)
{
	switch(objecttype)
	{
	case TYPE_PLAYER:
		return FindPlayer(guid);
		break;
	case TYPE_MONSTER:
		return FindMonster(guid);
		break;
	case TYPE_PET:
		return (CBaseObject*)GetInst(CPetCtrl).GetPet(guid);
		break;
	default:
		return NULL;
		break;
	}
	return NULL;
}

CMoveShape* CGame::FindShape(OBJECT_TYPE type, const CGUID& entityID)
{
	switch(type)
	{
	case TYPE_PLAYER:
		return (CMoveShape*)FindPlayer(entityID);
		break;
	case TYPE_MONSTER:
		return (CMoveShape*)FindMonster(entityID);
		break;
	case TYPE_PET:
		return (CMoveShape*)GetInst(CPetCtrl).GetPet(entityID);
		break;
	default:
		return NULL;
		break;
	}
	return NULL;
}
void CGame::SetFunctionFileData(char* pData, long lLen)
{
	M_FREE(m_strFunctionList_FileData, m_lFuncFileDataLen);
	m_strFunctionList_FileData = pData;
	m_lFuncFileDataLen = lLen;
	CScript::LoadFunction(NULL, m_strFunctionList_FileData);
}

void CGame::SetVariableFileData(char* pData, long lLen)
{
	M_FREE(m_strVariableList_FileData, m_lVariableFileDataLen);
	m_strVariableList_FileData = pData;

	m_lVariableFileDataLen = lLen;	
}

void CGame::SetGeneralVariableFileData(BYTE* pByte,long lPos)
{
	CScript::LoadGeneralVariable(pByte, lPos);
}

void CGame::SetScriptFileData(const char* filename, char* pData, long lLen)
{
	if(!filename) return;

	tagScriptDataInfoParam param;
	param.pInfoData = pData;
	param.lInfoSize = lLen;
	tagScriptDataInfo* pScriptDataInfo = MP_NEW tagScriptDataInfo(&param);
	map<string, tagScriptDataInfo*>::iterator it = m_mapScript_FileData.find(filename);
	if (it != m_mapScript_FileData.end())
	{
		MP_DELETE(it->second);
		it->second = pScriptDataInfo;
	}
	else
	{
		m_mapScript_FileData[filename] = pScriptDataInfo;
	}
}

//���빲�����ݿ�
BYTE* CGame::AllockShareDB(long lSize)
{
	return m_pDBAllocator->AllockShareDB(lSize);
}
//�ͷŹ������ݿ�
void CGame::FreeShareDB(BYTE* pData)
{
	m_pDBAllocator->FreeShareDB(pData);
}
// ˢ�����й����BasePropertyָ��
void CGame::RefreashAllMonsterBaseProperty()
{
	// normal region
	for(CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().begin(); it != GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end(); it++)
	{
		CServerRegion* pRegion = it->second;
		if( pRegion )
		{
			BEGIN_LOOP_REGION_SHAPE_BY_TYPE(pRegion, TYPE_MONSTER)
			{
				CMonster* pMonster = dynamic_cast<CMonster*>( pShape );
				if( pMonster )
				{
					
				}
			}
			END_LOOP_REGION_SHAPE_BY_TYPE;
		}
	}

	// Personal Region
	for(CRgnManager::RgnByIDMapItr it1 = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().begin(); it1 != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end(); it1++)
	{
		CServerRegion* pRegion = it1->second;
		if( pRegion )
		{
			BEGIN_LOOP_REGION_SHAPE_BY_TYPE(pRegion, TYPE_MONSTER)
			{
				CMonster* pMonster = dynamic_cast<CMonster*>( pShape );
				if( pMonster )
				{
					
				}
			}
			END_LOOP_REGION_SHAPE_BY_TYPE;
		}
	}
}



//���Ͷ�����ʾ��Ϣ���ͻ���
void CGame::SendTopInfoToClient(long lTopInfoID,const CGUID& PlayerGuid,long lFlag,long lParam,string strInfo)
{
	CMessage msgTopInf(MSG_S2C_OTHER_AddTopInfo);
	msgTopInf.Add(PlayerGuid);
	msgTopInf.Add(lTopInfoID);
	msgTopInf.Add(lFlag);
	msgTopInf.Add(lParam);
	msgTopInf.Add((char*)strInfo.c_str());
	//���͸����пͻ��˶�
	if(PlayerGuid == NULL_GUID)
	{
		msgTopInf.SendAll();
	}
	else
	{
		msgTopInf.SendToPlayer(PlayerGuid,false);
	}
}

bool CGame::CheckInvalidString(string& str,bool bReplace)
{
	return CWordsFilter::GetInstance()->Check(str,bReplace);
}


// ��Ϸ���߳�
unsigned __stdcall GameThreadFunc(void* pArguments)
{
	::CoInitialize(NULL);
#ifndef _DEBUG
	__try
	{
#endif

		CreateGame();
		BOOL ret = GetGame()->Init();

		if(ret)
		{
			while(1)
			{
				if( !GetGame()->MainLoop() )
				{
					break;
				}
			}
		}

		if(GetGame())
			GetGame()->Release();
		DeleteGame();

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode(),"Main"))
	{
		//	MessageBox(g_hWnd, "������ִ�����鿴���µĴ��󱨸��ļ�", "Error", MB_OK);

		PutDebugString("������ִ�����鿴���µĴ��󱨸��ļ�");

		if(GetGame())
			GetGame()->Release();
		DeleteGame();

		Sleep(5000);

		char str[] = "Error-GameServer";
		SendMessage(g_hWnd, WM_SETTEXT, 0, (LPARAM)str );
		//WinExec("gameserver.exe", SW_SHOW);

		PostMessage(g_hWnd, WM_DESTROY, 0, 0);
		SetEvent(g_hGameThreadExitEvent);

		_endthreadex( 0 );
		return 0;
	}
#endif

	SetEvent(g_hGameThreadExitEvent);
	PostMessage(g_hWnd, WM_CLOSE, 1, 0);

	::CoUninitialize();
	_endthreadex( 0 );
	return 0;
}

// ����WorldServer���̡߳���WorldServer�Ͽ�ʱ���ã���ͨʱ��ر�
unsigned __stdcall ConnectWorldFunc( void* pArguments )
{
	try
	{
		while(true)
		{
			if(bConnectThreadExit)
				break;
			Sleep(4000);

			if(GetGame()->ReConnectWorldServer()==TRUE)
			{
				break;
			}

		}
	}
	catch(...)
	{
		PutDebugString("����WorldServer���̳߳���!");
	}
	_endthreadex( 0 );
	return 0;
}

unsigned __stdcall		ConnectBSFunc( void* pArguments )
{
	try
	{
		while(true)
		{
			if(bConnectThreadExitBS)
				break;
			Sleep(4000);

			if(GetGame()->ReConnectBSServer()==TRUE)
			{
				break;
			}

		}
	}
	catch(...)
	{
		PutDebugString("����BillingServer���̳߳���!");
	}
	_endthreadex( 0 );
	return 0;
}

// ����WorldServer���̡߳���DBServer�Ͽ�ʱ���ã���ͨʱ��ر�
unsigned __stdcall ConnectDBFunc( void* pArguments )
{
	try
	{
		while(true)
		{
			if(bConnectThreadExitDBS)
				break;
			Sleep(8000);

			if(GetGame()->ReConnectDBServer()==TRUE)
			{
				break;
			}

		}
	}
	catch(...)
	{
		PutDebugString("����WorldServer���̳߳���!");
	}
	_endthreadex( 0 );
	return 0;
}

unsigned __stdcall ConnectBounsServerFunc(void* pArguments)
{
	try
	{
		while(true)
		{
			if(bConnectThreadExitBouns)
			{
				break;
			}

			Sleep(8000);
			
			if(GetGame()->ReConnectBounsServer())
			{
				break;
			}
		}
	}
	catch(...)
	{
		PutDebugString("����BounsServer���̳߳���!");
	}
	_endthreadex(0);
	return 0;
}

void CGame::CleanSequenceMap()
{
	for(map<CGUID,CSequenceString*>::iterator it = GetSequenceMap()->begin(); it!=GetSequenceMap()->end();)
	{
		SAFE_DELETE(it->second);
		it = GetSequenceMap()->erase(it);

	}
}
CSequenceString* CGame::GetSequence(const CGUID& guid)
{
	map<CGUID,CSequenceString*>::iterator it = GetSequenceMap()->find(guid);
	if(it != GetSequenceMap()->end())
	{
		return it->second;
	}
	return NULL;
}

bool *CGame::GetValidateTime(const CGUID& guid)
{
	map<CGUID, bool>::iterator it = GetValidateTimeMap()->find(guid);
	if( it != GetValidateTimeMap()->end() )
	{
		return &it->second;
	}
	return NULL;
}

void CGame::AppendValidateTime(const CGUID& guid, bool enable)
{
	map<CGUID, bool>::iterator it = GetValidateTimeMap()->find(guid);
	if( it != GetValidateTimeMap()->end() )
	{
		//return it->second;
	}
	GetValidateTimeMap()->insert( pair<CGUID,bool>(guid, enable) );
#ifdef _DEBUG
	AddLogText("��� ��� %d ��֤ʱ��", guid);
#endif
}

void CGame::RemoveValidateTime(const CGUID& guid)
{
	map<CGUID, bool>::iterator it = GetValidateTimeMap()->find(guid);
	if( it != GetValidateTimeMap()->end() )
	{
		GetValidateTimeMap()->erase(guid);
#ifdef _DEBUG
		AddLogText("��� ��� %d ��֤ʱ��", guid);
#endif
	}
}

long CGame::GetNumTeams()
{
	return GetOrganiCtrl()->GetTeamNum();
}



/////////////////////////
// ��ȡ��Ϸ��־�߼��ӿڶ���
LogicLogInterface* GetGameLogInterface()
{
	return GetGame()->GetLogicLogInterface();
}

void CGame::OnLOGSLost()
{
	m_LogClient.SetSoket(0);
	CMessage::SetLogSocket(0);
	CreateConnectLogThread();
}

void CGame::OnBSLost()
{
}

void CGame::OnBounsServerLost()
{
	m_lBounsSocket = 0;
	CMessage::SetBounsSocket(0);
	AddLogText("WARNING: BounsServerʧȥ����!!!");
	CreateConnectBounsServerThread();
}

//������DBServer�����ӣ���Ϊ�ͻ��˳�ʼ��
bool CGame::StartConnLogS(void)
{
	m_LogClient.SetSoket(s_pNetServer->Connect(GetSetup()->strLogIP.c_str(), GetSetup()->dwLogPort,NF_LogServer_Client));
	if(m_LogClient.GetSoket() > 0)
	{
		CMessage::SetLogSocket(m_LogClient.GetSoket());
		AddLogText("����LogServer �ɹ� !");

		CMessage msg(MSG_02LOG_SERVER_LOGIN);
		msg.Add(GetSetup()->dClientNo);
		msg.SendToSocket(m_LogClient.GetSoket());

		return true;
	}


	return false;
}

void CGame::CreateConnectLogThread(void)
{
	m_LogClient.bConnectThreadExit=true;
	WaitForSingleObject(m_LogClient.hConnectThread,INFINITE);
	CloseHandle(m_LogClient.hConnectThread);

	m_LogClient.bConnectThreadExit=false;

	unsigned threadID3;
	m_LogClient.hConnectThread = (HANDLE)_beginthreadex(NULL,0,ConnectLogServerFunc,NULL,0,&threadID3);
}

bool CGame::ReConnectLogServer()
{
	return GetLogClient().bConnectThreadExit = StartConnLogS();
}

unsigned __stdcall ConnectLogServerFunc( void* pArguments )
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


//////////////////////////////////////////////////////////////////////////
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

void	CGame::InitLogClient(CMessage *pMsg)
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

void	CGame::LogServerAffirm(CMessage *pMsg)
{
	if(0 == pMsg->GetLong())
		m_LogClient.PassAffirm(false);
	else
		m_LogClient.PassAffirm(true);
}

void	CGame::LogServerCondition(CMessage *pMsg)
{
	m_LogClient.UpdateCondition(pMsg);
}
LogicLogInterface*	CGame::GetLogicLogInterface(void)
{
	assert(NULL != m_pLogicLogInterface);
	return m_pLogicLogInterface;
};

// ���ͱ���PHDupRgn ����
void CGame::SendSavePHDupRgn(const CGUID& ownerGUID)
{
	CPlayer* player = FindPlayer(ownerGUID);
	if(player == NULL) return;

	CGUID RgnGUID = player->GetPersonalHouseRgnGUID();

	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(RgnGUID);
	if(rgn)
	{
		vector<BYTE> pBA;
		CMessage msg(MSG_S2W_SERVER_SAVEPERSONALHOUSE);

		// OPEntityID:    4byte, ����ʵ���IDֵ
		_AddToByteArray(&pBA, RgnGUID);
		_AddToByteArray(&pBA, rgn->GetExID());	
		_AddToByteArray(&pBA, rgn->GetName());
		_AddToByteArray(&pBA, ownerGUID);
		_AddToByteArray(&pBA, (long)rgn->GetID());
		_AddToByteArray(&pBA, (long)rgn->GetResourceID());
		rgn->GetVariableList()->AddToByteArray(&pBA);

		CRgnManager::PHGoodsMap& goodsMap = GameManager::GetInstance()->GetRgnManager()->GetPHGoodsMap();
		CRgnManager::PHGoodsMapItr goodsItr = goodsMap.find(rgn->GetExID());
		if(goodsItr != goodsMap.end())
		{
			_AddToByteArray(&pBA, (long)(*goodsItr).second.size());
			if ((*goodsItr).second.size() != 0)
			{
				for(int i = 0; i < (*goodsItr).second.size(); i++)
				{
					_AddToByteArray(&pBA, &((*goodsItr).second[i]), sizeof(CRgnManager::PHGoods));
				}
			}
		}
		else
		{
			_AddToByteArray(&pBA, (long)0);
		}

		// ���͸�WS
		msg.Add((long)pBA.size());
		if(pBA.size())
			msg.AddEx(&pBA[0], pBA.size());
		msg.SendToSocket(GetGame()->GetWSSocket());
	}
}

void CGame::AppendSaveMsgRegion(CServerRegion* pRegion,CMessage* pSendMsg)
{
	if(pRegion)
	{
		vector<BYTE> vecData;		
		_AddToByteArray(&vecData,(BYTE*)pRegion->GetRegionParam(),(long)sizeof(tagRegionParam));		
		pSendMsg->AddEx(&vecData[0],vecData.size());
	}
}

// ���ݷֲ�洢�ӿ�
void CGame::AppendSaveMsgProperty(CPlayer* pPlayer, DBWriteSet& setWriteDB)
{
	if(pPlayer)
	{
		// flag
		setWriteDB.AddToByteArray( (BYTE)1);
		// ��¼����----------------BEGIN
		setWriteDB.AddToByteArray( (char*)pPlayer->GetName());
		//	�ȼ�
		setWriteDB.AddToByteArray( (WORD)pPlayer->GetLevel());
		//	ְҵ
		setWriteDB.AddToByteArray( (BYTE)pPlayer->GetOccupation() );
		//	�Ա�(0:�� 1:Ů)
		setWriteDB.AddToByteArray( (BYTE)pPlayer->GetSex() );	
		setWriteDB.AddToByteArray( (BYTE)pPlayer->GetCountry());
		//	ͷ��
		setWriteDB.AddToByteArray( (long)pPlayer->GetHeadPic() );
		// ����
		setWriteDB.AddToByteArray( (long)pPlayer->GetFacePic() );
		// posX
		setWriteDB.AddToByteArray( (float)pPlayer->GetPosX());
		// posY
		setWriteDB.AddToByteArray( (float)pPlayer->GetPosY());
		//ticket
		setWriteDB.AddToByteArray( (long)pPlayer->GetTicket() );

		BYTE dwNull = 0;
		CGoods* pGoods = NULL;
		// ----------------------------
		//	1��ͷ��
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_HEAD );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	2������
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_NECKLACE );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	3�����
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_WING );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	4������
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_BODY );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	5������
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_BACK );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	6������	
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_GLOVE );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	7��Ь��
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_BOOT );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	8��ͷ��
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_HEADGEAR );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	9������		
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_FROCK );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	10�����ָ
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_LRING );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	11���ҽ�ָ		
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_RRING );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	12��ѫ��1
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_MEDAL1 );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	13	ѫ��2
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_MEDAL2 );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	14	ѫ��3
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_MEDAL3 );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	15��С����
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_FAIRY );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	16������
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_WEAPON );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) );// ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level
		//	17����������
		pGoods = pPlayer -> m_cEquipment.GetGoods(CEquipmentContainer::EC_WEAPON2 );
		setWriteDB.AddToByteArray( (BYTE)(pGoods?pGoods->GetBasePropertiesIndex():dwNull) ); // ID
		setWriteDB.AddToByteArray( (long)(pGoods?pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL,1):dwNull) ); // Level

		setWriteDB.AddToByteArray( (long)pPlayer->GetRegionID() );
		// ��¼����----------------END

		// ��ϸ����----------------BEGIN
		//������������ʱ��,����崻���������ʱ�䲻׼ȷ
		time_t curTime;
		time(&curTime);
		pPlayer->SetLastExitGameTime((DWORD)curTime);
		setWriteDB.AddToByteArray( &pPlayer->GetProperty(), sizeof(tagPlayerProperty));
		if(pPlayer)
			pPlayer->GetPlayerDepot()->CodeSetupToDataBlock(setWriteDB);
		// ��ϸ����----------------END

		//ϵͳ�ʼ�----------------BEGIN		
		map<long,long> mapSysMailList = pPlayer->GetSysMailList();
		setWriteDB.AddToByteArray((long)mapSysMailList.size());
		map<long,long>::iterator mit=mapSysMailList.begin();
		for (; mit!=mapSysMailList.end(); ++mit)
		{
			setWriteDB.AddToByteArray((*mit).first);
			setWriteDB.AddToByteArray((*mit).second);			
		}
		//ϵͳ�ʼ�----------------END

		//! ѫ������
		pPlayer->m_MedalContainer.CodeToDataBlock(setWriteDB);

	}
	else
	{
		// flag
		setWriteDB.AddToByteArray( (BYTE)0);
	}
}
void CGame::AppendSaveMsgWalletGoods(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->m_cWallet.Serialize(setWriteDB);	
}

void CGame::AppendSaveMsgSilverGoods(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->m_cSilverWallet.Serialize(setWriteDB);
}
void CGame::AppendSaveMsgDepotGold(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;

	char outStr[128] = {0};
	CGoodsContainer *pContainer = player->GetPlayerDepot()->FindContainer(eDCT_Gold, true);
	if(NULL == pContainer)
	{
		_snprintf(outStr, 128, "AppendSaveMsgDepotGold:Goods's Container is NULL!");
		AddLogText(outStr);
		setWriteDB.AddToByteArray( (long)0); // crc32
		return;
	}
	setWriteDB.AddToByteArray( (long)1); // flag
	pContainer->Serialize(setWriteDB);	
}
void CGame::AppendSaveMsgDepotSilver(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;

	char outStr[128] = {0};
	CGoodsContainer *pContainer = player->GetPlayerDepot()->FindContainer(eDCT_Silver, true);
	if(NULL == pContainer)
	{
		_snprintf(outStr, 128, "AppendSaveMsgDepotSilver:Goods's Container is NULL!");
		AddLogText(outStr);
		setWriteDB.AddToByteArray( (long)0); // flag
		return;
	}
	setWriteDB.AddToByteArray( (long)1); // flag
	pContainer->Serialize(setWriteDB);
}
void CGame::AppendSaveMsgDepotGoods(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;

	char outStr[128] = {0};
	CGoodsContainer *pContainer = player->GetPlayerDepot()->FindContainer(eDCT_Primary, true);
	if(NULL == pContainer)
	{
		_snprintf(outStr, 128, "DecodeDepotGoodsMsg:Goods's Container is NULL!");
		AddLogText(outStr);
		setWriteDB.AddToByteArray((long)0); // flag
		return;
	}

	setWriteDB.AddToByteArray((long)1); // flag
	pContainer->Serialize(setWriteDB);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::AppendSaveMsgSubDepot(long packID, CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	//�ӱ������л�
	player->SerializeSubDepot(setWriteDB, packID + eDCT_Secondary1);
}
void CGame::AppendSaveMsgSpriteSystem(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->m_SpriteSystem.CodeToDataBlock(setWriteDB);
}

void CGame::AppendSaveMsgEquipmentGoods(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->m_cEquipment.Serialize(setWriteDB);
}
void CGame::AppendSaveMsgPackGoods(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->getOriginPack()->Serialize(setWriteDB);
}
void CGame::AppendSaveMsgSubPackGoods(long packID, CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	//�ӱ������л�
	player->SerializeSubpack(setWriteDB, (CS2CContainerObjectMove::PLAYER_EXTEND_ID)(packID+CS2CContainerObjectMove::PEI_PACK1));
}
void CGame::AppendSaveMsgVarList(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->GetVariableList()->CodeToDataBlock(setWriteDB);
}
void CGame::AppendSaveMsgQuestList(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->CodeQuestDataToDataBlock(setWriteDB);
}

void CGame::AppendSaveMsgMerQuestList(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->CodeMerQuestDataToDataBlock(setWriteDB);
}


void CGame::AppendSaveMsgSkillList(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	//skills id field
	setWriteDB.AddToByteArray((long)player->GetNumSkills());
	CPlayer::itSkill it = player->GetSkillMap().begin();
	for(;it!=player->GetSkillMap().end();++it)
	{
		setWriteDB.AddToByteArray( static_cast<DWORD>(it->second.dwID) );
		setWriteDB.AddToByteArray( static_cast<DWORD>(it->second.wLevel) );
		setWriteDB.AddToByteArray( static_cast<DWORD>(it->second.wCooldown) );
	}
}


void CGame::AppendSaveMsgStateList(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->CodeBuffToDataBlock(setWriteDB);
}
void CGame::AppendSaveMsgDeputyOccuList(CPlayer* player, DBWriteSet& setWriteDB)
{
	if(!player) return;
	player->CodeDOccuToDataBlock(setWriteDB);
}

// ��ӻ����޸�һ�����Ҫ���������ö��ֵ
void CGame::UpdateSavePlayer(const CGUID& playerID, SAVE_DETAIL_ATTR_TYPE type)
{
	CPlayer* player = GetGame()->FindPlayer(playerID);
	if(player)
		UpdateSavePlayer(player, type);
}


void CGame::UpdatePlayerPack(CPlayer* pPlayer,DWORD dwContainerId,DWORD dwPos)
{
	if(!pPlayer)
		return;
	switch((CS2CContainerObjectMove::PLAYER_EXTEND_ID)dwContainerId)
	{
	case CS2CContainerObjectMove::PEI_PACKET://��ͨ����
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRPACKGOODS);
		break;
	case CS2CContainerObjectMove::PEI_EQUIPMENT://װ����
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTREQUIPMENTGOODS);
		break;
	case CS2CContainerObjectMove::PEI_WALLET://��Ǯ
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRWALLETGOODS);
		break;
	case CS2CContainerObjectMove::PEI_SILVERWALLET://����
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSILVERGOODS);
		break;
	case CS2CContainerObjectMove::PEI_PACK:
		{
			switch(dwPos)
			{
			case CS2CContainerObjectMove::PEI_PACK1://�ӱ�����λ1
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS1);
				break;
			case CS2CContainerObjectMove::PEI_PACK2://�ӱ�����λ2
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS2);
				break;
			case CS2CContainerObjectMove::PEI_PACK3://�ӱ�����λ3
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS3);
				break;
			case CS2CContainerObjectMove::PEI_PACK4://�ӱ�����λ4
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS4);
				break;
			case CS2CContainerObjectMove::PEI_PACK5://�ӱ�����λ5
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS5);
				break;
			}
		}
		break;
	case CS2CContainerObjectMove::PEI_PACK1://�ӱ�����λ1
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS1);
		break;
	case CS2CContainerObjectMove::PEI_PACK2://�ӱ�����λ2
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS2);
		break;
	case CS2CContainerObjectMove::PEI_PACK3://�ӱ�����λ3
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS3);
		break;
	case CS2CContainerObjectMove::PEI_PACK4://�ӱ�����λ4
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS4);
		break;
	case CS2CContainerObjectMove::PEI_PACK5://�ӱ�����λ5
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS5);
		break;
	case eDCT_Gold:
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRPROPERTY);
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRDEPOTGOLD);
		break;
	case eDCT_Silver:
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRPROPERTY);
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRDEPOTSILVER);
		break;
	case eDCT_Primary:
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRPROPERTY);
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRPRIMARY);
		break;
	case eDCT_Secondary:
		{
			switch(dwPos)
			{
			case CS2CContainerObjectMove::PEI_PACK1://�ӱ�����λ1
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT1);
				break;
			case CS2CContainerObjectMove::PEI_PACK2://�ӱ�����λ2
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT2);
				break;
			case CS2CContainerObjectMove::PEI_PACK3://�ӱ�����λ3
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT3);
				break;
			case CS2CContainerObjectMove::PEI_PACK4://�ӱ�����λ4
				UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT4);
				break;
			}
		}
		break;
	case eDCT_Secondary1:						
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT1);
		break;
	case eDCT_Secondary2:						
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT2);
		break;
	case eDCT_Secondary3:						
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT3);
		break;
	case eDCT_Secondary4:						
		UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBDEPOT4);
		break;
		// ��ҵ����
	case CS2CContainerObjectMove::PEI_BUSSINESPACK:
	case CS2CContainerObjectMove::PEI_BUSSINESPACKFIELD:
		UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRBUSINESSPACK );
		break;
	}
}




// ��ӻ����޸�һ�����Ҫ���������ö��ֵ
void CGame::UpdateSavePlayer(CPlayer* player, SAVE_DETAIL_ATTR_TYPE type)
{
	if(player)
	{
		DWORD flag = player->GetSavePartFlag();
		flag = flag | type;
		player->SetSavePartFlag(flag);
	}
}

VOID CGame::SendNotifyMessage( char* szText,const CGUID& playerGuid, DWORD dwColor, DWORD dwBkColor,eNOTIFY_POS ePos)
{
	if( szText )
	{
		CMessage msg( MSG_S2C_OTHER_ADDINFO );
		msg.Add( dwColor );
		msg.Add( dwBkColor );
		msg.Add((BYTE)ePos);
		msg.Add( szText );
		msg.SendToPlayer(playerGuid,false);
	}
}

VOID CGame::SendNotifyMessage( char* szText,CServerRegion* pRegion, DWORD dwColor, DWORD dwBkColor,eNOTIFY_POS ePos)
{
	if( szText && pRegion)
	{
		CMessage msg( MSG_S2C_OTHER_ADDINFO );
		msg.Add( dwColor );
		msg.Add( dwBkColor );
		msg.Add((BYTE)ePos);
		msg.Add( szText );
		msg.SendToRegion(pRegion);
	}
}

//������Ϣ���������
void CGame::SendMsgToAllPlayer(CMessage *pMsg)
{
	for( itPlayer it = s_mapPlayer.begin();it != s_mapPlayer.end(); ++it)
	{	
		pMsg->SendToPlayer(it->second->GetExID());
	}
}
//������Ϣ��ͬ���ҵ����
void CGame::SendMsgToSameCountryPlayer(CMessage *pMsg,BYTE btCountry)
{
	for( itPlayer it = s_mapPlayer.begin();it != s_mapPlayer.end(); ++it)
	{
		if( it->second && it->second->GetCountry() == btCountry)
		{
			pMsg->SendToPlayer(it->second->GetExID());
		}
	}
}
//������Ϣ��ͬ��Ӫ���ҵ����
void CGame::SendMsgToSameCampPlayer(CMessage *pMsg,BYTE btCountry)
{
	//! �õ�һ����Ӫ�����й���ID
	list<BYTE> UnionCountryIDList;
	GetPKSys()->GetCountryList(btCountry, UnionCountryIDList);

	for( itPlayer it = s_mapPlayer.begin();it != s_mapPlayer.end(); ++it)
	{
		for (list<BYTE>::iterator ite = UnionCountryIDList.begin(); ite != UnionCountryIDList.end(); ++ite)
		{
			if (it->second && it->second->GetCountry() == *ite)
			{
				pMsg->SendToPlayer(it->second->GetExID());
				break;
			}
		}
	}
}

//��ѯ�������
void CGame::QuerySilencePlayers(CGUID& GmID)
{
	size_t lSize = 0;
	/**## ����׼����������С */
	for( itPlayer itBegin = s_mapPlayer.begin();itBegin != s_mapPlayer.end(); itBegin ++ )
	{
		CPlayer* pPlayer = (*itBegin).second;
		if( pPlayer && pPlayer->IsInSilence() )
		{
			lSize += strlen( pPlayer->GetName() );
			lSize += 2; /**## Ϊ";"�ű���һ��λ�� */
		}
	}

	/**## �ж��Ƿ����˱����� */
	/**## MSG_S2W_GM_ANSWER_QUERY_SILENCE_PLAYERS ��ʽ */
	/**## long lGMID, long size, char* pList */
	CMessage msg( MSG_S2W_GM_ANSWER_QUERY_SILENCE_PLAYERS );
	if( lSize )
	{
		char* pText = (char*)M_ALLOC(sizeof(char)*(lSize + 64));
		if( pText )
		{
			ZeroMemory( pText, lSize + 64 );
			msg.Add( GmID );
			msg.Add( static_cast<long>(lSize + 64) );
			for( itPlayer itBegin = s_mapPlayer.begin();
				itBegin != s_mapPlayer.end(); itBegin ++ )
			{
				if( itBegin -> second && itBegin -> second -> IsInSilence() )
				{
					strcat( pText, itBegin -> second -> GetName() );
					strcat( pText, "��" );
				}
			}
			msg.Add( pText );
			msg.Send();

			/**## ɾ���������Դ */
			M_FREE( pText, sizeof(char)*(lSize + 64));
		}
		else 
		{
			msg.Add( GmID );
			msg.Add( static_cast<long>(24) );
			msg.Add( "��Դ���㣬�޷���ѯ��" );
			msg.Send();
		}
	}
	else 
	{
		msg.Add( GmID );
		msg.Add( static_cast<long>(24) );
		msg.Add( "�޽�����ҡ�" );
		msg.Send();
	}
}

void CGame::AddSaveDetailPartToByteArray(const CGUID& guid, DWORD flag, CPlayer* player)
{
	if(!player) return;

	char szGUID[128];
	CMessage msg(MSG_S2W_PLAYER_DETAIL_SAVE_PROPERTY);
	msg.Add(guid); // player guid

	guid.tostring(szGUID);


#ifdef _RUNSTACKINFO3_
	char pszStackInfo[1024]="";
	_snprintf(pszStackInfo,1024,"AddSaveDetailPartToByteArray(PlayerName:%s,guid:%s,saveflat:%d) Start.",player->GetName(),szGUID,flag);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif

	flag = flag|SAVE_DETAIL_ATTRVARLIST;
	flag = flag|SAVE_DETAIL_ATTRQUESTLIST;

	msg.Add(flag); // flag

	DBWriteSet setWriteDB;
	msg.GetDBWriteSet(setWriteDB);
	// ��װ��Ϣ
	if(flag & SAVE_DETAIL_ATTRPROPERTY)// ��������:��Ǯ,�ȼ�...��
	{
		AppendSaveMsgProperty(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTREQUIPMENTGOODS)// װ����Ʒ
	{
		AppendSaveMsgEquipmentGoods(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRPACKGOODS)// ��������Ʒ
	{
		AppendSaveMsgPackGoods(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBPACKGOODS1)// �ӱ���1��Ʒ
	{
		AppendSaveMsgSubPackGoods(0, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBPACKGOODS2)// �ӱ���2��Ʒ
	{
		AppendSaveMsgSubPackGoods(1, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBPACKGOODS3)// �ӱ���3��Ʒ
	{	
		AppendSaveMsgSubPackGoods(2, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBPACKGOODS4)// �ӱ���4��Ʒ
	{	
		AppendSaveMsgSubPackGoods(3, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBPACKGOODS5)// �ӱ���5��Ʒ
	{
		AppendSaveMsgSubPackGoods(4, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRWALLETGOODS)// ���Ǯ����Ʒ
	{
		AppendSaveMsgWalletGoods(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSILVERGOODS)// ����Ǯ����Ʒ
	{
		AppendSaveMsgSilverGoods(player, setWriteDB);
	}

	if(flag & SAVE_DETAIL_ATTRDEPOTGOLD)// �ƽ�ֿ�
	{
		AppendSaveMsgDepotGold(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRDEPOTSILVER)// ���Ҳֿ�
	{
		AppendSaveMsgDepotSilver(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRPRIMARY)// ���ֿ�
	{
		AppendSaveMsgDepotGoods(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBDEPOT1)// �Ӳֿ�1
	{
		AppendSaveMsgSubDepot(0, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBDEPOT2)// �Ӳֿ�2
	{
		AppendSaveMsgSubDepot(1, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBDEPOT3)// �Ӳֿ�3
	{
		AppendSaveMsgSubDepot(2, player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSUBDEPOT4)// �Ӳֿ�4
	{
		AppendSaveMsgSubDepot(3, player, setWriteDB);
	}
	if( flag & SAVE_DETAIL_ATTRBUSINESSPACK ) // ��ҵ����
	{
		player->GetBusinessContainer().EncodeToSave( setWriteDB );
	}
	if(flag & SAVE_DETAIL_ATTRVARLIST)// �����б�
	{
		AppendSaveMsgVarList(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRQUESTLIST)// �����б�
	{	
		AppendSaveMsgQuestList(player, setWriteDB);
	}

	if(flag & SAVE_DETAIL_ATTRSKILLLIST)// �����б�
	{
		AppendSaveMsgSkillList(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRSTATELIST)// ״̬�б�
	{	
		AppendSaveMsgStateList(player, setWriteDB);
	}
	if(flag & SAVE_DETAIL_ATTRDEPUTYOCCU)// ��ְҵ
	{	
		AppendSaveMsgDeputyOccuList(player, setWriteDB);
	}

	if(flag & SAVE_DETAIL_ATTRMERQUESTLIST)// ٸ�������б�
	{	
		AppendSaveMsgMerQuestList(player, setWriteDB);
	}

	if( flag & SAVE_DETAIL_PETS )			// �����б�
	{
		GetInst(CPetCtrl).CodeToDataBlock(setWriteDB, player);
	}


	// С����ϵͳ
    if (flag & SAVE_DETAIL_INNERHOOK)
    {
       AppendSaveMsgSpriteSystem(player,setWriteDB); 
    }
    
	// ����ϵͳ
	msg.SendToSocket(GetWSSocket());
	// �����־λ
	player->SetSavePartFlag((DWORD)0);

#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"AddSaveDetailPartToByteArray() End.");
#endif
}
void CGame::ClearSavePlayer(const CGUID& guid)
{
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::ClearSavePlayer() Start.");
#endif

	map<CGUID, CPlayer*>::iterator setItr = m_mapSavePlayers.find(guid);
	if( setItr != m_mapSavePlayers.end() )
	{
		GameManager::GetInstance()->FreeObject((CBaseObject**)&(setItr->second));
		setItr = m_mapSavePlayers.erase(setItr);
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CGame::ClearSavePlayer() End.");
#endif
}
// ������ұ���,������Ϣ
void CGame::SendQueuePlayerOperMsg(void)
{
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"SendSaveDetailPartMsg() Start.");
#endif

	if(GetWSSocket() > 0)
	{
		itPlayer spItr = s_mapPlayer.begin();
		while( spItr != s_mapPlayer.end() )
		{
			if( NULL == spItr->second )
			{
				spItr++;
				continue;
			}
			DWORD flag = spItr->second->GetSavePartFlag();
			if( 0 == flag ) // ��ʾ�л��������������
			{
				spItr++;
				continue;
			}
			AddSaveDetailPartToByteArray(spItr->first, flag, spItr->second);
			spItr++;
		}
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"SendSaveDetailPartMsg() End.");
#endif
}
void CGame::ClearChangeRgnPlayerVec(void)
{
	itCRP itr = m_ChangeRgnPlayers.begin();
	for(; itr != m_ChangeRgnPlayers.end(); ++itr)
	{
		////�������ڴ˷������ϣ�����������
		OnPlayerExit( (*itr).first, TRUE);

		//�����л�������������Ϣ
		CPlayer* pPlayer = GetGame()->FindPlayer((*itr).first);
		if(pPlayer)
		{
			tagPCR& ChangePlayer = (*itr).second;
			CMessage msg( MSG_S2W_SERVER_CHANGEREGION);
			msg.Add(ChangePlayer.lType);
			msg.Add(ChangePlayer.ltemplateID); // ��ͨ������ģ��ID���Բ��ҵ�GUID
			msg.Add(pPlayer->GetExID());
			msg.Add(ChangePlayer.RegionID);
			msg.Add(ChangePlayer.lX);
			msg.Add(ChangePlayer.lY);
			msg.Add(ChangePlayer.lDir);
			msg.Add(ChangePlayer.lUseGoods);
			msg.Add(ChangePlayer.lRange);
			DBWriteSet setWriteDataDB;
			msg.GetDBWriteSet(setWriteDataDB);
			pPlayer->CodeToDataBlock(setWriteDataDB);
			msg.Send();
		}

		RemovePlayer((*itr).first);
	}
	m_ChangeRgnPlayers.clear();
}

void CGame::PushChangeRgnPlayer(const CGUID& playerID,tagPCR& ChangeRegion)
{
	if(playerID != NULL_GUID)
	{
		m_ChangeRgnPlayers[playerID] = ChangeRegion;
	}
}

void CGame::PushDelDupRgn(CServerRegion* pRgn)
{
	if(pRgn)
	{
		map<CGUID, CServerRegion*>::iterator itr = m_mapDelDupRgn.find(pRgn->GetExID());
		if(itr == m_mapDelDupRgn.end())
		{
			m_mapDelDupRgn[pRgn->GetExID()] = pRgn;
		}
		else
		{
			char szGuid[128];
			pRgn->GetExID().tostring(szGuid);
			AddLogText("PushDelDupRgn() rgn[%s] is exist!", szGuid);
		}
	}
}
void CGame::ClearDelDupRgn(void)
{
	map<CGUID, CServerRegion*>::iterator itr = m_mapDelDupRgn.begin();
	for(; itr != m_mapDelDupRgn.end(); itr++)
	{
		if(itr->second)
		{
			// �Ƚ��ó��������ǿ������
			hashsetGUID tmpPlayerSet;
			itr->second->FindAllPlayer(tmpPlayerSet);
			hashsetGUID::iterator pitr = tmpPlayerSet.begin();
			for(; pitr != tmpPlayerSet.end(); pitr++)
			{
				// ����쳣
				char szGuid[128];
				(*pitr).tostring(szGuid);
				AddLogText("ClearDelDupRgn() ���[%s]�ӱ��������[%d]�л�����ʱ�쳣", szGuid, itr->second->GetID());
				CPlayer* pPlayer = GetGame()->FindPlayer((*pitr));
				// �����ڳ���ָ������Ϊ��
				if(pPlayer)
					pPlayer->SetFather(NULL);
				GetGame()->KickPlayer((*pitr));
			}
		}
		// ɾ���ó���
		MP_DELETE(itr->second);
	}
	m_mapDelDupRgn.clear();
}
void DoSaveData()
{
	// ��CDBEntity�����͵�WS

}
// �����߳�
unsigned __stdcall SaveThreadFunc(void* pArguments)
{
	AddLogText("SaveThread Starting...");


#ifndef _DEBUG
	__try
	{
#endif

		DoSaveData();

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		PutDebugString("�����̳߳��ִ�����鿴���µĴ��󱨸��ļ�");
		AddErrorLogText("�����̳߳��ִ�����鿴���µĴ��󱨸��ļ�");
	}
#endif
	_endthreadex( 0 );
	return 0;
}

void CGame::PushSavePlayer(CPlayer* pPlayer)
{
	if(!pPlayer) return;
	map<CGUID, CPlayer*>::iterator itr = m_mapSavePlayers.find(pPlayer->GetExID());
	if(itr == m_mapSavePlayers.end())
		m_mapSavePlayers[pPlayer->GetExID()] = pPlayer;
	else
	{
		if(itr->second != pPlayer)
		{
			delete itr->second;
			m_mapSavePlayers.erase(itr);
			m_mapSavePlayers[pPlayer->GetExID()] = pPlayer;
		}
		else
		{
			char szGuid[128];
			pPlayer->GetExID().tostring(szGuid);
			AddLogText("PushSavePlayer() player[%s] is Exist!", szGuid);
		}
	}
}
CPlayer* CGame::FindSavePlayer(const CGUID& guid)
{
	map<CGUID, CPlayer*>::iterator itr = m_mapSavePlayers.find(guid);
	if(itr != m_mapSavePlayers.end())
		return itr->second;
	return NULL;
}

VOID CGame::LogRegion(VOID)
{
	for(CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().begin(); it != GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end(); it++)
		GetGameLogInterface()->logT800_player_num_in_region(GetGame()->GetIDIndex(), it->second);

	//! ���������������־��¼
	for(CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().begin(); it != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end(); it++)
		GetGameLogInterface()->logT800_player_num_in_region(GetGame()->GetIDIndex(), it->second);

	for(CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().begin(); it != GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end(); it++)
		GetGameLogInterface()->logT800_player_num_in_region(GetGame()->GetIDIndex(), it->second);

	for(CRgnManager::RgnByIDMapItr it = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().begin(); it != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end(); it++)
		GetGameLogInterface()->logT800_player_num_in_region(GetGame()->GetIDIndex(), it->second);
}

/*
<!--================================================================-->
<!--ÿ��ͼ�ϵ��������¼-->
<Table name="t800_player_num_in_region" log_no="800" log_pos="1">
<item type="alone" Item_name="gs_id" data_type="int32"></item>
<item type="client_head" head_name=""></item>
<item type="alone" Item_name="region_name" data_type="int32"></item>
<item type="alone" Item_name="region_id" data_type="int32"></item>
<item type="alone" Item_name="region_guid" data_type="int32"></item>
<item type="alone" Item_name="player_num" data_type="int32"></item> <!--�������-->
</Table>
*/

bool LogicLogInterface::logT800_player_num_in_region(LONG lGsID, CServerRegion *pCServerRegion)
{
	{
		if(0 == GetGame()->GetSetup()->lUseLogServer) 
			return true;
		if(NULL == m_pLogClient)
			return false;
		if(NULL == pCServerRegion)
			return false;

		SYSTEMTIME	stTime;
		char		szTime[128] = {0};
		GetLocalTime(&stTime);

		LONG lYu = stTime.wMinute % 5;
		if(0 != lYu)
		{
			stTime.wMinute -= lYu;
		}

		_snprintf(szTime,128, "%d-%d-%d %d:%d:00" ,
			stTime.wYear,
			stTime.wMonth,
			stTime.wDay,
			stTime.wHour,
			stTime.wMinute,
			stTime.wSecond);

		return m_pLogClient->SendLogToServer(
			800, szTime,
			lGsID,
			pCServerRegion->GetName(),
			pCServerRegion->GetID(),
			&(pCServerRegion->GetExID()),
			pCServerRegion->GetPlayerAmout()
			);
	}
}
void CGame::OutPlayerAndRegionInfo(const char* pszFileName)
{
	if(pszFileName)
	{
		char str[256];
		PutStringToFile(pszFileName,"OutPlayerAndRegionInfo:");
		_snprintf(str, 256, "DelaySavePlayer Num:%d",m_mapSavePlayers.size());
		PutStringToFile(pszFileName,str);

		_snprintf(str, 256, "DeleteDupRgn Num:%d",m_mapDelDupRgn.size());
		PutStringToFile(pszFileName,str);
	}
}

//! ��Ӧ�µ�һ�쿪ʼ��
void CGame::OnNewDayStart(DWORD dwTime)
{
	ResetStarDay(dwTime);

	//! �������CP
	itPlayer ite = s_mapPlayer.begin();
	for (; ite != s_mapPlayer.end(); ++ite)
	{
		if(NULL != ite->second)
		ite->second->ResetActive(dwTime);
	}
}

//! ����������
void CGame::ResetStarDay(DWORD dwTime)
{
	tm tm2007;
	tm2007.tm_sec	= 59;/* seconds after the minute - [0,59] */
	tm2007.tm_min	= 59;/* minutes after the hour - [0,59] */
	tm2007.tm_hour	= 23;/* hours since midnight - [0,23] */
	tm2007.tm_mday	= 1;/* day of the month - [1,31] */
	tm2007.tm_mon	= 3;/* months since January - [0,11] */
	tm2007.tm_year	= 2007 - 1900;/* years since 1900 */

	long lDiffSec = dwTime - mktime(&tm2007);
	long lDiffDay = lDiffSec / (60 * 60 * 24) + 1 + 7/*Ϊ�˸��߻��ļ�����ͳһ��+7*/;

	static eConstellation arrConstellation[14] = 
	{
		CONST_No,				//��
		CONST_Aries,			//������
		CONST_Taurus,			//��ţ��
		CONST_Gemini,			//˫����
		CONST_Cancer,			//��з��
		CONST_Leo,				//ʨ����
		CONST_Virgo,			//��Ů��
		CONST_No,
		CONST_Libra,			//�����
		CONST_Scorpio,			//��Ы��
		CONST_Sagittarius,		//������
		CONST_Capricorn,		//Ħ����
		CONST_Aquarius,			//ˮƿ��
		CONST_Pisces,			//˫����
	};

	m_Constellation = arrConstellation[lDiffDay % 14];
}

//! �õ�������
eConstellation CGame::GetStarDay(void)
{
	return m_Constellation;
}

// ���ӽ���������Ϣ
void CGame::AddBounsDes(tagBounsDes *pBD)
{
	if(pBD == NULL)
	{
		return;
	}

	map<DWORD, tagBounsDes*>::iterator itor = m_mapBounsDes.find(pBD->dwBounsID);
	if(itor != m_mapBounsDes.end())
	{
		// �Ѿ������н���������
		OBJ_RELEASE(tagBounsDes, itor->second);
		m_mapBounsDes.erase(itor);
	}

	m_mapBounsDes[pBD->dwBounsID] = pBD;
}

tagBounsDes* CGame::FindBounsDes(DWORD dwBounsID)
{
	map<DWORD, tagBounsDes*>::iterator itor = m_mapBounsDes.find(dwBounsID);
	if(itor != m_mapBounsDes.end())
	{
		return itor->second;
	}
	return NULL;
}

void CGame::ClearBounsDes()
{
	for(map<DWORD, tagBounsDes*>::iterator itor = GetBounsDes()->begin(); itor!=GetBounsDes()->end();)
	{
		OBJ_RELEASE(tagBounsDes, itor->second);
		itor = GetBounsDes()->erase(itor);
	}
}

void CGame::OnReceiveSdoItemRequest(tagSDOA_ITEM_SEND_REQUEST &stSdo_Item_Send_Request)
{
#ifdef _ASCYN_DEBUG
	char szOutPut[512];
	_snprintf(szOutPut,512,"CGame::OnReceiveSdoItemRequest(tagSDOA_ITEM_SEND_REQUEST stSdo_Item_Send_Request),szOrderId:%s,",stSdo_Item_Send_Request.szOrderId);
	PutStringToFile("SDO_ITEM_SEND_RESPONSE",szOutPut);
#endif
	CMessage msg(MSG_B2S_SDO_ITEM_SEND_REQUEST);
	msg.Add(stSdo_Item_Send_Request.payErrorMsg);
	msg.Add(stSdo_Item_Send_Request.szCallTime);
	msg.Add(stSdo_Item_Send_Request.szGoodsInfoEx);
	msg.Add(stSdo_Item_Send_Request.szGoodsNum);
	msg.Add(stSdo_Item_Send_Request.szIndication);
	msg.Add(stSdo_Item_Send_Request.szOrderId);
	msg.Add(stSdo_Item_Send_Request.szPayStatus);
	msg.Add(stSdo_Item_Send_Request.szRoleName);
	msg.Add(stSdo_Item_Send_Request.szSendTimes);
	msg.Add(stSdo_Item_Send_Request.szUserId);
	msg.Add((WORD)stSdo_Item_Send_Request.nMsgType);
	msg.Add((WORD)stSdo_Item_Send_Request.nMsgFrom);
	msg.Add((WORD)stSdo_Item_Send_Request.nMsgSeq);
	msg.SetTotalSize();

	CMessage *pMsg = CMessage::AllocMessage();
	if(pMsg)	
	{
		vector<CDataBlock*>& MsgData = msg.GetMsgData();
		pMsg->Init(MsgData,NULL,FALSE);
		MsgData.clear();
	}

	m_RecvSDOMessages.PushMessage(pMsg);
}