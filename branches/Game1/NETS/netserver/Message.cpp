#include "stdafx.h"
#include "message.h"
#include "nets\Servers.h"
#include "nets\MemClient.h"
#include "server\gameserver\appserver\player.h"
#include "server\gameserver\appserver\serverregion.h"

#include "server\gameserver\appserver\OrganizingSystem\GSTeam.h"
#include "server\gameserver\appserver\OrganizingSystem\GameOrganizingCtrl.h"

#include "..\server\gameserver\appserver\area.h"
#include "..\public\Crc32Static.h"
#include "..\public\Singleton.h"
#include "..\server\gameserver\appserver\OrganizingSystem\GameUnion.h"
#include "..\server\gameserver\appserver\OrganizingSystem\GameFaction.h"
#include "..\server\gameserver\appserver\OrganizingSystem\GameOrganizingCtrl.h"
#include "..\server\gameserver\appserver\Session\CSessionFactory.h"
#include "..\server\gameserver\appserver\Session\CSession.h"
#include "..\server\gameserver\appserver\Session\CPlug.h"

#include <mmsystem.h>



extern void OnRgnMessage(CMessage* pMsg);					// ��ӦRgn��Ϣ
extern void OnServerMessage(CMessage* pMsg);				// ��ӦServer������Ϣ
extern void OnLogMessage(CMessage* pMsg);					// ��Ӧ��½��Ϣ
extern void OnOtherMessage(CMessage* pMsg);					// ��Ӧ������Ϣ
extern void OnGMMessage(CMessage* pMsg);					// ��ӦGM������Ϣ
extern void OnGoodsMessage(CMessage* pMsg);					// ��Ӧ��Ʒ��Ϣ
//extern void OnTeamMessage(CMessage* pMsg);				// ��Ӧ�����Ϣ
extern void OnShopMessage(CMessage* pMsg);
extern void OnIncrementShopMessage(CMessage* pMsg);
//extern void OnDepotMessage(CMessage* pMsg);
extern void OnSkillMessage(CMessage* pMsg);
extern void OnStateMessage(CMessage* pMsg);
extern void OnOrgasysMessage(CMessage* pMsg);
extern void OnPlayerShopMessage(CMessage* pMsg);
extern void OnContainerMessage(CMessage* pMsg);
extern void OnPetMessage(CMessage* pMsg);
extern void OnCountryMessage(CMessage *pMsg);
extern void OnPlayerDetailSave(CMessage *pMsg);

extern void OnBillingMessage(CMessage* pMsg);

//! [2007-7-19 add] ��ӦGameServer�����Ϣ 
extern void OnWSTeamMessage(CMessage *pMsg);
//! [2007-7-19 add] ��ӦClient�����Ϣ
extern void OnClientTeamMessage(CMessage *pMsg);

extern void OnLogServerMessage(CMessage *pMsg);

extern void OnLinkmanMessage_ForWs(CMessage *pMsg);			//! ��Ӧ�������������ϵ����Ϣ
extern void OnLinkmanMessage_ForClient(CMessage *pMsg);		//! ��Ӧ�ͻ��˵���ϵ����Ϣ
extern void OnSpriteMessage_ForClient(CMessage *pMsg);		//! ��Ӧ�ͻ��˵�С������Ϣ
extern void OnMailMessageForClient(CMessage *pMsg);		//! ��Ӧ�ͷ����ʼ���Ϣ
extern void OnMailMessageForWs(CMessage *pMsg);			//! ��ӦWS�ʼ���Ϣ
extern void OnPMMessage(CMessage *pMsg);				// ��Ӧ��ҹ�����Ϣ
extern void OnAntiCheatMsg(CMessage *pMsg);				/// @�����: ��ȡ�ͻ��˵���Ϣ
extern void OnSMMessage(CMessage* pMsg);
extern void OnBounsMessage(CMessage *pMsg);				// ������Ϣ����
extern void OnWsSentaiMessage(CMessage *pMsg);
extern void OnLotteryMessage(CMessage* pMsg);

CServer	*CMessage::m_pServer;
long	CMessage::m_lWSSocket;
long	CMessage::m_lDBSSocket;
long	CMessage::m_lLogSocket;
long	CMessage::m_lBSSocket;
long	CMessage::m_lBounsSocket;

CMessage::HMapLongGuid	CMessage::m_SocketPlayerID;
CMessage::HMapGuidLong	CMessage::m_PlayerIDSocket;
CMessage::Sockets		CMessage::m_TransCongSokcets;

CMessage::CMessage()
{
	m_pRegion = NULL;
	m_pPlayer = NULL;
	m_lSocketID=0;
	m_dwIP=0;

}

CMessage::CMessage(long lType)
:m_guDiscardFlag(NULL_GUID)
{

	m_pRegion = NULL;
	m_pPlayer = NULL;
	m_lSocketID=0;
	m_dwIP=0;
	Init(lType);
}

CMessage::CMessage(long lType,const CGUID& guDiscardFlag)
:m_guDiscardFlag(guDiscardFlag)
{
	m_pRegion = NULL;
	m_pPlayer = NULL;
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
	m_pRegion = NULL;
	m_pPlayer = NULL;
	m_lSocketID=0;
	m_dwIP=0;
	m_guDiscardFlag = NULL_GUID;
}

bool CMessage::Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum)
{
	m_SocketPlayerID.clear();
	m_PlayerIDSocket.clear();

	CBaseMessage::NewMsg = NewMessage;
	return CBaseMessage::Initial(pDBAllocator,nMaxFreeMsgNum);
}
bool CMessage::Release()
{
	return CBaseMessage::Release();
}

CBaseMessage* CMessage::NewMessage()
{
	return new CMessage();
}


//����һ����Ϣ
CMessage* CMessage::AllocMessage()
{
	return static_cast<CMessage*>(CBaseMessage::AllocMsg());
}

//�ж�������Ϣ�Ķ�������Ƿ�һ��
bool CMessage::IsDiscardFlagEqual(CBaseMessage* pMsg)
{
	CMessage *pTemptMsg = static_cast<CMessage*>(pMsg);
	if(pMsg && m_guDiscardFlag != NULL_GUID)
	{
		return (m_guDiscardFlag == pTemptMsg->GetDiscardFlag());
	}
	return false;
}

//����һ����Ϣ���׽���
long CMessage::SendToSocket(long lSocketID,bool bClone)
{
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	if(lSocketID>0)
	{
	//����һ����Ϣ
	CMessage *pMsg = AllocMessage();
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
	m_pServer->ASend(lSocketID,pMsg);
	}
	return true;
}

long CMessage::SendToBS()
{
	if(m_lBSSocket>0)
	{
		return SendToSocket(m_lBSSocket,false);
	}
	return false;
}

long CMessage::SendToBounsServer()
{
	if(m_lBounsSocket > 0)
	{
		return SendToSocket(m_lBounsSocket, false);
	}
	return 0;
}

// ����һ����Ϣ�����
long CMessage::SendToPlayer(const CGUID& PlayerGuid,bool bClone)
{
	long lSocketID = GetSocketByPlayerID(PlayerGuid);
	SendToSocket(lSocketID,bClone);
	return TRUE;
}

// [������] ���͸�һ���������������(pExceptionPlayer=����)
long CMessage::SendToRegion(CServerRegion* pRegion, CPlayer* pExceptionPlayer)
{
	if( pRegion == NULL )
	{
		//assert(0);
		return 0;
	}

	*((long*)Base()) = GetSize()+HEAD_SIZE;

	hashsetGUID setPlayers;setPlayers.clear();
	pRegion -> FindAllPlayer(setPlayers );	
	list<long>	SocketList;SocketList.clear();
	hsGUIDIt itSet = setPlayers.begin();
	for(; itSet != setPlayers.end(); itSet ++ )
	{
		if( (*itSet)  != NULL_GUID && ( pExceptionPlayer == NULL || (*itSet) != pExceptionPlayer->GetExID() ) )
		{
			long lSocketID = GetSocketByPlayerID((*itSet));
			if(lSocketID != 0)
			{
				SocketList.push_back(lSocketID);
			}
		}
	}

	if(SocketList.size() == 0 )
		return 1;
	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(SocketList.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	list<long>::iterator it = SocketList.begin();
	for(;it != SocketList.end();it++)
	{
		m_pServer->ASend((*it),pMsg);
	}
	return 1;
}

// ���͸�һ��AREA���������
long CMessage::SendToArea(CArea* pArea, CPlayer* pExceptionPlayer)
{
	if( pArea == NULL )
	{
		//assert(0);
		return 0;
	}
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	hashsetGUID setPlayers;
	pArea -> FindShapes( TYPE_PLAYER, setPlayers );
	list<long>	SocketList;SocketList.clear();
	for( hsGUIDIt it = setPlayers.begin(); it != setPlayers.end(); it++ )
	{
		if( (*it)  != NULL_GUID && ( pExceptionPlayer == NULL || (*it) != pExceptionPlayer->GetExID() ) )
		{			
			long lSocketID = GetSocketByPlayerID((*it));
			if(lSocketID != 0)
			{
				SocketList.push_back(lSocketID);
			}
		}
	}

	if(SocketList.size() == 0 )
		return 1;

	CBaseMessage *pMsg = AllocMsg();
	Clone(pMsg);
	pMsg->SetRefCount(SocketList.size());
	list<long>::iterator it = SocketList.begin();
	for(;it != SocketList.end();it++)
	{
		m_pServer->ASend((*it),pMsg);
	}
	return 1;
}

// [������] ���͸�һ��MOVESHAPE��Χ�����(pExceptionPlayer=����)
//bExceptTranCongSocket:�Ƿ��ų�����ӵ����Socket
long CMessage::SendToAround(CShape* pMain,CPlayer* pExceptionPlayer,
							bool bExceptTranCongSocket)
{
	if( pMain == NULL )
	{
		//assert(0);
		return 0;
	}

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(pMain->GetFather());
	return SendToAround(pRegion,pMain->GetTileX(), pMain->GetTileY(), pMain,
		pExceptionPlayer,bExceptTranCongSocket);
}

long CMessage::SendToAroundTeamate(CShape* pMain,CShape* pDest)
{
	//if( pMain == NULL )
	//{		
	//	return 0;
	//}
	//CServerRegion* pRegion = dynamic_cast<CServerRegion*>(pMain->GetFather());
	//if (pRegion==NULL)
	//{
	//	return 0;
	//}

	//int x = pMain->GetTileX();
	//int y = pMain->GetTileY();

	//*((long*)Base()) = GetSize()+HEAD_SIZE;
	//static int area[9][2] = {-1,-1,	0,-1,	1,-1,
	//	-1,0,	0,0,	1,0,
	//	-1,1,	0,1,	1,1};

	//x /= AREA_WIDTH;
	//y /= AREA_HEIGHT;

	//list<CShape*> lDestShape;
	//if (pMain->GetType()==TYPE_PLAYER)
	//{
	//	lDestShape.push_back(pMain);
	//}
	//else if (pDest!=NULL && pDest->GetType()==TYPE_PLAYER)
	//{
	//	lDestShape.push_back(pDest);
	//}

	//for (list<CShape*>::iterator it=lDestShape.begin(); it!=lDestShape.end(); ++it)
	//{
	//	CShape *pDestShape = (*it);
	//	list<CGUID> lMemberList;
	//	int nNum = 0;	
	//	GSTeam *pGSTeam = (GSTeam*)(GetOrganiCtrl()->GetGameTeamByID(((CPlayer*)pDestShape) -> GetTeamID()));

	//	if (pGSTeam)
	//	{
	//		pGSTeam->GetMemberList(lMemberList);		
	//	}
	//	else
	//	{
	//		lMemberList.push_back(pDestShape->GetExID());
	//	}

	//	vector<CGUID> vPlayers;
	//	for(int i=0; i<9; i++)
	//	{
	//		CArea* pArea = pRegion->GetArea(x+area[i][0], y+area[i][1]);
	//		if( pArea )
	//		{
	//			pArea -> FindShapes( TYPE_PLAYER, vPlayers );
	//			for(size_t j=0; j<vPlayers.size(); ++j)
	//			{
	//				if( vPlayers[j] != NULL_GUID)
	//				{
	//					for(list<CGUID>::iterator it=lMemberList.begin();it!=lMemberList.end(); ++it)
	//					{
	//						if (vPlayers[j]==(*it))
	//						{
	//							long lSocketID = GetSocketByPlayerID(vPlayers[j]);
	//							if(lSocketID != 0)
	//							{
	//								SocketList.push_back(lSocketID);
	//								//CMessage *pMsg = AllocMessage();
	//								//Clone(pMsg);
	//								//m_pServer->ASend(lSocketID,pMsg);
	//							}
	//						}							
	//					}
	//				}
	//			}
	//		}
	//	}

	//}
	//CBaseMessage *pMsg = AllocMsg();
	//pMsg->AddRefCount(SocketList.size());
	//vector<CDataBlock*>& MsgData = GetMsgData();
	//pMsg->SetMsgData(MsgData);
	//MsgData.clear();
	//list<long>::iterator it = SocketList.begin();
	//for(;it != SocketList.end();it++)
	//{
	//	m_pServer->ASend((*it),pMsg);
	//}
	return 1;
}

// [������] ���͸�һ������Χ�����(pExceptionPlayer=����)
long CMessage::SendToAround(CServerRegion* pRegion,int x, int y, CShape* pMain,
							CPlayer* pExceptionPlayer,bool bExceptTranCongSocket)
{
	if (pRegion==NULL)
	{
		return 0;
	}

	*((long*)Base()) = GetSize()+HEAD_SIZE;
	static int area[9][2] = {-1,-1,	0,-1,	1,-1,
		-1,0,	0,0,	1,0,
		-1,1,	0,1,	1,1};

	x /= AREA_WIDTH;
	y /= AREA_HEIGHT;
	hashsetGUID setPlayers;
	list<long>	SocketList;SocketList.clear();
	for(int i=0; i<9; i++)
	{
		CArea* pArea = pRegion->GetArea(x+area[i][0], y+area[i][1]);
		if( pArea )
		{
			pArea -> FindShapes( TYPE_PLAYER, setPlayers );
			for( hsGUIDIt it = setPlayers.begin(); it != setPlayers.end(); it++ )
			{
				if( (*it)  != NULL_GUID && ( pExceptionPlayer == NULL || (*it) != pExceptionPlayer->GetExID() ) )
				{
					long lSocketID = GetSocketByPlayerID((*it));
					if(lSocketID != 0)
					{
						SocketList.push_back(lSocketID);
					}
				}
			}
		}
	}
	//�ų�����ӵ�����׽���
	if(bExceptTranCongSocket)
		RemoveTransCongSockets(SocketList);
	if(SocketList.size() == 0 )
		return 1;
	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(SocketList.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	list<long>::iterator it = SocketList.begin();
	for(;it != SocketList.end();it++)
	{
		m_pServer->ASend((*it),pMsg);
	}
	return 1;
}


//��ָ�������������ҷ�����Ϣ
//bExceptTranCongSocket:�Ƿ��ų�����ӵ����Socket
long CMessage::SendAroundLimitPlayer(CMoveShape* pMoveShape,const CGUID& guidExeptID,bool bExceptTranCongSocket)
{
	static long lMaxSendNum = 8;
	if( !pMoveShape )	return 0;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(pMoveShape->GetFather());
	if( !pRegion )	return 0;

	*((long*)Base()) = GetSize()+HEAD_SIZE;
	hashsetGUID hsPlayers;
	pRegion->FindLimitPlayersAround(pMoveShape,CGlobeSetup::GetSetup()->lSendMsgLimitPlayerNum,hsPlayers,guidExeptID);

	SendToPlayers(hsPlayers,bExceptTranCongSocket);

	/*list<long>	SocketList;SocketList.clear();
	vector<CGUID>::iterator it = vPlayerID.begin();
	for( ; it != vPlayerID.end(); it++ )
	{
		if( (*it)  != NULL_GUID && ( pExceptionPlayer == NULL || (*it) != pExceptionPlayer->GetExID() ) )
		{
			long lSocketID = GetSocketByPlayerID((*it));
			if(lSocketID != 0)
			{
				SocketList.push_back(lSocketID);
			}
		}
	}
	if(SocketList.size() == 0 )
		return 1;
	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(SocketList.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	list<long>::iterator itSocket = SocketList.begin();
	for(;itSocket != SocketList.end();itSocket++)
	{
		m_pServer->ASend((*itSocket),pMsg);
	}*/
	return 1;
}
//��ָ�����������ҷ�����Ϣ
//bExceptTranCongSocket:�Ƿ��ų�����ӵ����Socket
long CMessage::SendToNearestPlayer(CShape* pMain,bool bExceptTranCongSocket,CPlayer* pExceptionPlayer)
{
	static long lDistance = 8;
	if( !pMain )	return 0;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(pMain->GetFather());
	if( !pRegion )	return 0;

	*((long*)Base()) = GetSize()+HEAD_SIZE;
	vector<CGUID> vPlayerID;
	pRegion->FindNearestPlayer(pMain->GetTileX(),pMain->GetTileY(),CGlobeSetup::GetSetup()->lSendMsgLimitDistance,vPlayerID);

	list<long>	SocketList;SocketList.clear();
	vector<CGUID>::iterator it = vPlayerID.begin();
	for( ; it != vPlayerID.end(); it++ )
	{
		if( (*it)  != NULL_GUID && ( pExceptionPlayer == NULL || (*it) != pExceptionPlayer->GetExID() ) )
		{
			long lSocketID = GetSocketByPlayerID((*it));
			if(lSocketID != 0)
			{
				SocketList.push_back(lSocketID);
			}
		}
	}
	//�ų�����ӵ�����׽���
	if(bExceptTranCongSocket)
		RemoveTransCongSockets(SocketList);
	if(SocketList.size() == 0 )
		return 1;
	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(SocketList.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	list<long>::iterator itSocket = SocketList.begin();
	for(;itSocket != SocketList.end();itSocket++)
	{
		m_pServer->ASend((*itSocket),pMsg);
	}
	return 1;
}

//������Ϣ��ָ�����
//bExceptTranCongSocket:�Ƿ��ų�����ӵ����Socket
long CMessage::SendToPlayers(hash_set<CGUID,hash_guid_compare>& hsPlayers,bool bExceptTranCongSocket)
{
	if(hsPlayers.size() == 0)	return 0;

	*((long*)Base()) = GetSize()+HEAD_SIZE;
	list<long>	SocketList;SocketList.clear();
	hash_set<CGUID,hash_guid_compare>::iterator it = hsPlayers.begin();
	for(;it != hsPlayers.end();it++)
	{
		long lSocketID = GetSocketByPlayerID((*it));
		if(lSocketID != 0)
		{
			SocketList.push_back(lSocketID);
		}
	}
	//�ų�����ӵ�����׽���
	if(bExceptTranCongSocket)
		RemoveTransCongSockets(SocketList);
	if(SocketList.size() == 0 )
		return 1;
	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(SocketList.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	list<long>::iterator itSocket = SocketList.begin();
	for(;itSocket != SocketList.end();itSocket++)
	{
		m_pServer->ASend((*itSocket),pMsg);
	}
	return 1;
}

//���б����ų������������׽���
void CMessage::RemoveTransCongSockets(list<long>&	SocketList)
{
	if(m_TransCongSokcets.size() == 0)	return;

	list<long>::iterator it = SocketList.begin();
	for(;it != SocketList.end(); )
	{
		itTCSocket itTC = m_TransCongSokcets.find((*it));
		if(itTC != m_TransCongSokcets.end() )
			it = SocketList.erase(it);
		else
			it++;
	}
}

// [������] ���͸����пͻ���
long CMessage::SendAll( )
{
	if(m_SocketPlayerID.size() == 0)
		return true;
	*((long*)Base()) = GetSize()+HEAD_SIZE;

	CBaseMessage *pMsg = AllocMsg();
	pMsg->SetRefCount(m_SocketPlayerID.size());
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	iteLG it = m_SocketPlayerID.begin();
	for(;it != m_SocketPlayerID.end();it++)
	{
		//CMessage *pMsg = AllocMessage();
		//Clone(pMsg);
		m_pServer->ASend((*it).first,pMsg);
	}
	return true;
}

// [�ͻ���] ������Ϣ
long CMessage::Send(bool bFront)
{	
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	//����һ����Ϣ
	CMessage *pMsg = AllocMessage();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);

	MsgData.clear();
	m_pServer->ASend(m_lWSSocket,pMsg);
	return true;
}

const CGUID& CMessage::GetPlayerID()
{
	return GetPlayerIDBySocket(m_lSocketID);
}


//ӳ��WorldID��SocketID
void CMessage::MapPlayerIDSocket(const CGUID& PlayerGuid,long lSocketID)
{
	char pszGuid[50]="";
	PlayerGuid.tostring(pszGuid);
	m_SocketPlayerID[lSocketID]=PlayerGuid;
	m_PlayerIDSocket[PlayerGuid]=lSocketID;

	//char pszLogingInfo[512]="";
	//_snprintf(pszLogingInfo,512,"Map the Socket[%d] to PlayerID[%s]",lSocketID,pszGuid);
	//PutStringToFile("Login_GS_Info",pszLogingInfo);
}
long CMessage::GetSocketByPlayerID(const CGUID& PlayerGuid)
{
	iteGL it = m_PlayerIDSocket.find(PlayerGuid);
	if(it != m_PlayerIDSocket.end())
		return (*it).second;
	return 0;
}
const CGUID& CMessage::GetPlayerIDBySocket(long lSocketID)
{
	iteLG it = m_SocketPlayerID.find(lSocketID);
	if(it != m_SocketPlayerID.end())
		return (*it).second;
	return NULL_GUID;
}
void CMessage::UnMapSocket(long lSocketID,long lNetFlag)
{
	if(lNetFlag&NF_Server_Client)
	{
		UnMapPlayerIDSocket(lSocketID);
	}
}
void CMessage::UnMapPlayerIDSocket(long lSocketID)
{
	iteLG it =  m_SocketPlayerID.find(lSocketID);
	if(it != m_SocketPlayerID.end() )
	{
		m_PlayerIDSocket.erase((*it).second);
		m_SocketPlayerID.erase(it);
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
		MsgLog0x.Send();
}

long CMessage::c_SmInfoOrder = 0;
//����ļ�����
void CMessage::AsyClearFile(const char* pszFileName,bool bSync )
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
void CMessage::AsyWriteFile(const char* pszFileName,
							const char* pszContent,bool bSync )
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
			/*char pszLogingInfo[512]="";
			_snprintf(pszLogingInfo,512,"Client Socket[%d] Connect", GetSocketID());
			PutStringToFile("Login_GS_Info",pszLogingInfo);*/
		}
		break;
	case BASEMSG_Socket_Close:
		{
			// [AHC 080809 DBSMSG]
			if(GetNetFlag()&NF_Server_Client)
			{
				char pszGuid[50]="";
				GetPlayerID().tostring(pszGuid);
				GetGame()->OnPlayerLost(GetPlayerID(),GetSocketID());
				CMessage::UnMapPlayerIDSocket(GetSocketID());

				m_TransCongSokcets.erase(GetSocketID());

				/*char pszLogingInfo[512]="";
				_snprintf(pszLogingInfo,512,"Client Socket[%d] PlayerID[%s] Close", GetSocketID(),pszGuid);
				PutStringToFile("Login_GS_Info",pszLogingInfo);*/
			}
			else if(GetNetFlag()&NF_WServer_Client)
			{
				GetGame()->OnWSLost();
					
				GetGame()->CreateConnectWorldThread();
			}
			else if(GetNetFlag()&NF_DBServer_Client)
			{
				GetGame()->OnDBSLost();
				GetGame()->CreateConnectDBThread();
			}
			else if(GetNetFlag()&NF_BillingServer_Client)
			{
				GetGame()->OnBSLost();
				GetGame()->CreateConnectBSThread();
			}
			else if(GetNetFlag()&NF_LogServer_Client)
			{
				GetGame()->OnLOGSLost();
			}
			else if(GetNetFlag()&NF_BounsServer_Client)
			{
				GetGame()->OnBounsServerLost();
			}
		}
		break;
		//����ӵ��
	case BASEMSG_Transfer_Congestion:
		{
			if(GetNetFlag()&NF_Server_Client)
			{
				m_TransCongSokcets.insert(GetSocketID());
				char pszGuid[50]="";
				GetPlayerID().tostring(pszGuid);
				GetGame()->OnTransferCongestion(GetPlayerID(),GetSocketID());
				char pszLogingInfo[512]="";
				_snprintf(pszLogingInfo,512,"Client Socket[%d] PlayerID[%s] Transfer Congestion", GetSocketID(),pszGuid);
				PutStringToFile("Net",pszLogingInfo);
			}
		}
		break;
		//���䳩ͨ
	case BASEMSG_Transfer_Smoothly:
		{
			if(GetNetFlag()&NF_Server_Client)
			{
				m_TransCongSokcets.erase(GetSocketID());
				char pszGuid[50]="";
				GetPlayerID().tostring(pszGuid);
				GetGame()->OnTransferSmoothly(GetPlayerID(),GetSocketID());
				char pszLogingInfo[512]="";
				_snprintf(pszLogingInfo,512,"Client Socket[%d] PlayerID[%s] Transfer Smoothly", GetSocketID(),pszGuid);
				PutStringToFile("Net",pszLogingInfo);
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
	DWORD dwStartTime = timeGetTime();
	char pszStackInfo[1024]="";
	_snprintf(pszStackInfo,1024,"CMessage::Run(msgtype:%d) Start.",GetType());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif

	if( !Validate() )
	{
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMessage::Run() End.the msg format error.");
#endif
		char pszErrorInfo[512]="";
		_snprintf(pszErrorInfo,512,"The Msg Format Error(msgtype:%d,IP:%d)",GetType(),GetIP());
		PutStringToFile("Net",pszErrorInfo);
		return false;
	}
	// ��ȡplayerָ��
	CGUID lPlayerID = GetPlayerIDBySocket(GetSocketID());
	if( lPlayerID != NULL_GUID )
		m_pPlayer = GetGame()->FindPlayer(lPlayerID);
	else
		m_pPlayer = NULL;

	// ��ȡregionָ��
	if( m_pPlayer )
		m_pRegion = dynamic_cast<CServerRegion*>(m_pPlayer->GetFather());
	else
		m_pRegion = NULL;

	//������Ϣ����ȷ��
	if((GetType() - (GetType() & 0x00000ff) != 0) && (GetNetFlag()&NF_Server_Client) )
	{
		if( GetType() <= MSG_C2S || GetType() >= MSG_C2C)
		{
#ifdef _RUNSTACKINFO_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMessage::Run() End.the msg type error.");
#endif
			char pszErrorInfo[512]="";
			_snprintf(pszErrorInfo,512,"The Msg type Error(msgtype:%d,IP:%d)",GetType(),GetIP());
			PutStringToFile("Net",pszErrorInfo);
			return false;
		}
	}

#ifdef _RUNSTACKINFO_
	//�������ջ���
	_snprintf(pszStackInfo,1024,"PlayerName:%s,RegionName:%s.",(m_pPlayer?m_pPlayer->GetName():""),(m_pRegion?m_pRegion->GetName():""));
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif

	// ��Ϣ���ദ��
	switch( GetType() - (GetType() & 0x00000ff) )	// GetType() - GetType() % 256
	{
	case 0:
		DefaultRun();
		break;
		//----------------------------------------------------/Rgn��Ϣ/
	case MSG_W2S_RGN:
		OnRgnMessage(this);
		break;
		//----------------------------------------------------/server��Ϣ/
	case MSG_S2S_SERVER:
	case MSG_W2S_SERVER:
	case MSG_DB2S_SERVER:
		OnServerMessage(this);
		break;
		//----------------------------------------------------/��½��Ϣ/
	case MSG_S2S_LOG:
	case MSG_W2S_LOG:
	case MSG_C2S_LOG:
		OnLogMessage(this);
		break;

		//----------------------------------------------------/������Ϣ/
	case MSG_C2S_RGN:
		if( m_pRegion && m_pPlayer )
		{
			m_pRegion->OnMessage(this);
		}
		break;

		//----------------------------------------------------/SHAPE��Ϣ/
	case MSG_C2S_SHAPE:
		if( m_pPlayer &&  m_pRegion)
		{
			m_pPlayer->OnShapeMessage(this);
		}
		break;

		//----------------------------------------------------/�����Ϣ/
	case MSG_C2S_PLAYER:
		if( m_pPlayer &&  m_pRegion)
		{
			m_pPlayer->OnMessage(this);
		}
		break;

		//----------------------------------------------------/��Ʒ��Ϣ/
	case MSG_C2S_GOODS:
		if(m_pPlayer && m_pRegion)
		{
			OnGoodsMessage(this);
		}
		break;

	case MSG_C2S_SHOP:
		OnShopMessage(this);
		break;

	case MSG_C2S_INCREMENTSHOP:
		OnIncrementShopMessage(this);
		break;

		//----------------------------------------------------/������Ϣ/
	case MSG_W2S_OTHER:
	case MSG_C2S_OTHER:
		OnOtherMessage(this);
		break;    

		//----------------------------------------------------/GM��Ϣ/
	case MSG_W2S_GM:
		OnGMMessage(this);
		break;

		//----------------------------------------------------/TEAM��Ϣ/ [2007-7-19 edit]
		//case MSG_S2S_TEAM:
	case MSG_W2S_TEAM:
		OnWSTeamMessage(this);
		break;
	case MSG_C2S_TEAM:
		OnClientTeamMessage(this);
		//OnTeamMessage(this);
		break;
		//----------------------------------------------------
	case MSG_W2S_DEPOT:
	case MSG_C2S_DEPOT:
		//OnDepotMessage(this);
		break;

	case MSG_C2S_SKILL:
		if( m_pPlayer &&  m_pRegion)
		{
			OnSkillMessage(this);
		}
		break;
	case MSG_C2S_STATE:
		if (m_pPlayer && m_pRegion)
		{
			OnStateMessage(this);	
		}
		break;

	case MSG_W2S_ORGSYS:
	case MSG_C2S_ORGSYS:
		{
			OnOrgasysMessage(this);
		}
		break;

	case MSG_C2S_PLAYERSHOP:
		{
			OnPlayerShopMessage(this);
		}
		break;
	case MSG_S2C_CONTAINER:
	case MSG_C2S_CONTAINER:
		{
			OnContainerMessage( this );
		}
		break;

	case MSG_C2S_PET:
	case MSG_S2C_PET:
		{
			OnPetMessage(this);
		}
		break;
	case MSG_B2S:
		{
			OnBillingMessage(this);
		}
		break;
	case MSG_W2S_COUNTRY:
	case MSG_C2S_COUNTRY:
		OnCountryMessage(this);
		break;
	case MSG_LOG20_SERVER:
		{
			OnLogServerMessage(this);
		}
		break;
	case MSG_W2S_LINKMAN:
		{
			OnLinkmanMessage_ForWs(this);
		}
		break;
	case MSG_C2S_LINKMAN:
		{
			OnLinkmanMessage_ForClient(this);
		}
		break;
	case MSG_C2S_SPRITE:
		{
			OnSpriteMessage_ForClient(this);
		}
		break;
	case MSG_C2S_MAIL:
		{
			OnMailMessageForClient(this);
		}
		break;
	case MSG_W2S_MAIL:
		{
			OnMailMessageForWs(this);
		}
		break;

		// Fox@20081107 ��ҹ���
	case MSG_C2S_PM:
	case MSG_W2S_PM:
		{
			OnPMMessage(this);
		}
		break;
		/// Expter:@����ң����ܿͻ��˵Ĵ�����Ϣ
	case MSG_W2S_ANTICHEAT:
	case MSG_C2S_ANTICHEAT:
		{
			OnAntiCheatMsg(this);
		}
		break;
	case MSG_SM2GAPP_OPERATOR:
		OnSMMessage(this);
		break;
		
			// ������Ϣ
	case MSG_BDS2S:
	case MSG_C2S_BOUNS:
		{
			OnBounsMessage(this);
		}
		break;
	case MSG_W2S_SENTAI:
		OnWsSentaiMessage(this);
		break;
	case MSG_C2S_LOTTERY:
	case MSG_W2S_LOTTERY:
		OnLotteryMessage(this);
		break;
	}

#ifdef _RUNSTACKINFO_
	_snprintf(pszStackInfo,1024,"CMessage::Run() End.(ExpendTime:%d)",timeGetTime()-dwStartTime);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif
	return true;
}
