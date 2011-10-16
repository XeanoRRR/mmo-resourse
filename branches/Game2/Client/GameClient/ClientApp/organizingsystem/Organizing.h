//Function:	������һ�����࣬�ṩ���л�ϵͳ��Ҳ��һ���ӿ���,
			//�ṩ�˶���֯�Ļ���������ÿ�����������������Щ������
			//���磬�����������Ա����ɢ��������
//Author:	Joe
//Create Time:2004.6.11

#pragma once
#include "../../../Public/Common/OrganizingDef.h"
class COrganizing
{
public:
	COrganizing(void);
	virtual ~COrganizing(void);

	virtual bool Save() = 0;											//���𱣴�����
	virtual bool Load() = 0;											//����װ������
public:

	//�ṩ��������̳�ʹ�õĹ������
public:
	virtual bool ApplyForJoin(CGUID& PlayerID,long lLvl,long lOrgMemID = 0) = 0;		//���л��Ա�������
	virtual bool DoJoin(const CGUID& ExecuteID,const CGUID& BeExecutedID,bool lAgree,tagTime& Time) = 0;		//����Ҫ������֯����Ա
	virtual bool Invite(const CGUID& InvitedID, const CGUID& BeInvitedID) = 0;					//������Ա����
	virtual bool Exit(const CGUID& PlayerID) = 0;											//�˳�
	virtual bool FireOut(const CGUID& FireID,const CGUID& BeFiredID) = 0;							//����
	virtual bool DubAndSetJobLvl(const CGUID& DubID,const CGUID& BeDubedID,string& strTitle,long lLvl) = 0;//����ƺź�����ְ��
	virtual bool EndueRightToMember(const CGUID& EndueID,const CGUID& BeEnduedID,eFactionPurviewIndex ePV) = 0;//�����ԱȨ��
	virtual bool AbolishRightToMember(const CGUID& EndueID,const CGUID& BeEnduedID,eFactionPurviewIndex ePV) = 0;//ȡ����Աĳ��Ȩ��
	virtual bool Pronounce(const CGUID& PlayerID,string& strPro,tagTime& Time) = 0;		//����
	virtual bool LeaveWord(const CGUID& PlayerID,string& strWords,tagTime& Time) = 0;		//����
	virtual bool EditLeaveWord(const CGUID& PlayerID,long lID,eOperator eOP)=0;			//�༭����
	virtual bool Disband(const CGUID& ID) = 0;												//��ɢ
	virtual bool Demise(const CGUID& MasterID,const CGUID& ID) = 0;								//��λ
	virtual bool OperatorTax(const CGUID& PlayerID) = 0;									//����˰��

	virtual const CGUID& GetID() = 0;														//�õ���֯��ID
	virtual const CGUID& GetMasterID() = 0;													//�õ����ŵ�ID
	virtual long IsOwnedCity(long lCityID) = 0;										//�ð��Ƿ�ӵ������
	virtual	void AddOwnedCity(long lCityID)= 0;										//�����л��������ID
	virtual	bool DelOwnedCity(long lCityID)= 0;										//ɾ��һ��ӵ�е�����ID
	virtual bool IsHaveEnymyFaction() = 0;											//�Ƿ��еж��л����
	virtual list<CGUID> GetFriendList() = 0;											//�õ�������֯�б�
	virtual	void AddEnemyOrganizing(list<long>& TemptList) = 0;						//���һ���ж���֯�б�
	virtual	void DelEnemyOrganizing(list<long>& TemptList) = 0;						//ɾ��һ���ж���֯�б�
	virtual	void AddEnemyOrganizing(const CGUID& FactionID) = 0;							//���һ���ж��л�
	virtual	void DelEnemyOrganizing(const CGUID& FactionID) = 0;							//ɾ��һ���ж��л�
	virtual	void ClearEnemyFation() = 0;											//����ж��л��б�
	virtual long IsMaster(const CGUID& ID) = 0;											//������ǰ�����
	virtual long IsMember(const CGUID& ID) = 0;											//������Ƿ��Ǹð��Ա��
	virtual long IsSuperiorOrganizing() = 0;										//�жϸ���֯�Ƿ����ϼ���֯
	virtual void SetSuperiorOrganizing(const CGUID& ID) = 0;								//�����ϼ���֯
	virtual	long RemoveApplyMember(const CGUID& ID) = 0;									//�Ƴ�һ�������Ա
	virtual const tagTime* GetEstablishedTime() const = 0;							//�õ����лὨ��ʱ��
	//virtual void SetLogo(long lLogoID) = 0;											//���ûձ�
	//virtual long GetLogo() = 0;														//�õ��ձ�
	virtual bool IsLWFunction() = 0;												//�Ƿ�������Թ���
	virtual void SetLWFunction(bool b) = 0;											//�������Թ���
	virtual bool GetIsPermit()=0;													//�õ��Ƿ�����ͬ�ˡ���ս��Ͷ�������
	virtual void SetIsPermit(long lPlayer,bool b)=0;								//�����Ƿ�����ͬ�ˡ���ս��Ͷ�������

	virtual bool IsControbute(const CGUID& MemID) = 0;										//�õ��Ƿ��ǹ�����
	virtual void SetControbuter(const CGUID& MemID,bool bControbute) = 0;					//���ù�����

	virtual void AddMember(tagFacMemInfo& Memnfo)=0;									//���һ����Ա
	virtual bool DelMember(const CGUID& ID)=0;												//�ӱ�����ɾ��һ����Ա

	virtual long GetMemberNum() = 0;												//�õ���Ա����

protected:
	virtual bool Initial() = 0;														//��ʼ��
	virtual bool IsUsingPV(const CGUID& ID,eFactionPurviewIndex ePV) = 0;								//�ж�ĳID�Ƿ���ʹ�ô�Ȩ��
	virtual void SetMemPV(const CGUID& ID,eFactionPurviewIndex ePV) = 0;								//��ĳID����һ��Ȩ��
	virtual void AbolishMemPV(const CGUID& ID,eFactionPurviewIndex ePV) = 0;							//ȡ��ĳ����Ա��Ȩ��
	virtual bool SetName(string& strName) = 0;										//������֯������
	virtual bool CheckOperValidate(const CGUID& ExecuteID,const CGUID& BeExecutedID,eFactionPurviewIndex ePV)=0;//���ĳIDִ��ĳ��Ȩ�޲����ĺϷ���
	virtual bool CheckOperValidate(const CGUID& ExecuteID,eFactionPurviewIndex ePV)=0;					//���ĳIDִ��ĳ��Ȩ�޲����ĺϷ���
	
};
