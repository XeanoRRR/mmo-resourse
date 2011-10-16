/**
2009/07/08
�ſ���

�����������ࣺ
	-����ָ����IP���ж��һ���ԵĶ�����
	-��Ӧ�����ӷ��͹������ַ�������
*/
#pragma once
#include <winsock2.h>


struct tagGsInfo
{
	bool bGsWorking;
	LONG lLSID;
	LONG lWSID;
	LONG lGSID;
	LONG lGSPlayerNum;

	tagGsInfo(VOID){memset(this, 0, sizeof(tagGsInfo));}
};

class COnCmdConnect
{
public:
	COnCmdConnect(VOID);
	~COnCmdConnect(VOID);
public:
	//!					��ʼ��
	BOOL				Init(VOID);
	//!					�ͷŶ���
	VOID				Release(VOID);
	//!					��ʼ����
	static	LPVOID		Watch(LPVOID pThis);
	//!					��Ӧ����
	VOID				OnConnect(SOCKET sClient, sockaddr_in &client);

	//!
	VOID				ClearGsInfo(){m_Lock.Lock();m_vGsInfo.clear();m_Lock.UnLock();}
	//!
	VOID				AddGsInfo(tagGsInfo &GsInfo){m_Lock.Lock();m_vGsInfo.push_back(GsInfo);m_Lock.UnLock();}
private:
	//!					���ڱ�ʶLS���ⲿ���
	string				m_strNumber;
	//!					���ӵĶ˿�
	u_short				m_usPort;
	//!					���������
	set<DWORD>			m_setIP;

	bool				m_bWorking;
	SOCKET				m_sListen;

	
	vector<tagGsInfo>	m_vGsInfo;
	LockOfWin32			m_Lock;
};