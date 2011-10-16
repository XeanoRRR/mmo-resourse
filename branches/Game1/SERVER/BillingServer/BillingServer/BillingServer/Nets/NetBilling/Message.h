#pragma once
#include "..\basemessage.h"

class CMySocket;
class CRegion;
class CPlayer;


class CMessage :
	public CBaseMessage
{
public:
	CMessage(long lType);
	virtual ~CMessage(void);
public:

	CRegion* m_pRegion;
	CPlayer* m_pPlayer;
	CRegion* GetRegion()	{ return m_pRegion; }
	CPlayer* GetPlayer()	{ return m_pPlayer; }

	static CMessage* CreateMessage(void* pRLEBuf, unsigned long lRLESize);	// �յ�Socket���ܵ��İ��󽫰���װ����Ϣ
	static CMessage* CreateMessageWithoutRLE(void* pBuf, unsigned long nSize);	// �յ�Socket���ܵ��İ��󽫰���װ����Ϣ

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

	// ������
protected:
	long	m_lMapID;				// ������Ϣ��PlayerID
	long	m_lSocketID;			// ������Ϣ������SocketID;
	DWORD	m_dwIP;					// ���͸���Ϣ��IP
	string  m_strCdkey;				// ������Ϣ������Cdkey
public:
	void SetClientSocketID(long lSocketID)	{ m_lSocketID=lSocketID;}
	long GetClientSocketID()				{return m_lSocketID;}

	void SetIP(DWORD dwIP)					{m_dwIP = dwIP;	}
	DWORD GetIP()							{return m_dwIP;	}

	void SetMapID(long lPlayerID)		{m_lMapID =lPlayerID;}
	long GetMapID()						{return m_lMapID;}

	void SetCdkey(const char* strCdkey)		{m_strCdkey = strCdkey;}
	const char* GetCdkey()					{return m_strCdkey.c_str();}

	long SendToGS(long lSocketID);					// ����һ����Ϣ���ͻ����׽���
	long SendToGS(const char* strCdkey);			// ������Ϣ��Cdkey

	long SendToWorldNumID(long lNumID);				//����һ����Ϣ��WorldID

	long SendToAllGS();								// ���͸����пͻ���
	
	virtual long Run();								// ����������Ϣ����
};
