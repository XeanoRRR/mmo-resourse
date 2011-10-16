/**
* @filename:	PlayerTradeMessage.cpp
* @date:		2010/7/22
* @author:		wing (totti19851101@gmail.com) 

* @purpose: ��ҽ�����Ϣ����
*/

#pragma once
#include "stdafx.h"
#include "Message.h"

#include "../Player.h"
#include "../ClientRegion.h"
#include "../GameClient/Game.h"
#include "../Other/AudioList.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../TradeSystem/PlayerTrade.h"
#include "../../../UI/GamePage/ChatList/ChatListPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace  PlayerTradeMessage
{
	struct  tagTrade
	{
		CGUID  SourID;
		CGUID  DestID;
	};
}

void RequestTrade(MsgEvent& event,eMsgResult UserId)
{
	PlayerTradeMessage::tagTrade* pTrade = (PlayerTradeMessage::tagTrade*)event.pData;
	if ( !pTrade ) return ;

	BYTE bResult = -1;
	// ͬ�⽻��
	if(UserId == S_Ok)
	{			
		bResult = 1;
		CPlayer* pPlayer = (CPlayer*)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,pTrade->SourID);
		CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
		if (pPlayer!=NULL&&pMainPlayer!=NULL&&pMainPlayer->Distance(pPlayer)<6)
		{
			CMessage msg(MSG_C2S_PLAYERTRADE_RESPONSE);
			msg.Add(pPlayer->GetExID());
			msg.Add(bResult);
			msg.Send();
		}
		else if (pPlayer==NULL)
		{
			// ��������Ҳ�����
            GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_13"),   //�����׵���Ҳ��ڸ���!
                0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
		}
		else if (pMainPlayer!=NULL&&pPlayer!=NULL&&pMainPlayer->Distance(pPlayer)>6)
		{
			// ����̫Զ
			GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_14"),  //���������׵���Ҿ���̫Զ��!
                0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
		}
	}
	else if (UserId == S_Cancel || UserId == S_Esc)
	{
		bResult = 0;
		CMessage msg(MSG_C2S_PLAYERTRADE_RESPONSE);
		msg.Add(pTrade->SourID);
		msg.Add(bResult);
		msg.Send();
	}

	SAFE_DELETE(pTrade);
}

/*
* ����: ��Ӧ��ҽ�����Ϣ
* ժҪ: -
* ����: pMsg - ���������͵���Ϣ
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*/

void OnPlayerTradeMessage(CMessage* pMsg)
{
	CPlayer* pPlayer=GetGame()->GetMainPlayer();
	if (!pPlayer)
		return;

	switch(pMsg->GetType())
	{
		//gameserverת����������ҽ�������
	case MSG_S2C_PLAYERTRADE_REQUEST:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);

			// ��Ϸ���ý���δ��ɣ��˴�Ĭ��������ҽ���
			bool bAllowExchange = true;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption( CGameConfigPageEx::OPTION_AllowExchange );
			if ( bAllowExchange ==false /*|| pPlayerTradePage->GetTradeRequestGUID()!=CGUID::GUID_INVALID*/)
			{
				CMessage msg(MSG_C2S_PLAYERTRADE_RESPONSE);
				msg.Add(PlayerID);
				msg.Add((BYTE)0);
				msg.Send();

				if(!bAllowExchange)
                    GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_12") //��û�д򿪽��׿��أ��Զ��ܾ��˶Է���ҵĽ�������
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);

				return;
			}

			CPlayer* pPlayer=(CPlayer*)(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID));
			if(pPlayer!=NULL)
			{
				PlayerTradeMessage::tagTrade* pTrade = new PlayerTradeMessage::tagTrade;
				pTrade->SourID = PlayerID;
				pTrade->DestID = GetGame()->GetMainPlayer()->GetExID();

				char strInfo[255];
                sprintf(strInfo,AppFrame::GetText("Trade_1")   //%s ������㽻�ף��Ƿ�ͬ�⣿
                    ,pPlayer->GetName());
				GetInst(MsgEventManager).PushEvent(Msg_Agree, strInfo, RequestTrade, pTrade);

				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-03.wav");
			}
		}
		break;

		//��ʼ����
	case MSG_S2C_PLAYERTRADE_BEGIN:
		{
			CGUID SessionID,RequestPlayerID,ResponsePlayerID;
			pMsg->GetGUID(SessionID);
			pMsg->GetGUID(RequestPlayerID);
			long lRequestPlugID = pMsg->GetLong();
			pMsg->GetGUID(ResponsePlayerID);
			long lResponsePlugID = pMsg->GetLong();

			if (pPlayer->GetExID()==RequestPlayerID)
			{
				GetInst(PlayerTrade).SetTradePara(SessionID,RequestPlayerID,lRequestPlugID,ResponsePlayerID,lResponsePlugID);
				FireUIEvent("PlayerTrade","Opened");
			}
			else if (pPlayer->GetExID()==ResponsePlayerID)
			{
				GetInst(PlayerTrade).SetTradePara(SessionID,ResponsePlayerID,lResponsePlugID,RequestPlayerID,lRequestPlugID);
				FireUIEvent("PlayerTrade","Opened");
			}
		}
		break;

		// �Է��Ѵ�������״̬
	case MSG_S2C_PLAYERTRADE_LOCK:
		{
			GetInst(PlayerTrade).SetLockedState(true, PlayerTrade::Trade_Other);
			FireUIEvent("PlayerTrade","UpdatePage");
		}
		break;

		//��ҽ��뽻�׵ȴ�״̬�ķ�����Ϣ
	case MSG_S2C_PLAYERTRADE_WAIT:
		{
			long lPlugID = pMsg->GetLong();
			BYTE bReady = pMsg->GetByte();
			if (bReady==1)
			{
				GetInst(PlayerTrade).SetLockedState(true,PlayerTrade::Trade_Other);
			}
			else
			{
				GetInst(PlayerTrade).SetLockedState(false,PlayerTrade::Trade_Other);
				GetInst(PlayerTrade).SetLockedState(false,PlayerTrade::Trade_Me);
			}
			FireUIEvent("PlayerTrade","UpdatePage");
		}
		break;

		//���ȡ�����׵ķ�����Ϣ
	case MSG_S2C_PLAYERTRADE_CANCEL:
		{
			if (!GetInst(PlayerTrade).GetResult())
			{
                GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_9") //����ȡ��
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);

				FireUIEvent("PlayerTrade","ClosePage");
				//���±���
				//if (pItemPage!=NULL)
				//{
				//	pItemPage->UpdataAllSubPack();
				//	pItemPage->UpdataAllItems();
				//}
			}
		}
		break;

		//gameserver���صĽ�����ɽ��
	case MSG_S2C_PLAYERTRADE_RESULT:
		{	
			BYTE result=pMsg->GetByte();
			//���׳ɹ�������������
			if(result==1)
			{
                GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_8") //���׳ɹ�
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-35.wav");
			}
			// ����ʧ�ܣ�����������
			else if (result==0)
			{
                GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Trade_6") //����ʧ��
                    , 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
			}
			// ����ʧ�ܣ������������ף��ص��������ý���״̬�����Լ����˴ν���
			else if (result==2)
			{
				//if (pTradePage!=NULL&&pTradePage->IsOpen())
				//{
				//pTradePage->SetLockedState(false,CPlayerTradePageEx::Trade_Other);
				//pTradePage->SetLockedState(false,CPlayerTradePageEx::Trade_Me);
				//pTradePage->UpdatePage();
				//}

				GetInst(PlayerTrade).SetLockedState(false,PlayerTrade::Trade_Other);
				GetInst(PlayerTrade).SetLockedState(false,PlayerTrade::Trade_Me);
				FireUIEvent("PlayerTrade","UpdatePage");
			}
			
			GetInst(PlayerTrade).SetResult(true);
			FireUIEvent("PlayerTrade","ClosePage");
			//���±���
			//	if (pItemPage!=NULL)
			//	{
			//		pItemPage->UpdataAllSubPack();
			//		pItemPage->UpdataAllItems();
			//	}
		}
		break;
	}
}