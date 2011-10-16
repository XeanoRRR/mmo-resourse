///////////////////////////////////////////////////////////////////////////////////////
//AttackCitySys.h
//����սϵͳ
//Function:��ɹ���սϵͳ��ʱ����Ƶȵ�
//Author:Joe
//Create Time:2004.7.29
#pragma once
#include "..\public\Timer.h"
#include "..\ServerCityRegion.h"
#include "..\public\Date.h"


typedef struct tagAttackCityTime
	{
		long	lTime;					//�ڼ���
		long	lCityRegionID;			//��ս��ͼ
		ULONG	lDeclarWarEventID;
		tagTime DeclarWarTime;			//��սʱ��
		ULONG	lStartInfoEventID;
		tagTime	AttackCityStartInfoTime;//���ǿ�ʼ��ʾʱ��
		ULONG	lStartEventID;
		tagTime	AttackCityStartTime;	//���ǿ�ʼʱ��
		ULONG	lEndInfoEventID;
		tagTime	AttackCityEndInfoTime;	//���ǽ�����ʾʱ��
		ULONG	lEndEventID;
		tagTime	AttackCityEndTime;		//���ǽ���ʱ��
		ULONG	lMassEventID;
		tagTime	MassTime;				//����ʱ��
		ULONG	lClearEventID;
		tagTime	ClearPlayerTime;		//�����ս�����������ʱ��
		ULONG	lRefreshEventID;
		tagTime	RefreshRegionTime;		//ˢ�³���ʱ��

		eCityState	RegionState;		//���ս���е�ǰ����������״̬
		list<CGUID>	DecWarFactions;		//����γ�ս��ս�İ��
	}tagACT;


class CAttackCitySys
{
public:
	CAttackCitySys(void);
	~CAttackCitySys(void);

public:
	
	struct tagGetTodayTaxTime
	{
		ULONG lEventID;
		long lHour;
		long lMinute;
		long lSecond;
	};

private:
	static map<long,tagACT>		m_Attacks;
	typedef map<long,tagACT>::iterator itAttack;

	static  tagGetTodayTaxTime m_GetTodayTaxTime;
	static CAttackCitySys* instance;											//����Ψһʵ�����

public:
	static CAttackCitySys* getInstance();										//�õ�Ψһʵ�����	
	void   Release();															//�ͷ���Դ

	bool DecordFromByteArray(BYTE* pByte, long& pos);

	bool UpdateApplyWarFacs(BYTE* pByte, long& pos);

public:
	void OnAttackCityStart(long lWarID);							//���ǿ�ʼ
	void OnAttackCityTimeOut(long lWarID);							//����ʱ�䵽
	void OnAttackCityEnd(long lWarID);								//���ǽ���
	void OnDeclarWar(long lWarID);									//������ս
	void OnMass(long lWarID);										//���Ǽ���
	void OnClearOtherPlayer(long lWarID);							//���������Ա
	void OnRefreshRegion(long lWarID);								//ˢ�³���

	void	InitiCityRegionState();

	long GetWarRegionIDByTime(long lWarID);
	bool IsAlreadyDeclarForWar(long lWarID,const CGUID& FactionID);			//�ж�ĳ������Ƿ��Ѿ���ս
	bool GetWarNameForDeclar(const CGUID& FactionID,string &strName);			//�õ�����ս��������

	long GetCurTimeWarNumByRegionID(long lRegionID);				//�õ���ǰʱ���WarID 
	eCityState GetCityStateByWarNum(long lWarID);					//�õ�WarID��ǰ״̬

	long	GetWarStartTime(long lWarRegionID);
};
CAttackCitySys* GetAttackCitySys();
