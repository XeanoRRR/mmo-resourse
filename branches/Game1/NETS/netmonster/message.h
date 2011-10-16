#pragma once

#include "..\basemessage.h"
#include "../../public\MsgType.h"
#include "../../public/guid.h"

class CMySocket;
class CServer;
class CPlayer;

class CMessage :
	public CBaseMessage
{
public:
	CMessage();
	CMessage( long lType, const CGUID &DiscardID )
	{
		assert( 0 && "Not implement!" );
	}
	CMessage(long lType);
	virtual ~CMessage(void);

private:
	static CServer	*m_pClientServer;
public:
	//��ʼ��
	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	//�ͷ���Դ
	static bool Release();

	static void SetServer(CServer *pServer)	{m_pClientServer=pServer;}

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

	long SendToSocket(long lSocketID);
	long SendGSID(long lGSID);		

	///////////////////////////////////////////////////////////////////////
	/// Only for compatible....
	long SendToAround( void *p ) {assert( 0 ); return 1l; }
	long SendToAround( ... ) {assert( 0 ); return 1l; }
	long SendToPlayer( const CGUID &guid ) { assert( 0 );return 1l; }
	long SendToRegion( ... ) { assert( 0 ); return 1l ; }
	long SendToArea( ... ) { assert( 0 ); return 1; }
	CGUID GetPlayerID() { assert( 0 ); return CGUID(); }
	void SendAll() { assert( 0 ); }
	CPlayer *GetPlayer() { assert( 0 ); return 0; }
	///////////////////////////////////////////////////////////////////////

	long DefaultRun();
	virtual long Run();					// ����������Ϣ����


	/// ������Ϣ��WS
	long Send();		// ������Ϣ

//////////////////////////////////////////////////////////////////////////
//	ʱ���
//////////////////////////////////////////////////////////////////////////
protected:
	DWORD	m_dwRecvTime;			// �յ���ʱ��

public:
	DWORD	GetRecvTime()			{return m_dwRecvTime;}
	void	SetRecvTime(DWORD dw)	{m_dwRecvTime=dw;}

};

