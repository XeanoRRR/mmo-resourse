#include "StdAfx.h"
#include "mynetclient.h"
#include "message.h"
#include "../PUBLIC/Crc32Static.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMyNetClient::CMyNetClient(void)
{
}

CMyNetClient::~CMyNetClient(void)
{
}

void CMyNetClient::HandleReceive()
{
	return;
}

void CMyNetClient::HandleClose()
{
	// ����������
	CMessage* msg;
	msg = new CMessage(MSG_S2S_SERVER_WORLDSERVERCLOSE);

	GetRecvMessages()->PushMessage(msg);
}

void CMyNetClient::HandleConnect()
{
}

CBaseMessage* CMyNetClient::CreateMsg(void* pBuf, unsigned long nSize)
{
	return CBaseMessage::CreateWithoutRLE(pBuf, nSize);
}
//-----------------------------------------------------------
// ���ܵ����ݵĴ���
// ������ܵ��˴ӷ��������������ݣ�����Ϸ�Ĵ���
//-----------------------------------------------------------
//void CMyNetClient::OnReceive(int nErrorCode)
//{
//	if(nErrorCode != 0)
//	{
//		//�ͻ��˽�������ʧ��
//		char str[256];
//		sprintf(str, "FD_READ failed with error %d\n",nErrorCode);
//		PutDebugString(str);
//		return;
//	}
//	int nReadSize = min(m_nBufferSize - m_nSize,10240);
//	int nRetsize = Recv(&m_pBuffer[m_nSize], nReadSize);		
//	if (nRetsize == SOCKET_ERROR)
//	{
//		int nError = GetLastError();
//		if(nError != WSAEWOULDBLOCK)
//		{
//			char str[200];
//			sprintf(str, "Client OnReceive Socket Error = %d",nError);
//			PutDebugString(str);
//		}
//		return;
//	}
//	else if (nRetsize == 0)
//	{
//		char str[256];
//		sprintf(str, "Client OnReceive size is 0");
//		PutDebugString(str);
//		return;
//	}
//	//��ӽ���ͳ����
//	AddRecvSize(nRetsize);
//	m_nSize += nRetsize;
//	long lPoint = 0;	// ��ǰ��Ч��������ַ
//	while( m_nSize >= 12 )
//	{
//		long lRLESize = *((long*)&m_pBuffer[lPoint]);
//		//�Գ���������У��
//		DWORD dwLenCrc32;
//		CCrc32Static::DataCrc32(&lRLESize,4,dwLenCrc32);
//		DWORD lClientSize = *((DWORD*)(&m_pBuffer[lPoint]+4));
//		if(dwLenCrc32 != lClientSize)
//		{
//			char str[256];
//			sprintf(str, "��WorldServer ������Ϣ������У�����,lPoint = %d,lRLESize=%d��",lPoint,lRLESize);
//			PutDebugString(str);
//			m_nSize=0;
//			return;
//		}
//		// ���������ݴ��ڵ���һ����Ϣ�ĳ���
//		if( lRLESize <= m_nSize )
//		{
//			CBaseMessage* pMsg = CreateMsg(&m_pBuffer[lPoint]+12, lRLESize-12);
//			// ��Ϣ�����ɹ���ѹ����Ϣ����
//			if( pMsg )
//			{
//				DWORD dwConCrc32;
//				CCrc32Static::DataCrc32(pMsg->GetMsgBuf(),pMsg->GetSize(),dwConCrc32);
//				DWORD lClientConCrc32= *((DWORD*)(&m_pBuffer[lPoint]+8));
//				//��⵽���ݲ�һ��
//				if(dwConCrc32 != lClientConCrc32)
//				{
//					char str[256];
//					sprintf(str, "��WorldServer ����%d��Ϣ ����У�����",pMsg->GetType());
//					PutDebugString(str);
//					m_nSize=0;
//					return;
//				}
//				m_RecvMessages.PushMessage( pMsg );
//
//				m_nSize -= lRLESize;
//				lPoint += lRLESize;		// �ƶ�������ָ��
//				// ����ǳ�������Ϣ���ѻ�������С��������
//				if( m_nBufferSize > MAX_MESSAGE_LENGTH && m_nSize <= MAX_MESSAGE_LENGTH )
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
//
//				SAFE_DELETE(m_pBuffer);
//				m_nBufferSize = lRLESize;
//				m_pBuffer = new char[m_nBufferSize];
//				memcpy(m_pBuffer, pTemp, m_nSize);
//				SAFE_DELETE(pTemp);
//
//				char str[256];
//				sprintf(str, "��������Ϣ Size = %d", m_nBufferSize);
//				PutDebugString(str);
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

void CMyNetClient::SetSendRevBuf()
{
	int nValue = 1024*1024*3;
	int nSize  = sizeof(nValue);
	setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&nValue,nSize);
}