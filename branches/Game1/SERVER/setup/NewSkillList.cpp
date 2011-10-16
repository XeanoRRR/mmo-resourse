///////////////////////////////////////////////////////////////////////////////
///  NewSkillList.cpp
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\setup
///
///  CREATED: 08/06/2007 13:49:01 PM by ���Ƚ�
///
///  PURPOSE:   ������
//////////////////////////////////////////////////////////////////////--

#include "stdafx.h"
#include "NewSkillList.h"
#include "../../public/StrUnifyInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CNewSkillProperties::tagChangeProperty::~tagChangeProperty()
//{
//	strName.clear();
//	strOperator.clear();
//	strScriptVarName.clear();
//}

/*
NAME:CState()
DESC:Ĭ�Ϲ��캯��
PARA:��
*/

CNewSkillProperties::CNewSkillProperties()
{
	m_dwID = 0;	
}


/*
NAME:CState(long lStateID)
DESC:���캯��
PARA:lStateIDΪ״̬��ID
*/

CNewSkillProperties::CNewSkillProperties(DWORD dwID)
{
	m_dwID = dwID;
}

/*
NAME:~CNewSkill()
DESC:���캯��
PARA:��
*/

CNewSkillProperties::~CNewSkillProperties()
{
	//Clear();
}

//CNewSkillProperties::tagChangeProperty::~tagChangeProperty()
//{
//	delete strName.c_str();
//}
/*
NAME:AddNewSkill()
DESC:��Ӽ��ܵ���Ҫ�ṹ
PARA:pSkill���ܵ���Ҫ�ṹ
*/
//
void CNewSkillProperties::AddNewSkill( tagSkill *pSkill)
{
	m_lSkill.push_back(pSkill);
}


//�ű�
void	CNewSkillProperties::SetFormat( DWORD dwID, string& strFileName,string& strContend)	
{
	tagExpression Express = {strFileName,strContend};
	m_mapFormat[dwID] = Express;
} 
/*
NAME:SetCondition()
DESC:�������
PARA:dwID����ID
	 tgConditon������
*/
void	CNewSkillProperties::SetCondition( DWORD dwID,tagCondition *tgCondtition)
{
	map<DWORD, tagCondition*>::iterator it = m_mapCondition.find(dwID);
	if (it!=m_mapCondition.end())
	{
		char str[256];
		_snprintf(str, 256, CStringReading::LoadString(IDS_WS_SKILL,STR_WS_SKILL_SECOND),this->GetNewSkillID(),dwID);	
		PutDebugString(str);
		return;
	}
	else
	{
		m_mapCondition[dwID] = tgCondtition;
	}
}

/*
NAME:SetConGroup()
DESC:���������
PARA:dwGroupID������ID
	 ptgConditionGroup������
*/
void CNewSkillProperties::SetConGroup(DWORD dwGroupID,tagConditionGroup *ptgConditionGroup)
{
	map<DWORD,tagConditionGroup*>::iterator it = m_mapConditionGroup.find(dwGroupID);
	if (it!=m_mapConditionGroup.end())
	{
		char str[256];		
		_snprintf(str, 256, CStringReading::LoadString(IDS_WS_SKILL,STR_WS_SKILL_FIRST),this->GetNewSkillID(),dwGroupID);	
		PutDebugString(str);
		return;
		
	} 
	else
	{
		m_mapConditionGroup[dwGroupID]=ptgConditionGroup;
	}
 
}

/*
NAME:GetNewSkill()
DESC:ͨ���ȼ���ȡ��Ӧ���ܵ���Ҫ�ṹ
PARA:dwLevel���ܵȼ�
*/
CNewSkillProperties::tagSkill *CNewSkillProperties::GetNewSkill(DWORD dwLevel)
{
	if (m_lSkill.size()==0)
	{
		return NULL;
	}
	list<tagSkill*>::iterator it=m_lSkill.begin();
	for (; it!=m_lSkill.end(); ++it)
	{
		if (dwLevel == (*it)->dwLevel)
		{
			tagSkill *ptgSkill = *it;
			return ptgSkill;
		}
	}
	return NULL;
}

CNewSkillProperties::stStaticParam	*CNewSkillProperties::GetStaticParam(DWORD dwLevel)	
{
	CNewSkillProperties::tagSkill *ptgSkill = GetNewSkill(dwLevel);
	if (ptgSkill)
	{
		return ptgSkill->pStaticParam;
	}
	return NULL;
}				

