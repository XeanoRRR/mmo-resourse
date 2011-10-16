#pragma once

class CServerForGS;
class CMessage;
class CBillingPlayerManager;

class CGame
{
public:
	CGame(void);
	~CGame(void);

public:
	bool Init();										// ��Ϸ��ʼ��
	bool InitServer();									// ��ʼ����Ϸ����������ˣ���������������ˣ�װ������
	bool InitBPManager();								// Create BillingPlayerManager threads
	bool ReleaseBPManager();							// Realse BillingPlayerManager threads
	bool Release();										// ��Ϸ�ͷ�
	bool ProcessMessage();								// ��Ϣ����
	bool MainLoop();									// ��Ϸ��ѭ��

public:
	// Setup.ini
	struct tagSetup
	{
		DWORD	dwGSListenPort;							// GS�����˿�
		//string	strSqlConType;							// sql server connection type
		string	strSqlServerIP;							// sql server ip address
		string  strSqlUserName;
		string  strSqlPassWord;
		string  strDBName;

		bool	bLogSvrSwitch;							// local log server switch
		string	strLogSvrIP;							// sql server ip address
		string  strLogSvrUsrName;
		string  strLogSvrPwd;
		string  strLogDBName;

		DWORD	dwRefeashInfoTime;						// ˢ����ʾʱ��
		DWORD	dwSaveInfoTime;							// ����һ����Ϣ�ļ��ʱ��
		DWORD	dwSaveLogSvrTime;						// Save logs for incrementshop

		DWORD	dwDBIOThreadNum;						// database i/o threads' num
	};

public:
	// GameServer��Ϣ
	CServerForGS		*GetServerForGS()				{ return m_pGSServer; }
	tagSetup			*GetSetup()						{ return &m_Setup; }
	bool				LoadSetup();
	bool				ReLoadSetup();
	bool				LoadGSSetup();
	vector<CBillingPlayerManager*> vecBPM;

private:
	tagSetup			m_Setup;
	CServerForGS		*m_pGSServer;					// ��ΪGS�ķ�������
	bool				m_bExit;						// �˳���Ϸ
};

extern HWND 			g_hWndInfo;
extern HWND 			g_hWndLog;
extern HWND 			g_hWndGSList;
extern HANDLE			g_hGameThreadExitEvent;			// ���߳��˳��¼�
extern bool				g_bGameThreadExit;				// �Ƿ��˳����߳�

bool CreateGame();										// ������Ϸ
CGame* GetGame();										// ��ȡ��Ϸ
bool DeleteGame();										// �ͷ���Ϸ
void RefreashInfo();									// ˢ�·�������Ϣ
void AddLogText(char* msg, ...);						// ����һ����¼
void SaveLogText(bool bForce);							// �����¼
void AddCdkeyList(const char *str);
void DelCdkeyList(const char *str);
unsigned __stdcall GameThreadFunc(void* pArguments);	// ��Ϸ���߳�