// Fox@20081118----------------------------------------------
// File:    Pet.h
// Brief:   CPet�����˳�������Լ�����. ���нӿ���δ���ǿ���������.
//			��Ҫ�漰�����ʼ�����ͷ�, �����¼�������Ӧ.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Nov.18, 2008
// Fox@20081118----------------------------------------------

#pragma once
#include "moveshape.h"
#include "PetCtrl.h"

#define UNI_CONF \
	GetInst(CPetCtrl).GetPetConfig()
#define MY_CONF \
	GetInst(CPetCtrl).GetPetsConfig()

class CGoods;
class CPlayer;
class CMonster;

class CPet :
	public CMoveShape
{
public:
	CPet(void);
	~CPet(void);

	friend class CPetCtrl;

	const tagPetDiff& GetDiffPro(void) { return m_DiffAttrs; }	// ��������
	const tagPetTemp& GetTempPro(void) { return m_TempAttrs; }	// ��ʱ����

public:
	// ��Ϣ��Ӧ
	virtual void OnMessage(CMessage*);					// ������Ϣ��Ӧ
	virtual long OnTimeOut(DWORD timerid,
		DWORD curtime,
		const void* var);								// ��ʱ��Ӧ
	virtual long Proc(eEventType type,
		const tagProcVar* pVar);						// ����ʵ�����Ϣ����

	void AddMoveMsg(float fNewX, float fNewY);			// ���һ���ƶ���Ϣ
	bool CheckMoveStep(float fCurX, float fCurY);		// ��֤ÿһ�����ƶ�
	bool CheckMoveSpeedValidate(void);					// �ƶ��ٶ���֤
	void ResetMoveValiData(bool bAll = false);			// �����������
	virtual void InitAI(void);							// ��ʼ��AI
	virtual void InitSkills(void);						// ��ʼ������
	virtual void OnQuestMoveStep(float fCurX,
		float fCurY,
		float fDestX,
		float fDestY);									// �����ƶ�
	virtual void OnQuestStopMove(float fCurX,
		float fCurY,
		short lDir);									// ����ֹͣ�ƶ�
	virtual void OnCannotMove(void);
	virtual long OnBeenAttacked(CMoveShape* pAttacker,
		long lHp);										// ���ﱻ����
	virtual void OnDied(void);							// ��������
	virtual bool IsAttackAble(CMoveShape* pAttacker);	// �ɷ񹥻�
	virtual void UpdateCurrentState(void);				// ����״̬
	virtual void ChangeStateTo(CShape::eSTATE eState);	// �ı�״̬
	virtual bool SetAttribute(std::string& attrName,
		long value);
	//virtual bool SetAttribute(char* attrName,
	//	long value);
	virtual long GetAttribute(std::string attrName);
	virtual bool ChangeAttribute(std::string& attrName,
		long value);
	virtual long UpdateAttributeToClient(bool bAll);	// ��������, true������Χ���, false����������
	long  UpdateAttribute(BOOL bImmediately=0);
	long  OnUpdateAttributeTimeOut();

	virtual void OnSpeedChanged(void);
	virtual void SetPosition(LONG lX, LONG lY);
	virtual void SetSlipPos(LONG lX, LONG lY, float fSpeed, float fDir);
	virtual bool CodeToDataBlock_ForClient(DBWriteSet& setWriteDB,
		bool bExData = true);							// GS==>Client����
	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB,
		bool bExData = true);							// GS==>WS����
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,
		bool bExData = true);							// WS==>GS����
	virtual CVariableList* GetVariableList(void)		{ return m_pVariableList; }
	virtual DWORD GetExp()		{ return m_DiffAttrs.lExp; }
	virtual DWORD GetExpDead()	{ return 0; }			// ɱ���õ��ľ���ֵ
	virtual void SetHP(DWORD l) { SetAttr("Hp", l); }
	virtual void SetMP(DWORD l) { SetAttr("Mp", l); }
	virtual DWORD GetHP()		{ return m_DiffAttrs.lHp ? m_DiffAttrs.lHp : (GetState()!=STATE_DIED); }
	virtual WORD GetMP()		{ return m_DiffAttrs.lMp; }
	virtual DWORD GetMaxHP()	{ return m_TempAttrs.lMaxHp + m_BaseAttrs.lMaxHp; }	
	virtual DWORD GetMaxMP()	{ return m_TempAttrs.lMaxMp + m_BaseAttrs.lMaxMp; }	
	virtual WORD GetMinAtk()	{ return 0; }
	virtual WORD GetMaxAtk()	{ return 0; }
	virtual WORD GetHit()		{ return 0; }
	virtual WORD GetDef()		{ return 0; }
	virtual WORD GetAtcSpeed()	{ return 0; }
	virtual WORD GetHpRecoverSpeed()	{ return m_TempAttrs.lHpRecoverSpeed; }
	virtual WORD GetMpRecoverSpeed()	{ return m_TempAttrs.lMpRecoverSpeed; }
	virtual WORD SetHpRecoverSpeed()	{ return m_TempAttrs.lHpRecoverSpeed; }
	virtual WORD SetMpRecoverSpeed()	{ return m_TempAttrs.lMpRecoverSpeed; }

	virtual CMoveShape* GetHost(void);
	virtual long AddActiveSkill(DWORD dwSkillID,
		DWORD dwLevel,
		CMoveShape *pTarget = NULL,
		long lGoodsIndex = 0,
		long lContainerID = 0,
		long lPos = 0);
	virtual void SetSkillCooldown(long lID, DWORD dwTime);
	virtual WORD GetLevel(void) { return m_DiffAttrs.lLevel; }	// ��ȡ����ȼ�
	virtual bool IsVisible(OBJECT_TYPE eType,
		const CGUID& rID);								// �Ƿ�ɱ�GUIDΪrID��MoveShape����

	long InitialSkills(void);							// ����ʱ��ʼ������
	bool UseSkill(tagSkill* pSkill,
		CMoveShape* pTarget = NULL);					// ʹ�ü���
	void IncExp(long lDelta);							// ��������
	bool ModifySkill(long lID,
		long lLevel,
		long lPos);										// ��������
	tagSkill* GetSkill(long lPos);						// ��ȡ����
	void SetSkillHotKey(long lPos, long lActPos);		// ���ó��＼�ܿ�ݼ�
	void SetHost(CPlayer* pPlayer);
	long GetPetType(void) { return m_BaseAttrs.lPetType; }
	long GetPetIndex(void) { return m_BaseAttrs.lIndex; }
	void SetLevel(long lLevel);							// ���ó���ȼ�
	bool ModifyPos(INIT_TYPE lInitType);				// ��������
	void SetMoveType(long lType);						// ��������ģʽ
	void SetFightType(long lType);						// ���ù���ģʽ
	long GetMoveType(void);								// ��ȡ����ģʽ
	long GetFightType(void);							// ��ȡ����ģʽ
	long CanBeSelected(void) { return m_BaseAttrs.lSelAtk & SAT_CAN_BESEL; }
	long CanBeAttacked(void) { return m_BaseAttrs.lSelAtk & SAT_CAN_BEATK; }
	long CanFight(void) { return m_BaseAttrs.lSelAtk & SAT_CAN_FIGHT; }
	bool NeedBeSaved(void)
	{
		return ( m_BaseAttrs.lPetType != PET_TYPE_TRAP &&
			m_BaseAttrs.lPetType != PET_TYPE_ORNA);
	}

	bool IsActive(void)
	{
		return (m_DiffAttrs.lActive > 0) ? true : false;
	}
	bool IsHangUp(void)
	{
		return (GetState() == STATE_HANGUP);
	}
	bool OnTrapSpring(void);							// ���崥��
	bool DelFromRegion(long lOpType);					// �ĳ����Єh��
	void SetTarget(CMoveShape* pTarget = NULL);			// ���ó��ﵱǰĿ��
	void SaveSkillSetting(long lPos,
		long lActPos,
		bool bAuto);									// ����ͻ��˼�������

private:
	// ��ʼ�����ͷ�
	void InitialOrigAttrs(long lOldLevel,
		long lCurLevel);								// ��ʼ��ԭʼ����
	bool InitialBaseAttrs(const tagPetBase& rPet,
		long lInitType);								// ��ʼ����������
	void InitialTempAttrs(void);						// ��ʼ����ʱ����
	bool InitialDiffAttrs(long lInitType);				// ��ʼ������Ĵ��̻��з��ı�������
	bool InitialByTarget(CMonster* pTarget);			// ͨ�������ʼ������
	bool InitialByHost(INIT_TYPE lInitType);			// ��ʼ����������
	bool AddToRegion(void);								// ��ӵ�������
	void InitNameValueMap(void);						// ��ʼ�������ֶ�

	// �¼�����
	void RegEvent(PET_EVENT_TYPE eType,
		long lDelayTime = 0,
		long lIntervalTime = 0);						// ע���¼�
	bool RegShadeEvent(void);							// ע�ὥ���¼�
	bool RegAutoRenew(void);							// ע���Զ��ָ�HP��MP��
	const CMoveShape* GetTarget(void);					// ���س��ﵱǰĿ��

	// �¼���Ӧ
	void OnHPRenew(void);								// 1) ս��, �ٻ��޵ĵ�HP
	void OnMPRenew(void);								// 2) ����, ս��, �ٻ��޵�MP
	void OnGorgeRenew(void);							// 3) ����, ս��ı�ʳ��
	void OnGorgeUpdate(long lOld);						// ��ʳ������, �����ӦӰ������
	void OnLoyaltyRenew(void);							// 4) ս����ҳ϶�
	void OnLifeEnd(void);								// 5) ���������ս�
	void OnActive(void);								// �������
	void OnHouse(long lOpType);							// �ղس���, bActiveΪ��ұ���ʱ�Ƿ񼤻�����־
	void OnLevelUpdate(void);							// ��������

	// ����
	//bool IsSkillCooldown(long lID);						// �����Ƿ���ȴ
	bool HasSkill(long lID);							// �Ƿ�ӵ�м���
	bool ModifySkill(tagSkill& rSkill);					// ��������
	long AppendGeniusSkill(void);						// ׷��һ���츳����
	long AddGeniusSkill(void);							// ����츳����
	long AddStudySkill(void);							// ��Ӳ�ۼ���
	long AddConfigSkill(void);							// ������ü��ܣ�
	long OpenSkillHole(long lType = 0, long lPos = 0);	// �������ܲ�
	void CloseSkillHole(long lPos);						// �رռ��ܲ�
	CNewSkill* CanModifySkill(tagSkill& rSkill);		// ��鼼����������
	bool UpdateSkill(long lID, long lLevel);			// ���¼���
	tagSkill* AddSkill(tagSkill& rSkill);				// ��Ӽ���
	bool AddSkill(CGoods* pGoods, long lPos);			// ��Ӽ���
	long CanUseSkill(tagSkill* pSkill,
		CMoveShape* pTarget,
		long& lSkillType);								// ��鼼��ʹ������
	void StudySkillScript(long lID, long lLevel);		// ��������ѧϰ�ű�
	void PetRunScript(const char *szScriptName);		// �������нű�

	long GetAttr(const char* szName);
	long SetAttr(const char* szName, long lVal);
	long ChgAttr(const char* szName, long lDelta);
	void AddDirtyAttr(const char* szName);				// ���������ݱ��

	void CodeChgAttr(DBWriteSet& setWriteDB,
		short sEnum,
		char cType,
		long lValue);									// ��������
	void CodeChgAttrs(DBWriteSet& setWriteDB,
		set<string>& rAttrs);							// ����������

	long CalcGorgeLevel(void);							// ���㱥ʳ�ȵȼ�
	long Feed(long lSeconds, long lDelta);				// ιʳ
	long GetIndexID(void) { return m_BaseAttrs.lIndex; }
	void SetIndexID(long lIndexID) { m_BaseAttrs.lIndex = m_DiffAttrs.lIndex = lIndexID; }
	void SetPetType(long lType) { m_BaseAttrs.lPetType = lType; }
	void SetActiveAttr(long lActiveType);
	void CancelAllTimer(void);

private:
	typedef map<string, tagPty>	ATTRS_MAP;
	typedef ATTRS_MAP::iterator ITR_ATTR_MAP; 

	// �ƶ�
	long	m_lAICount;									// AIʱ��
	DWORD	m_dwMoveValiStartTime;						// �ƶ���֤��ʼ�ļ�ʱ
	DWORD	m_dwLastRecvMsgTime;						// �ϴν��ܵ��ƶ���Ϣ��ʱ��
	float	m_fMaxPeriodMoveSpeed;						// һ��ʱ����������ٶ�
	CGUID	m_guMoveValiStartRgnID;						// ��ʼ��֤�ĳ���ID
	long	m_lMoveValiStartPosX;						// ��ʼ�ƶ���Xλ��
	long	m_lMoveValiStartPosY;						// ��ʼ�ƶ���Yλ��
	float	m_fMoveValiStartSpeed;						// �ƶ���֤��ʼ�ٶ�
	float	m_fMoveValiTotalDistance;					// �ƶ����ܾ���
	float	m_fMoveValiRecvMsgCount;					// �յ����ƶ���Ϣ����

	GROW_LIST			m_GrowCoefs;					// ԭʼ���Գɳ�ϵ��
	tagPetBase			m_BaseAttrs;					// ��������
	tagPetDiff			m_DiffAttrs;					// ��������
	tagPetTemp			m_TempAttrs;					// ��ʱ����
	ATTRS_MAP			m_Attrs;						// ���Լ�
	set<string>			m_DirtyAttrs;					// ����ˢ�µ�����������
	set<string>			m_BaseDirtyAttrs;				// ��Χ���ˢ�µ�����������
	string				m_strOriName;					// ԭʼ��
	CGUID				m_HostID;						// ����
	SKILL_LIST			m_Skills;						// �����б�
	CPlayer*			m_pHost;						// ����
	map<long, long>		m_Timers;						// ��ʱ������
	CVariableList*		m_pVariableList;				// �����˽�нű������б�
}; 