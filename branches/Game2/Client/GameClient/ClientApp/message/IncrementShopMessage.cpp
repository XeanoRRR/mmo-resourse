#include "stdafx.h"
#include "Message.h"
#include "Other/AudioList.h"
#include "../GameClient/Game.h"
#include "../ShopCity/ShopCitySys.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnIncrementShopMessage(CMessage *pMsg)
{
	ShopCityMsgMgr& mgr = GetInst(ShopCityMsgMgr);
	switch(pMsg->GetType()) 
	{
	case MSG_S2C_INCREMENTSHOP_GETLOG_RESPONSE:
		{
			mgr.Receive_LogResponse(pMsg);
		}
		break;

	case MSG_S2C_INCREMENTSHOP_AFFICHE_RESPONSE:
		{
			mgr.Receive_AfficheResponse(pMsg);
		}
		break;

	case MSG_S2C_INCREMENTSHOP_OPEN:	//gameserver֪ͨ�ͻ��˴��̵����
		{
			mgr.Receive_PageOpen(pMsg);
		}
		break;
	case MSG_S2C_INCREMENTSHOP_CLOSE:
		{
			mgr.Receive_PageClose(pMsg);
		}
		break;
	case MSG_S2C_INCREMENTSHOP_BUY: //gameserver���ش��̵�����һ����Ʒ�Ľ��
		{
			mgr.Receive_BuyResult(pMsg);
		}
		break;
	case MSG_S2C_INCREMENTSHOP_UPDATA: // ��������
		{
			mgr.Receive_Updata(pMsg);
		}
		break;
	}
}