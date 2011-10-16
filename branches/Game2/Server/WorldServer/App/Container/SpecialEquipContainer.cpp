/**
*	file:		SpecialEquipContainer.cpp
*	Brief:		����װ������
*	detail:		����װ����λ�� ���֡����֡�����
*	Authtor:	��η
*	Date:		2010-08-24
*/
#include "stdafx.h"
#include "SpecialEquipContainer.h"

CSpecialEquipContainer::CSpecialEquipContainer()
{
    // ���֡����ּ�������λ��Ϊ8��
    SetContainerVolume(8);
}

CSpecialEquipContainer::~CSpecialEquipContainer()
{

}

BOOL CSpecialEquipContainer::AddFromDB(CGoods* pObject, DWORD dwPosition /* = 0  */)
{
    BOOL bResult = FALSE;

	if (!pObject) return bResult;

	if (pObject->GetGoodsBaseType()==GBT_TRUMP)
	{
		if (pObject->GetTrumpType() == m_eTrumpType)
		{
			bResult = CVolumeLimitGoodsContainer::AddFromDB(pObject, dwPosition);
		}
	}

    return bResult;
}