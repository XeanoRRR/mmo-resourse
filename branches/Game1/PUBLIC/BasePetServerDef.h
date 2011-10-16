// Fox@20090115----------------------------------------------
// File:    BasePetServerDef.h
// Brief:   �������ʹ�õ�ö��/�ṹ/����(����GS/WSʹ��).
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Jan.15, 2009
// Fox@20090115----------------------------------------------

#ifndef BASE_PET_SERVER_DEF_H
#define BASE_PET_SERVER_DEF_H

#include "BasePetDef.h"
#include "../public/GUID.h"

#define CONDITION_CHECK(SWITCH) \
	if( false == (SWITCH) ) return 0;

class CPet;

const long				MAX_LEVEL = 256;
const long				F_MAX_CLASSES = 16;
const long				S_MAX_CLASSES = 5;

enum MOVE_TYPE
{
	MT_WALK				= 0x00,					// ����
	MT_RUN				= 0x01,					// ����
};

enum INVISIBLE_TYPE
{
	IT_VISIBLE			= 0x00,					// һֱ�ɼ�
	IT_VISIBLE_TEAM		= 0x01,					// ����, �����ѿɼ�
	IT_VISIBLE_COUNTRY	= 0x02,					// ����, �����˿ɼ�
	IT_VISIBLE_UNION	= 0x03,					// ����, �����˿ɼ�
	IT_VISIBLE_FACTION	= 0x04,					// ����, ������ɼ�
	IT_INVISIBLE		= 0xFF,					// ��ȫ���ɼ�
};

enum SEL_ATK_TYPE
{
	SAT_CAN_BESEL		= 0x01,					// ���Ա�ѡ��
	SAT_CAN_BEATK		= 0x02,					// ���Ա�����
	SAT_CAN_FIGHT		= 0x04,					// ���Բ���ս��
};

enum DN_OP_TYPE
{
	NOT_BE_SAVED		= 0x00,					// ����Ҫ����
	CAN_BE_SAVED		= 0x01,					// ��Ҫ����
};

enum INIT_TYPE
{
	IT_SAME_RGN			= 0x01,					// ͬһ�����л�
	IT_SAME_GS			= 0x02,					// ͬһGS�л�
	IT_CREATE			= 0x10,					// ��һ�α�����
	IT_RELIVE			= 0x11,					// ����
	IT_LOGIN_GS			= 0x12,					// ��ͬGS�л�(��¼��Ϸ)
};

enum PET_EVENT_TYPE
{
	PET_HPRenew,								// ����ˢ��HP
	PET_MPRenew,								// ����ˢ��MP
	PET_GorgeRenew,								// ����ˢ�±�ʳ��
	PET_LoyaltyRenew,							// ����ˢ���ҳ϶�
	PET_LifeTimeout,							// ���������ս�
	PET_SearchEnemy,							// ����Ѱ��
};

enum PET_OPER_TYPE
{
	PET_OT_NULL				= 0x00,
	PET_OT_APPEAR			= 0x01,				// ����
	PET_OT_INACTIVE			= 0x10,				// ������ҳ����б�ɾ��, �Ǽ���״̬
	PET_OT_DISAPPEAR		= 0x11,				// ��ʧ
	PET_OT_HOUSE			= 0x12,				// լ�ػ��ٻ��������ս�
	PET_OT_PLAYER_EXIT		= 0x13,				// �������(����ҳ����б����, ���������ʱ����, �������������)
	PET_OT_BEEN_KILLED		= 0x14,				// ս��, �ٻ��ﱻɱ��
	PET_OT_CHANGE_REGION	= 0x15,				// �л�����

	PET_OT_DELETE			= 0x20,				// ����ҳ����б�ɾ��
	PET_OT_DETECT			= 0x21,				// ����
	PET_OT_CANCEL			= 0x23,				// ������
	PET_OT_TRAP_END			= 0x24,				// ���������ս�
};

// ��ʳ��Ӱ�����Լ�
struct tagGorge
{
	const tagGorge& operator=(const tagGorge& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	int			nLoyalty;			// �ҳ϶�
	double		fHpMp;				// HP/MP�ظ�
	int			nAbsorb;			// רע
	int			nInterval;			// �����ͷż��
	double		fMiss;				// ��ʧ����
	double		fEffect;			// Ч���ӳ�
};

// ���ܿ���
struct tagSkillOpen
{
	const tagSkillOpen& operator=(const tagSkillOpen& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	int			nClass;				// ����ְҵ(������|������)
	int			nLevel;				// �����ȼ�
	int			nRate;				// ��������
};

// �츳��������
struct tagGenSkillBase
{
	long		lID;				// ����ID
	long		lLevel;				// ���ܵȼ�
};

// ����ϵͳͳһ����(����������޹�, �������ݿ��ȡ)
struct tagPetConf
{
	const tagPetConf& operator=(const tagPetConf& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	int			nFuncSwitch;						// ����ģ�鿪��
	int			nAISpringTime;						// AI�������(Ŀǰ������������)
	int			nHPMPInt;							// HP�Զ��ظ����
	int			nGorgeInt;							// ��ʳ���Զ��ظ����
	long		lGorgeLvNum;						// �ҳ϶Ȼظ��ּ�����
	int			nLoyaltyInt;						// �ҳ϶��Զ��ظ����
	long		lLoyaltyLvNum;						// �ҳ϶Ȼظ��ּ�����
	int			nMaxGorge;							// ��ʳ������
	int			nMaxLoyalty;						// �ҳ϶�����
	int			nFooddLvInt;						// ʳ��ּ����
	long		lFeedLvNum;							// ιʳ�ּ�����
	long		lMaxLevel;							// ������󿪷ŵȼ�
	int			nInitGeniusSkillMin;				// ��ʼ�츳������С����
	int			nInitGeniusSkillMax;				// ��ʼ�츳�����������
	long		lGeniusSkillsSize;					// �츳��������
	long		lGeniusSkillNum;					// �����츳���������
	long		lStudySkillsSize;					// ѧϰ��������
	long		lStudySkillNum;						// ����ѧϰ���������
	int			MaxDistances[S_MAX_CLASSES];		// �˳���Զ�������
	int			HPAutoRenew[F_MAX_CLASSES];			// HP�Զ��ظ�����
	int			MPAutoRenew[F_MAX_CLASSES];			// MP�Զ��ظ�����
	int			GorgeAutoRenew[S_MAX_CLASSES];		// ��ʳ���Զ��ظ�����
	int			Gorges[S_MAX_CLASSES];				// ��ʳ�ȵȼ��ּ�
	int			Loyalties[S_MAX_CLASSES];			// �ҳ϶ȵȼ��ּ�
	int			GeniusSkills[F_MAX_CLASSES];		// �츳�����б�
	long		Exps[MAX_LEVEL];					// ��������
	double		Feed[S_MAX_CLASSES];				// ιʳ����
	double		Speeds[S_MAX_CLASSES];				// �ٶ��趨
	tagGorge	GorgeEffs[S_MAX_CLASSES];			// ��ʳ��Ӱ������
	tagSkillOpen GeniusSkillOpen[F_MAX_CLASSES];	// �츳���ܿ�������
	tagSkillOpen StudySkillOpen[F_MAX_CLASSES];		// ѧϰ���ܿ�������

	// ����ԭʼ���Ի���
	double		fConsti2MaxHp;						// ����-->HP
	double		fConsti2HpRecoverSpeed;				// ����-->HP�ظ��ٶ�
	double		fConsti2Def;						// ����-->���
	double		fConsti2CriDef;						// ����-->�ﱬ����
	double		fWisdom2MaxMp;						// �ǻ�-->MP
	double		fWisdom2MpRecoverSpeed;				// �ǻ�-->MP�ظ��ٶ�
	double		fWisdom2Absorb;						// �ǻ�-->רע
	double		fWisdom2MDef;						// �ǻ�-->ħ��
	double		fStrenth2Parry;						// ����-->�м�
	double		fStrenth2Block;						// ����-->��
	double		fStrenth2MinAtk;					// ����-->��С�﹥
	double		fStrenth2MaxAtk;					// ����-->����﹥
	double		fStrenth2Cri;						// ����-->�ﱬ
	double		fAgility2Parry;						// ����-->�м�
	double		fAgility2Hit;						// ����-->����
	double		fAgility2Dodge;						// ����-->����
	double		fAgility2MinAtk;					// ����-->��С�﹥
	double		fAgility2MaxAtk;					// ����-->����﹥
	double		fIntell2MAtk;						// ����-->ħ��
	double		fIntell2MCri;						// ����-->ħ��
	double		fSpirit2WillDef;					// ����-->��־����
	double		fSpirit2MCriDef;					// ����-->ħ������
};

// ����ԭʼ����
struct tagPetOrig
{
	tagPetOrig(void)
		: lLevel(0)
		, fMaxHp(0)
		, fMaxMp(0)
		, fStrenth(0)
		, fAgility(0)
		, fConsti(0)
		, fIntell(0)
		, fWisdom(0)
		, fSpirit(0)
		, fCharm(0)
		, fLuck(0)
	{
	}

	const tagPetOrig& operator=(const tagPetOrig& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lLevel;								// �ȼ�
	double		fMaxHp;								// ����
	double		fMaxMp;								// ����
	double		fStrenth;							// ����
	double		fAgility;							// ����
	double		fConsti;							// ����
	double		fIntell;							// ����
	double		fWisdom;							// �ǻ�
	double		fSpirit;							// ����
	double		fCharm;								// ����
	double		fLuck;								// ����
};

// ��������(����������й�, �������ݿ��ȡ)
struct tagPetBase : public tagPetDisp
{
	tagPetBase(void)
		: lPetType(0)
		, lClass(0)
		, lIndex(0)
		, lPicID(0)
		, lExistTime(0)
		, lInvisible(0)
		, lMoveType(0)
		, lSelAtk(0)
		, lGorge(0)
		, lLoyalty(0)
		, lHpRecoverSpeed(0)
		, lMpRecoverSpeed(0)
	{
		memset(GeniusSkills, 0, sizeof(GeniusSkills));
		memset(szOriName, 0, sizeof(szOriName));
	}

	const tagPetBase& operator=(const tagPetBase& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lPetType;							// ��������
	long		lClass;								// ����������(��������, ս�����, �ٻ�������, ��������)
	long		lIndex;								// �������
	long		lPicID;								// ͼ�����
	long		lExistTime;							// ����ʱ��
	long		lInvisible;							// ��������
	long		lMoveType;							// �ƶ�����
	long		lSelAtk;							// ѡ�񹥻�ս��λ
	long		lGorge;								// ��ʳ��
	long		lLoyalty;							// �ҳ϶�
	long		lHpRecoverSpeed;					// �����ظ�
	long		lMpRecoverSpeed;					// �����ظ�
	long		lSkillNum;							// �����ʼ���еļ�������
	char		szOriName[32];						// ԭʼ��
	tagGenSkillBase	GeniusSkills[F_MAX_CLASSES];	// Ԥ���츳����
};

// �߼���������(������������, �������ݿ��ȡ)
struct tagPetTemp : public tagPetDetail
{
	tagPetTemp(void)
		: cSaveFlag(0)
		, lHpRecoverSpeed(0)
		, lMpRecoverSpeed(0)
		, lGorgeDelta(0)
		, lLoyaltyDelta(0)
		, lGeniusSkillNum(0)
		, lStudySkillNum(0)
		, lEffect(0)
		, lLastUseSkillTime(0)
	{
	}

	const tagPetTemp& operator=(const tagPetTemp& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	// ��������ֵ
	char		cSaveFlag;
	long		lCountry;
	long		lHpRecoverSpeed;
	long		lMpRecoverSpeed;
	long		lGorgeDelta;
	long		lLoyaltyDelta;
	long		lGeniusSkillNum;
	long		lStudySkillNum;
	long		lEffect;
	long		lLastUseSkillTime;
};

typedef map<long, tagSkill> SKILL_LIST;
typedef SKILL_LIST::iterator ITR_SKILL_LIST;
typedef vector<tagPetOrig> GROW_LIST;
typedef map<long, GROW_LIST> GROW_CONF;
typedef GROW_CONF::iterator ITR_GROW_CONF;
typedef map<long, tagPetBase> PET_LIST;
typedef const PET_LIST& C_PET_LIST;
typedef PET_LIST::iterator ITR_PET_LIST;
typedef PET_LIST::const_iterator C_ITR_PET_LIST;
typedef map<char, CGUID> ACT_PETS;
typedef const ACT_PETS& C_ACT_PETS;
typedef ACT_PETS::iterator ITR_ACT_PET;
typedef ACT_PETS::const_iterator C_ITR_ACT_PET;
typedef map<CGUID, CPet*> PETS;
typedef const PETS& C_PETS;
typedef PETS::iterator ITR_PET;
typedef PETS::const_iterator C_ITR_PET;

#endif