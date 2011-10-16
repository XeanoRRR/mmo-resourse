/**
* @filename:Auction.cpp
* @date: 2010/5/4
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: λ�����UI.
*/

#include "stdafx.h"
#include "../UIDef.h"
#include "../../GameClient/ClientApp/Auction/AuctionSys.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../GameClient/ClientApp/Other/GlobeSetup.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/Game.h"


///�����ѷ���
#define AGENT_EXTRACUST CGlobeSetup::GetSetup()->agentExtracust 
///һ����Ӧ��λ����Ŀ
#define ORDER_PER_NUM CGlobeSetup::GetSetup()->oderPerNum

///λ�����UI��ʼ��
CEGUI::Window* InitAuction();

///����UI�ؼ��ĳ�ʼ������
void SetInitializedAHUIProperty(CEGUI::Window* pageWnd);

///�¼�ע��
void SubscriberAHEvent(CEGUI::Window* pageWnd);

///������򿪵�ʱ���Զ�����һ������
bool OnRequestData(const CEGUI::EventArgs& e);
///�������λ�水ť�¼�������
bool OnBuyWeimianBtnClicked(const CEGUI::EventArgs& e);

///�������λ�水ť�¼�������
bool OnSaleWeimianBtnClicked(const CEGUI::EventArgs& e);

///�����ȡ��Ұ�ť�¼�������
bool OnPickUpGold(const CEGUI::EventArgs& e);

///�����ȡλ�水ť�¼�������
bool OnPickUpWeimian(const CEGUI::EventArgs& e);

///λ�����MCL�������б��)ѡ����Ŀ�����¼�
bool OnSaleMCLSelChanged(const CEGUI::EventArgs& e);

///λ����MCLѡ����Ŀ�����¼�
bool OnBuyMCLSelChanged(const CEGUI::EventArgs& e);

///ί�н���MCLѡ����Ŀ�����¼�
bool OnPerMCLSelChanged(const CEGUI::EventArgs& e);

///�ر�λ����۽���
bool OnSaleUIClosed(const CEGUI::EventArgs& e);

///��λ����۽���
bool OpenSaleUI();

///λ����۽���,�ύ��������
bool OnSaleSubmit(const CEGUI::EventArgs& e);

///�ر�λ�湺�����
bool OnBuyUIClosed(const CEGUI::EventArgs& e);

///��λ�湺�����
bool OpenBuyUI();

///λ���󹺽��棬�ύ������
bool OnBuySubmit(const CEGUI::EventArgs& e);

///����λ�湺�����UI
bool UpdateBuyUIDate(const CEGUI::EventArgs& e);

///����λ����۽���UI
bool UpdateSaleUIDate(const CEGUI::EventArgs& e);

///���¿ɹ���λ���б�MCLItem
bool UpdateCanBuyMCLDate(const CEGUI::EventArgs& e);

///���¿ɳ���λ���б�MCLItem
bool UpdateCanSaleMCLDate(const CEGUI::EventArgs& e);

///���¸��˹ҵ�MCLItem
bool UpdatePerMCLDate(const CEGUI::EventArgs& e);

///���۽��棬�ύ���۹ҵ�����
bool OnAgentBuySubmit(const CEGUI::EventArgs& e);

///���۽��棬�ύ���۹ҵ�ǰ������UI��ʾ
bool UpdateAgentBuyUIDate(const CEGUI::EventArgs& e);

///���۽��棬�ύ�󹺹ҵ�����
bool OnAgentSaleSubmit(const CEGUI::EventArgs& e);

///���۽��棬�ύ���۹ҵ�ǰ�����¼���UI��ʾ
bool UpdateAgentSaleUIDate(const CEGUI::EventArgs& e);

///��ȡ����UI��ʾ
bool UpdatePickUPUIDate(const CEGUI::EventArgs& e);

///����ί�н��棬�����ҵ�
bool OnCancelAgentOrder(const CEGUI::EventArgs& e);

///����������۹ҵ��б�
bool OnRefreshSaleMCL(const CEGUI::EventArgs& e);

///��������ˢ���󹺹ҵ��б�
bool OnRefreshBuyMCL(const CEGUI::EventArgs& e);

///����������˹ҵ��б�
bool OnRefreshPerMCL(const CEGUI::EventArgs& e);

///û���´򿪽���ʱ�򣬷���һ������ˢ������
bool OnOpenedUpdate(const CEGUI::EventArgs& e);

CEGUI::Window* InitAuction()
{
	//���ؽ���
	CEGUI::Window* wnd = LoadUI("Auction");
	//����UI�ؼ��ĳ�ʼ������
	SetInitializedAHUIProperty(wnd);
	//�¼�ע�ắ��
	SubscriberAHEvent(wnd);
	//Ĭ�ϴ򿪳�����UI������һ����������,���³�����UI��ʾ
	AHdata& ahd = GetInst(AHdata);
	ahd.Send_AH_REQUEST_ALL();
	return wnd;
}

void SetInitializedAHUIProperty(CEGUI::Window* pageWnd)
{
	//��һ�δ򿪴��ڣ�Ĭ�Ͽɼ�
	pageWnd->setVisible(false);

	CEGUI::Window* tab1 = pageWnd->getChildRecursive("Auction/Tab/BuySale");
	tab1->setVisible(true);//Ĭ��Tab1�ɼ�

	CEGUI::Editbox* editbox = WEditBox(pageWnd->getChildRecursive("Auction/Pickup/EditGold"));//�����ȡ�༭��ֻ��Ϊ����
	editbox->setText("");
	editbox->setReadOnly(true);
	//editbox->setMaxTextLength(10);
	//ֻ����������
	editbox->setValidationString("[0-9]*");

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Pickup/EditWeimian"));//λ����ȡ�༭��ֻ��Ϊ����
	editbox->setText("");
	editbox->setReadOnly(true);
	editbox->setValidationString("[0-9]*");
	//editbox->setMaxTextLength(10);
	CEGUI::MultiColumnList* mcl = WMCL(pageWnd->getChildRecursive("Auction/Tab/BuySale/SaleMCL"));
	mcl->setUserColumnDraggingEnabled(false);//�в����϶�
	mcl->setUserSortControlEnabled(false);//�в�������
	mcl->setUserColumnSizingEnabled(false);//�в��ɵ������

	mcl = WMCL(pageWnd->getChildRecursive("Auction/Tab/BuySale/BuyMCL"));
	mcl->setUserColumnDraggingEnabled(false);//�в����϶�
	mcl->setUserSortControlEnabled(false);//�в�������
	mcl->setUserColumnSizingEnabled(false);//�в��ɵ������

	CEGUI::Window* wnd = pageWnd->getChildRecursive("Auction/SaleWnd");
	wnd->setVisible(false);//���۽���Ĭ�ϲ��ɼ�

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/SaleWnd/saleNum"));//���۽���༭��ֻ����������
	editbox->setText("");
	editbox->setMaxTextLength(10);
	//editbox->setReadOnly(false);
	editbox->setValidationString("[0-9]*");
	editbox = WEditBox(pageWnd->getChildRecursive("Auction/SaleWnd/Text6"));//���۽��潻���ı���ʾ
	editbox->setText("");
	editbox = WEditBox(pageWnd->getChildRecursive("Auction/SaleWnd/Text61"));//
	editbox->setText("");
	

	wnd = pageWnd->getChildRecursive("Auction/BuyWnd");
	wnd->setVisible(false);//�������Ĭ�ϲ��ɼ�

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Buy/buyNum"));//�������༭��ֻ����������
	editbox->setText("");
	//editbox->setReadOnly(false);
	editbox->setValidationString("[0-9]*");
	editbox->setMaxTextLength(4);

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Buy/Text6"));//������潻���ı���ʾ
	editbox->setText("");
	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Buy/Text61"));
	editbox->setText("");

	mcl = WMCL(pageWnd->getChildRecursive("Auction/Tab/Query/MCL"));//���˲�ѯMCL����
	mcl->setUserColumnDraggingEnabled(false);//�в����϶�
	mcl->setUserSortControlEnabled(false);//�в�������
	mcl->setUserColumnSizingEnabled(false);//�в��ɵ������

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/sale/EditNum"));//�༭��ֻ����������
	editbox->setText("");
	//editbox->setReadOnly(false);
	editbox->setValidationString("[0-9]*");
	editbox->setMaxTextLength(5);

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/sale/EditPrice"));//�༭��ֻ����������
	editbox->setText("");
	//editbox->setReadOnly(false);
	editbox->setValidationString("[0-9]*");
	editbox->setMaxTextLength(5);

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/EditNum"));//�༭��ֻ����������
	editbox->setText("");
	//editbox->setReadOnly(false);
	editbox->setValidationString("[0-9]*");
	editbox->setMaxTextLength(5);

	editbox = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/EditPrice"));//�༭��ֻ����������
	editbox->setText("");
	//editbox->setReadOnly(false);
	editbox->setValidationString("[0-9]*");
	editbox->setMaxTextLength(5);

	//////////////////////////////////////////////////
	//ί��ҳ��֧����ʾ
	wnd = pageWnd->getChildRecursive("Auction/Tab/Agent/sale/subNum");//
	wnd->setText("");
	wnd = pageWnd->getChildRecursive("Auction/Tab/Agent/sale/subGold");//
	wnd->setText("");
	wnd = pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/subNum");
	wnd->setText("");
	wnd = pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/subGold");
	wnd->setText("");
	wnd = pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/Text");
	wnd->setText("");
	////////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// zhaohang  2010/5/14 
	// ˢ�°�ť����
	wnd = pageWnd->getChildRecursive("Auction/Tab/BuySale/RefreshSale");
	wnd->setVisible(false);
	wnd = pageWnd->getChildRecursive("Auction/Tab/BuySale/RefreshBuy");
	wnd->setVisible(false);
	wnd = pageWnd->getChildRecursive("Auction/Tab/Query/Refresh");
	wnd->setVisible(false);
	/////////////////////////////////////////////////
}

void SubscriberAHEvent(CEGUI::Window* pageWnd)
{
	pageWnd->subscribeEvent(EVENT_OPEN,CEGUI::Event::Subscriber(OnOpenedUpdate));//�󶨴򿪽���ˢ�������¼�

	pageWnd->subscribeEvent("UpdatePickUpUI",CEGUI::Event::Subscriber(UpdatePickUPUIDate));//������ȡ����UI

	CEGUI::PushButton* btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/BuySale/BtnBuyWeimian"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnBuyWeimianBtnClicked));//�����ť����λ��

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/BuySale/BtnSalWeimian"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnSaleWeimianBtnClicked));//�����ť����λ��

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Pickup/BtnGold"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnPickUpGold));//��ȡ���

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Pickup/BtnWeimian"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnPickUpWeimian));//��ȡλ��

	CEGUI::MultiColumnList* mcl = WMCL(pageWnd->getChildRecursive("Auction/Tab/BuySale/SaleMCL"));
	mcl->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged,CEGUI::Event::Subscriber(OnSaleMCLSelChanged));//λ��ɹ�����Ŀѡ���¼�
	mcl->subscribeEvent("UpdateCanBuyMCL",CEGUI::Event::Subscriber(UpdateCanBuyMCLDate));//���¿ɹ�������б�
	mcl->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(OnBuyWeimianBtnClicked));//˫���ɹ���Item���¼���Ӧ

	mcl = WMCL(pageWnd->getChildRecursive("Auction/Tab/BuySale/BuyMCL"));
	mcl->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged,CEGUI::Event::Subscriber(OnBuyMCLSelChanged));//λ��ɳ�����Ŀѡ���¼�
	mcl->subscribeEvent("UpdateBuyMCL",CEGUI::Event::Subscriber(UpdateCanSaleMCLDate));//���¿ɳ����б�
	mcl->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(OnSaleWeimianBtnClicked));//˫���ɳ���Item���¼���Ӧ

	mcl = WMCL(pageWnd->getChildRecursive("Auction/Tab/Query/MCL"));
	mcl->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged,CEGUI::Event::Subscriber(OnPerMCLSelChanged));//���˹ҵ���Ŀѡ���¼�
	mcl->subscribeEvent("UpdatePerMCL",CEGUI::Event::Subscriber(UpdatePerMCLDate));//���¸��˹ҵ��б�

	CEGUI::Window* wnd = pageWnd->getChildRecursive("Auction/SaleWnd");
	wnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(OnSaleUIClosed));//���۽���ر�

	btn = WPushButton(wnd->getChildRecursive("Auction/SaleWnd/Submit"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnSaleSubmit));//�ύ��������

	wnd = pageWnd->getChildRecursive("Auction/BuyWnd");
	wnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(OnBuyUIClosed));//�������ر�

	btn = WPushButton(wnd->getChildRecursive("Auction/Buy/Submit"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnBuySubmit));//�ύ��������

	CEGUI::Editbox* edb = WEditBox(pageWnd->getChildRecursive("Auction/Buy/buyNum"));//
	edb->subscribeEvent(CEGUI::Window::EventTextChanged,CEGUI::Event::Subscriber(UpdateBuyUIDate));//�������UI����

	edb = WEditBox(pageWnd->getChildRecursive("Auction/SaleWnd/saleNum"));
	edb->subscribeEvent(CEGUI::Window::EventTextChanged,CEGUI::Event::Subscriber(UpdateSaleUIDate));//���۽���UI����

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/Agent/sale/Submit"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnAgentSaleSubmit));//ί�г��۹ҵ��ύ

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/Submit"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnAgentBuySubmit));//ί���󹺹ҵ��ύ

	edb = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/sale/EditNum"));
	edb->subscribeEvent(CEGUI::Window::EventTextChanged,CEGUI::Event::Subscriber(UpdateAgentSaleUIDate));//ί�г��۹ҵ�UI����
	edb = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/sale/EditPrice"));
	edb->subscribeEvent(CEGUI::Window::EventTextChanged,CEGUI::Event::Subscriber(UpdateAgentSaleUIDate));//ί�г��۹ҵ�UI����


	edb = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/EditNum"));
	edb->subscribeEvent(CEGUI::Window::EventTextChanged,CEGUI::Event::Subscriber(UpdateAgentBuyUIDate));//ί����UI����
	edb = WEditBox(pageWnd->getChildRecursive("Auction/Tab/Agent/Buy/EditPrice"));
	edb->subscribeEvent(CEGUI::Window::EventTextChanged,CEGUI::Event::Subscriber(UpdateAgentBuyUIDate));//ί����UI����

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/Query/Cancel"));//����ί�г����ҵ�
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnCancelAgentOrder));

	//
	btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/Query/Refresh"));//����������¸��˹ҵ��б�
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnRefreshPerMCL));

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/BuySale/RefreshSale"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnRefreshSaleMCL));//����������³���ί�йҵ��б�

	btn = WPushButton(pageWnd->getChildRecursive("Auction/Tab/BuySale/RefreshBuy"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnRefreshBuyMCL));//�������������ί�йҵ��б�


}

bool OnBuyWeimianBtnClicked(const CEGUI::EventArgs& e)
{
	//�������ť���򿪹������
	OpenBuyUI();
	FireUIEvent("Auction/SaleWnd","CloseClicked");//�رճ��۽���

	return true;
}

bool OnSaleWeimianBtnClicked(const CEGUI::EventArgs& e)
{
	//������۰�ť���򿪳��۽���
	OpenSaleUI();
	FireUIEvent("Auction/BuyWnd","CloseClicked");//�رչ������
	return true;
}

bool OnPickUpGold(const CEGUI::EventArgs& e)
{
	AHdata& ah = GetInst(AHdata);
	ah.Send_AH_PICKUP(AH::PT_COIN);
	return true;
}

bool OnPickUpWeimian(const CEGUI::EventArgs& e)
{
	AHdata& ah = GetInst(AHdata);
	ah.Send_AH_PICKUP(AH::PT_GOODS);
	return true;
}

bool OnBuyMCLSelChanged(const CEGUI::EventArgs& e)
{
	return false;
}

bool OnSaleMCLSelChanged(const CEGUI::EventArgs& e)
{

	return false;
}

bool OnPerMCLSelChanged(const CEGUI::EventArgs& e)
{
	return false;
}

bool OnSaleUIClosed(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	wnd->setVisible(false);
	return true;
}

bool OpenSaleUI()
{
	CEGUI::WindowManager& wndmgr = GetWndMgr();
	//��ȡ���۶���ID
	CEGUI::MultiColumnList* mcl = WMCL(wndmgr.getWindow("Auction/Tab/BuySale/BuyMCL"));
	if(!mcl)
		return false;
	CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
	if(!lbi)
	{
		//MessageBox(g_hWnd,AppFrame::GetText("AU_100"),"ERROR",MB_OK);
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_100"),NULL,NULL,true);
		return false;
	}

	CEGUI::Window* wnd = wndmgr.getWindow("Auction/SaleWnd");
	wnd->setVisible(true);
	wnd->setAlwaysOnTop(true);
	CEGUI::Editbox* editbox = WEditBox(wnd->getChildRecursive("Auction/SaleWnd/saleNum"));//���۽���༭�򼤻�
	editbox->activate();

	AHdata& ah = GetInst(AHdata);
	uint ID = lbi->getID();
	ah.SetCanSaleID(ID);
	return true;
}

//�ύ��������
bool OnSaleSubmit(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& wndmgr = GetWndMgr();
	CEGUI::Editbox* edb = WEditBox(wndmgr.getWindow("Auction/SaleWnd/saleNum"));
	AHdata& ah = GetInst(AHdata);
	//��ȡ��������
	uint saleNum = CEGUI::PropertyHelper::stringToInt(edb->getText());
	uint cansaleNum = ah.GetCountByCanSaleID(ah.GetCanSaleID());
	if(saleNum == 0 || saleNum > cansaleNum )
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_101"),NULL,NULL,true);
		return false;
	}
	ah.Send_AH_SEL(cansaleNum);//���ͳ�������ǰҪ���ó��۵Ķ���ID
	return true;
}

bool OnBuyUIClosed(const CEGUI::EventArgs& e)
{
	CEGUI::Window* wnd = WEArgs(e).window;
	wnd->setVisible(false);
	return true;
}

bool OpenBuyUI()
{
	CEGUI::WindowManager& wndmgr = GetWndMgr();
	CEGUI::MultiColumnList* mcl = WMCL(wndmgr.getWindow("Auction/Tab/BuySale/SaleMCL"));
	if(!mcl)
		return false;
	CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
	if(!lbi)
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_102"),NULL,NULL,true);
		return false;
	}
	CEGUI::Window* wnd = wndmgr.getWindow("Auction/BuyWnd");
	wnd->setVisible(true);
	wnd->setAlwaysOnTop(true);
	CEGUI::Editbox* editbox = WEditBox(wnd->getChildRecursive("Auction/Buy/buyNum"));//�������༭�򼤻�
	editbox->activate();
	AHdata& ah = GetInst(AHdata);
	//�����ȡ���򶩵�ID
	uint ID = lbi->getID();
	ah.SetCanBuyID(ID);//����Ҫ����Ķ���ID
	return true;
}

////�ύ��������
bool OnBuySubmit(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& wndmgr = GetWndMgr();
	CEGUI::Editbox* edb = WEditBox(wndmgr.getWindow("Auction/Buy/buyNum"));
	AHdata& ah = GetInst(AHdata);
	//��ȡ��������
	uint buyNum = CEGUI::PropertyHelper::stringToInt(edb->getText());
	uint canbuyNum = ah.GetCountByCanBuyID(ah.GetCanBuyID());
	if(buyNum== 0 || buyNum > canbuyNum)
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_103"),NULL,NULL,true);
		return false;
	}
	ah.Send_AH_BUY(buyNum);//���͹�������ǰҪ���ù���Ķ���ID
	return true;
}

//����λ�湺�����UI
bool UpdateBuyUIDate( const CEGUI::EventArgs& e )
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Window* weimian = mgr.getWindow("Auction/Buy/Text6");
	weimian->setText("");
	CEGUI::Window* gold = mgr.getWindow("Auction/Buy/Text61");
	gold->setText("");
	//��ȡ����Ĺ�����Ŀ
	CEGUI::Editbox* edb = WEditBox(mgr.getWindow("Auction/Buy/buyNum"));
	if(!edb)
		return false;
	uint buyNum = CEGUI::PropertyHelper::stringToInt(edb->getText());
	if(buyNum == 0)
		return false;
	//���λ����ʾ
	weimian->setText(edb->getText());

	AHdata& ah = GetInst(AHdata);
	uint orderID = ah.GetCanBuyID();//����Ķ���ID
	uint price = ah.GetPriceByCanBuyID(orderID);
	//����۸�
	uint subCust = buyNum * price ;
	//������Ҫ֧�����
	gold->setText(CEGUI::PropertyHelper::intToString(subCust));
	return true;
}

bool UpdateSaleUIDate( const CEGUI::EventArgs& e )
{	
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Window* weimian = mgr.getWindow("Auction/SaleWnd/Text61");
	weimian->setText("");
	CEGUI::Window* gold = mgr.getWindow("Auction/SaleWnd/Text6");
	gold->setText("");
	//��ȡ����ĳ�����Ŀ
	CEGUI::Editbox* edb = WEditBox(mgr.getWindow("Auction/SaleWnd/saleNum"));
	if(!edb)
		return false;
	uint saleNum = CEGUI::PropertyHelper::stringToInt(edb->getText());
	if(saleNum==0)
		return false;
	//����֧��λ����
	weimian->setText(edb->getText());

	AHdata& ah = GetInst(AHdata);
	uint orderID = ah.GetCanSaleID();//��ȡҪ���۵Ķ���ID
	uint price = ah.GetPriceByCanSaleID(orderID);//����
	//����۸�
	uint subCust = saleNum * price;
	//������Ҫ֧�����
	gold->setText(CEGUI::PropertyHelper::intToString(subCust));
	return true;
}

bool UpdateCanBuyMCLDate(const CEGUI::EventArgs& e)
{
	AHdata& ah = GetInst(AHdata);
	AHdata::vecOrderRaw listOR = ah.m_CanBuyOrder;//�ɹ����б�
	CEGUI::MultiColumnList* mcl = WMCL(WEArgs(e).window);
	if(!mcl)
		return false;
	mcl->resetList();
	AHdata::vecOrderRaw::iterator it = listOR.begin();
	for (uint i = 0 ; i < listOR.size() ; ++i,++it)
	{
		mcl->addRow();
		CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(ToCEGUIString(AppFrame::GetText("AU_105")),it->id);//�ڶ��������붩��ID����
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,0,i);

		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(it->price));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,1,i);

		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(it->cnt));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,2,i);
	}

	return true;
}

bool UpdateCanSaleMCLDate(const CEGUI::EventArgs& e)
{
	AHdata& ah = GetInst(AHdata);
	AHdata::vecOrderRaw listOR = ah.m_CanSaleOrder;//�ɳ����б�
	CEGUI::MultiColumnList* mcl = WMCL(WEArgs(e).window);
	if(!mcl)
		return false;
	mcl->resetList();
	AHdata::vecOrderRaw::iterator it = listOR.begin();
	for (uint i = 0 ; i < listOR.size() ; ++i,++it)
	{
		mcl->addRow();
		CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(ToCEGUIString(AppFrame::GetText("AU_104")),it->id);//�ڶ��������붩��ID����
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,0,i);

		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(it->price));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,1,i);

		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(it->cnt));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,2,i);
	}
	return true;
}

bool UpdatePerMCLDate(const CEGUI::EventArgs& e)
{
	AHdata& ah = GetInst(AHdata);
	//AHdata::listSubOrderRaw listSubOR = ah.m_AgentOrder;
	AHdata::MapSubOrderRaw mapSubOR = ah.m_AgentOrder;
	CEGUI::MultiColumnList* mcl = WMCL(WEArgs(e).window);
	if(!mcl)
		return false;
	mcl->resetList();

	AHdata::MapSubOrderRaw::iterator it = mapSubOR.begin();
	for (int i = 0 ; it != mapSubOR.end(); ++it,++i)
	{
		mcl->addRow();
		CEGUI::ListboxTextItem* lti = NULL;
		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(it->first),it->first);//�ؼ�ID�붩��ID����
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,0,i);
		
		AH::SubOrderRaw& mapOr = it->second;
		if(mapOr.type == AH::OT_BUY)
			lti = new CEGUI::ListboxTextItem(ToCEGUIString(AppFrame::GetText("AU_104")));
		else if(mapOr.type == AH::OT_SELL)
			lti = new CEGUI::ListboxTextItem(ToCEGUIString(AppFrame::GetText("AU_105")));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,1,i);

		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(mapOr.cnt));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,2,i);

		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(mapOr.price));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,3,i);

		/////////////////////////////////////////////////
		// zhaohang  2010/5/12 
		// ʱ��UIδ����
		/////////////////////////////////////////////////
		uint subtime = mapOr.gentime / 60 / 60;
		lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(subtime));
		lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		mcl->setItem(lti,4,i);
	}
	return false;
}

bool UpdatePickUPUIDate(const CEGUI::EventArgs& e)
{
	CEGUI::Window* pageWnd = WEArgs(e).window;
	CEGUI::Editbox* edb = WEditBox(pageWnd->getChildRecursive("Auction/Pickup/EditGold"));
	AHdata& ah = GetInst(AHdata);
	edb->setText(CEGUI::PropertyHelper::intToString(ah.GetNumGoldCanPickUp()));

	edb = WEditBox(pageWnd->getChildRecursive("Auction/Pickup/EditWeimian"));
	edb->setText(CEGUI::PropertyHelper::intToString(ah.GetNumWeimianCanPickUp()));
	return true;
}

bool OnOpenedUpdate(const CEGUI::EventArgs& e)
{
	GetInst(AHdata).Send_AH_REQUEST_ALL();//����һ���б�����
	return true;
}

bool OnAgentBuySubmit(const CEGUI::EventArgs& e)
{
	//�õ�ί�й�����Ŀ�͵���
	AH::OrderRaw or;
	CEGUI::Editbox* edb = WEditBox(GetWndMgr().getWindow("Auction/Tab/Agent/Buy/EditNum"));
	if(!edb)
		return false;
	or.id = -1 ;// -1��ʶID�����õ�
	//��Ŀ
	or.cnt = CEGUI::PropertyHelper::stringToInt(edb->getText());//��λΪ���ٸ�
	if(or.cnt == 0)
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_106"),NULL,NULL,true);
		return false;
	}
	edb = WEditBox(GetWndMgr().getWindow("Auction/Tab/Agent/Buy/EditPrice"));
	if(!edb)
		return false;
	//����
	or.price = CEGUI::PropertyHelper::stringToInt(edb->getText());// ���/100λ��
	if(or.price == 0)
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_107"),NULL,NULL,true);
	}
	//���������Ͻ���Ƿ����㹺������
	CPlayer* player = GetGame()->GetMainPlayer();
	long money = player->GetMoney();
	float custm = or.price * or.cnt * AGENT_EXTRACUST ;//����������
	if( custm > money)
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_108"),NULL,NULL,true);
		return false;
	}

	//�ύί�й��򶩵�
	AHdata& ah = GetInst(AHdata);
	ah.Send_AH_ADD_AGENT(or,AH::OT_BUY);
	return true;
}

//����ί���󹺽�����ʾ
bool UpdateAgentBuyUIDate(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Window* wnd = NULL;
	//������
	CEGUI::Editbox* edb = WEditBox(mgr.getWindow("Auction/Tab/Agent/Buy/EditNum"));
	uint cnt = CEGUI::PropertyHelper::stringToInt(edb->getText());
	//�󹺵���
	edb = WEditBox(mgr.getWindow("Auction/Tab/Agent/Buy/EditPrice"));
	uint price = CEGUI::PropertyHelper::stringToInt(edb->getText());
	if(cnt == 0 || price == 0)//���һ��Ϊ0,ֱ�ӷ���
	{
		wnd = mgr.getWindow("Auction/Tab/Agent/Buy/subGold");
		wnd->setText("");//������
		wnd = mgr.getWindow("Auction/Tab/Agent/Buy/Text");//�ܹ�֧��
		wnd->setText("");
		return false;
	}
	//����ί�й���λ���ܼ���ʾ
	 wnd = mgr.getWindow("Auction/Tab/Agent/Buy/subNum");
	uint sumPrice = price * cnt;
	wnd->setText(CEGUI::PropertyHelper::intToString(sumPrice));

	float extreCust = price * cnt * AGENT_EXTRACUST;//������
	wnd = mgr.getWindow("Auction/Tab/Agent/Buy/subGold");
	wnd->setText(CEGUI::PropertyHelper::intToString((int)extreCust));

	wnd = mgr.getWindow("Auction/Tab/Agent/Buy/Text");//�ܹ�֧��
	wnd->setText(CEGUI::PropertyHelper::intToString((sumPrice + (int)extreCust)));
	return true;
}

bool OnAgentSaleSubmit(const CEGUI::EventArgs& e)
{
	//�õ�ί�г�����Ŀ�͵���
	AH::OrderRaw or;
	CEGUI::Editbox* edb = WEditBox(GetWndMgr().getWindow("Auction/Tab/Agent/sale/EditNum"));
	if(!edb)
		return false;
	or.id = -1;//-1��ʶ id�����õ�
	//����
	or.cnt = CEGUI::PropertyHelper::stringToInt(edb->getText());
	if(or.cnt == 0)
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_109"),NULL,NULL,true);
		return false;
	}
	edb = WEditBox(GetWndMgr().getWindow("Auction/Tab/Agent/sale/EditPrice"));
	if(!edb)
		return false;
	//����
	or.price = CEGUI::PropertyHelper::stringToInt(edb->getText());
	if(or.price == 0)
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_110"),NULL,NULL,true);
		return false;
	}
	//����������λ���Ƿ�������Ŀ
	CPlayer* player = GetGame()->GetMainPlayer();
	long cnt = player->GetGoodsAmount(CGlobeSetup::GetSetup()->strWeiMianName);
	if(cnt < or.cnt * 100 ) //�����������С�ڳ�������
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_111"),NULL,NULL,true);
		return false;
	}
	//���������ϵĽ���Ƿ�����ί����ȡ��������
	long money = player->GetMoney();
	float custm = or.cnt * or.price * AGENT_EXTRACUST;//����������
	if ( custm > money  )
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_112"),NULL,NULL,true);
		return false;
	}
	//�ύί�г��۶���
	AHdata& ah = GetInst(AHdata);
	ah.Send_AH_ADD_AGENT(or,AH::OT_SELL);
	return true;
}

//���¼���UI��ʾ
bool UpdateAgentSaleUIDate(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Window* wnd = NULL;
	CEGUI::Editbox* edb = WEditBox(mgr.getWindow("Auction/Tab/Agent/sale/EditNum"));//��������
	uint cnt = CEGUI::PropertyHelper::stringToInt(edb->getText());

	edb = WEditBox(mgr.getWindow("Auction/Tab/Agent/sale/EditPrice"));//���۵���
	uint price = CEGUI::PropertyHelper::stringToInt(edb->getText());

	if(cnt == 0 || price == 0)//����һ��Ϊ0����ֱ�ӷ��أ�������UI
	{
		//����֧����������ʾ
		wnd = mgr.getWindow("Auction/Tab/Agent/sale/subGold");
		wnd->setText("");
		return false;
	}
	//����֧��λ����ʾ
	wnd = mgr.getWindow("Auction/Tab/Agent/sale/subNum");
	wnd->setText(CEGUI::PropertyHelper::intToString(cnt * ORDER_PER_NUM));
	//����֧����������ʾ
	wnd = mgr.getWindow("Auction/Tab/Agent/sale/subGold");
	float extraCust = cnt * price * AGENT_EXTRACUST ;
	wnd->setText(CEGUI::PropertyHelper::intToString((int)extraCust));
	return true;
}


//����ί�ж����¼�
bool OnCancelAgentOrder(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::MultiColumnList* mcl = WMCL(mgr.getWindow("Auction/Tab/Query/MCL"));
	//���ѡ��Item��Ӧ�Ķ���ID
	CEGUI::ListboxItem* li = mcl->getFirstSelectedItem();
	if(!li)
	{
		/***********************************************************************/
		/* zhaohang fix 2010-9-3
		/* �޸���Ϣ����ʽ�����÷�װ��MsgEventManager,�Լ��µ��ַ���Դ���ط�ʽAppframe
		/***********************************************************************/
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_113"),NULL,NULL,true);
		return false;
	}
	uint orderID = li->getID();
	AHdata& ah = GetInst(AHdata);
	ah.Send_AH_REMOVE_AGENT(orderID);
	return true;
}

bool OnRefreshSaleMCL(const CEGUI::EventArgs& e)
{
	//����ˢ���б���Ϣ
	AHdata& ad = GetInst(AHdata);
	ad.Send_AH_REQUEST_BUY();
	return true;
}

bool OnRefreshBuyMCL(const CEGUI::EventArgs& e)
{
	//����ˢ���б���Ϣ
	AHdata& ad = GetInst(AHdata);
	ad.Send_AH_REQUEST_SELL();
	return true;
}

bool OnRefreshPerMCL(const CEGUI::EventArgs& e)
{
	//����ˢ���б���Ϣ
	AHdata& ad = GetInst(AHdata);
	ad.Send_AH_REQUEST_AGENT();
	return true;
}