//WSTeam:		�̳���COrganizing��������Ҷ������ݣ�ΪGS�ṩȫ������ͬ������
//Author:		Fred
//Create Time:	2007.7.18

#pragma once

#include "../../../../Public/Common/TeamDef.h"

class WSTeam
{
public:
	WSTeam();
	~WSTeam(void);

public:

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
		long	lExpDiff;			//�����
		long	lExpLimit;			//��Ҿ���ֵ�ݶ�������ϱ���

		float	fExpAmerce;			//����ͷ�ϵ��	
		
		tagDefaultSetup(long lGA, long lMMN, long lSTS);
	};
	struct tagShareQuestList
	{
		long lQuestID;
		char szName[20];
	};
public:
	//����Ҫ������֯����Ա
	bool DoJoin(const CGUID& ExecuteGuid,const CGUID& BeExecutedGuid,long lAgree,tagTime& Time);	
	//������Ա����
	bool Invite(const CGUID& InvitedGuid, const CGUID& BeInvitedGuid) ;							
	//�˳�
	bool Exit(const CGUID& PlayerGuid);													
	//����
	bool FireOut(const CGUID& FireID,const CGUID& BeFiredID);									
	//��λ
	bool Demise(const CGUID& MasterID,const CGUID& guid);										
	//��ɢ
	bool Disband(const CGUID& guid);														
	//�õ���֯��ID
	//long GetID();
	const	CGUID& GetExID();	
	void	SetExID(const CGUID& guid);
	//�õ����ŵ�ID
	const CGUID& GetMasterID();													
	//�õ���Ա�б�
	void GetMemberList(list<CGUID>& TemptList);									
	//������ǰ�����
	const CGUID& IsMaster(const CGUID& guid);													
	//������Ƿ��Ǹð��Ա��
	const CGUID& IsMember(const CGUID& guid);													
	//�õ���Ա��
	long GetMemberNum();														
	//�õ�ͷ��
	const CGUID& GetPlayerHeader();														
	//������ҵ���Ϣ����֯�ĳ�Ա�б�
	bool UpdatePlayerDataToOrgMemberList(const CGUID& PlayerGuid);	
	//! �ж϶�Ա�����ȼ����Ƿ�������ķ�Χ��
	BOOL InLvlDifference(LONG lMaxLvlSpace);
	//! �ж����ж�Ա�ĵȼ��Ƿ��������Χ��
	BOOL InLvlSpace(LONG lMin, LONG lMax);
	//����һ����Ա������Ϸ
	void OnMemberEnterGame(const CGUID& MemGuid);										
	//����һ����Ա�뿪��Ϸ
	void OnMemberExitGame(const CGUID& MemGuid);											
	//��Ա�ȼ������˱仯
	void OnMemberLvlChange(const CGUID& MemGuid,long lLvl);								
	//��Ա�ı���λ��(����)
	void OnMemberPosChange(const CGUID& MemGuid,long lNewPosID);							
	//����Ա�˳���ʱ��ɾ�������б�˶�Ա���Ƽ�����
	void OnExit(const char* szname,const CGUID& PlayerGuid);
protected:
	bool Initial();															//! ��ʼ��

public:
	bool Initial(const CGUID& TeamGuid, const CGUID& MastterGuid, const CGUID& MemberGuid);			//! ��ʼ������

	list<tagWSMemberInfo>::iterator FindMember(const CGUID& PlayerGuid);	//! �ҵ�һ����Ա��λ��

	long GetOnlineMemberNum(void);											//! �õ����߳�Ա������

	bool PlayerInTeam(const CGUID& PlayerGuid);

	list<CGUID>	GetMemberIDList();											//! �õ���ԱID�б�

	//void UpdateTeamInfoToGS(void);										//! ��������Ϣ���µ�GS

	const CGUID& SetNewMastter();											//! �����¶ӳ�

	void SetGoodsAllot(long eSet);											//! ������Ʒ����취

	bool AddToByteArray(vector<BYTE>& pByteArray);
	
	//! ���¹��������б�,true��չ����б�
	void UpdateShareQuestList(long lGSID,tagShareQuestList* quest=NULL,bool flag=false,CPlayer* player=NULL);		
	bool FindShareQuest(long lquest);										//! ���ҹ��������б�
	void SetCurrShareQuest(long lGSID,long questID);
	long GetCurrShareQuest()					{return m_lCurrShareQuestID;}
	bool DelCurrShareQuest(long lGSID,char* name);
	void SetMemberQuest(long lGSID,CGUID &guid,long lQuestID = 0);
	void ClearMemberQuest();
private:
	void PushBackToMsg(CMessage &msg);										//! �������Ķ���������ӵ���Ϣĩβ

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ա
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//! ����ID
	CGUID								m_guid;
	//! �ӳ�ID
	CGUID								m_MestterGuid;
	//! ��Ա�б�
	list<tagWSMemberInfo>				m_MemberList;
	//! ��Ʒ���䷽ʽ
	long								m_eGoodsAllot;
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