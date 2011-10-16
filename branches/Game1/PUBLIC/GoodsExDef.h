
#include "GUID.h"

#pragma once

//!		��Ʒ��չ������������
enum	eGoodsExWindowType
{
	eGWT_DisassembleWindow,		//! �ֽⴰ��
	eGWT_PrescriptionWindow,	//! �ϳɴ��ڡ��󶨺Ͷ��ᴰ�ڣ�����Ϊħ�У�
	//eGWT_BindAndFrostWindow,	//! 
	eGWT_UpgradeWindow = 3,			//! ��������
	eGWT_PlusEvilWindow,		//! ħ������
	eGWT_ResetElementTypeWindow,		//! ����ħ�����ʹ���
	eGWT_ResetElementXiangXingWindow,	//! ����ħ�����Դ���
	eGWT_DepotWindow,			//! �ֿⴰ��
	/////XY 2008.1.21/////
	eGWT_CardFuntionWindow,		//!	�⿨������ҳ��	
	eGWT_PlayerTradeWindow,		//! ��ҽ��׹���ҳ��
	//////////////////////
	eGWT_End,
};

enum eEnchaseWindowType
{
	EBWT_OPEN_HOLE=1,
	EWT_CLOSE_HOLE,
	EWT_ENCHASE_CARD,
	EWT_REMOVE_CARD,
};

enum eIncrementConsumeType
{
	eICT_NORMAL_BUY_ITEM=1, //��ͨ����
	eICT_MAIL_BUY_ITEM,		//�ʼ�����
	eICT_USE_FUNCTION		//ʹ����ֵ����
};

enum eIncrementBuyRet
{
	eICR_BUY_SUCCEED=1,	//�ɹ�
	eICR_GOLD_NOT_ENOUGH, //��Ҳ���
	eICR_TICKET_NOT_ENOUGH,	//��ȯ����
	eICR_WEIMIAN_NOT_ENOUGH,//λ�治��
	eICR_PERSONAL_NUM_LIMIT,//�����޹�
	eICR_WORLD_NUM_LIMIT,   //�����޹�
	eICR_TIME_LIMIT		    //ʱ������
};
enum eIncrementFunction
{
	eIFUNC_RELIVE=90000		//ԭ�ظ���
};

enum eGoodsBaseType
{
	GT_NORMAL,//��ͨ
	GT_CONSUMABLE,//����
	GT_CARD,//��
	GT_GOODS,//��Ʒ
	GT_HEAD,//ͷ��
	GT_BODY,//�·�
	GT_GLOVE,//����
	GT_BOOT,//Ь��
	GT_WEAPON,//����
	GT_WEAPON2,//����
	GT_NECKLACE,//����
	GT_RING,//��ָ
	GT_BACK,//����
	GT_WING,//���
	GT_HEADGEAR,//ͷ��
	GT_FROCK,//����
	GT_FAIRY,//����
	GT_MEDAL,//ѫ��
	GT_SUBPACK, //�ӱ���
	GT_ARROW,	//��ʸ
	GT_MONEY,	//���
	GT_HORSE,	//����
	GT_SILVERMONEY,	//����
	GT_PICKGOODS,//ʰȡƷ
	GT_MANTLE,	//����
	GT_JEWELRY,	//��Ʒ
	GT_ACCESSORIES,	//����
	GT_GUARDIAN,  //�����ػ�ʯ
};

enum eWeaponType
{
	WT_UNKNOWN,
	WT_SINGLE_SWORD,//���ֽ�
	WT_SINGLE_HAMMER,//���ִ�
	WT_SINGLE_KNIFE,//ذ��
	WT_WAND,//����
	WT_HEAVY_SWORD,//˫�ֽ�
	WT_HEAVY_AX,//˫�ָ�
	WT_BOW,//��
	WT_DOUBLE_SWORD,//˫��
	WT_FIST//ȭ��
};

enum eWeaponHandType
{
	WHT_UNKNOWN,
	WHT_SINGLE_HAND=1,
	WHT_DOUBLE_HAND
};

//�ӱ�������
enum eSubPackType
{
	SPT_NORMAL=1,//��ͨ�ӱ���
	SPT_BUSINESS	//����ר�ñ���
};

enum eBusinessGoodsType 
{
	BGT_NOT_BUSINESS_GOODS,

	BGT_FOOD,		///< ʳ��
	BGT_ORE,		///< ��ʯ
	BGT_PLANT,		///< ֲ��
	BGT_FIBRE,		///< ��ά
	BGT_ALCOHOL,	///< ��
	BGT_WOOD,		///< ľ��
	BGT_LEATHER,	///< Ƥ��
	BGT_CLOTH,		///< ��
	BGT_WEAPON,		///< ���
	BGT_FLAVOR,		///< ����
	BGT_HANDIWORK,	///< ��Ʒ
	BGT_GEM,		///< ��ʯ

	BGT_MAX_TYPE,
};

enum eAccountLockRetVal
{
	ACCOUNT_LOCK_OK=1,
	ACCOUNT_LOCK_ERROR_CDKEY,
	ACCOUNT_LOCK_NOT_ENOUGH,
	ACCOUNT_LOCK_UNKNOWN_ERROR
};



//!		��Ʒ��չ���ܣ�S2C��Ϣ����
enum	eGoodsExTxtCode
{
	eGTC_Failed			= 0,		//! ʧ��
	eGTC_Succeed		= 1,		//! �ɹ�

	eGTC_SessionLapse	= -0xFFFF,	//! �Ի������Զ���Ѿ�ʧЧ
	eGTC_UnknownErr,				//! δ֪ԭ��Ĵ���

	eGTC_CantDisassemble,			//! ���ֽܷ�
	eGTC_CantFindAim,				//! �����ҵ�����Ŀ��
	eGTC_NoEnoughSpace,				//! �����ռ䲻��
	eGTC_SrcIsLock,					//! ԭ��������

	eGTC_NoPrescription,			//! �ϳ��䷽������
	eGTC_ErrSP,						//! ����ĺϳ��䷽

	eGTC_NoArmInHand,				//! ����û������ �����������ܳ��ܣ���
	eGTC_NoReinforceArm,			//! ���ǳ������� �����������ܳ��ܣ���
	eGTC_LevelError,				//! ���ȼ��������ܳ��ܣ���
	eGTC_ArmIsFull,					//! ����������ʱ���ܱ�ͬ������͵������滻����

	eGTC_ErrOperation,				//! ����Ĳ���ֵ
	eGTC_ErrAssistant,				//! �������ϴ���
	eGTC_CantOperateAim,			//! Ŀ�겻�����˲���
	eGTC_DelAssistantFailed,		//! ɾ����������ʧ��

	eGTC_ErrStuff,					//! �����ϴ���
	eGTC_LevelTiptop,				//! �ȼ��ѵ����
	eGTC_NotEnoughMoney,			//! Ǯ����

	eGTC_LimitTooLow,				//! ���Ƶȼ�̫��
	eGTC_QualityTooLow,				//! Ʒ��̫��

	eGTC_NumError,					//! ������������
	eGTC_NotUpgrade,                //! û��ǿ����װ�����ܽ��и�ħ����
	eGTC_UpgradeLow,                //! ǿ���ȼ�̫��
};

//! װ���������
enum eUpgradeResult
{
	eUR_Succeed,					//! �ɹ�
	eUR_Failed_And_LevelNotChange,	//! ʧ�ܣ��ȼ�δ�ı�
	eUR_Failed_And_Degraded,		//! ʧ�ܣ����ұ�����
};

//! ������ֵ
enum eBindType
{
	//! δ��
	eBT_Untie = 0,
	//! �Ѱ�
	eBT_Bind,
	//! ���ܰ�
	eBT_CantBind,
	//! �����ɰ�
	eBT_CantUntie,
	//! �õ����󶨣��Ҳ��ܽ��
	eBT_GetAndCantUntie,
	//! �õ����󶨣����Խ��
	eBT_GetAndBind,
	//! װ����󶨣��Ҳ��ܽ��
	eBT_DressAndCantUntie,
	//! װ����󶨣����Խ��
	eBT_DressAndBind,
};


//! ��������ֵ
enum eFrostType
{
	//! δ����
	eFT_UnFrost = 0,
	//! �Ѷ���
	eFT_Frost,
	//! �ⶳ��
	eFT_Thawing,
	//! ���ɽⶳ�����ö��ᣩ
	eFT_CantThaw,

	//! ���ɶ���
	eFT_CantFrost,
};


//! װ������ʧ�ܺ�Ĳ���
enum eFailedOperation
{
	//! �޲���
	eFO_Null,
	//! ��С�ȼ�
	eFO_Reduce,
	//! ���õȼ�Ϊ
	eFO_SetTo,
};


//! װ��������������Ϲ���
enum eExtraStuffOperate
{
	//! �޹���
	eESO_Null,
	//! ���ӳɹ���
	eESO_AddOdds,
	//! �滻ʧ�ܲ���Ϊ
	eESO_AmendFailOpt,
};

//! ħ�����Ϻϳɰ취
enum eEvilStuffOperation
{
	eESO_1in1 = 1,	//! 1��1
	eESO_2in1,	//! 2��1
	eESO_3in1,	//! 3��1
	eESO_4in1,	//! 4��1
};



//!				�ֽ��������������
#define			MAX_DISASSEMBLE_NUM		3U

//!				�����Ʒԭʼ���ֳ�
#define			MAX_GOOODS_NAME_SIZE	32U

//!				���ħ���ȼ�
#define			MAX_ELEMENT_LEVEL		2000U
//��			���ħ���ȼ�old
#define			MAX_EVIL_LEVEL			10U

//!				�ֽ�����������
#define			DISASSEMBLE_TYPE_NUM	8U
//!				�ֽ������������������
#define			DISASSEMBLE_CHILD_TYPE_NUM	10U

//!				�Ǹ�ְҵ�����䷽���Ƴ���
#define			MAKE_TABLE_NAME_SIZE	21U
//!				�Ǹ�ְҵ�����䷽˵������
#define			MAKE_TABLE_NARRATE_SIZE	2048U




//!			���ϳɲ�������
#define		MAX_STUFF_NUM			15U

//! C2S��Ϣ�� �ϳ�������ṹ
struct tagSynthesizeGoods
{
	CGUID	arrGoodsGuid[MAX_STUFF_NUM];		//! ��ƷGUID
	long	arrContainerFlag[MAX_STUFF_NUM];	//! ��Ʒ��������
	long	arrPos[MAX_STUFF_NUM];				//! ��Ʒ��������λ��
	long	arrNum[MAX_STUFF_NUM];
};




//!			����������������
#define		MAX_UPGRADE_STUFF_NUM			1U
//!			����������������
#define		MAX_UPGRADE_ASSISTANT_NUM		3U
//!			����˵��[1������Ŀ�� + MAX_UPGRADE_STUFF_NUM������ + MAX_UPGRADE_ASSISTANT_NUM������]
#define		IN_TAGUPGRADESTUFF_GOODS_NUM	(1 + MAX_UPGRADE_STUFF_NUM + MAX_UPGRADE_ASSISTANT_NUM)

//! C2S��Ϣ�� ��Ȳ��Ͻṹ

struct tagUpgradeStuff
{
	CGUID	arrGoodsGuid[IN_TAGUPGRADESTUFF_GOODS_NUM];		//! ��ƷGUID
	long	arrContainerFlag[IN_TAGUPGRADESTUFF_GOODS_NUM];	//! ��Ʒ��������
	long	arrPos[IN_TAGUPGRADESTUFF_GOODS_NUM];			//! ��Ʒ��������λ��
};


struct tagEvilStuff
{
	CGUID	GoodsGuid;		//! ��ƷGUID
	long	lContainerFlag;	//! ��Ʒ��������
	long	lPos;			//! ��Ʒ��������λ��
	long	lUseNum;		//! ��������
};

//��Ʒ������ȴʱ��
struct tagGoodsCommonCooldown
{
	DWORD dwCooldownTime;//������ȴʱ��
	DWORD dwCooldownStartTime;//��ʼʱ��
};

//������Ʒ
struct tagBusinessGoods
{
	string strOriginName;//ԭʼ��
	DWORD dwId;//��ƷID
	BOOL bSell;//�Ƿ����
	BOOL bSpecial;//�Ƿ��ز�
	BOOL bNeed;//�Ƿ�����
	DWORD dwBear;//������
	DWORD dwCategory;//���
	DWORD dwBasePrice;//������
	double dBuyPriceMin;//�������
	double dBuyPriceMax;//�������
	double dSellPriceMin;//�������
	double dSellPriceMax;//�������
};

//����ҵ��������ʱ����CLIENT�Ľṹ
struct tagBusinessGoodsToClient
{
	DWORD dwId;//��ƷID
	DWORD dwPrice;//�۸�
	DWORD dwBuyLimit;//�޹���
	BYTE btSpecial;//�Ƿ��ز�
	BYTE btNeed;//�Ƿ�����Ʒ
};

//������Ʒ���
struct tagBusinessGoodsCategory
{
	DWORD dwCategoryId;//���ID
	string strName;//����
	DWORD dwBuyLimit;//�޹���
};

struct tagTradeSpotData
{
	DWORD dwSpotId;
	DWORD dwGameServerId;
	DWORD dwAreaId;
	DWORD dwCountryId;
	map<DWORD,tagBusinessGoodsCategory> mapCategory;
	map<DWORD,tagBusinessGoods> mapGoods;
};


