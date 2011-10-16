#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"

#include "..\player.h"
#include "..\Message Packaging\CS2CContainerObjectMove.h"
#include "..\Message Packaging\CS2CContainerObjectAmountChange.h"
#include "..\goods\cgoods.h"
#include "..\goods\cgoodsfactory.h"
#include "../goods/enchase/CKitCardRuleFactory.h"
#include "..\serverregion.h"
#include "..\..\..\Setup\GlobeSetup.h"
#include "..\Session\CSessionFactory.h"
#include "..\Session\CSession.h"

#include "..\Session\CEquipmentEnchase.h"
#include "..\script\script.h"
#include "..\setup\logsystem.h"
#include "..\container\CEquipmentEnchaseShadowContainer.h"
#include "../../../../public/StrUnifyInput.h"

#include "AppServer/GoodsExManage.h"
#include "AppServer/PlayerDepot.h"


void OnGoodsMessage(CMessage *pMsg)
{	
	if( pMsg == NULL ) return;

	CPlayer* pPlayer = pMsg -> GetPlayer();
	if( pPlayer == NULL ) return;
	if( pPlayer -> IsDied() ) return;

	CServerRegion* pRegion = pMsg -> GetRegion();
	if( pRegion == NULL ) return;

	switch(pMsg->GetType()) 
	{
	case MSG_C2S_EXCHANGEHOTKEY://�������һ������/��Ʒ�Ŀ�ݼ�(���λ������Ʒ OR ����)
		{
			int nIndex=pMsg->GetByte();
			DWORD dwGoodIndex=pMsg->GetDWord();

			// ����
			if (dwGoodIndex&0x80000000)
			{
				
				pPlayer->SetHotKey(nIndex, dwGoodIndex);// GetHotKey()

				CMessage msg(MSG_S2C_EXCHANGEHOTKEY);
				msg.Add((char)HOTKEY_SET_SUCCEED);
				msg.Add((BYTE)nIndex);
				msg.Add(dwGoodIndex);
				msg.SendToPlayer(pPlayer -> GetExID());	
			}
			else	// ��Ʒ
			{				
				//��鱳�����Ƿ��д˱����Ʒ
				if(nIndex>=0 && nIndex<24)
				{						
				
					CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(dwGoodIndex);
					if(pProperty)
					{
						//�����������Ե���
						if(pProperty -> GetGoodsType() == CGoodsBaseProperties::GT_CONSUMABLE )
						{
							pPlayer->SetHotKey(nIndex,dwGoodIndex);
							CMessage msg(MSG_S2C_EXCHANGEHOTKEY);
							msg.Add((char)HOTKEY_SET_SUCCEED);
							msg.Add((BYTE)nIndex);
							msg.Add(dwGoodIndex);
							msg.SendToPlayer( pPlayer -> GetExID() );	
						}																				
					}
				}			
			}
		}
		break;
	case MSG_C2S_CLEARHOTKEY://����ȼ�����
		{
			int nIndex=pMsg->GetByte();
			if( nIndex<0 || nIndex>=24 )
			{
				//������
				CMessage msg(MSG_S2C_CLEARHOTKEY);
				msg.Add((char)HOTKEY_CLEAR_FAILED);
				msg.SendToPlayer(pPlayer -> GetExID());
				break;
			}
			
			if(pPlayer->GetHotKey(nIndex) == 0)
			{
				//������
				CMessage msg(MSG_S2C_CLEARHOTKEY);
				msg.Add((char)HOTKEY_CLEAR_FAILED);
				msg.SendToPlayer(pPlayer -> GetExID());
				break;
			}
			
			pPlayer->SetHotKey(nIndex,0);
			CMessage msg(MSG_S2C_CLEARHOTKEY);
			msg.Add((char)HOTKEY_CLEAR_SUCCEED);
			msg.Add((BYTE)nIndex);
			msg.SendToPlayer((pPlayer -> GetExID()));
			//������Ϣ

			
		}
		break;
	case MSG_C2S_SETHOTKEY://�����ȼ�����
		{
			int nIndex=pMsg->GetByte();
			DWORD dwKeyVal=pMsg->GetDWord();
			
			if(pPlayer->GetHotKey(nIndex))
			{
			
				pPlayer->SetHotKey(nIndex,dwKeyVal);
				//������Ϣ

				CMessage msg(MSG_S2C_SETHOTKEY);
				msg.Add((char)HOTKEY_EXCHANGE_SUCCEED);
				msg.Add((BYTE)nIndex);
				msg.Add(dwKeyVal);

				msg.SendToPlayer((pPlayer -> GetExID()));
			}

			
		}
		break;
	case MSG_C2S_VIEWEQUIP://�������鿴��һ����ҵ�װ��
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			CPlayer* pOtherPlayer=GetGame()->FindPlayer(PlayerID);
			BYTE lSize=0;
			if(pOtherPlayer)
			{
				
				lSize = pOtherPlayer->m_cEquipment.GetGoodsAmount();

				CMessage msg(MSG_S2C_VIEWEQUIP);
				msg.Add(pOtherPlayer->GetExID());		
				msg.Add(lSize);
				//ÿ��װ������
				for(int i=0;i<CEquipmentContainer::EC_TOTAL;i++)
				{
					if(pOtherPlayer->m_cEquipment.GetGoods((CEquipmentContainer::EQUIPMENT_COLUMN)i))
					{
						msg.Add((BYTE)i);
						vector<BYTE> vecData;
						pOtherPlayer->m_cEquipment.GetGoods((CEquipmentContainer::EQUIPMENT_COLUMN)i)->SerializeForOldClient(&vecData);
						if(vecData.size() > 0)
							msg.AddEx(&vecData[0],vecData.size());
					}
				}
				msg.Add(pOtherPlayer->GetOccuLvl((eOccupation)pOtherPlayer->GetOccupation()));
				msg.Add((BYTE)pOtherPlayer->GetConst());
				msg.SendToPlayer(pPlayer -> GetExID());
			}
		}
		break;
			
	// ִ����Ʒ�����ű�
	case MSG_C2S_GOODS_CONTAINER:
		{
			long nFlag = pMsg->GetByte();
			if(nFlag == 1)
			{
				if (strlen(pPlayer->GetLastContainerScript()) == 0)
					break;

				stRunScript st;
				st.pszFileName = (char*)pPlayer->GetLastContainerScript();
				st.desShape = pPlayer;
				st.pRegion = pRegion;
				st.srcShape = NULL;
				RunScript(&st, (char*)GetGame()->GetScriptFileData(pPlayer->GetLastContainerScript()));
			}
			else if(nFlag == 0)
			{
				
			}
		}
		break;
		//! �ͻ������������
	case MSG_C2S_ENCHASE_INPUT_PWD:
		{
			char szPwd[PASSWORD_SIZE] = {0};
			pMsg->GetStr(szPwd, PASSWORD_SIZE);

			CPlayerPwd::ePwdState PwdState = pPlayer->m_CardPwd.Unlock(szPwd);

			if(CPlayerPwd::ePS_Free == PwdState)
			{
				const CGUID& SessionID = CSessionFactory::CreateSession(1, 1);
				CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
				if( pSession && pSession -> Start() )
				{
					LONG lPlugID = CSessionFactory::CreatePlug( CSessionFactory::PT_EQUIPMENT_ENCHASE, TYPE_PLAYER, (pPlayer->GetExID() ));
					if( CSessionFactory::InsertPlug(SessionID, lPlugID) )
					{
						CMessage msg( MSG_S2C_OPENGOODSENCHASE );
						msg.Add((BYTE)TYPE_BACKOUT);
						msg.Add( SessionID );
						msg.Add( lPlugID );

						msg.SendToPlayer(pPlayer->GetExID());
					}
				}
			}
			else
			{
				CMessage msg(MSG_S2C_ENCHASE_RE_INPUT_PWD);
				msg.Add((DWORD)((CPlayerPwd::ePS_Frost == PwdState) ? 1 : 0));
				msg.SendToPlayer(pPlayer->GetExID());
			}
		}
		break;
		//! Ҫ���޸Ĳ�����
	case MSG_C2S_ENCHASE_REQUEST_CHANGE_PWD:
		{
			char szOldPwd[PASSWORD_SIZE] = {0};
			pMsg->GetStr(szOldPwd, PASSWORD_SIZE);
			char szNewPwd[PASSWORD_SIZE] = {0};
			pMsg->GetStr(szNewPwd, PASSWORD_SIZE);

			CPlayerPwd::ePwdState PwdState = pPlayer->m_CardPwd.ChangePassword(szOldPwd, szNewPwd);

			CMessage msg(MSG_S2C_ENCHASE_RE_CHANGE_PWD);
			msg.Add((DWORD)((CPlayerPwd::ePS_Free == PwdState) ? 1 : 0));
			msg.SendToPlayer(pPlayer->GetExID());
		}
		break;
	case MSG_C2S_ENCHASE_ENDSESSION:
		{
			CGUID SessionID;
			pMsg -> GetGUID(SessionID);
			long PlugID = pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession && pSession -> IsSessionAvailable() )
			{
				CEquipmentEnchase* pPlug = dynamic_cast<CEquipmentEnchase*>( pSession -> QueryPlugByID(PlugID) );
				if( pPlug && pPlug -> GetOwnerID() == pPlayer -> GetExID() )
				{
					pSession -> End();
				}
			}

			pPlayer->m_CardPwd.Lock();
		}
		break;
	case MSG_C2S_ENCHASE_REMOVECARD:
		{
			if(CPlayerPwd::ePS_Free != pPlayer->m_CardPwd.GetState())
				break;

			CGUID sessionId;
			pMsg->GetGUID(sessionId);
			LONG lPlugId=pMsg->GetLong();
			BYTE btPos=pMsg->GetByte();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(sessionId) );
			if(!pSession)
				return;
			CPlug* pPlug = pSession -> QueryPlugByID( lPlugId >> 8 );
			CEquipmentEnchase* pEnchasePlug=NULL;
			if( pPlug )
			{
				pEnchasePlug=dynamic_cast<CEquipmentEnchase*>(pPlug);
				if(pEnchasePlug)
				{
					//��Ҫ��Ƕ����װ��
					CGoods* pMainGoods=pEnchasePlug->GetGoodsByPos(CEquipmentEnchaseShadowContainer::EEPC_EQUIPMENT);
					if(!pMainGoods)
					{
						//�޷�ȡ��Ҫ��Ƕ����װ��
						return;
					}
					//ȡ�ñ�����Ʒ
					CGoods* pReserveGoods=pEnchasePlug->GetGoodsByPos(CEquipmentEnchaseShadowContainer::EEPC_YUANBAO_ITEM1);
					bool bReserve=false;
					DWORD dwReserveCard=0;
					if(pReserveGoods && pReserveGoods->HasAddonProperty(GAP_RESERVE_CARD))
					{
						if(pPlayer->GetTotalEmptySpaceNum()<1)
						{
							char* strRes=CStringReading::LoadString(24,7);
							if(strRes)
								pPlayer->SendNotifyMessage(strRes,NOTIFY_COLOR_RED,0);
							return;
						}

						bReserve=true;
						dwReserveCard=1;
					}
					CGoods* pCard=pMainGoods->GetEnchasedCard(btPos);
					if(!pCard)
						return;

					//! ����ǵ�ƥ��
					
					if(pReserveGoods)
					{
						DWORD CardStar = pCard->GetAddonPropertyValues(GAP_STARLEVEL, 1);
						DWORD MinReserveStar = pReserveGoods->GetAddonPropertyValues(GAP_STARLEVEL_RANGE, 1);
						DWORD MaxReserveStar = pReserveGoods->GetAddonPropertyValues(GAP_STARLEVEL_RANGE, 2);
						if(!Included_Close(CardStar, MinReserveStar, MaxReserveStar))
						{
							//! ��Ƭ�ǵ�����Ӱ����ƥ��
							pPlayer->SendNotifyMessage(CStringReading::LoadString(24,8), NOTIFY_COLOR_RED,0);
							return;
						}
					}

					DWORD dwCardGoodsId=pCard->GetBasePropertiesIndex();
					BOOL bRet=pMainGoods->RemoveCard(pPlayer,btPos,bReserve);
					CMessage Msg( MSG_S2C_ENCHASE_REMOVECARD );
					if(bRet)
					{
						//Removed Card Log
						//Enchase Log
						GetGameLogInterface()->logT021_goods_log_enchase(pPlayer,pMainGoods->GetBasePropertiesIndex(),pMainGoods->GetExID(),
							CGoodsFactory::QueryGoodsName(pMainGoods->GetBasePropertiesIndex()),1,btPos,
							dwCardGoodsId,CGoodsFactory::QueryGoodsName(dwCardGoodsId),dwReserveCard);


						Msg.Add((BYTE)1);
						Msg.Add(btPos);		
						//ɾ��������Ʒ
						if(pReserveGoods && bReserve)
						{
							CGoodsShadowContainer::tagGoodsShadow* pShadow=pEnchasePlug->GetShadowContainer()->GetGoodsShadowTag(pReserveGoods);
							if(pShadow)
								pPlayer->DelGoodsAndSendMsg(pShadow->lOriginalContainerExtendID,pShadow->dwOriginalGoodsPosition);

							//���ر���
							if(pCard)
								pPlayer->AddGoodsWithProtect(pCard,FALSE);
						}												
					}
					else 
						Msg.Add((BYTE)0);
					Msg.SendToPlayer( pPlayer -> GetExID() );

					if(bRet)
					{
						pMainGoods->EnchaseArrange();						
						DWORD dwActivedHoleNum=pMainGoods->GetActivedHoleNum();
						if(dwActivedHoleNum>=0)
						{
							CMessage arrangeMsg(MSG_S2C_ENCHASE_ARRANGE);							
							
							arrangeMsg.Add( (BYTE)1 ); //�����ʾ
							arrangeMsg.Add((BYTE)(pMainGoods->GetEnchasedCardNum()));
							for(int i=0;i<pMainGoods->GetMaxEnchaseHoleNum();i++)
							{
								CGoods* pCard=pMainGoods->GetEnchasedCard(i);
								if(pCard)
								{
									arrangeMsg.Add(pCard->GetExID());
								}
							}
							arrangeMsg.SendToPlayer( pPlayer -> GetExID() );
						}
					}
				}
			}
		}
		break;
	case MSG_C2S_ENCHASE_ENCHASECARD:
		{
			CGUID sessionId;
			pMsg->GetGUID(sessionId);
			LONG lPlugId=pMsg->GetLong();
			BYTE btNum=pMsg->GetByte();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(sessionId) );
			if(!pSession)
				return;
			CPlug* pPlug = pSession -> QueryPlugByID( lPlugId >> 8 );
			CEquipmentEnchase* pEnchasePlug=NULL;
			if( pPlug )
			{
				pEnchasePlug=dynamic_cast<CEquipmentEnchase*>(pPlug);
				if(pEnchasePlug)
				{
					//��Ҫ��Ƕ����װ��
					CGoods* pMainGoods=pEnchasePlug->GetGoodsByPos(CEquipmentEnchaseShadowContainer::EEPC_EQUIPMENT);
					if(!pMainGoods)
					{
						//�޷�ȡ��Ҫ��Ƕ����װ��
						return;
					}

					DWORD dwLev=pMainGoods->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT,1);
					//һ���⿨���ü���
					DWORD dwPrice=CKitCardRuleFactory::GetEnchasePrice(dwLev);
					//Ԥ���⿨�ܷ���
					DWORD dwTotalPrice = dwPrice * btNum;

					if(dwPrice<=0)
					{
						return;
					}

					if(pPlayer->GetMoney()<dwPrice)
					{
						char* strRes=CStringReading::LoadString(24,5);
						if(strRes)
						{
							pPlayer->SendNotifyMessage(strRes);
						}
						return;
					}
					

					struct stEnchaseInfo 
					{
						WORD wPos;//λ��
						DWORD dwGoodsId;//CARD ID
					};
					vector<stEnchaseInfo> vecEnchaseInfo;
					vecEnchaseInfo.clear();
					for(BYTE i=0;i<btNum;i++)
					{
						stEnchaseInfo enchaseInfo;
						enchaseInfo.wPos=pMsg->GetWord();
						enchaseInfo.dwGoodsId=pMsg->GetDWord();
						vecEnchaseInfo.push_back(enchaseInfo);						
					}

					//�ȼ��һ�ο�Ƭ��װ�������Ƿ�ƥ��
					CGoodsBaseProperties::EQUIP_TYPE etype=pMainGoods->QueryEquipType();
					for(size_t i=0;i<vecEnchaseInfo.size();i++)
					{
						CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(vecEnchaseInfo[i].dwGoodsId);
						if(pProperty)
						{

							DWORD addonVal1=pProperty->GetAddonPropertyValue(GAP_ENCHASE_TARGET,1);
							DWORD addonVal2=pProperty->GetAddonPropertyValue(GAP_ENCHASE_TARGET,2);
							if(!((CGoodsBaseProperties::ATTACK == etype && addonVal1 == 1) || 
								(CGoodsBaseProperties::DEFENSE == etype && addonVal2 == 1)
								)) 
							{
								//! �ϵ����Ͳ�ƥ�䣬���µ�ƥ��취����
								DWORD ExType = pMainGoods->GetAddonPropertyValues(GAP_WEAPON_CATEGORY, 1);
								if(0 == ExType)
									ExType = pMainGoods->GetAddonPropertyValues(GAP_GUARD_EQUIP_CATEGORY, 1);

								if(!CheckCardValue(pProperty->GetAddonPropertyValue(GAP_CARD_CATEGORY, 1), pMainGoods->GetGoodsBaseType(), ExType))
								{
									/*
									*��Ƭ��װ����ƥ��
									*/
									char* strRes=CStringReading::LoadString(24,1);
									if(strRes)
									{
										pPlayer->SendNotifyMessage(strRes);
									}								
									return;
								}
							}
						}
						else
						{
							return;
						}
					}
						
					
					DWORD dwTotalPriceCalc = 0;
					for(size_t i=0;i<vecEnchaseInfo.size();i++)
					{	
						//ȡ�ø�λ�õĿ�Ƭ
						CGoods* pCard=pEnchasePlug->GetGoodsByPos(vecEnchaseInfo[i].wPos+CEquipmentEnchaseShadowContainer::EEPC_CARD1);						
						if(pCard)
						{
							//�ȼ��Ӱ������
							CGoodsShadowContainer::tagGoodsShadow* pShadow=pEnchasePlug->GetShadowContainer()->GetGoodsShadowTag(pCard);
							if(pShadow)
							{
								LONG lCardExtendId=pShadow->lOriginalContainerExtendID;
								DWORD dwCardExtendPos=pShadow->dwOriginalGoodsPosition;
								CGoods* pOriginCard=pPlayer->FindGoods(lCardExtendId,dwCardExtendPos);
								if(!pOriginCard)
								{
									//��֮ǰ��λ������ұ������Ҳ����ÿ�Ƭ�ˣ���������Ƕǰ�����Ŀ�Ƭλ���б仯
									break;
								}

								if(pOriginCard->GetExID()!=pCard->GetExID())
								{
									//�ҵ���Ƭ����GUID��һ��
									break;
								}

								BOOL bRet=pMainGoods->Enchase(pPlayer,pCard,vecEnchaseInfo[i].wPos);
								if(bRet)
								{	
									//Enchase Log
									GetGameLogInterface()->logT021_goods_log_enchase(pPlayer,pMainGoods->GetBasePropertiesIndex(),pMainGoods->GetExID(),
										CGoodsFactory::QueryGoodsName(pMainGoods->GetBasePropertiesIndex()),0,vecEnchaseInfo[i].wPos,
										pCard->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pCard->GetBasePropertiesIndex()),0);


									DWORD dwCardCnt =  pCard->GetAmount();
									if ( dwCardCnt > 1 )
									{
										//��Ƭ����d
										CVolumeLimitGoodsContainer* pVolumeContainer =
											dynamic_cast<CVolumeLimitGoodsContainer*>( pPlayer->FindContainer(pShadow->lOriginalContainerExtendID) );
										if (  pVolumeContainer )
										{
											//�Ƴ�һ�ſ�Ƭ
											pVolumeContainer->Remove( pShadow->dwOriginalGoodsPosition, 1 );
											//�����޸���Ʒ������Ϣ
											CS2CContainerObjectAmountChange coacMsg;
											coacMsg.SetSourceContainer( pShadow->lOriginalContainerType,pShadow->OriginalContainerID,
												pShadow->dwOriginalGoodsPosition );
											coacMsg.SetSourceContainerExtendID( pShadow->lOriginalContainerExtendID );
											coacMsg.SetObject( pCard -> GetType(), pCard -> GetExID() );
											coacMsg.SetObjectAmount( pCard->GetAmount()  );
											coacMsg.Send( pPlayer->GetExID() );
											MP_DELETE(pShadow);	
										}
									}
									else
									{
										pPlayer->DelGoods(pShadow->lOriginalContainerExtendID,pShadow->dwOriginalGoodsPosition);
										//����ɾ����Ʒ����Ϣ
										CS2CContainerObjectMove comMsg;
										comMsg.SetSourceContainer(pShadow->lOriginalContainerType,pShadow->OriginalContainerID,
											pShadow->dwOriginalGoodsPosition);
										comMsg.SetSourceObject(pCard->GetType(),pCard->GetExID());
										comMsg.SetSourceObjectAmount(pCard->GetAmount());
										comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
										comMsg.SetSourceContainerExtendID(pShadow->lOriginalContainerExtendID);
										comMsg.Send(pPlayer->GetExID());									
										MP_DELETE(pShadow);		
									}
									//�۳������⿨����
									dwTotalPriceCalc += dwPrice;
									pPlayer->SetMoney(pPlayer->GetMoney()-dwPrice,17);
								}
								else
								{
									//�ÿ�Ƭ��Ƕʧ��
								}
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}

					
					assert( dwTotalPrice == dwTotalPriceCalc );
					CMessage Msg( MSG_S2C_ENCHASE_ENCHASECARD );											
					Msg.SendToPlayer( pPlayer -> GetExID() );

					pMainGoods->EnchaseArrange();

					
					DWORD dwMaxHoleNum = pMainGoods->GetMaxEnchaseHoleNum();
					DWORD dwEnchasedCardNum = pMainGoods->GetEnchasedCardNum();
					
					if ( dwEnchasedCardNum <= dwMaxHoleNum )
					{
						CMessage arrangeMsg(MSG_S2C_ENCHASE_ARRANGE);
						arrangeMsg.Add( (BYTE)0 ); //��Ƕ��ʾ
						arrangeMsg.Add( (BYTE)dwEnchasedCardNum );
						for ( int i = 0; i < dwMaxHoleNum; ++i )
						{
							CGoods* pCard = pMainGoods->GetEnchasedCard(i);
							if ( pCard )
							{
								arrangeMsg.Add( pCard->GetExID() );
							}
						}
						arrangeMsg.SendToPlayer( pPlayer -> GetExID() );
					}
				}
			}
		}
		break;
	case MSG_C2S_GOODS_CLOSE_EX_WND: //! ��Ʒ���ڹر�֪ͨ
		{
			GoodsExManage::GetInstance().OnClientCloseGoodsExWindow(pMsg);
		}
		break;
	
	case MSG_C2S_GOODS_DISASSEMBLE_DO: //! ����ֽ���Ʒ
		{
			GoodsExManage::GetInstance().OnGoodsDisassemble(pMsg);
		}
		break;
	case MSG_C2S_GOODS_SYNTHESIZE_PREVIEW: //! Ԥ���ϳɽ��
		{
			
		}
		break;
	case MSG_C2S_GOODS_SYNTHESIZE_DO: //! ����ϳ���Ʒ
		{
			GoodsExManage::GetInstance().OnGoodsSynthesize(pMsg);
		}
		break;
	case MSG_C2S_GOODS_BIND:	//! ��Ʒ�󶨣��������
		{
			GoodsExManage::GetInstance().OnGoodsBind(pMsg);
		}
		break;
	case MSG_C2S_GOODS_FROST:	//! ��Ʒ���ᣨ�ⶳ������
		{
			GoodsExManage::GetInstance().OnGoodsFrost(pMsg);
		}
		break;
	case MSG_C2S_GOODS_UPGRADE_VIEW: //! ���Ԥ������
		{
			GoodsExManage::GetInstance().OnEquipmentUpgradeView(pMsg);
		}
		break;
	case MSG_C2S_GOODS_UPGRADE_DO: //! ���ִ������
		{
			GoodsExManage::GetInstance().OnEquipmentUpgrade(pMsg);
		}
		break;
	case MSG_C2S_GOODS_PLUG_EVIL_DO: //! ħ��ִ������
		{
			GoodsExManage::GetInstance().OnEquipmentPlugEvil(pMsg);
		}
		break;
	case MSG_C2S_GOODS_RESET_ELEMENT_TYPE: //! ����ħ������
		{
			GoodsExManage::GetInstance().OnEquipmentResetElementType(pMsg);
		}
		break;
	case MSG_C2S_GOODS_RESET_XIANG_XING: //! ����ħ������
		{
			GoodsExManage::GetInstance().OnEquipmentResetXiangXing(pMsg);
		}
		break;
	case MSG_C2S_DEPOT_INPUT_PWD:								//! �ͻ�������ֿ�����
		{
			pMsg->GetPlayer()->GetPlayerDepot()->OnInputPwdRequest(pMsg);
		}
		break;
	case MSG_C2S_DEPOT_REQUEST_CLOSE:							//! Ҫ��رղֿ�
		{
			pMsg->GetPlayer()->GetPlayerDepot()->OnLockRequest(pMsg);
		}
		break;
	case MSG_C2S_DEPOT_REQUEST_CHANGE_PWD:						//! Ҫ���޸�����
		{
			pMsg->GetPlayer()->GetPlayerDepot()->OnChangePwdRequest(pMsg);
		}
		break;
	case MSG_C2S_DEPOT_REQUEST_BUY_SUBPACK:						//! Ҫ�����ӱ���
		{
			pMsg->GetPlayer()->GetPlayerDepot()->OnBuySubpackRequest(pMsg);
		}
		break;
		//! ���������Ʒ
	case MSG_C2S_GOODS_AllPurpose_RequestPush:
		{
			DWORD dwIdx = pMsg->GetDWord();
			CGUID GoodsGuid;
			pMsg->GetGUID(GoodsGuid);
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				GoodsAllPurposeOpt::eOptResult re = pPlayer->GetGoodsAllPurposeOpt().OnGoodsPut(GoodsGuid, dwIdx);
				if(GoodsAllPurposeOpt::eOR_Lawless == re)
					pPlayer->GetGoodsAllPurposeOpt().Release();
			}
		}
		break;
		//! ����ȡ����Ʒ
	case MSG_C2S_GOODS_AllPurpose_RequestPop:
		{
			DWORD dwIdx = pMsg->GetDWord();
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				GoodsAllPurposeOpt::eOptResult re = pPlayer->GetGoodsAllPurposeOpt().OnGoodsPop(dwIdx);
				if(GoodsAllPurposeOpt::eOR_Lawless == re)
					pPlayer->GetGoodsAllPurposeOpt().Release();
			}
		}
		break;
		//! ����ر�����
	case MSG_C2S_GOODS_AllPurpose_RequestClose:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				pPlayer->GetGoodsAllPurposeOpt().Release(FALSE);
			}
		}
		break;
		//! ����ִ����������
	case MSG_C2S_GOODS_AllPurpose_RequestFinish:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				GoodsAllPurposeOpt::eOptResult re = pPlayer->GetGoodsAllPurposeOpt().FinishOpt();
				if(GoodsAllPurposeOpt::eOR_Lawless == re)
					pPlayer->GetGoodsAllPurposeOpt().Release();
			}
		}
		break;
	}
}
