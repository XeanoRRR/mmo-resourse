#include "StdAfx.h"
#include "PlayerShop.h"

#include "../MainPlayerHand.h"
#include "../../Game.h"
#include "../Other/AudioList.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../../../UI/GamePage/ChatList/ChatListPage.h"


extern void SetPlayerShopGoodsItemInfo(PlayerShop::tagGoodsItem& tgGoodsItem, int index);

const int PlayerShop::ITEMCOUNTS = 3;

PlayerShop::PlayerShop(void)
{

}

PlayerShop::~PlayerShop(void)
{

}

void PlayerShop::SetShopPara(const CGUID& sessionID, long plugID, const CGUID& masterID)
{
	m_sessionID = sessionID;
	m_plugID = plugID;
	m_masterID = masterID;
}

void PlayerShop::SetCurShopState(int state)
{
	if(state < SET_SHOP || state >= SHOP_STATE)
		return;

	if (m_curShopState==SET_SHOP&&state==OPEN_SHOP)
	{
		m_tradeGolds = 0;
		m_dwTradeWeiMian = 0;
	}
	m_curShopState = state;
	if(m_curShopState == SHOPPING_SHOP)
	{
		m_sellerShopGoodsList.clear();
		m_curGoodsNum = 0;
	}
}

int PlayerShop::GetCurShopState(void)
{
	return m_curShopState;
}

void PlayerShop::SetIsOpenShop(bool b)
{
	m_isOpenShop = b;
}

bool PlayerShop::OnCloseShop(void)
{
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	if (pMainPlayer==NULL) return false;

	if(m_masterID==pMainPlayer->GetExID())
	{
		CMessage msg(MSG_C2S_PLAYERSHOP_CLOSE);
		msg.Add(GetSessionID());
		msg.Add((long)GetPlugID());
		msg.Send();
		if (GetbSetTrue())
		{
			SetbSetTrue(false);
		}
	}
	else
	{
		CMessage msg(MSG_C2S_PLAYERSHOP_QUIT);
		msg.Add(GetSessionID());
		msg.Add((long)GetPlugID());
		msg.Send();
	}
	CloseShop();
	GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-03.wav");
	return true;
}

// �ر��̵����
void PlayerShop::CloseShop()
{	
	GoodsItemIter GoodsIter = m_sellerShopGoodsList.begin();
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	if (pMainPlayer!=NULL)
	{
		for (;GoodsIter!=m_sellerShopGoodsList.end();GoodsIter++)
		{
			vector<tagTradeGoodsInfo>::iterator GoodsInforIter = GoodsIter->vecTradeGoodsInfo.begin();
			for (;GoodsInforIter!=GoodsIter->vecTradeGoodsInfo.end();GoodsInforIter++)
			{
				CGoods* pAcessGoods = pMainPlayer->GetGoodsByPosition(GoodsInforIter->contentId,GoodsInforIter->index);
				if (pAcessGoods!=NULL)
				{
					if (pAcessGoods->IsLocked())
					{
						pAcessGoods->UnLock();
					}
					pAcessGoods->SetHaveShadowState(false);
					// ���±���
					//if (pItemPage!=NULL)
					//{
					//	pItemPage->UpdateGoodsShowByCGoods(pAcessGoods);
					//}
				}
			}
		}
	}
	ClearPlayerShopList();
	ClearMyShopList();

	m_curShopState = -1;
	m_curGoodsNum = 0;
	m_tradeGolds = 0;
	m_dwTradeWeiMian = 0;
	m_bSetSure = false;
	m_plugID=0;
	m_isOpenShop = false;
}

// �����Ҵ洢�ĵ����ϼ���Ʒ�б�
void PlayerShop::ClearPlayerShopList()
{
	GoodsItemIter Iter = m_buyerShopGoodsList.begin();
	for (; Iter!=m_buyerShopGoodsList.end(); ++Iter)
	{
		if (Iter->pItemGoods!=NULL)
		{
			SAFEDELETE(Iter->pItemGoods);
		}
	}
	m_buyerShopGoodsList.clear();
}

// ��ȡ���µĸ����̵�������Ʒ�б�
void PlayerShop::UpdataPlayerShopGoodsList()
{
	if (m_curShopState<0||m_curShopState>=SHOP_STATE)
		return;

	if (m_curShopState == OPEN_SHOP)
	{
		for (int i = 0;i<(int)m_sellerShopGoodsList.size();i++)
		{
			SetPlayerShopGoodsItemInfo(m_sellerShopGoodsList[i],i);
		}
	}else if (m_curShopState == SHOPPING_SHOP)
	{
		for (int i = 0;i<(int)m_buyerShopGoodsList.size();i++)
		{
			SetPlayerShopGoodsItemInfo(m_buyerShopGoodsList[i],i);
		}
	}else if (m_curShopState == SET_SHOP&&!m_sellerShopGoodsList.empty())
	{
		for (int i = 0;i<(int)m_sellerShopGoodsList.size();i++)
		{
			SetPlayerShopGoodsItemInfo(m_sellerShopGoodsList[i],i);
		}
	}
}

// �����̵������Ʒʱ��������������Ϣ
//void PlayerShop::SendAddGoodsMes(int nPos,long lPlugID)
//{
//	CPlayer* pPlayer = GetGame()->GetMainPlayer();
//	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
//	if (!pPlayer || !pHand) return;
//
//	CPlayer::tagGoods* pstGoods = pHand->GetTGoodsOfMainPlayerHand();
//	DWORD dwAmount = 1;
//
//	if(pstGoods==NULL || pstGoods->pGoods==NULL) return;
//	dwAmount=pstGoods->pGoods->GetNum();
//
//	pHand->SendPutDownMes(TYPE_PLAYER, pPlayer->GetExID(), 
//		pstGoods->lType, pstGoods->lPos,
//		TYPE_SESSION, m_sessionID, lPlugID,
//		nPos, TYPE_GOODS, pstGoods->pGoods->GetExID(), dwAmount);
//}

/// ���Ϳ�����Ϣ
void PlayerShop::SendOpenShopMes(void)
{
	if (m_curShopState==SET_SHOP && !m_sellerShopGoodsList.empty())
	{
		CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
		if (!pMainPlayer) return;

		GoodsItemIter iter = m_sellerShopGoodsList.begin();
		ulong num = 0;
		size_t goodsNum = m_sellerShopGoodsList.size();

		CMessage msg(MSG_C2S_PLAYERSHOP_OPENFORBUSINESS);
		msg.Add(m_sessionID);
		msg.Add(m_plugID);
		msg.Add((long)goodsNum);
		for (; iter!=m_sellerShopGoodsList.end(); ++iter)
		{
			vector<tagTradeGoodsInfo> tempTradeGoods;
			// Ԥ��������
			if (iter->isReadyTrade)
			{
				vector<tagTradeGoodsInfo>::iterator Fiter = iter->vecTradeGoodsInfo.begin();
				for (; Fiter!=iter->vecTradeGoodsInfo.end(); ++Fiter)
				{
					CGoods* pReadyGoods = pMainPlayer->GetGoodsByPosition(Fiter->contentId,Fiter->index);
					if (pReadyGoods!=NULL)
					{
						pReadyGoods->SetHaveShadowState(false);
						if (pReadyGoods->IsLocked())
						{
							pReadyGoods->UnLock();
						}
					}
				}

				num = iter->readyTradeNum;		//������Ʒ��Ԥ��������
				for(int i = 0; num>0; i++)
				{
					if(num <= 0)	break;
					ulong curNum = iter->vecTradeGoodsInfo[i].curNum;
					if(num >= curNum)
						num -= curNum;
					else
					{
						curNum = num;
						num = 0;
					}
					tagTradeGoodsInfo TradeGoodInfor;
					TradeGoodInfor.goodsId = iter->vecTradeGoodsInfo[i].goodsId;
					TradeGoodInfor.contentId = iter->vecTradeGoodsInfo[i].contentId;
					TradeGoodInfor.index = iter->vecTradeGoodsInfo[i].index;
					TradeGoodInfor.curNum = curNum;
					tempTradeGoods.push_back(TradeGoodInfor);
				}		

				msg.Add((WORD)tempTradeGoods.size());
				msg.Add((LONG)iter->goodsId);
				msg.Add((DWORD)iter->tradeType);
				msg.Add((DWORD)iter->oneGroupNum);
				msg.Add((DWORD)iter->groupNum);
				msg.Add((WORD)iter->priceType);
				msg.Add((DWORD)iter->price);
				for (int j = 0;j<(int)tempTradeGoods.size();j++)
				{
					msg.Add((LONG)tempTradeGoods[j].contentId);
					msg.Add((LONG)tempTradeGoods[j].index);
					msg.Add((DWORD)tempTradeGoods[j].curNum);
					CGoods* pLockedGoods = pMainPlayer->GetGoodsByPosition(tempTradeGoods[j].contentId,tempTradeGoods[j].index);
					if (pLockedGoods!=NULL)
					{
						if (!pLockedGoods->IsLocked())
						{
							pLockedGoods->Lock();
						}
						pLockedGoods->SetHaveShadowState(true);
					}
				}
				// ���� @todo 
				//GetGame()->GetCGuiEx()->GetItemPageEx()->UpdataAllItems();
				//GetGame()->GetCGuiEx()->GetItemPageEx()->UpdataAllSubPack();
			}
			tempTradeGoods.clear();
		}
		msg.Send();
	}
}

/// ���͹ر��̵���Ϣ
void PlayerShop::SendCloseShopMes(void)
{
	CMessage msg(MSG_C2S_PLAYERSHOP_CLOSEDOWN);
	msg.Add(GetSessionID());
	msg.Add(GetPlugID());
	msg.Send();
}

/// ���͹�����Ϣ
void PlayerShop::SendBuyGoodsMes(void)
{
	if (m_curShopState == SHOPPING_SHOP&&!m_buyerShopGoodsList.empty())
	{
		GoodsItemIter iter = m_buyerShopGoodsList.begin();
		CMessage msg(MSG_C2S_PLAYERSHOP_BUYITEM);
		long lGoodsNum = GetBuyGoodsNum();
		msg.Add(m_sessionID);
		msg.Add(m_plugID);
		msg.Add(lGoodsNum);
		for(;iter!=m_buyerShopGoodsList.end();++iter)
		{
			// Ԥ��������
			if(iter->isReadyTrade)
			{
				msg.Add((long)(iter->goodsId));
				msg.Add((CGUID)iter->guid);
				msg.Add((ulong)iter->readyTradeNum);	// ������Ʒ׼�����׵�����
			}
		}
		msg.Send();
	}
}

// �����Ƴ���Ʒ��Ϣ
//void PlayerShop::SendRemoveGoodsMes(int index)
//{
//	if( m_curShopState!=SET_SHOP || m_sellerShopGoodsList.empty())
//		return;
//	if(index<0 || index>=m_sellerShopGoodsList.size())
//		return;
//	GoodsItemIter it = m_sellerShopGoodsList.begin();
//	for(;it!=m_sellerShopGoodsList.end();++it)
//	{
//		// ����Ӧ��Ŀ���б���ɾ��;
//		DWORD dwAmount = 1;
//		LONG lExtendID = -1;
//		long lpos = -1;
//		CGUID guid = m_sellerShopGoodsList[index].guid;
//		if (CGoodsList::GetEffectVal(m_sellerShopGoodsList[index].goodsId,CGoods::GAP_MAXOVERLAP,1)==0)
//		{
//			lExtendID = m_sellerShopGoodsList[index].vecTradeGoodsInfo[0].contentId;
//			lpos = m_sellerShopGoodsList[index].vecTradeGoodsInfo[0].index;
//		}
//
//		CMessage msg(MSG_C2S_PLAYERSHOP_REMOVE);
//		msg.Add((long)m_sellerShopGoodsList[index].goodsId);
//		msg.Add((LONG)TYPE_SESSION);
//		msg.Add(m_sessionID);
//		msg.Add((LONG)m_plugID<<8);
//		msg.Add((LONG)0);
//
//		msg.Add((LONG)TYPE_PLAYER);
//		msg.Add(GetGame()->GetMainPlayer()->GetExID());
//		msg.Add((LONG)lExtendID);
//		msg.Add((LONG)lpos);
//		msg.Add((LONG)TYPE_GOODS);
//		msg.Add(guid);
//		msg.Add((LONG)dwAmount);
//		msg.Send();
//	
//		return;
//	}
//}

// ��ȡҪ�����Ʒ�������
long PlayerShop::GetBuyGoodsNum(void)
{
	long lNum = 0;
	if (m_curShopState == SHOPPING_SHOP && !m_buyerShopGoodsList.empty())
	{
		for (int i=0; i<(int)m_buyerShopGoodsList.size(); i++)
		{
			if (m_buyerShopGoodsList[i].isReadyTrade)
			{
				lNum++;
			}
		}
	}
	return lNum;
}

// ���Ҫ��������Ʒ
void PlayerShop::AddMyShopGoods(CGoods *pGoods,int iPos/*���̵��е�λ��*/,eTradeType eDType,
								DWORD dwGoodsNumPerTeam,DWORD dwTeamNum ,DWORD dwPrice,int iPriceType )
{
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	if (!pGoods || !pMainPlayer || m_masterID!=pMainPlayer->GetExID())
		return;

	// ��ʾ����Ʒ�Ƿ�Ϊ���ص���
	long bCanLaped = false;
	// �鿴����Ʒ�Ƿ���ص�
	if(pGoods->HasEffect(GAP_MAXOVERLAP)) 
	{
		bCanLaped = true;
	}
	tagGoodsItem item;
	// ���ص����������Ʒ�Ѵ��ڣ��򷵻أ�����ͳ�Ƹ�����Ʒ��������ϵ��������������̵���Ʒ�ṹ���뵽�̵��б���ȥ
	if(bCanLaped)
	{
		// �����̵��б�
		for(int i=0; i<(int)m_sellerShopGoodsList.size(); ++i)
		{
			// �Ѵ��ڣ�����
			if(m_sellerShopGoodsList[i].goodsId == pGoods->GetIndex())
				return;
		}

		// ���������Ʒ�б�
		int iNum = 0;
		list<CPlayer::tagGoods>::iterator its;
		list<CPlayer::tagGoods>::iterator itend;
		for(int i=0; i<PACK_NUM; ++i)
		{
			list<CPlayer::tagGoods> *pGoodsList = pMainPlayer->GetGoodsList(i);
			if(!pGoodsList || pGoodsList->empty())
				continue;
			// ��ñ�����Ʒ
			its = pGoodsList->begin();
			itend = pGoodsList->end();
			// �����ñ�������Ʒ�б�
			for(; its != itend; its++)
			{
				// �ҵ�ͬ����Ʒ����Ʒ���ۼ�
				if(pGoods->GetIndex() == its->pGoods->GetIndex()&&!its->pGoods->IsLocked())
				{
					its->pGoods->SetHaveShadowState(true);
					// ����@todo 
					//GetGame()->GetCGuiEx()->GetItemPageEx()->UpdateGoodsShow(its->lType,its->lPos);
					iNum += its->lNum;
					// ��Ʒ�Ľ�����Ϣ
					tagTradeGoodsInfo goodsInfo;
					if (i==0)
					{
						goodsInfo.contentId = PEI_PACKET;
					}else
					{
						goodsInfo.contentId = PEI_SUBPACK+i;
					}
					goodsInfo.curNum = its->lNum;
					goodsInfo.goodsId = pGoods->GetIndex();
					goodsInfo.index = its->lPos;
					item.vecTradeGoodsInfo.push_back(goodsInfo);
				}
			}
		}
		item.totalNum = iNum;
	}
	else
	{
		item.totalNum = 1;
		long lExtendID = 0;
		int	 iPos = 0;
		GetGame()->GetMainPlayer()->GetGoodsByGUID(pGoods->GetExID(),&iPos,&lExtendID);
		// ��Ʒ�Ľ�����Ϣ
		tagTradeGoodsInfo goodsInfo;
		goodsInfo.contentId = lExtendID;
		goodsInfo.curNum = item.totalNum;
		goodsInfo.goodsId = pGoods->GetIndex();
		goodsInfo.index = iPos;
		item.vecTradeGoodsInfo.push_back(goodsInfo);
		pGoods->SetHaveShadowState(true);
		// ����@todo 
		//GetGame()->GetCGuiEx()->GetItemPageEx()->UpdateGoodsShow(lExtendID,iPos);
	}
	// ����һ����Ʒ�ṹ
	item.goodsIconId = pGoods->GetProperty()->dwIconId;
	item.goodsId = pGoods->GetIndex();
	item.strGoodsName = pGoods->GetBaseProperty()->strName;
	item.guid = pGoods->GetExID();
	item.tradeType = eDType;
	item.oneGroupNum = dwGoodsNumPerTeam;
	item.groupNum = dwTeamNum;
	item.priceType = iPriceType;
	item.price = dwPrice;
	item.pItemGoods = pGoods;
	// �������ӵ���Ʒ�ɻ�Ļ������ӵ���ӯ������ȥ
	if (item.priceType==PT_Money)
	{
		m_tradeGolds += item.groupNum*item.price;
	}else if (item.priceType==PT_WeiMian)
	{
		m_dwTradeWeiMian += item.groupNum*item.price;
	}
	if (eDType==TT_SINGLE)
	{
		item.readyTradeNum = dwTeamNum;
	}else if (eDType==TT_GROUP)
	{
		item.readyTradeNum = dwGoodsNumPerTeam*dwTeamNum;
	}
	item.isReadyTrade = true;
	if (!bCanLaped)
	{
		item.pItemGoods = pGoods;
	}

	// ��ʱ��������ʾ����Ʒ�����ڸ����̵��λ��
	int index = 0;
	// ��ǰ�̵��е���Ʒ������С����������õ������Ʒ�����Զ����¼�����Ʒ���õ������̵�����һ��
	if(m_curGoodsNum < ITEMCOUNTS)
	{
		index = m_curGoodsNum;
		m_sellerShopGoodsList.push_back(item);
	}
	// �����ʱ�����̵����Ʒ���ﵽ���ޣ��û������̵������Ʒʱ�������µ���Ʒ�滻ԭ����Ŀλ�õ���Ʒ��
	else
	{
		index = iPos;
		if (index>=ITEMCOUNTS)
		{
			index = ITEMCOUNTS-1;
		}
		ReMoveShopGoods(m_sellerShopGoodsList[index].goodsId,m_sellerShopGoodsList[index].guid);
		// �滻
		m_sellerShopGoodsList.push_back(item);
		index = m_curGoodsNum;
        GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Shop_6")  //��û�и����λ�ðڷŻ�������
            , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
	}

	m_curGoodsNum= (int)(m_sellerShopGoodsList.size());

	// Ϊ�����̵��б����������Ʒ��Ϣ
	SetPlayerShopGoodsItemInfo(m_sellerShopGoodsList[index],index);

	FireUIEvent("PlayerShop", "UpdateOperBtn");
}

/// �������̵���ָ�����е���Ʒ�Ƴ�
bool PlayerShop::ReMoveShopGoods(long lGoodsID, CGUID guid)							
{
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	if( pMainPlayer==NULL||m_curShopState!=SET_SHOP || m_sellerShopGoodsList.empty())
		return false;

	// ����Ӧ��Ŀ���б���ɾ��
	GoodsItemIter iter = m_sellerShopGoodsList.begin();
	for (; iter!=m_sellerShopGoodsList.end(); ++iter)
	{
		if (iter->guid==guid)
		{
			vector<tagTradeGoodsInfo>::iterator GoodsInforIter = iter->vecTradeGoodsInfo.begin();
			for (;GoodsInforIter!=iter->vecTradeGoodsInfo.end();GoodsInforIter++)
			{
				CGoods* pAcessGoods = pMainPlayer->GetGoodsByPosition(GoodsInforIter->contentId,GoodsInforIter->index);
				if (pAcessGoods!=NULL)
				{
					if (pAcessGoods->IsLocked())
					{
						pAcessGoods->UnLock();
					}
					pAcessGoods->SetHaveShadowState(false);
					// ���� @todo
					//pItemPage->UpdateGoodsShowByCGoods(pAcessGoods);
				}
			}
			if (iter->priceType==PT_Money)
			{
				m_tradeGolds -= iter->groupNum*iter->price;
			}
			else if (iter->priceType==PT_WeiMian)
			{
				m_dwTradeWeiMian -= iter->groupNum*iter->price;
			}

			m_sellerShopGoodsList.erase(iter);
			--m_curGoodsNum;

			FireUIEvent("PlayerShop","UpdateOperBtn");
			break;
		}
	}

	UpdataPlayerShopGoodsList();

	return true;
}

// �����̵��м���������ϼ���Ʒ
void PlayerShop::AddPlayerShopGoods(CGoods* pGoods, ulong groupNum, ulong priceType,
									ulong price, ulong tradeType, ulong oneGroupNum)
{
	if (pGoods)
	{
		tagGoodsItem item;
		item.goodsId = pGoods->GetIndex();
		item.pItemGoods = pGoods;
		item.goodsIconId = pGoods->GetProperty()->dwIconId;
		item.price = price;
		item.groupNum = groupNum;
		item.oneGroupNum = oneGroupNum;
		item.tradeType= (eTradeType)tradeType;
		item.strGoodsName = pGoods->GetName();
		item.priceType = (int)priceType;
		item.guid = pGoods->GetExID();
		m_buyerShopGoodsList.push_back(item);
	}
}

// ������谴ťʱ��ȡ��Ԥ��������
void PlayerShop::CancelAllTrade()
{
	if(m_curShopState == OPEN_SHOP)
		return;

	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	//CItemPageEx* pItemPage = GetGame()->GetCGuiEx()->GetItemPageEx();

	// �ָ�ҳ�湦�ܰ�ť��ȷ������ʼ��̯/����
	FireUIEvent("PlayerShop", "UpdateOperBtn");

	vector<tagGoodsItem> *pVecGoodsItem = NULL;
	m_tradeGolds = 0;
	m_dwTradeWeiMian = 0;

	/// �жϵ�ǰ�Ƿ��������ڰ�̯״̬ת������״̬��
	bool bIsStopSell = false;

	// �����̵�״̬
	if(m_curShopState == SET_SHOP)
	{
		pVecGoodsItem = &m_sellerShopGoodsList;
		bIsStopSell = true;
	}
	// ����״̬
	else if(m_curShopState == SHOPPING_SHOP)
	{
		pVecGoodsItem = &m_buyerShopGoodsList;
	}
	if(pVecGoodsItem)
	{
		if(pVecGoodsItem->empty())	return;	

		vector<tagTradeGoodsInfo> vecGoodsInfor;
		map<DWORD,tagGoodsItem>	  mapGoodsItem;

		GoodsItemIter it=pVecGoodsItem->begin();
		for(;it!=pVecGoodsItem->end();it++)
		{
			if (m_curShopState == SET_SHOP)
			{		
				CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
				if (pMainPlayer!=NULL)
				{
					vector<tagTradeGoodsInfo>::iterator GoodsIterInfor = it->vecTradeGoodsInfo.begin();
					for (;GoodsIterInfor!=it->vecTradeGoodsInfo.end();GoodsIterInfor++)
					{
						CGoods* pGoods = pMainPlayer->GetGoodsByPosition(GoodsIterInfor->contentId,GoodsIterInfor->index);
						if (pGoods!=NULL&&pGoods->IsLocked())
						{
							pGoods->UnLock();
						}
						if (pHand!=NULL&&pHand->GetPGoodsOfMainPlayerHand()==pGoods)
						{
							pHand->ClearMainPlayerHand();
						}

						if(bIsStopSell)
						{
							tagTradeGoodsInfo temp;
							temp.contentId = GoodsIterInfor->contentId;
							temp.index = GoodsIterInfor->index;
							temp.goodsId = GoodsIterInfor->goodsId;
							temp.curNum = GoodsIterInfor->curNum;

							vecGoodsInfor.push_back(temp);
						}

					}
					//if (pItemPage!=NULL&&!bIsStopSell)
					//{
					//	pItemPage->UpdataAllItems();
					//	pItemPage->UpdataAllSubPack();
					//}
				}

				if(bIsStopSell)
				{
					tagGoodsItem tempGoodsItem;
					tempGoodsItem.goodsId = it->goodsId;
					tempGoodsItem.tradeType = it->tradeType;
					tempGoodsItem.readyTradeNum = it->readyTradeNum;
					tempGoodsItem.oneGroupNum = it->oneGroupNum;
					tempGoodsItem.groupNum = it->groupNum;
					tempGoodsItem.price = it->price;
					tempGoodsItem.priceType = it->priceType;
					mapGoodsItem.insert(pair<ulong,tagGoodsItem>(tempGoodsItem.goodsId,tempGoodsItem));
				}

			}
			else if (m_curShopState == SHOPPING_SHOP)
			{
				(*it).isReadyTrade = false;
				(*it).readyTradeNum = 0;
			}
		}

		if (bIsStopSell&&m_curShopState == SET_SHOP)
		{
			if (pMainPlayer!=NULL&&!vecGoodsInfor.empty())
			{
				m_sellerShopGoodsList.clear();
				m_curGoodsNum = 0;
				vector<tagTradeGoodsInfo>::iterator GoodsInforIter = vecGoodsInfor.begin();
				for (int i = 0;GoodsInforIter!=vecGoodsInfor.end();i++,GoodsInforIter++)
				{
					CGoods* pNeedGoods = pMainPlayer->GetGoodsByPosition(GoodsInforIter->contentId,GoodsInforIter->index);
					if (pNeedGoods!=NULL)
					{
						eTradeType eDType = TT_SINGLE;
						DWORD dwGoodsNumPerTeam = 0;
						DWORD dwTeamNum = 1;
						DWORD dwPrice = 0;
						int	  iPriceType = PT_Money;
						map<DWORD,tagGoodsItem>::iterator mapIter = mapGoodsItem.find(GoodsInforIter->goodsId);
						if(mapIter!=mapGoodsItem.end())
						{
							eDType = mapIter->second.tradeType;
							dwGoodsNumPerTeam = mapIter->second.oneGroupNum;
							dwTeamNum = mapIter->second.groupNum;
							dwPrice = mapIter->second.price;
							iPriceType = mapIter->second.priceType;
						}
						AddMyShopGoods(pNeedGoods,i,eDType,dwGoodsNumPerTeam,dwTeamNum,dwPrice,iPriceType);
						FireUIEvent("PlayerShop", "UpdateMoneyInfo");
					}
				}
				// @todo ����
				//if (pItemPage!=NULL)
				//{
				//	pItemPage->UpdataAllItems();
				//	pItemPage->UpdataAllSubPack();
				//}
			}
		}
	}
	UpdataPlayerShopGoodsList();
}

//���µ�������Ʒ�б�
void PlayerShop::UpdateMyShopList(const CGUID& guid,DWORD dwIndex,long lExtendID,DWORD dwPos,long lNum,int iAddOrSub)
{
	CPlayer* pPlayer = GetGame()->GetMainPlayer();
	if (pPlayer)
	{
		GoodsItemIter iter = m_sellerShopGoodsList.begin();
		for (; iter!=m_sellerShopGoodsList.end(); ++iter)
		{
			if (iter->guid == guid && CGoodsList::GetEffectVal(dwIndex,GAP_MAXOVERLAP,1)==0)
			{
				m_sellerShopGoodsList.erase(iter);
				m_curGoodsNum--;
				return;
			}else if (CGoodsList::GetEffectVal(dwIndex,GAP_MAXOVERLAP,1)!=0)
			{
				vector<tagTradeGoodsInfo>::iterator it = iter->vecTradeGoodsInfo.begin();
				for (;it!=iter->vecTradeGoodsInfo.end();it++)
				{
					if (it->contentId == lExtendID&&it->index == dwPos)
					{
						// ����˸��ӵ���Ʒ�����봫��ı仯������ͬ�����ʾ�˸��ӵ���Ʒȫ����������
						// ������Ʒ������Ҫ��ȥ����������
						if (it->curNum==lNum&&iAddOrSub==-1)
						{		
							iter->vecTradeGoodsInfo.erase(it);				
							iter->totalNum -= lNum;
							iter->readyTradeNum -= lNum; 
						}
						// ����������������ڴ˸��ӵ�����ʱ����ʾ��������Ǵ˸�����Ʒʣ�µ�����
						// ������Ʒ������Ҫ��ȥ���۵�����Ʒ��
						else if (iAddOrSub==0)
						{
							iter->totalNum -= lNum;
							iter->readyTradeNum -= lNum; 
							it->curNum -= lNum;
						}
						if (iter->readyTradeNum==0)
						{
							it = iter->vecTradeGoodsInfo.begin();
							for (;it!=iter->vecTradeGoodsInfo.end();it++)
							{
								CGoods* pGoods = pPlayer->GetGoodsByPosition(it->contentId,it->index);
								if (pGoods!=NULL)
								{
									// �����Ʒ������
									if (pGoods->IsLocked())
									{
										pGoods->UnLock();
										// @todo ˢ�±����������ʾ
										/*if (pItemPage!=NULL)
										{
											pItemPage->UpdateGoodsShowByCGoods(pGoods);
										}*/
									}
								}
							}
							m_sellerShopGoodsList.erase(iter);
							m_curGoodsNum--;
							// @todoˢ�±����������ʾ
							//CItemPageEx* pItemPage = GetGame()->GetCGuiEx()->GetItemPageEx();
							//if (pItemPage!=NULL)
							//{
							//	pItemPage->UpdataAllItems();
							//	pItemPage->UpdataAllSubPack();
							//}
						}
						return;
					}
				}
			}			
		}
	}
}

// ���µ����ϼ���Ʒ����
void PlayerShop::UpdateSellGoodsNum(const CGUID& guid, ulong index,ulong groupNum)
{
	GoodsItemIter iter = m_sellerShopGoodsList.begin();
	for (; iter!=m_sellerShopGoodsList.end(); ++iter)
	{
		if (iter->guid == guid && CGoodsList::GetEffectVal(index,GAP_MAXOVERLAP,1)==0 ||
			iter->goodsId==index && CGoodsList::GetEffectVal(index,GAP_MAXOVERLAP,1)!=0)
		{
			iter->groupNum = groupNum;
		}		
	}
}

// ��������̵��ʱ����������Ʒ���һ�����Ӧ����
bool PlayerShop::OnInputGoods(CPlayer::tagGoods* pstGoods)
{
	if (pstGoods!=NULL&&pstGoods->pGoods!=NULL&&m_curShopState==SET_SHOP&&!m_bSetSure)
	{
		//�����ѫ�£����ܷ���
		if(pstGoods->pGoods->GetProperty()->dwType == GBT_MEDAL)
			return true;

		long lFreezeVal = pstGoods->pGoods->GetEffectVal(GAP_FREEZE,1);
		long lBindVal = pstGoods->pGoods->GetEffectVal(GAP_BIND,1);
		//�������Ʒ���ܷ�������̵���н���
		if (lFreezeVal!=eFT_UnFrost&&lFreezeVal!=eFT_CantFrost)
		{
            GetInst(MsgEventManager).PushEvent(Msg_Ok, AppFrame::GetText("Goods_156"));
			pstGoods->pGoods->SetHaveShadowState(false);
			//@todo ����
			/*if (pItemPage!=NULL)
			{
				pItemPage->UpdateGoodsShowByCGoods(pstGoods->pGoods);
			}*/
			return true;
		}
		//�󶨵���Ʒ���ܷ�������̵���н���
		if (lBindVal==1||lBindVal==3)
		{
			GetInst(MsgEventManager).PushEvent(Msg_Ok, AppFrame::GetText("Goods_157"));
			pstGoods->pGoods->SetHaveShadowState(false);
			//@todo ����
			/*if (pItemPage!=NULL)
			{
				pItemPage->UpdateGoodsShowByCGoods(pstGoods->pGoods);
			}*/
			return true;
		}
		// ���ɽ��׵Ĳ��ܷ�������̵��з���
		if (pstGoods->pGoods->GetEffectVal(GAP_PARTICULAR_ATTRIBUTE,1)&32)
		{
			GetChatListPage().AddChatWords(AppFrame::GetText("Shop_11")
                , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
			return true;
		}

		// ����Ʒ�����嵥����
		if (m_curGoodsNum<ITEMCOUNTS)
		{
			OnOrderPageOpen(pstGoods->pGoods,m_curGoodsNum);
			return true;
		}
	}
	return false;
}

///��⵱ǰ�����嵥�е���ƷҪ���õ������Ƿ�������ȫ�ˣ����û�������Ҫ��ʾ
bool PlayerShop::IsOrderSetupOK(void)
{
	stCacheIter CacheIter = m_mapCacheGoods.find(m_orderGoods->GetIndex());
	if (CacheIter!=m_mapCacheGoods.end())
	{
		if (CacheIter->second.treadeType==TT_SINGLE)
		{
			if (CacheIter->second.groupNum==0)
			{
                GetChatListPage().AddChatWords(AppFrame::GetText("Shop_22")
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}
			else if (CacheIter->second.price<=0)
			{
				GetChatListPage().AddChatWords(AppFrame::GetText("Shop_15")
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}
		}
		else if (CacheIter->second.treadeType==TT_GROUP)
		{
			if (CacheIter->second.groupNum==0)
			{
				GetChatListPage().AddChatWords(AppFrame::GetText("Shop_22")
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}
			else if (CacheIter->second.oneGroupNum==0)
			{
				GetChatListPage().AddChatWords(AppFrame::GetText("Shop_21"), 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}
			else if (CacheIter->second.price<=0)
			{
				GetChatListPage().AddChatWords(AppFrame::GetText("Shop_15"), 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}
		}
		return true;
	}
	return false;
}

bool PlayerShop::OnShopOrderOK(void)
{
	if (!m_orderGoods) return false;

	stCacheIter CacheIter = m_mapCacheGoods.find(m_orderGoods->GetIndex());
	if (CacheIter==m_mapCacheGoods.end()) return false;

	if (IsOrderSetupOK())
	{
		AddMyShopGoods(m_orderGoods, CacheIter->second.shopPos, CacheIter->second.treadeType,
			CacheIter->second.oneGroupNum, CacheIter->second.groupNum,
			CacheIter->second.price,CacheIter->second.priceType);

		FireUIEvent("PlayerShop", "UpdateMoneyInfo");
		GetGame()->GetAudioList()->PlayEquipmentSound(m_orderGoods->GetIndex());

		FireUIEvent("PlayerShopOrder", "Closed");
		return true;
	}
	return false;
}

bool PlayerShop::OnOrderPageOpen(CGoods* pGoods,int shopPos)
{
	// ������Ʒ�Ȳ����ϼ���Ʒ�б����Ƿ����
	if (pGoods->HasEffect(GAP_MAXOVERLAP))
	{
		// �����̵��б�
		for(int i=0; i<(int)m_sellerShopGoodsList.size(); ++i)
		{
			// �Ѵ��ڣ�����
			if(m_sellerShopGoodsList[i].goodsId == pGoods->GetIndex())
				return false;
		}
	}

	/// ������Ʒָ���ݱ�����m_tpGoods
	m_orderGoods = pGoods;

	stCacheIter CacheIter = m_mapCacheGoods.find(pGoods->GetIndex());
	if (CacheIter!=m_mapCacheGoods.end())
	{
		CacheIter->second.shopPos = shopPos;
	}else
	{
		stCacheGoodsInfo _stCache;
		_stCache.shopPos = shopPos;
		m_mapCacheGoods.insert(pair<ulong,stCacheGoodsInfo>(pGoods->GetIndex(),_stCache));
	}

	FireUIEvent("PlayerShopOrder", "Opened");

	return true;
}

/// ������ƷID�ڻ����б��в�����Ʒ
PlayerShop::stCacheGoodsInfo* PlayerShop::GetCacheGoodsInfo(ulong goodsID)
{
	if (m_mapCacheGoods.empty()) return NULL;
	
	stCacheIter CacheIter = m_mapCacheGoods.find(goodsID);
	if (CacheIter!=m_mapCacheGoods.end())
		return  &(CacheIter->second);
	return NULL;
}

// �����ϼ���Ʒ��Ϣ
PlayerShop::tagGoodsItem* PlayerShop::FindtagGoods(CGoods *pGoods)
{
	if(!pGoods)	return NULL;
	if(m_curShopState < SET_SHOP || m_curShopState >= SHOP_STATE)
		return NULL;

	// �����̵�״̬
	if(m_curShopState == SET_SHOP || m_curShopState == OPEN_SHOP)
	{
		GoodsItemIter it = m_sellerShopGoodsList.begin();
		for(;it!=m_sellerShopGoodsList.end();it++)
		{
			if((*it).pItemGoods == pGoods)
				return &(*it);
		}
	}
	// �������Ʒ�б��в���
	else if(!m_buyerShopGoodsList.empty())
	{
		GoodsItemIter it=m_buyerShopGoodsList.begin();
		for(;it!=m_buyerShopGoodsList.end();it++)
		{
			if((*it).pItemGoods == pGoods)
				return &(*it);
		}
	}
	return NULL;
}

// �жϽ������Ƿ���������<�˽ӿ�ֻ������>
bool PlayerShop::IsTradeItemReady(void)
{
	bool isReady = false;

	if(m_curShopState==SHOPPING_SHOP)
	{
		
		if(m_buyerShopGoodsList.empty())
			return false;
		for(int i=0; i<(int)m_buyerShopGoodsList.size(); ++i)
		{
			if(m_buyerShopGoodsList[i].readyTradeNum > 0)
				isReady = m_buyerShopGoodsList[i].isReadyTrade = true;
			else
				m_buyerShopGoodsList[i].isReadyTrade = false;
		}
	}
	return isReady;
}

// ���ݵ�ǰ׼��������Ʒ����㽻�׶�
void PlayerShop::CalcTradeGolds(void)
{
	m_tradeGolds =  0;
	m_dwTradeWeiMian = 0;

	// ��������̵�״̬
	if(!m_buyerShopGoodsList.empty())
	{
		GoodsItemIter it=m_buyerShopGoodsList.begin();
		for(;it!=m_buyerShopGoodsList.end();it++)
		{
			// ��Ԥ��������
			if((*it).isReadyTrade)
			{
				// ����ĿǮ�ҷ��Ϊ���
				if((*it).priceType == PT_Money)
				{
					m_tradeGolds += (*it).price * ((*it).readyTradeNum);
					if (m_tradeGolds>=2000000000)
					{
						m_tradeGolds=2000000000;
					}
				}
				else if(it->priceType == PT_WeiMian)
				{
					m_dwTradeWeiMian += (*it).price * ((*it).readyTradeNum);
					if (m_dwTradeWeiMian>=2000000000)
					{
						m_dwTradeWeiMian=2000000000;
					}
				}
			}
		}
	}
}