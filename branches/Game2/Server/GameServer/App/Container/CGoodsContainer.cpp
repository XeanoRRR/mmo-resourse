// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "StdAfx.h"
#include "../Goods/CGoods.h"
#include "../Goods/CGoodsFactory.h"
#include "../Listener/CContainerListener.h"
#include "CGoodsContainer.h"
#include "../MessagePackage/CS2CContainerObjectMove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//##ModelId=41BF91DC003E
CGoodsContainer::CGoodsContainer()
{
	// TODO: Add your specialized code here.
	m_lOwnerType					= 0;
	m_OwnerGuid						= NULL_GUID;
	m_gcmCurrentGoodsContainerMode	= GCM_NORMAL;
}

//##ModelId=41BF91EE001F
//CGoodsContainer::CGoodsContainer(LONG lID) : CContainer( lID )
//{
//	// TODO: Add your specialized code here.
//	m_lOwnerType	= 0;
//	m_OwnerGuid		= 0;
//}

//##ModelId=41BF91E001E4
CGoodsContainer::~CGoodsContainer()
{
	// TODO: Add your specialized code here.
	m_lOwnerType					= 0;
	m_OwnerGuid						= NULL_GUID;
	m_gcmCurrentGoodsContainerMode	= GCM_NORMAL;
}

LONG CGoodsContainer::GetOwnerType()
{
	return m_lOwnerType;
}

const CGUID& CGoodsContainer::GetOwnerID()
{
	return m_OwnerGuid;
}

//##ModelId=41BF92970167
bool CGoodsContainer::Add(CBaseObject* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return CContainer::Add( pObject, pPreviousContainer, pVoid );
}

//##ModelId=41BF96FB01C5
bool CGoodsContainer::Add(DWORD dwPosition, CGoods* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	//##��Ϊͬλ�����,�������Add����ֻʵ������Ʒ���ڸ�λ�õ�ʱ����2��
	//##��Ʒ������ͬ��״���£������Ʒ�����⣬������ĺ������������ж��Ƿ�
	//##��Ҫ�ص���Ȼ����øú�����
	bool bResult = false;
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
				long nRemainedSlot = pGoods -> GetMaxStackNumber() -
					pGoods -> GetAmount();
				long lAddedAmount = pObject -> GetAmount();
				//##���������������ڵ����������ֱ�Ӽ��롣
				if(lAddedAmount>=0 && lAddedAmount <= nRemainedSlot )
				{
					//##���������ģʽ����ɾ������Ʒ������ǲ���ģʽ���Ͳ�ɾ��Դ��Ʒ
					if( GetContainerMode() == GCM_NORMAL )
					{						
						pGoods -> SetAmount( pGoods -> GetAmount() + lAddedAmount );
						//CGoodsFactory::AddGoodsToGCSet(pObject);
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pObject->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:��Ʒ����][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pObject->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pObject->GetBasePropertiesIndex()),srcGoodsGUID,pObject->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif	
						CGoodsFactory::GarbageCollect( &pObject,1 );
					}

					for( size_t i = 0; i < m_vListeners.size(); i ++ )
					{
						if( m_vListeners[i] )
						{
							//##�úϲ���Ϣ��������Ŀ����Ʒ
							m_vListeners[i] -> OnObjectAdded( this, pGoods, lAddedAmount, pVoid );
						}
					}

					bResult = true;
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

//##ModelId=41BF92C1002E
void CGoodsContainer::Clear(LPVOID pVoid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CContainer::Clear( pVoid );
}

//##ModelId=41BF92C700EA
void CGoodsContainer::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_lOwnerType					= 0;
	m_OwnerGuid						= NULL_GUID;
	m_gcmCurrentGoodsContainerMode	= GCM_NORMAL;
	CContainer::Release();
}

//##ModelId=41BF956500FA
void CGoodsContainer::AI()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	//return CContainer::AI();
}

//##ModelId=41BF992A001F
bool CGoodsContainer::IsFull()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return true;
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
				//##֪ͨ������
				for( size_t i = 0; i < m_vListeners.size(); i ++ )
				{
					if( m_vListeners[i] )
					{
						//##����Ϣ��������Ŀ����Ʒ�����Ƴ�֮ǰ֪ͨ��������������Ӧ���ȼ��������Ʒ�Ƿ���ڡ�
						m_vListeners[i] -> OnObjectRemoved( this, pGoods, dwAmount, pVoid );
					}
				}
				pResult = CGoodsFactory::CreateGoods( pGoods -> GetBasePropertiesIndex() ,14);
				pResult -> SetAmount( dwAmount );
				pGoods  -> SetAmount( pGoods -> GetAmount() - dwAmount );
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
void CGoodsContainer::SetOwner(LONG lType, const CGUID& OwnerGuid)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_lOwnerType	= lType;
	m_OwnerGuid		= OwnerGuid;
}


//##ModelId=41FEF83B00B0
void CGoodsContainer::SetContainerMode(GOODS_CONTAINER_MODE gcm)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_gcmCurrentGoodsContainerMode = gcm;
}

//##ModelId=41FEF8A30033
CGoodsContainer::GOODS_CONTAINER_MODE CGoodsContainer::GetContainerMode()
{
	return m_gcmCurrentGoodsContainerMode;
}

bool CGoodsContainer::CanPutIn(CGoods* pGoods,CPlayer* pPlayer)
{
	if(pGoods)
		return true;
	return false;
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
