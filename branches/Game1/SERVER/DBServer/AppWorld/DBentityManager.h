#ifndef DB_ENTITY_MANAGER_H
#define DB_ENTITY_MANAGER_H
#include "../public/BaseDef.h"
#include "../public/DBDefine.h"
#include "DBEntityDef.h"
#include "..\..\..\dbaccess\myadobase.h"


class CBaseDBEntity;
class CDBCard;
class CDBGoods;
class CDBLoginPlayer;
class CDBPlayer;
class CDBFaction;
class CDBCityWar;
class CDBVillageWar;
class CDBUnion;
class CDBCountry;
class CDBEnemyFaction;
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
class CDBMail;
class CDBMailGroup;
class CDBMailGoods;
class CDBMailGoodsGroup;
class CDBGenVarGroup;

class DBAdoBase;
class DBLoadThread;
class DBSaveThread;
class DBAccessThread;

struct LoadObj;
struct SaveObj;
struct LoadThreadData;
struct SaveThreadData;
#include <set>
#include <map>

#define MAX_DB_ENTITY_NUM 1024

class CDataEntityManager;

class CDBEntityManager
{
public:
	CDBEntityManager();
	~CDBEntityManager();

	// CDBLoginPlayer
	typedef std::map<CGUID, CDBLoginPlayer*> DBLoginPlayerMap;
	typedef DBLoginPlayerMap::iterator DBLoginPlayerMapItr;
	// CDKEY ��Ψһ�� ͬһ��CDBLoginPlayerӵ�ж��CDBPlayer����
	typedef std::map<string, set<CGUID>> CdkeyDBLoginPlayerMap;
	typedef CdkeyDBLoginPlayerMap::iterator CdkeyDBLoginPlayerMapItr;

	// CDBPlayer
	typedef std::map<CGUID, CDBPlayer*> DBPlayerMap;
	typedef DBPlayerMap::iterator DBPlayerMapItr;

	// CDBFaction
	typedef std::map<CGUID, CDBFaction*> DBFactionMap;
	typedef DBFactionMap::iterator DBFactionMapItr;
	// CDBCityWar
	typedef std::map<CGUID, CDBCityWar*> DBCityWarMap;
	typedef DBCityWarMap::iterator DBCityWarMapItr;
	// CDBVillageWar
	typedef std::map<CGUID, CDBVillageWar*> DBVillageWarMap;
	typedef DBVillageWarMap::iterator DBVillageWarMapItr;
	// CDBUnion
	typedef std::map<CGUID, CDBUnion*> DBUnionMap;
	typedef DBUnionMap::iterator DBUnionMapItr;
	// CDBCountry
	typedef std::map<CGUID, CDBCountry*> DBCountryMap;
	typedef DBCountryMap::iterator DBCountryMapItr;
	// CDBEnemyFaction
	typedef std::map<CGUID, CDBEnemyFaction*> DBEnemyFactionMap;
	typedef DBEnemyFactionMap::iterator DBEnemyFactionMapItr;

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

	// ��ʼ��DBThread
	void InitDBThread(string strProvider, 
		string strDataSource, 
		string strInitialCatalog,
		string strUserID, 
		string strPassword);

	void ProcessMsg(CMessage* msg);

	//-------------- interfaces

	// CDBLoginPlayer
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
	// ���һ��CDBLoginPlayer��Mapĩβ
	void AddLoginPlayerByCdkey(CDBLoginPlayer* player);
	// ɾ��һ��CDBLoginPlayer
	void DelLoginPlayerByCdkey(CDBLoginPlayer* player);
	// �������
	void ClearCdkeyLoginPlayerMap(void);
	// ȡ�ø���
	CdkeyDBLoginPlayerMap& GetCdkeyLoginPlayerMap(void) { return m_mapCdkeyLoginPlayers; }
	set<CGUID>* FindLoginPlayerByCdkey(const char* szCdkey);

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

	//!							�����Զ�������б�
	map<CGUID, vector<BYTE>>	m_mapFactionIcon;

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
	void ClearDBLinkmanMap(const CGUID& ownerID);
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

	// ����Ϣȡ�ò�������
	CBaseDBEntity* FindEntityFromMsg(DB_OBJ_TYPE type, const CGUID& entityid, const CGUID& ownerID);
	// ��������
	CBaseDBEntity* CreateEntityByMsg(DB_OBJ_TYPE type, const CGUID& guid);

	// ���߳����ݿ����ӹ������
public:
	map<long, DBLoadThread*>& GetLoadThreadMap(void) { return m_mapLoadThread; }
	map<long, DBSaveThread*>& GetSaveThreadMap(void) { return m_mapSaveThread; }
	DBLoadThread* FindLoadThread(long id);
	DBSaveThread* FindSaveThread(long id);
	DBAccessThread* GetAccessThread(void) { return m_pAccessThread; }

	// ���ڴ�����֤���Account�Ƿ����
	bool	ValidatePlayerIDInCdkey(const char* szCDKEY, const CGUID& guid);
private:
	map<long, DBLoadThread*>	m_mapLoadThread;
	map<long, DBSaveThread*>	m_mapSaveThread;
	DBAccessThread*				m_pAccessThread;

public:
	//*************************
	//���ָ������ log��Ϣ
	void PutSomeThingLogInfo(const char *msg);
	//*************************
	// ������־
	void SaveSomeThingLogText(bool bForce);
	// ����ָ������ һ����¼
	void AddSomeThingLogText(char* msg, ...);

	//!	ѹ��һ��LoadThreadData
	bool PushLoadThreadData(LoadThreadData* loadData);
	//!	����һ��LoadThreadData
	bool PopLoadThreadData(LoadThreadData** loadData,long dwThreadID);
	//!	ѹ��һ���ѱ������LoadThreadData
	bool PushDisposedLoadThreadData(LoadThreadData* loadData);
	//!	ѹ�����ѱ������LoadThreadData
	bool PushDisposedLoadThreadData(std::list<LoadThreadData*>& loadDataList);
	//! ����һ���ѱ������LoadThreadData
	bool PopDisposedLoadThreadData(std::list<LoadThreadData*>& loadDataList);

	//!	ѹ��һ��SaveThreadData
	bool PushSaveThreadData(SaveThreadData* saveData);
	//!	����һ��SaveThreadData
	bool PopSaveThreadData(std::list<SaveThreadData*>& saveDataList);
	//!	ѹ�����ѱ������SaveThreadData
	bool PushDisposedSaveThreadData(SaveThreadData* saveData);
	//! ����һ���ѱ������SaveThreadData
	bool PopDisposedSaveThreadData(SaveThreadData** saveData);

	//!								����LoadEvent
	static void						SetLoadEvent(void);
	//!								�ȴ�LoadEvent
	static void						WaitLoadEvent(void);
	static void						ResetLoadEvent(void);

	//!								����SaveEvent
	static void						SetSaveEvent(void);
	//!								�ȴ�SaveEvent
	static void						WaitSaveEvent(void);
	static void						ResetSaveEvent(void);

	//!					��DB�д�����񣨰��������ж�ȡ�ı����Ϣ������Ϊÿ����񴴽�һ��ʱ��ۼ�����
	//!					Ϊ��ֹ��־��¼����ɾ����������DROP���
	void				CreateOtherTableInDb(const string& tableName, const string& iniName, bool isIndexed=true);
	// ��ʼ����������
	void				InitPropertyIniData();
	void				LoadObjStructInfo(const char* szPath);
	
	std::map<string/*attrname*/, CDataEntityManager*>& GetObjAttrDef(void)		{ return m_mapObjAttrDef; }


	void				ExecSqlString(const string& sqlStr);

	// Ԥ������
	void PreLoadPlayerBase();
	// Ԥ������
	void PreLoadPlayerAttrs();

	CDBGenVarGroup*   GetGenVarGroup(void);
private:
	CDBGenVarGroup*   m_pTmpGenVarGroup;

	//!					SQL�����ٽ����
	CRITICAL_SECTION	m_LoadCriticalSection;
	CRITICAL_SECTION	m_DisposedLoadCriticalSection;

	//!					SQL�����ٽ����
	CRITICAL_SECTION	m_SaveCriticalSection;
	CRITICAL_SECTION	m_DisposedSaveCriticalSection;

	DBLoginPlayerMap m_mapDBLoginPlayers;
	CdkeyDBLoginPlayerMap m_mapCdkeyLoginPlayers;
	DBPlayerMap m_mapDBPlayers;

	DBLinkmanMap  m_mapDBLinkman;
	//DBCityWarMap m_mapDBCityWars;
	//!							�����б�
	DBFactionMap				m_mapDBFactions;

	//DBVillageWarMap m_mapDBVillageWars;
	DBUnionMap m_mapDBUnions;
	DBCountryMap m_mapDBCountrys;
	//DBEnemyFactionMap m_mapDBEnemyFactions;
	DBGenVarMap m_mapDBGenVars;
	DBSetupMap m_mapDBSetups;
	DBRegionMap m_mapDBRegions;
	DBDupRgnMap m_mapDBDupRgns;

	// ��������Ķ�ȡ����
	std::list<LoadThreadData*> m_listLoadThreadData;
	// �ѱ�����Ķ�ȡ����
	std::list<LoadThreadData*> m_listDisposedLoadThreadData;

	// ���������д������
	std::list<SaveThreadData*> m_listSaveThreadData;
	// �ѱ������д������
	std::list<SaveThreadData*> m_listDisposedSaveThreadData;
	//!					Load�¼����
	static HANDLE		m_hLoadEvent;
	//!					Save�¼����
	static HANDLE		m_hSaveEvent;

	// Obj������Ϣ,Obj�����������,id
	std::map<string, CDataEntityManager*> m_mapObjAttrDef;
};

#endif//DB_ENTITY_MANAGER_H