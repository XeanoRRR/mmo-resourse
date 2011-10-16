#include "stdafx.h"
#include <mmsystem.h>
#include "serverClient.h"
#include "Servers.h"
#include "../PUBLIC/Crc32Static.h"

#ifdef _SERVER
#include "netserver\message.h"
#endif

#ifdef _LOGIN_SERVER_
#include "netlogin\message.h"
#endif

#ifdef _WORLD_SERVER_
#include "networld\message.h"
#endif

#ifdef _AUTH_SERVER_
#include "netauth\Message.h"
#endif

#ifdef _BILLING_SERVER_
#include "NetBilling\Message.h"
#endif

//#include "gameserver/app/public/tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerClient::CServerClient(CServer* pServers)
:m_pServers(pServers)
,m_strMap("")
{
	assert(m_pServers);
	m_bLost = false;
	m_bQuit = false;
	m_nSize = 0;
	m_bServerType = 0;
	m_lMapID=0;

	m_dwStartTime = timeGetTime();
	m_dwSendStartTime = m_dwStartTime;
	m_lTotalSize = 0;
	m_lSizePerSecond = 0;

	m_lCurSendBufferSize = 0;
	m_lMaxSendBuffSize=0;

	m_lTotalSendSize = 0;
	m_lIOOperatorNum = 0;

	m_bCloseFlag = false;
}

CServerClient::~CServerClient()
{
	SAFE_DELETE(m_pSendBuffer);
}

//**********************************
//�����ɶ˿����յ�������
//**********************************
bool CServerClient::ReadFromCompletionPort(PER_IO_OPERATION_DATA* pIOData)
{	
	//memset( &pIOData->OverLapped,0,sizeof(OVERLAPPED) );

	pIOData->DataBuf.len = pIOData->nBufSize;
	pIOData->DataBuf.buf = pIOData->pBuffer;
	pIOData->OperationType = SOT_Receive;
	DWORD dwFlag = 0;
	DWORD dwReceivByte;
	int nRet = WSARecv(m_hSocket,&(pIOData->DataBuf),1,&dwReceivByte,&dwFlag,&(pIOData->OverLapped),NULL);
	if(nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}



// [TCP]������������ɶ˿�
int CServerClient::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	if(m_lCurSendBufferSize <= 0)
	{
		return true;
	}

	PER_IO_OPERATION_DATA* pPerIOData = (PER_IO_OPERATION_DATA*)calloc(1,sizeof(PER_IO_OPERATION_DATA));
	//�����䲻���ڴ�ʱ
	if(pPerIOData == NULL)
	{
#ifdef _DEBUG
		char str[200] = "Send MSG To Client, No Allocation.";
		PutDebugString(str);
#endif // _DEBUG

		return false;
	}

	char* pTemptBuf = new char[m_lCurSendBufferSize];
	memcpy(pTemptBuf,m_pSendBuffer,m_lCurSendBufferSize);

	pPerIOData->OperationType = SOT_Send;
	pPerIOData->nBufSize = m_lCurSendBufferSize;
	pPerIOData->pBuffer= (CHAR*)pTemptBuf;
	pPerIOData->DataBuf.buf=pPerIOData->pBuffer;
	pPerIOData->DataBuf.len=pPerIOData->nBufSize;
	pPerIOData->hSocket = m_hSocket;

	DWORD dwSentNum = 0;
	int ret = WSASend(m_hSocket, &(pPerIOData->DataBuf), 1, &dwSentNum, nFlags,
		(OVERLAPPED*)pPerIOData, NULL);

	if (ret == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();
		if ( nError != WSA_IO_PENDING)
		{
			free(pPerIOData);
			char str[200];
			sprintf(str, "Send To Client ERROR(errorID:%d)��",nError);
			PutDebugString(str);
			return false;
		}
	}

	m_pServers->AddPackageSize(m_lCurSendBufferSize);

	m_lCurSendBufferSize = 0;
	m_lIOOperatorNum++;
	//������ͻ��������ڹ涨����ԭ
	if(m_lSendBufferSize > m_lMaxSendBuffSize)
	{
		m_lSendBufferSize = m_lMaxSendBuffSize;
		SAFE_DELETE(m_pSendBuffer);
		m_pSendBuffer = new char[m_lSendBufferSize];

	}
	return true;
}


void CServerClient::AddReceiveData(CHAR* pBuf,int nBufSize)
{
	if(pBuf == NULL || nBufSize <= 0)
		return;

	if(m_nSize + nBufSize <= m_nBufferSize)
	{
		memcpy(&m_pBuffer[m_nSize],pBuf,nBufSize);
		m_nSize = m_nSize+nBufSize;
	}
	else
	{
		CHAR* pNewBuf = new CHAR[m_nSize+nBufSize];
		memcpy(&pNewBuf[0],&m_pBuffer[0],m_nSize);
		memcpy(&pNewBuf[m_nSize],pBuf,nBufSize);

		delete []m_pBuffer;
		m_pBuffer = pNewBuf;
		m_nBufferSize = m_nSize+nBufSize;
		m_nSize = m_nBufferSize;
	}
}

bool CServerClient::AddSendData(void* pBuf,int nBuffSize)
{
	if(pBuf == NULL)	return false;

	//�Ѿ���ʾ�˹ر�
	if( GetCloseFlag() == true)
		return true;

	if(m_lCurSendBufferSize + nBuffSize > m_pServers->GetMaxClientSendBufSize() )
	{
	 	return false;
	}

	if(m_lCurSendBufferSize + nBuffSize <= m_lSendBufferSize)
	{
		memcpy(m_pSendBuffer+m_lCurSendBufferSize,pBuf,nBuffSize);
	}
	else
	{
		char* pTemptBuf = new char[m_lCurSendBufferSize];
		memcpy(pTemptBuf,m_pSendBuffer,m_lCurSendBufferSize);

		SAFE_DELETE(m_pSendBuffer);
		m_lSendBufferSize = (m_lCurSendBufferSize+nBuffSize)*2;
		m_pSendBuffer = new char[m_lSendBufferSize];

		memcpy(m_pSendBuffer,pTemptBuf,m_lCurSendBufferSize);
		memcpy(m_pSendBuffer+m_lCurSendBufferSize,pBuf,nBuffSize);

		SAFE_DELETE(pTemptBuf);
	}
	m_lCurSendBufferSize = m_lCurSendBufferSize+nBuffSize;
	return true;
}
//-------------------------------------------------------------
// ʹ����ɶ˿�
//-------------------------------------------------------------
void CServerClient::OnReceive(int nErrorCode)
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
			sprintf(str, "%dSend MSG, Check Length ERROR.",GetMapID());
			PutDebugString(str);
			m_nSize=0;
			return;
		}

		// ���������ݴ��ڵ���һ����Ϣ�ĳ���
		if( lRLESize <= m_nSize )
		{
			CMessage* pMsg = CMessage::CreateMessage(&m_pBuffer[lPoint]+12, lRLESize-12);

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
					sprintf(str, "%d Send MSG %d, Check Content ERROR.",GetMapID(),pMsg->GetType());
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
				sprintf(str, "CreateMessage ERROR");
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
				sprintf(str, "ERROR MSG(RleSize=%d), , Discard Recent Data.", lRLESize);
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

#ifdef _DEBUG
				char str[256];
				sprintf(str, "\nOverflaw MSG Size = %d\n", m_nBufferSize);
				OutputDebugString(str);
#endif
				return;
			}

#ifdef _DEBUG
			char str[256];
			sprintf(str, "\nHalf-baked MSG m_nSize = %d, Point = %d\n", m_nSize, lPoint);
			OutputDebugString(str);
#endif
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

long	CServerClient::AddPackageSize(long lSize)
{
	m_lTotalSize += lSize;
	//�������ۼƴ�50K��ʱ��ʼͳ��
	if(m_lTotalSize >= 51200)
	{
		DWORD dwTime = timeGetTime();
		DWORD Time = dwTime-m_dwStartTime;
		if(Time >= 1)
		{
			m_lSizePerSecond = m_lTotalSize*1000/Time;
			m_dwStartTime = dwTime;
			m_lTotalSize = 0;
		}
	}
	return m_lSizePerSecond;
}

void CServerClient::SetSendRevBuf()
{
	//���ڲ��׽��ֵķ��ͻ�������Ϊ0
	//int nValue = 0;
	//int nSize  = sizeof(nValue);
	//setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&nValue,sizeof(nSize));
	//setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*)&nValue,sizeof(nSize));
}

long CServerClient::GetSendSizePerSecond()
{
	DWORD dwTime = timeGetTime();
	DWORD Time = dwTime-m_dwStartTime;
	if(Time <= 0)
	{
		Time = 1;
	}
	return  m_lTotalSendSize*1000/Time;
}