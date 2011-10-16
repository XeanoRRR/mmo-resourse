// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CGOODSBASEPROPERTIES_41A547FD0280_INCLUDED
#define _INC_CGOODSBASEPROPERTIES_41A547FD0280_INCLUDED
#include <windows.h>
#include <string>
#include "../../../../public/GoodsDef.h"
#include "../../../../public/GoodsExDef.h"
#include "../../../../public/BaseDef.h"



using namespace std;

class CGoodsFactory;



/*
*��Ʒ����������
*/
class CGoodsBaseProperties 
{
public:
	//װ��λ�ñ�ʶ
	static enum EQUIP_PLACE
	{
		EP_UNKNOW,
		EP_HEAD,				//	0��ͷ��
		EP_NECKLACE,			//	1������
		EP_WING,				//	2�����
		EP_BODY,				//	3������
		EP_BACK,				//	4������
		EP_GLOVE,			//	5������	
		EP_BOOT,			//	6��Ь��
		EP_HEADGEAR,			//	7��ͷ��
		EP_FROCK,			//	8������		
		EP_LRING,			//	9�����ָ
		EP_RRING,			//	10���ҽ�ָ		
		EP_MEDAL1,			//	11��ѫ��1
		EP_MEDAL2,			//	12	ѫ��2
		EP_MEDAL3,			//	13	ѫ��3
		EP_FAIRY,			//	14��С����
		EP_WEAPON,			//	15������
		EP_WEAPON2,			//	16����������
		EP_CARD,			//	17����Ƭ
		EP_HORSE,
		EP_CLOAK,			//����
		EP_DECORATION1,		//��Ʒ1
		EP_DECORATION2,		//��Ʒ2
		EP_ADORN,			//����
		EP_GUARDIAN,		//�����ػ�ʯ

		EP_SUM,				//	װ��������

		EP_PACK1=10001,
		EP_PACK2=10002,
		EP_PACK3=10003,
		EP_PACK4=10004,
		EP_PACK5=10005
	};

	//��Ʒ���
	static enum GOODS_TYPE
	{
		//������
		GT_USELESS,
		//����Ʒ
		GT_CONSUMABLE,
		//װ��
		GT_EQUIPMENT,
		//����
		GT_PACK,
		//��Ƭ
		GT_CARD
	};

	//װ�����������Ƿ���
	static enum EQUIP_TYPE
	{
		NEITHER_ATTACK_DEFENSE,//������
		ATTACK,//������
		DEFENSE //�Ƿ���
	};

	//��Ƭ����
	static enum CARD_TYPE
	{
		//��Ƕ��
		CARD_CAN_ENCHASE=1,
		CARD_ENHANCE_ATTACK,	//+����
		CARD_ENHANCE_DEFEND,	//+����

		//����Ƕ��
		CARD_CANNOT_ENCHASE,
		CARD_COLLECT			//�Ѽ���
	};

	//ICON���
	static enum ICON_TYPE
	{
		//������		
		IT_CONTAINER,
		//������		
		IT_GROUND,
		//װ����		
		IT_EQUIPED
	};

	//�������
	static enum SOUND_TYPE
	{
		//ʰȡ		
		ST_PICKUP,
		//����		
		ST_DROP,
		//ʹ��		
		ST_USE,
		//װ��		
		ST_EQUIP,
		//�Ӷ�,δ����		
		ST_FLOURISH,
		//����,���⹥��		
		ST_SLASH
	};

	
		
	//##��ҵĽ�ɫ����,���ڳ�������
	static enum PLAYER_ROLE
	{
		//##��ͨ���
		PR_NORMAL_PLAYER			= 0x00000001,
		//##����᳤
		PR_GUILD_MASTER				= 0x00000002,
		//##���˻᳤
		PR_UNION_MASTER				= 0x00000004,
		//##����
		PR_CITY_MASTER				= 0x00000008,
	};

	//��������
	static enum PARTICULAR_ATTRIBUTE
	{
		//��������		
		PA_CANNOT_REPAIRED			= 0x00000001,
		//��������		
		PA_NEVER_DROP_DOWN			= 0x00000002,
		//���ߺ�����		
		PA_DROP_DOWN_WHEN_LOGOUT	= 0x00000004,
		//�����ص���		
		PA_DROP_DOWN_WHEN_DEAD		= 0x00000008,
		//��غ����ʧ		
		PA_DELETE_WHEN_DROP_DOWN	= 0x00000010,
		//���ɽ���Ҳ���ɴ�ֿⲻ�ɰ�̯		
		PA_CANNOT_TRADE_AND_STORE	= 0x00000020,
		//����		
		PA_UNIQUE					= 0x00000040,
		//ʹ�ûسǾ�\˲�ƾ�����		
		PA_DROP_DOWN_WHEN_RECALL	= 0x00000080,
		//���
		PA_RARITY					= 0x00000100
	};
	
	//�������Ե�ֵ
	struct tagAddonPropertyValue
	{
		//���		
		DWORD									dwId;
		//����ֵ		
		LONG									lBaseValue;		
		
		tagAddonPropertyValue();

		
		~tagAddonPropertyValue();
		
		VOID Clear();
		
		BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
		
		BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

		BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);
		
		BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);
	};	

	//�������Խṹ
	struct tagAddonProperty
	{
		//�������Ե�����		
		GOODS_ADDON_PROPERTIES   				gapType;
		//�������Ƿ񱻼��Ϊ�Ժ���������Ԥ��		
		BOOL									bIsEnabled;
		//�Ƿ����������ԣ�����ʾ��		
		BOOL									bIsImplicitAttribute;		

		//�����Դ���ֵ		
		vector<tagAddonPropertyValue>			vValues;
		
		tagAddonProperty();
		
		~tagAddonProperty();
		
		VOID Clear();
		
		BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
		
		BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

		BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);
		
		BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);
	};


	struct tagSuitProperty
	{
		GOODS_ADDON_PROPERTIES  gapType;
		vector<tagAddonPropertyValue>       vValues;
		BOOL bActived;
		tagSuitProperty();		
		~tagSuitProperty();
		
		VOID Clear();
		
		BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
		
		BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

		BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);
		
		BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);
	};

	//��Ʒ�������Խṹ����
	struct tagTriggerProperty
	{
		GOODS_ADDON_PROPERTIES  gapType;
		LONG lVal1;
		LONG lVal2;
		WORD wTriggerLevel;
	};

	struct tagGoodsInfo
	{
		DWORD dwGoodsId;
		DWORD dwAmount;
	};

	struct tagGoodsDelInfo
	{
		DWORD dwPos;
		DWORD dwNum;
	};

protected:
	//ԭʼ��
	string m_strOriginalName;

	//��ʾ��
	string m_strName;

	friend class CGoodsFactory;

private:
	//��Ʒ�۸�
	DWORD m_dwPrice;	

	DWORD m_dwSilverPrice;

	//��Ʒ����
	string m_strDescribe;
	
	GOODS_TYPE m_gtGoodsType;

	eGoodsBaseType m_GoodsBaseType;

	//װ����λ��ʶ
	EQUIP_PLACE m_epEquipPlace;

	//�������Դ������
	vector<tagAddonProperty> m_vAddonProperties;
	vector<tagSuitProperty> m_vSuitProperties;
	vector<tagTriggerProperty> m_vTriggerProperties;


//
public:	
	struct tagIcon
	{	
		ICON_TYPE 	itIconType;		
		DWORD		dwIconID;
	};


private:
	vector<tagIcon> m_vIcons;

	
	DWORD GetIconID( ICON_TYPE itType );




public:	
	BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
	
	BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

	BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);
	
	BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);

	
	CGoodsBaseProperties();
	
	~CGoodsBaseProperties();
	
	const CHAR* GetOriginalName();
	
	const CHAR* GetName();
	
	const CHAR* GetDescribe();
	
	DWORD GetPrice();	

	DWORD GetSilverPrice(){return m_dwSilverPrice;}
	VOID SetSilverPrice(DWORD dwPrice){m_dwSilverPrice=dwPrice;}
	
	GOODS_TYPE GetGoodsType();

	VOID SetGoodsBaseType(eGoodsBaseType type){m_GoodsBaseType=type;}
	eGoodsBaseType GetGoodsBaseType(){return m_GoodsBaseType;}
	
	BOOL CanEquip();
	
	EQUIP_PLACE GetEquipPlace();

	vector<tagAddonProperty>& GetAddonProperty(){return m_vAddonProperties;}
	vector<tagTriggerProperty>& GetTriggerProperties(){return m_vTriggerProperties;}

	
	VOID GetAddonPropertyValues(GOODS_ADDON_PROPERTIES gapType, vector<tagAddonPropertyValue>& vOut);

	DWORD GetAddonPropertyValue(GOODS_ADDON_PROPERTIES gapType,DWORD dwId);

	//��ȡ����û�б���ֹ����Ч����	
	VOID GetValidAddonProperties(vector<GOODS_ADDON_PROPERTIES>& vOut);

	vector<tagSuitProperty>* GetSuitProperties(){return &m_vSuitProperties;}
	
	
	
	
	
	
	BOOL IsImplicit(GOODS_ADDON_PROPERTIES gapType);

	//�Ƿ��������������
	BOOL IsHasAddonPropertie(GOODS_ADDON_PROPERTIES gapType);

protected:

	VOID Clear();
};

#endif /* _INC_CGOODSBASEPROPERTIES_41A547FD0280_INCLUDED */
