#ifndef _SERVERCLIENT_H_
#define _SERVERCLIENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <vector>
using namespace std;

#include "Servers.h"

//const int MAX_MESSAGE_LENGTH = 10240;

/*-------------------------------------------------------------------
 [CServerClient] ��������Ӧÿ���ͻ��˵�����

 ���ܣ�
	������TCPЭ�飬UDPЭ�鲻��Ҫ���ࡣ
    ����һ���ͻ��˵��������󵽴�󣬷���������һ��ServerClient��֮����
 ���ฺ��;���ͻ���ͨѶ
--------------------------------------------------------------------*/



class CServerClient : public CMySocket
{
protected:
//	char m_pBuffer[MAX_MESSAGE_LENGTH];	// ������Ϣ�Ļ���

	char *m_pBuffer;					// ���ܻ���
	int m_nBufferSize;					// ��������С
	int m_nSize;						// �������е�ǰ���ݴ�С

	char *m_pSendBuffer;				// ���ͻ�����
	long m_lMaxSendBuffSize;			// ����ͻ�����
	long m_lSendBufferSize;				// ���ͻ�������С
	long m_lCurSendBufferSize;			// ��ǰ��������С
	long m_lSendSize;					// ÿ�η��͵����ݴ�С
	
	bool m_bLost;						// �Ƿ�����˵ı�־
	bool m_bQuit;						// �׽����Ƿ��˳�

	long m_bServerType;                 //�ͻ������� GameServer Or LogingServer
	long m_lMapID;						//����Clientӳ����ⲿID
	string m_strMap;					//����Clientӳ����ⲿstring

	CServer*		m_pServers;			//���Ƶĸ��׽���


	bool	m_bCloseFlag;				//�Ƿ���ɾ�����

public:
	bool IsLost()						{return m_bLost;}
	void SetLost(bool b)				{m_bLost = b;}
	bool IsQuit()						{return m_bQuit;}
	void SetQuit(bool b)				{m_bQuit = b;}

	long GetServerType()                { return m_bServerType;}            //��ÿͻ��˷���������
	void SetServerType(long lservertype){m_bServerType = lservertype;}      //���ÿͻ��˷���������

	CServer*	GetServer()				{return m_pServers;	}

	void SetMapID(long lID)				{m_lMapID=lID;}
	long GetMapID()						{return m_lMapID;}

	void SetMapStr(const char* str)		{m_strMap=str;}
	const char* GetMapStr()				{return m_strMap.c_str();}

	virtual void SetSendRevBuf();

	void	SetCloseFlag(bool b)		{m_bCloseFlag = b;}
	bool	GetCloseFlag()				{return m_bCloseFlag;}

public:

	CServerClient(CServer*	pServers);
	virtual ~CServerClient();

	virtual void AddReceiveData(CHAR* pBuf,int nBufSize);
	virtual bool AddSendData(void* pBuf,int nBuffSize);
	long	GetCurSendBuffSize()		{return m_lCurSendBufferSize;}
	long	GetSendBuffSize()			{return m_lSendBufferSize;}

	virtual void OnAccept(int errorCode=0)	{};	// ���ӳɹ�ʱ����
	virtual	void OnReceive(int nErrorCode=0);// ��ɶ˿����ܵ���Ϣʱ����
	virtual void OnClose(int errorCode=0)	{};	// ��Է��Ͽ���ʱ�򼤻�
	virtual void OnSend(int nErrorCode=0)	{};	// ����Ϣ���͵�ʱ�򼤻�

	virtual void OnOneMessageSizeOver(long lSize,long lPermitSize){};
	virtual void OnTotalMessageSizeOver(long lSize,long lPermitSize){};

	bool ReadFromCompletionPort(PER_IO_OPERATION_DATA* pIOData);  //����ɶ˿ڶ�ȡ����

	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);	// [TCP]��������

	friend unsigned __stdcall NetThreadFunc(void* pArguments);

//ͳ�ƿͻ��˵���������
private:
	DWORD	m_dwStartTime;						//ͳ�ƿ�ʼʱ��
	long	m_lTotalSize;						//���ܰ����ܴ�С
	long	m_lSizePerSecond;					//ÿ����ܵ����ݴ�С

	//ͳ�Ʒ���
	DWORD	m_dwSendStartTime;					//����ͳ�ƿ�ʼʱ��
	long	m_lTotalSendSize;					//�ܷ�������
	long	m_lIOOperatorNum;					//��ǰ�ص����͵ĸ���
public:
	long	AddPackageSize(long lSize);
	long	GetCurMsgLength()					//�õ���ǰ��Ϣ�ĳ���
	{
		if(m_nSize >= sizeof(long) )
		{
			return *((long*)(&m_pBuffer[0]));
		}
		return 0;
	}

	long AddSendPackageSize(long lSize)
	{
		m_lTotalSendSize = m_lTotalSendSize+lSize;
	}

	long GetSendSizePerSecond();

	long GetIOOperatorNum()	{return m_lIOOperatorNum;}
	void DecIoOperatorNum()	{m_lIOOperatorNum--;}
};

#endif
