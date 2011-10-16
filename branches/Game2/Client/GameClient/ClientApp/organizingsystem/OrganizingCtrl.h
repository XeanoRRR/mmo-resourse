#pragma once
//OrganizingCtrl.h
//Function:	//���ฺ������л�Ľ�������ɢ���Լ��԰ͬ�˵Ĺ������.
			//�����Organizings�����Ĺ���
			//�����л�ϵͳ��װ�ء��洢������
//Author:Joe
//Create Time:2004.6.11
//Note:����ȫ��ֻ��Ψһ��ʵ��
class COrganizing;
class CFaction;
class CUnion;
struct tagTime;

//�л����а�ṹ
struct tagFacBillboard
{
	long	lFactionID;		//�л�ID
	string	strName;		//�л�����
	long	lNum;			//����
};

//�л���Ϣ�ṹ��������սʱ��
struct DeclareWarFacInfo
{
	CGUID	FactionID;		//�л�ID
	BYTE	btCountry;		//����ID
	char	strName[20];	//�л�����
	long	lRelations;		//�ͱ���Ĺ�ϵ,0:�޹�ϵ,1:����,2:ͬ��,3:�ж�,4:����
};

struct tagAllFacState
{
	CGUID	FactionID;
	BYTE	btCountry;
	char	strFactionName[20];
	bool	bUionRelation;			//�Ƿ���ͬ�˹�ϵ
	bool	bFactWarEnemyRela;		//�Ƿ��ǰ�ս�жԹ�ϵ
    bool	bCityWarEnemyRela;		//�Ƿ��ǳ�ս�жԹ�ϵ
};


struct KeyCompareByFaction
{
	bool operator()(const tagAllFacState& KeyX,const tagAllFacState& KeyY)
	{
		if(KeyX.bUionRelation != KeyY.bUionRelation )
		{
			if(KeyX.bUionRelation == true)
				return true;
			else
				return false;
		}
		else
		{
			if(KeyX.bFactWarEnemyRela != KeyY.bFactWarEnemyRela)
			{
				if(KeyX.bFactWarEnemyRela == true)
					return true;
				else
					return false;
			}
			else
			{
				if(KeyX.bCityWarEnemyRela != KeyY.bCityWarEnemyRela)
				{
					if(KeyX.bCityWarEnemyRela)
						return true;
					else
						return false;
				}
				else
				{
					if(KeyX.FactionID < KeyY.FactionID)
						return true;
					else
						return false;
				}
			}
		}
		return false;
	}
};

class COrganizingCtrl
{
private:
	COrganizingCtrl(void);
	~COrganizingCtrl(void);
public:
	void DecordFactionListFromByteArray(BYTE* pByte, long& pos,long lPage,long lFactionNum);	//��ѹ�л��б�
	void DecordFactionBillboardFromByteArray(BYTE* pByte,long& pos,string& str);				//��ѹ�л����а�
	void DecordDeclareFactionInfoFromByteArray(BYTE* pByte, long& pos,long lPage,long lFactionNum);//��ѽ��ս�л��б�

	void DecordAllFactionsFromByteArray(BYTE* pByte,long& pos);					//��ѹ�л�����

public:
	struct tagApplyFaction
	{
		CGUID ID;
		char strName[20];
	};

	
	//����
private:
	static COrganizingCtrl* instance;										//����Ψһʵ�����

	CFaction* m_FactionOrganizing;											//��ҵ��л���֯
	CUnion* m_ConfedeOrganizing;											//��ҵ�ͬ����֯

	CGUID	m_ApplyFactionID;
	list<tagApplyFaction>	m_FactionList;									//�л��б�
	long	m_lFactinNum;
	long	m_lCurMaxPage;

	string	m_strFactionBillboardTitle;										//���а����
	list<tagFacBillboard>	m_FactionBillboard;								//��Ա�������а�

	list<DeclareWarFacInfo>		m_DecWarFationList;							//��ս���л��б�
	long	m_lDecWarFactionNum;
	long	m_lDecWarCurMaxPage;

	//���������л�
	set<tagAllFacState,KeyCompareByFaction>			m_AllFactions;			//��ǰϵͳ�������л�

	//�ڲ������ӿ�
private:
	bool m_bIsSendGetOrgaData;
	//��������,�ṩ���ⲿ�����Ľӿ�
public:
	const list<tagApplyFaction>& GetFactionList( long lPage,LONG64 lNetSessionID,long lPassword);
	void SetApplyFaction(const CGUID& ID)	{m_ApplyFactionID=ID;	}
	const CGUID& GetApplyFaction()			{return m_ApplyFactionID;	}
	bool IsNextFactionPage(long lPage);

	list<DeclareWarFacInfo>& GetDecWarFactionList(long lPage,LONG64 lNetSessionID,long lPassword);
	bool IsNetDecWarFactionPage(long lPage);
	bool IsDecWarFaction(const CGUID& FactionID);					//�Ƿ����������л���ս
	void SetDecWarFactionRelation(const CGUID& FactionID,long lRel);	//����һ����ս�л�Ĺ�ϵ

	void GetFactionStatInfo(set<tagAllFacState,KeyCompareByFaction>& Factions,long lPage,long lPageSize);//�õ�һҳ�л�״̬�б�
	bool IsNextFactionFromAllFaction(long lPage,long lPageSize);

	const string& GetFactionBillboardTitle()	{return m_strFactionBillboardTitle;	}
	//�õ��л����а�
	const list<tagFacBillboard>& GetFactionBillboard() const {return m_FactionBillboard;}
		
	//���һ���л���֯
	void SetFactionOrganizing(CFaction* pOrgan);
	void DelFactionOrganizing();
	
	//�õ�ĳ���л���֯
	CFaction* GetFactionOrganizing();						//������֯ID�õ���Ӧ����֯ʵ��
	
	//���һͬ����֯
	void SetConfederationOrganizing(CUnion* pOrgan);
	void DelConfederationOrganizing();
	//�õ�ĳ��ͬ����֯
	CUnion* GetConfederationOrganizing();				//������֯ID�õ���Ӧ����֯ʵ��

	bool GetIsSendingGetOrgaData()	{return m_bIsSendGetOrgaData;}
	void SetIsSendingGetOrgaData(bool b){m_bIsSendGetOrgaData=b;}
	//�������֯���еĲ���
	bool ApplyForJoin(const CGUID& FactionID);															//���л��Ա�������
	bool DoJoin(const CGUID& BeExecutedID);															//����Ҫ������֯����Ա

	bool CreateFaction(long lPlayerID,long lLvl,tagTime& Time,string& strName = string(""));	//�����л�
	bool DisbandFaction(const CGUID& PlayerID,const CGUID& FactionID);										//��ɢ�л�
	bool CreateConfederation(long lTargFactionID,string& strName= string(""));					//����ͬ��
	bool DisbandConferation(const CGUID& PlayerID,const CGUID& ConfeID);										//��ɢͬ��
	bool TransferIOwnerCity(long lOwnedID,long lBeTransferedID,long lCiytID);					//ת������
	bool BuyFactionLogo(long lPlayerID,long lIconID,tagTime& CurrTime);							//����ձ�

	long IsFactionMaster(long lPlayerID);										//�жϸ�����Ƿ��ǰ���
	long IsConferationMaster(long lFactionID);									//�жϸð��Ƿ�������
	long IsFreePlayer(long lPlayerID);											//�жϸ�����Ƿ������κ��л�
	long IsFreeFaction(long lFactionID);										//�жϸð��Ƿ������κ�ͬ��
	long RemovePersonFromApplyFactionList(long lPlayerID);						//�����������л��б����Ƴ�һ��������Ա
	long RemoveFactionFromApplyConfederList(long lFactionID);					//���������������б����Ƴ�һ��������֯
	bool IsAggressCityTime();													//�õ���ǰ�Ƿ��ǹ���սʱ�� 

	//��Ӧ���������л�仯,ɾ��һ���л�
	void DelFaction(const CGUID& FactionID);
	//��Ӧ���������л�仯,���һ���л�
	void AddFaction(const CGUID& FactionID,char* strFactionName);
	string GetFactionNameByID(const CGUID& FactionID);									//�õ��л������

	BYTE	GetFactionCountry(const CGUID& ID);

	bool	IsUnionRelation(const CGUID& FactionID);									//�õ��Լ��л�����л��Ƿ���ͬ�˹�ϵ
	bool	IsEnemyRelationWithFactionWar(const CGUID& FactionID);						//�Ƿ��ǰ�ս�ĵжԹ�ϵ
	bool	IsEnemyRelationWithCityWar(const CGUID& FactionID);						//�ж��Ƿ��ǳ�ս�ĵжԹ�ϵ
	
	static COrganizingCtrl* getInstance();										//�õ�Ψһʵ�����
	bool   Initialize();														//��ʼ��
	void   Release();															//�ͷ���Դ
	void   ReNew();

};

//�õ���֯��������
inline COrganizingCtrl* GetOrganiCtrl()
{
	return COrganizingCtrl::getInstance();
}