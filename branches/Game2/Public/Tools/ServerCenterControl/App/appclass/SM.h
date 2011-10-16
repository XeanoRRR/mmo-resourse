

#include "Gapp.h"
#include "HardInfoList.h"
#include "../SccPublic.h"

#pragma once

class SM : tagSmInfo
{
public:
	static		SM*					CreateSM(DWORD dwSMID, const char *pName, const char *pExplain, DWORD dwRegionID, LPCSTR pRegionName, DWORD dwGroupID, LPCSTR pGroupName);
	static		VOID				ReleaseSM(SM **ppSM);

public:
	//!
	VOID							Release(VOID);
	//!
	DWORD							GetID(VOID){return dwID;}
	//!
	LONG							GetSocket(VOID){return m_lSocket;}
	//!
	VOID							SetSocket(LONG lSocket);

	//!								���һ��Gapp��SM
	VOID							AddGapp(tagGappBaseInfo &GappBaseInfo);
	//!
	Gapp*							FindGapp(DWORD dwGappID);

	//!								Ӳ����Ϣ�������
	HardInfoList&					GetHardInfoList(VOID){return m_HardInfoList;}

	//!								����������Ϣ���ͻ���
	VOID							SendInfoToSocket(LONG lSocket);
	//!								����������Ϣ�����пͻ���
	VOID							SendInfoToAll(VOID);

	//!								���������Ϣ
	VOID							ShowAllInfo(VOID);

public:
	//!								�õ�������Ҫ���µ����°汾��GAPP��Ϣ
	VOID							GetUpdateInfo(tagUpdateInfo &UpdateInfo);

private:
	//!								
	LONG							m_lSocket;

	//!
	vector<Gapp*>					m_vGapp;

	//!
	HardInfoList					m_HardInfoList;


private:
	SM(DWORD dwSMID, const char *pName, const char *pExplain, DWORD dwRegionID, LPCSTR pRegionName, DWORD dwGroupID, LPCSTR pGroupName);
	~SM(VOID);
	SM(SM&);
};