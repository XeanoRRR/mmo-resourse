///
/// @file Shop.h
/// @brief Implement a NPC shop for FBusiness.
///
#ifndef ___FBUSINESS_SHOP_H_
#define ___FBUSINESS_SHOP_H_

#include "../../../../public/FBusinessSvrCommon.h"
#include "CachedSellData.h"

namespace FBusiness
{
	class CShop : public BaseMemObj
	{
	public:
		struct BuyGoods
		{
			long lIndex;
			long lPrice;
		};
		typedef std::vector<BuyGoods> BuyGoodsListT;
	public:
		CShop( long lId, const char *npcOrigName, const SellGoodsListT &goods_list );
	
		void SetSellList( const SellGoodsListT &goods_list );

		void RefreshBuyGoodsList();

		/// ������۵���Ʒ�б�
		void EncodeSellGoodsList( DBWriteSet &db );

		/// �����չ�����Ʒ�б�
		void EncodeBuyGoodsList( DBWriteSet &db );

		/// ��Ӧ��ҹ�����Ϣ
		void OnPlayerBuyMsg( CMessage *pMsg, const CGUID &npcID );

		/// ��Ӧ��ҳ�����Ϣ
		void OnPlayerSellMsg( CMessage *pMsg, const CGUID &npcID );

		/// ��Ӧ���ȷ�ϳ��ۼ۸�䶯��Ϣ
		void OnPlayerConformSellMsg( CMessage *pMsg, const CGUID &npcID );

		/// ��Ӧ��ҹر��̵���Ϣ
		void OnPlayerCloseMsg( CMessage *Msg );

		/// ��ҹر��̵�(����������)
		void PlayerClose( CPlayer *pPlayer );

		/// ��ȡ���̵��Ӧ��NPCԭʼ��
		const char *GetNpcOrigName() const
		{
			return m_npcOrigName;
		}
	private:
		/// ���۸��Ƿ�䶯
		bool IsPriceChanged( long lIndex, long lPrice );

		/// ��ȡָ����Ʒ���չ��۸�
		long GetBuyPrice( long lIndex );

		/// ִ����ҳ��۵Ĳ���
		void DoPlayerSell( CPlayer *pPlayer, const CCachedSellData::SellGoodsListT &sell_list );

		/// ���ָ����Ʒ�Ƿ���NPC�����б���
		bool IsInSellList( long lIndex );
	private:
		CCachedSellData m_CachedSellData;
		SellGoodsListT m_SellGoodsList; ///< �����б�
		BuyGoodsListT m_BuyGoodsList; ///< �չ��б�
		long m_lID;
		char m_npcOrigName[32];
	};
}

#endif
