



#ifndef _Skill_Mgr_H_
#define _Skill_Mgr_H_
#pragma once
#include <list>
using namespace std;
#include "../Utility/gtDef.h"


class CSkillMgr
{
public:
	CSkillMgr(void);
	~CSkillMgr(void);

	void	Release();

	void	ClearSkillList();

	void	ClearOccuSkill();

	//	�����
	void	CodeToByteArray(vector<BYTE>* pByteArray);

	void	DecodeFromDataBlock(DBReadSet& dbRead);

	tagPlayerSkill*	GetSkill(DWORD dwID);

	bool	AddSkill(DWORD dwID);

	void	AddOccuSkill(DWORD dwID);

	void	RemoveSkill(DWORD dwID);

	void	RemoveOccuSkill(DWORD dwID);

	BOOL	QuerySkill(DWORD dwID);

	BOOL	QueryOccuSkill(DWORD dwID);

	bool	IsAddOccupList(DWORD dwSkillID);

	list<tagPlayerSkill>&		GetSkillList()				{return m_listSkillID;}

private:
	//���＼��
	list<tagPlayerSkill>		m_listSkillID;	
	//	ְҵ�����б�
	list<DWORD>		m_listOccuSkill;
};
#endif//_Skill_Mgr_H_