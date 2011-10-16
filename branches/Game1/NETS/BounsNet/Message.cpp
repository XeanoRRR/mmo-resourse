#include "StdAfx.h"
#include "Message.h"
#include <MMSystem.h>

#include "..\Servers.h"
#include "..\MemClient.h"

#include "..\..\public\Crc32Static.h"
#include "..\..\public\singleton.h"

extern void OnLogServerMessage(CMessage *pMsg);
extern void OnServerMessage(CMessage* pMsg);			// ��ӦServer������Ϣ

CServer	*CMessage::m_pClientServer;
long CMessage::m_lServerSocket;
long CMessage::m_lLogSocket;

CMessage::HMapLongLong CMessage::m_SocketClientID;
CMessage::HMapLongLong CMessage::m_ClientIDSocket;

CMessage::CMessage(void)
:m_lSocketID(0),m_dwIP(0),m_dwRecvTime(0)
{
}

CMessage::CMessage(long lType)
:m_lSocketID(0),m_dwIP(0),m_dwRecvTime(0)
{
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

bool CMessage::Initial(CDataBlockAllocator* pDBAllocator, long nMaxFreeMsgNum)
{
	m_SocketClientID.clear();
	m_ClientIDSocket.clear();

	CBaseMessage::NewMsg = NewMessage;
	return CBaseMessage::Initial(pDBAllocator, nMaxFreeMsgNum);
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

long CMessage::SendToSocket(long lSocketID, bool bClone)
{
	*((long*)Base()) = GetSize() + HEAD_SIZE;

	// ����һ����Ϣ
	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	if(!bClone)
	{
		vector<CDataBlock*>& MsgData = GetMsgData();
		pMsg->SetMsgData(MsgData);
		MsgData.clear();
	}
	else
	{
		Clone(pMsg);
	}
	
	m_pClientServer->ASend(lSocketID, pMsg);
	return true;
}

// [������] ���͸����пͻ���
long CMessage::SendAll()
{
	*((long*)Base()) = GetSize() + HEAD_SIZE;
	if( m_SocketClientID.size() == 0 )
	{
		return true;
	}

	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(m_SocketClientID.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();

	itLL it = m_SocketClientID.begin();
	for(; it != m_SocketClientID.end(); ++it)
	{
		m_pClientServer->ASend((*it).first, pMsg);
	}
	return true;
}

long CMessage::Send(bool bFront)
{
	assert(m_pClientServer);
	*((long*)Base()) = GetSize() + HEAD_SIZE;

	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	m_pClientServer->ASend(m_lServerSocket, pMsg);
	return 0;
}

long CMessage::SendToSM()
{
	GetInst(CMemClient).Send(this);
	return 0;
}

long CMessage::GetGSID()
{
	return CMessage::GetGSIDBySocket(m_lSocketID);
}

void CMessage::MapGSIDSocket(long lGSID, long lSocketID)
{
	m_SocketClientID[lSocketID] = lGSID;
	m_ClientIDSocket[lGSID] = lSocketID;
}

long CMessage::GetSocketByGSID(long lGSID)
{
	itLL it = m_ClientIDSocket.find(lGSID);
	if(it != m_ClientIDSocket.end())
	{
		return (*it).second;
	}
	return 0;
}

long CMessage::GetGSIDBySocket(long lSocketID)
{
	itLL it = m_SocketClientID.find(lSocketID);
	if(it != m_SocketClientID.end())
	{
		return (*it).second;
	}
	return 0;
}

void CMessage::UnMapSocket(long lSocketID, long lNetFlag)
{
	if(lNetFlag&NF_SERVER_CLIENT)
	{
		UnMapGSIDSocket(lSocketID);
	}
}

void CMessage::UnMapGSIDSocket(long lSocketID)
{
	itLL it = m_SocketClientID.find(lSocketID);
	if( it != m_SocketClientID.end() )
	{
		m_ClientIDSocket.erase((*it).second);
		m_SocketClientID.erase(it);
	}
}

// ���ļ�
void CMessage::AsyOpenFile(const char* pszFileName)
{
	CMessage msgLog(MSG_MEM_OPEN_FILE);
	msgLog.Add(pszFileName);
	msgLog.SendToSM();
}

// �ر��ļ�
void CMessage::AsyCloseFile(const char* pszFileName)
{
	CMessage msgLog(MSG_MEM_CLOSE_FILE);
	msgLog.Add(pszFileName);
	msgLog.SendToSM();
}

// ����ļ�����
void CMessage::AsyClearFile(const char* pszFileName)
{
	CMessage msgLog(MSG_MEM_CLEAR_FILE);
	msgLog.Add(pszFileName);
	msgLog.SendToSM();
}

// �첽д�ļ��Ľӿ�
void CMessage::AsyWriteFile(const char* pszFileName, const char* pszContent)
{
	CMessage msgLog(MSG_MEM_WRITE_FILE);
	msgLog.Add(pszFileName);
	msgLog.Add(pszContent);
	msgLog.SendToSM();
}

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

long CMessage::DefaultRun()
{
	switch( GetType() )
	{
	case BASEMSG_Socket_Accept:
		break;
	case BASEMSG_Socket_Close:
		{
			// ��־���������ˣ����½��������߳�
			if( GetNetFlag()&NF_LOGSERVER_CLIENT )
			{
				GetGame()->OnLostLogServer();
				GetGame()->CreateConnectLogThread();
			}
			else if( GetNetFlag()&NF_SERVER_CLIENT )
			{
				// GameServer�Ͽ���
				GetGame()->OnGSLost(GetGSID());
				CMessage::UnMapGSIDSocket(GetSocketID());
			}
		}
		break;
	}

	return true;
}

long CMessage::Run()
{
#ifdef _RUNSTACKINFO_
	//�������ջ���
	char pszStackInfo[256]="";
	_snprintf(pszStackInfo,256,"CMessage::Run(msgtype:%d) Start.",GetType());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif
	// ��Ϣ���ദ��
	switch( GetType() - (GetType() & 0x00000ff) )	// GetType() - GetType() % 256
	{
	case 0:
		DefaultRun();
		break;

	 // ��־��Ϣ����
	case MSG_LOG20_SERVER:
		{
			OnLogServerMessage(this);
		}
		break;

	case MSG_S2BDS:
		{
			OnServerMessage(this);
		}
		break;
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMessage::Run() End.");
#endif
	return true;
}