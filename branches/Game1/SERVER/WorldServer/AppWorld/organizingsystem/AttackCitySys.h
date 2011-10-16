///////////////////////////////////////////////////////////////////////////////////////
//AttackCitySys.h
//����սϵͳ
//Function:��ɹ���սϵͳ��ʱ����Ƶȵ�
//Author:Joe
//Create Time:2004.7.29
#pragma once
#include "organizingctrl.h"
#include "..\public\Timer.h"
#include "..\WorldCityRegion.h"
#include "..\public\Date.h"
//
//typedef struct tagAttackCityTime
//	{
//		long	lTime;					//�ڼ���
//		long	lCityRegionID;			//��ս��ͼ
//		ULONG	lDeclarWarEventID;
//		tagTime DeclarWarTime;			//��սʱ��
//		ULONG	lStartInfoEventID;
//		tagTime	AttackCityStartInfoTime;//���ǿ�ʼ��ʾʱ��
//		ULONG	lStartEventID;
//		tagTime	AttackCityStartTime;	//���ǿ�ʼʱ��
//		ULONG	lEndInfoEventID;
//		tagTime	AttackCityEndInfoTime;	//���ǽ�����ʾʱ��
//		ULONG	lEndEventID;
//		tagTime	AttackCityEndTime;		//���ǽ���ʱ��
//		ULONG	lMassEventID;
//		tagTime	MassTime;				//����ʱ��
//		ULONG	lClearEventID;
//		tagTime	ClearPlayerTime;		//�����ս�����������ʱ��
//		ULONG	lRefreshEventID;
//		tagTime	RefreshRegionTime;		//ˢ�³���ʱ��
//
//		eCityState	RegionState;		//���ս���е�ǰ����������״̬
//		list<CGUID>	DecWarFactions;		//����γ�ս��ս�İ��
//	}tagACT;
//
//
//class CAttackCitySys
//{
//private:
//	CAttackCitySys(void);
//	~CAttackCitySys(void);
//
//public:
//	
//	struct tagGetTodayTaxTime
//	{
//		ULONG lEventID;
//		long lHour;
//		long lMinute;
//		long lSecond;
//	};
//
//private:
//	static map<long,tagACT>		m_Attacks;
//	typedef map<long,tagACT>::iterator itAttack;
//
//	static  tagGetTodayTaxTime m_GetTodayTaxTime;
//	static CAttackCitySys* instance;											//����Ψһʵ�����
//
//public:
//	static CAttackCitySys* getInstance();										//�õ�Ψһʵ�����
//	static bool   Initialize();													//��ʼ��
//	static bool	  Reload();
//	void   Release();															//�ͷ���Դ
//
//	bool AddToByteArray(vector<BYTE>& ByteArray);								//��ӵ�CByteArray
//	bool UpdateApplyWarFacsToGameServer(long lWarID,vector<BYTE>& ByteArray);	//�����ս��ᵽCByteArray
//	void GenerateSaveData();
//	void	SetDecWarFactionsFromDB(long lRegionID,tagTime& WarStartTime,list<CGUID>& DecWarFactions);
//public:
//	static	void CALLBACK OnAttackCityStart(LPARAM lparam);						//���ǿ�ʼ
//	static	void CALLBACK OnAttackCityEnd(LPARAM lparam);						//���ǽ���
//	static	void CALLBACK OnDeclarWar(LPARAM lparam);							//������ս
//	static	void CALLBACK OnMass(LPARAM lparam);								//���Ǽ���
//	static	void CALLBACK OnClearOtherPlayer(LPARAM lparam);					//���������Ա
//	static	void CALLBACK OnRefreshRegion(LPARAM lparam);						//ˢ�³���
//	static	void CALLBACK OnAttackCityStartInfo(LPARAM lparam);					//���ǿ�ʼ����ʱ��ʾ��Ϣ
//	static	void CALLBACK OnAttackCityEndnfo(LPARAM lparam);					//���ǽ�������ʱ��ʾ��Ϣ
//	
//	eCityState GetCityState(long lRegionID);									//�õ�һ�������ĵ�ǰ״̬
//	eCityState GetCityStateByWarNum(long lWarTime);								//�õ�ս����ʾ��ǰ״̬
//	long GetWarNumByRegionIDAndCurTime(long lRegionID);							//�õ���ǰʱ���һ�������Ĺ��Ǳ�ʾ		
//	void GetDecWarFactions(long lRegionID,list<CGUID>& Factions);				//�õ�һ����������ս���
//	void GetDecWarFactionsByWarNum(long lWarNum,list<CGUID>& Factions);			//�õ�һ��ս������ս���
//	long	GetWarRegionIDByTime(long lTime);
//	bool	IsAlreadyDeclarForWar(long lWarNum,const CGUID& FactionGuid);		//�ж�һ������Ƿ��ĳ��ս����ս
//	bool	IsAlreadyDeclarForWar(const CGUID& FactionGuid);								//�ж�һ������Ƿ���ս
//	bool	OnPlayerDeclareWar(const CGUID& PlayerGuid,long lWarID,long lMoney);			//��һ�������һ������սʱ
//	bool	IsEnemyRelation(const CGUID& FactionGuid1,const CGUID& FactionGuid2);					//�Ƿ��ڵжԹ�ϵ
//	bool	FindInList(list<long>& TemptList,long lID);							//list���Һ���
//
//	void UpdateAllDecFacToGameServer();											//�������е���ս��ᵽGameServer
//	
//	void	OnFacWinCity(long lWarNum,long lWarRegionID,const CGUID& FactionGuid,const CGUID& UnionGuid);	//����һ�����Ӯ���˳�ս]
//	void	OnCityWarEnd(long lWarNum);											//�ݵ�����ս����
//	void    OnCityWarEndToGameServer(long lRegionID,long lTime);				//���ͽ�����Ϣ��Gameserver
//
//	void	SetCityWarEnemyFactions(long lRegionID,list<CGUID>& DecWarFactions);	//����װ�ص���ս������õжԹ�ϵ
//
//	void	InitialCityAllFactionEnemyRelation();								//������ս������°��ĵжԹ�ϵ
//	void	UpdateCityAllFactionEnemyRelation();								//������ս������°��ĵжԹ�ϵ
//};
//CAttackCitySys* GetAttackCitySys();
