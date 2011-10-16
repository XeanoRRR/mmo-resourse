#include "StdAfx.h"
#include "IncrementShopManager.h"
#include "../DBEntity/EntityGroup.h"
#include "../DBEntity/EntityManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIncrementShopManager* CIncrementShopManager::m_pInstance=NULL;

CIncrementShopManager::CIncrementShopManager()
{

}

CIncrementShopManager::~CIncrementShopManager()
{
	
}

void CIncrementShopManager::Init()
{
	assert(!m_pInstance);
	if(!m_pInstance)
		m_pInstance=new CIncrementShopManager();
}

void CIncrementShopManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

CIncrementShopManager& CIncrementShopManager::GetSingleton()
{
	assert(m_pInstance);
	return *m_pInstance;
}

CIncrementShopManager* CIncrementShopManager::GetSingletonPtr()
{
	assert(m_pInstance);
	return m_pInstance;
}

//��ӹ����¼
void CIncrementShopManager::AddLimitGoodsRecord(DWORD dwGoodsId,DWORD dwGoodsNum)
{
	map<DWORD,DWORD>::iterator recordIt=m_mapLimitGoodsMap.find(dwGoodsId);
	if(recordIt==m_mapLimitGoodsMap.end())
		m_mapLimitGoodsMap[dwGoodsId]=dwGoodsNum;
	else
		recordIt->second+=dwGoodsNum;
}

//��ȡĳID��Ʒ�ѱ�������
DWORD CIncrementShopManager::GetLimitGoodsBuyNum(DWORD dwGoodsId)
{
	DWORD dwNum=0;
	map<DWORD,DWORD>::iterator recordIt=m_mapLimitGoodsMap.find(dwGoodsId);
	if(recordIt!=m_mapLimitGoodsMap.end())
		dwNum=recordIt->second;
	return dwNum;
}

//�Ƿ��ܹ���ָ���������޹���Ʒ
BOOL CIncrementShopManager::CanBuyGoods(DWORD dwGoodsId,DWORD dwGoodsNum)
{
	map<DWORD,DWORD>::iterator recordIt=m_mapLimitGoodsMap.find(dwGoodsId);
	if(recordIt==m_mapLimitGoodsMap.end())
		return TRUE;
	return FALSE;
}

void CIncrementShopManager::DecodeFromEntityGroup( long ret, CEntityGroup *entity )
{
	if( ret != S_OK || entity == NULL )
	{
		assert(0);
		return;
	}

	typedef map<CGUID, CBaseEntity*> EntityGroupTable;

	m_mapLimitGoodsMap.clear();
	CEntityGroup *pVarGroup = entity;
	EntityGroupTable &entity_table = pVarGroup->GetEntityGroupMap();
	// ����ÿһ������(���ݿ���һ��)
	for( EntityGroupTable::iterator it = entity_table.begin(); it != entity_table.end(); ++ it )
	{
		CBaseEntity* pEntity = it->second;
		if(pEntity)
		{
			// trade spot id
			long id = static_cast<DWORD>(pEntity->GetLongAttr( "goods_id"));
			long num= static_cast<DWORD>(pEntity->GetLongAttr("goods_num"));
			m_mapLimitGoodsMap[id]=num;
		}
	}// for
	//SEND TO GAMESERVER
}

bool CIncrementShopManager::AddToByteArray( vector<BYTE>* pByteArray )
{
	long lNum = (long)m_mapLimitGoodsMap.size();
	_AddToByteArray( pByteArray, lNum );
	map<DWORD, DWORD>::iterator it = m_mapLimitGoodsMap.begin();
	for ( ; it != m_mapLimitGoodsMap.end(); ++it )
	{
		_AddToByteArray( pByteArray, it->first );
		_AddToByteArray( pByteArray, it->second );
	}
	return true;
}