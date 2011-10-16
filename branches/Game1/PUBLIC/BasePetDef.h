// Fox@20090106----------------------------------------------
// File:    BasePetDef.h
// Brief:   �������ʹ�õ�ö��/�ṹ/����.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Jan.06, 2009
// Fox@20090106----------------------------------------------

#ifndef BASE_PET_DEF_H
#define BASE_PET_DEF_H

const int PET_STUDY_SKILL_TYPE	= 3;			// ��ۼ���������

enum PET_ATTR_TYPE
{
	PET_ATTR_UNKNOWN	= 0x0000,
	PET_ATTR_Hp,								// HP��ǰֵ
	PET_ATTR_MaxHp,								// HP����
	PET_ATTR_HpRecoverSpeed,					// HP�ظ��ٶ�
	PET_ATTR_Mp,								// MP��ǰֵ
	PET_ATTR_MaxMp,								// MP����
	PET_ATTR_MpRecoverSpeed,					// MP�ظ��ٶ�
	PET_ATTR_Gorge,								// ��ʳ��
	PET_ATTR_GorgeRecoverSpeed,					// ��ʳ�Ȼظ��ٶ�
	PET_ATTR_GorgeLevel,						// ��ʳ�ȵȼ�
	PET_ATTR_Loyalty,							// �ҳ϶�
	PET_ATTR_Exp,								// ����ֵ
	PET_ATTR_MaxExp,							// ����ֵ����
	PET_ATTR_Level,								// �ȼ�
	PET_ATTR_Strenth,							// ����
	PET_ATTR_Agility,							// ����
	PET_ATTR_Consti,							// ����
	PET_ATTR_Intell,							// ����
	PET_ATTR_Wisdom,							// �ǻ�
	PET_ATTR_Spirit,							// ����
	PET_ATTR_Charm,								// ����
	PET_ATTR_Luck,								// ����
	PET_ATTR_Parry,								// �м�
	PET_ATTR_Block,								// ��
	PET_ATTR_Pierce,							// ��͸
	PET_ATTR_Absorb,							// רע
	PET_ATTR_Hit,								// ����
	PET_ATTR_Dodge,								// ����
	PET_ATTR_Immunity,							// ����
	PET_ATTR_MAtkSpeed,							// �����ٶ�
	PET_ATTR_MinAtk,							// ��С������
	PET_ATTR_MaxAtk,							// ���������
	PET_ATTR_Def,								// �������
	PET_ATTR_MAtk,								// ħ������
	PET_ATTR_MDef,								// ħ������
	PET_ATTR_Cri,								// ������
	PET_ATTR_CriDef,							// ����������
	PET_ATTR_MCri,								// ħ������
	PET_ATTR_MCriDef,							// ħ����������
	PET_ATTR_WillDef,							// ��־����
	PET_ATTR_ElemDef0,							// ��ϵԪ�ؿ���
	PET_ATTR_ElemDef1,							// ˮϵԪ�ؿ���
	PET_ATTR_ElemDef2,							// ��ϵԪ�ؿ���
	PET_ATTR_ElemDef3,							// ��ϵԪ�ؿ���
	PET_ATTR_ElemDef4,							// ��ϵԪ�ؿ���
	PET_ATTR_ElemDef5,							// ʥϵԪ�ؿ���
	PET_ATTR_FightType,							// ս��ģʽ
	PET_ATTR_MoveType,							// �ƶ�ģʽ
	PET_ATTR_Country,							// ����
};

enum PET_FOOD_SATIETY							// ��ʳ��״̬
{
	PFS_HUNGRY			= 0,					// ����
	PFS_COMMON			= 1,					// ��ͨ
	PFS_SATIEFY 		= 2,					// ��ʳ
	PFS_PLEASED 		= 3,					// ����
};

enum PET_TYPE
{
	PET_TYPE_NULL		= 0x00,
	PET_TYPE_ELF		= 0x01,					// ����
	PET_TYPE_HUNTER,							// ս��
	PET_TYPE_CONJURED,							// �ٻ���
	PET_TYPE_TRAP,								// ����
	PET_TYPE_ORNA,								// �γ�
	PET_TYPE_COUNT,								// ������������
	PET_TYPE_ALL		= 0xFF,					// ��������
};

enum PET_MOVE_STYLE
{
	PET_AS_STOP			= 0x00,					// ͣ��
	PET_AS_FOLLOW		= 0x01,					// ����
};

enum PET_FIGHT_STYLE
{
	PET_AS_PASSIVE		= 0x00,					// ����
	PET_AS_DEFEND		= 0x01,					// ����
	PET_AS_ACTIVE		= 0x02,					// ����
};

enum PET_SKILL_TYPE
{
	PET_SKILL_NULL		= 0x0000,				// δ����
	PET_SKILL_RED		= 0x0001,				// ��ɫ��������
	PET_SKILL_YELLOW	= 0x0002,				// ��ɫ��������
	PET_SKILL_BLUE		= 0x0003,				// ��ɫ�߼�����
	PET_SKILL_GENIUS	= 0x0010,				// �츳����
	PET_SKILL_ENCHASED	= 0x1000,				// ����Ƕ
};

enum PET_OT_AI_TYPE
{
	PET_OT_AI_SET_TARGET	= 0x80,				// �Զ�Ѱ��
	PET_OT_AI_ADD_SKILL		= 0x81,				// ���ܲ���
};

// ���＼��
struct tagSkill
{
	tagSkill(void)
		: lID(0)
		, lType(0)
		, lPos(0)
		, lLevel(0)
		, lEndCooldown(0)
		, lActPos(-1)
		, bAuto(false)
	{}

	const tagSkill& operator=(const tagSkill& rSkill)
	{
		memcpy(this, &rSkill, sizeof(rSkill));
		return *this;
	}

	bool		bAuto;				// �Ƿ��Զ��ͷ�
	long		lID;				// ���
	long		lType;				// ��������
	long		lPos;				// ����λ��
	long		lLevel;				// �ȼ�
	long		lEndCooldown;		// ��ȴ����ʱ��
	long		lActPos;			// �������ϵ�λ��
};

// ����ս������/�ɱ�����/��ʾ����
struct tagPetDisp
{
	tagPetDisp(void)
		: lMaxHp(0)
		, lMaxMp(0)
		, lStrenth(0)
		, lAgility(0)
		, lConsti(0)
		, lIntell(0)
		, lWisdom(0)
		, lSpirit(0)
		, lCharm(0)
		, lLuck(0)
		, lParry(0)
		, lBlock(0)
		, lPierce(0)
		, lAbsorb(0)
		, lHit(0)
		, lDodge(0)
		, lImmunity(0)
		, lMAtkSpeed(0)
		, lMinAtk(0)
		, lMaxAtk(0)
		, lDef(0)
		, lMAtk(0)
		, lMDef(0)
		, lCri(0)
		, lCriDef(0)
		, lMCri(0)
		, lMCriDef(0)
		, lWillDef(0)
		, lElemDef0(0)
		, lElemDef1(0)
		, lElemDef2(0)
		, lElemDef3(0)
		, lElemDef4(0)
		, lElemDef5(0)
	{
	}

	const tagPetDisp& operator=(const tagPetDisp& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lMaxHp;								// ����
	long		lMaxMp;								// ����
	long		lStrenth;							// ����
	long		lAgility;							// ����
	long		lConsti;							// ����
	long		lIntell;							// ����
	long		lWisdom;							// �ǻ�
	long		lSpirit;							// ����
	long		lCharm;								// ����
	long		lLuck;								// ����
	long		lParry;								// �м�
	long		lBlock;								// ��
	long		lPierce;							// ��͸
	long		lAbsorb;							// רע
	long		lHit;								// ����
	long		lDodge;								// ����
	long		lImmunity;							// ����
	long		lMAtkSpeed;							// �����ٶ�
	long		lMinAtk;							// ��С������
	long		lMaxAtk;							// ���������
	long		lDef;								// �������
	long		lMAtk;								// ħ������
	long		lMDef;								// ħ������
	long		lCri;								// ������
	long		lCriDef;							// ����������
	long		lMCri;								// ħ������
	long		lMCriDef;							// ħ����������
	long		lWillDef;							// ��־����
	long		lElemDef0;							// ��ϵԪ�ؿ���
	long		lElemDef1;							// ˮϵԪ�ؿ���
	long		lElemDef2;							// ��ϵԪ�ؿ���
	long		lElemDef3;							// ��ϵԪ�ؿ���
	long		lElemDef4;							// ��ϵԪ�ؿ���
	long		lElemDef5;							// ʥϵԪ�ؿ���
};

// ������������(�����������ػ��޹�, ��Ҫ���ݿ��ȡ)
struct tagPetDiff
{
	tagPetDiff(void)
		: lIndex(0)
		, lPos(0)
		, lLevel(0)
		, lExp(0)
		, lHp(0)
		, lMp(0)
		, lNameChgTimes(0)
		, lGorge(0)
		, lLoyalty(0)
		, lSP(0)
		, lActive(0)
	{
		memset(tBirthday, 0, sizeof(tBirthday));
		memset(szName, 0, sizeof(szName));
		memset(szHostName, 0, sizeof(szHostName));
	}

	const tagPetDiff& operator=(const tagPetDiff& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lIndex;					// �������
	long		lPos;					// ���λ��, 0Ϊ�������
	long		lLevel;					// �ȼ�
	long		lExp;					// ����
	long		lHp;					// ����ֵ
	long		lMp;					// ħ��ֵ
	long		lNameChgTimes;			// ��������
	long		lGorge;					// ��ʳ��
	long		lLoyalty;				// �ҳ϶�
	long		lSP;					// ѵ������
	long		lActive;				// ����״̬
	long		tBirthday[6];			// ����
	char		szName[32];				// ��������
	char		szHostName[32];			// ��������
};

// ������ʱ��������
struct tagPetDetail : public tagPetDisp
{
	tagPetDetail(void)
		: lMoveType(0)
		, lFightType(0)
		, lClass(0)
		, lMaxExp(0)
		, lGorgeLevel(0)
		, lExistTime(0)
	{
	}

	const tagPetDetail& operator=(const tagPetDetail& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	const tagPetDetail& operator+=(const tagPetDisp& rTag)
	{
		lMaxHp += rTag.lMaxHp;		
		lMaxMp += rTag.lMaxMp;		
		lStrenth += rTag.lStrenth;	
		lAgility += rTag.lAgility;	
		lConsti += rTag.lConsti;		
		lIntell += rTag.lIntell;		
		lWisdom += rTag.lWisdom;	
		lSpirit += rTag.lSpirit;	
		lCharm += rTag.lCharm;		
		lLuck += rTag.lLuck;		
		lParry += rTag.lParry;		
		lBlock += rTag.lBlock;		
		lPierce += rTag.lPierce;	
		lAbsorb += rTag.lAbsorb;		
		lHit += rTag.lHit;		
		lDodge += rTag.lDodge;		
		lImmunity += rTag.lImmunity;	
		lMAtkSpeed += rTag.lMAtkSpeed;	
		lMinAtk += rTag.lMinAtk;	
		lMaxAtk += rTag.lMaxAtk;	
		lDef += rTag.lDef;	
		lMAtk += rTag.lMAtk;	
		lMDef += rTag.lMDef;	
		lCri += rTag.lCri;	
		lCriDef += rTag.lCriDef;	
		lMCri += rTag.lMCri;	
		lMCriDef += rTag.lMCriDef;	
		lWillDef += rTag.lWillDef;	
		lElemDef0 += rTag.lElemDef0;	
		lElemDef1 += rTag.lElemDef1;	
		lElemDef2 += rTag.lElemDef2;	
		lElemDef3 += rTag.lElemDef3;	
		lElemDef4 += rTag.lElemDef4;	
		lElemDef5 += rTag.lElemDef5;	

		return *this;
	}

	long		lMoveType;				// ����ģʽ
	long		lFightType;				// ս��ģʽ
	long		lClass;					// ����(��������, ս�����, �ٻ�������, ��������)
	long		lMaxExp;				// �ȼ�����
	long		lGorgeLevel;			// �����ȼ�
	long		lExistTime;				// ����ʱ��
};

#endif