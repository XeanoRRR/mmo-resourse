#include "StdAfx.h"
#include "myNetServerclient_Client.h"
#include "message.h"
#include "../PUBLIC/Crc32Static.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MAX_CLIENT_MESSAGE_LENGTH = 20480;
const long MaxSendBuffSize = 1024*50;

CMyNetServerClient_Client::CMyNetServerClient_Client(CServer* pServer)
:CMyServerClient(pServer)
{
	m_nBufferSize = MAX_CLIENT_MESSAGE_LENGTH;
	m_nIniMsgLength = MAX_CLIENT_MESSAGE_LENGTH;
	m_pBuffer = new char[m_nBufferSize]; 

	m_lMaxSendBuffSize = MaxSendBuffSize;
	m_lSendBufferSize = MaxSendBuffSize;
	m_pSendBuffer = new char[m_lSendBufferSize];
}

CMyNetServerClient_Client::~CMyNetServerClient_Client(void)
{
	SAFE_DELETE(m_pBuffer);
}
// ��Է��Ͽ���ʱ�򼤻�
void CMyNetServerClient_Client::OnClose(int errorCode)
{
	// ����������
	if(strlen(GetMapStr())==0)
		return;
	CMessage* msg;
	msg = new CMessage(MSG_L2L_LOG_CLIENTCLOSE);
	msg->Add(const_cast<char*>(GetMapStr()));
	GetServer()->GetRecvMessages()->PushMessage(msg);

	CMySocket::OnClose();
}

bool CMyNetServerClient_Client::CheckMsg(CBaseMessage *pMsg)
{
	if(pMsg->GetType() <= MSG_C2L || pMsg->GetType() >= MSG_W2W)
	{
		m_pServers->AddForbidIP(GetDwordIP());
		m_pServers->QuitClientBySocketID(GetIndexID());
		char str[200];
		sprintf(str, "CDKey%s(IP:%s)��������Ϣ���������Ϣ���͡�",GetMapStr(),GetIP());
		PutDebugString(str);
		return false;
	}
	return true;
}

//-------------------------------------------------------------
// ʹ����ɶ˿�
//-------------------------------------------------------------
//void CMyNetServerClient_Client::OnReceive(int nErrorCode)
//{	
//	if(m_pServers==NULL)	return;
//
//
//	long lPoint = 0;	// ��ǰ��Ч��������ַ
//	while( m_nSize >= 12 )
//	{
//		long lRLESize = *((long*)(&m_pBuffer[lPoint]));
//
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
//				sprintf(str, "CDKey%s(IP:%s)��������Ϣ����Ϣ����У��ʧ�ܡ�",GetMapStr(),GetIP());
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
//					m_nSize = 0;
//					char str[200];
//					sprintf(str, "CDKey%s(IP:%s)��������Ϣ����Ϣ����У��ʧ�ܡ�",GetMapStr(),GetIP());
//					PutDebugString(str);
//					return;
//				}
//			}
//
//			CMessage* pMsg = CreateMsg(&m_pBuffer[lPoint]+12, lRLESize-12);
//			// ��Ϣ�����ɹ���ѹ����Ϣ����
//			if( pMsg )
//			{
//				if(CheckMsg(pMsg) == false)
//				{
//					m_nSize= 0;
//					delete pMsg;
//					return;
//				}
//				pMsg->SetClientSocketID(GetIndexID());
//				pMsg->SetMapID(GetMapID());
//				pMsg->SetCdkey(GetMapStr());
//				pMsg->SetIP(GetDwordIP());
//
//				m_pServers->GetRecvMessages()->PushMessage( pMsg );
//				m_nSize -= lRLESize;
//				lPoint += lRLESize;		// �ƶ�������ָ��
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
//
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
//
//	// �Ѳ���ȫ����Ϣ�ƶ���������ͷ��
//	if( lPoint > 0 )
//	{
//		memmove(m_pBuffer, &m_pBuffer[lPoint], m_nSize);
//		lPoint = 0;
//	}
//}

void CMyNetServerClient_Client::OnOneMessageSizeOver(long lSize,long lPermitSize)
{
	char str[256];
	sprintf(str, "�ͻ���(CDkey:%s,IP:%s)��������Ϣ(����:%d BYTE)������������ֵ(%d BYTE),��ǿ���жϡ�",
		GetMapStr(),GetIP(),lSize,lPermitSize);
	PutDebugString(str);
}
void CMyNetServerClient_Client::OnTotalMessageSizeOver(long lSize,long lPermitSize)
{
	char str[256];
	sprintf(str, "�ͻ���(CDkey:%s,IP:%s)ÿ�뷢������Ϣ��С(%d BYTE)������������ֵ(%d BYTE),��ǿ���жϡ�",
		GetMapStr(),GetIP(),lSize,lPermitSize);
	PutDebugString(str);
}
