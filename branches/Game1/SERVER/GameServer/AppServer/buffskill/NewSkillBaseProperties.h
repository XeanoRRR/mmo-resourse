///////////////////////////////////////////////////////////////////////////////
///  NewSkillBaseProperties.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\GameServer\AppServer\BuffSkill
///
///  CREATED: 08/07/2007 12:50:01 PM by ���Ƚ�
///
///  PURPOSE:   ����������
///
//////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_NEWSKILLBASEPROPERTIES_INCLUDED
#define _INC_NEWSKILLBASEPROPERTIES_INCLUDED
#include <vector>
#include <list>
#include <string>
#include <map>

#include "BuffSkillDef.h"
#include "..\..\..\setup\NewSkillList.h"
using namespace std;


class CNewSkill : public CNewSkillProperties
{
private:	
	ModuleType						m_MType;					//����
public:
	CNewSkill(void);
	CNewSkill(DWORD dwSkillID);
	virtual ~CNewSkill(void);

	void							AddNewSkill( tagSkill *pSkill);														//��Ӽ���	
	tagSkill						*GetNewSkill(DWORD dwLevel);														//��ȡ����
	void							Clear();																			//���
	void							ClearCDo(tagCDo *ptgCDo);															//�ͷ�
	void							ClearBaseChange(tagBaseChange *ptgBaseChange);
	void							ClearCondition(void *pCondition,string strType);
	ModuleType						GetModuleType(string strName);														//��������
	void							SetSkillType( string strType ){m_MType = GetModuleType(strType);}					//��ȡ����	
	ModuleType						GetType()				  {return m_MType;}
	void							SetFormat(DWORD dwID, string& strFileName,string& strContend){tagExpression Express = {strFileName,strContend};m_mapFormat[dwID] = Express;}	//���ù�ʽ
	void							SetCondition(DWORD dwID,tagCondition *tgCondtition) {m_mapCondition[dwID] = tgCondtition;}														//��������
	void							SetConGroup(DWORD dwGroupID,tagConditionGroup *ptgConditionGroup){m_mapConditionGroup[dwGroupID]=ptgConditionGroup;}							//����������
	void							SetOperateObject(DWORD dwID,tagOperateObject *ptgOperateObject){m_mapOperateObject[dwID]=ptgOperateObject;}										//�������ö���
	map<DWORD,tagOperateObject*>	GetOperateObject()				{return m_mapOperateObject;}		//���ö����б�
	ModuleType						GetSkillType()					{return m_MType;}					//����
	map<DWORD,tagExpression>&		GetFormat()						{return m_mapFormat;}				//��ȡ��ʽ
	map<DWORD,tagCondition*>		GetCondition()					{return m_mapCondition;}			//����
	map<DWORD,tagConditionGroup*>	GetConditionGroup()				{return m_mapConditionGroup;}		//������
};

#endif//_INC_NEWSKILLBASEPROPERTIES_INCLUDED