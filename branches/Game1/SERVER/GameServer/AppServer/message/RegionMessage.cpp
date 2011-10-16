#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"
#include "..\area.h"
#include "..\ServerRegion.h"
#include "..\player.h"
#include "..\Monster.h"
#include "..\goods\cgoods.h"


// ��Ӧ������Ϣ
void CServerRegion::OnMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_C2S_RGN_ENTER:
		{
			CPlayer* pPlayer = pMsg->GetPlayer();
			long lUseGoods = pMsg->GetLong();
			pPlayer->SetClientIP(pMsg->GetIP());
	
			//��ҽ��볡��
			//PlayerEnter(pPlayer,lUseGoods);
		}
		break;

	// ����뿪��Ϸ����
	case MSG_C2S_RGN_EXIT:
		{
			CPlayer* pPlayer = GetGame()->FindPlayer(pMsg->GetPlayerID());
			if (NULL == pPlayer)
			{
				//! ��ʱ���󲻸ñ�����
				throw;
			}

			//if(NULL_GUID == pPlayer->GetTeamID()) break;
			//CMessage msg(MSG_S2W_TEAM_KICK_PLAYER);
			//msg.Add(pMsg->GetPlayerID());
			//msg.Add(pMsg->GetPlayerID());
			//msg.Send();	
		}
		break;
	}
}
