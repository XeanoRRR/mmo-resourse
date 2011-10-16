// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CGOODS_41A2E64F0232_INCLUDED
#define _INC_CGOODS_41A2E64F0232_INCLUDED

//#include "BaseObject.h"
#include "CGoodsBaseProperties.h"
#include "..\Shape.h"
#include "..\script\Script.h"
#include "../../../../public/GoodsExDef.h"
#include "../../../setup/GlobeSetup.h"

class CGoodsFactory;
class CEnchaseHole;

//��Ʒ��ʵ����

class CGoods 
: public CShape
{
private:	
public:
	//��������
	struct tagAddonProperty
	{
		//�������Ե�����		
		GOODS_ADDON_PROPERTIES     				gapType;		
		//�����Դ���ֵ		
		long	lValues[2];
		
		tagAddonProperty();
		
		~tagAddonProperty();
		
		VOID Clear();
		
		BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
		
		BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

		BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);	
		BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);
	};


	struct tagEnchaseHoleProperty
	{
		BOOL bIsActived;//�Ƿ񱻿���
		CGoods* pCard;//���⿨Ƭ
	};
	
	

    // [070608 AHC]
    bool  SetAttribute(DWORD ID, DWORD value, int valuePos=0/*��pos��ֵ*/);
    DWORD GetAttribute(DWORD ID, int valuePos=0/*��pos��ֵ*/);
	bool  AddAttribute(DWORD ID, DWORD value, int valuePos=0/*��pos��ֵ*/);
    void  ChangeAttribute(DWORD ID, DWORD value, int valuePos=0/*��pos��ֵ*/, bool bOverload = false);
    long  UpdateAttribute(/*const CGUID&ownerID, */const CGUID& sendPlayerID);

    // ���������� ����ͻ��˷���ʱ���»�ȡ��������
    typedef struct tagDIRTYATTRDATA
    {
        DWORD ID;
        int valuePos;

        tagDIRTYATTRDATA()
            :   ID(0),
                valuePos(0){}
        tagDIRTYATTRDATA(DWORD id, int pos)
            :   ID(id),
            valuePos(pos){}

    }DIRTYATTRDATA;
    list<DIRTYATTRDATA>  m_listDirtyAttrData;

private:
	//��Ʒ����Ʒ�����������ݿ��ڵ��������	
	DWORD m_dwBasePropertiesIndex;
	
	DWORD m_dwAmount;
	
	DWORD m_dwPrice;
	DWORD m_dwSilverPrice;

	DWORD m_dwBuyPrice;

	eGoodsBaseType m_GoodsBaseType;

	//����������
	string m_strMakerName;
	
	vector<CGoods::tagAddonProperty> m_vAddonProperties;
	vector<CGoodsBaseProperties::tagSuitProperty> m_vSuitProperties;

	//tagSubpack m_Subpack[SUBPACK_NUM];	
	DWORD m_dwActivedKitCardId;
	DWORD m_dwMaxHoleNum;
	CEnchaseHole** m_pEnchaseHoleArray;


protected:	
	VOID SetBasePropertiesIndex(DWORD dwID);	
	
public:
	BOOL SetAddonPropertyBaseValues(GOODS_ADDON_PROPERTIES gapType, DWORD dwID, LONG lValue);	

	BOOL AddAddonPropertyBaseValues(GOODS_ADDON_PROPERTIES gapType, DWORD dwID, LONG lVal1,LONG lVal2=0,BOOL bEnable=TRUE,BOOL bIsImplicit=FALSE);
	

	BOOL DelAddonPropertyBaseValues(GOODS_ADDON_PROPERTIES gapType);
	BOOL DelAddonPropertyNotInBase(VOID);


	
	
	CGoods();
	
	
	
	~CGoods();
	
	DWORD GetBasePropertiesIndex();

	vector<tagAddonProperty>* GetAddonProperties(){return &m_vAddonProperties;}
	
	VOID SetAmount(DWORD dwAmount);
	
	DWORD GetAmount();
	VOID SetBuyPrice(DWORD dwPrice){m_dwBuyPrice=dwPrice;}
	DWORD GetBuyPrice(){return m_dwBuyPrice;}

	DWORD GetSilverPrice(){return m_dwSilverPrice;}
	VOID SetSilverPrice(DWORD dwPrice){m_dwSilverPrice=dwPrice;}

	//��ȡ���ѵ�����������ֻ�ܻ�ȡ���������á�
	DWORD GetMaxStackNumber();

	//! ��Ʒ�İ󶨶�������
	enum eBindFreeze
	{
		eBF_NoAll,
		eBF_OnlyBind,
		eBF_Freeze,
	};
	//! �õ���Ʒ�İ󶨶�������
	eBindFreeze GetBindFreezeProperties(void);

	//! �ж���Ʒ�ܷ���������������Ʒ�ȼ��������ܣ�����-1
	long CanUpgrade(void);

	//! �ж���Ʒ�ܷ�ħ������������Ʒħ���ȼ��������ܣ�����-1
	long CanPlugEvil(void);

	BOOL CanUpgraded();
	
	BOOL CanReparied();

	VOID SetGoodsBaseType(eGoodsBaseType type){m_GoodsBaseType=type;}
	eGoodsBaseType GetGoodsBaseType(){return m_GoodsBaseType;}

	CGoodsBaseProperties::EQUIP_TYPE QueryEquipType();
	eWeaponType GetWeaponType();
	eWeaponHandType GetWeaponHandType();
	eBusinessGoodsType GetBusinessGoodsType();
	

	//�Ƿ������Ƕ
	BOOL CanEnchase();
	//�ɴ������	
	LONG* GetEnchaseHoleData();

	DWORD GetMaxEnchaseHoleNum();
	DWORD GetActivedHoleNum();
	DWORD GetEnchasedCardNum();
	VOID GetEnchasedCard(vector<DWORD>& vecCards);
	CGoods* GetEnchasedCard(int nIndex);
	VOID UpdateEnchaseAttr();

	BOOL Enchase(CPlayer* pPlayer,CGoods* pGoods,int nIndex);
	BOOL RemoveCard(CPlayer* pPlayer,int nIndex,bool bReserve);
	DWORD GetEnchaseNumByGoodsId(DWORD dwId);
	DWORD GetEnchaseNumByCardGroupID(DWORD GroupID);
	VOID EnchaseAttrUpdate(CPlayer* pPlayer,CGoods* pCard);
	VOID EnchaseAttrRemove(CPlayer* pPlayer,CGoods* pCard);
	VOID SetActivedKitCardId(DWORD dwId){m_dwActivedKitCardId=dwId;}
	DWORD GetActivedKitCardId(){return m_dwActivedKitCardId;}


	//�Ƿ�����ص�
	BOOL CanStacked();

	BOOL IsSuitGoods();

	VOID RunScriptOnGet(CPlayer* pPlayer);

	VOID RunScriptOnLose(CPlayer* pPlayer);

	VOID RunScriptOnEquip(CPlayer* pPlayer);

	VOID RunScriptOnRemoveEquip(CPlayer* pPlayer);

	VOID OnSuitNumChanged(CPlayer* pPlayer,int new_num);

	VOID InvalidSuitProperties(CPlayer* pPlayer);

	VOID InitEnchaseHole();
	VOID EnchaseRelease();

	BOOL ActiveEnchaseHole(CPlayer* pPlayer,int nIndex);
	DWORD ActiveEnchaseHole(CPlayer* pPlayer);
	BOOL CloseEnchaseHole(CPlayer* pPlayer,int nIndex);
	DWORD CloseEnchaseHole();
	VOID EnchaseArrange();
	VOID ActiveHoles(int nNum);

	BOOL ActiveEnchaseHole(WORD wIndex);

	VOID AddRefine(long lVal);


	//����������ʽ�ġ������õĸ������ԣ�����û�б���ֹ��������
	//ʽ���Զ����ᱻ����	
	VOID GetValidAddonProperties(vector<GOODS_ADDON_PROPERTIES>& vOut);	
	
	//��ȡ������Ч�ĸ������ԣ�������ʽ����	
	VOID GetEnabledAddonProperties(vector<GOODS_ADDON_PROPERTIES>& vOut);
	
	VOID GetAddonPropertyValues(GOODS_ADDON_PROPERTIES gapType, vector<long>& vOut);
	
	LONG GetAddonPropertyValues(GOODS_ADDON_PROPERTIES gapType, DWORD dwID);

	

	bool HasAddonPropertyValues(GOODS_ADDON_PROPERTIES gapType);
	BOOL HasAddonProperty(GOODS_ADDON_PROPERTIES gapType);

	//������Ʒ���������������ǵ�����Ʒ��������	
	//DWORD GetWeight();
	
	VOID SetPrice(DWORD dwPrice);

	//������Ʒ�ĵ���	
	DWORD GetPrice();

	VOID EDCalculate();

	/// ���������Ʒ���ӵ�PK CPֵ
	void CalculatePKCP();

	VOID AtkEDUpdate();

	VOID DefEDUpdate();	

	BOOL AddonPropertyUpdate(GOODS_ADDON_PROPERTIES gapProperty,GOODS_ADDON_PROPERTIES gapRange);
	BOOL ImmunityUpdate();//���˸���
	BOOL PierceUpdate();//��͸����
	BOOL LuckyUpdate();//���˸���
	BOOL ConstellationUpdate();//��������
	BOOL AddonHpUpdate();//hp����׷�ӷ�Χ

	VOID MountBind(const CGUID& playerID);
	

	

	//��Ʒ���������� �ӿ�
	const char* GetMakerName(){return m_strMakerName.c_str();}
	bool SetMakerName(const char* pszMakerName){if(256 > strlen(pszMakerName)){ m_strMakerName=pszMakerName;return true;}return false;}

	/// ���ű����ӵĶ�����Ʒ��������ؽӿ�
	/// 9.7.2009 : ʹ�������������ַ����������������Ʒ���������������������ܳ�ͻ��
	const char *GetScriptAddonDesc() const { return m_strMakerName.c_str(); }
	bool SetScriptAddonDesc( const std::string &desc ) {if(256 > desc.size()){ m_strMakerName = desc;return true;}return false; }

	/// ���¡��ű��������������ͻ���
	void UpdateScriptAddonDesc( const CGUID &playerID );

	/// ���͡������ػ�ʯ������Ʒ�ġ�����������ֵ
	/// ���ؽ��ֵ�����û�и����ԣ�����-1.
	long DecGuardianEnergy( unsigned long lTimes = 1 );

	/// ��ȡ������������ֵ�����û�����ֵ������-1.
	long GetGuardianEnergy() ;

	/// ����Ƿ��ǡ������ػ�ʯ����Ʒ
	bool IsGuardian() ;

	//��ȡ��Ʒ���ܼ�	
	DWORD GetTotalPrice();
	
	VOID SetGoodsName(const CHAR* strGoodsName);
	
	const CHAR* GetGoodsName();		
	
	virtual VOID AI();
	
	virtual BOOL Clone(CBaseObject* pObject);
	
	virtual BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
	
	virtual BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

	BOOL SerializeForOldClient(vector<BYTE>* pStream, BOOL b = TRUE);

	virtual BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);	
	virtual BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);
	BOOL SerializeForOldClient(DBWriteSet& setWriteDB, BOOL b = TRUE);

	BOOL SerializeForRgnDisplay(DBWriteSet& setWriteDB,BOOL b=TRUE);
	
	virtual const CHAR* ToString();
	
	virtual VOID Release();

	
	friend class CGoodsFactory;
};

#endif /* _INC_CGOODS_41A2E64F0232_INCLUDED */
