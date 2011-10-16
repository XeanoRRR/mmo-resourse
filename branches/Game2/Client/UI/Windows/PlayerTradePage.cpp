#include "StdAfx.h"
#include "../UIDef.h"

#include "../GamePage/Package/PackagePage.h"
#include "../GamePage/ChatList/ChatListPage.h"

#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "../../GameClient/ClientApp/MainPlayerHand.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../GameClient/ClientApp/Other/AudioList.h"
#include "../../GameClient/ClientApp/TradeSystem/PlayerTrade.h"


CEGUI::Window* InitPlayerTradePage(void);							///< ��ʼ�����׽���
void SubscribeTradePageEvent(CEGUI::Window* mainPage);				///< ���׽����¼�ע��
bool OnTradeAffirmBtn(const CEGUI::EventArgs& e);					///< ����ȷ�ϰ�ť
bool OnTradeOkBtn(const CEGUI::EventArgs& e);						///< ���װ�ť
bool OnTradeGoodsDropped(const CEGUI::EventArgs& e);				///< ��ק���뽻����Ʒ
bool OnTradeGoodsIconLeaves(const CEGUI::EventArgs& e);				///< ��קɾ��������Ʒ
bool OnTradeGoodsIconDragStart(const CEGUI::EventArgs& e);			///< 
bool OnTradeGoodsMouseRBtnDown(const CEGUI::EventArgs& e);			///< ��Ӧ�Ҽ��Ƴ�������Ʒ��Ϣ
bool OnPlayerTradeGoldsChange(const CEGUI::EventArgs& e);			///< ��Ӧ���׽������¼�

bool OnTradePageOpen(const CEGUI::EventArgs& e);					///< ����ҽ��׽���
bool OnSendTradePageCloseMsg(const CEGUI::EventArgs& e);			///< ��Ӧ��ҹرս���
bool OnTradePageClose(const CEGUI::EventArgs& e);					///< �ر���ҽ��׽���

void UpdataPlayerTradeItem(int index,DWORD dwWhose);				///< ���½���˫��ҳ���ϵ���Ʒ��ʾ
bool OnUpdatePlayerTradePage(const CEGUI::EventArgs& e);			///< ������������ҳ�����ʾ


CEGUI::Window* InitPlayerTradePage(void)
{
	CEGUI::Window* wnd = LoadUI("PlayerTrade");

	SubscribeTradePageEvent(wnd);

	wnd->setVisible(false);
	return wnd;
}

void SubscribeTradePageEvent(CEGUI::Window* mainPage)
{
	if (!mainPage)
		return;

	CEGUI::Window* btn;
	CEGUI::DragContainer* dragContainer;
	CEGUI::Window* dragItem;
	char tempText[256];

	// ע�ύ��ȷ�ϰ�ť�¼�
	btn = WPushButton(mainPage->getChildRecursive("TradePage/backgrond/Affirm"));
	if (btn)
	{
		btn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnTradeAffirmBtn));
	}

	// ע�ύ�װ�ť�¼�
	btn = WPushButton(mainPage->getChildRecursive("TradePage/backgrond/Trade"));
	if (btn)
	{
		btn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnTradeOkBtn));
	}

	// �Լ����׽��� DragContainer �¼���Ŀǰֻ֧�ֽ���˫����GRIDNUM��λ
	int gridNum = GetInst(PlayerTrade).GetTradeGridNum();
	for (int i=0; i<gridNum; ++i)
	{
		sprintf(tempText, "TradePage/backgrond/SelfTrader/Item%d/DragItem", i+1);
		dragContainer = WDragContainer(mainPage->getChildRecursive(tempText));

		sprintf(tempText, "TradePage/backgrond/SelfTrader/Item%d/DragItem/IconImage", i+1);
		dragItem = mainPage->getChildRecursive(tempText);

		if (dragContainer && dragItem)
		{
			dragContainer->subscribeEvent(CEGUI::Window::EventDragDropItemDropped,
				CEGUI::Event::Subscriber(OnTradeGoodsDropped));

			dragContainer->subscribeEvent(CEGUI::Window::EventDragDropItemLeaves,
				CEGUI::Event::Subscriber(OnTradeGoodsIconLeaves));

			dragItem->subscribeEvent(CEGUI::DragContainer::EventDragStarted,
				CEGUI::Event::Subscriber(OnTradeGoodsIconDragStart));

			dragContainer->subscribeEvent(CEGUI::Window::EventMouseButtonDown,
				CEGUI::Event::Subscriber(OnTradeGoodsMouseRBtnDown));
		}
	}

	// ���׽����
	CEGUI::Editbox* goldNum = WEditBox(mainPage->getChildRecursive("TradePage/backgrond/SelfTrader/SelfGold"));
	if (goldNum)
	{	
		goldNum->subscribeEvent(CEGUI::Window::EventDeactivated,
			CEGUI::Event::Subscriber(OnPlayerTradeGoldsChange));
	}

	// �������&�رղ���
	mainPage->subscribeEvent("Opened", CEGUI::Event::Subscriber(OnTradePageOpen));
	mainPage->subscribeEvent("Closed", CEGUI::Event::Subscriber(OnSendTradePageCloseMsg));

	mainPage->subscribeEvent("ClosePage", CEGUI::Event::Subscriber(OnTradePageClose));

	// ע�����ҳ���¼�
	mainPage->subscribeEvent("UpdatePage", CEGUI::Event::Subscriber(OnUpdatePlayerTradePage));
}


bool OnTradePageOpen(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	char tempText[256];

	// �򿪽��׽���ǰ���ر��������棨��Ƭ��Ƕ��������TalkBox��NPC�̵ꡢ��ֵ�̵ꡢ�����̵ꡢ���䡢�ֿ�Ƚ��棩

	CEGUI::Window* OtherName = wnd->getChildRecursive("TradePage/backgrond/OtherName");
	OtherName->setText(ToCEGUIString(GetInst(PlayerTrade).GetOther()->GetName()));

	int gridNum = GetInst(PlayerTrade).GetTradeGridNum();
	for (int i=0; i<gridNum; ++i)
	{
		sprintf(tempText, "TradePage/backgrond/SelfTrader/Item%d/DragItem/IconImage", i+1);
		CEGUI::Window* dragItem = wnd->getChildRecursive(tempText);
		dragItem->setProperty("Image","");
	}

	for (int j=0; j<gridNum; ++j)
	{
		sprintf(tempText, "TradePage/backgrond/OtherTrader/Item%d", j+1);
		CEGUI::Window* Item = wnd->getChildRecursive(tempText);
		Item->setProperty("Image","");
	}

	CEGUI::Window* affirmBtn = wnd->getChildRecursive("TradePage/backgrond/Affirm");
	affirmBtn->setText(ToCEGUIString("ȷ��"));

	CEGUI::Window* tradeBtn = wnd->getChildRecursive("TradePage/backgrond/Trade");
	tradeBtn->disable();

	return true;
}

bool OnSendTradePageCloseMsg(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	if (pMainPlayer!=NULL)
	{
		if (!pMainPlayer->IsDied())
		{
			GetInst(PlayerTrade).SendClosePageMes();
		}
	}

	return true;
}

bool OnTradePageClose(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	GetInst(PlayerTrade).CloseTrade();

	CEGUI::Window* OtherName = wnd->getChildRecursive("TradePage/backgrond/OtherName");
	OtherName->setText("");

	CEGUI::Window* myGold = wnd->getChildRecursive("TradePage/backgrond/SelfTrader/SelfGold");
	myGold->setText("");
	CEGUI::Window* otherGold = wnd->getChildRecursive("TradePage/backgrond/OtherTrader/OtherGold");
	otherGold->setText("");

	wnd->setVisible(false);
	return true;
}

bool OnTradeAffirmBtn(const CEGUI::EventArgs& e)
{	
	CEGUI::Window* wnd = WEArgs(e).window;
	if (!wnd) return false;

	if (!GetInst(PlayerTrade).GetLockedState(PlayerTrade::Trade_Me))
	{
		GetInst(PlayerTrade).SendCoinMes();
		GetInst(PlayerTrade).SendReadyTradeGoodsMes();
		FireUIEvent("PlayerTrade","UpdatePage");
	}
	else
	{
		GetInst(PlayerTrade).SendChangeTradeGoodsMes();
		FireUIEvent("PlayerTrade","UpdatePage");
	}

	return true;
}

bool OnTradeOkBtn(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if (!wnd)
		return false;
	
	GetInst(PlayerTrade).SendTradingGoodsMes();
	wnd->disable();

	return true;
}

bool OnTradeGoodsDropped(const CEGUI::EventArgs& e)
{
	CEGUI::DragContainer* dragTargetWnd = WDragContainer(WEArgs(e).window);
	CEGUI::Window* dropWnd = static_cast<const CEGUI::DragDropEventArgs&>(e).dragDropItem;

#ifdef _DEBUG
	const char* targetName = dragTargetWnd->getName().c_str();
	const char* dropName = dropWnd->getName().c_str();
#endif

	CEGUI::Window* packagePage = GetInst(PackagePage).GetPageWindow();
	if (!packagePage)
		return false;

	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
	if (!pHand)
		return false;
	CPlayer::tagGoods* ptagGoods = pHand->GetTGoodsOfMainPlayerHand();
	CGoods* pGoods = pHand->GetPGoodsOfMainPlayerHand();
	PlayerTrade::tagTradeGoods* pstGoods = GetInst(PlayerTrade).GetMyGoodsData(1);	

	//��������Ʒ
	if (pGoods!=NULL)				
	{
		if (pstGoods==NULL)
		{
			//�ж��Ƿ���ѫ�£���ѫ�����ܷ���
			if(pGoods->GetProperty()->dwType == GBT_MEDAL)
				return false;
			// ���ɽ��׵���Ʒ���ܷ��뽻��ҳ���н���
			if (pGoods->GetEffectVal(GAP_PARTICULAR_ATTRIBUTE,1)&32)
			{
				GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_10")    //��Ʒ���ɽ���
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}
			if (pHand->GetTGoodsOfMainPlayerHand()->lType==PEI_EQUIPMENT)
			{
				GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Equip_7")    //װ�����ܴ������Ͻ��ף�
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}
			if (pHand->GetTGoodsOfMainPlayerHand()->lType==PEI_SUBPACK)
			{
				GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_11")    //����ʹ�õ��ӱ������ܽ��ף�
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				return false;
			}

			long lFreezeVal = pGoods->GetEffectVal(GAP_FREEZE,1);
			long lBindVal = pGoods->GetEffectVal(GAP_BIND,1);
			if (lFreezeVal!=eFT_UnFrost&&lFreezeVal!=eFT_CantFrost)
			{
                const char* strInfo = AppFrame::GetText("Goods_152");         // ����Ʒ���ڶ���״̬,���ܽ���!
				GetInst(MsgEventManager).PushEvent(Msg_Ok, strInfo);

				pGoods->SetHaveShadowState(false);
				//packagePage->UpdateGoodsShowByCGoods(pGoods);
				pHand->ClearMainPlayerHand();
				return false;
			}
			if (lBindVal==1||lBindVal==3)
			{
				const char* strInfo = AppFrame::GetText("Goods_153");         // ����Ʒ���ڰ�״̬,���ܽ���!
				GetInst(MsgEventManager).PushEvent(Msg_Ok, strInfo);

				pGoods->SetHaveShadowState(false);
				//packagePage->UpdateGoodsShowByCGoods(pGoods);
				pHand->ClearMainPlayerHand();
				return false;
			}
			GetInst(PlayerTrade).SendAddTradeGoodsMes(1);
			GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->GetIndex());
			pHand->ClearMainPlayerHand();
			dragTargetWnd->setDraggingEnabled(false);
		}
		else
		{
			GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Equip_13")   
                , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);  //�˴�������Ʒ��
			return false;
		}
	}

	return true;
}

bool OnTradeGoodsIconLeaves(const CEGUI::EventArgs& e)
{
	return true;
}

bool OnTradeGoodsIconDragStart(const CEGUI::EventArgs& e)
{
	return true;
}

bool OnTradeGoodsMouseRBtnDown(const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs &e_mouse = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::DragContainer* dragTargetWnd = WDragContainer(e_mouse.window);
	if (!dragTargetWnd)
		return false;

#ifdef _DEBUG
	const char* targetName = dragTargetWnd->getName().c_str();
#endif

	if(e_mouse.button == CEGUI::RightButton)
	{
		CEGUI::Window* packagePage = GetInst(PackagePage).GetPageWindow();
		CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();

		if (!packagePage || !pHand)
			return false;

		CPlayer::tagGoods* ptagGoods = pHand->GetTGoodsOfMainPlayerHand();
		CGoods* pGoods = pHand->GetPGoodsOfMainPlayerHand();
		PlayerTrade::tagTradeGoods* pstGoods = GetInst(PlayerTrade).GetMyGoodsData(1);			

		if (!GetInst(PlayerTrade).IsReady())
		{				
			//ԭ��λ����Ʒ,����������Ʒ
			if (/*pGoods==NULL&&*/pstGoods!=NULL)
			{
				GetInst(PlayerTrade).SendRemoveTradeGoodsMes(1);
			}
		}
		return true;
	}
	return false;
}

// ����ҳ���ϵ���Ʒ��ʾ�������Է����������ҷ���������
void UpdataPlayerTradeItem(int index,DWORD dwWhose)
{
	CPlayer *pPlayer=GetGame()->GetMainPlayer();
	DWORD dwIconId = 0;
	CGoods *pGoods(NULL);
	PlayerTrade::tagTradeGoods* pTradeGoods(NULL);
	char strImageFilePath[128] = "";
	char strImageFileName[128] = "";
	if (dwWhose==PlayerTrade::Trade_Other)
	{
		const char * wndName = "TradePage/backgrond/OtherTrader/Item1";
		CEGUI::DefaultWindow *deWnd = WDefaultWindow(GetWndMgr().getWindow(wndName));

		map<int,CGoods>& otherTradeGoodsList = GetInst(PlayerTrade).GetOtherTradeGoodsList();

		map<int,CGoods>::iterator its = otherTradeGoodsList.find(index);
		if(its!=otherTradeGoodsList.end())
		{
			pGoods = &(otherTradeGoodsList[index]);
		}
		if (pGoods!=NULL)
		{
			dwIconId = pGoods->GetProperty()->dwIconId;
			if(dwIconId != 0)
			{
				// ��õ�ǰ��������Ӧ����Ʒͼ�����ݣ�������ͼ�����óɶ�Ӧ��������Ķ���ͼƬ��
				const char *strIconPath = GetGame()->GetPicList()->GetPicFilePathName(CPicList::PT_GOODS_ICON, dwIconId);
				if(strIconPath)
				{
					GetFilePath(strIconPath,strImageFilePath);
					GetFileName(strIconPath,strImageFileName);
					CEGUI::String strImagesetName = "GoodIcon/";
					strImagesetName += strImageFileName;
					SetBackGroundImage(deWnd,strImagesetName.c_str(),strImageFilePath,strImageFileName);
				}
				else
				{
					deWnd->setProperty("Image","");
				}

				// ����Ʒ������1��ʱ�����ʾ����
				long lNum = pGoods->GetNum();
				if( lNum > 1)
				{
					char strGoodsNum[128];
					sprintf_s(strGoodsNum,"%d",lNum);
					deWnd->setText(ToCEGUIString(strGoodsNum));
				}
				else
					deWnd->setText("");
			}
		}
		else
		{
			deWnd->setProperty("Image","");
			deWnd->setProperty("Text","");
		}
	}
	else if (dwWhose==PlayerTrade::Trade_Me)
	{
		const char * wndName = "TradePage/backgrond/SelfTrader/Item1/DragItem";
		
		CEGUI::Window *pContainer = GetWndMgr().getWindow(wndName);
		CEGUI::DefaultWindow *deWnd = WDefaultWindow(pContainer->getChildAtIdx(0));
	
		map<int,PlayerTrade::tagTradeGoods>& myTradeGoodsList = GetInst(PlayerTrade).GetMyTradeGoodsList();

		map<int,PlayerTrade::tagTradeGoods>::iterator its = myTradeGoodsList.find(index);
		if (its!=myTradeGoodsList.end())
		{
			pTradeGoods = &(myTradeGoodsList[index]);
		}
		if (pTradeGoods!=NULL)
		{
			pGoods = pPlayer->GetGoodsByGUID(pTradeGoods->guid);
		}
		if (pGoods!=NULL)
		{
			dwIconId = pTradeGoods->dwIconId;
			if(dwIconId != 0)
			{
				// ��õ�ǰ��������Ӧ����Ʒͼ�����ݣ�������ͼ�����óɶ�Ӧ��������Ķ���ͼƬ��
				const char *strIconPath = GetGame()->GetPicList()->GetPicFilePathName(CPicList::PT_GOODS_ICON, dwIconId);
				if(strIconPath)
				{
					GetFilePath(strIconPath,strImageFilePath);
					GetFileName(strIconPath,strImageFileName);
					CEGUI::String strImagesetName = "GoodIcon/";
					strImagesetName += strImageFileName;
					SetBackGroundImage(deWnd,strImagesetName.c_str(),strImageFilePath,strImageFileName);
				}
				else
				{
					deWnd->setProperty("Image","");
				}

				long lNum = pTradeGoods->lNum;
				// ����Ʒ������1��ʱ�����ʾ����
				if( lNum > 1)
				{
					char strGoodsNum[128];
					sprintf_s(strGoodsNum,"%d",lNum);
					deWnd->setText(ToCEGUIString(strGoodsNum));
				}
				else
					deWnd->setText("");
			}
		}
		else
		{
			deWnd->setProperty("Image","");
			deWnd->setProperty("Text","");
		}
	}
}

// ������������ҳ�����ʾ
bool OnUpdatePlayerTradePage(const CEGUI::EventArgs& e)
{
	CEGUI::Window* mainPage = WEArgs(e).window;
	if(!mainPage) 
		return false;

	char str[32] = "";

	CEGUI::Window* AffirmBtn = mainPage->getChildRecursive("TradePage/backgrond/Affirm");
	CEGUI::Window* okBtn = mainPage->getChildRecursive("TradePage/backgrond/Trade");
	if (!AffirmBtn || !okBtn)
		return false;


	CEGUI::Window* myGold = mainPage->getChildRecursive("TradePage/backgrond/SelfTrader/SelfGold");
	CEGUI::Window* otherGold = mainPage->getChildRecursive("TradePage/backgrond/OtherTrader/OtherGold");
	sprintf(str,"%d",GetInst(PlayerTrade).GetMyGold());
	myGold->setText(ToCEGUIString(str));
	sprintf(str,"%d",GetInst(PlayerTrade).GetOtherGold());
	otherGold->setText(ToCEGUIString(str));

	int gridNum = GetInst(PlayerTrade).GetTradeGridNum();
	for (int i=1; i<=gridNum; ++i)
	{
		UpdataPlayerTradeItem(i, PlayerTrade::Trade_Me);
		UpdataPlayerTradeItem(i, PlayerTrade::Trade_Other);
	}

	if (GetInst(PlayerTrade).GetLockedState(PlayerTrade::Trade_Me))
	{
		AffirmBtn->setText(ToCEGUIString("����"));
		if (GetInst(PlayerTrade).GetLockedState(PlayerTrade::Trade_Other))
		{
			okBtn->enable();
		}
	}
	else
	{
		AffirmBtn->setText(ToCEGUIString("ȷ��"));
		okBtn->disable();
	}
	
	// ����״̬
	return true;
}

// ��Ӧ���׽������¼�
bool OnPlayerTradeGoldsChange(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if (!wnd)
		return false;

	CEGUI::String golds = wnd->getText();
	char str[32] = "";

	// �����Ʒ��������Ƿ�Ϸ�
	ulong inputNum = (ulong)atoi(golds.c_str());
	ulong coinMaxNum = 2000000000;
	// ����ֵ�������ֵ
	if(inputNum >= coinMaxNum)
	{
		sprintf(str,"%d",coinMaxNum);
		wnd->setText(str);
		GetInst(PlayerTrade).SetMyGold(coinMaxNum);
		return true;
	}
	// ����ֵС��0
	else if(inputNum <= 0)
	{
		wnd->setText("0");
		GetInst(PlayerTrade).SetMyGold(0);
		return true;
	}
	else
	{
		sprintf(str,"%d",inputNum);
		wnd->setText(str);
		GetInst(PlayerTrade).SetMyGold(inputNum);
		return true;
	}

	return true;
}