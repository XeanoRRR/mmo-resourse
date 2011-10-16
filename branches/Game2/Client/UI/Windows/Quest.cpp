#include "StdAfx.h"
#include "../UIDef.h"
#include "QuestDef.h"
//�������include����ɾ������ʱ�����Ϊ�����Զ���ʾ��
#include "../CEGUI/cegui/include/elements/CEGUITree.h"
#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/QuestSystem/QuestSystem.h"

CEGUI::Window* InitQuest();

bool OnQuestTabSelectChanged(const CEGUI::EventArgs &args);         //�ı��������ͱ�ǩ��ѡ����Ŀ

bool OnNormalQuestTreeSelectChanged(const CEGUI::EventArgs &args);  //�ı���ͨ��������ѡ����Ŀ
bool OnNormalQuestTreeAdded(const CEGUI::EventArgs &args);          //��ͨ���������һ��
bool OnNormalQuestTreeDeleted(const CEGUI::EventArgs &args);        //��ͨ������ɾ��һ��

bool OnRemoveQuestButtonClicked(const CEGUI::EventArgs& args);      //���"��������"��ť
bool OnShareQuestButtonClicked(const CEGUI::EventArgs& args);       //���"��������"��ť
bool OnDrawAwardButtonClicked(const CEGUI::EventArgs& args);        //���"��ȡ����"��ť

bool AddQuest_impl(CEGUI::Tree* tree, CQuestRecord* questRecord);

bool OnUpdateTeamQuestList(const CEGUI::EventArgs& e);			    //���¶���������ʾ
bool OnUpdateTeamQuestRunning(const CEGUI::EventArgs& e);	        //���¶������ڽ����������ʾ

bool OnOpenQuestUI(const CEGUI::EventArgs& Args);                   //������UI
bool OnUpdateQuestSelectText(const CEGUI::EventArgs& Args);         //
bool OnClearQuestTreeSelectText(const CEGUI::EventArgs & Args);     //���������ʾ�����ı�

namespace  QuestUI
{
   static CQuestRecord*  g_CurrRecord = NULL;
}

CEGUI::Window* InitQuest()
{
    CEGUI::Window* wnd = LoadUI(QUEST_PAGE_NAME);

    CEGUI::Window* tabCtrl;
    CEGUI::Window* defWnd;
    CEGUI::Window* tree;
    CEGUI::Window* btn;
    wnd->subscribeEvent("Opened",CEGUI::Event::Subscriber(OnOpenQuestUI));
    tabCtrl = wnd->getChild("Quest/Tab");
    tabCtrl->subscribeEvent(CEGUI::TabControl::EventSelectionChanged,
        CEGUI::SubscriberSlot(OnQuestTabSelectChanged));

    //"Quest/Tab/Normal"��"Quest/Tab"���ӽڵ���ӽڵ㣬����Ҫ�ݹ�
    defWnd = tabCtrl->getChildRecursive("Quest/Tab/Normal");
    tree = defWnd->getChild("Quest/Tab/Normal/Tree");
    tree->subscribeEvent(CEGUI::Tree::EventSelectionChanged,
        CEGUI::SubscriberSlot(OnNormalQuestTreeSelectChanged));

    tabCtrl->subscribeEvent("UQuestText",CEGUI::SubscriberSlot(OnUpdateQuestSelectText));

    tree->subscribeEvent("AddNormalQuest",
        CEGUI::SubscriberSlot(OnNormalQuestTreeAdded));

    tree->subscribeEvent("DeleteNormalQuest",
        CEGUI::SubscriberSlot(OnNormalQuestTreeDeleted));

    //������ǩ
    //...   

    btn = wnd->getChild("Quest/Delete");
    btn->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::SubscriberSlot(OnRemoveQuestButtonClicked));

    btn = wnd->getChild("Quest/Share");
    btn->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::SubscriberSlot(OnShareQuestButtonClicked));

    btn = wnd->getChild("Quest/Draw");
    btn->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::SubscriberSlot(OnDrawAwardButtonClicked));

    //��������viaibleΪfalse�������ڵ�һ�ΰ���ݼ�"Q"�͵�������
    wnd->setVisible(false);
	/////////////////////////////////////////////////
	// zhaohang  2010/6/29 
	// ������������
	wnd->subscribeEvent(QUEST_TEAMFUNC_UPDATE,CEGUI::SubscriberSlot(OnUpdateTeamQuestList));
	wnd->subscribeEvent(QUEST_TEAMFUNC_UPDATERUNNING,CEGUI::SubscriberSlot(OnUpdateTeamQuestRunning));
	////////////////////////////////////////////////
    return wnd;
}

bool OnOpenQuestUI(const CEGUI::EventArgs& Args)
{
    //��ʼ������
    ////����ж���,����һ����������������
    //QuestMsgMgr& qmm = GetInst(QuestMsgMgr);
    //qmm.Send_TeamQuestUpdate(0);
    CEGUI::Window * tabCtrl = GetWndMgr().getWindow("Quest/Tab");
    CEGUI::Window * defWnd = tabCtrl->getChildRecursive("Quest/Tab/Normal");
    CEGUI::Window * tree = defWnd->getChild("Quest/Tab/Normal/Tree");

    static_cast<CEGUI::Tree*>(tree)->resetList();

    CPlayer* player = GetGame()->GetMainPlayer();
    std::list<CQuestRecord*>* quests = player->GetRoleQueue()->GetQuestRecordList();
    for (std::list<CQuestRecord*>::iterator it = quests->begin();
        it != quests->end(); ++it)
    {
        CQuestRecord* questRecord = *it;
        if (NULL == questRecord)
            continue;

        AddQuest_impl(dynamic_cast<CEGUI::Tree*>(tree), questRecord);
    }
    //��һ�δ򿪽��棬��ʾindexΪ0�ı�ǩ��
    if (dynamic_cast<CEGUI::TabControl*>(tabCtrl)->getTabCount() > 0)
    {
        dynamic_cast<CEGUI::TabControl*>(tabCtrl)->setSelectedTabAtIndex(0);
        OnQuestTabSelectChanged(CEGUI::WindowEventArgs(tabCtrl));
    }
    return true;
}

//������������񡱰�ť
bool OnRemoveQuestButtonClicked(const CEGUI::EventArgs& args)
{
    //�ҵ�ѡ�е�����
    CEGUI::Tree* tree = dynamic_cast<CEGUI::Tree*>(GetWndMgr().getWindow("Quest/Tab/Normal/Tree"));
    if (NULL == tree)
        return true;

    CEGUI::TreeItem* item = tree->getFirstSelectedItem();
    if (NULL == item)
        return true;

    //�����������Ϣ����ɾ��������
    uint questId = static_cast<CQuestRecord*>(item->getUserData())->GetQuestID();
    GetInst(QuestMsgMgr).Send_Disband(questId);

    return true;
}


//������������񡱰�ť
//��ʱ�������
bool OnShareQuestButtonClicked(const CEGUI::EventArgs& args)
{
    //�ҵ�ѡ�е�����
    CEGUI::Tree* tree = dynamic_cast<CEGUI::Tree*>(GetWndMgr().getWindow("Quest/Tab/Normal/Tree"));
    if (NULL == tree)
        return true;

    CEGUI::TreeItem* item = tree->getFirstSelectedItem();
    if (NULL == item)
        return true;

    //�����������Ϣ�����������
	//......
	uint questId = static_cast<CQuestRecord*>(item->getUserData())->GetQuestID();
	GetInst(QuestMsgMgr).Send_Shared(questId);
    return true;
}


//�������ȡ��������ť
//�о������ť��Ӧ�������������֣�Ӧ������NPC�Ի�ʱ��ȡ��
bool OnDrawAwardButtonClicked(const CEGUI::EventArgs& args)
{
    return true;
}


//��ͨ������ɾ��һ��
bool OnNormalQuestTreeDeleted(const CEGUI::EventArgs &args)
{
    //�ҵ�������
    CEGUI::Tree* tree = dynamic_cast<CEGUI::Tree*>(WEArgs(args).window);
    if (NULL == tree)
        return true;

    //�ҵ���ɾ��������
    CQuestRecord* questRecord = CQuestSystem::GetTempQuestRecord();
    if( QuestUI::g_CurrRecord == questRecord )
         QuestUI::g_CurrRecord = NULL;

    CEGUI::TreeItem* item = tree->findFirstItemWithID(questRecord->GetQuestID());

    //ɾ��������
    tree->removeItem(item);
	//��������������
	CEGUI::Window* wnd = tree->getParent()->getChildRecursive(1);
	if(wnd)
		wnd->setText("");

    static CEGUI::Window * TextWindow = GetWndMgr().getWindow("Quest/Tab/Normal/Text");
    TextWindow->setText( "" );
    
    return true;
}


//��ͨ���������һ��
bool OnNormalQuestTreeAdded(const CEGUI::EventArgs& args)
{
    //�ҵ�����ͨ������
    CEGUI::Tree* tree = dynamic_cast<CEGUI::Tree*>(WEArgs(args).window);
    if (NULL == tree)
        return true;

    //�ҵ�����ӵ�����
    CQuestRecord* questRecord = CQuestSystem::GetTempQuestRecord();

    //��ӵ���������
    AddQuest_impl(tree, questRecord);

    return true;
}

bool OnUpdateQuestSelectText(const CEGUI::EventArgs& Args)
{
    if ( !QuestUI::g_CurrRecord )
        return false;

    CQuestRecord* questRecord = QuestUI::g_CurrRecord;
    long questId = questRecord->GetQuestID();
    const CQuest* quest = CQuestSystem::GetQuest(questId);

    std::string  QuestDes;

    /// �������� 
    QuestDes = AppFrame::LoadQuestTextRes(questId,eQuestDes); 
    /// ����Ŀ��
    const CQuestTrace * trace =quest->GetStep(questRecord->GetQuestStep())->GetQuestTraceHead();
    while (  trace !=NULL )
    {
        char str[64];
        long ltraceNum = 0 ;
        string strID = trace->GetGoodsStrID();
        // �����׷�ٵ�Ŀ����ԭʼ������ʾ��Ҫ����ұ������Ҹ���Ʒ������Ȼ�����ø���׷����������
        if(!strID.empty())
        {
            CGoodsList::tagGoods2* ptgGoods = CGoodsList::GetProperty(strID.c_str());
            if(ptgGoods)
                ltraceNum = GetGame()->GetMainPlayer()->GetGoodsAmount(ptgGoods->BaseProperty.dwIndex);
        }
        // �������
        else
            ltraceNum =  questRecord->GetParam( trace->GetParam() );
        QuestDes += AppFrame::GetText(trace->GetDescIndex()) ;// CStringReading::LoadText( QuestRet(questId),trace->GetDescIndex() );
        sprintf_s(str,"     %d/%d   %s",ltraceNum,trace->GetSum(),ltraceNum == trace->GetSum()?"<���>":"");
        QuestDes += str;     
        QuestDes += "[BR][BR]";
        trace = trace->next;
    }

    /// ������
    QuestDes+= CQuestEncourMan::GetAwardText(questId);

    static CEGUI::Window * TextWindow = GetWndMgr().getWindow("Quest/Tab/Normal/Text");

    TextWindow->setText( ToCEGUIString(QuestDes.c_str()) );
    return true;

}

//�ı���ͨ��������ѡ����Ŀ
bool OnNormalQuestTreeSelectChanged(const CEGUI::EventArgs& args)
{
    //�õ���ǰѡ�����
    const CEGUI::TreeEventArgs& treeArgs = static_cast<const CEGUI::TreeEventArgs&>(args);
    CEGUI::TreeItem* item = treeArgs.treeItem;
    if (NULL == item)
        return true;

    CQuestRecord* questRecord = (CQuestRecord*)item->getUserData();
    if (NULL == questRecord)
        return true;

    QuestUI::g_CurrRecord = questRecord;

    long questId = questRecord->GetQuestID();
    const CQuest* quest = CQuestSystem::GetQuest(questId);

    std::string  QuestDes;

    /// ��������   //CStringReading::LoadText( QuestRet(questId),quest->GetStep(questRecord->GetQuestStep())->GetText(5));
    QuestDes = AppFrame::LoadQuestTextRes(questId,eQuestDes); //%s
    /// ����Ŀ��
    const CQuestTrace * trace =quest->GetStep(questRecord->GetQuestStep())->GetQuestTraceHead();
    while (  trace !=NULL )
    {
        char str[64];
        long ltraceNum = 0 ;
        string strID = trace->GetGoodsStrID();
        // �����׷�ٵ�Ŀ����ԭʼ������ʾ��Ҫ����ұ������Ҹ���Ʒ������Ȼ�����ø���׷����������
        if(!strID.empty())
        {
            CGoodsList::tagGoods2* ptgGoods = CGoodsList::GetProperty(strID.c_str());
            if(ptgGoods)
                ltraceNum = GetGame()->GetMainPlayer()->GetGoodsAmount(ptgGoods->BaseProperty.dwIndex);
        }
        // �������
        else
           ltraceNum =  questRecord->GetParam( trace->GetParam() );
        QuestDes += AppFrame::GetText(trace->GetDescIndex()) ;// CStringReading::LoadText( QuestRet(questId),trace->GetDescIndex() );
        sprintf_s(str,"     %d/%d   %s",ltraceNum,trace->GetSum(),ltraceNum == trace->GetSum()?"<���>":"");
        QuestDes += str;     
        QuestDes += "[BR][BR]";
        trace = trace->next;
    }

    /// ������
    QuestDes+= CQuestEncourMan::GetAwardText(questId);

    static CEGUI::Window * TextWindow = GetWndMgr().getWindow("Quest/Tab/Normal/Text");

    TextWindow->setText( ToCEGUIString(QuestDes.c_str()) );
    return true;
}

bool OnClearQuestTreeSelectText(const CEGUI::EventArgs & Args)
{
    static CEGUI::Window * TextWindow = GetWndMgr().getWindow("Quest/Tab/Normal/Text");

    TextWindow->setText( "" );

    QuestUI::g_CurrRecord = NULL;
    return true;
}


//�ı��������ͱ�ǩ��ѡ����Ŀ
bool OnQuestTabSelectChanged(const CEGUI::EventArgs& args)
{
    CEGUI::TabControl* tab = dynamic_cast<CEGUI::TabControl*>(WEArgs(args).window);
    if (NULL == tab)
        return true;
    size_t idx = tab->getSelectedTabIndex();
    CEGUI::Window* wnd = tab->getTabContentsAtIndex(idx);

    if (wnd->getName() == "Quest/Tab/Normal")
    {
        CEGUI::Tree* tree = dynamic_cast<CEGUI::Tree*>(wnd->getChild("Quest/Tab/Normal/Tree"));
        if (NULL == tree)
            return true;

        tree->show();
        return true;
    }
	else if ( wnd->getName() == "Quest/Tab/Team")
	{
		CEGUI::Tree* tree = dynamic_cast<CEGUI::Tree*>(wnd->getChild("Quest/Tab/Team/Tree"));
		if(NULL == tree)
			return true;
		//tree->show();
		return true;
	}
    else
    {
        return false;
    }
}


//�������������һ�������¼
bool AddQuest_impl(CEGUI::Tree* tree, CQuestRecord* questRecord)
{
    if (NULL == tree || NULL == questRecord)
        return false;

    long questId = questRecord->GetQuestID();
    const CQuest* quest = CQuestSystem::GetQuest(questId);
    const char* name = quest->GetAttribute().GetQuestName();
    CEGUI::TreeItem* item = new CEGUI::TreeItem( ToCEGUIString(name) );
    tree->addItem(item);
    item->setID(questId);
    item->setUserData(questRecord);
    item->setSelectionBrushImage(IMAGES_FILE_NAME, BRUSH_NAME);
    return true;
}

bool OnUpdateTeamQuestList(const CEGUI::EventArgs& e)
{
	return true;
}
bool OnUpdateTeamQuestRunning(const CEGUI::EventArgs& e)
{
	return true;
}