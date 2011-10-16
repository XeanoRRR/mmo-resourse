//GSTeam:		�̳���COrganizing��������Ҷ������ݣ�ΪGS�ṩȫ������ͬ������
//Author:		Fred
//Create Time:	2007.7.18

#pragma once
#include "..\..\public\TeamDef.h"

class GSTeam : public BaseMemObj
{
public:
	GSTeam();
	~GSTeam(void);

public:

	//! GS��Ա��Ϣ
	struct tagMemberInfo : tagWSMemberInfo
	{
		bool bChanged;		//�ǳ�Ա��Ϣ�����˸ı�ı��
	};

	//! ȫ��Ĭ������
	struct tagDefaultSetup
	{
		long	lCorrelativeSkillID;//����ӹ����ļ���ID
		long	lMinSkillLevel;		//������ӵ���С���ܵȼ�
		long	lGoodsAllot;		//Ĭ�Ϸ��䷽ʽ
		long	lMaxMemberNum;		//Ĭ���������
		long	lSessionTimeoutS;	//�Ự��ʱ����

		float	fIncreasedExp_2;	//2�˾���ӳɱ���
		float	fIncreasedExp_3;	//3�˾���ӳɱ���
		float	fIncreasedExp_4;	//4�˾���ӳɱ���
		float	fIncreasedExp_5;	//5�˾���ӳɱ���
		float	fIncreasedExp_6;	//6�˾���ӳɱ���

		long	lExperienceRadii;	//�������뾶����λ����
		long	lExpDiff;			//
		long	lExpLimit;			//

		float	fExpAmerce;			//����ͷ�ϵ��

		tagDefaultSetup(long lGA, long lMMN, long lSTS);
	};
	struct tagShareQuestList
	{
		long lQuestID;
		char szName[20];
	};
public:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////							
	const	CGUID&	GetExID(void);														//! �õ�ID
	const	CGUID&	SetExID(const CGUID& guid);											//! ����ID
	const	char*	GetName(void);
	const	char*	SetName(const char *pName);

	//�˳�
	bool	Exit(const CGUID& PlayerGuid);																				
	//��λ
	bool	Demise(const CGUID& MasterGuid,const CGUID& guid);										
	//��ɢ
	bool	Disband(const CGUID& guid, BOOL bNotifyClient = TRUE);														
															
	//�õ���Ա�б�
	void GetMemberList(list<CGUID>& TemptList);
	//������ǰ�����
	const CGUID& IsMaster(const CGUID& guid);
	//������Ƿ��Ǹð��Ա��
	const CGUID& IsMember(const CGUID& guid);
	bool IsMember( const std::string &name ) const;
	//�õ����ŵ�ID
	const CGUID& GetMasterID(void);													
	//�õ���Ա��
	long GetMemberNum();
	long GetMemberNumInRgn(const CGUID& RgnGuid);
private:
	//! ��Ӧ��Ա����
	void OnMemberEnter(const CGUID& MemGuid);
	//! ��Ӧ��Ա�˳�
	void OnMemberExit(const CGUID& MemGuid);

public:
	void SwapTeamRgnID(CPlayer *player, CPlayer *pAim);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:	
	void GetDataFromArray(BYTE *pData, LONG &lPos);
	static void SendAboutSkillToC(vector<BYTE> *pByteArray);						//! ����������ӵļ���ID�����Ƶļ������ͻ���
	static void SendAboutSkillToC(DBWriteSet& setWriteDB);				//! ����������ӵļ���ID�����Ƶļ������ͻ���

	bool Initial(CMessage *pMsg);											//! ��ʼ������

	bool DoJoin(CMessage *pMsg);											//! ��Ҽ�����Ϣ

	void RadiateInfo(char* szInfo);											//! ����Ϣ�������г�Ա

	void SetGoodsAllot(long eSet);											//! �޸���Ʒ��������

	void SetOptControl(long ControlType);									//! ���ò�������

	//! �������õķ��䷽ʽ������һ����ʰȡ�߷�������ʰȡ�ߵ�ͼ�����ָ�룬
	//! ����Ϊʰȡ��ID
	CPlayer* GetOneOwner(const CGUID& Picker);
	//! �õ����Է����ߵ�����
	long GetOneOwnerNum(void);

	//!
	long GetSentaiPoints(void){return m_SentaiPoint;}
	//!
	void SetSentaiPoints(long Points){m_SentaiPoint = Points;}
	//! 
	void ChangeRegion(long RegionID, long x, long y);


	void	GetOnServerTeamMember(list<CGUID> &relist);						//! ��õ�ǰ�������ϵĳ�Ա�б�

	void MarkPlayerPosChange(const CGUID& PlayerGuid);						//! ����������λ�÷����˸ı�

	void RadiateAllMemberPos(void);											//! �㲥���г�Աλ��

	void OnPlayerIdioinfoChange(const CGUID& PlayerGuid, eUpdateIdioinfo eInfo);		//! ��Ӧ��ҷǶ���״̬�ı�

	void OnPlayerIdioinfoChange(const CGUID& PlayerGuid, eUpdateIdioinfo eInfo, long lValue);		//! new ��Ӧ��ҷǶ���״̬�ı�

	void OnPlayerStateChange(const CGUID& PlayerGuid, long lState);					//! ��Ӧ��ҷǶ���״̬�ı�

	void SendToAllMember(CMessage *pMsg);									//! ����Ϣ���͸����г�Ա

	void SendToAllRegionMember(CMessage *pMsg, long lRegionID);				//! ���͵������ڱ���ͼ�ĳ�Ա

	void PlayerLeaveHere(const CGUID& PlayerGuid);										//! ���¼����ڱ��������ĳ�Ա����

	void MemberStateChange(const CGUID& PlayerGuid, long lState);					//! ��Ա����״̬�ı�

	void MemberMapChange(const CGUID& PlayerGuid);					//! ��Ա��ͼ�ı�

	void TeamChat(CPlayer *pSender, char *pInfo);												//! ����

	void UpdateLeaderRecruitedNum(void);									//! ���¶ӳ�������ļ����

	void SendMemberBuffToMember(CPlayer *pPlayer, const CGUID &SendAimGuid);

	void SendMemberBuffToAll(CPlayer *pPlayer, DWORD dwBuffID, DWORD dwBuffLevel, BOOL bOpen);



	const CGUID& GetLeader(void);													//! �õ��쵼ID

	long GetTeamatesAmount(void);											//! ��Ա����

	//!	 �õ�һ�����������ڵĻ��ŵĳ�Ա
	void GetLucrativeArea_Alive(map<CGUID,CPlayer*>& RegionTeamates, const CGUID &RegionGUID, LONG lPosX, LONG lPosY, BOOL bForGoods);
	void GetLucrativeArea_Alive(list<CGUID>& listMember, const CGUID &RegionGUID, LONG lPosX, LONG lPosY, BOOL bForGoods);

	//! �õ�һ�����������ڵĻ��ŵĳ�Ա��ƽ���ȼ�
	long GetLucrativeAreaAverageLevel_Alive(const CGUID &RegionGUID, LONG lPosX, LONG lPosY, BOOL bForGoods);	

	//! �õ�һ�����������ڵĻ��ŵĳ�Ա��ƽ��ְҵ�ȼ�
	long GetLucrativeAreaAverageOccuLevelCoe_Alive(const CGUID &RegionGUID, LONG lPosX, LONG lPosY, BOOL bForGoods);

	long GetCurrentRegionTeamatesAmount(const CGUID &RegionGUID);					//! �õ������ڶ����Ա������
	
	//! �õ�һ�����������ڵĻ��ŵĳ�Ա����
	long GetLucrativeAreaMemberAmount_Alive(const CGUID &RegionGUID, LONG lPosX, LONG lPosY, BOOL bForGoods);				

	long GetCurrentServerTeamatesAmount();									//! �õ��ڷ������ϵĶ�Ա����

	CPlayer* FindTeamatesInCurrentRegion(const CGUID& RegionGUID);					//! �õ���������ڵ�һ�������Ա

	long CalculateExperience(CPlayer *pPlayer, long lMonsterLevel, DWORD lExp, LONG lPosX, LONG lPosY);		//! ���㾭��
	long CalculateOccuExperience(CPlayer *pPlayer, long lMonsterLevel, DWORD lExp, LONG lPosX, LONG lPosY);	//! ����ְҵ����
	long CalculateOccuPoint(CPlayer *pPlayer, long lMonsterLevel, DWORD lOccuP, LONG lPosX, LONG lPosY);	//! ����ְҵ����

	

	void AddQuest(DWORD dwQuestID,CPlayer* pPlayer,long lDistance);			//! ���������һ������

	void RunScript(char* strScirptName,CPlayer* pPlayer,long lDistance, char* strScirptName2 = NULL);	//! ���нű�	//��ȡ���鵱ǰ���� 
	
	void SetCurrShareQuest(long questID);

	long GetCurrShareQuest()					{return m_lCurrShareQuestID;}

	void UpdateShareQuestList(tagShareQuestList *quest=NULL,bool flag = false);						//! ���¹��������б�
	long FindShareQuest(long lquest);
	void ClearShareQuestList();
	void SetAcceptTime(DWORD time)				{ m_dwAcceptTime=time; }
	DWORD GetAcceptTime()						{return m_dwAcceptTime;}
	void SetMemberQuest(CGUID &guid,long lQuestID);
	void ClearMemberQuest();
	bool IsCompleteQuest(CGUID &guid,long lQuestID);

	//!				�ж��Ƿ�Ϊһ�������飬����ǣ������һ����GUID
	const CGUID&	GetMarriageOtherOne(CPlayer *pOnePlayer);
private:
	list<tagMemberInfo>::iterator FindMember(const CGUID& PlayerGuid);



	void PushBackToMsg(CMessage *pMsg);										//! �������Ķ���������ӵ���Ϣĩβ

	void RadiateMemberData(list<tagMemberInfo>::iterator ite);				//! �㲥һ����Ա����

	void RadiateMemberData(const CGUID& PlayerGuid);									//! �㲥һ����Ա����

	void SendMemberData(tagMemberInfo &MemberInfo, list<tagMemberInfo>::iterator iteAim);					//! ����һ����Ա���ݸ���һ����Ա



	void RadiateAllMemberIdioinfo(void);									//! �ڶ����й㲥���г�Ա�Ƕ�����Ϣ

	bool GetIdioinfo(CPlayer *pPlayer, tagMemberIdioinfo &MemberIdioinfo);	//! ��ȡһ����ҵķǶ�����Ϣ

	void IdioinfoMemberToMember(list<tagMemberInfo>::iterator iteSrc, list<tagMemberInfo>::iterator iteAim); //! ����һ����Ա����Ϣ����һ����Ա

	void RadiateMemberIdioinfo(list<tagMemberInfo>::iterator ite);			//! �ڶ����й㲥ĳ����Ա�Ƕ�����Ϣ

	void RadiateMemberIdioinfo(const CGUID& PlayerGuid);								//! �ڶ����й㲥ĳ����Ա�Ƕ�����Ϣ

	void SendAllMemberIdioinfo(list<tagMemberInfo>::iterator ite);			//! �����г�Ա�Ƕ�����Ϣ���͸�һ����Ա

	void SendAllMemberIdioinfo(const CGUID& PlayerGuid);								//! �����г�Ա�Ƕ�����Ϣ���͸�һ����Ա



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ա
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//! ����ID
	CGUID								m_guid;
	//!
	char								m_szName[20];
	//! �ӳ�ID
	CGUID								m_MestterGuid;
	//! ��Ա�б�
	list<tagMemberInfo>					m_MemberList;
	//! ��Ʒ���䷽ʽ
	long								m_eGoodsAllot;
	//! �����������
	long								m_eOptControl;
	//! �����ս���ܻ���
	long								m_SentaiPoint;
	//! ��һ�εķ���Ŀ��
	CGUID								m_LastAllotAim;
	//! �ڱ��������ϵĳ�Ա����
	long								m_lOnThisGsMemberNum;
	//�����ͽ������б�
	list<tagShareQuestList>				m_ShareQuestList;
	//�������ڽ��е��ͽ�����
	long								m_lCurrShareQuestID;
	//�ӳ�����ȫ�������ʱ��
	DWORD								m_dwAcceptTime;
public:
	//! ����Ĭ������
	static	tagDefaultSetup				s_tagDefaultSetup;

};