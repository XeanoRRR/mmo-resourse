#include "stdafx.h"
#include "MainbarPreDef.h"
#include "../UIDef.h"
#include "SkillDef.h"
#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/MainPlayerHand.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/Skills/SkillListXml.h"
#include "../../GameClient/ClientApp/Skills/SkillLogicSys.h"
#include "../../../Public/Common/OccupSkillList.h"
#include "../../../Public/Common/SkillRelated.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
extern uint GetDCIndexInMainbar(CEGUI::Window* dc);

//�õ����������ؼ�������
uint GetSkillDescIndexInSkillTemplate(CEGUI::Window* dc);
//�ɼ���ID������DragContainer�����ӿؼ�StaticImage����ʾ(�޸�ͼ������ÿؼ�ID�뼼��ID����)
void SetDCIconBySkillID(uint skillID,CEGUI::Window* dragContainer);

//�޸ļ��ܽ����Icon������˵��
void SetSkillIconDesc(uint skillID,CEGUI::Window* descWnd,uint SkillLevel = 1);

CEGUI::Window* InitSkill();
//���ó�ʼ������״̬
void SetInitializedSkillTemplateUIProperty(CEGUI::Window* PageWnd);

//�¼�������ע�Ἧ��
void SubscriberSkillTemplateEvent(CEGUI::Window* PageWnd);

//����ͼ���϶��ͷ�
bool OnSkillIconDragDropped(const CEGUI::EventArgs& e);

//����ͼ���϶��뿪�ؼ�
bool OnSkillIconDragDropLeaves(const CEGUI::EventArgs& e);

////����ͼ���϶�����mainbar�ؼ�
//bool OnSkillIconDragDropEnters(const CEGUI::EventArgs& e);

bool OnPageChanged(const CEGUI::EventArgs& e);
//����ѻἼ��
bool OnAddCanUseSkill(const CEGUI::EventArgs& e);

bool OnListboxSelChanged(const CEGUI::EventArgs& e);

////���������ť
//bool OnUpdateSkillLvBtnClicked(const CEGUI::EventArgs& e);


//����Tab��ĳһѡ�����ʾ
void UpdateTabContent(CEGUI::Window* selTab,size_t idxInTabCtl);


//���½����Ҳ���ѻ���ʽͼ���ı�����,����startPos/endPos �������ʽ�б���� ��ʼ/���� λ��
void UpdateCanUseSkill(const uint startPos,const uint endPos);

//����ѡ�е�ListItem���͸����ѻ���ʽUI����UpdateCanUseSkill()����
void UpdateCanUseSkillByType(const uint startPos,const uint endPos,uint listitemID);

// ��UpdateCanUseskillByType()����
void UpdateRightUI(const DWORD dwSkillID,const uint indexWnd,uint skillLevel = 1);

//����ѻἼ��UI����ʾ����
void ClearCanUseSkill();

//��������ĳ��Tab�µ�ĳ��listItem��ļ����������ݵ���ʾ
void UpdateCustOnLvUp();

//���½����м��ͼ��
void UpdateCentreMainIcon();

//��ȡTabContent�µ�listbox����
CEGUI::Listbox* GetListBoxInCurSelTabContent();

//��ȡ��ǰtabCtrl��ѡ�е�tabContent
CEGUI::Window* GetCurSelTabContent();

typedef std::list<COccupSkill::tagOccupSkill*>listOccStudySkill;
//��ȡ��ǰ���ְҵ���ѻἼ���б�,��sSkill����ֵ
void GetOccupStudySkill(listOccStudySkill& sSkill);

CEGUI::TabControl* GetTabControl();

//��ȡ��ǰ��ListBox�б�ѡ����Ŀ����
size_t GetCurSelItemInListBox(const CEGUI::String &Lisboxname,CEGUI::Window& LbParentWnd);

//����mainbar��ݼ��ͼ���Icon�Ľ���
void ProcessMainbarDCToSkillTemplate(CEGUI::Window* targetWnd,CEGUI::Window* sourceWnd);

//��������ڵĻ��ཻ��
void ProcessInternalDragDropped(CEGUI::Window* targetWnd,CEGUI::Window* sourceWnd);

CEGUI::Window* InitSkill()
{
#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("SkillLayout.log");
#endif
	
	CEGUI::Window* wnd = LoadUI(SKILL_PAGE_NAME);

#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("CEGUI.log",true);
#endif
	//�����ʼ״̬����
	SetInitializedSkillTemplateUIProperty(wnd);
	//�¼�ע��
	SubscriberSkillTemplateEvent(wnd);

	//ҳ����ʾ��ʼ��,��Ϊ0-10Ϊ�������ܣ����Լ���һҳ���б���Ŀ
	UpdateCanUseSkill(0,17);
	return wnd;
}
/////////////////////////////////////////////////
// zhaohang  2010/5/17 
// ����״̬��ʼ��
void SetInitializedSkillTemplateUIProperty(CEGUI::Window* PageWnd)
{
	if(!PageWnd)
		return;
	PageWnd->setVisible(false);//����������Ϊ��ʹ��һ�δ򿪴���ʱ��ʹ���ڿɼ�
	//����SkillIcon�Ĵ�С
	for(int i = 0 ; i < (int)SKILL_PAGE_SIZE ; ++i)
	{
		char wndname[256];
		sprintf(wndname,"Skill/StaticText/StaticImage%d/DragContainer",i);
		CEGUI::Window* wnd = PageWnd->getChildRecursive(wndname);
		if(wnd)
		{
			wnd->setSize(CEGUI::UVector2(cegui_absdim(SKILL_ICON_SIZE + 0),cegui_absdim(SKILL_ICON_SIZE + 0)));
			wnd->setPosition(CEGUI::UVector2(cegui_absdim(5),cegui_absdim(5)));
		}
		sprintf(wndname,"Skill/StaticText/StaticImage%d",i);
		wnd = PageWnd->getChildRecursive(wndname);
		if(wnd)
		{
			wnd->setSize(CEGUI::UVector2(cegui_absdim(SKILL_ICON_SIZE + 10),cegui_absdim(SKILL_ICON_SIZE + 10)));
		}
	}
	ClearCanUseSkill();//����ѻἼ��
}
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// zhaohang  2010/5/17 
// �¼�������ע�Ἧ��
void SubscriberSkillTemplateEvent(CEGUI::Window* PageWnd)
{
	if(!PageWnd)
		return ;
	PageWnd->subscribeEvent("AddCanUseSkill",CEGUI::Event::Subscriber(OnAddCanUseSkill));//����ѻἼ��

	CEGUI::PushButton* btn = WPushButton(PageWnd->getChildRecursive("Skill/PrePage"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnPageChanged));//�ѻἼ�ܷ�ҳ

	btn = WPushButton(PageWnd->getChildRecursive("Skill/NextPage"));
	btn ->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnPageChanged));//�ѻἼ�ܷ�ҳ

	btn = WPushButton(PageWnd->getChildRecursive("Skill/BottomText/BtnUpLv"));
	//btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnUpdateSkillLvBtnClicked));//����������ť���

	CEGUI::TabControl* tbc = GetTabControl();
	size_t TabPageNum = tbc->getTabCount();

	for(size_t i = 0 ; i < TabPageNum ; ++i)
	{
		char listboxname[256];
		sprintf(listboxname,"Skill/Tab/Tab %d/ListBox",i);
		CEGUI::Listbox* lb = WListBox(tbc->getChildRecursive(listboxname));
		if(lb)
		{
			//listBox selectItemChangeed
			lb->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::Event::Subscriber(OnListboxSelChanged));
			CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(ToCEGUIString(AppFrame::GetText("Skill_64")));
			item->setID(0);
			item->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			item->setSelected(true);
			lb->addItem(item);
		}
	}
	for(size_t i = 0 ; i < TabPageNum ; ++i)
	{
		char listboxname[256];
		sprintf(listboxname,"Skill/Tab/Tab %d/ListBox",i);
		CEGUI::Listbox* lb = WListBox(tbc->getChildRecursive(listboxname));
		if(lb)
		{
			//listBox selectItemChangeed
			lb->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::Event::Subscriber(OnListboxSelChanged));
			CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(ToCEGUIString(AppFrame::GetText("Skill_63")));
			item->setID(1);
			item->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			//item->setSelected(true);
			lb->addItem(item);
		}
	}

	//DragContainer �¼� 
	char itemName[256];
	for( int i = 0 ; i < (int)SKILL_PAGE_SIZE ; ++i)
	{
		//�������
		sprintf(itemName,"Skill/StaticText/StaticImage%d",i);
		CEGUI::Window* dragItem = PageWnd->getChildRecursive(itemName);
		if(dragItem)
		{
			//dragItem->subscribeEvent(CEGUI::Window::EventDragDropItemDropped,CEGUI::Event::Subscriber(OnSkillIconDragDropped));//��ק�ͷ�
			//dragItem->subscribeEvent(CEGUI::Window::EventDragDropItemLeaves,CEGUI::Event::Subscriber(OnSkillIconDragDropLeaves));//��ק�뿪���ܴ���
			//dragItem->subscribeEvent(CEGUI::Window::EventDragDropItemEnters,CEGUI::Event::Subscriber(OnSkillIconDragDropEnters));//��ק����mainbar����
		}
		//�Ϸŵ��������Ѿ�����DragItem�����ϴ���(ʵ�ʾ���һ��������
		sprintf(itemName,"Skill/StaticText/StaticImage%d/DragContainer",i);
		dragItem = PageWnd->getChildRecursive(itemName);
		if(dragItem)
		{
			dragItem ->subscribeEvent(CEGUI::Window::EventDragDropItemDropped,CEGUI::Event::Subscriber(OnSkillIconDragDropped));//item����
			dragItem->subscribeEvent(CEGUI::Window::EventDragDropItemLeaves,CEGUI::Event::Subscriber(OnSkillIconDragDropLeaves));//mainbar��Dc��ק�뿪�¼�
			//dragItem->subscribeEvent(CEGUI::Window::EventDragDropItemEnters,CEGUI::Event::Subscriber(OnSkillIconDragDropEnters));//��ק����mainbar����
		}
	}
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// zhaohang  2010/5/17 
// �¼��뼼��ͼ����ק����¼�����

//����ͼ���϶��뿪������
bool OnSkillIconDragDropLeaves(const CEGUI::EventArgs& e)
{
	CEGUI::Window* targetDC = WEArgs(e).window;
	CEGUI::DragContainer* sourceDC = static_cast<const CEGUI::DragDropEventArgs&>(e).dragDropItem;
#ifdef _DEBUG
	const char* name = targetDC->getName().c_str();
#endif
	if(sourceDC->getName().find("MainBarPage") != CEGUI::String::npos)
	{
		CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
		if(pHand)
		{
			pHand->SetMainPlayerHandBySkill(sourceDC->getID());
		}
	}
	return true;
}

//����ͼ���϶��ͷ���
bool OnSkillIconDragDropped(const CEGUI::EventArgs& e)
{
	CEGUI::Window* targetWnd= WEArgs(e).window;
	CEGUI::DragContainer* dropWnd = static_cast<const CEGUI::DragDropEventArgs&>(e).dragDropItem;
#ifdef _DEBUG
	const char* tagetWndName = targetWnd->getName().c_str();
	const char* dragWndName = dropWnd->getName().c_str();
#endif 
	//if(dropWnd->getName().find(SKILL_PAGE_NAME) != CEGUI::String::npos)//���������ж��ǽ�����Icon�Ľ���
	//	//ProcessInternalDragDropped(targetWnd,dropWnd);//���������ڽ�������
	/////////////////////////////////////////////////
	// zhaohang  2010/5/19 
	// ����mianbar�ϼ����ϵ����ܽ���
	if( dropWnd->getName().find("MainBarPage") != CEGUI::String::npos)
		ProcessMainbarDCToSkillTemplate(targetWnd,dropWnd);
	/////////////////////////////////////////////////
	return true;
}


/////////////////////////////////////////////////

/////////////////////////////////////////////////
// zhaohang  2010/4/27 
// �¼���Ӧ����
bool OnListboxSelChanged(const CEGUI::EventArgs& e)
{
	//���ѡ�е������м��ܣ�����Ϊ0-10Ϊ�������ܣ����Խ���λ��Ϊ����һҳ���б���Ŀ
	UpdateCanUseSkill(0,17);
	return true;
}

//bool OnUpdateSkillLvBtnClicked(const CEGUI::EventArgs& e)
//{
//	return true;
//}
bool OnPageChanged(const CEGUI::EventArgs& e)
{
	//const char* name = NULL;
	ClearCanUseSkill();
	CPlayer* player = GetGame()->GetMainPlayer();
	std::list<CPlayer::tagSkill>* skilllist = &player->GetSkillList();

	CEGUI::PushButton* btn = WPushButton(WEArgs(e).window);
	CEGUI::String prepgbtn = "Skill/PrePage" ;
	CEGUI::String nextpgbtn = "Skill/NextPage";
	CEGUI::String btnName = btn->getName();
	//����ҳ����
	static int pageIdx = 0;
	int totalPage = (int)((float)skilllist->size() / SKILL_PAGE_SIZE );
	btnName == prepgbtn ?  pageIdx-- : (btnName == nextpgbtn ? pageIdx++ : pageIdx=0);
	if(pageIdx < 0 )
		pageIdx = 0;
	else if(pageIdx > totalPage)
		pageIdx = totalPage;
	UpdateCanUseSkill( int(pageIdx * SKILL_PAGE_SIZE),int((pageIdx + 1 ) * SKILL_PAGE_SIZE));
	return true;
}

bool OnAddCanUseSkill(const CEGUI::EventArgs& e)
{
//	CEGUI::Listbox* lb = WListBox(WEArgs(e).window);
//#ifdef _DEBUG
//	const char* name = lb->getName().c_str();
//#endif
	UpdateCanUseSkill(0,7);
	return false;
}
/////////////////////////////////////////////////


/////////////////////////////////////////////////
// zhaohang  2010/4/27 
// �������ݺ���
void UpdateCentreMainIcon()
{
	CEGUI::Window* tabct = GetCurSelTabContent();
	if(!tabct)
		return;
}

void UpdateCustOnLvUp(CEGUI::Window* tabctlContent /* = NULL */)
{

}
void ClearCanUseSkill()
{
	for(float i= 0 ; i < SKILL_PAGE_SIZE ; ++i)
	{
		//SKILLIcon �ؼ�����
		char si[256];
		//SkiLLDes �ؼ�����
		char st[256];
		//SkillDragcontainer����
		char dc[256];
		sprintf(si,"Skill/StaticText/StaticImage%d/DragContainer/sImage",(int)i);
		sprintf(st,"Skill/StaticText/StaticText%d",(int)i);
		sprintf(dc,"Skill/StaticText/StaticImage%d/DragContainer",(int)i);
		CEGUI::Window* siWnd = GetWndMgr().getWindow(si);
		siWnd->setProperty("Image","");
		siWnd->setID(0);
		CEGUI::Window* stWnd = GetWndMgr().getWindow(st);
		stWnd->setText("");
		stWnd->setID(0);
		CEGUI::Window* dcWnd = GetWndMgr().getWindow(dc);
		dcWnd->setID(0);
		dcWnd->setTooltipText("");
	}
}
void UpdateCanUseSkillByType(const uint startPos,const uint endPos,uint listitemID)
{
	if( listitemID == 0 )
	{
		listOccStudySkill os;
		GetOccupStudySkill(os);//��õ�ǰְҵ�µ���ѧ����
		//�жϷ�Χ�Ƿ�Ϸ�
		if(startPos >= os.size() || startPos >= endPos)
		{
//#ifdef _DEBUG
//			throw CEGUI::InvalidRequestException("UpdateCanUseSkillPage is out of range");
//#endif
			return;
		}
		//����λ��ȡ��Сֵ
		const size_t tempEndPos = min(endPos,os.size());
		//������ָ��ʼλ��
		listOccStudySkill::iterator skilliter = os.begin();
		for(size_t i = 0 ; i <startPos ; ++i)
			++skilliter;
		for(uint i = 0 ; i < tempEndPos - startPos ; ++i ,++skilliter)
		{
			DWORD dwSkillID = (*skilliter)->lSkillID;
			UpdateRightUI(dwSkillID,i);
		}

	}
	else if( listitemID == 1)
	{
		//��ȡ��Ҽ����б�
		CPlayer* player = GetGame()->GetMainPlayer();
		std::list<CPlayer::tagSkill>& skilllist = player->GetSkillList();
		//��ʼλ���Ƿ�Ϸ�
		if(startPos >= skilllist.size() || startPos >= endPos )
		{
//#ifdef _DEBUG
//			throw CEGUI::InvalidRequestException("UpdateCanUseSkillPage is out of range");
//#endif
			return;
		}
		//����λ��ȡ��С��
		const size_t tempEndPos = min(endPos,skilllist.size());
		std::list<CPlayer::tagSkill>::iterator skilliter = skilllist.begin();
		//������ָ��ʼλ��
		for(size_t i = 0 ; i < startPos; ++i)
		{
			++skilliter;
		}
		int wndIdx = 0;
		for(uint i = 0 ; i < tempEndPos - startPos ; ++i,++skilliter)
		{
			DWORD dwSkillID = skilliter->dwID;
			// ���ڻ��������༼��
			if(dwSkillID <= SKILL_BASE_BLADEFIST)
			{
				dwSkillID = player->GetDefaultAttackSkillID();
				UpdateRightUI(dwSkillID,wndIdx,skilliter->wLevel);
				continue;
			}
			else{
				UpdateRightUI(dwSkillID,++wndIdx,skilliter->wLevel);
			}
		}
	}
}
void UpdateRightUI(const DWORD dwSkillID,const uint indexWnd,uint SkillLevel)
{
	char staticimage[256];
	char statictext[256];
	//char imagesetname[256];

	//��ϴ�������
	sprintf(staticimage,"Skill/StaticText/StaticImage%d/DragContainer",indexWnd);
	CEGUI::Window* dcWnd = GetWndMgr().getWindow(staticimage);
	//����DCIcon������ID
	SetDCIconBySkillID(dwSkillID,dcWnd);

	//��ϴ�������
	sprintf(statictext,"Skill/StaticText/StaticText%d",indexWnd);
	CEGUI::Window* wndText = GetWndMgr().getWindow(statictext);
	//�޸ļ���˵��������ID
	SetSkillIconDesc(dwSkillID,wndText,SkillLevel);
}
void UpdateCanUseSkill(const uint startPos,const uint endPos)
{
	//����ѻ�SkillUI��ʾ
	ClearCanUseSkill();

	CEGUI::Listbox* lb = GetListBoxInCurSelTabContent();
	if(!lb)
		return;
	CEGUI::ListboxItem* lbi = lb->getFirstSelectedItem();
	if(!lbi)
		return;
	uint id = lbi->getID();
	UpdateCanUseSkillByType(startPos,endPos,id);
}


void UpdateTabContent(CEGUI::Window* selTab,size_t idxInTabCtl)
{
	if(!selTab)
		return;
	char listboxname[256];
	sprintf(listboxname,"Skill/Tab/Tab %d/ListBox",idxInTabCtl+1);
	size_t selitem = GetCurSelItemInListBox(listboxname,*selTab);
}



/////////////////////////////////////////////////
// zhaohang  2010/4/27 
// ��������
CEGUI::Listbox* GetListBoxInCurSelTabContent()
{
	CEGUI::TabControl* tabctrl  = GetTabControl();
	CEGUI::Window* tabcte = GetCurSelTabContent();
	char listbox[256];
	sprintf(listbox,"Skill/Tab/Tab %d/ListBox",tabctrl->getSelectedTabIndex());
	return WListBox(tabcte->getChildRecursive(listbox));
}
CEGUI::Window* GetCurSelTabContent()
{
	CEGUI::TabControl* tbc = GetTabControl();
	if(!tbc)
		return NULL;
	return tbc->getTabContentsAtIndex(tbc->getSelectedTabIndex());
}
void GetOccupStudySkill(listOccStudySkill& sSkill)
{
	sSkill.clear();
	CPlayer* player = GetGame()->GetMainPlayer();
	//��ȡ���ְҵ
	eOccupation eOccu = player->GetOccupation();
#ifdef _DEBUG
	const char* occuName = player->GetPlayerOccuType(eOccu);
#endif
	//����ְҵ��ȡ��Ӧ�����б�
	COccupSkill *pOccupSkill = GetGame()->GetOccupSkillList()->GetOccupSkill(eOccu);
	//��ȡְҵ����ļ�����Ŀ
	size_t opslNum = pOccupSkill->GetOccupSkillCount();
	//����ְҵ��Ӧ�ļ����Ƿ�������ѻἼ���б���
	for(size_t size = 0; size < opslNum ; ++size)
	{
		uint ID = pOccupSkill->GetOccupSkill()[size]->lSkillID;
		if(ID > 0 && player->CheckSkill(ID) > 0 ) //��
		{
			sSkill.push_back(pOccupSkill->GetOccupSkill()[size]);
		}
	}
}
CEGUI::TabControl* GetTabControl()
{
	return WTabControl(GetWndMgr().getWindow("Skill/Tab"));
}
size_t GetCurSelItemInListBox(const CEGUI::String &Lisboxname,CEGUI::Window& LbParentWnd)
{
	CEGUI::Listbox* lb = WListBox(LbParentWnd.getChildRecursive(Lisboxname));
	if(!lb)
		return CEGUI::String::npos;
#ifdef _DEBUG
	const char* name = lb->getName().c_str();
#endif
	CEGUI::ListboxItem* lbi = lb->getFirstSelectedItem();
	if(lbi)
	{
#ifdef _DEBUG
		name = lbi->getText().c_str();
		CEGUI::uint ID =lbi->getID();
#endif
		return lb->getItemIndex(lbi);
	}
	else
		return lb->getItemCount();
}

/////////////////////////////////////////////////


/////////////////////////////////////////////////
// zhaohang  2010/5/20
// 
void ProcessMainbarDCToSkillTemplate(CEGUI::Window* targetWnd,CEGUI::Window* sourceWnd)
{
	if(!targetWnd || !sourceWnd)
		return;
	//���mainbar��DC
	SetDCIconBySkillID(0,sourceWnd);
	//�޸�mainbar�Ŀ�ݼ�
	// ���ÿ�ݼ�
	uint hotKeyIdc = GetDCIndexInMainbar(sourceWnd);
	SkillLogicSys& SkillLS = GetInst(SkillLogicSys);
	SkillLS.Send_Set_Skill_HotKey(hotKeyIdc+SHORTCUT_COUNT,0);//0~11->F1~F12,���Լ�12
	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();//���ϵ��ͷ�
	if(pHand)
	{
		pHand->SetMainPlayerHandBySkill(sourceWnd->getID());
	}
}
void ProcessInternalDragDropped(CEGUI::Window* targetWnd,CEGUI::Window* sourceWnd)
{
	if(!targetWnd || !sourceWnd)
		return;
	uint targetID = targetWnd->getID();
	uint sourceID = sourceWnd->getID();
	//����
	SetDCIconBySkillID(targetID,sourceWnd);
	SetDCIconBySkillID(sourceID,targetWnd);
	//�������޸ļ�������˵��
}
/////////////////////////////////////////////////
// zhaohang  2010/5/20 
// 
//�ɼ���ID������DragContainer�����ӿؼ�StaticImage����ʾ(�޸�ͼ������ÿؼ�ID�뼼��ID����)
void SetDCIconBySkillID(uint skillID,CEGUI::Window* dragContainer)
{
	if(!dragContainer)
		return;
	char imagesetname[256];
	sprintf(imagesetname,SKILL_PREFIXID,skillID);
	CEGUI::String imagename = CEGUI::PropertyHelper::intToString(skillID)+".jpg";
	CEGUI::GUISheet* childImg = WGUISheet(dragContainer->getChildAtIdx(0));
	if(!childImg)
		return;
	if(skillID == 0)
	{
		childImg->setProperty("Image","");
		dragContainer->setTooltipText("");
	}
	else
	{
		SetBackGroundImage(childImg,imagesetname,SKILL_ICON_PATH,imagename.c_str());
		CSkillListXml::tagSkill* xmlSkill = CSkillListXml::GetProperty(skillID);
		if(!xmlSkill)
        {
            MsgEventManager& mgr = GetInst(MsgEventManager);
			char strTemp[256];
			sprintf(strTemp,AppFrame::GetText("SDCC_63"),skillID);
            mgr.PushEvent(Msg_Ok,strTemp);
            return;
        }
		CEGUI::String dest = xmlSkill->strSkillDesc.c_str();
		dragContainer->setTooltipText(dest);
	}
	//�޸Ĺ���ID
	childImg->setID(skillID);
	dragContainer->setID(skillID);
}

//�޸ļ��ܽ����Icon������˵��
void SetSkillIconDesc(uint skillID,CEGUI::Window* descWnd,uint SkillLevel)
{
	if(!descWnd)
		return;
	CSkillListXml::tagSkill* xmlSkill = CSkillListXml::GetProperty(skillID);
	if(!xmlSkill)
		return;
	CEGUI::String dest = ToCEGUIString(xmlSkill->strSkillName.c_str());
	dest += CEGUI::PropertyHelper::intToString((skillID));
	descWnd->setText(dest);

	descWnd->setID(skillID);//�ؼ�ID�ͼ���ID����
}


/////////////////////////////////////////////////