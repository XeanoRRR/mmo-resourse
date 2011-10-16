#include "StdAfx.h"
#include "clientforgs.h"
#include "message.h"
#include "../PUBLIC/Crc32Static.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MAX_WORLD_MESSAGE_LENGTH = 10485760;

const long MaxSendBuffSize = 1024*1024;

CClientForGS::CClientForGS(CServer* pServer)
:CServerClient(pServer)
{
	m_nBufferSize = MAX_WORLD_MESSAGE_LENGTH;
	m_pBuffer = new char[m_nBufferSize];

	m_lMaxSendBuffSize = MaxSendBuffSize;
	m_lSendBufferSize = MaxSendBuffSize;
	m_pSendBuffer = new char[m_lSendBufferSize];
}

CClientForGS::~CClientForGS(void)
{
	SAFE_DELETE(m_pBuffer);
}

// ��Է��Ͽ���ʱ�򼤻�
void CClientForGS::OnClose(int errorCode)
{
	// ����������
	CMessage* msg;
	msg = new CMessage(CMessage::MSG_B2B_SERVER_GAMESERVERLOST);
	msg->Add(GetMapID());
	GetServer()->GetRecvMessages()->PushMessage(msg);

	CMySocket::OnClose();
}

void CClientForGS::SetSendRevBuf()
{
	//���ڲ��׽��ֵķ��ͻ�������Ϊ0
	int nValue = 0;
	int nSize  = sizeof(nValue);
	setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&nValue,sizeof(nSize));
	//nValue = 1048576;
	//nSize  = sizeof(nValue);
	//int nRet = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*)&nValue,sizeof(nSize));
	//if(nRet == SOCKET_ERROR)
	//{
	//	int nError = WSAGetLastError();
	//	char str[256];
	//	sprintf(str,"���ÿͻ��˽��ܻ���������:%d.",nError);
	//	PutDebugString(str);
	//}
}
//-------------------------------------------------------------
// ʹ����ɶ˿�
//-------------------------------------------------------------
void CClientForGS::OnReceive(int nErrorCode)
{	
	if(m_pServers==NULL)	return;


	long lPoint = 0;	// ��ǰ��Ч��������ַ
	while( m_nSize >= 12 )
	{
		long lRLESize = *((long*)(&m_pBuffer[lPoint]));

		//�Գ���������У��
		DWORD dwLenCrc32;
		CCrc32Static::DataCrc32(&lRLESize,4,dwLenCrc32);
		DWORD lClientSize = *((DWORD*)(&m_pBuffer[lPoint]+4));
		if(dwLenCrc32 != lClientSize)
		{
			char str[256];
			sprintf(str, "WorldServer %d Send MSG, Length Check ERROR!",GetMapID());
			PutDebugString(str);
			m_nSize=0;
			return;
		}

		// ���������ݴ��ڵ���һ����Ϣ�ĳ���
		if( lRLESize <= m_nSize )
		{
			CMessage* pMsg = CMessage::CreateMessageWithoutRLE(&m_pBuffer[lPoint]+12, lRLESize-12);

			// ��Ϣ�����ɹ���ѹ����Ϣ����
			if( pMsg )
			{
				DWORD dwConCrc32;
				CCrc32Static::DataCrc32(pMsg->GetMsgBuf(),pMsg->GetSize(),dwConCrc32);
				DWORD lClientConCrc32= *((DWORD*)(&m_pBuffer[lPoint]+8));
				//��⵽���ݲ�һ��
				if(dwConCrc32 != lClientConCrc32)
				{
					char str[256];
					sprintf(str, "WorldServer %dSend MSG %d, Content Check ERROR!",GetMapID(),pMsg->GetType());
					PutDebugString(str);
					m_nSize=0;
					return;
				}


				pMsg->SetClientSocketID(GetIndexID());
				pMsg->SetMapID(GetMapID());
				pMsg->SetCdkey(GetMapStr());
				pMsg->SetIP(GetDwordIP());

				m_pServers->GetRecvMessages()->PushMessage( pMsg );
				m_nSize -= lRLESize;
				lPoint += lRLESize;		// �ƶ�������ָ��

				// ����ǳ�������Ϣ���ѻ�������С��������
				if( m_nBufferSize > MAX_MESSAGE_LENGTH && m_nSize <= MAX_MESSAGE_LENGTH )
				{
					char *pTemp = new char[m_nSize];
					memcpy(pTemp, &m_pBuffer[lPoint], m_nSize);

					SAFE_DELETE(m_pBuffer);
					m_nBufferSize = MAX_MESSAGE_LENGTH;
					m_pBuffer = new char[m_nBufferSize];

					memcpy(m_pBuffer, pTemp, m_nSize);
					SAFE_DELETE(pTemp);
					lPoint = 0;
				}
			}
			else
			{
				// ����������ǰ�յ�����������
				m_nSize = 0;

				char str[256];
				sprintf(str, "CreateMessage error");
				PutDebugString(str);
				return;
			}
		}
		else
		{
			// ����������ǰ�յ�����������
			if( lRLESize < 0 )
			{
				m_nSize = 0;

				char str[256];
				sprintf(str, "����������Ϣ(RleSize=%d)��������ǰ�յ�����������", lRLESize);
				PutDebugString(str);

				return;
			}

			// �յ���Ϣ���������Ϣ�����������µĻ�����
			if( lRLESize > m_nBufferSize )
			{
				char *pTemp = new char[m_nSize];
				memcpy(pTemp, &m_pBuffer[lPoint], m_nSize);
				SAFE_DELETE(m_pBuffer);
				m_nBufferSize = lRLESize;
				m_pBuffer = new char[m_nBufferSize];
				memcpy(m_pBuffer, pTemp, m_nSize);
				SAFE_DELETE(pTemp);

				//#ifdef _DEBUG
				char str[256];
				sprintf(str, "WorldServer%d ��������Ϣ Size = %d",GetMapID(),m_nBufferSize);
				PutDebugString(str);
				//#endif
				return;
			}

			char str[256];
			sprintf(str, "WorldServer%d ����������Ϣ m_nSize = %d,lRLESize = %d, Point = %d",GetMapID(),m_nSize,lRLESize, lPoint);
			PutDebugString(str);
			break;
		}
	}
	// �Ѳ���ȫ����Ϣ�ƶ���������ͷ��
	if( lPoint > 0 )
	{
		memmove(m_pBuffer, &m_pBuffer[lPoint], m_nSize);
		lPoint = 0;
	}
}