///
/// @file CTradeSpot.h
/// @brief ó�׵�����������߼�
///
#pragma once

#include "../../../../public/GameEvent.h"
#include "BusinessCommon.h"
#include "BusinessState.h"
#include <map>
#include <string>
#include <vector>

namespace Business
{
	class CBusinessGoods;

	class CTradeSpot : public BaseMemObj, public CGameEvent
	{
	public:
		/// [���ID-BusinessGoods*]
		typedef std::map<DWORD, CBusinessGoods*> GoodsTemplateTable;
		/// ���ID
		typedef std::list<DWORD> GoodsIndexList;
		/// ��Ʒ����[���-������б�]
		typedef std::map<int, GoodsIndexList> GoodsCategoryTable;
		/// ����Ͷ�ʶ��б�[����ID-Ͷ�ʶ�]
		typedef std::map<BYTE, DWORD> CountryInvestTable;
	public:
		CTradeSpot() 
		{
			memset( &m_Property, 0, sizeof( m_Property ) );
			m_lPriceTimer = 0;
			m_StateGroup.SetTradeSpot( this );
		}

		CTradeSpot( const tagTradeSpotProperty &p ) : m_Property( p )
		{
			m_lPriceTimer = 0;
			m_StateGroup.SetTradeSpot( this );
		}

		virtual ~CTradeSpot();

		///
		/// �ͷ���Ʒģ��
		///
		void ReleaseGoodsTable();

		///
		/// �����۸񲨶���ʱ��
		///
		bool StartPriceTimer();
		void StopPriceTimer();

		///
		/// Decode goods template list and the base trade spot property.
		///
		void DecodeFromDataBlock( DBReadSet &db );

		///
		/// ������Ʒ����ȡ��Ʒ�б�
		///
		void GetGoodsList( CPlayer *pPlayer, DBWriteSet &db );

		///
		/// ������Ʒ����ȡ��ұ�����Ʒ�۸�
		///
		void GetGoodsSellPrice( CPlayer *pPlayer, DBWriteSet &db );

		///
		/// ��ȡ���ָ����ѯ��������
		///
		void GetGoodsChecking( CPlayer *pPlayer, DBWriteSet &db );

		///
		/// ��������е�ID������Ʒ
		///
		CBusinessGoods *GetGoods( int id );

		///
		/// ������Ʒ������ļ��ٽ�ͬ������ͬ�༰��������Ʒ�Ŀ��
		/// 
		/// @param dwAmount ���仯��
		///
		void DecreaseGoodsVolume( CBusinessGoods *pGoods, DWORD dwAmount );
		void IncreaseGoodsVolume( CBusinessGoods *pGoods, DWORD dwAmount );

		///
		/// �۸񲨶���ʱ���ص������¸�ó�׵���Ʒ�۸�
		///
		void UpdateGoodsPrice();

		///
		/// ����ˢ���޹��ʱ����ʵ��һ��ʱ���ˢ��
		///
		/// ���������ڴ�ó�׵�û���κ��޹����¼���򲻿�����ʱ��
		///
		void StartRefreshBuyLimitTimer( const CGUID &player_id );

		///
		/// ���¶���ĳ�����˶��Ե���Ʒ�޹���
		///
		void RefreshGoodsBuyLimit( const CGUID &player_id );

		/// build the goods category by the goods list.
		void BuildGoodsCategory();

		///
		/// ����ĳ�������ڸ�ó�׵��Ͷ�ʶ�
		///
		/// @param dwAdd ���ӵ�Ͷ�ʶ�
		///
		void AddCountryInvest( BYTE country_id, DWORD dwAdd );

		///
		/// ��ȡĳ�����ҵ�Ͷ���ܶ�
		///
		DWORD GetCountryInvest( BYTE country_id ) const;
		
		///
		/// �������ݿ�浵����
		///
		void EncodeToSave( DBWriteSet &db );

		const GoodsTemplateTable &GetGoodsTemplateTable() const
		{
			return m_GoodsTable;
		}

		void SetProperty( const tagTradeSpotProperty &p )
		{
			m_Property = p;
		}

		const tagTradeSpotProperty &GetProperty() const 
		{
			return m_Property;
		}

		CStateGroup &QueryStateGroup()
		{
			return m_StateGroup;
		}

		///
		void OnMessage( CMessage* ) {}
		long OnTimeOut( DWORD timerid, DWORD curtime, const void *var );
		void OnTimerCancel( DWORD timerid, const void *var );
		long Proc( eEventType type, const tagProcVar *pVar ) { return 0; }

	private:
		///
		/// �ж�ָ����Ʒ�Ƿ񿪷Ÿ������
		///
		bool IsGoodsVisible( CBusinessGoods *pGoods, CPlayer *pPlayer );

		///
		/// �������ݿ�浵���ݵ���Ʒ
		///
		void DecodeGoodsDBProperty( DBReadSet &db );

		///
		/// �������ݿ�浵���ݵ�����Ͷ���ܶ�
		///
		void DecodeCountryInvestDBProperty( DBReadSet &db );

	private:
		/// ó�׵��������
		tagTradeSpotProperty m_Property;
		/// ��Ʒģ���б�
		GoodsTemplateTable m_GoodsTable;
		/// ��Ʒ����
		GoodsCategoryTable m_GoodsCategory;
		/// ��۲�����ʱ��ID
		long m_lPriceTimer;
		/// ����Ͷ�ʶ��б�
		CountryInvestTable m_CountryInvest;
		/// �������̽�����������״̬
		CStateGroup m_StateGroup;
	};

}