#include "stdafx.h"
#include "..\nets\networld\message.h"
#include "..\nets\Servers.h"
#include "..\goods\cgoods.h"
#include "..\..\worldserver\game.h"
#include "..\player.h"
#include "../Linkman/LinkmanSystem.h"
#include "..\Organizingsystem\OrganizingParam.h"



void OnPlayerMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_S2W_PLAYER_ADDSKILL://gm���������������һ������,����Ϣת�����������ӵ�gameserver
		{
			DWORD dwGSIndex=pMsg->GetGSID();
			pMsg->SetType(MSG_W2S_OTHER_ADDSKILL);
			pMsg->Add(dwGSIndex);
			pMsg->SendAll();			
		}
		break;
	case MSG_S2W_PLAYER_DELGOODS://gm�������ɾ��һ����ҵ���Ʒ,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_DELGOODS);
			pMsg->SendAll();
		}
		break;
	case MSG_S2W_PLAYER_DELSKILL://gm�������ɾ��һ����ҵļ���,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_DELSKILL);
			pMsg->SendAll();
		}
		break;
	case MSG_S2W_PLAYER_SETLEVEL://gm�����������һ����ҵĵȼ�,����Ϣת�����������ӵ�gameserver
		{
			pMsg->SetType(MSG_W2S_OTHER_SETLEVEL);
			pMsg->SendAll();
		}
		break;
	case MSG_S2W_PLAYER_OCCU_CHANGE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			LONG lNewOccu = pMsg->GetLong();
			CPlayer *pPlayer = GetGame()->GetMapPlayer(PlayerGuid);
			if (NULL != pPlayer)
			{
				pPlayer->SetOccupation(lNewOccu);
				LinkmanSystem::GetInstance().OnPlayerOccuChange(pPlayer);
				GetOrganiCtrl()->OnPlayerOccuChange(pPlayer, lNewOccu);
			}
			
		}
		break;
	}
}
