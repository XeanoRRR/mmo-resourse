  
#include "StdAfx.h"
#include "MedalContainer.h"
#include "Goods\CGoodsFactory.h"
#include "goods/CGoods.h"



MedalContainer::MedalContainer(CPlayer *pOwner)
:m_pOwner(pOwner)
{
}
MedalContainer::~MedalContainer(void)
{
	m_pOwner = NULL;
	m_setHold.clear();
	m_setUsing.clear();
}



//! ������(ѫ��ID)���ѫ��
eMCOptReValue MedalContainer::AddMedal(LONG lGoodsBaseIndex)
{
	if(NULL == FindMedia(lGoodsBaseIndex))
		return eMCRV_MedalNotBeing;
	if(HasMedal(lGoodsBaseIndex))
		return eMCRV_RepeatAdd;
	m_setHold.insert(lGoodsBaseIndex);
	return eMCRV_Succeed;
}

//! ������(ѫ��ID)ɾ��ѫ��
eMCOptReValue MedalContainer::DelMedal(LONG lGoodsBaseIndex)
{
	if(!HasMedal(lGoodsBaseIndex))
		return eMCRV_AimCantFind;
	m_setHold.erase(lGoodsBaseIndex);
	return eMCRV_Succeed;
}

//! �ж��Ƿ���ָ������(ѫ��ID)�ĵ�ѫ��
BOOL MedalContainer::HasMedal(LONG lGoodsBaseIndex)
{
	return (m_setHold.end() != m_setHold.find(lGoodsBaseIndex));
}

//! �õ���ʹ�õ�ѫ������
LONG MedalContainer::GetMedalNum(void)
{
	return m_setHold.size();
}

//! ʹ��һ��ѫ�£�ͬʱֻ��ʹ�����MEDAL_MAX_USE_NUM��ѫ�£�
CGoods* MedalContainer::UseMedal(LONG lGoodsBaseIndex)
{
	if((!HasMedal(lGoodsBaseIndex)) || MEDAL_MAX_USE_NUM <= m_setUsing.size() || m_setUsing.end() != m_setUsing.find(lGoodsBaseIndex))
		return NULL;
	m_setUsing.insert(lGoodsBaseIndex);
	assert(NULL != FindMedia(lGoodsBaseIndex));
	return FindMedia(lGoodsBaseIndex);
}

//! �黹һ��ʹ�õ�ѫ��
eMCOptReValue MedalContainer::ReturnMedal(LONG lGoodsBaseIndex)
{
	if(m_setUsing.end() == m_setUsing.find(lGoodsBaseIndex))
		return eMCRV_AimCantFind;
	assert(HasMedal(lGoodsBaseIndex));
	m_setUsing.erase(lGoodsBaseIndex);
	return eMCRV_Succeed;
}

//! ��һ������ʹ�õ�ѫ�½�����һ��δ��ʹ�õ�ѫ��
CGoods* MedalContainer::SwapMedal(LONG lUsingIndex, LONG lNewIndex)
{
	if((!HasMedal(lNewIndex)) || m_setUsing.end() == m_setUsing.find(lUsingIndex) || m_setUsing.end() != m_setUsing.find(lNewIndex))
		return NULL;
	assert(HasMedal(lUsingIndex));
	m_setUsing.erase(lUsingIndex);
	m_setUsing.insert(lNewIndex);

	assert(NULL != FindMedia(lNewIndex));
	return FindMedia(lNewIndex);
}


//! ����
void MedalContainer::AddToByteArray(vector<BYTE> &vData)
{
	_AddToByteArray(&vData, (LONG)m_setHold.size());
	for (set<LONG>::iterator ite = m_setHold.begin(); ite != m_setHold.end(); ++ite)
	{
		_AddToByteArray(&vData, *ite);
	}
}

//! ����
void MedalContainer::Decode(BYTE *pData, LONG &lPos)
{
	m_setHold.clear();
	LONG lNum = _GetLongFromByteArray(pData, lPos);
	for (LONG i = 0; i < lNum; ++i)
	{
		m_setHold.insert(_GetLongFromByteArray(pData, lPos));
	}
}

//!						����
void MedalContainer::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray( (LONG)m_setHold.size());
	for (set<LONG>::iterator ite = m_setHold.begin(); ite != m_setHold.end(); ++ite)
	{
		setWriteDB.AddToByteArray(*ite);
	}
}
	//!						����
void MedalContainer::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	m_setHold.clear();
	LONG lNum = setReadDB.GetLongFromByteArray( );
	for (LONG i = 0; i < lNum; ++i)
	{
		m_setHold.insert(setReadDB.GetLongFromByteArray( ));
	}
}



#pragma region ��̬ȫ�ֳ�Ա


//! ��ʼ��ѫ��ϵͳ
BOOL MedalContainer::InitializeMedalSystem(void)
{
	CGoodsFactory::CreateGoodsListByType(GT_MEDAL, s_mapMedalByIndex);
	return TRUE;
}

//! �ͷ���Դ
void MedalContainer::Release(void)
{
	map<LONG, CGoods*>::iterator ite = s_mapMedalByIndex.begin();
	for (; ite != s_mapMedalByIndex.end(); ++ite)
	{
#ifdef _GOODSLOG1_
		if(ite->second)
		{
			char pszGoodsLog[1024]="";			
			char srcGoodsGUID[50]="";
			ite->second->GetExID().tostring(srcGoodsGUID);					
			_snprintf(pszGoodsLog,1024,"[��Ʒ����:MedalContainer::Release][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
				ite->second->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(ite->second->GetBasePropertiesIndex()),srcGoodsGUID,ite->second->GetAmount());
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
		}		
#endif
		CGoodsFactory::GarbageCollect(&(ite->second),10000,TRUE);
	}
	s_mapMedalByIndex.clear();
}

//! �ҵ�һ��ѫ��
CGoods* MedalContainer::FindMedia(LONG lGoodsBaseIndex)
{
	map<LONG, CGoods*>::iterator ite = s_mapMedalByIndex.find(lGoodsBaseIndex);
	if(s_mapMedalByIndex.end() != ite)
		return ite->second;
	return NULL;
}

//! ������Ϊkey��ѫ�¶����б�
map<LONG, CGoods*> MedalContainer::s_mapMedalByIndex;

#pragma endregion