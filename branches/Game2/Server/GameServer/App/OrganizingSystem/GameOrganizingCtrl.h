#pragma once


//OrganizingCtrl.h
//Function:	//���ฺ����ư��ɵĽ�������ɢ���Լ��԰ͬ�˵Ĺ������.
			//�����Organizings�����Ĺ���
			//������ϵͳ��װ�ء��洢������
//Author:Joe
//Create Time:2004.6.11
//Note:����ȫ��ֻ��Ψһ��ʵ��
#include "GameFaction.h"
#include "GameUnion.h"

class CPlayer;
class GSTeam;

//���������������ʱ���Key
struct tagKey 
{
	long	l;
	tagTime Time;
};

//������а�ṹ
struct tagFacBillboard
{
	long	lFactionID;		//���ID
	string	strName;		//�������
	long	lNum;			//����
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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GameOrganizingCtrl
{
public:
	GameOrganizingCtrl(void);
	~GameOrganizingCtrl(void);
	//����
public:
	//!							�õ�Ψһʵ�����
	static GameOrganizingCtrl*	getInstance();									
	//!							�ͷŶ���
	static		void			Release(void);													



	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	//!							��Ӧ����ϵͳ�ĳ�ʼ����Ϣ
	void						OnInitGsFaction(CMessage *pMsg);
	

	//!							��Ӧ���ᴴ���ظ�
	void						OnCreateFactionRe(CMessage *pMsg);
	//!							��Ӧ����ɾ��
	void						OnWsDelFaction(CMessage *pMsg);
	//!							��ӦWS����ظ�
	void						OnWsInvitePlayerRe(CMessage *pMsg);
	//!							��ӦWS��ӳ�Ա֪ͨ
	void						OnWsAddMember(CMessage *pMsg);
	//!							��ӦWSְ����������
	void						OnWsSetJobName(CMessage *pMsg);
	//!							��ӦWSְ��Ȩ������
	void						OnWsSetJobPurview(CMessage *pMsg);
	//!							��Ӧ��Ա�˳�֪ͨ
	void						OnWsMemberQuit(CMessage *pMsg);
	//!							��Ӧ����༭֪ͨ
	void						OnWsEditPronounce(CMessage *pMsg);
	//!							��Ӧ�����༭֪ͨ
	void						OnWsEditLeaveword(CMessage *pMsg);
	//!							��Ӧ������Ա֪ͨ
	void						OnWsFire(CMessage *pMsg);
	//!							���ó�Ա��ְ��
	void						OnWsSetMemJobLvl(CMessage *pMsg);
	//!							���ó�Ա�ĳƺ�
	void						OnWsSetMemberTitle(CMessage *pMsg);
	//!							��Ա��ͼ֪ͨ
	void						OnWsMemberMap(CMessage *pMsg);
	//!							��Ա�ȼ�֪ͨ
	void						OnWsMemberLevel(CMessage *pMsg);
	//!							��Ա����֪ͨ
	void						OnWsMemberHide(CMessage *pMsg);
	//!							��Աְҵ֪ͨ
	void						OnWsMemberOccu(CMessage *pMsg);
	//!							�����»᳤
	void						OnWsSetMaster(CMessage *pMsg);
	//!							����״̬�仯
	void						OnWsMemOnlineChange(CMessage *pMsg);
	//!							������Ա�䶯
	void						OnWsApplicantChange(CMessage *pMsg);
	//!							�л�ȼ��ı�
	void						OnWsFacLvlChange(CMessage *pMsg);
	//!							�л�ͼ��ı�
	void						OnWsIconChange(CMessage *pMsg);
	//!							��Ӧִ�и���
	void						OnBoonRun(void);
	//--------------------------------------------------------------------------------
	//!							�ҳ�һ������
	GameFaction*				GetFactionOrganizing(const CGUID &FacGuid);
	//!							�ҳ�һ����ҵĹ���ID
	const		CGUID&			FindPlayerFacionID(const CGUID &PlayerGuid);
	//!							����һ����ҵĹ�����Ϣ
	void						UpdateFacInfo(CPlayer *pPlayer);

	//!							��Ҵ򿪼����л��������
	void						OpenApplyBoard(const CGUID &PlayerGuid);
	//!							��Ҵ��ϴ�ͼ�����
	void						OpenUpLoadIconBoard(const CGUID &PlayerGuid);
	//!							������ù�����ļ״̬
	bool						SetFactionRecruit(CPlayer *pPlayer, bool bOpen);
	//!							�õ�������ڹ���ȼ�
	LONG						GetFactionLvl(CPlayer *pPlayer);


	//--------------------------------------------------------------------------------
	//! ��ҹ����������
	enum ePlayerFacOptType
	{
		//! ��������
		ePFOT_Create,
		//! ��������
		ePFOT_Upgrade,
		//! ����������
		ePFOT_Apply,
		//! �ϴ�ͼ��
		ePFOT_UpLoadIcon,

		//! �������˲���
		ePFOT_CreateUnion,
	};
	//!							һ����ҿ�ʼ�������
	void						PlayerFacOptBegin(const CGUID &PlayerGuid, LONG lOptType);
	
private:
	//!							��֤��Ҳ���
	BOOL						TestFacOpt(const CGUID &PlayerGuid, LONG lOptType);
	//!							һ����ҽ����������
	void						PlayerFacOptEnd(const CGUID &PlayerGuid, LONG lOptType);

	//--------------------------------------------------------------------------------
public:
	//!							��Ӧ��Ҵ�������
	void						OnPlayerCreateFactionReques(CMessage *pMsg);
	//!							��Ӧ��ҽ�ɢ����
	void						OnPlayerDisbandFactionReques(CMessage *pMsg);
	//!							��Ӧ����������
	void						OnPlayerFacInviteReques(CMessage *pMsg);
	//!							��Ӧ��һش�����
	void						OnPlayerFacInviteAnswer(CMessage *pMsg);
	//!							��Ӧ����޸Ĺ���
	void						OnPlayerFacSetPlacardRequest(CMessage *pMsg);
	//!							��Ӧ����޸ı���
	void						OnPlayerFacSetLeavewordRequest(CMessage *pMsg);
	//!							��Ӧ�������
	void						OnPlayerKickReques(CMessage *pMsg);
	//!							��Ӧ���óƺ�
	void						OnPlayerFacSetMemberTitleReques(CMessage *pMsg);
	//!							��Ӧ��λ
	void						OnPlayerFacSetChairmanReques(CMessage *pMsg);
	//!							�����������
	void						OnPlayerFacCheckRequisition(CMessage *pMsg);
	//!							����������
	void						OnPlayerJoinFac(CMessage *pMsg);
	//!							��Ӧ����ϴ�ͼ������
	void						OnPlayerUpLoadIconReques(CMessage *pMsg);

	//!							��Ӧ�л������ĵ���
	void						OnPlayerUpgradeFac(CPlayer *pPlayer, DWORD dwUpgradeType);
	//!							����������������
	eFactionOptErrInfo			TestUpgradeFacCondition(CPlayer *pPlayer, LONG lGoalLvl, DWORD dwUpgradeType);
	//!							��ⴴ�����˵�����
	eFactionOptErrInfo			TestCreateUnionCondition(CPlayer *pPlayer);

private:
	
	//!							�������ᣬ�����õ�����
	eFactionOptErrInfo			UpgradeFac_ByCondition(CPlayer *pPlayer, LONG lGoalLvl, DWORD dwUpgradeType);
	//!							�����������ع�GS�ϵ������������
	void						BackRoll_UpgradeFac(CPlayer *pPlayer, LONG lGoalLvl, DWORD dwUpgradeType);
	//!							�������ᣬ�����õ�����
	eFactionOptErrInfo			CreateFac_ByCondition(CPlayer *pPlayer);
	//!							�������ˣ������õ�����
	eFactionOptErrInfo			CreateUnion_ByCondition(CPlayer *pPlayer);
public:
	//!							���ͳ�ʼ�����֪ͨ
	void						SendInitFacDataToPlayer(const CGUID &PlayerGuid);
	void						SendInitFacDataToPlayer(void);
private:
	//!							�㲥һ���л�Ĺ�������
	void						RadiateFactionPublic(GameFaction *pFaction);
	//��						����ͼ����ʱ����
	//void						UpdateIconData(void);


private:
	//!									�������������Ա��
	BOOL								m_bFacDataFull;
	//!									��ǰ����ͼ���CRC32Ч����
	//DWORD								m_dwCurrIconCRC32Code;
	////!									��ǰ����ͼ�����ݱ���
	//vector<BYTE>						m_vCurrIconData;
	//!									�ȴ���ʼ�����ݵĿͻ���
	set<CGUID>							m_setsetWaitingClient;

	//!									ȫ�����еİ�����֯
	map<CGUID, GameFaction*>			m_FactionOrganizings;
	//!									�ȴ����������
	set<CGUID>							m_mapWaitCreatePlayer;
	//!									�����������������
	map<CGUID, CGUID>					m_mapAllApplicant;


	
	struct tagFacOptSession
	{
		CGUID	PlayerGuid;
		LONG	lOptType;
		BOOL operator ==(const tagFacOptSession& right)
		{
			return (right.PlayerGuid == PlayerGuid) && (right.lOptType == lOptType);
		}
	};
	//!									��������Ự
	S2CSession<tagFacOptSession>		m_PlayerOptSession;
	

	struct tagFacInviteSession
	{
		CGUID	InviterGuid;
		CGUID	AimGuid;
		CGUID	FactionGuid;
		BOOL operator ==(const tagFacInviteSession& right)
		{
			return (right.InviterGuid == InviterGuid) && (right.AimGuid == AimGuid);
		}
	};
	//!									��������Ự
	S2CSession<tagFacInviteSession>		m_PlayerInviteSession;

	


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	GameUnion*					GetUnion(const CGUID &UnionGuid);

	//!							��Ӧ����ϵͳ�ĳ�ʼ����Ϣ
	void						OnInitGsUnion(vector<BYTE> &vData);
	//!							��Ӧ������������Ϣ
	void						OnCreateNewUnion(vector<BYTE> &vData);
	//!							��ӦWS��ɢ����
	void						OnWsDisbandUnion(const CGUID &UnionGuid);
	//!							��Ӧ�˳�����
	void						OnWsQuitUnion(const CGUID &FactionGuid, const CGUID &UnionGuid);
	//!							��Ӧ�������˳�Ա
	void						OnWsKickUnion(const CGUID &OptGuid, const CGUID &AimGuid, const CGUID &UnionGuid);
	//!							��Ӧ��λ
	void						OnWsSetChairman(const CGUID &UnionGuid, const CGUID &NewMasterFacGuid, const char *pNewMasterName);
	//!							��Ӧ����Ȩ��
	void						OnWsSetPurviewUnion(const CGUID &UnionGuid, const CGUID &AimFacGuid, DWORD dwPurview);


	//!							��Ӧ��Ҵ�����������
	void						OnPlayerCreateUnion(CPlayer *pPlayer, const char *pUnionName);
	//!							��Ӧ��ҽ�ɢ������Ϣ
	void						OnPlayerDisbandUnion(CPlayer *pPlayer);
	//!							��Ӧ��������˳�����
	void						OnPlayerQuitUnion(CPlayer *pPlayer);
	//!							��Ӧ��ҿ�������
	void						OnPlayerKick(CPlayer *pPlayer, const CGUID &AimFactionGuid);
	//!							��Ӧ��������
	void						OnPlayerSetChairman(CPlayer *pPlayer, const CGUID &AimFactionGuid);
	//!							��Ӧ�������Ȩ��
	void						OnPlayerSetPurview(CPlayer *pPlayer, const CGUID &AimFactionGuid, DWORD dwPurview);
	//!							��Ӧ��������������
	void						OnPlayerUnionInvite(CPlayer *pPlayer, const char *pAimPlayerName);

	//!							����һ�������Ƿ���ս��״̬
	BOOL						TestWarState(const CGUID &FactionGuid);
	//!							���������������
	void						SendPlayerUnionData(CPlayer *pPlayer, const CGUID &UnionGuid);


private:
	//!							���е�ͬ����֯
	map<CGUID, GameUnion*>		m_mapUnion;

	//!							�ȴ����������
	set<CGUID>					m_mapWaitCreateUnionPlayer;



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��Ӳ���begin
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	GSTeam*			GetGameTeamByID(const CGUID& TeamID);		//! �õ�ָ���Ķ���ָ��

	void			GetTeamData(CMessage *pMsg);				//! ����Ϣ�л�ȡȫ�����ж���

	GSTeam*			CreateTeam(CMessage *pMsg);					//! ����WS��Ϣ����������

	void			DisbandTeam(const CGUID& TeamID);			//! ��ɢ����

	void			DisbandAllTeam(void);						//! ��ɢ���ж���

	void			OnTeamSetupInit(CMessage *pMsg);			//! ��Ӧ�������ó�ʼ��

	void			UnpdateTeamMemberPos();						//! ����һ�ζ����еĳ�Աλ��

	long			GetTeamNum(void){return (long)m_TeamOrganizings.size();};

private:
	map<CGUID,GSTeam*>					m_TeamOrganizings;	//! ȫ���������


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


private:
	static GameOrganizingCtrl* instance;										//����Ψһʵ�����

};

//�õ���֯��������
inline GameOrganizingCtrl* GetOrganiCtrl()
{
	return GameOrganizingCtrl::getInstance();
}
