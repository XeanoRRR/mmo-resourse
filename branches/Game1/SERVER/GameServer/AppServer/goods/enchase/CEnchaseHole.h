#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

using namespace std;

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
	BOOL m_bIsActived; 
	//����Ƕ��Ƭ
	CGoods* m_pCard;
public:
	VOID SetParent(CGoods* pGoods){m_pParent=pGoods;}	

	CGoods* GetParent(){return m_pParent;}	

	CGoods* GetCard(){return m_pCard;}

	VOID SetCard(CGoods* pCard){m_pCard=pCard;}

	VOID SetState(BOOL bActived){m_bIsActived=bActived;}

	BOOL GetState(){return m_bIsActived;}

	BOOL CanEnchase(CGoods* pGoods);

	BOOL EnchaseCard(CPlayer* pPlayer,CGoods* pCard);

	BOOL RemoveCard(CPlayer* pPlayer,bool bReserve);
};