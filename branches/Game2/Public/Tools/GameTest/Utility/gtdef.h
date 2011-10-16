


#ifndef _GT_Define_H_
#define _GT_Define_H_
#pragma once

#define HOTDOG_SUM	8				
#define MAX_MONEY 2000000000
#define MAX_SILVERMONEY 2000000000
#define COTE_NUM	5				//�ӱ����ĸ���
#define PACK_WIDE   5				//�ӱ���������
#define	PACK_NUM	6				//���б����ĸ���
#define HOST_GRIDITEM	20			//ԭʼ�������ܸ�����
#define DEPOT_MAIN_CELL_NUM	42		//���ֿ��������
#define PACK_KB108_CELL_NUM	20

#define GRID_H_ITEM 5							//����5�����߸���
#define GRID_V_ITEM 4							//����4�����߸���
#define GRIDITEM	GRID_H_ITEM*GRID_V_ITEM		//�������ĸ��ӵ�����
#define GRIDPACK	5							//�ӱ���������
#define GRID_V_SUBPACK	6						//�ӱ����������

#define GRID_H_INTEG 8							//���ϱ�������ĺ��Ÿ�����
#define GRID_V_INTEGMAXLINE	15					//���ϱ������������
#define GRIDINTEGMAXNUM  GRID_H_INTEG*GRID_V_INTEGMAXLINE	//���ϱ�������������

//typedef CGoodsList::eGoodsType eGoodsType;

const DWORD ROLE_PRAME_NUM=10;
const DWORD ROLE_NUM_MAX=30;

#define MAIL_PREPAGE 8			// �ʼ��б�һҳ�ĸ���

enum ePacketType
{
	ePT_Main,
	ePT_Sub,
	ePT_Business,
};
//! ��Ա��Ϣ
struct tagWSMemberInfo
{
	CGUID guid;
	long lState;
	long lOccupation;
	long lSex;				//�Ա�
	DWORD dwJoinTime;
	long lCompleteQuest;	//����ɵĶ��鹲�������
	char szName[20];
};

//! Ҫ���µķ������Ϣ˵��
enum eUpdateIdioinfo
{
	eMIS_Pos			,//����λ��

	eMIS_CurrHp			,//���µ�ǰHP
	eMIS_CurrMp			,//���µ�ǰMP
	eMIS_CurrEnergy		,//���µ�ǰ����

	eMIS_MaxHp			,//�������HP
	eMIS_MaxMp			,//�������HP
	eMIS_MaxEnergy		,//�����������

	eMIS_Level			,//���µȼ�
	eMIS_Occupation		,//����ְҵ
	eMIS_Sex			,//�Ա�

	eMIS_State			,//����״̬
};

//! �Ƕ����Ա��Ϣ��������Ϣ���ͣ�
struct tagMemberIdioinfo
{
	float fPosX;
	float fPosY;

	long lCurrHp;			//��ǰHP
	long lCurrMp;			//��ǰMP
	long lCurrEnergy;		//��ǰ����

	long lMaxHp;			//���HP
	long lMaxMp;			//���HP
	long lMaxEnergy;		//�������

	long lLevel;			//��ǰ�ȼ�
	long lOccupation;		//ְҵ
	long lSex;				//�Ա�
};

//�������
struct tagRoleParam 
{
	WORD m_wParam;
	WORD m_wExcessRecordIndex;
	LONG m_lAccParam;
	tagRoleParam()
	{
		m_wParam = 0;
		m_wExcessRecordIndex = 0;
		m_lAccParam = 0;
	}
};

//�����¼
struct tagRoleRecord 
{
	BYTE m_cRoleStep;	//������еĲ���
	BYTE m_cAimNum;
	long m_lRoleID;		//����ID;
	tagRoleParam m_Param[ROLE_PRAME_NUM];
};
//��չ�����¼
struct tagExcessRecord 
{
	DWORD m_lRegionID;
	DWORD m_lRegionX;
	DWORD m_lRegionY;

	tagExcessRecord():
	m_lRegionID(0),
		m_lRegionX(0),
		m_lRegionY(0)
	{}
};
//���＼��
struct tagPlayerSkill
{
	DWORD dwID;
	tagPlayerSkill(void)	{ dwID = 0; }
};
// ��Ʒ�ṹ
struct tagGoods
{
	class CGoods *pGoods;					//	��Ʒ�ṹ������ָ�����Ʒָ�����
	bool	bChange;				//	�Ƿ�ᷢ�����Ա仯
	WORD	lNum;					//	����
	DWORD	lPos;					//	�ڱ������е�����
	DWORD	lType;					//	��Ʒ���ڵ���������

	int	GetPosX()	{ return (int)lPos%5; }	//ȡ��X����
	int	GetPosY()	{ return (int)lPos/5; } //...y...

	tagGoods() :
	pGoods(NULL),
		bChange(false),
		lNum(0),
		lPos(0),
		lType(0)			
	{}

	~tagGoods()
	{
		::ZeroMemory(this, sizeof(tagGoods));
	}
};

//���������λ�ṹ
struct tagCote
{
	CGoods* pGoods;						//�����ṹ������Ʒָ�����
	list<tagGoods> m_listCoteGoods;
	set<DWORD> m_setCoteHaveGoods;

	tagCote() :
	pGoods(NULL)
	{
		m_listCoteGoods.clear();
		m_setCoteHaveGoods.clear();
	}
};
struct tagownedReg
{
	long lRegionID;						//����ID
	WORD wType;							//��������
};

struct tagPlayerShopGoods
{
	BYTE	btNum;
	BYTE	btOldPos;
	BYTE	btPos;

	long	lValue;

	union
	{
		DWORD dwType;
		CGoods* pGoods;
	};

	tagPlayerShopGoods()
	{
		::ZeroMemory(this, sizeof(tagPlayerShopGoods));
	}
};
// װ��������
enum eEquipIndex
{
	EQUIP_HEAD,				//	0��ͷ��
	EQUIP_NECKLACE,			//	1������
	EQUIP_WING,				//	2�����
	EQUIP_BODY,				//	3������
	EQUIP_WAISTBAND,		//	4������
	EQUIP_GLOVE,			//	5������	
	EQUIP_SHOES,			//	6��Ь��
	EQUIP_HEADGEAR,			//	7��ͷ��
	EQUIP_FROCK,			//	8������		
	EQUIP_LRING,			//	9�����ָ
	EQUIP_RRING,			//	10���ҽ�ָ		
	EQUIP_MEDAL1,			//	11��ѫ��1
	EQUIP_MEDAL2,			//	12	ѫ��2
	EQUIP_MEDAL3,			//	13	ѫ��3
	EQUIP_FAIRY,			//	14��С����
	EQUIP_WEAPON,			//	15������
	EQUIP_WEAPON2,			//	16����������
	EQUIP_PIFENG,			//	17������
	EQUIP_HORSE,			//	18������

	EQUIP_SUM,				//	װ��������
};

enum PLAYER_EXTEND_ID
{
	PEI_PACKET					= 1,	// ����
	PEI_EQUIPMENT,						// װ��
	PEI_WALLET,							// ���Ǯ��
	PEI_SILVERWALLET,					// ����Ǯ��
	PEI_GEM,							// ��ʯ
	PEI_YUANBAO,						// Ԫ��
	PEI_JIFEN,							// ����
	PEI_MEDAL,                          // ѫ������
	PEI_SUBPACK=10000,
	PEI_PACK1=10001,
	PEI_PACK2=10002,
	PEI_PACK3=10003,
	PEI_PACK4=10004,
	PEI_PACK5=10005,
	PEI_BUSSINESPACKFIELD = 20000,		// �����ʱ�������λID
	PEI_BUSSINESPACK = 20001			// ���ʱ�������ID
};

//װ������
enum EquipRestrict
{
	UNKNOW_NOTFIT,
	LEVEL_NOTFIT,		//�ȼ�����
	STR_NOTFIT,			//��������
	DEX_NOTFIT,			//���ݲ���
	CON_NOTFIT,			//���ʲ���
	INT_NOTFIT,			//��������
	OCCUPATION_NOTFIT,	//ְҵ����
	SEX_NOTFIT,			//�Ա𲻶�
	REGION_FORBID,		//������ֹ
	EQUIPFIT,			//����װ��
};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�ֿ� ���ݽṹ
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum eDEPOT_PACK_ID				//	*********************
{								//	*	*	*	*	*	*
	DEPOT_MAIN_PACK,			//	*********************		
	DEPOT_ITEM_PACK,			//	*	*	*	*	*	*
	DEPOT_EXTEND_PACK1,			//	*	*	*	*	*	*
	DEPOT_EXTEND_PACK2,			//	*	*	*	*	*	*
	DEPOT_EXTEND_PACK3,			//	*	*	*	*	*	*
	DEPOT_EXTEND_PACK4,			//	*	*	*	*	*	*
	DEPOT_PACK_NUM,				//	*	*	*	*	*	*
};								//	*********************
//�ֿⱳ�����ݽṹ
struct tagDepotPackData 
{
	//		list<tagGoods> listGoods;
	long	lContainerID;				//ID-----���ֿ�/Ĭ���ӱ���/�ӱ���/(���/������λ)�ȵ�
	long	lGoodsAmount;				//����
	long	lCubage;					
	hash_map<CGUID, tagGoods, hash_guid_compare> mapGoodsList;
	map<DWORD, BOOL> mapHaveGoods;

	tagDepotPackData() :	
	lContainerID(-1),
		lGoodsAmount(0),
		lCubage(0)
	{}
};
//�ֿ����ݽṹ
struct tagDepotData 
{
	long	lMoneyAmount;
	long	lSilverAmount;
	CGUID	guidMoney;
	CGUID	guidSilver;
	BOOL	bOpen;
	bool	bHasPassWord;
	char	strOldPwd[DEPOT_PASSWORD_SIZE+1];		//���������
	char	strNewPwd[DEPOT_PASSWORD_SIZE+1];		//����������
	tagDepotPackData m_ArrPackData[DEPOT_PACK_NUM];
	tagDepotData() :
	lMoneyAmount(0),
		lSilverAmount(0),
		guidMoney(CGUID::GUID_INVALID),
		guidSilver(CGUID::GUID_INVALID),
		bOpen(FALSE),
		bHasPassWord(false)
		//m_ArrPackData[DEPOT_PACK_NUM];
	{
		::ZeroMemory(strOldPwd, DEPOT_PASSWORD_SIZE+1);
		::ZeroMemory(strNewPwd, DEPOT_PASSWORD_SIZE+1);
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//С����
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! ���ݶ��󣨿ͻ��˿ɰ����޸ģ�
//struct tagSetupOnServer
//{
//	// ??? member data����??���ҵ�ʹ�õط�Ϊ�Զ���ҩ
//};
//��������
enum eAutoSupplyType{
	eAST_Hp1,
	eAST_Hp2,
	eAST_Mp,
	eAST_SpriteMp,
	eAST_ArmPower,
	AutoSupplyTypeNum,
};
//! �����Զ��������ö���
struct tagAutoSupplyData 
{
	BOOL bActive;			//״̬
	LONG lSpringScale;		//�����ٷֱ�
	LONG lGoodsIndex;		//������ƷID
	//tagAutoSupplyData(void) {memset(this,0,sizeof(tagAutoSupplyData));}
	tagAutoSupplyData() : bActive(FALSE),
		lSpringScale(0),
		lGoodsIndex(0)
	{}
};

struct tagAutoPixie{
	long  lItemGoodsNum;
	long* pItemGoodsList;
	tagAutoPixie() : lItemGoodsNum(0),
		pItemGoodsList(NULL) 
	{}
};

// �½��������ݽṹ [12/26/2008 Square]
struct tagOneTeammateInfo
{
	CGUID PlayerGuid;
	//		long lOccu;
	float fPosX;
	float fPosY;
	bool  bOnline;
	char szName[20];

	tagOneTeammateInfo()
	{
		::ZeroMemory(this, sizeof(tagOneTeammateInfo) );
	}
};
typedef  hash_map<CGUID,tagOneTeammateInfo,hash_guid_compare> HSM_OneTM_Info;

//---------------------------------------------------------------------------
//������Ա�����ݽṹ
struct tagTeammate
{
	CGUID	 PlayerGuid;						//��Աguid
	float	fPosX;
	float	fPosY;
	bool	bOnLine;					//�Ƿ�����
	tagTeammate(CGUID& id):PlayerGuid(id)/*, wHP(0), wMaxHP(0), occupation(0), RegionID(CGUID::GUID_INVALID), strName("")	*/					
	{}
	tagTeammate():PlayerGuid(CGUID::GUID_INVALID)/*,wHP(0), wMaxHP(0), occupation(0), RegionID(CGUID::GUID_INVALID), strName("")	*/
	{}
	bool operator == (const tagTeammate & t) { return t.PlayerGuid == PlayerGuid; }
};

struct tagPlayerInfo
{
};

	enum eUIPage
	{
		UI_PAGE_CHARACTER = 0,
		UI_PAGE_SKILL, 
		UI_PAGE_DEPOT, 
		UI_PAGE_MAIL, 
		UI_PAGE_FRIEND, 
		UI_PAGE_TEAM, 
		UI_PAGE_ITEM, 
		UI_PAGE_SUBBACK, 
		UI_PAGE_MAX_NUM, 
	};


	struct tagGtBounsInfo
	{
		tagBounsInfo*	pInfo;
		bool	bQuestGet;
		DWORD	dwLastTime;
		tagGtBounsInfo(void)
		{
			pInfo = NULL;
			dwLastTime = 0;
			bQuestGet = false;
		}
	};

	//���﹦��
	struct tagPetInfo
	{
		//	��������
		BYTE byType;
		//	guid
		CGUID guid;
		//	����
		char szName[17];
		tagPetInfo(void) : 
		byType(0),
			guid(CGUID::GUID_INVALID)
		{
			::ZeroMemory(szName, 17*sizeof(char));
		}
	};


	struct tagTradePageGoods
	{
		long lNum;		//����
		long lPos;		//λ��
		DWORD dwIconID;
		CGUID guid;
		tagTradePageGoods() :
		lNum(-1),
			lPos(-1),
			dwIconID(0),
			guid(CGUID::GUID_INVALID)
		{}
	};

	// ��ϵ�˷������ö��
	enum ENUM_PANEL
	{	
		PANEL_FRIENDS,			// ����
		PANEL_LOVER,			// ��ż
		PANEL_ENEMY,			// ����
		PANEL_STRANGER,			// İ����
		PANEL_BLACK,			// ������

		MAXNUM_PANELS
	};

	enum eLinkmanGroupType
	{
		//! ������
		eLGT_Friend,
		//! ��ż��
		eLGT_Spouse,
		//! İ������
		eLGT_Stranger,
		//! ������
		eLGT_Hellion,
		//! ������
		eLGT_Blacklist,

		//! ��С��Чֵ
		eLGT_Invalid,
	};

	//GroupPlane�µĳ�Ա��Ϣ
	struct tagLinkmanBase_data
	{
		CGUID		guid;								// GUID
		CHAR		szName[21];							// ����
		ULONG		uState;								// ״̬��eLinmanStateType��
		tagLinkmanBase_data()
			:guid(CGUID::GUID_INVALID),
			uState(0)
		{
			::ZeroMemory(szName, 21*sizeof(CHAR));
		}
	};

	//! A\B
	//! ���ڱ������ϵ�����ݽṹ
	struct tagLinkman_data : public tagLinkmanBase_data
	{
		CHAR		szMapName[267];						// ���ڵ�ͼ����
		LONG		lLevel;								// �ȼ�
		LONG		lOccupation;						// ְҵ

		tagLinkman_data(void)
		{
			memset(this, 0, sizeof(tagLinkman_data));
		}
	};

	//! A\C
	//! ������Ϣ����ϵ�����ݽṹ
	struct tagLinkmanBase : public tagLinkmanBase_data
	{
		BOOL		bMutual;							//! ���ӹ�ϵ��ʾ
		tagLinkmanBase() : bMutual(FALSE)
		{}
	};

	//! A\B\C
	//! ������Ϣ����ϵ�����ݽṹ
	struct tagLinkman : public tagLinkman_data
	{
		tagLinkman(void)
		{
			memset(this, 0, sizeof(tagLinkman));
		}
		BOOL		bMutual;							//! ���ӹ�ϵ��ʾ
	};
	//GroupPlane

	struct SPanel
	{	
		int					 groupid;								 //���ڷ����������id
		int					 number;								 //�Ѿ����ڵ�����
		list<tagLinkman>     m_PanleMemberData;					     //��������
		SPanel() : groupid(0), number(0)
		{}
	};
#endif//_GT_Define_H_