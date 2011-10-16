#include "StdAfx.h"
#include "GoodsExSetup.h"
#include "GoodsSetup.h"
#include "../Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GoodsExSetup::GoodsExSetup(void)
{

}
GoodsExSetup::~GoodsExSetup(void)
{

}

//! ��XML�����ļ��ж�ȡ����
long GoodsExSetup::ReadSetup(const char* fileName)
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
			for (long i = 0; pOutGoods!=NULL; pOutGoods = pOutGoods->NextSiblingElement("OutGoods"))
			{
				if(MAX_DISASSEMBLE_NUM > i)
				{	
					const char *pOutGoodsName = pOutGoods->Attribute("Name");
					long lSize = (strlen(pOutGoodsName) + 1 > MAX_GOOODS_NAME_SIZE) ? MAX_GOOODS_NAME_SIZE : strlen(pOutGoodsName) + 1;
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

		SynthesizePrescription.bHasPrimary = (0 != atoi(pPrescription->Attribute("HasPrimaryStuff")));

		TiXmlElement* pStuff = pPrescription->FirstChildElement("Stuff");
		long lStuffIndex = 0;
		for (; pStuff!=NULL; pStuff = pStuff->NextSiblingElement())
		{
			SynthesizePrescription.arr_lStuffNum[lStuffIndex] = atoi(pStuff->Attribute("Num"));

			const char* pName = pStuff->Attribute("Name");
			memmove(SynthesizePrescription.sz_arr_StuffName[lStuffIndex], pName, strlen(pName) + 1);

			if (MAX_STUFF_NUM <= lStuffIndex) break;
			lStuffIndex++;
		}

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
		long lGoodsIndex = atoi(pGoods->Attribute("Index"));
		m_setBindGoodsIndex.insert(lGoodsIndex);
	}

	//! �����Ʒ�����б�
	TiXmlElement* pUntie = pGoodsExSetup->FirstChildElement("Untie");
	if (NULL == pUntie) return 42;

	for (TiXmlElement* pGoods = pUntie->FirstChildElement("Goods"); pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
	{
		long lGoodsIndex = atoi(pGoods->Attribute("Index"));
		m_setUntieGoodsIndex.insert(lGoodsIndex);
	}

	//! ������Ʒ�����б�
	TiXmlElement* pFrost = pGoodsExSetup->FirstChildElement("Frost");
	if (NULL == pFrost) return 42;

	for (TiXmlElement* pGoods = pFrost->FirstChildElement("Goods"); pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
	{
		long lGoodsIndex = atoi(pGoods->Attribute("Index"));
		m_setFrostGoodsIndex.insert(lGoodsIndex);
	}
	
	//! �ⶳ��Ʒ�����б�
	TiXmlElement* pThaw = pGoodsExSetup->FirstChildElement("Thaw");
	if (NULL == pThaw) return 42;

	for (TiXmlElement* pGoods = pThaw->FirstChildElement("Goods"); pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
	{
		long lGoodsIndex = atoi(pGoods->Attribute("Index"));
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
			const char* pName = pUpgradeSet->Attribute("StuffName");
			memmove(UpgradeSet.szStuffName, pName, strlen(pName) + 1);

			UpgradeSet.lOdds = atoi(pUpgradeSet->Attribute("Odds"));
			long lArmMinLevel = atoi(pUpgradeSet->Attribute("ArmMinLevel"));
			long lArmMaxLevel = atoi(pUpgradeSet->Attribute("ArmMaxLevel"));

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

		const char* pName = pSetup->Attribute("StuffName");
		memmove(ExtraStuff.szStuffName, pName, strlen(pName) + 1);

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
			long lEquipmentType = atoi(pSetup->Attribute("EquipmentType"));
			long lChildType		= atoi(pSetup->Attribute("ChildType"));

			long lID		= (lEquipmentType << 16) + lChildType;
			long lMoney		= atoi(pSetup->Attribute("Money"));

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


	//! ----------------------------�������----------------------------
	return 0;
}


//! ͨ����ƷƷ�ʣ�lQuality����ȡ�ֽ��䷽
const	GoodsExSetup::tagDisassemblePrescription*		GoodsExSetup::GetDisassemblePrescription(long lQuality)
{
	map<long, tagDisassemblePrescription>::iterator ite = m_mapDisassemblePrescription.find(lQuality);
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
long GoodsExSetup::GetDisassembleNum(long lValue1, long lValue2, long lQuality, long lOutIdx)
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
		return (long)fBaseResult;
	else
		return 0;
}

//! ͨ���䷽������ȡ�ϳ��䷽
const GoodsExSetup::tagSynthesizePrescription* GoodsExSetup::GetSynthesizePrescription(long lPrescriptionIndex)
{
	map<long, tagSynthesizePrescription>::iterator ite = m_mapSynthesizePrescription.find(lPrescriptionIndex);
	if (ite != m_mapSynthesizePrescription.end())
	{
		return &(ite->second);
	}
	return NULL;
}

//! �õ������������ٷֱ�
long GoodsExSetup::GetReinforceMaxPercent(void)
{
	return m_lReinforceMaxPercent;
}

//! ͨ�������ȼ���lLevel���ͳ���ǿ�ȣ�lIntensity�����һ����������
const GoodsExSetup::tagPowerAccessorial* GoodsExSetup::GetPowerAccessorial(long lLevel, long lIntensity)
{
	for (size_t i = 0; i < m_vecPowerIni.size(); ++i)
	{
		if(m_vecPowerIni[i].lMinLevel <= lLevel && m_vecPowerIni[i].lMaxLevel >= lLevel)
		{
			map<long, tagPowerAccessorial>::iterator ite = m_vecPowerIni[i].mapPowerAccessorial.find(lIntensity);
			if (m_vecPowerIni[i].mapPowerAccessorial.end() != ite)
				return &(ite->second);
			else 
				break;
		}
	}
	return NULL;
}

//! ���԰�������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestBindGoodsIndex(long lIndex)
{
	if(m_setBindGoodsIndex.end() != m_setBindGoodsIndex.find(lIndex))
		return true;
	return false;
}
//! ���Խ��������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestUntieGoodsIndex(long lIndex)
{
	if(m_setUntieGoodsIndex.end() != m_setUntieGoodsIndex.find(lIndex))
		return true;
	return false;
}
//! ���Զ���������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestFrostGoodsIndex(long lIndex)
{
	if(m_setFrostGoodsIndex.end() != m_setFrostGoodsIndex.find(lIndex))
		return true;
	return false;
}
//! ���Խⶳ������Ʒ�����Ƿ���ȷ
bool GoodsExSetup::TestThawGoodsIndex(long lIndex)
{
	if(m_setThawGoodsIndex.end() != m_setThawGoodsIndex.find(lIndex))
		return true;
	return false;
}


//! �õ�������Ʒ��������
void GoodsExSetup::GetBindGoodsIndex(vector<long> &vecReIdx)
{
	vecReIdx.clear();
	for (set<long>::iterator ite = m_setBindGoodsIndex.begin(); ite != m_setBindGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}
//! �õ�������Ʒ��������
void GoodsExSetup::GetUntieGoodsIndex(vector<long> &vecReIdx)
{
	vecReIdx.clear();
	for (set<long>::iterator ite = m_setUntieGoodsIndex.begin(); ite != m_setUntieGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}
//! �õ�������Ʒ��������
void GoodsExSetup::GetFrostGoodsIndex(vector<long> &vecReIdx)
{
	vecReIdx.clear();
	for (set<long>::iterator ite = m_setFrostGoodsIndex.begin(); ite != m_setFrostGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}
//! �õ�������Ʒ��������
void GoodsExSetup::GetThawGoodsIndex(vector<long> &vecReIdx)
{
	vecReIdx.clear();
	for (set<long>::iterator ite = m_setThawGoodsIndex.begin(); ite != m_setThawGoodsIndex.end(); ++ite)
	{
		vecReIdx.push_back(*ite);
	}
}


//! ð�����򣨴Ӵ�С��
//! ��srcKey�е�ֵ�Ӵ�С˳�򣬶�srcKey��srcValue��������Ӧ��ϵ���е�����������
void	GoodsExSetup::BubbleSort(long lSrcSize, ULONG srcKey[], ULONG srcValue[])
{
	ULONG lTempKey = 0, lTempValue = 0;

	for(int i = 0; i< lSrcSize; ++i)
	{
		long flag = 0;
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
const GoodsExSetup::tagUpgradeSet* GoodsExSetup::GetUpgradeSet(long lLimitLevel, long lArmLevel)
{
    for (size_t i = 0; i < m_vecUpgradeSetList.size(); ++i)
    {
        if (m_vecUpgradeSetList[i].lMinLimitLevel <= lLimitLevel && m_vecUpgradeSetList[i].lMaxLimitLevel >= lLimitLevel)
        {
            map<long, tagUpgradeSet>::iterator ite = m_vecUpgradeSetList[i].mapUpgradeSet.find(lArmLevel);
            if (m_vecUpgradeSetList[i].mapUpgradeSet.end() != ite)
            {
                return &(ite->second);
            }
        }
    }
	return NULL;
}

//! ͨ��װ���ȼ���װ�����͡�װ�������ͻ����Ҫ�Ľ�Ǯ
long GoodsExSetup::GetUpgradeMoney(long lLimitLevel, long lEquipmentType, long lChildType)
{
	long id = (lEquipmentType << 16) + lChildType;
	for (size_t i = 0; i < m_vecUpgradeMoneySetList.size(); ++i)
	{
		if (m_vecUpgradeMoneySetList[i].lMinLimitLevel <= lLimitLevel && m_vecUpgradeMoneySetList[i].lMaxLimitLevel >= lLimitLevel)
		{
			map<long, long>::iterator ite = m_vecUpgradeMoneySetList[i].mapUpgradeMoneySet.find(id);
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
		for (size_t i = 0; i < m_vecExtraStuff.size(); ++i)
		{
			if(0 == stricmp(m_vecExtraStuff[i].szStuffName, pGoodsName))
				return &m_vecExtraStuff[i];
		}
	}
	return NULL;
}

long GoodsExSetup::GetGoodsMaxLevel(void)
{
	return m_lUpgradeLimit;
}


long GoodsExSetup::GetUpgrade3DEffectID( bool bSucceed, long lResultCode )
{
	return bSucceed ? 11121 : 11122;
}


const GoodsExSetup::tagExtraStuff* GoodsExSetup::GetExtraStuffIni(long lGoodsIndex)
{
	if(0 < lGoodsIndex)
	{
		for (size_t i = 0; i < m_vecExtraStuff.size(); ++i)
		{
			if(m_vecExtraStuff[i].lStuffIndex == lGoodsIndex)
				return &m_vecExtraStuff[i];
		}
	}
	return NULL;
}



//! �õ�ħ��Ч��ֵ
bool GoodsExSetup::GetElementValue(long lGoodsLevel, long lElementLevel, tagElementValue &RetagElementValue)
{
	bool bRe = false;

	if (lElementLevel < (long)m_ElementSetup.vElementValue.size())
	{
		RetagElementValue.lAtkValue		= m_ElementSetup.vElementValue[lElementLevel].lAtkValue;
		RetagElementValue.lDefValue		= m_ElementSetup.vElementValue[lElementLevel].lDefValue;
		RetagElementValue.lAffixValue	= m_ElementSetup.vElementValue[lElementLevel].lAffixValue;
		bRe = true;

		for (size_t i = 0; i < m_ElementSetup.vEquipmentLevelBound.size(); ++i)
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
	for (size_t i = 0; i < m_ElementSetup.vElementBound.size(); ++i)
	{
		for (size_t j = 0; j < m_ElementSetup.vElementBound[i].vNeedNum.size(); ++j)
		{
			if(m_ElementSetup.vElementBound[i].vNeedNum[j].lElementMinLevel <= lElementLevel && m_ElementSetup.vElementBound[i].vNeedNum[j].lElementMaxLevel >= lElementLevel)
			{
				if(bIsAtt)
					lNeedNum	 = m_ElementSetup.vElementBound[i].vNeedNum[j].lStuffNumA;
				else
					lNeedNum	 = m_ElementSetup.vElementBound[i].vNeedNum[j].lStuffNumD;
				
				lSucceedOdds = m_ElementSetup.vElementBound[i].vNeedNum[j].lSucceedOdds;

				for (size_t n = 0; n < m_ElementSetup.vElementBound[i].vElementStuff.size(); ++n)
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
		for (size_t i = 0; i < m_ElementSetup.vEquipmentLevelBound.size(); ++i)
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
ulong GoodsExSetup::GetElement(bool bIsAtt, long lGoodsLevel, LPReStuffName pStuffName, long &lNeedNum, long &lSucceedOdds)
{
	size_t dwRe = MT_Invalid;
	if(NULL != pStuffName)
	{
		for (size_t n = 0; n < m_ElementSetup.vElementBound[0].vElementStuff.size(); ++n)
		{
		for (size_t i = 0; i < MT_Max; ++i)
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
			for (size_t i = 0; i < m_ElementSetup.vEquipmentLevelBound.size(); ++i)
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
	
	return (ulong)dwRe;
}

//! �õ�������ʯ���ӵĳɹ����ʣ����֮����ʧ�ܷ��ظ���
long GoodsExSetup::GetHelpfulStuffOdds(long lElementLevel, LPReStuffName pStuffName)
{
	if(NULL != pStuffName)
	{
		for (size_t i = 0; i < m_ElementSetup.vHelpfulSetup.size(); ++i)
		{
			if (m_ElementSetup.vHelpfulSetup[i].lMinLevel <= lElementLevel && m_ElementSetup.vHelpfulSetup[i].lMaxLevel >= lElementLevel)
			{
				for (size_t j = 0; j < m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff.size(); ++j)
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
		for (size_t i = 0; i < m_ElementSetup.vResetElement.size(); ++i)
		{
			if (m_ElementSetup.vResetElement[i].lMinLevel <= lGoodsLevel && m_ElementSetup.vResetElement[i].lMaxLevel >= lGoodsLevel)
			{
				for (size_t j = 0; j < m_ElementSetup.vResetElement[i].vResetStuff.size(); ++j)
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
		for (size_t i = 0; i < m_ElementSetup.vResetXiangXing.size(); ++i)
		{
			if (m_ElementSetup.vResetXiangXing[i].lMinLevel <= lGoodsLevel && m_ElementSetup.vResetXiangXing[i].lMaxLevel >= lGoodsLevel)
			{
				for (size_t j = 0; j < m_ElementSetup.vResetXiangXing[i].vResetStuff.size(); ++j)
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

//! ��ʼ������������
//! ���ܣ��������е���Ʒԭʼ����Ӧ����Ʒ�����ҳ�����ӵ�������
bool GoodsExSetup::InitServerSetup(void)
{
	bool bLocalResult = true;
	bool bReValue = true;
	//! �ֽ���Ʒ����
	map<LONG, tagDisassemblePrescription>::iterator iteDP = m_mapDisassemblePrescription.begin(); 
	for (; iteDP != m_mapDisassemblePrescription.end(); ++iteDP)
	{
		for (long idx = 0; idx < MAX_DISASSEMBLE_NUM; ++idx)
		{
			if (0 == iteDP->second.arr_szOutGoodsName[idx][0]) break;
			iteDP->second.arr_lOutGoodsId[idx] = GoodsSetup::QueryGoodsIDByOriginalName(iteDP->second.arr_szOutGoodsName[idx]);
			if (0 >= iteDP->second.arr_lOutGoodsId[idx])
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_10",iteDP->second.arr_szOutGoodsName[idx]));
				bLocalResult = false;
				continue;
			}
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vOut1;
			GoodsSetup::QueryGoodsBaseProperties(iteDP->second.arr_lOutGoodsId[idx])->GetAddonPropertyValues(GAP_QUALITY, vOut1);
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vOut2;
			GoodsSetup::QueryGoodsBaseProperties(iteDP->second.arr_lOutGoodsId[idx])->GetAddonPropertyValues(GAP_MAXOVERLAP, vOut2);
			if(0 < vOut1.size() && 0 < vOut2.size())
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_30",iteDP->second.arr_szOutGoodsName[idx]));
				bLocalResult = false;
				continue;
			}
		}
	}

	if (bLocalResult)
	{
		Log4c::Trace(ROOT_MODULE,GetText("WS_GOODSEX_50"));
	}
	else
	{
		bReValue = false;
	}

	//! �ϳ���Ʒ����
	bLocalResult = true;
	map<long, tagSynthesizePrescription>::iterator ite = m_mapSynthesizePrescription.begin();
	for ( ; ite != m_mapSynthesizePrescription.end(); ++ite)
	{
		ite->second.lOutGoodsId = GoodsSetup::QueryGoodsIDByOriginalName(ite->second.szOutGoodsName);
		if (0 >= ite->second.lOutGoodsId)
		{
			Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_60",ite->second.szOutGoodsName));
			bLocalResult = false;
			continue;
		}

		for (long i = 0; i < MAX_STUFF_NUM; ++i)
		{
			if(0 == ite->second.sz_arr_StuffName[i][0]) break;
			ite->second.arr_lStuffId[i] = GoodsSetup::QueryGoodsIDByOriginalName(ite->second.sz_arr_StuffName[i]);
			if (0 >= ite->second.arr_lStuffId[i])
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_60",ite->second.sz_arr_StuffName[i]));
				bLocalResult = false;
				continue;
			}
		}
		//! �����������Բ��������Ͳ������������ɴ�С����ԭʼ�����Ժ󶼲����õ��ˣ�����
		//GoodsExSetup::BubbleSort(MAX_STUFF_NUM, ite->second.arr_lStuffId, ite->second.arr_lStuffNum);
	}

	if (bLocalResult)
	{
		Log4c::Trace(ROOT_MODULE,GetText("WS_GOODSEX_80"));
	}
	else
	{
		bReValue = false;
	}


	//! װ����������
	bLocalResult = true;
	for (long i = 0; i < (long)m_vecUpgradeSetList.size(); ++i)
	{
		map<long, tagUpgradeSet>::iterator ite = m_vecUpgradeSetList[i].mapUpgradeSet.begin();
		for (; ite != m_vecUpgradeSetList[i].mapUpgradeSet.end(); ++ite)
		{
			ite->second.lStuffIndex = GoodsSetup::QueryGoodsIDByOriginalName(ite->second.szStuffName);
			if(0 == ite->second.lStuffIndex) 
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_90",ite->second.szStuffName));
				bLocalResult = false;
				continue;
			}
		}
	}

	if (bLocalResult)
	{
		Log4c::Trace(ROOT_MODULE,GetText("WS_GOODSEX_110"));
	}
	else
	{
		bReValue = false;
	}

	//! ������Ʒ����
	bLocalResult = true;
	for (long i = 0; i < (long)m_vecExtraStuff.size(); ++i)
	{
		m_vecExtraStuff[i].lStuffIndex = GoodsSetup::QueryGoodsIDByOriginalName(m_vecExtraStuff[i].szStuffName);
		if(0 == m_vecExtraStuff[i].lStuffIndex)
		{
			Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_120",m_vecExtraStuff[i].szStuffName));
			bLocalResult = false;
		}
	}

	if (bLocalResult)
	{
		Log4c::Trace(ROOT_MODULE,GetText("WS_GOODSEX_130"));
	}
	else
	{
		bReValue = false;
	}


	//! ħ������
	bLocalResult = true;

	for (long i = 0; i < (long)m_ElementSetup.vElementBound.size(); ++i)
	{
		for (long n = 0; n < (long)m_ElementSetup.vElementBound[0].vElementStuff.size(); ++n)
		{
			for (long j = 0; j < MT_Max; ++j)
			{
				long lStuffIndex = GoodsSetup::QueryGoodsIDByOriginalName(m_ElementSetup.vElementBound[i].vElementStuff[n].szStuffNameA[j]);
				if(0 == lStuffIndex) 
				{
					Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_140",m_ElementSetup.vElementBound[i].vElementStuff[n].szStuffNameA[j]));
					bLocalResult = false;
				}
			}
			for (long j = 0; j < MT_Max; ++j)
			{
				long lStuffIndex = GoodsSetup::QueryGoodsIDByOriginalName(m_ElementSetup.vElementBound[i].vElementStuff[n].szStuffNameD[j]);
				if(0 == lStuffIndex) 
				{
					Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_140",m_ElementSetup.vElementBound[i].vElementStuff[n].szStuffNameD[j]));
					bLocalResult = false;
				}
			}
		}
	}

	for (long i = 0; i < (long)m_ElementSetup.vHelpfulSetup.size(); ++i)
	{
		for (long j = 0; j < (long)m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff.size(); ++j)
		{
			long lStuffIndex = GoodsSetup::QueryGoodsIDByOriginalName(m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff[j].szStuffName);
			if(0 == lStuffIndex) 
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_140",m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff[j].szStuffName));
				bLocalResult = false;
			}
		}
	}

	for (long i = 0; i < (long)m_ElementSetup.vResetElement.size(); ++i)
	{
		for (long j = 0; j < (long)m_ElementSetup.vResetElement[i].vResetStuff.size(); ++j)
		{
			long lStuffIndex = GoodsSetup::QueryGoodsIDByOriginalName(m_ElementSetup.vResetElement[i].vResetStuff[j].szStuffName);
			if(0 == lStuffIndex) 
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_140",m_ElementSetup.vResetElement[i].vResetStuff[j].szStuffName));
				bLocalResult = false;
			}
		}
	}

	for (long i = 0; i < (long)m_ElementSetup.vResetXiangXing.size(); ++i)
	{
		for (long j = 0; j < (long)m_ElementSetup.vResetXiangXing[i].vResetStuff.size(); ++j)
		{
			long lStuffIndex = GoodsSetup::QueryGoodsIDByOriginalName(m_ElementSetup.vResetXiangXing[i].vResetStuff[j].szStuffName);
			if(0 == lStuffIndex) 
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_GOODSEX_140",m_ElementSetup.vResetXiangXing[i].vResetStuff[j].szStuffName));
				bLocalResult = false;
			}
		}
	}

	if (bLocalResult)
	{
		Log4c::Trace(ROOT_MODULE,GetText("WS_GOODSEX_190"));
	}
	else
	{
		bReValue = false;
	}

	return bReValue;
}

//! GoodsExSetup�ı��붨�壬��Ӧ����ӿڣ�Unserialize��λ�ã�GS GoodsExManage_base.cpp
bool GoodsExSetup::Serialize(vector<BYTE>* pStream)
{
	//! �ֽ���Ʒ����
	_AddToByteArray(pStream, (long)m_mapDisassemblePrescription.size());

	map<LONG, tagDisassemblePrescription>::iterator iteDP = m_mapDisassemblePrescription.begin(); 
	for (; iteDP != m_mapDisassemblePrescription.end(); ++iteDP)
	{
		_AddToByteArray(pStream, &(iteDP->second), sizeof(tagDisassemblePrescription));
	}

	//! �ϳ���Ʒ����
	_AddToByteArray(pStream, (long)m_mapSynthesizePrescription.size());
	map<long, tagSynthesizePrescription>::iterator ite = m_mapSynthesizePrescription.begin();
	for ( ; ite != m_mapSynthesizePrescription.end(); ++ite)
	{
		_AddToByteArray(pStream, &(ite->second), sizeof(tagSynthesizePrescription));
	}

	//! ��������
	_AddToByteArray(pStream, m_lReinforceMaxPercent);

	_AddToByteArray(pStream, (long)m_vecPowerIni.size());
	for (long i = 0; i < (long)m_vecPowerIni.size(); ++i)
	{
		_AddToByteArray(pStream, m_vecPowerIni[i].lMinLevel);
		_AddToByteArray(pStream, m_vecPowerIni[i].lMaxLevel);
		_AddToByteArray(pStream, (long)m_vecPowerIni[i].mapPowerAccessorial.size());
		map<long, tagPowerAccessorial>::iterator ite = m_vecPowerIni[i].mapPowerAccessorial.begin();
		for ( ; ite != m_vecPowerIni[i].mapPowerAccessorial.end(); ++ite)
		{
			_AddToByteArray(pStream, &(ite->second), sizeof(tagPowerAccessorial));
		}
	}

	//! ����Ʒ�����б�
	_AddToByteArray(pStream, (long)m_setBindGoodsIndex.size());
	for (set<long>::iterator ite = m_setBindGoodsIndex.begin(); ite != m_setBindGoodsIndex.end(); ++ite)
	{
		_AddToByteArray(pStream, *ite);
	}
	//! �����Ʒ�����б�
	_AddToByteArray(pStream, (long)m_setUntieGoodsIndex.size());
	for (set<long>::iterator ite = m_setUntieGoodsIndex.begin(); ite != m_setUntieGoodsIndex.end(); ++ite)
	{
		_AddToByteArray(pStream, *ite);
	}
	//! ������Ʒ�����б�
	_AddToByteArray(pStream, (long)m_setFrostGoodsIndex.size());
	for (set<long>::iterator ite = m_setFrostGoodsIndex.begin(); ite != m_setFrostGoodsIndex.end(); ++ite)
	{
		_AddToByteArray(pStream, *ite);
	}
	//! �ⶳ��Ʒ�����б�
	_AddToByteArray(pStream, (long)m_setThawGoodsIndex.size());
	for (set<long>::iterator ite = m_setThawGoodsIndex.begin(); ite != m_setThawGoodsIndex.end(); ++ite)
	{
		_AddToByteArray(pStream, *ite);
	}

	//! װ����������
	_AddToByteArray(pStream, (long)m_vecUpgradeSetList.size());
	for (long i = 0; i < (long)m_vecUpgradeSetList.size(); ++i)
	{
		_AddToByteArray(pStream, m_vecUpgradeSetList[i].lMinLimitLevel);
		_AddToByteArray(pStream, m_vecUpgradeSetList[i].lMaxLimitLevel);
		_AddToByteArray(pStream, (long)m_vecUpgradeSetList[i].mapUpgradeSet.size());
		map<long, tagUpgradeSet>::iterator ite = m_vecUpgradeSetList[i].mapUpgradeSet.begin();
		for ( ; ite != m_vecUpgradeSetList[i].mapUpgradeSet.end(); ++ite)
		{
			_AddToByteArray(pStream, &(ite->second), sizeof(tagUpgradeSet));
		}
	}

	//! ������Ʒ����
	_AddToByteArray(pStream, (long)m_vecExtraStuff.size());
	for (long i = 0; i < (long)m_vecExtraStuff.size(); ++i)
	{
		_AddToByteArray(pStream, &(m_vecExtraStuff[i]), sizeof(tagExtraStuff));
	}

	//! װ��������Ǯ����
	_AddToByteArray(pStream, (long)m_vecUpgradeMoneySetList.size());
	for (long i = 0; i < (long)m_vecUpgradeMoneySetList.size(); ++i)
	{
		_AddToByteArray(pStream, m_vecUpgradeMoneySetList[i].lMinLimitLevel);
		_AddToByteArray(pStream, m_vecUpgradeMoneySetList[i].lMaxLimitLevel);
		_AddToByteArray(pStream, (long)m_vecUpgradeMoneySetList[i].mapUpgradeMoneySet.size());
		map<long, long>::iterator ite = m_vecUpgradeMoneySetList[i].mapUpgradeMoneySet.begin();
		for ( ; ite != m_vecUpgradeMoneySetList[i].mapUpgradeMoneySet.end(); ++ite)
		{
			_AddToByteArray(pStream, ite->first);
			_AddToByteArray(pStream, ite->second);
		}
	}

	//! װ�����ȼ�
	_AddToByteArray(pStream, m_lUpgradeLimit);	

	//! ħ������
	{
		//! װ���ȼ���Χ����
		_AddToByteArray(pStream, (long)m_ElementSetup.vElementBound.size());
		for (long i = 0; i < (long)m_ElementSetup.vElementBound.size(); ++i)
		{
			_AddToByteArray(pStream, (long)m_ElementSetup.vElementBound[i].vElementStuff.size());
			if(0 < m_ElementSetup.vElementBound[i].vElementStuff.size())
				_AddToByteArray(pStream, &m_ElementSetup.vElementBound[i].vElementStuff[0], sizeof(tagElementStuff) * (long)m_ElementSetup.vElementBound[i].vElementStuff.size());
			_AddToByteArray(pStream, (long)m_ElementSetup.vElementBound[i].vNeedNum.size());
			if(0 < m_ElementSetup.vElementBound[i].vNeedNum.size())
				_AddToByteArray(pStream, &m_ElementSetup.vElementBound[i].vNeedNum[0], sizeof(tagNeedNum) * (long)m_ElementSetup.vElementBound[i].vNeedNum.size());
		}

		//! ħ����������
		_AddToByteArray(pStream, (long)m_ElementSetup.vEquipmentLevelBound.size());
		if(0 < m_ElementSetup.vEquipmentLevelBound.size())
			_AddToByteArray(pStream, &m_ElementSetup.vEquipmentLevelBound[0], sizeof(tagEquipmentLevelBound) * (long)m_ElementSetup.vEquipmentLevelBound.size());

		//! ���ʸ�����������
		_AddToByteArray(pStream, (long)m_ElementSetup.vHelpfulSetup.size());
		for (long i = 0; i < (long)m_ElementSetup.vHelpfulSetup.size(); ++i)
		{
			_AddToByteArray(pStream, m_ElementSetup.vHelpfulSetup[i].lMinLevel);
			_AddToByteArray(pStream, m_ElementSetup.vHelpfulSetup[i].lMaxLevel);
			_AddToByteArray(pStream, (long)m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff.size());
			if(0 < m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff.size())
				_AddToByteArray(pStream, &m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff[0], sizeof(tagHelpfulStuff) * (long)m_ElementSetup.vHelpfulSetup[i].vHelpfulStuff.size());
		}

		//! ħ���ȼ���Ӧħ������ֵ
		_AddToByteArray(pStream, (long)m_ElementSetup.vElementValue.size());
		if(0 < m_ElementSetup.vElementValue.size())
			_AddToByteArray(pStream, &m_ElementSetup.vElementValue[0], sizeof(tagElementValue) * (long)m_ElementSetup.vElementValue.size());

		//! Ԫ����������
		_AddToByteArray(pStream, (long)m_ElementSetup.vResetElement.size());
		for (long i = 0; i < (long)m_ElementSetup.vResetElement.size(); ++i)
		{
			_AddToByteArray(pStream, m_ElementSetup.vResetElement[i].lMinLevel);
			_AddToByteArray(pStream, m_ElementSetup.vResetElement[i].lMaxLevel);
			_AddToByteArray(pStream, (long)m_ElementSetup.vResetElement[i].vResetStuff.size());
			if(0 < m_ElementSetup.vResetElement[i].vResetStuff.size())
				_AddToByteArray(pStream, &m_ElementSetup.vResetElement[i].vResetStuff[0], sizeof(tagResetStuff) * (long)m_ElementSetup.vResetElement[i].vResetStuff.size());
		}

		//! ������������
		_AddToByteArray(pStream, (long)m_ElementSetup.vResetXiangXing.size());
		for (long i = 0; i < (long)m_ElementSetup.vResetXiangXing.size(); ++i)
		{
			_AddToByteArray(pStream, m_ElementSetup.vResetXiangXing[i].lMinLevel);
			_AddToByteArray(pStream, m_ElementSetup.vResetXiangXing[i].lMaxLevel);
			_AddToByteArray(pStream, &m_ElementSetup.vResetXiangXing[i].XiangXingOdds, sizeof(tagXiangXingOdds));
			_AddToByteArray(pStream, (long)m_ElementSetup.vResetXiangXing[i].vResetStuff.size());
			if(0 < m_ElementSetup.vResetXiangXing[i].vResetStuff.size())
				_AddToByteArray(pStream, &m_ElementSetup.vResetXiangXing[i].vResetStuff[0], sizeof(tagResetStuff) * (long)m_ElementSetup.vResetXiangXing[i].vResetStuff.size());
		}
	}

	return true;
}
