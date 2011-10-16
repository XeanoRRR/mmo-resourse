#include "stdafx.h"
#include "mmsystem.h"
#include "MySocket.h"
#include "basemessage.h"
#include "MsgDescribe.h"

#ifdef _DEBUG
#ifdef _CLIENT
#include "netclient/Message.h"
#endif
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")


//------------------------------------------------------------------
// ��ʼ��Winsock 2.0
//------------------------------------------------------------------
BOOL CMySocket::MySocketInit()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
	{
		OutputDebugString("WSAStartup FAILED!");
		return FALSE;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		OutputDebugString("Is not Winsock 2.0!");
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------
// �����˳�ʱ����
//------------------------------------------------------------------
void CMySocket::MySocketClearUp()
{
	WSACleanup();
}

/////////////////////////////////////////////////////////////////////////////
// CMySocket Construction

CMySocket::CMySocket()
{
	m_lProtocol = SOCK_STREAM;
	m_dwIP = 0;
	m_dwPort = 5000;
	strcpy(m_strIP, "127.0.0.1");
	m_hSocket = INVALID_SOCKET;
			
	m_dwLastRecvPort = 0;		// [UDP] ��һ���յ�����Ϣ�Ķ˿�
	m_strLastRecvIP[0] = 0;		// [UDP] ��һ���յ�����Ϣ��IP
}

CMySocket::~CMySocket()
{
	if( m_hSocket != INVALID_SOCKET )
	{
		int nRet = closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

//------------------------------------------------------------------
// ����SOCKET
//------------------------------------------------------------------
BOOL CMySocket::Create(UINT nSocketPort,			// �˿�
					   LPCTSTR lpszSocketAddress,	// ��ַ					   
					   int nSocketType,				// ����( TCP:SOCK_STREAM UDP:SOCK_DGRAM )
					   long lEvent,
					   bool bAsyncSelect  //defaultly,the flag==false,if create for client socket,set the flag=true
					   )
{
	return FALSE;
}


//���� overlapped socket
BOOL CMySocket::WSACreate(UINT nSocketPort,LPCTSTR lpszSocketAddress,int nSocketType)
{
	m_dwPort = nSocketPort;
	m_lProtocol = nSocketType;
	//����һ�� overlapped socket
	m_hSocket = WSASocket(AF_INET,nSocketType,0,NULL,0,WSA_FLAG_OVERLAPPED);
		
	//��������
	if(m_hSocket == INVALID_SOCKET)
	{
		char str[256];
		sprintf(str, "WSACreate Socket Error = %d",WSAGetLastError());
		PutDebugString(str);
		return FALSE;
	}

	//��˿ں͵�ַ
	if(!Bind(nSocketPort, lpszSocketAddress))
	{
		char str[256];
		sprintf(str, "WSACreate Socket Bind Error = %d",WSAGetLastError());
		PutDebugString(str);

		return FALSE;
	}

	return TRUE;
}


// �󶨶˿ں͵�ַ
BOOL CMySocket::Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress, int reuseaddr, int dontlinger)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = (LPTSTR)lpszSocketAddress;
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
	{
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		DWORD lResult = inet_addr(lpszAscii);
		if (lResult == INADDR_NONE)
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}

	sockAddr.sin_port = htons((u_short)nSocketPort);

	// winsock2Ĭ�� reuseaddr = 1 dontlinger = 0(linger = 1)

	// �ͻ��� ����� addr ������ ����reuseaddr = 1 dontlinger = 0
	// ���Ҫ�Ѿ���������״̬��soket�ڵ���closesocket��ǿ�ƹرգ�������TIME_WAIT�Ĺ��̣�
	if( ::setsockopt(m_hSocket, SOL_SOCKET, SO_DONTLINGER, (char*)&dontlinger, sizeof(dontlinger)) < 0 )
	{
		long error = WSAGetLastError();

		char str[256];
		sprintf(str, "Bind Error = %d",error);
		PutDebugString(str);

		return FALSE;
	}

	if( ::setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseaddr, sizeof(reuseaddr)) < 0 )
	{
		long error = WSAGetLastError();

		char str[256];
		sprintf(str, "Bind Error = %d",error);
		PutDebugString(str);

		return FALSE;
	}

	long hr = bind(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
		if(  hr == SOCKET_ERROR )
	{
		long error = WSAGetLastError();

		char str[256];
		sprintf(str, "Bind Error = %d",error);
		PutDebugString(str);

		return FALSE;
	}
	return TRUE;
}


// ----------------------------------------------------
// �ر�Socket
// �ȵȴ�SocketThread�߳̽���
// �ú���������SocketThread�߳��е���
// ----------------------------------------------------
BOOL CMySocket::Close()
{
	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	return TRUE;
}

// �ر�socket (�յ�FD_CLOSE)
void CMySocket::OnClose(int nErrorCode)
{
	//if (m_hSocket != INVALID_SOCKET)
	//{
	//	closesocket(m_hSocket));
	m_bClose = true;
	//}
} 

// [TCP] ��������
int CMySocket::Recv(void* lpBuf, int nBufLen, int nFlags)
{
	int rt = recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
	if( rt == SOCKET_ERROR )
	{
		long error = WSAGetLastError();
		if( error != WSAEWOULDBLOCK )
		{
			char str[256];
			sprintf(str,"recv ERROR : %d",error);
			PutDebugString(str);
		}
	}
	return rt;
}

// [TCP] ��������
int CMySocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	return true;
}


// [UDP] ��������
int CMySocket::RecvFrom(void* lpBuf, int nBufLen, char* strIP, DWORD& dwPort, int nFlags )
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	int framlen = sizeof(addr);
	int rt = recvfrom(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, (sockaddr*)&addr, &framlen);
	if( rt == SOCKET_ERROR )
	{
		long error = WSAGetLastError();
		if( error == WSAEWOULDBLOCK )
		{
			return 0;
		}
		char str[256];
		sprintf(str, "Socket RecvFrom Error = %d",error);
		PutDebugString(str);
		return FALSE;
	}

	strcpy(strIP, inet_ntoa(addr.sin_addr));
	dwPort = ntohs(addr.sin_port);
	return rt;
}

// [UDP] ��������
int CMySocket::Sendto(const void* lpBuf, int nBufLen, const char* strIP, DWORD dwPort, int nFlags)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((u_short)dwPort);
	addr.sin_addr.S_un.S_addr = inet_addr(strIP);

_Begin:
	if( sendto(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		long error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			goto _Begin;
		}
		return FALSE;
	}
	return TRUE;
}

// ��ȡ���׽��������ĵ�ַ
BOOL CMySocket::GetPeerName(char* rPeerAddress, UINT& rPeerPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_hSocket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (!bResult)
	{
		rPeerPort = ntohs(sockAddr.sin_port);
		strcpy(rPeerAddress, inet_ntoa(sockAddr.sin_addr));
	}
	return bResult;
}

// ��ȡ���ص�ַ
BOOL CMySocket::GetSockName(char* rSocketAddress, UINT& rSocketPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getsockname(m_hSocket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (!bResult)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		strcpy(rSocketAddress, inet_ntoa(sockAddr.sin_addr));
	}
	return bResult;
}

// ��ȡ���л��������ݵĴ�С
BOOL CMySocket::IOCtl(long lCommand, DWORD* lpArgument)
{
	int rt = ioctlsocket(m_hSocket,lCommand,lpArgument);
	if (rt == SOCKET_ERROR)
	{
		long error = WSAGetLastError();
		return FALSE;
	}
	return TRUE;
}


#ifdef _DEBUG
#ifdef _CLIENT
VOID CMySocket::Log( CMessage* pMsg, BOOL bIsSendMessage )
{
	if( !bIsLogFileOpened ) return;

	if( bIsSendMessage )
	{
		ofLogFile << "SM ";
	}
	else
	{
		ofLogFile << "RM ";
	}

	//##д��ʱ��
	DWORD dwTime = timeGetTime();
	ofLogFile << dwTime << ": ";

	if( pMsg )
	{
		ofLogFile << CMsgDescribe::GetInstance() -> GetDesc( pMsg -> GetType() ) << " Hex: ";
		CHAR szText[256];
		for ( size_t i = 0; i < pMsg -> GetSize(); i ++ )
		{
			sprintf( szText, "%X ", pMsg -> GetMsgBuf()[i] );
			ofLogFile.write( szText, static_cast<std::streamsize>( strlen(szText) ) );
		}
		//ofLogFile << " Msg: \"";
		//ofLogFile.write( const_cast<char*>( reinterpret_cast<char*>( pMsg -> GetMsgBuf() ) ), pMsg -> GetSize() );
		//ofLogFile << "\" " << endl;
		ofLogFile << endl;
	}
	else
	{
	}
}
#endif
#endif


long GetSocketID()
{
	static long lSocketID = 0;
	return ++lSocketID;
}

unsigned long STR2UL(const char *szIP)
{
	return inet_addr(szIP);
}