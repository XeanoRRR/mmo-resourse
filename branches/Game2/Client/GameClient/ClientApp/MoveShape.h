#pragma once
#include "Shape.h"
#include "../../Net/Message.h"
#include "../ClientApp/Other/FindPath.h"

using namespace std;
class CEffect;
class CSkillXml;

struct tagFaceAction;

class CMoveShape :public CShape
{
public:
	CMoveShape(void);
	virtual ~CMoveShape(void);

	// ��ʼ����������ֵ��Ӱ��
	virtual void InitNameValueMap();

	virtual void SetAction(WORD l);			//�趨����
	virtual void SetState(WORD l);

	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	//��ѹbuff
	bool DecordBuffFromByteArray(BYTE* pByte, long& pos);
	//��ѹdebuff
	bool DecordDeBuffFromByteArray(BYTE* pByte, long& pos);
	//��ѹonoffskill
	bool DecordOnOffSkillFromByteArray(BYTE* pByte, long& pos);
	//��ѹ����ʹ�ü���
	bool DecordActSkillFromByteArray(BYTE* pByte, long& pos);


	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData = true);	// ����
	//��ѹbuff
	bool DecodeBuffFromDataBlock(DBReadSet& setReadDB);
	//��ѹdebuff
	bool DecodeDeBuffFromDataBlock(DBReadSet& setReadDB);
	//��ѹonoffskill
	bool DecodeOnOffSkillFromDataBlock(DBReadSet& setReadDB);
	//��ѹ����ʹ�ü���
	bool DecodeActSkillFromDataBlock(DBReadSet& setReadDB);

//////////////////////////////////////////////////////////////////////////
//	����Ч����ض���
//////////////////////////////////////////////////////////////////////////
protected:
	// ����Ч������
	enum eADDEFFECT
	{
		ADDEFFECT_FLASH = 0,	//��ɫ����
		ADDEFFECT_SHAKE			//��ɫ��
	};

	// ����������ɵĸ���Ч���ṹ��
	struct tagAddEffect
	{
		bool  bPlayState;		//����Ч���Ĳ���״̬(Ϊ����ʼ����,Ϊ���򲻲��Ÿ�Ч��)
		DWORD dwStartTime;		//Ч����ʼʱ��
		DWORD dwTimeLength;		//Ч������ʱ��
		DWORD dwValue;			//Ч����ֵ(��ͬ��Ч�����ͻ��в�ͬ������)
	};

	// ���������
	float m_fXIncrement;		//��ǰģ����X����
	float m_fYIncrement;		//��ǰģ����Y����
	float m_fShakeMax;			//��ǰ��λ�Ƶ�������
	float m_fShakeMin;			//��ǰ��λ�Ƶ���С����
	bool  m_bRestoration;		//�𶯸�λ(Ϊ��ʱ��ʾ,��Ҫ��λ��ģ��ԭλ��,��֮����Ҫ��λ)

	tagAddEffect m_listAddEffect[2];		// ָ������0Ϊ"��ɫ����",����1Ϊ"��ɫ��"Ч��

	// Ѱ·
	CFindPath	m_FindPath;

public:
	void SetupFlashEffect(DWORD dwLum, DWORD dwTimeLength);				//�趨��ɫ���⸽��Ч��
	void SetupShakeEffect(DWORD dwExtent, DWORD dwTimeLength);			//�趨��ɫ�𶯸���Ч��
	float GetXIncrement()			{ return m_fXIncrement; }
	float GetYIncrement()			{ return m_fYIncrement; }

    /// ��λ
private:
    CGUID       m_PhaseID;

public:
    void    SetPhaseID(const CGUID& ID)  { m_PhaseID = ID ; }
    const   CGUID&  GetPhaseID()const { return  m_PhaseID ; }

//////////////////////////////////////////////////////////////////////////
//	�쳣״̬
//////////////////////////////////////////////////////////////////////////
public:
	// �쳣״̬
	struct tagStateEx
	{
		DWORD	dwID;					//	״̬���
		DWORD	dwLevel;				//	����״̬�ȼ�
		DWORD	dwAdditionalData;		//	�������� (ÿ�����ܵ���ʹ��)
		DWORD	dwStartTime;			//	��ʼʱ��
		DWORD	dwStandingTime;			//	����ʱ�� (0����һֱ����)
		bool	bEndSelf;				//	�����Լ�
		long	lCount;					//	�����������ʹ��ͬһ״̬ʱʹ��

		tagStateEx()
		{
			dwID = 0;
			dwLevel = 0;
			dwStartTime = 0;
			dwStandingTime = 0;
			bEndSelf = false;
			lCount = 0;
		}
	};
	//ǿ���ƶ�
	struct tagForceMoveInfo
	{
		float x,y;			// �켣�˶�����
		float dx,dy;		// ÿ��AI���x,y�����ƶ�����
		float endx,endy;	// Ŀ��λ��
		int count;			// ʱ���������Ҫִ�еĴ���
		CGUID RegionID;		// ����ID
	};
	// ���뼼�ܶ���
	struct tagDecordSkillList
	{
		long				lSkillKey;			// ���ܼ�ֵ
		long				lSkillID;			// ���ܱ��
		long				lSkillLvl;			// ���ܵȼ�
		long				lProceNum;			// ����ʩ�Ž׶�
		long				lMouseX;			// Ŀ��xλ��
		long				lMouseY;			// Ŀ��yλ��
		vector<long>		vecType;			// Ŀ������
		vector<CGUID>		vecID;				// Ŀ����
	};
	list<tagStateEx>*	GetStateExList()	{return &m_listStateEx;}

protected:
	tagForceMoveInfo * m_pForceMove;
	list<tagStateEx>   m_listStateEx;			// �쳣״̬�б�

	map<long, CSkillXml*> m_mapCurSkill;		// ��ǰ��������ִ�е����м���
	vector<tagDecordSkillList> m_vecDecordSkill;// �ӷ������������м��ܶ���
	/////////////XY 2008.2.16//////////////
	//��Ӧ�ɼ�������
	string m_strCollectionName;
	//�ɼ���id
	DWORD	m_dwCollectID;
	//(��ǰ)�ܱ��ɼ��Ĵ���
	DWORD	m_dwCollectTimes;
	//�Ƿ����ڱ��ɼ�
	bool	m_bIsBeenCollect;
	//�Ƿ��ܱ��ɼ�
	bool	m_bCanBeenCollect;
	///////////////////////////////////////
	//�Ƿ��ڳ���
	bool    m_bNotRide;

    ///@by:expter���Ӽ��ܵ�ֱ�Ӹ��ĵ�����״̬
    short                       m_sSelfControl;                 //������������Լ����
    short						m_sMoveable;                    //�Ƿ���ƶ�
    short                       m_sUseSkill;                    //�Ƿ����ʹ�ü���
    short                       m_sMoveDamage;                  //�ܵ��˺��Ƿ��Ƴ�
    short                       m_sBeenAttack;                  //�Ƿ��ܱ�����
    short						m_sRandomMove;					//����ܶ�

	bool m_bRun;					// ���߷�ʽ(false:walk,true:run)
	bool m_bAntiInvisibleable;		// �Ƿ�����
	bool m_bInvisible;				// ����
	bool m_bPretendDead;			// װ��	
	bool m_bUseRabbSkill;			// ʹ�÷�ʦ����
	bool m_bUseFighterSkill;		// ʹ��սʿ����
	bool m_bUseItem;				// ʹ����Ʒ
	bool m_bInstruction;			// ʹ��ָ��
	bool m_bUnAttack;				// ����������־��
	bool m_bAddHp;					// hp���ӱ�־��
	bool m_bHide;					// ����
	bool m_bForceAttact;			// �Ƿ�ǿ�ƹ���

	long m_lChangeGraphicstype;
	long m_lChangeGraphicsID;
	bool m_bInForceMoving;			// �Ƿ����ǿ���ƶ�״̬

	BOOL  m_bStateColorPlay;			//�쳣״̬����ɫ�������ſ���
	bool  m_bColorFade;					//�쳣״̬����ɫ�����Ƿ���ý���
	bool  m_bColorFadeFlag;				//�쳣״̬����ɫ��������������־
	float m_fColorFadePer;				//�쳣״̬����ɫ�������������İٷֱ�
	tagStateEx m_CurrentStateExForStateColor;	//��ǰ�����쳣״̬��ɫ���쳣״̬
	int	  m_iLastColorLevel;			//�쳣״̬ʱ����ɫ�������һ�εȼ�

	bool  m_busesimplemodel;

public:
	//////////XY 2008 2.18�йزɼ��Ľӿ�//////////
	string GetColletctionGoodsShowName(){return m_strCollectionName;}
	DWORD  GetCollectGoodsID()			{return m_dwCollectID;}
	DWORD  GetCollectTimes()			{return m_dwCollectTimes;}
	bool   IsBeingCollected()			{return m_bIsBeenCollect;}
	bool   IsCanBeCollected()			{return m_bCanBeenCollect;}

	
	bool   IsRide()						{return !m_bNotRide;}
	void   SetIsRide(bool b)			{ m_bNotRide = b; }
	bool   GetIsRun()					{return m_bRun;}
	void   SetIsRun(bool bRun)			{m_bRun = bRun;}
	void   SetCollectTimes(DWORD dwTimes){ m_dwCollectTimes = dwTimes;}
	void   SetBeingCollectState(bool bState) { m_bIsBeenCollect = bState;}
	void   SetCanBeCollectState(bool bState) { m_bCanBeenCollect = bState;}
	//////////////////////////////////////////////
	void AddCurSkillList(long lSkillKey, CSkillXml* pSkill){m_mapCurSkill[lSkillKey] = pSkill;}		// ��ӵ���ǰ�����б���
	CSkillXml* GetCurSkill(long lSkillKey);		// ȡ�ü����б���ĳһ������ʹ�õļ���
	void DeleteCurSkill(long lSkillKey);		// ɾ�������б���ĳһ������
	void DeleteCurSkillList();					// ɾ�������б�
	void SkillDisplay();		// ��ʾ����
	bool IsHaveSkill(){ return m_mapCurSkill.empty()?false:true; }	// �ж��Ƿ�������ʹ�õļ���
	bool IsHaveSkillByIndex(DWORD dwID);			// �쿴�Ƿ����ָ��ID�ļ���

	void UseCurSkill();					// ʹ�õ�ǰ����

	void AddStateEx(DWORD dwID,DWORD dwLevel, DWORD dwStandingTime, DWORD dwAdditionalData=0,BYTE bFlag = 0);
	void RemoveStateEx(DWORD dwID,DWORD dwLevel);
	void ClearStateEx();		// ��������쳣״̬
	bool IsHaveStateEx(DWORD dwID);	// �Ƿ���ĳ��״̬

	bool IsCanUseRabbSkill()		{return m_bUseRabbSkill;}		// �Ƿ���ʹ��ħ������
	bool IsCanUseFighterSkill()		{return m_bUseFighterSkill;}	// �Ƿ���ʹ��������
	bool IsCanUseItem()				{return m_bUseItem;}			// �Ƿ���ʹ����Ʒ

	bool IsInUnControlState(){return m_bInstruction;}	// �Ƿ��ڲ��ɿ��Ƶ��쳣״̬���������ܳ�ҩ�ͷż���
	short IsInUnMoveState(){return m_sMoveable;}		// �Ƿ��ڲ������ߵ��쳣״̬

	short IsInDreadState(){return m_sRandomMove;}	// �Ƿ��ڿ־���쳣״̬���¼ӣ�
	bool IsInDefianceState(){return m_bForceAttact;}	// �Ƿ��ڱ����Ƶ��쳣״̬���¼ӣ�
	bool IsInDieAwayState();						// �Ƿ��ڼ���״̬���¼ӣ�

	bool IsInForceMoving()		{return m_bInForceMoving;}
	void SetInForceMove(bool b)	{m_bInForceMoving=b;}
	bool GetIsConceal()			{return m_bInvisible;}					//ȡ���Ƿ��������״̬
	bool GetIsSpy()				{return m_bAntiInvisibleable;}			//ȡ���Ƿ������״̬
	bool GetIsHide()			{return m_bHide;}						//ȡ���Ƿ�������״̬

    short CanControlSelf()      {return m_sSelfControl;}                //�ܷ��Լ������Լ�
    short CanReMoveDamage()     {return m_sMoveDamage;}                 //�˺��Ƿ��Ƴ�
    short IsCanUseAllSkill()    {return m_sUseSkill;}                   //�ܷ�ʹ�ü���
    short IsCanBeenAttack()     {return m_sBeenAttack;}                 //�ܷ񱻹���

	void GetCurGraphicTypeID(long &lType,long &lID);

	void UpdataGetLocator();		// ��ʱ���°󶨵�λ��

	//void SetExRenderState();		//�趨�쳣״̬����Ⱦ״̬

	void MoveTo( float xDest, float yDest );
	BOOL IsStateColorPlay()		{ return m_bStateColorPlay; }			//�õ��쳣״̬��ɫ�Ƿ񲥷�

/////////////////////////////////////////////////////////////////////
// ��Ϣ��Ӧ AI
	/////////////////////////////////////////////////////////////////////
public:
	virtual void OnMessage(CMessage*);
	virtual void UpdateProperty(CMessage*);
	virtual	void DecordChangeProperty(BYTE* pByte,long& lPos);
	virtual	void DecodeChangePropertyFromDataBlock(DBReadSet& setReadDB);
	virtual void AI(void);
	virtual void SetSimpleModel(bool bsimplemodel){m_busesimplemodel = bsimplemodel;}
	virtual	bool Display(void);
	virtual	bool DisplayByLocator(CDisplayModel *pParentModel,AnimInfo *pParentAnimInfo,DWORD locatorname);
	virtual void DisplayLayer2D(float pos);
	virtual void RendShadow(bool bsimple = false);

	long AI_Stand();			// վAI
	long AI_Walk();				// ��AI
	long AI_Run();				// ��AI
	long AI_Attack();			// ����AI
	long AI_Perform();			// ʩ��AI
	long AI_BeAttack();			// ������AI
	long AI_Died();				// ����AI
	long AI_StateEx();			// �쳣״̬��AI
	void AI_Skill();			// ����AI
	void AI_SlipMove();			// ƽ����λAI
	
//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:
	// �����ƶ�ʱ�䶨ʱ��
	static void UpdateMoveTimeCounter(DWORD dwCurTime){m_dwMoveTimeCounter = dwCurTime;}
	static void UpdateLastMoveTimeCounter(DWORD dwCurTime){m_dwLastMoveTimeCounter = dwCurTime;}

	long MoveAI();											// ����AI
	long GetDestDir(long sx, long sy, long dx, long dy);	// ��ȡĿ�귽��
	long OnArrive();							// ����Ŀ��㴦��
	long OnArriveCell(long x, long y);						// �ﵽ�������Ĵ���
	//�������µĸ��Ӵ���
	DWORD GetEndMoveTime()			{return m_dwEndMoveTime;}
	void SetEndMoveTime(DWORD dwTime) {m_dwEndMoveTime = dwTime;}
	DWORD GetLastUpdatePosTime() {return m_dwLastUpdatePosTime;}
	void SetLastUpdatePosTime(DWORD dwTime);

	float GetDestX()					{return m_fDestX;}
	float GetDestY()					{return m_fDestY;}
	void SetDestXY(float x, float y)	{m_fDestX=x; m_fDestY=y;}
    float GetMoveOffsetX()				{return m_moveOffsetX;}
    float GetMoveOffsetY()				{return m_moveOffsetY;}
    void SetMoveOffset(float x, float y) {m_moveOffsetX=x; m_moveOffsetY=y;}

	void SetIsArrive(bool b)		{m_bIsArrive=b;}
	bool IsArrive()					{return m_bIsArrive;}
	
	long m_lStopCount;				// ֹͣ���ߵ��ӳ�֡��

protected:
	static DWORD m_dwMoveTimeCounter;//����moveshapeͳһʹ��ʱ���ʱ��������λ�ø���
	static DWORD m_dwLastMoveTimeCounter; //��һ���ƶ���ʱ���ʱ
	DWORD m_dwEndMoveTime;			// �����ƶ���ʱ��
	DWORD m_dwLastUpdatePosTime;	// ��һ�θ���λ��ʱ��
	float m_fBeginX;				// ��ʼ��X
	float m_fBeginY;				// ��ʼ��Y
	float m_fDestX;					// Ŀ���X
	float m_fDestY;					// Ŀ���Y
    float m_moveOffsetX;			// ��λʱ��(1ms)ƫ��X
    float m_moveOffsetY;			// ��λʱ��(1ms)ƫ��Y

	static long s_lIdleMinCount;	// �������idle�����ļ���
	static long s_lIdleMaxCount;	// ��ٽ���ilde�����ļ���
	long m_lIdleCount;				// վ����IDLE��������idle��һ���Ĵ�Сʱ������IDLE����
	bool m_bIdleStart;				// �Ƿ�ʼIDLE
	bool m_bIsArrive;				// �Ƿ�ﵽĿ���
	
//////////////////////////////////////////////////////////////////////////
//	̸��
//////////////////////////////////////////////////////////////////////////
public:
	void SetTalkText(const char* str)	{m_strTalkText=str;}
	const char* GetTalkText()			{return m_strTalkText.c_str();}
	void SetTalkLiveTimer(long l)		{m_lTalkLiveTimer=l;}
	long GetTalkLiveTimer()				{return m_lTalkLiveTimer;}
	
protected:
	string m_strTalkText;
	long m_lTalkLiveTimer;	

//////////////////////////////////////////////////////////////////////////
//	ðѪ
//////////////////////////////////////////////////////////////////////////
public:
	struct tagBloodText
	{
		string	strText;
		char	cType;		// ��������
		float	fPosX;		// λ������
		float	fPosY;		// λ������
		float	fPos;		// λ�ø߶�
		long	lAlpha;		// ͸����
		bool	bBlastAttack;	// ����
		float	fScale;		// ���ִ�С
		DWORD   dwStartTime; //��ʼʱ��
		float   fCurScale;
		float   fCurScale2;
	};

	void AddBloodText(const char* str, char cType, bool bBlastAttack=false, float fScale=1.0);

protected:
	list<tagBloodText>		m_listBloodText;

///////////////////////////////////////////////////////////////
//	ս���¼�
///////////////////////////////////////////////////////////////
public:
	enum eAttackEvent
	{
		AE_DAMAGE,				// ��Ѫ�˺�
		AE_BREAK,				// ����
		AE_DIED,				// ����
		AE_FULLMISS,			// ����
		AE_PARRY,				// �м�
		AE_BLOCK,				// ��
		AE_RESIST,				// �ֿ�
		AE_REBOUND,				// ����
		AE_SUCK,				// ����
		AE_LOST,				// ��ʧ
		AE_CHANGEDIR,			// �ı䷽��
		AE_CURE,				// ����
	};

	enum tagDeathType
	{
		DT_BEEN_KILLED			= 1,		// ��ɱ��
		DT_SELF_BLASTING		= 2,		// ��ɱ
		DT_EVANISH				= 3,		// ʱ�䵽
	};

	struct tagLost
	{
		char	cLostType;		// �˺�����(Public/skillrelated.h�����tagDamageType)
		long	lLostPoint;		// �˺�����
	};

	// ս���¼�
	struct tagAttackEvent
	{
		char cType;				// �¼�����(0:���� 1:���� 2:��ȫ����)
		long lTotalLostPoint;	// �ܵ��˺�����(hp+mp+...)
		long lFinalHP;			// ��������ʣ���HP
		long lHitNum;			// ������
		vector<tagLost>	vtLost;	// �˺��б�
		float fHurtedDir;		// �������ķ���
		//bool bCritical;			// �Ƿ�������һ��  ��ȡ����
		bool bBlastAttack;		// �Ƿ��Ǳ���
		tagDeathType eDeathType;// ��������
		DWORD dwSkillID;	    // �ܵ��˺��ļ��ܱ��
		BYTE  bySkillLevel;		// ���ܵȼ�
		CGUID Guidattracker;       // �����ߵ�GUID

		tagAttackEvent() 
		{
			cType = 0;
			lTotalLostPoint = 0;
			lFinalHP = 0;
			lHitNum = 0;
			fHurtedDir = 0.f;
			//bCritical = false;		// 
			bBlastAttack = false;
			eDeathType = DT_BEEN_KILLED;
			dwSkillID = 1;//SKILL_BASE_ATTACK
			bySkillLevel = 1;
			Guidattracker = CGUID::GUID_INVALID;
		}
	};

protected:
	queue<tagAttackEvent>		m_queueAttackEvent;		// ���Լ����ϵ�ս���¼�

public:
	queue<tagAttackEvent>* GetAttackEventQueue()	{return &m_queueAttackEvent;}
	long OnAttackEvent();		// �������¼�

	virtual	DWORD	GetHp()					{return 0;}
	virtual	void	SetHp(DWORD dw)			{}
	virtual	DWORD	GetMaxHp()    			{return 0;}
	virtual bool	IsDied();		// �Ƿ�����

public:
	static float		highhurt_monster;
	static float		highhurt_boss;
	static float		highhurt_knight;
	static float		highhurt_bow;
	static float		highhurt_magic;
	static float		mildhurt_monster ;
	static float		mildhurt_boss ;
	static float		mildhurt_knight ;
	static float		mildhurt_bow ;
	static float		mildhurt_magic ;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
public:
	long Distance(CShape* pShape);			// ������Shape��ľ��루��λ�����ӣ�

	//soft 2004.12.2
	long	m_lMoveSoundCount;				// ��������������

//////////////////////////////////////////////////////////////////////////
//	�Զ��嶯��ʱ�����
//////////////////////////////////////////////////////////////////////////
private:
	// �����������ݳ�Ա��CShape::eACTION�µ�ǰ12��ö������һһ��Ӧ
	float m_fActionCustomSpeedRate[12];
	// liuke 2008.8.14
	// ������ǰʹ�ü��ܵĹ涨ʱ��
	DWORD m_dwCurSkillActTime;
	// ������ǰʹ�ü��ܵĿ�ʼʱ��
	DWORD m_dwCurSkillStartTime;
	// liuke 08.8.21��ƽ����λ������ԣ�
	bool m_bIsSlipState;		// �Ƿ���ƽ����λ״̬
	float m_fSlipSpeed;			// ƽ����λ���ٶ�
	float m_fSlipDestPosX;		// ƽ����λ��Ŀ�������
	float m_fSlipDestPosY;
	float m_fSlipDir;			// ƽ���ƶ��ķ���
	DWORD m_dwSlipEndMoveTime;			// ���ƽ���ƶ���ʱ��
	DWORD m_dwSlipLastUpdatePosTime;	// ��һ��ƽ����λ����λ��ʱ��

public:
	void SetActionSpeedRate(WORD l, float fTimeRate = 1.0f)			{ m_fActionCustomSpeedRate[l] = fTimeRate; }
	// liuke 2008.8.14
	// ���ñ�����ǰʹ�ü��ܵĹ涨ʱ��
	void SetCurSkillStartTime(DWORD dwStartTime)	{m_dwCurSkillStartTime = dwStartTime;}
	// ���ñ�����ǰʹ�ü��ܵĹ涨ʱ��
	void SetCurSkillActTime(DWORD dwTime)	{m_dwCurSkillActTime = dwTime;}
	// ���ر�����ǰʹ�ü��ܵĹ涨ʱ��
	DWORD GetCurSkillActTime()	{return m_dwCurSkillActTime;}
	// ���ر�����ǰʹ�ü��ܵĿ�ʼʱ��
	DWORD GetCurSkillBeginTime()	{return m_dwCurSkillStartTime;}
	// �Ƿ�ﵽ��ǰʹ�ü��ܵĹ涨ʱ��
	bool IsCurSkillActTimeFinished();

	// ��ú�����ƽ����λ״̬
	void SetIsSlipState(bool bState)		{m_bIsSlipState = bState;}
	bool GetIsSlipState()					{return m_bIsSlipState;}
	// ��ú�����ƽ����λ�ٶ�
	void SetSlipSpeed(float fSpeed)			{m_fSlipSpeed = fSpeed;}
	float GetSlipSpeed()					{return m_fSlipSpeed;}
	// ��ú�����ƽ����λ����
	void SetSlipDestPos(float fx,float fy)	{m_fSlipDestPosX = fx;m_fSlipDestPosY = fy;}
	float GetSlipDestPosX()					{return m_fSlipDestPosX;}
	float GetSlipDestPosY()					{return m_fSlipDestPosY;}
	// ��ú�����ƽ����λ�ķ���
	void SetSlipDir(float fDir)				{m_fSlipDir = fDir;}
	float GetSlipDir()						{return m_fSlipDir;}
	// ��ú�������һ�θ���λ��ʱ��
	void SetSlipEndMoveTime(DWORD dwTime)	{m_dwSlipEndMoveTime = dwTime;}
	DWORD GetSlipEndMoveTime()				{return m_dwSlipEndMoveTime;}
	void SetSlipLastTime(DWORD dwTime)		{m_dwSlipLastUpdatePosTime = dwTime;}
	DWORD GetSlipLastTime()					{return m_dwSlipLastUpdatePosTime;}



//////////////////////////////////////////////////////////////////////////
//	�������͵�ַ��ӳ���
//////////////////////////////////////////////////////////////////////////
protected:
	struct tagPty
	{
		BYTE lType;		// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool 7:CGUID
		union
		{
			BYTE	*plValue;
			WORD	*pwValue;
			DWORD	*pdwValue;
			long	*pValue;
			float	*pfValue;
			short	*psValue;
			bool	*pbValue;
		};
	};

	map<string, tagPty>				m_mapNameValue;	// �������ֶ�Ӧ�ı�����ַ
	typedef map<string, tagPty>::iterator mapNameValueItr; 

	typedef map<DWORD,string>	mapProperttEnumToName;
	typedef map<DWORD,string>::iterator itProETN;

	//ö��ֵ�����ֵ�ӳ��
	mapProperttEnumToName m_PropertyEnumToName;

public:
	//�õ�����ֵ�ӿ�
	virtual bool  SetProperty(const char* attrName, long value);
	//������ֵ�ı��Ժ����
	virtual bool  OnPropertyChanged(const char* attrName);
	//�õ�����ֵ�ӿ�
	virtual long  GetProperty(const char* attrName);
	//�õ�����ֵ���ַ�����ʾ
	virtual const char* GetPropertyStringValue(const char* pszPropertyName);
	//���ݶ������Ը�ʽ���ַ���
	void FormatStringWithProperty(const char* pszIn,char *pszOut,int nOutSize);

	

//////////////////////////////////////////////////////////////////////////
// ��̬���(�����鶯��) add by lpf(2008.04.10)
// - ��������Ͷ�����������
//////////////////////////////////////////////////////////////////////////
private:
	// ����ṹ��
	struct tagFaceAnim
	{
		// ������Ϣ
		DWORD  dwFaceCount;			//����ѭ������
		DWORD  dwFaceStartIndex;	//���鿪ʼ���
		DWORD  dwFaceEndIndex;		//����������
		DWORD  dwFaceInterval;		//������֡�ٶ�(����)
		DWORD  dwFaceFileHandle;	//�����ļ����

		// ��ʱ��Ϣ
		bool  bPlayState;			//�Ƿ��ڲ���(Ϊtrue��ʾ���ڲ���)
		DWORD dwFaceLoopCount;		//��ǰ����ѭ������
		DWORD dwFaceCurrentIndex;	//��ǰ�������
		DWORD dwStartTime;			//��ʼ����ʱ��
	};

	// ��ǰ���ŵı�����Ϣ
	tagFaceAnim m_stCurrentFaceAnim;		//��ǰ���ŵı�����Ϣ

	// �����ṹ��
	struct tagActionAnim
	{
		int iActionIndex;	//�����ļ�����

		// ��ʱ��Ϣ
		CGUID guidEffect;			//��ǰ���ŵ���ЧGUID
		bool bActionPlayState;		//�Ƿ��ڲ��Ŷ���(Ϊtrue��ʾ���ڲ���)
		bool bEffectPlayState;		//����Ч�Ƿ��Ѿ����Ź�
		DWORD dwActionCurrentIndex;	//��ǰ���ŷֽ⶯�����
		DWORD dwStartTime;			//��ʼ����ʱ��
	};

	// ��ǰ���ŵĶ�����Ϣ
	tagActionAnim m_stCurrentActionAnim;		//��ǰ���ŵĶ�����Ϣ
	int   m_iActionChannel;                     //��ǰ���Ŷ�������ƵChannel
	string m_strActionAudioFile;                //��ǰ���Ŷ����������ļ�
	bool  m_bPlayActionAudio;                   //�Ƿ񲥷ű�����Ч

public:
	bool GetPlayStateOfActionAnim()		{ return m_stCurrentActionAnim.bActionPlayState; }		//�õ���ǰ��̬�����Ƿ����ڲ���
	int	 GetActionIndexOfActionAnim()	{ return m_stCurrentActionAnim.iActionIndex; }			//�õ���ǰ���ŵ���̬��������
	void SetupFaceAnim(tagFaceAction * pFaceAction);		//�趨���ű��鶯������Ҫ����Ϣ
	void SetupActionAnim(tagFaceAction * pFaceAction);		//�趨���Ŷ�����������Ҫ����Ϣ

	void PlayFaceAnim();			//���ű��鶯��
	void PlayActionAnim();			//���Ŷ�������

	void SetActionChannel(int iChannel){ m_iActionChannel=iChannel; } //���ñ��鶯������ƵChannel
	int GetActionChannel(){ return m_iActionChannel; }                //��ñ��鶯������ƵChannel
	void PlayActionSound() ;                                           //���ݶ������Ŷ�������Ч
	void SetActionAudio(string strFileName){m_strActionAudioFile = strFileName;} //���ñ��鶯��������
	string GetActionAudio(){ return m_strActionAudioFile; }           //��ñ��鶯��������
	void InitActionAudio(DWORD dwFaceActionID);                       //��ʼ�����鶯����Ч

/*************************************************************************
 * MoveShape���ϵ���Ч���У��������������Ч�ᰴ˳�򲥷�(����2008.11.21)
 * - ����һ����ʱ��˳�򲥷Ŷ����������Ч
 **************************************************************************/
	private:
		list<CEffect *> m_lsEffectByOrder;			// ˳����ʾ����Ч�б�
	public:
		// ���˳�򲥷ŵ���Ч
		CEffect* AddEffectToList(long lGraphicsID, CShape* pHoster, eLOCATOR eLocator, 
			bool bLoop=false, float fDir=0.f, long lID = 0,bool bAsyn=true);

		// ɾ��ָ��˳�򲥷ŵ���Ч
		void DelEffectByOrder(long lGraphicsID, long lID = 0);
		
		// �Ƴ�ָ��˳�򲥷ŵ���Ч
		void RemoveEffectByOrder(long lGraphicsID, long lID = 0);

		// ɾ��ȫ��˳�򲥷ŵ���Ч
		void DelAllEffectByOrder();
		
		// ����˳�򲥷ŵ���Ч
		void DisPlayEffectByOrder();

		// ˳�򲥷ŵ���Ч
		void AIEffectByOrder();

		// ����ָ��˳�򲥷ŵ���Ч
		CEffect * FindEffectByOrder(long lGraphicsID, long lID = 0);
	
	
	private:
		//===============07.12.11��������������ܵ�����������=============
		DWORD m_dwIntonateStartTime;		// �����������ܵĿ�ʼʱ��
		DWORD m_dwIntonateTime;				// �����������ܵ�����ʱ��
		DWORD m_wIntonateSkillID;			// �����������ܵ�ID
		long m_lIntonateSkillKey;			// �����������ܵļ�ֵ
		BYTE m_bIntonateAlpha;				// ��������alphaֵ
		bool m_bIsLToR;						// �Ƿ�������ҵ�����ʾ������
	public:
		// ���������������ܵĿ�ʼʱ�������ʱ��
		void SetIsLToR(bool bIsLToR)	{m_bIsLToR = bIsLToR;}
		// ���������������ܵĿ�ʼʱ�������ʱ��
		void SetCurIntonateTime(DWORD dwTime,DWORD dwStartTime);
		// ��õ�ǰ���������ļ���ID
		DWORD GetIntonateState()	{return m_wIntonateSkillID;}
		// ���õ�ǰ���������ļ���
		void SetIntonateSkill(DWORD wID)	{m_wIntonateSkillID=wID;}
		// ��õ�ǰ���������ļ���ID
		long GetIntonateKey()	{return m_lIntonateSkillKey;}
		// ���úͻ�õ�ǰ�������ܵļ�ֵ
		void SetIntonateSkillKey(long wKey)	{m_lIntonateSkillKey=wKey;}
		// ��������������
		void UpdataIntonatebar();
		// ����������
		void DisplayIntonate();
		//================================================================
};