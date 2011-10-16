//**************************************************
//		�ļ��� ��Colony.h
//		����   ��heliangchen
//		ʱ��   ��08/4/14
//		���   ��monster��Ⱥ������	��ServerRegion����
//				��AIһ�� ʵ�ֹ���Ⱥ����Ϊ
//***************************************************

#pragma once

#include "BaseObject.h"

const int COLONY_LEADER_DISTANCE_MAX=  3;
const int HURT_KEEP_TIME= 5000;
const int TRACING_DISTANCE= 15;

class CColony:public CBaseObject
{
public:
	CColony();
	~CColony();
public:
	//��ƽ״̬
	void OnPeaceState(CMoveShape* pMonster);
	//ս��״̬
	void OnFightState(CMoveShape* pMonster);
	//����ɱ����ʱ��
	void WhenBeenKilled(CMoveShape* pMonster);
	//��������ʱ��
	void WhenBorn(CMoveShape* pMonster);
	//����ս��״̬ת��Ϊ��ƽ״̬
	BOOL CheckReturnPeace();
	//���ɾ����Ա
	BOOL AddMember(BOOL bIsLeader, CMonster* pMonster);
	BOOL DelMember(CMonster* pMonster);
	//���Ŀ��
	//void AddTarget(CMonster* pMonster);
	//void AddTarget(int iType, const CGUID& guid);
	//����id
	void SetID(DWORD ID){m_ID= ID;}
	DWORD GetID(){return m_ID;}
	//�Ƿ���Ŀ��
	BOOL HasTarget(){return m_mapTargetList.size()!=0;}
	//�Ƿ���·��
	BOOL HasRoad();
	//��Ա����
	//BOOL MenberDied(CMoveShape* pMoveshape);
	//map<CGUID,OBJECT_TYPE>& GetTargetList(){return m_mapTargetList;}
	//ȡ��ʵ������Ŀ��
	CMoveShape* GetLeaderTarget();
	//ȡ��ʵ������
	CMonster* GetLeader();
	//ȡ��Ŀ���б�
	BOOL GetTargetList(vector<CMoveShape*>& vecTarget);
	//ȡ�ó�Ա�б�
	BOOL GetMemberList(vector<CMonster*>& vecMember);
	//ȡ��ս����Ա�б�
	BOOL GetFightMemberList(vector<CMonster*>& vecMember);
	//ȡ��Ŀ������
	POINT GetAimPoint(const CGUID& guid);
	//ȡ���쵼������
	POINT GetLeaderPoint();
	//�Ƿ���ʵ���쵼
	BOOL HaveFactLeader(){return m_guidFactLeader!= NULL_GUID;}
protected:
	//��������Ŀ��
	void SearchTarget();
	//void LoseAllTarget();
	void CountLeaderAim();
	void CountMemberAim();
	void CountOneMenberAim(int& AimX,int &AimY, int OppoX,int OppoY,int DirDis);
	enum //˳·��Ѱ·�ķ�������
	{
		NEXTSTEP_RESULT_NULL=0,	//��һ��
		NEXTSTEP_RESULT_NEXT,	//��һ���ڵ㣬������β�ڵ�
		NEXTSTEP_RESULT_RETURN,	//����β�ڵ�ʱ��ת��
		NEXTSTEP_RESULT_NEXTCYCLE,//��·����β������ʱ�򣬵��ߵ�β��ʱ��ת���¸����ڵ�ͷ�ڵ�
	};
	int RunByRoad(LONG &lX, LONG &lY,INT & CurrentIndex);
	void SetDirPos(int x,int y, int dir);
	void MoveTo(int x, int y);
protected:
	LONG m_ID;
	//����
	LONG m_iX;
	LONG m_iY;
	//����
	LONG m_iDir;
	//��ʼ����
	int m_iInitializeDir;

	struct  tgMember
	{
		tgMember()
		{
			MemType= 0;
			MemGuid= NULL_GUID;
			AimX= 0;
			AimY= 0;
			OppoX= 0;
			OppoY= 0;
		}
		void Clean()
		{
			MemType= 0;
			MemGuid= NULL_GUID;
			AimX= 0;
			AimY= 0;
			OppoX= 0;
			OppoY= 0;
		}
		//��Ա����
		int		MemType;
		//��Աid
		CGUID	MemGuid;
		//��ԱĿ������
		int		AimX;
		int		AimY;
		//��Ա��ԣ����죩����
		int		OppoX;
		int		OppoY;
	};
	//Ŀ����Ϣ
	struct STargetInfo
	{
		STargetInfo(){type=(OBJECT_TYPE)0;}
		OBJECT_TYPE type;
	};
	//�쵼
	tgMember m_tgLeader;
	//��Ա�б�
	map<CGUID,tgMember> m_mapMember;
	//ս����Ա�б�
	map<CGUID,OBJECT_TYPE> m_mapFightMember;
	//�߼��쵼
	CGUID m_guidFactLeader;
	//Ŀ���б�
	map<CGUID,STargetInfo> m_mapTargetList;
	//·��
	vector<POINT> m_vectRoad;
	//·������
	int m_iRoadDirect;
	//���ߵ��¼
	int m_iRoadRecord;

};