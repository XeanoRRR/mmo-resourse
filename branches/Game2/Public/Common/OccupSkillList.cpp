#include "StdAfx.h"
#include "OccupSkillList.h"
#include "Public.h"

#include "../AppFrame/TextRes.h"
using namespace AppFrame;

#ifdef _DEBUG
#define ENABLE_ALOADER
#include "../Package/ClientResLoader.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
list<COccupSkill *> COccupSkillList::m_OccupSkillList;


COccupSkill::COccupSkill()
{
	m_strName = "";
	m_strDesc = "";
	m_lOccupId = -1;
}

COccupSkill::~COccupSkill()
{
	Release();
}

// ����XML�й��ڼ��ܵ�����
bool COccupSkill::Init(TiXmlElement* pElem)
{
	if(pElem != NULL)
	{
		// �������ܽڵ㣬��ÿ�����ܽڵ�ļ��ܴ洢����
		for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
			pChildElem=pChildElem->NextSiblingElement())
		{
			if(!strcmp(pChildElem->Value(),"Skill")
				&& NULL!=pChildElem->Attribute("name")
				&& NULL!=pChildElem->Attribute("ID")
				&& NULL!=pChildElem->Attribute("lev"))
			{
				tagOccupSkill *stOccupSkill= new tagOccupSkill;
				stOccupSkill->strName = pChildElem->Attribute("name");
				stOccupSkill->lSkillID = (long)atoi(pChildElem->Attribute("ID"));
				stOccupSkill->lLeve = (long)atoi(pChildElem->Attribute("lev"));
				m_vecSkill.push_back(stOccupSkill);
			}
		}
		return true;
	}
	return false;
}


// �ͷ���Դ
void COccupSkill::Release()
{
	m_strName = "";
	m_strDesc = "";
	m_lOccupId = -1;
	for (vector<tagOccupSkill*>::iterator it= m_vecSkill.begin();
		it!=m_vecSkill.end();++it)
	{
		SAFE_DELETE(*it);
	}
	m_vecSkill.clear();
}


// ��ø�ְҵ�µ�ָ�����ܵ����ȼ�
long COccupSkill::GetOccupSkillMaxLev(ulong dwSkillID)
{
	for (vector<tagOccupSkill*>::iterator it= m_vecSkill.begin();
		it!=m_vecSkill.end();++it)
	{
		if((*it)->lSkillID == dwSkillID)
			return (*it)->lLeve;
	}
	return 0;
}



// ����ְҵ�����б�
COccupSkillList::COccupSkillList()
{}

COccupSkillList::~COccupSkillList()
{
	Release();
}

// ��ʼ������XML�����ļ�
bool COccupSkillList::Init(const char *filename)
{
	if(!filename)	return false;
	Release();
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
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SKILL_FOURTH",filename));

		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("OccupSkillList");
	if (pNode==NULL)
	{
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_SEVENTH"));
		return false;
	}
	//����ת��
	TiXmlElement* pElem = pNode->ToElement();
	// �����ӽڵ㣬��������
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if(!strcmp(pChildElem->Value(),"Occup")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("ID"))
		{
			COccupSkill * pOccupSkill = new COccupSkill;
			pOccupSkill->SetName(pChildElem->Attribute("name"));
			pOccupSkill->SetOccupId((long)atoi(pChildElem->Attribute("ID")));
			// ְҵ����
			if(NULL!=pChildElem->Attribute("Desc"))
			{
				pOccupSkill->SetDesc(pChildElem->Attribute("Desc"));
			}
			// ��ÿ��ְҵ�ļ����б�
			if(pOccupSkill->Init(pChildElem))
				m_OccupSkillList.push_back(pOccupSkill);
			// ��ȡְҵ�����б�ʧ��
			else
			{
				delete pOccupSkill;
				pOccupSkill = NULL;
			}
		}
	}
	m_Tdoc.Clear();
	return true;
}

//����
bool COccupSkillList::AddToByteArray(vector<uchar>  &pByteArray)
{
	 //�����б����
	 _AddToByteArray(&pByteArray,static_cast<long>(m_OccupSkillList.size()));
	 for (list<COccupSkill*>::iterator it = m_OccupSkillList.begin();it!=m_OccupSkillList.end();++it)
	 {
		//string m_strName;		// ְҵ��
		 //_AddToByteArray(pByteArray,(*it)->GetName());
		 // ְҵ����
		 //_AddToByteArray(pByteArray,(*it)->GetDesc());
		 //ְҵID(ͼƬID�����߶�Ӧenum eOccupation�е�ֵ)
		 _AddToByteArray(&pByteArray,(*it)->GetOccupId());
		 // ��ְҵ��Ӧ�ļ����б�
		 vector<COccupSkill::tagOccupSkill*> vcSkill = (*it)->GetOccupSkill();
		 //���ܸ���
		 _AddToByteArray(&pByteArray,static_cast<long>(vcSkill.size()));
		 
		 vector<COccupSkill::tagOccupSkill*>::iterator cit=vcSkill.begin();
		 for (;cit!=vcSkill.end();++cit)
		 {
			 //long lSkillID;		// ����ID
			 _AddToByteArray(&pByteArray,(*cit)->lSkillID);			 
			 //long lLeve;			// ����ܹ����ļ���
			 _AddToByteArray(&pByteArray,(*cit)->lLeve);
		 }
	 }
	 return true;
}

bool COccupSkillList::DecordFromByteArray(uchar* pByte, long& pos)
{
	//�����б����
	Release();
	long lSkillListNum = _GetLongFromByteArray(pByte,pos);
	for (long i=0; i<lSkillListNum; ++i)
	{
		COccupSkill *pOccupSkill = new COccupSkill;
		pOccupSkill->SetOccupId(_GetLongFromByteArray(pByte,pos));
		//���ܸ���
		long lSkillNum = _GetLongFromByteArray(pByte,pos);
		for (long j=0; j<lSkillNum; ++j)
		{
			 COccupSkill::tagOccupSkill * ptgOccupSkill= new COccupSkill::tagOccupSkill;
			 ptgOccupSkill->lSkillID = _GetLongFromByteArray(pByte,pos);
			 ptgOccupSkill->lLeve = _GetLongFromByteArray(pByte,pos);
			 pOccupSkill->SetSkill(ptgOccupSkill);
		}
		m_OccupSkillList.push_back(pOccupSkill);	 		
	}
	return true;
}
 
// ����ְҵID��ö�Ӧְҵ�ļ����б���
COccupSkill * COccupSkillList::GetOccupSkill(long lOccupID)
{
	if(!m_OccupSkillList.empty())
	{
		list<COccupSkill *>::iterator it = m_OccupSkillList.begin();
		for(; it != m_OccupSkillList.end(); ++it)
		{
			// �ҵ�����
			if(lOccupID == (*it)->GetOccupId())
				return (*it);
		}
	}
	return NULL;
}
bool COccupSkillList::OccuJudgeStudy(ulong dwID,ulong dwLv,long lOCcupID)
{
	//vector<COccupSkill::tagOccupSkill> vcOccupSkill = COccupSkill::get
	//string str="Occu";
	COccupSkill* pOccupSkill = COccupSkillList::GetOccupSkill(lOCcupID);
	vector<COccupSkill::tagOccupSkill*> vcOccupSkill = pOccupSkill->GetOccupSkill();
	vector<COccupSkill::tagOccupSkill*>::iterator it=vcOccupSkill.begin();
	for (;it!=vcOccupSkill.end();++it)
	{
		if (((*it)->lSkillID==(long)dwID)&&((*it)->lLeve>=(long)dwLv))
		{
			return true;
		}
	}	
	return false;
}

// �ͷ���Դ
void COccupSkillList::Release()
{
	list<COccupSkill *>::iterator it = m_OccupSkillList.begin();
	for(; it != m_OccupSkillList.end(); ++it)
	{
		SAFE_DELETE(*it);
		//*it = NULL;
	}
	m_OccupSkillList.clear();
}

/*******************************************************************
* תְϵͳ����
*******************************************************************/
vector<CChangeOccu::tagChangeOccu>	CChangeOccu::m_vecSubOccu;

CChangeOccu::CChangeOccu()
{}

CChangeOccu::~CChangeOccu()
{}

// �ͷ���Դ
void CChangeOccu::Release()
{
	m_vecSubOccu.clear();
}

// ��ʼ������XML�����ļ�
bool CChangeOccu::Init(const char *filename)
{
	if(filename)
	{
		m_vecSubOccu.clear();
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
			rfClose(prfile);
			return false;
		}
		rfClose(prfile);
		//=================================================================//
		//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
		pNode = m_Tdoc.FirstChild("ChangeOccu");
		if (pNode==NULL)
		{
			return false;
		}
		//����ת��
		TiXmlElement* pElem = pNode->ToElement();
		// �����ӽڵ㣬��������
		for (TiXmlElement *pOccuElem=pElem->FirstChildElement(); pOccuElem!=NULL; 
			pOccuElem=pOccuElem->NextSiblingElement())
		{
			if(!strcmp(pOccuElem->Value(),"Occu")
				&& NULL!=pOccuElem->Attribute("name")
				&& NULL!=pOccuElem->Attribute("id")
				&& NULL!=pOccuElem->Attribute("parentid")
				&& NULL!=pOccuElem->Attribute("value")
				&& NULL!=pOccuElem->Attribute("goods")
				&& NULL!=pOccuElem->Attribute("goodsNum"))
			{
				tagChangeOccu stChangeOccu;
				stChangeOccu.strName		= pOccuElem->Attribute("name");
				stChangeOccu.strGoodsName	= pOccuElem->Attribute("goods");
				stChangeOccu.nID			= atoi(pOccuElem->Attribute("id"));
				stChangeOccu.nParentID		= atoi(pOccuElem->Attribute("parentid"));
				stChangeOccu.nValue			= atoi(pOccuElem->Attribute("value"));
				stChangeOccu.lGoodsNum		= atoi(pOccuElem->Attribute("goodsNum"));
				m_vecSubOccu.push_back(stChangeOccu);
			}
		}
		return true;
	}
	return false;
}


// ���ݴ����ְҵ�����תְ��������Ʒ����������
void CChangeOccu::GetChangeOccuCondision(uchar bOccu,string &str,int &num)
{
	if(bOccu >= 0 && bOccu <= 21 && !m_vecSubOccu.empty())
	{
		str = m_vecSubOccu[bOccu].strGoodsName;
		num = m_vecSubOccu[bOccu].lGoodsNum;
	}
}

// ���ݿ�ʼ�ͽ�����ְҵ������תְ·��
void CChangeOccu::GetChangeOccuPath(vector<uchar> &vecOccu,uchar bStart,uchar bEnd)
{
	vecOccu.clear();
	// ��ְҵ
	if(bStart == bEnd || m_vecSubOccu.empty())
		return;
	// ������ʼְҵ��Ŀ��ְҵΪ���ӹ�ϵ
	if(m_vecSubOccu[bStart].nParentID == bEnd || m_vecSubOccu[bEnd].nParentID == bStart)
	{
		vecOccu.push_back(bStart);
		vecOccu.push_back(bEnd);
		return;
	}
	vector<char> vecChar;
	char start[6] = "";
	sprintf_s(start,"%d",m_vecSubOccu[bStart].nValue);
	char end[6] = "";
	sprintf_s(end,"%d",m_vecSubOccu[bEnd].nValue);
	size_t bStartLen = strlen(start);
	size_t bEndLen = strlen(end);
	start[bStartLen] = '\0';
	end[bEndLen] = '\0';
	// ������ת�ڵ�
	while (bStartLen > 0 && bEndLen > 0)
	{
		if(start[bStartLen - 1] == end[bEndLen - 1])
			vecChar.push_back(start[bStartLen - 1]);
		else
			break;
		start[bStartLen-1] = '\0';
		end[bEndLen-1] = '\0';
		bStartLen = strlen(start);
		bEndLen = strlen(end);
	}
	// ����������ת�ڵ�
	int i = 0;
	while(!vecChar.empty())
	{
		start[i] = vecChar.back();
		start[i+1] = '\0';
		vecChar.pop_back();
		++i;
	}
	// ��ת�ڵ�ID
	int nNode = atoi(start);
	tagChangeOccu stChangeOccu;
	for (size_t i=0; i<m_vecSubOccu.size(); ++i)
	{
		if(m_vecSubOccu[i].nValue == nNode)
		{
			stChangeOccu = m_vecSubOccu[i];
			break;
		}
	}
	// ��ת�ڵ��ְҵID�Ϸ�
	if(stChangeOccu.nID >= 0 && stChangeOccu.nID <= 21)
	{
		uchar bSOccu = bStart;
		// �ӿ�ʼְҵ����תְҵ·��
		while (bSOccu != stChangeOccu.nID)
		{
			vecOccu.push_back(bSOccu);
			bSOccu = m_vecSubOccu[bSOccu].nParentID;
		}
		// ѹ����תְҵ
		vecOccu.push_back(stChangeOccu.nID);
		// ����תְҵ��Ŀ��ְҵ·��
		vector<uchar> vecROccu;
		bSOccu = bEnd;
		while (bSOccu != stChangeOccu.nID)
		{
			vecROccu.push_back(bSOccu);
			bSOccu = m_vecSubOccu[bSOccu].nParentID;
		}
		// ����ְҵ�б�
		while (!vecROccu.empty())
		{
			vecOccu.push_back(vecROccu.back());
			vecROccu.pop_back();
		}
	}
}
