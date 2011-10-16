#pragma once
#include "..\basemessage.h"
#include "..\..\public\MsgType.h"
#include "..\..\public\ManageToolMsgType.h"
#include "..\..\GameManageTools\GMTPublic\def\MsgGMT_SCC2SM.h"
#include <hash_map>
using namespace stdext;

class CMySocket;
class CServer;
#define CHECKGAPP(GappID)\
	CGapp* pGapp=GetSM()->FindGapp(GappID);\
	if (NULL==pGapp)\
	{\
		break;\
	}

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
	static long	m_lSccSocket;			//����costserver�Ŀͻ���
public:
	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	//�ͷ���Դ
	static bool Release();

	static void SetServer(CServer *pServer)	{m_pClientServer=pServer;}
	static void SetLogingSocket(long lSocket){m_lLogingSocket=lSocket;}
	static void SetSccSocket(long lSocket){m_lSccSocket=lSocket;}

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

	long GetSCCID();

	long SendToSocket(long lSocketID);	//���͵�Gameserver��Socket
	long SendGSID(long lGSID);		//���͵�Gameserver��һ��ӳ��ID

	long SendAll();						// ���͸����пͻ���
	long DefaultRun();
	virtual long Run();					// ����������Ϣ����

	long SendToAllSCC();				//���͵����е�SCC
	long SendToScc(long sccID);			//���͵�ָ��SCC
	long Send(bool bFront=false);		// ������Ϣ
	void SendToCost(bool bFront=false);
	long SendToGapp(long lID);					//���͵�GAPP
//////////////////////////////////////////////////////////////////////////
//	ʱ���
//////////////////////////////////////////////////////////////////////////
protected:
	DWORD	m_dwRecvTime;			// �յ���ʱ��

public:
	DWORD	GetRecvTime()			{return m_dwRecvTime;}
	void	SetRecvTime(DWORD dw)	{m_dwRecvTime=dw;}

private:
	//logServer�����׽��ֺ�logServerID����
	typedef stdext::hash_map<long,long>	HMapLongLong;
	typedef stdext::hash_map<long,long>::iterator itLL;

	static HMapLongLong	m_SocketSCCID;//key����socketID
	static HMapLongLong	m_SCCIDSocket;//key  ��SCCID

public:
	//ӳ��logID��SocketID
	static void MapSCCIDSocket(long lGSID,long lSocketID);
	static long GetSocketBySCCID(long lGSID);
	static long GetSCCIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID,long lNetFlag);
	static void UnMapSCCIDSocket(long lSocketID);
};

