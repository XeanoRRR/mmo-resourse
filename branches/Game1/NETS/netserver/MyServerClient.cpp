#include "StdAfx.h"
#include "myserverclient.h"
#include "nets\netserver\message.h"
#include "server\gameserver\appserver\Player.h"
#include "server\gameserver\appserver\message\sequencestring.h"
#include "../PUBLIC/Crc32Static.h"
#include <MMSystem.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MAX_CLIENT_MESSAGE_LENGTH = 20480;

const long MaxSendBuffSize = 1024*50;

CMyServerClient::CMyServerClient(CServer*	pServers)
:CServerClient(pServers)
{
	m_nBufferSize = MAX_CLIENT_MESSAGE_LENGTH;
	m_nIniMsgLength = MAX_CLIENT_MESSAGE_LENGTH;
	m_pBuffer = new char[m_nBufferSize];

	m_lMaxSendBuffSize = MaxSendBuffSize;
	m_lSendBufferSize = MaxSendBuffSize;
	m_pSendBuffer = new char[m_lSendBufferSize];
}

CMyServerClient::~CMyServerClient(void)
{
	SAFE_DELETE(m_pBuffer);
}



// ��Է��Ͽ���ʱ�򼤻�
void CMyServerClient::OnClose(int errorCode)
{
	//CMessage::GetRecvQueue()->ClearSocket(this);
	long lMapID = GetMapID();

	if(lMapID==0)
		return;
	CMessage* msg = new CMessage(MSG_S2S_LOG_PLAYER_ONLOST);

	msg->Add(lMapID);
	msg->Add("");
	GetServer()->GetRecvMessages()->PushMessage(msg);


	CMySocket::OnClose();
}

bool CMyServerClient::CheckMsg(CBaseMessage *pMsg)
{
	//�������Ϣ����
	if( pMsg->GetType() <= MSG_C2S || pMsg->GetType() >= MSG_C2C)
	{
		m_pServers->AddForbidIP(GetDwordIP());
		m_pServers->QuitClientBySocketID(GetIndexID());
		char str[200];
		sprintf(str, "���%d(IP:%s)��������Ϣ���������Ϣ���͡�",GetMapID(),GetIP());
		PutDebugString(str);
		return false;
	}

	ULONG lPlayerID = GetMapID();
	if(lPlayerID)
	{
		// ��Ϣ��ʱ��֤
		if( GetGame()->GetSetup()->dwMsgValidateTime )
		{
			unsigned long ulClientTime = pMsg->GetMsgSendTime();
			if(ulClientTime)
			{
				unsigned long cur_time = /*(ULONG)time(NULL)*/ timeGetTime();
				long time_stamp = /*timeGetTime()*/ cur_time - ulClientTime;

				// ��Ϣ���ͺ�ʱ�� -x <= time_stamp <= +x �ǺϷ���
				// | ��time_stamp | <= x	�Ϸ�
				//	     		     > x	�Ƿ�
				bool *enable = GetGame()->GetValidateTime(lPlayerID);
				if( enable && *enable )
				{
					if( ulClientTime==0 )
					{
						AddLogText("����ʱ��Ч����յ�һ��0ʱ�� ���%d",lPlayerID);
					}
					else
					{
						if( abs(time_stamp) > GetGame()->GetSetup()->dwMsgValidateTime )
						{
							AddLogText("<worning>��� %d ��Ϣ��ʱ̫�� %d ���룬S:%d C:%d (��Ϣ���� %d)", lPlayerID, time_stamp, cur_time, ulClientTime, pMsg->GetType());
							m_pServers->AddForbidIP(GetDwordIP());
							m_pServers->QuitClientBySocketID(GetIndexID());
							return false;
						}
					}
				}
			}
		}
		//�������������
		if( GetGame()->GetSetup()->dwUseSeqeunce && CSequenceString::m_sSequenceString.size())
		{
			ULONG cssCode = pMsg->GetCode();
			if(cssCode)
			{
				// ͨ�������ַ�����Ч����Ϣ
				CSequenceString *css = GetGame()->GetSequence(lPlayerID);
				if( css && css->m_bIsUsable && css->GetPos() != -1)
				{
					if( cssCode == 0 )
					{
						AddLogText("�������к��յ�һ��0���� ���%d",lPlayerID);
					}
					else
					{
						if( css->Check(cssCode) == 0 )
						{
							AddErrorLogText("�ͻ���[%d] ����[%d] ����λ��[%d]",lPlayerID,cssCode, css->GetPos());
							AddErrorLogText( "��Ϣ���� %d", pMsg->GetType() );
							m_pServers->AddForbidIP(GetDwordIP());
							m_pServers->QuitClientBySocketID(GetIndexID());
							return false;
						}
					}
				}
			}
		}

	}
	return true;
}

//-------------------------------------------------------------
// ʹ����ɶ˿�
//-------------------------------------------------------------
//void CMyServerClient::OnReceive(int nErrorCode)
//{	
//	if(m_pServers==NULL)	return;
//	long lPoint = 0;	// ��ǰ��Ч��������ַ
//	while( m_nSize >= 12 )
//	{
//		long lRLESize = *((long*)(&m_pBuffer[lPoint]));
//		//�Խ�����Ϣ��У��
//		if(m_pServers->GetCheck())
//		{
//			//��Ϣ�ĳ�����У��
//			if(lRLESize > m_pServers->GetMaxMsgLen())
//			{
//				OnOneMessageSizeOver(lRLESize,m_pServers->GetMaxMsgLen());
//				m_pServers->AddForbidIP(GetDwordIP());
//				m_pServers->QuitClientBySocketID(GetIndexID());
//				return;
//			}
//			//�Գ���������У��
//			DWORD dwLenCrc32;
//			CCrc32Static::DataCrc32(&lRLESize,4,dwLenCrc32);
//			DWORD lClientSize = *((DWORD*)(&m_pBuffer[lPoint]+4));
//			if(dwLenCrc32 != lClientSize)
//			{
//				m_pServers->AddForbidIP(GetDwordIP());
//				m_pServers->QuitClientBySocketID(GetIndexID());
//				char str[200];
//				sprintf(str, "���%d(IP:%s)��������Ϣ����Ϣ����У��ʧ�ܡ�",GetMapID(),GetIP());
//				PutDebugString(str);
//				return;
//			}
//		}
//
//		// ���������ݴ��ڵ���һ����Ϣ�ĳ���
//		if( lRLESize <= m_nSize )
//		{
//			//����Ϣ��������У��
//			if(m_pServers->GetCheckMsgCon())
//			{
//				DWORD dwConCrc32;
//				CCrc32Static::DataCrc32(&m_pBuffer[lPoint]+12,lRLESize-12,dwConCrc32);
//				DWORD lClientConCrc32= *((DWORD*)(&m_pBuffer[lPoint]+8));
//				//��⵽���ݲ�һ��
//				if(dwConCrc32 != lClientConCrc32)
//				{
//					m_pServers->AddForbidIP(GetDwordIP());
//					m_pServers->QuitClientBySocketID(GetIndexID());
//					char str[200];
//					sprintf(str, "���%d(IP:%s)��������Ϣ����Ϣ����У��ʧ�ܡ�",GetMapID(),GetIP());
//					PutDebugString(str);
//					return;
//				}
//			}
//
//			CBaseMessage* pMsg = CreateMsg(&m_pBuffer[lPoint]+12, lRLESize-12);
//			// ��Ϣ�����ɹ���ѹ����Ϣ����
//			if( pMsg )
//			{
//				if(CheckMsg(pMsg) == false)
//				{
//					m_nSize = 0;
//					delete pMsg;
//					return;
//				}
//
//				pMsg->SetClientSocketID(GetIndexID());
//				pMsg->SetMapID(GetMapID());
//				pMsg->SetCdkey(GetMapStr());
//				pMsg->SetIP(GetDwordIP());
//				m_pServers->GetRecvMessages()->PushMessage( pMsg );
//				m_nSize -= lRLESize;
//				lPoint += lRLESize;		// �ƶ�������ָ��
//
//				// ����ǳ�������Ϣ���ѻ�������С��������
//				if( m_nBufferSize > m_nIniMsgLength && m_nSize <= m_nIniMsgLength )
//				{
//					char *pTemp = new char[m_nSize];
//					memcpy(pTemp, &m_pBuffer[lPoint], m_nSize);
//
//					SAFE_DELETE(m_pBuffer);
//					m_nBufferSize = MAX_MESSAGE_LENGTH;
//					m_pBuffer = new char[m_nBufferSize];
//
//					memcpy(m_pBuffer, pTemp, m_nSize);
//					SAFE_DELETE(pTemp);
//					lPoint = 0;
//				}
//			}
//			else
//			{
//				// ����������ǰ�յ�����������
//				m_nSize = 0;
//				char str[256];
//				sprintf(str, "CreateMessage error");
//				PutDebugString(str);
//				return;
//			}
//		}
//		else
//		{
//			// ����������ǰ�յ�����������
//			if( lRLESize < 0 )
//			{
//				m_nSize = 0;
//				char str[256];
//				sprintf(str, "����������Ϣ(RleSize=%d)��������ǰ�յ�����������", lRLESize);
//				PutDebugString(str);
//				return;
//			}
//			// �յ���Ϣ���������Ϣ�����������µĻ�����
//			if( lRLESize > m_nBufferSize )
//			{
//				char *pTemp = new char[m_nSize];
//				memcpy(pTemp, &m_pBuffer[lPoint], m_nSize);
//				SAFE_DELETE(m_pBuffer);
//				m_nBufferSize = lRLESize;
//				m_pBuffer = new char[m_nBufferSize];
//				memcpy(m_pBuffer, pTemp, m_nSize);
//				SAFE_DELETE(pTemp);
//				return;
//			}
//			break;
//		}
//	}
//	// �Ѳ���ȫ����Ϣ�ƶ���������ͷ��
//	if( lPoint > 0 )
//	{
//		memmove(m_pBuffer, &m_pBuffer[lPoint], m_nSize);
//		lPoint = 0;
//	}
//}

void CMyServerClient::OnOneMessageSizeOver(long lSize,long lPermitSize)
{
	char str[256];
	sprintf(str, "�ͻ���(PlayerID:%d,IP:%s)��������Ϣ(����:%d BYTE)������������ֵ(%d BYTE),��ǿ���жϡ�",
		GetMapID(),GetIP(),lSize,lPermitSize);
	PutDebugString(str);
}
void CMyServerClient::OnTotalMessageSizeOver(long lSize,long lPermitSize)
{
	char str[256];
	sprintf(str, "�ͻ���(PlayerID:%d,IP:%s)ÿ�뷢������Ϣ��С(%d BYTE)������������ֵ(%d BYTE),��ǿ���жϡ�",
		GetMapID(),GetIP(),lSize,lPermitSize);
	PutDebugString(str);
}