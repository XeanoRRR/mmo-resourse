#pragma once
#include "organizing.h"

class CConfederation :
	public COrganizing
{
public:
	CConfederation(long lID,long lMastterID,string& strName = string(""));
	~CConfederation(void);

	bool Save();											//���𱣴�����
	bool Load();											//����װ������
	bool SetMembers(BYTE* pByte,long lSize);				//���ó�Ա��Ϣ
	bool GetMembersData(vector<BYTE>& ByteArray);			//�õ���Ա��Ϣ����
	bool AddToByteArray();									//��ӵ�CByteArray
public:
	
	//����(�±����漰��ID�ǰ���ID)
private:
	long	m_lID;											//��ͬ�˵�ȫ��ΨһID
	string	m_strName;										//ͬ������

	long	m_lMastterID;									//�������ID	
	map<long,tagMemInfo> m_Members;							//��ͬ�����а�ἰ�ð���Ȩ�޺ͳƺ�
	long				m_ApplyPerson;						//��ǰ�������İ���

	tagTime	m_EstablishedTime;								//���˳���ʱ��32

	static	long MAX_MemberNum;								//����Ա��
	//��صĹ��ܲ���
public:
	bool ApplyForJoin(long lPlayerID,long lLvl,long lOrgMemID = 0);		//�ް��ɳ�Ա�������
	bool ClearApplyList(long lPlayerID);								//���������Ա�б�
	bool DoJoin(long lExecuteID,long lBeExecutedID,tagTime& Time);		//����Ҫ�������˵���֯
	bool Invite(long lInvitedID, long lBeInvitedID);					//������Ա����
	bool Exit(long lPlayerID);											//�˳�
	bool FireOut(long lFireID,long lBeFiredID);							//����
	bool DubAndSetJobLvl(long lDubID,long lBeDubedID,string& strTitle,long lLvl);//����ƺ�
	bool EndueRightToMember(long lEndueID,long lBeEnduedID,ePurview ePV);//�����ԱȨ��
	bool AbolishRightToMember(long lEndueID,long lBeEnduedID,ePurview ePV);//ȡ����Աĳ��Ȩ��
	bool Pronounce(long lPlayerID,string& strPro,tagTime& Time);		//����
	bool LeaveWord(long lPlayerID,string& strWords,tagTime& Time);		//����
	bool Disband(long lID);												//��ɢ
	bool Demise(long lMasterID,long lID);								//����֮λ
	bool OperatorTax(long lPlayerID);									//����˰��

	//���Բ����ӿ�
	long GetID(){return m_lID;}								//�õ���֯��ID
	long GetMasterID(){return m_lMastterID;}				//�õ����ŵ�ID
	//�������������
	//����ֵ:������������0�����򷵻ظ�ͬ�˵�ID
	long IsMaster(long lID)							
	{
		if(m_lMastterID == lID)	return m_lID;
		else return 0;
	}
	//������Ƿ��Ǹ�ͬ�˳�Ա��
	//����ֵ:���ǳ�Ա����0�����򷵻ظ�ͬ�˵�ID
	long IsMember(long lID)
	{
		if(m_Members.find(lID) != m_Members.end())	return m_lID;
		else return 0;
	}
	//�жϸ���֯�Ƿ����ϼ���֯
	//����ֵ:0��ʾû�У������ʾ���ϼ���֯
	long IsSuperiorOrganizing()
	{
		return false;
	}
	//�����ϼ���֯
	void SetSuperiorOrganizing(long lID)
	{
		return;
	}
	//�õ��ð�Ὠ��ʱ��
	const tagTime* GetEstablishedTime() const {	return &m_EstablishedTime; }
	bool IsLWFunction() {return false;}												//�Ƿ�������Թ���
	void SetLWFunction(bool b){}													//�������Թ���


	long IsOwnedCity(long lCityID);													//�жϸøð��Ƿ�ӵ������
	void AddOwnedCity(long lCityID);												//���øð�������ID
	bool DelOwnedCity(long lCityID);												//ɾ��һ��ӵ�е�����ID

	//�Եж԰���б�������
	bool IsHaveEnymyFaction();														//�Ƿ��еж԰�����
	list<long> GetFriendList();														//�õ�������֯�б�
	list<long> GetEnemyList();														//�õ��ж���֯�б�
	void AddEnemyOrganizing(list<long>& TemptList);									//���һ���ж���֯�б�
	void DelEnemyOrganizing(list<long>& TemptList);									//ɾ��һ���ж���֯�б�
	void AddEnemyOrganizing(long lFactionID);										//���һ���ж԰��
	void DelEnemyOrganizing(long lFactionID);										//ɾ��һ���ж԰��
	void ClearEnemyFation();														//����ж԰���б�

	long RemoveApplyMember(long lID);												//�Ƴ�һ�������б��еĳ�Ա
	void SetLogo(long lLogoID){}													//���ûձ�
	long GetLogo(){return 0;}														//�õ��ձ�

protected:
	bool Initial();																	//��ʼ��
	bool SetName(string& strName);													//���ð�������
	bool IsUsingPV(long lID,ePurview ePV);											//�ж�ĳ���Ƿ���ʹ�ô�Ȩ��
	void SetMemPV(long lID,ePurview ePV);											//��ĳID����һ��Ȩ��
	void AbolishMemPV(long lID,ePurview ePV);										//ȡ��ĳ����Ա��Ȩ��
	bool DelMember(long lID);														//�ӱ�����ɾ��һ����Ա
	bool CheckOperValidate(long lExecuteID,long lBeExecutedID,ePurview ePV);		//���ĳIDִ��ĳ��Ȩ�޲����ĺϷ���
	bool CheckOperValidate(long lExecuteID,ePurview ePV);							//���ĳIDִ��ĳ��Ȩ�޲����ĺϷ���
};
