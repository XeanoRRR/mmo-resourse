

#include "stdafx.h"
#include "PurviewSetup.h"




PurviewSetup PurviewSetup::c_PurviewSetup;

PurviewSetup::PurviewSetup(VOID)
:m_bIsReady(FALSE)
{

}

PurviewSetup::~PurviewSetup(VOID)
{

}

PurviewSetup& PurviewSetup::GetInstance(VOID)
{
	return c_PurviewSetup;
}


//! ����PurviewSetup::LoadSetup����ʱ�ṹ
struct tagStrAndEnum
{
	char	szStr[256];
	DWORD	dwEnum;
};

BOOL PurviewSetup::LoadSetup(const char *pXmlFilename)
{
	////////////////////////////////////////////
	//! Ȩ�޵������ַ�����ö��ֵ�Ķ�Ӧ��
	static tagStrAndEnum arrStrAndEnumTable[] = 
	{
		"GmNormal",			ePT_GmNormal		,
		"GmAffiche",		ePT_GmAffiche		,
		"ViewGmOpt",		ePT_ViewGmOpt		,

		"Scout1",			ePT_Scout1			,
		"Scout2",			ePT_Scout2			,

		"ServerControl1",	ePT_ServerControl1	,
		"ServerControl2",	ePT_ServerControl2	,

		"Update",			ePT_Update			,
		"UserManage",		ePT_UserManage		,

		"AllPurview",		ePT_AllPurview		,
	};
	////////////////////////////////////////////


	//! ������ʼ
	if(NULL == pXmlFilename)
		return FALSE;

	TiXmlNode *pNode = NULL; 		

	TiXmlDocument* m_Tdoc = new TiXmlDocument(pXmlFilename);
	if(!m_Tdoc->LoadFile())
		return FALSE;

	pNode = m_Tdoc->FirstChild("PurviewSetup");

	if (pNode == NULL)
	{
		SAFE_DELETE(m_Tdoc);
		return FALSE;
	}

	TiXmlElement* pPurviewSetup = pNode->ToElement();//��ȡnode ��ָ��

	if (pPurviewSetup == NULL)
	{
		SAFE_DELETE(m_Tdoc);
		return FALSE;
	}

	////////////////////////////////////////////
	m_bIsReady = FALSE;
	m_mapGMTPurviewSetup.clear();

	TiXmlElement* pPurviewGroup = pPurviewSetup->FirstChildElement("PurviewGroup");
	for (; pPurviewGroup != NULL; pPurviewGroup = pPurviewGroup->NextSiblingElement("PurviewGroup"))
	{
		tagGMTPurviewSetup GMTPurviewSetup;

		//! ��������
		GMTPurviewSetup.dwUserType = atoi(pPurviewGroup->Attribute("UserType"));
		if(0 == GMTPurviewSetup.dwUserType)
		{
			LogTools::OutLog("ignore UserType = 0 when load [%s]!", pXmlFilename);
			continue;
		}
		const char *pName = pPurviewGroup->Attribute("Name");
		if(NULL == pName)
		{
			LogTools::OutLog("load Name lost when load [%s]!", pXmlFilename);
			continue;
		}

		LONG lCopyLen = (LONG)strlen(pName);
		lCopyLen = (lCopyLen < MAX_USER_GROUP_NAME_SZIE) ? lCopyLen : MAX_USER_GROUP_NAME_SZIE;
		memcpy(GMTPurviewSetup.szName, pName, lCopyLen);
		
		//! Ȩ��
		TiXmlElement* pAbleUse = pPurviewGroup->FirstChildElement("AbleUse");
		for (; pAbleUse != NULL; pAbleUse = pAbleUse->NextSiblingElement("AbleUse"))
		{
			const char *pPurview = pAbleUse->Attribute("Purview");
			if(NULL == pPurview)
			{
				LogTools::OutLog("load UserGroup[%s]'s Purview lost when load [%s]!", GMTPurviewSetup.szName, pXmlFilename);
				continue;
			}

			LONG lLoopCount = sizeof(arrStrAndEnumTable) / sizeof(tagStrAndEnum);
			DWORD lPurview = 0;
			for (LONG i = 0; i < lLoopCount; ++i)
			{
				if(0 == stricmp(arrStrAndEnumTable[i].szStr, pPurview))
				{
					lPurview = arrStrAndEnumTable[i].dwEnum;
					break;
				}
			}
			if(0 == lPurview)
			{
				LogTools::OutLog("ignore UserGroup[%s]'s error Purview=%s when load [%s]!", GMTPurviewSetup.szName, pPurview, pXmlFilename);
			}
			else
				GMTPurviewSetup.dwPurview |= lPurview;
		}

		if(0 == GMTPurviewSetup.dwPurview)
		{
			LogTools::OutLog("ignore UserGroup[%s] when load [%s]!, it No Purview", GMTPurviewSetup.szName, pXmlFilename);
		}
		else
			m_mapGMTPurviewSetup.insert(pair<DWORD, tagGMTPurviewSetup>(GMTPurviewSetup.dwUserType, GMTPurviewSetup));
	}

	if(0 == m_mapGMTPurviewSetup.size())
	{
		LogTools::OutLog("the %s is null, system can't login", pXmlFilename);
		SAFE_DELETE(m_Tdoc);
		return FALSE;
	}
	////////////////////////////////////////////
	m_bIsReady = TRUE;
	SAFE_DELETE(m_Tdoc);
	return TRUE;
}

//! ͨ���û����͵õ�Ȩ��
const tagGMTPurviewSetup* PurviewSetup::GetPurviewByUserType(DWORD dwUserType)
{
	if(m_bIsReady)
	{
		map<DWORD, tagGMTPurviewSetup>::iterator ite = m_mapGMTPurviewSetup.find(dwUserType);
		if(m_mapGMTPurviewSetup.end() != ite)
			return &(ite->second);
	}
	return NULL;
}