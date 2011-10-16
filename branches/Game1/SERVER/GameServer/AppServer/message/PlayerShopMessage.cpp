#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"
#include "..\player.h"
#include "..\ServerRegion.h"
#include "..\Goods\CGoods.h"
#include "..\Session\CSession.h"
#include "..\Session\CSessionFactory.h"
#include "..\Session\CPersonalShopSeller.h"
#include "..\Session\CPersonalShopBuyer.h"
#include "..\..\..\..\Public\WordsFilter.h"
#include "..\goods\CGoodsFactory.h"
#include "..\container\CGoodsShadowContainer.h"
#include "../../../../public/StrUnifyInput.h"
#include "..\..\..\setup\IncrementShopList.h"
#include "..\GoodsExManage.h"


void OnPlayerShopMessage( CMessage* pMsg)
{
	if( pMsg == NULL ) return;

	CPlayer* pPlayer = pMsg->GetPlayer();
	if( pPlayer == NULL ) return;

	

	switch( pMsg->GetType() )
	{
		//##���󿪵�
	case MSG_C2S_PLAYERSHOP_CREATE:
		{			
			if( pPlayer->IsDied() ) return;

			if( pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_NONE )
			{
				//##������Ϣ����ǰ���.
				pPlayer->SendNotifyMessage( CStringReading::LoadString(21,3) );
				return;
			}

			if(pMsg->GetRegion() == NULL)	return;


			//�ж��Ƿ��ڰ�ȫ��
			if( pMsg->GetRegion()->GetSecurity( pPlayer->GetTileX(), pPlayer->GetTileY() ) != 
				CRegion::SECURTIY_SAFE )
			{
				pPlayer->SendNotifyMessage( CStringReading::LoadString(21,4) );
				return;
			}

			const CGUID& SessionID		= CSessionFactory::CreateSession( 1, 20 );
			CSession* pSession	= const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession && pSession->Start() )
			{
				long SellerPlugID	= CSessionFactory::CreatePlug( CSessionFactory::PT_PERSONAL_SHOP_SELLER, TYPE_PLAYER, pPlayer->GetExID() );
				if( CSessionFactory::InsertPlug(SessionID, SellerPlugID) )
				{
					//pPlayer->SetCurrentProgress( CPlayer::PROGRESS_OPEN_STALL );
					pPlayer->SetMoveable(FALSE);

					pPlayer->GetPlayerDepot()->OnLockRequest(NULL);

					CMessage msg( MSG_S2C_PALYERSHOP_CREATE );
					msg.Add( SessionID );
					msg.Add( SellerPlugID );

					msg.SendToPlayer( pPlayer->GetExID() );

					pPlayer->SetPersonalShopFlag(SessionID, SellerPlugID);
				}
			}

		}
		break;
	case MSG_C2S_PLAYERSHOP_GOODSPRICE:
		{				
		}
		break;
	case MSG_C2S_PLAYERSHOP_OPENFORBUSINESS://�������ʼ��̯
		{
			pPlayer->SetCurrentProgress( CPlayer::PROGRESS_OPEN_STALL );
			pPlayer->ProduceEvent(ET_OpenShop);
			pPlayer->ProduceEvent(ET_EventBreak);
			CGUID SessionID;
			DWORD dwGoodsAmount=0;
			pMsg->GetGUID(SessionID);				//Session ID
			long PlugID = pMsg->GetLong();		//�Ự�� ID
			LONG lNum=pMsg->GetLong();			//������ƷҪ��̯

			//�����̵���ϼ���λ����Ϊ15��
			if(lNum<=0 || lNum>15)
				return;
			CGoods* pGoods=NULL;
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			CPersonalShopSeller* pSellerPlug=NULL;

			if( pSession )
			{
				pSellerPlug = dynamic_cast<CPersonalShopSeller*>( pSession->QueryPlugByID(PlugID) );
				if( pSellerPlug && pSellerPlug->GetOwnerID() == pPlayer->GetExID() )
				{
					if( pSellerPlug->DoesShopOpenedForBusiness() == FALSE )
					{	
						pPlayer->ClearPersonalShopGoodsData();
						for(int i=0;i<lNum;i++)
						{
							WORD wNum=pMsg->GetWord();//Ӱ����ٸ�����
							if(wNum==0 || wNum>50)
							{
								pSellerPlug->ClearData();
								return;
							}
							LONG lGoodsId=0,lContainerId=0,lPos=0;	
							DWORD dwNum=0,dwPrice=0;
							WORD wPriceType=0;
							DWORD dwSellType = 0;
							DWORD dwGroupNum = 0;
							DWORD dwOneGroupCnt = 0;

							lGoodsId=pMsg->GetLong();//��Ʒ���
							CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(lGoodsId);
							if(!pProperty)
							{
								pSellerPlug->ClearData();
								return;
							}

							//�����Ʒ�ǵ�����Ʒ,���ܴ����Ѿ����õļ۸�
							if(pProperty->GetAddonPropertyValue(GAP_MAXOVERLAP,1)>1)
							{
								if(pSellerPlug->GoodsPriceExist(lGoodsId))
								{
									pSellerPlug->ClearData();
									return;
								}
							}							

							dwSellType = pMsg->GetDWord();				//��Ʒ���۵ķ�ʽ����Ϊ���֣�0������������1�����鷷����
							//������ʽֻ������Ϊ0��1
							if ( dwSellType != 0 && dwSellType != 1 )
							{
								pSellerPlug->ClearData();
								return;
							}
							dwOneGroupCnt = pMsg->GetDWord();	//��Ʒ�ĵ������( ��������ʱ�趨Ϊ����1 )
							dwGroupNum = pMsg->GetDWord();		//��Ʒ������
							//!������۷�ʽΪ �����������жϵ�������趨�Ƿ���ȷ
							//if ( dwSellType == CPersonalShopSeller::ST_SINGLE_SELL )
							//{
							//	if ( dwOneGroupCnt != 1 )
							//	{
							//		pSellerPlug->ClearData();
							//		return;
							//	}
							//}

							//��Ʒ�����ж�
							DWORD dwAllGoodsAmount = pPlayer->GetGoodsAmountByOriginName( CGoodsFactory::QueryGoodsOriginalName( lGoodsId ) );
							//�ϼ���Ʒ����ӦС�����ӵ������
							if(  dwAllGoodsAmount < dwGroupNum * dwOneGroupCnt )
							{
								pSellerPlug->ClearData();
								return;
							}

							wPriceType = pMsg->GetWord();		//��Ʒ���۵Ļ������ͣ���Ϊ���֣�0����ң�1��λ�澫����
							//��������ֻ������Ϊ0��1
							if ( wPriceType != 0 && wPriceType != 1 )
							{
								pSellerPlug->ClearData();
								return;
							}

							//������λ�澫���Ļ�������Ϊλ�澫��
							if ( (lGoodsId == CGlobeSetup::GetSetup()->dwWeimianGoodsId) && (wPriceType == 1) )
							{
								pSellerPlug->ClearData();
								return;
							}
							dwPrice=pMsg->GetDWord();//�۸�
							if ( dwPrice == 0 )
							{
								pSellerPlug->ClearData();
								return;
							}

#ifdef _DEBUG
							PutLogInfo("\r\n=============================== Start Save Goods Info ===============================\r\n");

							SYSTEMTIME stTime;
							GetLocalTime(&stTime);
							char str[256];
							_snprintf(str, 256, "%04d-%02d-%02d %02d:%02d:%02d\r\n\r\n", 
								stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
							PutLogInfo(str);
							_snprintf(str, 256, "Ӱ����ӣ�%d��ƷID��--%d������ʽ��--%d�۸����ͣ�--%d�۸�--%d������--%dÿ�������--%d\r\n\r\n", 
								wNum, lGoodsId, dwSellType, wPriceType, dwPrice, dwGroupNum,dwOneGroupCnt);
							PutLogInfo(str);
							PutLogInfo("\r\n");
							PutLogInfo("\r\n=============================== End Save Goods Info ===============================\r\n");
#endif
							
							CGoodsShadowContainer::tagGoodsShadow* pTagGoodsShadow=NULL;
							for(WORD j=0;j<wNum;j++)
							{								
								lContainerId=pMsg->GetLong();
								lPos=pMsg->GetLong();
								dwNum=pMsg->GetDWord();//��Ʒ����

#ifdef _DEBUG
								PutLogInfo("\r\n=============================== Start Save Every Goods Info ===============================\r\n");
								char str[128];
								_snprintf(str, 128, "����ID��%d����λ�ã�--%d������--%d\r\n\r\n", 
									lContainerId, lPos, dwNum);
								PutLogInfo(str);
								PutLogInfo("\r\n");
								PutLogInfo("================================ End Save Every Goods Info ================================\r\n");
#endif

								dwGoodsAmount+=dwNum;
								
								pGoods=pPlayer->FindGoods(lContainerId,lPos);
								//�Ҳ�������Ʒ
								if(!pGoods)
								{
									pSellerPlug->ClearData();
									char* strRes=CStringReading::LoadString(21,9);
									if(strRes)
										pPlayer->SendNotifyMessage(strRes);
									return;
								}
								//��ƷIDƥ����ж�
								if(pGoods->GetBasePropertiesIndex()!=lGoodsId)
								{
									pSellerPlug->ClearData();
									return;
								}
								//������Ʒ���ܰ�̯
								if(GoodsExManage::GetInstance().IsFrost(pGoods))
									return;
								//����Ʒ���ܰ�̯
								if(GoodsExManage::GetInstance().IsBind(pGoods))
									return;
								//��Ʒ��������
								if(pGoods->GetAmount()<dwNum || dwNum==0)
								{
									pSellerPlug->ClearData();
									return;
								}
								//��ֹͬһλ����Ʒ�ظ����
								if(pPlayer->IsInPersonalShop(pGoods))
								{
									pSellerPlug->ClearData();
									return;
								}

								if( ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
									CGoodsBaseProperties::PA_CANNOT_TRADE_AND_STORE ) == CGoodsBaseProperties::PA_CANNOT_TRADE_AND_STORE )
								{
									//
									pSellerPlug->ClearData();
									char* strRes=CStringReading::LoadString(21,12);
									if(strRes)
										pPlayer->SendNotifyMessage(strRes);
									return;
								}

								pPlayer->AddPersonalShopGoodsData(pGoods);
								
								CGoodsShadowContainer::tagGoodsShadow tShadow;
								tShadow.lOriginalContainerType=400;
								tShadow.OriginalContainerID=pPlayer->GetExID();
								tShadow.lOriginalContainerExtendID=lContainerId;
								tShadow.dwOriginalGoodsPosition=lPos;
								tShadow.dwGoodsAmount=dwNum;
								tShadow.guGoodsID=pGoods->GetExID();
								tShadow.dwGoodsBasePropertiesIndex=lGoodsId;
								pSellerPlug->AddShadowTag(&tShadow);
								
								//�����ϼ���Ʒ�۸���Ϣ
								if ( !pSellerPlug->SetGoodsPrice(pGoods->GetExID(),pGoods->GetBasePropertiesIndex(),
									dwSellType,wPriceType,dwGroupNum,dwOneGroupCnt,dwPrice) )
								{
									return;
								}
								
							}								
						}
						if(pSellerPlug->OpenForBusiness())
						{							
							CMessage msg( MSG_S2C_PLAYERSHOP_OPENFORBUSINESS );
							msg.Add( pPlayer->GetExID() );
							msg.Add( SessionID );
							msg.Add( PlugID );
							msg.SendToAround( pPlayer );

							//����Χ��ҷ����̵�������Ϣ
							CMessage msgName( MSG_S2C_PLAYERSHOP_MSGBOARDGET );
							msgName.Add( SessionID );
							msgName.Add( pPlayer->GetExID() );
							msgName.Add( (BYTE)1 );
							if ( strcmp( pSellerPlug->GetShopName() , "")  != 0 )
							{
								msgName.Add( pSellerPlug->GetShopName() );
							}
							else
							{
								//Ĭ�ϵ��̵�����Ϊ�������̵ꡱ
								msgName.Add( CStringReading::LoadString(21,18) );
							}
							msgName.SendToAround( pPlayer );

							char strFile[64];
							_snprintf(strFile, 64, "scripts/game/open.script");
							stRunScript st;
							st.pszFileName = strFile;
							st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
							st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
							st.desShape = NULL;
							st.guUsedItemID = CGUID::GUID_INVALID;
							RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));

						}
					}
				}
			}			
		}
		break;
	case MSG_C2S_PLAYERSHOP_CLOSEDOWN:
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long PlugID = pMsg->GetLong();

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession )
			{
				CPersonalShopSeller* pSellerPlug = dynamic_cast<CPersonalShopSeller*>( pSession->QueryPlugByID(PlugID) );
				if( pSellerPlug && pSellerPlug->GetOwnerID() == pPlayer->GetExID() )
				{
					if( pSellerPlug->DoesShopOpenedForBusiness() )
					{
						if( pSellerPlug->CloseDown() )
						{
							pPlayer->ClearPersonalShopGoodsData();
							pSession->PlugExit(PlugID);
							CMessage msg( MSG_S2C_PLAYERSHOP_CLOSEDOWN );
							msg.Add( pPlayer->GetExID() );
							msg.Add( SessionID );
							
							msg.SendToAround( pPlayer );

							char strFile[64];
							_snprintf(strFile, 64, "scripts/game/close.script");
							stRunScript st;
							st.pszFileName = strFile;
							st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
							st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
							st.desShape = NULL;
							st.guUsedItemID = CGUID::GUID_INVALID;
							RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));
						}
					}
				}
			}
		}
		break;

	case MSG_C2S_PLAYERSHOP_LOOKGOODS:
		{
			if( pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_NONE ) 
			{
				return;
			}

			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			CGUID SellerID;
			pMsg->GetGUID(SellerID);

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession && pSession->IsSessionAvailable() )
			{
				long BuyerPlugID = CSessionFactory::CreatePlug( 
					CSessionFactory::PT_PERSONAL_SHOP_BUYER, pPlayer->GetType(), pPlayer->GetExID() );

				if( CSessionFactory::InsertPlug(SessionID, BuyerPlugID) )
				{
					pPlayer->SetCurrentProgress( CPlayer::PROGRESS_SHOPPING );
					CPersonalShopBuyer* pBuyerPlug = dynamic_cast<CPersonalShopBuyer*>( pSession->QueryPlugByID(BuyerPlugID) );
					if( pBuyerPlug )
					{
						CPersonalShopSeller* pSellerPlug = pBuyerPlug->GetSeller();
						if( pSellerPlug )
						{
							vector<BYTE> vOut;
							pSellerPlug->GetGoodsList( vOut );
							if(vOut.size())
							{
								CMessage msg( MSG_S2C_PLAYERSHOP_LOOKGOODS );							
								msg.Add( SessionID );
								msg.Add(SellerID);
								msg.Add( BuyerPlugID );
								msg.AddEx( &vOut[0], static_cast<LONG>( sizeof(BYTE) * vOut.size() ) );
								msg.SendToPlayer( pPlayer->GetExID() );
							}

							//��Ҵ��̵�鿴��Ʒʱ��ͬʱ�������԰���Ϣ
							CMessage msg( MSG_S2C_PLAYERSHOP_MSGBOARDGET );
							BYTE bMsgType = 0;
							vector<string>& vecTradeInfo = pSellerPlug->GetTradeInfo();

							msg.Add( SessionID );
							msg.Add( pSellerPlug->GetOwnerID() );
							//����ڲ鿴�̵�ʱ���������û�������̵����ƣ�����ʾΪĬ�ϡ��˴�����Ϣ���һ������
							bMsgType = bMsgType | 0x01;
							if ( strcmp( pSellerPlug->GetShopInfo() , "") != 0  )
							{
								bMsgType = bMsgType | 0x02;
							}
							if ( !vecTradeInfo.empty() )
							{
								bMsgType = bMsgType | 0x04;
							}
							msg.Add( bMsgType );
							if ( strcmp( pSellerPlug->GetShopName() , "")  != 0 )
							{
								msg.Add( pSellerPlug->GetShopName() );
							}
							else
							{
								msg.Add( CStringReading::LoadString(21,18) );
							}
							if ( strcmp( pSellerPlug->GetShopInfo() , "") != 0 )
							{
								msg.Add( pSellerPlug->GetShopInfo() );
							}
							if ( !vecTradeInfo.empty() )
							{
								msg.Add( (BYTE)vecTradeInfo.size() );
								for ( int i = 0; i < vecTradeInfo.size(); ++i )
								{
									msg.Add( vecTradeInfo.at(i).c_str() );
								}
							}
							msg.SendToPlayer( pPlayer->GetExID() );
						}
					}
				}
				else
				{
					pPlayer->SendNotifyMessage( CStringReading::LoadString(21,6) );
					
				}
			}
		}
		break;

	case MSG_C2S_PLAYERSHOP_QUIT:
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long PlugID = pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession && pSession->IsSessionAvailable() )
			{
				CPlug* pPlug = pSession->QueryPlugByID( PlugID );
				if( pPlug && pPlug->GetOwnerID() == pPlayer->GetExID() )
				{
					pPlug->Exit();
				}
			}
		}
		break;

	case MSG_C2S_PLAYERSHOP_CLOSE:
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long PlugID = pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession && pSession->IsSessionAvailable() )
			{
				CPersonalShopSeller* pSellerPlug = dynamic_cast<CPersonalShopSeller*>( pSession->QueryPlugByID(PlugID) );
				if( pSellerPlug && pSellerPlug->GetOwnerID() == pPlayer->GetExID() )
				{
					pSession->End();
					pPlayer->SetCurrentProgress( CPlayer::PROGRESS_NONE );
					pPlayer->SetMoveable();

					char strFile[64];
					_snprintf(strFile, 64, "scripts/game/close.script");
					stRunScript st;
					st.pszFileName = strFile;
					st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
					st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
					st.desShape = NULL;
					st.guUsedItemID = CGUID::GUID_INVALID;
					RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));
				}
			}

			pPlayer->SetPersonalShopFlag(NULL_GUID, 0);
		}
		break;
	case MSG_C2S_PLAYERSHOP_REMOVE:
		{			
			LONG lGoodsId=pMsg->GetLong();
			LONG lSourceContainerType				= pMsg->GetLong();
			CGUID SourceContainerId;
			pMsg->GetGUID(SourceContainerId);

			LONG lSourceContainerExtendID			= pMsg->GetLong();
			LONG lSourceContainerPosition			= pMsg->GetLong();
			LONG lDestinationContainerType			= pMsg->GetLong();

			CGUID DestinationContainerID;
			pMsg->GetGUID(DestinationContainerID);
			LONG lDestinationContainerExtendID		= pMsg->GetLong();
			LONG lDestinationContainerPosition	= pMsg->GetLong();

			LONG lObjectType						= pMsg->GetLong();
			CGUID goodsGuid;
			pMsg ->GetGUID(goodsGuid);
			CGUID guidRet=goodsGuid;
			LONG lAmount							= pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>(CSessionFactory::QuerySession(SourceContainerId));
			if( pSession )
			{
				CPlug* pPlug = pSession->QueryPlugByID( lSourceContainerExtendID >> 8 );
				if( pPlug )
				{
					CGoodsShadowContainer* pContainer = dynamic_cast<CGoodsShadowContainer*>(
						pPlug->GetContainer(lSourceContainerExtendID & 0x000000ff) );
					CPlayer* pPlayer = dynamic_cast<CPlayer*>( pPlug->GetOwner() );
					if( pContainer && pPlayer && pPlayer->GetType() == lDestinationContainerType &&
						pPlayer->GetExID() == DestinationContainerID )
					{
						DWORD dwMaxStack=CGoodsFactory::QueryGoodsMaxStackNumber(lGoodsId);
						if(dwMaxStack>1)
						{
							//����GOODSID���
							pContainer->RemoveShadowByGoodsIndex(lGoodsId);
						}
						else
						{
							//����GUID���
							pContainer->RemoveShadowByGoodsGuid(goodsGuid);
						}
						CMessage msg(MSG_S2C_PLAYERSHOP_REMOVE);
						msg.Add(lGoodsId);
						msg.Add(guidRet);
						msg.SendToPlayer(pPlayer->GetExID());
					}
				}
			}
		}
		break;
	case MSG_C2S_PLAYERSHOP_BUYRELIVEITEM:
		{
		}
		break;
	case MSG_C2S_PLAYERSHOP_BUYITEM:
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long lPlugID	= pMsg->GetLong();
			long lGoodsNum	= pMsg->GetLong();

			//�����̵���ϼ���λ����Ϊ15��
			if(lGoodsNum<=0 || lGoodsNum>15)
				return;


			CPersonalShopSeller::VecBuyInfo *pVecBuyInfo = new CPersonalShopSeller::VecBuyInfo;
			pVecBuyInfo->clear();
			for(long i=0;i<lGoodsNum;i++)
			{
				CPersonalShopBuyer::tagBuyInfo stBuyInfo;
				stBuyInfo.lId	= pMsg->GetLong();

				//Ψһ���	
				CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(stBuyInfo.lId);
				if(!pProperty)
					return;
				if(pProperty->IsHasAddonPropertie(GAP_UNIQUE))
				{					
					if(!pPlayer->IsGoodsUnique(CGoodsFactory::QueryGoodsOriginalName(stBuyInfo.lId)))
					{
						char* strMsg=CStringReading::LoadString(20,49);
						if(strMsg)
						{
							CMessage msg(MSG_S2C_OTHER_GAMEMESSAGE);
							msg.Add(strMsg);
							msg.Add((long)0);
							msg.Add(CGUID::GUID_INVALID);
							msg.SendToPlayer(pPlayer->GetExID());
						}
						return;
					}
				}	

				pMsg->GetGUID(stBuyInfo.guid);
				stBuyInfo.dwNum	= pMsg->GetDWord();				
				if(stBuyInfo.dwNum==0)
					return;
				//ȷ��������Ϣ��guidΨһ
				for(size_t j=0;j<pVecBuyInfo->size();j++)
				{
					if(stBuyInfo.guid==(*pVecBuyInfo)[j].guid)
					{
						return;
					}
				}
				pVecBuyInfo->push_back(stBuyInfo);
			}

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( !pSession || !pSession->IsSessionAvailable() )
				break;

			CPersonalShopBuyer* pBuyerPlug = dynamic_cast<CPersonalShopBuyer*>( pSession->QueryPlugByID(lPlugID) );
			if( !pBuyerPlug || pBuyerPlug->GetOwnerID() != pPlayer->GetExID() )
				break;

			CPersonalShopSeller* pSellerPlug = pBuyerPlug->GetSeller();
			if( !pSellerPlug )
				break;

			CPlayer *pSeller = GetGame()->FindPlayer(pSellerPlug->GetOwnerID());
			if( !pSeller )
				break;

			if(pSeller->IsDied())
			{
				char* strRes=CStringReading::LoadStringA(21,11);
				if(strRes)
				{
					pPlayer->SendNotifyMessage(strRes);
				}
				break;
			}

			if ( pSeller->GetCurrentProgress() != CPlayer::PROGRESS_OPEN_STALL )
			{
				break;
			}

			pSellerPlug->AddMapBuying(lPlugID, pVecBuyInfo);
			DWORD dwTotalMoney = 0;
			DWORD dwTotalWeimian = 0;
			BOOL eRes = pSellerPlug->CheckCondition(lPlugID, dwTotalMoney, dwTotalWeimian);

			if( CPersonalShopSeller::CR_WAIT_FINISH == eRes )
			{
				pSellerPlug->PurchaseEx(lPlugID, dwTotalMoney, 0, dwTotalWeimian);
			}
			pSellerPlug->DelMapBuying(lPlugID);
		}
		break;

	case MSG_C2S_PLAYERSHOP_MSGBOARDSET:
		{

			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long SellerPlugID = pMsg->GetLong();
			BYTE	bMsgType = pMsg->GetByte();

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if ( !pSession ) return;
			CPersonalShopSeller* pSellerPlug = dynamic_cast<CPersonalShopSeller*>( pSession->QueryPlugByID(SellerPlugID) );

			if ( pSellerPlug )
			{
				if ( bMsgType& 0x1 )//��һλΪ1�����������̵�����
				{
					char szName[256];
					pMsg->GetStr( szName, 256 );

					if( strlen(szName) <= 16 )//16���ַ���8������
					{
						string strShopName = szName;

						CWordsFilter* pFilter = CWordsFilter::GetInstance();
						if( pFilter )
						{
							bool bCheck = pFilter->Check( strShopName, true, true );
							if ( !bCheck )
							{
								pPlayer->SendNotifyMessage( CStringReading::LoadString(21,15) );
								return;
							}
						}
						pSellerPlug->SetShopName( const_cast<CHAR*>( strShopName.c_str() ) );

						if( pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_OPEN_STALL ) 
						{
							return;
						}

						CMessage msg( MSG_S2C_PLAYERSHOP_MSGBOARDGET );
						msg.Add( SessionID );
						msg.Add( pPlayer->GetExID() );
						msg.Add( (BYTE)1 );
						msg.Add( pSellerPlug->GetShopName() );

						//Ⱥ�������̵�����
						msg.SendToAround( pPlayer );
					}
					else 
					{
						pPlayer->SendNotifyMessage( CStringReading::LoadString(21,5) );
					}
				}
				else if ( bMsgType & 0x2 )//�ڶ�λΪ1�����������̵���
				{
					char szShopInfo[256];
					pMsg->GetStr( szShopInfo, 256 );

					if( strlen(szShopInfo) <= 100 )//100���ַ�
					{
						string strShopInfo = szShopInfo;
						CWordsFilter* pFilter = CWordsFilter::GetInstance();
						if( pFilter )
						{
							bool bCheck = pFilter->Check( strShopInfo, true, true );
							if ( !bCheck )
							{
								pPlayer->SendNotifyMessage( CStringReading::LoadString(21,15) );
								return;
							}
						}
						pSellerPlug->SetShopInfo( strShopInfo );

						CMessage msg( MSG_S2C_PLAYERSHOP_MSGBOARDGET );
						msg.Add( SessionID );
						msg.Add( pPlayer->GetExID() );
						msg.Add( (BYTE)2 );
						msg.Add( pSellerPlug->GetShopInfo() );

						//Ⱥ�����õ����ƽ�
						list<LONG>::iterator plugIter = pSession->GetPlugList().begin();
						for( ; plugIter != pSession->GetPlugList().end(); ++plugIter )
						{
							CPlug* pPlug = const_cast<CPlug*>( CSessionFactory::QueryPlug(*plugIter) );
							if( pPlug && pPlug->GetOwnerType() == TYPE_PLAYER )
							{
								msg.SendToPlayer(pPlug->GetOwnerID());
							}
						}
					}
					else 
					{
						pPlayer->SendNotifyMessage( CStringReading::LoadString(21,13) );//�����и��´���ʾ��(�����ƽ��������50�����֡�)
					}
				}
				else if ( bMsgType & 0x4 )//�ڶ�λΪ1���������ý�����Ϣ
				{
					//�˴��ɷ������ÿ�ν������ʱ���£��ͻ��˲���������
				}
			}
		}
		break;
	case MSG_C2S_PLAYERSHOP_MSGBOARDCHAT:
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long lPlugID = pMsg->GetLong();
			char szChatInfo[256];	
			pMsg->GetStr( szChatInfo, 256 );

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if ( pSession && pSession->IsSessionAvailable()  )
			{
				CPlug* pPlug = pSession->QueryPlugByID(lPlugID);
				//��Ч��PlugID
				if ( pPlug == NULL )
				{
					return;
				}

				//��ҷ���������Ϣʱ��ָ����PlugIDΪ�����PlugID
				if ( pPlayer->GetExID() == pPlug->GetOwnerID() )
				{
					CPersonalShopSeller* pSeller = (dynamic_cast<CPersonalShopBuyer*>(pPlug))->GetSeller();
					long lSellPlugID = pSeller->Get32ID();

					CMessage msg( MSG_S2C_PLAYERSHOP_MSGBOARDCHAT );
					msg.Add( SessionID );
					msg.Add( lPlugID );
					msg.Add( pPlug->GetOwnerID() );
					msg.Add( szChatInfo );
					msg.SendToPlayer( pSeller->GetOwnerID() );
				}
				//���һظ����ԣ�ָ����PlugIDΪ��ҵ�PlugID
				else
				{
					CPersonalShopBuyer* pBuyer = dynamic_cast<CPersonalShopBuyer*>( pSession->QueryPlugByID(lPlugID) );
					CPersonalShopSeller* pSeller = pBuyer->GetSeller();
					CMessage msg( MSG_S2C_PLAYERSHOP_MSGBOARDCHAT );
					msg.Add( SessionID );
					msg.Add( lPlugID );
					msg.Add( pSeller->GetOwnerID() );
					msg.Add( szChatInfo );
					msg.SendToPlayer( pBuyer->GetOwnerID() );
				}
			}
		}
		break;

	default:
		break;
	}
}