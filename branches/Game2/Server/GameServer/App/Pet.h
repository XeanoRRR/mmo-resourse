/** 
* @file   Pet.h
* @author Fox (yulefox@gmail.com)
* @brief  CPet�����˳�������Լ�����. ���нӿ���δ���ǿ���������.
			��Ҫ�漰�����ʼ�����ͷ�, �����¼�������Ӧ.
*
* @date   Nov.18, 2008       
*/

#pragma once

#pragma  warning(disable:4244)
#pragma  warning(disable:4018)
#pragma  warning(disable:4267)

#include "MoveShape.h"
#include "Mover.h"
#include "PetCtrl.h"
#include "../../../Public/Common/BasePetDef.h"

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
	/// �������ͻ��˸�������
	bool UpdateAttributeToOtherClient();

	long  UpdateAttribute(BOOL bImmediately=0);
	long  OnUpdateAttributeTimeOut();

	/// �����Է����ı�ʱ�����
	virtual void OnPropertyValueChanged(const char* strName, LONG lValue);

	virtual void OnSpeedChanged(void);
	virtual void SetPosition(LONG lX, LONG lY);
	virtual void SetSlipPos(LONG lX, LONG lY, float fSpeed, float fDir);
	virtual bool CodeToDataBlock_ForClient(DBWriteSet& setWriteDB,
		bool bExData = true);							// GS==>Client����
	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB,
		bool bExData = true);							// GS==>WS����
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,
		bool bExData = true);							// WS==>GS����

	/// ����������Ը��ͻ��ˣ�������ֻ������������˿ͻ���
	bool CodeAttrToClient(DBWriteSet& setWriteDB);

	virtual DWORD GetExp()		{ return m_DiffAttrs.lExp; }
	virtual DWORD GetExpDead()	{ return 0; }			// ɱ���õ��ľ���ֵ
	virtual void SetHP(DWORD l) { SetAttr("Hp", l); }
	virtual void SetMP(DWORD l) { SetAttr("Mp", l); }
	virtual DWORD GetHP()		{ return m_DiffAttrs.lHp ? m_DiffAttrs.lHp : (GetState()!=STATE_DIED); }
	virtual WORD GetMP()		{ return m_DiffAttrs.lMp; }
	virtual DWORD GetEnergy()	{ return m_DiffAttrs.lEnergy; }
	virtual DWORD GetMaxHP()	{ return m_TempAttrs.lMaxHp; }	
	virtual DWORD GetMaxMP()	{ return m_TempAttrs.lMaxMp; }
	virtual DWORD GetMaxEnergy(){ return m_TempAttrs.lMaxEnergy;}

	long GetLoyalty(void)		{ return m_DiffAttrs.lLoyalty; }
	long GetMaxLoyalty(void)	{ return UNI_CONF.loyaltyMax; }
	long GetLife(void)			{ return m_DiffAttrs.lLife; }
	long GetMaxLife(void)		{ return UNI_CONF.maxLife; }

	/// ȡ��HP�ظ��ٶ�
	virtual WORD GetHpRecoverSpeed()	{ return m_BaseAttrs.lHpRecoverSpeed; }
	/// ȡ��MP�ظ��ٶ�
	virtual WORD GetMpRecoverSpeed()	{ return m_BaseAttrs.lMpRecoverSpeed; }
	/// ȡ�������ظ��ٶ�
	virtual WORD GetEnergyRecoverSpeed()	{ return m_BaseAttrs.lEnergyRecoverSpeed; }

	virtual CMoveShape* GetHost(void);
	virtual long AddActiveSkill(DWORD dwSkillID,
		DWORD dwLevel,
		CMoveShape *pTarget = NULL,
		long lGoodsIndex = 0,
		long lContainerID = 0,
		long lPos = 0);
	virtual void SetSkillCooldown(long lID, DWORD dwTime);
	virtual WORD GetLevel(void) { return m_DiffAttrs.lLevel; }	// ��ȡ����ȼ�
	virtual WORD GetDLevel(void) {return m_DiffAttrs.ServantAttr.lDLevel;} ///< ��ȡ������еȼ�
	virtual bool IsVisible(OBJECT_TYPE eType,
		const CGUID& rID);								// �Ƿ�ɱ�GUIDΪrID��MoveShape����

	long InitialSkills(void);							// ����ʱ��ʼ������
	bool UseSkill(tagSkill* pSkill,
		CMoveShape* pTarget = NULL);					// ʹ�ü���

	bool ModifySkill(long lID,
		long lLevel,
		long lPos);										// ��������

	/// ��Ӽ���(�ɼ�����Ʒ��Ƕ)
	bool AddSkill(CGoods* pGoods, long lPos, long lHoleType);

	/// ��ȡ����
	tagSkill* GetSkill(long lPos);

	/// ���ó��＼�ܿ�ݼ�
	void SetSkillHotKey(long lPos, long lActPos);

	/// ȡ�ó�������
	long GetPetType(void) { return m_BaseAttrs.lPetType; }
	/// ���ó�������
	void SetPetType(long lType) { m_BaseAttrs.lPetType = m_TempAttrs.lPetType =  lType; }

	///// ȡ�ó���ͼ�����
	//long GetPicID(void) { return m_BaseAttrs.lPicID; }
	///// ���ó���ͼ�����
	//void SetPicID(long lPicID) { m_BaseAttrs.lPicID = lPicID; }

	/// ȡ�ó����������
	long GetIndexID(void) { return m_BaseAttrs.lIndex; }
	/// ���ó����������
	void SetIndexID(long lIndexID) { m_BaseAttrs.lIndex = m_DiffAttrs.lIndex = lIndexID; }

	/// ȡ�ó�������
	const char* GetPetName(void) { return m_DiffAttrs.szName; }
	/// ���ó�������
	void SetPetName(const char* name) { strcpy_s(m_DiffAttrs.szName, 32, name); }
	/// ������������
	void SetHostName(const char* name) { strcpy_s(m_DiffAttrs.szHostName, 32, name); }

	/// ��������
	void SetHost(CPlayer* pPlayer);

	void SetLevel(long lLevel);							// ���ó���ȼ�
	bool ModifyPos(INIT_TYPE lInitType);				// ��������

	/// ��������ģʽ
	void SetMoveType(long lType);
	/// ���ù���ģʽ
	void SetFightType(long lType);
	/// ��ȡ����ģʽ
	long GetMoveType(void);
	/// ��ȡ����ģʽ
	long GetFightType(void);
	/// ����״̬ģʽ
	void SetStateType(long lType);
	/// ��ȡ״̬ģʽ
	long GetStateType(void);
	/// ת��״̬ģʽ
	void ChangeStateType(long lNewState);

	long CanBeSelected(void) { return m_BaseAttrs.lSelAtk & SAT_CAN_BESEL; }
	long CanBeAttacked(void) { return m_BaseAttrs.lSelAtk & SAT_CAN_BEATK; }
	long CanFight(void) { return m_BaseAttrs.lSelAtk & SAT_CAN_FIGHT; }
	bool NeedBeSaved(void)
	{
		return IsServant();
	}

	/// �Ƿ�Ұ��
	bool IsWild(void) { return m_bWild; }
	/// ����Ұ����־
	void SetWild(bool b) { m_bWild = b; }
	/// �Ƿ񼤻�״̬
	bool IsActive(void)
	{
		return (m_DiffAttrs.lStateType ==  PET_STATE_ACTIVE) ? true : false;
	}
	/// �Ƿ����
	bool IsHangUp(void)
	{
		return (GetState() == STATE_HANGUP);
	}

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
		long lInitType,long IsWild);								// ��ʼ����������
	void InitialTempAttrs(void);						///< ��ʼ����ʱ����
	bool InitialDiffAttrs(long lInitType, bool IsWild);				// ��ʼ������Ĵ��̻��з��ı�������
	//bool InitialByTarget(CMonster* pTarget);			// ͨ�������ʼ������
	bool InitialByHost(INIT_TYPE lInitType);			// ��ʼ����������
	bool AddToRegion(void);								// ��ӵ�������
	void InitNameValueMap(void);						// ��ʼ�������ֶ�
	void InitialFightProperty(void);					// ����ս������

	// �¼�����
	void RegEvent(PET_EVENT_TYPE eType,
		long lDelayTime = 0,
		long lIntervalTime = 0);						// ע���¼�
	bool RegShadeEvent(void);							// ע�ὥ���¼�
	bool RegAutoRenew(void);							// ע���Զ��ָ�HP��MP��
	const CMoveShape* GetTarget(void);					// ���س��ﵱǰĿ��

	// �¼���Ӧ
	void OnHPRenew(void);								///< �̴ӵ�HPˢ��
	void OnMPRenew(void);								///< �̴ӵ�MPˢ��
	void OnEnergyRenew(void);							///< �̴ӵ�����ˢ��
	void OnLoyaltyRenew(void);							///< �̴ӵ��ҳ϶�
	void OnLifeRenew(void);								///< �̴ӵ�����

	void OnLifeEnd(void);								// 5) ���������ս�
	void OnActive(void);								// �������
	void OnHouse(long lOpType);							// �ղس���, bActiveΪ��ұ���ʱ�Ƿ񼤻�����־
	/// ��������
	void OnCreate(void);

	// ����
	/// �Ƿ�ӵ�м���
	bool HasSkill(long lID);

	/// ��Ӽ���
	tagSkill* AddSkill(tagSkill& rSkill);

	/// ������Ƕ�����ж�
	bool CanAddSkill(CGoods* pGoods);

	/// ��鼼��ʹ������
	long CanUseSkill(tagSkill* pSkill,
		CMoveShape* pTarget,
		long& lSkillType);

	bool ModifySkill(tagSkill& rSkill);					///< ��������
	//long AppendGeniusSkill(void);						///< ׷��һ���츳����
	//long AddGeniusSkill(void);							///< ����츳����
	//long AddStudySkill(void);							///< ��Ӳ�ۼ���
	//long AddConfigSkill(void);							///< ������ü��ܣ�
	//long OpenSkillHole(long lType = 0, long lPos = 0);	///< �������ܲ�
	//void CloseSkillHole(long lPos);						///< �رռ��ܲ�
	CNewSkill* CanModifySkill(tagSkill& rSkill);		///< ��鼼����������
	//bool UpdateSkill(long lID, long lLevel);			///< ���¼���


	void StudySkillScript(long lID, long lLevel);		///< ��������ѧϰ�ű�
	void PetRunScript(const char *szScriptName);		///< �������нű�

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

	/// 
	void SetActiveAttr(long lActiveType);

	/// ע�����ж�ʱ��
	void CancelAllTimer(void);

	/// �����ҳ϶ȵȼ�
	long CalcLoyLevel(void);

	/// �����̴Ӳ�����
public:
	/// ���������Ƿ����̴�
	bool IsServant(void);

	/// ���������㷨
	bool OnReSetTalent(void);

	/// ��������ǿ�����
	bool DAfterValCheck(void);

	/// ��������ǿ��
	bool OnDAfterValUpGrade(long DVal);

	/// ���ú�������ǿ������
	bool OnResetDAfterCnt(void);

	/// �̴��ں�
	bool OnServantSyncretism(void);

	/// �̴��ں��㷨
	void CopyServantAttr(const CPet* pPet, const CPet* lPet, long lPLev);

	/// �ںϼ��㹫ʽ<�Ժ�����>
	void CALC_ELF_SYNCRETISM(const CPet* pPet, const CPet* lPet, long lPLev);

	/// �������Ե�
	bool  SetSawyPointValue(const tagPetBaseAttr& rtag);

	/// �������Ե�
	bool OnResetSavvy(void);

	/// ��ʯǿ�������ж�
	bool OnFStoneUpgradeCheck(void);

	/// ��ʯǿ��
	bool OnFStoneUpgrade(long fstone);

	/// ������λǿ���������
	bool OnlFUpgradeCheck(void);

	/// ������λǿ�� 
	bool OnlFUpgrade(long rate);

	/// ���ݳ����ҳ϶�ǿ���ɹ��ʵİٷֱ�
	int GetDownRate(int drop);

	/// �ǵ�ǿ���������
	bool OnStarLevUpGradeCheck(void);

	/// �ǵ�ǿ��
	bool OnStarLevUpGrade(long rates);

	/// Ʒ��ǿ���㷨�����ж�
	bool OnCElfPensValueUpGradeCheck(void);

	/// Ʒ��ǿ���㷨�㷨
	bool OnCElfPensValueUpGrade(long val);

	/// ȡ��Ʒ��ǿ��������վ�����Ŀ
	long GetLessNice();

	/// �ı��ҳ϶�
	void OnChangeLoy(long lDelta);

	/// �ı�����ֵ
	void OnChangeLife(long lDelta);

	/// �����Я����������
	long AddServantTakeNum(long num);

	/// �õ�Lev��Ҫ����������
	inline	long GetALevNeedExp(long  _lev);

	///  ���ÿ�������������Ե�������
	inline long	GetUpGradeUpValue(long _talentval , long _BaseVal);

	/// �ٻ�����, �ж�������
	bool VacationPet(long lev);

	/// ȡ�ó���Ʒ��
	long GetGrade(void);

	/// ȡ�ó������������ǿ������
	long GetDAfterCnt(void);

	/// ǿ���ӿ�
	bool OnServantUpGrade(long uId , long uRate);

	/// ��������
	bool OnUpgradeLev(void);

	/// �����������еȼ�
	bool OnUpgradeDLev(void);

	/// �����ȡ����
	void AddExp(CMonster* pMonster);

	/// �����ȡ��Ϊ����
	void AddDExp(CMonster* pMonster);
private:
	/// ��������
	void IncExp(long lDelta);
	/// ������Ϊ����
	void IncDExp(long lDelta);
	/// ��������㷨
	long GetBaseTalentRadomVal(void);
	/// Ʒ������㷨
	long GetBaseQualityRadomVal(void);

private:
	typedef map<string, tagPty>	ATTRS_MAP;
	typedef ATTRS_MAP::iterator ITR_ATTR_MAP; 

	// �ƶ�
	long	m_lAICount;									///< AIʱ��
	DWORD	m_dwMoveValiStartTime;						///< �ƶ���֤��ʼ�ļ�ʱ
	DWORD	m_dwLastRecvMsgTime;						///< �ϴν��ܵ��ƶ���Ϣ��ʱ��
	float	m_fMaxPeriodMoveSpeed;						///< һ��ʱ����������ٶ�
	CGUID	m_guMoveValiStartRgnID;						///< ��ʼ��֤�ĳ���ID
	long	m_lMoveValiStartPosX;						///< ��ʼ�ƶ���Xλ��
	long	m_lMoveValiStartPosY;						///< ��ʼ�ƶ���Yλ��
	float	m_fMoveValiStartSpeed;						///< �ƶ���֤��ʼ�ٶ�
	float	m_fMoveValiTotalDistance;					///< �ƶ����ܾ���
	float	m_fMoveValiRecvMsgCount;					///< �յ����ƶ���Ϣ����

	GROW_LIST			m_GrowCoefs;					///< ԭʼ���Գɳ�ϵ��
	tagPetBase			m_BaseAttrs;					///< ��������
	tagPetDiff			m_DiffAttrs;					///< ��������
	tagPetTemp			m_TempAttrs;					///< ��ʱ����
	ATTRS_MAP			m_Attrs;						///< ���Լ�
	set<string>			m_DirtyAttrs;					///< ����ˢ�µ�����������
	set<string>			m_BaseDirtyAttrs;				///< ��Χ���ˢ�µ�����������
	string				m_strOriName;					///< ԭʼ��
	CGUID				m_HostID;						///< ����
	SKILL_LIST			m_Skills;						///< �����б�
	CPlayer*			m_pHost;						///< ����
	map<long, long>		m_Timers;						///< ��ʱ������
	bool				m_bWild;						///< �Ƿ�Ұ����־
public:
    Mover &GetMover() { return m_mover; }
private:
    Mover m_mover;
}; 