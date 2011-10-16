#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

class CGoods;

//װ����Ƕ��
class CEnchaseHole 
{
public:
	CEnchaseHole();
	CEnchaseHole(CGoods* pParent);
	~CEnchaseHole();	
private:
	CGoods* m_pParent;
	//�ײ��Ƿ񱻼���
	//���������Ѹ���Ϊ���пײ۶�Ĭ��Ϊ����״̬
	bool m_bIsActived; 
	//����Ƕ��Ƭ
	CGoods* m_pCard;
public:
	void SetParent(CGoods* pGoods){m_pParent=pGoods;}	

	CGoods* GetParent(){return m_pParent;}	

	CGoods* GetCard(){return m_pCard;}

	void SetCard(CGoods* pCard){m_pCard=pCard;}

	void SetState(bool bActived){m_bIsActived=bActived;}

	bool GetState(){return m_bIsActived;}

	bool CanEnchase(CGoods* pGoods);

	bool EnchaseCard(CPlayer* pPlayer,CGoods* pCard);

	bool RemoveCard(CPlayer* pPlayer,bool bReserve);
};