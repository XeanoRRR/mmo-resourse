#pragma once
#ifndef _CIncrementShopList_h_
#define _CIncrementShopList_h_


#include "..\..\public\tinystr.h"
#include "..\..\public\tinyxml.h"

class CIncrementShopList
{
public:
	CIncrementShopList(void);
	virtual ~CIncrementShopList(void);


public:
	enum eTradeMode
	{
		TM_YUANBAO,											//Ԫ������ �����꽻�ף�
		TM_WEIMIAN,											//λ�潻��
		TM_TICKET,											//��ȯ����
	};

	enum eProfitMode
	{
		PM_NORMAL,
		PM_PROFIT,
	};

	enum eShopCloseMode
	{
		CM_NORMAL,
		CM_UPDATA,
	};

	typedef enum eMode
	{
		M_TOP10,
		M_NEWGOODS,
		M_GUIDETYPE,
		M_FILTER_XINGZUAN,
		M_FILTER_WEIMIAN,
		M_FILTER_TICKET,
	}eDecodeMode, eCodeMode;
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
public:
	struct IncrementShopElem
	{
	public:
		DWORD	dwGoodsType;									//��Ʒ����
		DWORD	dwGoodsIndex;									//��Ʒ���
		DWORD	dwPrice;										//�۸�
		DWORD   dwGoodsNum;										//����
		DWORD	dwDiscount;										//����
		string	strTimeLimit;									//��ʱ
		long	lQuantityLimit;									//����
		long	lBuyLimit;										//�޹�

	public:
		IncrementShopElem()
			: dwGoodsType(0)
			, dwGoodsIndex(0)
			,dwPrice(0)
			, dwGoodsNum(0)
			,dwDiscount(0)
			,strTimeLimit("")
			,lQuantityLimit(0)
			,lBuyLimit(0)
		{
		}

		IncrementShopElem( DWORD GoodsType,DWORD GoodsIndex, DWORD Price, DWORD GoodsNum, 
			DWORD Discount, string TimeLimit, long QuantityLimit, long BuyLimit )
			: dwGoodsType(GoodsType)
			, dwGoodsIndex(GoodsIndex)
			,dwPrice(Price)
			, dwGoodsNum(GoodsNum)
			,dwDiscount(Discount)
			,strTimeLimit(TimeLimit)
			,lQuantityLimit(QuantityLimit)
			,lBuyLimit(BuyLimit)
		{
		}
	};

	//���������صĲ���
	struct  tagReturnParam
	{
		float fNoramlRatio;										//��ͨ��������
		float fSpecialRatio;									//���ⷵ������
		string strStartTime;									//���ⷵ���Ŀ�ʼʱ��
		string strEndTime;										//���ⷵ���Ľ���ʱ��
		list<string> lSGoodsName;								//��ִ�з�����������Ʒ�б�
	};


	//�����¼���Ա�
	struct IncShopTradeLog
	{
	public:
		string	strBuyTime;										// ����ʱ��
		DWORD	dwPriceNum;										// ��������
		DWORD	dwPriceType;									// ��������
		DWORD   dwGoodsIdx;										// ��ƷID
		DWORD	dwBuyNum;										// ��������
	public:
		IncShopTradeLog()
			:strBuyTime(""),dwPriceNum(0),dwPriceType(0),dwGoodsIdx(0),dwBuyNum(0)
		{}
		IncShopTradeLog( string BuyTime, DWORD PriceNum, DWORD PriceType, DWORD GoodsIdx, DWORD BuyNum )
			:strBuyTime(BuyTime),dwPriceNum(PriceNum), dwPriceType(PriceType),dwGoodsIdx(GoodsIdx),dwBuyNum(BuyNum)
		{}
	};

public:
	typedef vector<DWORD> VEC_GOODSIDX;										//vector<dwGoodsIndex>
	//������Ʒ&��Ʒ�Ƽ����ݽṹ
	typedef map< DWORD, VEC_GOODSIDX >    MAP_TOP10, MAP_NEWGOODS,			//< dwShopType, vector<dwGoodsIndex> >
										MAP_CLASSGOODS;						//< dwClassType, vector<dwGoodsIndex> >

	//����&ɸѡ ���ݽṹ
	typedef map<string, VEC_GOODSIDX > MAP_TYPEGOODS;						//< strGuideType, vector<dwGoodsIndex> >
																			//< strFilterType, vector<dwGoodsIndex> >
	typedef map< DWORD, MAP_TYPEGOODS>   MAP_GUIDEGOODS,
										MAP_FILTERGOODS;					//< dwShopType & dwGoodsType, map<> >
	//��Ʒ��ϸ��Ϣ���ݽṹ
	typedef map<DWORD, IncrementShopElem>			MAP_INCSHOP;			// <dwGoodsIndex, IncrementShopElem>
	typedef map< DWORD, MAP_INCSHOP > MAP_INCGOODS;							//< dwShopType, map<> >
	typedef map<DWORD, MAP_CLASSGOODS> MAP_INCCLASSGOODS;					//< dwShopType, map<> >
	//�ֺ�ӳ��ṹ
	typedef map<DWORD,DWORD> MAP_INCMAP;
	//�����¼�б�
	typedef vector<IncShopTradeLog> VEC_BUYLIST;

	typedef VEC_GOODSIDX::iterator ITR_GOODSIDX;
	typedef MAP_TOP10::iterator ITR_TOP10;
	typedef MAP_TOP10::iterator ITR_NEWGOODS;
	typedef MAP_TYPEGOODS::iterator ITR_TYPEGOODS;
	typedef MAP_GUIDEGOODS::iterator ITR_GUIDEGOODSLIST;
	typedef MAP_FILTERGOODS::iterator ITR_FILTERTGOODSLIST;
	typedef MAP_INCSHOP::iterator ITR_INCSHOP;
	typedef MAP_INCGOODS::iterator ITR_INCGOODS;
	typedef VEC_BUYLIST::iterator ITR_BUYLIST;
	typedef VEC_BUYLIST::reverse_iterator RITR_BUYLIST;
	typedef MAP_CLASSGOODS::iterator ITR_CLASSGOODS;
	typedef MAP_INCCLASSGOODS::iterator ITR_INCCLASSGOODS;
	typedef MAP_INCMAP::iterator ITR_MAPLIST;

private:
	static string				m_sAffiche;										//����

	static MAP_TOP10			m_mTop10List;									//������Ʒ�б�
	static MAP_NEWGOODS			m_mNewGoodsList;								//��Ʒ�Ƽ��б�
	static MAP_GUIDEGOODS		m_mGuideGoodsList;								//������Ʒ�б�
	static MAP_FILTERGOODS		m_mFilterListOfWShop;							//λ���̵�ɸѡ��Ʒ�б�
	static MAP_FILTERGOODS		m_mFilterListOfXShop;							//�����̵�ɸѡ��Ʒ�б�
	static MAP_FILTERGOODS		m_mFilterListOfTShop;							//��ȯ�̵�ɸѡ��Ʒ�б�
	static MAP_INCGOODS			m_mIncGoodsList;								//��Ʒ��ϸ��Ϣ�б�
	static MAP_INCCLASSGOODS	m_mIncClassGoodsList;							//��Ʒ����ӳ���

	static MAP_INCSHOP			m_mMapIncGoodsList;								//����ӳ�����ñ�
	static MAP_INCMAP			m_mMapList;
	static tagReturnParam		m_tgReturnParm;									//��������ز���

public:
	static string*			GetAffiche()			{ return &m_sAffiche; }
	static MAP_TOP10* GetTop10List() { return &m_mTop10List; }
	static MAP_NEWGOODS* GetNewGoodsList() { return &m_mNewGoodsList; }
	static MAP_GUIDEGOODS* GetGuideGoodsList() { return &m_mGuideGoodsList; }
	static MAP_FILTERGOODS* GetFilterListOfWShop() { return &m_mFilterListOfWShop; }
	static MAP_FILTERGOODS* GetFilterListOfXShop() { return &m_mFilterListOfXShop; }
	static MAP_FILTERGOODS* GetFilterListOfTShop() { return &m_mFilterListOfTShop; }
	static MAP_INCGOODS* GetIncGoodsList() { return &m_mIncGoodsList; }
	static MAP_INCCLASSGOODS* GetIncClassGoods() { return &m_mIncClassGoodsList; }

	static MAP_INCSHOP* GetMapIncShopList() {return &m_mMapIncGoodsList;}
	static MAP_INCMAP* GetMapIndexList() { return &m_mMapList; }
	static tagReturnParam* GetReturnParam() {return &m_tgReturnParm;}

public:
	static bool LoadIncrementShopList(const char* filename);						// ���ļ���ȡ�б�
	static bool AddToByteArray(vector<BYTE>* pByteArray);							// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);						// ����

	static long GetLimitNumByID( DWORD dwShopType, DWORD dwGoodsIdx );				//ȡ���޹���Ŀ
	static long GetWorldBuyLimitNum( DWORD dwShopType, DWORD dwGoodsIdx );			//ȡ��������Ŀ
	static string GetLimitTimeByID( DWORD dwShopType, DWORD dwGoodsIdx );			//ȡ����ʱ��Ϣ
	static bool TimeLimited(const string& strTime);									//ʱ��Ƚ�

	static bool IsProfitGoods(DWORD dwGoodsID);										// ��ѯһ����Ʒ�Ƿ��Ƿֺ���Ʒ
	static bool IsReturnDianGuan(DWORD dwGoodsIdx);									//�ж��Ƿ񷵻ص��
	static long GetReturnNum(long lTotalAmount,float &fReturnRatio);									//�����������
	static bool IsStartTime();														//�ж���ʼʱ������
	static bool IsEndTime();														//�жϽ���ʱ������
	static bool IsHour();															//�ж�Сʱ


	//
	//public:
	//	static bool	GetReloadFlag()						{ return m_bReload; }
	//	static void SetReloadFlag(bool bReload = true)	{ m_bReload = bReload; }
	//private:
	//	static bool	m_bReload;

private:
	//������Ʒ�б�
	static void ParseGoodsList( TiXmlElement* , VEC_GOODSIDX& );

	//����
	//��������&��Ʒ�Ƽ��б�
	static void CodeShopGoodsList( vector<BYTE>* pByteArray, eCodeMode eCodeMode );
	static void CodeShopGoodsListToByteArray( vector<BYTE>* pByteArray, MAP_TOP10&  mapGoodsList );
	//���뵼��&ɸѡ�б�
	static void CodeTypeGoodsList( vector<BYTE>* pByteArray, eCodeMode eCodeMode );
	static void CodeTypeGoodsListToByteArray( vector<BYTE>* pByteArray,MAP_GUIDEGOODS&  mapTypeGoodsList );
	//������Ʒ��ϸ��Ϣ�б�
	static bool CodeDetailedGoodsList( vector<BYTE>* pByteArray );
	//������Ʒ����ӳ���
	static bool CodeIncClassGoodsList( vector<BYTE>* pByteArray );
	//�������ӳ�����ñ�
	static bool CodeMapIncShopList( vector<BYTE>* pByteArray );
	//���뷵���������ñ�
	static bool CodeReturnParam(vector<BYTE>* pByteArray);


	//����
	//������Ʒ�б�
	static void DecordGoodsList( BYTE* pByte, long& pos, VEC_GOODSIDX& vecGoodsList );
	//����������Ʒ&��Ʒ�Ƽ��б�
	static void DecordShopGoodsList( BYTE* pByte, long& pos, eMode eDecordMode );
	//���뵼��&ɸѡ��Ʒ�б�
	static void DecordTypeGoodsList( BYTE* pByte, long& pos, eMode eDecordMode );
	//������Ʒ��ϸ��Ϣ�б�
	static void DecordDetailedGoodsList( BYTE* pByte, long& pos );
	//������Ʒ����ӳ���
	static void DecordIncClassGoodsList( BYTE* pByte, long& pos );
	//�������ӳ�����ñ�
	static void DecordMapIncShopList( BYTE* pByte, long& pos );
	//���뷵���������ñ�
	static void DecordReturnParam(BYTE* pByte, long& pos);


	//�������
	static void ClearIncShop();
};

#endif