#pragma once

#include "Area.h"
#include "Region.h"


class CShape;
class CPlayer;
class CMonster;
class CPet;			// Fox@20081121 ����
class CBuild;
class CGate;
class CCollection;
class CArea;
class CMessage;
class CMoveShape;
class CGoods;
class CNpc;
class VariableList;
class CColony;

typedef enum eRegionWarType
{
	RWT_Normal=0,	//��ͨ����
	RWT_NormalWar,	//��ͨս��
	RWT_VillageWar,	//��ս
	RWT_CityWar		//��ս
}eRWT;

//�����ռ�����
typedef enum eRegiogSpaceType
{
	RST_Land=0,	//½��
	RST_Warter,	//ˮ��
	RST_Sky,	//���
}eRST;

//��ǰ���е�״̬
enum eCityState
{
	CIS_NO,					
	CIS_DUTH,			//��ս״̬
	CIS_Mass,			//����״̬
	CIS_Fight,			//�óǽ���ս��״̬
};
//�Գ��ŵĲ�������
enum eOperCityGate
{
	OC_Open=0,
	OC_Close,
	OC_Refresh,
};

//˰
struct tagRegionParam
{
	long  lID;							//������ID
	long  lMaxTaxRate;					//���˰��
	long  lCurrentTaxRate;				//��ǰ˰��
	long  lYuanbaoTaxRate;				//Ԫ��˰��
	long  lBusinessTaxRate;				//����˰��
	DWORD dwTotalTax;					//˰���ܺ�
	DWORD dwBusinessTotalTax;			//����˰���ܺ�
	DWORD dwTodayTotalTax;				//����˰���ۼ�ֵ
	long  lSupRegionID;					//����˰��ĳ���ID(0��ʾ���Ͻ�˰��)
	long  lTurnInTaxRate;				//�Ͻɵ�����������˰��
	CGUID  OwnedFactionID;				//ӵ����������İ��
	CGUID  OwnedUnionID;				//ӵ�����������ͬ��ID 
};

class CServerRegion :
	public CRegion
{

public:
	CServerRegion(void);
	CServerRegion(long id);
	CServerRegion(const CGUID& guid);
	CServerRegion(void* pParam);
	CServerRegion(const CGUID& guid, bool bNoPk);
	virtual ~CServerRegion(void);
private:
	hash_map<CGUID, CShape*, hash_guid_compare>		m_mOtherShapes;
	hash_map<CGUID, CCollection*, hash_guid_compare> m_mCollection;
	hash_map<CGUID, CBuild*, hash_guid_compare> m_mBuild;
	hash_map<CGUID, CMonster*, hash_guid_compare>	m_mMonsters;
	hash_map<CGUID, CPet*, hash_guid_compare>			m_Pets;		// ����
	hash_map<CGUID, CGoods*, hash_guid_compare>		m_mGoods;

	typedef hash_map<CGUID, CGoods*, hash_guid_compare>::iterator goodIt;
	hashsetGUID		m_setPlayers;
	map<DWORD,CColony*>		m_mapColony;
public:
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//����ʱ����ȡ���ĵ���
	virtual void OnTimerCancel(DWORD timerid,const void* var);
private:
	void AddRebornTimerID(CGUID guid,long lTimerID);
	void DelReboreTimerID(CGUID guid);
	typedef map<CGUID,long>		mapGuid2Long;
	typedef map<CGUID,long>::iterator itGuid2Long;
	mapGuid2Long m_mReBornTimerIDList;

	map<string, tagPty>		m_mapNameValue;		// �������ֶ�Ӧ�ı�����ַ

	void InitNameValueMap();	// ��ʼ��
public:
	size_t GetPlayerAmout() { return m_setPlayers.size(); }

	long GetCampPlayerNum(eCampType type);

	long GetID(void) { return m_ID; }
	void SetID(long id) { m_ID = id; }

	bool  SetAttribute(char* attrName, long value);
    long  GetAttribute(char* attrName);
    bool  ChangeAttribute(char* attrName, long value);
private:
	long        m_ID;
	BYTE		m_bNoPk;		// �����Ƿ�����PK
	bool		m_bNoContribute;
	DWORD       m_dwAreaId;
	//�Ƿ���������
	bool	m_Ride;
	//�Ƿ���Ըı�װ��
	bool	m_ChangeEquip;
	//�����Ŀռ�����
	eRegiogSpaceType    m_eSpaceType;
	eRegionWarType		m_WarRegionType;			//ս����������
	//ԭ�ظ����־��
	bool	m_bRelive;
	////���߻سǱ�־��
	bool    m_bDeadReturn;

	
public:
	BYTE		IsNoPk()		{ return m_bNoPk; }
	bool		IsNoContribute(){ return m_bNoContribute; }
	void		SetAreaId(DWORD dwId){m_dwAreaId=dwId;}
	DWORD		GetAreaId(){return m_dwAreaId;}

//////////////////////////////////////////////////////////////////////////
//	Region ��������
//////////////////////////////////////////////////////////////////////////
public:
	VariableList* GetVariableList(void);
	void SetVariableList(VariableList* vl);
	void UpdateVariableList(VariableList* vl);
private:
	VariableList* m_pVariableList;
//////////////////////////////////////////////////////////////////////////
//	��ͼMonster����
//////////////////////////////////////////////////////////////////////////
public:

	// ˢ�µĹ��Ｘ��
	struct tagMonsterList
	{
		WORD wOdds;				// ˢ�¼���(n%)
		WORD wSign;				// ����MonsterΨһ��ʶ
		WORD wLeaderSign;		// ����Monster�쵼��ʶ(ע:��Ϊ�쵼��Monster������Ӧ��ֻ��һ��)
		WORD wLeaderDistance;	// ���쵼�߱��ֵ�������
		WORD wRoadIndex;		// ·����ʶ
		string strName;			// ����
		string strScript;		// �ű�
		string strTalkScript;	// �Ի��ű�
	};

	// Monster
	struct tagMonster
	{
		long lIndex;			// ���
		RECT rectRange;			// ��Χ(����)
		long lNum;				// ����
		long lResetTime;		// ˢ������(��)
		long lStartTime;		// ˢ����ʼʱ��
		long lDir;				// ����(-1=���)
		vector<tagMonsterList> vectorMonsterList;
		DWORD dwLastResetTime;	// �ϴ�ˢ��ʱ��
		long lLivingCount;		// ��ˢ�������ڻ��ŵ�����

	};


protected:
	map<DWORD,tagMonster>		m_mpMonster;


public:
	BOOL AddMonsterRect(tagMonster&, long lNum, bool bGMCreate=false);		// ���һ��ˢ�������Monsters
	CMonster*	AddMonster(const char* strOrginName, long x, long y, long lDir, bool bGMCreate=false);		// ���һ��Monster
	BOOL		AddMonster(const char* strOrginName, long count, long left, long right, long top, long bottom);
	CMonster*	AddMonster(long lGID, long x, long y, long lDir, bool bGMCreate=false);		// ���һ��Monster
	//!			ɾ��ԭʼ��ΪpMonsterName�Ĺ������ɾ������
	long		DelMonster(const char *pMonsterName);
	
	tagMonster* GetMonsterRefeash(long lIndex);		// ��ȡˢ�¹���Ľṹָ��
	//void SetMonsterLeader();
	//void FreeMonsterLockPlayer(CPlayer* pPlayer);	// �ͷŸõ�ͼ��monster��pPlayer������
	void RefeashMonster();							// ˢ�µ�ͼ�ϵ�Monster
	
private:
	CGate* GetOneGate();
	CMonster* GetOneMonster();
	void DelMonster(CMonster* pMonster);
	
	void DelCollection(CCollection* pCollection);
	//�ӳ�����ɾ��һ��������monster����Ϊ��monster ע��һ�������¼�
	//void DeleteOneMonster(CMonster*pMonster);
	//�ڳ��������һ��monster
public:
	BOOL AddOneMonster(CMonster*pMonster);

//////////////////////////////////////////////////////////////////////////
//	Colony
//////////////////////////////////////////////////////////////////////////
public:
	CColony* GetOneColony(DWORD dwColonyIndex);
	void	ReleaseAllColony();

//////////////////////////////////////////////////////////////////////////
//	Collection
//////////////////////////////////////////////////////////////////////////
public:
	struct tagCollectionList
	{
		char cName[50];
		LONG lOdds;
	};

	struct tagCollection 
	{
		long lIndex;			// ���
		RECT rectRange;			// ��Χ(����)
		long lNum;				// ����
		long lResetTime;		// ˢ������(��)
		long lStartTime;		// ˢ����ʼʱ��
		long lDir;				// ����(-1=���)
		long lLivingCount;		// ��ˢ�������ڲɼ��������

		vector<tagCollectionList> vectorCollectionList;

	};
protected:
	list<tagCollection> m_listCollection;
public:
	BOOL AddCollectionRect(tagCollection&, long lNum);		// ���һ��ˢ�������Collection
	BOOL		AddCollection(const char* strOrginName, long count, long left, long right, long top, long bottom);
	BOOL			AddCollection_i (CCollection*collection ,long x,long y,long lDir);
	CCollection* AddCollection(const char* strOrginName,long x, long y, long lDir);

	BOOL RefreshCollection(CCollection* pCollection);
	tagCollection* GetCollectionRefeash(long lIndex);
protected:
	
	BOOL DeleteOneCollection(CCollection*);
	CCollection* GetOneCollection();
///////////////////////////////////////////////////////////
//          ��ͼ  door ���� 
///////////////////////////////////////////////////////////
public:
	struct tagGateProperty
	{
		long	lIndex;			//���
		long	lxPos;			//x����
		long	lyPos;			//y����
		long	lDir;			//����
		WORD	wOpenState;		//Ĭ�ϴ�״̬
		string	strOrigName;	//ԭʼ��
		string	strAttack;		//�ܹ����ű�
		string	strOpen;		//�򿪽ű�
		string	strClose;		//�رսű�
		string	strDestory;		//��ٽű�
		string	strRefresh;		//��ˢ�ű�
	};
	//���һ����
	CGate*		AddGate(tagGateProperty *pGateProperty);
	long		GetDoorState(long id);
	bool		SetDoorState(long id,WORD n);
	int			GetDoorDestroySwitch(long id);
	bool		SetDoorDestroySwitch(long id,bool n);
	long		GetDoorMaxHP(long id);
	bool		SetDoorMaxHP(long id,WORD n);
	long		GetDoorHP(long id);
	bool		SetDoorHP(long id,WORD n);
protected:
	map<long,tagGateProperty>m_mapGate;


//////////////////////////////////////////////////////////////////////////
//	NpcnameList
//////////////////////////////////////////////////////////////////////////
public:
	vector<BYTE>& GetNpcNameList(){return m_vectorNpcNameList;}			// ���NPC�����б�
	long GetNpcNameListNum();						// ��ȡNPC����

private:
	vector<BYTE>		m_vectorNpcNameList;
	long				m_lNpcNameListNum;

	///////////////////////////////////////////////////////////
	//          ��ͼ  �ݵ� ���� 
	///////////////////////////////////////////////////////////
public:
	struct tagStrongPoint
	{
		tagStrongPoint(void)
			:lPosX(0),lPosY(0),lState(0)
		{
			memset(szName, 0, 64);
		}
		~tagStrongPoint(void)
		{

		}

		long lPosX; // �ݵ�X����
		long lPosY; // �ݵ�Y����
		char szName[64]; // �ݵ�����
		long lState; // �ݵ�״̬
	};
	// ȡ�þݵ��Ƿ��� false:�ر� true������
	bool GetStrongPointFlag(void) { return m_bStrongPintFlag; }
	// ���þݵ��Ƿ���
	void SetStrongPointFlag(bool flag) { m_bStrongPintFlag = flag; }
	// ȡ�þݵ����
	long GetStronPointNum(void) { return (long)m_vecStrongPoint.size(); }
	// ȡ�þݵ�״ֵ̬
	long GetStrongPointState(const char* szName);
	// ���þݵ�״ֵ̬
	void SetStrongPointState(const char* szName, long lState);
	// ȡ�þݵ�����
	void GetStrongPointPos(const char* szName, long& lX, long& lY);
	// ȡ�þݵ�����
	std::vector<tagStrongPoint>& GetStrongPointVec(void) { return m_vecStrongPoint; }
private:
	bool m_bStrongPintFlag;
	std::vector<tagStrongPoint> m_vecStrongPoint;

//////////////////////////////////////////////////////////////////////////
//	��ͼ����
//////////////////////////////////////////////////////////////////////////
public:
	struct tagRegionSetup
	{
		tagRegionSetup()
		{
			// ��ʼ�������ݽṹ
			memset(this, 0, sizeof(tagRegionSetup));
		}
		bool	bReLiveIsUsed;	 // �Ƿ�ʹ��
		bool	bReLivePointFlag;//��������ͣ�type�� 0��ԭ�ظ��� 1��ָ����ͼ����
		long	lReLiveRegionID; //������ͼID
		long	lReLivePosX1;	 //��������귶Χ
		long	lReLivePosX2;
		long	lReLivePosY1;
		long	lReLivePosY2;

		bool	bLogoutIsUsed;	 // �Ƿ�ʹ��
		long	lLogoutRegionID; //���߻سǵ��ͼID
		long	lLogoutPosX1;	 //���߻سǵ����귶Χ
		long	lLogoutPosX2;
		long	lLogoutPosY1;
		long	lLogoutPosY2;

		bool	bRuneStoneIsUsed;	 // �Ƿ�ʹ��
		long	lRuneStoneRegionID;	//����ʯ�سǵ��ͼID
		long	lRuneStonePosX1;	//����ʯ�سǵ����귶Χ
		long	lRuneStonePosX2;
		long	lRuneStonePosY1;
		long	lRuneStonePosY2;
		bool	bRuneStoneCanRecord; // ����ʯ�سǵ��Ƿ���Լ�¼

		bool	bDupRgnIsUsed;	 // �Ƿ�ʹ��
		long	lDupRgnRegionID;//�����سǵ��ͼID
		long	lDupRgnPosX1;	//�����سǵ����귶Χ
		long	lDupRgnPosX2;
		long	lDupRgnPosY1;
		long	lDupRgnPosY2;

		bool	bReLiveCurPosFlag;	//ԭ�ظ���� 1//1���� 0����
		bool	bForbiddenFlag;		//���Ƹ���� 1//1���� 0����

		bool	bWarningFlag;	// �Ƿ񱨾�
	};

	tagRegionSetup& GetSetup(void) { return m_stSetup; }
protected:
	tagRegionSetup m_stSetup;
	set<string>	m_ForbidMakeGoods;									//�ڸóǾ��н�ֹʹ�õ���Ʒ
public:
	tagRegionSetup* GetRegionSetup(){return &m_stSetup;}
	bool FindForbidGood(const char* strOriName);					//���Ҹ���Ʒ�Ƿ��ǽ�ֹ��Ʒ

	virtual void GetReLiveReturnPoint(CPlayer* pPlayer, long &lRegion, long flag/*����һ�ָ��ʽ*/, long &l, long &t, long &r, long &b, long &d);
	virtual void GetLogoutReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);
	virtual void GetRuneStoneReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);
	virtual void GetDupRgnReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);

	virtual BOOL DoesRecallWhenLost(CPlayer* pPlayer);
	virtual bool BackToCity(CPlayer* pPlayer);						// ��player�س�

	//����Player���볡��
	void PlayerEnter(CPlayer* pPlayer,long lUseGoods);
	//����Player�˳�����
	void PlayerExit(CPlayer* pPlayer);

//////////////////////////////////////////////////////////
//	·��
//////////////////////////////////////////////////////////
protected:
	vector<vector<POINT>> m_vecRoadsList;

//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	virtual	void Run()		{CRegion::Run();};
	virtual BOOL Load();	// ��ȡ
	virtual BOOL Save();	// �洢
	virtual BOOL New();		// �½�

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	bool CodeToDataBlock(DBWriteSet& setWriteDB);		// ��ӵ�CByteArray
	bool		 AddSetupToByteArray(vector<BYTE>* pByteArray, bool bExData);
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	virtual bool DecordSetupFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����Setup
	virtual bool AddRegionParamToByteArray(vector<BYTE>* pByteArray, bool bExData = true);//��Ӳ�����ByteArray

	//void FreeMonsterLockedPlayer(CPlayer* pPlayer);	// �ͷŸõ�ͼ��NPC��pPlayer������

/////////////////////////////////////////////////////////////////////
// ��Ϣ��Ӧ	����ʵ���� RegionMessage.cpp ��
/////////////////////////////////////////////////////////////////////
public:
	virtual void OnMessage(CMessage*);

/////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////
public:
	virtual void ReBornObject(CBaseObject* pObject);			// ʹOBJECT����ˢ��
	virtual void AddObject(CBaseObject* pObject);				// ���OBJECT
	virtual void RemoveObject(CBaseObject* pObject);			// �Ƴ�OBJECT

	virtual void DeleteChildObject(long typeID, const CGUID& guid);		// ɾ����OBJECT
	virtual void DeleteChildObject(CBaseObject* p);				// ɾ����OBJECT

	virtual void DeleteAllChildObject(CBaseObject* pExpect=NULL);// ɾ��������OBJECT(pExpect = ����)

	//virtual CBaseObject* RecursiveFindObject(long typeID,long lID);		// �ݹ����
	//virtual CBaseObject* RecursiveFindObject(long typeID,char* strName);// �����ֲ���
	virtual CBaseObject* FindChildObject(long typeID,const CGUID& guid);	// ������object!
	virtual bool FindChildObject(CBaseObject* p);				// ������object!
	CBaseObject* FindChildObjectByName(long typeID, const char* strName);	// �����ֲ���

	CBaseObject* FindAroundObject(CShape* pShape, long type, const CGUID& guid);	// ����pShape��Χ��object
	CBaseObject* FindAroundObject(CShape* pShape, long type);
	CBaseObject* FindNearestAroundObject(CShape* pMoveShape,long type);	//����pShape��Χ��object
	CBaseObject* FindNearestObjectInArea(CShape* pMoveShape,long type,long x,long y,WORD& wDistance);
	void		 FindAroundObject(CShape* pShape, long type, vector<CShape*>& vOut);
	void		 FindAllPlayer(hashsetGUID& setOut)	{setOut=m_setPlayers;return;}
	void		 FindAroundPlayer(CShape* pShape, vector<CPlayer*>& vOut);

	//��һ����Χ����Ѱָ��������������
	void		 FindLimitPlayersAround(CMoveShape* pMoveShape,long lMaxNum,
										hashsetGUID& vOut,const CGUID& guidExceptID);
	//����Χ�ҳ�ָ����������
	void		 FindNearestPlayer(long lX,long lY,long lDis,vector<CGUID>& vOut);
	//void		 FindAroundPets(CShape* pShape, vector<CMonster*>& vOut);

	CMonster*	FindMonsterByID(const CGUID& guid);
	/// �ڳ����ϸ��ݳ���GUID���ҳ���
	CPet* FindPetByID(const CGUID& guid);

	virtual void	AddGuardIndex(long lIndex){}


	void CreateAreaArray();		// ����Area����
	CArea* GetArea(long x, long y);
	CArea* GetArea(long num)	{return &m_pArea[num];}
	long GetAreaX()	{return m_lAreaX;}
	long GetAreaY() {return m_lAreaY;}
	void OnShapeChangeArea(CShape* pShape,long lNextAreaX,long lNextAreaY);	// ����shape�ı�area
	

	CShape* GetShape(long x, long y);				// ��ȡһ���������Ƿ���Shape
	void	GetShape( LONG lX, LONG lY, vector<CShape*>& vOut );
	long GetDropGoodsPos(long sx, long sy, long& dx, long& dy, long lStartPos);
	virtual	void OnSymbolDestroy(const CGUID& SymID,const CGUID& PlayerID,const CGUID& FacID,const CGUID& UnionID){}											//����־��ݻ�ʱ 
	virtual bool OperatorCityGate(long lCityGateID,eOperCityGate eOp){return false;}		// �Գ��Ž��в���
	virtual void UpdateCityGateToClient(long lCityGateID) {return;}
	virtual bool OperatorSymbol(long lSymbolID,eOperCityGate eOp){return false;}			// �Ա�־����в��� 
	virtual void OnWarDeclare(long lWarNum)		{m_CityState=CIS_DUTH;m_lWarNum=lWarNum;}	//��ʼ��ս
	virtual void OnWarStart(long lWarNum)		{m_CityState=CIS_Fight;}					//���ǿ�ʼ
	virtual void OnWarTimeOut(long lWarNum){}												//����ʱ�����
	virtual void OnWarEnd(long lWarNum)			{m_CityState=CIS_NO;m_lWarNum=0;}			//���ǽ���
	virtual void OnWarMass(long lWarNum)		{m_CityState=CIS_Mass;}	
	virtual void OnClearOtherPlayer(long lWarNum){}											//���������Ա
	virtual void OnRefreshRegion(long lWarNum);										//ˢ�³���
	virtual bool SymbolIsAttackAble(CMoveShape* pAttackShape){return true;	}
	virtual bool GuardIsAttackAble(CMoveShape* pAttackShape){return true;	}
	virtual void AddGurdMonster(const CGUID& guid){}
	virtual void DelGurdMonster(const CGUID& guid){}


	void SendMsgToAroundShape(CShape* pMoveShape,CMessage* pMsg);
protected:
	long m_lAreaX;									// �����ϵ�����X
	long m_lAreaY;									// �����ϵ�����Y
	CArea* m_pArea;									// �����ϵ�����

	// �����ˡ���vector���棬���������꣬bugsע��
	//std::map<long, CPlayer*>	m_mapPlayer;		// ����������ڵ����

//////////////////////////////////////////////////////////////////////////
//	SHAPEλ�ñ仯
//////////////////////////////////////////////////////////////////////////
protected:
	list<CShape*>	m_listRemoveShape;			// ��Ҫ�ӳ����Ƴ���Shape�б�
	list<CShape*>	m_listDeleteShape;			// ��Ҫɾ����shape
	list<CShape*>	m_listChangeAreaShape;		// ��Ҫ�ı�area��Shape�б�
	list<CShape*>	m_listChangeRegionShape;	// ��Ҫ�ı�region��Shape�б�
public:
	list<CShape*>* GetRemoveShapeList()			{return &m_listRemoveShape;}
	list<CShape*>* GetDeleteShapeList()			{return &m_listDeleteShape;}
	list<CShape*>* GetChangeAreaShapeList()		{return &m_listChangeAreaShape;}
	list<CShape*>* GetChangeRegionShapeList()	{return &m_listChangeRegionShape;}

//////////////////////////////////////////////////////////////////////////
//	����Ч��
//////////////////////////////////////////////////////////////////////////
public:
	struct tagWeatherTime
	{
		struct tagOption
		{
			struct tagWeather
			{
				long	lWeatherIndex;				// ����Ч�����
				DWORD	dwFogColor;					// �����ɫ

				tagWeather()
				{
					lWeatherIndex = 0;
					dwFogColor = 0;
				}
			};

			long	lOdds;							// ����(% 0-100)
			vector<tagWeather>		vectorWeather;	// ����Ч��
		};

		long	lTime;								// ����ʱ�䣨��λ���֣�
		vector<tagOption>		vectorOption;		// ��ѡ����
	};

	BOOL LoadWeatherSetup();
	void SendWeatherInfo(CPlayer *pPlayer=NULL);	// �����ָ��������ң���ô���͸�������������
	void ChangeWeather( long lId );					// �ı�����
public:
	void RegisterWeatherTimer();
	void UnRegisterWeatherTimer();
	void OnChangeWeather();
private:
	//����������ʱ��
	long m_lWeatherTimerID;

protected:
	vector<tagWeatherTime>	m_vectorWeatherSetup;	// ��������
	long m_lCurWeatherTimeSegment;					// ��ǰʱ��(0-m_vectorWeatherSetup.size())
	long m_lCurWeatherCount;						// ����

	vector<tagWeatherTime::tagOption::tagWeather>	m_vectorCurWeather;	// ��ǰ����

public:
	//##��Ʒ�������
	void SetGoodsProtection( CPlayer* pPlayer, CGoods* pGoods ,BOOL bTeamProtected=TRUE);
	eGoodsPickValue CanPickUpGoods( CPlayer* pPlayer, CGoods* pGoods );

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
public:
	void RefeashBlock();		// ˢ�µ�ͼ�赲

///˰��ϵͳ
private:
	tagRegionParam		m_Param;				//˰����ز���
	long				m_lWarNum;				//ս���ı�ʾ
	eCityState			m_CityState;			//״̬

	
protected:
	bool	m_bKickOutPlayer;		//�Ƿ������������ϵ����
	long	m_lKickOutPlayerTime;	//����ó����ϵ���ҵ���ʱ
	DWORD	m_dwLastTime;			//��һ�μ�ʱʱ��

	void	ClearPlayerAI();
public:
	void	StartClearPlayerOut(long lCountDownTime);
	virtual void SetEnterPosXY(CPlayer* pPlayer){}

	eRST GetSpaceType(){return m_eSpaceType;}
	void SetSpaceType(eRST eType){m_eSpaceType=eType;}

	eRWT GetWarType(){return m_WarRegionType;}
	void SetWarType(eRWT eType){m_WarRegionType=eType;}

	bool IsRide()	{return m_Ride;}
	void SetRide(bool b) {m_Ride=b;}

	bool GetRelive()				{return m_bRelive;}
	bool GetDeadReturn()			{return m_bDeadReturn;}

	



	bool IsChangeEquip()	{return m_ChangeEquip;}
	void SetChangeEquip(bool b) {m_ChangeEquip;}

	void SetRegionParam(tagRegionParam param)	{m_Param=param;}
	tagRegionParam* GetRegionParam(){return &m_Param;}

	virtual void SetOwnedCityOrg(const CGUID& FactionGuid,const CGUID& UnionGuid)
	{m_Param.OwnedFactionID = FactionGuid;m_Param.OwnedUnionID = UnionGuid;}
	virtual const CGUID& GetOwnedCityFaction()	{return m_Param.OwnedFactionID;}
	virtual const CGUID& GetOwnedCityUnion()	{return m_Param.OwnedUnionID;}

	virtual long GetTaxRate(){return m_Param.lCurrentTaxRate;	}			//�õ�˰��
	virtual long GetMaxTaxRate(){return m_Param.lMaxTaxRate;	}			//�õ����˰��
	virtual void SetTaxRateAndUpdate(long lTaxRate);
	virtual void SetTaxRate(long lTaxRate)	{m_Param.lCurrentTaxRate = lTaxRate;}
	virtual void AddTaxMoney(long lMoney);//��˰Ǯ
	virtual void CollectTodayTax();

	long GetBusinessTaxRate(){return m_Param.lBusinessTaxRate;}//����˰��
	void SetBusinessTaxRate(long lRate){m_Param.lBusinessTaxRate=lRate;}
	long GetYuanbaoTaxRate(){return m_Param.lYuanbaoTaxRate;}//Ԫ��˰��
	void SetYuanbaoTaxRate(long lRate){m_Param.lYuanbaoTaxRate=lRate;}
	DWORD GetNormalTotalTax(){return m_Param.dwTotalTax;}
	DWORD GetBusinessTotalTax(){return m_Param.dwBusinessTotalTax;}
	DWORD GetTotalTax(){return m_Param.dwTotalTax+m_Param.dwBusinessTotalTax;}
	void AddBusinessTaxMoney(long lMoney);//������˰��

	DWORD GetTotalTaxPayment()			{return m_Param.dwTotalTax;	}
	void  SetTotalTaxPayment(DWORD lMoney)	{m_Param.dwTotalTax=lMoney;	}

	DWORD GetTodayTaxPayment()			{return m_Param.dwTodayTotalTax;	}
	void  SetTodayTaxPayment(DWORD lMoney)	{m_Param.dwTodayTotalTax=lMoney;	}

	virtual void ObtainTaxPayment(CPlayer* pPlayer);									//��ȡ˰��
	virtual void AdjustTaxRate(CPlayer* pPlayer);										//����˰��

	void UpdateTaxToWorldServer();

	virtual void SetWarNum(long lNum)	{m_lWarNum = lNum;}			//����ս���ı�ʾ����
	virtual long GetWarNum()			{return m_lWarNum;}

	virtual eCityState	GetCityState()		{return m_CityState;}
	virtual void SetCityState(eCityState eState){m_CityState = eState;}
	virtual void ReSetWarState(long lWarID,eCityState eState)	{m_lWarNum = lWarID; m_CityState=eState;} 

	virtual bool IsApplyWarFacsMem(const CGUID& FactionGuid){ return false;}
	virtual void AddContend(CPlayer* pPlayer,long lID,const char* chName,long lTime){}	//���һ������
	virtual bool CancelContendByPlayerID(CPlayer* pPlayer){return false;}				//ȡ��ĳ����ҵ�����
	virtual void OnPlayerDamage(CPlayer* pPlayer,long lDamHp){}							//����ҹ����ܵ��˺�
	virtual void DecContendTime(CPlayer* pPlayer,long lDecTime){}						//����ʱ��
	virtual long GetDiedStateTime()	{return 0;}
	virtual void UpdateContendPlayer(){}												//��������İ������������
	
	void	KickOutAllPlayerToReturnPoint();											//�޳�������ҵ������
	void	KickOutAllPlayerToReturnPointExceptOwner();									//�޳�������ҵ������������ǵ�ӵ����
	void	DelOneGoodFromAllPlayer(char* strGoodName);

	void SetOwnerGUID(const CGUID& guid) { m_OwnerGUID = guid; }
	const CGUID& GetOwnerGUID(void) { return m_OwnerGUID; }

	// ������������
	long GetDupRgnPlayerNumMax(void) { return m_DupRgnPlayerNumMax; };
	void SetDupRgnPlayerNumMax(long value) { m_DupRgnPlayerNumMax = value; };
	// ��ǰ��������
	//long GetCurDupRgnPlayerNum(void) { return m_CurDupRgnPlayerNum; };
	//void SetCurDupRgnPlayerNum(long value) { m_CurDupRgnPlayerNum = value; };

	// ����Ȩ���ţ���ֵ��1��0��
	bool GetOperFlag(void) { return b_OperFlag; }
	void SetOperFlag(bool flag) { b_OperFlag = flag; }
	// ɾ��ʱ��
	long GetDelTime(void) { return m_DelTime; }
	void SetDelTime(long value) { m_DelTime = value; }
	// ά�ַ���
	long GetCostValue(void) { return m_CostValue; }
	void SetCostValue(long value) { m_CostValue = value; }

	// ���ö�ʱ��
	void SetRgnDelTimerID(long id) { m_lRgnDelTimerID = id; }
	// ȡ�ö�ʱ��
	long GetRgnDelTimerID(void) { return m_lRgnDelTimerID; }

	//!	ע��һ��ɾ��������ʱ���������ض�ʱ��ID
	long RegisterDeleteRgnTimerID(long lFutureTime);
	//!	ȡ��һ��ɾ��������Timer
	void CancelDeleteRgnTimer(void);
	//!	��Ӧɾ��������ʱ���������¼�
	void OnDeleteRgnTimeout(long lTimerID);
	//! ��ɾ��
	void OnDeleteObject(CShape* pShape);
	//! ���Ƴ�
	void OnRemoveObject(CShape*pShape,LONG lTime);
	//! ���Ƴ�һ��ʱ��
	void OnRemoveSomeTime(CShape* pShape,LONG lTime);
	// �Ƿ�ǿ��ɾ��
	bool GetForceDelFlag(void)		{ return b_IsForceDelFlag; }
	void SetForceDelFlag(bool flag) { b_IsForceDelFlag = flag; }

	// �����ϴα���ʱ��
	void SetLastWarningTime(DWORD lTime) { m_dwLastWarningTime = lTime; }
	DWORD GetLastWarningTime(void) { return m_dwLastWarningTime; }
private:

	DWORD		m_dwLastWarningTime;		//�ϴξ���ʱ��

	//! ��Ӧ����Object��ʱ�������¼�
	void OnRebornObject(LONG lTimerID,stDestObj* pDestObj);
public:
	void RunScriptAllPlayer(char* strPath);

private:
	CGUID m_OwnerGUID;
	// ������������
	long m_DupRgnPlayerNumMax;
	// ��ǰ��������
	//long m_CurDupRgnPlayerNum;
	// ����Ȩ���ţ���ֵ��1��0��
	bool b_OperFlag;
	// ɾ��ʱ��
	long m_DelTime;
	// ά�ַ���
	long m_CostValue;
	// ����ɾ����ʱ��ID
	long m_lRgnDelTimerID;
	// �Ƿ�ǿ��ɾ��
	bool b_IsForceDelFlag;
	
public:
	void WakeUp();
	void Hibernate(){m_bIsHibernate=TRUE;}
	BOOL IsHibernate(){return m_bIsHibernate;}
private:
	BOOL m_bIsHibernate;

private:
	//ʵ����ͻ�����Ӷ���Ļ������
	
	//���Shape���󵽿ͻ��˵Ľṹ
	struct tagAddShapeToClient
	{
		//����ߵ�����
		long lType;
		//����ʱ��
		DWORD dwSendTime;
		//����ӵĿͻ�������б�
		hash_set<CGUID,hash_guid_compare>	Players;
	};

	typedef hash_set<CGUID,hash_guid_compare>	SendPlayers;
	typedef hash_set<CGUID,hash_guid_compare>::iterator itSendPlayer;

	typedef hash_map<CGUID,tagAddShapeToClient,hash_guid_compare>		AddShapeList;
	typedef hash_map<CGUID,tagAddShapeToClient,hash_guid_compare>::iterator itAddShape;
	//���Shape�Ķ����б�
	AddShapeList	m_AddShapes;
	long m_lAddShapeTimerID;
public:
	void RegisterAddShapeTimer();
	void AddShapeToAround(CShape* pShape);
	void AddShapeToArea(CArea* pArea,CShape* pShape);
	void AddShapeToPlayer(CShape* pShape,const CGUID& PlayerID);
	void DelShapeToAround(CShape* pShape);
	void DelShapeToArea(CArea* pArea,CShape* pShape);
	void DelShapeToPlayer(CShape* pShape,const CGUID& PlayerID); 
	void SendAddShapeMsgToClient();
	void SendAddShapeMsgToClient(CShape* pShape,hashsetGUID& guidPlayers);
	//��ʱ��������ͻ��������Ϣ
	long OnAddShapeTimeOut(DWORD timerid);
	//�õ�����ͼ����е���Ҿ���С�ڵ���5��ļ���
	void GetNearestShapes(CShape* pShape,hashsetGUID& InPlayers,hashsetGUID& OutPlayers);
};
