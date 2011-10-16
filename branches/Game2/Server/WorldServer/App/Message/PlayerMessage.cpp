#include "StdAfx.h"
#include "../Player.h"
#include "../Goods/Goods.h"
#include "../Linkman/LinkmanSystem.h"
#include "../OrganizingSystem/OrganizingParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnPlayerMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_S2W_PLAYER_ADDSKILL://gm���������������һ������,����Ϣת�����������ӵ�gameserver
		{
			DWORD dwGSIndex=pMsg->GetGSID();
			pMsg->SetType(MSG_W2S_OTHER_ADDSKILL);
			pMsg->Add(dwGSIndex);
			pMsg->SendToAllGS();			
		}
		break;
	case MSG_S2W_PLAYER_DELGOODS://gm�������ɾ��һ����ҵ���Ʒ,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_DELGOODS);
			pMsg->SendToAllGS();
		}
		break;
	case MSG_S2W_PLAYER_DELSKILL://gm�������ɾ��һ����ҵļ���,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_DELSKILL);
			pMsg->SendToAllGS();
		}
		break;
	case MSG_S2W_PLAYER_SETLEVEL://gm�����������һ����ҵĵȼ�,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_SETLEVEL);
			pMsg->SendToAllGS();
		}
		break;
	case MSG_S2W_PLAYER_OCCU_CHANGE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			LONG lNewOccu = pMsg->GetLong();
			CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
			if (NULL != pPlayer)
			{
				pPlayer->SetOccupation((BYTE)lNewOccu);
				LinkmanSystem::GetInstance().OnPlayerOccuChange(pPlayer);
				GetOrganiCtrl()->OnPlayerOccuChange(pPlayer, lNewOccu);
			}
			
		}
		break;
	}
}
