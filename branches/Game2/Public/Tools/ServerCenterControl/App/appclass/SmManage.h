


#include "SM.h"
#include "../Setup/SMSetup.h"

#pragma once



class SmManage
{
public:
	SmManage(VOID);
	~SmManage(VOID);

public:
	//!
	static	SmManage&		GetInstance(VOID);

public:
	//!						��ʼ������
	BOOL					Init(VOID);
	//!						�ͷŶ���
	VOID					Release(VOID);


public:
	//!						�ҵ�һ��SM
	SM*						FindSM(DWORD dwSMID);

	//!						��Ӧ������SM
	DWORD					OnConnect(LONG lSocket, LONG lIP);
	//!						��Ӧ��½SM
	VOID					OnLogin(LONG lSocket, LPCSTR pPwd, DWORD VersionNo);
	//!						��Ӧ�Ͽ�����
	VOID					OnClose(DWORD dwSMID);

	//!						����������Ϣ���ͻ���
	VOID					SendInfoToSocket(LONG lSocket);
	//!						���������Ϣ
	VOID					ShowAllInfo(VOID);

public:
	
	//!						�õ�������Ҫ���µ����°汾��GAPP��socket�������б�
	VOID					GetNeedToHighestGapp(vector<tagUpdateInfo> &vUpdateInfo);


private:
	//!						��ȡ����
	BOOL					LoadSetup(VOID);

	//!						�¼�һ��SM
	SM*						AddNewSM(DWORD dwSMID, const tagSMSetup *pSMSetup);
	//!						ɾ��һ��SM
	VOID					DelSM(DWORD dwSMID);
	

private:
	//!						�Ѿ���½�ģ��������е�SM
	map<DWORD, SM*>			m_mapRunningSM;

private:
	//!
	static	SmManage		c_SmManage;
};