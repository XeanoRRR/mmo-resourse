/**
* @filename:	PlayerShop.h
* @date:		2010/8/2
* @author:		wing (totti19851101@gmail.com) 

* @purpose: �����̵꽻��
*/

#pragma once

#include "../Goods/Goods.h"
#include "../Player.h"

class PlayerShop
{
public:
	/// ���׷�ʽ
	enum eTradeType
	{
		TT_SINGLE,			///< ��������
		TT_GROUP,			///< ���齻��
		TT_NUM,				///< ���׷�ʽ��
	};

	/// ��������
	enum tagPriceType
	{
		PT_Money,			///< ���
		PT_WeiMian,			///< λ�澫��
		PT_NUM				///< ����������
	};

	/// ����״̬
	enum ePlayerShopState
	{
		SET_SHOP,			///< 0��������
		OPEN_SHOP,			///< 1������
		SHOPPING_SHOP,		///< 2����򿪱������̵꣩
		SHOP_STATE			///< 3�̵�״̬��
	};

	/// ������Ʒ����<������>
	struct tagTradeGoodsInfo
	{
		ulong goodsId;		///< ��Ʒ��ID
		ulong contentId;	///< ������ID
		ulong index;		///< �������ж�Ӧ����λ
		ulong curNum;		///< ����Ʒ�ڱ���λ������
	};

	/// ������Ʒ����<�ͻ���>
	struct tagGoodsItem
	{
		bool	isReadyTrade;			///< �Ƿ����ڴ�������
		ulong	readyTradeNum;			///< ������Ʒ��Ԥ��������

		eTradeType tradeType;			///< ���׷�ʽ����������&���齻�ף�
		int		priceType;				///< �۸����ͣ����&λ�澫����
		ulong	price;					///< �۸�
		ulong	groupNum;				///< ����
		ulong	oneGroupNum;			///< �������

		ulong	goodsId;				///< ��ƷID
		ulong	goodsIconId;			///< ��Ʒͼ��ID
		ulong	totalNum;					///< Ҫ��ʾ�ı����д�����Ʒ��������
		string	strGoodsName;			///< ��Ʒ����
		CGUID   guid;					///< ��Ʒ��GUID

		vector<tagTradeGoodsInfo> vecTradeGoodsInfo;	///< ����Ʒ�б���Ľ�����Ϣ
		CGoods* pItemGoods;				///< ��Ʒָ��

		tagGoodsItem()
		{
			isReadyTrade = false;
			readyTradeNum = 0;

			tradeType = TT_SINGLE;
			priceType = PT_Money;
			price = 0;
			groupNum = 0;
			oneGroupNum = 0;

			goodsId = 0;
			goodsIconId = 0;
			totalNum = 0;
			pItemGoods = NULL;
			strGoodsName = "";
			memset(&guid,0,sizeof(CGUID));
		};
	};

	/// �������Ʒ����
	struct stCacheGoodsInfo 
	{		
		eTradeType	treadeType;				///< ���׷�ʽ
		int			priceType;				///< �۸�����
		ulong		price;					///< �۸�
		ulong		oneGroupNum;			///< ������Ʒ����
		ulong		groupNum;				///< ����
		int			shopPos;				///< ���̵��е�λ��
		stCacheGoodsInfo(void)
		{
			treadeType = TT_SINGLE;
			priceType = PT_Money;
			price = 0;
			oneGroupNum = 1;
			groupNum = 0;
			shopPos = 0;
		}
		stCacheGoodsInfo(eTradeType e, int pt, ulong p, ulong ogn, ulong gn, int sp)
		{
			treadeType = e;
			priceType = pt;
			price = p;
			oneGroupNum = ogn;
			groupNum = gn;
			shopPos = sp;
		}
	};


	typedef vector<tagGoodsItem> stGoodsItem;
	typedef stGoodsItem::iterator  GoodsItemIter;
	typedef map<ulong,stCacheGoodsInfo> stCacheGoods;
	typedef stCacheGoods::iterator stCacheIter;

public:
	PlayerShop(void);
	~PlayerShop(void);

public:
	/// ���ø����̵������Ϣ����
	void SetShopPara(const CGUID& sessionID, long plugID, const CGUID& masterID);
	/// ���õ�ǰ�ĸ����̵�����
	void SetCurShopState(int state);
	/// ��ȡ��ǰ�ĸ����̵�����
	int	 GetCurShopState(void);
	/// �����Ƿ�Ϊ�򿪸����̵�״̬
	void SetIsOpenShop(bool b);
	/// ȡ�ø����̵������
	const int GetShopGridNum(void) const { return ITEMCOUNTS; }
	/// ����Master��ID
	void SetMasterID(const CGUID& id) { m_masterID=id; }
	/// ���Master��ID				
	const CGUID& GetMasterID() { return m_masterID; }
	/// ȡ�ø����̵�ỰID
	CGUID& GetSessionID(void) { return m_sessionID; }
	/// ȡ�ÿ��̵���PlugID
	long GetPlugID(void) const { return m_plugID; }
	/// ���ص�ǰ�̵����Ʒ��
	int GetCurGoodsNum()	{return m_curGoodsNum;}
	/// ���õ�ǰ�̵����Ʒ��
	void SetCurGoodsNum(int num)	{ m_curGoodsNum = num; }


	/// ȡ�ý���Ԥ����������(����) & ����Ԥ�ڻ��ѽ����(���)
	ulonglong GetTradeGold(void) const { return m_tradeGolds; }
	/// �����ϼ���Ʒ��Ϣ
	tagGoodsItem* FindtagGoods(CGoods* pGoods);

	// ������谴ťʱ��ȡ��Ԥ��������
	void CancelAllTrade(void);

	/// ��ȡ���µĸ����̵�������Ʒ�б�
	void UpdataPlayerShopGoodsList(void);


	bool OnCloseShop(void);
	void CloseShop();
	inline void ClearMyShopList()      { m_sellerShopGoodsList.clear();}		// ��յ�������Ʒ�б�
	


	// ��Ϣ���ͽӿ�
public:
	/// ���ͼ�����Ʒ��Ϣ
	//void SendAddGoodsMes(int nPos,long lPlugID);
	/// ���Ϳ�����Ϣ
	void SendOpenShopMes(void);
	/// ���͹ر��̵���Ϣ
	void SendCloseShopMes(void);
	/// ���͹�����Ϣ
	void SendBuyGoodsMes(void);
	/// �����Ƴ���Ʒ��Ϣ
	//void SendRemoveGoodsMes(int index);


	// ��Ʒ�����嵥���ܽӿ�
public:
	/// ��Ӧ����Ʒ�����嵥
	bool OnOrderPageOpen(CGoods* pGoods,int shopPos);
	/// ������Ʒ�����嵥�������õ���Ʒ
	void SetOrderGoods(CGoods* goods) { m_orderGoods = goods; }
	/// ȡ����Ʒ�����嵥�������õ���Ʒ
	CGoods* GetOrderGoods(void) { return m_orderGoods; }
	/// �����Ʒ�����嵥�Ƿ�����OK
	bool IsOrderSetupOK(void);
	/// ��Ʒ�����嵥����OK
	bool OnShopOrderOK(void);
	/// ������ƷID�ڻ����б��в�����Ʒ
	stCacheGoodsInfo* GetCacheGoodsInfo(ulong goodsID);



	// �����̵����ҹ��ܽӿ�
public:
	/// ���Ҫ��������Ʒ
	void AddMyShopGoods(CGoods *pGoods,int iPos/*���̵��е�λ��*/,eTradeType eDType,
		DWORD dwGoodsNumPerTeam,DWORD dwTeamNum ,DWORD dwPrice,int iPriceType );
	/// �������̵���ָ�����е���Ʒ�Ƴ�
	bool ReMoveShopGoods(long lGoodsID,CGUID guid);
	/// ȡ���Լ���̯��Ʒ�б�
	vector<tagGoodsItem>& GetMyShopGoods(void) { return m_sellerShopGoodsList; }
	///���µ�������Ʒ�б�
	void UpdateMyShopList(const CGUID& guid,DWORD dwIndex,long lExtendID,DWORD dwPos,long lNum,int iAddOrSub);

	// �����̵���ҹ��ܽӿ�
public:
	/// �����̵��м���������ϼ���Ʒ
	void AddPlayerShopGoods(CGoods* pGoods, ulong groupNum, ulong priceType,
		ulong price, ulong tradeType, ulong oneGroupNum);
	/// �жϽ������Ƿ���������
	bool IsTradeItemReady(void);
	/// ���ݵ�ǰ׼��������Ʒ����㽻�׶�
	void CalcTradeGolds(void);
	/// �������ȷ�������־
	void SetbSetTrue(bool bSetValue) {m_bSetSure = bSetValue;}	
	// ��ȡȷ����־
	bool GetbSetTrue(void) {return m_bSetSure;}		
	/// �����Ҵ洢�ĵ����ϼ���Ʒ�б�
	void ClearPlayerShopList(void);
	/// ���µ����ϼ���Ʒ����
	void UpdateSellGoodsNum(const CGUID& guid, ulong index,ulong groupNum);

private:
	// ��������̵��ʱ����������Ʒ���һ�����Ӧ����
	bool OnInputGoods(CPlayer::tagGoods* pstGoods);

	// ��ȡҪ�����Ʒ�������
	long GetBuyGoodsNum(void);

private:
	static const int ITEMCOUNTS;			///< �����̵������
	CGUID m_masterID;						///< ��ǰ�����̵�����ID
	CGUID m_sessionID;						///< �����̵�ỰID
	long m_plugID;							///< �������̵��˵�ID

	int  m_curShopState;					///< �����̵�״̬
	int  m_curGoodsNum;						///< ��ǰ�̵�����Ʒ�ĸ���
	bool m_isOpenShop;						///< �����̵�򿪱�ʾ

	ulonglong m_tradeGolds;					///< ����Ԥ����������(����) & ����Ԥ�ڻ��ѽ����(���)
	CGoods* m_orderGoods;					///< �����嵥�����ڴ����ĳ������ĳһ����Ʒ�е�һ��������Ʒָ��

	stGoodsItem m_buyerShopGoodsList;		///< �����Ʒ�б�(�鿴�̵�ʱ����)
	stGoodsItem m_sellerShopGoodsList;		///< ������Ʒ�б�(��̯ʱ����)
	stCacheGoods m_mapCacheGoods;			///< ���������Ʒ��Ϣ����


	string m_strShopName;					///< ������̵�����
	string m_strOwnerRecommend;				///< ����ĵ����Ƽ�
	string m_ShopName;								// �����̵������
	bool m_bSetSure;								// ��Ʒ��������ȷ��ʱ���˱�־ΪTrue,��ʾ��Ʒ��������
	DWORD m_dwTradeWeiMian;						// ��ǰλ�澫���Ľ�����

	//DWORD m_dwShopNameCDTime;					// �޸��̵����Ƶ�CDʱ��ֵ
	//DWORD m_dwShopRecommendCDTime;				// �޸��̵��Ƽ����ݵ�CDʱ��ֵ
	//long m_lToChaterID;							// Ҫ����������Ϣ�Ķ����PlugID
};

#define GetPlayerShop() GetInst(PlayerShop)