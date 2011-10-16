/** 
* @file   BasePetServerDef.h
* @author Fox (yulefox@gmail.com)
* @brief  �������ʹ�õ�ö��/�ṹ/����(����GS/WSʹ��).
*
* @date   20090115       
*/

#ifndef BASE_PET_SERVER_DEF_H
#define BASE_PET_SERVER_DEF_H

#include "../../../Common/BasePetDef.h"
#define CONDITION_CHECK(SWITCH) \
	if( false == (SWITCH) ) return 0;

class CPet;

/// ���ȼ�
const long				MAX_LEVEL = 256;
const long				F_MAX_CLASSES = 16;
const long				S_MAX_CLASSES = 5;

/// �ҳ϶ȵȼ���
const long				Max_Loyalties_Lev = 4;
/// һ��������
const long				MAX_FIRST_ATTR = 5;
/// ����������
const long				MAX_SECOND_ATTR = 7;
/// ��ۼ�����
const long				MAX_HOLE_SKILL = 3;

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
	PET_HPRenew,								///< ����ˢ��HP
	PET_MPRenew,								///< ����ˢ��MP
	PET_EnergyRenew,							///< ����ˢ������
	PET_LoyaltyRenew,							///< ����ˢ���ҳ϶�
	PET_LifeRenew,								///< ����ˢ���ҳ϶�
	PET_LifeTimeout,							///< ���������ս�
	PET_SearchEnemy,							///< ����Ѱ��
};

enum PET_OPER_TYPE
{
	PET_OT_NULL				= 0x00,
	PET_OT_APPEAR			= 0x01,				// ����
	PET_OT_INACTIVE			= 0x10,				// ������ҳ����б�ɾ��, �Ǽ���״̬
	PET_OT_DISAPPEAR		= 0x11,				// ��ʧ
	PET_OT_HOUSE			= 0x12,				// լ��
	PET_OT_PLAYER_EXIT		= 0x13,				// �������(����ҳ����б����, ���������ʱ����, �������������)
	PET_OT_BEEN_KILLED		= 0x14,				// �̴ӱ�ɱ��
	PET_OT_CHANGE_REGION	= 0x15,				// �л�����

	PET_OT_DELETE			= 0x20,				// ����ҳ����б�ɾ��
	PET_OT_DETECT			= 0x21,				// ����
	PET_OT_CANCEL			= 0x23,				// ������
	PET_OT_TRAP_END			= 0x24,				// ���������ս�
};

/// ����ǿ����Ϣ����
enum  PET_UPGRADE_MSG_TYPE
{  
	PET_ERROR_ID			=0x502,				///< �����ID
	PET_LEVELUPGR_MSGTYPE	=0x503,				///< �ȼ�
	PET_DISTURBUTE_MSGTYPE	=0x504,				///< ���Ե����ɹ�
	PET_DAFTERUPGR_MSGTYPE	=0x505,				///< ��������ǿ��
	PET_DAFTERRESET_MSGTYPE	=0x506,				///< ��������������Ϣ
	PET_EXPISENOUGH_MSGTYPE	=0x507,				///< ǿ��ʧ��,������Ҫ����
	PET_PENVALUEUPGR_MSGTYPE=0x508,				///< Ʒ��ǿ��
	PET_FSTONEUPGR_MSGTYPE	=0x509,				///< ��ʯǿ��
	PET_LFUP_MSGTYPE		=0x510,				///< ��λ��λǿ��
	PET_STARLEVUPGR_MSGTYPE	=0x511,				///< �ǵ�ǿ��
	PET_AMALG_MSGTYPE		=0x512,				///< �̴��ں�
};

/// �츳��������
struct tagGenSkillBase
{
	long		lID;				// ����ID
	long		lLevel;				// ���ܵȼ�
};

/// ����ϵͳͳһ����(����������޹�, �������ݿ��ȡ)
struct tagPetConf
{
	const tagPetConf& operator=(const tagPetConf& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	int			nFuncSwitch;						///< ����ģ�鿪��
	int			nLoyaltyInt;						///< �ҳ϶��Զ��ظ����
	int			nHPMPInt;							///< HP\MP\�����Զ��ظ����

	long		lMaxLevel;							///< ������󿪷ŵȼ�
	int			MaxDistances[S_MAX_CLASSES];		///< �˳���Զ�������
	//int		HPAutoRenew[F_MAX_CLASSES];			///< HP�Զ��ظ�����
	//int		MPAutoRenew[F_MAX_CLASSES];			///< MP�Զ��ظ�����

	int			loyaltyMax;							///< �ҳ϶�����
	int			loyaltyMin;							///< �޷��ٻ���ֵ
	int			loyaDownUnit;						///< �ҳ϶�˥����λ
	int			loyaDownVal;						///< �ҳ϶�˥���ٶ�
	int			loyaUpGrageAdd;						///< �ҳ϶���������ֵ
	int			loyaTradeRate;						///< �ҳ϶Ƚ��׽��ͱ��� 
	int			loyaDieRate;						///< �ҳ϶��������ͱ���
	int			loyaltyLvNum;						///< �ҳ϶ȵȼ�
	int			loyalties[Max_Loyalties_Lev];		///< �ҳ϶ȵȼ��ּ�
	int			layaExp[Max_Loyalties_Lev];			///< �ҳ϶���Ӧ��þ������
	int			layaHurt[Max_Loyalties_Lev];		///< �ҳ϶�����˺�����

	long		Exps[MAX_LEVEL];					///< ��������
	long		DExps[MAX_LEVEL];					///< ���о���
	double		Speeds[S_MAX_CLASSES];				///< �ٶ��趨

	double		Quality[PET_QUALITY_NUM];			///< Ʒ������
	double		syncretismRate[9];					///< Ʒ����Ӧ�ں�ϵ��
	int			gradeNum;							///< Ʒ����Ŀ
	int			Speciality[PET_BASE_NUM];			///< ����ƫ�ö���������
	int			moreHostLevel;						///< ���ﳬ����ҵȼ�����
	int			maxTakeNum;							///< �������Я��������
	int			maxExpMultiple;						///< ���������洢����
	int			maxDExpMultiple;					///< ���������Ϊ����洢����

	int			syncretismMinLev;					///< �̴��ں���С�ȼ�
	int			syncretismMaxCount;					///< �̴��ں�������
	int			syncretismMaxDen;					///< �̴��ں�������

	int			maxLife;							///< �������ֵ
	int			lifeSubRate;						///< ����˥���ٶ�
	int			lifeSubUnit;						///< ����˥����λ
	int			lifeDieDownRate;					///< �����������ͱ���

	int			normalExp;							///< �������鱶��
	int			normalDExp;							///< ������Ϊ����
	int			expsBaseLevDiff;					///< ���鱶�ʻ����ȼ���
	int			expsMaxLevDiff;						///< ���鱶�����ȼ���
	int			levDiffExpRate[7];					///< �ȼ���鱶��
	int			levDiffDExpRate[7];					///< �ȼ�����Ϊ����

	int			lunarMaxUseCnt;						///< ����������ÿ�����ʹ�ô���
	int			lunarMaxUpCnt;						///< ����������ÿ�������ܴ���
	int			lunarBase;							///< ������������� 1000*Lev
	int			lunarReverRate;						///< ����������ظ��ٶ�
	int			lunarReverUnit;						///< ����������ظ���λ���룩

	int			talentResetLev;						///< ���ʵĵȼ��ָ���
	double		talentMaxVal;						///< �������ֵ
	double		talentMinVal;						///< ������Сֵ
	int			talentNumismatics;					///< �����������Ĺű���
	int			talentAlogRate[PET_QUALITY_NUM][3];	///< ���ʼ�������ֵ

	int			qualityMinLev;						///< Ʒ��ǿ����С�ȼ�
	int			qualityLoyaProp;					///< Ʒ��ǿ���ҳ϶ȱ���
	int			qualityDropProp;					///< Ʒ��ǿ���ɹ��ʽ��ͱ���
	int			qualityExpProp;						///< Ʒ��ǿ����ȼ��������
	int			qualityMaxCostGoodsCnt;				///< Ʒ��ǿ����󸶷ѵ�����
	int			qualityNumismatics;					///< Ʒ��ǿ�����Ĺű���
	int			qualitySuccessRate[PET_QUALITY_NUM-1];	///< Ʒ��ǿ���ɹ���

	int			fStoneMinLev;						///< ��ʯǿ����С�ȼ�
	int			fStoneMaxLev;						///< ��ʯǿ�����ȼ�
	int			fStoneExpProp;						///< ��ʯǿ�����ĵȼ��������
	int			fStoneN;							///< ��ʯǿ������Nֵ
	int			fStoneNumismatics;					///< ��ʯǿ�����Ĺű���
	int			fStoneSuccesRate[4];				///< ��ʯǿ���ɹ���

	int			exclamaMinLev;						///< ������λ��С�ȼ�
	int			exclamaLoyaProp;					///< ������λ�ҳ϶ȱ���
	int			exclamaDropProp;					///< ������λ�ɹ��ʽ��ͱ���
	int			exclamaExpProp;						///< ������λ���ĵȼ��������
	double		exclamaSuccesRate[11];				///< ������λ�ɹ���

	int			starMinLev;							///< �ǵ���С�ȼ�
	int			starMaxLev;							///< �ǵ����ȼ�
	int			starNumismatics;					///< �ǵ�ǿ�����Ĺű���
	int			starSuccessRate[10];				///< �ǵ�ǿ���ɹ���
	int			starDropVal[10];					///< �ǵ�ǿ��ʧ�ܽ���ֵ

	int			dAfterStartRate;					///< ��������ǿ����ʼ����
	int			dAfterDropRate;						///< ��������ǿ���ݼ�����
	int			dAfterMaxVal;						///< ��������ǿ����������ʵ���
	int			dAfterNumismatics;					///< ��������ǿ�����Ĺű���

	int			ReSetDAfterNumismatics;				///< ���ú�������ǿ���������Ĺű���

	int			savvyCnt;							///< ����������Ե���
	int			savvyNumismatics;					///< ���Ե��������Ĺű���

	// ����ԭʼ���Ի���
	double		fStrenth2Hit;						///< ����--����
	double		fStrenth2Dodge;						///< ����--����
	double		fStrenth2MAtkSpeed;					///< ����--�����ٶ�
	double		fStrenth2PhysicsAtk;				///< ����--�����˺�
	double		fStrenth2PhysicsDef;				///< ����--�������
	double		fStrenth2MAtk;						///< ����--ħ���˺�
	double		fStrenth2MDef;						///< ����--ħ������

	double		fDexterity2Hit;						///< ��--����
	double		fDexterity2Dodge;					///< ��--����
	double		fDexterity2MAtkSpeed;				///< ��--�����ٶ�
	double		fDexterity2PhysicsAtk;				///< ��--�����˺�
	double		fDexterity2PhysicsDef;				///< ��--�������
	double		fDexterity2MAtk;					///< ��--ħ���˺�
	double		fDexterity2MDef;					///< ��--ħ������

	double		fCon2Hit;							///< ����--����
	double		fCon2Dodge;							///< ����--����
	double		fCon2MAtkSpeed;						///< ����--�����ٶ�
	double		fCon2PhysicsAtk;					///< ����--�����˺�
	double		fCon2PhysicsDef;					///< ����--�������
	double		fCon2MAtk;							///< ����--ħ���˺�
	double		fCon2MDef;							///< ����--ħ������

	double		fIntellect2Hit;						///< ��־--����
	double		fIntellect2Dodge;					///< ��־--����
	double		fIntellect2MAtkSpeed;				///< ��־--�����ٶ�
	double		fIntellect2PhysicsAtk;				///< ��־--�����˺�
	double		fIntellect2PhysicsDef;				///< ��־--�������
	double		fIntellect2MAtk;					///< ��־--ħ���˺�
	double		fIntellect2MDef;					///< ��־--ħ������

	double		fSpiritualism2Hit;					///< ����--����
	double		fSpiritualism2Dodge;				///< ����--����
	double		fSpiritualism2MAtkSpeed;			///< ����--�����ٶ�
	double		fSpiritualism2PhysicsAtk;			///< ����--�����˺�
	double		fSpiritualism2PhysicsDef;			///< ����--�������
	double		fSpiritualism2MAtk;					///< ����--ħ���˺�
	double		fSpiritualism2MDef;					///< ����--ħ������
};

/// ����ԭʼ����
struct tagPetOrig
{
	tagPetOrig(void)
		: lLevel(0)
		, fMaxHp(0)
		, fMaxMp(0)
		, fMaxEnergy(0)
		, fStrenth(0)
		, fDexterity(0)
		, fCon(0)
		, fIntellect(0)
		, fSpiritualism(0)
	{
	}

	const tagPetOrig& operator=(const tagPetOrig& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lLevel;								///< �ȼ�
	double		fMaxHp;								///< ����
	double		fMaxMp;								///< ����
	double		fMaxEnergy;							///< ����
	double		fStrenth;							///< ����
	double		fDexterity;							///< ��
	double		fCon;								///< ����
	double		fIntellect;							///< ��־
	double		fSpiritualism;						///< ����
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
		, lGrade(0)
		, lLife(0)
		, lLoyalty(0)
		, lRace(0)
		, lTakeLevel(0)
		, lHpRecoverSpeed(0)
		, lMpRecoverSpeed(0)
		, lEnergyRecoverSpeed(0)
		, lMinStrenth(0)
		, lMaxStrenth(0)
		, lMinDexterity(0)
		, lMaxDexterity(0)
		, lMinCon(0)
		, lMaxCon(0)
		, lMinIntellect(0)
		, lMaxIntellect(0)
		, lMinSpiritualism(0)
		, lMaxSpiritualism(0)
	{
		memset(szOriName, 0, sizeof(szOriName));
		memset(lQualityRate, 0, PET_QUALITY_NUM*sizeof(long));
		memset(lSkillHole, 0, 3*sizeof(long));
		//memset(lBaseAttrBound, 0, sizeof(lBaseAttrBound));
	}

	const tagPetBase& operator=(const tagPetBase& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lPetType;							///< ��������
	long		lClass;								///< ����������(��������, ս�����, �ٻ�������, ��������)
	long		lIndex;								///< �������
	long		lPicID;								///< ͼ�����
	long		lExistTime;							///< ����ʱ��
	long		lInvisible;							///< ��������
	long		lMoveType;							///< �ƶ�����
	long		lSelAtk;							///< ѡ�񹥻�ս��λ
	long		lGrade;								///< Ʒ��
	long		lLife;								///< ����
	long		lLoyalty;							///< �ҳ϶�
	long		lRace;								///< ����
	long		lTakeLevel;							///< Я���ȼ�

	long		lHpRecoverSpeed;					///< �����ظ��ٶ�
	long		lMpRecoverSpeed;					///< �����ظ��ٶ�
	long		lEnergyRecoverSpeed;				///< �����ظ��ٶ�
	//long		lBaseAttrBound[PET_BASE_NUM][2];	///< ��������������
	long		lMinStrenth;						///< ������������
	long		lMaxStrenth;						///< ������������
	long		lMinDexterity;						///< ����������
	long		lMaxDexterity;						///< ����������
	long		lMinCon;							///< ������������
	long		lMaxCon;							///< ������������
	long		lMinIntellect;						///< ��־��������
	long		lMaxIntellect;						///< ��־��������
	long		lMinSpiritualism;					///< ������������
	long		lMaxSpiritualism;					///< ������������
	long		lNominateMode;						///< �Ƽ��ӵ�

	long		lQualityRate[PET_QUALITY_NUM];		///< Ʒ�ʻ���
	long		lSkillHole[MAX_HOLE_SKILL];			///< ���ܲ�����
	char		szOriName[32];						///< ԭʼ��
	tagGenSkillBase	GeniusSkill;					///< �츳����
	tagGenSkillBase	OrdinarilySkill;				///< ��ͨ����

};

//// �߼���������(������������, �������ݿ��ȡ)
//struct tagPetTemp : public tagPetDetail
//{
//	tagPetTemp(void)
//		: cSaveFlag(0)
//		, lCountry(0)
//		, lHpRecoverSpeed(0)
//		, lMpRecoverSpeed(0)
//		, lEnergyRecoverSpeed(0)
//		, lLoyaltyDelta(0)
//		, lSkillNum(0)
//		, lLoyaltyLev(0)
//		, lEffect(0)
//		, lLastUseSkillTime(0)
//		, lPetLoyaltyLev(-1)
//	{
//	}
//
//	const tagPetTemp& operator=(const tagPetTemp& rTag)
//	{
//		memcpy(this, &rTag, sizeof(rTag));
//		return *this;
//	}
//
//	// ��������ֵ
//	char		cSaveFlag;						///< ����Flag
//	long		lCountry;						///< ����
//	long		lHpRecoverSpeed;				///< �����ظ��ٶ�
//	long		lMpRecoverSpeed;				///< �����ظ��ٶ�
//	long		lEnergyRecoverSpeed;			///< �����ظ��ٶ�
//	long		lLoyaltyDelta;
//	long		lSkillNum;						///< �������������츳���ܺͺ���ѧϰ���ܣ�
//	long		lLoyaltyLev;					///< �ҳ϶ȵȼ�
//	long		lEffect;
//	long		lLastUseSkillTime;				///< ���ʹ�ü���ʱ��
//	long		lPetLoyaltyLev;					///< �ҳ϶ȵȼ�
//};

// ������������(�̴�װ�����̴�ǿ�������ӵ����Լ�)
struct  tagPetOther
{
	long		lPhysicsAbsorb;						///< �����˺�������
	long		lMDef;								///< ��������
	long		lMAbsorb;							///< �����˺�������
	long		lAtkSpeed;							///< �����ٶ�
	long		lMatcSpeed;							///< ʩ���ٶ�
	long		lPhysicsAtk;						///< �����˺�
	long		lMatcAtk;							///< ����������
	long		lMoveSpeed;							///< �ƶ��ٶ�

	long		lBledDef;							///< ��Ѫ����
	long		lDizzyDef;							///< ���ο���
	long		lLullDef;							///< ��Կ���
	long		lHypnosisDef;						///< ���߿���
	long		lPoisonDef;							///< �ж�����
	long		lFixDef;							///< ������
	long		lHpSuck;							///< ������Ѫ��
	long		lMpSuck;							///< ���շ�����
	long		lEnergySuck;						///< ����������
	long		lElemAtk[PP_NUM];					///< ������������˺���
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
//typedef map<char, CGUID> ACT_PETS;
typedef vector<CGUID> ACT_PETS;
typedef const ACT_PETS& C_ACT_PETS;
typedef ACT_PETS::iterator ITR_ACT_PET;
typedef ACT_PETS::const_iterator C_ITR_ACT_PET;
typedef map<CGUID, CPet*> PETS;
typedef const PETS& C_PETS;
typedef PETS::iterator ITR_PET;
typedef PETS::const_iterator C_ITR_PET;

#endif