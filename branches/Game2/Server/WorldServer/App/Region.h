#pragma once
#include "baseobject.h"

const static char *g_strRegionHead = "CLS-RGN";	// �ļ�ͷ
const static long g_lRegionVersion = 1;			// �汾��

class CRegion :
	public CBaseObject
{
public:
	CRegion(void);
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
	};

	//��ȫ������
	enum eSecurity
	{
		SECURITY_FREE = 0,		//�ǰ�ȫ��
		SECURTIY_FIGHT,			//������
		SECURTIY_SAFE,			//��ȫ��
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
	long GetSvrResourceID()			{return m_lSvrResourceID; }
	void SetSvrResourceID( long l ) {m_lSvrResourceID = l; }
	float GetExpScale()				{return m_fExpScale;}
	void SetExpScale(float f)		{m_fExpScale = f;}
	float GetOccuExpScale()			{return m_fOccuExpScale;}
	void SetOccuExpScale(float f)	{m_fOccuExpScale = f;}
	float GetSpScale()				{return m_fSpScale;}
	void SetSpScale(float f)		{m_fSpScale = f;}
	float GetMeriScale()			{return m_fMeriScale;}
	void  SetMeriScale(float f)		{m_fMeriScale = f;}
	long GetWidth()					{return m_lWidth;}
	void SetWidth(long l)			{m_lWidth = l;}
	long GetHeight()				{return m_lHeight;}
	void SetHeight(long l)			{m_lHeight = l;}
	BYTE GetCountry()				{ return m_btCountry; }
	void SetCountry(BYTE c)			{ m_btCountry = c; }

	long GetNotify()				{ return m_lNotify; }
	void SetNotify(long l)			{ m_lNotify = l; }

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

	// �赲����
	void SetBlock(int x, int y, BYTE b)	{if(x>-1&&x<GetWidth()&&y>-1&&y<GetHeight()){m_pCell[GetWidth()*y+x].bBlock = b;}}
	BYTE GetBlock(int x, int y)			{return (x>-1&&x<GetWidth()&&y>-1&&y<GetHeight())?m_pCell[GetWidth()*y+x].bBlock:BLOCK_UNFLY;}

	// �л���
	long AddSwitch(tagSwitch* pSwitch);
	void DelSwitch(long lPos);
	tagSwitch* GetSwitch(long l);
	tagSwitch* GetSwitch(long x, long y);
	long HasSwitch(tagSwitch* pSwitch);	// �Ƿ����ָ���ĵ��л���

	// set rgn type
	void SetRgnType(long type) { m_lRgnType = type; }
	long GetRgnType(void)      { return m_lRgnType; }

	void SetLinedIdFlag(long id) { m_lLinedIdFlag = id; }
	long GetLinedIdFlag(void)      { return m_lLinedIdFlag; }

	void SetACRgnType( long type ) { m_lACRgnType = type; }
	long GetACRgnType() const { return m_lACRgnType; }

	vector<tagSwitch>& GetSwitchVec(void) { return m_vectorSwitch; }
	tagCell* GetCellPtr(void) { return m_pCell; }

	BYTE GetRgnHideFlag(void) { return m_byRgnHideFlag; }
	void SetRgnHideFlag(BYTE flag) { m_byRgnHideFlag = flag; }

    //���
    long GetRgnRejoinFlag(void)  {return m_lRejoinTeam; }
    void SetRgnRejoinFlag(long flag) { m_lRejoinTeam = flag;}
protected:
	string m_strFileName;				// �ļ���

	long m_lRgnType;                    // ��ͼ���� ����/��ͨ ����
	long m_lRegionType;					// ��ͼ����
	long m_lResourceID;					// ������ԴID
	long m_lSvrResourceID;				// ��������ԴID
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
    long m_lRejoinTeam;                 // �Ƿ���й�Ӳ���

	tagCell*		m_pCell;			// ��������
	vector<tagSwitch>	m_vectorSwitch;	// �л����б�

	BYTE	m_byRgnHideFlag;				// ��������־ 0��Ĭ�ϲ���������� 1������ 2����������
//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	virtual	void Run()		{};
	virtual	void AI()		{};

	virtual BOOL Load();	// ��ȡ
	virtual BOOL Save();	// �洢
	virtual BOOL New();		// �½�

/////////////////////////////////////////////////////////////////////
// ��ȡ�ӿ�
/////////////////////////////////////////////////////////////////////
public:
	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
public:	
	bool GetRandomPos( long &lX , long &lY );		// ��ȡһ�����赲�������
	bool GetRandomPosInRange( long &lX , long &lY, long sx, long sy, long w, long h );	// ��һ����Χ�ڻ�ȡһ����������赲�ĵ�

	void SetID(long lRegionID){m_lID = lRegionID;};
	long GetID(void){return m_lID;};

	//! ��IDΪ�ֶ����õ�ID
	long m_lID;
};
