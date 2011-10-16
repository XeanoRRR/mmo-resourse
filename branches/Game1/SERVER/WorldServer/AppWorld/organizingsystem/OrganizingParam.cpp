#include "StdAfx.h"
#include "organizingparam.h"
#include "../public/tinystr.h"
#include "../public/tinyxml.h"
#include "../goods/CGoodsFactory.h"

//Function:	��ἰ�����֯�Ĳ�����
//Author:	Joe
//Create Time:2005.3.15



extern HWND g_hWnd;

WSOrganizingParam	g_OrganizingParam;

WSOrganizingParam::WSOrganizingParam(void)
:m_LoadSucceed(FALSE)
{
	memset(m_FunctionWithLevel, 0xff, sizeof(LONG) * MAXNUM_FactionPurview);
}

WSOrganizingParam::~WSOrganizingParam(void)
{
}

//�õ�Ψһʵ�����
//�ú�����֧�ֶ��̲߳���
WSOrganizingParam& WSOrganizingParam:: getInstance()
{
	return g_OrganizingParam;
}

BOOL	WSOrganizingParam::Initialize()
{
	return ReLoad();
}

void	WSOrganizingParam::Release()
{
}

BOOL	WSOrganizingParam::ReLoad()
{
	m_LoadSucceed = FALSE;
	//! ----------------------------���ڵ�----------------------------
	TiXmlNode *pNode = NULL;

	const char *pFilename = "data/Faction.xml";
	CRFile *prfile = rfOpen(pFilename);
	if(NULL == prfile)
	{
		//���������Ϣ
		return FALSE;
	}

	TiXmlDocument m_Tdoc(pFilename);
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		rfClose(prfile);
		return FALSE;
	}
	rfClose(prfile);

	pNode=m_Tdoc.FirstChild("Faction");
	if (pNode==NULL) 
		return FALSE;

	TiXmlElement* pFaction = pNode->ToElement();//��ȡnode ��ָ��
	if (pFaction==NULL) 
		return FALSE;

	//! ----------------------------��ȡ����----------------------------

	try
	{
		//! ����ȫ������
		TiXmlElement* pFacGlobe = pFaction->FirstChildElement("FacGlobe"); 
		m_tagGlobeSetup.lDemiseTime					= atoi(pFacGlobe->Attribute("DemiseTime"));
		m_tagGlobeSetup.lDisbandFactionTime			= atoi(pFacGlobe->Attribute("auto_disband_term"));
		m_tagGlobeSetup.lNegativeTime				= atoi(pFacGlobe->Attribute("auto_degrade_term"));
		m_tagGlobeSetup.lBaseBattleStart			= atoi(pFacGlobe->Attribute("BaseBattleStart"));
		m_tagGlobeSetup.lBaseBattleLast				= atoi(pFacGlobe->Attribute("BaseBattleLast"));
		m_tagGlobeSetup.byClearBaseTime				= (BYTE)atoi(pFacGlobe->Attribute("ClearBaseTime"));

		//! �����صĵȼ���������
		m_vFacLevelSetup.clear();
		TiXmlElement* pFactionLevel = pFaction->FirstChildElement("FactionLevel");
		for (; pFactionLevel!=NULL; pFactionLevel = pFactionLevel->NextSiblingElement("FactionLevel"))
		{
			tagFacLevelSetup FacLevelSetup;
			FacLevelSetup.lLevel			= atoi(pFactionLevel->Attribute("level"));
			FacLevelSetup.lMaxMemNum		= atoi(pFactionLevel->Attribute("maxMemberNum"));
			FacLevelSetup.lDisbandMemNum	= atoi(pFactionLevel->Attribute("DisbandMemNum"));

			TiXmlElement* pUpgradeCondition = pFactionLevel->FirstChildElement("UpgradeCondition");
			for (; pUpgradeCondition!=NULL; pUpgradeCondition = pUpgradeCondition->NextSiblingElement())
			{
				tagUpgradeCondition UpgradeCondition;
				//����Ǯ����Gem����ʯ�������᳤�ȼ�����������ID����
				const char *pTypeTxt = pUpgradeCondition->Attribute("type");
				if (0 == stricmp(pTypeTxt, "Money"))
				{
					UpgradeCondition.lConditionType = eUCT_Money;
				}
				else if (0 == stricmp(pTypeTxt, "LeaderLevel"))
				{
					UpgradeCondition.lConditionType = eUCT_LeaderLevel;
				}
				else if (0 == stricmp(pTypeTxt, "FacRes"))
				{
					UpgradeCondition.lConditionType = eUCT_FacRes;
				}
				else if (0 == stricmp(pTypeTxt, "FacExp"))
				{
					UpgradeCondition.lConditionType = eUCT_FacExp;
				}
				else if (0 == stricmp(pTypeTxt, "BaseNum"))
				{
					UpgradeCondition.lConditionType = eUCT_BaseNum;
				}
				else 
					throw;//! �������ʹ���

				UpgradeCondition.lValue = atoi(pUpgradeCondition->Attribute("value"));

				FacLevelSetup.vCondition.push_back(UpgradeCondition);
			}
			m_vFacLevelSetup.push_back(FacLevelSetup);
		}

		//! ��ᾭ�齱���ȼ�������
		m_vFacExpLevelSetup.clear();
		TiXmlElement* pExpLevel = pFaction->FirstChildElement("ExpLevel");
		for (; pExpLevel!=NULL; pExpLevel = pExpLevel->NextSiblingElement("ExpLevel"))
		{
			tagFacExpLevelSetup FacExpLevelSetup;
			FacExpLevelSetup.lLevel		= atoi(pExpLevel->Attribute("level"));
			FacExpLevelSetup.fBaseExp	= atof(pExpLevel->Attribute("baseExp"));

			TiXmlElement* pUpgradeCondition = pExpLevel->FirstChildElement("UpgradeCondition");
			for (; pUpgradeCondition!=NULL; pUpgradeCondition = pUpgradeCondition->NextSiblingElement())
			{
				tagUpgradeCondition UpgradeCondition;
				//����Ǯ����Gem����ʯ�������᳤�ȼ�����������ID����
				const char *pTypeTxt = pUpgradeCondition->Attribute("type");
				if (0 == stricmp(pTypeTxt, "Money"))
				{
					UpgradeCondition.lConditionType = eUCT_Money;
				}
				else if (0 == stricmp(pTypeTxt, "FacLevel"))
				{
					UpgradeCondition.lConditionType = eUCT_FacLevel;
				}
				else if (0 == stricmp(pTypeTxt, "FacRes"))
				{
					UpgradeCondition.lConditionType = eUCT_FacRes;
				}
				else 
					throw;//! �������ʹ���

				UpgradeCondition.lValue = atoi(pUpgradeCondition->Attribute("value"));
				//��Ӹ�����ߵȼ�
				if (UpgradeCondition.lConditionType == eUCT_FacLevel)
				{
					AddLevel(UpgradeCondition.lValue,eUT_ExpLevel,FacExpLevelSetup.lLevel);
				}
				FacExpLevelSetup.vCondition.push_back(UpgradeCondition);
			}
			m_vFacExpLevelSetup.push_back(FacExpLevelSetup);
		}

		//! ���רҵ�ȼ�������
		m_vFacDepotLevelSetup.clear();
		TiXmlElement* pDepotLevel = pFaction->FirstChildElement("DepotLevel");
		for (; pDepotLevel!=NULL; pDepotLevel = pDepotLevel->NextSiblingElement("DepotLevel"))
		{
			tagFacDepotLevelSetup FacExpLevelSetup;
			FacExpLevelSetup.lLevel		= atoi(pDepotLevel->Attribute("level"));
			FacExpLevelSetup.lCellNum	= atoi(pDepotLevel->Attribute("CellNum"));

			TiXmlElement* pUpgradeCondition = pDepotLevel->FirstChildElement("UpgradeCondition");
			for (; pUpgradeCondition!=NULL; pUpgradeCondition = pUpgradeCondition->NextSiblingElement())
			{
				tagUpgradeCondition UpgradeCondition;
				//����Ǯ����Gem����ʯ�������᳤�ȼ�����������ID����
				const char *pTypeTxt = pUpgradeCondition->Attribute("type");
				if (0 == stricmp(pTypeTxt, "Money"))
				{
					UpgradeCondition.lConditionType = eUCT_Money;
				}
				else if (0 == stricmp(pTypeTxt, "FacLevel"))
				{
					UpgradeCondition.lConditionType = eUCT_FacLevel;
				}
				else if (0 == stricmp(pTypeTxt, "FacRes"))
				{
					UpgradeCondition.lConditionType = eUCT_FacRes;
				}
				else 
					throw;//! �������ʹ���

				UpgradeCondition.lValue = atoi(pUpgradeCondition->Attribute("value"));
				//���רҵ��ߵȼ�
				if (UpgradeCondition.lConditionType == eUCT_FacLevel)
				{
					AddLevel(UpgradeCondition.lValue,eUT_SpecialityLevel,FacExpLevelSetup.lLevel);
				}
				FacExpLevelSetup.vCondition.push_back(UpgradeCondition);
			}
			m_vFacDepotLevelSetup.push_back(FacExpLevelSetup);
		}

		//! ս���ȼ�����
		m_SpecialLevelSetup.clear();
		TiXmlElement* pSmithingLevel = pFaction->FirstChildElement("SmithingLevel");
		for (; pSmithingLevel!=NULL; pSmithingLevel = pSmithingLevel->NextSiblingElement("SmithingLevel"))
		{
			tagFacSubLevelSetup FacSubLevelSetup;
			FacSubLevelSetup.lLevel		= atoi(pSmithingLevel->Attribute("level"));

			TiXmlElement* pUpgradeCondition = pSmithingLevel->FirstChildElement("UpgradeCondition");
			for (; pUpgradeCondition!=NULL; pUpgradeCondition = pUpgradeCondition->NextSiblingElement())
			{
				tagUpgradeCondition UpgradeCondition;
				//����Ǯ����Gem����ʯ�������᳤�ȼ�����������ID����
				const char *pTypeTxt = pUpgradeCondition->Attribute("type");
				if (0 == stricmp(pTypeTxt, "Money"))
				{
					UpgradeCondition.lConditionType = eUCT_Money;
				}
				else if (0 == stricmp(pTypeTxt, "FacLevel"))
				{
					UpgradeCondition.lConditionType = eUCT_FacLevel;
				}
				else if (0 == stricmp(pTypeTxt, "FacRes"))
				{
					UpgradeCondition.lConditionType = eUCT_FacRes;
				}
				else 
					throw;//! �������ʹ���

				UpgradeCondition.lValue = atoi(pUpgradeCondition->Attribute("value"));
				//���ս����ߵȼ�
				if (UpgradeCondition.lConditionType == eUCT_FacLevel)
				{
					AddLevel(UpgradeCondition.lValue,eUT_BattleLevel,FacSubLevelSetup.lLevel);
				}
				FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
			}
			m_SpecialLevelSetup.push_back(FacSubLevelSetup);
		}



		//! ��ҵ�ȼ�����
		m_BussinessLevelSetup.clear();
		TiXmlElement* pMetallurgyLevel = pFaction->FirstChildElement("MetallurgyLevel");
		for (; pMetallurgyLevel!=NULL; pMetallurgyLevel = pMetallurgyLevel->NextSiblingElement("MetallurgyLevel"))
		{
			tagFacSubLevelSetup FacSubLevelSetup;
			FacSubLevelSetup.lLevel		= atoi(pMetallurgyLevel->Attribute("level"));

			TiXmlElement* pUpgradeCondition = pMetallurgyLevel->FirstChildElement("UpgradeCondition");
			for (; pUpgradeCondition!=NULL; pUpgradeCondition = pUpgradeCondition->NextSiblingElement())
			{
				tagUpgradeCondition UpgradeCondition;
				//����Ǯ����Gem����ʯ�������᳤�ȼ�����������ID����
				const char *pTypeTxt = pUpgradeCondition->Attribute("type");
				if (0 == stricmp(pTypeTxt, "Money"))
				{
					UpgradeCondition.lConditionType = eUCT_Money;
				}
				else if (0 == stricmp(pTypeTxt, "FacLevel"))
				{
					UpgradeCondition.lConditionType = eUCT_FacLevel;
				}
				else if (0 == stricmp(pTypeTxt, "FacRes"))
				{
					UpgradeCondition.lConditionType = eUCT_FacRes;
				}
				else 
					throw;//! �������ʹ���

				UpgradeCondition.lValue = atoi(pUpgradeCondition->Attribute("value"));
				//���ս����ߵȼ�
				if (UpgradeCondition.lConditionType == eUCT_FacLevel)
				{
					AddLevel(UpgradeCondition.lValue,eUT_BussinessLevel,FacSubLevelSetup.lLevel);
				}
				FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
			}
			m_BussinessLevelSetup.push_back(FacSubLevelSetup);
		}

		//! ����ȼ�����
		m_NobleLevelSetup.clear();
		TiXmlElement* pSewingLevel = pFaction->FirstChildElement("SewingLevel");
		for (; pSewingLevel!=NULL; pSewingLevel = pSewingLevel->NextSiblingElement("SewingLevel"))
		{
			tagFacSubLevelSetup FacSubLevelSetup;
			FacSubLevelSetup.lLevel		= atoi(pSewingLevel->Attribute("level"));

			TiXmlElement* pUpgradeCondition = pSewingLevel->FirstChildElement("UpgradeCondition");
			for (; pUpgradeCondition!=NULL; pUpgradeCondition = pUpgradeCondition->NextSiblingElement())
			{
				tagUpgradeCondition UpgradeCondition;
				//����Ǯ����Gem����ʯ�������᳤�ȼ�����������ID����
				const char *pTypeTxt = pUpgradeCondition->Attribute("type");
				if (0 == stricmp(pTypeTxt, "Money"))
				{
					UpgradeCondition.lConditionType = eUCT_Money;
				}
				else if (0 == stricmp(pTypeTxt, "FacLevel"))
				{
					UpgradeCondition.lConditionType = eUCT_FacLevel;
				}
				else if (0 == stricmp(pTypeTxt, "FacRes"))
				{
					UpgradeCondition.lConditionType = eUCT_FacRes;
				}
				else 
					throw;//! �������ʹ���

				UpgradeCondition.lValue = atoi(pUpgradeCondition->Attribute("value"));
				//��ӹ�����ߵȼ�
				if (UpgradeCondition.lConditionType == eUCT_FacLevel)
				{
					AddLevel(UpgradeCondition.lValue,eUT_NobleLevel,FacSubLevelSetup.lLevel);
				}
				FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
			}
			m_NobleLevelSetup.push_back(FacSubLevelSetup);
		}

		
		//! ���˴�������
		TiXmlElement* pUnionCreate = pFaction->FirstChildElement("UnionCreate");
		{
			m_vUnionUpgradeCondition.clear();

			TiXmlElement* pUpgradeCondition = pUnionCreate->FirstChildElement("UpgradeCondition");
			for (; pUpgradeCondition!=NULL; pUpgradeCondition = pUpgradeCondition->NextSiblingElement())
			{
				tagUpgradeCondition UpgradeCondition;
				//����Ǯ����Gem����ʯ�������᳤�ȼ�����������ID����
				const char *pTypeTxt = pUpgradeCondition->Attribute("type");
				if (0 == stricmp(pTypeTxt, "Money"))
				{
					UpgradeCondition.lConditionType = eUCT_Money;
				}
				else if (0 == stricmp(pTypeTxt, "LeaderLevel"))
				{
					UpgradeCondition.lConditionType = eUCT_LeaderLevel;
				}
				else 
					throw;//! �������ʹ���


				UpgradeCondition.lValue = atoi(pUpgradeCondition->Attribute("value"));

				m_vUnionUpgradeCondition.push_back(UpgradeCondition);
			}
		}

		//! ���Ṧ�ܿ��ŵĵȼ�����
		TiXmlElement* pLevelFunction = pFaction->FirstChildElement("LevelFunction");
		{
			static char arr_szPurviewType[MAXNUM_FactionPurview][64] = 
			{
				"eFPI_SetAffiche",
				"eFPI_Authorize",
				"eFPI_Fire",
				"eFPI_TakeOut",
				"eFPI_OpenQuest",
				"eFPI_Invite",
				"eFPI_EditTitle",
				"eFPI_EditIcon",
				"eFPI_BeachheadWar",
				"eFPI_ResWar",

				"eFPI_Disband",
				"eFPI_Demise",
				"eFPI_SetMemberPurview",
			};

			TiXmlElement* pLF = pLevelFunction->FirstChildElement("LF");
			for (LONG i = 0; pLF!=NULL || i < MAXNUM_FactionPurview; pLF = pLF->NextSiblingElement("LF"), ++i)
			{
				const char* pTxtType = pLF->Attribute("type");
				for (LONG lFPI = eFPI_SetAffiche; lFPI < MAXNUM_FactionPurview; ++lFPI )
				{
					if (0 == stricmp(arr_szPurviewType[lFPI], pTxtType))
					{
						m_FunctionWithLevel[lFPI] = atoi(pLF->Attribute("level"));
						break;
					}
				}
			}
		}

		TiXmlElement* pConsume=pFaction->FirstChildElement("DailyConsume");
		TiXmlElement* pLv=pConsume->FirstChildElement("Level");
		tagBaseConsume tgConsume;
		LONG lBussLv=0,lBaseVal=0;
		for (;pLv!=NULL;pLv=pLv->NextSiblingElement())
		{
			if (0==stricmp("Level",pLv->Value()) )
			{
				lBussLv=atoi(pLv->Attribute("Lv"));
				tgConsume._BaseVal=atoi(pLv->Attribute("BaseVal"));
				tgConsume._Critical=atoi(pLv->Attribute("Critical"));
				m_BaseConsume[lBussLv]=tgConsume;
			}
			else if (0==stricmp("BusinessLv",pLv->Value()) )
			{
				lBussLv=atoi(pLv->Attribute("Lv"));
				lBaseVal=atoi(pLv->Attribute("BaseVal"));
				m_BussinessBase[lBussLv]=lBaseVal;
			}
		}
	}
	catch (...)
	{
		AddErrorLogText("�ڶ�ȡ�������õ�ʱ�����˴��󣬶�ȡʧ�ܣ����ܵ�ԭ�������ò�����");
		return FALSE;
	}
	

	//! ----------------------------�������----------------------------
	m_LoadSucceed = TRUE;
	return m_LoadSucceed;
}

//! ��ӵ���Ϣ����
BOOL WSOrganizingParam::CodeToMsgSet(DBWriteSet &DbWriteSet)
{
	if (m_LoadSucceed)
	{
		//! ����ȫ������
		DbWriteSet.AddToByteArray(&m_tagGlobeSetup, sizeof(tagGlobeSetup));

		//! �����صĵȼ���������
		DbWriteSet.AddToByteArray((LONG)m_vFacLevelSetup.size());
		for (LONG i = 0; i < m_vFacLevelSetup.size(); ++i)
		{
			DbWriteSet.AddToByteArray(m_vFacLevelSetup[i].lLevel);
			DbWriteSet.AddToByteArray(m_vFacLevelSetup[i].lMaxMemNum);
			DbWriteSet.AddToByteArray(m_vFacLevelSetup[i].lDisbandMemNum);
			DbWriteSet.AddToByteArray((LONG)m_vFacLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_vFacLevelSetup[i].vCondition.size(); ++j)
			{
				DbWriteSet.AddToByteArray(&m_vFacLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//! ���˴�������
		DbWriteSet.AddToByteArray((LONG)m_vUnionUpgradeCondition.size());
		for (LONG i = 0; i < m_vUnionUpgradeCondition.size(); ++i)
		{
			DbWriteSet.AddToByteArray(&m_vUnionUpgradeCondition[i], sizeof(tagUpgradeCondition));
		}

		//! ���Ṧ�ܿ��ŵĵȼ�����;
		DbWriteSet.AddToByteArray(&m_FunctionWithLevel, sizeof(LONG) * MAXNUM_FactionPurview);

		//! ��ᾭ�齱���ȼ�������
		DbWriteSet.AddToByteArray((LONG)m_vFacExpLevelSetup.size());
		for (LONG i = 0; i < m_vFacExpLevelSetup.size(); ++i)
		{
			DbWriteSet.AddToByteArray(m_vFacExpLevelSetup[i].lLevel);
			DbWriteSet.AddToByteArray(m_vFacExpLevelSetup[i].fBaseExp);
			DbWriteSet.AddToByteArray((LONG)m_vFacExpLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_vFacExpLevelSetup[i].vCondition.size(); ++j)
			{
				DbWriteSet.AddToByteArray(&m_vFacExpLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//! רҵ�ȼ�������
		DbWriteSet.AddToByteArray((LONG)m_vFacDepotLevelSetup.size());
		for (LONG i = 0; i < m_vFacDepotLevelSetup.size(); ++i)
		{
			DbWriteSet.AddToByteArray(m_vFacDepotLevelSetup[i].lLevel);
			DbWriteSet.AddToByteArray(m_vFacDepotLevelSetup[i].lCellNum);
			DbWriteSet.AddToByteArray((LONG)m_vFacDepotLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_vFacDepotLevelSetup[i].vCondition.size(); ++j)
			{
				DbWriteSet.AddToByteArray(&m_vFacDepotLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ��ҵ�ȼ�����
		DbWriteSet.AddToByteArray((LONG)m_BussinessLevelSetup.size());
		for (LONG i = 0; i < m_BussinessLevelSetup.size(); ++i)
		{
			DbWriteSet.AddToByteArray(m_BussinessLevelSetup[i].lLevel);
			DbWriteSet.AddToByteArray((LONG)m_BussinessLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_BussinessLevelSetup[i].vCondition.size(); ++j)
			{
				DbWriteSet.AddToByteArray(&m_BussinessLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ս���ȼ�����
		DbWriteSet.AddToByteArray((LONG)m_SpecialLevelSetup.size());
		for (LONG i = 0; i < m_SpecialLevelSetup.size(); ++i)
		{
			DbWriteSet.AddToByteArray(m_SpecialLevelSetup[i].lLevel);
			DbWriteSet.AddToByteArray((LONG)m_SpecialLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_SpecialLevelSetup[i].vCondition.size(); ++j)
			{
				DbWriteSet.AddToByteArray(&m_SpecialLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ����ȼ�����
		DbWriteSet.AddToByteArray((LONG)m_NobleLevelSetup.size());
		for (LONG i = 0; i < m_NobleLevelSetup.size(); ++i)
		{
			DbWriteSet.AddToByteArray(m_NobleLevelSetup[i].lLevel);
			DbWriteSet.AddToByteArray((LONG)m_NobleLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_NobleLevelSetup[i].vCondition.size(); ++j)
			{
				DbWriteSet.AddToByteArray(&m_NobleLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//�����ճ�����
		DbWriteSet.AddToByteArray((long)m_BaseConsume.size());
		map<LONG,tagBaseConsume>::iterator itBase=m_BaseConsume.begin();
		for (;itBase!=m_BaseConsume.end();itBase++)
		{
			DbWriteSet.AddToByteArray(itBase->first);
			DbWriteSet.AddToByteArray(&(itBase->second),sizeof(tagBaseConsume));
		}
		//��ҵ�ȼ���Ӧ�Ļ���
		DbWriteSet.AddToByteArray((long)m_BussinessBase.size());
		map<LONG,LONG>::iterator itBussBase=m_BussinessBase.begin();
		for (;itBussBase!=m_BussinessBase.end();itBussBase++)
		{
			DbWriteSet.AddToByteArray(itBussBase->first);
			DbWriteSet.AddToByteArray(itBussBase->second);
		}
	}
	return m_LoadSucceed;
}

//! ��õȼ�����
const tagFacLevelSetup* WSOrganizingParam::GetLevelSetup(LONG lLevel)
{
	if (m_LoadSucceed)
	{
		for (LONG i = 0; i < m_vFacLevelSetup.size(); ++i)
		{
			if (lLevel == m_vFacLevelSetup[i].lLevel)
			{
				return &m_vFacLevelSetup[i];
			}
		}
	}
	
	return NULL;
}

//! �õ�һ������ȼ����ŵ����л�ԱȨ��
LONG WSOrganizingParam::GetCurrLvlAllPurview(LONG lCurrLvl)
{
	LONG lRe = 0;

	for (LONG i = 0; i < MAXNUM_MemberPurview; ++i)
	{
		if(m_FunctionWithLevel[i] <= lCurrLvl)
		{
			lRe |= (1 << i);
		}
	}

	return lRe;
}

//! �õ�һ���������͵�����
const VecCondition* WSOrganizingParam::GetUpgradeVecCondition(DWORD dwUpgradeType, LONG lAimLevel)
{
	if (m_LoadSucceed)
	{
		switch(dwUpgradeType)
		{
		case eUT_FactionLevel:		//! ����ȼ�
			{
				for (LONG i = 0; i < m_vFacLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacLevelSetup[i].lLevel)
					{
						return &(m_vFacLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_ExpLevel:			//! ���鸣���ȼ�
			{
				for (LONG i = 0; i < m_vFacExpLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacExpLevelSetup[i].lLevel)
					{
						return &(m_vFacExpLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_SpecialityLevel:		//! רҵ�ȼ�
			{
				for (LONG i = 0; i < m_vFacDepotLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacDepotLevelSetup[i].lLevel)
					{
						return &(m_vFacDepotLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_BattleLevel:		//! ս���ȼ�
			{
				for (LONG i = 0; i < m_SpecialLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_SpecialLevelSetup[i].lLevel)
					{
						return &(m_SpecialLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_BussinessLevel:	//! ��ҵ�ȼ�
			{
				for (LONG i = 0; i < m_BussinessLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_BussinessLevelSetup[i].lLevel)
					{
						return &(m_BussinessLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_NobleLevel:		//! ����ȼ�
			{
				for (LONG i = 0; i < m_NobleLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_NobleLevelSetup[i].lLevel)
					{
						return &(m_NobleLevelSetup[i].vCondition);
					}
				}
			}
			break;
		}
	}

	return NULL;
}

void WSOrganizingParam::AddLevel(DWORD dwFacLv,eUpgradeType eType,DWORD dwLv)
{
	map<LONG,tagTechInfo>::iterator it=m_TechInfo.find(dwFacLv);
	if (it!=m_TechInfo.end())
	{
		switch(eType)
		{
		case eUT_ExpLevel:
			it->second._lSubExpLv=max(it->second._lSubExpLv,dwLv);
			break;
		case eUT_SpecialityLevel:													
			it->second._lSpecialityLv=max(it->second._lSpecialityLv,dwLv);
			break;
		case eUT_BattleLevel:	
			it->second._lBattleLv=max(it->second._lBattleLv,dwLv);
			break;
		case eUT_BussinessLevel:	
			it->second._lBussinessLv=max(it->second._lBussinessLv,dwLv);	
			break;
		case eUT_NobleLevel:
			it->second._lNoblelv=max(it->second._lNoblelv,dwLv);
			break;
		}
	}
	else
	{
		tagTechInfo techinfo;
		switch(eType)
		{
		case eUT_ExpLevel:
			techinfo._lSubExpLv=dwLv;
			break;
		case eUT_SpecialityLevel:													
			techinfo._lSpecialityLv=dwLv;
			break;
		case eUT_BattleLevel:	
			techinfo._lBattleLv=dwLv;
			break;
		case eUT_BussinessLevel:	
			techinfo._lBussinessLv=dwLv;	
			break;
		case eUT_NobleLevel:
			techinfo._lNoblelv=dwLv;
			break;
		}
		m_TechInfo[dwFacLv]=techinfo;
	}
}

const tagBaseConsume* WSOrganizingParam::GetConsume(DWORD dwLv)
{
	tagBaseConsume* pC=NULL;
	map<LONG,tagBaseConsume>::iterator it=m_BaseConsume.find(dwLv);
	if (it!=m_BaseConsume.end())
	{
		pC=&(it->second);
	}
	return pC;
}

float WSOrganizingParam::GetBussBase(DWORD dwLv)
{
	LONG lBase=0;
	if(m_BussinessBase.find(dwLv)!=m_BussinessBase.end())
		lBase=m_BussinessBase[dwLv];
	return lBase;
}