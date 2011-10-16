#include "StdAfx.h"
#include "LRMDef.h"
#include "tinyxml.h"

map<long,string> CLrmSetup::m_mpTable;
list<string> CLrmSetup::m_lRSType;
CLrmSetup::CLrmSetup()
{
	m_mpTable.clear();
}

CLrmSetup::~CLrmSetup()
{

}

string CLrmSetup::GetTableName(long lID)
{
	map<long,string>::iterator it = m_mpTable.find(lID);
	if (it!=m_mpTable.end())
	{
		return (*it).second;
	}
	//��ʾDBConfig.xml��û������lID	
	return "";
}
void CLrmSetup::GetCountyEnum(long &lVersion)
{
	switch (lVersion)
	{
		case V_SCHINA:
			//����
			lVersion = GB2312_CHARSET;
			break;
		case V_KOREA:			
			//����
			lVersion = HANGUL_CHARSET;
			break;
		case V_JAPAN:		
			//����
			lVersion =  SHIFTJIS_CHARSET;
			break;
		case V_TCHINA:		
			//���ķ���
			lVersion = CHINESEBIG5_CHARSET;
			break;
	}
}

bool CLrmSetup::LoadDBTableConfig()
{
	string strFileName = "DBConfig.xml";


	CRFile *prfile =  rfOpen(strFileName.c_str());

	if(NULL == prfile)	
		return false;

	TiXmlNode *pNode = NULL;
	TiXmlDocument doc(strFileName.c_str());


	if( !doc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		char pszInfo[512]="";
		_snprintf(pszInfo,512,"%s ���ô���!",strFileName.c_str());
		MessageBox(NULL,pszInfo,"",MB_OK);
		rfClose(prfile);
		return false;
	}

	//ȡ�õ�һ���ڵ�
	pNode = doc.FirstChild("Root");	

	if (NULL == pNode)
	{
		rfClose(prfile);
		return false;
	}
	TiXmlElement *pElem = pNode->ToElement();



	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"DBTable"))
		{
			long lID = atoi(pChildElem->Attribute("id"));
			string strName = pChildElem->Attribute("name");
			m_mpTable[lID]=strName;
		}
		else if (!stricmp(pChildElem->Value(),"RSType"))
		{
			string strType = pChildElem->Attribute("type");
			m_lRSType.push_back(strType);
		}
	}	
	rfClose(prfile);
	return true;
}