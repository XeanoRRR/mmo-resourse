

#include "WSTeam.h"


#pragma once
//! ս��ϵͳ������ս���Ŷӣ�����ɸѡ��ս������



class CSentaiSystem
{
public:
	CSentaiSystem(void);
	~CSentaiSystem(void){}

	//! ��ȡ���á���ʼ��ϵͳ���������ض�����
	bool Init(const char *pSetupName);

public:
	//! ��ӦGSת���ļ����˳�����
	void OnPlayerQueue(const CGUID &PlayerGuid, bool IsJoin);

	//! ��Ӧ�ű�ϵͳ���͵ĵ��ó���������Ϣ
	void OnWarFieldFree(long FieldRegionID);

	//! ��ѯ��������Ƿ����볡�������׶�
	bool IsQueuing(const CGUID &TeamGuid);

	//! �������Ƿ�����ս����
	bool IsBattle(const CGUID &TeamGuid);

	//! ��Ӧ������������
	void OnTeamLeaveGame(const CGUID &TeamGuid);

	//! ����һ�������ս��ս��ֻ����ս���в���Ч��
	void EndTeamWar(const CGUID &TeamGuid, long FieldRegionID);

public:
	//! ��ʱ����ʱ��Ӧ
	long OnTimeOut(DWORD timerid, DWORD curtime, const void* var);
	//! ����ʱ����ȡ���ĵ���
	void OnTimerCancel(DWORD timerid, const void* var){}
	//! ���ж�ʱ��
	void TimerRun(DWORD dwCurrTime){m_TimerQueue.Expire(dwCurrTime);}




	/////////////////////////////////////////////////////////////////////////////
private:

	//! ����
	struct tagField 
	{
		bool			IsFree;
		long			RegionID;
		vector<POINT>	BeginPos;
		vector<CGUID>	TeamGuids;
	};

	//! ��������
	struct tagSSSetup 
	{
		DWORD 			FieldTeamNum;		//! �������ɵĶ�����
		DWORD			TeamMemberNum;		//! �����������Ա�����ƣ����ܶ�Ҳ�����٣�
		DWORD			TeamMemberMinLvl;	//! �����Ա��С�ȼ�
		DWORD 			IntoWaitSec;		//! ��ʼ�ȴ���ʱ��
		DWORD 			PenaltyPoints;		//! �������ͷ�����
		float 			ReadyTimeFactor;	//! ����׼����ʱ��ϵ��
	};

	//! ս��
	struct tagEntryTeam 
	{
		long			SentaiPoint;
		CGUID			TeamGuid;	
	};

	typedef multimap<long, tagEntryTeam>	TeamQueue;
	typedef TeamQueue::iterator				ItrTeamQueue;
	typedef vector<tagEntryTeam>			TeamArray;
	typedef TeamArray::iterator				ItrTeamArray;

	typedef CTimerQueue<CSentaiSystem*>			SentaiTimer;

	//! ��ֻ������ɵ�ս��
	struct tagWarGroup
	{
		DWORD			TimerID;
		long			PreparedRegionID;
		TeamArray		Teams;				//! ���в��������GUID
	};

private:
	

	//! �ҵ�һ�������ŶӵĶ���
	ItrTeamQueue FindQueueTeam(const CGUID &TeamGuid);
	ItrTeamQueue FindQueueTeam(long TeamPoint, const CGUID &TeamGuid);

	//! ��һ��������뵽�Ŷ�
	void AddToQueue(long TeamPoint, const CGUID &TeamGuid, bool ReplaceWait);
	void AddToQueue(tagEntryTeam EntryTeam, bool ReplaceWait);
	//! ɾ���Ŷ�
	ItrTeamQueue DelFromQueue(ItrTeamQueue itrTeam, bool IsInNext);

	//! �ҵ�һ���Ѿ���ó��صĶ���
	ItrTeamArray FindWaitTeam(const CGUID &TeamGuid);
	//! ɾ��һ���Ѿ���ó��صĶ���
	void DelFromWait(ItrTeamArray iteWaitTeam);

	//! �������µĶ���ȥ�滻�ȴ��еĶ���
	bool TestAndSwapWait(tagEntryTeam &NewTeam);

	//! ����Ƿ�������Ž���ȴ��׶Σ�����ʼ����ʱ
	void TestGotoWait(void);

	//! �������Ƿ��ڵȴ����ؽ׶�
	bool IsWaitField(const CGUID &TeamGuid);
	//! �������Ƿ����ڵ���ʱ�׶�
	bool IsEntering(const CGUID &TeamGuid);

	

	//! �Ż��ȴ����Ŷ�
	void OptimizeWait(void);

	//! �ĵ�һ���յĳ��أ�������
	long GetAndLockFreeField(void);

	//! ��ʼ���뵹��ʱ
	void BeginIn(void);

	//! ����
	void InField(DWORD TimerID);

	//! ���ö�������˳�ս��ϵͳ
	void OnTeamInSys(const CGUID &TeamGuid, bool IsIn, bool IsChangeRgn);

	//! ����һ�������ս��ս��
	bool EndTeamWar(const CGUID &TeamGuid, tagField &Field, bool SendMsg);


private:

	//!					��������
	tagSSSetup			m_Setup;
	//!					�����б�
	map<long, tagField>	m_WarFields;
	//!					���г�������
	long				m_FreeNum;

	//!					�����ŶӵĶ���
	TeamQueue			m_TeamQueue;
	//!					�ŶӶ����Ӧ�Ļ���
	map<CGUID, long>	m_TeamPoint;

	//!					���Ŷӵ�ͷ����βȡ����������
	bool				m_GetInHead;
	//!					��ó��أ��ȴ����������
	tagWarGroup			m_WaitTeam;
	//!					׼�����볡�ص������б�
	list<tagWarGroup>	m_ReadyTeams;

	//!
	SentaiTimer			m_TimerQueue;


};