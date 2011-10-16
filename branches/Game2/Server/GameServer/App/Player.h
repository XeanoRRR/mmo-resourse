#pragma once

#include "Business/CBusinessPackage.h"
#include "Container/CAmountLimitGoodsContainer.h"
#include "Container/CAmountLimitGoodsShadowContainer.h"
#include "Container/CVolumeLimitGoodsContainer.h"
#include "Container/CEquipmentContainer.h"
#include "Container/CSubpackContainer.h"
#include "Container/SpecialEquipContainer.h"
#include "Container/CWallet.h"
#include "GoodsAllPurposeOpt.h"
#include "GoodsTimerHandler.h"
#include "Mail/MailManager.h"
#include "MedalContainer.h"
#include "MoveShape.h"
#include "PlayerDepot.h"
#include "Mover.h"
#include "PlayerPack.h"
#include "SpriteSystem.h"
#include "GeniusSys/GeniusAction.h"

#include "../../../Public/Common/SkillRelated.h"
#include "../../../Public/Common/TeamDef.h"
#include "../../../Public/Common/QuestRecord.h"
#include "../../../Public/Common/PlayerSetupOnServer.h"
#include "../../../Public/Common/StudySkillCondition.h"
#include "../../../Public/Common/OccupSkillList.h"
#include "../../../Public/Common/CreditSystem.h" 
#include "../../../Public/Common/BuddhismDef.h"
#include "../../../Public/ServerPublic/Server/Common/GoodsBaseProperties.h"
#include "../../../Public/ServerPublic/Server/BaseDef/BasePetServerDef.h"
#include "../../../Public/ServerPublic/Server/Common/DeputyOccuRecord.h"
#include "../../../Public/ServerPublic/Server/BaseDef/PhaseSeverDef.h"
#include "../../../Public/Setup/IncrementShopList.h"

class TauntedList;
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

class CPacketListener : public CContainerListener
{
public:
	DBWriteSet* m_pDBWriteData;
	CPacketListener();
	~CPacketListener();

	virtual bool OnTraversingContainer(CContainer* pContainer, CBaseObject* pObject);
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

	float	m_fExpScale;//��þ������
	float	m_fOccuExpScale;// ���ְҵ����ϵ��
	float	m_fSpScale;//��ȡԪ���ı���
	float	m_fPresentExpScale;//��ȡ���;���ı���

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
	long    DirtyAttrDataSize(void) { return (long)m_mapDirtyAttrData.size(); }

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
	static map<eGoodsAddonProperties,string> m_mapFightEnum;
	static void InitFightEnumMap();
	static string GetFightPropertyNameByEnum(eGoodsAddonProperties);

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
    virtual long  SetStateAttribute(std::string& attrName,long    value);
    virtual long  ChangeStateAttribute(std::string& attrName,long value);

	virtual void  ChangeStateTo(CShape::eSTATE state);

	void          CodeChangeAttrToDataBlock(DBWriteSet& setWriteDB);
	bool          CodeOtherUpdateAttrToDataBlock(DBWriteSet& setWriteDB);
	bool	CodeDirtyAttrToDataBlock(set<string>& dirtyAttr, DBWriteSet& setWriteDB);

	void		  AddFightPropertyTODirty();
	//��ӱ�������������
	void  AddAttributeToDirty(string attrName);

	BOOL SerializeSubDepot(DBWriteSet& setWriteDB, LONG eContainerId);

	// ȡ�õ�ǰֵ�ĺϷ�ֵ
	virtual long  GetCurrentValue(char* valueName, const tagPty& tagValue, long evalue/*�ı�ֵ*/, long tmpValue/*���μ���õ���ֻ*/);

	void ChangePropertyByGoods(CGoods* pGoods,eGoodsAddonProperties addon,int whichVal,string strAttrName,bool bMount);
	

	void UpdatePropertyByGoodsTrigger(CGoods* pGoods,bool bMount);

	//�ı�ְҵ
	void ChangeOccu(eOccupation eNewOccu);	

	//��ҵļ��ܳ�ʼ��
	void InitSkills();

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


	virtual tagSkillID GetDefaultAttackSkillID();

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
	//string						m_strDepotPassword;
	//����<ó����ID������ֵ>
	map<DWORD,DWORD> m_mapCredit;
	//Ͷ�ʼ�¼<ó�׵�ID��Ͷ�ʶ�>
	map<DWORD,DWORD> m_mapInvestRecord;
	long m_lInvestLimitTimeMark;

	//��ʱ�����������NPC�̵����Ʒ��������Ʒ�ع�����
	list<tagShopSelledGoods> m_listNpcShopSellGoods;

	CGUID m_TradeSessionId;
	LONG m_lTradePlugId;

    long       lLastSwitchEquipTime;      // ��һ�������л�ʱ��
    long       lLastSwitchTalismanTime;   // ��һ�η����л�ʱ��
    long       lLastSwitchRiderTime;      // ��һ�������л�ʱ��
    long       lLastSwitchSoulTime;       // ��һ�λ����л�ʱ��

public:
	void SetTradeSessionId(const CGUID& sessionId){m_TradeSessionId=sessionId;}
	void SetTradePlugId(const LONG lId){m_lTradePlugId=lId;}
	const CGUID& GetTradeSessionId(){return m_TradeSessionId;}
	const LONG GetTradePlugId(){return m_lTradePlugId;}



public:
	//static const BYTE SUBPACK_NUM=5;
	void SetCredit(DWORD dwAreaId,DWORD dwCredit);
	void AddCredit(DWORD dwAreaId,DWORD dwCredit);
	void ChangeCredit(DWORD dwAreaId,long lCredit);
	DWORD GetCredit(DWORD dwAreaId);
	BOOL SetInvestRecord(DWORD dwSpotId,DWORD dwInvestVal);
	DWORD GetInvestRecord(DWORD dwSpotId);
	void ClearInvestRecord(){m_mapInvestRecord.clear();}
	void SetInvestLimitTimeMark(long lVal){m_lInvestLimitTimeMark=lVal;}
	long GetInvestLimitTimeMark(){return m_lInvestLimitTimeMark;}

	void PushbakNpcShopSellGoods(CGoods* pGoods,DWORD dwPrice);
	CGoods* FetchNpcShopSellGoods(const CGUID& guid);
	CGoods* FindNpcShopSellGoods(CGUID& guid,DWORD* dwPrice);
	void ReleaseNpcShopSellGoods();
    void CodeNpcShopSellGoodsToDataBlock(DBWriteSet& setWriteDB);
    void DecodeNpcShopSellGoodsFromDataBlock(DBReadSet& setReadDB);

	void SetLastPickGoodsTime(long t){m_lLastPickGoodsTime=t;}
	long GetLastPickGoodsTime(){return m_lLastPickGoodsTime;}

    long	GetLastSwitchEquipTime()		    { return lLastSwitchEquipTime; }
    void	SetLastSwitchEquipTime(long l)      { lLastSwitchEquipTime=l; }
    long    GetLastSwitchTalismanTime()         { return lLastSwitchTalismanTime; }
    void    SetLastSwitchTalismanTime(long l)   { lLastSwitchTalismanTime = l; }
    long    GetLastSwitchRiderTime()            { return lLastSwitchRiderTime; }
    void    SetLastSwitchRiderTime(long l)      { lLastSwitchRiderTime = l; }
    long    GetLastSwitchSoulTime()             { return lLastSwitchSoulTime; }
    void    SetLastSwitchSoulTime(long l)       { lLastSwitchSoulTime = l; }

	BOOL UpdateCooldown(DWORD dwGoodsId);
	
	long GetCreditLevel(DWORD dwAreaID); //��ȡָ������������ȼ�
	long GetOrganLevel(const char* strName);//��ȡָ����֯�������ȼ�
	void AddOrganCredit(const char* strName,DWORD dwCredit);//������֯����
	void ChangeOrganCredit(const char* strName,DWORD dwCredit);//�޸���֯����
	long GetOrganCredit(DWORD dwOrganID);//��ȡ��֯����
    void CodeCreditToDataBlock(DBWriteSet& setWriteDB);
    void DecodeCreditFromDataBlock(DBReadSet& setReadDB);
    
private:
	//����
	//CVolumeLimitGoodsContainer* m_cPackets[PACK_NUM];
//	CVolumeLimitGoodsContainer* m_cOriginPack;
//	CSubpackContainer* m_pSubpackContainer;
    CPlayerPack    m_pPackage;     // �ܱ���

	//tagSubPack m_SubPack[SUBPACK_NUM];
//	void gcPacks();		
	//BYTE getSubPackAmount();
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

	CVolumeLimitGoodsContainer* getOriginPack(){return m_pPackage.GetOriginPack();}
	CSubpackContainer* getSubpackContainer(){return m_pPackage.GetSubpackContainer();}
    CPlayerPack* GetPackage()   { return &m_pPackage; }
	CEquipmentContainer* GetEquipmentContainer(){return &m_cEquipment;} 
	CVolumeLimitGoodsContainer* GetPackContainerById(DWORD dwId);
//	int GetSubpackCanPlace();
	void OnSuitNumChanged(long suit_id,int suit_num);
	CGoods* FindGoods(LONG lContainerId,LONG lPos);
	//������Ʒԭʼ�������б����в��ҵ�һ����Ʒ
	CGoods* FindGoodInPack(const char* strGoodsName);
	//! ������Ʒԭʼ���ڲֿ����ҵ���һ����Ʒ
	CGoods* FindGoodInDepot(const char* strGoodsName);

	BOOL IsGoodsUnique(const char* strGoodsName);

	BOOL DelGoods(LONG lContainerId,LONG lPos);

	void ClearGoods(LONG lContainerId,LONG lPos);

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
	void DelGoodsByTestResult(vector<tagDelTestResult> &vecDelResult);

	DWORD FindGoods(LONG lContainerId,LONG lPos,const char* strGoodsName);
	virtual DWORD FindGoodsByName(LONG lContainerId,const char* strGoodsName);
	virtual DWORD FindGoodsByType(LONG lContainerId,int nType);
	DWORD GetTotalEmptySpaceNum();
	DWORD GetTotalSpaceNum();//��ȡ������б��������� 090916by tanglei
	DWORD GetGoodsAmountByOriginName(const char* strOriginName);
	BOOL IsEquipBusinessPack();

	void ClearPersonalShopGoodsData(){m_vecInPersonalShopGoods.clear();}
	void RemovePersonalShopGoodsData(CGUID& guid);
	BOOL IsInPersonalShop(CGoods* pGoods);
	void AddPersonalShopGoodsData(CGoods* pGoods);

	BOOL CheckChangeGoods(vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsIn,vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsOut);
	void GoodsChangeGoods(vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsIn,vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsOut);

	void AllEquipmentRefineUpdate(long lVal);



	//CVolumeLimitGoodsContainer* getSubPack(BYTE pos);

	hash_map<CGUID,CGoods*,hash_guid_compare>* GetOriginPackGoodsMap(){return m_pPackage.GetOriginPack()->GetGoodsMap();}
//	hash_map<CGUID,CGoods*,hash_guid_compare>* GetSubPackGoodsMap(int i);
	map<eEquipType,CGoods*>* GetEquipmentGoodsMap(){return m_cEquipment.GetGoodsMap();}

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
	void OnGetGoods(DWORD dwGoodsId,CGoods* pGoods=NULL);
	//! ��Ӧ��Ʒʧȥ
	void OnLoseGoods(CGoods* pGoods);
	//! ��Ӧ�ֿ���Ʒ���
	void OnDepotGetGoods(CGoods* pGoods);
	//! ��Ӧ�ֿ���Ʒʧȥ
	void OnDepotLoseGoods(CGoods* pGoods);

	//! ɾ����Ʒ��֪ͨ���
	void DelGoodsAndSendMsg(long lContainerId, long lPos);
	//! ��������ϣ����б�����װ����λ���ֿ⣩�ҵ�һ����Ʒ��������������Ʒ��
	CGoods* FindGoodByGuid(const CGUID &GoodsGUID);

	CGoods* FindGoodsAllPlaceRemove(const CGUID& guid);
	//! ��������б����������ӱ��������ҵ�һ����Ʒ��������������Ʒ��
	CGoods* FindGoodsByGuidInPack(const CGUID &GoodsGUID);
	BOOL FindGoodsByGuid(const CGUID& guid,DWORD& dwContainer,DWORD& dwPos);
	CGoods* RemoveGoods(long lContainerID,long lPos);

    void    CheckGoodsTimeOut(CGoods* pGoods);      

private:


	//!								��Ʒ����ʱ�䴦��
	//!								��δ��ʱ���ö�ʱ��������ʱ������true��Ӧ���ٽӿ��ⲿɾ����Ʒ
	bool    TestGoodsTimeout(CGoods* pGoods);
	//!								ͨ����Ʒguid�ҵ���Ʒ����λ��
	//!								ReValue��pair<����ID����Ʒλ��>
	//!								�ҵ������棬���򷵻ؼ�
	bool	FindGoodsByGuid(const CGUID &GoodsGUID,  pair<long, long> &ReValue);

    
	/// ��顯����������˥��
//	bool CheckGuardianEnergy( CGoods *pGoods, bool bLogin );	

	/// ע�ᡮ�����������ݼ���ʱ��
	long RegisterGuardianEnergyDecTimer( const CGUID &goodsID );

	/// ��Ӧ��������������ʱ��
//	void OnGuardianEnergyTimeOut( const CGUID &goodsID );

	//!								Ϊ��Ʒע��һ����ʱ���������ض�ʱ��ID
	long							RegisterLimitGoodsTimerID(const CGUID &GoodsGUID, ULONG dwFutureTime);
	//!								ȡ��һ����Ʒ��Timer
	void							CancelLimitGoodsTimer(const CGUID &GoodsGUID);
	//!								��Ӧ��ʱ����������Ʒɾ���¼�
	void							OnLimitGoodsTimeout(long lTimerID);

	//!								��ƷGUID�Ͷ�ʱ��ID��������
	map<CGUID, long>				m_GoodsTimerID;

	long							RegisterInvestTimer();

	//hash_map<CGUID,CGoods*,hash_guid_compare>* GetOriginPackGoodsMap(){return m_cOriginPack->GetGoodsMap();}
	//hash_map<CGUID,CGoods*,hash_guid_compare>* GetSubPackGoodsMap(int i);
	//map<eEquipType,CGoods*>* GetEquipmentGoodsMap(){return m_cEquipment.GetGoodsMap();}

	//! �ֿ����
	////////////////////////////////////////////////////////////////////////////////////////////
public:
	PlayerDepot*				GetPlayerDepot(void);

private:
	//! �ֿ����
	PlayerDepot					m_pPlayerDepot;
	////////////////////////////////////////////////////////////////////////////////////////////

public:	

	//##����ϵ���Ʒ
	//CAmountLimitGoodsContainer	m_cHand;

	//##װ��
	CEquipmentContainer			m_cEquipment;
    CSpecialEquipContainer      m_cEquipmentGod;
    CSpecialEquipContainer      m_cEquipmentLand;
    CSpecialEquipContainer      m_cEquipmentHuman;


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
	void						DropParticularGoodsWhenDead();
	void						DropParticularGoodsWhenLost();
	void						DropParticularGoodsWhenRecall();

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

	//�ʼ��б�	
	//map<CGUID,CMail*> m_mapMail;
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
	bool				AddReceiveMailList(CGUID &MailGuid);
	void				DeleteReceiveMailList(CGUID &MailGuid);


	//map<CGUID,CMail*>	&GetMapMail()				{return m_mapMail;}
	//bool				SetMapMail(CMail *pMail);
	//void				DeleteMail(list<CGUID> &lguid);		//�������ɾ��
	//bool				CheckDeleteMail(list<CGUID> &lguid);//���ɾ��
	//void				DeleteMailBySys(list<CGUID> &lguid);//ϵͳɾ��
	//CMail				*GetMail(CGUID &guid);
	//void				ClearMail();

	/////////////////////////////////////////////////////////////////////
	// ���Դ�ȡ�ӿ�
	/////////////////////////////////////////////////////////////////////
private:
	DWORD	m_dwMurdererTimeStamp;

	//2009.01.07 ahc �޸Ĺ���ֵΪ0��BUG
	//BYTE	m_btCountry;				// ���ұ��
	//long	m_lContribute;			// �Թ��ҵĹ���ֵ
	long	m_lPKTimerID;			//PK��ʱ��id
	long	m_lPvPTimerID;			//PVP��ʱ��id
	long	m_lHpMpTimerID;			//�Զ��ָ�Hp,Mp��ʱ��id
	//long	m_lEnergyTimerID;		//����ֵ�ָ��Ķ�ʱ��ID
	long	m_lReliveTimerID;		//�����سǸ���Ķ�ʱ��ID

public:
	void	RegisterReliveTimerID();			//����ע��سǸ��ʱ��
	void	UnRegisterReliveTimerID();			//����ע���سǸ��ʱ��


public:

	////////////////////////////////////////////////////////////////////////////////
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
	void   SetLevel(WORD l, BOOL bScriptSet);//				{ m_Property.wLevel = l; }

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
	BYTE	GetHeadPic()				{return m_Property.byHairPic;}
	void	SetHeadPic(BYTE l)			{m_Property.byHairPic = l;}
	//�沿ͼƬ
	BYTE	GetFacePic()				{return m_Property.byFacePic;}
	void	SetFacePic(BYTE l)			{m_Property.byFacePic = l;}
	//ְҵ	
	BYTE	GetOccupation()				{return m_Property.byOccu;}
	void	SetOccupation(eOccupation l)		{m_Property.byOccu = l;}
	//��ְҵ
	BYTE	GetDOccupation()			{return m_Property.byAssOccu;}
	void	SetDOccupation(eDeputyOccu l)	{m_Property.byAssOccu=l;}
	//�Ա�
	BYTE	GetSex()					{return m_Property.bySex;}
	void	SetSex(BYTE l)				{m_Property.bySex = l;}
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
	void	SetHP(DWORD l);				//{m_Property.dwHp = l;if(m_Property.dwHp > GetMaxHP()){m_Property.dwHp = GetMaxHP();}}
	//MP
	WORD	GetMP()						{return m_Property.wMp;}
	void	SetMP(WORD l);//				{m_Property.wMp = l;if(m_Property.wMp > GetMaxMP()) {m_Property.wMp = GetMaxMP();}}
	//�ȼ�
	DWORD	GetHotKey(long lpos){if(lpos < 0){lpos = 0;}else if(lpos > 23){lpos = 23;}return m_Property.dwHotKey[lpos];}
	void	SetHotKey(long lpos,DWORD l){if(lpos < 0){lpos = 0;}else if(lpos > 23){lpos = 23;}m_Property.dwHotKey[lpos] = l;}

	//PK����(b0:ȫ�屣��)
	BOOL	IsPk_Normal();// 				{return (m_Property.byPKOnOff && 0x01);}
	void	SetPk_Normal(bool b);//		{if(b==true)m_Property.byPKOnOff = m_Property.byPKOnOff | 0x01;else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xfe; }
	//b1:��ӱ���
	BOOL	IsPk_Team();//					{return (m_Property.byPKOnOff && 0x02);}
	void	SetPk_Team(bool b);//			{if(b==true)m_Property.byPKOnOff = m_Property.byPKOnOff | 0x02;else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xfd; }
	//b2:���ᱣ��,
	BOOL	IsPk_Union();//				{return (m_Property.byPKOnOff && 0x04);}
	void	SetPk_Union(bool b);//			{if(b==true)m_Property.byPKOnOff = m_Property.byPKOnOff | 0x04;else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xfb; }
	//b3:��������
	BOOL	IsPk_Badman();//				{return (m_Property.byPKOnOff && 0x08);}
	void	SetPk_Badman(bool b);//		{if(b==true)m_Property.byPKOnOff = m_Property.byPKOnOff | 0x08;else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xf7; }
	//b4:��������
	BOOL	IsPk_Country();//				{return (m_Property.byPKOnOff && 0x10);}
	void	SetPk_Country(bool b);//		{if(b==true)m_Property.byPKOnOff = m_Property.byPKOnOff | 0x10;else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xef; }
	//b5:��Ӫ����
	BOOL	IsPk_Camp();//					{return (m_Property.byPKOnOff && 0x12);}
	void	SetPk_Camp(bool b);//			{if (b==true)m_Property.byPKOnOff = m_Property.byPKOnOff | 0x12;else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xed;}
	//b6.��������
	BOOL	IsPk_Pirate();//				{return (m_Property.byPKOnOff && 0x14);}
	void	SetPk_Pirate(bool b);//		{if (b==true)m_Property.byPKOnOff = m_Property.byPKOnOff | 0x14;else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xeb;}



	//void	SetPKOnOff(int i, bool b)	{m_Property.byPkOnOff}



	/*BYTE	GetPKOnOff()				{return m_Property.byPKOnOff;}
	void	SetPKOnOff(BYTE l)			{m_Property.byPKOnOff += l;}*/

	//����ֵ
	WORD	GetEnergy()						{return m_Property.wEnergy;}
	void	SetEnergy(WORD l);//				{m_Property.wEnergy = l; if(m_Property.wEnergy > GetMaxEnergy()) m_Property.wEnergy = GetMaxEnergy();}
	//��ǰ����
	WORD	GetStamina()				{return m_Property.wStamina;}
	void	SetStamian(WORD l)			{m_Property.wStamina = l;}
	//����ID
	BYTE	GetCountry() const			{return m_Property.byCountry;}
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
	//long GetAllEquipTimesCP();
	///////////// PK CP  ////////////////////////////////////////

	//�ɼ���
	//	BYTE    GetVisibility()             {return m_Property.byVisibility;}
	//	void    SetVisibility(BYTE l)       {m_Property.byVisibility=l;}

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
	void	SetMaxHP(DWORD l);//					{m_mapDirtyAttrData.push_back("wMaxHP"); m_FightProperty.dwMaxHp = l; if(GetHP()>GetMaxHP()) SetHP(GetMaxHP());}
	//���MP
	WORD	GetMaxMP()
	{
		long lMaxMp = m_FightProperty.wMaxMp+m_BaseFightProperty.wMaxMp;
		if(lMaxMp<1)	return 1;
		return (WORD)lMaxMp;
	} 
	void	SetMaxMP(WORD l);//						{m_mapDirtyAttrData.push_back("wMaxMP"); m_FightProperty.wMaxMp = l; if(GetMP()>GetMaxMP()) SetMP(GetMaxMP());}
	//��������
	WORD	GetMaxStamina()							{return m_FightProperty.wMaxStamina;}
	void	SetMaxStamina(WORD l);//					{m_mapDirtyAttrData.push_back("wMaxStamina"); m_FightProperty.wMaxStamina = l;if(GetStamina()>GetMaxStamina()) SetStamian(GetMaxStamina());}
	//�������ֵ
	WORD	GetMaxEnergy()							{return m_FightProperty.wMaxEnergy;}
	void	SetMaxEnergy(WORD l);//					{m_mapDirtyAttrData.push_back("wMaxEnergy"); m_FightProperty.wMaxEnergy = l; if(GetEnergy()>GetMaxEnergy()) SetEnergy(GetMaxEnergy());}

	//��С������
	WORD	GetMinAtk()								{return (WORD)m_FightProperty.wMinAtk;}
	void	SetMinAtk(WORD l);	//					{m_mapDirtyAttrData.push_back("wMinAtk"); m_FightProperty.wMinAtk = l;}
	//��󹥻���
	WORD	GetMaxAtk()								{return (WORD)m_FightProperty.wMaxAtk;}
	void	SetMaxAtk(WORD l);	//					{m_mapDirtyAttrData.push_back("wMaxAtk"); m_FightProperty.wMaxAtk = l;}
	//��������
	WORD	GetHit()								{return m_FightProperty.wHit;}
	void	SetHit(WORD l);			//				{m_mapDirtyAttrData.push_back("wHit"); m_FightProperty.wHit = l;}
	//�������
	WORD	GetDef()								{return (WORD)m_FightProperty.wDef;}
	void	SetDef(WORD l);			//				{m_mapDirtyAttrData.push_back("wDef"); m_FightProperty.wDef = l;}
	//����
	WORD	GetDodge()								{return m_FightProperty.wDodge;}
	void	SetDodge(WORD l);		//				{m_mapDirtyAttrData.push_back("wDodge"); m_FightProperty.wDodge = l;}
	//Ԫ�ؿ���
	WORD	GetElementResistant(long lpos)			{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_FightProperty.wElemDef[lpos];}
	void	SetElementResistant(long lpos,WORD l);	
	//�����ָ��ٶ�
	WORD	GetHpRecoverSpeed()						{return m_FightProperty.wHpRecoverSpeed;}
	void	SetHpRecoverSpeed(WORD l);//				{m_mapDirtyAttrData.push_back("wHpRecoverSpeed"); m_FightProperty.wHpRecoverSpeed = l;}
	//�����ָ��ٶ�
	WORD	GetMpRecoverSpeed()						{return m_FightProperty.wMpRecoverSpeed;}
	void	SetMpRecoverSpeed(WORD l);//				{m_mapDirtyAttrData.push_back("wMpRecoverSpeed"); m_FightProperty.wMpRecoverSpeed = l;}
	//�����ָ��ٶ�
	WORD	GetStamRecoverSpeed()					{return	m_FightProperty.wStamRecoverSpeed;}
	void	SetStamRecoverSpeed(WORD l)	;//			{m_mapDirtyAttrData.push_back("wMaxHP"); m_FightProperty.wStamRecoverSpeed = l;}

    short   GetStr()                                { return m_FightProperty.wStr; }
    void    SetStr(short s);

    short   GetDex()                                { return m_FightProperty.wDex; }
    void    SetDex(short s);
    //����
    short   GetCon()                                { return m_FightProperty.wCon; }
    void    SetCon(short s);
    //��־
    short   GetIntellect()                          { return m_FightProperty.wIntellect; }
    void    SetIntellect(short s);
    //����
    short   GetSpiritualism()                       { return m_FightProperty.wSpiritualism; }
    void    SetSpiritualism(short s);
    //Ǳ�ܵ�
    short   GetPoint()                              { return m_FightProperty.wPoint; }
    void    SetPoint(short s);
	//��
	WORD	GetFightBlock()							{return m_FightProperty.wFightBlock;}
	void	SetFightBlock(WORD l)	;//				{m_mapDirtyAttrData.push_back("wBlock"); m_FightProperty.wFightBlock = l;}
	//�м�
	WORD	GetParry()								{return m_FightProperty.wParry;}
	void	SetParry(WORD l)	;//					{m_mapDirtyAttrData.push_back("wParry"); m_FightProperty.wParry = l;}
	//���
	/*WORD	GetShot()								{return	m_FightProperty.wShot;}
	void	SetShot(WORD l)							{m_FightProperty.wShot = l;}*/
	//����
	WORD	GetCharm()								{return	m_FightProperty.wCharm;}
	void	SetCharm(WORD l)	;//					{m_mapDirtyAttrData.push_back("wCharm"); m_FightProperty.wCharm = l;}
	//רע
	WORD	GetAbsorb()								{return	m_FightProperty.wAbsorb;}
	void	SetAbsorb(WORD l)	;//					{m_mapDirtyAttrData.push_back("wAbsorb"); m_FightProperty.wAbsorb = l;}
	//ħ������
	WORD	GetMdef()								{return (WORD)m_FightProperty.wMdef;}
	void	SetMdef(WORD l)		;//					{m_mapDirtyAttrData.push_back("wMdef"); m_FightProperty.wMdef = l;}
	//������
	WORD	GetCri()								{return	(WORD)m_FightProperty.wCri;}
	void	SetCri(WORD l)		;//					{m_mapDirtyAttrData.push_back("wCri"); m_FightProperty.wCri = l;}

	WORD	GetMAtkSpeed()							{return m_FightProperty.wMAtkSpeed;}
	void	SetMAtSpeed(WORD l)	;//					{m_mapDirtyAttrData.push_back("wMAtkSpeed"); m_FightProperty.wMAtkSpeed = l;}
	//Buff����ǿ��
	WORD	GetBuffPower()							{return m_FightProperty.wBuffPower;}
	void	SetBuffPower(WORD l)	;//				{m_mapDirtyAttrData.push_back("wBuffPower"); m_FightProperty.wBuffPower = l;}
	//ħ��������
	WORD	GetMAtk()								{return (WORD)m_FightProperty.wMAtk;}
	void	SetMAtk(WORD l)			;//				{m_mapDirtyAttrData.push_back("wMAtk"); m_mapDirtyAttrData.push_back("wMaxHP"); m_FightProperty.wMAtk = l;}
	//ħ��������
	WORD	GetMCri()								{return	(WORD)m_FightProperty.wMCri;}
	void	SetMCri(WORD l)			;//				{m_mapDirtyAttrData.push_back("wMCri"); m_FightProperty.wMCri = l;}
	//����������
	WORD	GetBloodSuk()							{return m_FightProperty.wBloodSuck;}
	void	SetBloodSuk(WORD l)		;//				{m_mapDirtyAttrData.push_back("wBloodSuk"); m_FightProperty.wBloodSuk = l;}
	//����������
	WORD	GetManaSuck()							{return m_FightProperty.wManaSuck;}
	void	SetManaSuck(WORD l)		;//				{m_mapDirtyAttrData.push_back("wManaSuck"); m_FightProperty.wManaSuck = l;}
	//��־����
	WORD	GetWillDef()							{return	m_FightProperty.wWillDef;}
	void	SetWillDef(WORD l)		;//				{m_mapDirtyAttrData.push_back("willDef"); m_FightProperty.wWillDef = l;}
	//���ʿ���
	WORD	GetConstiDef()							{return m_FightProperty.wConstiDef;}
	void	SetConstiDef(WORD l)	;//				{m_mapDirtyAttrData.push_back("wConstiDef"); m_FightProperty.wConstiDef = l;}
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
	void	SetFightHPRecoverSpeed(WORD l)	;//		{m_mapDirtyAttrData.push_back("wFightHPRecoverSpeed"); m_FightProperty.wFightHpRecoverSpeed = l;}
	//ս��ʱ�����ָ��ٶ�
	WORD	GetFightMPRecoverSpeed()				{return m_FightProperty.wFightMpRecoverSpeed;}
	void	SetFightMPRecoverSpeed(WORD l)	;//		{m_mapDirtyAttrData.push_back("wFightMPRecoverSpeed"); m_FightProperty.wFightMpRecoverSpeed = l;}
	//�����ָ��ٶ�
	short	GetEnterRecoverSpeed()					{return m_FightProperty.wEnerRecoverSpeed;}
	void    SetEnterRecoverSpeed(WORD l)	;//		{m_mapDirtyAttrData.push_back("wtEnterRecoverSpeed"); m_FightProperty.wEnerRecoverSpeed = (short)l;}
	//ս�������ָ��ٶ�
	short	GetFightEnterRecoverSpeed()				{return m_FightProperty.wFightEnerRecoverSpeed;}
	void	SetFightEnterRecoverSpeed(WORD l);//		{m_mapDirtyAttrData.push_back("wFightEnterRecoverSpeed"); m_FightProperty.wFightEnerRecoverSpeed = (short)l;}
	//����������
	float	GetEnergySuck()							{return m_FightProperty.fEnergySuck;}
	void	SetEnergySuck(float f)			;//		{m_mapDirtyAttrData.push_back("wEnergySuck"); m_FightProperty.wEnergySuck = l;}
	//����������
	WORD	GetCriDef()								{return m_FightProperty.wCriDef;}
	void    SetCriDef(WORD l)				;//		{m_mapDirtyAttrData.push_back("wCriDef"); m_FightProperty.wCriDef = l;}
	//ħ����������
	WORD	GetMCriDef()							{return m_FightProperty.wMCriDef;}
	void	SetMCriDef(WORD l)				;//		{m_mapDirtyAttrData.push_back("wMCriDef"); m_FightProperty.wMCriDef = l;};



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
	void	SetBaseMaxEnergy(WORD l);					//{m_BaseFightProperty.wMaxEnergy = l; if(GetEnergy()>GetMaxEnergy()) SetEnergy(GetMaxEnergy());}

	//��С������
	WORD	GetBaseMinAtk()								{return (WORD)m_BaseFightProperty.wMinAtk;}
	void	SetBaseMinAtk(WORD l)						{m_BaseFightProperty.wMinAtk = l;}
	//��󹥻���
	WORD	GetBaseMaxAtk()								{return (WORD)m_BaseFightProperty.wMaxAtk;}
	void	SetBaseMaxAtk(WORD l)						{m_BaseFightProperty.wMaxAtk = l;}
	//��������
	WORD	GetBaseHit()								{return m_BaseFightProperty.wHit;}
	void	SetBaseHit(WORD l)							{m_BaseFightProperty.wHit = l;}
	//�������
	WORD	GetBaseDef()								{return (WORD)m_BaseFightProperty.wDef;}
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
	short	GetBaseStr()							    {return m_BaseFightProperty.wStr;}
	void	SetBaseStr(short s)						    {m_BaseFightProperty.wStr = s;}
    //��
    short   GetBaseDex()                                {return m_BaseFightProperty.wDex;}
    void    SetBaseDex(short s)                         {m_BaseFightProperty.wDex = s;}
	//����
	short	GetBaseCon()								{return m_BaseFightProperty.wCon;}
	void	SetBaseCon(short s)						    {m_BaseFightProperty.wCon = s;}
	//��־
	short	GetBaseIntellect()							{return	m_BaseFightProperty.wIntellect;}
	void	SetBaseIntellect(short s)					{m_BaseFightProperty.wIntellect = s;}
	//����
	short	GetBaseSpiritualism()						{return	m_BaseFightProperty.wSpiritualism;}
	void	SetBaseSpiritualism(short s)				{m_BaseFightProperty.wSpiritualism = s;}
	//��
	WORD	GetBaseBlock()								{return m_BaseFightProperty.wFightBlock;}
	void	SetBaseBlock(WORD l)						{m_BaseFightProperty.wFightBlock = l;}
	//�м�
	WORD	GetBaseParry()								{return m_BaseFightProperty.wParry;}
	void	SetBaseParry(WORD l)						{m_BaseFightProperty.wParry = l;}
	//���
	/*WORD	GetBaseShot()								{return	m_BaseFightProperty.wShot;}
	void	SetBaseShot(WORD l)							{m_BaseFightProperty.wShot = l;}*/
	//����
	WORD	GetBaseCharm()								{return	m_BaseFightProperty.wCharm;}
	void	SetBaseCharm(WORD l)						{m_BaseFightProperty.wCharm = l;}
	//רע
	WORD	GetBaseAbsorb()								{return	m_BaseFightProperty.wAbsorb;}
	void	SetBaseAbsorb(WORD l)						{m_BaseFightProperty.wAbsorb = l;}
	//ħ������
	WORD	GetBaseMdef()								{return (WORD)m_BaseFightProperty.wMdef;}
	void	SetBaseMdef(WORD l)							{m_BaseFightProperty.wMdef = l;}
	//������
	WORD	GetBaseCri()								{return	(WORD)m_BaseFightProperty.wCri;}
	void	SetBaseCri(WORD l)							{m_BaseFightProperty.wCri = l;}
	//ʩ���ٶ�
	WORD	GetBaseMAtkSpeed()							{return m_BaseFightProperty.wMAtkSpeed;}
	void	SetBaseMAtSpeed(WORD l)						{m_BaseFightProperty.wMAtkSpeed = l;}
	//Buff����ǿ��
	WORD	GetBaseBuffPower()							{return m_BaseFightProperty.wBuffPower;}
	void	SetBaseBuffPower(WORD l)					{m_BaseFightProperty.wBuffPower = l;}
	//ħ��������
	WORD	GetBaseMAtk()								{return (WORD)m_BaseFightProperty.wMAtk;}
	void	SetBaseMAtk(WORD l)							{m_BaseFightProperty.wMAtk = l;}
	//ħ��������
	WORD	GetBaseMCri()								{return	(WORD)m_BaseFightProperty.wMCri;}
	void	SetBaseMCri(WORD l)							{m_BaseFightProperty.wMCri = l;}
	//����������
	WORD	GetBaseBloodSuk()							{return m_BaseFightProperty.wBloodSuck;}
	void	SetBaseBloodSuk(WORD l)						{m_BaseFightProperty.wBloodSuck = l;}
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
	float	GetBaseEnergySuck()							{return m_BaseFightProperty.fEnergySuck;}
	void	SetBaseEnergySuck(float f)					{m_BaseFightProperty.fEnergySuck = f;}

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


	void	IncExp(LONG64 llExp, DWORD dwAddType);		//��������Ӿ���
	//!		��Ӿ�����־
	void	AddIncExpLog(DWORD dwOldExp, DWORD dwOldExpMultiple, DWORD dwAddExp, DWORD dwAddExpMultiple, DWORD dwAddType, const char *pSrcName);
	void	IncOccuSp(eOccupation eOccu,long lSp);		//��������Ӽ��ܵ���
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
	void	SetYuanbao(DWORD dwVal);	

	////////////////////! �ж���//////////////////
	//!		�õ���һ�����
	long	GetActive(void);
	//!		������һ�����
	void	ResetActive(DWORD dwCurrTime);
	//!		�����չ������
	bool	AddExActive(long lAddNum);
	//!		�۳�������
	bool	TakeActive(long lTakeNum);

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
	void	OnDecreaseMurdererSign		();
	void	OnUpdateMurdererSign		();
	void	OnClearMurdererSign			();

	void	OnDecreasePKValue			();//��pkֵ
	void	OnDecreasePvPValue			();//��pvpֵ
	void	OnIncreaseHpMpValue			();//�Զ�����hp,mp				
	void	RegisterPKValue				();//pkֵע�ᶨʱ��
	void	RegisterPvPValue			();//pvpֵע�ᶨʱ��
	void	RegisterAutoReNewHpMp		();//�Զ��ָ�Hp,Mp��ʱ��
	void	UnRegisterPKValue			();//pkֵע���ᶨʱ��
	void	UnRegisterPvPValue			();//pvpֵע����ʱ��
	void	UnRegisterAutoReNewHpMp		();//ע���Զ��ָ�Hp,Mp��ʱ��
	//void	RegisterEnergyUpdate		(long lUpdateTime);//ע������ֵ�ָ�
	//void	UnRegisterEnergyUpdate		();//ע������ֵ�ָ�
	//void	OnUpdateEnergy				();//��������ֵ
	//void	UpdateEnergyTimer			();//����������ʱ��
	void	UpdateHpMpRecoverSpeed		();//����HPMP�ָ��ٶ�
	//  [1/20/2009 chenxianj]
	void    ChangeRgnToIsland			();//����������
	bool	ChangeRgnToVillage			();//����������
	void	PostCountryInfo				(const char *strName);//���͹��ҹ���
	//void	OnMailTimeOut				(const tagTimerVar* pTimerVar);//�ʼ���ʱ����


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

	void    MountAllEquip();
	/// 9.28.2009 ����bGuardianFlagר����װ���������ػ�ʯ������Ʒ�������־
	void	MountEquipEx(CGoods *pGoods,bool bMount, bool bGuardianFlag = false );
	void	UseItem(WORD wPlace,BYTE lPos,long lTargetType,CGUID& lTargetGuid);	//ʹ�������Ե��ߵ��߼�
	void	UseItem(DWORD dwContainerId,DWORD dwPos);
	BOOL	AutoUseItem(WORD wPlace,BYTE lPos,long lTargetType,CGUID& lTargetGuid);	//! �Զ�ʹ�õ���
	long	CanMountEquip(CGoods *pGoods); //�Ƿ���װ��һ������
	long    CanMountEquip(CGoods *pGoods,DWORD dwPos,BOOL bMsgNotify=FALSE);
	bool	CanMountSubpack(CGoods* pGoods);
	long	CanUseItem(CGoods* pGoods);	//�Ƿ���ʹ��һ�������Ե���
	//long	GetCurBurden();//���㵱ǰ����
	//bool	CheckBurden(CGoods* pGoods);//��⸺��
	void    SendEquipNotifyMsg(long lVal);

	// ͨ���ú�����update��ɫ��װ����Ʒ�ϵĸ�ħ����
	// ������ ������������ ������ֵ
	int	MountFuMoProperty( eGoodsAddonProperties type, int value );

	//##ŭֵ������
	virtual void	IncreaseRp( BOOL bAttack/*##��־�Ƿ��ǹ��������Ϊfalse���Ǳ�����*/, WORD wLoseHP );

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
	//virtual CVariableList* GetVariableList()	{ return m_pVariableList; }
private:
	// ��ҵ�˽�нű������б�
	//CVariableList* m_pVariableList;
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
	void CodeGoodsCooldown(DBWriteSet& setWriteDB, bool bExData=true);
    void DecodeGoodsCooldown(DBReadSet& setReadDB);
    void CodeGoodsColldownForClient(DBWriteSet& setWriteDB);


	//ս�����ߵ�ʱ���
	DWORD												m_dwLostTimeStamp;
	void PetsChangeRegionLocal(INIT_TYPE lInitType);
	bool ChangeRegion(eRgnType type, const CGUID& lRegionID, long x, long y, long dir, long templateID=0, long lUseGoods=0, long lRange=0, long lCarDistance=0);

	//bool IsInChangingServer()			{return m_bInChangingServer;}
	//void SetInChangingServer(bool b)	{m_bInChangingServer=b;}

	//�ƶ�ʱ����
	void OnMove(float fNewX, float fNewY);
	// ���󸴻�(0:��ȫ�س� 1:ԭ�ظ���)
	virtual void OnRelive(long lType,long lRecoverRatio=50);

	//����ҵȼ������仯��ʱ�򴥷�����Ҫָ��������
	void OnLevelChanged();
	//�����ְҵ�ȼ��������仯��ʱ����Ҫָ��ְҵ�ȼ���ʱ��
	void OnOccuLevelChanged();

	//##����ĳ�������󣬳������Զ������������֪ͨ������볡���ɹ���
	//##�����ٻ���Ҳ���������.
	virtual void OnEnterRegion(bool bIsRelive=false);
	virtual void OnExitRegion();

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
	/// ȡ����ҳ����б�
	C_PETS GetPets(void)  { return m_Pets; }
	/// ȡ����ҳ�������
	long GetPetsNum(void) { return (long)m_Pets.size(); }
	/// ȡ����ҳ�ս�����б�
	C_ACT_PETS GetFightPets(void) { return m_FightPets; }
	/// ��������
	CPet* CreatePet(const CGUID& rID);
	/// ���ݳ���GUIDȡ�ó������
	CPet* GetPet(const CGUID& rID);
	/// ��ӳ���
	void AddPet(CPet* pPet);
	/// ����GUIDɾ������
	bool DelPet(const CGUID& rID);
	/// ��ճ����б�
	void ReleaseAllPets(void);

	void ActivatePet(bool bActive = true);

	/// ���ý������GUID
	void SetFocusPetGUID(CGUID guid);
	/// ȡ�ý������GUID
	const CGUID& GetFocusPetGUID(void);

	/// ȡ�ý���������
	CPet* GetFocusPet();

	/// ���ü������GUID
	void SetActivePetGUID(CGUID guid);
	/// ȡ�ü������GUID
	const CGUID& GetActivePetGUID(void);

	/// ȡ�ü���������
	CPet* GetActivePet();

	/// ����GUIDȡ�ó�ս�������
	CPet* GetFightPet(const CGUID& guid);

	/// ��ӳ�ս����
	bool AddFightPet(CPet* pPet);
	void AddFightPet(const CGUID& guid);

	/// ɾ����ս����
	void DelFightPet(const CGUID& guid);

	void CheckPetsMove(void);

	/// �޸ļ������״̬
	void ChangeActivePetState(long lState);

	/// �����̴�
	bool CatchPet(CMonster* pCatchTarget);

private:
	void DelPet(ITR_PET& itr);					// ���ɾ������

private:
	PETS					m_Pets;				// ���г����б�
	ACT_PETS				m_FightPets;		// ��ս״̬�����б�
	CGUID					m_FocusPet;			// ���������ѡ�������ǿ�������ĳ��
	CGUID					m_ActivePet;		// ������������õ����ڸ���ս���ĳ��

protected:
	bool m_bInChangingRegion;			// �Ƿ����л�����
	//bool m_bInChangingServer;			// �Ƿ����л�������

	/////////////////////////////////////////////////////////////////////
	// ��Ϣ��Ӧ
	/////////////////////////////////////////////////////////////////////
public:
	virtual void OnMessage(CMessage*);
	long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//long Proc(eEventType type,const tagProcVar *pVar);

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
	virtual void	OnCannotMove		();
	virtual void EndBuff(DWORD dwStateID);//����ID����״̬

	void SetPosition( LONG lX, LONG lY );

	void SetSlipPos( LONG lX, LONG lY ,float fSpeed, float fDir);

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

	//BOOL			UseGoodsEnlargeMaxHp( DWORD dwCoefficient, DWORD dwPersistTime);
	//BOOL			UseGoodsEnlargeMaxMp( DWORD dwCoefficient, DWORD dwPersistTime);

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
	void					OnWeaponDamaged		();
	//##���ߵ����
	void					OnArmorDamaged		();
	//##������ĵ�ͨ�ú���
	void					OnEquipmentWaste	( eEquipType ec, CGoods* pEquipment );

	virtual void			OnDied				();								// ��������
	//##���������AI����
	virtual void			OnBeenHurted		( LONG lAttackerType, const CGUID& AttackerID );
	virtual void			OnBeenMurdered		( LONG lMurdererType, const CGUID& MurdererGuid );
	void					ReportMurderer		( const CGUID& MurdererID );

	//void					SetSinStateCount	(long l)	{m_lSinStateCount=l;}
	//LONG					GetSinStateCount	()			{return m_lSinStateCount;}

	bool					IsBadman			();

	//##һЩս����ص�״̬����
	virtual void			EnterCriminalState	();
	virtual void			EnterResidentState	();
	//	virtual void			EnterCombatState	();
	//	virtual void			EnterPeaceState		();
	//##���ս��/��ƽ,�ﷸ/������ҵ�״̬.
	virtual void			UpdateCurrentState	();

	// ͨ����ұ�������Ϣ
	void		SendNotifyWarningMsg(bool isKilled, CPlayer* pAttacker);

protected:
	long					m_lFightStateCount;				// ����״̬������(�ݼ�����0��ʾ��������״̬)
	//long					m_lSinStateCount;				// ����״̬������(�ݼ�����0��ʾ��������״̬)
	DWORD					m_dwSinStateTimeStamp;

	DWORD					m_dwContinuousKillAmount;		// ��ǰ����������
	DWORD					m_dwContinuousKillTimeStamp;	// ��һ�λ��е�ʱ��

	long					m_lFightStateTimerID;//ս��״̬��ʱ��
public:

	//##����������
	void					IncreaseContinuousKill();

	virtual FLOAT			GetWeaponModifier( LONG lTargetLevel );


	//////////////////////////////////////////////////////////////////////////
	// ���
	//////////////////////////////////////////////////////////////////////////

private:
	CGUID m_TeamID;
	bool m_bCaptain;
	tagRecruitState m_RecruitState;

public:
	const CGUID& GetTeamID() const { return m_TeamID; }
	void SetTeamID(const CGUID& guid) { m_TeamID = guid;}
	void SetCaptain(bool b) { m_bCaptain = b; }
	bool GetCaptain(void) { return m_bCaptain; }

	virtual	void	OnAddBuff(DWORD dwStateID, DWORD dwBuffLevel);
	virtual	void	OnDelBuff(DWORD dwStateID, DWORD dwBuffLevel);

	void SetRecruitState(bool bIsRecruiting, char* pRecruitName, char* pPassword);
	tagRecruitState& GetRecruitState(void);

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
	bool SPJudgeStudy(DWORD dwID,DWORD dwLv);
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

	virtual void OnChangeStates();								// �ı�״̬�󣬷�����Ϣ�����
	LONG GetNumSkills();										// ��ȡ���ܵĸ���
	LONG GetSkillLv(long lSkillID);

	void CodeSkillsToDataBlock_ForClient(DBWriteSet& setWriteDB, BOOL bAddRestoreTime = FALSE );	// �����еļ�����ӵ�����	
    void CodeSkillsToDataBlock(DBWriteSet& setWriteDB);
	void DecodeSkillsFromDataBlock(DBReadSet& setReadDB);	// �������еļ������ݽ���

    void CodeGeniusToDataBlock_ForClient(DBWriteSet& setWriteDB, BOOL bAddRestoreTime = FALSE );
    void CodeGeniusToDataBlock(DBWriteSet& setWriteDB);
    void DecodeGeniusFromDataBlock(DBReadSet& setReadDB);	// �������е��츳���ݽ���

	virtual long AddActiveSkill(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget=NULL,long lGoodsIndex=0,long lContainerID=0,long lPos=0,CGUID GoodsGuid=NULL_GUID);
	virtual BOOL DelSkill( long lSkillID ,long lSkillLv);
	void DeleteGoodsInPacket(char* strGoodName);

    void CodeBrotherToDataBlock(DBWriteSet& setWriteDB);
    void DecodeBrotherFromDataBlock(DBReadSet& setReadDB);
private:

	//������еļ���
	map<long,tagSkill> m_Skills;

    map<CGUID, tagBrotherInfo>    m_mapBrotherInfo;      //�������
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
	//LONG					m_lClientCurrFacIconCRC32Code;			//! ��¼�ѷ��͸��ͻ��˵�ͼ�����ݵ�Ч��ֵ
	char					m_szFactionName[MAXSIZE_FactionName];	//! �������
	char					m_szUnionName[MAXSIZE_FactionName];		//! ��������
	LONG					m_lFacDemiseWarning;					//! ��ᴫλ���ѵ���ʱ
	LONG					m_lFacDisbandWarning;					//! ����ɢ���ѵ���ʱ

public:


	const CGUID&			GetFactionID(void) const						{return m_FactionID;}
	void					SetFactionID(const CGUID& FactionID);
	void					SetFactionInfo(GameFaction *pFaction);
	const CGUID&			GetUnionID(void)								{return m_UnionID;}
	void					SetUnionID(const CGUID& UnionID);
	const char*				GetFactionName()								{ return m_szFactionName; }
	const char*				GetUnionName()									{ return m_szUnionName; }
	const tagFacMemInfo*	GetFacMemInfo(void)								{return m_pFacMemInfo;}
	void					SetFacMemInfo(const tagFacMemInfo* pFacMemInfo)	{m_pFacMemInfo = pFacMemInfo;}
	//LONG					GetFacIconCRC32Code(void)						{return m_lClientCurrFacIconCRC32Code;}
	//void					SetFacIconCRC32Code(LONG lCRC32Code)			{m_lClientCurrFacIconCRC32Code = lCRC32Code;}



	long	GetNetExID()
	{
		static long lID = 0;
		return ++lID;
	}

	//��Ʒ��������
	bool	ItemBeginSkill(DWORD dwID, DWORD dwLevel, CMoveShape *pDestShape);
	int     JudgeSkillBegining(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget=NULL,long lGoodsIndex=0);//��������ǰ���ж�

	/*
	public:

	struct tagPlayerQuest
	{
	WORD	wQuestID;				//�����ID
	BYTE	byComplete;				//�Ƿ����
	};
	private:
	map<WORD,tagPlayerQuest>	m_PlayerQuests;	//����ѻ�õ������б�
	bool	m_bLogin;					//��ʾ����Ƿ��ѵ�½
	*/
private:
	CQuestQueue m_RoleQueue;
	//ٸ������
	CMercenaryQuestSys	m_MerQuestSys;
	//ˢ��ٸ������Ķ�ʱ��
	//long	m_lMercQuestUpdateTimerID;
	//�ϴ������ٸ�������б�ID
	//long	m_lLastAcquireMercListID;
	bool       m_bLogin;
public:
	bool CodeQuestDataToDataBlock(DBWriteSet& setWriteDB);		//����������ݵ�ByteArray
	bool CodeQuestDataToDataBlock_ForClient(DBWriteSet& setWriteDB);//����������ݵ�ByteArray
	bool DecodeQuestDataFromDataBlock(DBReadSet& setReadDB);

	eQuestState GetQuestState(LONG wQuestID);					//�õ�һ�������״̬
	long GetValidQuestNum();									//�õ���Ч����������
	BOOL AddQuest(LONG lQuestID);								//����������һ������
	BOOL AddQuest(CQuestRecord*& p);								//���һ������

	bool CompleteQuest(DWORD dwQuestID);						//��������һ������
	void RemoveQuest(LONG lQuestID);							//�Ƴ�һ������
	void UpdateQuestStep(DWORD dwQuestID,long lStep);			//����һ���������Ч��ʾ����
	void UpdataQuestData(DWORD dwQuestID,long lParamIndex,long lVal);
	void UpdataQuestParamData(DWORD dwQuestID,long lParamIndex, long lParam,long RegionID,long RegionX, long RegionY);
	void ShowTalkBox(DWORD wQuestID,long lTextStep);
	void PlayerRunScript(const char *strScriptName);			//�����������һ���ű�
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
	//��������ٸ������
	//void ResetAllMercQuestData(void);
	//����ٸ������ˢ��ʱ��
	void SetMercQuestTime(long lMercListID);
	DWORD GetMercQuestTime(long lMercListID);
	//ٸ������ˢ��ʱ�䳬ʱ
	//void OnUpdateMercQuestTimeOut(void);
	//ٸ������
	long GetMercQuest(){return m_MerQuestSys.GetCurQuestID();}
	void SetMercQuest(long id,bool bcall=true);//{ m_MerQuestSys.SetCurQuestID(id);}
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

	// ����Ƿ��¼��GS��־�����������Զ�̴���������ɵĴ�����Ҷ��������ۻ�
	bool	GetPlayerLogin()		{return m_bLogin;}
	void	SetPlayerLogin(bool b)	{m_bLogin = b;}

	DWORD	GetWeaponDamageLevel();		// ��ȡ��ǰʹ�õ������Ĺ���ǿ��
	//����ű�����

public:

	//��������������������������ϵ����ݵĲ���������������������

	//
	long GetRoleStepNum(long ID){return m_RoleQueue.GetQuestStepNum(ID);}
	//Ѱ��ĳ�������ĳһ�����Ƿ����
	bool FindRoleStep(long ID , long Step){return m_RoleQueue.FindQuestStep(ID,Step);}		

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ��ֵ
	long GetRoleParam(long RoleID, long Step ,long Index){return m_RoleQueue.GetParam(RoleID,Step,Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	bool SetRoleParam(long RoleID,  long Step,long Index, long val){return m_RoleQueue.SetParam(RoleID,Step,Index,val);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	bool AddRoleParam(long RoleID, long Step ,long Index, long val){return m_RoleQueue.AddParam(RoleID,Step,Index,val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ��ֵ
	long GetRoleAccParam(long RoleID, long Step ,long Index){return m_RoleQueue.GetAccParam(RoleID,Step,Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	bool SetRoleAccParam(long RoleID,  long Step,long Index, long val){return m_RoleQueue.SetAccParam(RoleID,Step,Index,val);}

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	bool AddRoleAccParam(long RoleID, long Step ,long Index, long val){return m_RoleQueue.AddAccParam(RoleID,Step,Index,val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	long GetRoleRegionID(long RoleID, long Step ,long Index){return m_RoleQueue.GetRegionID( RoleID,  Step , Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	bool SetRoleRegionID(long RoleID,  long Step,long Index, long val){return m_RoleQueue.SetRegionID( RoleID,   Step, Index,  val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	long GetRoleRegionX(long RoleID, long Step ,long Index){return m_RoleQueue.GetRegionX( RoleID,  Step , Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	bool SetRoleRegionX(long RoleID,  long Step,long Index, long val){return m_RoleQueue.SetRegionX( RoleID,   Step, Index,  val);}

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	long GetRoleRegionY(long RoleID, long Step ,long Index){return m_RoleQueue.GetRegionY( RoleID,  Step , Index);}

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	bool SetRoleRegionY(long RoleID,  long Step,long Index, long val){return m_RoleQueue.SetRegionY( RoleID,   Step, Index,  val);}
	//*******************************************
	//ȡ�������б����������
	long GetRoleSum(){return m_RoleQueue.GetQuestSum();}
	//
	//	BOOL AddOneRecord(LONG roleid,LONG step){return m_RoleQueue.AddOneRecord(roleid, step);}
	bool AddOneRecord(long roleid);
	//ɾ��һ������
	bool DelOneRecord(long ID){return m_RoleQueue.DelOneRecord(ID);}

	//ȡ���������������½�
	long GetChapter(){return m_RoleQueue.GetChapter();}

	//�������������½���
	bool SetChapter(long num);//{ m_RoleQueue.SetChapter(num);return TRUE;}

	//�ƶ����������½�������һ�½�
	BOOL JumpChapterNext(){return m_RoleQueue.JumpChapterNext();}

	//��Ӿ���������¼
	bool AddFinishedRoleRecord(long index);//{return m_RoleQueue.AddFinishedRoleRecord(index);}
	bool DelFinishedRoleRecord(long index);

	//ȡ��һ�������������״̬���Ƿ���ɣ�
	bool GetRoleFinishedRecord(long index){return m_RoleQueue.GetQuestFinishedRecord(index);}
	//��ȡһ���������ڶ�Ӧ���ı�index
	//LONG GetStepText(LONG lRoleID,LONG lTextID);
	//LONG GetText(LONG lRoleID,LONG StepID, LONG lTextID); 

    //�������״̬�ɣġ�(1-9��)
    eQuestTextId GetQuestStateId(long QuestId);
    
    //��ȡһ�������ı���Ӧ��id
    const char* GetQuestTextId(long QuestId,long QState);

	//��������������������������ϵͳ�Ĳ�����������������������������

	//�õ�ٸ���������ֵ
	long GetMercQuestData(long lQuestListID,long lPos){return m_MerQuestSys.GetData(lQuestListID,lPos);}
	//����ٸ������ֵ 
	void SetMercQuestData(long lQuestListID,long lPos,long lValue){ m_MerQuestSys.SetData(lQuestListID,lPos,lValue);}
private:
	const CQuest*		GetQuest(long RoleID);								//Ѱ��һ������
	const CQuestStep* GetStep(long RoleID, long StepID);				//Ѱ�Ҳ���
	const CQuestAim*  GetAim(long RoleID, long StepID,long AimID);	//Ѱ������Ŀ��
	const CQusetEncouragement *GetQuestEncouragement(long RoleID,long Index);
	const CQusetEncouragement *GetStepEncouragement(long RoleID, long StepID,long Index);
private:
	//const CQuest*		m_pQuest;			//����
	//const CQuestStep* m_pQuestStep;		//�����衣����������߲���Ч�ʣ�
	//const CQuestAim*  m_pQuestAim;		//����Ŀ�ꡣ����������߲���Ч�ʣ�
public:
	//ȡ���ı�����
	long GetTextType(long RoleID);
	//attribute:
	//LONG AttributeGetID(LONG RoleID)const{return m_lID;}
	//��ȡ��������
	const char* GetAttributeRoleName(long);
	//�Ƿ񱻼�¼
	long GetAttributeBeRecord(long RoleID);
	//��������
	long GetAttributeQuestType(long RoleID);
	//����������
	long GetAttributeQuestIsShare(long RoleID);
	//�Ƿ���ʾ
	long GetAttributeShowAble(long RoleID);
	//�Ƿ��ܷ�����
	long GetAttributeRepeated(long RoleID);
	//�����Ƿ�ʧ��
	long GetAttributeDeadLost(long RoleID);
	//����ʧ�ܺ��ܷ����½�
	long GetAttributeLostRepeated(long RoleID);
	//��С�ȼ�����
	long GetAttributeMinGrade(long RoleID);
	//���ȼ�����
	long GetAttributeMaxGrade(long RoleID);
	//ְҵ����
	bool GetAttributeOccupationRestrict(long RoleID,int index);
	//������
	long GetAttributeOdds(long RoleID);
    //�����Ա�Ҫ��
    bool GetAttributeQuestSex(long QuestID);
    //����������
    bool GetAttributeQuestSkill(long QuestID);
    //����Я������
    bool GetAttributeTotePet(long QuestID);
	//��һ�����id
	long GetAttributeNextRoleID(long RoleID);
	//�½���
	long GetAttributeChapterNum(long RoleID);
	//���Ч��
	long GetAttributeFinishEffect(long RoleID);
	//CQuestLabelPrefix

	//ȡ��index��Ӧ��ǰ������
	long GetLabelPrefix(long RoleID,long Index);
	//ȡ��ǰ����������
	long GetLabelSum(long RoleID);
	//CQuestAim

	//LONG GetID(LONG RoleID,LONG StepID,LONG AimID)const;
	//Ŀ��ģ������
	long GetAimAMTType(long RoleID,long StepID,long AimID);
	//Ŀ������
	long GetAimType(long RoleID,long StepID,long AimID);
	//ȡ��Ŀ��id
	long GetAimId(long RoleID,long StepID,long AimID);
	//ȡ��Ŀ������
	long GetAimNum(long RoleID,long StepID,long AimID);
	//ȡ�� ��������
	long GetAimParameIndex(long RoleID,long StepID,long AimID);
	//����������Ʒ���������ͣ�����Ʒ��
	long GetDropFrom(long RoleID,long StepID,long AimID);
	//����
	long GetDropOdds(long RoleID,long StepID,long AimID);
	//ȡ��Ŀ�������
	const char* GetAimName(long RoleID,long StepID,long AimID);
	//ȡ�ñ�������
	long GetPrepareParam(long RoleID,long StepID,long AimID,long index);

	//CQusetEncouragement
	//LONG GetStepEncouragementSum(LONG RoleID,LONG StepID);
	long GetQuestEncouragementSum(long RoleID);
	//ȡ�ý���������
	const char* GetSetpEncouragementName(long RoleID,long Step,long Index);
	const char* GetQuestEncouragementName(long RoleID,long Index);
	//ȡ�ý�������
	long GetStepEncouragementType(long RoleID,long Step,long Index);
	long GetQuestEncouragementType(long RoleID,long Index);
	//�����ľ���id
	long GetStepEncouragementID(long RoleID,long Step,long Index);
	long GetQuestEncouragementID(long RoleID,long Index);
	//��������
	long GetStepEncouragementNum(long RoleID,long Step,long Index);
	long GetQuestEncouragementNum(long RoleID,long Index);
	//������ռ�ĸ�����
	long GetStepEncouragementCell(long RoleID,long Step);
	long GetQuestEncouragementCell(long RoleID);

	//ִ�н���
	bool PerformEncouragement(long RoleID,long StepID);
	bool PerformEncouragement(long RoleID);
	//�����������
	long GetStepSum(long RoleID);
	//ȡ�ò����µ�Ŀ������
	long GetStepAimNum(long RoleID,long StepID);
	//����ϵͳ���
	long FindRoleIsInSystem(long RoleID);
	//һ�������ĳһ�����Ƿ�������ϵͳ��
	bool FindRoleStepIsInSystem(long RoleID,long StepID);

	//����end
	//-----------------------------------------------------------------------------------------
	//  �Զ����߹���
	//  �ӳ�n���Ͽ�Socket���ӣ�ȱʡ����0�룬���̶Ͽ���
	//-----------------------------------------------------------------------------------------
private:
	DWORD	m_dwDisconnectTimeStamp;
	DWORD	m_dwDisconnectDelayTime;

public:
	void	Disconnect( DWORD dwDelayTime = 0 );

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
        PROGRESS_FOLLOW_LEADER,                         // ����ӳ�ģʽ
		PROGRESS_FORCE_DWORD	= 0x7fffffff
	};

private:
	eProgress	m_eProgress;
	CGUID		m_PersonalShopSessionID;
	long		m_PersonalShopPlugID;


    
    /// by:Expter
private:
    ePhaseRest  m_PhaseRest;      ///> ��λ  
    tagBuddhism m_Buddhism;       ///> ��֮������ɫϵͳ

public:
    void        UnCancelBuddhismTimer();
    void        RegisterBuddhismTimer();
    void        RegisterDryTimer();
    void        UseSkillUpdateBuddhismDistract();
public:
	eProgress	GetCurrentProgress						();
	void		SetCurrentProgress						( eProgress ep );
	void		SetPersonalShopFlag						( const CGUID& SessionID, long PlugID );
    void        CodePersonalShopForClient( DBWriteSet& setWriteDB);
    
    /// �ж�����Ƿ�����λ�����͸���ģʽ״̬ (��Ҫ�������)
    bool        IsLockPlayerState()  ;
    void        SetPhaseRest( ePhaseRest rest )         { m_PhaseRest = rest ; }
    ePhaseRest  GetPhaseRest() const                    { return  m_PhaseRest; }

    /// �츳  by:Expter
private:
    /// �츳����Ӧ�츳ֵ���츳�ȼ�
    std::map<long,long>         m_GeniusTable;
    /// �츳Ӱ�켼�����ս��
    GeniusAction*               m_GeniusAction;

public:
    long         GetGeniusLvl(long  Geniusid);        
    void         RemoveGenius(long  Geniusid);

    std::map<long,long>  GetGenius() const                              {  return m_GeniusTable;           }
    void        StudyGenius(long Geniusid,long lvl);
    void        SetGeniusLvl(long Geniusid,long lvl)                    { m_GeniusTable[Geniusid] = lvl;   }
    virtual void  AddGeniusStates(long skillid,CMoveShape* pDest,long Gid){ m_GeniusAction->AddGeniusStates(skillid,pDest,Gid);}

    virtual long  GetSkillValue(long skill,long lvl,std::string name,
                                long xPos=0,long yPos=0)                { return  m_GeniusAction->GetSkillAttri(skill,lvl,name,xPos,yPos); }

	//-----------------------------------------------------------------------------------------
	//  ������Ϣ����ǰ�û�.��ʾ������Ի�����
	//  
	//-----------------------------------------------------------------------------------------
public:
	//void		SendNotifyMessage(const char* szText, DWORD dwColor = 0xffffffff, DWORD dwBkColor = 0,eNOTIFY_POS ePos=eNOTIFYPOS_CENTER);
	/***********************************************************************/
	/*  zhaohang 2010-11-8 fix ֱ���޳���ɫ����ɫ�����ַ����У��ͻ������Զ�����*/
	void		SendNotifyMessage(const char* szText,eNOTIFY_POS ePos=eNOTIFYPOS_CENTER);
	/***********************************************************************/
	

	void		SendSystemInfo(const char* szText, DWORD dwColor = 0xffffffff );
	void		SendOtherInfo(const char* szText );
	void		SendPlayerInfo(const char* szText, DWORD dwColor = 0xffffffff, DWORD dwBkColor = 0 );
	friend class CMonster;

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
public:
	virtual DWORD	GetStrikeOutTime();		//##��ȡ��ȭ��ʱ��
	virtual WORD	GetAtcInterval();		// �������

	//-----------------------------------------------------------------------------------------
	//  �������
	//  
	//-----------------------------------------------------------------------------------------
private:
	LONG		m_lEmotionIndex;
	DWORD		m_dwEmotonTimeStamp;

public:
	void		PerformEmotion( LONG lID );
	void		ClearEmotion();
	void		PackGlint();


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

	//virtual BOOL OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);
	//virtual BOOL OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

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

	////!					���һ�����鸱��GUID
	//void				AddTeamRgnGUID(LONG lTemplateRgnID, const CGUID& guid, BOOL bCover);
	////!					ɾ��һ�����鸱��GUID
	//void				DelTeamRgnGUID(LONG lTemplateRgnID);
	//void				DelTeamRgnGUID(const CGUID& guid);
	////!					�õ�һ�����鸱����GUID
	//const	CGUID&		GetTeamRgnGUID(LONG lTemplateRgnID);
	////!					ͨ��˳��õ�һ������ĸ���GUID
	//const	CGUID&		GetTeamRgnGuidByIndex(LONG lIndex);
	////!					ͨ��˳��õ�һ������ĸ���ģ��ID
	//LONG				GetTeamRgnTemplateIdByIndex(LONG lIndex);
	////!					�õ����鸱��GUID�б�
	//map<LONG, CGUID>&	GetTeamRgnGuidMap(void){return m_mapTeamRgnGUID;}
	////!					���鸱��GUID����
	//void				RgnGuidCode(vector<BYTE> &vData);
	////!					���鸱��GUID����
	//void				RgnGuidDecode(BYTE *pData, LONG &lPos);
	////!					���뽻�����鸱������������ͬģ��ĸ�����
	//void				RgnGuidSwapFromArray(BYTE *pData, LONG &lPos);

	////!					���鸱��GUID����
	//void				RgnGuidCode(DBWriteSet& setWriteDB);
	////!					���鸱��GUID����
	//void				RgnGuidDecode(DBReadSet& setReadDB);
	////!					���뽻�����鸱������������ͬģ��ĸ�����
	//void				RgnGuidSwapFromArray(DBReadSet& setReadDB);

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

	long    GetSignCode(void) const { return m_lSignCode; }
	void	SetSignCode(long code) { m_lSignCode = code; }

private: 
	// �Ƿ��һ�ε�¼
	bool	b_FirstEnterGame;
	CGUID m_PersonalRgnGUID;
	CGUID m_PersonalHouseRgnGUID;
	//map<LONG, CGUID> m_mapTeamRgnGUID;
	long					m_lQueueTimerID;//�����������ж�ʱ��

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
    void CodeIncShopToDataBlock(DBWriteSet& setWriteDB);
    void DecodeIncShopFromDataBlock(DBReadSet& setReadDB);

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
	void LockCollection(CMoveShape* pMoveShape);
	//����ɼ�������
	void OutlockCollection(CMoveShape* pMoveShape);
	//�Ƿ�������
	tagUseFailed CanFacture();
	//����
	BOOL Facture();
	//������ѧϰ
	void FactureSkillStudyBegin(CMoveShape* moveshape);
	void StudyFactureSkill(CMoveShape* moveshape,DWORD factureID);
	void CleanFacture();//{ m_DOccuRecord.CleanFacture();}
	void AddCollectionExp(DWORD exp);
	void AddFactureExp(DWORD exp);

	BOOL AddFacture(DWORD l);//{return m_DOccuRecord.AddFacture(l);}
	BOOL DeleteFacture(DWORD l);//{return m_DOccuRecord.DeleteFacture(l);}
	BOOL FindFacture(DWORD l){return m_DOccuRecord.FindFacture(l);}
	//���͸�ְҵʧ����Ϣ
	void SendDOccuSKillFailMsg(int nValue,int iSkillID, int iSkillLevel);
	//��ȡ�� ����  ѡ���������id
	DWORD GetElectFactureID(){return m_dwElectFactureID;}
	void  SetElectFactureID(DWORD id){m_dwElectFactureID= id;}

	//�Ը�ְҵϵͳ�Ĳ�ѯ
	long GetFactureNeedsLevel(DWORD ID);//{return CDeputyOccuSystem::GetFacture(ID)->GetFactureLevel();}
	long GetFactureNeedsDOccu(DWORD ID);//{return CDeputyOccuSystem::GetFacture(ID)->GetArtisanID();}

private:
	//��ְҵ��¼�Ĳ���
	LONG GetCollectionExp(){return m_DOccuRecord.m_lCollectionExp;}
	void SetCollectionExp(LONG l);//{m_DOccuRecord.m_lCollectionExp=l;}
	LONG GetCollectionGrade(){return m_DOccuRecord.m_lCollectionGrade;}
	void SetCollectionGrade(LONG l);//{m_DOccuRecord.m_lCollectionGrade=l;}
	LONG GetFactureExp(){return m_DOccuRecord.m_lFactureExp;}
	void SetFactureExp(LONG l);//{m_DOccuRecord.m_lFactureExp=l;}
	LONG GetFactureGrade(){return m_DOccuRecord.m_lFactureGrade;}
	void SetFactureGrade(LONG l);//{m_DOccuRecord.m_lFactureGrade=l;}
	LONG GetFactureSuccRatio(){return m_DOccuRecord.GetFactureSuccRatio();}
	void AddFactureSuccRatio(LONG l){m_DOccuRecord.AddFactureSuccRatio(l);}
	void ClearFactureSuccRatio(){m_DOccuRecord.ClearFactureSuccRatio();}

public:
	void GetPlayerIDAndTeammateID(hash_set<CGUID,hash_guid_compare>& hsPlayers);





private:
	CDeputyOccuRecord m_DOccuRecord;
	DWORD m_dwElectFactureID;

public:
	//virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const char* value);
	//virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray=0, double value=0);
	//virtual const char* GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	//virtual double GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray);

	//virtual int SetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const CGUID& guid);
	//virtual const CGUID& GetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	////ˢ�³�����Ч
	void UpdateEffect();//UPDATEEFFECT();


public:
	void SetServerKey(DWORD dwIdx, DWORD dwKey){if(SERVER_KEY_DWORD_SIZE - 1 < dwIdx)return; m_dwServerKey[dwIdx] = dwKey;}
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
//	const	char*			GetShowMedalBaseName(void);



private:
	//!						��ǰ�ƺ�����
	LONG					m_lCurrTitleType;

	//-------------------------------------------------------------------------------
	// ͨ����Ʒ��������
	// 
	//-------------------------------------------------------------------------------
public:
	GoodsAllPurposeOpt&		GetGoodsAllPurposeOpt(void){return m_GoodsAllPurposeOpt;}
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

public:
    Mover &GetMover() { return m_mover; }
private:
    Mover m_mover;

public:
    TauntedList *GetTauntedList() { return m_tauntList; }
    virtual bool OnIncreaseHp( long &validHp, CMoveShape *firer );
private:
    TauntedList *m_tauntList;

public:
    void SendValidateMsgForCs();
};
