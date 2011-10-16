#pragma once

#include "Shape.h"
#include "StateAI/BaseAI.h"
#include "Goods/CGoods.h"
#include "BuffSkill/NewSkillFactory.h"
#include "BuffSkill/AttackPower.h"
#include "MessagePackage/CS2CContainerObjectMove.h"
#include "../../../Public/Common/SkillRelated.h"
#include "../../../Public/Common/GoodsDef.h"
#include "../../../Public/Common/BasePetDef.h"
#include "../../../Public/Setup/SkillAttriSetup.h"

class VariableList;
class Script;

class CRegion;
class CPlayer;
class CBaseModule;
class COpPosition;
//class CS2CContainerObjectMove;
//static long   g_lPassCount;

class CScript;

class CMoveShape :
	public CShape
{
public:
	CMoveShape(void);
	virtual ~CMoveShape(void);

	virtual void SetAction(WORD l){}

protected:

	StateAI::BaseAI*					m_pAI;

	///��ǰѡ��ļ���
	tagSkillID					m_tCurrentSkill;
public:

    // [070611 AHC]
    virtual bool  SetAttribute(std::string& attrName, long value);
	virtual bool  SetAttribute(char* attrName, long value);
    virtual long  GetAttribute(std::string attrName);
    virtual bool  ChangeAttribute(std::string& attrName, long value);
	virtual void  AddAttributeToDirty(string attrName) {return;}

	virtual long  UpdateAttributeToClient(){return 0;}
	virtual long  UpdateAttribute(BOOL bImmediately=0){return 0;}
	virtual long  OnUpdateAttributeTimeOut(){return 0;}
	virtual long  UpdateHPMPToClient(set<string>& dirtyAttr){return 0;}

	virtual BOOL  SetBoolAttribute(const std::string& attrName, long value);
	virtual BOOL  SetBoolAttribute(const std::string &attrName, BOOL b);

    virtual long  SetStateAttribute(std::string& attrName,long    value);
    virtual long  ChangeStateAttribute(std::string& attrName,long value);

	long CMoveShape::GetValue(const char* strName);
	long CMoveShape::SetValue(const char* strName, long dwValue);
	//�����Է����ı�ʱ�����
	virtual void OnPropertyValueChanged(const char* strName);
	
	virtual	BYTE		GetCountry(){return 0;}
	virtual DWORD		GetPkValue(){return 0;}
	virtual DWORD		GetPVPValue(){return 0;}
	virtual CMoveShape* GetHost(void) {return NULL;}

	// ��ʼ��
	void InitNameValueMap();
	//////////////////////////////////////////////////////////////////////////
	//	�������͵�ַ��ӳ���
	//////////////////////////////////////////////////////////////////////////

private:
    void        SendSkillImmunityToClient(const std::string&  AttrName) ;

protected:
	map<string, tagPty>				m_mapShapeNameValue;	// �������ֶ�Ӧ�ı�����ַ
	typedef map<string, tagPty>::iterator mapShapeNameValueItr; 

	set<string>  m_mapShapeDirtyAttrData;
	void    PushShapeDirtyAttrData(string attrName) { m_mapShapeDirtyAttrData.insert(attrName); }
	string  PopShapeDirtyAttrData() 
	{ 
		string tmp = *m_mapShapeDirtyAttrData.begin(); 
		m_mapShapeDirtyAttrData.erase(m_mapShapeDirtyAttrData.begin());
		return tmp; 
	}
	long    DirtyShapeAttrDataSize(void) { return (long)m_mapShapeDirtyAttrData.size(); }

	//////////////////////////////////////////////////////////////////////////
	//	������
	//////////////////////////////////////////////////////////////////////////
public:
	void SetBeneficiary(long lType,CGUID guidFirst);				//����������
	void RecordBeneficiaryTime(DWORD time=0)			//��¼����ܹ���ʱ��
	{
		if (time==0)
		{
			m_dwBeneficiaryTime= timeGetTime();
		}
		else
		{
			m_dwBeneficiaryTime= time;
		}
	}		
	void CleanBeneficiary();							//���������
	long GetBeneficiaryTime(){return m_dwBeneficiaryTime;}
	CGUID GetBeneficiaryID(){return m_BeneficiaryID;}
	OBJECT_TYPE GetBeneficiaryType(){return m_BeneficiaryType;}
	long AddBeneficiaryEvent(long lTime);				//����������¼�
	virtual void OnLoseBeneficiary(){}					//��ʧ�������¼�
	virtual void StartRecordBeneficiary(CGUID guidFirst,OBJECT_TYPE typeFirst,DWORD Time=0){}				//��ʼ��¼������
private:
	CGUID m_BeneficiaryID;				// ���ȹ����Լ�����ҵ�id
	OBJECT_TYPE m_BeneficiaryType;		// �����ߵ�����
	DWORD m_dwBeneficiaryTime;			// ���һ�ι���ʱ��
	LONG m_lBeneficiaryTimerID;			// �������¼�id


    /// @Expter 2010/03/18 ��λ�ı�ʶ
public:
    const CGUID&  GetPhaseID()         { return m_PhaseID; }
    void  SetPhaseID(const CGUID& gid) { m_PhaseID = gid;}

protected:
    CGUID       m_PhaseID;


//---------------------------------------------------------------------
// ע����ʧ
//---------------------------------------------------------------------
public:
	void AddDelEvent(DWORD dwTime);		// ע��ɾ���¼�
	void LogoutDelEvent();				// ע��ɾ���¼�
	void CleanDelEvent();				// ���ɾ���¼�
	virtual void OnDelEvent(){}		// ��Ӧɾ���¼�
	
	//ע��������ԵĶ�ʱ��
	void RegisterUpdateAttrTimer();
	//ע���������Զ�ʱ��
	void CancelUpdateAttrTimer();
	//�������Զ�ʱ��
	void OnUpdateAttrTimeOut();
	//���÷�����Ϣ��ʱ��,�����ʱ�򷵻�true
	bool ResetSendMoveMsgTime();

private:
	LONG m_lDelEventTimerID;			// ע����ʧʱ��
	long	m_lUpdateAttrTimerID;	//����ʵ�����Ե��ͻ��˵Ķ�ʱ��
	//������Ǹ���Զ������ƶ���Ϣ��ʱ��
	DWORD	m_dwLastSendMoveSendTime;

//---------------------------------------------------------------------
//  ���񱣻���־
//---------------------------------------------------------------------
private:
	bool m_bRoleProtect;
public:
	void SetRoleProtect(bool l){m_bRoleProtect= l;}
	bool GetRoleProtect(){return m_bRoleProtect;}

	/////////////////////////////////////////////////////////////////////
	// ��Ϣ��Ӧ
	/////////////////////////////////////////////////////////////////////
public:
	virtual void OnMessage(CMessage* pMsg)	{return;}
	void OnShapeMessage(CMessage* pMsg);
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	virtual long Proc(eEventType type,const tagProcVar *pVar);
	void UpdateAbnormality();			// �쳣״̬����
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////

    enum  eShapeStates 
    {
        eMoveable  = 0x0001,
        eUseSkill  = 0x0002,
        eMoveDamage= 0x0004,
        eBeenAttack= 0x0006,
        eRandomMove= 0x0008,
    };

    enum  eActionStates
    {
        eAbnormity =0,
        eNormal    =1,
        eImmunity  =2,
    };
private:
	bool						m_bRun;							//�ƶ���ʽ�Ƿ�����
	
	LONG						m_lMoveableCount;

    ///@by:expter���Ӽ��ܵ�ֱ�Ӹ��ĵ�����״̬
    short                       m_sSelfControl;                 //������������Լ����
    short						m_sMoveable;                    //�Ƿ���ƶ�
    short                       m_sUseSkill;                    //�Ƿ����ʹ�ü���
    short                       m_sMoveDamage;                  //�ܵ��˺��Ƿ��Ƴ�
    short                       m_sBeenAttack;                  //�Ƿ��ܱ�����
    short						m_sRandomMove;					//����ܶ�
    float                       m_fMoveSpeed;                   //����Ӱ�������ƶ��ٶ�

    int                         m_ShapeStates;                  //�����Ϊ״̬


	bool						m_bUseFighterSkill;				//ʹ��ս�����ܱ�־��
	bool						m_bUseRabbSkill;				//ʹ�÷�ʦ���ܱ�־��
	bool						m_bUseItem;						//ʹ�õ��ߵı�־��
	bool						m_bAntiInvisibleable;			//�Ƿ�����
	bool						m_bInvisible;					//���ɼ�״̬
	bool						m_bHide;						//����		
	bool						m_bPretendDead;					//װ��
	bool						m_bInstruction;					//ʹ��ָ���־��
	bool						m_bAddHp;						//Hp�������ӵı�־��
	long						m_lChangeModelType;				//��������
	long						m_lChangeModelID;				//����id
	bool						m_bForceAttack;					//ǿ�ƹ���
	
	bool						m_bRide;						//������״̬
	CGUID						m_BefoolID;						//��Ū�Լ���id

private:
	bool						m_bWeakness;					//����״̬
	long						m_lWeakCount;					//��������

public:
	bool						GetWeaknessState(){return m_bWeakness;}
	void						SetWeakNessState(bool bWeak);
	//�ı�����״̬
	virtual void				ChangeWeaknessState(bool l){}
public:
	
    bool                        OnStates(eShapeStates e)   { return  (m_ShapeStates&e) == e ;}

	virtual void				OnCannotMove();
	virtual void				SetPosition( LONG lX, LONG lY );
    virtual void				SetPosition( float x, float y );

	virtual void				OnMove(float fNewX, float fNewY);
	virtual void				OnStopMove();
	virtual void				SetSlipPos( LONG lX, LONG lY,float fSpeed,float fDir);
    virtual void				SetSlipPos( float x, float y,float fSpeed,float fDir);


	virtual void OnQuestMoveStep(float fCurX,float fCurY,float fDestX,float fDestY){return;}
	virtual void OnQuestStopMove(float fCurX,float fCurY,short lDir){return;}

	bool						GetIsRide()	{return m_bRide;}
	void						SetIsRide(bool bRide){m_bRide = bRide;}

	bool						GetIsRun()	{return m_bRun;}
	void						SetIsRun(bool bRun){m_bRun = bRun;}

	virtual bool				GetForceAttack(){return m_bForceAttack;}            //ǿ�ƹ���
	void						SetForceAttack(bool b){m_bForceAttack = b;}

	virtual const CGUID&		GetBefoolID() {return NULL_GUID;}//return m_BefoolID;}	
	virtual void				SetBefoolID(const CGUID& guid){m_BefoolID=guid;}					//������Ū�Լ���id

public:
	void			SetUseFightSkill(bool b);					//����ʹ��սʿ���ܱ�־��
	bool			GetUseFightSkill(){return m_bUseFighterSkill;}

	void			SetUseRabbSkill(bool b);					//����ʹ�÷�ʦ���ܱ�־��
	bool			GetUseRabbSkill(){return m_bUseRabbSkill;}

	void			SetUseItem(bool b);							//����ʹ�õ��߱�־��
	bool			GetUseItem(){return m_bUseItem;}

	void			SetUseInstruction(bool b){m_bInstruction = b;}//���ʹ��ָ���־��
	bool			GetInstruction(){return m_bInstruction;}

	void			SetAddHp(bool b){m_bAddHp = b;}
	bool			GetAddHp(){return m_bAddHp;}

	void			SetChangeModelType(long l){m_lChangeModelType = l;}	//�ı�ģ������
	long			GetChangeModelType(){return m_lChangeModelType;}

	void			SetChangeModelID(long l){m_lChangeModelID = l;}		//�ı�ģ��id
	long			GetChangeModelID(){return m_lChangeModelID;}

	virtual void	SetAntiInvisibleable(bool l){m_bAntiInvisibleable=l;}			//�����Ƿ�����
	bool			GetAntiInvisibleable(){return m_bAntiInvisibleable;	}

	virtual void	SetInvisibleState(bool l){m_bInvisible=l;}			//�����Ƿ񲻿ɼ�״̬
	bool			GetInvisibleState(){return m_bInvisible;}

	virtual void	SetPretendDeadState(bool l){m_bPretendDead=l;}			//�����Ƿ�װ��
	bool			GetPretendDeadState(){return m_bPretendDead;}	
	virtual void	SetHide(bool l){m_bHide=l;}								//����
	bool			GetHide(){return m_bHide;}

	//����Ŀ���״̬���ж��Ƿ�ɼ�
	virtual bool	IsVisible(CMoveShape* pTarget);

	//! 
	//!*************************
	void			GetAllBuffID(list<pair<DWORD, DWORD>>& listBuff);	//! �õ���BUFF��ID
	virtual	void	OnAddBuff(DWORD dwStateID, DWORD dwBuffLevel);
	virtual	void	OnDelBuff(DWORD dwStateID, DWORD dwBuffLevel);
	//!*************************


	

	virtual void	InitSkillInstance(DWORD dwSkillID,DWORD dwLevel,long lGoodsIndex=0,long lContainerID=0,long lPos=0,CGUID GoodsGuid=NULL_GUID,long lType=0);
	virtual void	AddActiveSkill(stModuParam *pSkillParam);

	virtual long	AddActiveSkill(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget=NULL,long lGoodsIndex=0,long lContainerID=0,long lPos=0,CGUID GoodsGuid=NULL_GUID);				//�����������

	virtual stModuParam *AddPassiveSkill(DWORD dwSkillID, DWORD dwLevel);		//��ӱ�������
	void	AddPassSkill(DWORD dwSkillID, DWORD dwLevel);
	long	GetAllStateNum();											//��ȡ����״̬����
	long    GetStateParamByID(DWORD dwID,string strParam);				//ͨ��״̬id�Ͳ������Ƶõ�״̬����ز���
    long    GetStateParamByID(DWORD dwID,string strParam,CMoveShape* shape,long type);//ͨ��״̬id�Ͳ��������������Ŀ��shape��id����
	long	GetStateParamByIndex(DWORD dwIndex,string strParam);		//ͨ��״̬�����Ͳ������Ƶõ�״̬����ز���

	virtual	void	SetPkValue(DWORD lValue){}									//����PKֵ
																										  

	//���ͱ仯���ٶ�ֵ���ͻ���
	void			SendChangedSpeedToClients();
	long			GetDestDir		(long sx, long sy, long dx, long dy);			// ��ȡĿ�귽��
	virtual void	GetBeAttackedPoint(long lAttackerX,long lAttackerY,long& lTileX,long& lTileY)
	{
		lTileX = GetTileX();lTileY = GetTileY();
	}
	virtual long	GetAttackerDir(long lAttackerX,long lAttackerY,long ldx,long ldy)
	{return GetDestDir(lAttackerX,lAttackerY,ldx,ldy);}
	void SetPosXY(float fX,float fY );

	virtual void	InitAI(){ /*m_pAI = new CBaseAI; */};

	//##ǿ����dwTimeʱ�����ƶ�������x,y��
	void			ForceMove		( LONG lDestX, LONG lDestY, DWORD dwTime );

	virtual long	GetStopFrame	() {return 0;}						// ��ȡ�ڸ�������ͣ��������
	void			SetMoveable		( BOOL bMoveable = TRUE );	

	virtual StateAI::BaseAI* GetAI() {return m_pAI;}
	//�õ���ǰ��������
	virtual tagSkillID GetDefaultAttackSkillID();


	//////////////////////////////////////////////////////////////////////////
	//	ս��
	//////////////////////////////////////////////////////////////////////////
public:
	// ս����ʽ�����õ��麯��
	virtual DWORD	GetExp() {return 0;}			//	����ֵ
	virtual DWORD	GetExpDead() {return 0;}		//	ɱ���õ��ľ���ֵ
	virtual DWORD	GetHP() {return 0;}
	virtual void	SetHP(DWORD l) {}
	//����MP
	virtual WORD	GetMP() {return 0;}
	virtual void	SetMP(DWORD l) {}

	virtual DWORD	GetMaxHP() {return 0;}	

	virtual WORD	GetMinAtk() {return 0;}
	virtual WORD	GetMaxAtk() {return 0;}
	virtual WORD	GetHit(){return 0;}
	virtual WORD	GetDef() {return 0;}
	virtual WORD	GetDodge() {return 0;}
	virtual WORD	GetAtcSpeed() {return 0;}
	virtual WORD	GetElementResistant(){return 0;}
	virtual WORD	GetSoulResistant() {return 0;}
	virtual WORD	GetHpRecoverSpeed() {return 0;}
	virtual WORD	GetMpRecoverSpeed() {return 0;}
	//##��ȡ��ȭ��ʱ��
	virtual DWORD	GetStrikeOutTime();
	virtual WORD	GetLevel()	{return 1;}
	virtual WORD	GetCCH()	{return 0;}
	virtual WORD	GetAddElementAtk() {return 0;}
	virtual WORD	GetAddSoulAtk()	{return 0;}
	virtual WORD	GetAtcInterval();		// �������

	//##ŭֵ������
	virtual void	IncreaseRp( BOOL bAttack/*##��־�Ƿ��ǹ��������Ϊfalse���Ǳ�����*/, WORD wLoseHP ){}
	//##Ӳֱϵͳ
	DWORD			m_dwStiffenStartedTimeStamp;
	LONG			m_lStiffenCounts;
	virtual DWORD	Stiffen( WORD wLoseHp );

	virtual bool IsAttackAble(CMoveShape* pAttacker/*������*/) = 0;	// �ɷ񹥻�
	//�ж��Ƿ���Թ���
	virtual bool IsAttackAble() {return true;}
	//�ж��Ƿ���Ա�����
	virtual bool IsBeAttackAble() {return true;}

	virtual long GetAlertRange() {return 10;}
	virtual long GetTrackRange() {return 16;}
	virtual WORD GetReAnk()	{return 0;}

	virtual	WORD	GetAttackAvoid()			{return 0;}
	virtual	WORD	GetElementAvoid()			{return 0;}
	virtual	WORD	GetFullMiss()				{return 0;}

	
	void God( bool bIsGod = true )		{ m_bIsGod = bIsGod; }
	bool IsGod()						{ return m_bIsGod; }

    short CanMove()             {return m_sMoveable;}               //�Ƿ����ƶ�
    short CanControlSelf()      {return m_sSelfControl;}            //�ܷ��Լ������Լ�
    short CanReMoveDamage()     {return m_sMoveDamage;}             //�˺��Ƿ��Ƴ�
    short IsCanUseAllSkill()    {return m_sUseSkill;}               //�ܷ񹥻�
    short IsCanBeenAttack()     {return m_sBeenAttack;}             //�ܷ񱻹���
    void  SetBeAttack(short b)    {m_sBeenAttack = b;}              //�ܷ񱻹���
    short GetRandomMove()       {return m_sRandomMove;}				//����ܶ�
    void  SetRandomMove(short b){m_sRandomMove = b;}	

	//!									���ܼӳɿ�ʼ������һ��KEY��
	virtual	long						SkillReinforceBegin(bool isMagic, bool isAttack);
	//!									���ܼӳɽ��������Ч�����棩
	virtual	void						SkillReinforceEnd(long lKey);
	//!									��ü��ܼӳ�ֵ��first������ʱ��仯������second��Ч��ֵ�仯������
	virtual	const pair<float, float>&	GetSkillReinforceAmend(long lKey);
	//!
	virtual float						GetReinforceByEnum(long eValuePos,BOOL bMagic){return 0;}
	virtual bool						ExpendPower(BOOL isMagic){return true;}

protected:
	bool	m_bIsGod;				// �Ƿ����޵�ģʽ
	pair<float, float>				m_FixedReinforce;

protected:
	struct tagScriptTimer
	{
		long lUserData;
		string	strScript;
		bool bLoop;//�Ƿ�ѭ��
	};
	//ע��Ľű���ʱ������
	typedef map<long,tagScriptTimer>	mapScriptTimers;
	typedef map<long,tagScriptTimer>::iterator itST;
	//��ע��Ľű���ʱ��
	mapScriptTimers m_ScriptTimers;

public:
	//����һ���ű���ʱ��
	long RegisterScriptTimer(long lFutureTime,long lInterTime,
							const char* pszScriptName,long lUserData);
	//ע��һ���ű���ʱ��
	long UnRegisterScriptTimer(long lUserData,long lTimerID);
	//�ű���ʱ������
	void OnScripttTimeOut(long lTimerID);
	//������нű���ʱ��
	void ClearScriptTimer();


	virtual CGoods* FindGoods(LONG lContainerId,LONG lPos){return NULL;}
	virtual DWORD FindGoodsByName(LONG lContainerId,const char* strGoodsName){return 0;}
	virtual DWORD FindGoodsByType(LONG lContainerId,int nType){return 0;}
	virtual DWORD FindGoods(LONG lContainerId,LONG lPos,const char* strGoodsName){return 0;}

	//-------------------------------------------------------------
	// ս���Ĵ���
	//-------------------------------------------------------------
	/**## �������MoveShape���ϵ��쳣״̬�б� */
protected:
	typedef map<LONG,stModuParam*>::iterator itModu;
	typedef map<LONG,stModuParam*>	mapModules;
	//struct  tagState
	//{
	//	DWORD dwID;
	//	DWORD dwLv;
	//};

	//״̬������
	map<tagModuleKey,long,tagKeycmp> m_mpStateCount;
	
	//buff��̬����
	mapModules m_BuffParam;
	//debuff��̬����
	mapModules m_DeBuffParam;
	//�������ܶ�̬����
	mapModules m_ActiveSkillParam;
	//�������ܶ�̬����
	mapModules m_PassiveSkillParam;
	//����ʹ�õ��������ܵĶ�̬����
	stModuParam* m_pInstanceParam;
	//����ʹ�õ���������
	stModuParam *m_pInstanceSkill;
	
	
	//��������Ψһ��־��
	long m_ActiveID;

	friend class CPlayerAI;
	DWORD	m_dwIntonateStartTime;			//������ʼʱ��
	DWORD	m_dwIntonateTime;				//����ʱ��
	DWORD	m_dwActStartTime;				//������ʼʱ��
	DWORD   m_dwActTime;					//����ʱ��
    
    //@expter ���Ӽ���CD����ʱ��
    DWORD   m_dwCDStartTime;                //CD��ʼʱ��
    DWORD   m_dwCDControlTime;              //CD����ʱ��

	long	m_lAttackerType;				//����������
	CGUID   m_guidAttacker;					//������guid

	CGUID   m_guidAttackTarget;				//����Ŀ��GUID
	long	m_lAttackTargetType;			//����Ŀ������

	CGUID   m_guidTarget;					//ѡ��Ŀ��GUID
	long	m_lTargetType;					//ѡ��Ŀ������
	CGUID   m_guidKiller;					//��ɱ��guid
	long    m_lKillerType;					//��ɱ������

	hashsetGUID	m_SelectSelfPlayers;		//ѡ���Լ���Ҷ���

public:
	stModuParam  *InitModuParam(DWORD dwSkillID, DWORD dwLevel, DWORD dwGlobleID);					//������̬����
	stModuParam  *GetInstanceSkill(){return m_pInstanceSkill;}										//��ȡ��̬����
	void		 SetInstanceSkill(stModuParam *pInstanceSkill){ m_pInstanceSkill=pInstanceSkill; }	//���ö�̬��̬����
	void		 EndAllActiveSkill();																//���������������
	void		 AddStateCount(stModuParam *pState);												//��״̬������
    void		 SubStateCount(stModuParam *pState);												//��״̬������	
	void		 SetTarget(CGUID guid,long lType);													//���ö������
	void		 GetTarget(CGUID &guidTarget,long &lTargetType);									//��ȡ�������
	void		 SetKillerInfo(CGUID guid,long lType);												//���ô�ɱ�߲���
	void		 GetKillerInfo(CGUID &guidKiller,long &lKillerType);								//��ȡ��ɱ�߲���
	void		 SetAttackTarget(CGUID guid,long lType);											//���ù���Ŀ�����
	void		 GetAttacktarget(CGUID &guid,long &lType);											//��ȡ����Ŀ�����	
	void		 RemoveState(itModu &it);															//ɾ��״̬
	void		 RemoveState(mapModules &mpBaseModule,DWORD dwStateID,DWORD dwLevel,DWORD dwCount); //ɾ��״̬
	void		 ClearAllBuff();																	//������е�״̬
	void		 EndBuffAndDebuff();																//�������е�״̬
	void		 EndDeBuff();																		//�������в���״̬
	void		 EndState(CBaseModule *pBaseModule);												//����״̬
	void		 EndSkill(CBaseModule *pBaseModule);												//��������
	void		 AddNewState(CMoveShape *pUser, DWORD dwStateID, DWORD dwLevel,long lType);			//���Buff
	void		 StartUpState(CMoveShape *pUser, stModuParam *pModuParam, mapModules &mapState);	//����״̬
	void		 StartUpState(mapModules &mapState);												//����״̬
	void		 ClearState(mapModules &mapState);													//���״̬
	void		 EndState(mapModules &mapState);													//����״̬
	void		 DelNewState(DWORD dwStateID, DWORD dwLevel,DWORD dwCount=0);						//ɾ��״̬
    void         ClearState(eSkillAttackType Type,DWORD dwCount=0);                                 //���ָ�����͵ļ���״̬
	void		 EndActiveSkill(long lID,long lKeyID);												//������������
	void		 StartAllBuff();																	//���¿�ʼ���е�״̬
	void		 RemoveActive(CBaseModule* pActiveSkill);											//�Ƴ���������
	void		 RemoveBaseModule(stModuParam* pModuParam);											//�Ƴ���̬����
	void		 ReleaseBaseModule(stModuParam* pModuParam,mapModules &mpBaseModule);				//�ͷŶ�̬����
	void		 SetAttacker(long lType,const CGUID guid);											//���ù����ߵ����ͺ�guid
	bool		 EndPassiveSkill(DWORD dwID,DWORD dwLv);											//������������
	long		 GetAttackerType() {return m_lAttackerType;}										//��ȡ����������
	long		 GetStateCount(stModuParam *pState);												//��ȡ״̬����
    bool         GetStateCount(eSkillAttackType Type);                                              //��ȡ״̬��������
	CBaseModule *GetActiveSkill(long lActiveID);													//��ȡ��������ʵ��
	CGUID		&GetAttackerGuid()   {return m_guidAttacker;}										//��ȡ������guid
	mapModules	 GetBuff()			{return m_BuffParam;}											//��ȡBUFF�Ķ�̬����	
	virtual void EndBuff(DWORD dwStateID);															//����ID����״̬

	void	AddSelectSelfPlayer(const CGUID& PlayerID);
	void	RemoveSelectSelfPlayer(const CGUID& PlayerID);
	hashsetGUID& GetSelectSelfPlayer()	{return m_SelectSelfPlayers;}
	
	
	



	


	
	
	bool CooldownPast(long lSkilllD)	{return true;}
	virtual void SetCommonCooldown(long lID, DWORD dwTime){}
	virtual void SetSkillCooldown(long lID,DWORD dwTime){}


	//##��ʼ��������ļ�����Ϣ
	virtual void	InitSkills				();
	virtual void	SetIntonateTime(DWORD dwStartTime, DWORD dwTime){m_dwIntonateStartTime=dwStartTime,m_dwIntonateTime=dwTime;}
	virtual void	SetActTime(DWORD dwActStartTime,DWORD dwAtcTime){m_dwActStartTime=dwActStartTime,m_dwActTime=dwAtcTime;}
    virtual void    SetCDTime(DWORD dwCDStartTime,DWORD  dwCDTIme) {m_dwCDStartTime=dwCDStartTime,m_dwCDControlTime=dwCDTIme;}
	virtual BOOL	IsInAct();								//����TRUE��ʾ����������,FALSE��ʾ�����ѹ�
	virtual long	GetActStartTime(){return m_dwActStartTime;}
	virtual long    GetActTime(){return m_dwActTime;}
    virtual bool    IsCDTime()  {DWORD dwCurrTime = timeGetTime(); return dwCurrTime < (m_dwCDStartTime+m_dwCDControlTime) ;}

	void						ClearSkills				();
	void						ClearPassiveSkill		();

	void						OnAction				( tagAction a );

	BOOL AddSkill( const char* strName, LONG LLevel = 1 );		// ��Ӽ���
	BOOL AddSkill( tagSkillID tID, LONG lLevel = 1 );
	BOOL DelSkill( const char* strName );						// ɾ������
	virtual BOOL DelSkill( long lSkillID,long lSkillLv);
	BOOL SetSkillLevel(const char* strName, LONG lLevel);		// ���ü��ܵȼ�
	LONG CheckSkill( long lID );									// ����Ƿ���ָ�����ܣ����صȼ� (0����û��)
	LONG CheckSkill( const char* strName );						// ����Ƿ���ָ�����ܣ�y���صȼ� (0����û��)

	virtual void AddSkill(long id,long lv){}
	//���buff
	int CheckBuff(int lID,int lLvl)	{return 0;}
	//���skill
	int CheckSkill(int lID,int lLvl)	{return 0;}


	//##���麯��ʵ�������ɼ�����AI��ѯ��ʼ��ʱ����ã�����ʱ�򲻵��á�
	//##���صĲ���ֵ�����ж��Ƿ���Ҫֹͣ�ü��ܡ�TRUE��ʾ����FALSE��ʾ������
	virtual BOOL				OnBeginSkill( DWORD dwID,DWORD dwLv );

	//##���麯��ʵ�������ɼ�����AI��ѯֹͣ��ʱ����ã�����ʱ�򲻵��á�
	virtual void				OnEndSkill				() {}

	struct tagDamage
	{
		tagDamageType	dtType;
		WORD			wValue;
	};

	//��Ʒ��
public:
	virtual int	  CheckGoods(const char *goodoriname)	    {return 0;}//�����Ʒ�Ƿ��и���Ʒ��������Ʒ�ĸ���
    
    //�츳״̬
public:
    virtual long  GetSkillValue(long skill,long lvl,std::string name,long xPos=0,long yPos=0);
    virtual void  AddGeniusStates(long skillid,CMoveShape* pDest,long Gid){}

public:
	//##�����߹���������ʱ��������Ĺ���ǿ�Ⱥ����͵���Ϣ���ݸ�
	//##���ط��������ɷ�����������յķ��������
	virtual long				OnBeenAttacked			( CMoveShape *pAttacker,long lHp,long lDamageResult=0);
	virtual void				OnBeenKilled(CMoveShape* pAttacker);

	virtual bool				OnIncreaseHp			(long &lHp, CMoveShape *firer );

	//��ʹ�����漼�ܴ���
	virtual void				OnBeenAddAppendSkill	(CMoveShape *pAttacker);
	

	virtual void				OnChangeStates			();



	void						StopAllSkills			();
	void						StopPassiveSkills		();

	void						StopSkills				(long lID,long lLv,long lType);
	void						StopActiveSkill			(long lID,long lLv);
	void						StopPassiveSkill		(long lID,long lLv);
	

	//�ͷ��������ܲ���
	void						ClearActiveSkills			();
	void						ClearAllStates			(bool bIsDead = false);

	void						StartAllStates			(bool bIsRelive = false);

	//##�����ٻ��޻����л�����������.
	virtual void				OnEnterRegion			(bool bIsRelive = false);
	virtual void				OnExitRegion			(){}


public:
	virtual void				OnRelive(long lType,long lRecoverRatio=50){}

	//##��Ⲣ���𹥻���ָ��Shader����ĳ��Ŀ�꣬Ŀ������ڿɹ�����Χ��
	
	long						BeginAttack		( long lX, long lY );
	//##�ж������Ƿ�����
	BOOL						IsDied			();
	
	//##����������Ĵ���
	virtual void				OnDied			(){}
	//��λ�øı�֮�󴥷�
	virtual void OnPosChanged(){return;}

	//##���������Ҫ��Ϊ���ж�ɱ����
	virtual void				OnBeenMurdered	( LONG lMurdererType, const CGUID& MurdererID ) {}
	virtual void				OnBeenHurted	( LONG lAttackerType, const CGUID& AttackerID ) {}
	//ɾ����Ʒ
	virtual BOOL DelGoods(LONG lContainerId,LONG lPos){return TRUE;}
	virtual BOOL DelGoods(char* strOriginName,int num){return TRUE;}
	virtual DWORD DeleteGoods(CS2CContainerObjectMove::PLAYER_EXTEND_ID pei,eGoodsBaseType baseType,DWORD dwWeaponType,DWORD dwNum){return 0;}
	//! ɾ����Ʒ��֪ͨ���
	virtual void DelGoodsAndSendMsg(long lContainerId, long lPos){}

	//##��ȡ������ǿ�ȶ�Ŀ����˺�������ֵ��
	virtual FLOAT				GetWeaponModifier( LONG lTargetLevel ){ return 1.0f; }
	enum tagDeathType
	{
		DT_BEEN_KILLED			= 1,
		DT_SELF_BLASTING		= 2,
		DT_EVANISH				= 3,
	};

	virtual void Evanish();
	///////////////////////////////////////////////////////////////
	//	ɱ���Լ����ˣ������һ���ģ�
	///////////////////////////////////////////////////////////////
protected:
	tagAttackInformation	m_KilledMeAttackInfo;


public:
	void SetKilledMeAttackInfo(LONG lType, CGUID guid);
	const tagAttackInformation& GetKilledMeAttackInfo()const {return m_KilledMeAttackInfo;}

	long GetKilledMeType()			{return m_KilledMeAttackInfo.lAttackerType;}
	const CGUID& GetKilledMeID()			{return m_KilledMeAttackInfo.AttackerID;}


	bool AddBuffToByteArray	(vector<BYTE>* pByteArray);


	virtual bool CodeToDataBlock_ForClient( DBWriteSet& setWriteDB, bool bExData = TRUE );
	bool CodeBuffToDataBlock_ForClient( DBWriteSet& setWriteDB);
	bool CodeDeBuffToDataBlocky_ForClient( DBWriteSet& setWriteDB);
	bool CodeActSkillToDataBlock_ForClient( DBWriteSet& setWriteDB);
	bool CodeOnOffSkillToDataBlock_ForClient( DBWriteSet& setWriteDB);
	bool CodeBuffToDataBlock	(DBWriteSet& setWriteDB);
	bool DecodeBuffFromDataBlock(DBReadSet& setReadDB);


	//-------------------------------------------------------------
	//	һЩ��ս����ص�״̬�麯��
	//-------------------------------------------------------------
	virtual void				EnterCriminalState	()	{}
	virtual void				EnterResidentState	()  {}
//	virtual void				EnterCombatState	()	= 0;
//	virtual void				EnterPeaceState		()	= 0;
	virtual void				ChangeStateTo		(CShape::eSTATE);
	virtual void				UpdateCurrentState	()	= 0;

	//-------------------------------------------------------------
	// ������������
	//-------------------------------------------------------------

	//##���2��MoveShape֮���Ƿ�����档
	BOOL						IsFaceTo			( CMoveShape* pShape );
	//BOOL						DropGoods			( CGoods* pGoods, LONG lX, LONG lY );

	//---------------------------------------------------------------
	//  �ɼ����
	//---------------------------------------------------------------
protected:
	//��Ӧ�ɼ�������
	string m_strCollectionName;
	//�ɼ���id
	DWORD	m_dwCollectionID;
	//�ܱ��ɼ��Ĵ���
	DWORD	m_dwCollectTimes;
	//�Ƿ����ڱ��ɼ�
	bool	m_bIsBeenCollect;
	//�Ƿ��ܱ��ɼ�
	bool	m_bCanBeenCollect;
public:
	const string& GetCollectionName()			{return m_strCollectionName;}
	void	SetCollectionName(const string & l)	{m_strCollectionName=l;}
	DWORD	GetCollectionID()				{return m_dwCollectionID;}
	void	SetCollectionID(DWORD l);//		{m_dwCollectionID=l;}
	DWORD	GetCollectionTimes()			{return m_dwCollectTimes;}
	void	SetCollectionTimes(DWORD l);//		{m_dwCollectTimes=l;}
	bool	GetIsBeenCollect()				{return m_bIsBeenCollect;}
	void	SetIsBeenCollect(bool l);//			{m_bIsBeenCollect=l;}
	bool	GetCanBeenCollect()				{return m_bCanBeenCollect;}
	void	SetCanBeenCollect(bool l);//		{m_bCanBeenCollect=l;}
public:
	BOOL	BeenCollected();
	virtual BOOL	BeenCollectedAll(){return FALSE;}
	virtual void OnDiedEvent(){}

	friend class CBaseAI;
	friend class CMonsterAI;
	friend class CGuardWithSword;
	friend class CCityGuardWithSword;
	friend class CVilCouGuardWithSword;
	friend class COpPosition;
	friend class CColony;

	// ���ؽӿ�
	//virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const char* value);
	//virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray=0, double value=0);
	//virtual const char* GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	//virtual double GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray);

	//virtual int SetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const CGUID& guid);
	//virtual const CGUID& GetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);

	//virtual CVariableList* GetVariableList(void);
public:
	bool SetScriptVarValue( Script *script, const char *varName, const char *val );
	bool SetScriptVarValue( Script *script, const char *varName, double val, long index = 0 );
	bool SetScriptVarValue( Script *script, const char *varName, const CGUID &val );

	const char *GetScriptVarValueStr( Script *script, const char *varName );
	double GetScriptVarValueNum( Script *script, const char *varName, long index = 0 );
	const CGUID &GetScriptVarValueGuid( Script *script, const char *varName );

	VariableList *GetVariableList();
protected:
	VariableList *m_VarList;
};
