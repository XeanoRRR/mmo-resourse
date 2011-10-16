#pragma once
#include "MoveShape.h"
#include "../../../Public/Common/MonsterDef.h"
#include "../../../Public/Common/MultiHpBar.h"
class CMonster :
	public CMoveShape
{
public:
	CMonster(void);
	virtual ~CMonster(void);

	// ��ʼ����������ֵ��Ӱ��
	void InitNameValueMap();

public:
	// -------------NPC���������״̬��ʶ������07.10.11���ӣ�--------------
	// ��ö��״̬˳�򣬱�ʾNPC����״̬��ʾ�����ȼ�������-����-����-�Ƹ�-�̸�-����-����-�Ҹ�-������
	enum eEffectLevel
	{
		YELLOW_QMARK = 0,		// ��ɫ�ʺ�		���ѽ���������[�����]��
		GREEN_QMARK,			// ��ɫ�ʺ�		���ѽ�֧������[�����]��
		BLUE_QMARK,				// ��ɫ�ʺ�		���ѽ��ظ�����[�����]��
		YELLOW_EMARK,			// ��ɫ��̾��	���ɽ���������
		GREEN_EMARK,			// ��ɫ��̾��	���ɽ�֧������
		BLUE_EMARK,				// ��ɫ��̾��	���ɽӷ�������
		GRAY_QMARK,				// ��ɫ�ʺ�		���ѽ�����[δ���]��
		GRAY_EMARK,				// ��ɫ��̾��	�����񼴽��ɽ�[�ȼ�δ���]��
		EFFECT_LEVEL,
	};
	// ״̬����
	enum eStateByQuest
	{
		STATE_STEP = 0,			// ����
		STATE_TRACE,			// ׷��
		STATE_QUEST,			// ����
		STATE_NUM,				// �������״̬������
	};

	struct tagQuestInfo
	{
		long lQuestID;			// ����ID
		long lStepIndex;		// �����������
		long lTraceIndex;		// �ò���׷������
		long lPicOK;			// ͼƬ����ɻ�ɽӣ�
		long lPicNO;			// ͼƬ��δ��ɻ򲻿ɽӣ�
		bool bIsOK;				// �Ƿ���ʾ���ͼ�꣨���߿ɽ�����ͼ�꣩
		tagQuestInfo()
		{
			lQuestID = 0;
			lStepIndex = -1;
			lTraceIndex = -1;
			lPicOK = -1;
			lPicNO = -1;
			bIsOK = false;
		}
	};

	// �������״̬��ʶ�ṹ���ýṹΪÿ��״̬����
	struct tagStateByQuest
	{
		int iState;				// ״̬��ʶ
		vector<tagQuestInfo> vec_QuestInfo;		// ��NPC�Ĵ���״̬��Ӧ��������Ϣ�б�
		tagStateByQuest()
		{
			iState = -1;
		}
	};
	// ���ø�NPC����������Ϣ��ص�ָ��״̬
	void SetStateByQuest(int iState, long lPicOK, long lPicNO, bool bIsAdd, long lQuestID, long lStep = -1, long lTraceIndex=-1);
	// ��õ�ǰNPC������Ҫ��ʾ���������״̬����
	long GetStateByQuest();
	// ���ָ��״̬Ӧ�û��Ƶı�־�����/δ��ɣ����߿ɽӣ����ӣ�
	long GetPicIndexByNpc(int iState);
	// ��������Ϣ�����Ƴ�ָ������ID��������Ϣ
	void RemoveStateByQuest(int iState, long lQuestID, long lStep = -1, long lTraceIndex = -1);
	// ��ú����õ�ǰ������ص���Ч
	void SetCurQuestEffect(long lEffectID);
	long GetCurQuestEffect()				{return m_lCurQuestEffectID;}
	// ��ú����õ�ǰmonster��ת���ٶ�
	void SetTurnSpeed(float fSpeed)			{m_fTurnSpeed = fSpeed;}
	float GetTurnSpeed()					{return m_fTurnSpeed;}

private:
	tagStateByQuest m_State[STATE_NUM];
	long	m_lEffectType[EFFECT_LEVEL];
	long	m_lCurQuestEffectID;		// ��ǰ�����������ЧID
	list<long>	m_lsOtherQuestEffectID;	// ������������Ч���̶����ȼ�֮��ģ�
	void DelOtherQuestEffectID(long lID);		// ɾ��ָ��ID������������Ч
	//---------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:

	// ��������
	enum eMonsterKind
	{
		MK_HUMAN,				// ����
		MK_MONSTER,				// Ұ��
		MK_DEMON,				// ��ħ
		MK_UNDIED,				// ����
		MK_BOSS,				// �Ϲ�
		MK_GUARD,				// ����
	};
	
    // �������ע�� Bugs 2008-9-12
	//��Ϊ��������Ϣ
    //struct tagPetInfo
	//{
	//	long lMasterType;
	//	CGUID MasterID;
	//	char strMasterName[20];
	//	DWORD dwLevel;
	//	DWORD dwExp;
	//	tagPetInfo()
	//	{
	//		lMasterType=0;
	//		dwLevel=dwExp=0;
	//		memset(strMasterName,0,sizeof(strMasterName));
	//	}
	//};

private:
	//���ֵĽ���Ч��
	BOOL  m_bFade;
	BOOL  m_bFadeIn;//FALSEOUT TRUEIN
	DWORD m_dwFadeStartTime;
	DWORD m_dwFadeTimeLegth;
	float m_fFadeRatio;
	DWORD m_dwLastAction;


	//--------------07.12.6liuke------------------
	//BOOL	m_bIsCanTalk;	// Monster�ܷ�Ի����
	string	m_strTitle;		// Monster�ĳƺ�
	WORD	m_wTitleColor;	// �ƺŵ���ɫ
	WORD	m_wIconID;		// Monster��ͼ��ID
	string	m_strOrigName;	// ԭʼ����
	//--------------------------------------------

	//---------------08.7.2liuke------------------
	long	m_lCreatorType;		// ���������ߵ�����
	CGUID	m_CreatorID;		// ���������ߵ�GUID
	//--------------------------------------------
	// --------------08.8.21liuke-----------------
	float	m_fTurnSpeed;		// ת���ٶ�


	virtual void SetAction(WORD l);
public:
	//--------------07.12.6liuke------------------
	// ���úͻ��Monster�ĳƺ�
	void SetMonsterTitle(const char *strTitle)	{m_strTitle = strTitle;}
	const char * GetMonsterTitle()				{return m_strTitle.c_str();}
	// ���úͻ��Monster�ƺŵ���ɫ
	void SetTitleColor(WORD lColor)			{m_wTitleColor = lColor;}
	LONG GetTitleColor()					{return m_wTitleColor;}
	// ���úͻ��Monster��ͼ��ID
	void SetMonsterIconID(WORD lIconID)		{m_wIconID = lIconID;}
	LONG GetMonsterIconID()					{return m_wIconID;}

	//---------07.12.6�������ӣ���ʾMonster�ƺ�----------
	void DisplayMasterTitle(const char* str, long lIconID, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor=0,bool bShoWName = false);
	//---------------------------------------------------

	// ���Monster�����ߵ����ͺ�ID
	long GetCreatorType()		{return m_lCreatorType;}
	CGUID GetCreatorID()		{return m_CreatorID;}
	// ����Monster�����ߵ����ͺ�ID
	void SetCreatorType(long type)		{m_lCreatorType = type;}
	void SetCreatorID(CGUID guid)		{m_CreatorID = guid;}



	// ���Monster��ԭʼ��
	virtual const char* GetOrigName()			{ return (LPCSTR)m_strOrigName.c_str();}
	// ����λ��
	virtual void SetPosXY(float x, float y);

	float GetFadeRatio() const { return m_fFadeRatio;}
	BOOL  IsFade() const { return m_bFade;}
	void  UpdateProperty(CMessage* pMsg);
	void  DecordChangeProperty(BYTE* pByte, long& pos);
	void DecodeChangePropertyFromDataBlock(DBReadSet& setReadDB);
private:
	////////////////////////////�µĹ�������////////////////////////
	tagMPro			m_BaseProperty;				//�����������
	tagMFPro		m_FightProperty;			//����ս������
	CMultiHpBar		m_MultiHpBar;				// ����Ķ���Ѫ������
	/////////////////////////////////////////////////////////////////
	//tagPetInfo		m_tPetInfo;					//��������
	BOOL			m_bDeadStatus;				//��ʼ������
	DWORD			m_dwDeadStartTime;			// ����ʱ�����(������һ��ʱ���client���)
	bool			m_bAppearanceSoundPlayed;	// �Ƿ��Ѿ������˳�������Ч��
	DWORD			m_dwPetLastRefreshTime;
	DWORD			m_dwPetLastDiedTime;

public:
	//tagPetInfo*		GetPetInfo()	{return &m_tPetInfo;}
//////////////////////////�¶�������Խӿ�////////////////////////////////
////Monster��������	
	//�����������
	DWORD	GetIndex()				{return m_BaseProperty.dwIndex;}
	void	SetIndex(DWORD dw)		{m_BaseProperty.dwIndex = dw;}
	//ͼ��ID
	DWORD	GetPicID()				{return m_BaseProperty.dwPicID;}
	void	SetPicID(DWORD dw)		{ m_BaseProperty.dwPicID = dw;}
	//ͼ�εȼ�
	DWORD	GetPicLevel()			{return m_BaseProperty.dwPicLevel;}
	void	SetPicLevel(DWORD dw)	{m_BaseProperty.dwPicLevel = dw;}
	//������ɫ���,��Ӧ���ͻ�������
	DWORD	GetNameColor()			{return m_BaseProperty.dwNameColor;}
	void	SetNameColor(DWORD dw)	{m_BaseProperty.dwNameColor = dw;}
	//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
	DWORD	GetHpBarColor()			{return m_BaseProperty.dwHpBarColor;}
	void	SetHpBarColor(DWORD dw)	{m_BaseProperty.dwHpBarColor = dw;}
	//����ID
	DWORD	GetSoundID()			{return m_BaseProperty.dwSoundID;}
	void	SetSoundID(DWORD dw)	{m_BaseProperty.dwSoundID = dw;}
	//�ȼ�
	WORD	GetLevel()				{return m_BaseProperty.wLevel;}
	void	SetLevel(WORD w)		{m_BaseProperty.wLevel = w;}
	//����ֵ
	DWORD	GetExp()				{return m_BaseProperty.dwExp;}
	void	SetExp(DWORD dw)		{m_BaseProperty.dwExp = dw;}
	//����ID
	BYTE	GetCountry()			{return m_BaseProperty.byCountry;}
	void	SetCountry(BYTE b)		{m_BaseProperty.byCountry = b;}
	//�Ƿ���Բ���
	BYTE	GetCatchable()			{return m_BaseProperty.byCatchable;}
	void	SetCatchable(BYTE b)	{m_BaseProperty.byCatchable = b;}
	//�����赲����
	BYTE	GetBlockType()			{return m_BaseProperty.byBlockType;}
	void	SetBlockType(BYTE b)	{m_BaseProperty.byBlockType = b;}
	//������󲶻���
	DWORD	GetMaxCatchFastness()		{return m_BaseProperty.dwMaxCatchFastness;}
	void	SetMaxCatchFastness(DWORD dw)	{m_BaseProperty.dwMaxCatchFastness = dw;}
	//���ҹ���ֵ
	WORD	GetCountryContriValue()	{return m_BaseProperty.wCountryContriValue;}
	void	SetCountryContriValue(WORD w)	{m_BaseProperty.wCountryContriValue = w;}
	//SP����ֵ
	WORD	GetSPContriValue()		{return m_BaseProperty.wSPContriValue;}
	void	SetSPContrivalue(WORD w)	{m_BaseProperty.wSPContriValue = w;}
	//Ԫ�����
	eEleType	GeteEleType()		{return m_BaseProperty.eEType;}
	void	SetEleType(eEleType l)	{m_BaseProperty.eEType = l;}
	//Npc��� (NNPC,Monster,Guard)
	eNpcType	GeteNpcType()		{return m_BaseProperty.eNType;}
	void	SeteNpcType(eNpcType l)	{m_BaseProperty.eNType = l;}
	//����
	eClass	GetClass()				{return m_BaseProperty.eClass_;}
	void	SetClass(eClass w)		{m_BaseProperty.eClass_ = w;}
	//��Ⱥ(AIʹ��)
	WORD	GetRace()				{return m_BaseProperty.wRace;}
	void	SetRace(WORD w)			{m_BaseProperty.wRace = w;}
	//�����ƶ��ĸ���(����)
	WORD	GetMoveRandomValue()	{return m_BaseProperty.wMoveRandomValue;}
	void	SetMoveRandomValue(WORD w)	{m_BaseProperty.wMoveRandomValue = w;}
	//ͣ����ʱ��(��һ�������м�ͣ��֡��)
	WORD	GetStopTime()			{return m_BaseProperty.wStopTime;}
	void	SetStopTime(WORD w)		{m_BaseProperty.wStopTime = w;}
	//AI��϶
	//WORD	GetAIInterval()			{return m_BaseProperty.wAIInterval;}
	//void	SetAIInterval(WORD w)	{m_BaseProperty.wAIInterval = w;}
	//����(0:1x1 1:3*3 2:5*5)
	BYTE GetFigure(eDIR dir=DIR_UP)	{return (BYTE)m_BaseProperty.wFigure;}
	void	SetFigure(WORD w)		{m_BaseProperty.wFigure = w;}
	//��ǰ����ֵ
	DWORD	GetHp()					{return m_BaseProperty.dwHp;}
	void	SetHp(DWORD dw)			{m_BaseProperty.dwHp = dw;}
	//AI����
	DWORD	GetAIType()				{return m_BaseProperty.dwAIType;}
	void	SetAIType(DWORD dw)		{m_BaseProperty.dwAIType = dw;}
	//���˱���
	float	GetHurtProportion()		{return m_BaseProperty.fHurtProportion;}
	void	SetHurtProportion(float f)	{m_BaseProperty.fHurtProportion = f;}
	//����ʱ��
	DWORD   GetGuardTime()			{return m_BaseProperty.dwGuardTime;}
	void	SetGuardTime(DWORD d)	{m_BaseProperty.dwGuardTime = d;}
	//���ܵ��ߣ����ܵ�)
	float   GetEscapePoint()		{return m_BaseProperty.fEscapePoint;}
	void	SetEscapePoint(float f)	{m_BaseProperty.fEscapePoint = f;}
	//���ܸ���,һ��С�ڵ���100�����ڵ���0������
	WORD    GetSkillProportion()	{return m_BaseProperty.wSkillProportion;}
	void	SetSkillProportion(WORD w)	{m_BaseProperty.wSkillProportion = w;}
	//���ܼ��
	DWORD   GetSkillSpace()			{return m_BaseProperty.dwSkillSpace;}
	void	SetSkillSpace(DWORD dw)	{m_BaseProperty.dwSkillSpace = dw;}
	//AI�ο�
	DWORD   GetAIReferrence(int index)			{return m_BaseProperty.dwAIReferrence[index];}
	void	SetAIReferrence(int index,DWORD dw)	{m_BaseProperty.dwAIReferrence[index] = dw;}
	//boss����
	WORD	GetBossLevel()						{return	m_BaseProperty.wBossLevel;}
	void	SetBossLevel(WORD w)				{m_BaseProperty.wBossLevel = w;}

////Monsterս������
	//��������
	DWORD	GetMaxHp()				{return m_FightProperty.dwMaxHp;}
	void	SetMaxHp(DWORD dw)		{m_FightProperty.dwMaxHp = dw;}
	//�����ָ��ٶ�
	WORD	GetHpRecoverSpeed()		{return m_FightProperty.wHpRecoverSpeed;}	
	void	SetHpRecoverSpeed(WORD w)		{m_FightProperty.wHpRecoverSpeed = w;}
	//��С������
	WORD	GetMinAtk()				{return m_FightProperty.wMinAtk;}	
	void	SetMinAtk(WORD w)		{m_FightProperty.wMinAtk = w;}
	//��󹥻���
	WORD	GetMaxAtk()				{return m_FightProperty.wMaxAtk;}
	void	SetMaxAtk(WORD w)		{m_FightProperty.wMaxAtk = w;}
	//��С����������
	WORD	GetMinMAtk()			{return	m_FightProperty.wMinMAtk;}	
	void	SetMinMAtk(WORD w)		{m_FightProperty.wMinMAtk = w;}
	//�����������
	WORD	GetMaxMAtk()			{return	m_FightProperty.wMaxMAtk;}	
	void	SetMaxMAtk(WORD w)		{m_FightProperty.wMaxMAtk = w;}
	//��������
	WORD	GetHit()				{return	m_FightProperty.wHit;}		
	void	SetHit(WORD w)			{m_FightProperty.wHit = w;}
	//������
	WORD	GetDef()				{return	m_FightProperty.wDef;}
	void	SetDef(WORD w)			{m_FightProperty.wDef = w;}
	//��������
	WORD	GetDodge()				{return	m_FightProperty.wDodge;	}
	void	SetDodge(WORD w)		{m_FightProperty.wDodge = w;}
	//��
	WORD	GetFightBlock()			{return	m_FightProperty.wBlock;}
	void	SetFightBlock(WORD w)	{m_FightProperty.wBlock = w;}
	//�м�
	WORD	GetParry()				{return m_FightProperty.wParry;}	
	void	SetParry(WORD w)		{m_FightProperty.wParry = w;}
	//�����ٶ�
	//WORD	GetAtcSpeed()			{return m_FightProperty.wAtcSpeed;}
	//void	SetAtcSpeed(WORD w)		{m_FightProperty.wAtcSpeed = w;}
	//��Զ��������
	//WORD	GetFarAtkDis()			{return m_FightProperty.wFarAtkDis;}
	//void	SetFarAtkDis(WORD w)	{m_FightProperty.wFarAtkDis = w;}
	////�����������
	//WORD	GetNeaAtkDis()			{return	m_FightProperty.wNeaAtkDis;}	
	//void	SetNeaAtkDis(WORD w)	{m_FightProperty.wNeaAtkDis = w;}
	////������Χ(�Թ���Ŀ��Ϊ����0:1x1 1:3*3 2:5*5)
	////WORD	GetAtkRange()			{return	m_FightProperty.wAtkRange;}	
	////void	SetAtkRange(WORD w)		{m_FightProperty.wAtkRange = w;}
	////���䷶Χ
	//WORD	GetGuardRange()			{return	m_FightProperty.wGuardRange;}
	//void	SetGuardRange(WORD w)	{m_FightProperty.wGuardRange = w;}
	////׷����Χ
	//WORD	GetChaseRange()			{return m_FightProperty.wChaseRange;}	
	//void	SetChaseRange(WORD w)	{m_FightProperty.wChaseRange = w;}
	//��־����
	WORD	GetWillDef()			{return	m_FightProperty.wWillDef;}	
	void	SetWillDef(WORD w)		{m_FightProperty.wWillDef = w;}
	//���ʿ���
	WORD	GetConstiDef()			{return	m_FightProperty.wConstiDef;}
	void	SetConstiDef(WORD w)	{m_FightProperty.wConstiDef = w;}
	//����������
	WORD	GetCri()				{return m_FightProperty.wCri;}	
	void	SetCri(WORD w)			{m_FightProperty.wCri = w;}
	//������������
	WORD	GetCriDef()				{return	m_FightProperty.wCriDef;}
	void	SetCriDef(WORD w)		{m_FightProperty.wCriDef = w;}
	//ħ����������
	WORD	GetMCriDef()			{return m_FightProperty.wMCriDef;}	
	void	SetMCriDef(WORD w)		{m_FightProperty.wMCriDef = w;}
	//Debuff����
	WORD	GetDeBuffDef(eDefbuffType index)	{return	m_FightProperty.wDeBuffDef[index];}	
	void	SetDeBuffDef(eDefbuffType index,WORD w)	{m_FightProperty.wDeBuffDef[index] = w;}
	//����Ԫ�ؿ���
	WORD	GetElemDef(eEleType	index)			{return m_FightProperty.wElemDef[index];}
	void	SetElemDef(eEleType	index,WORD w)	{m_FightProperty.wElemDef[index] = w;}
	//�Ը��������˺�
	WORD	GetClassDam(eClass	index)			{return m_FightProperty.wClassDam[index];}	
	void	SetClassDam(eClass	index,WORD w)	{m_FightProperty.wClassDam[index] = w;}
	//�Ը���Ԫ���˺�
	WORD	GetElemDam(eEleType index)			{return	m_FightProperty.wElemDam[index];}	
	void	SetElemDam(eEleType	index,WORD w)	{m_FightProperty.wElemDam[index] = w;}

	// ���úͻ��Monster�ܷ�Ի�������(�Ի�ֵ��Ӧ�˶Ի�����ͼ�꣬0��ʾ���ܶԻ�)
	void SetIsCanTalk(WORD wIsTalk)		{m_BaseProperty.wCanTalk = wIsTalk;}
	WORD IsCanTalk()					{return m_BaseProperty.wCanTalk;}

	// ��ù���Ķ���Ѫ������
	CMultiHpBar * GetMultiHpBar()		{return &m_MultiHpBar;}


	//������ֵ�ı��Ժ����
	virtual bool  OnPropertyChanged(const char* attrName);
	//�õ�����ֵ���ַ�����ʾ
	virtual const char* GetPropertyStringValue(const char* pszPropertyName);

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);

	virtual void AI(void);

	// �˳�ս�� ��Ч
	void	RunLeaveFightAI( long xDest, long yDest, long time );
	void	AILeaveFight();
private:
	long	m_xLeaveFightDest;
	long	m_yLeaveFightDest;
	long	m_timeLeaveFight;
	long	m_timeLeaveFightStart;
	bool	m_bRestorColor;
};