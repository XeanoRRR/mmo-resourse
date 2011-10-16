// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "..\BaseObject.h"
#include "..\Player.h"
#include "..\Goods\CGoods.h"
#include "..\Message Packaging\CS2CContainerObjectMove.h"
#include "..\Message Packaging\CS2CContainerObjectAmountChange.h"
#include "CContainer.h"
#include "CGoodsShadowContainer.h"
#include "..\Listener\CGoodsListListener.h"



//##ModelId=41E5EFB4001F
CGoodsShadowContainer::CGoodsShadowContainer()
{
	// TODO: Add your specialized code here.
}

//##ModelId=41E5EFB702AF
//CGoodsShadowContainer::CGoodsShadowContainer(LONG lID)
//{
//	// TODO: Add your specialized code here.
//}

//##ModelId=41E5EFBD008C
CGoodsShadowContainer::~CGoodsShadowContainer()
{
	// TODO: Add your specialized code here.
	Release();
}

//##ModelId=41E745790186
BOOL CGoodsShadowContainer::AddShadow(const CGUID& guid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	goodsshadowIt it = m_mGoodsShadow.find( guid );
	if( it != m_mGoodsShadow.end() )
	{
		DWORD dwPosition = 0;
		if( QueryGoodsPosition(guid, dwPosition) )
		{
			CGoods* pGoods = GetGoods( dwPosition );
			if( pGoods )
			{
				DWORD dwOriginalAmount = pGoods -> GetAmount();

				CS2CContainerObjectMove comMsg;
				comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
				comMsg.SetSourceContainer(it->second.lOriginalContainerType,
					it->second.OriginalContainerID,it->second.dwOriginalGoodsPosition);
				comMsg.SetSourceContainerExtendID(it->second.lOriginalContainerExtendID);
				
				comMsg.SetDestinationContainer( GetOwnerType(), GetOwnerID(), dwPosition );
				comMsg.SetDestinationContainerExtendID( m_lContainerExtendID );
				comMsg.SetDestinationObject( TYPE_GOODS, it -> second.guGoodsID );
				comMsg.SetDestinationObjectAmount( it -> second.dwGoodsAmount );

				pGoods -> SetAmount( it -> second.dwGoodsAmount );
				pGoods -> SerializeForOldClient( &comMsg.GetObjectStream() );
				pGoods -> SetAmount( dwOriginalAmount );

				//##�����Ҫ��չ�����Ϣ���͵Ĳ���.OwnerID��һ���Ự��ID����Ҫ�ܶԻỰ��
				//##��������ҷ��͡�
				if( GetOwnerType() == TYPE_PLAYER )
				{
					comMsg.Send( GetOwnerID() );
				}
				else if( GetOwnerType() == TYPE_SESSION )
				{
					comMsg.SendToSession( GetOwnerID() );
				}
			}
		}
		//##�˳�֮ǰ��Ҫ֪ͨ���еļ�����?
		//##����������ʵ�ʵ���Ʒ,�Ƿ��б�Ҫ֪ͨ?
		//##��ֻ��֪ͨ����������Ʒ���룬���ǲ���֪�����ϸ�ڡ�
		for( size_t i = 0; i < m_vListeners.size(); i ++ )
		{
			if( m_vListeners[i] )
			{
				m_vListeners[i] -> OnObjectAdded( this, NULL, 0, NULL );
			}
		}
		bResult = TRUE;
	}
	return bResult;
}

BOOL CGoodsShadowContainer::Add(CBaseObject* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid)
{	
	BOOL bResult = FALSE;
	if( pPreviousContainer && IsFull() == FALSE )
	{
		CGoods* pGoods	= dynamic_cast<CGoods*>( pObject );
		if( pGoods )
		{			
			CGUID	guGoodsID	= pGoods ->GetExID();
			DWORD	dwAmount	= pGoods -> GetAmount();
			DWORD	dwBaseIndex	= pGoods -> GetBasePropertiesIndex();							

			//##Ȼ�����������������Ӱ��.
			tagGoodsShadow tShadow;
			tShadow.lOriginalContainerType		= pPreviousContainer -> lContainerType;
			tShadow.OriginalContainerID		= pPreviousContainer -> ContainerID;
			tShadow.lOriginalContainerExtendID	= pPreviousContainer -> lContainerExtendID;
			tShadow.dwOriginalGoodsPosition		= pPreviousContainer -> dwGoodsPosition;
			tShadow.dwGoodsAmount				= dwAmount;					
			tShadow.guGoodsID					= guGoodsID;
			tShadow.dwGoodsBasePropertiesIndex	= dwBaseIndex;
			m_mGoodsShadow[guGoodsID]			= tShadow;				
			bResult								= TRUE;		
		}
	}
	return bResult;
}

BOOL CGoodsShadowContainer::Add(DWORD dwPosition, CGoods* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return CGoodsShadowContainer::Add( pObject, pPreviousContainer, pVoid );
}

//##ModelId=41E5F0080177
CBaseObject* CGoodsShadowContainer::Remove(DWORD dwPosition, DWORD dwAmount, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	//CBaseObject* pResult = NULL;
	return CGoodsContainer::Remove( dwPosition, dwAmount, pVoid );
	//CGoods* pGoods = GetGoods( dwPosition );
	//if( pGoods )
	//{
	//	DWORD dwHashValue = CContainer::CalculateHashValue(	pGoods -> GetType(), pGoods -> GetID() );
	//	CGoodsContainer* pContainer = GetGoodsSourceContainer( dwHashValue );
	//	if( pContainer )
	//	{
	//		DWORD dwOriginalPosition = 0;
	//		if( pContainer -> QueryGoodsPosition(dwHashValue, dwOriginalPosition) )
	//		{
	//			pResult = dynamic_cast<CGoods*>( pContainer -> Remove( dwOriginalPosition, 
	//				dwAmount, pVoid) );
	//		}
	//	}
	//	//##����ж�����������Ϣ���ͻ��ˣ����ٻ�ɾ������Ʒ����
	//	RemoveShadow( dwHashValue );
	//}
	//return pResult;
}

//##ModelId=41E5F00F0251
VOID CGoodsShadowContainer::Clear(LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	// m_lContainerExtendID = 0;
	CGoodsContainer::Clear();
	m_mGoodsShadow.clear();
}

//##ModelId=41E5F01503C8
VOID CGoodsShadowContainer::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_lContainerExtendID = 0;
	CGoodsContainer::Release();
	m_mGoodsShadow.clear();
}


//##ModelId=41E7635D0000
VOID CGoodsShadowContainer::SetContainerExtendID(LONG lID)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_lContainerExtendID = lID;
}

//##ModelId=41F0C6990048
LONG CGoodsShadowContainer::GetContainerExtendID()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return m_lContainerExtendID;
}

//##ModelId=41E5F01B038A
VOID CGoodsShadowContainer::AI()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
}

//##ModelId=41E5F02203A9
BOOL CGoodsShadowContainer::IsFull()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return FALSE;
}

//##ModelId=41E5F76003D8
VOID CGoodsShadowContainer::TraversingContainer(CContainerListener* pListener)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	if( pListener )
	{
		goodsshadowIt it = m_mGoodsShadow.begin();
		for( ; it != m_mGoodsShadow.end(); it ++ )
		{
			CBaseObject* pObject = Find( it -> first );
			if( pObject )
			{
				CGoods* pGoods = dynamic_cast<CGoods*>( pObject );
				if( pGoods && pGoods -> GetAmount() == it -> second.dwGoodsAmount )
				{
					pListener -> OnTraversingContainer( this, pGoods );
				}
			}
		}

		//##�����һ������
		CGoodsListListener* pGoodsList = dynamic_cast<CGoodsListListener*>( pListener );
		if( pGoodsList )
		{
			if( m_mGoodsShadow.size() != pGoodsList -> m_vGoodsID.size() )
			{
				pGoodsList -> m_bIsAllGoodsExist = FALSE;
			}
		}
	}
}

/*
DWORD CGoodsShadowContainer::GetContentsWeight()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	DWORD dwResult = 0;
	goodsshadowIt it = m_mGoodsShadow.begin();
	for( ; it != m_mGoodsShadow.end(); it ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>( Find(it -> first) );
		if( pGoods )
		{
			dwResult += pGoods -> GetWeight();
		}
	}
	return dwResult;
}*/

//##ModelId=41E5F7760290
BOOL CGoodsShadowContainer::QueryGoodsPosition(CGoods* pGoods, DWORD& dwPosition)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	if( pGoods )
	{
		bResult = QueryGoodsPosition( pGoods->GetExID(), dwPosition );
	}
	return bResult;
}

//##ModelId=41E5F77D0222
BOOL CGoodsShadowContainer::QueryGoodsPosition(const CGUID& guid, DWORD& dwPosition)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult	= FALSE;
	DWORD dwValue	= 0;
	goodsshadowIt it = m_mGoodsShadow.begin();
	for( ; it != m_mGoodsShadow.end(); it ++ )
	{
		if( it -> first == guid )
		{
			bResult = TRUE;
			break;
		}
		++ dwValue;
	}
	if( bResult )
	{
		dwPosition = dwValue;
	}
	return bResult;
}

//##ModelId=41E5F78900FA
BOOL CGoodsShadowContainer::IsGoodsExisted(DWORD dwGoodsBasePropertiesIndex)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	goodsshadowIt it = m_mGoodsShadow.begin();
	for( ; it != m_mGoodsShadow.end(); it ++ )
	{
		if( it -> second.dwGoodsBasePropertiesIndex == 
			dwGoodsBasePropertiesIndex )
		{
			if( Find(it -> first) )
			{
				bResult = TRUE;
				break;
			}
		}
	}
	return bResult;
}

//##ModelId=41E5F791002E
CGoods* CGoodsShadowContainer::GetTheFirstGoods(DWORD dwGoodsBasePropertiesIndex)
{
	// TODO: Add your specialized code here.
	CGoods* pResult = NULL;
	goodsshadowIt it = m_mGoodsShadow.begin();
	for( ; it != m_mGoodsShadow.end(); it ++ )
	{
		if( it -> second.dwGoodsBasePropertiesIndex == 
			dwGoodsBasePropertiesIndex )
		{
			pResult = dynamic_cast<CGoods*>( Find(it -> first) );
			if( pResult )
			{
				break;
			}
		}
	}
	return pResult;
}

//##ModelId=41E5F797031C
CGoods* CGoodsShadowContainer::GetGoods(DWORD dwPosition)
{
	// TODO: Add your specialized code here.
	CGoods* pResult	= NULL;
	DWORD dwValue	= 0;
	goodsshadowIt it = m_mGoodsShadow.begin();
	for( ; it != m_mGoodsShadow.end(); it ++ )
	{
		if( dwValue == dwPosition )
		{
			pResult = dynamic_cast<CGoods*>( Find(it -> first) );
			break;
		}
		++ dwValue;
	}
	return pResult;
}

//##ModelId=41E5F79F0119
VOID CGoodsShadowContainer::GetGoods(DWORD dwGoodsBasePropertiesIndex, vector<CGoods*> vOut)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	goodsshadowIt it = m_mGoodsShadow.begin();
	for( ; it != m_mGoodsShadow.end(); it ++ )
	{
		if( it -> second.dwGoodsBasePropertiesIndex == 
			dwGoodsBasePropertiesIndex )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>( Find(it -> first) );
			if( pGoods )
			{
				vOut.push_back( pGoods );
			}
		}
	}
}

//##ModelId=41E5F7A70196
DWORD CGoodsShadowContainer::GetGoodsAmount()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return static_cast<DWORD>( m_mGoodsShadow.size() );
}


//##ModelId=41E735D901B5
BOOL CGoodsShadowContainer::RemoveShadow(const CGUID& guid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	goodsshadowIt it = m_mGoodsShadow.find( guid );
	if( it != m_mGoodsShadow.end() )
	{
		DWORD dwPosition = 0;
		if( QueryGoodsPosition(guid, dwPosition) )
		{
			CS2CContainerObjectMove comMsg;
			comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
			comMsg.SetSourceContainer( GetOwnerType(), GetOwnerID(), dwPosition );
			comMsg.SetSourceContainerExtendID( m_lContainerExtendID );
			comMsg.SetSourceObject( TYPE_GOODS,it->second.guGoodsID );
			//##�����Ҫ��չ�����Ϣ���͵Ĳ���.OwnerID��һ���Ự��ID����Ҫ�ܶԻỰ��
			//##��������ҷ��͡�
			if( GetOwnerType() == TYPE_PLAYER )
			{
				comMsg.Send( GetOwnerID() );
			}
			else if( GetOwnerType() == TYPE_SESSION )
			{
				comMsg.SendToSession( GetOwnerID() );
			}
		}
		//##ɾ��֮ǰ��Ҫ֪ͨ���еļ�����?
		//##��������Ƴ�ʵ�ʵ���Ʒ,�Ƿ��б�Ҫ֪ͨ?
		for( size_t i = 0; i < m_vListeners.size(); i ++ )
		{
			if( m_vListeners[i] )
			{
				m_vListeners[i] -> OnObjectAdded( this, NULL, 0, NULL );
			}
		}

		m_mGoodsShadow.erase( it );
		bResult = TRUE;
	}
	return bResult;
}

//##ModelId=41E61B590109
//##�ýӿ���Ϊ��ͬԴ��������ͬ����ʱ��ʹ�õġ��ýӿ���Դ��������Ʒ����֣����ѵ������ƶ�����ɾ����
//##��ʱ�򱣳ֺ�Դ������һ���ԡ�
BOOL CGoodsShadowContainer::OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	//##��Ϊ�޷���ȡ���յ�������Ŀǰ��ʱ����Ʒ�ľ���ɾ������
	if( pObj )
	{
		//DWORD dwHashValue = CContainer::CalculateHashValue( pObj -> GetType(), pObj -> GetID() );
		CGUID	guid = pObj->GetExID();
		goodsshadowIt it = m_mGoodsShadow.find( guid );
		if( it != m_mGoodsShadow.end() )
		{
			//##�ҵ���,���¸���Ʒ�ľ�����Ϣ.
			if( it -> second.dwGoodsAmount != dwAmount )
			{
				//##���ԭ��Ʒ�����͸ı��������ͬ
				it -> second.dwGoodsAmount += dwAmount;

				//##���������ı���Ϣ��
				DWORD dwPosition = 0;
				if( QueryGoodsPosition(it -> first, dwPosition) )
				{
					CS2CContainerObjectAmountChange coacMsg;
					coacMsg.SetSourceContainer( GetOwnerType(), GetOwnerID(), dwPosition );
					coacMsg.SetSourceContainerExtendID( GetContainerExtendID() );
					coacMsg.SetObject( pObj -> GetType(),guid);
					coacMsg.SetObjectAmount( it -> second.dwGoodsAmount );
					coacMsg.SendToSession( GetOwnerID() );
				}
			}
		}
	}
	return TRUE;
}

//##ModelId=41E61B6701F4
//##�ýӿ���Ϊ��ͬԴ��������ͬ����ʱ��ʹ�õġ��ýӿ���Դ��������Ʒ����֣����ѵ������ƶ�����ɾ����
//##��ʱ�򱣳ֺ�Դ������һ���ԡ�
BOOL CGoodsShadowContainer::OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	if( pObj )
	{
		//DWORD dwHashValue = CContainer::CalculateHashValue( pObj -> GetType(), pObj -> GetID() );
		CGUID	guid = pObj->GetExID();
		goodsshadowIt it = m_mGoodsShadow.find( guid );
		if( it != m_mGoodsShadow.end() )
		{
			//##�ҵ���,���¸���Ʒ�ľ�����Ϣ.
			//##���ԭ��Ʒ�����͸ı��������ͬ
			if( it -> second.dwGoodsAmount >= dwAmount )
			{
				it -> second.dwGoodsAmount -= dwAmount;
				if( it -> second.dwGoodsAmount == 0 )
				{
					RemoveShadow( guid );
				}
				else
				{
					//##���������ı���Ϣ��
					DWORD dwPosition = 0;
					if( QueryGoodsPosition(it -> first, dwPosition) )
					{
						CS2CContainerObjectAmountChange coacMsg;
						coacMsg.SetSourceContainer( GetOwnerType(), GetOwnerID(), dwPosition );
						coacMsg.SetSourceContainerExtendID( GetContainerExtendID() );
						coacMsg.SetObject( pObj -> GetType(),guid );
						coacMsg.SetObjectAmount( it -> second.dwGoodsAmount );
						coacMsg.SendToSession( GetOwnerID() );
					}
				}
			}
			else
			{
				RemoveShadow( guid );
			}
		}
	}
	return TRUE;
}

//##ModelId=41E61B73036B
BOOL CGoodsShadowContainer::OnTraversingContainer(CContainer* pContainer, CBaseObject* pObject)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return TRUE;
}

//##ModelId=41ECAAB8035B
BOOL CGoodsShadowContainer::GetGoodsOriginalContainerInformation(const CGUID& guid, tagPreviousContainer& tOriginalContainer)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	goodsshadowIt it = m_mGoodsShadow.find( guid );
	if( it != m_mGoodsShadow.end() )
	{
		tOriginalContainer.lContainerType		= it -> second.lOriginalContainerType;
		tOriginalContainer.ContainerID			= it -> second.OriginalContainerID;
		tOriginalContainer.lContainerExtendID	= it -> second.lOriginalContainerExtendID;
		tOriginalContainer.dwGoodsPosition		= it -> second.dwOriginalGoodsPosition;
		bResult = TRUE;
	}
	return bResult;
}

LONG CGoodsShadowContainer::GetGoodsSourceContainerExtendID(const CGUID& guid)
{
	LONG lResult = 0;
	goodsshadowIt it = m_mGoodsShadow.find( guid );
	if( it != m_mGoodsShadow.end() )
	{
		lResult = it -> second.lOriginalContainerExtendID;
	}
	return lResult;
}

//##ModelId=41E656140271
CGoodsContainer* CGoodsShadowContainer::GetGoodsSourceContainer(const CGUID& guid)
{
	// TODO: Add your specialized code here.
	CGoodsContainer* pResult = NULL;
	goodsshadowIt it = m_mGoodsShadow.find( guid );
	if( it != m_mGoodsShadow.end() )
	{
		if( it -> second.lOriginalContainerType == TYPE_PLAYER )
		{
			CPlayer* pPlayer = GetGame() -> FindPlayer( it -> second.OriginalContainerID );
			if( pPlayer )
			{
				switch( it -> second.lOriginalContainerExtendID )
				{
				case CS2CContainerObjectMove::PEI_PACKET:
					pResult = pPlayer -> getOriginPack();
					break;
				case CS2CContainerObjectMove::PEI_EQUIPMENT:
					pResult = &( pPlayer -> m_cEquipment );
					break;
				case CS2CContainerObjectMove::PEI_WALLET:
					pResult = &( pPlayer -> m_cWallet );
					break;
				case CS2CContainerObjectMove::PEI_GEM:
					pResult = &( pPlayer -> m_cGem );
					break;
				case CS2CContainerObjectMove::PEI_PACK:
					pResult=pPlayer->getSubpackContainer();
					break;
				case CS2CContainerObjectMove::PEI_PACK1:
					pResult=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
					break;
				case CS2CContainerObjectMove::PEI_PACK2:
					pResult=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
					break;
				case CS2CContainerObjectMove::PEI_PACK3:
					pResult=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
					break;
				case CS2CContainerObjectMove::PEI_PACK4:
					pResult=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
					break;
				case CS2CContainerObjectMove::PEI_PACK5:
					pResult=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
					break;
				}
			}
		}
	}
	return pResult;
}


CGoods* CGoodsShadowContainer::GetGoodsByPos(LONG ContainerID,LONG lPos)
{

	CGoods* pGoods=NULL;
	CPlayer* pPlayer = GetGame() -> FindPlayer( this->GetOwnerID() );
	CVolumeLimitGoodsContainer* pContainer=NULL;
	//if(pPlayer)
		//pGoods=pPlayer->FindGoods(ContainerID,lPos);
	//return pGoods;
	
	
	if( pPlayer )
	{
		switch(ContainerID)
		{
		case CS2CContainerObjectMove::PEI_PACKET:
			pContainer = pPlayer -> getOriginPack();
			break;		
		case CS2CContainerObjectMove::PEI_PACK1:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK2:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK3:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK4:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK5:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
			break;
		}
		if(pContainer)
		{
			pGoods=pContainer->GetGoods(lPos);
		}
	}
	return pGoods;
}

DWORD CGoodsShadowContainer::GetGoodsAmountByName(const char* strName)
{
	DWORD dwAmount=0;
	
	return dwAmount;
}

DWORD CGoodsShadowContainer::GetGoodsAmountByType(int nType)
{
	DWORD dwAmount=0;
	
	return dwAmount;
}

/*
CGoods* CGoodsShadowContainer::GetGoodsById(LONG ContainerID,LONG lId)
{
	CGoods* pGoods=NULL;
	CPlayer* pPlayer = GetGame() -> FindPlayer( this->GetOwnerID() );
	CVolumeLimitGoodsContainer* pContainer=NULL;
	//if(pPlayer)
	//pGoods=pPlayer->FindGoods(ContainerID,lPos);
	//return pGoods;
	if( pPlayer )
	{
		switch(ContainerID)
		{
		case CS2CContainerObjectMove::PEI_PACKET:
			pContainer = pPlayer -> getOriginPack();
			break;		
		case CS2CContainerObjectMove::PEI_PACK1:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK2:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK3:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK4:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK5:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
			break;
		}
		if(pContainer)
		{
			pGoods=pContainer->GetGoods()
		}
	}
	return pGoods;
}*/

//##ModelId=41E5EFDF01A5
CBaseObject* CGoodsShadowContainer::Find(const CGUID& guid)
{
	// TODO: Add your specialized code here.
	CBaseObject* pResult = NULL;
	goodsshadowIt it = m_mGoodsShadow.find( guid );
	if( it != m_mGoodsShadow.end() )
	{
		CGoodsContainer* pContainer = GetGoodsSourceContainer( it -> first );
		if( pContainer )
		{
			pResult = pContainer -> Find( TYPE_GOODS, it -> second.guGoodsID);
		}
	}
	return pResult;
}

//##ModelId=41E5F001029F
CBaseObject* CGoodsShadowContainer::Remove(const CGUID& guid, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	//## �ú�����Ȼ��һ���ӿ�,��ʵ��Ҫ���Ƴ�����,���ǵ�����ԭ�������Ƴ�����.
	//## �Ƴ���Ϣ��Ȼ��ԭ����ʵ�ֵģ�������۳ɹ����ֻҪ���������ҵ������
	//## ����ֵ���ͱ���Ѹ���Ʒ�Ľڵ��Ƴ��������ҷ���ɾ����Ϣ���ͻ��ˡ�
	CGoods* pResult = NULL;
	goodsshadowIt it = m_mGoodsShadow.find( guid );
	if( it != m_mGoodsShadow.end() )
	{
		CGUID guGoodId = it -> second.guGoodsID;
		CGoodsContainer* pContainer = GetGoodsSourceContainer( it -> first );
		if( pContainer )
		{
			DWORD dwPosition = 0;
			if( pContainer -> QueryGoodsPosition(guid, dwPosition) )
			{
				CGoods* pGoods = pContainer -> GetGoods( dwPosition );
				if( pGoods && pGoods -> GetAmount() == it -> second.dwGoodsAmount )
				{
					pResult = dynamic_cast<CGoods*>( pContainer -> Remove(TYPE_GOODS, 
						guGoodId, pVoid) );
				}
			}
		}
		//##������Ϣ���ͻ�����ɾ�������ڸ���Ʒ����
		RemoveShadow( guid );
	}
	return pResult;
}

CGoods* CGoodsShadowContainer::RemoveEx(const CGUID& PlayerId,LONG ContainerID,LONG lPos)
{
	CVolumeLimitGoodsContainer* pContainer = NULL;
	
	CGoods* pGoods=NULL;
	CPlayer* pPlayer = GetGame() -> FindPlayer( PlayerId );
	if( pPlayer )
	{
		switch( ContainerID )
		{
		case CS2CContainerObjectMove::PEI_PACKET:
			pContainer = pPlayer -> getOriginPack();
			break;		
		case CS2CContainerObjectMove::PEI_PACK1:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK2:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK3:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK4:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK5:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
			break;
		}
	}
	if( pContainer )
	{
		
		pGoods=pContainer->GetGoods(lPos);
		if(pContainer->Remove(pGoods->GetExID(),NULL))
		{
			return pGoods;
		}
		else
			return NULL;
	}
	return pGoods;
}

LONG CGoodsShadowContainer::RemoveEx(const CGUID& PlayerId,LONG ContainerID,LONG lPos,LONG lNum)
{	
	LONG lRet=0;
	if(lNum<=0)
		return lRet;
	CGoodsContainer* pContainer = NULL;
	
	CGoods* pGoods=NULL;
	CPlayer* pPlayer = GetGame() -> FindPlayer( PlayerId );
	if( pPlayer )
	{
		switch( ContainerID )
		{
		case CS2CContainerObjectMove::PEI_PACKET:
			pContainer = pPlayer -> getOriginPack();
			break;
		case CS2CContainerObjectMove::PEI_EQUIPMENT:
			pContainer = &( pPlayer -> m_cEquipment );
			break;
		case CS2CContainerObjectMove::PEI_WALLET:
			pContainer = &( pPlayer -> m_cWallet );
			break;
		case CS2CContainerObjectMove::PEI_GEM:
			pContainer = &( pPlayer -> m_cGem );
			break;
		case CS2CContainerObjectMove::PEI_PACK:
			pContainer=pPlayer->getSubpackContainer();
			break;
		case CS2CContainerObjectMove::PEI_PACK1:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK2:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK3:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK4:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK5:
			pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
			break;
		}
	}
	if( pContainer )
	{
		pGoods=pContainer->GetGoods(lPos);
		if(pGoods->GetAmount()>lNum)
		{
			pGoods->SetAmount(pGoods->GetAmount()-lNum);
			lRet=pGoods->GetAmount();			
		}		
	}
	return lRet;
}


CGoodsShadowContainer::tagGoodsShadow* CGoodsShadowContainer::RemoveByGoodsId(DWORD dwId)
{
	map<CGUID,tagGoodsShadow,hash_guid_compare>::iterator tagIter=m_mGoodsShadow.begin();
	for(;tagIter!=m_mGoodsShadow.end();tagIter++)
	{
		if(tagIter->second.dwGoodsBasePropertiesIndex==dwId)
			break;
	}
	if(tagIter!=m_mGoodsShadow.end())
	{
		tagGoodsShadow* pTag=MP_NEW tagGoodsShadow;
		pTag->lOriginalContainerType=tagIter->second.lOriginalContainerType;
		pTag->OriginalContainerID=tagIter->second.OriginalContainerID;
		pTag->lOriginalContainerExtendID=tagIter->second.lOriginalContainerExtendID;
		pTag->guGoodsID=tagIter->second.guGoodsID;
		pTag->dwOriginalGoodsPosition=tagIter->second.dwOriginalGoodsPosition;
		pTag->dwGoodsBasePropertiesIndex=tagIter->second.dwGoodsBasePropertiesIndex;
		pTag->dwGoodsAmount=tagIter->second.dwGoodsAmount;		
		m_mGoodsShadow.erase(tagIter);
		return pTag;
	}
	return NULL;	
}

VOID CGoodsShadowContainer::RemoveShadowByGoodsIndex(LONG lGoodsId)
{
	vector<goodsshadowIt> gcVec;
	goodsshadowIt it=m_mGoodsShadow.begin();
	for(;it!=m_mGoodsShadow.end();it++)
	{
		if(it->second.dwGoodsBasePropertiesIndex==lGoodsId)
		{
			gcVec.push_back(it);
		}
	}

	vector<goodsshadowIt>::iterator shadowIter=gcVec.begin();
	for(;shadowIter!=gcVec.end();shadowIter++)
	{
		m_mGoodsShadow.erase(*shadowIter);
	}
}

VOID CGoodsShadowContainer::RemoveShadowByGoodsGuid(CGUID& guid)
{	
	m_mGoodsShadow.erase(guid);
}

VOID CGoodsShadowContainer::AddShadowTag(CGoodsShadowContainer::tagGoodsShadow* pTag)
{
	tagGoodsShadow tShadow;
	tShadow.lOriginalContainerType		= pTag->lOriginalContainerType;
	tShadow.OriginalContainerID		= pTag->OriginalContainerID;
	tShadow.lOriginalContainerExtendID	= pTag ->lOriginalContainerExtendID;
	tShadow.dwOriginalGoodsPosition		= pTag->dwOriginalGoodsPosition;
	tShadow.dwGoodsAmount				= pTag->dwGoodsAmount;	
	tShadow.guGoodsID					= pTag->guGoodsID;
	tShadow.dwGoodsBasePropertiesIndex	= pTag->dwGoodsBasePropertiesIndex;
	//DWORD dwHashValue					= CContainer::CalculateHashValue( TYPE_GOODS, lGoodsID );
	m_mGoodsShadow[pTag->guGoodsID]		= tShadow;
}

DWORD CGoodsShadowContainer::GetGoodsNumByIndex(LONG lId)
{
	DWORD dwRet=0;
	map<CGUID,tagGoodsShadow,hash_guid_compare>::iterator it=m_mGoodsShadow.begin();
	for(;it!=m_mGoodsShadow.end();it++)
	{
		if(it->second.dwGoodsBasePropertiesIndex==lId)
		{
			dwRet+=it->second.dwGoodsAmount;
		}
	}
	return dwRet;
}

//##ModelId=41F46E6100A9
BOOL CGoodsShadowContainer::GetShadowPosition(tagPreviousContainer* ptPreviousContainer, DWORD& dwPosition)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	if( ptPreviousContainer )
	{
		goodsshadowIt it = m_mGoodsShadow.begin();
		for( ; it != m_mGoodsShadow.end(); it ++ )
		{
			if( ptPreviousContainer -> lContainerType == it -> second.lOriginalContainerType &&
				ptPreviousContainer -> ContainerID == it -> second.OriginalContainerID &&
				ptPreviousContainer -> lContainerExtendID == it -> second.lOriginalContainerExtendID &&
				ptPreviousContainer -> dwGoodsPosition == it -> second.dwOriginalGoodsPosition )
			{
				bResult = QueryGoodsPosition( it -> first, dwPosition );
				break;
			}
		}
	}
	return bResult;
}

CBaseObject* CGoodsShadowContainer::Find( CBaseObject* pObj )
{
	return CGoodsContainer::Find( pObj );
}

CBaseObject* CGoodsShadowContainer::Find( LONG oType,const CGUID& guid)
{
	return CGoodsContainer::Find( oType, guid );
}

CGoods* CGoodsShadowContainer::FindByGuid(LONG oType,const CGUID& guid,DWORD& dwNum)
{
	goodsshadowIt it=m_mGoodsShadow.find(guid);
	if(it!=m_mGoodsShadow.end())
	{
		dwNum=it->second.dwGoodsAmount;
		CGoods* pGoods=dynamic_cast<CGoods*>(Find(TYPE_GOODS, it -> first) );
		return pGoods;
	}
	return NULL;
}

//##ModelId=41E6485E0148
CGoodsShadowContainer::tagGoodsShadow::tagGoodsShadow()
{
	// TODO: Add your specialized code here.
		lOriginalContainerType		= 0;
		OriginalContainerID		= NULL_GUID;
		lOriginalContainerExtendID	= 0;
		dwOriginalGoodsPosition		= 0;
		//lGoodsID					= 0;
		dwGoodsAmount				= 0;
		dwGoodsBasePropertiesIndex	= 0;
}

//##ModelId=41E64861006D
CGoodsShadowContainer::tagGoodsShadow::~tagGoodsShadow()
{
	// TODO: Add your specialized code here.
	//vGoodsShadowStream.clear();
}
