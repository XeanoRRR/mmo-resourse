#include "stdafx.h"
#include "../ClientRegion.h"
#include "../Player.h"
#include "../Goods/Goods.h"
//#include "Message.h"
#include "../../Game.h"
//#include "Other/Chat.h"
#include "../../GameControl.h"
#include "Other/AudioList.h"
#include "../../Public/Common/Public.h"
//#include "../../Public/Common/StrType.h"
#include "../TradeSystem/PlayerShop.h"
#include "../../../UI/GamePage/ChatList/ChatListPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 * ����: ��Ӧ��ҿ�����Ϣ
 * ժҪ: -
 * ����: pMsg - ���������͵���Ϣ
 * ����ֵ: -
 * ����: -
 * ��������: -
 * �޸���־:
 */

void OnPlayerShopMessage(CMessage* pMsg)
{
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	if (!pMainPlayer) return;

	PlayerShop& playerShop = GetPlayerShop();
	
	switch(pMsg->GetType()) 
	{
		// �򿪸����̵�
	case MSG_S2C_PALYERSHOP_CREATE:
		{
			CGUID sessionID;
			pMsg->GetGUID(sessionID);
			long plugID = pMsg->GetLong();
			CGUID masterID = pMainPlayer->GetExID();

			pMainPlayer->SetPlayerShopSessionID(sessionID);
			pMainPlayer->SetPlayerShopPlugID(plugID);
			
			// �򿪸����̵�
			playerShop.SetCurShopState(PlayerShop::SET_SHOP);
			playerShop.SetShopPara(sessionID, plugID, masterID);
			FireUIEvent("PlayerShop", "Opened");
		}
		break;

		// ��ʼ��̯
	case MSG_S2C_PLAYERSHOP_OPENFORBUSINESS:
		{
			CGUID PlayerID,SessionID;
			pMsg->GetGUID(PlayerID);
			pMsg->GetGUID(SessionID);
			long lPlugID = pMsg->GetLong();
			CPlayer *pPlayer=(CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
			if(pPlayer==NULL)
				break;

			GetGame()->GetGameControl()->AutoMoveStop();
			GetGame()->GetGameControl()->AutoAttactStop();
			GetGame()->GetGameControl()->StopMove();
			pPlayer->SetPlayerShopOpen(true);				
			pPlayer->SetPlayerShopSessionID(SessionID);
			pPlayer->SetPlayerShopPlugID(lPlugID);
			if (pPlayer==GetGame()->GetMainPlayer()) 
			{		
				playerShop.SetCurShopState(PlayerShop::OPEN_SHOP);
				//playerShop->OpenShop(SessionID,lPlugID,PlayerID);

				// ����ʱ,����Ҳ������µ���̬����
				//if (pPlayer->GetActionIndexOfActionAnim() != 0 ||
				//	(pPlayer->GetActionIndexOfActionAnim() == 0 && !pPlayer->GetPlayStateOfActionAnim()))
				//{
				//	CMessage msg(MSG_C2S_OTHER_FACEACTION);
				//	msg.Add((unsigned long)1);
				//	msg.Add((long)0);
				//	msg.Add(NULL_GUID);
				//	msg.Send();
				//}

				// ���²������水ť״̬
				FireUIEvent("PlayerShop", "UpdateOperBtn");
				playerShop.UpdataPlayerShopGoodsList();
			}
		}
		break;

		// ��������ҵĸ����̵�ҳ��
	case MSG_S2C_PLAYERSHOP_LOOKGOODS:
		{
			CGUID SessionID;
			CGUID MasterID;		//������GUID
			pMsg->GetGUID(SessionID);
			pMsg->GetGUID(MasterID);
			long lBuyerPlugID = pMsg->GetLong();
			pMainPlayer->SetPlayerShopSessionID(SessionID);
			pMainPlayer->SetPlayerShopPlugID(lBuyerPlugID);				
			BYTE ByteData[102400];
			long lBytePos = 0;
			pMsg->GetEx(ByteData,102400);
			DWORD dwNumber = _GetDwordFromByteArray(ByteData,lBytePos);
			for(size_t i=0;i<dwNumber;i++)
			{
				CGoods *pGoods = new CGoods();
				pGoods->DecordFromByteArray(ByteData,lBytePos,true);
				DWORD dwDealType = _GetDwordFromByteArray(ByteData,lBytePos);
				DWORD dwPriceType = _GetDwordFromByteArray(ByteData,lBytePos);
				DWORD dwTeamNum = _GetDwordFromByteArray(ByteData,lBytePos);
				DWORD dwGoodsNumPerTeam = _GetDwordFromByteArray(ByteData,lBytePos);
				DWORD dwPrice = _GetDwordFromByteArray(ByteData,lBytePos);
				playerShop.AddPlayerShopGoods(pGoods,dwTeamNum,dwPriceType,dwPrice,dwDealType,dwGoodsNumPerTeam);
			}
            GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-30.wav");

			playerShop.SetCurShopState(PlayerShop::SHOPPING_SHOP);
			playerShop.SetShopPara(SessionID,lBuyerPlugID,MasterID);

			FireUIEvent("PlayerShop", "Opened");
		}
		break;

		// �ر��̵�
	case MSG_S2C_PLAYERSHOP_CLOSE:
		{
			CGUID PlayerID,SessionID;
			pMsg->GetGUID(PlayerID);
			pMsg->GetGUID(SessionID);
			CPlayer *pPlayer=(CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
			if(pMainPlayer==NULL)break;
			if (pPlayer!=NULL)
			{
				pPlayer->SetPlayerShopOpen(false);

				// ����ȡ��ʱ,����Ҳ���վ������
				pPlayer->SetAction(CMoveShape::ACT_STAND);
			}

			if (SessionID == playerShop.GetSessionID())
			{
                GetChatListPage().AddChatWords(AppFrame::GetText("Shop_7"),//����ֹͣ��̯
                    0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				playerShop.CloseShop();
				FireUIEvent("PlayerShop", "Closed");
			}
		}
		break;

		// ����ȡ����̯
	case MSG_S2C_PLAYERSHOP_CLOSEDOWN:
		{
			CGUID PlayerID,SessionID;
			pMsg->GetGUID(PlayerID);
			pMsg->GetGUID(SessionID);
			CPlayer *pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
			if(pPlayer==NULL) break;
			pPlayer->SetPlayerShopOpen(false);

			if(PlayerID==GetGame()->GetMainPlayer()->GetExID()
				&&SessionID==playerShop.GetSessionID()
				&&playerShop.GetCurShopState()==PlayerShop::OPEN_SHOP)
			{
				playerShop.SetbSetTrue(false);
				playerShop.SetCurShopState(PlayerShop::SET_SHOP);
				playerShop.CancelAllTrade();
				//playerShop.Open();
			}
			else if (playerShop.GetSessionID()==SessionID
				&&playerShop.GetCurShopState()==PlayerShop::SHOPPING_SHOP)
			{
				playerShop.CloseShop();
			}

			GetChatListPage().AddChatWords(AppFrame::GetText("Shop_7")  //����ֹͣ��̯
                , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
		}
		break;

		// �˳������̵�
	case MSG_S2C_PLAYERSHOP_QUIT:
		{
			CGUID SessionID;
			pMsg->GetGUID(SessionID);

			if (/*playerShop.IsOpen()&&*/playerShop.GetCurShopState()==PlayerShop::SHOPPING_SHOP)
			{
				CPlayer* pPlayer = (CPlayer*)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,playerShop.GetMasterID());
				if (pPlayer!=NULL&&pMainPlayer->Distance(pPlayer)>6)
				{
					GetChatListPage().AddChatWords(AppFrame::GetText("Shop_9"), //�����̫Զ��
                        0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				}
			}

			if (playerShop.GetSessionID()==SessionID
				&&playerShop.GetCurShopState()==PlayerShop::SHOPPING_SHOP)
			{
				playerShop.CloseShop();
			}
		}
		break;

		// �����
	case MSG_S2C_PLAYERSHOP_TOBUYER:
		{
			DWORD lGoodsIndex = 0;
			long lExtendID = -1;
			long lRetNum = 0;
			CGUID guid;
			DWORD dwPos = 0;
			DWORD dwMoney = 0;
			DWORD dwYuanBao = 0;
			DWORD dwOPNumber = pMsg->GetDWord();		/// ����������
			for(size_t i=0;i<dwOPNumber;i++)
			{
				CGoods *pGoods = NULL;
				BYTE bType = pMsg->GetByte();
				if (bType==OT_NEW_OBJECT)
				{
					pGoods = new CGoods();			
					BYTE ByteData[102400];
					long lBytePos = 0;
					pMsg->GetEx(ByteData,102400);
					pGoods->DecordFromByteArray(ByteData,lBytePos,true);
					lExtendID = pMsg->GetLong();
					dwPos = pMsg->GetDWord();
					lGoodsIndex = pGoods->GetIndex();
					CGoods ShowGoods(*pGoods);
					pMainPlayer->AddGoods(pGoods,dwPos,lExtendID);

					// �¼ӵ���Ʒ�Ƿ��ڱ�������ģ�ͬʱ����Ʒ������Դ�ڱ������������������Ϣ
					if(IS_PACKETCONTAINER(lExtendID))
					{
						// �Զ�װ������Ʒ
						pMainPlayer->AutoEquip(&ShowGoods,0,lExtendID,dwPos);
					}

					//���������Ʒ����������Ʒ����Ҫ����������ҵ�������Ʒ�б���
					if (pGoods!=NULL&&pGoods->GetGoodsBaseType()==GBT_CONSUMABLE&&(lExtendID==PEI_PACKET||
						(lExtendID>=PEI_PACK1&&lExtendID<=PEI_PACK5)))
					{
						pMainPlayer->AddShortCutGoods(lGoodsIndex);
						for(int i = 0;i<PACK_NUM;i++)
						{
							list<CPlayer::tagGoods>* GoodsList = pMainPlayer->GetGoodsList(i);
							list<CPlayer::tagGoods>::iterator GoodsIter = GoodsList->begin();
							for(;GoodsIter!=GoodsList->end();GoodsIter++)
							{
								if(GoodsIter->pGoods->GetEffectVal(GAP_COOLDOWN,2)==pGoods->GetEffectVal(GAP_COOLDOWN,2))
								{
									pMainPlayer->AddShortCutGoods(GoodsIter->pGoods->GetIndex());
								}
							}
						}
					}
				}
				else if (bType==OT_CHANGE_AMOUNT)
				{
					pMsg->GetGUID(guid);
					lRetNum = pMsg->GetDWord();
					lExtendID = pMsg->GetLong();
					dwPos = pMsg->GetDWord();
					pGoods = pMainPlayer->GetGoodsByGUID(guid);
					if (pGoods!=NULL)
					{
						lGoodsIndex = pGoods->GetIndex();
						pMainPlayer->ChangeObjectAmount(lExtendID,TYPE_GOODS,guid,lRetNum);
					}
				}
				else if (bType==OT_DELETE_OBJECT)
				{
					pMsg->GetGUID(guid);
					lRetNum = pMsg->GetLong();	
					lExtendID = pMsg->GetLong();
					dwPos = pMsg->GetDWord();
					pGoods = pMainPlayer->RemoveGoods(guid,guid,dwPos,lExtendID,lRetNum);
					lGoodsIndex = pGoods->GetIndex();
					SAFE_DELETE(pGoods);
					//�Ѵ���Ʒ�����Ŵ���ȴ�б���ɾ��
					if (pMainPlayer->GetGoodsAmount(lGoodsIndex)==0&&pMainPlayer->GetGoodsUseState(lGoodsIndex))
					{
						pMainPlayer->SetGoodsCoolDownStartTime((WORD)lGoodsIndex,0);
					}
				}
				if(i==0&&pGoods!=NULL)
				{
					GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->GetProperty()->dwIndex);
				}
				// ��Ʒ��ȡʱ����Ч
				//if (pGoods!=NULL&&pGoods->GetProperty()!=NULL)
				//{
				//	pItemPage->AddPickGoodsEffect(pGoods->GetIndex());
				//}

				//���¿�����ϵ���ʾ
				//CMainBarPageEx* pPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
				//if (pPage!=NULL)
				//{
				//	DWORD dwGoodsIndex = 0;
				//	for (int i = 0;i<24;i++)
				//	{
				//		dwGoodsIndex = pMainPlayer->GetHotKey(i);
				//		if (dwGoodsIndex!=0)
				//		{
				//			if (lGoodsIndex==dwGoodsIndex)
				//			{
				//				pPage->UpdataHotComp(i);
				//			}
				//		}
				//	}	
				//}
			}
			dwMoney = pMsg->GetDWord();
			dwYuanBao = pMsg->GetDWord();
			pMainPlayer->SetMoney(dwMoney);
			pMainPlayer->SetYuanBao(dwYuanBao);

			//if (pItemPage&&pItemPage->GetPackState())
			//{
			//	char strValue[32];
			//	sprintf(strValue,"%d",dwMoney);
			//	pItemPage->UpdataYBMoney("Gold",strValue);
			//	sprintf(strValue,"%d",dwYuanBao);
			//	pItemPage->UpdataYBMoney("Gem",strValue);

			playerShop.CloseShop();
			CMessage msg(MSG_C2S_PLAYERSHOP_LOOKGOODS);
			msg.Add(playerShop.GetSessionID());
			msg.Add(playerShop.GetMasterID());
			msg.Send();
		}
		break;

		// ������
	case MSG_S2C_PLAYERSHOP_TOSELLER:
		{
			long lExtendID = -1;
			DWORD dwPos = 0;
			int iAddOrSub = -1;
			long lRetNum = -1;
			DWORD dwMoney = 0;
			DWORD dwYuanBao = 0;
			DWORD dwWeiMian = 0;
			CGUID guid;
			DWORD lGoodsIndex = 0;

			//������Ʒ�Ĳ���������
			DWORD dwOPNumber = pMsg->GetDWord();
			CGoods* pGoods = NULL;
			for (size_t i = 0;i<dwOPNumber;i++)
			{
				BYTE bType = pMsg->GetByte();

				if (bType==OT_NEW_OBJECT)
				{
					pGoods = new CGoods();			
					BYTE ByteData[102400];
					long lBytePos = 0;
					pMsg->GetEx(ByteData,102400);
					pGoods->DecordFromByteArray(ByteData,lBytePos,true);
					lRetNum = pGoods->GetNum();
					lExtendID = pMsg->GetLong();
					dwPos = pMsg->GetDWord();
					pMainPlayer->AddGoods(pGoods,dwPos,lExtendID);
					lGoodsIndex = pGoods->GetIndex();
					//���������Ʒ����������Ʒ����Ҫ����������ҵ�������Ʒ�б���
					if (pGoods!=NULL&&pGoods->GetGoodsBaseType()==GBT_CONSUMABLE&&(lExtendID==PEI_PACKET||
						(lExtendID>=PEI_PACK1&&lExtendID<=PEI_PACK5)))
					{
						pMainPlayer->AddShortCutGoods(pGoods->GetIndex());
						for(int i = 0;i<PACK_NUM;i++)
						{
							list<CPlayer::tagGoods>* GoodsList = pMainPlayer->GetGoodsList(i);
							list<CPlayer::tagGoods>::iterator GoodsIter = GoodsList->begin();
							for(;GoodsIter!=GoodsList->end();GoodsIter++)
							{
								if(GoodsIter->pGoods->GetEffectVal(GAP_COOLDOWN,2)==pGoods->GetEffectVal(GAP_COOLDOWN,2))
								{
									pMainPlayer->AddShortCutGoods(GoodsIter->pGoods->GetIndex());
								}
							}
						}
					}
					// ��Ʒ��ȡʱ����Ч
					//if (pGoods->GetProperty()!=NULL)
					//{
					//	pItemPage->AddPickGoodsEffect(pGoods->GetIndex());
					//}
				}
				else if (bType==OT_DELETE_OBJECT)
				{
					pMsg->GetGUID(guid);
					lRetNum = pMsg->GetLong();	
					lExtendID = pMsg->GetLong();
					dwPos = pMsg->GetDWord();
					pGoods = pMainPlayer->GetGoodsByGUID(guid);
					if (pGoods!=NULL)
					{
						lGoodsIndex = pGoods->GetIndex();
						if(i==0)
						{
							GetGame()->GetAudioList()->PlayEquipmentSound(lGoodsIndex);
						}
					}
					pGoods = pMainPlayer->RemoveGoods(guid,guid,dwPos,lExtendID,lRetNum);
					SAFE_DELETE(pGoods);
				}
				else if (bType==OT_CHANGE_AMOUNT)
				{
					pMsg->GetGUID(guid);
					iAddOrSub = pMsg->GetByte();
					lRetNum = pMsg->GetLong();	
					lExtendID = pMsg->GetLong();
					dwPos = pMsg->GetDWord();
					pGoods = pMainPlayer->GetGoodsByGUID(guid);
					if (pGoods!=NULL)
					{
						lGoodsIndex = pGoods->GetIndex();
						if(i==0)
						{
							GetGame()->GetAudioList()->PlayEquipmentSound(lGoodsIndex);
						}
					}
					// ���Ǽ���һ������
					if (iAddOrSub==0)
					{
						pMainPlayer->ChangeObjectAmount(lExtendID,TYPE_GOODS,guid,pGoods->GetNum()-lRetNum);
					}else if (iAddOrSub==1)
					{
						pMainPlayer->ChangeObjectAmount(lExtendID,TYPE_GOODS,guid,pGoods->GetNum()+lRetNum);
					}
				}

				//���¿�����ϵ���ʾ
				//CMainBarPageEx* pPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
				//if (pPage!=NULL)
				//{
				//	DWORD dwGoodsIndex = 0;
				//	for (int i = 0;i<24;i++)
				//	{
				//		dwGoodsIndex = pMainPlayer->GetHotKey(i);
				//		if (dwGoodsIndex!=0)
				//		{
				//			if (lGoodsIndex==dwGoodsIndex)
				//			{
				//				pPage->UpdataHotComp(i);
				//			}
				//		}
				//	}	
				//}

				//�Ѵ���Ʒ�����Ŵ���ȴ�б���ɾ��
				if (pMainPlayer->GetGoodsAmount(lGoodsIndex)==0&&pMainPlayer->GetGoodsUseState(lGoodsIndex))
				{
					pMainPlayer->SetGoodsCoolDownStartTime((WORD)lGoodsIndex,0);
				}
				if (bType!=OT_NEW_OBJECT)
				{
					playerShop.UpdateMyShopList(guid,lGoodsIndex,lExtendID,dwPos,lRetNum,iAddOrSub);
				}				
			}
			dwMoney = pMsg->GetDWord();
			dwYuanBao = pMsg->GetDWord();
			pMainPlayer->SetMoney(dwMoney);
			pMainPlayer->SetYuanBao(dwYuanBao);

			//@todo �������»�����ʾ
			//if (pItemPage&&pItemPage->GetPackState())
			//{
			//	char strValue[32];
			//	sprintf(strValue,"%d",dwMoney);
			//	pItemPage->UpdataYBMoney("Gold",strValue);
			//	sprintf(strValue,"%d",dwYuanBao);
			//	pItemPage->UpdataYBMoney("Gem",strValue);
			//}

			dwMoney = pMsg->GetDWord();				//�˴ν������õĽ�Ǯ��
			dwYuanBao = pMsg->GetDWord();				//�˴ν������õ�Ԫ����
			dwWeiMian = pMsg->GetDWord();				//�˴ν������õ�λ�澫����

			//pShopPage->UpdateIncomeGolds(dwMoney,dwWeiMian);	//��ʱ�������ҵ�����
			FireUIEvent("PlayerShop", "UpdateMoneyInfo");
			if (playerShop.GetCurGoodsNum()==0)
			{
				FireUIEvent("PlayerShop", "Closed");
			}
		}
		break;

		//���̵�����ҳ���ˢ��
	case MSG_S2C_PLAYERSHOP_UPDATE:
		{				
			if (playerShop.GetCurShopState()==PlayerShop::SHOPPING_SHOP)
			{
				playerShop.ClearPlayerShopList();
			}
			bool bIsThatGoods = false;
			BYTE ByteData[102400];
			long lBytePos = 0;
			pMsg->GetEx(ByteData,102400);
			ulong goodsNum = _GetDwordFromByteArray(ByteData,lBytePos);
			for(size_t i=0; i<goodsNum; ++i)
			{
				CGoods* pGoods = new CGoods();
				pGoods->DecordFromByteArray(ByteData, lBytePos, true);

				ulong tradeType = _GetDwordFromByteArray(ByteData,lBytePos);
				ulong priceType = _GetDwordFromByteArray(ByteData,lBytePos);
				ulong groupNum = _GetDwordFromByteArray(ByteData,lBytePos);
				ulong oneGroupNum = _GetDwordFromByteArray(ByteData,lBytePos);
				ulong price = _GetDwordFromByteArray(ByteData,lBytePos);

				//if (pDescPage!=NULL&&pDescPage->IsShow()&&pDescPage->GetShowGoods()!=NULL
				//	&&pDescPage->GetShowGoods()->GetExID()==pGoods->GetExID())
				//{
				//	bIsThatGoods = true;
				//}

				if (playerShop.GetCurShopState()==PlayerShop::SHOPPING_SHOP)
				{
					playerShop.AddPlayerShopGoods(pGoods,groupNum,priceType,price,tradeType,oneGroupNum);
				}
				else if (playerShop.GetCurShopState()==PlayerShop::OPEN_SHOP)
				{
					playerShop.UpdateSellGoodsNum(pGoods->GetExID(), pGoods->GetIndex(), groupNum);
					SAFE_DELETE(pGoods);
				}
			}

			FireUIEvent("PlayerShop", "UpdateOperBtn");
			FireUIEvent("PlayerShop", "UpdateMoneyInfo");
			playerShop.UpdataPlayerShopGoodsList();

			//if (pDescPage->IsShow())
			//{
			//	pDescPage->SetIsShow(bIsThatGoods);
			//}
		}
		break;

	//	/// ���԰��е������̵����ơ������Ƽ���ϵͳ��Ϣ��
	//case MSG_S2C_PLAYERSHOP_MSGBOARDGET:
	//	{
	//		CGUID PlayerID,SessionID;
	//		pMsg->GetGUID(SessionID);
	//		pMsg->GetGUID(PlayerID);
	//		BYTE bInforType = pMsg->GetByte();
	//		CPlayer *pPlayer=(CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
	//		
	//		/// �����̵�����
	//		if (bInforType&0x1)		
	//		{
	//			char strShopName[64] = "";
	//			pMsg->GetStr(strShopName,64);
	//			if (pPlayer!=NULL)
	//			{
	//				pPlayer->SetPlayerShopName(strShopName);
	//			}
	//			
	//			//if (pShopPage->GetSessionID()==SessionID&&pMainPlayer->GetExID()!=PlayerID)
	//			{
	//			//	pShopPage->SetShopName(strShopName);
	//			}
	//		}
	//		/// ���õ����Ƽ�
	//		if (bInforType&0x2)
	//		{
	//			char strShopRecommend[256] = "";
	//			pMsg->GetStr(strShopRecommend,256);
	//			//if (pShopPage->GetSessionID()==SessionID&&pMainPlayer->GetExID()!=PlayerID)
	//			{
	//				//pShopPage->SetShopRecommend(strShopRecommend);
	//			}
	//		}
	//		/// ϵͳ��Ϣ
	//		if (bInforType&0x4)
	//		{
	//			BYTE dwNum = pMsg->GetByte();
	//			for (BYTE count = 0;count<dwNum;count++)
	//			{
	//				char strChatAllInfor[512] = "";
	//				pMsg->GetStr(strChatAllInfor,512);
	//				//pShopPage->AddChatItem(SessionID,strChatAllInfor);
	//			}
	//		}
	//	}
	//	break;
	//	/// ���԰��е���������
	//case MSG_S2C_PLAYERSHOP_MSGBOARDCHAT:
	//	{
	//		CGUID PlayerID,SessionID;
	//		long lPlayerPlug = 0;
	//		pMsg->GetGUID(SessionID);
	//		lPlayerPlug = pMsg->GetLong();
	//		pMsg->GetGUID(PlayerID);
	//		char strChatAllInfor[512] = "";
 //			pMsg->GetStr(strChatAllInfor,512);

	//		//pShopPage->AddChatItem(SessionID,strChatAllInfor,lPlayerPlug,PlayerID);
	//	}
	//	break;
	default:
		break;
	}
}