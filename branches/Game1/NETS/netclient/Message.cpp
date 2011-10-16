#include "stdafx.h"
#include "message.h"

#include "nets\MySocket.h"
#include "nets\clients.h"
#include "client\gameclient\gameclient\game.h"
#include "client\gameclient\appclient\region.h"
#include "client\gameclient\appclient\player.h"
#include "client\gameclient\appclient\goods.h"
#include "client\gameclient\appclient\message\sequencestring.h"
#include "client\gameclient\appclient\Other\TestMsgType.h"

#include "../PUBLIC/Crc32Static.h"
#include "..\NETS\MemClient.h"

extern void OnLogMessage(CMessage* pMsg);				// ��Ӧ��½��Ϣ
extern void OnOtherMessage(CMessage* pMsg);				// ��Ӧ������Ϣ
extern void OnWorldMessage(CMessage* pMsg);             // ��ӦWorld������Ϣ
extern void OnGoodsMessage(CMessage* pMsg);				// ��Ӧ��Ʒ������Ϣ
extern void OnTeamMessage(CMessage * pMsg);				// ��Ӧ�����Ϣ
extern void OnShopMessage(CMessage* pMsg);				//�̵���Ϣ
extern void OnIncrementShopMessage(CMessage* pMsg);				//�̵���Ϣ
extern void OnDepotMessage(CMessage* pMsg);
extern void OnGMMessage(CMessage* pMsg);				// ��ӦGM��Ϣ
extern void OnSkillMessage(CMessage* pMsg);				// ��Ӧ������Ϣ
extern void OnPlayerShopMessage(CMessage* pMsg);
extern void OnOrgasysMessage(CMessage* pMsg);
extern void OnContainerMessage(CMessage* pMsg);
extern void OnPetMessage(CMessage* pMsg);
extern void OnCostMessage(CMessage *pMsg);
extern void OnLinkmanMessage(CMessage *pMsg);
extern void OnSpriteMessage(CMessage *pMsg);

extern void OnCountryMessage(CMessage *pMsg);

extern void OnMailMessage(CMessage *pMsg);			// �ʼ������Ϣ
extern void OnFriendHuntMessage(CMessage *pMsg);	// ���Ѳ�ѯ��Ϣ

extern void OnCheckCheatMessage(CMessage *pMsg);    //����������Ϣ

extern void OnBounsMessage(CMessage* pMsg);			// ��Ϸ������ϵͳ�����Ϣ

extern void OnSentaiMessage(CMessage* pMsg);			// ս��ϵͳ

extern void OnLotteryMessage(CMessage* pMsg);			// ��Ʊϵͳ

//--------------------------------------------------------------------------------------
extern void OnTestMouseMessage(CMessage *pMsg);			// ���Գ����ģ�������Ϣ����
extern void OnTestKeyMessage(CMessage *pMsg);			// ���Գ����ģ�������Ϣ����
extern void OnTestLogicMessage(CMessage *pMsg);			// ���Գ�����߼��¼���Ϣ����
//--------------------------------------------------------------------------------------

extern HWND g_hRetLb;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CClient	*CMessage::m_pNetClient;
CMemClient *CMessage::m_pMemClient = NULL;

CMessage::CMessage(void)
{
	m_pRegion = NULL;
	m_pPlayer = NULL;
}

CMessage::CMessage(long lType)
{
	m_pRegion = NULL;
	m_pPlayer = NULL;

	Init(lType);
}

CMessage::~CMessage(void)
{
}

 //������Ϣ��ر���
void CMessage::Reset(void)
{
	m_pRegion = NULL;
	m_pPlayer = NULL;
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

// [�ͻ���] ������Ϣ
long CMessage::Send(CClient* pClient)
{
		// �����Ϣ������c to s �Ѵ�C��S����Ϣ��װ ʱ��Ч�� �� ����Ч��
	if(pClient == NULL)
		pClient = m_pNetClient;
	if(pClient == NULL)	return false;
	// ������ʱģ�����
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	vector<CDataBlock*>& MsgData = GetMsgData();
	pMsg->SetMsgData(MsgData);
	MsgData.clear();
	pClient->ASend(pMsg);
	return true;
}


//���ļ�
void CMessage::AsyOpenFile(const char* pszFileName)
{
	if(m_pMemClient)
	{
		CMessage msg(MSG_FILE_OPEN);
		msg.Add(pszFileName);
		m_pMemClient->Send(&msg);
	}
	return;
}
//�ر��ļ�
void CMessage::AsyCloseFile(const char* pszFileName)
{
	if(m_pMemClient)
	{
		CMessage msg(MSG_FILE_CLOSE);
		msg.Add(pszFileName);
		m_pMemClient->Send(&msg);
	}
	return;
}
//����ļ�����
void CMessage::AsyClearFile(const char* pszFileName)
{
	if(m_pMemClient)
	{
		CMessage msg(MSG_FILE_CLEAR);
		msg.Add(pszFileName);
		m_pMemClient->Send(&msg);
	}
	return;
}

//�첽д�ļ��Ľӿ�
void CMessage::AsyWriteFile(const char* pszFileName,const char* pszContent)
{
	if(m_pMemClient)
	{
		CMessage msg(MSG_FILE_WRITE);
		msg.Add(pszFileName);
		msg.Add(pszContent);
		m_pMemClient->Send(&msg);
	}
}


// ����Գ�������Ϣ
long CMessage::SendMemToTest(CMemClient *pMemClient)
{
#ifdef _CLENTTOTEST_
	if(pMemClient == NULL)
		pMemClient = m_pMemClient;
	if(pMemClient == NULL)
		return false;
	//����һ����Ϣ
	*((long*)Base()) = GetSize()+HEAD_SIZE;
	CBaseMessage *pMsg = CBaseMessage::AllocMsg();
	Clone(pMsg);
	pMemClient->Send(pMsg);
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////

long CMessage::DefaultRun()
{
	switch( GetType() )
	{
	case BASEMSG_Socket_Close:
		{
			GetGame()->OnServerLost();
			if(GetNetFlag()&NF_LS_Client)
			{			
			}
			else if(GetNetFlag()&NF_GS_Client)
			{
			}
		}
		break;
	}
	return true;
}

// �ͻ�����Ϣ����
long CMessage::Run()
{
	// ��ȡregion��playerָ��
	if (m_pPlayer==NULL)
	{
		m_pPlayer = GetGame()->GetMainPlayer();
	}
	if (m_pRegion==NULL)
	{
		m_pRegion = (CRegion*)GetGame()->GetRegion();
	}

	char str[1024] = "";
#ifdef _RUNSTACKINFO_
	sprintf_s(str,"CMessage::Run() Start.��Ϣ���ͣ�%d",GetType());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),str);
#endif
	//----------------------------------


	// ת����Test����
#ifdef _CLENTTOTEST_
	if(m_pMemClient)
	{
		SendMemToTest();
	}
#endif

	unsigned long lType = GetType() & 0xffffff00;
	// ��Ϣ���ദ��
	switch( lType )	// GetType() - GetType() % 256
	{
	case 0:
		DefaultRun();
		break;
	//----------------------------------------------------/��½��Ϣ/
	case MSG_C2C_LOG:
	case MSG_L2C_LOG:
	case MSG_S2C_LOG:
		OnLogMessage(this);
		break;

	//----------------------------------------------------/������Ϣ/
	case MSG_S2C_RGN:
		if( m_pRegion )
		{
			m_pRegion->OnMessage(this);
		}
		break;

	//----------------------------------------------------/SHAPE��Ϣ/
	case MSG_S2C_SHAPE:
		if( m_pPlayer )
		{
			m_pPlayer->CMoveShape::OnMessage(this);
		}
		break;
	//----------------------------------------------------/�����Ϣ/
	case MSG_S2C_PLAYER:
		if( m_pPlayer )
		{
			m_pPlayer->OnMessage(this);
		}
		break;

	//---------------------------------------------------/��Ʒ��Ϣ/
	case MSG_S2C_GOODS:
		if(m_pPlayer)
		{
			OnGoodsMessage(this);
		}
		break;
	//-----------------------------------------------------/���Ѳ�ѯ/
	case MSG_S2C_PM:
		if(m_pPlayer)
		{
			OnFriendHuntMessage(this);
		}
		break;
	case MSG_S2C_SHOP:
		OnShopMessage(this);
		break;

	case MSG_S2C_INCREMENTSHOP:
		OnIncrementShopMessage(this);
		break;

	case MSG_S2C_OTHER:
		OnOtherMessage(this);
		break;

	case MSG_S2C_TEAM:
		OnTeamMessage(this);
		break;

	case MSG_S2C_DEPOT:
		OnDepotMessage(this);
		break;

	case MSG_S2C_GM:
		/**## �������˶�GM�������Ӧ���ش��� */
		/**## Added By Jar @ 2004-4-6 */
		OnGMMessage(this);
		break;

	case MSG_S2C_SKILL:
		if( m_pRegion )
		{
			OnSkillMessage(this);
		}
		break;

	case MSG_S2C_ORGSYS:
		{
			OnOrgasysMessage(this);
		}
		break;

	case MSG_S2C_PLAYERSHOP:
		{
			OnPlayerShopMessage(this);
		}
		break;
	case MSG_S2C_CONTAINER:
		{
			OnContainerMessage(this);
		}
		break;
        // �������ע��  Bugs 2008-9-12
	case MSG_S2C_PET:
		{
			OnPetMessage(this);
		}
			break;
	case MSG_S2C_MAIL:
		{
			OnMailMessage(this);
		}

	case MSG_S2C_COST:
		{
			OnCostMessage(this);
		}
	case MSG_S2C_CARD:
		{
			//OnGameCardSaleMessage(this);
		}
		break;
	case MSG_S2C_COUNTRY:
		OnCountryMessage(this);
		break;
	case MSG_S2C_LINKMAN:
		OnLinkmanMessage(this);
		break; 
	case MSG_S2C_SPRITE:
		OnSpriteMessage(this);
		break;
		/* *****************************
		����:   �������Ϣ����
		����:zhaohang(kaba)
		��������: 2009/5/26
		�޸���־:
		�޸�����:
		�޸�����:
		�޸���  :
		* *****************************/
	case MSG_S2C_ANTICHEAT://������Ϣͷ���ݷ�����ȷ��
		OnCheckCheatMessage(this);
		break;

	case MSG_S2C_BOUNS:
		OnBounsMessage(this);
		break;
	case MSG_S2C_SENTAI:
		OnSentaiMessage(this);
		break;
	case MSG_S2C_LOTTERY:
		OnLotteryMessage(this);
		break;
	}
#ifdef _RUNSTACKINFO_
	sprintf_s(str,"CMessage::Run() End.��Ϣ���ͣ�%d",GetType());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),str);
#endif
	return true;
}

// ���Գ����͵���Ϣ����
long CMessage::TestRun()
{
	// ��ȡregion��playerָ��
	if (m_pPlayer==NULL)
		m_pPlayer = GetGame()->GetMainPlayer();
	if (m_pRegion==NULL)
		m_pRegion = (CRegion*)GetGame()->GetRegion();

	unsigned long lType = GetType() & 0xffffff00;

	// ת����Test����
#ifdef _CLENTTOTEST_
	// ��Ϣ���ദ��
	switch( lType )	// GetType() - GetType() % 256
	{
	case 0:
		break;
	case MSG_T2C_MOUSE:
		OnTestMouseMessage(this);
		break;
	case MSG_T2C_KEY:
		OnTestKeyMessage(this);
		break;
	case MSG_T2C_LOGIC:
		OnTestLogicMessage(this);
	}
#endif
	return 1;
}
