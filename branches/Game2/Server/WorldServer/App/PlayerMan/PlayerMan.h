// Fox@20081104----------------------------------------------
// File:    PlayerMan.h
// Brief:   CPlayerMan���ڹ������λ��, �Ա�, �ȼ�, ������
//			��ϵӳ��, Ŀǰ��Ҫ�������������������.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Nov.04, 2008
// Fox@20081104----------------------------------------------

#pragma once

class CPlayer;
class CMessage;

//#define PutoutPMLog(strLog) \
//PutoutLog("PM", LT_ERROR, strLog); \
//AppFrame::AddStdLog(strLog)

// Fox@20081107----------------------------------------------
// �Զ���ṹ
enum PLAYER_ELEM_TYPE					// �����Ϣ����
{
	PET_SEX			= 0x00000001,
	PET_LEVEL		= 0x00000002,
	PET_COUNTRY		= 0x00000004,
	PET_REGION		= 0x00000008,
	PET_AREA		= 0x00000010,
	PET_OCCUPATION	= 0x00000020,
	PEI_CONST		= 0x00000040,
	PET_HIDENAME	= 0x00000080,
	PET_NAME		= 0x10000040,
	PET_FACTION		= 0x10000080,
	PET_UNION		= 0x10000100,
	PET_STR_DESC	= 0x10000000		// �ַ��������ı�־
};

enum PET_SEX_TYPE						// �Ա�����
{
	PST_MALE		= 0x00000000,
	PST_FEMALE		= 0x00000001,
	PST_SIG_FEMALE	= 0x10000001
};

enum PET_REGION_TYPE					// ��������
{
	PRT_NULL		= 0xFFFFFFFF,
	PRT_CUR_REGION	= 0x00000001,
	PRT_CUR_AREA	= 0x00000002
};

struct tagPosition						// λ����Ϣ
{
	tagPosition()
		: lPosX(0)
		, lPosY(0)
	{
	}

	long			lPosX;
	long			lPosY;
};

struct tagPlayerInfo					// �����Ϣ
{
	tagPlayerInfo()
		: ExID(NULL_GUID)
		, strName("")
		, strFaction("")
		, strUnion("")
		, lOccupation(0)
		, lSex(-1)
		, lLevel(0)
		, lCountryID(-1)
		, lConst( -1 )
		, lRegionID(-1)
		, lAreaID(-1)
		, btHideName(0)
	{
	}

	tagPlayerInfo& operator=(tagPlayerInfo& info)
	{
		if( this == &info )
			return *this;

		ExID		= info.ExID;
		strName		= info.strName;
		strFaction	= info.strFaction;
		strUnion	= info.strUnion;
		lOccupation	= info.lOccupation;
		lSex		= info.lSex;
		lLevel		= info.lLevel;
		lCountryID	= info.lCountryID;
		lConst		= info.lConst;
		lRegionID	= info.lRegionID;
		lAreaID		= info.lAreaID;
		btHideName	= info.btHideName;
		Position.lPosX	= info.Position.lPosX;
		Position.lPosY	= info.Position.lPosY;

		return *this;
	}

	CGUID			ExID;		// ���ID
	string			strName;
	string			strFaction;
	string			strUnion;
	long			lOccupation;
	long			lSex;
	long			lLevel;
	long			lCountryID;
	long			lConst;
	long			lRegionID;
	long			lAreaID;
	BYTE			btHideName;
	tagPosition		Position;
};
// Fox@20081107----------------------------------------------

class CPlayerMan
{
public:
	CPlayerMan(void);
	~CPlayerMan(void);

	// Fox@20081120 ��ҹ�������
public:
	void Initial(void);
	void Release(void);
	bool LoadConfig(void);
	bool CodeToDataBlock(DBWriteSet& setWriteDB);		// ����
	bool DecodeFromDataBlock(DBReadSet& setReadDB);		// ����

	// Fox@20081107----------------------------------------------
public:
	// ��ҹ���, ���ⲿ����
	bool AddPlayer(CPlayer* pPlayer);		// ������
	bool DelPlayer(CPlayer* pPlayer);		// �ͷ����
	long RefreshElem(long eType,
		CPlayer* pPlayer);					// �޸���ҵ�����Ϣ
	long RefreshElem(long eType,
		CPlayer* pPlayer,
		long lNewVal);						// �޸���ҵ�����Ϣ
	bool RefreshElems(long eTypes,
		CPlayer* pPlayer);					// �޸���Ҷ�����Ϣ
	bool RefreshPlayer(CPlayer* pPlayer,
		bool bAll=false);					// ˢ�����
	bool ChangeGS(long lNew,
		const CGUID& rGuid);				// GS�л�
	bool Query(CMessage* pMsg);				// �ͻ��˲�ѯ

	// Fox@20081107----------------------------------------------
protected:
	// ��ҹ���, �����ڲ�ʹ��
	tagPlayerInfo* FindPlayer(const CGUID& rGuid);	// �������
	bool FillPlayerInfo(CPlayer* pPlayer,
		tagPlayerInfo& rPlayerInfo);		// ����CPlayer�������PlayerInfo
	bool AddPlayerMap(tagPlayerInfo* pInfo);	// ����CPlayer�������ӳ��
	bool DelPlayerMap(tagPlayerInfo* pInfo,
		bool bComplete = false);			// ����CPlayer��������ӳ��
	tagPlayerInfo* AllocPlayer(void);		// �������
	void FreePlayer(tagPlayerInfo* pInfo);	// �������
	long CalcAreaID(tagPosition& rPos,
		long lRegionID);					// ����AreaID
	long CalcLevel(long lLevel);			// ����ȼ�����
	bool CheckCountry(long lID, long lIDs);	// ���ұȽ�

	// Fox@20081107----------------------------------------------
protected:
	// ��Ҽ���
	typedef std::list<tagPlayerInfo*> SET_PLAYER;
	typedef SET_PLAYER::iterator ITR_SET_PLAYER;
	SET_PLAYER			m_PrePlayers;				// Ԥ������Ҽ���

	typedef std::map<CGUID, tagPlayerInfo*> MAP_PLAYER;
	typedef MAP_PLAYER::iterator ITR_MAP_PLAYER;
	MAP_PLAYER			m_mapPlayers;

	// ��ϵӳ��
	typedef std::list<CGUID> LIST_GUID;
	typedef LIST_GUID::iterator ITR_LIST_GUID;
	typedef std::map<long, LIST_GUID > MAP_PLAYER_MAP;
	typedef MAP_PLAYER_MAP::iterator ITR_MAP_PLAYER_MAP;
	MAP_PLAYER_MAP		m_Region2Players;
	MAP_PLAYER_MAP		m_Area2Players;
	MAP_PLAYER_MAP		m_Sex2Players;
	MAP_PLAYER_MAP		m_Country2Players;
	MAP_PLAYER_MAP		m_Level2Players;
	MAP_PLAYER_MAP		m_Const2Players;

	// ����
	long				m_lMaxPlayerNum;	// ��GS��������
	long				m_lAreaWidth;		// ����߳�
	long				m_lPageLength;		// ÿҳ��ʾ�����
	long				m_lLevelLength;		// �ȼ����
	long				m_lGSNum;			// GS����
};