/** 
* @file   BasePetDef.h 
* @author Fox (yulefox@gmail.com)
* @brief  �������ʹ�õ�ö��/�ṹ/����.
*
* @date   20090106        
*/

#ifndef BASE_PET_DEF_H
#define BASE_PET_DEF_H

const int PET_STUDY_SKILL_TYPE	= 3;			// ��ۼ���������
const int PET_SELECT_WND_CNT = 7;				// ����ѡ�񴰿��г�����λ����

enum PET_ATTR_TYPE
{
	PET_ATTR_UNKNOWN	= 0x0000,
	PET_ATTR_Hp,								///< HP��ǰֵ
	PET_ATTR_MaxHp,								///< HP����
	PET_ATTR_HpRecoverSpeed,					///< HP�ظ��ٶ�

	PET_ATTR_Mp,								///< MP��ǰֵ
	PET_ATTR_MaxMp,								///< MP����
	PET_ATTR_MpRecoverSpeed,					///< MP�ظ��ٶ�

	PET_ATTR_Energy,							///< ������ǰֵ
	PET_ATTR_MaxEnergy,							///< ��������
	PET_ATTR_EnergyRecoverSpeed,				///< �����ظ��ٶ�

	PET_ATTR_Life,								///< ����
	PET_ATTR_Loyalty,							///< �ҳ϶�

	PET_ATTR_Exp,								///< ����ֵ
	PET_ATTR_MaxExp,							///< ����ֵ����
	PET_ATTR_Level,								///< �ȼ�

	PET_ATTR_DExp,								///< ��Ϊ����
	PET_ATTR_MaxDExp,							///< ��Ϊ��������
	PET_ATTR_DLevel,							///< ���еȼ�

	PET_ATTR_Strenth,							///< ����
	PET_ATTR_Dexterity,							///< ��
	PET_ATTR_Con,								///< ����
	PET_ATTR_Intellect,							///< ��־
	PET_ATTR_Spiritualism,						///< ����

	PET_ATTR_Hit,								///< ����
	PET_ATTR_Dodge,								///< ����
	PET_ATTR_MAtkSpeed,							///< �����ٶ�
	PET_ATTR_PhysicsAtk,						///< �����˺�
	PET_ATTR_PhysicsDef,						///< �������
	PET_ATTR_MAtk,								///< ħ������
	PET_ATTR_MDef,								///< ħ������

	PET_ATTR_ElemDef0,							///< ˮ��
	PET_ATTR_ElemDef1,							///< ����
	PET_ATTR_ElemDef2,							///< ľ��
	PET_ATTR_ElemDef3,							///< ��
	PET_ATTR_ElemDef4,							///< ��

	PET_ATTR_Race,								///< ����
	PET_ATTR_TakeLev,							///< Я���ȼ�
	PET_ATTR_StateType,							///< ״̬����
	PET_ATTR_FightType,							///< ս��ģʽ
	PET_ATTR_MoveType,							///< �ƶ�ģʽ
	PET_ATTR_Country,							///< ����
	PET_ATTR_LoyLev,							///< �ҳ϶ȵȼ�

	PET_ATTR_Wild,								///< Ұ����־
	PET_ATTR_Ages,								///< ����
	PET_ATTR_Quality,							///< Ʒ��
	PET_ATTR_Speciality,						///< ����ƫ��
	PET_ATTR_DAfterValCount,					///< ��������ǿ������
	PET_ATTR_StarLev,							///< �ǵ�
	PET_ATTR_StarLevCount,						///< �ǵ�ǿ������

	PET_ATTR_talent_Strenth,
	PET_ATTR_talent_Dexterity,
	PET_ATTR_talent_Con,
	PET_ATTR_talent_Intellect,
	PET_ATTR_talent_Spiritualism,

	PET_ATTR_dAfter_Strenth,
	PET_ATTR_dAfter_Dexterity,
	PET_ATTR_dAfter_Con,
	PET_ATTR_dAfter_Intellect,
	PET_ATTR_dAfter_Spiritualism,

	PET_ATTR_syncretize_Strenth,
	PET_ATTR_syncretize_Dexterity,
	PET_ATTR_syncretize_Con,
	PET_ATTR_syncretize_Intellect,
	PET_ATTR_syncretize_Spiritualism,

	PET_ATTR_stoneUp_Strenth,
	PET_ATTR_stoneUp_Dexterity,
	PET_ATTR_stoneUp_Con,
	PET_ATTR_stoneUp_Intellect,
	PET_ATTR_stoneUp_Spiritualism,

	PET_ATTR_savvy_Strenth,
	PET_ATTR_savvy_Dexterity,
	PET_ATTR_savvy_Con,
	PET_ATTR_savvy_Intellect,
	PET_ATTR_savvy_Spiritualism
};

/// �����������
enum PET_BASE_ATTR
{
	PET_BASE_STRENTH,								///< ����
	PET_BASE_DEXTERITY,								///< ��
	PET_BASE_CON,									///< ����
	PET_BASE_INTELLECT,								///< ��־
	PET_BASE_SPIRITUALISM,							///< ����
	PET_BASE_NUM,									///< ���������������
};

/// �̴�Ʒ��
enum PET_QUALITY
{
	PET_QUALITY_ORDINARILY,							///< Ʒ����ͨ
	PET_QUALITY_CHOICENESS,							///< Ʒ������
	PET_QUALITY_EXCELLENCE,							///< Ʒ������
	PET_QUALITY_BRILLIANT,							///< Ʒ��׿Խ
	PET_QUALITY_NUM,								///< Ʒ������
};

/// �̴Ӽ�������
enum PET_SKILL_TYPE
{
	PET_SKILL_ORDINARILY,							///< ��ͨ��������
	PET_SKILL_GENIUS,								///< �츳����
	PET_SKILL_HEAVEN,								///< ��ϵ����
	PET_SKILL_TERRA,								///< ��ϵ����
	PET_SKILL_HUMAN,								///< ��ͳ����
	PET_SKILL_NUM,									///< ������������
};

/// �̴�����
enum PET_RACE
{
	PP_HUMAN,									///< ����
	PP_GRASS,									///< �ݱ�
	PP_WORM,									///< ��
	PP_CHAOS,									///< ����
	PP_MACHINE,									///< ����
	PP_DRAGON,									///< ��
	PP_ARHAT,									///< �޺�
	PP_GHOST,									///< ����
	PP_PERI,									///< ����
	PP_SHURA,									///< ����
	PP_LUCIFER,									///< ��ħ
	PP_BEAST,									///< ����
	PP_FISH,									///< �㱴
	PP_ELEMENT,									///< Ԫ��
	PP_NUM,										///< �̴���������
};

/// �������ͣ�Ŀǰ�����̴�һ�����ͣ�
enum PET_TYPE
{
	PET_TYPE_NULL		= 0x00,
	//PET_TYPE_ELF		= 0x01,					///< ����
	//PET_TYPE_HUNTER,							///< ս��
	//PET_TYPE_CONJURED,						///< �ٻ���
	//PET_TYPE_TRAP,							///< ����
	//PET_TYPE_ORNA,							///< �γ�
	PET_TYPE_SERVANT,							///< �̴�
	PET_TYPE_COUNT,								///< ������������
	PET_TYPE_ALL		= 0xFF,					///< ��������
};

/// �����ƶ�ģʽ
enum PET_MOVE_STYLE
{
	PET_MOVETYPE_NULL		= 0x00,				///< ���ƶ�ģʽ
	PET_MOVETYPE_STOP		= 0x01,				///< ͣ��
	PET_MOVETYPE_FOLLOW		= 0x02,				///< ����
};

/// ����ս��ģʽ��AIģʽ��
enum PET_FIGHT_STYLE
{
	PET_FIGHETYPE_NULL		= 0x00,				///< ��ս��ģʽ
	PET_FIGHETYPE_DEFEND	= 0x01,				///< ����ģʽ
	PET_FIGHETYPE_FIGHT		= 0x02,				///< Э������ģʽ
};

/// ����״̬
enum PET_STATE
{
	PET_STATE_SLEEP		= 0x00,					///< ��˯״̬
	PET_STATE_AWAIT		= 0x01,					///< ����״̬
	PET_STATE_FIGHT		= 0x02,					///< ��ս״̬
	PET_STATE_ACTIVE	= 0x03,					///< ����״̬
};

/// ���Ｄ��״̬
//enum PET_ACTIVE_STATE
//{
//	PET_AS_UNACTIVE		= 0x00,					///< δ����״̬
//	PET_AS_ACTIVE		= 0x01,					///< ����״̬
//};

//enum PET_SKILL_TYPE
//{
//	PET_SKILL_NULL		= 0x0000,				// δ����
//	PET_SKILL_RED		= 0x0001,				// ��ɫ��������
//	PET_SKILL_YELLOW	= 0x0002,				// ��ɫ��������
//	PET_SKILL_BLUE		= 0x0003,				// ��ɫ�߼�����
//	PET_SKILL_GENIUS	= 0x0010,				// �츳����
//	PET_SKILL_ENCHASED	= 0x1000,				// ����Ƕ
//};

enum PET_OT_AI_TYPE
{
	PET_OT_AI_SET_TARGET	= 0x80,				// �Զ�Ѱ��
	PET_OT_AI_ADD_SKILL		= 0x81,				// ���ܲ���
};

/// ����ǿ������ID
enum PET_ELF_UPGRADE_ID
{
	PET_QUALITYUP_ID		=1000,				///< Ʒ��ǿ��ID
	PET_SAVVY_RESET_ID		=2000,				///< ���Ե�����ID
	PET_FSTONEUP_ID			=3000,				///< ��ʯǿ��ID
	PET_STARTUP_ID			=4000,				///< �ǵ�ǿ��ID
	PET_TALENT_RESET_ID		=5000,				///< ��������ID
	PET_DAFTERUP_ID			=6000,				///< ��������ǿ��ID
	PET_DAFTER_RESET_ID		=7000,				///< ��������ǿ����������ID
	PET_FFUP_ID				=8000,				///< ������λǿ��ID
};


/// ���＼��
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

	bool		bAuto;				///< �Ƿ��Զ��ͷ�
	long		lID;				///< ���
	long		lType;				///< ��������
	long		lPos;				///< ����λ��
	long		lLevel;				///< �ȼ�
	long		lEndCooldown;		///< ��ȴ����ʱ��
	long		lActPos;			///< �������ϵ�λ��
};

// ����ս������/�ɱ�����/��ʾ����
struct tagPetDisp
{
	tagPetDisp(void)
		: lMaxHp(0)
		, lMaxMp(0)
		, lMaxEnergy(0)
		, lStrenth(0)
		, lDexterity(0)
		, lCon(0)
		, lIntellect(0)
		, lSpiritualism(0)
		, lHit(0)
		, lDodge(0)
		, lMAtkSpeed(0)
		, lPhysicsAtk(0)
		, lPhysicsDef(0)
		, lMAtk(0)
		, lMDef(0)
		, lElemDef0(0)
		, lElemDef1(0)
		, lElemDef2(0)
		, lElemDef3(0)
		, lElemDef4(0)
	{
	}

	const tagPetDisp& operator=(const tagPetDisp& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lMaxHp;								///< ����
	long		lMaxMp;								///< ����
	long		lMaxEnergy;							///< ����
	long		lStrenth;							///< ����
	long		lDexterity;							///< ��
	long		lCon;								///< ����
	long		lIntellect;							///< ��־
	long		lSpiritualism;						///< ����
	long		lHit;								///< ����
	long		lDodge;								///< ����
	long		lMAtkSpeed;							///< �����ٶ�
	long		lPhysicsAtk;						///< �����˺�
	long		lPhysicsDef;						///< �������
	long		lMAtk;								///< ħ������
	long		lMDef;								///< ħ������
	long		lElemDef0;							///< ˮ��
	long		lElemDef1;							///< ����
	long		lElemDef2;							///< ľ��
	long		lElemDef3;							///< ��
	long		lElemDef4;							///< ��
};


/// ����������ԣ�һ�����ԣ������ڿ�����ϵ��������ݿ�洢
struct tagPetBaseAttr
{
	tagPetBaseAttr():lStrenth(0)
		,lDexterity(0)
		,lCon(0)
		,lIntellect(0)
		,lSpiritualism(0)
	{}

	const tagPetBaseAttr& operator=(const tagPetBaseAttr& eTag)
	{
		memcpy(this, &eTag, sizeof(eTag));
		return *this;
	}

	long GetTotalVal() const
	{
		return lStrenth + lDexterity + lCon + lIntellect +lSpiritualism;
	}

	void ReSet()
	{
		lStrenth = lDexterity = lCon = lIntellect = lSpiritualism = 0;
	}

	void SetElfBase(const tagPetBaseAttr& eTag)
	{
		lStrenth += eTag.lStrenth;
		lDexterity += eTag.lDexterity;
		lCon += eTag.lCon;
		lIntellect += eTag.lIntellect;
		lSpiritualism += eTag.lSpiritualism;
	}

	long		lStrenth;				///< ����
	long		lDexterity;				///< ��
	long		lCon;					///< ����
	long		lIntellect;				///< ��־
	long		lSpiritualism;			///< ����
};

/// ����ս�����ԣ��������ԣ�����ͨ���������ԣ�һ�����ԣ����㣬����Ҫ���ݽ��д洢
struct tagPetAtkAttr
{
	tagPetAtkAttr():lHit(0)
		,lDodge(0)
		,lMAtkSpeed(0)
		,lPhysicsAtk(0)
		,lPhysicsDef(0)
		,lMAtk(0)
		,lMDef(0)
	{}

	const tagPetAtkAttr& operator=(const tagPetAtkAttr& eTag)
	{
		memcpy(this, &eTag, sizeof(eTag));
		return *this;
	}

	long		lHit;								///< ����
	long		lDodge;								///< ����
	long		lMAtkSpeed;							///< �����ٶ�
	long		lPhysicsAtk;						///< �����˺�
	long		lPhysicsDef;						///< �������
	long		lMAtk;								///< ħ������
	long		lMDef;								///< ħ������
};

/// �̴ӻ������ԣ���Ҫ���ݿ���д洢
struct tagServantBase
{
	tagServantBase():lWild(0)
		, lAges(1)
		, lDLevel(0)
		, lDExp(0)
		, lQuality(0)
		, lSpeciality(0)
		, lDAfterValCount(0)
		, lFStoneUpgradeFlag(-1)
		, lFUpgradeCount(-1)
		, lStarLev(0)
		, lStarLevCount(0)
	{
	}

	const tagServantBase& operator=(const tagServantBase& eTag)
	{
		memcpy(this, &eTag, sizeof(eTag));
		return *this;
	}

	long 		lWild;					///< �Ƿ�Ұ��
	long		lAges;					///< ��ǰ����
	long		lDLevel;				///< ���еȼ�
	long		lDExp;					///< ��Ϊ����
	long		lQuality;				///< Ʒ��
	long		lSpeciality;			///< ����ƫ��
	long		lDAfterValCount;		///< ��������ǿ������

	long		lFStoneUpgradeFlag;		///< ��ʯǿ����λ �� ������λ  ��־ֵ ( 0 , 1 , 2 , 3 , 4 )
	long		lFUpgradeCount;			///< ������λ��ֵ����ǿ���ɹ��Ĵ���
	long		lStarLev;				///< �ǵȵȼ�
	long		lStarLevCount;			///< �ǵ�ǿ������
	tagPetBaseAttr  talentVal;			///< ����ֵ
	tagPetBaseAttr	dAfterVal;			///< ��������ǿ������
	tagPetBaseAttr  syncretizeVal;		///< �ںϺ����ӵ���������
	tagPetBaseAttr  stoneUpVal;			///< ��ʯǿ�����ӵĸ�������
	tagPetBaseAttr	savvyVal;			///< ͨ�����Ե��������
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
		, lEnergy(0)
		, lLife(0)
		, lNameChgTimes(0)
		, lLoyalty(0)
		, lStateType(-1)
	{
		memset(szName, 0, sizeof(szName));
		memset(szHostName, 0, sizeof(szHostName));
	}

	const tagPetDiff& operator=(const tagPetDiff& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	long		lIndex;					///< �������
	long		lPos;					///< ���λ��, 0Ϊ�������
	long		lLevel;					///< �ȼ�
	long		lExp;					///< ����
	long		lHp;					///< ����ֵ
	long		lMp;					///< ħ��ֵ
	long		lEnergy;				///< ����ֵ
	long		lLife;					///< ����
	long		lLoyalty;				///< �ҳ϶�
	long		lNameChgTimes;			///< ��������
	long		lStateType;				///< ״̬���ͣ���@PET_STATE��
	char		szName[32];				///< ��������
	char		szHostName[32];			///< ��������
	tagServantBase ServantAttr;			///< �̴ӻ�������
};

// ������ʱ��������
struct tagPetDetail : public tagPetDisp
{
	tagPetDetail(void)
		: lMoveType(0)
		, lFightType(0)
		, lRace(-1)
		, lMaxExp(0)
		, lMaxDExp(0)
		, lExistTime(0)
		, lTakeLev(0)
		, lPetType(-1)
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
		lMaxEnergy += rTag.lMaxEnergy;
		lStrenth += rTag.lStrenth;
		lDexterity += rTag.lDexterity;
		lCon += rTag.lCon;
		lIntellect += rTag.lIntellect;
		lSpiritualism += rTag.lSpiritualism;
		lHit += rTag.lHit;
		lDodge += rTag.lDodge;
		lMAtkSpeed += rTag.lMAtkSpeed;
		lPhysicsAtk += rTag.lPhysicsAtk;
		lPhysicsDef += rTag.lPhysicsDef;
		lMAtk += rTag.lMAtk;
		lMDef += rTag.lMDef;
		lElemDef0 += rTag.lElemDef0;	
		lElemDef1 += rTag.lElemDef1;	
		lElemDef2 += rTag.lElemDef2;	
		lElemDef3 += rTag.lElemDef3;	
		lElemDef4 += rTag.lElemDef4;	

		return *this;
	}

	long		lMoveType;				///< ����ģʽ
	long		lFightType;				///< ս��ģʽ
	long		lRace;					///< ����
	long		lMaxExp;				///< �ȼ���������
	long		lMaxDExp;				///< ��Ϊ��������
	long		lExistTime;				///< ����ʱ��
	long		lTakeLev;				///< Я���ȼ�
	long		lPetType;				///< ��������
	long		lNominateMode;			///< �Ƽ��ӵ�
};

// �߼���������(������������, �������ݿ��ȡ)
struct tagPetTemp : public tagPetDetail
{
	tagPetTemp(void)
		: cSaveFlag(0)
		, lCountry(0)
		, lHpRecoverSpeed(0)
		, lMpRecoverSpeed(0)
		, lEnergyRecoverSpeed(0)
		, lSkillNum(0)
		, lLastUseSkillTime(0)
		, lLoyLev(-1)
	{
	}

	const tagPetTemp& operator=(const tagPetTemp& rTag)
	{
		memcpy(this, &rTag, sizeof(rTag));
		return *this;
	}

	// ��������ֵ
	char		cSaveFlag;						///< ����Flag
	long		lCountry;						///< ����
	long		lHpRecoverSpeed;				///< �����ظ��ٶ�
	long		lMpRecoverSpeed;				///< �����ظ��ٶ�
	long		lEnergyRecoverSpeed;			///< �����ظ��ٶ�
	long		lSkillNum;						///< �������������츳���ܺͺ���ѧϰ���ܣ�
	long		lLastUseSkillTime;				///< ���ʹ�ü���ʱ��
	long		lLoyLev;						///< �ҳ϶ȵȼ�
};


/// �̴�ǿ��������������
enum eMaterialType
{
	MT_NULL		= 0x00,								///< ������
	MT_MUST		= 0x01,								///< �������
	MT_OPTIONAL = 0x02,								///< �Ǳ������
};

/// ǿ�����Ͻṹ
struct tagMaterial
{
	tagMaterial(void)
		:orderFlag(0)
		,goodsID("")
		,goodsNum(0)
		,rate(0)
	{}

	tagMaterial(long flag, string goodsIdx, long goodsNum, long rate)
		:orderFlag(flag)
		,goodsID(goodsIdx)
		,goodsNum(goodsNum)
		,rate(rate)
	{}

	long orderFlag;									///< ���������־(�������&�Ǳ������)
	string goodsID;									///< ����ID
	long goodsNum;									///< ��������
	long rate;										///< ����(�������Ϊ0���Ǳ����������ǿ���ɹ���)
};

/// �̴�ǿ���������Ͻṹ��������ϺͷǱ�����϶�ʹ�øýṹ��
struct tagMaterialGoods
{
	tagMaterialGoods(void)
		:keyNum(0)
		,key("")
	{
		goodstable.clear();
	}
	int keyNum;										///< ���ϼ�ֵ��Ŀ				
	string key;										///< ���ϼ�ֵ
	vector<tagMaterial> goodstable;					///< ���Ͽ�ѡ�б�
};

/// �̴�ǿ�������б�
struct  tagMaterialList
{
	tagMaterialList(void)
	{
		goodsList.clear();
	}
	string explainText;								///< ǿ����������
	vector<tagMaterialGoods> goodsList;				///< �����б�
};

typedef map<long,tagMaterialList> MATERIAL_LIST;
typedef MATERIAL_LIST::iterator ITR_MATERIAL_LIST;

#endif