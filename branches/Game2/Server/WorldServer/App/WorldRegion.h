#pragma once
#include "Region.h"
class CPlayer;
class VariableList;

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

enum eCityState
{
	CIS_NO,					
	CIS_DUTH,			//��ս״̬
	CIS_Mass,			//����״̬
	CIS_Fight,			//�óǽ���ս��״̬
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
	void operator=(tagRegionParam& right)
	{
		if(this != &right)
		{
			lID = right.lID;							
			lMaxTaxRate = right.lMaxTaxRate;					
			lCurrentTaxRate = right.lCurrentTaxRate;				
			lYuanbaoTaxRate = right.lYuanbaoTaxRate;				
			lBusinessTaxRate = right.lBusinessTaxRate;				
			dwTotalTax = right.dwTotalTax;					
			dwBusinessTotalTax = right.dwBusinessTotalTax;			
			dwTodayTotalTax = right.dwTodayTotalTax;				
			lSupRegionID = right.lSupRegionID;					
			lTurnInTaxRate = right.lTurnInTaxRate;				
			 OwnedFactionID = right.OwnedFactionID;				
			 OwnedUnionID = right.OwnedUnionID;
		}
	}
};

class CWorldRegion :
	public CRegion
{
public:
	CWorldRegion(void);

	CWorldRegion(int type);
	virtual ~CWorldRegion(void);


	// gsid
	long GetGsid(void) { return m_Gsid; }
	void SetGsid(long value) { m_Gsid = value; }

	void		SetNoPk(BYTE b)			{ m_bNoPk = b; }
	void		SetNoContribute(bool b) { m_bNoContribute = b; }
	BYTE		IsNoPk()				{ m_bNoPk; }
	bool		IsNoContribute()		{ m_bNoContribute; }

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

	};

	BOOL LoadMonsterList();					// ��ȡ��ͼ��Monster�ֲ�
	tagMonster* GetMonster(long lIndex);	// ��ȡָ��index��Monster�ֲ�

	DWORD GetMonsterNum () { return (DWORD)m_mpMonster.size(); }

	map<DWORD, tagMonster>& GetMonsterSetup(void) { return m_mpMonster; }
protected:
	map<DWORD,tagMonster>		m_mpMonster;
///////////////////////////////////////////////////////////
//          ��ͼ  collection ���� 
///////////////////////////////////////////////////////////
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
	map<long,tagCollection> m_mapCollection;
public:
	BOOL LoadCollectionList();					// ��ȡ��ͼ��Collection�ֲ�
	tagCollection* GetCollection(long lIndex);	// ��ȡָ��index��Collection�ֲ�

	long GetColletionNum(void) { return (long)m_mapCollection.size(); }

	map<long,tagCollection>& GetCollectionMap(void) { return m_mapCollection; }
///////////////////////////////////////////////////////////
//          ��ͼ  door ���� 
///////////////////////////////////////////////////////////
public:
	struct tagGate
	{
		long	lIndex;			//���
		long	lxPos;			//x����
		long	lyPos;			//y����
		long	lDir;			//����
		int		nOpenState;		//Ĭ�ϴ�״̬
		string	strOrigName;	//ԭʼ��
		string	strAttack;		//�ܹ����ű�
		string	strOpen;		//�򿪽ű�
		string	strClose;		//�رսű�
		string	strDestory;		//��ٽű�
		string	strRefresh;		//��ˢ�ű�

		void operator=(tagGate& right)
		{
			if(this != &right)
			{
				lIndex = right.lIndex;		
				lxPos = right.lxPos;		
				lyPos = right.lyPos;		
				lDir = right.lDir;		
				nOpenState = right.nOpenState;	
				strOrigName = right.strOrigName;
				strAttack = right.strAttack;	
				strOpen = right.strOpen;	
				strClose = right.strClose;	
				strDestory = right.strDestory;	
				strRefresh = right.strRefresh;
			}
		}
	};
protected:
	map<long,tagGate> m_mapGate;
public:
	BOOL LoadGateList();

	map<long,tagGate>& GetGateMap(void) { return m_mapGate; }

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
	// ȡ�þݵ���Ϣ
	std::vector<tagStrongPoint>& GetStrongPointVec(void) { return m_vecStrongPoint; }
private:
	bool m_bStrongPintFlag;
	std::vector<tagStrongPoint> m_vecStrongPoint;
//////////////////////////////////////////////////////////////////////////
//	��ͼ����
//////////////////////////////////////////////////////////////////////////
public:				//Region Setup
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

		void operator=(tagRegionSetup& right)
		{
			if(this != &right)
			{
				bReLiveIsUsed = right.bReLiveIsUsed;	
				bReLivePointFlag = right.bReLivePointFlag;
				lReLiveRegionID = right.lReLiveRegionID; 
				lReLivePosX1 = right.lReLivePosX1;
				lReLivePosX2 = right.lReLivePosX2;
				lReLivePosY1 = right.lReLivePosY1;
				lReLivePosY2 = right.lReLivePosY2;
				
				bLogoutIsUsed = right.bLogoutIsUsed;	
				lLogoutRegionID = right.lLogoutRegionID;
				lLogoutPosX1 = right.lLogoutPosX1;	 
				lLogoutPosX2 = right.lLogoutPosX2;
				lLogoutPosY1 = right.lLogoutPosY1;
				lLogoutPosY2 = right.lLogoutPosY2;
				
				bRuneStoneIsUsed = right.bRuneStoneIsUsed;	
				lRuneStoneRegionID = right.lRuneStoneRegionID;	
				lRuneStonePosX1 = right.lRuneStonePosX1;	
				lRuneStonePosX2 = right.lRuneStonePosX2;
				lRuneStonePosY1 = right.lRuneStonePosY1;
				lRuneStonePosY2 = right.lRuneStonePosY2;
				bRuneStoneCanRecord = right.bRuneStoneCanRecord;
				
				bDupRgnIsUsed = right.bDupRgnIsUsed;	
				lDupRgnRegionID = right.lDupRgnRegionID;
				lDupRgnPosX1 = right.lDupRgnPosX1;
				lDupRgnPosX2 = right.lDupRgnPosX2;
				lDupRgnPosY1 = right.lDupRgnPosY1;
				lDupRgnPosY2 = right.lDupRgnPosY2;

				bWarningFlag = right.bWarningFlag; // �Ƿ񱨾�
			}
		}
	};

	BOOL LoadSetup();
	tagRegionSetup& GetSetup(void) { return m_stSetup; }

	vector<string>& GetForbidMakeGoodsVec(void) { return m_ForbidMakeGoods; }

protected:
	tagRegionSetup m_stSetup;
	vector<string>	m_ForbidMakeGoods;	//�ڸóǾ��н�ֹʹ�õ���Ʒ
	

//////////////////////////////////////////////////////////////////////////
//	��ͼNPC����
//////////////////////////////////////////////////////////////////////////
public:

///////////////////////////////////////////////////////
//	��ͼ·������
///////////////////////////////////////////////////////
public:
	BOOL	LoadRoads();
	vector<vector<POINT>>& GetRoadList(void) { return m_vecRoadsList; }
protected:
	vector<vector<POINT>> m_vecRoadsList;

public:
	BOOL	LoadRegionDrop();

//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	virtual	void Run()		{CRegion::Run();};
	virtual	void AI()		/*{CRegion::AI();}*/;

	virtual BOOL Load();	// ��ȡ
	virtual BOOL New()		{return CRegion::New();};		// �½�

	virtual void SetEnterPosXY(CPlayer* pPlayer);



/////////////////////////////////////////////////////////////////////
// ��ȡ�ӿ�
/////////////////////////////////////////////////////////////////////
public:
	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool AddToByteArrayForProxy(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	virtual bool DecordRegionParamFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool AddSetupToByteArray(vector<BYTE>* pByteArray, bool bExData = true);// ���Setup��CByteArray

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
	vector<tagWeatherTime>& GetWeatherTimeVec(void)	{ return m_vectorWeatherSetup; }
protected:
	vector<tagWeatherTime>	m_vectorWeatherSetup;	// ��������

	//˰����ع���
private:
	BYTE	m_bNoPk;
	bool	m_bNoContribute;
	//�Ƿ���������
	bool	m_Ride;
	//�Ƿ���Ըı�װ��
	bool	m_ChangeEquip;
	//ԭ�ظ����־��
	bool	m_bRelive;
	//���߻سǱ�־����0��ʾ���������߾Ͳ��سǣ�1��ʾ�������߻س�
    bool	m_bDeadReturn;
	//�����Ŀռ�����
	eRegiogSpaceType    m_eSpaceType;
	eRegionWarType		m_WarRegionType;			//ս����������
	tagRegionParam		m_Param;					//˰����ز���

	
	//friend class CRsRegion;

public:
	void	InitOwnerRelation();					//��ʼ��������ӵ����
	virtual void GetReLiveReturnPoint(CPlayer* pPlayer, long &lRegion, long flag/*����һ�ָ��ʽ*/, long &l, long &t, long &r, long &b, long &d);
	virtual void GetLogoutReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);
	virtual void GetRuneStoneReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);
	virtual void GetDupRgnReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);

	CWorldRegion* GenerateSaveData();

	BOOL LoadTaxParam();		//��ȡ˰����ز���
	void SetRegionParam(tagRegionParam& RegionParam)	{m_Param = RegionParam;}
	tagRegionParam& GetRegionParam()						{return m_Param;}
	
	virtual void SetOwnedCityOrg(const CGUID& FactionGuid,const CGUID& UnionGuid)
	{m_Param.OwnedFactionID = FactionGuid;m_Param.OwnedUnionID = UnionGuid;}
	virtual const CGUID& GetOwnedCityFaction()		{return m_Param.OwnedFactionID;}
	virtual const CGUID& GetOwnedCityUnion()		{return m_Param.OwnedUnionID;}

	void SetParamFromDB(const CGUID& OwnedFactionGuid,const CGUID& OwnedUnionGuid,long lCurTaxRate,long lTotadyTotalTax,
						long lTotalTax);
	void SetParamFromGS(long lCurTaxRate,DWORD dwTotadyTotalTax,DWORD dwTotalTax);

	// ������������
	long GetDupRgnPlayerNumMax(void) { return m_DupRgnPlayerNumMax; };
	void SetDupRgnPlayerNumMax(long value) { m_DupRgnPlayerNumMax = value; };
	// ��ǰ��������
	long GetCurDupRgnPlayerNum(void) { return m_CurDupRgnPlayerNum; };
	void SetCurDupRgnPlayerNum(long value) { m_CurDupRgnPlayerNum = value; };

	// ����Ȩ���ţ���ֵ��1��0��
	bool GetOperFlag(void) { return b_OperFlag; }
	void SetOperFlag(bool flag) { b_OperFlag = flag; }
	// ɾ��ʱ��
	long GetDelTime(void) { return m_DelTime; }
	void SetDelTime(long value) { m_DelTime = value; }
	// ά�ַ���
	long GetCostValue(void) { return m_CostValue; }
	void SetCostValue(long value) { m_CostValue = value; }

	void SetOwnerGUID(const CGUID& guid) { m_OwnerGUID = guid; }
	const CGUID& GetOwnerGUID(void) { return m_OwnerGUID; }

	DWORD GetAreaId(){return m_dwAreaId;}
	VOID SetAreaId(DWORD dwId){m_dwAreaId=dwId;}

	// �Ƿ���ģ�峡��
	bool GetIsTemplateRgnFlag(void) { return m_bIsTemplateRgnFlag; }
	void SetIsTemplateRgnFlag(bool flag) { m_bIsTemplateRgnFlag = flag; }

	eRST GetSpaceType(){return m_eSpaceType;}
	void SetSpaceType(eRST eType){m_eSpaceType=eType;}

	eRWT GetWarType(){return m_WarRegionType;}
	void SetWarType(eRWT eType){m_WarRegionType=eType;}

	bool IsRide()	{return m_Ride;}
	void SetRide(bool b) {m_Ride=b;}

	
	void SetRelive(bool bRelive){m_bRelive = bRelive;}
	bool GetRelive()			{return m_bRelive;}
		
	void SetDeadReturn(bool bDeadReturn){m_bDeadReturn = bDeadReturn;}
	bool GetDeadReturn()				{return m_bDeadReturn;}


	bool IsChangeEquip()	{return m_ChangeEquip;}
	void SetChangeEquip(bool b) {m_ChangeEquip = b;}

	// ���ݵ�ǰ������������
	bool CloneData(CWorldRegion* pRgn);

	// ����ȫ������
	bool CloneAllData(CWorldRegion* pRgn);

private:
	long        m_ID;
	DWORD		m_dwAreaId;//������������ID
	CGUID	m_OwnerGUID;
	// ������������
	long m_DupRgnPlayerNumMax;
	// ��ǰ��������
	long m_CurDupRgnPlayerNum;
	// ����Ȩ���ţ���ֵ��1��0��
	bool b_OperFlag;
	// ɾ��ʱ��
	long m_DelTime;
	// ά�ַ���
	long m_CostValue;

	// gsid
	long m_Gsid;

	// �Ƿ���ģ�峡��
	bool m_bIsTemplateRgnFlag;
};
