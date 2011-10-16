


#ifndef _Task_H_
#define _Task_H_
#pragma once
#include "../Utility/gtDef.h"


class CTask
{
public:
	CTask(void);
	~CTask(void);

	void Release();

	bool DecodeFromDataBlock(DBReadSet& dbr);

	BOOL AddOneRoleRecord(tagRoleRecord*& pRole);

	list<tagRoleRecord*>*	GetQuestList(void)	{ return &m_listRoleRecord; }

	BOOL IsRoleBeingByID(long id);

private:
	list<tagRoleRecord*>	m_listRoleRecord;
	map<WORD , tagExcessRecord> m_mapExcessRecord;				//�����¼
	long	m_lChapterID;
	set<LONG>				m_setRoleRecord;					//�����¼
};
#endif//_Task_H_