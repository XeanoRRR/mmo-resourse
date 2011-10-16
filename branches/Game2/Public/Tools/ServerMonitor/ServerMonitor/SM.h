#pragma once
#include "StdAfx.h"
#include "CmdLine.h"
#include "../App/Gapp.h"
#include "../App/HardWareInfo.h"
#include "../App/SMDef.h"
#include "../net/Message.h"
#include "../../../SCMPublic/def/InfoDef.h"

class CServer;
class CMemServer;
class CDataBlockAllocator;
typedef CTimerQueue<CGapp*> GappTimer;
enum eNetFlag
{
	NF_SCC_Client=0x001,
	NF_SM_Client=0x010,
	NF_DBServer_Client=0x100,// [AHC 080809 DBSMSG]
	NF_BillingServer_Client=0x1000,
	NF_LogServer_Client = 0x10000,
};

class CSM
{
public:
	CSM(void);
	~CSM(void);
	
	struct tagSetup
	{
		//long	lUseLogServer;		// �Ƿ�ʹ��LogServer��0=��ʹ�ã�1=ʹ��
		//string	strLogIP;			// LogServerIP
	//	DWORD	dwLogPort;			// LogServerPort
	//	string  strErrFile;			// �����ļ�·��+�ļ���
	//	string  strLogFile;			// ������־�ļ�·��+�ļ�����SQL��ʽ��
	//	string  strTxtLogFile;		// ������־�ļ�·��+�ļ��������ĸ�ʽ��
	//	DWORD	dClientNo;			// �ͻ��˱��
		//long	lUseBillingServer;		//�Ƿ�����BillingServers
		

		//string	strLoginIP;			// ��¼IP
		//long	dwLoginPort;		// ��¼PORT
		//DWORD	dwLoginPassword;	//��¼SCC����
		long	lSwitch;			//����,��ʾ�Ƿ��Զ�����SCC

		long	lMaxBlockConNum;
		long	lValidDelayRecDataTime;

		DWORD	dwDataBlockNum;			//���ݿ�����
		DWORD	dwDataBlockSize;		//���ݿ��С
		DWORD	dwFreeMsgNum;			//Ԥ�������Ϣ����

		DWORD	dwFreeSockOperNum;		//�����������Ԥ��������
		DWORD	dwFreeIOOperNum;		//��ɶ˿���IO����Ԥ��������
		long	lIOOperDataBufNum;		//Ĭ��IO������DataBuf����

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
	};
	tagScc2SmSetup m_SCCPrama;
	//���빲�����ݿ�
	BYTE* AllockShareDB(long lSize);
	//�ͷŹ������ݿ�
	void FreeShareDB(BYTE* pData);


	// SCC��Ϣ
	struct tagSCC
	{
		long			lAuto;
		DWORD			dwIndex;			// ���������
		string			strIP;				// IP
		DWORD			dwPort;				//��¼�˿�
		char			cPassward[MD5_16_PASSWORD_SIZE];		// ����
	};

	BOOL Init();
	BOOL InitNetServer(void);			//���������������
	BOOL InitMemServer(void);			//��ʼ���ڴ�ӳ���ļ�,��GAPPͨ��
	BOOL InitSccPrama(CMessage* pMsg=NULL);				//��SCC���������ݳ�ʼ��
	BOOL LoadSccSetup(const char *filename);
	BOOL LoadSetup();
	BOOL MainLoop(void);
	BOOL ProcessMessage();
	BOOL Release();
	BOOL OnSCCClose();
	tagSCC* GetSCC(DWORD dwIndex){return (s_mapSCC.find(dwIndex)!=s_mapSCC.end())?&s_mapSCC[dwIndex]:NULL;}
	tagSCC* GetSCC(const char* strIP,const char* strPass);			// ��ȡGameServer������
	CGapp*	FindGapp(DWORD dwID){return (m_mapGappInfo.find(dwID) != m_mapGappInfo.end()) ? m_mapGappInfo[dwID] : NULL;}
	bool	IsSameProcID(DWORD dwProcID);//GAPP��ִ���ļ�����һ��,ö��ʱ����Ƿ���ͬһ����
	DWORD	GetGappNumByType(eGappType eType);
	bool 	UpdateGappByType(DWORD dwType,DBReadSet& dbreadset);
	unsigned	 FindGappThreadID(DWORD dwID);
	tagSetup*	 GetSetup()	{return &m_Setup;}
	tagScc2SmSetup*	GetSccPrama(){return &m_SCCPrama;}
	BOOL		 StartAccept();
	BOOL		 LoadGappInfo(const char* filename);
	BOOL		 InitMemUsageName();
	BOOL		 AddGapp(CGapp* pGapp);
	const char* GetMemUsageFileName()	{return m_pszMemUsageFile;}
	void SendGappInfo();
	void SendHardWareInfo();
	void DelayTest();
	BOOL CheckGappVersion(long lGappID = -1);
	long GetGappSize()	{return (long)m_mapGappInfo.size();}
	bool ConnScc();
	//�ṩ����������������
	BOOL ShutDown(long id);							//�ر�GAPP
	BOOL Start(long id);							//����GAPP
	BOOL ShowGapp();
	BOOL GetPackRes(long id);						//��SCC�ϻ�ȡGAPPԭʼ��
	void SendError(const char* Info,...);
private:
	long				m_lSocketID;				//����SCC
	map<DWORD,long>		m_mapVersion;
	map<long,CGapp*>	m_mapGappInfo;				//key  �����ļ��е�ID,����GAPPΨһ��ʶ
	map<long,tagSCC>	s_mapSCC;					//scc�б�,key ��SCC ID
	tagSetup			m_Setup;				//����
	CDataBlockAllocator	*m_pDBAllocator;
	CServer				*s_pNetServer;				
	// �Ƿ��������SCC
	bool					b_IsFirstConnectionSCC;
	//�ڴ��ļ���־����
	char m_pszMemUsageFile[MAX_PATH];
	//CPU�ļ���־����
	char m_pszLogFile[MAX_PATH];
	tagSCC*				m_CurrScc;				//��ǰ���ӵ�SCC
	map<DWORD,CGapp*>m_TimeOut;//GAPP��ʱ������
	GappTimer  m_GappTimer;
	list<tagSMTimerVar*> m_listTimerManager;
public:
	typedef	map<long,CGapp*>::iterator	GAPPITER;
	typedef map<long,tagSCC>::iterator  SCCITER;
	typedef deque<string>::iterator		CMDITER;	
	void RunCmdLine();
	GappTimer* GetGappTimer(void) { return &m_GappTimer;}
	tagSMTimerVar* GetTimerVar();
	void FreeTimerVar(tagSMTimerVar* p);
	void ReleaseTimerVarManager();
};


extern HANDLE g_hSMThreadExitEvent;		// ���߳��˳��¼�
extern bool	g_bSMThreadExit;				// �Ƿ��˳����߳�
BOOL CreateSM();
CSM* GetSM();
BOOL DeleteSM();
unsigned __stdcall		SMThreadFunc(void* pArguments);	// SM���߳�
unsigned __stdcall		StartGappThreadFunc(void* pArguments);//�Զ�����GAPP
