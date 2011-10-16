#include "StdAfx.h"
#include "../UIDef.h"

#include "../GamePage/Package/PackagePage.h"
#include "../GamePage/ChatList/ChatListPage.h"

#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "../../GameClient/ClientApp/MainPlayerHand.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"

CEGUI::Window* InitPlayerShopMessageBoardPage(void);							///< ��ʼ�������̵����԰����
void SubscribePlayerShopMessageBoardPageEvent(CEGUI::Window* mainPage);			///< �����̵���Ʒ���԰�����¼�ע��

CEGUI::Window* InitPlayerShopMessageBoardPage(void)
{
#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("PlayerShopSaleNoter.log");
#endif

	CEGUI::Window* wnd = LoadUI("PlayerShopMessageBoard");

#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLogFilename("CEGUI.log",true);
#endif

	SubscribePlayerShopMessageBoardPageEvent(wnd);

	wnd->setVisible(false);
	return wnd;
}

void SubscribePlayerShopMessageBoardPageEvent(CEGUI::Window* mainPage)
{

}