//======================================================================================
/**
*	@file		TradeList.h
*	@data		
*	@author		
*/
//======================================================================================

#pragma once

/**
*	@class TradeList
*
*	@brief �����б�����
*/

class TradeList
{
public:
	/// ����
	TradeList(void);

	/// ����
	~TradeList(void);

public:

	///  �̵�����
	enum eShopType
	{
		/// 0���ߵ�
		ARMOR_SHOP = 0,	
		/// 1���ߵ�						
		ITEM_SHOP,	
		/// 2���ε�							
		JEWEL_SHOP,		
		/// 3������						
		WEAPON_SHOP,		
		/// 4�̵��������					
		SHOP_NUM								
	};
	
	/// ��������
	enum eTradeType
	{
		/// ��Ǯ��
		ETT_MONEY = 1,		
		/// ����Ʒ��					
		ETT_GOODS,	
		/// ������							
		ETT_SILVER								
	};

	/// ��������
	enum eCreditType
	{
		/// ������
		CREDIT_NOT = 0,	
		/// ��������			
		CREDIT_AREA,						
		/// ��֯����	
		CREDIT_ORGAN							
	};

	/// ������Ʒ�ṹ
	struct tagTradeGoods
	{
		/// Ҫ������Ʒ��ID
		DWORD dwGoodsChgId;					
		/// Ҫ������Ʒ����	
		DWORD dwChgAmount;						
	};

	/// ��������
	struct tagTradeData
	{
		/// ��ƷID
		DWORD dwGoodsId;		
		/// ��Ʒ����				
		DWORD dwGoodsNum;		
		/// ��������ID				
		DWORD dwCreditId;				
		/// ��������ֵ		
		DWORD dwCreditVal;						
		/// ��������������Ʒ�б�	
		vector<tagTradeGoods>_vecTreadGoods;	

		/// ����
		tagTradeData()
			:dwCreditId(0),dwCreditVal(0){}
	};

	///  һ��NPC���׵���Ʒ�б�
	struct tagTrade
	{
		/// NPCԭʼ��	
		string	strNpcName;				
		/// �̵�����		
		eShopType shopType;						
		/// ���׷�ʽ
		eTradeType tradeType;					
		/// ��������
		eCreditType _creditType;				
		/// ��Ʒ�б�
		vector<tagTradeData>	vecGoods;		
	};

private:
	/// ���е�NPC�����б� map<NPCԭʼ���������б�>
	static map<string, tagTrade>	m_mapTradeList;

public:
	/// ���ļ����ؽ����б�
	static bool LoadTradeList(const char* filename);		

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ����NPC�̵���ȡ�ý����б�
	static tagTrade* GetTradeList(const char* strNpcName);
};
