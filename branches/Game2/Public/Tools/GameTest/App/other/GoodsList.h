#pragma once
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
	virtual ~CGoodsList(void);

	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
public:

	// ��Ʒ����
	enum eGoodsType
	{
		GT_NORMAL,				// 0��ͨ(����ʹ�ã�����װ��)
		GT_EXPENDABLE,			// 1������
		GT_CARD,				// 2��Ƭ
		GT_MERCHANDISE,			// 3������Ʒ
		GT_HEAD,				// 4ͷ��
		GT_BODY,				// 5����
		GT_GLOVE,				// 6����
		GT_BOOT,				// 7ѥ��		
		GT_WEAPON,				// 8����
		GT_WEAPON2,				// 9����
		GT_NECKLACE,			// 10����
		GT_RING,				// 11��ָ
		GT_WAISTBAND,			// 12����
		GT_WING,				// 13���
		GT_HEADGEAR,			// 14ͷ��
		GT_FROCK,				// 15����
		GT_FAIRY,				// 16����
		GT_TATTOO,				// 17����
		GT_PACK,				// 18����
		GT_MEDAL,				// 19ѫ��
		GT_HORSE,				// 20����

		GT_NUM,					
	};


	// ��Ʒ�������ԣ�ÿ����Ʒ���е�
	struct tagBaseProperty
	{
		DWORD	dwIndex;		//	���
		string  strOrginName;	//	ԭʼ��   < 20
		string	strName;		//	�������� < 20

		// �ͻ���ר������
		DWORD	dwIconId;		//	����ͼ��ID
		DWORD	dwGroundId;		//	��غ�ͼ��ID
		DWORD	dwEquipID;		//	��װͼ��ID
		DWORD	dwSound;		//	��������
		DWORD	dwSoundID1;		//	�Ӷ�/���˺�����
		DWORD	dwSoundID2;		//	���⹥��/��������
		bool	bSoundSwitch;	//	����ʱ�Ƿ����
		DWORD	dwWeaponActType;//	������Ӧ�Ķ�������
		DWORD	dwType;			//	����
		DWORD	dwPrice;		//	��Ҽ۸�
		DWORD	dwSilverPrice;	//	���Ҽ۸�
		//DWORD	dwWeight;		//	����

		string	strContent;		//	�������� < 64
	};

	// ��Ʒ�������ԣ�������Ʒ������һ
	struct tagEffect
	{
		WORD	wType;			// ��������
		bool	bEnable;		// �Ƿ���Ч(Ϊ�����������Ԥ��)
		bool	bHide;			// �Ƿ�����������
		long	dwValue1;		// ��һ��ֵ
		long	dwValue2;		// �ڶ���ֵ
		//WORD	wOdds;			// ���ּ���
	};

	//������װ�ĵ�����Ʒ����������
	struct  tagGoodsBeControlledAttri
	{
		WORD	wPropertyName;	// ��Ӧĳ�������������ƣ����������Ե�ö��ֵ��
		long	dwValue1;		// ��������ֵ1
		long	dwValue2;		// ��������ֵ2
		WORD	wActiveNum;		// Ҫ�����������ﵽ����װ����
	};

	//��װ����
	struct  tagSuitAttribute
	{
		vector<string> Suit_OriginName;															// �����װ��ԭʼ��
		BYTE	wSuit_Count;																	// ��װ�ܵļ���
		map<DWORD/*�ض��ļ���*/,map<DWORD/*��������*/,int/*����ֵ*/>>	 Suit_ActiveProperty;	// �����װ�ض��ļ����ɼ�������Լ���ֵ
	};

	//�׿�����
	struct tagSeriesCardProperty 
	{
		BYTE HOLE_NUM_REQUIRE;							//Ҫ�󿪿׵�����
		BYTE CARD_NUM_REQUIRE;							//�׿���������
		vector<DWORD>         CardsID;          		//���ſ�ƬID(����Ƭ������)
		map<DWORD,int>     CardsEffects;				//�׿��ĸ�������
	};

	/*
	struct tagFixValues
	{		
	WORD wType;		
	long lFixVal1;
	long lFixVal2;
	};*/

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
	static map<DWORD, tagGoods2>	m_mapGoodsList;				// ԭʼ��Ʒ�б�(wIndex, tagGoods)
	static map<string, tagGoods2*>	m_mapGoodsListByName;		// ������Ϊ������ԭʼ��Ʒ�б�(strOrginName, *tagGoods)
	static map<DWORD/*��װID*/, tagSuitAttribute>	m_SuitList;	// ��װ�����Ա������������װ�ڸ��ּ��������¼������ܵ�����
	static vector<tagMoneyGraphics>	m_stMoneyGraphics;			// ��Ǯ��������Ϣ
	static map<string,DWORD>		m_mapEffects;				// ӳ����Ʒ���ƺ͸�������
	static map<string,DWORD>		m_mapAllEffects;			// ӳ�丽���������ƺ͸�������ö��ֵ
	static map<DWORD,string>		m_mapEffectReverse;			// ӳ����Ʒ�������Ժ�����
	static void InitMapEffects();								// ������Ʒ�������Ժ����Ƶľ���ӳ��

	static map<DWORD/*�׿���ID��*/,tagSeriesCardProperty>	m_mapSeriesCardList;	// �׿������б�

	//m_mapGoodsEffectValue;	// ��Ʒ�ĸ���������ֵ��ӳ��
public:
	//��ȡ����ӳ��map
	static map<string,DWORD>&			 GetMapEffects()							{return m_mapEffects;}
	//��ȡ��ת��ӳ��map
	static map<DWORD,string>&			 GetMapEffectReverse()						{return m_mapEffectReverse;}
	//��ȡ�����������͸�������ֵ��ӳ���
	static map<string,DWORD>&		     GetMapAllEffect()							{return m_mapAllEffects;}
	//��ȡ���е���װ����Ϣ�б�
	static map<DWORD, tagSuitAttribute>& GetSuitList()								{return m_SuitList;}
	//��ȡ��ID����װ���ܼ���
	static BYTE							 GetSuitCount(DWORD SuitID);
	//��ȡ��ID����װ��ԭʼ���б�
	static vector<string>*				 GetSuitOriginNameList(DWORD SuitID)		{return &(m_SuitList[SuitID].Suit_OriginName);}
	//��ȡ��ID����װ��������Ϣ�б�
	static map<DWORD,map<DWORD,int>>&	 GetSuitAttributeInfor(DWORD SuitID)		{return m_SuitList[SuitID].Suit_ActiveProperty;}
	//��ȡ��ID����װ��Ʒ����������(nWhich = 1 ȡ��װ������Ʒ��ĳ������ֵ1�� = 2ȡ����ֵ2��= 3ȡ����ֵ3���������Ǽ�����������ﵽ����װ��Ʒ������)
	static long GetSuitAttrOfGoods(DWORD dwIndex,DWORD wType,int nWhich);

	/////////////////�׿����Խӿ�//////////////
	//��ȡ���е��׿���Ϣ
	static map<DWORD/*�׿���ID��*/,tagSeriesCardProperty>& GetSeriesCardList()		{return m_mapSeriesCardList;}
	//��ȡĳ���׿�
	static tagSeriesCardProperty& GetSeriesCardProperty(DWORD index)				{return m_mapSeriesCardList[index];}
	//��ȡĳ���׿��ĸ��������б�
	static map<DWORD,int>& GetSeriesCardEffectList(DWORD index)						{return m_mapSeriesCardList[index].CardsEffects;}
	//��ȡĳ���׿��Ŀ�ƬID�б�
	static vector<DWORD>&  GetSeriesCardIDList(DWORD index)							{return m_mapSeriesCardList[index].CardsID;}
	//��ȡĳ���׿��ľ�������
	static BYTE GetSeriesCardRequireNum(DWORD index)								{return m_mapSeriesCardList[index].CARD_NUM_REQUIRE;}
	//����ĳ����Ƭ��ID���Ҵ����׿���ID
	static DWORD GetSeriesCardIndexByCardID(DWORD cardIndex);
	///////////////////////////////////////////

	static BOOL LoadGoodsList(const char* filename);			// ���ļ���ȡԭʼ��Ʒ�б�
	static void	LoadSuitList(const char* filename);				// ���ļ���ȡԭʼ��װ�б�

	static void LoadCardList(const char* filename);				// ���ļ���ȡ�׿��б���Ϣ

	static tagGoods2*	GetProperty(DWORD dwIndex)				{return (m_mapGoodsList.find(dwIndex)!=m_mapGoodsList.end())?(&m_mapGoodsList[dwIndex]):NULL;}
	static tagGoods2*	GetProperty(const char* strOrginName)
	{
		if(NULL == strOrginName)	return NULL;
		return (m_mapGoodsListByName.find(strOrginName)!=m_mapGoodsListByName.end())?m_mapGoodsListByName[strOrginName]:NULL;
	}
	static long GetEffectVal(DWORD dwIndex,WORD wType,int nWhich);
	static tagEffect* GetEffectTag(DWORD dwIndex,WORD wType);
	static bool InitMoneyGraphics(const char* filename);		// ���ļ���ȡ��Ǯ��������Ϣ
	static long GetMoneyGraphics(long lMoneyNum);
	static tagGoods2*	GetPropertyByEquipID(DWORD dwEquipID);	// ����װ����װ��Ż�ȡ��Ʒ����
	static bool FillGoodsInfo(DWORD dwIndex,CGoods *pGood);		// ����������Ʒ��Ϣ������Ʒ��
};
