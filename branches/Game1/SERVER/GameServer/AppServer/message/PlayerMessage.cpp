#include "stdafx.h"
#include "..\serverregion.h"
#include "..\Player.h"
#include "..\Monster.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"
#include "..\script\script.h"
#include "..\goods\cgoods.h"
#include "../setup/GlobeSetup.h"
#include "..\Session\CSessionFactory.h"
#include "..\Session\CSession.h"
#include "..\Session\CPlug.h"
#include "..\Session\CTrader.h"
#include "../../../../public/StrUnifyInput.h"
#include "../../../../public/GoodsExDef.h"
#include "../goods/CGoodsFactory.h"
#include "../GoodsExManage.h"
#include "..\SMClient.h"
// ��ӦGM��Ϣ
void CPlayer::OnMessage(CMessage* pMsg)
{
	//if( IsInChangingServer() )
	//{
	//	return;
	//}

	switch(pMsg->GetType())
	{
		// ����NPC�ű�
	case MSG_C2S_PLAYER_TOUCHNPC:
		{
			CPlayer* pPlayer = pMsg -> GetPlayer();

			if (pPlayer && pPlayer->JudgeNpcScriptslegality())
			{
				return;	
			}

			if( pPlayer && pMsg -> GetRegion())
			{
				CSMClient::GetSM()->SendWatchResult(pPlayer->GetName(),eGGC_TouchNpc);
				if( pPlayer -> IsDied() )
				{
					return;
				}

				if( pPlayer -> GetCurrentProgress() != CPlayer::PROGRESS_NONE )
				{
					return;
				}

				CGUID guid;
				pMsg -> GetGUID(guid);
				CMonster* pNpc	= dynamic_cast<CMonster*>( pMsg -> GetRegion() -> FindAroundObject( pPlayer, TYPE_MONSTER, guid ) );
				if( pNpc )
				{
					if( pNpc -> Distance( pPlayer ) > 8 )
					{
						char* strRes=CStringReading::LoadString(20,50);
						if(strRes)
						{
							pPlayer -> SendNotifyMessage(strRes);
						}
						return;
					}

					if( pNpc -> GetTalkScriptFile()[0] != '0' )
					{
						stRunScript st;
						st.pszFileName = (char*)pNpc->GetTalkScriptFile();
						st.desShape =pNpc; //pNpc;
						st.pRegion = pMsg->GetRegion();
						st.srcShape =this; //this;
						RunScript(&st, (char*)GetGame()->GetScriptFileData(pNpc->GetTalkScriptFile()));
					}
				}
				else
				{
					//##�Ҳ���ָ����NPC��
				}
			}
		}
		break;
	case MSG_C2S_PLAYER_USEITEM: //�������ʹ��һ�������Ե��ߵĴ���
		{   
			if( pMsg -> GetPlayer() -> IsDied() == FALSE )
			{

				if( pMsg -> GetPlayer() -> GetCurrentProgress() != CPlayer::PROGRESS_TRADING && 
					/*pMsg -> GetPlayer() -> GetCurrentProgress() != CPlayer::PROGRESS_SHOPPING &&	*/				
					pMsg -> GetPlayer() -> GetCurrentProgress() != CPlayer::PROGRESS_INCREMENT)
				{
					WORD wPlace=pMsg->GetWord();
					BYTE  bytePos=pMsg->GetByte();
					LONG lType=pMsg->GetLong();
					CGUID objGuid;
					pMsg->GetGUID(objGuid);
					DWORD dwGoodsId=0;

					//��֤containerId
					CVolumeLimitGoodsContainer* pVolContainer=NULL;
					if(wPlace==CS2CContainerObjectMove::PEI_PACKET)
					{
						pVolContainer=pMsg->GetPlayer()->getOriginPack();
					}
					else if(wPlace>=CS2CContainerObjectMove::PEI_PACK1 && 
						wPlace<=CS2CContainerObjectMove::PEI_PACK5)
					{
						pVolContainer=pMsg->GetPlayer()->getSubpackContainer()->GetSubpack(wPlace-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
					}

					if(pVolContainer)
					{
						if(bytePos>=0 && bytePos<pVolContainer->GetVolume())
						{
							CGUID goodsGuid=pVolContainer->GetCellGuid()[bytePos];
							CGoods* pGoods=pVolContainer->GetGoods(bytePos);
							if(pGoods && pGoods->GetGoodsBaseType()==GT_CONSUMABLE)
							{
#ifdef _GOODSLOG1_
								char pszGoodsLog[1024]="";			
								char srcGoodsGUID[50]="";
								char serGoodsGUID[50]="";
								objGuid.tostring(srcGoodsGUID);
								goodsGuid.tostring(serGoodsGUID);
								_snprintf(pszGoodsLog,1024,"[MSG_C2S_PLAYER_USEITEM][����:%d][λ��:%d][TYPE:%d][GUID:%s][GUID-SERVER:%s]",
									wPlace,bytePos,lType,srcGoodsGUID,serGoodsGUID);
								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
								dwGoodsId=pGoods->GetBasePropertiesIndex();
								pMsg -> GetPlayer() -> UseItem(wPlace, bytePos ,lType,objGuid);
							}
						}						
					}					
				}
			}
		}
		break;

	case MSG_C2S_PLAYER_PKSWITCH:
		{
			// �ݵ�ս���ܿ���ʱ�����޸�PK״̬
			CServerRegion* pRgn = dynamic_cast<CServerRegion*>(GetFather());
			if( pRgn && pRgn->GetStrongPointFlag()>0 ) return;

			BYTE lPos = pMsg->GetByte();
			bool bPk = pMsg->GetByte()==0?false:true;
			switch(lPos)
			{
				case 0:
					SetPk_Normal(bPk);				
					break;
				case 1:
					SetPk_Team(bPk);				
					break;
				case 2:
					SetPk_Union(bPk);				
					break;
				case 3:
					SetPk_Badman(bPk);				
					break;
				case 4:
					SetPk_Country(bPk);				
					break;
				case 5:
					SetPk_Camp(bPk);
					break;
				case 6:
					SetPk_Pirate(bPk);
					break;
				case 7:
					SetPk_Hint(bPk);
					break;
				case 8:
					SetPk_AutoPlayer(bPk);
					break;
				case 9:
					SetPk_AutoMonster(bPk);
					break;
			}
		}
		break;
	case MSG_C2S_PLAYER_TRADEREQUEST://��ҷ���һ������
		{
			//##�����ǰ�ڽ��������,�������ͽ�������.
			CPlayer* pPlayer = pMsg -> GetPlayer();
			pPlayer->ProduceEvent(ET_Trade);
			if( pPlayer && pPlayer -> IsDied() == FALSE )
			{				
				/*
				LONG lSkillLev= pPlayer->GetSkillLv(CGlobeSetup::GetSetup()->lCorrelativeSkillID);
				if(lSkillLev<CGlobeSetup::GetSetup()->lMinTradeLevel)
				{
					char notifyMsg[256];
					char* strRes=CStringReading::LoadString(23,3);
					if(strRes)
					{
						sprintf_s(notifyMsg,strRes,"���",CGlobeSetup::GetSetup()->lMinTradeLevel);
						pPlayer -> SendNotifyMessage(notifyMsg,NOTIFY_COLOR_RED,0,eNOTIFYPOS_CENTER);
					}					
					return;
				}*/
				//if(pPlayer->GetPlayerDepot()->TestSession())
				//{
				//	char* strRes=CStringReading::LoadString(23,4);
				//	if(strRes)
				//	{
				//		pPlayer -> SendNotifyMessage(strRes);
				//	}					
				//	return;
				//}
				if( pPlayer -> GetCurrentProgress() != CPlayer::PROGRESS_NONE )
				{
					//##������Ϣ����ǰ���.
					char* strRes=CStringReading::LoadString(23,5);
					if(strRes)
					{
						pPlayer -> SendNotifyMessage(strRes);
					}					
					return;
				}

				const CGUID& PlayerID			= pPlayer -> GetExID();
				CGUID TradePlayerID;
				pMsg -> GetGUID(TradePlayerID);	//����Ŀ����ҵ�ID
				CPlayer* pTradePlayer	= GetGame() -> FindPlayer( TradePlayerID );
				if(pTradePlayer)
				{
					CRegion* pLocalRgn = static_cast<CRegion*>(pTradePlayer->GetFather());
					if(pLocalRgn)
					{
						if( pTradePlayer->GetHideFlag() == 1 && !pTradePlayer->IsGM() && pLocalRgn->GetRgnHideFlag() != 2
							&& (pPlayer->GetTeamID() != pTradePlayer->GetTeamID() || pPlayer->GetTeamID()==NULL_GUID || pTradePlayer->GetTeamID()==NULL_GUID) )// ������,��GM
						{
							return;
						}
					}
				}

				//������Ϣת��
				if( pTradePlayer && pTradePlayer -> IsDied() == FALSE )
				{
					//##������������ҵ�ǰû���κλ,����������Ϣ.
					if( pTradePlayer -> GetCurrentProgress() == CPlayer::PROGRESS_NONE )
					{
						/*
						LONG lLev= pTradePlayer->GetSkillLv(CGlobeSetup::GetSetup()->lCorrelativeSkillID);
						if(lLev<CGlobeSetup::GetSetup()->lMinTradeLevel)
						{
							
							char* strRes=CStringReading::LoadString(23,3);
							if(strRes)
							{
								char notifyMsg[256];
								sprintf_s(notifyMsg,strRes,pTradePlayer->GetName(),CGlobeSetup::GetSetup()->lMinTradeLevel);
								pPlayer -> SendNotifyMessage(notifyMsg,NOTIFY_COLOR_RED,0,eNOTIFYPOS_CENTER);
							}					
							return;
						}*/
						if(pTradePlayer->GetPlayerDepot()->TestSession())
						{
							char* strRes=CStringReading::LoadString(23,4);
							if(strRes)
							{
								pPlayer -> SendNotifyMessage(strRes);
							}	
							return;
						}
						CServerRegion* pRegion1 = dynamic_cast<CServerRegion*>( pPlayer -> GetFather() );
						CServerRegion* pRegion2 = dynamic_cast<CServerRegion*>( pTradePlayer -> GetFather() );
						if(pRegion1 && pRegion2 && (pRegion2==pRegion1))
						{						
							if( pPlayer -> Distance( pTradePlayer ) <= 8 )
							{
								CMessage msg(MSG_S2C_PLAYER_TRADEREQUEST);
								msg.Add(PlayerID);
								msg.SendToPlayer( pTradePlayer -> GetExID() );
								char* strRes=CStringReading::LoadString(23,6);
								if(strRes)
								{
									pPlayer -> SendNotifyMessage(strRes);
								}							
							}
							else
							{
								//##���������
								char* strRes=CStringReading::LoadString(23,7);
								if(strRes)
								{
									pPlayer -> SendNotifyMessage(strRes);
								}							
							}
						}
						else
						{
							char* strRes=CStringReading::LoadString(23,22);
							if(strRes)
							{
								pPlayer -> SendNotifyMessage(strRes);
							}	
						}
					}
					else
					{
						//##������Ϣ����ǰ���.
						char* strRes=CStringReading::LoadString(23,8);
						if(strRes)
						{
							pPlayer -> SendNotifyMessage( strRes );
						}						
					}
				}
				else if( pTradePlayer )
				{
					//##�Է��Ѿ�����
					char* strRes=CStringReading::LoadString(23,9);
					if(strRes)
					{
						pPlayer -> SendNotifyMessage(strRes);
					}					
				}
				else
				{
					//##�޷��ҵ�����������,֪ͨ���û�.
					char* strRes=CStringReading::LoadString(23,10);
					if(strRes)
					{
						pPlayer -> SendNotifyMessage(strRes );
					}					
				}
			}
			else if( pPlayer )
			{
				//##��ʵ����Ѿ�����
				char* strRes=CStringReading::LoadString(23,11);
				if(strRes)
				{
					pPlayer -> SendNotifyMessage( strRes );
				}
			}
		}
		break;
	case MSG_C2S_PLAYER_TRADERESPONSE: //��Ҷ�һ�������������Ӧ
		{
			//##��Ӧ�����������
			CPlayer* pPlayer = pMsg -> GetPlayer();
			if( pPlayer && pPlayer -> IsDied() == FALSE )
			{
				//������������ID
				CGUID PlayerID;
				pMsg -> GetGUID(PlayerID);
				BYTE bAgree		= pMsg -> GetByte(); //�Ƿ�ͬ�⽻��

				CPlayer* pSender = GetGame() -> FindPlayer( PlayerID );

				if( pPlayer -> GetCurrentProgress() == CPlayer::PROGRESS_NONE )
				{
					if( pSender && pSender -> IsDied() == FALSE )
					{
						if( pSender -> GetCurrentProgress() == CPlayer::PROGRESS_NONE )
						{
							if( pSender -> Distance( pPlayer ) <= 8 )
							{
								//CMessage msg( MSG_S2C_PLAYER_TRADERESPONSE );
								//msg.Add( pPlayer -> GetExID() );
								//msg.Add( bAgree );
								//msg.SendToPlayer( pSender -> GetExID() );
								if( bAgree )
								{
									//##���뽻��״̬
									//pPlayer -> SetTradePlayerID( pSender -> GetExID() );
									pPlayer -> SetCurrentProgress( CPlayer::PROGRESS_TRADING );
									//pSender -> SetTradePlayerID( pPlayer -> GetExID() );
									pSender -> SetCurrentProgress( CPlayer::PROGRESS_TRADING );
									//##�����Ự
									const CGUID& SessionID			= CSessionFactory::CreateSession( 2, 2 );
									CSession* pSession		= const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
									if( pSession && pSession -> Start() )
									{
										//! �ر�������Ʒ���ܴ���
										pPlayer->GetPlayerDepot()->OnLockRequest(NULL);
										GoodsExManage::GetInstance().CloseGoodsExWindow(pPlayer);
										pSender->GetPlayerDepot()->OnLockRequest(NULL);
										GoodsExManage::GetInstance().CloseGoodsExWindow(pSender);

										pPlayer->ClearPersonalShopGoodsData();
										pSender->ClearPersonalShopGoodsData();
										long RequesterPlugID	= CSessionFactory::CreatePlug( CSessionFactory::PT_TRADER, TYPE_PLAYER, PlayerID );
										long ResponserPlugID	= CSessionFactory::CreatePlug( CSessionFactory::PT_TRADER, TYPE_PLAYER, pPlayer -> GetExID() );
										CSessionFactory::InsertPlug( SessionID, RequesterPlugID );
										CSessionFactory::InsertPlug( SessionID, ResponserPlugID );

										pPlayer->SetTradeSessionId(SessionID);
										pPlayer->SetTradePlugId(ResponserPlugID);

										pSender->SetTradeSessionId(SessionID);
										pSender->SetTradePlugId(RequesterPlugID);

										CMessage msg( MSG_S2C_PLAYER_TRADEBEGIN );
										msg.Add( SessionID );//���ν���session id
										msg.Add( PlayerID );//������������ID
										msg.Add( RequesterPlugID );//������PLUG_ID
										msg.Add( pPlayer -> GetExID() );//��Ӧ�����ID
										msg.Add( ResponserPlugID );//��Ӧ����ҵ�PLUG_ID

										//��Ϣ����˫��
										msg.SendToPlayer( PlayerID );
										msg.SendToPlayer( pPlayer -> GetExID() );
									}
								}
								else 
								{
									char* strRes=CStringReading::LoadString(23,12);
									pSender -> SendNotifyMessage(strRes);
								}
							}
							else
							{
								//##����ʱ˫��û�������
								char* strRes=CStringReading::LoadString(23,7);
								pPlayer -> SendNotifyMessage( strRes );
								pSender -> SendNotifyMessage( strRes );
							}
						}
						else
						{
							//##�����������������������
							char* strRes=CStringReading::LoadString(23,13);
							pPlayer -> SendNotifyMessage(strRes);
							strRes=CStringReading::LoadString(23,5);
							pSender -> SendNotifyMessage( strRes );
						}
					}
					else if( pSender )
					{
						char* strRes=CStringReading::LoadString(23,14);
						pPlayer -> SendNotifyMessage( strRes );
						strRes=CStringReading::LoadString(23,11);
						pSender -> SendNotifyMessage( strRes );
					}
					else
					{
						//##�Ҳ������뷽
						char* strRes=CStringReading::LoadString(23,15);
						pPlayer -> SendNotifyMessage( strRes );
					}
				}
				else
					//##��Ӧ����������˴�������״̬��
				{
					char* strRes=CStringReading::LoadString(23,5);
					pPlayer -> SendNotifyMessage(strRes);
					if( pSender )
					{
						strRes=CStringReading::LoadString(23,16);
						pSender -> SendNotifyMessage( strRes );
					}
				}
			}
			else if( pPlayer )
			{
				//##��ʵ����Ѿ�����
				char* strRes=CStringReading::LoadString(23,11);
				pPlayer -> SendNotifyMessage( strRes );
			}
		}
		break;	
	case MSG_C2S_PLAYER_TRADECHANGEMONEY://�޸Ľ������Ľ������
		{
			CPlayer* pPlayer=pMsg->GetPlayer();
			if(!pPlayer)
				return;
			CGUID sessionGuid;
			pMsg->GetGUID(sessionGuid);
			//���ҽ���session
			CSession* pSession=const_cast<CSession*>(CSessionFactory::QuerySession(sessionGuid));				
			if( pSession )
			{
				LONG lPlugId=pMsg->GetLong();
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
				CGoods* pGoods=CGoodsFactory::CreateGoods(CGlobeSetup::GetSetup()->dwGoldGoodsId,345);
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
	case MSG_C2S_PLAYER_TRADELOCK://��Ұ���������ť
		{			
			CPlayer* pPlayer=pMsg->GetPlayer();
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			LONG lPlugId=pMsg->GetLong();

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID));
			if( pPlayer && pSession )
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
	case MSG_C2S_PLAYER_TRADING://��Ұ��½��װ�ť,���뽻�׵ȴ�״̬
		{
			CPlayer* pPlayer	= pMsg -> GetPlayer();
			CGUID SessionID;
			pMsg -> GetGUID(SessionID);
			long PlugID = pMsg->GetLong();

			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pPlayer && pSession )
			{
				if(pPlayer->GetTradeSessionId()!=SessionID || pPlayer->GetTradePlugId()!=PlugID)
					return;
				CTrader* pPlug = dynamic_cast<CTrader*>( pSession -> QueryPlugByOwner( 
					pPlayer -> GetType(), pPlayer -> GetExID() ) );
				if( pPlug && pPlug -> Get32ID() == PlugID )
				{
					DWORD dwTradeSize=pMsg->GetLong();
					if(dwTradeSize>12)
						return;
					pPlug->ClearBuyGoodsInfo();
					for(DWORD i=0;i<dwTradeSize;i++)
					{
						CGUID goodsGuid;
						pMsg->GetGUID(goodsGuid);
						pPlug->AddBuyGoodsInfo(goodsGuid,pMsg->GetDWord());
					}
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
	case MSG_C2S_PLAYER_TRADECANCEL://��ҽ��׹�����ȡ������
		{
			CPlayer* pPlayer	= pMsg -> GetPlayer();
			CGUID SessionID;
			pMsg -> GetGUID(SessionID);
			long PlugID = pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pPlayer && pSession )
			{
				CTrader* pPlug = dynamic_cast<CTrader*>( pSession -> QueryPlugByOwner( 
					pPlayer -> GetType(), pPlayer -> GetExID() ) );
				if( pPlug && pPlug -> Get32ID() == PlugID )
				{
					if( pSession -> IsSessionAvailable() )
					{
						pSession -> Abort();
					}
				}
			}
		}
		break;
	case MSG_C2S_PLAYER_HELPREQUEST://�����������ĵ�
		{
			stRunScript st;
			st.pszFileName = "scripts/help/help.script";
			st.desShape = this;
			st.pRegion = pMsg->GetRegion();
			st.srcShape = NULL;
			RunScript(&st, (char*)GetGame()->GetScriptFileData(st.pszFileName));
		}
		break;
	case MSG_C2S_PLAYER_QUEST_RELIVE:
		{

			CPlayer *pPlayer = pMsg->GetPlayer();
			if(!pPlayer->IsDied())
				return;
			BYTE bRliveType = pMsg->GetByte();
			if (bRliveType==0)
			{
				//�سǸ���
				pPlayer->OnRelive(0);
			} 
			else
			{
				//ԭ�ظ���
				string strLv = "Level";
				if (pPlayer->GetAttribute(strLv)<=10)
				{
					pPlayer->OnRelive(1);
				}
				else
				{
					
					string str=CGlobeSetup::GetSetup()->szReliveGoodsName;
					DWORD dwIndex = CGoodsFactory::QueryGoodsIDByOriginalName(str.c_str());

					if (pPlayer->GetGoodsAmountByOriginName(str.c_str())>=1)
					{
						//ԭ�ظ���
						pPlayer->DelGoods((char*)str.c_str(),1);
						OnRelive(1);
						CMessage msg(MSG_S2C_PLAYER_RELIVE_RESULT);
						msg.Add(BYTE(1));//�ɹ�
						msg.Add(dwIndex);
						msg.Add(pPlayer->GetExID());

					} 
					else
					{
						//����Ԥ��																		
						CMessage msg(MSG_S2B_ACCOUNT_LOCK);
						msg.Add((BYTE)eICT_USE_FUNCTION); //��������
						msg.Add(pPlayer->GetExID());	//player guid
						msg.Add(pPlayer->GetAccount());	//player cdkey
						msg.Add(NULL_GUID);				//attach guid
						msg.Add((DWORD)1);
						msg.Add((DWORD)eIFUNC_RELIVE);			//goods id
						msg.Add((DWORD)1);					//goods num
						msg.Add(CGlobeSetup::GetSetup()->lRelivePrice);			//money sum
						msg.SendToBS();
					}
					/*
					else
					{				
						CMessage msg(MSG_S2C_PLAYER_RELIVE_RESULT);
						msg.Add(BYTE(0));//ʧ��
						msg.Add(dwIndex);
						msg.SendToPlayer(pPlayer->GetExID());// Add(pPlayer->GetExID());
					}*/

				}
				

			}						
		}
		break;
	case MSG_C2S_PLAYER_SKILL_RELIVE:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (pPlayer && pPlayer->GetReliveSkill())
			{
				long lRecoverRatio = pMsg->GetLong();
				pPlayer->SetReliveSkill(false);				
				pPlayer->OnRelive(1,lRecoverRatio);
			}			
		}
		break;
	case MSG_C2S_PLAYER_SKILL_CANCEL_RELIVE:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (pPlayer && pPlayer->IsDied())
			{
				pPlayer->FreeReliveScript();

				const char* strScript = CGlobeSetup::GetSetup()->pszPlayerDiedScript;
				if(strScript)
				{
					stRunScript sScript;
					sScript.pszFileName = (char*)strScript;
					sScript.desShape = NULL;
					sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					sScript.srcShape	= this;
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
				}
			}
		}
		break;

	case MSG_C2S_PLAYER_UpdateShowFashion:
		{
			if(pMsg->GetPlayer())
			{
				BYTE bShowFashion = pMsg->GetByte();
				
				pMsg->GetPlayer()->SetShowFashion(bShowFashion);

				CMessage msg(MSG_S2C_PLAYER_UpdateShowFashion);
				msg.Add(pMsg->GetPlayer()->GetExID());
				msg.Add(bShowFashion);
				msg.SendToAround(pMsg->GetPlayer(),NULL);
			}
		}
		break;
	case MSG_C2S_PLAYER_CHANGE_TITLE_TYPE:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			LONG lNewTitleType = pMsg->GetLong();
			if(NULL != pPlayer)
			{
				pPlayer->SetTitleType(lNewTitleType);
			}
		}
		break;
	case MSG_C2S_PLAYER_SETHOTKEY:
		{
			CPlayer* pPlayer = pMsg->GetPlayer();
			if (pPlayer!=NULL)
			{
				MAPFKEY& mapkey = pPlayer->GetCurMapKey();
				MAPFKEY  tmapkey;
				tmapkey.clear();
				DWORD dwNum = pMsg->GetDWord();
				for (DWORD i = 0;i<dwNum;i++)
				{
					MAPKEY tkey;
					pMsg->GetEx(&tkey,sizeof(MAPKEY));
					if (!CFuncKeyMapSystem::InsertKeyToCurMap(tmapkey,tkey.funcIndex,tkey))
					{
						// ����й��ܱ��ظ��������ȼ���������ʧ�ܡ�ȡ���˴�����
						CMessage msg(MSG_S2C_PLAYER_SETHOTKEY_RESULT);
						msg.Add((BYTE)1);
						msg.SendToPlayer(pPlayer->GetExID());
						return;
					}
				}
				// ȫ�����ݶ�����ɹ����ٽ����Ǳ��浽��ǰ��mapkey��
				mapkey.clear();
				MAPFKEYITER keyiter_b = tmapkey.begin();
				MAPFKEYITER keyiter_e = tmapkey.end();
				for (;keyiter_b!=keyiter_e;keyiter_b++)
				{
					CFuncKeyMapSystem::InsertKeyToCurMap(mapkey,keyiter_b->first,keyiter_b->second);
				}
				// ���ͳɹ�����Ϣ
				CMessage msg(MSG_S2C_PLAYER_SETHOTKEY_RESULT);
				msg.Add((BYTE)0);
				msg.SendToPlayer(pPlayer->GetExID());
			}
		}
		break;
	}
}