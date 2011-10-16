#pragma once
#include "baseai.h"

class CMoveShape;
class CPlayer;

class CMonsterAI : public CBaseAI
{
public:
	CMonsterAI();
	~CMonsterAI();

protected:
	//���ڵ��ú���
	virtual BOOL				OnCycle					();
	//������Ϊ��Ӧ
	virtual BOOL				OnActive				();


	//������Ϊ����Ĳ�ͬ״̬����Ϊ  ��ƽ��ս�������ܣ�����������
	virtual BOOL				OnPeaceState			();
	BOOL						OnPeaceStateOnColony	();
	virtual BOOL				OnFightState			();
	BOOL						OnFightStateOnColony	();
	virtual BOOL				OnEscapeState			();
	virtual BOOL				OnNoHpState				();
	virtual BOOL				OnDeadState				();
	virtual BOOL				OnTransitionState		();


	//��ƽ���ڵ���������
	virtual BOOL				OnPeaceIdle				();
	//�����غ�ƽ״̬
	virtual BOOL				CheckReturnPeace		();
	//�ʹ���ı�ai
	virtual BOOL				OnChangeAI				();



//�ж�
protected:
//Ѱ��
	//�����£�Ѱ�ҵ���
	virtual BOOL				SearchEnemy				();
	//�����������
	BOOL						SearchEnemyRandom		(int iDisMax,int iDisMin);
	//�Ƿ���ִ������
	BOOL						CanSearchEnemy			();
	//ִ����������
	virtual BOOL				CheckSearchEnemy		();
	//�ڼ�Ⱥ����������
	BOOL						SearchEnemyOnColony		();
	//�ڼ�Ⱥ��Ѱ�����������Ŀ��ĳ�Ա
	CMonster*					SearchFriendOnColony	();
	//�ýű�Ѱ�ҵ���
	virtual BOOL				SearchEnemyByScript		();
//�ܶ�
	//׷��
	enum{ TRACE_RESULT_SUCCESS=0,TRACE_RESULT_TRACE,TRACE_RESULT_OUT,};
	virtual INT					Tracing					(CMoveShape* pTarget);
	//����
	virtual BOOL				Escape					();
		    VOID				Escape					(CRegion* pRegion, LONG lX, LONG lY, BOOL bRun = FALSE );
			VOID				Escape					(CRegion* pRegion, LONG lDir, BOOL bRun = FALSE);

	//�жϸø����Ƿ����ƶ�
	BOOL						IsCanMoveTo			(CRegion* pRegion,LONG lX, LONG lY);
	//��Ŀ�귽���ƶ�
	virtual BOOL				MoveTo					(long lDir);
	//��·��
	virtual BOOL                RunByRoad				();
	//����߶�
	virtual BOOL				RandomRun				();
	BOOL						RandomRunOnColony		();
	//���״̬����
	BOOL						CheckRandomRunState		();
//ѡ����
	//Ѱ�Ҽ���
	virtual BOOL				SearchSkill				(CMoveShape* pTarget);
	BOOL						SearchOneTypeSkill		(DWORD dwSkillID,DWORD dwSkillLevel,DWORD dwType,LONG lDistance);
	//ʹ�ü���
	virtual BOOL				BeginSkill				(stModuParam* pSkill, LONG lx=0, LONG ly=0,CMoveShape* pTarget=NULL);

//�������ڴ���
	//BOOL						CheckCycleSpring		(SCycleSpringEvent& cycleenent);

//������ս�����ڴ�����ս�����������˴�������������
protected:
	virtual DWORD				CycleSpring				();
	virtual DWORD				BornSpring				();
	virtual DWORD				FightSpring				();
	virtual DWORD				GBHSpring				(int hurt);
	virtual DWORD				DeadSpring				();
	virtual DWORD				FindTargetSpring		();
	virtual DWORD				EscapeSpring			();
	DWORD						ScriptSpring			(CMonster* pMonster, const string& str);
//״̬�ĸı�
protected:
	virtual	VOID				ChangeStateTo			(INT state);
	virtual BOOL                ReturnPeace				();

//�ӿں���
public:
	//����������ʱ��
	virtual BOOL				WhenBeenHurted			(LONG lType,const CGUID& guid, DWORD a);
	//��ɱ����ʱ��
	virtual BOOL				WhenBeenKilled			(LONG lType,const CGUID& guid);
	//��ʧĿ��
	virtual	BOOL				WhenLoseTarget			();
	//����
	BOOL						OnReBorn				();

//·�����
protected:
	//����������ʱ��(�м�Ⱥ��ʱ��)
	BOOL						WhenBeenHurtedOnColony			(LONG lType,const CGUID& guid, DWORD);
	//VOID						CheckLevelRoad			();
	enum //˳·��Ѱ·�ķ�������
	{
		NEXTSTEP_RESULT_NULL=0,	//��һ��
		NEXTSTEP_RESULT_NEXT,	//��һ���ڵ㣬������β�ڵ�
		NEXTSTEP_RESULT_RETURN,	//����β�ڵ�ʱ��ת��
		NEXTSTEP_RESULT_NEXTCYCLE,//��·����β������ʱ�򣬵��ߵ�β��ʱ��ת���¸����ڵ�ͷ�ڵ�
	};
	//�������һ��Ҫ�ߵ�λ�ã��������ǽ����¸���¼�λ�����ͣ�����ǡ���
	virtual INT					CountNextStepByRoad		(LONG &lX, LONG &lY,INT &CurrentIndex);
	//�ɻص���ƽ��¼�㣬������·����
	VOID						FlyToRecordePoint		();	
	//
	BOOL						FlyToFriendOnColony		(CMoveShape* pMoveShape);

public:
	//��¼������
	VOID						RecordBornPoint			(int x, int y);
	//���·��
	virtual VOID				AddRoad					(vector<POINT>&);
	//��ȫ�ָ�������aiҲ��������ǰ��״̬
	// bForce : �Ƿ�ǿ��resume��Ϊ��ʱ����ԡ��ָ�����־
	virtual VOID				ResumeAll				( bool bForce = false );
	//��������
	BOOL						OnCheckDeadState		(){return OnDeadState();}
	//����ս��״̬
	void						EnterFight				();//{ChangeStateTo(CShape::STATE_FIGHT);}

	// �ж��Ƿ񻹴����Ѻ�ʱ���ڣ������Ѻ�ʱ��Ĺ��ﲻ���С�
	BOOL IsInKindlyTime();

//�ű��ӿ�
public:
	virtual VOID				SetTarget				( LONG lType, const CGUID& TargetGuid );
	VOID						SetForceTarget		( LONG lType, const CGUID& TargetGuid );
	VOID						CleanForceTarget	();
	//������Χ��ͬ�����
	BOOL				CallFriendAround		();
	//������Χ�����й���
	BOOL				CallMonsterAround		();
	//ע��ı�ai�¼�
	BOOL				AddChangeAIEvent		(LONG AIType);
	//ȡ��·��
	vector<POINT>&		GetRoad					(){ return m_vectRoad;}	
	//����·����¼
	bool				HasRoad					(){return m_vectRoad.size()!= 0;}

	// ����ʩ�ż��ܣ�ʧ�ܻ����һ��AI��ʱ��������AI����ִ��
	bool UseSkillSyncSafe( long skillType, long skillID, long skillLevel, const CGUID &targetID = NULL_GUID );
	// �����Ը���ʩ�ż��ܣ�ʧ�ܻ����һ��AI��ʱ��������AI����ִ��
	bool UseSkillOnCellSyncSafe( long skillType, long skillID, long skillLevel, long targetX = 0, long targetY = 0 );
	// ����ʩ�ż���
	bool UseSkillSync( long skillType, long skillID, long skillLevel, long targetX, long targetY, CMoveShape *pTarget = NULL );

//��¼ѡ���ܼ�¼����
protected:

	//��¼ѡ����ļ��ܣ� ����id�� ���ܵȼ��� ���ܼ�¼���ͣ� ��������
	VOID				RecordChoiceSkill(DWORD SkillID,DWORD SkillLevel,WORD SkillType,WORD SkillIndex);
	//��¼ʹ�õ�ǰ�ļ���
	VOID				RecordUsedCurrentSkill();
	//���ѡ����ļ���
	VOID				CleanChoiceSkillRecord();
	//�Ƿ��Ѿ�ѡ����˼���
	BOOL				HasChoiceSkill(){return m_dwChoiceSkillID!=0;}
	//
	WORD				GetChoiceSkillType(){return m_wSkillType;}
private:
	int mm;
	//ѡ����id
	DWORD m_dwChoiceSkillID;
	//ѡ���ܵȼ�
	DWORD m_dwChoiceSkillLevel;
	//ѡ��������
	WORD  m_wSkillType;
	//ѡ��������
	WORD  m_wSkillIndex;
	//�Ƿ���뱻ʹ��
	BOOL  m_bMustUsed;
	//�Ƿ�ʹ��
	BOOL  m_bBeenUsed;
protected:
	//���˱�ʶ
	//float                       m_fGBHStamp;				
	//����ʱ���ʶ
	DWORD                       m_dwGuardTimeStamp;			
	//��󱻹���ʱ���ʶ
	DWORD						m_dwHurtedTimeStamp;		
	//���ܼ��ʱ���־
	DWORD                       m_dwSkillSpaceTimeStamp;	
	//���ɽ׶ε�ʱ���־
	DWORD						m_dwTransitionTimeStamp;
	// ��������ʱʱ��
	DWORD m_dwRebornTime;

	//·�����
protected:
	//������
	POINT						m_BornPoint;				
	//·��
	vector<POINT>				m_vectRoad;					
	//���ߵ��¼
	INT                         m_iRoadRecord;				
	//���߷���
	INT                         m_iRoadDirect;             
	//�뿪·���ļ�¼��
	POINT                       m_LeaveRoadPoint;			
	//����ս��״̬��
	POINT						m_EnterFightPoing;			
	//�Ƿ�����������
	bool m_bAttackedByGuard;
public:
	bool IsAttackedByGuard() const
	{
		return m_bAttackedByGuard;
	}
public:
	friend class CMoveShape;
};
