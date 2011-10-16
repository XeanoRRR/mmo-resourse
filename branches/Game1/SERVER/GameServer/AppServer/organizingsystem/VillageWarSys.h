#pragma once

#include "..\ServerRegion.h"
#include "..\public\Date.h"

///////////////////////////////////////////////////////////////////////////////////////
//VillageWarSys.h
//��սϵͳ
//Function:��ɰ�սϵͳ����ع���
//Author:Joe
//Create Time:2005.7.13
//////////////////////////////////////////////////////////////////////////


struct tagVilWarSetup
{
	long	lID;				//��ʾID
	long	lWarRegionID;		//ս���ĵ�ͼ
	ULONG	lDeclarWarEventID;
	tagTime DeclarWarTime;		//��սʱ��
	ULONG	lStartInfoEventID;
	tagTime WarStartInfoTime;	//��ս��ʼ��Ϣʱ��
	ULONG	lStartEventID;
	tagTime WarStartTime;		//��ս��ʼʱ��
	ULONG	lEndInfoEventID;
	tagTime WarEndInfoTime;		//��ս������Ϣʱ��
	ULONG	lEndEventID;
	tagTime WarEndTime;			//��ս����ʱ��
	ULONG	lClearEventID;
	tagTime	ClearPlayerTime;	//����ݵ��ͼ�����ʱ��
	long	lVilRegionID;		//�����ͼ
	
	eCityState	RegionState;	//���ս���е�ǰ����������״̬
	list<CGUID>	DecWarFactions;	//�����İ��
};
class CVillageWarSys
{
public:
	CVillageWarSys(void);
	~CVillageWarSys(void);

private:
	static CVillageWarSys* instance;											//����Ψһʵ�����


	map<long,tagVilWarSetup>	m_VillageWars;									//��ս��	
	typedef map<long,tagVilWarSetup>::iterator itVelWar;

	bool	FindInList(list<long>& TemptList,long lID);							//list���Һ���
public:
	static CVillageWarSys* getInstance();										//�õ�Ψһʵ�����
	void   Release();	

	bool DecordFromByteArray(BYTE* pByte, long& pos);
	bool UpdateApplyWarFacs(BYTE* pByte, long& pos);

public:
	void OnDelcareWar(long lWarID);									//��ս��ʼ
	void OnAttackVillageStart(long lWarID);							//���忪ʼ
	void OnAttackVillageOutTime(long lWarID);						//����ʱ�䵽
	void OnAttackVillageEnd(long lWarID);							//�������
	void OnClearPlayer(long lWarID);								//����ݵ����

	void	InitiVillRegionState();

	long	GetWarRegionIDByTime(long lTime);
	long	GetVilRegionIDByTime(long lTime);
	bool	IsAlreadyDeclarForWar(long lWarID,const CGUID& FactionID);			//�ж�ĳ������Ƿ��Ѿ���ս
	bool	GetWarNameForDeclar(const CGUID& FactionID,string &strName);		//�õ�����ս��������

	long	GetWarStartTime(long lWarRegionID);
};

CVillageWarSys* GetVilWarSys();
