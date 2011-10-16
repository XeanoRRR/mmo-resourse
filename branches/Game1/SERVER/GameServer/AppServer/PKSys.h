//////////////////////////////////////////////////////////////////////////
//PKsys.h
//Fun:PKϵͳ�����ֹ�����ɱ�ˡ���������
//Author:Joe
//Create Time:2005.6.24
//////////////////////////////////////////////////////////////////////////
#include "Region.h"

struct tagDiedLost;
class CPlayer;
class CServerRegion;

#pragma once

class CPKSys : public BaseMemObj
{
public:
	CPKSys(void);
	~CPKSys(void);
public:
	//�ж�һ��MoveShape�Ƿ���Թ�������һ��MoveShape
	bool	IsAttackAble(CMoveShape* pAttacker,CMoveShape* pBeAttacker);
	//�ж��Ƿ��ǵжԹ�ϵ
	bool	IsEnemy(CMoveShape* pShape1,CMoveShape* pShape2);

	void	OnFirstAttack(CPlayer* pAttackPlayer,CPlayer* pBeAttackedPlayer,
							CServerRegion* pRegion,CRegion::eSecurity SecurityType);					//���ֹ���

	void	OnFirstSkill(CPlayer* pAttackPlayer,CPlayer* pBeAttackedPlayer,
							CServerRegion* pRegion,CRegion::eSecurity SecurityType);					//���ּ���
	void	OnKill(CPlayer* pKillPlyaer,CPlayer* pBeKilledPlayer,CServerRegion* pRegion);				//ɱ������
	void	OnDeath(CPlayer* pPlayer,CServerRegion* pRegion);											//��������
	
	long	GetDiedLostExp(CPlayer* pBeKilledPlayer, CServerRegion* pRegion,
		CRegion::eSecurity SecurityType);										//�õ��������ľ���

	bool GetDiedLostGoods(CPlayer* pBeKilledPlayer,long lMurdererType,const CGUID& MurdererID,CServerRegion* pRegion,
						CRegion::eSecurity SecurityType,long lNameColor,tagDiedLost& DiedLost);//�õ������������Ʒ�ĸ���															//�õ������ı���
	//�������ͱ��������ĵ��ж�
	BOOL    JudgeAttackRelation(CMoveShape *pAttacker,CMoveShape *pBeenAttacker,int nAddSub);
	void    SetPKValue(CMoveShape *pAttacker,CMoveShape *pBeenAttacker,int nAddSub);
	void	GetCountryList(	long lCountyID,list<BYTE> &lCountryIDList);			//����ͬ��ID�õ������б�
	bool	IsSameCamp(CMoveShape *pAttacker,CMoveShape *pBeenAttacker);	//��Ӫ�ж�
private:
	bool	IsCityWarState(CPlayer* pPlayerA,CPlayer* pPlayerB);			//�ж���������Ƿ���ڳ�ս״̬
	bool	ISFactionWarState(CPlayer* pPlayerA,CPlayer* pPlayerB);			//�ж���������Ƿ���ڵж԰�ս״̬

	bool	IsCountryWarState(CPlayer* pPlayerA,CPlayer* pPlayerB);			//�ж���������Ƿ���ڹ�ս״̬
	

	

	VOID	ReportMurderer(CPlayer* pMurderer,CPlayer* pBMurderer);								//��������
private:
	static CPKSys* pInstance;
public:
	static CPKSys* getInstance();
	void	Release();														//�ͷ���Դ
};

inline CPKSys* GetPKSys()
{
	return CPKSys::getInstance();
}
