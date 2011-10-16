// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "CGoods.h"
#include "CGoodsFactory.h"
#include "../GoodsExManage.h"
#include "..\Player.h"
#include "..\ServerRegion.h"
#include "enchase\cenchasehole.h"
#include "enchase\CKitCardRuleFactory.h"


CGoods::CGoods()
{
	// TODO: Add your specialized code here.
	m_lType		= TYPE_GOODS;
	
	m_dwAmount	= 1;
	m_dwPrice	= 0;
	m_dwSilverPrice=0;
	m_dwBuyPrice=0;
	m_dwMaxHoleNum=0;
	m_pEnchaseHoleArray=NULL;
	m_dwActivedKitCardId=0;
	
}


CGoods::~CGoods()
{
	// TODO: Add your specialized code here.
	Release();
}

// [070608 AHC]
bool  CGoods::SetAttribute(DWORD ID, DWORD value, int valuePos) 
{
	// ��ӵ�������
	m_listDirtyAttrData.insert(m_listDirtyAttrData.end(), DIRTYATTRDATA(ID, valuePos));

    if(GP_PRICE == ID)
    {
        m_dwPrice = value;
        return true;
    }
    else if(GP_DESCRIBE == ID)
    {         
       return true;       
    }
    else if(GP_TYPE == ID)
    {       
        return true;
    }
    else if(GP_EQUIPPLACE == ID)
    {       
        return true;
    }
    return (TRUE == SetAddonPropertyBaseValues( (GOODS_ADDON_PROPERTIES)ID, valuePos, value ));
}

bool  CGoods::AddAttribute(DWORD ID, DWORD value, int valuePos/*��pos��ֵ*/)
{
	// ��ӵ�������
	m_listDirtyAttrData.insert(m_listDirtyAttrData.end(), DIRTYATTRDATA(ID, valuePos));

    if(GP_PRICE == ID)
    {
        m_dwPrice = value;
        return true;
    }
    else if(GP_DESCRIBE == ID)
    {
		return true;
    }
    else if(GP_TYPE == ID)
    {        
        return true;
    }
    else if(GP_EQUIPPLACE == ID)
    {       
        return true;
    }
    return (TRUE == AddAddonPropertyBaseValues( (GOODS_ADDON_PROPERTIES)ID, valuePos, value ));
}

DWORD CGoods::GetAttribute(DWORD ID, int valuePos)
{
    if(GP_PRICE == ID)
    {
        return CGoodsFactory::QueryGoodsPrice(valuePos);
    }
    else if(GP_DESCRIBE == ID)
    {
        return (DWORD)CGoodsFactory::QueryGoodsDescribe(valuePos);
    }
    else if(GP_TYPE == ID)
    {        
        return (DWORD)CGoodsFactory::QueryGoodsType(valuePos);
    }
    else if(GP_EQUIPPLACE == ID)
    {
        return (DWORD)CGoodsFactory::QueryGoodsEuipPlace(valuePos);
    }
    else if(GP_ORIGINAL_NAME == ID)
    {
        return (DWORD)CGoodsFactory::QueryGoodsOriginalName(valuePos);
    }
    else if(GP_NAME == ID)
    {        
        return (DWORD)CGoodsFactory::QueryGoodsName(valuePos);
    }
    return GetAddonPropertyValues( (GOODS_ADDON_PROPERTIES)ID, valuePos );
}
void  CGoods::ChangeAttribute(DWORD ID, DWORD value, int valuePos/*��pos��ֵ*/, bool bOverload)
{
	if(bOverload)
	{
		SetAddonPropertyBaseValues( (GOODS_ADDON_PROPERTIES)ID, valuePos, value);
	}
	else
	{
		SetAddonPropertyBaseValues( (GOODS_ADDON_PROPERTIES)ID, valuePos,
			value+GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)ID, valuePos) );
	}

    // ��ӵ�������
	list<DIRTYATTRDATA>::iterator itr = m_listDirtyAttrData.begin();

	for(; itr != m_listDirtyAttrData.end(); itr++)
	{
		if(itr->ID == ID && itr->valuePos == valuePos)
			return;
	}

	m_listDirtyAttrData.insert(m_listDirtyAttrData.end(), DIRTYATTRDATA(ID, valuePos));
}
long  CGoods::UpdateAttribute(const CGUID& sendPlayerID)
{
    long num = 0;

    CMessage msg( MSG_S2C_PLAYER_CHANGE_PROPERTIES );

	

	// CGood ExID
	msg.Add(GetExID());

	
	// value num
	msg.Add((long)m_listDirtyAttrData.size());
	
	list<DIRTYATTRDATA>::iterator itr = m_listDirtyAttrData.begin();
    for(; itr != m_listDirtyAttrData.end(); itr++)
    {
        DIRTYATTRDATA& data = *itr;

		// ID
		msg.Add((long)data.ID);
		
		// pos
		msg.Add((long)data.valuePos);
		

		// value
        msg.Add(GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)data.ID, data.valuePos));
		
    }	
    msg.SendToPlayer(sendPlayerID);
    m_listDirtyAttrData.clear();

    return num;
}

DWORD CGoods::GetBasePropertiesIndex()
{	
	return m_dwBasePropertiesIndex;
}


VOID CGoods::InitEnchaseHole()
{
	//����Կ���
	DWORD dwNum=GetMaxEnchaseHoleNum();
	if(dwNum>0)
	{
		m_pEnchaseHoleArray=new  CEnchaseHole*[dwNum];
		m_dwMaxHoleNum=dwNum;
		for(int i=0;i<dwNum;i++)
		{
			m_pEnchaseHoleArray[i]=new CEnchaseHole(this);
		}
	}
}

VOID CGoods::EnchaseRelease()
{	
	if(m_dwMaxHoleNum>0 && m_pEnchaseHoleArray)
	{
		for(int i=0;i<m_dwMaxHoleNum;i++)
		{
			SAFE_DELETE(m_pEnchaseHoleArray[i]);
		}
	}
	SAFE_DELETE(m_pEnchaseHoleArray);
	m_dwMaxHoleNum=0;
}

BOOL CGoods::ActiveEnchaseHole(CPlayer* pPlayer,int nIndex)
{	
	if(m_dwMaxHoleNum>0 && nIndex>=0 && nIndex<m_dwMaxHoleNum)
	{
		//������δ����״̬ ��δ��Ƕ��Ƭ
		if(m_pEnchaseHoleArray[nIndex]->GetState() || 
			m_pEnchaseHoleArray[nIndex]->GetCard())
		{
			return FALSE;
		}	
		m_pEnchaseHoleArray[nIndex]->SetState(TRUE);
		if(m_dwActivedKitCardId>0)
		{
			CKitCardRuleFactory::KitCardInvalidate(pPlayer,this,m_dwActivedKitCardId);
			SetActivedKitCardId(0);
		}
		CKitCardRuleFactory::KitCardActive(pPlayer,this);
		return TRUE;
	}	
	return FALSE;
}

VOID CGoods::AddRefine(long lVal)
{
	if(HasAddonProperty(GAP_REFINE))
	{
		long lCurVal=GetAddonPropertyValues(GAP_REFINE,1);
		long lMaxVal=GetAddonPropertyValues(GAP_REFINE,2);
		lCurVal+=lVal;
		if(lCurVal<0)
			lCurVal=0;
		else if(lCurVal>lMaxVal)
			lCurVal=lMaxVal;
		SetAddonPropertyBaseValues(GAP_REFINE,1,lCurVal);
	}
}

DWORD CGoods::ActiveEnchaseHole(CPlayer* pPlayer)
{	
	for(DWORD i=0;i<m_dwMaxHoleNum;i++)
	{
		if(!m_pEnchaseHoleArray[i]->GetState() && 
			!m_pEnchaseHoleArray[i]->GetCard())
		{
			m_pEnchaseHoleArray[i]->SetState(TRUE);
			if(m_dwActivedKitCardId>0)
			{
				CKitCardRuleFactory::KitCardInvalidate(pPlayer,this,m_dwActivedKitCardId);
				SetActivedKitCardId(0);
			}
			CKitCardRuleFactory::KitCardActive(pPlayer,this);
			return i;
		}
	}	
	return -1;
}


BOOL CGoods::CloseEnchaseHole(CPlayer* pPlayer,int nIndex)
{
	DWORD dwNum=GetMaxEnchaseHoleNum();
	if(dwNum>0 && nIndex>=0 && nIndex<dwNum)
	{		
		CGoods* pCard=m_pEnchaseHoleArray[nIndex]->GetCard();
		if(pCard)
		{
			m_pEnchaseHoleArray[nIndex]->RemoveCard(pPlayer,false);
			m_pEnchaseHoleArray[nIndex]->SetCard(NULL);
		}
		m_pEnchaseHoleArray[nIndex]->SetState(FALSE);
		//Ŀǰ�׿�����
		if(m_dwActivedKitCardId>0)
		{
			CKitCardRuleFactory::KitCardInvalidate(pPlayer,this,m_dwActivedKitCardId);
			SetActivedKitCardId(0);
		}
		CKitCardRuleFactory::KitCardActive(pPlayer,this);
		return TRUE;
	}	
	return FALSE;
}


DWORD CGoods::CloseEnchaseHole()
{
	DWORD dwNum=GetMaxEnchaseHoleNum();
	if(dwNum>0)
	{
		DWORD i=dwNum-1;
		for(;i>=0;i--)
		{
			if(m_pEnchaseHoleArray[i]->GetState())
			{
				if(m_pEnchaseHoleArray[i]->GetCard())
				{
				}
				m_pEnchaseHoleArray[i]->SetState(FALSE);
				return i;
			}
		}	
	}
	return -1;
}

//һ�μ���ָ�������Ŀ�
VOID CGoods::ActiveHoles(int nNum)
{
	DWORD dwNum=GetMaxEnchaseHoleNum();
	if(nNum>0 && nNum<=dwNum)
	{
		int j=0;
		for(DWORD i=0;i<dwNum;i++)
		{			
			m_pEnchaseHoleArray[i]->SetState(TRUE);			
			j++;
			if(j>=nNum)
				return;
		}	
	}
}


//��Ƭ����������
VOID CGoods::EnchaseArrange()
{	
	vector<CGoods*> vecGoods;
	vecGoods.clear();
	//������
	DWORD dwNum=GetMaxEnchaseHoleNum();

	//wang-xx
	////�������
	
	for(DWORD i=0;i<dwNum;i++)
	{	
		if( m_pEnchaseHoleArray[i]->GetCard() )
		{
			//����Ƕ�˵Ŀ�����vector
			vecGoods.push_back(m_pEnchaseHoleArray[i]->GetCard());
		}			
		m_pEnchaseHoleArray[i]->SetCard(NULL);
	}		
	
	//��˳��װ��
	vector<CGoods*>::iterator goodsIter=vecGoods.begin();
	int j=0;
	for(;goodsIter!=vecGoods.end();goodsIter++)
	{
		m_pEnchaseHoleArray[j]->SetCard(*goodsIter);
		j++;
	}
}

VOID CGoods::OnSuitNumChanged(CPlayer* pPlayer,int new_num)
{
	InvalidSuitProperties(pPlayer);

	vector<CGoodsBaseProperties::tagSuitProperty>::iterator propertyIter=m_vSuitProperties.begin();
	for(;propertyIter!=m_vSuitProperties.end();propertyIter++)
	{
		if(propertyIter->vValues[2].lBaseValue<=new_num)
		{
			string strPropertyName=CPlayer::GetFightPropertyNameByEnum(propertyIter->gapType);
			if(strPropertyName!="")
			{
				pPlayer->ChangeAttribute(strPropertyName,propertyIter->vValues[0].lBaseValue);
				propertyIter->bActived=TRUE;
			}
		}
	}
	
}

VOID CGoods::InvalidSuitProperties(CPlayer* pPlayer)
{
	vector<CGoodsBaseProperties::tagSuitProperty>::iterator propertyIter=m_vSuitProperties.begin();
	for(;propertyIter!=m_vSuitProperties.end();propertyIter++)
	{				
	    if(propertyIter->bActived)
		{
			string strPropertyName=CPlayer::GetFightPropertyNameByEnum(propertyIter->gapType);
			propertyIter->bActived=FALSE;			
			if(strPropertyName!="")
			{
				pPlayer->ChangeAttribute(strPropertyName,-(propertyIter->vValues[0].lBaseValue));
			}
		}
	}
}


VOID CGoods::RunScriptOnGet(CPlayer* pPlayer)
{
	
	if(pPlayer==NULL)
		return;
	DWORD dwVal=this->GetAttribute(GAP_GET_TRIGGER,1);
	if(dwVal!=0)
	{
		char strFile[64];
		_snprintf(strFile, 64, "scripts/goods/%d.script",dwVal);
	    stRunScript st;
		st.pszFileName = strFile;
		st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
		st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
		st.desShape = NULL;
		st.guUsedItemID = GetExID();
		RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));
	}
}

VOID CGoods::RunScriptOnLose(CPlayer* pPlayer)
{
	
	if(pPlayer==NULL)
		return;
	DWORD dwVal=this->GetAttribute(GAP_LOST_TRIGGER,1);
	if(dwVal!=0)
	{
		char strFile[64];
		_snprintf(strFile, 64, "scripts/goods/%d.script",dwVal);
	    stRunScript st;
		st.pszFileName = strFile;
		st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
		st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
		st.desShape = NULL;
		st.guUsedItemID = GetExID();
		RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));
	}
}

VOID CGoods::RunScriptOnEquip(CPlayer* pPlayer)
{
	
	if(pPlayer==NULL)
		return;
	DWORD dwVal=this->GetAttribute(GAP_EQUIP_TRIGGER,1);
	if(dwVal!=0)
	{
		char strFile[64];
		_snprintf(strFile, 64, "scripts/goods/%d.script",dwVal);
	    stRunScript st;
		st.pszFileName = strFile;
		st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
		st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
		st.desShape = NULL;
		st.guUsedItemID = GetExID();
		RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));
	}
}


VOID CGoods::RunScriptOnRemoveEquip(CPlayer* pPlayer)
{
	
	if(pPlayer==NULL)
		return;
	DWORD dwVal=this->GetAttribute(GAP_UNEQUIP_TRIGGER,1);
	if(dwVal!=0)
	{
		char strFile[64];
		_snprintf(strFile, 64, "scripts/goods/%d.script",dwVal);
	    stRunScript st;
		st.pszFileName = strFile;
		st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
		st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
		st.desShape = NULL;
		st.guUsedItemID = GetExID();
		RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));
	}
}


VOID CGoods::SetAmount(DWORD dwAmount)
{	
	if( dwAmount == 0 )
	{
		//##���Լ���������ɾ����
	}
	m_dwAmount = dwAmount;
}

DWORD CGoods::GetAmount()
{	
	return m_dwAmount;
}

//ȡ�õ��ߵ�������������
DWORD CGoods::GetMaxStackNumber()
{	
	CGoodsBaseProperties* pProperties = 
		CGoodsFactory::QueryGoodsBaseProperties( GetBasePropertiesIndex() );
	if(GetBasePropertiesIndex()==CGoodsFactory::GetGoldCoinIndex())
	{
		return CGlobeSetup::GetSetup()->dwGoldCoinLimit;
	}
	else if(GetBasePropertiesIndex()==CGoodsFactory::GetSilverIndex())
	{
		return CGlobeSetup::GetSetup()->dwSilverLimit;
	}

	DWORD dwResult = 1;
	if( pProperties && (pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_CONSUMABLE ||
		pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_USELESS || 
		pProperties->GetGoodsBaseType()==GT_CARD) )
	{
		vector<CGoodsBaseProperties::tagAddonPropertyValue> vOut;
		pProperties -> GetAddonPropertyValues( GAP_MAXOVERLAP, vOut );
		for( size_t i = 0; i < vOut.size(); i ++ )
		{
			if( vOut[i].dwId == 1 )
			{
				dwResult = static_cast<DWORD>( vOut[i].lBaseValue );
				break;
			}
		}
		vOut.clear();
	}

	return dwResult;
}

//! �õ���Ʒ�İ󶨶�������
CGoods::eBindFreeze CGoods::GetBindFreezeProperties(void)
{
	long lValue = GetAddonPropertyValues(GAP_FREEZE, 1);
	if (0 != lValue) return eBF_Freeze;
	
	lValue = GetAddonPropertyValues(GAP_BIND, 1);
	if (0 == lValue && 2 == lValue) return eBF_NoAll;

	return eBF_OnlyBind;
}

//! �ж���Ʒ�ܷ���������������Ʒ�ȼ��������ܣ�����-1
long CGoods::CanUpgrade(void)
{
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties( GetBasePropertiesIndex() );
	if( pProperties && pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_EQUIPMENT )
	{
		for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
		{
			if( m_vAddonProperties[i].gapType == GAP_WEAPON_LEVEL )
			{
				return m_vAddonProperties[i].lValues[0];
			}
		}
	}
	return -1;
}

//! �ж���Ʒ�ܷ�ħ������������Ʒħ���ȼ��������ܣ�����-1
long CGoods::CanPlugEvil(void)
{
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties( GetBasePropertiesIndex() );
	if( pProperties && pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_EQUIPMENT )
	{
		for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
		{
			if( m_vAddonProperties[i].gapType == GAP_PLUG_EVIL_LEVEL )
			{
				return m_vAddonProperties[i].lValues[0];
			}
		}
	}
	return -1;
}

BOOL CGoods::CanUpgraded()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile

	CGoodsBaseProperties* pProperties = 
		CGoodsFactory::QueryGoodsBaseProperties( GetBasePropertiesIndex() );
	if( pProperties && pProperties -> GetGoodsType() == 
		CGoodsBaseProperties::GT_EQUIPMENT )
	{
		for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
		{
			if( m_vAddonProperties[i].gapType == GAP_WEAPON_LEVEL )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
	//return TRUE;
}

//�ж���Ʒ�Ƿ��������
BOOL CGoods::CanReparied()
{	
	BOOL bResult = FALSE;
	CGoodsBaseProperties* pProperties = 
		CGoodsFactory::QueryGoodsBaseProperties( GetBasePropertiesIndex() );
	//����Ʒ��װ����û���������Ե�����
	if( pProperties && pProperties -> GetGoodsType() == 
		CGoodsBaseProperties::GT_EQUIPMENT )
	{
		if( ( GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) & 
			CGoodsBaseProperties::PA_CANNOT_REPAIRED ) != CGoodsBaseProperties::PA_CANNOT_REPAIRED )
		{
			bResult = TRUE;
		}
	}
	return bResult;
}

CGoodsBaseProperties::EQUIP_TYPE CGoods::QueryEquipType()
{
	if(m_GoodsBaseType==GT_WEAPON)
	{
		return CGoodsBaseProperties::ATTACK;
	}
	else if(m_GoodsBaseType==GT_HEAD || m_GoodsBaseType==GT_GLOVE || 
		m_GoodsBaseType==GT_BODY || m_GoodsBaseType==GT_BOOT || m_GoodsBaseType==GT_WEAPON2)
	{
		return CGoodsBaseProperties::DEFENSE;
	}
	return CGoodsBaseProperties::NEITHER_ATTACK_DEFENSE;

}

eWeaponType CGoods::GetWeaponType()
{
	DWORD dwVal=GetAddonPropertyValues(GAP_WEAPON_CATEGORY,1);
	if(dwVal>=WT_SINGLE_SWORD && dwVal<=WT_FIST)
	{
		return static_cast<eWeaponType>(dwVal);
	}
	else
		return WT_UNKNOWN;
}


eWeaponHandType CGoods::GetWeaponHandType()
{
	eWeaponType eWT=GetWeaponType();
	if(eWT!=WHT_UNKNOWN)
	{
		if(eWT==WT_SINGLE_SWORD || eWT==WT_SINGLE_KNIFE || eWT==WT_SINGLE_HAMMER)
			return WHT_SINGLE_HAND;
		else
			return WHT_DOUBLE_HAND;
	}
	else
		return WHT_UNKNOWN;
}

eBusinessGoodsType CGoods::GetBusinessGoodsType()
{
	DWORD dwVal=GetAddonPropertyValues(GAP_TRADEGOODS_TYPE,1);
	if(dwVal>=BGT_FOOD && dwVal<BGT_MAX_TYPE)
	{
		eBusinessGoodsType eVal=static_cast<eBusinessGoodsType>(dwVal);
		return eVal;
	}	
	return BGT_NOT_BUSINESS_GOODS;
}

BOOL CGoods::CanEnchase()
{
	LONG lBaseHoleNum=GetAddonPropertyValues(GAP_HOLE_NUM,2);
	if(lBaseHoleNum>0)
	{
		return TRUE;
	}
	return FALSE;
}
LONG* CGoods::GetEnchaseHoleData()
{
	LONG* pArray=new LONG[5];
	DWORD dwNum=GetMaxEnchaseHoleNum();
	for(int i=0;i<5;i++)
	{			
		if(i<dwNum)
		{
			if(m_pEnchaseHoleArray[i]->GetCard())
			{
				pArray[i]=m_pEnchaseHoleArray[i]->GetCard()->GetBasePropertiesIndex();
			}
			else if(m_pEnchaseHoleArray[i]->GetState())
			{
				pArray[i]=0;
			}	
			else 
			{
				pArray[i]=-1;
			}
		}
		else
		{
			pArray[i]=-1;
		}

	}

	return pArray;
}
DWORD CGoods::GetMaxEnchaseHoleNum()
{
	return GetAddonPropertyValues(GAP_HOLE_NUM,2);
}

DWORD CGoods::GetActivedHoleNum()
{
	DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
	if(dwMaxHoleNum>0)
	{
		DWORD dwActivedHoleNum=0;
		for(int i=0;i<dwMaxHoleNum;i++)
		{
			if(m_pEnchaseHoleArray[i]->GetState())
			{
				dwActivedHoleNum++;
			}
		}
		return dwActivedHoleNum;
	}	
	return 0;
	
}

DWORD CGoods::GetEnchasedCardNum()
{
	DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
	if(dwMaxHoleNum>0)
	{
		DWORD dwCardNum=0;
		for(int i=0;i<dwMaxHoleNum;i++)
		{
			if(m_pEnchaseHoleArray[i]->GetCard())
			{
				dwCardNum++;
			}
		}
		return dwCardNum;
	}
	return 0;
}

VOID CGoods::GetEnchasedCard(vector<DWORD>& vecCards)
{
	vecCards.clear();
	DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
	if(dwMaxHoleNum>0)
	{
		DWORD dwCardNum=0;
		for(int i=0;i<dwMaxHoleNum;i++)
		{
			CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
			if(pCard)
			{
				vecCards.push_back(pCard->GetBasePropertiesIndex());
			}
		}
	}
}


CGoods* CGoods::GetEnchasedCard(int nIndex)
{
	DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
	if(nIndex>=0 && nIndex<dwMaxHoleNum)
	{
		return m_pEnchaseHoleArray[nIndex]->GetCard();
	}
	return NULL;
}

VOID CGoods::UpdateEnchaseAttr()
{
	struct  stCardData
	{
		DWORD dwPos;
		CGoods* pGoods;
	};
	vector<stCardData> vecCards;
	DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
	for(DWORD i=0;i<dwMaxHoleNum;i++)
	{
		CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
		if(pCard)
		{
			stCardData stCard;
			stCard.dwPos=i;
			stCard.pGoods=pCard;
			vecCards.push_back(stCard);
			m_pEnchaseHoleArray[i]->SetCard(NULL);
		}
	}
	for(int i=0;i<vecCards.size();i++)
	{
		Enchase(NULL,vecCards[i].pGoods,vecCards[i].dwPos);
	}
}

BOOL CGoods::Enchase(CPlayer* pPlayer,CGoods* pGoods,int nIndex)
{
	if(!pGoods || nIndex<0)
		return FALSE;
	//��Ʒ����������Ƕ
	if(!CanEnchase())
		return FALSE;
	//��Ƭ��������Ƕ�Ϳ�Ƭ
	CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(pGoods->GetBasePropertiesIndex());
	if(pProperty)
	{
		//����nIndex����Ч��Χ
		if(nIndex>=GetMaxEnchaseHoleNum())
			return FALSE;
		//����û����Ƕ��Ƭ
		if(m_pEnchaseHoleArray[nIndex]->GetCard())
			return FALSE;
		//�ȼ�ƥ��
		LONG lCardLevelLimit=pGoods->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT,1);
		LONG lEquipLevelLimit=GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT,1);
		if(lCardLevelLimit>lEquipLevelLimit)
			return FALSE;
		//ͬһID��Ƭ��Ƕ��������
		LONG lNumLimit1=pGoods->GetAddonPropertyValues(GAP_CARD_ENCHASE_NUM_LIMIT,1);
		LONG lNumLimit2=pGoods->GetAddonPropertyValues(GAP_CARD_ENCHASE_NUM_LIMIT,2);
		//! ���ֵ2��Ϊ0����ֵ1Ĭ��Ϊ1���������µĿ�Ƭ��������ж�
		DWORD dwEnchaseNum = 0;
		if(0 != lNumLimit2)
		{
			lNumLimit1 = 1;
			dwEnchaseNum = GetEnchaseNumByCardGroupID(lNumLimit2);
		}
		else
		{
			dwEnchaseNum=GetEnchaseNumByGoodsId(pGoods->GetBasePropertiesIndex());
		}

		if(lNumLimit1 > 0)
		{
			if(dwEnchaseNum + 1 > lNumLimit1)
				return FALSE;
		}

		BOOL bRet=m_pEnchaseHoleArray[nIndex]->EnchaseCard(pPlayer,pGoods);
		if(!bRet)
			return FALSE;
		//Ŀǰ�׿�����
		if(m_dwActivedKitCardId>0)
		{
			CKitCardRuleFactory::KitCardInvalidate(pPlayer,this,m_dwActivedKitCardId);
			SetActivedKitCardId(0);
		}
		CKitCardRuleFactory::KitCardActive(pPlayer,this);
		//����Ӱ��			
		return TRUE;
	}
	return FALSE;
}

BOOL CGoods::RemoveCard(CPlayer* pPlayer,int nIndex,bool bReserve)
{
	DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
	if(nIndex<0 || nIndex>=dwMaxHoleNum)
		return FALSE;
	BOOL bRet=m_pEnchaseHoleArray[nIndex]->RemoveCard(pPlayer,bReserve);
	if(m_dwActivedKitCardId>0)
	{
		CKitCardRuleFactory::KitCardInvalidate(pPlayer,this,m_dwActivedKitCardId);
		SetActivedKitCardId(0);
	}
	CKitCardRuleFactory::KitCardActive(pPlayer,this);
	return bRet;
	
}

VOID CGoods::EnchaseAttrUpdate(CPlayer* pPlayer,CGoods* pCard)
{	
	vector<CGoods::tagAddonProperty>::iterator tagCardIter=pCard->GetAddonProperties()->begin();
	CGoodsBaseProperties* pEquipBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	for(;tagCardIter!=pCard->GetAddonProperties()->end();tagCardIter++)
	{	
		if(tagCardIter->gapType==GAP_PARTICULAR_ATTRIBUTE)
			continue;
		if(tagCardIter->gapType==GAP_MIN_REINFORCE_LEVEL || 
			tagCardIter->gapType==GAP_MAX_REINFORCE_LEVEL)
			continue;	
		if(tagCardIter->gapType==GAP_COLOR)
			continue;
		if(tagCardIter->gapType==GAP_QUALITY)
			continue;
		//��������װ�����Ƿ����
		BOOL bBasePropertyExist=pEquipBaseProperty->IsHasAddonPropertie(tagCardIter->gapType);
		//װ�����Ƿ��и�����
		BOOL bExist=HasAddonProperty(tagCardIter->gapType);		

		if(bExist)
		{
			//�Ѿ��и����ԣ�ֱ�Ӹ���				
			ChangeAttribute(tagCardIter->gapType,tagCardIter->lValues[0],1);
		}
		else
		{
			//�����ڸ����ԣ����			
			AddAttribute(tagCardIter->gapType,tagCardIter->lValues[0],1);
		}
	}
	if(pPlayer)
		UpdateAttribute(pPlayer->GetExID());
}

VOID CGoods::EnchaseAttrRemove(CPlayer* pPlayer,CGoods* pCard)
{	
	vector<CGoods::tagAddonProperty>::iterator tagCardIter=pCard->GetAddonProperties()->begin();
	CGoodsBaseProperties* pEquipBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	for(;tagCardIter!=pCard->GetAddonProperties()->end();tagCardIter++)
	{				
		//װ�����Ƿ��и�����
		if(tagCardIter->gapType==GAP_PARTICULAR_ATTRIBUTE)
			continue;
		if(tagCardIter->gapType==GAP_MIN_REINFORCE_LEVEL || 
			tagCardIter->gapType==GAP_MAX_REINFORCE_LEVEL)
			continue;
		if(tagCardIter->gapType==GAP_COLOR)
			continue;
		if(tagCardIter->gapType==GAP_QUALITY)
			continue;
		

		BOOL bExist=HasAddonProperty(tagCardIter->gapType);		
		if(bExist)
		{
			//�Ѿ��и����ԣ�ֱ�Ӹ���
			ChangeAttribute(tagCardIter->gapType,-(tagCardIter->lValues[0]),1);
		}		
	}
	UpdateAttribute(pPlayer->GetExID());
}

DWORD CGoods::GetEnchaseNumByGoodsId(DWORD dwId)
{
	DWORD dwRet=0;
	LONG lVal=GetMaxEnchaseHoleNum();
	for(int i=0;i<lVal;i++)
	{
		CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
		if(pCard && pCard->GetBasePropertiesIndex()==dwId)
			dwRet++;
	}
	return dwRet;
}

DWORD CGoods::GetEnchaseNumByCardGroupID(DWORD GroupID)
{
	DWORD dwRet=0;
	LONG lVal=GetMaxEnchaseHoleNum();
	for(int i=0;i<lVal;i++)
	{
		CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
		if(pCard && GroupID == pCard->GetAddonPropertyValues(GAP_CARD_ENCHASE_NUM_LIMIT, 2))
			dwRet++;
	}
	return dwRet;
}

//�Ƿ�����ص�
BOOL CGoods::CanStacked()
{
	CGoodsBaseProperties* pProperties = 
		CGoodsFactory::QueryGoodsBaseProperties( GetBasePropertiesIndex() );
	DWORD dwResult = false;
	if( pProperties && pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_CONSUMABLE ||
		pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_USELESS )
	{
		return pProperties->IsHasAddonPropertie(GAP_MAXOVERLAP);
	}	
	return FALSE;
}

BOOL CGoods::IsSuitGoods()
{
	return HasAddonPropertyValues(GAP_SUITID);
}

VOID CGoods::GetValidAddonProperties(vector<GOODS_ADDON_PROPERTIES>& vOut)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{		
		vOut.push_back( m_vAddonProperties[i].gapType );		
	}

	if( vOut.size() == 0 )
	{
		CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
			GetBasePropertiesIndex() );
		if( pProperties )
		{
			if( pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_CONSUMABLE )
			{
				pProperties -> GetValidAddonProperties( vOut );
			}
		}
	}
}


VOID CGoods::GetEnabledAddonProperties(vector<GOODS_ADDON_PROPERTIES>& vOut)
{	
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{		
		vOut.push_back( m_vAddonProperties[i].gapType );	
	}

	if( vOut.size() == 0 )
	{
		CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
			GetBasePropertiesIndex() );
		if( pProperties )
		{
			if( pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_CONSUMABLE )
			{
				pProperties -> GetValidAddonProperties( vOut );
			}
		}
	}
}


VOID CGoods::GetAddonPropertyValues( GOODS_ADDON_PROPERTIES gapType, 
									vector<long>& vOut )
{	
	//��������������
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{
		if( m_vAddonProperties[i].gapType == gapType )
		{
			vOut.push_back(m_vAddonProperties[i].lValues[0]);
			vOut.push_back(m_vAddonProperties[i].lValues[1]);			
			break;
		}
	}
	//�������û�и����κ����ԣ����Դ�CGoodsFactory�������ҡ�
	if( vOut.size() == 0 )
	{
		CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
			GetBasePropertiesIndex() );
		if( pProperties )
		{
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vValues;
			pProperties -> GetAddonPropertyValues( gapType, vValues );
			for( size_t i = 0; i < vValues.size(); i ++ )
			{
				//ֻ��ȡ����׼ֵ��				
				vOut.push_back( vValues[i].lBaseValue );
			}
		}
	}
}

LONG CGoods::GetAddonPropertyValues(GOODS_ADDON_PROPERTIES gapType, DWORD dwID)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	LONG lResult	= 0;
	if(dwID!=1 && dwID!=2)
		return lResult;	
	BOOL bFounded	= FALSE;
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{
		if( m_vAddonProperties[i].gapType == gapType )
		{
			lResult=m_vAddonProperties[i].lValues[dwID-1];
			bFounded=TRUE;
			break;			
		}
	}

	if( bFounded == FALSE )
	{
		CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
			GetBasePropertiesIndex() );
		if( pProperties )
		{
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vValues;
			pProperties -> GetAddonPropertyValues( gapType, vValues );
			for( size_t i = 0; i < vValues.size(); i ++ )
			{
				if( dwID == vValues[i].dwId )
				{
					//##ֻ��ȡ����׼ֵ��
					lResult		= vValues[i].lBaseValue;
					bFounded	= TRUE;
				}
			}
		}
	}

	return lResult;
}

bool CGoods::HasAddonPropertyValues(GOODS_ADDON_PROPERTIES gapType)
{
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
		GetBasePropertiesIndex() );
	if( pProperties )
	{
		vector<CGoodsBaseProperties::tagAddonPropertyValue> vValues;
		pProperties -> GetAddonPropertyValues( gapType, vValues );
		if (vValues.size())
			return true;
	}
	return false;
}

BOOL CGoods::HasAddonProperty(GOODS_ADDON_PROPERTIES gapType)
{	
	vector<CGoods::tagAddonProperty>::iterator propertyIter=m_vAddonProperties.begin();
	for(;propertyIter!=m_vAddonProperties.end();propertyIter++)
	{
		if(propertyIter->gapType==gapType)
			return TRUE;
	}
	return FALSE;
}

BOOL CGoods::SetAddonPropertyBaseValues(GOODS_ADDON_PROPERTIES gapType, DWORD dwID, LONG lValue)
{
	BOOL bResult = FALSE;
	if(dwID!=1 && dwID!=2)
		return bResult;

    

	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{
		if( m_vAddonProperties[i].gapType == gapType )
		{
			m_vAddonProperties[i].lValues[dwID-1]=lValue;
			bResult=TRUE;
			break;			
		}
	}
	return bResult;
}

BOOL CGoods::AddAddonPropertyBaseValues(GOODS_ADDON_PROPERTIES gapType,DWORD dwID,LONG lVal1,LONG lVal2,BOOL bEnable,BOOL bIsImplicit)
{	
	
	if(dwID!=1 && dwID!=2)
		return FALSE;
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{
		if( m_vAddonProperties[i].gapType == gapType )
		{
			return FALSE;		
		}
	}
	CGoods::tagAddonProperty stPro;
	stPro.gapType=gapType;
	stPro.lValues[dwID-1]=lVal1;	
	m_vAddonProperties.push_back(stPro);
	return TRUE;
}

VOID CGoods::AI()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
}

BOOL CGoods::Clone(CBaseObject* pObject)
{	
	CGoods* pGoods = dynamic_cast<CGoods*>( pObject );
	if( pGoods )
	{
		vector<BYTE> vData;
		LONG lOffset = 0;

		Serialize( &vData );
		pGoods -> Unserialize( &vData[0], lOffset );

		return TRUE;
	}
	return FALSE;
}

BOOL CGoods::Serialize(vector<BYTE>* pStream, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CShape::AddToByteArray( pStream, b==FALSE?false:true );

	_AddToByteArray( pStream, m_dwBasePropertiesIndex );
	_AddToByteArray( pStream, m_dwAmount );
	_AddToByteArray( pStream, m_dwPrice );
	_AddToByteArray(pStream,m_dwBuyPrice);
	_AddToByteArray(pStream,(DWORD)m_GoodsBaseType);
	
	_AddToByteArray( pStream, const_cast<CHAR*>( m_strMakerName.c_str() ) );		

	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(!pBaseProperty)
		return FALSE;
	DWORD dwAddonNum=0;
	DWORD dwPropertyId=0;
	vector<BYTE> vecAddonData;
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{		
		if(pBaseProperty->IsHasAddonPropertie(m_vAddonProperties[i].gapType))	
		{			
			dwPropertyId=m_vAddonProperties[i].gapType;
			if(CGoodsFactory::s_GoodsAttrDBSetup[dwPropertyId][0]==1)
			{
				dwAddonNum++;
				m_vAddonProperties[i].Serialize(&vecAddonData);
			}
		}
		
	}
	_AddToByteArray( pStream,dwAddonNum);
	if(dwAddonNum>0)
	{
		_AddToByteArray(pStream,&vecAddonData[0],vecAddonData.size());
	}
	

	_AddToByteArray( pStream, static_cast<DWORD>( m_vSuitProperties.size() ) );
	for( size_t i = 0; i < m_vSuitProperties.size(); i ++ )
	{
		m_vSuitProperties[i].Serialize( pStream );
	}


	EnchaseArrange();
	//enchase data	
	
	_AddToByteArray(pStream,m_dwMaxHoleNum);
	//���������
	_AddToByteArray( pStream, GetActivedHoleNum());
	_AddToByteArray( pStream, m_dwActivedKitCardId);
	for(int i=0;i<m_dwMaxHoleNum;i++)
	{
		//״̬
		//��������������Ƕ������Ϊ����
		_AddToByteArray(pStream,BYTE(1));
		
		//CARD ID
		CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
		if(pCard)
		{
			_AddToByteArray(pStream,pCard->GetBasePropertiesIndex());
			pCard->Serialize(pStream);
		}
		else
		{
			_AddToByteArray(pStream,DWORD(0));
		}
	}
	return TRUE;
}

BOOL CGoods::Unserialize(BYTE* pStream, LONG& lOffset, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	Release();

	CHAR szString[256];

	CShape::DecordFromByteArray( pStream, lOffset, b==FALSE?false:true );

	m_dwBasePropertiesIndex	= _GetDwordFromByteArray( pStream, lOffset );
	CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(pProperty)
	{
		m_dwSilverPrice=pProperty->GetSilverPrice();
	}
	m_dwAmount				= _GetDwordFromByteArray( pStream, lOffset );
	m_dwPrice				= _GetDwordFromByteArray( pStream, lOffset );
	m_dwBuyPrice=_GetDwordFromByteArray( pStream, lOffset );
	m_GoodsBaseType=static_cast<eGoodsBaseType>(_GetDwordFromByteArray(pStream,lOffset));

	

	//##��ȡ��Ʒ����
	ZeroMemory( szString, 256 );
	_GetStringFromByteArray( pStream, lOffset, szString );
	m_strMakerName			= szString;


	m_vAddonProperties.clear();
	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(!pBaseProperty)
		return FALSE;	

	
	CGoodsFactory::CreateAddonProperties(this);

	//##��ȡ��Ʒ��չ����
	DWORD dwNumProperties	= _GetDwordFromByteArray( pStream, lOffset );
	for( DWORD i = 0; i < dwNumProperties; i ++ )
	{
		CGoods::tagAddonProperty apAddonProperty;
		apAddonProperty.Unserialize( pStream, lOffset );
		
		if(pBaseProperty->IsHasAddonPropertie(apAddonProperty.gapType))
		{			
			for(int i=0;i<m_vAddonProperties.size();i++)
			{
				if(m_vAddonProperties[i].gapType==apAddonProperty.gapType)
				{
					m_vAddonProperties[i].lValues[0]=apAddonProperty.lValues[0];
					m_vAddonProperties[i].lValues[1]=apAddonProperty.lValues[1];
				}
			}			
		}
		
	}


	dwNumProperties	= _GetDwordFromByteArray( pStream, lOffset );
	for( DWORD i = 0; i < dwNumProperties; i ++ )
	{
		CGoodsBaseProperties::tagSuitProperty apAddonProperty;
		apAddonProperty.Unserialize( pStream, lOffset );
		m_vSuitProperties.push_back( apAddonProperty );
	}
	InitEnchaseHole();


	DWORD dwMaxHoleNum=_GetDwordFromByteArray(pStream,lOffset);
	DWORD dwActivedHoleNum=_GetDwordFromByteArray( pStream, lOffset );
	m_dwActivedKitCardId=_GetDwordFromByteArray( pStream, lOffset );
	for(int i=0;i<dwMaxHoleNum;i++)
	{		
		BYTE btState=_GetByteFromByteArray(pStream,lOffset);
		
		//��������������Ƕ������Ϊ����
		m_pEnchaseHoleArray[i]->SetState(TRUE);
		
		//CARD ID
		DWORD dwGoodsId=_GetDwordFromByteArray(pStream,lOffset);		

		if(dwGoodsId>0)
		{
			CGoods* pGoods = CGoodsFactory::UnserializeGoods( pStream, lOffset,40 );
			//BOOL bRet=Enchase(NULL,pGoods,i);
			if( i< m_dwMaxHoleNum)
				m_pEnchaseHoleArray[i]->SetCard(pGoods);
		}
		else
		{
			
		}
	}
	return TRUE;
}

BOOL CGoods::SerializeForOldClient(vector<BYTE>* pStream, BOOL b)
{
	BOOL bResult = FALSE;
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
		GetBasePropertiesIndex() );
	if( pProperties )
	{
		_AddToByteArray( pStream, GetBasePropertiesIndex() );
		
		_AddToByteArray( pStream, GetExID() );
		_AddToByteArray( pStream, GetAmount() );
		_AddToByteArray( pStream, const_cast<CHAR*>( GetMakerName() ) );		
		_AddToByteArray( pStream, GetPrice() );	
		_AddToByteArray(pStream,GetSilverPrice());
		_AddToByteArray(pStream,m_dwBuyPrice);
		_AddToByteArray(pStream,(DWORD)m_GoodsBaseType);
		

		CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
		if(!pBaseProperty)
			return FALSE;
		LONG lAddonNum=0;
		DWORD dwPropertyId=0;

		vector<BYTE> vecAddonData;
		for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
		{		
			if(pBaseProperty->IsHasAddonPropertie(m_vAddonProperties[i].gapType))	
			{
				dwPropertyId=m_vAddonProperties[i].gapType;

				lAddonNum++;
				_AddToByteArray( &vecAddonData, static_cast<WORD>(m_vAddonProperties[i].gapType) );
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[0] );
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[1] );
				
			}
			else
			{
				lAddonNum++;
				_AddToByteArray( &vecAddonData, static_cast<WORD>(m_vAddonProperties[i].gapType) );
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[0] );
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[1] );
			}
		}
		_AddToByteArray( pStream,lAddonNum);
		if(lAddonNum>0)
		{
			_AddToByteArray(pStream,&vecAddonData[0],vecAddonData.size());
		}
		/////////////////
		
		_AddToByteArray( pStream, GetTileX() );
		_AddToByteArray( pStream, GetTileY() );

		EnchaseArrange();

		//enchase data	
		DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
		if(dwMaxHoleNum>0)
		{
			//������
			_AddToByteArray(pStream,dwMaxHoleNum);
			//���������
			//��������Ĭ�����п׶�����������ɾ����������
			
			for(int i=0;i<dwMaxHoleNum;i++)
			{		
				//CARD ID
				CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
				if(pCard)
				{
					_AddToByteArray(pStream,pCard->GetBasePropertiesIndex());
					pCard->SerializeForOldClient(pStream);
				}
				else
				{
					_AddToByteArray(pStream,DWORD(0));
				}
			}
		}

		bResult = TRUE;
	}
	return bResult;
}

BOOL CGoods::Serialize(DBWriteSet& setWriteDB, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CShape::CodeToDataBlock( setWriteDB, b==FALSE?false:true );

	setWriteDB.AddToByteArray( m_dwBasePropertiesIndex );
	setWriteDB.AddToByteArray( m_dwAmount );
	setWriteDB.AddToByteArray( m_dwPrice );
	setWriteDB.AddToByteArray( m_dwBuyPrice);
	setWriteDB.AddToByteArray( (DWORD)m_GoodsBaseType);
	//רҵ����������
	setWriteDB.AddToByteArray( const_cast<CHAR*>( m_strMakerName.c_str()));		

	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(!pBaseProperty)
		return FALSE;
	DWORD dwAddonNum=0;
	DWORD dwPropertyId=0;
	vector<BYTE> vecAddonData;
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{		
		if(pBaseProperty->IsHasAddonPropertie(m_vAddonProperties[i].gapType))	
		{			
			dwPropertyId=m_vAddonProperties[i].gapType;
			if(CGoodsFactory::s_GoodsAttrDBSetup[dwPropertyId][0]==1)
			{
				dwAddonNum++;
				m_vAddonProperties[i].Serialize(&vecAddonData);
			}
		}
		
	}
	setWriteDB.AddToByteArray(dwAddonNum);
	if(dwAddonNum>0)
	{
		setWriteDB.AddToByteArray(&vecAddonData[0],vecAddonData.size());
	}
	

	setWriteDB.AddToByteArray( static_cast<DWORD>( m_vSuitProperties.size() ) );
	for( size_t i = 0; i < m_vSuitProperties.size(); i ++ )
	{
		m_vSuitProperties[i].Serialize( setWriteDB );
	}
	


	EnchaseArrange();
	//enchase data	
	
	setWriteDB.AddToByteArray(m_dwMaxHoleNum);
	//���������
	setWriteDB.AddToByteArray( GetActivedHoleNum());
	setWriteDB.AddToByteArray( m_dwActivedKitCardId);
	for(int i=0;i<m_dwMaxHoleNum;i++)
	{
		//״̬
		//��������������Ƕ������Ϊ����
		setWriteDB.AddToByteArray(BYTE(1));
		
		//CARD ID
		CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
		if(pCard)
		{
			setWriteDB.AddToByteArray(pCard->GetBasePropertiesIndex());
			pCard->Serialize(setWriteDB);
		}
		else
		{
			setWriteDB.AddToByteArray(DWORD(0));
		}
	}
	return TRUE;
}
BOOL CGoods::Unserialize(DBReadSet& setReadDB, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	Release();
	CHAR szString[256];
	CShape::DecodeFromDataBlock( setReadDB, b==FALSE?false:true );

	m_dwBasePropertiesIndex	= setReadDB.GetDwordFromByteArray(  );
	CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(pProperty)
	{
		m_dwSilverPrice=pProperty->GetSilverPrice();
	}
	m_dwAmount				= setReadDB.GetDwordFromByteArray(  );
	m_dwPrice				= setReadDB.GetDwordFromByteArray(  );
	m_dwBuyPrice=setReadDB.GetDwordFromByteArray(  );
	m_GoodsBaseType=static_cast<eGoodsBaseType>(setReadDB.GetDwordFromByteArray());

	//##��ȡ��Ʒ����
	ZeroMemory( szString, 256 );
	setReadDB.GetStringFromByteArray( szString ,256);
	m_strMakerName			= szString;


	m_vAddonProperties.clear();
	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(!pBaseProperty)
		return FALSE;	

	CGoodsFactory::CreateAddonProperties(this);

	//##��ȡ��Ʒ��չ����
	DWORD dwNumProperties	= setReadDB.GetDwordFromByteArray(  );
	for( DWORD i = 0; i < dwNumProperties; i ++ )
	{
		CGoods::tagAddonProperty apAddonProperty;
		apAddonProperty.Unserialize( setReadDB );
		if(pBaseProperty->IsHasAddonPropertie(apAddonProperty.gapType))
		{			
			for(int i=0;i<m_vAddonProperties.size();i++)
			{
				if(m_vAddonProperties[i].gapType==apAddonProperty.gapType)
				{
					m_vAddonProperties[i].lValues[0]=apAddonProperty.lValues[0];
					m_vAddonProperties[i].lValues[1]=apAddonProperty.lValues[1];
				}
			}			
		}		
	}


	dwNumProperties	= setReadDB.GetDwordFromByteArray(  );
	for( DWORD i = 0; i < dwNumProperties; i ++ )
	{
		CGoodsBaseProperties::tagSuitProperty apAddonProperty;
		apAddonProperty.Unserialize( setReadDB );
		m_vSuitProperties.push_back( apAddonProperty );
	}
	if(m_vSuitProperties.size()==0)
	{
		CGoodsFactory::CreateSuitProperties(this);
	}
	
	InitEnchaseHole();

	DWORD dwMaxHoleNum=setReadDB.GetDwordFromByteArray();
	DWORD dwActivedHoleNum=setReadDB.GetDwordFromByteArray(  );
	m_dwActivedKitCardId=setReadDB.GetDwordFromByteArray(  );
	for(int i=0;i<dwMaxHoleNum;i++)
	{		
		BYTE btState=setReadDB.GetByteFromByteArray();
		
		//��������������Ƕ������Ϊ����
		m_pEnchaseHoleArray[i]->SetState(TRUE);
		
		//CARD ID
		DWORD dwGoodsId=setReadDB.GetDwordFromByteArray( );		

		if(dwGoodsId>0)
		{
			CGoods* pGoods = CGoodsFactory::UnserializeGoods( setReadDB ,41);
			if( i< m_dwMaxHoleNum)
				m_pEnchaseHoleArray[i]->SetCard(pGoods);
			else
			{
				char debugStr[128]="";
				sprintf_s(debugStr,"������Ƕ�쳣i=%d,m_dwMaxHoleNum=%d,dwMaxHoleNum=%d",i,m_dwMaxHoleNum,dwMaxHoleNum);
				PutDebugString(debugStr);
				CGoodsFactory::GarbageCollect(&pGoods,41);
			}
		}
		else
		{
			
		}
	}
	return TRUE;
}

BOOL CGoods::SerializeForRgnDisplay(DBWriteSet& setWriteDB,BOOL b)
{
	BOOL bResult=FALSE;
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
		GetBasePropertiesIndex() );
	if( pProperties )
	{
		setWriteDB.AddToByteArray( GetBasePropertiesIndex() );		
		setWriteDB.AddToByteArray( GetAmount() );
		setWriteDB.AddToByteArray(GetPosX());
		setWriteDB.AddToByteArray(GetPosY());
		long lLev=GetAddonPropertyValues(GAP_WEAPON_LEVEL,1);
		setWriteDB.AddToByteArray(lLev);
		bResult=TRUE;
	}
	return bResult;
}

BOOL CGoods::SerializeForOldClient(DBWriteSet& setWriteDB, BOOL b)
{
	BOOL bResult = FALSE;
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
		GetBasePropertiesIndex() );
	if( pProperties )
	{
		setWriteDB.AddToByteArray( GetBasePropertiesIndex() );
		setWriteDB.AddToByteArray( GetExID() );
		setWriteDB.AddToByteArray( GetAmount() );
		setWriteDB.AddToByteArray( const_cast<CHAR*>( GetMakerName() ) );		
		setWriteDB.AddToByteArray( GetPrice() );	
		setWriteDB.AddToByteArray( GetSilverPrice());
		setWriteDB.AddToByteArray( m_dwBuyPrice);
		setWriteDB.AddToByteArray( (DWORD)m_GoodsBaseType);		

		CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
		if(!pBaseProperty)
			return FALSE;
		LONG lAddonNum=0;
		DWORD dwPropertyId=0;

		vector<BYTE> vecAddonData;
		for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
		{		
			if(pBaseProperty->IsHasAddonPropertie(m_vAddonProperties[i].gapType))	
			{
				dwPropertyId=m_vAddonProperties[i].gapType;				
				lAddonNum++;
				_AddToByteArray( &vecAddonData, static_cast<WORD>(m_vAddonProperties[i].gapType) );
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[0]);
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[1]);
			}
			else
			{
				lAddonNum++;
				_AddToByteArray( &vecAddonData, static_cast<WORD>(m_vAddonProperties[i].gapType) );
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[0]);
				_AddToByteArray( &vecAddonData, m_vAddonProperties[i].lValues[1]);
			}
		}
		setWriteDB.AddToByteArray(lAddonNum);
		if(lAddonNum>0)
		{
			setWriteDB.AddToByteArray( &vecAddonData[0],vecAddonData.size());
		}
		/////////////////
		
		setWriteDB.AddToByteArray( GetTileX() );
		setWriteDB.AddToByteArray( GetTileY() );

		EnchaseArrange();
		//enchase data	
		DWORD dwMaxHoleNum=GetMaxEnchaseHoleNum();
		if(dwMaxHoleNum>0)
		{
			//������
			setWriteDB.AddToByteArray( dwMaxHoleNum);
			//���������
			//��������Ĭ�����п׶�����������ɾ����������
			
			for(int i=0;i<dwMaxHoleNum;i++)
			{		
				//CARD ID
				CGoods* pCard=m_pEnchaseHoleArray[i]->GetCard();
				if(pCard)
				{
					setWriteDB.AddToByteArray( pCard->GetBasePropertiesIndex());
					pCard->SerializeForOldClient(setWriteDB);
				}
				else
				{
					setWriteDB.AddToByteArray(DWORD(0));
				}
			}
		}

		bResult = TRUE;
	}
	return bResult;
}

VOID CGoods::EDCalculate()
{
	//����ED
	BOOL bAtkEdAttr=HasAddonProperty(GAP_ATK_ED);
	BOOL bCurAtkEdAttr=HasAddonProperty(GAP_CUR_ATK_ED);
	if(bAtkEdAttr && bCurAtkEdAttr)
	{
		LONG lAtkEdVal1=GetAddonPropertyValues(GAP_ATK_ED,1);
		LONG lAtkEdVal2=GetAddonPropertyValues(GAP_ATK_ED,2);
		if(lAtkEdVal1>lAtkEdVal2 || (lAtkEdVal2-lAtkEdVal1)<0)
			return;
		int ranVal=random(lAtkEdVal2 - lAtkEdVal1+1);		
		//������������İٷֱ�
		ranVal+=lAtkEdVal1;
		//���ε�ǰED�����ٷֱ�
		ChangeAttribute(GAP_CUR_ATK_ED,ranVal,1,true);
	}

	//����ED
	BOOL bDefEdAttr=HasAddonProperty(GAP_DEF_ED);
	BOOL bCurDefEdAttr=HasAddonProperty(GAP_CUR_DEF_ED);
	if(bDefEdAttr && bCurDefEdAttr)
	{
		LONG lDefEdVal1=GetAddonPropertyValues(GAP_DEF_ED,1);
		LONG lDefEdVal2=GetAddonPropertyValues(GAP_DEF_ED,2);
		if(lDefEdVal1>lDefEdVal2 || (lDefEdVal2-lDefEdVal1)<0)
			return;
		int ranVal=random(lDefEdVal2 - lDefEdVal1+1);		
		//������������İٷֱ�
		ranVal+=lDefEdVal1;
		//���µ�ǰED�����ٷֱ�
		ChangeAttribute(GAP_CUR_DEF_ED,ranVal,1, true);
	}

}

void CGoods::CalculatePKCP()
{
	// ȷ�������Ƿ����
	BOOL bHasAddProper = HasAddonProperty( GAP_ADD_MAX_PKCP );
	BOOL bHasAddRangeProper = HasAddonProperty( GAP_ADD_MAX_PKCPRANGE );
	if( bHasAddProper && bHasAddRangeProper )
	{
		LONG lAddMin = GetAddonPropertyValues( GAP_ADD_MAX_PKCPRANGE, 1 );
		LONG lAddMax = GetAddonPropertyValues( GAP_ADD_MAX_PKCPRANGE, 2 );
		if( lAddMax >= lAddMin )
		{
			LONG val = random( lAddMax - lAddMin + 1 ) + lAddMin;
			ChangeAttribute( GAP_ADD_MAX_PKCP, val, 1, true );
		}
	}

	// ����
	BOOL bHasTimesProper = HasAddonProperty( GAP_TIMES_MAX_PKCP );
	BOOL bHasTimesRangeProper = HasAddonProperty( GAP_TIMES_MAX_PKCPRANGE );
	if( bHasTimesProper && bHasTimesRangeProper )
	{
		LONG lTimesMin = GetAddonPropertyValues( GAP_TIMES_MAX_PKCPRANGE, 1 );
		LONG lTimesMax = GetAddonPropertyValues( GAP_TIMES_MAX_PKCPRANGE, 2 );
		if( lTimesMax >= lTimesMin )
		{
			LONG val = random( lTimesMax - lTimesMin + 1 ) + lTimesMin;
			ChangeAttribute( GAP_TIMES_MAX_PKCP, val, 1, true );
		}
	}
}

//���˸���
BOOL CGoods::AddonPropertyUpdate(GOODS_ADDON_PROPERTIES gapProperty,GOODS_ADDON_PROPERTIES gapRange)
{
	BOOL bPropertyExist=HasAddonProperty(gapProperty);
	BOOL bRangeExist=HasAddonProperty(gapRange);
	if(bPropertyExist && bRangeExist)
	{
		
		LONG lRange1=GetAddonPropertyValues(gapRange,1);
		LONG lRange2=GetAddonPropertyValues(gapRange,2);
		if(lRange1>lRange2)
			return FALSE;
		int ranVal=random(lRange2-lRange1+1);
		ranVal+=lRange1;
		ChangeAttribute(gapProperty,ranVal,1,true);
		return TRUE;
	}
	return FALSE;
}

BOOL CGoods::ImmunityUpdate()
{
	return AddonPropertyUpdate(GAP_EXCUSE_HURT,GAP_EXCUSE_HURT_RANGE);	
}

//��͸����
BOOL CGoods::PierceUpdate()
{
	return AddonPropertyUpdate(GAP_PENETRATE,GAP_PENETRATE_RANGE);	
}

//���˸���
BOOL CGoods::LuckyUpdate()
{
	return AddonPropertyUpdate(GAP_LUCKY,GAP_LUCKY_RANGE);
}

//��������
BOOL CGoods::ConstellationUpdate()
{
	BOOL bExist=HasAddonProperty(GAP_CONSTELLATION);
	if(bExist)
	{
		LONG lPropertyVal=GetAddonPropertyValues(GAP_CONSTELLATION,1);
		if(lPropertyVal!=0)
			return FALSE;
		int nRanVal=random(12);
		nRanVal++;
		ChangeAttribute(GAP_CONSTELLATION,nRanVal,1,true);
		return TRUE;
	}
	return FALSE;
}

//hp����׷�ӷ�Χ
BOOL CGoods::AddonHpUpdate()
{
	return AddonPropertyUpdate(GAP_RANDOM_ADDON_MAXHP,GAP_ADDON_HP_RANGE);
}

VOID CGoods::AtkEDUpdate()
{
	//��ǰ����ED
	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(!pBaseProperty)
		return;
	long lCurAtkED=GetAddonPropertyValues(GAP_CUR_ATK_ED,1);

	if(lCurAtkED>=0)
	{
		float fRate=(float)lCurAtkED/100.0f;
		//������С����		
		long lMinAtk=pBaseProperty->GetAddonPropertyValue(GAP_MIN_ATTACK,1);
		//������󹥻�
		long lMaxAtk=pBaseProperty->GetAddonPropertyValue(GAP_MAX_ATTACK,1);
		//����ħ������
		long lMAtk=pBaseProperty->GetAddonPropertyValue(GAP_FUMO_PROPERTY,1);
		float fMinAtkChg=lMinAtk*fRate;
		float fMaxAtkChg=lMaxAtk*fRate;
		float fMAtkChg=lMAtk*fRate;
		ChangeAttribute(GAP_MIN_ATTACK,0.5f+lMinAtk+fMinAtkChg,1, true);
		ChangeAttribute(GAP_MAX_ATTACK,0.5f+lMaxAtk+fMaxAtkChg,1, true);
		ChangeAttribute(GAP_FUMO_PROPERTY,0.5f+lMAtk+fMAtkChg,1, true);
	}
}

VOID CGoods::DefEDUpdate()
{
	//��ǰ����ED
	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(!pBaseProperty)
		return;
	long lCurDefED=GetAddonPropertyValues(GAP_CUR_DEF_ED,1);
	if(lCurDefED>=0)
	{
		float fRate=(float)lCurDefED/100.0f;
		//��������
		long lDef=pBaseProperty->GetAddonPropertyValue(GAP_NOR_DEF,1);
		//����ħ��
		long lMDef=pBaseProperty->GetAddonPropertyValue(GAP_MAGIC_DEF,1);
		
		float fDefChg=lDef*fRate;
		float fMDefChg=lMDef*fRate;

		ChangeAttribute(GAP_NOR_DEF,0.5f+lDef+fDefChg,1, true);
		ChangeAttribute(GAP_MAGIC_DEF,0.5f+lMDef+fMDefChg,1, true);
	}	
}

VOID CGoods::MountBind(const CGUID& playerID)
{	
	if(!GoodsExManage::GetInstance().IsBind(this))
	{
		LONG lBindType = GetAddonPropertyValues(GAP_BIND, 1);

		if(eBT_DressAndCantUntie == lBindType)//! װ�����󶨣��Ҳ��ܽ��
		{
			GoodsExManage::GetInstance().SetGoodsBindProperty_System(this,eBT_CantUntie,playerID);
		}
		else if(eBT_DressAndBind == lBindType)//! װ�����󶨣����Խ��
		{
			GoodsExManage::GetInstance().SetGoodsBindProperty_System(this,eBT_Bind,playerID);
		}
	}	
}

BOOL CGoods::DelAddonPropertyBaseValues(GOODS_ADDON_PROPERTIES gapType)
{
	vector<CGoods::tagAddonProperty>::iterator proIter=m_vAddonProperties.begin();
	for(; proIter!=m_vAddonProperties.end(); proIter ++ )
	{
		if( proIter->gapType== gapType )
		{
			break;
		}
	}
	if(proIter!=m_vAddonProperties.end())
	{
		m_vAddonProperties.erase(proIter);
		return TRUE;
	}
	return FALSE;
}

BOOL CGoods::DelAddonPropertyNotInBase(VOID)
{
	CGoodsBaseProperties* pBaseProperty= CGoodsFactory::QueryGoodsBaseProperties(m_dwBasePropertiesIndex);
	if(!pBaseProperty)
		return FALSE;
	vector<CGoods::tagAddonProperty>::iterator proIter=m_vAddonProperties.begin();
	for(; proIter!=m_vAddonProperties.end();)
	{
		if(!pBaseProperty->IsHasAddonPropertie(proIter->gapType))
		{
			//������Բ鲻��
			proIter=m_vAddonProperties.erase(proIter);
		}
		else
		{
			proIter ++;
		}
	}
	
	return TRUE;
}
const CHAR* CGoods::ToString()
{
	// TODO: Add your specialized code here.
	return (const CHAR*)0;
}

VOID CGoods::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	EnchaseRelease();
	m_dwBasePropertiesIndex	= 0;
	m_dwAmount				= 0;
	for( size_t i = 0; i < m_vAddonProperties.size(); i ++ )
	{
		m_vAddonProperties[i].Clear();
	}
	m_vAddonProperties.clear();
}

VOID CGoods::SetBasePropertiesIndex(DWORD dwID)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_dwBasePropertiesIndex	= dwID;
}

CGoods::tagAddonProperty::tagAddonProperty()
{
	// TODO: Add your specialized code here.
	gapType					= GAP_UNKNOW;	
	ZeroMemory(lValues,2*sizeof(long));
}

CGoods::tagAddonProperty::~tagAddonProperty()
{
	// TODO: Add your specialized code here.
	Clear();
}

VOID CGoods::tagAddonProperty::Clear()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	gapType					= GAP_UNKNOW;
	ZeroMemory(lValues,2*sizeof(long));
}

BOOL CGoods::tagAddonProperty::Serialize(vector<BYTE>* pStream, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	_AddToByteArray( pStream, static_cast<DWORD>(gapType) );
	_AddToByteArray(pStream,lValues[0]);
	_AddToByteArray(pStream,lValues[1]);	
	return TRUE;
}

BOOL CGoods::tagAddonProperty::Unserialize(BYTE* pStream, LONG& lOffset, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	gapType					= static_cast<GOODS_ADDON_PROPERTIES>( 
		_GetDwordFromByteArray(pStream, lOffset) );	
	lValues[0]=_GetLongFromByteArray(pStream,lOffset);
	lValues[1]=_GetLongFromByteArray(pStream,lOffset);
	return TRUE;
}

BOOL CGoods::tagAddonProperty::Serialize(DBWriteSet& setWriteDB, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	setWriteDB.AddToByteArray( static_cast<DWORD>(gapType) );
	setWriteDB.AddToByteArray(lValues[0]);
	setWriteDB.AddToByteArray(lValues[1]);
	return TRUE;
}
BOOL CGoods::tagAddonProperty::Unserialize(DBReadSet& setReadDB, BOOL b)
{
	gapType					= static_cast<GOODS_ADDON_PROPERTIES>( 
		setReadDB.GetDwordFromByteArray() );
	lValues[0]=setReadDB.GetLongFromByteArray();
	lValues[1]=setReadDB.GetLongFromByteArray();
	return TRUE;
}

VOID CGoods::SetPrice(DWORD dwPrice)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	m_dwPrice = dwPrice;
}

//##ModelId=41B809CB0298
DWORD CGoods::GetPrice()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return m_dwPrice;
}

DWORD CGoods::GetTotalPrice()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return m_dwPrice * m_dwAmount;
}


VOID CGoods::SetGoodsName(const CHAR* strGoodsName)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CShape::SetName( strGoodsName );
}

const CHAR* CGoods::GetGoodsName()
{
	// TODO: Add your specialized code here.
	return CShape::GetName();
}

void CGoods::UpdateScriptAddonDesc( const CGUID &playerID )
{
	CMessage msg( MSG_S2C_GOODS_UPDATE_SCRIPTDESC );
	msg.Add( GetExID() );
	msg.Add( GetScriptAddonDesc() );
	msg.SendToPlayer( playerID );
}

long CGoods::DecGuardianEnergy( unsigned long lTimes )
{
	if( !IsGuardian() )
	{
		return -1;
	}
	unsigned long lDec = lTimes * GetAddonPropertyValues( GAP_GUARDIAN_ENERGY_DEC, 1 );
	long lRet = GetAddonPropertyValues( GAP_GUARDIAN_ENERGY, 1 ) - lDec; 
	ChangeAttribute( GAP_GUARDIAN_ENERGY, lRet < 0 ? 0 : lRet, 1, true );

	return lRet < 0 ? 0 : lRet;
}

long CGoods::GetGuardianEnergy() 
{
	if( !IsGuardian() )
	{
		return -1;
	}
	return GetAddonPropertyValues( GAP_GUARDIAN_ENERGY, 1 ); 
}

bool CGoods::IsGuardian() 
{
	return GetGoodsBaseType() == GT_GUARDIAN &&
		HasAddonProperty( GAP_GUARDIAN_ENERGY );
}
