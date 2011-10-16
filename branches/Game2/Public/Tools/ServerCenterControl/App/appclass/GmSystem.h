
#include "SmManage.h"
#include "UserManage.h"
#include "ReportDbManage.h"

#pragma once


class GmSystem
{
public:
	//!
	static GmSystem& GetInstance(VOID) {static GmSystem GmSystmereturn; return GmSystmereturn;}

public:
	//! ����SM�����ľٱ�
	VOID OnReport(tagReportInfo &ReportInfo);
	//! ���ӿ�ʼ/����
	VOID OnWatch(DWORD SMID, DWORD GappID, BOOL IsBegin);
	//! ��ӦGM������Ϣ����
	VOID OnCommandToGm(DWORD WsSmid, DWORD GappID, DWORD GmCommand, const char *pChatTxt);

public:
	//! GM�ӹ�һ��WS
	VOID GmInWs(DWORD WsSmid, DWORD GappID, const OnlineUser *pUser);
	//! GM�˳�һ��WS
	VOID GmOutWs(LONG UserID);


	//! ���½ӿڣ���Ҫ�ڽӹ�WS����ܵ���
public:
	//! ��ӦSUC�����ľٱ��鿴����
	VOID OnViewReport(LONG UserID);
	//! ��ӦSUC�����Ĵ������Ĳ鿴����
	//! �ɲ鿴ָ��ʱ�俪ʼ��һ���ڵĴ�����
	VOID OnViewResult(LONG UserID, DWORD BeginDate);
	//! ��ӦSUC���˺ż�������
	VOID OnScoutPlayer(const char *PlayerName, BOOL IsWatch, LONG UserID);
	//! ��ӦSUC���˺Ŵ�������
	VOID OnOptPlayer(tagReportResultInfo &ReportResultInfo, LONG UserID);
	//! ��ӦSUC�����ķ��͹�������
	VOID OnSendAffiche(const char *pAffiche, LONG UserID, DWORD SMID, DWORD GappID);
	//! ��ӦGM�����
	VOID OnCommandToPlayer(DWORD GmCommand, const char *pChatTxt, LONG UserID);

	//! ɾ��һ��GM�Ĺ�����Ϣ
	VOID DelListen(LONG UserID);

public:
	//!
	ReportDbManage& GetReportDbManage(VOID) {return m_ReportDbManage;}
	//!
	VOID EndScoutPlayer(string *pStrLog, const char *PlayerName, LONG UserID);
	//! ����һ��SMID+������ֹ���������
	VOID CreatePlayerName(string &outName, const char *PlayerName, DWORD SMID);

public:
	union tagKey
	{
		__int64 _64Key;
		struct  
		{
			DWORD _SMID;
			DWORD _GappID;
		};

		tagKey(VOID):_64Key(0){}
		tagKey(__int64 Key64):_64Key(Key64){}
		tagKey(DWORD SMID, DWORD GappID):_SMID(SMID),_GappID(GappID){}
	};

private:
	struct tagListenInfo
	{
		tagKey _ServerID;
		LONG _SmSocket;
		const OnlineUser *_pUser;
		BOOL _IsWatching;
		string _WatchPlayerName;
		//! ���һ�β�����ʱ�䣬������ʱ����磬�������û�����������£���ǰ�û������
		DWORD LastOptTime;
		//! ��һ�ζ�ȡ�ٱ���Ϣ��ʱ�䣨ms��
		DWORD LastLoadReportTime;
		//! ��һ�ζ�ȡ��������ʱ�䣨ms��
		DWORD LastLoadResultTime;
	};

	struct tagWatchInfo
	{
		LONG _UserID;
		string _strInfo;
	};

private:
	
	//! �ҵ�GM�����һ��WS
	tagListenInfo* FindGmWs(LONG UserID, eGMTPurviewType TestPurview=ePT_Penniless);
	//! ��ʽ��һ�������־�ַ���
	static VOID FormatLogToString(string &strLog, DWORD GmCommand, const char *pChatTxt, BOOL GmToS);


private:
	//! ���ݿ�������
	ReportDbManage				m_ReportDbManage;
	//! �ӹ���Ϣ map<UserID, SMID>
	map<LONG, tagListenInfo>	m_UserToSm;
	map<__int64, LONG>			m_SmToUser;
	//! �����ӵ�player
	map<string, tagWatchInfo>	m_PlayerToUser;
};