#pragma once
#include "..\basemessage.h"
#include "..\..\public\MsgType.h"
#include <hash_map>

using namespace stdext;

class CServer;
class CRegion;
class CPlayer;


class CMessage :
	public CBaseMessage
{
public:
	CMessage(long lType);
	CMessage(void);
	virtual ~CMessage(void);
private:
	
	static CServer *m_pGSServer;
public:

	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	
	static void SetGSServer(CServer *pGSServer);
	//�ͷ���Դ
	static bool Release();
	static CBaseMessage* NewMessage();

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

	// ������
private:
	long	m_lSocketID;			//������Ϣ������SocketID;
	DWORD	m_dwIP;

	static long m_lLogSocket;		//������־�������Ŀͻ���
public:
	void SetSocketID(long lSocketID)	{ m_lSocketID=lSocketID;}
	long GetSocketID()					{return m_lSocketID;}

	void SetIP(DWORD dwIP)				{m_dwIP = dwIP;	}
	DWORD GetIP()						{return m_dwIP;	}

	
	long GetWorldID();

	void Reset(void);        //������Ϣ��ر���

	
	long SendToGS(long lSocketID);		//����һ����Ϣ��WorldServer�׽���

	
	long SendAllGS();					// ���͸�����WorldServer

	long SendToWorldNumID(long lWorldID);	

	long SendToSocket(long lSocketID);	
	
	long DefaultRun();   //ȱʡ����Ϣ��������
	long Run();			// ����������Ϣ����

	static void SetLogSocket(long lSocket){m_lLogSocket=lSocket;}

private:	
	//GameServer�����׽��ֺ�WorldServerID����

	typedef stdext::hash_map<long,long>	HMapLongLong;
	typedef stdext::hash_map<long,long>::iterator itLL;

	static HMapLongLong	m_SocketGSID;
	static HMapLongLong	m_GSIDSocket;

public:	
	//ӳ��GameServer ID��SocketID
	static void MapGSIDSocket(long lGSID,long lSocketID);
	static long GetSocketByGSID(long lGSID);
	static long GetGSIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID,long lNetFlag);	
	static void UnMapGSIDSocket(long lSocketID);
};
