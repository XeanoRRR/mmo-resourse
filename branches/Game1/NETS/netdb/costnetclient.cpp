#include "stdafx.h"
#include "costnetclient.h"
#include "message.h"
#include "../PUBLIC/Crc32Static.h"
#include <sys\timeb.h>
CSocketCommands		CostSocketOperaCommands;		//������������������

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCostNetClient::CCostNetClient()
{	
	m_bIsEverConnectSuccess=false;
	m_bIsRecvConnectMsg=false;
}

CCostNetClient::~CCostNetClient()
{
}

void CCostNetClient::HandleClose()
{
	CMessage* msg;
	msg = new CMessage(MSG_W2W_SERVER_COSTSERVERCLOSE);
	GetRecvMessages()->PushMessage(msg);
}

CBaseMessage* CCostNetClient::CreateMsg(void* pBuf, unsigned long nSize)
{
	return CBaseMessage::CreateWithoutRLE(pBuf, nSize);
}

//void CCostNetClient::OnReceive(int nErrorCode)
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
//
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
//			sprintf(str, "��Cost Server������Ϣ������У�����");
//			PutDebugString(str);
//			m_nSize=0;
//			return;
//		}
//
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
//					sprintf(str, "��Cost Server����%d��Ϣ ����У�����",pMsg->GetType());
//					PutDebugString(str);
//					m_nSize=0;
//					return;
//				}
//				m_RecvMessages.PushMessage( pMsg );
//				m_nSize -= lRLESize;
//				lPoint += lRLESize;		// �ƶ�������ָ��
//				// ����ǳ�������Ϣ���ѻ�������С��������
//				if( m_nBufferSize > MAX_MESSAGE_LENGTH && m_nSize <= MAX_MESSAGE_LENGTH )
//				{
//					char *pTemp = new char[m_nSize];
//					memcpy(pTemp, &m_pBuffer[lPoint], m_nSize);
//					SAFE_DELETE(m_pBuffer);
//					m_nBufferSize = MAX_MESSAGE_LENGTH;
//					m_pBuffer = new char[m_nBufferSize];
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
//
//				char str[256];
//				sprintf(str, "��������Ϣ Size = %d", m_nBufferSize);
//				PutDebugString(str);
//				return;
//			}
//			char str[256];
//			sprintf(str, "�յ�����������Ϣ m_nSize = %d,Point = %d,m_nBufferSize =%d",
//				m_nSize, lPoint,m_nBufferSize);
//			PutDebugString(str);
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


CSocketCommands& CCostNetClient::GetSocketCommand()
{
	return CostSocketOperaCommands;
}

long CCostNetClient::GetCostTimePoint()
{
	struct _timeb stCostTime;
	_ftime(&stCostTime);
	return stCostTime.time-stCostTime.timezone*60;
}