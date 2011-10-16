#include "stdafx.h"
#include "message.h"
#include <MMSystem.h>

#include "../NetSUC/Message.h"
#include "../Servers.h"
#include "../Clients.h"
//#include "../PUBLIC/Crc32Static.h"
//#include "CostNetClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//xtern void OnClientMessage(CMessage* pMsg);				// ��Ӧ��־��Ϣ


//CServer	*CMessage::m_pClientServer;
//long	CMessage::m_lLogingSocket;
//long	CMessage::m_lCostSocket;
CClient* CMessage::m_pNetClient;

// CMessage::HMapLongLong	CMessage::m_SocketGSID;
// CMessage::HMapLongLong	CMessage::m_GSIDSocket;

CMessage::CMessage()
{
//	m_lSocketID	=0;
//	m_dwIP		=0;
//	m_dwRecvTime = 0;
}

CMessage::CMessage(long lType)
{
	
//	m_lSocketID	=0;
//	m_dwIP		=0;
//	m_dwRecvTime = 0;

	Init(lType);
}

CMessage::~CMessage(void)
{
	UnInit();
}

 //������Ϣ��ر���
void CMessage::Reset(void)
{
//	m_lSocketID=0;
//	m_dwIP=0;
}

bool CMessage::Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum)
{
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

//	�ͻ��� ������Ϣ
long CMessage::Send(CClient * pClient /* = NULL  */)
{
	if( pClient == NULL )
		pClient = m_pNetClient;
	if( pClient == NULL )
		return false;

	*((long*)Base()) = GetSize() + HEAD_SIZE;
	CBaseMessage* pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	pClient->ASend(pMsg);
	return true;
}
// //���͵�Gameserver��Socket
// long CMessage:: SendToSocket(long lSocketID)
// {
// 	assert(m_pClientServer);
// 	*((long*)Base()) = GetSize()+12;
// 	//����һ����Ϣ
// 	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
// 	//vector<CDataBlock*>& MsgData = GetMsgData();
// 	//pMsg->SetMsgData(MsgData);
// 	//MsgData.clear();
// 	Clone(pMsg);
// 	m_pClientServer->ASend(lSocketID,pMsg);
// 	return true;
// }
// 
// // //���͵�Gameserver��Socket
// long CMessage:: SendGSID(long lGSID)
// {
// 	long lSocket = GetSocketByGSID(lGSID);
// 	SendToSocket(lSocket);
// 	return true;
// }
// 
// // [������] ���͸����пͻ���
// long CMessage::SendAll()
// {
// 	*((long*)Base()) = GetSize()+12;
// 	itLL it = m_SocketGSID.begin();
// 	for(;it != m_SocketGSID.end();it++)
// 	{
// 		CBaseMessage *pMsg = CBaseMessage::AllocMsg();
// 		Clone(pMsg);
// 		m_pClientServer->ASend((*it).first,pMsg);
// 	}
// 	return true;
// }

// long CMessage::Send(bool bFront)
// {
// 	assert(m_pClientServer);
// 	*((long*)Base()) = GetSize()+12;
// 	//����һ����Ϣ
// 	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
// 	vector<CDataBlock*>& MsgData = GetMsgData();
// 	pMsg->SetMsgData(MsgData);
// 	MsgData.clear();
// 	m_pClientServer->ASend(m_lLogingSocket,pMsg);
// 	return 0;
// }
// 
// void CMessage::SendToCost(bool bFront)
// {	
// 	return;
// }
// 
// long CMessage::GetGSID()
// {
// 	return GetGSIDBySocket(m_lSocketID);
// }
// 
// void CMessage::MapGSIDSocket(long lGSID,long lSocketID)
// {
// 	m_SocketGSID[lSocketID]=lGSID;
// 	m_GSIDSocket[lGSID]=lSocketID;
// }
// long CMessage::GetSocketByGSID(long lGSID)
// {
// 	itLL it = m_GSIDSocket.find(lGSID);
// 	if(it != m_GSIDSocket.end())
// 		return (*it).second;
// 	return 0;
// }
// long CMessage::GetGSIDBySocket(long lSocketID)
// {
// 	itLL it = m_SocketGSID.find(lSocketID);
// 	if(it != m_SocketGSID.end())
// 		return (*it).second;
// 	return 0;
// }
// 
// void CMessage::UnMapSocket(long lSocketID,long lNetFlag)
// {
// 	//if(lNetFlag&NF_Server_Client)
// 	{
// 		UnMapGSIDSocket(lSocketID);
// 	}
// }
// void CMessage::UnMapGSIDSocket(long lSocketID)
// {
// 	itLL it =  m_SocketGSID.find(lSocketID);
// 	if(it != m_SocketGSID.end() )
// 	{
// 		m_GSIDSocket.erase((*it).second);
// 		m_SocketGSID.erase(it);
// 	}
// }
// 

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

long CMessage::DefaultRun()
{
	switch( GetType() )
	{
	case BASEMSG_Socket_Accept:
		{
		//	GetGame()->OnGameServerConnect(GetIP(), GetSocketID());
		}
		break;
	case BASEMSG_Socket_Close:
		{
		//	if(GetNetFlag()&NF_Server_Client)
			{
		//		GetGame()->OnGameServerClose(GetSocketID());
//				CMessage::UnMapGSIDSocket(GetSocketID());
			}
// 			else if(GetNetFlag()&NF_LServer_Client)
// 			{
// 			}
// 			else if(GetNetFlag()&NF_CServer_Client)
// 			{
// 			}
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
	//	OnClientMessage(this);
		break;

	}


	return true;
}
