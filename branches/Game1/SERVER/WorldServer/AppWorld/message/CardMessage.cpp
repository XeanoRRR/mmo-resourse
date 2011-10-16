#include "stdafx.h"
#include "..\nets\networld\message.h"
#include "../../Server/setup/GlobeSetup.h"
#include "..\..\worldserver\game.h"
#include "..\..\appworld\CardsDealingSystem\CardsHandler.h"
#include "..\..\appworld\CardsDealingSystem\Card.h"

#include "../../../setup/LogSystem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��Ӧ���ۿ���Ϣ
void OnCardMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_S2W_CARD_CHARGE_REQUEST:
		
		break;

	case MSG_COST2W_CARD_CHARGE_RESPONSE:
		
		break;

	case MSG_S2W_CARD_SELL_REQUEST:				// ת����cost���е㿨��֤
		
		break;

	case MSG_COST2W_CARD_VALIDATE_RESPONSE:		// 
		
		break;

	case MSG_COST2W_CARD_SELL_RESPONSE:
		
		break;

	case MSG_S2W_CARD_RESERVE_REQUEST: // Ԥ����
		
		break;

		// CostServer ���� Ԥ������Ϣ
	case MSG_COST2W_CARD_RESERVE_RESPONSE:
		break;

	case MSG_S2W_CARD_BUY_REQUEST: // Ԥ���ɹ���GameServer����������Ϣ
		
		break;

	case MSG_COST2W_CARD_BUY_RESPONSE: // Cost Server������������
		
		break;

	case MSG_S2W_CARD_CANCEL_REQUEST: // Gameserver��������ȡ���㿨
		
		break;

	case MSG_COST2W_CARD_CANCEL_RESPONSE:
		
		break;

	case MSG_S2W_CARD_LIST_REQUEST: // ��������б�
		
		break;
	} // end switch
} // end fuction
