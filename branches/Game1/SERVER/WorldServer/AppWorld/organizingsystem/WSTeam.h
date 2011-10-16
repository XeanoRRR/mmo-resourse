//WSTeam:		�̳���COrganizing��������Ҷ������ݣ�ΪGS�ṩȫ������ͬ������
//Author:		Fred
//Create Time:	2007.7.18

#pragma once
#include "..\..\public\TeamDef.h"

class WSTeam:public BaseMemObj
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
	//�˳�
	bool Exit(const CGUID& PlayerGuid);																						
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
								
	//������ǰ�����
	const CGUID& IsMaster(const CGUID& guid);													
	//������Ƿ��Ǹð��Ա��
	const CGUID& IsMember(const CGUID& guid);													
	//�õ���Ա��
	long GetMemberNum();		
	//! �Ƿ��Ա����һ����ͼ
	bool IsInSameRegion(void);
	//�õ�ͷ��
	const CGUID& GetPlayerHeader();														
	//! �ж϶�Ա�����ȼ����Ƿ�������ķ�Χ��
	BOOL InLvlDifference(LONG lMaxLvlSpace);
	//! �ж����ж�Ա�ĵȼ��Ƿ��������Χ��
	BOOL InLvlSpace(LONG lMin, LONG lMax);
	//����һ����Ա������Ϸ
	void OnMemberEnterGame(CPlayer *pMember);										
	//����һ����Ա�뿪��Ϸ
	void OnMemberExitGame(const CGUID& MemGuid);											
	//! ��Ӧ��Ա���ָı�
	void OnMebereChangePoint(const CGUID &MemberGuid, long ChangePoint);
	//����Ա�˳���ʱ��ɾ�������б�˶�Ա���Ƽ�����
	void OnExit(const char* szname,const CGUID& PlayerGuid);


public:
	bool Initial(const CGUID& TeamGuid, const CGUID& MastterGuid, const CGUID& MemberGuid);			//! ��ʼ������

	list<tagWSMemberInfo>::iterator FindMember(const CGUID& PlayerGuid);	//! �ҵ�һ����Ա��λ��

	long GetOnlineMemberNum(void);											//! �õ����߳�Ա������

	long GetMemberMinLvl(void);												//! �õ���Ա��С�ȼ������ж�Ա�����ߣ��᷵��0

	bool PlayerInTeam(const CGUID& PlayerGuid);

	list<CGUID>	GetMemberIDList();											//! �õ���ԱID�б�

	//void UpdateTeamInfoToGS(void);										//! ��������Ϣ���µ�GS

	const CGUID& SetNewMastter();											//! �����¶ӳ�

	void SetGoodsAllot(long eSet);											//! ������Ʒ����취

	//! �õ�ս�ӻ��֣����г�Ա���ֵ��ܺͣ�
	long GetSentaiPoint(void){return m_SentaiPoint;}
	//! ���óͷ�����
	void SetPenaltyPoints(long Points);
	//! ���ò�������
	void SetOptControl(long ControlType);
	//! ��ò�������
	long GetOptControl(void){return m_eOptControl;}	
	//! �ܷ����һ������
	bool IsAbleControl(eTeamOptControlType ControlType){return (0 != (ControlType & m_eOptControl));}					


	bool AddToByteArray(vector<BYTE>& pByteArray);
	
	//! ���¹��������б�,true��չ����б�
	void UpdateShareQuestList(long lGSID,tagShareQuestList* quest=NULL,bool flag=false);		
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
	//! �����������
	long								m_eOptControl;
	//! �����ս���ܻ���
	long								m_SentaiPoint;
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