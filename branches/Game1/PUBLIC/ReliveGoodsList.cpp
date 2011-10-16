/*
*	file:		ReliveGoodsList.cpp
*	Brief:		������Ʒ�б���
*	detail:		
*	Authtor:	���Ƚ�
*	Datae:		2009-08-27
*/
#include "StdAfx.h"
#include "ReliveGoodsList.h"
#include "StrUnifyInput.h"
#include "public.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

list<CReLiveGoodsList::tagReLiveGoods*>  CReLiveGoodsList::m_lReLiveGoods;
map<string,CReLiveGoodsList::tagExpGoods*>  CReLiveGoodsList::m_mpExpGoods;

CReLiveGoodsList::CReLiveGoodsList()
{

}


CReLiveGoodsList::~CReLiveGoodsList()
{

}

// ����XML�й��ڸ�����Ʒ������
bool CReLiveGoodsList::Init(const char *filename)
{
	if (NULL==filename)
	{
		return false;
	}
	char strError[128];
	//=================================================================//
	CRFile *prfile = rfOpen(filename);
	if(NULL == prfile)
	{
		//���������Ϣ
		_snprintf(strError, 128, "LoadFileError:%s",filename);
		MessageBox(NULL,strError,"",MB_OK); 
		return false;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument m_Tdoc(filename);
	//���븴����Ʒ�����ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		_snprintf(strError, 128, CStringReading::LoadString(IDS_WS_SKILL,STR_WS_SKILL_FOURTH),filename);
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("Goods");
	if (pNode==NULL)
	{
		string strNode = "Goods";
		_snprintf(strError, 128, CStringReading::LoadString(IDS_WS_SKILL,STR_WS_SKILL_FIFTH),filename,strNode.c_str());
		MessageBox(NULL,strError,"",MB_OK); 
		return false;
	}
	TiXmlElement* pElem = pNode->ToElement();

	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{

		if (!stricmp(pChildElem->Value(),"ReLiveGoods"))
		{
			tagReLiveGoods *ptgReliveGoods  = new tagReLiveGoods;
			ptgReliveGoods->strOrignalName  = pChildElem->Attribute("OrignalName");
			ptgReliveGoods->dwIndex			= (DWORD)atol(pChildElem->Attribute("Index"));
			ptgReliveGoods->dwNum			= (DWORD)atol( pChildElem->Attribute("GoodsNum"));
			ptgReliveGoods->dwRecoverRatio  = (DWORD)atol(pChildElem->Attribute("RecoverRatio"));
			m_lReLiveGoods.push_back(ptgReliveGoods);		
		}
		else if(!stricmp(pChildElem->Value(),"ExpGoods"))
		{
			tagExpGoods *ptgExpGoods = new tagExpGoods;
			string strOrgName		 = pChildElem->Attribute("OrignalName");
			ptgExpGoods->dwExpRatio  = atoi(pChildElem->Attribute("ExpRatio"));			
			if (pChildElem->Attribute("Script"))
			{
				ptgExpGoods->strScript   = pChildElem->Attribute("Script");
			}
			if (pChildElem->Attribute("Hint"))
			{
				ptgExpGoods->strHint	 = pChildElem->Attribute("Hint");
			}
			
			
			map<string,tagExpGoods*>::iterator it = m_mpExpGoods.find(strOrgName);
			if (it!=m_mpExpGoods.end())
			{
				SAFE_DELETE(ptgExpGoods);
			}
			else
			{
				m_mpExpGoods[strOrgName] = ptgExpGoods;
			}
			
		}
	}
	return true;
}

// �ͷ���Դ	
void CReLiveGoodsList::Release()
{
	list<tagReLiveGoods*>::iterator it = m_lReLiveGoods.begin();
	for (;it!=m_lReLiveGoods.end(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_lReLiveGoods.clear();

	Eit eit = m_mpExpGoods.begin();
	for (; eit!=m_mpExpGoods.end(); ++eit)
	{
		SAFE_DELETE((*eit).second);
	}
	m_mpExpGoods.clear();
}
// ��ӵ�CByteArray
bool CReLiveGoodsList::AddToByteArray(vector<BYTE> &pByteArray)
{
	//������Ʒ����		
	_AddToByteArray(&pByteArray,static_cast<long>(m_lReLiveGoods.size()));

	for (list<tagReLiveGoods *>::iterator it = m_lReLiveGoods.begin();it!=m_lReLiveGoods.end();++it)
	{
		// ԭʼ��
		_AddToByteArray(&pByteArray,(char*)((*it)->strOrignalName.c_str()));			
		//����
		_AddToByteArray(&pByteArray,(*it)->dwIndex);
		//����
		_AddToByteArray(&pByteArray,(*it)->dwNum);
		//����ֵ
		_AddToByteArray(&pByteArray,(*it)->dwRecoverRatio);	
	}
	//��ȡ������Ʒ����
	_AddToByteArray(&pByteArray,static_cast<long>(m_mpExpGoods.size()));
	for (Eit it=m_mpExpGoods.begin(); it!=m_mpExpGoods.end(); ++it)
	{
		//ԭʼ��
		_AddToByteArray(&pByteArray,(char*)((*it).first.c_str()));
		//����
		_AddToByteArray(&pByteArray,(*it).second->dwExpRatio);
		//�ű�
		_AddToByteArray(&pByteArray,(*it).second->strScript.c_str());
		//��ʾ
		_AddToByteArray(&pByteArray,(*it).second->strHint.c_str());
	}


	return true;
}

// ����	
 bool CReLiveGoodsList::DecordFromByteArray(BYTE* pByte, long& pos)
{
	//������Ʒ����
	
	long lNum = _GetLongFromByteArray(pByte,pos);
	for(long l=0; l<lNum; ++l )
	{
		char strTemp[256];
		tagReLiveGoods *ptgReliveGoods  = new tagReLiveGoods;
		_GetStringFromByteArray(pByte,pos,strTemp);
		ptgReliveGoods->strOrignalName = strTemp;
		ptgReliveGoods->dwIndex			 = _GetDwordFromByteArray(pByte,pos);
		ptgReliveGoods->dwNum			 = _GetDwordFromByteArray(pByte,pos);
		ptgReliveGoods->dwRecoverRatio   = _GetDwordFromByteArray(pByte,pos);
		m_lReLiveGoods.push_back(ptgReliveGoods);

	}
	//���վ�����Ʒ
	long lENum = _GetLongFromByteArray(pByte,pos);
	for (long i=0; i<lENum; ++i)
	{	
		char strOrgName[256];
		char strTemp[1024];
		tagExpGoods *ptgExpGoods = new tagExpGoods;
		_GetStringFromByteArray(pByte,pos,strOrgName);
		ptgExpGoods->dwExpRatio = _GetLongFromByteArray(pByte,pos);
		_GetStringFromByteArray(pByte,pos,strTemp);
		ptgExpGoods->strScript = strTemp;
		_GetStringFromByteArray(pByte,pos,strTemp);
		ptgExpGoods->strHint = strTemp;
		m_mpExpGoods[strOrgName] = ptgExpGoods;
	}
	return true;
}

//����ԭʼ����ȡ��Ʒ���Ӿ������ز���
CReLiveGoodsList::tagExpGoods* CReLiveGoodsList::GetExpGoods(string strName)
 {
	 Eit it = m_mpExpGoods.find(strName);
		 if (it!=m_mpExpGoods.end())
		 {
			 return (*it).second;
		 }
		 return NULL;
 }
