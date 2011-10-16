#include "StdAfx.h"
#include "../Area.h"
#include "../ServerRegion.h"
#include "../Player.h"
#include "../Monster.h"
#include "../Goods/CGoods.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
