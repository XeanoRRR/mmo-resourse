#include "stdafx.h"
#include "../../../GameClient/ClientApp/ClientRegion.h"
#include "../../../GameClient/ClientApp/Player.h"
#include "../../../Net/Message.h"
#include "../../../GameClient/Game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnCountryMessage(CMessage* pMsg)
{
	CPlayer* pPlayer=GetGame()->GetMainPlayer();
	switch(pMsg->GetType())
	{
	case MSG_S2C_COUNTRY_CHANGE:
		{
			BYTE ret = pMsg->GetByte();

			// ������
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
	
			if( pPlayer->GetExID() == PlayerID ) // ����
			{
				switch(ret)
				{
				case -1:
					
					break;
				case -2:
					
					break;
				case -3:
					
					break;
				case -4:			// ʧ��
					
					break;
				case -5:			// ʧ��
					
					break;

				case 1:			// ��ʾ���������ɹ�
				case 2:
				case 3:
				case 4:
					pPlayer->SetCountry(ret);
					
					break;
				default:			// ʧ��
					
					break;
				}
			}
			else // ������
			{
				CPlayer *pShowPlayer =(CPlayer*) GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);

				if(pShowPlayer)
				{
					if(ret >= 1 && ret <= 4)
					{
						pShowPlayer->SetCountry(ret);
					}
				}
			}
		}
		break;
	}
}
	