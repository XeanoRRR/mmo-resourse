#include "StdAfx.h"
#include "Message.h"
#include "NetHandler.h"
#include "../Server/Game.h"
#include "../App/PhaseSystem/PhaseMap.h"
#include "../../../Public/Common/ManageToolMsgType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void OnRgnMessage(CMessage* msg);           // ��ӦRgn������Ϣ
extern void OnServerMessage(CMessage* msg);		   // ��ӦServer������Ϣ
extern void OnDBOPMessage(CMessage* msg);          // ��Ӧ���ݿ������Ϣ
extern void OnLogMessage(CMessage* msg);           // ��Ӧ��½��Ϣ
extern void OnOtherMessage(CMessage* msg);         // ��Ӧ������Ϣ
extern void OnGMMessage(CMessage* msg);            // ��ӦGM������Ϣ
extern void OnGSTeamMessage(CMessage* msg);        // ��ӦTeam������Ϣ
extern void OnPlayerMessage(CMessage* msg);
extern void OnOrgasysMessage(CMessage* msg);
extern void OnPlayerDetailSaveMessage(CMessage* msg);
extern void OnCountryMessage(CMessage* msg);
extern void OnLogServerMessage(CMessage* msg);
extern void OnMailMessage(CMessage* msg);
extern void OnLinkmanMan_DBS(CMessage* msg);
extern void OnLinkmanMan_GS(CMessage* msg);
extern void OnMail_GS(CMessage* msg);              // ��ӦGS�ʼ���Ϣ
extern void OnPMMessage(CMessage* msg);            // ��Ӧ��ҹ�����Ϣ
extern void OnBusinessMsg(CMessage* msg);
extern void OnSMMessage(CMessage* msg);
extern void OnAHMsg( CMessage *msg );
extern void OnQuestMessage(CMessage* msg);

CMessage::Clients CMessage::s_GSs;
CMessage::Clients CMessage::s_GSIDs;
CServer* CMessage::s_server;

CMessage::CMessage()
{
    m_socket = 0;
	m_IP = 0;
	m_recvTime = 0;
}

CMessage::CMessage(uint type)
{
	m_socket = 0;
	m_IP =0;
	m_recvTime = 0;

    Init(type);
}

CMessage::~CMessage()
{
	UnInit();
}

//������Ϣ��ر���
void CMessage::Reset()
{
	m_socket = 0;
	m_IP = 0;
}

bool CMessage::Initial(CDataBlockAllocator* allocator, uint maxFreeMsgNum)
{
	s_GSs.clear();
	s_GSIDs.clear();

	CBaseMessage::NewMsg = NewMessage;
	return CBaseMessage::Initial(allocator, maxFreeMsgNum);
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

bool CMessage::IsGSReady(uint id)
{
    if (s_GSIDs.empty()) return false;
    if (0 == id) return true;
    if (-1 == id) return GetGSNum() == CMessage::GetGSNum();
    if (s_GSIDs.find(id) != s_GSIDs.end()) return true; 
    return false;
}

void CMessage:: SendToSocket(int socket, bool clone)
{
	assert(s_server);
	*((long*)Base()) = GetSize() + HEAD_SIZE;
	CBaseMessage* msg = CBaseMessage::AllocMsg();
	if (!clone)
	{
		vector<CDataBlock*>& msgData = GetMsgData();
		msg->SetMsgData(msgData);
		msgData.clear();
	}
	else
	{
		Clone(msg);
	}
	s_server->ASend(socket, msg);
}

void CMessage::SendToGS(uint id, bool clone)
{
	int socket = GetSocketByGSID(id);
	SendToSocket(socket, clone);
}

void CMessage::SendToAllGS()
{
	assert(s_server);
    if( s_GSs.empty() ) return;

	*((long*)Base()) = GetSize() + HEAD_SIZE;

	CBaseMessage* msg = AllocMsg();
	msg->SetRefCount(long(s_GSs.size()));
	vector<CDataBlock*>& msgData = GetMsgData();
	msg->SetMsgData(msgData);
	msgData.clear();

	ClientItr itr = s_GSs.begin();
	for (; itr != s_GSs.end(); ++itr)
	{
		s_server->ASend(itr->first, msg);
	}
}

void CMessage::SendToServer(NetFlag flag, bool prior)
{
    SendToSocket(GetServerSocket(flag));
}

uint CMessage::GetGSID()
{
	return GetGSIDBySocket(m_socket);
}

int CMessage::GetServerSocket(NetFlag flag)
{
    ServerConnection* server = GetNetHandler().FindServer(flag);
    if (!server) return -1;
	return server->con.socket;
}

void CMessage::MapGSIDSocket(uint id, int socket)
{
	s_GSs[socket] = id;
	s_GSIDs[id]= socket;
}

int CMessage::GetSocketByGSID(uint id)
{
	ClientItr itr = s_GSIDs.find(id);
	if (itr != s_GSIDs.end())
		return itr->second;

	return -1;
}

uint CMessage::GetGSIDBySocket(int socket)
{
	ClientItr itr = s_GSs.find(socket);
	if (itr != s_GSs.end())
		return itr->second;

	return 0;
}

void CMessage::UnMapSocket(int socket, NetFlag flag)
{
	if (flag & NF_GAME_SERVER)
	{
		UnMapGSIDSocket(socket);
	}
}

void CMessage::UnMapGSIDSocket(int socket)
{
	ClientItr itr = s_GSs.find(socket);
	if(itr != s_GSs.end() )
	{
		s_GSIDs.erase(itr->second);
		s_GSs.erase(itr);
	}
}

void CMessage::SendToSM()
{
	GetInst(CMemClient).Send(this);
}

//���ļ�
void CMessage::AsyOpenFile(const char* file)
{
	CMessage MsgLog0x(MSG_MEM_OPEN_FILE);
	MsgLog0x.Add(file);
	MsgLog0x.SendToSM();
}

//�ر��ļ�
void CMessage::AsyCloseFile(const char* file)
{
	CMessage MsgLog0x(MSG_MEM_CLOSE_FILE);
	MsgLog0x.Add(file);
	MsgLog0x.SendToSM();
}
//����ļ�����
void CMessage::AsyClearFile(const char* file)
{
	CMessage MsgLog0x(MSG_MEM_CLEAR_FILE);
	MsgLog0x.Add(file);
	MsgLog0x.SendToSM();
}

//�첽д�ļ��Ľӿ�
void CMessage::AsyWriteFile(const char* file, const char* content)
{
	CMessage MsgLog0x(MSG_MEM_WRITE_FILE);
	MsgLog0x.Add(file);
	MsgLog0x.Add(content);
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
        NetFlag flag = (NetFlag)GetNetFlag();
        if (flag & NF_GAME_SERVER)
        {
            GetNetHandler().OnClientLost(GetGSID());
            CMessage::UnMapGSIDSocket(GetSocketID());
        }
        else
        {
            GetNetHandler().OnServerLost(flag);
        }
    }
    break;
	}

	return 0;
}

// ����������Ϣ����
long CMessage::Run()
{
#ifdef _RUNSTACKINFO_
	// �������ջ���
	char stackInfo[256] = "";
	_snprintf(stackInfo,
              256,
              "CMessage::Run(msgtype:%d) Start.",
              GetType());
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),
                           stackInfo);
#endif

	// ��Ϣ���ദ��
	switch (GetType() - (GetType() & 0x00000ff))
	{
    case 0:
        DefaultRun();
        break;

    case MSG_S2W_RGN:
        OnRgnMessage(this);
        break;

        //----------------------------------------------------/������Ϣ/
    case MSG_DB2W_OPER:
        OnDBOPMessage(this);
        break;

        //----------------------------------------------------/��������Ϣ/
    case MSG_W2W_SERVER:
    case MSG_S2W_SERVER:
    case MSG_L2W_SERVER:
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

    case MSG_S2W_PHASESYSTEM:
        GetInst(PhaseMap).OnMessage(this);
        break;
    case MSG_S2W_AH:
        {
            OnAHMsg( this );
        }
        break;
    case MSG_S2W_QUEST:
        {
            OnQuestMessage(this);
        }
        break;
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),
                           "CMessage::Run() End.");
#endif

	return 0;
}
