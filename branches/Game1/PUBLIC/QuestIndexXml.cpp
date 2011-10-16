#include "StdAfx.h"
//#include "../../AppClient/stdafx.h"
#include "QuestIndexXml.h"

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CQuestIndexXml::CQuestIndexXml()
{}

CQuestIndexXml::~CQuestIndexXml()
{
	Release();
}

void CQuestIndexXml::Release()
{
	m_vecQuestNode.clear();
	m_vecQuestIndex.clear();
	m_tagQuestIndexForServer.clear();
}

//��ȡXML����
bool CQuestIndexXml::LoadQuestSetupXml()
{
    Release();
	//////////////////////////////////////////////////////////////////////
	//��ȡ�����ļ�
	CRFile *prfile = rfOpen("data\\QuestIndex.xml");
	if(NULL == prfile)
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","QuestIndex.xml");
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
		wsprintf(strError,"LoadFileError:%s","PlayerSetup.xml");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	/////////////////////////////////////////////////////////////////
	//�����̵���Ч�������
	pNode = doc.FirstChild("QuestIndex");
	if(NULL==pNode)
	{
		return false;
	}
	//�����̵���Ч����
	TiXmlElement* pElement=pNode->ToElement();
	if(NULL==pElement)
		return false;
	TiXmlElement* pElem=pElement->FirstChildElement("QuestNode");
	if(pElem&&!LoadQuestNode_New(pElem))
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","QuestIndex.xml:QuestNode");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	pElem=pElement->FirstChildElement("QuestTree");
	if(pElem&&!LoadQuestIndex_New(pElem))
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","QuestIndex.xml:QuestTree_1");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	return true;
}

bool CQuestIndexXml::LoadQuestNode_New(TiXmlElement* pElem)
{
	for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{	    
			if(strcmp(pChildElem->Value(),"Stuff")==0)
			{
				tagQuestNode QuestNode;
				QuestNode.lNodeID = (long)atoi(pChildElem->Attribute("NodeID")); //��ȡ�������������ϵ�BUFFID
				QuestNode.lTree = (long)atoi(pChildElem->Attribute("Tree"));
				QuestNode.strNodeName = pChildElem->Attribute("NodeName");  //��ȡ��Ӧ����ЧID
				QuestNode.lParentID = (long)atoi(pChildElem->Attribute("ParentID"));
				m_vecQuestNode.push_back(QuestNode);
		    }
	}
	//�ж������Ƿ��ȡ��ȷ
	if(m_vecQuestNode.size()>0)
		return true;
	return false;
}

bool CQuestIndexXml::LoadQuestIndex_New(TiXmlElement* pElem)
{
	for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{	    
			if(strcmp(pChildElem->Value(),"Stuff")==0)
			{
				tagQuestIndex QuestIndex;
				QuestIndex.lQuestID = (long)atoi(pChildElem->Attribute("QuestID")); //��ȡ�������������ϵ�BUFFID
				QuestIndex.lTree = (long)atoi(pChildElem->Attribute("Tree"));       //������������
				QuestIndex.lTreeID = (long)atoi(pChildElem->Attribute("NodeID"));  //��ȡ��Ӧ����ЧID
				QuestIndex.iQuestNote = atoi(pChildElem->Attribute("QuestNote"));  //��ȡ������
				QuestIndex.lNumSum = (long)atoi(pChildElem->Attribute("ParamSum"));
				QuestIndex.lParamTextPos = (long)atoi(pChildElem->Attribute("ParamTextPos"));
				QuestIndex.iLevelLimit = atoi(pChildElem->Attribute("LevelLimit"));
				QuestIndex.iStep = atoi(pChildElem->Attribute("Step"));
				m_vecQuestIndex.push_back(QuestIndex);
		    }
	}
	//�ж������Ƿ��ȡ��ȷ
	if(m_vecQuestIndex.size()>0)
		return true;
	return false;
}

bool CQuestIndexXml::FindIndexQuest(long Index)
{
	vector<CQuestIndexXml::tagQuestIndex>::iterator _itor = m_vecQuestIndex.begin();
	for(;_itor!=m_vecQuestIndex.end();_itor++)
	{
		if((*_itor).lQuestID==Index)
			return true;
	}
	return false;
}

void CQuestIndexXml::SetIndextQuest(long Index,long NumParam)
{
	vector<CQuestIndexXml::tagQuestIndex>::iterator _itor = m_vecQuestIndex.begin();
	for(;_itor!=m_vecQuestIndex.end();_itor++)
	{
		if((*_itor).lQuestID==Index)
		{
			(*_itor).lNumParam = NumParam;
			return ;
		}
		
	}
}

bool CQuestIndexXml::GetIndextQuest(long Index,long &NumParam)
{
	vector<CQuestIndexXml::tagQuestIndex>::iterator _itor = m_vecQuestIndex.begin();
	for(;_itor!=m_vecQuestIndex.end();_itor++)
	{
		if((*_itor).lQuestID==Index)
		{
			
			NumParam = (*_itor).lNumParam;
			return true;
		}
	}
	return false;
}

void CQuestIndexXml::SetIndexQuestState(long Index, int istate)
{
	vector<CQuestIndexXml::tagQuestIndex>::iterator _itor = m_vecQuestIndex.begin();
	for(;_itor!=m_vecQuestIndex.end();_itor++)
	{
		if((*_itor).lQuestID==Index)
		{
			(*_itor).iState = istate;
			return ;
		}
	}
}

int CQuestIndexXml::GetIndextQuestState(long Index)
{
	vector<CQuestIndexXml::tagQuestIndex>::iterator _itor = m_vecQuestIndex.begin();
	for(;_itor!=m_vecQuestIndex.end();_itor++)
	{
		if((*_itor).lQuestID==Index)
		{
			 return (*_itor).iState;

		}
	}
	return 0;
}

int CQuestIndexXml::GetIndexQuestStep(long Index)
{
	vector<CQuestIndexXml::tagQuestIndex>::iterator _itor = m_vecQuestIndex.begin();
	for(;_itor!=m_vecQuestIndex.end();_itor++)
	{
		if((*_itor).lQuestID==Index)
		{
			 return (*_itor).iStep;

		}
	}
	return 0;
}

bool CQuestIndexXml::LoadQuestSetupXmlForServer()
{
	Release();
///////////////////////////////////////////////////
	//��ȡ�����ļ�
	CRFile *prfile = rfOpen("data\\QuestIndex.xml");
	if(NULL == prfile)
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","QuestIndex.xml");
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
		wsprintf(strError,"LoadFileError:%s","PlayerSetup.xml");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	/////////////////////////////////////////////////////////////////
	//�����̵���Ч�������
	pNode = doc.FirstChild("QuestIndex");
	if(NULL==pNode)
	{
		return false;
	}
	TiXmlElement* pElement=pNode->ToElement();
	if(NULL==pElement)
		return false;
	TiXmlElement* pElem=pElement->FirstChildElement("QuestTree");
	if(pElem&&!LoadQuestIndex_NewEx(pElem))
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s","QuestIndex.xml:QuestTree_1");
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	return true;
}

bool CQuestIndexXml::LoadQuestIndex_NewEx(TiXmlElement* pElem)
{
	for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{	    
			if(strcmp(pChildElem->Value(),"Stuff")==0)
			{
				tagQuestIndexForServer QuestIndex;
				QuestIndex.lQuestID = (long)atoi(pChildElem->Attribute("QuestID")); //��ȡ�������������ϵ�BUFFID
				QuestIndex.lNumSum = (long)atoi(pChildElem->Attribute("ParamSum"));  //����������ܹ���ɵĴ���
				QuestIndex.lReSetTimeToXml = (long)atoi(pChildElem->Attribute("ReSetTime"));
				//m_tagQuestIndexForServer.push_back(QuestIndex);
				m_tagQuestIndexForServer[QuestIndex.lQuestID] = QuestIndex;
		    }
	}
	//�ж������Ƿ��ȡ��ȷ
	if(m_tagQuestIndexForServer.size()>0)
		return true;
	return false;
}

bool CQuestIndexXml::FindIndexQuestEx(long Index)
{
	//vector<CQuestIndexXml::tagQuestIndexForServer>::iterator _itor = m_tagQuestIndexForServer.begin();
	//for(;_itor!=m_tagQuestIndexForServer.end();_itor++)
	//{
	//	if((*_itor).lQuestID==Index)
	//		return true;
	//}
	map<long,CQuestIndexXml::tagQuestIndexForServer>::iterator _itor=m_tagQuestIndexForServer.find(Index);
	if(_itor!=m_tagQuestIndexForServer.end())
		return true;
	return false;
}

void CQuestIndexXml::SetIndextQuestEx(long Index,long NumParam)
{
	//vector<CQuestIndexXml::tagQuestIndexForServer>::iterator _itor = m_tagQuestIndexForServer.begin();
	//for(;_itor!=m_tagQuestIndexForServer.end();_itor++)
	//{
	//	if((*_itor).lQuestID==Index)
	//	{
	//		(*_itor).lNumParam = NumParam;
	//		return ;
	//	}
	//	
	//}
	map<long,CQuestIndexXml::tagQuestIndexForServer>::iterator _itor=m_tagQuestIndexForServer.find(Index);
	if(_itor!=m_tagQuestIndexForServer.end())
	{
		(*_itor).second.lNumParam = NumParam;
	}
}

bool CQuestIndexXml::GetIndextQuestEx(long Index,long &NumParam)
{
	//vector<CQuestIndexXml::tagQuestIndexForServer>::iterator _itor = m_tagQuestIndexForServer.begin();
	//for(;_itor!=m_tagQuestIndexForServer.end();_itor++)
	//{
	//	if((*_itor).lQuestID==Index)
	//	{			
	//		NumParam = (*_itor).lNumParam;
	//		return true;
	//	}
	//}
	//return false;
	map<long,CQuestIndexXml::tagQuestIndexForServer>::iterator _itor=m_tagQuestIndexForServer.find(Index);
	if(_itor!=m_tagQuestIndexForServer.end())
	{
		NumParam = (*_itor).second.lNumParam;
		return true;
	}
	return false;
}

void CQuestIndexXml::SetIndexQuestStateEx(long Index,int istate)
{
	//vector<CQuestIndexXml::tagQuestIndexForServer>::iterator _itor = m_tagQuestIndexForServer.begin();
	//for(;_itor!=m_tagQuestIndexForServer.end();_itor++)
	//{
	//	if((*_itor).lQuestID==Index)
	//	{
	//		(*_itor).iState = istate;
	//		return ;
	//	}
	//}
	map<long,CQuestIndexXml::tagQuestIndexForServer>::iterator _itor=m_tagQuestIndexForServer.find(Index);
	if(_itor!=m_tagQuestIndexForServer.end())
	{
		(*_itor).second.iState = istate ;
	}
}

bool CQuestIndexXml::CodeToByteArray(DBWriteSet& dbWriteSet)
{
	dbWriteSet.AddToByteArray((DWORD)m_tagQuestIndexForServer.size());
	map<long,CQuestIndexXml::tagQuestIndexForServer>::iterator it=m_tagQuestIndexForServer.begin();
	int i=0;
	for (;it!=m_tagQuestIndexForServer.end();++it)
	{
		//dbWriteSet.AddToByteArray(&(m_tagQuestIndexForServer[i]),sizeof(tagQuestIndexForServer));
		dbWriteSet.AddToByteArray(&(*it),sizeof(tagQuestIndexForServer));
		i++;
	}
	return true;
}

bool CQuestIndexXml::DecodeToByteArray(DBReadSet& dbReadSet)
{
	DWORD dwsize=dbReadSet.GetDwordFromByteArray();	
	tagQuestIndexForServer tagQuestIndex;
	for (int i=0;i<dwsize;++i)
	{
		dbReadSet.GetBufferFromByteArray(&tagQuestIndex,sizeof(tagQuestIndexForServer));
		//m_tagQuestIndexForServer.push_back(tagQuestIndex);
		m_tagQuestIndexForServer[tagQuestIndex.lQuestID] = tagQuestIndex;
	}
	return true;
}

CQuestIndexXml::tagQuestIndexForServer CQuestIndexXml::GetIndextQuestFromXML(long Index)
{
	tagQuestIndexForServer tagQuestIndex;
	//vector<CQuestIndexXml::tagQuestIndexForServer>::iterator _itor = m_tagQuestIndexForServer.begin();
	//for(;_itor!=m_tagQuestIndexForServer.end();_itor++)
	//{
	//	if((*_itor).lQuestID==Index)
	//	{
	//		tagQuestIndex = (*_itor);
	//		return tagQuestIndex;
	//	}
	//}
	map<long,CQuestIndexXml::tagQuestIndexForServer>::iterator _itor = m_tagQuestIndexForServer.find(Index);
	if(_itor!=m_tagQuestIndexForServer.end())
	{
		tagQuestIndex = (*_itor).second;
		return tagQuestIndex;
	}
	return tagQuestIndex;
}