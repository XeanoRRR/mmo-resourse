#pragma once
#include "MonsterAI.h"

class CMoveShape;
class CPlayer;
class CMonsterTaming;


class CPet :
	public CMonsterAI
{
public:

	enum PET_SEARCH_ENEMY_MODE
	{
		PSEM_ACTIVE_ATTACK,				//##��������
		PSEM_PASSIVELY_ATTACK,			//##��������
		PSEM_DEFENSE,					//##����
	};

	enum PET_ACTION
	{
		PA_ATTACKING,
		PA_FALLOWING,
		PA_STAYING,
		PA_DISMISSING,
	};

	CPet											();
	~CPet											();

//	virtual AI_EXEC_STATE		Run					();
//	virtual VOID				WhenBeenHurted		( LONG lType, LONG lID, DWORD dwStiffenTime = 0 );
	virtual VOID				OnIdle				();

	//##����/��ȡ����ĵ�ǰAIģʽ
	VOID						SetPetCurrentAIMode	( PET_SEARCH_ENEMY_MODE psem );
	PET_SEARCH_ENEMY_MODE		GetPetCurrentAIMode	();

	VOID						SetPetCurrentAction	( PET_ACTION pa );
	PET_ACTION					GetPetCurrentAction	();

	//##��ȡ���������
	CMoveShape*					GetPetMaster		();

protected:
	virtual VOID				OnSchedule			();
	virtual BOOL				OnSearchEnemy		();
	virtual BOOL				OnMoving			();

	virtual BOOL				OnLoseTarget		();
	virtual VOID				SetTarget			( LONG lType, const CGUID& TargetGuid );


	//##���������������棬ͣ��
	VOID						OnAttackingSchedule	();
	VOID						OnFallowingSchedule	();
	VOID						OnStayingSchedule	();

private:
	PET_SEARCH_ENEMY_MODE		m_psemPetAIMode;
	PET_ACTION					m_paPetAction;

	//##���ڴ���ģʽ��ʱ�򣬼�¼�����Ƿ���Ч��ʱ���
	DWORD						m_dwInvalidMasterTimeStamp;
	DWORD						m_dwSeekMasterTimeStamp;
	BOOL						m_bMasterLogout;

	//##24Сʱ����ʧ��ʱ
	DWORD						m_dwPetLifeCycleTimeStamp;
	DWORD						m_dwPetLifeCycleCounter;

	friend class CPlayer;
	friend class CMonsterTaming;
};
