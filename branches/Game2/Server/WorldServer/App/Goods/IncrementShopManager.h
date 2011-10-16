#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

class CEntityGroup;
class CIncrementShopManager
{
private:
	static CIncrementShopManager* m_pInstance;

	map<DWORD,DWORD> m_mapLimitGoodsMap;
private:		
	CIncrementShopManager();	
	~CIncrementShopManager();
public:
	static void Init();
	static void Destroy();

	static CIncrementShopManager& GetSingleton();
	static CIncrementShopManager* GetSingletonPtr();

	map<DWORD,DWORD>& GetLimitGoodsMap(){return m_mapLimitGoodsMap;}

	//��ӹ����¼
	void AddLimitGoodsRecord(DWORD dwGoodsId,DWORD dwGoodsNum);
	//��ȡĳID��Ʒ�ѱ�������
	DWORD GetLimitGoodsBuyNum(DWORD dwGoodsId);
	//�Ƿ��ܹ���ָ���������޹���Ʒ
	BOOL CanBuyGoods(DWORD dwGoodsId,DWORD dwGoodsNum);
	//��������¼
	void ClearLimitGoodsRecord(){m_mapLimitGoodsMap.clear();}

	void DecodeFromEntityGroup( long ret, CEntityGroup *entity );
	
	bool AddToByteArray(vector<BYTE>* pByteArray);
};