#ifndef _INCREMENT_SHOP_MANAGER_H_
#define _INCREMENT_SHOP_MANAGER_H_

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

class CIncrementShopManager
{
private:
	static map<DWORD,DWORD> m_mapLimitGoodsMap;
private:		
	CIncrementShopManager();	
	~CIncrementShopManager();
public:
	//ȡ��������¼
	static map<DWORD,DWORD>& GetLimitGoodsMap() { return m_mapLimitGoodsMap; }
	//��ӹ����¼
	static void AddLimitGoodsRecord( DWORD dwGoodsId, DWORD dwGoodsNum );
	//��ȡĳID��Ʒ�ѱ�������
	static DWORD GetWorldLimitNumByGoodsID( DWORD dwGoodsId );
	//��������¼
	static void ClearLimitGoodsRecord() { m_mapLimitGoodsMap.clear(); }
	//��WS����
	static bool DecordFromByteArray(BYTE* pByte, long& pos);
};

#endif//_INCREMENT_SHOP_MANAGER_H_