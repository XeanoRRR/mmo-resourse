
#pragma once

#include "region.h"

class CPlayer;
class CShape;
class CMoveShape;
class CFindPath;
class CGoods;
class CMonster;

enum REGION_TYPE
{
	RT_Normal=0,	//��ͨ����
	RT_Village,		//��ս��ͼ
	RT_City,		//��
};

// �ͻ���ר�õĳ�����
class CClientRegion : public CRegion
{
public:
	CClientRegion(void);
	virtual ~CClientRegion(void);

	// DriveClient, Client to Test Tile Hight
	// by MartySa 2009.2.5
public:
	struct tagTilePoint
	{
		long lTilePosX;
		long lTilePosY;
		tagTilePoint() : lTilePosX(0), lTilePosY(0)
		{}		

		bool operator == (const tagTilePoint &tilepoint) const
		{	
			if ( &tilepoint == this )
			{
				return true; 
			}

			return ( tilepoint.lTilePosX == lTilePosX && tilepoint.lTilePosY == lTilePosY);
		}

		bool operator != (const tagTilePoint &tilepoint) const
		{	
			if ( &tilepoint == this )
			{
				return false; 
			}

			return ( tilepoint.lTilePosX != lTilePosX || tilepoint.lTilePosY != lTilePosY);
		}

		bool operator < (const tagTilePoint &tilepoint) const
		{
			if ( &tilepoint == this )
			{
				return false; 
			}

			if ( tilepoint.lTilePosX != lTilePosX )
			{
				return ( tilepoint.lTilePosX < lTilePosX );	
			}

			return ( tilepoint.lTilePosY < lTilePosY );	
		}
	};
private:
	//��Ϸ�������Ӹ߶ȹ���
	map<tagTilePoint, long>		 m_mapTileHightManager;

public:
	void					  _AddTileHight(tagTilePoint tilepoint, long lHeight);
	map<tagTilePoint, long>	 *_GetTileHightManager() { return m_mapTileHightManager.size()>0 ? &m_mapTileHightManager : NULL; }
	void					  _ClearTileHightManager() { m_mapTileHightManager.clear(); }
	long					  _GetTileHightByTilePoint(tagTilePoint tilepoint);
	//end

private:
	long m_lTaxRate;									//˰��
	REGION_TYPE  m_WarRegionType;						//ս����������
	//////////////////////////////////////////////////////////////////////////
	//	�ӿ�
	//////////////////////////////////////////////////////////////////////////
public:
	virtual BOOL Load();		// ��ȡ

	void	RemoveObject(CBaseObject* pObject);						//�Ƴ�Object

	bool	AddGoods(CGoods* pGoods,DWORD dwPos,LONG lExtendID);
	CGoods*	RemoveGoods(CGUID& guid, DWORD dwPos, LONG lExtendID, DWORD dwAmount);
	bool	ClearGoods();

	//�õ�һ�����ĵ㸽��û��MoveShape�ĸ���
	void	GetNoShapeCell(long lCenterX,long lCenterY,long lRadius,long lFirstDir,long& lX,long& lY);
	//�õ��ռ�ĳ������������moveshape�ĸ���
	void	GetLeastShapesCellFromSpace(int nObjCenterX,int nObjCenterY, int nObjFigureX,int nObjFigureY,int nMaxDistance,int nDiretion, long& lX,long& lY);
	//�õ�һ��Բ�οռ���ĳ������������moveshape�ĸ���
	void	GetLeastShapesCellFromCircularSpace(int nObjCenterX,int nObjCenterY,
		int nObjFigureX,int nObjFigureY,int nMaxDistance,float fDir,
		long& lX,long& lY);

	//---------------------------------------
	//�¼�shape����ӿ�
	//  [12/6/2008 Square]
	CShape*	GetShape(long x, long y);							//ĳ���ϵ�Shape
	CGoods* GetGoods(long x, long y);
	CPlayer* GetPlayer(long x,long y);
	CMonster* GetMonster(long x,long y);
	CGoods* GetRegionGoods();				//�ӳ��������б���ȡһ��Goods,
	CShape* GetShapeByShowName(const char* strShowName);

	//��ȡһ�����ָ����Χ���Ƿ���Shape
	CShape* GetShapeFromSpace(long x, long y, long lSize, OBJECT_TYPE eType, CShape* pShape = NULL);		//��ȡ����Χ�ڵ�Shape

	//�õ�һ��������pPlayer�Ķ���
	CShape* GetAimShape(CPlayer* pPlayer);

	CShape* GetShapeByType(OBJECT_TYPE eType,CPlayer* pSelf);

	//˰��
	void SetTaxRate(long l)		{ m_lTaxRate = l; }
	long GetTaxRate()			{return m_lTaxRate;}
	//��������
	void SetWarRegionType(REGION_TYPE eType) {m_WarRegionType = eType;}
	REGION_TYPE GetWarRegionType()		{ return m_WarRegionType;}

	//	
	CPlayer * FindTradeAnswer(CPlayer* pSelf);
	//////////////////////////////////////////////////////////////////////////
	//	Ѱ·ģ��
	//////////////////////////////////////////////////////////////////////////
public:
	CFindPath* GetFindPath();
private:
	CFindPath *	m_pFindPath;	// Ѱ·ģ��
	//ȡ����
public:
	//////////////////////////////////////////////////////////////////////////
	struct tagNpc{
		char strName[256];
		long	lPosX;
		long	lPosY;
		tagNpc(void) 
		{
			memset(this,0,sizeof(tagNpc));
		}
	};
	list<tagNpc*>*	GetNpcList()	{return &m_listNpc;}
	//	map<string,tagNpc*>* GetMapNpcList()	{ return &m_mapNpcList; }
private:
	//npc�б�
	list<tagNpc*>	m_listNpc;
	bool	m_bNpcListLoaded;

	//  [8/11/2009 Square]
public:
	void AddObject(CBaseObject* pObject);
	CShape* GetNearestShapeByType(OBJECT_TYPE,CPlayer*);
};
