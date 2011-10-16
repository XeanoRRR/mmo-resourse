/**
* @filename:ShopChild.cpp
* @date: 2010/7/21
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: �̳�ҳ����Tab���̵����ͣ��µ�Ψһһ����Tab
*/

#include "stdafx.h"
#include "ShopCityDef.h"
#include "../UIDef.h"
#include "../../GameClient/ClientApp/ShopCity/ShopCitySys.h"

using namespace CEGUI;
extern Window* InitShopCityItemSet();
extern Window* GetWindow(const CEGUI::String& name);

bool OnShopCityChildTabContentSelChanged(const CEGUI::EventArgs& e);
void SetInitProShopCityChild(Window* pageWnd);
void SubEventShopCityChild(Window* pageWnd);

Window* InitShopCityChild()
{
	Window* wnd = LoadUI(SHOPCITY_CHILD_PAGE_NAME);
	SetInitProShopCityChild(wnd);
	SubEventShopCityChild(wnd);
	return wnd;
}

void SetInitProShopCityChild(Window* pageWnd)
{
	TabControl* tbc = WTabControl(pageWnd);
	//Ĭ�ϵ�һ���ɼ�
	tbc->setSelectedTabAtIndex(0);
	Window* ctt1 = tbc->getTabContentsAtIndex(tbc->getSelectedTabIndex());
	if(ctt1)
	{
		ctt1->setVisible(true);
		//��ShopCityItemSet �ӵ�������
		ctt1->addChildWindow(InitShopCityItemSet());
	}
}

void SubEventShopCityChild(Window* pageWnd)
{
	TabControl* tbc = WTabControl(pageWnd);
	//
	tbc->subscribeEvent(TabControl::EventSelectionChanged,CEGUI::SubscriberSlot(OnShopCityChildTabContentSelChanged));
}

bool OnShopCityChildTabContentSelChanged(const CEGUI::EventArgs& e)
{
	TabControl* tbc = WTabControl(WEArgs(e).window);
	if(tbc)
	{
		CEGUI::Window* tbcontent = tbc->getTabContentsAtIndex(tbc->getSelectedTabIndex());
		if(tbcontent)
		{
			//��ΨһShopCityItemSet�ӵ���ǰѡ������
			CEGUI::Window* itemSet = GetWindow(SHOPCITY_ITEMSET_PAGE_NAME);
			tbcontent->addChildWindow(itemSet);
			//����ItemSet����ʾ
			FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE);
			//����������(ɸѡ)�˵�
			FireUIEvent(SHOPCITY_SEARCH_RIGHTWND_NAME,SHOPCITY_SEARCHRIGHT_EVENT_MENUUPDATE_NAME);
			//����������(����)�˵�
			FireUIEvent(SHOPCITY_SEARCH_LEFTWND_NAME,SHOPCITY_SEARCHLEFT_EVENT_MENUUPDATE_NAME);
		}
	}
	return true;
}
