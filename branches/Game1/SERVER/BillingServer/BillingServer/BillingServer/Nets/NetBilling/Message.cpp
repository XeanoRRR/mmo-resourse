#include "stdafx.h"
#include "message.h"
#include "mmsystem.h"
#include "ServerForGS.h"
#include "../PUBLIC/Crc32Static.h"

#include "..\servers.h"
#include "..\serverclient.h"
#include "..\..\public\tools.h"

extern void OnServerMessage(CMessage* pMsg);			// ��ӦServer��Ϣ
extern void OnBillingMessage(CMessage* pMsg);			// ��Ӧ�Ʒ���Ϣ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessage::CMessage(long lType)
:m_strCdkey("")
{
	SetType(lType);
	m_pPlayer       = NULL;
	m_pRegion       = NULL;
	m_lMapID		= 0;
	m_lSocketID		= 0;
	m_dwIP			= 0;
}

CMessage::~CMessage(void)
{
}

// �յ�Socket���ܵ��İ��󽫰���װ����Ϣ
CMessage* CMessage::CreateMessage(void* pRLEBuf, unsigned long lRLESize)
{
	EnterCriticalSection(&m_CriticalSectionCreateMSG);
	static BYTE pBuf[1048576];
	static BYTE *pBigBuf = NULL;
	stMsg* p = NULL;
	unsigned long nSize = 0;

	// �⿪��ĳ��ȿ��ܳ���1M����Ϣ
	if( lRLESize > 131072 )
	{
		pBigBuf = new BYTE[lRLESize*8];
		nSize = DecodeRLE_SAFE(((BYTE*)pRLEBuf), pBigBuf, lRLESize,lRLESize*8);
		if(nSize==false)
		{
			LeaveCriticalSection(&m_CriticalSectionCreateMSG);
			return NULL;
		}
		p = (stMsg*)pBigBuf;
	}
	else	// �⿪С��1M����Ϣ
	{
		nSize = DecodeRLE_SAFE(((BYTE*)pRLEBuf), pBuf, lRLESize,1048576);
		if(nSize==false)
		{
			LeaveCriticalSection(&m_CriticalSectionCreateMSG);
			return NULL;
		}
		p = (stMsg*)pBuf;
	}

	// ʧ�ܣ�������Ϣ
	if( nSize == false )
	{
		LeaveCriticalSection(&m_CriticalSectionCreateMSG);
		return NULL;
	}

	// ������Ϣ
	CMessage* pMsg = new CMessage(p->eType);
	if( pMsg )
	{
		memcpy(pMsg->GetMsgBuf(), p, sizeof(stMsg));
		pMsg->SetSize(sizeof(stMsg));
		void* pv = &(((BYTE*)p)[sizeof(stMsg)]);
		pMsg->Add(pv, nSize-sizeof(stMsg));
	}

	SAFE_DELETE(pBigBuf);

	LeaveCriticalSection(&m_CriticalSectionCreateMSG);
	return pMsg;
}

// �յ�Socket���ܵ��İ��󽫰���װ����Ϣ
CMessage* CMessage::CreateMessageWithoutRLE(void* pBuf, unsigned long nSize)
{
	// ʧ�ܣ�������Ϣ
	if( pBuf ==NULL || nSize == false )
	{
		return NULL;
	}

	stMsg* p = (stMsg*)pBuf;
	// ������Ϣ
	CMessage* pMsg = new CMessage(p->eType);
	if( pMsg )
	{
		memcpy(pMsg->GetMsgBuf(), p, sizeof(stMsg));
		pMsg->SetSize(sizeof(stMsg));
		void* pv = &(((BYTE*)p)[sizeof(stMsg)]);
		pMsg->Add(pv, nSize-sizeof(stMsg));
	}
	return pMsg;
}


// [������] ������Ϣ
long CMessage::SendToGS(long lSocketID)
{
	CServerForGS *pServer = GetGame()->GetServerForGS();
	assert(pServer);

	// ѹ������
	int nSize = DoTemptBuffer(GetMsgBuf(),12,GetSize());
	DWORD dwLenCrc32,dwConCrc32;
	CCrc32Static::DataCrc32(&nSize,sizeof(long),dwLenCrc32);
	CCrc32Static::DataCrc32(GetMsgBuf(),GetSize(),dwConCrc32);
	*((long*)&s_pTemptBuffer[0]) = nSize;
	*((DWORD*)&s_pTemptBuffer[4]) = dwLenCrc32;
	*((DWORD*)&s_pTemptBuffer[8]) = dwConCrc32;

	return pServer->SendBySocketID(lSocketID,s_pTemptBuffer, nSize);
}


// ������Ϣ
long CMessage::SendToGS(const char* strCdkey)
{
	CServerForGS *pServer = GetGame()->GetServerForGS();
	assert(pServer);

	// ѹ������
	int nSize = DoTemptBuffer(GetMsgBuf(),12,GetSize());
	DWORD dwLenCrc32,dwConCrc32;
	CCrc32Static::DataCrc32(&nSize,sizeof(long),dwLenCrc32);
	CCrc32Static::DataCrc32(GetMsgBuf(),GetSize(),dwConCrc32);
	*((long*)&s_pTemptBuffer[0]) = nSize;
	*((DWORD*)&s_pTemptBuffer[4]) = dwLenCrc32;
	*((DWORD*)&s_pTemptBuffer[8]) = dwConCrc32;

	return pServer->SendByMapStr(strCdkey,s_pTemptBuffer, nSize);
}

// [������] ���͸����пͻ���
long CMessage::SendToAllGS()
{
	CServerForGS *pServer = GetGame()->GetServerForGS();
	assert(pServer);

	// ѹ������
	int nSize = DoTemptBuffer(GetMsgBuf(),12,GetSize());
	DWORD dwLenCrc32,dwConCrc32;
	CCrc32Static::DataCrc32(&nSize,sizeof(long),dwLenCrc32);
	CCrc32Static::DataCrc32(GetMsgBuf(),GetSize(),dwConCrc32);
	*((long*)&s_pTemptBuffer[0]) = nSize;
	*((DWORD*)&s_pTemptBuffer[4]) = dwLenCrc32;
	*((DWORD*)&s_pTemptBuffer[8]) = dwConCrc32;

	return pServer->SendAll(s_pTemptBuffer, nSize);
}

// ��Ϣ����
long CMessage::Run()
{
	// ��Ϣ���ദ��
	switch( GetType() - (GetType()&0x00000ff) )	// GetType() - GetType() % 256
	{
	//----------------------------------------------------/Server��Ϣ/
	case MSG_S2B_SERVER:
	case MSG_B2B_SERVER:
		OnServerMessage(this);
		break;

	//----------------------------------------------------/Billing��Ϣ/
	case MSG_S2B_BILLING:
	case MSG_B2S_BILLING:
		OnBillingMessage(this);
		break;
	}
	return true;
}
