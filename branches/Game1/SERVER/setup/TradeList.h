#pragma once
#include "..\..\public\tinystr.h"
#include "..\..\public\tinyxml.h"
class CTradeList
{
public:
	CTradeList(void);
	virtual ~CTradeList(void);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:

	// �̵�����
	enum eShopType
	{
		ARMOR_SHOP=0,				//0���ߵ�
		ITEM_SHOP,					//1���ߵ�
		JEWEL_SHOP,					//2���ε�
		WEAPON_SHOP,				//3������
		SHOP_NUM					//4�̵��������
	};

	enum eTradeType
	{
		ETT_MONEY=1,	//��Ǯ��
		ETT_GOODS,		//����Ʒ��
		ETT_SILVER	//������
	};

	enum eCreditType
	{
		CREDIT_NOT=0,
		CREDIT_AREA,	 //��������
		CREDIT_ORGAN	//��֯����
	};
	struct tagTradeGoods
	{
		DWORD dwGoodsChgId;	//Ҫ������Ʒ��ID
		DWORD dwChgAmount;	//Ҫ������Ʒ����
	};
	struct tagTradeData
	{
		DWORD dwGoodsId;	//��ƷID
		DWORD dwGoodsNum;	//��Ʒ����
		DWORD dwCreditId;		//��������ID
		DWORD dwCreditVal;		//��������ֵ
		vector<tagTradeGoods>_vecTreadGoods;//��������������Ʒ�б�	
		tagTradeData():dwCreditId(0),dwCreditVal(0){}
	};

	// һ��NPC���׵���Ʒ�б�
	struct tagTrade
	{
		string	strNpcName;					// NPCԭʼ��	
		eShopType shopType;					// �̵�����
		eTradeType tradeType;				// ���׷�ʽ
		eCreditType _creditType;			// ��������
		vector<tagTradeData>	vecGoods;	// ��Ʒ�б�
	};

private:
	static map<string, tagTrade>	m_mapTradeList;		// ���е�NPC�����б� map<NPCԭʼ���������б�>

public:
	static tagTrade* GetTradeList(const char* strNpcName)
	{
		return m_mapTradeList.find(strNpcName)!=m_mapTradeList.end()?
		(&m_mapTradeList[strNpcName]):NULL;
	}

public:
	static BOOL LoadTradeList(const char* filename);			// ���ļ���ȡ�б�
	static BOOL SaveTradeList(const char* filename);			// ���浽�ļ��б�

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����

	static bool	AddToClient(tagTrade* pTrade,DBWriteSet& dbWriteSet);// �����̵���Ʒ�б�
};
