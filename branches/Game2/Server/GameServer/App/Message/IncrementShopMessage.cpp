// ��ֵ�̵ֻ꣬��������ֵ���ߣ���������������������������˰�ա�
// bugs

#include "StdAfx.h"

#include "../Player.h"
#include "../MessagePackage/CS2CContainerObjectMove.h"
#include "../MessagePackage/CS2CContainerObjectAmountChange.h"
#include "../Goods/CGoods.h"
#include "../Goods/CGoodsFactory.h"
#include "../Goods/IncrementShopManager.h"
#include "../Listener/CGoodsRepairListener.h"
#include "../Listener/CGoodsRepairPriceListener.h"
#include "../ServerRegion.h"
#include "../Listener/CSeekGoodsListener.h"

#include "../../Server/BillClient.h"
#include "../BillApp/BillTrade.h"

#include "../Container/CTestContainer.h"

#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ȡ������
void CancelIncrementTrading( CPlayer* pPlayer )
{
	pPlayer->SetCurrentProgress( CPlayer::PROGRESS_NONE );
	pPlayer->SetMoveable();
	CMessage msg( MSG_S2C_INCREMENTSHOP_CLOSE );
	msg.SendToPlayer( pPlayer->GetExID() );
}
//�������͹���
//bool IncPrsentProcessing(CPlayer* pPlayer, vector<tagSGoods> vtgSGoods, DWORD dwTradeMode, long &lTotalAmount, CGUID &MailGuid)
//{
//	CMail *pMail = GameManager::GetInstance()->GetMailManager()->GetMail(MailGuid);
//	if ((long)pPlayer->GetYuanbao()<lTotalAmount)
//	{
//		list<CGUID> lMailID;
//		lMailID.push_back(MailGuid);
//		GameManager::GetInstance()->GetMailManager()->ReleaseMail(lMailID);
//		GameManager::GetInstance()->GetMailManager()->SendMailFailedToClient(pPlayer->GetExID(),MAIL_SENDING_FAILED_INVALID_GOODS);
//		return false;
//	}
//	if (pMail)
//	{
//		vector<CGoods*> vGoods;
//		vector<tagSGoods>::iterator it=vtgSGoods.begin();
//		for (;it!=vtgSGoods.end();++it)
//		{
//			//����Ҫ�������ƷVECTOR			
//			CGoodsFactory::CreateGoods((*it).lIndex,(*it).lNum, vGoods,30);
//		}
//		//pMail->AddSGoodsToContainer(vGoods);
//		//GameManager::GetInstance()->GetMailManager()->SendPMail(MailGuid,pPlayer,1);		
//		return true;
//	}
//	return false;
//}

// ������
bool IncShopProcessing( CPlayer* pPlayer, DWORD dwGoodsIndex, DWORD dwNum, DWORD dwTradeMode, long lTotalAmount )
{
	if(!pPlayer)
		return false;

	vector<CGoods*> vGoods;
	//����Ҫ�������ƷVECTOR
	CGoodsFactory::CreateGoods( dwGoodsIndex, dwNum, vGoods,31 );
	if( !vGoods.empty() )
	{	
		CTestContainer testContainer;
		testContainer.Init(pPlayer);
		vector<tagTestResult> vecResult;
		BOOL bSucced=testContainer.AddTest(vGoods,&vecResult);		

		if(bSucced)
		{
			//ȫ����Ʒ���Է���			
			if(vecResult.size() == vGoods.size())
			{
				for(int i=0;i<(int)vecResult.size();i++)
				{
					CVolumeLimitGoodsContainer* pVolumeContainer=
						dynamic_cast<CVolumeLimitGoodsContainer*>(pPlayer->FindContainer(vecResult[i].dwContainerId));
					if(pVolumeContainer)
					{
						if(vecResult[i].eOperType == CS2CContainerObjectMove::OT_NEW_OBJECT)
						{
							//�������Ʒ							
							CS2CContainerObjectMove comMsg;
							if(pVolumeContainer->Add(vecResult[i].dwPos, vGoods[i], NULL, &comMsg) )
							{
								pPlayer->OnGetGoods(dwGoodsIndex,vGoods[i]);
								DWORD dwGoodsId=vGoods[i]->GetBasePropertiesIndex();
								CGUID goodsGuid=vGoods[i]->GetExID();
								const char* strGoodsName=GoodsSetup::QueryGoodsName(dwGoodsId);
								DWORD dwGoodsNum=vGoods[i]->GetAmount();
								DWORD dwRemainWeimain =  pPlayer->GetGoodsAmountByOriginName( 
									GoodsSetup::QueryGoodsOriginalName( 
									GlobalSetup::GetSetup()->dwWeimianGoodsId) );
								DWORD dwRemainTicket=pPlayer->GetTicket();
								if(dwTradeMode==CIncrementShopList::TM_TICKET)
								{
									dwRemainTicket-=lTotalAmount;
								}
								else if ( dwTradeMode == CIncrementShopList::TM_WEIMIAN )
								{
									dwRemainWeimain -= lTotalAmount;
								}

								GetLogicLogInterface()->logT501_Ticket_consume_log(pPlayer,dwGoodsId,goodsGuid,strGoodsName,lTotalAmount,
									dwGoodsNum,dwRemainWeimain,dwRemainTicket,dwTradeMode);
								comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
								comMsg.SetDestinationContainerExtendID( vecResult[i].dwContainerId );
								vGoods[i]->SerializeForOldClient( &comMsg.GetObjectStream() );
								comMsg.Send( pPlayer->GetExID() );
							}
						}
						else if(vecResult[i].eOperType == CS2CContainerObjectMove::OT_CHANGE_AMOUNT)						
						{
							//�ı���Ʒ����
							DWORD dwGoodsId=vGoods[i]->GetBasePropertiesIndex();
							CGUID goodsGuid=vGoods[i]->GetExID();
							const char* strGoodsName=GoodsSetup::QueryGoodsName(dwGoodsId);
							DWORD dwGoodsNum=vGoods[i]->GetAmount();
							DWORD dwRemainWeimain =  pPlayer->GetGoodsAmountByOriginName( 
								GoodsSetup::QueryGoodsOriginalName( 
								GlobalSetup::GetSetup()->dwWeimianGoodsId) );
							DWORD dwRemainTicket=pPlayer->GetTicket();
							if(dwTradeMode==CIncrementShopList::TM_TICKET)
							{
								dwRemainTicket-=lTotalAmount;
							}
							else if ( dwTradeMode == CIncrementShopList::TM_WEIMIAN )
							{
								dwRemainWeimain -= lTotalAmount;
							}

							if(pVolumeContainer->Add(vecResult[i].dwPos, vGoods[i], NULL) )
							{
								pPlayer->OnGetGoods(dwGoodsId);
								GetLogicLogInterface()->logT501_Ticket_consume_log(pPlayer,dwGoodsId,goodsGuid,strGoodsName,lTotalAmount,
									dwGoodsNum,dwRemainWeimain,dwRemainTicket,dwTradeMode);
								CGoods* pExistedGoods=pVolumeContainer->GetGoods(vecResult[i].dwPos);
								if(pExistedGoods)
								{
									CS2CContainerObjectAmountChange coacMsg;
									coacMsg.SetSourceContainer( TYPE_PLAYER, pPlayer->GetExID(), 
										vecResult[i].dwPos );
									coacMsg.SetSourceContainerExtendID( vecResult[i].dwContainerId );
									coacMsg.SetObject( pExistedGoods->GetType(), pExistedGoods->GetExID() );
									coacMsg.SetObjectAmount( pExistedGoods->GetAmount() );
									coacMsg.Send( pPlayer->GetExID() );
								}								
							}
						}
					}
				}
				//�����������¼
				pPlayer->Add2IncShopCur10(  dwTradeMode, dwGoodsIndex );
				//��Ӹ��˹�����ϸ��¼
				pPlayer->Add2IncTradeList( dwTradeMode, dwGoodsIndex, dwNum, lTotalAmount );
				//���¸������������¼
				long lAllowBuyNum = CIncrementShopList::GetLimitNumByID( dwTradeMode, dwGoodsIndex );
				if ( lAllowBuyNum != -1 )
				{
					pPlayer->AddLimitGoodsRecord( dwGoodsIndex, dwNum );
					DWORD dwAlreadyBuyNum = pPlayer->GetLimitGoodsBuyNum( dwGoodsIndex );

					CMessage msg(MSG_S2C_INCREMENTSHOP_UPDATA);
					msg.Add( (BYTE)0 );												//ˢ�±�ʾ��0:�����޹�1:��������
					msg.Add( (BYTE)dwTradeMode );									//�̵�����
					CIncrementShopList::MAP_INCGOODS* pIncGoodsList = CIncrementShopList::GetIncGoodsList();
					if ( !pIncGoodsList ) return false;
					CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwGoodsIndex );
					if ( !pProperties ) return false;
					CIncrementShopList::ITR_INCGOODS itIncGoodsList = pIncGoodsList->find( dwTradeMode );
					CIncrementShopList::MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
					CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( dwGoodsIndex );
					if ( itInc != mapIncGoodsList.end() )
					{
						msg.Add( itInc->second.dwGoodsType );						//��Ʒ����
						assert( dwGoodsIndex == itInc->second.dwGoodsIndex );
						msg.Add( itInc->second.dwGoodsIndex );						//���ID
					}
					msg.Add( DWORD(lAllowBuyNum - dwAlreadyBuyNum) );				//ʣ��ɹ�������
					msg.SendToPlayer( pPlayer->GetExID() );
				}
				return true;
			}
			else
			{
				//���ý��SIZE �� ��ƷSIZE��һ��
				for( size_t i = 0; i < vGoods.size(); i ++ )
				{
#ifdef _GOODSLOG1_
					if(vGoods[i])
					{
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						vGoods[i]->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:IncShopProcessing][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							vGoods[i]->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(vGoods[i]->GetBasePropertiesIndex()),srcGoodsGUID,vGoods[i]->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
					}					
#endif
					CGoodsFactory::GarbageCollect( &vGoods[i] ,44);
				}
				vGoods.clear();
				return false;
			}
		}
		else
		{
			//����ʧ��
			for( size_t i = 0; i < vGoods.size(); i ++ )
			{
#ifdef _GOODSLOG1_
				if(vGoods[i])
				{
					char pszGoodsLog[1024]="";			
					char srcGoodsGUID[50]="";
					vGoods[i]->GetExID().tostring(srcGoodsGUID);					
					_snprintf(pszGoodsLog,1024,"[��Ʒ����:IncShopProcessing:����ʧ��][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						vGoods[i]->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(vGoods[i]->GetBasePropertiesIndex()),srcGoodsGUID,vGoods[i]->GetAmount());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
				}				
#endif
				CGoodsFactory::GarbageCollect( &vGoods[i] ,44);
			}
			vGoods.clear();
			return false;
		}
	}
	return false;
}

//����
void ProcessIncrementTrading( CPlayer* pPlayer, DWORD dwGoodsIndex, DWORD dwBuyNum, DWORD dwTradeMode )
{
	if(!pPlayer)
		return;

	CIncrementShopList::MAP_INCGOODS* pIncGoodsList = CIncrementShopList::GetIncGoodsList();
	if ( !pIncGoodsList ) return;
	CIncrementShopList::ITR_INCGOODS itIncGoodsList = pIncGoodsList->find( dwTradeMode );
	CIncrementShopList::MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
	CIncrementShopList::ITR_INCSHOP it = mapIncGoodsList.find( dwGoodsIndex );
	assert( it->first == dwGoodsIndex );

	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwGoodsIndex );
	if ( !pProperties ) return;

	long lTotalAmount(0);					// ������Ԫ��/��ȯ���

	//����
	switch( dwTradeMode )
	{
	case CIncrementShopList::TM_YUANBAO:
		{									
			// ������ɼƷ����Ĵ���
			lTotalAmount = it->second.dwPrice * dwBuyNum * it->second.dwDiscount / 10;

			//dwBuyNum*=it->second.dwGoodsNum;
			if(GetGame()->GetSetup()->lUseBillingServer==BILL_CONNECT_XM)
			{
				CMessage msg(MSG_S2B_ACCOUNT_LOCK);
				msg.Add((BYTE)eICT_NORMAL_BUY_ITEM);		//��������
				msg.Add(pPlayer->GetExID());				//player guid
				msg.Add(pPlayer->GetAccount());				//player cdkey
				msg.Add(NULL_GUID);							//attach guid
				msg.Add((DWORD)1);
				msg.Add(dwGoodsIndex);						//goods id
				msg.Add(dwBuyNum);							//goods num
				msg.Add(lTotalAmount);						//money sum
				msg.SendToBS();
			}
			else if(GetGame()->GetSetup()->lUseBillingServer==BILL_CONNECT_SNDA)
			{
				if(GetGame()->IsSDOAValid())
				{
					vector<CGoods*> vTestGoods;
					//����Ҫ�������ƷVECTOR
					CGoodsFactory::CreateGoods( dwGoodsIndex,it->second.dwGoodsNum * dwBuyNum, vTestGoods,278 );
					if( !vTestGoods.empty() )
					{	
						CTestContainer testContainer;
						testContainer.Init(pPlayer);
						vector<tagTestResult> vecResult;
						BOOL bSucced=testContainer.AddTest(vTestGoods,&vecResult);	
						for(size_t i=0;i<vTestGoods.size();i++)
						{
							OBJ_RELEASE(CGoods, vTestGoods[i]);
						}
						vTestGoods.clear();
						if(!bSucced)
						{
							const char* strRes=AppFrame::GetText("3");
							if(strRes)
								pPlayer->SendNotifyMessage(strRes);									
							return;
						}
					}
					else
						return;
					/////////////
					ISDOAOrderHandler* pOrderHandler=GetGame()->GetSDOAOrderHandler();
					ULONG orderObj=pOrderHandler->CreateOrderInfo(6);
					if(orderObj==0)
						return;
					char szOrderId[33];
					char szData[128];
					char szURL[2048];
					char szGoodsInfoEx[512];
					ZeroMemory(szOrderId,33);
					ZeroMemory(szData,128);
					ZeroMemory(szURL,2048);
					ZeroMemory(szGoodsInfoEx,512);
					//���ɶ���Ψһ���
					pOrderHandler->GetUniqueId(szOrderId);
					//��д��������
					pOrderHandler->SetOrderInfoValue(orderObj,"orderId",szOrderId);
					pOrderHandler->SetOrderInfoValue(orderObj,"userId",pPlayer->GetAccount());
					pOrderHandler->SetOrderInfoValue(orderObj,"roleName","");
					//payInfo
					_snprintf(szData, 128, "3|%d",lTotalAmount);
					pOrderHandler->SetOrderInfoValue(orderObj,"payInfo",szData);
					//feelInfo							
					pOrderHandler->SetOrderInfoValue(orderObj,"feeInfo","");
					//goodsNum
					pOrderHandler->SetOrderInfoValue(orderObj,"goodsNum","1");
					//goodsInfoEx
					ZeroMemory(szGoodsInfoEx,512);
					_snprintf(szGoodsInfoEx, 512, "%d,%s,%d,%d",dwGoodsIndex,pProperties->GetName(),dwBuyNum,lTotalAmount);
					pOrderHandler->SetOrderInfoValue(orderObj,"goodsInfoEx",szGoodsInfoEx);
					//notifyAddrType
					pOrderHandler->SetOrderInfoValue(orderObj,"notifyAddrType","3");
					//notifyAddr
					pOrderHandler->SetOrderInfoValue(orderObj,"notifyAddr","");
					//postBackUrl
					pOrderHandler->SetOrderInfoValue(orderObj,"postBackUrl","");
					//callTime
					ZeroMemory(szData,128);
					SYSTEMTIME stTime;
					GetLocalTime(&stTime);
					/*
					_snprintf(szData,"%d-%02d-%02d %02d:%02d:%02d",stTime.wYear,stTime.wMonth,stTime.wDay,
					stTime.wHour,stTime.wMinute,stTime.wSecond);
					pOrderHandler->SetOrderInfoValue(orderObj,"callTime",szData);*/
					//sessionId
					pOrderHandler->SetOrderInfoValue(orderObj,"sessionId","0");
					//indication
					pOrderHandler->SetOrderInfoValue(orderObj,"indication","0");
					//responseType
					pOrderHandler->SetOrderInfoValue(orderObj,"responseType","1");
					//remark
					pOrderHandler->SetOrderInfoValue(orderObj,"remark","");

					pOrderHandler->SetOrderInfoValue(orderObj,"reserveInt","0");
					pOrderHandler->SetOrderInfoValue(orderObj,"reserveString","");

					//����ǩ��URL
					pOrderHandler->BuildOrderUrl(orderObj,szURL);
					CGoodsFactory::PushPendingOrder(szOrderId,pPlayer->GetExID());

					//URL���͸��ͻ���
					CMessage msg(MSG_S2C_OTHER_BILLURL);
					msg.Add(szURL);
					msg.SendToPlayer(pPlayer->GetExID());
					//����order����
					pOrderHandler->DestroyOrderInfo(orderObj);
					Log4c::Trace(ROOT_MODULE,"logT521_snda_url_send_log");
					GetLogicLogInterface()->logT521_snda_url_send_log(pPlayer,szOrderId,lTotalAmount,szGoodsInfoEx);
				}
			}					
		}
		break;
	case CIncrementShopList::TM_TICKET:
		{
			if(it->second.dwPrice==0)
				return;
			//�����ܼ�
			lTotalAmount = it->second.dwPrice * dwBuyNum * it->second.dwDiscount / 10;
			if( pPlayer->GetTicket() < lTotalAmount)
			{
				char strTemp[256] = "";
				const char* strRes=AppFrame::GetText("20");
				_snprintf( strTemp, 256, strRes, lTotalAmount);
				if( strTemp && strRes )
					pPlayer->SendNotifyMessage(strTemp);
				return;
			}

			dwBuyNum*=it->second.dwGoodsNum;
			bool bConfirm = IncShopProcessing( pPlayer, it->second.dwGoodsIndex, dwBuyNum, dwTradeMode, lTotalAmount );
			if( bConfirm )
			{
				// �۳���ȯ
				if( lTotalAmount )
					pPlayer->SetTicket( pPlayer->GetTicket() - lTotalAmount );
			}
			else
			{
				const char* strRes=AppFrame::GetText("3");
				if(strRes)
					pPlayer->SendNotifyMessage(strRes);
			}
		}
		break;
	case CIncrementShopList::TM_WEIMIAN:
		{
			lTotalAmount = it->second.dwPrice * dwBuyNum * it->second.dwDiscount / 10;
			if ( lTotalAmount < 0 ) return;
			//���λ���Ƿ��㹻
			if ( (long)pPlayer->GetGoodsAmountByOriginName( 
				GoodsSetup::QueryGoodsOriginalName( 
				GlobalSetup::GetSetup()->dwWeimianGoodsId ) )  < lTotalAmount )
			{
				//λ�澫������,�޷�����
				const char* strRes=AppFrame::GetText("23");
				if(strRes)
					pPlayer->SendNotifyMessage(strRes);
				return;
			}

			//�����Ʒ��Ӽ�⣬λ���Ƴ����
			vector<CGoods*> vTestGoods;
			CGoodsFactory::CreateGoods( dwGoodsIndex,it->second.dwGoodsNum *dwBuyNum, vTestGoods,599 );
			vector<tagDelTestResult> vecDelResult;
			if( !vTestGoods.empty() )
			{	
				CTestContainer testContainer;
				testContainer.Init(pPlayer);
				vector<tagTestArg_ID_Num> vecDelGoodsForBuyer;
				tagTestArg_ID_Num DelTest;
				DelTest._dwGoodsID = GlobalSetup::GetSetup()->dwWeimianGoodsId;//180
				DelTest._dwNum = lTotalAmount;
				vecDelGoodsForBuyer.push_back( DelTest );
				BOOL bDelSucced = testContainer.DelTest( vecDelGoodsForBuyer, &vecDelResult );
				BOOL bAddSucced = testContainer.AddTest(vTestGoods);
				for(size_t i=0;i<vTestGoods.size();i++)
				{
					OBJ_RELEASE(CGoods, vTestGoods[i]);
				}
				vTestGoods.clear();
				if ( !bDelSucced ) return;
				if( !bAddSucced )
				{
					const char* strRes=AppFrame::GetText("3");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);									
					return;
				}
			}

			//�������֧����λ�澫���Ƴ�����
			for ( int i = 0; i < (int)vecDelResult.size(); ++i )
			{
				CVolumeLimitGoodsContainer* pVolumeContainer =
					dynamic_cast<CVolumeLimitGoodsContainer*>( pPlayer->FindContainer(vecDelResult[i].dwContainerId) );
				if(pVolumeContainer)
				{
					if( vecDelResult[i].eOperType == CS2CContainerObjectMove::OT_DELETE_OBJECT)
					{
						//ɾ��
						CGUID goodsGuid = pVolumeContainer->GetCellGuid().at( vecDelResult[i].dwPos );
						CGoods* pGoods = 
							dynamic_cast<CGoods*>( pVolumeContainer->Remove( goodsGuid ) );

						//����ɾ����Ʒ����Ϣ
						CS2CContainerObjectMove comMsg;
						comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
						comMsg.SetSourceContainer(TYPE_PLAYER,pPlayer->GetExID(),
							vecDelResult[i].dwPos );
						comMsg.SetSourceObject( pGoods->GetType(),pGoods->GetExID() );
						comMsg.SetSourceObjectAmount( pGoods->GetAmount() );
						comMsg.SetSourceContainerExtendID( vecDelResult[i].dwContainerId );
						comMsg.Send( pPlayer->GetExID() );
						if ( pGoods )
						{
							CGoodsFactory::GarbageCollect(&pGoods,222);
							GetLogicLogInterface()->logT504_weimian_log( pPlayer, vecDelResult[i].dwNum, 1, 1 );
						}
						else
						{
							return;
						}
					}
					else if ( vecDelResult[i].eOperType == CS2CContainerObjectMove::OT_CHANGE_AMOUNT )
					{
						//�޸�����
						CGUID goodsGuid = pVolumeContainer->GetCellGuid().at( vecDelResult[i].dwPos );
						CGoods* pGoods =pVolumeContainer->GetGoods( vecDelResult[i].dwPos );
						LONG lGoodsNum = pGoods->GetAmount() - vecDelResult[i].dwNum;
						pGoods->SetAmount( lGoodsNum );
						if ( pGoods )
						{
							CS2CContainerObjectAmountChange coacMsg;
							coacMsg.SetSourceContainer( TYPE_PLAYER, pPlayer->GetExID(), 
								vecDelResult[i].dwPos );
							coacMsg.SetSourceContainerExtendID( vecDelResult[i].dwContainerId );
							coacMsg.SetObject( pGoods->GetType(), pGoods->GetExID() );
							coacMsg.SetObjectAmount( lGoodsNum );
							coacMsg.Send( pPlayer->GetExID() );
							GetLogicLogInterface()->logT504_weimian_log( pPlayer, vecDelResult[i].dwNum, 1, 2 );
						}
					}
				}
			}

			dwBuyNum*=it->second.dwGoodsNum;
			bool bConfirm = IncShopProcessing( pPlayer, it->second.dwGoodsIndex, dwBuyNum, dwTradeMode, lTotalAmount );
		}
		break;
	default:
		{
			const char* strRes=AppFrame::GetText("21");
			if(strRes)
				pPlayer->SendNotifyMessage(strRes);
			return;
		}
		break;
	}
}

int SNDACALLBACK MsgHandle(int msgType, ISDOAMsg *req, ISDOAMsg *res) 
{ 
	//�������ʹ���ص���Ϣ,�������RecvMsgCallbackʾ����ISDOAMsg�ӿ�
	static int nCallTimes=0;
	switch(msgType)
	{
	case SDOA_ITEM_SEND_REQUEST:
		{	
			//orderId
			const char* szOrderId=req->GetValue("orderId");

			if(szOrderId)
			{
				if(CGoodsFactory::IsOrderExist(szOrderId))
					return 0;
			}
			else
				return 0;

			//payStatus
			const char* szPayStatus=req->GetValue("payStatus");
			if(!szPayStatus)
				return 0;
			int nErrorCode=atoi(szPayStatus);

			//sendTimes
			const char* szSendTimes=req->GetValue("sendTimes");

			//userId
			const char* szUserId=req->GetValue("userId");

			//roleName
			const char* szRoleName=req->GetValue("roleName");

			//goodsNum
			const char* szGoodsNum=req->GetValue("goodsNum");

			//goodsInfoEx
			const char* szGoodsInfoEx=req->GetValue("goodsInfoEx");

			//indication
			const char* szIndication=req->GetValue("indication");

			//payErrorMsg
			const char* payErrorMsg=req->GetValue("payErrorMsg");

			if(szUserId && szGoodsNum && szGoodsInfoEx)
			{
				nCallTimes++;
				char descMsg[64],appendix[256];
				_snprintf(descMsg, 64, "descMsg-%d",nCallTimes);
				_snprintf(appendix, 256, "%s",req->GetValue("indication"));

				if(nErrorCode!=0)
				{
					CGoodsFactory::ErasePendingOrder(szOrderId);
					res->SetValue("result","2");
					res->SetValue("orderId",req->GetValue("orderId"));
					res->SetValue("descMsg",descMsg);
					res->SetValue("appendix",appendix);
					return 0;
				}

				CPlayer* pPlayer=NULL;
				CGUID& playerGuid=CGoodsFactory::GetGUIDByOrderId(szOrderId);
				if(playerGuid==CGUID::GUID_INVALID)
				{
					res->SetValue("result","-10227000");
					res->SetValue("orderId",req->GetValue("orderId"));
					res->SetValue("descMsg",descMsg);
					res->SetValue("appendix",appendix);
					return 0;
				}
				else
				{
					pPlayer=GetGame()->FindPlayer(playerGuid);
					if(!pPlayer) //�Ҳ��������
					{
						res->SetValue("result","-10227000");
						res->SetValue("orderId",req->GetValue("orderId"));
						res->SetValue("descMsg",descMsg);
						res->SetValue("appendix",appendix);
						return 0;
					}
				}				

				//���nGoodsNum�Ƿ�Ϸ�
				int nGoodsNum=atoi(szGoodsNum);
				if(nGoodsNum!=1)
					return 0;

				//���goodsInfoEx�ĸ�ʽ
				string strGoodsInfoEx=szGoodsInfoEx;
				vector<string> goodsInfoArgs;
				size_t start=0,pos=0;

				do 
				{
					pos=strGoodsInfoEx.find_first_of(',',start);
					goodsInfoArgs.push_back(strGoodsInfoEx.substr(start,pos-start));
					start=pos+1;
				} 
				while(pos!=string::npos);
				if(goodsInfoArgs.size()!=4)
				{
					return 0;
				}

				int nGoodsId=atoi(goodsInfoArgs[0].c_str());
				int nNum=atoi(goodsInfoArgs[2].c_str());
				int nLockNum=atoi(goodsInfoArgs[3].c_str());

				CIncrementShopList::MAP_INCGOODS* pIncGoodsList = CIncrementShopList::GetIncGoodsList();
				CIncrementShopList::ITR_INCGOODS itIncGoodsList = pIncGoodsList->find( CIncrementShopList::TM_YUANBAO );
				CIncrementShopList::MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
				CIncrementShopList::ITR_INCSHOP it;
				if( !pIncGoodsList )
				{
					return 0;
				}
				else
				{
					it = mapIncGoodsList.find(nGoodsId);
					if( it == mapIncGoodsList.end() || !(&it->second) )
					{
						return 0;
					}
				}

				long lTotalAmount = it->second.dwPrice * nNum * it->second.dwDiscount / 10;
				bool bSucceed=IncShopProcessing(pPlayer,nGoodsId,nNum*it->second.dwGoodsNum,
					CIncrementShopList::TM_YUANBAO,lTotalAmount);				

				int retcode=0;
				if(bSucceed)
				{
					res->SetValue("result","0");
					CGoodsFactory::PushOrder(szOrderId,szGoodsInfoEx);
					CGoodsFactory::ErasePendingOrder(szOrderId);
				}
				else
				{
					retcode=-10227000;
					res->SetValue("result","-10227000");
					const char* strRes=AppFrame::GetText("3");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);
				}
				GetLogicLogInterface()->logT522_snda_ticket_consume_log(pPlayer,szOrderId,szSendTimes,szGoodsInfoEx,
					payErrorMsg,retcode);

				res->SetValue("orderId",req->GetValue("orderId"));
				res->SetValue("descMsg",descMsg);
				res->SetValue("appendix",appendix);
			}
			else
			{
				return 0;
			}
		}
		break;
	}
	return 0; 
}

void OnIncrementShopMessage(CMessage* pMsg)
{
	CPlayer* pPlayer = pMsg->GetPlayer();
	if( pPlayer == NULL )
	{
		return;
	}

	//����������رս���
	if( pPlayer->IsDied() )
	{
		CancelIncrementTrading( pPlayer );
		const char* strRes=AppFrame::GetText("16");
		if(strRes)
			pPlayer->SendNotifyMessage( strRes );
		return;
	}

	CServerRegion* pRegion = pMsg->GetRegion();
	if( pRegion == NULL )
	{
		CancelIncrementTrading( pPlayer );
		return;
	}

	switch(pMsg->GetType()) 
	{
	case MSG_C2S_INCREMENTSHOP_GETLOG_REQUEST:
		{
			CIncrementShopList::VEC_BUYLIST vecBuyList = pPlayer->GetIncTradeList();
			CMessage msg( MSG_S2C_INCREMENTSHOP_GETLOG_RESPONSE );
			msg.Add( (long)vecBuyList.size() );
			CIncrementShopList::RITR_BUYLIST itBuylist = vecBuyList.rbegin();
			for ( ; itBuylist != vecBuyList.rend(); ++itBuylist )
			{
				msg.Add( itBuylist->strBuyTime.c_str() );
				msg.Add( itBuylist->dwPriceType );
				msg.Add( itBuylist->dwPriceNum );
				msg.Add( itBuylist->dwBuyNum );
				msg.Add( itBuylist->dwGoodsIdx );
			}
			msg.SendToPlayer( pPlayer->GetExID() );
		}
		break;

	case MSG_C2S_INCREMENTSHOP_AFFICHE_REQUEST:
		{
			CMessage msg(MSG_S2C_INCREMENTSHOP_AFFICHE_RESPONSE);
			msg.Add( (long)CIncrementShopList::GetAffiche()->length() );
			msg.AddEx( (void*)CIncrementShopList::GetAffiche()->data(), (long)CIncrementShopList::GetAffiche()->length() );
			msg.SendToPlayer(pPlayer->GetExID());
		}
		break;

	case MSG_C2S_INCREMENTSHOP_OPEN:		// ����ֵ�̵�
		{
			if( pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_NONE )
			{
				CMessage msg(MSG_S2C_INCREMENTSHOP_OPEN);
				msg.Add((BYTE)0);			// ʧ��
				msg.SendToPlayer(pPlayer->GetExID());
				return;
			}

			pPlayer->ProduceEvent(ET_EventBreak);

			CIncrementShopList::MAP_TOP10* pTop10List = CIncrementShopList::GetTop10List();
			CIncrementShopList::MAP_NEWGOODS* pNewGoodsList = CIncrementShopList::GetNewGoodsList();
			CIncrementShopList::MAP_GUIDEGOODS* pGuideGoodsList = CIncrementShopList::GetGuideGoodsList();
			CIncrementShopList::MAP_FILTERGOODS* pFilterListOfWShop = CIncrementShopList::GetFilterListOfWShop();
			CIncrementShopList::MAP_FILTERGOODS* pFilterListOfXShop = CIncrementShopList::GetFilterListOfXShop();
			CIncrementShopList::MAP_FILTERGOODS* pFilterListOfTShop = CIncrementShopList::GetFilterListOfTShop();
			CIncrementShopList::MAP_INCGOODS* pIncGoodsList = CIncrementShopList::GetIncGoodsList();
			CIncrementShopList::MAP_INCCLASSGOODS* pIncClassGoodsList = CIncrementShopList::GetIncClassGoods();

			if ( !pTop10List || !pNewGoodsList || !pGuideGoodsList || !pFilterListOfWShop
				|| !pFilterListOfXShop|| !pFilterListOfTShop || !pIncGoodsList || !pIncClassGoodsList )
			{
				CMessage msg(MSG_S2C_INCREMENTSHOP_OPEN);
				msg.Add((BYTE)0);			// ʧ��
				msg.SendToPlayer(pPlayer->GetExID());
				return;
			}

			// ��ǰΪ�̵��������ƶ���
			pPlayer->SetCurrentProgress( CPlayer::PROGRESS_INCREMENT );
			pPlayer->SetMoveable( FALSE );

			CMessage msg(MSG_S2C_INCREMENTSHOP_OPEN);
			msg.Add((BYTE)1);				// �ɹ�

			BYTE byteFlag = pPlayer->GetIncShopFlag();
			msg.Add(byteFlag);

			// ��ҵ�½ʱ����������Ʒ��Ϣ
			if( byteFlag )
			{
				long lNum = (long)pTop10List->size();
				assert( lNum == 3 );
				msg.Add( (BYTE)lNum );//�̵����࣬ĿǰΪ�����̵꣨λ���̵� & �����̵� & ��ȯ�̵꣩
				for ( int i = 0; i < lNum; ++i )
				{
					DWORD dwShopType = i;
					CIncrementShopList::ITR_INCGOODS itIncGoodsList = pIncGoodsList->find( dwShopType );
					CIncrementShopList::ITR_INCCLASSGOODS itIncClassGoods = pIncClassGoodsList->find( dwShopType );
					CIncrementShopList::MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
					CIncrementShopList::MAP_CLASSGOODS& mapIncClassGoods = itIncClassGoods->second;

					//��ϸ��Ʒ�б�
					msg.Add( (long)mapIncGoodsList.size() );
					CIncrementShopList::ITR_CLASSGOODS iterIncGoods = mapIncClassGoods.begin();
					for ( ; iterIncGoods != mapIncClassGoods.end(); ++iterIncGoods )
					{
						CIncrementShopList::VEC_GOODSIDX& vecGoodsIdx = iterIncGoods->second;
						CIncrementShopList::ITR_GOODSIDX iter = vecGoodsIdx.begin();
						for ( ; iter != vecGoodsIdx.end(); ++iter )
						{
							CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iter );
							if(pProperties)
							{
								CIncrementShopList::ITR_INCSHOP iterIncGoods = mapIncGoodsList.find( *iter );
								if ( iterIncGoods != mapIncGoodsList.end() )
								{
									msg.Add( iterIncGoods->second.dwGoodsType );
									msg.Add( iterIncGoods->second.dwGoodsIndex );
									msg.Add( iterIncGoods->second.dwPrice );
									msg.Add( iterIncGoods->second.dwGoodsNum );
									msg.Add( (BYTE)iterIncGoods->second.dwDiscount );
									msg.Add( iterIncGoods->second.strTimeLimit.c_str() );
									if ( iterIncGoods->second.lQuantityLimit != -1 )
									{
										DWORD dwWorldAlreadyBuyNum = CIncrementShopManager::GetWorldLimitNumByGoodsID( *iter );
										DWORD dwNum = iterIncGoods->second.lQuantityLimit - dwWorldAlreadyBuyNum;
										msg.Add( dwNum );
									}
									else
									{
										msg.Add( iterIncGoods->second.lQuantityLimit );
									}
									if ( iterIncGoods->second.lBuyLimit != -1 )
									{
										DWORD dwAlreadyBuyNum = pPlayer->GetLimitGoodsBuyNum( *iter );
										DWORD dwNum = iterIncGoods->second.lBuyLimit - dwAlreadyBuyNum;
										msg.Add( dwNum );
									}
									else
									{
										msg.Add( iterIncGoods->second.lBuyLimit );
									}
								}
								else
									return;
							}
						}
					}

					//������Ʒ
					CIncrementShopList::ITR_TOP10 itTop10 = pTop10List->find( dwShopType );
					CIncrementShopList::VEC_GOODSIDX& vecTop10GoodsIdxList = itTop10->second;
					msg.Add( (long)vecTop10GoodsIdxList.size() );
					CIncrementShopList::ITR_GOODSIDX iterTop10GoodsIdx = vecTop10GoodsIdxList.begin();
					for ( ; iterTop10GoodsIdx != vecTop10GoodsIdxList.end(); ++iterTop10GoodsIdx )
					{
						CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iterTop10GoodsIdx );
						CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( *iterTop10GoodsIdx );
						if ( pProperties && itInc != mapIncGoodsList.end() )
						{
							assert( *iterTop10GoodsIdx == itInc->second.dwGoodsIndex );
							msg.Add( itInc->second.dwGoodsType );
							msg.Add( itInc->second.dwGoodsIndex );
						}
						else
						{
							return;
						}
					}

					//��Ʒ�Ƽ�
					CIncrementShopList::ITR_NEWGOODS itNewGoods = pNewGoodsList->find( dwShopType );
					CIncrementShopList::VEC_GOODSIDX& vecNewGoodsIdxList = itNewGoods->second;
					msg.Add( (long)vecNewGoodsIdxList.size() );
					CIncrementShopList::ITR_GOODSIDX iterNewGoodsIdx = vecNewGoodsIdxList.begin();
					for ( ; iterNewGoodsIdx != vecNewGoodsIdxList.end(); ++iterNewGoodsIdx )
					{
						CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iterNewGoodsIdx );
						CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( *iterNewGoodsIdx );
						if ( pProperties && itInc != mapIncGoodsList.end() )
						{
							assert( *iterNewGoodsIdx == itInc->second.dwGoodsIndex );
							msg.Add( itInc->second.dwGoodsType );
							msg.Add( itInc->second.dwGoodsIndex );
						}
						else
						{
							return;
						}
					}

					//������Ʒ�б�
					CIncrementShopList::ITR_GUIDEGOODSLIST itGuideGoods = pGuideGoodsList->find( dwShopType );
					CIncrementShopList::MAP_TYPEGOODS& mapTypeGoods = itGuideGoods->second;
					msg.Add( (long)mapTypeGoods.size() );
					CIncrementShopList::ITR_TYPEGOODS iterType = mapTypeGoods.begin();
					for ( ; iterType != mapTypeGoods.end(); ++iterType )
					{
						string strGuideType = iterType->first;
						msg.Add( strGuideType.c_str() );
						CIncrementShopList::VEC_GOODSIDX& vecGuideGoodsIdxList = iterType->second;
						msg.Add( (long)vecGuideGoodsIdxList.size() );
						CIncrementShopList::ITR_GOODSIDX iterGuideGoodsIdxList = vecGuideGoodsIdxList.begin();
						for ( ; iterGuideGoodsIdxList != vecGuideGoodsIdxList.end(); ++iterGuideGoodsIdxList )
						{
							CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iterGuideGoodsIdxList );
							CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( *iterGuideGoodsIdxList );
							if ( pProperties && itInc != mapIncGoodsList.end() )
							{
								assert( *iterGuideGoodsIdxList == itInc->second.dwGoodsIndex );
								msg.Add( itInc->second.dwGoodsType );
								msg.Add( itInc->second.dwGoodsIndex );
							}
							else
							{
								return;
							}
						}
					}

					//ɸѡ��Ʒ�б�
					if ( dwShopType == CIncrementShopList::TM_WEIMIAN )
					{
						msg.Add( (long)pFilterListOfWShop->size() );//��Ʒ����
						CIncrementShopList::ITR_FILTERTGOODSLIST iterFilter = pFilterListOfWShop->begin();
						for ( ; iterFilter != pFilterListOfWShop->end(); ++iterFilter )
						{
							DWORD dwGoodsType = iterFilter->first;
							CIncrementShopList::MAP_TYPEGOODS& mapTypeGoods = iterFilter->second;
							msg.Add( (long)mapTypeGoods.size() );
							CIncrementShopList::ITR_TYPEGOODS itFilterGoods = mapTypeGoods.begin();
							for ( ; itFilterGoods != mapTypeGoods.end(); ++itFilterGoods )
							{					
								string strFilterType = itFilterGoods->first;
								msg.Add( strFilterType.c_str() );
								CIncrementShopList::VEC_GOODSIDX& vecFilterGoodsIdxList = itFilterGoods->second;
								msg.Add( (long)vecFilterGoodsIdxList.size() );
								CIncrementShopList::ITR_GOODSIDX iterFilterGoodsIdxList = vecFilterGoodsIdxList.begin();
								for ( ; iterFilterGoodsIdxList != vecFilterGoodsIdxList.end(); ++iterFilterGoodsIdxList )
								{
									CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iterFilterGoodsIdxList );
									CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( *iterFilterGoodsIdxList );
									if ( pProperties && itInc != mapIncGoodsList.end() )
									{
										assert( *iterFilterGoodsIdxList == itInc->second.dwGoodsIndex );
										msg.Add( itInc->second.dwGoodsType );
										msg.Add( itInc->second.dwGoodsIndex );
									}
									else
									{
										return;
									}
								}
							}
						}
					}
					else if ( dwShopType == CIncrementShopList::TM_YUANBAO )
					{
						msg.Add( (long)pFilterListOfXShop->size() );//��Ʒ����
						CIncrementShopList::ITR_FILTERTGOODSLIST iterFilter = pFilterListOfXShop->begin();
						for ( ; iterFilter != pFilterListOfXShop->end(); ++iterFilter )
						{
							DWORD dwGoodsType = iterFilter->first;
							CIncrementShopList::MAP_TYPEGOODS& mapTypeGoods = iterFilter->second;
							msg.Add( (long)mapTypeGoods.size() );
							CIncrementShopList::ITR_TYPEGOODS itFilterGoods = mapTypeGoods.begin();
							for ( ; itFilterGoods != mapTypeGoods.end(); ++itFilterGoods )
							{
								string strFilterType = itFilterGoods->first;
								msg.Add( strFilterType.c_str() );
								CIncrementShopList::VEC_GOODSIDX& vecFilterGoodsIdxList = itFilterGoods->second;
								msg.Add( (long)vecFilterGoodsIdxList.size() );
								CIncrementShopList::ITR_GOODSIDX iterFilterGoodsIdxList = vecFilterGoodsIdxList.begin();
								for ( ; iterFilterGoodsIdxList != vecFilterGoodsIdxList.end(); ++iterFilterGoodsIdxList )
								{
									CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iterFilterGoodsIdxList );
									CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( *iterFilterGoodsIdxList );
									if ( pProperties && itInc != mapIncGoodsList.end() )
									{
										assert( *iterFilterGoodsIdxList == itInc->second.dwGoodsIndex );
										msg.Add( itInc->second.dwGoodsType );
										msg.Add( itInc->second.dwGoodsIndex );
									}
									else
									{
										return;
									}
								}
							}
						}
					}
					else if ( dwShopType == CIncrementShopList::TM_TICKET )
					{
						msg.Add( (long)pFilterListOfTShop->size() );//��Ʒ����
						CIncrementShopList::ITR_FILTERTGOODSLIST iterFilter = pFilterListOfTShop->begin();
						for ( ; iterFilter != pFilterListOfTShop->end(); ++iterFilter )
						{
							DWORD dwGoodsType = iterFilter->first;
							CIncrementShopList::MAP_TYPEGOODS& mapTypeGoods = iterFilter->second;
							msg.Add( (long)mapTypeGoods.size() );
							CIncrementShopList::ITR_TYPEGOODS itFilterGoods = mapTypeGoods.begin();
							for ( ; itFilterGoods != mapTypeGoods.end(); ++itFilterGoods )
							{
								string strFilterType = itFilterGoods->first;
								msg.Add( strFilterType.c_str() );
								CIncrementShopList::VEC_GOODSIDX& vecFilterGoodsIdxList = itFilterGoods->second;
								msg.Add( (long)vecFilterGoodsIdxList.size() );
								CIncrementShopList::ITR_GOODSIDX iterFilterGoodsIdxList = vecFilterGoodsIdxList.begin();
								for ( ; iterFilterGoodsIdxList != vecFilterGoodsIdxList.end(); ++iterFilterGoodsIdxList )
								{	
									CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iterFilterGoodsIdxList );
									CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( *iterFilterGoodsIdxList );
									if ( pProperties && itInc != mapIncGoodsList.end() )
									{
										assert( *iterFilterGoodsIdxList == itInc->second.dwGoodsIndex );
										msg.Add( itInc->second.dwGoodsType );
										msg.Add( itInc->second.dwGoodsIndex );
									}
									else
									{
										return;
									}
								}
							}
						}
					}
				}
				pPlayer->SetIncShopFlag();
			}

			//�̵���������б�
			long lListSize = (long)pPlayer->GetIncShopCur10().size();
			msg.Add( (BYTE)lListSize );
			CPlayer::IncShopCur10Iter iterCur10 = pPlayer->GetIncShopCur10().begin();
			for ( ; iterCur10 != pPlayer->GetIncShopCur10().end(); ++iterCur10 )
			{	
				DWORD dwShopType = iterCur10->first;
				msg.Add( (BYTE)dwShopType );
				list<DWORD>&  lstCur10 = iterCur10->second;
				msg.Add( (long)lstCur10.size() );
				list<DWORD>::iterator iter = lstCur10.begin();
				for ( ; iter != lstCur10.end(); ++iter )
				{
					CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( *iter );
					CIncrementShopList::ITR_INCGOODS itIncGoodsList = pIncGoodsList->find( dwShopType );
					CIncrementShopList::MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
					CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( *iter );
					if ( pProperties && itInc != mapIncGoodsList.end() )
					{
						msg.Add( itInc->second.dwGoodsType );
						msg.Add( itInc->second.dwGoodsIndex );
					}
				}
			}
			msg.SendToPlayer(pPlayer->GetExID());
		}
		break;

	case MSG_C2S_INCREMENTSHOP_BUY:			// ����������̵��й���һ����Ʒ
		{
			DWORD dwGoodsIndex	= pMsg->GetDWord();	// �������ƷID
			DWORD dwNum			= pMsg->GetDWord();	// ׼����������������û��������ֱ�ӷ��ء�
			DWORD dwTradeMode	= pMsg->GetDWord();	// ���׷�ʽ��Ԫ������ȯ��λ�棩 ���̵�����һ��
			if( dwNum == 0 ) return;
			if( dwTradeMode > CIncrementShopList::TM_TICKET ) return;

			CIncrementShopList::MAP_INCGOODS* pIncGoodsList = CIncrementShopList::GetIncGoodsList();
			CIncrementShopList::ITR_INCSHOP it;
			if ( !pIncGoodsList )
			{
				//��ʾ��ȡ��Ʒ�б�ʧ�ܣ��˳���
				const char* strRes=AppFrame::GetText("17");
				if(strRes)
					pPlayer->SendNotifyMessage(strRes);
				CancelIncrementTrading( pPlayer );
				return;
			}
			else
			{
				CIncrementShopList::ITR_INCGOODS itIncGoodsList = pIncGoodsList->find( dwTradeMode );
				if ( itIncGoodsList == pIncGoodsList->end() )
					return;
				CIncrementShopList::MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
				it = mapIncGoodsList.find( dwGoodsIndex );
				if( it == mapIncGoodsList.end() )
				{
					const char* strRes=AppFrame::GetText("18");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);
					return;
				}
			}
	
			CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwGoodsIndex );
			if( !pProperties )
			{
				const char* strRes=AppFrame::GetText("19");
				if(strRes)
					pPlayer->SendNotifyMessage(strRes);
				return;
			}

			// ����ȷ����������Ч��
			if( pProperties->GetGoodsBaseType() == GBT_EQUIPMENT )
			{
				// װ���������������Ϊ1
				//dwNum = 1;
			}

			//��ʱ���������޹��ж�
			//��ʱ�ж�
			std::string strTimeLimit = CIncrementShopList::GetLimitTimeByID( dwTradeMode, dwGoodsIndex );
			if ( strTimeLimit != "" )
			{
				if ( !CIncrementShopList::TimeLimited( strTimeLimit ) )
				{
					const char* strRes=AppFrame::GetText("25");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);
					CancelIncrementTrading( pPlayer );
					return;
				}
			}

			//���������ж�(����Ϊ-1��������Ʒû����������)
			long lAllowBuyNum = CIncrementShopList::GetLimitNumByID( dwTradeMode, dwGoodsIndex );
			if ( lAllowBuyNum != -1 )
			{						
				DWORD dwAlreadyBuyNum = pPlayer->GetLimitGoodsBuyNum( dwGoodsIndex );
				if ( (long)(dwAlreadyBuyNum+dwNum) > lAllowBuyNum )
				{
					const char* strRes=AppFrame::GetText("24");
					if(strRes)
						pPlayer->SendNotifyMessage(strRes);
					CancelIncrementTrading( pPlayer );
					return;
				}
			}

			//�޹��ж�(����Ϊ-1��������Ʒû����������)
			long lWorldAllowBuyNum = CIncrementShopList::GetWorldBuyLimitNum( dwTradeMode, dwGoodsIndex );
			if ( lWorldAllowBuyNum != -1 )
			{
				CMessage msg( MSG_S2W_OTHER_LIMIT_GOODS_REQUEST );
				msg.Add( pPlayer->GetExID() );
				msg.Add( dwGoodsIndex );
				msg.Add( dwNum );
				msg.Add( dwTradeMode );
				msg.Add( lWorldAllowBuyNum );
				msg.Send();
			}
			else
			{
				ProcessIncrementTrading( pPlayer, dwGoodsIndex, dwNum, dwTradeMode );
			}
		}
		break;
	case MSG_C2S_INCREMENTSHOP_CLOSE:
		//##�ͻ��˸��߷�������Ҫ�ر��̵ꡣ
		{
			pPlayer->SetCurrentProgress( CPlayer::PROGRESS_NONE );
			pPlayer->SetMoveable();			
		}
		break;
	}
}