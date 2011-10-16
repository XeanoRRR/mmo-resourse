// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CPERSONALSHOPSELLER_42266D9200FA_INCLUDED
#define _INC_CPERSONALSHOPSELLER_42266D9200FA_INCLUDED

#include "CPlug.h"
#include "..\Listener\CContainerListener.h"
#include "..\Container\CVolumeLimitGoodsShadowContainer.h"
#include "CPersonalShopBuyer.h"

class CContainer;
class CBaseObject;
class CGoods;
class CPersonalShopBuyer;

//���屣��Ľ�����Ϣ���������
const int MAX_TRADE_INFO = 5;

//##ModelId=42266D9200FA
class CPersonalShopSeller 
: public CPlug
, public CContainerListener
{
private:
	struct tagPrice
	{
		DWORD dwSellType;					//��Ʒ���۵ķ�ʽ����Ϊ���֣�0������������1�����鷷����
		DWORD dwOneGroupCnt;			//��Ʒ�ĵ������( ��������ʱ�趨Ϊ����1 )
		DWORD dwGroupNum;				//��Ʒ������
		DWORD dwPriceType;
		DWORD dwPrice;
		CGUID guid;
		LONG lGoodsId;
	};

	struct tagSellInfo
	{
		CGoods* pGoods;
		DWORD dwSellType;					//��Ʒ���۵ķ�ʽ����Ϊ���֣�0������������1�����鷷����
		DWORD dwOneGroupCnt;			//��Ʒ�ĵ������( ��������ʱ�趨Ϊ����1 )
		DWORD dwNum;							//��Ʒ������
		DWORD dwPriceType;					//��Ʒ���۵Ļ������ͣ���Ϊ���֣�0����ң�1��λ�澫����
		DWORD dwPrice;
		LONG lContainerId;
		LONG lPos;
	};
public:
	typedef vector<CPersonalShopBuyer::tagBuyInfo>	VecBuyInfo;
	typedef map<long, VecBuyInfo* >	MapBuying;		// <lPlugID, tagBuyInfo>
	typedef MapBuying::iterator ItrBuying;

	//##ModelId=422E5347006D
	BOOL GetGoodsList(vector<BYTE>& vOut);
	VOID GetUpdateGoodsList(vector<BYTE>& vOut);

	//##ModelId=42266DFF0242
	CPersonalShopSeller();

	//##ModelId=42266E09037A
	//CPersonalShopSeller(LONG lID);

	//##ModelId=42266E1A0109
	~CPersonalShopSeller();

	//##ModelId=42266E2A035B
	virtual BOOL IsPlugAvailable();

	//##ModelId=42266E34005D
	virtual BOOL OnSessionEnded();

	//##ModelId=42266E3C02EE
	virtual BOOL OnSessionAborted();

	//##ModelId=42266E4B00FA
	virtual BOOL OnPlugInserted();

	//##ModelId=42266E54005D
	virtual BOOL OnPlugAborted();

	//##ModelId=42266E6900AB
	virtual BOOL OnPlugEnded();

	//##ModelId=42266E710232
	virtual BOOL OnChangeState(LONG lPlugID, LONG lStateID, BYTE* pData);

	//##ModelId=42266FB40128
	virtual VOID Release();

	//������������������Ľ�ɫ��Ӱ������
	//##ModelId=42266FBD031C
	virtual CContainer* GetContainer(LONG lID);

	//##ModelId=422E542D01A5
	virtual BOOL OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

	//##ModelId=422E543D0251
	virtual BOOL OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

	//##ModelId=422E545301D4
	virtual BOOL OnTraversingContainer(CContainer* pContainer, CBaseObject* pObject);

	//##ModelId=42267E6701D4
	BOOL OpenForBusiness();

	//�����̵����
	//##ModelId=4227C0520186
	BOOL CloseDown();

	//##ModelId=422FAFB40196
	BOOL DoesShopOpenedForBusiness();

	//##ModelId=422BAFA30148
	BOOL SetShopName(CHAR* strName);

	//##ModelId=422BAFBD007D
	char* GetShopName();

	//ȡ�õ����ƽ���Ϣ
	char* GetShopInfo();
	//���õ����ƽ���Ϣ
	BOOL SetShopInfo( string strShopInfo );
	//ȡ�ý�����Ϣ
	vector<string>& GetTradeInfo();
	//����һ��������Ϣ
	void InsertTradeInfo( const string& strTradeInfo );

	//##ModelId=422BB13A02EE
	//������Ʒ�۸���Ϣ������������ʽ��Ϣ��
	BOOL SetGoodsPrice(const CGUID& guid,LONG lGoodsId, WORD wSellType, 
		WORD dwPriceType, DWORD dwGroupNum,  DWORD dwOneGroupCnt,DWORD dwPrice);

	//##ModelId=422BB17300AB
	//ȡ����Ʒ�۸���Ϣ
	BOOL GetGoodsPrice(const CGUID& guid,LONG lGoodsId,DWORD &dwPriceType, DWORD& dwPrice);
	//ȡ����Ʒ������ʽ��Ϣ
	BOOL GetGoodsSellType( const CGUID& guid, LONG lGoodsId, WORD& wSellType, DWORD& dwGroupNum, DWORD& dwOneGroupCnt );

	//�Ƿ����ָ��goodsid����Ʒ�۸�
	BOOL GoodsPriceExist(LONG lGoodsId);


	//�����������
	BOOL CheckCondition(LONG lPlugID, DWORD &dwTotalMoney, DWORD& dwTotalWeimian );
	//����
	bool PurchaseEx(LONG lPlugID, DWORD dwTotalMoney, long lTotalYuanbao, long lTotalWeimian );

	VOID ClearData(){m_mGoodsPrice.clear();m_cSample.Clear();}

	//##ModelId=422FD7B40167
	DWORD GetValidGoodsAmount();
	DWORD GetValidGoodsAmountEx(); 

	CGoodsShadowContainer::tagGoodsShadow* RemoveByGoodsId(DWORD dwId);
	VOID AddShadowTag(CGoodsShadowContainer::tagGoodsShadow* pTag);

	void AddMapBuying(long lPlugID, VecBuyInfo *pVecBuyInfo);
	void DelMapBuying(long lPlugID);
	void ReleaseMapBuying();

private:
	//##ModelId=42267E500232
	BOOL m_bShopOpened;

	//##ModelId=422BAF8A000F
	//�̵�����
	string m_strShopName;
	//�����ƽ�
	string m_strShopInfo;
	//������Ϣ
	vector<string> m_vecTradeInfo;

	//##ModelId=422BB05B01D4
	map<CGUID, tagPrice,guid_compare> m_mGoodsPrice;
	typedef map<CGUID, tagPrice,guid_compare>::iterator itGoodsPrice;

	//##ModelId=4226A3780196
	CVolumeLimitGoodsShadowContainer m_cSample;

	MapBuying	m_mapBuying;

public:
	enum CheckResult
	{
		CR_WAIT_FINISH	= 0,
		CR_WAIT_YUANBAO,
		CR_MISS_MONEY,
		CR_MISS_WEIMIAN,				//λ�治��
		CR_OTHR_ERROR,
	};

	//������ʽ����������/���鷷����
	enum eSellType
	{
		ST_SINGLE_SELL,
		ST_GROUP_SELL,
	};
	//�۸����ͣ���ҽ���/λ�潻�ף�
	enum ePriceType
	{
		PT_GOLD_SELL,
		PT_WEIMIAN_SELL,
	};
};

#endif /* _INC_CPERSONALSHOPSELLER_42266D9200FA_INCLUDED */
