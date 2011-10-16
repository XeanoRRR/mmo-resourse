#pragma once

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
	static CServer	*m_pClientServer;

public:

	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	static void SetClientServer(CServer *pClientServer);

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
public:
	void SetSocketID(long lSocketID)	{ m_lSocketID=lSocketID;}
	long GetSocketID()					{return m_lSocketID;}

	void SetIP(DWORD dwIP)				{m_dwIP = dwIP;	}
	DWORD GetIP()						{return m_dwIP;	}

	long GetWorldID();

	void Reset(void);        //������Ϣ��ر���

	long SendToClient(long lSocketID);		//����һ����Ϣ���ͻ����׽���

	long SendAllClient();					// ���͸����пͻ���
	
	long DefaultRun();   //ȱʡ����Ϣ��������
	long Run();			// ����������Ϣ����

private:
	//�ͻ��������׽��ֺ�WorldServerID����

	typedef stdext::hash_map<long,long>	HMapLongLong;
	typedef stdext::hash_map<long,long>::iterator itLL;

	static HMapLongLong	m_SocketWorldID;
	static HMapLongLong	m_WorldIDSocket;

public:
	//ӳ��WorldID��SocketID
	static void MapWorldIDSocket(long lWorldID,long lSocketID);
	static long GetSocketByWorldID(long lWorldID);
	static long GetWorldIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID,long lNetFlag);
	static void UnMapWorldIDSocket(long lSocketID);
};
