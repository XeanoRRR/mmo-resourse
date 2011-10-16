///
/// @file OnMessage.cpp
/// @brief handle the messages from client.
///
#include "StdAfx.h"
#include "Shop.h"
#include "ShopManager.h"
#include "GoodsHelper.h"
#include "../Goods/CGoodsFactory.h"
#include "../ServerRegion.h"
#include "../Player.h"
#include "../Monster.h"
#include "../Container/CTestContainer.h"
#include "../../../../Public/Common/FBusinessCommon.h"

extern void CancelTrading( CPlayer* pPlayer );

namespace FBusiness
{
	const long BUY_GOODS_FLAG = 191;
	const long SELL_GOODS_FLAG = 192;
	const long NPC_SHOP_STRING = 22;

	/// ���״�����ʾ��ʹ����ԭNPC�̵������ʾ��
	/// @see textresource/GS_NPCShop.xml
	enum ErrorNotifyID
	{
		ENT_NO_SPACE = 3,
		ENT_NO_COIN = 23,
		ENT_FATAL_ERROR = 24,
		ENT_NO_SPACE_FOR_COIN = 25,
		ENT_NO_GOODS_TO_SELL = 26,
	};

	/// fatal error number
	enum FatalErrorNum
	{
		FEN_BUY_DEL_COIN = 1,
		FEN_BUY_ADD_GOODS,
		FEN_SELL_ADD_COIN,
		FEN_SELL_DEL_GOODS,
		FEN_INVALID_OPER,
	};

	//
	// ���״̬����˵���������̵꿪���ɿͻ��˴���NPC�Ի��ű���ʼ���ڽű�����
	// OpenFBusiness�п���(PROGRESS_FBUSINESS)���ڽ��յ��κοͻ��˵Ĺ���/����
	// ��Ϣʱ����(PROGRESS_NONE)��
	//
	static void SetPlayerState( CPlayer *pPlayer, bool bOff = false )
	{
		pPlayer->SetCurrentProgress( bOff ? CPlayer::PROGRESS_NONE : CPlayer::PROGRESS_FBUSINESS );
		pPlayer->SetMoveable( bOff ? TRUE : FALSE );
	}

	static void NotifyFatalError( CPlayer *pPlayer, long err )
	{
		char strNotify[1024];
		_snprintf( strNotify, 1024, AppFrame::GetText("GS_NPCSHOP_42"), err ); 
#ifdef _ENABLE_NOTIFY
		pPlayer->SendNotifyMessage( strNotify );
#else
		Log4c::Error(ROOT_MODULE,"%-15s Fatal error [%d] in Player [%s] 's trading.",__FUNCTION__, err, pPlayer->GetName() );
#endif
	}

	static void NotifyNormalError( CPlayer *pPlayer, long type )
	{
		//pPlayer->SendNotifyMessage( AppFrame::GetText( NPC_SHOP_STRING, type ) );
	}

	static void DelGoodsList( std::vector<CGoods*> &GoodsList, long lFlag )
	{
		for( size_t i = 0; i < GoodsList.size(); ++ i )
		{
			CGoodsFactory::GarbageCollect( &GoodsList[i], lFlag );
		}
		GoodsList.clear();
	}

	static void SavePlayerGoodsData( CPlayer *pPlayer )
	{
		GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRPACKGOODS );										
		GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS1 );					
		GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS2 );						
		GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS3 );					
		GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS4 );					
		GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS5 );
	}

	static int IsValidOper( CShopManager *pShopMgr, CMessage *pMsg, const CGUID &npcID, long lShopID )
	{
		CPlayer *pPlayer = pMsg->GetPlayer();
		CServerRegion *pRegion = pMsg->GetRegion();
		if( pPlayer == NULL || pRegion == NULL )
		{
			return -2;
		}

		if( pPlayer->IsDied() )
		{
			return -1;
		}

		if( pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_FBUSINESS )
		{
			Log4c::Warn(ROOT_MODULE, "The Player [%s] is not in PROGRESS_FBUSINESS state.",pPlayer->GetName() );
			return -1;
		}

		SetPlayerState( pPlayer, true );

		CMonster *pNPC = (CMonster*) pRegion->FindAroundObject( pPlayer, TYPE_MONSTER, npcID );
		if( pNPC == NULL )
		{
			return -1;
		}

		CShop *pShop = pShopMgr->GetShop( lShopID );
		if( pShop == NULL )
		{
			return -1;
		}

		// NPC�Ƿ����̵��Ӧ
		if( strcmp( pNPC->GetOriginName(), pShop->GetNpcOrigName() ) != 0 )
		{
			return -1;
		}

		if( pNPC->Distance( pPlayer ) > 10 )
		{
			return -1;
		}

		return 0;
	}

	void CShopManager::OnMessage( CMessage *pMsg )
	{
		if( !GetInst( CConfiger ).IsEnable() )
		{
			Log4c::Warn(ROOT_MODULE, "Disabled, but received client messages." );
			return ;
		}

		unsigned char type = pMsg->GetByte();
		CGUID npcID;
		long lShopID = 0;
		pMsg->GetGUID( npcID );
		lShopID = pMsg->GetLong();
		// make sure all the params are valid.
		int ret = IsValidOper( this, pMsg, npcID, lShopID ); 
		if( ret < 0 )
		{
			if( ret == -1 )
			{
				CancelTrading( pMsg->GetPlayer() );
				NotifyFatalError( pMsg->GetPlayer(), FEN_INVALID_OPER );
			}
			return ;
		}

		CShop *pShop = GetShop( lShopID );
		switch( type )
		{
		case C2S_PLAYER_BUY:
			pShop->OnPlayerBuyMsg( pMsg, npcID );	
			break;

		case C2S_PLAYER_SELL:
			pShop->OnPlayerSellMsg( pMsg, npcID );
			break;

		case C2S_PRICE_CHANGED_RES:
			pShop->OnPlayerConformSellMsg( pMsg, npcID );
			break;

		case C2S_PLAYER_CLOSE:
			pShop->OnPlayerCloseMsg( pMsg );
			break;

		default:
			CPlayer *pPlayer = pMsg->GetPlayer();
            Log4c::Error(ROOT_MODULE,"%-15s Unknown oper type from client [%s].",__FUNCTION__,pPlayer->GetName() );
			break;
		}
	}

	void CShop::OnPlayerBuyMsg( CMessage *pMsg, const CGUID &npcID )
	{
		struct GoodsInfo
		{
			long lIndex;
			long lAmount;
		};
		std::vector<GoodsInfo> BuyGoodsList;

		CPlayer *pPlayer = pMsg->GetPlayer();
		if( pPlayer == NULL )
		{
			return ;
		}

		long lPrice = 0;
		long lCount = pMsg->GetLong();

		if( lCount <= 0 )
		{
			// invalid operation
			Log4c::Warn(ROOT_MODULE, "Player [%s] no goods to buy, ignore this operation.",pPlayer->GetName() );
			return ;
		}

		// ��ȡ��Ʒ��˳�������ܼ�ֵ��lPrice�������ʾֵ���ٸ����ң������Ǹ���Ʒ��
		for( long i = 0; i < lCount; ++ i )
		{
			GoodsInfo goods;
			goods.lIndex = pMsg->GetLong();
			goods.lAmount = pMsg->GetLong();
			BuyGoodsList.push_back( goods );

			lPrice += ( GoodsHelper::GetPrice( goods.lIndex ) * goods.lAmount );

			// ������Ʒ�Ƿ���NPC�����б���
			if( !IsInSellList( goods.lIndex ) )
			{
				// �Ƿ�����
				Log4c::Warn(ROOT_MODULE, "Player [%s] buy goods [%d] is not in NPC sell list.",pPlayer->GetName(), goods.lIndex );
				return ;
			}
		}
		
		CTestContainer testContainer;
		testContainer.Init( pPlayer );
		
		// ��������Ƿ����㹻�Ļ�����ɴ˴ι���˳��õ��۳����ҵ�ɾ�����
		tagTestArg_ID_Num testArg;
		testArg._dwGoodsID = GetInst( CConfiger ).GetGlobalCfg().lCoinGoodsIndex;
		testArg._dwNum = lPrice;
		std::vector<tagTestArg_ID_Num> delArg;
		delArg.push_back( testArg );
		std::vector<tagDelTestResult> delResult;
		BOOL bDelTestRet = testContainer.DelTest( delArg, &delResult );
		if( !bDelTestRet )
		{
			// ���ҿ��ܲ���
			NotifyNormalError( pPlayer, ENT_NO_COIN );
			return ;
		}

		// ������Ʒ������Ӳ���
		std::vector<CGoods*> GainList;
		for( size_t i = 0; i < BuyGoodsList.size(); ++ i )
		{
			const GoodsInfo &info = BuyGoodsList[i];
			CGoodsFactory::CreateGoods( info.lIndex, info.lAmount, GainList, BUY_GOODS_FLAG ); 
		}

		std::vector<tagTestResult> addResult;
		BOOL bAddTestRet = testContainer.AddTest( GainList, &addResult );
		assert( GainList.size() == addResult.size() );
		if( !bAddTestRet )
		{
			// ���ʧ�ܣ�������Ʒ��Դ
			DelGoodsList( GainList, BUY_GOODS_FLAG );
			NotifyNormalError( pPlayer, ENT_NO_SPACE );	
			return ;
		}

		bool bRet = true;
		long err = 0;

		// �����ϣ��۳�������ϵĻ���
		for( size_t i = 0; i < delResult.size(); ++ i )
		{
			if( !pPlayer->DelGoodsByTest( &delResult[i] ) )
			{
				bRet = false;
				err = FEN_BUY_DEL_COIN;
				break;
			}
		}
		if( bRet )
		{
			// ��ӹ������Ʒ
			for( size_t i = 0; i < addResult.size(); ++ i )
			{
				DWORD dwGoodsIndex=GainList[i]->GetBasePropertiesIndex();
				CGUID guid=GainList[i]->GetExID();
				DWORD dwAmount=GainList[i]->GetAmount();
				string strGoodsName = GainList[i]->GetName();
				if( !pPlayer->AddGoodsByTest( &addResult[i], GainList[i] ) )
				{
					CGoodsFactory::GarbageCollect( &GainList[i], BUY_GOODS_FLAG );
				}
				else
				{
					//��¼��־
					 GetLogicLogInterface()->logT030_goods_log_npc( pPlayer, dwGoodsIndex,
						guid,strGoodsName.c_str(),0,GoodsHelper::GetPrice(dwGoodsIndex),dwAmount );
				}
			}
		}
		if( !bRet )
		{
			// fatal error
			DelGoodsList( GainList, BUY_GOODS_FLAG );
			NotifyFatalError( pPlayer, err );
		}
		else
		{
			SavePlayerGoodsData( pPlayer );
		}
	}

	void CShop::DoPlayerSell( CPlayer *pPlayer, const CCachedSellData::SellGoodsListT &sell_list )
	{
		long lPrice = 0;
		std::vector<tagTestArg_ID_Num> delArg;
		// ����۸��ܺͣ����ɾ�����Բ���
		for( size_t i = 0; i < sell_list.size(); ++ i )
		{
			const CCachedSellData::GoodsInfo &info = sell_list[i];
			lPrice += ( info.amount * info.price );

			tagTestArg_ID_Num testDel;
			testDel._dwGoodsID = info.index;
			testDel._dwNum = info.amount;
			delArg.push_back( testDel );
		}

		// ��������
		std::vector<CGoods*> GainList;
		CGoodsFactory::CreateGoods( GetInst( CConfiger ).GetGlobalCfg().lCoinGoodsIndex, lPrice, GainList, SELL_GOODS_FLAG );

		CTestContainer testContainer;
		testContainer.Init( pPlayer );

		// ������Ӳ���
		std::vector<tagTestResult> addResult;
		BOOL bAddTestRet = testContainer.AddTest( GainList, &addResult );
		assert( addResult.size() == GainList.size() );
		if( !bAddTestRet )
		{
			// ���ʧ��
			NotifyNormalError( pPlayer, ENT_NO_SPACE_FOR_COIN );
			DelGoodsList( GainList, SELL_GOODS_FLAG );
			return ;
		}

		// ����ɾ������
		std::vector<tagDelTestResult> delResult;
		BOOL bDelTestRet = testContainer.DelTest( delArg, &delResult );
		if( !bDelTestRet )
		{
			// ɾ��ʧ��
			NotifyNormalError( pPlayer, ENT_NO_GOODS_TO_SELL );
			DelGoodsList( GainList, SELL_GOODS_FLAG );
			return;
		}

		// ������ϣ�ɾ����ҳ��۵���Ʒ��Ϊ������ӻ�õĻ�����Ʒ
		bool bRet = true;
		long err = 0;
		for( size_t i = 0; i < delResult.size(); ++ i )
		{
			CGoods *pGoods = pPlayer->FindGoods( delResult[i].dwContainerId, delResult[i].dwPos );
			if( pGoods != NULL )
			{
				// ɾ����Ʒ��־
				 GetLogicLogInterface()->logT030_goods_log_npc( pPlayer, pGoods->GetBasePropertiesIndex(), 
					pGoods->GetExID(), pGoods->GetName(), 1, GetBuyPrice( pGoods->GetBasePropertiesIndex() ), 
					delResult[i].dwNum );
			}

			if( !pPlayer->DelGoodsByTest( &delResult[i] ) )
			{
				bRet = false;
				err = FEN_SELL_DEL_GOODS;
				break;
			}
		}
		if( bRet )
		{
			// ��ӻ�����Ʒ
			for( size_t i = 0; i < addResult.size(); ++ i )
			{
				DWORD dwGoodsIndex=GainList[i]->GetBasePropertiesIndex();
				CGUID guid=GainList[i]->GetExID();
				DWORD dwAmount=GainList[i]->GetAmount();
				string strGoodsName = GainList[i]->GetName();
				if( !pPlayer->AddGoodsByTest( &addResult[i], GainList[i] ) )
				{
					CGoodsFactory::GarbageCollect( &GainList[i], SELL_GOODS_FLAG );	
				}
				else
				{
					// ���۳ɹ���ȡ������־
					 GetLogicLogInterface()->logT030_goods_log_npc( pPlayer, dwGoodsIndex,
						guid, strGoodsName.c_str(), 1, 0,dwAmount);
				}
			}
		}
		if( !bRet )
		{
			DelGoodsList( GainList, SELL_GOODS_FLAG );
			NotifyFatalError( pPlayer, err );
			return ;
		}
		else
		{
			SavePlayerGoodsData( pPlayer );
		}
	}

	void CShop::OnPlayerSellMsg( CMessage *pMsg, const CGUID &npcID )
	{
		CPlayer *pPlayer = pMsg->GetPlayer();
		if( pPlayer == NULL )
		{
			return ;
		}
		CCachedSellData::SellGoodsListT goods_list;
		long lCount = pMsg->GetLong();
		bool bPriceChanged = false;

		if( lCount <= 0 )
		{
			// invalid operation
			Log4c::Warn(ROOT_MODULE, "Player [%s] No goods to sell, ignore this operation.", pPlayer->GetName() );
			return ;
		}

		// ȡ��������Ʒ��Ϣ���ж��չ��۸��Ƿ�䶯
		for( long i = 0; i < lCount; ++ i )
		{
			// ȡ����Ʒ
			CCachedSellData::GoodsInfo info;
			info.index = pMsg->GetLong();
			info.amount = pMsg->GetLong();
			info.price = pMsg->GetLong();

			goods_list.push_back( info );
			
			// �۸��Ƿ�䶯
			if( !bPriceChanged )
			{
				bPriceChanged = IsPriceChanged( info.index, info.price );
			}
		}

		// �۸��Ѿ��仯������������ݣ��ȴ����ȷ��
		if( bPriceChanged )
		{
			CCachedSellData::SellGoodsListT *cachedSellList = m_CachedSellData.New( pPlayer->GetExID() );
			for( size_t i = 0; i < goods_list.size(); ++ i )
			{
				CCachedSellData::GoodsInfo info = goods_list[i];
				info.price = GetBuyPrice( info.index );
				cachedSellList->push_back( info );
			}

			SetPlayerState( pPlayer, false );

			CMessage msg( MSG_S2C_FBUSINESS_OPER );
			msg.Add( (BYTE) S2C_PRICE_CHANGED );
			msg.Add( npcID );
			msg.Add( m_lID );
			msg.SendToPlayer( pPlayer->GetExID(), false );
			return ;
		}

		DoPlayerSell( pPlayer, goods_list );	
	}

	void CShop::OnPlayerConformSellMsg( CMessage *pMsg, const CGUID &npcID )
	{
		CPlayer *pPlayer = pMsg->GetPlayer();
		if( pPlayer == NULL )
		{
			return ;
		}

		unsigned char ret = pMsg->GetByte();
		if( ret == 0 )
		{
			// ȡ�����۲���
			m_CachedSellData.Remove( pPlayer->GetExID() );
			// ���´��չ�����
			GetInst( CShopManager ).OpenShopForSell( m_lID, pPlayer->GetExID(), npcID );
			return ;
		}

		// ȷ�ϳ���
		const CCachedSellData::SellGoodsListT *sell_list = m_CachedSellData.Get( pPlayer->GetExID() );
		if( sell_list == NULL )
		{
            Log4c::Error(ROOT_MODULE,"%-15s No cached sell data for player [%s].",__FUNCTION__,pPlayer->GetName());
			return;
		}

		DoPlayerSell( pPlayer, *sell_list );
		m_CachedSellData.Remove( pPlayer->GetExID() );
	}

	void CShop::OnPlayerCloseMsg( CMessage *pMsg )
	{
		CPlayer *pPlayer = pMsg->GetPlayer();
		if( pPlayer == NULL )
		{
			return ;
		}
		const CGUID &playerID = pPlayer->GetExID();
		// remove the cached sell data if there is.
		m_CachedSellData.Remove( playerID );
	}

	void CShop::PlayerClose( CPlayer *pPlayer )
	{
		// ��Ҫȷ������Ƿ����
		if( m_CachedSellData.Get( pPlayer->GetExID() ) == NULL )
		{
			return ;
		}
		m_CachedSellData.Remove( pPlayer->GetExID() );
		CancelTrading( pPlayer );
	}
}
