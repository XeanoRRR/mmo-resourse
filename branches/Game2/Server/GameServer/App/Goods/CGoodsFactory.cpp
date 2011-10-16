// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "StdAfx.h"
#include "CGoods.h"
#include "CGoodsFactory.h"
#include "../GoodsExManage.h"
#include "../Player.h"
#include "../ServerRegion.h"
#include "../LuaScript/Script.h"
#include "../LuaScript/ScriptSys.h"
#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GlobalSetup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


map<string,string> CGoodsFactory::s_mapSucceedOrder;
map<string,CGUID> CGoodsFactory::s_mapPendingOrder;
list<string> CGoodsFactory::s_listSuccedOrder;

set<CGoods*> CGoodsFactory::s_mGcGoodsSet;
vector<CGoods*> CGoodsFactory::s_vNoDelGoods;

map<long,set<CGoods*> >CGoodsFactory::s_UsingGoods;

BOOL CGoodsFactory::msNoDelFlag=FALSE;


CGoodsFactory::CGoodsFactory()
{
	// TODO: Add your specialized code here.
}

CGoodsFactory::~CGoodsFactory()
{
	// TODO: Add your specialized code here.
}

void CGoodsFactory::AddGoodsToGCSet(CGoods* pGoods)
{
	if(pGoods)
	{
		s_mGcGoodsSet.insert(pGoods);		
	}
}

void CGoodsFactory::ReleaseGCGoodsSet()
{
	set<CGoods*>::iterator goodsIter=s_mGcGoodsSet.begin();
	for(;goodsIter!=s_mGcGoodsSet.end();goodsIter++)
	{
		if(*goodsIter)
		{
#ifdef _GOODSLOG1_
			char pszGoodsLog[1024]="";			
			char srcGoodsGUID[50]="";
			(*goodsIter)->GetExID().tostring(srcGoodsGUID);					
			_snprintf(pszGoodsLog,1024,"[��Ʒ����:CGoodsFactory::ReleaseGCGoodsSet][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
				(*goodsIter)->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName((*goodsIter)->GetBasePropertiesIndex()),srcGoodsGUID,(*goodsIter)->GetAmount());
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
			CGoodsFactory::GarbageCollect(&(*goodsIter),43);
		}
	}
	s_mGcGoodsSet.clear();
}

void CGoodsFactory::ReleaseNoDelGoods()
{	
	for(size_t i=0;i<s_vNoDelGoods.size();i++)
	{
		OBJ_RELEASE(CGoods, s_vNoDelGoods[i]);
	}
	s_vNoDelGoods.clear();
}

bool CGoodsFactory::IsOrderExist(const char* szOrder)
{
	map<string,string>::iterator orderIt=s_mapSucceedOrder.find(szOrder);
	if(orderIt!=s_mapSucceedOrder.end())
		return true;
	return false;
}

void CGoodsFactory::PushOrder(const char* szOrder,const char* szContent)
{
	map<string,string>::iterator orderIt=s_mapSucceedOrder.find(szOrder);
	if(orderIt==s_mapSucceedOrder.end())
	{
		s_mapSucceedOrder[szOrder]=szContent;
		if(s_listSuccedOrder.size()>=1000)
		{
			list<string>::iterator it=s_listSuccedOrder.begin();
			s_listSuccedOrder.erase(it);
			s_mapSucceedOrder.erase(*it);
			s_listSuccedOrder.push_back(szOrder);
		}		
	}
}

void CGoodsFactory::PushPendingOrder(const char* szOrder,const CGUID& guid)
{
	map<string,CGUID>::iterator orderIt=s_mapPendingOrder.find(szOrder);
	if(orderIt==s_mapPendingOrder.end())
	{
		s_mapPendingOrder[szOrder]=guid;
	}
}

void CGoodsFactory::ErasePendingOrder(const char* szOrder)
{
	s_mapPendingOrder.erase(szOrder);
}

CGUID& CGoodsFactory::GetGUIDByOrderId(const char* szOrder)
{
	map<string,CGUID>::iterator orderIt=s_mapPendingOrder.find(szOrder);
	if(orderIt!=s_mapPendingOrder.end())
		return orderIt->second;
	else
		return CGUID::GUID_INVALID;
}


//���´�����Ʒ�ĸ������ԣ�������Ʒ�ĵȼ�����
void CGoodsFactory::ReCreateAddonProperties(CGoods* pGoods)
{
	if(pGoods == NULL)	return;
	//�õ���ǰ�ȼ�
	LONG lCurrentLevel =1;// pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_WEAPON_LEVEL, 1 );
	//������е�����
	pGoods -> m_vAddonProperties.clear();

	CreateAddonProperties(pGoods);
	//��ԭ��Ʒ�ĵȼ�
	if(lCurrentLevel > 0)
		UpgradeEquipment(pGoods,lCurrentLevel);
}

//��Ʒ��װ���Ե�����
void CGoodsFactory::CreateSuitProperties(CGoods* pGoods)
{
	if(pGoods == NULL)	return;
	DWORD dwID = pGoods->GetBasePropertiesIndex();
	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwID );

	vector<CGoodsBaseProperties::tagSuitProperty>* pSuitProVec=pProperties->GetSuitProperties();
	//##�ж���Щ���������Ƿ���Ч(���ϳ��ֵĸ���).			
	for( size_t i = 0; i < pSuitProVec->size(); i ++ )
	{	
		//##׼���ø����ԵĽṹ.
		CGoodsBaseProperties::tagSuitProperty suitProperties;
		suitProperties.bActived=FALSE;
		suitProperties.gapType				=(*pSuitProVec)[i].gapType;		

		//##��ȡ�����Ե�ֵ.

		//##��������Ե�ֵ
		for( size_t j = 0; j < ((*pSuitProVec)[i]).vValues.size(); j ++ )
		{
			//##׼����CGoods��ֵ�Ľṹ
			CGoodsBaseProperties::tagAddonPropertyValue apvValue;

			apvValue.dwId			= ((*pSuitProVec)[i]).vValues[j].dwId;
			apvValue.lBaseValue		= ((*pSuitProVec)[i]).vValues[j].lBaseValue;
			if(suitProperties.gapType==GAP_GOODS_MAX_DURABILITY)
			{
				if(j==1)
				{
					apvValue.lBaseValue=((*pSuitProVec)[i]).vValues[0].lBaseValue;
				}
			}					
			//##��������ֵ��ӵ��ø���������.
			suitProperties.vValues.push_back( apvValue );
		}
		//##���ø���������ӵ���Ʒ�ṹ��.
		pGoods ->m_vSuitProperties.push_back( suitProperties );
	}	
}


//��Ʒ�������Ե����ɼ���
void CGoodsFactory::CreateAddonProperties(CGoods* pGoods)
{
	if(pGoods == NULL)	return;
	DWORD dwID = pGoods->GetBasePropertiesIndex();
	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwID );
	if( pProperties )
	{
		//##��ȡ������Ч�ĸ�������
		vector<eGoodsAddonProperties> vOut;
		pProperties -> GetValidAddonProperties( vOut );
		//##�ж���Щ���������Ƿ���Ч(���ϳ��ֵĸ���).			
		for( size_t i = 0; i < vOut.size(); i ++ )
		{	

			//##׼���ø����ԵĽṹ.
			CGoods::tagAddonProperty apGoodsAddonProperty;
			apGoodsAddonProperty.gapType				= vOut[i];	
			apGoodsAddonProperty.lValues[0]=0;
			apGoodsAddonProperty.lValues[1]=0;

			//##��ȡ�����Ե�ֵ.
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vValues;
			pProperties -> GetAddonPropertyValues( vOut[i], vValues );
			if(vValues.size()==2)
			{
				apGoodsAddonProperty.lValues[0]=vValues[0].lBaseValue;
				apGoodsAddonProperty.lValues[1]=vValues[1].lBaseValue;
			}
			//##���ø���������ӵ���Ʒ�ṹ��.
			pGoods -> m_vAddonProperties.push_back( apGoodsAddonProperty );
		}	
	}
}

//������Ʒ��Ŵ�����Ʒ
CGoods* CGoodsFactory::CreateGoods( DWORD dwID ,long lType)
{
	// ���ݱ��ȡ����Ʒ�Ļ�������
	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwID );
	CGoods* pGoods = NULL;
	if( pProperties )
	{
		//##��������Ʒ,�������úó�ʼ������.
		pGoods = OBJ_CREATE(CGoods);
		pGoods->SetBaseProperties(pProperties);
		pGoods -> SetBasePropertiesIndex( dwID );
		pGoods -> SetName( pProperties -> GetName() );
		pGoods -> SetGoodsName( pProperties -> GetName() );
		
		//pGoods -> SetPrice( pProperties -> GetPrice() );
		//pGoods->SetSilverPrice(pProperties->GetSilverPrice());
		pGoods -> SetGraphicsID( pProperties -> GetIconID(IT_GROUND) );
		pGoods -> SetAmount( 1 );
		//pGoods->SetGoodsBaseType(pProperties->GetGoodsBaseType());

		//������Ʒ�ĸ�������
		CreateAddonProperties(pGoods);
		CreateSuitProperties(pGoods);
		pGoods->InitEnchaseHole();
		//pGoods->EDCalculate();
		//pGoods->AtkEDUpdate();
		//pGoods->DefEDUpdate();
		//pGoods->CalculatePKCP(); // ��ʼPK CPֵ����������ֵ��

		//pGoods->ImmunityUpdate();
		//pGoods->PierceUpdate();
		//pGoods->ConstellationUpdate();
		//pGoods->LuckyUpdate();
		//pGoods->AddonHpUpdate();

		//##������
		CGUID	guid;
		CGUID::CreateGUID(guid);
		pGoods->SetExID(guid);
	}
	s_UsingGoods[lType].insert(pGoods);
	return pGoods;
}



//##ModelId=41A688EC0109
void CGoodsFactory::Release()
{
	s_mapSucceedOrder.clear();
	s_mapPendingOrder.clear();
	s_listSuccedOrder.clear();
}

//����ָ����������Ʒ
void CGoodsFactory::CreateGoods(DWORD dwID, DWORD dwAmount, vector<CGoods*>& vOut,long lType)
{	
	//ͨ�����ȡ�û�������
	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwID );
	if( pProperties )
	{
		//�ɵ��ӵ�����Ʒ�Ͳ���ʹ�õ���Ʒ
		if( pProperties -> GetGoodsBaseType() == GBT_CONSUMABLE ||
			pProperties -> GetGoodsBaseType() == GBT_NORMAL ||
			pProperties -> GetGoodsBaseType() == GBT_USELESS )
		{
			DWORD dwRemainedGoods	= dwAmount;
			DWORD dwMaxStackNumber	= GoodsSetup::QueryGoodsMaxStackNumber( dwID );
			while( dwRemainedGoods != 0 )
			{
				CGoods* pGoods = CreateGoods( dwID,lType );
				if( pGoods )
				{
					/*
					*�����Ҫ����������������Ʒ��������
					*��ִδ�����Ʒ ����Ʒ��������Ϊ������ֵ
					*/
					if( dwRemainedGoods > dwMaxStackNumber )
					{
						pGoods -> SetAmount( dwMaxStackNumber );
						dwRemainedGoods -= dwMaxStackNumber;
					}
					else 
					{
						pGoods -> SetAmount( dwRemainedGoods );
						dwRemainedGoods = 0;
					}
					vOut.push_back( pGoods );
				}
			}
		}
		else
		{
			for( DWORD i = 0; i < dwAmount; i ++ )
			{
				CGoods* pGoods = CreateGoods( dwID,lType );
				if( pGoods )
				{
					vOut.push_back( pGoods );
				}
			}
		}
	}
}


//##ModelId=41C632940182
CGoods* CGoodsFactory::UnserializeGoods(BYTE* pStream, LONG& lOffset,long lType)
{
	// TODO: Add your specialized code here.
	CGoods* pResult = NULL;
	if( pStream )
	{
		pResult = OBJ_CREATE(CGoods);
		if( pResult == NULL )
		{
			assert( 0 );
			OutputDebugString( "Can't alloc memory for unserialize goods.\n" );
		}
		else 
		{
			pResult -> Unserialize( pStream, lOffset );

			CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( 
				pResult -> GetBasePropertiesIndex() );
			if(! pProperties )
			{
				GarbageCollect( &pResult ,100);
			}
			else
			{
				pResult->SetBaseProperties(pProperties);
			}
		}
	}
	s_UsingGoods[lType].insert(pResult);
	return pResult;
}

//ͨ��ByteArray��ԭ��Ʒ
CGoods* CGoodsFactory::UnserializeGoods(DBReadSet& setReadDB,long lType)
{
	// TODO: Add your specialized code here.
	CGoods* pResult = NULL;	
	pResult = OBJ_CREATE(CGoods);
	if( pResult == NULL )
	{
		assert( 0 );
		OutputDebugString( "Can't alloc memory for unserialize goods.\n" );
	}
	else 
	{
		pResult -> Unserialize( setReadDB );

		CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( 
			pResult -> GetBasePropertiesIndex() );
		if(! pProperties )
		{
			GarbageCollect( &pResult ,100);
		}
        else
        {
            pResult->SetBaseProperties(pProperties);
        }
	}
	s_UsingGoods[lType].insert(pResult);
	return pResult;
}

//������Ʒ�����ڴ�
BOOL CGoodsFactory::GarbageCollect(CGoods** ppGoods,long lFlag, BOOL bCollectMedal)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	if( ppGoods )
	{
		//#ifdef _DEBUG
		//		if( *ppGoods )
		//		{
		//			char szDebug[256];
		//			_snprintf( szDebug, "Goods Address: %d ", reinterpret_cast<int>(ppGoods) );
		//			OutputDebugString( szDebug );
		//			char szGoodsID[40] = {0};
		//			(*ppGoods) -> GetExID().tostring(szGoodsID);
		//			_snprintf( szDebug, "Delete Goods[%s]: id[%s], amount[%d]\n", (*ppGoods) -> GetName(), szGoodsID, (*ppGoods) -> GetAmount() );
		//			OutputDebugString( szDebug );
		//		}
		//#endif
		if(*ppGoods)
		{
			CServerRegion * pRegion = dynamic_cast<CServerRegion*>((*ppGoods)->GetFather());
			if(pRegion && pRegion->FindChildObject((*ppGoods)->GetType(),(*ppGoods)->GetExID()) )
			{
				char pszGUID[50]="";
				(*ppGoods)->GetExID().tostring(pszGUID);
				Log4c::Trace(ROOT_MODULE,FormatText("GS_GOODSFACTORY_0",(*ppGoods)->GetName(),pszGUID));//ɾ����Ʒ(Name:%s,GUID:%s)ʱ,����Ʒ���ڳ�����
				//pRegion->RemoveObject((*ppGoods));
			}
			if((*ppGoods)->GetGoodsBaseType() == GBT_MEDAL && !bCollectMedal ) 
				return TRUE;
		}
		map<long,set<CGoods*> >::iterator it = s_UsingGoods.begin();
		for(;it != s_UsingGoods.end();it++)
		{
			(*it).second.erase(*ppGoods);
		}
		if(msNoDelFlag)
		{
			s_vNoDelGoods.push_back(*ppGoods);
		}
		else
		{
			OBJ_RELEASE(CGoods, *ppGoods );
		}		
		bResult = TRUE;
	}
	return bResult;
}



//������Ʒ����۸�
DWORD CGoodsFactory::CalculateRepairPrice(CGoods* pGoods)
{
	DWORD dwGoodsPrice=pGoods->GetPrice();
	if(dwGoodsPrice>0)
	{
		dwGoodsPrice *= GlobalSetup::GetSetup()->dwNpcSellGoldToSilver;
	}
	else
	{
		dwGoodsPrice=pGoods->GetSilverPrice();
	}
	DWORD dwRepairPrice = 0;
	if( pGoods && pGoods -> CanReparied() )
	{
		//��ȡ��ǰ�;�ֵ
		LONG lCurrentDurability	= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 );
		//��ȡ����;�ֵ
		LONG lMaxDurability		= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 1 );
		FLOAT fFactor			= GlobalSetup::GetSetup() -> fRepairFactor;
		FLOAT fRate				= ( lMaxDurability - lCurrentDurability ) / static_cast<FLOAT>( lMaxDurability );
		if( fRate < 0.0f )
		{
			fRate = 0.0f;
		}
		dwRepairPrice = static_cast<DWORD>( dwGoodsPrice * fRate * fFactor );
	}
	return dwRepairPrice;
}

//������Ʒ��������
DWORD CGoodsFactory::CalculateUpgradePrice(CGoods* pGoods)
{
	DWORD dwResult = 0;
	if( pGoods && pGoods -> CanUpgraded() )
	{
		// ÿ���������ã���Ʒ�����۸����1����38��4��lg��Ʒ�����۸�/15��pow����Ʒ��ǰ�ȼ���
		LONG lCurrentLevel	=1;// pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_WEAPON_LEVEL, 1 );
		LONG lUpgradePrice	=1;// pGoods -> GetAddonPropertyValues( CGoodsBaseProperties::GAP_GOODS_UPGRADE_PRICE, 1 );
		FLOAT fFactor		= 1 + ( 38 - 4 * log10( static_cast<FLOAT>(lUpgradePrice) ) ) / 15;
		fFactor				= pow( fFactor, lCurrentLevel );
		dwResult			= static_cast<DWORD>( lUpgradePrice * fFactor );
	}
	return dwResult;
}

//������Ʒ���ۼ۸�
DWORD CGoodsFactory::CalculateVendPrice(CGoods *pGoods)
{
	DWORD dwVendPrice = 0;
	if( pGoods && pGoods -> GetPrice() > 0 )
	{
		//##���Ȼ�ȡȫ������
		FLOAT fVendPrice = static_cast<FLOAT>( pGoods -> GetPrice() );

		CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( 
			pGoods -> GetBasePropertiesIndex() );
		if( pProperties )
		{
			if( pProperties -> GetGoodsBaseType() == GBT_EQUIPMENT ) 
			{
				//##��ȡ�۾ɱ�ֵ��
				FLOAT fDepreciableRate = GlobalSetup::GetSetup() -> fBasePriceRate;

				//##��ȡ��ǰ/����;�
				DWORD dwCurrentDurability	= pGoods -> GetAddonPropertyValues( 
					GAP_GOODS_MAX_DURABILITY, 2 );
				DWORD dwMaxDurability		= pGoods -> GetAddonPropertyValues( 
					GAP_GOODS_MAX_DURABILITY, 1 );

				if( dwMaxDurability == 0 )
				{
					//##����޷���ȡ������;�,��ô�ͱ�������0����
					//	fVendPrice = pGoods -> GetPrice() * fDepreciableRate;
				}
				else 
				{
					if( dwCurrentDurability > dwMaxDurability )
					{
						dwCurrentDurability = dwMaxDurability;
					}
					//##�����۾ɺ�ļ۸�
					fVendPrice = pGoods -> GetPrice() * fDepreciableRate + 
						pGoods -> GetPrice() * ( 1.0f - fDepreciableRate ) * ( dwCurrentDurability / dwMaxDurability );
				}
			}
		}

		//##��ȡ�ۼ���
		FLOAT fTradeInPriceRate = GlobalSetup::GetSetup() -> fTradeInRate;

		dwVendPrice = static_cast<DWORD>( fVendPrice * fTradeInPriceRate );
	}
	return dwVendPrice;
}


//������Ʒ���ۼ۸�
DWORD CGoodsFactory::CalculateVendSilverPrice(CGoods *pGoods)
{
	if(!pGoods)
		return 0;
	DWORD dwSilver=0;
	if(pGoods->GetPrice()>0)
	{
		dwSilver=pGoods->GetPrice() * GlobalSetup::GetSetup()->dwNpcSellGoldToSilver;
	}
	else
	{
		dwSilver=pGoods->GetSilverPrice();
	}

	DWORD dwVendPrice = 0;
	if(dwSilver>0)
	{
		//##���Ȼ�ȡȫ������
		FLOAT fVendPrice =(float)dwSilver;

		CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( 
			pGoods -> GetBasePropertiesIndex() );
		if( pProperties )
		{
			if( pProperties -> GetGoodsBaseType() == GBT_EQUIPMENT ) 
			{
				//##��ȡ�۾ɱ�ֵ��
				FLOAT fDepreciableRate = GlobalSetup::GetSetup() -> fBasePriceRate;

				//##��ȡ��ǰ/����;�
				DWORD dwCurrentDurability	= pGoods -> GetAddonPropertyValues( 
					GAP_GOODS_MAX_DURABILITY, 2 );
				DWORD dwMaxDurability		= pGoods -> GetAddonPropertyValues( 
					GAP_GOODS_MAX_DURABILITY, 1 );

				if( dwMaxDurability == 0 )
				{
					//##����޷���ȡ������;�,��ô�ͱ�������0����
					//	fVendPrice = pGoods -> GetPrice() * fDepreciableRate;
				}
				else 
				{
					if( dwCurrentDurability > dwMaxDurability )
					{
						dwCurrentDurability = dwMaxDurability;
					}
					//##�����۾ɺ�ļ۸�
					fVendPrice = dwSilver * fDepreciableRate + 
						dwSilver * ( 1.0f - fDepreciableRate ) * ( dwCurrentDurability / dwMaxDurability );
				}
			}
		}

		//##��ȡ�ۼ���
		FLOAT fTradeInPriceRate = GlobalSetup::GetSetup() -> fTradeInRate;

		dwVendPrice = static_cast<DWORD>( fVendPrice * fTradeInPriceRate );
	}
	return dwVendPrice;
	return 0;
}

//��Ʒ������
BOOL CGoodsFactory::RepairEquipment(CGoods* pGoods)
{
	BOOL bResult = FALSE;
	if( pGoods && pGoods -> CanReparied() )
	{
		//������Ʒ�ĵ�ǰ�;�Ϊ����;�ֵ
		LONG lCurrentDurability	= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 );
		LONG lMaxDurability		= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 1 );
		//pGoods -> SetAddonPropertyBaseValues( CGoodsBaseProperties::GAP_GOODS_MAX_DURABILITY, 2, lMaxDurability );
		pGoods->ChangeAttribute( GAP_GOODS_MAX_DURABILITY,lMaxDurability,2,true);
		bResult = TRUE;
	}
	return bResult;
}

//װ������
BOOL CGoodsFactory::EquipmentWaste(CGoods* pGoods, BOOL bIsSpecialWaste)
{
	BOOL bResult = FALSE;
	if( pGoods )
	{
		LONG lWeaponWaste = 0;
		//ĥ������
		if( bIsSpecialWaste )
		{
			lWeaponWaste = GlobalSetup::GetSetup() -> lGoodsDurSpecialFray;
		}
		else
		{
			lWeaponWaste = GlobalSetup::GetSetup() -> lGoodsDurFray;
		}

		LONG lCurrentDurability	= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 );
		LONG lMaxDurability		= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 1 );

		if( lMaxDurability > 0 )
		{
			//������Ʒ�ĵ�ǰ�;�ֵ
			lCurrentDurability -= lWeaponWaste;
            if( lCurrentDurability < 0 )
                lCurrentDurability = 0;
			//pGoods -> SetAddonPropertyBaseValues( CGoodsBaseProperties::GAP_GOODS_MAX_DURABILITY, 2, lCurrentDurability );
			pGoods->ChangeAttribute(GAP_GOODS_MAX_DURABILITY,lCurrentDurability,2,true);
			bResult = TRUE;
		}
	}
	return bResult;
}

//����װ��
BOOL CGoodsFactory::UpgradeEquipment(CGoods* pGoods, LONG lTargetLevel)
{
	return TRUE;
}

//! �ı�װ���ȼ�����ԭ�еȼ������ϸı䣩
BOOL CGoodsFactory::ChangeEquipmentLevel(CGoods* pEquipment, long lChangeDegree)
{
	if(0 != lChangeDegree && pEquipment && pEquipment -> CanUpgraded() )
	{
		long lOldLevel = pEquipment->GetAddonPropertyValues(GAP_WEAPON_LEVEL, 1);
		lOldLevel += lChangeDegree;
		lOldLevel = (0 > lOldLevel) ? 0 : lOldLevel;

		pEquipment->SetAddonPropertyBaseValues(GAP_WEAPON_LEVEL, 1 , lOldLevel);
		UpdateEquipmentAttr(pEquipment);
		LONG lMaxDurability = pEquipment -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 1 );
		pEquipment -> SetAddonPropertyBaseValues( GAP_GOODS_MAX_DURABILITY, 2, lMaxDurability);//��ǰ�;�
		return TRUE;
	}
	return FALSE;
}

//! ����װ���ȼ�
BOOL CGoodsFactory::SetEquipmentLevel(CGoods* pEquipment, LONG lLevel)
{
	if(0 <= lLevel && pEquipment && pEquipment -> CanUpgraded() )
	{
		pEquipment->SetAddonPropertyBaseValues(GAP_WEAPON_LEVEL, 1 , lLevel);
		UpdateEquipmentAttr(pEquipment);
		LONG lMaxDurability = pEquipment -> GetAddonPropertyValues(GAP_GOODS_MAX_DURABILITY, 1 );
		pEquipment -> SetAddonPropertyBaseValues( GAP_GOODS_MAX_DURABILITY, 2, lMaxDurability);//��ǰ�;�
		return TRUE;
	}
	return FALSE;
}

//! �õ�װ���Ƿ��ǹ�����
BOOL CGoodsFactory::IsAttEquipment(CGoods* pEquipment)
{
    if(NULL != pEquipment)
    {
		if (pEquipment->GetEquipType()==EC_E_WEAPON ||
			pEquipment->GetEquipType()==EC_E_NECKLACE)
		{
			return TRUE;
		}
    }

	return FALSE;
}


//! ����װ�����ԣ��������﹥��ħ���������ħ����
void CGoodsFactory::UpdateEquipmentAttr(CGoods* pEquipment)
{

	////! ������ʽ��[������+��ȣ�*��1+ħ����+��Ƭ+��װ]
	//if (NULL == pEquipment) return;

	//pEquipment->DelAddonPropertyNotInBase();

	//

	////! ----------------------------------------��Ʒ����ֵ----------------------------------------
	//CGoodsBaseProperties *pGoodsBaseProperties = GoodsSetup::QueryGoodsBaseProperties(pEquipment->GetBasePropertiesIndex());
	//if (NULL == pGoodsBaseProperties) return;

	//vector<CGoodsBaseProperties::tagAddonPropertyValue> vOutProperty;

	//long lMinPA = 0, lMaxPA = 0, lMA = 0, lPR = 0, lMR = 0, 
	//	lHR = 0, lKR = 0, lMKR = 0, lP = 0, lB = 0, lF = 0, 
	//	lMaxD = 0, lSD = 0, lWDL = 0, lAML = 0; //! ��������

	//float fIncrementMinPA = 0,fIncrementMaxPA = 0,fIncrementMA = 0,fIncrementPR = 0,fIncrementMR = 0, 
	//	fIncrementHR = 0, fIncrementKR = 0, fIncrementMKR = 0, fIncrementP = 0, fIncrementB = 0, fIncrementF = 0,
	//	fIncrementMaxD = 0, fIncrementSD = 0, fIncrementWDL = 0, fIncrementAML = 0;//! ��ǰ���������ԣ��仯����

	//bool bHasMinPA = false, bHasMaxPA = false, bHasMA = false, bHasPR = false, bHasMR = false, 
	//	bHasHR = false, bHasKR = false, bHasMKR = false, bHasP = false, bHasB = false, bHasF = false,
	//	bHasMaxD = false, bHasSD = false, bHasWDL = false, bHasAML = false;//! �з�����

	////! ���ò�������
	//{
	//	//! ��Ҫ���õ������б��ܳ�Ⱥ�ħ��Ӱ������Բ���Ҫ���������ã�
	//	DWORD arrProperties[] = 
	//	{
	//		CGoodsBaseProperties::GAP_CRI_DAMAGE,	//�������˺�
	//		CGoodsBaseProperties::GAP_CRI_DEF,		//����������
	//		CGoodsBaseProperties::GAP_MCRI_DAMAGE,	//ħ�������˺�
	//		CGoodsBaseProperties::GAP_MCRI_DEF,		//ħ����������
	//		CGoodsBaseProperties::GAP_MAXHP_CORRECT,	//������������
	//		CGoodsBaseProperties::GAP_DEDICANT_CORRECT,	//רע����
	//		CGoodsBaseProperties::GAP_CON_DEF,			//�������ʿ���
	//	};
	//	
	//	//! ��ʼ����
	//	for (LONG i = 0; i < sizeof(arrProperties) / sizeof(DWORD); ++i)
	//	{
	//		vOutProperty.clear();
	//		pGoodsBaseProperties->GetAddonPropertyValues((eGoodsAddonProperties)arrProperties[i], vOutProperty);
	//		if(0 != vOutProperty.size())
	//			pEquipment->ChangeAttribute(arrProperties[i], vOutProperty[0].lBaseValue, 1, true);	
	//	}
	//}
	//vOutProperty.clear();
	////! ��С������
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MIN_ATTACK, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lMinPA = vOutProperty[0].lBaseValue;
	//	bHasMinPA = true;
	//}
	////! ��󹥻���
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAX_ATTACK, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lMaxPA = vOutProperty[0].lBaseValue;
	//	bHasMaxPA = true;
	//}
	////! ħ������
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_FUMO_PROPERTY, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lMA = vOutProperty[0].lBaseValue;
	//	bHasMA = true;
	//}
	////! ���������
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_NOR_DEF, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lPR = vOutProperty[0].lBaseValue;
	//	bHasPR = true;
	//}
	////! ħ������
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAGIC_DEF, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lMR = vOutProperty[0].lBaseValue;
	//	bHasMR = true;
	//}
	////! ����
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_HIT_RATE_CORRECT, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lHR = vOutProperty[0].lBaseValue;
	//	bHasHR = true;
	//}
	////! ��������һ��
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_NORATTACK_KILLRATE, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lKR = vOutProperty[0].lBaseValue;
	//	bHasKR = true;
	//}
	////! ħ������һ��
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAGIC_KILLRATE, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lMKR = vOutProperty[0].lBaseValue;
	//	bHasMKR = true;
	//}
	////! �м�����
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_BLOCK_CORRECT, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lP = vOutProperty[0].lBaseValue;
	//	bHasP = true;
	//}
	////! ������
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARRY_CORRECT, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lB = vOutProperty[0].lBaseValue;
	//	bHasB = true;
	//}
	////! ��������
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_FLEE_CORRECT, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lF = vOutProperty[0].lBaseValue;
	//	bHasF = true;
	//}
	////! ��������;�ֵ�ɳ�ֵ
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_GOODS_MAX_DURABILITY, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lMaxD = vOutProperty[0].lBaseValue;
	//	bHasMaxD = true;
	//}
	////! ������������ɳ�
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_SHIELD_DEF, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lSD = vOutProperty[0].lBaseValue;
	//	bHasSD = true;
	//}
	////! ����ǿ�ȳɳ�
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_WEAPON_DAMAGE_LEVEL, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lWDL = vOutProperty[0].lBaseValue;
	//	bHasWDL = true;
	//}
	////! ���׷����������
	//vOutProperty.clear();
	//pGoodsBaseProperties->GetAddonPropertyValues(CGoodsBaseProperties::GAP_ADD_MAXHP_LEVEL, vOutProperty);
	//if(0 != vOutProperty.size())
	//{
	//	lAML = vOutProperty[0].lBaseValue;
	//	bHasAML = true;
	//}

	////! ----------------------------------------���----------------------------------------
	//long lLevel = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_WEAPON_LEVEL, 1);
	////! ��С��������
	//if(bHasMinPA)
	//{
	//	fIncrementMinPA = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MIN_ATK_UP, 1) * 0.001f;
	//}
	////! ��󹥻�����
	//if(bHasMaxPA)
	//{
	//	fIncrementMaxPA = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAX_ATK_UP, 1) * 0.001f;
	//}
	////! ��ͨ��������
	//if(bHasPR)
	//{
	//	fIncrementPR = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_DEF_UP, 1) * 0.001f;
	//}
	////! ħ����������
	//if(bHasMA)
	//{
	//	fIncrementMA = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAGIC_UP, 1) * 0.001f;
	//}
	////! ħ����������
	//if(bHasMR)
	//{
	//	fIncrementMR = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAGICDEF_UP, 1) * 0.001f;
	//}
	////! ���������ɳ�
	//if(bHasHR)
	//{
	//	fIncrementHR = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_HIT_RATE_UP, 1) * 0.001f;
	//}
	////! ��������һ���ʳɳ�
	//if(bHasKR)
	//{
	//	fIncrementKR = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_KILL_RATE_UP, 1) * 0.001f;
	//}
	////! ħ������һ���ʳɳ�
	//if(bHasMKR)
	//{
	//	fIncrementMKR = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAGIC_KILL_RATE_UP, 1) * 0.001f;
	//}
	////! �м����������ɳ�
	//if(bHasP)
	//{
	//	fIncrementP = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARRY_UP, 1) * 0.001f;
	//}
	////! �����������ɳ�
	//if(bHasB)
	//{
	//	fIncrementB = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_BLOCK_UP, 1) * 0.001f;
	//}
	////! �������������ɳ�
	//if(bHasF)
	//{
	//	fIncrementF = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_FLEE_UP, 1) * 0.001f;
	//}
	////! ��������;�ֵ�ɳ�ֵ����
	//if(bHasMaxD)
	//{
	//	fIncrementMaxD = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAXDUR_UP, 1) * 0.001f;
	//}
	////! ������������ɳ�����								
	//if(bHasSD)
	//{
	//	fIncrementSD = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_SHIELD_DEF_UP, 1) * 0.001f;
	//}
	////! ����ǿ�ȳɳ�����							
	//if(bHasWDL)
	//{
	//	fIncrementWDL = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_WEAPON_INTEN_UP, 1) * 0.001f;
	//}
	////! ���׷���������޳ɳ�����							
	//if(bHasAML)
	//{
	//	fIncrementAML = lLevel * pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_ADD_MAXHP_UP, 1) * 0.001f;
	//}
	////! ----------------------------------------ED----------------------------------------
	////����DEF ED / ATK ED
	//if(pEquipment->HasAddonProperty(CGoodsBaseProperties::GAP_ATK_ED) && 
	//	pEquipment->HasAddonProperty(CGoodsBaseProperties::GAP_CUR_ATK_ED))
	//{
	//	long lMaxAtkEd=pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_ATK_ED,2);
	//	long lCurAtkEd=pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_CUR_ATK_ED,1);
	//	if(lCurAtkEd>lMaxAtkEd)
	//	{
	//		lCurAtkEd=lMaxAtkEd;
	//		pEquipment->SetAddonPropertyBaseValues(CGoodsBaseProperties::GAP_CUR_ATK_ED,1,lCurAtkEd);
	//	}
	//}

	//if(pEquipment->HasAddonProperty(CGoodsBaseProperties::GAP_DEF_ED) && 
	//	pEquipment->HasAddonProperty(CGoodsBaseProperties::GAP_CUR_DEF_ED))
	//{
	//	long lMaxDefEd=pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_DEF_ED,2);
	//	long lCurDefEd=pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_CUR_DEF_ED,1);
	//	if(lCurDefEd>lMaxDefEd)
	//	{
	//		lCurDefEd=lMaxDefEd;
	//		pEquipment->SetAddonPropertyBaseValues(CGoodsBaseProperties::GAP_CUR_DEF_ED,1,lCurDefEd);
	//	}
	//}
	////! ����
	//long lCurAtkED = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_CUR_ATK_ED,1);
	//if(lCurAtkED>=0)
	//{
	//	//! ��С��������
	//	if(bHasMinPA)
	//	{
	//		fIncrementMinPA += (fIncrementMinPA + lMinPA) * lCurAtkED * 0.01f;
	//	}
	//	//! ��󹥻�����
	//	if(bHasMaxPA)
	//	{
	//		fIncrementMaxPA += (fIncrementMaxPA + lMaxPA) * lCurAtkED * 0.01f;
	//	}
	//	//! ħ����������
	//	if(bHasMA)
	//	{
	//		fIncrementMA += (fIncrementMA + lMA) * lCurAtkED  * 0.01f;
	//	}
	//}
	////! ����
	//long lCurDefED = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_CUR_DEF_ED,1);
	//if(lCurDefED>=0)
	//{
	//	//! ��ͨ��������
	//	if(bHasPR)
	//	{
	//		fIncrementPR += (fIncrementPR + lPR) * lCurDefED * 0.01f;
	//	}
	//	//! ħ����������
	//	if(bHasMR)
	//	{
	//		fIncrementMR += (fIncrementMR + lMR) * lCurDefED  * 0.01f;
	//	}
	//}

	////! ----------------------------------------����ֵ----------------------------------------
	////! ��С��������
	//if(bHasMinPA)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MIN_ATTACK, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_MIN_ATTACK, DWORD(0.5f + fIncrementMinPA + lMinPA), 1, true);
	//}

	////! ��󹥻�����
	//if(bHasMaxPA)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAX_ATTACK, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_MAX_ATTACK, DWORD(0.5f + fIncrementMaxPA + lMaxPA), 1, true);
	//}

	////! ��ͨ��������
	//if(bHasPR)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_NOR_DEF, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_NOR_DEF, DWORD(0.5f + fIncrementPR + lPR), 1, true);
	//}
	////! ħ����������
	//if(bHasMA)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_FUMO_PROPERTY, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_FUMO_PROPERTY, DWORD(0.5f + fIncrementMA + lMA), 1, true);
	//}
	////! ħ����������
	//if(bHasMR)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAGIC_DEF, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_MAGIC_DEF, DWORD(0.5f + fIncrementMR + lMR), 1, true);
	//}
	////! ���������ɳ�
	//if(bHasHR)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_HIT_RATE_CORRECT, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_HIT_RATE_CORRECT, DWORD(0.5f + fIncrementHR + lHR), 1, true);
	//}
	////! ��������һ���ʳɳ�
	//if(bHasKR)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_NORATTACK_KILLRATE, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_NORATTACK_KILLRATE, DWORD(0.5f + fIncrementKR + lKR), 1, true);
	//}
	////! ħ������һ���ʳɳ�
	//if(bHasMKR)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_MAGIC_KILLRATE, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_MAGIC_KILLRATE, DWORD(0.5f + fIncrementMKR + lMKR), 1, true);
	//}
	////! �м����������ɳ�
	//if(bHasP)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_BLOCK_CORRECT, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_BLOCK_CORRECT, DWORD(0.5f + fIncrementP + lP), 1, true);
	//}
	////! �����������ɳ�
	//if(bHasB)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARRY_CORRECT, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_PARRY_CORRECT, DWORD(0.5f + fIncrementB + lB), 1, true);
	//}
	////! �������������ɳ�
	//if(bHasF)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_FLEE_CORRECT, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_FLEE_CORRECT, DWORD(0.5f + fIncrementF + lF), 1, true);
	//}
	////! ��������;�ֵ�ɳ�ֵ����
	//if(bHasMaxD)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_GOODS_MAX_DURABILITY, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_GOODS_MAX_DURABILITY, DWORD(0.5f + fIncrementMaxD + lMaxD), 1, true);
	//}
	////! ������������ɳ�����								
	//if(bHasSD)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_SHIELD_DEF, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_SHIELD_DEF, DWORD(0.5f + fIncrementSD + lSD), 1, true);
	//}
	////! ����ǿ�ȳɳ�����							
	//if(bHasWDL)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_WEAPON_DAMAGE_LEVEL, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_WEAPON_DAMAGE_LEVEL, DWORD(0.5f + fIncrementWDL + lWDL), 1, true);
	//}
	////! ���׷��������������							
	//if(bHasAML)
	//{
	//	//long fIncrementValue = pEquipment->GetAddonPropertyValues(CGoodsBaseProperties::GAP_WEAPON_DAMAGE_LEVEL, 1);
	//	pEquipment->ChangeAttribute(CGoodsBaseProperties::GAP_ADD_MAXHP_LEVEL, DWORD(0.5f + fIncrementAML + lAML), 1, true);
	//}
	//pEquipment->UpdateEnchaseAttr();


	
}

eEquipLimitType CGoodsFactory::QueryPlayerEquipLimitType(CPlayer* pPlayer)
{
	BYTE btOccu=pPlayer->GetOccupation();
	if(btOccu==OCC_Mystic || btOccu==OCC_Warlock || btOccu==OCC_Cleric || btOccu==OCC_Sorcerer ||
		btOccu==OCC_Necromancer || btOccu==OCC_Bishop || btOccu==OCC_Prophet)
	{
		return ELT_LIGHT;
	}
	else if(btOccu==OCC_Rogue || btOccu==OCC_Scout || btOccu==OCC_Craftsman || btOccu==OCC_Ranger ||
		btOccu==OCC_Assassin || btOccu==OCC_BladeDancer || btOccu==OCC_Bard)
	{
		return ELT_MID;
	}
	else if(btOccu==OCC_Fighter || btOccu==OCC_Knight || btOccu==OCC_Warrior || btOccu==OCC_Paladin ||
		btOccu==OCC_DarkKnight || btOccu==OCC_Berserker || btOccu==OCC_Gladiator)
	{
		return ELT_HEAVY;
	}
	return ELT_UNKNOWN;
}

eBusinessGoodsType CGoodsFactory::GetBusinessGoodsType(DWORD dwGoodsId)
{
	CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(dwGoodsId);
	if(!pProperty)
		return BGT_NOT_BUSINESS_GOODS;
	DWORD dwVal=pProperty->GetAddonPropertyValue(GAP_TRADEGOODS_TYPE,1);
	if(dwVal>=BGT_FOOD && dwVal<BGT_MAX_TYPE)
	{
		eBusinessGoodsType eVal=static_cast<eBusinessGoodsType>(dwVal);
		return eVal;
	}	
	return BGT_NOT_BUSINESS_GOODS;
}

//! ����ͬһ�����͵�������Ʒ��������һ������Ʒ����Ϊkey��map
void  CGoodsFactory::CreateGoodsListByType(eGoodsBaseType eType, map<LONG, CGoods*> &mapGoods)
{
	mapGoods.clear();

	map<DWORD,CGoodsBaseProperties*>* pGoodsBasePropertiesList = GoodsSetup::GetGoodsBasePropertiesList();
	map<DWORD,CGoodsBaseProperties*>::iterator ite = pGoodsBasePropertiesList->begin();
	for (; ite != pGoodsBasePropertiesList->end(); ++ite)
	{
		if(eType == ite->second->GetGoodsBaseType())
		{
			CGoods *pGoods = CreateGoods(ite->first,10000);
			if(NULL != pGoods)
			{
				mapGoods[ite->first] = pGoods;
			}
		}
	}

}

BOOL CGoodsFactory::Upgrade(CGoods* pGoods, eGoodsAddonProperties gapSource,
							eGoodsAddonProperties gapDest, BOOL bUpgrade)
{
	return TRUE;
}


void CGoodsFactory::RunScriptOnGet(DWORD dwGoodsId,CPlayer* pPlayer)
{
	if(pPlayer==NULL)
		return;
	CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(dwGoodsId);
	if(!pProperty)
		return;

	DWORD dwVal=pProperty->GetAddonPropertyValue(GAP_GET_TRIGGER,1);
	if(dwVal!=0)
	{
		char strFile[64];
		_snprintf(strFile, 64, "scripts/Goods/%d.lua",dwVal);
		Script::RunEnv env( pPlayer, NULL, static_cast<CRegion*>( pPlayer->GetFather() ) );
		GetInst( ScriptSys ).RunScript( env, strFile );
	}
}

////��ȡ��Ʒ��������
//DWORD CGoodsFactory::QueryGoodsMaxStackNumber( DWORD dwGoodsBasePropertiesID )
//{
//	DWORD dwResult = 1;
//	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwGoodsBasePropertiesID );
//
//	if(dwGoodsBasePropertiesID==GoodsSetup::GetGoldCoinIndex())
//	{			
//		return GlobalSetup::GetSetup()->dwGoldCoinLimit;
//	}
//	else if(dwGoodsBasePropertiesID==GoodsSetup::GetSilverIndex())
//	{
//		return GlobalSetup::GetSetup()->dwSilverLimit;
//	}
//	if( pProperties && (pProperties -> GetGoodsType() == GT_CONSUMABLE ||
//		pProperties -> GetGoodsType() == GT_USELESS))
//	{
//		vector<CGoodsBaseProperties::tagAddonPropertyValue> vOut;
//		pProperties -> GetAddonPropertyValues( GAP_MAXOVERLAP, vOut );
//		for( size_t i = 0; i < vOut.size(); i ++ )
//		{
//			if( vOut[i].dwId == 1 )
//			{
//				dwResult = static_cast<DWORD>( vOut[i].lBaseValue );
//				break;
//			}
//		}
//		vOut.clear();
//	}
//	return dwResult;
//}

map<CGUID, vector<BYTE>>	CGoodsFactory::m_mapChatGoodsInfo;
list<pair<CGUID, LONG>>		CGoodsFactory::m_listSaveTimeOrder;

//! ���һ����Ʒ��Ϣ
void CGoodsFactory::AddGoodsInfo(const CGUID &Key, const vector<BYTE> &vGoodsInfo)
{
	if(m_mapChatGoodsInfo.end() != m_mapChatGoodsInfo.find(Key)) return;

	if (GlobalSetup::GetSetup()->lMaxGoodInfoNum <= (long)m_mapChatGoodsInfo.size())
	{
		ClearInfo(GlobalSetup::GetSetup()->lMaxGoodInfoNum / 10);
	}

	m_mapChatGoodsInfo[Key] = vGoodsInfo;
	m_listSaveTimeOrder.push_back(pair<CGUID, LONG>(Key, (long)time(NULL)));
}

//! ȡ��һ����Ʒ��Ϣ
BOOL CGoodsFactory::GetGoodsInfo(const CGUID &Key, vector<BYTE> &vGoodsInfo)
{
	map<CGUID, vector<BYTE>>::iterator ite = m_mapChatGoodsInfo.find(Key);
	if (m_mapChatGoodsInfo.end() != ite)
	{
		if(0 >= ite->second.size())
			return FALSE;
		vGoodsInfo.resize(ite->second.size());
		memmove(&vGoodsInfo[0], &(ite->second[0]), ite->second.size());
		return TRUE;
	}
	return FALSE;
}


//! ����
void CGoodsFactory::ClearInfo(LONG lClearNum)
{
	if (0 >= lClearNum)
		lClearNum = 1;

	list<pair<CGUID, LONG>>::iterator ite = m_listSaveTimeOrder.begin();
	LONG i = 0;
	for ( ; ite != m_listSaveTimeOrder.end(); ++ite)
	{
		if(i < lClearNum)
			break;
		++i;
	}

	m_listSaveTimeOrder.erase(m_listSaveTimeOrder.begin(), ite);
}


long CGoodsFactory::OutputUseGoodsInfo(const char *pszFileName)
{
	Log4c::Trace(ROOT_MODULE,"UseGoodsInfo:");
	map<long,set<CGoods*> >::iterator it = s_UsingGoods.begin();
	for(;it != s_UsingGoods.end();it++)
	{
		if((*it).second.size() > 0)
		{
			Log4c::Trace(ROOT_MODULE, "Flag:%d,Count:%d",(*it).first,(*it).second.size() );
		}
	}
	return 0;
}