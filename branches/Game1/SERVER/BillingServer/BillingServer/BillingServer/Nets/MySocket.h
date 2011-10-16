#ifndef _MYSOCKET_H_
#define _MYSOCKET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <iostream>
#include <vector>
#include "winsock2.h"

using namespace std;

const int MAX_MESSAGE_LENGTH = 1048576;

//���׽��ֵĲ�������
enum SocketOperType
{	
	SOT_Send,			//����
	SOT_Receive,		//����
	SOT_Accept,			//����
};

//IO�����ṹ:��С:44BYTE
typedef struct PER_IO_OPERATION_DATA
{
	OVERLAPPED	OverLapped;
	WSABUF		DataBuf;

	CHAR*		pBuffer;
	int			nBufSize;

	SOCKET		hSocket;			// �������׽���
	SocketOperType	OperationType;

}*LPER_IO_OPERATION_DATA;

/*-----------------------------------------------------------
 [CMySocket] WinSock����
 ���಻ֱ��ʹ�ã�һ��������ʹ�á�
 
 ���ܣ�
	* ����Socket
	* �󶨶˿ں͵�ַ
	* ���������¼�����Ӧ
	* ��������
	* ��������
	* �ر�socket
------------------------------------------------------------*/
class CMessage;
class CMySocket
{
public:
	static BOOL MySocketInit();		// ��������ʱ���ã���ʼ��Winsock
	static void MySocketClearUp();	// �����˳�ʱ����

	CMySocket();
	virtual ~CMySocket();

//////////////////////////////////////////////////////////////////////////
// ������Ϣ
//////////////////////////////////////////////////////////////////////////
protected:
	long m_lProtocol;	// ��ǰЭ��	TCP:SOCK_STREAM UDP:SOCK_DGRAM
	DWORD m_dwPort;		// socket�˿�
	char m_strIP[32];	// socket IP

	DWORD m_dwIP;		// IP(4byte)
	bool m_bClose;		// socket�Ƿ�ر�

	long m_lIndexID;	//����Socket�����ID

public:
	void SetIndexID(long lID)	{m_lIndexID=lID;}
	long GetIndexID()			{return m_lIndexID;	}

	long GetProtocol()			{return m_lProtocol;}
	void SetProtocol(long l)	{m_lProtocol=l;}

	void SetPort(DWORD w)		{m_dwPort = w;}
	DWORD GetPort(void)			{return m_dwPort;}
	void SetIP(const char* str)	{strcpy(m_strIP,str);}
	char* GetIP()				{return m_strIP;}
	void SetDwordIP(DWORD ip)	{m_dwIP = ip;}
	DWORD GetDwordIP()			{return m_dwIP;}
	bool IsClose()				{return m_bClose;}
	void SetClose(bool b)		{m_bClose = b;}

//////////////////////////////////////////////////////////////////////////
// [UDP] ������һ����Ϣ����Ϣ
//////////////////////////////////////////////////////////////////////////
protected:
	DWORD m_dwLastRecvPort;		// [UDP] ��һ���յ�����Ϣ�Ķ˿�
	char m_strLastRecvIP[32];	// [UDP] ��һ���յ�����Ϣ��IP

public:
	void SetLastRecvPort(DWORD w)		{m_dwLastRecvPort = w;}
	DWORD GetLastRecvPort(void)			{return m_dwLastRecvPort;}
	void SetLastRecvIP(const char* str)	{strcpy(m_strLastRecvIP,str);}
	char* GetLastRecvIP()				{return m_strLastRecvIP;}

//////////////////////////////////////////////////////////////////////////
// �ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	// ����SOCKET
	virtual BOOL Create(UINT nSocketPort = 0,
				LPCTSTR lpszSocketAddress = NULL,				
				int nSocketType = SOCK_STREAM,	// TCP:SOCK_STREAM UDP:SOCK_DGRAM
				long lEvent = FD_READ|FD_WRITE|FD_CONNECT|FD_CLOSE,
				bool bAsyncSelect=true  //asyncselect flag,if create socket for client,set the flag=true
				);

	//���� overlapped socket
	BOOL WSACreate(UINT nSocketPort,LPCTSTR lpszSocketAddress,int nSocketType=SOCK_STREAM);

	// winsock2Ĭ�� reuseaddr = 1 dontlinger = 1(linger = 0)
	BOOL Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL, int reuseaddr=1, int dontlinger=1);	// �󶨶˿ں͵�ַ
	
	virtual BOOL Close();	// �ر�socket���ȵȴ�SocketThread����
	
	virtual int Recv(void* lpBuf, int nBufLen, int nFlags = 0);			// [TCP]��������
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);	// [TCP]��������
	virtual int RecvFrom(void* lpBuf, int nBufLen, char* strIP, DWORD& dwPort, int nFlags = 0);		// [UDP]��������
	virtual int Sendto(const void* lpBuf, int nBufLen, const char* strIP, DWORD dwPort, int nFlags = 0);	// [UDP]��������

public:
	virtual void OnReceive(int nErrorCode)	{};
	virtual void OnSend(int nErrorCode)		{};
	virtual void OnOutOfBandData(int nErrorCode)	{};
	virtual void OnAccept(int nErrorCode)	{};
	virtual void OnConnect(int nErrorCode)	{};
	virtual void OnClose(int nErrorCode=0);	// �ر�socket (�յ�FD_CLOSE)

	virtual void SetSendRevBuf(){}

//////////////////////////////////////////////////////////////////////////
// ���ߺ���
//////////////////////////////////////////////////////////////////////////
public:
	BOOL GetPeerName(char * rPeerAddress, UINT& rPeerPort);		// ��ȡ���׽��������ĵ�ַ
	BOOL GetSockName(char * rSocketAddress, UINT& rSocketPort);	// ��ȡ���ص�ַ
	BOOL IOCtl(long lCommand=FIONREAD, DWORD* lpArgument=0);	// ��ȡ���л��������ݵĴ�С
	
//////////////////////////////////////////////////////////////////////////
// SocketThread
//////////////////////////////////////////////////////////////////////////
public:
	SOCKET m_hSocket;			// �׽���

#ifdef _DEBUG
#ifdef _CLIENT
//##��־���
	ofstream		ofLogFile;
	BOOL			bIsLogFileOpened;

	VOID			Log					( CMessage* pMsg, BOOL bIsSendMessage = FALSE );
#endif
#endif

	friend class	CMessage;

};

//�õ�һ��ID�����Socket
long GetSocketID();

unsigned long STR2UL(const char *szIP);

#endif
