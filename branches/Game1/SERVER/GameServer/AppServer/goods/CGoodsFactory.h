// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CGOODSFACTORY_41A59A7103B9_INCLUDED
#define _INC_CGOODSFACTORY_41A59A7103B9_INCLUDED
#include "CGoodsBaseProperties.h"

class CGoods;
class CPlayer;

class CGoodsFactory 
{
public:	
	static VOID Release();
	
	CGoodsFactory();
	
	~CGoodsFactory();

	//����ID��ȡ��Ʒ��������
	static CGoodsBaseProperties* QueryGoodsBaseProperties(DWORD dwID);

	static VOID RunScriptOnGet(DWORD dwGoodsId,CPlayer* pPlayer);	

	//����ԭʼ����ȡ��Ʒ��������
	static CGoodsBaseProperties* QueryGoodsBasePropertiesByOriginalName(const CHAR* pName);

	//�������ƻ�ȡ��Ʒ��������
	static CGoodsBaseProperties* QueryGoodsBasePropertiesByName(const CHAR* pName);

    //����ID��ȡ��Ʒ��λ
    static CGoodsBaseProperties::EQUIP_PLACE QueryGoodsEuipPlace(DWORD dwID);

    //����ID��ȡ��Ʒ����
    static CGoodsBaseProperties::GOODS_TYPE QueryGoodsType(DWORD dwID);

    //����ID��ȡ��Ʒ�۸�
    static DWORD QueryGoodsPrice(DWORD dwID);

    //����ID��ȡ��Ʒ����
    static const CHAR* QueryGoodsDescribe(DWORD dwID);

	//����ID��ȡ��Ʒԭʼ��
	static const CHAR* QueryGoodsOriginalName(DWORD dwID);

	//����ID��ȡ��Ʒ����
	static const CHAR* QueryGoodsName(DWORD dwID);

	static const CHAR* QueryGoodsName(const CHAR* pOriginName);

	//������Ʒ����ȡ��Ʒԭʼ��
	static DWORD QueryGoodsIDByOriginalName(const CHAR* pName);

	//������Ʒ����ȡ��ƷID���
	static DWORD QueryGoodsIDByName(const CHAR* pName);	

	//���л���ByteArray
	static BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);

	//ͨ��ByteArray��ԭ
	static BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

	//���л���DataBlock
	static BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);

	//ͨ��DataBlock��ԭ
	static BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);

	//ͨ���ƶ���Ŵ�����Ʒ
	static CGoods* CreateGoods(DWORD dwID,long lType);

	//���������Ʒ	
	static VOID CreateGoods(DWORD dwID, DWORD dwAmount, vector<CGoods*>& vOut,long lType);

	//������Ʒ�ĸ�������
	static VOID CreateAddonProperties(CGoods* pGoods);
	//���²�����Ʒ�ĸ�������,������Ʒ�ĵȼ���
	static VOID ReCreateAddonProperties(CGoods* pGoods);

	//������Ʒ����װ����
	static VOID CreateSuitProperties(CGoods* pGoods);


	//ͨ��ByteArray��ԭ��Ʒ
	static CGoods* UnserializeGoods(BYTE* pStream, LONG& lOffset,long lType);

	//ͨ��ByteArray��ԭ��Ʒ
	static CGoods* UnserializeGoods(DBReadSet& setReadDB,long lType);

	//������Ʒ�����ڴ�
	static BOOL GarbageCollect(CGoods** ppGoods,long lFlag, BOOL bCollectMedal = FALSE);

	//��á���Ǯ������Ʒ���
	static DWORD GetGoldCoinIndex();

	static const char* GetGoldOrginName();

	static const char* GetGoldName();

	static DWORD GetSilverIndex();
	static const char* GetSilverOrginName();
	static const char* GetSilverName();

	//��á���ʯ������Ʒ���
	static DWORD GetGemIndex();

	//��á�Ԫ��������Ʒ���
	static DWORD GetYuanBaoIndex();
	

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
	static DWORD QueryGoodsMaxStackNumber( DWORD dwGoodsBasePropertiesID );
	static VOID InitAddonNameMap();
	static DWORD GetAddonByName(string str);

public:
	static BYTE s_GoodsAttrDBSetup[512][2];


private:	
	static map<string, DWORD> s_mOriginalNameIndex;
	
	static map<string, DWORD> s_mNameIndex;
	
	static map<DWORD,CGoodsBaseProperties*> s_mGoodsBaseProperties;

	static map<string,DWORD> s_mAddonNameMap;

	static map<string,string> s_mapSucceedOrder;
	static map<string,CGUID> s_mapPendingOrder;
	static list<string> s_listSuccedOrder;

	static set<CGoods*> s_mGcGoodsSet;

	static vector<CGoods*> s_vNoDelGoods;

	static map<long,set<CGoods*> >	s_UsingGoods;	

	static DWORD s_dwAddonPropertyNum;

	static BOOL Upgrade(CGoods* pGoods, GOODS_ADDON_PROPERTIES gapSource, GOODS_ADDON_PROPERTIES gapDest, BOOL bUpgrade = TRUE );

public:

	static VOID AddGoodsToGCSet(CGoods* pGoods);

	static VOID ReleaseGCGoodsSet();
	static VOID SetNoDelFlag(BOOL bVal){msNoDelFlag=bVal;}
	static BOOL GetNoDelFlag(){return msNoDelFlag;}

	typedef vector<CGoods*>& NoDelGoodsVec;
	static vector<CGoods*>& GetNoDelGoodsVec(){return s_vNoDelGoods;}
	static VOID ReleaseNoDelGoods();

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

#endif /* _INC_CGOODSFACTORY_41A59A7103B9_INCLUDED */
