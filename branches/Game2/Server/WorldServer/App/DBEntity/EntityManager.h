//EntityManager.h/////////////////////////////////////////////////////////////////////
//������:�ö���������еĶ���ʵ���Ͷ����������Ϣ�����Լ���DBS�ĻỰ����
//Author:������
//Create Time:2008.11.03
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Account.h"

class CCountry;
class CGoods;
class CEntityGroup;

#define MAX_DB_ENTITY_NUM 1024
	
// �����������ļ���Ϣ����
struct tagBuildFileInfo
{
	CGUID	leafGuid;
	long	lLevel;
	long	lComType;
	string	strLeafComFlag;	// Ҷ�Ӷ�������
	string	strRootComFlag;	// ����������
	CGUID	rootGuid;

	tagBuildFileInfo()
		:leafGuid(NULL_GUID),rootGuid(NULL_GUID),lLevel(0),lComType(-1),
		strLeafComFlag(""),strRootComFlag("")
	{

	}
	~tagBuildFileInfo()
	{
		
	}
	void DecodeFromByteArray(BYTE* buf, long& pos, long bufSize=-1);
};
struct tagEntityAttrInfo
{
	tagEntityAttrInfo()
		:eNumDataType(DATA_OBJECT_UNKNOWN),lMaxSize(0),lUseage(0),lAttrTypeEnum(0)
	{}
	~tagEntityAttrInfo(){}

	// ��������ö��ֵ
	long lAttrTypeEnum;
	// ��󳤶�
	long lMaxSize;
	// �÷�
	long lUseage;
	DATA_OBJECT_TYPE eNumDataType;
};
struct tagEntityBuildInfo
{
	tagEntityBuildInfo();
	~tagEntityBuildInfo();

	long							lComType;			// ��������
	string							strComFlag;			// ��������
	long							lHeight;			// ���߶�
	CGUID							guid;				// guid
	string							strTblName;			// ���ݿ����
	long							lDbOperFlag;		// ���ݿ������־
	long							lLeafNum;			// Ҷ�Ӹ���
	string							strPrimarykey;		// ����ֵ��
	long							lHhasAttrFlag;		// �Ƿ�������
	long							lLeafComType;		// Ҷ������
	string							strLeafComFlag;		// Ҷ������
	long							lHasDetailLeaves;	// �Ƿ��з�ɢҶ�ӽڵ�(����goodsgroup)
	string							strDbQueryName;		// ���ݿ���ұ�����
	long							lDbQueryType;		// ���ݿ���ұ�������
	vector<string>					vecIndexStr;		// ���ݱ�������
	map<string, tagEntityAttrInfo>	pDataMgr;			// ��������
	map<string, tagBuildFileInfo*>	mapBuildFileInfo;	// �ṹ����
	BYTE							pQueryVar[256];		// ���ݿ���ұ���������Ϣ

	void DecodeFromByteArray(BYTE* buf, long& pos, long bufSize);
};

class CEntityManager
{
public:
	CEntityManager();
	~CEntityManager();

	// EntityMap
	typedef std::map<string, std::map<CGUID, CEntityGroup*>> DBEntityComponentMap;
	typedef DBEntityComponentMap::iterator DBEntityComponentMapItr;

	typedef std::map<std::string, Account*> AccountMap;
	typedef AccountMap::iterator AccountMapItr;

	typedef std::map<std::string, tagEntityBuildInfo*> AttrDefMap;
	typedef AttrDefMap::iterator AttrDefMapItr;

	void ProcessMsg(CMessage* msg);

	//-------------- interfaces
	// CBaseEntity
	// ���һ��BaseEntity��Mapĩβ
	void AddOneRootEntity(CEntityGroup*entity);
	// ɾ��һ��BaseEntity
	void DelOneRootEntity(CEntityGroup*entity);
	// ɾ��һ��BaseEntity
	void DelOneRootEntity(const string& comFlag, const CGUID& playerID);
	// �������
	void ClearRootEntityMap(void);

	DBEntityComponentMap& GetBaseEntityMap(void) { return m_mapEntitys; }
	CEntityGroup*FindBaseEntity(string, const CGUID& guid);

	// ����ID
	bool GenerateGUID(CGUID& guid);

	// ����Ϣȡ�ò�������
	CEntityGroup*FindRootEntity(const string& flag, const CGUID& guid);
	// ��������
	CEntityGroup* CreateRootEntity(const string& flag, const CGUID& guid);

	AccountMap&	GetAccountMap(void) { return m_mapAccounts; }
	// ���һ��Account��Mapĩβ
	bool AddAccount(Account* acc);
	// ɾ��һ��Account
	void DelAccount(Account* acc);
	Account* FindAccount(const std::string& strCdkey);
	
	//interface--DB�������Ϸ����ת���ӿ�
	void CDBCountryToCCountry(CBaseEntity* pEntity, CCountry* country);
	void CCountryToCDBCountry(CCountry* country, CBaseEntity* pEntity); 

	void CDBLoginPlayerToCPlayer(CEntityGroup* pEntity, CPlayer* player);
	void CPlayerToCDBLoginPlayer(CPlayer* player, CEntityGroup* pEntity); 

	void CDBPlayerToCPlayer(CEntityGroup* pEntity, CPlayer* player);
	void CPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pEntity); 

	void CDBGoodToCGoods(CBaseEntity* pBaseEntity, CGoods* goods, const CGUID& ownerID);
	void CGoodsToCDBGood(CGoods* goods, CBaseEntity* pBaseEntity, const CGUID& ownerID);

	void CPlayerFriendToCDBFriendGroup(CPlayer* player, CEntityGroup*pBaseEntityOwner, CEntityGroup* pBaseEntityAim); 
	void CDBFriendGroupToCPlayerFriend(CPlayer* player, CEntityGroup*linkmanGroupOwner, CEntityGroup*linkmanGroupAim); 
	
	void CDBRegionToCRegion(CEntityGroup* pBaseEntity, CWorldRegion* pRegion);
	void CRegionToCDBRegion(CWorldRegion* pRegion, CEntityGroup* pBaseEntity); 
	
	void CDBGlobalVarGroupToCGlobalVar(CEntityGroup* pBaseEntity);
	void CGlobalVarGroupToCGDBlobalVar(CEntityGroup* pBaseEntity);

	void CDBPhRgnToCPhRgn(CEntityGroup* pBaseEntity, CWorldRegion* pRegion);
	void CPhRgnToCDBPhRgn(CWorldRegion* pRegion, CEntityGroup* pBaseEntity);

	void CFactionToCDBFaction(CFaction* fac, CEntityGroup* pBaseEntity);
	void CDBFactionToCFaction(CEntityGroup* pBaseEntity, CFaction* fac);

	void CUnionToCDBUnion(CUnion* pUnion, CEntityGroup* pBaseEntity);
	void CDBUnionToCUnion(CEntityGroup* pBaseEntity, CUnion* pUnion);
	void CDBMailToCMail(CEntityGroup* pEntity, CMail* pMail);
	void CMailToCDBMail(CMail* pMail, CEntityGroup* pEntity); 

	void CLimitGoodsRecordToCDBRecord(CEntityGroup* pEntity);
	void CDBLimitGoodsRecordToCRecord(CEntityGroup* pEntity);
	void CDBMailGoodsToCGoods(CBaseEntity* dbGood, CGoods* good, const CGUID& ownerID);
	void CGoodsToCDBMailGoods(CGoods* pGoods, CBaseEntity* pDBMailGoods, const CGUID& ownerID);

	void CDBPhRgnGroupToCPhRgnGroup(CEntityGroup* pPhRgnGroup);
	void CPhRgnGroupToCDBPhRgnGroup(CEntityGroup* pPhRgnGroup);
	// �ֲ�ת�� CDBPlayer �� CPlayer
	void DetailPropertyCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
    void DetailGoodsCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer, PLAYER_EXTEND_ID cID);
	void DetailEquipmentCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailOrignalPackCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailSubpackCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* player, long subPackPos);
	void DetailWalletCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailSilverCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailQuestCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailMerQuestCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailSpriteCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* player);
	void DetailSkillCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
    void DetailBrotherCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailStateCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailVarDataCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailMedalCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailAreaCreditCDBPlayerToCPlayer	(CEntityGroup* pBaseEntity, CPlayer* player);
	void DetailLimitGoodsRecordCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* player);
	void DetailIncGoodsCDBPlayerToCPlayer(CEntityGroup* pBaseEntity, CPlayer* pPlayer);
	void DetailIncTradeRecordCDBPlayerToCPlayer( CEntityGroup* pBaseEntity, CPlayer* playe );
	void DetailOrignalDepotCDBPlayerToCPlayer	(CEntityGroup* pBaseEntity, CPlayer* player);
	void DetailSubDepotCDBPlayerToCPlayer		(CEntityGroup* pBaseEntity, CPlayer* player, long subpackPos);
	void DetailDepotWalletCDBPlayerToCPlayer	(CEntityGroup* pBaseEntity, CPlayer* player);
	void DetailDepotSilverCDBPlayerToCPlayer	(CEntityGroup* pBaseEntity, CPlayer* player);
	//void DetailSpriteCPlayerToCDBPlayer		(CEntityGroup* pBaseEntity, CPlayer* player); 
	void DetailSysMailCDBPlayerToCPlayer	(CEntityGroup* pBaseEntity, CPlayer* player);

	// �ֲ�ת�� CPlayer �� CDBPlayer
	void DetailSpriteCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity); 
	void DetailPropertyCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailEquipmentCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
    void DetailEquipGodCPlayerToCDBPlayer(CPlayer* pPlayer, CEntityGroup* pBaseEntity);
    void DetailEquipLandCPlayerToCDBPlayer(CPlayer* pPlayer, CEntityGroup* pBaseEntity);
    void DetailEquipHumanCPlayerToCDBPlayer(CPlayer* pPlayer, CEntityGroup* pBaseEntity);
	void DetailOrignalPackCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailSubpackCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity, long subpackPos);
	void DetailWalletCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailSilverCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailQuestCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailMerQuestCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);//
	void DetailSkillCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
    void DetailBrotherCPlayerToCDBPlayer(CPlayer* pPlayer, CEntityGroup* pBaseEntity);
	void DetailStateCPlayerToCDBPlayer		(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailVarDataCPlayerToCDBPlayer	(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailMedalCPlayerToCDBPlayer		(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailAreaCreditCPlayerToCDBPlayer	(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailLimitGoodsRecordCPlayerToCDBPlayer(CPlayer* player,CEntityGroup* pBaseEntity);
	void DetailIncTradeRecordCPlayerToCDBPlayer( CPlayer* player,CEntityGroup* pBaseEntity );
	void DetailGoodsCooldownCPlayerToCDBPlayer	(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailIncGoodsCPlayerToCDBPlayer	(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailDeOccuCPlayerToCDBPlayer		(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailSysMailCplayerToCDBPlayer	(CPlayer* player, CEntityGroup* pBaseEntity);

	void DetailDepotCPlayerToCDBPlayer(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailSubDepotCPlayerToCDBPlayer	(CPlayer* player, CEntityGroup* pBaseEntity, long subpackPos);
	void DetailDepotWalletCPlayerToCDBPlayer		(CPlayer* player, CEntityGroup* pBaseEntity);
	void DetailDepotSilverCPlayerToCDBPlayer		(CPlayer* player, CEntityGroup* pBaseEntity);

	/// ��ҵ������DBS�ı����
	void DetailBusinessPackCPlayerToCDBPlayer( CPlayer *player, CEntityGroup *pBaseEntity );
	void DetailBusinessPackCDBPlayerToCPlayer( CEntityGroup *pBaseEntity, CPlayer *player );

    /// �츳�����
    void DetailGeniusDataCPlayerToCDPlayer( CPlayer *player, CEntityGroup *pBaseEntity );
    void DetailGeniusDataCDPlayerToCPlayer( CEntityGroup *pBaseEntity, CPlayer *player );

	bool LoadPlayerPet(CEntityGroup* pDBPlayer,
		CPlayer* pPlayer);						// ������ҳ����б�
	bool SavePlayerPet(CPlayer* pPlayer,
		CEntityGroup* pDBPlayer);				// �洢��ҳ����б�

	//------��Ҷ���������߽ӿ�--------
	// ������װ����Ʒmap
	void ClearPlayerContainerMap(CEntityGroup* pBaseEntity, PLAYER_EXTEND_ID containerID);
	// �������ӱ�������
	void ClearPlayerSubpackSelfMap(CEntityGroup* pBaseEntity, long subPos);
	// �����Ҳֿ��ӱ�������
	void ClearPlayerSubDepotSelfMap(CEntityGroup* pBaseEntity, long subPos);
	// ����PLAYER_EXTEND_IDȡ��Entity������ֵ
	string GetComFlagByExtendID(PLAYER_EXTEND_ID containerID);
	// ���һ����Ʒ���������
	void AddGoodsToPlayer(CEntityGroup* pBaseEntity, CGoods* pGoods, long pos, PLAYER_EXTEND_ID containerID);
	// ����LoginPlayer��ɾ��ʱ��
	long ComputeLoginPlayerDelTime(CEntityGroup* pBaseEntity);
	// ������ҽڵ���ĳЩ�ӽڵ�����ݿ������־λ
	void SetEntityLeavesDbOperFlag(CBaseEntity* pBaseEntity, const string& leafComFlag, bool operFlag);
	// ����ҽڵ��������ӽڵ����ݿ������־λ����
	void ResetEntityAllDbOperFlag(CBaseEntity* pBaseEntity, bool flag = false);
	// ���ôӸ���㿪ʼ�����нڵ����ݿ������־
	void SetEntityAllDbOperType(CBaseEntity* pBaseEntity, DB_OPERATION_TYPE opType);
	// ����Ҷ���������������²����丸�ڵ�
	CEntityGroup* FindLeafFatherByComFlag(CEntityGroup* pRoot, const string& leafComFlag);
	// ����Ҷ���������������²�����ڵ�
	CBaseEntity* FindLeafByComFlag(CEntityGroup* pRoot, const string& leafComFlag);
	
	// ӳ�������ַ�������ö��ֵ��
	void AddAttrEnumAndStr(const string& strName, long lEnum);
	long GetAttrEnumByStr(const string& strComFlag, const string& strName);
	const string& GetAttrStrByEnum(long lEnum);
	// ȡ�������ļ���������Ϣ
	tagEntityBuildInfo* GetEntityBuildInfo(const string& strComFlag);
	// DATA_OBJECT_TYPE
	long GetDataObjectType(const string& strComFlag, const string& strName);
	long GetDataObjectType(const string& strComFlag, long lEnum);
	// ���ݿ����Ա�־λ: 1|0: 1(���ݿ��Զ�����) 0�������(Ĭ��)
	BYTE GetDbUseageFlag(const string& strComFlag, const string& strName);
	BYTE GetDbUseageFlag(const string& strComFlag, long lEnum);
	// ���ݿ���󳤶�
	long GetBufMaxSize(const string& strComFlag, const string& strName);
	long GetBufMaxSize(const string& strComFlag, long lEnum);
	
	long GetAttrEnumValue(void) { return m_lAttrEnumValue; }

	//! ��Ҫ�ֶ��ͷŸýӿڷ�����ڴ�
	CBaseEntity*	NewBaseEntity(const string& strComFlag, const CGUID& guid);
	//! �����ڴ���սӿ�
	void			DelBaseEntity(CBaseEntity* pEntity);

	//! ����������ͼ�����
	void			OutEntityInfo(VOID);
public:
	std::map<std::string/*attrname*/, tagEntityBuildInfo*>& GetObjAttrDef(void)		{ return m_mapObjAttrDef; }

	void CreateChangeNameProcSession(const CGUID& guid, const char* szName, long gsscoketid);

	void CreateSaveAccountSession(Account* pAcc);
	void CreateLoadAccountSession(Account* pAcc);
	void CreateLoadCountrySession(void);
	BOOL CreateDelFactionSession(const CGUID &FactionGuid);
	void CreateDelUnionSession(const CGUID &UnionGuid);
	void CreatePlayerCreateSession(const char* szCdkey, const char* szName, BYTE nOccu,
		BYTE sex,BYTE nHead, BYTE nFace, BYTE btCountry, BYTE btMaxCharactersNum,
		BYTE byConstellation,bool bSelectRecommCountry);
	void CreateLoadPlayerDataSession(const char* szCdkey, const CGUID& guid);
	void CreateSaveLoginPlayerDataSession(const CGUID& guid);
	void CreateSavePlayerDataSession(const CGUID& guid, long flag/*SAVE_DETAIL_ATTR_TYPE*/);
	void CreateLoadFactionGroupSession(void);
	void CreateLoadUnionGroupSession(void);
	void CreateSaveFactionSession(vector<CEntityGroup*> &vCEntityGroup);
	void CreateSaveUnionSession(CEntityGroup* pUnion);
	void CreateInsertFactionSession(CEntityGroup* pFaction);
	void CreateInsertUnionSession(CEntityGroup* pUnion);
	void CreateLoadRegionSession(void);
	void CreateSaveRegionSession(CEntityGroup* pRegion);
	void CreateCreateRegionSession(CEntityGroup* pRegion);
	void CreateLoadPhRgnSession(void);
	void CreateSavePhRgnSession(CEntityGroup* pPhRgn);
	void CreateLoadGlobalVarSession(void);
	void CreateSaveGlobalVarSession(void);
	
	void CreateSaveBusinessSession();
	void CreateLoadBusinessSession();

    void CreateLoadAgentOrdersSession();
    void CreateSaveAgentOrdersSession();

	void CreateSaveLimitGoodsRecordSession();
	void CreateLoadLimitGoodsRecordSession();
	// ������ҵ�ɾ��ʱ��
	void CreateUpdateLoginPlayerDelTimeSession(const char* szCdkey, const CGUID& guid);
	// �ָ���ҵ�ɾ��ʱ��
	void CreateRestoreLoginPlayerDelTimeSession(const char* szCdkey, const CGUID& guid);

	void CreateBanPlayerSession(const char* szName, long time);
	
	// ��ҳ�ʼ����ȡ�ż�
	void CreateInitLoadMailSession(const CGUID& guid);
	// �������ż�
	void CreateInsertMailSession(CMail *Mails);
	// ������ұ����ż�
	void CreateSaveOnlineMailSession(const CGUID& ownerID, vector<CMail*> &mails);
	// �������ɾ���ż�
	void CreateDeleteOnlineMailSession(const CGUID& ownerID, vector<CMail*> &mail);
	// �������������ż�
	//void CreateInsertOfflineMailsSession(const char* szOwnerName, CMail* pMail);
	// ϵͳȺ���ż�
	void CreateInsertGroupMailSession(const char* szOwnerName, long level, long sex, vector<long>& countryIDVec);
	// ϵͳɾ������������ż�
	void CreateSystemDeleteOfflineMailsSession(long lCurTime);
	// ���͸���Ҽ�Ǯ(�����½���Ǯ)
	void CreateAddOfflinePlayerCostMailGoldProcSession(const char* szPlayerName, long goldNum);
	
	long GetDbPlayerNewNum(void) { return m_lNewDbPlayerNum; }
	void SetDbPlayerNewNum(long lNum) { m_lNewDbPlayerNum = lNum; }

private:
	DBEntityComponentMap		m_mapEntitys;
	AccountMap					m_mapAccounts;

	// Obj������Ϣ,Obj�����������,id
	std::map<std::string, tagEntityBuildInfo*> m_mapObjAttrDef;

	long m_lAttrEnumValue;
	// ӳ�������ַ�������ö��ֵ��
	map<long, string> m_mapAttrEnumToStr;

	long			m_lNewDbPlayerNum;
};

	extern std::string NULL_STRING;
#endif//DB_ENTITY_MANAGER_H