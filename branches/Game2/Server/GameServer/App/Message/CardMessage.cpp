#include "StdAfx.h"
#include "../Player.h"
#include "../Goods/CGoodsFactory.h"
#include "../MessagePackage/CS2CContainerObjectMove.h"
#include "../MessagePackage/CS2CContainerObjectAmountChange.h"

#include "../../../../Public/Setup/GlobalSetup.h"

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
		// ��ֵ���� 
	case MSG_C2S_CARD_CHARGE_REQUEST:
		{
			CPlayer* pPlayer = pMsg -> GetPlayer();
			if( !pPlayer ) return;

			
			char CardNumber[16];
			char CardPassword[16];
			char szAccount[32];

			pMsg->GetStr(CardNumber,16);
			pMsg->GetStr(CardPassword, 16);
			pMsg->GetStr(szAccount, 32);

			DWORD dwClientIP = pMsg->GetIP();

			if( strlen(CardNumber) != 12 )
			{
				CMessage msg(MSG_S2C_CARD_CHARGE_RESPONSE);
				msg.Add(long(200)); 
				msg.Add(CardNumber);
				msg.SendToPlayer(pPlayer -> GetExID());
				return;
			}

			if( strlen(CardPassword) != 10 )
			{
				CMessage msg(MSG_S2C_CARD_CHARGE_RESPONSE);
				msg.Add(long(201)); 
				msg.Add(CardNumber);
				msg.SendToPlayer(pPlayer -> GetExID());
				return;
			}

			if( strlen(szAccount) < 4 )
			{
				CMessage msg(MSG_S2C_CARD_CHARGE_RESPONSE);
				msg.Add(long(202)); 
				msg.Add(CardNumber);
				msg.SendToPlayer(pPlayer -> GetExID());
				return;
			}

			// Player���Ͻ�Ǯ����ָ����Ŀ
			if( pPlayer->GetMoney() >= 2000000000 )
			{
				CMessage msg(MSG_S2C_CARD_CHARGE_RESPONSE);
				msg.Add(long(100)); 
				msg.Add(CardNumber);
				msg.SendToPlayer(pPlayer -> GetExID());
				return;
			}

			// Player���Ͻ�Ǯ����ָ����Ŀ
			if( pPlayer->GetYuanbao() >= 2000000000 )
			{
				CMessage msg(MSG_S2C_CARD_CHARGE_RESPONSE);
				msg.Add(long(101)); 
				msg.Add(CardNumber);
				msg.SendToPlayer(pPlayer -> GetExID());
				return;
			}

			// �ɹ���ת����ws

			CMessage msg(MSG_S2W_CARD_CHARGE_REQUEST);
			msg.Add(CardNumber);
			msg.Add(CardPassword);
			msg.Add(szAccount);
			msg.Add(pPlayer -> GetExID());
			msg.Add(dwClientIP);
			msg.Send();
		}
		break;

	case MSG_W2S_CARD_CHARGE_RESPONSE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
			if(!pPlayer) return;

			pMsg->SetType(MSG_S2C_CARD_CHARGE_RESPONSE);
			pMsg->SendToPlayer(PlayerGuid);

		}
		break;
		//////
		//////
	case MSG_C2S_CARD_SELL_REQUEST:		// �ͻ���������۵㿨
		{
			CPlayer* pPlayer = pMsg -> GetPlayer();
			if( !pPlayer ) return;

			char CardNumber[16];
			char CardPassword[16];
			DWORD CardSellPrice;
			BYTE CardTimeType;

			pMsg->GetStr(CardNumber,16);
			pMsg->GetStr(CardPassword, 16);
			CardSellPrice = pMsg->GetDWord();
			// ��ȡ�������� 6 12 24Сʱ
			CardTimeType= pMsg->GetByte();

			//// �ֿ����20�ң����ܼ���
			//if( pPlayer->GetDepotMoney() >= GlobalSetup::GetSetup()->dwGoldCoinLimit )
			//{
			//	CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
			//	msg.Add(long(9001)); 
			//	msg.Add(CardNumber);
			//	msg.SendToPlayer(pPlayer -> GetExID());
			//	return;
			//}

			// ͨ���������� ��ȫ�������ж�ȡ������
			switch(CardTimeType)
			{
			case 0: // 6 hours
				if( pPlayer->GetMoney() < GlobalSetup::GetSetup()->m_dwCard6HoursChargePrice ) {
					CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
					msg.Add(long(1001)); // �����Ѳ�������ʧ�� 6Сʱ��
					msg.Add(CardNumber);
					msg.SendToPlayer(pPlayer -> GetExID());
					return;
				}
				break;

			case 1: // 12 hours
				if( pPlayer->GetMoney() < GlobalSetup::GetSetup()->m_dwCard12HoursChargePrice ) {
					CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
					msg.Add(long(1002)); // �����Ѳ�������ʧ�� 12Сʱ��
					msg.Add(CardNumber);
					msg.SendToPlayer(pPlayer -> GetExID());
					return;
				}
				break;

			case 2:
			default: // 24 hours and other
				if( pPlayer->GetMoney() < GlobalSetup::GetSetup()->m_dwCard24HoursChargePrice ) {
					CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
					msg.Add((long)1003); // �����Ѳ�������ʧ�� 24Сʱ��
					msg.Add(CardNumber);
					msg.SendToPlayer(pPlayer -> GetExID());
					return;
				}
				//break;
			}

			// �����ѹ��������۽���Ƿ��ںϷ�������
			if( CardSellPrice < GlobalSetup::GetSetup()->m_dwCardLowerLimitPrice ||
				CardSellPrice > GlobalSetup::GetSetup()->m_dwCardUpperLimitPrice )
			{
				CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
				msg.Add((long)1004); // ���۽��Ƿ�
				msg.Add(CardNumber);
				msg.SendToPlayer(pPlayer -> GetExID());
				return;
			}

			// �����ѹ���ת����WS
			pMsg->SetType(MSG_S2W_CARD_SELL_REQUEST);
			pMsg->Add(pPlayer -> GetExID());
			pMsg->Send();
		}
		break;

		//////
		//////
	case MSG_W2S_CARD_SELL_RESPONSE:
		{
			long res;

			char CardNumber[16];
			char CardType[16];
			DWORD ChargePrice;
			
			DWORD CardTime;
			DWORD Price;
			
			res = pMsg->GetLong();
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			pMsg->GetStr(CardNumber, 16);

			if(res)
			{
				CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
				msg.Add((long)res); 
				msg.Add(CardNumber);
				//...
				msg.SendToPlayer(PlayerGuid);
				return;
			}

			CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
			if(!pPlayer) return;
			

			pMsg->GetStr(CardType, 16);
			CardTime = pMsg->GetDWord();
			Price = pMsg->GetDWord();
			ChargePrice = pMsg->GetDWord();

			// ������ߣ��۳�������
			DWORD money = pPlayer->GetMoney();

			if( money < ChargePrice )
			{
				CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
				msg.Add((long)1005); // ���������Ѳ����ˡ�
				msg.Add(CardNumber);
				msg.SendToPlayer(PlayerGuid);
				return;
			}

			// ����
			if(!pPlayer->SetMoney( money - ChargePrice,14))
			{
				CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
				msg.Add((long)1005); // ���������Ѳ����ˡ�
				msg.Add(CardNumber);
				msg.SendToPlayer(PlayerGuid);
				return;
			}

			// ����Ϣ��Client
			CMessage msg(MSG_S2C_CARD_SELL_RESPONSE);
			msg.Add((long)0); // �ɹ�
			msg.Add((char*)CardNumber);
			msg.Add((char*)CardType);
			msg.Add(CardTime);
			msg.Add(Price);
			msg.SendToPlayer(PlayerGuid);

		}
		break;

		//////
		//////

		// ����˿�
	case MSG_C2S_CARD_BUY_REQUEST:
		{
			CPlayer* pPlayer = pMsg -> GetPlayer();
			if( !pPlayer ) return;

			char CardNumber[16];
			DWORD money;

			pMsg->GetStr(CardNumber, 16);
			const CGUID& PlayerID = pPlayer -> GetExID();
			money = pPlayer->GetMoney();

						
			pMsg->SetType(MSG_S2W_CARD_RESERVE_REQUEST);
			pMsg->Add(PlayerID);
			pMsg->Add(money);
			pMsg->Send();

		}
		break;

		/////
		/////
	case MSG_W2S_CARD_RESERVE_RESPONSE: // Ԥ����ɺ󣬿۳������ߵĽ��
		{
			long res;
			DWORD money;
			char CardNumber[16];

			res = pMsg->GetLong();
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			// ʧ��
			if(res)
			{
				// ֱ�ӷ��͵�Client ����ʧ��
				// ���͹�����Ϣ��WorldServer
				CMessage msg(MSG_S2C_CARD_BUY_RESPONSE);
				msg.Add(res);
				msg.Add(CardNumber);
				msg.SendToPlayer(PlayerGuid);
				return;
			}
				
			pMsg->GetStr(CardNumber, 16);
			money = pMsg->GetDWord();

			CPlayer *pPlayer = GetGame()->FindPlayer( PlayerGuid );
			if( !pPlayer ) return;

			if( pPlayer->GetMoney() < money ) 
			{
				// ����������ʧ�ܡ�
				CMessage msg(MSG_S2C_CARD_BUY_RESPONSE);
				msg.Add(long(2001));
				msg.Add(CardNumber);
				msg.SendToPlayer(PlayerGuid);
				return;
			}


			// ��Ǯ
			if(!pPlayer->SetMoney( pPlayer->GetMoney() - money,15 ))
			{
				// ����������ʧ�ܡ�
				CMessage msg(MSG_S2C_CARD_BUY_RESPONSE);
				msg.Add(long(2001));
				msg.Add(CardNumber);
				msg.SendToPlayer(PlayerGuid);
				return;
			}

			// ���͹�����Ϣ��WorldServer
			CMessage msg(MSG_S2W_CARD_BUY_REQUEST);
			msg.Add(CardNumber);
			msg.Add(PlayerGuid);
			msg.Send();
		}
		break;


		//////
		//////
	case MSG_W2S_CARD_BUY_RESPONSE: // ���ع�������
		{
			long res;
			char CardNumber[16];

			res = pMsg->GetLong();
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			pMsg->GetStr(CardNumber,16);

			CMessage msg(MSG_S2C_CARD_BUY_RESPONSE);
			msg.Add(res);
			msg.Add(CardNumber);
			msg.SendToPlayer(PlayerGuid);

			// send to player
		}
		break;


		////////
		////////
	case MSG_C2S_CARD_CANCEL_REQUEST: // ���ȡ���㿨�ļ���
		{
			CPlayer* pPlayer = pMsg -> GetPlayer();
			if( !pPlayer ) return;

			pMsg->SetType(MSG_S2W_CARD_CANCEL_REQUEST);
			pMsg->Add(pPlayer -> GetExID());
			pMsg->Send();
		}
		break;


		///////
		///////
	case MSG_W2S_CARD_CANCEL_RESPONSE:
		{
			long res;
			char CardNumber[16];

			res = pMsg->GetLong();
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			pMsg->GetStr(CardNumber, 16);

			CMessage msg(MSG_S2C_CARD_CANCEL_RESPONSE);
			msg.Add(res);
			msg.Add(CardNumber);
			msg.SendToPlayer(PlayerGuid);
		}
		break;

	case MSG_C2S_CARD_LIST_REQUEST:// ��������б�
		{
			CPlayer* pPlayer = pMsg -> GetPlayer();
			if( !pPlayer ) return;

			pMsg->SetType(MSG_S2W_CARD_LIST_REQUEST);
			pMsg->Add(pPlayer -> GetExID());
			pMsg->Send();
		}
		break;

	case MSG_W2S_CARD_LIST_RESPONSE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			pMsg->SetType(MSG_S2C_CARD_LIST_RESPONSE);
			pMsg->SendToPlayer(PlayerGuid);
		}
		break;

	case MSG_C2S_CARD_SALECOSTS_REQUEST: // ������á�
		{
			CPlayer* pPlayer = pMsg -> GetPlayer();
			if( !pPlayer ) return;

			CMessage msg(MSG_S2C_CARD_SALECOSTS_RESPONSE);
			msg.Add(GlobalSetup::GetSetup()->m_dwCard6HoursChargePrice);
			msg.Add(GlobalSetup::GetSetup()->m_dwCard12HoursChargePrice);
			msg.Add(GlobalSetup::GetSetup()->m_dwCard24HoursChargePrice);
			msg.SendToPlayer(pPlayer -> GetExID());
		}
		break;

	} // end switch;
} // end OnCardMessage