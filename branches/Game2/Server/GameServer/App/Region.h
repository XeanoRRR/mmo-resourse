#pragma once
#include "baseobject.h"

//// ���������е�shape, ������ò��󣬿���ȥ����
//#define BEGIN_LOOP_SHAPE(pRegion)	\
//	itBaseObject it;\
//	long num = (pRegion)->GetAreaX() * (pRegion)->GetAreaY();\
//	for(int i=0; i<num; i++)\
//	{\
//		if( (pRegion)->GetArea(i)->GetObjectList()->size() )\
//		{\
//			list<CBaseObject*> listBack = *(pRegion)->GetArea(i)->GetObjectList();\
//			for(it=listBack.begin(); it!=listBack.end(); it++)\
//			{\
//				CShape* pShape = dynamic_cast<CShape*>(*it);\
//				if( pShape )\
//				{\
//
//#define END_LOOP_SHAPE	}}}}


// ����Area�е�shape�� ���Ҳ���Է������ô�����
//#define BEGIN_LOOP_AREA_SHAPE(pArea)	\
//	itBaseObject it;\
//	for(it=(pArea)->GetObjectList()->begin(); it!=(pArea)->GetObjectList()->end(); it++)\
//	{\
//		CShape* pShape = dynamic_cast<CShape*>(*it);\
//		if( pShape )\
//		{\
//
//#define END_LOOP_AREA_SHAPE	}}


static int _area[9][2] = 
{
	0,0,
	-1,-1,	0,-1,	1,-1,
	-1,0,			1,0,
	-1,1,	0,1,	1,1
};

// ���������Χ�е�shape, �������ıȽ��ף��������
#define BEGIN_LOOP_AROUND_SHAPE(pRegion, x, y)	\
	for(int i=0; i<9; i++)\
	{\
		CArea* pArea = (pRegion)->GetArea((x)+_area[i][0], (y)+_area[i][1]);\
		if( pArea )\
		{\
			vector<CShape*> vShapes;\
			pArea -> GetAllShapes( vShapes );\
			for( size_t j = 0; j < vShapes.size(); j ++ )\
			{\
				CShape* pShape = vShapes[j];\
				if( pShape )\
				{\

#define END_LOOP_AROUND_SHAPE	}}}}

// ���������Χ�ض����͵�shape
#define BEGIN_LOOP_AROUND_SHAPE_BY_TYPE(pRegion, x, y, type)	\
	for(int i=0; i<9; i++)\
	{\
		CArea* pArea = (pRegion)->GetArea((x)+_area[i][0], (y)+_area[i][1]);\
		if( pArea )\
		{\
			vector<CShape*> vShapes;\
			pArea -> FindShapes( type, vShapes );\
			for( size_t j = 0; j < vShapes.size(); j ++ )\
			{\
				CShape* pShape = vShapes[j];\
				if( pShape )\
				{\

#define END_LOOP_AROUND_SHAPE_BY_TYPE	}}}}


// ������Χ�����
#define BEGIN_LOOP_AROUND_PLAYER( pRegion, pMoveShape )	\
	for(int i=0; i<9; i++)\
	{\
		CArea* pArea = (pRegion)->GetArea( ( pMoveShape -> GetArea() -> GetX() )+_area[i][0], ( pMoveShape -> GetArea() -> GetY() )+_area[i][1]);\
		if( pArea )\
		{\
			vector<CShape*> vShapes;\
			pArea -> FindShapes( TYPE_PLAYER, vShapes );\
			for( size_t j = 0; j < vShapes.size(); j ++ )\
			{\
				CPlayer* pPlayer = dynamic_cast<CPlayer*>( vShapes[j] );\
				if( pPlayer )\
				{\

#define END_LOOP_AROUND_PLAYER	}}}}


// �����������ض����͵�shape
#define BEGIN_LOOP_REGION_SHAPE_BY_TYPE(pRegion, lType)	\
	if( pRegion )\
	{\
		DWORD dwNumAreas = pRegion -> GetAreaX() * pRegion -> GetAreaY();\
		for( DWORD i = 0; i < dwNumAreas; i ++ )\
		{\
			CArea* pArea = pRegion -> GetArea( i );\
			if( pArea )\
			{\
				vector<CShape*> vShapes;\
				pArea -> FindShapes( lType, vShapes );\
				for( size_t j = 0; j < vShapes.size(); j ++ )\
				{\
					CShape* pShape = vShapes[j];\
					if( pShape )\
					{\

#define END_LOOP_REGION_SHAPE_BY_TYPE	}}}}}


const static char *g_strRegionHead = "CLS-RGN";	// �ļ�ͷ
const static long g_lRegionVersion = 1;			// �汾��

class CRegion :
	public CBaseObject
{
public:
	CRegion(void);
public:
	CRegion(const CGUID& guid);
	virtual ~CRegion(void);

//////////////////////////////////////////////////////////////////////////
//	���ݽṹ
//////////////////////////////////////////////////////////////////////////
public:
	// ��ͼ����
	enum eREGION_TYPE
	{
		RT_KILLABLE	= 0x00000001,		// ����PKս��
		RT_FIGHTABLE= 0x00000002,		// �����ƽ����
		RT_NIGHTABLE= 0x00000004,		// ��ҹ����
		RT_RAINABLE	= 0x00000008,		// ��������
		RT_SNOWABLE	= 0x00000010,		// ������ѩ
		RT_DARK		= 0x00000020,		// ����
		RT_RECALL	= 0x00000040,		// �ܷ��ûسǾ�
		RT_RANDOM	= 0x00000080,		// �ܷ��������
		RT_DRUG		= 0x00000100,		// �ܷ��ò�ҩ
	};

	// ��������
	enum eCELL_MATERIAL
	{
		CM_NULL = 0,		// ��
		CM_GRASS,			// �ݵ�
		GM_MUD,				// ���
		GM_SAND,			// ɳ��
		GM_WOOD,			// ľ��
		GM_STONE,			// ʯ��
		GM_WATER,			// ˮ
	};

	// �赲����
	enum eBLOCK
	{
		BLOCK_NO = 0,		// ���赲
		BLOCK_CANFLY,		// �ɷ����赲
		BLOCK_UNFLY,		// ���ɷ����赲
		BLOCK_SHAPE,		// ����
		BLOCK_AIM,			// �����Լ��ɼ���
	};

	//��ȫ������
	enum eSecurity
	{
		SECURITY_FREE = 0,		//�ǰ�ȫ��
		SECURTIY_FIGHT,			//������
		SECURTIY_SAFE,			//��ȫ��

		SECURITY_CITYWAR,		//��ս��
	};


	// �������ӽṹ
	struct tagCell
	{
		unsigned short	bBlock		:3;		// �赲��0��������  1���ɷ���  2�����ɷ��У�
		unsigned short	bSecurity	:3;		// ��ȫ��
		unsigned short	lWar		:2;		// ��ս��
		unsigned short	lMaterial	:8;		// ����
		WORD			lSwitch;			// �л���(��Ŵ�1��ʼ)
	};

	//ռ�ݸø��ӵĶ���
	struct tagCellObj
	{
		//�����ڸø����ϵĶ�������
		long nObjNum;
	};

	// �л�������
	enum eCHANGE_POINT_TYPE
	{
		CPT_NORMAL,		// ֱ���л�
		CPT_RANDOM,		// ����л�
		CPT_SCRIPT,		// �ű�����
	};

	// �л���
	struct tagSwitch
	{
		long			lState;							// �л�״̬
		long			lRegionID;						// ���ڵ�ID��
		long			lCoordX;						// ���ڵ�����
		long			lCoordY;
		long			lDir;							// ���ڵ㷽��
	};

	// ����ҳ�������
	enum ACRgnType
	{
		ACRT_ACTION = 0, //���ͼ
		ACRT_FIELD = 1, // Ұ���ͼ
	};

    // ��������λ��ӱ�־
    enum PhaseJoinType
    {   
        PhaseJoin_NO, 
        PhaseJoin_YES,   // ������   
    };

/////////////////////////////////////////////////////////////////////
// ��ͼ���Խӿ�
/////////////////////////////////////////////////////////////////////
public:

	const char* GetFileName()			{return m_strFileName.c_str();}
	void SetFileName(const char* str)	{m_strFileName = str;}

	long GetRegionType()			{return m_lRegionType;}
	void SetRegionType(long l)		{m_lRegionType=l;}
	long GetResourceID()			{return m_lResourceID;}
	void SetResourceID(long l)		{m_lResourceID = l;}
	float GetExpScale()				{return m_fExpScale;}
	float GetOccuExpScale()			{return m_fOccuExpScale;}
	float GetSpScale()				{return m_fSpScale;}
	float GetMeriScale()			{return m_fMeriScale;}
	void  SetMeriScale(float f)		{m_fMeriScale = f;}
	long GetWidth()					{return m_lWidth;}
	void SetWidth(long l)			{m_lWidth = l;}
	long GetHeight()				{return m_lHeight;}
	void SetHeight(long l)			{m_lHeight = l;}

	bool IsKillAble()		{return (m_lRegionType & RT_KILLABLE)?true:false;}
	bool IsFightAble()		{return (m_lRegionType & RT_FIGHTABLE)?true:false;}
	bool IsNightAble()		{return (m_lRegionType & RT_NIGHTABLE)?true:false;}
	bool IsRainAble()		{return (m_lRegionType & RT_RAINABLE)?true:false;}
	bool IsSnowAble()		{return (m_lRegionType & RT_SNOWABLE)?true:false;}
	bool IsDark()			{return (m_lRegionType & RT_DARK)?true:false;}
	bool IsRecall()			{return (m_lRegionType & RT_RECALL)?true:false;}
	bool IsRandom()			{return (m_lRegionType & RT_RANDOM)?true:false;}
	bool IsDrug()			{return (m_lRegionType & RT_DRUG)?true:false;}

	void SetKillAble(bool b)	{(b)?(m_lRegionType |= RT_KILLABLE):(m_lRegionType &= ~RT_KILLABLE);}
	void SetFightAble(bool b)	{(b)?(m_lRegionType |= RT_FIGHTABLE):(m_lRegionType &= ~RT_FIGHTABLE);}
	void SetNightAble(bool b)	{(b)?(m_lRegionType |= RT_NIGHTABLE):(m_lRegionType &= ~RT_NIGHTABLE);}
	void SetRainAble(bool b)	{(b)?(m_lRegionType |= RT_RAINABLE):(m_lRegionType &= ~RT_RAINABLE);}
	void SetSnowAble(bool b)	{(b)?(m_lRegionType |= RT_SNOWABLE):(m_lRegionType &= ~RT_SNOWABLE);}
	void SetDark(bool b)		{(b)?(m_lRegionType |= RT_DARK):(m_lRegionType &= ~RT_DARK);}
	void SetRecall(bool b)		{(b)?(m_lRegionType |= RT_RECALL):(m_lRegionType &= ~RT_RECALL);}
	void SetRandom(bool b)		{(b)?(m_lRegionType |= RT_RANDOM):(m_lRegionType &= ~RT_RANDOM);}
	void SetDrug(bool b)		{(b)?(m_lRegionType |= RT_DRUG):(m_lRegionType &= ~RT_DRUG);}

	BYTE GetCountry()				{ return m_btCountry; }
	void SetCountry(BYTE c)			{ m_btCountry = c; }
 
	long GetNotify()				{ return m_lNotify; }
	void SetNotify(long l)			{ m_lNotify = l; }

	DWORD GetLastNotifyHurtTime()		{ return m_dwLastNotifyHurtTime; }
	void SetLastNotifyHurtTime(DWORD dw) { m_dwLastNotifyHurtTime = dw; }

	DWORD GetLastNotifyKillTime()		{ return m_dwLastNotifyKillTime; }
	void SetLastNotifyKillTime(DWORD dw) { m_dwLastNotifyKillTime = dw; }

	// ����
	void SetCell(int x, int y, tagCell* p)
	{
		if(x>-1&&x<GetWidth()&&y>-1&&y<GetHeight())
		{m_pCell[GetWidth()*y+x] = *p;}
	}
	tagCell* GetCell(int x, int y)
	{
		return (x>-1&&x<GetWidth()&&y>-1&&y<GetHeight())?&m_pCell[GetWidth()*y+x]:NULL;
	}
	virtual CRegion::eSecurity GetSecurity(int iTileX, int iTileY)
	{
		tagCell* pCell  = GetCell( iTileX, iTileY );
		if(pCell) return (CRegion::eSecurity)pCell->bSecurity;
		return SECURTIY_SAFE;
	}
	tagCell* GetCellArray(void)
	{
		return m_pCell;
	}

	// �赲����
	void SetBlock(int x, int y, BYTE b)	{if(x>-1&&x<GetWidth()&&y>-1&&y<GetHeight()){m_pCell[GetWidth()*y+x].bBlock = b;}}
	BYTE GetBlock(int x, int y)			{return (x>-1&&x<GetWidth()&&y>-1&&y<GetHeight())?m_pCell[GetWidth()*y+x].bBlock:BLOCK_UNFLY;}

	//�õ�ĳ�������ϵ�obj����
	long GetCellObjNum(long x,long y)
	{
		if(x>-1&&x<GetWidth()&&y>-1&&y<GetHeight())
		{
			tagCellObj* pCellObj = &m_pCellObj[GetWidth()*y+x];
			return pCellObj->nObjNum;
		}
		return 0;
	}
	//�Ը�����obj������1
	void IncCellObjNum(long x,long y)
	{
		if(x>-1&&x<GetWidth()&&y>-1&&y<GetHeight())
		{
			tagCellObj* pCellObj = &m_pCellObj[GetWidth()*y+x];
			pCellObj->nObjNum++;
		}
	}
	//�Ը�����obj������1
	void DecCellObjNum(long x,long y)
	{
		if(x>-1&&x<GetWidth()&&y>-1&&y<GetHeight())
		{
			tagCellObj* pCellObj = &m_pCellObj[GetWidth()*y+x];
			if(pCellObj->nObjNum>0)
				pCellObj->nObjNum--;
		}
	}
	// �л���
	long AddSwitch(tagSwitch* pSwitch);
	void DelSwitch(long lPos);
	tagSwitch* GetSwitch(long l);
	tagSwitch* GetSwitch(long x, long y);
	long HasSwitch(tagSwitch* pSwitch);	// �Ƿ����ָ���ĵ��л���
	vector<tagSwitch>& GetSwitchVec(void) { return m_vectorSwitch; }
	// set rgn type
	void SetRgnType(long type) { m_lRgnType = type; }
	long GetRgnType(void)      { return m_lRgnType; }
	void SetLinedIdFlag(long id) { m_lLinedIdFlag = id; }
	long GetLinedIdFlag(void)      { return m_lLinedIdFlag; }
	void SetACRgnType( long type ) { m_lACRgnType = type; }
	long GetACRgnType() const { return m_lACRgnType; }

	BYTE GetRgnHideFlag(void) { return m_byRgnHideFlag; }
	void SetRgnHideFlag(BYTE flag) { m_byRgnHideFlag = flag; }

    long GetRgnRejoinTeam(void) { return m_lRejoinTeam; }
    void SetRgnRejoinTeam(long team) { m_lRejoinTeam = team; }

protected:
	string m_strFileName;				// �ļ���

	long m_lRgnType;                    // ��ͼ���� ����/��ͨ ����
	long m_lRegionType;					// ��ͼ����
	long m_lResourceID;					// ������ԴID
	float m_fExpScale;					// ��þ���ϵ��
	float m_fOccuExpScale;				// ���ְҵ����ϵ��
	float m_fSpScale;					// ��ȡԪ����ϵ��
	float m_fMeriScale;					// ��ȡ��ѫ��ϵ��
	long m_lWidth;						// ��������
	long m_lHeight;						// ��������
	BYTE m_btCountry;					// ��������
	long m_lNotify;						// ��ɱ֪ͨ
	long m_lLinedIdFlag;				// ���߳���ID
	long m_lACRgnType;					// ����ҳ�������
    long m_lRejoinTeam;                 // ���¼������

	DWORD m_dwLastNotifyHurtTime;			// ���֪ͨʱ��
	DWORD m_dwLastNotifyKillTime;			// ���֪ͨʱ��

	tagCell*		m_pCell;			// ��������
	vector<tagSwitch>	m_vectorSwitch;	// �л����б�

	tagCellObj* m_pCellObj;				//�����ϵ�Obj����

	BYTE	m_byRgnHideFlag;				// ��������־ 0��Ĭ�ϲ���������� 1������ 2����������
//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	virtual	void Run()		{};

	virtual BOOL Load();	// ��ȡ
	virtual BOOL Save();	// �洢
	virtual BOOL New();		// �½�

	virtual void GetReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);		
/////////////////////////////////////////////////////////////////////
// ��ȡ�ӿ�
/////////////////////////////////////////////////////////////////////
public:
	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
public:	
	bool GetRandomPos( long &lX , long &lY );		// ��ȡһ�����赲�������
	bool GetRandomPosInRange( long &lX , long &lY, long sx, long sy, long w, long h );	// ��һ����Χ�ڻ�ȡһ����������赲�ĵ�

	//�õ�һ���ռ���ĳ�����ϵ�һ�����ӣ��ø����ϵ�MoveShape��������
	void GetLeastShapesCellFromSpace(int nObjCenterX,int nObjCenterY,
		int nObjFigureX,int nObjFigureY,int nMaxDistance,int nDiretion,
		long& lX,long& lY);

	long			GetID(void)				{return m_lID;};
	void			SetID(long lID)			{m_lID = lID;};

	//const CGUID&	GetRegionGUID(void)			{return m_guid;};
	//void			SetRegionGUID(CGUID& guid)	{m_guid = guid;};

	const CGUID&	GetExID(void)			{return m_guid;};
	void			SetExID(const CGUID& guid)	{ m_guid = guid; };

	long			m_lID;
};
