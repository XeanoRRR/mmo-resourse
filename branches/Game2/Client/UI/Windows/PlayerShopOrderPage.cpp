#include "StdAfx.h"
#include "../UIDef.h"

#include "../GamePage/Package/PackagePage.h"
#include "../GamePage/ChatList/ChatListPage.h"

#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "../../GameClient/ClientApp/MainPlayerHand.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../GameClient/ClientApp/TradeSystem/PlayerShop.h"

CEGUI::Window* InitPlayerShopOrderPage(void);							///< ��ʼ�������̵���Ʒ�嵥����
void SubscribePlayerShopOrderPageEvent(CEGUI::Window* mainPage);		///< �����̵���Ʒ�嵥�����¼�ע��

bool OnPlayerShopOrderPageOpen(const CEGUI::EventArgs& e);				///< �򿪸����̵������嵥����
bool OnPlayerShopOrderPageClose(const CEGUI::EventArgs& e);				///< �رո����̵������嵥����

bool OnPlayerShopOrderOk(const CEGUI::EventArgs& e);					///< ȷ��
bool OnPlayerShopOrderClose(const CEGUI::EventArgs& e);					///< �ر�

bool OnPlayerShopOrderGoodsPriceChange(const CEGUI::EventArgs& e);		///< �۸���
bool OnPlayerShopOrderGoodsSaleNumChange(const CEGUI::EventArgs& e);	///< �������
bool OnPlayerShopOrderOneGroupNumChange(const CEGUI::EventArgs& e);		///< ����������

bool OnPlayerShopOrderGoodsSaleNumMaxClicked(const CEGUI::EventArgs& e);		///< ����Max��ť
bool OnPlayerShopOrderGoodsOneGroupMaxNumClicked(const CEGUI::EventArgs& e);	///< �������Max��ť

bool OnPlayerShopOrderTabChanged(const CEGUI::EventArgs& e);			///< ������ʽTabҳ�л�


void ClosePlayerShopOrderPage(void);									///< ����嵥����

CEGUI::Window* InitPlayerShopOrderPage(void)
{
#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("PlayerShopOrder.log");
#endif

	CEGUI::Window* wnd = LoadUI("PlayerShopOrder");

#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("CEGUI.log",true);
#endif

	SubscribePlayerShopOrderPageEvent(wnd);

	wnd->setVisible(false);
	return wnd;
}

void SubscribePlayerShopOrderPageEvent(CEGUI::Window* mainPage)
{
	if (!mainPage) return;
	
	// ȷ�ϰ�ť
	CEGUI::Window* singleOkBtn = mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 1/OkBtn");
	CEGUI::Window* groupOkBtn = mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 2/OkBtn");
	if (singleOkBtn && groupOkBtn )
	{
		singleOkBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopOrderOk));
		groupOkBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopOrderOk));
	}

	// �رհ�ť
	CEGUI::Window* singleCloseBtn = mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 1/CloseBtn");
	CEGUI::Window* groupCloseBtn = mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 2/CloseBtn");
	if (singleCloseBtn && groupCloseBtn )
	{
		singleCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopOrderClose));
		groupCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopOrderClose));
	}

	// ����Max��ť
	CEGUI::Window* singleMaxSaleNumBtn = mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 1/MaxSaleNumBtn");
	CEGUI::Window* groupMaxSaleNumBtn = mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 2/MaxSaleNumBtn");
	if (singleMaxSaleNumBtn && groupMaxSaleNumBtn )
	{
		singleMaxSaleNumBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopOrderGoodsSaleNumMaxClicked));
		groupMaxSaleNumBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopOrderGoodsSaleNumMaxClicked));
	}

	// �������Max��ť
	CEGUI::Window* groupOneGroupMaxNumBtn = mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 2/OneGroupMaxNumBtn");
	if (groupOneGroupMaxNumBtn)
	{
		groupOneGroupMaxNumBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(OnPlayerShopOrderGoodsOneGroupMaxNumClicked));
	}

	// ����������
	CEGUI::Editbox* oneGroupNum = WEditBox(mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 2/OneGroupNum"));
	if (oneGroupNum)
	{
		oneGroupNum->subscribeEvent(CEGUI::Window::EventDeactivated,
			CEGUI::Event::Subscriber(OnPlayerShopOrderOneGroupNumChange));
	}

	// �۸���
	CEGUI::Editbox* singlePrice = WEditBox(mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 1/Price"));
	CEGUI::Editbox* groupPrice = WEditBox(mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 1/Price"));
	if (singlePrice && groupPrice)
	{	
		singlePrice->subscribeEvent(CEGUI::Window::EventDeactivated,
			CEGUI::Event::Subscriber(OnPlayerShopOrderGoodsPriceChange));
		groupPrice->subscribeEvent(CEGUI::Window::EventDeactivated,
			CEGUI::Event::Subscriber(OnPlayerShopOrderGoodsPriceChange));
	}

	// �������
	CEGUI::Editbox* singleSaleNum = WEditBox(mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 1/SaleNum"));
	CEGUI::Editbox* groupSaleNum = WEditBox(mainPage->getChildRecursive("PlayerShopOrder/TabControl/Tab 2/SaleNum"));
	if (singleSaleNum && groupSaleNum)
	{	
		singleSaleNum->subscribeEvent(CEGUI::Window::EventDeactivated,
			CEGUI::Event::Subscriber(OnPlayerShopOrderGoodsSaleNumChange));
		groupSaleNum->subscribeEvent(CEGUI::Window::EventDeactivated,
			CEGUI::Event::Subscriber(OnPlayerShopOrderGoodsSaleNumChange));
	}

	// �л�������ʽ��ǩ
	CEGUI::Window* tabContorlWnd = mainPage->getChildRecursive("PlayerShopOrder/TabControl");
	if (tabContorlWnd)
	{
		tabContorlWnd->subscribeEvent(CEGUI::TabControl::EventSelectionChanged,
			CEGUI::Event::Subscriber(OnPlayerShopOrderTabChanged));
	}

	// �������&�رղ���
	mainPage->subscribeEvent("Opened", CEGUI::Event::Subscriber(OnPlayerShopOrderPageOpen));
	//mainPage->subscribeEvent("Closed", CEGUI::Event::Subscriber(OnPlayerShopOrderPageClose));
}


bool OnPlayerShopOrderPageOpen(const CEGUI::EventArgs& e)
{
	CEGUI::Window* mainPage = WEArgs(e).window;
	if(!mainPage) return false;

	mainPage->setAlwaysOnTop(true);

	CEGUI::Window* wnd;
	char tempText[256];
	char strImageFilePath[128] = "";
	char strImageFileName[128] = "";

	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		PlayerShop::stCacheGoodsInfo* cacheGoodsInfo = GetPlayerShop().GetCacheGoodsInfo(orderGoods->GetIndex());
		if (cacheGoodsInfo!=NULL)
		{
			// ���ɵ�����Ʒ
			if (!orderGoods->HasEffect(GAP_MAXOVERLAP))
			{
				CEGUI::Window* groupTabWnd = GetWndMgr().getWindow("PlayerShopOrder/TabControl/Tab 2");
				groupTabWnd->disable();
			}

			for (int i=1; i<=PlayerShop::TT_NUM; ++i)
			{
				// ��ƷͼƬ
				sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/Icon", i);
				CEGUI::DefaultWindow *deWnd = WDefaultWindow(GetWndMgr().getWindow(tempText));
				if (deWnd)
				{
					// ��õ�ǰ��������Ӧ����Ʒͼ�����ݣ�������ͼ�����óɶ�Ӧ��������Ķ���ͼƬ��
					const char *strIconPath = GetGame()->GetPicList()->GetPicFilePathName(CPicList::PT_GOODS_ICON, orderGoods->GetProperty()->dwIconId);
					GetFilePath(strIconPath,strImageFilePath);
					GetFileName(strIconPath,strImageFileName);
					CEGUI::String strImagesetName = "GoodIcon/";
					strImagesetName += strImageFileName;
					SetBackGroundImage(deWnd, strImagesetName.c_str(),strImageFilePath,strImageFileName);
				}

				// ��Ʒ����
				sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/Name", i);
				wnd = GetWndMgr().getWindow(tempText);
				if (wnd)
				{
					char strGoodsName[64] = "";
					DWORD dwNameSize = (DWORD)strlen(orderGoods->GetBaseProperty()->strName.c_str());
					if (dwNameSize>20)
					{
						_snprintf(strGoodsName,21,"%s",orderGoods->GetBaseProperty()->strName.c_str());
						sprintf((strGoodsName+20),"...");
					}else
						sprintf(strGoodsName,"%s",orderGoods->GetBaseProperty()->strName.c_str());
					wnd->setText(ToCEGUIString(strGoodsName));
				}

				// �۸�
				sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/Price", i);
				wnd = GetWndMgr().getWindow(tempText);
				if (wnd)
				{
					if (cacheGoodsInfo->price!=0)
					{
						char strPrice[64] = "";
						sprintf(strPrice,"%d",cacheGoodsInfo->price);
						wnd->setText(ToCEGUIString(strPrice));
					}
				}

				// ����
				sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/SaleNum", i);
				wnd = GetWndMgr().getWindow(tempText);
				if (wnd)
				{
					char szNum[32] = "";
					if (cacheGoodsInfo->treadeType == PlayerShop::TT_SINGLE)
					{
						if (orderGoods->HasEffect(GAP_MAXOVERLAP))
						{
							DWORD dwTotalNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
							if (cacheGoodsInfo->groupNum == 0)
							{
								wnd->setText("");
							}else if (cacheGoodsInfo->groupNum*cacheGoodsInfo->oneGroupNum<=dwTotalNum&&cacheGoodsInfo->oneGroupNum!=0)
							{
								sprintf(szNum,"%d",cacheGoodsInfo->groupNum);
								wnd->setText(ToCEGUIString(szNum));
							}else
							{
								cacheGoodsInfo->groupNum = dwTotalNum;
								sprintf(szNum,"%d",cacheGoodsInfo->groupNum);
								wnd->setText(ToCEGUIString(szNum));
							}
						}else
						{
							cacheGoodsInfo->groupNum = 1;
							wnd->setText("1");
						}
					}
					else if (cacheGoodsInfo->treadeType == PlayerShop::TT_GROUP)
					{
						if (orderGoods->HasEffect(GAP_MAXOVERLAP))
						{
							DWORD dwTotalNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
							if (cacheGoodsInfo->groupNum == 0)
							{
								wnd->setText("");
							}else if (cacheGoodsInfo->groupNum*cacheGoodsInfo->oneGroupNum<=dwTotalNum&&cacheGoodsInfo->oneGroupNum!=0)
							{
								sprintf(szNum,"%d",cacheGoodsInfo->groupNum);
								wnd->setText(ToCEGUIString(szNum));
							}else
							{
								cacheGoodsInfo->groupNum = dwTotalNum;
								sprintf(szNum,"%d",cacheGoodsInfo->groupNum);
								wnd->setText(ToCEGUIString(szNum));
							}
						}
					}
				}

				// ��������
				sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/HaveNum", i);
				wnd = GetWndMgr().getWindow(tempText);
				if (wnd)
				{
					// ��������д���Ʒ�����Ƕ���
					DWORD dwCurTotalNum = 1;
					if (orderGoods->HasEffect(GAP_MAXOVERLAP))
					{
						dwCurTotalNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
					}
					char szCurTotalNum[32] = "";
					sprintf(szCurTotalNum,AppFrame::GetText("Shop_20")  //����������%d
                        ,dwCurTotalNum);
					wnd->setText(ToCEGUIString(szCurTotalNum));
				}
			}

			// �������
			wnd = GetWndMgr().getWindow("PlayerShopOrder/TabControl/Tab 2/OneGroupNum");
			if (wnd)
			{
				char szNum[32] = "";
				if (cacheGoodsInfo->treadeType == PlayerShop::TT_GROUP)
				{
					DWORD dwTotalNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
					if (cacheGoodsInfo->oneGroupNum == 0)
					{
						wnd->setText("");
					}else if(cacheGoodsInfo->groupNum*cacheGoodsInfo->oneGroupNum<=dwTotalNum&&cacheGoodsInfo->groupNum!=0)
					{
						sprintf(szNum,"%d",cacheGoodsInfo->oneGroupNum);
						wnd->setText(szNum);
					}else
					{
						cacheGoodsInfo->oneGroupNum = 1;
						wnd->setText("1");
					}

				}
			}
		}
	}
	return true;
}

bool OnPlayerShopOrderPageClose(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	ClosePlayerShopOrderPage();

	return true;
}

bool OnPlayerShopOrderOk(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	return GetPlayerShop().OnShopOrderOK();
}

bool OnPlayerShopOrderClose(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	ClosePlayerShopOrderPage();
	FireUIEvent("PlayerShopOrder", "Closed");

	return true;
}

void ClosePlayerShopOrderPage(void)
{
	CEGUI::Window* wnd;
	char tempText[256];

	for (int i=1; i<=PlayerShop::TT_NUM; ++i)
	{
		sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/Icon", i);
		wnd = GetWndMgr().getWindow(tempText);
		if (wnd)
			wnd->setProperty("Image","");

		sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/Name", i);
		wnd = GetWndMgr().getWindow(tempText);
		if (wnd)
			wnd->setText("");

		sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/Price", i);
		wnd = GetWndMgr().getWindow(tempText);
		if (wnd)
			wnd->setText("");

		sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/SaleNum", i);
		wnd = GetWndMgr().getWindow(tempText);
		if (wnd)
			wnd->setText("");

		sprintf(tempText, "PlayerShopOrder/TabControl/Tab %d/HaveNum", i);
		wnd = GetWndMgr().getWindow(tempText);
		if (wnd)
			wnd->setText("");
	}

	wnd = GetWndMgr().getWindow("PlayerShopOrder/TabControl/Tab 2/OneGroupNum");
	if (wnd)
		wnd->setText("");

	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		if (orderGoods->IsLocked())
		{
			orderGoods->UnLock();
		}
		orderGoods->SetHaveShadowState(false);
		// @todo ����
		//GetGame()->GetCGuiEx()->GetItemPageEx()->UpdateGoodsShowByCGoods(orderGoods);
	}
	GetPlayerShop().SetOrderGoods(NULL);
}

bool OnPlayerShopOrderGoodsPriceChange(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::String price = wnd->getText();
	char str[32] = "";

	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		PlayerShop::stCacheGoodsInfo* cacheGoodsInfo = GetPlayerShop().GetCacheGoodsInfo(orderGoods->GetIndex());
		// �����Ʒ��������Ƿ�Ϸ�
		ulong inputNum = (ulong)atoi(price.c_str());
		ulong coinMaxNum = 2000000000;
		// ����ֵ�������ֵ
		if(inputNum >= coinMaxNum)
		{
			sprintf(str,"%d",coinMaxNum);
			wnd->setText(str);
			cacheGoodsInfo->price = coinMaxNum;
			return true;
		}
		// ����ֵС��0
		else if(inputNum <= 0)
		{
			wnd->setText("1");
			cacheGoodsInfo->price = 0;
			return true;
		}
		else
		{
			sprintf(str,"%d",inputNum);
			wnd->setText(str);
			cacheGoodsInfo->price = inputNum;
			return true;
		}
	}

	return false;
}

bool OnPlayerShopOrderGoodsSaleNumChange(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::String saleNum = wnd->getText();
	char str[32] = "";

	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		PlayerShop::stCacheGoodsInfo* cacheGoodsInfo = GetPlayerShop().GetCacheGoodsInfo(orderGoods->GetIndex());
		// �����Ʒ��������Ƿ�Ϸ�
		ulong inputNum = (ulong)atoi(saleNum.c_str());
		// ��������д���Ʒ�����Ƕ���
		ulong haveGoodsNum = 1;
		if (orderGoods->HasEffect(GAP_MAXOVERLAP))
		{
			haveGoodsNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
		}
		if (cacheGoodsInfo->treadeType==PlayerShop::TT_GROUP)
		{
			if (cacheGoodsInfo->oneGroupNum>0)
			{
				haveGoodsNum/=cacheGoodsInfo->oneGroupNum;
			}
		}
		// ����ֵ�������ֵ
		if(inputNum >= haveGoodsNum)
		{
			sprintf(str,"%d",haveGoodsNum);
			wnd->setText(str);
			cacheGoodsInfo->groupNum = haveGoodsNum;
			return true;
		}
		// ����ֵС��0
		else if(inputNum <= 0)
		{
			wnd->setText("1");
			cacheGoodsInfo->groupNum = 1;
			return true;
		}
		else
		{
			sprintf(str,"%d",inputNum);
			wnd->setText(str);
			cacheGoodsInfo->groupNum = inputNum;
			return true;
		}
	}
	return false;
}

bool OnPlayerShopOrderOneGroupNumChange(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::String oneGroupNum = wnd->getText();
	char str[32] = "";

	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		PlayerShop::stCacheGoodsInfo* cacheGoodsInfo = GetPlayerShop().GetCacheGoodsInfo(orderGoods->GetIndex());
		if (cacheGoodsInfo->treadeType==PlayerShop::TT_GROUP)
		{
			// �����Ʒ��������Ƿ�Ϸ�
			ulong inputNum = (ulong)atoi(oneGroupNum.c_str());
			// ��������д���Ʒ�����Ƕ���
			DWORD goodsNum = 1;
			if (orderGoods->HasEffect(GAP_MAXOVERLAP))
			{
				goodsNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
			}
			if (cacheGoodsInfo->groupNum>0)
			{
				goodsNum/=cacheGoodsInfo->groupNum;
			}
			// ����ֵ�������ֵ
			if(inputNum >= goodsNum)
			{
				sprintf(str,"%d",goodsNum);
				wnd->setText(str);
				cacheGoodsInfo->oneGroupNum = goodsNum;
				return true;
			}
			// ����ֵС��0
			else if(inputNum <= 0)
			{
				wnd->setText("1");
				cacheGoodsInfo->oneGroupNum = 1;
				return true;
			}
			else
			{
				sprintf(str,"%d",inputNum);
				wnd->setText(str);
				cacheGoodsInfo->oneGroupNum = inputNum;
				return true;
			}
		}
	}
	return false;
}

bool OnPlayerShopOrderGoodsSaleNumMaxClicked(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::Window* saleNumItem;
	char str[32] = "";
	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		PlayerShop::stCacheGoodsInfo* cacheGoodsInfo = GetPlayerShop().GetCacheGoodsInfo(orderGoods->GetIndex());
		// ��������д���Ʒ�����Ƕ���
		ulong goodsNum = 1;
		if (orderGoods->HasEffect(GAP_MAXOVERLAP))
		{
			goodsNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
		}
		if (cacheGoodsInfo->treadeType==PlayerShop::TT_SINGLE)
		{
			saleNumItem = GetWndMgr().getWindow("PlayerShopOrder/TabControl/Tab 1/SaleNum");
		}
		else if (cacheGoodsInfo->treadeType==PlayerShop::TT_GROUP)
		{
			if (cacheGoodsInfo->groupNum>0)
			{
				goodsNum/=cacheGoodsInfo->groupNum;
			}
			saleNumItem =GetWndMgr().getWindow("PlayerShopOrder/TabControl/Tab 2/SaleNum");
		}

		if (saleNumItem!=NULL)
		{
			sprintf(str,"%d",goodsNum);
			saleNumItem->setText(str);
			cacheGoodsInfo->groupNum = goodsNum;
		}
	}
	return true;
}

bool OnPlayerShopOrderGoodsOneGroupMaxNumClicked(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	char str[32] = "";
	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		PlayerShop::stCacheGoodsInfo* cacheGoodsInfo = GetPlayerShop().GetCacheGoodsInfo(orderGoods->GetIndex());
		if (cacheGoodsInfo->treadeType==PlayerShop::TT_GROUP)
		{
			// ��������д���Ʒ�����Ƕ���
			ulong goodsNum = 1;
			if (orderGoods->HasEffect(GAP_MAXOVERLAP))
			{
				goodsNum = GetGame()->GetMainPlayer()->GetGoodsAmount(orderGoods->GetIndex());
			}
			
			CEGUI::Window* oneGroupNumItem = GetWndMgr().getWindow("PlayerShopOrder/TabControl/Tab 2/OneGroupNum");
			if (oneGroupNumItem!=NULL)
			{
				if (cacheGoodsInfo->groupNum>0)
				{
					goodsNum/=cacheGoodsInfo->groupNum;
				}
				sprintf(str,"%d",goodsNum);
				oneGroupNumItem->setText(str);
				cacheGoodsInfo->oneGroupNum = goodsNum;
			}
		}
	}
	return true;
}

bool OnPlayerShopOrderTabChanged(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	if(!wnd) return false;

	CEGUI::TabControl* tab = static_cast<CEGUI::TabControl*>(wnd);
	size_t tabIndex = tab->getSelectedTabIndex();
	CEGUI::Window* selectedWnd = tab->getTabContentsAtIndex(tabIndex);
	if (!selectedWnd) return false;

	CGoods* orderGoods = GetPlayerShop().GetOrderGoods();
	if (orderGoods!=NULL)
	{
		PlayerShop::stCacheGoodsInfo* cacheGoodsInfo = GetPlayerShop().GetCacheGoodsInfo(orderGoods->GetIndex());
		if (tabIndex == PlayerShop::TT_SINGLE)
		{
			cacheGoodsInfo->treadeType = PlayerShop::TT_SINGLE;
		}
		else if (tabIndex == PlayerShop::TT_GROUP)
		{
			cacheGoodsInfo->treadeType = PlayerShop::TT_GROUP;
		}
	}
	
	return true;
}