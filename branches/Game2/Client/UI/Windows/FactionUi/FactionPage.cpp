#include "StdAfx.h"  
#include "FactionPageDef.h"
#include "../../UIDef.h"  
#include "../../CEGUI/cegui/include/CEGUIWindowManager.h"
#include "../../CEGUI/cegui/include/CEGUIWindow.h"   
#include "../../../GameClient/Game.h"
#include "../../../GameClient/ClientApp/Player.h"
#include "../../UIManager.h"
#include "../../../GameClient/ClientApp/organizingsystem/Faction.h"
#include "../../../GameClient/ClientApp/organizingsystem/FactionPageOper.h"
#include "../../../GameClient/ClientApp/organizingsystem/Union.h"

bool  OnOpenFactionPage(const CEGUI::EventArgs& e); 
bool  OnUpdateFaction(const CEGUI::EventArgs& e);
bool  OnCheckFactionApply(const CEGUI::EventArgs& e);
bool  OnCheckFactionExp(const CEGUI::EventArgs& e);
bool  OnExitFaction(const CEGUI::EventArgs& e);
bool  OnCheckFactionQuest(const CEGUI::EventArgs& e);
bool  OnFactionSet(const CEGUI::EventArgs& e);
bool  OnOpenFactionList(const CEGUI::EventArgs& e); 
bool  OnDbClickFactionList(const CEGUI::EventArgs& e);
bool  OnBuOk(const CEGUI::EventArgs& e); 
bool  OnBuCancel(const CEGUI::EventArgs& e);
bool  OnFactionClosed(const CEGUI::EventArgs& e);
bool  OnOpenMemInfoPage(const CEGUI::EventArgs& e);

namespace nFactionMemInfo
{
    extern CFacMember*  g_FacMemInfo;
}

CEGUI::Window*  InitFactionPage()
{
    CEGUI::Window * factionPage = LoadUI("FactionPage");
    factionPage->setVisible(false);
    factionPage->setAlwaysOnTop(true);

    factionPage->setPosition( CEGUI::UVector2(cegui_reldim(0.3f),cegui_reldim(0.2f)));
    
    factionPage->subscribeEvent("Opened",CEGUI::Event::Subscriber(OnOpenFactionPage) );
   
    factionPage->subscribeEvent("Closed",CEGUI::Event::Subscriber(OnFactionClosed) );

    factionPage->subscribeEvent("UpdateMem",CEGUI::Event::Subscriber(OnUpdateFaction) );

    factionPage->subscribeEvent("UpdateFacAppList",CEGUI::Event::Subscriber(OnCheckFactionApply));

    
    /// ���水ť����
    CEGUI::PushButton * ExitBt = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionControl/TabGroup/MemList/ExitFac"));
    if ( ExitBt )
    {
        ExitBt->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnExitFaction));
    }
    CEGUI::PushButton * ApplyBt  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionControl/TabGroup/MemList/ApplyList"));
    if ( ApplyBt )
    {
        ApplyBt->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnCheckFactionApply));
    }
    CEGUI::PushButton * OpenBt = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionControl/TabGroup/MemList/DetailInfo") );
    if ( OpenBt )
    {
        OpenBt->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnOpenMemInfoPage));
    }
    CEGUI::MultiColumnList * mclbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionControl/FactionPage/MemList"));
    if ( mclbox )
    {
        mclbox->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(OnOpenMemInfoPage));
    }
    /*
    CEGUI::PushButton * ExpBt  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionPage/FactionExp"));
    if ( ExpBt )
    {
        ExpBt->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnCheckFactionExp));
    }
    
    CEGUI::PushButton * QuestBt= static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionPage/FactionQuest"));
    if ( QuestBt )
    {
        QuestBt->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnCheckFactionQuest));
    }
    CEGUI::PushButton * SetBt  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionPage/FactionSet"));
    if ( SetBt )
    {
        OpenBt->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnFactionSet));
    }

    /// ȷ��
    CEGUI::PushButton * BuOk  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionPage/BuOk"));
    if ( BuOk )
    {
        BuOk->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnBuOk));
    }
    /// ȡ��
    CEGUI::PushButton * BuCancel  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionPage/BuCancel"));
    if ( BuCancel )
    {
        BuCancel->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(OnBuCancel));
    }
    */

    return factionPage;
}
/*

CEGUI::Window*  InitFactionList()
{
    CEGUI::Window * factionList = LoadUI("FactionList");
    factionList->setVisible(false);
    factionList->setAlwaysOnTop(true);

    factionList->setPosition( CEGUI::UVector2(cegui_reldim(0.3f),cegui_reldim(0.2f)));

    factionList->subscribeEvent("Opened",CEGUI::Event::Subscriber(OnOpenFactionList) );

    CEGUI::Listbox * flist = static_cast<CEGUI::Listbox*>(GetWndMgr().getWindow("FactionList/FactionList"));
    flist->subscribeEvent(CEGUI::Listbox::EventMouseDoubleClick,CEGUI::Event::Subscriber(OnDbClickFactionList));

    return factionList;
}

bool  OnOpenFactionList(const CEGUI::EventArgs& e)
{
    CEGUI::Listbox * flist = static_cast<CEGUI::Listbox*>(GetWndMgr().getWindow("FactionList/FactionList"));
    flist->setVisible(true);
    flist->resetList();
    if ( flist )
    {
         CFactionManager* FactionMan = GetGame()->GetFactionManager();
         if ( FactionMan )
         {
             const CEGUI::Image* cov_img = &CEGUI::ImagesetManager::getSingleton().get("TaharezLook").getImage("CheckboxMark");
             std::list<tagApplyInfo>*  Apply = FactionMan->GetListOfApplyInfo();
             int i = 0;
             for ( std::list<tagApplyInfo>::iterator it = Apply->begin() ; it != Apply->end() ; it++,i++ )
             {
                 char str[1024];

                 sprintf_s( str,1024,"%s--(%d/%d)",(*it).szName,(*it).lMemberNum,(*it).lMaxMemberNum);
                 CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(str);
                 item->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
                 item->setCoverBrushImage(cov_img);
                 item->setUserData((void*)&(*it));

                 flist->addItem(item);
             }
         }
    }
    flist->activate();
    return true;
}
*/

bool  OnFactionClosed(const CEGUI::EventArgs& e)
{
    GetInst(FactionOper).SetVisible();
    return true;
}

bool  OnOpenMemInfoPage(const CEGUI::EventArgs& e)
{
    CEGUI::MultiColumnList * mlist =  static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionControl/FactionPage/MemList"));
    if ( mlist )
    {
        CEGUI::ListboxTextItem * sel =  static_cast<CEGUI::ListboxTextItem*>( mlist->getFirstSelectedItem() );
        if( !sel )    return false;
        CFacMember* SelMem = static_cast<CFacMember*>( sel->getUserData() );
        if( !SelMem ) return false;

        nFactionMemInfo::g_FacMemInfo = SelMem ;

        FireUIEvent("FactionMemInfo","Opened");
    }
    return true;
}

bool  OnDbClickFactionList(const CEGUI::EventArgs& e)
{
    CEGUI::Listbox* lb = WListBox(WEArgs(e).window);
    if(!lb)
        return false;
    CEGUI::ListboxItem* item = lb->getFirstSelectedItem();
    if(!item)
        return false;
    tagApplyInfo* apply = (tagApplyInfo*)item->getUserData();
    if(!apply)
        return false;

    GetInst(FactionOper).JoinFactionList(apply->guid);
    return true;
}

bool  OnOpenFactionPage(const CEGUI::EventArgs& e)
{
    CPlayer * player = GetGame()->GetMainPlayer();

    /// ����Ա�б�
    CEGUI::MultiColumnList * mclbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionControl/FactionPage/MemList"));
    mclbox->resetList();
    CEGUI::TabControl * TabCon = static_cast<CEGUI::TabControl*>( GetWndMgr().getWindow("FactionControl/TabGroup/MemList/Tab"));
    TabCon->setSelectedTabAtIndex(1);
    return true;
}

bool  OnUpdateFaction(const CEGUI::EventArgs& e)
{
    /// �����ı�
    CEGUI::PushButton * BuOk  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionControl/TabGroup/MemList/ApplyList"));
    /// ����Ա�б�
    CEGUI::MultiColumnList * mclbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionControl/FactionPage/MemList"));
    CEGUI::MultiColumnList * qlist = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionControl/TabGroup/MemList/Tab 5/MemList"));
    /// ��������
    CEGUI::DefaultWindow * stext = static_cast<CEGUI::DefaultWindow*>(GetWndMgr().getWindow("FactionPage/MemDes/MemDesText"));
    if ( mclbox )
    {
        if( CFaction* pMyFaction = GetGame()->GetFactionManager()->GetMyFaction() )
        {
            TCHAR szDesc[ 256 ];
            // ������������
            wsprintf( szDesc, AppFrame::GetText("FA_1001"), 
                pMyFaction->GetMaxNumOfMembers(),pMyFaction->GetNumOfMembers(),
                pMyFaction->GetNumOfOnlineMembers());
            stext->setText(ToCEGUIString(szDesc));

            // �����б���Ϣ
            std::list<CFacMember*>*	pApplyList = pMyFaction->GetListOfApply();
            // ������������
            wsprintf( szDesc, AppFrame::GetText("FA_118"),pApplyList ? pApplyList->size():0 );
            BuOk->setText(ToCEGUIString(szDesc));

            // �����б���Ϣ
            std::list<CFacMember*>*	plistMembers = pMyFaction->GetListOfMembers();
            long  i = 0;
            mclbox->resetList();
            qlist->resetList();
            for( std::list<CFacMember*>::iterator p = plistMembers->begin(); p != plistMembers->end(); p++,i++ )
            {
                mclbox->addRow();

                ///UpFactionPageList(mclbox, (*p)->GetOccuText(),4,i);
                UpFactionPageListAppData(mclbox, (*p)->GetName(),0,i,*p);
                UpFactionPageList(mclbox, (((*p)->GetJob()!=-1)?"��Ա":"����"),1,i);
                UpFactionPageList(mclbox, CEGUI::PropertyHelper::intToString((*p)->GetLevel()),2,i);
                UpFactionPageList(mclbox, "0",3,i);
                UpFactionPageList(mclbox, (*p)->GetOccuText(),4,i);   ///�����ƶ�Ϊְҵ
                UpFactionPageList(mclbox, (*p)->GetRegion(),5,i);
                UpFactionPageList(mclbox, "0",6,i);
                UpFactionPageList(mclbox, "0",7,i);
                UpFactionPageList(mclbox, "0",8,i);
                UpFactionPageList(mclbox, "0",9,i); 
                long lTime = (*p)->GetLastTime();
                time_t tTime = lTime;
                tm * t = localtime( &tTime );
                char pstr[512];
                sprintf_s(pstr,"%d/%d/%d %d:%d:%d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
                UpFactionPageList(mclbox,pstr,10,i);
            }
            
            i = 0 ;
            for ( std::list<CFacMember*>::iterator p = plistMembers->begin() ; p != plistMembers->end() ; p++,i++)
            {
                qlist->addRow() ;

                UpFactionPageListAppData(qlist, (*p)->GetName(),0,i,*p);
                UpFactionPageList(qlist, (((*p)->GetJob()!=-1)?"��Ա":"����"),1,i);
            }
        }
    }
    return true;
}

bool  OnCheckFactionApply(const CEGUI::EventArgs& e)
{                
    FireUIEvent("FactionApplyList","Opened");
    /*
    /// ����Ա�б�
    CEGUI::MultiColumnList * mclbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/MemberList"));
    mclbox->setVisible(false);
    /// ��������б�
    CEGUI::Listbox * qlist = static_cast<CEGUI::Listbox*>(GetWndMgr().getWindow("FactionPage/QuestList"));
    qlist->setVisible(false);
    CEGUI::DefaultWindow * stext = static_cast<CEGUI::DefaultWindow*>(GetWndMgr().getWindow("FactionPage/QuestInfo"));
    stext->setVisible(false);
    /// ��������б�
    CEGUI::MultiColumnList * appbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/ApplyList"));
    appbox->setVisible(true);
    /// ��ʾ��Ϣ
    CEGUI::DefaultWindow * VisInfo = static_cast<CEGUI::DefaultWindow*>(GetWndMgr().getWindow("FactionPage/MemInfo"));
    VisInfo->setVisible(true);
    /// ȷ��
    CEGUI::PushButton * BuOk  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionPage/BuOk"));
    BuOk->setText("ͬ������");
    /// ȡ��
    CEGUI::PushButton * BuCancel  = static_cast<CEGUI::PushButton*>( GetWndMgr().getWindow("FactionPage/BuCancel"));
    BuCancel->setText("�ܾ�");
    if ( appbox )
    {
        if( CFaction* pMyFaction = GetGame()->GetFactionManager()->GetMyFaction() )
        {
            // �����б���Ϣ
            std::list<CFacMember*>*	plistMembers = pMyFaction->GetListOfApply();
            long  i = 0;

            appbox->resetList();
            for( std::list<CFacMember*>::iterator p = plistMembers->begin(); p != plistMembers->end(); p++,i++ )
            {
                appbox->addRow();

                //UpFactionPageListAppData(appbox, (*p)->GetName(),0,i,*p);
                //UpFactionPageList(appbox, (*p)->GetOccuText(),1,i);
                //UpFactionPageList(appbox, CEGUI::PropertyHelper::intToString((*p)->GetLevel()),2,i);           
            }
        }
    }

    */
    return true;
}

bool  OnCheckFactionExp(const CEGUI::EventArgs& e)
{

    return true;
}

bool  OnExitFaction(const CEGUI::EventArgs& e)
{
    GetInst(FactionOper).ExitFaction();
    return true;
}

bool  OnCheckFactionQuest(const CEGUI::EventArgs& e)
{
    /// ����Ա�б�
    CEGUI::MultiColumnList * mclbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/MemberList"));
    mclbox->setVisible(false);
    /// ��������б�
    CEGUI::Listbox * qlist = static_cast<CEGUI::Listbox*>(GetWndMgr().getWindow("FactionPage/QuestList"));
    qlist->setVisible(true);
    CEGUI::DefaultWindow * stext = static_cast<CEGUI::DefaultWindow *>(GetWndMgr().getWindow("FactionPage/QuestInfo"));
    stext->setVisible(true);
    /// ��������б�
    CEGUI::MultiColumnList * appbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/ApplyList"));
    appbox->setVisible(false);
    /// ��ʾ��Ϣ
    CEGUI::DefaultWindow * VisInfo = static_cast<CEGUI::DefaultWindow*>(GetWndMgr().getWindow("FactionPage/MemInfo"));
    VisInfo->setVisible(false);

    return true;
}

bool  OnFactionSet(const CEGUI::EventArgs& e)
{

    return true;
}

bool  OnBuOk(const CEGUI::EventArgs& e)
{
    /// ����Ա�б�
    CEGUI::MultiColumnList * mclbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/MemberList"));
    /// ��������б�
    CEGUI::Listbox * qlist = static_cast<CEGUI::Listbox*>(GetWndMgr().getWindow("FactionPage/QuestList"));
    /// ��������б�
    CEGUI::MultiColumnList * appbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/ApplyList"));
    /// ɾ����Ա
    if ( mclbox->isVisible() )
    {
    }
    /// ��������
    else if ( qlist->isVisible() )
    {
    }
    /// ͬ��������
    else if ( appbox->isVisible() )
    {
       CEGUI::ListboxTextItem * sel =  static_cast<CEGUI::ListboxTextItem*>( appbox->getFirstSelectedItem() );
       if( !sel )    return false;
       CFacMember* SelMem = static_cast<CFacMember*>( sel->getUserData() );
       if( !SelMem ) return false;
       GetInst(FactionOper).PushApplyInfo( SelMem->GetGUID(),TRUE );
    }
    return true;
}

bool  OnBuCancel(const CEGUI::EventArgs& e)
{
    /// ����Ա�б�
    CEGUI::MultiColumnList * mclbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/MemberList"));
    /// ��������б�
    CEGUI::Listbox * qlist = static_cast<CEGUI::Listbox*>(GetWndMgr().getWindow("FactionPage/QuestList"));
    /// ��������б�
    CEGUI::MultiColumnList * appbox = static_cast<CEGUI::MultiColumnList*>(GetWndMgr().getWindow("FactionPage/ApplyList"));
    /// ɾ����Ա
    if ( mclbox->isVisible() )
    {
        CEGUI::ListboxTextItem * sel =  static_cast<CEGUI::ListboxTextItem*>( mclbox->getFirstSelectedItem() );
        if( !sel )    return false;
        CFacMember* SelMem = static_cast<CFacMember*>( sel->getUserData() );
        if( !SelMem ) return false;
        GetGame()->GetFactionManager()->SendKickMsg(SelMem->GetGUID());
    }
    /// ��������
    else if ( qlist->isVisible() )
    {
    }
    /// ͬ��������
    else if ( appbox->isVisible() )
    {
        CEGUI::ListboxTextItem * sel =  static_cast<CEGUI::ListboxTextItem*>( appbox->getFirstSelectedItem() );
        if( !sel )    return false;
        CFacMember* SelMem = static_cast<CFacMember*>( sel->getUserData() );
        if( !SelMem ) return false;
        GetInst(FactionOper).PushApplyInfo( SelMem->GetGUID(),FALSE );
    }
    return true;
}