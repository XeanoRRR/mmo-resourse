#include "stdafx.h"
#include "message.h"
#include "../../../Public/Common/ManageToolMsgType.h"


extern void OnServerMessage(CMessage* pMsg);			// ��ӦServer������Ϣ
extern void OnLogMessage(CMessage* pMsg);				// ��Ӧ��½��Ϣ
extern void OnOtherMessage(CMessage* pMsg);				// ��Ӧ������Ϣ
extern void OnGMMessage(CMessage* pMsg);
extern void OnASMessage(CMessage* pMsg);				//! ��ӦAccountServer��Ϣ
extern void OnSMMessage(CMessage* pMsg);


CServer *CMessage::m_pClientServer	= NULL;
CServer *CMessage::m_pWorldServer	= NULL;
long	CMessage::m_lASSocket		= 0;;

CMessage::HMapLongString	CMessage::m_SocketCDkey;
CMessage::HMapStringLong	CMessage::m_CDkeySocket;

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
	m_SocketCDkey.clear();
	m_CDkeySocket.clear();
	m_SocketWorldID.clear();
	m_WorldIDSocket.clear();

	CBaseMessage::NewMsg = NewMessage;
	return CBaseMessage::Initial(pDBAllocator,nMaxFreeMsgNum);
}

void CMessage::SetClientServer(CServer *pClientServer)
{
	m_pClientServer = pClientServer;
}
void CMessage::SetWorldServer(CServer *pWorldServer)
{
	m_pWorldServer = pWorldServer;
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

//����һ����Ϣ��WorldServer�׽���
long CMessage::SendToWorld(long lSocketID)
{
	assert(m_pWorldServer);
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	m_pWorldServer->ASend(lSocketID,pMsg);
	return true;
}

//���͸����пͻ���
long CMessage::SendAllClient()
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	itLS it = m_SocketCDkey.begin();
	for(;it != m_SocketCDkey.end();it++)
	{
		CBaseMessage *pMsg = CBaseMessage::AllocMsg();
		Clone(pMsg);
		m_pClientServer->ASend((*it).first,pMsg);
	}
	return true;
}

//���͸�����WorldServer
long CMessage::SendAllWorld()
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	itLL it = m_SocketWorldID.begin();
	for(;it != m_SocketWorldID.end();it++)
	{
		CBaseMessage *pMsg = CBaseMessage::AllocMsg();
		Clone(pMsg);
		m_pWorldServer->ASend((*it).first,pMsg);
	}
	return true;
}

long CMessage::SendToWorldNumID(long lWorldID)
{
	long lSocket = GetSocketByWorldID(lWorldID);
	SendToWorld(lSocket);
	return true;
}

long CMessage::SendToClient(const char *szCdkey)
{
	long lSocket = GetSocketByCdkey(szCdkey);
	SendToClient(lSocket);
	return true;
}

//! [��ΪAS�ͻ���] ������Ϣ
BOOL CMessage::SendToAS(VOID)
{
	if(0 == m_lASSocket) return FALSE;
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	m_pWorldServer->ASend(m_lASSocket,pMsg);
	return true;
}

const char *CMessage::GetCdkey()
{
	return CMessage::GetCDkeyBySocket(m_lSocketID);
}

long CMessage::GetWorldID()
{
	return CMessage::GetWorldIDBySocket(m_lSocketID);
}

//ӳ��CDkey��SocketID
void CMessage::MapCDkeySocket(const char* szCdkey,long lSocketID)
{
	if(NULL == szCdkey || lSocketID <= 0)	return;
	m_CDkeySocket[szCdkey] = lSocketID;
	m_SocketCDkey[lSocketID] = szCdkey;
}
long CMessage::GetSocketByCdkey(const char* szCdkey)
{
	if(NULL == szCdkey)	return 0;

	itSL  it = m_CDkeySocket.find(szCdkey);
	if(it != m_CDkeySocket.end() )
		return (*it).second;
	return 0;
}

const char* CMessage::GetCDkeyBySocket(long lSocketID)
{
	itLS it = m_SocketCDkey.find(lSocketID);
	if(it != m_SocketCDkey.end())
		return (*it).second.c_str();
	return NULL;
}

void CMessage::UnCDkeyBySocket(const char* szCdkey)
{
	if(NULL == szCdkey)	return;

	itSL  it = m_CDkeySocket.find(szCdkey);
	if(it != m_CDkeySocket.end() )
	{
		LONG lSocket = (*it).second;
		m_SocketCDkey.erase(lSocket);
		m_CDkeySocket.erase(it);
	}
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
	if(lNetFlag&NF_Server_Client)
	{
		UnMapCDkeySocket(lSocketID);
	}
	else if(lNetFlag&NF_Server_World)
	{
		UnMapWorldIDSocket(lSocketID);
	}
}

void CMessage::UnMapCDkeySocket(long lSocketID)
{
	itLS it =  m_SocketCDkey.find(lSocketID);
	if(it != m_SocketCDkey.end() )
	{
		m_CDkeySocket.erase((*it).second);
		m_SocketCDkey.erase(it);
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
			if(GetNetFlag()&NF_Server_World)
			{
				Log4c::Trace(ROOT_MODULE,FormatText("LOGIN_MSG_0",GetSocketID(),GetStringIP()));//WorldServer[Socket:%d,IP:%s] Connect
				//GetGame()->OnNetWorldClose(GetWorldID());
				//CMessage::UnMapWorldIDSocket(GetSocketID());
			}
		}
		break;
	case BASEMSG_Socket_Close:
		{
			if(GetNetFlag()&NF_Server_Client)
			{
				GetGame()->GetCSdoaValidate().OnClientDisConn(GetSocketID());
				char* szCdkey = (char*)GetCdkey();
				if(szCdkey)
				{
					map<string, long>::iterator itr = GetGame()->GetPassDetailDataCdkeyMap().find(szCdkey);
					if(itr != GetGame()->GetPassDetailDataCdkeyMap().end()) // �ǵ�¼��Ϸ��Ͽ����������
					{
						// �����¼��ҵ�cdkey
						if(szCdkey)
							GetGame()->GetPassDetailDataCdkeyMap().erase(string(szCdkey));			
					}
					GetGame()->OnNetClientClose(szCdkey, GetSocketID());
				}	
				CMessage::UnMapCDkeySocket(GetSocketID());
			}
			else if(GetNetFlag()&NF_Server_World)
			{			
				Log4c::Trace(ROOT_MODULE,FormatText("LOGIN_MSG_1",GetSocketID(),GetStringIP()));//WorldServer[Socket:%d,IP:%s] Close
			
				GetGame()->OnNetWorldClose(GetWorldID());
				CMessage::UnMapWorldIDSocket(GetSocketID());
			}
			else if(GetNetFlag()&NF_AsscountServer_Client)
			{
				GetGame()->OnNetASClose();
				m_lASSocket = 0;
			}
		}
		break;
	}
	return true;
}

long CMessage::SendToSM()
{
	GetInst(CMemClient).Send(this);
	return 0;
}
//���ļ�
void CMessage::AsyOpenFile(const char* pszFileName)
{
	CMessage MsgLog0x(MSG_MEM_OPEN_FILE);
	MsgLog0x.Add(pszFileName);
	MsgLog0x.SendToSM();
}
//�ر��ļ�
void CMessage::AsyCloseFile(const char* pszFileName)
{
	CMessage MsgLog0x(MSG_MEM_CLOSE_FILE);
	MsgLog0x.Add(pszFileName);
	MsgLog0x.SendToSM();
}
//����ļ�����
void CMessage::AsyClearFile(const char* pszFileName)
{
	CMessage MsgLog0x(MSG_MEM_CLEAR_FILE);
	MsgLog0x.Add(pszFileName);
	MsgLog0x.SendToSM();
}

//�첽д�ļ��Ľӿ�
void CMessage::AsyWriteFile(const char* pszFileName,const char* pszContent)
{
	CMessage MsgLog0x(MSG_MEM_WRITE_FILE);
	MsgLog0x.Add(pszFileName);
	MsgLog0x.Add(pszContent);
	MsgLog0x.SendToSM();
}

// ��Ϣ����
long CMessage::Run()
{
#ifdef _RUNSTACKINFO_
	//�������ջ���
	char pszStackInfo[256] = {0};
	_snprintf(pszStackInfo,256,"CMessage::Run(msgtype:%d) Start.",GetType());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif

	if( !Validate() )
	{
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMessage::Run() End.the msg format error.");
#endif
		return false;
	}

//������Ϣ����ȷ��
	u_long lType = GetType();
	if((lType - (lType & 0x00000ff) != 0) && (GetNetFlag()&NF_Server_Client) )
	{
		if( lType <= MSG_C2L || lType >= MSG_W2W)
		{
#ifdef _RUNSTACKINFO_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMessage::Run() End.the msg type error.");
#endif
			return false;
		}
	}
	// ��Ϣ���ദ��
	switch( lType - (lType & 0x00000ff) )	// GetType() - GetType() % 256
	{
	case 0:
		DefaultRun();
		break;
	//----------------------------------------------------/Server��Ϣ/
	case MSG_L2L_SERVER:
	case MSG_W2L_SERVER:
		OnServerMessage(this);
		break;

	//----------------------------------------------------/��½��Ϣ/
	case MSG_L2L_LOG:
	case MSG_C2L_LOG:
	case MSG_W2L_LOG:
		OnLogMessage(this);
		break;

	//----------------------------------------------------/������Ϣ/
//	case MSG_OTHER:
//		OnOtherMessage(this);

		break;
	case MSG_W2L_GM:
		OnGMMessage(this);
		break;
	case MSG_A2O_LOGIN:
	case MSG_A2O_ACCOUNT:
		OnASMessage(this);
		break;
	case MSG_SM2GAPP_OPERATOR:
		OnSMMessage(this);
		break;
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMessage::Run() End.");
#endif
	return true;
}

