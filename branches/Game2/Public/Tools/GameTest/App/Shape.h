#pragma once
#pragma warning(disable:4819)

#include <d3d9.h>
#include <d3dx9.h>

#include "baseobject.h"
#include "mmsystem.h"

class CEffect;
class CBitmapX;

static int _dir[8][2] =
{
	0,-1,	1,-1,	1,0,	1,1,
	0,1,	-1,1,	-1,0,	-1,-1
};

//�ڳ�����ʰȡ��Ʒ�ĸ���˳��
static int _Pick_Orher[9][2] = {
	0,0,	//������ڵĸ���
	-1,0,	//��
	1,0,	//��	
	0,-1,	//��
	0,1,	//��
	-1,1,	//����
	-1,-1,	//����
	1,-1,	//����
	1,1		//����
};
// 8������Ļ���˳��
static int _slip_order[8][8] =
{
	0, 7, 1, 6, 2, 5, 3, 4,
	1, 0, 2, 7, 3, 6, 4, 5,
	2, 1, 3, 0, 4, 7, 5, 6,
	3, 2, 4, 1, 5, 0, 6, 7,
	4, 3, 5, 2, 6, 1, 7, 0,
	5, 4, 6, 3, 7, 2, 0, 1,
	6, 5, 7, 4, 0, 3, 1, 2,
	7, 6, 0, 5, 1, 4, 2, 3
};


class CShape :
	public CBaseObject
{
public:
	CShape(void);
	inline virtual ~CShape(void);

	// ״̬
	enum eSTATE
	{
		STATE_PEACE,			// ��ƽ״̬
		STATE_FIGHT,			// ս��״̬
		STATE_DIED,				// ����״̬
		STATE_FEIGNDEATH,		// ����״̬
		STATE_OPEN,				// ���Ŵ�״̬
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


		// ѡ�˽��涯��
		ACT_SEL_X0,				// ����վ��
		ACT_SEL_X1,				// �ν�
		ACT_SEL_X2,				// �ӽ�x
		ACT_SEL_X3,				// �ֽ�վ��x
		ACT_SEL_X4,				// �ս�x
		ACT_SEL_X5,				// �ָ�վ��

		ACT_PERFORM = 50,		// ʩ��
		ACT_MINE    = 751,		// �ɿ�
		ACT_MINEMAX = 771,		// �ɿ����ֵ
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

	// ��λ��
	enum eLOCATOR
	{
		LOCATOR_WIND			= MAKEFOURCC('0','0','0','0'),		// �ؿ�
		LOCATOR_HEAD			= MAKEFOURCC('0','0','0','1'),		// ͷ��
		LOCATOR_FOOTCENTER		= MAKEFOURCC('0','0','0','2'),		// �ŵ�����
		LOCATOR_LEFTHAND		= MAKEFOURCC('0','0','0','3'),		// ����
		LOCATOR_RIGHTHAND		= MAKEFOURCC('0','0','0','4'),		// ����
		LOCATOR_LEFTFOOT		= MAKEFOURCC('0','0','0','5'),		// ���
		LOCATOR_RIGHTFOOT		= MAKEFOURCC('0','0','0','6'),		// �ҽ�
		LOCATOR_BACK			= MAKEFOURCC('0','0','0','7'),		// ��
		LOCATOR_LEFTWAIST		= MAKEFOURCC('0','0','0','8'),		// ����	
		LOCATOR_RIGHTWAIST		= MAKEFOURCC('0','0','0','9'),		// ����	
		LOCATOR_LEFTARM         = MAKEFOURCC('0','0','1','0'),		// ���
		LOCATOR_RIGHTARM        = MAKEFOURCC('0','0','1','1'),		// �ұ�
		LOCATOR_LEFTBACK		= MAKEFOURCC('0','0','1','2'),		// ���
		LOCATOR_RIGHTBACK		= MAKEFOURCC('0','0','1','3'),		// �Һ�
		LOCATOR_BIGSWORD		= MAKEFOURCC('0','0','1','4'),		// �󱳳���
		LOCATOR_BACKBOW			= MAKEFOURCC('0','0','1','5'),		// �󱳹�
		LOCATOR_BACKARROW		= MAKEFOURCC('0','0','1','6'),		// �󱳼���
		LOCATOR_BACKSTAFF       = MAKEFOURCC('0','0','1','7'),		// ����,��
		LOCATOR_LEFTBOXING      = MAKEFOURCC('0','0','1','8'),		// ��ȭ
		LOCATOR_RIGHTBOXING     = MAKEFOURCC('0','0','1','9'),		// ��ȭ
		LOCATOR_LEFTHURTPOINT   = MAKEFOURCC('0','0','2','1'),		// �����˵�
		LOCATOR_RIGHTHURTPOINT  = MAKEFOURCC('0','0','2','2'),		// �����˵�
		LOCATOR_HOLDPOINT		= MAKEFOURCC('0','0','2','3'),		// ���˵�

	};

	// ��������
protected:
	long  m_lCurRgnType;					// ���ڳ���������
	long  m_lRegionID;						// ���ڳ�����id
	CGUID m_RegionGuid;						// ���ڳ�����guid
	float m_fPosX;							// ���ڳ���λ��X
	float m_fPosY;							// ���ڳ���λ��Y
	float m_fDir;							// ���ڳ�������

	//	bool m_bIsSelected;						//�Ƿ�ѡ��
    float m_fSpeed;							// �ٶ�
    WORD m_wState;							// ״̬
	WORD m_wAction;							// ����
	//	DWORD m_dwFMFTime[8];						//��ӡʱ����,���8...��...�ȣ�Ӧ�ù��˰ɣ�

	//	float m_fHeight;						// �߶�

public:
	//	virtual DWORD GetFMFTime(DWORD dwIndex) const { return m_dwFMFTime[dwIndex];}
	//	virtual void SetFMFTime(DWORD dwIndex,DWORD dwTime) { m_dwFMFTime[dwIndex] = dwTime;}

	virtual long GetRegionID(){return m_lRegionID;}
	virtual void SetRegionID(long l){m_lRegionID=l;}

	virtual const CGUID& GetRegionGUID()		{ return m_RegionGuid; }
	virtual void GetRegionGUID(const CGUID& id){ m_RegionGuid = id; }

	virtual void SetPosXY(float x, float y);

	virtual float GetPosX()			{ return m_fPosX; }
	virtual void SetPosX(float l)	{ SetPosXY(l, GetPosY()); /*m_fPosX = l;*/ }

	virtual float GetPosY()			{ return m_fPosY; }
	virtual void SetPosY(float l)	{ SetPosXY(GetPosX(), l); /*m_fPosY = l;*/ }

	virtual eDIR GetDir();
	virtual void SetDir(eDIR dir);
	//�õ�/���� ���ȷ���
	virtual float GetDirEx()	{ return m_fDir;}
	virtual void SetDirEx( float f) { m_fDir = f;}

    virtual float GetSpeed()		{ return m_fSpeed; }
    virtual void SetSpeed(float l)	{ m_fSpeed = l; }

	virtual WORD GetState()			{ return m_wState; }
	virtual void SetState(WORD l)	{ m_wState = l; }

	virtual WORD GetAction()		{return m_wAction;}
	virtual void SetAction(WORD l)	{ m_wAction = l; }
	virtual void OnActionChange(){}

	//	virtual float GetHeight()		{return m_fHeight;}
	//	virtual void SetHeight(float f)	{m_fHeight=f;}

	long GetTileX();
	long GetTileY();

	virtual BYTE GetFigure(eDIR dir=DIR_UP)	{return 0;}	// ����
	virtual void SetBlock(int x, int y, unsigned short block);

	virtual bool AddShapeToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	virtual bool DecordShapeFromByteArray(BYTE* pByte, long& pos);	// ����

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	//----------------------------------------------
	//	�¼ӽ���ӿ�
	//----------------------------------------------
	virtual bool DecordFromDataBlock(DBReadSet& db,bool bEx = true);
public:
	virtual	DWORD	GetValue(const char* strName)	{return 0;};
	virtual	DWORD	SetValue(const char* strName, DWORD dwValue)	{return 0;};
	virtual	DWORD	ChangeValue(const char* strName, long lValue)		{return 0;};

	///////////////////////////////////////////////////////////////
	// ��������
	///////////////////////////////////////////////////////////////
public:
	void GetFacePos(long& x, long& y);				// ��ȡ��Ե�����
	CShape* GetFaceShape();							// ��ȡ��Ե�Ŀ��

	void GetDirPos(long dir, long& x, long& y);		// ��ȡdir���������
	long GetFaceDir();								// ��ȡ��Ե�����

	virtual long Distance(CShape* pShape);			// ������Shape��ľ��루��λ�����ӣ�
	virtual long MinXYDistance(CShape* pShape);		// ��x,y�����ϵ���С���루��λ�����ӣ�

	FLOAT RealDistance( FLOAT fX, FLOAT fY );
	LONG  RealDistance( LONG lX, LONG lY );
	LONG  RealDistance( CShape* pTarget );

	static float GetLineDirF(float sx, float sy, float dx, float dy, float fdir = 0.0f);	// ��ȡ��(sx,sy)��(dx,dy)�ķ���(0-PI)
	static eDIR RadianDirTo8Dir(float fRadianDir);
	//	��ʾ����
public:
	void SetNowDir(float f)					{m_fNowDir = f;}
	float GetNowDir()						{return m_fNowDir;}
	//��ʾ����
protected:
	float	m_fNowDir;			// ��ǰ����ʾ����(0-360)

	//���ڼ���shape��2Dƽ������ֵ
	// by  MartySa 2009.2.5

	//POINT	&GetPickBoxCenterOfScreenCoord(POINT &ptScreen, long lTileHeight, D3DXMATRIX &ViewMatrix, D3DXMATRIX &ProjectionMatrix);
	//�жϵ�ǰshape�Ƿ��ܵ�����������DriveClient 
private:
	bool	m_bIsAtk;
public:
	void    SetShapeIsAtk(bool b)			{ m_bIsAtk = b; }
	bool	GetShapeIsAtk()					{ return m_bIsAtk; }

};