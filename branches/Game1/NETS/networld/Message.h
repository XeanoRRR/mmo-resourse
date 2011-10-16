#pragma once

#include "..\basemessage.h"
#include "../../public\MsgType.h"
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
	//[Amend AHC 070719]
	static long	m_lDBSocket;		//����DBserver�Ŀͻ���
	static long m_lLogSocket;		//������־�������Ŀͻ���
public:
	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	//�ͷ���Դ
	static bool Release();

	static void SetServer(CServer *pServer)	{m_pClientServer=pServer;}
	static void SetLogingSocket(long lSocket){m_lLogingSocket=lSocket;}
	static void SetCostSocket(long lSocket){m_lCostSocket=lSocket;}
	static void SetDBSocket(long lSocket){m_lDBSocket=lSocket;}
	static void SetLogSocket(long lSocket){m_lLogSocket=lSocket;}

	static CBaseMessage* NewMessage();
////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////
	// ������
protected:
	long	m_lSocketID;			//������Ϣ������SocketID;
	DWORD	m_dwIP;					//���͸���Ϣ��IP

public:
	void SetSocketID(long lSocketID)	{ m_lSocketID=lSocketID;}
	long GetSocketID()					{return m_lSocketID;}

	void SetIP(DWORD dwIP)				{m_dwIP = dwIP;	}
	DWORD GetIP()						{return m_dwIP;	}

	void Reset(void);        //������Ϣ��ر���

	long GetGSID();
	long GetDBSID();
	long GetLOGSID();
	long GetLSID();

	long SendToSocket(long lSocketID,bool bClone=true);	//���͵�GS,DBS��Socket
	long SendGSID(long lGSID,bool bClone=true);		//���͵�GS,DBS��һ��ӳ��ID


	long SendAll();						// ���͸����пͻ���
	long DefaultRun();
	virtual long Run();					// ����������Ϣ����


	long Send(bool bFront=false);		// ������Ϣ
	void SendToCost(bool bFront=false);
	long SendToSM();
	//ͬ�����͵�SM,���ܶ�ʧ,����ȫ��
	long SyncSendToSM();
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

public:
	//ӳ��WorldID��SocketID
	static void MapGSIDSocket(long lGSID,long lSocketID);
	static long GetSocketByGSID(long lGSID);
	static long GetGSIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID,long lNetFlag);
	static void UnMapGSIDSocket(long lSocketID);

// Fox@20080922----------------------------------------------
// ��������
	static void SetGateSocketID(int nSocket){ m_nGateSocket = nSocket; }
	static int GetGateSocketID(void) { return m_nGateSocket; }

private:
	static int				m_nGateSocket;		// ����GateServer��SocketID

//���ڴ�Ӱ���ļ��Ľ���ͨ�Ż���,ʵ��д�ļ����첽
//Author:wangqiao
public:
	//���ļ�
	static void AsyOpenFile(const char* pszFileName,bool bSync = true);
	//�ر��ļ�
	static void AsyCloseFile(const char* pszFileName,bool bSync = true);
	//����ļ�����
	static void AsyClearFile(const char* pszFileName,bool bSync = true);
	//�첽д�ļ��Ľӿ�
	static void AsyWriteFile(const char* pszFileName,const char* pszContent,bool bSync = true);
private:
	//! ���͵�SM����Ϣ˳��
	static long c_SmInfoOrder;
};

