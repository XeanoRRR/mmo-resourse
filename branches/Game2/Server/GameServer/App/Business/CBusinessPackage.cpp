///
/// @file CBusinessPackage.cpp
/// @brief ���̱�������
///
#include "StdAfx.h"
#include "CBusinessPackage.h"
#include "BusinessCommon.h"
#include "CBusinessManager.h"

#include "../Container/CVolumeLimitGoodsContainer.h"
#include "../Goods/CGoodsFactory.h"
#include "../Goods/CGoods.h"
#include "../Player.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Business
{
	CPackage::CPackage()
	{
		m_pContainer = OBJ_CREATE(CVolumeLimitGoodsContainer);
		m_pContainer->SetContainerVolume( 0 );
		m_pGoods = NULL;
		m_dwGoodsCount = m_dwMaxGoodsCount = 0;
	}

	CPackage::~CPackage()
	{
		if( m_pGoods != NULL )
		{
			CGoodsFactory::GarbageCollect( &m_pGoods, 48 );
		}
		OBJ_RELEASE(CVolumeLimitGoodsContainer, m_pContainer);
	}

	bool CPackage::IsValid() const
	{
		// ��ҵ������Ч���������ڣ������ݻ�����0�Ҷ�Ӧ�ı�����Ʒ���ڣ��ұ�����ǰ�־ô���0
		return m_pContainer->GetVolume() > 0 && m_pGoods != NULL &&
			m_pGoods->GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 ) > 0;
	}

	LONG CPackage::GetCurDurability()
	{
		if( m_pGoods == NULL )
		{
			return 0;
		}
		return m_pGoods->GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 );
	}

	DWORD CPackage::UpdateEndure( DWORD dwProfit )
	{
		if( m_pGoods == NULL )
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "Busiess package GOODS does not exist!" );
			return 0;
		}

		DWORD dwSubEndure = (DWORD)(dwProfit * GetInst( CBusinessManager ).GetSetup().fEndureQuo);
		DWORD dwCurEndure = GetCurDurability();
		DWORD dwRet = dwCurEndure >= dwSubEndure ? dwCurEndure - dwSubEndure : 1;
		m_pGoods->ChangeAttribute( GAP_GOODS_MAX_DURABILITY, dwRet, 2, true );
		
		return dwRet;
	}

	void CPackage::UpdateEndureToClient( const CGUID &player_id )
	{
		if( m_pGoods == NULL )
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"Busiess package GOODS does not exist!" );
			return ;
		}
		// TODO:�ͻ��˼���;�Ϊ0�ñ�����ʧ
		m_pGoods->UpdateAttribute( player_id );
	}

	// ��õ�ǰ��ҵ��������Ʒ����
	DWORD CPackage::GetCurGoodsAmount()
	{
		if(m_pGoods && m_pContainer)
		{
			DWORD dwNum=0;
			CAmountLimitGoodsContainer::goodsIt it=m_pContainer->GetGoodsMap()->begin();
			for(;it!=m_pContainer->GetGoodsMap()->end();it++)
			{
				if(it->second)
				{
					dwNum+=it->second->GetAmount();
				}
			}
			return dwNum;
		}
		return 0;
	}

	DWORD CPackage::UpdateGoodsPrice( CGoods *pGoods )
	{
		if( pGoods != NULL )
		{
			DWORD ret = UpdateGoodsPrice( pGoods->GetBasePropertiesIndex(), 
				pGoods->GetAmount(), pGoods->GetBuyPrice() );
			pGoods->SetBuyPrice( ret );
			return ret;
		}

		return 0;
	}

	DWORD CPackage::UpdateGoodsPrice( DWORD index, DWORD amount, DWORD price )
	{
		vector<CGoods*> goods_list;
		DWORD amount_sum = 0;
		DWORD base_price = 0;

		CAmountLimitGoodsContainer::goodsIt it = m_pContainer->GetGoodsMap()->begin();
		for( ; it != m_pContainer->GetGoodsMap()->end(); ++it )
		{
			CGoods *pGoods = it->second;
			if( pGoods != NULL && pGoods->GetBasePropertiesIndex() == index )
			{
				amount_sum += pGoods->GetAmount();
				base_price = pGoods->GetBuyPrice();
				goods_list.push_back( pGoods );
			}
		}

		DWORD price_ret = ( amount * price + amount_sum * base_price ) / ( amount + amount_sum );

		for( vector<CGoods*>::iterator it = goods_list.begin(); it != goods_list.end(); ++ it )
		{
			(*it)->SetBuyPrice( price_ret );
		}

		return price_ret;
	}

	// ��õ�ǰ��ҵ����ʹ���˶��ٸ���
	DWORD CPackage::GetUsedVolume()
	{
		if(m_pGoods && m_pContainer)		
			return (DWORD)m_pContainer->GetGoodsMap()->size();
		
		return 0;
	}

	BOOL CPackage::MountBusinessPackGoods(CGoods* pGoods)
	{
		if( pGoods == NULL )
		{
			return FALSE;
		}

		if( !CanSwap( pGoods ) )
		{
			return FALSE;
		}

		LONG lVolumeLimit = pGoods->GetAddonPropertyValues( GAP_PACK_CAPACITY, 1 );
		LONG lAmountLimit = pGoods->GetAddonPropertyValues( GAP_PACK_CAPACITY, 2 );

		// ��װ������Ʒ
		m_pGoods = pGoods;
		m_pContainer->ChangeContainerVolume( lVolumeLimit );
		m_dwMaxGoodsCount = lAmountLimit;
		return TRUE;
	}

	BOOL CPackage::CanSwap( CGoods *pPackGoods )
	{
		DWORD dwPackType = pPackGoods->GetAddonPropertyValues( GAP_PACK_TYPE, 1 );
		if( dwPackType != SPT_BUSINESS )
		{
			return FALSE;
		}

		if( m_pGoods == NULL )
		{
			return TRUE;
		}

		// �µ����̱��������������ݻ�����Ʒ��������	
		LONG lVolumeLimit = pPackGoods->GetAddonPropertyValues( GAP_PACK_CAPACITY, 1 );
		LONG lAmountLimit = pPackGoods->GetAddonPropertyValues( GAP_PACK_CAPACITY, 2 );

		// ����Ѿ���װ�������̱�����������ͬһ��Ʒ
		if( m_pGoods != NULL )
		{
			if( ( m_pGoods == pPackGoods ) || ( m_pGoods->GetExID() == pPackGoods->GetExID() ) )
			{
				return FALSE;
			}
			
			// �±����������
			if( lVolumeLimit < (long)GetUsedVolume() )
			{
				return FALSE;
			}

			// ��Ʒ�ݻ����ɲ��µ�ǰ��Ʒ��
			DWORD dwCurGoodsAmount = GetCurGoodsAmount();
			if( lAmountLimit < (long)GetCurGoodsAmount() )
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	int CPackage::Swap( CGoods *pPackGoods )
	{
		MountBusinessPackGoods( pPackGoods );
		DWORD dwGoodsAmount = m_pContainer->GetGoodsAmount();
		int ret = 0;
		if( dwGoodsAmount > 0 )
		{
			ret = 2;
			m_pContainer->ReFillCell();
		}
		else
		{
			ret = 1;
		}

		return ret;
	}

	bool CPackage::CheckValid()
	{
		if( m_pGoods == NULL )
		{
			return false;
		}

		if( GetCurDurability() <= 1 )
		{
			// TODO:ɾ�����ݿ��еı���
			m_pContainer->Release();
			CGoodsFactory::GarbageCollect( &m_pGoods, 48 );
			m_pGoods = NULL;
			return false;
		}

		return true;
	}

	void CPackage::EncodeToSave( DBWriteSet &db )
	{
		if( m_pGoods == NULL )
		{
			// invalid
			db.AddToByteArray( (BYTE) 0 );
		}
		else
		{
			db.AddToByteArray( (BYTE) 1 );
			m_pGoods->Serialize( db );
			m_pContainer->Serialize( db );
		}
	}

    void CPackage::CodeForClient(DBWriteSet& db)
    {
        if( IsValid() )
        {
            // ��������
            db.AddToByteArray( (BYTE) 1 );
            m_pGoods->SerializeForOldClient( db );

            // ��������Ʒ
            db.AddToByteArray( (DWORD) m_pContainer->GetGoodsAmount() );
            CPacketListener cPacketListener;
            cPacketListener.m_pDBWriteData = &db;
            m_pContainer->TraversingContainer( &cPacketListener );
        }
        else
        {
            db.AddToByteArray( (BYTE) 0 );
        }
    }

	void CPackage::DecodeFromDB( DBReadSet &db )
	{
		BYTE valid = db.GetByteFromByteArray();
		if( valid == 1 )
		{
			// valid
			CGoods *pGoods = CGoodsFactory::UnserializeGoods( db ,54);
			if( pGoods == NULL )
			{
				Log4c::Warn(ROOT_MODULE, "Decode pakcage goods failed." );
			}
			MountBusinessPackGoods( pGoods );
			m_pContainer->Unserialize( db );
		}
	}
}

