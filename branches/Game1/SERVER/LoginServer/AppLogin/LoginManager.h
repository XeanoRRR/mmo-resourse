

/*
*	file:		LoginManager.h
*	Brief:		��ҵ�½���Ŷӹ���
*	detail:		
*				
*				
*				
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008
*/




/*
--------��½����--------
MSG_C2L_LOG_QUESTCDKEY				//�ͻ�CDKEY��֤
MSG_C2L_LOG_QUEST_PLAYERBASELIST	//�ͻ������Լ��Ļ�����Ϣ
							--->�����Ŷ�
MSG_W2L_LOG_ANSWER_PLAYERBASELIST	//worldserver����һ���˺ŵ���һ�����Ϣ�б�
MSG_C2L_LOG_QUEST_PLAYERDATA		//�ͻ������Լ�����ϸ��Ϣ -> ������Ϸ
MSG_W2L_LOG_ANSWER_PLAYERDATA		//worldserver���ؿͻ�����ϸ��Ϣ�Ƿ�ɹ�ȡ��

--------С�˵�½����--------
MSG_W2L_LOG_PLAYERQUIT				//worldserver�����˳���ҵ�cdkey
MSG_C2L_LOG_QUESTCDKEY				//�ͻ�CDKEY��֤
MSG_W2L_LOG_ANSWER_PLAYERBASELIST	//worldserver����һ���˺ŵ���һ�����Ϣ�б�
MSG_C2L_LOG_QUEST_PLAYERDATA		//�ͻ������Լ�����ϸ��Ϣ -> ������Ϸ
MSG_W2L_LOG_ANSWER_PLAYERDATA		//worldserver���ؿͻ�����ϸ��Ϣ�Ƿ�ɹ�ȡ��
*/
#include "../public/AccountServerDef.h"
#include "../public/ConvenientQueue.h"
#include "..\public\BaseDef.h"

#pragma once

#define USE_GAME_WS_PLAYER_NUM
//#define _OUT_LOGIN_FLOW_INFO_




class LoginManager
{
public:
	LoginManager(VOID);
	~LoginManager(VOID);

private:
	//!														�ȴ�����WS���Ŷ�
	typedef ConvenientQueue<string, tagAccountInfo_ToAS>	WaitInWsQueue;

public:
	struct tagLimitIpSetup
	{
		//!			�Ƿ�����IP��½����
		BOOL		m_bIpLogin;
		//!			һ��IPͬһ���˺�����½����
		DWORD		m_MaxIpLoginOncCdKeyCount;
		//!			һ��IP��½��ͬ�˺������
		DWORD		m_MaxIpLoginCdKeyCount;
		//!			�����½IP��¼ʱ�䣨���룩
		DWORD		m_ClearIpLoginTime;
		//!			�Ƿ�����IP������ɫ����
		BOOL		m_bIpCreate;
		//!			ͬIP��󴴽���ɫ����
		DWORD		m_MaxIpCreateCount;
		//!			���������ɫIP��¼ʱ�䣨���룩
		DWORD		m_ClearIpCreateTime;
		//!			����IPʱ�䣨���룩
		DWORD		m_FrozenIpTime;

		tagLimitIpSetup(VOID){memset(this, 0, sizeof(tagLimitIpSetup));}
	};

public:
	//!						�ı�����
	BOOL					ChangeSetup(DWORD dwMaxWsMum, DWORD dwProcessRankInterval, DWORD dwSendQueueMsgInterval, DWORD dwOneProcessNum);
	//!						�ı�IP��������
	BOOL					ChangeIpSetup(tagLimitIpSetup &LimitIpSetup);
	//!						�ض����Ŷ��˺�
	BOOL					ReLoadNoQueue(VOID);
	
	//!						���һ��WS�Ŷ�
	BOOL					AddWsWaitQueue(LONG lWsID);
	//!						ɾ��һ��WS�Ŷ�
	BOOL					DelWsWaitQueue(LONG lWsID);
	//!						�õ�һ��WS�Ľ������
	WaitInWsQueue*			GetWaitInWsQueue(LONG lWsID);
	//!						�����Ŷӵȴ������
	VOID					ProcessQueue(VOID);

	//!						���һ��������֤���˺ţ�����PT��֤��
	VOID					AddValidating(tagAccountInfo_ToAS &AccountInfo_ToAS);

	//!						�����½������Ϣ
	VOID					OutLoginInfo(VOID);

	//!						���͵ȴ���Ϣ�����
	static	VOID			SendWaitMsg(tagAccountInfo_ToAS &AccountInfo_ToAS, LPVOID pParam);
	//!						������Ӧ��½��Ϣ�����
	static	VOID			SendAnswerCdkeyMsg(tagAccountInfo_ToAS &AccountInfo_ToAS, LPVOID pParam);
public:

	//! �ͻ�����Ϣ����
	///////////////////////////////////////////////////////
	//!						��Ӧ�ͻ��˵�½����
	VOID					OnClientLoginRequest(tagAccountInfo_ToAS &AccountInfo_ToAS);
	//!						��Ӧ�ͻ���С�˺�ĵ�½����
	VOID					OnClientAgainLoginRequest(tagAccountInfo_ToAS &AccountInfo_ToAS, DWORD dwServerKey[]);
	//!						��Ӧ�ͻ������밲ȫ��Ϣ
	VOID					OnClientSecurityInput(const char *pCdKey, const char arrCard[], LONG lSocketID);
	//!						��Ӧ�ͻ��������ɫ������Ϣ�б�
	VOID					OnClientQuestPlayerBaseList(const char *pCdKey, const char *pWsName);
	//!						��Ӧ�ͻ���������ϸ����
	VOID					OnClientQuestPlayerData(LONG lWsID, const char *pCdKey, CGUID& PlayerID, DWORD dwSignCode);

	//!						��Ӧ�ͻ��˶Ͽ�����
	VOID					OnClientNetClose(const char *pCdKey, BOOL bInWs);


	//! AS��Ϣ����
	///////////////////////////////////////////////////////
	//!						��ӦAS��֤�����uValidateType��0=AS��֤�����1=�ֲ���֤��� 2=ʢ����֤�����
	VOID					OnValidateReturn(tagAccountInfo_FromAS &AccountInfo_FromAS, ULONG uValidateType);
	//!						��Ӧ��ȫ��Ϣ֪ͨ
	VOID					OnSecurityData(tagAccountInfo_FromAS &AccountInfo_FromAS, tagSecurityData &SecurityData, BOOL bIsLocalValidate = FALSE);

	//!						���һ���������˺ŵļ�ʱ
	VOID					AddFcmCdkey(const char *pCdKey, DWORD dwHoldSecond);
	//!						ɾ��һ���������˺ŵļ�ʱ
	VOID					DelFcmCdkey(const char *pCdKey);
	//!						ɾ��һ��ʱ�������еķ������˺ż�ʱ
	VOID					OnTimeOutFcmCdkey(DWORD dwCurrTime);
	//!						������м�ʱ
	VOID					ClearFcm(VOID);


	//! WS��Ϣ����
	///////////////////////////////////////////////////////
	//!						��ӦWS������ϸ���ݳɹ�
	VOID					OnWsAnswerPlayerDataSucceed(const char *pCdKey, LONG lWsID);
	//!						��ӦWS���͵�����˳���Ϸ��Ϣ
	VOID					OnWsPlayerQuit(const char *pCdKey, LONG lWsID, DWORD dwServerKey[]);


	//! �˺Ű�ȫ
	///////////////////////////////////////////////////////
	//!						�ж��Ƿ�ΪС�����
	BOOL					IsJustOutPlayer(const char *pCdKey, LONG &lWsID, DWORD dwServerKey[], LONG bDel);
	//!						�ж��Ƿ�Ϊ���Ŷӵ����
	BOOL					IsNotQueueCdKey(const char *pCdKey);

	//!						��Ӧ�û�����������󣬷����˻��Ƿ��Ѿ�����
	BOOL					OnUserPwdErr(tagAccountInfo_FromAS &AccountInfo_FromAS);
	//!						�õ��������δ��󱻶�����˺�����
	LONG					GetFrozenCdkeyNum(VOID){return (LONG)m_mapFrozenCdkey.size();}


	//! ��������ȫ
	////////////////////////////////////////////////////////
	//!						��Ӧһ��IP��½�˺ţ�����ֵ��0=ִ��ʧ�� 1=�����½ LOGIN_IP_LOGIN_MAX_SAME_CDKEY, LOGIN_IP_LOGIN_NIMIETY_CDKEY,LOGIN_IP_CREATE_MAX��
	DWORD					OnIpLogin(LONG lIP, LPCTSTR pCdKey);
	//!						��Ӧһ��IP������ɫ������ֵ��0=ִ��ʧ�� 1=�����½ LOGIN_IP_LOGIN_MAX_SAME_CDKEY, LOGIN_IP_LOGIN_NIMIETY_CDKEY,LOGIN_IP_CREATE_MAX��
	DWORD					OnIpCreate(LONG lIP);

	//!						����ֵ��0=ִ��ʧ�� 1=�����½ LOGIN_IP_LOGIN_MAX_SAME_CDKEY, LOGIN_IP_LOGIN_NIMIETY_CDKEY,LOGIN_IP_CREATE_MAX��
	DWORD					IsFrozenIp(LONG lIP, LPCTSTR pCdKey, DWORD dwCurrTime);


public:
	//!						��Ӧ��֤ʧ��
	VOID					OnValidateReturnLost(tagAccountInfo_FromAS &AccountInfo_FromAS);


private:
	//!						���Ͱ�ȫ��֤��Ϣ���ͻ���
	VOID					SendSecurityToC(tagLSSecurityData& LSSecurityData);

private:
	//!						WS�������
	DWORD					m_dwMaxWsMum;
	//!						�����ŶӼ�������룩
	DWORD					m_dwProcessRankInterval;
	//!						�����Ŷ���Ϣ��������룩
	DWORD					m_dwSendQueueMsgInterval;
	//!						ÿ�δ���������ÿ��WS��
	DWORD					m_dwOneProcessNum;

	//!						�ϴδ����ʱ��
	DWORD					m_dwLastProcessTime;
	//!						�ϴη����Ŷ���Ϣ��ʱ��
	DWORD					m_dwLastSendQueueMsgTime;

	tagLimitIpSetup			m_LimitIpSetup;
	

	//!													��¼�����������������ʱ��<�˺�, <��һ�δ����ʱ��, ����Ĵ���>>
	map<string, pair<DWORD, DWORD>>						m_mapPwdErrLog;
	//!													�����½�˺��б�
	map<string, DWORD>									m_mapFrozenCdkey;

	//!													������֤���˺�<�˺ţ� ��֤��ʱʱ��>
	ConvenientQueue<string, tagAccountInfo_ToAS>		m_cqueueValidatingCdKey;
	//!													�˺ŵİ�ȫ��֤��Ϣ
	map<string, tagLSSecurityData>						m_mapLSSecurityData;
	//!													ͨ����֤���˺�
	ConvenientQueue<string, tagAccountInfo_FromAS>		m_cqueuePassedAccount;
	//!													����WS���Ŷ��б�
	map<LONG, WaitInWsQueue>							m_mapAllWaitInWsQueue;
	
	
	//!													һ�����ڴ�WS�˳����˺ţ�����С����֤��
	struct tagWSIDAndPwd
	{
		DWORD _dwEndTime;
		LONG _lWSID;
		DWORD _dwServerKey[SERVER_KEY_DWORD_SIZE];
	};
	ConvenientQueue<string, tagWSIDAndPwd>				m_cqueueJustOutCdKey_FromWS;

	//!													���Ŷӵ����
	set<string>											m_setNotQueueCdKey;
	//!													���Խ�����Ϸ�����
	typedef ConvenientQueue<string, tagAccountInfo_ToAS> QueueWsCanInGamePlayer;
	map<LONG, QueueWsCanInGamePlayer>					m_mapCanInGamePlayer;

	//!													Ϊ�������˺ŵļ�ʱ��¼
	multimap<DWORD, string>								m_multimapFcmTimeCdkey;
	//!													��Ӧ��m_multimapFcmTimeCdkey��map
	map<string, DWORD>									m_mapCdkeyToFcmTime;

	//!													ͬIP��½ÿ���˺Ŵ�����<IP,<�˺�, ����>>
	map<LONG, map<string, LONG>>						m_cqueueIpLoginCount;
	//!													��һ�������¼��ʱ��
	DWORD												m_dwClearIpLoginTime;

	//!													ͬIP��½������ʱ<IP, ����>
	map<LONG, LONG>										m_cqueueIpCreateCount;
	//!													��һ�������¼��ʱ��
	DWORD												m_dwClearIpCreateTime;	

#ifndef USE_GAME_WS_PLAYER_NUM
	//!													�Ѿ�������Ϸ�����set<WsName, set<CdKey>>
	map<LONG, set<string>>							m_mapInGamePlayer;
#endif
	
};