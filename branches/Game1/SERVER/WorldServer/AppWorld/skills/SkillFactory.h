#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_SKILL_FACTORY_INCLUDED
#define _INC_SKILL_FACTORY_INCLUDED
#include "..\..\..\..\Public\SkillRelated.h"

class CSkill;
class CSkillFactory
{
private:
	//##Skill����ʱ�б�
	static map<DWORD, CSkill*>					g_mSkillMap;
	static map<string, tagSkillUsage>			g_mSkillUsage;

	//##��ȡ����
	static HRESULT			LoadConfigration	( LONG lSkillID, LONG lLevel, CSkill* pSkill );

	static HRESULT			LoadConfigration	( const char* p );

	static HRESULT			LoadUsage			( const char* p );

public:
	//##����һ���µ�Skill�࣬�������ID�ͼ������еļ�������
	//##��ͨ���ļ���ȡ��
	static CSkill*			QuerySkill			( LONG lSkillID, LONG lLevel );

	static VOID				ClearSkillCache		();

	static BOOL				LoadSkillCache		();

	static BOOL				Serialize			( vector<BYTE>& vRet );

	static BOOL				LoadUsageCache		();

	static VOID				ClearUsageCache		();

	static tagSkillUsage	StringToUsage		( CHAR* pStr );
};

#endif
