#pragma once
#include "shape.h"
#include <queue>
//#include "../../nets/netGT/Message.h"
#include "../Utility/FindPath.h"
#include "../Net/Message.h"

class Mover;

class CMoveShape :
	public CShape
{
public:
	CMoveShape(void);
	inline virtual ~CMoveShape(void);

	// ��ʼ����������ֵ��Ӱ��
	virtual void InitNameValueMap();
	//����
	virtual void SetAction(WORD l);
	//״̬
	virtual void SetState(WORD l);

	//-------------------------------------------------------------------
	virtual	void DecordChangeProperty(BYTE* pByte,long& lPos);
	// ����
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	
	//��ѹbuff
	bool DecordBuffFromByteArray(BYTE* pByte, long& pos);
	//��ѹdebuff
	bool DecordDeBuffFromByteArray(BYTE* pByte, long& pos);
	//��ѹonoffskill
	bool DecordOnOffSkillFromByteArray(BYTE* pByte, long& pos);
	//��ѹ����ʹ�ü���
	bool DecordActSkillFromByteArray(BYTE* pByte, long& pos);

	virtual bool DecordFromDataBlock(DBReadSet& readDB,bool bExData = true);
	virtual void DecordChangePropertyFromDataBlock(DBReadSet& readDB);
	bool DecordBuffFromDataBlock(DBReadSet& readDB);
	bool DecordDebuffFromDataBlock(DBReadSet& readDB);
	bool DecordOnOffSkillFromDataBlock(DBReadSet& readDB);
	bool DecordActSkillFromDataBlock(DBReadSet& readDB);

	//  [8/11/2009 Square]
	void	IncrUsedTimes()	{ m_dwUsedTimes++; }
	void	ReduUsedTimes()	{ m_dwUsedTimes--; }
	DWORD	GetUsedTimes()	{ return m_dwUsedTimes; }
	void	SetUsedTimes(DWORD t)	{ m_dwUsedTimes = t; }

	/// ��λ
private:
	CGUID       m_PhaseID;

protected:
	//	���ü���
	DWORD	m_dwUsedTimes;

public:
	//// �쳣״̬
	//struct tagStateEx
	//{
	//	DWORD	dwID;					//	״̬���
	//	DWORD	dwLevel;				//	����״̬�ȼ�
	//	DWORD	dwAdditionalData;		//	�������� (ÿ�����ܵ���ʹ��)
	//	DWORD	dwStartTime;			//	��ʼʱ��
	//	DWORD	dwStandingTime;			//	����ʱ�� (0����һֱ����)
	//	bool	bEndSelf;				//	�����Լ�
	//	long	lCount;					//	�����������ʹ��ͬһ״̬ʱʹ��

	//	tagStateEx()
	//	{
	//		dwID = 0;
	//		dwLevel = 0;
	//		dwStartTime = 0;
	//		dwStandingTime = 0;
	//		bEndSelf = false;
	//		lCount = 0;
	//	}
	//};
	////ǿ���ƶ�
	//struct tagForceMoveInfo
	//{
	//	float x,y;			// �켣�˶�����
	//	float dx,dy;		// ÿ��AI���x,y�����ƶ�����
	//	float endx,endy;	// Ŀ��λ��
	//	int	  count;			// ʱ���������Ҫִ�еĴ���
	//	CGUID RegionGuid;		// ����ID
	//};
	// ���뼼�ܶ���
	//struct tagDecordSkillList
	//{
	//	long				lSkillKey;			// ���ܼ�ֵ
	//	long				lSkillID;			// ���ܱ��
	//	long				lSkillLvl;			// ���ܵȼ�
	//	long				lProceNum;			// ����ʩ�Ž׶�
	//	long				lObjectX;			// Ŀ��xλ��
	//	long				lObjectY;			// Ŀ��yλ��
	//	vector<long>		vecType;			// Ŀ������
	//	vector<CGUID>		vecID;				// Ŀ����
	//};
	//	list<tagStateEx>*	GetStateExList()	{return &m_listStateEx;}

protected:
	//	tagForceMoveInfo * m_pForceMove;
	//	list<tagStateEx>   m_listStateEx;							// �쳣״̬�б�
	//	vector<tagDecordSkillList> m_vecDecordSkill;				// �ӷ������������м��ܶ���

	//�ɼ�
protected:
	//	string m_strCollectionName;					//��Ӧ�ɼ�������
	//	DWORD	m_dwCollectID;							//�ɼ���id
	//	DWORD	m_dwCollectTimes;					//(��ǰ)�ܱ��ɼ��Ĵ���
	//	bool	m_bIsBeenCollect;					//�Ƿ����ڱ��ɼ�
	//	bool	m_bCanBeenCollect;					//�Ƿ��ܱ��ɼ�
		bool    m_bNotRide;							//�Ƿ��ڳ���

		bool m_bRun;					// ���߷�ʽ(false:walk,true:run)
		//bool m_bAntiInvisibleable;		// �Ƿ�����
		//bool m_bInvisible;				// ����
		//bool m_bPretendDead;			// װ��	
		//bool m_bUseRabbSkill;			// ʹ�÷�ʦ����
		//bool m_bUseFighterSkill;		// ʹ��սʿ����
		//bool m_bUseItem;				// ʹ����Ʒ
		//bool m_bInstruction;			// ʹ��ָ��
		//bool m_bUnAttack;				// ����������־��
		//bool m_bAddHp;					// hp���ӱ�־��
		//bool m_bHide;					// ����
		//bool m_bForceAttact;			// �Ƿ�ǿ�ƹ���

// 		long m_lChangeGraphicstype;
// 		long m_lChangeGraphicsID;
		//bool m_bInForceMoving;			// �Ƿ����ǿ���ƶ�״̬

public:
	//////////XY 2008 2.18�йزɼ��Ľӿ�//////////
	//string GetColletctionGoodsShowName(){return m_strCollectionName;}
	//DWORD  GetCollectGoodsID()			{return m_dwCollectID;}
	//DWORD  GetCollectTimes()			{return m_dwCollectTimes;}
	//bool   IsBeingCollected()			{return m_bIsBeenCollect;}
	//bool   IsCanBeCollected()			{return m_bCanBeenCollect;}

	bool   IsRide()						{return !m_bNotRide;}
	bool   GetIsRun()					{return m_bRun;}
	void   SetIsRun(bool bRun)			{m_bRun = bRun;}

	//void   SetCollectTimes(DWORD dwTimes){ m_dwCollectTimes = dwTimes;}
	//void   SetBeingCollectState(bool bState) { m_bIsBeenCollect = bState;}
	//void   SetCanBeCollectState(bool bState) { m_bCanBeenCollect = bState;}

	void DeleteCurSkill(long lSkillKey);						// ɾ�������б���ĳһ������
	void DeleteCurSkillList();									// ɾ�������б�

	void UseCurSkill();											// ʹ�õ�ǰ����

	void AddStateEx(DWORD dwID,DWORD dwLevel, DWORD dwStandingTime, DWORD dwAdditionalData=0);
	void RemoveStateEx(DWORD dwID);
	void ClearStateEx();										// ��������쳣״̬
	bool IsHaveStateEx(DWORD dwID);								// �Ƿ���ĳ��״̬

	//bool IsInUnControlState(){return m_bInstruction;}			// �Ƿ��ڲ��ɿ��Ƶ��쳣״̬���������ܳ�ҩ�ͷż���
	//bool IsInUnMoveState(){return m_bMoveable;}					// �Ƿ��ڲ������ߵ��쳣״̬

	//bool IsInDreadState(){return m_bRandomMove;}				// �Ƿ��ڿ־���쳣״̬���¼ӣ�
	//bool IsInDefianceState(){return m_bForceAttact;}			// �Ƿ��ڱ����Ƶ��쳣״̬���¼ӣ�
	bool IsInDieAwayState();									// �Ƿ��ڼ���״̬���¼ӣ�

	//bool IsInForceMoving()		{return m_bInForceMoving;}
	//void SetInForceMove(bool b)	{m_bInForceMoving=b;}
	bool GetIsConceal();										// ȡ���Ƿ��������״̬
	bool GetIsSpy();											// ȡ���Ƿ������״̬
	//bool IsInHideState();										// �Ƿ�������״̬

	void UpdataGetLocator();		// ��ʱ���°󶨵�λ��

	/////////////////////////////////////////////////////////////////////
	// ��Ϣ��Ӧ AI
	/////////////////////////////////////////////////////////////////////
protected:
	// Ѱ·
	CFindPath	m_FindPath;

public:
	long GetDestDir(long sx, long sy, long dx, long dy);		// ��ȡĿ�귽��
	long OnArrive();										// ����Ŀ��㴦��
	long OnArriveCell(long x, long y);							// �ﵽ�������Ĵ���
	//�������µĸ��Ӵ���
	long MoveAI();											// ����AI
	void AI(void);
	//	DWORD GetBeginMoveTime()			{return m_dwBeginMoveTime;}
	//	void SetBeginMoveTime(DWORD dw)		{m_dwBeginMoveTime=dw;}
	//	DWORD GetEndMoveTime()				{return m_dwEndMoveTime;}
		void SetEndMoveTime(DWORD dwTime)	 {m_dwEndMoveTime = dwTime;}
		DWORD GetLastUpdatePosTime()		 {	return m_dwLastUpdatePosTime;}
		void SetLastUpdatePosTime(DWORD dwTime) { m_dwLastUpdatePosTime = dwTime;}
	float GetBeginX()					{return m_fBeginX;}
	float GetBeginY()					{return m_fBeginY;}
	void SetBeginXY(float x, float y)	{m_fBeginX=x; m_fBeginY=y;}

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

	float GetDestX()					{return m_fDestX;}
	float GetDestY()					{return m_fDestY;}
	void SetDestXY(float x, float y)	{m_fDestX=x; m_fDestY=y;}

	void SetIsArrive(bool b)		{m_bIsArrive=b;}
	bool IsArrive()					{return m_bIsArrive;}

	float GetMoveOffsetX()				{return m_moveOffsetX;}
	float GetMoveOffsetY()				{return m_moveOffsetY;}
	void SetMoveOffset(float x, float y) {m_moveOffsetX=x; m_moveOffsetY=y;}

	//virtual float GetSpeed()		{ return m_fSpeed; }
	//virtual void SetSpeed(float l)	{ m_fSpeed = l; }
	//	long m_lStopCount;				// ֹͣ���ߵ��ӳ�֡��
protected:
	//	static DWORD m_dwMoveTimeCounter;//����moveshapeͳһʹ��ʱ���ʱ��������λ�ø���
	//	static DWORD m_dwLastMoveTimeCounter; //��һ���ƶ���ʱ���ʱ
	//	DWORD m_dwBeginMoveTime;		// ��ʼ�ƶ���ʱ��
		DWORD m_dwEndMoveTime;			// �����ƶ���ʱ��
		DWORD m_dwLastUpdatePosTime;	// ��һ�θ���λ��ʱ��
	float m_fBeginX;				// ��ʼ��X
	float m_fBeginY;				// ��ʼ��Y
	float m_fDestX;					// Ŀ���X
	float m_fDestY;					// Ŀ���Y
	float m_moveOffsetX;			// ��λʱ��(1ms)ƫ��X
	float m_moveOffsetY;			// ��λʱ��(1ms)ƫ��Y

	//float m_fSpeed;					// �ٶ�
	//	static long s_lIdleMinCount;	// �������idle�����ļ���
	//	static long s_lIdleMaxCount;	// ��ٽ���ilde�����ļ���
	//	long m_lIdleCount;				// վ����IDLE��������idle��һ���Ĵ�Сʱ������IDLE����
	//	bool m_bIdleStart;				// �Ƿ�ʼIDLE
	bool m_bIsArrive;				// �Ƿ�ﵽĿ���


	//////////////////////////////////////////////////////////////////////////
	//	̸��
	//////////////////////////////////////////////////////////////////////////
public:
	//	void SetTalkText(const char* str)	{m_strTalkText=str;}
	//	const char* GetTalkText()			{return m_strTalkText.c_str();}
	//	void SetTalkLiveTimer(long l)		{m_lTalkLiveTimer=l;}
	//	long GetTalkLiveTimer()				{return m_lTalkLiveTimer;}
protected:
	//	string m_strTalkText;
	//	long m_lTalkLiveTimer;	
	//////////////////////////////////////////////////////////////////////////
	//	ðѪ
	//////////////////////////////////////////////////////////////////////////
public:
	//struct tagBloodText
	//{
	//	string	strText;
	//	char	cType;		// ��������
	//	float	fPosX;		// λ������
	//	float	fPosY;		// λ������
	//	float	fPos;		// λ�ø߶�
	//	long	lAlpha;		// ͸����
	//	bool	bBlastAttack;	// ����
	//	float	fScale;		// ���ִ�С
	//	DWORD   dwStartTime; //��ʼʱ��
	//	float   fCurScale;
	//};

	//	void AddBloodText(const char* str, char cType, bool bBlastAttack=false, float fScale=1.0);

protected:
	//	list<tagBloodText>		m_listBloodText;
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
	};

	enum tagDeathType
	{
		DT_BEEN_KILLED			= 1,		// ��ɱ��
		DT_SELF_BLASTING		= 2,		// ��ɱ
		DT_EVANISH				= 3,		// ʱ�䵽
	};

	//struct tagLost
	//{
	//	char	cLostType;		// �˺�����(public\skillrelated.h�����tagDamageType)
	//	WORD	wLostPoint;		// �˺�����
	//};

	//// ս���¼�
	//struct tagAttackEvent
	//{
	//	char cType;				// �¼�����(0:���� 1:���� 2:��ȫ����)
	//	long lTotalLostPoint;	// �ܵ��˺�����(hp+mp+...)
	//	long lFinalHP;			// ��������ʣ���HP
	//	long lHitNum;			// ������
	//	vector<tagLost>	vtLost;	// �˺��б�
	//	float fHurtedDir;		// �������ķ���
	//	//bool bCritical;			// �Ƿ�������һ��  ��ȡ����
	//	bool bBlastAttack;		// �Ƿ��Ǳ���
	//	tagDeathType eDeathType;// ��������
	//	DWORD dwSkillID;	    // �ܵ��˺��ļ��ܱ��
	//	BYTE  bySkillLevel;		// ���ܵȼ�
	//	CGUID AtkerGuid;

	//	tagAttackEvent() 
	//	{
	//		cType = 0;
	//		lTotalLostPoint = 0;
	//		lFinalHP = 0;
	//		lHitNum = 0;
	//		fHurtedDir = 0.f;
	//		bBlastAttack = false;
	//		eDeathType = DT_BEEN_KILLED;
	//		dwSkillID = 1;//SKILL_BASE_ATTACK
	//		bySkillLevel = 1;
	//		AtkerGuid = CGUID::GUID_INVALID;
	//	}
	//};

protected:
	//	queue<tagAttackEvent>		m_queueAttackEvent;		// ���Լ����ϵ�ս���¼�

public:
	//	queue<tagAttackEvent>* GetAttackEventQueue()	{return &m_queueAttackEvent;}
	long OnAttackEvent();		// �������¼�

	virtual	DWORD	GetHp()					{return 0;}
	virtual	void	SetHp(DWORD dw)			{}
	virtual	DWORD	GetMaxHp()    			{return 0;}
	virtual bool	IsDied();		// �Ƿ�����

public:
	//static float		highhurt_monster;
	//static float		highhurt_boss;
	//static float		highhurt_knight;
	//static float		highhurt_bow;
	//static float		highhurt_magic;
	//static float		mildhurt_monster ;
	//static float		mildhurt_boss ;
	//static float		mildhurt_knight ;
	//static float		mildhurt_bow ;
	//static float		mildhurt_magic ;

public:
	long Distance(CShape* pShape);			// ������Shape��ľ��루��λ�����ӣ�

	//////////////////////////////////////////////////////////////////////////
	//	�Զ��嶯��ʱ�����
	//////////////////////////////////////////////////////////////////////////
private:
	// �����������ݳ�Ա��CShape::eACTION�µ�ǰ12��ö������һһ��Ӧ
		float m_fActionCustomSpeedRate[12];
	// liuke 2008.8.14
	// ������ǰʹ�ü��ܵĹ涨ʱ��
	//	DWORD m_dwCurSkillActTime;
	// ������ǰʹ�ü��ܵĿ�ʼʱ��
	//	DWORD m_dwCurSkillStartTime;
	// liuke 08.8.21��ƽ����λ������ԣ�
	//	bool m_bIsSlipState;		// �Ƿ���ƽ����λ״̬
	//	float m_fSlipSpeed;			// ƽ����λ���ٶ�
	//	float m_fSlipDestPosX;		// ƽ����λ��Ŀ�������
	//	float m_fSlipDestPosY;
	//	float m_fSlipDir;			// ƽ���ƶ��ķ���
	//	DWORD m_dwSlipEndMoveTime;			// ���ƽ���ƶ���ʱ��
	//	DWORD m_dwSlipLastUpdatePosTime;	// ��һ��ƽ����λ����λ��ʱ��
public:
	void SetActionSpeedRate(WORD l, float fTimeRate = 1.0f)			{ m_fActionCustomSpeedRate[l] = fTimeRate; }
	// liuke 2008.8.14
	// ���ñ�����ǰʹ�ü��ܵĹ涨ʱ��
	//	void SetCurSkillStartTime(DWORD dwStartTime)	{m_dwCurSkillStartTime = dwStartTime;}
	// ���ñ�����ǰʹ�ü��ܵĹ涨ʱ��
	//	void SetCurSkillActTime(DWORD dwTime)	{m_dwCurSkillActTime = dwTime;}
	// ���ر�����ǰʹ�ü��ܵĹ涨ʱ��
	//	DWORD GetCurSkillActTime()	{return m_dwCurSkillActTime;}
	// �Ƿ�ﵽ��ǰʹ�ü��ܵĹ涨ʱ��
	bool IsCurSkillActTimeFinished();

	// ��ú�����ƽ����λ״̬
	//	void SetIsSlipState(bool bState)		{m_bIsSlipState = bState;}
	//	bool GetIsSlipState()					{return m_bIsSlipState;}
	// ��ú�����ƽ����λ�ٶ�
	//	void SetSlipSpeed(float fSpeed)			{m_fSlipSpeed = fSpeed;}
	//	float GetSlipSpeed()					{return m_fSlipSpeed;}
	// ��ú�����ƽ����λ����
	//	void SetSlipDestPos(float fx,float fy)	{m_fSlipDestPosX = fx;m_fSlipDestPosY = fy;}
	//	float GetSlipDestPosX()					{return m_fSlipDestPosX;}
	//	float GetSlipDestPosY()					{return m_fSlipDestPosY;}
	// ��ú�����ƽ����λ�ķ���
	//	void SetSlipDir(float fDir)				{m_fSlipDir = fDir;}
	//	float GetSlipDir()						{return m_fSlipDir;}
	// ��ú�������һ�θ���λ��ʱ��
	//	void SetSlipEndMoveTime(DWORD dwTime)	{m_dwSlipEndMoveTime = dwTime;}
	//	DWORD GetSlipEndMoveTime()				{return m_dwSlipEndMoveTime;}
	//	void SetSlipLastTime(DWORD dwTime)		{m_dwSlipLastUpdatePosTime = dwTime;}
	//	DWORD GetSlipLastTime()					{return m_dwSlipLastUpdatePosTime;}
	//�������͵�ַ��ӳ���
protected:
	//struct tagPty
	//{
	//	BYTE lType;		// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool 7:CGUID
	//	union
	//	{
	//		BYTE	*plValue;
	//		WORD	*pwValue;
	//		DWORD	*pdwValue;
	//		long	*pValue;
	//		float	*pfValue;
	//		short	*psValue;
	//		bool	*pbValue;
	//	};
	//};

	//map<string,tagPty>		m_mapNameValue;
	//typedef map<string,tagPty>::iterator mapNameValueItr;
	//typedef map<DWORD,string>::iterator itProETN;
	//typedef map<DWORD,string>	m_mapPropertyEnumToName;
	//m_mapPropertyEnumToName m_PropertyEnumToName;
public:
	virtual bool SetProperty(const char* strName,long value);
	virtual bool OnPropertyChanged(const char* strName);
	virtual long GetProperty(const char*strName);
	virtual const char* GetPropertyStringValue(const char* pszPropertyName);
	void FormatStringWithProperty(const char* pszIn,char*pszOut,int nOutSize);

public:
    void RequestMove( float destX, float destY );

    bool UpdateMove();

    void StopMove();

    virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
private:
    Mover *m_mover;
};
