#include "stdafx.h"
#include "Interface.h"
#include "../../PUBLIC/rfile.h"
#include "../../PUBLIC/clientresource.h"
//
CInterfaceXml::CInterfaceXml(void)
{
	vecInterfacePage.clear();
}

CInterfaceXml::~CInterfaceXml(void)
{
	vecInterfacePage.clear();
}

//��ȡXML����
bool CInterfaceXml::LoadInterfaceXml()
{
   // Release();
	//////////////////////////////////////////////////////////////////////
	//��ȡ�����ļ�
	CRFile *prfile = rfOpen("launcher\\Interface\\guixml\\launcher.xml");
	if(NULL == prfile)
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","Interface.xml");
		MessageBox(NULL,strError,"",MB_OK); 
		return false;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument doc;
	//���뼼�������ļ�
	if( !doc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","Interface.xml");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	/////////////////////////////////////////////////////////////////
	//�����̵���Ч�������
	pNode = doc.FirstChild("InterfaceIndex");
	if(NULL==pNode)
	{
		return false;
	}
	//�����̵���Ч����
	TiXmlElement* pElement=pNode->ToElement();
	if(NULL==pElement)
		return false;
	TiXmlElement* pElem=pElement->FirstChildElement("Frame");
	if(pElem&&!LoadButtonNode(pElem))
	{
		////���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","Interface.xml:Frame");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	if(pElem&&!LoadLauncherBack(pElem))
	{
		////���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","Interface.xml:Frame");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	return true;
}

bool CInterfaceXml::LoadButtonNode(TiXmlElement* pElem)
{
	for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{	    
			if(strcmp(pChildElem->Value(),"Button")==0)
			{
				tagInterface Interface;
				Interface.strName = pChildElem->Attribute("Name"); //��ȡ�������������ϵ�BUFFID
				Interface.PosX = (long)atoi(pChildElem->Attribute("x"));
				Interface.PosY = (long)atoi(pChildElem->Attribute("y"));;  //��ȡ��Ӧ����ЧID
				Interface.Width = (long)atoi(pChildElem->Attribute("w"));
				Interface.Height = (long)atoi(pChildElem->Attribute("h"));
				Interface.Path1 = pChildElem->Attribute("Normal");  //��ȡ��Ӧ����ЧID
				Interface.Path2 = pChildElem->Attribute("Suspend");
				Interface.Path3 = pChildElem->Attribute("Pressed");
				vecInterfacePage.push_back(Interface);
		    }
	}
	//�ж������Ƿ��ȡ��ȷ
	if(vecInterfacePage.size()>0)
		return true;
	return false;
}

bool CInterfaceXml::GetInterfaceTag(const char* Name,tagInterface &Interface)
{
	vector<tagInterface>::iterator _itor = vecInterfacePage.begin();
	for(;_itor!=vecInterfacePage.end();_itor++)
	{
		if(strcmp(Name,(*_itor).strName.c_str())==0)
		{
			Interface = (*_itor);
			return true;
		}
	}
	return false;
}

tagInterface CInterfaceXml::GetInterfaceTag(const char* Name)
{
	tagInterface Interface;
	vector<tagInterface>::iterator _itor = vecInterfacePage.begin();
	for(;_itor!=vecInterfacePage.end();_itor++)
	{
		if(strcmp(Name,(*_itor).strName.c_str())==0)
		{
			Interface = (*_itor);
			break;
		}
	}
	return Interface;
}

bool CInterfaceXml::LoadLauncherBack(TiXmlElement* pElem)
{
	for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{	    
			if(strcmp(pChildElem->Value(),"Background")==0)
			{
				m_tagLauncherBack.strName = pChildElem->Attribute("Name");
				m_tagLauncherBack.width = (long)atoi(pChildElem->Attribute("w"));
				m_tagLauncherBack.height = (long)atoi(pChildElem->Attribute("h"));
				m_tagLauncherBack.webX = (long)atoi(pChildElem->Attribute("webX"));
				m_tagLauncherBack.webY = (long)atoi(pChildElem->Attribute("webY"));
				m_tagLauncherBack.webW = (long)atoi(pChildElem->Attribute("webW"));
				m_tagLauncherBack.webH = (long)atoi(pChildElem->Attribute("webH"));
				m_tagLauncherBack.ImagePath = pChildElem->Attribute("Image");
		    }
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
map<long,string> CStringRead::m_mapStringFile;

CStringRead::CStringRead()
{
	m_mapStringFile.clear();
}

CStringRead::~CStringRead()
{
	m_mapStringFile.clear();
}

bool CStringRead::LoadTextresource()
{
	//��ȡ�����ļ�
	CRFile *prfile = rfOpen("launcher\\textresource\\launcher.xml"); 
	if(NULL == prfile)
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","Interface.xml");
		MessageBox(NULL,strError,"",MB_OK); 
		return false;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument doc;
	//���뼼�������ļ�
	if( !doc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","Interface.xml");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	/////////////////////////////////////////////////////////////////
	//�����̵���Ч�������
	pNode = doc.FirstChild("textresource");
	if(NULL==pNode)
	{
		return false;
	}
	//�����̵���Ч����
	TiXmlElement* pElement=pNode->ToElement();
	if(NULL==pElement)
		return false;
	TiXmlElement* pElem=pElement->FirstChildElement("textType");
	if(pElem&&!LoadStringA(pElem))
	{
		////���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","Interface.xml:Frame");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	return true;
}

bool CStringRead::LoadStringA(TiXmlElement *pElem)
{
	for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{	    
			if(strcmp(pChildElem->Value(),"String")==0)
			{
				int id = (long)atoi(pChildElem->Attribute("id"));
				string strText =  pChildElem->Attribute("text");
				m_mapStringFile[id] = strText;
		    }
	}
	if(m_mapStringFile.size()>0)
		return true;
	return false;
}

string CStringRead::GetString(long id)
{
	map<long,string>::iterator it = m_mapStringFile.find(id);
		if(it!=m_mapStringFile.end())
		   return m_mapStringFile[id];
	string str = "String error";
	return str;
}