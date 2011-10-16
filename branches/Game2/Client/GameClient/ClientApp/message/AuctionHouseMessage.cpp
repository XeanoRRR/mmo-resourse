/**
* @filename:AuctionHouseMessage.cpp
* @date: 2010/5/6
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: �ɱ�������Ϣ����
*/

#include "stdafx.h"
#include "../Auction/AuctionSys.h"
#include "../../../Net/Message.h"
#include "../MsgBoxSys/MsgEventManager.h"
/////////////////////////////////////////////////
// zhaohang  2010/5/12 
// ��Ϣ������ʱ����Ϣ����ʽ��ӳ�����Ժ���Ӷ�Ӧ����
/***********************************************************************/
/* zhaohang fix 2010-9-3
/* �޸���Ϣ����ʽ�����÷�װ��MsgEventManager,�Լ��µ��ַ���Դ���ط�ʽAppframe
/***********************************************************************/
/////////////////////////////////////////////////

///������������ص��ɱ�������Ϣ
void OnAuctionHouseMessage(CMessage* pMsg)
{
	//��ȡ����
	int optype = (int)pMsg->GetByte();
	AHdata& ahd = GetInst(AHdata);
	//�����������,���ﰴoptype���ʹ�����AHBaseDef.h����� message operation type ö��ֵ���ǰ����
	if(true == ahd.DecodeFromDataBlock(pMsg,optype))
	{
		return ; 
	}
	else //��������optype����
	{
		uint result = AH::RES_END;
		uint orderID = 0;
		switch(optype)
		{
		case AH::AH_ADD_AGENT:
			result = (uint)pMsg->GetByte();
			orderID = (uint)pMsg->GetLong();
			if(result== AH::RES_OK) //��Ӷ����ɹ�������һ�θ��¸��˶����б�����
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_1"),NULL,NULL,true);
				ahd.Send_AH_REQUEST_ALL();
			}
			else if(result == AH::RES_NOT_ENOUGH_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_2"),NULL,NULL,true);
			}
			else if(result == AH::RES_NOT_ENOUGH_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_3"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ORDER)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_4"),NULL,NULL,true);
			}
			else if (result == AH::RES_INVALID_ORDER_CNT)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_5"),NULL,NULL,true);
			}
			else if ( result == AH::RES_MAX_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_6"),NULL,NULL,true);
			}
			else if ( result == AH::RES_MAX_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_7"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ARG)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_54"),NULL,NULL,true);
			}
			else if(result == AH::RES_AGENT_MAX)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_8"),NULL,NULL,true);
			}
			else if(result == AH::RES_UNKNWON_ERROR)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_9"),NULL,NULL,true);
			}
			else if(result == AH::RES_END)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_10"),NULL,NULL,true);
			}
			break;
		case AH::AH_REMOVE_AGENT:
			result = (uint)pMsg->GetByte();
			if(result== AH::RES_OK) //�Ƴ������ɹ���
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_11"),NULL,NULL,true);
				ahd.Send_AH_REQUEST_ALL();//����������
			}
			else if(result == AH::RES_NOT_ENOUGH_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_12"),NULL,NULL,true);
			}
			else if(result == AH::RES_NOT_ENOUGH_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_55"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ORDER)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_13"),NULL,NULL,true);
				ahd.Send_AH_REQUEST_AGENT();//��������ʧ�ܣ����͸��˶����б��������
			}
			else if (result == AH::RES_INVALID_ORDER_CNT)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_14"),NULL,NULL,true);
				ahd.Send_AH_REQUEST_AGENT();//��������ʧ�ܣ����͸��˶����б��������
			}
			else if ( result == AH::RES_MAX_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_15"),NULL,NULL,true);
			}
			else if ( result == AH::RES_MAX_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_16"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ARG)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_17"),NULL,NULL,true);
			}
			else if(result == AH::RES_AGENT_MAX)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_18"),NULL,NULL,true);
			}
			else if(result == AH::RES_UNKNWON_ERROR)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_19"),NULL,NULL,true);
			}
			else if(result == AH::RES_END)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_20"),NULL,NULL,true);
			}
			break;
		case AH::AH_BUY:
			result = (uint)pMsg->GetByte();

			if(result == AH::RES_OK)//����ɹ�
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_21"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_BUY();//���¿ɹ����б�
				GetInst(AHdata).Send_AH_REQUEST_AGENT();//���¸����б����ȡ��Ϣ
			}
			else if(result == AH::RES_NOT_ENOUGH_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_22"),NULL,NULL,true);
			}
			else if(result == AH::RES_NOT_ENOUGH_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_23"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ORDER)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_24"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_BUY();//����ʧ�ܸ��¿ɹ����б�
			}
			else if (result == AH::RES_INVALID_ORDER_CNT)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_25"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_BUY();//����ʧ�ܸ��¿ɹ����б�
			}
			else if ( result == AH::RES_MAX_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_26"),NULL,NULL,true);
			}
			else if ( result == AH::RES_MAX_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_27"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ARG)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_28"),NULL,NULL,true);
			}
			else if(result == AH::RES_AGENT_MAX)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_29"),NULL,NULL,true);
			}
			else if(result == AH::RES_UNKNWON_ERROR)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_30"),NULL,NULL,true);
			}
			else if(result == AH::RES_END)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_31"),NULL,NULL,true);
			}
			break;
		case AH::AH_SELL:
			result = (uint)pMsg->GetByte();
			if(result == AH::RES_OK)//���۳ɹ���
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_32"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_SELL();//����һ�οɳ�����Ϣ�б�
				GetInst(AHdata).Send_AH_REQUEST_AGENT();//����һ�θ����б�Ϳ���ȡ��Ϣ
			}
			else if(result == AH::RES_NOT_ENOUGH_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_33"),NULL,NULL,true);
			}
			else if(result == AH::RES_NOT_ENOUGH_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_34"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ORDER)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_35"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_SELL();//����ʧ�ܣ�����һ�οɳ����б�
			}
			else if (result == AH::RES_INVALID_ORDER_CNT)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_36"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_SELL();//����ʧ�ܣ�����һ�οɳ����б�
			}
			else if ( result == AH::RES_MAX_MONEY)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_37"),NULL,NULL,true);
			}
			else if ( result == AH::RES_MAX_GOODS)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_38"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ARG)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_39"),NULL,NULL,true);
			}
			else if(result == AH::RES_AGENT_MAX)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_40"),NULL,NULL,true);
			}
			else if(result == AH::RES_UNKNWON_ERROR)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_41"),NULL,NULL,true);
			}
			else if(result == AH::RES_END)
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_42"),NULL,NULL,true);
			}
			break;
		case AH::AH_PICKUP:
			result = (uint)pMsg->GetByte();
			if(result == AH::RES_OK)//��ȡ�ɹ�������һ�θ�����ȡ�б�����
			{

				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_43"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_AGENT();
			}
			else if(result == AH::RES_NOT_ENOUGH_MONEY)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_44"),NULL,NULL,true);
			}
			else if(result == AH::RES_NOT_ENOUGH_GOODS)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_45"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ORDER)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_46"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_AGENT();//��ȡʧ�ܣ�����һ�ο���ȡ�б�
			}
			else if (result == AH::RES_INVALID_ORDER_CNT)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_47"),NULL,NULL,true);
				GetInst(AHdata).Send_AH_REQUEST_AGENT();//��ȡʧ�ܣ�����һ�ο���ȡ�б�
			}
			else if ( result == AH::RES_MAX_MONEY)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_48"),NULL,NULL,true);
			}
			else if ( result == AH::RES_MAX_GOODS)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_49"),NULL,NULL,true);
			}
			else if(result == AH::RES_INVALID_ARG)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_50"),NULL,NULL,true);
			}
			else if(result == AH::RES_AGENT_MAX)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_51"),NULL,NULL,true);
			}
			else if(result == AH::RES_UNKNWON_ERROR)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_52"),NULL,NULL,true);
			}
			else if(result == AH::RES_END)
			{
				
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("AU_53"),NULL,NULL,true);
			}
			break;
		default:
			break;
		}
	}
}