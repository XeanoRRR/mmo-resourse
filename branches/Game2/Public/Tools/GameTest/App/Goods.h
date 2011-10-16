#pragma once

#include "shape.h"
#include "MoveShape.h"
#include "other\goodslist.h"

class CClientRegion;

class CGoods :	public CMoveShape
{
	friend class CGoodsList;
public:
	enum eAction
	{
		GOOD_DROP,
		GOOD_NORMAL,
		GOOD_EXCEPT
	};

	//��������ȫ������
	enum GOODS_ADDON_PROPERTIES
	{
		//δ֪����		
		GAP_UNKNOW				=0,
		//���ߵ���
		GAP_GOODS_CATEGORY		,//1
		//��ʾ��ɫ
		GAP_COLOR				,//2
		//���ﻻװ���
		GAP_COSPLAYID           ,//3
		//װ��������Ч1
		GAP_EQUIP_EFFECT1		,//4
		//װ��������Ч2
		GAP_EQUIP_EFFECT2       ,//5
		//װ��������Ч3						
		GAP_EQUIP_EFFECT3		,//6
		//�������
		GAP_WEAPON_CATEGORY		,//7
		//�ȼ�����
		GAP_ROLE_MINIMUM_LEVEL_LIMIT			,//8
		//��������
		GAP_SKILL_LIMIT			,//9
		//ְҵ����
		GAP_REQUIRE_OCCUPATION	,//10
		//�Ա�����
		GAP_REQUIRE_GENDER			,//11
		//���������趨
		GAP_PARTICULAR_ATTRIBUTE,//12
		//����ǿ��
		GAP_WEAPON_DAMAGE_LEVEL	,//13
		//��С������
		GAP_MIN_ATTACK			,//14
		//��󹥻���
		GAP_MAX_ATTACK			,//15
		//���������
		GAP_NOR_DEF				,//16
		//�˺����ձ���
		GAP_HURT_SUCK_PER		,//17
		//������������
		GAP_HIT_RATE_CORRECT	,//18
		//��������һ����
		GAP_NORATTACK_KILLRATE	,//19
		//������������
		GAP_FLEE_CORRECT		,//20
		//����������
		GAP_PARRY_CORRECT		,//21
		//�м���������
		GAP_BLOCK_CORRECT	    ,//22
		//ħ������
		GAP_FUMO_PROPERTY		,//23
		//ħ������һ����
		GAP_MAGIC_KILLRATE		,//24
		//רע����
		GAP_DEDICANT_CORRECT	,//25
		//ʩ���ٶ�
		GAP_MAGIC_SPEED			,//26
		//ħ������
		GAP_MAGIC_DEF			,//27
		//������
		GAP_FIREDEF_CORRECT		,//28
		//ˮ������				
		GAP_WATERDEF_CORRECT	,//29
		//�翹����
		GAP_WINDDEF_CORRECT		,//30
		//�ؿ�����
		GAP_EARTHDEF_CORRECT	,//31
		//��ʥ��������
		GAP_HOLYDEF_CORRECT		,//32
		//�ڰ���������
		GAP_DARKDEF_CORRECT		,//33
		//��־����
		GAP_WILL_DEF			,//34
		//�������߿���
		GAP_HYPNOSIS_DEF		,//35
		//�����ж�����
		GAP_POISON_DEF			,//36
		//����������
		GAP_FIX_DEF				,//37
		//�������ʿ���
		GAP_CON_DEF				,//38
		//������Ѫ����
		GAP_BLED_DEF			,//39
		//�������迹��
		GAP_DIZZY_DEF			,//40
		//������Կ���
		GAP_LULL_DEF			,//41
		//����׷��
		GAP_ADD_STR				,//42
		//����׷��				
		GAP_ADD_DEX				,//43
		//����׷��
		GAP_ADD_CON				,//44
		//����׷��
		GAP_ADD_INT				,//45
		//�ǻ�׷��
		GAP_ADD_WIS				,//46
		//����׷��
		GAP_ADD_SPI				,//47
		//������������
		GAP_MAXHP_CORRECT		,//48
		//HP�ָ��ٶ�����
		GAP_HP_RESUME_CORRECT	,//49
		//������������
		GAP_MAXMP_CORRECT		,//50
		//�����ָ��ٶ�����
		GAP_MP_RESUME_CORRECT	,//51
		//����HP
		GAP_ADD_HP				,//52
		//����HP�ٷֱ�
		GAP_ADD_PERCENT_HP		,//53
		//����MP
		GAP_ADD_MP				,//54
		//����MP�ٷֱ�
		GAP_ADD_PERCENT_MP		,//55
		//��������;�ֵ
		GAP_GOODS_MAXIMUM_DURABILITY	,//56
		//������������
		GAP_MAXOVERLAP			,//57
		//ѧϰ���ܵ�ID
		GAP_LEARN_SKILL			,//58
		//��������ID
		GAP_ADD_SKILL			,//59
		//����HP��
		GAP_LEECH_HP			,//60
		//����MP��
		GAP_LEECH_MP			,//61
		//����תMP��
		GAP_HURT2MP				,//62
		//����
		GAP_KNOCK				,//63
		//�س�
		GAP_HOME				,//64
		//�������
		GAP_RAN_TRANS			,//65
		//ִ�нű�
		GAP_SCRIPT				,//66
		//������ʯ����
		GAP_GEM_TYPE			,//67
		//������ʯ����
		GAP_GEM_LEVEL				,//68
		//������ʯ�ɹ���������
		GAP_GEM_PROBABILITY		,//69
		//������ʯ�ɹ����
		GAP_GEM_UPGRADE_SUCCEED_RESULT	,//70
		//������ʯʧ�ܽ��		
		GAP_GEM_UPGRADE_FAILED_RESULT		,//71
		//�������
		GAP_MOUNT_TYPE			,//72
		//���Ｖ��
		GAP_MOUNT_LEVEL			,//73
		//����������ҽ�ɫ����
		GAP_MOUNT_PLAYER_ROLE_LIMIT	,//74
		//���޴�ʹ��
		GAP_UNLIMITED_ACCESS		,//75
		//�쳣״̬
		GAP_EXCEPTION_STATE		,//76
		//����EXP����
		GAP_EXP_BONUS			,//77
		//ʹ����ID
		GAP_EXPBALL_USERID				,//78
		//���Ǿ���
		GAP_EXPBALL_EXP		,//79
		//�ֿ�Ԫ��ֵ
		GAP_YUANBAO_QUITS		,//80
		//��¼����ID
		GAP_RECORD_REGIONID		,//81
		//��¼����ID
		GAP_RECORD_COOR			,//82
		//װ�������
		GAP_EQUIP_MAXEXP		,//83
		//��װ����ID
		GAP_SUITID				,//84
		//��װ���
		GAP_SUIT_NUMBER			,//85
		//����������
		GAP_CREATOR_NAME		,//86
		//��ʧʱ��
		GAP_DEL_TIME			,//87
		//����
		GAP_LIFE				,//88
		//�õ����ߴ����ű�
		GAP_GET_TRIGGER			,//89
		//ʧȥ���ߴ����ű�
		GAP_LOST_TRIGGER		,//90
		//��������
		GAP_HOLE_NUM			,//91
		//��ǶĿ�����
		GAP_ENCHASE_TARGET		,//92
		//��������
		GAP_PACK_TYPE			,//93
		//��������
		GAP_PACK_CAPACITY		,//94
		//��С�������ɳ�
		GAP_MIN_ATK_UP		,//95		
		//��󹥻����ɳ�
		GAP_MAX_ATK_UP			,//97		
		//ħ�������ɳ�
		GAP_MAGIC_UP		,//100		
		//��������ɳ�		
		GAP_DEF_UP			,//102		
		//ħ��������߳ɳ�
		GAP_MAGICDEF_UP		,//104
		//���������ɳ�
		GAP_HIT_RATE_UP			,//105
		//����һ���ʳɳ�
		GAP_KILL_RATE_UP		,//106
		//
		GAP_MAGIC_KILL_RATE_UP,
		//�м����������ɳ�
		GAP_PARRY_UP			,//107
		//�����������ɳ�
		GAP_BLOCK_UP			,//108
		//��������
		GAP_FLEE_UP				,//109
		//��������;�ֵ�ɳ�ֵ
		GAP_MAXDUR_UP			,//110
		//�����ȼ�
		GAP_WEAPON_LEVEL		,//111
		//��С���������ٷֱ�
		GAP_MINATK_MODIFY_PER	,//112
		//��󹥻������ٷֱ�
		GAP_MAXATK_MODIFY_PER	,//113
		//��ͨ���������ٷֱ�
		GAP_DEF_MODIFY_PER		,//114
		//ħ�����������ٷֱ�
		GAP_MAGICATK_MODIFY_PER	,//115
		//ħ�����������ٷֱ�
		GAP_MAGICDEF_MODIFY_PER, //116
		//�������
		GAP_GUARD_EQUIP_CATEGORY,//117
		//װ��ʱ�����ű�
		GAP_EQUIP_TRIGGER, //118
		//жװʱ�����ű�
		GAP_UNEQUIP_TRIGGER, //119
		//״̬׷��
		GAP_ADD_STATE,//120
		//��ȴʱ��
		GAP_COOLDOWN,
		//˫������ͼ��ID
		GAP_BOTHHAND_GRAPHIC,
		//��Ƭ����
		GAP_CARD_CATEGORY,
		//��ƬͬһID��Ƕ��������
		GAP_CARD_ENCHASE_NUM_LIMIT,
		//�������˺�
		GAP_CRI_DAMAGE,
		//����������
		GAP_CRI_DEF,
		//ħ�������˺�
		GAP_MCRI_DAMAGE,
		//ħ����������
		GAP_MCRI_DEF,
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
		GAP_WRITEPROPERTY_ID, //�ýű�����Ʒ��m_strManufacturerNameд����ַ�����ID��������ʾ���Ϣ��
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

		GAP_AMOUNT,
	};

	//��������
	struct tagBaseProperty
	{
		string	strName;		//	�������� < 20
		DWORD	dwValue;		//	��Ҽ۸�
		DWORD	dwSilverPrice;	//	���Ҽ۸�
		DWORD	dwBuyPrice;		//  ��Ʒ����۸���ҵϵͳʹ�ã�
		DWORD	dwPriceType;	//  �۸�����
		DWORD	dwType;			//	��Ʒ����

		tagBaseProperty()
		{
			strName = "";
			dwValue = 0;
			dwSilverPrice = 0;
			dwBuyPrice = 0;
			dwPriceType = 0;
			dwType = 0;
		}
	};

	//��Ƭ����
	struct tagCardProperty
	{
		DWORD  dwCardIndex;			//  ��Ƭ������
		CGUID  CardGUID;			//	��Ƭ��GUID
		string strCardName;			//	��Ƭ������
		DWORD  dwCardType;			//	��Ƭ������
		DWORD  dwInlayEquipType;	//	��Ƭ��Ӧ��װ������
		BYTE   bInlayEquipLevel;	//	��Ƭ��Ӧ��װ������
		BYTE   bMaxNum;				//	�˿�Ƭ��ͬһװ������ʹ������
		bool   bIsSeriesCard;		//�˿��Ƿ�Ϊ�׿�

		vector<CGoodsList::tagEffect>  CardEffect;	//��Ƭ�ĸ�������
		tagCardProperty()
		{
			dwCardIndex = -1;
			CardGUID = CGUID::GUID_INVALID;
			strCardName = "";
			dwCardType = 0;
			dwInlayEquipType = 0;
			bInlayEquipLevel = 0;
			bMaxNum = 0;
			bIsSeriesCard = false;
		}
		tagCardProperty(const tagCardProperty& src)
		{
			dwCardIndex = src.dwCardIndex;
			CardGUID = src.CardGUID;
			strCardName = src.strCardName;
			dwCardType = src.dwCardType;
			dwInlayEquipType = src.dwInlayEquipType;
			bInlayEquipLevel = src.bInlayEquipLevel;
			bMaxNum = src.bMaxNum;
			bIsSeriesCard = src.bIsSeriesCard;
			CGoodsList::tagGoods2* pstGoods=CGoodsList::GetProperty(dwCardIndex);
			if (pstGoods)
			{
				vector<CGoodsList::tagEffect>::iterator EffectIter=pstGoods->vectorEffect.begin();
				for(;EffectIter!=pstGoods->vectorEffect.end();EffectIter++)
				{
					CardEffect.push_back(*EffectIter);
				}
			}
		}
	};

	CGoods(void);
	CGoods(DWORD dwIndexo);
	CGoods(const CGoods &srcGoods);
	virtual ~CGoods(void);

	DWORD GetIndex(){return m_dwIndex;}
	void  SetIndex(DWORD dwVal){m_dwIndex=dwVal;}

	void SetNum(long lVal){m_lNum=lVal;}
	long GetNum(){return m_lNum;}

	const char* GetOrginName(){return GetProperty()->strOrginName.c_str();}

	VOID SetGoodsBaseType(eGoodsBaseType type){m_GoodsBaseType=type;}
	DWORD GetGoodsBaseType();

	vector<CGoodsList::tagEffect>*	GetEffect()				{return &m_vectorEffect;}

	tagBaseProperty* GetBaseProperty()						{return &m_stBaseProperty;}
	void SetBaseProperty(tagBaseProperty *pstBaseProperty)	{m_stBaseProperty=*pstBaseProperty;}

	CGoodsList::tagBaseProperty* GetProperty()	{return &(CGoodsList::GetProperty(m_dwIndex)->BaseProperty);}

	bool AddToByteArray(vector<BYTE>*,bool bExtData=true);
	// ����
	bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData=true);
	bool DecordFromDataBlock(DBReadSet& readDB,bool bExData = true );

	////////////XY 2007.11.13///////////////
	void UpdateProperty(CMessage* pMsg);
	////////////////////////////////////////

	long GetEffectVal(WORD wType,int nWhich);
	void SetEffectVal(WORD wType,int nWhich,long lVal);
	void SetNewEffectVal(WORD wType,int nWhich,long lVal);		//����Ʒ����µĸ�������
	//long GetFixVal(WORD wType,int nWhich);
	bool CanRepair();
	bool CanUpgrade();

	bool HasEffect( WORD wType );
	bool IsEffectChange();
	//�ж���Ʒ�Ƿ���Ե���
	static bool isGoodsCanAccumulate(CGoods *pOnGroundGoods,CPlayer* pPlayer);
	DWORD RepairPrice();
	DWORD GetUpdatePrice();
	DWORD GetPrice();
	DWORD GetPriceType();
	DWORD GetSalePrice();

	//void SetNameColor(DWORD dwColor);
	//DWORD GetNameColor();
	float GetHeight();
	void  SetDropEffect();
	BOOL  GetDropEffect() {return m_bDropEffect;}


	//��Ʒ����Ƕ�׽ӿ�
	bool Stiletto();						//����Ʒ��׵Ľӿ�
	bool PlugUpHole(BYTE index,WORD wHoleNum);								//�����Ƕ��
	bool EnchaseCard(BYTE index,tagCardProperty card);			//��Ƕ��Ƭ
	bool RemoveCard(BYTE index);								//�����Ƭ
	void SetHoleNum(WORD wNum);									//������Ʒ�ĵ�ǰ�ѿ��Ŀ���
	WORD GetHoleNum()	{return m_wHoleNum;}					//��ȡ��Ʒ�ĵ�ǰ�ѿ��Ŀ���
	WORD GetCardNum()	{return (WORD)m_InlayHole.size();}		//������Ʒ������Ƕ�Ŀ�Ƭ��
	map<WORD,tagCardProperty>& GetInlayHole() {return m_InlayHole;}	//������Ʒ�ϵ���Ƕ����Ϣ
	bool GetHoleState(BYTE HoleIndex);							//��ȡ��Ʒָ��λ�õ���Ƕ�׵�״̬�������أ�
	void SetHoleState(BYTE HoleIndex,bool bState);				//������Ʒָ��λ�õ���Ƕ�׵�״̬�������أ�

	// ��������
	void Lock();
	void UnLock();
	bool IsLocked();
	////////XY 2008.1.3////////
	bool IsHaveShadow()	{return m_bHaveShadow;}
	void SetHaveShadowState(bool bState)	{m_bHaveShadow = bState;}

	//* add by MartySa
	/// ��ȡ�����ߵ�����
	const string& GetManufacturerName() { return m_strManufacturerName; }
	/// ���������ߵ�����
	void SetManufacturerName(const char* pStrName);
	///////////////////////////
private:
	DWORD	m_dwIndex;						// ���
	long	m_lNum;
	eGoodsBaseType m_GoodsBaseType;
	bool	m_bHaveShadow;					//����Ʒ�Ƿ�����Ӱ

	///////��ҵϵͳ���������ԣ������NPC��Ʒ���д����ԣ�///////
	DWORD	m_dwPurchaseLimit;				//�޹���
	///////////////////////////////////////////////////////////

	tagBaseProperty m_stBaseProperty;		//��Ʒ��������
	vector<CGoodsList::tagEffect>	m_vectorEffect;		// ��չ�����б�

	///////////��Ƕ������//////////
	map<WORD,tagCardProperty> m_InlayHole;	//��Ƕ�ף�ֻ����������ߵ���Ʒ���ܾ��У�
	WORD	m_wHoleNum;						//����Ʒ��ǰ�ѿ�����Ƕ����
	map<WORD,bool> m_mapHoleState;			//��Ʒ��ǰ�����׵Ŀ���״̬
	///////////////////////////////

	//CEffect*		m_pFlashEffect;			//������Ч
	//DWORD			m_dwNameColor;			//������ɫ

	//��Ʒ�����Ч��
	BOOL  m_bDropEffect;
	DWORD m_dwDropTimeStart;
	float m_fDropHeight;

	// ������־
	long	m_lLockCounter;

	// ����������(add by MartySa)
	string  m_strManufacturerName;
};
