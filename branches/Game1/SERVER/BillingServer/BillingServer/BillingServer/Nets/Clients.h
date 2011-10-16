

#ifndef _CLIENT_H_
#define _CLIENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mysocket.h"
#include "MsgQueue.h"
#include "SocketCommands.h"

//const int MAX_MESSAGE_LENGTH = 10240;	// �����Ϣ����
const int CONNECT_TIMEOUT = 10000;		// ���ӵȴ�ʱ��

/*-----------------------------------------------------------
   [CClient] �ͻ����׽�����

  ����:
	 �������Ӻ���Ϣ����

  ��ʼ������:
	[TCP]
		CClient* pClient = new CClient;
		pClient->Create(dwPort, strIP, nSocketType);	// ����socket
		pClient->CreateSocketThread();					// ����SocketThread
		pClient->Connect(strServerIP, lServerPort);		// ���ӷ�����

  	[UDP]
		CClient* pClient = new CClient;
		pClient->Create(dwPort, strIP, nSocketType);	// ����socket
		pClient->CreateSocketThread();					// ����SocketThread

------------------------------------------------------------*/
class CMessage;
class CClient : public CMySocket  
{
protected:
	char m_strServerIP[64];				// ������IP
	long m_lServerPort;					// �������˿�

//	char m_pBuffer[MAX_MESSAGE_LENGTH];	// ���ܻ���

	char *m_pBuffer;					// ���ܻ���
	int m_nBufferSize;					// ��������С
	int m_nSize;						// �������е�ǰ���ݴ�С

	HANDLE m_hConnectEventSuccess;		// ���ӳɹ�
	bool m_bConnect;					// �Ƿ�����

	void* pFinishCallback;			//�첽���ӵĻص�����

public:
	
	HANDLE m_hSocketEvent[2];	// 0 ������Ϣ 1 ����SocketThread
	HANDLE m_hSocketThread;		// �����¼��Ľ���
	HANDLE m_hNetClientThread;	// �ͻ����������߳�

	BOOL CreateSocketThread();	// ���������̹߳���socket
	BOOL ExitSocketThread();	//�Ƴ�ClientSocket���߳�
	
	friend unsigned __stdcall SocketThread(void* pArguments);
	void DoSocketThread(CClient* pClient);
	friend unsigned __stdcall NetClientThreadFunc(void* pArguments);		//�ͻ����������߳� 
	friend void DoNetClientThreadFunc(CClient* pClient);					//�ͻ����������߳� 

	bool m_bSendData;					//��ʾ�ͻ����Ƿ���Է�����
	bool m_bSocketThreadExit;
	bool m_bNetClientThreadExit;

	CSocketCommands		m_ClientSocketOnSendCommands;		//�����ʾ�ɷ�����Ϣ��һ���������


protected:
	bool m_bControlSend;				//���Ʒ��͵�һ��ȫ�ֱ���
	CMsgQueue			m_RecvMessages;				//���յ�����Ϣ����

public:
	void	SetControlSend(bool bSend)	{m_bControlSend=bSend;}
	bool	GetControlSend()			{return m_bControlSend;}
	virtual bool IsSameSocketID(long lSocketID1,long lSocketID2)
	{
		return true;
	}

	virtual CSocketCommands& GetSocketCommand();

	CMsgQueue*	GetRecvMessages()	{return & m_RecvMessages;}

	// ����SOCKET
	virtual BOOL Create(UINT nSocketPort = 0,
		LPCTSTR lpszSocketAddress = NULL,				
		int nSocketType = SOCK_STREAM,	// TCP:SOCK_STREAM UDP:SOCK_DGRAM
		long lEvent = FD_READ|FD_WRITE|FD_CONNECT|FD_CLOSE,
		bool bAsyncSelect=true  //asyncselect flag,if create socket for client,set the flag=true
		);

	BOOL AsyncSelect(long lEvent = FD_READ|FD_WRITE|FD_ACCEPT|FD_CONNECT|FD_CLOSE);	// ���������¼�����Ӧ
	void SetServerPort(DWORD w)		{m_lServerPort = w;}
	long GetServerPort(void)		{return m_lServerPort;}
	void SetServerIP(const char* str)	{strcpy(m_strServerIP,str);}
	char* GetServerIP()				{return m_strServerIP;}

	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);	// [TCP]��������
	virtual int SendToServer(const void* lpBuf, int nBufLen, bool bFront,int nFlags = 0);//

	virtual BOOL Close();				// �ر�socket
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	// ��������ʹ�õĽӿ�
	virtual void HandleReceive() = 0;
	virtual void HandleConnect() = 0;
	virtual void HandleClose() = 0;

	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);	// ����
	BOOL ConnectServer(LPCTSTR lpszHostAddress, UINT nHostPort);//����
	bool IsConnect();		// �Ƿ�����
	long WaitForConnect();	// �ȴ�����

	CClient();
	virtual ~CClient();

//////////////////////////////////////////////////////
// Socks5����
//////////////////////////////////////////////////////
protected:
	enum 
	{
		PROXY_INVALID = 0,		// ������
		PROXY_VALID = 1,		// ����
	};

	bool m_bUseProxy;			// �Ƿ�ʹ�ô���
	char m_strProxyIP[64];		// ���������IP
	long m_lProxyPort;			// ����������˿�
	char m_strProxyUser[32];	// �û�
	char m_strProxyPass[32];	// ����

	long m_lConnectProxyStep;	// ��ʼ������Ĳ���
	bool m_bProxyIsValid;		// ����������Ƿ����

public:
	void EnableProxy(bool b)	{ m_bUseProxy = b; }
	bool IsUseProxy()			{ return m_bUseProxy; }
	bool IsProxyValid()			{ return m_bProxyIsValid; }

	void SetProxy(const char* ip, long port, const char* user=NULL, const char* pass=NULL);
	void ConnectProxy(long step);

public:
	struct sock5req1
	{
		char Ver;
		char nMethods;
		char Methods[255];
	};

	struct sock5ans1
	{
		char Ver;
		char Method;
	};

	struct sock5req2
	{
		char Ver;
		char Cmd;
		char Rsv;
		char Atyp;
		char other[1];
	};

	struct sock5ans2
	{
		char Ver;
		char Rep;
		char Rsv;
		char Atyp;
		char other[1];
	};

	struct authreq
	{
		char Ver;
		char Ulen;
		char Name[255];
		char PLen;
		char Pass[255];
	};

	struct authans
	{
		char Ver;
		char Status;
	};
//ͳ������
protected:
	bool				m_bCheck;					//�Ƿ���

	DWORD				m_dwSendStartTime;			//ͳ�ƿ�ʼʱ��
	long				m_lTotalSendSize;			//���͵�������
	long				m_lSendSizePerSecond;		//���뷢�͵�����
	LONG64				m_llSendSize;

	DWORD				m_dwRecvStartTime;			//ͳ�ƿ�ʼʱ��
	long				m_lTotalRecvSize;			//����������
	long				m_lRecvSizePerSecond;		//ÿ���������
	LONG64				m_llRecvSize;
public:

	bool	GetCheck()	{ return m_bCheck; }

	long	AddSendSize(long lSize);
	long	GetSendSizePerSecond()			{return m_lSendSizePerSecond;}
	LONG64  GetTotalSendSize()				{return m_llSendSize;}

	long	AddRecvSize(long lSize);
	long	GetRecvSizePerSecond()			{return m_lRecvSizePerSecond;}
	LONG64	GetTotalRecvSize()				{return m_llRecvSize;}
};

#endif
