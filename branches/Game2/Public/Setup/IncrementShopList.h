//======================================================================================
/**
*	@file		IncrementShopList.h
*	@data		
*	@author		
*/
//======================================================================================
#pragma once

/**
*	@class CIncrementShopList
*
*	@brief ��ֵ�̵�������
*/
class CIncrementShopList
{
public:
	/// ����
	CIncrementShopList(void);

	/// ����
	~CIncrementShopList(void);

public:
	/// ��������
	enum eTradeMode
	{
		TM_YUANBAO,											//Ԫ������ �����꽻�ף�
		TM_WEIMIAN,											//λ�潻��
		TM_TICKET,											//��ȯ����
	};

	/// ���������
	typedef enum eMode
	{
		M_TOP10,
		M_NEWGOODS,
		M_GUIDETYPE,
		M_FILTER_XINGZUAN,
		M_FILTER_WEIMIAN,
		M_FILTER_TICKET,
	}eDecodeMode, eCodeMode;

public:
	/// ��ֵ�̵���ϸ��Ϣ�ṹ
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

	/// �����¼���Ա�
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

private:
	/// ����
	static string				m_sAffiche;
	/// ������Ʒ�б�
	static MAP_TOP10			m_mTop10List;									
	/// ��Ʒ�Ƽ��б�
	static MAP_NEWGOODS			m_mNewGoodsList;								
	/// ������Ʒ�б�
	static MAP_GUIDEGOODS		m_mGuideGoodsList;								
	/// λ���̵�ɸѡ��Ʒ�б�
	static MAP_FILTERGOODS		m_mFilterListOfWShop;							
	/// �����̵�ɸѡ��Ʒ�б�
	static MAP_FILTERGOODS		m_mFilterListOfXShop;							
	/// ��ȯ�̵�ɸѡ��Ʒ�б�
	static MAP_FILTERGOODS		m_mFilterListOfTShop;							
	/// ��Ʒ��ϸ��Ϣ�б�
	static MAP_INCGOODS			m_mIncGoodsList;								
	/// ��Ʒ����ӳ���
	static MAP_INCCLASSGOODS	m_mIncClassGoodsList;							

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

public:
	/// ���ļ�������ֵ�̵��б�
	static bool LoadIncrementShopList(const char* filename);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ȡ���޹���Ŀ
	static long GetLimitNumByID(DWORD dwShopType, DWORD dwGoodsIdx);	

	/// ȡ��������Ŀ
	static long GetWorldBuyLimitNum(DWORD dwShopType, DWORD dwGoodsIdx);	

	/// ȡ����ʱ��Ϣ
	static string GetLimitTimeByID(DWORD dwShopType, DWORD dwGoodsIdx);	

	/// ʱ��Ƚ�
	static bool TimeLimited(const string& strTime);

private:
	/// ������Ʒ�б�
	static void ParseGoodsList( TiXmlElement* , VEC_GOODSIDX& );

	//����
	/// ��������&��Ʒ�Ƽ��б�
	static void CodeShopGoodsList( vector<uchar>* pByteArray, eCodeMode eCodeMode );
	static void CodeShopGoodsListToByteArray( vector<uchar>* pByteArray, MAP_TOP10&  mapGoodsList );
	//���뵼��&ɸѡ�б�
	static void CodeTypeGoodsList( vector<uchar>* pByteArray, eCodeMode eCodeMode );
	static void CodeTypeGoodsListToByteArray( vector<uchar>* pByteArray,MAP_GUIDEGOODS&  mapTypeGoodsList );
	//������Ʒ��ϸ��Ϣ�б�
	static bool CodeDetailedGoodsList( vector<uchar>* pByteArray );
	//������Ʒ����ӳ���
	static bool CodeIncClassGoodsList( vector<uchar>* pByteArray );

	//����
	//������Ʒ�б�
	static void DecordGoodsList( uchar* pByte, long& pos, VEC_GOODSIDX& vecGoodsList );
	//����������Ʒ&��Ʒ�Ƽ��б�
	static void DecordShopGoodsList( uchar* pByte, long& pos, eMode eDecordMode );
	//���뵼��&ɸѡ��Ʒ�б�
	static void DecordTypeGoodsList( uchar* pByte, long& pos, eMode eDecordMode );
	//������Ʒ��ϸ��Ϣ�б�
	static void DecordDetailedGoodsList( uchar* pByte, long& pos );
	//������Ʒ����ӳ���
	static void DecordIncClassGoodsList( uchar* pByte, long& pos );

	//�������
	static void ClearIncShop();
};
