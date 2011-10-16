#pragma once

#include "..\basemessage.h"
#include "..\public\MsgType.h"
#include "..\..\public\ManageToolMsgType.h" 
#include <hash_map>
using namespace stdext;

class CMySocket;
class CServer;
class CMapFile;

class CMessage :
	public CBaseMessage
{
public:
	CMessage();
	CMessage(long lType);
	virtual ~CMessage(void);

private:
	static CServer	*m_pClientServer;
	static long	m_lLogingSocket;		//����loginserver�Ŀͻ���
	static long	m_lCostSocket;			//����costserver�Ŀͻ���
public:
	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	//�ͷ���Դ
	static bool Release();

	static void SetServer(CServer *pServer)	{m_pClientServer=pServer;}
	static void SetLogingSocket(long lSocket){m_lLogingSocket=lSocket;}
	static void SetCostSocket(long lSocket){m_lCostSocket=lSocket;}

	static CBaseMessage* NewMessage();
////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////
	// ������
protected:
	long	m_lSocketID;			//������Ϣ������SocketID;
	DWORD	m_dwIP;					//���͸���Ϣ��IP

//	static long	m_lWSSocket;
//	static long	m_lGSSocket;

public:
	void SetSocketID(long lSocketID)	{ m_lSocketID=lSocketID;}
	long GetSocketID()					{return m_lSocketID;}

	void SetIP(DWORD dwIP)				{m_dwIP = dwIP;	}
	DWORD GetIP()						{return m_dwIP;	}

	void Reset(void);        //������Ϣ��ر���

	long GetGSID();
	long GetWSID();

//	static long GetGSSocketID() { return m_lGSSocket; };
//	static long GetWSSocketID() { return m_lWSSocket; };

	long SendToSocket(long lSocketID);	//���͵�Gameserver��Socket
	long SendToWorld(long lSocketID);		//����һ����Ϣ��WorldServer�׽���
	long SendGSID(long lGSID);				//���͵�Gameserver��һ��ӳ��ID
	long SendWSID(long lWSID);				//���͵�WS��һ��ӳ��ID

	long SendAll();						// ���͸�����GS
	long SendAllWS();                   // ���͸�����WS
	long DefaultRun();
	virtual long Run();					// ����������Ϣ����


	long Send(bool bFront=false);		// ������Ϣ
	void SendToCost(bool bFront=false);
	long SendToSM();
//////////////////////////////////////////////////////////////////////////
//	ʱ���
//////////////////////////////////////////////////////////////////////////
protected:
	DWORD	m_dwRecvTime;			// �յ���ʱ��

public:
	DWORD	GetRecvTime()			{return m_dwRecvTime;}
	void	SetRecvTime(DWORD dw)	{m_dwRecvTime=dw;}

private:
	//dbserver�����׽��ֺ�dbserverID����
	typedef stdext::hash_map<long,long>	HMapLongLong;
	typedef stdext::hash_map<long,long>::iterator itLL;

	static HMapLongLong	m_SocketGSID;
	static HMapLongLong	m_GSIDSocket;

	static HMapLongLong	m_SocketWSID;
	static HMapLongLong	m_WSIDSocket;

public:
	//ӳ��GSID��SocketID
	static void MapGSIDSocket(long lGSID,long lSocketID);
	static long GetSocketByGSID(long lGSID);
	static long GetGSIDBySocket(long lSocketID);

	static void UnMapSocketGS(long lSocketID,long lNetFlag);
	static void UnMapGSIDSocket(long lSocketID);

	//ӳ��WSID��SocketID
	static void MapWSIDSocket(long lWSID,long lSocketID);
	static long GetSocketByWSID(long lWSID);
	static long GetWSIDBySocket(long lSocketID);

	static void UnMapSocketWS(long lSocketID,long lNetFlag);
	static void UnMapWSIDSocket(long lSocketID);

	//���ڴ�Ӱ���ļ��Ľ���ͨ�Ż���,ʵ��д�ļ����첽
	//Author:wangqiao
public:
	//���ļ�
	static void AsyOpenFile(const char* pszFileName);
	//�ر��ļ�
	static void AsyCloseFile(const char* pszFileName);
	//����ļ�����
	static void AsyClearFile(const char* pszFileName);
	//�첽д�ļ��Ľӿ�
	static void AsyWriteFile(const char* pszFileName,const char* pszContent);
private:
	//! ���͵�SM����Ϣ˳��
	static long c_SmInfoOrder;
};

