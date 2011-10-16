/*
*	file:		GoodsExSetup.h
*	Brief:		��Ʒ��չ���ܣ��ֽ�/�ϳɡ��������ܡ�����/���ᣩ����
*	detail:		��ȡ�����ļ����ṩ���÷��ʽӿ�
*	Authtor:	�ſ���
*	Datae:		2007-12-10
*/


//! ������������������������������������������������������
//! ע�⣺ ������ڰ������ļ�ǰ���趨��USE_IN_SERVER��


#pragma once

#include <set>
#include "GoodsExDef.h"
#include "basedef.h"


class GoodsExSetup
{



	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! �ṹ����
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:

	//! ------------------------------�ֽ��䷽------------------------------
	struct tagDisassemblePrescription
	{
		LONG	lOutGoodsQuality;												//! �ֽ���Ʒ��

		char	arr_szOutGoodsName[MAX_DISASSEMBLE_NUM][MAX_GOOODS_NAME_SIZE];	//! �ֽ����Ʒԭʼ��
#ifdef USE_IN_SERVER 
		//! ����������
		LONG	arr_lOutGoodsId[MAX_DISASSEMBLE_NUM];							//! ��Ӧ�ֽ����ID��������û�д�������ó���
#else
#endif //! USE_IN_SERVER


		//! �ͻ�������
		


		tagDisassemblePrescription(void){memset(this, 0, sizeof(tagDisassemblePrescription));}

	};


	//! ------------------------------�ϳ��䷽------------------------------
	struct  tagSynthesizePrescription
	{
		LONG lIndex;												//! �䷽����
		LONG lOutGoodsNum;											//! �ɺϳ���Ʒ����
		char szOutGoodsName[MAX_GOOODS_NAME_SIZE];					//! ���ϳ���ԭʼ����
		LONG arr_lStuffNum[MAX_STUFF_NUM];							//! ��������
		char sz_arr_StuffName[MAX_STUFF_NUM][MAX_GOOODS_NAME_SIZE];	//! ����ԭʼ��

#ifdef USE_IN_SERVER 
		//! ����������
		LONG lOutGoodsId;											//! ���ϳ���ID
		LONG arr_lStuffId[MAX_STUFF_NUM];							//! ����ID
		BOOL bHasPrimary;											//! �Ƿ���������
#else
		//! �ͻ�������
		
#endif //! USE_IN_SERVER
		
		tagSynthesizePrescription(void){memset(this, 0, sizeof(tagSynthesizePrescription));}
	
	};

	
	//! ------------------------------������Ʒ���ӹ���------------------------------

	//! ����ϵ��
	struct  tagAmendCoefficient
	{
		float	fAmendIT;						//! ����ħ������ʱ��
		float	fAmendMA;						//! ����ħ���˺�
		float	fAmendPA;						//! ������ħ���˺�
		float	fAmendMC;						//! ����ħ������
		float	fAmendPC;						//! ������ħ������
	};

	//! ����ϵ������
	enum eAmendCoefficientType
	{
		eACT_AmendIT,						//! ����ħ������ʱ��
		eACT_AmendMA,						//! ����ħ���˺�
		eACT_AmendPA,						//! ������ħ���˺�
		eACT_AmendMC,						//! ����ħ������
		eACT_AmendPC,						//! ������ħ������
	};

	//! ��������
	struct  tagPowerAccessorial
	{
		LONG				lIntensity;			//! ����ǿ��
		LONG				lMExpend;			//! ħ�����ĳ���ֵ
		LONG				lPExpend;			//! ��ħ�����ĳ���ֵ
		tagAmendCoefficient AmendCoefficient;

		tagPowerAccessorial(void){memset(this, 0, sizeof(tagPowerAccessorial));}
	};

	//! һ���ȼ���Χ�ĳ�������
	struct  tagLevelBoundPA
	{
		LONG lMinLevel;											//! �ȼ�������
		LONG lMaxLevel;											//! �ȼ�������
		map<LONG, tagPowerAccessorial> mapPowerAccessorial;		//! �Գ���ǿ��Ϊkey����������
	};
	

	//! ------------------------------װ����������------------------------------


	//! �����������ýṹ
	struct tagUpgradeSet
	{
		LONG		lOdds;								//! �ɹ�����
		LONG		lFailedOperate;						//! ʧ��ʱ�Ĵ���취��ö��ֵ��
		LONG		lOperateNum;						//! ʧ��ʱ�Ĳ�����
		LONG		ArmLevel;							//! ���õ������ȼ�	
		list<string> listStuffName;						//! ��������ԭʼ��
#ifdef USE_IN_SERVER 
		//! ����������
		list<long>	listStuffIndex;						//! �������ϵ���Ʒ����
#else
		//! �ͻ�������
		
#endif //! USE_IN_SERVER
		tagUpgradeSet(void)
			:lOdds(0)
			,lFailedOperate(0)
			,lOperateNum(0)
			,ArmLevel(0)
		{}

	};

	//! ������������
	struct tagExtraStuff
	{
		LONG		lMaxUseCount;						//! ÿ������װ�����ʹ������
		LONG		lMinGoodsLevel;						//! ʹ�øñ�ʯ�ĳ����Ʒ����С��ȵȼ�
		LONG		lAddOdds;							//! ʹ�ú����ӵĳɹ����ʰٷ���
		LONG		lFailedMinishLevel;					//! ʹ�ú󣬳��ʧ��ʱ����Ʒ���͵ĳ�ȼ���
		list<string> listStuffName;						//! ��������ԭʼ��
#ifdef USE_IN_SERVER
		//! ����������
		list<long>	listStuffIndex;						//! �������ϵ���Ʒ����
#else
		//! �ͻ�������

#endif //! USE_IN_SERVER
		tagExtraStuff(void)
			:lMaxUseCount(0)
			,lMinGoodsLevel(0)
			,lAddOdds(0)
			,lFailedMinishLevel(0)
		{}
	};

	

	



	//! ------------------------------װ��ħ������------------------------------

	//! 
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
		LONG		lFour2One;							//! 4��1�ļ���
		LONG		lThree2One;							//! 3��1�ļ���
		LONG		lTwo2One;							//! 2��1�ļ���
		LONG		lOne2One;							//! 1��1�ļ���
#ifdef USE_IN_SERVER 
		//! ����������
		LONG		lStuffIndex;						//! ���ϵ���Ʒ����
		LONG		lOutIndex;							//! �������Ʒ����
#else
		//! �ͻ�������

#endif //! USE_IN_SERVER

		tagStuffSynthesize(void){memset(this, 0, sizeof(tagStuffSynthesize));}

		// ��ȡ���������ļ���
		LONG	GetOdds( eEvilStuffOperation eso )
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
		LONG		lCurrEvilLevel;						//! װ����ǰ��ħ���ȼ�
		LONG		lNeedMoney;							//! ��Ҫ�Ľ�Ǯ
#ifdef USE_IN_SERVER 
		//! ����������
		LONG		lStuffIndex;						//! ��Ҫ��ħ�����ϵ���Ʒ����
#else
		//! �ͻ�������

#endif //! USE_IN_SERVER

		tagPlusEvilStuff(void){memset(this, 0, sizeof(tagPlusEvilStuff));}
	};


	//! װ��������ص�����
	struct tagTypeWith
	{
		LONG lArmType;								//! װ������
		LONG lStuffNum;								//! ÿ��ħ����Ҫ�Ĳ�������
		LONG lBackoutGainStuffNum;					//! ������õ�ͬ�ȼ�������
		bool bAmendPA;								//! ��װ���Ƿ񱻼ӳ�����������
		bool bAmendMA;								//! ��װ���Ƿ񱻼ӳ�ħ����������
		bool bAmendPR;								//! ��װ���Ƿ񱻼ӳ������������
		bool bAmendMR;								//! ��װ���Ƿ񱻼ӳ�ħ����������

		tagTypeWith(void){memset(this, 0, sizeof(tagTypeWith));}
	};






	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! �����ʼ��
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	GoodsExSetup(void);
	~GoodsExSetup(void);

public:
	//!										��XML�����ļ��ж�ȡ���ã�����Ԫ�����ƴ���ᵼ���׳��쳣��
	//!										��������ֵ��
	//!										0	= �ɹ���
	//!										1	= GoodsExSetup���ö�ȡʧ�ܣ�������ļ���
	//!										2	= ���ڵ����Ʋ��ǡ�GoodsExSetup�����ڵ��ȡʧ��
	//!										3	= ��ȡ���ڵ�ָ��ʧ��
	//!										11	= ��ȡ�ֽ��䷽ʧ��
	//!										21	= ��ȡ�ϳ��䷽ʧ��
	//!										31	= ��ȡ������������ʧ��
	LONG									ReadSetup(const char* fileName);

#ifdef USE_IN_SERVER 
	//! ����������

	//!										��ʼ������������
	bool									InitServerSetup(void);

	//!										����
	bool									Serialize(vector<BYTE>* pStream);

	//!										����
	bool									Unserialize(BYTE* pStream, LONG& lOffset);
	
#else
	//! �ͻ��˷���

#endif //! USE_IN_SERVER





	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ����ʹ��
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//! �ֽ⡢�ϳ�
	//!-------------------------------------------------------------------------------------

	//!										ͨ����ƷƷ�ʣ�lQuality����ȡ�ֽ��䷽
	const	tagDisassemblePrescription*		GetDisassemblePrescription(LONG lQuality);
	//!										����ֽ�������
	//!											lValue1����Ʒ�ֽ��ֵ1
	//!											lValue2����Ʒ�ֽ��ֵ1
	//!											lQuality����ƷƷ��
	//!											lOutIdx���ڼ��ֲ���
	LONG									GetDisassembleNum(LONG lValue1, LONG lValue2, LONG lQuality, LONG lOutIdx);

	//!										ͨ���䷽������lPrescriptionIndex����ȡ�ϳ��䷽
	const	tagSynthesizePrescription*		GetSynthesizePrescription(LONG lPrescriptionIndex);


	//! ð�����򣨴Ӵ�С��
	static	void	BubbleSort(LONG lSrcSize, ULONG srcKey[], ULONG srcValue[]);


	//! ��������
	//!-------------------------------------------------------------------------------------
	//!										�õ������������ٷֱ�
	LONG									GetReinforceMaxPercent(void);
	//!										ͨ�������ȼ���lLevel���ͳ���ǿ�ȣ�lIntensity�����һ����������
	const	tagPowerAccessorial*			GetPowerAccessorial(LONG lLevel, LONG lIntensity);


	//! �󶨶���
	//!-------------------------------------------------------------------------------------
	//!										���԰�������Ʒ�����Ƿ���ȷ
	bool									TestBindGoodsIndex(LONG lIndex);
	//!										���Խ��������Ʒ�����Ƿ���ȷ
	bool									TestUntieGoodsIndex(LONG lIndex);
	//!										���Զ���������Ʒ�����Ƿ���ȷ
	bool									TestFrostGoodsIndex(LONG lIndex);
	//!										���Խⶳ������Ʒ�����Ƿ���ȷ
	bool									TestThawGoodsIndex(LONG lIndex);

	//!										�õ�������Ʒ��������
	void									GetBindGoodsIndex(vector<LONG> &vecReIdx);
	//!										�õ�������Ʒ��������
	void									GetUntieGoodsIndex(vector<LONG> &vecReIdx);
	//!										�õ�������Ʒ��������
	void									GetFrostGoodsIndex(vector<LONG> &vecReIdx);
	//!										�õ�������Ʒ��������
	void									GetThawGoodsIndex(vector<LONG> &vecReIdx);


	//! ���
	//!-------------------------------------------------------------------------------------
	//!										ͨ��װ�����Ƶȼ���װ���ȼ����õ�����䷽
	const	tagUpgradeSet*					GetUpgradeSet(LONG lLimitLevel, LONG lArmLevel);

	//!										ͨ��װ���ȼ���װ�����͡�װ�������ͻ����Ҫ�Ľ�Ǯ
	LONG									GetUpgradeMoney(LONG lLimitLevel, LONG lEquipmentType, LONG lChildType);

	//!										�õ�������Ʒ�Ĺ�������
	const	tagExtraStuff*					GetExtraStuffIni(const char* pGoodsName);

	//!										�õ����ȼ�����
	LONG									GetGoodsMaxLevel(void);

	//!										�õ�����3D��Ч��bSucceed �Ƿ�ɹ���lResultCode �ɹ�ʧ�ܺ�� ��ʶ
	long									GetUpgrade3DEffectID( BOOL bSucceed, long lResultCode = 0);

#ifdef USE_IN_SERVER 
	const	tagExtraStuff*					GetExtraStuffIni(LONG lGoodsIndex);
	
#endif

								
	

	//! ħ��
	//!-------------------------------------------------------------------------------------
	
	//! �õ�ħ��Ч��ֵ
	bool GetElementValue(long lGoodsLevel, long lElementLevel, tagElementValue &RetagElementValue);

	//! �õ�ħ����������
	typedef const char*	LPReStuffName;
	bool GetElement(bool bIsAtk, long lGoodsLevel, long lElementLevel, long lElementType, LPReStuffName &pReStuffName, long &lNeedNum, long &lSucceedOdds);

	//! �õ�ħ���ĳ�ʼ��������
	DWORD GetElement(bool bIsAtk, long lGoodsLevel, LPReStuffName pStuffName, long &lNeedNum, long &lSucceedOdds);

	//! �õ�������ʯ���ӵĳɹ����ʣ����֮����ʧ�ܷ��ظ���
	long GetHelpfulStuffOdds(long lElementLevel, LPReStuffName pStuffName);

	//! �õ�ħ��Ԫ�����õĲ��ϼ�������
	const tagResetStuff* GetResetElement(long lGoodsLevel, LPReStuffName pStuffName);
	//! �õ�ħ���������õĲ��ϼ�������
	typedef const tagXiangXingOdds* LPCXiangXingOdds;
	const tagResetStuff* GetResetXiangXing(long lGoodsLevel, LPReStuffName pStuffName, LPCXiangXingOdds &pXiangXingOdds);
    //! ��õ�ǰǿ���ȼ���ߵĸ�ħ�ȼ�
	long GetMaxElementLevelByUpgrade(long Upgrade);
	

#ifdef USE_IN_SERVER 
	

#else //! �ͻ��˷���
	

	//!				�ҵ�һ���ϳ��䷽����
	LONG			FindSynthesizePrescriptionIdx(
						char* arr_pStuffName[MAX_STUFF_NUM],
						LONG arr_lStuffNum[MAX_STUFF_NUM]);

	//!  ͨ��ħ���ȼ����ظ�����ʯ��ԭʼ��
	const char*  GetHelpfulStuffName(long lElementLevel);
private:
	//! ˽�����򷽷�
	static	void	MeBubbleSort(char srcKey[MAX_STUFF_NUM][MAX_GOOODS_NAME_SIZE], LONG srcValue[MAX_STUFF_NUM]);

#endif //! USE_IN_SERVER






	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ˽�г�Ա
	///////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//!										�ֽ��䷽�б�
	map<LONG, tagDisassemblePrescription>	m_mapDisassemblePrescription;
	//!										�ϳ���ӳ�䣬key=�䷽��ţ� value=�䷽
	map<LONG, tagSynthesizePrescription>	m_mapSynthesizePrescription;

	//!										��Ʒ�������ٷֱ�
	LONG									m_lReinforceMaxPercent;
	//!										��������
	vector<tagLevelBoundPA>					m_vecPowerIni;


	//!										����Ʒ�����б�
	set<LONG>								m_setBindGoodsIndex;
	//!										�����Ʒ�����б�
	set<LONG>								m_setUntieGoodsIndex;
	//!										������Ʒ�����б�
	set<LONG>								m_setFrostGoodsIndex;
	//!										�ⶳ��Ʒ�����б�
	set<LONG>								m_setThawGoodsIndex;


	
	//! װ���������ö���
	struct tagArmUpgradeSet
	{
		//!							��С���Ƶȼ�
		LONG						lMinLimitLevel;
		//!							������Ƶȼ�
		LONG						lMaxLimitLevel;
		//!							����key=װ����ǰ�ȼ�
		map<LONG, tagUpgradeSet>	mapUpgradeSet;
	};

	//! װ��������Ǯ���ö���
	struct tagArmUpgradeMoneySet
	{
		//!							��С���Ƶȼ�
		LONG						lMinLimitLevel;
		//!							������Ƶȼ�
		LONG						lMaxLimitLevel;
		//!							��Ǯ����key=װ��id��H16װ������ + L16�����ͣ�
		map<LONG, LONG>				mapUpgradeMoneySet;
	};

	//!										װ����������
	vector<tagArmUpgradeSet>				m_vecUpgradeSetList;
	//!										װ����������
	vector<tagArmUpgradeMoneySet>			m_vecUpgradeMoneySetList;
	//!										������Ʒ����
	vector<tagExtraStuff>					m_vecExtraStuff;
	//!										װ�����������ȼ�����
	LONG									m_lUpgradeLimit;
	
	//!										�µ�ħ������
	tagElementSetup							m_ElementSetup;

	//!                                     ǿ���ȼ���Ӧ��ħ�ȼ�
	map<long,long>                   mapUpgradeToElementLevel;               
};