// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "..\Goods\CGoods.h"
#include "..\Goods\CGoodsFactory.h"
#include "..\Listener\CContainerListener.h"
#include "CGoodsContainer.h"
//#include "..\Message Packaging\CS2CContainerObjectMove.h"




//##ModelId=41BF91DC003E
CGoodsContainer::CGoodsContainer()
{
	// TODO: Add your specialized code here.
	m_lOwnerType	= 0;
	m_lOwnerID		= 0;
}

//##ModelId=41BF91EE001F
//CGoodsContainer::CGoodsContainer(LONG lID) : CContainer( lID )
//{
//	// TODO: Add your specialized code here.
//	m_lOwnerType	= 0;
//	m_lOwnerID		= 0;
//}

//##ModelId=41BF91E001E4
CGoodsContainer::~CGoodsContainer()
{
	// TODO: Add your specialized code here.
	m_lOwnerType	= 0;
	m_lOwnerID		= 0;
}

LONG CGoodsContainer::GetOwnerType()
{
	return m_lOwnerType;
}

LONG CGoodsContainer::GetOwnerID()
{
	return m_lOwnerID;
}

//##ModelId=41BF92970167
BOOL CGoodsContainer::Add(CBaseObject* pObject, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return CContainer::Add( pObject, pVoid );
}

//##ModelId=41BF96FB01C5
BOOL CGoodsContainer::Add(DWORD dwPosition, CGoods* pObject, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	//##��Ϊͬλ�����,�������Add����ֻʵ������Ʒ���ڸ�λ�õ�ʱ����2��
	//##��Ʒ������ͬ��״���£������Ʒ�����⣬������ĺ������������ж��Ƿ�
	//##��Ҫ�ص���Ȼ����øú�����
	BOOL bResult = FALSE;
	if( pObject )
	{
		CGoods* pGoods = GetGoods( dwPosition );
		if( pGoods )
		{
			//##���Ҫ�������Ʒ��������ͬ����Ʒ�ǿɶѵ��ġ�
			if( pGoods -> GetBasePropertiesIndex() ==
				pObject -> GetBasePropertiesIndex() &&
				pGoods -> GetMaxStackNumber() > 1 )
			{
				DWORD dwRemainedSlot = pGoods -> GetMaxStackNumber() -
					pGoods -> GetAmount();
				//##���������������ڵ����������ֱ�Ӽ��롣
				if( pObject -> GetAmount() <= dwRemainedSlot )
				{
					pGoods -> SetAmount( pGoods -> GetAmount() + pObject -> GetAmount() );
					CGoodsFactory::GarbageCollect( &pObject );

					//##����Ŀ����������
					//CS2CContainerObjectMove* pMsg = reinterpret_cast<CS2CContainerObjectMove*>( pVoid );
					//if( pMsg )
					//{
					//	pMsg -> SetObject( 0, 0 );
					//	pMsg -> SetObjectAmount( 0 );
					//	pMsg -> GetObjectStream().clear();
					//}

					for( size_t i = 0; i < m_vListeners.size(); i ++ )
					{
						if( m_vListeners[i] )
						{
							//##�úϲ���Ϣ��������Ŀ����Ʒ
							m_vListeners[i] -> OnObjectAdded( this, pGoods, pVoid );
						}
					}

					bResult = TRUE;
				}
				//##����ʲô������
				else
				{
				}
			}
		}
	}
	return bResult;
}
BOOL CGoodsContainer::AddFromDB( CGoods* pObject, DWORD dwPosition )
{
	CGoods* pGoods = GetGoods( dwPosition );

	if( pGoods ) //m_pGoldCoins������
	{
		char str[256];
		char szGuid[40];
		char szExGuid[40];
		
		CGUID guid,guidExsit;
		guid = pObject->GetExID();
		guid.tostring(szGuid);
		guidExsit = pGoods->GetExID();
		guidExsit.tostring(szExGuid);

		sprintf(str, "��������:[%s](%s)��%dλ������[%s](%s)��",pObject->GetName(),szGuid,
			dwPosition,	pGoods->GetName(), szExGuid );

		PutStringToFile("debug-DB", str);
		return FALSE;
	}

	BOOL bResult = FALSE;
	if( pObject )
	{
		bResult = TRUE;
	}
	return bResult;
}
//##ModelId=41BF92C1002E
VOID CGoodsContainer::Clear(LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CContainer::Clear( pVoid );
}

//##ModelId=41BF92C700EA
VOID CGoodsContainer::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_lOwnerType	= 0;
	m_lOwnerID		= 0;
	CContainer::Release();
}

//##ModelId=41BF956500FA
VOID CGoodsContainer::AI()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	//return CContainer::AI();
}

//##ModelId=41BF992A001F
BOOL CGoodsContainer::IsFull()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return TRUE;
}

//##ModelId=41BF928C00FA
CBaseObject* CGoodsContainer::Find(const CGUID& guid)
{
	// TODO: Add your specialized code here.
	return NULL;
}

//##ModelId=41BF92B5030D
CBaseObject* CGoodsContainer::Remove(const CGUID& guid, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	return NULL;
}

//##ModelId=41C0E6B9032C
CBaseObject* CGoodsContainer::Remove(DWORD dwPosition, DWORD dwAmount, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CGoods* pResult	= NULL;
	CGoods* pGoods	= GetGoods( dwPosition );
	if( pGoods && dwAmount > 0 )
	{
		//##���û��ȫ���ó�ȥ����ֱ�Ӽ���ԭ�еļ���,Ȼ��������һ����Ʒ.
		//##����������������������Ʒ.
		if( pGoods -> GetAmount() > dwAmount )
		{
			if( pGoods -> GetMaxStackNumber() >  1 )
			{
#ifdef __MEMORY_LEAK_CHECK__
				CBaseObject::SetConstructFlag(52);
#endif
				pResult = CGoodsFactory::CreateGoods( pGoods -> GetBasePropertiesIndex(),21 );
				pResult -> SetAmount( dwAmount );

				//##֪ͨ������
				for( size_t i = 0; i < m_vListeners.size(); i ++ )
				{
					if( m_vListeners[i] )
					{
						//##����Ϣ��������Ŀ����Ʒ�����Ƴ�֮ǰ֪ͨ��������������Ӧ���ȼ��������Ʒ�Ƿ���ڡ�
						m_vListeners[i] -> OnObjectRemoved( this , pResult, pVoid );
					}
				}

				pGoods -> SetAmount( pGoods -> GetAmount() - dwAmount );
			}
		}
		else if( pGoods -> GetAmount() == dwAmount )
		{
			pResult = dynamic_cast<CGoods*>( Remove(pGoods, pVoid) );
		}
	}
	return pResult;
}

//##ModelId=41BF9944009C
VOID CGoodsContainer::SetOwner(LONG lType, LONG lID)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_lOwnerType	= lType;
	m_lOwnerID		= lID;
}

CBaseObject* CGoodsContainer::Remove( CBaseObject* pObject, LPVOID pVoid )
{
	return CContainer::Remove( pObject, pVoid );
}

CBaseObject* CGoodsContainer::Remove( LONG oType, const CGUID& guid, LPVOID pVoid )
{
	return CContainer::Remove( oType, guid, pVoid );
}

CBaseObject* CGoodsContainer::Find( CBaseObject* pObj )
{
	return CContainer::Find( pObj );
}

CBaseObject* CGoodsContainer::Find( LONG oType,const CGUID& guid )
{
	return CContainer::Find( oType, guid );
}

