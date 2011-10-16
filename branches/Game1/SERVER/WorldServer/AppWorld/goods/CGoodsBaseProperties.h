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
using namespace std;

class CGoodsFactory;

//##ModelId=41A547FD0280
class CGoodsBaseProperties 
{
public:
	//װ��λ�ñ�ʶ
	enum EQUIP_PLACE
	{
		EP_UNKNOW,
		EP_HEAD,				//	1��ͷ��
		EP_NECKLACE,			//	2������
		EP_WING,				//	3�����
		EP_BODY,				//	4������
		EP_BACK,				//	5������
		EP_GLOVE,			//	6������	
		EP_BOOT,			//	7��Ь��
		EP_HEADGEAR,			//	8��ͷ��
		EP_FROCK,			//	9������		
		EP_LRING,			//	10�����ָ
		EP_RRING,			//	11���ҽ�ָ		
		EP_MEDAL1,			//	12��ѫ��1
		EP_MEDAL2,			//	13	ѫ��2
		EP_MEDAL3,			//	14	ѫ��3
		EP_FAIRY,			//	15��С����
		EP_WEAPON,			//	16������
		EP_WEAPON2,			//	17����������
		EP_CARD,			// ��Ƭ
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
	enum GOODS_TYPE
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

	//ICON���
	enum ICON_TYPE
	{
		//������		
		IT_CONTAINER,
		//������		
		IT_GROUND,
		//װ����		
		IT_EQUIPED
	};

	//�������
	enum SOUND_TYPE
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


	//�������Ե����ݣ���������	
	struct tagAddonPropertyValue
	{
		//���		
		DWORD		dwId;
		//����ֵ		
		LONG					lBaseValue;
								
		//�Ƿ���������		
		//BOOL									bIsEnableModifier;
		//�����Ե�����ֵ		
		//vector<tagAddonPropertyValueModifier>	vModifiers;
		
		tagAddonPropertyValue();
		
		~tagAddonPropertyValue();
		
		VOID Clear();
		
		BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
		
		BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);

		BOOL Serialize(DBWriteSet& setWriteDB, BOOL b = TRUE);
		
		BOOL Unserialize(DBReadSet& setReadDB, BOOL b = TRUE);
	};	
	
	struct tagAddonProperty
	{
		//�������Ե�����		
		GOODS_ADDON_PROPERTIES   				gapType;

		//�������Ƿ񱻼��Ϊ�Ժ���������Ԥ��		
		BOOL									bIsEnabled;

		//�Ƿ����������ԣ�����ʾ��		
		BOOL									bIsImplicitAttribute;

		//�ø������Գ��ֵļ��ʣ����10000�����1��ȱʡ10000		
		//DWORD									dwOccurProbability;

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


public:
	static const BYTE PACK_WIDTH=5;

protected:	
	string m_strOriginalName;
	
	string m_strName;

	eGoodsBaseType m_GoodsBaseType;

	friend class CGoodsFactory;

private:	
	DWORD m_dwPrice;
	
	DWORD m_dwSilverPrice;//���Ҽ۸�
	
	string m_strDescribe;
	
	GOODS_TYPE m_gtGoodsType;
	
	EQUIP_PLACE m_epEquipPlace;
	
	vector<tagAddonProperty> m_vAddonProperties;
	vector<tagSuitProperty> m_vSuitProperties;
	vector<tagTriggerProperty> m_vTriggerProperties;


//#ifdef _CLIENT
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

	VOID SetGoodsBaseType(eGoodsBaseType type){m_GoodsBaseType=type;}
	eGoodsBaseType GetGoodsBaseType(){return m_GoodsBaseType;}
	
	DWORD GetPrice();

	DWORD GetSilverPrice(){return m_dwSilverPrice;}
	VOID SetSilverPrice(DWORD dwPrice){m_dwSilverPrice=dwPrice;}
	
	/*DWORD GetHeight();*/
	
	//DWORD GetWidth();
	
	/*DWORD GetWeight();*/
	
	GOODS_TYPE GetGoodsType();
	
	BOOL CanEquip();
	
	EQUIP_PLACE GetEquipPlace();
	
	VOID GetAddonPropertyValues(GOODS_ADDON_PROPERTIES gapType, vector<tagAddonPropertyValue>& vOut);

	//��ȡ����û�б���ֹ����Ч����	
	VOID GetValidAddonProperties(vector<GOODS_ADDON_PROPERTIES>& vOut);

	DWORD GetAddonPropertyValue(GOODS_ADDON_PROPERTIES gapType,DWORD dwId);
	
	/*DWORD GetOccurProbability(GOODS_ADDON_PROPERTIES gapType);*/
	
	BOOL IsImplicit(GOODS_ADDON_PROPERTIES gapType);

	//�Ƿ��������������
	BOOL IsHasAddonPropertie(GOODS_ADDON_PROPERTIES gapType);


protected:

	VOID Clear();
};

#endif /* _INC_CGOODSBASEPROPERTIES_41A547FD0280_INCLUDED */
