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
		CM_MUD,				// ���
		CM_SAND,			// ɳ��
		CM_WOOD,			// ľ��
		CM_STONE,			// ʯ��
		CM_WATER,			// ˮ
		CM_SNOW,			// ѩ��
		CM_MARISH,			// ����
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

/////////////////////////////////////////////////////////////////////
// ��ͼ���Խӿ�
/////////////////////////////////////////////////////////////////////
public:

	const char* GetFileName()			{return m_strFileName.c_str();}
	void SetFileName(const char* str)	{m_strFileName = str;}

	long GetRegionType()			{return m_lRegionType;}
	void SetRegionType(long l)		{m_lRegionType=l;}
	long GetResourceID()			{return m_lResourceID;}
	void SetResourceID(long l)		{m_lResourceID=l;}
	long GetConfigID()				{return m_lConfigID;}
	void SetConfigID(long l)		{m_lConfigID=l;}
	float GetExpScale()				{return m_fExpScale;}
	void SetExpScale(float f)		{ m_fExpScale = f;}
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

protected:
	string m_strFileName;				// �ļ���

	long m_lRegionType;					// ��ͼ����
	long m_lConfigID;					// �����ļ����õ�ID
	long m_lResourceID;					// ��ԴID
	float m_fExpScale;					// ��õľ��鱶��
	long m_lWidth;						// ��������
	long m_lHeight;						// ��������

	tagCell*		m_pCell;			// ��������
	vector<tagSwitch>	m_vectorSwitch;	// �л����б�

	tagCellObj* m_pCellObj;				//�����ϵ�Obj����

//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	virtual	void Run()		{};
//	virtual	void AI()		{};
	virtual	bool Display()	{ return true;}
	virtual	void ScrollCenter(long x, long y)	{};

	virtual BOOL Load();	// ��ȡ
	virtual BOOL Save();	// �洢
	virtual BOOL New();		// �½�
};
