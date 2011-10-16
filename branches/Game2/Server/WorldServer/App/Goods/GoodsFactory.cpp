#include "StdAfx.h"
#include "Goods.h"
#include "GoodsFactory.h"

#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

std::map<long,std::set<CGoods*> >CGoodsFactory::s_UsingGoods;

CGoodsFactory::CGoodsFactory()
{
	
}

CGoodsFactory::~CGoodsFactory()
{
	
}

void CGoodsFactory::AddCreateGoodsLog(CGoods* pGoods,long lType)
{
	if(pGoods)
	{
		s_UsingGoods[lType].insert(pGoods);
	}
}

// ������Ʒ
CGoods* CGoodsFactory::CreateGoods(ulong dwID, long lType)
{
	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties(dwID);
	CGoods* pGoods = NULL;
	if(pProperties)
	{
		// ��������Ʒ,�������úó�ʼ������.
		pGoods = new CGoods;
		pGoods->SetBaseProperties(pProperties);
		pGoods->SetBasePropertiesIndex(pProperties->GetIndex());
		pGoods->SetName(pProperties->GetName());
		pGoods->SetGraphicsID(pProperties->GetIconID(IT_GROUND));
		pGoods->SetAmount(1);

		//##��ȡ������Ч�ĸ�������
		vector<eGoodsAddonProperties> vOut;
		pProperties->GetValidAddonProperties( vOut );

		//##�ж���Щ���������Ƿ���Ч(���ϳ��ֵĸ���).
		for( size_t i = 0; i < vOut.size(); i ++ )
		{			
			//##׼���ø����ԵĽṹ.
			CGoods::tagAddonProperty apGoodsAddonProperty;
			apGoodsAddonProperty.gapType = vOut[i];
			apGoodsAddonProperty.lValues[0]=0;
			apGoodsAddonProperty.lValues[1]=0;				

			//##��ȡ�����Ե�ֵ.
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vValues;
			pProperties->GetAddonPropertyValues( vOut[i], vValues );
			if(vValues.size()==2)
			{
				apGoodsAddonProperty.lValues[0]=vValues[0].lBaseValue;
				apGoodsAddonProperty.lValues[1]=vValues[1].lBaseValue;
			}
			//##���ø���������ӵ���Ʒ�ṹ��.
			pGoods->m_vAddonPropertiesList.push_back( apGoodsAddonProperty );
		}

		CGUID guid;
		CGUID::CreateGUID(guid);
		pGoods->SetExID(guid);
		s_UsingGoods[lType].insert(pGoods);
	}
	return pGoods;
}

void CGoodsFactory::CreateGoods(ulong dwID, ulong dwAmount, vector<CGoods*>& vOut,long lType)
{
	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties(dwID);
	if(pProperties)
	{
		//�ɵ��ӵ�����Ʒ�Ͳ���ʹ�õ���Ʒ
		if( pProperties->GetGoodsBaseType() == GBT_CONSUMABLE ||
			pProperties->GetGoodsBaseType() == GBT_NORMAL ||
			pProperties->GetGoodsBaseType() == GBT_USELESS )
		{
			ulong dwRemainedGoods = dwAmount;
			ulong dwMaxStackNumber = GoodsSetup::QueryGoodsMaxStackNumber(dwID);
			while(dwRemainedGoods != 0)
			{
				CGoods* pGoods = CreateGoods( dwID,lType );
				if(pGoods)
				{
					/*
					*�����Ҫ����������������Ʒ��������
					*��ִδ�����Ʒ ����Ʒ��������Ϊ������ֵ
					*/
					if(dwRemainedGoods > dwMaxStackNumber)
					{
						pGoods->SetAmount(dwMaxStackNumber);
						dwRemainedGoods -= dwMaxStackNumber;
					}
					else 
					{
						pGoods->SetAmount(dwRemainedGoods);
						dwRemainedGoods = 0;
					}
					vOut.push_back(pGoods);
				}
			}
		}
		else
		{
			for(ulong i = 0; i < dwAmount; i++)
			{
				CGoods* pGoods = CreateGoods(dwID,lType);
				if(pGoods)
				{
					vOut.push_back(pGoods);
				}
			}
		}
	}
}

bool CGoodsFactory::GarbageCollect(CGoods** ppGoods)
{
	bool bResult = false;
	if( ppGoods )
	{
		map<long,set<CGoods*> >::iterator it = s_UsingGoods.begin();
		for(;it != s_UsingGoods.end();it++)
		{
			(*it).second.erase(*ppGoods);
		}

		SAFE_DELETE( *ppGoods );
		bResult = true;
	}
	return bResult;
}

CGoods* CGoodsFactory::UnserializeGoods(uchar* pStream, long& lOffset,long lType)
{
	CGoods* pResult = NULL;
	if( pStream )
	{
		pResult = new CGoods;
		if( pResult == NULL )
		{
			assert( 0 );
			OutputDebugString( "Can't alloc memory for unserialize goods.\n" );
		}
		else 
		{
			pResult->Unserialize( pStream, lOffset );

			CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( 
				pResult->GetBasePropertiesIndex() );
			if(! pProperties )
			{
				GarbageCollect( &pResult );
			}
			else
			{
				pResult->SetBaseProperties(pProperties);
				s_UsingGoods[lType].insert(pResult);
			}
		}
	}
	return pResult;
}


//ͨ��ByteArray��ԭ��Ʒ
CGoods* CGoodsFactory::UnserializeGoods(DBReadSet& setReadDB,long lType)
{
	CGoods* pResult = NULL;	
	pResult = new CGoods;
	if( pResult == NULL )
	{
		assert( 0 );
		OutputDebugString( "Can't alloc memory for unserialize goods.\n" );
	}
	else 
	{
		pResult->Unserialize( setReadDB );
		
		CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( 
			pResult->GetBasePropertiesIndex() );
		if(! pProperties )
		{
			GarbageCollect( &pResult );
		}
		else
		{
            pResult->SetBaseProperties(pProperties);
			s_UsingGoods[lType].insert(pResult);
		}
	}
	return pResult;
}

bool CGoodsFactory::UpgradeEquipment(CGoods* pGoods, long lTargetLevel)
{		
	return true;
}

bool CGoodsFactory::Upgrade(CGoods* pGoods, eGoodsAddonProperties gapSource,
							eGoodsAddonProperties gapDest, bool bUpgrade)
{
	return true;
}

map<CGUID, vector<uchar>>	CGoodsFactory::m_mapChatGoodsInfo;
list<pair<CGUID, long>>		CGoodsFactory::m_listSaveTimeOrder;

//! ���һ����Ʒ��Ϣ
void CGoodsFactory::AddGoodsInfo(const CGUID &Key, const vector<uchar> &vGoodsInfo)
{
	assert(m_mapChatGoodsInfo.end() == m_mapChatGoodsInfo.find(Key));
	if (GlobalSetup::GetSetup()->lMaxGoodInfoNum <= (long)m_mapChatGoodsInfo.size())
	{
		ClearInfo(GlobalSetup::GetSetup()->lMaxGoodInfoNum / 10);
	}
	
	m_mapChatGoodsInfo[Key] = vGoodsInfo;
	m_listSaveTimeOrder.push_back(pair<CGUID, long>(Key, (long)time(NULL)));
}

//! ȡ��һ����Ʒ��Ϣ
bool CGoodsFactory::GetGoodsInfo(const CGUID &Key, vector<uchar> &vGoodsInfo)
{
	map<CGUID, vector<uchar>>::iterator ite = m_mapChatGoodsInfo.find(Key);
	if (m_mapChatGoodsInfo.end() != ite)
	{
		vGoodsInfo.resize(ite->second.size());
		memmove(&vGoodsInfo[0], &(ite->second[0]), ite->second.size());
		return true;
	}
	return false;
}

//! ����
void CGoodsFactory::ClearInfo(long lClearNum)
{
	if (0 >= lClearNum)
		lClearNum = 1;

	list<pair<CGUID, long>>::iterator ite = m_listSaveTimeOrder.begin();
	long i = 0;
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
//	char str[1024];
	//PutStringToFile(pszFileName,"UseGoodsInfo:");
    Log4c::Trace(ROOT_MODULE,"UseGoodsInfo:");
	map<long,set<CGoods*> >::iterator it = s_UsingGoods.begin();
	for(;it != s_UsingGoods.end();it++)
	{
		if((*it).second.size() > 0)
		{
// 			sprintf(str,"Goods Flag:%d,Count:%d",(*it).first,(*it).second.size() );
// 			PutStringToFile(pszFileName,str);

            Log4c::Trace(ROOT_MODULE,"Goods Flag:%d,Count:%d",(*it).first,(*it).second.size() );
		}
	}
	return 0;
}
