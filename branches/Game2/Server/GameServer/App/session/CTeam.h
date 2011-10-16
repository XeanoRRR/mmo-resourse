/*
#pragma once
#include "csession.h"

class CTeam :
	public CSession
{
public:
	enum ALLOCATION_SCHEME
	{
		AS_EXCLUSIVE,
		AS_SHARE,
		AS_FORCE_DWORD	= 0xFFFFFFFF,
	};

private:
	DWORD					m_dwTeamID;
	string					m_strPassword;
	string					m_strTeamName;
	LONG					m_lTeamLeaderID;
	ALLOCATION_SCHEME		m_asAllocationScheme;
	DWORD					m_dwLastCheckedTimeStamp;

protected:
	struct tagQueryResult
	{
		DWORD dwQueryTimeStamp;
		BOOL bQueried;
		BOOL bSucceed;

		tagQueryResult();
	};

	//##�����˲�ѯ����Ķ���.��Ҫ��World Server���������ݡ�
	static map<DWORD, tagQueryResult> s_mQuestedTeams;

	friend class CSessionFactory;

public:
	CTeam								(DWORD dwMinPlugs, DWORD dwMaxPlugs, DWORD dwLifeTime);
	CTeam								(LONG lID, DWORD dwMinPlugs, DWORD dwMaxPlugs, DWORD dwLifeTime);
	virtual				~CTeam			();

	VOID				SetTeamID		( DWORD dwID );
	DWORD				GetTeamID		();

	VOID				SetLeader		( LONG lPlayerID );
	LONG				GetLeader		();

	VOID				SetPassword		( CHAR* pPassword );
	CHAR*				GetPassword		();

	VOID				SetTeamName		( CHAR* pTeamName );
	CHAR*				GetTeamName		();

	VOID				SetAllocationScheme	( ALLOCATION_SCHEME as );
	ALLOCATION_SCHEME	GetAllocationScheme	();

	DWORD				GetTeamatesAmount();

	DWORD				GetCurrentServerTeamatesAmount();

	DWORD				GetCurrentRegionTeamatesAmount( LONG lRegionID );
	DWORD				GetCurrentRegionTeamatesAmount_Alive( LONG lRegionID );
	VOID				GetCurrentRegionTeamates(map<long,CPlayer*>& RegionTeamates,LONG lRegionID);	//�õ���������ڵ�ǰ�����ĳ�Ա
	VOID				GetCurrentRegionTeamates_Alive(map<long,CPlayer*>& RegionTeamates,LONG lRegionID);	//�õ���������ڵ�ǰ�����ĳ�Ա

	FLOAT				GetCurrentRegionTeamatesAverageLevel( LONG lRegionID );
	FLOAT				GetCurrentRegionTeamatesAverageLevel_Alive( LONG lRegionID );

	CPlayer*			FindTeamatesInCurrentRegion( LONG lRegionID );

	VOID				KickPlayer( LONG lPlayerID );

	VOID				AddQuest(WORD wQuestID,CPlayer* pPlayer,long lDistance);
	VOID				RunScript(char* strScirptName,CPlayer* pPlayer,long lDistance);

	virtual VOID		AI();

	virtual BOOL		Serialize(vector<BYTE>* pStream);

	virtual BOOL		Unserialize(BYTE* pStream, LONG& lOffset);

	//##��Ա����,�Ƴ��Ĳ���
	virtual BOOL		OnPlugInserted(LONG lPlugID);

	virtual BOOL		OnPlugEnded(LONG lPlugID);

	virtual BOOL		OnPlugAborted(LONG lPlugID);

	virtual BOOL OnPlugChangeState(LONG lPlugID, LONG lState, BYTE* pData, BOOL bSendToTrigger = FALSE);

	//##�Ự�Ŀ�ʼ,��ֹ����.
	virtual BOOL OnSessionStarted( BOOL bFlag );

	virtual BOOL OnSessionEnded( BOOL bFlag );

	virtual BOOL OnSessionAborted( BOOL bFlag );

	//##�����World Server�˻�ȡ�����Ϣ��
	static VOID QuestTeamData( DWORD dwID );

	static VOID CompleteTeamData( DWORD dwID );
};
*/