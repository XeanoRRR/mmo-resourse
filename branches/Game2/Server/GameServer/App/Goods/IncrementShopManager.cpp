#include "StdAfx.h"
#include "IncrementShopManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

map<DWORD,DWORD> CIncrementShopManager::m_mapLimitGoodsMap;

CIncrementShopManager::CIncrementShopManager()
{

}

CIncrementShopManager::~CIncrementShopManager()
{
	
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
DWORD CIncrementShopManager::GetWorldLimitNumByGoodsID(DWORD dwGoodsId)
{
	DWORD dwNum=0;
	map<DWORD,DWORD>::iterator recordIt=m_mapLimitGoodsMap.find(dwGoodsId);
	if(recordIt!=m_mapLimitGoodsMap.end())
		dwNum=recordIt->second;
	return dwNum;
}

//��WS����
bool CIncrementShopManager::DecordFromByteArray( BYTE* pByte, long& pos )
{
	long lNum = _GetLongFromByteArray( pByte, pos );
	for ( int i = 0; i < lNum; ++i )
	{
		DWORD dwGoodsIdx = _GetDwordFromByteArray( pByte, pos );
		DWORD dwGoodsNum = _GetDwordFromByteArray( pByte, pos );
		m_mapLimitGoodsMap[dwGoodsIdx] = dwGoodsNum;
	}
	return true;
}