#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"

#include "..\player.h"
#include "..\Monster.h"
#include "..\Message Packaging\CS2CContainerObjectMove.h"

#include "..\Message Packaging\CS2CContainerObjectAmountChange.h"
#include "..\Goods\CGoods.h"
#include "..\Goods\CGoodsFactory.h"
#include "../GoodsExManage.h"
#include "..\business\CBusinessManager.h"
#include "..\Listener\CGoodsRepairListener.h"
#include "..\container\CTestContainer.h"
#include "..\Listener\CGoodsRepairPriceListener.h"

#include "..\..\..\setup\tradelist.h"
#include "..\..\..\setup\playerlist.h"
#include "..\..\..\setup\globesetup.h"
#include "../../../setup/logsystem.h"
#include "..\serverregion.h"

#include "../../../../public/StrUnifyInput.h"
#include "../FBusiness\ShopManager.h"


#define DELGOODS(vecGoods)\
for (int i=0;i<vecGoods.size();i++)\
{\
	CGoodsFactory::GarbageCollect(&vecGoods[i],169);\
}
#define SENDMSGBOX(pPlayer,szText)\
	CMessage msg(MSG_S2C_OTHER_GAMEMESSAGE);\
	msg.Add(szText);\
	msg.Add((long)0);\
	msg.Add(NULL_GUID);\
	msg.SendToPlayer(pPlayer->GetExID());
bool CheckUnique(CPlayer* pPlayer,CGoodsBaseProperties* pProperty,DWORD dwTotalNum)
{
	if(!pPlayer->IsGoodsUnique(pProperty->GetOriginalName())) 
	{
		SENDMSGBOX(pPlayer,CStringReading::LoadString(22,27));
		return false;
	}
	else
	{
		//���û�������Ʒ,��������������1,����
		if(dwTotalNum>1)
		{
			SENDMSGBOX(pPlayer,CStringReading::LoadString(22,30));
			return false;
		}
	}
	return true;
}

VOID CancelTrading( CPlayer* pPlayer )
{
	pPlayer -> SetCurrentProgress( CPlayer::PROGRESS_NONE );
	pPlayer -> SetMoveable();
	CMessage msg( MSG_S2C_SHOP_CLOSE );
	msg.SendToPlayer( pPlayer -> GetExID() );
}


void OnShopMessage(CMessage* pMsg)
{
	CPlayer* pPlayer = pMsg -> GetPlayer();
	if( pPlayer == NULL /*|| pPlayer -> GetCurrentProgress() != CPlayer::PROGRESS_SHOPPING*/ )
	{
		return;
	}
	if( pPlayer -> IsDied() )
	{
		CancelTrading( pPlayer );
		pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,1));
		return;
	}

	if(pPlayer->GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
		|| pPlayer->GetCurrentProgress()==CPlayer::PROGRESS_TRADING)
	{
		return;
	}

	CServerRegion* pRegion = pMsg -> GetRegion();
	if( pRegion == NULL )
	{
		CancelTrading( pPlayer );
		return;
	}

	switch(pMsg->GetType()) 
	{	
	case MSG_C2S_SHOP_BUY:	//����������̵��й���һ����Ʒ		
		{					
			CGUID NpcID; 
			pMsg -> GetGUID(NpcID);
			DWORD dwGoodsId=pMsg->GetDWord();
			//���������
			DWORD dwGoodsNum=pMsg->GetDWord();
			//����dwContainerId����Ƿ����϶���Ʒ��������
			DWORD dwContainerId=pMsg->GetDWord();
			DWORD dwContainerPos=pMsg->GetDWord();

			if(dwGoodsNum==0)
				return;

			//NPC���
			CMonster* pNpc = (CMonster*)pRegion->FindAroundObject(pPlayer, TYPE_MONSTER, NpcID);

			if (pNpc)
			{
				if( pNpc -> Distance(pPlayer) > 10 )
				{
					CancelTrading( pPlayer );
					break;
				}
#ifdef _GOODSLOG1_
				char pszLog1[1024]="";							

				_snprintf(pszLog1,1024,"[MSG_C2S_SHOP_BUY][NPC NAME:%s][��ƷID:%d][��Ʒ��:%s][��Ʒ����:%d][ConID:%d][ConPos:%d]",
					pNpc->GetName(),dwGoodsId,CGoodsFactory::QueryGoodsName(dwGoodsId),dwGoodsNum,dwContainerId,dwContainerPos);
				CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszLog1);							
#endif
			}
			else
			{
				//##NPC������
				CancelTrading( pPlayer );
				break;
			}

		/*	CServerRegion* pServerRegion=dynamic_cast<CServerRegion*>(pNpc->GetFather());
			if(!pServerRegion)
			{
				CancelTrading( pPlayer );
				break;
			}*/
		//	DWORD dwNpcAreaId=pServerRegion->GetAreaId();

			//��Ʒ���
 			CTradeList::tagTrade *pTrade = CTradeList::GetTradeList(pNpc->GetOriginName());			
			if(!pTrade)
			{				
				CancelTrading( pPlayer );
				return;
			}
			bool bFound=false;
			CTradeList::tagTradeData* pTradeData=NULL;
			for(size_t i=0;i<pTrade->vecGoods.size();i++)
			{
				if(pTrade->vecGoods[i].dwGoodsId == dwGoodsId)
				{
					pTradeData=&pTrade->vecGoods[i];
					bFound=true;
					break;
				}
			}
			CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(dwGoodsId);
			if(!pProperty)
			{
				//��Ϣ����������ƷID���Ϸ���
				return;
			}

			//�������
			if(!bFound || pTradeData==NULL)
				return;
			DWORD dwPrice=0,dwSilver=0,dwTotalPrice=0,dwTotalSilver=0;

			if (pTradeData->dwCreditId!=0 && pTrade->_creditType!=CTradeList::CREDIT_NOT)
			{
				//���������̵�
				if (pTrade->_creditType==CTradeList::CREDIT_AREA)
				{
					if (pTradeData->dwCreditVal>pPlayer->GetCredit(pTradeData->dwCreditId))
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,29));
						return;
					}
				}
				//��֯�����̵�
				else if (pTrade->_creditType==CTradeList::CREDIT_ORGAN)
				{
					if(pTradeData->dwCreditVal>pPlayer->GetOrganCredit(pTradeData->dwCreditId))
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,29));
						return;
					}
				}
			}
			
			//��Ʒ�ܵ�����������*ÿ�����
			DWORD dwTotalNum=dwGoodsNum*pTradeData->dwGoodsNum;
			CTestContainer testContainer;
			testContainer.Init(pPlayer);
			//bResultΪ��ʱҪ������Ʒ
			bool bResult=false,bGoodsChange=false;
			//ָ��λ�ã�һ��ֻ����һ����Ʒ
			if (dwContainerId!=0)
			{
				//λ�ü��
				CVolumeLimitGoodsContainer* pContainer=NULL;
				if(dwContainerId==CS2CContainerObjectMove::PEI_PACKET)
				{
					pContainer=pPlayer->getOriginPack();
				}
				else if(dwContainerId>=CS2CContainerObjectMove::PEI_PACK1 && dwContainerId<=CS2CContainerObjectMove::PEI_PACK5)
				{
					CGoods* pSubpackGoods=pPlayer->getSubpackContainer()->GetSubpack(dwContainerId-CS2CContainerObjectMove::PEI_PACK1)->pGoods;
					if(!pSubpackGoods)
					{
						//û��װ���ӱ�����dwContainerId�Ǵ����
						return;
					}
					pContainer=pPlayer->getSubpackContainer()->GetSubpack(dwContainerId-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
				}
				if(!pContainer)
					return ;	
#ifdef _GOODSLOG1_
					char pszLog2[1024]="";							

				_snprintf(pszLog2,1024,"[MSG_C2S_SHOP_BUY][λ�ü��ͨ��][NPC NAME:%s]",pNpc->GetName());
				CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszLog2);							
#endif
				//��һ���������������ļ��е�������Ȼ���
				if (dwTotalNum!=pTradeData->dwGoodsNum)
				{
					return;
				}
				//������Ʒ
				CGoods* pGoods=CGoodsFactory::CreateGoods(dwGoodsId,32);
				if(!pGoods)
				{
					//������Ʒʧ��
					return;
				}
				//Ψһ�Լ��
				if (pGoods->HasAddonProperty(GAP_UNIQUE))
				{
					if(!CheckUnique(pPlayer,pProperty,dwTotalNum))
						return;
				}
				pGoods->SetAmount(dwTotalNum);
				//�����̵�
				if(pTrade->tradeType==CTradeList::ETT_MONEY || pTrade->tradeType==CTradeList::ETT_SILVER)
				{
					if(pProperty->GetPrice()==0 && pProperty->GetSilverPrice()==0)
					{
						return;
					}
					//�ȼ���۸�
					dwTotalPrice=pGoods->GetAmount()*pProperty->GetPrice();
					dwTotalSilver=pGoods->GetAmount()*pProperty->GetSilverPrice();
	
					DWORD dwLogPrice=dwTotalSilver;
					if(dwTotalPrice>0)
					{
						dwLogPrice=dwTotalPrice;
					}
	
					//��Ǯ����
					if(dwTotalPrice  > pPlayer->GetMoney())
					{
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:NPC_SHOP_BUY ��Ǯ����][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
						CGoodsFactory::GarbageCollect(&pGoods,170);					
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,2));
						return;
					}
	
					//���Ҳ���
					if(dwTotalSilver  > pPlayer->GetSilver())
					{
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:NPC_SHOP_BUY ���Ҳ���][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
						CGoodsFactory::GarbageCollect(&pGoods,171);	
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,14));
						return;
					}

#ifdef _GOODSLOG1_
					char pszLog3[1024]="";							

					_snprintf(pszLog3,1024,"[MSG_C2S_SHOP_BUY][������Ҽ��ͨ��][NPC NAME:%s]",pNpc->GetName());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszLog3);							
#endif
					CGUID GoodsGuid = pGoods->GetExID();
					string strGoodsName = pGoods->GetName();
					if(pPlayer->AddGoodsWithProtect(pGoods,FALSE))
					{
						pPlayer -> SetMoney( pPlayer -> GetMoney() - dwTotalPrice,24 );
						pPlayer->SetSilver(pPlayer->GetSilver()-dwTotalSilver);	
						//���NPC�̵�����Ʒ����־��¼												
						GetGameLogInterface()->logT030_goods_log_npc(pPlayer,dwGoodsId,GoodsGuid,strGoodsName.c_str(),0,dwLogPrice,dwGoodsNum);	
					}
					else
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,4));
						bResult=true;
					}
				}
				//���ﻻ���̵�
				else if(pTrade->tradeType==CTradeList::ETT_GOODS)
				{
					vector<tagTestArg_ID_Num>vecDel;
					vector<CTradeList::tagTradeGoods>&vecTrade=pTradeData->_vecTreadGoods;
					tagTestArg_ID_Num tgdelArg;
					//�õ�Ҫɾ����Ʒ���б�
					for (int i=0;i<vecTrade.size();i++)
					{
						tgdelArg._dwGoodsID=vecTrade[i].dwGoodsChgId;
						tgdelArg._dwNum=vecTrade[i].dwChgAmount;
						vecDel.push_back(tgdelArg);
					}
					//����ɾ��
					vector<tagDelTestResult>vecDelResult;
					BOOL bDelSucc=testContainer.DelTest(vecDel,&vecDelResult);
					if (!bDelSucc)
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,28));
						bResult=true;
					}
					CGUID GoodsGuid = pGoods->GetExID();
					//���Գɹ�
					if (bDelSucc)
					{		
						string strGoodsName = pGoods->GetName();
						if(pPlayer->AddGoodsWithProtect(pGoods,FALSE))
						{	
							//ɾ����Ʒ
							for (int i=0;i<vecDelResult.size();i++)
							{
								pPlayer->DelGoodsByTest(&vecDelResult[i]);
							}
							//���NPC�̵�����Ʒ����־��¼												
							GetGameLogInterface()->logT030_goods_log_npc(pPlayer,dwGoodsId,GoodsGuid,strGoodsName.c_str(),0,0,dwGoodsNum);
						}
						else
						{
							SENDMSGBOX(pPlayer,CStringReading::LoadString(22,4));
							bResult=true;
						}
					}
					//��ӳɹ�
					if (!bResult)
					{
						//��¼λ�澫��������־
						DWORD dwSum=0;
						for (int i=0;i<vecDel.size();i++)
						{
							if (vecDel[i]._dwGoodsID==CGlobeSetup::GetSetup()->dwWeimianGoodsId)
							{
								dwSum+=vecDel[i]._dwNum;
							}
						}
						if(dwSum>0)
						{
							GetGameLogInterface()->logT031_goods_wm_npc(pPlayer,dwGoodsId,GoodsGuid,dwTotalNum,dwSum);
						}	
					}
				}
				
				if(bResult)
				{
#ifdef _GOODSLOG1_
					for (int i=0;i<GainList.size();i++)
					{
						CGoods* pGoods=GainList[i];
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:NPC_SHOP_BUY ��Ǯ����][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
					}

#endif
					CGoodsFactory::GarbageCollect(&pGoods,168);
					return;
				}
				else
					bGoodsChange=true;
				
			}
			//��ָ��λ��,������������һ�ο���������
			else
			{
				//��������
				DWORD dwMaxStack=pProperty->GetAddonPropertyValue(GAP_MAXOVERLAP,1);
				if (dwMaxStack==0)
					dwMaxStack=1;
				//ͨ��������������Ʒ���������ж����Ҫ��������Ʒ���ڱ����ܸ���������ʧ��
				DWORD dwMaxGoods=dwTotalNum/dwMaxStack;
				DWORD dwGoodsLeft=dwMaxGoods;
				//���Ӻ���ʣ����Ʒ,��1
				if (dwTotalNum%dwMaxStack>0)
					dwGoodsLeft=dwMaxGoods+1;
				DWORD dwSpaceNum=pPlayer->GetTotalSpaceNum();
				if (dwMaxGoods>dwSpaceNum || dwGoodsLeft>dwSpaceNum)
				{
					//�����ռ䲻����
					SENDMSGBOX(pPlayer,CStringReading::LoadString(22,4));
					return;
				}
				//������Ʒ���ڲ���
				vector<CGoods*>GainList;
				if (dwMaxStack>=dwTotalNum)
				{
					CGoodsFactory::CreateGoods( dwGoodsId, dwTotalNum, GainList ,169);
				}
				//���ڵ�������Ҫ���������Ʒ
				else
				{
					for (int i=0;i<dwMaxGoods;i++)
					{
						CGoodsFactory::CreateGoods( dwGoodsId, dwMaxStack, GainList ,173);
					}
					if (dwGoodsNum%dwMaxStack>0)
					{
						CGoodsFactory::CreateGoods( dwGoodsId, dwTotalNum%dwMaxStack, GainList ,175);
					}
				}
				//Ψһ�Լ��
				for (size_t i=0;i<GainList.size();i++)
				{
					if (GainList[i]->HasAddonProperty(GAP_UNIQUE))
					{
						if(!CheckUnique(pPlayer,pProperty,dwTotalNum))
							return;
					}
				}
				//�����̵�
				if(pTrade->tradeType==CTradeList::ETT_MONEY || pTrade->tradeType==CTradeList::ETT_SILVER)
				{
					if(pProperty->GetPrice()==0 && pProperty->GetSilverPrice()==0)
					{
						return;
					}
					//���Խ�Ǯ,��һ����Ʒ����
					for (int i=0;i<GainList.size();i++)
					{
						dwTotalPrice+= GainList[i]->GetAmount()*pProperty->GetPrice();
						dwTotalSilver+= GainList[i]->GetAmount()*pProperty->GetSilverPrice();
					}
					DWORD dwLogPrice=dwTotalSilver;
					if(dwTotalPrice>0)
					{
						dwLogPrice=dwTotalPrice;
					}
					bool bMoney=false;
					//��Ҳ���
					if(dwTotalPrice  > pPlayer->GetMoney())
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,2));
						bMoney=true;
#ifdef _GOODSLOG1_
						for (int i=0;i<GainList.size();i++)
						{
							CGoods* pGoods=GainList[i];
							char pszGoodsLog[1024]="";			
							char srcGoodsGUID[50]="";
							pGoods->GetExID().tostring(srcGoodsGUID);					
							_snprintf(pszGoodsLog,1024,"[��Ʒ����:NPC_SHOP_BUY ��Ǯ����][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
								pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
						}

#endif
					}
					//���Ҳ���
					if(dwTotalSilver  > pPlayer->GetSilver())
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,14));
						bMoney=true;
#ifdef _GOODSLOG1_
						for (int i=0;i<GainList.size();i++)
						{
							CGoods* pGoods=GainList[i];
							char pszGoodsLog[1024]="";			
							char srcGoodsGUID[50]="";
							pGoods->GetExID().tostring(srcGoodsGUID);					
							_snprintf(pszGoodsLog,1024,"[��Ʒ����:NPC_SHOP_BUY ���Ҳ���][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
								pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
						}
#endif
					}
					if (bMoney)
					{
						//��Ӳ��ɹ�,�����ڴ�
						DELGOODS(GainList);
						return;
					}
					//���Ա���
					vector<tagTestResult>vecAddResult;
					BOOL bAddSuccess=testContainer.AddTest(GainList,&vecAddResult);
					if (!bAddSuccess)
					{
						//�����ռ䲻����
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,4));
						bResult=true;
					}
					//��ʽ��ӵ�����
					if (bAddSuccess && vecAddResult.size() == GainList.size())
					{
						for(int i=0;i<vecAddResult.size();i++)
						{
							//�����Ʒ�п���ɾ��GainList[i]ָ��,�ȼ�������������־
							CGUID goodsguid=GainList[i]->GetExID();
							//���ʧ�������
							if(!pPlayer->AddGoodsByTest(&vecAddResult[i],GainList[i]))
							{
								CGoodsFactory::GarbageCollect(&GainList[i],176);
							}
							else
							{
								GetGameLogInterface()->logT030_goods_log_npc(pPlayer,dwGoodsId,goodsguid,
									CGoodsFactory::QueryGoodsName(dwGoodsId),0,dwLogPrice,dwTotalNum);
							}
						}
					}
					else
						bResult=true;
					if (bResult)
					{
#ifdef _GOODSLOG1_
						for (int i=0;i<GainList.size();i++)
						{
							CGoods* pGoods=GainList[i];
							char pszGoodsLog[1024]="";			
							char srcGoodsGUID[50]="";
							pGoods->GetExID().tostring(srcGoodsGUID);					
							_snprintf(pszGoodsLog,1024,"[��Ʒ����:NPC_SHOP_BUY ��ӵ��������ɹ�][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
								pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
						}
#endif
						//��Ӳ��ɹ�,�����ڴ�
						DELGOODS(GainList);
					}
					else
					{
						pPlayer -> SetMoney( pPlayer -> GetMoney() - dwTotalPrice,24 );
						pPlayer->SetSilver(pPlayer->GetSilver()-dwTotalSilver);
						bGoodsChange=true;
					}
				}
				//���ﻻ���̵�
				else if(pTrade->tradeType==CTradeList::ETT_GOODS)
				{
					vector<tagTestArg_ID_Num>vecDel;
					vector<CTradeList::tagTradeGoods>&vecTrade=pTradeData->_vecTreadGoods;
					tagTestArg_ID_Num tgdelArg;
					DWORD dwWmNum=0;//ͳ��ʹ��λ�澫��������
					//�õ�Ҫɾ����Ʒ���б��������
					for (int i=0;i<vecTrade.size();i++)
					{
						tgdelArg._dwGoodsID=vecTrade[i].dwGoodsChgId;
						tgdelArg._dwNum=vecTrade[i].dwChgAmount * dwGoodsNum;
						if (tgdelArg._dwGoodsID==CGlobeSetup::GetSetup()->dwWeimianGoodsId)
						{
							dwWmNum+=tgdelArg._dwNum;
						}
						vecDel.push_back(tgdelArg);
					}
					vector<tagDelTestResult>vecDelResult;
					BOOL bDelSucc=testContainer.DelTest(vecDel,&vecDelResult);
					if (!bDelSucc)
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,28));
						//��Ӳ��ɹ�,�����ڴ�
						DELGOODS(GainList);
						break;
					}
					vector<tagTestResult>vecAddResult;
					BOOL bAddSucc=testContainer.AddTest(GainList,&vecAddResult);
					if (!bAddSucc)
					{
						SENDMSGBOX(pPlayer,CStringReading::LoadString(22,4));
						bResult=true;
					}
					//��ʽ��ӵ�����
					if (bDelSucc && bAddSucc && vecAddResult.size() == GainList.size())
					{
						//ɾ����Ʒ
						for (int i=0;i<vecDelResult.size();i++)
						{
							if (!pPlayer->DelGoodsByTest(&vecDelResult[i]))	
							{
								bResult=true;
								break;
							}
						}
						//��ӻ��Ʒ
						if (!bResult)
						{
							for(int i=0;i<vecAddResult.size();i++)
							{
								CGUID goodsguid=GainList[i]->GetExID();
								DWORD dwGoodsNum=GainList[i]->GetAmount();
								if (!pPlayer->AddGoodsByTest(&vecAddResult[i],GainList[i]))
								{
									CGoodsFactory::GarbageCollect(&GainList[i],177);
								}
								else
								{
									//���NPC�̵�����Ʒ����־��¼,���ﻻ���ǮΪ0												
									GetGameLogInterface()->logT030_goods_log_npc(pPlayer,dwGoodsId,goodsguid,
									CGoodsFactory::QueryGoodsName(dwGoodsId),0,0,dwTotalNum);
									//��ӳɹ�,��¼λ�澫��������־,һ����ƷҪ���ĵ�λ������
									if (dwWmNum>0)
									{
										GetGameLogInterface()->logT031_goods_wm_npc(pPlayer,dwGoodsId,goodsguid,
										dwGoodsNum,dwWmNum/dwGoodsNum);
									}	
								}
							}//for
						}//if	
					}
					else
						bResult=true;
					if (bResult)
					{
#ifdef _GOODSLOG1_
						for (int i=0;i<GainList.size();i++)
						{
							CGoods* pGoods=GainList[i];
							char pszGoodsLog[1024]="";			
							char srcGoodsGUID[50]="";
							pGoods->GetExID().tostring(srcGoodsGUID);					
							_snprintf(pszGoodsLog,1024,"[��Ʒ����:NPC_SHOP_BUY ��ӵ��������ɹ�][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
								pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
						}
#endif
						//��Ӳ��ɹ�,�����ڴ�
						DELGOODS(GainList);
					}
					else
					{
						bGoodsChange=true;	
					}
					
				}	
			}
			if (bGoodsChange)
			{
				GetGame()->UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRPACKGOODS);										
				GetGame()->UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS1);					
				GetGame()->UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS2);						
				GetGame()->UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS3);					
				GetGame()->UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS4);					
				GetGame()->UpdateSavePlayer(pPlayer, SAVE_DETAIL_ATTRSUBPACKGOODS5);
			}		
		}
		break;
	case MSG_C2S_SHOP_BUYBACK:
		{
			CGUID NpcID; 
			pMsg -> GetGUID(NpcID);
			CGUID goodsID;
			pMsg->GetGUID(goodsID);
			DWORD dwContainerId=pMsg->GetDWord();
			DWORD dwContainerPos=pMsg->GetDWord();						

			CMonster* pNpc = (CMonster*)pRegion->FindAroundObject(pPlayer, TYPE_MONSTER, NpcID);

			if (pNpc)
			{
				if( pNpc -> Distance(pPlayer) > 10 )
				{
					CancelTrading( pPlayer );
					CMessage msg(MSG_S2C_SHOP_BUYBACK);
					msg.Add((BYTE)0);
					msg.Add(goodsID);
					msg.SendToPlayer(pPlayer->GetExID());
					break;
				}
			}
			else
			{
				//##NPC������
				CancelTrading( pPlayer );
				CMessage msg(MSG_S2C_SHOP_BUYBACK);
				msg.Add((BYTE)0);
				msg.Add(goodsID);
				msg.SendToPlayer(pPlayer->GetExID());
				break;
			}

			//λ�ü��
			CVolumeLimitGoodsContainer* pContainer=NULL;
			if(dwContainerId==CS2CContainerObjectMove::PEI_PACKET)
			{
				pContainer=pPlayer->getOriginPack();
			}
			else if(dwContainerId>=CS2CContainerObjectMove::PEI_PACK1 && dwContainerId<=CS2CContainerObjectMove::PEI_PACK5)
			{
				CGoods* pSubpackGoods=pPlayer->getSubpackContainer()->GetSubpack(dwContainerId-CS2CContainerObjectMove::PEI_PACK1)->pGoods;
				if(!pSubpackGoods)
				{
					//û��װ���ӱ�����dwContainerId�Ǵ����
					CMessage msg(MSG_S2C_SHOP_BUYBACK);
					msg.Add((BYTE)0);
					msg.Add(goodsID);
					msg.SendToPlayer(pPlayer->GetExID());
					return;
				}
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(dwContainerId-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
			}
			if(!pContainer)
				return;
			DWORD dwPrice=0;
			CGoods* pGoods=pPlayer->FindNpcShopSellGoods(goodsID,&dwPrice);
			if(!pGoods)
			{
				// ���ݿͻ��˷�������GUID�ڻع�LIST���в���ƥ�����Ʒ
				CMessage msg(MSG_S2C_SHOP_BUYBACK);
				msg.Add((BYTE)0);
				msg.Add(goodsID);
				msg.SendToPlayer(pPlayer->GetExID());
				return;
			}
			CGoodsFactory::UpdateEquipmentAttr(pGoods);
			DWORD dwTotalPrice=0,dwTotalSilver=0;
			dwTotalSilver=dwPrice;
			/*
			if(pGoods->GetPrice()>0)
			dwTotalPrice=dwPrice;
			if(pGoods->GetSilverPrice()>0)
			dwTotalSilver=dwPrice;	
			*/

			//��Ǯ����
			/*
			if(dwTotalPrice  > pPlayer->GetMoney())
			{						
			pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,2));
			CMessage msg(MSG_S2C_SHOP_BUYBACK);
			msg.Add((BYTE)0);
			msg.Add(goodsID);
			msg.SendToPlayer(pPlayer->GetExID());
			return;
			}*/
			//���Ҳ���
			if(dwTotalSilver  > pPlayer->GetSilver())
			{								
				pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,14));
				CMessage msg(MSG_S2C_SHOP_BUYBACK);
				msg.Add((BYTE)0);
				msg.Add(goodsID);
				msg.SendToPlayer(pPlayer->GetExID());
				return;
			}

			CGoods* pExistedGoods = pContainer->GetGoods( dwContainerPos );
			if( pExistedGoods == NULL )
			{
				CS2CContainerObjectMove comMsg;
				DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
				CGUID goodsGuid=pGoods->GetExID();
				const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);
				DWORD dwGoodsNum=pGoods->GetAmount();
				if( pContainer->Add(dwContainerPos,pGoods, NULL, &comMsg) )
				{
#ifdef _GOODSLOG1_
					char pszGoodsLog[1024]="";						
					char srcGoodsGUID[50]="";
					pGoods->GetExID().tostring(srcGoodsGUID);					
					_snprintf(pszGoodsLog,1024,"[����:SHOP_BUYBACK][�����:%s][����ID:%d][����λ��:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						pPlayer->GetName(),dwContainerId,dwContainerPos,pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif	
					comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
					comMsg.SetDestinationContainerExtendID( dwContainerId);
					pGoods-> SerializeForOldClient( &comMsg.GetObjectStream() );
					comMsg.Send( pPlayer ->GetExID() );

					pPlayer -> SetMoney( pPlayer -> GetMoney() - dwTotalPrice,25 );
					pPlayer->SetSilver(pPlayer->GetSilver()-dwTotalSilver);	

					pPlayer->FetchNpcShopSellGoods(goodsID);

					CMessage msg(MSG_S2C_SHOP_BUYBACK);
					msg.Add((BYTE)1);
					msg.Add(goodsID);
					msg.SendToPlayer(pPlayer->GetExID());

					//���NPC�̵�ع���Ʒ����־��¼												
					GetGameLogInterface()->logT030_goods_log_npc(pPlayer,dwGoodsId,goodsGuid,strGoodsName,3,dwTotalSilver,dwGoodsNum);																									

				}				
			}
			else
			{
				DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
				DWORD dwMaxStack=pGoods->GetMaxStackNumber();
				DWORD dwGoodsNum=pGoods->GetAmount();				
				CGUID goodsGuid=pGoods->GetExID();
				const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);				
				if(dwGoodsId==pExistedGoods->GetBasePropertiesIndex() && dwMaxStack>1 && 
					dwGoodsNum+pExistedGoods->GetAmount()<=dwMaxStack)
				{
					pPlayer->FetchNpcShopSellGoods(goodsID);
					if( pContainer->Add(dwContainerPos,pGoods, NULL) )
					{
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";						
						char srcGoodsGUID[50]="";
						goodsID.tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[����:SHOP_BUYBACK_����][�����:%s][����ID:%d][����λ��:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pPlayer->GetName(),dwContainerId,dwContainerPos,dwGoodsId,strGoodsName,srcGoodsGUID,dwGoodsNum);
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif	
						CGoods* pNewGoods=pContainer->GetGoods(dwContainerPos);
						if(pNewGoods)
						{
							CS2CContainerObjectAmountChange coacMsg;
							coacMsg.SetSourceContainer( TYPE_PLAYER, pPlayer->GetExID(), 
								dwContainerPos );
							coacMsg.SetSourceContainerExtendID( dwContainerId);
							coacMsg.SetObject( pNewGoods -> GetType(), pNewGoods -> GetExID() );
							coacMsg.SetObjectAmount( pNewGoods -> GetAmount() );
							coacMsg.Send( pPlayer->GetExID());
						}					

						pPlayer -> SetMoney( pPlayer -> GetMoney() - dwTotalPrice,26 );
						pPlayer->SetSilver(pPlayer->GetSilver()-dwTotalSilver);						

						CMessage msg(MSG_S2C_SHOP_BUYBACK);
						msg.Add((BYTE)1);
						msg.Add(goodsID);
						msg.SendToPlayer(pPlayer->GetExID());

						//���NPC�̵�ع���Ʒ����־��¼												
						GetGameLogInterface()->logT030_goods_log_npc(pPlayer,dwGoodsId,goodsGuid,strGoodsName,3,dwTotalSilver,dwGoodsNum);																								

					}				
				}
			}			
		}
		break;
	case MSG_C2S_BUSINESS_BUY:
		{
			return;
			// ��ұ����Ѿ�ȡ��������ݣ�������ҵ����
			if( !( pPlayer->IsBusinessMan() && pPlayer->GetBusinessContainer().IsValid() ) )
			{
				return ;
			}

			CGUID NpcID; 
			pMsg->GetGUID( NpcID );
			DWORD dwSpotId = pMsg->GetDWord();
			Business::CBusinessManager &BM = GetInst( Business::CBusinessManager );
			Business::CTradeSpot *pSpot = BM.GetTradeSpot( dwSpotId );

			if( pSpot == NULL )
			{
				return ;
			}
			// ׼����������������û��������ֱ�ӷ��ء�
			WORD wAmount = pMsg->GetWord();
			if( wAmount == 0 ) 
			{
				return;
			}

			// �����ж�
			CMonster *pNpc = static_cast<CMonster*>( pRegion->FindAroundObject( pPlayer, TYPE_MONSTER, NpcID ) );
			if( pNpc != NULL )
			{
				if( pNpc->Distance( pPlayer ) > 10 )
				{
					CancelTrading( pPlayer );
					break;
				}
			}
			else
			{
				CancelTrading( pPlayer );
				break;
			}

			vector<LONG> vecBuyGoodsId;
			vector<DWORD> vecGoodsNum;
			vector<DWORD> vecGoodsPrice;
			vecBuyGoodsId.reserve( wAmount );
			vecGoodsNum.reserve( wAmount );
			vecGoodsPrice.reserve( wAmount );

			for( WORD n = 0; n < wAmount; n++ )
			{
				vecBuyGoodsId.push_back( pMsg->GetLong() );
				vecGoodsNum.push_back( pMsg->GetDWord() );
				vecGoodsPrice.push_back( pMsg->GetDWord() );
			}

			// ��Ʒ���Լ��޹���ļ��
			for( WORD n = 0; n < wAmount; n++ )
			{

				Business::CBusinessGoods *pGoods = pSpot->GetGoods( vecBuyGoodsId[n] );
				if( pGoods == NULL )
				{
					pPlayer->SendNotifyMessage( "��ó�׵�û�д���Ʒ" );
					return ;
				}

				DWORD dwBuyLimit = pGoods->GetBuyLimit( pPlayer->GetExID() );
				if( dwBuyLimit < vecGoodsNum[n] )
				{
					pPlayer->SendNotifyMessage( "��Ʒ�����޹�����ܹ���" );
					return;
				}

				// �۸����䶯
				if( pGoods->GetBuyPrice() != vecGoodsPrice[n] )
				{
					pPlayer->SendNotifyMessage( "��Ʒ�۸����仯������ʧ��" );
					return ;
				}
			}

			struct tagBuyBusinessGoodsInfo
			{
				CGoods *pGoods;	
				DWORD dwPrice;
				DWORD dwPos;

			};
			vector<tagBuyBusinessGoodsInfo> buyGoodsVec;	
			vector<CGoods*> testGoodsVec;

			DWORD dwPrice = 0,dwTotalPrice = 0;
			LONG lTotalTax = 0;		
			DWORD dwTotalCount = 0; 

			// ��ʼ������Ʒ
			DWORD dwLockGrid = 0,dwMaxStack = 0;
			for( WORD n = 0; n < wAmount; n++ )
			{					
				dwMaxStack = CGoodsFactory::QueryGoodsMaxStackNumber( vecBuyGoodsId[n] );
				CGoodsBaseProperties *pProperties = CGoodsFactory::QueryGoodsBaseProperties( vecBuyGoodsId[n] );
				if( !pProperties )
				{
					continue;
				}

				Business::CBusinessGoods *pBusinessGoods = pSpot->GetGoods( vecBuyGoodsId[n] );
				if( pBusinessGoods == NULL )
				{
					continue;
				}

				if( dwMaxStack > 1 )
				{
					// �������ᱻ��ֳɼ���������						
					vector<CGoods*> goodsVec;
					CGoodsFactory::CreateGoods( vecBuyGoodsId[n], vecGoodsNum[n], goodsVec ,33);
					dwLockGrid += goodsVec.size();
					for( size_t i = 0; i < goodsVec.size(); i++ )
					{
						tagBuyBusinessGoodsInfo stInfo;
						stInfo.pGoods = goodsVec[i];						
						stInfo.dwPos = 0;
						stInfo.dwPrice = pBusinessGoods->GetBuyPrice();
						stInfo.pGoods->SetBuyPrice( stInfo.dwPrice );
						buyGoodsVec.push_back( stInfo );
						testGoodsVec.push_back( stInfo.pGoods );
						dwPrice = stInfo.dwPrice * goodsVec[i]->GetAmount();
						dwTotalPrice += dwPrice;

						dwTotalCount += goodsVec[i]->GetAmount();
					}
				}
				else
				{
					dwLockGrid += vecGoodsNum[n];
					for( int i=0; i < vecGoodsNum[n]; i++ )
					{
						CGoods *pGoods = CGoodsFactory::CreateGoods( vecBuyGoodsId[n],34 );
						tagBuyBusinessGoodsInfo stInfo;
						stInfo.pGoods = pGoods;						
						stInfo.dwPos = 0;
						stInfo.dwPrice = pBusinessGoods->GetBuyPrice();
						stInfo.pGoods->SetBuyPrice( stInfo.dwPrice );
						buyGoodsVec.push_back( stInfo );	
						testGoodsVec.push_back( stInfo.pGoods );
						dwTotalPrice += stInfo.dwPrice;
					}

					dwTotalCount += vecGoodsNum[n];
				}
			}

			// �������Ƿ���Ǯ����
			if( dwTotalPrice > pPlayer->GetMoney() )
			{
				for( size_t i = 0; i < buyGoodsVec.size(); i++ )
				{
#ifdef _GOODSLOG1_
					if( buyGoodsVec[i].pGoods )
					{
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						buyGoodsVec[i].pGoods->GetExID().tostring( srcGoodsGUID );					
						_snprintf( pszGoodsLog, 1024, "[��Ʒ����:BUSINESS_BUY ��Ҳ���][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							buyGoodsVec[i].pGoods->GetBasePropertiesIndex(), CGoodsFactory::QueryGoodsName(buyGoodsVec[i].pGoods->GetBasePropertiesIndex()),srcGoodsGUID,buyGoodsVec[i].pGoods->GetAmount());
						CMessage::AsyWriteFile( GetGame()->GetGoodsLogFile(), pszGoodsLog );							
					}					
#endif
					CGoodsFactory::GarbageCollect( &( buyGoodsVec[i].pGoods ), 47 );						
				}
				pPlayer -> SendNotifyMessage( CStringReading::LoadString( 22, 2 ) );
				return;
			}

			Business::CPackage &businessContainer = pPlayer->GetBusinessContainer();

			// �����Ƿ������ӵ�����
			CTestContainer test_container;
			vector<tagTestResult> test_result;
			test_container.AddContainer( CS2CContainerObjectMove::PEI_BUSSINESPACK, businessContainer.m_pContainer );
			if( dwTotalCount > businessContainer.GetLeftGoodsCount() || 
				!test_container.AddTest( testGoodsVec, &test_result ) ||
				test_result.size() != buyGoodsVec.size() )
			{
				// ���ʧ�ܣ����մ�������Ʒ
				for( size_t i = 0; i < buyGoodsVec.size(); i++ )
				{
#ifdef _GOODSLOG1_
					if( buyGoodsVec[i].pGoods )
					{
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						buyGoodsVec[i].pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:BUSINESS_BUY �����ռ䲻��][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							buyGoodsVec[i].pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(buyGoodsVec[i].pGoods->GetBasePropertiesIndex()),srcGoodsGUID,buyGoodsVec[i].pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
					}					
#endif
					CGoodsFactory::GarbageCollect( &(buyGoodsVec[i].pGoods), 48 );
				}
				pPlayer->SendNotifyMessage( CStringReading::LoadString(25,1) );
				return;
			}

			// ���±���ʣ��ɷ�����Ʒ����
			businessContainer.AddGoodsCount( dwTotalCount );
			pPlayer->SetMoney( pPlayer->GetMoney() - dwTotalPrice,27 );	

			// ���ͻ�Ӧ��Ϣ��������Ʒ����ҵ����
			vector<BYTE> array;
			CMessage msg( MSG_S2C_BUSINESS_BUY );
			msg.Add( (WORD)buyGoodsVec.size() );
			for( size_t i = 0; i < buyGoodsVec.size(); i++ )
			{
				pPlayer->OnGetGoods( buyGoodsVec[i].pGoods->GetBasePropertiesIndex(), buyGoodsVec[i].pGoods );

				// ��¼������־
				GetGameLogInterface()->logT700_business_npc( pPlayer, buyGoodsVec[i].pGoods, 
					buyGoodsVec[i].pGoods->GetAmount(), 
					buyGoodsVec[i].pGoods->GetBuyPrice() * buyGoodsVec[i].pGoods->GetAmount(),
					1, dwSpotId );

				// ���µ��޹����¼��
				Business::CBusinessGoods *pBusinessGoods = pSpot->GetGoods( buyGoodsVec[i].pGoods->GetBasePropertiesIndex() );
				pBusinessGoods->SubBuyLimit( pPlayer->GetExID(), buyGoodsVec[i].pGoods->GetAmount() ); 	
				// ���ٿ����
				pBusinessGoods->SetVolume( pBusinessGoods->GetVolume() - buyGoodsVec[i].pGoods->GetAmount() );
				// Ӱ��������Ʒ�����
				pSpot->DecreaseGoodsVolume( pBusinessGoods, buyGoodsVec[i].pGoods->GetAmount() );

				long lIndex = 0;
				DWORD dwNewPrice = 0;
				// ���¼۸�
				dwNewPrice = businessContainer.UpdateGoodsPrice( buyGoodsVec[i].pGoods );
				lIndex = buyGoodsVec[i].pGoods->GetBasePropertiesIndex();

				msg.Add( (BYTE)test_result[i].eOperType );
				if( test_result[i].eOperType == CS2CContainerObjectMove::OT_NEW_OBJECT )
				{
					// ����Ʒ�Ž�����
					businessContainer.m_pContainer->Add( test_result[i].dwPos, buyGoodsVec[i].pGoods, NULL, NULL );					

					// �����Ʒ��Ϣ����Ϣ
					array.clear();
					buyGoodsVec[i].pGoods->SerializeForOldClient( &array );
					msg.Add( (long)array.size() );
					msg.AddEx( &array[0], (long) array.size() );
					msg.Add( test_result[i].dwPos );
				}
				else if( test_result[i].eOperType == CS2CContainerObjectMove::OT_CHANGE_AMOUNT )
				{
					// ���ӱ��������Ʒ�ĵ�����
					CGoods *pGoods = businessContainer.m_pContainer->GetGoods( test_result[i].dwPos );
					pGoods->SetAmount( pGoods->GetAmount() + buyGoodsVec[i].pGoods->GetAmount() );

					// ������ӵĵ���������Ϣ
					msg.Add( (long) buyGoodsVec[i].pGoods->GetAmount() );
					msg.Add( pGoods->GetExID() );
					// ���մ���Ʒ
					CGoodsFactory::GarbageCollect( &buyGoodsVec[i].pGoods, 48 );
				}

				// ���Ӹ��º����Ʒ�۸�
				msg.Add( (long) lIndex );
				msg.Add( (DWORD) dwNewPrice );
			}
			msg.SendToPlayer( pPlayer->GetExID() );	
			// ���ô浵��־
			GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRBUSINESSPACK );
		}
		break;
	case MSG_C2S_SHOP_SELL:	//���������һ����Ʒ���̵�
		{			
			CGUID NpcID; 
			pMsg -> GetGUID(NpcID);	//NPC��ID��			
			DWORD dwContianerId=0,dwPos=0;
			dwContianerId=pMsg->GetDWord();
			dwPos=pMsg->GetDWord();	

			//ȡ������
			CVolumeLimitGoodsContainer* pContainer=NULL;
			if(dwContianerId==CS2CContainerObjectMove::PEI_PACKET)
			{
				pContainer=pPlayer->getOriginPack();
			}
			else if(dwContianerId>=CS2CContainerObjectMove::PEI_PACK1 && dwContianerId<=CS2CContainerObjectMove::PEI_PACK5)
			{
				CGoods* pSubpackGoods=pPlayer->getSubpackContainer()->GetSubpack(dwContianerId-CS2CContainerObjectMove::PEI_PACK1)->pGoods;
				if(!pSubpackGoods)
				{
					//û��װ���ӱ�����dwContainerId�Ǵ����

					return;
				}
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(dwContianerId-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
			}
			else
			{
				//��֧�����ContainerId
				return;
			}

			if(!pContainer)
				return;
			if(dwPos >= pContainer->GetVolume())
				return;

			CGoods* pGoods=pContainer->GetGoods(dwPos);
			if(!pGoods)
			{
				//��λ����û����Ʒ��
				return;
			}

			CMonster* pNpc	= dynamic_cast<CMonster*>( pRegion ->FindAroundObject(pPlayer, TYPE_MONSTER, NpcID) );
			if( pNpc )
			{
				if( pNpc -> Distance(pPlayer) > 10 )
				{
					pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,5));
					CMessage msg(MSG_S2C_SHOP_SELL);
					msg.Add((BYTE)0);
					msg.Add(pGoods->GetExID());
					msg.SendToPlayer(pPlayer->GetExID());
					CancelTrading( pPlayer );
					break;
				}
			}
			else
			{
				CancelTrading( pPlayer );
				CMessage msg(MSG_S2C_SHOP_SELL);
				msg.Add((BYTE)0);
				msg.Add(pGoods->GetExID());
				msg.SendToPlayer(pPlayer->GetExID());
				break;
			}									


			if (GoodsExManage::GetInstance().IsFrost(pGoods)) //! �������
			{
				//! ��������ҽ��ף����ס���̯��
				//! ���ܷ������̵ꣻ
				//! �������������ۣ���Ϊ�ƻ����Ź��ܣ�Ŀǰ��δ����
				//! �������ʱ������䣻
				//! ���ܽ��зֽ������
				//! ���ܶ�����
				char* strRes=CStringReading::LoadString(20,25);
				if(strRes)
				{
					pPlayer->SendNotifyMessage(strRes,NOTIFY_COLOR_RED);
					CMessage msg(MSG_S2C_SHOP_SELL);
					msg.Add((BYTE)0);
					msg.Add(pGoods->GetExID());
					msg.SendToPlayer(pPlayer->GetExID());
					return;
				}
			}
			/*
			else if (GoodsExManage::GetInstance().IsBind(pGoods)) //! �����
			{
			//! ��������ҽ��ף����ס���̯��
			//! �������������ۣ���Ϊ�ƻ����Ź��ܣ�Ŀǰ��δ����
			//! �������ʱ������䣻
			//! ����ʱ��ʾ��Ҷ�������Ʒ����Ʒ��������
			char* strRes=CStringReading::LoadString(20,26);
			if(strRes)
			{
			pPlayer->SendNotifyMessage(strRes,NOTIFY_COLOR_RED);
			CMessage msg(MSG_S2C_SHOP_SELL);
			msg.Add((BYTE)0);
			msg.Add(pGoods->GetExID());
			msg.SendToPlayer(pPlayer->GetExID());
			return;
			}
			}*/

			CGUID guid = pGoods->GetExID();
			char szGoodsName[32];
			lstrcpy( szGoodsName, pGoods->GetGoodsName() );

			//##�õ�������Ʒ�ĳ��ۼ۸�
			DWORD dwSilverPrice=0;

			dwSilverPrice=CGoodsFactory::CalculateVendSilverPrice(pGoods);


			if(dwSilverPrice==0 )
			{
				pPlayer -> SendNotifyMessage( CStringReading::LoadString(22,6));
				CMessage msg(MSG_S2C_SHOP_SELL);
				msg.Add((BYTE)0);
				msg.Add(pGoods->GetExID());
				msg.SendToPlayer(pPlayer->GetExID());
			}
			else
			{				
				dwSilverPrice=dwSilverPrice * pGoods->GetAmount();				

				//##�ж�����
				DWORD dwMaxMoney=CGoodsFactory::QueryGoodsMaxStackNumber( CGoodsFactory::GetGoldCoinIndex() );
				DWORD dwMaxSilver=CGoodsFactory::QueryGoodsMaxStackNumber(CGoodsFactory::GetSilverIndex());		
				/*
				if(pPlayer->GetMoney()+dwPrice>dwMaxMoney)
				{
				pPlayer -> SendNotifyMessage( CStringReading::LoadString(22,7) );
				CMessage msg(MSG_S2C_SHOP_SELL);
				msg.Add((BYTE)0);
				msg.Add(pGoods->GetExID());
				msg.SendToPlayer(pPlayer->GetExID());

				}*/
				if(pPlayer->GetSilver()+dwSilverPrice>dwMaxSilver)
				{
					pPlayer -> SendNotifyMessage( CStringReading::LoadString(22,15) );
					CMessage msg(MSG_S2C_SHOP_SELL);
					msg.Add((BYTE)0);
					msg.Add(pGoods->GetExID());
					msg.SendToPlayer(pPlayer->GetExID());
				}
				else 
				{	
					CS2CContainerObjectMove comMsg;
					comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );											
					//pContainer->Remove(pGoods,&comMsg);
					CGoods* pRemovedGoods=dynamic_cast<CGoods*>(pContainer->Remove(dwPos,pGoods->GetAmount(),&comMsg));
					if(pRemovedGoods)
					{
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";						
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[����:SHOP_SELL][�����:%s][����ID:%d][����λ��:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pPlayer->GetName(),dwContianerId,dwPos,pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif	

						pPlayer->PushbakNpcShopSellGoods(pGoods,dwSilverPrice);
						pPlayer->SetSilver(pPlayer->GetSilver()+dwSilverPrice);


						comMsg.SetSourceContainerExtendID(dwContianerId);
						comMsg.Send( pPlayer -> GetExID() );

						CMessage msg(MSG_S2C_SHOP_SELL);
						msg.Add((BYTE)1);
						msg.Add(pGoods->GetExID());
						msg.SendToPlayer(pPlayer->GetExID());

						//ʧȥ��Ʒ����
						pPlayer->OnLoseGoods(pGoods);
						//�������Ʒ��NPC�̵����־��¼
						DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
						CGUID goodsGuid=pGoods->GetExID();
						const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);
						DWORD dwGoodsNum=pGoods->GetAmount();
						GetGameLogInterface()->logT030_goods_log_npc(pPlayer,dwGoodsId,goodsGuid,strGoodsName,1,dwSilverPrice,dwGoodsNum);																									
					}																									
				}
			}//else end				
		}
		break;
	case MSG_C2S_ESSENCE_SHOP_OPEN:
		{
			DWORD dwAreaId=0;
			CServerRegion* pRegion=dynamic_cast<CServerRegion*>(pPlayer->GetFather());
			if(pRegion)
				dwAreaId=pRegion->GetAreaId();
			char szNpcName[256];
			//szNpcName="TreasureShop";
			pMsg->GetStr(szNpcName,256);
			if(strcmp(szNpcName,"")==0)
			{
				strcpy_s(szNpcName,256, "TreasureShop");
			}			

			CTradeList::tagTrade *pTrade = CTradeList::GetTradeList(szNpcName);
			if (pTrade)
			{
				pPlayer->ProduceEvent(ET_EventBreak);
				CMessage msg(MSG_S2C_SHOP_OPEN);				
				
				CGUID emptyGuid;
				msg.Add(emptyGuid);
				

				msg.Add(long(pTrade->shopType));//�̵�����

				msg.Add(BYTE(pTrade->tradeType));//���ͣ���Ǯ���� OR ���ﻻ��
				msg.Add(CGlobeSetup::GetSetup() -> fBasePriceRate);//FLOAT
				msg.Add(CGlobeSetup::GetSetup() -> fTradeInRate);//FLOAT	

				long lNum=pTrade->vecGoods.size();			
				msg.Add(lNum);//��Ʒ����
				if(pTrade->tradeType==CTradeList::ETT_MONEY || pTrade->tradeType==CTradeList::ETT_SILVER)
				{
					for(int i=0;i<pTrade->vecGoods.size();i++)
					{					
						CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
							pTrade->vecGoods[i].dwGoodsId);
						if(!pProperties)
						{
							return;
						}


						msg.Add(pTrade->vecGoods[i].dwGoodsId);//��ƷID		
						msg.Add(pTrade->vecGoods[i].dwGoodsNum);										
					}
				}
				else if(pTrade->tradeType==CTradeList::ETT_GOODS)
				{
					/*
					struct tagTradeData
					{
					DWORD dwGoodsId;	//��ƷID
					DWORD dwGoodsChgId;	//Ҫ������Ʒ��ID
					DWORD dwChgAmount;	//Ҫ������Ʒ����
					DWORD dwAreaId;		//��������ID
					DWORD dwCredit;		//������������
					DWORD dwCreditConsume;	//��������
					};
					*/
					//for(int i=0;i<pTrade->vecGoods.size();i++)
					//{
					//	msg.Add(pTrade->vecGoods[i].dwGoodsId);//��ƷID
					//	msg.Add(pTrade->vecGoods[i].dwGoodsNum);
					//	msg.Add(pTrade->vecGoods[i].dwGoodsChgId);//Ҫ��������ƷID
					//	msg.Add(pTrade->vecGoods[i].dwChgAmount);//��Ҫ��������
					//	msg.Add(dwAreaId);//��Ҫ�ĸ����������
					//	msg.Add(pTrade->vecGoods[i].dwCredit);//��Ҫ�����������ﵽ���ټ�
					//	msg.Add(pTrade->vecGoods[i].dwCreditConsume);//��Ҫ���Ķ�����������
					//}
				}
				msg.SendToPlayer(pPlayer->GetExID());
			}
			else 
			{
				pPlayer -> SetCurrentProgress( CPlayer::PROGRESS_NONE );
				pPlayer -> SetMoveable();
			}							
		}
		break;
	case MSG_C2S_BUSINESS_SELL:	//���������һ����Ʒ���̵�
		{	
			return;
			// ��ұ����Ѿ�ȡ��������ݣ�������ҵ����
			if( !( pPlayer->IsBusinessMan() && pPlayer->GetBusinessContainer().IsValid() ) )
			{
				return ;
			}

			CGUID NpcID; 
			pMsg->GetGUID( NpcID );
			DWORD dwSpotId = pMsg->GetDWord();
			Business::CBusinessManager &BM = GetInst( Business::CBusinessManager );
			Business::CTradeSpot *pSpot = BM.GetTradeSpot( dwSpotId );
			Business::CPackage &businessContainer = pPlayer->GetBusinessContainer();
			if( pSpot == NULL )
			{
				return ;
			}
			// �������������û��������ֱ�ӷ��ء�
			long lSellAmount = pMsg->GetLong();
			if( lSellAmount == 0 ) 
			{
				return;
			}

			// �����ж�
			CMonster *pNpc = static_cast<CMonster*>( pRegion->FindAroundObject( pPlayer, TYPE_MONSTER, NpcID ) );
			if( pNpc != NULL )
			{
				if( pNpc->Distance( pPlayer ) > 10 )
				{
					CancelTrading( pPlayer );
					break;
				}
			}
			else
			{
				CancelTrading( pPlayer );
				break;
			}

			struct tagSellGoods
			{
				CGUID guid;
				long count;
				DWORD price;
				CGoods *pGoods;
				Business::CBusinessGoods *pBusinessGoods;
			};
			typedef std::vector<tagSellGoods> SellGoodsList;
			SellGoodsList sell_goods;

			for( long i = 0; i < lSellAmount; ++ i )
			{
				tagSellGoods goods_info;
				pMsg->GetGUID( goods_info.guid );
				goods_info.count = pMsg->GetLong();
				goods_info.price = pMsg->GetDWord();

				// �ӱ�����ȡ��Ʒ
				CGoods *pGoods = static_cast<CGoods*>( businessContainer.m_pContainer->Find( goods_info.guid ) );
				if( pGoods == NULL )
				{
					pPlayer->SendNotifyMessage( "���۷Ƿ���ƷID" );
					return ;
				}

				if( pGoods->GetAmount() < goods_info.count )
				{
					pPlayer->SendNotifyMessage( "�����������������" );
					return ;
				}

				// ��ȡ��Ʒ����
				Business::CBusinessGoods *pBusinessGoods = pSpot->GetGoods( pGoods->GetBasePropertiesIndex() );
				if( pBusinessGoods == NULL )
				{
					BUSINESS_LOG_ERROR( Business::FMT_STR( "Not found business goods from id [%d]", pGoods->GetBasePropertiesIndex() ) );
					pPlayer->SendNotifyMessage( "��ó�׵��޴���Ʒ" );
					return ;
				}

				// �۸�Ա�
				bool bLocal = pBusinessGoods->GetBaseProperty().Local != 0;
				if(	goods_info.price != pBusinessGoods->GetSellPrice( bLocal ) )
				{
					pPlayer->SendNotifyMessage( "��Ʒ�۸����仯������ʧ��" );
					return ;
				}

				goods_info.pGoods = pGoods;
				goods_info.pBusinessGoods = pBusinessGoods;
				sell_goods.push_back( goods_info );
			}

			CMessage msg(MSG_S2C_BUSINESS_SELL);
			DBWriteSet db;
			long lProfit = 0;
			long lPos = 0;
			long lCount = 0;
			DWORD dwSellCount = 0;
			msg.GetDBWriteSet( db );
			db.AddToByteArray( (long) sell_goods.size() ); 
			for( SellGoodsList::iterator it = sell_goods.begin(); it != sell_goods.end(); ++ it )
			{
				tagSellGoods &goods_info = *it;

				// ��Ʒ����ʱ�ļ۸�
				DWORD dwBuyPrice = goods_info.pGoods->GetBuyPrice();
				// ��ȡ��ó�׵����Ʒ�۸�����Ǳ��س��ۣ���Ϊ1/2��ǰ�����۸�
				DWORD dwSellPrice = goods_info.price;

				// TODO:�������󡢼���ȡ����ҵ���顢��ҵSP
				lProfit += ( (long)dwSellPrice - (long)dwBuyPrice ) * goods_info.count;

				// ...

				// TODO:���ݻ�õ�����ֵ�����Ƿ���ֱ���

				// ���¿�漰���ۼ�¼
				goods_info.pBusinessGoods->AddSellCount( goods_info.count );
				goods_info.pBusinessGoods->SetVolume( goods_info.pBusinessGoods->GetVolume() + goods_info.count );
				// Ӱ��������Ʒ�����
				pSpot->IncreaseGoodsVolume( goods_info.pBusinessGoods, goods_info.count );

				// ��¼������־
				GetGameLogInterface()->logT700_business_npc( pPlayer, goods_info.pGoods, 
					goods_info.count, dwSellPrice * goods_info.count, 
					2, dwSpotId );

				// ������Ʒ
				if( goods_info.pGoods->GetAmount() == goods_info.count )
				{
					db.AddToByteArray( (BYTE)CS2CContainerObjectMove::OT_DELETE_OBJECT);
					db.AddToByteArray( goods_info.guid );

					businessContainer.m_pContainer->Remove( goods_info.pGoods );
					pPlayer->OnLoseGoods( goods_info.pGoods );

#ifdef _GOODSLOG1_
					char pszGoodsLog[1024]="";			
					char srcGoodsGUID[50]="";
					goods_info.pGoods->GetExID().tostring( srcGoodsGUID );					
					_snprintf( pszGoodsLog, 1024, "[��Ʒ����:BUSINESS_SELL][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						goods_info.pGoods->GetBasePropertiesIndex(), CGoodsFactory::QueryGoodsName( goods_info.pGoods->GetBasePropertiesIndex() ), srcGoodsGUID, goods_info.pGoods->GetAmount() );
					CMessage::AsyWriteFile( GetGame()->GetGoodsLogFile(), pszGoodsLog );							
#endif
					CGoodsFactory::GarbageCollect( &goods_info.pGoods ,49);
					dwSellCount ++;
				}
				else
				{
					goods_info.pGoods->SetAmount( goods_info.pGoods->GetAmount() - goods_info.count );
					db.AddToByteArray( (BYTE)CS2CContainerObjectMove::OT_CHANGE_AMOUNT );
					db.AddToByteArray( goods_info.guid );
					db.AddToByteArray( goods_info.pGoods->GetAmount() );
					dwSellCount ++;
				}

				// ��ȡ��Ǯ
				DWORD dwMoney = dwSellPrice * goods_info.count;
				pPlayer->SetMoney( pPlayer->GetMoney() + dwMoney,28 );
			}

			// �ı䱳���;�
			businessContainer.UpdateEndure( lProfit );
			// �����;����Ե��ͻ���
			businessContainer.UpdateEndureToClient( pPlayer->GetExID() );
			// ��鱳���Ƿ���Ч����Ч�����
			businessContainer.CheckValid();
			if( dwSellCount > 0 )
			{
				GetGame()->UpdateSavePlayer( pPlayer, SAVE_DETAIL_ATTRBUSINESSPACK );
				msg.SendToPlayer( pPlayer->GetExID() );
			}
		}
		break;
	case MSG_C2S_BUSINESS_INVEST:
		{
			//CGUID NpcID; 
			//pMsg -> GetGUID(NpcID);	//NPC��ID��
			//DWORD dwSpotId=pMsg->GetDWord();
			//DWORD dwInvestNum=pMsg->GetDWord();			
			//CMonster* pNpc	= dynamic_cast<CMonster*>( pRegion ->FindAroundObject(pPlayer, TYPE_MONSTER, NpcID) );
			//if( pNpc )
			//{
			//	if( pNpc -> Distance(pPlayer) > 10 )
			//	{
			//		pPlayer -> SendNotifyMessage(CStringReading::LoadString(25,2) );
			//		CancelTrading( pPlayer );
			//		return;
			//	}
			//}
			//else
			//{
			//	CancelTrading( pPlayer );
			//	return;
			//}
			//if(dwInvestNum>CBusinessManager::GetInvestMaxValue())
			//{
			//	//�����˵������Ͷ�ʶ�
			//	pPlayer->SendNotifyMessage(CStringReading::LoadString(25,3));
			//	return;
			//}
			//CTradeSpot* pSpot=CBusinessManager::GetTradeSpotById(dwSpotId);
			//if(!pSpot)
			//	return;
			//if(pSpot->GetCountryId()!=0)
			//{
			//	pPlayer->SendNotifyMessage(CStringReading::LoadString(25,4));
			//	return;
			//}
			//BOOL bCanInvest=pPlayer->SetInvestRecord(dwSpotId,dwInvestNum);
			//if(!bCanInvest)
			//{
			//	pPlayer->SendNotifyMessage(CStringReading::LoadString(25,5));
			//	return;
			//}
			////���¹���Ͷ�ʶ��
			//BYTE btCountryId=pPlayer->GetCountry();
			//pSpot->AddInvestValue(btCountryId,dwInvestNum);

			////������������������
			//pPlayer->AddCredit(pSpot->GetTradeAreaId(),dwInvestNum);


		}
		break;
	case MSG_C2S_SHOP_REQUESTREPAIRALLPRICE:
		{
			CGoodsRepairPriceListener lPrice;
			pPlayer ->getOriginPack()->TraversingContainer( &lPrice );
			pPlayer -> m_cEquipment.TraversingContainer( &lPrice );
			pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer->TraversingContainer(&lPrice);

			lPrice.m_dwPrice = static_cast<DWORD>( lPrice.m_dwPrice * ( 1 + pRegion -> GetTaxRate() / 100 ) );
			CMessage msg(MSG_S2C_SHOP_REPAIRALLCOST);
			msg.Add(lPrice.m_dwPrice);
			msg.SendToPlayer(pPlayer->GetExID());
		}
		break;
	case MSG_C2S_SHOP_REPAIR: //����������̵�������һ����Ʒ
		{
			//##�����ж�NPC����
			CGUID NpcID; pMsg -> GetGUID(NpcID);
			CMonster* pNpc	= dynamic_cast<CMonster*>( pRegion -> 
				FindAroundObject(pPlayer, TYPE_MONSTER, NpcID) );
			if( pNpc )
			{
				CTradeList::tagTrade *pTrade = CTradeList::GetTradeList(pNpc->GetOriginName());			
				if(!pTrade)
				{				
					CancelTrading( pPlayer );
					break;
				}
				if( pNpc -> Distance(pPlayer) > 10 )
				{
					pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,5) );
					CancelTrading( pPlayer );
					break;
				}	
			}
			else
			{
				CancelTrading( pPlayer );
				break;
			}

			//##��Σ���ʼ������Ʒ
			LONG lContainerId=pMsg->GetLong();
			LONG lPos=pMsg->GetLong();			


			//����IDȡ��������
			CGoodsContainer* pContainer = NULL;
			CGoods* pGoods=NULL;
			switch(lContainerId)
			{
			case CS2CContainerObjectMove::PEI_PACKET:
				pContainer = pPlayer -> getOriginPack();
				break;
			case CS2CContainerObjectMove::PEI_EQUIPMENT:
				pContainer = &( pPlayer -> m_cEquipment );
				break;						
				/*			case CS2CContainerObjectMove::PEI_DEPOT:										
				pContainer = &( pPlayer -> m_cDepot);					
				break;	*/			
			case CS2CContainerObjectMove::PEI_PACK:
				pContainer =  pPlayer ->getSubpackContainer();
				break;
			case CS2CContainerObjectMove::PEI_PACK1:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK2:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK3:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK4:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK5:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
				break;
			}
			if( pContainer )
			{
				pGoods=pContainer->GetGoods(lPos);
			}
			/*
			//װ�����е���Ʒ
			else if( lPos >= pPlayer ->getOriginPack()->GetGoodsAmountLimit() && lPos < 
			pPlayer ->getOriginPack()->GetGoodsAmountLimit() + CEquipmentContainer::s_dwEquipmentLimit )
			{
			pGoods = pPlayer -> m_cEquipment.GetGoods( lPos - pPlayer ->getOriginPack()->GetGoodsAmountLimit() );
			}*/
			if(pGoods && pGoods -> CanReparied())
			{
				LONG lCurrentDurability	= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAXIMUM_DURABILITY, 2 );
				DWORD dwPrice = CGoodsFactory::CalculateRepairPrice( pGoods );
				dwPrice = static_cast<DWORD>( dwPrice * ( 1 + pRegion -> GetTaxRate() / 100 ) );
				if( pPlayer ->GetSilver() >= dwPrice )
				{
					if( CGoodsFactory::RepairEquipment(pGoods) )
					{
						//GetGameLogInterface()->logT023_goods_log_repair(pPlayer,pGoods->GetBasePropertiesIndex(),pGoods->GetExID(),1);
						pPlayer ->SetSilver( pPlayer ->GetSilver() - dwPrice );
						if (pContainer==&( pPlayer -> m_cEquipment )&&lCurrentDurability==0)
						{
							DWORD  dwPosition = 0;
							pPlayer->GetEquipmentContainer()->QueryGoodsPosition(pGoods,dwPosition);
							pPlayer->MountEquipEx(pGoods,true);
							pPlayer->UpdateAttribute(TRUE);
							pPlayer->ProduceEvent(ET_EquipMount,
								CS2CContainerObjectMove::PEI_EQUIPMENT,
								dwPosition,
								pGoods->GetGoodsBaseType(), 
								pGoods-> GetAddonPropertyValues( GAP_WEAPON_CATEGORY, 1 ),
								(void*)CGoodsFactory::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex()));
						}
						CMessage msg(MSG_S2C_SHOP_REPAIR);
						msg.Add((char)SHOP_REPAIR_SUCCEED);
						msg.Add(lContainerId);
						msg.Add(lPos);
						msg.Add(dwPrice);
						msg.SendToPlayer( pPlayer -> GetExID() );
						return;
					}
					else
					{
						pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,8));
					}
				}
				else
				{
					pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,9));
				}
			}
		}
		break;
	case MSG_C2S_SHOP_REQUESTREPAIRSINGLEPRICE:
		{
			//##��Σ���ʼ������Ʒ
			LONG lContainerId=pMsg->GetLong();
			LONG lPos=pMsg->GetLong();			


			//����IDȡ��������
			CGoodsContainer* pContainer = NULL;
			CGoods* pGoods=NULL;
			switch(lContainerId)
			{
			case CS2CContainerObjectMove::PEI_PACKET:
				pContainer = pPlayer -> getOriginPack();
				break;
			case CS2CContainerObjectMove::PEI_EQUIPMENT:
				pContainer = &( pPlayer -> m_cEquipment );
				break;						
				/*			case CS2CContainerObjectMove::PEI_DEPOT:										
				pContainer = &( pPlayer -> m_cDepot);					
				break;	*/			
			case CS2CContainerObjectMove::PEI_PACK:
				pContainer =  pPlayer ->getSubpackContainer();
				break;
			case CS2CContainerObjectMove::PEI_PACK1:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK2:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK3:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK4:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
				break;
			case CS2CContainerObjectMove::PEI_PACK5:
				pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
				break;
			}
			if( pContainer )
			{
				pGoods=pContainer->GetGoods(lPos);
			}
			/*
			//װ�����е���Ʒ
			else if( lPos >= pPlayer ->getOriginPack()->GetGoodsAmountLimit() && lPos < 
			pPlayer ->getOriginPack()->GetGoodsAmountLimit() + CEquipmentContainer::s_dwEquipmentLimit )
			{
			pGoods = pPlayer -> m_cEquipment.GetGoods( lPos - pPlayer ->getOriginPack()->GetGoodsAmountLimit() );
			}*/
			if(pGoods && pGoods -> CanReparied())
			{
				DWORD dwPrice = CGoodsFactory::CalculateRepairPrice( pGoods );
				dwPrice = static_cast<DWORD>( dwPrice * ( 1 + pRegion -> GetTaxRate() / 100 ) );
				CMessage priceMsg(MSG_S2C_SHOP_REPAIRSINGLECOST);
				priceMsg.Add(dwPrice);
				priceMsg.SendToPlayer(pPlayer->GetExID());
			}
		}
		break;
	case MSG_C2S_SHOP_REPAIRALL:	//����ȫ����Ʒ
		{
			//##�����ж�NPC����
			CGUID NpcID; pMsg -> GetGUID(NpcID);
			CMonster* pNpc	= dynamic_cast<CMonster*>(pRegion->FindAroundObject(pPlayer, TYPE_MONSTER, NpcID));
			if( pNpc )
			{
				CTradeList::tagTrade *pTrade = CTradeList::GetTradeList(pNpc->GetOriginName());			
				if(!pTrade)
				{				
					CancelTrading( pPlayer );
					break;
				}
				if( pNpc -> Distance(pPlayer) > 10 )
				{
					pPlayer -> SendNotifyMessage( CStringReading::LoadString(22,5));
					CancelTrading( pPlayer );
					break;
				}
			}
			else
			{
				CancelTrading( pPlayer );
				break;
			}

			CGoodsRepairPriceListener lPrice;
			pPlayer ->getOriginPack()->TraversingContainer( &lPrice );
			pPlayer -> m_cEquipment.TraversingContainer( &lPrice );
			pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer->TraversingContainer(&lPrice);
			pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer->TraversingContainer(&lPrice);

			lPrice.m_dwPrice = static_cast<DWORD>( lPrice.m_dwPrice * ( 1 + pRegion -> GetTaxRate() / 100 ) );

			//##���û����Ҫ�������Ʒ
			if( lPrice.m_dwNumEquipments == 0 )
			{
				pPlayer -> SendNotifyMessage( CStringReading::LoadString(22,10) );
				break;
			}

			//���������ֽ�Ǯ���������������,����Ʒ�ĵ�ǰ�;�ֵ����Ϊ����;�ֵ
			if(pPlayer->GetSilver()>=lPrice.m_dwPrice)
			{
				pPlayer->SetSilver(pPlayer->GetSilver() - lPrice.m_dwPrice);


				CGoodsRepairListener lRepair;
				pPlayer ->getOriginPack()->TraversingContainer( &lRepair );
				//pPlayer -> m_cEquipment.TraversingContainer( &lRepair );
				pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer->TraversingContainer(&lRepair);
				pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer->TraversingContainer(&lRepair);
				pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer->TraversingContainer(&lRepair);
				pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer->TraversingContainer(&lRepair);
				pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer->TraversingContainer(&lRepair);
				for(size_t i = 0; i<CEquipmentContainer::EC_TOTAL; i++)
				{	
					BOOL bReMount = FALSE;
					CGoods* pGoods = pPlayer->m_cEquipment.GetGoods(i);
					if (pGoods&&pGoods->HasAddonProperty(GAP_GOODS_MAXIMUM_DURABILITY)&&pGoods->GetAddonPropertyValues( GAP_GOODS_MAXIMUM_DURABILITY, 2 )==0)
						bReMount = TRUE;
					if(CGoodsFactory::RepairEquipment(pGoods))
					{
						if (pGoods!=NULL&&bReMount)
						{
							DWORD  dwPosition = 0;
							pPlayer->GetEquipmentContainer()->QueryGoodsPosition(pGoods,dwPosition);
							pPlayer->MountEquipEx(pGoods,true);
							pPlayer->UpdateAttribute(TRUE);
							pPlayer->ProduceEvent(ET_EquipMount,
								CS2CContainerObjectMove::PEI_EQUIPMENT,
								dwPosition,
								pGoods->GetGoodsBaseType(), 
								pGoods-> GetAddonPropertyValues( GAP_WEAPON_CATEGORY, 1 ),
								(void*)CGoodsFactory::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex()));
						}
					}
				}
				CSubpackContainer* pSubpack=pPlayer->getSubpackContainer();	
				
				//GetGameLogInterface()->logT023_goods_log_repair(pPlayer,0,CGUID::GUID_INVALID,2);

				CMessage msg(MSG_S2C_SHOP_REPAIRALL);
				msg.Add((char)SHOP_REPAIR_SUCCEED);
				msg.Add(lPrice.m_dwPrice);
				msg.SendToPlayer(pPlayer -> GetExID());
			}
			else
			{
				pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,9));
			}
		}
		break;
	case MSG_C2S_SHOP_DURUPDATE://�̵�鿴�������ʱ��Ʒ��ǰ�;�ֵ�ĸ���(��ȷֵ)
		{
			//##�����ж�NPC����
			CGUID NpcID; pMsg -> GetGUID(NpcID);
			CMonster* pNpc	= dynamic_cast<CMonster*>( pMsg -> GetRegion() -> 
				FindAroundObject(pPlayer, TYPE_MONSTER, NpcID) );
			if( pNpc )
			{
				if( pNpc -> Distance(pPlayer) > 10 )
				{
					pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,5) );
					CancelTrading( pPlayer );
					break;
				}
			}
			else
			{
				CancelTrading( pPlayer );
				break;
			}

			//CMessage msg(MSG_S2C_SHOP_DURUPDATE);
			//list<CPlayer::tagGoods>::iterator GoodsIter=pPlayer->GetGoodsList().begin();
			//long lCount=0;
			////װ����
			//for(int i=0;i<CPlayer::EQUIP_SUM;i++)
			//{
			//	CGoods* pEquip=pPlayer->GetEquip(i);
			//	if(pEquip)
			//	{
			//		DWORD dwVal=pEquip->GetEffectVal(37,2);
			//		if(dwVal!=0 && dwVal!=pEquip->GetLastNotifyDur())						
			//			lCount++;					
			//	}
			//}
			//msg.Add(lCount);

			//
			//for(int i=0;i<CPlayer::EQUIP_SUM;i++)
			//{
			//	CGoods* pEquip=pPlayer->GetEquip(i);
			//	if(pEquip)
			//	{
			//		DWORD dwVal=pEquip->GetEffectVal(37,2);
			//		if(dwVal!=0 && dwVal!=pEquip->GetLastNotifyDur())
			//		{
			//			msg.Add(BYTE(48+i));
			//			msg.Add(dwVal);						
			//		}
			//	}
			//}

			////������
			//lCount=0;
			//for(;GoodsIter!=pPlayer->GetGoodsList().end();GoodsIter++)
			//{
			//	if(GoodsIter->bChange)
			//	{
			//		DWORD dwVal=GoodsIter->pGoods->GetEffectVal(37,2);
			//		if(dwVal!=0 && dwVal!=GoodsIter->pGoods->GetLastNotifyDur())									
			//			lCount++;					
			//	}
			//}
			//msg.Add(lCount);
			//for(;GoodsIter!=pPlayer->GetGoodsList().end();GoodsIter++)
			//{
			//	if(GoodsIter->bChange)
			//	{
			//		DWORD dwVal=GoodsIter->pGoods->GetEffectVal(37,2);
			//		if(dwVal!=0 && dwVal!=GoodsIter->pGoods->GetLastNotifyDur())
			//		{
			//			msg.Add(GoodsIter->lPos);
			//			msg.Add(dwVal);						
			//		}
			//	}
			//}

			////������Ʒ
			//if(pPlayer->GetHand()->bChange && pPlayer->GetHand()->pGoods)
			//{
			//	DWORD dwVal=pPlayer->GetHand()->pGoods->GetEffectVal(37,2);
			//	if(dwVal!=0 && dwVal!=pPlayer->GetHand()->pGoods->GetLastNotifyDur())
			//	{
			//		msg.Add((BYTE)true);
			//		msg.Add(dwVal);
			//	}
			//	else
			//		msg.Add((BYTE)false);
			//}		
			//msg.SendToPlayer((CMySocket*)(pPlayer->GetSocket()));
		}
		break;
	case MSG_C2S_SHOP_CLOSE:
		//##�ͻ��˸��߷�������Ҫ�ر��̵ꡣ
		{
			pPlayer -> SetCurrentProgress( CPlayer::PROGRESS_NONE );
			pPlayer -> SetMoveable();
		}
		break;
	case MSG_C2S_FBUSINESS_OPER:
		{
			GetInst( FBusiness::CShopManager ).OnMessage( pMsg );
		}
		break;
	}
}