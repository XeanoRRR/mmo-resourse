/** 
*  @file   GoodsBaseProperties.h
*  @author
*
*  @brief  ��Ʒ����������
*  @date   
*/

#pragma once

#include "../../../Module/NoCopyable.h"
#include "../../../Common/GoodsDef.h"

class GoodsSetup;
class CGoodsFactory;

class CGoodsBaseProperties 
	: private NonCopyable
{
public:
	/// �������Ե�ֵ
	struct tagAddonPropertyValue
	{
		/// ���		
		ulong dwId;
		/// ����ֵ		
		long lBaseValue;		
		
		tagAddonPropertyValue();
		~tagAddonPropertyValue();
		/// ����
		void Clear(void);
		/// ����
		bool Serialize(vector<uchar>* pStream, bool b = true);
		/// ����
		bool Unserialize(uchar* pStream, long& lOffset, bool b = true);
		/// ����
		bool Serialize(DBWriteSet& setWriteDB, bool b = true);
		/// ����
		bool Unserialize(DBReadSet& setReadDB, bool b = true);
	};

	typedef vector<tagAddonPropertyValue> AddonPropertyValueList;

	/// �������Խṹ
	struct tagAddonProperty
	{
		/// �������Ե�����		
		eGoodsAddonProperties gapType;
		/// �������Ƿ񱻼��Ϊ�Ժ���������Ԥ��		
		bool bIsEnabled;
		/// �Ƿ����������ԣ�����ʾ��		
		bool bIsImplicitAttribute;		
		/// ����ֵ�б�		
		AddonPropertyValueList vValues;
		
		tagAddonProperty();
		~tagAddonProperty();	
		/// ����
		void Clear();
		/// ����
		bool Serialize(vector<uchar>* pStream, bool b = true);
		/// ����
		bool Unserialize(uchar* pStream, long& lOffset, bool b = true);
		/// ����
		bool Serialize(DBWriteSet& setWriteDB, bool b = true);
		/// ����
		bool Unserialize(DBReadSet& setReadDB, bool b = true);
	};

	/// ��װ����
	struct tagSuitProperty
	{
		/// ��������
		eGoodsAddonProperties gapType;
		/// ����ֵ�б�
		AddonPropertyValueList vValues;
		/// �Ƿ񼤻�
		bool bActived;

		tagSuitProperty();		
		~tagSuitProperty();
		/// ����
		void Clear();
		/// ����
		bool Serialize(vector<uchar>* pStream, bool b = true);
		/// ����
		bool Unserialize(uchar* pStream, long& lOffset, bool b = true);
		/// ����
		bool Serialize(DBWriteSet& setWriteDB, bool b = true);
		/// ����
		bool Unserialize(DBReadSet& setReadDB, bool b = true);
	};

	/// ��Ʒ��������
	struct tagTriggerProperty
	{
		/// ��������
		eGoodsAddonProperties  gapType;
		/// ֵ1
		long lVal1;
		/// ֵ2
		long lVal2;
		/// �����ȼ�
		ushort wTriggerLevel;
	};

	/// ��Ʒ��Ϣ
	struct tagGoodsInfo
	{
		/// ���ID
		ulong dwGoodsId;
		/// ����
		ulong dwAmount;
	};

	/// ��Ʒɾ����Ϣ
	struct tagGoodsDelInfo
	{
		/// λ��
		ulong dwPos;
		/// ����
		ulong dwNum;
	};

	/// ͼ����Ϣ
	struct tagIcon
	{	
		/// ͼ������
		eIconType itIconType;
		/// ͼ��ID
		ulong dwIconID;
	};

	typedef vector<tagAddonProperty> AddonPropertyList;
	typedef vector<tagSuitProperty> SuitPropertyList;
	typedef vector<tagTriggerProperty> TriggerPropertyList;
	typedef vector<tagIcon> IconList;

private:
	/// ���ID
	ulong m_index;
	/// ԭʼ��
	string m_strOriginalName;
	/// ��ʾ��
	string m_strName;
	/// ��Ʒ�۸�
	ulong m_dwPrice;
	/// ��Ʒ���Ҽ۸�
	ulong m_dwSilverPrice;
	/// ��Ʒ����
	string m_strDescribe;
	/// ��Ʒ��������
	eGoodsBaseType m_GoodsBaseType;
	/// �������Լ�
	AddonPropertyList m_vAddonProperties;
	/// ��װ���Լ�
	SuitPropertyList m_vSuitProperties;
	/// �������Լ�
	TriggerPropertyList m_vTriggerProperties;
	/// ͼ�꼯
	IconList m_vIcons;

	friend class GoodsSetup;
	friend class CGoodsFactory;
	
public:
	CGoodsBaseProperties();
	~CGoodsBaseProperties();
	/// ����
	bool Serialize(vector<uchar>* pStream, bool b = true);
	/// ����
	bool Unserialize(uchar* pStream, long& lOffset, bool b = true);
	/// ����
	bool Serialize(DBWriteSet& setWriteDB, bool b = true);
	/// ����
	bool Unserialize(DBReadSet& setReadDB, bool b = true);

	/// ��ȡ���ID
	ulong GetIndex(void) {return m_index;}
	/// ��ȡԭʼ��
	const char* GetOriginalName();
	/// ��ȡ��ʾ��
	const char* GetName();
	/// ��ȡ����
	const char* GetDescribe();
	/// ��ȡ�۸�
	ulong GetPrice();
	/// ����ͼ�����ͻ�ȡͼ��ID
	ulong GetIconID( eIconType itType );
	/// ��ȡ���Ҽ۸�
	ulong GetSilverPrice(){return m_dwSilverPrice;}
	/// ��ȡ��Ʒ��������
	eGoodsBaseType GetGoodsBaseType(){return m_GoodsBaseType;}
	/// ��ȡ�������Լ�
	AddonPropertyList& GetAddonProperty(){return m_vAddonProperties;}
	/// ��ȡ��װ���Լ�
	SuitPropertyList* GetSuitProperties(){return &m_vSuitProperties;}
	/// ��ȡ�������Լ�
	TriggerPropertyList& GetTriggerProperties(){return m_vTriggerProperties;}
	/// ���ݸ����������ͻ�ȡ��������
	void GetAddonPropertyValues(eGoodsAddonProperties gapType, AddonPropertyValueList& vOut);
	/// ���ݸ����������ͺ͸������Ա�Ż�ȡ��������ֵ
	ulong GetAddonPropertyValue(eGoodsAddonProperties gapType,ulong dwId);
	/// ��ȡ����û�б���ֹ����Ч�������Լ�
	void GetValidAddonProperties(vector<eGoodsAddonProperties>& vOut);
	/// �жϸ��������Ƿ�����
	bool IsImplicit(eGoodsAddonProperties gapType);
	/// �Ƿ��������������
	bool IsHasAddonPropertie(eGoodsAddonProperties gapType);

private:
	/// ����
	void Clear();
	/// ������Ʒ��������
	void SetGoodsBaseType(eGoodsBaseType type){m_GoodsBaseType=type;}
};
