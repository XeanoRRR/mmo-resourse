#pragma once
#include "../../Public/Module/GUID.h"
//���������һЩ��Ϸ�ṹ��ö��

/* ��Ϸ�е���ͨ����� 
AddBy Ф�� 09.10.13*/
enum MERCHANTABLE_THING
{
    MT_XINGZHUAN = 0,	// ����
    MT_WEIMIAN ,		// λ��
    MT_DIANJUAN ,		// ���
    MT_GOLD ,			// ���
    MT_SILVERMONEY,	    // ����
    MT_NUM
};

enum COUNTRY_TYPE
{
    COUNTRY_STAR = 1,		// ��
    COUNTRY_SEA,			// ��
    COUNTRY_CLOUD,			// ��
    COUNTRY_LUNA,			// ��
    COUNTRY_SAND,			// ɰ
    COUNTRY_WIND,			// ��
    COUNTRY_UPPER			// ����
};

enum eOccupation
{
    OCC_Novice=0,			//������
    OCC_Fighter,			//սʿ
    OCC_Mystic,				//��ʦ(���෨ʦ)
    OCC_Rogue,				//����
    OCC_Knight,				//��ʿ
    OCC_Warrior,			//��ʿ
    OCC_Scout,				//���
    OCC_Craftsman,			//����
    OCC_Warlock,			//��ʿ
    OCC_Cleric,				//��ʦ
    OCC_Paladin,			//ʥ��ʿ
    OCC_DarkKnight,			//����ʿ
    OCC_Gladiator,			//�Ƕ�ʿ
    OCC_Berserker,			//��սʿ
    OCC_Ranger,				//Ѳ��
    OCC_Assassin,			//�̿�
    OCC_BladeDancer,		//������
    OCC_Bard,				//����ʫ��
    OCC_Sorcerer,			//��ʦ
    OCC_Necromancer,		//������ʦ
    OCC_Bishop,				//����
    OCC_Prophet,			//����(��֪)

    OCC_Max,				//ְҵ�����
};

typedef enum eConstellation
{
    CONST_No=0,				//��
    CONST_Aries,			//������
    CONST_Taurus,			//��ţ��
    CONST_Gemini,			//˫����
    CONST_Cancer,			//��з��
    CONST_Leo,				//ʨ����
    CONST_Virgo,			//��Ů��
    CONST_Libra,			//�����
    CONST_Scorpio,			//��Ы��
    CONST_Sagittarius,		//������
    CONST_Capricorn,		//Ħ����
    CONST_Aquarius,			//ˮƿ��
    CONST_Pisces,			//˫����
}eConst;

//�����������
const int MONSTER_SKILL_SUM=6;
const int MONSTER_AI_REFERRENCE_NUM=5;

// [070622 AHC]
//������������:	�����Ե�C++��������
enum ATTRIBUTE_DATA_TYPE
{
    ATTR_DATA_BYTE = 0,
    ATTR_DATA_WORD,
    ATTR_DATA_DWORD,
    ATTR_DATA_FLOAT,
    ATTR_DATA_LONG,
    ATTR_DATA_SHORT,
    ATTR_DATA_CHAR,
    ATTR_DATA_BOOL,

    ATTR_DATA_BYTEARRAY,
    ATTR_DATA_WORDARRAY,
    ATTR_DATA_DWORDARRAY,
    ATTR_DATA_FLOATARRAY,
    ATTR_DATA_LONGARRAY,
    ATTR_DATA_SHORTARRAY,
    ATTR_DATA_CHARARRAY,
    ATTR_DATA_BOOLARRAY,
};
/**
    S_ATTR_ROOT,                //����
    S_ATTR_DIZZY,               //����
    S_ATTR_PARALYSIS,           //̱��״̬
    S_ATTR_BANISH,              //����
    S_ATTR_BAFFLE,              //����
    S_ATTR_CONFUSE,             //����
    S_ATTR_DREAD,               //�־�
 */
//MoveShape��������:	�������������ö��ֵ
enum S_ATTRIBUTE_TYPE
{
    S_ATTR_No=0,				//��
    S_ATTR_INVISIBLETIME,	    //����
    S_ATTR_HIDE,			    //����
    S_ATTR_PRETENDDEADTIME,     //����
    S_ATTR_CHANGEMODELTYPE,     //�ı�����
    S_ATTR_CHANGEMODELID,	    //�ı�ID
    S_ATTR_USEINSTRUCTION,      //�޷�ʹ��ָ��
    S_ATTR_MOVEABLE,		    //�ܷ��ƶ�
    S_ATTR_FIGHTABLE,		    //�ܷ񹥻�
    S_ATTR_USEFIGHTSKILL,	    //�ܷ�ʹ��ս������
    S_ATTR_USERABBSKILL,	    //�ܷ�ʹ�÷�ʦ����
    S_ATTR_USEITEM, 		    //ʹ�õ���
    S_ATTR_ANTIINVISIBLEABLE,	//������
    S_ATTR_ADDHP,			    //��������HP
    S_ATTR_UNATTACK,		    //���ɹ���
    S_ATTR_FORCEATTACK,		    //ǿ�ƹ���
    S_ATTR_RAMDOMMOVE,		    //����ܶ�
    S_ATTR_SHAPESTATE,          //Cshape state
    S_ATTR_SPEED,			    //�ٶȣ�shape�ϵģ�
    S_ATTR_COLLECTIONID,	    //�ɼ���id
    S_ATTR_COLLECTIONTIMES,	    //�ܲɼ��Ĵ���
    S_ATTR_ISBEENCOLLECT,	    //�Ƿ����ڱ��ɼ�
    S_ATTR_CANBEENCOLLECT,	    //�Ƿ��ܱ��ɼ�
    S_ATTR_NOTRIDE,			    //������״̬
    S_ATTR_SELFCONTROL,         //�Ƿ���Կ��ƽ�ɫ
    S_ATTR_USEALLSKILL,         //�Ƿ����ʹ�����еļ���
    S_ATTR_MOVEDAMAGE,          //�ܵ��˺��Ƿ��Ƴ�
    S_ATTR_CANBEENATTACK,       //�ܷ񱻹���
    S_ATTR_MOVECONTROL,         //�ƶ�״̬Ч������

    S_ATTR_UNKOWN,              //�޷�ʶ������� 1BYTE
    S_ATTR_END = 0x3F,
};
//player��������:	�������������ö��ֵ
enum P_ATTRIBUTE_TYPE
{
    P_ATTR_MONEY = 0x3F+1,
    P_ATTR_YUANBAO,
    P_ATTR_FACTIONTITLE,		            //string����
    P_ATTR_FACTIONNAME,			            //string����
    P_ATTR_ENEMYFACTIONS,		            //longarray����
    P_ATTR_FACTIONID,
    P_ATTR_FACTIONLOGOID,
    P_ATTR_FACMASTERID,
    P_ATTR_FACTIONLVL,
    P_ATTR_FACTIONEXP,
    P_ATTR_FACTIONCONTROBUTE,
    P_ATTR_UNIONID,
    P_ATTR_UNIONMASTERID,
    P_ATTR_CONTENDSTATE,
    P_ATTR_CITYWARDIEDSTATE,
    P_ATTR_CITYWARDIEDSTATETIME,			//������ר��
    P_ATTR_CREATEFACTIONOPERATOR,			//������ר��
    P_ATTR_APLLYJOINOPERATOR,				//������ר��
    P_ATTR_FACTIONDELCAREOPERATOR,			//������ר��
    //////////////////////�ͻ����Ƿ���Ҫ����////////////////////////
    P_ATTR_TYPE,
    P_ATTR_ID,
    P_ATTR_GRAPHICSID,
    P_ATTR_REGIONID,
    P_ATTR_TILEX,
    P_ATTR_TILEY,
    P_ATTR_DIR,
    P_ATTR_POS,
    P_ATTR_SPEED,
    P_ATTR_STATE,
    P_ATTR_ACTION,
    //////////////////////////////////////////////////////////////////
    /*��������*/
    P_ATTR_OCCU,
    P_ATTR_DOCCU,//��ְҵ
    P_ATTR_SEX,
    P_ATTR_HEADPIC,
    P_ATTR_FACEPIC,
    P_ATTR_HP,
    P_ATTR_MP,
    P_ATTR_ENERGY,
    P_ATTR_STAMINA,
    P_ATTR_LEVEL,
    P_ATTR_EXP,
    P_ATTR_EXPMULTIPLE,
    P_ATTR_PRESENTEXP,
    P_ATTR_BUDDHISM,
    P_ATTR_DRYNUM,
    P_ATTR_DRYENDTIME,
    P_ATTR_OCCULVL0,//������ְҵ�ȼ�
    //...����ְҵ�ȼ�
    P_ATTR_OCCULVL21=P_ATTR_OCCULVL0+OCC_Prophet,//����ְҵ�ȼ�
    P_ATTR_OCCUEXP0,//������ְҵ����
    //...����ְҵ����
    P_ATTR_OCCUEXP21=P_ATTR_OCCUEXP0+OCC_Prophet,//����ְҵ����
    P_ATTR_CUROCCUUPGRADEEXP,//��ǰְҵ�ȼ�����������

    P_ATTR_TAOISMLVL,
    P_ATTR_TAOISMEXP,
    P_ATTR_RELATIONVAL,
    P_ATTR_HIDDENVIRTUE,
    P_ATTR_VIGOUR,
    P_ATTR_THEW,

    P_ATTR_COUNTRY,
    P_ATTR_COUNTRYCONTRIBUTE,
    P_ATTR_COUNTRYSTATUS,

    P_ATTR_SPOUSEID,
    P_ATTR_SPOUSEPARAM,
    P_ATTR_BUSINESSLEVEL,
    P_ATTR_BUSINESSEXP,

    P_ATTR_PKCOUNT,
    P_ATTR_PKVALUE,
    P_ATTR_PVPCOUNT,
    P_ATTR_PVPVALUE,

    P_ATTR_PKONOFF,
    P_ATTR_OCCUSP0,//������ְҵSP
    //...����ְҵSP
    P_ATTR_OCCUSP21=P_ATTR_OCCUSP0+OCC_Prophet,//����ְҵSP
    P_ATTR_UPGRADEEXP,
    P_ATTR_SHOWCOUNTRY,
    P_ATTR_HOTKEY0,
    P_ATTR_HOTKEY1,
    P_ATTR_HOTKEY2,
    P_ATTR_HOTKEY3,
    P_ATTR_HOTKEY4,
    P_ATTR_HOTKEY5,
    P_ATTR_HOTKEY6,
    P_ATTR_HOTKEY7,
    P_ATTR_HOTKEY8,
    P_ATTR_HOTKEY9,
    P_ATTR_HOTKEY10,
    P_ATTR_HOTKEY11,
    P_ATTR_HOTKEY12,
    P_ATTR_HOTKEY13,
    P_ATTR_HOTKEY14,
    P_ATTR_HOTKEY15,
    P_ATTR_HOTKEY16,
    P_ATTR_HOTKEY17,
    P_ATTR_HOTKEY18,
    P_ATTR_HOTKEY19,
    P_ATTR_HOTKEY20,
    P_ATTR_HOTKEY21,
    P_ATTR_HOTKEY22,
    P_ATTR_HOTKEY23,
    /*ս������*/
    P_ATTR_MAXHP,
    P_ATTR_MAXMP,
    P_ATTR_MAXSTAMINA,
    P_ATTR_MAXENERGY,	
    P_ATTR_HPRECOVERRATIO,
    P_ATTR_HPRECOVERSPEED,	
    P_ATTR_MPRECOVERSPEED,	
    P_ATTR_ENERRECOVERSPEED,
    P_ATTR_STAMRECOVERSPEED,		
    P_ATTR_FIGHTHPRECOVERSPEED,
    P_ATTR_FIGHTMPRECOVERSPEED,
    P_ATTR_FIGHTENERRECOVERSPEED,
    P_ATTR_FIGHTSTAMRECOVERSPEED,

    P_ATTR_MANLY,
    P_ATTR_PERVERSETREND,
    P_ATTR_COMPLAINT,
    P_ATTR_MURDEROUSLOOK,

    P_ATTR_STR,
    P_ATTR_DEX,
    P_ATTR_CON,
    P_ATTR_INTELLECT,
    P_ATTR_SPIRITUALISM,
    P_ATTR_POINT,
    P_ATTR_PARRY,
    P_ATTR_DODGE,
    P_ATTR_DEF,
    P_ATTR_DEFSUCK,
    P_ATTR_MDEF,
    P_ATTR_MDEFSUCK,
    P_ATTR_BLOODSUCK,
    P_ATTR_MANASUCK,
    P_ATTR_ENERGYSUCK,
    P_ATTR_STAMINASUCK,
    P_ATTR_HIT,
    P_ATTR_ATKSPEED,
    P_ATTR_MATKSPEED,
    P_ATTR_MINATK,
    P_ATTR_MAXATK,
    P_ATTR_MINMATK,
    P_ATTR_MAXMATK,
    P_ATTR_PHYSICIMMUNITY,
    P_ATTR_MAGICIMMUNITY,
    P_ATTR_MOVESPEED,
    P_ATTR_MOVEACCELERATION,
    P_ATTR_MOVERADIUS,
    P_ATTR_TURNSPEED,
    P_ATTR_ELEMDEF_FIRE,
    P_ATTR_ELEMDEF_WATER,
    P_ATTR_ELEMDEF_EARTH,
    P_ATTR_ELEMDEF_WIND,
    P_ATTR_ELEMDEF_DARK,
    P_ATTR_ELEMDEF_HOLY,
    P_ATTR_DEBUFFDEF_BLED,
    P_ATTR_DEBUFFDEF_DIZZY,
    P_ATTR_DEBUFFDEF_LULL,
    P_ATTR_DEBUFFDEF_HYPNOSIS,
    P_ATTR_DEBUFFDEF_POSION,
    P_ATTR_DEBUFFDEF_FIX,
    P_ATTR_DEBUFFDEF_SILENCE,
    P_ATTR_CLASSDAM_HUMAN,
    P_ATTR_CLASSDAM_ELEMENT,
    P_ATTR_CLASSDAM_ALIEN,
    P_ATTR_CLASSDAM_ANIMAL,
    P_ATTR_CLASSDAM_UNDEAD,
    P_ATTR_CLASSDAM_EVIL,
    P_ATTR_CLASSDAM_HEXAPODS,

    P_ATTR_STRENTH,
    P_ATTR_AGILITY,	
    P_ATTR_CONSTI,
    P_ATTR_INTELL,
    P_ATTR_WISDOM,
    P_ATTR_SPIRIT,
    P_ATTR_BLOCK,
    P_ATTR_CHARM,
    P_ATTR_ABSORB,	
    P_ATTR_SHIELD_DEF,
    P_ATTR_CRI,
    P_ATTR_BUFFPOWER,
    P_ATTR_MATK,
    P_ATTR_MCRI,
    P_ATTR_WILLDEF,
    P_ATTR_CONSTIDEF,
    P_ATTR_ELEMDAM_FIRE,
    P_ATTR_ELEMDAM_WATER,
    P_ATTR_ELEMDAM_EARTH,
    P_ATTR_ELEMDAM_WIND,
    P_ATTR_ELEMDAM_DARK,
    P_ATTR_ELEMDAM_HOLY,
    P_ATTR_ELEMATK_FIRE,	
    P_ATTR_ELEMATK_WATER,	
    P_ATTR_ELEMATK_EARTH,	
    P_ATTR_ELEMATK_WIND,	
    P_ATTR_ELEMATK_DARK,	
    P_ATTR_ELEMATK_HOLY,	
    P_ATTR_CRIDEF,
    P_ATTR_MCRIDEF,
    P_ATTR_CRIDAMAGE,
    P_ATTR_MCRIDAMAGE,
    P_ATTR_IMMUNITY,
    P_ATTR_PIERCE,
    P_ATTR_LUCK,
    P_ATTR_CONCEALMENT,
    P_ATTR_HURTFACTOR,

    /*����ս������*/
    P_ATTR_BASEMAXHP,
    P_ATTR_BASEMAXMP,		
    P_ATTR_BASEMAXSTAMINA,
    P_ATTR_BASEMAXENERGY,
    P_ATTR_BASEHPRECOVERSPEED,
    P_ATTR_BASEMPRECOVERSPEED,
    P_ATTR_BASESTAMRECOVERSPEED,	
    P_ATTR_BASESTRENTH,
    P_ATTR_BASEAGILITY,
    P_ATTR_BASECONSTI,
    P_ATTR_BASEINTELL,
    P_ATTR_BASEWISDOM,
    P_ATTR_BASESPIRIT,		
    P_ATTR_BASEBLOCK,		
    P_ATTR_BASEPARRY,	
    P_ATTR_BASEDODGE,
    P_ATTR_BASECHARM,
    P_ATTR_BASEHIT,
    P_ATTR_BASEABSORB,
    P_ATTR_BASEDEF,
    P_ATTR_BASEMDEF,		
    P_ATTR_BASECRI,		
    P_ATTR_BASEMATKSPEED,	
    P_ATTR_BASEBUFFPOWER,
    P_ATTR_BASEMINATK,
    P_ATTR_BASEMAXATK,
    P_ATTR_BASEMATK,		
    P_ATTR_BASEMCRI,		
    P_ATTR_BASEBLOODSUK,		
    P_ATTR_BASEMANASUCK,		
    P_ATTR_BASEWILLDEF,	
    P_ATTR_BASECONSTIDEF,
    P_ATTR_BASEDEBUFFDEF_BLED,
    P_ATTR_BASEDEBUFFDEF_DIZZY,
    P_ATTR_BASEDEBUFFDEF_LULL,
    P_ATTR_BASEDEBUFFDEF_HYPNOSIS,
    P_ATTR_BASEDEBUFFDEF_POSION,
    P_ATTR_ATTR_BASEDEBUFFDEF_FIX,
    P_ATTR_BASEELEMDEF_FIRE,
    P_ATTR_BASEELEMDEF_WATER,
    P_ATTR_BASEELEMDEF_EARTH,
    P_ATTR_BASEELEMDEF_WIND,
    P_ATTR_BASEELEMDEF_DARK,
    P_ATTR_BASEELEMDEF_HOLY,
    P_ATTR_BASECLASSDAM_HUMAN,
    P_ATTR_BASECLASSDAM_ELEMENT,
    P_ATTR_BASECLASSDAM_ALIEN,
    P_ATTR_BASECLASSDAM_ANIMAL,
    P_ATTR_BASECLASSDAM_UNDEAD,
    P_ATTR_BASECLASSDAM_EVIL,
    P_ATTR_BASECLASSDAM_HEXAPODS,
    P_ATTR_BASEELEMDAM_FIRE,
    P_ATTR_BASEELEMDAM_WATER,
    P_ATTR_BASEELEMDAM_EARTH,
    P_ATTR_BASEELEMDAM_WIND,
    P_ATTR_BASEELEMDAM_DARK,
    P_ATTR_BASEELEMDAM_HOLY,
    P_ATTR_BASECRIDEF,
    P_ATTR_BASEMCRIDEF,
    P_ATTR_BASECRIDAMAGE,
    P_ATTR_BASEMCRIDAMAGE,
    P_ATTR_SAVEDTIME,
    P_ATTR_REMAINPOINT,
    P_ATTR_DISPLAYHEADPIECE,

    P_ATTR_COLLECTIONEXP,
    P_ATTR_FACTUREEXP,
    P_ATTR_COLLECTIONGRADE,
    P_ATTR_FACTUREGRADE,

    P_ATTR_CURPKCP,
    P_ATTR_MAXPKCP,

    P_EARTH_ATTACK,
    P_EARTH_DEFEND,
    P_WATER_ATTACK,
    P_WATER_DEFEND,
    P_FIRE_ATTACK,	
    P_FIRE_DEFEND,	
    P_WIND_ATTACK,	
    P_WIND_DEFEND,	
    P_BRIGHT_ATTACK,
    P_BRIGHT_DEFEND,
    P_DARK_ATTACK,
    P_DARK_DEFEND,	

    P_EARTH_ATTACKXIANG,
    P_EARTH_DEFENDXIANG,
    P_WATER_ATTACKXIANG,
    P_WATER_DEFENDXIANG,
    P_FIRE_ATTACKXIANG,
    P_FIRE_DEFENDXIANG,
    P_WIND_ATTACKXIANG,
    P_WIND_DEFENDXIANG,
    P_BRIGHT_ATTACKXIANG,
    P_BRIGHT_DEFENDXIANG,
    P_DARK_ATTACKXIANG,
    P_DARK_DEFENDXIANG,


};



//monster��������:	�������������ö��ֵ
enum M_ATTRIBUTE_TYPE
{
    M_ATTR_TYPE = 0x3F+1,
    M_ATTR_ID,
    M_ATTR_GRAPHICSID,
    M_ATTR_REGIONID,
    M_ATTR_TILEX,
    M_ATTR_TILEY,
    M_ATTR_DIR,
    M_ATTR_POS,

    M_ATTR_STATE,
    M_ATTR_ACTION,


    /*��������*/
    // 0: uchar 1:ushort 2:ulong	3:FLOAT 4:long 5:SHORT 6:bool
    M_ATTR_INDEX,
    M_ATTR_PICLEVEL,
    M_ATTR_NAMECOLOR,
    M_ATTR_HPBARCOLOR,
    M_ATTR_SOUNDID,
    M_ATTR_LEVEL,
    M_ATTR_EXP,
    M_ATTR_OCCUPOINT,
    M_ATTR_COUNTRY,
    M_ATTR_TAMABLE,
    M_ATTR_MAXTAMECOUNT,
    M_ATTR_COUNTRYCONTRIVALUE,
    M_ATTR_SPCONTRIVALUE,
    M_ATTR_ELETYPE_,
    M_ATTR_NPCTYPE_,
    M_ATTR_CLASS,
    M_ATTR_RACE,
    M_ATTR_MOVERANDOMVALUE,
    M_ATTR_STOPTIME,
    M_ATTR_AIHANGUPTYPE,
    M_ATTR_FIGURE,
    M_ATTR_HP,
    M_ATTR_AITYPE,

    M_ATTR_HURTPROPORTION,
    M_ATTR_GUARDTIME,
    M_ATTR_ESCAPEPOINT,
    M_ATTR_SKILLPROPORTION,
    M_ATTR_SKILLSPACE	,

    M_ATTR_AIREFERRENCE0	,
    M_ATTR_AIREFERRENCE1,
    M_ATTR_AIREFERRENCE2,
    M_ATTR_AIREFERRENCE3,
    M_ATTR_AIREFERRENCE4,

    /*ս������*/
    M_ATTR_MAXHP		,
    M_ATTR_HPRECOVERSPEED	,		
    M_ATTR_MINATK,
    M_ATTR_MAXATK		,
    M_ATTR_MINMATK	,	
    M_ATTR_MAXMATK,
    M_ATTR_HIT,
    M_ATTR_DEF,
    M_ATTR_MDEF,
    M_ATTR_DODGE,
    M_ATTR_BLOCK,
    M_ATTR_PARRY,
    M_ATTR_IMMUNITY,
    M_ATTR_PIERCE,
    M_ATTR_ATCSPEED,
    M_ATTR_MOVESPEED,
    M_ATTR_FIGHTMOVESPEED,
    /*M_ATTR_FIGHTBLOCK*/	
    M_ATTR_FARATKDIS,
    M_ATTR_NEAATKDIS,
    M_ATTR_ATKRANGE,
    M_ATTR_GUARDRANGE,
    M_ATTR_CHASERANGE	,
    M_ATTR_WILLDEF,
    M_ATTR_CONSTIDEF		,	
    M_ATTR_CRI			,
    M_ATTR_CRIDEF,
    M_ATTR_MCRIDEF	,
    M_ATTR_CANTALK	,

    M_ATTR_DEBUFFDEF_BLED,
    M_ATTR_DEBUFFDEF_DIZZY,
    M_ATTR_DEBUFFDEF_LULL,
    M_ATTR_DEBUFFDEF_HYPNOSIS,
    M_ATTR_DEBUFFDEF_POSION,
    M_ATTR_DEBUFFDEF_FIX,
    M_ATTR_DEBUFFDEF_SILENCE,

    M_ATTR_ELEMDEF_FIRE,
    M_ATTR_ELEMDEF_WATER,
    M_ATTR_ELEMDEF_EARTH,
    M_ATTR_ELEMDEF_WIND,
    M_ATTR_ELEMDEF_DARK,
    M_ATTR_ELEMDEF_HOLY,

    M_ATTR_CLASSDAM_HUMAN,
    M_ATTR_CLASSDAM_ELEMENT,
    M_ATTR_CLASSDAM_ALIEN,
    M_ATTR_CLASSDAM_ANIMAL,
    M_ATTR_CLASSDAM_UNDEAD,
    M_ATTR_CLASSDAM_EVIL,
    M_ATTR_CLASSDAM_HEXAPODS,

    M_ATTR_ELEMDAM_FIRE,
    M_ATTR_ELEMDAM_WATER,
    M_ATTR_ELEMDAM_EARTH,
    M_ATTR_ELEMDAM_WIND,
    M_ATTR_ELEMDAM_DARK,
    M_ATTR_ELEMDAM_HOLY,

    /*����ս������*/
    M_ATTR_BASEMAXHP,
    M_ATTR_BASEHPRECOVERSPEED	,	
    M_ATTR_BASEMINATK		,
    M_ATTR_BASEMAXATK		,
    M_ATTR_BASEMINMATK		,
    M_ATTR_BASEMAXMATK		,
    M_ATTR_BASEHIT	,
    M_ATTR_BASEDEF,
    M_ATTR_BASEDODGE,
    M_ATTR_BASEBLOCK,
    M_ATTR_BASEPARRY,
    M_ATTR_BASEATCSPEED,
    M_ATTR_BASEMOVESPEED		,

    M_ATTR_BASEFARATKDIS,
    M_ATTR_BASENEAATKDIS,
    M_ATTR_BASEATKRANGE,
    M_ATTR_BASEGUARDRANGE,
    M_ATTR_BASECHASERANGE		,
    M_ATTR_BASEWILLDEF,
    M_ATTR_BASECONSTIDEF		,
    M_ATTR_BASECRI	,
    M_ATTR_BASECRIDEF		,
    M_ATTR_BASEMCRIDEF		,
    M_ATTR_BASEDEBUFFDEF_BLED,
    M_ATTR_BASEDEBUFFDEF_DIZZY,
    M_ATTR_BASEDEBUFFDEF_LULL,
    M_ATTR_BASEDEBUFFDEF_HYPNOSIS,
    M_ATTR_BASEDEBUFFDEF_POSION,
    M_ATTR_BASEDEBUFFDEF_FIX,
    M_ATTR_BASEELEMDEF_FIRE,
    M_ATTR_BASEELEMDEF_WATER,
    M_ATTR_BASEELEMDEF_EARTH,
    M_ATTR_BASEELEMDEF_WIND,
    M_ATTR_BASEELEMDEF_DARK,
    M_ATTR_BASEELEMDEF_HOLY,
    M_ATTR_BASECLASSDAM_HUMAN,
    M_ATTR_BASECLASSDAM_ELEMENT,
    M_ATTR_BASECLASSDAM_ALIEN,
    M_ATTR_BASECLASSDAM_ANIMAL,
    M_ATTR_BASECLASSDAM_UNDEAD,
    M_ATTR_BASECLASSDAM_EVIL,
    M_ATTR_BASECLASSDAM_HEXAPODS,
    M_ATTR_BASEELEMDAM_FIRE,
    M_ATTR_BASEELEMDAM_WATER,
    M_ATTR_BASEELEMDAM_EARTH,
    M_ATTR_BASEELEMDAM_WIND,
    M_ATTR_BASEELEMDAM_DARK,
    M_ATTR_BASEELEMDAM_HOLY,
};
//��������
enum B_ATTRIBUTE_TYPE
{
    B_ATTR_TYPE =  0x3F+1,
    B_ATTR_GRAPHICSID,
    B_ATTR_REGIONID,
    B_ATTR_TILEX,
    B_ATTR_TILEY,
    B_ATTR_DIR,
    B_ATTR_POS,
    B_ATTR_STATE,
    B_ATTR_ACTION,
    B_ATTR_INDEX,
    B_ATTR_HP,
    B_ATTR_NAMECOLOR,
    B_ATTR_LEVEL,
    B_ATTR_MAXHP,
    B_ATTR_DEF,
    B_ATTR_MDEF,
};

//��������������������:	�������������ö��ֵ
enum R_ATTRIBUTE_TYPE
{
    R_ATTR_RGNTYPE=0x7000+1,
    R_ATTR_MERISCALE,
    R_ATTR_COUNTRY,
    R_ATTR_ISPK,
    R_ATTR_ISCONTRIBUTE,
    R_ATTR_ISRIDE,
    R_ATTR_SPACETYPE,
    R_ATTR_WARTYPE,
};
enum eMAILTYPE
{
    SYSTEM_MAIL,	//ϵͳ�ʼ�
    COMMON_MAIL,	//��ͨ�ʼ�
    AFFIX_MAIL,		//��ͨ����
    PAYGOLD_MAIL,	//�����ʼ�
    SENDGOLD_MAIL,	//���ѻظ����ʼ�
    MAX_MAIL	= 100,	
    //PRESENT_MAIL,	//�����ʼ�
    //REQUEST_MAIL,	//��ȡ�ʼ�
};

//	//�ɼ�������
enum C_ATTRIBUTE_TYPE
{
    C_ATTRIBUTE_CANCOLLECTION,
};

#define INVILID_VALUE -99999999

enum eClass
{
    C_Human=0,		//����
    C_Element,		//Ԫ��
    C_Alien,		//����
    C_Animal,		//����
    C_Undead,		//����
    C_Evil,			//��ħ
    C_Hexapods,		//����

    C_Max,
};

//Ԫ������
typedef enum eElementalType
{
    ET_Fire=0,		//��ϵ
    ET_Water,		//ˮϵ
    ET_Earth,		//��ϵ
    ET_Wind,		//��ϵ
    ET_Dark,		//��ϵ
    ET_Holy,		//ʥϵ
    ET_Other,		//������

    ET_Max,
}eEleType;

//���壺ħ����������������
typedef enum eAttackDefend
{
    AD_Attack,		//���Թ���
    AD_Defend,		//���Է���
    AD_AttackXiang,	//���Թ���
    AD_DefendXiang,	//���Է���

    AD_Max,
    AD_Invalid = 0xFFFFFFFF,
}eAttackDefend;

//! ��������
enum eXiangXingIndex
{
    eXXV_3,
    eXXV_6,
    eXXV_9,
    eXXV_12,
    eXXV_15,

    eXXV_NUM,
};

//! ����ֵ
static long s_XiangXingValus[eXXV_NUM] = 
{
    3, 6, 9, 12, 15
};

//���壺ħ��Ԫ������
typedef enum eMagicElement
{
    ME_Earth,		//��ϵ
    ME_Water,		//ˮϵ
    ME_Fire,		//��ϵ
    ME_Wind,		//��ϵ
    ME_Bright,		//��ϵ
    ME_Dark,		//��ϵ

    MT_Max,
    MT_Invalid = 0xFFFFFFFF,
}eMagicElement;

//! ��Ʒħ�������ͣ�Ԫ������Ҳʹ�����ö�٣�
//! ʹ��10���Ƶ�ģʽ�����ڽű�����
//! ��Ʒ�׵�����=10λ�ϵ�����+��λ�ϵ����ͣ����У���λ�ϵ���������eMagicElement��ֵ��
enum eGoodsElementType
{
    eGET_Disabled	= 0,	//! û�п���
    eGET_Enabled	= 1,	//! ��ʼ����
    eGET_Attack		= 10L,	//! ��������
    eGET_Defend		= 20L,	//! ��������
};


enum eEquipLimitType
{
    ELT_UNKNOWN=0,
    ELT_LIGHT,
    ELT_MID,
    ELT_HEAVY,

    ELT_Max,
};

enum eNpcType
{
    NT_Normal=0,	//��ͨNPC
    NT_Monster,		//����
    NT_S_Monster,   //�������
    NT_Guard,		//����

    NT_Max,
};

//��ְҵϵͳְҵ�б�
enum  eDeputyOccu		
{
    DOccu_Smithing=1,		//����
    DOccu_Metallurgy,		//����
    DOccu_Sewing,			//����
    DOccu_Other,			//����(��Ҫ��������)	
};

// ���Ͷ���
enum OBJECT_TYPE
{
    TYPE_SESSION			= 10,	// �Ự
    TYPE_PLUG				= 11,	// �Ự�Ĳ�ͷ
    TYPE_WORLD				= 100,	// ����
    TYPE_REGION				= 200,	// ��ͼ����
    TYPE_AREA				= 300,	// ���򣨽�server�У�

    TYPE_PLAYER				= 400,	// ���

    TYPE_COLLECTION			= 500,	// �ɼ���
    TYPE_MONSTER			= 600,	// ����
    TYPE_PET				= 604,	// ����

    TYPE_GOODS				= 700,	// ��Ʒ

    TYPE_SKILL				= 800,	// ���ܣ���client�У�
    TYPE_EFFECT				= 900,	// ��Ч����client�У�

    TYPE_SUMMON				= 1000,	// �ٻ�������ǽ��

    TYPE_BUILD				= 1100, // �������Ľ�����
    TYPE_CITYGATE			= 1200, // ����
    TYPE_TALKBOXMODEL       = 1300, // TalkBoxģ��

    TYPE_COUNTRY			= 1400, // ����
    TYPE_FACTION			= 1500,	// �л�
    TYPE_TEAM				= 1600, // ����
};

enum EVENT_TYPE
{
    EVENT_OnEquipMount		= 0,	//����װ��
    EVENT_OnEquipUnMount,			//����װ��
};

enum QUEST_TYPE						//��������
{
    QUEST_MAIN=1,					//��������
    QUEST_LATERAL,					//֧������
    QUEST_ENGAGEARMS,				//Ӷ������
    QUEST_OTHER						//��������
};

//������Ϸ�����ʵ������ʾ
struct stDestObj
{
    long nDestType;
    CGUID DestID;
};

// ���峡������
enum eRgnType
{
    RGN_NORMAL = 0,
    RGN_PERSONAL,
    RGN_PERSONAL_HOUSE,
    RGN_TEAM,
    RGN_PROXY,
    RGN_VILLAGE,		// ��վ��ͼ
    RGN_CITY,			// ��
    RGN_INVALID = 0xFE, // �Ƿ�����
    RGN_ALL = 0xFF,		// ��ʾ���г�������
};
// ���״̬����
enum ePlayerState
{
    PLAYER_STATE_VOID,					// ��Ҵ�����Ч״̬(��ʼֵ)
    PLAYER_STATE_LOADED_BASE_DATA,		// �Ѷ�ȡ��������
    PLAYER_STATE_LOADED_CHAR_DATA,		// �Ѷ�ȡ��ɫ����
    PLAYER_STATE_ENTERING_GAME,			// ���ڽ�����Ϸ
    PLAYER_STATE_ENTERED_GAME,			// �ѽ�����Ϸ
    PLAYER_STATE_EXITED_GAME,			// ���˳���Ϸ
    PLAYER_STATE_CHANGING_SERVER,		// �л���������
    PLAYER_STATE_CREATING,				// ���ڴ�����Ҷ���
    PLAYER_STATE_BACKTOLOGIN,			// �˻ص�¼����
};

// �����Ҫ��������Բ���ö��ֵ
enum ePlayerSavePartFlag
{
    PLAYER_SAVE_BASEATTRS = 0,			// ��������:��Ǯ,�ȼ�...��
    PLAYER_SAVE_ORIGINALPACKGOODS,		// ��������Ʒ
    PLAYER_SAVE_SUBPACK1GOODS,			// �ӱ���1��Ʒ
    PLAYER_SAVE_SUBPACK2GOODS,			// �ӱ���2��Ʒ
    PLAYER_SAVE_SUBPACK3GOODS,			// �ӱ���3��Ʒ
    PLAYER_SAVE_SUBPACK4GOODS,			// �ӱ���4��Ʒ
    PLAYER_SAVE_SUBPACK5GOODS,			// �ӱ���5��Ʒ
    PLAYER_SAVE_VARLIST,				// �����б�
    PLAYER_SAVE_QUESTLIST,				// �����б�
    PLAYER_SAVE_SKILLLIST,				// �����б�
    PLAYER_SAVE_STATEIST,				// ״̬�б�
};

// ��������
enum eChatType
{
    CHAT_NORMAL = 0,		// ͬ������
    CHAT_REGION,			// ͬ������������
    CHAT_UNION,				// ��������
    CHAT_FACTION,			// �л�����
    CHAT_TEAM,				// ��������
    CHAT_PRIVATE,			// ����˽��
    CHAT_COUNTRY,			// ��������
    CHAT_COUNTRY_UNION,		// �˹�����
    CHAT_WORLD,				// ��������

    CHAT_SYSTEM,			// ϵͳƵ��

    CHAT_BOARDCAST,			// �㲥

    GM_CMD,					// GM����
    RUN_SCRIPT,				// ���нű�
    RUN_HELP,				// ����
    PLAYER_CMD,				// �������

    CHAT_NPC,				// NPC˵��

    TYPE_NUM,
};

//! ��Ʒ��Ϣ��������
enum eChatsGoodsInfoType
{
    eCGIT_Player,	//! ��Ʒ���������
    eCGIT_Npc,		//����Ʒ��Npc
    eCGIT_Copy,		//! ��Ʒ�Ǵ�������Ϣ�и���
};

//! �����и�������Ʒ��Ϣ
struct tagChatsGoodsInfo
{	
    long	color;
    long	posBegin;
    long	posEnd;

    long	lInfoType;	//! ��Ʒ��Ϣ�������� eChatsGoodsInfoType
    CGUID	OwnerGuid;	//! ӵ����GUID�����ӵ��������ΪeCGIT_Copy������Ҫ���ø�ֵ
    CGUID	KeyGuid;	//! C2Sʱ����ƷԴGUID����Ʒ��ϢGUID��S2Cʱֻ����Ʒ��ϢGUID
};

//! ��ҵĳƺ�����
enum ePlayerTitleType
{	
    ePTT_Hide,		//! �ƺ�����
    ePTT_Knight,	//! ��λ�ƺ�
    ePTT_Faction,	//! ���ƺ�
    ePTT_Medal1,	//! ѫ��1�ƺ�
    ePTT_Medal2,	//! ѫ��2�ƺ�
    ePTT_Medal3,	//! ѫ��3�ƺ�
    ePTT_LOVER,	    //! ��ż�ƺ� 
};

//! ���Ӿ��������
enum eAddExpType
{
    eAET_Monster,	//! ���ﾭ��
    eAET_FacBoon,	//! ���ḣ������
    eAET_NPC,		//! NPC����
    eAET_Script,	//! �ű�
};

enum RET_CREATE_RGN_FLAG
{
    C_RET_NOT_FIND_OWNER_ERROR	= -7,   // δ��WS���ҵ������
    C_RET_RGN_HAS_CREATED_ERROR	= -6,	// �ø����Ѵ���
    C_RET_TIMED_RGNS_FULL_ERROR	= -4,	// �����ʱ�ڴ������ั���Ѵ�����
    C_RET_ALL_RGNS_FULL_ERROR	= -3,	// �����������ռ�����
    C_RET_THESE_RGNS_FULL_ERROR	= -2,	// ���ั������
    C_RET_RGN_TEMPLAYTEID_ERROR	= -1,	// ��ͼID�����ڻ��Ǵ˴�Ҫ�����ĸ�������
    C_RET_LOCAL_SUCCESS			= 1,	// ���������ش��������ɹ�
    C_RET_REMOTE_SUCCESS		= 2,	// ������Զ�̴��������ɹ�
};
enum RET_TEAMRGN_REFRESH_FLAG
{
    RET_TEAMRGN_REMOTE_OK = 2,
    RET_TEAMRGN_LOCAL_OK  = 1,
    RET_TEAMRGN_LOCAL_ERROR = -1,
    RET_TEAMRGN_REMOTE_ERROR = -2,
};

//! ��ϢMSG_S2C_OTHER_TIME_INFO_BOX��ָ������������
enum eOtherTimeInfoType
{
    AT_ONCE_KICK,			//! ��������֪ͨ��Ĭ�ϣ�
    RGN_CLEARDUPRGNPLAYER,	//! ��������֪ͨ
    FCM_KICK_PLAYER,		//! ����������֪ͨ 
};

// ��Ϸ������ϵͳ�������Ե�����
enum eAwardType
{
    AWARD_GOODS = 1,	// ����
    AWARD_PROPERTY,		// ����
    AWARD_SCRIPT,		// �ű�
};

#define SERVER_KEY_DWORD_SIZE 4

const ulong ACCOUNT_SIZE = 32;
const ulong TITLE_SIZE = 32;

const ulong PLAYER_NAME_SIZE = 20;

//! ͨ����Ʒ��������������
const ulong MAX_ALLPURPOSE_CONTAINER_SIZE = 4;
//! ��Ӫ����
enum eCampType
{
    eCampUnion  = 0, // ����
    eCampEmpire = 1  // �۹�
};

//! ��������
enum eVariableType
{
    eVarNumber = 0, // ��ֵ��
    eVarString = 1, // �ַ�����
    eVarGuid   = 2  // GUID��
};

// ��Ӫ����
const int COUNTRY_NUM   = 6;

//! ����GMT����
//! ---------------------------------------------------------------------------------------------------
//GAPP����
enum eGappType
{
    eGT_INVALID = 0,
    eGT_SM	= 1,			//ServerMonitor
    eGT_AS,					//AccountServer
    eGT_LOGS,				//LogServer
    eGT_BS,					//BillServer
    eGT_LS,					//LoginServer
    eGT_DB,					//DBServer
    eGT_WS,					//WorldServer	
    eGT_GS,					//GameServer

};
//! GM������ʽ
enum eGmOptType
{
    eGOT_Ban,			//! ��ͣ
    eGOT_Unban,			//! ���

    eGOT_ShutUp,		//! ����
    eGOT_OpenUp,		//! �������
};
//! �ٱ�����
enum eReportType
{
    eRT_LawlessSoft,	//! ���
    eRT_LawlessInfo,	//! �����Ƿ���Ϣ
    eRT_UseBug,			//! ����bug
};

//! �����Ϊ
enum eGmtGmCommand
{
    eGGC_Move,			//! �ƶ�
    eGGC_Skill,			//! ʹ�ü���
    eGGC_TouchNpc,		//! �ⷢNPC
    eGGC_NomalChat,		//! ��ͨ����
    eGGC_GmChat,		//! GM����
};

// ��ǰ���Ա�־(ע���ö���ڵĶ���˳���벻Ҫ���ױ��,��Ҫ�����˳�����ĩβ)
enum CURRENT_LANGUAGE
{
    CL_CHINESE = 0,		//����
    CL_KOREA,			//������
    CL_JAPANESE,		//����

    CL_NUM,
};

const int GAPP_NAME_SZIE=32;
//! �����ٱ�������Ϣ
struct tagBaseResultInfo 
{
    //!				��������
    char			_IndicteeName[GAPP_NAME_SZIE];
    //!				����������
    char			_szGmName[GAPP_NAME_SZIE];
    //!				����ʱ��
    DWORD			_OptTime;
    //!				������
    DWORD			_OptResult;		//	��š�������
    //!				ԭ��
    DWORD			_dwCause;		//	ʹ����ҡ�����
    //!				-1�ǽ�⣬0�����÷�ͣ,��0�Ƿ�ͣʱ��
    long			_lBanTime;
};

//! �����ٱ�������Ϣ
struct tagReportResultInfo : public tagBaseResultInfo
{
    //!				����ԭ������������ӦС��MAX_AFFICHE_SIZE��
    string			_strWhys;
    //!				�������ݼ�¼������С��8000��
    string			_strOptLog;
};

//! �����ٱ���Ϣ
struct tagBaseReportInfo
{
    //!				��������
    char			_IndicteeName[GAPP_NAME_SZIE];
    //!				�ٱ�������
    char			_ReportName[GAPP_NAME_SZIE];
    //!				�ٱ�����(�� �ж���� ����Ϊ׼)
    DWORD			_ReportType;
    //!				�ٱ�ʱ��
    DWORD			_ReportTime;
};

//! �ٱ���Ϣ
struct tagReportInfo : public tagBaseReportInfo
{
    //!				�����������SMID
    DWORD			_WsSmId;
    //!				�����������GappID
    DWORD			_WsGappId;
};

//! ������󳤶�
const DWORD MAX_AFFICHE_SIZE = 256;


//! �����������
const DWORD PASSWORD_CHAT_COUNT			= 16;
const DWORD PASSWORD_SIZE				= PASSWORD_CHAT_COUNT + 1;
