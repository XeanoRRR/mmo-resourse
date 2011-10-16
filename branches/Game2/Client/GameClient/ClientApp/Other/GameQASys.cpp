#include "stdafx.h"
#include "GameQASys.h"

#include "../../../Public/Common/CStrConverter.h"
CGameQASys::tagQAConfig		CGameQASys::m_tagQaConfig[CGameQASys::QA_TYPE_TOTAL];
CGameQASys::tagQaDoc		CGameQASys::m_tagQaDoc[CGameQASys::QA_TYPE_TOTAL];

CGameQASys::CGameQASys()
{

}
CGameQASys::~CGameQASys()
{

}
void CGameQASys::InitialGameQaSys()
{
	static const char GameQaSysConfigFileName[128] = "data\\GameQA\\QaSysConfig.xml";
	char strError[128];
	CRFile *prfile = rfOpen(GameQaSysConfigFileName);
	if(NULL == prfile)
	{
		//���������Ϣ
		wsprintf(strError,"LoadFileError:%s",GameQaSysConfigFileName);
		MessageBox(NULL,strError,"",MB_OK); 
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument m_Tdoc(GameQaSysConfigFileName);
	//���������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		wsprintf(strError, "%s Load Failed!" ,GameQaSysConfigFileName);
        assert(strError);
	}
	rfClose(prfile);

	// �ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	if( !( pNode = m_Tdoc.FirstChild( "QaSystem" ) ) )
	{
		MessageBox( GetActiveWindow(), "Can't find the GameQAList node in GameQAList.xml file!", "Error", MB_OK );
	}
	// �����ӽڵ�
	TiXmlElement* pElem = pNode->ToElement();
	long lID = 0;
	for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem; pChildElem = pChildElem->NextSiblingElement())
	{	
		if( !strcmp( pChildElem->Value(), "Subject" ))
		{
			
			if( pChildElem->Attribute("ID"))
			{
				lID = CStrConverter::parseLong( pChildElem->Attribute( "ID" ) );
				if(lID >= (long)CGameQASys::QA_TYPE_TOTAL)
				{
					wsprintf(strError,"Qa Confige Subject Can't Over more than %d Item",(long)CGameQASys::QA_TYPE_TOTAL);
					MessageBox(NULL,strError,"",MB_OK);
					break;
				}
				m_tagQaConfig[lID].type = (QA_TYPE)lID;
			}
			if( pChildElem->Attribute("Name"))
			{
				m_tagQaConfig[lID].strDesc = pChildElem->Attribute("Name");
			}

			TiXmlElement* ppChildElem = pChildElem->FirstChildElement();
			if(!strcmp(ppChildElem->Value(),"TreeFileName"))
				m_tagQaConfig[lID].strQuestionTreeFileName = ppChildElem->GetText();
			
			TiXmlElement* pppCElem = ppChildElem->NextSiblingElement();
			if(!strcmp(pppCElem->Value(),"ContentFileName"))
				m_tagQaConfig[lID].strAnswerContentFileName = pppCElem->GetText();

		}
	}
}

void CGameQASys::LoadQaDoc(QA_TYPE type)
{
	if(type < QA_BEGINER || type>= QA_TYPE_TOTAL)
	{
		MessageBox(NULL,"Load Qa System Document Error","Error",MB_OK);
		return;
	}
	const char * strTreeConfigFile		= m_tagQaConfig[type].strQuestionTreeFileName.c_str();
	const char * strAnswerContentFile	= m_tagQaConfig[type].strAnswerContentFileName.c_str();
	char strError[128];
	
	if(m_tagQaDoc[type].isTreeLoaded == false)
	{
		//�������οؼ����������ļ�
		CRFile *prfile = rfOpen(strTreeConfigFile);
		if(NULL == prfile)
		{
			//���������Ϣ
			wsprintf(strError,"LoadFileError:%s",strTreeConfigFile);
			assert(strError);
            rfClose(prfile);
		}

		m_tagQaDoc[type].m_QaTreeDoc.LoadFile(strTreeConfigFile);
		//���������ļ�
		if( !m_tagQaDoc[type].m_QaTreeDoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
		{
			//���������Ϣ
            wsprintf(strError,"LoadFileError: %s",strTreeConfigFile);
			assert(strError);
			rfClose(prfile);
		}
		rfClose(prfile);
		m_tagQaDoc[type].isTreeLoaded = true;
	}
	//----------------------------------------------------------------------------------

	if(m_tagQaDoc[type].isAnswerLoaded == false)
	{
		//���ش������ļ�
		CRFile *prfile = rfOpen(strAnswerContentFile);
		if(NULL == prfile)
		{
			//���������Ϣ
			wsprintf(strError,"LoadFileError:%s",strAnswerContentFile);
			MessageBox(NULL,strError,"",MB_OK); 
		}
		TiXmlNode *pNode = NULL; 	
		TiXmlDocument m_Tdoc(strAnswerContentFile);
		m_Tdoc.LoadFile(strAnswerContentFile);
		//���������ļ�
		if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
		{
			//���������Ϣ
			wsprintf(strError,"Load FileError %s",strAnswerContentFile);
			assert(strError);
			rfClose(prfile);
		}
		rfClose(prfile);

		// �ҵ����ڵ㣬����޽ڵ㣬���������ʾ
		if( !( pNode = m_Tdoc.FirstChild( "RootNode" ) ) )
		{
			wsprintf(strError,"Can not find the RootNode from file %s",strAnswerContentFile);
			MessageBox( GetActiveWindow(), strError, "Error", MB_OK );
		}
		//װ�ص�ǰ���͵Ĵ�����
		TiXmlElement* pElem = pNode->ToElement();
		for(TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem; pChildElem = pChildElem->NextSiblingElement())
		{
			if( !strcmp( pChildElem->Value(), "Node" ))
			{
				const char * strAnswerContent = NULL;			
				DWORD dwAnswerIndex = -1;
				if( pChildElem->Attribute("ID"))
				{
					dwAnswerIndex		=  CStrConverter::parseLong(pChildElem->Attribute( "ID" )) ;
				}
				strAnswerContent =  pChildElem->GetText();
				m_tagQaDoc[type].mapQaAnswerDB.insert(std::pair<DWORD,string>(dwAnswerIndex,strAnswerContent));
			}
		}
		m_tagQaDoc[type].isAnswerLoaded = true;
	}
}