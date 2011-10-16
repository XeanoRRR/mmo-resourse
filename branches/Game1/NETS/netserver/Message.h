#pragma once
#include "..\basemessage.h"
#include "..\..\public\MsgType.h"
#include "..\..\public\ManageToolMsgType.h" 
#include "..\..\public\GUID.h"
#include "..\..\server\gameserver\gameserver\game.h"
#include <hash_map>
using namespace stdext;

class CMySocket;
class CServerRegion;
class CArea;
class CShape;
class CPlayer;
class CServer;
class CMapFile;

class CMessage :
	public CBaseMessage
{
public:
	CMessage();
	CMessage(long lType);
	CMessage(long lType,const CGUID& guDiscardFlag);
	virtual ~CMessage(void);
private:
	static CServer	*m_pServer;
	static long	m_lWSSocket;		//����worldserver�Ŀͻ���
	static long m_lBSSocket;		//����BILLINGSERVER�Ŀͻ���
	//[Amend AHC 070719]
	static long	m_lDBSSocket;		//����DBserver�Ŀͻ���
	static long m_lLogSocket;		//������־�������Ŀͻ���
	static long m_lBounsSocket;		//����BounsServer�Ŀͻ���

public:
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	static bool Release();

	static void SetServer(CServer *pServer)	{m_pServer=pServer;}
	static void SetWSSocket(long lSocket)	{m_lWSSocket=lSocket;}
	static long GetWSSocket(void)			{return m_lWSSocket;}
	static void SetDBSSocket(long lSocket)	{m_lDBSSocket=lSocket;}
	static void SetLogSocket(long lSocket)	{m_lLogSocket=lSocket;}
	static void SetBSSocket(long lSocket)	{m_lBSSocket=lSocket;}
	static void SetBounsSocket(long lSocket){ m_lBounsSocket = lSocket; }


	static CBaseMessage* NewMessage();
////////////////////////////////////////////////////////////////////////
//	Player & Region
////////////////////////////////////////////////////////////////////////
private:
	CServerRegion* m_pRegion;
	CPlayer* m_pPlayer;

public:
	CServerRegion* GetRegion()		{return m_pRegion;}
	CPlayer* GetPlayer()			{return m_pPlayer;}
	void SetPlayer(CPlayer* p)		{m_pPlayer=p;}

	void Reset(void);        //������Ϣ��ر���

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

// ������
protected:
	long	m_lSocketID;			//������Ϣ������SocketID;
	DWORD	m_dwIP;					//���͸���Ϣ��IP
	CGUID	m_guDiscardFlag;		//��Ϣ�Ķ�����ʾ

public:
	void SetSocketID(long lSocketID)	{ m_lSocketID=lSocketID;}
	long GetSocketID()					{return m_lSocketID;}

	void SetIP(DWORD dwIP)				{m_dwIP = dwIP;	}
	DWORD GetIP()						{return m_dwIP;	}

	void SetDiscardFlag(const CGUID& guid)	{m_guDiscardFlag=guid;}
	const CGUID& GetDiscardFlag()		{return m_guDiscardFlag;}
	//�ж�������Ϣ�Ķ�������Ƿ�һ��
	virtual bool IsDiscardFlagEqual(CBaseMessage* pMsg);
	//����һ����Ϣ
	static CMessage* AllocMessage();

	const CGUID& GetPlayerID();

	long SendToSocket(long lSocketID,bool bClone=true);			//����һ����Ϣ���׽���
	long SendToPlayer(const CGUID& PlayerID,bool bClone=true);	// ����һ����Ϣ�����
	long SendToBS();
	long SendToRegion(CServerRegion* =NULL, CPlayer* =NULL);	// ���͸�һ���������������
	long SendToArea(CArea* =NULL, CPlayer* =NULL);				// ���͸�һ��AREA���������
	long SendToAround(CShape* pMain,CPlayer* pExceptionPlayer=NULL,
		bool bExceptTranCongSocket=false);	// [������] ���͸�һ�������Χ�����(pExceptionPlayer=����)
	long SendToAround(CServerRegion* pRegion,int x, int y, CShape* pMain=NULL, 
		CPlayer* pExceptionPlayer=NULL,bool bExceptTranCongSocket=false);	// [������] ���͸�һ������Χ�����(pExceptionPlayer=����)
	//����Ϣ��ָ�����������
	long SendAroundLimitPlayer(CMoveShape* pMoveShape,const CGUID& guidExeptID,bool bExceptTranCongSocket=false);
	//��ָ�����������ҷ�����Ϣ
	long SendToNearestPlayer(CShape* pMain,bool bExceptTranCongSocket=false,CPlayer* pExceptionPlayer=NULL);	
	//������Ϣ��ָ�����
	long SendToPlayers(hash_set<CGUID,hash_guid_compare>& hsPlayers,bool bExceptTranCongSocket=false);

	//���б����ų������������׽���
	void RemoveTransCongSockets(list<long>&	SocketList);
	long SendToAroundTeamate(CShape* pMain,CShape* pDest);					//������Χ����
	long SendAll();						// ���͸����пͻ���
	long Send(bool bFront=false);		// ������Ϣ
	long SendToBounsServer();			// ������Ϣ��BounsServer
	long SendToSM();
	//ͬ�����͵�SM,���ܶ�ʧ,����ȫ��
	long SyncSendToSM();
	long DefaultRun();
	virtual long Run();					// ����������Ϣ����
private:
	//WorldServer�����׽��ֺ�WorldServerID����
	typedef stdext::hash_map<long,CGUID>				HMapLongGuid;
	typedef stdext::hash_map<CGUID,long, hash_guid_compare>				HMapGuidLong;
	typedef stdext::hash_map<long,CGUID>::iterator		iteLG;
	typedef stdext::hash_map<CGUID,long, hash_guid_compare>::iterator		iteGL;

	static HMapLongGuid	m_SocketPlayerID;
	static HMapGuidLong	m_PlayerIDSocket;

	typedef set<long>	Sockets;
	typedef set<long>::iterator itTCSocket;
	//���紫��ӵ����sockets����
	static Sockets	m_TransCongSokcets;

public:
	//ӳ��WorldID��SocketID
	static void MapPlayerIDSocket(const CGUID& PlayerGuid,long lSocketID);
	static long GetSocketByPlayerID(const CGUID& PlayerGuid);
	static const CGUID& GetPlayerIDBySocket(long lSocketID);

	static void UnMapSocket(long lSocketID,long lNetFlag);
	static void UnMapPlayerIDSocket(long lSocketID);


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