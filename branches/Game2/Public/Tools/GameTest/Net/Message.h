//=============================================================================
/**
 *  file: Message.h
 *
 *  Brief:��Ϣ�࣬��װ�˹�����Ϣ��������� ��������Ϣ�Ĺ���
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================
#pragma once
#include "..\App\ClientRegion.h"

//

class CMySocket;
class CServer;
class CPlayer;

class CMessage :
	public CBaseMessage
{
public:
	CMessage(void);
	CMessage(long lType);
	virtual ~CMessage(void);

private:
	static CServer	*m_pServer;
	static long	m_lWSSocket;		//����worldserver�Ŀͻ���
	//[Amend AHC 070719]
	static long	m_lDBSSocket;		//����DBserver�Ŀͻ���
	static long m_lLogSocket;		//������־�������Ŀͻ���

	//����һ����Ϣ
	static CMessage* AllocMessage();

	//MappingFile ������
	static CMemServer *m_pMemServerManager;
public:
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	static bool Release();

	static void SetServer(CServer *pServer)	{m_pServer=pServer;}
	static void SetWSSocket(long lSocket){m_lWSSocket=lSocket;}
	static void SetDBSSocket(long lSocket){m_lDBSSocket=lSocket;}
	static void SetLogSocket(long lSocket){m_lLogSocket=lSocket;}
	static void SetMemServerManager(CMemServer *pManager){ m_pMemServerManager = pManager; }
	static CBaseMessage* NewMessage();
	////////////////////////////////////////////////////////////////////////
//	Player & Region
////////////////////////////////////////////////////////////////////////
private:
	//CClientRegion* m_pRegion;
	//CPlayer* m_pPlayer;

public:
	//CClientRegion* GetRegion()		{return m_pRegion;}
	//CPlayer* GetPlayer()			{return m_pPlayer;}
	//void SetPlayer(CPlayer* p)		{m_pPlayer=p;}

	void Reset(void);        //������Ϣ��ر���

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

// ������
private:
	long	m_lSocketID;			//������Ϣ������SocketID;
	DWORD	m_dwIP;					//���͸���Ϣ��IP
	CGUID	m_guDiscardFlag;		//��Ϣ�Ķ�����ʾ

	
public:
	void SetSocketID(long lSocketID)	{ m_lSocketID=lSocketID;}
	long GetSocketID()					{return m_lSocketID;}

	void SetIP(DWORD dwIP)				{m_dwIP = dwIP;	}
	DWORD GetIP()						{return m_dwIP;	}

	const CGUID& GetPlayerID();


//	void Reset(void);

	long SendBySocket(long lSocketID,bool bClone=true);
//	long SendByPlayer(long lPlayerID);
	long SendByPlayer(const CGUID& lPlayerID);
	//bool SendToClient(unsigned uID);			//on �߳�ID
	long DefaultRun();
	long Run();					// ����������Ϣ����
private:
	//WorldServer�����׽��ֺ�WorldServerID����
	typedef stdext::hash_map<long,CGUID>	HMapLongGUID;
	//typedef stdext::hash_map<long,long>::iterator itLL;
	typedef stdext::hash_map<long,CGUID>::iterator itLG;

	typedef stdext::hash_map<CGUID,long,hash_guid_compare>	HMapGUIDLong;
	typedef stdext::hash_map<CGUID,long,hash_guid_compare>::iterator itGL;

	static HMapLongGUID	m_SocketPlayerID;
	static HMapGUIDLong	m_PlayerIDSocket;

public:
	//ӳ��WorldID��SocketID
	//static void MapPlayerIDSocket(long lPlayerID,long lSocketID);
	//static long GetSocketByPlayerID(long lPlayerID);
	static void MapPlayerIDSocket(const CGUID& lPlayerID,long lSocketID);
	static long GetSocketByPlayerID(const CGUID& lPlayerID);
	static const CGUID& GetPlayerIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID,long lNetFlag);
	static void UnMapPlayerIDSocket(long lSocketID);
};
