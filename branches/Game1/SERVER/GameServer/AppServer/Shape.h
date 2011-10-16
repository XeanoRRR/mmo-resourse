#pragma once
#include "baseobject.h"

extern long _dir[8][2];

// 8������Ļ���˳��
extern long _slip_order[8][8];

// ���ƶ�ʱ��Ҫ����ǰ�������б�[figure][dir]
extern list<POINT> s_listMoveCheckCell[3][8];

class CArea;

#define RET_AI_CONTINUE		1		// ����ִ��AI
#define RET_AI_BREAK		0		// �ж�ִ��AI

#define SPEED_LEVEL_COUNT	9		//�ٶȼ���
#define  PI	3.141592654f


class CShape :
	public CBaseObject
{
public:
	static void InitMoveCheckCellList();

	CShape(void);
	virtual ~CShape(void);

	// ״̬
	enum eSTATE
	{
		STATE_PEACE,			// ��ƽ״̬
       	STATE_FIGHT,			// ս��״̬
		STATE_DIED,				// ����״̬
		STATE_FEIGNDEATH,		// ����״̬
		STATE_HANGUP,			// ����
		STATE_DELET,			// ɾ��
	};

	// ����
	enum eACTION
	{
		ACT_NO = -1,			// û���κζ���			
		ACT_STAND,				// վ(������ֹ)
		ACT_IDLE,				// վ��idle
		ACT_WALK,				// ��
		ACT_RUN,				// ��
		ACT_BEATTACK,			// ������
		ACT_INTONATE,			// ����
		ACT_BLOCK,				// ��
		ACT_PARRY,				// �м�
		ACT_MISS,				// ����
		ACT_MEGRIM,				// ѣ��
		ACT_DIED,				// ����(��������)
		ACT_PICK,				// ʰȡ
		ACT_ALREADYDIED,		// �Ѿ�����		
		ACT_OPEN,				// ���Ŵ�
		ACT_ALREADYOPEN,		// �����Ѿ���

		ACT_PERFORM = 50,		// ʩ��
		ACT_MINE    = 700,		// �ɿ�
		ACT_MINEMAX = 721,		// �ɿ����ֵ
		ACT_ATTACK = 950,		// ����
		ACT_MAX_PERFORM = 999,	// ���ʩ������
		ACT_MAX = 1000			// �����ٸ�����
	};

	// ����
	enum eDIR
	{
		DIR_UP = 0,
		DIR_RIGHTUP,
		DIR_RIGHT,
		DIR_RIGHTDOWN,
		DIR_DOWN,
		DIR_LEFTDOWN,
		DIR_LEFT,
		DIR_LEFTUP,
	};

	//����������ٶȷּ��ṹ
	struct tagSpeedLvl
	{
		//�仯����
		float	fRatio;
		//�����仯����
		float	fActionRaio;
		//�ü�����ٶ�ֵ
		float	fSpeed;
	};
///////////////////////////////////////////////////////////////
// ����
///////////////////////////////////////////////////////////////
    
protected:
	eRgnType m_CurRgnType;// ���ڵĳ���������:����/��ͨ����
	long m_lRegionID;	// ���ڵĳ���
	CGUID m_RegionGuid; //���ڳ�����GUID
	float m_fPosX;		// λ��X
	float m_fPosY;		// λ��Y	
	float m_fDir;		//����
	long m_lPos;		// ��ǰ��
	WORD m_wState;		// ״̬
	WORD m_wAction;		// ����

	//ʵ���ƶ�ʵ��ķּ�����
	//ʵ��ԭ���ƶ�ʵ�干��9���ٶȼ�,�ֱ��ǻ����ٶȵ�2/6,3/6,4/6,5/6,1,1.5,2,2.5,3
	//����	��Χ	�ٶ�ֵ
	//1��	-2/6	2/6
	//2��	2/6-3/6	3/6
	//3��	3/6-4/6	4/6
	//4��	4/6-5/6	5/6
	//5��	5/6-1.5	1
	//6��	1-1.5	1.5
	//7��	1.5-2	2
	//8��	2-2.5	2.5
	//9��	3-		3
	//���������ٶ�
	float	m_fSpeed;
	//�ı�������ٶ�
	float	m_fChangeSpeed;
	//�ּ������ٶ�ֵ
	tagSpeedLvl	m_SpeedLvlValue[SPEED_LEVEL_COUNT];
	//��ǰ�������ٶȼ���
	short	m_sCurSpeedLvl;
	//�ٶȵȼ��Ƿ����仯
	bool	m_bSpeedLvlChanged;
public:
	//��Ӧ�����ٶȣ����ø��ȼ���Ӧ���ٶȷ�Χֵ
	void InitSpeedLvlValue();
	//�����������ٶȡ��仯�ٶȷ����ı��ʱ�������ٶȼ�ֵ
	void SetSpeedLvl();
	//�ı��ٶ�ֵ
	void ChangeSpeed(short nSpeed);
	void SetChangeSpeed(short nSpeed);
	//���ٶȷ����ı��Ժ󴥷�
	virtual void OnSpeedChanged(){return;}
	
	void SetCurRgnType(eRgnType type) { m_CurRgnType = type; }
	eRgnType GetCurRgnType(void)	{ return m_CurRgnType; }

	virtual long GetRegionID()		{ return m_lRegionID; }
	virtual void SetRegionID(long l){ m_lRegionID = l; }
	virtual const CGUID& GetRegionGUID()				{ return m_RegionGuid; }
	virtual void SetRegionGUID(const CGUID& guid)		{ m_RegionGuid = guid; }

	virtual long GetDir()			{ return RadianDirTo8Dir(m_fDir);}
	virtual void SetDir(long dir)	{ if( dir>= DIR_UP && dir<= DIR_LEFTUP){m_fDir = PI/4*dir-PI/2;}}

	virtual	float GetDirEx()		{return m_fDir;}
	virtual void  SetDirEx(float fDir) {m_fDir = fDir;}
	
	virtual long GetPos()			{ return m_lPos; }
	virtual void SetPos(long l)		{ m_lPos = l; }
	virtual float GetSpeed();
	float GetSpeedChangedRatio();
	virtual void SetSpeed(float l);
	virtual WORD GetState()			{ return m_wState; }
	virtual void SetState(WORD l)	{ m_wState = l; }
	virtual WORD GetAction()		{ return m_wAction; }
	virtual void SetAction(WORD l)	{ m_wAction = l; }
	CShape::eDIR CShape::RadianDirTo8Dir(float fRadianDir);
	long GetTileX() {return static_cast<long>(m_fPosX);}
	long GetTileY() {return static_cast<long>(m_fPosY);}
	float GetPosX()	{return m_fPosX;}
	float GetPosY()	{return m_fPosY;}
	virtual void SetPosXY(float fX,float fY) {m_fPosX=fX;m_fPosY=fY;}

	virtual WORD GetFigure(eDIR dir=DIR_UP)	{return 0;}	// ����
	virtual void SetBlock(int x, int y, unsigned short block);

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	bool CodeToDataBlock_ForClient(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);		// ����
	virtual bool IsVisible(OBJECT_TYPE eType, const CGUID& rID) { return true; }	// �Ƿ�ɱ�GUIDΪrID��MoveShape����

///////////////////////////////////////////////////////////////
// ��������
///////////////////////////////////////////////////////////////
public:
	void GetFacePos(long& x, long& y);			// ��ȡ��Ե�����
	void GetBackPos(long& x, long& y);			//��ȡ���������
	CShape* GetFaceShape();							//��ȡ��Ե�Ŀ��
	void GetFacePosForBigShape(long& x,long& y);	// ��ȡMONSTER��Ե�����
	void GetDirPos(long dir, long& x, long& y);	// ��ȡdir���������
	long GetRearDir();							// ��ȡ��Ե�����
	long GetLeftDir();							// ��ȡ��ߵ�����
	long GetRightDir();							// ��ȡ�ұߵ�����
	long Distance(CShape* pShape);				// ������Shape��ľ��루��λ�����ӣ�
	long Distance(long x,long y);
	long Distance(long sx,long sy,long dx,long dy);
	FLOAT RealDistance( FLOAT fX, FLOAT fY );
	LONG  RealDistance( LONG lX, LONG lY );
	LONG  RealDistance( CShape* pTarget );
	bool IsInAround(CShape* pShape);			// ������SHAPE�Ƿ��ڸ���(9��Area֮��)

///////////////////////////////////////////////////////////////
//	�ڷ�������������ָ��
///////////////////////////////////////////////////////////////
public:
	enum eChangeState
	{
		CS_DELETE = 1,			// �Ƿ���һ��ִ��ɾ���Ķ���
		CS_REMOVE,				// �Ƿ���һ��ִ���Ƴ��Ķ���
		CS_CHANGEAREA,			// �Ƿ�ı�������ڵ�AREA
		CS_CHANGEREGION,		// �Ƿ�ı䳡��
		CS_QUIET,				// �Ƿ�Ϊ�뿪״̬
	};

protected:
	CArea* m_pArea;				// ���ڵ�Areaָ��
	CArea* m_pNextArea;			// ��Ҫ�����Area
	long m_lNextAreaX;			// ��Ҫ���������X
	long m_lNextAreaY;			// ��Ҫ���������Y

	CGUID m_NextRgnGUID;        // ��Ҫ����ĳ���GUID
	long m_lNextRegionID;		// ��Ҫ����ĳ���ID
	long m_lNextTileX;			// ��Ҫ����ĳ���X
	long m_lNextTileY;			// ��Ҫ����ĳ���Y
	long m_lNextDir;			// ��Ҫ����ĳ���Dir
	long m_lChangeState;		// ����״̬

public:
	CArea* GetArea()				{return m_pArea;}
	virtual void SetArea(CArea* p)	{m_pArea=p;}
	void SetNextAreaX(long l)		{m_lNextAreaX = l;	}
	void SetNextAreaY(long l)		{m_lNextAreaY = l;	}
	long GetNextAreaX()				{return m_lNextAreaX;}
	long GetNextAreaY()				{return m_lNextAreaY;}
	CArea* GetNextArea()			{return m_pNextArea;}
	void SetNextArea(CArea* p)		{m_pNextArea=p;}
	void SetNextRegionID(long l)	{m_lNextRegionID=l;}
	long GetNextRegionID()			{return m_lNextRegionID;}
	void SetNextRegionGUID(const CGUID& l)	{m_NextRgnGUID=l;}
	const CGUID& GetNextRegionGUID()			{return m_NextRgnGUID;}
	void SetNextTileX(long l)		{m_lNextTileX=l;}
	long GetNextTileX()				{return m_lNextTileX;}
	void SetNextTileY(long l)		{m_lNextTileY=l;}
	long GetNextTileY()				{return m_lNextTileY;}
	void SetNextDir(long l)		{m_lNextDir=l;}
	long GetNextDir()			{return m_lNextDir;}
	void SetChangeState( long l)	{ m_lChangeState = l;	}
	long GetChangeState()			{ return m_lChangeState; }
};
