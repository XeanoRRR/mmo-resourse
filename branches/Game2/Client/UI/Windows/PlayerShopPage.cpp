#include "StdAfx.h"
#include "../UIDef.h"

#include "../GamePage/Package/PackagePage.h"
#include "../GamePage/ChatList/ChatListPage.h"

#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/MainPlayerHand.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../GameClient/ClientApp/TradeSystem/PlayerShop.h"
#include "../../../Public/Common/DepotDef.h"

CEGUI::Window* InitPlayerShopPage(void);							///< ��ʼ�������̵����
void SubscribePlayerShopPageEvent(CEGUI::Window* mainPage);			///< �����̵�����¼�ע��
bool OnPlayerShopPageOpen(const CEGUI::EventArgs& e);				///< �򿪸����̵�
bool OnPlayerShopPageClose(const CEGUI::EventArgs& e);				///< �رո����̵�
bool OnPlayerShopBlackboard(const CEGUI::EventArgs& e);				///< ��/�ر����԰�
bool OnHideShowPlayerShop(const CEGUI::EventArgs& e);				///< ����/��ʾ�����̵�
bool OnBeginStopPlayerShop(const CEGUI::EventArgs& e);				///< ��ʼ����/ֹͣ����

bool OnPlayerShopOperBtnUpdate(const CEGUI::EventArgs& e);			///< ������Ʒ���������ť����
bool OnPlayerShopClearGoodsInfo(const CEGUI::EventArgs& e);			///< ������Ʒ��Ʒ������Ϣ���
bool OnPlayerShopUpdateMoneyInfo(const CEGUI::EventArgs& e);		///< �����̵���»�����Ϣ
bool OnPlayerShopOrderGoods(const CEGUI::EventArgs& e);				///< �����̵���Ʒ�ϼ�
bool OnPlayerShopResetGoods(const CEGUI::EventArgs& e);				///< ˫�������ϼ���Ʒ��Ϣ
bool OnPlayerShopShowGoodsInfo(const CEGUI::EventArgs& e);			///< ��������鿴��Ʒ��Ϣ
bool OnPlayerShopAddBuyNum(const CEGUI::EventArgs& e);				///< ���ӹ���������ť�¼�
bool OnPlayerShopSubBuyNum(const CEGUI::EventArgs& e);				///< ���ٹ���������ť�¼�
bool OnPlayerShopBuyNumChange(const CEGUI::EventArgs& e);			///< �����������


void SetPlayerShopGoodsItemInfo(PlayerShop::tagGoodsItem& tgGoodsItem, int index); ///< ��ӻ����б�

CEGUI::Window* InitPlayerShopPage(void)
{
#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("PlayerShop.log");
#endif

	CEGUI::Window* wnd = LoadUI("PlayerShop");

#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("CEGUI.log",true);
#endif

	SubscribePlayerShopPageEvent(wnd);

	wnd->setVisible(false);
	return wnd;
}

void SubscribePlayerShopPageEvent(CEGUI::Window* mainPage)
{
	if (!mainPage)
		return;

	CEGUI::Window* wnd;
	char tempText[256];

	// ע���/�ر����԰尴ť�¼�
	wnd = WPushButton(mainPage->getChildRecursive("PlayerShop/backgrond/Blackboard"));
	if (wnd)
	{
		wnd->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopBlackboard));
	}

	// ע������/��ʾ�̵갴ť�¼�
	wnd = WPushButton(mainPage->getChildRecursive("PlayerShop/backgrond/HideShowBtn"));
	if (wnd)
	{
		wnd->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnHideShowPlayerShop));
	}

	// ��ʼ����/ֹͣ������ť�¼�
	wnd = WPushButton(mainPage->getChildRecursive("PlayerShop/backgrond/BeginStopBtn"));
	if (wnd)
	{
		wnd->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnBeginStopPlayerShop));
	}

	// �ر�/�뿪��ť�¼�
	wnd = WPushButton(mainPage->getChildRecursive("PlayerShop/backgrond/CloseBtn"));
	if (wnd)
	{
		wnd->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopPageClose));
	}

	// �������&�رղ���
	mainPage->subscribeEvent("Opened", CEGUI::Event::Subscriber(OnPlayerShopPageOpen));
	mainPage->subscribeEvent("Closed", CEGUI::Event::Subscriber(OnPlayerShopPageClose));

	// ע���̵������ť�����¼�
	mainPage->subscribeEvent("UpdateOperBtn", CEGUI::Event::Subscriber(OnPlayerShopOperBtnUpdate));
	// ע����½��׻�����ʾ�¼�
	mainPage->subscribeEvent("UpdateMoneyInfo", CEGUI::Event::Subscriber(OnPlayerShopUpdateMoneyInfo));
	// ע�������Ʒ��Ʒ������Ϣ����¼�
	mainPage->subscribeEvent("ClearGoodsInfo", CEGUI::Event::Subscriber(OnPlayerShopClearGoodsInfo));
	
	// ע�������������̵���棬����������Ʒ�嵥�����¼�
	CEGUI::Window* backgrondWnd = mainPage->getChild("PlayerShop/backgrond");
	backgrondWnd->subscribeEvent(CEGUI::Window::EventDragDropItemDropped,
		CEGUI::Event::Subscriber(OnPlayerShopOrderGoods));

	for (int i = 1; i <= GetPlayerShop().GetShopGridNum(); ++i)
	{
		// ��Ʒ��ʾ��˫�������¼� & ���������ʾ��Ʒ��Ϣ�¼�
		sprintf(tempText, "PlayerShop/backgrond/Goods%d", i);
		wnd = mainPage->getChildRecursive(tempText);
		if (wnd)
		{
			wnd->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,
				CEGUI::Event::Subscriber(OnPlayerShopResetGoods));

			wnd->subscribeEvent(CEGUI::Window::EventMouseClick,
				CEGUI::Event::Subscriber(OnPlayerShopShowGoodsInfo));
		}

		// ���ӹ���������ť�¼�
		sprintf(tempText, "PlayerShop/backgrond/Goods%d/AddBuyNum", i);
		wnd = mainPage->getChildRecursive(tempText);
		if (wnd)
		{
			wnd->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(OnPlayerShopAddBuyNum));
		}

		// ���ٹ���������ť�¼�
		sprintf(tempText, "PlayerShop/backgrond/Goods%d/SubBuyNum", i);
		wnd = mainPage->getChildRecursive(tempText);
		if (wnd)
		{
			wnd->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(OnPlayerShopSubBuyNum));
		}

		// �������������
		sprintf(tempText, "PlayerShop/backgrond/Goods%d/BuyNum", i);
		wnd = mainPage->getChildRecursive(tempText);
		if (wnd)
		{
			wnd->subscribeEvent(CEGUI::Window::EventDeactivated,
				CEGUI::SubscriberSlot(OnPlayerShopBuyNumChange));
		}
	}
}

bool OnPlayerShopPageOpen(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	PlayerShop& playerShop = GetPlayerShop();

	// �򿪸����̵�ǰ�ر��������棨��Ƭ��Ƕ��������TalkBox��NPC�̵ꡢ���ס��ֿ⡢װ����ħ���ȣ�

	// �򿪱���

	playerShop.SetIsOpenShop(true);

	// �����԰����
	FireUIEvent("PlayerShopMessageBoard", "Opened");

	// ���²������水ť״̬
	FireUIEvent("PlayerShop", "UpdateOperBtn");
	// ���»���ͳ����Ϣ
	FireUIEvent("PlayerShop", "UpdateMoneyInfo");

	playerShop.UpdataPlayerShopGoodsList();

	return true;
}

bool OnPlayerShopPageClose(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	GetPlayerShop().OnCloseShop();
	return true;
}

bool OnPlayerShopBlackboard(const CEGUI::EventArgs& e)
{
	return true;
}

bool OnHideShowPlayerShop(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	int curShopState = GetPlayerShop().GetCurShopState();

	if (curShopState==PlayerShop::SET_SHOP)
	{
	}
	else if (curShopState==PlayerShop::SHOPPING_SHOP)
	{
		// ȷ��Ҫ���׵���Ʒ
		if (!GetPlayerShop().GetbSetTrue())
		{		
			if(!GetPlayerShop().IsTradeItemReady())
				return false;
			GetPlayerShop().CalcTradeGolds();
			FireUIEvent("PlayerShop", "UpdateMoneyInfo");
			GetPlayerShop().SetbSetTrue(true);
			FireUIEvent("PlayerShop", "UpdateOperBtn");
			//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-03.wav");
		}
		// ȡ��Ҫ���׵���Ʒ
		else if (GetPlayerShop().GetbSetTrue())
		{
			GetPlayerShop().CancelAllTrade();
			FireUIEvent("PlayerShop", "UpdateMoneyInfo");
			GetPlayerShop().SetbSetTrue(false);
			FireUIEvent("PlayerShop", "UpdateOperBtn");
			//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-03.wav");
		}
	}

	return true;
}

bool OnBeginStopPlayerShop(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CPlayer* pPlayer = GetGame()->GetMainPlayer();
	if (!pPlayer) return false;

	int curShopState = GetPlayerShop().GetCurShopState();
	// ��ʼ��̯
	if (curShopState==PlayerShop::SET_SHOP)
	{
		/// �ж������Χ5*5�ķ�Χ���Ƿ��б������Ѿ��ڿ����ˣ��������������
		if (pPlayer->CheckIsAnyBodyShopOpenedOnRange(2))
		{
			GetChatListPage().AddChatWords(AppFrame::GetText("Shop_16") //�Ѿ������������Χ���꣬��������ѡ�񿪵��ַ
                , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
			FireUIEvent("PlayerShop", "Closed");
			return false;
		}
		// ���Ͱ�̯��Ϣ
		//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-03.wav");

		if (!pPlayer->IsRecruitingTeam())
		{
			GetPlayerShop().SetbSetTrue(true);
			GetPlayerShop().SendOpenShopMes();
		}else
			GetChatListPage().AddChatWords(AppFrame::GetText("Shop_12") //��ļ״̬�²��ܿ���
            , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
	}
	// ȡ����̯
	else if (curShopState==PlayerShop::OPEN_SHOP)
	{
		GetPlayerShop().SendCloseShopMes();
	}
	// ����
	else if (curShopState==PlayerShop::SHOPPING_SHOP)
	{
		// ���ͽ�����Ϣ
		GetPlayerShop().SendBuyGoodsMes();
		//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-03.wav");
	}

	return true;
}

bool OnPlayerShopUpdateMoneyInfo(const CEGUI::EventArgs& e)
{
	// Ŀǰ�����̵�ֻ֧�ֽ��һ�ֻ�������@todo
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CPlayer* pPlayer = GetGame()->GetMainPlayer();
	if (!pPlayer) return false;

	CEGUI::Window* tradeGoldsWnd = wnd->getChildRecursive("PlayerShop/backgrond/SumUp/TradeGolds");
	CEGUI::Window* haveGoldsWnd = wnd->getChildRecursive("PlayerShop/backgrond/SumUp/HaveGolds");
	if (!tradeGoldsWnd || !haveGoldsWnd) return false;

	int curShopState = GetPlayerShop().GetCurShopState();
	ulonglong tradeGolds = GetPlayerShop().GetTradeGold();

	char str[256];
	if(curShopState==PlayerShop::SET_SHOP || curShopState==PlayerShop::OPEN_SHOP)
	{
		// ��ý��
		if (tradeGolds>=2000000000)
		{
			wsprintf(str,"+ %d",2000000000);
		}else
			wsprintf(str,"+ %d",tradeGolds);
		tradeGoldsWnd->setText(ToCEGUIString(str));

		// ���н��
		if (pPlayer->GetMoney()>=2000000000)
		{
			wsprintf(str,"%d",2000000000);
		}else
			wsprintf(str,"%d",pPlayer->GetMoney());
		haveGoldsWnd->setText(ToCEGUIString(str));
	}
	else if(curShopState==PlayerShop::SHOPPING_SHOP)
	{
		// ���ѽ��
		if (tradeGolds>=2000000000)
		{
			wsprintf(str,"- %d",2000000000);
		}else
			wsprintf(str,"- %d",tradeGolds);
		tradeGoldsWnd->setText(ToCEGUIString(str));

		// ���н��
		if (pPlayer->GetMoney()>=2000000000)
		{
			wsprintf(str,"%d",2000000000);
		}else
			wsprintf(str,"%d",pPlayer->GetMoney());
		haveGoldsWnd->setText(ToCEGUIString(str));
	}
	return true;
}

// ���½��������ť״̬
bool OnPlayerShopOperBtnUpdate(const CEGUI::EventArgs& e)
{
	CEGUI::Window* mainPage = WEArgs(e).window;
	if(!mainPage) return false;

	CEGUI::Window* messageBoardBtn = mainPage->getChildRecursive("PlayerShop/backgrond/Messageboard");
	CEGUI::Window* HideShowBtn = mainPage->getChildRecursive("PlayerShop/backgrond/HideShowBtn");
	CEGUI::Window* beginStopBtn = mainPage->getChildRecursive("PlayerShop/backgrond/BeginStopBtn");
	CEGUI::Window* closeBtn = mainPage->getChildRecursive("PlayerShop/backgrond/CloseBtn");
	if (!messageBoardBtn || !HideShowBtn || !beginStopBtn || !closeBtn) return false;

	PlayerShop& playerShop = GetPlayerShop();
	int shopState = playerShop.GetCurShopState();
	bool shopSetSure = playerShop.GetbSetTrue();
	if(shopState < 0 || shopState >= PlayerShop::SHOP_STATE)
		return false;

	vector<PlayerShop::tagGoodsItem>& myShopGoods = playerShop.GetMyShopGoods();

	// �����̵�
	if(shopState == PlayerShop::SET_SHOP)
	{
		if (!myShopGoods.empty())
		{
			// ����̵�������Ʒ�Ͱѿ�ʼ��̯��ʾ����
			beginStopBtn->setVisible(true);
			beginStopBtn->enable();
			beginStopBtn->setText(ToCEGUIString(AppFrame::GetText("��ʼ����")));
		}else
		{
			// ����̵�������Ʒ���ÿ�ʼ��̯����ʾ����
			beginStopBtn->setVisible(false);
		}
		HideShowBtn->setVisible(false);
	}
	// ���̵�
	else if(shopState == PlayerShop::OPEN_SHOP)
	{
		// ֹͣ��̯ & �����̵���ʾ����
		beginStopBtn->setVisible(true);
		beginStopBtn->enable();
		beginStopBtn->setText(ToCEGUIString(AppFrame::GetText("ֹͣ����")));
		HideShowBtn->setVisible(true);
		HideShowBtn->enable();
		HideShowBtn->setText(ToCEGUIString(AppFrame::GetText("�����̵�")));
	}
	// ����״̬
	else if(shopState == PlayerShop::SHOPPING_SHOP)
	{	
		if (shopSetSure)
		{
			// ���� & ������ʾ����
			beginStopBtn->setVisible(true);
			beginStopBtn->enable();
			beginStopBtn->setText(ToCEGUIString(AppFrame::GetText("����")));
			HideShowBtn->setVisible(true);
			HideShowBtn->enable();
			HideShowBtn->setText(ToCEGUIString(AppFrame::GetText("����")));
		}
		else
		{
			// ȷ�� & ������ʾ����
			beginStopBtn->setVisible(true);
			beginStopBtn->disable();
			beginStopBtn->setText(ToCEGUIString(AppFrame::GetText("����")));
			HideShowBtn->setVisible(true);
			HideShowBtn->enable();
			HideShowBtn->setText(ToCEGUIString(AppFrame::GetText("ȷ��")));
		}
	}

	return true;
}

bool OnPlayerShopOrderGoods(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CPlayer* pPlayer = GetGame()->GetMainPlayer();
	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
	if (!pPlayer || !pHand) return false;

	PlayerShop& playShop = GetPlayerShop();
	ChatListPage& chatListPage = GetChatListPage();
	int curShopState = playShop.GetCurShopState();
	int index = -1;

	FireUIEvent("PlayerShop", "ClearGoodsInfo");

	CGoods* pHandGoods = pHand->GetPGoodsOfMainPlayerHand();
	CPlayer::tagGoods* psTagGoods = pHand->GetTGoodsOfMainPlayerHand();
	// �����������Ʒ
	if (pHandGoods!=NULL && curShopState==PlayerShop::SET_SHOP && !playShop.GetbSetTrue())
	{
		if (pHand->GetTGoodsOfMainPlayerHand()->lType > DEPOT_CONTAINER_TYPE_BEGIN_VALUE  
			&&   pHand->GetTGoodsOfMainPlayerHand()->lType < DEPOT_CONTAINER_TYPE_END_VALUE )
		{
            chatListPage.AddChatWords(AppFrame::GetText("Shop_13")  //�ֿ��е���Ʒ����ֱ�ӳ��ۣ�
                ,0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
			return false;
		}
		if (pHand->GetTGoodsOfMainPlayerHand()->lType==PEI_EQUIPMENT)
		{
			chatListPage.AddChatWords(AppFrame::GetText("Package_6")  //���õ�����Ʒ��%s
                , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
			return false;
		}
		if (pHand->GetTGoodsOfMainPlayerHand()->lType==PEI_SUBPACK)
		{
			chatListPage.AddChatWords(AppFrame::GetText("Package_16") //����ʹ�õ��ӱ������ܳ��ۣ�
                , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
			return false;
		}
		//�����ѫ�£����ܷ���
		if(pHandGoods->GetProperty()->dwType == GBT_MEDAL)
			return false;
		long lFreezeVal = pHandGoods->GetEffectVal(GAP_FREEZE,1);
		long lBindVal = pHandGoods->GetEffectVal(GAP_BIND,1);
		//�������Ʒ���ܷ�������̵���н���
		if (lFreezeVal!=eFT_UnFrost&&lFreezeVal!=eFT_CantFrost)
		{
			GetInst(MsgEventManager).PushEvent(Msg_Ok, AppFrame::GetText("Goods_156")); //����Ʒ���ڶ���״̬,���ܳ���!
			pHandGoods->SetHaveShadowState(false);
			// @todo ����
			/*if (pItemPage!=NULL)
			{
				pItemPage->UpdateGoodsShowByCGoods(pHandGoods);
			}*/
			pHand->ClearMainPlayerHand();
			return false;
		}
		//�󶨵���Ʒ���ܷ�������̵���н���
		if (lBindVal==1||lBindVal==3)
		{
			GetInst(MsgEventManager).PushEvent(Msg_Ok, AppFrame::GetText("Goods_157"));//����Ʒ���ڰ�״̬,���ܳ���!
			pHandGoods->SetHaveShadowState(false);
			// @todo ����
			//if (pItemPage!=NULL)
			//{
			//	pItemPage->UpdateGoodsShowByCGoods(pHandGoods);
			//	//pItemPage->UpdataAllItems();
			//	//pItemPage->UpdataAllSubPack();
			//}
			pHand->ClearMainPlayerHand();
			return false;
		}
		// ���ɽ��׵Ĳ��ܷ�������̵��з���
		if (pHandGoods->GetEffectVal(GAP_PARTICULAR_ATTRIBUTE,1)&32)
		{
            chatListPage.AddChatWords(AppFrame::GetText("Shop_11")  //���ɽ�����Ʒ���ܷ�����
                , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
			return false;
		}

		// ����Ʒ�����嵥����
		if (index == -1 || (playShop.GetCurGoodsNum()<playShop.GetShopGridNum())) 
		{
			index = playShop.GetCurGoodsNum();
		}
		playShop.OnOrderPageOpen(pHandGoods,index);

		pHand->ClearMainPlayerHand();
	}
	return true;
}

// �����Ʒ�����е���ʾ
bool OnPlayerShopClearGoodsInfo(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::Window* iconWnd = wnd->getChildRecursive("PlayerShop/backgrond/GoodsInfo/Icon");
	CEGUI::Window* nameWnd = wnd->getChildRecursive("PlayerShop/backgrond/GoodsInfo/Name");
	CEGUI::Window* oneGroupNumWnd = wnd->getChildRecursive("PlayerShop/backgrond/GoodsInfo/OneGroupNum");
	CEGUI::Window* priceWnd = wnd->getChildRecursive("PlayerShop/backgrond/GoodsInfo/Price");
	CEGUI::Window* averagePriceWnd = wnd->getChildRecursive("PlayerShop/backgrond/GoodsInfo/AveragePrice");
	if (!iconWnd || !nameWnd || !oneGroupNumWnd || !priceWnd || !averagePriceWnd)
		return false;

	iconWnd->setProperty("Image","");
	nameWnd->setText("");
	oneGroupNumWnd->setText("");
	priceWnd->setText("");
	averagePriceWnd->setText("");

	return true;
}

// ��ӻ����б�
void SetPlayerShopGoodsItemInfo(PlayerShop::tagGoodsItem& tgGoodsItem, int index)
{
	/// ��Ŀ����ͼ

	CEGUI::Window* wnd;
	char tempText[256];
	char strImageFilePath[128] = "";
	char strImageFileName[128] = "";

	sprintf(tempText, "PlayerShop/backgrond/Goods%d", index+1);
	CEGUI::Window* goodsWnd = GetWndMgr().getWindow(tempText);
	goodsWnd->setUserData(tgGoodsItem.pItemGoods);
	goodsWnd->setVisible(true);

	//�����̵�״̬����UI����
	int shopState = GetPlayerShop().GetCurShopState();
	if(shopState >= 0 && shopState < PlayerShop::SHOP_STATE)
	{
		//�����̵�
		if(shopState == PlayerShop::SET_SHOP)
		{
			sprintf(tempText, "PlayerShop/backgrond/Goods%d/BuyNum", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);
			
			sprintf(tempText, "PlayerShop/backgrond/Goods%d/AddBuyNum", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);

			sprintf(tempText, "PlayerShop/backgrond/Goods%d/SubBuyNum", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);

			sprintf(tempText, "PlayerShop/backgrond/Goods%d/Text", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setText(ToCEGUIString("˫���������ü۸�"));
		}
		//����
		else if(shopState == PlayerShop::OPEN_SHOP)
		{
			sprintf(tempText, "PlayerShop/backgrond/Goods%d/BuyNum", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);

			sprintf(tempText, "PlayerShop/backgrond/Goods%d/AddBuyNum", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);

			sprintf(tempText, "PlayerShop/backgrond/Goods%d/SubBuyNum", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);

			sprintf(tempText, "PlayerShop/backgrond/Goods%d/Text", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);
		}
		// ���̵�ҳ��
		else if( shopState == PlayerShop::SHOPPING_SHOP)
		{
			sprintf(tempText, "PlayerShop/backgrond/Goods%d/Text", index+1);
			wnd = GetWndMgr().getWindow(tempText);
			wnd->setVisible(false);
		}
	}

	// ������Ʒ�ϼ���Ϣ
	// ��ƷͼƬ
	sprintf(tempText, "PlayerShop/backgrond/Goods%d/Icon", index+1);
	CEGUI::DefaultWindow* iconWnd = WDefaultWindow(GetWndMgr().getWindow(tempText));
	if(iconWnd && tgGoodsItem.goodsIconId != 0)
	{
		// ��õ�ǰ��������Ӧ����Ʒͼ�����ݣ�������ͼ�����óɶ�Ӧ��������Ķ���ͼƬ��
		const char *strIconPath = GetGame()->GetPicList()->GetPicFilePathName(CPicList::PT_GOODS_ICON, tgGoodsItem.goodsIconId);
		GetFilePath(strIconPath,strImageFilePath);
		GetFileName(strIconPath,strImageFileName);
		CEGUI::String strImagesetName = "GoodIcon/";
		strImagesetName += strImageFileName;
		SetBackGroundImage(iconWnd,strImagesetName.c_str(),strImageFilePath,strImageFileName);

		// ����Ʒ������1��ʱ�����ʾ����
		if(tgGoodsItem.tradeType==PlayerShop::TT_GROUP && tgGoodsItem.oneGroupNum>=1)
		{
			char strGoodsNum[32];
			sprintf_s(strGoodsNum,"%4d",tgGoodsItem.oneGroupNum);
			iconWnd->setText(strGoodsNum);
		}
	}

	// ��Ʒ��
	sprintf(tempText, "PlayerShop/backgrond/Goods%d/Num", index+1);
	wnd = GetWndMgr().getWindow(tempText);
	if (wnd)
	{	
		if (tgGoodsItem.tradeType==PlayerShop::TT_SINGLE)
		{
			sprintf_s(tempText,"ʣ%d��",tgGoodsItem.groupNum);
		}else if (tgGoodsItem.tradeType==PlayerShop::TT_GROUP)
		{
			sprintf_s(tempText,"ʣ%d��",tgGoodsItem.groupNum);
		}
		wnd->setText(ToCEGUIString(tempText));
	}
	
	// ��Ʒ��
	sprintf(tempText, "PlayerShop/backgrond/Goods%d/Name", index+1);
	wnd = GetWndMgr().getWindow(tempText);
	if (wnd)
	{
		char strGoodsName[32] = "";
		DWORD dwNameSize = (DWORD)strlen(tgGoodsItem.strGoodsName.c_str());
		if (tgGoodsItem.tradeType==PlayerShop::TT_SINGLE&&dwNameSize>18)
		{
			_snprintf(strGoodsName,19,"%s",tgGoodsItem.strGoodsName.c_str());
			sprintf((strGoodsName+18),"...");
		}else if (tgGoodsItem.tradeType==PlayerShop::TT_GROUP&&dwNameSize>10)
		{
			_snprintf(strGoodsName,11,"%s",tgGoodsItem.strGoodsName.c_str());
			sprintf((strGoodsName+10),"...");
		}
		else
			sprintf(strGoodsName,"%s",tgGoodsItem.strGoodsName.c_str());
		wnd->setText(ToCEGUIString(strGoodsName));
		wnd->setTooltipText(tgGoodsItem.strGoodsName);
	}

	// ���׷�ʽ
	sprintf(tempText, "PlayerShop/backgrond/Goods%d/TradeType", index+1);
	wnd = GetWndMgr().getWindow(tempText);
	if (wnd)
	{
		if (tgGoodsItem.tradeType==PlayerShop::TT_SINGLE)
		{
			wnd->setText(ToCEGUIString("��������"));
			//CEGUI::FreeTypeFont * font = static_cast<CEGUI::FreeTypeFont*>( wnd->getFont() );
			//font->setPointSize(8);
		}
		else if (tgGoodsItem.tradeType==PlayerShop::TT_GROUP)
		{
			wnd->setText(ToCEGUIString("���鷷��"));
		}
	}

	// �۸�
	sprintf(tempText, "PlayerShop/backgrond/Goods%d/Price", index+1);
	wnd = GetWndMgr().getWindow(tempText);
	if (wnd)
	{
		sprintf(tempText,"%d",tgGoodsItem.price);
		wnd->setText(tempText);
	}
}

bool OnPlayerShopResetGoods(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CGoods* goods = static_cast<CGoods*>(wnd->getUserData());
	if (!goods) return false;

	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
	if (!pPlayer || !pHand) return false;
	
	int shopState = GetPlayerShop().GetCurShopState();
	if (shopState==PlayerShop::SET_SHOP)
	{
		GetPlayerShop().ReMoveShopGoods(goods->GetIndex(),goods->GetExID());
		wnd->setUserData(NULL);
		int index = GetPlayerShop().GetCurGoodsNum();
		GetPlayerShop().OnOrderPageOpen(goods, index);

		FireUIEvent("PlayerShop","ClearGoodsInfo");
		FireUIEvent("PlayerShop","UpdateMoneyInfo");

		return true;
	}

	return false;
}

bool OnPlayerShopShowGoodsInfo(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CGoods* goods = static_cast<CGoods*>(wnd->getUserData());
	if (!goods) return false;

	CEGUI::DefaultWindow* iconWnd = WDefaultWindow(GetWndMgr().getWindow("PlayerShop/backgrond/GoodsInfo/Icon"));
	CEGUI::Window* nameWnd = GetWndMgr().getWindow("PlayerShop/backgrond/GoodsInfo/Name");
	CEGUI::Window* oneGroupNumWnd = GetWndMgr().getWindow("PlayerShop/backgrond/GoodsInfo/OneGroupNum");
	CEGUI::Window* priceWnd = GetWndMgr().getWindow("PlayerShop/backgrond/GoodsInfo/Price");
	CEGUI::Window* averagePriceWnd = GetWndMgr().getWindow("PlayerShop/backgrond/GoodsInfo/AveragePrice");
	if (!iconWnd || !nameWnd || !oneGroupNumWnd || !priceWnd || !averagePriceWnd)
		return false;

	char tempText[256];
	char strImageFilePath[128] = "";
	char strImageFileName[128] = "";

	PlayerShop::tagGoodsItem* pGoodsItem = GetPlayerShop().FindtagGoods(goods);
	if (pGoodsItem!=NULL)
	{
		// ��Ʒ����
		DWORD dwNameSize = (DWORD)strlen(pGoodsItem->strGoodsName.c_str());
		if (dwNameSize>20)
		{
			_snprintf(tempText,21,"%s", pGoodsItem->strGoodsName.c_str());
			sprintf((tempText+20),"...");
		}else
			sprintf(tempText,"%s", pGoodsItem->strGoodsName.c_str());
		nameWnd->setText(ToCEGUIString(tempText));

		// ��ƷͼƬ
		const char *strIconPath = GetGame()->GetPicList()->GetPicFilePathName(CPicList::PT_GOODS_ICON, pGoodsItem->goodsIconId);
		GetFilePath(strIconPath,strImageFilePath);
		GetFileName(strIconPath,strImageFileName);
		CEGUI::String strImagesetName = "GoodIcon/";
		strImagesetName += strImageFileName;
		SetBackGroundImage(iconWnd,strImagesetName.c_str(),strImageFilePath,strImageFileName);

		// ��Ʒ�������
		sprintf(tempText,"%d",pGoodsItem->oneGroupNum);
		oneGroupNumWnd->setText(ToCEGUIString(tempText));

		// ��Ʒ�ۼ�
		sprintf(tempText,"%d", pGoodsItem->price);
		priceWnd->setText(ToCEGUIString(tempText));

		// ��Ʒ��������
		char strGoodsPrice[64] = "",strNumLen[64] = "";
		sprintf(strNumLen,"%d",pGoodsItem->price/pGoodsItem->oneGroupNum);
		float fPrice = static_cast<float>(pGoodsItem->price)/static_cast<float>(pGoodsItem->oneGroupNum);
		sprintf(strGoodsPrice,"%*.2f",(int)strlen(strNumLen)+2,fPrice);
		averagePriceWnd->setText(ToCEGUIString(strGoodsPrice));
	}

	return true;
}

bool OnPlayerShopAddBuyNum(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::Window* goodsWnd = wnd->getParent();
	if (goodsWnd)
	{	
		CGoods* goods = static_cast<CGoods*>(goodsWnd->getUserData());
		if (!goods) return false;

		PlayerShop::tagGoodsItem* pGoodsItem = GetPlayerShop().FindtagGoods(goods);
		if (pGoodsItem!=NULL)
		{
			char str[32];
			// ȡ�������ؼ���
			CEGUI::String name = wnd->getName();
			name.assign(name, 0, name.find_last_of("/"));
			name += "/BuyNum";

			CEGUI::Window* buyNumWnd = GetWndMgr().getWindow(name);
			ulong num = atoi(buyNumWnd->getText().c_str());

			if (num>=pGoodsItem->groupNum)
			{
				sprintf(str,"%d",num);
				wnd->disable();
			}
			else
				sprintf(str,"%d",++num);
			buyNumWnd->setText(ToCEGUIString(str));
			pGoodsItem->readyTradeNum = num;			
		}
	}

	return true;
}

bool OnPlayerShopSubBuyNum(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::Window* goodsWnd = wnd->getParent();
	if (goodsWnd)
	{	
		CGoods* goods = static_cast<CGoods*>(goodsWnd->getUserData());
		if (!goods) return false;

		PlayerShop::tagGoodsItem* pGoodsItem = GetPlayerShop().FindtagGoods(goods);
		if (pGoodsItem!=NULL)
		{
			char str[32];
			// ȡ�������ؼ���
			CEGUI::String name = wnd->getName();
			name.assign(name, 0, name.find_last_of("/"));
			name += "/BuyNum";

			CEGUI::Window* buyNumWnd = GetWndMgr().getWindow(name);
			int num = atoi(buyNumWnd->getText().c_str());

			if (num<=0)
			{
				sprintf(str,"%d",0);
				wnd->disable();
			}
			else
				sprintf(str,"%d",num--);
			buyNumWnd->setText(ToCEGUIString(str));
			pGoodsItem->readyTradeNum = num;	
		}
	}

	return true;
}

bool OnPlayerShopBuyNumChange(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::String buyNum = wnd->getText();
	char str[32] = "";

	CEGUI::Window* goodsWnd = wnd->getParent();
	if (goodsWnd)
	{	
		CGoods* goods = static_cast<CGoods*>(goodsWnd->getUserData());
		if (!goods) return false;

		PlayerShop::tagGoodsItem* pGoodsItem = GetPlayerShop().FindtagGoods(goods);
		if (pGoodsItem!=NULL)
		{
			ulong num = atoi(buyNum.c_str());

			if (num>=pGoodsItem->groupNum)
			{
				sprintf(str,"%d",pGoodsItem->groupNum);
			}
			else if (num<=0)
			{
				sprintf(str,"%d",0);
			}
				sprintf(str,"%d",num);
			wnd->setText(ToCEGUIString(str));
			pGoodsItem->readyTradeNum = num;	
		}
	}
	return true;
}