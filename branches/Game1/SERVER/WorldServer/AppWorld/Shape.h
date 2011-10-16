#pragma once
#include "baseobject.h"

class CShape :
	public CBaseObject
{
public:
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
		ACT_STAND,				// վ(������ֹ)
		ACT_IDLE,				// վ��idle
		ACT_WALK,				// ��
		ACT_RUN,				// ��
		ACT_ATTACK,				// ����
		ACT_BEATTACK,			// ������
		ACT_DIED,				// ����(��������)
		ACT_OPEN,				// ���Ŵ�

		ACT_PERFORM = 21,		// ʩ��
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

///////////////////////////////////////////////////////////////
// ����
///////////////////////////////////////////////////////////////
protected:
	eRgnType m_CurRgnType;// ���ڵĳ���������:����/��ͨ����
	long m_lRegionID;	// ���ڵĳ���
	CGUID m_RegionID;    // ���ڵĳ���GUID
	float m_fPosX;		// λ��X
	float m_fPosY;		// λ��Y
	long m_lDir;		// ����
	long m_lPos;		// ��ǰ��
	float m_fSpeed;		// �ٶ�
	WORD m_wState;		// ״̬
	WORD m_wAction;		// ����

public:
	void SetCurRgnType(eRgnType type) { m_CurRgnType = type; }
	eRgnType GetCurRgnType(void)	{ return m_CurRgnType; }

	virtual long GetRegionID()		{ return m_lRegionID; }
	virtual void SetRegionID(long l);
	virtual const CGUID& GetRegionExID()		{ return m_RegionID; }
	virtual void SetRegionExID(const CGUID& l){ m_RegionID = l; }
	virtual void SetPosX(float l)	{ m_fPosX = l; }
	virtual void SetPosY(float l)	{ m_fPosY = l;}
	virtual long GetDir()			{ return m_lDir; }
	virtual void SetDir(long l)		{ if(l>-1 && l<8) m_lDir = l; }
	virtual long GetPos()			{ return m_lPos; }
	virtual void SetPos(long l)		{ m_lPos = l; }
	virtual float GetSpeed()		{ return m_fSpeed; }
	virtual void SetSpeed(float l)	{ m_fSpeed = l; }
	virtual WORD GetState()			{ return m_wState; }
	virtual void SetState(WORD l)	{ m_wState = l; }
	virtual WORD GetAction()		{ return m_wAction; }
	virtual void SetAction(WORD l)	{ m_wAction = l; }

	float GetPosX();
	float GetPosY();
	virtual void SetPosXY(float x, float y);
	virtual BYTE GetFigure()	{return 0;}	// ����
	void SetBlock(int x, int y, unsigned short block);

	virtual bool AddShapeToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	virtual bool DecordShapeFromByteArray(BYTE* pByte, long& pos);	// ����

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);	
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);	

	virtual void AI(void);

///////////////////////////////////////////////////////////////
// ��������
///////////////////////////////////////////////////////////////
public:
	void GetDirPos(long dir, float& x, float& y);	// ��ȡdir���������
	long GetFaceDir();							// ��ȡ��Ե�����
};
