/**
* @filename:ShopCity.cpp
* @date: 2010/7/21
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: �̳ǵ�UI����
*/
#include "stdafx.h"
#include "ShopCityDef.h"
#include "../UIDef.h"
#include "../../GameClient/ClientApp/ShopCity/ShopCitySys.h"
//GoodsList.h ������GoodsExDef.h ��goodsExDef.h�õ���CGUI�࣬�����Ȱ���һ��GUID.h�ļ�
#include "../../../Public/Module/GUID.h"
#include "../../GameClient/ClientApp/Goods/GoodsList.h"

extern CEGUI::Window* InitShopCityChild();
extern CEGUI::Window* GetWindow(const CEGUI::String& name);
extern SCGData::eSCType GetShopCityTypeByTabContentSelIndex();
extern SCGData::eSType  GetShopTypeByTabContentSelIndex();
void SetInitProShopCity(CEGUI::Window* pageWnd);
void SubEventShopCity(CEGUI::Window* pageWnd);

///ShopCityTablContentSelChanged
bool OnShopCityContentSelChanged(const CEGUI::EventArgs& e);
///�����������ȯ��Edixbox��ʾ����
bool OnClearMoneyEdboxes(const CEGUI::EventArgs& e);
///������������ȯ��UI����
bool OnShopCityUpdateMoneyEdboxes(const CEGUI::EventArgs& e);

///����
bool OnShopCitySearch(const CEGUI::EventArgs& e);

///������ť��ߵ�ccboxItemchange�¼�
bool OnSearchLeftccboxItemChange(const CEGUI::EventArgs& e);

///����������(����)�˵�
bool OnSearchLeftMenuUpdate(const CEGUI::EventArgs& e);

///���ذ�ť�ұߵ�ccboxItemChange�¼�
bool OnSearchRightccboxItemChange(const CEGUI::EventArgs& e);

///����������(ɸѡ)�˵�
bool OnSearchRightMenuUpdate(const CEGUI::EventArgs& e);

///������������¼��ʾ
bool OnUpdateLatestBuy(const CEGUI::EventArgs& e);

///�����Ƽ�
bool OnUpdateTwitter(const CEGUI::EventArgs& e);
///˫���Ƽ��б��򿪹���ҳ��
bool OnShopCityTwitterMouseDoubleClicked(const CEGUI::EventArgs& e);
///��ֵ
bool OnShopCitySaveMoney(const CEGUI::EventArgs& e);

///ɾ���Դ�����Ʒ
bool OnShopCityTryOnRoomDel(const CEGUI::EventArgs& e);

///����ShopCity����
bool OnShopCityUpdate(const CEGUI::EventArgs& e);
///�ر�shopCity����
bool OnShopCityClosed(const CEGUI::EventArgs& e);

CEGUI::Window* InitShopCity()
{
	CEGUI::Window* wnd = LoadUI(SHOPCITY_PAGE_NAME);
	SubEventShopCity(wnd);
	//��ע���¼��������ó�ʼ��״̬����Ϊ��ʼ��ʱ�������¼�
	SetInitProShopCity(wnd);
	GetInst(ShopCityMsgMgr).SetPageState(true);
	return wnd;
}

void SetInitProShopCity(CEGUI::Window* pageWnd)
{
	CEGUI::TabControl* tbs = WTabControl(pageWnd->getChildRecursive(SHOPCITY_TAB_NAME));
	if(tbs)
	{
		//Ĭ���õ�һ��TabContent�ɼ�
		tbs->setSelectedTabAtIndex(0);
		CEGUI::Window* tbs1 = tbs->getTabContentsAtIndex(tbs->getSelectedTabIndex());
#ifdef _DEBUG
		const char* name = tbs1->getName().c_str();
		OutputDebugStr(name);
		OutputDebugStr("\n");
#endif
		tbs1->setVisible(true);
		//��ΨһShopCityChild��ӵ�����
		CEGUI::TabControl* child = WTabControl(InitShopCityChild());
		//Ĭ���õ�һtabContent�ɼ�
		child->setSelectedTabAtIndex(0);
		tbs1->addChildWindow(child);
	}
	//���moneyEdboxes
	FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_CLEARMONEYEDBOXES);
	//moneyEdboxes��������
	CEGUI::Editbox* edbox = NULL;
	//����
	edbox = WEditBox(pageWnd->getChildRecursive(SHOPCITY_XINGZUAN_EDBOX_NAME));
	if(edbox)
	{
		//ֻ����������
		edbox->setValidationString("[0-9]*");
		//���ֿ�
		edbox->setMaxTextLength(9);
		//ֻ��
		edbox->setReadOnly(true);
	}
	//��ȯ
	edbox = WEditBox(pageWnd->getChildRecursive(SHOPCITY_DIANJUAN_EDBOX_NAME));
	if(edbox)
	{
		//ֻ����������
		edbox->setValidationString("[0-9]*");
		//���ֿ�
		edbox->setMaxTextLength(9);
		//ֻ��
		edbox->setReadOnly(true);
	}
	//λ��
	edbox = WEditBox(pageWnd->getChildRecursive(SHOPCITY_WEIMIAN_EDBOX_NAME));
	if(edbox)
	{
		//ֻ����������
		edbox->setValidationString("[0-9]*");
		//���ֿ�
		edbox->setMaxTextLength(9);
		//ֻ��
		edbox->setReadOnly(true);
	}
	//�����ؿ�
	CEGUI::Combobox* cbboxL = WComboBox(pageWnd->getChildRecursive(SHOPCITY_SEARCH_LEFTWND_NAME));
	if(cbboxL)
	{
		cbboxL->setReadOnly(true);
	}
	//�����ؿ�
	CEGUI::Combobox* cbboxR = WComboBox(pageWnd->getChildRecursive(SHOPCITY_SEARCH_RIGHTWND_NAME));
	if(cbboxR)
	{
		cbboxR->setReadOnly(true);
	}
}

void SubEventShopCity(CEGUI::Window* pageWnd)
{
	//ɾ����װ����Ʒ
	CEGUI::Window* flDel = pageWnd->getChildRecursive(SHOPCITY_FILLTERROOM_NAME);
	if(flDel)
	{
		flDel->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(OnShopCityTryOnRoomDel));
	}
	//��ֵ
	CEGUI::Window* saveMoney = pageWnd->getChildRecursive(SHOPCITY_SAVEMONEY_NAME);
	if(saveMoney)
	{
		saveMoney->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(OnShopCitySaveMoney));
	}
	//��������¼
	CEGUI::Window* latesBuy = pageWnd->getChildRecursive(SHOPCITY_LATESTBUY_NAME);
	if(latesBuy)
		latesBuy->subscribeEvent(SHOPCITY_PAGE_EVENT_UPDATE_LATESTBUY,CEGUI::SubscriberSlot(OnUpdateLatestBuy));
	//�������money��ʾ
	pageWnd->subscribeEvent(SHOPCITY_PAGE_EVENT_CLEARMONEYEDBOXES,CEGUI::SubscriberSlot(OnClearMoneyEdboxes));
	//�������ָ���money��ʾ
	pageWnd->subscribeEvent(SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES,CEGUI::SubscriberSlot(OnShopCityUpdateMoneyEdboxes));
	//�Ƽ�
	CEGUI::Window* twitter = pageWnd->getChildRecursive(SHOPCITY_TWITTER_NAME);
	if(twitter)
	{
		twitter->subscribeEvent(SHOPCITY_TWITTER_EVENT_NAME,CEGUI::SubscriberSlot(OnUpdateTwitter));
	}
	//�Ƽ�˫��
	CEGUI::Listbox* ltb = WListBox(GetWindow(SHOPCITY_TWITTER_CHILDLISTBOX_NAME));
	ltb->subscribeEvent(CEGUI::Listbox::EventMouseDoubleClick,CEGUI::SubscriberSlot(OnShopCityTwitterMouseDoubleClicked));
	CEGUI::TabControl* tbs = WTabControl(pageWnd->getChildRecursive(SHOPCITY_TAB_NAME));
	{
		if (tbs)
		{
			tbs->subscribeEvent(CEGUI::TabControl::EventSelectionChanged,CEGUI::SubscriberSlot(OnShopCityContentSelChanged));
		}
	}
	//�����¼�
	CEGUI::Window* search = pageWnd->getChildRecursive(SHOPCITY_SEARCH_NAME);
	if(search){
		search->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(OnShopCitySearch));
	}
	//������ߵ�ccboxItemchange�¼�
	CEGUI::Window* searchL = pageWnd->getChildRecursive(SHOPCITY_SEARCH_LEFTWND_NAME);
	if (searchL)
	{
		searchL->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::SubscriberSlot(OnSearchLeftccboxItemChange));
		//������ť��ߵĲ˵�(����)�����¼�
		searchL->subscribeEvent(SHOPCITY_SEARCHLEFT_EVENT_MENUUPDATE_NAME,CEGUI::SubscriberSlot(OnSearchLeftMenuUpdate));
	}
	//�����ұߵ�ccboxItemchange�¼�
	CEGUI::Window* searchR = pageWnd->getChildRecursive(SHOPCITY_SEARCH_RIGHTWND_NAME);
	if(searchR)
	{
		searchR->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::SubscriberSlot(OnSearchRightccboxItemChange));
		//������ť�ұߵ�(ɸѡ)�˵������¼�
		searchR->subscribeEvent(SHOPCITY_SEARCHRIGHT_EVENT_MENUUPDATE_NAME,CEGUI::SubscriberSlot(OnSearchRightMenuUpdate));
	}

	//��ʱ�򣬾͸��½���UI��ʾ
	pageWnd->subscribeEvent(EVENT_OPEN,CEGUI::SubscriberSlot(OnShopCityUpdate));
	//�ر�ʱ��
	pageWnd->subscribeEvent(EVENT_CLOSE,CEGUI::SubscriberSlot(OnShopCityClosed));
	pageWnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::SubscriberSlot(OnShopCityClosed));
}

bool OnShopCityContentSelChanged(const CEGUI::EventArgs& e)
{
	CEGUI::TabControl* tbs = WTabControl(WEArgs(e).window);
	if(tbs)
	{
		CEGUI::Window* tbcontent =  tbs->getTabContentsAtIndex(tbs->getSelectedTabIndex());
		if(tbcontent)
		{
			tbcontent->addChildWindow(GetWindow(SHOPCITY_CHILD_PAGE_NAME));//��ΨһShopCityChild�ӵ���ǰѡ��tbcontent��
			//����ItemSet����ʾ
			FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE);
			//�����������
			FireUIEvent(SHOPCITY_LATESTBUY_NAME,SHOPCITY_PAGE_EVENT_UPDATE_LATESTBUY);
			//�����Ƽ�
			FireUIEvent(SHOPCITY_TWITTER_NAME,SHOPCITY_TWITTER_EVENT_NAME);
			//����������(����)�˵�
			FireUIEvent(SHOPCITY_SEARCH_LEFTWND_NAME,SHOPCITY_SEARCHLEFT_EVENT_MENUUPDATE_NAME);
			//����������(ɸѡ)�˵�
			FireUIEvent(SHOPCITY_SEARCH_RIGHTWND_NAME,SHOPCITY_SEARCHRIGHT_EVENT_MENUUPDATE_NAME);
			//���ø���Դ����
			ShopCityMsgMgr& msgMgr = GetInst(ShopCityMsgMgr);
			msgMgr.SetStateUpdateUIByType(0);//��ѡ�е��̳��̵�����������
		}
	}
	return true;
}

bool OnClearMoneyEdboxes(const CEGUI::EventArgs& e)
{
	CEGUI::Window* pageWnd = WEArgs(e).window;
	//����
	CEGUI::Window* temp = pageWnd->getChildRecursive(SHOPCITY_XINGZUAN_EDBOX_NAME);
	if(temp)
		temp->setText("");
	//��ȯ
	temp = pageWnd->getChildRecursive(SHOPCITY_DIANJUAN_EDBOX_NAME);
	if(temp)
		temp->setText("");
	//λ��
	temp = pageWnd->getChildRecursive(SHOPCITY_WEIMIAN_EDBOX_NAME);
	if(temp)
		temp->setText("");
	return true;
}
bool OnShopCitySearch(const CEGUI::EventArgs& e)
{
	//��ȡ�����ı�
	CEGUI::Combobox* guide = WComboBox(GetWindow(SHOPCITY_SEARCH_LEFTWND_NAME));
	if(guide && guide->getEditbox()->getText()=="")
	{
		return true;//�ı�Ϊ��ֱ�ӷ���
	}
	ShopCityMsgMgr& msgMgr = GetInst(ShopCityMsgMgr);
	msgMgr.SetStateUpdateUIByType(2);//��Ϊ����ԴΪ����
	//���ݵ������ݸ�����ʾ��Ŀ
	FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE_BY_GUIDE);
	return true;
}
bool OnSearchLeftccboxItemChange(const CEGUI::EventArgs& e)
{
	return true;
}

bool OnSearchRightccboxItemChange(const CEGUI::EventArgs& e)
{
	//
	ShopCityMsgMgr& msgMgr = GetInst(ShopCityMsgMgr);
	//��Ϊ����ɸѡ���ݸ�����ʾ��Ŀ
	msgMgr.SetStateUpdateUIByType(1);
	//����ɸѡ���ݸ���Itemset����ʾ
	FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE_BY_FILTER);
	return true;
}

bool OnUpdateLatestBuy(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	//�Ȱ�ͼƬ��� ,Ĭ��Ϊʮ����Ŀ
	char name[256] = "";
	for(uint i = 0 ;i < 10 ; ++i)
	{
		sprintf(name,SHOPCITY_LATESTBUY_ITME_NAME_D,i);
		CEGUI::Window* temp = wnd->getChild(name);
		if(temp)
		{
			temp->setProperty("Image","");
			OutputDebugStr(temp->getName().c_str());
			OutputDebugStr("\n");
		}
	}

	//�����������̳�����
	SCGData::eSCType eCityType = GetShopCityTypeByTabContentSelIndex();
	SCGData* dt = GetInst(ShopCityMsgMgr).GetShopCityGoodsData();
	SCGData::MapSBT10& personal = dt->GetSelfBuyTop10();
	SCGData::VecGDPTA perDTA = personal[eCityType];

	size_t count = perDTA.size();
	for(size_t i = 0 ; i < count ; ++i)
	{
		char name[256] = "";
		sprintf(name,SHOPCITY_LATESTBUY_ITME_NAME_D,i);
		CEGUI::Window* temp = wnd->getChild(name);
		if(temp)
		{
			CGoodsList::tagGoods2* tg2 = CGoodsList::GetProperty(perDTA[i].index);
			if(tg2)
			{
				char imagesetname[256];
				sprintf(imagesetname,GOODS_PREFIXID,tg2->BaseProperty.dwIconId);
				CEGUI::String imagename = CEGUI::PropertyHelper::intToString(tg2->BaseProperty.dwIconId)+".jpg";
				SetBackGroundImage(WGUISheet(temp),imagesetname,GOODS_ICON_PATH,imagename.c_str());
			}
		}
	}
	return true;
}

//��Ʒ�Ƽ�
bool OnUpdateTwitter(const CEGUI::EventArgs& e)
{
	CEGUI::Window* twitter = WEArgs(e).window;
	CEGUI::Listbox*  lb = WListBox(twitter->getChildRecursive(SHOPCITY_TWITTER_CHILDLISTBOX_NAME));
#ifdef _DEBUG
	OutputDebugStr(lb->getName().c_str());
	OutputDebugStr("\n");
	OutputDebugStr(twitter->getChildAtIdx(0)->getName().c_str());
	OutputDebugStr("n");
#endif
	//���
	lb->resetList();

	//�����������̳�����
	SCGData::eSCType eCityType = GetShopCityTypeByTabContentSelIndex();
	SCGData* dt = GetInst(ShopCityMsgMgr).GetShopCityGoodsData();
	//��Ʒ�Ƽ���ʾ
	SCGData::MapNewestA& resdta = dt->GetNewestVec();
	SCGData::VecGDPTA& vecDTA = resdta[eCityType];
	for(uint i = 0 ; i < vecDTA.size() ; ++i)
	{
		CGoodsList::tagGoods2* ptg2 = CGoodsList::GetProperty(vecDTA[i].index);
		if(ptg2)
		{
			string str  = ptg2->BaseProperty.strName.c_str();
			//CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(str.c_str(),vecDTA[i].index);//��������Item ID
			CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(ToCEGUIString(str.c_str()),vecDTA[i].index);//��������Item ID
			lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			lb->addItem(lti);
		}
	}
	return true;
}

//��ֵ
bool OnShopCitySaveMoney(const CEGUI::EventArgs& e)
{
	return true;
}

//ɾ���Դ�����Ʒ
bool OnShopCityTryOnRoomDel(const CEGUI::EventArgs& e)
{
	return true;
}

bool OnShopCityUpdate(const CEGUI::EventArgs& e)
{
	//����Itemset
	FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE);
	//�����Ƽ�
	FireUIEvent(SHOPCITY_TWITTER_NAME,SHOPCITY_TWITTER_EVENT_NAME);
	//�����������
	FireUIEvent(SHOPCITY_LATESTBUY_NAME,SHOPCITY_PAGE_EVENT_UPDATE_LATESTBUY);
	//����������(ɸѡ)�˵�
	FireUIEvent(SHOPCITY_SEARCH_RIGHTWND_NAME,SHOPCITY_SEARCHRIGHT_EVENT_MENUUPDATE_NAME);
	//����������(����)�˵�
	FireUIEvent(SHOPCITY_SEARCH_LEFTWND_NAME,SHOPCITY_SEARCHLEFT_EVENT_MENUUPDATE_NAME);
	//��ո���ȯ��UI
	FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_CLEARMONEYEDBOXES);
	//Ȼ����¸���ȯ��UI
	FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);
	return true;
}
//������������ȯ��UI����
bool OnShopCityUpdateMoneyEdboxes(const CEGUI::EventArgs& e)
{
	ShopCityMsgMgr& mgr = GetInst(ShopCityMsgMgr);
	CEGUI::Window* pgWnd = WEArgs(e).window;
	CEGUI::Window* temp = pgWnd->getChildRecursive(SHOPCITY_XINGZUAN_EDBOX_NAME);
	if(temp)
	{
		temp->setText(CEGUI::PropertyHelper::intToString(mgr.GetPlayerXinZuan()));
	}
	temp = pgWnd->getChildRecursive(SHOPCITY_WEIMIAN_EDBOX_NAME);
	if(temp)
		temp->setText(CEGUI::PropertyHelper::intToString(mgr.GetPlayerWeiMian()));
	temp = pgWnd->getChildRecursive(SHOPCITY_DIANJUAN_EDBOX_NAME);
	if(temp)
		temp->setText(CEGUI::PropertyHelper::intToString(mgr.GetPlayerDianQuan()));
	return true;
}

//����������(����)�˵�
bool OnSearchLeftMenuUpdate(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* cbbox = WComboBox(WEArgs(e).window);
	cbbox->clearAllSelections();
	cbbox->resetList();
	cbbox->getEditbox()->setText("");
	//�����������̳�����
	SCGData::eSCType eCityType = GetShopCityTypeByTabContentSelIndex();
	SCGData* dt = GetInst(ShopCityMsgMgr).GetShopCityGoodsData();
	SCGData::MapGuideDataA& mapGuide = dt->GetGuideList();
	//����������ȡ��������
	SCGData::MapStrGGDTPA& mapGuideDTA = mapGuide[eCityType];
	CEGUI::Combobox* cbboxRight = WComboBox(GetWndMgr().getWindow(SHOPCITY_SEARCH_RIGHTWND_NAME));
	if(cbboxRight)
	{
		CEGUI::ListboxItem* lbi = cbboxRight->getSelectedItem();
		size_t idx = 0;
		if(lbi)
			idx = cbboxRight->getItemIndex(lbi);
		SCGData::MapStrGGDTPA::iterator iter = mapGuideDTA.begin();
		for(; iter != mapGuideDTA.end() ; ++iter)
		{
			//��ӵ����˵�
			string menuStr = iter->first;
			//CEGUI::ListboxItem* lbi = new CEGUI::ListboxTextItem(menuStr.c_str());
			CEGUI::ListboxItem* lbi = new CEGUI::ListboxTextItem(ToCEGUIString(menuStr.c_str()));
			lbi->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			if(iter == mapGuideDTA.begin())//Ĭ���õ�һ��Ϊѡ��
				lbi->setSelected(true);
			cbbox->addItem(lbi);
		}
	}
	return true;
}

//����������(ɸѡ)�˵�
bool OnSearchRightMenuUpdate(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* cbbox = WComboBox(WEArgs(e).window);
	cbbox->clearAllSelections();
	cbbox->resetList();
	cbbox->getEditbox()->setText("");

	//�����������̳�����
	SCGData::eSCType eShopCityType = GetShopCityTypeByTabContentSelIndex();
	//�����������̵����ͣ�tabControl������0������Ӧ������Ʒ
	SCGData::eSType shoptype = GetShopTypeByTabContentSelIndex();
	if(shoptype == SCGData::TABTYPE_HOT)//����û��ɸѡ��
		return true;
	//�����̳Ǻ��̵����ͻ�ȡɸѡ����
	SCGData* dt = GetInst(ShopCityMsgMgr).GetShopCityGoodsData();
	SCGData::MapFLDTA& mapSel = dt->GetFilterList();
	SCGData::MapUFLDTPA& mapUSel = mapSel[eShopCityType];
	SCGData::MapStrFilDTPA& mapStrSel = mapUSel[shoptype];
	SCGData::MapStrFilDTPA::iterator iter = mapStrSel.begin();
	for( ; iter != mapStrSel.end() ; ++iter)
	{
		//��ʼ��ɸѡ�˵�
		string str = (*iter).first;
		//CEGUI::ListboxItem* lbi = new CEGUI::ListboxTextItem(str.c_str());
		CEGUI::ListboxItem* lbi = new CEGUI::ListboxTextItem(ToCEGUIString(str.c_str()));
		lbi->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		if(iter == mapStrSel.begin() )//Ĭ���õ�һ��Ϊѡ����
			lbi->setSelected(true);
		cbbox->addItem(lbi);
	}
	return true;
}

bool OnShopCityClosed(const CEGUI::EventArgs& e)
{
	GetInst(ShopCityMsgMgr).Send_CloseShop();
	FireUIEvent(SHOPCITY_BUY_PAGE_NAME, EVENT_CLOSE);
	return true;
}

//˫���Ƽ��б��򿪹���ҳ��
bool OnShopCityTwitterMouseDoubleClicked(const CEGUI::EventArgs& e)
{
	CEGUI::Listbox* twitterList = WListBox(WEArgs(e).window);  
	CEGUI::ListboxItem* lbi = twitterList->getFirstSelectedItem();
	if(lbi)
	{
		uint index = lbi->getID();//��ȡ����������������Ʒ����
		CEGUI::Window* buyPage = GetWindow(SHOPCITY_BUY_PAGE_NAME);
		buyPage->setID(index);//�������ID����Ʒ��������
		//�򿪹������
		FireUIEvent(SHOPCITY_BUY_PAGE_NAME,EVENT_OPEN);
	}
	return true;
}