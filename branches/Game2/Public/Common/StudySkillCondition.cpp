#include "StdAfx.h"
#include "StudySkillCondition.h"
#include "Public.h"

#include "../AppFrame/TextRes.h"
using namespace AppFrame;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

list<CStudySkillCondition::tagCondition *> CStudySkillCondition::m_StudyConditionList;
map<string,CStudySkillCondition::tagTeachSkill*> CStudySkillCondition::m_mapTeachSkill;
list<long>										 CStudySkillCondition::m_lFactureSkillList;

CStudySkillCondition::CStudySkillCondition()
{}

CStudySkillCondition::~CStudySkillCondition()
{}

// �������ܵĸ���ѧϰ����
bool CStudySkillCondition::InitConditionEx(vector<tagConditionEx*> &vec_ConditionEx, TiXmlElement* pElem)
{
	if(pElem !=  NULL)
	{
		for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
			pChildElem=pChildElem->NextSiblingElement())
		{
			tagConditionEx *stConditionEx = new tagConditionEx;
			// ��������
			if(!strcmp(pChildElem->Value(),"Goods"))
			{
				if(NULL!=pChildElem->Attribute("name") && NULL!=pChildElem->Attribute("num"))
				{
					stConditionEx->wConditionExType = CONDITIONEX_GOODS;
					stConditionEx->strValue = pChildElem->Attribute("name");
					stConditionEx->lValue = ((long)atoi(pChildElem->Attribute("num")));
				}
			}
			// ǰ����������
			else if(!strcmp(pChildElem->Value(),"PreSkill"))
			{
				if(NULL!=pChildElem->Attribute("ID") && NULL!=pChildElem->Attribute("Levlimit"))
				{
					stConditionEx->wConditionExType = CONDITIONEX_PRESKILL;
					stConditionEx->strValue = pChildElem->Attribute("ID");
					stConditionEx->lValue = ((long)atoi(pChildElem->Attribute("Levlimit")));
				}
			}
			// ��ͻ��������
			else if(!strcmp(pChildElem->Value(),"ConflictSkill"))
			{
				if(NULL!=pChildElem->Attribute("ID"))
				{
					stConditionEx->wConditionExType = CONDITIONEX_CONFLICTSKILL;
					stConditionEx->strValue = pChildElem->Attribute("ID");
				}
			}
			else if (!strcmp(pChildElem->Value(),"Script"))
			{
				if (NULL!=pChildElem->Attribute("name"))
				{
					stConditionEx->wConditionExType = CONDITIONEX_SCRIPT;
					stConditionEx->strValue = pChildElem->Attribute("name");
				}
			}
			// ����
			else
				continue;
			vec_ConditionEx.push_back(stConditionEx);
		}
		return true;
	}
	return false;
}

// �������ܵ�ѧϰ����
bool CStudySkillCondition::InitCondition(ulong dwID, TiXmlElement* pElem)
{
	if(pElem !=  NULL)
	{
		// �������ܽڵ㣬��ÿ�����ܽڵ�ļ��ܴ洢����
		for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL;
			pChildElem=pChildElem->NextSiblingElement())
		{
			if(!strcmp(pChildElem->Value(),"Lev")
				&& NULL!=pChildElem->Attribute("value")
				&& NULL!=pChildElem->Attribute("PlayerLev")
				&& NULL!=pChildElem->Attribute("Sp")
				&& NULL!=pChildElem->Attribute("OccuLv"))
			{
				tagCondition * pCondition = new tagCondition;
				
				pCondition->dwSkillID =  dwID; 
				pCondition->wSkillLev = ((ushort)atoi(pChildElem->Attribute("value")));
				pCondition->wPlayerLev = ((ushort)atoi(pChildElem->Attribute("PlayerLev")));
				pCondition->dwSp = ((ulong)atoi(pChildElem->Attribute("Sp")));
				pCondition->dwOccuLv = ((ulong)atoi(pChildElem->Attribute("OccuLv")));
				if(NULL!=pChildElem->Attribute("StudyMode"))
					pCondition->wStudyMode = ((ushort)atoi(pChildElem->Attribute("StudyMode")));
				InitConditionEx(pCondition->vec_ConditionEx,pChildElem);
				m_StudyConditionList.push_back(pCondition);
			}
		}
	}
	return false;
}
	//��ʼ��רҵ���������б�
bool CStudySkillCondition::InitFactureSkillList(const char *filename)
{
	if (NULL==filename)
	{
		return false;
	}
	//char strError[128];
	//=================================================================//
	CRFile *prfile = rfOpen(filename);
	if(NULL == prfile)
	{
		//���������Ϣ
        Log4c::Error(ROOT_MODULE,"%-15s LoadFileError:%s",__FUNCTION__,filename);
		return false;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument m_Tdoc(filename);
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_FOURTH"),filename);
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("SkillList");
	if (pNode==NULL)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_FIFTH"));
		return false;
	}
	TiXmlElement* pElem = pNode->ToElement();

	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{

		if (!stricmp(pChildElem->Value(),"Skill"))
		{
			long lID = GetDwordAttribute(pChildElem,"id");
			m_lFactureSkillList.push_back(lID);		
		}
	}
	return true;
}

//����ѧϰ�����б�
bool CStudySkillCondition::InitSkillList(const char *filename)
{
	if (NULL==filename)
	{
		return false;
	}
	//char strError[128];
	//=================================================================//
	CRFile *prfile = rfOpen(filename);
	if(NULL == prfile)
	{
		//���������Ϣ
        Log4c::Error(ROOT_MODULE,"%-15s LoadFileError:%s",__FUNCTION__,filename);
		return false;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument m_Tdoc(filename);
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_FOURTH"),filename);
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("StudySkillList");
	if (pNode==NULL)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_FIFTH"));
		return false;
	}
	TiXmlElement* pElem = pNode->ToElement();
	// �����ӽڵ㣬���ü���ID
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if(!strcmp(pChildElem->Value(),"StudyTeacher")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("occ"))
		{
			string strTeacherName = pChildElem->Attribute("name");
			CStudySkillCondition::tagTeachSkill *ptgTeachSkill = new CStudySkillCondition::tagTeachSkill;
			ptgTeachSkill->strOcc = pChildElem->Attribute("occ");

			list<ulong> lSkillList;
			for (TiXmlElement *pSub=pChildElem->FirstChildElement(); pSub!=NULL; 
				pSub=pSub->NextSiblingElement())
			{
				if(!strcmp(pSub->Value(),"Skill")
					&& NULL!=pSub->Attribute("id"))
				{
					ulong dwSkillID = GetDwordAttribute(pSub,"id");
					ptgTeachSkill->lSkillList.push_back(dwSkillID);
					//lSkillList.push_back(dwSkillID);
				}
				else if (!strcmp(pSub->Value(),"Occ")
					&& NULL!=pSub->Attribute("id"))
				{
					ulong dwOccu = GetDwordAttribute(pSub,"id");
					ptgTeachSkill->lOccList.push_back(dwOccu);
				}
			}
			m_mapTeachSkill[strTeacherName] = ptgTeachSkill;

		}
	}

	return true;

}
// ����XML�й��ڼ��ܵ�����
bool CStudySkillCondition::Init(const char *filename)
{
	if(filename != NULL)
	{
		Release();

		//char strError[128];
		CRFile *prfile = rfOpen(filename);
		if(NULL == prfile)
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_FOURTH"),filename);
			return false;
		}
		
		TiXmlNode *pNode = NULL;
		TiXmlDocument m_Tdoc;		

		if(! m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_FOURTH"),filename);
			return false;
		}
		rfClose(prfile);

		//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
		pNode = m_Tdoc.FirstChild("StudySkillCondition");
		if (pNode==NULL)
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_SIXTH"));
			return false;
		}
		//����ת��
		TiXmlElement* pElem = pNode->ToElement();
		// �����ӽڵ㣬���ü���ID
		for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
			pChildElem=pChildElem->NextSiblingElement())
		{
			if(!strcmp(pChildElem->Value(),"Skill")
				&& NULL!=pChildElem->Attribute("ID"))
			{				
				ulong wSkillID = GetDwordAttribute(pChildElem,"ID");
				// ��ÿ�����ܵ�ѧϰ����
 				InitCondition(wSkillID, pChildElem);		
			}
		}
		m_Tdoc.Clear();
		return true;
	}
	return false;
}

ulong CStudySkillCondition::GetDwordAttribute(TiXmlElement *pElem,string strName)
{
	double dValue;
	if(pElem->QueryDoubleAttribute(strName.c_str(),&dValue))
	{
		dValue = 0;
	}
	return (ulong)dValue;
}

// ����Ҫѧϰ���ܵ�ID�Լ���Ҫ�����ĵȼ����ҳ�������ܵĸ���ѧϰ����
CStudySkillCondition::tagCondition * CStudySkillCondition::GetCondition(ulong dwSkillID, ushort wSkillLev)
{
	tagCondition *pCondition = NULL;
	list<tagCondition *>::iterator it = m_StudyConditionList.begin();
	for(; it != m_StudyConditionList.end(); ++it)
	{
		pCondition = (*it);
		if(pCondition)
		{
			// �ҵ���Ӧ���ܵĶ�Ӧ�ȼ�
			if(pCondition->dwSkillID == dwSkillID && pCondition->wSkillLev == wSkillLev)
			{
				return pCondition;
			}
		}
	}
	return NULL;
}

//ѧϰ��������
bool  CStudySkillCondition::AddToByteArray(vector<uchar> &pByteArray)		// ��ӵ�CByteArray
{
		//����ѧϰ�����б����		
		_AddToByteArray(&pByteArray,static_cast<long>(m_StudyConditionList.size()));
		
		for (list<tagCondition *>::iterator it = m_StudyConditionList.begin();it!=m_StudyConditionList.end();++it)
		{
			//ulong dwSkillID;			// ����ID
			_AddToByteArray(&pByteArray,(*it)->dwSkillID);			
			//ushort wSkillLev;			// ��ǰ���ܵĵȼ�
			_AddToByteArray(&pByteArray,(*it)->wSkillLev);
			//ushort wStudyMode;		// ��ǰ�ȼ���ѧϰģʽ
			_AddToByteArray(&pByteArray,(*it)->wStudyMode);
			//ushort wPlayerLev;		// Ҫ����ҵĵȼ�����
			_AddToByteArray(&pByteArray,(*it)->wPlayerLev);
			//ushort wSp;				// ѧϰ��Ҫ���ĵ�SPֵ
			_AddToByteArray(&pByteArray,(*it)->dwSp);
			//ְҵ�ȼ�
			_AddToByteArray(&pByteArray,(*it)->dwOccuLv);
			//������������
			_AddToByteArray(&pByteArray,static_cast<long>((*it)->vec_ConditionEx.size()));
			//
			 for (vector<tagConditionEx*>::iterator eit=(*it)->vec_ConditionEx.begin(); eit!=(*it)->vec_ConditionEx.end();++eit)
			 {
				 //��������������
				 _AddToByteArray(&pByteArray,static_cast<ulong>((*eit)->wConditionExType));
				 // ��һ��ֵ���п�����������Ʒ��ԭʼ����Ҳ�п�����������ID��XML����ʱ�����ַ�����ʽ���棩
				 _AddToByteArray(&pByteArray,(char*)((*eit)->strValue.c_str()));																	 
				 //�ڶ���ֵ�����ڸ��ӵ�����˵�����������������ڸ���ǰ�ü��ܣ��������ܵȼ������Ƽ������޴��ʼ��Ϊ-1��
				 _AddToByteArray(&pByteArray,(*eit)->lValue);
			 }
			//vector<tagConditionEx*>  vec_ConditionEx;	// ����ѧϰ�����б�
		}
		//��ʦ����
		_AddToByteArray(&pByteArray,static_cast<long>(m_mapTeachSkill.size()));
	
		for (map<string,tagTeachSkill*>::iterator it = m_mapTeachSkill.begin();it!=m_mapTeachSkill.end();++it)
		{
			//��ʦ����
			_AddToByteArray(&pByteArray,(char*)it->first.c_str());
			//ְҵ
			_AddToByteArray(&pByteArray,(char*)it->second->strOcc.c_str());
			//ְҵ��Χ
			_AddToByteArray(&pByteArray,static_cast<long>(it->second->lOccList.size()));
			list<ulong>::iterator Oit=it->second->lOccList.begin();
			for(;Oit!=it->second->lOccList.end();++Oit)
			{
				_AddToByteArray(&pByteArray,(*(Oit)));
			}
			//���ܸ���
			_AddToByteArray(&pByteArray,static_cast<long>(it->second->lSkillList.size()));
			list<ulong>::iterator sit=it->second->lSkillList.begin();
			for(;sit!=it->second->lSkillList.end();++sit)
			{
				_AddToByteArray(&pByteArray,(*sit));
			}
		}
	//רҵ�������ܸ���
	_AddToByteArray(&pByteArray,static_cast<long>(m_lFactureSkillList.size()));
	for (list<long>::iterator it=m_lFactureSkillList.begin();it!=m_lFactureSkillList.end();++it)
	{
		_AddToByteArray(&pByteArray,(*it));
	}
	return true;	
		
		
}
// ����
bool  CStudySkillCondition::DecordFromByteArray(uchar* pByte, long& pos)
{
	//����ѧϰ�����б����
	Release();
	char strTemp[256];
	long lSkillNum = _GetLongFromByteArray(pByte,pos);
	for (long l=0; l<lSkillNum; ++l)
	{
		tagCondition *ptgCondition = new tagCondition;
		ptgCondition->dwSkillID = _GetDwordFromByteArray(pByte,pos);
		ptgCondition->wSkillLev = _GetWordFromByteArray(pByte,pos);
		ptgCondition->wStudyMode = _GetWordFromByteArray(pByte,pos);
		ptgCondition->wPlayerLev = _GetWordFromByteArray(pByte,pos);
		ptgCondition->dwSp = _GetDwordFromByteArray(pByte,pos);
		ptgCondition->dwOccuLv = _GetDwordFromByteArray(pByte,pos);
		//������������
		long  lESkillNum =	_GetLongFromByteArray(pByte,pos);
		for (long j=0; j<lESkillNum; ++j)
		{
			tagConditionEx *ptgConditionEx = new tagConditionEx;
			ptgConditionEx->wConditionExType = (eConditionEx)_GetDwordFromByteArray(pByte,pos);// _GetDWordFromByteArray();
			_GetStringFromByteArray(pByte,pos,strTemp);
			ptgConditionEx->strValue = strTemp;
			ptgConditionEx->lValue = _GetLongFromByteArray(pByte,pos);
			ptgCondition->vec_ConditionEx.push_back(ptgConditionEx);			
		}		
		m_StudyConditionList.push_back(ptgCondition);		
	}
	long lTNum = _GetLongFromByteArray(pByte,pos);
	for(long l=0; l<lTNum; ++l)
	{
		//��ʦ����
		_GetStringFromByteArray(pByte,pos,strTemp);
		string strTeacherName = strTemp;//_GetDwordFromByteArray(pByte,pos);
		tagTeachSkill *ptgTeachSkill = new tagTeachSkill;
		//ְҵ
		_GetStringFromByteArray(pByte,pos,strTemp);
		ptgTeachSkill->strOcc = strTemp;
		long lONum = _GetLongFromByteArray(pByte,pos);
		for (long j=0;j<lONum;++j)
		{
			//ְҵ��Χ
			ulong dwOccu = _GetDwordFromByteArray(pByte,pos);
			ptgTeachSkill->lOccList.push_back(dwOccu);
		}
		long lSNum = _GetLongFromByteArray(pByte,pos);
		//list<ulong> lSkillList;
		for(long j=0; j<lSNum; ++j)
		{
			//�����б�
			ulong dwSkillID = _GetDwordFromByteArray(pByte,pos);
			ptgTeachSkill->lSkillList.push_back(dwSkillID);
		}
		m_mapTeachSkill[strTeacherName]=ptgTeachSkill;
	}

	//רҵ�������ܸ���
	long lFNum = _GetLongFromByteArray(pByte,pos);
	for (long l=0; l<lFNum; ++l);
	{
		long lID = _GetLongFromByteArray(pByte,pos);
		m_lFactureSkillList.push_back(lID);
	}
   return false;
}
bool CStudySkillCondition::CheckFactureSkill(long lID)
{
	list<long>::iterator it =find(m_lFactureSkillList.begin(),m_lFactureSkillList.end(),lID);
	if (it!=m_lFactureSkillList.end())
	{
		return true;
	}
	return false;
}


// �ͷ���Դ
void CStudySkillCondition::Release()
{
	list<tagCondition *>::iterator it = m_StudyConditionList.begin();
	for(; it != m_StudyConditionList.end(); ++it)
	{
		//(*it)->vec_ConditionEx.clear();
		vector<tagConditionEx*>::iterator eit = (*it)->vec_ConditionEx.begin();
		for (;eit!=(*it)->vec_ConditionEx.end();++eit)
		{
			SAFE_DELETE(*eit);
		}
		(*it)->vec_ConditionEx.clear();
		SAFE_DELETE(*it);
	}
	m_StudyConditionList.clear();
	map<string,tagTeachSkill*>::iterator mit = m_mapTeachSkill.begin();
	for (;mit!=m_mapTeachSkill.end();++mit)
	{
		SAFE_DELETE(mit->second);
	}
	m_mapTeachSkill.clear();
	
}