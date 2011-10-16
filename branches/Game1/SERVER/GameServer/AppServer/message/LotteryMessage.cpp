#include "stdafx.h"
#include "..\serverregion.h"
#include "..\Player.h"
#include "..\Monster.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"
#include "..\script\script.h"
#include "..\goods\cgoods.h"
#include "../setup/GlobeSetup.h"
#include "..\Session\CSessionFactory.h"
#include "..\Session\CSession.h"
#include "..\Session\CPlug.h"
#include "..\Session\CTrader.h"
#include "../../../../public/StrUnifyInput.h"
#include "../../../../public/GoodsExDef.h"
#include "../goods/CGoodsFactory.h"
#include "../GoodsExManage.h"
#include "..\SMClient.h"
// ��ӦGM��Ϣ
void OnLotteryMessage( CMessage* pMsg)
{
	//if( IsInChangingServer() )
	//{
	//	return;
	//}

	switch(pMsg->GetType())
	{
	case MSG_C2S_LOTTERY_ResetPrizes:   //���ý�Ʒ
		{
			CPlayer* pPlayer = pMsg->GetPlayer();
			if (pPlayer!=NULL)
			{
				pPlayer->RefreshVecLottery();
				pPlayer->RefreshVecConstel(false);
			}
		}
		break;
	case MSG_C2S_LOTTERY_ResetConstellations:    //���ó齱����
		{
			CPlayer* pPlayer = pMsg->GetPlayer();
			if (pPlayer!=NULL)
			{
				pPlayer->RefreshVecConstel();
			}
		}
		break;
	case MSG_C2S_LOTTERY_Startup:     //�齱
		{
			CPlayer* pPlayer = pMsg->GetPlayer();
			if (pPlayer!=NULL)
			{
				pPlayer->DoLottery();
			}
		}
		break;
	case MSG_C2S_LOTTERY_Endup:
		{
			CPlayer* pPlayer = pMsg->GetPlayer();
			if (pPlayer!=NULL)
			{
				//pPlayer->SendVecLottery();
				pPlayer->SendLotteryGoods();
				pPlayer->SendFieldType();
				if(pPlayer->GetIsSendWorldInfo())
				{
					char* strScript = "scripts/lottery/lotterysendgoods.script";
	                pPlayer->PlayerRunScript(strScript);
					pPlayer->SetSendWorldInfo(false);
				}
				pPlayer->SendPersonalCreditInfo();
			}
		}
		break;
	case MSG_C2S_LOTTERY_DoSystemReset: //ǿ������
		{
			CPlayer * pPlayer = pMsg->GetPlayer();
			if (pPlayer!=NULL)
			{
				pPlayer->SystemReset();
			}
		}
		break;
	case MSG_C2S_LOTTERY_OpenPage: //�򿪽���
		{
			CPlayer * pPlayer = pMsg->GetPlayer();
			if (pPlayer!=NULL)
			{
				pPlayer->OpenLotteryPage();
			}
		}
		break;
	case MSG_W2S_LOTTERY_SENDWORLDCREDIT:      //GS��������Ҹ�������
		{
			long lWorldCredit = pMsg->GetLong();
			CMessage msg(MSG_S2C_LOTTERY_WorldPoint);
			msg.Add( (long)lWorldCredit );	
			msg.SendAll();
		}
		break;
	case MSG_W2S_LOTTREY_SENDCreditHighest:
		{
			long lCreditHighest = pMsg->GetLong();
			char szName[128];
			pMsg->GetStr(szName, 128);
			//������߸��˻���
			CMessage msg(MSG_S2C_LOTTERY_MaxTodayPoint);
			msg.Add( (long)lCreditHighest );	
			msg.Add( (char*)szName );	
			msg.SendAll();
		}
		break;
	case MSG_W2S_LOTTREY_SENDWorldInfo:
		{
			char szText[1024];
			pMsg->GetStr(szText, 1024);
			CMessage msg(MSG_S2C_LOTTERY_WorldInfo);
			msg.Add( (char*)szText );	
			msg.SendAll();
		}
		break;
	}
}