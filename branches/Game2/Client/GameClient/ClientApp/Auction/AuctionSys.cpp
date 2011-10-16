#include "stdafx.h"
#include "AuctionSys.h"
#include "../../../Net/Message.h"

extern void FireUIEvent(const char* id, const char* e);
using namespace AH;
//
#define FUNC_GETGUID { CGUID guid ; pMsg->GetGUID(guid); }

AHdata::AHdata()
{
	m_CanSaleOrder.clear();
	m_AgentOrder.clear();
	m_CanBuyOrder.clear();
	m_GoldPUp = 0;
	m_WeiMianPUp = 0;
	m_CanSaleID = 0;
	m_CanBuyID = 0;
}
AHdata::~AHdata()
{
	m_AgentOrder.clear();
	m_CanSaleOrder.clear();
	m_CanBuyOrder.clear();
}
bool AHdata::DecodeFromDataBlock(CMessage* pMsg,uint optype)
{
	switch(optype)
	{
	case AH_REQUEST_BUY://����ɹ����б���Ϣ
		{
			FUNC_GETGUID;
			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
			long count =  setReadDB.GetLongFromByteArray();
			m_CanBuyOrder.clear();
			for(long l = 0 ; l < count ; ++l)
			{
				AH::OrderRaw or;
				setReadDB.GetBufferFromByteArray(&or,sizeof(or));
				m_CanBuyOrder.push_back(or);
			}
			//m_SaleOrder.sort();
			FireUIEvent("Auction/Tab/BuySale/SaleMCL","UpdateCanBuyMCL");//���¿ɹ����б�UI
			//FireUIEvent("Auction","UpdatePickUpUI");//������ȡUI
		}
		break;
	case AH_REQUEST_SELL://���������ҳ����б���Ϣ
		{
			FUNC_GETGUID;
			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
			long cnt = setReadDB.GetLongFromByteArray();
			m_CanSaleOrder.clear();
			for(long l = 0; l < cnt ; ++l)
			{
				AH::OrderRaw or;
				setReadDB.GetBufferFromByteArray(&or,sizeof(or));
				m_CanSaleOrder.push_back(or);
			}
			//m_BuyOrder.sort();
			FireUIEvent("Auction/Tab/BuySale/BuyMCL","UpdateBuyMCL");//���¿�����ҳ����б�UI
			//FireUIEvent("Auction","UpdatePickUpUI");//������ȡUI
		}
		break;
	case AH_REQUEST_AGENT://�������ί���б���Ϣ
		{
			FUNC_GETGUID;
			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
			char flag = setReadDB.GetByteFromByteArray();//��ʶ�������Ƿ����ί�ж�������
			if(!flag)//������ί�еĶ���
			{
				m_WeiMianPUp = 0;
				m_GoldPUp = 0;
				m_AgentOrder.clear();
				FireUIEvent("Auction/Tab/Query/MCL","UpdatePerMCL");//���¸��˹ҵ��б�UI
				FireUIEvent("Auction","UpdatePickUpUI");//������ȡUI
				break;
			}
			else
			{
				long cnt = setReadDB.GetLongFromByteArray();
				m_AgentOrder.clear();
				AH::SubOrderRaw sor;
				for(long l = 0 ; l < cnt ; ++l)
				{
					uint ID = setReadDB.GetLongFromByteArray();
					setReadDB.GetBufferFromByteArray(&sor,sizeof(sor));
					m_AgentOrder[ID] = sor;
				}
				//����������������ݣ��ֱ����������ҿ���ȡ�Ľ�Һ�λ����
				m_GoldPUp = setReadDB.GetLongFromByteArray();//�������ȡ�����
				m_WeiMianPUp = setReadDB.GetLongFromByteArray();//�������ȡλ����
				FireUIEvent("Auction/Tab/Query/MCL","UpdatePerMCL");//���¸��˹ҵ��б�UI
				FireUIEvent("Auction","UpdatePickUpUI");//������ȡUI
			}
		}
		break;
	default:
		return false;
	};
	return true;
}

void AHdata::Send_AH_REQUEST_ALL()
{
	Send_AH_REQUEST_BUY();
	Send_AH_REQUEST_SELL();
	Send_AH_REQUEST_AGENT();
}
void AHdata::Send_AH_REQUEST_BUY()
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)AH::AH_REQUEST_BUY);
	msg.Send();
}
void AHdata::Send_AH_REQUEST_SELL()
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)AH::AH_REQUEST_SELL);
	msg.Send();
}

void AHdata::Send_AH_REQUEST_AGENT()
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)AH::AH_REQUEST_AGENT);
	msg.Send();
}

void AHdata::Send_AH_ADD_AGENT(AH::OrderRaw& or,int ordertype)
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)(AH::AH_ADD_AGENT));
	msg.Add((long)or.cnt);
	msg.Add((long)or.price);
	msg.Add((long)ordertype);
	msg.Send();
}

void AHdata::Send_AH_REMOVE_AGENT(int orderID)
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)(AH::AH_REMOVE_AGENT));
	msg.Add((long)orderID);
	msg.Send();
}

void AHdata::Send_AH_BUY(long cnt)
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)(AH::AH_BUY));
	msg.Add((long)m_CanBuyID);
	msg.Add(cnt);
	msg.Send();
}

void AHdata::Send_AH_SEL(long cnt)
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)(AH::AH_SELL));
	msg.Add((long)m_CanSaleID);
	msg.Add(cnt);
	msg.Send();
}

void AHdata::Send_AH_PICKUP(int type)
{
	CMessage msg(MSG_C2S_AH_OPER);
	msg.Add((uchar)AH::AH_PICKUP);
	if(type == AH::PT_GOODS)
		msg.Add((uchar)AH::PT_GOODS);
	else if(type ==AH::PT_COIN )
		msg.Add((uchar)AH::PT_COIN);
	msg.Send();
}

uint AHdata::GetPriceByCanSaleID(uint ID){
	for(uint i = 0 ; i < m_CanSaleOrder.size(); ++i)
	{
		if (m_CanSaleOrder[i].id == ID)
		{
			return m_CanSaleOrder[i].price;
		}
	}
	return 0;
}

uint AHdata::GetCountByCanSaleID(uint ID)
{
	for(uint i = 0 ; i < m_CanSaleOrder.size(); ++i)
	{
		if (m_CanSaleOrder[i].id == ID )
		{
			return m_CanSaleOrder[i].cnt;
		}
	}
	return 0;
}

uint AHdata::GetPriceByCanBuyID(uint ID)
{
	for (uint i = 0 ; i < m_CanBuyOrder.size() ; ++i)
	{
		if( m_CanBuyOrder[i].id == ID)
			return m_CanBuyOrder[i].price;
	}
	return 0;
}

uint AHdata::GetCountByCanBuyID(uint ID)
{
	for( uint i = 0 ; i < m_CanBuyOrder.size(); ++i)
	{
		if (m_CanBuyOrder[i].id == ID)
			return m_CanBuyOrder[i].cnt;
	}
	return 0;
}
