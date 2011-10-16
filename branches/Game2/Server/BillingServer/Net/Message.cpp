#include "stdafx.h"
#include "Message.h"

extern void OnServerMessage(CMessage* pMsg);			// ��ӦServer������Ϣ
extern void OnLogMessage(CMessage* pMsg);				// ��Ӧ��½��Ϣ
extern void OnOtherMessage(CMessage* pMsg);				// ��Ӧ������Ϣ
extern void OnGMMessage(CMessage* pMsg);
extern void OnLogServerMessage(CMessage *pMsg);

CServer *CMessage::m_pGSServer= NULL;

CMessage::HMapLongLong	CMessage::m_SocketGSID;
CMessage::HMapLongLong	CMessage::m_GSIDSocket;

long	CMessage::m_lLogSocket;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessage::CMessage(void)
{
	m_lSocketID=0;
	m_dwIP=0;
}

CMessage::CMessage(long lType)
{
	m_lSocketID=0;
	m_dwIP=0;
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

void CMessage::SetGSServer(CServer *pGSServer)
{
	m_pGSServer = pGSServer;
}

//�ͷ���Դ
bool CMessage::Release()
{
	CBaseMessage::NewMsg = NULL;
	return CBaseMessage::Release();
}

CBaseMessage* CMessage::NewMessage()
{
	return new CMessage();
}

//����һ����Ϣ��WorldServer�׽���
long CMessage::SendToGS(long lSocketID)
{
	assert(m_pGSServer);
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	m_pGSServer->ASend(lSocketID,pMsg);
	return true;
}

//���͸�����WorldServer
long CMessage::SendAllGS()
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	itLL it = m_SocketGSID.begin();
	for(;it != m_SocketGSID.end();it++)
	{
		CBaseMessage *pMsg = CBaseMessage::AllocMsg();
		Clone(pMsg);
		m_pGSServer->ASend((*it).first,pMsg);
	}
	return true;
}

// //���͵�GS,DBS��Socket
long CMessage:: SendToSocket(long lSocketID)
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	//����һ����Ϣ
	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	/*vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();*/
	Clone(pMsg);
	m_pGSServer->ASend(lSocketID,pMsg);

	//assert(m_pClientServer);
	//*((long*)Base()) = GetSize()+12;
	////����һ����Ϣ
	//CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	//vector<CDataBlock*>& MsgData = GetMsgData();
	//pMsg->SetMsgData(MsgData);
	//MsgData.clear();
	//m_pClientServer->ASend(lSocketID,pMsg);
	return true;
}

long CMessage::SendToWorldNumID(long lWorldID)
{
	long lSocket = GetSocketByGSID(lWorldID);
	SendToGS(lSocket);
	return true;
}

long CMessage::GetWorldID()
{
	return CMessage::GetGSIDBySocket(m_lSocketID);
}


//ӳ��WorldID��SocketID
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
	if(lNetFlag & NF_Server_World)
	{
		UnMapGSIDSocket(lSocketID);
	}
}

void CMessage::UnMapGSIDSocket(long lSocketID)
{
	itLL it = m_SocketGSID.find(lSocketID);
	if(it != m_SocketGSID.end())
	{
		m_GSIDSocket.erase((*it).second);
		m_SocketGSID.erase(it);
	}
}

long CMessage::DefaultRun()
{
	switch( GetType() )
	{
	case BASEMSG_Socket_Accept:
		{
		}
		break;
	case BASEMSG_Socket_Close:
		{
			if(GetNetFlag()&NF_Server_World)
			{
				GetGame()->OnNetGSClose(GetWorldID());
				CMessage::UnMapGSIDSocket(GetSocketID());
			}
			else if(GetNetFlag()&NF_Log_Client)
			{
				GetGame()->OnLOGSLost();
			}
		}
		break;
	}
	return true;
}
// ��Ϣ����
long CMessage::Run()
{
	// ��Ϣ���ദ��
	switch( GetType() - (GetType() & 0x00000ff) )	// GetType() - GetType() % 256
	{
	case 0:
		DefaultRun();
		break;
	//----------------------------------------------------/Server��Ϣ/
	case MSG_S2B:	
		OnServerMessage(this);
		break;

	//----------------------------------------------------/��½��Ϣ/
	case MSG_L2L_LOG:
	case MSG_C2L_LOG:
	case MSG_W2L_LOG:
		OnLogMessage(this);
		break;
	case MSG_LOG20_SERVER:
		{
			OnLogServerMessage(this);
		}
		break;

	//----------------------------------------------------/������Ϣ/
//	case MSG_OTHER:
//		OnOtherMessage(this);

		break;
	case MSG_W2L_GM:
		OnGMMessage(this);
		break;

	}
	return true;
}

