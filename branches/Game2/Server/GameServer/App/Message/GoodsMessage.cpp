#include "StdAfx.h"

#include "../Container/CEquipmentEnchaseShadowContainer.h"
#include "../Goods/CGoods.h"
#include "../GoodsExManage.h"
#include "../MessagePackage/CS2CContainerObjectMove.h"
#include "../MessagePackage/CS2CContainerObjectAmountChange.h"
#include "../Player.h"
#include "../PlayerDepot.h"
#include "../Session/CEquipmentEnchase.h"
#include "../Session/CSessionFactory.h"
#include "../Session/CSession.h"
#include "../ServerRegion.h"
//#include "../Script/Script.h"
#include "../LuaScript/Script.h"
#include "../LuaScript/ScriptSys.h"
#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/KitCardRuleSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
				//pPlayer->GetBaseProperty()->dwHotKey[nIndex]=;
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
					//pPlayer->GetBaseProperty()->dwHotKey[nIndex]=dwGoodIndex;
					CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(dwGoodIndex);
					if(pProperty)
					{
						//�����������Ե���
						if(pProperty -> GetGoodsBaseType() == GBT_CONSUMABLE )
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
			//if( pPlayer->GetBaseProperty()->dwHotKey[nIndex]==0 )
			if(pPlayer->GetHotKey(nIndex) == 0)
			{
				//������
				CMessage msg(MSG_S2C_CLEARHOTKEY);
				msg.Add((char)HOTKEY_CLEAR_FAILED);
				msg.SendToPlayer(pPlayer -> GetExID());
				break;
			}
			//pPlayer->GetBaseProperty()->dwHotKey[nIndex]=0;
			pPlayer->SetHotKey(nIndex,0);
			CMessage msg(MSG_S2C_CLEARHOTKEY);
			msg.Add((char)HOTKEY_CLEAR_SUCCEED);
			msg.Add((BYTE)nIndex);
			msg.SendToPlayer((pPlayer -> GetExID()));
			//������Ϣ

			//int nIndex=pMsg->GetByte();
			//if(nIndex>=0 && nIndex<=15 && pPlayer->GetBaseProperty()->dwHotKey[nIndex]!=0)
			//{
			//	pPlayer->GetBaseProperty()->dwHotKey[nIndex]=0;
			//	CMessage msg(MSG_S2C_CLEARHOTKEY);
			//	msg.Add((char)HOTKEY_CLEAR_SUCCEED);
			//	msg.Add((BYTE)nIndex);
			//	msg.SendToPlayer((CMySocket*)(pPlayer->GetSocket()));
			//	return;
			//}
			//CMessage msg(MSG_S2C_CLEARHOTKEY);
			//msg.Add((char)HOTKEY_CLEAR_FAILED);
			//msg.SendToPlayer((CMySocket*)(pPlayer->GetSocket()));
		}
		break;
	case MSG_C2S_SETHOTKEY://�����ȼ�����
		{
			int nIndex=pMsg->GetByte();
			DWORD dwKeyVal=pMsg->GetDWord();
			//if( pPlayer->GetBaseProperty()->dwHotKey[nIndex] )
			if(pPlayer->GetHotKey(nIndex))
			{
				//pPlayer->GetBaseProperty()->dwHotKey[nIndex]=dwKeyVal;
				pPlayer->SetHotKey(nIndex,dwKeyVal);
				//������Ϣ

				CMessage msg(MSG_S2C_SETHOTKEY);
				msg.Add((char)HOTKEY_EXCHANGE_SUCCEED);
				msg.Add((BYTE)nIndex);
				msg.Add(dwKeyVal);

				msg.SendToPlayer((pPlayer -> GetExID()));
			}

			//int nIndex=pMsg->GetByte();
			//DWORD dwVal=pMsg->GetDWord();
			//DWORD dwTemVal=pPlayer->GetBaseProperty()->dwHotKey[nIndex];
			//if(dwTemVal!=0)
			//{
			//	pPlayer->GetBaseProperty()->dwHotKey[nIndex]=dwVal;
			//	CMessage msg(MSG_S2C_SETHOTKEY);
			//	msg.Add((char)HOTKEY_EXCHANGE_SUCCEED);
			//	msg.Add((BYTE)nIndex);
			//	msg.Add(dwVal);

			//	msg.SendToPlayer((CMySocket*)(pPlayer->GetSocket()));
			//}
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
				//for(int i=0;i<6;i++)
				//{
				//	if(pOtherPlayer->m_cEquipment.GetGoods((eEquipType)i))
				//	{
				//		lSize++;
				//	}
				//}
				lSize = (BYTE)pOtherPlayer->m_cEquipment.GetGoodsAmount();

				CMessage msg(MSG_S2C_VIEWEQUIP);
				msg.Add(pOtherPlayer->GetExID());
				//msg.Add(pOtherPlayer->GetBaseProperty()->lHeadPic);
				//msg.Add(pOtherPlayer->GetBaseProperty()->lFacePic);
				msg.Add(pOtherPlayer->GetHeadPic());
				msg.Add(pOtherPlayer->GetFacePic());
				msg.Add(lSize);
				//ÿ��װ������
				for(int i=0;i<EC_TOTAL;i++)
				{
					if(pOtherPlayer->m_cEquipment.GetGoods((eEquipType)i))
					{
						msg.Add((BYTE)i);
						vector<BYTE> vecData;
						pOtherPlayer->m_cEquipment.GetGoods((eEquipType)i)->SerializeForOldClient(&vecData);
						if(vecData.size() > 0)
							msg.AddEx(&vecData[0],(long)vecData.size());
					}
				}
				msg.Add(pOtherPlayer->GetOccuLvl((eOccupation)pOtherPlayer->GetOccupation()));
		//		msg.Add((BYTE)pOtherPlayer->GetConst());
				msg.SendToPlayer(pPlayer -> GetExID());
			}
		}
		break;
		/*
	case MSG_C2S_CLOSEGOODSUPGRADE:
		{
			CGUID SessionID;
			pMsg -> GetGUID(SessionID);
			long PlugID = pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession )
			{
				CEquipmentUpgrade* pPlug = dynamic_cast<CEquipmentUpgrade*>( pSession -> 
					QueryPlugByOwner( pPlayer -> GetType(), pPlayer -> GetExID() ) );
				if( pPlug && pPlug -> Get32ID() == PlugID )
				{
					pPlug -> Exit();
				}
			}
		}
		break;
	case MSG_C2S_GOODSUPGRADE_UPGRADE://��Ұ�����������ť���е�������
		{
			CGUID SessionID;
			pMsg -> GetGUID(SessionID);
			long PlugID = pMsg->GetLong();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pSession )
			{
				CEquipmentUpgrade* pPlug = dynamic_cast<CEquipmentUpgrade*>( pSession -> 
					QueryPlugByOwner( pPlayer -> GetType(), pPlayer -> GetExID() ) );
				if( pPlug && pPlug -> Get32ID() == PlugID )
				{
					pPlug -> Upgrade();
				}
			}			
		}
		break;*/	
	// ִ����Ʒ�����ű�
	case MSG_C2S_GOODS_CONTAINER:
		{
			long nFlag = pMsg->GetByte();
			if(nFlag == 1)
			{
				if (strlen(pPlayer->GetLastContainerScript()) == 0)
					break;

				//stRunScript st;
				//st.pszFileName = (char*)pPlayer->GetLastContainerScript();
				//st.desShape = pPlayer;
				//st.pRegion = pRegion;
				//st.srcShape = NULL;
				//RunScript(&st, (char*)GetGame()->GetScriptFileData(pPlayer->GetLastContainerScript()));
				Script::RunEnv env( NULL, pPlayer, pRegion );
				GetInst( ScriptSys ).RunScript( env, pPlayer->GetLastContainerScript() );
			}
			else if(nFlag == 0)
			{
				
			}
		}
		break;
		//����Ƕ��
	case MSG_C2S_ENCHASE_OPEN_HOLE:
		break;
		//������Ƕ��
	case MSG_C2S_ENCHASE_CLOSE_HOLE:

		//��Ƕ�Ự����
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
		}
		break;
	case MSG_C2S_ENCHASE_REMOVECARD:
		{
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
						bReserve=true;
						if(pPlayer->GetTotalEmptySpaceNum()<1)
						{
							const char* strRes=AppFrame::GetText("7");
							if(strRes)
								pPlayer->SendNotifyMessage(strRes,eNOTIFYPOS_CENTER);
							return;
						}
						dwReserveCard=1;
					}
					CGoods* pCard=pMainGoods->GetEnchasedCard(btPos);
					if(!pCard)
						return;
					DWORD dwCardGoodsId=pCard->GetBasePropertiesIndex();
					BOOL bRet=pMainGoods->RemoveCard(pPlayer,btPos,bReserve);
					CMessage Msg( MSG_S2C_ENCHASE_REMOVECARD );
					if(bRet)
					{
						//Removed Card Log
						//Enchase Log
						GetLogicLogInterface()->logT021_goods_log_enchase(pPlayer,pMainGoods->GetBasePropertiesIndex(),pMainGoods->GetExID(),
							GoodsSetup::QueryGoodsName(pMainGoods->GetBasePropertiesIndex()),1,btPos,
							dwCardGoodsId,GoodsSetup::QueryGoodsName(dwCardGoodsId),dwReserveCard);


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
							//arrangeMsg.Add((BYTE)dwActivedHoleNum);
							arrangeMsg.Add( (BYTE)1 ); //�����ʾ
							arrangeMsg.Add((BYTE)(pMainGoods->GetEnchasedCardNum()));
							for(int i=0;i<(int)pMainGoods->GetMaxEnchaseHoleNum();i++)
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

					DWORD dwLev=pMainGoods->GetAddonPropertyValues(GAP_MIN_REINFORCE_LEVEL,1);
					//һ���⿨���ü���
					DWORD dwPrice=KitCardRuleSetup::GetEnchasePrice(dwLev);
					//Ԥ���⿨�ܷ���
					DWORD dwTotalPrice = dwPrice * btNum;

					if(dwPrice<=0)
					{
						return;
					}

					if(pPlayer->GetMoney()<dwPrice)
					{
						const char* strRes=AppFrame::GetText("5");
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
					//eEquipType etype=pMainGoods->QueryEquipType();
					for(size_t i=0;i<vecEnchaseInfo.size();i++)
					{
						CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(vecEnchaseInfo[i].dwGoodsId);
						if(pProperty)
						{
							DWORD addonVal1=pProperty->GetAddonPropertyValue(GAP_ENCHASE_TARGET,1);
							DWORD addonVal2=pProperty->GetAddonPropertyValue(GAP_ENCHASE_TARGET,2);
							//if((etype==ET_ATTACK && addonVal1!=1) || 
							//	(etype==ET_DEFENSE && addonVal2!=1))
							//{
							//	/*
							//	*��Ƭ��װ����ƥ��
							//	*/
							//	const char* strRes=AppFrame::GetText("1");
							//	if(strRes)
							//	{
							//		pPlayer->SendNotifyMessage(strRes);
							//	}								
							//	return;
							//}
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
									GetLogicLogInterface()->logT021_goods_log_enchase(pPlayer,pMainGoods->GetBasePropertiesIndex(),pMainGoods->GetExID(),
										GoodsSetup::QueryGoodsName(pMainGoods->GetBasePropertiesIndex()),0,vecEnchaseInfo[i].wPos,
										pCard->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pCard->GetBasePropertiesIndex()),0);


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
											OBJ_RELEASE(CGoodsShadowContainer::tagGoodsShadow, pShadow);	
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
										OBJ_RELEASE(CGoodsShadowContainer::tagGoodsShadow, pShadow);		
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

					//pPlayer->SetMoney(pPlayer->GetMoney()-dwPrice,17);
					assert( dwTotalPrice == dwTotalPriceCalc );
					CMessage Msg( MSG_S2C_ENCHASE_ENCHASECARD );											
					Msg.SendToPlayer( pPlayer -> GetExID() );

					pMainGoods->EnchaseArrange();

					//DWORD dwActivedHoleNum=pMainGoods->GetActivedHoleNum();
					DWORD dwMaxHoleNum = pMainGoods->GetMaxEnchaseHoleNum();
					DWORD dwEnchasedCardNum = pMainGoods->GetEnchasedCardNum();
					//if(dwActivedHoleNum>=0)
					if ( dwEnchasedCardNum <= dwMaxHoleNum )
					{
						CMessage arrangeMsg(MSG_S2C_ENCHASE_ARRANGE);
						arrangeMsg.Add( (BYTE)0 ); //��Ƕ��ʾ
						arrangeMsg.Add( (BYTE)dwEnchasedCardNum );
						for ( int i = 0; i < (int)dwMaxHoleNum; ++i )
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
	//case MSG_C2S_GOODS_DISASSEMBLE_PREVIEW://! Ԥ���ֽ���
	//	{
	//		GoodsExManage::GetInstance().OnDisassemblePreview(pMsg);
	//	}
	//	break;
	case MSG_C2S_GOODS_DISASSEMBLE_DO: //! ����ֽ���Ʒ
		{
			GoodsExManage::GetInstance().OnGoodsDisassemble(pMsg);
		}
		break;
	case MSG_C2S_GOODS_SYNTHESIZE_PREVIEW: //! Ԥ���ϳɽ��
		{
			//GoodsExManage::GetInstance().OnSynthesizePreview(pMsg);
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
