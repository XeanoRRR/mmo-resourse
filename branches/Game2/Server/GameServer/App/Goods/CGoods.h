/** 
*  @file   CGoods.h
*  @author
*
*  @brief  ��Ʒ��ʵ����
*  @date   
*/

#pragma once

#include "../Shape.h"
#include "../../../../Public/ServerPublic/Server/Common/GoodsBaseProperties.h"

class CGoodsFactory;
class CEnchaseHole;

class CGoods 
: public CShape
{
public:
	//��������
	struct tagAddonProperty
	{
		//�������Ե�����		
		eGoodsAddonProperties gapType;		
		//�����Դ���ֵ		
		long	lValues[2];
		
		tagAddonProperty();
		
		~tagAddonProperty();
		
		void Clear();
		
		bool Serialize(vector<uchar>* pStream, bool b = true);
		
		bool Unserialize(uchar* pStream, long& lOffset, bool b = true);

		bool Serialize(DBWriteSet& setWriteDB, bool b = true);	
		bool Unserialize(DBReadSet& setReadDB, bool b = true);
	};


	struct tagEnchaseHoleProperty
	{
		bool bIsActived;//�Ƿ񱻿���
		CGoods* pCard;//���⿨Ƭ
	};
	
	bool  SetAttribute(ulong ID, ulong value, int valuePos=0/*��pos��ֵ*/);
    ulong GetAttribute(ulong ID, int valuePos=0/*��pos��ֵ*/);
	bool  AddAttribute(ulong ID, ulong value, int valuePos=0/*��pos��ֵ*/);
    void  ChangeAttribute(ulong ID, ulong value, int valuePos=0/*��pos��ֵ*/, bool bOverload = false);
    long  UpdateAttribute(/*const CGUID&ownerID, */const CGUID& sendPlayerID);

    // ���������� ����ͻ��˷���ʱ���»�ȡ��������
    typedef struct tagDIRTYATTRDATA
    {
        ulong ID;
        int valuePos;

        tagDIRTYATTRDATA()
            :   ID(0),
                valuePos(0){}
        tagDIRTYATTRDATA(ulong id, int pos)
            :   ID(id),
            valuePos(pos){}

    }DIRTYATTRDATA;
    list<DIRTYATTRDATA>  m_listDirtyAttrData;


protected:	
	void SetBasePropertiesIndex(ulong dwID);	
	
public:
	bool SetAddonPropertyBaseValues(eGoodsAddonProperties gapType, ulong dwID, long lValue);	
	bool AddAddonPropertyBaseValues(eGoodsAddonProperties gapType, ulong dwID, long lVal1,long lVal2=0,bool bEnable=true,bool bIsImplicit=false);
	bool DelAddonPropertyBaseValues(eGoodsAddonProperties gapType);
	bool DelAddonPropertyNotInBase(void);
	
	CGoods();
	~CGoods();
	
	ulong GetBasePropertiesIndex();

	vector<tagAddonProperty>* GetAddonProperties(){return &m_vAddonProperties;}
	
	void SetAmount(ulong dwAmount);
	ulong GetAmount();
	void SetBuyPrice(ulong dwPrice){m_dwBuyPrice=dwPrice;}
	ulong GetBuyPrice(){return m_dwBuyPrice;}

	//������Ʒ�ĵ���	
	ulong GetPrice();
	ulong GetSilverPrice(){ return m_baseProperties->GetSilverPrice(); }

	/// ��ȡ��Ʒ��������
	CGoodsBaseProperties* GetBaseProperties(void) { return m_baseProperties; }
	/// ������Ʒ��������
	void SetBaseProperties(CGoodsBaseProperties* baseProperties) { m_baseProperties = baseProperties; }

	//��ȡ���ѵ�����������ֻ�ܻ�ȡ���������á�
	ulong GetMaxStackNumber();

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

	bool CanUpgraded();
	bool CanReparied();

	eGoodsBaseType GetGoodsBaseType(){ return m_baseProperties->GetGoodsBaseType(); }

	eEquipType QueryEquipType();
	eWeaponType GetWeaponType();
	eWeaponHandType GetWeaponHandType();
	eBusinessGoodsType GetBusinessGoodsType();

	/// ��ȡ��Ʒװ������
	eEquipType GetEquipType(void);
	/// ��ȡ��Ʒ��������
	eTrumpType GetTrumpType(void);
	

	//�Ƿ������Ƕ
	bool CanEnchase();
	//�ɴ������	
	long* GetEnchaseHoleData();

	ulong GetMaxEnchaseHoleNum();
	ulong GetActivedHoleNum();
	ulong GetEnchasedCardNum();
	void GetEnchasedCard(vector<ulong>& vecCards);
	CGoods* GetEnchasedCard(int nIndex);
	void UpdateEnchaseAttr();

	bool Enchase(CPlayer* pPlayer,CGoods* pGoods,int nIndex);
	bool RemoveCard(CPlayer* pPlayer,int nIndex,bool bReserve);
	ulong GetEnchaseNumByGoodsId(ulong dwId);
	void EnchaseAttrUpdate(CPlayer* pPlayer,CGoods* pCard);
	void EnchaseAttrRemove(CPlayer* pPlayer,CGoods* pCard);
	void SetActivedKitCardId(ulong dwId){m_dwActivedKitCardId=dwId;}
	ulong GetActivedKitCardId(){return m_dwActivedKitCardId;}


	//�Ƿ�����ص�
	bool CanStacked();
	bool IsSuitGoods();

	void RunScriptOnGet(CPlayer* pPlayer);
	void RunScriptOnLose(CPlayer* pPlayer);
	void RunScriptOnEquip(CPlayer* pPlayer);
	void RunScriptOnRemoveEquip(CPlayer* pPlayer);
	void OnSuitNumChanged(CPlayer* pPlayer,int new_num);
	void InvalidSuitProperties(CPlayer* pPlayer);

	void InitEnchaseHole();
	void EnchaseRelease();

	bool ActiveEnchaseHole(CPlayer* pPlayer,int nIndex);
	ulong ActiveEnchaseHole(CPlayer* pPlayer);
	bool CloseEnchaseHole(CPlayer* pPlayer,int nIndex);
	ulong CloseEnchaseHole();
	void EnchaseArrange();
	void ActiveHoles(int nNum);

	bool ActiveEnchaseHole(WORD wIndex);

	//����������ʽ�ġ������õĸ������ԣ�����û�б���ֹ��������
	//ʽ���Զ����ᱻ����	
	void GetValidAddonProperties(vector<eGoodsAddonProperties>& vOut);	
	
	//��ȡ������Ч�ĸ������ԣ�������ʽ����	
	void GetEnabledAddonProperties(vector<eGoodsAddonProperties>& vOut);
	void GetAddonPropertyValues(eGoodsAddonProperties gapType, vector<long>& vOut);
	long GetAddonPropertyValues(eGoodsAddonProperties gapType, ulong dwID);
	bool HasAddonPropertyValues(eGoodsAddonProperties gapType);
	bool HasAddonProperty(eGoodsAddonProperties gapType);

	bool AddonPropertyUpdate(eGoodsAddonProperties gapProperty,eGoodsAddonProperties gapRange);

	void MountBind(const CGUID& playerID);
	void AddRefine(long lVal);

	//��Ʒ���������� �ӿ�
	const char* GetMakerName(){return m_strMakerName.c_str();}
	bool SetMakerName(const char* pszMakerName){if(256 > strlen(pszMakerName)){ m_strMakerName=pszMakerName;return true;}return false;}

	/// ���ű����ӵĶ�����Ʒ��������ؽӿ�
	/// 9.7.2009 : ʹ�������������ַ����������������Ʒ���������������������ܳ�ͻ��
	const char *GetScriptAddonDesc() const { return m_strMakerName.c_str(); }
	bool SetScriptAddonDesc( const std::string &desc ) {if(256 > desc.size()){ m_strMakerName = desc;return true;}return false; }

	/// ���¡��ű��������������ͻ���
	void UpdateScriptAddonDesc( const CGUID &playerID );


	//��ȡ��Ʒ���ܼ�	
	ulong GetTotalPrice();
	
	void SetGoodsName(const CHAR* strGoodsName);
	
	const CHAR* GetGoodsName();		
	
	virtual void AI();
	
	virtual bool Clone(CBaseObject* pObject);
	
	virtual bool Serialize(vector<uchar>* pStream, bool b = true);
	
	virtual bool Unserialize(uchar* pStream, long& lOffset, bool b = true);

	bool SerializeForOldClient(vector<uchar>* pStream, bool b = true);

	virtual bool Serialize(DBWriteSet& setWriteDB, bool b = true);	
	virtual bool Unserialize(DBReadSet& setReadDB, bool b = true);
	bool SerializeForOldClient(DBWriteSet& setWriteDB, bool b = true);

	bool SerializeForRgnDisplay(DBWriteSet& setWriteDB,bool b=true);
	
	virtual const CHAR* ToString();
	
	virtual void Release();

	friend class CGoodsFactory;

private:
	/// ���ID	
	ulong m_dwBasePropertiesIndex;
	/// ��Ʒ��������
	CGoodsBaseProperties* m_baseProperties;
	/// ����
	ulong m_dwAmount;

	ulong m_dwBuyPrice;

	/// ����������
	string m_strMakerName;
	/// ��Ʒ�������Լ�
	vector<CGoods::tagAddonProperty> m_vAddonProperties;
	/// ��Ʒ��װ���Լ�
	vector<CGoodsBaseProperties::tagSuitProperty> m_vSuitProperties;

	/// �����׿�ID
	ulong m_dwActivedKitCardId;
	/// �����Ƕ����
	ulong m_dwMaxHoleNum;
	/// ��Ƕ������
	CEnchaseHole** m_pEnchaseHoleArray;

private:
	//void AddRefine(long lVal);
	//void EDCalculate();
	/// ���������Ʒ���ӵ�PK CPֵ
	//void CalculatePKCP();
	//void AtkEDUpdate();
	//void DefEDUpdate();	
	//bool ImmunityUpdate();//���˸���
	//bool PierceUpdate();//��͸����
	//bool LuckyUpdate();//���˸���
	//bool ConstellationUpdate();//��������
	//bool AddonHpUpdate();//hp����׷�ӷ�Χ
};
