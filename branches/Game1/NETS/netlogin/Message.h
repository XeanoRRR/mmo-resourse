#pragma once
#include "..\basemessage.h"
#include "..\public\MsgType.h"
#include "..\..\public\ManageToolMsgType.h"
#include <hash_map>

using namespace stdext;

class CServer;
class CRegion;
class CPlayer;
class CMapFile;


class CMessage :
	public CBaseMessage
{
public:
	CMessage(long lType);
	CMessage(void);
	virtual ~CMessage(void);
private:
	static CServer	*m_pClientServer;
	static CServer *m_pWorldServer;
	static long			m_lASSocket;		//����AccountServer�Ŀͻ���
public:

	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	static void SetClientServer(CServer *pClientServer);
	static void SetWorldServer(CServer *pWorldServer);
	static void SetASSocket(long lSocket){m_lASSocket=lSocket;}
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

	const char* GetStringIP()
	{
		static char pszIP[256]="";
		_snprintf(pszIP,256, "%d.%d.%d.%d", LOBYTE(LOWORD(m_dwIP)), HIBYTE(LOWORD(m_dwIP)), LOBYTE(HIWORD(m_dwIP)), HIBYTE(HIWORD(m_dwIP)));
		return pszIP;
	}

	const char *GetCdkey();
	long GetWorldID();

	void Reset(void);        //������Ϣ��ر���

	long SendToClient(long lSocketID);		//����һ����Ϣ���ͻ����׽���
	long SendToWorld(long lSocketID);		//����һ����Ϣ��WorldServer�׽���

	long SendAllClient();					// ���͸����пͻ���
	long SendAllWorld();					// ���͸�����WorldServer

	long SendToWorldNumID(long lWorldID);
	long SendToClient(const char *szCdkey);
	
	long SendToSM();
	//! [��ΪAS�ͻ���] ������Ϣ
	BOOL SendToAS(VOID);
	
	long DefaultRun();   //ȱʡ����Ϣ��������
	long Run();			// ����������Ϣ����

private:
	//�ͻ����׽��ֺ�CDkey����
	typedef stdext::hash_map<long,string>	HMapLongString;
	typedef stdext::hash_map<long,string>::iterator itLS;

	typedef stdext::hash_map<string,long>	HMapStringLong;
	typedef stdext::hash_map<string,long>::iterator itSL;

	
	static HMapLongString	m_SocketCDkey;
	static HMapStringLong	m_CDkeySocket;
	//WorldServer�����׽��ֺ�WorldServerID����

	typedef stdext::hash_map<long,long>	HMapLongLong;
	typedef stdext::hash_map<long,long>::iterator itLL;

	static HMapLongLong	m_SocketWorldID;
	static HMapLongLong	m_WorldIDSocket;

public:
	//ӳ��CDkey��SocketID
	static void MapCDkeySocket(const char* szCdkey,long lSocketID);
	static long GetSocketByCdkey(const char* szCdkey);
	static const char* GetCDkeyBySocket(long lSocketID);
	static void UnCDkeyBySocket(const char* szCdkey);
	//ӳ��WorldID��SocketID
	static void MapWorldIDSocket(long lWorldID,long lSocketID);
	static long GetSocketByWorldID(long lWorldID);
	static long GetWorldIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID,long lNetFlag);
	static void UnMapCDkeySocket(long lSocketID);
	static void UnMapWorldIDSocket(long lSocketID);


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
