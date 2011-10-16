#pragma once
//#define MAX_MONEY 4000000000
#include "moveshape.h"
#include "..\..\setup\playerlist.h"
#include "..\..\setup\IncrementShopList.h"

#include "Container\CAmountLimitGoodsContainer.h"
#include "Container\CAmountLimitGoodsShadowContainer.h"
#include "Container\CVolumeLimitGoodsContainer.h"
#include "Container\CEquipmentContainer.h"
#include "Container\CSubpackContainer.h"
#include "Container\CWallet.h"
#include "container\CSilverWallet.h"

#include "..\..\..\public\PlayerDef.h"
#include "..\..\..\public\BasePetServerDef.h"
#include "..\..\..\public\RoleRecord.h"
#include "PlayerDepot.h"
#include "..\..\public\DeputyOccu.h"
#include "..\..\public\DeputyOccuRecord.h"
#include "..\public\OrganizingDef.h"
#include "..\public\StrType.h"
#include "SpriteSystem.h"
#include "..\public\PlayerSetupOnServer.h"
#include "..\public\teamdef.h"
#include "..\public\Cosmetology.h"
#include "MedalContainer.h"
#include "Mail/MailManager.h"
#include "business\CBusinessPackage.h"
#include "GoodsAllPurposeOpt.h"
#include "GoodsTimerHandler.h"
#include "..\..\public\QuestIndexXml.h"
#include "..\..\..\public\FunctionKeyMapSystem.h"
#include "Lottery\CLottery.h"

#include <set>
#include <map>

using namespace std;

class CGoods;
class CMoveShape;
class CVariableList;
class CMyServerClient;
class CMonster;
class CQuest;
class CQuestStep;
class CQuestAim;
class CQusetEncouragement;
class CScript;
struct tagTestResult;
struct tagDelTestResult;
//�ƶ���֤ʱ��,�����ƶ���Ϣ��������
const static int MoveValiMaxMsgCount = 15;



//����������ʧ����͵�����Ʒ�ļ���
struct tagDiedLost
{	
	float fDropEquip_Head;//(ͷ���������)
	float fDropEquip_Necklace;//(�����������)	
	float fDropEquip_Wing;//(������)	
	float fDropEquip_Body;//(���׵������)	
	float fDropEquip_Back;//(�����������)	
	float fDropEquip_Glove;//(���׵������)	
	float fDropEquip_Boot;//(Ь�ӵ������)	
	float fDropEquip_Headgear;//(ͷ�ε������)
	float fDropEquip_Frock;//(���׵������)	
	float fDropEquip_LRing;//(���ָ�������)
	float fDropEquip_RRing;//(�ҽ�ָ�������)
	float fDropEquip_Medal1;//(ѫ��1�������)
	float fDropEquip_Medal2;//(ѫ��2�������)
	float fDropEquip_Medal3;//(ѫ��3�������)
	float fDropEquip_Fairy;//(С����������)
	float fDropEquip_Weapon;//(�����������)
	float fDropEquip_Weapon2;//(���������������)	
	float fDropEquip_Cloak;//(����������)	
	float fDropEquip_Horse;//(����������)
	float fDropEquip_Bag;//(������Ʒ�������)
	float fDropEquip_Money;//(��Ǯ�������)	
	float fDropEquip_MoneyPercent;//(����Ǯ��������)
};

class CPacketListener : public CContainerListener
{
public:
	DBWriteSet* m_pDBWriteData;
	CPacketListener();
	~CPacketListener();

	virtual BOOL OnTraversingContainer(CContainer* pContainer, CBaseObject* pObject);
};

class CPlayer :
	public CMoveShape, public CContainerListener
{
public:
	CPlayer(void);
	virtual ~CPlayer(void);

public:
	// ���＼��
	struct tagSkill
	{
		DWORD	dwID;					//���
		WORD	wLevel;					//�ȼ�
		DWORD	wCooldown;				//��ȴʱ��
		DWORD   dwOldCooldown;			//ԭʼ��ȴʱ��

		bool	bCommonCooldown;		//�Ƿ��ܹ�����ȴʱ��Ӱ��
		DWORD	dwCooldownStartTime;	//��ȴ��ʼʱ��
		time_t	tCooldownEndtime;		//��ȴ����ʱ��
	};

	struct tagSuitGoods
	{
		BOOL bActived;
		CGoods* pGoods;
	};

	struct tagExpendableEffect
	{
		DWORD dwType;	//�������Եı��
		DWORD dwStartTime;	//��ʼ����ʱ��
		DWORD dwEffectTime;	//��Чʱ��
		long  lValue;	//����ֵ
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
		SKILL_NOTFIT,		//���ܲ���
		REGION_FORBID,		//������ֹ
		COOLDOWN_NOTFIT,	//��ȴ����
		EQUIP_TYPE_NOTFIT,	//�������Ͳ���
		BUSINESS_STATE,		//��������״̬
		EQUIPFIT,			//����װ��
	};
	//ս��״̬��pkֵ��pvpֵ����С���ٽ�ֵ
	enum UNDECPKPVP
	{
		eUnDecPKPVP_Value = 20,
	};
	//����ֵ������ֵ
	enum PVPLEVEL
	{
		ePvp_First_Level = 36000,
		ePvp_Second_Level = 72000,
		ePvp_Third_Level = 144000,
	};

	//////////////////////////////////////////////////////////////////////////
	/*�޸���������															*/
	//////////////////////////////////////////////////////////////////////////
public:
	tagPlayerProperty& GetProperty(void) { return m_Property; }
private:

	//��������
	tagPlayerProperty			m_Property;			//��������
	tagPlayerFightProperty		m_FightProperty;		//ս������
	tagPlayerFightProperty		m_BaseFightProperty;	//����ս������
	tagPlayerAppendProperty		m_AppendProperty;		//��������
	tagPlayerElementProperty	m_arrElementProperty[MT_Max];//Ԫ������

	MAPFKEY m_FuncKey;		// �����ɫ�Ĺ����ȼ�ӳ�������޸Ĺ��ģ���Ĭ��ֵ��ͬ��

	float	m_fExpScale;//��þ������
	float	m_fOccuExpScale;// ���ְҵ����ϵ��
	float	m_fSpScale;//��ȡԪ���ı���
	float	m_fPresentExpScale;//��ȡ���;���ı���
	float	m_fActiveScale;			//����ֵ����

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	// ���������� ����ͻ��˷���ʱ���»�ȡ��������
	set<string>  m_mapDirtyAttrData;
	map<string, P_ATTRIBUTE_TYPE>		    m_mapAttrType;		// �������ֶ�Ӧ��ö��ֵ

	//��ʾ����Ƿ��Ѿ�������Ϸ
	bool m_bAlreadyEnterGame;

	void    PushDirtyAttrData(string attrName) { m_mapDirtyAttrData.insert(attrName); }
	string  PopDirtyAttrData() 
	{ 
		string tmp = *m_mapDirtyAttrData.begin(); 
		m_mapDirtyAttrData.erase(m_mapDirtyAttrData.begin());
		return tmp; 
	}
	long    DirtyAttrDataSize(void) { return m_mapDirtyAttrData.size(); }

	typedef map<string, tagPty>::iterator mapNameValueItr; 
	typedef map<string, P_ATTRIBUTE_TYPE>::iterator mapAttrTypeItr;

	// ��Χ��Ҫ���µ�����
	static set<string>  m_mapOtherUpdateDirtyAttrData;
	set<string>			m_mapOtherDirtyAttrData;

	void InsertOtherDirtyName(string& attrName)
	{
		if(m_mapOtherUpdateDirtyAttrData.find(attrName) != m_mapOtherUpdateDirtyAttrData.end())
			m_mapOtherDirtyAttrData.insert(attrName);
	}

	void SetEnterGame(bool b)	{m_bAlreadyEnterGame = b;}
	bool IsEnterGame()	{return m_bAlreadyEnterGame;}
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
public:
	static map<GOODS_ADDON_PROPERTIES,string> m_mapFightEnum;
	static VOID InitFightEnumMap();
	static string GetFightPropertyNameByEnum(GOODS_ADDON_PROPERTIES);

	// [070608 AHC]
	virtual bool  SetAttribute(std::string& attrName, long value);
	virtual bool  SetAttribute(char* attrName, long value);
	virtual long  GetAttribute(std::string attrName);
	virtual bool  ChangeAttribute(std::string& attrName, long value);
	virtual long  UpdateAttributeToClient();
	virtual long  UpdateHPMPToClient(set<string>& dirtyAttr);
	long	UpdateOtherAttributeToClient();

	long  UpdateAttribute(BOOL bImmediately=0);
	long  OnUpdateAttributeTimeOut();

	virtual VOID  ChangeStateTo(CShape::eSTATE state);

	void          CodeChangeAttrToDataBlock(DBWriteSet& setWriteDB);
	bool          CodeOtherUpdateAttrToDataBlock(DBWriteSet& setWriteDB);
	bool	CodeDirtyAttrToDataBlock(set<string>& dirtyAttr, DBWriteSet& setWriteDB);

	void		  AddFightPropertyTODirty();
	//��ӱ�������������
	void  AddAttributeToDirty(string attrName);

	BOOL SerializeSubDepot(DBWriteSet& setWriteDB, LONG eContainerId);

	// ȡ�õ�ǰֵ�ĺϷ�ֵ
	virtual long  GetCurrentValue(char* valueName, const tagPty& tagValue, long evalue/*�ı�ֵ*/, long tmpValue/*���μ���õ���ֻ*/);

	VOID ChangePropertyByGoods(CGoods* pGoods,GOODS_ADDON_PROPERTIES addon,int whichVal,string strAttrName,bool bMount);


	VOID UpdatePropertyByGoodsTrigger(CGoods* pGoods,bool bMount);

	//�ı�ְҵ
	void ChangeOccu(eOccupation eNewOccu);		

	//������Գ�ʼ��
	void InitProperty(void);
	//��ʼ�����;���ֵ
	void InitPresentExp(void);
	//����ս�������е����������ݡ����ʡ��������ǻۡ���������ֵ������ֵ�Ȼ�������
	void ResetBaseFightProperty(void);
	//���¼����������,������,����,����,����,�ǻ�,����Ļ������Ժͱ仯���Է����ı��ʱ�򣬶�Ҫ���¼����,�м�,���ܵȻ�������
	void ResetFightProperty(void);
	//��pkֵ/pvpֵ��һ����������һ���ű�
	void OnPKOrPVPValueChange(long lOldValue,long lNewValue);

	//SCRIPT FUNTIONS
	bool IsGM();
	long GetGMLevel();

	// �����ҵ�TileX,Y�Ƿ���ָ����Χ�� (һ����������)
	// x,y�����������꣬range��ȣ�
	//
	//	��������������
	//	��p1����������	r
	//	��������������	a
	//	������xy������	n = 3
	//	��������������	g
	//	��������������	e
	//	��������������
	//		p2
	//
	// p1 �������ڣ� p2 ��������
	bool IsInArea(/*long regionid,*/ long x, long y, long range = 3);
	// ��ⳡ��
	bool IsInRegion(long regionid);

public:
	struct tagShopSelledGoods 
	{
		DWORD dwSelledPrice;
		CGoods* pGoods;
	};

private:
	//���ʰȡ��Ʒʱ��
	long m_lLastPickGoodsTime;
	//����<ó����ID������ֵ>
	map<DWORD,DWORD> m_mapCredit;
	//Ͷ�ʼ�¼<ó�׵�ID��Ͷ�ʶ�>
	map<DWORD,DWORD> m_mapInvestRecord;
	long m_lInvestLimitTimeMark;

	//��ʱ�����������NPC�̵����Ʒ��������Ʒ�ع�����
	list<tagShopSelledGoods> m_listNpcShopSellGoods;
	CGUID m_TradeSessionId;
	LONG m_lTradePlugId;
	VOID SetTradeSessionId(const CGUID& sessionId){m_TradeSessionId=sessionId;}
	VOID SetTradePlugId(const LONG lId){m_lTradePlugId=lId;}
public:
	const CGUID& GetTradeSessionId(){return m_TradeSessionId;}
	const LONG GetTradePlugId(){return m_lTradePlugId;}
public:
	VOID SetCredit(DWORD dwAreaId,DWORD dwCredit);
	VOID AddCredit(DWORD dwAreaId,DWORD dwCredit);
	VOID ChangeCredit(DWORD dwAreaId,long lCredit);
	DWORD GetCredit(DWORD dwAreaId);
	BOOL SetInvestRecord(DWORD dwSpotId,DWORD dwInvestVal);
	DWORD GetInvestRecord(DWORD dwSpotId);
	VOID ClearInvestRecord(){m_mapInvestRecord.clear();}
	VOID SetInvestLimitTimeMark(long lVal){m_lInvestLimitTimeMark=lVal;}
	long GetInvestLimitTimeMark(){return m_lInvestLimitTimeMark;}

	VOID PushbakNpcShopSellGoods(CGoods* pGoods,DWORD dwPrice);
	CGoods* FetchNpcShopSellGoods(const CGUID& guid);
	CGoods* FindNpcShopSellGoods(CGUID& guid,DWORD* dwPrice);
	VOID ReleaseNpcShopSellGoods();

	VOID SetLastPickGoodsTime(long t){m_lLastPickGoodsTime=t;}
	long GetLastPickGoodsTime(){return m_lLastPickGoodsTime;}

	BOOL UpdateCooldown(DWORD dwGoodsId);

	long GetCreditLevel(DWORD dwAreaID); //��ȡָ������������ȼ�
	long GetOrganLevel(const char* strName);//��ȡָ����֯�������ȼ�
	void AddOrganCredit(const char* strName,DWORD dwCredit);//������֯����
	void ChangeOrganCredit(const char* strName,DWORD dwCredit);//�޸���֯����
	long GetOrganCredit(DWORD dwOrganID);//��ȡ��֯����
private:
	//����
	CVolumeLimitGoodsContainer* m_cOriginPack;
	CSubpackContainer* m_pSubpackContainer;
	VOID gcPacks();		
	map<long,vector<tagSuitGoods>*> m_mSuitState;
	vector<CGoods*> m_vecInPersonalShopGoods;
	DWORD m_dwYuanbao;
	/// ��ҵ����
	Business::CPackage m_BusinessContainer;
public:	
	/// ��ȡ��ҵ����
	Business::CPackage &GetBusinessContainer() { return m_BusinessContainer; }
	/// �жϸ�����Ƿ�������
	bool IsBusinessMan() const { return m_Property.byIsBusinessMan == 1; }
	/// �ı��������
	void SetBusinessMan( bool bBusinessMan ) { m_Property.byIsBusinessMan = ( bBusinessMan ? 1 : 0 ); }
	/// ��������Ƿ�������״̬
	bool IsBusinessState() 
	{
		return IsBusinessMan() && m_BusinessContainer.GetCurGoodsAmount() != 0;
	}

	CVolumeLimitGoodsContainer* getOriginPack(){return m_cOriginPack;}
	CSubpackContainer* getSubpackContainer(){return m_pSubpackContainer;}
	CEquipmentContainer* GetEquipmentContainer(){return &m_cEquipment;} 
	CVolumeLimitGoodsContainer* GetPackContainerById(DWORD dwId);
	int GetSubpackCanPlace();
	VOID OnSuitNumChanged(long suit_id,int suit_num);
	CGoods* FindGoods(LONG lContainerId,LONG lPos);
	//������Ʒԭʼ�������б����в��ҵ�һ����Ʒ
	CGoods* FindGoodInPack(const char* strGoodsName);
	//! ������Ʒԭʼ���ڲֿ����ҵ���һ����Ʒ
	CGoods* FindGoodInDepot(const char* strGoodsName);

	BOOL IsGoodsUnique(const char* strGoodsName);

	BOOL DelGoods(LONG lContainerId,LONG lPos);

	VOID ClearGoods(LONG lContainerId,LONG lPos);

	//! �ҵ�ָ��ԭʼ����������[δ����]��Ʒ
	BOOL FindGoods(char* strOriginName,int num);
	//! ɾ��ָ��ԭʼ����������[δ����]��Ʒ
	BOOL DelGoods(char* strOriginName,int num);
	//! ɾ��ָ��GUID��������[δ����]��Ʒ
	BOOL DelGoods(const CGUID &GoodsGuid,int num);
	//! �ҵ�ָ��������������[δ����]��Ʒ
	BOOL FindGoodsByIndex(LONG lGoodsIdx, LONG num);
	//! ɾ��ָ��������������[δ����]��Ʒ
	BOOL DelGoodsByIndex(LONG lGoodsIdx,  LONG num);

	//! �����Խ��ɾ����Ʒ
	VOID DelGoodsByTestResult(vector<tagDelTestResult> &vecDelResult);

	DWORD FindGoods(LONG lContainerId,LONG lPos,const char* strGoodsName);
	virtual DWORD FindGoodsByName(LONG lContainerId,const char* strGoodsName);
	virtual DWORD FindGoodsByType(LONG lContainerId,int nType);
	DWORD GetTotalEmptySpaceNum();
	DWORD GetTotalSpaceNum();//��ȡ������б��������� 090916by tanglei
	DWORD GetGoodsAmountByOriginName(const char* strOriginName);
	BOOL IsEquipBusinessPack();

	VOID ClearPersonalShopGoodsData(){m_vecInPersonalShopGoods.clear();}
	VOID RemovePersonalShopGoodsData(CGUID& guid);
	BOOL IsInPersonalShop(CGoods* pGoods);
	VOID AddPersonalShopGoodsData(CGoods* pGoods);

	BOOL CheckChangeGoods(vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsIn,vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsOut);
	VOID GoodsChangeGoods(vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsIn,vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsOut);

	BOOL SerializeSubpack(DBWriteSet& setWriteDB,CS2CContainerObjectMove::PLAYER_EXTEND_ID extendId);
	VOID AllEquipmentRefineUpdate(long lVal);
	hash_map<CGUID,CGoods*,hash_guid_compare>* GetOriginPackGoodsMap(){return m_cOriginPack->GetGoodsMap();}
	hash_map<CGUID,CGoods*,hash_guid_compare>* GetSubPackGoodsMap(int i);
	map<CEquipmentContainer::EQUIPMENT_COLUMN,CGoods*>* GetEquipmentGoodsMap(){return m_cEquipment.GetGoodsMap();}
	//  [3/15/2010 chenxianj]
	//������Ʒ�ĸ��Ӿ���
	void SetGoodsExtraExp(long lExp);

	//! ��Ʒ������ʱ������
	//! ����λ�ã�GAP_TIME_LIMIT��һ��ֵ
	//! ���ã���ʶ��GAP_TIME_LIMIT�ĵڶ���ֵ����
	enum eGoodsTimeLimit
	{
		eGTL_Minute = 1,	//! �Է�Ϊ��λ��ʱ�Σ���Ʒ��ʹ�õ�ʱ��
		eGTL_Date = 2,		//! ����Ϊ��λ��ʱ�̣���Ʒ����ʧʱ��
	};

	//!								�ҵ��������󣨲�����Ǯ������ʯ��Ԫ�������ֵȣ�
	CGoodsContainer*				FindContainer(long lContainerId);
	//!								ͨ����Ʒ�������ڱ������ӱ������ҵ���һ��һ����Ʒ����Ϣ
	BOOL							FindFirstGoodsByIndex(LONG lGoodsIndex, tagGoodsParam &GoodsParam);

	//���ٶȷ����ı��Ժ󴥷�
	void OnSpeedChanged();
	//��Ӧλ�øı�
	void OnPosChanged();
	//! ��Ӧ��Ʒ���
	VOID OnGetGoods(DWORD dwGoodsId,CGoods* pGoods=NULL);
	//! ��Ӧ��Ʒʧȥ
	VOID OnLoseGoods(CGoods* pGoods);
	//! ��Ӧ�ֿ���Ʒ���
	VOID OnDepotGetGoods(CGoods* pGoods);
	//! ��Ӧ�ֿ���Ʒʧȥ
	VOID OnDepotLoseGoods(CGoods* pGoods);
	//! ɾ����Ʒ��֪ͨ���
	void DelGoodsAndSendMsg(long lContainerId, long lPos);
	//! ��������ϣ����б�����װ����λ���ֿ⣩�ҵ�һ����Ʒ��������������Ʒ��
	CGoods* FindGoodByGuid(const CGUID &GoodsGUID,bool NoTest=false);
	CGoods* FindGoodsAllPlaceRemove(const CGUID& guid);
	//! ��������б����������ӱ��������ҵ�һ����Ʒ��������������Ʒ��
	CGoods* FindGoodsByGuidInPack(const CGUID &GoodsGUID);
	BOOL FindGoodsByGuid(const CGUID& guid,DWORD& dwContainer,DWORD& dwPos);
	CGoods* RemoveGoods(long lContainerID,long lPos);
	//!								��Ʒ����ʱ�䴦��
	//!								��δ��ʱ���ö�ʱ��������ʱ������true��Ӧ���ٽӿ��ⲿɾ����Ʒ
	bool							TestGoodsTimeout(CGoods* pGoods);

	//!								ͨ����Ʒguid�ҵ���Ʒ����λ��
	//!								ReValue��pair<����ID����Ʒλ��>
	//!								�ҵ������棬���򷵻ؼ�
	bool							FindGoodsByGuid(const CGUID &GoodsGUID,  pair<long, long> &ReValue);
private:
	/// ��顯����������˥��
	bool CheckGuardianEnergy( CGoods *pGoods, bool bLogin );	
	/// ע�ᡮ�����������ݼ���ʱ��
	long RegisterGuardianEnergyDecTimer( const CGUID &goodsID );
	/// ��Ӧ��������������ʱ��
	void OnGuardianEnergyTimeOut( const CGUID &goodsID );
	//!								Ϊ��Ʒע��һ����ʱ���������ض�ʱ��ID
	long							RegisterLimitGoodsTimerID(const CGUID &GoodsGUID, ULONG dwFutureTime);
	//!								ȡ��һ����Ʒ��Timer
	void							CancelLimitGoodsTimer(const CGUID &GoodsGUID);
	//!								��Ӧ��ʱ����������Ʒɾ���¼�
	void							OnLimitGoodsTimeout(long lTimerID);
	//!								��ƷGUID�Ͷ�ʱ��ID��������
	map<CGUID, long>				m_GoodsTimerID;
	long							RegisterInvestTimer();

public:
	//!								ע�����ֺ͸���������װ��ʱ��
	void							RegisterChangeModelTimer();
	//!								Ϊ��װ����ע��һ����ʱ���������ض�ʱ��ID
	void							RegisterChangeGoodsTimerID(const CGUID &GoodsGUID, long lFutureTime);
	//!								ȡ��һ����װ�����Ķ�ʱ��
	void							CancelChangeTimer(const CGUID &GoodsGUID);
	//!								��Ӧ��װ�����Ķ�ʱ��
	void							OnChangeGoodsTimeOut(long lTimerID);
private:
	//!								��װ����GUID�Ͷ�ʱ��ID��������
	map<CGUID, long>				m_ChangeGoodsTimerID;

	//! �ֿ����
	////////////////////////////////////////////////////////////////////////////////////////////
public:
	PlayerDepot*				GetPlayerDepot(void);

	//!							��Ƭ��������
	CPlayerPwd					m_CardPwd;

private:
	//! �ֿ����
	PlayerDepot					m_pPlayerDepot;
	////////////////////////////////////////////////////////////////////////////////////////////

public:	
	//##װ��
	CEquipmentContainer			m_cEquipment;
	//##Ǯ��
	CWallet						m_cWallet;

	//����Ǯ��
	CSilverWallet				m_cSilverWallet;

	//! ��ʯ��
	GemContainer				m_cGem;

	DWORD						DeleteGoods( CS2CContainerObjectMove::PLAYER_EXTEND_ID pei, const CGUID& guid, DWORD dwAmount );
	DWORD						DeleteDepotGoods( DWORD dwContainerType, const CGUID& guid, DWORD dwAmount );
	DWORD						DeleteGoods(CS2CContainerObjectMove::PLAYER_EXTEND_ID pei,eGoodsBaseType baseType,DWORD dwWeaponType,DWORD dwNum);
	BOOL						DropGoods( CS2CContainerObjectMove::PLAYER_EXTEND_ID pei, const CGUID& guid );
	int							CheckGoods(const char *goodoriname);//�����Ʒ�Ƿ��и���Ʒ��������Ʒ�ĸ���
	BOOL						AddGoodsWithProtect(CGoods* pGoods,BOOL bProtect=TRUE, LONG *pOptRerult = NULL);	
	BOOL						AddAndReturnGoodsWithProtect(CGoods **ppGoods,BOOL bProtect=TRUE, LONG *pOptRerult = NULL);
	BOOL						EquipBackToPack();

	int							check_item_in_packet(const int item_idx);//�����Ʒ�Ƿ��и���Ʒ��������Ʒ�ĸ���

	int							remove_item_in_packet(const int item_idx, const int item_num);
	BOOL						DelGoodsAmountByOriginName( const char* goodsname, int goodsnum);

	//##������Ʒ��ص�
	VOID						DropParticularGoodsWhenDead();
	VOID						DropParticularGoodsWhenLost();
	VOID						DropParticularGoodsWhenRecall();

	//!							���ܼӳɿ�ʼ������һ��KEY��
	long						SkillReinforceBegin(bool isMagic, bool isAttack);
	//!							���ܼӳɽ��������Ч�����棩
	void						SkillReinforceEnd(long lKey);
	//!							��ü��ܼӳ�ֵ��first������ʱ��仯������second��Ч��ֵ�仯������
	const pair<float, float>&	GetSkillReinforceAmend(long lKey);
	//!							������Ʒ��ǰ������û�г��ܵ���������-1��
	long						GetArmCurrPower(void);
	//!							������Ʒ��ǰ���������ֵ�ı������ðٷ������ӱ�ʾ��
	long						GetArmCurrPowerSacle(void);

	//!							��������������õ�һ�����õļӳ�ֵ
	//!							eValuePos�����Ͷ�����GoodsExSetup::eAmendCoefficientType��
	float						GetReinforceByEnum(long eValuePos, BOOL isMagic);

	//!							�۳�һ�����ĵ�����������
	bool						ExpendPower(BOOL isMagic);

private:
	list<tagExpendableEffect> m_listExpendableEffect;	//���ĵ�����ʱ�������Ե����ñ�

	long							m_EffectIdSeed;
	//! ʹ�ü��ܼӳɱ�����first������ʱ��仯������second��Ч��ֵ�仯������
	map<long, pair<float, float>>	m_mapReturnReinforce;

	//�ѽ���ϵͳ�����ʼ��б�
	list<CGUID>		m_lReceiveMail;
	bool			m_bReceiveMail;
	map<long,long>	m_mapSysMail;

public:
	void				InsertSysMail(long lID,long lEndTime);//����ϵͳ�ʼ��б�
	void				CheckSysMailTime();					  //���ϵͳ�ʼ��б�
	bool				CheckSysMail(long lSysID);			  //�����û�н��չ�lSysIDϵͳ�ʼ�
	map<long,long>		&GetSysMailList()	  { return m_mapSysMail;}
	void				SetReceiveMail(bool b){ m_bReceiveMail = b; }
	bool				GetReceiveMail()	  { return m_bReceiveMail; }
	list<CGUID>			&GetReceiveMailList() { return m_lReceiveMail;}
	void				AddReceiveMailList(CGUID &MailGuid);
	void				DeleteReceiveMailList(CGUID &MailGuid);

	/////////////////////////////////////////////////////////////////////
	// ���Դ�ȡ�ӿ�
	/////////////////////////////////////////////////////////////////////
private:
	DWORD	m_dwMurdererTimeStamp;
	long	m_lPKTimerID;			//PK��ʱ��id
	long	m_lPvPTimerID;			//PVP��ʱ��id
	long	m_lHpMpTimerID;			//�Զ��ָ�Hp,Mp��ʱ��id
	long	m_lReliveTimerID;		//�����سǸ���Ķ�ʱ��ID

public:
	void	RegisterReliveTimerID();			//����ע��سǸ��ʱ��
	void	UnRegisterReliveTimerID();			//����ע���سǸ��ʱ��
public:
	////////////////////////////////////////////////////////////////////////////////
	//���û���ֵ����
	void    SetActiveRatio(float f)	{m_fActiveScale = f;}
	float	GetActiveRatio()		{return m_fActiveScale;}
	//�Ƿ��ֵ
	void	SetCharged(bool b=true)		{m_AppendProperty.bIsCharged = b;}
	bool	IsCharged()					{return m_AppendProperty.bIsCharged;}

	const char *GetAccount()			{return m_AppendProperty.strAccount;}
	void	SetAccount(char *l)			{strcpy_s(m_AppendProperty.strAccount, ACCOUNT_SIZE, l);}
	const char *GetTitle()				{return m_AppendProperty.strTitle;}
	void	SetTitle(char *l)			{strcpy_s(m_AppendProperty.strTitle, TITLE_SIZE, l);}
	//ͷ�� ���� ����
	BYTE	GetShowFashion()		{return m_AppendProperty.bShowFashion;}
	void	SetShowFashion(BYTE b)	{m_AppendProperty.bShowFashion = b;}

	////////////////////////////////////////////////////////////////////////////////
	// ����
	virtual void SetArea(CArea* p);

public:
	//�ȼ�
	WORD   GetLevel()					{return m_Property.wLevel;}
	void   SetLevel(WORD l, BOOL bScriptSet);

	//����
	eConst GetConst() const { return m_Property.byConst; }
	void SetConst( eConst c ) { m_Property.byConst = c; }

	//�õ�ְҵ�ȼ�
	BYTE	GetOccuLvl(eOccupation byOccu)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			return m_Property.byOccuLvl[byOccu];
		return 0;
	}
	void  SetOccuLvl(eOccupation byOccu,BYTE l, BOOL bScriptSet);

	//����ֵ
	LONG64  GetTotalExp(void)			
	{
		LONG64 llTmp = (LONG64)dwExpMutipleValue*m_Property.dwExpMultiple;
		LONG64 llRet = llTmp + m_Property.dwExp;
		return llRet;
	}
	DWORD	GetExp()					{return m_Property.dwExp;}
	void	SetExp(LONG64 l);
	DWORD	GetExpMultiple(void)		{return m_Property.dwExpMultiple;}
	void	SetExpMultiple(DWORD l)		{m_Property.dwExpMultiple = l;}

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
	//���;���ֵ
	DWORD	GetPresentExp()				{return m_Property.dwPresentExp;}
	void	SetPresentExp(DWORD l)		{m_Property.dwPresentExp=l;}

	//�������辭��ֵ
	DWORD	GetUpgradeExp()				{return m_Property.dwUpgradeExp;}
	void	SetUpgradeExp(DWORD l)		{m_Property.dwUpgradeExp = l;}
	//ͷ��ͼƬ
	long	GetHeadPic()				{return m_Property.lHairPic;}
	void	SetHeadPic(long l)			{m_Property.lHairPic = l;}
	//�沿ͼƬ
	long	GetFacePic()				{return m_Property.lFacePic;}
	void	SetFacePic(long l)			{m_Property.lFacePic = l;}
	//ְҵ	
	BYTE	GetOccupation()				{return m_Property.byOccu;}
	void	SetOccupation(eOccupation l)		{m_Property.byOccu = l;}
	//��ְҵ
	BYTE	GetDOccupation()			{return m_Property.byAssOccu;}
	void	SetDOccupation(eDeputyOccu l)	{m_Property.byAssOccu=l;}
	//�Ա�
	BYTE	GetSex()					{return m_Property.bySex;}
	void	SetSex(BYTE l)				{m_Property.bySex = l;}
	//��������
	BYTE	GetClass()					{return m_Property.byClass;}
	void	SetClass(BYTE l)				{m_Property.byClass = l;}
	//��żID
	DWORD	GetSpouseId()				{return m_Property.dwSpouseID;}
	void	SetSpouseId(DWORD l)		{m_Property.dwSpouseID = l;}
	//pk����
	DWORD	GetPkCount()				{return m_Property.dwPkCount;}
	void	SetPkCount(DWORD l)			{m_Property.dwPkCount = l;}
	//ɱ������
	DWORD	GetPkValue()				{return m_Property.dwPkValue;}
	void	SetPkValue(DWORD l)	;
	//HP
	DWORD	GetHP()						{return m_Property.dwHp;}
	void	SetHP(DWORD l);				
	//MP
	WORD	GetMP()						{return m_Property.wMp;}
	void	SetMP(WORD l);
	//�ȼ�
	DWORD	GetHotKey(long lpos){if(lpos < 0){lpos = 0;}else if(lpos > 23){lpos = 23;}return m_Property.dwHotKey[lpos];}
	void	SetHotKey(long lpos,DWORD l){if(lpos < 0){lpos = 0;}else if(lpos > 23){lpos = 23;}m_Property.dwHotKey[lpos] = l;}

	//PK����(b0:ȫ�屣��)
	BOOL	IsPk_Normal();
	void	SetPk_Normal(bool b);
	//b1:��ӱ���
	BOOL	IsPk_Team();
	void	SetPk_Team(bool b);
	//b2:���ᱣ��,
	BOOL	IsPk_Union();
	void	SetPk_Union(bool b);
	BOOL	IsPk_Badman();
	void	SetPk_Badman(bool b);
	//b4:��������
	BOOL	IsPk_Country();
	void	SetPk_Country(bool b);
	//b5:��Ӫ����
	BOOL	IsPk_Camp();
	void	SetPk_Camp(bool b);
	//b6.��������
	BOOL	IsPk_Pirate();
	void	SetPk_Pirate(bool b);
	//b7.������ʾ
	BOOL	IsPk_Hint();
	void	SetPk_Hint(bool b);
	//b8.�Զ�ѡ��
	BOOL	IsPk_AutoPlayer();
	void	SetPk_AutoPlayer(bool b);
	//b9.�Զ�ѡ��
	BOOL	IsPk_AutoMonster();
	void	SetPk_AutoMonster(bool b);



	//����ֵ
	WORD	GetEnergy()					{return m_Property.wEnergy;}
	void	SetEnergy(WORD l);
	//��λ�ȼ�
	BYTE	GetRankofNobility()			{return m_Property.byRankOfNobility;}
	void	SetRankofNobility(BYTE l)	{m_Property.byRankOfNobility = l;}
	//��ǰ����
	WORD	GetStamina()				{return m_Property.wStamina;}
	void	SetStamian(WORD l)			{m_Property.wStamina = l;}
	//��λ����ֵ
	DWORD   GetRankOfNobCredit()		{return m_Property.dwRankOfNobCredit;}
	void	SetRankOfNobCredit(DWORD l){m_Property.dwRankOfNobCredit = l;}
	//Ӷ���ȼ�
	BYTE	GetRankOfMercenary()		{return m_Property.byRankOfMercenary;}
	void	SetRankOfMercenary(DWORD l) {m_Property.byRankOfMercenary = l;}
	//Ӷ������ֵ
	DWORD	GetMercenaryCredit()		{return m_Property.dwMercenaryCredit;}
	void	SetMercenaryCredit(DWORD l){m_Property.dwMercenaryCredit = l;}
	//����ID
	BYTE	GetCountry()				{return m_Property.byCountry;}
	void	SetCountry(BYTE l)			{m_Property.byCountry = l;}
	//���ҹ���ֵ
	long	GetCountryContribute()		{return m_Property.lCountryContribute;}
	void	SetCountryContribute(long l);
	//��ż��ϵֵ
	DWORD	GetSpouseParam()			{return	m_Property.dwSpouseParam;}
	void	SetSpouseParam(DWORD l)		{m_Property.dwSpouseParam = l;}
	//��ҵ�ȼ�
	BYTE	GetBusinessLevel()			{return m_Property.byBusinessLevel;}
	void	SetBusinessLevel(BYTE l)	{m_Property.byBusinessLevel = l;}
	//��ҵ����
	DWORD	GetBusinessExp()			{return	m_Property.dwBusinessExp;}
	void	SetBusinessExp(DWORD l)		{m_Property.dwBusinessExp = l;}
	//��������
	DWORD   GetArtisanCredit()			{return m_Property.dwArtisanCredit;}
	void	SetArtisanCredit(DWORD l)			{m_Property.dwArtisanCredit = l;}
	//�����ȼ�
	WORD	GetArtisanLevel()			{return m_Property.byArtisanLevel;}
	void	SetArtisanLevel(WORD l)		{m_Property.byArtisanLevel = l;}
	//��������
	DWORD	GetArtisanExp()				{return	m_Property.dwArtisanExp;}
	void	SetArtisanExp(DWORD	l)		{m_Property.dwArtisanExp = l;}
	//ɱ�������
	DWORD	GetPVPCount()				{return m_Property.dwPVPCount;}
	void	SetPVPCount(DWORD l)		{m_Property.dwPVPCount = l;}
	//PVPֵ
	DWORD	GetPVPValue()				{return	m_Property.dwPVPValue;}
	void	SetPVPValue(DWORD l)		{m_Property.dwPVPValue = l;}

	DWORD	GetLastExitGameTime()		{return m_Property.dwLastExitGameTime;}
	void	SetLastExitGameTime(DWORD l){m_Property.dwLastExitGameTime=l;}

	///////////// PK CP  ////////////////////////////////////////
	long GetPKCP() const { return m_Property.lCurPKCP; }
	void SetPKCP( long v );
	long GetMaxPKCP();

	long GetBaseMaxPKCP() const { return m_BaseFightProperty.lMaxPKCP; }
	void SetBaseMaxPKCP( long v );

	/// ���������������װ��CP����ϵ��
	long GetAllEquipTimesCP();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ս������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���HP
	DWORD	GetMaxHP()
	{
		long lMaxHp =m_FightProperty.dwMaxHp+m_BaseFightProperty.dwMaxHp;
		if(lMaxHp<1)	return 1;
		return lMaxHp;
	}
	void	SetMaxHP(DWORD l);
	//���MP
	WORD	GetMaxMP()
	{
		long lMaxMp = m_FightProperty.wMaxMp+m_BaseFightProperty.wMaxMp;
		if(lMaxMp<1)	return 1;
		return lMaxMp;
	} 
	void	SetMaxMP(WORD l);
	//��������
	WORD	GetMaxStamina()							{return m_FightProperty.wMaxStamina;}
	void	SetMaxStamina(WORD l);
	//�������ֵ
	WORD	GetMaxEnergy()							{return m_FightProperty.wMaxEnergy;}
	void	SetMaxEnergy(WORD l);

	//��С������
	WORD	GetMinAtk()								{return m_FightProperty.wMinAtk;}
	void	SetMinAtk(WORD l);	
	//��󹥻���
	WORD	GetMaxAtk()								{return m_FightProperty.wMaxAtk;}
	void	SetMaxAtk(WORD l);
	//��������
	WORD	GetHit()								{return m_FightProperty.wHit;}
	void	SetHit(WORD l);		
	//�������
	WORD	GetDef()								{return m_FightProperty.wDef;}
	void	SetDef(WORD l);			
	//����
	WORD	GetDodge()								{return m_FightProperty.wDodge;}
	void	SetDodge(WORD l);		
	//Ԫ�ؿ���
	WORD	GetElementResistant(long lpos)			{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_FightProperty.wElemDef[lpos];}
	void	SetElementResistant(long lpos,WORD l);	

	//�����ָ��ٶ�
	WORD	GetHpRecoverSpeed()						{return m_FightProperty.wHpRecoverSpeed;}
	void	SetHpRecoverSpeed(WORD l);
	//�����ָ��ٶ�
	WORD	GetMpRecoverSpeed()						{return m_FightProperty.wMpRecoverSpeed;}
	void	SetMpRecoverSpeed(WORD l);
	//�����ָ��ٶ�
	WORD	GetStamRecoverSpeed()					{return	m_FightProperty.wStamRecoverSpeed;}
	void	SetStamRecoverSpeed(WORD l)	;
	//����
	WORD	GetStrenth()							{return m_FightProperty.wStrenth;}
	void	SetStrenth(WORD l)	;
	//����	
	WORD	GetAgility()							{return m_FightProperty.wAgility;}
	void	SetAgility(WORD l)	;
	//����
	WORD	GetConsti()								{return m_FightProperty.wConsti;}
	void	SetConsti(WORD l)	;
	//����
	WORD	GetIntell()								{return	m_FightProperty.wIntell;}
	void	SetIntell(WORD l)	;
	//�ǻ�
	WORD	GetWisdom()								{return	m_FightProperty.wWisdom;}
	void	SetWisdom(WORD l)	;
	//����
	WORD	GetSpirit()								{return	m_FightProperty.wSpirit;}
	void	SetSpirit(WORD l)	;
	//��
	WORD	GetFightBlock()							{return m_FightProperty.wFightBlock;}
	void	SetFightBlock(WORD l)	;
	//�м�
	WORD	GetParry()								{return m_FightProperty.wParry;}
	void	SetParry(WORD l)	;

	//����
	WORD	GetCharm()								{return	m_FightProperty.wCharm;}
	void	SetCharm(WORD l)	;
	//רע
	WORD	GetAbsorb()								{return	m_FightProperty.wAbsorb;}
	void	SetAbsorb(WORD l)	;
	//ħ������
	WORD	GetMdef()								{return m_FightProperty.wMdef;}
	void	SetMdef(WORD l)		;
	//������
	WORD	GetCri()								{return	m_FightProperty.wCri;}
	void	SetCri(WORD l)		;

	WORD	GetMAtkSpeed()							{return m_FightProperty.wMAtkSpeed;}
	void	SetMAtSpeed(WORD l)	;
	//Buff����ǿ��
	WORD	GetBuffPower()							{return m_FightProperty.wBuffPower;}
	void	SetBuffPower(WORD l)	;
	//ħ��������
	WORD	GetMAtk()								{return m_FightProperty.wMAtk;}
	void	SetMAtk(WORD l)			;
	//ħ��������
	WORD	GetMCri()								{return	m_FightProperty.wMCri;}
	void	SetMCri(WORD l)			;
	//����������
	WORD	GetBloodSuk()							{return m_FightProperty.wBloodSuk;}
	void	SetBloodSuk(WORD l)		;
	//����������
	WORD	GetManaSuck()							{return m_FightProperty.wManaSuck;}
	void	SetManaSuck(WORD l)		;
	//��־����
	WORD	GetWillDef()							{return	m_FightProperty.wWillDef;}
	void	SetWillDef(WORD l)		;
	//���ʿ���
	WORD	GetConstiDef()							{return m_FightProperty.wConstiDef;}
	void	SetConstiDef(WORD l)	;
	//Debuff����
	WORD	GetDeBuffDef(long lpos)					{if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1; return	m_FightProperty.wDeBuffDef[lpos]+m_BaseFightProperty.wDeBuffDef[lpos];}
	void	SetDefBuffDef(long lpos,WORD l);			
	//�Ը��������˺�
	WORD	GetClassDam(long lpos)					{if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1; return	m_FightProperty.wClassDam[lpos];}
	void	SetClassDam(long lpos,WORD l);			
	WORD	GetElemDam(long lpos)					{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return	m_FightProperty.wElemDam[lpos];}
	void	SetElemDam(long lpos,WORD l)	;
	//ս��ʱ�����ָ��ٶ�
	WORD	GetFightHPRecoverSpeed()				{return m_FightProperty.wFightHpRecoverSpeed;}
	void	SetFightHPRecoverSpeed(WORD l)	;
	//ս��ʱ�����ָ��ٶ�
	WORD	GetFightMPRecoverSpeed()				{return m_FightProperty.wFightMpRecoverSpeed;}
	void	SetFightMPRecoverSpeed(WORD l)	;
	//�����ָ��ٶ�
	short	GetEnterRecoverSpeed()					{return m_FightProperty.wEnerRecoverSpeed;}
	void    SetEnterRecoverSpeed(WORD l)	;
	//ս�������ָ��ٶ�
	short	GetFightEnterRecoverSpeed()				{return m_FightProperty.wFightEnerRecoverSpeed;}
	void	SetFightEnterRecoverSpeed(WORD l);
	//����������
	WORD	GetEnergySuck()							{return m_FightProperty.wEnergySuck;}
	void	SetEnergySuck(WORD l)			;
	//����������
	WORD	GetCriDef()								{return m_FightProperty.wCriDef;}
	void    SetCriDef(WORD l)				;
	//ħ����������
	WORD	GetMCriDef()							{return m_FightProperty.wMCriDef;}
	void	SetMCriDef(WORD l)				;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����ս������
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���HP
	DWORD	GetBaseMaxHP()								{return m_BaseFightProperty.dwMaxHp;}
	void	SetBaseMaxHP(DWORD l)						{m_BaseFightProperty.dwMaxHp = l; }
	//���MP
	WORD	GetBaseMaxMP()								{return m_BaseFightProperty.wMaxMp;} 
	void	SetBaseMaxMP(WORD l)						{m_BaseFightProperty.wMaxMp = l; }
	//��������
	WORD	GetBaseMaxStamina()							{return m_BaseFightProperty.wMaxStamina;}
	void	SetBaseMaxStamina(WORD l)					{m_BaseFightProperty.wMaxStamina = l;}
	//�������ֵ
	WORD	GetBaseMaxEnergy()							{return m_BaseFightProperty.wMaxEnergy;}
	void	SetBaseMaxEnergy(WORD l);				

	//��С������
	WORD	GetBaseMinAtk()								{return m_BaseFightProperty.wMinAtk;}
	void	SetBaseMinAtk(WORD l)						{m_BaseFightProperty.wMinAtk = l;}
	//��󹥻���
	WORD	GetBaseMaxAtk()								{return m_BaseFightProperty.wMaxAtk;}
	void	SetBaseMaxAtk(WORD l)						{m_BaseFightProperty.wMaxAtk = l;}
	//��������
	WORD	GetBaseHit()								{return m_BaseFightProperty.wHit;}
	void	SetBaseHit(WORD l)							{m_BaseFightProperty.wHit = l;}
	//�������
	WORD	GetBaseDef()								{return m_BaseFightProperty.wDef;}
	void	SetBaseDef(WORD l)							{m_BaseFightProperty.wDef = l;}
	//����
	WORD	GetBaseDodge()								{return m_BaseFightProperty.wDodge;}
	void	SetBaseDodge(WORD l)						{m_BaseFightProperty.wDodge = l;}
	//Ԫ�ؿ���
	WORD	GetBaseElementResistant(long lpos)			{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_BaseFightProperty.wElemDef[lpos];}
	void	SetBaseElementResistant(long lpos,WORD l)	{m_BaseFightProperty.wElemDef[lpos] = l;}
	//�����ָ��ٶ�
	WORD	GetBaseHpRecoverSpeed()						{return m_BaseFightProperty.wHpRecoverSpeed;}
	void	SetBaseHpRecoverSpeed(WORD l)				{m_BaseFightProperty.wHpRecoverSpeed = l;}
	//�����ָ��ٶ�
	WORD	GetBaseMpRecoverSpeed()						{return m_BaseFightProperty.wMpRecoverSpeed;}
	void	SetBaseMpRecoverSpeed(WORD l)				{m_BaseFightProperty.wMpRecoverSpeed = l;}
	//�����ָ��ٶ�
	WORD	GetBaseStamRecoverSpeed()					{return	m_BaseFightProperty.wStamRecoverSpeed;}
	void	SetBaseStamRecoverSpeed(WORD l)				{m_BaseFightProperty.wStamRecoverSpeed = l;}
	//����
	WORD	GetBaseStrenth()							{return m_BaseFightProperty.wStrenth;}
	void	SetBaseStrenth(WORD l)						{m_BaseFightProperty.wStrenth = l;}
	//����	
	WORD	GetBaseAgility()							{return m_BaseFightProperty.wAgility;}
	void	SetBaseAgility(WORD l)						{m_BaseFightProperty.wAgility = l;}
	//����
	WORD	GetBaseConsti()								{return m_BaseFightProperty.wConsti;}
	void	SetBaseConsti(WORD l)						{m_BaseFightProperty.wConsti = l;}
	//����
	WORD	GetBaseIntell()								{return	m_BaseFightProperty.wIntell;}
	void	SetBaseIntell(WORD l)						{m_BaseFightProperty.wIntell = l;}
	//�ǻ�
	WORD	GetBaseWisdom()								{return	m_BaseFightProperty.wWisdom;}
	void	SetBaseWisdom(WORD l)						{m_BaseFightProperty.wWisdom = l;}
	//����
	WORD	GetBaseSpirit()								{return	m_BaseFightProperty.wSpirit;}
	void	SetBaseSpirit(WORD l)						{m_BaseFightProperty.wSpirit = l;}
	//��
	WORD	GetBaseBlock()								{return m_BaseFightProperty.wFightBlock;}
	void	SetBaseBlock(WORD l)						{m_BaseFightProperty.wFightBlock = l;}
	//�м�
	WORD	GetBaseParry()								{return m_BaseFightProperty.wParry;}
	void	SetBaseParry(WORD l)						{m_BaseFightProperty.wParry = l;}
	//����
	WORD	GetBaseCharm()								{return	m_BaseFightProperty.wCharm;}
	void	SetBaseCharm(WORD l)						{m_BaseFightProperty.wCharm = l;}
	//רע
	WORD	GetBaseAbsorb()								{return	m_BaseFightProperty.wAbsorb;}
	void	SetBaseAbsorb(WORD l)						{m_BaseFightProperty.wAbsorb = l;}
	//ħ������
	WORD	GetBaseMdef()								{return m_BaseFightProperty.wMdef;}
	void	SetBaseMdef(WORD l)							{m_BaseFightProperty.wMdef = l;}
	//������
	WORD	GetBaseCri()								{return	m_BaseFightProperty.wCri;}
	void	SetBaseCri(WORD l)							{m_BaseFightProperty.wCri = l;}
	//ʩ���ٶ�
	WORD	GetBaseMAtkSpeed()							{return m_BaseFightProperty.wMAtkSpeed;}
	void	SetBaseMAtSpeed(WORD l)						{m_BaseFightProperty.wMAtkSpeed = l;}
	//Buff����ǿ��
	WORD	GetBaseBuffPower()							{return m_BaseFightProperty.wBuffPower;}
	void	SetBaseBuffPower(WORD l)					{m_BaseFightProperty.wBuffPower = l;}
	//ħ��������
	WORD	GetBaseMAtk()								{return m_BaseFightProperty.wMAtk;}
	void	SetBaseMAtk(WORD l)							{m_BaseFightProperty.wMAtk = l;}
	//ħ��������
	WORD	GetBaseMCri()								{return	m_BaseFightProperty.wMCri;}
	void	SetBaseMCri(WORD l)							{m_BaseFightProperty.wMCri = l;}
	//����������
	WORD	GetBaseBloodSuk()							{return m_BaseFightProperty.wBloodSuk;}
	void	SetBaseBloodSuk(WORD l)						{m_BaseFightProperty.wBloodSuk = l;}
	//����������
	WORD	GetBaseManaSuck()							{return m_BaseFightProperty.wManaSuck;}
	void	SetBaseManaSuck(WORD l)						{m_BaseFightProperty.wManaSuck = l;}
	//��־����
	WORD	GetBaseWillDef()							{return	m_BaseFightProperty.wWillDef;}
	void	SetBaseWillDef(WORD l)						{m_BaseFightProperty.wWillDef = l;}
	//���ʿ���
	WORD	GetBaseConstiDef()							{return m_BaseFightProperty.wConstiDef;}
	void	SetBaseConstiDef(WORD l)					{m_BaseFightProperty.wConstiDef = l;}
	//Debuff����
	WORD	GetBaseDeBuffDef(long lpos)					{if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1;  return	m_BaseFightProperty.wDeBuffDef[lpos];}
	void	SetBaseDefBuffDef(long lpos,WORD l)			{m_BaseFightProperty.wDeBuffDef[lpos] = l;}

	//�Ը��������˺�
	WORD	GetBaseClassDam(long lpos)					{if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1; if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return	m_BaseFightProperty.wClassDam[lpos];}
	void	SetBaseClassDam(long lpos,WORD l)			{m_BaseFightProperty.wClassDam[lpos] = l;}

	//�Ը���Ԫ���˺�
	WORD	GetBaseElemDam(long lpos)					{return	m_BaseFightProperty.wElemDam[lpos];}
	WORD	SetBaseElemDam(long lpos,WORD l)		    {m_BaseFightProperty.wElemDam[lpos] = l;}

	//ս��ʱ�����ָ��ٶ�
	WORD	GetBaseFightHPRecoverSpeed()				{return m_BaseFightProperty.wFightHpRecoverSpeed;}
	void	SetBaseFightHPRecoverSpeed(WORD l)			{m_BaseFightProperty.wFightHpRecoverSpeed = l;}

	//ս��ʱ�����ָ��ٶ�
	WORD	GetBaseFightMPRecoverSpeed()				{return m_BaseFightProperty.wFightMpRecoverSpeed;}
	void	SetBaseFightMPRecoverSpeed(WORD l)			{m_BaseFightProperty.wFightMpRecoverSpeed = l;}

	//�����ָ��ٶ�
	short	GetBaseEnterRecoverSpeed()					{return m_BaseFightProperty.wEnerRecoverSpeed;}
	void    SetBaseEnterRecoverSpeed(WORD l)			{m_BaseFightProperty.wEnerRecoverSpeed = (short)l;}

	//ս�������ָ��ٶ�
	short	GetBaseFightEnterRecoverSpeed()				{return m_BaseFightProperty.wFightEnerRecoverSpeed;}
	void	SetBaseFightEnterRecoverSpeed(WORD l)		{m_BaseFightProperty.wFightEnerRecoverSpeed = (short)l;}

	//����������
	WORD	GetBaseEnergySuck()							{return m_BaseFightProperty.wEnergySuck;}
	void	SetBaseEnergySuck(WORD l)					{m_BaseFightProperty.wEnergySuck = l;}

	//����������
	WORD	GetBaseCriDef()								{return m_BaseFightProperty.wCriDef;}
	void    SetBaseCriDef(WORD l)						{m_BaseFightProperty.wCriDef = l;}

	//ħ����������
	WORD	GetBaseMCriDef()							{return m_BaseFightProperty.wMCriDef;}
	void	SetBaseMCriDef(WORD l)						{m_BaseFightProperty.wMCriDef = l;};

	//! ----------------------ԭ������----------------------
	//!		�õ�Ԫ������
	DWORD	GetElementProperty(DWORD dwElementType, DWORD dwAttackType);
	//!		����Ԫ�����ԣ��ɹ����ص�ǰ�������ԣ�ʧ�ܷ���0
	DWORD	SetElementProperty(DWORD dwElementType, DWORD dwAttackType, long lNewValue);
	//!		���Ԫ�����ԣ��ɹ����ص�ǰ�������ԣ�ʧ�ܷ���0
	DWORD	AddElementProperty(DWORD dwElementType, DWORD dwAttackType, long lAddValue);


	float	GetExpScale()			{return m_fExpScale;}
	void	SetExpScale(float f)	{m_fExpScale=f;}
	float	GetOccuExpScale()		{return m_fOccuExpScale;}
	void	SetOccuExpScale(float f){m_fOccuExpScale=f;}
	float	GetSpScale()			{return m_fSpScale;}
	void	SetSpScale(float f)		{m_fSpScale=f;}
	float	GetPresentExpScale()			{return m_fPresentExpScale;}
	void	SetPresentExpScale(float f)		{m_fPresentExpScale=f;}
	//����
	void	AddStr()					{m_FightProperty.wStrenth++;}
	//����
	void	AddDex()					{m_FightProperty.wAgility++;}
	//����
	void	AddCon()					{m_FightProperty.wConsti++;}

	void	IncExp(LONG64 llExp, DWORD dwAddType);		//��������Ӿ���
	//!		��Ӿ�����־
	void	AddIncExpLog(DWORD dwOldExp, DWORD dwOldExpMultiple, DWORD dwAddExp, DWORD dwAddExpMultiple, DWORD dwAddType, const char *pSrcName);
	void	IncOccuSp(eOccupation eOccu,long lSp,long lType);		//��������Ӽ��ܵ���
	void	DecOccuSp(eOccupation eOccu,long lSp);		//�۳�SP
	//��ȡ���;���
	DWORD	AcquirePresentExp(DWORD dwMaxAcqExp);

	//����ĳ��ְҵ��ְҵ����
	void	IncOccuExp(eOccupation byOccu,long lExp, DWORD dwAddType);//���������ְ������

	//!		���ְҵ������־
	void	AddIncOccuExpLog(BYTE byOccu,DWORD dwOldExp, DWORD dwAddExp, DWORD dwAddType, const char *pSrcName);

	DWORD	GetMoney();
	//!		�õ���װ�����Ǯ��
	DWORD	GetMaxMoney(void);
	bool	SetMoney(DWORD l,int nFlag);

	DWORD   GetSilver();
	DWORD   GetMaxSilver();
	bool    SetSilver(DWORD l);

	DWORD	GetGem(void);
	bool	SetGem(DWORD lNum);

	DWORD   GetYuanbao(){return m_dwYuanbao;}
	VOID	SetYuanbao(DWORD dwVal);	

	////////////////////! �ж���//////////////////
	//!		�õ���һ�����
	long	GetActive(void);
	//!		������һ�����
	void	ResetActive(DWORD dwCurrTime);
	//!		�����չ������
	bool	AddExActive(long lAddNum);
	//!		�۳�������
	bool	TakeActive(long lTakeNum);
	//! ÿ��0���Ժ���ҵ�¼��Ϸʱ�������CP
	void    ResetActive(void);

	////////////////////! ���//////////////////
	//!				�õ���ҽ�鲽��
	long			GetMarriageStep(void);
	//!				������ҽ�鲽��
	long			SetMarriageStep(long lStep);

	//!				���ý�����
	bool			SetSpouse(CPlayer *pPlayer);
	//!				�õ�����ID
	const CGUID&	GetSpouseGuid(void);
	//!				�õ���������
	const char*		GetSpouseName(void);

	//////////////////////////////////////////////////////

	//##ɱ�����Ĺ���.
	VOID	OnDecreaseMurdererSign		();
	VOID	OnUpdateMurdererSign		();
	VOID	OnClearMurdererSign			();

	void	OnDecreasePKValue			();//��pkֵ
	void	OnDecreasePvPValue			();//��pvpֵ
	void	OnIncreaseHpMpValue			();//�Զ�����hp,mp				
	void	RegisterPKValue				();//pkֵע�ᶨʱ��
	void	RegisterPvPValue			();//pvpֵע�ᶨʱ��
	void	RegisterAutoReNewHpMp		();//�Զ��ָ�Hp,Mp��ʱ��
	void	UnRegisterPKValue			();//pkֵע���ᶨʱ��
	void	UnRegisterPvPValue			();//pvpֵע����ʱ��
	void	UnRegisterAutoReNewHpMp		();//ע���Զ��ָ�Hp,Mp��ʱ��

	void	UpdateHpMpRecoverSpeed		();//����HPMP�ָ��ٶ�
	//  [1/20/2009 chenxianj]
	void    ChangeRgnToIsland			();//����������
	bool	ChangeRgnToVillage			();//����������
	void	PostCountryInfo				(const char *strName);//���͹��ҹ���



	DWORD	GetNextExp()				{return m_Property.dwUpgradeExp;}
	DWORD	GetNextExpMultiple(void)	{return m_Property.dwUpgradeExpMultiple;}
	LONG64	GetNextTotalExp(void)		
	{
		LONG64 llTmp = (LONG64)dwExpMutipleValue*m_Property.dwUpgradeExpMultiple;
		LONG64 llRet = llTmp+m_Property.dwUpgradeExp;
		return llRet;
	}

	long	CheckLevel();				// ���ȼ��仯

	DWORD	GetNextOccuExp()			{return m_Property.dwCurOccuUpgradeExp;}
	long	CheckOccuLevel(eOccupation byOccu);	// ���ְҵ�ȼ��仯

	VOID    MountAllEquip();
	/// 9.28.2009 ����bGuardianFlagר����װ���������ػ�ʯ������Ʒ�������־
	VOID	MountEquipEx(CGoods *pGoods,bool bMount, bool bGuardianFlag = false );
	void	UseItem(WORD wPlace,BYTE lPos,long lTargetType,CGUID& lTargetGuid);	//ʹ�������Ե��ߵ��߼�
	void	UseItem(DWORD dwContainerId,DWORD dwPos);
	BOOL	AutoUseItem(WORD wPlace,BYTE lPos,long lTargetType,CGUID& lTargetGuid);	//! �Զ�ʹ�õ���
	long	CanMountEquip(CGoods *pGoods); //�Ƿ���װ��һ������
	long    CanMountEquip(CGoods *pGoods,DWORD dwPos,BOOL bMsgNotify=FALSE);
	BOOL	CanMountSubpack(CGoods* pGoods);
	long	CanUseItem(CGoods* pGoods);	//�Ƿ���ʹ��һ�������Ե���

	VOID    SendEquipNotifyMsg(long lVal);


	CGoods* GetGoodsById(const CGUID& guid);				//����ID�õ���Ʒ
	CGoods* GetGoodsById_FromPackage(const CGUID& guid);	//����ID�õ�������Ʒ
private:
	map<string, tagPty>		m_mapNameValue;		// �������ֶ�Ӧ�ı�����ַ

	void InitNameValueMap();	// ��ʼ��

	map<DWORD,tagGoodsCommonCooldown>  m_mapGoodsIdCoolDown;
	map<DWORD,tagGoodsCommonCooldown>  m_mapClassIdCoolDown;

public:
	virtual long	GetValue(const char* strName);
	virtual long	SetValue(const char* strName, long dwValue);

	//�����Է����ı�ʱ�����
	virtual void OnPropertyValueChanged(const char* strName, LONG lValue);
	static void InitUpdateDirtyAttrData();

	///////////////////////////////////////////////////////////////////
	// ��ҵĽű�����
	///////////////////////////////////////////////////////////////////
public:
	virtual CVariableList* GetVariableList()	{ return m_pVariableList; }
private:
	// ��ҵ�˽�нű������б�
	CVariableList* m_pVariableList;
	//////////////////////////////////////////////////////////////////////////
	//	�ӿ�
	//////////////////////////////////////////////////////////////////////////

public:
	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);	// ����
	bool CodeOrgSysToDataBlock(DBWriteSet& setWriteDB, bool bExData);
	bool DecodeOrgSysFromDataBlock(DBReadSet& setReadDB,bool bExData);
	bool CodeToDataBlock_ForClient(DBWriteSet& setWriteDB, bool bExData = true);
	void CodeRecruitToDataBlock_ForClient(DBWriteSet& setWriteDB);
	BOOL CodeDOccuToDataBlock(DBWriteSet& setWriteDB){return m_DOccuRecord.CodeToDataBlock(setWriteDB);}
	VOID CodeGoodsCooldown(DBWriteSet& setWriteDB,bool bExData=true);

	// ��WS������ȼ��ӿ�
	bool CodeHotKeyData(DBWriteSet& setWriteDB,bool bExData = true);
	bool DecodeHotKeyData(DBReadSet& setReadDB,bool bExData = true);

	// ��ȡ��ǰ����ʹ�õĹ����ȼ�ӳ���
	MAPFKEY& GetCurMapKey() { return m_FuncKey; }

	//ս�����ߵ�ʱ���
	DWORD												m_dwLostTimeStamp;
	void PetsChangeRegionLocal(INIT_TYPE lInitType);
	bool ChangeRegion(eRgnType type, const CGUID& lRegionID, long x, long y, long dir, long templateID=0, long lUseGoods=0, long lRange=0, long lCarDistance=0);

	//! ��������Ӱ���ƶ����߼�״̬�����̯�����ף�
	void EndAllStateWithMove(void);

	//�ƶ�ʱ����
	VOID OnMove(float fNewX, float fNewY);
	// ���󸴻�(0:��ȫ�س� 1:ԭ�ظ���)
	virtual void OnRelive(long lType,long lRecoverRatio=50);

	//����ҵȼ������仯��ʱ�򴥷�����Ҫָ��������
	void OnLevelChanged();
	//�����ְҵ�ȼ��������仯��ʱ����Ҫָ��ְҵ�ȼ���ʱ��
	void OnOccuLevelChanged();

	//##����ĳ�������󣬳������Զ������������֪ͨ������볡���ɹ���
	//##�����ٻ���Ҳ���������.
	virtual VOID OnEnterRegion(bool bIsRelive=false);
	virtual VOID OnExitRegion();

	//������GS����,������һ�ν���GS�������Ժ���л�������
	bool OnEnter(void);
	//��������Ϸʱ����,��һ�ε�½��ʱ�򴥷����Ժ��л����������ᴥ��
	void OnEnterGame(void);

	//���ѡ���˳���Ϸʱ�˳�GS�����������л������������˳�GSҲ����
	void OnExit ( BOOL bOnChangeServer = FALSE );
	//���˳���Ϸʱ����,�л���������ʱ��ᴥ��
	void OnExitGame(BOOL bOnChangeServer = FALSE);

	// Fox@20081126 ����
public:
	C_PETS GetPets(void) { return m_Pets; }
	long GetPetsNum(void) { return m_Pets.size(); }
	C_ACT_PETS GetActivePets(void) { return m_ActivePets; }
	CPet* CreatePet(const CGUID& rID);
	CPet* GetPet(const CGUID& rID);
	CPet* GetPet(long lID);
	void AddPet(CPet* pPet);
	void DelPet(const CGUID& rID);
	void ReleaseAllPets(void);
	void ActivateAllPets(bool bActive = true);
	void SetFocusPet(long lType);
	const CGUID& GetFocusPet(void);
	CPet* GetActivePet(long lType);
	bool AddActivePet(CPet* pPet);
	void DelActivePet(long lType);
	void CheckPetsMove(void);
	void ChangePetsState(long lState);

private:
	void DelPet(ITR_PET& itr);					// ���ɾ������
	void AddPetsExp(long lDelta, long lType);	// �������ﾭ��
	void AddPetsLev(void);						// ��������ȼ�
	void AddActivePet(long lType, const CGUID& rID);

private:
	PETS					m_Pets;				// ���г����б�
	ACT_PETS				m_ActivePets;		// ��������б�
	CMonster*				m_pTameTarget;		// ѱ����Ŀ��(��Ҫ���Ѳ��)
	long					m_lFocusPet;		// �������

protected:
	bool m_bInChangingRegion;			// �Ƿ����л�����	

	/////////////////////////////////////////////////////////////////////
	// ��Ϣ��Ӧ
	/////////////////////////////////////////////////////////////////////
public:
	virtual void OnMessage(CMessage*);
	long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);


private:
	long m_lAICount;			// AIʱ��
	//�ƶ���֤���� 
	DWORD	m_dwMoveValiStartTime;//�ƶ���֤��ʼ�ļ�ʱ
	DWORD	m_dwLastRecvMsgTime;  //�ϴν��ܵ��ƶ���Ϣ��ʱ��
	float	m_fMaxPeriodMoveSpeed;	//��һ��ʱ�����,ͳ�Ƴ�������ƶ��ٶȡ�
	CGUID	m_guMoveValiStartRgnID;//��ʼ��֤�ĳ���ID
	long	m_lMoveValiStartPosX;//��ʼ�ƶ���Xλ��
	long	m_lMoveValiStartPosY;//��ʼ�ƶ���Yλ��
	float	m_fMoveValiStartSpeed;//�ƶ���֤��ʼ�ٶ�
	float	m_fMoveValiTotalDistance;//�ƶ����ܾ���
	float	m_fMoveValiRecvMsgCount;//�յ����ƶ���Ϣ����
public:
	//���һ���ƶ���Ϣ
	void AddMoveMsg(float fNewX,float fNewY);
	//��֤ÿһ�����ƶ�
	bool CheckMoveStep(float fCurX,float fCurY);
	//�ƶ��ٶ���֤
	bool CheckMoveSpeedValidate();
	//�����������
	void ResetMoveValiData();
	// ������������
	void	OnQuestMoveStep	(float fCurX,float fCurY,float fDestX,float fDestY);
	void	OnQuestStopMove	(float fCurX,float fCurY,short lDir);
	virtual bool	CanMove				();
	virtual VOID	OnCannotMove		();
	virtual void EndBuff(DWORD dwStateID);//����ID����״̬

	VOID SetPosition( LONG lX, LONG lY );

	VOID SetSlipPos( LONG lX, LONG lY ,float fSpeed, float fDir);

	//##�л���ļ�⡣
	BOOL			CheckSwitchPoint();

	/////////////////////////////////////////////////////////////////////
	// ����HP/MP/MA
	/////////////////////////////////////////////////////////////////////
private:
	//##���һ�μ�Ѫ��ʱ���.
	DWORD								m_dwLastRestoreHpTimeStamp;
	//##���һ�μ�MP��ʱ���.
	DWORD								m_dwLastRestoreMpTimeStamp;

public:
	BOOL			RestoreHp			( DWORD dwHp, DWORD dwPersistTime, DWORD dwFrequency );
	BOOL			RestoreMp			( DWORD dwMp, DWORD dwPersistTime, DWORD dwFrequency );

	/////////////////////////////////////////////////////////////////////
	// ��ʱPING
	/////////////////////////////////////////////////////////////////////
protected:
	DWORD m_dwLastPingTime;		// �ϴη���ping��ʱ��
public:
	DWORD GetLastPingTime()			{return m_dwLastPingTime;}
	void SetLastPingTime(DWORD l)	{m_dwLastPingTime=l;}

	//-------------------------------------------------------------------
	// ���Դ�����߼�:
	// ÿ�η���������,�����û���½��ʱ��,���ȴ����ݿ��м��ؽ���ʱ��,���
	// û�б�����,��ô������m_lSilenceTime��m_lSilenceTimeStampΪ0,����
	// ������m_lSilenceTimeStampΪ��½ʱ��,m_lSilenceTimeΪ���ݿ�Ľ���
	// ʱ��.
	// ���û������ڼ���ԣ���ֱ�����ú�m_lSilenceTimeStamp ��
	// m_lSilenceTime ������������
	// ע��: ���л���������ʱ��һ��Ҫ���ʣ��Ľ���ʱ��.
	//--------------------------------
	//-----------------------------------
private:
	long m_lSilenceTime;			// ���Ե�ʱ�䣨��λ�����ӣ�
	long m_lSilenceTimeStamp;		// ���Ե�ʱ�������λ�����ӣ�
	bool m_bGMInvincible;			// ��ʾGM�Ƿ��޵�
	bool m_bGMConcealment;			// GM�Ƿ�����
	bool m_bNetTransferStatus;		//������ҵ����紫��״̬false:��ʾ����,1:��ʾӵ��

public:
	/**## ���ý��Ե�ʱ�䣨��λ�����ӣ�*/
	void					SetSilence			(long lTime);

	/**## �ж��Ƿ�Ϊ����ʱ����. */
	bool					IsInSilence			();

	/**## �Դ˺�����ֱ�ӷ���m_lSilenceTime���������߼���������ɣ�       */
	/**## �û��ڽ����ڼ��˳��Ļ�����ô��ϵͳOnLost��Ϣ��Ӧ���ڼ䣬        */
	/**## ֱ�Ӽ��� m_lSilenceTime Ϊʣ��Ľ���ʱ�䣬m_lSilenceTimeStamp */
	/**## ����Ϊ�û�OnLost��ʱ��.                                         */ 
	/**## �˺���ֻ���Ϊ���ݿ�洢�ӿ�ʹ��                                */
	long					GetSilence			()			{ return m_lSilenceTime; }

	bool GetIsGMInvincible()	{return m_bGMInvincible;}
	bool GetIsGMConcealment()	{return m_bGMConcealment;}

	void SetNetTransferStatus(bool b)	{m_bNetTransferStatus = b;}
	bool GetNetTransferStatus()			{return m_bNetTransferStatus;}

	//////////////////////////////////////////////////////////////////////////
	//	ս��
	//////////////////////////////////////////////////////////////////////////
public:
	bool	IsVisible(CMoveShape* pTarget);
	virtual bool IsAttackAble(CMoveShape* pAttackShape);	// �ɷ񹥻�
	long				OnBeenAttacked			( CMoveShape* pAttacker,long lHp,long lDamageResult=0);
	void	OnBeenKilled(CMoveShape* pAttacker);

	virtual long GetAckRangeMin()					{return 1;}
	virtual long GetAckRangeMax()					{return 1;}

	//##���׵����
	VOID					OnWeaponDamaged		();
	//##���ߵ����
	VOID					OnArmorDamaged		();
	//##������ĵ�ͨ�ú���
	VOID					OnEquipmentWaste	( CEquipmentContainer::EQUIPMENT_COLUMN ec, CGoods* pEquipment );

	virtual void			OnDied				();								// ��������
	//##���������AI����
	virtual VOID			OnBeenHurted		( LONG lAttackerType, const CGUID& AttackerID );
	virtual VOID			OnBeenMurdered		( LONG lMurdererType, const CGUID& MurdererGuid );
	VOID					ReportMurderer		( const CGUID& MurdererID );
	bool					IsBadman			();

	//##һЩս����ص�״̬����
	virtual VOID			EnterCriminalState	();
	virtual VOID			EnterResidentState	();

	//##���ս��/��ƽ,�ﷸ/������ҵ�״̬.
	virtual VOID			UpdateCurrentState	();

	// ͨ����ұ�������Ϣ
	VOID		SendNotifyWarningMsg(bool isKilled, CPlayer* pAttacker);

protected:
	long					m_lFightStateCount;				// ����״̬������(�ݼ�����0��ʾ��������״̬)
	DWORD					m_dwSinStateTimeStamp;

	DWORD					m_dwContinuousKillAmount;		// ��ǰ����������
	DWORD					m_dwContinuousKillTimeStamp;	// ��һ�λ��е�ʱ��

	long					m_lFightStateTimerID;//ս��״̬��ʱ��
public:

	//##����������
	VOID					IncreaseContinuousKill();

	virtual FLOAT			GetWeaponModifier( LONG lTargetLevel );
	//////////////////////////////////////////////////////////////////////////
	// ���
	//////////////////////////////////////////////////////////////////////////

public:
	const CGUID& GetTeamID() { return m_TeamID; }
	void SetTeamID(const CGUID& guid) { m_TeamID = guid;}
	void SetCaptain(bool b) { m_bCaptain = b; }
	bool GetCaptain(void) { return m_bCaptain; }

	virtual	void	OnAddBuff(DWORD dwStateID, DWORD dwBuffLevel);
	virtual	void	OnDelBuff(DWORD dwStateID, DWORD dwBuffLevel);

	void SetRecruitState(bool bIsRecruiting, char* pRecruitName, char* pPassword);
	tagRecruitState& GetRecruitState(void);

	long GetSentaiPoints(void)				{return m_SentaiPoints;}
	void SetSentaiPoints(long Points)		{m_SentaiPoints = Points;}


private:
	CGUID m_TeamID;
	bool m_bCaptain;
	tagRecruitState m_RecruitState;

	//! ս�ӻ���
	long m_SentaiPoints;
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////

public:
	typedef map<long,tagSkill>::iterator itSkill;
	//�жϼ�����ȴʱ���Ƿ񵽴�
	bool CooldownPast(long lSkillID);
	//�ж���Ʒ��ȴ
	bool GoodsCoolDown(long lGoodsIndex);
	//ʹ����Ʒ�ɹ�
	void SendUseItemSucceed(long lGoodsIndex);

	bool CheckStudySkill(DWORD dwID,DWORD &dwSkillLv);//ѧϰ�¼����ж�
	void StudySkill(DWORD dwID,DWORD dwSkillLv);//ѧϰ�¼���


	//ְҵ�ȼ��ж�
	bool OccuLvJudgeStudy(DWORD dwID,DWORD dwLv);
	//ְҵѧϰ����
	bool OccuJudgeStudy(DWORD dwID,DWORD dwLv);
	//�ȼ�ѧϰ����	
	bool LevelJudgeStudy(DWORD dwID,DWORD dwLv);
	//SPѧϰ����		
	int SPJudgeStudy(DWORD dwID,DWORD dwLv);
    //Expѧϰ����
    LONG64 ExpJudgeStudy(DWORD dwID, DWORD dwLv);
	//ǰ�ü���ѧϰ����	
	bool SkillJudgeStudy(DWORD dwID,DWORD dwLv);
	//ǰ�����Ƽ���ѧϰ����
	bool LimitSkillJudgeStudy(DWORD dwID,DWORD dwLv);
	//��Ʒ����
	bool GoodJudgeStudy(DWORD dwID,DWORD dwLv);
	//����ѧϰʧ��
	void StudySkillFailed(long lValue);
	//ѧϰ���ܴ����ű�
	void StudyKillScript(DWORD dwID,DWORD dwLv);
	//��Ӽ����ж�
	bool CanAddSkill(DWORD dwID, DWORD dwLv);

	//���ؼ����б�
	void SendBackSkill(string strName,CGUID guidNpc=NULL_GUID);
	//ְҵ�ж�
	bool CareerJudge(string strName);
	//������������
	void StartUpPassiveSkill();

	map<long,tagSkill>& GetSkillMap(void) { return m_Skills; }
	virtual void SetCommonCooldown(long lID, DWORD dwTime);
	virtual void SetSkillCooldown(long lID,DWORD dwTime);
	virtual void AddSkill(long id,long lv,long lCoolDown=0,time_t tEndTime=0);


	LONG CheckSkill(long id);

	virtual VOID OnChangeStates();								// �ı�״̬�󣬷�����Ϣ�����
	LONG GetNumSkills();										// ��ȡ���ܵĸ���
	LONG GetSkillLv(long lSkillID);

	VOID CodeSkillsToDataBlock_ForClient(DBWriteSet& setWriteDB, BOOL bAddRestoreTime = FALSE );	// �����еļ�����ӵ�����	
	VOID DecodeSkillsFromDataBlock(DBReadSet& setReadDB);	// �������еļ������ݽ���

	virtual long AddActiveSkill(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget=NULL,long lGoodsIndex=0,long lContainerID=0,long lPos=0,CGUID GoodsGuid=NULL_GUID);
	virtual BOOL DelSkill( long lSkillID ,long lSkillLv);
	void DeleteGoodsInPacket(char* strGoodName);
private:

	//������еļ���
	map<long,tagSkill> m_Skills;
public:
	struct tagOwnedReg
	{
		long lRegionID;						//������ID
		WORD wType;							//����������
	};
	//���ϵͳ
private:
	CGUID					m_FactionID;							//! �Լ����ID
	CGUID					m_UnionID;								//! �Լ����ID
	const tagFacMemInfo*	m_pFacMemInfo;							//! �Լ��Ĺ�����Ϣ	
	char					m_szFactionName[MAXSIZE_FactionName];	//! �������
	char					m_szUnionName[MAXSIZE_FactionName];		//! ��������
	LONG					m_lFacDemiseWarning;					//! ��ᴫλ���ѵ���ʱ
	LONG					m_lFacDisbandWarning;					//! ����ɢ���ѵ���ʱ

public:


	const CGUID&			GetFactionID(void)								{return m_FactionID;}
	void					SetFactionID(const CGUID& FactionID);
	void					SetFactionInfo(GameFaction *pFaction);
	const CGUID&			GetUnionID(void)								{return m_UnionID;}
	void					SetUnionID(const CGUID& UnionID);
	const char*				GetFactionName()								{ return m_szFactionName; }
	const char*				GetUnionName()									{ return m_szUnionName; }
	const tagFacMemInfo*	GetFacMemInfo(void)								{return m_pFacMemInfo;}
	void					SetFacMemInfo(const tagFacMemInfo* pFacMemInfo)	{m_pFacMemInfo = pFacMemInfo;}
	//��Ʒ��������
	bool	ItemBeginSkill(DWORD dwID, DWORD dwLevel, CMoveShape *pDestShape);
	int     JudgeSkillBegining(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget=NULL,long lGoodsIndex=0);//��������ǰ���ж�


private:
	CRoleQueue m_RoleQueue;
	//ٸ������
	CMercenaryQuestSys	m_MerQuestSys;
	bool       m_bLogin;
public:
	bool CodeQuestDataToDataBlock(DBWriteSet& setWriteDB);		//����������ݵ�ByteArray
	bool CodeQuestDataToDataBlock_ForClient(DBWriteSet& setWriteDB);//����������ݵ�ByteArray
	bool DecodeQuestDataFromDataBlock(DBReadSet& setReadDB);

	eQuestState GetQuestState(LONG wQuestID);					//�õ�һ�������״̬
	long GetValidQuestNum();									//�õ���Ч����������
	BOOL AddQuest(LONG lQuestID);								//����������һ������
	BOOL AddQuest(CRoleRecord*& p);								//���һ������

	bool CompleteQuest(DWORD dwQuestID);						//��������һ������
	void RemoveQuest(LONG lQuestID);							//�Ƴ�һ������
	void UpdateQuestStep(DWORD dwQuestID,long lStep);			//����һ���������Ч��ʾ����
	void UpdataQuestData(DWORD dwQuestID,long lParamIndex,long lVal);
	void UpdataQuestParamData(DWORD dwQuestID,long lParamIndex, long lParam,long RegionID,long RegionX, long RegionY);
	void ShowTalkBox(DWORD wQuestID,long lTextStep);
	void PlayerRunScript(char *strScriptName);					//�����������һ���ű�
	long GetCurrentMasterRoleID();								//ȡ�õ�ǰ��������id

	//�ñ�����//start
	bool CodeMerQuestDataToDataBlock(DBWriteSet& setWriteDB);	//���ٸ���������ݵ�ByteArray
	bool DecodeMerQuestDataFromDataBlock(DBReadSet& setReadDB);//��ٸ����������

	//��ҽ���GS�Ժ��ʼ�����ٸ����������
	void IniMercQuest();

	//�Ƿ���ˢ��
	enum FreshType{FT_NO=0 , FT_YES , FT_NEEDGOODS};
	long CanFreshMercQuest(long lMercListID);
	////ˢ��
	void FreshMercQuest(long lMercListID,bool usegoods=false);

	//�ж��Ƿ���ٸ���б�����
	bool IsMercListQuest(long lQuestID);
	//�ж��Ƿ��Ƿ��������б��е�����
	bool IsDisbandQuest(long lQuestID);
	//����˳�GSʱ,�ͷ����ٸ������
	//���ĳ���б�ID��ٸ������
	void AcquireMercQuestList(long lMercListID,bool usegoods=false);
	bool IsMerQuestInit(long lMercListID);
	//����ٸ���������ݵ��ͻ���
	void SendMercQuestDataToClient(long lMercListID);
	//��ٸ���������
	void OpenMercQuestInterface(long lMercListID);
	//����ٸ������ˢ��ʱ��
	void SetMercQuestTime(long lMercListID);
	DWORD GetMercQuestTime(long lMercListID);
	//ٸ������
	long GetMercQuest(){return m_MerQuestSys.GetCurQuestID();}
	void SetMercQuest(long id,bool bcall=true);
	//�ͽ�����
	void OpenShareQuestInterface();
	long GetTeamCurrQuest();
	void SetTeamCurrQuest(long lQuestID);
	void DeleteTeamQuest(long lQuestID);
	bool UpdateTeamQuestList(long lQuestID);
	void OnCompleteQuest(long lQuestID);
	bool IsCompleteQuest(long lQuestID);
	DWORD GetQuestOdds(long lQuestID);
	long CanAcceptMercQuest(long lQuestID);
	void SetMasterQuest(long lQuestID);
	//�ñ�����//end
	//��������
	void OpenIndexQuestInterface();
	void SetIndexQuestParamNum(long lQuest,long lParam);
	void SetIndexQuestState(long lQuest,long lState);
	long GetIndexQuestParamNum(long lQuest);
	long GetIndexQuestState(long lQuest);

	// ����Ƿ��¼��GS��־�����������Զ�̴���������ɵĴ�����Ҷ��������ۻ�
	bool	GetPlayerLogin()		{return m_bLogin;}
	void	SetPlayerLogin(bool b)	{m_bLogin = b;}

	DWORD	GetWeaponDamageLevel();		// ��ȡ��ǰʹ�õ������Ĺ���ǿ��
	//����ű�����

public:

	//��������������������������ϵ����ݵĲ���������������������

	//
	LONG GetRoleStepNum(LONG ID){return m_RoleQueue.GetRoleStepNum(ID);}
	//Ѱ��ĳ�������ĳһ�����Ƿ����
	BOOL FindRoleStep(LONG ID , LONG Step){return m_RoleQueue.FindRoleStep(ID,Step);}		

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ��ֵ
	LONG GetRoleParam(LONG RoleID, LONG Step ,LONG Index){return m_RoleQueue.GetParam(RoleID,Step,Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	BOOL SetRoleParam(LONG RoleID,  LONG Step,LONG Index, LONG val){return m_RoleQueue.SetParam(RoleID,Step,Index,val);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	BOOL AddRoleParam(LONG RoleID, LONG Step ,LONG Index, LONG val){return m_RoleQueue.AddParam(RoleID,Step,Index,val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ��ֵ
	LONG GetRoleAccParam(LONG RoleID, LONG Step ,LONG Index){return m_RoleQueue.GetAccParam(RoleID,Step,Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	BOOL SetRoleAccParam(LONG RoleID,  LONG Step,LONG Index, LONG val){return m_RoleQueue.SetAccParam(RoleID,Step,Index,val);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	BOOL AddRoleAccParam(LONG RoleID, LONG Step ,LONG Index, LONG val){return m_RoleQueue.AddAccParam(RoleID,Step,Index,val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	LONG GetRoleRegionID(LONG RoleID, LONG Step ,LONG Index){return m_RoleQueue.GetRegionID( RoleID,  Step , Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	BOOL SetRoleRegionID(LONG RoleID,  LONG Step,LONG Index, LONG val){return m_RoleQueue.SetRegionID( RoleID,   Step, Index,  val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	LONG GetRoleRegionX(LONG RoleID, LONG Step ,LONG Index){return m_RoleQueue.GetRegionX( RoleID,  Step , Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	BOOL SetRoleRegionX(LONG RoleID,  LONG Step,LONG Index, LONG val){return m_RoleQueue.SetRegionX( RoleID,   Step, Index,  val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	LONG GetRoleRegionY(LONG RoleID, LONG Step ,LONG Index){return m_RoleQueue.GetRegionY( RoleID,  Step , Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	BOOL SetRoleRegionY(LONG RoleID,  LONG Step,LONG Index, LONG val){return m_RoleQueue.SetRegionY( RoleID,   Step, Index,  val);}
	//*******************************************
	//ȡ�������б����������
	LONG GetRoleSum(){return m_RoleQueue.GetRoleSum();}

	BOOL AddOneRecord(LONG roleid);
	//ɾ��һ������
	BOOL DelOneRecord(LONG ID){return m_RoleQueue.DelOneRecord(ID);}

	//ȡ���������������½�
	LONG GetChapter(){return m_RoleQueue.GetChapter();}

	//�������������½���
	BOOL SetChapter(LONG num);

	//�ƶ����������½�������һ�½�
	BOOL JumpChapterNext(){return m_RoleQueue.JumpChapterNext();}

	//��Ӿ���������¼
	BOOL AddFinishedRoleRecord(LONG index);
	BOOL DelFinishedRoleRecord(LONG index);

	//ȡ��һ�������������״̬���Ƿ���ɣ�
	BOOL GetRoleFinishedRecord(LONG index){return m_RoleQueue.GetRoleFinishedRecord(index);}
	//��ȡһ���������ڶ�Ӧ���ı�index
	LONG GetStepText(LONG lRoleID,LONG lTextID);
	LONG GetText(LONG lRoleID,LONG StepID, LONG lTextID);
	//��������������������������ϵͳ�Ĳ�����������������������������

	//�õ�ٸ���������ֵ
	long GetMercQuestData(long lQuestListID,long lPos){return m_MerQuestSys.GetData(lQuestListID,lPos);}
	//����ٸ������ֵ 
	void SetMercQuestData(long lQuestListID,long lPos,long lValue){ m_MerQuestSys.SetData(lQuestListID,lPos,lValue);}
private:
	const CQuest*		GetQuest(LONG RoleID);								//Ѱ��һ������
	const CQuestStep* GetStep(LONG RoleID, LONG StepID);				//Ѱ�Ҳ���
	const CQuestAim*  GetAim(LONG RoleID, LONG StepID,LONG AimID);	//Ѱ������Ŀ��
	const CQusetEncouragement *GetQuestEncouragement(LONG RoleID,LONG Index);
	const CQusetEncouragement *GetStepEncouragement(LONG RoleID, LONG StepID,LONG Index);
public:
	//ȡ���ı�����
	LONG GetTextType(LONG RoleID);
	//��ȡ��������
	const char* GetAttributeRoleName(LONG);
	//�Ƿ񱻼�¼
	LONG GetAttributeBeRecord(LONG RoleID);
	//��������
	LONG GetAttributeRoleType(LONG RoleID);
	//�������npc��id
	LONG GetAttributeEmbracerNPCID(LONG RoleID);
	//�Ƿ���ʾ
	LONG GetAttributeShowAble(LONG RoleID);
	//�Ƿ��ܷ�����
	LONG GetAttributeRepeated(LONG RoleID);
	//�����Ƿ�ʧ��
	LONG GetAttributeDeadLost(LONG RoleID);
	//����ʧ�ܺ��ܷ����½�
	LONG GetAttributeLostRepeated(LONG RoleID);
	//��С�ȼ�����
	LONG GetAttributeMinGrade(LONG RoleID);
	//���ȼ�����
	LONG GetAttributeMaxGrade(LONG RoleID);
	//ְҵ����
	BOOL GetAttributeOccupationRestrict(LONG RoleID,INT index);
	//������
	LONG GetAttributeOdds(LONG RoleID);
	//��ҪӶ������
	LONG GetAttributeEngageArmsRepute(LONG RoleID);
	//��һ�����id
	LONG GetAttributeNextRoleID(LONG RoleID);
	//�½���
	LONG GetAttributeChapterNum(LONG RoleID);
	//���Ч��
	LONG GetAttributeFinishEffect(LONG RoleID);
	//�Ƿ��ܾ��鱶��Ӱ��
	LONG GetAttributeExpRatio(LONG RoleID);


	//ȡ��index��Ӧ��ǰ������
	LONG GetLabelPrefix(LONG RoleID,LONG Index);
	//ȡ��ǰ����������
	LONG GetLabelSum(LONG RoleID);

	//Ŀ��ģ������
	LONG GetAimAMTType(LONG RoleID,LONG StepID,LONG AimID);
	//Ŀ������
	LONG GetAimType(LONG RoleID,LONG StepID,LONG AimID);
	//ȡ��Ŀ��id
	LONG GetAimId(LONG RoleID,LONG StepID,LONG AimID);
	//ȡ��Ŀ������
	LONG GetAimNum(LONG RoleID,LONG StepID,LONG AimID);
	//ȡ�� ��������
	LONG GetAimParameIndex(LONG RoleID,LONG StepID,LONG AimID);
	//����������Ʒ���������ͣ�����Ʒ��
	LONG GetDropFrom(LONG RoleID,LONG StepID,LONG AimID);
	//����
	LONG GetDropOdds(LONG RoleID,LONG StepID,LONG AimID);
	//ȡ��Ŀ�������
	const char* GetAimName(LONG RoleID,LONG StepID,LONG AimID);
	//ȡ�ñ�������
	LONG GetPrepareParam(LONG RoleID,LONG StepID,LONG AimID,LONG index);


	LONG GetStepEncouragementSum(LONG RoleID,LONG StepID);
	LONG GetQuestEncouragementSum(LONG RoleID);
	//ȡ�ý���������
	const char* GetSetpEncouragementName(LONG RoleID,LONG Step,LONG Index);
	const char* GetQuestEncouragementName(LONG RoleID,LONG Index);
	//ȡ�ý�������
	LONG GetStepEncouragementType(LONG RoleID,LONG Step,LONG Index);
	LONG GetQuestEncouragementType(LONG RoleID,LONG Index);
	//�����ľ���id
	LONG GetStepEncouragementID(LONG RoleID,LONG Step,LONG Index);
	LONG GetQuestEncouragementID(LONG RoleID,LONG Index);
	//��������
	LONG GetStepEncouragementNum(LONG RoleID,LONG Step,LONG Index);
	LONG GetQuestEncouragementNum(LONG RoleID,LONG Index);
	//������ռ�ĸ�����
	LONG GetStepEncouragementCell(LONG RoleID,LONG Step);
	LONG GetQuestEncouragementCell(LONG RoleID);

	//ִ�н���
	BOOL PerformEncouragement(LONG RoleID,LONG StepID);
	BOOL PerformEncouragement(LONG RoleID);
	//�����������
	LONG GetStepSum(LONG RoleID);
	//ȡ�ò����µ�Ŀ������
	LONG GetStepAimNum(LONG RoleID,LONG StepID);
	//����ϵͳ���
	LONG FindRoleIsInSystem(LONG RoleID);
	//һ�������ĳһ�����Ƿ�������ϵͳ��
	BOOL FindRoleStepIsInSystem(LONG RoleID,LONG StepID);
	//�����������
private:
	map<long,CQuestIndexXml::tagQuestIndexForServer>  m_mapQuestIndex;      //��¼���������������Ϣ
public:
    void AddQuestIndex_Param(long lQuest,long lParam);
	void AddQuestIndex_State(long lQuest,long lState);
	void ResetQuestIndex(long lQuest);    //��������һ������������״̬��ʵ�ʴ��б����Ƴ�
	void CheckUpateTime();     //������������Ƿ��Ѵﵽˢ�µ�ʱ��
	bool IsResetTime(CQuestIndexXml::tagQuestIndexForServer IndexQuest);   //���һ�������Ƿ���ˢ������ʱ�䣬�ڴ򿪽���͸ı�һ������ʱ����

	//����end
	//-----------------------------------------------------------------------------------------
	//  �Զ����߹���
	//  �ӳ�n���Ͽ�Socket���ӣ�ȱʡ����0�룬���̶Ͽ���
	//-----------------------------------------------------------------------------------------
private:
	DWORD	m_dwDisconnectTimeStamp;
	DWORD	m_dwDisconnectDelayTime;

public:
	VOID	Disconnect( DWORD dwDelayTime = 0 );

	//-----------------------------------------------------------------------------------------
	//  ���ף��ֿ⣬��̯�ȹ����еĹ��̱�ǣ�
	//  
	//-----------------------------------------------------------------------------------------
	enum eProgress
	{
		PROGRESS_NONE,									//##û���κ�״̬
		PROGRESS_BANKING,								//##�ֿ�
		PROGRESS_TRADING,								//##����
		PROGRESS_SHOPPING,								//##��NPC��������Ʒ
		PROGRESS_OPEN_STALL,							//##��̯
		PROGRESS_INCREMENT,								// ʹ����ֵ�̵�
		PROGRESS_BOUNS,									// �콱
		PROGRESS_FBUSINESS,								// ���������̵�
		PROGRESS_FORCE_DWORD	= 0x7fffffff
	};

private:
	eProgress	m_eProgress;
	CGUID		m_PersonalShopSessionID;
	long		m_PersonalShopPlugID;

public:
	eProgress	GetCurrentProgress						();
	VOID		SetCurrentProgress						( eProgress ep );
	VOID		SetPersonalShopFlag						( const CGUID& SessionID, long PlugID );
	CGUID       GetPersonalShopSessionID()              { return m_PersonalShopSessionID;};
	long        GetPersonalShopPlugID()                 { return m_PersonalShopPlugID;};

	//-----------------------------------------------------------------------------------------
	//  ������Ϣ����ǰ�û�.��ʾ������Ի�����
	//  
	//-----------------------------------------------------------------------------------------

public:
	VOID		SendNotifyMessage( char* szText, DWORD dwColor = 0xffffffff, DWORD dwBkColor = 0,eNOTIFY_POS ePos=eNOTIFYPOS_CENTER);
	VOID		SendSystemInfo( char* szText, DWORD dwColor = 0xffffffff );
	VOID		SendOtherInfo( char* szText );
	VOID		SendPlayerInfo( char* szText, DWORD dwColor = 0xffffffff, DWORD dwBkColor = 0 );
	friend class CMonster;


	//-----------------------------------------------------------------------------------------
	//  �������
	//  
	//-----------------------------------------------------------------------------------------
private:
	LONG		m_lEmotionIndex;
	DWORD		m_dwEmotonTimeStamp;

public:
	VOID		PerformEmotion( LONG lID );
	VOID		ClearEmotion();
	VOID		PackGlint();

	//-----------------------------------------------------------------------------------------
	//  �������
	//  
	//-----------------------------------------------------------------------------------------

	BOOL		Mount( DWORD dwMountType, DWORD dwMountLevel, DWORD dwRoleLimit,const char* strGoodName );
private:
	DWORD m_dwClientIP;
public:
	DWORD GetClientIP() {return m_dwClientIP;}
	void SetClientIP(DWORD dwClientIP) {m_dwClientIP=dwClientIP;}

	//-----------------------------------------------------------------------------------------
	//  ʰȡ,������Ʒ���
	//  
	//-----------------------------------------------------------------------------------------
public:

	virtual BOOL OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

	virtual BOOL OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);
	//�������Գɹ��������Ʒ��֪ͨ�ͻ���
	BOOL AddGoodsByTest(tagTestResult* tgResult,CGoods* pGoods);
	//�������Գɹ���ɾ����Ʒ��֪ͨ�ͻ���
	BOOL DelGoodsByTest(tagDelTestResult* tgResult);
	//////////////////////////////////////////////////////////////////////////
	//	�����ű�
	//////////////////////////////////////////////////////////////////////////
public:
	void	SetLastContainerScript(const char* szScript)	{m_strLastContainerScript = szScript;}
	const char *GetLastContainerScript()					{return m_strLastContainerScript.c_str();}

	//���һ��������ť��ʾ
	long AddHelpHint(const char* pszScript,long lFutureDelTime);
	//�Ƴ�һ��������ʾ
	void RemoveHelpHint(long lHintID);
	//����ҵ��������ʾ��ť
	void OnPlayerHitHelpHint(long lHintID,long lFlag);
	//���һ��������ʾ
	long AddInterfaceHint(const char* pszScript,long lFutureDelTime,
		const char* pszInterName,long lX,long lY,long lWidth,long lHeight);
	//�Ƴ�һ��������ʾ
	void RemoveInterfaceHint(long lHintID);
	//����ҵ��������ʾ
	void OnPlayerHitInterfaceHint(long lHintID,long lFlag);
	//����ʾ��ʱʱ
	void OnHintTimeOut(long lHintID);
	//���������ʾ
	void ClearAllHint();

private:
	string	m_strLastContainerScript;		// �������һ��׼��ִ�е������ű�

	struct tagHint
	{
		//���֮��ִ�еĽű�
		string	strScript;
		//��ʱ��ID
		long	lTimerID;
		//���
		long	lFlag;
	};
	typedef map<long,tagHint>	mapHints;
	typedef map<long,tagHint>::iterator itHint;

	//��ʾ�ļ�����
	long m_nHintCounter;
	//������ʾ
	mapHints m_HelpHints;
	//������ʾ
	mapHints m_InterfaceHints;

public:
	const CGUID& GetPersonalRgnGUID(void);
	const CGUID& GetPersonalHouseRgnGUID(void);
	void SetPersonalRgnGUID(const CGUID& guid);
	void SetPersonalHouseRgnGUID(const CGUID& guid);

	//!	ע��һ���ŶӶ�ʱ���������ض�ʱ��ID
	long RegisterQueueTimerID(long lFutureTime);
	//!	ȡ��һ���Ŷӵ�Timer
	void CancelQueueTimer(void);
	//!	��Ӧ�ŶӶ�ʱ���������¼�
	void OnQueueTimeout(long lTimerID);

	//!	ע��һ������P������ʱ���������ض�ʱ��ID
	long RegisterCreatePRgnTimerID(long lFutureTime);
	//!	ȡ��һ������P������Timer
	void CancelCreatePRgnTimer(void);
	//!	��Ӧ����P������ʱ���������¼�
	void OnCreatePRgnTimeout(long lTimerID);

	//!	ע��һ������T������ʱ���������ض�ʱ��ID
	long RegisterCreateTRgnTimerID(long lFutureTime);
	//!	ȡ��һ������T������Timer
	void CancelCreateTRgnTimer(void);
	//!	��Ӧ����T������ʱ���������¼�
	void OnCreateTRgnTimeout(long lTimerID);

	//!	ע��һ���������˶�ʱ���������ض�ʱ��ID
	long RegisterRgnKickPlayerTimerID(long lFutureTime);
	//! ע��һ��������ϵͳ���˵Ķ�ʱ���������ض�ʱ��ID
	long RegisterFcmKickPlayerTimerID(long lFutureTime);
	//!	ȡ��һ������T������Timer
	void CancelRgnKickPlayerTimer(void);
	//!	��Ӧ����T������ʱ���������¼�
	void OnRgnKickPlayerTimeout(long lTimerID);

	// �õ������������ж�ʱ��ID
	long GetQueueTimerID(void) { return m_lQueueTimerID; }
	// ���ô����������ж�ʱ��ID
	void SetQueueTimerID(long value) { m_lQueueTimerID = value; }

	// �õ������������ж�ʱ��ID
	long GetPRgnTimerID(void) { return m_lPRgnTimerID; }
	// ���ô����������ж�ʱ��ID
	void SetPRgnTimerID(long value) { m_lPRgnTimerID = value; }

	// �õ������������ж�ʱ��ID
	long GetTRgnTimerID(void) { return m_lTRgnTimerID; }
	// ���ô����������ж�ʱ��ID
	void SetTRgnTimerID(long value) { m_lTRgnTimerID = value; }

	// ���ϣ��������DupRgn����
	eRgnType GetCreateDupRgnType(void) { return m_CreateDupRgnType; }
	void SetCreateDupRgnType(eRgnType type) { m_CreateDupRgnType = type; }
	// ���ϣ��������DupRgn��ģ��ID
	long GetlCreateDupRgnTID(void) { return m_lCreateDupRgnTID; }
	void SetlCreateDupRgnTID(long value) { m_lCreateDupRgnTID = value; }

	// ���ϣ��������DupRgn�Ľű�ID
	const CGUID& GetlCreateDupRgnScriptID(void) { return m_DupRgnScriptID; }
	void SetlCreateDupRgnScriptID(const CGUID& value) { m_DupRgnScriptID = value; }

	long GetCurPRgnNum(void) { return m_CurPRgnNum; }
	long GetCurTRgnNum(void) { return m_CurTRgnNum; }

	void SetCurPRgnNum(long num) { m_CurPRgnNum = num; }
	void SetCurTRgnNum(long num) { m_CurTRgnNum = num; }

	// �õ����״̬����
	ePlayerState	GetPlayerState(void) { return m_PlayerState; }
	// �������״̬����
	void			SetPlayerState(ePlayerState state) { m_PlayerState = state; }

	DWORD GetSavePartFlag(void) { return m_PlayerSavePartFlag; }
	void  SetSavePartFlag(DWORD flag) { m_PlayerSavePartFlag = flag; } 

	bool	GetFirstLogin()		{return b_FirstEnterGame;}
	void	SetFirstLogin(bool b)	{b_FirstEnterGame = b;}

	long    GetSignCode(void) { return m_lSignCode; }
	void	SetSignCode(long code) { m_lSignCode = code; }

private: 
	// �Ƿ��һ�ε�¼
	bool	b_FirstEnterGame;
	CGUID	m_PersonalRgnGUID;
	CGUID	m_PersonalHouseRgnGUID;	
	long	m_lQueueTimerID;//�����������ж�ʱ��

	long m_lPRgnTimerID;
	long m_lTRgnTimerID;
	long m_lRgnKickPlayerTimerID;

	long m_CurPRgnNum;
	long m_CurTRgnNum;
	// ���ϣ��������DupRgn����
	eRgnType   m_CreateDupRgnType;
	// ���ϣ��������DupRgn��ģ��ID
	long		m_lCreateDupRgnTID;
	// ���������Ľű�ID��������
	CGUID        m_DupRgnScriptID;
	// ���״̬����
	ePlayerState	m_PlayerState;
	// ���Ҫ��������Բ���ö��ֵ
	DWORD m_PlayerSavePartFlag;
	// ��¼��֤��
	long m_lSignCode;

	//-------------------------------------------------------------------------------
	// �ỰID. by Fox.		2008-01-22
	// func : ��Ӧ����ڸ�GS�ϵļƷ�Ψһ��ʶ
	//-------------------------------------------------------------------------------
public:
	char *GetSessionID()			{ return m_szSessionID; }
	void SetSessionID(char *szID)	{ strcpy_s(m_szSessionID, 33, szID); }

private:
	char		m_szSessionID[33];
public:
	typedef map<DWORD,list<DWORD>> IncShopCur10Map;
	typedef IncShopCur10Map::iterator IncShopCur10Iter;

	//-------------------------------------------------------------------------------
	// ��ֵ�̵�flag. by Fox.		2008-02-14
	// func : flagΪtrue������Ʒ��Ϣ��Ϊfalse�򲻷���
	//-------------------------------------------------------------------------------
public:
	bool GetIncShopFlag()					{ return m_bIncShopFlag; }
	void SetIncShopFlag(bool bFlag = false)	{ m_bIncShopFlag = bFlag; }

	//��ֵ�̵���������Top10
	void Add2IncShopCur10( DWORD dwMode,DWORD dwGoodsID );
	IncShopCur10Map &GetIncShopCur10()			{ return m_mapIncShopCur10; }
	void ClearIncShopCur10();

	void Add2IncTradeList( DWORD dwMode, DWORD dwGoodsId, DWORD dwGoodsNum, long lTotalAmount);
	CIncrementShopList::VEC_BUYLIST GetIncTradeList() { return m_vecTradeList; }
	void ClearIncTradeList() { m_vecTradeList.clear(); }

	//��ֵ�̵�����������Ӽ�¼
	void AddLimitGoodsRecord(DWORD dwGoodsId,DWORD dwGoodsNum);
	//����ֵ�̵꣩��ȡ�Ѿ���������
	DWORD GetLimitGoodsBuyNum(DWORD dwGoodsId);

private:
	bool		m_bIncShopFlag;
	//��ֵ�̵���������Top10��¼
	IncShopCur10Map m_mapIncShopCur10;
	//��ֵ�̵깺����ϸ��¼�б�
	CIncrementShopList::VEC_BUYLIST m_vecTradeList;
	//��ֵ�̵������޹���Ʒ�����¼
	map<DWORD,DWORD> m_mapLimitGoodsRecord;


	//-------------------------------------------------------------------------------
	// �����õ�ȯ. by Fox.		2008-02-15
	// func : �����õ�ȯ
	//-------------------------------------------------------------------------------
public:

	long GetTicket()				{ return m_lTicket; }
	void SetTicket(long	lTicket);

private:
	long		m_lTicket;

private:
	//�Ƿ��ܲɼ�
	tagUseFailed CanDOccuCollect(CMoveShape* moveshape);
	//�ɼ�
	BOOL DOccuCollect(CMoveShape* moveshape);
	//�Ƿ�������ɼ�
	tagUseFailed CanRoleCollect(CMoveShape* pMoveShape);
	//����ɼ�
	BOOL RoleCollect(CMoveShape* pMoveShape);
public:
	tagUseFailed CanCollect(CMoveShape* pMoveShape);
	tagUseFailed CheckCanCollect(CMoveShape* pMoveShape);
	BOOL CheckCollect(CMoveShape* pMoveShape);
	//�����ɼ���
	VOID LockCollection(CMoveShape* pMoveShape);
	//����ɼ�������
	VOID OutlockCollection(CMoveShape* pMoveShape);
	//�Ƿ�������
	tagUseFailed CanFacture();
	//����
	BOOL Facture();
	//������ѧϰ
	VOID FactureSkillStudyBegin(CMoveShape* moveshape);
	VOID StudyFactureSkill(CMoveShape* moveshape,DWORD factureID);
	VOID CleanFacture();
	VOID AddCollectionExp(DWORD exp);
	VOID AddFactureExp(DWORD exp);

	BOOL AddFacture(DWORD l);
	BOOL DeleteFacture(DWORD l);
	BOOL FindFacture(DWORD l){return m_DOccuRecord.FindFacture(l);}
	//���͸�ְҵʧ����Ϣ
	void SendDOccuSKillFailMsg(int nValue,int iSkillID, int iSkillLevel);
	//��ȡ�� ����  ѡ���������id
	DWORD GetElectFactureID(){return m_dwElectFactureID;}
	void  SetElectFactureID(DWORD id){m_dwElectFactureID= id;}

	//�Ը�ְҵϵͳ�Ĳ�ѯ
	long GetFactureNeedsLevel(DWORD ID);
	long GetFactureNeedsDOccu(DWORD ID);

private:
	//��ְҵ��¼�Ĳ���
	LONG GetCollectionExp(){return m_DOccuRecord.m_lCollectionExp;}
	VOID SetCollectionExp(LONG l);
	LONG GetCollectionGrade(){return m_DOccuRecord.m_lCollectionGrade;}
	VOID SetCollectionGrade(LONG l);
	LONG GetFactureExp(){return m_DOccuRecord.m_lFactureExp;}
	VOID SetFactureExp(LONG l);
	LONG GetFactureGrade(){return m_DOccuRecord.m_lFactureGrade;}
	VOID SetFactureGrade(LONG l);
	LONG GetFactureSuccRatio(){return m_DOccuRecord.GetFactureSuccRatio();}
	VOID AddFactureSuccRatio(LONG l){m_DOccuRecord.AddFactureSuccRatio(l);}
	VOID ClearFactureSuccRatio(){m_DOccuRecord.ClearFactureSuccRatio();}

public:
	void GetPlayerIDAndTeammateID(hash_set<CGUID,hash_guid_compare>& hsPlayers);
private:
	CDeputyOccuRecord m_DOccuRecord;
	DWORD m_dwElectFactureID;

public:
	virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const char* value);
	virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray=0, double value=0);
	virtual const char* GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	virtual double GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray);

	virtual int SetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const CGUID& guid);
	virtual const CGUID& GetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	//ˢ�³�����Ч
	void UpdateEffect();//UPDATEEFFECT();


public:
	VOID SetServerKey(DWORD dwIdx, DWORD dwKey){if(SERVER_KEY_DWORD_SIZE - 1 < dwIdx)return; m_dwServerKey[dwIdx] = dwKey;}
	DWORD GetServerKey(DWORD dwIdx){if(SERVER_KEY_DWORD_SIZE - 1 < dwIdx)return 0; DWORD dwRe = m_dwServerKey[dwIdx]; m_dwServerKey[dwIdx] = 0; return dwRe;}

private:
	DWORD m_dwServerKey[SERVER_KEY_DWORD_SIZE];

	//-------------------------------------------------------------------------------
	// С����ϵͳ
	// 
	//-------------------------------------------------------------------------------
public:
	SpriteSystem			m_SpriteSystem;

	//-------------------------------------------------------------------------------
	// �����Ҫ�����ڷ�����������
	// 
	//-------------------------------------------------------------------------------
public:
	PlayerSetupOnServer		m_SetupOnServer;
	//-------------------------------------------------------------------------------
	// ѫ����������ǰ�ƺ�
	// 
	//-------------------------------------------------------------------------------
public:
	//!						ѫ������
	MedalContainer			m_MedalContainer;

	//!						���óƺŵ���ʾ����
	BOOL					SetTitleType(LONG lType);
	//!						�õ��ƺŵ���ʾ����
	LONG					GetTitleType(void){return m_lCurrTitleType;};
	//!						���³ƺ�
	void					UpdateTitle(BOOL bSendNotify);

	//!						�õ���ǰ��ʾ��ѫ�µ�ԭʼ��
	const	char*			GetShowMedalBaseName(void);
private:
	//!						��ǰ�ƺ�����
	LONG					m_lCurrTitleType;

	//-------------------------------------------------------------------------------
	// ͨ����Ʒ��������
	// 
	//-------------------------------------------------------------------------------
public:
	GoodsAllPurposeOpt&		GetGoodsAllPurposeOpt(VOID){return m_GoodsAllPurposeOpt;}
private:
	//!						
	GoodsAllPurposeOpt		m_GoodsAllPurposeOpt;
private:
	long					m_lTotalSkillNum;
	DWORD					m_dwSkillStartTime;	
	long					m_lTotalNpcScriptsNum;
	DWORD					m_dwNpcScriptsStartTIme;
	CGUID					m_guidRelivScript;					//��������ű�GUID
	bool					m_bReliveSkill;						//���ܸ����־��

public:
	//�жϼ����Ƿ�������
	bool					JudgeSkilllegality();		
	//�ж�NPC�ű��Ƿ�������
	bool					JudgeNpcScriptslegality(void);
	//�жϵ�ǰ�����ܷ�ԭ�ظ���
	bool					CheckReliveByOrigin();
	//�ж�ԭ�ظ�����Ʒ�Ƿ���������
	bool					CheckReliveGoods(long &lIndex, long &lRatio, long &lCostNum,char* strOriginName);			
	//������������ű�
	void					SetReliveScript(CGUID guid){ m_guidRelivScript = guid; }
	//�ͷŹ������������ű�
	void					FreeReliveScript();
	//���ü��ܸ����־��
	void					SetReliveSkill(bool bSkill)	   {m_bReliveSkill = bSkill;}
	//��ȡ���ܸ����־��
	bool					GetReliveSkill()			   {return m_bReliveSkill ;}
	//���ݴ���,�ɹ�����true,���򷵻�false
	long					ChangeFaceHair(tagCosKey tgKey,long lHairColor);
	//����ʧ�ܣ����ͻ��˷���ʧ����Ϣ(bTypeΪ1��ʾ���ϲ��㣬2��ʾ���п���)
	//void					FailedChangeFaceHair(BYTE bType);

private:
	GoodsTimerHandler m_GoodsTimerHandler;

	//////////////////////////////////////////////////////////////////////////
	// ����ʱ���
	//////////////////////////////////////////////////////////////////////////
private:
	DWORD m_dwBounsTime;

public:
	void	SetBounsTime(DWORD dwBountTime)		{ m_dwBounsTime = dwBountTime; }
	DWORD	GetBounsTime()						{ return m_dwBounsTime; }
private:
	DWORD m_dwReportTime;
	long  m_lCostYuanBao;
public:
	void SetReportTime(DWORD dwTime)			{m_dwReportTime=dwTime;}
	DWORD GetReportTime()						{return m_dwReportTime;}

//��������
	void OnCostYuanBao(long lNum);
	//��������Ԫ������
	void SetCostYuanBao(long lNum)				{m_lCostYuanBao = lNum;}
	long GetCostYuanBao()						{return m_lCostYuanBao;}

public:

	struct tagCountTimer
	{
		tagCountTimer()
			: m_bCountType(false)
			, m_dwCountTime(0)
			, m_dwID(-1)
			, m_dwRetTime(0)
			, m_dwTimerType(-1)
			, m_dwStartRetTime(0)
		{

		}
		~tagCountTimer()
		{

		}

		void operator=(const tagCountTimer& r)
		{
			m_bCountType = r.m_bCountType;
			m_dwCountTime = r.m_dwCountTime;
			m_dwID = r.m_dwID;
			m_dwRetTime = r.m_dwRetTime;
			m_dwTimerType = r.m_dwTimerType;
			m_dwStartRetTime = r.m_dwStartRetTime;
		}

		bool m_bCountType; //��ʱ���ͣ�0��˳�� 1������
		DWORD m_dwCountTime; // ��ʱ��ʼ�㣬 ��λ����
		DWORD m_dwID; // ��ʱ��ID
		DWORD m_dwRetTime; // ��ʱʣ��ֵ,��λ����
		DWORD m_dwTimerType; // ��ʱ�����ͣ������û�ȡ������
		DWORD m_dwStartRetTime; // ʣ���ʱ�����ֵ
	};

	void AddCountTimer(tagCountTimer& timer);
	tagCountTimer& GetCountTimer(DWORD id);
	void DelCountTimer(DWORD id);
	DWORD GetCountTimerNum(void);
	void ClearCountTimer(void);
	DWORD GetCurCountTimerID(void);
	void SetCurCountTimerID(DWORD id);
	
private:
	// ��ʱ������
	map<DWORD, tagCountTimer> m_mapCountTimer;
	static tagCountTimer NULL_TIMER;
	DWORD m_dwCountTimerID;

public:
	//!                         ����������һ����Ʒ���ýӿڲ���ͻ��˷���AddGoods��Ϣ��
	BOOL                        AddGoodsToServer(CGoods* pGoods);
	//!                         ������������Ʒ(����1����Ʒ��ţ�����2:����)
	bool                        AddGoodsForServer(DWORD dwGoodsIndex,int n);        
	//!                         ��ͻ������һ����Ʒ ���ýӿ���AddGoodsToServer�ӿڶ�Ӧ��
	BOOL                        AddGoodsToClient();
private:
	bool    m_bAddGoodsToServer ;     //������������Ʒ�Ƿ�ɹ�
	CS2CContainerObjectMove m_comMsg; //�����ڷ����������Ʒʱ��¼
	CGUID  m_GoodsToServerGUID;        //��¼��ӵ�����������Ʒ��GUID
	DWORD m_AddGoodsContainerId;
	DWORD m_AddGoodsPos;
	int    m_iAddGoodsType;           //�����Ʒ�ķ�ʽ[�¼���Ʒ��ı���Ʒ������]
private:
	bool m_bSortContainer;		 //��������־��
public:
	bool GetSortSontainr()	{return m_bSortContainer;}
	void SetSortContainer(bool b){m_bSortContainer = b;}
//////////////////////////��Ʊϵͳ���////////////////////////////////////////////////////////////
private:
	CLottery m_Lottery;               //���ó齱��
	vector<tagLottery> m_VecLottery;
	DWORD m_dwLotteryRe;              //һ�γ齱�Ľ�Ʒ��
	DWORD m_dwLastLotteryTime;	      //�ϴγ齱ʱ��
	BYTE m_btFileId;					//�н���Ʒ��λ���

	long m_lPersonalCredit;       //���˻���
	long m_lPersonalCreditTotal;  //�����ۼƻ���

	int m_iPersonalCreditVelue;  //���˻��ֲ���
	int m_iWorldCreditVelue;     //������ֱ���
	int m_iLotteryLevel;         //�齱����
	bool m_bSendWorldInfo;       //�Ƿ���������Ϣ
	

public:
	bool DoLottery();                 //��ʼ�齱
	bool RefreshVecLottery(bool bCost=true);         //ˢ�½�Ʒ��λ
	bool RefreshVecConstel(bool bCost=true);         //ˢ��������λ
	void OpenLotteryPage();           //�򿪳齱����
	void SendVecLottery();            //������λ��Ϣ
	void SendVecConstel();            //����������Ϣ
	void SendLotteryGoods();		  //������Ʒ�Լ�ǿ��������Ϣ.
	//void UpdateLotteryInf();		  //���������ص���Ϣ
	void SystemReset();
	void SetPersonalCredit(long l){m_lPersonalCredit=l;};   //���ø��˻���
	long GetPersonalCredit(){return m_lPersonalCredit;};    //��ø��˻���
	DWORD GetLastLotteryTime(){return m_dwLastLotteryTime;};
	void SetPersonalCreditTotal(long l){m_lPersonalCreditTotal=l;};  //���ø����ۼƻ���
	long GetPersonalCreditTotal(){return m_lPersonalCreditTotal;};   //��ø����ۼƻ���
	void SendPersonalCreditInfo();
	void SetPersonalCreditVelue(int i){m_iPersonalCreditVelue=i;};  //������ҵĻ��ֱ���
	int GetPersonalCreditVelue(){return m_iPersonalCreditVelue;};
	void SetWorldCreditVelue(long l){ m_iWorldCreditVelue = l;};			//����������ֱ���
	int GetWorldCreditVelue(){return m_iWorldCreditVelue;};		//���������ֱ���
	void SetLotteryLevel(int i){m_iLotteryLevel = i;};
	int  GetLotteryLevel(){return m_iLotteryLevel;};
	void ResetPersonalCredit();
	void SendFieldType();
	void SetSendWorldInfo(bool b){m_bSendWorldInfo=b;};
	bool GetIsSendWorldInfo(){return m_bSendWorldInfo;};
	DWORD GetLotteryRe(){return m_dwLotteryRe;};
};
