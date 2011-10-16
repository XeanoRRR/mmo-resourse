



#ifndef _increment_shop_h_
#define _increment_shop_h_
#pragma once


enum eIncrShopClass
{
	//	@	Ԫ��������
	eSHOP_STAR,
	//	@ λ��
	eSHOP_PLANES,
	//	@	��ȯ
	eSHOP_TICKET,
	//	@
	eSHOP_NUM,
};

enum eShopGoodsList
{
	eGOODS_All,
	eGOODS_Top,
	eGOODS_New,
	eGOODS_Guide,
	eGOODS_2,
	eGOODS_3,
	eGOODS_NUM,
};


//	----------------------
//	@	name:	��ֵ�̵���
//	@	brief:	��ֵ�̵���������Ϸ��ֻ����һ��Ψһ��ʵ�����
//	-------------------------------------------------

class CIncrementShop
{
public:
	~CIncrementShop();

	static CIncrementShop*	GetIncrShop(void);

	void	DecodeIncrShop(class CMessage*	pMsg);

	bool	IsOpend(void)	{ return m_bOpend; }

	// @	�˽ӿ�ר��Ϊgt�ṩ [11/10/2009 Square]
	DWORD	RandomOneGoodsIndex(DWORD dwShopID,DWORD dwGoodsListID);

	//	bool	AddGoods(DWORD dwShopID,DWORD dwListID,DWORD dwGoodsIndex);

private:
	CIncrementShop();

	static CIncrementShop*	m_pIncrShop;

	//	!@ �Ƿ�򿪹�
	bool	m_bOpend;

	//	@	�̵���Ʒ�б�
	vector<DWORD>		m_vecIncrShopGoodsIndexList[eSHOP_NUM][eGOODS_NUM];
};
#endif//_increment_shop_h_