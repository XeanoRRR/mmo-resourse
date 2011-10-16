///
/// @file DHFGladiator.h
/// @author Kevin Lynx
/// @brief ���ȼ�¼-��������AI
///
#pragma once

#include "MonsterAI.h"
#include <list>

///
/// DHF( Distance and Hurted First )���ȹ������Լ�����ҹ������Լ���Ŀ��
///
class CDHFGladiator : public CMonsterAI
{
public:
	typedef std::list<CGUID> TargetListType;
public:
	BOOL SearchEnemy();

	BOOL WhenBeenHurted( LONG lAttackerType, const CGUID &attackerID, DWORD dwHurt );

	void SetTarget( LONG lType, const CGUID &TargetGUID );

	void ResumeAll( bool bForce = false )
	{
		CMonsterAI::ResumeAll( bForce );
		m_Targets.clear();
	}

private:
	TargetListType m_Targets;
};
