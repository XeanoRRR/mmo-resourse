#pragma once
#include "MoveShape.h"
#include "Goods/Goods.h"
#include "Horse.h"
#include "Talisman.h"
#include "Wing.h"
#include "HeadWear.h"
#include "Weapon.h"

//----------------------------------------
#include "../../GameClient/ClientApp/Goods/GoodsList.h"
#include "../../../Public/Common/PlayerSetupOnServer.h"
#include "../../../Public/Common/BaseDef.h"
#include "../../../Public/Common/PlayerDef.h"
#include "../../../Public/Common/TeamDef.h"
#include "../../../Public/Common/QuestRecord.h"
//-----------------08.12.11.nikai----------------
class CPet;
//-----------------------------------------------
#define HOTDOG_SUM	8				
#define MAX_MONEY	2000000000		
#define MAX_SILVER	2000000000		
#define COTE_NUM	5				//�ӱ����ĸ���
#define PACK_WIDE   5				//�ӱ���������
#define	PACK_NUM	6				//���б����ĸ���
#define EX_EQUIP_NUM    8           //װ���������֡����֡�������λ��Ϊ8��
#define HOST_GRIDITEM	20			//ԭʼ�������ܸ�����


//-----------------09.03.11.lpf----------------
#define FASHION_TYPE_GREATCOAT	0x01		//������ʾ״̬
#define FASHION_TYPE_MANTEAU	0x02		//������ʾ״̬
#define FASHION_TYPE_HELM		0x04		//ͷ����ʾ״̬(����ԭ�����,��Ŀǰû��ʹ��)
//---------------------------------------------


//-------------------07.9.6liuke------------------
//class CRoleQueue;
class CMonster;
class CQuest;
//------------------------------------------------
namespace CEGUI
{
    class Window;
    class EventArgs;
};
class CRegion;
class CEmotionShower;
class CPlayer : public CMoveShape
{
	PlayerSetupOnServer	m_PlayerSetupOnServer;

public:

	// ְҵ
	static string s_strOccupation[OCC_Max];
	static void InitPlayerOccuType();
	static const char* GetPlayerOccuType(long lOccuTypeIndex);

	// ����
	static string s_strConstellation[CONST_Pisces+1];
	static void InitPlayerConstType();
	static const char* GetPlayerConstType(long lConstTypeIndex);

	CPlayer(void);
	virtual ~CPlayer(void);

	// ��ʼ����������ֵ��Ӱ��
	void InitNameValueMap();

	//��ʼ����ʾģ��
	void SetDisplayModelGroup(bool bAsyn=true);
	virtual bool IsNeedDisplay();	// ��Player�Ƿ���Ҫ��ʾ
	void RefeashState()	{};
	bool Display(void);
	void RendShadow(bool bsimpleshadow);
	void DisplayText(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor=0, bool bIsDisplayHPBar = false);				// ��shape������ʾ����
	void DisplayHideInfo(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor=0);			// ��shape������ʾ������Ϣ�������
	void DisplayWarBitmap();								//��ʾս��״̬�����ͼ��

	WORD GetOccuLvlCoe(eOccupation byOccu,WORD wOccuLevel);	// ��ְҵ�ȼ�ϵ��

	// ���＼��
	struct tagSkill
	{
		DWORD	dwID;					//	���
		WORD	wLevel;					//	�ȼ�
		DWORD	dwRestoreTime;			//	��ȴʱ�� (����)
		DWORD	dwCoolDownStartTime;	//	��ʼ������ȴ��Чʱ�� timeGetTime (Ϊ0��ʾ����ʾCoolDown��Ч)
		DWORD	dwCoolDownGrayStartTime;//	ͼ����ʱ���ʱ�䣬�������3���ɫ����ô�Զ��л�����
		DWORD	dwIntonateStartTime;	//	����������ʼʱ��
		DWORD	dwIntonateTime;			//	���ܵ�����ʱ��
		float	fAffectRangeMin;		//	��С�ͷž���
		float   fAffectRangeMax;		//	����ͷž���
		WORD	wCostMP;				//	���ĵ�MP
		bool	bIsPublicCoolEffect;	//	�Ƿ��ڹ�����ȴʱ����
		DWORD	dwUseRestoreTime;		//	ʹ�õĹ�����,�ڿ�����ϲ���ʹ��Ч���ĳ���ʱ��
		DWORD	dwUseingStartTime;		//	��ʼʹ�øü��ܵ�ʱ��

		tagSkill()
		{
			dwID = 0;
			wLevel = 0;
			dwRestoreTime = 0;
			dwCoolDownStartTime = 0;
			dwCoolDownGrayStartTime = 0;
			dwIntonateStartTime = 0;	//	����������ʼʱ��
			dwIntonateTime = 0;			//	���ܵ�����ʱ��
			fAffectRangeMin = 0.0f;
			fAffectRangeMax = 0.0f;
			wCostMP = 0;
			bIsPublicCoolEffect = false;
			dwUseRestoreTime = 3200;
			dwUseingStartTime = 0;
		}
	};


	// ���Ͽ���������Ʒ
	struct  tagShortCutGoods
	{
		ulong	wID;					//	���
		ushort	wPublicCooldownType;	//	������ȴ����
		ulong	dwRestoreTime;			//	��Ʒ�ܵ���ȴʱ��(����)
		ulong	dwRemainTime;			//  ʣ�����ȴʱ�䣨���룩
		ulong	dwCoolDownStartTime;	//	��ʼ������ȴ��Чʱ�� timeGetTime (Ϊ0��ʾ����ʾCoolDown��Ч)
		ulong	dwCoolDownGrayStartTime;//	ͼ����ʱ���ʱ�䣬�������3���ɫ����ô�Զ��л�����
		bool	bCanBeUsed;				//  ��ǰ��Ʒ�Ƿ�ɱ�ʹ����

		tagShortCutGoods()
		{
			wID = 0;
			wPublicCooldownType = 0;
			dwRestoreTime = 0;
			dwRemainTime = 0;
			dwCoolDownStartTime = 0;
			dwCoolDownGrayStartTime = 0;
			bCanBeUsed = true;
		}
	};

	//������ӵ�е���װ
	struct  tagSuitAttribute_P
	{
		map<string,bool> Suit_Equit;			//��װװ���������Ƿ�װ����
		WORD	Suit_Num;						//��ӵ�еļ���
		WORD	Suit_SetupedNum;				//�Ѵ��ϵļ���
	};

public:
    /// �����λ������Ϣ
    struct  PhasePlace
    {
        /// ����
        float  x_point;
        float  y_point;
        /// ��Χ1��2�Ĵ�С
        float  a_size;
        float  b_size;
    };

    void   SetPhasePlace(const PhasePlace& place)   { m_PhasePlace = place ;} 
    const  PhasePlace&  GetPhasePlace() { return m_PhasePlace ; }

private:
    PhasePlace                     m_PhasePlace;

private:
	map<WORD,tagSuitAttribute_P>	m_SuitList;			//���������ӵ�е�������װ��Ϣ
	//��ʾ�Ƿ��������Լ�
	bool m_bSelf;

public:	
	bool IsSelf() {return m_bSelf;}
	void SetIsSelf(bool b) {m_bSelf=b;}
	////////////////��װ�ӿ�/////////////////////////
	map<WORD,tagSuitAttribute_P>*  GetPSuitList()		{return &m_SuitList;}								//��ȡ��������ӵ�е���װ�б�
	bool IsSuitBeHold(WORD SuitID);																			//�ж������Ƿ��д�ID����װ
	bool IsSuitBeSetuped(WORD SuitID);																		//�ж��Ƿ��д�ID��װ����������
	void AddSuit(WORD SuitID,string SuitOriginName);														//����IDԭʼ��ΪSuitOriginName����װ����m_SuitList��
	map<string,bool>&	GetSuitEquipList(WORD SuitID)		{return m_SuitList[SuitID].Suit_Equit;}			//���ش�ID����װ��ӵ�е�װ����ԭʼ�����Ƿ�װ���ϵ���Ϣ
	WORD GetNumOfPossessionSuit(WORD SuitID);																//��ȡ��ID����װ��ӵ�еļ���
	WORD GetNumOfSetupEquip(WORD SuitID);																	//��ȡ��ID����װ��װ���ϵļ���
	bool CompareSuitOriginName(WORD SuitID,string SuitOriginName);											//�Ƚ��ж���ӵ�е���װ���Ƿ��д�ԭʼ����װ��
	bool CompareSetEquipOriName(WORD SuitID,string SuitOriginName);											//����ԭʼ����װ���Ƿ��ѱ�װ����
	void SetupEquip(WORD SuitID,string SuitOriginName);														//����ԭʼ����װ����������
	void UnfixEquip(WORD SuitID,string SuitOriginName);														//����ԭʼ����װ������������
	bool IsSuitComplete(WORD SuitID);																		//�жϴ�ID����װ�Ƿ��Ѵ���
	bool RemoveEquipFromSuit(WORD SuitID,string SuitOriginName);											//�Ѵ�ID����װ��ԭʼ��ΪSuitOriginName��װ����tagSuitAttribute_P��ɾ��
	bool RemoveSuit(WORD SuitID);																			//�Ѵ�ID����װ��m_SuitList��ɾ��
	WORD FindEquipByOrName(WORD SuitID,string SuitOriginName);												//���������еĴ��������в�����װIDΪSuitID��ԭʼ��ΪSuitOriginName��װ����ͳ�ƴ�����Ʒ������
	WORD FindSetupEquipForOriName(WORD SuitID,string SuitOriginName);										//���װ�����д�IDԭʼ��ΪSuitOriginName����Ʒ����

	// ��Ʒ�ṹ
	struct tagGoods
	{
		bool	bChange;				//	�Ƿ�ᷢ�����Ա仯
		long	lNum;					//	����
		DWORD	lPos;					//	�ڱ������е�����
		DWORD	lType;					//	��Ʒ���ڵ���������

		int	GetPosX()	{return (int)lPos%5;}
		int	GetPosY()	{return (int)lPos/5;}
		CGoods *pGoods;
		tagGoods()
		{
			bChange = false;
			lNum = 0;
			lPos = 0;
			lType = 0;
			pGoods = NULL;
		}
		~tagGoods()
		{
			bChange = false;
			lNum = 0;
			lPos = 0;
			lType = 0;
			pGoods = NULL;
		}
		//union
		//{
		//	DWORD	dwType;				//	��Ʒ���ͱ��
		//	CGoods	*pGoods;			//	�ɸı����Ե���Ʒָ��
		//};
	};

	//�ӱ�����λ�ṹ
	struct tagCote
	{
		CGoods* pGoods;
		list<tagGoods> m_listCoteGoods;
		tagCote()
		{
			pGoods = NULL;
			m_listCoteGoods.clear();
		}
	};

	/*
	*	���ܣ��Զ�װ����Ʒ��װ����
	*	ϸ���������ڱ�������һ������Ʒ��ʱ����������Ʒ����װ����
	*				��ԭװ������װ�����������������װ����Ϣ��
	*	������	pGoods - ��Ʒ
	*			lSExtend - ��Դ������ֻ���ձ������ӱ����������е���Դ��
	*			lDExtend - Ŀ��������һ���Ǳ������ӱ�����Ч��
	*			dwDPos	 - Ŀ�ĵ�λ�ã��������ӱ����е�λ�ã�
	*	����ֵ�� true - ����װ���������������װ����Ϣ��false - ����װ��
	*	����: ����
	*	��������: 09.10.23
	*	�޸���־: -
	*/
	bool	AutoEquip(CGoods * pGoods,long lSExtend,long lDExtend,DWORD dwDPos);
	////////////////////////#�����ӵ���Ʒ����ĺ���/////////////////////////////////////
	bool	AddGoods(CGoods *pGoods,DWORD dwPos,LONG lExtendID);
	CGoods	*RemoveGoods(const CGUID& SourceGUID,const CGUID& DesGUID,DWORD dwPos,LONG lExtendID,DWORD dwAmount);
	CGoods	*GetGoodsByGUID(const CGUID& guID,int *pPos=NULL,long* lExtendID = NULL);
	WORD	GetGoodsContainerID(CGoods* pGoods);						//��ȡ�������ϵ���Ʒ���ڵ�����ID
	WORD	FindContainerIDOfGoods(CGoods* pGoods);						//������������(����ָ���������ӱ�����Χ��)�Ƿ���ĳ�����͵���Ʒ������з������ڵ�����ID
	CGoods* GetGoodsByPosition(long lExtendID,DWORD dwPos);				//ͨ��λ�ò�����Ʒ
	CGoods* FindFirstGoodsByIndex(DWORD dwIndex,int& lExtendID,int& pPos);						//���ҵ�һ�����ID���ڴ����ID��δ����������Ʒ
	// ���ݴ������ƷID��Ҫ���ӵ���������һ���ѵ������ᳬ���ѵ����޲���δ����������Ʒ
	CGoods* FindOverLapGoodsByIndex(DWORD dwIndex,DWORD dwGoodsNum,int& lExtendID,int& pPos);
	//������Ʒ���������������������Ƿ��������Ʒ�����������Ʒ���ڵ�λ�ã������İﶨλ��
	bool	FindAppointGoods(long lGoodType,long lGoodId, long lGoodNum, long lContainerID);									
	//�������Ų��������������еĴ�����Ʒ������
	long	GetGoodsAmount(long nIdx);
	//������Ʒ���Ʋ��������������еĴ�����Ʒ������
	long	GetGoodsAmount(const char* cGoodsName);

	bool	ClearGoods();
	bool	ChangeObjectAmount(LONG lContainerExtendID,LONG lObjectType,CGUID& guObjectID,DWORD dwAmount);

	//�����ӿ�
	//��������ID��ָ����λ�û�ȡ�����ϵ���Ʒ������ԭʼ�������ӱ�����
	tagGoods* GetGoodsOnPlayer(WORD ExtendID,WORD lPos);

	//��ȡָ����������Ʒ��������ԭʼ������
	list<tagGoods>* GetGoodsList(WORD index=0);

	//��ȡԭʼ������ָ��λ�õ���Ʒ
	tagGoods* GetGoods(WORD wPos);

	//��ȡָ����ŵ��ӱ�������Ʒ����	
	list<tagGoods>* GetSubGoodslist(WORD index);

	//��ȡָ����ŵ��ӱ�����ĳ��λ�õ���Ʒ
	tagGoods* GetSubGoods(WORD ExtendID,WORD lPos);	

	//��ȡָ����ŵ��ӱ���������Ϊ��Ʒ������
	CGoods* GetSubPGoods(WORD index);	

	//����ָ����ŵ��ӱ�����pGoods
	void SetSubPGoods(WORD index,CGoods* pGoods);

	//���ָ����ŵ��ӱ���
	void ClearCoteGoods(WORD index);

	//��ȡָ����ŵ��ӱ�����������
	WORD GetPackageXRowNum(WORD index)		  {return (WORD)PACK_WIDE;}		
	
	//��ȡָ����ŵ��ӱ������ݻ�
	WORD GetPackageDimen(WORD index);	
	
	//��ȡԭʼ�������ݻ�
	WORD GetHostPackgeDimen()				  {return (WORD)HOST_GRIDITEM;}		

	//��ȡָ����ŵı������ݻ����������������ӱ�����
	WORD GetPacketDimen(DWORD dwPacketIndex);

	//�Ƿ����еı�����������
	bool IsAllPackageFull();

	//��ȡ��һ��δ���ı��������(��ԭʼ������ʼ������-1��ʾû���ҵ�δ���ı���)
	int GetNotFullPackage();

	//��ȡָ����ŵı����ĵ�һ���ո��ӵ�λ��(dwExtendID>=0&&dwExtendID<=5)
	int GetFirstEmptyGrid(DWORD dwExtendID);	

	//��ȡ����ָ����ŵ��ӱ������ݽṹ
	tagCote* GetCotePackage(WORD index);	

	//�ж�ĳ�������Ƿ�����
	bool IsFullForPackage(long PackageIndex);	

	//��ȡ��ǰ�����Ŀո�������
	long GetEmptyGridNum();	

	//���˱����Ƿ�����Ʒ�������� 
	bool IsHaveLockedGoodsOnPackage(DWORD dwPacketIndex);												

	////////////���ʱ����ӿ�///////////
	
	///��ȡ���ʱ���ָ��λ�õ���Ʒ
	tagGoods* GetBusinessGoods(WORD wPos);
	///��ȡ���ʱ�����λ�ϵı���
	CGoods* GetBusinessPackGoods();
	///�������ʱ���
	void SetBussinesPack(CGoods*pPackGoods);
	///�������е����ʱ��������ʱ�����CGoods���󣬱����������Ʒ����û�б仯��
	bool ClearBussinesPackGoods();
	///��ȡ���ʱ����ĸ�����
	long GetBussinesPackDimen();
	///��ȡ���ʱ������������˴��ġ�������ָ�ܴ�ŵ�������Ʒ�ܵ����������������;�ֵ�йأ�
	long GetBussinesPackCapacity();
	///��ȡ���ʱ����ĵ�ǰ����
	long GetBussinesPackCurCapacity();
	///���ʱ��������Ƿ�����
	bool IsGrimFull();
	///���ʱ��������Ƿ�����
	bool IsCapacityFull();
	///��ȡ���ʱ����ĸ�������ֵ
	long GetEffectValueOfPackGoods(WORD wEffectType,int iWhich);
	///�Ƿ�װ���˿��õ����ʱ���
	bool IsHaveBusinessPack();

	///////////////////////////////////

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
	};


	// װ��������
	enum eEquipIndex
	{
        // װ����
        EQUIP_HEAD,				  // ͷ��1
        EQUIP_NECKLACE,			  // ����
        EQUIP_WING,				  // ���
        EQUIP_BODY,				  // ����
        EQUIP_BACK,				  // ����5
        EQUIP_HAND,                // �ֲ�
        EQUIP_SHOULDER,            // �粿
        EQUIP_WRIST,			      // ����	
        EQUIP_LEG,                 // �Ȳ�
        EQUIP_BOOT,			      // Ь��10	
        EQUIP_LRING,			      // ���ָ
        EQUIP_RRING,			      // �ҽ�ָ		
        EQUIP_WEAPON,			  // ����
        EQUIP_CARD,			      // ����
        EQUIP_GODGOODS,            // ʥ��15
        EQUIP_EFFIGY,              // ����
        EQUIP_LIFECOWRY,           // ��������
        EQUIP_SOULGUARD,           // ����
        EQUIP_RIDER,               // ����19   


        // ʱװ��    
        EQUIP_P_HEADGEAR,			// ͷ��
        EQUIP_P_FROCK,			    // ����	
        EQUIP_P_WRIST,             // ����
        EQUIP_P_LOIN,              // ��
        EQUIP_P_NECK,              // ��
        EQUIP_P_AMULET,            // ����
        EQUIP_P_FOOT,              // ��
        EQUIP_P_LEG,               // ��

        //�ƺ���
        EQUIP_MEDAL1,			//	�ƺ�1
        EQUIP_MEDAL2,			//	�ƺ�2
        EQUIP_MEDAL3,			//	�ƺ�3
        EQUIP_SUM,
	};

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


	// PVP�����жϱ�׼ֵ������09.9.14��
	enum ePVPType
	{
		PVP_PIRATE1		=	36000,				//	1�ຣ��ͼ��
		PVP_PIRATE2		=	72000,				//	2�ຣ��ͼ��
		PVP_PIRATE3		=	144000,				//	3�ຣ��ͼ��
	};
	///////////////////////////////////////////////////////////////////////////
	//��ҵȼ�ϵ��
	///////////////////////////////////////////////////////////////////////////
private:
	static long m_iEnemyPlayerLevelFirstQuotiety ;		//�������й���ҵȼ�ϵ��1
	static long m_iEnemyPlayerLevelSecondQuotiety ;	//�������й���ҵȼ�ϵ��1
public:
	static	long	GetEnemyPlayerLevelFirstQuotiety() {return m_iEnemyPlayerLevelFirstQuotiety;}
	static	long	GetEnemyPlayerLevelSecondQuotiety() {return m_iEnemyPlayerLevelSecondQuotiety;}
	static	void	SetEnemyPlayerLevelFirstQuotiety(long i){m_iEnemyPlayerLevelFirstQuotiety = i;}
	static	void	SetEnemyPlayerLevelSecondQuotiety(long i){m_iEnemyPlayerLevelSecondQuotiety = i;}
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
public:
	/// ��������Χָ����Χ���Ƿ��б������ڿ���
	bool	CheckIsAnyBodyShopOpenedOnRange(DWORD dwRangeSize);
	list<tagPlayerShopGoods>&	GetPlayerShopList() {return m_lstPlayerShop;}
	bool	IsPlayerShopOpened() {return m_bPlayerShopOpened; }
	void	SetPlayerShopOpen(bool bOpen) {m_bPlayerShopOpened = bOpen;}
	void	SetPlayerShopName(const char* pName) {m_strShopName = pName;}
	const char* GetPlayerShopName() {return m_strShopName.c_str();}
	const CGUID& GetPlayerShopSessionID(){return m_PlayerShopSessionID;}
	long	GetPlayerShopPlugID(){return m_lPlayerShopPlugID;}
	void	SetPlayerShopSessionID(const CGUID& SessionID){m_PlayerShopSessionID=SessionID;}
	void	SetPlayerShopPlugID(long lPlugID){m_lPlayerShopPlugID=lPlugID;}


	/*//////////////�ɵ��������Խӿ�/////////////////////////////////////////////////
	tagBaseProperty*	GetBaseProperty()		{return &m_BaseProperty;}
	const tagProperty*		GetProperty()		{return &m_Property;} 
	*////////////////////////////////////////////////////////////////////////////////

	list<tagSkill>& GetSkillList()				{return m_listSkillID;}

	eEquipLimitType QueryPlayerEquipLimitType(CPlayer* pPlayer); // �õ�ĳ��Player�ܹ����ķ�������
	bool	CanEquip(CGoods* pGoods);			// ������Ʒ��ǰ�Ƿ���װ��
	//bool	CanEquip(DWORD dwGoodsIndex);		
	CGoods* GetEquip(int index);
	void	SetEquip(int index,CGoods *pGood);
	//tagGoods* GetHand() { return &m_stHand;}
	//void SetHand(tagGoods* pstGood){m_stHand=*pstGood;}
	//void SetHand(bool bChanged,WORD lNum,DWORD lPos,CGoods *pGood,DWORD lType=0);
	//void ClearHand(){SetHand(true,0,0,NULL);}


	CGoods *pMoney;
	/////////////////////////////////////////////////////////////////////////////////////
	// ��������
	void UpdateWeaponShow(CGoods* pGoods);
	void UpdateWeaponShow(DWORD dwPos);
	void RemoveWeaponShow(CGoods* pGoods);
	void RemoveWeaponShow(DWORD dwPos);
	void UpdateRideShow();
	void UpdateAllWeaponShow();
	void UpdateProperty(CMessage* pMsg/*,char** &strName,DWORD* &dwValue,char** &str,WORD& wNum*/);
	void DecordChangeProperty(BYTE* pByte, long& pos);
	void DecodeChangePropertyFromDataBlock(DBReadSet& setReadDB);
	/////////////////////////////������������װ���Ľӿ�//////////////////////////////////////
	DWORD GetEquipIndex(long l)	{return m_dwEquipIndex[l];}
	void SetEquipIndex(long lIndex, DWORD dwIndex);
	DWORD GetEquipLevel(long l)	{return m_btEquipLevel[l];}
	void SetEquipLevel(long lIndex,DWORD level);
	DWORD TotalRepairPrice();
	void UseItem(DWORD dwIndex,int nExtendId = -1,int iPos = -1);				//nExtendId��������ID(-1��ʾ��������ϵ����������в��ҵ�һ����Ʒ���ΪdwIndex����Ʒ)��dwIndex��ʹ�õ���Ʒ��ID
	// �ⲿʹ�õ�һ����̬���ýӿ�
	static void CALLBACK CallUseItem(DWORD dwIndex,int nExtendId,int iPos,CPlayer*pPlayer=NULL);
	long CanUseItem(DWORD dwIndex);
	///////////////////////////////////////////////////////////////////////////
	void SetPlayerRemainDays(char remainDay){m_cRemainDays = remainDay;}
	char GetPlayerRemainDays(){return m_cRemainDays;}
    ///////////////////////////////////////////////////////////////////////////

	//////////////////��ҵϵͳ�ӿ�////////////////
	//void InvestOnPoint(DWORD dwPoint,DWORD dwValue);			//���Ͷ��ĳ��ó�׵�
	//���������ĳ�����������ֵ
	void SetCreditForZone(DWORD dwZoneID,DWORD dwCreditValue) {m_mapPlayerCreditForZone[dwZoneID] = dwCreditValue;}
	//��������ĳ�����������ֵ
	DWORD GetCreditForZone(DWORD dwZoneID);
	//�������ܽ�����ϵ����������
	long GetCreditNum(){return m_lCreditNum;};
	//������������
	void SetCreditNum(long l){m_lCreditNum=l;};
	//////////////////XY 2008.2.2/////////////////

	//���õ�ǰ���õĹ���ID�����ݸ�ְҵ���ͺ͸�ְҵ��Ϊ����dwBeavioralID��
	void SetToolsID(DWORD dwBeavioralID,DWORD dwActionID = 1);
	//��ȡ���ù��ߵ�ID
	DWORD GetToolsID() {return m_dwToolIndex;}

	// ----------����ָ����ҵ�����װ����Ϣ08.8.28-------------
	void CopyAllEquip(CShape *pShape);

private:
	static		BYTE  BuyBackGoodsNum;
	string		m_strPlayerOfficialRank;	//���ҹٽ�
	D3DCOLOR	m_OfficialRankTextColor;	//���ҹٽ��ֵ���ɫ
	D3DCOLOR	m_OfficialRankBkTextColor;	//���ҹٽ��ֱ�����ɫ

	list<tagPlayerShopGoods>	m_lstPlayerShop;
	bool						m_bPlayerShopOpened;				//����Ƿ��ڰ�̯״̬
	string						m_strShopName;
	CGUID						m_PlayerShopSessionID;				
	long						m_lPlayerShopPlugID;

	bool						m_bIsShowLoverName;					//�Ƿ���ʾ�������� �����ѽ����У�
	//////////��ҵϵͳ�����˵��������/////////
	string						m_strTraderName;				//���˳ƺ�
	//bool						m_bTradeModeSign;				//��ҵģʽ��־����������ģʽ�Ŀ��أ�
	map<DWORD,DWORD>			m_mapPlayerCreditForZone;		//����ڸ���ó��������������Ӧÿ��ó������IDֵ��
	map<DWORD,DWORD>			m_mapPlayerInvestmentForPoint;	//����ڸ���ó�׵��Ͷ�ʶ��Ӧÿ��ó�׵��IDֵ��
	long                        m_lCreditNum;                   //�����������
	//////////////XY 2008.1.16/////////////////

	///////////�ɼ�����������//////////////////
	DWORD						m_dwCollectExp;					//�ɼ�����
	DWORD						m_dwCollectLevel;				//�ɼ��ȼ�
	DWORD						m_dwFactureExp;					//��������
	DWORD						m_dwFactureLevel;				//�����ȼ�
	vector<DWORD>				m_vecFactureEntrys;				//ѧ���������Ŀ�б�
	DWORD						m_dwToolIndex;					//��ǰʹ�õĹ������ID
	//////////////XY 2008.1.24////////////////

	////////////��ǰҪ�õ��������Խṹ////////////////////////////////////////
	tagPPro						m_Property;
	tagPFPro					m_FightProperty;
	tagPAPro					m_AppendProperty;
	tagFriendshipCounty			m_FriendshipCounty;
	tagPlayerElementProperty	m_arrElementProperty[MT_Max];	//���壺ħ��Ԫ������

	///////////////////////////////////////////////////////////////////////////

	////////////�ɵ��������Խṹ///////////////////////////////////////////////
	//tagBaseProperty m_BaseProperty;		//	��������
	//tagProperty		m_Property;			//	ʵ������
	///////////////////////////////////////////////////////////////////////////

	CGUID			m_guMoneyID;
	CGUID			m_guSilverMoneyID;
	CGUID			m_guBaoShiID;
	CGUID			m_guYuanBaoID;
	CGUID			m_guJiFenID;

	//	װ����Ʒ
	//	0��ͷ		//	1������		//	2������
	//	3������		//	4��Ь��		//	5������1
	//	6������2	//	7������3	//	8������
	CGoods*	m_Equip[EQUIP_SUM];

    CGoods* m_eEquipGod[EX_EQUIP_NUM];        //װ������������λ
    CGoods* m_eEquipLand[EX_EQUIP_NUM];       //װ������������λ
    CGoods* m_eEquipHuman[EX_EQUIP_NUM];      //װ������������λ
	//tagGoods m_stHand;	//��
	//tagGoods m_stPreHand;  //���ڴ��ĳ����Ʒ�ڱ���������ǰ���ڵ���������λ����Ϣ
	bool m_bTradeDisAble;
	bool m_bFriendDisAble;

	//���ﱳ��
	tagCote*			m_Cote[COTE_NUM];	  //	���������
	list<tagGoods>		m_listGoods;		  //	��������ԭʼ������Ʒ������
    

	tagCote*		m_CoteBussinesPack;	  //	������������ʱ����е���Ʒ

	deque<CGoods*>	m_queBuyBackGoodsItem;	  //    �洢���Թ��ص���Ʒ

	list<tagShortCutGoods>	m_listShortCutGoods;	//��ȴ��Ʒ�б�

	typedef DWORD PCooldownRTime;			
	map<WORD,PCooldownRTime> m_mapPublicCooldown;	// ��Ʒ������ȴ��

	// ���＼��
	list<tagSkill>		m_listSkillID;		//	�����б�
    map<CGUID, tagBrotherInfo>    m_mapBrotherInfo;      //�������

	// -----------�����ְҵ�б�ְҵ����liuke08.8.29-------------
	vector<DWORD>			m_listOccuped;
	bool		IsAddOccupedList(DWORD dwSkillID);		// ָ���ļ����Ƿ��ܴ�ʹְҵ���ı仯

	bool m_bCanSetHotKey;					// �ܷ����ÿ����

	bool m_bGMConcealment;			        // GM�Ƿ�����

    std::map<long,long>   m_GeniusTable;    // �츳���ñ�,ID-level

	//bool	m_bIsHideInfo;			// �Ƿ����ؽ�ɫ��Ϣ
	//string	m_strHideInfoName;		// ������Ϣ֮��Ҫ��ʾ������

    /// �츳���@2010/10/21
public:
    void DecodeGeniusFromDataBlock(DBReadSet& setReadDB);	// �������е��츳���ݽ���
	std::map<long,long>* GetGenius(){ return &m_GeniusTable ;}
public:
	/////////���Խӿ�////////
	void RandomMoveGoods();
	/////////////////////////
	bool AddBuyBackGoods(CGoods* pBuyBackGoods);								// ����Ҫ���ص���Ʒ
	bool DelBuyBackGoods(int index = -1);										// ɾ��һ���ع������е���Ʒ(Ĭ�ϰ����е��Ƚ��ȳ�ɾ������ͷ������) 
	deque<CGoods*>& GetQueueBuyBackGoods()	{ return m_queBuyBackGoodsItem; }	// ��ȡ������Ʒ����	
	bool		IsInOccupedList(DWORD dwOccupID);								// �鿴ָ����ְҵ�Ƿ��������ҵ�ְҵ��֮��
	vector<DWORD> GetOccupedList()	{return m_listOccuped;}						// ��������ְҵ��
	vector<DWORD>	GetPlayerFactureList() {return m_vecFactureEntrys;}
	bool AddFactureSkill(DWORD dwFactureID);
	void DeleteFactureSkill(DWORD dwFactureID);
	void DeleteAllFactureSkill();

	DWORD	m_dwEquipIndex[EQUIP_SUM];		// װ����ţ��������ʹ�ã�
	DWORD	m_btEquipLevel[EQUIP_SUM];		// װ���ȼ����������ʹ�ã�

	char    m_cRemainDays;				//  ��ɾ���ʺŵ�ʣ������


	bool GetIsGMConcealment()	{return m_bGMConcealment;}
	void SetGMConcealment(bool b) {m_bGMConcealment=b;}


	// ��ɫ��Ϣ�Ƿ�����
	//bool GetIsHideInfo()		{return m_bIsHideInfo;}
	//void SetIsHideInfo(bool b)	{m_bIsHideInfo=b;}
	// ��ɫ��Ϣ���غ����ʾ��
	//const char * GetHideInfoName()				{return m_strHideInfoName.c_str();}
	//void SetIsHideInfoName(string strName)		{m_strHideInfoName = strName;}
	/////////////////////////////////////////////////////////////////////
	// ���Դ�ȡ�ӿ�
	/////////////////////////////////////////////////////////////////////
public:

	/////////�ѸĹ����������Խӿ�//////////////////////////////////////////////////////////////////////
	DWORD   GetCollectSkillLevel()			  {return m_dwCollectLevel;}			//�ɼ��ȼ�
	void	SetCollectSkillLevel(DWORD dwLevel) {m_dwCollectLevel = dwLevel;}

	DWORD	GetCollectSkillExp()			{return m_dwCollectExp;}			//�ɼ�����
	void	SetCollectSkillExp(DWORD dwExp)	{m_dwCollectExp = dwExp;}

	DWORD	GetFactureSkillExp()			{return m_dwFactureExp;}			//��������
	void	SetFactureSkillExp(DWORD dwExp) {m_dwFactureExp = dwExp;}			

	DWORD	GetFactureSkillLevel()			 {return m_dwFactureLevel;}			//�����ȼ�
	void	SetFactureSkillLevel(DWORD dwLevel){m_dwFactureLevel = dwLevel;}

	//DWORD	GetOccu()					{return m_Property.byAssOccu;}		
	//void	SetOccu(DWORD dwOccu)		{m_Property.byAssOccu = (eDeputyOccu)dwOccu;}	

	DWORD	GetAssOccu()				{return (DWORD)m_Property.byAssOccu;}			//��ȡ��ְҵ
	void	SetAssOccu(DWORD dwOccu)	{m_Property.byAssOccu = (eDeputyOccu)dwOccu;}

	DWORD	GetHp()						{return	m_Property.dwHp;}			//��ǰ����ֵ
	void	SetHp(DWORD l)				{m_Property.dwHp=l;}

	WORD	GetMp()						{return	m_Property.wMp;}			//��ǰħ��ֵ
	void	SetMp(WORD l)				{m_Property.wMp=l;}

	WORD	GetStamina()				{return	m_Property.wStamina;}		//��ǰ����
	void	SetStamina(WORD l)			{m_Property.wStamina=l;}

	WORD	GetEnergy()					{return	m_Property.wEnergy;}		//��ǰ����ֵ 
	void	SetEnergy(WORD l)			{m_Property.wEnergy=l;}

	//BYTE	GetRankOfNobility()			{return m_Property.byRankOfNobility;}		//��λ�ȼ�
	//void	SetRankOfNobility(BYTE l)	{m_Property.byRankOfNobility=l;}

	//DWORD	GetRanOfNobCredit()			{return m_Property.dwRankOfNobCredit;}	//��λ������ֵ
	//void	SetRanOfNobCredit(DWORD l)	{m_Property.dwRankOfNobCredit=l;}

	//BYTE	GetRankOfMercenary()		{return m_Property.byRankOfMercenary;}	//Ӷ���ȼ�
	//void	SetRankOfMercenary(BYTE l)	{m_Property.byRankOfMercenary=l;}

	//DWORD	GetMercenaryCredit()		{return m_Property.dwMercenaryCredit;}	//Ӷ��������ֵ
	//void	SetMercenaryCredit(DWORD l)	{m_Property.dwMercenaryCredit=l;}

	BYTE	GetCountry()				{return	m_Property.byCountry;}			//����ID
	void	SetCountry(BYTE l)			{m_Property.byCountry=l;}

	long	GetCountryContribute()		{return m_Property.lCountryContribute;}	//�Թ��ҵĹ���ֵ
	void	SetCountryContribute(long l){m_Property.lCountryContribute=l;}

	DWORD	GetUpgradeExp()				{return	m_Property.dwUpgradeExp;}			//��һ���������辭��
	void	SetUpgradeExp(DWORD l)		{m_Property.dwUpgradeExp=l;}				

	DWORD	GetOccuUpgradeExp()			{return	m_Property.dwCurOccuUpgradeExp;}	//��һ��ְҵ�ȼ��������辭��
	void	SetOccuUpgradeExp(DWORD l)	{m_Property.dwCurOccuUpgradeExp=l;}	

	BYTE	GetBusinessLevel()			{return m_Property.byBusinessLevel;}		//��ҵ�ȼ�
	void	SetBusinessLevel(BYTE l)	{m_Property.byBusinessLevel=l;}

	DWORD	GetBusinessExp()			{return m_Property.dwBusinessExp;}		//��ҵ����
	void	SetBusinessExp(DWORD l)		{m_Property.dwBusinessExp = l;}

	//BYTE	GetArtisanLevel()			{return	m_Property.byArtisanLevel;}		//�����ȼ�
	//void	SetArtisanLevel(BYTE l)		{m_Property.byArtisanLevel=l;}

	//DWORD	GetArtisanExp()				{return m_Property.dwArtisanExp;}			//��������
	//void    SetArtisanExp(DWORD l)		{m_Property.dwArtisanExp=l;}

	//DWORD	GetArtisanCredit()			{return m_Property.dwArtisanCredit;}			//��������
	//void    SetArtisanCredit(DWORD l)	{m_Property.dwArtisanCredit=l;}

	WORD	GetLevel()					{return m_Property.wLevel;}				//��ҵ�ǰ�ȼ�
	void	SetLevel(WORD l)			{m_Property.wLevel=l;}	

	DWORD	GetExp()					{return m_Property.dwExp;}				//����ֵ
	void	SetExp(DWORD l)				{m_Property.dwExp=l;}

	DWORD	GetPresentExp()				{return m_Property.dwPresentExp;}				//����ֵ
	void	SetPresentExp(DWORD l)		{m_Property.dwPresentExp=l;}

	BYTE	GetHairPic()				{return m_Property.byHairPic;}			//�õ�ͷ����Ϣ
	void	SetHairPic(BYTE by)			{m_Property.byHairPic=by;}				//�趨ͷ����Ϣ

	BYTE	GetHairStyle()				{return (m_Property.byHairPic & 0xF0) >> 4;}					//�õ����ͱ��
	void	SetHairStyle(BYTE by)		{m_Property.byHairPic = (m_Property.byHairPic & 0x0F) | by;}	//�趨���ͱ��

	BYTE	GetHairColor()				{return (m_Property.byHairPic & 0x0F);}									//�õ���ɫ���
	void	SetHairColor(BYTE by)		{m_Property.byHairPic = (m_Property.byHairPic & 0xF0) | (by << 4);}		//�趨��ɫ���

	BYTE	GetFacePic()				{return m_Property.byFacePic;}			//�沿ͼƬID
	void	SetFacePic(BYTE l)			{m_Property.byFacePic=l;}

	eOccupation	GetOccupation()			{return m_Property.byOccu;}				//ְҵ
	void	SetOccupation(BYTE l)		{m_Property.byOccu = (eOccupation)l;}

	//eConst	GetConst()					{return m_Property.byConst;}			//����
	//void	SetConst(BYTE l)			{m_Property.byConst=(eConst)l;}

	BYTE	GetSex()					{return m_Property.bySex;}				//�Ա�(0:�� 1:Ů) 
	void	SetSex(BYTE l)				{m_Property.bySex=l;}

	DWORD	GetSpouseID()				{return m_Property.dwSpouseID;}			//��ż���ID
	void	SetSpouseID(DWORD l)		{m_Property.dwSpouseID=l;}

	DWORD	GetSpouseParam()			{return m_Property.dwSpouseParam;}		//����ż�Ĺ�ϵֵ
	void	SetSpouseParam(DWORD l)		{m_Property.dwSpouseParam=l;}

	DWORD	GetPkCount()				{return m_Property.dwPkCount;}			//���PKɱ����������
	void	SetPkCount(DWORD l)			{m_Property.dwPkCount=l;}

	DWORD	GetPkValue()				{return m_Property.dwPkValue;}			//���PKֵ(����ֵ)
	void	SetPkValue(DWORD l)			{m_Property.dwPkValue=l;}

	DWORD	GetPVPCount()				{return m_Property.dwPVPCount;}			//���PVPɱ���������
	void	SetPVPCount(DWORD l)		{m_Property.dwPVPCount=l;}

	DWORD	GetPVPValue()				{return m_Property.dwPVPValue;}			//���PVPֵ(����ֵ)
	void	SetPVPValue(DWORD l)		{m_Property.dwPVPValue=l;}

	//DWORD   GetMedalScores()            { return m_Property.dwMedalScores; }    //���ѫ�·���
	//void    SetMedalScores(DWORD l)     { m_Property.dwMedalScores = l;}

	bool	GetShowCountry()			{ return m_Property.bShowCountry; }		//����Ƿ���ʾ���ұ�־
	void	SetShowCountry(bool b)		{ m_Property.bShowCountry = b; }

	DWORD	GetHotKey(long lPos)		{return m_Property.dwHotKey[lPos];}		//�Ƚ�(F1~F12,1~+)
	void	SetHotKey(long lPos,DWORD l){m_Property.dwHotKey[lPos]=l;}

	bool	IsCanSetHotKey()					{return m_bCanSetHotKey;}		// ����ܷ����ÿ�ݼ�
	void	SetIsCanSetHotKey(bool bIsCan)		{m_bCanSetHotKey = bIsCan;}		// �����ܷ����ÿ�ݼ���װ̬

	DWORD   GetExpMultiple()             {return m_Property.dwExpMultiple;}     // ��þ��鱶��
	void    SetExpMultiple(DWORD l)      {m_Property.dwExpMultiple = l;}        // ���þ��鱶��

	DWORD   GetNextExpMultiple()         {return m_Property.dwUpgradeExpMultiple;} //����¼����鱶��
	void    SetNextExpMultiple(DWORD l)  {m_Property.dwUpgradeExpMultiple = l;}   //�����¼����鱶��

	//DWORD   GetBatakCredit()             {return m_Property.dwBatakCredit;}     //��ð���������
	//void    SetBatakCredit(DWORD l)      {m_Property.dwBatakCredit = l;}        //���ð���������

	//DWORD   GetZandoCredit()             {return m_Property.dwZanDoHunterCredit;}     //����޶���������
	//void    SetZandoCredit(DWORD l)      {m_Property.dwZanDoHunterCredit = l;}        //�����޶���������

	long	GetCurrBaseCP()				 {return m_Property.lCurrBaseActive;}		//ʣ�����CP
	void	SetCurrBaseCP(long cp)		 {m_Property.lCurrBaseActive = cp;}
	long	GetCurrExCP()				 {return m_Property.lCurrExActive;}			//ʣ����չCP
	void	SetCurrExCP(long cp)		 {m_Property.lCurrExActive = cp;}

	long    GetCurPKCP()                 {return m_Property.lCurPKCP;}           //��õ�ǰCPֵ
	void    SetCurPKCP(long l)           {m_Property.lCurPKCP = l;}              //���õ�ǰCPֵ

//	DWORD   GetChurchCredit()            {return m_Property.dwChurchCredit;}      //��ù�֮�̻�����
//    void    SetChurchCredit(DWORD l)     {m_Property.dwChurchCredit = l;}          //��ù�֮�̻�����

	//PK����(b1:ȫ�屣��,b2:��ӱ���,b4:���ᱣ��,b8:��������,b16:��������,b32:��Ӫ����,b64.��������)
	bool IsPk_All()												
	{
		if(m_Property.byPKOnOff&0x01) 
			return true;
		else
			return false;
	}		
	bool IsPk_Team()												
	{
		if(m_Property.byPKOnOff&0x02) 
			return true;
		else
			return false;
	}
	bool IsPk_Union()											
	{
		if(m_Property.byPKOnOff&0x04) 
			return true;
		else
			return false;
	}
	bool IsPk_RedName()												
	{
		if(m_Property.byPKOnOff&0x08) 
			return true;
		else
			return false;
	}
	bool IsPk_National()												
	{
		if(m_Property.byPKOnOff&16) 
			return true;
		else
			return false;
	}
	bool IsPk_Camp()												
	{
		if(m_Property.byPKOnOff&32) 
			return true;
		else
			return false;
	}
	bool IsPk_Pirates()												
	{
		if(m_Property.byPKOnOff&64) 
			return true;
		else
			return false;
	}
	void	SetPKOnOff(BYTE b)			{ m_Property.byPKOnOff = b; }				//����PK����
	BYTE	GetPKOnOff()				{ return m_Property.byPKOnOff; }

	//�õ���ǰְҵӵ�еļ��ܵ���(SP)
	DWORD	GetSkillPoint()
	{
		return	m_Property.dwOccuSP[m_Property.byOccu];
	}	

	//�õ�ְҵ�ȼ�
	BYTE	GetOccuLvl(eOccupation byOccu)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			return m_Property.byOccuLvl[byOccu];
		return 0;
	}
	void  SetOccuLvl(eOccupation byOccu,BYTE l)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			m_Property.byOccuLvl[byOccu] = l;
	}

	//�õ�ְҵ����
	DWORD	GetOccuExp(eOccupation byOccu)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			return m_Property.dwOccuExp[byOccu];
		return 0;
	}
	void SetOccuExp(eOccupation byOccu,DWORD l)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			m_Property.dwOccuExp[byOccu] = l;
	}
	
	DWORD GetOccuSp(eOccupation byOccu)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			return m_Property.dwOccuSP[byOccu];
		return 0;
	}
	void SetOccuSp(eOccupation byOccu,DWORD l)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			m_Property.dwOccuSP[byOccu] = l;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////
	string	GetOfficialRank()							{return m_strPlayerOfficialRank;}
	void	SetOfficialRank(string str)					{m_strPlayerOfficialRank = str;}

	void	SetOffcialRankTextColor(D3DCOLOR color)		{m_OfficialRankTextColor = color;}
	D3DCOLOR	GetOffcialRankTextColor()				{return m_OfficialRankTextColor;}

	void	SetOffcialRankBkTextColor(D3DCOLOR color)	{m_OfficialRankBkTextColor = color;}
	D3DCOLOR	GetOffcialRankBkTextColor()				{return m_OfficialRankBkTextColor;}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//										����ս�����Խӿ�
	///////////////////////////////////////////////////////////////////////////////////////////////

	DWORD	GetMaxHp()					{return m_FightProperty.dwMaxHp;}				//��������
	void	SetMaxHp(DWORD l)			{m_FightProperty.dwMaxHp=l;}

	WORD	GetMaxMp()					{return m_FightProperty.wMaxMp;}				//��������
	void	SetMaxMp(WORD l)			{m_FightProperty.wMaxMp=l;}

	WORD	GetMaxStamina()				{return m_FightProperty.wMaxStamina;}			//��������
	void	SetMaxStamina(WORD l)		{m_FightProperty.wMaxStamina=l;}

	WORD	GetMaxEnergy()				{return	m_FightProperty.wMaxEnergy;}			//�������� 
	void	SetMaxEnergy(WORD l)		{m_FightProperty.wMaxEnergy=l;}

	WORD	GetHpRecoverSpeed()			{return	m_FightProperty.wHpRecoverSpeed;}		//�����ָ��ٶ�
	void	SetHpRecoverSpeed(WORD l)	{m_FightProperty.wHpRecoverSpeed;}

	WORD	GetMpRecoverSpeed()			{return	m_FightProperty.wMpRecoverSpeed;}		//�����ָ��ٶ�
	void	SetMpRecoverSpeed(WORD l)	{m_FightProperty.wMpRecoverSpeed=l;}

	WORD	GetStamRecoverSpeed()		{return m_FightProperty.wStamRecoverSpeed;}		//�����ָ��ٶ�	
	void	SetStamRecoverSpeed(WORD l)	{m_FightProperty.wStamRecoverSpeed=l;}

	//WORD	GetStrenth()				{return m_FightProperty.wStrenth;}				//����
	//void	SetStrenth(WORD l)			{m_FightProperty.wStrenth=l;}

	//WORD	GetAgility()				{return	m_FightProperty.wAgility;}				//����
	//void	SetAgility(WORD l)			{m_FightProperty.wAgility=l;}

	//WORD	GetConsti()					{return	m_FightProperty.wConsti;}				//���� 
	//void	SetConsti(WORD l)			{m_FightProperty.wConsti=l;}

	//WORD	GetIntell()					{return	m_FightProperty.wIntell;}				//����
	//void	SetIntell(WORD l)			{m_FightProperty.wIntell=l;}

	//WORD	GetWisdom()					{return	m_FightProperty.wWisdom;}				//�ǻ�
	//void	SetWisdom(WORD l)			{m_FightProperty.wWisdom=l;}

	//WORD	GetSpirit()					{return	m_FightProperty.wSpirit;}				//����
	//void	SetSpirit(WORD l)			{m_FightProperty.wSpirit=l;}

    WORD   GetStr()                     { return m_FightProperty.wStr; }
    void   SetStr(WORD s)               { m_FightProperty.wStr = s; }
    WORD   GetDex()                     { return m_FightProperty.wDex; }
    void   SetDex(WORD s)               { m_FightProperty.wDex = s; }
    WORD   GetCon()                     { return m_FightProperty.wCon; }
    void   SetCon(WORD s)               { m_FightProperty.wCon = s; }
    WORD   GetIntellect()               { return m_FightProperty.wIntellect; }
    void   SetIntellect(WORD s)         { m_FightProperty.wIntellect = s; }    
    WORD   GetSpiritualism()            { return m_FightProperty.wSpiritualism; }
    void   SetSpiritualism(WORD s)      { m_FightProperty.wSpiritualism = s; }
    WORD   GetPoint()                   { return m_FightProperty.wPoint; }
    void   SetPoint(WORD s)             { m_FightProperty.wPoint = s; }

	WORD	GetFightBlock()				{return	m_FightProperty.wFightBlock;}			//��
	void	SetFightBlock(WORD l)		{m_FightProperty.wFightBlock=l;}

	WORD	GetParry()					{return	m_FightProperty.wParry;}				//�м�
	void	SetParry(WORD l)			{m_FightProperty.wParry=l;}

	WORD	GetDodge()					{return	m_FightProperty.wDodge;}				//���� 
	void	SetDodge(WORD l)			{m_FightProperty.wDodge=l;}

	WORD	GetCharm()					{return	m_FightProperty.wCharm;}				//����
	void	SetCharm(WORD l)			{m_FightProperty.wCharm=l;}

	WORD	GetLuck()					{return	m_FightProperty.wLuck;}					//����
	void	SetLuck(WORD l)				{m_FightProperty.wLuck=l;}

	WORD	GetHit()					{return m_FightProperty.wHit;}					//����
	void	SetHit(WORD l)				{m_FightProperty.wHit=l;}

	WORD	GetAbsorb()					{return m_FightProperty.wAbsorb;}				//רע
	void	SetAbsorb(WORD l)			{m_FightProperty.wAbsorb=l;}

	long	GetDef()					{return m_FightProperty.wDef;}					//�������
	void	SetDef(WORD l)				{m_FightProperty.wDef=l;}

	long	GetMdef()					{return m_FightProperty.wMdef;}					//ħ������
	void	SetMdef(WORD l)				{m_FightProperty.wMdef=l;}

	long	GetCri()					{return m_FightProperty.wCri;}					//������
	void	SetCri(WORD l)				{m_FightProperty.wCri=l;}

	WORD	GetMAtkSpeed()				{return m_FightProperty.wMAtkSpeed;}			//ʩ���ٶ�
	void	SetMAtkSpeed(WORD l)		{m_FightProperty.wMAtkSpeed=l;}

	WORD	GetBuffPower()				{return m_FightProperty.wBuffPower;}			//Buff����ǿ��
	void	SetBuffPower(WORD l)		{m_FightProperty.wBuffPower=l;}

	long	GetMinAtk()					{return m_FightProperty.wMinAtk;}				//��С������
	void	SetMinAtk(WORD l)			{m_FightProperty.wMinAtk=l;}

	long	GetMaxAtk()					{return m_FightProperty.wMaxAtk;}				//��󹥻���
	void	SetMaxAtk(WORD l)			{m_FightProperty.wMaxAtk=l;}

	long	GetMAtk()					{return m_FightProperty.wMAtk;}					//ħ��������
	void	SetMAtk(WORD l)				{m_FightProperty.wMAtk=l;}

	long	GetMCri()					{return m_FightProperty.wMCri;}					//ħ��������
	void	SetMCri(WORD l)				{m_FightProperty.wMCri=l;}

	WORD	GetBloodSuk()				{return m_FightProperty.wBloodSuck;}				//����������
	void	SetBloodSuk(WORD l)			{m_FightProperty.wBloodSuck=l;}

	WORD	GetManaSuck()				{return m_FightProperty.wManaSuck;}				//����������
	void	SetManaSuck(WORD l)			{m_FightProperty.wManaSuck=l;}

	WORD	GetWillDef()				{return m_FightProperty.wWillDef;}				//��־����
	void	SetWillDef(WORD l)			{m_FightProperty.wWillDef=l;}

	WORD	GetConstiDef()				{return m_FightProperty.wConstiDef;}			//���ʿ���
	void	SetConstiDef(WORD l)		{m_FightProperty.wConstiDef=l;}

	WORD	GetImmunity()				{return m_FightProperty.wImmunity;}				//����
	void	SetImmunity(WORD l)			{m_FightProperty.wImmunity=l;}

	WORD	GetPierce()					{return m_FightProperty.wPierce;}				//��͸
	void	SetPierce(WORD l)			{m_FightProperty.wPierce=l;}

	WORD	GetDeBuffDef(long pos)				{return m_FightProperty.wDeBuffDef[pos];}		//Debuff����
	void	SetDeBuffDef(long pos,WORD l)		{m_FightProperty.wDeBuffDef[pos]=l;}

	WORD	GetElemDef(long pos)				{return m_FightProperty.wElemDef[pos];}		//����Ԫ�ؿ���
	void	SetElemDef(long pos,WORD l)			{m_FightProperty.wElemDef[pos]=l;}

	WORD	GetClassDam(long pos)				{return m_FightProperty.wClassDam[pos];}		//�Ը��������˺�
	void	SetClassDam(long pos,WORD l)		{m_FightProperty.wClassDam[pos]=l;}

	WORD	GetElemDam(long pos)				{return m_FightProperty.wElemDam[pos];}		//�Ը���Ԫ���˺�
	void	SetElemDam(long pos,WORD l)			{m_FightProperty.wElemDam[pos]=l;}

	WORD	GetFightHpRecoverSpeed()			{return	m_FightProperty.wFightHpRecoverSpeed;}	//ս��ʱ�����ָ��ٶ�
	void	SetFightHpRecoverSpeed(WORD l)		{m_FightProperty.wFightHpRecoverSpeed=l;}

	WORD	GetFightMpRecoverSpeed()			{return	m_FightProperty.wFightMpRecoverSpeed;}	//ս��ʱ�����ָ��ٶ�
	void	SetFightMpRecoverSpeed(WORD l)		{m_FightProperty.wFightMpRecoverSpeed=l;}

	short	GetEnerRecoverSpeed()				{return m_FightProperty.wEnerRecoverSpeed;}		//�����ָ��ٶ�
	void	SetEnerRecoverSpeed(short l)		{m_FightProperty.wEnerRecoverSpeed=l;}

	short	GetFightEnerRecoverSpeed()			{return m_FightProperty.wFightEnerRecoverSpeed;}	//ս�������ָ��ٶ�
	void	SetFightEnerRecoverSpeed(short l)	{m_FightProperty.wFightEnerRecoverSpeed=l;}

//	WORD	GetEnergySuck()						{return	m_FightProperty.wEnergySuck;}			//����������
//	void	SetEnergySuck(WORD l)				{m_FightProperty.wEnergySuck=l;}

	WORD	GetCriDef()							{return	m_FightProperty.wCriDef;}				//����������
	void	SetCriDef(WORD l)					{m_FightProperty.wCriDef=l;}

	WORD	GetMCriDef()						{return	m_FightProperty.wMCriDef;}				//ħ����������
	void	SetMCriDef(WORD l)					{m_FightProperty.wMCriDef=l;}
	long    GetMaxPKCP()                        {return m_FightProperty.lMaxPKCP;}             //�������PKCPֵ
	void    SetMaxPKCP(long l)                  {m_FightProperty.lMaxPKCP=l;}                   //��������PKCPֵ

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////���︽�����Խӿ�////////////////////////////////////////////////////////////

	BYTE GetShowFashion()				{ return m_AppendProperty.bShowFashion; }		//�Ƿ���ʾ��������
	void SetShowFashion(BYTE by)		{ m_AppendProperty.bShowFashion = by; }

	const char *GetAccount()			{return m_AppendProperty.strAccount;}			//�ʺ�
	void	SetAccount(const char* l)	{strcpy_s(m_AppendProperty.strAccount,ACCOUNT_SIZE,l);}

	const char *GetTitle()				{return m_AppendProperty.strTitle;}				//�ƺ�
	void	SetTitle(const char* l)		{strcpy_s(m_AppendProperty.strAccount,ACCOUNT_SIZE,l);}

	bool	GetIsCharged()				{return m_AppendProperty.bIsCharged;}			//�Ƿ��ֵ
	void	SetIsCharged(bool l)		{m_AppendProperty.bIsCharged=l;}

	/////////////////////////////////////////////////�������Խӿ�/////////////////////////////////////////////////
	DWORD	GetCountyID()				{return m_FriendshipCounty.dwCountyID;}				//������ID
	void	SetCountyID(DWORD id)		{m_FriendshipCounty.dwCountyID=id;}

	DWORD	GetFrienDeg()				{return	m_FriendshipCounty.dwFrienDeg;}				//�Ѻö�
	void	SetFrienDeg(DWORD deg)		{m_FriendshipCounty.dwFrienDeg=deg;}

	long	CheckSpace(CGoods* pGoods);//���ռ�
	void	RefreshProperty();			// ���ݵ�ǰװ��ˢ����������
	void	MountEquip(CGoods *pGood,bool bPositive=true);
	void	UnMountEquip(CGoods *pGood);
	long	CanMountEquip(CGoods *pGoods);

    void	SetguID(LONG lExtendID,const CGUID& guid);
	void	SetguMoneyID(const CGUID& guid)		{m_guMoneyID =guid; }
	void	SetguSilverMoneyID(const CGUID& guid){m_guSilverMoneyID = guid;}
	void	SetguBaoShiID(const CGUID& guid)	{m_guBaoShiID =guid; }
	void	SetguYuanBaoID(const CGUID& guid)	{m_guYuanBaoID =guid;}
	void	SetguJiFenID(const CGUID& guid)		{m_guJiFenID =guid;}

	CGUID&	GetguMoneyID()		{return m_guMoneyID;}
	CGUID&	GetguSilverID()		{return m_guSilverMoneyID;}
	CGUID&	GetguBaoShiID()		{return m_guBaoShiID;}
	CGUID&	GetguYuanBaoID()	{return m_guYuanBaoID;}
	CGUID&	GetguJiFenID()		{return m_guJiFenID;}

	//������ֵ�ı��Ժ����
	virtual bool  OnPropertyChanged(const char* attrName);
	//�õ�����ֵ���ַ�����ʾ
	virtual const char* GetPropertyStringValue(const char* pszPropertyName);
	//�Ƿ���ʾ��ż����
	bool IsShowLoverName() {return m_bIsShowLoverName;}
	void SetIsShowLoverName(bool isShow) {m_bIsShowLoverName = isShow;}
public:
	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	virtual bool DecordOrgSysFromByteArray(BYTE* pByte, long& pos);

	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData = true);
	virtual bool DecodeOrgSysFromDataBlock(DBReadSet& setReadDB);

	//////////////////////////////////////////////////////////////////////////
	//	ս��
	//////////////////////////////////////////////////////////////////////////
public:
	void SetSinState(bool b)		{m_bSinState=b;}
	bool IsSinState()				{return m_bSinState;}
	void SetHitNum(long l)			{m_lHitNum=l;}
	long GetHitNum()				{return m_lHitNum;}
	void SetLastHitTimer(DWORD dw)	{m_dwLastHitTimer=dw;}
	DWORD GetLastHitTimer()			{return m_dwLastHitTimer;}
	DWORD GetDefaultAttackSkillID();	//���ܵ�ѡ���߼�

	// ���壺ħ�����Խӿ�
	void SetMagicElem( eAttackDefend eAD, eMagicElement eMT, long lVal );
	long GetMagicElem( eAttackDefend eAD, eMagicElement eMT );
	long GetNumOfXiangXing();

protected:
	bool m_bSinState;			// ����״̬
	long m_lHitNum;				// ��ǰ����������
	DWORD m_dwLastHitTimer;		// ��һ�λ��е�ʱ��

	/////////////////////////////////////////////////////////////////////
	// AI
	/////////////////////////////////////////////////////////////////////
public:
	virtual void AI();		// AI����

	/////////////////////////////////////////////////////////////////////
	// ��Ϣ��Ӧ
	/////////////////////////////////////////////////////////////////////
public:
	virtual void OnMessage(CMessage*);

	/////////////////////////////////////////////////////////////////////
	// ��������HP/MP/MA
	/////////////////////////////////////////////////////////////////////
protected:
	long m_lGrowTimer;

public:
	list<short>	m_listGrowHP;
	list<short> m_listGrowMP;

	//////////////////////////////////////////////////////////////////////////
	// ������
	//////////////////////////////////////////////////////////////////////////
public:
	struct tagTeammate
	{
		tagTeammate(CGUID& id):ID(id) {}
		tagTeammate(){}
		bool operator == (const tagTeammate & t) { return t.ID == ID; }
		CGUID ID;
		WORD wHP;
		WORD wMaxHP;
		BYTE occupation;
		CGUID RegionID;
		string strName;
	};
	struct tagPlayerInfo
	{
		DWORD dwForeColor;
		DWORD dwBackColor;
		DWORD dwStartTime;
		BYTE backAlpha;
		BYTE foreAlpha;
		string strText;
	};

private:
	long lTest;
	vector<tagTeammate> m_vecTeammates;
	CGUID m_CaptainID;
	string m_strTeamname;
	CGUID m_TeamID;
	bool m_bIsRecruitingTeam;
	long m_lTeammateAmount;
	bool m_bTeamHasPassword;
	long m_lLimtSkillID;				// ������Ƽ���ID
	long m_lLimtSkillLev;				// ������Ƽ��ܵĵȼ�
	long m_lAllotMade;					// ����ķ��䷽ʽ

private:
	vector<tagPlayerInfo> m_vecPlayerInfo;
	//-----------07.8.3(liuke)----------
	vector<tagWSMemberInfo> m_vecTeamMateInfo;// �����Ա�Ļ�����Ϣ
	//vector<tagMemberIdioinfo> m_vecMemberInfo;// ��Ա��ϸ��Ϣ
	void DecordRecruitDataFromByteArray(BYTE* pByte, long &pos);		// ��ļ��Ϣ
	void DecodeRecruitDataFromDataBlock(DBReadSet& setReadDB);
	//----------------------------------

public:
	vector<tagPlayerInfo> & GetPlayerInfo(){return m_vecPlayerInfo;}

public:
	//-----------07.8.3����µĶ�����Ϣ�б�(liuke)-----------
	vector<tagWSMemberInfo> & GetTeamMateInfo() { return m_vecTeamMateInfo; }
	//-------------------------------------------------------
	vector<tagTeammate> & GetTeam() { return m_vecTeammates; }
	// ���úͻ�ö���ľ�����䷽ʽ
	void SetAllotMade(long lMode)	{m_lAllotMade = lMode;}
	long GetAllotMade()				{return m_lAllotMade;}
	// �쿴ָ��������Ƿ��Ǳ������
	bool IsTeammate(const CGUID& id);
	// �쿴ָ��������Ƿ��Ǳ������
	bool IsTeammate(const char *szName);
	// ���ָ��ID��Ա����Ϣ
	tagWSMemberInfo * GetWsMemberInfo(const CGUID& id);
	// ��ú����öӳ�
	const CGUID& GetCaptainID() const { return m_CaptainID; }
	void SetCaptainID(CGUID& id);// { m_CaptainID = id; }
	// ��ú����ö���
	const CGUID& GetTeamID(){return m_TeamID;}
	void SetTeamID(CGUID& ID){m_TeamID=ID;}
	// ��ú�������ļ״̬
	bool IsRecruitingTeam(){return m_bIsRecruitingTeam;}
	void SetRecruitingTeam(bool b){m_bIsRecruitingTeam=b;}
	// ��ú����ö����Ա������
	long GetTeammateAmount(){return m_lTeammateAmount;}
	void SetTeammateAmount(long l){m_lTeammateAmount=l;}
	// ��ú����ö��������
	const char * GetTeamname(){return m_strTeamname.c_str();}
	void SetTeamname(const char *strName){m_strTeamname=strName;}
	// ��ú����ö����Ƿ��������
	bool IsTeamHasPassword(){return m_bTeamHasPassword;}
	void SetTeamHasPassword(bool b){m_bTeamHasPassword=b;}
	// �Ƿ���Բ���������
	bool IsOperforTeam();
	// ����������Ƽ���
	void SetLimtSkill(long ID, long lLev)	{m_lLimtSkillID = ID;m_lLimtSkillLev = lLev;}

	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
public:
	bool AddSkill(long lSkillId, WORD wLevel=1, DWORD dwRestoreTime=0, float RangeMin=1.0f, float fRangMax=1.0f, WORD wCostMP=0);		// ��Ӽ���
	bool DelSkill(DWORD lSkillId);						// ɾ������
	bool DelSkill(long lPos);								// ɾ�����ܣ�λ��)
	tagSkill*		GetSkill(DWORD dwID);						// ��ȡ����
	bool SetSkillLevel(const char* strName, WORD wLevel, DWORD dwRestoreTime=0, float RangeMin=1.0f, float RangMax=1.0f, WORD wCostMP=0);	// ���ü��ܵȼ�
	WORD CheckSkill(DWORD dwIndex);							// ����Ƿ���ָ�����ܣ����صȼ� (0����û��)
	void UpdateSkillCoolDownTime(bool bIsPublicTime=false);	// ˢ�¼�����ȴʱ��(������ʾ�Ƿ�ˢ�¹�����ȴʱ��)
	void SortSkillList();									// Ϊ�����б���������
	/***********************************************************************/
	/*  zhaohang 2010-10-21 add*/
	void UpdateSkillAttr(long skillid,long level,
		const char* strKey,long value);						//���¼�������
	/***********************************************************************/
	////////////////////07.12.17 XY��Ӳ�����������Ʒ����������ȴЧ���Ľӿ�///////////////////
	list<tagShortCutGoods>* GetShortCutGoodsList() {return &m_listShortCutGoods;}	
	bool AddShortCutGoods(ulong wGoodsIndex);				//�������ΪwGoodsIndex����Ʒ���б���
	bool DelShortCutGoods(ulong wGoodsIndex);				//ɾ������ΪwGoodsIndex����Ʒ
	void SetGoodsRestoreTime(ulong wGoodsIndex,ulong dwTime);		//��������ΪwGoodsIndex����Ʒ����ȴʱ��
	void SetGoodsCoolDownStartTime(ulong wGoodsIndex,ulong dwTime); //��������ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
	ulong GetGoodsRestoreTime(ulong wGoodsIndex);					//�õ�����ΪwGoodsIndex����Ʒ����ȴʱ��
	ulong GetGoodsCoolDownStartTime(ulong wGoodsIndex);		//�õ�����ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
	void SetGoodsUseState(ulong wGoodsIndex,bool bState);	//��������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬
	bool GetGoodsUseState(ulong wGoodsIndex);				//ȡ������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬

	///��������ΪwGoodsIndex����Ʒ��ʣ����ȴʱ��
	void SetGoodsRemainTime(ulong wGoodsIndex,ulong dwTime);
	///�õ�����ΪwGoodsIndex����Ʒ����ȴʱ��
	DWORD GetGoodsRemainTime(ulong wGoodsIndex);

	///�����Ʒ������ȴ����
	bool AddPublicCooldownType(WORD wType,DWORD dwTime);

	///��ȡĳ�����͵���Ʒ������ȴʣ��ʱ��
	DWORD GetReTimeOfPCooldownType(WORD wType);
	////////////////////////////////////////////////////////////////////////////

	//------------------07.11.22liuke����ѧϰ�����ж�--------------
	bool IsCanStudySkill(DWORD dwSkillID, WORD wLevel);		// ����ָ�����������ļ��ܺ͵ȼ����ж��Ƿ���ѧϰ
	static WORD m_wPublicCoolTime;							// ������ȴʱ�䣨ʹ�ü��ܻ����ĵ���֮��Ļ�����ȴʱ�䣩
	//------------------------------------------------------------------------
	// GMȨ�ޱ�־
	//------------------------------------------------------------------------
private:
	
	long m_lGMLevel;
public:
	void SetGMLevel( long lLevel ) {m_lGMLevel = lLevel;}
	long GetGMLevel() {return m_lGMLevel;}

public:
	struct tagownedReg
	{
		long lRegionID;						//����ID
		WORD wType;							//��������
	};
	//�л�ϵͳ
private:
	CGUID	m_FactionID;					//�Լ��л�ID
	string	m_strFactionName;				//�л�����
	string	m_strFactionTitle;				//�л�ĳƺ�
	CGUID	m_FacMasterID;					//�������л�İ���ID;
	long	m_lFactionJob;					//�л�ٽ�
	CGUID	m_UnionID;						//�Լ����ڵ�ͬ��ID
	set<CGUID,guid_compare> m_EnemyFactions;				//�ж��л��б�
	set<CGUID,guid_compare> m_CityWarEnemyFactions;		//��ս�ĵж��л��б�
	list<tagownedReg> m_OwnedRegions;		//ӵ�еĳ����б�

	bool	m_bContendState;				//�Ƿ��������״̬
	bool	m_lCityWarDiedState;			//��ս����״̬

	DWORD	m_dwFacIconCRC;					//�л�ͼ��CRC

public:
	const CGUID& GetFactionID()			{return m_FactionID;}
	void	SetFactionID(const CGUID& ID)	{m_FactionID=ID;	}

	const string& GetFactionName() {return m_strFactionName;	}
	void	SetFactionName(string& strName){m_strFactionName=strName;}

	const string& GetFactionTitle() {return m_strFactionTitle;	}
	void	SetFactionTitle(string& strTitle){m_strFactionTitle=strTitle;}

	const CGUID& GetFactionMasterID()	{return m_FacMasterID;}
	void	SetFacMasterID(const CGUID& id){m_FacMasterID = id;}

	long	GetFactionJob()				{return m_lFactionJob;}
	void	SetFactionJob(long lFacJob)	{m_lFactionJob = lFacJob;}

	bool	GetIsContendState()		{return m_bContendState;}
	void	SetContendState(bool b)	{m_bContendState = b;}

	bool	GetCityWarDiedState()	{return m_lCityWarDiedState;}
	void	SetCityWarDiedState(bool b)	{m_lCityWarDiedState = b;}

	DWORD	GetFacIconCRC()						{return m_dwFacIconCRC;}
	void	SetFacIconCRC(DWORD dwFacIconCRC)	{m_dwFacIconCRC = dwFacIconCRC;}

	const CGUID&	GetUnionID()			{return m_UnionID;}
	void	SetUnionID(const CGUID& ID)	{m_UnionID=ID;}

	void	SetEnemyFactions(set<CGUID,guid_compare>& Factions) { m_EnemyFactions = Factions; }
	bool	IsHaveEnemyFactions()		{return m_EnemyFactions.size() > 0;}
	bool	IsEnemyFaction(const CGUID& FactionID)
	{
		return m_EnemyFactions.find(FactionID) != m_EnemyFactions.end();
	}
	bool	IsHaveCityWarEnemyFactions(){return m_CityWarEnemyFactions.size()>0;}
	bool	IsCityWarEnemyFaction(const CGUID& ID)
	{
		return m_CityWarEnemyFactions.find(ID) != m_CityWarEnemyFactions.end();
	}
	bool	IsFriendFaction(const CGUID& FactionID,const CGUID& UnionID)
	{
		if(FactionID == CGUID::GUID_INVALID || GetFactionID() == CGUID::GUID_INVALID )	return false;
		if(FactionID == GetFactionID())	return true;
		if(UnionID == CGUID::GUID_INVALID  || GetUnionID() == CGUID::GUID_INVALID )	return false;
		if(UnionID == GetUnionID())	return true;
		return false;
	}
	WORD GetDisplayOwneRegionIcon();							//�õ���ʾӵ�г�����IcongID
	//����ϵͳ-----------------07.9.6liuke�޸�--------------------
public:
	// ��¼��ҵ��������볡����Ϣ��ص���Чͼ��
	struct stQuestInfoByRgn
	{
		long lQuestID;
		long lIndex;
		long lRgnID;
		long lRgnX;
		long lRgnY;
		long lEffectIDOK;
		long lEffectIDNO;
		CGUID guidEffect;
		stQuestInfoByRgn()
		{
			lQuestID	= 0;
			lIndex		= 0;
			lRgnID		= 0;
			lRgnX		= 0;
			lRgnY		= 0;
			lEffectIDOK = 0;
			lEffectIDNO = 0;
			guidEffect	= CGUID::GUID_INVALID;
		}
	};
	list<stQuestInfoByRgn>* GetQuestInfoByRgn(){return &m_lsQuestInfoByRgn;} // ������صĳ�����Ϣ�б�
	CQuestQueue * GetRoleQueue()	{return &m_RoleQueue;}
	CMercenaryQuestSys* GetMercQuestSys() {return &m_MerQuestSys;}

	bool AddQuest(long lQuestID, long lStepID);					// ���ָ�������һ������
	const char * RemoveQuest(long lQuestID);					// ɾ��һ������
	bool UpdataQuestStep(long lQuestID, long lStepIndex);		// ����һ��������
	bool UpdataQuestTarget(long lQuestID, long lIndex, long lValue,long lRegionID=0, long x=0, long y=0);	// ����һ�����������׷��
	void CollectionQuestClewInfo(CMonster *pNpc);				// �ɼ�������NPC��Ӧ��������ʾ��Ϣ
	void UpdataSpecialQuestInfoByNpc(CMonster *pNpc);			// ���³����ϵ������������NPC����ʾ��Ϣ
	void UpdataQuestInfoAboutNpc(CMonster *pNpc=NULL);			// ����ָ��NPC������������б��е����������ʾ��Ϣ(Ĭ��Ϊ���³���������������NPC��������ʾ״̬)
	void UpdataQuestInfoAboutNpcByQuestID(long lQuestID);		// ����ָ�������NPC�����������ʾ��Ϣ

	void UpdataQuestByGoods(CGoods *pGoods);					// ��Ʒ�仯������������

	void SetQuestInfoByRgn(long lQuestID, long lIndex, stQuestInfoByRgn tagInfo);// ����ָ�������ָ�����еĵ�ͼ��Ч
	void DelQuestInfoByRgn(long lQuestID, long lIndex);			// ɾ��ָ�������ָ�����еĵ�ͼ��Ч
	void DelAllQuestInfoByRgn(long lQuestID);					// ɾ��ָ����������е�ͼ��Ч
	//****************************����08.11.11***************************//
	void TidySearchQuestList();									// ����������������б��ﵽ�ɽ���������������
	vector<CQuest *> GetCanSearchQuest()	{return m_vecCanSearchQuest;}	// ��õ�ǰ����������������������б�
	CQuest * RedomCanSearchQuest(bool bIsReSearch=false);					// ������һ��������������������(������ʾ�Ƿ�������������б�)
	//*******************************************************************//
private:
    // friend CEGUI::Window* InitQuest();
    // friend bool OnQuestTabSelectChanged(const CEGUI::EventArgs& args);
	CQuestQueue m_RoleQueue;		// �����б����
	vector<CQuest *>	m_vecCanSearchQuest;					// ��ǰ����������������������б�
	//ٸ������
	CMercenaryQuestSys	m_MerQuestSys;
	list<stQuestInfoByRgn> m_lsQuestInfoByRgn;					// ������صĳ�����Ϣ�б�

	////-------------------------------07.9.6liukeע��-------------------------------
	//public:
	//	struct tagPlayerQuest
	//	{
	//		WORD	wQuestID;			//�����ID
	//		bool	bDisplay;			//�Ƿ���ʾ
	//		long	lEffectID;			//�����ʾ��ЧID
	//		long	lRegionID;			//��������Чλ��
	//		long	lTileX;
	//		long	lTileY;
	//		CEffect*	pEffect;
	//
	//		string	strName;			//��������
	//		string	strDesc;			//��������
	//	};
	//private:
	//	long						m_lMaxQuestNum;	//�����������
	//	map<WORD,tagPlayerQuest>	m_PlayerQuests;	//����ѻ�õ������б�
	//
	//public:
	//	bool DecordQuestDataFromByteArray(BYTE* pByte, long& pos);
	//
	//	map<WORD,tagPlayerQuest> * GetPlayerQuests(){return &m_PlayerQuests;}
	//	long GetMaxQuestNum()		{return m_lMaxQuestNum;}
	//	void AddQuest(WORD wQuestID,char* strName,char* strDesc,bool bDisplay,long lEffectID,long lRegionID,long lTileX,long lTileY);//����������һ������
	//	void RemoveQuest(WORD wQuestID);							//�Ƴ�һ������
	//	void UpdateQuestPosXY(WORD wQuestID,long lReginID,long lTileX,long lTileY);
	//===============================================================================


    // �������ע�� Bugs 2008-9-12
//	//�������
//private:
//	set<CGUID> m_setPetID;
//public:
//	set<CGUID> *GetPetIDSet()	{return &m_setPetID;}
//	bool HasPetArount();		//�ж���Χ��û���Լ��ĳ���
//	long GetPetAmount();		//�����Լ���Χ���������


	//����
private:
	CEmotionShower *m_pEmotionShower;
public:
	virtual void SetAction(WORD l);
	virtual void SetState(WORD l);
	void SetEmotion(long lIndex,DWORD dwTime=0);
	bool IsEmotionShowing(); 
	///////////////////////////////////////////////////////////////
	//����
	///////////////////////////////////////////////////////////////
private:
	CHorse	   m_Horse;
	CTalisman  m_Talisman;
	CWing	   m_Wing;
	CHeadWear  m_HeadWear;
	CWeapon    m_LeftHandWeapon;
	CWeapon    m_RightHandWeapon;
	CWeapon    m_REnergy;
	CWeapon    m_LEnergy;
	CWeapon    m_CollectTool;
	BOOL       m_bPlayEnergyEffect;
	BOOL       m_bEnergyTimeStarted;
	DWORD      m_dwEnergyHitEffectID;
	DWORD      m_dwEnergyStartTime;
public:
	CHorse * GetHorse() { 
		return &m_Horse;
	}
	CTalisman * GetTalisman() { return &m_Talisman;}
	void UpdateSpecialEquipment();
	CWing * GetWing() { return &m_Wing;}
	CHeadWear * GetHeadWear() { return &m_HeadWear;}
	CWeapon * GetLeftHandWeapon() { return &m_LeftHandWeapon;}
	CWeapon * GetRightHandWeapon() { return &m_RightHandWeapon;}
	CWeapon * GetCollectTool() { return &m_CollectTool;}
	CWeapon * GetLEnergy() { return &m_LEnergy;}
	CWeapon * GetREnergy() { return &m_REnergy;}
	void      PlayEnergyEffect(DWORD energylevle);
	void      EndEnergyEffect();
	//���ݳ���ǿ�ȸ��³�����Ч
	void      UpdateEnergyEffect(DWORD energylevle);
	void      SetEnergyHitEffectID(DWORD ID ){m_dwEnergyHitEffectID = ID;}
	DWORD     GetEnergyHitEffectID(){return m_dwEnergyHitEffectID;}

	void SetGreatcoatVisible(BOOL bIsVisible);			//�趨���׵Ŀɼ���
	void SetManteauVisible(BOOL bIsVisible);			//�趨����Ŀɼ���

private:
	DWORD	m_dwMoney;				//	���
	DWORD	m_dwSilverMoney;		//	����
	DWORD	m_dwBaoShi;
	DWORD	m_dwYuanBao;
	DWORD	m_dwJiFen;
public:
	DWORD	GetMoney()					{return m_dwMoney;}
	void	SetMoney(DWORD l)			{m_dwMoney=l;}
	DWORD	GetSilverMoney()			{return m_dwSilverMoney;}
	void	SetSilverMoney(DWORD l)		{m_dwSilverMoney=l;}
	DWORD	GetBaoShi()					{return m_dwBaoShi;}
	void	SetBaoShi(DWORD l)			{m_dwBaoShi=l;}
	DWORD	GetYuanBao()				{return m_dwYuanBao;}
	void	SetYuanBao(DWORD l)			{m_dwYuanBao=l;}
	DWORD	GetJiFen()					{return m_dwJiFen;}
	void	SetJiFen(DWORD l)			{m_dwJiFen=l;}
	//��ǰ�ĳƺ�
private:
	long    m_lTitleType;                       //��ҵ�ǰʹ�õĳƺ�����
	string  m_pPlayerTitle;                      //��ҵ�ǰʹ�õĳƺŵ�ֵ
	vector<DWORD>  m_VecMedalOnPlayer;          //���ӵ�е�ѫ������
public:
	void SetPlayerTitle(string strTitle) {m_pPlayerTitle = strTitle;}                // ������ҵ�ǰ�ĳƺ�
	//string GetPlayerTitle()  {return m_pPlayerTitle;};                                  // �����ҵ�ǰ�ĳƺ�
	string GetPlayerTitle();                                 // �����ҵ�ǰ�ĳƺ�
	void SetTitleType(long nTitleType) {m_lTitleType = nTitleType;}                    // ���õ�ǰ�ƺŵ�����
	long GetTitleType() {return m_lTitleType;}                                         // ��õ�ǰ�ƺŵ�����
	void DecodeMedalFromDataBlockAndInit(DBReadSet& setReadDB);                             // ������ɫ��ѫ���б�
	vector<DWORD> GetVecMedalOnPlayer() { return m_VecMedalOnPlayer;};                 // ��ý�ɫ��ѫ��
	void AddMedal(DWORD Index);          //���һ��ѫ��
	void DeleteMedal(DWORD Index);       //ɾ��һ��ѫ��
	bool FindMedalOnPlayer(DWORD Index);  //����������������ϵ�ѫ��

	void UpdateAccouterment();					//������Ʒ

	//���﹦��
private:
	map<CGUID, CPet*> m_PetList;	///< ���г����б�
	set<CGUID> m_ShowPets;			///< չʾ�����б�
	CGUID m_ActivePetGUID;			///< �������GUID
	CGUID m_CurPetGUID;				///< ��ǰ�������GUID
	CGUID m_CurOperPetGUID;			///< ��ǰ��������GUID
	
public:
	/// ȡ����ҳ����б�
	map<CGUID, CPet*>* GetPetList() { return &m_PetList; }
	/// ȡ�����ӵ�г�����Ŀ
	long GetPetNum(void) { return (long)m_PetList.size(); }
	/// ȡ�ü������GUID
	const CGUID& GetActivePetGUID(void) { return m_ActivePetGUID; }
	/// ���ü������GUID
	void SetActivePetGUID(CGUID guid) { m_ActivePetGUID = guid; }
    /// ȡ�ü������
    CPet* GetActivePet() const;
	/// ��������
	CPet* CreatePet(const CGUID& guid);
	/// ɾ������
	void DelPet(const CGUID& guid);
	/// չʾ����
	void ShowPet(const CGUID& guid);
	/// �ջس���
	void UnShowPet(const CGUID& guid);
	/// ���ݳ���GUIDȡ�ó���
	CPet* GetPet(const CGUID& guid);
	/// �������
	void DecodePetFromDataBlock(DBReadSet& setReadDB, bool bExData);
	/// ��ӳ���
	void AddPet(CPet *pPet);
	/// ȡ�ó�ս״̬������Ŀ
	long GetFightPetNum();
	/// ���ó���Ϊ��ս״̬
	void SetPetFighting(CPet *pPet);
	/// ������г���
	void PetEnterRegion();
	/// ������＼������
	void SavePetSkillSetting();

	CPet* GetCurPet() { return GetPet(m_CurPetGUID); }
	void SetCurPetGUID(const CGUID& guid) {m_CurPetGUID = guid;}

	CPet* GetCurOperPet() { return GetPet(m_CurOperPetGUID); }
	void SetCurOperPetGUID(const CGUID& guid);

	void ValidatePetInterface();				//Ч�鵱ǰ������ؽ���򿪻�ر�
private:
	/// �����ҳ����б�
	void RemoveAllPet();


//////////////////////////////////////////////////////////////////////////
// ��Ҿ�����ʾ��ʾ
//////////////////////////////////////////////////////////////////////////
public:
	enum TIPTEXT_TYPE
	{
		TT_EXP,			//����ֵ������ʾ�ı�
		TT_CON,			//��ѫֵ������ʾ�ı�
	};

	struct tagTipText
	{
		TIPTEXT_TYPE eType;			//��ʾ����
		string		 sNumber;		//����
		float		 fAlpha;		//͸����
		DWORD		 dwStartTime;	//��ʼʱ��
		D3DXVECTOR3  vPos;			//λ������
};

	void AddTipText(TIPTEXT_TYPE eType, long lNumber);		//�����ʾ�ı�
	void DisplayTipText();									//��ʾ��ҵ���ʾ�ı�

private:
	vector<tagTipText> m_vecTipText;	//��Ϸ����һ�þ���͹�ѫ��ʾ��
	DWORD m_dwLevelCurrentDisplayNum;	//��ǰ������ʾ�Ĳ㼶����
	DWORD m_dwLevelMax;					//���㼶(�㼶��0��ʼ,���������ۼ�)

	long m_lOldContribute;		//����ϴεĹ�ѫֵ

public:
	void SetOldContribute(long lOldContribute)		{ m_lOldContribute = lOldContribute; };		//�趨����ϴεĹ�ѫֵ
};



//������ʾ��
class CEmotionShower
{
public:
	//����ṹ
	struct tagEmotion
	{
		DWORD dwType;
		DWORD dwIndex;
		int nSoundLoops;
		int nSoundChannel;
		char sounds[128];

		vector<DWORD>vecActions;
		vector<DWORD>vecTypes;
		string strName;
		string strText;
		string strSound;
	};
private:
	CPlayer *m_pSource;
	int m_nCurIndex;
	tagEmotion m_stEmotion;
	//CDisplayModel::AnimInfo::tagActionInfo *m_pActionInfo;
public:
	CEmotionShower(CPlayer *m_pSource);
	~CEmotionShower();
	//void SetActionInfo(GameModel::AnimInfo::tagActionInfo *pActionInfo);
	bool IsStopped();
	void Init(int emotionType);
	void Start(DWORD dwStartTime=0);
	void Stop();
	void Update();

private:
	void ShowNextAction();
	void RestoreToNormal();
};