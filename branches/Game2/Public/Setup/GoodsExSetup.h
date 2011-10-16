//======================================================================================
/**
*	@file		GoodsExSetup.h
*	@data		2007-12-10
*	@author		�ſ���
*/
//======================================================================================
#pragma once

#include "../Common/BaseDef.h"
#include "../Common/GoodsDef.h"

/**
*	@class GoodsExSetup
*
*	@brief ��Ʒ��չ���ܣ��ֽ�/�ϳɡ��������ܡ�����/���ᣩ����
*/
class GoodsExSetup
{
public:

	/// �ֽ��䷽
	struct tagDisassemblePrescription
	{
		long	lOutGoodsQuality;												//! �ֽ���Ʒ��
		char	arr_szOutGoodsName[MAX_DISASSEMBLE_NUM][MAX_GOOODS_NAME_SIZE];	//! �ֽ����Ʒԭʼ��
		long	arr_lOutGoodsId[MAX_DISASSEMBLE_NUM];							//! ��Ӧ�ֽ����ID��������û�д�������ó���

		tagDisassemblePrescription(void){memset(this, 0, sizeof(tagDisassemblePrescription));}
	};


	/// �ϳ��䷽
	struct  tagSynthesizePrescription
	{
		long lIndex;												//! �䷽����
		long lOutGoodsNum;											//! �ɺϳ���Ʒ����
		char szOutGoodsName[MAX_GOOODS_NAME_SIZE];					//! ���ϳ���ԭʼ����
		long arr_lStuffNum[MAX_STUFF_NUM];							//! ��������
		char sz_arr_StuffName[MAX_STUFF_NUM][MAX_GOOODS_NAME_SIZE];	//! ����ԭʼ��
		long lOutGoodsId;											//! ���ϳ���ID
		long arr_lStuffId[MAX_STUFF_NUM];							//! ����ID
		bool bHasPrimary;											//! �Ƿ���������
		
		tagSynthesizePrescription(void){memset(this, 0, sizeof(tagSynthesizePrescription));}
	};

	
	//! ------------------------------������Ʒ���ӹ���------------------------------

	/// ����ϵ��
	struct  tagAmendCoefficient
	{
		float	fAmendIT;						//! ����ħ������ʱ��
		float	fAmendMA;						//! ����ħ���˺�
		float	fAmendPA;						//! ������ħ���˺�
		float	fAmendMC;						//! ����ħ������
		float	fAmendPC;						//! ������ħ������
	};

	/// ����ϵ������
	enum eAmendCoefficientType
	{
		eACT_AmendIT,						//! ����ħ������ʱ��
		eACT_AmendMA,						//! ����ħ���˺�
		eACT_AmendPA,						//! ������ħ���˺�
		eACT_AmendMC,						//! ����ħ������
		eACT_AmendPC,						//! ������ħ������
	};

	/// ��������
	struct  tagPowerAccessorial
	{
		long				lIntensity;			//! ����ǿ��
		long				lMExpend;			//! ħ�����ĳ���ֵ
		long				lPExpend;			//! ��ħ�����ĳ���ֵ
		tagAmendCoefficient AmendCoefficient;

		tagPowerAccessorial(void){memset(this, 0, sizeof(tagPowerAccessorial));}
	};

	/// һ���ȼ���Χ�ĳ�������
	struct  tagLevelBoundPA
	{
		long lMinLevel;											//! �ȼ�������
		long lMaxLevel;											//! �ȼ�������
		map<long, tagPowerAccessorial> mapPowerAccessorial;		//! �Գ���ǿ��Ϊkey����������
	};
	

	//! ------------------------------װ����������------------------------------


	/// �����������ýṹ
	struct tagUpgradeSet
	{
		long		lOdds;								//! �ɹ�����
		long		lFailedOperate;						//! ʧ��ʱ�Ĵ���취��ö��ֵ��
		long		lOperateNum;						//! ʧ��ʱ�Ĳ�����
		long		ArmLevel;							//! ���õ������ȼ�
		char		szStuffName[MAX_GOOODS_NAME_SIZE];	//! ��������ԭʼ��
		long		lStuffIndex;						//! �������ϵ���Ʒ����

		tagUpgradeSet(void){memset(this, 0, sizeof(tagUpgradeSet));}
	};

	/// ������������
	struct tagExtraStuff
	{
		char		szStuffName[MAX_GOOODS_NAME_SIZE];	//! ��������ԭʼ��
		long		lMaxUseCount;						//! ÿ������װ�����ʹ������
		long		lMinGoodsLevel;						//! ʹ�øñ�ʯ�ĳ����Ʒ����С��ȵȼ�
		long		lAddOdds;							//! ʹ�ú����ӵĳɹ����ʰٷ���
		long		lFailedMinishLevel;					//! ʹ�ú󣬳��ʧ��ʱ����Ʒ���͵ĳ�ȼ���
		long		lStuffIndex;						//! ���ϵ���Ʒ����

		tagExtraStuff(void){memset(this, 0, sizeof(tagExtraStuff));}
	};

	//! ------------------------------װ��ħ������------------------------------

	/// װ���ȼ���Χ���� 
	struct tagEquipmentLevelBound
	{
		long 					lMinLevel;
		long 					lMaxLevel;
		long 					lGetEffect;
		long 					lConsumeEffect;
	};
	//! 
	struct tagNeedNum
	{
		long 					lElementMinLevel;
		long 					lElementMaxLevel;
		long 					lStuffNumA;
		long 					lStuffNumD;
		//!						������ļ���
		long 					lSucceedOdds;
	};

	struct tagElementStuff
	{
		long 					lMinLevel;
		long 					lMaxLevel;
		//!
		char					szStuffNameA[MT_Max][MAX_GOOODS_NAME_SIZE];
		//!
		char					szStuffNameD[MT_Max][MAX_GOOODS_NAME_SIZE];
	};
	//! ħ������
	struct tagElementBound
	{
		vector<tagElementStuff> vElementStuff;
		//!
		vector<tagNeedNum>		vNeedNum;
	};
	//! �ɹ��ʸ�������
	struct tagHelpfulStuff
	{
		char					szStuffName[MAX_GOOODS_NAME_SIZE];
		//!						������ļ���
		long					lOdds;
	};
	//! �ɹ��ʸ�������
	struct tagHelpfulSetup
	{
		long 					lMinLevel;
		long 					lMaxLevel;
		vector<tagHelpfulStuff>	vHelpfulStuff;
	};
	//! ���ò���
	struct tagResetStuff
	{
		char					szStuffName[MAX_GOOODS_NAME_SIZE];
		//!						�ٷ����ļ���
		long					lOdds[MT_Max];
	};
	//!	Ԫ����������
	struct tagResetElement
	{
		long 					lMinLevel;
		long 					lMaxLevel;
		vector<tagResetStuff>	vResetStuff;
	};
	//!
	struct tagXiangXingOdds
	{
		//!						�ٷ����ļ���
		long					lOdds[eXXV_NUM];
	};
	//! ������������
	struct tagResetXiangXing
	{
		long 					lMinLevel;
		long 					lMaxLevel;
		tagXiangXingOdds		XiangXingOdds;
		vector<tagResetStuff>	vResetStuff;
	};
	//!
	struct tagElementValue
	{
		//!						����ֵ
		long					lAtkValue;
		//!						����ֵ
		long					lDefValue;
		//!						��������ֵ
		long					lAffixValue;

		tagElementValue(void){memset(this, 0, sizeof(tagElementValue));}
	};
	//! 
	struct tagElementSetup
	{
		//!									װ���ȼ���Χ����
		vector<tagEquipmentLevelBound>		vEquipmentLevelBound;
		//!									ħ����������
		vector<tagElementBound>				vElementBound;
		//!									���ʸ�����������
		vector<tagHelpfulSetup>				vHelpfulSetup;
		//!									ħ���ȼ���Ӧħ������ֵ
		vector<tagElementValue>				vElementValue;
		
		//!									Ԫ����������
		vector<tagResetElement>				vResetElement;
		//!									������������
		vector<tagResetXiangXing>			vResetXiangXing;
	};

	///////////////////////////////////old
	//! ���Ϻϳɼ��ʽṹ
	struct tagStuffSynthesize
	{
		char		szStuffName[MAX_GOOODS_NAME_SIZE];	//! ���ϵ�ԭʼ��
		char		szOutName[MAX_GOOODS_NAME_SIZE];	//! �����ԭʼ��
		long		lFour2One;							//! 4��1�ļ���
		long		lThree2One;							//! 3��1�ļ���
		long		lTwo2One;							//! 2��1�ļ���
		long		lOne2One;							//! 1��1�ļ���
		long		lStuffIndex;						//! ���ϵ���Ʒ����
		long		lOutIndex;							//! �������Ʒ����

		tagStuffSynthesize(void){memset(this, 0, sizeof(tagStuffSynthesize));}

		// ��ȡ���������ļ���
		long	GetOdds( eEvilStuffOperation eso )
		{
			switch( eso )
			{
				case eESO_1in1:
					return	lOne2One;
				case eESO_2in1:
					return	lTwo2One;
				case eESO_3in1:
					return	lThree2One;
				case eESO_4in1:
					return	lFour2One;
			}
			return 0;
		}
	};


	//! ħ����������
	struct tagPlusEvilStuff
	{
		char		szStuffName[MAX_GOOODS_NAME_SIZE];	//! ��Ҫ��ħ�����ϵ�ԭʼ��
		long		lCurrEvilLevel;						//! װ����ǰ��ħ���ȼ�
		long		lNeedMoney;							//! ��Ҫ�Ľ�Ǯ
		long		lStuffIndex;						//! ��Ҫ��ħ�����ϵ���Ʒ����

		tagPlusEvilStuff(void){memset(this, 0, sizeof(tagPlusEvilStuff));}
	};


	//! װ��������ص�����
	struct tagTypeWith
	{
		long lArmType;								//! װ������
		long lStuffNum;								//! ÿ��ħ����Ҫ�Ĳ�������
		long lBackoutGainStuffNum;					//! ������õ�ͬ�ȼ�������
		bool bAmendPA;								//! ��װ���Ƿ񱻼ӳ�����������
		bool bAmendMA;								//! ��װ���Ƿ񱻼ӳ�ħ����������
		bool bAmendPR;								//! ��װ���Ƿ񱻼ӳ������������
		bool bAmendMR;								//! ��װ���Ƿ񱻼ӳ�ħ����������

		tagTypeWith(void){memset(this, 0, sizeof(tagTypeWith));}
	};

public:
	/// ����
	GoodsExSetup(void);

	/// ����
	~GoodsExSetup(void);

public:

	/*
	* ��XML�����ļ��ж�ȡ���ã�����Ԫ�����ƴ���ᵼ���׳��쳣��
	*
	* @return	0	= �ɹ���
	*			1	= GoodsExSetup���ö�ȡʧ�ܣ�������ļ���
	*			2	= ���ڵ����Ʋ��ǡ�GoodsExSetup�����ڵ��ȡʧ��
	*			3	= ��ȡ���ڵ�ָ��ʧ��
	*			11	= ��ȡ�ֽ��䷽ʧ��
	*			21	= ��ȡ�ϳ��䷽ʧ��
	*			31	= ��ȡ������������ʧ��		
	*/
	long ReadSetup(const char* fileName);

	/// ��ʼ������������
	bool InitServerSetup(void);

	/// ����
	bool Serialize(vector<uchar>* pStream);

	/// ����
	bool Unserialize(uchar* pStream, long& lOffset);

public:
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! �ֽ⡢�ϳ�
	///////////////////////////////////////////////////////////////////////////////////////////////////

	/// ͨ����ƷƷ�ʣ�lQuality����ȡ�ֽ��䷽
	const tagDisassemblePrescription* GetDisassemblePrescription(long lQuality);

	/**
	*  ����ֽ�������
	*
	*  @param lValue1����Ʒ�ֽ��ֵ1
	*  @param lValue2����Ʒ�ֽ��ֵ1
	*  @param lQuality����ƷƷ��
	*  @param lOutIdx���ڼ��ֲ���
	*/
	long GetDisassembleNum(long lValue1, long lValue2, long lQuality, long lOutIdx);

	/// ͨ���䷽������lPrescriptionIndex����ȡ�ϳ��䷽
	const tagSynthesizePrescription* GetSynthesizePrescription(long lPrescriptionIndex);


	/// ð�����򣨴Ӵ�С��
	static	void	BubbleSort(long lSrcSize, ULONG srcKey[], ULONG srcValue[]);


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ��������
	///////////////////////////////////////////////////////////////////////////////////////////////////

	/// �õ������������ٷֱ�
	long GetReinforceMaxPercent(void);

	/// ͨ�������ȼ���lLevel���ͳ���ǿ�ȣ�lIntensity�����һ����������
	const tagPowerAccessorial* GetPowerAccessorial(long lLevel, long lIntensity);


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! �󶨶���
	///////////////////////////////////////////////////////////////////////////////////////////////////

	/// ���԰�������Ʒ�����Ƿ���ȷ
	bool TestBindGoodsIndex(long lIndex);

	/// ���Խ��������Ʒ�����Ƿ���ȷ
	bool TestUntieGoodsIndex(long lIndex);

	/// ���Զ���������Ʒ�����Ƿ���ȷ
	bool									TestFrostGoodsIndex(long lIndex);
	//!										���Խⶳ������Ʒ�����Ƿ���ȷ
	bool									TestThawGoodsIndex(long lIndex);

	//!										�õ�������Ʒ��������
	void									GetBindGoodsIndex(vector<long> &vecReIdx);
	//!										�õ�������Ʒ��������
	void									GetUntieGoodsIndex(vector<long> &vecReIdx);
	//!										�õ�������Ʒ��������
	void									GetFrostGoodsIndex(vector<long> &vecReIdx);
	//!										�õ�������Ʒ��������
	void									GetThawGoodsIndex(vector<long> &vecReIdx);


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ���
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//!										ͨ��װ�����Ƶȼ���װ���ȼ����õ�����䷽
	const	tagUpgradeSet*					GetUpgradeSet(long lLimitLevel, long lArmLevel);

	//!										ͨ��װ���ȼ���װ�����͡�װ�������ͻ����Ҫ�Ľ�Ǯ
	long									GetUpgradeMoney(long lLimitLevel, long lEquipmentType, long lChildType);

	//!										�õ�������Ʒ�Ĺ�������
	const	tagExtraStuff*					GetExtraStuffIni(const char* pGoodsName);

	//!										�õ����ȼ�����
	long									GetGoodsMaxLevel(void);

	//!										�õ�����3D��Ч��bSucceed �Ƿ�ɹ���lResultCode �ɹ�ʧ�ܺ�� ��ʶ
	long									GetUpgrade3DEffectID( bool bSucceed, long lResultCode = 0);

	const	tagExtraStuff*					GetExtraStuffIni(long lGoodsIndex);
	


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ħ��
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	//! �õ�ħ��Ч��ֵ
	bool GetElementValue(long lGoodsLevel, long lElementLevel, tagElementValue &RetagElementValue);

	//! �õ�ħ����������
	typedef const char*	LPReStuffName;
	bool GetElement(bool bIsAtk, long lGoodsLevel, long lElementLevel, long lElementType, LPReStuffName &pReStuffName, long &lNeedNum, long &lSucceedOdds);

	//! �õ�ħ���ĳ�ʼ��������
	ulong GetElement(bool bIsAtk, long lGoodsLevel, LPReStuffName pStuffName, long &lNeedNum, long &lSucceedOdds);

	//! �õ�������ʯ���ӵĳɹ����ʣ����֮����ʧ�ܷ��ظ���
	long GetHelpfulStuffOdds(long lElementLevel, LPReStuffName pStuffName);

	//! �õ�ħ��Ԫ�����õĲ��ϼ�������
	const tagResetStuff* GetResetElement(long lGoodsLevel, LPReStuffName pStuffName);
	//! �õ�ħ���������õĲ��ϼ�������
	typedef const tagXiangXingOdds* LPCXiangXingOdds;
	const tagResetStuff* GetResetXiangXing(long lGoodsLevel, LPReStuffName pStuffName, LPCXiangXingOdds &pXiangXingOdds);
	

private:
	//!										�ֽ��䷽�б�
	map<long, tagDisassemblePrescription>	m_mapDisassemblePrescription;
	//!										�ϳ���ӳ�䣬key=�䷽��ţ� value=�䷽
	map<long, tagSynthesizePrescription>	m_mapSynthesizePrescription;

	//!										��Ʒ�������ٷֱ�
	long									m_lReinforceMaxPercent;
	//!										��������
	vector<tagLevelBoundPA>					m_vecPowerIni;


	//!										����Ʒ�����б�
	set<long>								m_setBindGoodsIndex;
	//!										�����Ʒ�����б�
	set<long>								m_setUntieGoodsIndex;
	//!										������Ʒ�����б�
	set<long>								m_setFrostGoodsIndex;
	//!										�ⶳ��Ʒ�����б�
	set<long>								m_setThawGoodsIndex;


	
	//! װ���������ö���
	struct tagArmUpgradeSet
	{
		//!							��С���Ƶȼ�
		long						lMinLimitLevel;
		//!							������Ƶȼ�
		long						lMaxLimitLevel;
		//!							����key=װ����ǰ�ȼ�
		map<long, tagUpgradeSet>	mapUpgradeSet;
	};

	//! װ��������Ǯ���ö���
	struct tagArmUpgradeMoneySet
	{
		//!							��С���Ƶȼ�
		long						lMinLimitLevel;
		//!							������Ƶȼ�
		long						lMaxLimitLevel;
		//!							��Ǯ����key=װ��id��H16װ������ + L16�����ͣ�
		map<long, long>				mapUpgradeMoneySet;
	};

	//!										װ����������
	vector<tagArmUpgradeSet>				m_vecUpgradeSetList;
	//!										װ����������
	vector<tagArmUpgradeMoneySet>			m_vecUpgradeMoneySetList;
	//!										������Ʒ����
	vector<tagExtraStuff>					m_vecExtraStuff;
	//!										װ�����������ȼ�����
	long									m_lUpgradeLimit;
	
	//!										�µ�ħ������
	tagElementSetup							m_ElementSetup;
};