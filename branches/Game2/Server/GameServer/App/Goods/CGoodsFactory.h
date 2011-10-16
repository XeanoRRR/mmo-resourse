/** 
*  @file   CGoodsFactory.h
*  @author
*
*  @brief  ��Ʒ������
*  @date   
*/

#pragma once

class CGoods;
class CPlayer;

class CGoodsFactory 
{
public:	
	static void Release();
	
	CGoodsFactory();
	
	~CGoodsFactory();

	static void RunScriptOnGet(DWORD dwGoodsId,CPlayer* pPlayer);	


	//ͨ���ƶ���Ŵ�����Ʒ
	static CGoods* CreateGoods(DWORD dwID,long lType);

	//���������Ʒ	
	static void CreateGoods(DWORD dwID, DWORD dwAmount, vector<CGoods*>& vOut,long lType);

	//������Ʒ�ĸ�������
	static void CreateAddonProperties(CGoods* pGoods);
	//���²�����Ʒ�ĸ�������,������Ʒ�ĵȼ���
	static void ReCreateAddonProperties(CGoods* pGoods);

	//������Ʒ����װ����
	static void CreateSuitProperties(CGoods* pGoods);


	//ͨ��ByteArray��ԭ��Ʒ
	static CGoods* UnserializeGoods(BYTE* pStream, LONG& lOffset,long lType);

	//ͨ��ByteArray��ԭ��Ʒ
	static CGoods* UnserializeGoods(DBReadSet& setReadDB,long lType);

	//������Ʒ�����ڴ�
	static BOOL GarbageCollect(CGoods** ppGoods,long lFlag, BOOL bCollectMedal = FALSE);

	//������Ʒ������۸�
	static DWORD CalculateRepairPrice(CGoods* pGoods);

	//������Ʒ�������۸�
	static DWORD CalculateUpgradePrice(CGoods* pGoods);

	//������Ʒ�ĳ��ۼ۸�
	static DWORD CalculateVendPrice(CGoods *pGoods);

	//������Ʒ�����ҳ��ۼ۸�
	static DWORD CalculateVendSilverPrice(CGoods *pGoods);
	

	//װ������
	static BOOL EquipmentWaste(CGoods* pGoods, BOOL bIsSpecialWaste = FALSE);

	//����װ��
	static BOOL RepairEquipment(CGoods* pGoods);

	//����װ��
	static BOOL UpgradeEquipment(CGoods* pGoods, LONG lTargetLevel);

	//! -----------------------------------2008-1-8 Add-----------------------------------


	//! �ı�װ���ȼ�����ԭ�еȼ������ϸı䣩
	//! ��С�ı䵽0�����ﵽ���õĵȼ�����
	static BOOL ChangeEquipmentLevel(CGoods* pEquipment, long lChangeDegree);

	//! ����װ���ȼ�
	//! ��С�ı䵽0�����ﵽ���õĵȼ�����
	static BOOL SetEquipmentLevel(CGoods* pEquipment, LONG lLevel);



	//! �õ�װ���Ƿ��ǹ�����
	static BOOL IsAttEquipment(CGoods* pEquipment);



	//! ����װ������
	static void UpdateEquipmentAttr(CGoods* pEquipment);

	static  eEquipLimitType QueryPlayerEquipLimitType(CPlayer* pPlayer);

	static eBusinessGoodsType GetBusinessGoodsType(DWORD dwGoodsId);


	//! -----------------------------------2008-1-8 End-----------------------------------

	//! ����ͬһ�����͵�������Ʒ��������һ������Ʒ����Ϊkey��map
	static void CreateGoodsListByType(eGoodsBaseType eType, map<LONG, CGoods*> &mapGoods);

	//��Ʒ����ֵ����������
	static BOOL ModifyGoodsAddonProperty(CGoods* pGoods, DWORD dwID, LONG lValue1, LONG lValue2 = 0, BOOL bIsImplicitAttribute = FALSE);
	//��ȡ��Ʒ��������
	//static DWORD QueryGoodsMaxStackNumber( DWORD dwGoodsBasePropertiesID );

private:	

	static map<string,string> s_mapSucceedOrder;
	static map<string,CGUID> s_mapPendingOrder;
	static list<string> s_listSuccedOrder;

	static set<CGoods*> s_mGcGoodsSet;

	static vector<CGoods*> s_vNoDelGoods;

	static map<long,set<CGoods*> >	s_UsingGoods;	

	static BOOL Upgrade(CGoods* pGoods, eGoodsAddonProperties gapSource, eGoodsAddonProperties gapDest, BOOL bUpgrade = TRUE );

public:

	static void AddGoodsToGCSet(CGoods* pGoods);

	static void ReleaseGCGoodsSet();
	static void SetNoDelFlag(BOOL bVal){msNoDelFlag=bVal;}
	static BOOL GetNoDelFlag(){return msNoDelFlag;}

	typedef vector<CGoods*>& NoDelGoodsVec;
	static vector<CGoods*>& GetNoDelGoodsVec(){return s_vNoDelGoods;}
	static void ReleaseNoDelGoods();

	static bool IsOrderExist(const char* szOrder);
	static void PushOrder(const char* szOrder,const char* szContent);
	static void PushPendingOrder(const char* szOrder,const CGUID& guid);
	static CGUID& GetGUIDByOrderId(const char* szOrder);
	static void ErasePendingOrder(const char* szOrder);

	//! �����е���Ʒ��Ϣ����
	////////////////////////////////////////////////////////////////////////////////
public:
	//!							���һ����Ʒ��Ϣ
	static		void			AddGoodsInfo(const CGUID &Key, const vector<BYTE> &vGoodsInfo);
	//!							ȡ��һ����Ʒ��Ϣ
	static		BOOL			GetGoodsInfo(const CGUID &Key, vector<BYTE> &vGoodsInfo);

	static		long			OutputUseGoodsInfo(const char *pszFileName);

private:
	//!							����
	static		void			ClearInfo(LONG lClearNum);


	//!									�������Ʒ��Ϣ
	static	map<CGUID, vector<BYTE>>	m_mapChatGoodsInfo;
	//!									�����ʱ���Ⱥ�˳��
	static	list<pair<CGUID, LONG>>		m_listSaveTimeOrder;

	static BOOL msNoDelFlag;

};