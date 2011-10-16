/** 
*  @file   Goods.h
*  @author
*
*  @brief  ��Ʒʵ����
*  @date   
*/

#pragma once

#include "../Shape.h"
#include "../../../../Public/ServerPublic/Server/Common/GoodsBaseProperties.h"

class CGoodsFactory;
class CRsPlayer;
class CEnchaseHole;

class CGoods 
: public CShape
{
public:
	/// ��������
	struct tagAddonProperty
	{
		/// �������Ե�����		
		eGoodsAddonProperties gapType;		
		/// �����Դ���ֵ		
		long lValues[2];

		tagAddonProperty();
		~tagAddonProperty();

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

public:
	CGoods();
	CGoods(OBJECT_TYPE otType);
	~CGoods();

	/// ��ȡ��Ʒ���������б�
	vector<CGoods::tagAddonProperty>& GetAllAddonProperties(void);

	/// ����GUID
	void SetExID(CGUID& guid){m_guExID = guid;}
	/// ��ȡ�������ID
	ulong GetBasePropertiesIndex(void);
	
	/// ��������
	void SetAmount(ulong dwAmount);
	/// ��ȡ����
	ulong GetAmount(void);
	/// ��ȡ���Ҽ۸�
	ulong GetSilverPrice(void){return m_baseProperties->GetSilverPrice();}
	/// ��ȡ��Ʒ����
	eGoodsBaseType GetGoodsBaseType(void){return m_baseProperties->GetGoodsBaseType();}
	/// ��ȡ�۸�
	ulong GetPrice(void);
	/// ��ȡ����
	const char* GetGoodsName(void);
	/// ��ȡ����
	const char* GetDescribe(void);

	/// ��ȡ��Ʒװ������
	eEquipType GetEquipType(void);
	/// ��ȡ��Ʒ��������
	eTrumpType GetTrumpType(void);

	//��ȡ��Ʒ���ܼ�	
	ulong GetTotalPrice(void);
	/// ��ȡ�����Ƕ��
	ulong GetMaxEnchaseHoleNum(void);
	/// �Ƿ��������
	bool CanReparied();	
	/// ��ʼ����Ƕ��
	void InitEnchaseHole(void);
	/// ȡ�ü�����Ƕ������
	ulong GetActivedHoleNum(void);
	/// ��Ƕ����
	void EnchaseArrange();
	/// �Ƿ������Ƕ
	bool CanEnchase();
	/// �ɴ������	
	long* GetEnchaseHoleData();
	/// ������Ƕ��״̬
	void SetEnchaseHoleState(int nIndex,bool bVal);

	/// ȡ����Ʒ��������
	CGoodsBaseProperties* GetBaseProperties(void) { return m_baseProperties; }
	/// ��ȡ��Ʒ����������
	const char* GetMakerName(){return m_strMakerName.c_str();}
	/// ������Ʒ����������
	void SetMakerName(const char* pszMakerName){m_strMakerName=pszMakerName;}
	
	/// ���ű����ӵĶ�����Ʒ��������ؽӿ�
	/// 9.7.2009 : ʹ�������������ַ����������������Ʒ���������������������ܳ�ͻ��
	const char *GetScriptAddonDesc() const { return m_strMakerName.c_str(); }
	void SetScriptAddonDesc( const std::string &desc ) { m_strMakerName = desc; }

	/// ��Ƕ
	bool Enchase(CGoods* pGoods,int nIndex);

	//����������ʽ�ġ������õĸ������ԣ�����û�б���ֹ��������
	//ʽ���Զ����ᱻ���ء�	
	void GetValidAddonProperties(vector<eGoodsAddonProperties>& vOut);	
	//��ȡ�Ѿ���������ֵ	
	void GetAddonPropertyValues(eGoodsAddonProperties gapType, vector<long>& vOut);
	long GetAddonPropertyValues(eGoodsAddonProperties gapType, ulong dwID);

	// �������Ƿ���ڸ���Ʒ�ϡ�
	bool IsAddonProperyExist(eGoodsAddonProperties gapType);
	bool SetAddonPropertyBaseValues(eGoodsAddonProperties gapType, ulong dwID, long lValue);
	bool AddAddonPropertyBaseValues(eGoodsAddonProperties gapType,ulong dwID,long lVal1,long lVal2=0,bool bEnable=true,bool bIsImplicit=FALSE);
	bool DelAddonPropertyBaseValues(eGoodsAddonProperties gapType);
	void AddAddonProperty(uchar* pStream, long size );
	
	/// ��¡
	virtual bool Clone(CBaseObject* pObject);
	/// ����
	virtual bool Serialize(vector<uchar>* pStream, bool b = true);
	/// ����
	virtual bool Unserialize(uchar* pStream, long& lOffset, bool b = true);
	/// ����
	virtual bool Serialize(DBWriteSet& setWriteDB, bool b = true);
	/// ����
	virtual bool Unserialize(DBReadSet& setReadDB, bool b = true);
	/// ���
	virtual void Release();

	friend class CGoodsFactory;

private:
	/// ��������ID
	void SetBasePropertiesIndex(ulong dwID);
	/// ������Ʒ��������
	void SetBaseProperties(CGoodsBaseProperties* baseProperties) { m_baseProperties = baseProperties; }
	/// ������Ʒ��Ƕ����
	void EnchaseRelease(void);

private:
	/// �������ID
	ulong m_dwBasePropertiesIndex;
	/// ��Ʒ��������
	CGoodsBaseProperties* m_baseProperties;
	/// ����
	ulong m_dwAmount;
	/// ����������
	string m_strMakerName;

	/// ���������б�
	vector<CGoods::tagAddonProperty> m_vAddonPropertiesList;

	/// ��Ƕ������
	CEnchaseHole** m_pEnchaseHoleArray;
	/// ��ʼ��Ƕ������
	int	m_nInitHoleNum;
	/// �����׿�ID
	ulong m_dwActivedKitCardId;
};
