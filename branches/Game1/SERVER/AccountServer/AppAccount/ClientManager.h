

/*
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-15
*/

#include "..\nets\netAccount\message.h"

#pragma once

//#define APEX_TEST


//! ǰ������
///////////////////////////////////////////////////////////////////////////////////////////////////
const ULONG	CLIENT_NAME_SIZE = 256;

//!	�ͻ�����Ϣ�ṹ
struct tagClientInfo
{
	//!		ָ��IP
	DWORD	_dwIP;
	//!		ָ�����
	DWORD	_dwNo;
	//!		�ͻ�������
	DWORD	_dwType;
	//!		����
	CHAR	_szName[CLIENT_NAME_SIZE];

	//!		��½״̬
	DWORD	_dwLoginState;
	//!		��ǰsocket
	DWORD	_dwCurrSocket;

	BOOL	_bAbleWork;

	//////
	tagClientInfo(VOID){memset(this, 0, sizeof(tagClientInfo));}
};



//! �ͻ��˹�����
//!		�����֤����½������Ϣ����
///////////////////////////////////////////////////////////////////////////////////////////////////

class ClientManager
{
public:
	ClientManager(VOID);
	~ClientManager(VOID);


public:
	//!						��ʼ������
	BOOL					Init(const char *pFilename);
	//!						�ض��ͻ�������
	VOID					ReLoadClientSetup(const char *pFilename);
	//!						�ͷŶ���
	VOID					Release(VOID);


public:
	//!						��֤һ���ͻ����Ƿ�Ϸ�
	//!							�ɹ�ʱ��dwRetrunInfo���ؿͻ�������
	//!							ʧ��ʱ��dwRetrunInfo���ص�½��������
	BOOL					OnClientLogin(DWORD dwIP, DWORD dwNo, DWORD dwSocket, DWORD &dwRetrunInfo);
	//!						��Ӧ�ͻ����˳�
	BOOL					OnClientLogout(DWORD dwSocket);

	//!						�ÿͻ����ܷ���в��������ؿͻ���ID
	DWORD					ClientAbleWork(LONG lSocket);

#ifdef ENABLE_FCM
	//!						������Ϣ����
	VOID					OnlineInfoRevise(DBReadSet &dbReadSet, LONG lSocket);
#endif

	//!						�õ��ͻ��˵�Socket
	LONG					GetClientSocket(DWORD dwNo);
	//!						�õ��ͻ��˵�Socket
	LONG					MultiGetClientSocket(DWORD dwNo);
	//!						�õ��ͻ��˵�����
	DWORD					GetClientType(DWORD dwNo);


private:
	//!						��ȡ�ͻ�������
	BOOL					LoadClientSetup(const char *pFilename);
	//!						���¼�⵱ǰ���߿ͻ����Ƿ�Ϸ�
	VOID					CheckOnlineClient(VOID);


private:
	typedef		map<DWORD, tagClientInfo>				mapClientInfo;
	typedef		map<DWORD, tagClientInfo>::iterator		iteClientInfo;

	typedef		map<DWORD, DWORD>						mapOnlineClient;
	typedef		map<DWORD, DWORD>::iterator				iteOnlineClient;

	//!						�ͻ�����Ϣ�б�key��ClienteNo��
	mapClientInfo			m_mapClientInfo;
	//!						�����̵߳Ŀͻ�����Ϣ�б�key��ClienteNo��
	mapClientInfo			m_mapMultiClientInfo;
	//!						m_mapClientInfo����
	AsyncLock				m_LockForClientInfo;

	//!						���߿ͻ����б�key��Socket��value��ClienteNo��
	mapOnlineClient			m_mapOnlineClient;

	
	

};








