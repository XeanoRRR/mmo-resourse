///////////////////////////////////////////////////////////////////////////////
///  NewSkillFactory.cpp
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\WorldServer\AppWorld\New Skill
///
///  CREATED: 08/07/2007 12:50:01 PM by ���Ƚ�
///
///  PURPOSE:   ���ܹ�����
///
// NewSkillFactory.h: interface for the CNewSkillFactory class
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "NewSkillFactory.h"
#include "../../../../public/CStrConverter.h"



map<DWORD, CNewSkill*>	CNewSkillFactory::g_mapNewSkill;
DWORD					CNewSkillFactory::g_dwVersion = 0;
map<DWORD, TiXmlElement*>		CNewSkillFactory::g_mapTemplateElem;
map<DWORD, TiXmlElement*>		CNewSkillFactory::g_mapComTemplateElem;
list<long>						CNewSkillFactory::g_lInitSkillList;
map<DWORD,list<long>>				CNewSkillFactory::g_mapOccuInitSkill;
/*
���뼼�������ļ�
*/
bool CNewSkillFactory::LoadNewSkillCache()
{
	ClearNewSkillCache();

	// ��ȡ���������ļ�
	list<string> listFileName;	
	LoadSkillList("skill/SkillList.xml",&listFileName);


	bool bResult = true;
	for( list<string>::iterator it = listFileName.begin(); it != listFileName.end(); it++ )
	{
		const char* pFileName = it -> c_str();

		//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
		char strTest[256];
		sprintf(strTest,"\n ��ȡ�������ã�%s\n",pFileName);
		OutputDebugString(strTest);
#endif
		//////////////////////////////////////////////////////////////////////////


		//����һ�������ļ�		
		if( FAILED( LoadConfigration( pFileName ) ) )
		{

			char  str[256];
			sprintf(str,CStringReading::LoadString(IDS_WS_SKILL,STR_WS_SKILL_THIRD),pFileName);
			bResult = false;
			AddLogText(str);	
		}	
	}
	LoadInitSkillConfig("data/InitSkill.xml");
	return bResult;
}




/*
NAME:LoadConfigration
DESC:��ȡ���������ļ�
PARA:pFileName,�����ļ�����
*/
HRESULT CNewSkillFactory::LoadConfigration( const string pFileName )
{
	if( pFileName.empty()) 
		return E_FAIL;

	CRFile *prfile = rfOpen(pFileName.c_str());
	if(NULL == prfile)	return E_FAIL;

	TiXmlNode *pNode = NULL;
	TiXmlDocument doc(pFileName.c_str());

	//���뼼�������ļ�
	doc.LoadData(prfile->GetData(),prfile->GetDatalen());
	rfClose(prfile);

	//ȡ�õ�һ���ڵ�
	pNode = doc.FirstChild("Module");	

	if (NULL == pNode)
	{
		return E_FAIL;
	}

	TiXmlElement *pElem = pNode->ToElement();

	if(LoadConfigration(pElem))
	{
		return S_OK;	
	}
	else
	{
		return E_FAIL;
	}
}
//��ȡ˫��
DWORD CNewSkillFactory::GetDwordAttribute(TiXmlElement *pElem,string strName)
{
	double dValue;
	if(pElem->QueryDoubleAttribute(strName.c_str(),&dValue))
	{
		dValue = 0;
	}
	return dValue;
}
//��ȡ��ʼ��ְҵ����
list<long>	CNewSkillFactory::GetOccuSkillList(DWORD dwOccu)
{
	list<long> lSkill;
	map<DWORD,list<long>>::iterator it = g_mapOccuInitSkill.find(dwOccu);
	if (it!=g_mapOccuInitSkill.end())
	{
		return it->second;
	}
	return lSkill;
}
//��ȡλ��
BYTE CNewSkillFactory::GetIntAttribute(TiXmlElement *pElem,string strName)
{
	int iValue;
	if(pElem->QueryIntAttribute(strName.c_str(),&iValue))
	{
		iValue = 0;
	}
	return iValue;
}

//��ȡ�ַ���
string CNewSkillFactory::GetStringAttribute(TiXmlElement *pElem,string strName)
{
	string newStr;
	if (NULL != pElem->Attribute(strName.c_str()))
	{
		newStr = pElem->Attribute(strName.c_str());
	} 
	else
	{
		newStr = "";
	}
	return newStr;
}
//��ȡ�ı�
string CNewSkillFactory::GetTextAttribute(TiXmlElement *pElem)
{
	string strValue;
	if (NULL != pElem->GetText())
	{
		strValue = pElem->GetText();
	} 
	else
	{
		strValue = "";
	}
	return strValue;
}

//�����б�
HRESULT	CNewSkillFactory::LoadSkillList(const string pFileName, list<string>* listFileName)
{
	if( pFileName.empty()) 
		return E_FAIL;

	CRFile *prfile = rfOpen(pFileName.c_str());
	if(NULL == prfile)	return E_FAIL;

	TiXmlNode *pNode = NULL;
	TiXmlDocument doc(pFileName.c_str());

	//���뼼�������ļ�
	doc.LoadData(prfile->GetData(),prfile->GetDatalen());
	rfClose(prfile);

	//ȡ�õ�һ���ڵ�
	pNode = doc.FirstChild("SkillList");	

	if (NULL == pNode)
	{
		return E_FAIL;
	}

	TiXmlElement *pElem = pNode->ToElement();

	LoadSkillListConfigration(pElem,listFileName);

	return S_OK;	
}

void CNewSkillFactory::LoadSkillListConfigration(TiXmlElement *pElem,list<string>* listFileName)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{

		if (!stricmp(pChildElem->Value(),"Skill"))
		{
			string strSkillName = "skill\\";
			strSkillName = strSkillName + GetStringAttribute(pChildElem,"name");
			listFileName->push_back(strSkillName);			
		}
	}
}


HRESULT CNewSkillFactory::LoadInitSkillConfig( const string pFileName )
{
	if( pFileName.empty()) 
		return E_FAIL;

	CRFile *prfile = rfOpen(pFileName.c_str());
	if(NULL == prfile)	return E_FAIL;

	TiXmlNode *pNode = NULL;
	TiXmlDocument doc(pFileName.c_str());

	//���뼼�������ļ�
	doc.LoadData(prfile->GetData(),prfile->GetDatalen());
	rfClose(prfile);

	//ȡ�õ�һ���ڵ�
	pNode = doc.FirstChild("InitSkill");	

	if (NULL == pNode)
	{
		return E_FAIL;
	}

	TiXmlElement *pElem = pNode->ToElement();

	LoadInitSkillConfigration(pElem);

	return S_OK;	
}

void CNewSkillFactory::LoadInitSkillConfigration(TiXmlElement *pElem)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{

		if (!stricmp(pChildElem->Value(),"Skill"))
		{
			long lSkillID = GetDwordAttribute(pChildElem,"id");
			g_lInitSkillList.push_back(lSkillID);
		}
		else if (!stricmp(pChildElem->Value(),"Occu"))
		{
			DWORD dwOccuID = GetDwordAttribute(pChildElem,"id");
			list<long> lSkillID;
			for (TiXmlElement *pSubElem=pChildElem->FirstChildElement(); pSubElem!=NULL; 
				pSubElem=pSubElem->NextSiblingElement())
			{
				if (!stricmp(pSubElem->Value(),"Skill"))
				{
					long lInitSkill = GetDwordAttribute(pSubElem,"id");
					lSkillID.push_back(lInitSkill);
				}
			}
			g_mapOccuInitSkill[dwOccuID]=lSkillID;
		}
	}
}

/*
NAME:LoadConfigration
DESC:ͨ���ڵ�����
PARA:pElem�ڵ�Ԫ��
*/
bool	CNewSkillFactory::LoadConfigration	( TiXmlElement *pElem )
{
	DWORD dwID = 0;	
	string strName;
	string strDesc;
	DWORD  dwElement;
	string strType;
	DWORD  dwUseType;
	BYTE   bOnly;
	DWORD  dwOnlyType;
	BYTE   bNameOnly;
	BYTE   bRoleOnly;
	DWORD  dwTotalNum;
	string strAddSub;	
	BYTE	bAttack;
	BYTE	bTarget;
	BYTE	bCoolDown;
	BYTE	bDeadUse;
	BYTE	bDeadEnd;
	BYTE	bBreak;
	BYTE	bPetType;
	BYTE	bTime;
	BYTE	bSystem;

	static CNewSkill *pNewSkill	= NULL;
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{

		if (!stricmp(pChildElem->Value(),"BaseModule"))
		{
			strName = GetStringAttribute(pChildElem,"name");
			dwID = GetDwordAttribute(pChildElem, "id");
			strDesc = GetStringAttribute(pChildElem,"sriptparam");	
			dwElement = GetDwordAttribute(pChildElem,"element"); 
			strType	= GetStringAttribute(pChildElem,"type");		
			dwUseType = GetDwordAttribute(pChildElem,"usetype");	
			bOnly = GetIntAttribute(pChildElem,"only");
			dwOnlyType = GetDwordAttribute(pChildElem,"onlytype");
			bNameOnly = GetIntAttribute(pChildElem,"nameonly");
			bRoleOnly = GetIntAttribute(pChildElem,"roleonly");
			dwTotalNum = GetDwordAttribute(pChildElem,"totalnum");
			strAddSub = GetStringAttribute(pChildElem,"addsub");
			bAttack	= GetIntAttribute(pChildElem,"attack");
			bTarget = GetIntAttribute(pChildElem,"target");
			bCoolDown = GetIntAttribute(pChildElem,"cooldown");
			bDeadUse = GetIntAttribute(pChildElem,"deaduse");
			bDeadEnd = GetIntAttribute(pChildElem,"deadend");
			bBreak	= GetIntAttribute(pChildElem,"break");
			bPetType = GetIntAttribute(pChildElem,"pettype");
			bTime	= GetIntAttribute(pChildElem,"time");
			bSystem = GetIntAttribute(pChildElem,"system");


			


			
			map<DWORD, CNewSkill*>::iterator it = g_mapNewSkill.find(dwID);
			if (it!=g_mapNewSkill.end())
			{
				char str[256];
				sprintf(str,CStringReading::LoadString(IDS_WS_SKILL,STR_WS_SKILL_EIGHTH),dwID);	
				PutDebugString(str);
				return false;
			} 
			else
			{
				pNewSkill = MP_NEW CNewSkill(dwID);			//����״̬
				g_mapNewSkill[dwID] = pNewSkill;
			}
			
			pNewSkill->SetNewSkillName(strName);
			pNewSkill->SetDescription(strDesc);
			pNewSkill->SetElement(dwElement);
			pNewSkill->SetSkillType(strType);
			pNewSkill->SetUseType(dwUseType);
			pNewSkill->SetOnly(bOnly);	
			pNewSkill->SetOnlyType(dwOnlyType);
			pNewSkill->SetNameOnly(bNameOnly);
			pNewSkill->SetRoleOnly(bRoleOnly);
			pNewSkill->SetTotalNum(dwTotalNum);
			pNewSkill->SetAddSub(strAddSub);		
			pNewSkill->SetAttack(bAttack);
			pNewSkill->SetTarget(bTarget);
			pNewSkill->SetCoolDown(bCoolDown);
			pNewSkill->SetDeadUse(bDeadUse);
			pNewSkill->SetDeadEnd(bDeadEnd);
			pNewSkill->SetBreak(bBreak);
			pNewSkill->SetPetType(bPetType);
			pNewSkill->SetTime(bTime);
			pNewSkill->SetSystem(bSystem);

			LoadConfigration(pChildElem);		
		}	
		else if (!stricmp(pChildElem->Value(),"Template"))
		{		
			LoadTemplateConfig(pChildElem);			
		}	
		else if(!stricmp(pChildElem->Value(),"CommonTemplate"))
		{
			LoadComTemplate(pChildElem);
		}	
		else if(!stricmp(pChildElem->Value(),"ComTemplate"))
		{
			DWORD dwTempID = GetDwordAttribute(pChildElem,"id");
			TiXmlElement *pTElem = g_mapComTemplateElem[dwTempID];
			if (pTElem)
			{
				LoadComTemplate(pTElem,pNewSkill);
			}
		}
		else if (!stricmp(pChildElem->Value(),"Level"))
		{ 
			//�����ȼ��ļ���
			LoadNewLevelConfigration(pChildElem,pNewSkill);

		}
	}	
	return true;
}

//��ȡ����ģ��
void CNewSkillFactory::LoadComTemplate(TiXmlElement *pElem,CNewSkill *pNewSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Expression"))
		{
			//��ʽ
			LoadExpresstionConfigration(pChildElem,pNewSkill);	
		}
		else if (!stricmp(pChildElem->Value(),"Condition"))
		{
			//������
			string strType;
			DWORD  dwID;		
			strType = GetStringAttribute(pChildElem,"type");
			dwID	= GetDwordAttribute(pChildElem,"id");			
			pNewSkill->SetCondition(dwID,LoadConditionConfigration(pChildElem,strType,dwID));	
		}
		else if (!stricmp(pChildElem->Value(),"ConditionGroup"))
		{
			//������
			LoadConditionGroupfigration(pChildElem,pNewSkill);
		}		
		else if (!stricmp(pChildElem->Value(),"OperatObjects"))
		{
			LoadOperatObjects(pChildElem,pNewSkill);
		}
	}
}
//�����ȼ�������
void CNewSkillFactory::LoadNewLevelConfigration(TiXmlElement *pElem,CNewSkill *pNewSkill)
{
	DWORD dwLevel;
	DWORD dwTemplateID;
	TiXmlElement *pTeplateElem = NULL;
	map<string,string>				mapVarablist;				//�����б�

	dwLevel = GetDwordAttribute(pElem,"value");
	dwTemplateID = GetDwordAttribute(pElem,"tempid");

	map<DWORD, TiXmlElement*>::iterator it = g_mapTemplateElem.find(dwTemplateID);
	if (it!=g_mapTemplateElem.end())
	{
		pTeplateElem = it->second;
	}

	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if(!stricmp(pChildElem->Value(),"Variablelist"))
		{				
			string strVaName = GetStringAttribute(pChildElem,"name");
			string  strValue	= GetStringAttribute(pChildElem,"value");
			mapVarablist[strVaName] = strValue;
		}
	}
	if (pTeplateElem)
	{
		LoadLevelConfigration(pTeplateElem,pNewSkill,dwLevel,mapVarablist);
	}
}
//��Ҫ�ṹ
void CNewSkillFactory::LoadLevelConfigration(TiXmlElement *pElem, CNewSkill *pNewSkill,DWORD dwLevel,map<string,string>	mpVarablist )
{

	CNewSkill::tagSkill *ptgSkill = OBJ_CREATE(CNewSkill::tagSkill);
	ptgSkill->mapVarible = mpVarablist;
	ptgSkill->pStaticParam = NULL;

	ptgSkill->dwLevel = dwLevel;

	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		//���ܾ�̬����
		if(!stricmp(pChildElem->Value(),"StaticParam"))
		{
			LoadLevelStatiParam(pChildElem,ptgSkill,mpVarablist);
		}
		else if (!stricmp(pChildElem->Value(),"ComTemplate"))
		{
			DWORD dwID = GetDwordAttribute(pChildElem,"id");
			TiXmlElement *pSubElem = g_mapComTemplateElem[dwID];
			LoadCommonConfig(pSubElem,pNewSkill,ptgSkill,mpVarablist);
		}

		else if (!stricmp(pChildElem->Value(),"CProcedure"))
		{
			//���̽׶�
			LoadProcesseExecute(pChildElem,ptgSkill,pNewSkill);
		}
	}
	pNewSkill->AddNewSkill(ptgSkill);
}

//��������
void	CNewSkillFactory::LoadCommonConfig( TiXmlElement *pElem, CNewSkill *pNewSkill,CNewSkill::tagSkill *ptgSkill,map<string,string>	mpVarablist)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Condition"))
		{
			//������
			string strType;
			DWORD  dwID;		
			strType = GetStringAttribute(pChildElem,"type");
			dwID	= GetDwordAttribute(pChildElem,"id");					   		
			ptgSkill->mapCondition[dwID] = LoadConditionConfigration(pChildElem,strType,dwID,ptgSkill);
		}
	}
}

//������
void CNewSkillFactory::LoadConditionGroupfigration(TiXmlElement *pElem,CNewSkill *pNewSkill)
{
	DWORD dwGroupID = GetDwordAttribute(pElem,"id");
	CNewSkill::tagConditionGroup *ptgConGroup = OBJ_CREATE(CNewSkill::tagConditionGroup);
	ptgConGroup->strMode = GetStringAttribute(pElem,"mode");
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"ConditionID"))
		{
			DWORD dwConID = GetDwordAttribute(pChildElem ,"value");
			ptgConGroup->lConditonGroup.push_back(dwConID);
		}
	}
	pNewSkill->SetConGroup(dwGroupID,ptgConGroup);
}

//����
CNewSkill::tagCondition *CNewSkillFactory::LoadConditionConfigration(TiXmlElement *pElem,string strType,DWORD dwID,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagCondition *ptgCondition = OBJ_CREATE(CNewSkill::tagCondition);
	if (strType=="BaseProperty")
	{
		//������������		
		CNewSkill::tagConProperty *ptgConProperty = OBJ_CREATE(CNewSkill::tagConProperty);		
		LoadPropertyCondition(pElem,ptgConProperty,ptgSkill); 
		ptgCondition->dwLength = ptgConProperty->ltgProtertyConditon.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConProperty) ;
	} 
	else if(strType=="Good")
	{
		//��Ʒ����		
		CNewSkill::tagConGood *ptgConGood = OBJ_CREATE(CNewSkill::tagConGood);
		LoadGoodCondition(pElem,ptgConGood,ptgSkill);
		ptgCondition->dwLength = ptgConGood->ltgGoodCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConGood);
	}
	else if (strType=="Buff")
	{
		//״̬����
		CNewSkill::tagConBuff *ptgConBuff = OBJ_CREATE(CNewSkill::tagConBuff);
		LoadBuffCondition(pElem,ptgConBuff,ptgSkill);
		ptgCondition->dwLength = ptgConBuff->ltgBuffCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConBuff);		
	}
	else if (strType=="Skill")
	{
		//���뼼������
		CNewSkill::tagConSkill *ptgConSkill = OBJ_CREATE(CNewSkill::tagConSkill);
		LoadSkillCondition(pElem,ptgConSkill,ptgSkill);
		ptgCondition->dwLength = ptgConSkill->ltgSkillCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConSkill);
	}
	else if (strType=="Distance")
	{
		//�����������
		CNewSkill::tagConDistance *ptgConDistance = OBJ_CREATE(CNewSkill::tagConDistance);
		LoadDistanceCondition(pElem,ptgConDistance,ptgSkill);
		ptgCondition->dwLength = ptgConDistance->ltgDistanceCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConDistance);
	}
	else if (strType=="Event")
	{
		//�¼�
		CNewSkill::tagConEvent *ptgConEvent = OBJ_CREATE(CNewSkill::tagConEvent);
		LoadEventCondition(pElem,ptgConEvent,ptgSkill);
		ptgCondition->dwLength = ptgConEvent->ltgEventCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConEvent);

	} 
	else if(strType=="ValidObject")
	{
		//����
		CNewSkill::tagConValidObject *ptgConValidObject = OBJ_CREATE(CNewSkill::tagConValidObject);
		LoadValidObject(pElem,ptgConValidObject,ptgSkill);
		ptgCondition->dwLength = ptgConValidObject->ltgValidObjectCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConValidObject);
	}
	else if (strType=="Script")
	{
		//�ű�
		CNewSkill::tagConScript *ptgConScript =OBJ_CREATE(CNewSkill::tagConScript);
		LoadScript(pElem,ptgConScript,ptgSkill);
		ptgCondition->dwLength = ptgConScript->strScript.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgConScript);
	}
	else if (strType=="Timer")
	{
		CNewSkill::tagConTimer *ptgTimer = OBJ_CREATE(CNewSkill::tagConTimer);
		LoadTimerCondition(pElem,ptgTimer,ptgSkill);
		ptgCondition->dwLength = ptgTimer->ldwConTimer.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgTimer);
	}
	else if (strType=="CoolDown")
	{
		ptgCondition->dwLength = 0;
		ptgCondition->strType = strType;
		ptgCondition->pVoidCondition = NULL;
	}
	else if (strType=="Block")
	{
		CNewSkill::tagConBlock *ptgBlock = OBJ_CREATE(CNewSkill::tagConBlock);
		LoadBlockCondition(pElem,ptgBlock,ptgSkill);
		ptgCondition->dwLength = ptgBlock->ltgBlockCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgBlock);
	}
	else if (strType=="Face")
	{
		CNewSkill::tagConFace *ptgFace = OBJ_CREATE(CNewSkill::tagConFace);
		LoadFaceCondition(pElem,ptgFace,ptgSkill);
		ptgCondition->dwLength = ptgFace->ltgFaceCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgFace);
	}
	else if (strType=="Region")
	{
		CNewSkill::tagConRegion *ptgRegion = OBJ_CREATE(CNewSkill::tagConRegion);
		LoadRegionCondition(pElem,ptgRegion,ptgSkill);
		ptgCondition->dwLength = ptgRegion->ltgRegionCondition.size();
		ptgCondition->pVoidCondition = reinterpret_cast<void*>(ptgRegion);
	}

	if (ptgCondition)
	{
		ptgCondition->strType = strType;
		return ptgCondition;
	}
	return NULL;
}

//����ʱ������
void CNewSkillFactory::LoadTimerCondition(TiXmlElement *pElem,CNewSkill::tagConTimer *ptgTimer,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			DWORD dwFlag = GetDwordAttribute(pChildElem,"flag");
			ptgTimer->ldwConTimer.push_back(dwFlag);
		}
	}
}


//�����¼�
void CNewSkillFactory::LoadEventCondition(TiXmlElement *pElem,CNewSkill::tagConEvent *ptgConEvent,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagEventCondition *ptgEventConditon = OBJ_CREATE(CNewSkill::tagEventCondition);

			ptgEventConditon->strEventName  = GetStringAttribute(pChildElem,"eventtype");
			ptgEventConditon->dwFlag = GetDwordAttribute(pChildElem,"flag");
			ptgEventConditon->dwVar1 = GetDwordAttribute(pChildElem,"var1");			
			ptgEventConditon->dwVar2 = GetDwordAttribute(pChildElem,"var2");
			ptgEventConditon->dwVar3 = GetDwordAttribute(pChildElem,"var3");
			ptgEventConditon->dwVar4 = GetDwordAttribute(pChildElem,"var4");
			ptgEventConditon->strName = GetStringAttribute(pChildElem,"var5");			
			ptgConEvent->ltgEventCondition.push_back(ptgEventConditon);
		}
	}

}

//�������
void CNewSkillFactory::LoadValidObject(TiXmlElement *pElem,CNewSkill::tagConValidObject *ptgConValidObject,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagValidObjectCondition *ptgValidObjectCondition = OBJ_CREATE(CNewSkill::tagValidObjectCondition);

			ptgValidObjectCondition->strType = GetStringAttribute(pChildElem,"type");

			for (TiXmlElement *pSubElem=pChildElem->FirstChildElement(); pSubElem!=NULL; 
				pSubElem=pSubElem->NextSiblingElement())
			{
				if (!stricmp(pSubElem->Value(),"Object"))
				{
					string strObjectType = GetStringAttribute(pSubElem,"value");
					ptgValidObjectCondition->lObjType.push_back(strObjectType);
				}
			}

			ptgConValidObject->ltgValidObjectCondition.push_back(ptgValidObjectCondition);
		}
	}
}

void CNewSkillFactory::LoadOccupationCondition(TiXmlElement *pElem,CNewSkill::tagConOccupation *ptgConOccupation,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagOccupationCondition *ptgOccupationCondition = OBJ_CREATE(CNewSkill::tagOccupationCondition);
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			long lOccupationID = GetDwordAttribute(pChildElem,"value");
			ptgOccupationCondition->lOccu.push_back(lOccupationID);
		}
	}
	ptgConOccupation->ltgOccupationCondition.push_back(ptgOccupationCondition);
}


//����ű�
void CNewSkillFactory::LoadScript(TiXmlElement *pElem,CNewSkill::tagConScript *ptgConScript,CNewSkill::tagSkill *ptgSkill)
{	
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			ptgConScript->strScript = GetStringAttribute(pChildElem,"value");
			break;
		}

	}


}
//�赲
void CNewSkillFactory::LoadBlockCondition(TiXmlElement *pElem,CNewSkill::tagConBlock *ptgConBlock,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagBlockCondition *ptgBlockCondition = OBJ_CREATE(CNewSkill::tagBlockCondition);
			ptgBlockCondition->strType = GetStringAttribute(pChildElem,"type");
			ptgBlockCondition->strDir = GetStringAttribute(pChildElem,"dir");
			ptgBlockCondition->dwBlockType = GetDwordAttribute(pChildElem,"blocktype");
			ptgBlockCondition->dwShapeType = GetDwordAttribute(pChildElem,"shapetype");
			ptgConBlock->ltgBlockCondition.push_back(ptgBlockCondition);
		}
	}
}
//���볡������
void CNewSkillFactory::LoadRegionCondition(TiXmlElement *pElem,CNewSkill::tagConRegion *ptgRegion,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagRegionCondition *ptgRegionCondition = OBJ_CREATE(CNewSkill::tagRegionCondition);
			ptgRegionCondition->strName = GetStringAttribute(pChildElem,"name");
			ptgRegionCondition->dwType = GetDwordAttribute(pChildElem,"value");
			ptgRegion->ltgRegionCondition.push_back(ptgRegionCondition);
		}
	}
}
//����
void CNewSkillFactory::LoadFaceCondition(TiXmlElement *pElem,CNewSkill::tagConFace *ptgFace, CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagFaceCondition *ptgFaceCondition = OBJ_CREATE(CNewSkill::tagFaceCondition);
			ptgFaceCondition->strFaceType = GetStringAttribute(pChildElem,"value");
			ptgFace->ltgFaceCondition.push_back(ptgFaceCondition);
		}
	}
}

//�����������
void CNewSkillFactory::LoadDistanceCondition(TiXmlElement *pElem,CNewSkill::tagConDistance *ptgConDistance,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagDistanceCondition *ptgDistanceCondition = OBJ_CREATE(CNewSkill::tagDistanceCondition);
			ptgDistanceCondition->strCmpFun = GetStringAttribute(pChildElem,"cmpfun");
			ptgDistanceCondition->dwValue = GetDwordAttribute(pChildElem,"value");
			ptgDistanceCondition->strRefObj = GetStringAttribute(pChildElem,"refobj"); 


			string strValueName = GetStringAttribute(pChildElem,"valuename");
			if (strValueName.size()>0)
			{
				ptgDistanceCondition->dwValue = atol(ptgSkill->mapVarible[strValueName].c_str());
			}

			ptgConDistance->ltgDistanceCondition.push_back(ptgDistanceCondition);
		}
	}	
}



//��������
void CNewSkillFactory::LoadSkillCondition(TiXmlElement *pElem,CNewSkill::tagConSkill *ptgConSkill,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagSkillCondition *ptgSkillCondition = OBJ_CREATE(CNewSkill::tagSkillCondition);
			ptgSkillCondition->dwID = GetDwordAttribute(pChildElem,"id");
			ptgSkillCondition->dwLevel = GetDwordAttribute(pChildElem,"level");
			ptgSkillCondition->strObject = GetStringAttribute(pChildElem,"object");
			ptgConSkill->ltgSkillCondition.push_back(ptgSkillCondition);
		}
	}	
}

//״̬����
void  CNewSkillFactory::LoadBuffCondition(TiXmlElement *pElem,CNewSkill::tagConBuff *ptgConBuff,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagBuffCondition *ptgBuffCondition = OBJ_CREATE(CNewSkill::tagBuffCondition);
			ptgBuffCondition->dwID = GetDwordAttribute(pChildElem,"id");
			ptgBuffCondition->dwLevel = GetDwordAttribute(pChildElem,"level");
			ptgBuffCondition->dwNum	  = GetDwordAttribute(pChildElem,"num");
			ptgBuffCondition->strObject = GetStringAttribute(pChildElem,"object");

		
			string strNumName = GetStringAttribute(pChildElem,"numname");
			if (strNumName.size()>0)
			{
				ptgBuffCondition->dwNum = atol(ptgSkill->mapVarible[strNumName].c_str());
			}

			string strLvName = GetStringAttribute(pChildElem,"levelname");
			if (strLvName.size()>0)
			{
				ptgBuffCondition->dwLevel = atol(ptgSkill->mapVarible[strLvName].c_str());
			}
			ptgConBuff->ltgBuffCondition.push_back(ptgBuffCondition);
		}
	}	
}

//��Ʒ����
void CNewSkillFactory::LoadGoodCondition(TiXmlElement *pElem,CNewSkill::tagConGood *ptgConGood,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagGoodCondition *ptgGoodCondition = OBJ_CREATE(CNewSkill::tagGoodCondition);
			ptgGoodCondition->strObject = GetStringAttribute(pChildElem,"object");
			ptgGoodCondition->dwPlace = GetDwordAttribute(pChildElem,"place");
			ptgGoodCondition->dwPos = GetDwordAttribute(pChildElem,"pos");
			ptgGoodCondition->strName = GetStringAttribute(pChildElem, "goodname");
			ptgGoodCondition->dwType = GetDwordAttribute(pChildElem,"goodtype");
			ptgGoodCondition->strCmpFun = GetStringAttribute(pChildElem,"cmpfun");
			ptgGoodCondition->dwValue = GetDwordAttribute(pChildElem,"value");

			string strValueName = GetStringAttribute(pChildElem,"valuename");
			if (strValueName.size()>0)
			{
				ptgGoodCondition->dwValue = atol(ptgSkill->mapVarible[strValueName].c_str());
			}

			for (TiXmlElement *pSubElem=pChildElem->FirstChildElement(); pSubElem!=NULL; 
				pSubElem=pSubElem->NextSiblingElement())
			{
				if (!stricmp(pSubElem->Value(),"CheckValue"))
				{
					CNewSkill::tagGoodAddProperty *ptgGoodAddProperty = OBJ_CREATE(CNewSkill::tagGoodAddProperty);
					ptgGoodAddProperty->dwAddProType = GetDwordAttribute(pSubElem,"type");
					ptgGoodAddProperty->dwValue1 = GetDwordAttribute(pSubElem,"value1");
					ptgGoodAddProperty->dwValue2 = GetDwordAttribute(pSubElem,"value2");
					ptgGoodCondition->ltgGoodAddProperty.push_back(ptgGoodAddProperty);
				}

			}

			ptgConGood->ltgGoodCondition.push_back(ptgGoodCondition);
		}
	}

}

//������������
void CNewSkillFactory::LoadPropertyCondition(TiXmlElement *pElem,CNewSkill::tagConProperty *ptgConProperty,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Check"))
		{
			CNewSkill::tagPropertyCondition *ptgPropertyCondition =  OBJ_CREATE(CNewSkill::tagPropertyCondition);

			ptgPropertyCondition->cmpareOp = GetStringAttribute(pChildElem,"cmpfun");
			ptgPropertyCondition->dwValue = GetDwordAttribute(pChildElem,"value");
			ptgPropertyCondition->strName = GetStringAttribute(pChildElem,"name");
			ptgPropertyCondition->strObject = GetStringAttribute(pChildElem,"object");

			string strValueName =  GetStringAttribute(pChildElem,"valuename");
			if (strValueName.size()>0)
			{
				ptgPropertyCondition->dwValue = atol(ptgSkill->mapVarible[strValueName].c_str());
			}
			ptgConProperty->ltgProtertyConditon.push_back(ptgPropertyCondition);
		}
	}																			   	
}


//��ʽ
void CNewSkillFactory::LoadExpresstionConfigration(TiXmlElement *pElem, CNewSkill *pNewSkill)
{
	DWORD dwExpressionID;
	string strFormat;

	dwExpressionID = GetDwordAttribute(pElem,"id");
	string strFileName = GetStringAttribute(pElem,"filename");
	string strContend = GetTextAttribute(pElem);
	pNewSkill->SetFormat(dwExpressionID,strFileName,strContend);	
}

//��������ȼ��ľ�̬����
void CNewSkillFactory::LoadLevelStatiParam( TiXmlElement *pElem, CNewSkill::tagSkill *pSkill,map<string,string>	mpVarablist )
{
	CNewSkill::stStaticParam *pParam = OBJ_CREATE(CNewSkill::stStaticParam);
	pParam->lCoolDownTime = GetDwordAttribute(pElem,"cooldown");
	pParam->lConsMp=GetDwordAttribute(pElem,"consmp");
	pParam->lMinAtkDistance=GetDwordAttribute(pElem,"minatkdis");
	pParam->lMaxAtkDistance=GetDwordAttribute(pElem,"maxatkdis");

	pParam->strBuffID		  = GetStringAttribute(pElem,"buffid");
	pParam->strNBuffID		  = GetStringAttribute(pElem,"nbuffid");
	pParam->strScriptParam	  = GetStringAttribute(pElem,"scriptparam");
	pParam->lActTime		  = GetDwordAttribute(pElem,"acttime");
	if (pParam->lActTime==0)
	{
		pParam->lActTime = 1;
	}


	string strCoolDownName =  GetStringAttribute(pElem,"cooldownname");
	if (strCoolDownName.size()>0)
	{
		pParam->lCoolDownTime = atol(mpVarablist[strCoolDownName].c_str());
	}
	string strCostMp = GetStringAttribute(pElem,"consmpname");
	if (strCostMp.size()>0)
	{
		pParam->lConsMp = atol(mpVarablist[strCostMp].c_str());
	}

	string strMaxDis = GetStringAttribute(pElem,"maxatkdisname");
	if (strMaxDis.size()>0)
	{
		pParam->lMaxAtkDistance = atol(mpVarablist[strMaxDis].c_str());
	}

	string strMinDis = GetStringAttribute(pElem,"minatkdisname");
	if (strMinDis.size()>0)
	{
		pParam->lMinAtkDistance = atol(mpVarablist[strMinDis].c_str());
	}

	string strBuff = GetStringAttribute(pElem,"buffname");
	if (strBuff.size()>0)
	{
		pParam->strBuffID = mpVarablist[strBuff];
	}
	string strNBuff = GetStringAttribute(pElem,"nbuffname");
	if (strNBuff.size()>0)
	{
		pParam->strNBuffID = mpVarablist[strNBuff];
	}
	string strFirstParam = GetStringAttribute(pElem,"scriptparamname");
	if (strFirstParam.size()>0)
	{
		pParam->strScriptParam = mpVarablist[strFirstParam];
	}
	string strActTime = GetStringAttribute(pElem,"acttimename");
	if (strActTime.size()>0)
	{
		pParam->lActTime = atol(mpVarablist[strActTime].c_str());
	}

	pSkill->pStaticParam = pParam;
}

/*
NAME:ClearNewSkillCache()
DESC:������ܻ���
PARA:��
*/


//���̴���׶�
void CNewSkillFactory::LoadProcesseExecute(TiXmlElement *pElem,CNewSkill::tagSkill *ptgSkill,CNewSkill *pNewSkill)
{
	CNewSkill::tagProcess *ptgProcess = OBJ_CREATE(CNewSkill::tagProcess);
	if (ptgProcess)
	{
		ptgProcess->strName = GetStringAttribute(pElem,"name");
		for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
			pChildElem=pChildElem->NextSiblingElement())
		{	
			if (!stricmp(pChildElem->Value(),"Step"))
			{
				CNewSkill::tagStep *ptgStep = OBJ_CREATE(CNewSkill::tagStep);
				ptgStep->strName = GetStringAttribute(pChildElem,"name");
				LoadStep(pChildElem,ptgStep,pNewSkill,ptgSkill);
				ptgProcess->ltgStep.push_back(ptgStep);
			}
		}
		ptgSkill->ltgExecuteStep.push_back(ptgProcess);

	}
}
void CNewSkillFactory::LoadComTemplate(TiXmlElement *pElem)
{
	DWORD dwID = GetDwordAttribute(pElem,"id");
	g_mapComTemplateElem[dwID] = pElem;
}
void CNewSkillFactory::LoadTemplateConfig(TiXmlElement *pElem)
{  	
	DWORD dwID = GetDwordAttribute(pElem,"id");  				
	g_mapTemplateElem[dwID] = pElem;	
}

//����ִ�в���
void CNewSkillFactory::LoadStep(TiXmlElement *pElem,CNewSkill::tagStep *ptgStep,CNewSkill *pNewSkill,CNewSkill::tagSkill *ptgSkill)
{


	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"Do"))
		{
			CNewSkill::tagDo *ptgDo = OBJ_CREATE(CNewSkill::tagDo);						
			ptgDo->dwOperobjid = GetDwordAttribute(pChildElem,"operobjid");		
			ptgDo->strRet = GetStringAttribute(pChildElem,"ret");
			LoadCDoProcess(pChildElem,ptgDo,ptgSkill);	
			ptgStep->ltgDo.push_back(ptgDo);			
		}
		else if (!stricmp(pChildElem->Value(),"Template"))
		{
			CNewSkill::tagDo *ptgDo = OBJ_CREATE(CNewSkill::tagDo);									
			DWORD dwTemplateID = GetDwordAttribute(pChildElem,"id");			
		}

	}

}

//
void CNewSkillFactory::LoadCDoProcess(TiXmlElement *pElem,CNewSkill::tagDo *ptgDo,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"CondiGroup"))
		{
			CNewSkill::tagCDo *ptgCDo = OBJ_CREATE(CNewSkill::tagCDo);
			ptgCDo->tgCondition.dwConditionGroup = GetDwordAttribute(pChildElem,"id");
			ptgCDo->tgCondition.strTrueRet = GetStringAttribute(pChildElem,"trueret");
			ptgCDo->tgCondition.strFalseRet = GetStringAttribute(pChildElem,"falseret");
			LoadStepProcess(pChildElem,ptgCDo,ptgSkill);
			ptgDo->ltgCDo.push_back(ptgCDo);				
		}
	}
}


//������������޸�
void CNewSkillFactory::LoadPropertyChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagChangeProperty *ptgChangeProperty = OBJ_CREATE(CNewSkill::tagChangeProperty);
	if (ptgChangeProperty)
	{
		ptgChangeProperty->strObj = GetStringAttribute(pElem,"object");
		ptgChangeProperty->strName = GetStringAttribute(pElem,"name");
		ptgChangeProperty->dwValue = GetDwordAttribute(pElem,"value");
		ptgChangeProperty->strOperator = GetStringAttribute(pElem,"operator");
		ptgChangeProperty->strScriptVarName = GetStringAttribute(pElem,"valuesname");
		ptgChangeProperty->strChangeType = pElem->Value();

		string strValueName = GetStringAttribute(pElem,"valuename");
		if (strValueName.size()>0)
		{
			ptgChangeProperty->dwValue = atol(ptgSkill->mapVarible[strValueName].c_str());
		}
		ptgDoProcess->ltgBaseChange.push_back(ptgChangeProperty);

	}
}
//״̬
void CNewSkillFactory::LoadBuffChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagChangeBuff *ptgChangeBuff = OBJ_CREATE(CNewSkill::tagChangeBuff);
	if (ptgChangeBuff)
	{
		ptgChangeBuff->strObj = GetStringAttribute(pElem,"object");
		ptgChangeBuff->dwID = GetDwordAttribute(pElem,"bufid");
		ptgChangeBuff->dwLevel = GetDwordAttribute(pElem,"buflvl");
		ptgChangeBuff->dwCount = GetDwordAttribute(pElem,"bufcount");

		ptgChangeBuff->strSIDName = GetStringAttribute(pElem,"bufidsname");
		ptgChangeBuff->strSLvName = GetStringAttribute(pElem,"buflvlsname");
		ptgChangeBuff->strSCountName = GetStringAttribute(pElem,"bufcountsname");

		string strbufid = GetStringAttribute(pElem,"bufidname");
		if (strbufid.size()>0)
		{
			ptgChangeBuff->dwID = atol(ptgSkill->mapVarible[strbufid].c_str());
		}		
		string strbuflv = GetStringAttribute(pElem,"buflvlname");
		if (strbuflv.size()>0)
		{
			ptgChangeBuff->dwLevel = atol(ptgSkill->mapVarible[strbuflv].c_str());
		}
		string strbufcount = GetStringAttribute(pElem,"bufcountname");
		if (strbufcount.size()>0)
		{
			ptgChangeBuff->dwCount = atol(ptgSkill->mapVarible[strbufcount].c_str());
		}

		ptgChangeBuff->strOperator = GetStringAttribute(pElem,"operator");
		ptgChangeBuff->strChangeType = pElem->Value();		
		ptgDoProcess->ltgBaseChange.push_back(ptgChangeBuff);		
	}
}

//λ��
void CNewSkillFactory::LoadPosChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	CNewSkill::tagChangePos *ptgChangePos = OBJ_CREATE(CNewSkill::tagChangePos);
	if (ptgChangePos)
	{
		ptgChangePos->strObj = GetStringAttribute(pElem,"object");
		ptgChangePos->strRefObject = GetStringAttribute(pElem,"referobj");
		ptgChangePos->strDir = GetStringAttribute(pElem,"dir");
		ptgChangePos->strStyle = GetStringAttribute(pElem,"style");
		ptgChangePos->strSpeed = GetStringAttribute(pElem,"speed");
		ptgChangePos->strChangeType = pElem->Value();
		ptgDoProcess->ltgBaseChange.push_back(ptgChangePos);
	}
}
//��ȴʱ��
void CNewSkillFactory::LoadCooldownChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagCoolDownChange *ptgCoolDownChange = OBJ_CREATE(CNewSkill::tagCoolDownChange);
	if (ptgCoolDownChange)
	{
		ptgCoolDownChange->strObj = GetStringAttribute(pElem,"object");
		ptgCoolDownChange->strType = GetStringAttribute(pElem,"type");
		ptgCoolDownChange->dwValue = GetDwordAttribute(pElem,"value");
		ptgCoolDownChange->strSVarianame = GetStringAttribute(pElem,"valuesname");

		string strValueName = GetStringAttribute(pElem,"valuename");
		if (strValueName.size()>0)
		{
			ptgCoolDownChange->dwValue = atol(ptgSkill->mapVarible[strValueName].c_str());
		}

		ptgCoolDownChange->strChangeType = pElem->Value();
		ptgDoProcess->ltgBaseChange.push_back(ptgCoolDownChange);
	}
}
//���ܲ���
void	CNewSkillFactory::LoadSkillChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagSkillChange *pTemp = OBJ_CREATE(CNewSkill::tagSkillChange);
	pTemp->strObj   = GetStringAttribute(pElem,"object"); 
	pTemp->strType  = GetStringAttribute(pElem,"type");
	pTemp->strOpera = GetStringAttribute(pElem,"operator");
	pTemp->lID      = GetDwordAttribute(pElem,"id");
	pTemp->lLv		= GetDwordAttribute(pElem,"lv");

	pTemp->strSIDName = GetStringAttribute(pElem,"idsname");
	pTemp->strSLvName = GetStringAttribute(pElem,"lvsname");

	string strid = GetStringAttribute(pElem,"idname");
	if (strid.size()>0)
	{
		pTemp->lID  = atol(ptgSkill->mapVarible[strid].c_str());
	}		
	string strlv = GetStringAttribute(pElem,"lvname");
	if (strlv.size()>0)
	{
		pTemp->lLv  = atol(ptgSkill->mapVarible[strlv].c_str());
	}
	pTemp->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(pTemp);
}

//�����б�
void CNewSkillFactory::LoadDestObjChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	CNewSkill::tagDestObjChange *pTemp = OBJ_CREATE(CNewSkill::tagDestObjChange);
	pTemp->strDestObjChange = GetStringAttribute(pElem,"type");
	pTemp->strObj = GetStringAttribute(pElem,"object");
	pTemp->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(pTemp);
}

//ע��ʱ��
void CNewSkillFactory::LoadUnRegisterTimer(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	CNewSkill::tagUnRegisterTimer *pTemp = OBJ_CREATE(CNewSkill::tagUnRegisterTimer);
	pTemp->dwUnRegisterTimer = GetDwordAttribute(pElem,"flag");
	pTemp->strObj = GetStringAttribute(pElem,"object");
	pTemp->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(pTemp);

}

//ע���¼�
void CNewSkillFactory::LoadUnRegisterEvent(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	CNewSkill::tagUnRegisterEvent *pTemp = OBJ_CREATE(CNewSkill::tagUnRegisterEvent);
	pTemp->strObj = GetStringAttribute(pElem,"object");
	pTemp->dwUnRegisterEvent = GetDwordAttribute(pElem,"flag");
	pTemp->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(pTemp);
}
//����
void CNewSkillFactory::LoadDirection(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	CNewSkill::tagDirection *pTemp = OBJ_CREATE(CNewSkill::tagDirection);
	pTemp->strObj = GetStringAttribute(pElem,"object");
	pTemp->strDir = GetStringAttribute(pElem,"direction");
	pTemp->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(pTemp);
}

//ע��ʱ��
void CNewSkillFactory::LoadRegisterTimer(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagRegisterTimer *ptgRegisterTime = OBJ_CREATE(CNewSkill::tagRegisterTimer);
	ptgRegisterTime->strObj = GetStringAttribute(pElem,"object");
	ptgRegisterTime->dwFlag = GetDwordAttribute(pElem,"flag");
	ptgRegisterTime->dwCount = GetDwordAttribute(pElem,"count");
	ptgRegisterTime->strSCountName = GetStringAttribute(pElem,"countvarsname");
	ptgRegisterTime->dwIntelTime = GetDwordAttribute(pElem,"intel");
	ptgRegisterTime->strSIntelName = GetStringAttribute(pElem,"intervarsname");
	ptgRegisterTime->dwFuture = GetDwordAttribute(pElem,"future");
	ptgRegisterTime->strSFutvarname = GetStringAttribute(pElem,"futvarsname");

	string strFu = GetStringAttribute(pElem,"futvarname");
	if (strFu.size()>0)
	{
		ptgRegisterTime->dwFuture = atol(ptgSkill->mapVarible[strFu].c_str());
	}
	string strIntel = GetStringAttribute(pElem,"intervarname");
	if (strIntel.size()>0)
	{
		ptgRegisterTime->dwIntelTime = atol(ptgSkill->mapVarible[strIntel].c_str());
	}					
	string strCount = GetStringAttribute(pElem,"countvarname");
	if (strCount.size()>0)
	{
		ptgRegisterTime->dwCount = atol(ptgSkill->mapVarible[strCount].c_str());
	}

	ptgRegisterTime->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(ptgRegisterTime);
}

//ע���¼�
void CNewSkillFactory::LoadRegisterEvent(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	CNewSkill::tagRegisterEvent *ptgRegisterEvent = OBJ_CREATE(CNewSkill::tagRegisterEvent);
	ptgRegisterEvent->strObj = GetStringAttribute(pElem,"object"); 
	ptgRegisterEvent->dwFlag = GetDwordAttribute(pElem,"flag");
	ptgRegisterEvent->strName = GetStringAttribute(pElem,"name");
	ptgRegisterEvent->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(ptgRegisterEvent);	
}
//������Ϣ
void CNewSkillFactory::LoadSendMsg(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagSendMsg *ptgSendMsg = OBJ_CREATE(CNewSkill::tagSendMsg);
	ptgSendMsg->strObj = GetStringAttribute(pElem,"object"); 
	ptgSendMsg->strType = GetStringAttribute(pElem,"type");
	ptgSendMsg->strRange = GetStringAttribute(pElem,"range");
	ptgSendMsg->dwValue = GetDwordAttribute(pElem,"value");
	ptgSendMsg->strSVName = GetStringAttribute(pElem,"valuesname");
	ptgSendMsg->nAdd	= GetIntAttribute(pElem,"update");

	string strValue = GetStringAttribute(pElem,"valuename");
	if (strValue.size()>0)
	{
		ptgSendMsg->dwValue = atol(ptgSkill->mapVarible[strValue].c_str()); 
	}
	ptgSendMsg->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(ptgSendMsg);	
}
//�˺�����
void CNewSkillFactory::LoadDamageChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagDamageChange *ptDamageChange = OBJ_CREATE(CNewSkill::tagDamageChange);
	ptDamageChange->strOperator = GetStringAttribute(pElem,"operator");
	ptDamageChange->strType = GetStringAttribute(pElem,"type");
	ptDamageChange->strSVarianame = GetStringAttribute(pElem,"valuesname");
	ptDamageChange->dwValue = GetDwordAttribute(pElem,"value");
	ptDamageChange->strObj = GetStringAttribute(pElem,"object");
	ptDamageChange->strChangeType = pElem->Value();

	string strValue = GetStringAttribute(pElem,"valuename");
	if (strValue.size()>0)
	{
		ptDamageChange->dwValue = atol(ptgSkill->mapVarible[strValue].c_str());
	}

	ptgDoProcess->ltgBaseChange.push_back(ptDamageChange);
}
//DO�˺�
void CNewSkillFactory::LoadDoDamage(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagDoDamage *ptgDoDamage = OBJ_CREATE(CNewSkill::tagDoDamage);
	ptgDoDamage->strObj = GetStringAttribute(pElem,"object");
	ptgDoDamage->dwBlastAttack = GetDwordAttribute(pElem,"blastvalue");
	ptgDoDamage->dwResult = GetDwordAttribute(pElem,"damageresvalue");	
	ptgDoDamage->strSResVariName = GetStringAttribute(pElem,"damageresvaluesname");
	ptgDoDamage->strSBlastVariName = GetStringAttribute(pElem,"blastvaluesname");

	string strRes = GetStringAttribute(pElem,"damageresvaluename");
	if (strRes.size()>0)
	{
		ptgDoDamage->dwResult = atol(ptgSkill->mapVarible[strRes].c_str());
	}

	string strBlast = GetStringAttribute(pElem,"blastvaluename");
	if (strBlast.size()>0)
	{
		ptgDoDamage->dwBlastAttack = atol(ptgSkill->mapVarible[strBlast].c_str());
	}	

	ptgDoDamage->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(ptgDoDamage);	
}
//��Ʒ
void CNewSkillFactory::LoadGoodChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	CNewSkill::tagGoodChange *ptgGoodChange = OBJ_CREATE(CNewSkill::tagGoodChange);
	ptgGoodChange->strObj = GetStringAttribute(pElem,"object");
	ptgGoodChange->strOperator = GetStringAttribute(pElem,"operator");
	ptgGoodChange->strName = GetStringAttribute(pElem,"name");
	ptgGoodChange->dwPlace = GetDwordAttribute(pElem,"place");
	ptgGoodChange->dwPos  = GetDwordAttribute(pElem,"pos");
	ptgGoodChange->dwValue = GetDwordAttribute(pElem,"value");
	ptgGoodChange->dwGoodsType = GetDwordAttribute(pElem,"goodtype");
	ptgGoodChange->dwWeaponType = GetDwordAttribute(pElem,"weapontype");
	ptgDoProcess->ltgBaseChange.push_back(ptgGoodChange);
	ptgGoodChange->strChangeType = pElem->Value();
}




//��������
void CNewSkillFactory::LodAttackObject(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	CNewSkill::tagAttackChangeObject *ptgTemp = OBJ_CREATE(CNewSkill::tagAttackChangeObject);
	ptgTemp->strObj = GetStringAttribute(pElem,"object");
	ptgTemp->strAttackChangeObject = GetStringAttribute(pElem,"selobj");	
	ptgTemp->strChangeType = pElem->Value();
	ptgDoProcess->ltgBaseChange.push_back(ptgTemp);

}

//ִ�в���
void CNewSkillFactory::LoadDoProcess(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"ObjectPropertyChange"))
		{
			//�����������
			LoadPropertyChange(pChildElem,ptgDoProcess,ptgSkill);

		}
		else if (!stricmp(pChildElem->Value(),"BuffChange"))
		{
			//״̬
			LoadBuffChange(pChildElem,ptgDoProcess,ptgSkill);
		}
		else if (!stricmp(pChildElem->Value(),"PosChange"))
		{
			//λ��
			LoadPosChange(pChildElem,ptgDoProcess);
		}
		else if (!stricmp(pChildElem->Value(),"AttakObjectChange"))
		{
			//�ı乥������
			LodAttackObject(pChildElem,ptgDoProcess);
		}
		else if (!stricmp(pChildElem->Value(),"Script"))
		{			
			//�ű�
			LoadScriptChange(pChildElem,ptgDoProcess);
		}
		else if (!stricmp(pChildElem->Value(),"CooldownChange"))
		{
			//��ȴʱ��
			LoadCooldownChange(pChildElem,ptgDoProcess,ptgSkill);
		}
		else if (!stricmp(pChildElem->Value(),"DestObjsChange"))
		{
			//�����б�
			LoadDestObjChange(pChildElem,ptgDoProcess);
		}
		else if (!stricmp(pChildElem->Value(),"RegisterTimer"))
		{
			//ע��ʱ��
			LoadRegisterTimer(pChildElem,ptgDoProcess,ptgSkill);
		}
		else if (!stricmp(pChildElem->Value(),"RegisterEvent"))
		{
			//ע���¼�
			LoadRegisterEvent(pChildElem,ptgDoProcess);
		}
		else if (!stricmp(pChildElem->Value(),"UnRegisterTimer"))
		{
			//ע��ʱ��
			LoadUnRegisterTimer(pChildElem,ptgDoProcess);
		}
		else if (!stricmp(pChildElem->Value(),"UnRegisterEvent"))
		{
			//ע���¼�
			LoadUnRegisterEvent(pChildElem,ptgDoProcess);
		}

		else if (!stricmp(pChildElem->Value(),"SendMsg"))
		{
			//������Ϣ
			LoadSendMsg(pChildElem,ptgDoProcess,ptgSkill);
		}
		else if (!stricmp(pChildElem->Value(),"DamageChange"))
		{
			//�˺�����
			LoadDamageChange(pChildElem,ptgDoProcess,ptgSkill);
		}
		else if (!stricmp(pChildElem->Value(),"DoDamage"))
		{
			LoadDoDamage(pChildElem,ptgDoProcess,ptgSkill);
		}
		else if (!stricmp(pChildElem->Value(),"GoodChange"))
		{
			LoadGoodChange(pChildElem,ptgDoProcess,ptgSkill);
		}
		else if (!stricmp(pChildElem->Value(),"DirecChange"))
		{
			LoadDirection(pChildElem,ptgDoProcess);
		}
		else if (!stricmp(pChildElem->Value(),"SkillChange"))
		{
			LoadSkillChange(pChildElem,ptgDoProcess,ptgSkill);
		}	
		else if (!stricmp(pChildElem->Value(),"CondiGroup"))
		{
			CNewSkill::tagCDo *ptgCDo = OBJ_CREATE(CNewSkill::tagCDo);
			ptgCDo->tgCondition.dwConditionGroup = GetDwordAttribute(pChildElem,"id");
			ptgCDo->tgCondition.strTrueRet = GetStringAttribute(pChildElem,"trueret");
			ptgCDo->tgCondition.strFalseRet = GetStringAttribute(pChildElem,"falseret");
			LoadStepProcess(pChildElem,ptgCDo,ptgSkill);
			ptgDoProcess->ltgCDo.push_back(ptgCDo);			
		}
	}

}



//�ű�
void CNewSkillFactory::LoadScriptChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess)
{
	string strType = pElem->Value();
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"ExpressionID"))
		{

			CNewSkill::tagScriptID *ptgScripID = OBJ_CREATE(CNewSkill::tagScriptID);
			ptgScripID->dwScriptID = GetDwordAttribute(pChildElem, "value");
			ptgScripID->strObj = GetStringAttribute(pChildElem,"object");
			ptgScripID->strChangeType = strType;
			ptgDoProcess->ltgBaseChange.push_back(ptgScripID);
		}
	}

}


//���봦����
void CNewSkillFactory::LoadStepProcess(TiXmlElement *pElem,CNewSkill::tagCDo *ptgCDo,CNewSkill::tagSkill *ptgSkill)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"True"))
		{
			CNewSkill::tagDoProcess *ptgDoProcess = OBJ_CREATE(CNewSkill::tagDoProcess);
			LoadDoProcess(pChildElem,ptgDoProcess,ptgSkill);			
			ptgCDo->ltgTrueDoProcess.push_back(ptgDoProcess);
		} 
		else if (!stricmp(pChildElem->Value(),"False")) 
		{
			CNewSkill::tagDoProcess *ptgDoProcess = OBJ_CREATE(CNewSkill::tagDoProcess);
			LoadDoProcess(pChildElem,ptgDoProcess,ptgSkill);
			ptgCDo->ltgFalseDoProcess.push_back(ptgDoProcess);
		}


	}
}


//��������
void CNewSkillFactory::LoadOperatObjects(TiXmlElement *pElem,CNewSkill *pNewSkill)
{
	CNewSkill::tagOperateObject *ptgOperateObject = OBJ_CREATE(CNewSkill::tagOperateObject);
	if (ptgOperateObject)
	{
		ptgOperateObject->dwID = GetDwordAttribute(pElem,"id");
		ptgOperateObject->strType = GetStringAttribute(pElem,"type");
		ptgOperateObject->strPosition = GetStringAttribute(pElem,"position");
		ptgOperateObject->dwDistance = GetDwordAttribute(pElem,"distance");
		ptgOperateObject->dwRadius = GetDwordAttribute(pElem,"radius");
		ptgOperateObject->dwSkillID = GetDwordAttribute(pElem,"skillid");
		ptgOperateObject->dwSkillLevel =  GetDwordAttribute(pElem,"skilllvl");
		ptgOperateObject->strCenterObject = GetStringAttribute(pElem,"center");
		ptgOperateObject->strObject = GetStringAttribute(pElem,"object");
		ptgOperateObject->dwConGroupID = GetDwordAttribute(pElem,"condigroup");
		ptgOperateObject->dwNumObject = GetDwordAttribute(pElem,"numobject"); 
		pNewSkill->SetOperateObject(ptgOperateObject->dwID,ptgOperateObject);				
	}		
}

/*
NAME:ClearNewSkillCache()
DESC:������ܻ���
PARA:��
*/
void CNewSkillFactory::ClearNewSkillCache()
{	
	for( map<DWORD, CNewSkill*>::iterator it = g_mapNewSkill.begin(); 
		it != g_mapNewSkill.end(); it ++ )
	{
		MP_DELETE( it -> second );
	}
	g_mapNewSkill.clear();
}


/*
NAME:CodeNewSkill
DESC:״̬����
PARA:vRet����
*/
bool CNewSkillFactory::CodeNewSkill(vector<BYTE>& vRet )
{
	_AddToByteArray( &vRet, static_cast<DWORD>( g_mapNewSkill.size() ) );
	//�汾��
	_AddToByteArray(&vRet,++g_dwVersion);
	//ְҵ��ʼ�������б�
	_AddToByteArray(&vRet,(long)g_mapOccuInitSkill.size());
	for (map<DWORD,list<long>>::iterator it = g_mapOccuInitSkill.begin(); 
		 it != g_mapOccuInitSkill.end(); ++it)
	{
		_AddToByteArray(&vRet,(DWORD)it->first);
		_AddToByteArray(&vRet,(long)(it->second.size()));
		for (list<long>::iterator sit = it->second.begin(); sit!=it->second.end(); ++sit)
		{
			_AddToByteArray(&vRet,(long)(*sit));
		}
	}


	size_t	lLength	= 0;
	BYTE*	pBuffer	= NULL;
	for( map<DWORD, CNewSkill*>::iterator it = g_mapNewSkill.begin(); 
		it != g_mapNewSkill.end(); it ++ )
	{
		if( it -> second )
		{			
			it ->second ->AddNewSkillToByteArray(vRet);		
		}
		else 
		{
			_AddToByteArray( &vRet, static_cast<DWORD>( 0 ) );
		}
	}
	return true;
}

