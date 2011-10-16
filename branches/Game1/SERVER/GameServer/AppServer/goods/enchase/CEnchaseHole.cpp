#include "stdafx.h"
#include "cenchasehole.h"
#include "../cgoods.h"
#include "../CGoodsFactory.h"

CEnchaseHole::CEnchaseHole()
{	
}

CEnchaseHole::CEnchaseHole(CGoods *pParent)
{
	m_pParent=pParent;
	//�����������пײ۶�Ĭ��Ϊ����״̬	
	m_bIsActived = TRUE;
	m_pCard=NULL;
}

CEnchaseHole::~CEnchaseHole()
{	
	CGoodsFactory::GarbageCollect(&m_pCard,41);
}

BOOL CEnchaseHole::CanEnchase(CGoods* pGoods)
{
	return FALSE;
}

BOOL CEnchaseHole::EnchaseCard(CPlayer* pPlayer,CGoods* pCard)
{
	//�����������пײ۶�Ĭ��Ϊ����״̬������ɾ�����ж�	
	if ( m_pCard )
		return FALSE;	
	
	//��������Ҫ��Ƕ��������ͬ�Ŀ�Ƭ�������Ƭ������ɵ���Ʒָ�뱻ɾ������
	if(pCard->GetAmount()==1)
		m_pCard=pCard;
	else
		m_pCard = CGoodsFactory::CreateGoods( pCard->GetBasePropertiesIndex(), 55 );
	
	m_pParent->EnchaseAttrUpdate(pPlayer,m_pCard);
	return TRUE;
}

BOOL CEnchaseHole::RemoveCard(CPlayer* pPlayer,bool bReserve)
{
	//�����������пײ۶�Ĭ��Ϊ����״̬������ɾ�����ж�
	if ( !m_pCard )	
		return FALSE;
	m_pParent->EnchaseAttrRemove(pPlayer,m_pCard);
	if(bReserve)
	{
		m_pCard=NULL;
	}
	else
	{
		CGoodsFactory::GarbageCollect(&m_pCard,41);	
	}	
	return TRUE;
}