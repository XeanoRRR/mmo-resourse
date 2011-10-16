#include "stdafx.h"
#include "message.h"
#include <MMSystem.h>

#include "..\Servers.h"
#include "../PUBLIC/Crc32Static.h"
//#include "CostNetClient.h"
#include "..\public\Singleton.h"
#include "..\nets\MemClient.h"
#include "..\public\ManageToolMsgType.h"  
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void OnClientMessage(CMessage* pMsg);				// ��Ӧ��־��Ϣ
extern void OnSMMessage(CMessage* pMsg);					// ��ӦSM��Ϣ

CServer	*CMessage::m_pClientServer;

CMessage::HMapLongLong	CMessage::m_SocketGSID;
CMessage::HMapLongLong	CMessage::m_GSIDSocket;

CMessage::CMessage()
{
	m_lSocketID	=0;
	m_dwIP		=0;
	m_dwRecvTime = 0;
}

CMessage::CMessage(long lType)
{
	
	m_lSocketID	=0;
	m_dwIP		=0;
	m_dwRecvTime = 0;

	Init(lType);
}

CMessage::~CMessage(void)
{
	UnInit();
}

 //������Ϣ��ر���
void CMessage::Reset(void)
{
	m_lSocketID=0;
	m_dwIP=0;
}

bool CMessage::Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum)
{
	m_SocketGSID.clear();
	m_GSIDSocket.clear();

	CBaseMessage::NewMsg = NewMessage;
	return CBaseMessage::Initial(pDBAllocator,nMaxFreeMsgNum);
}
bool CMessage::Release()
{
	CBaseMessage::NewMsg = NULL;
	return CBaseMessage::Release();
}

CBaseMessage* CMessage::NewMessage()
{
	return new CMessage();
}

// //���͵�Gameserver��Socket
long CMessage:: SendToSocket(long lSocketID)
{
	assert(m_pClientServer);
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	//����һ����Ϣ
	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	//vector<CDataBlock*>& MsgData = GetMsgData();
	//pMsg->SetMsgData(MsgData);
	//MsgData.clear();
	Clone(pMsg);
	m_pClientServer->ASend(lSocketID,pMsg);
	return true;
}

// //���͵�Gameserver��Socket
long CMessage:: SendGSID(long lGSID)
{
	long lSocket = GetSocketByGSID(lGSID);
	SendToSocket(lSocket);
	return true;
}

// [������] ���͸����пͻ���
long CMessage::SendAll()
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	itLL it = m_SocketGSID.begin();
	for(;it != m_SocketGSID.end();it++)
	{
		CBaseMessage *pMsg = CBaseMessage::AllocMsg();
		Clone(pMsg);
		m_pClientServer->ASend((*it).first,pMsg);
	}
	return true;
}

long CMessage::GetGSID()
{
	return GetGSIDBySocket(m_lSocketID);
}

void CMessage::MapGSIDSocket(long lGSID,long lSocketID)
{
	m_SocketGSID[lSocketID]=lGSID;
	m_GSIDSocket[lGSID]=lSocketID;
}
long CMessage::GetSocketByGSID(long lGSID)
{
	itLL it = m_GSIDSocket.find(lGSID);
	if(it != m_GSIDSocket.end())
		return (*it).second;
	return 0;
}
long CMessage::GetGSIDBySocket(long lSocketID)
{
	itLL it = m_SocketGSID.find(lSocketID);
	if(it != m_SocketGSID.end())
		return (*it).second;
	return 0;
}

void CMessage::UnMapSocket(long lSocketID,long lNetFlag)
{
	if(lNetFlag&NF_Server_Client)
	{
		UnMapGSIDSocket(lSocketID);
	}
}
void CMessage::UnMapGSIDSocket(long lSocketID)
{
	itLL it =  m_SocketGSID.find(lSocketID);
	if(it != m_SocketGSID.end() )
	{
		m_GSIDSocket.erase((*it).second);
		m_SocketGSID.erase(it);
	}
}

void CMessage::SendToSM()
{
	GetInst(CMemClient).Send(this);
}

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

long CMessage::DefaultRun()
{
	switch( GetType() )
	{
	case BASEMSG_Socket_Accept:
		{
			GetGame()->OnGameServerConnect(GetIP(), GetSocketID());
		}
		break;
	case BASEMSG_Socket_Close:
		{
			if(GetNetFlag()&NF_Server_Client)
			{
				GetGame()->OnGameServerClose(GetSocketID());
				CMessage::UnMapGSIDSocket(GetSocketID());
			}
			else if(GetNetFlag()&NF_LServer_Client)
			{
			}
			else if(GetNetFlag()&NF_CServer_Client)
			{
			}
		}
		break;
	}
	return true;
}

// ����������Ϣ����
long CMessage::Run()
{
	// ��Ϣ���ദ��
	switch( GetType() - (GetType() & 0x00000ff) )	// GetType() - GetType() % 256
	{
	case 0:
		DefaultRun();
		break;
	//----------------------------------------------------/�ͻ���������Ϣ��Ϣ/
	case MSG_02LOG_SERVER:
		OnClientMessage(this);
		break;
	case MSG_SM2GAPP_OPERATOR:
		OnSMMessage(this);
		break;

	}


	return true;
}
