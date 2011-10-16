#pragma once

#include "..\Net\AinMessage.h"

class CServer;
class CDataBlockAllocator;
struct tagTempConnSetup;

class ServiceNet
{
public:
	ServiceNet(VOID);
	~ServiceNet(VOID);

public:
	static		ServiceNet&			GetInstance(VOID);

public:
	//!								��ʼ��
	BOOL							Init(VOID);
	//!
	VOID							ProcessMessage(VOID);
	//!
	VOID							Release(VOID);

	//!
	AinMsgQueue&					GetMsgQueue(VOID) {return m_AinMsgQueue;}

public:
	/*
	//!								��������SM
	VOID							ConnectAllSM(VOID);
	//!								����һ��SM
	VOID							ConnectSM(DWORD dwSMID);
	//!								��ӦSM���ӳɹ�
	VOID							OnConnectSM(VOID);
	*/

private:
	//!								��ʼ���������
	BOOL							InitNetServer_SUC(VOID);
	BOOL							InitNetServer_SM(VOID);
	/* ��������SM�Ľӿ�
	//!								����һ��������
	BOOL							ConnectSM(tagTempConnSetup &TempConnSetup);
	//!								���һ��SM���õ��첽����
	VOID							AddToAsynConnectSM(tagTempConnSetup &TempConnSetup);

private:
	//!
	static	unsigned	__stdcall	ConnectSMFunc( void* pThis );
	*/

private:
	//!								���ݿ������
	CDataBlockAllocator				*m_pDBAllocator;
	//!								������ܿͻ�����Ϣ���������
	CServer							*s_pNetServer_SUC;
	//!								������ܿͻ�����Ϣ���������
	CServer							*s_pNetServer_SM;
	//!								��ΪSM�Ŀͻ���
	set<LONG>						m_mapMsSocket;

	//!
	AinMsgQueue						m_AinMsgQueue;

	

private:
	struct tagSocketAndID 
	{
		DWORD	dwID;
		LONG	lSocket;
		tagSocketAndID(DWORD dwInID, LONG lInSocket)
			:dwID(dwInID), lSocket(lInSocket)
		{}
	};
	

	/* ��������SM������
	//!
	BOOL							m_bIsWorking;
	//!					
	LockOfWin32						m_ConnLock;
	//!								û�����ϵ�SM
	list<tagTempConnSetup>			m_listLostSM;
	//!								��������SM���߳�
	HANDLE							m_hConnThrd;
	//!								���������̱߳��
	BOOL							m_bEndConnThrd;

	//!								��Ӧ���ӳɹ�����
	LockOfWin32						m_OnConnLock;
	//!								���ӳɹ���Socket
	list<tagSocketAndID>			m_listSocket;
	*/

private:
	static	ServiceNet				c_ServiceNet;
};