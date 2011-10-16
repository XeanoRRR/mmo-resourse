/**
* @filename:TeamCaptainMenu.cpp
* @date: 2010/6/25
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ����Ҽ����������浯���˵�����
*/

#include "stdafx.h"
#include "../UIDef.h"
#include "TeamSetup.h"
#include "TeamDef.h"
#include "../../GameClient/Game.h"
#include "../../GameClient/GameControl.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "../../GameClient/ClientApp/Team/Team.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"

///��ʼ����������Ҽ��˵�����
CEGUI::Window* InitTeamCaptainMenu();
///ע������Ҽ��˵������¼�
void SubscriberTCMenuEvent(CEGUI::Window* pageWnd);
/// ���ö����Ҽ��˵�UI����
void SetTCMenuProperty(CEGUI::Window* pageWnd);

CEGUI::Window* GetTeamCaptainMenuWnd();

///�����ƶ�
bool OnFollowMove(const CEGUI::EventArgs& e);
///��������
bool OnBuyQuery(const CEGUI::EventArgs& e);
///˽������
extern bool OnPrivateChat(const CEGUI::EventArgs& e);
///��Ϊ����
bool OnAddFriend(const CEGUI::EventArgs& e);
///�����Ϣ
bool OnPlayerInfo(const CEGUI::EventArgs& e);
///��������
bool OnCopyName(const CEGUI::EventArgs& e);
///��Ϊ�ӳ�
bool OnSetCaptain(const CEGUI::EventArgs& e);
///�߳�����
bool OnKickOutOfTeam(const CEGUI::EventArgs& e);

///����λ��
bool OnAdjustPos(const CEGUI::EventArgs& e);
///�����Ƿ��Ƕӳ������˵�����
bool OnAdjustMenuContent(const CEGUI::EventArgs& e);

CEGUI::Window* InitTeamCaptainMenu()
{
	CEGUI::Window* pagewnd = LoadUI(TEAM_CAPTAINMENU_NAME);
	SetTCMenuProperty(pagewnd);
	SubscriberTCMenuEvent(pagewnd);
	return pagewnd;
}

void SubscriberTCMenuEvent(CEGUI::Window* pageWnd)
{
	if(!pageWnd)
		return;
	pageWnd->subscribeEvent(EVENT_OPEN,OnAdjustPos);
	pageWnd->subscribeEvent(EVENT_OPEN,OnAdjustMenuContent);

	CEGUI::MenuItem* follow = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/Follow"));
	CEGUI::MenuItem* buy  = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/BuyQuery"));
	CEGUI::MenuItem* pri = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/PriChat"));
	CEGUI::MenuItem* AddFri = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/AddFriend"));
	CEGUI::MenuItem* playerInfo = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/PlayerInfo"));
	CEGUI::MenuItem* copyName = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/CopyName"));
	CEGUI::MenuItem* setCap = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/SetCaption"));
	CEGUI::MenuItem* kick = WMenuItem(pageWnd->getChildRecursive("TeamCaptainMenu/Kick"));

	if(pri != NULL)
		pri->subscribeEvent(CEGUI::MenuItem::EventClicked,OnPrivateChat);
	if(follow != NULL)
		follow->subscribeEvent(CEGUI::MenuItem::EventClicked,OnFollowMove);
	if ( buy != NULL )
		buy->subscribeEvent(CEGUI::MenuItem::EventClicked,OnBuyQuery);
	if(AddFri != NULL)
		AddFri->subscribeEvent(CEGUI::MenuItem::EventClicked,OnAddFriend);
	if(playerInfo != NULL)
		playerInfo->subscribeEvent(CEGUI::MenuItem::EventClicked,OnPlayerInfo);
	if(copyName != NULL)
		copyName -> subscribeEvent(CEGUI::MenuItem::EventClicked,OnCopyName);
	if(setCap != NULL)
		setCap->subscribeEvent(CEGUI::MenuItem::EventClicked,OnSetCaptain);
	if(kick != NULL)
		kick->subscribeEvent(CEGUI::MenuItem::EventClicked,OnKickOutOfTeam);
}

void SetTCMenuProperty(CEGUI::Window* pageWnd)
{
	if(!pageWnd)
		return;
}
CEGUI::Window* GetTeamCaptainMenuWnd()
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	return mgr.getWindow(TEAM_CAPTAINMENU_NAME);
}

bool OnAdjustPos(const CEGUI::EventArgs& e)
{
	CEGUI::Window* pageWnd = WEArgs(e).window;
	//CEGUI::MouseCursor& mc = CEGUI::MouseCursor::getSingleton();
	//CEGUI::Point& pt = mc.getPosition();
	//CEGUI::Window* wnd = GetTeamCaptainMenuWnd();
	//wnd->setPosition(CEGUI::UVector2(
	//	cegui_absdim(pt.d_x),
	//	cegui_absdim(pt.d_y))
	//	);
	char name[256];
	TeamData& td = GetInst(TeamData);
	TeamSetup& ts = GetInst(TeamSetup);
	uint index = td.GetSelMember();
	sprintf(name,ts.GetItemWndName(),index);
	CEGUI::Window* TeamWnd =  GetWndMgr().getWindow(TEAM_PAGE_NAME);
	CEGUI::Window* itemWnd = TeamWnd->getChildRecursive(name);
	CEGUI::UVector2 TeamPos = TeamWnd->getPosition();
	CEGUI::UVector2 itempos = itemWnd->getPosition();
	CEGUI::UVector2 size = itemWnd->getSize();
	float x = TeamPos.d_x.d_offset + itempos.d_x.d_offset + size.d_x.d_offset;
	float y = TeamPos.d_y.d_offset + itempos.d_y.d_offset ;
	pageWnd->setPosition(CEGUI::UVector2(
		cegui_absdim(x),
		cegui_absdim(y))
		);
	pageWnd->notifyScreenAreaChanged(true);
	return true;
}

bool OnAdjustMenuContent(const CEGUI::EventArgs& e)
{
	CEGUI::Window* pagewnd = WEArgs(e).window;
	CEGUI::Window* wnd1 = pagewnd->getChildRecursive("TeamCaptainMenu/SetCaption");
	CEGUI::Window* wnd2 = pagewnd->getChildRecursive("TeamCaptainMenu/Kick");
	CPlayer* player = GetGame()->GetMainPlayer();
	bool IsCaptain = (player->GetCaptainID() == player->GetExID());
	if(!IsCaptain)//���Ƕӳ�
	{
		wnd1->setEnabled(false);
		wnd2->setEnabled(false);
	}
	else
	{
		wnd1->setEnabled(true);
		wnd2->setEnabled(true);
	}
	return true;
}

bool OnFollowMove(const CEGUI::EventArgs& e)
{
	TeamData& td = GetInst(TeamData);
	const CGUID& ID = td.GetSelMemberGUID();
	if(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,ID))
	{
		GetGame()->GetGameControl()->SetMPActionMode(CGameControl::MPAM_AutoMove);
		GetGame()->GetGameControl()->SetAutoMoveMode(CGameControl::AMM_FOLLOW_OBJECT);
		GetGame()->GetGameControl()->FollowObject(TYPE_PLAYER,ID,1);
	}
	else
	{
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("TM_40"));
	}
	return true;
}
bool OnBuyQuery(const CEGUI::EventArgs& e)
{
	return true;
}
bool OnAddFriend(const CEGUI::EventArgs& e)
{
	TeamData& td = GetInst(TeamData);
	TeamMsgMgr& tmgr = GetInst(TeamMsgMgr);
	//tmgr.SendAddLinkMan();
	return true;
}

bool OnPlayerInfo(const CEGUI::EventArgs& e)
{
	return true;
}
bool OnCopyName(const CEGUI::EventArgs& e)
{
	return true;
}

bool OnSetCaptain(const CEGUI::EventArgs& e)
{
	TeamData& td = GetInst(TeamData);
	TeamMsgMgr& tmgr = GetInst(TeamMsgMgr);
	const CGUID& guid = td.GetSelMemberGUID();
	tmgr.SendSetCaptain(guid);
	return true;
}

bool OnKickOutOfTeam(const CEGUI::EventArgs& e) 
{
	TeamData& td = GetInst(TeamData);
	TeamMsgMgr& tmgr = GetInst(TeamMsgMgr);
	const CGUID& guid = td.GetSelMemberGUID();
	tmgr.SendLeaveTeam(guid);
	return true;
}

