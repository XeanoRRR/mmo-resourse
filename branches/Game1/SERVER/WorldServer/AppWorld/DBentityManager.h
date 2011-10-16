#ifndef DB_ENTITY_MANAGER_H
#define DB_ENTITY_MANAGER_H

#include "DBEntityDef.h"

class CBaseDBEntity;
class CDBCard;
class CDBLoginPlayer;
class CDBPlayer;
class CDBFaction;
//class CDBCityWar;
//class CDBVillageWar;
class CDBUnion;
class CDBCountry;
//class CDBEnemyFaction;
class CDBGenVar;
class CDBSetup;
class CDBRegion;
class CDBDupRgn;
class CDBGoodsGroup;
class CDBRgnGoods;
class CDBRgnGoodsGroup;
class CDBLinkMan;
class CDBLinkmanGroup;
class CDBFactionApply;
class CDBFactionApplyGroup;
class CDBFactionMember;
class CDBFactionMemberGroup;
class CMail;
class CDBMail;
class CDBMailGroup;
class CDBMailGoods;
class CDBMailGoodsGroup;
class CDBGenVarGroup;

class CGoods;
class CPlayer;
class CFaction;
class CCard;
struct tagAttackCityTime;
class CCountry;
struct tagEnemyFaction;
class CVariableList;
class CWorldRegion;
class CUnion;
struct tagVilWarSetup;

#include <map>

#define MAX_DB_ENTITY_ID 10240
class CDataEntityManager;

class CDBEntityManager
{
public:
	CDBEntityManager();
	~CDBEntityManager();

	struct tagLoginInfo
	{
		set<CGUID> setLoginGuid;
		long		lSignCode;	// ��¼��֤��
		long		lLastLoginTime; // �ϴ��˳�ʱ��
		ePlayerState  PlayerState;

		tagLoginInfo(void);
		~tagLoginInfo(void);
	};

	// CDBcard
	typedef std::map<CGUID, CDBCard*> DBCardMap;
	typedef DBCardMap::iterator DBCardMapItr;
	// CDBLoginPlayer
	// CGUID ��Ψһ��
	typedef std::map<CGUID, CDBLoginPlayer*> DBLoginPlayerMap;
	typedef DBLoginPlayerMap::iterator DBLoginPlayerMapItr;
	// CDKEY ��Ψһ�� ͬһ��CDBLoginPlayerӵ�ж��CDBPlayer����
	typedef std::map<string, tagLoginInfo*> CdkeyDBLoginPlayerMap;
	typedef CdkeyDBLoginPlayerMap::iterator CdkeyDBLoginPlayerMapItr;

	// CDBPlayer
	typedef std::map<CGUID, CDBPlayer*> DBPlayerMap;
	typedef DBPlayerMap::iterator DBPlayerMapItr;
	
	// CDBFaction
	typedef std::map<CGUID, CDBFaction*> DBFactionMap;
	typedef DBFactionMap::iterator DBFactionMapItr;
	//// CDBCityWar
	//typedef std::map<CGUID, CDBCityWar*> DBCityWarMap;
	//typedef DBCityWarMap::iterator DBCityWarMapItr;
	//// CDBVillageWar
	//typedef std::map<CGUID, CDBVillageWar*> DBVillageWarMap;
	//typedef DBVillageWarMap::iterator DBVillageWarMapItr;
	// CDBUnion
	typedef std::map<CGUID, CDBUnion*> DBUnionMap;
	typedef DBUnionMap::iterator DBUnionMapItr;
	// CDBCountry
	typedef std::map<CGUID, CDBCountry*> DBCountryMap;
	typedef DBCountryMap::iterator DBCountryMapItr;
	//// CDBEnemyFaction
	//typedef std::map<CGUID, CDBEnemyFaction*> DBEnemyFactionMap;
	//typedef DBEnemyFactionMap::iterator DBEnemyFactionMapItr;

	// CDBGenVar
	typedef std::map<CGUID, CDBGenVar*> DBGenVarMap;
	typedef DBGenVarMap::iterator DBGenVarMapItr;
	// CDBSetup
	typedef std::map<CGUID, CDBSetup*> DBSetupMap;
	typedef DBSetupMap::iterator DBSetupMapItr;
	// CDBRegion
	typedef std::map<CGUID, CDBRegion*> DBRegionMap;
	typedef DBRegionMap::iterator DBRegionMapItr;

	// CDBDupRgn
	typedef std::map<CGUID, CDBDupRgn*> DBDupRgnMap;
	typedef DBDupRgnMap::iterator DBDupRgnMapItr;

	typedef std::map<CGUID, CDBLinkMan*> DBLinkmanMap;
	typedef DBLinkmanMap::iterator DBLinkmanMapItr;

	typedef std::map<CGUID, CDBFactionApply*> DBFactionApplyMap;
	typedef DBFactionApplyMap::iterator DBFactionApplyMapItr;

	typedef std::map<CGUID, CDBFactionMember*> DBFactionMemberMap;
	typedef DBFactionMemberMap::iterator DBFactionMemberMapItr;

	void ProcessMsg(CMessage* msg);

	//-------------- interfaces
	//CDBCard
	// ���һ��DBCard��Mapĩβ
	void AddOneDBCard(CDBCard* card);
	// ɾ��һ��DBCard
	void DelOneDBCard(CDBCard* card);
	// ɾ��һ��DBCard
	void DelOneDBCard(const CGUID& cardID);
	// �������
	void ClearCardMap(void);
	// ȡ�ø���
	long GetDBCardNum(void) { return m_mapDBCards.size(); }
	DBCardMapItr DBCardMapBegin(void) { return m_mapDBCards.begin(); }
	DBCardMapItr DBCardMapEnd(void) { return m_mapDBCards.end(); }
	DBCardMap& GetDBCardMap(void) { return m_mapDBCards; }

	// CDBLoginPlayer
	// CGUIDֻ�ܱ�ʶ��ҽ�ɫ��GUID,һ��CDKEY�����ж����ɫ
	// ���һ��CDBLoginPlayer��Mapĩβ
	void AddOneDBLoginPlayer(CDBLoginPlayer* player);
	// ɾ��һ��CDBLoginPlayer
	void DelOneDBLoginPlayer(CDBLoginPlayer* player);
	// ɾ��һ��CDBLoginPlayer
	void DelOneDBLoginPlayer(const CGUID& playerID);
	// �������
	void ClearLoginPlayerMap(void);
	// ȡ�ø���
	long GetDBLoginPlayerNum(void) { return m_mapDBLoginPlayers.size(); }
	DBLoginPlayerMapItr DBLoginPlayerMapBegin(void) { return m_mapDBLoginPlayers.begin(); }
	DBLoginPlayerMapItr DBLoginPlayerMapEnd(void) { return m_mapDBLoginPlayers.end(); }
	DBLoginPlayerMap& GetDBLoginPlayerMap(void) { return m_mapDBLoginPlayers; }
	CDBLoginPlayer* FindLoginPlayer(const CGUID& guid);
	
	// ���� CDKEY ���� CDBLoginPlayer,��ΪΨһ�ؼ���
	// ͬһCDKEY��Ӧһ��vecotor<CDBLoginPlayer>:������ͬCDKEY�µ����н�ɫ
	// ��ҵ�һ�ε�¼ʱ��ʼ��
	bool InitLoginPlayer(const char* szCdkey);
	// ���һ��CDBLoginPlayer��Mapĩβ
	void AddLoginPlayerByCdkey(CDBLoginPlayer* player);
	// ɾ��һ��CDBLoginPlayer
	void DelLoginPlayerByCdkey(CDBLoginPlayer* player);
	// �������
	void ClearCdkeyLoginPlayerMap(void);
	// ȡ�ø���
	CdkeyDBLoginPlayerMap& GetCdkeyLoginPlayerMap(void) { return m_mapCdkeyLoginPlayers; }
	tagLoginInfo* FindLoginPlayerByCdkey(const char* szCdkey);

	
	// CDBPlayer
	// ���һ��DBPlayer��Mapĩβ
	void AddOneDBPlayer(CDBPlayer* player);
	// ɾ��һ��DBPlayer
	void DelOneDBPlayer(const CGUID& playerID);
	// �������
	void ClearPlayerMap(void);
	// ȡ�ø���
	long GetDBPlayerNum(void) { return m_mapDBPlayers.size(); }
	DBPlayerMapItr DBPlayerMapBegin(void) { return m_mapDBPlayers.begin(); }
	DBPlayerMapItr DBPlayerMapEnd(void) { return m_mapDBPlayers.end(); }
	DBPlayerMap& GetDBPlayerMap(void) { return m_mapDBPlayers; }
	CDBPlayer* FindPlayer(const CGUID& guid);
	CDBRegion* FindRegion(const CGUID& guid);

	// CDBFaction
	// ���һ��DBFaction��Mapĩβ
	void AddOneDBFaction(CDBFaction* faction);
	// ɾ��һ��DBFaction
	void DelOneDBFaction(CDBFaction* faction);
	// ɾ��һ��DBFaction
	void DelOneDBFaction(const CGUID& factionID);
	// �������
	void ClearFactionMap(void);
	// ȡ�ø���
	long GetDBFactionNum(void) { return m_mapDBFactions.size(); }
	DBFactionMapItr DBFactionMapBegin(void) { return m_mapDBFactions.begin(); }
	DBFactionMapItr DBFactionMapEnd(void) { return m_mapDBFactions.end(); }
	DBFactionMap& GetDBFactionMap(void) { return m_mapDBFactions; }
	CDBFaction* FindFaction(const CGUID& guid);

	void AddOneDBUnion(CDBUnion* Union);
	// ɾ��һ��DBUnion
	void DelOneDBUnion(CDBUnion* Union);
	// ɾ��һ��DBUnion
	void DelOneDBUnion(const CGUID& UnionID);
	// �������
	void ClearUnionMap(void);
	// ȡ�ø���
	long GetDBUnionNum(void) { return m_mapDBUnions.size(); }
	DBUnionMapItr DBUnionMapBegin(void) { return m_mapDBUnions.begin(); }
	DBUnionMapItr DBUnionMapEnd(void) { return m_mapDBUnions.end(); }
	DBUnionMap& GetDBUnionMap(void) { return m_mapDBUnions; }

	// CDBCountry
	// ���һ��DBCountry��Mapĩβ
	void AddOneDBCountry(CDBCountry* Country);
	// ɾ��һ��DBCountry
	void DelOneDBCountry(CDBCountry* Country);
	// ɾ��һ��DBCountry
	void DelOneDBCountry(const CGUID& CountryID);
	// �������
	void ClearCountryMap(void);
	// ȡ�ø���
	long GetDBCountryNum(void) { return m_mapDBCountrys.size(); }
	DBCountryMapItr DBCountryMapBegin(void) { return m_mapDBCountrys.begin(); }
	DBCountryMapItr DBCountryMapEnd(void) { return m_mapDBCountrys.end(); }
	DBCountryMap& GetDBCountryMap(void) { return m_mapDBCountrys; }

	void AddOneDBGenVar(CDBGenVar* GenVar);
	// ɾ��һ��DBGenVar
	void DelOneDBGenVar(CDBGenVar* GenVar);
	// ɾ��һ��DBGenVar
	void DelOneDBGenVar(const CGUID& GenVarID);
	// �������
	void ClearGenVarMap(void);
	// ȡ�ø���
	long GetDBGenVarNum(void) { return m_mapDBGenVars.size(); }
	DBGenVarMapItr DBGenVarMapBegin(void) { return m_mapDBGenVars.begin(); }
	DBGenVarMapItr DBGenVarMapEnd(void) { return m_mapDBGenVars.end(); }
	DBGenVarMap& GetDBGenVarMap(void) { return m_mapDBGenVars; }

	// CDBSetup
	// ���һ��DBSetup��Mapĩβ
	void AddOneDBSetup(CDBSetup* Setup);
	// ɾ��һ��DBSetup
	void DelOneDBSetup(CDBSetup* Setup);
	// ɾ��һ��DBSetup
	void DelOneDBSetup(const CGUID& SetupID);
	// �������
	void ClearSetupMap(void);
	// ȡ�ø���
	long GetDBSetupNum(void) { return m_mapDBSetups.size(); }
	DBSetupMapItr DBSetupapBegin(void) { return m_mapDBSetups.begin(); }
	DBSetupMapItr DBSetupMapEnd(void) { return m_mapDBSetups.end(); }
	DBSetupMap& GetDBSetupMap(void) { return m_mapDBSetups; }

	// CDBRegion
	// ���һ��DBRegion��Mapĩβ
	void AddOneDBRegion(CDBRegion* Region);
	// ɾ��һ��DBRegion
	void DelOneDBRegion(CDBRegion* Region);
	// ɾ��һ��DBRegion
	void DelOneDBRegion(const CGUID& RegionID);
	// �������
	void ClearRegionMap(void);
	// ȡ�ø���
	long GetDBRegionNum(void) { return m_mapDBRegions.size(); }
	DBRegionMapItr DBRegionMapBegin(void) { return m_mapDBRegions.begin(); }
	DBRegionMapItr DBRegionMapEnd(void) { return m_mapDBRegions.end(); }
	DBRegionMap& GetDBRegionMap(void) { return m_mapDBRegions; }

	
	// CDBDupRgn
	// ���һ��CDBDupRgn��Mapĩβ
	void AddOneDBDupRgn(CDBDupRgn* Region);
	// ɾ��һ��CDBDupRgn
	void DelOneDBDupRgn(CDBDupRgn* Region);
	// ɾ��һ��CDBDupRgn
	void DelOneDBDupRgn(const CGUID& RegionID);
	// �������
	void ClearDupRgnMap(void);
	// ȡ�ø���
	long GetDBDupRgnNum(void) { return m_mapDBDupRgns.size(); }
	DBDupRgnMapItr DBDupRgnMapBegin(void) { return m_mapDBDupRgns.begin(); }
	DBDupRgnMapItr DBDupRgnMapEnd(void) { return m_mapDBDupRgns.end(); }
	DBDupRgnMap& GetDBDupRgnMap(void) { return m_mapDBDupRgns; }

	// CDBLinkman
	// ���һ��CDBLinkman��Mapĩβ
	void AddOneDBLinkman(CDBLinkMan* good);
	// ɾ��һ��DBGood
	void DelOneDBLinkman(CDBLinkMan* good);
	// ɾ��һ��DBGood
	void DelOneDBLinkman(const CGUID& goodID);
	// �������
	void ClearDBLinkmanMap(void);
	// ���������ҵ���Ϣ
	void ClearDBLinkmanMap(const CGUID& PlayerGuid);
	// ȡ�ø���
	long GetDBLinkmanNum(void) { return m_mapDBLinkman.size(); }
	DBLinkmanMapItr DBLinkmanMapBegin(void) { return m_mapDBLinkman.begin(); }
	DBLinkmanMapItr DBLinkmanMapEnd(void) { return m_mapDBLinkman.end(); }
	DBLinkmanMap& GetDBLinkmanMap(void) { return m_mapDBLinkman; }
	CDBLinkMan* FindDBLinkman(const CGUID& guid) 
	{
		CDBLinkMan* ret = NULL;
		DBLinkmanMapItr itr = m_mapDBLinkman.find(guid);
		if(itr != m_mapDBLinkman.end())
			ret = itr->second;
		return ret;
	}
	bool HasDBLinkman(const CGUID& ownerID, long place);

	// ����ID
	bool GenerateGUID(CGUID& guid);

	void CGoodsToCDBGood(CGoods* good, CDBGoods* dbGood, const CGUID& ownerID);
	void CPlayerToCDBPlayer(CPlayer* player, CDBPlayer* dbPlayer);
	void CPlayerToCDBLoginPlayer(CPlayer* player, CDBLoginPlayer* dbLPlayer);
	void CFactionToCDBFaction(CFaction* fac, CDBFaction* dbFac);
	void CCardToCDBCard(CCard* card, CDBCard* dbCard);
	//void CCityWarToCDBCityWar(tagAttackCityTime* CityWarParam, CDBCityWar* cw);
	void CCountryToCDBCountry(CCountry* country, CDBCountry* dbCountry);
	//void CEnemyFactionToCDBEnemyFaction(tagEnemyFaction* efParam, CDBEnemyFaction* ef);
	void CVarsListToCDBGenVar(CVariableList* vl, CDBGenVarGroup* pGenVarGroup);
	void CWorldRgnToCDBRgn(CWorldRegion* wRgn, CDBRegion* dbRgn);
	void CSetupToCDBSetup(CDBSetup* dbSetup);
	void CUnionToCDBUnion(CUnion* u, CDBUnion* dbU);
	//void CVillageWarToCDBVillageWar(tagVilWarSetup* vws, CDBVillageWar* dbs);
	// ��������ת��
	void CWorldRgnToCDBDupRgn(CWorldRegion* wRgn, CDBDupRgn* dbDupRgn);
	// �ʼ�����ת��
	void CMailToCDBMail(CMail* pMail, CDBMail* pDBMail);
	// �ʼ���Ʒ����ת��
	void CGoodsToCDBMailGoods(CGoods* pGoods, CDBMailGoods* pDBMailGoods, const CGUID& ownerID);
	void CPlayerToCDBPlayerMails(CPlayer* player, CDBPlayer* dbPlayer);

	void CDBGoodToCGoods		(CDBGoods* dbGood, CGoods* good, const CGUID& ownerID);
	void CDBPlayerToCPlayer		(CDBPlayer* dbPlayer, CPlayer* player);
	void CDBLoginPlayerToCPlayer(CDBLoginPlayer* dbLPlayer, CPlayer* player);
	void CDBFactionToCFaction	(CDBFaction* dbFac, CFaction* fac);
	void CDBCardToCCard(CDBCard* dbCard, CCard* card);
	//void CCDBCityWarToCityWar(CDBCityWar* cw);
	void CDBCountryToCCountry(CDBCountry* dbCountry, CCountry* country);
	//void CDBEnemyFactionToCEnemyFaction(CDBEnemyFaction* ef);
	void CCDBGenVarToVarsList(CDBGenVarGroup* pGenVarGroup, CVariableList* vl);
	void CCDBRgnToWorldRgn(CDBRegion* dbRgn, CWorldRegion* wRgn);
	void CDBSetupToCSetup(CDBSetup* dbSetup);
	void CDBUnionToCUnion(CDBUnion* dbU, CUnion* u);
	//void CCDBVillageWarToVillageWar(CDBVillageWar* dbs);
	// ��������ת��
	void CDBDupRgnToCWorldRgn(CDBDupRgn* dbDupRgn, CWorldRegion* wRgn);
	// �ʼ�����ת��
	void CDBMailToCMail(CDBMail* pDBMail, CMail* pMail);
	// �ʼ���Ʒ����ת��
	void CDBMailGoodsToCGoods(CDBMailGoods* pDBMailGoods, CGoods* pGoods, const CGUID& ownerID);
	void CDBPlayerToCPlayerMails(CDBPlayer* dbPlayer, CPlayer* player);

	// ����Ϣȡ�ò�������
	CBaseDBEntity* FindEntityFromMsg(DB_OBJ_TYPE type, const CGUID& entityid, const CGUID& ownerID);
	// ��������						 
	CBaseDBEntity* CreateEntityByMsg(DB_OBJ_TYPE type, const CGUID& guid);

	// �ֲ�ת�� CDBPlayer �� CPlayer
	void DetailPropertyCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	void DetailGoodsGroupCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	//void DetailEquipmentCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	//void DetailOrignalPackCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	//void DetailSubpackCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* player, long subPackPos);
	//void DetailWalletCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	//void DetailSilverCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	void DetailQuestCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	void DetailMerQuestCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);

	void DetailSkillCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	void DetailStateCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	void DetailVarDataCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);
	void DetailMedalCDBPlayerToCPlayer(CDBPlayer* dbPlayer, CPlayer* pPlayer);

	// �ֲ�ת�� CPlayer �� CDBPlayer
	void DetailPropertyCPlayerToCDBPlayer	(CPlayer* player, CDBPlayer* dbPlayer);
	void DetailGoodsGroupCPlayerToCDBPlayer	(CPlayer* player, CDBPlayer* dbPlayer);
	//void DetailEquipmentCPlayerToCDBPlayer	(CPlayer* player, CDBPlayer* dbPlayer);
	//void DetailOrignalPackCPlayerToCDBPlayer(CPlayer* player, CDBPlayer* dbPlayer);
	//void DetailSubpackCPlayerToCDBPlayer	(CPlayer* player, CDBPlayer* dbPlayer, long subpackPos);
	//void DetailWalletCPlayerToCDBPlayer		(CPlayer* player, CDBPlayer* dbPlayer);
	//void DetailSilverCPlayerToCDBPlayer		(CPlayer* player, CDBPlayer* dbPlayer);
	void DetailQuestCPlayerToCDBPlayer		(CPlayer* player, CDBPlayer* dbPlayer);
	void DetailMerQuestCPlayerToCDBPlayer	(CPlayer* player, CDBPlayer* dbPlayer);//
	void DetailSkillCPlayerToCDBPlayer		(CPlayer* player, CDBPlayer* dbPlayer);
	void DetailStateCPlayerToCDBPlayer		(CPlayer* player, CDBPlayer* dbPlayer);
	void DetailVarDataCPlayerToCDBPlayer	(CPlayer* player, CDBPlayer* dbPlayer);
	void DetailMedalCPlayerToCDBPlayer		(CPlayer* player, CDBPlayer* dbPlayer);
	//void DetailDepotCPlayerToCDBPlayer		(CPlayer* player, CDBPlayer* dbPlayer, eDepotContainerType eCContainerType);
	//void DetailSubDepotCPlayerToCDBPlayer	(CPlayer* player, CDBPlayer* dbPlayer, eDepotContainerType eCContainerType);

	//!					��DB�д�����񣨰��������ж�ȡ�ı����Ϣ������Ϊÿ����񴴽�һ��ʱ��ۼ�����
	//!					Ϊ��ֹ��־��¼����ɾ����������DROP���
	// ��ʼ����������
	std::map<string/*attrname*/, CDataEntityManager*>& GetObjAttrDef(void)		{ return m_mapObjAttrDef; }

	CDBGenVarGroup*   GetGenVarGroup(void);
private:
	CDBGenVarGroup*   m_pTmpGenVarGroup;
	DBCardMap m_mapDBCards;
	DBLoginPlayerMap m_mapDBLoginPlayers;
	CdkeyDBLoginPlayerMap m_mapCdkeyLoginPlayers;
	DBPlayerMap m_mapDBPlayers;
	DBLinkmanMap  m_mapDBLinkman;
	//DBCityWarMap m_mapDBCityWars;
	DBFactionMap m_mapDBFactions;
	//DBVillageWarMap m_mapDBVillageWars;
	DBUnionMap m_mapDBUnions;
	DBCountryMap m_mapDBCountrys;
	//DBEnemyFactionMap m_mapDBEnemyFactions;
	DBGenVarMap m_mapDBGenVars;
	DBSetupMap m_mapDBSetups;
	DBRegionMap m_mapDBRegions;
	DBDupRgnMap m_mapDBDupRgns;

	// Obj������Ϣ,Obj�����������,id
	std::map<string, CDataEntityManager*> m_mapObjAttrDef;
};

//*************************
//���SomeThing log��Ϣ
extern void PutSomeThingLogInfo(const char *msg);
//*************************
// ������־
extern void SaveSomeThingLogText(bool bForce);
// ����һ����¼
extern void AddSomeThingLogText(char* msg, ...);

#endif//DB_ENTITY_MANAGER_H