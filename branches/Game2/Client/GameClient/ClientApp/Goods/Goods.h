#pragma once
#include "../MoveShape.h"
#include "Goodslist.h"

class CEffect;
class CBitmapX;
class CClientRegion;

class CGoods:public CMoveShape
{
	friend class CGoodsList;
public:
	enum eAction
	{
		GOOD_DROP,
		GOOD_NORMAL,
		GOOD_EXCEPT
	};

	struct tagBaseProperty
	{
		string	strName;		//	�������� < 20
		ulong	dwValue;		//	��Ҽ۸�
		ulong	dwSilverPrice;	//	���Ҽ۸�
		ulong	dwBuyPrice;	//  ��Ʒ����۸���ҵϵͳʹ�ã�
		ulong	dwPriceType;	//  �۸�����
		ulong	dwType;			//	��Ʒ����

		tagBaseProperty()
		{
			strName = "";
			dwValue = 0;
			dwSilverPrice = 0;
			dwBuyPrice = 0;
			dwPriceType = 0;
			dwType = 0;
		}
	};

	struct tagCardProperty
	{
		ulong  dwCardIndex;			//  ��Ƭ������
		CGUID  CardGUID;			//	��Ƭ��GUID
		string strCardName;			//	��Ƭ������
		ulong  dwCardType;			//	��Ƭ������
		ulong  dwInlayEquipType;	//	��Ƭ��Ӧ��װ������
		ulong   bInlayEquipLevel;	//	��Ƭ��Ӧ��װ������
		ulong   bMaxNum;				//	�˿�Ƭ��ͬһװ������ʹ������
		bool   bIsSeriesCard;		//�˿��Ƿ�Ϊ�׿�
		vector<CGoodsList::tagEffect>  CardEffect;	//��Ƭ�ĸ�������

		tagCardProperty()
		{
			dwCardIndex = -1;
			CardGUID = CGUID::GUID_INVALID;
			strCardName = "";
			dwCardType = 0;
			dwInlayEquipType = 0;
			bInlayEquipLevel = 0;
			bMaxNum = 0;
			bIsSeriesCard = false;
		}
		tagCardProperty(const tagCardProperty& src)
		{
			dwCardIndex = src.dwCardIndex;
			CardGUID = src.CardGUID;
			strCardName = src.strCardName;
			dwCardType = src.dwCardType;
			dwInlayEquipType = src.dwInlayEquipType;
			bInlayEquipLevel = src.bInlayEquipLevel;
			bMaxNum = src.bMaxNum;
			bIsSeriesCard = src.bIsSeriesCard;
			CGoodsList::tagGoods2* pstGoods=CGoodsList::GetProperty(dwCardIndex);
			if (pstGoods)
			{
				vector<CGoodsList::tagEffect>::iterator EffectIter=pstGoods->vectorEffect.begin();
				for(;EffectIter!=pstGoods->vectorEffect.end();EffectIter++)
				{
					CardEffect.push_back(*EffectIter);
				}
			}
		}
	};

	CGoods(void);
	CGoods(ulong dwIndex);
	CGoods(const CGoods &srcGoods);
	~CGoods(void);
	void AI(void);
	ulong GetIndex(){return m_dwIndex;}
	void  SetIndex(ulong dwVal){m_dwIndex=dwVal;}
	void SetNum(long lVal){m_lNum=lVal;}
	long GetNum(){return m_lNum;}
	const char* GetOrginName(){return GetProperty()->strOrginName.c_str();}
	void SetGoodsBaseType(eGoodsBaseType type){m_GoodsBaseType=type;}
	ulong GetGoodsBaseType();
	vector<CGoodsList::tagEffect>*	GetEffect()				{return &m_vectorEffect;}
	//vector<CGoodsList::tagFixValues>* GetFixs()				{return &m_vecFixValues;}
	tagBaseProperty* GetBaseProperty()						{return &m_stBaseProperty;}
	void SetBaseProperty(tagBaseProperty *pstBaseProperty)	{m_stBaseProperty=*pstBaseProperty;}

	CGoodsList::tagBaseProperty* GetProperty()	{return &(CGoodsList::GetProperty(m_dwIndex)->BaseProperty);}

	bool AddToByteArray(vector<uchar>*,bool bExtData=true);
	// ����
	bool DecordFromByteArray(uchar* pByte, long& pos, bool bExData=true);
	bool DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData=true);


	////////////XY 2007.11.13///////////////
	void UpdateProperty(CMessage* pMsg);
	////////////////////////////////////////

	long GetEffectVal(ushort wType,int nWhich);
	void SetEffectVal(ushort wType,int nWhich,long lVal);
	void SetNewEffectVal(ushort wType,int nWhich,long lVal);		//����Ʒ����µĸ�������
	//long GetFixVal(ushort wType,int nWhich);
	bool CanRepair();
	bool CanUpgrade();

	bool HasEffect( ushort wType );
	//bool IsEffectChange();
	//bool IsInTradePage(){return m_bInTradePage;}
	//void SetInTradePage(bool bVal){m_bInTradePage=bVal;}
	//bool IsInPlayerShop(){return m_bInPlayerShop;}
	//void SetInPlayerShop(bool bFlag) {m_bInPlayerShop = bFlag;}
	//�ж���Ʒ�Ƿ���Ե���
	static bool isGoodsCanAccumulate(CGoods *pOnGroundGoods);
	ulong RepairPrice();
	ulong GetUpdatePrice();
	ulong GetPrice();
	ulong GetPriceType();
	ulong GetSalePrice();

	bool Display(void);

	void DispalayFlashEffect();			// ��ʾ��Ʒ����
	void IniFlashEffect();				// ��ʼ��������Ч

	//void SetNameColor(ulong dwColor);
	//ulong GetNameColor();
	float GetHeight();
	void  SetDropEffect();
	bool  GetDropEffect() {return m_bDropEffect;}

	CEffect& GetFlashEffect() {return (*m_pFlashEffect);}

	//��Ʒ����Ƕ�׽ӿ�
	bool Stiletto();						//����Ʒ��׵Ľӿ�
	bool PlugUpHole(ushort index,ushort wHoleNum);								//�����Ƕ��
	bool EnchaseCard(ushort index,tagCardProperty card);			//��Ƕ��Ƭ
	bool RemoveCard(ushort index);								//�����Ƭ
	void SetHoleNum(ushort wNum);									//������Ʒ�ĵ�ǰ�ѿ��Ŀ���
	long GetHoleNum()	{return m_wHoleNum;}					//��ȡ��Ʒ�ĵ�ǰ�ѿ��Ŀ���
	long GetCardNum()	{return (long)m_InlayHole.size();}		//������Ʒ������Ƕ�Ŀ�Ƭ��
	map<long,tagCardProperty>& GetInlayHole() {return m_InlayHole;}	//������Ʒ�ϵ���Ƕ����Ϣ
	bool GetHoleState(ushort HoleIndex);							//��ȡ��Ʒָ��λ�õ���Ƕ�׵�״̬�������أ�
	void SetHoleState(ushort HoleIndex,bool bState);				//������Ʒָ��λ�õ���Ƕ�׵�״̬�������أ�

	// ��������
	void Lock();
	void UnLock();
	bool IsLocked();
	////////XY 2008.1.3////////
	bool IsHaveShadow()	{return m_bHaveShadow;}
	void SetHaveShadowState(bool bState)	{m_bHaveShadow = bState;}
	///////////////////////////
	// ���ݴ������Ʒ��ö�Ӧ��Ʒ��ͼƬ·���Լ���ɫ
	void GetGoodsQuality(string & str, long & color);

	/// ��ȡ��Ʒ��ʱ��
	const string GetGoodsLimitTime();

	/// ��ȡ�����ߵ�����  AddBy X.Y 2009.9.18
	inline const string& GetManufacturerName() { return m_strManufacturerName; }
	/// ���������ߵ�����  AddBy X.Y 2009.9.18
	void SetManufacturerName(const char* pStrName);

	/// ��ȡ��Ʒװ������
	eEquipType GetEquipType(void);
	/// ��ȡ��Ʒ��������
	eTrumpType GetTrumpType(void);

	/// ��ȡ�ű���ӵĻ��Ϣ����
	//inline const string& GetActiveInfor() { return m_strActiveInfor; }

	/// ���ýű���ӵĻ��Ϣ����
	//void SetActiveInfor(const char* pStr);
private:
	ulong	m_dwIndex;						// ���
	long	m_lNum;
	eGoodsBaseType m_GoodsBaseType;
	//bool	m_bInTradePage;
	//bool	m_bInPlayerShop;
	bool	m_bHaveShadow;					//����Ʒ�Ƿ�����Ӱ
	
	///////��ҵϵͳ���������ԣ������NPC��Ʒ���д����ԣ�///////
	ulong	m_dwPurchaseLimit;				//�޹���
	///////////////////////////////////////////////////////////

	tagBaseProperty m_stBaseProperty;		//��Ʒ��������
	vector<CGoodsList::tagEffect>	m_vectorEffect;		// ��չ�����б�

	///////////��Ƕ������//////////
	map<long,tagCardProperty> m_InlayHole;	//��Ƕ�ף�ֻ����������ߵ���Ʒ���ܾ��У�
	long	m_wHoleNum;						//����Ʒ��ǰ�ѿ�����Ƕ����
	map<long,bool> m_mapHoleState;			//��Ʒ��ǰ�����׵Ŀ���״̬
	///////////////////////////////

	CEffect*		m_pFlashEffect;			//������Ч
	//ulong			m_dwNameColor;			//������ɫ
	
	//��Ʒ�����Ч��
	bool  m_bDropEffect;
	ulong m_dwDropTimeStart;
	float m_fDropHeight;

	// ������־
	long	m_lLockCounter;

	// ����������  AddBy X.Y 2009.9.18
	string  m_strManufacturerName;

	/// ���Ϣ
	//string  m_strActiveInfor;
};
