#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\player.h"
#include "..\..\gameserver\game.h"
#include "..\country\country.h"
#include "..\country\countryparam.h"
#include "..\country\countryHandler.h"
#include "..\PlayerMan\PlayerMan.h"



void OnCountryMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_W2S_COUNTRY_CHANGE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			long lRet = pMsg->GetLong();
			BYTE btCountry = pMsg->GetByte();
			CPlayer* pPlayer = GetGame()->FindPlayer( PlayerGuid );
			if(!pPlayer) return;

			// ����ɹ�gs���ܻ�ͬ��һ�����ݣ��������Ҫͬ���������Ҫ�ֶ���������
			///////////////

			if(lRet >= 1 && lRet <= 4)
			{
				pPlayer->SetCountry((BYTE)lRet);
			}

			CMessage msg(MSG_S2C_COUNTRY_CHANGE);
			msg.Add(lRet);
			msg.Add(PlayerGuid);
			msg.SendToAround(pPlayer);

			// Fox@20081110 ˢ�½�ɫ����
			GetInst(CPlayerMan).RefreshElem(PET_COUNTRY, pPlayer);
		}
		break;

	case MSG_W2S_COUNTRY_POWER_CHANGE:
		{
			BYTE btCid = pMsg->GetByte();
			long lPow = pMsg->GetLong();

			CCountry *pCountry = GetCountryHandler()->GetCountry(btCid);
			if(!pCountry) return;

			pCountry->SetPower( lPow );
		}
		break;

	case MSG_W2S_COUNTRY_TECHLEVEL_CHANGE:
		{
			BYTE btCid = pMsg->GetByte();
			long ln = pMsg->GetLong();

			CCountry *pCountry = GetCountryHandler()->GetCountry(btCid);
			if(!pCountry) return;

			pCountry->SetTechLevel( ln );
		}
		break;

	case MSG_W2S_COUNTRY_KING_CHANGE:
		{
			BYTE btCid = pMsg->GetByte();
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			CCountry *pCountry = GetCountryHandler()->GetCountry(btCid);
			if(!pCountry) return;

			pCountry->SetKing( PlayerGuid );
		}
		break;
	}
}