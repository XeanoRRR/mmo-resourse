/**
*File:  EntityManager.h
*Brief: ������:�ö���������еĶ���ʵ���Ͷ����������Ϣ����
*Author:������
*Date:  2008.11.03
*/
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Account.h"

#include "../../../../Public/Common/RankDef.h"
class CDataEntityManager;
class CEntityGroup;
class CMessage;
class DBAdoBase;
class DBThread;
class DBAccessThread;
class DBMailThread;
class DBRankThread;

struct ThreadData;

#define MAX_DB_ENTITY_NUM 1024

// �����������ļ���Ϣ����
struct tagBuildFileInfo
{
	CGUID	leafGuid;           // Ҷ��GUID
    CGUID	rootGuid;           // ��GUID
    long	lComType;           // 1:LEAF   2:COMPOSITE
	long	lLevel;	
	string	strLeafComFlag;	    // Ҷ�Ӷ�������
	string	strRootComFlag;	    // ����������
	


	tagBuildFileInfo()
		:leafGuid(NULL_GUID),rootGuid(NULL_GUID),lLevel(0),lComType(-1),
		strLeafComFlag(""),strRootComFlag("")
	{
	}

	~tagBuildFileInfo()
	{
	}
	void AddToByteArray(vector<BYTE>& pBA)
	{
		_AddToByteArray(&pBA, leafGuid);
		_AddToByteArray(&pBA, lLevel);
		_AddToByteArray(&pBA, lComType);
		_AddToByteArray(&pBA, strLeafComFlag.c_str());
		_AddToByteArray(&pBA, strRootComFlag.c_str());
		_AddToByteArray(&pBA, rootGuid);
	}

};
struct tagEntityAttrInfo
{
	tagEntityAttrInfo()
		:eNumDataType(DATA_OBJECT_UNKNOWN),lMaxSize(0),lUseage(0),lAttrTypeEnum(0)
	{
    }
	~tagEntityAttrInfo()
    {
    }

	// ��������ö��ֵ
	long lAttrTypeEnum;
	// ��󳤶�
	long lMaxSize;
	// �÷�
	long lUseage;
	DATA_OBJECT_TYPE eNumDataType;
};
struct tagDbIdxInfo
{
	tagDbIdxInfo::tagDbIdxInfo()
		: strIndex(""),lUnique(0)
	{
	}

	tagDbIdxInfo::tagDbIdxInfo(const string& stridx, long lUni)
		: strIndex(stridx),lUnique(lUni)
	{
	}

	tagDbIdxInfo::~tagDbIdxInfo()
	{
	}

	string  strIndex;
	long    lUnique;
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
	string							strPrimarykey;		// MEM����ֵ��
	string							strDBPrimarykey;	// DB����ֵ��
	long							lHhasAttrFlag;		// �Ƿ�������
	long							lLeafComType;		// Ҷ������
	string							strLeafComFlag;		// Ҷ������
	long							lHasDetailLeaves;	// �Ƿ��з�ɢҶ�ӽڵ�(����goodsgroup)
	string							strDbQueryName;		// ���ݿ���ұ�����
	long							lDbQueryType;		// ���ݿ���ұ�������
	vector<tagDbIdxInfo>			vecIndex;		    // ���ݱ�ۼ�������
	vector<tagDbIdxInfo>			vecNoCluIndex;	    // ���ݱ�Ǿۼ�������
	vector<tagDbIdxInfo>			vecCoIndex;		    // ���ݱ���������
	map<string, tagEntityAttrInfo>	pDataMgr;			// ��������
	map<string, tagBuildFileInfo*>	mapBuildFileInfo;	// �ṹ����
	vector<long>					vecAttrEnum;		// ��˳�������ö��ֵvector
	BYTE							pQueryVar[256];		// ���ݿ���ұ���������Ϣ

	void AddToByteArray(vector<BYTE>& pBA)
	{
		_AddToByteArray(&pBA, lComType);
		_AddToByteArray(&pBA, strComFlag.c_str());
		_AddToByteArray(&pBA, lHeight);
		_AddToByteArray(&pBA, guid);
		_AddToByteArray(&pBA, strTblName.c_str());
		_AddToByteArray(&pBA, lDbOperFlag);
		_AddToByteArray(&pBA, lLeafNum);
		_AddToByteArray(&pBA, strPrimarykey.c_str());
		_AddToByteArray(&pBA, lHhasAttrFlag);
		_AddToByteArray(&pBA, lLeafComType);
		_AddToByteArray(&pBA, strLeafComFlag.c_str());
		_AddToByteArray(&pBA, lHasDetailLeaves);

		_AddToByteArray(&pBA, strDbQueryName.c_str());		//���ݿ���ұ�����	
		if(strDbQueryName == "NULL")
			_AddToByteArray(&pBA, (BYTE)0);
		else
		{
			_AddToByteArray(&pBA, (BYTE)1);
			_AddToByteArray(&pBA, (long)lDbQueryType);//���ݿ���ұ�������
			_AddToByteArray(&pBA, pQueryVar, sizeof(pQueryVar));
		}

		_AddToByteArray(&pBA, (WORD)pDataMgr.size()); // �����������
		map<string, tagEntityAttrInfo>::iterator itr = pDataMgr.begin();
		for(; itr != pDataMgr.end(); itr++)
		{
			_AddToByteArray(&pBA, itr->first.c_str());//�������ַ���
			_AddToByteArray(&pBA, (long)itr->second.lAttrTypeEnum);//����ö��ֵ
			_AddToByteArray(&pBA, (long)itr->second.eNumDataType);//�������ͣ�long��
		}

		_AddToByteArray(&pBA, (WORD)mapBuildFileInfo.size());
		map<string, tagBuildFileInfo*>::iterator bfInfoItr = mapBuildFileInfo.begin();
		for(; bfInfoItr != mapBuildFileInfo.end(); bfInfoItr++)
		{
			bfInfoItr->second->AddToByteArray(pBA);
		}
	}
};

class CEntityManager
{
public:
	CEntityManager();
	~CEntityManager();

	typedef map<string, map<CGUID, CEntityGroup*>> DBEntityComponentMap;
	typedef DBEntityComponentMap::iterator DBEntityComponentMapItr;

	typedef map<string, Account*> AccountMap;
	typedef AccountMap::iterator AccountMapItr;

	typedef map<string, tagEntityBuildInfo*> AttrDefMap;
	typedef AttrDefMap::iterator AttrDefMapItr;

	// ��ʼ��DBThread
	void InitDBThread(string strProvider, 
		string strDataSource, 
		string strInitialCatalog,
		string strUserID, 
		string strPassword);

	void ProcessMsg(CMessage* msg);

	//-------------- interfaces
	// CBaseEntity
	// ���һ��BaseEntity��Mapĩβ
	void AddOneRootEntity(CEntityGroup* entity);
	// ɾ��һ��BaseEntity
	void DelOneRootEntity(CEntityGroup* entity);
	// ɾ��һ��BaseEntity
	void DelOneRootEntity(const string& comFlag, const CGUID& playerID);
	// �������
	void ClearBaseEntityMap(void);

	DBEntityComponentMap&		GetBaseEntityMap(void)	{ return m_mapEntitys; }
	CEntityGroup*				FindBaseEntity(const string& comFlag, const CGUID& guid);

	AccountMap&					GetAccountMap(void)		{ return m_mapAccounts; }

	// ���һ��Account��Mapĩβ
	bool						AddAccount(Account* acc);
	// ɾ��һ��Account
	void						DelAccount(const char* szCdkey);
	Account*					FindAccount(const string& strCdkey);

	// ����ID
	bool			GenerateGUID(CGUID& guid);

	// ����Ϣȡ�ò�������
	CEntityGroup*	FindRootEntity(const string& flag, const CGUID& guid);
	// ��������
	CEntityGroup*	CreateRootEntity(const string& flag, const CGUID& guid);

	//! �������ж��󣬲�����Ϣ
	void			TravelForGenerateThreadData(void);
	//! ������̷߳��ص���Ϣ
	void			ProcessRetThreadData(void);

	//! ȡ�ô洢���̵Ķ���ָ��
	CEntityGroup*	ReleaseProcedurePtr(const CGUID& guid);
	//! ȡ��ָ�����Ͷ���ָ��
	CEntityGroup*	ReleaseEntityPtr(const string& strComFlag, const CGUID& guid);
	//! ȡ��account����ָ��
	CEntityGroup*	ReleaseAccountPtr(const string& szCdkey);

	//! ��Ҫ�ֶ��ͷŸýӿڷ�����ڴ�
	CBaseEntity*	NewBaseEntity(const string& strComFlag, const CGUID& guid);
	//! �����ڴ���սӿ�
	void			DelBaseEntity(CBaseEntity** pEntity);

	//!	������
	bool			ClearEntitys(void);

public:
	map<long, DBThread*>&	GetLoadThreadMap(void)		{ return m_mapLoadThread; }
	map<long, DBThread*>&	GetSaveThreadMap(void)		{ return m_mapSaveThread; }
	DBAccessThread*				GetAccessThread(void)		{ return m_pAccessThread; }
    DBRankThread*           GetRankThread(void)         { return m_pRankThread; }

private:
	map<long, DBThread*>	m_mapLoadThread;
	map<long, DBThread*>	m_mapSaveThread;
	DBAccessThread*				m_pAccessThread;
	DBMailThread*				m_pMailThread;
    DBRankThread*           m_pRankThread; 

public:
	//!	ѹ��һ��ThreadData
	bool						PushLoadThreadData(ThreadData* loadData);
	bool						PushSaveThreadData(ThreadData* saveData);

	//!	ѹ����ThreadData
	bool						PushLoadThreadData(list<ThreadData*>& loadDataList);
	bool						PushSaveThreadData(list<ThreadData*>& saveDataList);
	//!	ȡ��һ��ThreadData
	bool						GetThreadData(ThreadData** loadData, long dwThreadID, eThreadType type);
	//! ѹ��һ������ThreadData
	bool						PushRetThreadData(ThreadData* loadData);
	bool						GetRetThreadData(list<ThreadData*>& loadDataList);
	long						GetSaveThreadOperDataSize(list<CGUID>& saveBeginGuidList);
	//! ����һ�����ڱ��̴߳����guid
	bool						PopBeginEntityGuid(const CGUID& guid, eThreadType type);
	
	//! ȡ��LoadThreadDataSize
	long						GetLoadThreadDataSize(void);

	//!	���صȴ������������
	size_t						GetLoadWaitOperEntityNum(void) { return m_listLoadThreadOperData.size(); }
	size_t						GetSaveWaitOperEntityNum(void) { return m_listDirtyEntityGuid.size() + m_listDirtyEntityName.size() + m_listSaveThreadOperData.size(); }
	//!	ѹ��һ��ThreadData
	bool						PushMailThreadData(list<ThreadData*>& loadDataList);
	//!	ѹ��һ��ThreadData
	bool						PushMailThreadData(ThreadData* loadData);
	//!	ȡ��һ��ThreadData
	bool						GetMailThreadData(list<ThreadData*>& loadDataList, long dwThreadID);
	bool						SetMailThreadData(list<ThreadData*>& loadDataList);

	//! ѹ��һ���߳��Ƴ��¼����
	bool						PushThreadQuitEventHandle(HANDLE& handle);
	//
	bool						AddMailThreadQuitEventHandle(HANDLE& handle);

	//!	����LoadEvent
	static void					SetLoadThreadEvent(void);
	//!	�ȴ�LoadEvent
	static void					WaitLoadThreadEvent(void);
	//! ����LoadEvent
	static void					ResetLoadThreadEvent(void);

	//!	����SaveEvent
	static void					SetSaveThreadEvent(void);
	//!	�ȴ�SaveEvent
	static void					WaitSaveThreadEvent(void);
	//! ����SaveEvent
	static void					ResetSaveThreadEvent(void);

	//!	����MailEvent
	static void					SetMailThreadEvent(void);
	//! �ȴ�MailEvent
	static void					WaitMailThreadEvent(void);
	//! ����MailEvent
	static void					ResetMailThreadEvent(void);

	//!	��DB�д�����
	void						CreateOtherTableInDb(const string& tableName, const string& iniName);
    /// �����Ƿ���ڣ����򴴽�
    void                        CheckTableInDb(const string& strTableName, const string& strInitName);
	
	//!	�����ۼ�/�Ǿۼ�����
	void						CreateOtherTableIndexInDb(const string& tableName, vector<tagDbIdxInfo>& vecIdx, bool isClustered);
	//!	������������
	void						CreateOtherTableComIndexInDb(const string& tableName, vector<tagDbIdxInfo>& vecIdx);

	//!	���ݶ��󴴽���Insert��Update�洢���̣���������m_mapTblProc���Թ���ѯ
	void						CreateTblProcedure(CBaseEntity* pRoot);
    /// �����洢����
    void                        CreateProcedure(const string& strProcedureName, const string& strCreateSQL);
	//!	��ѯһ������ĳ���洢������
	const string&				FindTblProcedure(CBaseEntity* pRoot);
	//! ���һ������Ĵ洢����
	void						AddTblProcedure(CBaseEntity* pRoot, DB_OPERATION_TYPE opType, const string& name);
	//!	����һ�������ʱ�ʼ��Ĵ洢����
	void						RunDeleteTimeoutMailProc(void);
	//! ��ʼ����������
	void						InitPropertyIniData();
    void                        InitRanksData();
    void                        LoadRanksFile(const char* szPath);
	//! ��ȡ���������ļ�
	const char* 				LoadObjAttrInfo(const char* szPath, bool bCheckTable = false);
	void						LoadObjStructInfo(const char* szPath);
	void						LoadProcedureInfo(const char* szPath);

	AttrDefMap&					GetObjAttrDef(void) { return m_mapObjAttrDef; }
	void						ExecSqlString(const string& sqlStr);
	void						Release(void);

	vector<string>& GetProcedureNameVec(void) { return m_vecProcedureName; }

	//------��Ҷ���������߽ӿ�--------
	// ����Ҷ���������������²����丸�ڵ�
	CEntityGroup*	FindLeafFatherByComFlag(CEntityGroup* pRoot, const string& leafComFlag);
	// ����Ҷ���������������²�����ڵ�
	CBaseEntity*	FindLeafByComFlag(CEntityGroup* pRoot, const string& leafComFlag, const CGUID& leafGuid);
	// ������ҽڵ���ĳЩ�ӽڵ�����ݿ������־λ
	void			SetEntityLeavesDbOperFlag(CEntityGroup* pBaseEntity, const string& leafComFlag, bool operFlag);
	// ����ҽڵ��������ӽڵ����ݿ������־λ����
	void			ResetEntityAllDbOperFlag(CBaseEntity* pBaseEntity, bool operFlag=false);
	// ���ôӸ���㿪ʼ�����нڵ����ݿ������־
	void			SetEntityAllDbOperType(CBaseEntity* pBaseEntity, DB_OPERATION_TYPE opType);

	// ����һ��EntityGroup�Լ����ӽ���Size
	long			ComputeEntityGroupMemSize(CEntityGroup* pBaseEntity);
	// ����Ҷ���������������²�����ڵ�
	CEntityGroup*	FindLeafGroupByComFlag(CEntityGroup* pRoot, const string& leafComFlag);


	// ӳ�������ַ�������ö��ֵ��
	void				AddAttrEnumAndStr(const string& strName, long lEnum);
	long				GetAttrEnumByStr(const string& strComFlag, const string& strName);
	const string&		GetAttrStrByEnum(long lEnum);
	// ȡ�������ļ���������Ϣ
	tagEntityBuildInfo* GetEntityBuildInfo(const string& strComFlag);
	// DATA_OBJECT_TYPE
	long				GetDataObjectType(const string& strComFlag, const string& strName);
	long				GetDataObjectType(const string& strComFlag, long lEnum);
	// ���ݿ����Ա�־λ: 1|0: 1(���ݿ��Զ�����) 0�������(Ĭ��)
	BYTE				GetDbUseageFlag(const string& strComFlag, const string& strName);
	BYTE				GetDbUseageFlag(const string& strComFlag, long lEnum);
	// ���ݿ���󳤶�
	long				GetBufMaxSize(const string& strComFlag, const string& strName);
	long				GetBufMaxSize(const string& strComFlag, long lEnum);

	// ���ݶ���Ľڵ���Ϣ������ѯ����
	bool				GenerateQuerySavePlayerInfo(CEntityGroup* pPlayer);
	//	���ظ��Ĳ���һ���仯�����GUID
	void				AddDirtyEntityGuid(const CGUID& guid);
	void				AddDirtyEntityName(const char* szName);

	// ����ȫ������
	void				SaveAllEntity(void);

	//! ȡ�����ڱ��̲߳����Ķ�������
	size_t				GetBeginLoadEntityNum(void)	{ return m_listLoadBeginEntityGuid.size(); }
	size_t				GetBeginSaveEntityNum(void)	{ return m_listSaveBeginEntityGuid.size(); }

	//! �����Ƿ��ڲ��������и��˺�
	bool				FindAccountIsInOperQueue(const string& strAcc);
	//! �����Ƿ��ٲ��������иö���
	bool				FindEntityIsInOperQueue(const CGUID& guid);
    // ��ȡ����������Ϣ
    map<long, tagRankConfig>& GetRankMap() { return m_mapRank; }
    // ��ȡ������Ϣ
    map<long, tagRankInfo*>& GetRankInfo() { return m_mapRankInfo; }        
    void ReleaseRankInfo();
    void ClearRankInfo(long lRankID);
    // ���¼���ˢ��������Ϣ
    void ReLoadRanks();
private:
	// ���ر��ֶ�����
	void LoadObjInfo(vector<const char*>& AttrInfoList,
		const char* szStructInfo,
		const char* szTable);

	
	CRITICAL_SECTION	m_LoadCriticalSection;      // SQL����Load�ٽ����	
	CRITICAL_SECTION	m_SaveCriticalSection;      // SQL����Save�ٽ����	
	CRITICAL_SECTION	m_MailCriticalSection;      // �ʼ������ٽ����	
	CRITICAL_SECTION	m_RetCriticalSection;       // �̷߳��ض����ٽ����	
	CRITICAL_SECTION	m_RetMailCriticalSection;   // �ʼ��̷߳��ض����ٽ����
	CRITICAL_SECTION	m_ThreadQuitCS;

	DBEntityComponentMap	m_mapEntitys;
	AccountMap				m_mapAccounts;

	
	list<CGUID>		m_listDirtyEntityGuid;      // ��¼�б仯�Ķ���
	list<string>	m_listDirtyEntityName;

	
	list<CGUID>		m_listLoadBeginEntityGuid;  // ��¼���ڱ��̲߳����Ķ���
	list<CGUID>		m_listSaveBeginEntityGuid;

	
	list<ThreadData*> m_listLoadThreadOperData;     // Ͷ�ݵ��̵߳Ĳ�������
	list<ThreadData*> m_listSaveThreadOperData;	
	list<ThreadData*> m_listThreadRetData;          // ��ȡ��洢���̷�������

	
	static HANDLE		m_hLoadThreadEvent;     // Load�¼����	
	static HANDLE		m_hSaveThreadEvent;     // Save�¼����	
	static HANDLE		m_hMailThreadEvent;     // �ʼ��߳��¼����

	// Obj������Ϣ,Obj�����������,id
	AttrDefMap			m_mapObjAttrDef;

	vector<string> m_vecProcedureName;

	vector<HANDLE> m_vecThreadQuitEvent;

	// �Զ��������Ե�ö��ֵ
	long m_lAttrEnumValue;
	// ӳ�������ַ�������ö��ֵ��
	map<long, string> m_mapAttrEnumToStr;

	
	list<ThreadData*> m_listMailThreadData;     // �ʼ�������ȫ����	
	list<ThreadData*> m_listMailThreadRetData;  // �ʼ�������������

	HANDLE	m_MailThreadQuitEvent;
	map<string, map<DB_OPERATION_TYPE,string>> m_mapTblProc;
    map<long, tagRankConfig> m_mapRank;
    map<long, tagRankInfo*> m_mapRankInfo;
 //   CRITICAL_SECTION    m_csRankInfo;           // ���а���Ϣ�ٽ����
};

extern string NULL_STRING;

#endif//DB_ENTITY_MANAGER_H