//////////////////////////////////////////////////////////////////////////
//FactionWarSys.h
//Function:��սϵͳ��ع���
//Author:Joe
//Create Time:2005.6.27
//////////////////////////////////////////////////////////////////////////
struct tagTime;
class COrganizing;
#include <set>
#include "../public/GUID.h"
using namespace std;

#pragma once
//struct tagEnemyFaction
//{
//	CGUID	FactionGuid1;		//�ж԰��׷�
//	CGUID	FactionGuid2;		//�ж԰���ҷ�
//	DWORD	dwDisandTime;		//�ж�״̬��ɢ����ʱ(����)
//
//	tagEnemyFaction()
//		:FactionGuid1(NULL_GUID)
//		,FactionGuid2(NULL_GUID)
//		,dwDisandTime(0)
//	{
//	}
//
//	tagEnemyFaction(const CGUID& guid1,const CGUID& guid2,DWORD dwTime)
//		:FactionGuid1(guid1)
//		,FactionGuid2(guid2)
//		,dwDisandTime(dwTime)
//	{
//	}
//
//};
//
////��ս���
//struct tagWarType
//{
//	long	lType;		//���
//	long	lFightTime;	//ս��ʱ��
//	long	lMoney;		//��ս��Ҫ�Ľ�Ǯ
//};
//
//class CFactionWarSys
//{
//public:
//	CFactionWarSys(void);
//	~CFactionWarSys(void);
//private:
//	static	CFactionWarSys* pInstance;
//public:
//	static	CFactionWarSys* GetInstance();
//	void	Release();
//private:
//	map<long,tagWarType>	m_FactionWars;		//��ս������
//	list<tagEnemyFaction>	EnemyFactionList;	//�ж԰���б�
//	DWORD					m_dwStartTime;		//��ʼͳ��ʱ��
//
//public:
//	bool   Initialize();																		//��ʼ��
//	bool	LoadIni();
//	void	Clear()		{EnemyFactionList.clear();}
//
//	void	Run();																				//��սϵͳ����
//
//	bool DigUpTheHatchet(const CGUID& DigGuid,const CGUID& BeDigedFactinID,long lLvl,tagTime& Time);			//��ս
//	void StopFactionWar(const CGUID& FactionGuid1,const CGUID& FactionGuid2);										//��ֹ��ս
//
//	void OnPlayerDied(const CGUID& PlayerID, const CGUID& Killer);								//�������ʱ����
//	void OnTimeOut(const CGUID& FactionGuid1,const CGUID& FactionGuid2);											//��սʱ���ѵ�����
//
//	bool IsEnemyRelation(const CGUID& FactionGuid1,const CGUID& FactionGuid2);					//�ж���������Ƿ��ǵжԹ�ϵ
//	long GetDecWarMoneyByType(long lType);														//�õ���սĳ��������Ҫ�Ľ�Ǯ
//
//	void GetEnemyFactionsByFactionID(const CGUID& FactionGuid,set<COrganizing*>& FactionList,long& DisTime);//�õ�һ�����ĵж԰���б�
//
//	void AddOneEnmeyFaction(const CGUID& FactionGuid1,const CGUID& FactionGuid2,DWORD dwLeaveTime);
//	void ClearEnemyFaction(const CGUID& FactionGuid1,const CGUID& FactionGuid2);								//�����������ĵжԹ�ϵ
//
//	void GenerateSaveData();
//};
//
//CFactionWarSys* GetFactionWarSys();
