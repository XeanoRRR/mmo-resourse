//======================================================================================
/**
*	@file		GoodsTailorSetup.h
*	@data		2009-1-8
*	@author		�ſ���
*/
//======================================================================================
#pragma once

#include "../Common/GoodsDef.h"

/// ������������
enum eMakeType
{
	eMT_FORM_GOODS,
	eMT_FROM_NPC,
};

/// ��Ʒ������Ϣ/������
struct	tagExpend
{
	long lGoodsIndex;
	long lGoodsNum;

	bool operator==(tagExpend& RightObj){ return (this->lGoodsIndex == RightObj.lGoodsIndex); }
};


/// ��Ʒ������Ϣ/����
struct tagProduction : public tagExpend
{
	long lOutOdds;

	bool operator==(tagExpend& RightObj){ return (this->lGoodsIndex == RightObj.lGoodsIndex); }
};


/**
*	@class OneMakeTable
*
*	@brief һ���䷽�嵥
*/
class OneMakeTable
{
public:
	/// ����
	OneMakeTable(void);

	/// ����
	~OneMakeTable(void)
	{
		m_vExpend.clear();
		m_vProduction.clear();
	}

public:
	/// ����
	void Code(DBWriteSet& setWriteSet) const;

	/// ����
	void Decode(DBReadSet& setReadDB);

	/// �õ�ID
	long GetID (void){ return m_lID; }

	/// �õ�����
	const char*	GetName(void){ return m_szName; }

	/// �õ�˵��
	const char*	GetNarrate(void){ return m_szNarrate; }

	/// ���û�����Ϣ
	bool SetBaseInfo(long lID, const char* pName, const char* pNarrate);

	/// ���һ���ķ���
	bool AddExpend(tagExpend& Expend);

	/// ���һ��������
	bool AddProduction(tagProduction& Production);

	/// ȡ�úķ�������
	long GetExpendNum(void){ return (long)m_vExpend.size(); }

	/// ����IDȡ�úķ���
	tagExpend* GetExpend(ulong dwIndex) { return &m_vExpend[dwIndex]; }

	/// ȡ������������
	long GetProductionNum(void) { return (long)m_vProduction.size(); }

	/// ����IDȡ��������
	tagProduction* GetProduction(ulong dwIndex)	{ return &m_vProduction[dwIndex]; }

public:
	/// �Ƚ�����
	bool operator==(OneMakeTable& RightObj){ return (this->m_lID == RightObj.m_lID); }

protected:
	/// �䷽ID
	long m_lID;

	/// �䷽����
	char m_szName[MAKE_TABLE_NAME_SIZE];

	/// �䷽����
	char m_szNarrate[MAKE_TABLE_NARRATE_SIZE];

	/// �ķ����б�
	vector<tagExpend>			m_vExpend;

	/// �������б�
	vector<tagProduction>		m_vProduction;

private:
	/// ��������
	OneMakeTable(OneMakeTable&);

	/// ��ֵ������
	OneMakeTable& operator=(OneMakeTable&);
};


/**
*	@class GoodsTailorSetup
*
*	@brief �Ǹ�ְҵ�����䷽����
*/

class GoodsTailorSetup
{
public:
	/// ����
	GoodsTailorSetup(void);

	/// ����
	~GoodsTailorSetup(void);

public:
	/// ���طǸ�ְҵ�����䷽����
	static bool LoadGoodsTailorSetup(const char *filename);

	/// ����
	static void Code(DBWriteSet& setWriteSet);

	/// ����
	static void Decode(DBReadSet& setReadDB);

public:
	/// ���������䷽
	static void Release(void);

	/// ���һ���䷽
	static bool AddOneMakeTable(OneMakeTable* pOneMakeTable);

	/// �ҳ�һ���䷽
	static const OneMakeTable* FindOneMakeTable(long lID);
	
private:
	/// �䷽�б�
	static map<long, OneMakeTable*> m_mapOneMakeTable;
};