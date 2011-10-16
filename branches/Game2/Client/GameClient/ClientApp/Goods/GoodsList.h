#pragma once
#include "../../../Public/Common/GoodsDef.h"

class CGoods;

#define MONEY_INDEX	1185	// Ǯ�ı��
#define YUANBAO_INDEX	48	// Ԫ���ı��
#define JIFEN_INDEX	303023044	// ���ֵı��

// ��Ʒ����ȫ�ֻ�����
class CGoodsList
{
	friend class CGoods;
public:
	CGoodsList(void);
	~CGoodsList(void);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:

	// ��Ʒ�������ԣ�ÿ����Ʒ���е�
	struct tagBaseProperty
	{
		ulong	dwIndex;		//	���
		string  strOrginName;	//	ԭʼ��   < 20
		string	strName;		//	�������� < 20

		// �ͻ���ר������
		ulong	dwIconId;		//	����ͼ��ID
		ulong	dwGroundId;		//	��غ�ͼ��ID
		ulong	dwEquipID;		//	��װͼ��ID
		ulong	dwSound;		//	��������
		ulong	dwSoundID1;		//	�Ӷ�/���˺�����
		ulong	dwSoundID2;		//	���⹥��/��������
		bool	bSoundSwitch;	//	����ʱ�Ƿ����
		ulong	dwWeaponActType;//	������Ӧ�Ķ�������
		ulong	dwType;			//	����
		ulong	dwPrice;		//	��Ҽ۸�
		ulong	dwSilverPrice;	//	���Ҽ۸�
		//ulong	dwWeight;		//	����

		string	strContent;		//	�������� < 64
	};

	// ��Ʒ�������ԣ�������Ʒ������һ
	struct tagEffect
	{
		ushort	wType;			// ��������
		bool	bEnable;		// �Ƿ���Ч(Ϊ�����������Ԥ��)
		bool	bHide;			// �Ƿ�����������
		long	dwValue1;		// ��һ��ֵ
		long	dwValue2;		// �ڶ���ֵ
		//ushort	wOdds;			// ���ּ���

		tagEffect()
		{
			wType = 0;
			bEnable = false;
			bHide = false;
			dwValue1 = 0;
			dwValue2 = 0;
		}
	};
	
	//�ܿصĸ������Խṹ��1.��Ӧ�ܿص�ĳ���������Եı�ţ�2.�˸������Ե�ֵ1��3.�˸������Ե�ֵ2��4.�˸��������ܿص�����ֵ����Ҫ������װ�����Լ�������
	struct  tagGoodsBeControlledAttri
	{
		ushort	wPropertyName;	// ��Ӧĳ���������Ա�ţ����������Ե�ö��ֵ��
		long	dwValue1;		// ��������ֵ1
		long	dwValue2;		// ��������ֵ2
		ushort	wActiveNum;	// Ҫ�����������ﵽ����С��ֵ
	};

	//��װ����
	struct  tagSuitAttribute
	{
		vector<string> Suit_OriginName;															// �����װ��ԭʼ��
		uchar	wSuit_Count;																	// ��װ�ܵļ���
		map<ulong/*�ض��ļ���*/,map<ulong/*��������*/,tagEffect/*�˸������Ե������ṹ*/>>	 Suit_ActiveProperty;	// �����װ�ض��ļ����ɼ�������Լ���ֵ
	};

	//�׿�����
	struct tagSeriesCardProperty 
	{
		uchar HOLE_NUM_REQUIRE;							//Ҫ�󿪿׵�����
		uchar CARD_NUM_REQUIRE;							//�׿���������
		vector<ulong>         CardsID;          		//���ſ�ƬID(����Ƭ������)
		map<ulong,int>     CardsEffects;				//�׿��ĸ�������
	};

	// һ��������Ʒ�ṹ
	struct tagGoods2
	{
		tagBaseProperty			BaseProperty;			// ��������
		vector<tagEffect>		vectorEffect;			// ��������
		vector<tagGoodsBeControlledAttri>		m_SuitProperty;		// ��Ʒ��Ϊ��װʱ���е���������
		vector<tagGoodsBeControlledAttri>		m_AkiviertProperty;	// ��������
	};

	struct tagMoneyGraphics
	{
		long lMinNum;		//��������
		long lMaxNum;		//��������
		long lGraphicsID;	//ͼ�α��
	};

private:
	static map<ulong,string>		m_mapSuitName;				// ����װID��Ӧ���������װ������
	static map<ulong, tagGoods2>	m_mapGoodsList;				// ԭʼ��Ʒ�б�(wIndex, tagGoods)
	static map<string, tagGoods2*>	m_mapGoodsListByName;		// ������Ϊ������ԭʼ��Ʒ�б�(strOrginName, *tagGoods)
	static map<ulong/*��װID*/, tagSuitAttribute>	m_SuitList;	// ��װ�����Ա������������װ�ڸ��ּ��������¼������ܵ�����
	static vector<tagMoneyGraphics>	m_stMoneyGraphics;			// ��ҵ�������Ϣ
	static vector<tagMoneyGraphics>	m_stSilverMoneyGraphics;	// ���ҵ�������Ϣ
	//static map<string,ulong>		m_mapEffects;				// ӳ����Ʒ���ƺ͸�������
	static map<string,ulong>		m_mapAllEffects;			// ӳ�丽���������ƺ͸�������ö��ֵ
	static map<ulong,string>		m_mapEffectReverse;			// ӳ����Ʒ�������Ժ�����
	static void InitMapEffects();								// ������Ʒ�������Ժ����Ƶľ���ӳ��

	static map<ulong/*�׿���ID��*/,tagSeriesCardProperty>	m_mapSeriesCardList;	// �׿������б�

	 //m_mapGoodsEffectValue;	// ��Ʒ�ĸ���������ֵ��ӳ��
public:
	//��ȡͨ����װID���Ҷ�Ӧ����װ����
	static string&						 GetSuitName(ulong dwSuitID);
	//��ȡ����ӳ��map
	//static map<string,ulong>&			 GetMapEffects()							{return m_mapEffects;}
	//��ȡ��ת��ӳ��map
	static map<ulong,string>&			 GetMapEffectReverse()						{return m_mapEffectReverse;}
	//��ȡ�����������͸�������ֵ��ӳ���
	static map<string,ulong>&		     GetMapAllEffect()							{return m_mapAllEffects;}
	//��ȡ���е���װ����Ϣ�б�
	static map<ulong, tagSuitAttribute>& GetSuitList()								{return m_SuitList;}
	//��ȡ��ID����װ���ܼ���
	static WORD							 GetSuitCount(ulong SuitID);
	//��ȡ��ID����װ��ԭʼ���б�
	static vector<string>*				 GetSuitOriginNameList(ulong SuitID)		{return &(m_SuitList[SuitID].Suit_OriginName);}
	//��ȡ��ID����װ��������Ϣ�б�
	static map<ulong,map<ulong,tagEffect>>&	 GetSuitAttributeInfor(ulong SuitID)		{return m_SuitList[SuitID].Suit_ActiveProperty;}
	//��ȡ��ID����װ��Ʒ����������(nWhich = 1 ȡ��װ������Ʒ��ĳ������ֵ1�� = 2ȡ����ֵ2��= 3ȡ����ֵ3���������Ǽ�����������ﵽ����װ��Ʒ������)
	static long GetSuitAttrOfGoods(ulong dwIndex,ulong wType,int nWhich);

	/////////////////�׿����Խӿ�//////////////
	//��ȡ���е��׿���Ϣ
	static map<ulong/*�׿���ID��*/,tagSeriesCardProperty>& GetSeriesCardList()		{return m_mapSeriesCardList;}
	//��ȡĳ���׿�
	static tagSeriesCardProperty& GetSeriesCardProperty(ulong index)				{return m_mapSeriesCardList[index];}
	//��ȡĳ���׿��ĸ��������б�
	static map<ulong,int>& GetSeriesCardEffectList(ulong index)						{return m_mapSeriesCardList[index].CardsEffects;}
	//��ȡĳ���׿��Ŀ�ƬID�б�
	static vector<ulong>&  GetSeriesCardIDList(ulong index)							{return m_mapSeriesCardList[index].CardsID;}
	//��ȡĳ���׿��ľ�������
	static ulong GetSeriesCardRequireNum(ulong index)								{return m_mapSeriesCardList[index].CARD_NUM_REQUIRE;}
	//����ĳ����Ƭ��ID���Ҵ����׿���ID
	static ulong GetSeriesCardIndexByCardID(ulong cardIndex);
	///////////////////////////////////////////

	static bool LoadGoodsList(const char* filename);			// ���ļ���ȡԭʼ��Ʒ�б�
	static void	LoadSuitList(const char* filename);				// ���ļ���ȡԭʼ��װ�б�

	static void LoadCardList(const char* filename);				// ���ļ���ȡ�׿��б���Ϣ

	static map<ulong, tagGoods2>& GetMapGoodsList()		{return m_mapGoodsList;}
	static map<string, tagGoods2*>& GetMapGoodsListByName() {return m_mapGoodsListByName;}
	static tagGoods2*	GetProperty(ulong dwIndex)				{return (m_mapGoodsList.find(dwIndex)!=m_mapGoodsList.end())?(&m_mapGoodsList[dwIndex]):NULL;}
	static tagGoods2*	GetProperty(const char* strOrginName)
	{
		if(NULL == strOrginName)	return NULL;
		return (m_mapGoodsListByName.find(strOrginName)!=m_mapGoodsListByName.end())?m_mapGoodsListByName[strOrginName]:NULL;
	}
	static long GetEffectVal(ulong dwIndex,ushort wType,int nWhich);
	static tagEffect* GetEffectTag(ulong dwIndex,ushort wType);
	static bool InitMoneyGraphics(const char* filename);		// ���ļ���ȡ��Ǯ��������Ϣ
    static long GetMoneyGraphics(long lMoneyNum,int iMoneyType = GBT_MONEY);	// ���ݻ��ҵ������������ȡ��Ӧ�Ļ���ģ��ID��iMoneyType 0=��ҡ�1=���ң�
	static tagGoods2*	GetPropertyByEquipID(ulong dwEquipID);	// ����װ����װ��Ż�ȡ��Ʒ����
	static bool FillGoodsInfo(ulong dwIndex,CGoods *pGood);		// ����������Ʒ��Ϣ������Ʒ��

	// �ж��Ƿ���װ��
	static bool IsCanEquip(CGoods *pGoods);


	// ���ݴ������Ʒ��ö�Ӧ��Ʒ��ͼƬ·���Լ���ɫ
	static void GetGoodsQuality(ulong dwIndex,string & str, long & color);

	/// ��ȡ��Ʒ��ʱ��
	static const string GetGoodsLimitTime(ulong dwGoodsIndex);

	/// �Ƿ��д˸�������
	static bool HaveEffect(ulong dwGoodsIndex,ushort wEffectType);
};
