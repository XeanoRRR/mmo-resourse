#pragma once

#include "..\basemessage.h"
#include "..\..\public\MsgType.h"
#include "..\..\public\ManageToolMsgType.h"
#include <hash_map>
using namespace stdext;

class CServer;
class CMySocket;
class CMapFile;

class CMessage : public CBaseMessage
{
public:
	CMessage(void);
	CMessage(long lType);
	virtual ~CMessage(void);

private:
	static CServer *m_pClientServer;
	static long m_lServerSocket;
	static long m_lLogSocket;

public:
	// ��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator, long nMaxFreeMsgNum);
	// �ͷ���Դ
	static bool Release();

	static void SetServer(CServer* pServer)		{ m_pClientServer = pServer; }
	static void SetServerSocket(long lSocket)	{ m_lServerSocket = lSocket; }
	static void SetLogSocket(long lSocket)		{ m_lLogSocket = lSocket; }

	static CBaseMessage* NewMessage();

protected:
	long  m_lSocketID;			// ������Ϣ������SocketID
	DWORD m_dwIP;				// ���͸���Ϣ��IP

public:
	void SetSocketID(long lSocketID)	{ m_lSocketID = lSocketID; }
	long GetSocketID()					{ return m_lSocketID; }

	void SetIP(DWORD dwIP)			{ m_dwIP = dwIP; }
	DWORD GetIP()					{ return m_dwIP; }

	void Reset();			// ������Ϣ��ر���

	long GetDSID();
	long GetLSID();
	long GetWSID();
	long GetGSID();

	long SendToSocket(long lSocketID, bool bClone=true);
	
	long SendAll();
	long DefaultRun();
	virtual long Run();

	long Send(bool bFront=false);
	long SendToSM();

//////////////////////////////////////////////////////////////////////////
//	ʱ���
//////////////////////////////////////////////////////////////////////////
protected:
	DWORD	m_dwRecvTime;			// �յ���ʱ��

public:
	DWORD GetRecvTime()					{ return m_dwRecvTime; }
	void SetRecvTime(DWORD dwRecvTime)	{ m_dwRecvTime = dwRecvTime; }

private:
	typedef stdext::hash_map<long, long> HMapLongLong;
	typedef stdext::hash_map<long, long>::iterator itLL;

	static HMapLongLong m_SocketClientID;
	static HMapLongLong m_ClientIDSocket;

public:
	static void MapGSIDSocket(long lGSID, long lSocketID);
	static long GetSocketByGSID(long lGSID);
	static long GetGSIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID, long lNetFlag);
	static void UnMapGSIDSocket(long lSocketID);

//���ڴ�Ӱ���ļ��Ľ���ͨ�Ż���,ʵ��д�ļ����첽
public:
	//���ļ�
	static void AsyOpenFile(const char* pszFileName);
	//�ر��ļ�
	static void AsyCloseFile(const char* pszFileName);
	//����ļ�����
	static void AsyClearFile(const char* pszFileName);
	//�첽д�ļ��Ľӿ�
	static void AsyWriteFile(const char* pszFileName,const char* pszContent);

};
