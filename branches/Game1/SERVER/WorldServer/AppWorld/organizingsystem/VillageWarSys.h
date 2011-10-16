#pragma once
#include "organizingctrl.h"
#include "..\WorldRegion.h"
#include "..\public\Date.h"

///////////////////////////////////////////////////////////////////////////////////////
//VillageWarSys.h
//��սϵͳ
//Function:��ɰ�սϵͳ����ع���
//Author:Joe
//Create Time:2005.7.13
//////////////////////////////////////////////////////////////////////////

//
//struct tagVilWarSetup
//{
//	long	lID;				//��ʾID
//	long	lWarRegionID;		//ս���ĵ�ͼ
//	ULONG	lDeclarWarEventID;
//	tagTime DeclarWarTime;		//��սʱ��
//	ULONG	lStartInfoEventID;
//	tagTime WarStartInfoTime;	//��ս��ʼ��Ϣʱ��
//	ULONG	lStartEventID;
//	tagTime WarStartTime;		//��ս��ʼʱ��
//	ULONG	lEndInfoEventID;
//	tagTime WarEndInfoTime;		//��ս������Ϣʱ��
//	ULONG	lEndEventID;
//	tagTime WarEndTime;			//��ս����ʱ��
//	ULONG	lClearEventID;
//	tagTime	ClearPlayerTime;	//����ݵ��ͼ�����ʱ��
//	long	lVilRegionID;		//�����ͼ
//		
//	eCityState	RegionState;	//���ս���е�ǰ����������״̬
//	list<CGUID>	DecWarFactions;	//�����İ��
//};
//class CVillageWarSys
//{
//public:
//	CVillageWarSys(void);
//	~CVillageWarSys(void);
//
//private:
//	static CVillageWarSys* instance;											//����Ψһʵ�����
//
//
//	map<long,tagVilWarSetup>	m_VillageWars;									//��ս��	
//	typedef map<long,tagVilWarSetup>::iterator itVelWar;
//
//	bool	FindInList(list<long>& TemptList,long lID);							//list���Һ���
//public:
//	static CVillageWarSys* getInstance();										//�õ�Ψһʵ�����
//	bool   Initialize();														//��ʼ��
//	bool   ReLoad();															//����
//	void   Release();	
//	bool   CheckSetup();														//����ս�ʹ��ӵ�ͼ�Ƿ���һ����������
//
//	bool	AddToByteArray(vector<BYTE>& ByteArray);							//��ӵ�CByteArray
//	bool	UpdateApplyWarFacsToGameServer(long lWarID,vector<BYTE>& ByteArray);//�����ս��ᵽCByteArray
//	void	GenerateSaveData();
//	void	SetDecWarFactionsFromDB(long lRegionID,tagTime& WarStartTime,list<CGUID>& DecWarFactions);
//public:
//
//	bool	GetVilWarSetupByTime(tagVilWarSetup& VilWarSetup,long lTime);		//�õ�ĳ�ε�ս���Ĳ���
//	long	GetWarRegionIDByTime(long lTime);
//	long	GetWarTimeByRegionID(long lRegionID);
//	static	void CALLBACK OnDelcareWar(LPARAM lparam);							//��ս��ʼ
//	static	void CALLBACK OnAttackVillageStart(LPARAM lparam);					//���忪ʼ
//	static	void CALLBACK OnAttackVillageEnd(LPARAM lparam);					//�������
//	static	void CALLBACK OnClearPlayer(LPARAM lparam);							//���������Ա
//	static	void CALLBACK OnAttackVillageStartInfo(LPARAM lparam);				//���忪ʼ��Ϣ
//	static	void CALLBACK OnAttackVillageEndInfo(LPARAM lparam);				//���������Ϣ
//
//	bool	ApplyForVillageWar(const CGUID& PlayerGuid,long lWarID,long lMoney);			//����������ݵ�����ս
//	long 	IsApplyForWarByRegionID(long lRegionID);							//�õ��ó����Ƿ�������뱨��
//
//	bool	IsExitWarID(long lWarID);											//�ж��Ƿ�������WarID
//	eCityState GetRegionState(long lRegionID);									//�õ�һ�������ĵ�ǰ״̬
//	eCityState GetCityStateByWarNum(long lWarTime);								//�õ�ս����ʾ��ǰ״̬
//	long GetWarNumByRegionIDAndCurTime(long lRegionID);							//�õ���ǰʱ���һ�������Ĺ��Ǳ�ʾ
//	void GetDecWarFactions(long lRegionID,list<CGUID>& Factions);				//�õ�һ����������ս���
//	void GetDecWarFactionsByWarNum(long lWarNum,list<CGUID>& Factions);			//�õ�һ��ս������ս���
//	void SetVilWarState(long lTime,eCityState CityState);						//����ĳ��ս����״̬
//	bool IsAlreadyDeclarForWar(long lWarNum,const CGUID& FactionGuid);					//�ж�һ������Ƿ��ĳ��ս����ս
//	bool IsAlreadyDeclarForWar(const CGUID& FactionGuid);								//�ж�һ������Ƿ���ս
//	bool IsVilRegionLeftClearTime(long lRegionID);								//�ж���һ���壬���ҹ�������ʱ��
//
//	void OnFacWinVillage(long lWarNum,long lWarRegionID,const CGUID& FactionGuid,const CGUID& UnionGuid);	//��һ�����Ӯ���˴�ս
//	void OnVillageWarEnd(long lWarNum,long lWarRegionID);						//�ݵ�����ս����
//};
//
//CVillageWarSys* GetVilWarSys();
