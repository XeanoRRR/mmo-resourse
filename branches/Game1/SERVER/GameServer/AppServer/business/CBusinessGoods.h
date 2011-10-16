///
/// @file CBusinessGoods.h
/// @brief ��ҵϵͳ��Ʒģ�� 
///
#ifndef ___BUSINESS_GOODS_H_
#define ___BUSINESS_GOODS_H_

#include "../../../../public/BusinessGoodsDef.h"
#include "../../../../public/GUID.h"

namespace Business
{
	class CTradeSpot;

	class CBusinessGoods : public BaseMemObj
	{
	public:
		typedef stdext::hash_map<CGUID, DWORD, hash_guid_compare> BuyLimitTable;
	public:
		CBusinessGoods( CTradeSpot *pSpot );
		CBusinessGoods( const tagGoods &bp, CTradeSpot *pSpot );
		
		///
		/// ����ǰ��ʱ���������������µ�ǰ�����۸�������������������
		/// ��۱仯ϵͳ�μ��߻��ĵ���������ֵ��ʽ��
		///
		void UpdatePrice();

		///
		/// ��ȡָ����Ҷ��ڸ���Ʒ���޹���
		///
		/// ��������޹����¼�У��򷵻ص�ǰ�޹����¼���������¼����޹����������޹����¼
		///
		DWORD GetBuyLimit( const CGUID &player_id );
		
		///
		/// ��ȡĳ����ҵ�����޹���
		///
		DWORD GetMaxBuyLimit( const CGUID &player_id );

		void SetBuyLimit( const CGUID &player_id, DWORD dwBuyLimit );

		///
		/// ˢ���޹�������¼����������ӣ�Ч�ʿ��ǣ�
		///
		void RefreshBuyLimit( const CGUID &player_id );

		///
		/// ���ĳ������Ƿ����޹����¼
		///
		bool HasBuyLimitRecord( const CGUID &player_id );

		///
		/// �����޹����������û�м�¼������¼�¼
		///
		DWORD SubBuyLimit( const CGUID &player_id, DWORD dwSub );

		///
		/// ��ȡ˰��ָ�����ٷ��Ʒ���
		///
		void GetTaxScale( CPlayer *pPlayer, int &OrigTaxScale, int &TaxScale );

		///
		/// �������ݿ�浵����
		///
		void EncodeToSave( vector<BYTE> &buf );
		
		///
		/// �������ݿ�浵����
		///
		void DecodeFromDB( DBReadSet &db );

		void AddBuyCount( DWORD add )
		{
			// TODO : remove this invalid function later
		}

		void AddSellCount( DWORD add )
		{
			// TODO : remove this invalid function later
		}

		void SetBaseProperty( const tagGoods &bp );

		const tagGoods &GetBaseProperty() const 
		{
			return m_BaseProperty;
		}

		/// �õ�����Ʒ���ID
		int GetIndex() const
		{
			return m_Index;
		}

		DWORD GetBuyPrice() const
		{
			return m_dwBuyPrice;
		}

		///
		/// ��ȡ����Ʒ�ڴ�ó�׵�ĳ��ۼ۸������ó�׵���ۣ�
		/// �����ó�׵�Ҳ���۴���Ʒ����ô��Ʒ�ĳ��ۼ۸�Ϊ����۸�50%
		///
		DWORD GetSellPrice( bool bLocal = false ) const
		{
			return bLocal ? GetBuyPrice() * 0.5f : m_dwSellPrice;
		}

		int GetPriceState() const
		{
			return m_PriceState;
		}

		int GetVolume() const
		{
			return m_Volume;
		}

		void SetVolume( int v )
		{
			m_Volume = v;
		}

	private:
		void SetPriceState( int sub );
	private:
		tagGoods m_BaseProperty;
		/// ����е�ID
		int m_Index;
		/// ��ǰ���
		int m_Volume;
		/// �۸�����
		int m_PriceState;
		/// ����۸������������룩
		DWORD m_dwBuyPrice;
		/// �����۸���������������
		DWORD m_dwSellPrice;
		/// �ܱ���������
		DWORD m_dwBuyCount;
		/// �ܱ���������
		DWORD m_dwSellCount;
		/// ��������仯ϵ��
		float m_fBuyQuo;
		/// ���������仯ϵ��
		float m_fSellQuo;
		/// ��¼ÿ����Ҷ��ڴ���Ʒ���޹���
		BuyLimitTable m_BuyLimitRecord;
		/// �������(�ܷ�չ�ȱ仯)
		int m_MaxVolume;
		/// ����Ʒ����ó�׵�
		CTradeSpot *m_pFather;
	};
}
#endif