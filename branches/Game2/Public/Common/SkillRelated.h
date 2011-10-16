#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_SKILL_RELATED_INCLUDED
#define _INC_SKILL_RELATED_INCLUDED

//#define  _DEBUG_SKILL
//#define  _SKILL_DEBUG


enum eSkillTimerType
{
	SKILL_INTONATE,			//������ʱ
	SKILL_FIRE,			    //ʩ�Ŷ�ʱ
};

enum tagAction
{
	ACTION_WALK,
	ACTION_RUN,
	ACTION_STAND,
	ACTION_FIGHT,
	ACTION_DEFENSE,
	ACTION_CAST_SPELL,
	ACTION_TALK,
	ACTION_USE_ITEM,
};

//##ModelId=4088B5B3009B
enum tagSkillType
{
	//##ModelId=4088B5B300AA
	SKILL_TYPE_ATTACK,
	//##ModelId=4088B5B300BA
	SKILL_TYPE_DEFENSE,
	//##ModelId=4088B5B300CA
	SKILL_TYPE_STATE,
	//##ModelId=4088B5B300D9
	SKILL_TYPE_SUMMON,
	//##ModelId=4088B5B300DA
	SKILL_TYPE_UNKNOW							= 0x7fffffff,
};


enum tagStateID
{
	STATE_RESTORE_HP							= 100000,	// ��Ʒ�ָ�HP
	STATE_RESTORE_MP							= 100001,	// ��Ʒ�ָ�MP
	STATE_AUTOMATIC_RESTORE_HP					= 100002,	// �Զ��ָ�HP
	STATE_AUTOMATIC_RESTORE_MP					= 100003,	// �Զ��ָ�MP
	STATE_RIDE_HORSE							= 100004,	// �����״̬
	STATE_PARTICULAR_STATE						= 100005,	// ����״̬
	STATE_TEAM									= 100006,	// �����ļ״̬
	STATE_USER_GOODS_ENLARGE_MAX_HP				= 100007,	// ʹ�õ�������HP����
	STATE_USER_GOODS_ENLARGE_MAX_MP				= 100008,	// ʹ�õ�������MP����
	STATE_IMPROVE_EXP							= 100009,	// ʹ�õ�������EXP����
	STATE_USER_GOODS_ENLARGE_DEF				= 100010,	// ʹ�õ������ӷ�
	STATE_USER_GOODS_ENLARGE_ELM_DEF			= 100011,	// ʹ�õ�������ħ��
	STATE_USER_GOODS_ENLARGE_FULL_MISS			= 100012,	// ʹ�õ�������ȫ����
	STATE_AUTO_PROTECT							= 110000,	// ���볡��ʱ���Զ��ͷţ����뱣��״̬/
};

enum tagSkillStep
{
	SKILL_OBJECT,					// �ж�Ŀ�����0��
	SKILL_FORCE_OBJECT,				// �ж�ǿ��Ŀ�����
	FIRS_TATT,						// �ж��Ƿ���ԶԷ�pvp��ɫ��������
	SKILL_STAR_BUFF,				// �ж�Ҫʩ�ż��������buff
	SKILL_STAR_STATE,				// �ж�ʩ�ż���ʱ��ұ��봦�ں�����Ϸ״̬
	SKILL_STAR_EQUIP_ID,			// ����Ҫʩ�ż�������װ����id
	SKILL_STAR_EQUIP_PLACE,			// ����Ҫʩ�ż�������װ������ںδ�
	SKILL_STAR_EQUIP_USE,			// �ж�Ҫʩ�ż��ܱ����װ������
	SKILL_ITEM_ID,					// ����ʩ�ż����������id
	SKILL_ITEM_NUM,					// ����ʩ�ż������������Ҫ������
	SKILL_ITEM_PLACE,				// ����ʩ�ż��������������ںδ�
	SKILL_USE_ITEM,					// �ж�ʩ�ż��������������
	SKILL_HP,						// �ж�ʩ�ż�������hp
	SKILL_MP,						// �ж�ʩ�ż�������mp
	SKILL_ENERGY,					// �ж�ʩ�ż�����������
	INTONATE_LEAST_SPACE,			// �ж�������Сʩ�ž���
	INTONATE_MOST_SPACE,			// �ж��������ʩ�ž���
	FIRE_LEAST_SPACE,				// �ж�ʩ����Сʩ�ž���
	FIRE_MOST_SPACE,				// �ж�ʩ�����ʩ�ž���
	INTONATE_ACT,					// ������������ID
	INTONATE_EFFECT,				// ��������������ЧID
	INTONATE_EFFECT_INDEX,			// ����������Ч����
	INTONATE_EFFECT_BY_EQUIP,		// ������Ч�Ƿ����������
	INTONATE_EFFECT_POSITION,		// ����������Ч��λ��
	INTONATE_EFFECT_CYCLE,			// ����������Ч�Ƿ�ѭ��
	INTONATE_PLAY,					// ��ʼ����
	INTONATE_SOUND,					// ����������Чid
	INTONATE_SOUND_BY_EQUIP,		// ������Ч�Ƿ����������
	INTONATE_SOUND_CYCLE,			// ����������Ч�Ƿ�ѭ��
	INTONATE_SOUND_PLAY,			// ��ʼ����������Ч
	INTONATE_BREAK,					// �����Ƿ���Ա����
	RELEASE_ACT,					// ����ʩ�Ŷ���ID
	RELEASE_EFFECT,					// ����ʩ�Ŷ�����Чid
	RELEASE_EFFECT_INDEX,			// ���ö�����Ч����
	RELEASE_EFECT_BY_EQUIP,			// ʩ����Ч�Ƿ����������
	RELEASE_EFFECT_DEST,			// ����ʩ����ЧĿ��
	RELEASE_EFFECT_POSITION,		// ����ʩ����Ч��λ��
	RELEASE_EFFECT_CYCLE,			// ����ʩ����Ч�Ƿ�ѭ��
	RELEASE_PLAY,					// ��ʼʩ�Ŷ���
	RELEASE_SOUND,					// ����ʩ����Чid
	RELEASE_SOUND_BY_EQUIP,			// ʩ����Ч�Ƿ����������
	RELEASE_SOUND_CYCLE,			// ����ʩ����Ч�Ƿ�ѭ��
	RELEASE_SOUND_DEST,				// ����ʩ����ЧĿ��
	RELEASE_SOUND_PLAY,				// ����ʩ����Ч
	SHOCK_RATIO,					// ��Ƶ��(�ٷֱ�)
	SHOCK_INTERVAL,					// �𶯼��ʱ
	SHOCK_TIME_LENGTH,				// ��ʱ�䳤��
	SHOCK_PLAY,						// ��ʼ��
	FLY_EFFECT,						// ���÷�����Чid
	FLY_EFFECT_INDEX,				// ���÷�����Ч����
	FLY_EFFECT_BY_EQUIP,			// ������Ч�Ƿ����������
	FLY_EFFECT_POSITION,			// ���÷�����Ч�Ӻδ�����
	FLY_EFFECT_CYCLE,				// ���÷�����Ч�Ƿ�ѭ������
	FLY_EFFECT_TURNX,				// ���÷�����ЧX�᷽��ƫ�ƽǶ�
	FLY_EFFECT_TURNY,				// ���÷�����ЧY�᷽��ƫ�ƽǶ�
	FLY_PLAY,						// ��ʼ���ж���
	FLY_SOUND,						// ���÷�����Чid
	FLY_SOUND_BY_EQUIP,				// ������Ч�Ƿ����������
	FLY_SOUND_POSITION,				// ���÷�����Ч�Ӻδ��ͷ�
	FLY_SOUND_CYCLE,				// ���÷�����Ч�Ƿ�ѭ��
	FLY_SOUND_PLAY,					// ��ʼ���ŷ�����Ч
	FLY_TIME,						// ���÷���ʱ�䳤��
	FLY_DELAY,						// ���÷����ӳ�ʱ��
	DEL_LOC_EFFECT,					// �趨ɾ����Ч
	DEL_LOC_EFFECT_INDEX,			// �趨ɾ����Ч����
	START_DEL_EFFECT,				// ��ʼɾ����Ч
	SUMMON_EFFECT,					// �����ٻ�������Ч
	SUMMON_SOUND,					// �����ٻ���Ч
	SUMMON_LIFETIME,				// �����ٻ���������
	RANDOM_SUMMONX,					// ��������ٻ�����X�᷶Χ
	RANDOM_SUMMONY,					// ��������ٻ�����Y�᷶Χ
	RANDOM_SUMMON_NUM,				// ��������ٻ������ٻ�������
	INTONATE_TIME,					// ����������ʱ��(72)
	FLY_EFFECT_NUM,					// ������Ч�ĸ���(73)
	INTONATE_MAINACT,				// �����׶ε���Ҫ������һ������������������ѭ������(74)
	SUMMON_DELAYTIMA,				// �ٻ���Ч���ӳٲ���ʱ�䣨75��
	WEAPONEFFECT,					// ������Ч���ű�ʶ��76��
	CHANGEPROPERTY,					// �ı似���ͷ��ߵ�ĳ�����ԣ�77��
	ACTTIME,						// ��ǰ�����Ĺ涨ʱ�䣨78��
	RELEASE_ACTEX,					// �ͷŽ׶εĺ���������79��
	DELAY_CYCELEFFECT_TIME,			// ѭ����Чɾ�����ӳ�ʱ�䣨80��
	LAST_ACTEX_ISCYCEL,				// ���һ���ͷŽ׶εĺ�̶����Ƿ�ѭ�����ţ�81��
	HITEFFECT = 98,					// ������Ч
	HITSOUND = 99,						// ������Ч
	FLASHEFFECT = 100,					// ����Ч��
	FLASHEEFFECTLUM = 101,				// ��������
	FLASHEEFFECTTIME = 102,				// �������ʱ��
	SHAKEEFFECT = 103,				//��Ч��
	SHAKEEFFECTEXTENT = 104,		//�𶯷���ֵ
	SHAKEEFFECTTIME = 105,			//��Ч��ʱ��
};
enum tagSkillID
{
	//#########################################################################
	STATE_BASE_WEAKNESS							= 6322,//����״̬
	STATE_BASE_GOD								= 7777,//�޵�״̬
	STATE_BASE_AC_ENTER							= 6217,//�ս�����Ϸ�ķ����״̬
	STATE_BASE_SUBSPEED							= 6322,//����״̬
	SKILL_BASE_BUFF								= 10000,// �����ͷż���
	SKILL_BASE_ATTACK							= 10001,// ���ֻ�������
	SKILL_BASE_SINGLEHANDED_GLAIVE				= 10002,// ���ֽ���������
	SKILL_BASE_SINGLEHANDED_HAMMER				= 10003,// ���ִ���������
	SKILL_BASE_DAGGER							= 10004,// ذ�׻�������
	SKILL_BASE_MAGIC_CRUTCH						= 10005,// ���Ȼ�������(˫��)
	SKILL_BASE_BOTHHANDS_GLAIVE					= 10006,// ˫�ֽ���������(˫��,һ�ѽ�)
	SKILL_BASE_BOTHHANDS_AX						= 10007,// ˫�ָ���������(˫��)
	SKILL_BASE_BOW								= 10008,// ����������
	SKILL_BASE_BOTH_GLAIVES						= 10009,// ˫����������(˫��,���ѽ�)
	SKILL_BASE_BLADEFIST						= 10010,// ȭ�л�������(˫��)
	// ְҵ����
	SKILL_BASE_NOVICE_OCC						= 10099,// ������ְҵ����
	SKILL_BASE_FIGHTER_OCC						= 10100,// սʿְҵ����
	SKILL_BASE_RABBI_OCC						= 10200,// ��ʦְҵ����
	SKILL_BASE_FAQUIR_OCC						= 10300,// ����ְҵ����
	SKILL_BASE_KNIGHT_OCC						= 10400,// ��ʿְҵ����
	SKILL_BASE_WARRIOR_OCC						= 10500,// ��ʿְҵ����
	SKILL_BASE_SCOUT_OCC						= 10600,// ���ְҵ����

	SKILL_BASE_CRAFTSMAN_OCC					= 10700,// ����ְҵ����
	SKILL_BASE_WARLOCK_OCC						= 10800,// ��ʿְҵ����
	SKILL_BASE_CLERIC_OCC						= 10900,// ��ʦְҵ����
	SKILL_BASE_PALADIN_OCC						= 11000,// ʥ��ʿְҵ����
	SKILL_BASE_DARKKINGHT_OCC					= 11100,// ����ʿְҵ����

	SKILL_BASE_GLADIATOR_OCC					= 11200,// �Ƕ�ʿְҵ����
	SKILL_BASE_BERSERKER_OCC					= 11300,// ��սʿְҵ����
	SKILL_BASE_RANGER_OCC						= 11400,// Ѳ��ְҵ����
	SKILL_BASE_ASSASSIN_OCC						= 11500,// �̿�ְҵ����
	SKILL_BASE_BLADEDANCER_OCC					= 11600,// ������ְҵ����


	SKILL_BASE_BARD_OCC							= 11700,// ����ʫ��ְҵ����
	SKILL_BASE_SORCERER_OCC						= 11800,// ��ʦְҵ����
	SKILL_BASE_NECROMANCER_OCC					= 11900,// ������ʦְҵ����
	SKILL_BASE_BISHOP_OCC						= 12000,// ����ְҵ����
	SKILL_BASE_PROPHET_OCC						= 12100,// ����(��֪)ְҵ����

	SKILL_RIDE_START							= 12201,// ���＼��
	SKILL_RIDE_END								= 12301,// ���＼��
	

	SKILL_DEPUTY_OCCU							= 30000,// ��ְҵ���ܱ�ʾ
	//SKILL_OCCU_COLLECT_SMITHING					= 50001,// ��ְҵ����ɼ�����
	//SKILL_OCCU_COLLECT_METALLURGY				= 50002,// ��ְҵ����ɼ�����
	//SKILL_OCCU_COLLECT_SEWING					= 50003,// ��ְҵ���Ҳɼ�����
	//SKILL_OCCU_COLLECT_OTHER					= 50004,// ��ְҵ�������Ͳɼ�����
	//
	//SKILL_OCCU_FACTURE							= 60000,
	//SKILL_OCCU_FACTURE_SMITHING					= 60001,// ��ְҵ������������
	//SKILL_OCCU_FACTURE_METALLURGY				= 60002,// ��ְҵ������������
	//SKILL_OCCU_FACTURE_SEWING					= 60003,// ��ְҵ������������

	/*SKILL_BASE_ARCHERY							= 1002,
	SKILL_BASE_MAGIC							= 1003,
	SKILL_BASE_STAB								= 1004,
	SKILL_MONSTER_ATTACK						= 1005,
	SKILL_BASE_SUMMON							= 1006,
	SKILL_BASE_RSUMMON							= 1007,
	SKILL_BASE_DEFENCE							= 1008,
	SKILL_BASE_FLASH							= 1009,
	SKILL_BASE_RAINBOW							= 1010,*/


	//�ᴩ�̻�
	//SKILL_BASE_DEFENSE							= 10,

	////#####################
	//SKILL_EX_STATE								= 50,

	////#########################################################################
	//SKILL_MOSOU									= 101,		// ��˫ն
	//SKILL_GHOST_CUT								= 102,		// ��ն
	//SKILL_KNIGHT_CUT							= 103,		// �����ն
	//SKILL_ARMY_BREAK							= 104,		// �ƾ�
	//SKILL_FLASH									= 105,		// һ��
	//SKILL_SWALLOW								= 106,		// �෵
	//SKILL_LEAF_CUT								= 107,		// ҳ�к�
	//SKILL_JU_CUT								= 108,		// �Ӻ�
	//SKILL_RAGE									= 109,		// ŭ�����
	//SKILL_RAGE_BREAK							= 110,		// ŭ����
	//SKILL_SWORDSHIP								= 111,		// ��������
	//SKILL_LIGHTNING_SWORD						= 112,		// ������
	//SKILL_LITTLE_FLASH							= 113,		// Խ��
	//SKILL_THUNDER_SLASH							= 114,		// ���׽�
	//SKILL_RUSH									= 115,		// ���
	//SKILL_PILLAR								= 116,		// ��������
	//SKILL_CALLOSITY								= 117,		// ����
	//SKILL_BLIND									= 118,		// ��Ŀ

	////��2���Ƽ���
	//SKILL_LIGHTNING_SWORD_2						= 119,		// �׽�2
	//SKILL_LIGHTNING_SWORD_3						= 120,		// �׽�3

	//SKILL_GHOST_CUT_2							= 121,		// ��ն2
	//SKILL_GHOST_CUT_3							= 122,		// ��ն2

	//SKILL_ARMY_BREAK_2							= 123,		// �ƾ�2

	//SKILL_RUSH_2								= 124,		// ���2

	//SKILL_CALLOSITY_2							= 125,		// ����2
	//SKILL_LIGHTNING_SWORD_4						= 126,		// �׽�4
	//SKILL_LITTLE_FLASH_2						= 127,		// Խ��2
	//SKILL_LEAF_CUT_2							= 128,		// ҳ�к�2

	//SKILL_AGILITY_2								= 129,		// ��������2


	////#########################################################################

	//SKILL_POISON_FOG							= 201,		// �Ի���
	//SKILL_HEARTLESS_ARROW						= 202,		// �����
	//SKILL_LIGHTING_ARROW						= 203,		// ����
	//SKILL_METEOR_ARROW_MASS						= 204,		// ���Ǽ����䣩
	//SKILL_METEOR_ARROW							= 205,		// ���Ǽ����䣩
	//SKILL_RAIN_ARROW							= 206,		// ��
	//SKILL_POISON_MOTH							= 207,		// ����
	//SKILL_BLOOD_ROSE							= 208,		// Ѫɫõ��
	//SKILL_SCORPION								= 209,		// ��βЫ��
	//SKILL_BOA_LOCK								= 210,		// ������
	//SKILL_HEAL									= 211,		// ҽ��
	//SKILL_MONSTER_TAMING						= 212,		// ѱ��
	//SKILL_FALLING_STAR							= 213,		// ��׹
	//SKILL_EXPLOSIVE_ARROW						= 214,		// ��ըʸ
	//SKILL_PETS_CONTROL							= 215,		// ���﹥��
	//SKILL_GIBE									= 216,		// ����ĳ���
	//SKILL_SUPER_HEAL							= 217,		// ������

	//SKILL_AGILITY								= 218,		// ��������
	//SKILL_RAPTURE								= 219,		// ȫ���ע
	//SKILL_NATURAL								= 220,		// ��Ȼ֮��
	//SKILL_STRIKE								= 221,		// ���

	//SKILL_MACHINE_SHIELD						= 222,		// ���ػ���

	//SKILL_DAUB_POISON							= 223,		// Ϳ��

	////��2���Ƽ���
	//SKILL_SWORDSHIP_2							= 224,		// ��������2
	//SKILL_EXPLOSIVE_ARROW_2						= 225,		// ��ըʸ2
	//SKILL_EXPLOSIVE_ARROW_3						= 226,		// ��ըʸ3
	//SKILL_HEAL_2								= 227,		// ҽ��2
	//SKILL_SUPER_HEAL_2							= 228,		// ������2
	//SKILL_HEARTLESS_ARROW_2						= 229,		// �����2
	//SKILL_HEARTLESS_ARROW_3						= 230,		// �����3
	//SKILL_LIGHTING_ARROW_2						= 231,		// ����2
	//SKILL_SWORDSHIP_3							= 232,		// ��������3
	//SKILL_SWORDSHIP_4							= 233,		// ��������4

	////#########################################################################
	//SKILL_TAIJI									= 301,		// ̫����
	//SKILL_WEAK									= 302,		// ̫����
	//SKILL_GOD_BLESS								= 303,		// ��վ�
	//SKILL_ORIGIN								= 304,		// ���칦
	//SKILL_CURE									= 305,		// ������
	//SKILL_FIRE_BOLT								= 306,		// ڤ����
	//SKILL_LIGHTNING								= 307,		// ����
	//SKILL_FIRE_WALL								= 308,		// �������
	//SKILL_INFERNOL								= 309,		// ��������
	//SKILL_SEVEN_SHOOTING_STAR					= 310,		// ���ǻ�â
	//SKILL_CHAOS_SPHERE							= 311,		// ������
	//SKILL_SEAL									= 312,		// ��ħ��
	//SKILL_YINYANG								= 313,		// ������
	//SKILL_GOD_PUNISHMENT						= 314,		// �췣
	//SKILL_SOUL_COLLECT							= 315,		// ������
	//SKILL_SOUL_MIRROR							= 316,		// ���꾵
	//SKILL_FIRE_BALL								= 317,		// ��ڤ����
	//SKILL_CHAIN_LIGHTNING						= 318,		// ����
	//SKILL_THUNDER_BLOW							= 319,		// �ױ�
	//SKILL_GOD_THUNDER							= 320,		// ������
	//SKILL_MANASHIELD							= 321,		// ��������
	//SKILL_PROMOTION								= 322,		// ����

	////#########################################################################
	//SKILL_GOD_THUNDER_2							= 323,		// ������2
	//SKILL_HEARTEN								= 324,		// ���
	//SKILL_GOD_BLESS_2							= 325,		// ��վ�2
	//SKILL_YINYANG_2								= 326,		// ������2

	////#########################################################################
	////##����ļ���
	//SKILL_SPIDER_POISON							= 401,		// ֩�붾��
	//SKILL_KNOCK_OUT								= 402,		// �丸�Ļ���
	//SKILL_SNOW_STORM							= 403,		// ѩ����ѩ��
	//SKILL_CORPSE_CANDLE_BLASTING				= 404,		// ����Ա�
	//SKILL_SPORE_BLASTING						= 405,		// �����Ա�
	//SKILL_YAKSHA_SLASH							= 406,		// �ߵ�������ն
	//SKILL_MONSTER_THORN							= 407,		// ����ش�
	//SKILL_SPIDER_MIST							= 408,		// �˽ǽ�����
	//SKILL_SPIDER_WEB							= 409,		// �˽ǲ���
	//SKILL_SUMMON_CORPSE_CANDLE					= 410,		// �ٻ����
	//SKILL_SUMMON_SKELETON						= 411,		// �ٻ�����
	//SKILL_SUMMON_SPORE							= 412,		// �ٻ�����

	//SKILL_CHUCK_STONE							= 413,		// ɽ����ʯ
	//SKILL_YUNSHENG_LIGHTNING					= 414,		// ��������
	//SKILL_CORPSE_PTOMAINE						= 415,		// ʬ��
	//SKILL_ENERGY_BOLT							= 416,		// а��ڤ��
	//SKILL_SKELETON_ARCHERY						= 417,		// ���乭��
	//SKILL_ZOMBIE_CLAW							= 418,		// ���Ĺ�צ

	//SKILL_FURY									= 419,		// ��
	//SKILL_LITTLE_STAR							= 420,		// С����
	//SKILL_SNAKE_BOLT							= 421,		// ���ߵ���
	//SKILL_SPRITE_BURN							= 422,		// ��ħ����
	//SKILL_MACHINERY_STOMP						= 423,		// ���ؼ�̤

	//SKILL_MONSTER_HEAL							= 424,		// �����Ѫ����

	////���������
	//SKILL_LORD_FAST_ATTACK						= 501,		// ������칥
	//SKILL_LORD_WIDERANGING_ATTACK				= 502,		// ������湥

	//SKILL_BOSS_BLUE_FURY						= 503,		// ������ŭ
	//SKILL_BOSS_BLUE_QUAKE						= 504,		// �������

	//SKILL_BOSS_FIEND_SUMMON						= 505,		// а���ٻ�
	//SKILL_BOSS_FIEND_PENETRATE					= 506,		// а��ᴩ


	////#########################################################################
	//// ��2�¼���
	//SKILL_ENLARGE_MAX_HP						= 601,		// ����HP���޵ı�������
	//SKILL_ENLARGE_MAX_MP						= 602,		// ����MP���޵ı�������
	//SKILL_ENLARGE_FULL_MISS						= 603,		// ����FULLMISS��������

	////*****************
	//SKILL_NON_FUN								= 900,		//�����ü��ܣ�����Ϊ��ʾ

	//SKILL_NON_FUN_1								= 901,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_2								= 902,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_3								= 903,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_4								= 904,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_5								= 905,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_6								= 906,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_7								= 907,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_8								= 908,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_9								= 909,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_10							= 910,		//�����ü��ܣ�����Ϊ��ʾ

	//SKILL_NON_FUN_11							= 911,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_12							= 912,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_13							= 913,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_14							= 914,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_15							= 915,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_16							= 916,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_17							= 917,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_18							= 918,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_19							= 919,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_20							= 920,		//�����ü��ܣ�����Ϊ��ʾ

	//SKILL_NON_FUN_21							= 921,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_22							= 922,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_23							= 923,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_24							= 924,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_25							= 925,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_26							= 926,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_27							= 927,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_28							= 928,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_29							= 929,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_30							= 930,		//�����ü��ܣ�����Ϊ��ʾ

	//SKILL_NON_FUN_31							= 931,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_32							= 932,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_33							= 933,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_34							= 934,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_35							= 935,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_36							= 936,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_37							= 937,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_38							= 938,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_39							= 939,		//�����ü��ܣ�����Ϊ��ʾ
	//SKILL_NON_FUN_40							= 940,		//�����ü��ܣ�����Ϊ��ʾ

	//***************************
	SKILL_UNKNOW								= 0x7fffffff,
};

enum tagSkillUsage
{
	//##ʹ��������ʧ�Ļ�������
	SKILL_USAGE_USER_HP_LOSE					= 1,
	SKILL_USAGE_USER_MP_LOSE					= 2,
	SKILL_USAGE_USER_RP_LOSE					= 3,
	SKILL_USAGE_USER_YP_LOSE					= 4,

	SKILL_USAGE_USER_HP_PERCENT_LOSE			= 5,
	SKILL_USAGE_USER_MP_PERCENT_LOSE			= 6,
	SKILL_USAGE_USER_RP_PERCENT_LOSE			= 7,
	SKILL_USAGE_USER_YP_PERCENT_LOSE			= 8,

	//##��ʩ��������ʧ�Ļ�������
	SKILL_USAGE_TARGET_HP_LOSE					= 11,
	SKILL_USAGE_TARGET_MP_LOSE					= 12,
	SKILL_USAGE_TARGET_RP_LOSE					= 13,
	SKILL_USAGE_TARGET_YP_LOSE					= 14,

	SKILL_USAGE_TARGET_HP_PERCENT_LOSE			= 15,
	SKILL_USAGE_TARGET_MP_PERCENT_LOSE			= 16,
	SKILL_USAGE_TARGET_RP_PERCENT_LOSE			= 17,
	SKILL_USAGE_TARGET_YP_PERCENT_LOSE			= 18,

	//##ʩ���������ӵĻ�������
	SKILL_USAGE_USER_HP_GAIN					= 21,
	SKILL_USAGE_USER_MP_GAIN					= 22,
	SKILL_USAGE_USER_RP_GAIN					= 23,
	SKILL_USAGE_USER_YP_GAIN					= 24,

	SKILL_USAGE_USER_HP_PERCENT_GAIN			= 25,
	SKILL_USAGE_USER_MP_PERCENT_GAIN			= 26,
	SKILL_USAGE_USER_RP_PERCENT_GAIN			= 27,
	SKILL_USAGE_USER_YP_PERCENT_GAIN			= 28,

	//##��ʩ���������ӵĻ�������
	SKILL_USAGE_TARGET_HP_GAIN					= 31,
	SKILL_USAGE_TARGET_MP_GAIN					= 32,
	SKILL_USAGE_TARGET_RP_GAIN					= 33,
	SKILL_USAGE_TARGET_YP_GAIN					= 34,

	SKILL_USAGE_TARGET_HP_PERCENT_GAIN			= 35,
	SKILL_USAGE_TARGET_MP_PERCENT_GAIN			= 36,
	SKILL_USAGE_TARGET_RP_PERCENT_GAIN			= 37,
	SKILL_USAGE_TARGET_YP_PERCENT_GAIN			= 38,

	//###############################################################
	//##��ʩ���ߵĻ������Եĸı�
	SKILL_USAGE_TARGET_STR_GAIN					= 101,
	SKILL_USAGE_TARGET_DEX_GAIN					= 102,
	SKILL_USAGE_TARGET_CON_GAIN					= 103,
	SKILL_USAGE_TARGET_INT_GAIN					= 104,
	SKILL_USAGE_TARGET_ATK_GAIN					= 105,
	SKILL_USAGE_TARGET_HIT_GAIN					= 106,
	SKILL_USAGE_TARGET_BURDEN_GAIN				= 107,
	SKILL_USAGE_TARGET_CCH_GAIN					= 108,
	SKILL_USAGE_TARGET_DEF_GAIN					= 109,
	SKILL_USAGE_TARGET_DODGE_GAIN				= 110,
	SKILL_USAGE_TARGET_ATK_SPEED_GAIN			= 111,
	SKILL_USAGE_TARGET_ELEMENT_RESISTANT_GAIN	= 112,
	SKILL_USAGE_TARGET_HP_RECOVER_SPEED_GAIN	= 113,
	SKILL_USAGE_TARGET_MP_RECOVER_SPEED_GAIN	= 114,
	SKILL_USAGE_TARGET_ELEMENT_MODIFY_GAIN		= 115,
	SKILL_USAGE_TARGET_MIN_ATK_GAIN				= 116,
	SKILL_USAGE_TARGET_MAX_ATK_GAIN				= 117,
	SKILL_USAGE_MAX_HP_GAIN						= 118,
	SKILL_USAGE_MAX_MP_GAIN						= 119,

	SKILL_USAGE_TARGET_ELEMENT_MODIFY_GAIN_COEFFICIENT	= 120,	// ����Ŀ��Ԫ��ϵ��coefficient
	SKILL_USAGE_TARGET_MIN_ATK_GAIN_COEFFICIENT			= 121,	// ����Ŀ����С����ϵ��coefficient
	SKILL_USAGE_TARGET_MAX_ATK_GAIN_COEFFICIENT			= 122,	// ����Ŀ����󹥻�ϵ��

	SKILL_USAGE_TARGET_DEF_GAIN_COEFFICIENT		= 123,			// ����Ŀ��DEFϵ��coefficient
	SKILL_USAGE_TARGET_ER_GAIN_COEFFICIENT		= 124,			// ����Ŀ��ER��Ԫ�صֿ�����ϵ��

	SKILL_USAGE_TARGET_BLAST_COEFFICIENT_GAIN			= 125,	// ����������ϵ��
	SKILL_USAGE_TARGET_ELEMENTBLAST_COEFFICIENT_GAIN	= 126,	// ���ӷ�������ϵ��
	SKILL_USAGE_TARGET_FULLMISS_GAIN					= 127,
	SKILL_USAGE_ATTACK_AVOID_COEFFICIENT_GAIN	= 128,
	SKILL_USAGE_ELEMENT_AVOID_COEFFICIENT_GAIN	= 129,

	//###############################################################
	SKILL_USAGE_TARGET_STR_LOSE					= 201,
	SKILL_USAGE_TARGET_DEX_LOSE					= 202,
	SKILL_USAGE_TARGET_CON_LOSE					= 203,
	SKILL_USAGE_TARGET_INT_LOSE					= 204,
	SKILL_USAGE_TARGET_ATK_LOSE					= 205,
	SKILL_USAGE_TARGET_HIT_LOSE					= 206,
	SKILL_USAGE_TARGET_BURDEN_LOSE				= 207,
	SKILL_USAGE_TARGET_CCH_LOSE					= 208,
	SKILL_USAGE_TARGET_DEF_LOSE					= 209,
	SKILL_USAGE_TARGET_DODGE_LOSE				= 210,
	SKILL_USAGE_TARGET_ATK_SPEED_LOSE			= 211,
	SKILL_USAGE_TARGET_ELEMENT_RESISTANT_LOSE	= 212,
	SKILL_USAGE_TARGET_HP_RECOVER_SPEED_LOSE	= 213,
	SKILL_USAGE_TARGET_MP_RECOVER_SPEED_LOSE	= 214,
	SKILL_USAGE_TARGET_ELEMENT_MODIFY_LOSE		= 215,
	SKILL_USAGE_TARGET_MIN_ATK_LOSE				= 216,
	SKILL_USAGE_TARGET_MAX_ATK_LOSE				= 217,
	SKILL_USAGE_MAX_HP_LOSE						= 218,
	SKILL_USAGE_MAX_MP_LOSE						= 219,

	SKILL_USAGE_TARGET_ELEMENT_MODIFY_LOSE_COEFFICIENT	= 220,	// ����Ŀ��Ԫ��ϵ��coefficient
	SKILL_USAGE_TARGET_MIN_ATK_LOSE_COEFFICIENT			= 221,	// ����Ŀ����С����ϵ��coefficient
	SKILL_USAGE_TARGET_MAX_ATK_LOSE_COEFFICIENT			= 222,	// ����Ŀ����󹥻�ϵ��


	SKILL_USAGE_TARGET_DEF_LOSE_COEFFICIENT		= 223,			// ����Ŀ��DEFϵ��coefficient
	SKILL_USAGE_TARGET_ER_LOSE_COEFFICIENT		= 224,			// ����Ŀ��ER��Ԫ�صֿ�����ϵ��


	SKILL_USAGE_TARGET_BLAST_COEFFICIENT_LOSE			= 225,	// ����������ϵ��
	SKILL_USAGE_TARGET_ELEMENTBLAST_COEFFICIENT_LOSE	= 226,	// ���ٷ�������ϵ��
	SKILL_USAGE_TARGET_FULLMISS_LOSE					= 227,
	SKILL_USAGE_ATTACK_AVOID_COEFFICIENT_LOSE	= 228,
	SKILL_USAGE_ELEMENT_AVOID_COEFFICIENT_LOSE	= 229,
	//###############################################################
	//SKILL_USAGE_TARGET_ATK_MULTIPLE_GAIN		= 705,
	//SKILL_USAGE_TARGET_ATK_MULTIPLE_LOSE		= 805,

	//##����Ч��
	SKILL_USAGE_TARGET_BACK_STEP				= 1001,
	SKILL_USAGE_TARGET_CANNOT_MOVE				= 1002,

	//##����Ч����ص�һЩ����
	SKILL_USAGE_TARGET_MOVE_SPEED				= 2001,			//##�ƶ��ٶ�
	SKILL_USAGE_MIN_ANGLE						= 2002,			//##��С�Ƕ�
	SKILL_USAGE_MAX_ANGLE						= 2003,			//##���Ƕ�

	//##������Χ
	SKILL_USAGE_TARGET_ATTACK_SQUARE			= 5001,
	SKILL_USAGE_TARGET_ATTACK_LINE				= 5002,
	SKILL_USAGE_TARGET_MAX_DISTANT				= 5003,
	SKILL_USAGE_TARGET_MIN_DISTANT				= 5004,

	//##Ƶ����صĲ���
	SKILL_USAGE_TARGET_AFFECT_FREQUENCY			= 6001,			//##Ƶ��

	//##��������ʱ�Ĳ���
	SKILL_USAGE_DELAY_TIME						= 10001,		//##����׼��ʱ��
	SKILL_USAGE_STATE_PERSIST_TIME				= 10002,		//##״̬����ʱ��
	SKILL_USAGE_STATE_PERSIST_TIME_MODIFIER		= 10003,		//##״̬����ʱ������
	SKILL_USAGE_STATE_PRESENT_DEGREE			= 10004,		//##״̬���̶ֳ�
	SKILL_USAGE_REUSE_SKILL_DELAY_TIME			= 10005,
	SKILL_USAGE_CAN_BE_BREAKED					= 10006,		//##�ܷ񱻴��
	SKILL_USAGE_SKILL_PERSIST_TIME				= 10007,		//##���ܵ����ʹ��ʱ��
	SKILL_USAGE_MISSILE_FLYING_TIME				= 10008,		//##���������ʱ�䣬��λ������/��
	SKILL_USAGE_ACTION_INTERVAL					= 10009,		//##�������
	SKILL_USAGE_STATE_HP						= 10010,		// ״̬����ֵ

	//##ʱ����صĲ���
	SKILL_USAGE_FIRST_TIME						= 15001,		//##��һ��ʱ��
	SKILL_USAGE_SECOND_TIME						= 15002,		//##�ڶ���ʱ��
	SKILL_USAGE_THIRD_TIME						= 15003,		//##������ʱ��
	SKILL_USAGE_TIME_LIMIT						= 15004,		//##ʱ������
	SKILL_USAGE_TIME_PERCENT					= 15005,		//##ʱ��ٷֱ�

	//##ս����صĲ���
	SKILL_USAGE_USER_HIT_MODIFIER				= 20001,		//##�����ʵ�����(����������������)
	SKILL_USAGE_TARGET_FINAL_DAMAGE_MODIFIER	= 20002,		//##�����˺�����(����������������)
	SKILL_USAGE_TARGET_DAMAGE_FACTOR			= 20003,		//##�˺��ӳ�ϵ��
	SKILL_USAGE_USER_DEX_TO_DAMAGE_RATE			= 20004,		//##���ݵ��˺��Ļ���ϵ��
	SKILL_USAGE_USER_DAMAGE_FACTOR				= 20005,		//##�����������ӳ�ϵ��
	SKILL_USAGE_ADDITION_EX_STATE				= 20006,		//##�������쳣״̬
	SKILL_USAGE_ADDITION_EX_STATE_RATE			= 20007,		//##�������쳣״̬�ļ���
	SKILL_USAGE_MIN_ATTACK						= 20008,		//##��С������
	SKILL_USAGE_MAX_ATTACK						= 20009,		//##��󹥻���
	SKILL_USAGE_CONST							= 20010,		//##����

	SKILL_USAGE_MARJO_TARGET_DAMAGE_FACTOR		= 20011,		//##��ҪĿ����˺��ӳ�ϵ��
	SKILL_USAGE_MINOR_TARGET_DAMAGE_FACTOR		= 20012,		//##��ҪĿ����˺��ӳ�ϵ��

	SKILL_USAGE_ADDITION_ELEMENT_ATK			= 20013,		//##���ӵ�Ԫ�ع�����
	SKILL_USAGE_ADDITION_SOUL_ATK				= 20014,		//##���ӵ���ϵ������
	SKILL_USAGE_EM_MODIFIER						= 20015,		//##Ԫ������������ֵ[�ٷֱ� 0 - N]

	SKILL_USAGE_AMOUNT							= 20016,		//##����
	SKILL_USAGE_AMOUNT_LIMIT					= 20017,		//##��������

	SKILL_USAGE_WEAPON_DAMAGE_LEVEL_MODIFIER	= 20018,		// ����ǿ������
	SKILL_USAGE_HEAL_RECOVER_COEFFICIENT		= 20019,		// ���ƻظ�ϵ��
	SKILL_USAGE_PARAMETER_PERCENT				= 20020,		// �����ٷֱ�

	SKILL_USAGE_TARGET_DAMAGE_FACTOR2			= 20021,		//##�˺��ӳ�ϵ��2
	SKILL_USAGE_TARGET_DAMAGE_FACTOR3			= 20022,		//##�˺��ӳ�ϵ��3
	SKILL_USAGE_TARGET_DAMAGE_FACTOR4			= 20023,		//##�˺��ӳ�ϵ��4

	SKILL_USAGE_TARGET_HP_DECREASE_FACTOR		= 20024,		// HP ����ϵ��
	SKILL_USAGE_TARGET_MP_DECREASE_FACTOR		= 20025,		// MP ����ϵ��

	//##�ٻ���صĲ���
	SKILL_USAGE_SUMMONED_CREATURE_LIEF_TIME		= 30001,		//##�ٻ�������ʱ��
	SKILL_USAGE_SUMMONED_CREATURE_SPEED			= 30002,		//##�ٻ����ƶ��ٶ�,��λ:����/��s
	SKILL_USAGE_SUMMONED_CREATURE_ID			= 30003,		//##�ٻ��޵�ID
	SKILL_USAGE_SUMMONED_CREATURE_ID2			= 30004,		//##�ٻ��޵�ID2
	SKILL_USAGE_SUMMONED_CREATURE_ID3			= 30005,		//##�ٻ��޵�ID3

	//##�������
	SKILL_USAGE_PET_LEVEL_LIMIT					= 31000,		//##�������߼�������
	SKILL_USAGE_PET_AMOUNT_LIMIT				= 31001,		//##�������������

	//##�������
	SKILL_USAGE_BASE_PROBABILITY				= 40001,		//##��С�ĸ���

	//�������
	SKILL_USAGE_ITEM_IDX						= 50001,		// ��Ʒ��������
	SKILL_USAGE_ITEM_NUM						= 50002,		// ��Ʒ������

	SKILL_USAGE_UNKNOW							= 0x7fffffff,
};

enum tagUseSkillResult
{
	SKILL_USE_RESULT_BEGIN,						//##��ʾ�������㣬��ʼʹ�ü���
	SKILL_USE_RESULT_FAILED,
	SKILL_USE_RESULT_END,						//##ʩ������
};

enum tagUseFailed
{
	SKILL_USE_FAILED_INVALID_UNKNOWN=0,			//δ֪ʧ��	
	SKILL_USE_FAILED_INVALID_HP,
	SKILL_USE_FAILED_INVALID_MP,
	SKILL_USE_FAILED_INVALID_RP,
	SKILL_USE_FAILED_INVALID_YP,
	SKILL_USE_FAILED_INVALID_TARGET,
	SKILL_USE_FAILED_INVALID_DISTANT,
	SKILL_USE_FAILED_INVALID_SKILL,
	SKILL_USE_FAILED_INVALID_TIME,				//##�����ٴ�ʹ�ü��ܵ�ʱ��δ����
    SKILL_USE_FAILED_INVALID_CDTIME,            //CDʱ��
	SKILL_USE_FAILED_INVALID_EQUIP,				//##��Ч��װ��
	SKILL_USE_FAILED_BLOCKED,					//##���赲��	
	SKILL_USE_FAILED_BREAK,						//�ж�
	SKILL_USE_FAILED_NOOBJECT,					//û������Ŀ��
	SKILL_USE_FAILED_INVALID_INTONATE,			//����û�н���
	SKILL_USE_FAILED_INVALID_OCCUPATION,		//ְҵ
	SKILL_USE_FAILED_INVALID_FACTURE,			//����ʧ��
	SKILL_USE_FAILED_INVALID_COLLECT,			//�ɼ�ʧ��
	SKILL_USE_FAILED_INVALID_ACT,				//����û�����
	SKILL_USE_FAILED_INVALID_MOVING,			//�ƶ��в���ʹ�ü���

	SKILL_USE_FAILED_C_NOCOLLECTION,			//�޲ɼ�����
	SKILL_USE_FAILED_C_STATE_ERROR,				//���ܱ��ɼ�״̬
	SKILL_USE_FAILED_C_ROLE_PROTECT,			//���񱣻�״̬
	SKILL_USE_FAILED_C_UNKNOW_COLLECTION,		//δ֪�ɼ���
	SKILL_USE_FAILED_C_DOCCU_ERROR,				//��ְҵ����
	SKILL_USE_FAILED_C_LEVEL_ERROR,				//�ɼ��ȼ�����
	SKILL_USE_FAILED_C_GOODSCONDITION_ERROR,	//��Ʒ����������
	SKILL_USE_FAILED_C_NO_BAGSPACE,				//�����ռ䲻����
	SKILL_USE_FAILED_C_NOT_ROLECOLLECTION,		//��������ɼ���
	SKILL_USE_FAILED_C_QUESTSTEP_ERROR,			//�����費�ܶ�Ӧ
	SKILL_USE_FAILED_C_UNKOWN_COLLECTTYPE,		//δ֪�Ĳɼ�����
	SKILL_USE_FAILED_CANNOTCOLLECTION_STATE,	//��ǰ״̬���ܲɼ�

	SKILL_USE_FAILED_F_NO_FACTURE,				//�����ڵ�������
	SKILL_USE_FAILED_F_DOCCU_ERROR,				//��ְҵ����
	SKILL_USE_FAILED_F_NOTHAS_FACTURE,			//�޴�������Ŀ
	SKILL_USE_FAILED_F_GOODSCONDITION_ERROR,	//��������
	SKILL_USE_FAILED_F_MATERIALS_ERROR,			//ԭ������
	SKILL_USE_FAILED_F_NPC_ERROR,				//npc��������
	SKILL_USE_FAILED_F_NO_BAGSPACE,				//�����ռ�����
	SKILL_USE_FAILED_CANNOTFACTURE_STATE,		//��ǰ״̬��������

	SKILL_USE_FAILED_OBJECT_INGOD,				//Ŀ�괦���޵�״̬
	SKILL_USE_FAILED_INVALID_REGION,			//����������ʩ������
	SKILL_USE_FAILED_INVALID_STATE,				//##��ǰ״̬����ʹ�ü��ܣ����类���
    SKILL_USE_FAILED_INVALID_NOTSAMEPHASE,      //��ͬ����λ,���ܻ���ʹ�ü���
    SKILL_USE_FAILED_INVALID_NOTUSESKILL,       //��ǰ״̬����ʹ�ü���
    SKILL_USE_FAILED_INVALID_FULLDISTRACT,      //��������ֵ

	
	SKILL_USE_NO_FAILED,						//�ɹ�
};
enum tagStudyFailed
{
	SKILL_STUDY_FAILED_OCCUPATION,				//ְҵ������ѧϰ����
	SKILL_STUDY_FAILED_RANK,					//�ȼ�������ѧϰ����
	SKILL_STUDY_FAILED_SP,						//SP������ѧϰ����
	SKILL_STUDY_FAILED_SKILL,					//ǰ�ü��ܲ�����ѧϰ����
	SKILL_STUDY_FAILED_LIMIT_SKILL,				//ǰ�����Ƽ��ܲ���������
	SKILL_STUDY_FAILED_GOOD,					//��Ʒ
	SKILL_STUDY_FAILED_GOLD,					//��Ҳ���
	SKILL_STUDY_FAILED_SILVER,					//���Ҳ���
	SKILL_STUDY_FAILED_OCCULV,					//ְҵ�ȼ�����������
	
};

enum tagDamageType
{
	DAMAGE_TYPE_NO,
	DAMAGE_TYPE_HP,
	DAMAGE_TYPE_MP,
    DAMAGE_TYPE_TAUNT,
};

struct tagExState
{
	ushort	type;		// ����
	ulong	level;		//�ȼ�					2
	ulong	keeptime;	//״̬����ʱ��			4

	ushort	maxhp;		//����MAXHP����		2
	ushort	maxmp;		//����MAXMP����		2

	ushort	minatk;		//����Ŀ����СATK		2
	ushort	maxatk;		//����Ŀ�����ATK		2

	ushort	elm;		//����Ŀ��Ԫ�ع�����	2
	ushort	def;		//����Ŀ��DEF			2
	ushort	elmdef;		//����Ŀ��Ԫ�صֿ���	2

	ushort	cch;		//����Ŀ��CCH			2
	ushort	fullmiss;	//����FULLMISSϵ��		2
	ushort	atkavoid;	//����AttackAvoidϵ��	2
	ushort	elmavoid;	//����ElementAvoidϵ��	2
	ushort	hit;		//��������				2
	ushort	dodge;		//����Ŀ��DODGE			2

	ulong	item_idx;	// �����������
	ulong	item_num;	// ��������
	ulong	frequency;	// ��λ�����Ƶ��
};



#endif