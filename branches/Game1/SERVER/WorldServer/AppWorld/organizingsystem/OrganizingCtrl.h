#pragma once
#include <set>

#include "S2CSession.h"
#include "Faction.h"
#include "Union.h"
#include "..\..\public\TimerQueue.h"
//OrganizingCtrl.h
//Function:	//���ฺ����ư��ɵĽ�������ɢ���Լ��԰ͬ�˵Ĺ������.
			//�����Organizings�����Ĺ���
			//������ϵͳ��װ�ء��洢������
//Author:Joe
//Create Time:2004.6.11
//Note:����ȫ��ֻ��Ψһ��ʵ��

class CPlayer;
class CMySocket;
class WSTeam;
class CMessage;



//���������������ʱ���Key
struct tagKey 
{
	long	l;
	tagTime Time;
};

//������а�ṹ
struct tagFacBillboard
{
	CGUID	FactionGuid;	//���ID
	string	strName;		//�������
	long	lNum;			//����
};
//������֯ϵͳ�ķ��ؽ��
enum eCrOrgResult
{
	CORT_Fail=0,		//ʧ��
	CORT_NameExist,		//��������
	CORT_Ok,			//�����ɹ�
};

//�����Ϣ�ṹ��������սʱ��
struct DeclareWarFacInfo
{
	long	lFactionID;		//���ID
	char	strName[20];	//�������
	long	lRelations;		//�ͱ���Ĺ�ϵ,0:�޹�ϵ,1:�Լ�,2:ͬ��,3:�ж�
};


//��Ļ�϶ε���ʾ��Ϣ
struct stTopInfo
{
	long	lID;			//��ʾID
	long	lTimerFlag;		//����ʱ��ʽ(1:������,2:ʱ��)
	long	lParam;			//ʱ��������
	DWORD	dwStartTime;	//��ʼ��ʱʱ��
	string	strInfo;		//��Ϣ����
};

class COrganizingCtrl
{
private:
	COrganizingCtrl(VOID);
	~COrganizingCtrl(VOID);
	//����
public:
	static	COrganizingCtrl*	getInstance();														//�õ�Ψһʵ�����
	static	VOID				releaseInstance(){SAFE_DELETE(instance);}
	bool						Initialize();														//��ʼ��
	VOID						Release();															//�ͷ���Դ
	VOID						Run(long lInterTime);												//��֯ϵͳ��Run
	VOID						ClearFaction(VOID);
	VOID						ClearUnion(VOID);

	VOID						OutOrganiInfo(VOID);

public:
	//!							��ӦGS������
	VOID						OnGsConnect(LONG lGsSocket);

	VOID 						OnPlayerEnterGame(const CGUID& PlayerGuid);							//����һ����ҽ�����Ϸ
	VOID 						OnPlayerEnterGameLaterDataSend(const CGUID& PlayerGuid);					//����һ����ҽ�����Ϸ
	VOID 						OnPlayerExitGame(CPlayer *pPlayer, bool IsChangeGS);			//����һ������뿪��Ϸ
	VOID 						OnPlayerDied(const CGUID& PlayerGuid, const CGUID& lKiller);		//�������ʱ����
	int  						OnDeleteRole(const CGUID& PlayerGuid);								//���ɾ����ɫʱ����
	VOID						OnPlayerMapChange(const CGUID &PlayerGuid, const char* pMapName);	//��Ӧ��ҵ�ͼ�ı�
	VOID						OnPlayerOccuChange(CPlayer *pPlayer, LONG lOccu);			//��Ӧ���ְҵ�ı�
	VOID						OnPlayerLevelChange(CPlayer *pPlayer, LONG lLevel);			//��Ӧ��ҵȼ��ı�
	VOID						OnPlayerHideChange(CPlayer *pPlayer, BYTE cHide);			//��Ӧ�������״̬�ı�

	VOID SendOrgaInfoToClient(const CGUID& PlayerGuid,string& strSendCon,string& strSender,long lGameServerID=-1,
		DWORD dwFontColor=MAKELONG(MAKEWORD(254,237), MAKEWORD(218,255)),
		DWORD dwBkColor=MAKELONG(MAKEWORD(0,0), MAKEWORD(0,0)) );

private:
	static	COrganizingCtrl*	instance;										//����Ψһʵ�����

	
	
	


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:

	VOID						SaveAllFaction(VOID);
	VOID						SaveAllUnion(VOID);

public:
	//!							��Ӧ��ʼ���������
	VOID						OnInitFactionEndMsg						(VOID);
	//!							��Ӧ����ɾ���Ļظ�
	VOID						OnDelFactionFinishMsg					(const CGUID &AimGuid, BOOL bIsSysOpt);
	//!							��ӦGS�Ĺ��ᴴ������
	VOID						OnGsCreateFaction						(CMessage *pMsg);
	//!							��ӦGS�Ĺ�����������
	VOID						OnGsUpgradeFaction						(CMessage *pMsg);
	//!							��ӦGS��ɾ����������
	VOID						OnDelFaction							(const CGUID &DelFacGuid, BOOL bIsSysOpt);
	//!							��Ӧ����������
	VOID						OnGsFacInvitePlayer						(CMessage *pMsg);
	//!							��Ӧ�����Ҷ���
	VOID						OnGsFacAddMember						(CMessage *pMsg);
	//!							��Ӧ���ùٽ�����
	VOID						OnGsFacSetJobName						(CMessage *pMsg);
	//!							��Ӧ���ùٽ�Ȩ��
	VOID						OnGsFacSetJobPurview					(CMessage *pMsg);
	//!							��Ӧ��Ա�˳�
	VOID						OnGsFacQuit								(CMessage *pMsg);
	//!							��Ӧ���ù���
	VOID						OnGsSetPronounce						(CMessage *pMsg);
	//!							��Ӧ���ñ���
	VOID						OnGsSetLeaveword						(CMessage *pMsg);
	//!							��Ӧ������Ա
	VOID						OnGsFire								(CMessage *pMsg);
	//!							���ó�Աְ��
	VOID						OnGsSetMemberJob						(CMessage *pMsg);
	//!							���ó�Ա�ƺ�
	VOID						OnGsSetMemberTitle						(CMessage *pMsg);
	//!							��λ
	VOID						OnGsSetNewMaster						(CMessage *pMsg);
	//!							����Ӧ����
	VOID						OnGsCheckRequisition					(CMessage *pMsg);
	//!							�������
	VOID						OnGsJoinFac								(CMessage *pMsg);
	//!							�ϴ�ͼ��
	VOID						OnGsUpLoadIcon							(CMessage *pMsg);

public:
	//!							��һ���������������
	BOOL						AddFactionOrganizing					(CFaction *pFaction);
	//!							�ҳ�һ���������
	CFaction*					GetFactionOrganizing					(const CGUID &FacGuid);	
	//!							�ҳ�һ����ҵĹ���ID
	const		CGUID&			FindPlayerFacionID						(const CGUID &PlayerGuid);

	//!							���ͳ�ʼ��GS������Ϣ
	VOID						SendFactionDataToGS						(LONG lGsSocket);
	VOID						SendFactionDataToGS						(set<LONG> &setSocket);

	//!							����������������
	eFactionOptErrInfo			TestUpgradeFacCondition					(CFaction *pFaction, DWORD dwUpgradeType);
	//!							�������ᣬ�����õ�����
	eFactionOptErrInfo			UpgradeFac_ByCondition					(CFaction *pFaction, DWORD dwUpgradeType);


public:
	typedef						CTimerQueue<COrganizingCtrl*>			OrganTimer;

public:
	//!							��Ӧ�µ�һ�쵽��
	void						OnNewDay								(DWORD dwTime);
	//!							��ʱ����ʱ��Ӧ
	long						OnTimeOut								(DWORD timerid,DWORD curtime,const void* var);
	//!							����ʱ����ȡ���ĵ���
	void						OnTimerCancel							(DWORD timerid,const void* var);
	//!							
	void						TimerRun								(DWORD dwCurrTime){m_TimerQueue.Expire(dwCurrTime);}

private:
	//!							�õ���ʱ������
	OrganTimer&					GetTimerQueue							(void){return m_TimerQueue;}
	//!
	VOID						BeginTimer								(VOID);
	//!
	VOID						EndTimer								(VOID);

private:
	//!							�������������Ա��
	BOOL						m_bFacDataFull;
	//!							�ȴ���ʼ�����ݵ�GS
	set<LONG>					m_setWaitFacSocket;

	//!							ȫ�����еİ�����֯
	map<CGUID, CFaction*>		m_FactionOrganizings;
	//!							�����������������
	map<CGUID, CGUID>			m_mapAllApplicant;
	
	//!							��ʱ��
	OrganTimer					m_TimerQueue;
	//!							������帣����TimerID
	LONG						m_lExpTimerID;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//!							��һ�����˶���������
	BOOL						AddUnionOrganizing(CUnion *pUnion);
	//!							�ҳ�һ�����˶���
	CUnion*						GetUnionOrganizing(const CGUID &UnionGuid);

	//!							���ͳ�ʼ��GS������Ϣ
	VOID						SendUnionDataToGS(LONG lGsSocket);
	VOID						SendUnionDataToGS(set<LONG> &setSocket);

	//!							��Ӧ��ʼ���������
	VOID						OnInitUnionEndMsg(VOID);
	//!							��Ӧɾ������ɹ�
	VOID						OnDelUnionFinishMsg(const CGUID &UnionGuid);

	//!							��Ӧ��Ҵ�������
	VOID						OnGsCreateUnion(CPlayer *pPlayer, const char *pUnionName);
	//!							��Ӧ��ҽ�ɢ����
	VOID						OnGsDisbandUnion(CPlayer *pPlayer);
	//!							��Ӧ�˳���������
	VOID						OnGsQuitUnion(const CGUID &FactionGuid, const CGUID &UnionGuid);
	//!							��Ӧ������������
	VOID						OnGsKickUnion(const CGUID &OptGuid, const CGUID &AimGuid, const CGUID &UnionGuid);
	//!							��λ
	VOID						OnGsSetChairmanUnion(const CGUID &MasterGuid, const CGUID &AimFacGuid);
	//!							����Ȩ��
	VOID						OnGsSetPurviewUnion(const CGUID &MasterGuid, const CGUID &AimFacGuid, DWORD dwPurview);
	//!							�����������
	VOID						OnGsUnionInvit(const CGUID &MasterGuid, const char *pAimPlayerName);
	//!							Ӧ������
	VOID						OnGsUnionAnswerInvit(const CGUID &InviterGuid, const CGUID &RespondentGuid, const CGUID & UnionGuid, LONG lResult);

private:
	//!							�������������Ա��
	BOOL						m_bUnionDataFull;
	//!							�ȴ���ʼ�����ݵ�GS
	set<LONG>					m_setWaitUnionSocket;

	//!							ȫ�����е�ͬ����֯
	map<CGUID, CUnion*>			m_mapUnion;

	struct tagUnionSession
	{
		CGUID SourceID;		//! ������
		CGUID TargetID;		//! Ӧ����
		CGUID UnionID;		//! ����ID
		tagUnionSession():SourceID(NULL_GUID),TargetID(NULL_GUID){}
		tagUnionSession(const CGUID &SrcID, const CGUID &AimID, const CGUID &UnionGuid)
			:SourceID(SrcID),TargetID(AimID),UnionID(UnionGuid){}
		BOOL operator ==(const tagUnionSession& right)
		{
			return ((right.SourceID == SourceID) && (right.TargetID == TargetID) && (right.UnionID == UnionID));
		}
	};
	S2CSession<tagUnionSession>			m_UnionSession;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//����Ự�ṹ
	struct tagPlayerTeamSession
	{
		CGUID SourceID;		//! ������
		CGUID TargetID;		//! Ӧ����
		tagPlayerTeamSession():SourceID(NULL_GUID),TargetID(NULL_GUID){}
		tagPlayerTeamSession(const CGUID &SrcID, const CGUID &AimID):SourceID(SrcID),TargetID(AimID){}
		BOOL operator ==(const tagPlayerTeamSession& right)
		{
			return ((right.SourceID == SourceID) && (right.TargetID == TargetID));
		}
	};

public:
	VOID			SendTeamSetupToGS(LONG lGsSocket);							//! ���Ͷ���������Ϣ��GS

	VOID			SendTeamDataToGS(LONG lGsSocket);							//! ����ȫ�ֶ������ݵ�GS

	VOID			SendInfoToOneClient(const CGUID& PlayerGuid, char *szInfo);	//! ��ָ����ҷ���һ����Ϣ

	WSTeam*			GetGameTeamByID(const CGUID& TeamID);						//! �õ�ָ���Ķ����ָ��

	long			GetGameTeamNum(VOID);										//! �õ���������

	//long			GetNewTeamID(VOID);											//! �õ�һ���µĶ���ID

	VOID			OnQueryJoin(CMessage *pMsg);								//! ��Ӧ��ҷ����������
	
	VOID			OnQueryJoinByName(CPlayer* pInvitee, CPlayer* pCaptain);	//! ��Ӧ��ҷ����������
	
	VOID			OnAnswerJoin(CMessage *pMsg);								//! ��Ӧ��Ҵ��������

	VOID			OnChangLeader(CMessage *pMsg);								//! ��Ӧ�ı�ӳ�

	VOID			OnKickPlayer(CMessage *pMsg);								//! ��Ӧ����

	VOID			OnSetGoods(CMessage *pMsg);									//! ��Ӧ��Ʒ��������

	VOID			OnRecruit(CMessage *pMsg);									//! ��Ӧ��ļ����

	VOID			CloseRecruit(const CGUID &PlayerGuid);						//! �ر���ҵ���ļ

	VOID			OnBeRecruited(CMessage *pMsg);								//! ��ӦӦ��

	VOID			OnChat(CMessage *pMsg);										//! ��Ӧ����

	VOID			OnTeamMemberAllIdioinfo(CMessage *pMsg);					//! ��Ӧ��Ա��ϸ��Ϣת��

	VOID			OnTeamMemberPos(CMessage *pMsg);							//! ��Ӧ��Աλ����Ϣת��

	bool			PlayerJoinTeam(const CGUID& PlayerGuid, const CGUID& TeamGuid);		//! ��ҽ���һ������

	VOID			PlayerLeaveTeam(const CGUID& PlayerGuid);					//! ����뿪����

	const CGUID&	FindPlayerTeamID(const CGUID& PlayerGuid);					//! �õ�һ����ҵĶ���ID

	WSTeam*	CreateTeam(const CGUID& TeamGuid, const CGUID& MastterGuid, const CGUID& MemberGuid);	//! �������飬����������б�


	VOID			DisbandTeam(WSTeam *pTeam);									//! ��ɢ����
	

private:
	map<CGUID, WSTeam*>				m_TeamOrganizings;							//! ȫ���������

	map<CGUID, CGUID>				m_MemberIDvsTeamID;							//! ȫ�����г�ԱID��Ӧ�Ķ���ID

	map<CGUID, CPlayer*>			m_RecruitingPlayer;							//! ������ļ�����

	//map<CGUID, long>				m_ChangeGSPlayerIDAndOuttime;				//! ����ת���������Ķ�����Ҷ�Ӧ�ĳ�ʱʱ��
	S2CSession<tagPlayerTeamSession>	m_InviteSession;						//! �������Ự
	

};

//�õ���֯��������
inline COrganizingCtrl* GetOrganiCtrl()
{
	return COrganizingCtrl::getInstance();
}
