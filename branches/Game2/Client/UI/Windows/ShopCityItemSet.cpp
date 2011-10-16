/**
* @filename:ShopCityItemSet.cpp
* @date: 2010/7/22
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: �̳��̵������ĳ����Ʒ��Item����
*/
#include "stdafx.h"
#include "ShopCityDef.h"
#include "../UIDef.h"
#include "../../GameClient/ClientApp/ShopCity/ShopCitySys.h"
//GoodsList.h ������GoodsExDef.h ��goodsExDef.h�õ���CGUI�࣬�����Ȱ���һ��GUID.h�ļ�
#include "../../../Public/Module/GUID.h"
#include "../../GameClient/ClientApp/Goods/GoodsList.h"

using namespace CEGUI;
extern Window* GetWindow(const CEGUI::String& name);
void SetInitProShopCityItemSet(CEGUI::Window* pageWnd);
void SubEventShopCityItemSet(CEGUI::Window* pageWnd);

///���ݴ�������ݼ���ҳ��,�����¶�Ӧҳ���ݣ���������[starIdx,endIdx)
void CalcShopCityItemSetPage(SCGData::VecGD& vec,SCGData::eSCType& scType,
							 SCGData::eSType& sType,
							 int& startIdx,int& endIdx);
void UpdateShopCityItemSet_impl(SCGData::VecGD& vecGD,int startIdx,int endIdx,CEGUI::Window* pageWnd);

///�������Item��ʾ����
bool OnShopCityItemSetClear(const CEGUI::EventArgs& e);
///��ѡ�е��̳Ǻ��̵����͸���ItemSet��ʾ
bool OnShopCityItemSetUpdate(const CEGUI::EventArgs& e);
///����ѡ�е�ɸѡ�ı�����Itemset��ʾ
bool OnShopCityItemSetUpdateByFilter(const CEGUI::EventArgs& e);
///����ѡ�еĵ����ı�����Itemset��ʾ
bool OnShopCityItemSetUpdateByGuide(const CEGUI::EventArgs& e);

///��ҳ
bool OnShopCityItemSetPrePage(const CEGUI::EventArgs& e);
bool OnShopCityItemSetNextPage(const CEGUI::EventArgs& e);

///�����װ�¼�
bool OnShopCityItemSetTryOn(const CEGUI::EventArgs& e);
///��������¼�
bool OnShopCityItemSetBuy(const CEGUI::EventArgs& e);

///��ҳBtn�Ƿ�����
void ShopCityEnabledPrePageBtn(bool enabled);
void ShopCityEnabledNextPageBtn(bool enabled);

///���ݵ�ǰѡ�е�TabContent�����õ��̳�����
SCGData::eSCType GetShopCityTypeByTabContentSelIndex();
///���ݵ�ǰѡ�е�TabContent�����õ��̵�����
SCGData::eSType  GetShopTypeByTabContentSelIndex();

Window* InitShopCityItemSet()
{
	Window* wnd = LoadUI(SHOPCITY_ITEMSET_PAGE_NAME);
	SubEventShopCityItemSet(wnd);
	//��ע���¼��������ó�ʼ��״̬����Ϊ��ʼ��ʱ�������¼�
	SetInitProShopCityItemSet(wnd);
	return wnd;
}

void SetInitProShopCityItemSet(CEGUI::Window* pageWnd)
{
	//���Itemset
	FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_CLEAR);
}

void SubEventShopCityItemSet(CEGUI::Window* pageWnd)
{
	pageWnd->subscribeEvent(SHOPCITY_ITEMSET_EVENT_CLEAR,CEGUI::SubscriberSlot(OnShopCityItemSetClear));
	pageWnd->subscribeEvent(SHOPCITY_ITEMSET_EVENT_UPDATE,CEGUI::SubscriberSlot(OnShopCityItemSetUpdate));
	pageWnd->subscribeEvent(SHOPCITY_ITEMSET_EVENT_UPDATE_BY_FILTER,CEGUI::SubscriberSlot(OnShopCityItemSetUpdateByFilter));
	pageWnd->subscribeEvent(SHOPCITY_ITEMSET_EVENT_UPDATE_BY_GUIDE,CEGUI::SubscriberSlot(OnShopCityItemSetUpdateByGuide));
	CEGUI::Window* prepg = pageWnd->getChildRecursive(SHOPCITY_PREPAGE_NAME);
	if(prepg)
	{
		prepg->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(OnShopCityItemSetPrePage));
	}
	CEGUI::Window* nextpg = pageWnd->getChildRecursive(SHOPCITY_NEXTPAGE_NAME);
	if (nextpg)
	{
		nextpg->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(OnShopCityItemSetNextPage));
	}
	//��װ�͹����¼�
	char name[256] = "";
	CEGUI::Window* tryOn = NULL;
	CEGUI::Window* buy = NULL;
	for(int i = 0 ; i < SHOPCITY_ITEMSET_ITEM_COUNT ; ++i)
	{
		//��װ
		sprintf(name,SHOPCITY_ITEMSET_ITEM_TRYONBTN_D,i);
		tryOn = pageWnd->getChildRecursive(name);
		if(tryOn)
		{
			tryOn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(OnShopCityItemSetTryOn));
		}
		//����
		sprintf(name,SHOPCITY_ITEMSET_ITEM_BUYBTN_D,i);
		buy = pageWnd->getChildRecursive(name);
		if(buy)
		{
			buy->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(OnShopCityItemSetBuy));
		}
	}
}

bool OnShopCityItemSetClear(const CEGUI::EventArgs& e)
{
	//��������
	char wndName[256] = "";
	CEGUI::Window* pageWnd = WEArgs(e).window;
	CEGUI::Window* tempWnd = NULL;
	CEGUI::Window* item = NULL;
	for(int i = 0 ; i < SHOPCITY_ITEMSET_ITEM_COUNT ; ++i)
	{
		//��ձ���
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_TITLE_D,i);
		item = tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
		{
			tempWnd->setText("");
			//��Item���ɼ�
			item->setVisible(false);
		}
		//����ͼƬ
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_BGIMG_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setProperty("Image","");
		//����ͼƬ
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_HOTIMG_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setProperty("Image","");
		//�ۿ�ͼ
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_DISCONT_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setProperty("Image","");
		//moneyTypeͼ
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_MONEYTYPE_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setProperty("Image","");
		//�ּ�
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_PRICEEDBOX_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setText("");
		//hot
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_LABLEEDBOX_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setText("");
		//����
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_BUYBTN_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setID(0);//����ID
		sprintf(wndName,SHOPCITY_ITEMSET_ITEM_TRYONBTN_D,i);
		tempWnd = pageWnd->getChildRecursive(wndName);
		if(tempWnd)
			tempWnd->setID(0);//����ID
	}
	return true;
}

bool OnShopCityItemSetUpdate(const CEGUI::EventArgs& e)
{
	//���Itemset
	FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_CLEAR);
	CEGUI::Window* pageWnd = WEArgs(e).window;
	//�̳�����
	SCGData::eSCType shopcityType = GetShopCityTypeByTabContentSelIndex();
	//�̵�����
	SCGData::eSType shoptype = GetShopTypeByTabContentSelIndex();
	//��ȡ����
	ShopCityMsgMgr& scmm = GetInst(ShopCityMsgMgr);
	SCGData* dt = scmm.GetShopCityGoodsData();
	//size_t curPage = scmm.GetPageInfo(shopcityType,shoptype);//��ȡ��ǰҳ��¼[0,x)
	if(dt)
	{
		SCGData::VecGD vecGD;//�������
		if(shoptype != SCGData::TABTYPE_HOT)
			dt->GetVecGoodsData(shopcityType,shoptype,vecGD);
		else //�����������Ʒ
			dt->GetTop10InGoodsData(shopcityType,vecGD);
		int startIdx = 0;
		int endIdx = 0;
		CalcShopCityItemSetPage(vecGD,shopcityType,shoptype,startIdx,endIdx);
		UpdateShopCityItemSet_impl(vecGD,startIdx,endIdx,pageWnd);
	}
	return true;
}
bool OnShopCityItemSetUpdateByGuide(const CEGUI::EventArgs& e)
{
	//���Itemset
	FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_CLEAR);
	CEGUI::Window* pageWnd = WEArgs(e).window;
	//�̳�����
	SCGData::eSCType shopcityType = GetShopCityTypeByTabContentSelIndex();
	////�̵�����
	SCGData::eSType shoptype = GetShopTypeByTabContentSelIndex();
	//��ȡ����
	ShopCityMsgMgr& scmm = GetInst(ShopCityMsgMgr);
	SCGData* dt = scmm.GetShopCityGoodsData();
	//��ȡ��������
	string guidstr;
	CEGUI::Combobox* guide = WComboBox(GetWindow(SHOPCITY_SEARCH_LEFTWND_NAME));
	if(guide)
	{
		//guidstr = guide->getEditbox()->getText().c_str();
		guidstr = CEGUIStringToAnsiChar(guide->getEditbox()->getText());
	}
	if(dt)
	{
		SCGData::VecGD vecGD;//�����Ʒ����
		dt->GetGuideInGoodsData(shopcityType,guidstr,vecGD);
		int startIdx = 0;
		int endIdx = 0;
		CalcShopCityItemSetPage(vecGD,shopcityType,shoptype,startIdx,endIdx);
		UpdateShopCityItemSet_impl(vecGD,startIdx,endIdx,pageWnd);
	}
	return true;
}
bool OnShopCityItemSetUpdateByFilter(const CEGUI::EventArgs& e)
{
	//���Itemset
	FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_CLEAR);
	CEGUI::Window* pageWnd = WEArgs(e).window;
	//�̳�����
	SCGData::eSCType shopcityType = GetShopCityTypeByTabContentSelIndex();
	//�̵�����
	SCGData::eSType shoptype = GetShopTypeByTabContentSelIndex();
	//��ȡ����
	ShopCityMsgMgr& scmm = GetInst(ShopCityMsgMgr);
	SCGData* dt = scmm.GetShopCityGoodsData();
	//��ȡɸѡ�ı�
	string filterStr;
	CEGUI::Combobox* filter = WComboBox(GetWindow(SHOPCITY_SEARCH_RIGHTWND_NAME));
	if(filter)
	{
		//filterStr = filter->getEditbox()->getText().c_str();
		filterStr = CEGUIStringToAnsiChar(filter->getEditbox()->getText());
	}
	if(dt)
	{
		SCGData::VecGD vecGD;//�����Ʒ����
		dt->GetFilterInGoodsData(shopcityType,shoptype,filterStr,vecGD);
		int startIdx = 0;
		int endIdx = 0;
		CalcShopCityItemSetPage(vecGD,shopcityType,shoptype,startIdx,endIdx);
		UpdateShopCityItemSet_impl(vecGD,startIdx,endIdx,pageWnd);
	}
	return true;
}

bool OnShopCityItemSetPrePage(const CEGUI::EventArgs& e)
{
	//�̳�����
	SCGData::eSCType shopcityType = GetShopCityTypeByTabContentSelIndex();
	//�̵�����
	SCGData::eSType shoptype = GetShopTypeByTabContentSelIndex();
	ShopCityMsgMgr& scmgr = GetInst(ShopCityMsgMgr);
	int CurPG = scmgr.GetPageInfo(shopcityType,shoptype);
	scmgr.SetPageInfo(shopcityType,shoptype,CurPG-1);
	//�ɸ���Դ����������UI����
	switch(scmgr.GetStateUpdateUIByType())
	{
	case 0:
		//����ItemSet����ʾ
		FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE);
		break;
	case 1:
		FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE_BY_FILTER);
		break;
	case 2:
		FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE_BY_GUIDE);
		break;
	default:
		break;
	}	
	return true;
}

//
bool OnShopCityItemSetNextPage(const CEGUI::EventArgs& e)
{
	//�̳�����
	SCGData::eSCType shopcityType = GetShopCityTypeByTabContentSelIndex();
	//�̵�����
	SCGData::eSType shoptype = GetShopTypeByTabContentSelIndex();
	ShopCityMsgMgr& scmgr = GetInst(ShopCityMsgMgr);
	int CurPG = scmgr.GetPageInfo(shopcityType,shoptype);
	scmgr.SetPageInfo(shopcityType,shoptype,CurPG+1);
	//�ɸ���Դ����������UI����
	switch(scmgr.GetStateUpdateUIByType())
	{
	case 0:
		//����ItemSet����ʾ
		FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE);
		break;
	case 1:
		FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE_BY_FILTER);
		break;
	case 2:
		FireUIEvent(SHOPCITY_ITEMSET_PAGE_NAME,SHOPCITY_ITEMSET_EVENT_UPDATE_BY_GUIDE);
		break;
	default:
		break;
	}
	return true;
}

bool OnShopCityItemSetTryOn(const CEGUI::EventArgs& e)
{
	return true;
}

bool OnShopCityItemSetBuy(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = GetWindow(SHOPCITY_BUY_PAGE_NAME);
	//������ƷID
	CEGUI::Window* buy = WEArgs(e).window;
	wnd->setID(buy->getID());
	FireUIEvent(SHOPCITY_BUY_PAGE_NAME,EVENT_OPEN);
	return true;
}

void ShopCityEnabledPrePageBtn(bool enabled)
{
	CEGUI::Window* temp = GetWindow(SHOPCITY_PREPAGE_NAME);
	if(temp)
		temp->setEnabled(enabled);
}
void ShopCityEnabledNextPageBtn(bool enabled)
{
	CEGUI::Window* temp = GetWindow(SHOPCITY_NEXTPAGE_NAME);
	if(temp)
		temp->setEnabled(enabled);
}

SCGData::eSCType GetShopCityTypeByTabContentSelIndex()
{
	//�̳�����
	SCGData::eSCType shopcityType = SCGData::ST_XINGZUAN;
	CEGUI::TabControl* ShopCityTab  = WTabControl(GetWindow(SHOPCITY_TAB_NAME));
	if(ShopCityTab)
	{
		shopcityType = SCGData::eSCType(ShopCityTab->getSelectedTabIndex());//�̳�����ͬindex���� 
	}
	return shopcityType;
}
SCGData::eSType GetShopTypeByTabContentSelIndex()
{
	//�̵�����
	SCGData::eSType shoptype = SCGData::TABTYPE_HOT;
	CEGUI::TabControl* shopcityChildTab = WTabControl(GetWindow(SHOPCITY_CHILD_PAGE_NAME));
	if(shopcityChildTab)
	{
		shoptype = SCGData::eSType(shopcityChildTab->getSelectedTabIndex());
	}
	return shoptype;
}

void CalcShopCityItemSetPage(SCGData::VecGD& vecGD,SCGData::eSCType& scType, SCGData::eSType& sType, int& startIdx,int& endIdx)
{
	ShopCityMsgMgr& scmm = GetInst(ShopCityMsgMgr);
	SCGData* dt = scmm.GetShopCityGoodsData();
	int curPage = scmm.GetPageInfo(scType,sType);//��ȡ��ǰҳ��¼[0,x)
	//�������ҳ��
	int maxPage = (int)vecGD.size() / SHOPCITY_ITEMSET_ITEM_COUNT;//[0,x)
	if((int)vecGD.size() % SHOPCITY_ITEMSET_ITEM_COUNT == 0)//�ܹ�����
		maxPage--;
	//���㵱ǰҳ,������ҳ��¼
	if(curPage > maxPage)
	{
		curPage = maxPage;
		scmm.SetPageInfo(scType,sType,(int)curPage);
	}
	else if( curPage < 0)
	{
		curPage = 0;
		scmm.SetPageInfo(scType,sType,(int)curPage);
	}
	//������ʼ�ͽ�������
	if(maxPage == 0)//ֻ��һҳ
	{
		startIdx = 0;
		endIdx =  min((int)vecGD.size(),SHOPCITY_ITEMSET_ITEM_COUNT);//��������ȡ��С����
		//����ҳ��Ч
		ShopCityEnabledPrePageBtn(false);
		ShopCityEnabledNextPageBtn(false);
	}
	else if(maxPage > 0)//����1ҳ
	{
		startIdx =curPage * SHOPCITY_ITEMSET_ITEM_COUNT;//��ʼλ��
		if(curPage < maxPage && curPage != 0)//û�е����һҳ���Ҳ��ǵ�һҳ
		{
			endIdx = (curPage+1) * SHOPCITY_ITEMSET_ITEM_COUNT;
			ShopCityEnabledPrePageBtn(true);
			ShopCityEnabledNextPageBtn(true);
		}
		else if ( curPage == maxPage) //�������һҳ
		{
			ShopCityEnabledPrePageBtn(true);
			ShopCityEnabledNextPageBtn(false);//��ҳ��Ч
			endIdx = (int)vecGD.size();
		}
		else if ( curPage == 0 )//���˵�һҳ
		{
			endIdx = (curPage + 1 ) * SHOPCITY_ITEMSET_ITEM_COUNT;
			ShopCityEnabledPrePageBtn(false);//��ҳ��Ч
			ShopCityEnabledNextPageBtn(true);
		}
	}//�����������
}

void UpdateShopCityItemSet_impl(SCGData::VecGD& vecGD,int startIdx,int endIdx,CEGUI::Window* pageWnd)
{
	//����Item
	char wndName[256] = "";
	CEGUI::Window* tempWnd = NULL;
	for(int i = 0  ; startIdx < endIdx ; ++i,++startIdx)
	{

		SCGData::tagGD& gd = vecGD[startIdx];
		CGoodsList::tagGoods2* tgg2 = CGoodsList::GetProperty(gd.index);
		if(tgg2){
			//����(��Ʒ����)
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_TITLE_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
			{
				//tempWnd->setText(tgg2->BaseProperty.strName.c_str());
				SetText(tempWnd, tgg2->BaseProperty.strName.c_str());
				//��Item���ɼ�
				tempWnd->setVisible(true);
			}
			//����ͼƬ
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_BGIMG_D,i);
			CEGUI::GUISheet* tpGSheet = WGUISheet(pageWnd->getChildRecursive(wndName));
			if(tpGSheet)
			{
				char imagesetname[256];
				sprintf(imagesetname,GOODS_PREFIXID,tgg2->BaseProperty.dwIconId);
				CEGUI::String imagename = CEGUI::PropertyHelper::intToString(tgg2->BaseProperty.dwIconId)+".jpg";
				SetBackGroundImage(tpGSheet,imagesetname,GOODS_ICON_PATH,imagename.c_str(),false);
			}
			//����ͼƬ
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_HOTIMG_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
			{
				tempWnd->setProperty("Image","");
				tempWnd->setVisible(false);
			}
			//�ۿ�ͼ
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_DISCONT_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
			{
				tempWnd->setProperty("Image","");
				tempWnd->setVisible(false);
			}
			//moneyTypeͼ
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_MONEYTYPE_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
			{
				tempWnd->setProperty("Image","");
				tempWnd->setVisible(false);
			}
			////�ּ�
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_PRICEEDBOX_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
			{
				//tempWnd->setText(CEGUI::PropertyHelper::intToString(gd.price));
				SetText(tempWnd, CEGUI::PropertyHelper::intToString(gd.price).c_str());
			}
			//hot
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_LABLEEDBOX_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
				//tempWnd->setText("HOT");
				SetText(tempWnd, "HOT");
			//����
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_BUYBTN_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
				tempWnd->setID(gd.index);//id����ƷID����
			sprintf(wndName,SHOPCITY_ITEMSET_ITEM_TRYONBTN_D,i);
			tempWnd = pageWnd->getChildRecursive(wndName);
			if(tempWnd)
				tempWnd->setID(gd.index);//id����ƷID����
		}
	}
}