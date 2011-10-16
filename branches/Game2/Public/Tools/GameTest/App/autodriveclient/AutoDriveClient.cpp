#include "stdafx.h"
#include "AutoDriveClient.h"
//#include "../../../nets/netGT/message.h"
#include "..\..\Net\Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//��̬��ʵ��
CMemServer *CDriveClient::m_pMemServerManager = NULL;

CDriveClient::CDriveClient() : m_uID(0)
{	
	//m_pMemServerManager = new CMemServer; 
	GetMemServerInstance();
	//����CMessage��MappingFile������
	CMessage::SetMemServerManager(GetMemServerInstance());
}

CDriveClient::~CDriveClient()
{
	_ReleaseGameClientManager();

	//�ͷ�MapFile��Դ
	//_ReleaseFileMapping();			//ת�Ƶ�Dlg�ͷ��ڴ��֮ǰ�ͷ�

	//�ͷ�MapFile����ָ�����
	SAFE_DELETE(m_pMemServerManager);
}

//����һ��GameClient
CGameClient *CDriveClient::_CreateGameClient(string cdkey, unsigned uID, Timer *timer)
{	
	//�����GameClient���Ѿ�����
	map<string, CGameClient*>::iterator it = m_mapGameClientManager.begin();
	for ( ; it != m_mapGameClientManager.end(); ++it)
	{	
		if ( cdkey.compare(it->first) == 0)
		{
			return NULL;
		}
	}		
	//���ݴ����Timer���жϵ�����Ӧ�Ĺ��죬��������Ϸ�ͻ��˹�������
	m_uID = uID;
	CGameClient *gameclient = NULL;
	if ( timer == NULL )
	{	
		gameclient = new CDriveGameClient(uID);	
	}
	else
	{	
		gameclient = new CDriveGameClient(timer, uID);	
	}			
	m_mapGameClientManager.insert(make_pair(cdkey, gameclient));
	return gameclient;


}

//ɾ��ָ���ͻ���
void CDriveClient::_DeleteGameClient(string cdkey)
{
	map<string, CGameClient*>::iterator it = m_mapGameClientManager.begin();
	for ( ; it!= m_mapGameClientManager.end(); ++it)
	{
		if ( cdkey == it->first)
		{	
			if (it->second != NULL)
			{
				SAFE_DELETE((*it).second);
			}

			m_mapGameClientManager.erase(it);
		}
	}
}

//����ָ���ͻ���
CGameClient *CDriveClient::_FindGameClient(string cdkey)
{
	map<string, CGameClient*>::iterator it = m_mapGameClientManager.find(cdkey);
	if ( it!= m_mapGameClientManager.end() )
	{
		return ((*it).second);
	}

	return NULL;
}

//���һ���ͻ���
void CDriveClient::_AddGameClient(string cdkey, CGameClient *gameclient)
{	
	if ( gameclient == NULL )
	{
		return ;
	}

	map<string, CGameClient*>::iterator it = m_mapGameClientManager.find(cdkey);	
	if ( it == m_mapGameClientManager.end() )
	{
		m_mapGameClientManager.insert(make_pair(cdkey, gameclient));
	}
}

//��տͻ��˹�����
void CDriveClient::_ReleaseGameClientManager()
{
	if ( !m_mapGameClientManager.empty() )
	{
		map<string, CGameClient*>::iterator it = m_mapGameClientManager.begin();
		for ( ; it!= m_mapGameClientManager.end(); ++it)
		{
			if ((*it).second != NULL)
			{
				SAFE_DELETE((*it).second);
			}
		}
		m_mapGameClientManager.clear();
	}
}

//Create or Get CMemServerInstance
CMemServer	*CDriveClient::GetMemServerInstance()
{
	if ( m_pMemServerManager == NULL )
	{
		m_pMemServerManager = new CMemServer; 
	}
	return m_pMemServerManager;
}

//Create FileMapping
unsigned CDriveClient::_CreateFileMapping(const char *pFileobj, const char *pFileName, const char *pFileDir)
{	
	if ( pFileobj && pFileName && pFileDir )
	{	
		/*return m_uID = GetMemServerInstance()->AddServer(pFileobj, pFileName, pFileDir);*/
		//��������MemSrv�����޸�
		m_uID = 111111;
		//GetMemServerInstance()->AddServer(m_uID, pFileobj, pFileName, pFileDir);
		return m_uID;
	}

	return 0;	
}

//init fileMapping
void CDriveClient::_InitialFileMapping(CDataBlockAllocator *pDBAllocator)
{	
	if (pDBAllocator != NULL )
	{	
		if ( !GetMemServerInstance()->Initial(pDBAllocator) )
		{
			::MessageBox(NULL,"��ʼ��CMemServer�ڴ��ʧ��!", "DriveClient::MartySa", MB_YESNO);
		}
	}
}

//Write FileMapping
void CDriveClient::_WriteFileMapping(unsigned uID, CBaseMessage *pMsg)
{	
	if ( uID>0 && pMsg )
	{
		//GetMemServerInstance()->Send(uID, pMsg);
	}
}

//Release fileMaping resource
void CDriveClient::_ReleaseFileMapping()
{	
	if ( m_pMemServerManager )
	{	
		m_pMemServerManager->Release();
		SAFE_DELETE(m_pMemServerManager);
	}
}
