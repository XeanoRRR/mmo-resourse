/**
* @filename:	PlayerTradeMessage.cpp
* @date:		2010/7/22
* @author:		wing (totti19851101@gmail.com) 

* @purpose: ��ҽ�����Ϣ����
*/

#pragma once
#include "stdafx.h"
#include "../Player.h"
#include "../Goods/CGoods.h"
#include "../Goods/CGoodsFactory.h"
#include "../GoodsExManage.h"
#include "../Session/CSessionFactory.h"
#include "../Session/CSession.h"
#include "../Session/CPlug.h"
#include "../Session/CTrader.h"

#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void OnPlayerTradeMessage(CMessage* pMsg)
{
	CPlayer* pPlayer = pMsg->GetPlayer();
	if (!pPlayer)
		return;

	switch(pMsg->GetType())
	{
		//��ҷ���һ����������
	case MSG_C2S_PLAYERTRADE_REQUEST:
		{
			// �����ǰ�ڽ��������,�������ͽ�������.
			if(!pPlayer->IsDied())
			{				
				if(pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_NONE)
				{
					// ������Ϣ����ǰ���.
					const char* strRes = AppFrame::GetText("GS_TRADE_5");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);
					return;
				}

				const CGUID& PlayerID = pPlayer->GetExID();
				CGUID TradePlayerID;
				pMsg->GetGUID(TradePlayerID);	//����Ŀ����ҵ�ID
				CPlayer* pTradePlayer = GetGame()->FindPlayer(TradePlayerID);
				if (!pTradePlayer)
				{
					//##�޷��ҵ�����������,֪ͨ���û�.
					const char* strRes = AppFrame::GetText("GS_TRADE_10");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);
					return;
				}

				CRegion* pLocalRgn = static_cast<CRegion*>(pTradePlayer->GetFather());
				if(pLocalRgn)
				{
					if( pTradePlayer->GetHideFlag() == 1 && !pTradePlayer->IsGM() && pLocalRgn->GetRgnHideFlag() != 2
						&& (pPlayer->GetTeamID() != pTradePlayer->GetTeamID() || pPlayer->GetTeamID()==NULL_GUID || pTradePlayer->GetTeamID()==NULL_GUID) )// ������,��GM
					{
						return;
					}
				}

				//������Ϣת��
				if(!pTradePlayer->IsDied())
				{
					//##������������ҵ�ǰû���κλ,����������Ϣ.
					if(pTradePlayer->GetCurrentProgress() == CPlayer::PROGRESS_NONE)
					{
						if(pTradePlayer->GetPlayerDepot()->TestSession())
						{
							const char* strRes = AppFrame::GetText("GS_TRADE_4");
							if(strRes)
								pPlayer -> SendNotifyMessage(strRes);
							return;
						}
						CServerRegion* pRegion1 = dynamic_cast<CServerRegion*>(pPlayer->GetFather());
						CServerRegion* pRegion2 = dynamic_cast<CServerRegion*>(pTradePlayer->GetFather());
						if(pRegion1 && pRegion2 && (pRegion2==pRegion1))
						{						
							if(pPlayer->Distance(pTradePlayer) <= 8)
							{
								CMessage msg(MSG_S2C_PLAYERTRADE_REQUEST);
								msg.Add(PlayerID);
								msg.SendToPlayer(pTradePlayer->GetExID());

								pPlayer->ProduceEvent(ET_Trade);
								const char* strRes = AppFrame::GetText("GS_TRADE_6");
								if(strRes)
									pPlayer->SendNotifyMessage(strRes);
							}
							else
							{
								// ���������
								const char* strRes = AppFrame::GetText("GS_TRADE_7");
								if(strRes)
									pPlayer->SendNotifyMessage(strRes);
							}
						}
						else
						{
							const char* strRes = AppFrame::GetText("GS_TRADE_22");
							if(strRes)
								pPlayer->SendNotifyMessage(strRes);
						}
					}
					else
					{
						// ������Ϣ����ǰ���.
						const char* strRes = AppFrame::GetText("GS_TRADE_8");
						if(strRes)
							pPlayer->SendNotifyMessage(strRes);
					}
				}
				else
				{
					// �Է��Ѿ�����
					const char* strRes = AppFrame::GetText("GS_TRADE_9");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);
				}
			}
			else
			{
				// ��ʵ����Ѿ�����
				const char* strRes = AppFrame::GetText("GS_TRADE_11");
				if(strRes)
					pPlayer->SendNotifyMessage(strRes);
			}
		}
		break;

		//��Ҷ�һ�������������Ӧ
	case MSG_C2S_PLAYERTRADE_RESPONSE:
		{
			// ��Ӧ�����������
			if(pPlayer->IsDied() == FALSE )
			{
				//������������ID
				CGUID PlayerID;
				pMsg->GetGUID(PlayerID);
				BYTE bAgree = pMsg->GetByte(); //�Ƿ�ͬ�⽻��

				CPlayer* pSender = GetGame()->FindPlayer(PlayerID);

				if( pPlayer->GetCurrentProgress() == CPlayer::PROGRESS_NONE )
				{
					if( pSender && pSender->IsDied() == FALSE )
					{
						if( pSender->GetCurrentProgress() == CPlayer::PROGRESS_NONE )
						{
							if( pSender->Distance( pPlayer ) <= 8 )
							{
								if(bAgree)
								{
									//##���뽻��״̬
									pPlayer->SetCurrentProgress(CPlayer::PROGRESS_TRADING);
									pSender->SetCurrentProgress(CPlayer::PROGRESS_TRADING);
									//##�����Ự
									const CGUID& SessionID = CSessionFactory::CreateSession(2, 2);
									CSession* pSession = const_cast<CSession*>(CSessionFactory::QuerySession(SessionID));
									if(pSession && pSession->Start())
									{
										//! �ر�������Ʒ���ܴ���
										pPlayer->GetPlayerDepot()->OnLockRequest(NULL);
										GoodsExManage::GetInstance().CloseGoodsExWindow(pPlayer);
										pSender->GetPlayerDepot()->OnLockRequest(NULL);
										GoodsExManage::GetInstance().CloseGoodsExWindow(pSender);

										pPlayer->ClearPersonalShopGoodsData();
										pSender->ClearPersonalShopGoodsData();
										long RequesterPlugID = CSessionFactory::CreatePlug(CSessionFactory::PT_TRADER, TYPE_PLAYER, PlayerID);
										long ResponserPlugID = CSessionFactory::CreatePlug(CSessionFactory::PT_TRADER, TYPE_PLAYER, pPlayer->GetExID());
										CSessionFactory::InsertPlug(SessionID, RequesterPlugID);
										CSessionFactory::InsertPlug(SessionID, ResponserPlugID);

										pPlayer->SetTradeSessionId(SessionID);
										pPlayer->SetTradePlugId(ResponserPlugID);

										pSender->SetTradeSessionId(SessionID);
										pSender->SetTradePlugId(RequesterPlugID);

										CMessage msg(MSG_S2C_PLAYERTRADE_BEGIN);
										msg.Add(SessionID);				//���ν���session id
										msg.Add(PlayerID);				//������������ID
										msg.Add(RequesterPlugID);		//������PLUG_ID
										msg.Add(pPlayer->GetExID());	//��Ӧ�����ID
										msg.Add(ResponserPlugID);		//��Ӧ����ҵ�PLUG_ID

										//��Ϣ����˫��
										msg.SendToPlayer(PlayerID);
										msg.SendToPlayer(pPlayer->GetExID());
									}
								}
								else 
								{
									const char* strRes = AppFrame::GetText("GS_TRADE_12");
									pSender->SendNotifyMessage(strRes);
								}
							}
							else
							{
								//##����ʱ˫��û�������
								const char* strRes = AppFrame::GetText("GS_TRADE_7");
								pPlayer->SendNotifyMessage(strRes);
								pSender->SendNotifyMessage(strRes);
							}
						}
						else
						{
							//##�����������������������
							const char* strRes = AppFrame::GetText("GS_TRADE_13");
							pPlayer->SendNotifyMessage(strRes);
							strRes = AppFrame::GetText("GS_TRADE_5");
							pSender->SendNotifyMessage(strRes);
						}
					}
					else if(pSender)
					{
						const char* strRes = AppFrame::GetText("GS_TRADE_14");
						pPlayer->SendNotifyMessage(strRes);
						strRes = AppFrame::GetText("GS_TRADE_11");
						pSender->SendNotifyMessage(strRes);
					}
					else
					{
						//##�Ҳ������뷽
						const char* strRes = AppFrame::GetText("GS_TRADE_15");
						pPlayer->SendNotifyMessage(strRes);
					}
				}
				else
					//##��Ӧ����������˴�������״̬��
				{
					const char* strRes = AppFrame::GetText("GS_TRADE_5");
					pPlayer->SendNotifyMessage(strRes);
					if(pSender)
					{
						strRes = AppFrame::GetText("GS_TRADE_16");
						pSender->SendNotifyMessage(strRes);
					}
				}
			}
			else if(pPlayer)
			{
				//##��ʵ����Ѿ�����
				const char* strRes = AppFrame::GetText("GS_TRADE_11");
				pPlayer->SendNotifyMessage(strRes);
			}
		}
		break;

		//�޸Ľ������Ľ������
	case MSG_C2S_PLAYERTRADE_CHANGEMONEY:
		{
			CGUID sessionGuid;
			pMsg->GetGUID(sessionGuid);
			//���ҽ���session
			CSession* pSession=const_cast<CSession*>(CSessionFactory::QuerySession(sessionGuid));				
			if( pSession )
			{
				long lPlugId=pMsg->GetLong();
				if(pPlayer->GetTradeSessionId()!=sessionGuid || pPlayer->GetTradePlugId()!=lPlugId)
					return;
				//���ҽ���PLUG
				CPlug* pPlug = pSession -> QueryPlugByID(lPlugId);				
				CTrader* pTrader=dynamic_cast<CTrader*>(pPlug);
				if(pTrader)
				{
					//���PLUG�Ѿ����� ���ܸı��Ǯ
					if(pTrader->IsLocked())
						return;
				}
				else
					return;
				//��Ҫ�ı������
				DWORD dwAmount=pMsg->GetDWord();
				if(pPlayer->GetMoney() < dwAmount)
					return;				
				CGoods* pGoods=CGoodsFactory::CreateGoods(GlobalSetup::GetSetup()->dwGoldGoodsId,345);
				pGoods->SetAmount(dwAmount);
				CGoodsContainer* pContainer = pTrader->GetGoldContainer();
				if(!pContainer)
					return;
				CContainer::tagPreviousContainer stPreviousContainer;
				stPreviousContainer.lContainerType=TYPE_PLAYER;
				stPreviousContainer.ContainerID=pPlayer->GetExID();
				stPreviousContainer.lContainerExtendID=CS2CContainerObjectMove::PEI_WALLET;
				stPreviousContainer.dwGoodsPosition=0;
				BOOL bResult=pContainer->Add( 0,pGoods,&stPreviousContainer,NULL);
				CGoodsFactory::GarbageCollect(&pGoods,345);
			}
		}
		break;

	case MSG_C2S_PLAYERTRADE_LOCK://��Ұ���������ť
		{			
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long lPlugId=pMsg->GetLong();

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID));
			if(pSession )
			{
				if(pPlayer->GetTradeSessionId()!=SessionID || pPlayer->GetTradePlugId()!=lPlugId)
					return;
				CTrader* pPlug = dynamic_cast<CTrader*>( pSession -> QueryPlugByOwner( 
					pPlayer -> GetType(), pPlayer -> GetExID() ) );
				if(pSession->IsSessionAvailable() && pPlug /*&& !pPlug->IsLocked()*/)
				{	
					if(pPlug->IsLocked())//Ŀǰ�Ѿ���������ı䵽׼��
					{
						pPlug->SetLockState(FALSE);
						pPlug->SetTradeState(FALSE);
						pSession->OnPlugChangeState(lPlugId,CTrader::TS_TRADE_PREPARING,NULL);
					}
					else
					{
						pPlug->SetLockState(TRUE);
						pSession->OnPlugChangeState(lPlugId,CTrader::TS_TRADE_LOCK,NULL);
					}					
				}
			}
		}
		break;		
	case MSG_C2S_PLAYERTRADE_TRADING://��Ұ��½��װ�ť,���뽻�׵ȴ�״̬
		{
			CGUID SessionID;
			pMsg -> GetGUID(SessionID);
			long PlugID = pMsg->GetLong();

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if(pSession )
			{
				if(pPlayer->GetTradeSessionId()!=SessionID || pPlayer->GetTradePlugId()!=PlugID)
					return;
				CTrader* pPlug = dynamic_cast<CTrader*>( pSession -> QueryPlugByOwner( 
					pPlayer -> GetType(), pPlayer -> GetExID() ) );
				if( pPlug && pPlug -> Get32ID() == PlugID )
				{
					if( pSession -> IsSessionAvailable() )
					{
						if( pPlug -> IsReady() )
						{
							pPlug -> SetTradeState( FALSE );
							pSession -> OnPlugChangeState( PlugID, CTrader::TS_TRADE_PREPARING, NULL );
						}
						else
						{
							pPlug -> SetTradeState( TRUE );
							pSession -> OnPlugChangeState( PlugID, CTrader::TS_TRADE_READY, NULL );
						}
					}
				}
			}
		}
		break;
	case MSG_C2S_PLAYERTRADE_CANCEL://��ҽ��׹�����ȡ������
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long PlugID = pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if(pSession )
			{
				CTrader* pPlug = 
					dynamic_cast<CTrader*>(pSession->QueryPlugByOwner(pPlayer->GetType(), pPlayer->GetExID()));
				if( pPlug && pPlug->Get32ID() == PlugID )
				{
					if( pSession->IsSessionAvailable() )
					{
						pSession->Abort();
					}
				}
			}
		}
		break;
	}
}