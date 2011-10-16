#include "stdafx.h"
#include "Message.h"


extern void OnLogMessage(CMessage* pMsg);				// ��Ӧ��½��Ϣ
extern void OnAccountMessage(CMessage* pMsg);			// ��Ӧ�˺Ų�����Ϣ
extern void OnSocketLost(LONG lSockte);

CServer *CMessage::m_pClientServer = NULL;

CMessage::HMapLongLong	CMessage::m_SocketWorldID;
CMessage::HMapLongLong	CMessage::m_WorldIDSocket;

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
	m_SocketWorldID.clear();
	m_WorldIDSocket.clear();

	CBaseMessage::NewMsg = NewMessage;
	return CBaseMessage::Initial(pDBAllocator,nMaxFreeMsgNum);
}

void CMessage::SetClientServer(CServer *pClientServer)
{
	m_pClientServer = pClientServer;
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

// ������Ϣ��ָ���ͻ���
long CMessage::SendToClient(long lSocketID)
{
	assert(m_pClientServer);
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	//����һ����Ϣ
	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	m_pClientServer->ASend(lSocketID,pMsg);
	return true;
}

//���͸����пͻ���
long CMessage::SendAllClient()
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	itLL it = m_WorldIDSocket.begin();
	for(;it != m_WorldIDSocket.end();it++)
	{
		CBaseMessage *pMsg = CBaseMessage::AllocMsg();
		Clone(pMsg);
		m_pClientServer->ASend((*it).first,pMsg);
	}
	return true;
}



long CMessage::GetWorldID()
{
	return CMessage::GetWorldIDBySocket(m_lSocketID);
}




//ӳ��WorldID��SocketID
void CMessage::MapWorldIDSocket(long lWorldID,long lSocketID)
{
	m_SocketWorldID[lSocketID]=lWorldID;
	m_WorldIDSocket[lWorldID]=lSocketID;
}

long CMessage::GetSocketByWorldID(long lWorldID)
{
	itLL it = m_WorldIDSocket.find(lWorldID);
	if(it != m_WorldIDSocket.end())
		return (*it).second;
	return 0;
}

long CMessage::GetWorldIDBySocket(long lSocketID)
{
	itLL it = m_SocketWorldID.find(lSocketID);
	if(it != m_SocketWorldID.end())
		return (*it).second;
	return 0;
}

void CMessage::UnMapSocket(long lSocketID,long lNetFlag)
{
	if(lNetFlag&NF_AsscountServer_Client)
	{
		UnMapWorldIDSocket(lSocketID);
	}
}


void CMessage::UnMapWorldIDSocket(long lSocketID)
{
	itLL it = m_SocketWorldID.find(lSocketID);
	if(it != m_SocketWorldID.end())
	{
		m_WorldIDSocket.erase((*it).second);
		m_SocketWorldID.erase(it);
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
			if(GetNetFlag()&NF_AsscountServer_Client)
			{
				CMessage::UnMapWorldIDSocket(GetSocketID());
				OnSocketLost(GetSocketID());
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
	//----------------------------------------------------/��½��Ϣ/
	case MSG_O2A_LOGIN:
		OnLogMessage(this);
		break;

	//----------------------------------------------------/�˺Ų�����Ϣ/
	case MSG_O2A_ACCOUNT:
		OnAccountMessage(this);
		break;
	}
	return true;
}

