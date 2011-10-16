#include "stdafx.h"

#include "../../../Net/Message.h"

#include "../Player.h"
#include "../ClientRegion.h"
#include "../Goods/Goods.h"
#include "../MainPlayerHand.h"
#include "../Other/AudioList.h"
#include "../TradeSystem/PlayerTrade.h"
#include "../../GameClient/Game.h"

extern void UpdataPlayerTradeItem(int index,DWORD dwWhose);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VOID OnContainerMessage( CMessage *pMsg )
{	
	CPlayer* pMainPlayer = pMsg -> GetPlayer();
	CClientRegion *pRegion = (CClientRegion *)pMsg->GetRegion();
	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
	
	if( pMainPlayer == NULL )
	{
		return;
	}
    long lMsgType = pMsg -> GetType();
	switch( lMsgType )
	{
	case MSG_S2C_CONTAINER_OBJECT_AMOUNT_CHANGE:	// ������֪ͨ�ͻ�����Ʒ�¼�����Ʒ�������ı�
		{
            CGUID SourceContainerID;
            LONG lSourceContainerType = pMsg->GetLong();
            pMsg->GetGUID(SourceContainerID);
            LONG lSourceContainerExtendID  = pMsg->GetLong();
            DWORD dwSourceContainerPosition = pMsg->GetDWord();
            LONG lObjectType = pMsg->GetLong();
            CGUID guID;
            pMsg->GetGUID(guID);
            DWORD dwAmount=pMsg->GetDWord();

            CGoods* pGoods = NULL;
            pGoods = pMainPlayer->GetGoodsByGUID(guID);
            if(pGoods==NULL&&lSourceContainerExtendID!=PEI_WALLET&&lSourceContainerExtendID!=PEI_SILVERWALLET)
                return;
            if(pGoods)
            {
                GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->GetIndex());
            }
            if(lSourceContainerExtendID==PEI_WALLET||lSourceContainerExtendID==PEI_SILVERWALLET)
            {
                GetGame()->GetAudioList()->Play2DSound("SOUNDS\\equipments\\00018.wav");
            }
            // ֻ������ֱ����Ч
            if( lSourceContainerType==TYPE_PLAYER && pMainPlayer->GetExID() == SourceContainerID)
            {
                if (lSourceContainerExtendID==PEI_PACKET||(lSourceContainerExtendID<=PEI_PACK5&&lSourceContainerExtendID>=PEI_PACK1))
                {
                    if (dwAmount > (DWORD)pGoods->GetNum())
                    {
                        char str[64];
                        sprintf(str,AppFrame::GetText("Package_5"),pGoods->GetName(),dwAmount-pGoods->GetNum());
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str);
                    }
                    else if (pGoods->GetProperty()->dwType!=GBT_CONSUMABLE && dwAmount < (DWORD)pGoods->GetNum())
                    { 
                        char str[64];
                        sprintf(str,AppFrame::GetText("Package_19"),pGoods->GetName(),pGoods->GetNum()-dwAmount);
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str);
                    }
                }else if ((lSourceContainerExtendID==PEI_WALLET ))
                {
                    if (dwAmount > GetGame()->GetMainPlayer()->GetMoney())
                    {
                        char str[64];
                        sprintf(str,AppFrame::GetText("Package_3"), dwAmount - GetGame()->GetMainPlayer()->GetMoney());
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str,0xfffff600);
                    }
                    else if (GetGame()->GetMainPlayer()->GetMoney()>dwAmount)
                    {
                        char str[64];
                        sprintf(str,AppFrame::GetText("Package_14"),GetGame()->GetMainPlayer()->GetMoney() - dwAmount);
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str);
                    }
                }else if ((lSourceContainerExtendID==PEI_SILVERWALLET ))
                {
                    if (dwAmount > GetGame()->GetMainPlayer()->GetSilverMoney())
                    {   
                        char str[64];
                        sprintf(str,AppFrame::GetText("Package_17"), dwAmount - GetGame()->GetMainPlayer()->GetSilverMoney());
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str,0xfffff600);
                        
                    }
                    else if (GetGame()->GetMainPlayer()->GetSilverMoney()>dwAmount)
                    {
                        char str[64];
                        sprintf(str,AppFrame::GetText("Package_18"),GetGame()->GetMainPlayer()->GetSilverMoney() - dwAmount);
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str);  
                    }
                }
                else if (lSourceContainerExtendID==PEI_YUANBAO)
                {
                    if (dwAmount > GetGame()->GetMainPlayer()->GetYuanBao())
                    {
                        
                        char str[256];
                        sprintf(str,AppFrame::GetText("Package_4"), dwAmount - GetGame()->GetMainPlayer()->GetYuanBao());
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str);  
                    }
                    else if (dwAmount < GetGame()->GetMainPlayer()->GetYuanBao())
                    {
                        char str[256];
                        sprintf(str,AppFrame::GetText("Package_15"),  GetGame()->GetMainPlayer()->GetYuanBao() - dwAmount);
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str);
                    }
                }
                //CGoods* pGoods = NULL;
                DWORD oldGoodsNum = 0;
                if (lSourceContainerExtendID!=PEI_WALLET&&lSourceContainerExtendID!=PEI_SILVERWALLET&&lSourceContainerExtendID!=PEI_YUANBAO)
                {
                    /*pGoods = pMainPlayer->GetGoodsByGUID(guID);*/
                    //if(pGoods==NULL)
                    //	return;
                    oldGoodsNum = pGoods->GetNum();		//��������Ϊ���ں����ж��Ƿ�����Ʒ�ڱ�����
                    ///////////////��Ʒ��ȡʱ����Ч//////////////
                    if (pGoods->GetProperty()!=NULL&&dwAmount>oldGoodsNum)
                    {
                        //pItemPage->AddPickGoodsEffect(pGoods->GetIndex());
                    }
                }
                pMainPlayer->ChangeObjectAmount(lSourceContainerExtendID, lObjectType, guID, dwAmount);

                if (lSourceContainerExtendID!=PEI_WALLET&&lSourceContainerExtendID!=PEI_SILVERWALLET&&lSourceContainerExtendID!=PEI_YUANBAO)
                {
                    // ˢ������ҳ��Ĳ�����ʾ
                    //CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
                    //if (pFacturePage!=NULL&&pFacturePage->IsOpen())
                    //{	
                    //    pFacturePage->UpdateMaterial();
                    //    pFacturePage->CalculateTotalNum();
					//    pFacturePage->UpdateFactureTree();
					//}
					//���¿�����ϵ���Ʒ��ʾ
					//CMainBarPageEx* pMainBarPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
					//if (pMainBarPage!=NULL)
					//{
					//    DWORD dwGoodsIndex = 0;
					//    for (int i = 0;i<24;i++)
					//    {
					//        dwGoodsIndex = pMainPlayer->GetHotKey(i);
					//        if (dwGoodsIndex!=0)
					//        {
					//            if (pGoods->GetIndex()==dwGoodsIndex)
					//            {
					//                pMainBarPage->UpdataHotComp(i);
					//            }	
					//        }
					//    }
					//}
				}
				//pMainPlayer->ChangeObjectAmount(lSourceContainerExtendID, lObjectType, guID, dwAmount);
				if (pHand->GetPGoodsOfMainPlayerHand()!=NULL)
				{
					if (guID==pHand->GetPGoodsOfMainPlayerHand()->GetExID())
					{
						pHand->GetTGoodsOfMainPlayerHand()->lNum = dwAmount;
					}
				}
			}
		}
		break;
	case MSG_S2C_CONTAINER_OBJECT_MOVE:	// ������֪ͨ�ͻ�����Ʒ�¼����ƶ���ɾ�����½�
		{
			BYTE opType = pMsg->GetByte();
			if( opType== OT_ROLL_BACK )
			{
				break;
			}

			CGUID guSourceObjectGUID;
			CGUID guDestinationObjectGUID;
			CGUID SourceContainerID,DestinationContainerID;
			// Դ
			LONG lSourceContainerType=pMsg->GetLong();
			pMsg->GetGUID(SourceContainerID);
			LONG lSourceContainerExtendID=pMsg->GetLong();
			DWORD dwSourceContainerPosition=pMsg->GetDWord();
			//Ŀ��
			LONG lDestinationContainerType=pMsg->GetLong();
			pMsg->GetGUID(DestinationContainerID);
			LONG lDestinationContainerExtendID=pMsg->GetLong();
			DWORD dwDestinationContainerPosition=pMsg->GetDWord();

			LONG lSourceObjectType = pMsg->GetLong();
			pMsg->GetGUID(guSourceObjectGUID);
			LONG lDestinationObjectType = pMsg->GetLong();
			pMsg->GetGUID(guDestinationObjectGUID);
			DWORD dwSourceAmount = pMsg->GetDWord();

			// �¼���Ʒ
			if( opType==OT_NEW_OBJECT )
			{
				//��ȡ��Ʒ���л�������

				DWORD dwSize=pMsg->GetDWord();
				if( dwSize==0 )
					break;
				BYTE *pBuff=new BYTE[dwSize];
				pMsg->GetEx(pBuff,dwSize);
				CGoods *pGoods=new CGoods;

				LONG lPos=0;
				if (pGoods!=NULL)
				{
					pGoods->DecordFromByteArray(pBuff,lPos);
				}
				SAFEDELETEARRAY(pBuff);

				// ��ҵõ���Ʒ
				if( lDestinationContainerType==TYPE_PLAYER )
				{
					// ����Ŀ��
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(pRegion->FindChildObject(TYPE_PLAYER, DestinationContainerID));
					if (pPlayer == NULL)
						break;
					CGoods ShowGoods(*pGoods);
					pPlayer->AddGoods(pGoods,dwDestinationContainerPosition,lDestinationContainerExtendID);

					// �¼ӵ���Ʒ�Ƿ��ڱ�������ģ�ͬʱ����Ʒ������Դ�ڱ������������������Ϣ
					if(IS_PACKETCONTAINER(lDestinationContainerExtendID) && !IS_PACKETCONTAINER(lSourceContainerExtendID))
					{
						pPlayer->UpdataQuestByGoods(&ShowGoods);
						// �Զ�װ������Ʒ
						pPlayer->AutoEquip(&ShowGoods,lSourceContainerExtendID,lDestinationContainerExtendID,dwDestinationContainerPosition);
					}
					// ����Ʒ�ǽ�һ�������ʱ���ڼӵ�������Ϻ��Ҫ����ɾ���������ڴ�й©
					if (lDestinationContainerExtendID==PEI_WALLET||lDestinationContainerExtendID==PEI_SILVERWALLET)
					{
						SAFEDELETE(pGoods);
					}
					//������Ʒ��Ч

					GetGame()->GetAudioList()->PlayEquipmentSound(ShowGoods.GetIndex()); 

					if(lSourceContainerExtendID==PEI_WALLET||lSourceContainerExtendID==PEI_SILVERWALLET)
					{
						GetGame()->GetAudioList()->Play2DSound("SOUNDS\\equipments\\00018.wav");
					}
					//���������Ʒ����������Ʒ����Ҫ����������ҵ�������Ʒ�б���
					if (/*pGoods!=NULL&&*/ShowGoods.GetGoodsBaseType()==GBT_CONSUMABLE&&(lDestinationContainerExtendID==PEI_PACKET||
						(lDestinationContainerExtendID>=PEI_PACK1&&lDestinationContainerExtendID<=PEI_PACK5)))
					{
						pPlayer->AddShortCutGoods(ShowGoods.GetIndex());
						/// ��ȡ������Ʒ����ȴ����
						DWORD dwRemainTime = pPlayer->GetGoodsRemainTime((WORD)ShowGoods.GetIndex());
						if (dwRemainTime!=0)
						{
							DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)ShowGoods.GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
							pPlayer->SetGoodsCoolDownStartTime((WORD)ShowGoods.GetIndex(),timeGetTime()-dwTimeElapse);
							pPlayer->SetGoodsUseState((WORD)ShowGoods.GetIndex(),false);
						}
						for(int i = 0;i<PACK_NUM;i++)
						{
							list<CPlayer::tagGoods>* GoodsList = pPlayer->GetGoodsList(i);
							list<CPlayer::tagGoods>::iterator GoodsIter = GoodsList->begin();
							for(;GoodsIter!=GoodsList->end();GoodsIter++)
							{
								if(GoodsIter->pGoods->GetEffectVal(GAP_COOLDOWN,2)==ShowGoods.GetEffectVal(GAP_COOLDOWN,2))
								{
									pPlayer->AddShortCutGoods(GoodsIter->pGoods->GetIndex());
									/// ��ȡ������Ʒ����ȴ����
									DWORD dwRemainTime = pPlayer->GetGoodsRemainTime((WORD)GoodsIter->pGoods->GetIndex());
									if (dwRemainTime!=0)
									{
										DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)GoodsIter->pGoods->GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
										pPlayer->SetGoodsCoolDownStartTime((WORD)GoodsIter->pGoods->GetIndex(),timeGetTime()-dwTimeElapse);
										pPlayer->SetGoodsUseState((WORD)GoodsIter->pGoods->GetIndex(),false);
									}
								}
							}
						}
					}
					//���¿�����ϵ���Ʒ��ʾ
					//CMainBarPageEx* pPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
					//if (pPage!=NULL)
					//{
					//    DWORD dwGoodsIndex = 0;
					//    for (int i = 0;i<24;i++)
					//    {
					//        dwGoodsIndex = pPlayer->GetHotKey(i);
					//        if (dwGoodsIndex!=0)
					//        {
					//            if (/*pGoods!=NULL&&*/ShowGoods.GetIndex()==dwGoodsIndex)
					//            {
					//                pPage->UpdataHotComp(i);								
					//            }
					//        }
					//    }
					//}
					// ˢ������ҳ��Ĳ�����ʾ
					//CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
					//if (pFacturePage!=NULL&&pFacturePage->IsOpen())
					//{	
					//    pFacturePage->UpdateMaterial();
					//    pFacturePage->CalculateTotalNum();
					//    pFacturePage->UpdateFactureTree();
					//}
					//if( lDestinationContainerExtendID==PEI_PACKET||(lDestinationContainerExtendID<=PEI_PACK5&&lDestinationContainerExtendID>=PEI_PACK1) )
					//{
					//    if (CStringReading::LoadText(200,5)!=NULL)
					//    {
					//        char str[256];
					//        sprintf(str,CStringReading::LoadText(200,5),ShowGoods.GetName(),ShowGoods.GetNum());
					//        GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(str);
					//    }
					//    ///////////////��Ʒ��ȡʱ����Ч//////////////
					//    if (ShowGoods.GetProperty()!=NULL)
					//    {
					//        pItemPage->AddPickGoodsEffect(ShowGoods.GetIndex());	
					//    }
					//}else if (lDestinationContainerExtendID == PEI_WALLET) //���
					//{
					//    char strValue[64];
					//    if (CStringReading::LoadText(200,3)!=NULL)
					//    {
					//        sprintf(strValue,CStringReading::LoadText(200,3),dwSourceAmount);
					//        GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(strValue);
					//    }
					//}
					//else if (lDestinationContainerExtendID == PEI_SILVERWALLET)
					//{
					//    char strValue[64];
					//    if (CStringReading::LoadText(200,17)!=NULL)
					//    {
					//        sprintf(strValue,CStringReading::LoadText(200,17),dwSourceAmount);
					//    }
					//}
					//else if (lDestinationContainerExtendID == PEI_YUANBAO)	//Ԫ��
					//{
					//    pPlayer->SetYuanBao(dwSourceAmount);
					//}
				}
				// ����������Ʒ
				else if( lDestinationContainerType==TYPE_REGION )
				{
					pRegion->AddGoods(pGoods,dwDestinationContainerPosition,lDestinationContainerExtendID);
				}
				// �Ự��Ʒ
				else if( lDestinationContainerType==TYPE_SESSION )
				{
					//����
					if(DestinationContainerID == GetInst(PlayerTrade).GetSessionID())
					{
						CPlayer* pPlayer = GetGame()->GetMainPlayer();
						if (lSourceContainerExtendID == PEI_WALLET)			//���
						{
							if (SourceContainerID!=pPlayer->GetExID())
							{
								if (g_bDebug)
								{
									/// �������������Ʒ�ƶ�����Ϣ
									char strTitle[64] = "";
									strcpy_s(strTitle,"--��������ͻ��˷��͵Ľ��׽�Ǯ��Ϣ--");
									char strPrintInfor[512] = "";
									char strSessionID[64] = "";
									char strGoodsGUID[64] = "";
									DestinationContainerID.tostring(strSessionID);
									pGoods->GetExID().tostring(strGoodsGUID);
									sprintf_s(strPrintInfor,"%s\n%s%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%s\n%s%d\n",
										strTitle,
										"�˴ν��׵�SessionID��:",strSessionID,
										"���ԭ�����ڵ�����ID:",lSourceContainerExtendID,
										"�����ԭ�������е�λ��:",dwSourceContainerPosition,
										"���Ҫ�ƶ�����Ŀ������ID:",lDestinationContainerExtendID,
										"���Ҫ�ƶ���Ŀ��������λ��:",dwDestinationContainerPosition,
										"���β����Ľ�ҵ�GUID:",strGoodsGUID,
										"���β����Ľ������:",dwSourceAmount);
									PutStrToFileSaveToDir( "log/GoldsTradeLog", "��¼��ҵĽ��׹���", strPrintInfor, false );
								}

								//GetInst(PlayerTrade).UpdataTradeInfo(PlayerTrade::PT_Money,dwSourceAmount,PlayerTrade::Trade_Other);
								GetInst(PlayerTrade).SetOtherGold(dwSourceAmount);
                            }
                            break;
                        }
                        else if (lSourceContainerExtendID == PEI_YUANBAO)
                        {
                            if (SourceContainerID!=pPlayer->GetExID())
                               GetInst(PlayerTrade).UpdataTradeInfo(PlayerTrade::PT_YuanBao,dwSourceAmount,PlayerTrade::Trade_Other);
                            break;
                        }
                        else
                        {
                            if (SourceContainerID==GetGame()->GetMainPlayer()->GetExID())
							{
								GetInst(PlayerTrade).AddMyTradeGoods(pGoods,(int)dwDestinationContainerPosition);
								UpdataPlayerTradeItem((int)dwDestinationContainerPosition,PlayerTrade::Trade_Me);
							}
							else
							{
								GetInst(PlayerTrade).AddOtherTradeGoods(pGoods,(int)dwDestinationContainerPosition);
								UpdataPlayerTradeItem((int)dwDestinationContainerPosition,PlayerTrade::Trade_Other);
							}
                            break;
                        }
                    }
                }
                else
                {
                    delete pGoods;
                }
            }
            // ɾ����Ʒ
            else if( opType==OT_DELETE_OBJECT )
            {
                //��ȡ��Ʒ����				
                DWORD dwDestinationAmount=pMsg->GetDWord();
                CGoods *pGoods=NULL;

                // ɾ�������Ʒ
                if( lSourceContainerType==TYPE_PLAYER )
                {
                    // ����Ŀ��
                    CPlayer* pPlayer = dynamic_cast<CPlayer*>(pRegion->FindChildObject(TYPE_PLAYER, SourceContainerID));
                    if (pPlayer == NULL)
                        break;

                    pGoods = pPlayer->RemoveGoods( guSourceObjectGUID,guDestinationObjectGUID,dwSourceContainerPosition,lSourceContainerExtendID,dwSourceAmount);
                    // ɾ�����������Ʒ��ˢ���������
                    if(IS_PACKETCONTAINER(lSourceContainerExtendID))
                    {
                        if(pGoods)
                            pPlayer->UpdataQuestByGoods(pGoods);
                    }
                    //���¿�����ϵ���ʾ
                    //CMainBarPageEx* pPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
                    //if (pPage!=NULL)
                    //{
                    //    DWORD dwGoodsIndex = 0;
                    //    for (int i = 0;i<24;i++)
                    //    {
                    //        dwGoodsIndex = pMainPlayer->GetHotKey(i);
                    //        if (dwGoodsIndex!=0)
                    //        {
                    //            if (pGoods!=NULL&&pGoods->GetIndex()==dwGoodsIndex)
                    //            {
                    //                pPage->UpdataHotComp(i);
                    //            }
                    //        }
                    //    }	
                    //}
                    //�Ѵ���Ʒ�����Ŵ���ȴ�б���ɾ��
                    if (pGoods!=NULL&&CGoodsList::GetProperty(pGoods->GetIndex())->BaseProperty.dwType==GBT_CONSUMABLE
                        &&pPlayer->GetGoodsAmount(pGoods->GetIndex())==0&&pPlayer->GetGoodsUseState(pGoods->GetIndex()))
                    {
                        pPlayer->SetGoodsCoolDownStartTime((WORD)pGoods->GetIndex(),0);
                        //pPlayer->SetGoodsUseState(pGoods->GetIndex(),false);
                    }

                    /*CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
                    if (pFacturePage!=NULL&&pFacturePage->IsOpen())
                    {
                    pFacturePage->UpdateMaterial();
                    pFacturePage->CalculateTotalNum();
                    pFacturePage->UpdateFactureTree();
                    }*/
                }
                // ɾ��������Ʒ
                else if( lSourceContainerType ==TYPE_REGION )
                {                
                    pGoods=pRegion->RemoveGoods( guSourceObjectGUID,dwSourceContainerPosition,lSourceContainerExtendID,dwSourceAmount);
                }

                if(pGoods)
                {
                    SAFE_DELETE(pGoods);
                }
            }
            // �ƶ���Ʒ
            else if( opType==OT_MOVE_OBJECT )
            {
                if (g_bDebug)
                {
                    /// �������������Ʒ�ƶ�����Ϣ
                    char strTitle[64] = "";
                    //if (GetGame()->GetCGuiEx()->GetItemPageEx()->GetFinishBagState())
                    //{
                    //    strcpy_s(strTitle,"--��������ͻ��˷��͵ģ�������Ʒ�ƶ���Ϣ--");
                    //}
                    //else
                    //    strcpy_s(strTitle,"--��������ͻ��˷��͵���Ʒ����ͨ���ƶ���Ϣ--");
                    char strPrintInfor[512] = "";
                    char strGoodsGUID[64] = "";
                    guSourceObjectGUID.tostring(strGoodsGUID);
                    sprintf_s(strPrintInfor,"%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%s\n%s%d\n",
                        strTitle,
                        "��Ʒԭ�����ڵ�����ID:",lSourceContainerExtendID,
                        "��Ʒ��ԭ�������е�λ��:",dwSourceContainerPosition,
                        "��ƷҪ�ƶ�����Ŀ������ID:",lDestinationContainerExtendID,
                        "��ƷҪ�ƶ���Ŀ��������λ��:",dwDestinationContainerPosition,
                        "���β�������Ʒ��GUID:",strGoodsGUID,
                        "���β�������Ʒ����:",dwSourceAmount);
                    PutStrToFileSaveToDir( "log/GoodsMoveLog", "��¼��Ʒ���ƶ�����", strPrintInfor, false );
                    //////////////////////////////
                }

                // ��ȡ��Ʒ����
                //DWORD dwSourceAmount=pMsg->GetDWord();
                DWORD dwDestinationAmount=pMsg->GetDWord();
                CGoods *pGoods=NULL;

                // �������������Ʒ
                if( lSourceContainerType==TYPE_PLAYER )
                {
                    // ����Ŀ�ֻ꣬���Լ���Ч
                    CPlayer* pPlayer = dynamic_cast<CPlayer*>(pRegion->FindChildObject(TYPE_PLAYER, SourceContainerID));
                    if (pPlayer != pMainPlayer)
                        break;
                    pGoods = pPlayer->RemoveGoods( guSourceObjectGUID,guDestinationObjectGUID,dwSourceContainerPosition,lSourceContainerExtendID,dwSourceAmount);
                    // ����Ʒ�ӱ������ƶ�����������,ˢ��������Ϣ
                    if(IS_PACKETCONTAINER(lSourceContainerExtendID) && !IS_PACKETCONTAINER(lDestinationContainerExtendID))
                    {
                        if(pGoods)
                            pPlayer->UpdataQuestByGoods(pGoods);
                    }
                    // ˢ������ҳ��Ĳ�����ʾ
                    //CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
                    //if (pFacturePage!=NULL&&pFacturePage->IsOpen())
                    //{	
                    //    pFacturePage->UpdateMaterial();
                    //    pFacturePage->CalculateTotalNum();
                    //    pFacturePage->UpdateFactureTree();
                    //}

                    //�Ѵ���Ʒ�����Ŵ���ȴ�б���ɾ��
                    if (pGoods!=NULL&&CGoodsList::GetProperty(pGoods->GetIndex())->BaseProperty.dwType==GBT_CONSUMABLE
                        &&pPlayer->GetGoodsAmount(pGoods->GetIndex())==0&&pPlayer->GetGoodsUseState(pGoods->GetIndex()))
                    {
                        pPlayer->SetGoodsCoolDownStartTime((WORD)pGoods->GetIndex(),0);
                        //pPlayer->SetGoodsUseState(pGoods->GetIndex(),false);
                    }

                    if( lSourceContainerExtendID== PEI_MEDAL )
                    {
                        //pGoods = GetGame()->GetCGuiEx()->GetCharacterPageEx()->GetGoods(dwSourceContainerPosition);
                    }
                    //GetGame()->GetCGuiEx()->GetCharacterPageEx()->SetTitleShow();
                }
                // �ӵ��ϼ���
                else if( lSourceContainerType==TYPE_REGION )
                {
                    pGoods=pRegion->RemoveGoods( guSourceObjectGUID,dwSourceContainerPosition,lSourceContainerExtendID,dwSourceAmount);
                }
                if( pGoods==NULL )
                {
                    break;
                }

                // ׼�����Ƴ�����Ʒ�ƶ���Ŀ��ص�
                if( guDestinationObjectGUID != CGUID::GUID_INVALID )
                {
                    pGoods->SetExID(guDestinationObjectGUID);
                }
                // �ƶ���������
                if( lDestinationContainerType==TYPE_PLAYER )
                {
                    //CMainBarPageEx* pMainBarPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
                    // ����Ŀ�꣬����ֻ���Լ���Ч
                    CPlayer* pPlayer = dynamic_cast<CPlayer*>(pRegion->FindChildObject(TYPE_PLAYER, DestinationContainerID));
                    if (pPlayer != pMainPlayer)
                        break;

                    // �ӵ��ϻ����Ʒ
                    if( (lDestinationContainerExtendID==PEI_PACKET||(lDestinationContainerExtendID<=PEI_PACK5&&lDestinationContainerExtendID>=PEI_PACK1)) && lSourceContainerType==TYPE_REGION )
                    {

                        char str[256];
                        if(CGoodsList::GetEffectVal(pGoods->GetIndex(),GAP_MAXOVERLAP,1)>0)
                            sprintf(str,AppFrame::GetText("Package_5"),pGoods->GetName(),pGoods->GetNum());
                        else
                            sprintf(str,AppFrame::GetText("Package_6"),pGoods->GetName());
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(str);
                        GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->GetIndex());

                        /////��ȡ��Ʒʱ����Ч/////
                        if (pGoods->GetProperty()!=NULL)
                        {
                            //pItemPage->AddPickGoodsEffect(pGoods->GetIndex());
                        }
                    }
                    // �ӵ��ϻ�ý�Ǯ
                    else if( (lDestinationContainerExtendID==PEI_WALLET||lDestinationContainerExtendID==PEI_SILVERWALLET) && lSourceContainerType==TYPE_REGION )
                    {
                        char str[128];
                        if (lDestinationContainerExtendID==PEI_WALLET)
                        {
                            sprintf(str,AppFrame::GetText("Package_3"),dwSourceAmount);
                        }
                        else if (lDestinationContainerExtendID==PEI_SILVERWALLET)
                        {
                            sprintf(str,AppFrame::GetText("Package_17"),dwSourceAmount);
                        }
                        //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(str);					
                        pGoods->SetNum(dwSourceAmount);
                    }
                    //if (pPlayer->GetMoney() == 0)
                    //	pPlayer->SetguID(lDestinationContainerExtendID,pGoods->GetExID());

                    //��ȡ��Ʒ���л�������
                    if (pGoods!=NULL)
                    {	
                        /// �ж��ƶ���������ϵ��Ƿ�����Ҫ���±������Ʒ
                        if ( dwDestinationAmount!=0)
                        {
                            BYTE *pBuff=new BYTE[dwDestinationAmount];
                            pMsg->GetEx(pBuff,dwDestinationAmount);
                            LONG lPos=0;
                            if (pGoods!=NULL)
                            {
                                pGoods->DecordFromByteArray(pBuff,lPos);
                            }
                            SAFEDELETEARRAY(pBuff);
                        }
                    }

                    CGoods ShowGoods(*pGoods);
                    pPlayer->AddGoods( pGoods,dwDestinationContainerPosition,lDestinationContainerExtendID );
                    // ����Ʒ�����������ƶ���������,ˢ��������Ϣ
                    if(!IS_PACKETCONTAINER(lSourceContainerExtendID) && IS_PACKETCONTAINER(lDestinationContainerExtendID))
                    {
                        pPlayer->UpdataQuestByGoods(&ShowGoods);
                        // �Զ�װ������Ʒ
                        pPlayer->AutoEquip(&ShowGoods,lSourceContainerExtendID,lDestinationContainerExtendID,dwDestinationContainerPosition);
                    }					
                    ///// �����ڱ�������״̬ʱ�ͼ���ִ����ȥ������ֱ��������ϡ���
                    //if (pItemPage!=NULL&&pItemPage->GetFinishBagState())
                    //{
                    //	if (pItemPage->CanFinishBag()&&pItemPage->GoodsSort())
                    //	{
                    //		pItemPage->SendMsgForFinishBag();
                    //	}
                    //}

                    //GetGame()->GetCGuiEx()->GetCharacterPageEx()->UpdateCoolIcon();
                    //GetGame()->GetCGuiEx()->GetCharacterPageEx()->UpdateEquipButton();
                    GetGame()->GetAudioList()->PlayEquipmentSound(ShowGoods.GetIndex());

                    // ����Ʒ�ǽ�һ�������ʱ���ڼӵ�������Ϻ��Ҫ����ɾ���������ڴ�й©
                    if (lDestinationContainerExtendID==PEI_WALLET||lDestinationContainerExtendID==PEI_SILVERWALLET)
                    {
                        SAFEDELETE(pGoods);
                    }

                    // ˢ������ҳ��Ĳ�����ʾ
                    //CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
                    //if (pFacturePage!=NULL&&pFacturePage->IsOpen())
                    //{	
                    //    pFacturePage->UpdateMaterial();
                    //    pFacturePage->CalculateTotalNum();
                    //    pFacturePage->UpdateFactureTree();
                    //}

                    //���������Ʒ����������Ʒ����Ҫ����������ҵ�������Ʒ�б���
                    if (ShowGoods.GetGoodsBaseType()==GBT_CONSUMABLE&&(lDestinationContainerExtendID==PEI_PACKET||
                        (lDestinationContainerExtendID>=PEI_PACK1&&lDestinationContainerExtendID<=PEI_PACK5)))
                    {
                        pPlayer->AddShortCutGoods(ShowGoods.GetIndex());
                        /// ��ȡ������Ʒ����ȴ����
                        DWORD dwRemainTime = pPlayer->GetGoodsRemainTime((WORD)ShowGoods.GetIndex());
                        if (dwRemainTime!=0)
                        {
                            DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)ShowGoods.GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
                            pPlayer->SetGoodsCoolDownStartTime((WORD)ShowGoods.GetIndex(),timeGetTime()-dwTimeElapse);
                            pPlayer->SetGoodsUseState((WORD)ShowGoods.GetIndex(),false);
                        }
                        for(int i = 0;i<PACK_NUM;i++)
                        {
                            list<CPlayer::tagGoods>* GoodsList = pPlayer->GetGoodsList(i);
                            list<CPlayer::tagGoods>::iterator GoodsIter = GoodsList->begin();
                            for(;GoodsIter!=GoodsList->end();GoodsIter++)
                            {
                                if(GoodsIter->pGoods->GetEffectVal(GAP_COOLDOWN,2)==ShowGoods.GetEffectVal(GAP_COOLDOWN,2))
                                {
                                    pPlayer->AddShortCutGoods(GoodsIter->pGoods->GetIndex());
                                    /// ��ȡ������Ʒ����ȴ����
                                    DWORD dwRemainTime = pPlayer->GetGoodsRemainTime((WORD)GoodsIter->pGoods->GetIndex());
                                    if (dwRemainTime!=0)
                                    {
                                        DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)GoodsIter->pGoods->GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
                                        pPlayer->SetGoodsCoolDownStartTime((WORD)GoodsIter->pGoods->GetIndex(),timeGetTime()-dwTimeElapse);
                                        pPlayer->SetGoodsUseState((WORD)GoodsIter->pGoods->GetIndex(),false);
                                    }
                                }
                            }
                        }
                    }

                }
                // �ƶ���������
                else if(lDestinationContainerType==TYPE_REGION )
                {
                    pRegion->AddGoods( pGoods,dwDestinationContainerPosition,lDestinationContainerExtendID );
                }
                else
                {
                    SAFE_DELETE(pGoods);
                }
            }
            else if (opType==OT_SWITCH_OBJECT)
            {
                // ������Ϣ�����ߣ�����ֻ���Լ���Ч
                CPlayer* pPlayer = dynamic_cast<CPlayer*>(pRegion->FindChildObject(lSourceContainerType, SourceContainerID));
                if (pPlayer != pMainPlayer)
                    break;

                if(lSourceContainerType!=TYPE_PLAYER)
                {
                    break;
                }

                int sPos = 0;
                int dPos = 0;

                bool bIsMedal = false;
                CGoods *pGoods1 = NULL;
                CGoods *pGoods2 = NULL;
                //�жϽ�������Ʒ�Ƿ���ѫ��
                if(lSourceContainerExtendID == PEI_MEDAL)
                {
                    bIsMedal = true;
                    //pGoods1 = GetGame()->GetCGuiEx()->GetCharacterPageEx()->GetGoods(dwSourceContainerPosition);
                    CGoods *pMedalOnPlayer = pPlayer->GetEquip(dwDestinationContainerPosition);
                    if(pMedalOnPlayer)
                    {
                        DWORD dwDesGoods = pMedalOnPlayer->GetProperty()->dwIndex; 
                        //pGoods2 = GetGame()->GetCGuiEx()->GetCharacterPageEx()->GetGoods(dwDesGoods);
                    }

                    sPos = -1;
                    dPos = -1;
                }
                else
                {
                    pGoods1=pPlayer->GetGoodsByGUID(guSourceObjectGUID,&sPos);
                    pGoods2=pPlayer->GetGoodsByGUID(guDestinationObjectGUID,&dPos);
                }

                if(pGoods2!=NULL&&pGoods1!=NULL)
                {
                    pPlayer->RemoveGoods( guDestinationObjectGUID,guDestinationObjectGUID,dPos,lDestinationContainerExtendID,pGoods2->GetNum());
                    pPlayer->RemoveGoods(guSourceObjectGUID,guSourceObjectGUID, sPos, lSourceContainerExtendID, pGoods1->GetNum());
                    pPlayer->AddGoods(pGoods1,dwDestinationContainerPosition,lDestinationContainerExtendID);
                    pPlayer->AddGoods(pGoods2,dwSourceContainerPosition,lSourceContainerExtendID);
                    GetGame()->GetAudioList()->PlayEquipmentSound(pGoods1->GetIndex());
                    // ����ѫ��
                    if(!bIsMedal)
                    {
                        // ���Ǳ����ڵ���Ʒ������ˢ��������Ϣ
                        if(!(IS_PACKETCONTAINER(lSourceContainerExtendID) && IS_PACKETCONTAINER(lDestinationContainerExtendID)))
                        {
                            pPlayer->UpdataQuestByGoods(pGoods1);
                            pPlayer->UpdataQuestByGoods(pGoods2);
                        }
                    }
                }
            }
          }
        break;
	case MSG_S2C_CONTAINER_OBJECT_UNLOCK:	//������֪ͨ�ͻ��˸���Sessionҳ�����Ʒ�������������Ϣ
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);
			long lPlugID = pMsg->GetLong();
			long lPos = pMsg->GetLong();
			CGUID GoodsGUID;
			pMsg->GetGUID(GoodsGUID);
			CPlayer* pPlayer = GetGame()->GetMainPlayer();
			if(pPlayer==NULL) 
				return;

			//��Ƕ��Ƭ
			//CCardMutifunPageEx* pMutiPage = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
			//if (pMutiPage!=NULL&&pMutiPage->IsOpen()&&SessionID==pMutiPage->GetSessionID())
			//{
			//	if (lPos==CCardMutifunPageEx::EEPC_EQUIPMENT)
			//	{
			//		pMutiPage->RemoveGoodsFromPage();
			//		pMutiPage->UpdatePageForGoods();
			//	}else if (lPos>=CCardMutifunPageEx::EEPC_CARD1&&lPos<=CCardMutifunPageEx::EEPC_CARD5)
			//	{
			//		BYTE index = (BYTE)(lPos-CCardMutifunPageEx::EEPC_CARD1);
			//		pMutiPage->RemoveCardFromPage(index);
			//		pMutiPage->UpdatePage();
			//		pMutiPage->UpdateAppointHole(index);
			//	}else if (lPos==CCardMutifunPageEx::EEPC_YUANBAO_ITEM1)
			//	{
			//		pMutiPage->RemoveAssGoodsFromPage();
			//		pMutiPage->UpdateAssGoodsShow();
			//	}
			//	if (pCharacterPage!=NULL)
			//	{
			//		pCharacterPage->UpdataEquip();
			//	}
			//}
			// ����
			/*else */if (SessionID==GetInst(PlayerTrade).GetSessionID())
			{
				CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
				if(lPlugID == GetInst(PlayerTrade).GetMyPlugID())
				{
					long lExtendID = 0;
					int iPos = -1;
					GetInst(PlayerTrade).RemoveTradeGoods(lPos, PlayerTrade::Trade_Me);

					CGoods* pRemoveGoods = pMainPlayer->GetGoodsByGUID(GoodsGUID,&iPos,&lExtendID);
					if (pRemoveGoods!=NULL)
					{
						// Ϊ�Ƴ����׽������Ʒ����
						if (pRemoveGoods->IsLocked())
						{
							pRemoveGoods->UnLock();
						}

						// ���������Ƴ�����Ʒͼ��ʱ�������е�ͼ�겻������������Ϊ�رս��׽���ʱ�Ƴ���Ʒ����ʱ��Ʒ�ڱ����е�
						// ͼ��Ҫ����
						if (pHand!=NULL&&pRemoveGoods==pHand->GetPGoodsOfMainPlayerHand())
						{
							pRemoveGoods->SetHaveShadowState(true);
						}else
							pRemoveGoods->SetHaveShadowState(false);
					}
				}
				else
				{
					GetInst(PlayerTrade).RemoveTradeGoods(lPos, PlayerTrade::Trade_Other);
				}
			}
		}
		break;

	case MSG_S2C_CONTAINER_CLEAR:	//������֪ͨ�ͻ�����Ʒ�¼������������������Ʒ
		
		break;		

	}
}