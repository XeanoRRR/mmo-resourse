
/*
*	file:		GoodsTailorSetup.h
*	Brief:		�Ǹ�ְҵ�����䷽����
*	detail:		
*	Authtor:	�ſ���
*	Datae:		2009-1-8
*/


#include "GoodsExDef.h"
#include "DataBlockSetWriteRead.h"

#pragma once

//!-------------------------------------------------------------------------------------------------
//! ������������
enum eMakeType
{
	eMT_FORM_GOODS,
	eMT_FROM_NPC,
};

//! һ����Ʒ������Ϣ/������
struct	tagExpend
{
	LONG lGoodsIndex;
	LONG lGoodsNum;

	BOOL operator==(tagExpend& RightObj){ return (this->lGoodsIndex == RightObj.lGoodsIndex); }
};


//! ����
struct tagProduction : public tagExpend
{
	LONG lOutOdds;

	BOOL operator==(tagExpend& RightObj){ return (this->lGoodsIndex == RightObj.lGoodsIndex); }
};


//!-------------------------------------------------------------------------------------------------
//! һ���䷽�嵥
class OneMakeTable
{
public:
	OneMakeTable(VOID);
	~OneMakeTable(VOID){}

public:
	//!							����
	VOID						Code					(DBWriteSet& setWriteSet) const;
	//!							����
	VOID						Decode					(DBReadSet& setReadDB);

	//!							�õ�ID
	LONG						GetID					(VOID){ return m_lID; }
	//!							�õ�����
	const char*					GetName					(VOID){ return m_szName; }
	//!							�õ�˵��
	const char*					GetNarrate				(VOID){ return m_szNarrate; }

	//!							���û�����Ϣ
	BOOL						SetBaseInfo				(LONG lID, const char *pName, const char *pNarrate);
	//!							���һ���ķ���
	BOOL						AddExpend				(tagExpend &Expend);
	//!							���һ��������
	BOOL						AddProduction			(tagProduction &Production);

	//!							�ķ���
	LONG						GetExpendNum( VOID )			{ return (LONG)m_vExpend.size(); }
	tagExpend*					GetExpend( DWORD dwIndex )		{ return &m_vExpend[ dwIndex ]; }

	//!							������
	LONG						GetProductionNum( VOID )		{ return (LONG)m_vProduction.size(); }
	tagProduction*				GetProduction( DWORD dwIndex )	{ return &m_vProduction[ dwIndex ]; }

public:
	//!							�Ƚ�����
	BOOL						operator==				(OneMakeTable& RightObj){ return (this->m_lID == RightObj.m_lID); }

protected:
	LONG						m_lID;
	char						m_szName[MAKE_TABLE_NAME_SIZE];
	char						m_szNarrate[MAKE_TABLE_NARRATE_SIZE];
	//!							�ķ����б�
	vector<tagExpend>			m_vExpend;
	//!							�������б�
	vector<tagProduction>		m_vProduction;




private:
	OneMakeTable(OneMakeTable&);
	OneMakeTable& operator=(OneMakeTable&);
};


//!-------------------------------------------------------------------------------------------------
//! �䷽����
class GoodsTailorSetup
{
public:
	GoodsTailorSetup(VOID);
	~GoodsTailorSetup(VOID);

public:
	static	GoodsTailorSetup&	GetInstance				(VOID);

public:
	//!							����
	VOID						Code					(DBWriteSet& setWriteSet);
	//!							����
	VOID						Decode					(DBReadSet& setReadDB);

public:
	//!							���������䷽
	VOID						Clear					(VOID);
	//!							���һ���䷽
	BOOL						AddOneMakeTable			(OneMakeTable *pOneMakeTable);
	//!							�ҳ�һ���䷽
	const	OneMakeTable*		FindOneMakeTable		(LONG lID);
	
protected:
	//!												�䷽�б�
	map<LONG, OneMakeTable*>						m_mapOneMakeTable;

	static	GoodsTailorSetup						c_GoodsTailorSetup;
};