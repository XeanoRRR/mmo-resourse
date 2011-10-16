#include "stdafx.h"
#include "message.h"
#include <MMSystem.h>

#include "..\Servers.h"
#include "nets\MemClient.h"
#include "..\public\Singleton.h"
#include "../PUBLIC/Crc32Static.h"
#include "..\..\server\WorldServer\AppWorld\DBEntity\EntityManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void OnRgnMessage(CMessage* pMsg);		// ��ӦRgn������Ϣ
extern void OnServerMessage(CMessage* pMsg);		// ��ӦServer������Ϣ
extern void OnDBOPMessage(CMessage* pMsg);			// ��Ӧ���ݿ������Ϣ
extern void OnLogMessage(CMessage* pMsg);			// ��Ӧ��½��Ϣ
extern void OnOtherMessage(CMessage* pMsg);			// ��Ӧ������Ϣ
extern void OnGMMessage(CMessage* pMsg);			// ��ӦGM������Ϣ
extern void OnGSTeamMessage(CMessage* pMsg);			// ��ӦTeam������Ϣ
extern void OnPlayerMessage(CMessage* pMsg);
extern void OnOrgasysMessage(CMessage* pMsg);
extern void OnPlayerDetailSaveMessage(CMessage *pMsg);
extern void OnWriteLogMessage(CMessage* pMsg);				// ��Ӧ��־��Ϣ

extern void OnCountryMessage(CMessage *pMsg);
extern void OnLogServerMessage(CMessage *pMsg);
extern void OnMailMessage(CMessage* pMsg);
extern void OnLinkmanMan_DBS(CMessage *pMsg);
extern void OnLinkmanMan_GS(CMessage *pMsg);
extern void OnMail_GS(CMessage *pMsg);						//��ӦGS�ʼ���Ϣ
extern void OnPMMessage(CMessage *pMsg);				// ��Ӧ��ҹ�����Ϣ
extern void OnBusinessMsg( CMessage *pMsg );
extern void OnSMMessage(CMessage* pMsg);
extern void OnSentaiMessage(CMessage* pMsg);
extern void OnLotteryMessage(CMessage* pMsg);         //��Ӧ��Ʊϵͳ��Ϣ

CServer	*CMessage::m_pClientServer;
long	CMessage::m_lLogingSocket;
long	CMessage::m_lCostSocket;
long    CMessage::m_lDBSocket;
long    CMessage::m_lLogSocket;
int		CMessage::m_nGateSocket;

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

// //���͵�GS,DBS��Socket
long CMessage:: SendToSocket(long lSocketID,bool bClone)
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	//����һ����Ϣ
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
	m_pClientServer->ASend(lSocketID,pMsg);

	return true;
}

// //���͵�GS,DBS��Socket
long CMessage:: SendGSID(long lGSID,bool bClone)
{
	long lSocket = GetSocketByGSID(lGSID);
	SendToSocket(lSocket,bClone);
	return true;
}

// [������] ���͸����пͻ���
long CMessage::SendAll()
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	if( m_SocketGSID.size() == 0 )
		return true;

	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(m_SocketGSID.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();

	itLL it = m_SocketGSID.begin();
	for(;it != m_SocketGSID.end();it++)
	{
		m_pClientServer->ASend((*it).first,pMsg);
	}
	return true;
}

long CMessage::Send(bool bFront)
{
	assert(m_pClientServer);
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	//����һ����Ϣ
	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	m_pClientServer->ASend(m_lLogingSocket,pMsg);
	return 0;
}

void CMessage::SendToCost(bool bFront)
{	
	return;
}

long CMessage::GetGSID()
{
	return GetGSIDBySocket(m_lSocketID);
}
long CMessage::GetDBSID()
{
	return m_lDBSocket;
}
long CMessage::GetLOGSID()
{
	return m_lLogSocket;
}
long CMessage::GetLSID()
{
	return m_lLogingSocket;
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

long CMessage::SendToSM()
{
	GetInst(CMemClient).Send(this);
	return 0;
}

//ͬ�����͵�SM
long CMessage::SyncSendToSM()
{
	GetInst(CMemClient).SyncSend(this);
	return 0;
}

//���ļ�
void CMessage::AsyOpenFile(const char* pszFileName,bool bSync )
{
	CMessage MsgLog0x(MSG_MEM_OPEN_FILE);
	MsgLog0x.Add(pszFileName);
	if(bSync)
		MsgLog0x.SyncSendToSM();
	else
		MsgLog0x.SendToSM();
}
//�ر��ļ�
void CMessage::AsyCloseFile(const char* pszFileName,bool bSync )
{
	CMessage MsgLog0x(MSG_MEM_CLOSE_FILE);
	MsgLog0x.Add(pszFileName);
	if(bSync)
		MsgLog0x.SyncSendToSM();
	else
		MsgLog0x.SendToSM();
}

long CMessage::c_SmInfoOrder = 0;
//����ļ�����
void CMessage::AsyClearFile(const char* pszFileName,bool bSync)
{
	c_SmInfoOrder = 0;

	CMessage MsgLog0x(MSG_MEM_CLEAR_FILE);
	MsgLog0x.Add(pszFileName);
	if(bSync)
		MsgLog0x.SyncSendToSM();
	else
		MsgLog0x.SendToSM();
}

//�첽д�ļ��Ľӿ�
void CMessage::AsyWriteFile(const char* pszFileName,const char* pszContent,bool bSync)
{
	CMessage MsgLog0x(MSG_MEM_WRITE_FILE);
	MsgLog0x.Add(pszFileName);
	MsgLog0x.Add(c_SmInfoOrder++);
	MsgLog0x.Add(pszContent);
	if(bSync)
		MsgLog0x.SyncSendToSM();
	else
		MsgLog0x.SendToSM();
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
		}
		break;
	case BASEMSG_Socket_Close:
		{
			// [AHC 080809 DBSMSG]
			if(GetNetFlag()&NF_Server_Client)
			{
				if(GetGame()->OnGameServerClose(GetGSID()))
				{
					CMessage::UnMapGSIDSocket(GetSocketID());
				}
			}
			else if(GetNetFlag()&NF_DBServer_Client)
			{
				GetGame()->OnDBSLost();
				GetGame()->CreateConnectDBThread();
			}
			else if(GetNetFlag()&NF_LServer_Client)
			{
				GetGame()->OnLSLost();
				GetGame()->CreateConnectLoginThread();
			}
			else if(GetNetFlag()&NF_CServer_Client)
			{
			}
			else if(GetNetFlag()&NF_LogServer_Client)
			{
				GetGame()->OnLOGSLost();
			}
			else if(GetNetFlag()&NF_Gate_Client)
			{
				GetGame()->OnGateLost();
			}
		}
		break;
	}
	return true;
}

// ����������Ϣ����
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
	case MSG_S2W_RGN:
		OnRgnMessage(this);
		break;
	//----------------------------------------------------/������Ϣ/
	//case MSG_S2DB_OPER:
	case MSG_DB2W_OPER:
		OnDBOPMessage(this);
		break;
	//----------------------------------------------------/��������Ϣ/
	case MSG_W2W_SERVER:
	case MSG_S2W_SERVER:
	case MSG_L2W_SERVER:
	//[070716 AHC]
	case MSG_S2DB_SERVER:
	case MSG_W2DB_SERVER:
	case MSG_DB2S_SERVER:
	case MSG_DB2W_SERVER:
		OnServerMessage(this);
		break;
	//----------------------------------------------------/��½��Ϣ/
	case MSG_L2W_LOG:
	case MSG_S2W_LOG:
		OnLogMessage(this);
		break;
	case MSG_DB2W_LINKMAN:
		OnLinkmanMan_DBS(this);
		break;
	case MSG_S2W_LINKMAN:
		OnLinkmanMan_GS(this);
		break;
	//----------------------------------------------------/������Ϣ/
	case MSG_S2W_OTHER:
		OnOtherMessage(this);
		break;

	case MSG_S2W_PLAYER:
		OnPlayerMessage(this);
		break;

	case MSG_S2W_GM:
		OnGMMessage(this);
		break;

	//----------------------------------------------------/TEAM��Ϣ/
	case MSG_S2W_TEAM:
		OnGSTeamMessage(this);
		break;

	case MSG_S2W_ORGSYS:
		{
			OnOrgasysMessage(this);
		}
		break;
	case MSG_S2W_MAIL:
	case MSG_DB2W_MAIL:
		OnMailMessage(this);
		break;

	//----------------------------------------------------/��־��Ϣ/
	case MSG_S2W_WRITELOG:
		break;

		// ����
	case MSG_S2W_COUNTRY:
	case MSG_W2S_COUNTRY:
		OnCountryMessage(this);
		break;
	case MSG_LOG20_SERVER:
		{
			OnLogServerMessage(this);
		}
		break;

	case MSG_S2W_PLAYER_DETAIL_SAVE:
		{
			OnPlayerDetailSaveMessage(this);
		}
		break;

// Fox@20081107 ��ҹ���
	case MSG_S2W_PM:
		{
			OnPMMessage(this);
		}
		break;

	case MSG_S2W_BUSINESS:
		{
			OnBusinessMsg( this );
		}
		break;
	case MSG_SM2GAPP_OPERATOR:
		OnSMMessage(this);
		break;
	case MSG_S2W_SENTAI:
		OnSentaiMessage(this);
		break;
	case MSG_S2W_LOTTERY:
		OnLotteryMessage(this);
		break;
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMessage::Run() End.");
#endif
	return true;
}