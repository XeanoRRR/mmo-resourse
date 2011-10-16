#ifndef _SERVER_H_
#define _SERVER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySocket.h"
#include "MsgQueue.h"
#include ".\socketcommands.h"

#define CP_TIMEOUT	200

class CServerClient;
const int MAX_CLIENTS = 2048;

typedef map<long,CServerClient*> SocketIDClient;

// client info
struct AllowedClient
{
	char			_ip[32];
	unsigned short	_port;
	unsigned long	_sock;
};

class CServer : public CMySocket
{
protected:
	long m_lClientSocketCount;							// �����˷��������׽�������

	SocketIDClient				 m_Clients;				//���ӵ��������Ŀͻ����׽����б�

	map<long, AllowedClient *>	 m_MapIDSocketID;		//���ID��Ӧ���׽����б�<_sock, AllowedClient>
	map<string,long>			 m_MapStrSocketID;	    //���Str��Ӧ���׽���

	vector<HANDLE> m_hWorkerThreads;			//�������߳̾������
	HANDLE m_hNetMainTheads;					//�������������߳�
	HANDLE m_hAcceptThread;						//acceptThread

	bool m_bNetMainThreadExit;					//�Ƿ����߳��˳�
	bool m_bAcceptThreadExit;					//�Ƿ����AcceptThread
	bool m_bWorkerThreadsExit;					//�Ƿ�����������߳�

	DWORD m_dwAcceptThreadSleepTime;			//�����߳�˯��ʱ��
	DWORD m_dwNetMainThreadSleepTime;			//���߳�˯��ʱ��

	bool AddAClient(long lSocketID,CServerClient* pClient);	// ���һ���ͻ���
	CServerClient* FindOneClient(long lSocketID);			// ����һ���ͻ���
	bool DelOneClient(long lSocketID);						// ɾ��һ���ͻ���
	bool						_cl_available;				// clients list is available

public:
	 HANDLE m_hCompletionPort;   //completion port

public:
	bool GetClientCheck() { return _cl_available; }
	bool LoadAllowedClient( const char *filename );
	long CheckAllowedClient( const struct sockaddr_in &addr );
	AllowedClient *GetClientByID( unsigned long socket );
	long GetClientSocketCount()	{ return m_lClientSocketCount;}
	HANDLE* GetWorkerThread()	{ return &m_hWorkerThreads[0];}
	HANDLE GetAcceptThread()	{ return m_hAcceptThread; }
	BOOL Host(UINT dwPort, const char* strIP,DWORD nSocketType=SOCK_STREAM,bool bUseCP=false);	// ����Server���׽��֣��������״̬���ȴ��ͻ��˵�����
	BOOL Listen(int nConnectionBacklog=SOMAXCONN);			// listen
	virtual int SendBySocketID(long lSocketID, void *buffer, unsigned long size);			// ��ĳ��SocketID��������
	virtual int SendByMapID(long lMapID, void *buffer, unsigned long size);					// ��ĳ��MapID��������
	virtual int SendByMapStr(const char* strMap, void *buffer, unsigned long size);			//��ĳ��MapStr��������
	int SendAll(void* buffer,unsigned long size);						// ���͹㲥
	int Sendto(void *buffer, unsigned long size, const char* strIP, DWORD dwPort);	// [UDP] ��ĳһ��ip������Ϣ
	int SendtoAll(void* buffer,unsigned long size)	{};	// [UDP] ���͹㲥

	int QuitClientBySocketID(long lSocketID);								// �ϵ�ĳ���׽���
	int QuitClientByMapID(long lMapID);										// �ϵ�ĳ��ӳ���ID�Ŀͻ���
	int QuitClientByMapStr(const char* strMap);								// �ϵ�ĳ��ӳ���ID�Ŀͻ���

	int OnQuitOneClient(long lSocketID);									//
	int OnQuitAllClient();

	int SetClientMapID(long lSocketID,long lMapID = 0);						//���ÿͻ����׽��ֵ�PlayerID
	long GetSocketIDByMapID(long lMapID);									//����MapID�õ�SocketID

	int SetClientMapStr(long lSocketID,const char* strMap);					//���ÿͻ����׽��ֵ�PlayerID
	int GetSocketIDByMapStr(const char* strMap);							//���ÿͻ����׽��ֵ�PlayerID

	AllowedClient *IsExistMapID(long lID);										//�Ƿ���ڶ�ӦMapID�Ŀͻ���
	bool IsExistMapStr(const char* strMap);									//�Ƿ���ڶ�ӦMapstr�Ŀͻ���

	void Exit();															//�˳�������׽���
	void ExitWorkerThread(void);// {m_bWorkerThreadsExit = true;}			//�����˳���ɶ˿ڹ������̱߳�־
	
	virtual void OnClose(int nErrorCode);	// [TCP] Close��ʱ������
	virtual void OnError(int errorCode);	// error��ʱ������
	virtual void OnReceive(int nErrorCode);	// [UDP] �ڽ��յ���Ϣʱ����
	virtual void OnAccept(int nErrorCode); //SERVER����ɶ˿�ģʽ��
	virtual CServerClient* CreateServerClient();

	virtual void OnMapIDError(long lID)	{};
	virtual void OnMapStrError(const char* strMap){};

	BOOL CreateNetMainThread();					//�����������߳�
	BOOL CreateWorkerThreads(int nProcNum);		//��������ɶ˿��ϵȴ��Ĺ������߳�
	BOOL CreateAcceptThread();					//�������տͻ�Socket���߳�

	CServer();
	virtual ~CServer();

	friend unsigned __stdcall NetThreadFunc(void* pArguments);
	friend void DoNetThreadFunc(CServer* pServer);
	friend unsigned __stdcall WorkerThreadFunc(void* pArguments);
	friend void DoWorkerThreadFunc(CServer* pServer);
	friend unsigned __stdcall AcceptThreadFunc(void* pArguments);


//
private:
	CSocketCommands		m_SocketOperaCommands;		//������������������
	CMsgQueue			m_RecvMessages;				//���յ�����Ϣ����

public:
	CMsgQueue* GetRecvMessages()	{return &m_RecvMessages;}

//ͳ�ƿͻ��˵���������
protected:
	bool				m_bCheck;					//�Ƿ���ͻ���
	bool				m_bCheckMsgCon;				//�Ƿ�����Ϣ����
	long				m_lSizePerSecond;			//�ͻ��˵�����������׼
	long				m_lForbidTime;				//����IP��ʱ��
	long				m_lMaxMessageLength;		//��Ϣ������󳤶�

	DWORD				m_dwStartTime;				//ͳ�ƿ�ʼʱ��
	long				m_lTotalSendSize;			//���͵�������
	long				m_lSendSizePerSecond;		//���뷢�͵�����
	LONG64				m_llSendSize;

	DWORD				m_dwRecvStartTime;			//ͳ�ƿ�ʼʱ��
	long				m_lTotalRecvSize;			//����������
	long				m_lRecvSizePerSecond;		//ÿ���������
	LONG64				m_llRecvSize;

	long				m_lMaxClientConNum;			//���Ŀͻ�����������
	long				m_lMaxIPOperatorNum;		//��ÿ���ͻ������ķ��Ͳ�������
	long				m_lPermitMaxClientSendBufSize;//���Ŀͻ��˷��ͻ�����
	map<u_long,DWORD>	m_ForbidIPs;				//����IP�б�
	long				m_lMaxBlockConnetNum;
	long				m_lSendInterTime;
	

	map<DWORD,DWORD>	g_NewAcceptSockets;
public:

	bool	GetCheck()	{ return m_bCheck; }
	bool	GetCheckMsgCon()	{return m_bCheckMsgCon;	}
	long	GetSizePerSecond(){ return m_lSizePerSecond; }
	long	GetMaxMsgLen()		{ return m_lMaxMessageLength; }

	void	SetPraram(bool bCheck,bool bCheckCon,long lSizePerSecond,long lForbidTime,long lMaxMsgLen,
		long lMaxConNums,long lSendIONums,long lMaxClientSendBufSize)
	{
		m_bCheck = bCheck;m_bCheckMsgCon=bCheckCon;
		m_lSizePerSecond = lSizePerSecond;
		m_lForbidTime = lForbidTime;m_lMaxMessageLength=lMaxMsgLen;
		m_lMaxClientConNum = lMaxConNums;m_lMaxIPOperatorNum = lSendIONums;
		m_lPermitMaxClientSendBufSize = lMaxClientSendBufSize;
	}

	void	AddForbidIP(u_long laddr);
	bool	FindForbidIP(u_long laddr);


	long	AddPackageSize(long lSize);
	long	GetSendSizePerSecond()			{return m_lSendSizePerSecond;}
	LONG64  GetTotalSendSize()				{return m_llSendSize;}

	long	AddRecvSize(long lSize);
	long	GetRecvSizePerSecond()			{return m_lRecvSizePerSecond;}
	LONG64	GetTotalRecvSize()				{return m_llRecvSize;}

	long	GetMaxConNum()				{return m_lMaxClientConNum;}
	long	GetMaxIOOperNum()			{return m_lMaxIPOperatorNum;}

	long	GetMaxClientSendBufSize()	{return m_lPermitMaxClientSendBufSize;}

	void	SetParamEx(long lBlockConNum,long lSendInterTime)
	{m_lMaxBlockConnetNum=lBlockConNum;m_lSendInterTime=lSendInterTime;}

	void	AddNewAcceptSocket(DWORD dwSocketID);
	void	RemoveNewAcceptSocket(DWORD dwSocketID);
	void	DoNewAcceptSocket();
};

#endif