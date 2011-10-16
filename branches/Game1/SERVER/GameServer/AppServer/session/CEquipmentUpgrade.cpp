// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "CEquipmentUpgrade.h"
#include "..\Player.h"
#include "..\ServerRegion.h"
#include "..\Listener\CUpgradePriceListener.h"
#include "..\Goods\CGoodsFactory.h"
#include "..\..\..\setup\globesetup.h"
#include "../../../setup/LogSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEquipmentUpgrade::CEquipmentUpgrade()
{	
}

CEquipmentUpgrade::~CEquipmentUpgrade()
{
	// TODO: Add your specialized code here.
	Release();
}

BOOL CEquipmentUpgrade::IsPlugAvailable()
{	
	BOOL bResult = FALSE;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>( GetOwner() );
	if( pPlayer )
	{
		CServerRegion* pRegion = dynamic_cast<CServerRegion*>( pPlayer -> GetFather() );
		if( pRegion )
		{
			if( pRegion -> GetSecurity( pPlayer -> GetTileX(), pPlayer -> GetTileY() ) == CRegion::SECURTIY_SAFE )
			{
				bResult = TRUE;
			}
		}
	}
	return bResult;
}

BOOL CEquipmentUpgrade::OnSessionEnded()
{
	return OnPlugEnded();
}

BOOL CEquipmentUpgrade::OnSessionAborted()
{
	return OnPlugEnded();
}

BOOL CEquipmentUpgrade::OnPlugInserted()
{
	m_cUpgradeContainer.SetOwner( TYPE_SESSION, m_SessionID );
	m_cUpgradeContainer.SetContainerExtendID( Get32ID() << 8 );

	//##�Ѽ��������뵽��ҵ������ڡ�
	CPlayer* pPlayer = dynamic_cast<CPlayer*>( GetOwner() );
	if( pPlayer )
	{
		pPlayer -> getOriginPack()->AddListener( &m_cUpgradeContainer );
		pPlayer -> m_cEquipment.AddListener( &m_cUpgradeContainer );
	}

	return TRUE;
}

BOOL CEquipmentUpgrade::OnPlugAborted()
{	
	return OnPlugEnded();
}

BOOL CEquipmentUpgrade::OnPlugEnded()
{
	//##ֱ�ӷ��͹رս������Ϣ���ͻ��ˡ�
	//##�Ѽ��������뵽��ҵ������ڡ�
	CPlayer* pPlayer = dynamic_cast<CPlayer*>( GetOwner() );
	if( pPlayer )
	{
		pPlayer ->getOriginPack()->RemoveListener( &m_cUpgradeContainer );
		pPlayer -> m_cEquipment.RemoveListener( &m_cUpgradeContainer );
	}

	CMessage msg( MSG_S2C_CLOSEGOODSUPGRADE );
	msg.SendToPlayer( GetOwnerID() );

	return TRUE;
}

BOOL CEquipmentUpgrade::OnChangeState(LONG lPlugID, LONG lStateID, BYTE* pData)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return TRUE;
}

VOID CEquipmentUpgrade::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_cUpgradeContainer.Clear();
}

CContainer* CEquipmentUpgrade::GetContainer(LONG lID)
{
	// TODO: Add your specialized code here.
	CContainer* pResult = NULL;
	if( lID == 0 )
	{
		pResult = &m_cUpgradeContainer;
	}
	return pResult;
}


DWORD CEquipmentUpgrade::GetSucceedResult()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	// �ɹ���Ľ��������ʾ������������ʾ����������������������
	DWORD dwResult = 0;

	//##���Ȼ�ȡ������ʯ����Ϣ.������ʯ���������2���ĸ�������
	CGoods* pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_BASE_GEM );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_SUCCEED_RESULT, 1 );
		if( dwOperation > dwResult )
		{
			dwResult = dwOperation;
		}
	}

	//##Ȼ���ȡ��ʯ1,2,3����Ϣ
	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_ONE );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_SUCCEED_RESULT, 1 );
		if( dwOperation > dwResult )
		{
			//##��ȡ���༶����
			LONG lProbability = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_SUCCEED_RESULT, 2 );
			if( random(100) <= lProbability )
			{
				dwResult = dwOperation;
			}
		}
	}

	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_TWO );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_SUCCEED_RESULT, 1 );
		if( dwOperation > dwResult )
		{
			//##��ȡ���༶����
			LONG lProbability = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_SUCCEED_RESULT, 2 );
			if( random(100) <= lProbability )
			{
				dwResult = dwOperation;
			}
		}
	}

	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_THREE );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_SUCCEED_RESULT, 1 );
		if( dwOperation > dwResult )
		{
			//##��ȡ���༶����
			LONG lProbability = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_SUCCEED_RESULT, 2 );
			if( random(100) <= lProbability )
			{
				dwResult = dwOperation;
			}
		}
	}

	return dwResult;
}

DWORD CEquipmentUpgrade::GetFailedResult()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	// ʧ�ܺ�Ľ����1.ʲô��������2.��1����3.����0����4.����
	DWORD dwResult = 4;

	//##���Ȼ�ȡ������ʯ����Ϣ.
	CGoods* pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_BASE_GEM );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_FAILED_RESULT, 1 );
		if( dwOperation > 0 && dwOperation < dwResult )
		{
			dwResult = dwOperation;
		}
	}

	//##Ȼ���ȡ��ʯ1,2,3����Ϣ
	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_ONE );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_FAILED_RESULT, 1 );
		if( dwOperation > 0 && dwOperation < dwResult )
		{
			dwResult = dwOperation;
		}
	}

	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_TWO );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_FAILED_RESULT, 1 );
		if( dwOperation > 0 && dwOperation < dwResult )
		{
			dwResult = dwOperation;
		}
	}

	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_THREE );
	if( pGoods )
	{
		DWORD dwOperation = pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_UPGRADE_FAILED_RESULT, 1 );
		if( dwOperation > 0 && dwOperation < dwResult )
		{
			dwResult = dwOperation;
		}
	}

	return dwResult;
}

//##ModelId=423FBACD035B
DWORD CEquipmentUpgrade::GetProbability()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	LONG lProbability = 0;

	//##���Ȼ�ȡ������ʯ����Ϣ.
	CGoods* pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_BASE_GEM );
	if( pGoods )
	{
		lProbability += pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_PROBABILITY, 1 );
	}

	//##Ȼ���ȡ��ʯ1,2,3����Ϣ
	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_ONE );
	if( pGoods )
	{
		lProbability += pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_PROBABILITY, 1 );
	}

	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_TWO );
	if( pGoods )
	{
		lProbability += pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_PROBABILITY, 1 );
	}

	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_THREE );
	if( pGoods )
	{
		lProbability += pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_PROBABILITY, 1 );
	}

	//##�ٻ�ȡ��Ʒ����ĸ���ֵ
	pGoods = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_EQUIPMENT );
	if( pGoods )
	{
		lProbability += pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_PROBABILITY, 1 );
	}

	//##�ü��߽�
	if( lProbability < 0   ) lProbability = 0;
	if( lProbability > 100 ) lProbability = 100;

	return static_cast<DWORD>( lProbability );
}

//##ModelId=4240C60C00BB
DWORD CEquipmentUpgrade::GetUpgradePrice()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CUpgradePriceListener lListener;
	m_cUpgradeContainer.TraversingContainer( &lListener );
	return lListener.m_dwPrice;
}

//##ModelId=423FBB240000
BOOL CEquipmentUpgrade::DoesUpgradeValid()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	// ������Ч��������1.װ����������2.�л�����ʯ���Ҷ�Ӧ������ǰ�ȼ���3.���㹻��Ǯ��
	BOOL bResult = FALSE;
	CPlayer* pOwner = dynamic_cast<CPlayer*>( GetOwner() );
	if( pOwner )
	{
		CServerRegion* pRegion = dynamic_cast<CServerRegion*>( pOwner -> GetFather() );
		if( pRegion )
		{
			DWORD dwUpgradePrice = GetUpgradePrice();
			if( pOwner -> GetMoney() >= dwUpgradePrice )
			{
				CGoods* pEquipment = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_EQUIPMENT );
				if( pEquipment && pEquipment -> CanUpgraded() )
				{
					//##��ȡװ���ȼ�
					LONG lEquipmentLevel =1;// pEquipment -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_WEAPON_LEVEL, 1 );

					//##��ȡ������ʯ
					CGoods* pBaseGem = m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_BASE_GEM );
					if( pBaseGem )
					{
						//##��ȡ������ʯ�ļ�����Ϣ.
						LONG lMinLevel = pBaseGem -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_LEVEL, 1 );
						LONG lMaxLevel = pBaseGem -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GEM_LEVEL, 2 );
						if( lMaxLevel < lMinLevel )
						{
							lMaxLevel = lMinLevel;
						}
						//##�ж�����
						if( lMinLevel <= lEquipmentLevel &&
							lMaxLevel >= lEquipmentLevel )
						{
							bResult = TRUE;
						}
						else
						{
							pOwner -> SendNotifyMessage( "������ʯ�ļ��𲻷���Ҫ��" );
						}
					}
					else
					{
						pOwner -> SendNotifyMessage( "������������Ӧ�Ļ�����ʯ��" );
					}
				}
				else 
				{
					pOwner -> SendNotifyMessage( "������������Ӧ��װ����" );
				}
			}
			else
			{
				CHAR szMsg[256];
				sprintf( szMsg, "������Ҫ%dö��ң���Ľ�Ҳ�����", dwUpgradePrice  );
				pOwner -> SendNotifyMessage( szMsg );
			}
		}
	}
	return bResult;
}

//##ModelId=423FBC9A01B5
BOOL CEquipmentUpgrade::Upgrade()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	if( DoesUpgradeValid() )
	{
		CPlayer* pPlayer	= dynamic_cast<CPlayer*>( GetOwner() );
		CGoods* pEquipment	= m_cUpgradeContainer.GetGoods( CEquipmentUpgradeShadowContainer::UEPC_EQUIPMENT );
		if( pPlayer && pEquipment )
		{
			CServerRegion* pRegion = dynamic_cast<CServerRegion*>( pPlayer -> GetFather() );
			if( pRegion )
			{
				DWORD dwUpgradePrice	= GetUpgradePrice();
				DWORD dwCurrentLevel	=1;// pEquipment -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_WEAPON_LEVEL, 1 );
				if( dwCurrentLevel < 99 )
				{
					DWORD dwProbability	= GetProbability();
					BOOL bGoodsUpdated	= TRUE;
					BOOL bGoodsRemoved	= FALSE;
					// ����������Ƿ��㹻��
					if(pPlayer -> GetMoney() >= dwUpgradePrice)
					{
						//##���Ľ�Ǯ
						pPlayer -> SetMoney( pPlayer -> GetMoney() - dwUpgradePrice  );
						if( static_cast<DWORD>( random(100) )+1 <= dwProbability )
							//##�����ɹ�
						{
							DWORD dwSucceedResult = GetSucceedResult();

							//##�ü��߽�
							dwCurrentLevel += dwSucceedResult;
							if( dwCurrentLevel > 99 ) dwCurrentLevel = 99;

							//##�����ɹ�
							CGoodsFactory::UpgradeEquipment( pEquipment, dwCurrentLevel );
							pPlayer -> SendNotifyMessage( "����Ʒ�����ɹ���" );
						}
						else
							//##����ʧ��
						{

							DWORD dwFailedResult = GetFailedResult();
							switch( dwFailedResult )
							{
							case 1:
								//##ʲô������
								bGoodsUpdated = FALSE;
								pPlayer -> SendNotifyMessage( "����Ʒ����ʧ�ܡ�" );
								break;
							case 2:
								//##��1��
								pPlayer -> SendNotifyMessage( "����Ʒ����ʧ�ܲ�����1�ȼ���" );
								if( dwCurrentLevel > 0 )
								{
									CGoodsFactory::UpgradeEquipment( pEquipment, dwCurrentLevel - 1 );
								}
								break;
							case 3:
								//##ֱ�ӽ���0��
								pPlayer -> SendNotifyMessage( "����Ʒ����ʧ�ܣ��ȼ���Ϊ0����" );
								CGoodsFactory::UpgradeEquipment( pEquipment, 0 );
								break;
							case 4:
								//##ɾ����Ʒ
								pPlayer -> SendNotifyMessage( "����Ʒ����ʧ�ܣ���Ĺ��ɣ����ױ��ϡ�" );
								bGoodsUpdated = FALSE;
								bGoodsRemoved = TRUE;
								break;
							}
						}
						//##���¿ͻ�����Ʒ����
						if( bGoodsUpdated )
						{
							vector<BYTE> vGoods;
							pEquipment -> SerializeForOldClient( &vGoods );
							CMessage Msg( MSG_S2C_GOODS_UPDATEPROPERTY );
							Msg.Add( pPlayer->GetExID() );
							//Msg.Add( pEquipment -> GetID() );
							Msg.Add( pEquipment -> GetExID() );
							Msg.Add( static_cast<LONG>( vGoods.size() ) );
							Msg.AddEx( &vGoods[0], static_cast<LONG>( vGoods.size() ) );
							Msg.SendToPlayer( pPlayer->GetExID() );
						}

						if( bGoodsRemoved )
						{
							DeleteGoods( CEquipmentUpgradeShadowContainer::UEPC_EQUIPMENT );
						}

						//##ɾ����װ�����������Ʒ
						DeleteGoods( CEquipmentUpgradeShadowContainer::UEPC_BASE_GEM );
						DeleteGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_ONE );
						DeleteGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_TWO );
						DeleteGoods( CEquipmentUpgradeShadowContainer::UEPC_GEM_THREE );
					}
					else
					{
						pPlayer -> SendNotifyMessage( "�������ò��㣬�޷���������" );
					}
				}
				else 
				{
					pPlayer -> SendNotifyMessage( "�������Ѿ���������ߵȼ����޷���������" );
				}
			}
		}
	}
	return bResult;
}

//##ModelId=424104490167
BOOL CEquipmentUpgrade::DeleteGoods(CEquipmentUpgradeShadowContainer::UPGRADING_EQUIPMENT_PLACE_CELL uepc)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	CGoods* pGoods	 = m_cUpgradeContainer.GetGoods( uepc );
	if( pGoods )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>( GetOwner() );
		if( pPlayer )
		{
			//##��ȡװ��ԭʼλ�����ԡ�
			CGoodsContainer::tagPreviousContainer tPreviousContainer;
			m_cUpgradeContainer.GetGoodsOriginalContainerInformation( pGoods -> GetExID(), tPreviousContainer );

			//##ɾ������Ʒ
			CS2CContainerObjectMove comRemoveMsg;
			comRemoveMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
			comRemoveMsg.SetSourceContainerExtendID( tPreviousContainer.lContainerExtendID );
			CGoods* pRemovedGoods = dynamic_cast<CGoods*>( m_cUpgradeContainer.Remove(pGoods, &comRemoveMsg) );
			if( pRemovedGoods )
			{
				comRemoveMsg.Send( pPlayer->GetExID() );
				CGoodsFactory::GarbageCollect( &pRemovedGoods ,52);
				bResult = TRUE;
			}
		}
	}
	return bResult;
}
