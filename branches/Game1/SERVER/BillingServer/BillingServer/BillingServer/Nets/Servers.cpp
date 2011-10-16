#include "stdafx.h"
#include "mmsystem.h"
#include "Servers.h"
#include "Mysocket.h"
#include "serverClient.h"
#include <time.h>


#ifdef _SERVER
#include "netserver\myserverclient.h"
#include "netserver\message.h"
#endif

#ifdef _LOGIN_SERVER_
#include "netlogin\myserverclient.h"
#include "netlogin\message.h"
#endif

#ifdef _WORLD_SERVER_
#include "networld\MyServerClient.h"
#include "networld\message.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//HANDLE g_hCompletionPort;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CServer::CServer()
: _cl_available(false)
{
	m_bNetMainThreadExit=false;
	m_bAcceptThreadExit=false;
	m_bWorkerThreadsExit=false;
	m_lClientSocketCount = 0;

	m_hNetMainTheads=0;
	m_hAcceptThread = 0;	
	m_hWorkerThreads.clear();

	m_dwAcceptThreadSleepTime = 100;
	m_dwNetMainThreadSleepTime= 1;

	m_bCheck = false;
	m_bCheckMsgCon=false;
	m_lSizePerSecond = 409600000;
	m_lForbidTime = 0;	
	m_ForbidIPs.clear();
	m_lMaxBlockConnetNum = SOMAXCONN;
	m_lSendInterTime = 8000;

	m_dwStartTime = timeGetTime();				//ͳ�ƿ�ʼʱ��
	m_lTotalSendSize = 0;						//���͵�������
	m_lSendSizePerSecond = 0;					//���뷢�͵�����
	m_llSendSize = 0;
	m_dwRecvStartTime = timeGetTime();
	m_lTotalRecvSize = 0;
	m_lRecvSizePerSecond = 0;
	m_llRecvSize = 0;
	m_lMaxClientConNum = 100;					//���Ŀͻ�����������
	m_lMaxIPOperatorNum = 1;					//��ÿ���ͻ������ķ��Ͳ�������
	m_lPermitMaxClientSendBufSize = 1024*50;				//��ͻ��˷��͵Ļ��������ֵ

}

CServer::~CServer()
{	
	map<long, AllowedClient *>::iterator it = m_MapIDSocketID.begin();
	for( ; it!=m_MapIDSocketID.end(); ++it )
	{
		SAFE_DELETE(it->second);
	}

	CloseHandle( m_hAcceptThread );
	CloseHandle(m_hNetMainTheads);

	for(int i=0;i<(int)m_hWorkerThreads.size();i++)
	{
		CloseHandle(m_hWorkerThreads[i]);
	}


	CloseHandle(m_hCompletionPort);
}

//�����������߳�
BOOL CServer::CreateNetMainThread()
{
	g_NewAcceptSockets.clear();
	unsigned threadID;
	m_hNetMainTheads = (HANDLE)_beginthreadex(NULL,0,NetThreadFunc,this,0,&threadID);

	if(m_hNetMainTheads==NULL)
		return FALSE;
	return TRUE;
}
//��������ɶ˿��ϵȴ��Ĺ������߳�
BOOL CServer::CreateWorkerThreads(int nProcNum)
{
	//	DWORD dwThreadId;

	for( int i=0;i<nProcNum;i++ )
	{
		unsigned threadID;
		HANDLE hWorkerThread = (HANDLE)_beginthreadex(NULL,0,WorkerThreadFunc,this,0,&threadID);

		m_hWorkerThreads.push_back(hWorkerThread);

		if(hWorkerThread==NULL)
			return FALSE;
	}

	char str[256]="";
	sprintf(str,"The server has created %d IOCP worker threads!",nProcNum);
	PutDebugString(str);
	return TRUE;
}


//�������ܿͻ�socket�߳�
BOOL CServer::CreateAcceptThread()
{
	unsigned threadID;
	m_hAcceptThread = (HANDLE)_beginthreadex(NULL,0,AcceptThreadFunc,this,0,&threadID);

	if(m_hAcceptThread==NULL)
		return FALSE;
	return TRUE;
}

//�˳�������׽���
void CServer::Exit()
{
}

//�����˳���ɶ˿ڹ������̱߳�־
void CServer::ExitWorkerThread(void)
{
	//�����߳��˳�
	//m_bAcceptThreadExit=true;
	//WaitForSingleObject(m_hAcceptThread,INFINITE);
	TerminateThread(m_hAcceptThread,0);

	tagSocketOper* pSocketOpera = new tagSocketOper(SOCKETOPERATYPE_QUITALL,0,NULL,0);
	m_SocketOperaCommands.Push_Back(pSocketOpera);

	while(m_Clients.size()>0)
	{
		Sleep(100);
	}


	//��ɶ˿ڵĹ����߳��˳�
	int nSize = (int)m_hWorkerThreads.size();
	for(int i=0;i < nSize; i++)
	{
		//���������̷߳����˳���Ϣ
		PostQueuedCompletionStatus(m_hCompletionPort,0,0,0);
	}

	WaitForMultipleObjects((DWORD)m_hWorkerThreads.size(),&m_hWorkerThreads[0],TRUE,INFINITE);

	m_bNetMainThreadExit=true;
	//�Ⱥ��������߳����
	WaitForSingleObject(m_hNetMainTheads,INFINITE);
	//TerminateThread(m_hNetMainTheads,0);
}


// [TCP] ����������رյ�����
void CServer::OnClose(int nErrorCode)
{
	CMySocket::OnClose(nErrorCode);
	OnError(nErrorCode);
}

//Server��������ɶ˿�ģʽ��ʱ��ʹ�ø÷�������OnAccept
void CServer::OnAccept(int nErrorCode)
{
	if(m_lClientSocketCount >= m_lMaxClientConNum)
	{
		Sleep(1000);
		return;
	}
	SOCKET clientSocket;
	struct sockaddr_in stClientAddress;
	int nClientAddressLen=sizeof(stClientAddress);

	clientSocket=WSAAccept(m_hSocket,(struct sockaddr*)&stClientAddress,&nClientAddressLen,NULL,0);


	if(clientSocket==INVALID_SOCKET)
	{
		DWORD dwEorror = WSAGetLastError();
		return;
	}

	long lMapID = 0;
	if( _cl_available )
	{
		lMapID = CheckAllowedClient( stClientAddress );
		if( !lMapID )
		{
			closesocket(clientSocket);
			AddLogText( "Invalid Client %s:%d!", inet_ntoa(stClientAddress.sin_addr), ntohs(stClientAddress.sin_port) );
			return;
		}
	}

	//�쿴��IP�Ƿ񱻽�ֹ
	if(m_bCheck && FindForbidIP(stClientAddress.sin_addr.S_un.S_addr) == true)
	{
		closesocket(clientSocket);
		return;
	}

	//����
	CServerClient* pServerClient = CreateServerClient();
	pServerClient->m_hSocket = clientSocket;

	//����IP
	pServerClient->SetIP(inet_ntoa(stClientAddress.sin_addr));
	pServerClient->SetDwordIP(stClientAddress.sin_addr.S_un.S_addr);

	//����Socket��indexID
	if( !lMapID )
	{
		lMapID = GetSocketID();
	}
	pServerClient->SetIndexID(lMapID);
	pServerClient->SetMapID(lMapID);
	pServerClient->SetSendRevBuf();

	tagSocketOper *pSocketOper = new tagSocketOper(SOCKETOPERATYPE_ADD,lMapID,pServerClient,0);
	m_SocketOperaCommands.Push_Back(pSocketOper);

	/// added by Kevin Lynx 1.25.2008
	/// ��Ȼ��ͻ��˶Ͽ�����ʱ��Ҫ����ServerClient::OnClose
	/// ��ô�ս�������ʱ��ҲӦ�õ���ServerClient::OnAccept
	pServerClient->OnAccept();
}

CServerClient* CServer::CreateServerClient()
{
	return new CServerClient(this);
}

// error��ʱ�����
void CServer::OnError(int errorCode)
{
	if (errorCode != 0)
	{
		char str[256];
		sprintf(str, "OnError Server code = %d", errorCode);
		PutDebugString(str);
	}
}

// [UDP] �ڽ��յ���Ϣʱ����
void CServer::OnReceive(int nErrorCode)
{
	//char m_pBuffer[1024];
	///RecvFrom(m_pBuffer, 1024, m_strLastRecvIP, m_dwLastRecvPort);
}

// [TCP] ���͹㲥
int CServer::SendAll(void *buffer, unsigned long size)
{
	if(size <= 0 )	return false;

	char* pBuf = new char[size];
	memcpy(pBuf,buffer,size);

	tagSocketOper *pSocketOper = new tagSocketOper(SOCKETOPERATYPE_SENDALL,0,pBuf,size);
	m_SocketOperaCommands.Push_Back(pSocketOper);
	return 1;
}

// ��ĳ��SocketID��������
int CServer::SendBySocketID(long lSocketID, void *buffer, unsigned long size)
{
	if(size <= 0 )	return false;

	char* pBuf = new char[size];
	memcpy(pBuf,buffer,size);
	tagSocketOper *pSocketOper = new tagSocketOper(SOCKETOPERATYPE_SENDTOSOCKET,lSocketID,pBuf,size);
	m_SocketOperaCommands.Push_Back(pSocketOper);

	return true;
}

// ��ĳ��MapID��������
int CServer::SendByMapID(long lMapID, void *buffer, unsigned long size)
{
	if(size <= 0 )	return false;

	char* pBuf = new char[size];
	memcpy(pBuf,buffer,size);

	tagSocketOper *pSocketOper = new tagSocketOper(SOCKETOPERATYPE_SENDTOMAPID,lMapID,pBuf,size);
	m_SocketOperaCommands.Push_Back(pSocketOper);

	return true;
}

//��ĳ��MapStr��������
int CServer::SendByMapStr(const char* strMap, void *buffer, unsigned long size)
{
	if(size <= 0 || strMap ==NULL)	return false;

	char* pBuf = new char[size];
	memcpy(pBuf,buffer,size);

	int strMapLen = lstrlen(strMap);
	char* strMapBuf = new char[strMapLen+1];
	strncpy(strMapBuf,strMap,strMapLen);
	strMapBuf[strMapLen]='\0';

	tagSocketOper *pSocketOper = new tagSocketOper(SOCKETOPERATYPE_SENDTOMAPSTR,
		0,strMapBuf,pBuf,size);
	m_SocketOperaCommands.Push_Back(pSocketOper);

	return true;
}



// [UDP] ��ĳһ��ip������Ϣ
int CServer::Sendto(void *buffer, unsigned long size, const char* strIP, DWORD dwPort)
{
	return CMySocket::Sendto(buffer, size, strIP, dwPort);
}

// ���� Listen Socket���������״̬���ȴ��ͻ��˵�����
BOOL CServer::Host(UINT dwPort, const char* strIP, DWORD nSocketType,bool bUseCP)
{
	// ���� socket
	BOOL bRet = WSACreate(dwPort, strIP,nSocketType);	
	//����ʧ��
	if(!bRet)
	{
		return bRet;
	}

	//�õ�CPU������
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	//����cpu����������ɶ˿�
	m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,sysInfo.dwNumberOfProcessors);

	//������ɶ˿�ʧ��
	if(m_hCompletionPort==NULL)
	{
		PutDebugString("ERROR : Create IO completion port FAILED!");
		return FALSE;
	}

	//�����������߳�
	bRet = CreateNetMainThread();
	if(!bRet)
	{
		PutDebugString("ERROR : Create the net main thread FAILED!");
		return FALSE;
	}

	//��������ɶ˿��ϵȴ��Ĺ������߳�
	bRet=CreateWorkerThreads(sysInfo.dwNumberOfProcessors);	
	if(!bRet)
	{
		PutDebugString("ERROR : Create IOCP worker thread FAILED!");
		return FALSE;
	}

	//�������տͻ�socket�߳�
	bRet=CreateAcceptThread();
	if(!bRet)
	{
		PutDebugString("ERROR : Create accept-thread FAILED!");
		return FALSE;
	}

	// listen
	bRet = Listen(m_lMaxBlockConnetNum);
	if(!bRet)
	{
		PutDebugString("ERROR : socket listen FAILED!");
		return FALSE;
	}

	return bRet;
}

// [������]
BOOL CServer::Listen(int nConnectionBacklog)
{
	// [TCP]
	if( GetProtocol() == SOCK_STREAM )
	{
		if(SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog))
		{
			return TRUE;
		}
	}
	return FALSE;
}


// �ϵ�ĳ���׽���
int CServer::QuitClientBySocketID(long lSocketID)
{
	tagSocketOper* pSocketOper = new tagSocketOper(SOCKETOPERATYPE_QUITBYSOCKETID,lSocketID,NULL,0);
	m_SocketOperaCommands.Push_Back(pSocketOper);
	return true;
}

// �ϵ�ĳ��ӳ���ID�Ŀͻ���
int CServer::QuitClientByMapID(long lMapID)
{
#ifdef _DEBUG
	char str[256];
	sprintf(str, "The server disconnect from a client(MapID:%d)\n",lMapID);
	OutputDebugString(str);
#endif

	tagSocketOper* pSocketOper = new tagSocketOper(SOCKETOPERATYPE_QUITBYMAPID,lMapID,NULL,0);
	m_SocketOperaCommands.Push_Back(pSocketOper);
	return true;
}

// �ϵ�ĳ��ӳ���ID�Ŀͻ���
int CServer::QuitClientByMapStr(const char* strMap)
{
	int strMapLen = lstrlen(strMap);
	char* strMapBuf = new char[strMapLen+1];
	strncpy(strMapBuf,strMap,strMapLen);
	strMapBuf[strMapLen]='\0';

	tagSocketOper* pSocketOper = new tagSocketOper(SOCKETOPERATYPE_QUITBYMAPSTR,0,strMapBuf,NULL,0);
	m_SocketOperaCommands.Push_Back(pSocketOper);
	return true;
}


int CServer::OnQuitOneClient(long lSocketID)
{
	CServerClient* pClient = FindOneClient(lSocketID);
	if(pClient)
	{
		//pClient->Close();
		pClient->SetCloseFlag(true);
	}	
	return true;
}
int CServer::OnQuitAllClient()
{
	SocketIDClient::iterator it = m_Clients.begin();
	for(;it != m_Clients.end();it++)
	{
		CServerClient* pClient = (*it).second;
		if(pClient)
			pClient->SetCloseFlag(true);
	}
	return true;
}

//���ÿͻ����׽��ֵ�PlayerID
int CServer::SetClientMapID(long lSocketID,long lMapID)
{
	if( !lMapID )
		lMapID = GetSocketID();
	tagSocketOper* pSocketOper =
		new tagSocketOper(SOCKETOPERATYPE_PLAYERJOIN,lSocketID,NULL,lMapID);
	m_SocketOperaCommands.Push_Back(pSocketOper);
	return true;
}

//���ÿͻ����׽��ֵ�PlayerID
int CServer::SetClientMapStr(long lSocketID,const char* strMap)
{
	int strMapLen = lstrlen(strMap);
	char* strMapBuf = new char[strMapLen+1];
	strncpy(strMapBuf,strMap,strMapLen);
	strMapBuf[strMapLen]='\0';

	tagSocketOper* pSocketOper =
		new tagSocketOper(SOCKETOPERATYPE_CDKEYJOIN,lSocketID,strMapBuf,NULL,0);
	m_SocketOperaCommands.Push_Back(pSocketOper);

	return true;
}


int CServer::GetSocketIDByMapStr(const char* strMap)
{
	if(m_MapStrSocketID.find(strMap) != m_MapStrSocketID.end())
	{
		return m_MapStrSocketID[strMap];
	}
	return true;
}
//����MapID�õ�SocketID
long CServer::GetSocketIDByMapID(long lMapID)
{
	if(m_MapIDSocketID.find(lMapID) != m_MapIDSocketID.end())
	{
		return m_MapIDSocketID[lMapID]->_sock;
	}
	return 0;
}

//�Ƿ���ڶ�ӦMapID�Ŀͻ���
AllowedClient *CServer::IsExistMapID(long lID)
{
	map<long, AllowedClient *>::iterator it = m_MapIDSocketID.find(lID);
	if( it != m_MapIDSocketID.end() )
		return it->second;
	return NULL;
}

//�Ƿ���ڶ�ӦMapstr�Ŀͻ���
bool CServer::IsExistMapStr(const char* strMap)
{
	if(m_MapStrSocketID.find(strMap) != m_MapStrSocketID.end())
		return true;
	return false;
}
// ���һ���ͻ���
bool CServer::AddAClient(long lSocketID,CServerClient* pClient)
{
	m_lClientSocketCount++;
	SocketIDClient::iterator it = m_Clients.find(lSocketID);
	if(  it != m_Clients.end())
	{
		delete (*it).second;
		m_Clients.erase(it);
	}

	m_Clients[lSocketID] = pClient;
	return true;
}

// ����һ���ͻ��ˣ��������꣬-1��ʾû��
CServerClient* CServer::FindOneClient(long lSocketID)
{

	SocketIDClient::iterator it = m_Clients.find(lSocketID);
	if(  it != m_Clients.end())
	{
		return (*it).second;
	}
	return NULL;
}

// ɾ��һ���ͻ���
bool CServer::DelOneClient(long lSocketID)
{
	CServerClient* pClient = FindOneClient(lSocketID);
	if(pClient)
	{
		m_lClientSocketCount--;
		pClient->OnClose(0);//close the client socket
		m_Clients.erase(lSocketID);
		delete pClient;
		return true;
	}	
	return false;
}


void	CServer::AddForbidIP(u_long laddr)
{
	m_ForbidIPs[laddr] = timeGetTime();
}

bool	CServer::FindForbidIP(u_long laddr)
{
	map<u_long,DWORD>::iterator it = m_ForbidIPs.find(laddr);
	if(it != m_ForbidIPs.end())
	{
		DWORD dwTime = timeGetTime();
		if(dwTime-(*it).second <= (DWORD)m_lForbidTime)
		{
			return true;
		}
		m_ForbidIPs.erase(laddr);
	}
	return false;
}

long	CServer::AddPackageSize(long lSize)
{
	m_lTotalSendSize += lSize;
	m_llSendSize += lSize;
	//�������ۼƴ�10M��ʱ��ʼͳ��
	if(m_lTotalSendSize >= 10485760)
	{
		DWORD dwTime = timeGetTime();
		DWORD Time = dwTime-m_dwStartTime;
		if(Time >= 1)
		{
			m_lSendSizePerSecond = m_lTotalSendSize*1000/Time;
			m_dwStartTime = dwTime;
			m_lTotalSendSize = 0;
		}
	}
	return m_lSendSizePerSecond;
}

long	CServer::AddRecvSize(long lSize)
{
	m_lTotalRecvSize += lSize;
	m_llRecvSize += lSize;
	//�������ۼƴ�10M��ʱ��ʼͳ��
	if(m_lTotalRecvSize >= 10485760)
	{
		DWORD dwTime = timeGetTime();
		DWORD Time = dwTime-m_dwRecvStartTime;
		if(Time >= 1)
		{
			m_lRecvSizePerSecond = m_lTotalRecvSize*1000/Time;
			m_dwRecvStartTime = dwTime;
			m_lTotalRecvSize = 0;
		}
	}
	return m_lRecvSizePerSecond;
}

void	CServer::AddNewAcceptSocket(DWORD dwSocketID)
{
	DWORD dwTime = timeGetTime();
	g_NewAcceptSockets[dwSocketID] = dwTime;
}
void	CServer::RemoveNewAcceptSocket(DWORD dwSocketID)
{
	g_NewAcceptSockets.erase(dwSocketID);
}
void	CServer::DoNewAcceptSocket()
{
	DWORD dwTime = timeGetTime();
	map<DWORD,DWORD>::iterator it = g_NewAcceptSockets.begin();
	for(;it != g_NewAcceptSockets.end();)
	{
		if( dwTime-(*it).second >= m_lSendInterTime )
		{
			QuitClientBySocketID((*it).first );
			it = g_NewAcceptSockets.erase(it);
		}
		else
		{
			it++;
		}
	}
}

/// load list of allowed clients
bool CServer::LoadAllowedClient( const char *filename )
{
	map<long, AllowedClient *>::iterator it = m_MapIDSocketID.begin();
	for( ; it!=m_MapIDSocketID.end(); ++it )
	{
		SAFE_DELETE(it->second);
	}
	m_MapIDSocketID.clear();

	char tmp[128];
	ifstream ifs;
	ifs.open( filename );
	if( !ifs.is_open() )
		return false;

	ifs >> tmp >> _cl_available;
	if( !_cl_available )
		return true;

	long mapid = 0;

	while( ReadTo( ifs, "#" ) )
	{
		AllowedClient *node = new AllowedClient;
		ifs >> tmp >> node->_port;
		strcpy(node->_ip, tmp);
		node->_sock = 0;
		m_MapIDSocketID[++mapid] = node;
	}

	return true;
}

/// check allowed clients
long CServer::CheckAllowedClient( const struct sockaddr_in &addr )
{
	AllowedClient node;
	strcpy(node._ip, inet_ntoa(addr.sin_addr));
	node._port = ntohs(addr.sin_port);

	map<long, AllowedClient *>::iterator it = m_MapIDSocketID.begin();
	for( ; it!=m_MapIDSocketID.end(); ++it )
	{
		if( !strcmp(it->second->_ip, node._ip) && it->second->_port == node._port )
			return it->first;
	}
	return 0;
}

// get client by socket id
AllowedClient *CServer::GetClientByID( unsigned long socket )
{
	map<long, AllowedClient *>::iterator it = m_MapIDSocketID.find(socket);
	if( it != m_MapIDSocketID.end() )
	{
		return it->second;

	}
	return NULL;
}

void DoWorkerThreadFunc(CServer* pServer)
{
	BOOL bResult;//completion port packet flag
	DWORD dwNumRead;//bytes num be readed
	ULONG CPDataKey;
	LPER_IO_OPERATION_DATA lpPerIOData;

	char strTempt[100]="";
	while(true)
	{
		//�ȴ���ɶ˿�ͨ��
		dwNumRead = -1;
		bResult=GetQueuedCompletionStatus(pServer->m_hCompletionPort,&dwNumRead,
			&CPDataKey,(LPOVERLAPPED*)&lpPerIOData,INFINITE);

		if(!bResult)
		{
			if(lpPerIOData == NULL && dwNumRead == -1)
			{
				DWORD dwError = GetLastError();
				char str[200];
				sprintf(str, "IOCP Thread Error, Not Read Completed Pack(bResult == false,lpPerIOData == NULL,ErrorID:%d).",dwError);
				PutDebugString(str);
			}
			else if(lpPerIOData)
			{
				DWORD dwError = GetLastError();
				char str[200];
				if(lpPerIOData->OperationType == SOT_Send)
				{
					sprintf(str, "IOCP Thread Send I/O Error(ErrorID:%d).",dwError);
					delete []lpPerIOData->pBuffer;
				}
				if(lpPerIOData->OperationType == SOT_Receive)
				{
					sprintf(str, "IOCP Thread Receive I/O Error(ErrorID:%d).",dwError);
					delete []lpPerIOData->pBuffer;
				}		
				//���һ��ɾ��Client�Ĳ�������
				tagSocketOper *pSocketOper =
					new tagSocketOper(SOCKETOPERATYPE_DELBYSOCKETID,CPDataKey,NULL,0);
				pServer->m_SocketOperaCommands.Push_Back(pSocketOper);
				free(lpPerIOData);

				if(dwError != 64)
					PutDebugString(str);
			}
			else
			{
				PutDebugString("IOCP Thread ERROR.");
			}
		}
		else  ////data is valid,dispose!
		{	
			//�˳�
			if(CPDataKey==0)
			{
				break;
			}
			else if(lpPerIOData == NULL)
			{
				PutDebugString("IOCP Thread ERROR (bResult == true,lpPerIOData == NULL)��");
			}
			else if(dwNumRead==0)
			{
				//���һ��ɾ��Client�Ĳ�������
				tagSocketOper *pSocketOper =
					new tagSocketOper(SOCKETOPERATYPE_DELBYSOCKETID,CPDataKey,NULL,0);
				pServer->m_SocketOperaCommands.Push_Back(pSocketOper);

				delete []lpPerIOData->pBuffer;
				free(lpPerIOData);
			}
			else if(lpPerIOData->OperationType == SOT_Receive)
			{
				CHAR* pBuf = new CHAR[dwNumRead];
				memcpy(pBuf,lpPerIOData->pBuffer,dwNumRead);
				tagSocketOper *pSocketOper =
					new tagSocketOper(SOCKETOPERATYPE_RECIEVE,CPDataKey,pBuf,dwNumRead);	
				pServer->m_SocketOperaCommands.Push_Back(pSocketOper);

				//�ٴη��Ͷ�ȡ���ݵ�����
				memset( &lpPerIOData->OverLapped,0,sizeof(OVERLAPPED) );

				lpPerIOData->DataBuf.len = lpPerIOData->nBufSize;
				lpPerIOData->DataBuf.buf = lpPerIOData->pBuffer;
				DWORD dwFlag = 0;
				DWORD dwReceivByte;
				int nRet = WSARecv(lpPerIOData->hSocket,&(lpPerIOData->DataBuf),1,&dwReceivByte,
					&dwFlag,&(lpPerIOData->OverLapped),NULL);
				if(nRet == SOCKET_ERROR)
				{
					int nErrorID = WSAGetLastError();
					if (nErrorID != WSA_IO_PENDING)
					{
						//���һ��ɾ��Client�Ĳ�������
						tagSocketOper *pSocketOper =
							new tagSocketOper(SOCKETOPERATYPE_DELBYSOCKETID,CPDataKey,NULL,0);
						pServer->m_SocketOperaCommands.Push_Back(pSocketOper);

						delete []lpPerIOData->pBuffer;
						free(lpPerIOData);
					}
				}

			}
			else if(lpPerIOData->OperationType == SOT_Send)
			{
				if( dwNumRead != lpPerIOData->DataBuf.len)
				{
					char str[200];
					sprintf(str, "IOCP Not All Send Data S:%d,C:%d.",lpPerIOData->DataBuf.len,dwNumRead);
					PutDebugString(str);
				}
				delete []lpPerIOData->pBuffer;
				free(lpPerIOData);

				//���һ����ɷ��͵���Ϣ
				tagSocketOper *pSocketOper =
					new tagSocketOper(SOCKETOPERATYPE_SENDEND,CPDataKey,NULL,0);
				pServer->m_SocketOperaCommands.Push_Back(pSocketOper);
			}
		}
	}
}

//*****************************************
//[������]�������̵߳��߼�����
//*****************************************
unsigned __stdcall WorkerThreadFunc(void* pArguments)
{
	CServer* pServer=(CServer*)pArguments;

#ifndef _DEBUG
	__try
	{
#endif

		DoWorkerThreadFunc(pServer);

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		PutDebugString("********IOCP WorkerThread ERROR, Cheak Last ERROR********");
		AddLogText("********IOCP WorkerThread ERROR, Cheak Last ERROR********");
	}
#endif

	_endthreadex( 0 );
	return 0;
}

unsigned __stdcall AcceptThreadFunc(void* pArguments)
{	
	CServer* pServer=(CServer*)pArguments;

	if(pServer==NULL)
	{
		_endthreadex( 0 );
		return 0;
	}

#ifndef _DEBUG
	__try
	{
#endif
		//����client socket
		while(true)
		{	
			//if(pServer->m_bAcceptThreadExit)
			//	break;
			pServer->OnAccept(0);

			//������������״������������
			//Sleep(pServer->m_dwAcceptThreadSleepTime);
		}

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		PutDebugString("********Server AcceptThread ERROR, Cheak Last ERROR********");
		AddLogText("********Server AcceptThread ERROR, Cheak Last ERROR********");
	}
#endif

	_endthreadex( 0 );
	return 0;
}

void DoNetThreadFunc(CServer* pServer)
{
	char strTempt[200]="";
	CommandsQueue TemptCommands;
	//��ȡһ����������
	while(true)
	{

		Sleep(pServer->m_dwNetMainThreadSleepTime);

		if(pServer->m_bNetMainThreadExit && pServer->m_SocketOperaCommands.GetSize() <=0 )
			break;

		TemptCommands.clear();
		pServer->m_SocketOperaCommands.CopyAllCommand(TemptCommands);

		CommandsQueue::iterator itCommand = TemptCommands.begin();
		for(; itCommand != TemptCommands.end();itCommand++)
		{
			tagSocketOper* pSocketOper = (*itCommand);

			if(pSocketOper)
			{
				switch(pSocketOper->OperaType)
				{
					//���һ���ͻ����׽���
				case SOCKETOPERATYPE_ADD:
					{
						CServerClient* pClient = (CServerClient*)(pSocketOper->pBuf);
						pServer->AddAClient(pSocketOper->lSocketID,pClient);

						pServer->AddNewAcceptSocket(pSocketOper->lSocketID);

						// �� client socket�����ɶ˿�
						HANDLE hRet = CreateIoCompletionPort((HANDLE)(pClient->m_hSocket),pServer->m_hCompletionPort,
							(ULONG_PTR)pSocketOper->lSocketID,0);

						PER_IO_OPERATION_DATA* pPerIOData = (PER_IO_OPERATION_DATA*)calloc(1,sizeof(PER_IO_OPERATION_DATA));
						//PER_IO_OPERATION_DATA* pPerIOData = new PER_IO_OPERATION_DATA;
						//memset(pPerIOData,0,sizeof(PER_IO_OPERATION_DATA));

						pPerIOData->pBuffer = new CHAR[8192];
						pPerIOData->nBufSize = 8192;
						pPerIOData->hSocket = pClient->m_hSocket;

						//����ɶ˿��ϻ������
						if(pClient->ReadFromCompletionPort(pPerIOData)==false)
						{
							//delete pPerIOData;
							delete []pPerIOData->pBuffer;
							free(pPerIOData);
							pServer->DelOneClient(pSocketOper->lSocketID);
						}
					}
					break;
				case SOCKETOPERATYPE_DELBYSOCKETID:
					{	//ɾ��һ���ͻ���
						pServer->DelOneClient(pSocketOper->lSocketID);
					}
					break;
				case SOCKETOPERATYPE_QUITBYSOCKETID:
					{	//�����˳�һ���ͻ���
						pServer->OnQuitOneClient(pSocketOper->lSocketID);
					}
					break;
				case SOCKETOPERATYPE_QUITBYMAPID:
					{	//����ӳ��IDɾ��һ���ͻ���
						long lSocketID = pServer->GetSocketIDByMapID(pSocketOper->lSocketID);
						delete pServer->m_MapIDSocketID[pSocketOper->lSocketID];
						pServer->m_MapIDSocketID.erase(pSocketOper->lSocketID);
						pServer->OnQuitOneClient(lSocketID);
					}
					break;
				case SOCKETOPERATYPE_QUITBYMAPSTR:
					{	//����ӳ��IDɾ��һ���ͻ���
						long lSocketID = pServer->GetSocketIDByMapStr(pSocketOper->pStrID);

						pServer->m_MapStrSocketID.erase(pSocketOper->pStrID);
						pServer->OnQuitOneClient(lSocketID);

						delete []pSocketOper->pStrID;
					}
					break;
				case SOCKETOPERATYPE_QUITALL:
					{	//ɾ�����пͻ���
						pServer->OnQuitAllClient();
					}
					break;
				case SOCKETOPERATYPE_PLAYERJOIN:
					{
						//��ҽ���ʱ,����SocketID ��PlayerID
						CServerClient* pClient = pServer->FindOneClient(pSocketOper->lSocketID);
						if(pClient)
						{
							pClient->SetMapID(pSocketOper->lNum1);
							if( !pServer->m_MapIDSocketID[pSocketOper->lNum1] )
							{
								AllowedClient *node = new AllowedClient;
								pServer->m_MapIDSocketID[pSocketOper->lNum1] = node;
								pClient->GetPeerName(node->_ip, (UINT &)node->_port);
							}
							pServer->m_MapIDSocketID[pSocketOper->lNum1]->_sock = pClient->GetIndexID();
						}
						//�˿ͻ����Ѿ��Ͽ�������Ӱ�������Ϣ
						else
						{
							pServer->OnMapIDError(pSocketOper->lNum1);
						}
					}
					break;
				case SOCKETOPERATYPE_CDKEYJOIN:
					{
						//CDkey����ʱ,����SocketID ��Cdkey
						CServerClient* pClient = pServer->FindOneClient(pSocketOper->lSocketID);
						if(pClient)
						{
							pClient->SetMapStr(pSocketOper->pStrID);
							pServer->m_MapStrSocketID[pSocketOper->pStrID] = pClient->GetIndexID();
						}
						//Ӱ�����
						else
						{
							pServer->OnMapStrError(pSocketOper->pStrID);
						}
						delete []pSocketOper->pStrID;
					}
					break;
				case SOCKETOPERATYPE_RECIEVE:
					{
						CServerClient* pClient = pServer->FindOneClient(pSocketOper->lSocketID);

						//���һ���µ������ڹ涨��ʱ���ھ͵�����,����Ч
						pServer->RemoveNewAcceptSocket(pSocketOper->lSocketID);

						if(pClient && pClient->GetCloseFlag() == false)
						{
							long  lSize= (long)(pSocketOper->lNum1);
							CHAR* pBuf = (CHAR*)pSocketOper->pBuf;
							//���ӿͻ��˵���������

							if( pServer->GetCheck() )
							{
								//���ͳ�Ƶķ������ݴ�С
								pServer->AddRecvSize(lSize);
								long lCurSize = pClient->AddPackageSize(lSize);
								if( lCurSize > pServer->GetSizePerSecond() )
								{
									pClient->OnTotalMessageSizeOver(lCurSize,pServer->GetSizePerSecond());

									pServer->AddForbidIP( pClient->GetDwordIP() );
									pServer->QuitClientBySocketID(pClient->GetIndexID());
									break;
								}
							}

							pClient->AddReceiveData(pBuf,lSize);
							pClient->OnReceive();
						}

						delete []pSocketOper->pBuf;
					}
					break;
				case SOCKETOPERATYPE_SENDTOSOCKET:
					{
						CServerClient* pClient = pServer->FindOneClient(pSocketOper->lSocketID);
						if(pClient)
						{
							//��ӷ�������
							if(pClient->AddSendData(pSocketOper->pBuf,pSocketOper->lNum1)==false)
							{
								char str[200];
								sprintf(str, "Client ID(%d),IP(%s)Send Cache Size Overflaw, Forced Intermitted.",
									pClient->GetMapID(),pClient->GetIP());
								PutDebugString(str);
								pServer->DelOneClient(pSocketOper->lSocketID);
							}
						}
						delete []pSocketOper->pBuf;
					}
					break;
				case SOCKETOPERATYPE_SENDTOMAPID:
					{//����ӳ���ID
						long lSocketID = pServer->GetSocketIDByMapID(pSocketOper->lSocketID);
						if(lSocketID)
						{
							CServerClient* pClient = pServer->FindOneClient(lSocketID);
							if(pClient)
							{
								//��ӷ�������
								if(pClient->AddSendData(pSocketOper->pBuf,pSocketOper->lNum1) == false)
								{
									char str[200];
									sprintf(str, "Client ID(%d),IP(%s)Send Cache Size Overflaw, Forced Intermitted.",
										pClient->GetMapID(),pClient->GetIP());
									PutDebugString(str);
									pServer->DelOneClient(lSocketID);
								}
							}
						}
						delete []pSocketOper->pBuf;
					}
					break;
				case SOCKETOPERATYPE_SENDTOMAPSTR:
					{//����ӳ���Str
						long lSocketID = pServer->GetSocketIDByMapStr(pSocketOper->pStrID);
						if(lSocketID)
						{
							CServerClient* pClient = pServer->FindOneClient(lSocketID);
							if(pClient)
							{
								//��ӷ�������
								if(pClient->AddSendData(pSocketOper->pBuf,pSocketOper->lNum1)==false)
								{
									char str[200];
									sprintf(str, "Client ID(%d),IP(%s)Send Cache Size Overflaw, Forced Intermitted.",
										pClient->GetMapID(),pClient->GetIP());
									PutDebugString(str);
									pServer->DelOneClient(lSocketID);
								}
							}
						}
						delete []pSocketOper->pBuf;
						delete []pSocketOper->pStrID;
					}
					break;

				case SOCKETOPERATYPE_SENDALL:
					{
						SocketIDClient::iterator it = pServer->m_Clients.begin();

						for(;it != pServer->m_Clients.end();)
						{
							CServerClient* pClient = (*it).second;
							if(pClient)
							{
								if( pClient->AddSendData(pSocketOper->pBuf,pSocketOper->lNum1) == false)
								{
									char str[200];
									sprintf(str, "Client ID(%d),IP(%s)Send Cache Size Overflaw, Forced Intermitted.",
										pClient->GetMapID(),pClient->GetIP());
									PutDebugString(str);

									it= pServer->m_Clients.erase(it);
									pClient->OnClose(0);
									delete pClient;
									pServer->m_lClientSocketCount--;
								}
								else
								{
									++it;
								}
							}
							else
							{
								++it;
							}
						}
						delete []pSocketOper->pBuf;
					}
					break;
				case SOCKETOPERATYPE_SENDEND:
					{
						CServerClient* pClient = pServer->FindOneClient(pSocketOper->lSocketID);
						if(pClient)
						{
							pClient->DecIoOperatorNum();
						}
					}
					break;
				}

				delete pSocketOper;
			}
		}

		TemptCommands.clear();
		//����ɶ˿ڷ�������
		SocketIDClient::iterator it = pServer->m_Clients.begin();
		for(;it != pServer->m_Clients.end();it++)
		{
			CServerClient* pClient = (*it).second;
			//�ж�δ��ɵķ���IO�����ĸ����Ƿ񳬶�������,����ɶ˿ڷ�������
			if(pClient->GetIOOperatorNum() < pServer->GetMaxIOOperNum())
			{
				if(pClient->GetCurSendBuffSize() > 0 && pClient->Send(NULL,0)==false)
				{
					it= pServer->m_Clients.erase(it);
					pClient->OnClose(0);
					delete pClient;
					pServer->m_lClientSocketCount--;
				}
			}
			else
			{
				//char str[200];
				//sprintf(str, "����:�ͻ���ID(%d),IP(%s)����ɶ˿��ϵķ��Ͳ���������%d��.",
				//pClient->GetMapID(),pClient->GetIP(),pClient->GetIOOperatorNum());
				//PutDebugString(str);
			}
		}

		//������������socket,����Ƿ�ʱ��������
		pServer->DoNewAcceptSocket();

		//�ر�
		it = pServer->m_Clients.begin();
		for(;it != pServer->m_Clients.end();it++)
		{
			CServerClient* pClient = (*it).second;
			//�ж�δ��ɵķ���IO�����ĸ����Ƿ񳬶�������,����ɶ˿ڷ�������
			if(pClient && pClient->GetCloseFlag() == true)
			{
				pClient->Close();
			}
		}
	}
}

//�������̣߳�������Ĳ���������н�����ִ��
unsigned __stdcall NetThreadFunc(void* pArguments)
{
	CServer* pServer=(CServer*)pArguments;
	if(pServer==NULL)
	{
		_endthreadex( 0 );
		return 0;
	}

#ifndef _DEBUG
	__try
	{
#endif

		DoNetThreadFunc(pServer);

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		PutDebugString("********Server WorkerThread ERROR, Cheak Last ERROR********");
		AddLogText("********Server WorkerThread ERROR, Cheak Last ERROR********");
	}
#endif

	_endthreadex( 0 );
	return 0;
}