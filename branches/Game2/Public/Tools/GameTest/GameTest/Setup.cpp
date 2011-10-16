//=============================================================================
/**
 *  file: CSetup.h
 *
 *  Brief:������
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-7-30
 */
//=============================================================================
#include "StdAfx.h"
#include ".\setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSetup* CSetup::instance=NULL;

CSetup::CSetup(void)
{
}

CSetup::~CSetup(void)
{
}

CSetup* CSetup::getInstance()
{
	if(NULL == instance)
		instance = new CSetup;
	return instance;
}

void CSetup::Release()
{
	if(instance)
		SAFE_DELETE(instance);
}

void CSetup::Load(const char* file)
{
 	char strfile[512] = "setup\\Initial.xml";
 	if(file)
 		strncpy(strfile,file,510);

	TiXmlNode *pNode = NULL;
	TiXmlDocument doc(strfile);
	///���������ļ�
	if(!doc.LoadFile())
	{
		PutDebugString("װ�������ļ�<<Initial.xml>>����");
		return;
	}

	pNode = doc.FirstChild("Setup");
	if(pNode)
	{
		TiXmlElement *pChildElement = pNode->FirstChildElement();
		for(;pChildElement!=NULL;pChildElement=pChildElement->NextSiblingElement())
		{
			if(!strcmp(pChildElement->Value(),"Server"))
			{
				m_strServerIP = string(pChildElement->Attribute("ip"));
				pChildElement->Attribute("port",&m_nPort);
				//����IP
			}
			else if(!strcmp(pChildElement->Value(),"ScriptFile"))
			{
				//���ó�ʼ����ʱ��Ҫ���Ľű��ļ�
				m_strIniScript = string(pChildElement->Attribute("name"));
			}
		}
	}
}
