/**
*	file:		SpecialEquipContainer.h
*	Brief:		����װ������
*	detail:		����װ����λ�� ���֡����֡�����
*	Authtor:	��η
*	Date:		2010-08-24
*/
#pragma once
#include "volumelimitgoodscontainer.h"
#include "../Goods/Goods.h"

class CSpecialEquipContainer :
    public CVolumeLimitGoodsContainer
{
public:
    CSpecialEquipContainer(void);
    ~CSpecialEquipContainer(void);

	/// ���÷���װ����λ����
	void SetTrumpType(eTrumpType eType) { m_eTrumpType = eType; }
	/// ��ȡ����װ����λ����
	eTrumpType GetTrumpType(void) { return m_eTrumpType; }

    virtual BOOL AddFromDB( CGoods* pObject, DWORD dwPosition = 0 );

private:
	eTrumpType m_eTrumpType;			///< ����װ������
};
