///
/// @file CTradeSpot.cpp
/// @brief ó�׵�����������߼�
///
#include "StdAfx.h"
#include "CTradeSpot.h"
#include "../../../../public/BusinessGoodsDef.h"
#include "../../GameServer/GameManager.h"
#include "CBusinessGoods.h"
#include "CBusinessManager.h"
#include "../Player.h"
#include "../Nets/NetServer/Message.h"
#include "../goods/CGoodsFactory.h"

namespace Business
{
	CTradeSpot::~CTradeSpot()
	{
		ReleaseGoodsTable();
	}

	void CTradeSpot::ReleaseGoodsTable()
	{
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			MP_DELETE(it->second);
		}
		m_GoodsTable.clear();
	}

	bool CTradeSpot::StartPriceTimer()
	{
		if( m_GoodsTable.size() == 0 )
		{
			// ����û����Ʒ��ó�׵���ԣ�����Ҫ�˷Ѷ�ʱ����Դ
			return false;
		}
		tagTimerVar* pTimerVar = CBaseObject::CreateTimerVar(39);
		pTimerVar->TimerType = (eChildObjType)TIMER_UPDATE_PRICE;
		m_lPriceTimer = GameManager::GetInstance()->Schedule( this, pTimerVar, timeGetTime(), 
			GetInst( CBusinessManager ).GetSetup().dwUpdatePriceInterval );
		return m_lPriceTimer != 0;
	}

	void CTradeSpot::StopPriceTimer()
	{
		if( m_lPriceTimer != 0 )
		{
			GameManager::GetInstance()->Cancel( m_lPriceTimer, 0 );
			m_lPriceTimer = 0;
		}
	}

	void CTradeSpot::DecodeFromDataBlock( DBReadSet &db )
	{
		long lGoodsSize = db.GetLongFromByteArray();
		for( long i = 0; i < lGoodsSize; ++ i )
		{
			tagGoods goods;
			db.GetBufferFromByteArray( &goods, sizeof( goods ) );
			DWORD dwIndex = CGoodsFactory::QueryGoodsIDByOriginalName( goods.strOrigName );
			if( dwIndex == 0 )
			{
				// not exist
				BUSINESS_LOG_WARNING( FMT_STR( "Goods [%s] does NOT exist in goodslist.dat", goods.strOrigName ) );
				continue;
			}
			GoodsTemplateTable::iterator it = m_GoodsTable.find( dwIndex );
			if( it == m_GoodsTable.end() )
			{
				CBusinessGoods *pGoods = MP_NEW CBusinessGoods(goods, this);
				m_GoodsTable.insert( std::make_pair( dwIndex, pGoods ) );
			}
			else
			{
				it->second->SetBaseProperty( goods );
			}
		}

		// goods list raw data (��Ʒ���ݿ�浵����)
		DecodeGoodsDBProperty( db );

		// country invest raw data (����Ͷ���ܶ����ݿ�浵����)
		DecodeCountryInvestDBProperty( db );

		// ������Ʒ�����
		BuildGoodsCategory();
	}

	void CTradeSpot::BuildGoodsCategory()
	{
		m_GoodsCategory.clear();
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			const tagGoods &goods = it->second->GetBaseProperty();
			int category = CGoodsFactory::GetBusinessGoodsType( it->first ); 
			if( category == BGT_NOT_BUSINESS_GOODS )
			{
				BUSINESS_LOG_WARNING( FMT_STR( "Goods [%d] is not business goods.", it->first ) );
			}
			else
			{
				m_GoodsCategory.insert( std::make_pair( category, it->first ) ); 
			}
		}
	}

	void CTradeSpot::AddCountryInvest( BYTE country_id, DWORD dwAdd )
	{
		if( m_CountryInvest.find( country_id ) == m_CountryInvest.end() )
		{
			m_CountryInvest.insert( std::make_pair( country_id, dwAdd ) );
		}
		else
		{
			m_CountryInvest[country_id] += dwAdd;
		}
	}

	DWORD CTradeSpot::GetCountryInvest( BYTE country_id ) const
	{
		CountryInvestTable::const_iterator it = m_CountryInvest.find( country_id );
		if( it != m_CountryInvest.end() )
		{
			return it->second;
		}
		return 0;
	}

	long CTradeSpot::OnTimeOut( DWORD timerid, DWORD curtime, const void *var )
	{
		tagTimerVar *pTimerVar = (tagTimerVar*) var;
		assert( pTimerVar != 0 );
		if( pTimerVar->TimerType == TIMER_UPDATE_PRICE )
		{
			UpdateGoodsPrice();
		}
		else if( pTimerVar->TimerType == TIMER_UPDATE_BUYLIMIT )
		{
			CGUID *player_id = (CGUID*)pTimerVar->pvar;
			if (NULL != player_id)
			{
				if (sizeof(CGUID) == pTimerVar->pvar_size)
				{
					RefreshGoodsBuyLimit( *player_id );
					M_FREE(player_id, pTimerVar->pvar_size);
					pTimerVar->pvar_size = 0;
				}
				else
					assert(false);
			}
			
			// free the memory in OnTimerCancel;
		}

		return 0;
	}

	void CTradeSpot::OnTimerCancel( DWORD timerid, const void *var )
	{
		if( var != NULL )
		{
			tagTimerVar *pTimervar = (tagTimerVar*)var;
			if( pTimervar->pvar != NULL )
			{
				// delete a void pointer is not safe especially when pvar pointing to some objects.
				// so i cast the pointer here
				if( pTimervar->TimerType == TIMER_UPDATE_BUYLIMIT )
				{
					M_FREE(pTimervar->pvar, pTimervar->pvar_size);
					pTimervar->pvar_size = 0;
				}
			}
			CBaseObject::ReleaseTimerVar(&pTimervar );
		}
	}

	void CTradeSpot::UpdateGoodsPrice()
	{
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			it->second->UpdatePrice();
		}

		dump_spot_state( this );
		m_StateGroup.SendUpdatePriceMsg();
	}

	void CTradeSpot::StartRefreshBuyLimitTimer( const CGUID &player_id )
	{
		// ����Ƿ�����޹����¼
		bool bHasBuyLimitRecord = false;
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			if( it->second->HasBuyLimitRecord( player_id ) )
			{
				bHasBuyLimitRecord = true;
				break;
			}
		}

		if( !bHasBuyLimitRecord )
		{
			return;
		}

		// ׼��������ʱ��
		CGUID *copy_id = (CGUID*)M_ALLOC(sizeof(CGUID));
		*copy_id = player_id;

		tagTimerVar* pTimerVar = CBaseObject::CreateTimerVar(40);
		pTimerVar->TimerType = (eChildObjType)TIMER_UPDATE_BUYLIMIT;
		pTimerVar->pvar = copy_id;
		pTimerVar->pvar_size = sizeof(CGUID);
		GameManager::GetInstance()->Schedule( this, pTimerVar, 
			timeGetTime() + GetInst( CBusinessManager ).GetSetup().dwUpdateBuyLimitInterval, 0 );
	}

	void CTradeSpot::RefreshGoodsBuyLimit( const CGUID &player_id )
	{
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			it->second->RefreshBuyLimit( player_id );
		}
	}

	void CTradeSpot::GetGoodsList( CPlayer *pPlayer, DBWriteSet &db )
	{
		typedef std::vector<tagGoodsClient> GoodsClientList;
		GoodsClientList goods_list;

		// get the goods the player can buy
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			if( IsGoodsVisible( it->second, pPlayer ) )
			{
				CBusinessGoods *pGoods = it->second ;
				tagGoodsClient goods ;
				goods.dwIndex = pGoods->GetIndex() ;
				goods.dwBuyLimit = pGoods->GetBuyLimit( pPlayer->GetExID() );
				goods.dwPrice = m_StateGroup.AdjustPrice( pPlayer,  pGoods->GetBuyPrice() );
				// TODO:calculate the price buy the TAX
				goods.PriceScale = ( goods.dwPrice / (float)pGoods->GetBaseProperty().dwBasePrice ) * 100;

				goods_list.push_back( goods );
			}
		}

		// add the goods list to the message
		db.AddToByteArray( (long) goods_list.size() );
		if( goods_list.size() > 0 )
		{
			db.AddToByteArray( &goods_list[0], goods_list.size() * sizeof( tagGoodsClient ) );
		}
	}

	void CTradeSpot::GetGoodsChecking( CPlayer *pPlayer, DBWriteSet &db )
	{
		typedef std::vector<tagGoodsCheckingClient> GoodsClientList;
		GoodsClientList goods_list;

		// get the goods the player can buy
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			if( IsGoodsVisible( it->second, pPlayer ) )
			{
				CBusinessGoods *pGoods = it->second ;
				tagGoodsCheckingClient goods ;
				goods.dwIndex = pGoods->GetIndex() ;
				goods.dwPrice = pGoods->GetBuyPrice();
				goods.iPriceScale = ( goods.dwPrice / (float)pGoods->GetBaseProperty().dwBasePrice ) * 100;
				goods.iPriceState = pGoods->GetPriceState();
				goods_list.push_back( goods );
			}
		}

		// add the goods list to the message
		db.AddToByteArray( (long) goods_list.size() );
		if( goods_list.size() > 0 )
		{
			db.AddToByteArray( &goods_list[0], goods_list.size() * sizeof( tagGoodsClient ) );
		}

		// ������ϵ���Ʒ��Ϣ
		goods_list.clear();
		CPackage &container = pPlayer->GetBusinessContainer();
		DWORD dwVolume = container.m_pContainer->GetVolume();
		for( DWORD i = 0; i < dwVolume; ++ i )
		{
			CGoods *pGoods = container.m_pContainer->GetGoods( i );
			if( pGoods != NULL )
			{
				DWORD dwIndex = pGoods->GetBasePropertiesIndex();
				CBusinessGoods *pBusinessGoods = GetGoods( dwIndex );
				if( pBusinessGoods != NULL )
				{
					bool bLocal = pBusinessGoods->GetBaseProperty().Local != 0;
					tagGoodsCheckingClient goods;
					goods.dwIndex = dwIndex;
					goods.dwPrice = pBusinessGoods->GetSellPrice( bLocal );
					goods.iPriceScale = ( goods.dwPrice / (float)pBusinessGoods->GetBaseProperty().dwBasePrice ) * 100;
					goods.iPriceState = pBusinessGoods->GetPriceState();
					goods_list.push_back( goods );
				}
			}
		}
		db.AddToByteArray( (long) goods_list.size() );
		if( goods_list.size() > 0 )
		{
			db.AddToByteArray( &goods_list[0], goods_list.size() * sizeof( tagGoodsClient ) );
		}
	}

	void CTradeSpot::GetGoodsSellPrice( CPlayer *pPlayer, DBWriteSet &db )
	{
		typedef std::vector<tagGoodsSellClient> IndexPriceList;
		IndexPriceList price_list;

		CPackage &container = pPlayer->GetBusinessContainer();
		DWORD dwVolume = container.m_pContainer->GetVolume();
		for( DWORD i = 0; i < dwVolume; ++ i )
		{
			CGoods *pGoods = container.m_pContainer->GetGoods( i );
			if( pGoods != NULL )
			{
				DWORD dwIndex = pGoods->GetBasePropertiesIndex();
				CBusinessGoods *pBusinessGoods = GetGoods( dwIndex );
				if( pBusinessGoods != NULL )
				{
					bool bLocal = pBusinessGoods->GetBaseProperty().Local != 0;
					tagGoodsSellClient ip = { dwIndex, pBusinessGoods->GetSellPrice( bLocal ) };
					price_list.push_back( ip );
				}
			}
		}

		//
		db.AddToByteArray( (long) price_list.size() );
		if( price_list.size() > 0 )
		{
			db.AddToByteArray( &price_list[0], price_list.size() * sizeof( tagGoodsSellClient ) );
		}
	}

	CBusinessGoods *CTradeSpot::GetGoods( int id )
	{
		GoodsTemplateTable::iterator it = m_GoodsTable.find( id );
		if( it != m_GoodsTable.end() )
		{
			return it->second ;
		}
		BUSINESS_LOG_WARNING( FMT_STR( "Cannot find the goods by id [%d]", id ) );
		return NULL;
	}

	void CTradeSpot::DecreaseGoodsVolume( CBusinessGoods *pGoods, DWORD dwAmount )
	{
		const tagGoods &goods_property = pGoods->GetBaseProperty();
		int category = CGoodsFactory::GetBusinessGoodsType( pGoods->GetIndex() );
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			CBusinessGoods *pOtherGoods = it->second ;
			if( category == CGoodsFactory::GetBusinessGoodsType( pOtherGoods->GetIndex() ) )
			{
				// ��ͬ����Ʒ����Ӱ��
				int sub = ( 1.0f * goods_property.dwBasePrice / pOtherGoods->GetBaseProperty().dwBasePrice ) * dwAmount *
					m_Property.fCategoryVolumeQuo ;
				pOtherGoods->SetVolume( pOtherGoods->GetVolume() - sub );
			}
			else
			{
				// �Բ�ͬ����Ʒ����Ӱ��
				int sub = ( 1.0f * goods_property.dwBasePrice / pOtherGoods->GetBaseProperty().dwBasePrice ) * dwAmount *
					m_Property.fAllVolumeQuo ;
				pOtherGoods->SetVolume( pOtherGoods->GetVolume() - sub );
			}
		}
	}

	void CTradeSpot::IncreaseGoodsVolume( CBusinessGoods *pGoods, DWORD dwAmount )
	{
		const tagGoods &goods_property = pGoods->GetBaseProperty();
		int category = CGoodsFactory::GetBusinessGoodsType( pGoods->GetIndex() );
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			CBusinessGoods *pOtherGoods = it->second ;
			if( category == CGoodsFactory::GetBusinessGoodsType( pOtherGoods->GetIndex() ) )
			{
				// ��ͬ����Ʒ����Ӱ��
				int add = ( 1.0f * goods_property.dwBasePrice / pOtherGoods->GetBaseProperty().dwBasePrice ) * dwAmount *
					m_Property.fCategoryVolumeQuo ;
				pOtherGoods->SetVolume( pOtherGoods->GetVolume() + add );
			}
			else
			{
				// �Բ�ͬ����Ʒ����Ӱ��
				int add = ( 1.0f * goods_property.dwBasePrice / pOtherGoods->GetBaseProperty().dwBasePrice ) * dwAmount *
					m_Property.fAllVolumeQuo ;
				pOtherGoods->SetVolume( pOtherGoods->GetVolume() + add );
			}
		}
	}

	void CTradeSpot::EncodeToSave( DBWriteSet &db )
	{
		// id
		db.AddToByteArray( (long) m_Property.Id );

		vector<BYTE> tmpBuf;
		// goods list
		if( m_GoodsTable.size() > 0 )
		{
			_AddToByteArray( &tmpBuf, (long) m_GoodsTable.size() );
		}
		for( GoodsTemplateTable::iterator it = m_GoodsTable.begin(); it != m_GoodsTable.end(); ++ it )
		{
			it->second->EncodeToSave( tmpBuf );
		}
		db.AddToByteArray( (long) tmpBuf.size() );
		if( tmpBuf.size() > 0 )
		{
			db.AddToByteArray( &tmpBuf[0], tmpBuf.size() );
		}

		// country invest list
		tmpBuf.clear();
		if( m_CountryInvest.size() > 0 )
		{
			_AddToByteArray( &tmpBuf, (long) m_CountryInvest.size() );
		}
		for( CountryInvestTable::iterator it = m_CountryInvest.begin(); it != m_CountryInvest.end(); ++ it )
		{
			_AddToByteArray( &tmpBuf, (BYTE) it->first );
			_AddToByteArray( &tmpBuf, (DWORD) it->second );
		}
		db.AddToByteArray( (long) tmpBuf.size() );
		if( tmpBuf.size() > 0 )
		{
			db.AddToByteArray( &tmpBuf[0], tmpBuf.size() );
		}
	}

	bool CTradeSpot::IsGoodsVisible( CBusinessGoods *pGoods, CPlayer *pPlayer )
	{
		const tagGoods &property = pGoods->GetBaseProperty();
		if( property.Local == 0 )
		{
			return false;
		}
		// ��ȡ����ڴ�ó�׵����ڵ�ó�����������ֵ
		DWORD dwCredit = pPlayer->GetCredit( m_Property.areaId );

		// �ڴ�ó�׵�Ͷ�ʶ�
		DWORD dwInvest = pPlayer->GetInvestRecord( m_Property.Id );

		// ����Ͷ���ܶ�
		DWORD dwCountryInvest = GetCountryInvest( pPlayer->GetCountry() );

		// TODO:�жϷ�չ��

		/*
		1.������
		2.����Ͷ���ܶ�
		3.����Ͷ�ʶ�
		4.��չ��
		*/
		if( dwCredit >= property.dwCredit && 	
			dwCountryInvest >= property.dwCountryInvest &&
			dwInvest >= property.dwPersonalInvest &&
			/* ��չ�� */ 1 )
		{
			return true;
		}

		return false;
	}

	void CTradeSpot::DecodeGoodsDBProperty( DBReadSet &db )
	{
		long buf_size = db.GetLongFromByteArray();
		if( buf_size > 0 )
		{
			// goods list count
			long goods_count = db.GetLongFromByteArray();
			for( long i = 0; i < goods_count; ++ i )
			{
				// goods index 
				long goods_index = db.GetLongFromByteArray();
				// find the goods
				CBusinessGoods *pGoods = GetGoods( goods_index );
				if( pGoods != NULL )
				{
					// update its property
					pGoods->DecodeFromDB( db );
				}
				else
				{
					BUSINESS_LOG_WARNING( FMT_STR( "Load invalid goods [%d] property from database", 
						goods_index ) );
				}
			} // for
		} // if
	}

	void CTradeSpot::DecodeCountryInvestDBProperty( DBReadSet &db )
	{
		long buf_size = db.GetLongFromByteArray();
		if( buf_size > 0 )
		{
			long country_count = db.GetLongFromByteArray();
			for( long i = 0; i < country_count; ++ i )
			{
				// country id
				BYTE country_id = db.GetByteFromByteArray();
				// invest value
				DWORD invest_value = db.GetDwordFromByteArray();

				// update the property
				m_CountryInvest[country_id] = invest_value;
			}
		}
	}
}
