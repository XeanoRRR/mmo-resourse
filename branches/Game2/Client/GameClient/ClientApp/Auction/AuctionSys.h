/**
* @filename:AuctionHouseSys.h
* @date: 2010/5/6
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: �ɱ�����ϵͳ�߼���
*/
#include "../../../../Public/Common/AHBaseDef.h"

class CMessage;
/// @brief
/// ���ݽṹ����������� 
class AHdata
{
public:
	///���캯��
	AHdata();
	///��������
	~AHdata();

	///���룬��䶩������,optype��������AHBaseDef.h����� message operation type ö��ֵ���ǰ�������ͷ��سɹ������򷵻�ʧ��
	bool DecodeFromDataBlock(CMessage* msg,uint optype);

	///
	typedef std::vector<AH::OrderRaw>vecOrderRaw;
	///�ɹ���ҵ��б�
	vecOrderRaw m_CanBuyOrder;
	/// �ɳ��۹ҵ��б�
	vecOrderRaw m_CanSaleOrder;

	///�Զ���ID��Ϊmap��key
	typedef std::map<uint,AH::SubOrderRaw> MapSubOrderRaw;
	///����Լ��Ĺҵ���Ϣ�б�
	MapSubOrderRaw m_AgentOrder;

	///����������Ͳ�ѯ��������
	void Send_AH_REQUEST_ALL();

	///����������Ͳ�ѯ�ɹ��򶩵��б�
	void Send_AH_REQUEST_BUY();

	///����������Ͳ�ѯ�ɳ��۶����б�
	void Send_AH_REQUEST_SELL();

	///����������Ͳ�ѯ����ί�ж����б�,����ȡ��Ϣ
	void Send_AH_REQUEST_AGENT();

	///��������������ί�ж���,0 = OT_SELL,1 = OT_BUY
	void Send_AH_ADD_AGENT(AH::OrderRaw& or,int ordertype);

	///����������ͳ���ί�ж�����orderID ��ʶ����ID��
	void Send_AH_REMOVE_AGENT(int orderID);

	///����������͹�����Ϣ,orederIDָ������Ķ���ID��cnt��������
	void Send_AH_BUY(long cnt);

	///����������ͳ�����Ϣ��orederIDָ�����۵Ķ���ID��cnt��������
	void Send_AH_SEL(long cnt);

	///�������������ȡ��Ϣ,type = PT_GOODS(λ��) type = PT_COIN(���)
	void Send_AH_PICKUP(int type);

	///��ÿ���ȡ�����Ŀ
	uint GetNumGoldCanPickUp(){ return m_GoldPUp;}

	///��ȡ����ȡλ����
	uint GetNumWeimianCanPickUp(){ return m_WeiMianPUp; }

	///���õ�ǰ�ɳ��۵Ķ���ID��
	void SetCanSaleID(uint id){ m_CanSaleID = id ; }

	/// ��ȡ��ǰ�ɳ��۵Ķ�����
	uint GetCanSaleID() const { return m_CanSaleID;}

	///���õ�ǰ�ɹ���Ķ���ID��
	void SetCanBuyID(uint id){ m_CanBuyID = id ;  }

	///��ȡ��ǰ�ɹ���Ķ�����
	uint GetCanBuyID() const { return m_CanBuyID; }

	////���ݿɹ��򶩵�ID����ȡ�ɹ���ĵ���
	uint GetPriceByCanBuyID(uint ID);

	////���ݿɹ��򶩵�ID����ȡ�ɹ���ĵ���
	uint GetCountByCanBuyID(uint ID);

	////���ݿɳ��۶���ID����ȡ�ɳ�����Ŀ
	uint GetPriceByCanSaleID(uint ID);

	////���ݿɳ��۶���ID, ��ȡ�ɳ��۵ĵ���
	uint GetCountByCanSaleID(uint ID);

private:
	uint m_GoldPUp;///����ȡ�������
	uint m_WeiMianPUp;///λ�����ȡ��Ŀ

	uint m_CanSaleID;///��ǰҪ���۵Ķ���ID��
	uint m_CanBuyID;///��ǰҪ����Ķ���ID��
};