#pragma once
#include "../App/MoveShape.h"

const int GOODSNAME_LEN_ =50;	//��Ʒ���ֳ���

struct tagConditionGoods
{
	char	cGoodsName[GOODSNAME_LEN_];
	DWORD	dwGoodsIndex;
	int		iNumType;
	int		iNum;
};

class CCollectionGoods
	: public CMoveShape
{
public:
	CCollectionGoods();
	~CCollectionGoods() {};
protected:
private:
	long  m_lCreatorType;
	CGUID m_CreatorGuid;
public:
	void SetAction(WORD lState);

	virtual bool DecordFromDataBlock(DBReadSet& readDB,bool bExData = true);
	// ���Monster�����ߵ����ͺ�ID
	long GetCreatorType()		{return m_lCreatorType;}
	CGUID GetCreatorID()		{return m_CreatorGuid;}
	// ����Monster�����ߵ����ͺ�ID
	void SetCreatorType(long type)		{m_lCreatorType = type;}
	void SetCreatorID(CGUID guid)		{m_CreatorGuid = guid;}
};