#include "stdafx.h"
#include "GoodsDef.h"
#include "GoodsExSetup.h"
#include "tinystr.h"
#include "tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef USE_IN_SERVER 

#endif

GoodsExSetup::GoodsExSetup(void)
{

}
GoodsExSetup::~GoodsExSetup(void)
{

}

//! ��XML�����ļ��ж�ȡ����
LONG GoodsExSetup::ReadSetup(const char* fileName)
{
	//! ----------------------------���ڵ� �������ڵĴ���Ӧ����1~10----------------------------
	TiXmlNode *pNode = NULL; 		

	CRFile *prfile = rfOpen(fileName);
	if(NULL == prfile)
	{
		//���������Ϣ
		return 1;
	}

	TiXmlDocument m_Tdoc(fileName);
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		rfClose(prfile);
		return 1;
	}
	rfClose(prfile);

	pNode=m_Tdoc.FirstChild("GoodsExSetup");
	if (pNode==NULL) return 2;

	TiXmlElement* pGoodsExSetup = pNode->ToElement();//��ȡnode ��ָ��
	if (pGoodsExSetup==NULL) return 3;

	//! ----------------------------�ֽ��䷽ �������ڵĴ���Ӧ����11~20----------------------------
	m_mapDisassemblePrescription.clear(); //! ����ϵ��䷽

	TiXmlElement* pDisassemblePrescription = pGoodsExSetup->FirstChildElement("DisassemblePrescription");
	if (NULL == pDisassemblePrescription) return 11;
	
	{
		TiXmlElement* pPrescription = pDisassemblePrescription->FirstChildElement("Prescription");
		for (; pPrescription!=NULL; pPrescription = pPrescription->NextSiblingElement())
		{
			tagDisassemblePrescription DisassemblePrescription;
			DisassemblePrescription.lOutGoodsQuality = atoi(pPrescription->Attribute("Quality"));

			TiXmlElement* pOutGoods = pPrescription->FirstChildElement("OutGoods");
			for (LONG i = 0; pOutGoods!=NULL; pOutGoods = pOutGoods->NextSiblingElement("OutGoods"))
			{
				if(MAX_DISASSEMBLE_NUM > i)
				{	
					const char *pOutGoodsName = pOutGoods->Attribute("Name");
					LONG lSize = (strlen(pOutGoodsName) + 1 > MAX_GOOODS_NAME_SIZE) ? MAX_GOOODS_NAME_SIZE : strlen(pOutGoodsName) + 1;
					memmove(DisassemblePrescription.arr_szOutGoodsName[i], pOutGoodsName, lSize);

					++i;
				}
				else
					break;
			}

			if(m_mapDisassemblePrescription.end() != m_mapDisassemblePrescription.find(DisassemblePrescription.lOutGoodsQuality))
			{
				continue;
			}
			m_mapDisassemblePrescription[DisassemblePrescription.lOutGoodsQuality] = DisassemblePrescription;
		}
	}
	


	//! ----------------------------�ϳ��䷽ �������ڵĴ���Ӧ����21~30----------------------------
	m_mapSynthesizePrescription.clear(); //! ����ϵ��䷽

	TiXmlElement* pSynthesizePrescription = pGoodsExSetup->FirstChildElement("SynthesizePrescription");
	if (NULL == pSynthesizePrescription) return 21;
	
	TiXmlElement* pPrescription = pSynthesizePrescription->FirstChildElement("Prescription");
	for (; pPrescription!=NULL; pPrescription = pPrescription->NextSiblingElement())
	{
		tagSynthesizePrescription SynthesizePrescription;

		SynthesizePrescription.lIndex		= atoi(pPrescription->Attribute("Index"));
		SynthesizePrescription.lOutGoodsNum = atoi(pPrescription->Attribute("OutGoodsNum"));

		const char* pOutGoodsName = pPrescription->Attribute("OutGoodsName");
		memmove(SynthesizePrescription.szOutGoodsName, pOutGoodsName, strlen(pOutGoodsName) + 1);

#ifdef USE_IN_SERVER
		SynthesizePrescription.bHasPrimary = (0 != atoi(pPrescription->Attribute("HasPrimaryStuff")));
#endif

		TiXmlElement* pStuff = pPrescription->FirstChildElement("Stuff");
		LONG lStuffIndex = 0;
		for (; pStuff!=NULL; pStuff = pStuff->NextSiblingElement())
		{
			SynthesizePrescription.arr_lStuffNum[lStuffIndex] = atoi(pStuff->Attribute("Num"));

			const char* pName = pStuff->Attribute("Name");
			memmove(SynthesizePrescription.sz_arr_StuffName[lStuffIndex], pName, strlen(pName) + 1);

			if (MAX_STUFF_NUM <= lStuffIndex) break;
			lStuffIndex++;
		}
#ifndef USE_IN_SERVER
		MeBubbleSort(SynthesizePrescription.sz_arr_StuffName, SynthesizePrescription.arr_lStuffNum);
#endif
		m_mapSynthesizePrescription[SynthesizePrescription.lIndex] = SynthesizePrescription;
	}


	//! ----------------------------�������� �������ڵĴ���Ӧ����31~40----------------------------
	TiXmlElement* pReinforce_MaxPercent = pGoodsExSetup->FirstChildElement("Reinforce_MaxPercent");
	if (NULL == pReinforce_MaxPercent) return 31;
	m_lReinforceMaxPercent = atoi(pReinforce_MaxPercent->Attribute("Value"));

	m_vecPowerIni.clear();

	TiXmlElement* pPowerReinforce = pGoodsExSetup->FirstChildElement("PowerReinforce");
	if (NULL == pPowerReinforce) return 32;

	TiXmlElement* pLevelBound = pPowerReinforce->FirstChildElement("LevelBound");
	for (; pLevelBound!=NULL; pLevelBound = pLevelBound->NextSiblingElement())
	{
		//! �õ��ȼ���Χ
		tagLevelBoundPA LevelBoundPA;
		LevelBoundPA.lMinLevel = atoi(pLevelBound->Attribute("MinLevel"));
		LevelBoundPA.lMaxLevel = atoi(pLevelBound->Attribute("MaxLevel"));
		
		//! �õ�����
		TiXmlElement* pAccessorial = pLevelBound->FirstChildElement("Accessorial");
		for (; pAccessorial!=NULL; pAccessorial = pAccessorial->NextSiblingElement())
		{
			tagPowerAccessorial PowerAccessorial;
			PowerAccessorial.lIntensity = atoi(pAccessorial->Attribute("Intensity"));		//! ����ǿ��
			PowerAccessorial.AmendCoefficient.fAmendIT = atoi(pAccessorial->Attribute("AmendIT")) / 100.0f;	//! ����ħ������ʱ��
			PowerAccessorial.AmendCoefficient.fAmendMA = atoi(pAccessorial->Attribute("AmendMA")) / 100.0f;	//! ����ħ���˺�
			PowerAccessorial.AmendCoefficient.fAmendPA = atoi(pAccessorial->Attribute("AmendPA")) / 100.0f;	//! ������ħ���˺�
			PowerAccessorial.AmendCoefficient.fAmendMC = atoi(pAccessorial->Attribute("AmendMC")) / 100.0f;	//! ����ħ������
			PowerAccessorial.AmendCoefficient.fAmendPC = atoi(pAccessorial->Attribute("AmendPC")) / 100.0f;	//! ������ħ������
			PowerAccessorial.lMExpend = atoi(pAccessorial->Attribute("MExpend"));			//! ħ�����ĳ���ֵ
			PowerAccessorial.lPExpend = atoi(pAccessorial->Attribute("PExpend"));			//! ��ħ�����ĳ���ֵ
			
			if (LevelBoundPA.mapPowerAccessorial.end() == LevelBoundPA.mapPowerAccessorial.find(PowerAccessorial.lIntensity))
			{
				LevelBoundPA.mapPowerAccessorial[PowerAccessorial.lIntensity] = PowerAccessorial;
			}
			else
			{
				return 35;
			}
		}

		m_vecPowerIni.push_back(LevelBoundPA);
	}

	//! ----------------------------�󶨶��� �������ڵĴ���Ӧ����41~50----------------------------

	//! ����Ʒ�����б�
	TiXmlElement* pBind = pGoodsExSetup->FirstChildElement("Bind");
	if (NULL == pBind) return 41;

	for (TiXmlElement* pGoods = pBind->FirstChildElement("Goods"); pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
	{
		LONG lGoodsIndex = atoi(pGoods->Attribute("Index"));
		m_setBindGoodsIndex.insert(lGoodsIndex);
	}

	//! �����Ʒ�����б�
	TiXmlElement* pUntie = pGoodsExSetup->FirstChildElement("Untie");
	if (NULL == pUntie) return 42;

	for (TiXmlElement* pGoods = pUntie->FirstChildElement("Goods"); pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
	{
		LONG lGoodsIndex = atoi(pGoods->Attribute("Index"));
		m_setUntieGoodsIndex.insert(lGoodsIndex);
	}

	//! ������Ʒ�����б�
	TiXmlElement* pFrost = pGoodsExSetup->FirstChildElement("Frost");
	if (NULL == pFrost) return 42;

	for (TiXmlElement* pGoods = pFrost->FirstChildElement("Goods"); pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
	{
		LONG lGoodsIndex = atoi(pGoods->Attribute("Index"));
		m_setFrostGoodsIndex.insert(lGoodsIndex);
	}
	
	//! �ⶳ��Ʒ�����б�
	TiXmlElement* pThaw = pGoodsExSetup->FirstChildElement("Thaw");
	if (NULL == pThaw) return 42;

	for (TiXmlElement* pGoods = pThaw->FirstChildElement("Goods"); pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
	{
		LONG lGoodsIndex = atoi(pGoods->Attribute("Index"));
		m_setThawGoodsIndex.insert(lGoodsIndex);
	}

	//! ----------------------------װ������ �������ڵĴ���Ӧ����51~60----------------------------
	TiXmlElement* pArmUpgrade = pGoodsExSetup->FirstChildElement("ArmUpgrade");
	if (NULL == pArmUpgrade) return 51;

	//! ����Ʒ�����Ƶȼ���Χ�趨
	TiXmlElement* pPlayerLevelBound = pArmUpgrade->FirstChildElement("LevelBound");
	for (; pPlayerLevelBound!=NULL; pPlayerLevelBound = pPlayerLevelBound->NextSiblingElement("LevelBound"))
	{
		tagArmUpgradeSet ArmUpgradeSet;
		ArmUpgradeSet.lMinLimitLevel = atoi(pPlayerLevelBound->Attribute("MinLevel"));
		ArmUpgradeSet.lMaxLimitLevel = atoi(pPlayerLevelBound->Attribute("MaxLevel"));

		TiXmlElement* pUpgradeSet = pPlayerLevelBound->FirstChildElement("UpgradeSet");
		for (; pUpgradeSet!=NULL; pUpgradeSet = pUpgradeSet->NextSiblingElement())
		{
			tagUpgradeSet UpgradeSet;
			TiXmlElement* pStuff = pUpgradeSet->FirstChildElement("Stuff");
			for (; pStuff!=NULL; pStuff = pStuff->NextSiblingElement())
			{
				const char* pName = pStuff->Attribute("Name");
				if(NULL != pName)
					UpgradeSet.listStuffName.push_back(string(pName));
			}

			UpgradeSet.lOdds = atoi(pUpgradeSet->Attribute("Odds"));
			LONG lArmMinLevel = atoi(pUpgradeSet->Attribute("ArmMinLevel"));
			LONG lArmMaxLevel = atoi(pUpgradeSet->Attribute("ArmMaxLevel"));

			const char* pFailedOperate= pUpgradeSet->Attribute("FailedOperate");
			if (0 == stricmp("Reduce", pFailedOperate))
				UpgradeSet.lFailedOperate = eFO_Reduce;
			else if(0 == stricmp("SetTo", pFailedOperate))
				UpgradeSet.lFailedOperate = eFO_SetTo;
			else
				UpgradeSet.lFailedOperate = eFO_Null;

			UpgradeSet.lOperateNum = atoi(pUpgradeSet->Attribute("OperateNum"));

			for (UpgradeSet.ArmLevel = lArmMinLevel; UpgradeSet.ArmLevel <= lArmMaxLevel; ++UpgradeSet.ArmLevel)
			{
				ArmUpgradeSet.mapUpgradeSet[UpgradeSet.ArmLevel] = UpgradeSet;
				if (m_lUpgradeLimit < UpgradeSet.ArmLevel) m_lUpgradeLimit = UpgradeSet.ArmLevel;
			}
		}
		m_vecUpgradeSetList.push_back(ArmUpgradeSet);
	}
	++m_lUpgradeLimit;

	
	

	//�� �����������
	TiXmlElement* pExtraStuff = pArmUpgrade->FirstChildElement("ExtraStuff");
	if (NULL == pExtraStuff) return 55;
	TiXmlElement* pSetup = pExtraStuff->FirstChildElement("Setup");
	for (; pSetup!=NULL; pSetup = pSetup->NextSiblingElement())
	{
		tagExtraStuff ExtraStuff;

		TiXmlElement* pStuff = pSetup->FirstChildElement("Stuff");
		for (; pStuff!=NULL; pStuff = pStuff->NextSiblingElement())
		{
			const char* pName = pStuff->Attribute("Name");
			if(NULL != pName)
				ExtraStuff.listStuffName.push_back(string(pName));
		}

		ExtraStuff.lMaxUseCount			= atoi(pSetup->Attribute("MaxUseCount"));
		ExtraStuff.lMinGoodsLevel		= atoi(pSetup->Attribute("MinGoodsLevel"));	
		ExtraStuff.lAddOdds				= atoi(pSetup->Attribute("AddOdds"));		
		ExtraStuff.lFailedMinishLevel	= atoi(pSetup->Attribute("FailedMinishLevel"));

		m_vecExtraStuff.push_back(ExtraStuff);
	}

	TiXmlElement* pMoneySetup = pArmUpgrade->FirstChildElement("MoneySetup");
	for (; pMoneySetup!=NULL; pMoneySetup = pMoneySetup->NextSiblingElement("MoneySetup"))
	{
		tagArmUpgradeMoneySet ArmUpgradeMoneySet;
		ArmUpgradeMoneySet.lMinLimitLevel = atoi(pMoneySetup->Attribute("MinLevel"));
		ArmUpgradeMoneySet.lMaxLimitLevel = atoi(pMoneySetup->Attribute("MaxLevel"));

		TiXmlElement* pSetup = pMoneySetup->FirstChildElement("Setup");
		for (; pSetup!=NULL; pSetup = pSetup->NextSiblingElement())
		{
			LONG lEquipmentType = atoi(pSetup->Attribute("EquipmentType"));
			LONG lChildType		= atoi(pSetup->Attribute("ChildType"));

			LONG lID		= (lEquipmentType << 16) + lChildType;
			LONG lMoney		= atoi(pSetup->Attribute("Money"));

			ArmUpgradeMoneySet.mapUpgradeMoneySet[lID] = lMoney;
		}

		m_vecUpgradeMoneySetList.push_back(ArmUpgradeMoneySet);
	}


	//! ----------------------------װ��ħ�� �������ڵĴ���Ӧ����61~70----------------------------
	m_ElementSetup.vEquipmentLevelBound.clear();
	m_ElementSetup.vElementBound.clear();
	m_ElementSetup.vHelpfulSetup.clear();
	m_ElementSetup.vElementValue.clear();
	m_ElementSetup.vElementValue.resize(MAX_ELEMENT_LEVEL);
	m_ElementSetup.vResetElement.clear();
	m_ElementSetup.vResetXiangXing.clear();

	TiXmlElement* pElementSetup = pGoodsExSetup->FirstChildElement("ElementSetup");
	//! װ���ȼ�������
	TiXmlElement* pEquipmentLevelBound = pElementSetup->FirstChildElement("EquipmentLevelBound");
	for (; pEquipmentLevelBound != NULL; pEquipmentLevelBound = pEquipmentLevelBound->NextSiblingElement("EquipmentLevelBound"))
	{
		tagEquipmentLevelBound EquipmentLevelBound;
		EquipmentLevelBound.lMinLevel		= atoi(pEquipmentLevelBound->Attribute("MinLevel"));
		EquipmentLevelBound.lMaxLevel		= atoi(pEquipmentLevelBound->Attribute("MaxLevel"));
		EquipmentLevelBound.lGetEffect		= atoi(pEquipmentLevelBound->Attribute("GetEffect"));
		EquipmentLevelBound.lConsumeEffect	= atoi(pEquipmentLevelBound->Attribute("ConsumeEffect"));

		m_ElementSetup.vEquipmentLevelBound.push_back(EquipmentLevelBound);
	}
	//! ħ������������
	TiXmlElement* pElementBound = pElementSetup->FirstChildElement("ElementBound");
	for (; pElementBound != NULL; pElementBound = pElementBound->NextSiblingElement("ElementBound"))
	{
		tagElementBound ElementBound;

		TiXmlElement* pStuff = pElementBound->FirstChildElement("Stuff");
		for (; pStuff != NULL; pStuff = pStuff->NextSiblingElement("Stuff"))
		{
			tagElementStuff ElementStuff;

			ElementStuff.lMinLevel = atoi(pStuff->Attribute("MinLevel"));
			ElementStuff.lMaxLevel = atoi(pStuff->Attribute("MaxLevel"));

		const char *pStuffName = NULL;

			pStuffName = pStuff->Attribute("Name_EarthA");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameA[ME_Earth], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_WaterA");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameA[ME_Water], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_FireA");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameA[ME_Fire], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_WindA");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameA[ME_Wind], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_BrightA");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameA[ME_Bright], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_DarkA");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameA[ME_Dark], MAX_GOOODS_NAME_SIZE, pStuffName);


			pStuffName = pStuff->Attribute("Name_EarthD");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameD[ME_Earth], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_WaterD");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameD[ME_Water], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_FireD");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameD[ME_Fire], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_WindD");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameD[ME_Wind], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_BrightD");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameD[ME_Bright], MAX_GOOODS_NAME_SIZE, pStuffName);

			pStuffName = pStuff->Attribute("Name_DarkD");
		if(NULL != pStuffName)
				strcpy_s(ElementStuff.szStuffNameD[ME_Dark], MAX_GOOODS_NAME_SIZE, pStuffName);

			ElementBound.vElementStuff.push_back(ElementStuff);
		}
		

		TiXmlElement* pNeedNum = pElementBound->FirstChildElement("NeedNum");
		for (; pNeedNum != NULL; pNeedNum = pNeedNum->NextSiblingElement("NeedNum"))
		{
			tagNeedNum NeedNum;
			NeedNum.lElementMinLevel	= atoi(pNeedNum->Attribute("ElementMinLevel"));
			NeedNum.lElementMaxLevel	= atoi(pNeedNum->Attribute("ElementMaxLevel"));
			NeedNum.lStuffNumA			= atoi(pNeedNum->Attribute("StuffNumA"));
			NeedNum.lStuffNumD			= atoi(pNeedNum->Attribute("StuffNumD"));
			NeedNum.lSucceedOdds		= atoi(pNeedNum->Attribute("SucceedOdds"));

			ElementBound.vNeedNum.push_back(NeedNum);
		}

		m_ElementSetup.vElementBound.push_back(ElementBound);
	}

	//! ���ʸ�������
	TiXmlElement* pHelpfulStuff = pElementSetup->FirstChildElement("HelpfulStuff");
	for (; pHelpfulStuff != NULL; pHelpfulStuff = pHelpfulStuff->NextSiblingElement("HelpfulStuff"))
	{
		tagHelpfulSetup HelpfulSetup;
		HelpfulSetup.lMinLevel = atoi(pHelpfulStuff->Attribute("ElementMinLevel"));
		HelpfulSetup.lMaxLevel = atoi(pHelpfulStuff->Attribute("ElementMaxLevel"));

		TiXmlElement* pStuff = pHelpfulStuff->FirstChildElement("Stuff");
		for (; pStuff != NULL; pStuff = pStuff->NextSiblingElement("Stuff"))
		{
			tagHelpfulStuff HelpfulStuff;
			const char *pStuffName = pStuff->Attribute("Name");
			if(NULL != pStuffName)
				strcpy_s(HelpfulStuff.szStuffName, MAX_GOOODS_NAME_SIZE, pStuffName);
			HelpfulStuff.lOdds = atoi(pStuff->Attribute("AddOdds"));

			HelpfulSetup.vHelpfulStuff.push_back(HelpfulStuff);
		}
		m_ElementSetup.vHelpfulSetup.push_back(HelpfulSetup);
	}

	//! ħ���ȼ���Ӧֵ
	TiXmlElement* pElementValue = pElementSetup->FirstChildElement("ElementValue");
	for (; pElementValue != NULL; pElementValue = pElementValue->NextSiblingElement("ElementValue"))
	{
		long lLevel = atoi(pElementValue->Attribute("ElementLevel"));
		tagElementValue ElementValue;
		ElementValue.lAtkValue		= atoi(pElementValue->Attribute("AtkValue"));
		ElementValue.lDefValue		= atoi(pElementValue->Attribute("DefValue"));
		ElementValue.lAffixValue	= atoi(pElementValue->Attribute("AffixValue")); 

		if (MAX_ELEMENT_LEVEL > lLevel && 0 <= lLevel)
		{
			m_ElementSetup.vElementValue[lLevel] = ElementValue;
		}
	}

	//! ħ��������������
	TiXmlElement* pResetElement = pElementSetup->FirstChildElement("ResetElement");
	for (; pResetElement != NULL; pResetElement = pResetElement->NextSiblingElement("ResetElement"))
	{
		tagResetElement ResetElement;
		ResetElement.lMinLevel = atoi(pResetElement->Attribute("MinLevel"));
		ResetElement.lMaxLevel = atoi(pResetElement->Attribute("MaxLevel"));

		TiXmlElement* pStuff = pResetElement->FirstChildElement("Stuff");
		for (; pStuff != NULL; pStuff = pStuff->NextSiblingElement("Stuff"))
		{
			tagResetStuff ResetStuff;
			const char *pStuffName = pStuff->Attribute("Name");
			if(NULL != pStuffName)
				strcpy_s(ResetStuff.szStuffName, MAX_GOOODS_NAME_SIZE, pStuffName);
			ResetStuff.lOdds[ME_Earth]	= atoi(pStuff->Attribute("Odds_Earth"));
			ResetStuff.lOdds[ME_Water]	= atoi(pStuff->Attribute("Odds_Water"));
			ResetStuff.lOdds[ME_Fire]	= atoi(pStuff->Attribute("Odds_Fire"));
			ResetStuff.lOdds[ME_Wind]	= atoi(pStuff->Attribute("Odds_Wind"));
			ResetStuff.lOdds[ME_Bright] = atoi(pStuff->Attribute("Odds_Bright"));
			ResetStuff.lOdds[ME_Dark]	= atoi(pStuff->Attribute("Odds_Dark"));

			ResetElement.vResetStuff.push_back(ResetStuff);
		}

		m_ElementSetup.vResetElement.push_back(ResetElement);
	}

	//! ħ��������������
	TiXmlElement* pResetXiangXing = pElementSetup->FirstChildElement("ResetXiangXing");
	for (; pResetXiangXing != NULL; pResetXiangXing = pResetXiangXing->NextSiblingElement("ResetXiangXing"))
	{
		tagResetXiangXing ResetXiangXing;
		ResetXiangXing.lMinLevel = atoi(pResetXiangXing->Attribute("MinLevel"));
		ResetXiangXing.lMaxLevel = atoi(pResetXiangXing->Attribute("MaxLevel"));

		ResetXiangXing.XiangXingOdds.lOdds[0] = atoi(pResetXiangXing->Attribute("Odds_3"));
		ResetXiangXing.XiangXingOdds.lOdds[1] = atoi(pResetXiangXing->Attribute("Odds_6"));
		ResetXiangXing.XiangXingOdds.lOdds[2] = atoi(pResetXiangXing->Attribute("Odds_9"));
		ResetXiangXing.XiangXingOdds.lOdds[3] = atoi(pResetXiangXing->Attribute("Odds_12"));
		ResetXiangXing.XiangXingOdds.lOdds[4] = atoi(pResetXiangXing->Attribute("Odds_15"));

		TiXmlElement* pStuff = pResetXiangXing->FirstChildElement("Stuff");
		for (; pStuff != NULL; pStuff = pStuff->NextSiblingElement("Stuff"))
		{
			tagResetStuff ResetStuff;
			const char *pStuffName = pStuff->Attribute("Name");
			if(NULL != pStuffName)
				strcpy_s(ResetStuff.szStuffName, MAX_GOOODS_NAME_SIZE, pStuffName);
			ResetStuff.lOdds[ME_Earth]	= atoi(pStuff->Attribute("Odds_Earth"));
			ResetStuff.lOdds[ME_Water]	= atoi(pStuff->Attribute("Odds_Water"));
			ResetStuff.lOdds[ME_Fire]	= atoi(pStuff->Attribute("Odds_Fire"));
			ResetStuff.lOdds[ME_Wind]	= atoi(pStuff->Attribute("Odds_Wind"));
			ResetStuff.lOdds[ME_Bright] = atoi(pStuff->Attribute("Odds_Bright"));
			ResetStuff.lOdds[ME_Dark]	= atoi(pStuff->Attribute("Odds_Dark"));

			ResetXiangXing.vResetStuff.push_back(ResetStuff);
		}

		m_ElementSetup.vResetXiangXing.push_back(ResetXiangXing);
	}

	//ǿ���ȼ���Ӧ��ħ�ȼ�
	TiXmlElement* pUpgradeToElementLevel = pElementSetup->FirstChildElement("UpgradeToElementLevel");
	for (; pUpgradeToElementLevel != NULL; pUpgradeToElementLevel = pUpgradeToElementLevel->NextSiblingElement("UpgradeToElementLevel"))
	{
		TiXmlElement* pStuff = pUpgradeToElementLevel->FirstChildElement("Stuff");
		for (; pStuff != NULL; pStuff = pStuff->NextSiblingElement("Stuff"))
		{
			long UpgradeLevel = atoi(pStuff->Attribute("Upgrade"));
			long ElementLevel = atoi(pStuff->Attribute("Element"));
			mapUpgradeToElementLevel[UpgradeLevel] = ElementLevel;
		}
	}


	//! ----------------------------�������----------------------------
	return 0;
}


//! ͨ����ƷƷ�ʣ�lQuality����ȡ�ֽ��䷽
const	GoodsExSetup::tagDisassemblePrescription*		GoodsExSetup::GetDisassemblePrescription(LONG lQuality)
{
	map<LONG, tagDisassemblePrescription>::iterator ite = m_mapDisassemblePrescription.find(lQuality);
	if (m_mapDisassemblePrescription.end() != ite)
	{
		return &(ite->second);
	}
	return NULL;
}

//! ����ֽ�������
//! 	lValue1����Ʒ�ֽ��ֵ1
//! 	lValue2����Ʒ�ֽ��ֵ2
//! 	lQuality����ƷƷ��
//!		lOutIdx���ڼ��ֲ���
LONG GoodsExSetup::GetDisassembleNum(LONG lValue1, LONG lValue2, LONG lQuality, LONG lOutIdx)
{
	if(2 < lOutIdx || 0 > lOutIdx) return 0;

	//! �ڼ��ֲ���Բ��������Ӱ��
	FLOAT fQuotiety = 1.0f;
	switch(lQuality)
	{
	case 1:
		fQuotiety = 0.2f;
	    break;
	case 2:
		fQuotiety = 1.0f;
	    break;
	case 3:
		fQuotiety = 1.6f;
		break;
	case 4:
		fQuotiety = 2.0f;
		break;
	case 99:
		return 1;
	default:
		return 0;
	}

	//! ���������
	FLOAT fBaseResult = lValue2 * fQuotiety * lValue1 / 24.0f + 0.9999999999f;
	
	//! �Խ���ӽ�������ȡ��
	if(0 == lOutIdx)
		return fBaseResult;
	else
		return 0;
}

//! ͨ���䷽������ȡ�ϳ��䷽
const GoodsExSetup::tagSynthesizePrescription* GoodsExSetup::GetSynthesizePrescription(LONG lPrescriptionIndex)
{
	map<LONG, tagSynthesizePrescription>::iterator ite = m_mapSynthesizePrescription.find(lPrescriptionIndex);
	if (ite != m_mapSynthesizePrescription.end())
	{
		return &(ite->second);
	}
	return NULL;
}

//! �õ������������ٷֱ�
LONG GoodsExSetup::GetReinforceMaxPercent(void)
{
	return m_lReinforceMaxPercent;
}

//! ͨ�������ȼ���lLevel���ͳ���ǿ�ȣ�lIntensity�����һ����������
const GoodsExSetup::tagPowerAccessorial* GoodsExSetup::GetPowerAccessorial(LONG lLevel, LONG lIntensity)
{
	for (LONG i = 0; i < m_vecPowerIni.size(); ++i)
	{
		if(m_vecPowerIni[i].lMinLevel <= lLevel && m_vecPowerIni[i].lMaxLevel >= lLevel)
		{
			map<LONG, tagPowerAccessorial>::iterator ite = m_vecPowerIni[i].mapPowerAccessorial.find(lIntensity);
			if (m_vecPowerIni[i].mapPowerAccessorial.end() != ite)
				return &(ite->second);
			else 
				break;
		}
	}
	return NULL;
}

//! ���԰�������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestBindGoodsIndex(LONG lIndex)
{
	if(m_setBindGoodsIndex.end() != m_setBindGoodsIndex.find(lIndex))
		return true;
	return false;
}
//! ���Խ��������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestUntieGoodsIndex(LONG lIndex)
{
	if(m_setUntieGoodsIndex.end() != m_setUntieGoodsIndex.find(lIndex))
		return true;
	return false;
}
//! ���Զ���������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestFrostGoodsIndex(LONG lIndex)
{
	if(m_setFrostGoodsIndex.end() != m_setFrostGoodsIndex.find(lIndex))
		return true;
	return false;
}
//! ���Խⶳ������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestThawGoodsIndex(LONG lIndex)
{
	if(m_setThawGoodsIndex.end() != m_setThawGoodsIndex.find(lIndex))
		return true;
	return false;
}


//! �õ�������Ʒ��������
void GoodsExSetup::GetBindGoodsIndex(vector<LONG> &vecReIdx)
{
	vecReIdx.clear();
	for (set<LONG>::iterator ite = m_setBindGoodsIndex.begin(); ite != m_setBindGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}
//! �õ�������Ʒ��������
void GoodsExSetup::GetUntieGoodsIndex(vector<LONG> &vecReIdx)
{
	vecReIdx.clear();
	for (set<LONG>::iterator ite = m_setUntieGoodsIndex.begin(); ite != m_setUntieGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}
//! �õ�������Ʒ��������
void GoodsExSetup::GetFrostGoodsIndex(vector<LONG> &vecReIdx)
{
	vecReIdx.clear();
	for (set<LONG>::iterator ite = m_setFrostGoodsIndex.begin(); ite != m_setFrostGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}
//! �õ�������Ʒ��������
void GoodsExSetup::GetThawGoodsIndex(vector<LONG> &vecReIdx)
{
	vecReIdx.clear();
	for (set<LONG>::iterator ite = m_setThawGoodsIndex.begin(); ite != m_setThawGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}


//! ð�����򣨴Ӵ�С��
//! ��srcKey�е�ֵ�Ӵ�С˳�򣬶�srcKey��srcValue��������Ӧ��ϵ���е�����������
void	GoodsExSetup::BubbleSort(LONG lSrcSize, ULONG srcKey[], ULONG srcValue[])
{
	ULONG lTempKey = 0, lTempValue = 0;

	for(int i = 0; i< lSrcSize; ++i)
	{
		LONG flag = 0;
		for(int j = lSrcSize - 1; j > i; --j)
		{
			if(srcKey[j] > srcKey[j - 1])
			{
				lTempKey	= srcKey[j];
				lTempValue	= srcValue[j];

				srcKey[j]	= srcKey[j - 1];
				srcValue[j]	= srcValue[j - 1];

				srcKey[j - 1]	= lTempKey;
				srcValue[j - 1] = lTempValue; 

				flag = 1;
			}
		}
		if(0 == flag) break;
	}
}


//! ͨ��װ�����Ƶȼ���װ���ȼ����õ�����䷽
const GoodsExSetup::tagUpgradeSet* GoodsExSetup::GetUpgradeSet(LONG lLimitLevel, LONG lArmLevel)
{
	for (LONG i = 0; i < m_vecUpgradeSetList.size(); ++i)
	{
		if (m_vecUpgradeSetList[i].lMinLimitLevel <= lLimitLevel && m_vecUpgradeSetList[i].lMaxLimitLevel >= lLimitLevel)
		{
			map<LONG, tagUpgradeSet>::iterator ite = m_vecUpgradeSetList[i].mapUpgradeSet.find(lArmLevel);
			if (m_vecUpgradeSetList[i].mapUpgradeSet.end() != ite)
			{
				return &(ite->second);
			}
		}
	}
	return NULL;
}

//! ͨ��װ���ȼ���װ�����͡�װ�������ͻ����Ҫ�Ľ�Ǯ
LONG GoodsExSetup::GetUpgradeMoney(LONG lLimitLevel, LONG lEquipmentType, LONG lChildType)
{
	LONG id = (lEquipmentType << 16) + lChildType;
	for (LONG i = 0; i < m_vecUpgradeMoneySetList.size(); ++i)
	{
		if (m_vecUpgradeMoneySetList[i].lMinLimitLevel <= lLimitLevel && m_vecUpgradeMoneySetList[i].lMaxLimitLevel >= lLimitLevel)
		{
			map<LONG, LONG>::iterator ite = m_vecUpgradeMoneySetList[i].mapUpgradeMoneySet.find(id);
			if (m_vecUpgradeMoneySetList[i].mapUpgradeMoneySet.end() != ite)
			{
				return ite->second;
			}
		}
	}
	return -1;
}

//! �õ�������Ʒ�Ĺ�������
const GoodsExSetup::tagExtraStuff* GoodsExSetup::GetExtraStuffIni(const char* pGoodsName)
{
	if(NULL != pGoodsName)
	{
		for (LONG i = 0; i < m_vecExtraStuff.size(); ++i)
		{
			list<string>::iterator itrStuff = m_vecExtraStuff[i].listStuffName.begin();
			for (; itrStuff != m_vecExtraStuff[i].listStuffName.end(); ++itrStuff)
			{
				if(0 == strcmp(pGoodsName, (*itrStuff).c_str()))
					return &m_vecExtraStuff[i];
			}
		}
	}
	return NULL;
}

LONG GoodsExSetup::GetGoodsMaxLevel(void)
{
	return m_lUpgradeLimit;
}


long GoodsExSetup::GetUpgrade3DEffectID( BOOL bSucceed, long lResultCode )
{
	return bSucceed ? 11121 : 11122;
}


#ifdef USE_IN_SERVER 


const GoodsExSetup::tagExtraStuff* GoodsExSetup::GetExtraStuffIni(LONG lGoodsIndex)
{
	if(0 < lGoodsIndex)
	{
		for (LONG i = 0; i < m_vecExtraStuff.size(); ++i)
		{
			list<long>::iterator itrStuff = m_vecExtraStuff[i].listStuffIndex.begin();
			for (; itrStuff != m_vecExtraStuff[i].listStuffIndex.end(); ++itrStuff)
			{
				if(*itrStuff == lGoodsIndex)
					return &m_vecExtraStuff[i];
			}
		}
	}
	return NULL;
}




#endif //! USE_IN_SERVER


//! �õ�ħ��Ч��ֵ
bool GoodsExSetup::GetElementValue(long lGoodsLevel, long lElementLevel, tagElementValue &RetagElementValue)
{
	bool bRe = false;

	if (lElementLevel < m_ElementSetup.vElementValue.size())
	{
		RetagElementValue.lAtkValue		= m_ElementSetup.vElementValue[lElementLevel].lAtkValue;
		RetagElementValue.lDefValue		= m_ElementSetup.vElementValue[lElementLevel].lDefValue;
		RetagElementValue.lAffixValue	= m_ElementSetup.vElementValue[lElementLevel].lAffixValue;
		bRe = true;

		for (long i = 0; i < m_ElementSetup.vEquipmentLevelBound.size(); ++i)
		{
			if (m_ElementSetup.vEquipmentLevelBound[i].lMinLevel <= lGoodsLevel && m_ElementSetup.vEquipmentLevelBound[i].lMaxLevel >= lGoodsLevel)
			{
				RetagElementValue.lAtkValue		= RetagElementValue.lAtkValue * m_ElementSetup.vEquipmentLevelBound[i].lGetEffect / 10000;
				RetagElementValue.lDefValue		= RetagElementValue.lDefValue * m_ElementSetup.vEquipmentLevelBound[i].lGetEffect / 10000;
				RetagElementValue.lAffixValue	= RetagElementValue.lAffixValue * m_ElementSetup.vEquipmentLevelBound[i].lGetEffect / 10000;
				break;
			}
		}	
	}
	
	return bRe;
}

//! �õ�ħ����������
bool GoodsExSetup::GetElement(bool bIsAtt, long lGoodsLevel, long lElementLevel, long lElementType, LPReStuffName &pReStuffName, long &lNeedNum, long &lSucceedOdds)
{
	bool bRe = false;
	for (long i = 0; i < m_ElementSetup.vElementBound.size(); ++i)
	{
		for (long j = 0; j < m_ElementSetup.vElementBound[i].vNeedNum.size(); ++j)
		{
			if(m_ElementSetup.vElementBound[i].vNeedNum[j].lElementMinLevel <= lElementLevel && m_ElementSetup.vElementBound[i].vNeedNum[j].lElementMaxLevel >= lElementLevel)
			{
				if(bIsAtt)
					lNeedNum	 = m_ElementSetup.vElementBound[i].vNeedNum[j].lStuffNumA;
				else
					lNeedNum	 = m_ElementSetup.vElementBound[i].vNeedNum[j].lStuffNumD;
				
				lSucceedOdds = m_ElementSetup.vElementBound[i].vNeedNum[j].lSucceedOdds;

				for (long n = 0; n < m_ElementSetup.vElementBound[i].vElementStuff.size(); ++n)
				{
					if(m_ElementSetup.vElementBound[i].vElementStuff[n].lMinLevel <= lGoodsLevel && m_ElementSetup.vElementBound[i].vElementStuff[n].lMaxLevel >= lGoodsLevel)
					{
						if(bIsAtt)
							pReStuffName = m_ElementSetup.vElementBound[i].vElementStuff[n].szStuffNameA[lElementType];
						else
							pReStuffName = m_ElementSetup.vElementBound[i].vElementStuff[n].szStuffNameD[lElementType];

						bRe = true;
						goto JumpHere_1;
					}
				}	
			}
		}
	}

JumpHere_1:
	if(bRe)
	{
		for (long i = 0; i < m_ElementSetup.vEquipmentLevelBound.size(); ++i)
		{
			if (m_ElementSetup.vEquipmentLevelBound[i].lMinLevel <= lGoodsLevel && m_ElementSetup.vEquipmentLevelBound[i].lMaxLevel >= lGoodsLevel)
			{
				lNeedNum = lNeedNum * m_ElementSetup.vEquipmentLevelBound[i].lConsumeEffect / 10000;
				lNeedNum = max(1, lNeedNum);
				break;
			}
		}
	}
	
	return bRe;
}

//! �õ�ħ���ĳ�ʼ��������
DWORD GoodsExSetup::GetElement(bool bIsAtt, long lGoodsLevel, LPReStuffName pStuffName, long &lNeedNum, long &lSucceedOdds)
{
	DWORD dwRe = MT_Invalid;
	if(NULL != pStuffName)
	{
		for (long n = 0; n < m_ElementSetup.vElementBound[0].vElementStuff.size(); ++n)
		{
		for (long i = 0; i < MT_Max; ++i)
		{
			if(bIsAtt)
			{
					if(0 == strcmp(m_ElementSetup.vElementBound[0].vElementStuff[n].szStuffNameA[i], pStuffName))
				{
					lNeedNum		= m_ElementSetup.vElementBound[0].vNeedNum[0].lStuffNumA;
					lSucceedOdds	= m_ElementSetup.vElementBound[0].vNeedNum[0].lSucceedOdds;
					dwRe			= i;
						goto JumpHere_2;
				}
			}
			else
			{
					if(0 == strcmp(m_ElementSetup.vElementBound[0].vElementStuff[n].szStuffNameD[i], pStuffName))
				{
					lNeedNum		= m_ElementSetup.vElementBound[0].vNeedNum[0].lStuffNumD;
					lSucceedOdds	= m_ElementSetup.vElementBound[0].vNeedNum[0].lSucceedOdds;
					dwRe			= i;
						goto JumpHere_2;
					}
				}
			}
		}

JumpHere_2:
		if(MT_Invalid != dwRe)
		{
			for (long i = 0; i < m_ElementSetup.vEquipmentLevelBound.size(); ++i)
			{
				if (m_ElementSetup.vEquipmentLevelBound[i].lMinLevel <= lGoodsLevel && m_ElementSetup.vEquipmentLevelBound[i].lMaxLevel >= lGoodsLevel)
				{
					lNeedNum = lNeedNum * m_ElementSetup.vEquipmentLevelBound[i].lConsumeEffect / 10000;
					lNeedNum = max(1, lNeedNum);
					break;
				}
			}
		}
	}
	
	return dwRe;
}

//! �õ�������ʯ���ӵĳɹ����ʣ����֮����ʧ�ܷ��ظ���
long GoodsExSetup::GetHelpfulStuffOdds(long lElementLevel, LPReStuffName pStuffName)
{
	if(NULL != pStuffName)
	{
		for (long i = 0; i < m_ElementSetup.vHelpfulSetup.size(); ++i)
		{
			if (m_ElementSetup.vHelpfulSetup[i].lMinLevel <= lElementLevel && m_ElementSetup.vHelpfulSetup[i].lMaxLevel >= lElementLevel)
			{
				for (long j = 0; j < m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff.size(); ++j)
				{
					if(0 == strcmp(m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff[j].szStuffName, pStuffName))
						return m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff[j].lOdds;
				}
			}
		}
	}
	return -1;
}

//! �õ�ħ��Ԫ�����õĲ��ϼ�������
const GoodsExSetup::tagResetStuff* GoodsExSetup::GetResetElement(long lGoodsLevel, LPReStuffName pStuffName)
{
	if(NULL != pStuffName)
	{
		for (long i = 0; i < m_ElementSetup.vResetElement.size(); ++i)
		{
			if (m_ElementSetup.vResetElement[i].lMinLevel <= lGoodsLevel && m_ElementSetup.vResetElement[i].lMaxLevel >= lGoodsLevel)
			{
				for (long j = 0; j < m_ElementSetup.vResetElement[i].vResetStuff.size(); ++j)
				{
					if(0 == strcmp(m_ElementSetup.vResetElement[i].vResetStuff[j].szStuffName, pStuffName))
					{
						return &(m_ElementSetup.vResetElement[i].vResetStuff[j]);
					}
				}
			}
		}
	}
	
	return NULL;
}

//! �õ�ħ���������õĲ��ϼ�������
const GoodsExSetup::tagResetStuff* GoodsExSetup::GetResetXiangXing(long lGoodsLevel, LPReStuffName pStuffName, LPCXiangXingOdds &pXiangXingOdds)
{
	if(NULL != pStuffName)
	{
		for (long i = 0; i < m_ElementSetup.vResetXiangXing.size(); ++i)
		{
			if (m_ElementSetup.vResetXiangXing[i].lMinLevel <= lGoodsLevel && m_ElementSetup.vResetXiangXing[i].lMaxLevel >= lGoodsLevel)
			{
				for (long j = 0; j < m_ElementSetup.vResetXiangXing[i].vResetStuff.size(); ++j)
				{
					if(0 == strcmp(m_ElementSetup.vResetXiangXing[i].vResetStuff[j].szStuffName, pStuffName))
					{
						pXiangXingOdds = &(m_ElementSetup.vResetXiangXing[i].XiangXingOdds);
						return &(m_ElementSetup.vResetXiangXing[i].vResetStuff[j]);
					}
				}
			}
		}
	}

	pXiangXingOdds = NULL;
	return NULL;
}

//! ��õ�ǰǿ���ȼ���ߵĸ�ħ�ȼ�
long GoodsExSetup::GetMaxElementLevelByUpgrade(long Upgrade)
{
	map<long,long>::iterator itor = mapUpgradeToElementLevel.find(Upgrade);
	if(itor!=mapUpgradeToElementLevel.end())
	{
		return itor->second;
	}
	return 0;
}


//! �ͻ��˷�������
///////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef USE_IN_SERVER

//! �ҵ�һ���ϳ��䷽����
LONG GoodsExSetup::FindSynthesizePrescriptionIdx(
	char* arr_pStuffName[MAX_STUFF_NUM], 
	LONG arr_lStuffNum[MAX_STUFF_NUM])
{
	//! �ȵ�����ͬ��Ʒ
	for (int i = 0; i< MAX_STUFF_NUM; ++i)
	{
		if (NULL != arr_pStuffName[i])
		{
			for(int j = i + 1; j < MAX_STUFF_NUM; ++j)
			{
				if (NULL != arr_pStuffName[j])
				{
					if( 0 == stricmp(arr_pStuffName[i], arr_pStuffName[j]))
					{
						arr_lStuffNum[i]	+= arr_lStuffNum[j];
						arr_pStuffName[j]	= NULL;
						arr_lStuffNum[j]	= 0;
					}
				}
			}
		}	
	}

	//! �������������
	LONG lTempValue = 0;
	char *pTempKey = NULL;

	for(int i = 0; i< MAX_STUFF_NUM; ++i)
	{
		LONG flag = 0;
		for(int j = MAX_STUFF_NUM - 1; j > i; --j)
		{
			bool bSwap = false;
			if (NULL != arr_pStuffName[j] && NULL != arr_pStuffName[j - 1])
			{
				if(0 < stricmp(arr_pStuffName[j], arr_pStuffName[j - 1])) bSwap = true;
			}
			else
			{
				if(NULL != arr_pStuffName[j] && NULL == arr_pStuffName[j - 1]) bSwap = true;
			}
			
			if(bSwap)
			{
				pTempKey	= arr_pStuffName[j];
				lTempValue	= arr_lStuffNum[j];

				arr_pStuffName[j]	= arr_pStuffName[j - 1];
				arr_lStuffNum[j]	= arr_lStuffNum[j - 1];

				arr_pStuffName[j - 1]	= pTempKey;
				arr_lStuffNum[j - 1]	= lTempValue; 

				flag = 1;
			}
		}
		if(0 == flag) break;
	}

	//! �����ý��бȽ�
	map<LONG, tagSynthesizePrescription>::iterator ite = m_mapSynthesizePrescription.begin();
	for ( ; ite != m_mapSynthesizePrescription.end(); ++ite)
	{
		bool bFind = true;
		for (int i = 0; i< MAX_STUFF_NUM; ++i)
		{
			//! ���ȼ������
			if(arr_lStuffNum[i] == ite->second.arr_lStuffNum[i])
			{
				//! �ڼ����Ʒԭʼ��
				if (0 != ite->second.sz_arr_StuffName[i][0])
				{
					if (NULL != arr_pStuffName[i])
					{
						if (0 == stricmp(ite->second.sz_arr_StuffName[i], arr_pStuffName[i]))
						{
							continue;
						}
					}
				}
				else if(NULL == arr_pStuffName[i])
				{
					continue;
				}
			}
			
			bFind = false;
			break;
		}

		if (bFind)
		{
			return ite->first;
		}
	}

	return 0;
}

void GoodsExSetup::MeBubbleSort(char srcKey[MAX_STUFF_NUM][MAX_GOOODS_NAME_SIZE], LONG srcValue[MAX_STUFF_NUM])
{
	LONG lTempValue = 0;
	char szTempKey[MAX_STUFF_NUM] = {0};

	for(int i = 0; i< MAX_STUFF_NUM; ++i)
	{
		LONG flag = 0;
		for(int j = MAX_STUFF_NUM - 1; j > i; --j)
		{
			if(0 < stricmp(srcKey[j], srcKey[j - 1]))
			{
				memmove(szTempKey, srcKey[j], MAX_STUFF_NUM);
				lTempValue	= srcValue[j];

				memmove(srcKey[j], srcKey[j - 1], MAX_STUFF_NUM);
				srcValue[j]	= srcValue[j - 1];

				memmove(srcKey[j - 1], szTempKey, MAX_STUFF_NUM);
				srcValue[j - 1] = lTempValue; 

				flag = 1;
			}
		}
		if(0 == flag) break;
	}
}

//!  ͨ��ħ���ȼ����ظ�����ʯ��ԭʼ��
const char*   GoodsExSetup::GetHelpfulStuffName(long lElementLevel)
{
	for (long i = 0; i < m_ElementSetup.vHelpfulSetup.size(); ++i)
	{
		if (m_ElementSetup.vHelpfulSetup[i].lMinLevel <= lElementLevel && m_ElementSetup.vHelpfulSetup[i].lMaxLevel >= lElementLevel)
		{
                    return m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff[0].szStuffName;
		}
	}
	return NULL;
}

#endif


//! ��⿨Ƭ�ܷ���Ƕ
//! CardSet����Ƭ�ϵ���Ƕ����ֵ
//! BaseType��װ������Ʒ����ֵ����Ӧ�������Ʒ������趨��ֵ��
//! ExType������/������𣨶�Ӧ��Ʒ�������ԡ�������GAP_WEAPON_CATEGORY������GAP_GUARD_EQUIP_CATEGORY�����ƽ�ָ����û��=0��
bool CheckCardValue(DWORD CardSet, DWORD BaseType, DWORD ExType)
{
	//! û�����õĿ�Ƭ������Ƕ
	if (0 == CardSet)
		return false;

	DWORD MapValue = 0;
	for (DWORD i = 0; i < CARD_VALUE_MAP_SIZE; ++i)
	{
		if (CardValueMap[i].BaseType == BaseType && CardValueMap[i].ExType == ExType)
		{
			MapValue = CardValueMap[i].BitValue;
			break;
		}
	}

	return (0 != (MapValue & CardSet));
}