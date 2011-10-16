/** 
*  @file   GoodsDef.h
*  @author
*
*  @brief  ��Ʒ��ؽṹ����
*  @date   2010/09/03
*/

#pragma once

/// ��Ʒ����
enum eGoodsBaseType
{
	GBT_USELESS,				///< ������
	GBT_NORMAL,					///< ��ͨ
	GBT_CONSUMABLE,				///< �����Ե���
	GBT_CARD,					///< ��Ƭ
	GBT_GOODS,					///< ��Ʒ
	GBT_SUBPACK,				///< �ӱ���
	GBT_PICKGOODS,				///< ʰȡƷ
	GBT_MEDAL,                  ///< ѫ��(�ƺ�)
	GBT_MONEY,	                ///< ���
	GBT_SILVERMONEY,	        ///< ����
	GBT_EQUIPMENT,				///< װ��
	GBT_TRUMP,					///< ����
};

// װ������
enum eEquipType
{
	// װ����
	EC_E_HEAD,				  // ͷ��1
	EC_E_NECKLACE,			  // ����
	EC_E_WING,				  // ���
	EC_E_BODY,				  // ����
	EC_E_BACK,				  // ����5
	EC_E_HAND,                // �ֲ�
	EC_E_SHOULDER,            // �粿
	EC_E_WRIST,			      // ����	
	EC_E_LEG,                 // �Ȳ�
	EC_E_BOOT,			      // Ь��10	
	EC_E_LRING,			      // ���ָ
	EC_E_RRING,			      // �ҽ�ָ		
	EC_E_WEAPON,			  // ����
	EC_E_CARD,			      // ����
	EC_E_GODGOODS,            // ʥ��15
	EC_E_EFFIGY,              // ����
	EC_E_LIFECOWRY,           // ��������
	EC_E_SOULGUARD,           // ����
	EC_E_RIDER,               // ����19   


	// ʱװ��    
	EC_P_HEADGEAR,			// ͷ��
	EC_P_FROCK,			    // ����	
	EC_P_WRIST,             // ����
	EC_P_LOIN,              // ��
	EC_P_NECK,              // ��
	EC_P_AMULET,            // ����
	EC_P_FOOT,              // ��
	EC_P_LEG,               // ��

	//�ƺ���
	EC_MEDAL1,			//	�ƺ�1
	EC_MEDAL2,			//	�ƺ�2
	EC_MEDAL3,			//	�ƺ�3

	EC_TOTAL
};

/// װ�����
enum eEquipClass
{
	EC_NONE,					///< ������
	EC_ATTACK,					///< ����
	EC_DEFENSE					///< ����
};

/// ��Ƭ����
enum eCardType
{
	CARD_CAN_ENCHASE=1,			///< ��Ƕ��
	CARD_ENHANCE_ATTACK,		///< +����
	CARD_ENHANCE_DEFEND,		///< +����

	CARD_CANNOT_ENCHASE,		///< ����Ƕ��
	CARD_COLLECT				///< �Ѽ���
};

/// ICON���
enum eIconType
{
	IT_CONTAINER,				///< ������
	IT_GROUND,					///< ������
	IT_EQUIPED					///< װ����
};

/// �������
enum eSoundType
{
	ST_PICKUP,					///< ʰȡ
	ST_DROP,					///< ����
	ST_USE,						///< ʹ��
	ST_EQUIP,					///< װ��
	ST_FLOURISH,				///< �Ӷ�,δ����
	ST_SLASH					///< ����,���⹥��
};

/// ������λ����
enum eTrumpType
{
	TT_GOD,					///< ������λ8��
	TT_GLAND,				///< ������λ8��
	TT_HUMAN,				///< ������λ8��
	TT_NUM					///< ������λ����
};


/// ��Ʒ������������
enum eGoodsAddonProperties
{
	//////////////////////////////////////////////////////////////////////////
	GAP_UNKNOW=0,				//0 δ֪����
	GAP_GOODS_CATEGORY,			//1 ���ߵ���
	GAP_COLOR,					//2 ��ʾ��ɫ
	GAP_COSPLAYID,				//3 ���ﻻװ���
	GAP_EQUIP_EFFECT1,			//4 װ��������Ч1
	GAP_EQUIP_EFFECT2,			//5 װ��������Ч2
	GAP_EQUIP_EFFECT3,			//6 װ��������Ч3
	GAP_WEAPON_CATEGORY,		//7 �������
	GAP_ROLE_LEVEL_LIMIT,		//8 �ȼ�����
	GAP_SKILL_LIMIT			,//9��������
	GAP_REQUIRE_OCCUPATION	,//10ְҵ����
	GAP_REQUIRE_GENDER			,//11�Ա�����
	GAP_PARTICULAR_ATTRIBUTE,//12���������趨
	GAP_WEAPON_DAMAGE_LEVEL	,//13����ǿ��
	GAP_MIN_ATTACK			,//14��С������
	GAP_MAX_ATTACK			,//15��󹥻���
	GAP_NOR_DEF				,//16���������
	GAP_HURT_SUCK_PER		,//17�˺����ձ���
	GAP_HIT_RATE_CORRECT	,//18������������
	GAP_NORATTACK_KILLRATE	,//19��������һ����
	GAP_FLEE_CORRECT		,//20������������
	GAP_PARRY_CORRECT		,//21����������
	GAP_BLOCK_CORRECT	    ,//22�м���������
	GAP_FUMO_PROPERTY		,//23ħ������
	GAP_MAGIC_KILLRATE		,//24ħ������һ����
	GAP_DEDICANT_CORRECT	,//25רע����
	GAP_MAGIC_SPEED			,//26ʩ���ٶ�
	GAP_MAGIC_DEF			,//27ħ������
	GAP_FIREDEF_CORRECT		,//28������
	GAP_WATERDEF_CORRECT	,//29ˮ������
	GAP_WINDDEF_CORRECT		,//30�翹����
	GAP_EARTHDEF_CORRECT	,//31�ؿ�����
	GAP_HOLYDEF_CORRECT		,//32��ʥ��������
	GAP_DARKDEF_CORRECT		,//33�ڰ���������
	GAP_WILL_DEF			,//34��־����
	GAP_HYPNOSIS_DEF		,//35�������߿���
	GAP_POISON_DEF			,//36�����ж�����
	GAP_FIX_DEF				,//37����������
	GAP_CON_DEF				,//38�������ʿ���
	GAP_BLED_DEF			,//39������Ѫ����
	GAP_DIZZY_DEF			,//40�������迹��
	GAP_LULL_DEF			,//41������Կ���
	GAP_ADD_STR				,//42����׷��
	GAP_ADD_DEX				,//43����׷��
	GAP_ADD_CON				,//44����׷��
	GAP_ADD_INT				,//45����׷��
	GAP_ADD_WIS				,//46�ǻ�׷��
	GAP_ADD_SPI				,//47����׷��
	GAP_MAXHP_CORRECT		,//48������������
	GAP_HP_RESUME_CORRECT	,//49HP�ָ��ٶ�����
	GAP_MAXMP_CORRECT		,//50������������
	GAP_MP_RESUME_CORRECT	,//51�����ָ��ٶ�����
	GAP_ADD_HP				,//52����HP
	GAP_ADD_PERCENT_HP		,//53����HP�ٷֱ�
	GAP_ADD_MP				,//54����MP
	GAP_ADD_PERCENT_MP		,//55����MP�ٷֱ�
	GAP_GOODS_MAX_DURABILITY	,//56��������;�ֵ
	GAP_MAXOVERLAP			,//57������������
	GAP_LEARN_SKILL			,//58ѧϰ���ܵ�ID
	GAP_ADD_SKILL			,//59��������ID
	GAP_LEECH_HP			,//60����HP��
	GAP_LEECH_MP			,//61����MP��
	GAP_HURT2MP				,//62����תMP��
	GAP_KNOCK				,//63����
	GAP_HOME				,//64�س�
	GAP_RAN_TRANS			,//65�������
	GAP_SCRIPT				,//66ִ�нű�
	GAP_GEM_TYPE			,//67������ʯ����
	GAP_GEM_LEVEL				,//68������ʯ����
	GAP_GEM_PROBABILITY		,//69������ʯ�ɹ���������
	GAP_GEM_UPGRADE_SUCCEED_RESULT	,//70������ʯ�ɹ����
	GAP_GEM_UPGRADE_FAILED_RESULT		,//71������ʯʧ�ܽ��	
	GAP_MOUNT_TYPE			,//72�������
	GAP_MOUNT_LEVEL			,//73���Ｖ��
	GAP_MOUNT_ROLE_LIMIT	,//74����������ҽ�ɫ����
	GAP_UNLIMITED_ACCESS		,//75���޴�ʹ��
	GAP_EXCEPTION_STATE		,//76�쳣״̬
	GAP_EXP_BONUS			,//77����EXP����
	GAP_EXPBALL_USERID				,//78ʹ����ID
	GAP_EXPBALL_EXP		,//79���Ǿ���
	GAP_YUANBAO_QUITS		,//80�ֿ�Ԫ��ֵ
	GAP_RECORD_REGIONID		,//81��¼����ID
	GAP_RECORD_COOR			,//82��¼����ID
	GAP_EQUIP_MAXEXP		,//83װ�������
	GAP_SUITID				,//84��װ����ID
	GAP_SUIT_NUMBER			,//85��װ���
	GAP_CREATOR_NAME		,//86����������
	GAP_DEL_TIME			,//87��ʧʱ��
	GAP_LIFE				,//88����
	GAP_GET_TRIGGER			,//89�õ����ߴ����ű�
	GAP_LOST_TRIGGER		,//90ʧȥ���ߴ����ű�
	GAP_HOLE_NUM			,//91��������
	GAP_ENCHASE_TARGET		,//92��ǶĿ�����
	GAP_PACK_TYPE			,//93��������
	GAP_PACK_CAPACITY		,//94��������
	GAP_MIN_ATK_UP		,//95	��С�������ɳ�	
	GAP_MAX_ATK_UP			,//97		��󹥻����ɳ�
	GAP_MAGIC_UP		,//100		ħ�������ɳ�
	
	GAP_DEF_UP			,//102		��������ɳ�
	GAP_MAGICDEF_UP		,//104ħ��������߳ɳ�
	GAP_HIT_RATE_UP			,//105���������ɳ�
	GAP_KILL_RATE_UP		,//106����һ���ʳɳ�
	GAP_MAGIC_KILL_RATE_UP,
	GAP_PARRY_UP			,//107�м����������ɳ�
	GAP_BLOCK_UP			,//108�����������ɳ�
	GAP_FLEE_UP				,//109��������
	GAP_MAXDUR_UP			,//110��������;�ֵ�ɳ�ֵ
	GAP_WEAPON_LEVEL		,//111�����ȼ�
	GAP_MINATK_MODIFY_PER	,//112��С���������ٷֱ�
	GAP_MAXATK_MODIFY_PER	,//113��󹥻������ٷֱ�
	GAP_DEF_MODIFY_PER		,//114��ͨ���������ٷֱ�
	GAP_MAGICATK_MODIFY_PER	,//115ħ�����������ٷֱ�
	GAP_MAGICDEF_MODIFY_PER, //116ħ�����������ٷֱ�
	GAP_GUARD_EQUIP_CATEGORY,//117�������
	GAP_EQUIP_TRIGGER, //118װ��ʱ�����ű�
	GAP_UNEQUIP_TRIGGER, //119жװʱ�����ű�
	GAP_ADD_STATE,//120״̬׷��
	GAP_COOLDOWN,//��ȴʱ��
	GAP_BOTHHAND_GRAPHIC,//˫������ͼ��ID
	GAP_CARD_CATEGORY,//��Ƭ����
	GAP_CARD_ENCHASE_NUM_LIMIT,//��ƬͬһID��Ƕ��������
	GAP_CRI_DAMAGE,//�������˺�
	GAP_CRI_DEF,//����������
	GAP_MCRI_DAMAGE,//ħ�������˺�
	GAP_MCRI_DEF,//ħ����������
	//! GoodsEx����
	GAP_CAN_DISASSEMBLE,	//! �ܷ�ֽ� 125
	GAP_DISASSEMBLE_VALUE,	//! �ֽ��ֵ
	GAP_QUALITY,			//! ��ƷƷ��

	GAP_CURR_POWER,			//! ��ǰ����ֵ
	GAP_MAX_POWER,			//! ��������
	GAP_POWER_INTENSITY,	//! ����ǿ��
	GAP_MIN_REINFORCE_LEVEL,//! �ȼ�����
	GAP_MAX_REINFORCE_LEVEL,//! �ȼ�����

	GAP_BIND,				//! ��
	GAP_FREEZE,				//! ���� 134
	GAP_THAWTIME,			//! �ⶳʱ��

	GAP_ATK_ADD_RATIO,//���������ӳ�ϵ��
	GAP_MAGICATK_ADD_RATIO,//ħ���������ӳ�ϵ��
	GAP_WEAPON_INTEN_UP,//����ǿ�ȳɳ�
	GAP_DEF_ADD_RATIO,//����������ӳ�ϵ��
	GAP_MAGICDEF_ADD_RATIO,//ħ���������ӳ�ϵ��
	GAP_MAX_ENERGY_COR,//������������
	GAP_ENERGY_RESUME_COR,//�����ָ��ٶ�����
	GAP_ENERGY_SUCK,//��������
	GAP_PEOPLE_HURT_RATIO,//������������˺��ӳ�
	GAP_ELEMENT_HURT_RATIO,//��Ԫ��������˺��ӳ�
	GAP_DIFFRACE_HURT_RATIO,//������������˺��ӳ�
	GAP_ANIM_HURT_RATIO,//�Զ���������˺��ӳ�
	GAP_SOUL_HURT_RATIO,//������������˺��ӳ�
	GAP_DEMON_HURT_RATIO,//�Զ�ħ������˺��ӳ�
	GAP_INSECT_HURT_RATIO,//������������˺��ӳ�
	GAP_FIRE_HURT_RATIO,//�Ի�ϵ�����˺��ӳ�
	GAP_WATER_HURT_RATIO,//��ˮϵ�����˺��ӳ�
	GAP_EARTH_HURT_RATIO,//����ϵ�����˺��ӳ�
	GAP_WIND_HURT_RATIO,//�Է�ϵ�����˺��ӳ�
	GAP_DARK_HURT_RATIO,//�԰�ϵ�����˺��ӳ�
	GAP_HOLINESS_HURT_RATIO,//��ʥϵ�����˺��ӳ�
	GAP_GOODS_SELL_PRICE,//����Ʒ����۸�
	GAP_GOODS_BUY_PRICE,//����Ʒ�����۸�

	GAP_PLUG_EVIL_LEVEL,	//! װ��ħ���ȼ�

	GAP_HURT_RID,	//��������
	GAP_MAGIC_HURT_RID,//ħ������
	GAP_UPGRADE_PRICE,	//��Ʒ�����۸�
	GAP_SUBPACK_TYPE,	//�ӱ�������
	GAP_TRADEGOODS_TYPE,//������Ʒ���
	GAP_SHIELD_DEF,//�����������
	GAP_SHIELD_DEF_UP,//������������ɳ�		
	GAP_TIME_LIMIT,	//��ʱ
	GAP_ADDON_CHARM,//��������
	GAP_EQUIP_DEGREE,// װ���Ƚ�
	GAP_EQUIP_SEAL,//װ����ӡ
	GAP_SEAL_ATTR,//��ӡ����
	GAP_GOODS_TYPE,//��Ʒ����
	GAP_REFINE,//����
	GAP_CANNOT_SAVE_DEPOT,//���ܴ�ֿ�
	GAP_HP_RESUME_RATE,//����ֵ�ָ�����
	GAP_BUFF_ATK_STRENGTH,//״̬����ǿ��		

	GAP_RESERVE_CARD,//������Ƭ
	GAP_UNIQUE,//Ψһ��Ʒ
	GAP_HIDE_HEAR,//�Ƿ�����ͷ��
	GAP_HIDE_FACE,//�Ƿ���������
	GAP_HIDE_BODY,//�Ƿ���������
	GAP_NORMAL_DEF_UP,//�������˳ɳ�
	GAP_MAGIC_DEF_UP,//ħ�����˳ɳ�
	GAP_BLUE_CRYSTAL,//�ֽ���ɫ������Ŀ
	GAP_ATK_ED,//����ED��Χֵ
	GAP_CUR_ATK_ED,//��ǰ����EDֵ
	GAP_DEF_ED,//����ED��Χֵ
	GAP_CUR_DEF_ED,//��ǰ����EDֵ

	GAP_EXCUSE_HURT,//����
	GAP_PENETRATE,//��͸
	GAP_CONSTELLATION,// ����
	GAP_LUCKY,//����
	GAP_SILENCE_DEF,//��Ĭ����
	GAP_MEDAL_POINT,//ѫ�»���
	GAP_MIN_ATK_ADDON,// С��׷��
	GAP_MAX_ATK_ADDON,// ��׷��
	GAP_MAGIC_ATK_ADDON,//ħ��׷��
	GAP_DEF_ADDON,//���׷��
	GAP_MAGIC_DEF_ADDON,//ħ��׷��

	GAP_SKILL_CALL_SCRIPT,//���ܵ��ýű�

	GAP_SCRIPT_VAR1,	//�ű�����
	GAP_SCRIPT_VAR2,	//�ű�����
	GAP_SCRIPT_VAR3,	//�ű�����
	GAP_ADD_MAXHP_UP,	//��������׷�ӳɳ�
	GAP_ADD_MAXHP_LEVEL, //���׷����������

	GAP_EXCUSE_HURT_RANGE,	//���˷�Χ
	GAP_PENETRATE_RANGE,	//��͸��Χ
	GAP_LUCKY_RANGE,		//���˷�Χ
	GAP_ROULEAU_MAKE_GOODS,	//����������ƷID	
	GAP_ADDON_HP_RANGE,		//HP����׷�ӷ�Χ
	GAP_RANDOM_ADDON_MAXHP, //�����������׷��

	GAP_ADDON_PENETRATE,//��͸����
	GAP_ADDON_EXCUSE_HURT,//���˸���
	GAP_ADDON_LUCKY,//���˸���
	GAP_ADDON_ATK_ED,//����ED����
	GAP_ADDON_DEF_ED,//����ED����

	GAP_MOUNT_POTENTIAL,//����Ǳ��
	GAP_MOUNT_MAXHP,//������������׷��
	GAP_MOUNT_MIN_ATK,//��������С��׷��
	GAP_MOUNT_MAX_ATK,//���������׷��
	GAP_MOUNT_MAGIC_ATK,//����ħ������׷��
	GAP_MOUNT_PENETRATE,//���ﴩ͸׷��
	GAP_MOUNT_EXCUSE_HURT,//��������׷��

	GAP_PROPERTY_TRIGGER,	//�������Լ���������
	GAP_GOODSDESC_RES_ID, //�ýű�����Ʒ��m_strManufacturerNameд����ַ�����ID��������ʾ���Ϣ��
	GAP_ADD_MAX_PKCP=226,//����PK CPֵ����
	GAP_ADD_MAX_PKCPRANGE,///���ӵ�PK CPֵ��Χ
	GAP_TIMES_MAX_PKCP, //����PK CP����ֵ����ϵ�����˷���
	GAP_TIMES_MAX_PKCPRANGE,//���������Χ 
	GAP_DATE_FOR_SCRIPT, //! �ű����ڣ�Ϊ�ű��ṩ��ʱ�䱣����λ��ֵ�������ɽű�����

	// ���壺ħ���������
	GAP_ELEMENT_1,//! ��Ʒħ�����ԣ�ֵ����ħ�����ԣ�ֵ��ʾħ���ȼ�
	GAP_ELEMENT_2,//! ��Ʒħ�����ԣ�ֵ����ħ�����ԣ�ֵ��ʾħ���ȼ�
	GAP_ELEMENT_3,//! ��Ʒħ�����ԣ�ֵ����ħ�����ԣ�ֵ��ʾħ���ȼ�
	GAP_ELEMENT_XIANG_XING_1,//! ħ������
	GAP_ELEMENT_XIANG_XING_2,//! ħ������
	GAP_ELEMENT_XIANG_XING_3,//! ħ������

	GAP_ENERGY_MAX_GUARDIAN, //��������ػ�ʯ����
	GAP_GUARDIAN_ENERGY_DEC, //��������ֵ��������
	GAP_FBUSINESS_PRICE, //����������Ʒ�۸�

	GAP_STARLEVEL,			// �ǵ�
	GAP_GOODS_USE_LIMIT,	// ��Ʒ���ʱ�ı�ʹ������
	GAP_EQUIP_CATEGORY,		// װ�����
	GAP_TRUMP_CATEGORY,		// �������

	//////////////////////////////////////////////////////////////////////////

	GAP_TATAL_NUM,

	//////////////////////////////////////////////////////////////////////////
	GAP_BASE_HP_UPPER_LIMIT,		///< ������Ѫ����
	GAP_BASE_MP_UPPER_LIMIT,		///< ������������
	GAP_BASE_EP_UPPER_LIMIT,		///< ������������
	GAP_BASE_STAMINA_UPPER_LIMIT,	///< ����ŭ������

	GAP_BASE_HP_RECOVER,			///< ������Ѫ�ָ��ٶ�
	GAP_BASE_MP_RECOVER,			///< ���������ָ��ٶ�
	GAP_BASE_EP_RECOVER,			///< ���������ָ��ٶ�
	GAP_BASE_STAMINA_RECOVER,		///< ����ŭ���ָ��ٶ�

	GAP_FIGHT_HP_RECOVER,			///< ����ս����Ѫ�ָ��ٶ�
	GAP_FIGHT_MP_RECOVER,			///< ����ս�������ָ��ٶ�
	GAP_FIGHT_EP_RECOVER,			///< ����ս�������ָ��ٶ�
	GAP_FIGHT_STAMINA_RECOVER,		///< ����ս��ŭ���ָ��ٶ�

	GAP_ARMOR_HP_1,					///< ������������1
	GAP_ARMOR_HP_2,					///< ������������2
	GAP_ARMOR_HP_3,					///< ������������3

	GAP_BASE_MANLY,					///< ��������
	GAP_BASE_MANLY_UPPER_LIMIT,		///< ������������
	GAP_BASE_EVILLY,				///< ����ɷ��
	GAP_BASE_EVILLY_UPPER_LIMIT,	///< ����ɷ������
	GAP_BASE_RESENT,				///< ����Թ��
	GAP_BASE_RESENT_UPPER_LIMIT,	///< ����Թ������
	GAP_BASE_KILLLY,				///< ����ɱ��
	GAP_BASE_KILLLY_UPPER_LIMIT,	///< ����ɱ������

	GAP_BASE_STR,					///< ��������
	GAP_BASE_DEX,					///< ������
	GAP_BASE_CON,					///< ��������
	GAP_BASE_INTELLECT,				///< ������־
	GAP_BASE_SPIRITUALISM,			///< ��������

	GAP_BASE_PARRY_LEVEL,			///< �����мܵȼ�
	GAP_BASE_PARRY_RATIO,			///< �����мܻ���
	GAP_BASE_DODGE_LEVEL,			///< �������ܵȼ�
	GAP_BASE_DODGE_RATIO,			///< �������ܻ���
	GAP_BASE_ARMOR_DEF,				///< ��������ֵ

	GAP_BASE_DEF_SUCK,				///< �����˺�������
	GAP_BASE_ENERGY_DEF,			///< �����ڹ�����
	GAP_BASE_ENERGY_SUCK,			///< �����ڹ�������
	GAP_BASE_OUTENERGY_HIT_LEVEL,	///< �����⹦���еȼ�
	GAP_BASE_OUTENERGY_HIT_RATIO,	///< �����⹦���л���
	GAP_BASE_ENERGY_HIT_LEVEL,		///< �����ڹ����еȼ�
	GAP_BASE_ENERGY_HIT_RATIO,		///< �����ڹ����л���
	GAP_BASE_OUTENERGY_SPEED_LEVEL,	///< �����⹦���ٵȼ�
	GAP_BASE_OUTENERGY_SPEED_RATIO,	///< �����⹦���ٻ���
	GAP_BASE_ENERGY_SPEED_LEVEL,	///< �����ڹ����ٵȼ�
	GAP_BASE_ENERGY_SPEED_RATIO,	///< �����ڹ����ٻ���

	GAP_BASE_OUTENERGY_INTENSITY,	///< �����⹦ǿ��
	GAP_BASE_OUTENERGY_HERT,		///< �����⹦�˺�
	GAP_BASE_ENERGY_HERT_INC,		///< �����ڹ��˺��ӳ�
	GAP_BASE_ENERGY_HEAL_INC,		///< �����ڹ����Ƽӳ�
	GAP_BASE_OUTENERGY_BLAST_LEVEL,	///< �����⹦�����ȼ�
	GAP_BASE_OUTENERGY_BLAST_RATE,	///< �����⹦������
	GAP_BASE_OUTENERGY_BLAST_HERT,	///< �����⹦�����˺�
	GAP_BASE_ENERGY_BLAST_LEVEL,	///< �����ڹ������ȼ�
	GAP_BASE_ENERGY_BLAST_RATE,		///< �����ڹ�������
	GAP_BASE_ENERGY_BLAST_HERT,		///< �����ڹ������˺�

	GAP_BASE_SLEIGHT_LEVEL,			///< ���������ȼ�
	GAP_BASE_UNFLEE_RATIO,			///< ��������Ŀ�����ܱ���
	GAP_BASE_UNPARRY_RATIO,			///< ��������Ŀ���мܱ���
	GAP_BASE_DEF_LEVEL,				///< ���������ȼ�
	GAP_BASE_POJIA_LEVEL,			///< �����Ƽ׵ȼ�
	GAP_BASE_UNDEF_RATIO,			///< ��������Ŀ�껤�ױ���

	//////////////////////////////////////////////////////////////////////////


	GP_PRICE,					///< ��Ʒ�۸�
	GP_DESCRIBE,				///< ��Ʒ����
	GP_TYPE,					///< ��Ʒ����
	//GP_EQUIPPLACE,				///< װ����λ��ʶ
	GP_ORIGINAL_NAME,			///< ԭʼ����
	GP_NAME						///< ��ʾ��
};

/// ��ҵĽ�ɫ����,���ڳ�������
enum PLAYER_ROLE
{
	PR_NORMAL_PLAYER			= 0x00000001,///< ��ͨ���
	PR_GUILD_MASTER				= 0x00000002,///< ����᳤
	PR_UNION_MASTER				= 0x00000004,///< ���˻᳤
	PR_CITY_MASTER				= 0x00000008,///< ����
};

/// ��������
enum PARTICULAR_ATTRIBUTE
{
	PA_CANNOT_REPAIRED			= 0x00000001,///< ��������
	PA_NEVER_DROP_DOWN			= 0x00000002,///< ��������
	PA_DROP_DOWN_WHEN_LOGOUT	= 0x00000004,///< ���ߺ�����
	PA_DROP_DOWN_WHEN_DEAD		= 0x00000008,///< �����ص���
	PA_DELETE_WHEN_DROP_DOWN	= 0x00000010,///< ��غ����ʧ	
	PA_CANNOT_TRADE_AND_STORE	= 0x00000020,///< ���ɽ���Ҳ���ɴ�ֿⲻ�ɰ�̯
	PA_UNIQUE					= 0x00000040,///< ����	
	PA_DROP_DOWN_WHEN_RECALL	= 0x00000080,///< ʹ�ûسǾ�\˲�ƾ�����	
	PA_RARITY					= 0x00000100///< ���
};

/// ��Ʒ��չ������������
enum eGoodsExWindowType
{
	eGWT_DisassembleWindow,				///< �ֽⴰ��
	eGWT_PrescriptionWindow,			///< �ϳɴ��ڡ��󶨺Ͷ��ᴰ�ڣ�����Ϊħ�У�
	eGWT_UpgradeWindow = 3,				///< ��������
	eGWT_PlusEvilWindow,				///< ħ������
	eGWT_ResetElementTypeWindow,		///< ����ħ�����ʹ���
	eGWT_ResetElementXiangXingWindow,	///< ����ħ�����Դ���
	eGWT_DepotWindow,					///< �ֿⴰ��
	eGWT_CardFuntionWindow,				///< �⿨������ҳ��	
	eGWT_PlayerTradeWindow,				///< ��ҽ��׹���ҳ��
	eGWT_End,
};

/// ��Ƕ��������
enum eEnchaseWindowType
{
	EBWT_OPEN_HOLE=1,					///< ������Ƕ��
	EWT_CLOSE_HOLE,						///< �ر���Ƕ��
	EWT_ENCHASE_CARD,					///< ��Ƕ��Ƭ
	EWT_REMOVE_CARD,					///< �Ƴ���Ƭ
};

/// ��ֵ��������
enum eIncrementConsumeType
{
	eICT_NORMAL_BUY_ITEM=1,				///< ��ͨ����
	eICT_MAIL_BUY_ITEM,					///< �ʼ�����
	//eICT_USE_FUNCTION					///< ʹ����ֵ����
};

/// ��ֵ���򷵻�����
enum eIncrementBuyRet
{
	eICR_BUY_SUCCEED=1,					///< �ɹ�
	eICR_GOLD_NOT_ENOUGH,				///< ��Ҳ���
	eICR_TICKET_NOT_ENOUGH,				///< ��ȯ����
	eICR_WEIMIAN_NOT_ENOUGH,			///< λ�治��
	eICR_PERSONAL_NUM_LIMIT,			///< �����޹�
	eICR_WORLD_NUM_LIMIT,				///< �����޹�
	eICR_TIME_LIMIT						///< ʱ������
};

enum eIncrementFunction
{
	eIFUNC_RELIVE=90000		//ԭ�ظ���
};

/// ��������
enum eWeaponType
{
	WT_UNKNOWN,							///< δ֪
	WT_SINGLE_SWORD,					///< ���ֽ�
	WT_SINGLE_HAMMER,					///< ���ִ�
	WT_SINGLE_KNIFE,					///< ذ��
	WT_WAND,							///< ����
	WT_HEAVY_SWORD,						///< ˫�ֽ�
	WT_HEAVY_AX,						///< ˫�ָ�
	WT_BOW,								///< ��
	WT_DOUBLE_SWORD,					///< ˫��
	WT_FIST								///< ȭ��
};

/// �����ֳ�����
enum eWeaponHandType
{
	WHT_UNKNOWN,						///< δ֪
	WHT_SINGLE_HAND=1,					///< ����
	WHT_DOUBLE_HAND						///< ˫��
};

/// �ӱ�������
enum eSubPackType
{
	SPT_NORMAL=1,						///< ��ͨ�ӱ���
	SPT_BUSINESS						///< ����ר�ñ���
};

/// ������Ʒ����
enum eBusinessGoodsType 
{
	BGT_NOT_BUSINESS_GOODS,				///< δ֪	

	BGT_FOOD,							///< ʳ��
	BGT_ORE,							///< ��ʯ
	BGT_PLANT,							///< ֲ��
	BGT_FIBRE,							///< ��ά
	BGT_ALCOHOL,						///< ��
	BGT_WOOD,							///< ľ��
	BGT_LEATHER,						///< Ƥ��
	BGT_CLOTH,							///< ��
	BGT_WEAPON,							///< ���
	BGT_FLAVOR,							///< ����
	BGT_HANDIWORK,						///< ��Ʒ
	BGT_GEM,							///< ��ʯ

	BGT_MAX_TYPE,						///< �������
};

/// �ʺ�������������
enum eAccountLockRetVal
{
	ACCOUNT_LOCK_OK=1,					///< ����OK
	ACCOUNT_LOCK_ERROR_CDKEY,			///< ����CDKey
	ACCOUNT_LOCK_NOT_ENOUGH,			///< ����
	ACCOUNT_LOCK_UNKNOWN_ERROR			///< δ֪����
};

/// ��Ʒ��չ���ܣ�S2C��Ϣ����
enum eGoodsExTxtCode
{
	eGTC_Failed = 0,					///< ʧ��
	eGTC_Succeed = 1,					///< �ɹ�

	eGTC_SessionLapse = -0xFFFF,		///< �Ի������Զ���Ѿ�ʧЧ
	eGTC_UnknownErr,					///< δ֪ԭ��Ĵ���

	eGTC_CantDisassemble,				///< ���ֽܷ�
	eGTC_CantFindAim,					///< �����ҵ�����Ŀ��
	eGTC_NoEnoughSpace,					///< �����ռ䲻��
	eGTC_SrcIsLock,						///< ԭ��������

	eGTC_NoPrescription,				///< �ϳ��䷽������
	eGTC_ErrSP,							///< ����ĺϳ��䷽

	eGTC_NoArmInHand,					///< ����û������ �����������ܳ��ܣ���
	eGTC_NoReinforceArm,				///< ���ǳ������� �����������ܳ��ܣ���
	eGTC_LevelError,					///< ���ȼ��������ܳ��ܣ���
	eGTC_ArmIsFull,						///< ����������ʱ���ܱ�ͬ������͵������滻����

	eGTC_ErrOperation,					///< ����Ĳ���ֵ
	eGTC_ErrAssistant,					///< �������ϴ���
	eGTC_CantOperateAim,				///< Ŀ�겻�����˲���
	eGTC_DelAssistantFailed,			///< ɾ����������ʧ��

	eGTC_ErrStuff,						///< �����ϴ���
	eGTC_LevelTiptop,					///< �ȼ��ѵ����
	eGTC_NotEnoughMoney,				///< Ǯ����

	eGTC_LimitTooLow,					///< ���Ƶȼ�̫��
	eGTC_QualityTooLow,					///< Ʒ��̫��

	eGTC_NumError,						///< ������������
};

/// װ���������
enum eUpgradeResult
{
	eUR_Succeed,						///< �ɹ�
	eUR_Failed_And_LevelNotChange,		///< ʧ�ܣ��ȼ�δ�ı�
	eUR_Failed_And_Degraded,			///< ʧ�ܣ����ұ�����
};

/// ������ֵ
enum eBindType
{
	eBT_Untie = 0,						///< δ��
	eBT_Bind,							///< �Ѱ�
	eBT_CantBind,						///< ���ܰ�
	eBT_CantUntie,						///< �����ɰ�
	eBT_GetAndCantUntie,				///< �õ����󶨣��Ҳ��ܽ��
	eBT_GetAndBind,						///< �õ����󶨣����Խ��
	eBT_DressAndCantUntie,				///< װ����󶨣��Ҳ��ܽ��
	eBT_DressAndBind,					///< װ����󶨣����Խ��
};


/// ��������ֵ
enum eFrostType
{
	eFT_UnFrost = 0,					///< δ����
	eFT_Frost,							///< �Ѷ���
	eFT_Thawing,						///< �ⶳ��
	eFT_CantThaw,						///< ���ɽⶳ�����ö��ᣩ
	eFT_CantFrost,						///< ���ɶ���
};


/// װ������ʧ�ܺ�Ĳ���
enum eFailedOperation
{
	eFO_Null,							///< �޲���
	eFO_Reduce,							///< ��С�ȼ�
	eFO_SetTo,							///< ���õȼ�Ϊ
};


/// װ��������������Ϲ���
enum eExtraStuffOperate
{
	eESO_Null,							///< �޹���
	eESO_AddOdds,						///< ���ӳɹ���
	eESO_AmendFailOpt,					///< �滻ʧ�ܲ���Ϊ
};

/// ħ�����Ϻϳɰ취
enum eEvilStuffOperation
{
	eESO_1in1 = 1,						///< 1��1
	eESO_2in1,							///< 2��1
	eESO_3in1,							///< 3��1
	eESO_4in1,							///< 4��1
};

/// �ֽ��������������
#define	MAX_DISASSEMBLE_NUM		3U
/// �����Ʒԭʼ���ֳ�
#define MAX_GOOODS_NAME_SIZE	32U
///	���ħ���ȼ�
#define	MAX_ELEMENT_LEVEL		2000U
///	���ħ���ȼ�old
#define	MAX_EVIL_LEVEL			10U
///	�ֽ�����������
#define	DISASSEMBLE_TYPE_NUM	8U
///	�ֽ������������������
#define	DISASSEMBLE_CHILD_TYPE_NUM	10U
///	�Ǹ�ְҵ�����䷽���Ƴ���
#define	MAKE_TABLE_NAME_SIZE	21U
///	�Ǹ�ְҵ�����䷽˵������
#define	MAKE_TABLE_NARRATE_SIZE	2048U
/// ���ϳɲ�������
#define	MAX_STUFF_NUM			15U

/// C2S��Ϣ�� �ϳ�������ṹ
struct tagSynthesizeGoods
{
	CGUID	arrGoodsGuid[MAX_STUFF_NUM];		///< ��ƷGUID
	long	arrContainerFlag[MAX_STUFF_NUM];	///< ��Ʒ��������
	long	arrPos[MAX_STUFF_NUM];				///< ��Ʒ��������λ��
	long	arrNum[MAX_STUFF_NUM];				///< ��Ʒ����
};


/// ����������������
#define	MAX_UPGRADE_STUFF_NUM			1U
/// ����������������
#define	MAX_UPGRADE_ASSISTANT_NUM		3U
/// ����˵��[1������Ŀ�� + MAX_UPGRADE_STUFF_NUM������ + MAX_UPGRADE_ASSISTANT_NUM������]
#define	IN_TAGUPGRADESTUFF_GOODS_NUM	(1 + MAX_UPGRADE_STUFF_NUM + MAX_UPGRADE_ASSISTANT_NUM)

/// C2S��Ϣ�� ��Ȳ��Ͻṹ
struct tagUpgradeStuff
{
	CGUID	arrGoodsGuid[IN_TAGUPGRADESTUFF_GOODS_NUM];		///< ��ƷGUID
	long	arrContainerFlag[IN_TAGUPGRADESTUFF_GOODS_NUM];	///< ��Ʒ��������
	long	arrPos[IN_TAGUPGRADESTUFF_GOODS_NUM];			///< ��Ʒ��������λ��
};

/// 
struct tagEvilStuff
{
	CGUID	GoodsGuid;		///< ��ƷGUID
	long	lContainerFlag;	///< ��Ʒ��������
	long	lPos;			///< ��Ʒ��������λ��
	long	lUseNum;		///< ��������
};

/// ��Ʒ������ȴʱ��
struct tagGoodsCommonCooldown
{
	ulong dwCooldownTime;		///< ������ȴʱ��
	ulong dwCooldownStartTime;	///< ��ʼʱ��
};

/// ������Ʒ
struct tagBusinessGoods
{
	string strOriginName;		///< ԭʼ��
	ulong dwId;					///< ��ƷID
	bool bSell;					///< �Ƿ����
	bool bSpecial;				///< �Ƿ��ز�
	bool bNeed;					///< �Ƿ�����
	ulong dwBear;				///< ������
	ulong dwCategory;			///< ���
	ulong dwBasePrice;			///< ������
	double dBuyPriceMin;		///< �������
	double dBuyPriceMax;		///< �������
	double dSellPriceMin;		///< �������
	double dSellPriceMax;		///< �������
};

/// ����ҵ��������ʱ����CLIENT�Ľṹ
struct tagBusinessGoodsToClient
{
	ulong dwId;					///< ��ƷID
	ulong dwPrice;				///< �۸�
	ulong dwBuyLimit;			///< �޹���
	uchar btSpecial;			///< �Ƿ��ز�
	uchar btNeed;				///< �Ƿ�����Ʒ
};

/// ������Ʒ���
struct tagBusinessGoodsCategory
{
	ulong dwCategoryId;			///< ���ID
	string strName;				///< ����
	ulong dwBuyLimit;			///< �޹���
};

/// ��������������
struct tagTradeSpotData
{
	ulong dwSpotId;										///< ������ID
	ulong dwGameServerId;								///< GS ID
	ulong dwAreaId;										///< ��ΧID
	ulong dwCountryId;									///< ����ID
	map<ulong,tagBusinessGoodsCategory> mapCategory;	///< ���
	map<ulong,tagBusinessGoods> mapGoods;				///< ��Ʒ
};