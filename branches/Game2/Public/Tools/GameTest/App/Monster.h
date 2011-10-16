#pragma once
#include "moveshape.h"

class CMonster :
	public CMoveShape
{
public:
	CMonster(void);
	virtual ~CMonster(void);
	// ��ʼ����������ֵ��Ӱ��
	void InitNameValueMap();
public:
	// ��ö��״̬˳�򣬱�ʾNPC����״̬��ʾ�����ȼ�����->��
	enum eStateByQuest
	{
		STATE_STEP = 0,			// ����
		STATE_TRACE,			// ׷��
		STATE_QUEST,			// ����
		STATE_NUM,				// �������״̬������
	};
	//������Ϣ
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
	void SetStateByQuest(int iState, tagQuestInfo stQuestInfo);
	// ��õ�ǰNPC������Ҫ��ʾ���������״̬
	int GetStateByQuest();
	// ���ָ��״̬Ӧ�û��Ƶı�־�����/δ��ɣ����߿ɽӣ����ӣ�
	long GetPicIndexByNpc(int iState);
	// ��������Ϣ�����Ƴ�ָ������ID��������Ϣ
	void RemoveStateByQuest(int iState, long lQuestID, long lStep = -1, long lTraceIndex = -1);
private:
	tagStateByQuest m_State[STATE_NUM];
	//��������
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
	////�ڳ���Ϣ
	//struct tagCarriageInfo
	//{
	//	long lMasterType;
	//	CGUID MasterID;
	//	char strMasterName[20];
	//	tagCarriageInfo()
	//	{
	//		lMasterType=0;
	//		memset(strMasterName,0,sizeof(strMasterName));
	//	}
	//};
private:
	//-------------------------------------------------//
	string	m_strTitle;				// Monster�ĳƺ�
	string	m_strOrigName;			// ԭʼ����
	long	m_lCreatorType;			// ���������ߵ�����
	CGUID	m_CreatorGuid;			// ���������ߵ�GUID
	//-------------------------------------------------//
	//	float	m_fTurnSpeed;

	virtual void SetAction(WORD l)  { m_wAction = l; }

public:
	virtual const char* GetOrgName()
	{ return (LPCSTR)m_strOrigName.c_str(); }						// ���Monster��ԭʼ��
	virtual void	SetPosXY(float x, float y);										//����λ��

	void SetMonsterTitle(const char* strTitle)	{ m_strTitle = strTitle; }
	const char* GetMonsterTitle()		{ return m_strTitle.c_str(); }

	long GetCreatorType()		{ return m_lCreatorType; }
	void SetCreatorType(long l)	{ m_lCreatorType = l; }

	const CGUID& GetCreatorGuid()	{ return m_CreatorGuid; }
	void SetCreatorGuid(const CGUID& guid)	{ m_CreatorGuid = guid; }

private:
	BYTE m_byBlockType;
	WORD m_wCanTalk;
	eNpcType m_eNpcType;

	//�µĹ�������
	//tagMPro			m_BaseProperty;				//�����������
	//tagMFPro		m_FightProperty;			//����ս������

	//	BOOL			m_bDeadStatus;				//��ʼ������
	//	DWORD			m_dwDeadStartTime;			// ����ʱ�����(������һ��ʱ���client���)
	//	bool			m_bAppearanceSoundPlayed;	// �Ƿ��Ѿ������˳�������Ч��
	//	DWORD			m_dwPetLastRefreshTime;
	//	DWORD			m_dwPetLastDiedTime;

public:
	//////////////////////////�¶�������Խӿ�////////////////////////////////
	//�����������
	//DWORD	GetIndex()				{return m_BaseProperty.dwIndex;}
	//void	SetIndex(DWORD dw)		{m_BaseProperty.dwIndex = dw;}
	////�ȼ�
	//WORD	GetLevel()				{return m_BaseProperty.wLevel;}
	//void	SetLevel(WORD w)		{m_BaseProperty.wLevel = w;}
	////����ֵ
	//DWORD	GetExp()				{return m_BaseProperty.dwExp;}
	//void	SetExp(DWORD dw)		{m_BaseProperty.dwExp = dw;}
	////����ID
	//BYTE	GetCountry()			{return m_BaseProperty.byCountry;}
	//void	SetCountry(BYTE b)		{m_BaseProperty.byCountry = b;}
	////�Ƿ����ѱ��
	//BYTE	GetTamable()			{return m_BaseProperty.byTamable;}
	//void	SetTamable(BYTE b)		{m_BaseProperty.byTamable = b;}
	////�����赲����
	BYTE	GetBlockType()			{ return m_byBlockType;}
	void	SetBlockType(BYTE b)	{ m_byBlockType = b;}
	////�����ѱ������
	//DWORD	GetMaxTameCount()		{return m_BaseProperty.dwMaxTameCount;}
	//void	SetMaxTameCount(DWORD dw)	{m_BaseProperty.dwMaxTameCount = dw;}
	////���ҹ���ֵ
	//WORD	GetCountryContriValue()	{return m_BaseProperty.wCountryContriValue;}
	//void	SetCountryContriValue(WORD w)	{m_BaseProperty.wCountryContriValue = w;}
	////SP����ֵ
	//WORD	GetSPContriValue()		{return m_BaseProperty.wSPContriValue;}
	//void	SetSPContrivalue(WORD w)	{m_BaseProperty.wSPContriValue = w;}
	////Ԫ�����
	//eEleType	GeteEleType()		{return m_BaseProperty.eEType;}
	//void	SetEleType(eEleType l)	{m_BaseProperty.eEType = l;}
	////Npc��� (NNPC,Monster,Guard)
	eNpcType	GeteNpcType()		{return m_eNpcType;}
	void	SeteNpcType(eNpcType l)	{m_eNpcType= l;}
	////����
	//WORD	GetClass()				{return m_BaseProperty.eClass_;}
	//void	SetClass(eClass w)		{m_BaseProperty.eClass_ = w;}
	////��Ⱥ(AIʹ��)
	//WORD	GetRace()				{return m_BaseProperty.wRace;}
	//void	SetRace(WORD w)			{m_BaseProperty.wRace = w;}
	////�����ƶ��ĸ���(����)
	//WORD	GetMoveRandomValue()	{return m_BaseProperty.wMoveRandomValue;}
	//void	SetMoveRandomValue(WORD w)	{m_BaseProperty.wMoveRandomValue = w;}
	////ͣ����ʱ��(��һ�������м�ͣ��֡��)
	//WORD	GetStopTime()			{return m_BaseProperty.wStopTime;}
	//void	SetStopTime(WORD w)		{m_BaseProperty.wStopTime = w;}
	////����(0:1x1 1:3*3 2:5*5)
	//WORD	GetFigure()				{return m_BaseProperty.wFigure;}
	//void	SetFigure(WORD w)		{m_BaseProperty.wFigure = w;}
	////��ǰ����ֵ
	//DWORD	GetHp()					{return m_BaseProperty.dwHp;}
	//void	SetHp(DWORD dw)			{m_BaseProperty.dwHp = dw;}
	////AI����
	//DWORD	GetAIType()				{return m_BaseProperty.dwAIType;}
	//void	SetAIType(DWORD dw)		{m_BaseProperty.dwAIType = dw;}
	////���˱���
	//float	GetHurtProportion()		{return m_BaseProperty.fHurtProportion;}
	//void	SetHurtProportion(float f)	{m_BaseProperty.fHurtProportion = f;}
	////����ʱ��
	//DWORD   GetGuardTime()			{return m_BaseProperty.dwGuardTime;}
	//void	SetGuardTime(DWORD d)	{m_BaseProperty.dwGuardTime = d;}
	////���ܵ��ߣ����ܵ�)
	//float   GetEscapePoint()		{return m_BaseProperty.fEscapePoint;}
	//void	SetEscapePoint(float f)	{m_BaseProperty.fEscapePoint = f;}
	////���ܸ���,һ��С�ڵ���100�����ڵ���0������
	//WORD    GetSkillProportion()	{return m_BaseProperty.wSkillProportion;}
	//void	SetSkillProportion(WORD w)	{m_BaseProperty.wSkillProportion = w;}
	////���ܼ��
	//DWORD   GetSkillSpace()			{return m_BaseProperty.dwSkillSpace;}
	//void	SetSkillSpace(DWORD dw)	{m_BaseProperty.dwSkillSpace = dw;}
	////AI�ο�
	//DWORD   GetAIReferrence(int index)			{return m_BaseProperty.dwAIReferrence[index];}
	//void	SetAIReferrence(int index,DWORD dw)	{m_BaseProperty.dwAIReferrence[index] = dw;}

	////boss����
	//WORD	GetBossLevel()						{return	m_BaseProperty.wBossLevel;}
	//void	SetBossLevel(WORD w)				{m_BaseProperty.wBossLevel = w;}

	//Monsterս������

	//��������
	//	DWORD	GetMaxHp()				{return m_FightProperty.dwMaxHp;}
	//	void	SetMaxHp(DWORD dw)		{m_FightProperty.dwMaxHp = dw;}
	//�����ָ��ٶ�
	//	WORD	GetHpRecoverSpeed()		{return m_FightProperty.wHpRecoverSpeed;}	
	//	void	SetHpRecoverSpeed(WORD w)		{m_FightProperty.wHpRecoverSpeed = w;}
	//��С������
	//	WORD	GetMinAtk()				{return m_FightProperty.wMinAtk;}	
	//	void	SetMinAtk(WORD w)		{m_FightProperty.wMinAtk = w;}
	//��󹥻���
	//	WORD	GetMaxAtk()				{return m_FightProperty.wMaxAtk;}
	//	void	SetMaxAtk(WORD w)		{m_FightProperty.wMaxAtk = w;}
	//��С����������
	//	WORD	GetMinMAtk()			{return	m_FightProperty.wMinMAtk;}	
	//	void	SetMinMAtk(WORD w)		{m_FightProperty.wMinMAtk = w;}
	//�����������
	//	WORD	GetMaxMAtk()			{return	m_FightProperty.wMaxMAtk;}	
	//	void	SetMaxMAtk(WORD w)		{m_FightProperty.wMaxMAtk = w;}
	//��������
	//	WORD	GetHit()				{return	m_FightProperty.wHit;}		
	//	void	SetHit(WORD w)			{m_FightProperty.wHit = w;}
	//������
	//	WORD	GetDef()				{return	m_FightProperty.wDef;}
	//	void	SetDef(WORD w)			{m_FightProperty.wDef = w;}
	//��������
	//	WORD	GetDodge()				{return	m_FightProperty.wDodge;	}
	//	void	SetDodge(WORD w)		{m_FightProperty.wDodge = w;}
	//�м�
	//	WORD	GetParry()				{return m_FightProperty.wParry;}	
	//	void	SetParry(WORD w)		{m_FightProperty.wParry = w;}

	//��Զ��������
	//	WORD	GetFarAtkDis()			{return m_FightProperty.wFarAtkDis;}
	//	void	SetFarAtkDis(WORD w)	{m_FightProperty.wFarAtkDis = w;}
	//�����������
	//	WORD	GetNeaAtkDis()			{return	m_FightProperty.wNeaAtkDis;}	
	//	void	SetNeaAtkDis(WORD w)	{m_FightProperty.wNeaAtkDis = w;}
	//������Χ(�Թ���Ŀ��Ϊ����0:1x1 1:3*3 2:5*5)
	//WORD	GetAtkRange()			{return	m_FightProperty.wAtkRange;}	
	//void	SetAtkRange(WORD w)		{m_FightProperty.wAtkRange = w;}
	//���䷶Χ
	//	WORD	GetGuardRange()			{return	m_FightProperty.wGuardRange;}
	//	void	SetGuardRange(WORD w)	{m_FightProperty.wGuardRange = w;}
	//׷����Χ
	//	WORD	GetChaseRange()			{return m_FightProperty.wChaseRange;}	
	//	void	SetChaseRange(WORD w)	{m_FightProperty.wChaseRange = w;}
	//��־����
	//	WORD	GetWillDef()			{return	m_FightProperty.wWillDef;}	
	//	void	SetWillDef(WORD w)		{m_FightProperty.wWillDef = w;}
	//���ʿ���
	//	WORD	GetConstiDef()			{return	m_FightProperty.wConstiDef;}
	//	void	SetConstiDef(WORD w)	{m_FightProperty.wConstiDef = w;}
	//��������
	//	WORD	GetCri()				{return m_FightProperty.wCri;}	
	//	void	SetCri(WORD w)			{m_FightProperty.wCri = w;}
	//����������
	//	WORD	GetCriDef()				{return	m_FightProperty.wCriDef;}
	//	void	SetCriDef(WORD w)		{m_FightProperty.wCriDef = w;}
	//ħ����������
	//	WORD	GetMCriDef()			{return m_FightProperty.wMCriDef;}	
	//	void	SetMCriDef(WORD w)		{m_FightProperty.wMCriDef = w;}
	//Debuff����
	//	WORD	GetDeBuffDef(eDefbuffType index)	{return	m_FightProperty.wDeBuffDef[index];}	
	//	void	SetDeBuffDef(eDefbuffType index,WORD w)	{m_FightProperty.wDeBuffDef[index] = w;}
	//����Ԫ�ؿ���
	//	WORD	GetElemDef(eEleType	index)			{return m_FightProperty.wElemDef[index];}
	//	void	SetElemDef(eEleType	index,WORD w)	{m_FightProperty.wElemDef[index] = w;}
	//�Ը��������˺�
	//	WORD	GetClassDam(eClass	index)			{return m_FightProperty.wClassDam[index];}	
	//	void	SetClassDam(eClass	index,WORD w)	{m_FightProperty.wClassDam[index] = w;}
	//�Ը���Ԫ���˺�
	//	WORD	GetElemDam(eEleType index)			{return	m_FightProperty.wElemDam[index];}	
	//	void	SetElemDam(eEleType	index,WORD w)	{m_FightProperty.wElemDam[index] = w;}


	// ���úͻ��Monster�ܷ�Ի�������(�Ի�ֵ��Ӧ�˶Ի�����ͼ�꣬0��ʾ���ܶԻ�)
	void SetIsCanTalk(WORD wIsTalk)		{m_wCanTalk = wIsTalk;}
	WORD IsCanTalk()					{return m_wCanTalk;}

	//////////////////////////////////////////////////////////////////////////
	//	�ӿ�
	//////////////////////////////////////////////////////////////////////////
public:
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	void  DecordChangeProperty(BYTE* pByte, long& pos);

	virtual bool DecordFromDataBlock(DBReadSet& readDB,bool bExData = true);
	void DecordChangePropertyFromDataBlock(DBReadSet& readDB);
	//������ֵ�ı��Ժ����
	virtual bool  OnPropertyChanged(const char* attrName);
	//�õ�����ֵ���ַ�����ʾ
	virtual const char* GetPropertyStringValue(const char* pszPropertyName);
	void UpdateProperty(CMessage* pMsg);
};
