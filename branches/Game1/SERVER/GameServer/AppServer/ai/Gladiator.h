#pragma once
#include "MonsterAI.h"
#include "..\monster.h"

/*
*AI��������:�ⲫ��ͨ
*/
class CGladiator :
	public CMonsterAI
{
protected:

	BOOL SearchEnemy();
public:
	CGladiator										();
	~CGladiator										();
	// �������������BUG by kevin lynx
	VOID SetTarget( LONG lType, const CGUID& TargetGuid );
};
