#include <queue>
#include "..\..\..\..\Public\SkillRelated.h"
//#include "..\MoveShape.h"
#pragma once

class CRegion;
class CMoveShape;
class CPlayer;
class CGibe;
class CPet;
//class CSkill;
struct stModuParam;


const LONG  EXCEPTION_STATE_SPACE=500;
const LONG  POST_AREA=8;
const LONG  SKILL_FAIL_SPACE_TIME=1000;
const LONG  MOVE_FAIL_APACE_TIME=1000;
const LONG  STAND_ONCE_TIME=1000;
const LONG  SKILL_SPACE_TIME=500;

class CBaseModule;
class CBaseAI : public BaseMemObj
{
public:

	//AIִ��״̬
	enum AI_EXEC_STATE
	{
		AES_INVALID = -1, // ��ЧAI״̬������һЩ�ٽ�״̬���ɱ���ܶ��ٽ�BUG
		AES_FIGHT_NORMAL=0,							//ս������
		//AES_FIGHT_ATTACK,							//ս�����������Լ����ܣ�
		AES_FIGHT_ESCAPE,							//ս������
		AES_FIGHT_TRANSITION,						//ս�����ƽ����״̬
		AES_FIGHT_NOHP,								//ս��״̬����hp��ʱ��
	};

	//��ʱ�¼�����
	enum AI_EVENT_TYPE
	{
		AET_NULL=0,                         //�޶���
		AET_CYCLE,							//����
		AET_ACT,							//���������������Լ����ܵȣ�
		AET_MOVE,							//����,��վ����
		//AET_RELIVE,							//����
		AET_RETURNPEACE,					//�ָ���ƽ
		AET_DIED,							//�����¼�
		AET_CHANGEAI,						//�ı�ai
		AET_SKILL_CONTINUE,		// �ű�AI�����ܶ�����ϣ�����ִ�нű�
	};

	struct AI_EVENT							//��ʱ��¼�ṹ
	{
		AI_EVENT();
		VOID Clean();
		VOID operator=(AI_EVENT& ae);

		AI_EVENT_TYPE			aetEventType;		//��ʱ�¼�����
		LONG					lTimerID;			//ʱ��id
		DWORD					dwSpringTime;		//����ʱ��
	};



	//������ע��Ķ�������Ϊ��Ӱ����¼�����Ҫ�����������á�Ҳ�п��ܱ��ű�����
	enum CYCLE_SPRINGACT
	{
		CS_NULL=0,
		CS_MOVETO_CELL,
		CS_MOVEBY_STEP,
		CS_MOVEBY_TIME,
		CS_RANDOMMOVE,
		CS_STAND,
		CS_SKILL,
		CS_ESCAPE,
		CS_SENDEXPACT,
		CS_SENDEXPACTNEXT,
		CS_LEVELFIGHT,
		CS_SKILLEX, // ��չʹ�ü��ܣ����Զ�ָ��GUID����һ����ʹ�ü��ܣ�CS_SKILLֻ���ԶԵ�ǰĿ��ʹ�ü���
		CS_SKILLCELL, // ��ĳ������ʹ�ü���
	};

	struct SCycleSpringEvent			//���ڴ����ṹ���ֱ����ö��CYCLE_SPRINGACT �еĲ�ͬ����
	{
		SCycleSpringEvent();
		VOID Clean();

		CYCLE_SPRINGACT csSpringType;	//���ڴ��������ͣ�����5�ࣩ
		union 
		{
			struct USESKILL
			{
				LONG lSkillID;			//����id
				LONG lSkillLevel;		//���ܵȼ�
				LONG lPos;		//ʩ��Ŀ�����(x|y)
			}tgUseSkill;

			struct  MOVEBYSTEP    //����by����
			{
				LONG lDirect;
				LONG lStepNum;	
			} tgMoveByStep;

			struct MOVEBYTIME		//����byʱ��
			{
				LONG lDirect;
				LONG lStopTime;
			}tgMoveByTime;

			struct MOVETOCELL		//���ߵ�ĳ��
			{
				LONG lX;
				LONG lY; 
			} tgMoveToCell;

			LONG lRandomMoveTime;	//���ߵ�ʱ��

			struct STAND //վ��
			{
				LONG lStandTimes;
				LONG lStandTime;
			}tgStand;
					
			struct SENDEXPACTION  //���Ŷ���
			{
				LONG lActionID;
				LONG lActTime;
				LONG lAimType;
				
			}tgSendExpAct;

			struct LEVELFIGHT
			{
				LONG lTime;
				LONG AimX;
				LONG AimY;
			}tgLevelFight;

		} unActType;				//������Ϊ
		CGUID gAimGUID;

	};

public:
	CBaseAI					();
	virtual	~CBaseAI		();
	//CBaseAI					(CBaseAI& AI);
	//VOID operator=			(CBaseAI& AI);
	
//��ʱ����װ��������
protected:
	//���һ��ai�¼�
	BOOL						AddAIEvent				(AI_EVENT_TYPE iOutTimeType ,DWORD dwOutTime=0);
	//ɾ��һ��AI�¼�
	BOOL						DeleteAIEvent			(AI_EVENT_TYPE iOutTimerType);
	//�ڶ�ʱ����ע��һ���¼�
	BOOL						LogoutEvent				(AI_EVENT& aeAiEvent);


//��Ϊ���
public:
	//�¼������ĵ��ýӿ�
	virtual BOOL				Run						(DWORD dwTimerID=0,int iOutTimeType=0);
protected:
	////���ڵ��ú���
	virtual BOOL				OnCycle					();
	////������Ϊ��Ӧ
	virtual BOOL				OnActive				();

	////������Ϊ����Ĳ�ͬ״̬����Ϊ  ��ƽ��ս�������ܣ�����������
	virtual BOOL				OnPeaceState			();
	virtual BOOL				OnFightState			();
	virtual BOOL				OnEscapeState			();
	virtual BOOL				OnNoHpState				();
	virtual BOOL				OnDeadState				();
	virtual BOOL				OnTransitionState		();

//������ʱ����Ϣ�ͼ��ⲿ���ú���
public:
	// ����״̬�������������������
	virtual void Reset();

	//����������Ϊ
	VOID						StartActiveAct			();
	//ע����ǰ��������Ϊ
	BOOL						LogoutCurrentEvent		(){return LogoutEvent(m_aeActive);}
	//��Ӹı�ai�¼�
	VOID						StartDiedEvent			(DWORD dwTime){AddAIEvent(AET_DIED,dwTime);}
	//����ս��
	virtual void				EnterFight				(){};
public:
	//�ı�ai�¼�
	virtual BOOL				OnChangeAI				();
	//�������
	VOID						OnBoring				();
	//��������
	//BOOL						OnCheckDeadState		(){return OnDeadState();}		

	//ת��Ϊ��ƽ״̬
	virtual BOOL				OnReturnPeace			();
	//�����ܽ�����ʱ�򣬣����ܽ�����ʱ����ã�
	virtual BOOL				OnSkillEnd				(long lAddSub);
	//
	//virtual BOOL				CheckReturnPeace		(){return FALSE;}

//������Ϊ
public:
	//��һ��ָ�������ƶ�
	virtual BOOL				MoveTo					( float fDestX, float fDestY);
	//ȡ������
	BOOL						GetNearPosition				(LONG &x, LONG &y, LONG dir=-1,LONG searchDir=-1);
protected:
	//��һ���������ƶ�
	virtual BOOL				MoveTo					( long lDir) {return TRUE;}

	//Ѱ�Ҽ���
	virtual BOOL				SearchSkill				(CMoveShape* pTarget);
	//ʹ�ü���
	virtual BOOL				BeginSkill				(stModuParam *pModuParam, LONG lx=0, LONG ly=0,CMoveShape* pTarget=NULL);

	//��·������
	virtual BOOL                RunByRoad				();

	//�����߶�
	virtual BOOL				RandomRun				();
	//��ƽ���ڵ���������
	virtual BOOL				OnPeaceIdle				();
	//�������ڴ���
	BOOL						CheckCycleSpring		(SCycleSpringEvent& cycleevent);


//ִ�еļ���ִ���ƶ�
protected:
	//�жϸø����Ƿ����ƶ�
	virtual BOOL				IsCanMoveTo			(CRegion* pRegion,LONG lX, LONG lY);
	//�ͼ���ܷ��ƶ���ָ���ĸ���
	virtual BOOL				MoveToCell			(LONG lX, LONG lY);
	virtual BOOL				MoveToCell			(LONG lDir,LONG lCurrentX,LONG lCurrentY);
	//�õ���һ���ƶ���Ŀ���
	BOOL GetNextMoveDestCell(long lDir,long lMaxDistance,long &lX,long &lY);
	BOOL GetNextMoveDestCell(long lDestX,long lDestY,long lMaxDistance,long &lX,long &lY);
	//ִ���ƶ�
	virtual BOOL				OnMove					(LONG lX, LONG lY,LONG actTime);
	//ɾ�����ڴ���
	bool						CleanCycleSpring		();


//�ı�״̬
protected:
	virtual	VOID				ChangeStateTo			(INT state);
	//�ص���ƽ״̬
	virtual BOOL                ReturnPeace				();


//�ű�������ս�����ڴ�����ս�����������˴�������������
public:
	virtual DWORD				CycleSpring				();
	virtual DWORD				BornSpring				();
	virtual DWORD				FightSpring				();
	virtual DWORD				GBHSpring				(int hurt);
	virtual DWORD				DeadSpring				();
	virtual DWORD				FindTargetSpring		();
	virtual DWORD				EscapeSpring			();

//�������Ե�����
public:
	//����������
	CMoveShape*					GetOwner				();
	VOID						SetOwner				(CMoveShape* pOwner);
	//AI����
	LONG						GetAIType				();
	VOID						SetAIType				(LONG lAIType);
	//ȡ�õ�ǰս��״̬�еľ���״̬
	AI_EXEC_STATE				GetCurrentActiveAction	(){return m_aesAIState;}
	//�Ƿ�����ʹ�ü���
	BOOL						IsInAct					();
//Ŀ�������
public:
	//Ŀ�꣺ ���ã�ѯ�ʣ���ȡ���ͣ���ȡid����ȡָ��
	virtual VOID				SetForceTarget			( LONG lType, const CGUID& TargetGuid );
	virtual VOID				CleanForceTarget		();
	virtual VOID				SetTarget				( LONG lType, const CGUID& TargetGuid );
	BOOL				        HasTarget				();
	LONG						GetTargetType			();
	const CGUID&				GetTargetID				();
	CMoveShape*					GetTarget				();
	//��ʧĿ��
	virtual	BOOL				WhenLoseTarget			();
protected:
	BOOL						LoseTarget              ();
	//virtual BOOL				ForceTarget				(CMoveShape * pTarget , DWORD dwForceTime);
//���ߣ����ߣ����ѣ���ȡ����ʱ�䣬��ѯ�Ƿ���
public:
	virtual VOID				WakeUp					();
	BOOL						IsHibernated			();
protected:	
	virtual VOID				Hibernate				();
	DWORD						GetDormancyInterval		();


//�ⲿ�ӿ�
public:
	//����������ʱ��
	virtual BOOL				WhenBeenHurted			(LONG lType,const CGUID& guid,DWORD a);
	//��ɱ����ʱ��
	virtual BOOL				WhenBeenKilled			(LONG lType,const CGUID& guid);
	//����
	virtual void				Attack					( long tID, LONG lX=0, LONG lY=0, CMoveShape* pTarget=NULL );

//��������
public:
	//�������ڴ���
	BOOL						AddCycleSpring			(CYCLE_SPRINGACT type, LONG m_1, LONG m_2=0, LONG m_3=0,CGUID guid= NULL_GUID);
	enum{ RIGHT=-1, ERROR_MOVING, ERROR_SKILLUSING, ERROR_FIGHTSTATE};
	//�ܷ�ʹ�ü���
	virtual	INT					CanUseSkill				(BOOL bRunUse){return 0;}
	//##���AI�����������¼��Ľӿ�
	virtual VOID				Clear					();


	//��ӿ���ʱ��Ľű���
	virtual VOID                AddIdleActScript		(string & strScriptName);


//�ű��ӿ�
public:
	//ʹ��һ������
	BOOL				UseSkillByID			(LONG skillid,LONG skilllevel, const CGUID &targetID = NULL_GUID);
	//��ָ�������ͷż��ܣ�ȱʡ�Ե�ǰĿ�����������ͷ�
	BOOL				UseSkillByIDOnCell		(LONG skillID, LONG skillLevel, LONG posX=0, LONG posY=0);
	//�ں�ƽ״̬���ƶ���ĳ��
	BOOL				MoveToCellOnPeace		(LONG x,LONG y);
	//��ս��״̬���ƶ���ĳ��
	BOOL				MoveToCellOnFight		(LONG x,LONG y);
	//�ƶ���ĳ��ĸ���������Ѿ����ⷶΧ�ڣ���һ�������û�ڣ����ƶ�������ϡ�
	BOOL				MoveToCellAround		(LONG x,LONG y);
	//�ں�ƽ״̬����ĳһ�����ƶ�����
	BOOL				MoveByStepOnPeace		(LONG dir, LONG step);
	//��ս��״̬����ĳһ�����ƶ�����
	BOOL				MoveByStepOnFight		(LONG dir, LONG step);

	//�ں�ƽ״̬�²���һ������
	BOOL				SendExpActOnPeace		(LONG lActID,LONG lAimTime,LONG lAimType=0,CGUID guid=NULL_GUID );
	//��ս��״̬�²���һ������
	BOOL				SendExpActOnFight		(LONG lActID,LONG lAimTime,LONG lAimType=0,CGUID guid=NULL_GUID );

	//�ں�ƽ״̬����ĳ�����ƶ�����ʱ��
	BOOL				MoveByTimeOnPeace		(LONG dir, LONG time);
	//��ս��״̬����ĳ�����ƶ�����ʱ��
	BOOL				MovebyTimeOnFight		(LONG dir, LONG time);
	//��ս��״̬��ʹ֮�����߶�
	BOOL				RandomMoveByTime		(LONG time);
	//վ��һ��ʱ��	��������ÿ��ͣ����ʱ�䣩
	BOOL				Stand					(LONG times,LONG time);
	//��վ��һ��ʱ�� 
	VOID				Stand					(LONG time);
	//����һ��ʱ��(��ƽ״̬�£�  ֻ��Ҫģ�����ܣ���Ϊ����������)
	BOOL				EscapeByTimeOnPeace		(LONG dir,LONG time);
	//����һ��ʱ�䣨ս��״̬�£�
	BOOL				EscapeByTimeOnFight		(LONG time);
	//����ս��״̬����
	BOOL				LevelFight				(LONG time,LONG aimX,LONG aimY);
	//ȡ��ע�����Ϊ
	BOOL				DeleteSpring			();
	//�ر�����״̬
	BOOL				EndEscape				();
	//��ʼ������Ϊ
	VOID				StartCycle				(DWORD dwStatrTime, DWORD dwCycleTime);
	//����������Ϊ
	VOID				EndCycle				();

protected:
	LONG						m_lAIType;					//##AI���ͱ��
	DWORD						m_dwLastExecTimeStamp;		//##AI���һ��ִ�е�ʱ��

	CGUID						m_TargetGuid;				//##��������ID
	LONG						m_lTargetType;				//##������������

	LONG						m_lTargetX;					//##Ŀ��X
	LONG						m_lTargetY;					//##Ŀ��Y

	CMoveShape*					m_pOwner;					//##AI���� ���� OR ���
	//
	SCycleSpringEvent			m_csCycleSpring;			//���ڴ����¼� 
	string						m_strIdleActScript;			//������Ϊ�ű�
	AI_EXEC_STATE               m_aesAIState;               //AI״̬
	AI_EVENT                    m_aeActive;					//������Ϊ��¼
	AI_EVENT                    m_aeCycle;					//������Ϊ��¼
	AI_EVENT					m_aeScriptAISkill;			//�ű�AI����
protected:
	DWORD						m_dwDormancyTime;			// ���߿�ʼʱ��
	DWORD						m_dwDormancyInterval;		// ����ʱ����
//

//�������Եı���
	
	int checknum;

public:
	friend class CMoveShape;
	friend class CPlayer;
	friend class CGibe;
	friend class CPet;
};