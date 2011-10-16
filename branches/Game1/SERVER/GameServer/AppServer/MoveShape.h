#pragma once
#include "shape.h"
#include "..\..\..\Nets\MySocket.h"
#include <queue>
#include <vector>
#include <set>
#include <list>

#include "ai\baseai.h"
#include "Goods\CGoods.h"
#include "BuffSkill/NewSkillFactory.h"
#include "BuffSkill/AttackPower.h"
#include "Message Packaging\CS2CContainerObjectMove.h"
using namespace std;

class CRegion;
class CPlayer;
class CBaseAI;
class CPlayerAI;
class CBaseModule;
class COpPosition;


class CScript;

class CMoveShape :
	public CShape
{
public:
	CMoveShape(void);
	virtual ~CMoveShape(void);

	virtual void SetAction(WORD l){}

protected:

	CBaseAI*					m_pAI;
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
	//��ӱ�������������
	virtual void  AddAttributeToDirty(string& attrName){return;}


	virtual BOOL  SetBoolAttribute(const std::string& attrName, long value);
	virtual BOOL  SetBoolAttribute(const std::string &attrName, BOOL b);

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
	long    DirtyShapeAttrDataSize(void) { return m_mapShapeDirtyAttrData.size(); }



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
	VOID UpdateAbnormality();			// �쳣״̬����
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
private:
	bool						m_bRun;							//�ƶ���ʽ�Ƿ�����
	bool						m_bMoveable;
	LONG						m_lMoveableCount;

	bool						m_bUseFighterSkill;				//ʹ��ս�����ܱ�־��
	bool						m_bUseRabbSkill;				//ʹ�÷�ʦ���ܱ�־��
	bool						m_bUseItem;						//ʹ�õ��ߵı�־��
	bool						m_bAntiInvisibleable;			//�Ƿ�����
	bool						m_bInvisible;					//���ɼ�״̬
	bool						m_bHide;						//����		
	bool						m_bPretendDead;					//װ��
	bool						m_bInstruction;					//ʹ��ָ���־��
	bool						m_bUnAttack;					//����������־��
	bool						m_bAddHp;						//Hp�������ӵı�־��
	long						m_lChangeModelType;				//��������
	long						m_lChangeModelID;				//����id
	bool						m_bForceAttack;					//ǿ�ƹ���
	bool						m_bRandomMove;					//����ܶ�
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
	
	virtual VOID				OnCannotMove();
	virtual VOID				SetPosition( LONG lX, LONG lY );
	virtual VOID				OnMove(float fNewX, float fNewY);
	virtual VOID				OnStopMove();
	virtual VOID				SetSlipPos( LONG lX, LONG lY,float fSpeed,float fDir);

	virtual void OnQuestMoveStep(float fCurX,float fCurY,float fDestX,float fDestY){return;}
	virtual void OnQuestStopMove(float fCurX,float fCurY,short lDir){return;}

	bool						GetIsRide()	{return m_bRide;}
	void						SetIsRide(bool bRide){m_bRide = bRide;}

	bool						GetIsRun()	{return m_bRun;}
	void						SetIsRun(bool bRun){m_bRun = bRun;}

	virtual bool				GetForceAttack(){return m_bForceAttack;}            //ǿ�ƹ���
	void						SetForceAttack(bool b){m_bForceAttack = b;}

	bool						GetRandomMove(){return m_bRandomMove;}				//����ܶ�
	void						SetRandomMove(bool b){m_bRandomMove = b;}	

	virtual const CGUID&		GetBefoolID() {return NULL_GUID;}//return m_BefoolID;}	
	virtual VOID				SetBefoolID(const CGUID& guid){m_BefoolID=guid;}					//������Ū�Լ���id

public:
	void			SetUseFightSkill(bool b);					//����ʹ��սʿ���ܱ�־��
	bool			GetUseFightSkill(){return m_bUseFighterSkill;}

	void			SetUseRabbSkill(bool b);					//����ʹ�÷�ʦ���ܱ�־��
	bool			GetUseRabbSkill(){return m_bUseRabbSkill;}

	void			SetUseItem(bool b);							//����ʹ�õ��߱�־��
	bool			GetUseItem(){return m_bUseItem;}

	void			SetUseInstruction(bool b){m_bInstruction = b;}//���ʹ��ָ���־��
	bool			GetInstruction(){return m_bInstruction;}

	void		    SetUnAttack(bool b){m_bUnAttack = b;}
	bool			GetUnAttack(){return m_bUnAttack;}			 //���ܱ�����

	void			SetAddHp(bool b){m_bAddHp = b;}
	bool			GetAddHp(){return m_bAddHp;}

	void			SetChangeModelType(long l){m_lChangeModelType = l;}	//�ı�ģ������
	long			GetChangeModelType(){return m_lChangeModelType;}

	void			SetChangeModelID(long l){m_lChangeModelID = l;}		//�ı�ģ��id
	long			GetChangeModelID(){return m_lChangeModelID;}

	virtual VOID	SetAntiInvisibleable(bool l){m_bAntiInvisibleable=l;}			//�����Ƿ�����
	bool			GetAntiInvisibleable(){return m_bAntiInvisibleable;	}

	virtual VOID	SetInvisibleState(bool l){m_bInvisible=l;}			//�����Ƿ񲻿ɼ�״̬
	bool			GetInvisibleState(){return m_bInvisible;}

	virtual VOID	SetPretendDeadState(bool l){m_bPretendDead=l;}			//�����Ƿ�װ��
	bool			GetPretendDeadState(){return m_bPretendDead;}	
	virtual VOID	SetHide(bool l){m_bHide=l;}								//����
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

	virtual void	InitAI(){};

	//##ǿ����dwTimeʱ�����ƶ�������x,y��
	VOID			ForceMove		( LONG lDestX, LONG lDestY, DWORD dwTime );

	virtual long	GetStopFrame	() {return 0;}						// ��ȡ�ڸ�������ͣ��������
	VOID			SetMoveable		( BOOL bMoveable = TRUE );	

	virtual CBaseAI* GetAI() {return m_pAI;}



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
	
	virtual WORD	GetLevel()	{return 1;}
	virtual WORD	GetCCH()	{return 0;}
	virtual WORD	GetAddElementAtk() {return 0;}
	virtual WORD	GetAddSoulAtk()	{return 0;}


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

	virtual bool CanMove()	{return m_bMoveable;}	//�Ƿ����ƶ�	

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
	void		 EndActiveSkill(long lID,long lKeyID);												//������������
	void		 StartAllBuff();																	//���¿�ʼ���е�״̬
	void		 RemoveActive(CBaseModule* pActiveSkill);											//�Ƴ���������
	void		 RemoveBaseModule(stModuParam* pModuParam);											//�Ƴ���̬����
	void		 ReleaseBaseModule(stModuParam* pModuParam,mapModules &mpBaseModule);				//�ͷŶ�̬����
	void		 SetAttacker(long lType,const CGUID guid);											//���ù����ߵ����ͺ�guid
	bool		 EndPassiveSkill(DWORD dwID,DWORD dwLv);											//������������
	long		 GetAttackerType() {return m_lAttackerType;}										//��ȡ����������
	long		 GetStateCount(stModuParam *pState);												//��ȡ״̬����
	CBaseModule *GetActiveSkill(long lActiveID);													//��ȡ��������ʵ��
	CGUID		&GetAttackerGuid()   {return m_guidAttacker;}										//��ȡ������guid
	mapModules	 GetBuff()			{return m_BuffParam;}											//��ȡBUFF�Ķ�̬����	
	virtual void EndBuff(DWORD dwStateID);															//����ID����״̬

	void	AddSelectSelfPlayer(const CGUID& PlayerID);
	void	RemoveSelectSelfPlayer(const CGUID& PlayerID);
	hashsetGUID& GetSelectSelfPlayer()	{return m_SelectSelfPlayers;}	
	//��ȴʱ�䴦��
	bool		 CooldownPast(long lSkilllD)	{return true;}
	virtual void SetCommonCooldown(long lID, DWORD dwTime){}
	virtual void SetSkillCooldown(long lID,DWORD dwTime){}


	//##��ʼ��������ļ�����Ϣ
	virtual VOID	InitSkills				();
	virtual void	SetIntonateTime(DWORD dwStartTime, DWORD dwTime){m_dwIntonateStartTime=dwStartTime,m_dwIntonateTime=dwTime;}
	virtual void	SetActTime(DWORD dwActStartTime,DWORD dwAtcTime){m_dwActStartTime=dwActStartTime,m_dwActTime=dwAtcTime;}
	virtual BOOL	IsInAct();								//����TRUE��ʾ����������,FALSE��ʾ�����ѹ�
	virtual long	GetActStartTime(){return m_dwActStartTime;}
	virtual long    GetActTime(){return m_dwActTime;}


	VOID						ClearSkills				();
	VOID						ClearPassiveSkill		();

	BOOL AddSkill( const char* strName, LONG LLevel = 1 );		// ��Ӽ���
	BOOL AddSkill( tagSkillID tID, LONG lLevel = 1 );
							// ɾ������
	virtual BOOL DelSkill( long lSkillID,long lSkillLv);
	BOOL SetSkillLevel(const char* strName, LONG lLevel);		// ���ü��ܵȼ�
	
	virtual void AddSkill(long id,long lv){}
	//���buff
	int CheckBuff(int lID,int lLvl)	{return 0;}
	//���skill



	//##���麯��ʵ�������ɼ�����AI��ѯ��ʼ��ʱ����ã�����ʱ�򲻵��á�
	//##���صĲ���ֵ�����ж��Ƿ���Ҫֹͣ�ü��ܡ�TRUE��ʾ����FALSE��ʾ������
	virtual BOOL				OnBeginSkill( DWORD dwID,DWORD dwLv );

	//##���麯��ʵ�������ɼ�����AI��ѯֹͣ��ʱ����ã�����ʱ�򲻵��á�
	virtual VOID				OnEndSkill				() {}

	//��Ʒ��
public:
	virtual int	 CheckGoods(const char *goodoriname)	{return 0;}//�����Ʒ�Ƿ��и���Ʒ��������Ʒ�ĸ���

public:
	//##�����߹���������ʱ��������Ĺ���ǿ�Ⱥ����͵���Ϣ���ݸ�
	//##���ط��������ɷ�����������յķ��������
	virtual long				OnBeenAttacked			( CMoveShape *pAttacker,long lHp,long lDamageResult=0);
	virtual void				OnBeenKilled(CMoveShape* pAttacker);

	virtual bool				OnIncreaseHp			(long &lHp);

	//��ʹ�����漼�ܴ���
	virtual VOID				OnBeenAddAppendSkill	(CMoveShape *pAttacker);
	

	virtual VOID				OnChangeStates			();



	VOID						StopAllSkills			();
	VOID						StopPassiveSkills		();

	VOID						StopSkills				(long lID,long lLv,long lType);
	VOID						StopActiveSkill			(long lID,long lLv);
	VOID						StopPassiveSkill		(long lID,long lLv);
	

	//�ͷ��������ܲ���
	VOID						ClearActiveSkills			();
	VOID						ClearAllStates			(bool bIsDead = false);

	VOID						StartAllStates			(bool bIsRelive = false);

	//##�����ٻ��޻����л�����������.
	virtual VOID				OnEnterRegion			(bool bIsRelive = false);
	virtual VOID				OnExitRegion			(){}

	///////////////////////////////////////////////////////////////
	//	Shapeս������
	///////////////////////////////////////////////////////////////
protected:
	LONG						m_lCanFightCount;	//##������
	bool						m_bCanFight;		//##�ܷ��������
	bool						m_bAttackAble;		//##�ɷ񹥻�
	

public:
	virtual void				SetAttackAble	( bool b )			{m_bAttackAble = b;}
	virtual bool				GetAttackAble	()					{ return m_bAttackAble; }
	virtual void				OnRelive(long lType,long lRecoverRatio=50){}

	VOID						SetFightable	( bool b );
	bool						IsFightable		() { return m_bCanFight; }
	


	//##�ж������Ƿ�����
	BOOL						IsDied			();
	
	//##����������Ĵ���
	virtual void				OnDied			(){}
	//��λ�øı�֮�󴥷�
	virtual void OnPosChanged(){return;}

	//##���������Ҫ��Ϊ���ж�ɱ����
	virtual VOID				OnBeenMurdered	( LONG lMurdererType, const CGUID& MurdererID ) {}
	virtual VOID				OnBeenHurted	( LONG lAttackerType, const CGUID& AttackerID ) {}
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

	///////////////////////////////////////////////////////////////
	//	ɱ���Լ����ˣ������һ���ģ�
	///////////////////////////////////////////////////////////////
protected:
	tagAttackInformation	m_KilledMeAttackInfo;


public:
	VOID SetKilledMeAttackInfo(LONG lType, CGUID guid);
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
	virtual VOID				EnterCriminalState	()	{}
	virtual VOID				EnterResidentState	()  {}

	virtual VOID				ChangeStateTo		(CShape::eSTATE);
	virtual VOID				UpdateCurrentState	()	= 0;

	//-------------------------------------------------------------
	// ������������
	//-------------------------------------------------------------

	//##���2��MoveShape֮���Ƿ�����档
	BOOL						IsFaceTo			( CMoveShape* pShape );	
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
	VOID	SetCollectionName(const string & l)	{m_strCollectionName=l;}
	DWORD	GetCollectionID()				{return m_dwCollectionID;}
	VOID	SetCollectionID(DWORD l);
	DWORD	GetCollectionTimes()			{return m_dwCollectTimes;}
	VOID	SetCollectionTimes(DWORD l);
	bool	GetIsBeenCollect()				{return m_bIsBeenCollect;}
	VOID	SetIsBeenCollect(bool l);
	bool	GetCanBeenCollect()				{return m_bCanBeenCollect;}
	VOID	SetCanBeenCollect(bool l);
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
	virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const char* value);
	virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray=0, double value=0);
	virtual const char* GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	virtual double GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray);

	virtual int SetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const CGUID& guid);
	virtual const CGUID& GetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);

	virtual CVariableList* GetVariableList(void);


};
