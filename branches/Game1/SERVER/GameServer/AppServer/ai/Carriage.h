#pragma once

#ifndef _Carriage_h_
#define _Carriage_h_
#include "MonsterAI.h"

class CMoveShape;
class CPlayer;
/*
class CCarriage :
	public CMonsterAI
{
public:
	
	enum CAR_ACTION
	{
		FALLOWING,		//����
		STAYING,		//ͣ��
	};

	CCarriage											();
	~CCarriage											();

	virtual AI_EXEC_STATE		Run					();
	virtual VOID				OnIdle				();

	//##����/��ȡ����ĵ�ǰAIģʽ
	VOID						SetCurrentAction	( CAR_ACTION ca );
	CAR_ACTION					GetCurrentAction	();

	//##��ȡ�ڳ�������
	CMoveShape*					GetMaster			();

protected:
	virtual VOID				OnSchedule			();
	virtual BOOL				OnMoving			();


	//##���������棬ͣ��
	VOID						OnFallowingSchedule	();
	VOID						OnStayingSchedule	();

private:
	CAR_ACTION					m_caAction;

	//##���ڴ���ģʽ��ʱ�򣬼�¼�����Ƿ���Ч��ʱ���
	DWORD						m_dwInvalidMasterTimeStamp;
	DWORD						m_dwSeekMasterTimeStamp;
	BOOL						m_bMasterLogout;

	//##24Сʱ����ʧ��ʱ
	//DWORD						m_dwPetLifeCycleTimeStamp;
	//DWORD						m_dwPetLifeCycleCounter;
	friend class CPlayer;
};
*/
const LONG CARRIAGE_STATE_DISTANCE=3;
const LONG CARRIAGE_STAY_TIME=500;

class CCarriage:
	public CMonsterAI
{
public:
	enum CAR_ACTION
	{
		NOMONSTER=0,	//������ͣ��״̬
		FALLOWING,		//����
		STAYING,		//ͣ��
	};

public:
	CCarriage											();
	~CCarriage											();
	BOOL WhenBeenHurted();
	CMoveShape*	GetMaster();
	VOID						SetCurrentAction	( CAR_ACTION ca ){m_caFollowState=ca;}
	CAR_ACTION					GetCurrentAction	(){return m_caFollowState;}
protected:
	VOID ChangeStateTo(AI_EXEC_STATE aes);
	BOOL SearchEnemy();
	BOOL RandomRun();
	//INT Tracing(CMoveShape* pTarget);
protected:
	//##���ڴ���ģʽ��ʱ�򣬼�¼�����Ƿ���Ч��ʱ���
	DWORD						m_dwInvalidMasterTimeStamp;
	DWORD						m_dwSeekMasterTimeStamp;
	BOOL						m_bMasterLogout;

	//##24Сʱ����ʧ��ʱ
	DWORD						m_dwPetLifeCycleTimeStamp;
	DWORD						m_dwPetLifeCycleCounter;

	CAR_ACTION					m_caFollowState;


	friend class CPlayer;
};

#endif