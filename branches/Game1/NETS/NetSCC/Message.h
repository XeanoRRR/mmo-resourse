#pragma once
#include "..\basemessage.h"
#include "..\..\public\MsgType.h"
#include <hash_map>
using namespace stdext;

class CMySocket;
class CServer;


class CMessage :
	public CBaseMessage
{
public:
	CMessage();
	CMessage(long lType);
	virtual ~CMessage(void);

private:
	static CServer	*m_pSUCServer;
	static CServer	*m_pSMServer;
	static long	m_lLogingSocket;		//����loginserver�Ŀͻ���
	static long	m_lCostSocket;			//����costserver�Ŀͻ���
public:
	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	//�ͷ���Դ
	static bool Release();

	static void SetSUCServer		(CServer *pServer)	{m_pSUCServer=pServer;}
	static void SetSMServer			(CServer *pServer)	{m_pSMServer=pServer;}
	static void SetLogingSocket		(long lSocket)		{m_lLogingSocket=lSocket;}
	static void SetCostSocket		(long lSocket)		{m_lCostSocket=lSocket;}

	static CBaseMessage* NewMessage();

	static void DisSucConn(long lSocket);
	static void DisSmConn(long lSocket);
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

	long SendToSuc(long lSocketID);
	long SendToSm(long lSocketID);

	long DefaultRun();
	virtual long Run();					// ����������Ϣ����


//////////////////////////////////////////////////////////////////////////
//	ʱ���
//////////////////////////////////////////////////////////////////////////
protected:
	DWORD	m_dwRecvTime;			// �յ���ʱ��

public:
	DWORD	GetRecvTime()			{return m_dwRecvTime;}
	void	SetRecvTime(DWORD dw)	{m_dwRecvTime=dw;}

private:
	static map<DWORD, LONG>		c_mapSMID2Socket;
	static map<LONG, DWORD>		c_mapSMSocket2ID;

	static map<string, LONG>	c_mapCdkey2Socket;
	static map<LONG, string>	c_mapSocket2Cdkey;

	//!							�Ƿ�����SCC����
	static BOOL					c_bAllowSUCConn;

public:
	static void		OnSUCAccept(long lSocketID);
	static void		OnSUCClose(long lSocketID);
	static void		OnSMAccept(long lSocketID, long lIP);
	static void		OnSMClose(long lSocketID);

	static void		AllowSUC(BOOL bIsAllow){c_bAllowSUCConn = bIsAllow;};

	static LONG		GetSMSocket(DWORD dwSMID);
	static DWORD	GetSMID(long lSMSocket);
};

